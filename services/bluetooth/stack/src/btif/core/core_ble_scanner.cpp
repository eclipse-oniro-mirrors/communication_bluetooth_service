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

#include "btif/core/core_ble_scanner.h"

#include <algorithm>
#include <chrono>
#include <functional>
#include <cstdint>
#include <cstring>
#include <map>
#include <mutex>
#include <string>

#include "btif/common/btif_constants.h"
#include "btif/common/btif_task_scheduler.h"
#include "btif/core/core_ble_scan_policy.h"
#include "btif/core/core_pairing_manager.h"
#include "btstack.h"
#include "gap_comm.h"
#include "gap_le_if.h"
#include "hci/hci_error.h"
#include "log.h"
#include "platform/include/alarm.h"
#include "securec.h"

using bluetooth::btif::CoreBleScanPolicy;

namespace {

using BleScanCaller = CoreBleScanPolicy::Caller;
using BleScanParam = CoreBleScanPolicy::Param;

}  // namespace

enum class BleScanMode : uint8_t {
    LEGACY,
    EXTENDED,
};

enum class BleScanState : uint8_t {
    IDLE,
    DISABLING,
    SETTING_PARAM,
    ENABLING,
    SCANNING,
    STOPPING,
};

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

struct CoreBleScanner::State {
    std::mutex scanMutex;
    // Keep advertising-report delivery independent from scanMutex. The scan
    // state machine holds scanMutex while synchronously waiting for a GAP task.
    std::mutex callbackMutex;
    bool scanRegistered = false;
    bool exScanRegistered = false;
    bool scanning = false;
    CoreBleScanPolicy scanPolicy{
        GAP_SCAN_INTERVAL_RANGE_MAX,
        static_cast<uint16_t>(GAP_SCAN_INTERVAL_RANGE_MAX / bluetooth::btif::kScanDutyDenominator),
        GAP_SCAN_WINDOW_RANGE_MAX};
    CoreBleScanPolicy::Caller scanTrigger = CoreBleScanPolicy::Caller::NONE;
    uint8_t scanRetryCount = 0;
    bool extendedRetryTried = false;
    bool scanReconfigure = false;
    bool bleReportLogged = false;
    bool bleRoleSet = false;
    bool scanFault = false;
    BtifBleDiscoveryCallback bleDiscoveryCallback = nullptr;
    std::map<std::string, uint64_t> bleDiscoveryReportTime;
    BleScanMode scanMode = BleScanMode::LEGACY;
    BleScanState scanState = BleScanState::IDLE;
    GapExScanCallback exScanCallback = {};
    AlarmPtr scanStageAlarm;
    uint32_t scanStageGeneration = 0;
};

CoreBleScanner::CoreBleScanner() : state_(std::make_unique<State>())
{
}

CoreBleScanner::~CoreBleScanner() = default;

CoreBleScanner &::CoreBleScanner::GetInstance()
{
    static CoreBleScanner instance;
    return instance;
}

CoreBleScanner::State &CoreBleScanner::GetState()
{
    return *state_;
}

namespace {

RawAddress ToRawAddress(const BtAddr &address)
{
    RawAddress result;
    if (memcpy_s(result.address, sizeof(result.address), address.addr, RawAddress::kLength) != EOK) {
        HILOGE("convert bt address failed");
        return RawAddress{};
    }
    return result;
}

std::string FindAdLocalName(const uint8_t *data, uint8_t length)
{
    size_t offset = 0;
    while (offset < length) {
        uint8_t fieldLength = data[offset];
        if (fieldLength == 0) {
            break;
        }
        if (offset + static_cast<size_t>(fieldLength) + 1 > length) {
            break;
        }
        uint8_t type = data[offset + 1];
        if ((type == bluetooth::btif::kEirTypeCompleteLocalName ||
             type == bluetooth::btif::kEirTypeShortenedLocalName) &&
            fieldLength > 1) {
            return std::string(reinterpret_cast<const char *>(data + offset + 1) + 1, fieldLength - 1);
        }
        offset += static_cast<size_t>(fieldLength) + 1;
    }
    return {};
}

static void DispatchToDiscovery(const RawAddress &rawAddress, const std::string &name, int8_t rssi, uint8_t dataLen)
{
    BtifBleDiscoveryCallback discoveryCallback = nullptr;
    bool reportToDiscovery = false;
    {
        std::lock_guard<std::mutex> lock(::CoreBleScanner::GetInstance().GetState().callbackMutex);
        discoveryCallback = ::CoreBleScanner::GetInstance().GetState().bleDiscoveryCallback;
        if (!::CoreBleScanner::GetInstance().GetState().bleReportLogged) {
            ::CoreBleScanner::GetInstance().GetState().bleReportLogged = true;
            HILOGI(
                "BLE advertising report dataLen=%{public}u nameLen=%{public}d callback=%{public}d", dataLen,
                static_cast<int>(name.size()), discoveryCallback != nullptr ? 1 : 0);
        }
        if (discoveryCallback != nullptr && !name.empty()) {
            std::string key(reinterpret_cast<const char *>(rawAddress.address), RawAddress::kLength);
            auto nowPoint = std::chrono::steady_clock::now().time_since_epoch();
            uint64_t now =
                static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(nowPoint).count());
            auto iter = ::CoreBleScanner::GetInstance().GetState().bleDiscoveryReportTime.find(key);
            if (iter == ::CoreBleScanner::GetInstance().GetState().bleDiscoveryReportTime.end() ||
                now - iter->second >= bluetooth::btif::kBleDiscoveryDedupMs) {
                ::CoreBleScanner::GetInstance().GetState().bleDiscoveryReportTime[key] = now;
                reportToDiscovery = true;
            }
        }
    }
    if (reportToDiscovery) {
        discoveryCallback(rawAddress, name, rssi);
    }
}

static void ReportScanResult(
    uint16_t eventType, uint8_t addressType, RawAddress reportAddress, int8_t rssi, std::vector<uint8_t> data)
{
    const btgatt_scanner_callbacks_t *current = BtifGetGattScannerCallbacks();
    if (current == nullptr || current->scan_result_cb == nullptr) {
        return;
    }
    current->scan_result_cb(eventType, addressType, &reportAddress, 0, 0, 0, 0, rssi, 0, data);
}

static void HandleGattScanResult(
    uint16_t eventType, uint8_t addressType, RawAddress reportAddress, int8_t rssi, std::vector<uint8_t> data)
{
    ::CorePairingManager::RememberLeAddress(reportAddress, addressType);
    ReportScanResult(eventType, addressType, reportAddress, rssi, data);
}

static void DispatchToGattScanner(
    uint8_t advType, const BtAddr *address, const GapAdvReportParam &report, const RawAddress &rawAddress)
{
    const btgatt_scanner_callbacks_t *callbacks = BtifGetGattScannerCallbacks();
    if (callbacks == nullptr || callbacks->scan_result_cb == nullptr) {
        return;
    }
    std::vector<uint8_t> data;
    if (report.data != nullptr && report.dataLen > 0) {
        data.assign(report.data, report.data + report.dataLen);
    }
    // The scanner callback enters the service/GATT layer. Do not run it on the
    // GAP task: GAP commands are synchronous and will deadlock with discovery
    // start/cancel if the service callback waits for the Settings thread.
    uint16_t eventType = advType;
    uint8_t addressType = address->type;
    int8_t rssi = report.rssi;
    RawAddress reportAddress = rawAddress;
    auto task = std::bind(HandleGattScanResult, eventType, addressType, reportAddress, rssi, data);
    if (!bluetooth::btif::TaskScheduler::Post(
        bluetooth::btif::BtifTaskLane::CALLBACK, task)) {
        HILOGW("drop GATT scan result: callback lane unavailable");
    }
}

void OnAdvertisingReport(
    uint8_t advType, const BtAddr *address, GapAdvReportParam report, const BtAddr *currentAddress, void *context)
{
    (void)currentAddress;
    (void)context;
    if (address == nullptr) {
        return;
    }

    RawAddress rawAddress = ToRawAddress(*address);
    std::string name;
    if (report.data != nullptr && report.dataLen > 0) {
        name = FindAdLocalName(report.data, report.dataLen);
    }
    DispatchToDiscovery(rawAddress, name, report.rssi, report.dataLen);
    DispatchToGattScanner(advType, address, report, rawAddress);
}

void OnExtendedAdvertisingReport(
    uint8_t advType, const BtAddr *address, GapExAdvReportParam report, const BtAddr *currentAddress, void *context)
{
    GapAdvReportParam legacyReport = {};
    legacyReport.dataLen = report.dataLen;
    legacyReport.data = report.data;
    legacyReport.rssi = report.rssi;
    OnAdvertisingReport(advType, address, legacyReport, currentAddress, context);
}

void AdvanceScanStateLocked();
void HandleScanParamComplete(uint8_t status);
void HandleScanEnableComplete(uint8_t status);

void OnExtendedScanParamResult(uint8_t status, void *context)
{
    (void)context;
    HandleScanParamComplete(status);
}

void OnExtendedScanEnableResult(uint8_t status, void *context)
{
    (void)context;
    HandleScanEnableComplete(status);
}

void OnScanParamResult(uint8_t status, void *context)
{
    (void)context;
    HandleScanParamComplete(status);
}

void OnScanEnableResult(uint8_t status, void *context)
{
    (void)context;
    HandleScanEnableComplete(status);
}

namespace {
const GapScanCallback kScanCallback = {
    OnAdvertisingReport,
    OnScanParamResult,
    OnScanEnableResult,
};
}  // namespace

bool EnsureExScanCallbackRegistered()
{
    if (::CoreBleScanner::GetInstance().GetState().exScanRegistered) {
        return true;
    }
    ::CoreBleScanner::GetInstance().GetState().exScanCallback = {};
    ::CoreBleScanner::GetInstance().GetState().exScanCallback.exAdvertisingReport = OnExtendedAdvertisingReport;
    ::CoreBleScanner::GetInstance().GetState().exScanCallback.scanExSetParamResult = OnExtendedScanParamResult;
    ::CoreBleScanner::GetInstance().GetState().exScanCallback.scanExSetEnableResult = OnExtendedScanEnableResult;
    if (GAPIF_RegisterExScanCallback(&::CoreBleScanner::GetInstance().GetState().exScanCallback, nullptr) !=
        BT_SUCCESS) {
        return false;
    }
    ::CoreBleScanner::GetInstance().GetState().exScanRegistered = true;
    return true;
}

bool EnsureScanCallbackRegistered()
{
    if (::CoreBleScanner::GetInstance().GetState().scanRegistered) {
        return true;
    }
    if (GAPIF_RegisterScanCallback(&kScanCallback, nullptr) != BT_SUCCESS) {
        return false;
    }
    ::CoreBleScanner::GetInstance().GetState().scanRegistered = true;
    return true;
}

bool EnsureBleRole()
{
    if (::CoreBleScanner::GetInstance().GetState().bleRoleSet) {
        return true;
    }
    uint8_t roles = GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_PERIPHERAL | GAP_LE_ROLE_CENTRAL;
    int ret = GAPIF_LeSetRole(roles);
    HILOGI("BLE set role result=%{public}d", ret);
    if (ret != BT_SUCCESS && ret != BT_ALREADY) {
        return false;
    }
    ::CoreBleScanner::GetInstance().GetState().bleRoleSet = true;
    return true;
}

const char *ScanStateName(BleScanState state)
{
    switch (state) {
        case BleScanState::IDLE:
            return "IDLE";
        case BleScanState::DISABLING:
            return "DISABLING";
        case BleScanState::SETTING_PARAM:
            return "SETTING_PARAM";
        case BleScanState::ENABLING:
            return "ENABLING";
        case BleScanState::SCANNING:
            return "SCANNING";
        case BleScanState::STOPPING:
            return "STOPPING";
        default:
            return "UNKNOWN";
    }
}

bool HasCallerLocked(BleScanCaller caller)
{
    return ::CoreBleScanner::GetInstance().GetState().scanPolicy.HasCaller(caller);
}

void AddCallerLocked(BleScanCaller caller)
{
    ::CoreBleScanner::GetInstance().GetState().scanPolicy.AddCaller(caller);
}

void RemoveCallerLocked(BleScanCaller caller)
{
    ::CoreBleScanner::GetInstance().GetState().scanPolicy.RemoveCaller(caller);
}

bool NeedScanLocked()
{
    return ::CoreBleScanner::GetInstance().GetState().scanPolicy.NeedScan();
}

void ResetScanSessionLocked()
{
    ::CoreBleScanner::GetInstance().GetState().scanMode = BleScanMode::LEGACY;
    ::CoreBleScanner::GetInstance().GetState().extendedRetryTried = false;
    ::CoreBleScanner::GetInstance().GetState().scanRetryCount = 0;
}

void GetOptimalParamLocked(uint16_t &interval, uint16_t &window, bool &filterDuplicates)
{
    CoreBleScanPolicy::OptimalParam param = ::CoreBleScanner::GetInstance().GetState().scanPolicy.GetOptimalParam();
    interval = param.interval;
    window = param.window;
    filterDuplicates = param.filterDuplicates;
}

void CancelScanStageTimeoutLocked();

void FailScan(const char *stage, int status, bool allowExtendedFallback = false)
{
    CancelScanStageTimeoutLocked();
    HILOGI(
        "BLE scan failed stage=%{public}s status=%{public}d mode=%{public}d callers=0x%{public}02x", stage, status,
        static_cast<int>(::CoreBleScanner::GetInstance().GetState().scanMode),
        ::CoreBleScanner::GetInstance().GetState().scanPolicy.GetCallerMask());

    if (allowExtendedFallback && ::CoreBleScanner::GetInstance().GetState().scanMode == BleScanMode::LEGACY &&
        !::CoreBleScanner::GetInstance().GetState().extendedRetryTried && status == HCI_COMMAND_DISALLOWED) {
        ::CoreBleScanner::GetInstance().GetState().extendedRetryTried = true;
        ::CoreBleScanner::GetInstance().GetState().scanMode = BleScanMode::EXTENDED;
        ::CoreBleScanner::GetInstance().GetState().scanState = BleScanState::IDLE;
        ::CoreBleScanner::GetInstance().GetState().scanning = false;
        ::CoreBleScanner::GetInstance().GetState().scanFault = false;
        ::CoreBleScanner::GetInstance().GetState().scanRetryCount = 1;
        HILOGI(
            "BLE legacy scan 0x0c, retry with extended scan callers=0x%{public}02x",
            ::CoreBleScanner::GetInstance().GetState().scanPolicy.GetCallerMask());
        AdvanceScanStateLocked();
        return;
    }

    if (::CoreBleScanner::GetInstance().GetState().scanTrigger != BleScanCaller::NONE) {
        RemoveCallerLocked(::CoreBleScanner::GetInstance().GetState().scanTrigger);
        ::CoreBleScanner::GetInstance().GetState().scanTrigger = BleScanCaller::NONE;
    }
    ::CoreBleScanner::GetInstance().GetState().scanState = BleScanState::IDLE;
    ::CoreBleScanner::GetInstance().GetState().scanning = false;

    if (NeedScanLocked() && ::CoreBleScanner::GetInstance().GetState().scanRetryCount < 1) {
        ::CoreBleScanner::GetInstance().GetState().scanRetryCount++;
        ::CoreBleScanner::GetInstance().GetState().scanFault = false;
        HILOGI(
            "BLE scan retry with remaining callers=0x%{public}02x",
            ::CoreBleScanner::GetInstance().GetState().scanPolicy.GetCallerMask());
        AdvanceScanStateLocked();
        return;
    }
    ::CoreBleScanner::GetInstance().GetState().scanFault = true;
}

void CancelScanStageTimeoutLocked()
{
    ++::CoreBleScanner::GetInstance().GetState().scanStageGeneration;
    if (::CoreBleScanner::GetInstance().GetState().scanStageAlarm != nullptr) {
        AlarmCancel(::CoreBleScanner::GetInstance().GetState().scanStageAlarm.get());
    }
}

void OnScanStageTimeout(void *context)
{
    uint32_t generation = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(context));
    std::lock_guard<std::mutex> lock(::CoreBleScanner::GetInstance().GetState().scanMutex);
    if (generation != ::CoreBleScanner::GetInstance().GetState().scanStageGeneration) {
        return;
    }
    if (::CoreBleScanner::GetInstance().GetState().scanState == BleScanState::IDLE ||
        ::CoreBleScanner::GetInstance().GetState().scanState == BleScanState::SCANNING) {
        return;
    }
    HILOGE(
        "BLE scan stage timeout state=%{public}s mode=%{public}d callers=0x%{public}02x",
        ScanStateName(::CoreBleScanner::GetInstance().GetState().scanState),
        static_cast<int>(::CoreBleScanner::GetInstance().GetState().scanMode),
        ::CoreBleScanner::GetInstance().GetState().scanPolicy.GetCallerMask());
    FailScan("stage-timeout", HCI_TIMEOUT, false);
}

void ArmScanStageTimeoutLocked()
{
    CancelScanStageTimeoutLocked();
    if (::CoreBleScanner::GetInstance().GetState().scanStageAlarm == nullptr) {
        ::CoreBleScanner::GetInstance().GetState().scanStageAlarm = AlarmPtr(AlarmCreate("BtLeScan", false));
        if (::CoreBleScanner::GetInstance().GetState().scanStageAlarm == nullptr) {
            HILOGE("BLE scan stage timeout alarm create failed");
            return;
        }
    }
    uint32_t generation = ::CoreBleScanner::GetInstance().GetState().scanStageGeneration;
    if (AlarmSet(::CoreBleScanner::GetInstance().GetState().scanStageAlarm.get(),
        bluetooth::btif::kBleScanStageTimeoutMs, OnScanStageTimeout,
        reinterpret_cast<void *>(static_cast<uintptr_t>(generation))) != 0) {
        HILOGE(
            "BLE scan stage timeout arm failed state=%{public}s",
            ScanStateName(::CoreBleScanner::GetInstance().GetState().scanState));
        return;
    }
    HILOGI(
        "BLE scan stage timeout armed state=%{public}s mode=%{public}d timeoutMs=%{public}d",
        ScanStateName(::CoreBleScanner::GetInstance().GetState().scanState),
        static_cast<int>(::CoreBleScanner::GetInstance().GetState().scanMode), bluetooth::btif::kBleScanStageTimeoutMs);
}

bool RequestScanParam()
{
    uint16_t interval = GAP_SCAN_INTERVAL_RANGE_MAX;
    uint16_t window = GAP_SCAN_WINDOW_RANGE_MAX;
    bool filterDuplicates = true;
    GetOptimalParamLocked(interval, window, filterDuplicates);
    (void)filterDuplicates;

    GapLeScanParam parameter = {};
    parameter.scanType = 1;
    parameter.param.scanInterval = interval;
    parameter.param.scanWindow = window;
    ::CoreBleScanner::GetInstance().GetState().scanState = BleScanState::SETTING_PARAM;
    ArmScanStageTimeoutLocked();

    if (::CoreBleScanner::GetInstance().GetState().scanMode == BleScanMode::EXTENDED) {
        if (!EnsureExScanCallbackRegistered()) {
            FailScan("ext-register", BT_BAD_STATUS);
            return false;
        }
        int ret = GAPIF_LeExScanSetParam(GAP_SCAN_NOT_USE_WL, GAP_EX_SCAN_PHY_1M, &parameter);
        HILOGI(
            "BLE extended scan param request interval=%{public}u window=%{public}u ret=%{public}d", interval, window,
            ret);
        if (ret != BT_SUCCESS) {
            FailScan("ex-param", ret);
            return false;
        }
        return true;
    }

    int ret = GAPIF_LeScanSetParam(parameter, GAP_SCAN_NOT_USE_WL);
    HILOGI("BLE legacy scan param request interval=%{public}u window=%{public}u ret=%{public}d", interval, window, ret);
    if (ret != BT_SUCCESS) {
        FailScan("param", ret, true);
        return false;
    }
    return true;
}

bool RequestScanEnable()
{
    uint16_t interval = GAP_SCAN_INTERVAL_RANGE_MAX;
    uint16_t window = GAP_SCAN_WINDOW_RANGE_MAX;
    bool filterDuplicates = true;
    GetOptimalParamLocked(interval, window, filterDuplicates);
    (void)interval;
    (void)window;

    ::CoreBleScanner::GetInstance().GetState().scanState = BleScanState::ENABLING;
    ArmScanStageTimeoutLocked();
    if (::CoreBleScanner::GetInstance().GetState().scanMode == BleScanMode::EXTENDED) {
        int ret = GAPIF_LeExScanSetEnable(1, filterDuplicates ? 1 : 0, 0, 0);
        HILOGI("BLE extended scan enable request filterDuplicates=%{public}d ret=%{public}d",
            filterDuplicates ? 1 : 0, ret);
        if (ret != BT_SUCCESS) {
            FailScan("ex-enable", ret);
            return false;
        }
        return true;
    }

    int ret = GAPIF_LeScanSetEnable(1, filterDuplicates ? 1 : 0);
    HILOGI("BLE legacy scan enable request filterDuplicates=%{public}d ret=%{public}d", filterDuplicates ? 1 : 0, ret);
    if (ret != BT_SUCCESS) {
        FailScan("enable", ret, true);
        return false;
    }
    return true;
}

void RequestScanStop()
{
    if (::CoreBleScanner::GetInstance().GetState().scanState != BleScanState::SCANNING) {
        return;
    }
    ::CoreBleScanner::GetInstance().GetState().scanState = BleScanState::STOPPING;
    ArmScanStageTimeoutLocked();
    int ret;
    if (::CoreBleScanner::GetInstance().GetState().scanMode == BleScanMode::EXTENDED) {
        ret = GAPIF_LeExScanSetEnable(0, 1, 0, 0);
    } else {
        ret = GAPIF_LeScanSetEnable(0, 1);
    }
    HILOGI(
        "BLE scan stop request mode=%{public}d ret=%{public}d",
        static_cast<int>(::CoreBleScanner::GetInstance().GetState().scanMode), ret);
    if (ret != BT_SUCCESS) {
        FailScan("stop", ret);
    }
}

void StartScanSequence()
{
    if (!NeedScanLocked()) {
        return;
    }
    if (!EnsureScanCallbackRegistered() || !EnsureBleRole()) {
        FailScan("init", BT_BAD_STATUS);
        return;
    }
    // Only clear the controller state when this host believes a scan is running.
    // Sending LE Set Scan Enable(0) while already stopped commonly returns
    // Command Disallowed, which is now treated as a hard failure.
    if (!::CoreBleScanner::GetInstance().GetState().scanning) {
        RequestScanParam();
        return;
    }
    ::CoreBleScanner::GetInstance().GetState().scanState = BleScanState::DISABLING;
    ArmScanStageTimeoutLocked();
    int ret;
    if (::CoreBleScanner::GetInstance().GetState().scanMode == BleScanMode::EXTENDED) {
        ret = GAPIF_LeExScanSetEnable(0, 1, 0, 0);
    } else {
        ret = GAPIF_LeScanSetEnable(0, 1);
    }
    HILOGI(
        "BLE scan disable request mode=%{public}d ret=%{public}d callers=0x%{public}02x",
        static_cast<int>(::CoreBleScanner::GetInstance().GetState().scanMode), ret,
        ::CoreBleScanner::GetInstance().GetState().scanPolicy.GetCallerMask());
    if (ret != BT_SUCCESS) {
        FailScan("disable", ret);
    }
}

void AdvanceScanStateLocked()
{
    if (!NeedScanLocked()) {
        RequestScanStop();
        return;
    }
    if (::CoreBleScanner::GetInstance().GetState().scanState != BleScanState::IDLE) {
        return;
    }
    if (::CoreBleScanner::GetInstance().GetState().scanFault) {
        HILOGI("BLE scan is in fault state, wait for explicit restart");
        return;
    }
    StartScanSequence();
}

void ApplyCallerChangeLocked()
{
    if (::CoreBleScanner::GetInstance().GetState().scanState == BleScanState::SCANNING) {
        ::CoreBleScanner::GetInstance().GetState().scanReconfigure = true;
        RequestScanStop();
        return;
    }
    AdvanceScanStateLocked();
}

void ScheduleScanStateUpdate()
{
    if (!bluetooth::btif::TaskScheduler::Post(bluetooth::btif::BtifTaskLane::CONTROL, []() {
            std::lock_guard<std::mutex> lock(::CoreBleScanner::GetInstance().GetState().scanMutex);
            ApplyCallerChangeLocked();
        })) {
        HILOGW("drop BLE scan state update: control lane unavailable");
    }
}

void HandleScanParamComplete(uint8_t status)
{
    std::lock_guard<std::mutex> lock(::CoreBleScanner::GetInstance().GetState().scanMutex);
    HILOGI(
        "BLE scan param complete state=%{public}s status=%{public}d callers=0x%{public}02x",
        ScanStateName(::CoreBleScanner::GetInstance().GetState().scanState), status,
        ::CoreBleScanner::GetInstance().GetState().scanPolicy.GetCallerMask());
    if (::CoreBleScanner::GetInstance().GetState().scanState != BleScanState::SETTING_PARAM) {
        return;
    }
    if (status != BT_SUCCESS) {
        FailScan("param-complete", status, true);
        return;
    }
    if (!NeedScanLocked() || ::CoreBleScanner::GetInstance().GetState().scanFault) {
        CancelScanStageTimeoutLocked();
        ::CoreBleScanner::GetInstance().GetState().scanState = BleScanState::IDLE;
        return;
    }
    RequestScanEnable();
}

static void HandleDisableCompleteLocked(uint8_t status)
{
    if (status != BT_SUCCESS) {
        FailScan("disable-complete", status);
        return;
    }
    if (!NeedScanLocked() || ::CoreBleScanner::GetInstance().GetState().scanFault) {
        CancelScanStageTimeoutLocked();
        ::CoreBleScanner::GetInstance().GetState().scanState = BleScanState::IDLE;
        return;
    }
    RequestScanParam();
}

static void HandleEnableCompleteLocked(uint8_t status)
{
    if (status != BT_SUCCESS) {
        FailScan("enable-complete", status, true);
        return;
    }
    ::CoreBleScanner::GetInstance().GetState().scanState = BleScanState::SCANNING;
    ::CoreBleScanner::GetInstance().GetState().scanning = true;
    ::CoreBleScanner::GetInstance().GetState().scanRetryCount = 0;
    ::CoreBleScanner::GetInstance().GetState().scanTrigger = BleScanCaller::NONE;
    CancelScanStageTimeoutLocked();
    HILOGI("BLE scan state=SCANNING");
    if (!NeedScanLocked()) {
        RequestScanStop();
    }
}

static void HandleStopCompleteLocked(uint8_t status)
{
    if (status != BT_SUCCESS) {
        FailScan("stop-complete", status);
        return;
    }
    ::CoreBleScanner::GetInstance().GetState().scanState = BleScanState::IDLE;
    ::CoreBleScanner::GetInstance().GetState().scanning = false;
    CancelScanStageTimeoutLocked();
    HILOGI("BLE scan state=IDLE");
    if (::CoreBleScanner::GetInstance().GetState().scanReconfigure) {
        ::CoreBleScanner::GetInstance().GetState().scanReconfigure = false;
        HILOGI("BLE scan reconfigure with merged parameters");
    }
    if (NeedScanLocked() && !::CoreBleScanner::GetInstance().GetState().scanFault) {
        AdvanceScanStateLocked();
    }
}

void HandleScanEnableComplete(uint8_t status)
{
    std::lock_guard<std::mutex> lock(::CoreBleScanner::GetInstance().GetState().scanMutex);
    HILOGI(
        "BLE scan enable complete state=%{public}s status=%{public}d callers=0x%{public}02x",
        ScanStateName(::CoreBleScanner::GetInstance().GetState().scanState), status,
        ::CoreBleScanner::GetInstance().GetState().scanPolicy.GetCallerMask());
    switch (::CoreBleScanner::GetInstance().GetState().scanState) {
        case BleScanState::DISABLING:
            HandleDisableCompleteLocked(status);
            return;
        case BleScanState::ENABLING:
            HandleEnableCompleteLocked(status);
            return;
        case BleScanState::STOPPING:
            HandleStopCompleteLocked(status);
            return;
        default:
            break;
    }
    HILOGI(
        "BLE scan enable complete ignored, state=%{public}s",
        ScanStateName(::CoreBleScanner::GetInstance().GetState().scanState));
}

class BtifBleScanner : public BleScannerInterface {
   public:
    void RegisterCallbacks(ScanningCallbacks *callbacks) override
    {
        (void)callbacks;
        HILOGI("BLE scanner RegisterCallbacks called");
    }

    void RegisterScanner(RegisterCallback callback) override
    {
        if (callback != nullptr) {
            callback(0, 0);
        }
    }

    void Unregister(int scannerId) override
    {
        (void)scannerId;
    }

    void Scan(bool start) override
    {
        if (start && BtifGetGattScannerCallbacks() == nullptr) {
            return;
        }
        bool schedule = false;
        {
            std::lock_guard<std::mutex> lock(::CoreBleScanner::GetInstance().GetState().scanMutex);
            bool active = HasCallerLocked(BleScanCaller::GATT);
            if (start == active) {
                if (start && ::CoreBleScanner::GetInstance().GetState().scanFault) {
                    ::CoreBleScanner::GetInstance().GetState().scanFault = false;
                    schedule = true;
                }
            } else {
                if (start) {
                    if (!NeedScanLocked()) {
                        ResetScanSessionLocked();
                    }
                    AddCallerLocked(BleScanCaller::GATT);
                    ::CoreBleScanner::GetInstance().GetState().scanTrigger = BleScanCaller::GATT;
                    ::CoreBleScanner::GetInstance().GetState().scanFault = false;
                } else {
                    RemoveCallerLocked(BleScanCaller::GATT);
                }
                schedule = true;
            }
        }
        if (schedule) {
            ScheduleScanStateUpdate();
        }
    }

    void ScanFilterParamSetup(
        uint8_t clientIf, uint8_t action, uint8_t filtIndex, std::unique_ptr<btgatt_filt_param_setup_t> filtParam,
        FilterParamSetupCallback callback) override
    {
        (void)clientIf;
        (void)filtIndex;
        (void)filtParam;
        if (callback != nullptr) {
            callback(0, action, 0);
        }
    }

    void ScanFilterAdd(int filterIndex, std::vector<ApcfCommand> filters, FilterConfigCallback callback) override
    {
        (void)filterIndex;
        (void)filters;
        if (callback != nullptr) {
            callback(0, 0, 0, 0);
        }
    }

    void ScanFilterClear(int filtIndex, FilterConfigCallback callback) override
    {
        (void)filtIndex;
        if (callback != nullptr) {
            callback(0, 0, 0, 0);
        }
    }

    void ScanFilterEnable(bool enable, EnableCallback callback) override
    {
        if (callback != nullptr) {
            callback(enable ? 1 : 0, 0);
        }
    }

    void SetScanParameters(
        uint32_t scanInterval, uint32_t scanWindow, uint8_t legacy, uint8_t scanPhy,
        std::function<void(uint8_t status)> callback) override
    {
        (void)legacy;
        (void)scanPhy;
        uint16_t interval = static_cast<uint16_t>(scanInterval);
        uint16_t window = static_cast<uint16_t>(scanWindow);
        if (interval == 0 || interval > GAP_SCAN_INTERVAL_RANGE_MAX) {
            interval = GAP_SCAN_INTERVAL_RANGE_MAX;
        }
        if (window == 0 || window > GAP_SCAN_WINDOW_RANGE_MAX) {
            window = GAP_SCAN_WINDOW_RANGE_MAX;
        }
        if (window > interval) {
            window = interval;
        }
        {
            std::lock_guard<std::mutex> lock(::CoreBleScanner::GetInstance().GetState().scanMutex);
            ::CoreBleScanner::GetInstance().GetState().scanPolicy.SetGattParam({interval, window, true, true});
        }
        ScheduleScanStateUpdate();
        if (callback != nullptr) {
            callback(0);
        }
    }

    void BatchscanConfigStorage(int clientIf, int batchScanFullMax, int batchScanTruncMax,
        int batchScanNotifyThreshold, Callback callback) override
    {
        (void)clientIf;
        (void)batchScanFullMax;
        (void)batchScanTruncMax;
        (void)batchScanNotifyThreshold;
        if (callback != nullptr) {
            callback(0);
        }
    }

    void BatchscanEnable(
        int scanMode, int scanInterval, int scanWindow, int addrType, int discardRule, Callback callback) override
    {
        (void)scanMode;
        (void)scanInterval;
        (void)scanWindow;
        (void)addrType;
        (void)discardRule;
        if (callback != nullptr) {
            callback(0);
        }
    }

    void BatchscanDisable(Callback callback) override
    {
        if (callback != nullptr) {
            callback(0);
        }
    }

    void BatchscanReadReports(int clientIf, int scanMode) override
    {
        (void)clientIf;
        (void)scanMode;
    }

    void StartSync(
        uint8_t sid, RawAddress address, uint16_t skip, uint16_t timeout, StartSyncCb startCallback,
        SyncReportCb reportCallback, SyncLostCb lostCallback) override
    {
        (void)sid;
        (void)address;
        (void)skip;
        (void)timeout;
        (void)startCallback;
        (void)reportCallback;
        (void)lostCallback;
    }

    void StopSync(uint16_t handle) override
    {
        (void)handle;
    }
};

}  // namespace

BleScannerInterface *BtifGetBleScannerInstance()
{
    static BtifBleScanner instance;
    return &instance;
}

void BtifStartBleDiscovery(BtifBleDiscoveryCallback callback)
{
    if (callback == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(::CoreBleScanner::GetInstance().GetState().scanMutex);
    {
        std::lock_guard<std::mutex> callbackLock(::CoreBleScanner::GetInstance().GetState().callbackMutex);
        ::CoreBleScanner::GetInstance().GetState().bleDiscoveryCallback = callback;
        ::CoreBleScanner::GetInstance().GetState().bleReportLogged = false;
    }
    if (HasCallerLocked(BleScanCaller::DISCOVERY)) {
        if (::CoreBleScanner::GetInstance().GetState().scanFault) {
            ::CoreBleScanner::GetInstance().GetState().scanFault = false;
            ApplyCallerChangeLocked();
        }
        return;
    }
    if (!NeedScanLocked()) {
        ResetScanSessionLocked();
    }
    AddCallerLocked(BleScanCaller::DISCOVERY);
    ::CoreBleScanner::GetInstance().GetState().scanTrigger = BleScanCaller::DISCOVERY;
    ::CoreBleScanner::GetInstance().GetState().scanPolicy.SetDiscoveryParam({
        GAP_SCAN_INTERVAL_RANGE_MAX,
        static_cast<uint16_t>(GAP_SCAN_INTERVAL_RANGE_MAX / bluetooth::btif::kScanDutyDenominator),
        false,
        true,
    });
    ::CoreBleScanner::GetInstance().GetState().scanFault = false;
    ApplyCallerChangeLocked();
}

void BtifStopBleDiscovery()
{
    std::lock_guard<std::mutex> lock(::CoreBleScanner::GetInstance().GetState().scanMutex);
    {
        std::lock_guard<std::mutex> callbackLock(::CoreBleScanner::GetInstance().GetState().callbackMutex);
        ::CoreBleScanner::GetInstance().GetState().bleDiscoveryCallback = nullptr;
        ::CoreBleScanner::GetInstance().GetState().bleDiscoveryReportTime.clear();
    }
    RemoveCallerLocked(BleScanCaller::DISCOVERY);
    AdvanceScanStateLocked();
}

void BtifSetBleScanCallerActive(BtifBleScanCaller caller, bool active)
{
    BleScanCaller internal = static_cast<BleScanCaller>(static_cast<uint8_t>(caller));
    std::lock_guard<std::mutex> lock(::CoreBleScanner::GetInstance().GetState().scanMutex);
    bool current = HasCallerLocked(internal);
    if (current == active) {
        return;
    }
    if (active) {
        if (!NeedScanLocked()) {
            ResetScanSessionLocked();
        }
        AddCallerLocked(internal);
        ::CoreBleScanner::GetInstance().GetState().scanTrigger = internal;
        ::CoreBleScanner::GetInstance().GetState().scanFault = false;
    } else {
        RemoveCallerLocked(internal);
    }
    ApplyCallerChangeLocked();
}

void BtifBleScanRecover()
{
    std::lock_guard<std::mutex> lock(::CoreBleScanner::GetInstance().GetState().scanMutex);
    HILOGI(
        "BLE scan recover entry state=%{public}s mode=%{public}d fault=%{public}d callers=0x%{public}02x",
        ScanStateName(::CoreBleScanner::GetInstance().GetState().scanState),
        static_cast<int>(::CoreBleScanner::GetInstance().GetState().scanMode),
        ::CoreBleScanner::GetInstance().GetState().scanFault ? 1 : 0,
        ::CoreBleScanner::GetInstance().GetState().scanPolicy.GetCallerMask());
    if (::CoreBleScanner::GetInstance().GetState().scanState == BleScanState::SCANNING &&
        !::CoreBleScanner::GetInstance().GetState().scanFault) {
        HILOGI("BLE scan recover ignored, already SCANNING");
        return;
    }
    CancelScanStageTimeoutLocked();
    ::CoreBleScanner::GetInstance().GetState().scanState = BleScanState::IDLE;
    ::CoreBleScanner::GetInstance().GetState().scanning = false;
    ::CoreBleScanner::GetInstance().GetState().scanFault = false;
    ::CoreBleScanner::GetInstance().GetState().extendedRetryTried = false;
    ::CoreBleScanner::GetInstance().GetState().scanMode = BleScanMode::LEGACY;
    ::CoreBleScanner::GetInstance().GetState().scanRetryCount = 0;
    ApplyCallerChangeLocked();
}
BleScannerInterface *CoreBleScanner::GetInterface()
{
    return BtifGetBleScannerInstance();
}
void CoreBleScanner::StartDiscovery(BtifBleDiscoveryCallback callback)
{
    BtifStartBleDiscovery(callback);
}
void CoreBleScanner::StopDiscovery()
{
    BtifStopBleDiscovery();
}
void CoreBleScanner::SetCallerActive(BtifBleScanCaller caller, bool active)
{
    BtifSetBleScanCallerActive(caller, active);
}
void CoreBleScanner::Recover()
{
    BtifBleScanRecover();
}
