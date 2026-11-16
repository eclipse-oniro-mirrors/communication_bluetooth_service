/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "btif/core/core_discovery_manager.h"

#include <algorithm>
#include <atomic>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <functional>
#include <map>
#include <mutex>
#include <set>
#include <string>

#include "btif/common/btif_constants.h"
#include "btif/common/btif_task_scheduler.h"
#include "btif/core/core_ble_scanner.h"
#include "btif/core/core_callbacks.h"
#include "btif/core/core_device_store.h"
#include "btif/core/core_discovery_state_machine.h"
#include "btif/core/core_pairing_manager.h"
#include "btif/core/core_util.h"
#include "gap_comm.h"
#include "gap_if.h"
#include "log.h"
#include "securec.h"

using bluetooth::btif::CoreDiscoveryStateMachine;

namespace {

void LogName(const char *source, const uint8_t *name, size_t length)
{
    std::string text(reinterpret_cast<const char *>(name), length);
    HILOGI("%{public}s length=%{public}d text=%{public}s", source, static_cast<int>(length), text.c_str());
}

}  // namespace

struct CoreDiscoveryManager::State {
    CoreDiscoveryStateMachine discoveryState;
    std::set<std::string> nameRequested;
    std::mutex nameRequestMutex;
    std::atomic<bool> pairingMode{false};
    bool discoveryCallbackRegistered = false;
};

CoreDiscoveryManager::CoreDiscoveryManager() : state_(std::make_unique<State>())
{
}

CoreDiscoveryManager::~CoreDiscoveryManager() = default;

CoreDiscoveryManager &::CoreDiscoveryManager::GetInstance()
{
    static CoreDiscoveryManager instance;
    return instance;
}

CoreDiscoveryManager::State &CoreDiscoveryManager::GetState()
{
    return *state_;
}

namespace {

const uint8_t *FindEirLocalName(const uint8_t eir[GAP_EIR_SIZE_MAX], size_t &nameLength)
{
    size_t offset = 0;
    while (offset < GAP_EIR_SIZE_MAX && eir[offset] != 0) {
        uint8_t length = eir[offset];
        if (offset + static_cast<size_t>(length) + 1 > GAP_EIR_SIZE_MAX) {
            break;
        }
        uint8_t type = eir[offset + 1];
        if ((type == bluetooth::btif::kEirTypeCompleteLocalName ||
             type == bluetooth::btif::kEirTypeShortenedLocalName) &&
            length > 1) {
            nameLength = static_cast<size_t>(length - 1);
            if (nameLength > GAP_NAME_SIZE_MAX) {
                nameLength = GAP_NAME_SIZE_MAX;
            }
            return &eir[offset + 1] + 1;
        }
        offset += static_cast<size_t>(length) + 1;
    }
    return nullptr;
}

void RequestRemoteNameNow(const BtAddr *address)
{
    if (address == nullptr) {
        return;
    }
    std::string key(reinterpret_cast<const char *>(address->addr), RawAddress::kLength);
    {
        std::lock_guard<std::mutex> lock(::CoreDiscoveryManager::GetInstance().GetState().nameRequestMutex);
        if (!::CoreDiscoveryManager::GetInstance().GetState().nameRequested.insert(key).second) {
            return;
        }
    }
    int ret = GAPIF_GetRemoteName(address);
    if (ret != BT_SUCCESS) {
        {
            std::lock_guard<std::mutex> lock(::CoreDiscoveryManager::GetInstance().GetState().nameRequestMutex);
            ::CoreDiscoveryManager::GetInstance().GetState().nameRequested.erase(key);
        }
        HILOGW("remote name request failed ret=%{public}d", ret);
    }
}

struct DeviceFoundEvent {
    RawAddress address;
    bt_bdname_t name = {};
    size_t nameLength = 0;
    uint32_t classOfDevice = 0;
    int8_t rssi = 0;
    bool hasClassOfDevice = false;
    bool hasRssi = false;
    bt_device_type_t deviceType = BT_DEVICE_DEVTYPE_BREDR;
};

bool PostDelegateTask(std::function<void()> task)
{
    return bluetooth::btif::TaskScheduler::Post(
        bluetooth::btif::BtifTaskLane::CALLBACK, std::move(task), bluetooth::btif::BtifTaskPolicy::DROP_OLDEST);
}

void DispatchDeviceFound(std::shared_ptr<DeviceFoundEvent> event)
{
    if (event == nullptr) {
        return;
    }
    if (CoreCallbackRegistry::HasDeviceFound()) {
        std::array<bt_property_t, bluetooth::btif::kDevicePropertyMax> properties = {};
        int count = 0;
        properties[count].type = BT_PROPERTY_BDADDR;
        properties[count].len = sizeof(event->address);
        properties[count].val = &event->address;
        count++;

        if (event->nameLength > 0) {
            properties[count].type = BT_PROPERTY_BDNAME;
            properties[count].len = static_cast<int>(event->nameLength);
            properties[count].val = &event->name;
            count++;
        }
        if (event->hasClassOfDevice) {
            properties[count].type = BT_PROPERTY_CLASS_OF_DEVICE;
            properties[count].len = sizeof(event->classOfDevice);
            properties[count].val = &event->classOfDevice;
            count++;
        }
        properties[count].type = BT_PROPERTY_TYPE_OF_DEVICE;
        properties[count].len = sizeof(event->deviceType);
        properties[count].val = &event->deviceType;
        count++;
        if (event->hasRssi) {
            properties[count].type = BT_PROPERTY_REMOTE_RSSI;
            properties[count].len = sizeof(event->rssi);
            properties[count].val = &event->rssi;
            count++;
        }
        CoreCallbackRegistry::NotifyDeviceFound(count, properties.data());
    }
}

void QueueDeviceFound(const DeviceFoundEvent &source)
{
    auto event = std::make_shared<DeviceFoundEvent>(source);
    if (!PostDelegateTask([event]() { DispatchDeviceFound(event); })) {
        HILOGW("drop discovery callback: callback lane unavailable");
    }
}

void DispatchInquiryComplete(void *parameter)
{
    bool cancelled = static_cast<bool>(reinterpret_cast<uintptr_t>(parameter) != 0);
    BtifStopBleDiscovery();
    if (cancelled) {
        if (::CoreDiscoveryManager::GetInstance().GetState().discoveryState.ConsumePendingStart()) {
            HILOGI("discovery execute pending start");
            ::CoreDiscoveryManager::Start();
        }
        return;
    }
    CoreBootstrapNotifyDiscoveryState(BT_DISCOVERY_STOPPED);
}

void QueueInquiryComplete(bool cancelled)
{
    if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CONTROL, [cancelled]() {
            DispatchInquiryComplete(reinterpret_cast<void *>(cancelled ? 1 : 0));
        })) {
        HILOGW("drop inquiry complete: control lane unavailable");
    }
}

void OnInquiryResult(
    const BtAddr *address, uint32_t classOfDevice, int8_t rssi, bool hasRssi, const uint8_t *deviceName,
    size_t nameLength, void *context)
{
    (void)context;
    if (address == nullptr || !CoreCallbackRegistry::HasDeviceFound()) {
        return;
    }

    DeviceFoundEvent event;
    event.address = CoreUtil::ToRawAddress(*address);
    ::CorePairingManager::RememberBrAddress(event.address);
    event.classOfDevice = classOfDevice;
    event.hasClassOfDevice = true;
    event.rssi = rssi;
    event.hasRssi = hasRssi;
    event.deviceType = BT_DEVICE_DEVTYPE_BREDR;

    if (deviceName != nullptr && nameLength > 0) {
        LogName("eir", deviceName, nameLength);
        ::CoreDeviceStore::RememberName(event.address, deviceName, nameLength);
        event.nameLength = std::min(nameLength, static_cast<size_t>(GAP_NAME_SIZE_MAX));
        if (memcpy_s(event.name.name, sizeof(event.name.name), deviceName, event.nameLength) != EOK) {
            HILOGE("copy inquiry result device name failed");
            return;
        }
    }

    QueueDeviceFound(event);
    if (deviceName == nullptr || nameLength == 0) {
        BtAddr addressCopy = *address;
        if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CONTROL, [addressCopy]() {
                RequestRemoteNameNow(&addressCopy);
            })) {
            HILOGW("drop remote name request: control lane unavailable");
        }
    }
}

void OnInquiryResultNoRssi(const BtAddr *address, uint32_t classOfDevice, void *context)
{
    OnInquiryResult(address, classOfDevice, 0, false, nullptr, 0, context);
}

void OnInquiryResultWithRssi(const BtAddr *address, uint32_t classOfDevice, int8_t rssi, void *context)
{
    OnInquiryResult(address, classOfDevice, rssi, true, nullptr, 0, context);
}

void OnExtendedInquiryResult(
    const BtAddr *address, uint32_t classOfDevice, int8_t rssi, const uint8_t eir[GAP_EIR_SIZE_MAX], void *context)
{
    size_t nameLength = 0;
    const uint8_t *name = FindEirLocalName(eir, nameLength);
    OnInquiryResult(address, classOfDevice, rssi, true, name, nameLength, context);
}

bool ReportRemoteName(const RawAddress &address, const uint8_t *name, size_t nameLength)
{
    bt_bdname_t bdName = {};
    if (memcpy_s(bdName.name, sizeof(bdName.name), name, nameLength) != EOK) {
        HILOGE("copy remote name for property failed");
        return false;
    }
    std::array<bt_property_t, bluetooth::btif::kNamePropertyCount> properties = {};
    properties[0].type = BT_PROPERTY_BDADDR;
    properties[0].len = sizeof(address);
    properties[0].val = const_cast<RawAddress *>(&address);
    properties[1].type = BT_PROPERTY_BDNAME;
    properties[1].len = static_cast<int>(nameLength);
    properties[1].val = &bdName;
    RawAddress callbackAddress = address;
    CoreCallbackRegistry::NotifyRemoteDeviceProperties(
        BT_STATUS_SUCCESS, &callbackAddress, bluetooth::btif::kNamePropertyCount, properties.data());
    return true;
}

void QueueRemoteNameFound(const RawAddress &address, const uint8_t *name, size_t nameLength)
{
    if (!CoreCallbackRegistry::HasDeviceFound()) {
        return;
    }
    DeviceFoundEvent event;
    event.address = address;
    event.nameLength = nameLength;
    if (memcpy_s(event.name.name, sizeof(event.name.name), name, nameLength) != EOK) {
        HILOGE("copy remote name to device found event failed");
        return;
    }
    event.deviceType = BT_DEVICE_DEVTYPE_BREDR;
    QueueDeviceFound(event);
}

void OnRemoteName(uint8_t status, const BtAddr *address, const uint8_t name[GAP_NAME_SIZE_MAX], void *context)
{
    (void)context;
    if (address == nullptr) {
        return;
    }
    std::string key(reinterpret_cast<const char *>(address->addr), RawAddress::kLength);
    {
        std::lock_guard<std::mutex> lock(::CoreDiscoveryManager::GetInstance().GetState().nameRequestMutex);
        ::CoreDiscoveryManager::GetInstance().GetState().nameRequested.erase(key);
    }
    if (status != BT_SUCCESS || name == nullptr) {
        HILOGW("remote name request failed status=%{public}d", static_cast<int>(status));
        return;
    }

    size_t nameLength = 0;
    while (nameLength < GAP_NAME_SIZE_MAX && name[nameLength] != 0) {
        nameLength++;
    }
    if (nameLength == 0) {
        return;
    }
    LogName("remote", name, nameLength);
    RawAddress reportAddress = CoreUtil::ToRawAddress(*address);
    ::CoreDeviceStore::RememberName(reportAddress, name, nameLength);
    if (ReportRemoteName(reportAddress, name, nameLength)) {
        QueueRemoteNameFound(reportAddress, name, nameLength);
    }
}

void OnInquiryComplete(uint8_t status, void *context)
{
    (void)status;
    (void)context;
    bool wasCancelling = false;
    if (::CoreDiscoveryManager::GetInstance().GetState().discoveryState.Complete(wasCancelling)) {
        QueueInquiryComplete(wasCancelling);
    }
}

void OnBleDeviceFound(const RawAddress &address, const std::string &name, int8_t rssi)
{
    if (!CoreCallbackRegistry::HasDeviceFound() || name.empty()) {
        return;
    }

    DeviceFoundEvent event;
    event.address = address;
    event.nameLength = std::min(name.size(), static_cast<size_t>(GAP_NAME_SIZE_MAX));
    LogName("ble", reinterpret_cast<const uint8_t *>(name.data()), event.nameLength);
    ::CoreDeviceStore::RememberName(event.address, reinterpret_cast<const uint8_t *>(name.data()), event.nameLength);
    if (memcpy_s(event.name.name, sizeof(event.name.name), name.data(), event.nameLength) != EOK) {
        HILOGE("copy ble device name failed");
        return;
    }
    event.rssi = rssi;
    event.hasRssi = true;
    event.deviceType = BT_DEVICE_DEVTYPE_BLE;
    QueueDeviceFound(event);
}

namespace {
const GapDiscoveryCallback kDiscoveryCallback = {
    OnInquiryResultNoRssi, OnInquiryResultWithRssi, OnExtendedInquiryResult, OnRemoteName, OnInquiryComplete,
};
}  // namespace

void RunDiscoveryStart()
{
    if (::CoreDiscoveryManager::GetInstance().GetState().discoveryState.GetState() !=
        CoreDiscoveryStateMachine::State::ACTIVE) {
        return;
    }
    BtifStartBleDiscovery(OnBleDeviceFound);
    if (::CoreDiscoveryManager::GetInstance().GetState().discoveryState.GetState() !=
        CoreDiscoveryStateMachine::State::ACTIVE) {
        BtifStopBleDiscovery();
        return;
    }
    int result = GAPIF_Inquiry(GAP_INQUIRY_MODE_GENERAL, bluetooth::btif::kInquiryLength);
    if (result != BT_SUCCESS && result != BT_ALREADY) {
        BtifStopBleDiscovery();
        HILOGE("discovery start failed result=%{public}d", result);
        (void)::CoreDiscoveryManager::GetInstance().GetState().discoveryState.Stop();
        CoreBootstrapNotifyDiscoveryState(BT_DISCOVERY_STOPPED);
    }
}

void RunDiscoveryCancel()
{
    BtifStopBleDiscovery();
    int result = GAPIF_InquiryCancel();
    if (result != BT_SUCCESS && result != BT_ALREADY) {
        HILOGE("discovery cancel failed result=%{public}d", result);
    } else {
        HILOGI("discovery cancel GAP request completed result=%{public}d", result);
    }
}

}  // namespace

static int CoreSearchStart()
{
    if (::CoreDiscoveryManager::GetInstance().GetState().pairingMode.load()) {
        HILOGI("discovery start deferred while pairing");
        return BT_STATUS_SUCCESS;
    }
    CoreDiscoveryStateMachine::StartResult start =
        ::CoreDiscoveryManager::GetInstance().GetState().discoveryState.Start();
    if (!start.changed) {
        if (start.queued) {
            HILOGI("discovery start queued, state CANCELLING");
        } else {
            HILOGI("discovery start ignored, already ACTIVE");
        }
        return BT_STATUS_SUCCESS;
    }

    // StartBtDiscovery is a synchronous service/Binder call. GAPIF_Inquiry and
    // BtifStartBleDiscovery synchronously wait for the GAP task; doing that on
    // the caller thread can block Settings for 6 seconds while the GAP task is
    // delivering an advertising callback. Report STARTED now and perform the
    // actual GAP work on the dispatch worker.
    CoreBootstrapNotifyDiscoveryState(BT_DISCOVERY_STARTED);
    if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CONTROL, []() { RunDiscoveryStart(); })) {
        HILOGW("drop discovery start: control lane unavailable");
    }
    return BT_STATUS_SUCCESS;
}

static int CoreSearchCancel()
{
    CoreDiscoveryStateMachine::State state = ::CoreDiscoveryManager::GetInstance().GetState().discoveryState.GetState();
    if (state == CoreDiscoveryStateMachine::State::IDLE) {
        HILOGI("discovery cancel ignored, already IDLE");
        if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CONTROL, []() {
                BtifStopBleDiscovery();
            })) {
            HILOGW("drop discovery stop: control lane unavailable");
        }
        return BT_STATUS_SUCCESS;
    }
    if (state == CoreDiscoveryStateMachine::State::CANCELLING ||
        !::CoreDiscoveryManager::GetInstance().GetState().discoveryState.Cancel()) {
        HILOGI("discovery cancel ignored, already CANCELLING");
        return BT_STATUS_SUCCESS;
    }

    // Settings calls CancelBtDiscovery synchronously. GAPIF_InquiryCancel is a
    // blocking GAP task request; waiting for it here can deadlock the Settings
    // binder thread when the GAP task is delivering a discovery callback.
    // Report STOPPED immediately and perform the actual GAP cleanup off-thread.
    CoreBootstrapNotifyDiscoveryState(BT_DISCOVERY_STOPPED);
    if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CONTROL, []() { RunDiscoveryCancel(); })) {
        HILOGW("drop discovery cancel: control lane unavailable");
    }
    return BT_STATUS_SUCCESS;
}

static bool CoreSearchRegisterCallback()
{
    if (::CoreDiscoveryManager::GetInstance().GetState().discoveryCallbackRegistered) {
        return true;
    }
    int result = GAPIF_RegisterDiscoveryCallback(&kDiscoveryCallback, nullptr);
    if (result != BT_SUCCESS && result != BT_ALREADY) {
        HILOGE("register discovery callback failed result=%{public}d", result);
        return false;
    }
    ::CoreDiscoveryManager::GetInstance().GetState().discoveryCallbackRegistered = true;
    return true;
}

static void CoreSearchUnregisterCallback()
{
    if (::CoreDiscoveryManager::GetInstance().GetState().discoveryCallbackRegistered) {
        GAPIF_DeregisterDiscoveryCallback();
        ::CoreDiscoveryManager::GetInstance().GetState().discoveryCallbackRegistered = false;
    }
    ::CoreDiscoveryManager::GetInstance().GetState().nameRequested.clear();
}

static void CoreSearchStop()
{
    BtifStopBleDiscovery();
    CoreDiscoveryStateMachine::State state = ::CoreDiscoveryManager::GetInstance().GetState().discoveryState.Stop();
    ::CoreDiscoveryManager::GetInstance().GetState().nameRequested.clear();
    if (state != CoreDiscoveryStateMachine::State::IDLE) {
        // Also cancel while CANCELLING: the controller may still be in
        // Inquiry even though the logical state has already advanced.
        GAPIF_InquiryCancel();
        CoreBootstrapNotifyDiscoveryState(BT_DISCOVERY_STOPPED);
    }
}

static void CoreSearchSetPairingMode(bool enabled)
{
    bool previous = ::CoreDiscoveryManager::GetInstance().GetState().pairingMode.exchange(enabled);
    if (previous != enabled) {
        HILOGI("pairing discovery lock %{public}s", enabled ? "on" : "off");
    }
}

static bool CoreSearchIsPairingMode()
{
    return ::CoreDiscoveryManager::GetInstance().GetState().pairingMode.load();
}
int CoreDiscoveryManager::Start()
{
    return CoreSearchStart();
}
int CoreDiscoveryManager::Cancel()
{
    return CoreSearchCancel();
}
bool CoreDiscoveryManager::RegisterCallback()
{
    return CoreSearchRegisterCallback();
}
void CoreDiscoveryManager::UnregisterCallback()
{
    CoreSearchUnregisterCallback();
}
void CoreDiscoveryManager::Stop()
{
    CoreSearchStop();
}
void CoreDiscoveryManager::SetPairingMode(bool enabled)
{
    CoreSearchSetPairingMode(enabled);
}
bool CoreDiscoveryManager::IsPairingMode()
{
    return CoreSearchIsPairingMode();
}

void CoreDiscoveryManager::RequestRemoteName(const RawAddress &address)
{
    BtAddr btAddress = CoreUtil::ToBtAddr(address);
    if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CONTROL, [btAddress]() {
            RequestRemoteNameNow(&btAddress);
        })) {
        HILOGW("drop remote name request: control lane unavailable");
    }
}
