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

#include "adapter_manager.h"

#include <array>
#include <functional>
#include <unistd.h>

#include "btm.h"
#include "btstack.h"
#include "log.h"
#include "log_util.h"

#include "adapter_config.h"
#include "adapter_device_config.h"
#include "adapter_device_info.h"
#include "adapter_state_machine.h"
#include "base_def.h"
#include "base_observer_list.h"
#include "bluetooth_common_event_helper.h"
#include "class_creator.h"
#include "foundation/communication/bluetooth_service/services/bluetooth/service/src/dialog/dialog_switch.h"
#include "interface_adapter_classic.h"
#include "permission_utils.h"
#include "permission_manager.h"
#include "power_manager.h"
#include "profile_config.h"
#include "profile_service_manager.h"
#include "sys_state_machine.h"


namespace OHOS {
namespace bluetooth {
// data define
const int TRANSPORT_MAX = 2;
const std::string PERMISSIONS = "ohos.permission.USE_BLUETOOTH";

// T is BleAdapter or ClassicAdapter
template <typename T>
struct AdapterInfo {
    AdapterInfo(std::shared_ptr<T> instance, std::unique_ptr<AdapterStateMachine> stateMachine)
        : instance(instance), stateMachine(std::move(stateMachine))
    {}
    ~AdapterInfo()
    {}

    BTStateID state = BTStateID::STATE_TURN_OFF;
    std::shared_ptr<T> instance = nullptr;
    std::unique_ptr<AdapterStateMachine> stateMachine = nullptr;
};

// static function
IAdapterManager *IAdapterManager::GetInstance()
{
    return AdapterManager::GetInstance();
}
AdapterManager *AdapterManager::GetInstance()
{
    static AdapterManager instance;
    return &instance;
}

// impl class
struct AdapterManager::impl {
    impl();
    ~impl();

    std::recursive_mutex syncMutex_ = {};
    std::promise<void> stopPromise_ = {};
    std::promise<void> resetPromise_ = {};
    std::unique_ptr<utility::Dispatcher> dispatcher_ = nullptr;
    std::unique_ptr<AdapterInfo<ClassicAdapter>> classicAdapter_ = nullptr;
    std::unique_ptr<AdapterInfo<BleAdapter>> bleAdapter_ = nullptr;
    SysStateMachine sysStateMachine_ = {};
    std::string sysState_ = SYS_STATE_STOPPED;
    BtmCallbacks hciFailureCallbacks = {};
    BaseObserverList<IAdapterStateObserver> adapterObservers_ = {};
    BaseObserverList<ISystemStateObserver> systemObservers_ = {};

    class AdaptersContextCallback;
    std::unique_ptr<AdaptersContextCallback> contextCallback_ = nullptr;

    void OnEnable(const std::string &name, bool ret);
    void OnDisable(const std::string &name, bool ret);
    void ProcessMessage(const BTTransport transport, const utility::Message &msg);

    BT_DISALLOW_COPY_AND_ASSIGN(impl);
};

class AdapterManager::impl::AdaptersContextCallback : public utility::IContextCallback {
public:
    explicit AdaptersContextCallback(AdapterManager::impl &impl) : impl_(impl){};
    ~AdaptersContextCallback() = default;

    void OnEnable(const std::string &name, bool ret)
    {
        LOG_DEBUG("%{public}s, name=%{public}s, ret=%{public}d\n", __PRETTY_FUNCTION__, name.c_str(), ret);
        impl_.OnEnable(name, ret);
    }
    void OnDisable(const std::string &name, bool ret)
    {
        LOG_DEBUG("%{public}s, name=%{public}s, ret=%{public}d\n", __PRETTY_FUNCTION__, name.c_str(), ret);
        impl_.OnDisable(name, ret);
    }

private:
    AdapterManager::impl &impl_;
};

AdapterManager::impl::impl()
{
    dispatcher_ = std::make_unique<utility::Dispatcher>("AdapterManager");
    dispatcher_->Initialize();

    // context callback create
    contextCallback_ = std::make_unique<AdaptersContextCallback>(*this);
}

AdapterManager::impl::~impl()
{
    if (dispatcher_ != nullptr) {
        dispatcher_->Uninitialize();
    }
}

void AdapterManager::impl::OnEnable(const std::string &name, bool ret)
{
    LOG_DEBUG("%{public}s, name=%{public}s, ret=%{public}d\n", __PRETTY_FUNCTION__, name.c_str(), ret);
    BTTransport transport = BTTransport::ADAPTER_BREDR;

    if (name.c_str() == ADAPTER_NAME_CLASSIC) {
        transport = BTTransport::ADAPTER_BREDR;
    } else if (name.c_str() == ADAPTER_NAME_BLE) {
        transport = BTTransport::ADAPTER_BLE;
    } else {
        LOG_ERROR("%{public}s, name=%{public}s is warning transport\n", __PRETTY_FUNCTION__, name.c_str());
    }

    utility::Message msg(AdapterStateMachine::MSG_ADAPTER_ENABLE_CMP, ret ? true : false);
    dispatcher_->PostTask(std::bind(&AdapterManager::impl::ProcessMessage, this, transport, msg));
}

void AdapterManager::impl::OnDisable(const std::string &name, bool ret)
{
    LOG_DEBUG("%{public}s, name=%{public}s, ret=%{public}d\n", __PRETTY_FUNCTION__, name.c_str(), ret);
    BTTransport transport = BTTransport::ADAPTER_BREDR;

    if (name.c_str() == ADAPTER_NAME_CLASSIC) {
        transport = BTTransport::ADAPTER_BREDR;
    } else if (name.c_str() == ADAPTER_NAME_BLE) {
        transport = BTTransport::ADAPTER_BLE;
    } else {
        LOG_ERROR("%{public}s, name=%{public}s is warning transport\n", __PRETTY_FUNCTION__, name.c_str());
    }

    utility::Message msg(AdapterStateMachine::MSG_ADAPTER_DISABLE_CMP, ret ? true : false);
    dispatcher_->PostTask(std::bind(&AdapterManager::impl::ProcessMessage, this, transport, msg));
}

void AdapterManager::impl::ProcessMessage(const BTTransport transport, const utility::Message &msg)
{
    std::lock_guard<std::recursive_mutex> lock(syncMutex_);

    if (transport == ADAPTER_BREDR && classicAdapter_ && classicAdapter_->stateMachine) {
        classicAdapter_->stateMachine->ProcessMessage(msg);
        return;
    }

    if (transport == ADAPTER_BLE && bleAdapter_ && bleAdapter_->stateMachine) {
        bleAdapter_->stateMachine->ProcessMessage(msg);
        return;
    }
    LOG_ERROR("%{public}s transport(%{public}d) failed", __PRETTY_FUNCTION__, transport);
}

// AdapterManager class
AdapterManager::AdapterManager() : pimpl(std::make_unique<AdapterManager::impl>())
{
    // sys state Machine create
    pimpl->sysStateMachine_.Init(*this);
}

AdapterManager::~AdapterManager()
{}

bool AdapterManager::Start()
{
    LOG_DEBUG("%{public}s start", __PRETTY_FUNCTION__);

    if (GetSysState() == SYS_STATE_STARTED) {
        LOG_ERROR("Bluetooth has been started!!");
        return false;
    }

    if (!AdapterConfig::GetInstance()->Load()) {
        LOG_ERROR("Load Config File Failed!!");
        return false;
    }

    if (!ProfileConfig::GetInstance()->Load()) {
        LOG_ERROR("Load Profile Config File Failed!!");
        return false;
    }

    if (BTM_Initialize() != BT_SUCCESS) {
        LOG_ERROR("Bluetooth Stack Initialize Failed!!");
        return false;
    }

    if (!OutputSetting()) {
        LOG_ERROR("Bluetooth output set Failed!!");
        return false;
    }

    CreateAdapters();

    ProfileServiceManager::Initialize(*pimpl->dispatcher_);

    IPowerManager::Initialize(*pimpl->dispatcher_);

    RegisterHciResetCallback();

    OnSysStateChange(SYS_STATE_STARTED);

    utility::Message msg(SysStateMachine::MSG_SYS_START_CMP);
    pimpl->dispatcher_->PostTask(std::bind(&utility::StateMachine::ProcessMessage, &(pimpl->sysStateMachine_), msg));

    RestoreTurnOnState();

    return true;
}

bool AdapterManager::OutputSetting() const
{
    bool outputValue = false;
    bool desensitization = false;
    int maxSize = 0;
    AdapterConfig::GetInstance()->GetValue(SECTION_OUTPUT_SETTING, PROPERTY_OUTPUTMAXSIZE, maxSize);
    AdapterConfig::GetInstance()->GetValue(SECTION_OUTPUT_SETTING, PROPERTY_DESENSITIZATION, desensitization);
    if (AdapterConfig::GetInstance()->GetValue(SECTION_OUTPUT_SETTING, PROPERTY_BTSNOOP_OUTPUT, outputValue) &&
        outputValue) {
        std::string outputPath = "./snoop.log";
        AdapterConfig::GetInstance()->GetValue(SECTION_OUTPUT_SETTING, PROPERTY_BTSNOOP_OUTPUT_PATH, outputPath);

        if (BTM_SetSnoopOutputMaxsize(maxSize)) {
            LOG_ERROR("Set snoop file output maxsize Failed!!");
            return false;
        }

        if (BTM_SetSnoopFilePath(outputPath.c_str(), outputPath.length()) != BT_SUCCESS) {
            LOG_ERROR("Set snoop file path Failed!!");
            return false;
        }

        if (BTM_EnableSnoopFileOutput(desensitization) != BT_SUCCESS) {
            LOG_ERROR("Enable snoop file output Failed!!");
            return false;
        }
    } else {
        if (BTM_DisableSnoopFileOutput() != BT_SUCCESS) {
            LOG_ERROR("Disable snoop file output Failed!!");
            return false;
        }
    }

    outputValue = false;
    if (AdapterConfig::GetInstance()->GetValue(SECTION_OUTPUT_SETTING, PROPERTY_HCILOG_OUTPUT, outputValue) &&
        outputValue) {
        if (BTM_SetSnoopOutputMaxsize(maxSize)) {
            LOG_ERROR("Set snoop file output maxsize Failed!!");
            return false;
        }
        
        if (BTM_EnableHciLogOutput(desensitization) != BT_SUCCESS) {
            LOG_ERROR("Enable HciLog output Failed!!");
            return false;
        }
    } else {
        if (BTM_DisableHciLogOutput() != BT_SUCCESS) {
            LOG_ERROR("Disable HciLog output Failed!!");
            return false;
        }
    }

    return true;
}

void AdapterManager::Stop() const
{
    LOG_DEBUG("%{public}s start", __PRETTY_FUNCTION__);

    if (GetSysState() == SYS_STATE_STOPPED) {
        LOG_DEBUG("AdapterManager is stoped");
    } else if (GetSysState() == SYS_STATE_STOPPING) {
        LOG_DEBUG("AdapterManager is stoping...");
    } else {
        std::promise<void> stopPromise;
        std::future<void> stopFuture = stopPromise.get_future();

        {
            std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);
            pimpl->stopPromise_ = std::move(stopPromise);
        }

        utility::Message msg(SysStateMachine::MSG_SYS_STOP_REQ);
        pimpl->dispatcher_->PostTask(
            std::bind(&utility::StateMachine::ProcessMessage, &(pimpl->sysStateMachine_), msg));
        stopFuture.wait();
    }
}

bool AdapterManager::AdapterStop() const
{
    LOG_DEBUG("%{public}s start", __PRETTY_FUNCTION__);
    ProfileServiceManager::Uninitialize();
    IPowerManager::Uninitialize();

    DeregisterHciResetCallback();

    if (pimpl->classicAdapter_) {
        pimpl->classicAdapter_->instance->GetContext()->Uninitialize();
        pimpl->classicAdapter_ = nullptr;
    }
    if (pimpl->bleAdapter_) {
        pimpl->bleAdapter_->instance->GetContext()->Uninitialize();
        pimpl->bleAdapter_ = nullptr;
    }

    BTM_Close();

    utility::Message msg(SysStateMachine::MSG_SYS_STOP_CMP);
    pimpl->dispatcher_->PostTask(std::bind(&utility::StateMachine::ProcessMessage, &(pimpl->sysStateMachine_), msg));

    return true;
}

bool AdapterManager::Enable(const BTTransport transport) const
{
    LOG_DEBUG("%{public}s start transport is %{public}d", __PRETTY_FUNCTION__, transport);
    std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);
    std::string propertynames[] = {PROPERTY_BREDR_TURNON, PROPERTY_BLE_TURNON};

    if (PermissionUtils::VerifyDiscoverBluetoothPermission() == PERMISSION_DENIED) {
        LOG_ERROR("Enable() false, check permission failed");
        return false;
    }

    if (GetSysState() != SYS_STATE_STARTED) {
        LOG_ERROR("AdapterManager system is stoped");
        return false;
    }

    if ((transport == ADAPTER_BREDR && pimpl->classicAdapter_ == nullptr) ||
        (transport == ADAPTER_BLE && pimpl->bleAdapter_ == nullptr)) {
        LOG_INFO("%{public}s BTTransport not register", __PRETTY_FUNCTION__);
        return false;
    }

    if (GetState(transport) == BTStateID::STATE_TURN_OFF) {
        if (!PermissionManager::IsSystemHap() && transport == ADAPTER_BLE &&
            DialogSwitch::RequestBluetoothSwitchDialog(ENABLE_BLUETOOTH)) {
            return true;
        }
        utility::Message msg(AdapterStateMachine::MSG_USER_ENABLE_REQ);
        pimpl->dispatcher_->PostTask(std::bind(&AdapterManager::impl::ProcessMessage, pimpl.get(), transport, msg));

        AdapterDeviceConfig::GetInstance()->SetValue(SECTION_HOST, propertynames[transport], (int)true);
        AdapterDeviceConfig::GetInstance()->Save();

        return true;
    } else if (GetState(transport) == BTStateID::STATE_TURN_ON) {
        LOG_INFO("%{public}s is turn on", __PRETTY_FUNCTION__);
        return false;
    } else {
        LOG_INFO("%{public}s is turning state %{public}d", __PRETTY_FUNCTION__, GetState(transport));
        return false;
    }
}

bool AdapterManager::Disable(const BTTransport transport) const
{
    LOG_DEBUG("%{public}s start transport is %{public}d", __PRETTY_FUNCTION__, transport);
    std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);
    std::string propertynames[] = {PROPERTY_BREDR_TURNON, PROPERTY_BLE_TURNON};

    if (PermissionUtils::VerifyDiscoverBluetoothPermission() == PERMISSION_DENIED) {
        LOG_ERROR("Disable() false, check permission failed");
        return false;
    }

    if ((transport == ADAPTER_BREDR && pimpl->classicAdapter_ == nullptr) ||
        (transport == ADAPTER_BLE && pimpl->bleAdapter_ == nullptr)) {
        LOG_INFO("%{public}s BTTransport not register", __PRETTY_FUNCTION__);
        return false;
    }

    if (GetState(transport) == BTStateID::STATE_TURN_ON) {
        utility::Message msg(AdapterStateMachine::MSG_USER_DISABLE_REQ);
        pimpl->dispatcher_->PostTask(std::bind(&AdapterManager::impl::ProcessMessage, pimpl.get(), transport, msg));

        AdapterDeviceConfig::GetInstance()->SetValue(SECTION_HOST, propertynames[transport], (int)false);
        AdapterDeviceConfig::GetInstance()->Save();
        
        return true;
    } else if (GetState(transport) == BTStateID::STATE_TURN_OFF) {
        LOG_INFO("%{public}s is turn off", __PRETTY_FUNCTION__);
        return false;
    } else {
        LOG_INFO("%{public}s is turning state %{public}d", __PRETTY_FUNCTION__, GetState(transport));
        return false;
    }
}

bool AdapterManager::FactoryReset() const
{
    LOG_DEBUG("%{public}s start", __PRETTY_FUNCTION__);

    if (GetSysState() == SYS_STATE_STARTED) {
        std::promise<void> resetPromise;
        std::future<void> resetFuture = resetPromise.get_future();

        {
            std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);
            pimpl->resetPromise_ = std::move(resetPromise);
        }

        utility::Message msg(SysStateMachine::MSG_SYS_FACTORY_RESET_REQ);
        pimpl->dispatcher_->PostTask(
            std::bind(&utility::StateMachine::ProcessMessage, &(pimpl->sysStateMachine_), msg));
        resetFuture.wait();
        return true;
    } else {
        LOG_INFO("System state is not started");
        return false;
    }
}

void AdapterManager::HciFailedReset(void *context)
{
    LOG_DEBUG("%{public}s start", __PRETTY_FUNCTION__);
    (static_cast<AdapterManager *>(context))->Reset();
}

void AdapterManager::RegisterHciResetCallback()
{
    pimpl->hciFailureCallbacks.hciFailure = HciFailedReset;
    BTM_RegisterCallbacks(&(pimpl->hciFailureCallbacks), this);
}

void AdapterManager::DeregisterHciResetCallback() const
{
    if (pimpl->hciFailureCallbacks.hciFailure != nullptr) {
        BTM_DeregisterCallbacks(&(pimpl->hciFailureCallbacks));
        pimpl->hciFailureCallbacks.hciFailure = nullptr;
    }
}

void AdapterManager::Reset() const
{
    LOG_DEBUG("%{public}s start", __PRETTY_FUNCTION__);

    utility::Message msg(SysStateMachine::MSG_SYS_RESET_REQ);
    pimpl->dispatcher_->PostTask(std::bind(&utility::StateMachine::ProcessMessage, &(pimpl->sysStateMachine_), msg));
}

bool AdapterManager::ClearAllStorage() const
{
    LOG_DEBUG("%{public}s start", __PRETTY_FUNCTION__);

    if (!AdapterConfig::GetInstance()->Reload()) {
        return false;
    }
    if (!ProfileConfig::GetInstance()->Reload()) {
        return false;
    }
    if (!AdapterDeviceConfig::GetInstance()->Reload()) {
        return false;
    }
    if (!AdapterDeviceInfo::GetInstance()->Reload()) {
        return false;
    }

    utility::Message msg(SysStateMachine::MSG_SYS_CLEAR_ALL_STORAGE_CMP);
    pimpl->dispatcher_->PostTask(std::bind(&utility::StateMachine::ProcessMessage, &(pimpl->sysStateMachine_), msg));
    return true;
}

BTStateID AdapterManager::GetState(const BTTransport transport) const
{
    std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);

    BTStateID state = BTStateID::STATE_TURN_OFF;
    if (transport == ADAPTER_BREDR && pimpl->classicAdapter_) {
        state = pimpl->classicAdapter_->state;
    }
    if (transport == ADAPTER_BLE && pimpl->bleAdapter_) {
        state = pimpl->bleAdapter_->state;
    }
    return state;
}

bool AdapterManager::RegisterStateObserver(IAdapterStateObserver &observer) const
{
    return pimpl->adapterObservers_.Register(observer);
}

bool AdapterManager::DeregisterStateObserver(IAdapterStateObserver &observer) const
{
    return pimpl->adapterObservers_.Deregister(observer);
}

bool AdapterManager::RegisterSystemStateObserver(ISystemStateObserver &observer) const
{
    return pimpl->systemObservers_.Register(observer);
}

bool AdapterManager::DeregisterSystemStateObserver(ISystemStateObserver &observer) const
{
    return pimpl->systemObservers_.Deregister(observer);
}

BTConnectState AdapterManager::GetAdapterConnectState() const
{
    LOG_DEBUG("%{public}s start", __PRETTY_FUNCTION__);
    return ProfileServiceManager::GetInstance()->GetProfileServicesConnectState();
}

std::shared_ptr<IAdapterClassic> AdapterManager::GetClassicAdapterInterface(void) const
{
    std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);
    return pimpl->classicAdapter_ != nullptr ? pimpl->classicAdapter_->instance : nullptr;
}

std::shared_ptr<IAdapterBle> AdapterManager::GetBleAdapterInterface(void) const
{
    std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);
    return pimpl->bleAdapter_ != nullptr ? pimpl->bleAdapter_->instance : nullptr;
}

void AdapterManager::OnSysStateChange(const std::string &state) const
{
    LOG_DEBUG("%{public}s state is %{public}s", __PRETTY_FUNCTION__, state.c_str());

    std::string oldSysState;
    std::string newSysState = state;

    {  // lock start,update systerm state
        std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);
        oldSysState = pimpl->sysState_;
        pimpl->sysState_ = state;
    }  // lock end

    // notify systerm state update
    BTSystemState notifySysState = (newSysState == SYS_STATE_STARTED) ? BTSystemState::ON : BTSystemState::OFF;
    if ((newSysState != oldSysState) && ((newSysState == SYS_STATE_STARTED) || (oldSysState == SYS_STATE_STARTED))) {
        HILOGE("oldSysState is %{public}s, newSysState is %{public}s", oldSysState.c_str(), newSysState.c_str());
        pimpl->systemObservers_.ForEach(
            [notifySysState](ISystemStateObserver &observer) { observer.OnSystemStateChange(notifySysState); });
    }
}

std::string AdapterManager::GetSysState() const
{
    std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);
    return pimpl->sysState_;
}

void AdapterManager::OnSysStateExit(const std::string &state) const
{
    LOG_DEBUG("%{public}s state is %{public}s", __PRETTY_FUNCTION__, state.c_str());

    if (state == SYS_STATE_FRESETTING) {
        pimpl->resetPromise_.set_value();
    } else if (state == SYS_STATE_STOPPING) {
        pimpl->stopPromise_.set_value();
    } else {
        // Nothing to do.
    }
}
void AdapterManager::PublishBluetoothStateChangeEvent(const BTTransport transport, const BTStateID state) const
{
    if (transport == ADAPTER_BREDR && (state == BTStateID::STATE_TURN_ON || state == BTStateID::STATE_TURN_OFF)) {
        std::vector<std::string> permissions;
        permissions.emplace_back(PERMISSIONS);
        BluetoothHelper::BluetoothCommonEventHelper::PublishBluetoothStateChangeEvent(state, permissions);
    }
}

void AdapterManager::OnAdapterStateChange(const BTTransport transport, const BTStateID state) const
{
    LOG_DEBUG("%{public}s transport is %{public}d state is %{public}d", __PRETTY_FUNCTION__, transport, state);
    std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);

    if ((transport == ADAPTER_BREDR && pimpl->classicAdapter_ == nullptr) ||
        (transport == ADAPTER_BLE && pimpl->bleAdapter_ == nullptr)) {
        return;
    }
    if ((transport == ADAPTER_BLE) && (state == STATE_TURN_ON)) {
        HILOGI("enable ADAPTER_BREDR");
        Enable(ADAPTER_BREDR);
    }
    if ((transport == ADAPTER_BREDR) && (state == STATE_TURN_OFF)) {
        HILOGI("disable ADAPTER_BLE");
        Disable(ADAPTER_BLE);
    }
    // notify observers state update
    auto &adapterState = transport == ADAPTER_BREDR ? pimpl->classicAdapter_->state : pimpl->bleAdapter_->state;
    if (adapterState != state) {
        adapterState = state;
        if (GetSysState() != SYS_STATE_RESETTING) {
            PublishBluetoothStateChangeEvent(transport, state);
            pimpl->adapterObservers_.ForEach(
                [transport, state](IAdapterStateObserver &observer) { observer.OnStateChange(transport, state); });
        }
    }

    // notify sys state machine
    int classicState = pimpl->classicAdapter_ ? pimpl->classicAdapter_->state : BTStateID::STATE_TURN_OFF;
    int bleState = pimpl->bleAdapter_ ? pimpl->bleAdapter_->state : BTStateID::STATE_TURN_OFF;

    utility::Message msg(SysStateMachine::MSG_SYS_ADAPTER_STATE_CHANGE_REQ);
    msg.arg1_ = ((unsigned int)classicState << CLASSIC_ENABLE_STATE_BIT) + bleState;
    pimpl->dispatcher_->PostTask(std::bind(&utility::StateMachine::ProcessMessage, &(pimpl->sysStateMachine_), msg));
}

void AdapterManager::OnProfileServicesEnableComplete(const BTTransport transport, const bool ret) const
{
    LOG_DEBUG("%{public}s transport is %{public}d, ret is %{public}d", __PRETTY_FUNCTION__, transport, ret);

    utility::Message msg(AdapterStateMachine::MSG_PROFILE_ENABLE_CMP, ret ? true : false);
    pimpl->dispatcher_->PostTask(std::bind(&AdapterManager::impl::ProcessMessage, pimpl.get(), transport, msg));
}

void AdapterManager::OnProfileServicesDisableComplete(const BTTransport transport, const bool ret) const
{
    LOG_DEBUG("%{public}s transport is %{public}d, ret is %{public}d", __PRETTY_FUNCTION__, transport, ret);

    utility::Message msg(AdapterStateMachine::MSG_PROFILE_DISABLE_CMP, ret ? true : false);
    pimpl->dispatcher_->PostTask(std::bind(&AdapterManager::impl::ProcessMessage, pimpl.get(), transport, msg));
}

void AdapterManager::OnPairDevicesRemoved(const BTTransport transport, const std::vector<RawAddress> &devices) const
{
    pimpl->dispatcher_->PostTask(std::bind(&AdapterManager::RemoveDeviceProfileConfig, this, transport, devices));
}

void AdapterManager::RemoveDeviceProfileConfig(
    const BTTransport transport, const std::vector<RawAddress> &devices) const
{
    LOG_DEBUG("%{public}s start", __PRETTY_FUNCTION__);

    std::vector<RawAddress> otherDevices {};
    std::shared_ptr<IAdapter> otherAdapter =
        (transport == BTTransport::ADAPTER_BREDR) ?
        static_cast<std::shared_ptr<IAdapter>>(GetBleAdapterInterface()) :
        static_cast<std::shared_ptr<IAdapter>>(GetClassicAdapterInterface());
    if (otherAdapter) {
        otherDevices = otherAdapter->GetPairedDevices();
    }

    for (auto &device : devices) {
        if (std::find(otherDevices.begin(), otherDevices.end(), device) == otherDevices.end()) {
            ProfileConfig::GetInstance()->RemoveAddr(device.GetAddress());
        }
    }
}

namespace {
template <typename T>
std::unique_ptr<AdapterInfo<T>> CreateAdapter(const std::string &adapterSection, const std::string &adapterName,
    utility::Dispatcher &dispatcher, utility::IContextCallback &callback)
{
    bool value = false;
    std::unique_ptr<AdapterInfo<T>> adapterInfo = nullptr;
    if (AdapterConfig::GetInstance()->GetValue(adapterSection, PROPERTY_IS_VALID, value) && value) {
        std::shared_ptr<T> adapter(ClassCreator<T>::NewInstance(adapterName));
        auto stateMachine = std::make_unique<AdapterStateMachine>(dispatcher);
        if (adapter && stateMachine) {
            adapter->GetContext()->Initialize();
            adapter->GetContext()->RegisterCallback(callback);
            stateMachine->Init(*adapter);
            adapterInfo = std::make_unique<AdapterInfo<T>>(adapter, std::move(stateMachine));
        } else {
            LOG_ERROR("Create %{public}s failed", adapterName.c_str());
        }
    }
    return adapterInfo;
}
} // namespace {}

void AdapterManager::CreateAdapters() const
{
    pimpl->classicAdapter_ = CreateAdapter<ClassicAdapter>(
        SECTION_CLASSIC_ADAPTER, ADAPTER_NAME_CLASSIC, *pimpl->dispatcher_, *(pimpl->contextCallback_));
    pimpl->bleAdapter_ = CreateAdapter<BleAdapter>(
        SECTION_BLE_ADAPTER, ADAPTER_NAME_BLE, *pimpl->dispatcher_, *(pimpl->contextCallback_));
}

int AdapterManager::GetMaxNumConnectedAudioDevices() const
{
    LOG_DEBUG("%{public}s start", __PRETTY_FUNCTION__);
    std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);

    int value = 0;
    IAdapterConfig *config = AdapterConfig::GetInstance();

    if (!config->GetValue(SECTION_A2DP_SRC_SERVICE, PROPERTY_MAX_CONNECTED_DEVICES, value)) {
        HILOGE("%{public}s %{public}s not found", SECTION_A2DP_SRC_SERVICE.c_str(),
            PROPERTY_MAX_CONNECTED_DEVICES.c_str());
    }
    return value;
}

bool AdapterManager::SetPhonebookPermission(const std::string &address, BTPermissionType permission) const
{
    LOG_DEBUG("%{public}s start", __PRETTY_FUNCTION__);
    std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);

    IProfileConfig *config = ProfileConfig::GetInstance();
    bool tmp = false;
    switch (permission) {
        case BTPermissionType::ACCESS_UNKNOWN:
            return config->RemoveProperty(address, SECTION_PERMISSION, PROPERTY_PHONEBOOK_PERMISSION);
        case BTPermissionType::ACCESS_ALLOWED:
            tmp = true;
            break;
        case BTPermissionType::ACCESS_FORBIDDEN:
            break;
        default:
            return false;
    }
    return config->SetValue(address, SECTION_PERMISSION, PROPERTY_PHONEBOOK_PERMISSION, tmp);
}

BTPermissionType AdapterManager::GetPhonebookPermission(const std::string &address) const
{
    LOG_DEBUG("%{public}s start", __PRETTY_FUNCTION__);
    std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);

    IProfileConfig *config = ProfileConfig::GetInstance();
    bool value = false;

    if (!config->GetValue(address, SECTION_PERMISSION, PROPERTY_PHONEBOOK_PERMISSION, value)) {
        HILOGE("%{public}s %{public}s not found", GetEncryptAddr(address).c_str(),
            PROPERTY_PHONEBOOK_PERMISSION.c_str());
        return BTPermissionType::ACCESS_UNKNOWN;
    }

    if (value) {
        return BTPermissionType::ACCESS_ALLOWED;
    } else {
        return BTPermissionType::ACCESS_FORBIDDEN;
    }
}

bool AdapterManager::SetMessagePermission(const std::string &address, BTPermissionType permission) const
{
    LOG_DEBUG("%{public}s start", __PRETTY_FUNCTION__);
    std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);

    IProfileConfig *config = ProfileConfig::GetInstance();
    bool tmp = false;

    switch (permission) {
        case BTPermissionType::ACCESS_UNKNOWN:
            return config->RemoveProperty(address, SECTION_PERMISSION, PROPERTY_MESSAGE_PERMISSION);
        case BTPermissionType::ACCESS_ALLOWED:
            tmp = true;
            break;
        case BTPermissionType::ACCESS_FORBIDDEN:
            break;
        default:
            return false;
    }
    return config->SetValue(address, SECTION_PERMISSION, PROPERTY_MESSAGE_PERMISSION, tmp);
}

BTPermissionType AdapterManager::GetMessagePermission(const std::string &address) const
{
    LOG_DEBUG("%{public}s start", __PRETTY_FUNCTION__);
    std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);

    IProfileConfig *config = ProfileConfig::GetInstance();
    bool value = false;

    if (!config->GetValue(address, SECTION_PERMISSION, PROPERTY_MESSAGE_PERMISSION, value)) {
        HILOGE("%{public}s %{public}s not found", GetEncryptAddr(address).c_str(),
            PROPERTY_MESSAGE_PERMISSION.c_str());
        return BTPermissionType::ACCESS_UNKNOWN;
    }

    if (value) {
        return BTPermissionType::ACCESS_ALLOWED;
    } else {
        return BTPermissionType::ACCESS_FORBIDDEN;
    }
}

int AdapterManager::GetPowerMode(const std::string &address) const
{
    LOG_DEBUG("%{public}s start", __PRETTY_FUNCTION__);
    std::lock_guard<std::recursive_mutex> lock(pimpl->syncMutex_);

    RawAddress addr = RawAddress(address);
    return static_cast<int>(IPowerManager::GetInstance().GetPowerMode(addr));
}

void AdapterManager::RestoreTurnOnState()
{
    std::thread([this] {
        static const std::array<std::pair<std::string, BTTransport>, TRANSPORT_MAX> adapterConfigTbl = {
            std::make_pair(PROPERTY_BLE_TURNON, BTTransport::ADAPTER_BLE),
            std::make_pair(PROPERTY_BREDR_TURNON, BTTransport::ADAPTER_BREDR),
        };

        auto classicAdapter = GetClassicAdapterInterface();
        if (!classicAdapter) {
            LOG_ERROR("classicAdapter is nullptr");
            return;
        }

        while (true) {
            sleep(1);
            
            int processed = 0;
            const unsigned char btStateFlag = 0;
            int turnOn = 0;

            AdapterDeviceConfig::GetInstance()->GetValue(SECTION_HOST, adapterConfigTbl[btStateFlag].first, turnOn);
            LOG_INFO("restore turnon, %{public}s, %{public}d", adapterConfigTbl[btStateFlag].first.c_str(), turnOn);
            for (int i =  0; i < TRANSPORT_MAX; i++) {
                if (!turnOn) {
                    processed++;
                    continue;
                }
                if (GetState(adapterConfigTbl[i].second) != BTStateID::STATE_TURN_ON) {
                    Enable(adapterConfigTbl[i].second);
                    continue;
                }
                
                processed++;
                if (adapterConfigTbl[i].second == BTTransport::ADAPTER_BREDR) {
                    classicAdapter->SetBtScanMode(SCAN_MODE_CONNECTABLE_GENERAL_DISCOVERABLE, 0);
                    classicAdapter->SetBondableMode(BONDABLE_MODE_ON);
                }
            }

            if (processed == TRANSPORT_MAX) {
                LOG_INFO("restore turnon thread END");
                return;
            }
        }
    }).detach();
}
}  // namespace bluetooth
}  // namespace OHOS
