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

#include <gtest/gtest.h>

#include "btif/core/core_discovery_manager.h"
#include "btif/core/core_discovery_state_machine.h"

using namespace testing::ext;
using namespace bluetooth::btif;

class CoreDiscoveryStateMachineTest : public testing::Test {};

HWTEST_F(CoreDiscoveryStateMachineTest, CoreDiscoveryStateMachine_InitialIdle_001, TestSize.Level1)
{
    CoreDiscoveryStateMachine machine;
    EXPECT_EQ(machine.GetState(), CoreDiscoveryStateMachine::State::IDLE);
    EXPECT_FALSE(machine.IsPendingStart());
    EXPECT_STREQ(CoreDiscoveryStateMachine::Name(CoreDiscoveryStateMachine::State::IDLE), "IDLE");
    EXPECT_STREQ(CoreDiscoveryStateMachine::Name(CoreDiscoveryStateMachine::State::ACTIVE), "ACTIVE");
    EXPECT_STREQ(CoreDiscoveryStateMachine::Name(CoreDiscoveryStateMachine::State::CANCELLING), "CANCELLING");
    EXPECT_STREQ(CoreDiscoveryStateMachine::Name(static_cast<CoreDiscoveryStateMachine::State>(0xFF)), "UNKNOWN");
    EXPECT_EQ(machine.Stop(), CoreDiscoveryStateMachine::State::IDLE);
}

HWTEST_F(CoreDiscoveryStateMachineTest, CoreDiscoveryStateMachine_StartAndDuplicate_002, TestSize.Level1)
{
    CoreDiscoveryStateMachine machine;
    auto first = machine.Start();
    EXPECT_TRUE(first.changed);
    EXPECT_FALSE(first.queued);
    EXPECT_EQ(machine.GetState(), CoreDiscoveryStateMachine::State::ACTIVE);

    auto duplicate = machine.Start();
    EXPECT_FALSE(duplicate.changed);
    EXPECT_FALSE(duplicate.queued);
    EXPECT_EQ(machine.GetState(), CoreDiscoveryStateMachine::State::ACTIVE);
}

HWTEST_F(CoreDiscoveryStateMachineTest, CoreDiscoveryStateMachine_CancelTransitions_003, TestSize.Level1)
{
    CoreDiscoveryStateMachine machine;
    EXPECT_FALSE(machine.Cancel());
    EXPECT_TRUE(machine.Start().changed);
    EXPECT_TRUE(machine.Cancel());
    EXPECT_EQ(machine.GetState(), CoreDiscoveryStateMachine::State::CANCELLING);
    EXPECT_FALSE(machine.Cancel());
}

HWTEST_F(CoreDiscoveryStateMachineTest, CoreDiscoveryStateMachine_QueuedStart_004, TestSize.Level1)
{
    CoreDiscoveryStateMachine machine;
    EXPECT_TRUE(machine.Start().changed);
    EXPECT_TRUE(machine.Cancel());

    auto queued = machine.Start();
    EXPECT_FALSE(queued.changed);
    EXPECT_TRUE(queued.queued);
    EXPECT_TRUE(machine.IsPendingStart());
    EXPECT_EQ(machine.GetState(), CoreDiscoveryStateMachine::State::CANCELLING);
}

HWTEST_F(CoreDiscoveryStateMachineTest, CoreDiscoveryStateMachine_CompleteTransitions_005, TestSize.Level1)
{
    CoreDiscoveryStateMachine machine;
    bool wasCancelling = true;
    EXPECT_FALSE(machine.Complete(wasCancelling));

    EXPECT_TRUE(machine.Start().changed);
    EXPECT_TRUE(machine.Complete(wasCancelling));
    EXPECT_FALSE(wasCancelling);
    EXPECT_EQ(machine.GetState(), CoreDiscoveryStateMachine::State::IDLE);

    EXPECT_TRUE(machine.Start().changed);
    EXPECT_TRUE(machine.Cancel());
    EXPECT_TRUE(machine.Complete(wasCancelling));
    EXPECT_TRUE(wasCancelling);
    EXPECT_EQ(machine.GetState(), CoreDiscoveryStateMachine::State::IDLE);
}

HWTEST_F(CoreDiscoveryStateMachineTest, CoreDiscoveryStateMachine_ConsumePendingOnce_006, TestSize.Level1)
{
    CoreDiscoveryStateMachine machine;
    EXPECT_TRUE(machine.Start().changed);
    EXPECT_TRUE(machine.Cancel());
    EXPECT_TRUE(machine.Start().queued);
    EXPECT_TRUE(machine.ConsumePendingStart());
    EXPECT_FALSE(machine.IsPendingStart());
    EXPECT_FALSE(machine.ConsumePendingStart());
}

HWTEST_F(CoreDiscoveryStateMachineTest, CoreDiscoveryStateMachine_StopResetsState_007, TestSize.Level1)
{
    CoreDiscoveryStateMachine machine;
    EXPECT_TRUE(machine.Start().changed);
    EXPECT_TRUE(machine.Cancel());
    EXPECT_TRUE(machine.Start().queued);
    EXPECT_EQ(machine.Stop(), CoreDiscoveryStateMachine::State::CANCELLING);
    EXPECT_EQ(machine.GetState(), CoreDiscoveryStateMachine::State::IDLE);
    EXPECT_FALSE(machine.IsPendingStart());
}

HWTEST_F(CoreDiscoveryStateMachineTest, CoreDiscoveryStateMachine_PairingDiscoveryLock_008, TestSize.Level1)
{
    CoreDiscoveryManager::SetPairingMode(true);
    EXPECT_TRUE(CoreDiscoveryManager::IsPairingMode());
    CoreDiscoveryManager::SetPairingMode(false);
    EXPECT_FALSE(CoreDiscoveryManager::IsPairingMode());
}
