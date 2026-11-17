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

#ifndef BTIF_COMMON_BTIF_ASYNC_TASK_H
#define BTIF_COMMON_BTIF_ASYNC_TASK_H

#include <functional>
#include <string>

namespace bluetooth {
namespace btif {

enum class BtifTaskLane {
    CALLBACK, // service-layer callbacks, never run on GAP/HCI task threads
    CONTROL,  // blocking GAP operations, serialized separately from callbacks
};

enum class BtifTaskPolicy {
    FIFO,
    COALESCE_BY_KEY,
    DROP_OLDEST,
};

class TaskScheduler final {
public:
    // Returns false only when the lane is shutting down or its queue is full.
    static bool Post(BtifTaskLane lane, std::function<void()> task,
        BtifTaskPolicy policy = BtifTaskPolicy::FIFO, const std::string &key = "");

    // Stop both workers. Idempotent; intended for stack cleanup.
    static void Cleanup();
};

} // namespace btif
} // namespace bluetooth

#endif // BTIF_COMMON_BTIF_ASYNC_TASK_H
