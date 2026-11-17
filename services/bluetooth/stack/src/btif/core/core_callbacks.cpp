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

#include "btif/core/core_callbacks.h"

#include <mutex>

struct CoreCallbackRegistry::Impl {
    mutable std::mutex mutex;
    const CoreCallbacks *callbacks = nullptr;

    const CoreCallbacks *Get() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return callbacks;
    }
};

CoreCallbackRegistry::CoreCallbackRegistry() : impl_(std::make_unique<Impl>()) {}

CoreCallbackRegistry::~CoreCallbackRegistry() = default;

CoreCallbackRegistry &CoreCallbackRegistry::GetInstance()
{
    static CoreCallbackRegistry instance;
    return instance;
}

CoreCallbackRegistry::Impl &CoreCallbackRegistry::GetImpl()
{
    return *GetInstance().impl_;
}

void CoreCallbackRegistry::Register(const CoreCallbacks *callbacks)
{
    std::lock_guard<std::mutex> lock(GetImpl().mutex);
    GetImpl().callbacks = callbacks;
}

void CoreCallbackRegistry::Unregister()
{
    std::lock_guard<std::mutex> lock(GetImpl().mutex);
    GetImpl().callbacks = nullptr;
}

bool CoreCallbackRegistry::HasAdapterProperties()
{
    const CoreCallbacks *callbacks = GetImpl().Get();
    return callbacks != nullptr && callbacks->adapterProperties != nullptr;
}

bool CoreCallbackRegistry::HasRemoteDeviceProperties()
{
    const CoreCallbacks *callbacks = GetImpl().Get();
    return callbacks != nullptr && callbacks->remoteDeviceProperties != nullptr;
}

bool CoreCallbackRegistry::HasDeviceFound()
{
    const CoreCallbacks *callbacks = GetImpl().Get();
    return callbacks != nullptr && callbacks->deviceFound != nullptr;
}

bool CoreCallbackRegistry::HasSspRequest()
{
    const CoreCallbacks *callbacks = GetImpl().Get();
    return callbacks != nullptr && callbacks->sspRequest != nullptr;
}

bool CoreCallbackRegistry::HasPinRequest()
{
    const CoreCallbacks *callbacks = GetImpl().Get();
    return callbacks != nullptr && callbacks->pinRequest != nullptr;
}

void CoreCallbackRegistry::NotifyAdapterState(bt_state_t state)
{
    const CoreCallbacks *callbacks = GetImpl().Get();
    if (callbacks != nullptr && callbacks->adapterStateChanged != nullptr) {
        callbacks->adapterStateChanged(state);
    }
}

void CoreCallbackRegistry::NotifyAdapterProperties(bt_status_t status, int numProperties,
    bt_property_t *properties)
{
    const CoreCallbacks *callbacks = GetImpl().Get();
    if (callbacks != nullptr && callbacks->adapterProperties != nullptr) {
        callbacks->adapterProperties(status, numProperties, properties);
    }
}

void CoreCallbackRegistry::NotifyRemoteDeviceProperties(bt_status_t status, RawAddress *address,
    int numProperties, bt_property_t *properties)
{
    const CoreCallbacks *callbacks = GetImpl().Get();
    if (callbacks != nullptr && callbacks->remoteDeviceProperties != nullptr) {
        callbacks->remoteDeviceProperties(status, address, numProperties, properties);
    }
}

void CoreCallbackRegistry::NotifyAclState(bt_status_t status, RawAddress *address,
    bt_acl_state_t state, bt_hci_error_code_t reason, tBT_TRANSPORT transport)
{
    const CoreCallbacks *callbacks = GetImpl().Get();
    if (callbacks != nullptr && callbacks->aclStateChanged != nullptr) {
        callbacks->aclStateChanged(status, address, state, reason, transport);
    }
}

void CoreCallbackRegistry::NotifyDiscoveryState(bt_discovery_state_t state)
{
    const CoreCallbacks *callbacks = GetImpl().Get();
    if (callbacks != nullptr && callbacks->discoveryStateChanged != nullptr) {
        callbacks->discoveryStateChanged(state);
    }
}

bool CoreCallbackRegistry::NotifyBondState(bt_status_t status, RawAddress *address, bt_bond_state_t state)
{
    const CoreCallbacks *callbacks = GetImpl().Get();
    if (callbacks == nullptr || callbacks->bondStateChanged == nullptr) {
        return false;
    }
    callbacks->bondStateChanged(status, address, state);
    return true;
}

void CoreCallbackRegistry::NotifyDeviceFound(int numProperties, bt_property_t *properties)
{
    const CoreCallbacks *callbacks = GetImpl().Get();
    if (callbacks != nullptr && callbacks->deviceFound != nullptr) {
        callbacks->deviceFound(numProperties, properties);
    }
}

bool CoreCallbackRegistry::NotifySspRequest(RawAddress *address, bt_bdname_t *name, uint32_t cod,
    bt_ssp_variant_t variant, uint32_t passkey)
{
    const CoreCallbacks *callbacks = GetImpl().Get();
    if (callbacks == nullptr || callbacks->sspRequest == nullptr) {
        return false;
    }
    callbacks->sspRequest(address, name, cod, variant, passkey);
    return true;
}

bool CoreCallbackRegistry::NotifyPinRequest(RawAddress *address, bt_bdname_t *name,
    uint32_t cod, bool min16Digit)
{
    const CoreCallbacks *callbacks = GetImpl().Get();
    if (callbacks == nullptr || callbacks->pinRequest == nullptr) {
        return false;
    }
    callbacks->pinRequest(address, name, cod, min16Digit);
    return true;
}
