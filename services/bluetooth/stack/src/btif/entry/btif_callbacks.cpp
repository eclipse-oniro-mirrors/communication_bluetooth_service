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

#include "btif/entry/btif_callbacks.h"

#include <atomic>

#include "btif/core/core_callbacks.h"

namespace {

void AdapterStateChanged(bt_state_t state)
{
    bt_callbacks_t *callbacks = BtifCallbacks::Get();
    if (callbacks != nullptr && callbacks->adapter_state_changed_cb != nullptr) {
        callbacks->adapter_state_changed_cb(state);
    }
}

void AdapterProperties(bt_status_t status, int numProperties, bt_property_t *properties)
{
    bt_callbacks_t *callbacks = BtifCallbacks::Get();
    if (callbacks != nullptr && callbacks->adapter_properties_cb != nullptr) {
        callbacks->adapter_properties_cb(status, numProperties, properties);
    }
}

void RemoteDeviceProperties(bt_status_t status, RawAddress *address, int numProperties,
    bt_property_t *properties)
{
    bt_callbacks_t *callbacks = BtifCallbacks::Get();
    if (callbacks != nullptr && callbacks->remote_device_properties_cb != nullptr) {
        callbacks->remote_device_properties_cb(status, address, numProperties, properties);
    }
}

void AclStateChanged(bt_status_t status, RawAddress *address, bt_acl_state_t state,
    bt_hci_error_code_t reason, tBT_TRANSPORT transport)
{
    bt_callbacks_t *callbacks = BtifCallbacks::Get();
    if (callbacks != nullptr && callbacks->acl_state_changed_cb != nullptr) {
        callbacks->acl_state_changed_cb(status, address, state, reason, transport);
    }
}

void DiscoveryStateChanged(bt_discovery_state_t state)
{
    bt_callbacks_t *callbacks = BtifCallbacks::Get();
    if (callbacks != nullptr && callbacks->discovery_state_changed_cb != nullptr) {
        callbacks->discovery_state_changed_cb(state);
    }
}

void BondStateChanged(bt_status_t status, RawAddress *address, bt_bond_state_t state)
{
    bt_callbacks_t *callbacks = BtifCallbacks::Get();
    if (callbacks != nullptr && callbacks->bond_state_changed_cb != nullptr) {
        callbacks->bond_state_changed_cb(status, address, state);
    }
}

void DeviceFound(int numProperties, bt_property_t *properties)
{
    bt_callbacks_t *callbacks = BtifCallbacks::Get();
    if (callbacks != nullptr && callbacks->device_found_cb != nullptr) {
        callbacks->device_found_cb(numProperties, properties);
    }
}

void SspRequest(RawAddress *address, bt_bdname_t *name, uint32_t cod,
    bt_ssp_variant_t variant, uint32_t passkey)
{
    bt_callbacks_t *callbacks = BtifCallbacks::Get();
    if (callbacks != nullptr && callbacks->ssp_request_cb != nullptr) {
        callbacks->ssp_request_cb(address, name, cod, variant, passkey);
    }
}

void PinRequest(RawAddress *address, bt_bdname_t *name, uint32_t cod, bool min16Digit)
{
    bt_callbacks_t *callbacks = BtifCallbacks::Get();
    if (callbacks != nullptr && callbacks->pin_request_cb != nullptr) {
        callbacks->pin_request_cb(address, name, cod, min16Digit);
    }
}

const CoreCallbacks kCoreCallbacks = {
    AdapterStateChanged,
    AdapterProperties,
    RemoteDeviceProperties,
    AclStateChanged,
    DiscoveryStateChanged,
    BondStateChanged,
    DeviceFound,
    SspRequest,
    PinRequest,
};

} // namespace

bool BtifCallbacks::Init(bt_callbacks_t *callbacks)
{
    if (callbacks == nullptr) {
        return false;
    }
    Set(callbacks);
    CoreCallbackRegistry::Register(&kCoreCallbacks);
    return true;
}

void BtifCallbacks::Cleanup()
{
    CoreCallbackRegistry::Unregister();
    callbacks_.store(nullptr);
}

bt_callbacks_t *BtifCallbacks::Get()
{
    return callbacks_.load();
}

void BtifCallbacks::Set(bt_callbacks_t *callbacks)
{
    callbacks_.store(callbacks);
}
