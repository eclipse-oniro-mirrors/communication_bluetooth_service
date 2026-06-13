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
#define STUB_FUNC(code, func, perm) \
    static_cast<int>(BluetoothPanInterfaceCode::code), {&BluetoothPanStub::func, perm} \

namespace OHOS {
namespace Bluetooth {
using namespace OHOS::bluetooth;
const uint32_t PAN_DEVICES_STATES_MAX_NUMS = 0XFF;

// Note: Permissions need to be configured when the itf to be used. "nullptr" means no permission needed.
const std::map<uint32_t, BluetoothPanStub::BluetoothPanStubPerm> BluetoothPanStub::memberFuncMap_ = {
    {STUB_FUNC(COMMAND_CONNECT, ConnectInner,
        CHECK_PERM(true, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_DISCONNECT, DisconnectInner,
        CHECK_PERM(true, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_GET_DEVICE_STATE, GetDeviceStateInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_GET_DEVICES_BY_STATES, GetDevicesByStatesInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_REGISTER_OBSERVER, RegisterObserverInner, nullptr)},
    {STUB_FUNC(COMMAND_DEREGISTER_OBSERVER, DeregisterObserverInner, nullptr)},
    {STUB_FUNC(COMMAND_SET_TETHERING, SetTetheringInner,
        CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(COMMAND_IS_TETHERING_ON, IsTetheringOnInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_SET_CONNECT_STRATEGY, SetConnectStrategyInner,
        CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(COMMAND_GET_CONNECT_STRATEGY, GetConnectStrategyInner,
        CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
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

int32_t BluetoothPanStub::ConnectInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothPanStub: ConnectInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothPanStub::DisconnectInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothPanStub: DisconnectInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothPanStub::GetDeviceStateInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothPanStub: GetDeviceStateInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothPanStub::GetDevicesByStatesInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothPanStub: GetDevicesByStatesInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothPanStub::RegisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothPanStub: RegisterObserverInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothPanStub::DeregisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothPanStub: DeregisterObserverInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothPanStub::SetTetheringInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothPanStub: SetTetheringInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothPanStub::IsTetheringOnInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothPanStub: IsTetheringOnInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothPanStub::SetConnectStrategyInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothPanStub: SetConnectStrategyInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothPanStub::GetConnectStrategyInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothPanStub: GetConnectStrategyInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}
}  // namespace Bluetooth
}  // namespace OHOS