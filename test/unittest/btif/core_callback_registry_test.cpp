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

#include <array>

#include "btif/core/core_callbacks.h"

using namespace testing::ext;

namespace {
int g_adapterStateCount = 0;
int g_bondStateCount = 0;

void OnAdapterState(bt_state_t state)
{
    (void)state;
    g_adapterStateCount++;
}

void OnBondState(bt_status_t status, RawAddress *address, bt_bond_state_t state)
{
    (void)status;
    (void)address;
    (void)state;
    g_bondStateCount++;
}

CoreCallbacks g_callbacks = {
    OnAdapterState, nullptr, nullptr, nullptr, nullptr, OnBondState, nullptr, nullptr, nullptr,
};
}  // namespace

class CoreCallbackRegistryTest : public testing::Test {
   public:
    void SetUp() override
    {
        g_adapterStateCount = 0;
        g_bondStateCount = 0;
        ::CoreCallbackRegistry::Unregister();
        ::CoreCallbackRegistry::Register(&g_callbacks);
    }

    void TearDown() override
    {
        ::CoreCallbackRegistry::Unregister();
    }
};

HWTEST_F(CoreCallbackRegistryTest, CoreCallbackRegistry_RegisterNull_001, TestSize.Level1)
{
    ::CoreCallbackRegistry::Unregister();
    ::CoreCallbackRegistry::Register(nullptr);
    EXPECT_FALSE(::CoreCallbackRegistry::HasAdapterProperties());
    EXPECT_FALSE(::CoreCallbackRegistry::HasDeviceFound());
    EXPECT_FALSE(::CoreCallbackRegistry::HasSspRequest());
    EXPECT_FALSE(::CoreCallbackRegistry::HasPinRequest());
}

HWTEST_F(CoreCallbackRegistryTest, CoreCallbackRegistry_AdapterStateForwarded_002, TestSize.Level1)
{
    ::CoreCallbackRegistry::NotifyAdapterState(BT_STATE_ON);
    EXPECT_EQ(g_adapterStateCount, 1);
}

HWTEST_F(CoreCallbackRegistryTest, CoreCallbackRegistry_BondStateForwarded_003, TestSize.Level1)
{
    RawAddress address(std::array<uint8_t, RawAddress::kLength>{0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x01});
    EXPECT_TRUE(::CoreCallbackRegistry::NotifyBondState(BT_STATUS_SUCCESS, &address, BT_BOND_STATE_BONDED));
    EXPECT_EQ(g_bondStateCount, 1);
}

HWTEST_F(CoreCallbackRegistryTest, CoreCallbackRegistry_UnregisterStopsCallback_004, TestSize.Level1)
{
    ::CoreCallbackRegistry::Unregister();
    ::CoreCallbackRegistry::NotifyAdapterState(BT_STATE_ON);
    EXPECT_EQ(g_adapterStateCount, 0);
}
