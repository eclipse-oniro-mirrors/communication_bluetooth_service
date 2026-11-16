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

#ifndef CORE_BLE_SCANNER_H
#define CORE_BLE_SCANNER_H

#include <cstdint>
#include <memory>
#include <string>

#include "hardware/ble_scanner.h"
#include "types/raw_address.h"

using BtifBleDiscoveryCallback = void (*)(const RawAddress &address, const std::string &name, int8_t rssi);

enum class BtifBleScanCaller : uint8_t {
    GATT = 1,
    DISCOVERY = 1 << 1,
    FRAMEWORK = 1 << 1 << 1,
};

BleScannerInterface *BtifGetBleScannerInstance();
void BtifStartBleDiscovery(BtifBleDiscoveryCallback callback);
void BtifStopBleDiscovery();
void BtifSetBleScanCallerActive(BtifBleScanCaller caller, bool active);
void BtifBleScanRecover();

// GATT owns the callback registration; core scanner consumes the active
// provider without depending on the profiles/gatt header.
const btgatt_scanner_callbacks_t *BtifGetGattScannerCallbacks();


class CoreBleScanner final {
public:
    struct State;

    CoreBleScanner();
    ~CoreBleScanner();
    static CoreBleScanner &GetInstance();
    State &GetState();

    BleScannerInterface *GetInterface();
    void StartDiscovery(BtifBleDiscoveryCallback callback);
    void StopDiscovery();
    void SetCallerActive(BtifBleScanCaller caller, bool active);
    void Recover();

private:
    CoreBleScanner(const CoreBleScanner &) = delete;
    CoreBleScanner &operator=(const CoreBleScanner &) = delete;
    std::unique_ptr<State> state_;
};

#endif // CORE_BLE_SCANNER_H
