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

#include "btif/core/core_pairing_manager.h"

#include <fcntl.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include "alarm.h"
#include "btif/common/btif_constants.h"
#include "btif/common/btif_task_scheduler.h"
#include "btif/core/core_callbacks.h"
#include "btif/core/core_device_store.h"
#include "btif/core/core_discovery_manager.h"
#include "btif/core/core_sdp_discovery.h"
#include "btif/core/core_util.h"
#include "btm.h"
#include "gap_le_if.h"
#include "hci/cmd/hci_cmd.h"
#include "hci/hci_def_controller_baseband_cmd.h"
#include "hci/hci_error.h"
#include "log.h"
#include "securec.h"

namespace {

struct BondKeyInfo {
    std::array<uint8_t, GAP_LINKKEY_SIZE> linkKey = {};
    uint8_t keyType = 0;
};

constexpr uint64_t BTIF_LE_CONNECT_TIMEOUT_MS = 8000;

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

struct LeConnectWatchdog {
    RawAddress address;
    AlarmPtr alarm;
};

}  // namespace

struct CorePairingManager::State {
    std::mutex bondMutex;
    std::map<std::string, BondKeyInfo> bondKeys;
    std::set<std::string> pairedAddresses;
    std::map<std::string, bt_bond_state_t> bondStates;
    bool authCallbackRegistered = false;
    bool bondStoreLoaded = false;
    bool pairedStoreLoaded = false;
    GapAuthenticationCallback authCallbacks = {};
    GapLePairCallback lePairCallbacks = {};
    std::set<std::string> leAddresses;
    std::map<std::string, uint8_t> leAddressTypes;
    std::set<std::string> brAddresses;
    std::set<std::string> pendingLePairs;
    std::set<std::string> leAclConnected;
    std::map<std::string, std::unique_ptr<LeConnectWatchdog>> leWatchdogs;
    std::map<std::string, std::string> removedNames;
};

CorePairingManager::CorePairingManager() : state_(std::make_unique<State>())
{
}

CorePairingManager::~CorePairingManager() = default;

CorePairingManager &::CorePairingManager::GetInstance()
{
    static CorePairingManager instance;
    return instance;
}

CorePairingManager::State &CorePairingManager::GetState()
{
    return *state_;
}

namespace {

bool IsLeAddress(const std::string &key)
{
    std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
    return ::CorePairingManager::GetInstance().GetState().leAddresses.find(key) !=
           ::CorePairingManager::GetInstance().GetState().leAddresses.end();
}

void RememberTransport(const RawAddress &address, bool le, uint8_t addressType = 0)
{
    std::string key = CoreUtil::AddressKey(address);
    std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
    if (le) {
        if (::CorePairingManager::GetInstance().GetState().brAddresses.find(key) ==
            ::CorePairingManager::GetInstance().GetState().brAddresses.end()) {
            ::CorePairingManager::GetInstance().GetState().leAddresses.insert(key);
            ::CorePairingManager::GetInstance().GetState().leAddressTypes[key] = addressType;
        }
        return;
    }
    ::CorePairingManager::GetInstance().GetState().brAddresses.insert(key);
    ::CorePairingManager::GetInstance().GetState().leAddresses.erase(key);
    ::CorePairingManager::GetInstance().GetState().leAddressTypes.erase(key);
}

uint8_t GetLeAddressType(const std::string &key)
{
    std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
    auto iter = ::CorePairingManager::GetInstance().GetState().leAddressTypes.find(key);
    if (iter == ::CorePairingManager::GetInstance().GetState().leAddressTypes.end()) {
        return BT_PUBLIC_DEVICE_ADDRESS;
    }
    return iter->second;
}

BtAddr ToLeBtAddr(const RawAddress &address, const std::string &key)
{
    BtAddr result = CoreUtil::ToBtAddr(address);
    result.type = GetLeAddressType(key);
    return result;
}

void LoadPairedStore()
{
    if (::CorePairingManager::GetInstance().GetState().pairedStoreLoaded) {
        return;
    }
    ::CorePairingManager::GetInstance().GetState().pairedStoreLoaded = true;

    auto records = ::CoreDeviceStore::Snapshot();
    std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
    int count = 0;
    for (const auto &record : records) {
        if (record.paired) {
            ::CorePairingManager::GetInstance().GetState().pairedAddresses.insert(
                std::string(reinterpret_cast<const char *>(record.address.data()), RawAddress::kLength));
            count++;
        }
    }
    HILOGI("paired address store loaded count=%{public}d", count);
}

void UpdatePairedRecords(std::vector<bluetooth::btif::DeviceStoreRecord> &records, const std::set<std::string> &paired)
{
    std::set<std::string> existing;
    for (auto &record : records) {
        std::string key(reinterpret_cast<const char *>(record.address.data()), RawAddress::kLength);
        existing.insert(key);
        record.paired = paired.find(key) != paired.end();
    }
    for (const auto &key : paired) {
        if (existing.find(key) == existing.end() && key.size() == RawAddress::kLength) {
            bluetooth::btif::DeviceStoreRecord record;
            if (memcpy_s(record.address.data(), record.address.size(), key.data(), RawAddress::kLength) != EOK) {
                HILOGE("copy paired store address failed");
                continue;
            }
            record.paired = true;
            records.push_back(record);
        }
    }
}

void SavePairedStore()
{
    std::set<std::string> paired;
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        paired = ::CorePairingManager::GetInstance().GetState().pairedAddresses;
    }

    (void)::CoreDeviceStore::Update([paired](std::vector<bluetooth::btif::DeviceStoreRecord> &records) {
        UpdatePairedRecords(records, paired);
    });
}

void RememberPairedAddress(const RawAddress &address)
{
    LoadPairedStore();
    bool changed = false;
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        std::string key(reinterpret_cast<const char *>(address.address), RawAddress::kLength);
        changed = ::CorePairingManager::GetInstance().GetState().pairedAddresses.insert(key).second;
    }
    // Persist even when the address was already in memory. A previous save may
    // have failed or been interrupted, and a stale empty file must be repaired.
    (void)changed;
    SavePairedStore();
}

void RemovePairedAddress(const RawAddress &address)
{
    LoadPairedStore();
    bool changed = false;
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        std::string key(reinterpret_cast<const char *>(address.address), RawAddress::kLength);
        changed = ::CorePairingManager::GetInstance().GetState().pairedAddresses.erase(key) > 0;
    }
    if (changed) {
        SavePairedStore();
    }
}

void LoadBondStore()
{
    if (::CorePairingManager::GetInstance().GetState().bondStoreLoaded) {
        return;
    }
    ::CorePairingManager::GetInstance().GetState().bondStoreLoaded = true;
    auto records = ::CoreDeviceStore::Snapshot();
    std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
    int count = 0;
    for (const auto &record : records) {
        if (!record.hasBond) {
            continue;
        }
        std::string key(reinterpret_cast<const char *>(record.address.data()), RawAddress::kLength);
        BondKeyInfo info;
        if (memcpy_s(info.linkKey.data(), info.linkKey.size(), record.linkKey.data(), record.linkKey.size()) != EOK) {
            HILOGE("load bond store link key failed");
            continue;
        }
        info.keyType = record.keyType;
        ::CorePairingManager::GetInstance().GetState().bondKeys[key] = info;
        count++;
    }
    HILOGI("bond store loaded count=%{public}d source=unified", count);
}

void UpdateExistingBondRecords(
    std::vector<bluetooth::btif::DeviceStoreRecord> &records, const std::map<std::string, BondKeyInfo> &bonds)
{
    for (auto &record : records) {
        std::string key(reinterpret_cast<const char *>(record.address.data()), RawAddress::kLength);
        auto iter = bonds.find(key);
        if (iter == bonds.end()) {
            record.hasBond = false;
            record.keyType = 0;
            record.linkKey.fill(0);
            continue;
        }
        record.hasBond = true;
        record.keyType = iter->second.keyType;
        if (memcpy_s(record.linkKey.data(), record.linkKey.size(), iter->second.linkKey.data(),
            iter->second.linkKey.size()) != EOK) {
            HILOGE("copy stored link key failed");
            continue;
        }
    }
}

void AppendMissingBondRecords(
    std::vector<bluetooth::btif::DeviceStoreRecord> &records, const std::map<std::string, BondKeyInfo> &bonds)
{
    std::set<std::string> existing;
    for (const auto &record : records) {
        existing.insert(std::string(reinterpret_cast<const char *>(record.address.data()), RawAddress::kLength));
    }
    for (const auto &entry : bonds) {
        if (existing.find(entry.first) != existing.end() || entry.first.size() != RawAddress::kLength) {
            continue;
        }
        bluetooth::btif::DeviceStoreRecord record;
        if (memcpy_s(record.address.data(), record.address.size(), entry.first.data(), RawAddress::kLength) != EOK) {
            HILOGE("copy bond store address failed");
            continue;
        }
        record.hasBond = true;
        record.keyType = entry.second.keyType;
        if (memcpy_s(record.linkKey.data(), record.linkKey.size(), entry.second.linkKey.data(),
            entry.second.linkKey.size()) != EOK) {
            HILOGE("copy bond store link key failed");
            continue;
        }
        records.push_back(record);
    }
}

void SaveBondStore()
{
    std::map<std::string, BondKeyInfo> bonds;
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        bonds = ::CorePairingManager::GetInstance().GetState().bondKeys;
    }
    (void)::CoreDeviceStore::Update([bonds](std::vector<bluetooth::btif::DeviceStoreRecord> &records) {
        UpdateExistingBondRecords(records, bonds);
        AppendMissingBondRecords(records, bonds);
    });
}

bool NotifyBondState(bt_status_t status, const RawAddress &address, bt_bond_state_t state)
{
    if (state == BT_BOND_STATE_BONDED || state == BT_BOND_STATE_NONE) {
        ::CoreDiscoveryManager::SetPairingMode(false);
    }
    std::string key = CoreUtil::AddressKey(address);
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        auto iter = ::CorePairingManager::GetInstance().GetState().bondStates.find(key);
        if (iter != ::CorePairingManager::GetInstance().GetState().bondStates.end() && iter->second == state) {
            return false;
        }
        ::CorePairingManager::GetInstance().GetState().bondStates[key] = state;
    }
    RawAddress callbackAddress = address;
    if (!bluetooth::btif::TaskScheduler::Post(
        bluetooth::btif::BtifTaskLane::CALLBACK,
        [status, callbackAddress, state]() {
            RawAddress reportAddress = callbackAddress;
            CoreCallbackRegistry::NotifyBondState(status, &reportAddress, state);
        })) {
        HILOGW("drop bond state callback: callback lane unavailable");
        return false;
    }
    return true;
}

bool HasBondKey(const RawAddress &address)
{
    std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
    return ::CorePairingManager::GetInstance().GetState().bondKeys.find(CoreUtil::AddressKey(address)) !=
           ::CorePairingManager::GetInstance().GetState().bondKeys.end();
}

std::string TakeRemovedName(const RawAddress &address)
{
    std::string key = CoreUtil::AddressKey(address);
    std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
    auto iter = ::CorePairingManager::GetInstance().GetState().removedNames.find(key);
    if (iter == ::CorePairingManager::GetInstance().GetState().removedNames.end()) {
        return {};
    }
    std::string name = iter->second;
    ::CorePairingManager::GetInstance().GetState().removedNames.erase(iter);
    return name;
}

void PublishBonded(const RawAddress &address)
{
    RememberPairedAddress(address);
    std::string removedName = TakeRemovedName(address);
    if (!removedName.empty()) {
        std::string currentName;
        if (!::CoreDeviceStore::GetName(address, currentName) || currentName.empty()) {
            ::CoreDeviceStore::RememberName(
                address, reinterpret_cast<const uint8_t *>(removedName.data()), removedName.size());
        }
    }
    std::string name;
    if (!::CoreDeviceStore::GetName(address, name) || name.empty()) {
        ::CoreDiscoveryManager::RequestRemoteName(address);
    }
    if (!NotifyBondState(BT_STATUS_SUCCESS, address, BT_BOND_STATE_BONDED)) {
        return;
    }
    // SDP_ServiceSearchAttribute blocks waiting for the SDP task. Keep it off
    // the GAP task, and only run it once when BONDED is published. LE devices
    // do not publish BR/EDR SDP profiles.
    if (IsLeAddress(CoreUtil::AddressKey(address))) {
        return;
    }
    RawAddress sdpAddress = address;
    if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CONTROL, [sdpAddress]() {
            ::CoreSdpDiscovery::OnBonded(sdpAddress);
        })) {
        HILOGW("drop bonded SDP query: control lane unavailable");
    }
}

void OnLeConnectTimeout(void *parameter)
{
    LeConnectWatchdog *watchdog = static_cast<LeConnectWatchdog *>(parameter);
    if (watchdog == nullptr) {
        return;
    }
    std::string key = CoreUtil::AddressKey(watchdog->address);
    std::unique_ptr<LeConnectWatchdog> holder;
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        auto iter = ::CorePairingManager::GetInstance().GetState().leWatchdogs.find(key);
        if (iter == ::CorePairingManager::GetInstance().GetState().leWatchdogs.end() ||
            iter->second.get() != watchdog) {
            return;
        }
        holder = std::move(iter->second);
        ::CorePairingManager::GetInstance().GetState().leWatchdogs.erase(iter);
        ::CorePairingManager::GetInstance().GetState().pendingLePairs.erase(key);
    }
    HILOGW("LE ACL connect timeout addr=%{public}s", watchdog->address.ToString().c_str());
    NotifyBondState(BT_STATUS_FAIL, watchdog->address, BT_BOND_STATE_NONE);
}

void StartLeWatchdog(const RawAddress &address)
{
    auto watchdog = std::make_unique<LeConnectWatchdog>();
    watchdog->address = address;
    watchdog->alarm = AlarmPtr(AlarmCreate("leConnect", false));
    if (watchdog->alarm == nullptr) {
        HILOGE("create LE connect watchdog failed");
        return;
    }

    std::string key = CoreUtil::AddressKey(address);
    LeConnectWatchdog *rawWatchdog = watchdog.get();
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        ::CorePairingManager::GetInstance().GetState().leWatchdogs[key] = std::move(watchdog);
    }

    if (AlarmSet(rawWatchdog->alarm.get(), BTIF_LE_CONNECT_TIMEOUT_MS, OnLeConnectTimeout, rawWatchdog) != 0) {
        HILOGE("arm LE connect watchdog failed");
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        ::CorePairingManager::GetInstance().GetState().leWatchdogs.erase(key);
    }
}

void CancelLeWatchdog(const std::string &key)
{
    std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
    ::CorePairingManager::GetInstance().GetState().leWatchdogs.erase(key);
}

void CancelAllLeWatchdogs()
{
    std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
    ::CorePairingManager::GetInstance().GetState().leWatchdogs.clear();
}

void StartLePairOnControl(const RawAddress &address)
{
    std::string key = CoreUtil::AddressKey(address);
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        if (::CorePairingManager::GetInstance().GetState().pendingLePairs.find(key) ==
            ::CorePairingManager::GetInstance().GetState().pendingLePairs.end()) {
            return;
        }
    }
    BtAddr btAddr = ToLeBtAddr(address, key);
    int result = GAPIF_LePair(&btAddr);
    HILOGI("create LE bond result=%{public}d", result);
    if (result != BT_SUCCESS && result != BT_ALREADY) {
        CancelLeWatchdog(key);
        {
            std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
            ::CorePairingManager::GetInstance().GetState().pendingLePairs.erase(key);
        }
        NotifyBondState(BT_STATUS_FAIL, address, BT_BOND_STATE_NONE);
    }
}

void HandleLeConnectRequest(const RawAddress &address, const std::string &key, bool connected)
{
    if (connected) {
        CancelLeWatchdog(key);
        StartLePairOnControl(address);
        return;
    }
    StartLeWatchdog(address);
    BtAddr btAddr = ToLeBtAddr(address, key);
    int result = BTM_LeConnect(&btAddr);
    HILOGI("create LE ACL result=%{public}d", result);
    if (result != BT_SUCCESS && result != BT_ALREADY) {
        CancelLeWatchdog(key);
        {
            std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
            ::CorePairingManager::GetInstance().GetState().pendingLePairs.erase(key);
        }
        NotifyBondState(BT_STATUS_FAIL, address, BT_BOND_STATE_NONE);
    }
}

void RequestLePair(const RawAddress &address)
{
    std::string key = CoreUtil::AddressKey(address);
    bool connected = false;
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        ::CorePairingManager::GetInstance().GetState().pendingLePairs.insert(key);
        connected = ::CorePairingManager::GetInstance().GetState().leAclConnected.find(key) !=
                    ::CorePairingManager::GetInstance().GetState().leAclConnected.end();
    }

    auto task = std::bind(HandleLeConnectRequest, address, key, connected);
    if (!bluetooth::btif::TaskScheduler::Post(
        bluetooth::btif::BtifTaskLane::CONTROL, task)) {
        HILOGW("drop LE bond request: control lane unavailable");
        CancelLeWatchdog(key);
        {
            std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
            ::CorePairingManager::GetInstance().GetState().pendingLePairs.erase(key);
        }
        NotifyBondState(BT_STATUS_FAIL, address, BT_BOND_STATE_NONE);
    }
}

void StartBrAuthentication(const RawAddress &address)
{
    // Give the controller time to leave Inquiry/Scanning after CancelBtDiscovery.
    // Creating an ACL while Inquiry is still active is rejected with 0x0C
    // Command Disallowed.
    usleep(bluetooth::btif::kLeConnectStartDelayUs);
    if (!::CoreDiscoveryManager::IsPairingMode()) {
        return;
    }
    BtAddr btAddr = CoreUtil::ToBtAddr(address);
    int result = GAPIF_AuthenticationReq(&btAddr);
    HILOGI("create BR bond request result=%{public}d", result);
    if (result != BT_SUCCESS && result != BT_ALREADY) {
        NotifyBondState(BT_STATUS_FAIL, address, BT_BOND_STATE_NONE);
    }
}

void OnLePairComplete(const BtAddr *addr, uint8_t result, uint8_t keyType, void *context)
{
    (void)context;
    if (addr == nullptr) {
        return;
    }
    RawAddress address = CoreUtil::ToRawAddress(*addr);
    std::string key = CoreUtil::AddressKey(address);
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        ::CorePairingManager::GetInstance().GetState().pendingLePairs.erase(key);
    }
    HILOGI("LE pair complete result=0x%{public}02x keyType=%{public}d", result, keyType);
    if (result == BT_SUCCESS) {
        PublishBonded(address);
    } else {
        NotifyBondState(BT_STATUS_FAIL, address, BT_BOND_STATE_NONE);
    }
}

void OnLePairScUserConfirmReq(const BtAddr *addr, uint32_t number, void *context)
{
    (void)context;
    if (addr == nullptr) {
        return;
    }
    RawAddress address = CoreUtil::ToRawAddress(*addr);
    NotifyBondState(BT_STATUS_SUCCESS, address, BT_BOND_STATE_BONDING);
    if (CoreCallbackRegistry::HasSspRequest()) {
        bt_bdname_t name = {};
        if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CALLBACK, [address, name, number]() {
                RawAddress reportAddress = address;
                bt_bdname_t reportName = name;
                CoreCallbackRegistry::NotifySspRequest(
                    &reportAddress, &reportName, 0, BT_SSP_VARIANT_PASSKEY_CONFIRMATION, number);
            })) {
            HILOGW("drop LE confirm request: callback lane unavailable");
        }
        return;
    }
    (void)GAPIF_LePairScUserConfirmRsp(addr, 1);
}

void OnLePairKeyNotify(const BtAddr *addr, LePairedKeys keys, void *context)
{
    (void)keys;
    (void)context;
    if (addr != nullptr) {
        HILOGI("LE pair key notified addr=%{public}s", CoreUtil::ToRawAddress(*addr).ToString().c_str());
    }
}

void OnIOCapabilityRequest(const BtAddr *addr, void *context)
{
    (void)context;
    if (addr == nullptr) {
        return;
    }
    // authReq is the MITM bit only. GAP adds the bonding bits internally,
    // so passing a full Authentication Requirements value produces 0x12.
    int ret = GAPIF_IOCapabilityRsp(
        addr, 1, static_cast<uint8_t>(bluetooth::btif::kBrIoCapability), GAP_OOB_DATA_NOT_PRESENT,
        static_cast<uint8_t>(bluetooth::btif::kBrAuthReq));
    HILOGI("pair IO capability rsp ret=%{public}d", ret);
}

void OnUserConfirmRequest(
    const BtAddr *addr, uint32_t number, int localMitmRequired, int remoteMitmRequired, void *context)
{
    (void)context;
    if (addr == nullptr) {
        return;
    }
    bool hasCallback = CoreCallbackRegistry::HasSspRequest();
    HILOGI(
        "pair user confirm req number=%{public}u localMitm=%{public}d remoteMitm=%{public}d cb=%{public}d", number,
        localMitmRequired, remoteMitmRequired, hasCallback ? 1 : 0);
    // Incoming pairing does not pass through CorePairingCreateBond, so the
    // service layer has not seen BONDING yet. Report it before the dialog,
    // otherwise SetDevicePairingConfirmation rejects with "not in pairing state".
    ::CoreDiscoveryManager::SetPairingMode(true);
    NotifyBondState(BT_STATUS_SUCCESS, CoreUtil::ToRawAddress(*addr), BT_BOND_STATE_BONDING);
    if (hasCallback) {
        RawAddress address = CoreUtil::ToRawAddress(*addr);
        bt_bdname_t name = {};
        if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CALLBACK, [address, name, number]() {
                RawAddress reportAddress = address;
                bt_bdname_t reportName = name;
                CoreCallbackRegistry::NotifySspRequest(
                    &reportAddress, &reportName, 0, BT_SSP_VARIANT_PASSKEY_CONFIRMATION, number);
            })) {
            HILOGW("drop ssp confirm request: callback lane unavailable");
        }
        return;
    }
    (void)GAPIF_UserConfirmRsp(addr, 0);
}

void OnUserPasskeyRequest(const BtAddr *addr, void *context)
{
    (void)context;
    if (addr == nullptr) {
        return;
    }
    ::CoreDiscoveryManager::SetPairingMode(true);
    NotifyBondState(BT_STATUS_SUCCESS, CoreUtil::ToRawAddress(*addr), BT_BOND_STATE_BONDING);
    if (CoreCallbackRegistry::HasSspRequest()) {
        RawAddress address = CoreUtil::ToRawAddress(*addr);
        bt_bdname_t name = {};
        if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CALLBACK, [address, name]() {
                RawAddress reportAddress = address;
                bt_bdname_t reportName = name;
                CoreCallbackRegistry::NotifySspRequest(&reportAddress, &reportName, 0, BT_SSP_VARIANT_PASSKEY_ENTRY, 0);
            })) {
            HILOGW("drop ssp passkey request: callback lane unavailable");
        }
        return;
    }
    (void)GAPIF_UserPasskeyRsp(addr, 0, 0);
}

void OnUserPasskeyNotification(const BtAddr *addr, uint32_t number, void *context)
{
    (void)context;
    if (addr == nullptr) {
        return;
    }
    if (CoreCallbackRegistry::HasSspRequest()) {
        RawAddress address = CoreUtil::ToRawAddress(*addr);
        bt_bdname_t name = {};
        if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CALLBACK, [address, name, number]() {
                RawAddress reportAddress = address;
                bt_bdname_t reportName = name;
                CoreCallbackRegistry::NotifySspRequest(
                    &reportAddress, &reportName, 0, BT_SSP_VARIANT_PASSKEY_NOTIFICATION, number);
            })) {
            HILOGW("drop ssp passkey notification: callback lane unavailable");
        }
    }
}

void OnPinCodeRequest(const BtAddr *addr, void *context)
{
    (void)context;
    if (addr == nullptr) {
        return;
    }
    ::CoreDiscoveryManager::SetPairingMode(true);
    NotifyBondState(BT_STATUS_SUCCESS, CoreUtil::ToRawAddress(*addr), BT_BOND_STATE_BONDING);
    if (CoreCallbackRegistry::HasPinRequest()) {
        RawAddress address = CoreUtil::ToRawAddress(*addr);
        bt_bdname_t name = {};
        if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CALLBACK, [address, name]() {
                RawAddress reportAddress = address;
                bt_bdname_t reportName = name;
                CoreCallbackRegistry::NotifyPinRequest(&reportAddress, &reportName, 0, false);
            })) {
            HILOGW("drop pin request: callback lane unavailable");
        }
        return;
    }
    (void)GAPIF_PinCodeRsp(addr, 0, nullptr, 0);
}

void OnLinkKeyRequest(const BtAddr *addr, void *context)
{
    (void)context;
    if (addr == nullptr) {
        return;
    }
    RawAddress address = CoreUtil::ToRawAddress(*addr);
    BondKeyInfo info;
    bool found = false;
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        auto iter = ::CorePairingManager::GetInstance().GetState().bondKeys.find(CoreUtil::AddressKey(address));
        if (iter != ::CorePairingManager::GetInstance().GetState().bondKeys.end()) {
            info = iter->second;
            found = true;
        }
    }
    std::array<uint8_t, GAP_LINKKEY_SIZE> emptyKey = {};
    int ret = found ? GAPIF_LinkKeyRsp(addr, 1, info.linkKey.data(), info.keyType)
                    : GAPIF_LinkKeyRsp(addr, 0, emptyKey.data(), 0);
    HILOGI("pair link key rsp found=%{public}d ret=%{public}d", found ? 1 : 0, ret);
}

void OnLinkKeyNotification(const BtAddr *addr, const uint8_t linkKey[GAP_LINKKEY_SIZE], uint8_t keyType, void *context)
{
    (void)context;
    if (addr == nullptr) {
        return;
    }
    RawAddress address = CoreUtil::ToRawAddress(*addr);
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        auto &info = ::CorePairingManager::GetInstance().GetState().bondKeys[CoreUtil::AddressKey(address)];
        if (memcpy_s(info.linkKey.data(), info.linkKey.size(), linkKey, GAP_LINKKEY_SIZE) != EOK) {
            HILOGE("store notified link key failed");
            return;
        }
        info.keyType = keyType;
    }
    SaveBondStore();
    HILOGI("pair link key stored type=%{public}d", keyType);
    // The controller may report a non-success Authentication Complete after
    // distribution when the link key was already delivered. A stored link key
    // is the definitive bonding result and must be published here, otherwise
    // the board stays in BONDING while the peer already shows paired.
    PublishBonded(address);
}

void OnAuthenticationComplete(const BtAddr *addr, uint8_t status, void *context)
{
    (void)context;
    if (addr == nullptr) {
        return;
    }
    RawAddress address = CoreUtil::ToRawAddress(*addr);
    HILOGI("pair authentication complete status=0x%{public}02x", status);
    if (status == HCI_SUCCESS || HasBondKey(address)) {
        PublishBonded(address);
    } else {
        NotifyBondState(BT_STATUS_FAIL, address, BT_BOND_STATE_NONE);
    }
}

void OnSimplePairComplete(const BtAddr *addr, uint8_t status, void *context)
{
    (void)context;
    if (addr == nullptr) {
        return;
    }
    RawAddress address = CoreUtil::ToRawAddress(*addr);
    // Simple Pairing Complete only means the SSP stage finished. Link key
    // notification and authentication complete follow; publishing BONDED here
    // makes the board look paired while the phone is still pairing.
    HILOGI("pair simple complete status=0x%{public}02x, wait for authentication", status);
    if (status != HCI_SUCCESS && !HasBondKey(address)) {
        NotifyBondState(BT_STATUS_FAIL, address, BT_BOND_STATE_NONE);
    }
}

bool RegisterPairingCallback()
{
    if (::CorePairingManager::GetInstance().GetState().authCallbackRegistered) {
        return true;
    }
    ::CorePairingManager::GetInstance().GetState().authCallbacks = {};
    ::CorePairingManager::GetInstance().GetState().authCallbacks.userConfirmReq = OnUserConfirmRequest;
    ::CorePairingManager::GetInstance().GetState().authCallbacks.userPasskeyReq = OnUserPasskeyRequest;
    ::CorePairingManager::GetInstance().GetState().authCallbacks.userPasskeyNotification = OnUserPasskeyNotification;
    ::CorePairingManager::GetInstance().GetState().authCallbacks.pinCodeReq = OnPinCodeRequest;
    ::CorePairingManager::GetInstance().GetState().authCallbacks.linkKeyReq = OnLinkKeyRequest;
    ::CorePairingManager::GetInstance().GetState().authCallbacks.linkKeyNotification = OnLinkKeyNotification;
    ::CorePairingManager::GetInstance().GetState().authCallbacks.simplePairComplete = OnSimplePairComplete;
    ::CorePairingManager::GetInstance().GetState().authCallbacks.IOCapabilityReq = OnIOCapabilityRequest;
    ::CorePairingManager::GetInstance().GetState().authCallbacks.authenticationComplete = OnAuthenticationComplete;
    int ret =
        GAPIF_RegisterAuthenticationCallback(&::CorePairingManager::GetInstance().GetState().authCallbacks, nullptr);
    if (ret != BT_SUCCESS) {
        HILOGE("register authentication callback failed ret=%{public}d", ret);
        return false;
    }
    ::CorePairingManager::GetInstance().GetState().lePairCallbacks = {};
    ::CorePairingManager::GetInstance().GetState().lePairCallbacks.lePairScUserConfirmReq = OnLePairScUserConfirmReq;
    ::CorePairingManager::GetInstance().GetState().lePairCallbacks.lePairComplete = OnLePairComplete;
    ::CorePairingManager::GetInstance().GetState().lePairCallbacks.lePairKeyNotify = OnLePairKeyNotify;
    ret = GAPIF_RegisterLePairCallback(&::CorePairingManager::GetInstance().GetState().lePairCallbacks, nullptr);
    if (ret != BT_SUCCESS) {
        HILOGE("register LE pair callback failed ret=%{public}d", ret);
        (void)GAPIF_DeregisterAuthenticationCallback();
        return false;
    }
    ::CorePairingManager::GetInstance().GetState().authCallbackRegistered = true;
    LoadPairedStore();
    LoadBondStore();
    return true;
}

}  // namespace

static int CorePairingSetSecurityMode()
{
    int result = GAPIF_SetSecurityMode(static_cast<GAP_SecurityMode>(bluetooth::btif::kBrSecurityMode));
    HILOGI("set BR security mode=%{public}d ret=%{public}d", bluetooth::btif::kBrSecurityMode, result);
    return result;
}

static bool CorePairingRegisterCallback()
{
    return RegisterPairingCallback();
}

static void CorePairingLoadStore()
{
    LoadPairedStore();
    LoadBondStore();
}

static void CorePairingCleanup()
{
    if (::CorePairingManager::GetInstance().GetState().authCallbackRegistered) {
        (void)GAPIF_DeregisterLePairCallback();
        (void)GAPIF_DeregisterAuthenticationCallback();
        ::CorePairingManager::GetInstance().GetState().authCallbackRegistered = false;
    }
    ::CoreDiscoveryManager::SetPairingMode(false);
    CancelAllLeWatchdogs();
    // Persist the current in-memory bond state before the process exits. The
    // stores are only saved when they were actually loaded, so an early cleanup
    // cannot overwrite a valid file with an empty map.
    if (::CorePairingManager::GetInstance().GetState().pairedStoreLoaded) {
        SavePairedStore();
    }
    if (::CorePairingManager::GetInstance().GetState().bondStoreLoaded) {
        SaveBondStore();
    }
    std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
    ::CorePairingManager::GetInstance().GetState().bondKeys.clear();
    ::CorePairingManager::GetInstance().GetState().pairedAddresses.clear();
    ::CorePairingManager::GetInstance().GetState().bondStates.clear();
    ::CorePairingManager::GetInstance().GetState().bondStoreLoaded = false;
    ::CorePairingManager::GetInstance().GetState().pairedStoreLoaded = false;
}

static bool CorePairingIsBonded(const RawAddress &address)
{
    LoadPairedStore();
    LoadBondStore();
    std::string key = CoreUtil::AddressKey(address);
    std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
    return ::CorePairingManager::GetInstance().GetState().pairedAddresses.find(key) !=
               ::CorePairingManager::GetInstance().GetState().pairedAddresses.end() ||
           ::CorePairingManager::GetInstance().GetState().bondKeys.find(key) !=
               ::CorePairingManager::GetInstance().GetState().bondKeys.end();
}

static void CorePairingRememberLeAddress(const RawAddress &address, uint8_t addressType)
{
    RememberTransport(address, true, addressType);
}

static void CorePairingRememberBrAddress(const RawAddress &address)
{
    RememberTransport(address, false);
}

static void CorePairingOnLeAclConnected(const RawAddress &address)
{
    std::string key = CoreUtil::AddressKey(address);
    bool pending = false;
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        ::CorePairingManager::GetInstance().GetState().leAclConnected.insert(key);
        pending = ::CorePairingManager::GetInstance().GetState().pendingLePairs.find(key) !=
                  ::CorePairingManager::GetInstance().GetState().pendingLePairs.end();
    }
    CancelLeWatchdog(key);
    if (!pending) {
        return;
    }
    // This callback runs on the GAP task. GAPIF_LePair blocks on the GAP task,
    // so calling it here returns BT_BAD_STATUS (-1001) and drops the pairing
    // even though the peer later completes SMP encryption. Continue on CONTROL.
    if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CONTROL, [address]() {
            StartLePairOnControl(address);
        })) {
        HILOGW("drop LE pair after ACL: control lane unavailable");
        {
            std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
            ::CorePairingManager::GetInstance().GetState().pendingLePairs.erase(key);
        }
        NotifyBondState(BT_STATUS_FAIL, address, BT_BOND_STATE_NONE);
    }
}

static void CorePairingOnLeAclDisconnected(const RawAddress &address)
{
    std::string key = CoreUtil::AddressKey(address);
    CancelLeWatchdog(key);
    std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
    ::CorePairingManager::GetInstance().GetState().leAclConnected.erase(key);
    ::CorePairingManager::GetInstance().GetState().pendingLePairs.erase(key);
}

static size_t CorePairingGetBondedDevices(RawAddress *devices, size_t maxDevices)
{
    if (devices == nullptr || maxDevices == 0) {
        return 0;
    }
    LoadPairedStore();
    std::set<std::string> addresses;
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        for (const auto &entry : ::CorePairingManager::GetInstance().GetState().bondKeys) {
            addresses.insert(entry.first);
        }
        addresses.insert(
            ::CorePairingManager::GetInstance().GetState().pairedAddresses.begin(),
            ::CorePairingManager::GetInstance().GetState().pairedAddresses.end());
    }

    size_t count = 0;
    for (const auto &entry : addresses) {
        if (count >= maxDevices) {
            break;
        }
        if (entry.size() != RawAddress::kLength) {
            continue;
        }
        if (memcpy_s(devices[count].address, sizeof(devices[count].address), entry.data(), RawAddress::kLength) !=
            EOK) {
            HILOGE("fill bonded device address failed");
            continue;
        }
        count++;
    }
    return count;
}

static void CorePairingNotifyBondedDevices()
{
    LoadPairedStore();
    std::set<std::string> addresses;
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        for (const auto &entry : ::CorePairingManager::GetInstance().GetState().bondKeys) {
            addresses.insert(entry.first);
        }
        addresses.insert(
            ::CorePairingManager::GetInstance().GetState().pairedAddresses.begin(),
            ::CorePairingManager::GetInstance().GetState().pairedAddresses.end());
    }

    std::vector<RawAddress> devices;
    for (const auto &entry : addresses) {
        if (entry.size() != RawAddress::kLength) {
            continue;
        }
        RawAddress address;
        if (memcpy_s(address.address, sizeof(address.address), entry.data(), RawAddress::kLength) != EOK) {
            HILOGE("fill notify bonded device address failed");
            continue;
        }
        devices.push_back(address);
    }
    for (const auto &address : devices) {
        NotifyBondState(BT_STATUS_SUCCESS, address, BT_BOND_STATE_BONDED);
    }
    HILOGI("notified bonded devices count=%{public}d", static_cast<int>(devices.size()));
}

static int CorePairingCreateBond(const RawAddress *bdAddr, int transport)
{
    if (bdAddr == nullptr) {
        return BT_STATUS_PARM_INVALID;
    }
    std::string key = CoreUtil::AddressKey(*bdAddr);
    bool lePair = transport == BT_TRANSPORT_LE || ((transport <= 0) && IsLeAddress(key));

    // Pairing and Inquiry cannot reliably coexist on all controllers:
    // Create_Connection is rejected with 0x0C while Inquiry is still active.
    // Stop discovery, hold the discovery lock, then start authentication after
    // the controller has had time to leave Inquiry.
    ::CoreDiscoveryManager::SetPairingMode(true);
    ::CoreDiscoveryManager::Stop();

    int bondModeResult = GAPIF_SetBondableMode(1);
    HILOGI(
        "create bond set bondable ret=%{public}d transport=%{public}d le=%{public}d", bondModeResult, transport,
        lePair ? 1 : 0);
    NotifyBondState(BT_STATUS_SUCCESS, *bdAddr, BT_BOND_STATE_BONDING);

    if (lePair) {
        RequestLePair(*bdAddr);
        return BT_STATUS_SUCCESS;
    }

    RawAddress pairAddress = *bdAddr;
    if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CONTROL, [pairAddress]() {
            StartBrAuthentication(pairAddress);
        })) {
        HILOGW("drop BR bond request: control lane unavailable");
        NotifyBondState(BT_STATUS_FAIL, *bdAddr, BT_BOND_STATE_NONE);
        return BT_STATUS_FAIL;
    }
    return BT_STATUS_SUCCESS;
}

static int CorePairingRemoveBond(const RawAddress *bdAddr)
{
    if (bdAddr == nullptr) {
        return BT_STATUS_PARM_INVALID;
    }
    std::string key = CoreUtil::AddressKey(*bdAddr);
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        ::CorePairingManager::GetInstance().GetState().bondKeys.erase(key);
        ::CorePairingManager::GetInstance().GetState().bondStates.erase(key);
    }
    SaveBondStore();
    RemovePairedAddress(*bdAddr);
    ::CoreSdpDiscovery::OnUnpaired(*bdAddr);

    std::string removedName;
    if (::CoreDeviceStore::GetName(*bdAddr, removedName) && !removedName.empty()) {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        ::CorePairingManager::GetInstance().GetState().removedNames[CoreUtil::AddressKey(*bdAddr)] = removedName;
    }
    ::CoreDeviceStore::ForgetName(*bdAddr);

    std::array<uint8_t, bluetooth::btif::BTIF_DEVICE_ADDRESS_SIZE> storeAddress = {};
    if (memcpy_s(storeAddress.data(), storeAddress.size(), bdAddr->address, RawAddress::kLength) != EOK) {
        HILOGE("copy remove bond address failed");
        return BT_STATUS_FAIL;
    }
    ::CoreDeviceStore::Remove(storeAddress);

    HciDeleteStoredLinkKeyParam param = {};
    if (memcpy_s(param.bdAddr.raw, sizeof(param.bdAddr.raw), bdAddr->address, BD_ADDR_LENGTH) != EOK) {
        HILOGE("fill delete stored link key param failed");
        return BT_STATUS_PARM_INVALID;
    }
    param.deleteAllFlag = 0;
    HciCmd *cmd = HciAllocCmd(HCI_DELETE_STORED_LINK_KEY, &param, sizeof(param));
    if (cmd != nullptr) {
        (void)HciSendCmd(cmd);
    } else {
        HILOGE("remove bond: allocate delete stored link key command failed");
    }
    HILOGI("remove bond request");
    NotifyBondState(BT_STATUS_SUCCESS, *bdAddr, BT_BOND_STATE_NONE);
    return BT_STATUS_SUCCESS;
}

static int CorePairingCancelBond(const RawAddress *bdAddr)
{
    if (bdAddr == nullptr) {
        return BT_STATUS_PARM_INVALID;
    }
    BtAddr address = CoreUtil::ToBtAddr(*bdAddr);
    std::string key = CoreUtil::AddressKey(*bdAddr);
    CancelLeWatchdog(key);
    {
        std::lock_guard<std::mutex> lock(::CorePairingManager::GetInstance().GetState().bondMutex);
        ::CorePairingManager::GetInstance().GetState().pendingLePairs.erase(key);
    }
    ::CoreDiscoveryManager::SetPairingMode(false);
    (void)GAPIF_CancelAuthenticationReq(&address);
    HILOGI("cancel bond request");
    NotifyBondState(BT_STATUS_SUCCESS, *bdAddr, BT_BOND_STATE_NONE);
    return BT_STATUS_SUCCESS;
}

static int CorePairingPinReply(const RawAddress *bdAddr, uint8_t accept, uint8_t pinLen, bt_pin_code_t *pinCode)
{
    if (bdAddr == nullptr) {
        return BT_STATUS_PARM_INVALID;
    }
    BtAddr address = CoreUtil::ToBtAddr(*bdAddr);
    const uint8_t *code = (pinCode != nullptr) ? pinCode->pin : nullptr;
    int result = GAPIF_PinCodeRsp(&address, accept, code, pinLen);
    HILOGI("pin reply accept=%{public}d len=%{public}d ret=%{public}d", accept, pinLen, result);
    return result == BT_SUCCESS ? BT_STATUS_SUCCESS : BT_STATUS_FAIL;
}

static int CorePairingSspReply(const RawAddress *bdAddr, bt_ssp_variant_t variant, uint8_t accept, uint32_t passkey)
{
    if (bdAddr == nullptr) {
        return BT_STATUS_PARM_INVALID;
    }
    BtAddr address = CoreUtil::ToBtAddr(*bdAddr);
    bool lePair = IsLeAddress(CoreUtil::AddressKey(*bdAddr));
    int result = BT_SUCCESS;
    switch (variant) {
        case BT_SSP_VARIANT_PASSKEY_CONFIRMATION:
        case BT_SSP_VARIANT_CONSENT:
            result = lePair ? GAPIF_LePairScUserConfirmRsp(&address, accept) : GAPIF_UserConfirmRsp(&address, accept);
            break;
        case BT_SSP_VARIANT_PASSKEY_ENTRY:
            result = lePair ? GAPIF_LePairPassKeyRsp(&address, accept, passkey)
                            : GAPIF_UserPasskeyRsp(&address, accept, passkey);
            break;
        case BT_SSP_VARIANT_PASSKEY_NOTIFICATION:
            result = BT_SUCCESS;
            break;
        default:
            result = BT_OPERATION_FAILED;
            break;
    }
    HILOGI("ssp reply variant=%{public}d accept=%{public}d ret=%{public}d", static_cast<int>(variant), accept, result);
    return result == BT_SUCCESS ? BT_STATUS_SUCCESS : BT_STATUS_FAIL;
}
int CorePairingManager::SetSecurityMode()
{
    return CorePairingSetSecurityMode();
}
bool CorePairingManager::RegisterCallback()
{
    return CorePairingRegisterCallback();
}
void CorePairingManager::Cleanup()
{
    CorePairingCleanup();
}
void CorePairingManager::LoadStore()
{
    CorePairingLoadStore();
}
size_t CorePairingManager::GetBondedDevices(RawAddress *devices, size_t maxDevices)
{
    return CorePairingGetBondedDevices(devices, maxDevices);
}
bool CorePairingManager::IsBonded(const RawAddress &address)
{
    return CorePairingIsBonded(address);
}
void CorePairingManager::NotifyBondedDevices()
{
    CorePairingNotifyBondedDevices();
}
int CorePairingManager::CreateBond(const RawAddress *bdAddr, int transport)
{
    return CorePairingCreateBond(bdAddr, transport);
}
int CorePairingManager::RemoveBond(const RawAddress *bdAddr)
{
    return CorePairingRemoveBond(bdAddr);
}
int CorePairingManager::CancelBond(const RawAddress *bdAddr)
{
    return CorePairingCancelBond(bdAddr);
}
void CorePairingManager::RememberLeAddress(const RawAddress &address, uint8_t addressType)
{
    CorePairingRememberLeAddress(address, addressType);
}
void CorePairingManager::RememberBrAddress(const RawAddress &address)
{
    CorePairingRememberBrAddress(address);
}
void CorePairingManager::OnLeAclConnected(const RawAddress &address)
{
    CorePairingOnLeAclConnected(address);
}
void CorePairingManager::OnLeAclDisconnected(const RawAddress &address)
{
    CorePairingOnLeAclDisconnected(address);
}
int CorePairingManager::PinReply(const RawAddress *bdAddr, uint8_t accept, uint8_t pinLen, bt_pin_code_t *pinCode)
{
    return CorePairingPinReply(bdAddr, accept, pinLen, pinCode);
}
int CorePairingManager::SspReply(const RawAddress *bdAddr, bt_ssp_variant_t variant, uint8_t accept, uint32_t passkey)
{
    return CorePairingSspReply(bdAddr, variant, accept, passkey);
}
