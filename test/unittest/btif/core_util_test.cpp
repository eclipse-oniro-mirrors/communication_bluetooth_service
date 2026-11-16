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

#include "btif/core/core_util.h"

#include <gtest/gtest.h>

#include <array>

using namespace testing::ext;

class CoreUtilTest : public testing::Test {};

HWTEST_F(CoreUtilTest, CoreUtil_AddressRoundTrip_001, TestSize.Level1)
{
    RawAddress address(std::array<uint8_t, RawAddress::kLength>{0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x01});
    BtAddr btAddress = CoreUtil::ToBtAddr(address);
    RawAddress roundTrip = CoreUtil::ToRawAddress(btAddress);
    EXPECT_EQ(address, roundTrip);
}

HWTEST_F(CoreUtilTest, CoreUtil_AddressKeyStable_002, TestSize.Level1)
{
    RawAddress first(std::array<uint8_t, RawAddress::kLength>{0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x02});
    RawAddress second(std::array<uint8_t, RawAddress::kLength>{0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x02});
    std::string firstKey = CoreUtil::AddressKey(first);
    std::string secondKey = CoreUtil::AddressKey(second);
    EXPECT_EQ(firstKey.size(), RawAddress::kLength);
    EXPECT_EQ(firstKey, secondKey);
}

HWTEST_F(CoreUtilTest, CoreUtil_EmptyAddressRoundTrip_003, TestSize.Level1)
{
    RawAddress empty = {};
    BtAddr btAddress = CoreUtil::ToBtAddr(empty);
    RawAddress roundTrip = CoreUtil::ToRawAddress(btAddress);
    EXPECT_TRUE(roundTrip.IsEmpty());
}
