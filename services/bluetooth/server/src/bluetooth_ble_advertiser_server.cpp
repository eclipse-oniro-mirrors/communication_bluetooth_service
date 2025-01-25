/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#include "bluetooth_ble_advertiser_server.h"

#include "bluetooth_ble_central_manager_server.h"
#include "bluetooth_errorcode.h"
#include "bluetooth_log.h"
#include "interface_adapter_ble.h"
#include "interface_adapter_manager.h"
#include "ipc_skeleton.h"
#include "remote_observer_list.h"
#include "permission_utils.h"
#include "safe_map.h"

namespace OHOS {
namespace Bluetooth {
using namespace OHOS::bluetooth;
class BleAdvertiserCallback : public IBleAdvertiserCallback {
public:
    BleAdvertiserCallback() = default;
    ~BleAdvertiserCallback() override = default;

    void OnStartResultEvent(int result, uint8_t advHandle, int opcode) override
    {
        HILOGI("result: %{public}d, advHandle: %{public}d, opcode: %{public}d", result, advHandle, opcode);

        observers_->ForEach([this, result, advHandle, opcode](IBluetoothBleAdvertiseCallback *observer) {
            int32_t pid = observersPid_.ReadVal(observer->AsObject());
            if (BluetoothBleCentralManagerServer::IsResourceScheduleControlApp(pid)) {
                HILOGD("pid:%{public}d is proxy pid, not callback.", pid);
                return;
            }
            observer->OnStartResultEvent(result, advHandle, opcode);
        });
    }

    void OnEnableResultEvent(int result, uint8_t advHandle) override
    {}

    void OnDisableResultEvent(int result, uint8_t advHandle) override
    {}

    void OnStopResultEvent(int result, uint8_t advHandle) override
    {
        HILOGI("result: %{public}d, advHandle: %{public}d", result, advHandle);

        observers_->ForEach([this, result, advHandle](IBluetoothBleAdvertiseCallback *observer) {
            int32_t pid = observersPid_.ReadVal(observer->AsObject());
            if (BluetoothBleCentralManagerServer::IsResourceScheduleControlApp(pid)) {
                HILOGD("pid:%{public}d is proxy pid, not callback.", pid);
                return;
            }
            observer->OnStopResultEvent(result, advHandle);
        });
    }

    void OnAutoStopAdvEvent(uint8_t advHandle) override
    {
        HILOGI("advHandle: %{public}d", advHandle);

        observers_->ForEach(
            [advHandle](IBluetoothBleAdvertiseCallback *observer) { observer->OnAutoStopAdvEvent(advHandle); });
    }

    void OnSetAdvDataEvent(int32_t result, int32_t advHandle) override
    {
        return;
    }

    void SetObserver(RemoteObserverList<IBluetoothBleAdvertiseCallback> *observers)
    {
        observers_ = observers;
    }

    SafeMap<sptr<IRemoteObject>, int32_t> observersPid_;

private:
    RemoteObserverList<IBluetoothBleAdvertiseCallback> *observers_;
};

struct BluetoothBleAdvertiserServer::impl {
    impl();
    ~impl();

    BleAdvertiserDataImpl ConvertAdvertisingData(const BluetoothBleAdvertiserData &data) const;

    /// sys state observer
    class SystemStateObserver;
    std::unique_ptr<SystemStateObserver> systemStateObserver_ = nullptr;

    RemoteObserverList<IBluetoothBleAdvertiseCallback> observers_;
    std::unique_ptr<BleAdvertiserCallback> observerImp_ = std::make_unique<BleAdvertiserCallback>();
    std::vector<sptr<IBluetoothBleAdvertiseCallback>> advCallBack_;
    std::mutex advCallBackMutex;
};

class BluetoothBleAdvertiserServer::impl::SystemStateObserver : public ISystemStateObserver {
public:
    explicit SystemStateObserver(BluetoothBleAdvertiserServer::impl *pimpl) : pimpl_(pimpl) {};
    void OnSystemStateChange(const BTSystemState state) override
    {
        auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
        switch (state) {
            case BTSystemState::ON:
                if (bleService != nullptr) {
                    bleService->RegisterBleAdvertiserCallback(*pimpl_->observerImp_.get());
                }
                break;
            default:
                break;
        }
    };

private:
    BluetoothBleAdvertiserServer::impl *pimpl_ = nullptr;
};

BluetoothBleAdvertiserServer::impl::impl()
{}

BluetoothBleAdvertiserServer::impl::~impl()
{
    auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
    if (bleService != nullptr) {
        bleService->DeregisterBleAdvertiserCallback();
    }
}

BluetoothBleAdvertiserServer::BluetoothBleAdvertiserServer()
{
    pimpl = std::make_unique<impl>();
    pimpl->observerImp_->SetObserver(&(pimpl->observers_));
    pimpl->systemStateObserver_ = std::make_unique<impl::SystemStateObserver>(pimpl.get());
    IAdapterManager::GetInstance()->RegisterSystemStateObserver(*(pimpl->systemStateObserver_));

    auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
    if (bleService != nullptr) {
        bleService->RegisterBleAdvertiserCallback(*pimpl->observerImp_.get());
    }
}

BluetoothBleAdvertiserServer::~BluetoothBleAdvertiserServer()
{
    IAdapterManager::GetInstance()->DeregisterSystemStateObserver(*(pimpl->systemStateObserver_));
}

BleAdvertiserDataImpl BluetoothBleAdvertiserServer::impl::ConvertAdvertisingData(
    const BluetoothBleAdvertiserData &data) const
{
    BleAdvertiserDataImpl outData;

    std::map<uint16_t, std::string> manufacturerData = data.GetManufacturerData();
    for (auto iter = manufacturerData.begin(); iter != manufacturerData.end(); iter++) {
        outData.AddManufacturerData(iter->first, iter->second);
    }
    std::map<Uuid, std::string> serviceData = data.GetServiceData();
    for (auto it = serviceData.begin(); it != serviceData.end(); it++) {
        outData.AddServiceData(it->first, it->second);
    }
    std::vector<Uuid> serviceUuids = data.GetServiceUuids();
    outData.AddServiceUuids(serviceUuids);
    outData.AddData(data.GetPayload());

    return outData;
}

int BluetoothBleAdvertiserServer::StartAdvertising(const BluetoothBleAdvertiserSettings &settings,
    const BluetoothBleAdvertiserData &advData, const BluetoothBleAdvertiserData &scanResponse, int32_t advHandle,
    uint16_t duration, bool isRawData)
{
    HILOGI("enter");
    if (PermissionUtils::VerifyDiscoverBluetoothPermission() == PERMISSION_DENIED) {
        HILOGE("check permission failed");
        return BT_ERR_PERMISSION_FAILED;
    }

    auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
    if (bleService != nullptr) {
        BleAdvertiserSettingsImpl settingsImpl;
        settingsImpl.SetConnectable(settings.IsConnectable());
        settingsImpl.SetInterval(settings.GetInterval());
        settingsImpl.SetLegacyMode(settings.IsLegacyMode());
        settingsImpl.SetTxPower(settings.GetTxPower());

        BleAdvertiserDataImpl bleAdvertiserData = pimpl->ConvertAdvertisingData(advData);
        if (!isRawData) {
            bleAdvertiserData.SetFlags(advData.GetAdvFlag());
        }
        BleAdvertiserDataImpl bleScanResponse = pimpl->ConvertAdvertisingData(scanResponse);
        HILOGI("NOT support duration now");
        bleService->StartAdvertising(settingsImpl, bleAdvertiserData, bleScanResponse, advHandle);
    }
    return NO_ERROR;
}

int BluetoothBleAdvertiserServer::EnableAdvertising(uint8_t advHandle, uint16_t duration)
{
    HILOGI("NOT SUPPORT NOW");
    return NO_ERROR;
}

int BluetoothBleAdvertiserServer::DisableAdvertising(uint8_t advHandle)
{
    HILOGI("NOT SUPPORT NOW");
    return NO_ERROR;
}

int BluetoothBleAdvertiserServer::StopAdvertising(int32_t advHandle)
{
    HILOGI("enter, advHandle: %{public}d", advHandle);
    if (PermissionUtils::VerifyDiscoverBluetoothPermission() == PERMISSION_DENIED) {
        HILOGE("check permission failed");
        return BT_ERR_PERMISSION_FAILED;
    }

    auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
    if (bleService != nullptr) {
        bleService->StopAdvertising(advHandle);
    }
    pimpl->observerImp_->OnStopResultEvent(NO_ERROR, advHandle);
    return NO_ERROR;
}

void BluetoothBleAdvertiserServer::Close(int32_t advHandle)
{
    HILOGI("enter, advHandle: %{public}d", advHandle);

    auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
    if (bleService != nullptr) {
        bleService->Close(advHandle);
    }
}

void BluetoothBleAdvertiserServer::RegisterBleAdvertiserCallback(const sptr<IBluetoothBleAdvertiseCallback> &callback)
{
    HILOGI("enter");

    if (callback == nullptr) {
        HILOGE("callback is null");
        return;
    }
    if (pimpl != nullptr) {
        pimpl->observerImp_->observersPid_.EnsureInsert(callback->AsObject(), IPCSkeleton::GetCallingPid());
        auto func = std::bind(&BluetoothBleAdvertiserServer::DeregisterBleAdvertiserCallback,
            this, std::placeholders::_1);
        pimpl->observers_.Register(callback, func);
        std::lock_guard<std::mutex> lock(pimpl->advCallBackMutex);
        pimpl->advCallBack_.push_back(callback);
    }
}

void BluetoothBleAdvertiserServer::DeregisterBleAdvertiserCallback(const sptr<IBluetoothBleAdvertiseCallback> &callback)
{
    HILOGI("enter");

    if (callback == nullptr || pimpl == nullptr) {
        HILOGE("callback is null, or pimpl is null");
        return;
    }
    {
        std::lock_guard<std::mutex> lock(pimpl->advCallBackMutex);
        for (auto iter = pimpl->advCallBack_.begin(); iter != pimpl->advCallBack_.end(); ++iter) {
            if ((*iter)->AsObject() == callback->AsObject()) {
                HILOGI("Deregister observer");
                pimpl->observers_.Deregister(*iter);
                pimpl->advCallBack_.erase(iter);
                break;
            }
        }
    }
    pimpl->observerImp_->observersPid_.Erase(callback->AsObject());
}

int32_t BluetoothBleAdvertiserServer::GetAdvertiserHandle(int32_t &advHandle)
{
    HILOGI("enter");
    if (PermissionUtils::VerifyDiscoverBluetoothPermission() == PERMISSION_DENIED) {
        HILOGE("check permission failed");
        return BT_ERR_PERMISSION_FAILED;
    }
    auto bleService = IAdapterManager::GetInstance()->GetBleAdapterInterface();
    if (bleService == nullptr) {
        return BT_ERR_INTERNAL_ERROR;
    }
    advHandle = bleService->GetAdvertiserHandle();
    if (advHandle == BLE_INVALID_ADVERTISING_HANDLE) {
        return BT_ERR_INTERNAL_ERROR;
    }

    return NO_ERROR;
}

void BluetoothBleAdvertiserServer::SetAdvertisingData(const BluetoothBleAdvertiserData &advData,
    const BluetoothBleAdvertiserData &scanResponse, int32_t advHandle)
{
    HILOGI("NOT SUPPORT NOW");
    return;
}

int BluetoothBleAdvertiserServer::ChangeAdvertisingParams(uint8_t advHandle,
    const BluetoothBleAdvertiserSettings &settings)
{
    HILOGI("NOT SUPPORT NOW");
    return BT_ERR_API_NOT_SUPPORT;
}
}  // namespace Bluetooth
}  // namespace OHOS
