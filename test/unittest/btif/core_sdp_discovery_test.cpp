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

#include "btif/core/core_sdp_discovery.h"

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

class CoreSdpDiscoveryTest : public testing::Test {
   public:
    void SetUp() override
    {
        BackupStore();
        ::CoreSdpDiscovery::Cleanup();
    }

    void TearDown() override
    {
        ::CoreSdpDiscovery::Cleanup();
        RestoreStore();
    }
};

HWTEST_F(CoreSdpDiscoveryTest, CoreSdpDiscovery_CleanupIsIdempotent_001, TestSize.Level1)
{
    ::CoreSdpDiscovery::Cleanup();
    ::CoreSdpDiscovery::Cleanup();
    SUCCEED();
}

HWTEST_F(CoreSdpDiscoveryTest, CoreSdpDiscovery_UnpairedEmptyAddress_002, TestSize.Level1)
{
    RawAddress address = {};
    ::CoreSdpDiscovery::OnUnpaired(address);
    SUCCEED();
}
