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

#ifndef CORE_CALLBACKS_H
#define CORE_CALLBACKS_H

#include <memory>

#include "hardware/bluetooth.h"

class CoreCallbackRegistry final {
public:
    static CoreCallbackRegistry &GetInstance();

    static void Register(const struct CoreCallbacks *callbacks);
    static void Unregister();

    static bool HasAdapterProperties();
    static bool HasRemoteDeviceProperties();
    static bool HasDeviceFound();
    static bool HasSspRequest();
    static bool HasPinRequest();

    static void NotifyAdapterState(bt_state_t state);
    static void NotifyAdapterProperties(bt_status_t status, int numProperties, bt_property_t *properties);
    static void NotifyRemoteDeviceProperties(bt_status_t status, RawAddress *address, int numProperties,
        bt_property_t *properties);
    static void NotifyAclState(bt_status_t status, RawAddress *address, bt_acl_state_t state,
        bt_hci_error_code_t reason, tBT_TRANSPORT transport);
    static void NotifyDiscoveryState(bt_discovery_state_t state);
    static bool NotifyBondState(bt_status_t status, RawAddress *address, bt_bond_state_t state);
    static void NotifyDeviceFound(int numProperties, bt_property_t *properties);
    static bool NotifySspRequest(RawAddress *address, bt_bdname_t *name, uint32_t cod,
        bt_ssp_variant_t variant, uint32_t passkey);
    static bool NotifyPinRequest(RawAddress *address, bt_bdname_t *name, uint32_t cod, bool min16Digit);

private:
    CoreCallbackRegistry();
    ~CoreCallbackRegistry();
    CoreCallbackRegistry(const CoreCallbackRegistry &) = delete;
    CoreCallbackRegistry &operator=(const CoreCallbackRegistry &) = delete;

    struct Impl;
    static Impl &GetImpl();
    std::unique_ptr<Impl> impl_;
};

struct CoreCallbacks {
    void (*adapterStateChanged)(bt_state_t state);
    void (*adapterProperties)(bt_status_t status, int numProperties, bt_property_t *properties);
    void (*remoteDeviceProperties)(bt_status_t status, RawAddress *address, int numProperties,
        bt_property_t *properties);
    void (*aclStateChanged)(bt_status_t status, RawAddress *address, bt_acl_state_t state,
        bt_hci_error_code_t reason, tBT_TRANSPORT transport);
    void (*discoveryStateChanged)(bt_discovery_state_t state);
    void (*bondStateChanged)(bt_status_t status, RawAddress *address, bt_bond_state_t state);
    void (*deviceFound)(int numProperties, bt_property_t *properties);
    void (*sspRequest)(RawAddress *address, bt_bdname_t *name, uint32_t cod,
        bt_ssp_variant_t variant, uint32_t passkey);
    void (*pinRequest)(RawAddress *address, bt_bdname_t *name, uint32_t cod, bool min16Digit);
};

#endif // CORE_CALLBACKS_H
