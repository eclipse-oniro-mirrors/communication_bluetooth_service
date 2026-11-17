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

#include "btif/entry/btif_entry.h"

#include "btif/core/core_adapter_controller.h"
#include "btif/core/core_ble_scanner.h"
#include "btif/core/core_runtime.h"
#include "btif/entry/btif_callbacks.h"
#include "btif/entry/btif_dispatch.h"
#include "btif/profiles/ble/ble.h"
#include "btif/profiles/gatt/gatt.h"
#include "btif_dm.h"
#include "log.h"

using bluetooth::btif::CoreRuntime;

/* Stable C ABI symbols resolved by the service layer via dlsym. */
extern "C" __attribute__((visibility("default"))) btgatt_client_interface_t btgattClientInterface = {};
extern "C" __attribute__((visibility("default"))) btgatt_server_interface_t btgattServerInterface = {};

namespace {

struct GattSymbolBootstrap {
    GattSymbolBootstrap()
    {
        btgattClientInterface = *BtifGetGattClientInterface();
        btgattServerInterface = *BtifGetGattServerInterface();
    }
};

int BtifEntryInit(
    bt_callbacks_t *callbacks, bool guestMode, bool isCommonCriteriaMode, int configCompareResult,
    const char **initFlags, bool isAtv
)
{
    CoreRuntime &runtime = CoreRuntime::GetInstance();
    if (!runtime.Initialize()) {
        return BT_STATUS_FAIL;
    }
    if (!BtifCallbacks::Init(callbacks)) {
        runtime.Shutdown();
        return BT_STATUS_PARM_INVALID;
    }
    int result =
        runtime.Adapter().Init(callbacks, guestMode, isCommonCriteriaMode, configCompareResult, initFlags, isAtv);
    if (result != BT_STATUS_SUCCESS) {
        BtifCallbacks::Cleanup();
        runtime.Shutdown();
    }
    return result;
}

void BtifEntryCleanup()
{
    CoreRuntime &runtime = CoreRuntime::GetInstance();
    runtime.Adapter().Cleanup();
    BtifCallbacks::Cleanup();
    runtime.Shutdown();
}

void FillBtInterface(bt_interface_t *iface)
{
    static GattSymbolBootstrap gattSymbolBootstrap;
    if (iface == nullptr) {
        return;
    }
    ::CoreAdapterController::SetProfileInterfaceProvider(BtifGetProfileInterface);
    ::CoreAdapterController::FillInterfaceTable(iface);
    iface->init = BtifEntryInit;
    iface->cleanup = BtifEntryCleanup;
}

}  // namespace

extern "C" __attribute__((visibility("default"))) bt_interface_t *get_bt_interface(void)
{
    static bt_interface_t iface = {};
    FillBtInterface(&iface);
    return &iface;
}

extern "C" __attribute__((visibility("default"))) BleAdvertiserInterface *get_ble_advertiser_instance(void)
{
    return BtifGetBleAdvertiserInstance();
}

extern "C" __attribute__((visibility("default"))) BleScannerInterface *get_ble_scanner_instance(void)
{
    return BtifGetBleScannerInstance();
}

/*
 * GATT 接口表的 C 符号导出。
 * 框架侧按名字 dlsym（stack_interface/src/stack_stub.cpp 的
 * "get_gatt_client_interface" / "get_gatt_server_interface"），而本仓的实现是
 * C++ 名字 BtifGetGattClientInterface/BtifGetGattServerInterface，符号会被 mangle，
 * 故必须在此以 extern "C" 重新导出，否则绑定失败。
 */
extern "C" __attribute__((visibility("default"))) const btgatt_client_interface_t *get_gatt_client_interface(void)
{
    return BtifGetGattClientInterface();
}

extern "C" __attribute__((visibility("default"))) const btgatt_server_interface_t *get_gatt_server_interface(void)
{
    return BtifGetGattServerInterface();
}

extern "C" __attribute__((visibility("default"))) bool btif_get_address_type(
    const RawAddress &bda, tBLE_ADDR_TYPE *pAddrType
)
{
    (void)bda;
    HILOGW("%{public}s: stub called, output=%{public}d", __FUNCTION__, pAddrType != nullptr ? 1 : 0);
    if (pAddrType != nullptr) {
        *pAddrType = BLE_ADDR_ANONYMOUS;
    }
    return false;
}

extern "C" __attribute__((visibility("default"))) bt_status_t btif_storage_get_remote_addr_type(
    const RawAddress *remoteBdAddr, uint8_t *pRemoteAddrType
)
{
    (void)remoteBdAddr;
    HILOGW("%{public}s: stub called, output=%{public}d", __FUNCTION__, pRemoteAddrType != nullptr ? 1 : 0);
    if (pRemoteAddrType != nullptr) {
        *pRemoteAddrType = {};
    }
    return BT_STATUS_FAIL;
}

// Keep the C ABI symbol name; the service resolves it by name.
extern "C" __attribute__((visibility("default"))) void btif_sock_cleanup(void)
{
    HILOGW("%{public}s: stub called, no socket resources to clean", __FUNCTION__);
}
