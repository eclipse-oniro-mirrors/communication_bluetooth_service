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

static bt_status_t BtifGattClientRegisterClient(const bluetooth::Uuid& uuid, bool eatt_support)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientUnregisterClient(int client_if)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientConnect(int client_if, const RawAddress& bd_addr, bool is_direct, int transport,
    bool opportunistic, int initiating_phys)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientDisconnect(int client_if, const RawAddress& bd_addr, int connId)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientRefresh(int client_if, const RawAddress& bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientSearchService(int connId, const bluetooth::Uuid* filter_uuid)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static void BtifGattClientBtifGattcDiscoverServiceByUuid(int connId, const bluetooth::Uuid& uuid)
{
    // Stub: the stack API for this profile is not wired up yet.
}

static bt_status_t BtifGattClientReadCharacteristic(int connId, uint16_t handle, int auth_req)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientReadUsingCharacteristicUuid(int connId, const bluetooth::Uuid& uuid, uint16_t s_handle,
    uint16_t e_handle, int auth_req)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientWriteCharacteristic(int connId, uint16_t handle, int write_type, int auth_req,
    std::vector<uint8_t> value)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientReadDescriptor(int connId, uint16_t handle, int auth_req)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientWriteDescriptor(int connId, uint16_t handle, int auth_req, std::vector<uint8_t> value)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientExecuteWrite(int connId, int execute)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientRegisterForNotification(int client_if, const RawAddress& bd_addr, uint16_t handle)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientDeregisterForNotification(int client_if, const RawAddress& bd_addr, uint16_t handle)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientReadRemoteRssi(int client_if, const RawAddress& bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static int BtifGattClientGetDeviceType(const RawAddress& bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientConfigureMtu(int connId, int mtu)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientConnParameterUpdate(const RawAddress& bd_addr, int min_interval, int max_interval,
    int latency, int timeout, uint16_t min_ce_len, uint16_t max_ce_len)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientSetPreferredPhy(const RawAddress& bd_addr, uint8_t tx_phy, uint8_t rx_phy,
    uint16_t phy_options)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientReadPhy(const RawAddress& bd_addr,
    std::function<void(uint8_t tx_phy, uint8_t rx_phy, uint8_t status)> cb)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientTestCommand(int command, const btgatt_test_params_t& params)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientGetGattDb(int connId)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientRegisterConnStateCb()
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifGattClientDeregisterConnStateCb()
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

} // namespace

const btgatt_client_interface_t *BtifGetGattClientInterface()
{
    static const btgatt_client_interface_t table = {
        BtifGattClientRegisterClient,
        BtifGattClientUnregisterClient,
        BtifGattClientConnect,
        BtifGattClientDisconnect,
        BtifGattClientRefresh,
        BtifGattClientSearchService,
        BtifGattClientBtifGattcDiscoverServiceByUuid,
        BtifGattClientReadCharacteristic,
        BtifGattClientReadUsingCharacteristicUuid,
        BtifGattClientWriteCharacteristic,
        BtifGattClientReadDescriptor,
        BtifGattClientWriteDescriptor,
        BtifGattClientExecuteWrite,
        BtifGattClientRegisterForNotification,
        BtifGattClientDeregisterForNotification,
        BtifGattClientReadRemoteRssi,
        BtifGattClientGetDeviceType,
        BtifGattClientConfigureMtu,
        BtifGattClientConnParameterUpdate,
        BtifGattClientSetPreferredPhy,
        BtifGattClientReadPhy,
        BtifGattClientTestCommand,
        BtifGattClientGetGattDb,
        BtifGattClientRegisterConnStateCb,
        BtifGattClientDeregisterConnStateCb,
    };
    return &table;
}
