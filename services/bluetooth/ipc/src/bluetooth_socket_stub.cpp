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
#include "permission_manager.h"

#ifdef STUB_FUNC
#undef STUB_FUNC
#endif
#define STUB_FUNC(code, func, perm) BluetoothSocketInterfaceCode::code, {&BluetoothSocketStub::func, perm}

namespace OHOS {
namespace Bluetooth {
using namespace OHOS::bluetooth;
// Note: Permissions need to be configured when the itf to be used. "nullptr" means no permission needed.
const std::map<uint32_t, BluetoothSocketStub::SocketStubFuncPerm> BluetoothSocketStub::memberFuncMap_ = {
{STUB_FUNC(SOCKET_CONNECT, ConnectInner, CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
{STUB_FUNC(SOCKET_LISTEN, ListenInner, CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
{STUB_FUNC(SOCKET_UPDATE_COC_PARAMS, UpdateCocConnectionParamsInner,
    CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
{STUB_FUNC(DEREGISTER_SERVER_OBSERVER, DeregisterServerObserverInner, nullptr)},
{STUB_FUNC(REGISTER_CLIENT_OBSERVER, RegisterClientObserverInner, nullptr)},
{STUB_FUNC(DEREGISTER_CLIENT_OBSERVER, DeregisterClientObserverInner, nullptr)},
{STUB_FUNC(SOCKET_IS_ALLOW_CONNECT, IsAllowSocketConnectInner,
    CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
};
BluetoothSocketStub::BluetoothSocketStub()
{}

BluetoothSocketStub::~BluetoothSocketStub()
{}

int32_t BluetoothSocketStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    CHECK_PERMISSION_AND_EXECUTE_FUNC_RETURN(BluetoothSocketStub);
}

int32_t BluetoothSocketStub::ConnectInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothSocketStub::ListenInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothSocketStub::DeregisterServerObserverInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("DeregisterServerObserverInner starts");
    sptr<IBluetoothServerSocketObserver> observer =
        OHOS::iface_cast<IBluetoothServerSocketObserver>(data.ReadRemoteObject());
    DeregisterServerObserver(observer);

    return NO_ERROR;
}

int32_t BluetoothSocketStub::RegisterClientObserverInner(MessageParcel &data, MessageParcel &reply)
{
    sptr<BluetoothRawAddress> addr(data.ReadStrongParcelable<BluetoothRawAddress>());
    if (!addr) {
        HILOGE("BluetoothSocketStub::RegisterClientObserver failed");
        return ERR_INVALID_VALUE;
    }
    std::shared_ptr<BluetoothUuid> uuid(data.ReadParcelable<BluetoothUuid>());
    if (uuid == nullptr) {
        HILOGE("reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    sptr<IBluetoothClientSocketObserver> observer =
        OHOS::iface_cast<IBluetoothClientSocketObserver>(data.ReadRemoteObject());
    CHECK_AND_RETURN_LOG_RET(observer != nullptr, ERR_INVALID_VALUE, "observer is nullptr");
    int result = RegisterClientObserver(*addr, *uuid, observer);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("reply writing failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return BT_NO_ERROR;
}

int32_t BluetoothSocketStub::DeregisterClientObserverInner(MessageParcel &data, MessageParcel &reply)
{
    return reply.WriteInt32(BT_NO_ERROR);
}

int32_t BluetoothSocketStub::UpdateCocConnectionParamsInner(MessageParcel &data, MessageParcel &reply)
{
    return reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
}

int32_t BluetoothSocketStub::IsAllowSocketConnectInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_NO_ERROR);
    return reply.WriteBool(true);
}
}  // namespace Bluetooth
}  // namespace OHOS