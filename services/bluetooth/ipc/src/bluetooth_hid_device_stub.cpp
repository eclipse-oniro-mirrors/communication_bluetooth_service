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

namespace OHOS {
namespace Bluetooth {
BluetoothHidDeviceStub::BluetoothHidDeviceStub()
{
    HILOGD("%{public}s start.", __func__);
    memberFuncMap_[static_cast<uint32_t>(BluetoothHidDeviceInterfaceCode::COMMAND_REGISTER_APP)] =
        &BluetoothHidDeviceStub::RegisterAppInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothHidDeviceInterfaceCode::COMMAND_UNREGISTER_APP)] =
        &BluetoothHidDeviceStub::UnRegisterAppInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothHidDeviceInterfaceCode::COMMAND_CONNECT)] =
        &BluetoothHidDeviceStub::ConnectInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothHidDeviceInterfaceCode::COMMAND_DISCONNECT)] =
        &BluetoothHidDeviceStub::DisconnectInner;

    memberFuncMap_[static_cast<uint32_t>(BluetoothHidDeviceInterfaceCode::COMMAND_GET_CONNECTION_STATE)] =
        &BluetoothHidDeviceStub::GetConnectionStateInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothHidDeviceInterfaceCode::COMMAND_GET_CONNECTED_DEVICES)] =
        &BluetoothHidDeviceStub::GetConnectedDevicesInner;

    memberFuncMap_[static_cast<uint32_t>(BluetoothHidDeviceInterfaceCode::COMMAND_REGISTER_OBSERVER)] =
        &BluetoothHidDeviceStub::RegisterObserverInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothHidDeviceInterfaceCode::COMMAND_DEREGISTER_OBSERVER)] =
        &BluetoothHidDeviceStub::DeregisterObserverInner;

    memberFuncMap_[static_cast<uint32_t>(BluetoothHidDeviceInterfaceCode::COMMAND_SEND_REPORT)] =
        &BluetoothHidDeviceStub::SendReportInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothHidDeviceInterfaceCode::COMMAND_REPLY_REPORT)] =
        &BluetoothHidDeviceStub::ReplyReportInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothHidDeviceInterfaceCode::COMMAND_REPORT_ERROR)] =
        &BluetoothHidDeviceStub::ReportErrorInner;

    memberFuncMap_[static_cast<uint32_t>(BluetoothHidDeviceInterfaceCode::COMMAND_SET_CONNECT_STRATEGY)] =
        &BluetoothHidDeviceStub::HidDeviceSetConnectStrategyInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothHidDeviceInterfaceCode::COMMAND_GET_CONNECT_STRATEGY)] =
        &BluetoothHidDeviceStub::HidDeviceGetConnectStrategyInner;
    HILOGD("%{public}s ends.", __func__);
}

BluetoothHidDeviceStub::~BluetoothHidDeviceStub()
{
    HILOGD("%{public}s start.", __func__);
    memberFuncMap_.clear();
}

int BluetoothHidDeviceStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    HILOGI("BluetoothHidDeviceStub::OnRemoteRequest, cmd = %{public}d, flags= %{public}d", code, option.GetFlags());
    if (BluetoothHidDeviceStub::GetDescriptor() != data.ReadInterfaceToken()) {
        HILOGE("local descriptor is not equal to remote");
        return IPC_INVOKER_TRANSLATE_ERR;
    }
    auto itFunc = memberFuncMap_.find(code);
    if (itFunc != memberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            return (this->*memberFunc)(data, reply);
        }
    }
    HILOGW("BluetoothHidDeviceStub::OnRemoteRequest, default case, need check.");
    return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
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