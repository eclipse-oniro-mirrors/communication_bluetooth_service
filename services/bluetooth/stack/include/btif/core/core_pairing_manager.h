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

#ifndef CORE_PAIRING_H
#define CORE_PAIRING_H

#include <cstddef>
#include <memory>

#include "hardware/bluetooth.h"

class CorePairingManager final {
public:
    struct State;

    CorePairingManager();
    ~CorePairingManager();
    static CorePairingManager &GetInstance();
    State &GetState();

    static int SetSecurityMode();
    static bool RegisterCallback();
    static void Cleanup();
    static void LoadStore();
    static size_t GetBondedDevices(RawAddress *devices, size_t maxDevices);
    static bool IsBonded(const RawAddress &address);
    static void NotifyBondedDevices();
    static void RememberLeAddress(const RawAddress &address, uint8_t addressType);
    static void RememberBrAddress(const RawAddress &address);
    static void OnLeAclConnected(const RawAddress &address);
    static void OnLeAclDisconnected(const RawAddress &address);
    static int CreateBond(const RawAddress *bdAddr, int transport);
    static int RemoveBond(const RawAddress *bdAddr);
    static int CancelBond(const RawAddress *bdAddr);
    static int PinReply(const RawAddress *bdAddr, uint8_t accept, uint8_t pinLen, bt_pin_code_t *pinCode);
    static int SspReply(const RawAddress *bdAddr, bt_ssp_variant_t variant, uint8_t accept, uint32_t passkey);

private:
    CorePairingManager(const CorePairingManager &) = delete;
    CorePairingManager &operator=(const CorePairingManager &) = delete;
    std::unique_ptr<State> state_;
};

#endif // CORE_PAIRING_H
