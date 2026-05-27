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

#include "bluetooth_errorcode.h"
#include "bluetooth_log.h"
#include "bluetooth_utils_server.h"
#include "bt_def.h"
#include "interface_profile_manager.h"
#include "interface_profile_socket.h"
#include "bluetooth_socket_server.h"

using namespace OHOS::bluetooth;

namespace OHOS {
namespace Bluetooth {
int BluetoothSocketServer::Connect(ConnectSocketParam &param, int &fd)
{
    IProfileSocket *socket = (IProfileSocket *)IProfileManager::GetInstance()->GetProfileService(PROFILE_NAME_SPP);
    if (socket != nullptr) {
        fd = socket->Connect(param.addr, param.uuid, (int)param.securityFlag, (int)param.type);
    }

    return NO_ERROR;
}

int BluetoothSocketServer::Listen(ListenSocketParam &param, int &fd)
{
    IProfileSocket *socket = (IProfileSocket *)IProfileManager::GetInstance()->GetProfileService(PROFILE_NAME_SPP);
    if (socket != nullptr) {
        fd = socket->Listen(param.name, param.uuid, (int)param.securityFlag, (int)param.type);
    }

    if (fd != -1 && param.observer != nullptr) {
        socketObserverList_->AddObserver(fd, param.observer->AsObject());
    }

    return NO_ERROR;
}

int BluetoothSocketServer::DeregisterServerObserver(const sptr<IBluetoothServerSocketObserver> &observer)
{
    socketObserverList_->RemoveObserver(observer->AsObject());
    return NO_ERROR;
}

int BluetoothSocketServer::RegisterClientObserver(const BluetoothRawAddress &addr, const bluetooth::Uuid uuid,
    const sptr<IBluetoothClientSocketObserver> &observer)
{
    if (!IsValidAddress(addr.GetAddress())) {
        return BT_ERR_INTERNAL_ERROR;
    }
    return BT_ERR_API_NOT_SUPPORT;
}

int BluetoothSocketServer::DeregisterClientObserver(const BluetoothRawAddress &addr, const bluetooth::Uuid uuid,
    const sptr<IBluetoothClientSocketObserver> &observer)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int BluetoothSocketServer::UpdateCocConnectionParams(const BluetoothSocketCocInfo &info)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int BluetoothSocketServer::IsAllowSocketConnect(int socketType, const std::string &addr, bool &isAllowed)
{
    return BT_ERR_API_NOT_SUPPORT;
}
}  // namespace Bluetooth
}  // namespace OHOS