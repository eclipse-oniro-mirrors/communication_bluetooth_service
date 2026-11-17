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

#include "btif/core/core_discovery_state_machine.h"

namespace bluetooth {
namespace btif {

CoreDiscoveryStateMachine::StartResult CoreDiscoveryStateMachine::Start()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (state_ == State::ACTIVE) {
        return { false, false };
    }
    if (state_ == State::CANCELLING) {
        pendingStart_ = true;
        return { false, true };
    }
    state_ = State::ACTIVE;
    return { true, false };
}

bool CoreDiscoveryStateMachine::Cancel()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (state_ != State::ACTIVE) {
        return false;
    }
    state_ = State::CANCELLING;
    return true;
}

bool CoreDiscoveryStateMachine::Complete(bool &wasCancelling)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (state_ == State::IDLE) {
        return false;
    }
    wasCancelling = state_ == State::CANCELLING;
    state_ = State::IDLE;
    return true;
}

bool CoreDiscoveryStateMachine::ConsumePendingStart()
{
    std::lock_guard<std::mutex> lock(mutex_);
    bool pending = pendingStart_;
    pendingStart_ = false;
    return pending;
}

CoreDiscoveryStateMachine::State CoreDiscoveryStateMachine::Stop()
{
    std::lock_guard<std::mutex> lock(mutex_);
    State previous = state_;
    state_ = State::IDLE;
    pendingStart_ = false;
    return previous;
}

CoreDiscoveryStateMachine::State CoreDiscoveryStateMachine::GetState() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return state_;
}

bool CoreDiscoveryStateMachine::IsPendingStart() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return pendingStart_;
}

const char *CoreDiscoveryStateMachine::Name(State state)
{
    switch (state) {
        case State::IDLE:
            return "IDLE";
        case State::ACTIVE:
            return "ACTIVE";
        case State::CANCELLING:
            return "CANCELLING";
        default:
            return "UNKNOWN";
    }
}

} // namespace btif
} // namespace bluetooth
