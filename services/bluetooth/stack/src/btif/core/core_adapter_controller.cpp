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

#include "btif/core/core_adapter_controller.h"

#include <algorithm>
#include <array>
#include <functional>
#include <cstring>
#include <map>
#include <mutex>
#include <string>

#include "alarm.h"
#include "btif/common/btif_constants.h"
#include "btif/common/btif_task_scheduler.h"
#include "btif/core/core_callbacks.h"
#include "btif/core/core_device_store.h"
#include "btif/core/core_discovery_manager.h"
#include "btif/core/core_pairing_manager.h"
#include "btif/core/core_sdp_discovery.h"
#include "btif/core/core_util.h"
#include "btm.h"
#include "btstack.h"
#include "gap_comm.h"
#include "gap_if.h"
#include "hci/hci_error.h"
#include "log.h"
#include "securec.h"

namespace {

/* BDADDR + BDNAME: the property array that remote_device_properties_cb expects
 * when reporting a bonded device. */
constexpr int PROPERTY_NUM_BDADDR_BDNAME = 2;

struct AclConnectionInfo {
    RawAddress address;
    tBT_TRANSPORT transport;
};

}  // namespace

constexpr uint64_t BTIF_NAME_REPLAY_DELAY_MS = 800;

struct AlarmDeleter {
    void operator()(Alarm *alarm) const
    {
        if (alarm != nullptr) {
            AlarmCancel(alarm);
            AlarmDelete(alarm);
        }
    }
};

using AlarmPtr = std::unique_ptr<Alarm, AlarmDeleter>;

struct CoreAdapterController::State {
    CoreProfileInterfaceProvider profileInterfaceProvider = nullptr;
    bt_scan_mode_t scanMode = BT_SCAN_MODE_CONNECTABLE;
    std::array<char, GAP_NAME_SIZE_MAX + 1> localName = {};
    size_t localNameLength = 0;
    bt_os_callouts_t *osCallouts = nullptr;
    bool btmInitialized = false;
    bool aclCallbackRegistered = false;
    std::mutex aclMutex;
    std::map<uint16_t, AclConnectionInfo> aclConnections;
    std::mutex nameReplayMutex;
    AlarmPtr nameReplayAlarm;
};

CoreAdapterController::CoreAdapterController() : state_(std::make_unique<State>())
{
}

CoreAdapterController::~CoreAdapterController() = default;

CoreAdapterController &::CoreAdapterController::GetInstance()
{
    static CoreAdapterController instance;
    return instance;
}

CoreAdapterController::State &CoreAdapterController::GetState()
{
    return *state_;
}

namespace {

void NotifyAdapterState(bt_state_t state)
{
    CoreCallbackRegistry::NotifyAdapterState(state);
}

/*
 * Give the service layer the persisted bond list before ClassicAdapter
 * StartUp reads AdapterProperties. The service does not own link-key storage,
 * so btif's bond store remains the single source of truth.
 */
void NotifyBondedDevicesProperty()
{
    if (!CoreCallbackRegistry::HasAdapterProperties()) {
        return;
    }
    std::array<RawAddress, bluetooth::btif::kBondedDevicesMax> devices = {};
    size_t count = ::CorePairingManager::GetBondedDevices(devices.data(), devices.size());
    if (count == 0) {
        HILOGI("no persisted bonded device to report");
        return;
    }
    bt_property_t property = {};
    property.type = BT_PROPERTY_ADAPTER_BONDED_DEVICES;
    property.len = static_cast<int>(count * sizeof(RawAddress));
    property.val = devices.data();
    CoreCallbackRegistry::NotifyAdapterProperties(BT_STATUS_SUCCESS, 1, &property);
    HILOGI("reported bonded devices property count=%{public}d", static_cast<int>(count));
}

void NotifyBondedDeviceNames()
{
    if (!CoreCallbackRegistry::HasRemoteDeviceProperties()) {
        return;
    }
    std::array<RawAddress, bluetooth::btif::kBondedDevicesMax> devices = {};
    size_t count = ::CorePairingManager::GetBondedDevices(devices.data(), devices.size());
    for (size_t index = 0; index < count; index++) {
        std::string name;
        if (!::CoreDeviceStore::GetName(devices[index], name) || name.empty()) {
            ::CoreDiscoveryManager::RequestRemoteName(devices[index]);
            continue;
        }
        size_t nameLength = std::min(name.size(), static_cast<size_t>(GAP_NAME_SIZE_MAX));
        bt_bdname_t bdName = {};
        if (memcpy_s(bdName.name, sizeof(bdName.name), name.data(), nameLength) != EOK) {
            HILOGE("copy bonded device name failed");
            continue;
        }

        std::array<bt_property_t, bluetooth::btif::kNamePropertyCount> properties = {};
        properties[0].type = BT_PROPERTY_BDADDR;
        properties[0].len = sizeof(devices[index]);
        properties[0].val = &devices[index];
        properties[1].type = BT_PROPERTY_BDNAME;
        properties[1].len = static_cast<int>(nameLength);
        properties[1].val = &bdName;
        RawAddress reportAddress = devices[index];
        CoreCallbackRegistry::NotifyRemoteDeviceProperties(
            BT_STATUS_SUCCESS, &reportAddress, PROPERTY_NUM_BDADDR_BDNAME, properties.data());
        HILOGI(
            "reported bonded device name addr=%{public}s nameLen=%{public}d", reportAddress.ToLogString().c_str(),
            static_cast<int>(nameLength));
    }
}

void CancelBondedNameReplay()
{
    AlarmPtr alarm;
    {
        std::lock_guard<std::mutex> lock(::CoreAdapterController::GetInstance().GetState().nameReplayMutex);
        alarm = std::move(::CoreAdapterController::GetInstance().GetState().nameReplayAlarm);
    }
}

void OnBondedNameReplay(void *parameter)
{
    Alarm *alarm = static_cast<Alarm *>(parameter);
    {
        std::lock_guard<std::mutex> lock(::CoreAdapterController::GetInstance().GetState().nameReplayMutex);
        if (::CoreAdapterController::GetInstance().GetState().nameReplayAlarm.get() != alarm) {
            return;
        }
        (void)::CoreAdapterController::GetInstance().GetState().nameReplayAlarm.release();
    }
    AlarmDelete(alarm);
    if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CALLBACK, []() {
            NotifyBondedDeviceNames();
        })) {
        HILOGW("drop bonded device name replay: callback lane unavailable");
    }
}

void ScheduleBondedNameReplay()
{
    CancelBondedNameReplay();
    AlarmPtr alarm(AlarmCreate("bondedNameReplay", false));
    if (alarm == nullptr) {
        HILOGE("create bonded name replay alarm failed");
        return;
    }
    {
        std::lock_guard<std::mutex> lock(::CoreAdapterController::GetInstance().GetState().nameReplayMutex);
        ::CoreAdapterController::GetInstance().GetState().nameReplayAlarm = std::move(alarm);
    }
    if (AlarmSet(
        ::CoreAdapterController::GetInstance().GetState().nameReplayAlarm.get(), BTIF_NAME_REPLAY_DELAY_MS,
            OnBondedNameReplay, ::CoreAdapterController::GetInstance().GetState().nameReplayAlarm.get()) != 0) {
        HILOGE("arm bonded name replay alarm failed");
        std::lock_guard<std::mutex> lock(::CoreAdapterController::GetInstance().GetState().nameReplayMutex);
        ::CoreAdapterController::GetInstance().GetState().nameReplayAlarm.reset();
        return;
    }
    HILOGI("scheduled bonded device name replay delay=%{public}dms", static_cast<int>(BTIF_NAME_REPLAY_DELAY_MS));
}

void NotifyAclState(
    bt_status_t status, const RawAddress &address, bt_acl_state_t state, uint8_t reason, tBT_TRANSPORT transport)
{
    {
        RawAddress callbackAddress = address;
        CoreCallbackRegistry::NotifyAclState(
            status, &callbackAddress, state, static_cast<bt_hci_error_code_t>(reason), transport);
    }
}

void OnBrAclConnectionComplete(const BtmAclConnectCompleteParam *param, void *context)
{
    (void)context;
    if (param == nullptr || param->addr == nullptr) {
        return;
    }
    RawAddress address = CoreUtil::ToRawAddress(*param->addr);
    if (param->status != HCI_SUCCESS) {
        NotifyAclState(BT_STATUS_FAIL, address, BT_ACL_STATE_DISCONNECTED, param->status, BT_TRANSPORT_BR_EDR);
        return;
    }
    {
        std::lock_guard<std::mutex> lock(::CoreAdapterController::GetInstance().GetState().aclMutex);
        ::CoreAdapterController::GetInstance().GetState().aclConnections[param->connectionHandle] = {
            address, BT_TRANSPORT_BR_EDR};
    }
    HILOGI("ACL BR connected handle=0x%{public}04x", param->connectionHandle);
    NotifyAclState(BT_STATUS_SUCCESS, address, BT_ACL_STATE_CONNECTED, 0, BT_TRANSPORT_BR_EDR);
}

void OnBrAclDisconnectionComplete(uint8_t status, uint16_t connectionHandle, uint8_t reason, void *context)
{
    (void)context;
    RawAddress address;
    bool found = false;
    {
        std::lock_guard<std::mutex> lock(::CoreAdapterController::GetInstance().GetState().aclMutex);
        auto iter = ::CoreAdapterController::GetInstance().GetState().aclConnections.find(connectionHandle);
        if (iter != ::CoreAdapterController::GetInstance().GetState().aclConnections.end()) {
            address = iter->second.address;
            ::CoreAdapterController::GetInstance().GetState().aclConnections.erase(iter);
            found = true;
        }
    }
    if (found) {
        HILOGI("ACL BR disconnected handle=0x%{public}04x reason=0x%{public}02x", connectionHandle, reason);
        NotifyAclState(
            status == HCI_SUCCESS ? BT_STATUS_SUCCESS : BT_STATUS_FAIL, address, BT_ACL_STATE_DISCONNECTED, reason,
            BT_TRANSPORT_BR_EDR);
    }
}

void OnLeAclConnectionComplete(
    uint8_t status, uint16_t connectionHandle, const BtAddr *addr, uint8_t role, void *context)
{
    (void)role;
    (void)context;
    if (addr == nullptr) {
        return;
    }
    RawAddress address = CoreUtil::ToRawAddress(*addr);
    if (status != HCI_SUCCESS) {
        NotifyAclState(BT_STATUS_FAIL, address, BT_ACL_STATE_DISCONNECTED, status, BT_TRANSPORT_LE);
        return;
    }
    {
        std::lock_guard<std::mutex> lock(::CoreAdapterController::GetInstance().GetState().aclMutex);
        ::CoreAdapterController::GetInstance().GetState().aclConnections[connectionHandle] = {address, BT_TRANSPORT_LE};
    }
    HILOGI("ACL LE connected handle=0x%{public}04x", connectionHandle);
    NotifyAclState(BT_STATUS_SUCCESS, address, BT_ACL_STATE_CONNECTED, 0, BT_TRANSPORT_LE);
    ::CorePairingManager::OnLeAclConnected(address);
}

void OnLeAclDisconnectionComplete(uint8_t status, uint16_t connectionHandle, uint8_t reason, void *context)
{
    (void)context;
    RawAddress address;
    bool found = false;
    {
        std::lock_guard<std::mutex> lock(::CoreAdapterController::GetInstance().GetState().aclMutex);
        auto iter = ::CoreAdapterController::GetInstance().GetState().aclConnections.find(connectionHandle);
        if (iter != ::CoreAdapterController::GetInstance().GetState().aclConnections.end()) {
            address = iter->second.address;
            ::CoreAdapterController::GetInstance().GetState().aclConnections.erase(iter);
            found = true;
        }
    }
    if (found) {
        HILOGI("ACL LE disconnected handle=0x%{public}04x reason=0x%{public}02x", connectionHandle, reason);
        NotifyAclState(
            status == HCI_SUCCESS ? BT_STATUS_SUCCESS : BT_STATUS_FAIL, address, BT_ACL_STATE_DISCONNECTED, reason,
            BT_TRANSPORT_LE);
        ::CorePairingManager::OnLeAclDisconnected(address);
    }
}

namespace {
const BtmAclCallbacks kAclCallbacks = {
    OnBrAclConnectionComplete,
    OnBrAclDisconnectionComplete,
    OnLeAclConnectionComplete,
    OnLeAclDisconnectionComplete,
    nullptr,
    nullptr,
};
}  // namespace

bool RegisterAclCallback()
{
    if (::CoreAdapterController::GetInstance().GetState().aclCallbackRegistered) {
        return true;
    }
    int ret = BTM_RegisterAclCallbacks(&kAclCallbacks, nullptr);
    if (ret != BT_SUCCESS) {
        HILOGE("register ACL callback failed ret=%{public}d", ret);
        return false;
    }
    ::CoreAdapterController::GetInstance().GetState().aclCallbackRegistered = true;
    return true;
}

bool EnableBtsnoopLog()
{
    const char *path = bluetooth::btif::kBtsnoopFilePath;
    int ret = BTM_SetSnoopFilePath(path, static_cast<uint16_t>(strlen(path)));
    if (ret == BT_SUCCESS) {
        ret = BTM_SetSnoopOutputMaxsize(bluetooth::btif::kBtsnoopMaxMb);
    }
    if (ret == BT_SUCCESS) {
        ret = BTM_EnableSnoopFileOutput(false);
    }
    if (ret != BT_SUCCESS) {
        HILOGE("btsnoop enable failed path=%{public}s ret=%{public}d", path, ret);
        return false;
    }
    HILOGI("btsnoop enabled path=%{public}s maxMB=%{public}d", path, bluetooth::btif::kBtsnoopMaxMb);
    return true;
}

void DisableBtsnoopLog()
{
    int ret = BTM_DisableSnoopFileOutput();
    HILOGI("btsnoop disabled ret=%{public}d", ret);
}

void PublishAdapterProperty(bt_property_t *property)
{
    if (property == nullptr || !CoreCallbackRegistry::HasAdapterProperties()) {
        return;
    }
    CoreCallbackRegistry::NotifyAdapterProperties(BT_STATUS_SUCCESS, 1, property);
}

void PublishScanMode()
{
    bt_scan_mode_t mode = ::CoreAdapterController::GetInstance().GetState().scanMode;
    bt_property_t property = {};
    property.type = BT_PROPERTY_ADAPTER_SCAN_MODE;
    property.len = sizeof(mode);
    property.val = &mode;
    PublishAdapterProperty(&property);
}

void OnScanModeResult(uint8_t status, void *context)
{
    (void)context;
    if (status == BT_SUCCESS) {
        PublishScanMode();
    }
}

bool ApplyScanMode(bt_scan_mode_t mode)
{
    GapDiscoverModeInfo discover = {};
    GapConnectableModeInfo connectable = {};

    discover.type = GAP_INQUIRY_SCAN_TYPE_GENERAL;
    discover.scanInterval = GAP_SCAN_INTERVAL_RANGE_MAX;
    discover.scanWindow = GAP_SCAN_INTERVAL_RANGE_MAX / bluetooth::btif::kScanDutyDenominator;
    connectable.type = GAP_PAGE_SCAN_TYPE_GENERAL;
    connectable.scanInterval = GAP_SCAN_INTERVAL_RANGE_MAX;
    connectable.scanWindow = GAP_SCAN_INTERVAL_RANGE_MAX / bluetooth::btif::kScanDutyDenominator;

    switch (mode) {
        case BT_SCAN_MODE_NONE:
            discover.mode = GAP_DISCOVERABLE_MODE_NON;
            connectable.mode = GAP_CONNECTABLE_MODE_NON;
            break;
        case BT_SCAN_MODE_CONNECTABLE:
            discover.mode = GAP_DISCOVERABLE_MODE_NON;
            connectable.mode = GAP_CONNECTABLE_MODE;
            break;
        case BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE:
            discover.mode = GAP_DISCOVERABLE_MODE_GENERAL;
            connectable.mode = GAP_CONNECTABLE_MODE;
            break;
        default:
            return false;
    }

    int result = GAPIF_SetScanMode(&discover, &connectable, OnScanModeResult, nullptr);
    if (result != BT_SUCCESS) {
        return false;
    }
    ::CoreAdapterController::GetInstance().GetState().scanMode = mode;
    return true;
}

}  // namespace

BtAddr CoreUtil::ToBtAddr(const RawAddress &address)
{
    BtAddr result = {};
    if (memcpy_s(result.addr, sizeof(result.addr), address.address, RawAddress::kLength) != EOK) {
        HILOGE("convert raw address failed");
        return BtAddr{};
    }
    result.type = BT_PUBLIC_DEVICE_ADDRESS;
    return result;
}

RawAddress CoreUtil::ToRawAddress(const BtAddr &address)
{
    RawAddress result;
    if (memcpy_s(result.address, sizeof(result.address), address.addr, RawAddress::kLength) != EOK) {
        HILOGE("convert bt address failed");
        return RawAddress{};
    }
    return result;
}

std::string CoreUtil::AddressKey(const RawAddress &address)
{
    return std::string(reinterpret_cast<const char *>(address.address), RawAddress::kLength);
}

void CoreBootstrapNotifyDiscoveryState(bt_discovery_state_t state)
{
    if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CALLBACK, [state]() {
            CoreCallbackRegistry::NotifyDiscoveryState(state);
        })) {
        HILOGW("drop discovery state callback: callback lane unavailable");
    }
}

static int CoreBootstrapInit(
    bt_callbacks_t *callbacks, bool guestMode, bool isCommonCriteriaMode, int configCompareResult,
    const char **initFlags, bool isAtv)
{
    (void)callbacks;
    (void)guestMode;
    (void)isCommonCriteriaMode;
    (void)configCompareResult;
    (void)initFlags;
    (void)isAtv;
    HILOGI("CoreBootstrapInit enter");
    ::CorePairingManager::LoadStore();
    ::CoreSdpDiscovery::LoadStore();
    NotifyBondedDevicesProperty();
    if (!::CoreAdapterController::GetInstance().GetState().btmInitialized) {
        int initResult = BTM_Initialize();
        HILOGI("BTM_Initialize ret=%{public}d", initResult);
        if (initResult != BT_SUCCESS) {
            HILOGE("BTM_Initialize failed");
            return BT_STATUS_FAIL;
        }
        ::CoreAdapterController::GetInstance().GetState().btmInitialized = true;
    }
    HILOGI("CoreBootstrapInit success");
    return BT_STATUS_SUCCESS;
}

static int CoreBootstrapSetOsCallouts(bt_os_callouts_t *callouts)
{
    ::CoreAdapterController::GetInstance().GetState().osCallouts = callouts;
    return BT_STATUS_SUCCESS;
}

static int ApplyLocalNameOnEnable()
{
    auto &state = ::CoreAdapterController::GetInstance().GetState();
    if (state.localNameLength == 0) {
        const char *defaultName = bluetooth::btif::kDefaultLocalName;
        if (memcpy_s(state.localName.data(), sizeof(state.localName), defaultName, std::strlen(defaultName)) != EOK) {
            HILOGE("copy default local name failed");
            return BT_STATUS_FAIL;
        }
        state.localNameLength = std::strlen(defaultName);
    }
    int nameResult = GAPIF_SetLocalName(state.localName.data(), static_cast<int>(state.localNameLength));
    HILOGI("reapply local name len=%{public}d ret=%{public}d", static_cast<int>(state.localNameLength), nameResult);
    return BT_STATUS_SUCCESS;
}

static void NotifyEnabledAdapters()
{
    NotifyAdapterState(BT_STATE_ON);
    // Addresses are published immediately. Names are replayed after the service
    // and Settings have finished the OFF->ON transition; otherwise the service
    // caches the name while still OFF and suppresses the later same-name event.
    NotifyBondedDevicesProperty();
    ::CorePairingManager::NotifyBondedDevices();
    ::CoreSdpDiscovery::ReportCachedUuids();
    ScheduleBondedNameReplay();
}

static int CoreBootstrapEnable()
{
    auto &state = ::CoreAdapterController::GetInstance().GetState();
    HILOGI("CoreBootstrapEnable enter initialized=%{public}d", state.btmInitialized ? 1 : 0);
    if (!state.btmInitialized) {
        HILOGE("CoreBootstrapEnable rejected: stack not initialized");
        return BT_STATUS_FAIL;
    }

    (void)EnableBtsnoopLog();
    int brResult = BTM_Enable(BREDR_CONTROLLER);
    if (brResult != BT_SUCCESS && brResult != BT_ALREADY) {
        HILOGE("BTM_Enable BR controller failed ret=%{public}d", brResult);
        return BT_STATUS_FAIL;
    }
    int leResult = BTM_Enable(LE_CONTROLLER);
    if (leResult != BT_SUCCESS && leResult != BT_ALREADY) {
        HILOGE("BTM_Enable LE controller failed ret=%{public}d", leResult);
        return BT_STATUS_FAIL;
    }

    int bondableResult = GAPIF_SetBondableMode(1);
    HILOGI("set bondable mode ret=%{public}d", bondableResult);
    if (::CorePairingManager::SetSecurityMode() != BT_SUCCESS) {
        return BT_STATUS_FAIL;
    }
    if (!RegisterAclCallback() || !::CorePairingManager::RegisterCallback() ||
        !::CoreDiscoveryManager::RegisterCallback()) {
        return BT_STATUS_FAIL;
    }
    int nameResult = ApplyLocalNameOnEnable();
    if (nameResult != BT_STATUS_SUCCESS) {
        return nameResult;
    }
    NotifyEnabledAdapters();
    return BT_STATUS_SUCCESS;
}

static int CoreBootstrapDisable()
{
    CancelBondedNameReplay();
    ::CoreDiscoveryManager::Stop();
    if (::CoreAdapterController::GetInstance().GetState().btmInitialized) {
        BTM_Disable(LE_CONTROLLER);
        BTM_Disable(BREDR_CONTROLLER);
    }
    DisableBtsnoopLog();
    NotifyAdapterState(BT_STATE_OFF);
    return BT_STATUS_SUCCESS;
}

static void CoreBootstrapCleanup()
{
    CancelBondedNameReplay();
    ::CoreDiscoveryManager::Stop();
    ::CoreDiscoveryManager::UnregisterCallback();
    ::CoreSdpDiscovery::Cleanup();
    bluetooth::btif::TaskScheduler::Cleanup();
    ::CorePairingManager::Cleanup();

    if (::CoreAdapterController::GetInstance().GetState().aclCallbackRegistered) {
        (void)BTM_DeregisterAclCallbacks(&kAclCallbacks);
        ::CoreAdapterController::GetInstance().GetState().aclCallbackRegistered = false;
    }
    DisableBtsnoopLog();
    if (::CoreAdapterController::GetInstance().GetState().btmInitialized) {
        BTM_Close();
        ::CoreAdapterController::GetInstance().GetState().btmInitialized = false;
    }
    {
        std::lock_guard<std::mutex> lock(::CoreAdapterController::GetInstance().GetState().aclMutex);
        ::CoreAdapterController::GetInstance().GetState().aclConnections.clear();
    }
    ::CoreAdapterController::GetInstance().GetState().osCallouts = nullptr;
}

static int CoreBootstrapGetAdapterProperties()
{
    if (!CoreCallbackRegistry::HasAdapterProperties()) {
        return BT_STATUS_FAIL;
    }

    BtAddr localAddress = {};
    if (BTM_GetLocalAddr(&localAddress) != BT_SUCCESS) {
        return BT_STATUS_FAIL;
    }
    RawAddress rawAddress = CoreUtil::ToRawAddress(localAddress);
    bt_scan_mode_t scanMode = ::CoreAdapterController::GetInstance().GetState().scanMode;

    std::array<bt_property_t, bluetooth::btif::kAdapterPropertyMax> properties = {};
    int count = 0;
    properties[count].type = BT_PROPERTY_BDADDR;
    properties[count].len = sizeof(rawAddress);
    properties[count].val = &rawAddress;
    count++;
    properties[count].type = BT_PROPERTY_ADAPTER_SCAN_MODE;
    properties[count].len = sizeof(scanMode);
    properties[count].val = &scanMode;
    count++;

    std::array<RawAddress, bluetooth::btif::kBondedDevicesMax> bondedDevices = {};
    size_t bondedCount = ::CorePairingManager::GetBondedDevices(bondedDevices.data(), bondedDevices.size());
    if (bondedCount > 0) {
        properties[count].type = BT_PROPERTY_ADAPTER_BONDED_DEVICES;
        properties[count].len = static_cast<int>(bondedCount * sizeof(RawAddress));
        properties[count].val = bondedDevices.data();
        count++;
    }
    HILOGI("adapter properties count=%{public}d bonded=%{public}d", count, static_cast<int>(bondedCount));

    CoreCallbackRegistry::NotifyAdapterProperties(BT_STATUS_SUCCESS, count, properties.data());
    return BT_STATUS_SUCCESS;
}

static int CoreBootstrapGetAdapterProperty(bt_property_type_t type)
{
    if (!CoreCallbackRegistry::HasAdapterProperties()) {
        return BT_STATUS_FAIL;
    }

    bt_property_t property = {};
    if (type == BT_PROPERTY_BDADDR) {
        BtAddr localAddress = {};
        if (BTM_GetLocalAddr(&localAddress) != BT_SUCCESS) {
            return BT_STATUS_FAIL;
        }
        RawAddress rawAddress = CoreUtil::ToRawAddress(localAddress);
        property.type = type;
        property.len = sizeof(rawAddress);
        property.val = &rawAddress;
    } else if (type == BT_PROPERTY_ADAPTER_SCAN_MODE) {
        bt_scan_mode_t scanMode = ::CoreAdapterController::GetInstance().GetState().scanMode;
        property.type = type;
        property.len = sizeof(scanMode);
        property.val = &scanMode;
    } else if (type == BT_PROPERTY_ADAPTER_BONDED_DEVICES) {
        std::array<RawAddress, bluetooth::btif::kBondedDevicesMax> bondedDevices = {};
        size_t bondedCount = ::CorePairingManager::GetBondedDevices(bondedDevices.data(), bondedDevices.size());
        property.type = type;
        property.len = static_cast<int>(bondedCount * sizeof(RawAddress));
        property.val = bondedDevices.data();
        HILOGI("bonded devices property count=%{public}d", static_cast<int>(bondedCount));
    } else {
        HILOGW("%{public}s: unsupported property type=%{public}d", __FUNCTION__, type);
        return BT_STATUS_UNSUPPORTED;
    }

    CoreCallbackRegistry::NotifyAdapterProperties(BT_STATUS_SUCCESS, 1, &property);
    return BT_STATUS_SUCCESS;
}

static void ApplyScanModeAsync(bt_scan_mode_t mode)
{
    if (!ApplyScanMode(mode)) {
        HILOGE("apply scan mode failed mode=%{public}d", static_cast<int>(mode));
        return;
    }
    PublishScanMode();
}

static int SetAdapterScanModeProperty(const bt_property_t *property)
{
    if (property->len != sizeof(bt_scan_mode_t)) {
        return BT_STATUS_PARM_INVALID;
    }
    bt_scan_mode_t mode = *reinterpret_cast<const bt_scan_mode_t *>(property->val);
    // GAPIF_SetScanMode is a blocking GAP task request. Settings calls
    // SetBtScanMode synchronously while leaving the Bluetooth page, so
    // waiting for the GAP task here can block the UI for 6 seconds.
    auto task = std::bind(ApplyScanModeAsync, mode);
    if (!bluetooth::btif::TaskScheduler::Post(
        bluetooth::btif::BtifTaskLane::CONTROL, task)) {
        HILOGW("drop scan mode update: control lane unavailable");
        return BT_STATUS_FAIL;
    }
    return BT_STATUS_SUCCESS;
}

static int SetAdapterLocalNameProperty(const bt_property_t *property)
{
    auto &state = ::CoreAdapterController::GetInstance().GetState();
    if (property->len == 0 || property->len > GAP_NAME_SIZE_MAX) {
        return BT_STATUS_PARM_INVALID;
    }
    if (memset_s(state.localName.data(), sizeof(state.localName), 0, sizeof(state.localName)) != EOK) {
        HILOGE("clear local name buffer failed");
        return BT_STATUS_PARM_INVALID;
    }
    if (memcpy_s(state.localName.data(), sizeof(state.localName), property->val, property->len) != EOK) {
        HILOGE("copy adapter local name failed");
        return BT_STATUS_PARM_INVALID;
    }
    state.localNameLength = property->len;
    if (GAPIF_SetLocalName(state.localName.data(), static_cast<int>(state.localNameLength)) != BT_SUCCESS) {
        return BT_STATUS_FAIL;
    }
    return BT_STATUS_SUCCESS;
}

static int CoreBootstrapSetAdapterProperty(const bt_property_t *property)
{
    if (property == nullptr || property->val == nullptr) {
        return BT_STATUS_PARM_INVALID;
    }
    if (property->type == BT_PROPERTY_ADAPTER_SCAN_MODE) {
        return SetAdapterScanModeProperty(property);
    }
    if (property->type == BT_PROPERTY_BDNAME) {
        return SetAdapterLocalNameProperty(property);
    }
    HILOGW("%{public}s: unsupported property type=%{public}d", __FUNCTION__, property->type);
    return BT_STATUS_UNSUPPORTED;
}

static int CoreBootstrapStartDiscovery()
{
    return ::CoreDiscoveryManager::Start();
}

static int CoreBootstrapCancelDiscovery()
{
    return ::CoreDiscoveryManager::Cancel();
}

static int CoreBootstrapGetRemoteDeviceProperties(RawAddress *remoteAddress)
{
    if (remoteAddress == nullptr) {
        return BT_STATUS_PARM_INVALID;
    }
    std::string name;
    if (!::CoreDeviceStore::GetName(*remoteAddress, name) || name.empty()) {
        HILOGW("%{public}s: no cached name", __FUNCTION__);
        ::CoreDiscoveryManager::RequestRemoteName(*remoteAddress);
        if (CoreCallbackRegistry::HasRemoteDeviceProperties()) {
            CoreCallbackRegistry::NotifyRemoteDeviceProperties(BT_STATUS_UNSUPPORTED, remoteAddress, 0, nullptr);
        }
        return BT_STATUS_UNSUPPORTED;
    }

    bt_bdname_t deviceName = {};
    size_t nameLength = name.size();
    if (nameLength > GAP_NAME_SIZE_MAX) {
        nameLength = GAP_NAME_SIZE_MAX;
    }
    if (memcpy_s(deviceName.name, sizeof(deviceName.name), name.data(), nameLength) != EOK) {
        HILOGE("copy remote device name failed");
        return BT_STATUS_PARM_INVALID;
    }

    std::array<bt_property_t, bluetooth::btif::kNamePropertyCount> properties = {};
    properties[0].type = BT_PROPERTY_BDADDR;
    properties[0].len = sizeof(*remoteAddress);
    properties[0].val = remoteAddress;
    properties[1].type = BT_PROPERTY_BDNAME;
    properties[1].len = static_cast<int>(nameLength);
    properties[1].val = &deviceName;

    HILOGI(
        "remote device properties addr=%{public}s nameLen=%{public}d", remoteAddress->ToLogString().c_str(),
        static_cast<int>(nameLength));
    if (CoreCallbackRegistry::HasRemoteDeviceProperties()) {
        CoreCallbackRegistry::NotifyRemoteDeviceProperties(
            BT_STATUS_SUCCESS, remoteAddress, PROPERTY_NUM_BDADDR_BDNAME, properties.data());
    }
    return BT_STATUS_SUCCESS;
}

static int CoreBootstrapCreateBond(const RawAddress *bdAddr, int transport)
{
    return ::CorePairingManager::CreateBond(bdAddr, transport);
}

static int CoreBootstrapRemoveBond(const RawAddress *bdAddr)
{
    return ::CorePairingManager::RemoveBond(bdAddr);
}

static int CoreBootstrapCancelBond(const RawAddress *bdAddr)
{
    return ::CorePairingManager::CancelBond(bdAddr);
}

static int CoreBootstrapGetConnectionState(const RawAddress *bdAddr)
{
    if (bdAddr == nullptr) {
        return 0;
    }
    std::lock_guard<std::mutex> lock(::CoreAdapterController::GetInstance().GetState().aclMutex);
    for (const auto &entry : ::CoreAdapterController::GetInstance().GetState().aclConnections) {
        if (entry.second.address == *bdAddr) {
            return 1;
        }
    }
    return 0;
}

static int CoreBootstrapCreateAclConnection(const RawAddress *bdAddr)
{
    if (bdAddr == nullptr) {
        return BT_STATUS_PARM_INVALID;
    }
    BtAddr address = CoreUtil::ToBtAddr(*bdAddr);
    int result = BTM_AclConnect(&address);
    HILOGI("create ACL connection result=%{public}d", result);
    return (result == BT_SUCCESS || result == BT_ALREADY) ? BT_STATUS_SUCCESS : BT_STATUS_FAIL;
}

static int CoreBootstrapGetRemoteDeviceProperty(RawAddress *remoteAddress, bt_property_type_t type)
{
    (void)remoteAddress;
    HILOGW("%{public}s: type=%{public}d not implemented", __FUNCTION__, type);
    return BT_STATUS_UNSUPPORTED;
}

static int CoreBootstrapSetRemoteDeviceProperty(RawAddress *remoteAddress, const bt_property_t *property)
{
    (void)remoteAddress;
    (void)property;
    HILOGW("%{public}s: not implemented", __FUNCTION__);
    return BT_STATUS_UNSUPPORTED;
}

static int CoreBootstrapGetRemoteServiceRecord(const RawAddress &remoteAddress, const bluetooth::Uuid &uuid)
{
    (void)remoteAddress;
    (void)uuid;
    HILOGW("%{public}s: not implemented", __FUNCTION__);
    return BT_STATUS_UNSUPPORTED;
}

static int CoreBootstrapGetRemoteServices(RawAddress *remoteAddress)
{
    if (remoteAddress == nullptr) {
        return BT_STATUS_PARM_INVALID;
    }
    ::CoreSdpDiscovery::QueryRemoteUuids(*remoteAddress, true);
    return BT_STATUS_SUCCESS;
}

static int CoreBootstrapCreateBondOutOfBand(
    const RawAddress *bdAddr, int transport, const bt_oob_data_t *p192Data, const bt_oob_data_t *p256Data)
{
    (void)bdAddr;
    (void)transport;
    (void)p192Data;
    (void)p256Data;
    HILOGW("%{public}s: OOB bonding not implemented", __FUNCTION__);
    return BT_STATUS_UNSUPPORTED;
}

static int CoreBootstrapPinReply(const RawAddress *bdAddr, uint8_t accept, uint8_t pinLen, bt_pin_code_t *pinCode)
{
    return ::CorePairingManager::PinReply(bdAddr, accept, pinLen, pinCode);
}

static int CoreBootstrapSspReply(const RawAddress *bdAddr, bt_ssp_variant_t variant, uint8_t accept, uint32_t passkey)
{
    return ::CorePairingManager::SspReply(bdAddr, variant, accept, passkey);
}

static int CoreBootstrapDutModeConfigure(uint8_t enable)
{
    HILOGW("%{public}s: enable=%{public}d not implemented", __FUNCTION__, enable);
    return BT_STATUS_UNSUPPORTED;
}

static int CoreBootstrapDutModeSend(uint16_t opcode, uint8_t *buf, uint8_t len)
{
    (void)buf;
    HILOGW("%{public}s: opcode=0x%{public}04x len=%{public}d not implemented", __FUNCTION__, opcode, len);
    return BT_STATUS_UNSUPPORTED;
}

static int CoreBootstrapLeTestMode(uint16_t opcode, uint8_t *buf, uint8_t len)
{
    (void)buf;
    HILOGW("%{public}s: opcode=0x%{public}04x len=%{public}d not implemented", __FUNCTION__, opcode, len);
    return BT_STATUS_UNSUPPORTED;
}

static int CoreBootstrapReadEnergyInfo()
{
    HILOGW("%{public}s: not implemented", __FUNCTION__);
    return BT_STATUS_UNSUPPORTED;
}

static void CoreBootstrapDumpMetrics(std::string *output)
{
    (void)output;
    HILOGW("%{public}s: metrics collection not implemented", __FUNCTION__);
}

static int CoreBootstrapConfigClear()
{
    HILOGW("%{public}s: not implemented", __FUNCTION__);
    return BT_STATUS_UNSUPPORTED;
}

static void CoreBootstrapInteropDatabaseClear()
{
    HILOGW("%{public}s: not implemented", __FUNCTION__);
}

static void CoreBootstrapInteropDatabaseAdd(uint16_t feature, const RawAddress *addr, size_t len)
{
    (void)addr;
    (void)len;
    HILOGW("%{public}s: feature=%{public}d not implemented", __FUNCTION__, feature);
}

static bluetooth::avrcp::ServiceInterface *CoreBootstrapGetAvrcpService()
{
    HILOGW("%{public}s: AVRCP service not available", __FUNCTION__);
    return nullptr;
}

static std::string CoreBootstrapObfuscateAddress(const RawAddress &address)
{
    (void)address;
    HILOGW("%{public}s: not implemented", __FUNCTION__);
    return {};
}

static int CoreBootstrapGetMetricId(const RawAddress &address)
{
    (void)address;
    HILOGW("%{public}s: not implemented", __FUNCTION__);
    return -1;
}

static int CoreBootstrapSetDynamicAudioBufferSize(int codec, int size)
{
    (void)codec;
    (void)size;
    HILOGW("%{public}s: codec=%{public}d size=%{public}d not implemented", __FUNCTION__, codec, size);
    return BT_STATUS_UNSUPPORTED;
}

static int CoreBootstrapGenerateLocalOobData(tBT_TRANSPORT transport)
{
    (void)transport;
    HILOGW("%{public}s: not implemented", __FUNCTION__);
    return BT_STATUS_UNSUPPORTED;
}

static void CoreBootstrapEnableBluetoothHighPower(uint8_t highPowerLevel)
{
    HILOGW("%{public}s: level=%{public}d not implemented", __FUNCTION__, highPowerLevel);
}

static void CoreBootstrapEnableFastScan(bool isEnabled)
{
    HILOGW("%{public}s: enabled=%{public}d not implemented", __FUNCTION__, isEnabled ? 1 : 0);
}

static void CoreBootstrapSetFastScan(int level)
{
    HILOGW("%{public}s: level=%{public}d not implemented", __FUNCTION__, level);
}

static void CoreBootstrapCleanupProfiles()
{
    HILOGW("%{public}s: not implemented", __FUNCTION__);
}

static void CoreBootstrapCleanBackgroundConnection()
{
    HILOGW("%{public}s: not implemented", __FUNCTION__);
}

static void CoreBootstrapSendAntennaStatusMsg(bool isFixed)
{
    HILOGW("%{public}s: fixed=%{public}d not implemented", __FUNCTION__, isFixed ? 1 : 0);
}

static void CoreBootstrapSendBleAdvMsg(bool isStarted)
{
    HILOGW("%{public}s: started=%{public}d not implemented", __FUNCTION__, isStarted ? 1 : 0);
}

static bool CoreBootstrapGetBleMacByBrMac(RawAddress *bdAddr, uint8_t *addrType)
{
    (void)bdAddr;
    (void)addrType;
    HILOGW("%{public}s: not implemented", __FUNCTION__);
    return false;
}

static void CoreBootstrapSendBtWearableStatusMsg(bool isConnected)
{
    HILOGI("%{public}s: connected=%{public}d", __FUNCTION__, isConnected ? 1 : 0);
}

static void CoreBootstrapSendBleAdvStartMsg()
{
    HILOGW("%{public}s: not implemented", __FUNCTION__);
}

static void CoreBootstrapSendBleAdvStopMsg()
{
    HILOGW("%{public}s: not implemented", __FUNCTION__);
}

static int CoreBootstrapRegisterRangingAntSwitchCallback(void (*handler)(uint8_t macId, uint8_t antId))
{
    (void)handler;
    HILOGW("%{public}s: not implemented", __FUNCTION__);
    return BT_STATUS_UNSUPPORTED;
}

static void CoreBootstrapUnregisterRangingAntSwitchCallback()
{
    HILOGW("%{public}s: not implemented", __FUNCTION__);
}

static void CoreBootstrapSendAntennaInfoQueryMsg()
{
    HILOGW("%{public}s: not implemented", __FUNCTION__);
}

static bool CoreBootstrapGetBrAddressByBleAddress(RawAddress *randomPseudo, uint8_t *identityAddressType)
{
    (void)randomPseudo;
    (void)identityAddressType;
    HILOGW("%{public}s: not implemented", __FUNCTION__);
    return false;
}

static const void *CoreBootstrapGetProfileInterface(const char *profileId)
{
    const void *interface = ::CoreAdapterController::GetInstance().GetState().profileInterfaceProvider != nullptr
                                ? ::CoreAdapterController::GetInstance().GetState().profileInterfaceProvider(profileId)
                                : nullptr;
    if (interface == nullptr) {
        HILOGW("%{public}s: profile=%{public}s not available", __FUNCTION__, profileId != nullptr ? profileId : "null");
    }
    return interface;
}

static void CoreSetProfileInterfaceProvider(CoreProfileInterfaceProvider provider)
{
    ::CoreAdapterController::GetInstance().GetState().profileInterfaceProvider = provider;
}

static void FillCoreInterface(bt_interface_t *iface)
{
    iface->init = &CoreAdapterController::Init;
    iface->enable = CoreBootstrapEnable;
    iface->disable = CoreBootstrapDisable;
    iface->cleanup = &CoreAdapterController::Cleanup;
    iface->get_adapter_properties = CoreBootstrapGetAdapterProperties;
    iface->get_adapter_property = CoreBootstrapGetAdapterProperty;
    iface->set_adapter_property = CoreBootstrapSetAdapterProperty;
    iface->get_remote_device_properties = CoreBootstrapGetRemoteDeviceProperties;
    iface->get_remote_device_property = CoreBootstrapGetRemoteDeviceProperty;
    iface->set_remote_device_property = CoreBootstrapSetRemoteDeviceProperty;
    iface->get_remote_service_record = CoreBootstrapGetRemoteServiceRecord;
    iface->get_remote_services = CoreBootstrapGetRemoteServices;
    iface->start_discovery = CoreBootstrapStartDiscovery;
    iface->cancel_discovery = CoreBootstrapCancelDiscovery;
    iface->create_bond = CoreBootstrapCreateBond;
    iface->create_bond_out_of_band = CoreBootstrapCreateBondOutOfBand;
    iface->remove_bond = CoreBootstrapRemoveBond;
    iface->cancel_bond = CoreBootstrapCancelBond;
    iface->get_connection_state = CoreBootstrapGetConnectionState;
    iface->create_acl_connection = CoreBootstrapCreateAclConnection;
    iface->pin_reply = CoreBootstrapPinReply;
    iface->ssp_reply = CoreBootstrapSspReply;
    iface->get_profile_interface = CoreBootstrapGetProfileInterface;
    iface->dut_mode_configure = CoreBootstrapDutModeConfigure;
    iface->dut_mode_send = CoreBootstrapDutModeSend;
    iface->le_test_mode = CoreBootstrapLeTestMode;
    iface->set_os_callouts = CoreBootstrapSetOsCallouts;
}

static void FillExtensionInterface(bt_interface_t *iface)
{
    iface->read_energy_info = CoreBootstrapReadEnergyInfo;
    iface->dumpMetrics = CoreBootstrapDumpMetrics;
    iface->config_clear = CoreBootstrapConfigClear;
    iface->interop_database_clear = CoreBootstrapInteropDatabaseClear;
    iface->interop_database_add = CoreBootstrapInteropDatabaseAdd;
    iface->get_avrcp_service = CoreBootstrapGetAvrcpService;
    iface->obfuscate_address = CoreBootstrapObfuscateAddress;
    iface->get_metric_id = CoreBootstrapGetMetricId;
    iface->set_dynamic_audio_buffer_size = CoreBootstrapSetDynamicAudioBufferSize;
    iface->generate_local_oob_data = CoreBootstrapGenerateLocalOobData;
    iface->enable_bluetooth_highpower = CoreBootstrapEnableBluetoothHighPower;
    iface->enable_fast_scan = CoreBootstrapEnableFastScan;
    iface->setFastScan = CoreBootstrapSetFastScan;
    iface->cleanup_profiles = CoreBootstrapCleanupProfiles;
    iface->clean_background_connection = CoreBootstrapCleanBackgroundConnection;
    iface->send_antenna_status_msg = CoreBootstrapSendAntennaStatusMsg;
    iface->send_ble_adv_msg = CoreBootstrapSendBleAdvMsg;
    iface->get_ble_mac_by_br_mac = CoreBootstrapGetBleMacByBrMac;
    iface->send_bt_wearable_status_msg = CoreBootstrapSendBtWearableStatusMsg;
    iface->sendbleadvstartmsg = CoreBootstrapSendBleAdvStartMsg;
    iface->sendbleadvstopmsg = CoreBootstrapSendBleAdvStopMsg;
    iface->register_ranging_ant_switch_callback = CoreBootstrapRegisterRangingAntSwitchCallback;
    iface->unregister_ranging_ant_switch_callback = CoreBootstrapUnregisterRangingAntSwitchCallback;
    iface->send_antenna_info_query_msg = CoreBootstrapSendAntennaInfoQueryMsg;
    iface->get_br_address_by_ble_address = CoreBootstrapGetBrAddressByBleAddress;
}

static void CoreFillInterfaceTable(bt_interface_t *iface)
{
    if (iface == nullptr) {
        return;
    }
    iface->size = sizeof(bt_interface_t);
    FillCoreInterface(iface);
    FillExtensionInterface(iface);
}
int CoreAdapterController::Init(
    bt_callbacks_t *callbacks, bool guestMode, bool isCommonCriteriaMode, int configCompareResult,
    const char **initFlags, bool isAtv)
{
    return CoreBootstrapInit(callbacks, guestMode, isCommonCriteriaMode, configCompareResult, initFlags, isAtv);
}
void CoreAdapterController::Cleanup()
{
    CoreBootstrapCleanup();
}
void CoreAdapterController::FillInterfaceTable(bt_interface_t *iface)
{
    CoreFillInterfaceTable(iface);
}
void CoreAdapterController::SetProfileInterfaceProvider(CoreProfileInterfaceProvider provider)
{
    CoreSetProfileInterfaceProvider(provider);
}
