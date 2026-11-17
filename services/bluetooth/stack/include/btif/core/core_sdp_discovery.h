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

#ifndef CORE_SDP_DISCOVERY_H
#define CORE_SDP_DISCOVERY_H

#include <cstdint>
#include <memory>

#include "hardware/bluetooth.h"
#include "types/raw_address.h"

class CoreSdpDiscovery final {
public:
    struct State;

    CoreSdpDiscovery();
    ~CoreSdpDiscovery();
    static CoreSdpDiscovery &GetInstance();
    State &GetState();

    static void QueryRemoteUuids(const RawAddress &address, bool forceRefresh);
    static void OnBonded(const RawAddress &address);
    static void OnUnpaired(const RawAddress &address);
    static void LoadStore();
    static void ReportCachedUuids();
    static void Cleanup();

private:
    CoreSdpDiscovery(const CoreSdpDiscovery &) = delete;
    CoreSdpDiscovery &operator=(const CoreSdpDiscovery &) = delete;
    std::unique_ptr<State> state_;
};

#endif // CORE_SDP_DISCOVERY_H
