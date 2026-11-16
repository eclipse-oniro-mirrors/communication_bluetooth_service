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

#ifndef CORE_RUNTIME_H
#define CORE_RUNTIME_H

#include "btif/core/core_ble_scanner.h"
#include "btif/core/core_adapter_controller.h"
#include "btif/core/core_callbacks.h"
#include "btif/core/core_device_store.h"
#include "btif/core/core_pairing_manager.h"
#include "btif/core/core_sdp_discovery.h"
#include "btif/core/core_discovery_manager.h"

namespace bluetooth {
namespace btif {

// Process-wide composition root. Entry owns the lifecycle; core modules use
// the registry through this runtime instead of static callback globals.
class CoreRuntime final {
public:
    static CoreRuntime &GetInstance();

    CoreCallbackRegistry &Callbacks();
    CoreAdapterController &Adapter();
    CoreBleScanner &Scanner();
    CoreDeviceStore &Devices();
    CoreSdpDiscovery &Sdp();
    CoreDiscoveryManager &Discovery();
    CorePairingManager &Pairing();

    bool Initialize();
    void Shutdown();
    bool IsInitialized() const;

private:
    CoreRuntime() = default;
    ~CoreRuntime() = default;
    CoreRuntime(const CoreRuntime &) = delete;
    CoreRuntime &operator=(const CoreRuntime &) = delete;

    bool initialized_ = false;
    CoreAdapterController &adapter_ = ::CoreAdapterController::GetInstance();
    CoreBleScanner &scanner_ = CoreBleScanner::GetInstance();
    CoreDeviceStore &devices_ = CoreDeviceStore::GetInstance();
    CoreSdpDiscovery &sdp_ = ::CoreSdpDiscovery::GetInstance();
    CoreDiscoveryManager &discovery_ = CoreDiscoveryManager::GetInstance();
    CorePairingManager &pairing_ = ::CorePairingManager::GetInstance();
};

} // namespace btif
} // namespace bluetooth

#endif // CORE_RUNTIME_H
