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

#include "btif/profiles/pan/pan.h"
#include "btif/profiles/profile_interfaces.h"

namespace {

static bt_status_t BtifPanInit(const btpan_callbacks_t* callbacks)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifPanEnable(int local_role)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static int BtifPanGetLocalRole(void)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifPanConnect(const RawAddress* bd_addr, int local_role, int remote_role)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifPanDisconnect(const RawAddress* bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static void BtifPanCleanup(void)
{
    // Stub: the stack API for this profile is not wired up yet.
}

} // namespace

const void *ProfileInterfaces::GetPan()
{
    static const btpan_interface_t table = {
        sizeof(btpan_interface_t),
        BtifPanInit,
        BtifPanEnable,
        BtifPanGetLocalRole,
        BtifPanConnect,
        BtifPanDisconnect,
        BtifPanCleanup,
    };
    return &table;
}
