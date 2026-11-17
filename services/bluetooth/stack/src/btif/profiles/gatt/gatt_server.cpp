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

namespace {

static bt_status_t BtifGattServerRegisterServer(const bluetooth::Uuid& uuid, bool eatt_support)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattServerUnregisterServer(int server_if)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattServerConnect(int server_if, const RawAddress& bd_addr, bool is_direct, int transport)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattServerDisconnect(int server_if, const RawAddress& bd_addr, int conn_id)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattServerAddService(int server_if, std::vector<btgatt_db_element_t> service)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattServerStopService(int server_if, int service_handle)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattServerDeleteService(int server_if, int service_handle)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattServerSendIndication(int server_if, int attribute_handle, int conn_id, int confirm,
    std::vector<uint8_t> value)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattServerSendResponse(int conn_id, int trans_id, int status, const btgatt_response_t& response)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattServerSetPreferredPhy(const RawAddress& bd_addr, uint8_t tx_phy, uint8_t rx_phy,
    uint16_t phy_options)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattServerReadPhy(const RawAddress& bd_addr,
    std::function<void(uint8_t tx_phy, uint8_t rx_phy, uint8_t status)> cb)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

} // namespace

const btgatt_server_interface_t *BtifGetGattServerInterface()
{
    static const btgatt_server_interface_t table = {
        BtifGattServerRegisterServer,
        BtifGattServerUnregisterServer,
        BtifGattServerConnect,
        BtifGattServerDisconnect,
        BtifGattServerAddService,
        BtifGattServerStopService,
        BtifGattServerDeleteService,
        BtifGattServerSendIndication,
        BtifGattServerSendResponse,
        BtifGattServerSetPreferredPhy,
        BtifGattServerReadPhy,
    };
    return &table;
}
