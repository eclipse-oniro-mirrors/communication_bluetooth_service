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

#ifndef CORE_BOOTSTRAP_H
#define CORE_BOOTSTRAP_H

#include <memory>

#include "hardware/bluetooth.h"

using CoreProfileInterfaceProvider = const void *(*)(const char *profileId);

class CoreAdapterController final {
public:
    struct State;

    CoreAdapterController();
    ~CoreAdapterController();
    static CoreAdapterController &GetInstance();
    State &GetState();

    static int Init(bt_callbacks_t *callbacks, bool guestMode, bool isCommonCriteriaMode,
        int configCompareResult, const char **initFlags, bool isAtv);
    static void Cleanup();
    static void FillInterfaceTable(bt_interface_t *iface);
    static void SetProfileInterfaceProvider(CoreProfileInterfaceProvider provider);

private:
    CoreAdapterController(const CoreAdapterController &) = delete;
    CoreAdapterController &operator=(const CoreAdapterController &) = delete;
    std::unique_ptr<State> state_;
};

#endif // CORE_BOOTSTRAP_H
