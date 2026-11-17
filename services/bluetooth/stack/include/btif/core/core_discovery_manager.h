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

#ifndef CORE_SEARCH_H
#define CORE_SEARCH_H

#include <cstddef>
#include <functional>
#include <memory>
#include <string>

#include "hardware/bluetooth.h"

class CoreDiscoveryManager final {
public:
    struct State;

    CoreDiscoveryManager();
    ~CoreDiscoveryManager();
    static CoreDiscoveryManager &GetInstance();
    State &GetState();

    static int Start();
    static int Cancel();
    static bool RegisterCallback();
    static void UnregisterCallback();
    static void Stop();
    static void SetPairingMode(bool enabled);
    static bool IsPairingMode();
    static void RequestRemoteName(const RawAddress &address);

private:
    CoreDiscoveryManager(const CoreDiscoveryManager &) = delete;
    CoreDiscoveryManager &operator=(const CoreDiscoveryManager &) = delete;
    std::unique_ptr<State> state_;
};

#endif // CORE_SEARCH_H
