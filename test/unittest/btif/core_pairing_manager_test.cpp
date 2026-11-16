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

#include "btif/core/core_pairing_manager.h"

#include <gtest/gtest.h>

#include <cstdio>
#include <string>

#include "btif/common/btif_constants.h"

using namespace testing::ext;

namespace {
const char *STORE_PATH = bluetooth::btif::kDeviceStorePath;

std::string BackupPath()
{
    return std::string(STORE_PATH) + ".unittest.bak";
}

void BackupStore()
{
    std::remove(BackupPath().c_str());
    (void)std::rename(STORE_PATH, BackupPath().c_str());
}

void RestoreStore()
{
    std::remove(STORE_PATH);
    (void)std::rename(BackupPath().c_str(), STORE_PATH);
}
}  // namespace

class CorePairingManagerTest : public testing::Test {
   public:
    void SetUp() override
    {
        BackupStore();
        ::CorePairingManager::Cleanup();
    }

    void TearDown() override
    {
        ::CorePairingManager::Cleanup();
        RestoreStore();
    }
};

HWTEST_F(CorePairingManagerTest, CorePairingManager_CleanupIsIdempotent_001, TestSize.Level1)
{
    ::CorePairingManager::Cleanup();
    ::CorePairingManager::Cleanup();
    SUCCEED();
}

HWTEST_F(CorePairingManagerTest, CorePairingManager_EmptyAddressIsNotBonded_002, TestSize.Level1)
{
    RawAddress address = {};
    EXPECT_FALSE(::CorePairingManager::IsBonded(address));
}

HWTEST_F(CorePairingManagerTest, CorePairingManager_NullParameters_003, TestSize.Level1)
{
    bt_pin_code_t pin = {};
    EXPECT_EQ(::CorePairingManager::CreateBond(nullptr, 0), BT_STATUS_PARM_INVALID);
    EXPECT_EQ(::CorePairingManager::RemoveBond(nullptr), BT_STATUS_PARM_INVALID);
    EXPECT_EQ(::CorePairingManager::CancelBond(nullptr), BT_STATUS_PARM_INVALID);
    EXPECT_EQ(::CorePairingManager::PinReply(nullptr, 0, 0, &pin), BT_STATUS_PARM_INVALID);
    EXPECT_EQ(
        ::CorePairingManager::SspReply(nullptr, BT_SSP_VARIANT_PASSKEY_CONFIRMATION, 0, 0), BT_STATUS_PARM_INVALID);
    EXPECT_EQ(::CorePairingManager::GetBondedDevices(nullptr, 1), 0);
}
