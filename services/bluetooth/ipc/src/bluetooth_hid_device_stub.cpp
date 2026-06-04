/*
 * Copyright (C) 2025 Huawei Device Co., Ltd.
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
#include "bluetooth_hid_device_stub.h"

#include "bluetooth_errorcode.h"
#include "bluetooth_log.h"
#include "permission_manager.h"
#ifdef STUB_FUNC
#undef STUB_FUNC
#endif
#define STUB_FUNC(code, func, perm) BluetoothHidDeviceInterfaceCode::code, {&BluetoothHidDeviceStub::func, perm}


namespace OHOS {
namespace Bluetooth {
using namespace OHOS::bluetooth;
// Note: Permissions need to be configured when the itf to be used. "nullptr" means no permission needed.
const std::map<uint32_t, BluetoothHidDeviceStub::HidDeviceStubFuncPerm> BluetoothHidDeviceStub::memberFuncMap_ = {
    {STUB_FUNC(COMMAND_CONNECT, ConnectInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_DISCONNECT, DisconnectInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_GET_CONNECTION_STATE, GetConnectionStateInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_GET_CONNECTED_DEVICES, GetConnectedDevicesInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_REGISTER_APP, RegisterAppInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_UNREGISTER_APP, UnRegisterAppInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_REGISTER_OBSERVER, RegisterObserverInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_DEREGISTER_OBSERVER, DeregisterObserverInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_SEND_REPORT, SendReportInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_REPLY_REPORT, ReplyReportInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_REPORT_ERROR, ReportErrorInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_SET_CONNECT_STRATEGY, HidDeviceSetConnectStrategyInner,
        CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(COMMAND_GET_CONNECT_STRATEGY, HidDeviceGetConnectStrategyInner,
        CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
};

BluetoothHidDeviceStub::BluetoothHidDeviceStub()
{}

BluetoothHidDeviceStub::~BluetoothHidDeviceStub()
{}

int BluetoothHidDeviceStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    CHECK_PERMISSION_AND_EXECUTE_FUNC_RETURN(BluetoothHidDeviceStub);
}

int32_t BluetoothHidDeviceStub::ConnectInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return BT_ERR_IPC_TRANS_FAILED;
    }
    HILOGD("BluetoothHidDeviceStub::ConnectInner");
    int32_t errCode = Connect(*device);
    // write error code
    if (!reply.WriteInt32(errCode)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHidDeviceStub::ConnectInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothHidDeviceStub: WriteFrameInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHidDeviceStub::DisconnectInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothHidDeviceStub: WriteFrameInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHidDeviceStub::GetConnectionStateInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothHidDeviceStub: WriteFrameInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHidDeviceStub::GetConnectedDevicesInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothHidDeviceStub: WriteFrameInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHidDeviceStub::RegisterAppInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothHidDeviceStub: WriteFrameInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHidDeviceStub::UnRegisterAppInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothHidDeviceStub: WriteFrameInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHidDeviceStub::RegisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothHidDeviceStub: WriteFrameInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHidDeviceStub::DeregisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothHidDeviceStub: WriteFrameInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHidDeviceStub::SendReportInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothHidDeviceStub: WriteFrameInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHidDeviceStub::ReplyReportInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothHidDeviceStub: WriteFrameInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHidDeviceStub::ReportErrorInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothHidDeviceStub: WriteFrameInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHidDeviceStub::HidDeviceSetConnectStrategyInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothHidDeviceStub: WriteFrameInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHidDeviceStub::HidDeviceGetConnectStrategyInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothHidDeviceStub: WriteFrameInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

} // Bluetooth
} // OHOS