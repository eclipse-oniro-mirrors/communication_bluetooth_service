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

#include "hci/hci_tx_gate.h"

using namespace testing::ext;

class HciTxGateTest : public testing::Test {
protected:
    HciTxGate gate_ = { false, false };
};

HWTEST_F(HciTxGateTest, HciTxGate_InitialCanSend_001, TestSize.Level1)
{
    EXPECT_TRUE(HciTxGateCanSend(&gate_));
    EXPECT_FALSE(HciTxGateIsInFlight(&gate_));
    EXPECT_FALSE(HciTxGateIsStopped(&gate_));
}

HWTEST_F(HciTxGateTest, HciTxGate_SingleInFlight_002, TestSize.Level1)
{
    EXPECT_TRUE(HciTxGateTryBegin(&gate_));
    EXPECT_TRUE(HciTxGateIsInFlight(&gate_));
    EXPECT_FALSE(HciTxGateCanSend(&gate_));
    EXPECT_FALSE(HciTxGateTryBegin(&gate_));
}

HWTEST_F(HciTxGateTest, HciTxGate_CompleteAllowsNext_003, TestSize.Level1)
{
    ASSERT_TRUE(HciTxGateTryBegin(&gate_));
    HciTxGateComplete(&gate_);
    EXPECT_FALSE(HciTxGateIsInFlight(&gate_));
    EXPECT_TRUE(HciTxGateCanSend(&gate_));
    EXPECT_TRUE(HciTxGateTryBegin(&gate_));
}

HWTEST_F(HciTxGateTest, HciTxGate_StopBlocks_004, TestSize.Level1)
{
    HciTxGateStop(&gate_);
    EXPECT_TRUE(HciTxGateIsStopped(&gate_));
    EXPECT_FALSE(HciTxGateCanSend(&gate_));
    EXPECT_FALSE(HciTxGateTryBegin(&gate_));
}

HWTEST_F(HciTxGateTest, HciTxGate_ResetAfterStop_005, TestSize.Level1)
{
    HciTxGateStop(&gate_);
    HciTxGateReset(&gate_);
    EXPECT_FALSE(HciTxGateIsStopped(&gate_));
    EXPECT_FALSE(HciTxGateIsInFlight(&gate_));
    EXPECT_TRUE(HciTxGateCanSend(&gate_));
    EXPECT_TRUE(HciTxGateTryBegin(&gate_));
}

HWTEST_F(HciTxGateTest, HciTxGate_StopWhileInFlight_006, TestSize.Level1)
{
    ASSERT_TRUE(HciTxGateTryBegin(&gate_));
    HciTxGateStop(&gate_);
    HciTxGateComplete(&gate_);
    EXPECT_FALSE(HciTxGateIsInFlight(&gate_));
    EXPECT_TRUE(HciTxGateIsStopped(&gate_));
    EXPECT_FALSE(HciTxGateCanSend(&gate_));
}

HWTEST_F(HciTxGateTest, HciTxGate_NullSafe_007, TestSize.Level1)
{
    HciTxGateReset(nullptr);
    HciTxGateStop(nullptr);
    HciTxGateComplete(nullptr);
    EXPECT_FALSE(HciTxGateTryBegin(nullptr));
    EXPECT_FALSE(HciTxGateIsInFlight(nullptr));
    EXPECT_FALSE(HciTxGateIsStopped(nullptr));
    EXPECT_FALSE(HciTxGateCanSend(nullptr));
}
