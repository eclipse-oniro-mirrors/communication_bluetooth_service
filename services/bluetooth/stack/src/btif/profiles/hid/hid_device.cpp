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

static bt_status_t BtifHidDeviceInit(bthd_callbacks_t* callbacks)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static void BtifHidDeviceCleanup(void)
{
    // Stub: the stack API for this profile is not wired up yet.
}

static bt_status_t BtifHidDeviceRegisterApp(bthd_app_param_t* app_param, bthd_qos_param_t* in_qos,
    bthd_qos_param_t* out_qos)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidDeviceUnregisterApp(void)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidDeviceConnect(RawAddress* bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidDeviceDisconnect(void)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidDeviceSendReport(bthd_report_type_t type, uint8_t id, uint16_t len, uint8_t* p_data)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidDeviceReportError(uint8_t error)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifHidDeviceVirtualCableUnplug(void)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

} // namespace

const void *ProfileInterfaces::GetHidDevice()
{
    static const bthd_interface_t table = {
        sizeof(bthd_interface_t),
        BtifHidDeviceInit,
        BtifHidDeviceCleanup,
        BtifHidDeviceRegisterApp,
        BtifHidDeviceUnregisterApp,
        BtifHidDeviceConnect,
        BtifHidDeviceDisconnect,
        BtifHidDeviceSendReport,
        BtifHidDeviceReportError,
        BtifHidDeviceVirtualCableUnplug,
    };
    return &table;
}
