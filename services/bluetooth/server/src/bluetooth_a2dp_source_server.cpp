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
#include <cinttypes>
#include <mutex>
#include "bluetooth_def.h"
#include "bluetooth_errorcode.h"
#include "bluetooth_hitrace.h"
#include "bluetooth_log.h"
#include "bluetooth_utils_server.h"
#include "hisysevent.h"
#include "interface_profile_manager.h"
#include "interface_profile_a2dp_src.h"
#include "remote_observer_list.h"
#include "interface_adapter_manager.h"
#include "bluetooth_a2dp_source_server.h"
#include "permission_manager.h"

namespace OHOS {
namespace Bluetooth {
class A2dpSourceObserver : public IA2dpObserver {
public:
    A2dpSourceObserver() = default;
    ~A2dpSourceObserver() override = default;

    void OnConnectionStateChanged(const RawAddress &device, int state) override
    {
        HILOGI("addr: %{public}s, state: %{public}d", GET_ENCRYPT_ADDR(device), state);
        if (state == static_cast<int>(BTConnectState::CONNECTED) ||
            state == static_cast<int>(BTConnectState::DISCONNECTED)) {
            HiSysEventWrite(OHOS::HiviewDFX::HiSysEvent::Domain::BT_SERVICE, "A2DP_CONNECTED_STATE",
                OHOS::HiviewDFX::HiSysEvent::EventType::STATISTIC, "STATE", state);
        }
        observers_->ForEach([device, state](sptr<IBluetoothA2dpSourceObserver> observer) {
            observer->OnConnectionStateChanged(device, state,
                static_cast<uint32_t>(ConnChangeCause::CONNECT_CHANGE_COMMON_CAUSE));
        });
    }

    void OnCaptureConnectionStateChanged(const RawAddress &device, int state, const A2dpSrcCodecInfo &info) override
    {
        HILOGI("addr: %{public}s, state: %{public}d", GET_ENCRYPT_ADDR(device), state);
        observers_->ForEach([device, state, info](sptr<IBluetoothA2dpSourceObserver> observer) {
            BluetoothA2dpCodecInfo tmpInfo {};
            tmpInfo.bitsPerSample = info.bitsPerSample;
            tmpInfo.channelMode = info.channelMode;
            tmpInfo.codecType = info.codecType;
            tmpInfo.sampleRate = info.sampleRate;
            observer->OnCaptureConnectionStateChanged(device, state, tmpInfo);
        });
    }

    void OnPlayingStatusChaned(const RawAddress &device, int playingState, int error) override
    {
        HILOGI("addr: %{public}s, state: %{public}d, error: %{public}d",
            GET_ENCRYPT_ADDR(device), playingState, error);
        observers_->ForEach([device, playingState, error](sptr<IBluetoothA2dpSourceObserver> observer) {
            observer->OnPlayingStatusChanged(device, playingState, error);
        });
    }

    void OnConfigurationChanged(const RawAddress &device, const A2dpSrcCodecInfo &info, int error) override
    {
        HILOGI("addr: %{public}s, error: %{public}d", GET_ENCRYPT_ADDR(device), error);
        observers_->ForEach([device, info, error](sptr<IBluetoothA2dpSourceObserver> observer) {
            BluetoothA2dpCodecInfo  tmpInfo {};
            tmpInfo.bitsPerSample = info.bitsPerSample;
            tmpInfo.channelMode = info.channelMode;
            tmpInfo.codecPriority = info.codecPriority;
            tmpInfo.codecType = info.codecType;
            tmpInfo.sampleRate = info.sampleRate;
            tmpInfo.codecSpecific1 = info.codecSpecific1;
            tmpInfo.codecSpecific2 = info.codecSpecific2;
            tmpInfo.codecSpecific3 = info.codecSpecific3;
            tmpInfo.codecSpecific4 = info.codecSpecific4;

            observer->OnConfigurationChanged(device, tmpInfo, error);
        });
    }

    void OnMediaStackChanged(const RawAddress &device, int action) override
    {
        HILOGI("addr: %{public}s, action: %{public}d", GET_ENCRYPT_ADDR(device), action);
    }

    void SetObserver(RemoteObserverList<IBluetoothA2dpSourceObserver> *observers)
    {
        observers_ = observers;
    }

private:
    RemoteObserverList<IBluetoothA2dpSourceObserver> *observers_;
};

struct BluetoothA2dpSourceServer::impl {
    impl();
    ~impl();

    /// sys state observer
    class SystemStateObserver;
    std::unique_ptr<SystemStateObserver> systemStateObserver_ = nullptr;

    RemoteObserverList<IBluetoothA2dpSourceObserver> observers_;
    std::unique_ptr<A2dpSourceObserver> observerImp_{std::make_unique<A2dpSourceObserver>()};
    // Guards a2dpSrcService_ against the BT on/off switch: it is set when BT turns ON, and cleared by
    // the state observer when BT turns OFF, ahead of ProfileServiceManager::Stop() deleting the profile
    // objects. Every entry takes this lock and keeps it for the whole service call, so an OFF
    // transition cannot clear the pointer while a call is in flight, and since the observer runs on
    // the state-machine thread before AdapterStop(), the object is deleted only after that call
    // returns. Keeping the lock only around the pointer read would leave a stale copy that the OFF
    // path can invalidate. Lock order is one-way: this mutex before the profile-service / adapter
    // locks, never after. Recursive because OnSystemStateChange() and the destructor call
    // RegisterObserver / DeregisterObserver on the same thread while holding it, and that call can
    // re-enter this class. Cost: entries are serialized while a call runs (e.g. SetConnectStrategy
    // writes the xml config).
    std::recursive_mutex a2dpSrcServiceMutex_ {};
    IProfileA2dpSrc *a2dpSrcService_ = nullptr;
};

class BluetoothA2dpSourceServer::impl::SystemStateObserver : public ISystemStateObserver {
public:
    SystemStateObserver(BluetoothA2dpSourceServer::impl *pimpl) : pimpl_(pimpl) {};
    ~SystemStateObserver() override = default;

    void OnSystemStateChange(const BTSystemState state) override
    {
        IProfileManager *serviceMgr = IProfileManager::GetInstance();
        if (!pimpl_) {
            HILOGI("failed: pimpl_ is null");
            return;
        }

        switch (state) {
            case BTSystemState::ON: {
                IProfileA2dpSrc *service = nullptr;
                if (serviceMgr != nullptr) {
                    service = (IProfileA2dpSrc *)serviceMgr->GetProfileService(PROFILE_NAME_A2DP_SRC);
                }
                {
                    std::lock_guard<std::recursive_mutex> lock(pimpl_->a2dpSrcServiceMutex_);
                    pimpl_->a2dpSrcService_ = service;
                    if (service != nullptr) {
                        service->RegisterObserver(pimpl_->observerImp_.get());
                    }
                }
                break;
            }
            case BTSystemState::OFF: {
                std::lock_guard<std::recursive_mutex> lock(pimpl_->a2dpSrcServiceMutex_);
                if (pimpl_->a2dpSrcService_ != nullptr) {
                    pimpl_->a2dpSrcService_->DeregisterObserver(pimpl_->observerImp_.get());
                }
                pimpl_->a2dpSrcService_ = nullptr;
                break;
            }
            default:
                break;
        }
    }

private:
    BluetoothA2dpSourceServer::impl *pimpl_ = nullptr;
};

BluetoothA2dpSourceServer::impl::impl()
{
    HILOGI("starts");
}

BluetoothA2dpSourceServer::impl::~impl()
{
    HILOGI("starts");
}

BluetoothA2dpSourceServer::BluetoothA2dpSourceServer()
{
    HILOGI("starts");
    pimpl = std::make_unique<impl>();
    pimpl->observerImp_->SetObserver(&(pimpl->observers_));
    pimpl->systemStateObserver_ = std::make_unique<impl::SystemStateObserver>(pimpl.get());
    IAdapterManager::GetInstance()->RegisterSystemStateObserver(*(pimpl->systemStateObserver_));

    IProfileManager *serviceMgr = IProfileManager::GetInstance();
    if (serviceMgr != nullptr) {
        IProfileA2dpSrc *service = (IProfileA2dpSrc *)serviceMgr->GetProfileService(PROFILE_NAME_A2DP_SRC);
        {
            std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
            pimpl->a2dpSrcService_ = service;
            if (service != nullptr) {
                service->RegisterObserver(pimpl->observerImp_.get());
            }
        }
    }
}

BluetoothA2dpSourceServer::~BluetoothA2dpSourceServer()
{
    HILOGI("starts");
    IAdapterManager::GetInstance()->DeregisterSystemStateObserver(*(pimpl->systemStateObserver_));
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    if (pimpl->a2dpSrcService_ != nullptr) {
        pimpl->a2dpSrcService_->DeregisterObserver(pimpl->observerImp_.get());
        pimpl->a2dpSrcService_ = nullptr;
    }
}

void BluetoothA2dpSourceServer::RegisterObserver(const sptr<IBluetoothA2dpSourceObserver> &observer)
{
    HILOGI("starts");
    if (observer == nullptr) {
        HILOGI("observer is null");
        return;
    }
    auto func = std::bind(&BluetoothA2dpSourceServer::DeregisterObserver, this, std::placeholders::_1);
    pimpl->observers_.Register(observer, func);
    // During A2DP HDF Registration, check the current status and callback
    RawAddress device = GetActiveSinkDevice();
    int state = static_cast<int>(BTConnectState::DISCONNECTED);
    GetDeviceState(static_cast<const RawAddress &>(device), state);
    if (state == static_cast<int>(BTConnectState::CONNECTED)) {
        HILOGI("onConnectionStateChanged");
        observer->OnConnectionStateChanged(device, state,
            static_cast<uint32_t>(ConnChangeCause::CONNECT_CHANGE_COMMON_CAUSE));
    }
}

void BluetoothA2dpSourceServer::DeregisterObserver(const sptr<IBluetoothA2dpSourceObserver> &observer)
{
    HILOGI("starts");
    if (observer == nullptr) {
        HILOGE("observer is null");
        return;
    }

    if (pimpl != nullptr) {
        pimpl->observers_.Deregister(observer);
    }
}

int32_t BluetoothA2dpSourceServer::Connect(const RawAddress &device)
{
    HILOGI("addr: %{public}s", GET_ENCRYPT_ADDR(device));
    if (PermissionManager::GetApiVersion() >= API_VERSION_10 && !PermissionManager::IsSystemHap()) {
        HILOGE("check permission failed, system hap need.");
        return BT_ERR_PERMISSION_FAILED;
    }
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return BT_ERR_INVALID_STATE;
    }
    OHOS::Bluetooth::BluetoothHiTrace::BluetoothStartAsyncTrace("A2DP_SRC_CONNECT", 1);
    int32_t result = service->Connect(device);
    OHOS::Bluetooth::BluetoothHiTrace::BluetoothFinishAsyncTrace("A2DP_SRC_CONNECT", 1);
    return result;
}

int32_t BluetoothA2dpSourceServer::Disconnect(const RawAddress &device)
{
    HILOGI("addr: %{public}s", GET_ENCRYPT_ADDR(device));
    if (PermissionManager::GetApiVersion() >= API_VERSION_10 && !PermissionManager::IsSystemHap()) {
        HILOGE("check permission failed, system hap need.");
        return BT_ERR_PERMISSION_FAILED;
    }
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return BT_ERR_INVALID_STATE;
    }
    return service->Disconnect(device);
}

int BluetoothA2dpSourceServer::GetDeviceState(const RawAddress &device, int &state)
{
    HILOGI("addr: %{public}s", GET_ENCRYPT_ADDR(device));
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return BT_ERR_INVALID_STATE;
    }
    state = service->GetDeviceState(device);
    return NO_ERROR;
}

int BluetoothA2dpSourceServer::GetDevicesByStates(const std::vector<int32_t> &states, std::vector<RawAddress> &rawAddrs)
{
    HILOGI("starts");
    std::vector<int> tmpStates;
    for (int32_t state : states) {
        HILOGI("state = %{public}d", state);
        tmpStates.push_back((int)state);
    }

    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return BT_ERR_INVALID_STATE;
    }
    rawAddrs = service->GetDevicesByStates(tmpStates);
    return NO_ERROR;
}

int32_t BluetoothA2dpSourceServer::GetPlayingState(const RawAddress &device, int &state)
{
    HILOGI("addr: %{public}s", GET_ENCRYPT_ADDR(device));
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return BT_ERR_INVALID_STATE;
    }
    int ret = service->GetPlayingState(device, state);
    if (ret != NO_ERROR) {
        return BT_ERR_INTERNAL_ERROR;
    }
    return NO_ERROR;
}

int BluetoothA2dpSourceServer::SetConnectStrategy(const RawAddress &device, int strategy)
{
    HILOGI("addr: %{public}s, strategy: %{public}d", GET_ENCRYPT_ADDR(device), strategy);
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return BT_ERR_INVALID_STATE;
    }
    return service->SetConnectStrategy(device, strategy);
}

int BluetoothA2dpSourceServer::GetConnectStrategy(const RawAddress &device, int &strategy)
{
    HILOGI("addr: %{public}s", GET_ENCRYPT_ADDR(device));
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return BT_ERR_INVALID_STATE;
    }
    strategy = service->GetConnectStrategy(device);
    return NO_ERROR;
}

int BluetoothA2dpSourceServer::SetActiveSinkDevice(const RawAddress &device)
{
    HILOGI("addr: %{public}s", GET_ENCRYPT_ADDR(device));
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return BT_ERR_INVALID_STATE;
    }
    return service->SetActiveSinkDevice(device);
}

RawAddress BluetoothA2dpSourceServer::GetActiveSinkDevice()
{
    HILOGI("starts");
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return RawAddress();
    }
    return service->GetActiveSinkDevice();
}

BluetoothA2dpCodecStatus BluetoothA2dpSourceServer::GetCodecStatus(const RawAddress &device)
{
    HILOGI("addr: %{public}s", GET_ENCRYPT_ADDR(device));
    bluetooth::RawAddress addr(device.GetAddress());
    bluetooth::A2dpSrcCodecStatus ret;
    BluetoothA2dpCodecStatus codeStatus;
    BluetoothA2dpCodecInfo serviceInfo;
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return BluetoothA2dpCodecStatus();
    }
    ret = service->GetCodecStatus(addr);

    codeStatus.codecInfo.codecPriority = ret.codecInfo.codecPriority;
    codeStatus.codecInfo.codecType = ret.codecInfo.codecType;
    codeStatus.codecInfo.sampleRate = ret.codecInfo.sampleRate;
    codeStatus.codecInfo.bitsPerSample = ret.codecInfo.bitsPerSample;
    codeStatus.codecInfo.channelMode = ret.codecInfo.channelMode;
    codeStatus.codecInfo.codecSpecific1 = ret.codecInfo.codecSpecific1;
    codeStatus.codecInfo.codecSpecific2 = ret.codecInfo.codecSpecific2;
    codeStatus.codecInfo.codecSpecific3 = ret.codecInfo.codecSpecific3;
    codeStatus.codecInfo.codecSpecific4 = ret.codecInfo.codecSpecific4;

    for (auto it = ret.codecInfoConfirmedCap.begin(); it != ret.codecInfoConfirmedCap.end(); it++) {
        serviceInfo.codecPriority = it->codecPriority;
        serviceInfo.codecType = it->codecType;
        serviceInfo.sampleRate = it->sampleRate;
        serviceInfo.bitsPerSample = it->bitsPerSample;
        serviceInfo.channelMode = it->channelMode;
        serviceInfo.codecSpecific1 = it->codecSpecific1;
        serviceInfo.codecSpecific2 = it->codecSpecific2;
        serviceInfo.codecSpecific3 = it->codecSpecific3;
        serviceInfo.codecSpecific4 = it->codecSpecific4;
        codeStatus.codecInfoConfirmCap.push_back(serviceInfo);
    }

    for (auto it = ret.codecInfoLocalCap.begin(); it != ret.codecInfoLocalCap.end(); it++) {
        serviceInfo.codecPriority = it->codecPriority;
        serviceInfo.codecType = it->codecType;
        serviceInfo.sampleRate = it->sampleRate;
        serviceInfo.bitsPerSample = it->bitsPerSample;
        serviceInfo.channelMode = it->channelMode;
        serviceInfo.codecSpecific1 = it->codecSpecific1;
        serviceInfo.codecSpecific2 = it->codecSpecific2;
        serviceInfo.codecSpecific3 = it->codecSpecific3;
        serviceInfo.codecSpecific4 = it->codecSpecific4;
        codeStatus.codecInfoLocalCap.push_back(serviceInfo);
    }

    return codeStatus;
}

int BluetoothA2dpSourceServer::GetCodecPreference(const RawAddress &device, BluetoothA2dpCodecInfo &info)
{
    return  NO_ERROR;
}

int BluetoothA2dpSourceServer::SetCodecPreference(const RawAddress &device, const BluetoothA2dpCodecInfo &info)
{
    HILOGI("BluetoothA2dpSourceServer::SetCodecPreference starts, codecPriority = %{public}u,"
           "codecPriority = %{public}u, sampleRate = %{public}u, bitsPerSample = %{public}d, "
           "channelMode = %{public}d, codecSpecific1 = %{public}llu, codecSpecific2 = %{public}llu, "
           "codecSpecific3 = %{public}llu, codecSpecific4 = %{public}llu",
           info.codecPriority, info.codecType, info.sampleRate, info.bitsPerSample, info.channelMode,
           (unsigned long long)info.codecSpecific1, (unsigned long long)info.codecSpecific2,
           (unsigned long long)info.codecSpecific3, (unsigned long long)info.codecSpecific4);
    bluetooth::A2dpSrcCodecInfo setInfo;

    setInfo.bitsPerSample = info.bitsPerSample;
    setInfo.channelMode = info.channelMode;
    setInfo.codecPriority = info.codecPriority;
    setInfo.codecType = info.codecType;
    setInfo.sampleRate = info.sampleRate;
    setInfo.codecSpecific1 = info.codecSpecific1;
    setInfo.codecSpecific2 = info.codecSpecific2;
    setInfo.codecSpecific3 = info.codecSpecific3;
    setInfo.codecSpecific4 = info.codecSpecific4;

    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return BT_ERR_INVALID_STATE;
    }
    return service->SetCodecPreference(device, setInfo);
}

void BluetoothA2dpSourceServer::SwitchOptionalCodecs(const RawAddress &device, bool isEnable)
{
    HILOGI("addr: %{public}s, isEnable = %{public}d", GET_ENCRYPT_ADDR(device), isEnable);
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return;
    }
    service->SwitchOptionalCodecs(device, isEnable);
}

int BluetoothA2dpSourceServer::GetOptionalCodecsSupportState(const RawAddress &device)
{
    HILOGI("addr: %{public}s", GET_ENCRYPT_ADDR(device));
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return static_cast<int>(A2DP_OPTIONAL_SUPPORT_UNKNOWN);
    }
    return service->GetOptionalCodecsSupportState(device);
}

int BluetoothA2dpSourceServer::StartPlaying(const RawAddress &device)
{
    HILOGI("addr: %{public}s", GET_ENCRYPT_ADDR(device));
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return BT_ERR_INVALID_STATE;
    }
    return service->StartPlaying(device);
}

int BluetoothA2dpSourceServer::SuspendPlaying(const RawAddress &device)
{
    HILOGI("addr: %{public}s", GET_ENCRYPT_ADDR(device));
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return BT_ERR_INVALID_STATE;
    }
    return service->SuspendPlaying(device);
}

int BluetoothA2dpSourceServer::StopPlaying(const RawAddress &device)
{
    HILOGI("addr: %{public}s", GET_ENCRYPT_ADDR(device));
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return BT_ERR_INVALID_STATE;
    }
    return service->StopPlaying(device);
}

int BluetoothA2dpSourceServer::WriteFrame(const uint8_t *data, uint32_t size)
{
    HILOGI("size = %{public}u", size);
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        return BT_ERR_INVALID_STATE;
    }
    return service->WriteFrame(data, size);
}

int BluetoothA2dpSourceServer::GetRenderPosition(const RawAddress &device, uint32_t &delayValue, uint64_t &sendDataSize,
                                                 uint32_t &timeStamp)
{
    HILOGI("starts");
    std::lock_guard<std::recursive_mutex> lock(pimpl->a2dpSrcServiceMutex_);
    IProfileA2dpSrc *service = pimpl->a2dpSrcService_;
    if (service == nullptr) {
        HILOGE("%{public}s: a2dpSrcService_ is nullptr", __func__);
        delayValue = 0;
        sendDataSize = 0;
        timeStamp = 0;
        return BT_ERR_INVALID_STATE;
    }
    int ret = service->GetRenderPosition(device, delayValue, sendDataSize, timeStamp);
    HILOGI("delayValue = %{public}u, sendDataSize = %{public}" PRIu64 ", timeStamp = %{public}u", delayValue,
        sendDataSize, timeStamp);
    return ret;
}

int BluetoothA2dpSourceServer::OffloadStartPlaying(const RawAddress &device, const std::vector<int32_t> &sessionsId)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int BluetoothA2dpSourceServer::OffloadStopPlaying(const RawAddress &device, const std::vector<int32_t> &sessionsId)
{
    return BT_ERR_API_NOT_SUPPORT;
}
int BluetoothA2dpSourceServer::A2dpOffloadSessionPathRequest(const RawAddress &device,
    const std::vector<BluetoothA2dpStreamInfo> &info)
{
    return BT_ERR_API_NOT_SUPPORT;
}

BluetoothA2dpOffloadCodecStatus BluetoothA2dpSourceServer::GetOffloadCodecStatus(const RawAddress &device)
{
    BluetoothA2dpOffloadCodecStatus ret;
    HILOGI("enter");
    return ret;
}

int BluetoothA2dpSourceServer::EnableAutoPlay(const RawAddress &device)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int BluetoothA2dpSourceServer::DisableAutoPlay(const RawAddress &device, const int duration)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int BluetoothA2dpSourceServer::GetAutoPlayDisabledDuration(const RawAddress &device, int &duration)
{
    return BT_ERR_API_NOT_SUPPORT;
}

void BluetoothA2dpSourceServer::GetVirtualDeviceList(std::vector<std::string> &devices)
{
    return;
}

void BluetoothA2dpSourceServer::UpdateVirtualDevice(int32_t action, const std::string &address)
{
    return;
}
}  // namespace Bluetooth
}  // namespace OHOS
