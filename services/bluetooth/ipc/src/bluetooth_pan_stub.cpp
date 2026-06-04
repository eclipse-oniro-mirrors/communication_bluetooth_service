/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
#include "bluetooth_errorcode.h"
#include "bluetooth_pan_stub.h"
#include "bluetooth_log.h"
#include "permission_manager.h"

#ifdef STUB_FUNC
#undef STUB_FUNC
#endif
#define STUB_FUNC(code, func, perm) BluetoothPanInterfaceCode::code, {&BluetoothPanStub::func, perm}

namespace OHOS {
namespace Bluetooth {
using namespace OHOS::bluetooth;
const uint32_t PAN_DEVICES_STATES_MAX_NUMS = 0XFF;

// Note: Permissions need to be configured when the itf to be used. "nullptr" means no permission needed.
const std::map<uint32_t, BluetoothPanStub::BluetoothPanStubPerm> BluetoothPanStub::memberFuncMap_ = {

    {STUB_FUNC(COMMAND_DISCONNECT, DisconnectInner,
    CHECK_PERM(true, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_GET_DEVICE_STATE, GetDeviceStateInner,
    CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_GET_DEVICES_BY_STATES, GetDevicesByStatesInner,
    CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_REGISTER_OBSERVER, RegisterObserverInner, nullptr)},
    {STUB_FUNC(COMMAND_DEREGISTER_OBSERVER, DeregisterObserverInner, nullptr)},
    {STUB_FUNC(COMMAND_SET_TETHERING, SetTetheringInner,
    CHECK_PERM(true, {DISCOVER_BLUETOOTH}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(COMMAND_IS_TETHERING_ON, IsTetheringOnInner,
    CHECK_PERM(true, {}, {ACCESS_BLUETOOTH}))},
};
BluetoothPanStub::BluetoothPanStub()
{}

BluetoothPanStub::~BluetoothPanStub()
{}

int BluetoothPanStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    CHECK_PERMISSION_AND_EXECUTE_FUNC_RETURN(BluetoothPanStub);
}

int32_t BluetoothPanStub::DisconnectInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return BT_ERR_IPC_TRANS_FAILED;
    }
    int32_t errCode = Disconnect(*device);
    // write error code
    if (!reply.WriteInt32(errCode)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothPanStub::GetDeviceStateInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return BT_ERR_IPC_TRANS_FAILED;
    }
    int32_t state;
    int32_t errCode = GetDeviceState(*device, state);
    // write error code
    if (!reply.WriteInt32(errCode)) {
        HILOGE("reply write failed.");
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

int32_t BluetoothPanStub::GetDevicesByStatesInner(MessageParcel &data, MessageParcel &reply)
{
    std::vector<int32_t> states = {};
    int32_t stateSize = data.ReadInt32();
    if (static_cast<uint32_t>(stateSize) > PAN_DEVICES_STATES_MAX_NUMS) {
        return ERR_INVALID_STATE;
    }
    for (int i = 0; i < stateSize; i++) {
        int32_t state = data.ReadInt32();
        states.push_back(state);
    }
    std::vector<BluetoothRawAddress> rawAdds;
    int32_t errCode = GetDevicesByStates(states, rawAdds);
    // write error code
    if (!reply.WriteInt32(errCode)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    if (errCode != NO_ERROR) {
        HILOGE("internal error.");
        return BT_ERR_INTERNAL_ERROR;
    }
    // write size
    if (!reply.WriteInt32(rawAdds.size())) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    // write devices
    for (auto rawAdd : rawAdds) {
        if (!reply.WriteParcelable(&rawAdd)) {
            return BT_ERR_IPC_TRANS_FAILED;
        }
    }
    return NO_ERROR;
}

int32_t BluetoothPanStub::RegisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGD("BluetoothPanStub::RegisterObserverInner");
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothPanObserver> observer = OHOS::iface_cast<IBluetoothPanObserver>(remote);
    RegisterObserver(observer);
    return NO_ERROR;
}

int32_t BluetoothPanStub::DeregisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGD("BluetoothPanStub::DeregisterObserverInner");
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothPanObserver> observer = OHOS::iface_cast<IBluetoothPanObserver>(remote);
    DeregisterObserver(observer);
    return NO_ERROR;
}

int32_t BluetoothPanStub::SetTetheringInner(MessageParcel &data, MessageParcel &reply)
{
    const bool value = data.ReadBool();
    int32_t errCode = SetTethering(value);
    // write error code
    if (!reply.WriteInt32(errCode)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothPanStub::IsTetheringOnInner(MessageParcel &data, MessageParcel &reply)
{
    bool result = false;
    int32_t errCode = IsTetheringOn(result);
    // write error code
    if (!reply.WriteInt32(errCode)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    if (errCode != NO_ERROR) {
        HILOGE("internal error.");
        return BT_ERR_INTERNAL_ERROR;
    }
    // write result
    if (!reply.WriteInt32(result)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }

    return NO_ERROR;
}
}  // namespace Bluetooth
}  // namespace OHOS