/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CORE_SEARCH_STATE_H
#define CORE_SEARCH_STATE_H

#include <cstdint>
#include <mutex>

namespace bluetooth {
namespace btif {

// Pure discovery state machine. It deliberately has no GAP/HCI dependency so
// the transition rules can be tested without a controller.
class CoreDiscoveryStateMachine {
public:
    enum class State : uint8_t {
        IDLE,
        ACTIVE,
        CANCELLING,
    };

    struct StartResult {
        bool changed; // false for duplicate ACTIVE or queued start
        bool queued;  // true when the request was queued during CANCELLING
    };

    StartResult Start();
    bool Cancel();
    bool Complete(bool &wasCancelling);
    bool ConsumePendingStart();
    State Stop();

    State GetState() const;
    bool IsPendingStart() const;
    static const char *Name(State state);

private:
    mutable std::mutex mutex_;
    State state_ = State::IDLE;
    bool pendingStart_ = false;
};

} // namespace btif
} // namespace bluetooth

#endif // CORE_SEARCH_STATE_H
