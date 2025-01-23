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

#include "bluetooth_def.h"
#include "bluetooth_errorcode.h"
#include "bluetooth_log.h"
#include "bluetooth_utils_server.h"
#include "interface_profile.h"
#include "interface_profile_hid_host.h"
#include "i_bluetooth_host_observer.h"
#include "permission_utils.h"
#include "remote_observer_list.h"
#include "hilog/log.h"
#include "bluetooth_hid_host_server.h"

namespace OHOS {
namespace Bluetooth {
class BluetoothHidHostCallback : public bluetooth::IHidHostObserver {
public:
    BluetoothHidHostCallback() = default;
    ~BluetoothHidHostCallback() override = default;

    void OnConnectionStateChanged(const RawAddress &device, int state) override
    {
        // Reference "BTConnectState"
        HILOGI("addr:%{public}s, state:%{public}d", GET_ENCRYPT_ADDR(device), state);
        observers_->ForEach([device, state](sptr<IBluetoothHidHostObserver> observer) {
            observer->OnConnectionStateChanged(device, state,
                static_cast<uint32_t>(ConnChangeCause::CONNECT_CHANGE_COMMON_CAUSE));
        });
    }

    void SetObserver(RemoteObserverList<IBluetoothHidHostObserver> *observers)
    {
        observers_ = observers;
    }

private:
    RemoteObserverList<IBluetoothHidHostObserver> *observers_;
};

struct BluetoothHidHostServer::impl {
    impl();
    ~impl();

    class SystemStateObserver;
    std::unique_ptr<SystemStateObserver> systemStateObserver_ = nullptr;

    RemoteObserverList<IBluetoothHidHostObserver> observers_;
    std::unique_ptr<BluetoothHidHostCallback> observerImp_ = std::make_unique<BluetoothHidHostCallback>();
    IProfileHidHost *hidHostService_ = nullptr;
    std::vector<sptr<IBluetoothHidHostObserver>> advCallBack_;
    std::mutex advCallBackMutex;

    IProfileHidHost *GetServicePtr()
    {
        if (IProfileManager::GetInstance() == nullptr) {
            return nullptr;
        }
        return static_cast<IProfileHidHost *>(
            IProfileManager::GetInstance()->GetProfileService(PROFILE_NAME_HID_HOST));
    }
};

class BluetoothHidHostServer::impl::SystemStateObserver : public ISystemStateObserver {
public:
    explicit SystemStateObserver(BluetoothHidHostServer::impl *pimpl) : pimpl_(pimpl) {};
    void OnSystemStateChange(const BTSystemState state) override
    {
        HILOGI("start, BTSystemState:%{public}d", state);
        switch (state) {
            case BTSystemState::ON:
                pimpl_->hidHostService_ = pimpl_->GetServicePtr();
                if (pimpl_->hidHostService_ != nullptr) {
                    pimpl_->hidHostService_->RegisterObserver(*pimpl_->observerImp_.get());
                }
                break;
            case BTSystemState::OFF:
                pimpl_->hidHostService_ = nullptr;
                break;
            default:
                break;
        }
    };

private:
    BluetoothHidHostServer::impl *pimpl_ = nullptr;
};

BluetoothHidHostServer::impl::impl()
{
    HILOGI("enter");
}

BluetoothHidHostServer::impl::~impl()
{
    HILOGI("enter");
}

BluetoothHidHostServer::BluetoothHidHostServer()
{
    HILOGI("start");
    pimpl = std::make_unique<impl>();
    pimpl->observerImp_->SetObserver(&(pimpl->observers_));
    pimpl->systemStateObserver_ = std::make_unique<impl::SystemStateObserver>(pimpl.get());
    IAdapterManager::GetInstance()->RegisterSystemStateObserver(*(pimpl->systemStateObserver_));

    pimpl->hidHostService_ = pimpl->GetServicePtr();
    if (pimpl->hidHostService_ != nullptr) {
        pimpl->hidHostService_->RegisterObserver(*pimpl->observerImp_.get());
    }
}

BluetoothHidHostServer::~BluetoothHidHostServer()
{
    HILOGI("start");
    IAdapterManager::GetInstance()->DeregisterSystemStateObserver(*(pimpl->systemStateObserver_));
    if (pimpl->hidHostService_ != nullptr) {
        pimpl->hidHostService_->DeregisterObserver(*pimpl->observerImp_.get());
    }
}

ErrCode BluetoothHidHostServer::RegisterObserver(const sptr<IBluetoothHidHostObserver> observer)
{
    HILOGI("start");

    if (observer == nullptr) {
        HILOGE("observer is null");
        return ERR_INVALID_VALUE;
    }
    if (pimpl == nullptr) {
        HILOGE("pimpl is null");
        return ERR_NO_INIT;
    }
    auto func = std::bind(&BluetoothHidHostServer::DeregisterObserver, this, std::placeholders::_1);
    pimpl->observers_.Register(observer, func);
    std::lock_guard<std::mutex> lock(pimpl->advCallBackMutex);
    pimpl->advCallBack_.push_back(observer);
    return ERR_OK;
}

ErrCode BluetoothHidHostServer::DeregisterObserver(const sptr<IBluetoothHidHostObserver> observer)
{
    HILOGI("start");
    if (observer == nullptr) {
        HILOGE("observer is null");
        return ERR_INVALID_VALUE;
    }
    if (pimpl == nullptr) {
        HILOGE("pimpl is null");
        return ERR_NO_INIT;
    }
    {
        std::lock_guard<std::mutex> lock(pimpl->advCallBackMutex);
        for (auto iter = pimpl->advCallBack_.begin(); iter != pimpl->advCallBack_.end(); ++iter) {
            if ((*iter)->AsObject() == observer->AsObject()) {
                if (pimpl != nullptr) {
                    pimpl->observers_.Deregister(*iter);
                    pimpl->advCallBack_.erase(iter);
                    break;
                }
            }
        }
    }
    if (pimpl->observerImp_ == nullptr) {
        HILOGE("pimpl->observerImp_ is null");
        return ERR_NO_INIT;
    }
    pimpl->hidHostService_->DeregisterObserver(*pimpl->observerImp_.get());
    return ERR_OK;
}

int32_t BluetoothHidHostServer::GetDevicesByStates(
    const std::vector<int32_t> &states, std::vector<BluetoothRawAddress>& result)
{
    HILOGI("start");
    if (PermissionUtils::VerifyUseBluetoothPermission() == PERMISSION_DENIED) {
        HILOGE("check permission failed");
        return BT_ERR_PERMISSION_FAILED;
    }
    if (pimpl == nullptr || pimpl->hidHostService_ == nullptr) {
        HILOGI("hidHostService_ is null");
        return BT_ERR_INTERNAL_ERROR;
    }

    std::vector<bluetooth::RawAddress> serviceDeviceList = pimpl->hidHostService_->GetDevicesByStates(states);
    for (auto &device : serviceDeviceList) {
        BluetoothRawAddress bluetoothDevice(device.GetAddress());
        result.push_back(bluetoothDevice);
        HILOGI("%{public}s", GET_ENCRYPT_ADDR(bluetoothDevice));
    }
    return NO_ERROR;
}

int32_t BluetoothHidHostServer::GetDeviceState(const BluetoothRawAddress &device, int32_t &state)
{
    HILOGI("start, addr:%{public}s", GET_ENCRYPT_ADDR(device));
    if (PermissionUtils::VerifyUseBluetoothPermission() == PERMISSION_DENIED) {
        HILOGE("check permission failed");
        return BT_ERR_PERMISSION_FAILED;
    }
    if (pimpl == nullptr || pimpl->hidHostService_ == nullptr) {
        HILOGI("hidHostService_ is null");
        return BT_ERR_INTERNAL_ERROR;
    }
    state = pimpl->hidHostService_->GetDeviceState(device);
    HILOGI("end, result:%{public}d", state);
    return NO_ERROR;
}

int32_t BluetoothHidHostServer::Connect(const BluetoothRawAddress &device)
{
    HILOGI("start, addr:%{public}s", GET_ENCRYPT_ADDR(device));
    if (!PermissionUtils::CheckSystemHapApp()) {
        HILOGE("check system api failed.");
        return BT_ERR_SYSTEM_PERMISSION_FAILED;
    }
    if (PermissionUtils::VerifyDiscoverBluetoothPermission() == PERMISSION_DENIED) {
        HILOGE("check permission failed");
        return BT_ERR_PERMISSION_FAILED;
    }
    if (pimpl == nullptr || pimpl->hidHostService_ == nullptr) {
        HILOGE("hidHostService_ is null");
        return BT_ERR_INTERNAL_ERROR;
    }
    return pimpl->hidHostService_->Connect(device);
}

int32_t BluetoothHidHostServer::Disconnect(const BluetoothRawAddress &device)
{
    HILOGI("start, addr:%{public}s", GET_ENCRYPT_ADDR(device));
    if (!PermissionUtils::CheckSystemHapApp()) {
        HILOGE("check system api failed.");
        return BT_ERR_SYSTEM_PERMISSION_FAILED;
    }
    if (PermissionUtils::VerifyDiscoverBluetoothPermission() == PERMISSION_DENIED) {
        HILOGE("check permission failed");
        return BT_ERR_PERMISSION_FAILED;
    }
    if (pimpl == nullptr || pimpl->hidHostService_ == nullptr) {
        HILOGI("hidHostService_ is null");
        return BT_ERR_INTERNAL_ERROR;
    }
    return pimpl->hidHostService_->Disconnect(device);
}

ErrCode BluetoothHidHostServer::HidHostVCUnplug(std::string &device,
    uint8_t &id, uint16_t &size, uint8_t &type, int& result)
{
    HILOGI("start");
    if (PermissionUtils::VerifyDiscoverBluetoothPermission() == PERMISSION_DENIED) {
        HILOGE("check permission failed");
        return ERR_PERMISSION_DENIED;
    }
    if (pimpl == nullptr || pimpl->hidHostService_ == nullptr) {
        HILOGI("hidHostService_ is null");
        return ERR_NO_INIT;
    }
    result = pimpl->hidHostService_->HidHostVCUnplug(device, id, size, type);
    HILOGI("end, result:%{public}d", result);
    return ERR_OK;
}

ErrCode BluetoothHidHostServer::HidHostSendData(std::string &device,
    uint8_t &id, uint16_t &size, uint8_t &type, int& result)
{
    HILOGI("start");
    if (PermissionUtils::VerifyDiscoverBluetoothPermission() == PERMISSION_DENIED) {
        HILOGE("check permission failed");
        return ERR_PERMISSION_DENIED;
    }
    if (pimpl == nullptr || pimpl->hidHostService_ == nullptr) {
        HILOGI("hidHostService_ is null");
        return ERR_NO_INIT;
    }
    result = pimpl->hidHostService_->HidHostSendData(device, id, size, type);
    HILOGI("end, result:%{public}d", result);
    return ERR_OK;
}

ErrCode BluetoothHidHostServer::HidHostSetReport(std::string &device,
    uint8_t &type, std::string &report, int& result)
{
    HILOGI("start");
    if (PermissionUtils::VerifyDiscoverBluetoothPermission() == PERMISSION_DENIED) {
        HILOGE("check permission failed");
        return ERR_PERMISSION_DENIED;
    }
    if (pimpl == nullptr || pimpl->hidHostService_ == nullptr) {
        HILOGI("hidHostService_ is null");
        return ERR_NO_INIT;
    }
    std::vector<uint8_t> data;
    for (char ch : report) {
        data.emplace(data.end(), static_cast<uint8_t>(ch));
    }
    data.emplace(data.end(), static_cast<uint8_t>('\0'));
    result = pimpl->hidHostService_->HidHostSetReport(device, type, data.size(), data.data());
    HILOGI("end, result:%{public}d", result);
    return ERR_OK;
}

ErrCode BluetoothHidHostServer::HidHostGetReport(std::string &device,
    uint8_t &id, uint16_t &size, uint8_t &type, int& result)
{
    HILOGI("start");
    if (PermissionUtils::VerifyDiscoverBluetoothPermission() == PERMISSION_DENIED) {
        HILOGE("check permission failed");
        return ERR_PERMISSION_DENIED;
    }
    if (pimpl == nullptr || pimpl->hidHostService_ == nullptr) {
        HILOGI("hidHostService_ is null");
        return ERR_NO_INIT;
    }
    result = pimpl->hidHostService_->HidHostGetReport(device, id, size, type);
    HILOGI("end, result:%{public}d", result);
    return ERR_OK;
}

int32_t BluetoothHidHostServer::SetConnectStrategy(const BluetoothRawAddress &device, int strategy)
{
    HILOGI("target device:%{public}s()", GET_ENCRYPT_ADDR(device));
    if (!PermissionUtils::CheckSystemHapApp()) {
        HILOGE("check system api failed.");
        return BT_ERR_SYSTEM_PERMISSION_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHidHostServer::GetConnectStrategy(const BluetoothRawAddress &device, int &strategy)
{
    return NO_ERROR;
}
}  // namespace Bluetooth
}  // namespace OHOS
