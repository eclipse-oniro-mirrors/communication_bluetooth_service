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

#include "btif/profiles/hid/hid.h"
#include "btif/profiles/profile_interfaces.h"

namespace {

static bt_status_t BtifHidHostInit(bthh_callbacks_t* callbacks)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidHostConnect(RawAddress* bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidHostDisconnect(RawAddress* bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidHostVirtualUnplug(RawAddress* bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidHostSetInfo(RawAddress* bd_addr, bthh_hid_info_t hid_info)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidHostGetProtocol(RawAddress* bd_addr, bthh_protocol_mode_t protocolMode)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidHostSetProtocol(RawAddress* bd_addr, bthh_protocol_mode_t protocolMode)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidHostGetIdleTime(RawAddress* bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidHostSetIdleTime(RawAddress* bd_addr, uint8_t idleTime)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidHostGetReport(RawAddress* bd_addr, bthh_report_type_t reportType, uint8_t reportId,
    int bufferSize)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidHostSetReport(RawAddress* bd_addr, bthh_report_type_t reportType, char* report)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidHostSendData(RawAddress* bd_addr, char* data)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static void BtifHidHostCleanup(void)
{
    // Stub: the stack API for this profile is not wired up yet.
}

} // namespace

const void *ProfileInterfaces::GetHidHost()
{
    static const bthh_interface_t table = {
        sizeof(bthh_interface_t),
        BtifHidHostInit,
        BtifHidHostConnect,
        BtifHidHostDisconnect,
        BtifHidHostVirtualUnplug,
        BtifHidHostSetInfo,
        BtifHidHostGetProtocol,
        BtifHidHostSetProtocol,
        BtifHidHostGetIdleTime,
        BtifHidHostSetIdleTime,
        BtifHidHostGetReport,
        BtifHidHostSetReport,
        BtifHidHostSendData,
        BtifHidHostCleanup,
    };
    return &table;
}
