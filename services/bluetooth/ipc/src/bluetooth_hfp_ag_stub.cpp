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

#include "bluetooth_hfp_ag_stub.h"

#include "bluetooth_errorcode.h"
#include "bluetooth_log.h"
#include "permission_manager.h"

#ifdef STUB_FUNC
#undef STUB_FUNC
#endif
#define STUB_FUNC(code, func, perm) BluetoothHfpAgInterfaceCode::code, {&BluetoothHfpAgStub::func, perm}

namespace OHOS {
namespace Bluetooth {
using namespace OHOS::bluetooth;

// Note: Permissions need to be configured when the itf to be used. "nullptr" means no permission needed.
const std::map<uint32_t, BluetoothHfpAgStub::HfpAgStubFuncPerm> BluetoothHfpAgStub::memberFuncMap_ = {
    {STUB_FUNC(BT_HFP_AG_GET_CONNECT_DEVICES, GetConnectDevicesInner,
        CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_HFP_AG_GET_DEVICES_BY_STATES, GetDevicesByStatesInner,
        CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_HFP_AG_GET_DEVICE_STATE, GetDeviceStateInner,
        CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_HFP_AG_CONNECT, ConnectInner,
        CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_HFP_AG_DISCONNECT, DisconnectInner,
        CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_HFP_AG_GET_SCO_STATE, GetScoStateInner, nullptr)},
    {STUB_FUNC(BT_HFP_AG_CONNECT_SCO_EX, ConnectScoInnerEx,
        CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_HFP_AG_DISCONNECT_SCO_EX, DisconnectScoInnerEx,
        CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_HFP_AG_CONNECT_SCO, ConnectScoInner,
        CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_HFP_AG_DISCONNECT_SCO, DisconnectScoInner,
        CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},

    {STUB_FUNC(BT_HFP_AG_PHONE_STATE_CHANGED, PhoneStateChangedInner,
    CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},

    {STUB_FUNC(BT_HFP_AG_CLCC_RESPONSE, ClccResponseInner,
    CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},

    {STUB_FUNC(BT_HFP_AG_OPEN_VOICE_RECOGNITION, OpenVoiceRecognitionInner,
    CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},

    {STUB_FUNC(BT_HFP_AG_CLOSE_VOICE_RECOGNITION, CloseVoiceRecognitionInner,
    CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},

    {STUB_FUNC(BT_HFP_AG_IS_AUDIO_CONNECTED, IsAudioConnectedInner,
    CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_HFP_AG_SET_ACTIVE_DEVICE, SetActiveDeviceInner,
    CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},

    {STUB_FUNC(BT_HFP_AG_GET_ACTIVE_DEVICE, GetActiveDeviceInner,
    CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},

    {STUB_FUNC(BT_HFP_AG_INTO_MOCK, IntoMockInner,
    CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},

    {STUB_FUNC(BT_HFP_AG_SEND_NO_CARRIER, SendNoCarrierInner,
    CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},

    {STUB_FUNC(BT_HFP_AG_REGISTER_OBSERVER, RegisterObserverInner, nullptr)},
    {STUB_FUNC(BT_HFP_AG_DEREGISTER_OBSERVER, DeregisterObserverInner, nullptr)},
    {STUB_FUNC(BT_HFP_AG_SET_CONNECT_STRATEGY, SetConnectStrategyInner,
    CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_HFP_AG_GET_CONNECT_STRATEGY, GetConnectStrategyInner,
    CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},

    {STUB_FUNC(BT_HFP_AG_IS_IN_BAND_RINGING_ENABLE, IsInbandRingingEnabledInner,
    CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_HFP_AG_CALL_DETAILS_CHANGED, CallDetailsChangedInner,
    CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_HFP_AG_IS_HFP_FEATURE_SUPPORTED, IsHfpFeatureSupportedInner, nullptr)},
    {STUB_FUNC(BT_HFP_AG_CALL_LOG, EnableBtCallLogInner,
    CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_HFP_AG_GET_VIRTUALDEVICE_LIST, GetVirtualDeviceListInner,
    CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_HFP_AG_UPDATE_VIRTUALDEVICE, UpdateVirtualDeviceInner,
    CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_HFP_AG_GET_CALLTYPE, GetCurrentCallTypeInner,
    CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
};

BluetoothHfpAgStub::BluetoothHfpAgStub()
{}

BluetoothHfpAgStub::~BluetoothHfpAgStub()
{}

int BluetoothHfpAgStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    CHECK_PERMISSION_AND_EXECUTE_FUNC_RETURN(BluetoothHfpAgStub);
}

int32_t BluetoothHfpAgStub::GetConnectDevicesInner(MessageParcel &data, MessageParcel &reply)
{
    std::vector<BluetoothRawAddress> devices;
    GetConnectDevices(devices);
    uint32_t DevNum = devices.size();
    if (!reply.WriteUint32(DevNum)) {
        HILOGE("BluetoothHfpAgStub:WriteInt32 failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    for (uint32_t i = 0; i < DevNum; i++) {
        if (!reply.WriteParcelable(&devices[i])) {
            HILOGE("BluetoothHfpAgStub:WriteParcelable failed in: %{public}s.", __func__);
            return BT_ERR_IPC_TRANS_FAILED;
        }
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::GetDevicesByStatesInner(MessageParcel &data, MessageParcel &reply)
{
    std::vector<BluetoothRawAddress> devices;
    std::vector<int> states;
    data.ReadInt32Vector(&states);
    GetDevicesByStates(states, devices);
    uint32_t DevNum = devices.size();
    if (!reply.WriteUint32(DevNum)) {
        HILOGE("BluetoothHfpAgStub:WriteInt32 failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    for (uint32_t i = 0; i < DevNum; i++) {
        if (!reply.WriteParcelable(&devices[i])) {
            HILOGE("BluetoothHfpAgStub:WriteParcelable failed in: %{public}s.", __func__);
            return ERR_INVALID_VALUE;
        }
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::GetDeviceStateInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return BT_ERR_IPC_TRANS_FAILED;
    }
    int32_t state;
    int32_t errCode = GetDeviceState(*device, state);
    if (!reply.WriteInt32(errCode)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    if (errCode != NO_ERROR) {
        HILOGE("internal error.");
        return BT_ERR_INTERNAL_ERROR;
    }
    // write state
    if (!reply.WriteInt32(state)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::ConnectInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return BT_ERR_IPC_TRANS_FAILED;
    }
    int32_t errCode = Connect(*device);
    if (!reply.WriteInt32(errCode)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::DisconnectInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return BT_ERR_IPC_TRANS_FAILED;
    }
    int32_t errCode = Disconnect(*device);
    if (!reply.WriteInt32(errCode)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::GetScoStateInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return TRANSACTION_ERR;
    }
    int result = GetScoState(*device);
    if (!reply.WriteInt32(result)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::ConnectScoInner(MessageParcel &data, MessageParcel &reply)
{
    bool result = ConnectSco();
    if (!reply.WriteBool(result)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::DisconnectScoInner(MessageParcel &data, MessageParcel &reply)
{
    bool result = DisconnectSco();
    if (!reply.WriteBool(result)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::PhoneStateChangedInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothPhoneState> phoneState(data.ReadParcelable<BluetoothPhoneState>());
    CHECK_AND_RETURN_LOG_RET(phoneState, BT_ERR_IPC_TRANS_FAILED,
        "BluetoothHfpAgStub: read phone state failed");
    PhoneStateChanged(*phoneState);
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::ClccResponseInner(MessageParcel &data, MessageParcel &reply)
{
    int index = data.ReadInt32();
    int direction = data.ReadInt32();
    int status = data.ReadInt32();
    int mode = data.ReadInt32();
    bool mpty = data.ReadBool();
    std::string number = data.ReadString();
    int type = data.ReadInt32();
    ClccResponse(index, direction, status, mode, mpty, number, type);
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::OpenVoiceRecognitionInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return TRANSACTION_ERR;
    }
    int result = OpenVoiceRecognition(*device);
    if (!reply.WriteInt32(result)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::CloseVoiceRecognitionInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return TRANSACTION_ERR;
    }
    int result = CloseVoiceRecognition(*device);
    if (!reply.WriteInt32(result)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::IsAudioConnectedInner(MessageParcel &data, MessageParcel &reply)
{
    bool isAudioOn = false;
    int result = IsAudioConnected(isAudioOn);
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(result), BT_ERR_INTERNAL_ERROR, "reply WriteInt32 failed");
    CHECK_AND_RETURN_LOG_RET(reply.WriteBool(isAudioOn), BT_ERR_INTERNAL_ERROR, "reply WriteBool failed");
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::SetActiveDeviceInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return TRANSACTION_ERR;
    }
    int result = SetActiveDevice(*device);
    if (!reply.WriteInt32(result)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::IntoMockInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return TRANSACTION_ERR;
    }
    int state = data.ReadInt32();
    int result = IntoMock(*device, state);
    if (!reply.WriteInt32(result)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::SendNoCarrierInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return TRANSACTION_ERR;
    }
    int result = SendNoCarrier(*device);
    if (!reply.WriteInt32(result)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::GetActiveDeviceInner(MessageParcel &data, MessageParcel &reply)
{
    std::string result = GetActiveDevice();
    if (!reply.WriteString(result)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::RegisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    sptr<IRemoteObject> tempObject = data.ReadRemoteObject();
    sptr<IBluetoothHfpAgObserver> observer = iface_cast<IBluetoothHfpAgObserver>(tempObject);
    RegisterObserver(observer);
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::DeregisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    sptr<IRemoteObject> tempObject = data.ReadRemoteObject();
    sptr<IBluetoothHfpAgObserver> observer = iface_cast<IBluetoothHfpAgObserver>(tempObject);
    DeregisterObserver(observer);
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::SetConnectStrategyInner(MessageParcel &data, MessageParcel &reply)
{
    int result = BT_ERR_SYSTEM_PERMISSION_FAILED;
    if (!reply.WriteInt32(result)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::GetConnectStrategyInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::IsInbandRingingEnabledInner(MessageParcel &data, MessageParcel &reply)
{
    bool isEnabled = true;
    int result = IsInbandRingingEnabled(isEnabled);
    if (!reply.WriteInt32(result)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    if (!reply.WriteBool(isEnabled)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::ConnectScoInnerEx(MessageParcel &data, MessageParcel &reply)
{
    bool result = ConnectSco();
    if (!reply.WriteBool(result)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::DisconnectScoInnerEx(MessageParcel &data, MessageParcel &reply)
{
    bool result = DisconnectSco();
    if (!reply.WriteBool(result)) {
        HILOGE("BluetoothHfpAgStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::CallDetailsChangedInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::EnableBtCallLogInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::IsHfpFeatureSupportedInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    CHECK_AND_RETURN_LOG_RET(device, BT_ERR_IPC_TRANS_FAILED, "Read device address failed.");
    int type = data.ReadInt32();
    bool isSupported = false;
    int32_t result = IsHfpFeatureSupported(*device, isSupported, type);
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(result), BT_ERR_INTERNAL_ERROR, "reply WriteInt32 failed");
    CHECK_AND_RETURN_LOG_RET(reply.WriteBool(isSupported), BT_ERR_INTERNAL_ERROR, "reply WriteBool failed");
    return NO_ERROR;
}

int32_t BluetoothHfpAgStub::UpdateVirtualDeviceInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHfpAgStub::GetVirtualDeviceListInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHfpAgStub::GetCurrentCallTypeInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}
}  // namespace Bluetooth
}  // namespace OHOS
