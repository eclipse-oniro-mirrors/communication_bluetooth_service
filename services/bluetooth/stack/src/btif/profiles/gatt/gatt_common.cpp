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

#include "btif/profiles/gatt/gatt.h"

#include "btif/core/core_ble_scanner.h"
#include "btif/profiles/ble/ble.h"
#include "btif/profiles/profile_interfaces.h"

namespace {

const btgatt_callbacks_t *&GattCallbacks()
{
    static const btgatt_callbacks_t *callbacks = nullptr;
    return callbacks;
}

bt_status_t BtifGattInit(const btgatt_callbacks_t *callbacks)
{
    GattCallbacks() = callbacks;
    return BT_STATUS_SUCCESS;
}

void BtifGattCleanup()
{
    GattCallbacks() = nullptr;
}

} // namespace

const btgatt_scanner_callbacks_t *BtifGetGattScannerCallbacks()
{
    if (GattCallbacks() == nullptr) {
        return nullptr;
    }
    return GattCallbacks()->scanner;
}

const void *ProfileInterfaces::GetGatt()
{
    static const btgatt_interface_t table = {
        sizeof(btgatt_interface_t),
        BtifGattInit,
        BtifGattCleanup,
        BtifGetGattClientInterface(),
        BtifGetGattServerInterface(),
        BtifGetBleScannerInstance(),
        BtifGetBleAdvertiserInstance(),
    };
    return &table;
}
