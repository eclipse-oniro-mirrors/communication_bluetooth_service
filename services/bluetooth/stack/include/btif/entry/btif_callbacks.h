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

#ifndef BTIF_ENTRY_BTIF_CALLBACKS_H
#define BTIF_ENTRY_BTIF_CALLBACKS_H

#include <atomic>

#include "hardware/bluetooth.h"

// Entry owns the service callback table. Core only sees CoreCallbacks.
class BtifCallbacks final {
public:
    static bool Init(bt_callbacks_t *callbacks);
    static void Cleanup();
    static bt_callbacks_t *Get();
    static void Set(bt_callbacks_t *callbacks);

private:
    inline static std::atomic<bt_callbacks_t *> callbacks_ { nullptr };
};

#endif // BTIF_ENTRY_BTIF_CALLBACKS_H
