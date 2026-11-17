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

#include "btif/core/core_ble_scan_policy.h"

using namespace testing::ext;
using namespace bluetooth::btif;

class CoreBleScanPolicyTest : public testing::Test {
protected:
    CoreBleScanPolicy policy_ { 0x1000, 0x0800, 0x1000 };
};

HWTEST_F(CoreBleScanPolicyTest, CoreBleScanPolicy_NoCaller_001, TestSize.Level1)
{
    EXPECT_FALSE(policy_.NeedScan());
    EXPECT_EQ(policy_.GetCallerMask(), 0);
    auto param = policy_.GetOptimalParam();
    EXPECT_EQ(param.interval, 0x1000);
    EXPECT_EQ(param.window, 0x1000);
    EXPECT_TRUE(param.filterDuplicates);
}

HWTEST_F(CoreBleScanPolicyTest, CoreBleScanPolicy_GattOnly_002, TestSize.Level1)
{
    policy_.AddCaller(CoreBleScanPolicy::Caller::GATT);
    EXPECT_TRUE(policy_.NeedScan());
    EXPECT_TRUE(policy_.HasCaller(CoreBleScanPolicy::Caller::GATT));
    policy_.SetGattParam({ 0x0400, 0x0200, true, true });
    auto param = policy_.GetOptimalParam();
    EXPECT_EQ(param.interval, 0x0400);
    EXPECT_EQ(param.window, 0x0200);
    EXPECT_TRUE(param.filterDuplicates);
}

HWTEST_F(CoreBleScanPolicyTest, CoreBleScanPolicy_DiscoveryOnly_003, TestSize.Level1)
{
    policy_.AddCaller(CoreBleScanPolicy::Caller::DISCOVERY);
    policy_.SetDiscoveryParam({ 0x0300, 0x0180, false, true });
    auto param = policy_.GetOptimalParam();
    EXPECT_EQ(param.interval, 0x0300);
    EXPECT_EQ(param.window, 0x0180);
    EXPECT_FALSE(param.filterDuplicates);
}

HWTEST_F(CoreBleScanPolicyTest, CoreBleScanPolicy_CombineCallers_004, TestSize.Level1)
{
    policy_.AddCaller(CoreBleScanPolicy::Caller::GATT);
    policy_.AddCaller(CoreBleScanPolicy::Caller::DISCOVERY);
    policy_.SetGattParam({ 0x0400, 0x0200, true, true });
    policy_.SetDiscoveryParam({ 0x0300, 0x0280, false, true });
    auto param = policy_.GetOptimalParam();
    EXPECT_EQ(param.interval, 0x0300);
    EXPECT_EQ(param.window, 0x0280);
    EXPECT_FALSE(param.filterDuplicates);
}

HWTEST_F(CoreBleScanPolicyTest, CoreBleScanPolicy_FrameworkUsesGattParam_005, TestSize.Level1)
{
    policy_.AddCaller(CoreBleScanPolicy::Caller::FRAMEWORK);
    policy_.SetGattParam({ 0x0500, 0x0200, false, true });
    auto param = policy_.GetOptimalParam();
    EXPECT_EQ(param.interval, 0x0500);
    EXPECT_EQ(param.window, 0x0200);
    EXPECT_FALSE(param.filterDuplicates);
}

HWTEST_F(CoreBleScanPolicyTest, CoreBleScanPolicy_RemoveOneCaller_006, TestSize.Level1)
{
    policy_.AddCaller(CoreBleScanPolicy::Caller::GATT);
    policy_.AddCaller(CoreBleScanPolicy::Caller::DISCOVERY);
    policy_.RemoveCaller(CoreBleScanPolicy::Caller::DISCOVERY);
    EXPECT_TRUE(policy_.NeedScan());
    EXPECT_TRUE(policy_.HasCaller(CoreBleScanPolicy::Caller::GATT));
    EXPECT_FALSE(policy_.HasCaller(CoreBleScanPolicy::Caller::DISCOVERY));
    policy_.RemoveCaller(CoreBleScanPolicy::Caller::GATT);
    EXPECT_FALSE(policy_.NeedScan());
}

HWTEST_F(CoreBleScanPolicyTest, CoreBleScanPolicy_InvalidGattFallsBack_007, TestSize.Level1)
{
    policy_.AddCaller(CoreBleScanPolicy::Caller::GATT);
    policy_.SetGattParam({ 0x0200, 0x0100, true, false });
    auto param = policy_.GetOptimalParam();
    EXPECT_EQ(param.interval, 0x1000);
    EXPECT_EQ(param.window, 0x1000);
    EXPECT_TRUE(param.filterDuplicates);
}

HWTEST_F(CoreBleScanPolicyTest, CoreBleScanPolicy_ClampWindow_008, TestSize.Level1)
{
    policy_.AddCaller(CoreBleScanPolicy::Caller::GATT);
    policy_.SetGattParam({ 0x0200, 0x0400, true, true });
    auto param = policy_.GetOptimalParam();
    EXPECT_EQ(param.interval, 0x0200);
    EXPECT_EQ(param.window, 0x0200);
}
