/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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

#include <string>
#include "bluetooth_ble_filter_matcher.h"
#include "ble_service_data.h"
#include "bluetooth_log.h"
#include "bluetooth_utils_server.h"
#include "bluetooth_errorcode.h"
#include "event_handler.h"
#include "event_runner.h"
#include "hisysevent.h"
#include "interface_adapter_ble.h"
#include "interface_adapter_manager.h"
#include "ipc_skeleton.h"
#include "remote_observer_list.h"
#include "permission_utils.h"
#include "bluetooth_ble_central_manager_server.h"
#include "safe_map.h"

namespace OHOS {
namespace Bluetooth {
using namespace OHOS::bluetooth;
struct BluetoothBleCentralManagerServer::impl {
    impl();
    ~impl();

    /// sys state observer
    class SystemStateObserver;
    std::unique_ptr<SystemStateObserver> systemStateObserver_ = nullptr;
    RemoteObserverList<IBluetoothBleCentralManagerCallback, int32_t> observers_;
    SafeMap<sptr<IRemoteObject>, uint32_t> observersToken_;
    std::map<sptr<IRemoteObject>, int32_t> observersPid_;
    std::map<sptr<IRemoteObject>, int32_t> observersScannerId_;
    std::map<int32_t, std::vector<bluetooth::BleScanFilterImpl>> observersBleScanFilters_;
    std::map<int32_t, bool> observersScanFiltersIsEnanled_;
    std::mutex bleScanFiltersMutex_;
    class BleCentralManagerCallback;
    std::unique_ptr<BleCentralManagerCallback> observerImp_ = std::make_unique<BleCentralManagerCallback>(this);

    struct ScanCallbackInfo;
    struct ScanSettingsParam;
    std::vector<ScanCallbackInfo> scanCallbackInfo_;

    BleScanSettingsImpl scanSettingImpl_;
    bool isScanning; // Indicates the bluetooth service is scanning or not.

    std::shared_ptr<AppExecFwk::EventRunner> eventRunner_;
    std::shared_ptr<AppExecFwk::EventHandler> eventHandler_;
};

struct BluetoothBleCentralManagerServer::impl::ScanSettingsParam {
    long reportDelayMillis;
    uint16_t scanInterval;
    uint16_t scanWindow;
    int scanMode;
    bool legacy;
    int phy;
};

struct BluetoothBleCentralManagerServer::impl::ScanCallbackInfo {
    int pid;
    int uid;
    bool isStart; // Indicates the process for which scanning is started or not.
    ScanSettingsParam param;
    sptr<IBluetoothBleCentralManagerCallback> callback;
};

class BluetoothBleCentralManagerServer::impl::BleCentralManagerCallback : public IBleCentralManagerCallback {
public:
    explicit BleCentralManagerCallback(BluetoothBleCentralManagerServer::impl *pimpl) : pimpl_(pimpl) {};
    ~BleCentralManagerCallback() override = default;

    void OnScanCallback(const BleScanResultImpl &result) override
    {
        HILOGI("Address: %{public}s",
            GetEncryptAddr(result.GetPeripheralDevice().GetRawAddress().GetAddress()).c_str());
        observers_->ForEach([this, result](IBluetoothBleCentralManagerCallback *observer) {
            uint32_t tokenId = this->pimpl_->observersToken_.ReadVal(observer->AsObject());
            int32_t pid = this->pimpl_->observersPid_[observer->AsObject()];
            if (BluetoothBleCentralManagerServer::IsResourceScheduleControlApp(pid)) {
                HILOGD("pid:%{public}d is proxy pid, not callback.", pid);
                return;
            }
            if (PermissionUtils::VerifyUseBluetoothPermission(tokenId) == PERMISSION_DENIED) {
                HILOGE("OnScanCallback(): failed, check permission failed, tokenId: %{public}u", tokenId);
            } else {
                BluetoothBleScanResult bleScanResult(result);
                int32_t scannerId = this->pimpl_->observersScannerId_[observer->AsObject()];
                std::lock_guard<std::mutex> lock(this->pimpl_->bleScanFiltersMutex_);
                HILOGD("OnScanCallback() start bleScanFilter Address: %{public}s scannerId:%{public}d",
                    GetEncryptAddr(result.GetPeripheralDevice().GetRawAddress().GetAddress()).c_str(), scannerId);
                bool scanFiltersIsEnanled_ = this->pimpl_->observersScanFiltersIsEnanled_[scannerId];
                // if bleScanFilter been set empty when stopped scan, we need refuse callback instead of filter pass
                if (!scanFiltersIsEnanled_) {
                    return;
                }
                std::vector<bluetooth::BleScanFilterImpl> scanFilters_ = this->pimpl_->
                    observersBleScanFilters_[scannerId];
                if (scanFilters_.empty() ||
                    BluetoothBleFilterMatcher::MatchesScanFilters(scanFilters_, bleScanResult) == MatchResult::MATCH) {
                    observer->OnScanCallback(bleScanResult);
                    HILOGD("OnScanCallback() passed bleScanFilter Address: %{public}s scannerId:%{public}d",
                        GetEncryptAddr(result.GetPeripheralDevice().GetRawAddress().GetAddress()).c_str(), scannerId);
                }
            }
        });
    }

    void OnBleBatchScanResultsEvent(std::vector<BleScanResultImpl> &results) override
    {
        HILOGI("enter");

        observers_->ForEach([this, results](IBluetoothBleCentralManagerCallback *observer) {
            int32_t pid = this->pimpl_->observersPid_[observer->AsObject()];
            if (BluetoothBleCentralManagerServer::IsResourceScheduleControlApp(pid)) {
                HILOGD("pid:%{public}d is proxy pid, not callback.", pid);
                return;
            }
            std::vector<BluetoothBleScanResult> bleScanResults;

            for (auto iter = results.begin(); iter != results.end(); iter++) {
                BluetoothBleScanResult bleScanResult;

                if (iter->GetPeripheralDevice().IsRSSI()) {
                    bleScanResult.SetRssi(iter->GetPeripheralDevice().GetRSSI());
                }

                bleScanResult.SetAdvertiseFlag(iter->GetPeripheralDevice().GetAdFlag());

                if (iter->GetPeripheralDevice().IsManufacturerData()) {
                    std::map<uint16_t, std::string> manuData = iter->GetPeripheralDevice().GetManufacturerData();
                    for (auto manuDataIter = manuData.begin(); manuDataIter != manuData.end(); manuDataIter++) {
                        bleScanResult.AddManufacturerData(manuDataIter->first, manuDataIter->second);
                    }
                }

                bleScanResult.SetConnectable(iter->GetPeripheralDevice().IsConnectable());

                if (iter->GetPeripheralDevice().IsServiceUUID()) {
                    std::vector<Uuid> uuids = iter->GetPeripheralDevice().GetServiceUUID();
                    for (auto serviceUuidIter = uuids.begin(); serviceUuidIter != uuids.end(); serviceUuidIter++) {
                        bleScanResult.AddServiceUuid(*serviceUuidIter);
                    }
                }

                if (iter->GetPeripheralDevice().IsServiceData()) {
                    std::vector<Uuid> uuids = iter->GetPeripheralDevice().GetServiceDataUUID();
                    int index = 0;
                    for (auto serviceDataIter = uuids.begin(); serviceDataIter != uuids.end(); serviceDataIter++) {
                        bleScanResult.AddServiceData(
                            *serviceDataIter, iter->GetPeripheralDevice().GetServiceData(index));
                        ++index;
                    }
                }

                bleScanResult.SetPeripheralDevice(iter->GetPeripheralDevice().GetRawAddress());

                bleScanResult.SetPayload(std::string(iter->GetPeripheralDevice().GetPayload(),
                    iter->GetPeripheralDevice().GetPayload() + iter->GetPeripheralDevice().GetPayloadLen()));

                bleScanResults.push_back(bleScanResult);
            }
            observer->OnBleBatchScanResultsEvent(bleScanResults);
        });
    }

    void OnStartOrStopScanEvent(int resultCode, bool isStartScanEvt) override
    {
        HILOGI("code: %{public}d, isStartScanEvt: %{public}d", resultCode, isStartScanEvt);
        if (pimpl_ == nullptr || pimpl_->eventHandler_ == nullptr) {
            HILOGE("pimpl_ or eventHandler_ is nullptr");
            return;
        }
        pimpl_->eventHandler_->PostTask([=]() {
            /* start scan -> close bluetooth -> open bluetooth -> start scan
               After the bluetooth is closed, the stack stops scanning.
               When receiving this event, the related status needs to be cleared. Otherwise, the next scanning fails. */
            HILOGI("isScanning: %{public}d", pimpl_->isScanning);
            if (pimpl_->isScanning && !isStartScanEvt && resultCode == 0) {
                pimpl_->isScanning = false;
                ClearMultiProcessScanState();
                OnStartOrStopScanEventCb(resultCode, isStartScanEvt);
                return;
            }

            if (resultCode != 0) {
                pimpl_->isScanning = !pimpl_->isScanning;
            }
            auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
            if (bleService == nullptr) {
                HILOGE("bleService is nullptr.");
                return;
            }

            if (!pimpl_->isScanning && resultCode == 0) {
                // When updating params the scanning is stopped successfully and the scanning will be restarted.
                for (auto iter = pimpl_->scanCallbackInfo_.begin(); iter != pimpl_->scanCallbackInfo_.end(); ++iter) {
                    if (iter->isStart) {
                        bleService->StartScan(pimpl_->scanSettingImpl_);
                        pimpl_->isScanning = true;
                        break;
                    }
                }
            } else if (!pimpl_->isScanning && resultCode != 0) {
                // When updating params the scanning is stopped successfully and the scanning restart failed.
                ClearMultiProcessScanState();
            }
            OnStartOrStopScanEventCb(resultCode, isStartScanEvt);
        });
    }

    void OnNotifyMsgReportFromLpDevice(FilterIdxInfo &info, int msgType,
        const std::vector<uint8_t> &notifyValue) override
    {
        return;
    }

    void SetObserver(RemoteObserverList<IBluetoothBleCentralManagerCallback, int32_t> *observers)
    {
        observers_ = observers;
    }

private:
    RemoteObserverList<IBluetoothBleCentralManagerCallback, int32_t> *observers_ = nullptr;
    BluetoothBleCentralManagerServer::impl *pimpl_ = nullptr;

    void ClearMultiProcessScanState()
    {
        for (auto iter = pimpl_->scanCallbackInfo_.begin(); iter != pimpl_->scanCallbackInfo_.end(); ++iter) {
            if (iter->isStart) {
                iter->isStart = false;
            }
        }
    }

    void OnStartOrStopScanEventCb(int resultCode, bool isStartScanEvt)
    {
        observers_->ForEach([resultCode, isStartScanEvt](IBluetoothBleCentralManagerCallback *observer) {
            observer->OnStartOrStopScanEvent(resultCode, isStartScanEvt);
        });
    }
};

class BluetoothBleCentralManagerServer::impl::SystemStateObserver : public ISystemStateObserver {
public:
    explicit SystemStateObserver(BluetoothBleCentralManagerServer::impl *pimpl) : pimpl_(pimpl){};
    void OnSystemStateChange(const BTSystemState state) override
    {
        auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
        switch (state) {
            case BTSystemState::ON:
                if (bleService != nullptr) {
                    bleService->RegisterBleCentralManagerCallback(*pimpl_->observerImp_.get());
                }
                break;
            default:
                break;
        }
    };

private:
    BluetoothBleCentralManagerServer::impl *pimpl_ = nullptr;
};

BluetoothBleCentralManagerServer::impl::impl()
{
    eventRunner_ = AppExecFwk::EventRunner::Create("bt central manager server");
    eventHandler_ = std::make_shared<AppExecFwk::EventHandler>(eventRunner_);
    isScanning = false;
}

BluetoothBleCentralManagerServer::impl::~impl()
{
    auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
    if (bleService != nullptr) {
        bleService->DeregisterBleCentralManagerCallback();
    }
}

BluetoothBleCentralManagerServer::BluetoothBleCentralManagerServer()
{
    pimpl = std::make_unique<impl>();

    pimpl->eventHandler_->PostSyncTask(
        [&]() {
            pimpl->observerImp_->SetObserver(&(pimpl->observers_));
            pimpl->systemStateObserver_ = std::make_unique<impl::SystemStateObserver>(pimpl.get());
            IAdapterManager::GetInstance()->RegisterSystemStateObserver(*(pimpl->systemStateObserver_));

            auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
            if (bleService != nullptr) {
                bleService->RegisterBleCentralManagerCallback(*pimpl->observerImp_.get());
            }
        },
        AppExecFwk::EventQueue::Priority::HIGH);
}

BluetoothBleCentralManagerServer::~BluetoothBleCentralManagerServer()
{
    pimpl->eventHandler_->PostSyncTask(
        [&]() { IAdapterManager::GetInstance()->DeregisterSystemStateObserver(*(pimpl->systemStateObserver_)); },
        AppExecFwk::EventQueue::Priority::HIGH);
}

std::mutex BluetoothBleCentralManagerServer::proxyMutex_;
std::set<int32_t> BluetoothBleCentralManagerServer::proxyPids_;

bool BluetoothBleCentralManagerServer::FreezeByRss(std::set<int> pidSet, bool isProxy)
{
    HILOGD("bluetooth proxy, pid[%{public}s] isProxy: %{public}d", ToLogString(pidSet).c_str(), isProxy);
    std::lock_guard<std::mutex> lock(proxyMutex_);
    for (int pid : pidSet) {
        if (isProxy) {
            proxyPids_.insert(pid);
        } else {
            proxyPids_.erase(pid);
        }
    }
    return true;
}

bool BluetoothBleCentralManagerServer::ResetAllProxy()
{
    HILOGI("Start bluetooth ResetAllProxy");
    std::lock_guard<std::mutex> lock(proxyMutex_);
    proxyPids_.clear();
    return true;
}

bool BluetoothBleCentralManagerServer::IsResourceScheduleControlApp(int32_t pid)
{
    std::lock_guard<std::mutex> lock(proxyMutex_);
    return proxyPids_.find(pid) != proxyPids_.end();
}

bool CheckBleScanPermission()
{
    if (PermissionUtils::GetApiVersion() >= 10) { // 10:api version
        if (PermissionUtils::VerifyAccessBluetoothPermission() == PERMISSION_DENIED) {
            HILOGE("check access permission failed.");
            return false;
        }
    } else {
        if (PermissionUtils::VerifyDiscoverBluetoothPermission() == PERMISSION_DENIED ||
            PermissionUtils::VerifyManageBluetoothPermission() == PERMISSION_DENIED) {
            HILOGE("check permission failed.");
            return false;
        }
        if (PermissionUtils::VerifyApproximatelyPermission() == PERMISSION_DENIED &&
            PermissionUtils::VerifyLocationPermission() == PERMISSION_DENIED) {
            HILOGE("No location permission");
            return false;
        }
    }
    return true;
}

int BluetoothBleCentralManagerServer::StartScan(int32_t scannerId, const BluetoothBleScanSettings &settings,
    const std::vector<BluetoothBleScanFilter> &filters)
{
    int32_t pid = IPCSkeleton::GetCallingPid();
    int32_t uid = IPCSkeleton::GetCallingUid();
    if (!CheckBleScanPermission()) {
        HILOGE("check permission failed.");
        return BT_ERR_PERMISSION_FAILED;
    }

    ConfigScanFilterInner(scannerId, filters);

    pimpl->eventHandler_->PostSyncTask([&]() {
        auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
        if (bleService == nullptr) {
            HILOGE("bleService is nullptr.");
            return;
        }

        for (auto iter = pimpl->scanCallbackInfo_.begin(); iter != pimpl->scanCallbackInfo_.end(); ++iter) {
            if (iter->pid == pid && iter->uid == uid) {
                iter->isStart = true;
                SetWindowAndInterval(settings.GetScanMode(), iter->param.scanWindow, iter->param.scanInterval);
                iter->param.reportDelayMillis = settings.GetReportDelayMillisValue();
                iter->param.scanMode = settings.GetScanMode();
                iter->param.legacy = settings.GetLegacy();
                iter->param.phy = settings.GetPhy();
                break;
            }
        }

        if (!pimpl->isScanning) {
            HILOGI("start ble scan.");
            SetScanParams(settings);
            bleService->StartScan(pimpl->scanSettingImpl_);
            pimpl->isScanning = true;
            HiSysEventWrite(OHOS::HiviewDFX::HiSysEvent::Domain::BT_SERVICE, "BLE_SCAN_START",
                OHOS::HiviewDFX::HiSysEvent::EventType::STATISTIC, "PID", pid, "UID", uid,
                "TYPE", (settings.GetReportDelayMillisValue() > 0) ? 1 : 0);
        } else if (IsNewScanParams()) {
            // Stop an ongoing ble scan, update parameters and restart the ble scan in OnStartOrStopScanEvent().
            HILOGI("restart ble scan.");
            bleService->StopScan();
            pimpl->isScanning = false;
        } else {
            HILOGI("scan is already started and has the same params.");
        }
    });
    return NO_ERROR;
}

int BluetoothBleCentralManagerServer::StopScan(int32_t scannerId)
{
    int32_t pid = IPCSkeleton::GetCallingPid();
    int32_t uid = IPCSkeleton::GetCallingUid();
    HILOGI("pid: %{public}d, uid: %{public}d", pid, uid);
    if (PermissionUtils::VerifyDiscoverBluetoothPermission() == PERMISSION_DENIED) {
        HILOGE("check permission failed.");
        return BT_ERR_PERMISSION_FAILED;
    }

    pimpl->eventHandler_->PostSyncTask([&]() {
        if (!pimpl->isScanning) {
            HILOGE("scan is not started.");
            return;
        }

        auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
        if (bleService == nullptr) {
            HILOGE("bleService is nullptr.");
            return;
        }

        for (auto iter = pimpl->scanCallbackInfo_.begin(); iter != pimpl->scanCallbackInfo_.end(); ++iter) {
            if (iter->pid == pid && iter->uid == uid) {
                iter->isStart = false;
                break;
            }
        }

        if (IsAllStop() || IsNewScanParams()) {
            HILOGI("stop ble scan.");
            bleService->StopScan();
            pimpl->isScanning = false;
            HiSysEventWrite(OHOS::HiviewDFX::HiSysEvent::Domain::BT_SERVICE, "BLE_SCAN_STOP",
                OHOS::HiviewDFX::HiSysEvent::EventType::STATISTIC, "PID", pid, "UID", uid);
        }
    });
    return NO_ERROR;
}

int BluetoothBleCentralManagerServer::ConfigScanFilterInner(
    int32_t scannerId, const std::vector<BluetoothBleScanFilter> &filters)
{
    HILOGI("enter, scannerId: %{public}d", scannerId);

    auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
    if (bleService != nullptr) {
        std::vector<BleScanFilterImpl> filterImpls {};
        for (auto filter : filters) {
            BleScanFilterImpl filterImpl;
            filterImpl.SetDeviceId(filter.GetDeviceId());
            filterImpl.SetName(filter.GetName());
            if (filter.HasServiceUuid()) {
                filterImpl.SetServiceUuid(filter.GetServiceUuid());
            }
            if (filter.HasServiceUuidMask()) {
                filterImpl.SetServiceUuidMask(filter.GetServiceUuidMask());
            }
            if (filter.HasSolicitationUuid()) {
                filterImpl.SetServiceSolicitationUuid(filter.GetServiceSolicitationUuid());
            }
            if (filter.HasSolicitationUuidMask()) {
                filterImpl.SetServiceSolicitationUuidMask(filter.GetServiceSolicitationUuidMask());
            }
            filterImpl.SetServiceData(filter.GetServiceData());
            filterImpl.SetServiceDataMask(filter.GetServiceDataMask());
            filterImpl.SetManufacturerId(filter.GetManufacturerId());
            filterImpl.SetManufactureData(filter.GetManufactureData());
            filterImpl.SetManufactureDataMask(filter.GetManufactureDataMask());
            filterImpls.push_back(filterImpl);
        }
        std::lock_guard<std::mutex> lock(pimpl->bleScanFiltersMutex_);
        pimpl->observersBleScanFilters_[scannerId] = filterImpls;
        pimpl->observersScanFiltersIsEnanled_[scannerId] = true;
        return bleService->ConfigScanFilter(scannerId, filterImpls);
    }
    return NO_ERROR;
}

void BluetoothBleCentralManagerServer::RemoveScanFilter(int32_t scannerId)
{
    HILOGI("enter, scannerId: %{public}d", scannerId);

    auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
    if (bleService != nullptr) {
        bleService->RemoveScanFilter(scannerId);
    }
    std::lock_guard<std::mutex> lock(pimpl->bleScanFiltersMutex_);
    pimpl->observersScanFiltersIsEnanled_[scannerId] = false;
    pimpl->observersBleScanFilters_.erase(scannerId);
}

void BluetoothBleCentralManagerServer::RegisterBleCentralManagerCallback(int32_t &scannerId, bool enableRandomAddrMode,
    const sptr<IBluetoothBleCentralManagerCallback> &callback)
{
    int32_t pid = IPCSkeleton::GetCallingPid();
    int32_t uid = IPCSkeleton::GetCallingUid();
    HILOGI("pid: %{public}d, uid: %{public}d", pid, uid);

    if (callback == nullptr) {
        HILOGE("callback is null");
        return;
    }
    auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
    if (bleService == nullptr) {
        HILOGE("bleService is null");
        return;
    }
    scannerId = bleService->AllocScannerId();
    if (scannerId == 0) {
        HILOGE("Alloc ScannerId fail.");
        return;
    }

    pimpl->eventHandler_->PostSyncTask([&]() {
        if (pimpl != nullptr) {
            pimpl->observersToken_.EnsureInsert(callback->AsObject(), IPCSkeleton::GetCallingTokenID());
            pimpl->observersPid_[callback->AsObject()] = pid;
            pimpl->observersScannerId_[callback->AsObject()] = scannerId;
            auto func = std::bind(&BluetoothBleCentralManagerServer::DeregisterBleCentralManagerCallbackInner,
                this, std::placeholders::_1, std::placeholders::_2);
            pimpl->observers_.Register(callback, func, scannerId);
            impl::ScanCallbackInfo info;
            info.pid = pid;
            info.uid = uid;
            info.isStart = false;
            info.callback = callback;
            pimpl->scanCallbackInfo_.push_back(info);
        }
    });
}

void BluetoothBleCentralManagerServer::DeregisterBleCentralManagerCallbackInner(
    const sptr<IBluetoothBleCentralManagerCallback> &callback, int32_t scannerId)
{
    return DeregisterBleCentralManagerCallback(scannerId, callback);
}

int BluetoothBleCentralManagerServer::ChangeScanParams(int32_t scannerId, const BluetoothBleScanSettings &settings,
    const std::vector<BluetoothBleScanFilter> &filters, uint32_t filterAction)
{
    return BT_ERR_API_NOT_SUPPORT;
}

void BluetoothBleCentralManagerServer::DeregisterBleCentralManagerCallback(int32_t scannerId,
    const sptr<IBluetoothBleCentralManagerCallback> &callback)
{
    HILOGI("enter, scannerId: %{public}d", scannerId);
    pimpl->eventHandler_->PostSyncTask([&]() {
        if (callback == nullptr || pimpl == nullptr) {
            HILOGE("DeregisterBleCentralManagerCallback(): callback is null, or pimpl is null");
            return;
        }
        for (auto iter = pimpl->scanCallbackInfo_.begin(); iter != pimpl->scanCallbackInfo_.end(); ++iter) {
            if (iter->callback->AsObject() == callback->AsObject()) {
                pimpl->observers_.Deregister(iter->callback);
                pimpl->scanCallbackInfo_.erase(iter);
                break;
            }
        }
        pimpl->observersToken_.Erase(callback->AsObject());

        for (auto iter = pimpl->observersPid_.begin(); iter != pimpl->observersPid_.end(); ++iter) {
            if (iter->first == callback->AsObject()) {
                pimpl->observersPid_.erase(iter);
                break;
            }
        }
        for (auto iter = pimpl->observersScannerId_.begin(); iter != pimpl->observersScannerId_.end(); ++iter) {
            if (iter->first == callback->AsObject()) {
                pimpl->observersScannerId_.erase(iter);
                break;
            }
        }
    });

    auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
    if (bleService == nullptr) {
        HILOGE("bleService is null");
        return;
    }
    bleService->RemoveScannerId(scannerId);
}

void BluetoothBleCentralManagerServer::SetScanParams(const BluetoothBleScanSettings &settings)
{
    if (pimpl == nullptr) {
        HILOGE("pimpl is nullptr");
    }
    pimpl->scanSettingImpl_.SetReportDelay(settings.GetReportDelayMillisValue());
    pimpl->scanSettingImpl_.SetScanMode(settings.GetScanMode());
    pimpl->scanSettingImpl_.SetLegacy(settings.GetLegacy());
    pimpl->scanSettingImpl_.SetPhy(settings.GetPhy());
}

void BluetoothBleCentralManagerServer::SetWindowAndInterval(const int mode, uint16_t &window, uint16_t &interval)
{
    switch (mode) {
        case SCAN_MODE_LOW_POWER:
            window = BLE_SCAN_MODE_LOW_POWER_WINDOW_MS;
            interval = BLE_SCAN_MODE_LOW_POWER_INTERVAL_MS;
            break;
        case SCAN_MODE_BALANCED:
            window = BLE_SCAN_MODE_BALANCED_WINDOW_MS;
            interval = BLE_SCAN_MODE_BALANCED_INTERVAL_MS;
            break;
        case SCAN_MODE_LOW_LATENCY:
            window = BLE_SCAN_MODE_LOW_LATENCY_WINDOW_MS;
            interval = BLE_SCAN_MODE_LOW_LATENCY_INTERVAL_MS;
            break;
        case SCAN_MODE_OP_P2_60_3000:
            window = BLE_SCAN_MODE_OP_P2_60_3000_WINDOW_MS;
            interval = BLE_SCAN_MODE_OP_P2_60_3000_INTERVAL_MS;
            break;
        case SCAN_MODE_OP_P10_60_600:
            window = BLE_SCAN_MODE_OP_P10_60_600_WINDOW_MS;
            interval = BLE_SCAN_MODE_OP_P10_60_600_INTERVAL_MS;
            break;
        case SCAN_MODE_OP_P25_60_240:
            window = BLE_SCAN_MODE_OP_P25_60_240_WINDOW_MS;
            interval = BLE_SCAN_MODE_OP_P25_60_240_INTERVAL_MS;
            break;
        case SCAN_MODE_OP_P100_1000_1000:
            window = BLE_SCAN_MODE_OP_P100_1000_1000_WINDOW_MS;
            interval = BLE_SCAN_MODE_OP_P100_1000_1000_INTERVAL_MS;
            break;
        case SCAN_MODE_OP_P50_100_200:
            window = BLE_SCAN_MODE_OP_P50_100_200_WINDOW_MS;
            interval = BLE_SCAN_MODE_OP_P50_100_200_INTERVAL_MS;
            break;
        case SCAN_MODE_OP_P10_30_300:
            window = BLE_SCAN_MODE_OP_P10_30_300_WINDOW_MS;
            interval = BLE_SCAN_MODE_OP_P10_30_300_INTERVAL_MS;
            break;
        default:
            SetOtherWindowAndInterval(mode, window, interval);
            break;
    }
}

void BluetoothBleCentralManagerServer::SetOtherWindowAndInterval(const int mode, uint16_t &window, uint16_t &interval)
{
    switch (mode) {
        case SCAN_MODE_OP_P2_30_1500:
            window = BLE_SCAN_MODE_OP_P2_30_1500_WINDOW_MS;
            interval = BLE_SCAN_MODE_OP_P2_30_1500_INTERVAL_MS;
            break;
        case SCAN_MODE_OP_P75_30_40:
            window = BLE_SCAN_MODE_OP_P75_30_40_WINDOW_MS;
            interval = BLE_SCAN_MODE_OP_P75_30_40_INTERVAL_MS;
            break;
        case SCAN_MODE_OP_P50_30_60:
            window = BLE_SCAN_MODE_OP_P50_30_60_WINDOW_MS;
            interval = BLE_SCAN_MODE_OP_P50_30_60_INTERVAL_MS;
            break;
        default:
            HILOGE("invalid scan mode.");
            break;
    }
}

bool BluetoothBleCentralManagerServer::IsNewScanParams()
{
    int pid;
    impl::ScanSettingsParam max;
    auto iter = pimpl->scanCallbackInfo_.begin();
    for (; iter != pimpl->scanCallbackInfo_.end(); ++iter) {
        if (iter->isStart) {
            max = iter->param;
            pid = iter->pid;
            break;
        }
    }
    if (iter == pimpl->scanCallbackInfo_.end()) {
        HILOGI("all is stop.");
        return false;
    }

    for (; iter != pimpl->scanCallbackInfo_.end(); ++iter) {
        if (!iter->isStart) {
            continue;
        }
        double maxDutyCycle = 1.0 * max.scanWindow / max.scanInterval;
        double currDutyCycle = 1.0 * iter->param.scanWindow / iter->param.scanInterval;
        if ((currDutyCycle > maxDutyCycle) ||
            (currDutyCycle == maxDutyCycle && iter->param.scanInterval < max.scanInterval)) {
            max = iter->param;
            pid = iter->pid;
        }
    }

    HILOGI("maxPid=%{public}d, maxScanMode=%{public}s, currScanMode=%{public}s",
        pid, GetScanModeName(max.scanMode).c_str(), GetScanModeName(pimpl->scanSettingImpl_.GetScanMode()).c_str());
    if (pimpl->scanSettingImpl_.GetReportDelayMillisValue() != max.reportDelayMillis ||
        pimpl->scanSettingImpl_.GetScanMode() != max.scanMode ||
        pimpl->scanSettingImpl_.GetLegacy() != max.legacy ||
        pimpl->scanSettingImpl_.GetPhy() != max.phy) {
        pimpl->scanSettingImpl_.SetReportDelay(max.reportDelayMillis);
        pimpl->scanSettingImpl_.SetScanMode(max.scanMode);
        pimpl->scanSettingImpl_.SetLegacy(max.legacy);
        pimpl->scanSettingImpl_.SetPhy(max.phy);
        return true;
    }
    return false;
}

bool BluetoothBleCentralManagerServer::IsAllStop()
{
    for (auto iter = pimpl->scanCallbackInfo_.begin(); iter != pimpl->scanCallbackInfo_.end(); ++iter) {
        if (iter->isStart) {
            return false;
        }
    }

    HILOGI("all is stop.");
    pimpl->scanSettingImpl_.SetReportDelay(0);
    pimpl->scanSettingImpl_.SetScanMode(SCAN_MODE_LOW_POWER);
    pimpl->scanSettingImpl_.SetLegacy(true);
    pimpl->scanSettingImpl_.SetPhy(PHY_LE_ALL_SUPPORTED);
    return true;
}

int BluetoothBleCentralManagerServer::SetLpDeviceAdvParam(int duration, int maxExtAdvEvents,
    int window, int interval, int advHandle)
{
    HILOGI("NOT SUPPORT NOW");
    return NO_ERROR;
}

int BluetoothBleCentralManagerServer::SetScanReportChannelToLpDevice(int32_t scannerId, bool enable)
{
    HILOGI("NOT SUPPORT NOW");
    return NO_ERROR;
}

int BluetoothBleCentralManagerServer::EnableSyncDataToLpDevice()
{
    HILOGI("NOT SUPPORT NOW");
    return NO_ERROR;
}

int BluetoothBleCentralManagerServer::DisableSyncDataToLpDevice()
{
    HILOGI("NOT SUPPORT NOW");
    return NO_ERROR;
}

int BluetoothBleCentralManagerServer::SendParamsToLpDevice(const std::vector<uint8_t> &dataValue, int32_t type)
{
    HILOGI("NOT SUPPORT NOW");
    return NO_ERROR;
}

bool BluetoothBleCentralManagerServer::IsLpDeviceAvailable()
{
    HILOGI("NOT SUPPORT NOW");
    return false;
}

int BluetoothBleCentralManagerServer::SetLpDeviceParam(const BluetoothLpDeviceParamSet &paramSet)
{
    HILOGI("NOT SUPPORT NOW");
    return NO_ERROR;
}

int BluetoothBleCentralManagerServer::RemoveLpDeviceParam(const bluetooth::Uuid &uuid)
{
    HILOGI("NOT SUPPORT NOW");
    return NO_ERROR;
}
}  // namespace Bluetooth
}  // namespace OHOS
