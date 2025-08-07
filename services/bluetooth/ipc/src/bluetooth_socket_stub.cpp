/*
 * Copyright (C) 2021-2023 Huawei Device Co., Ltd.
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

#include "bluetooth_bt_uuid.h"
#include "bluetooth_errorcode.h"
#include "bluetooth_log.h"
#include "bluetooth_socket_observer_proxy.h"
#include "ipc_types.h"
#include "bluetooth_socket_stub.h"

namespace OHOS {
namespace Bluetooth {
BluetoothSocketStub::BluetoothSocketStub()
{
    HILOGD("%{public}s start.", __func__);
    memberFuncMap_[static_cast<uint32_t>(BluetoothSocketInterfaceCode::SOCKET_CONNECT)] =
        &BluetoothSocketStub::ConnectInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothSocketInterfaceCode::SOCKET_LISTEN)] =
        &BluetoothSocketStub::ListenInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothSocketInterfaceCode::DEREGISTER_SERVER_OBSERVER)] =
        &BluetoothSocketStub::DeregisterServerObserverInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothSocketInterfaceCode::REGISTER_CLIENT_OBSERVER)] =
        &BluetoothSocketStub::RegisterClientObserverInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothSocketInterfaceCode::DEREGISTER_SERVER_OBSERVER)] =
        &BluetoothSocketStub::DeregisterClientObserverInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothSocketInterfaceCode::SOCKET_UPDATE_COC_PARAMS)] =
        &BluetoothSocketStub::UpdateCocConnectionParamsInner;
    memberFuncMap_[static_cast<uint32_t>(BluetoothSocketInterfaceCode::SOCKET_IS_ALLOW_CONNECT)] =
        &BluetoothSocketStub::IsAllowSocketConnectInner;
}

BluetoothSocketStub::~BluetoothSocketStub()
{
    HILOGD("%{public}s start.", __func__);
    memberFuncMap_.clear();
}

int32_t BluetoothSocketStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    HILOGD("BluetoothSocketStub::OnRemoteRequest, cmd = %{public}d, flags= %{public}d", code, option.GetFlags());

    std::u16string descriptor = BluetoothSocketStub::GetDescriptor();
    std::u16string remoteDescriptor = data.ReadInterfaceToken();
    if (descriptor != remoteDescriptor) {
        HILOGE("BluetoothSocketStub::OnRemoteRequest, local descriptor is not equal to remote");
        return ERR_INVALID_STATE;
    }

    auto itFunc = memberFuncMap_.find(code);
    if (itFunc != memberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            return (this->*memberFunc)(data, reply);
        }
    }

    HILOGW("BluetoothHostObserverStub::OnRemoteRequest, default case, need check.");
    return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
}

ErrCode BluetoothSocketStub::ConnectInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("ConnectInner starts");
    std::string addr = data.ReadString();
    std::shared_ptr<BluetoothUuid> uuid(data.ReadParcelable<BluetoothUuid>());
    if (uuid == nullptr) {
        HILOGE("reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    ConnectSocketParam param {
        .addr = addr,
        .uuid = *uuid,
        .securityFlag = data.ReadInt32(),
        .type = data.ReadInt32(),
        .psm = data.ReadInt32()
    };
    int fd = -1;
    int ret = Connect(param, fd);
    if (!reply.WriteInt32(ret)) {
        HILOGE("reply writing failed.");
        return ERR_INVALID_VALUE;
    }

    if (ret == NO_ERROR) {
        if (!reply.WriteFileDescriptor(fd)) {
            HILOGE("reply writing failed");
            return ERR_INVALID_VALUE;
        }
    }
    return NO_ERROR;
}

ErrCode BluetoothSocketStub::ListenInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("ListenInner starts");
    std::string name = data.ReadString();
    std::shared_ptr<BluetoothUuid> uuid(data.ReadParcelable<BluetoothUuid>());
    if (uuid == nullptr) {
        HILOGE("reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    ListenSocketParam param {
        .name = name,
        .uuid = *uuid,
        .securityFlag = data.ReadInt32(),
        .type = data.ReadInt32(),
        .psm = data.ReadInt32(),
        .observer = OHOS::iface_cast<IBluetoothServerSocketObserver>(data.ReadRemoteObject())
    };

    int fd = -1;
    int ret = Listen(param, fd);
    if (!reply.WriteInt32(ret)) {
        HILOGE("reply writing failed.");
        return ERR_INVALID_VALUE;
    }
    if (ret == NO_ERROR) {
        if (!reply.WriteFileDescriptor(fd)) {
            HILOGE("reply writing failed");
            return ERR_INVALID_VALUE;
        }
    }
    return NO_ERROR;
}

ErrCode BluetoothSocketStub::DeregisterServerObserverInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("DeregisterServerObserverInner starts");
    sptr<IBluetoothServerSocketObserver> observer =
        OHOS::iface_cast<IBluetoothServerSocketObserver>(data.ReadRemoteObject());
    DeregisterServerObserver(observer);

    return NO_ERROR;
}

ErrCode BluetoothSocketStub::RegisterClientObserverInner(MessageParcel &data, MessageParcel &reply)
{
    return reply.WriteInt32(BT_NO_ERROR);
}

ErrCode BluetoothSocketStub::DeregisterClientObserverInner(MessageParcel &data, MessageParcel &reply)
{
    return reply.WriteInt32(BT_NO_ERROR);
}

ErrCode BluetoothSocketStub::UpdateCocConnectionParamsInner(MessageParcel &data, MessageParcel &reply)
{
    return reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
}

ErrCode BluetoothSocketStub::IsAllowSocketConnectInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_NO_ERROR);
    return reply.WriteBool(true);
}
}  // namespace Bluetooth
}  // namespace OHOS