/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include "btif/profiles/socket/socket.h"
#include "btif/profiles/profile_interfaces.h"

namespace {

static bt_status_t BtifSocketListen(btsock_type_t type, const char* service_name, const bluetooth::Uuid* service_uuid,
    int channel, int* sock_fd, int flags, int callingUid, int callingPid, const char* appName)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifSocketConnect(const RawAddress* bd_addr, btsock_type_t type, const bluetooth::Uuid* uuid,
    int channel, int* sock_fd, int flags, int callingUid, int callingPid, const char* appName)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static void BtifSocketRequestMaxTxDataLength(const RawAddress& bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
}

} // namespace

const void *ProfileInterfaces::GetSocket()
{
    static const btsock_interface_t table = {
        sizeof(btsock_interface_t),
        BtifSocketListen,
        BtifSocketConnect,
        BtifSocketRequestMaxTxDataLength,
    };
    return &table;
}
