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

#include "btif/core/core_device_store.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

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

std::vector<bluetooth::btif::DeviceStoreRecord> FindRecords(const std::array<uint8_t, RawAddress::kLength> &address)
{
    auto records = ::CoreDeviceStore::Snapshot();
    std::vector<bluetooth::btif::DeviceStoreRecord> matched;
    for (const auto &record : records) {
        if (record.address == address) {
            matched.push_back(record);
        }
    }
    return matched;
}
}  // namespace

class CoreDeviceStoreTest : public testing::Test {
   public:
    void SetUp() override
    {
        BackupStore();
        ::CoreDeviceStore::Replace({});
    }

    void TearDown() override
    {
        ::CoreDeviceStore::Replace({});
        RestoreStore();
    }
};

HWTEST_F(CoreDeviceStoreTest, CoreDeviceStore_UpdateKeepsFields_001, TestSize.Level1)
{
    bluetooth::btif::DeviceStoreRecord record;
    record.address = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x01};
    record.paired = true;
    record.name = "Unit Device";
    ASSERT_TRUE(::CoreDeviceStore::Replace({record}));

    const auto address = record.address;
    ASSERT_TRUE(::CoreDeviceStore::Update([address](std::vector<bluetooth::btif::DeviceStoreRecord> &records) {
        for (auto &item : records) {
            if (item.address == address) {
                item.hasBond = true;
                item.linkKey.fill(0x5A);
                item.uuids.push_back({0x01, 0x02});
            }
        }
    }));

    auto matched = FindRecords(address);
    ASSERT_EQ(matched.size(), 1U);
    EXPECT_EQ(matched[0].name, "Unit Device");
    EXPECT_TRUE(matched[0].hasBond);
    EXPECT_EQ(matched[0].uuids.size(), 1U);
}

HWTEST_F(CoreDeviceStoreTest, CoreDeviceStore_EmptyNameOmitsField_002, TestSize.Level1)
{
    bluetooth::btif::DeviceStoreRecord record;
    record.address = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x02};
    record.paired = true;
    record.hasBond = true;
    record.linkKey.fill(0x33);
    record.uuids.push_back({0x11, 0x22});
    record.name = "";
    ASSERT_TRUE(::CoreDeviceStore::Replace({record}));

    std::ifstream file(STORE_PATH, std::ios::binary);
    ASSERT_TRUE(file.is_open());
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    ASSERT_GE(data.size(), 12U);
    EXPECT_EQ(data[4], 2);

    size_t offset = 5 + 2 + 6;
    ASSERT_LT(offset, data.size());
    uint8_t flags = data[offset++];
    EXPECT_EQ(flags & 0x04, 0);
    offset += 3;   // transport, addressType, keyType
    offset += 16;  // linkKey
    ASSERT_LT(offset, data.size());
    EXPECT_EQ(data[offset], 1);  // uuidCount, not omitted nameLength
}

HWTEST_F(CoreDeviceStoreTest, CoreDeviceStore_NamePolicy_003, TestSize.Level1)
{
    bluetooth::btif::DeviceStoreRecord record;
    record.address = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x03};
    record.paired = true;
    record.name = "Original";
    ASSERT_TRUE(::CoreDeviceStore::Replace({record}));

    const auto address = record.address;
    ASSERT_TRUE(::CoreDeviceStore::Update(
        [address](std::vector<bluetooth::btif::DeviceStoreRecord> &records) {
            for (auto &item : records) {
                if (item.address == address) {
                    item.name = "Changed";
                }
            }
        },
        bluetooth::btif::NameUpdatePolicy::kPreserve));
    auto matched = FindRecords(address);
    ASSERT_EQ(matched.size(), 1U);
    EXPECT_EQ(matched[0].name, "Original");

    ASSERT_TRUE(::CoreDeviceStore::Update(
        [address](std::vector<bluetooth::btif::DeviceStoreRecord> &records) {
            for (auto &item : records) {
                if (item.address == address) {
                    item.name = "Changed";
                }
            }
        },
        bluetooth::btif::NameUpdatePolicy::kUpdateIfNonEmpty));
    matched = FindRecords(address);
    ASSERT_EQ(matched.size(), 1U);
    EXPECT_EQ(matched[0].name, "Changed");
}

HWTEST_F(CoreDeviceStoreTest, CoreDeviceStore_RememberGetForget_004, TestSize.Level1)
{
    RawAddress address(std::array<uint8_t, RawAddress::kLength>{0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x04});
    const char *name = "Remembered";
    ::CoreDeviceStore::RememberName(address, reinterpret_cast<const uint8_t *>(name), std::strlen(name));

    std::string stored;
    ASSERT_TRUE(::CoreDeviceStore::GetName(address, stored));
    EXPECT_EQ(stored, name);

    ::CoreDeviceStore::ForgetName(address);
    EXPECT_FALSE(::CoreDeviceStore::GetName(address, stored));
}

HWTEST_F(CoreDeviceStoreTest, CoreDeviceStore_RemoveDeletesRecord_005, TestSize.Level1)
{
    bluetooth::btif::DeviceStoreRecord record;
    record.address = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x05};
    record.paired = true;
    record.name = "ToRemove";
    ASSERT_TRUE(::CoreDeviceStore::Replace({record}));
    ASSERT_EQ(FindRecords(record.address).size(), 1U);

    ::CoreDeviceStore::Remove(record.address);
    EXPECT_TRUE(FindRecords(record.address).empty());
}

HWTEST_F(CoreDeviceStoreTest, CoreDeviceStore_UpdateEmptyNamePreserves_006, TestSize.Level1)
{
    bluetooth::btif::DeviceStoreRecord record;
    record.address = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x06};
    record.paired = true;
    record.name = "Keep";
    ASSERT_TRUE(::CoreDeviceStore::Replace({record}));

    ASSERT_TRUE(::CoreDeviceStore::Update(
        [](std::vector<bluetooth::btif::DeviceStoreRecord> &records) {
            for (auto &item : records) {
                item.name.clear();
            }
        },
        bluetooth::btif::NameUpdatePolicy::kUpdateIfNonEmpty));
    auto matched = FindRecords(record.address);
    ASSERT_EQ(matched.size(), 1U);
    EXPECT_EQ(matched[0].name, "Keep");
}

HWTEST_F(CoreDeviceStoreTest, CoreDeviceStore_NameOnlyRecordPersists_007, TestSize.Level1)
{
    RawAddress address(std::array<uint8_t, RawAddress::kLength>{0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x07});
    const char *name = "NameOnly";
    ::CoreDeviceStore::RememberName(address, reinterpret_cast<const uint8_t *>(name), std::strlen(name));

    auto matched = FindRecords(std::array<uint8_t, RawAddress::kLength>{0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x07});
    ASSERT_EQ(matched.size(), 1U);
    EXPECT_EQ(matched[0].name, name);
    EXPECT_FALSE(matched[0].paired);
}

HWTEST_F(CoreDeviceStoreTest, CoreDeviceStore_LoadIsIdempotent_008, TestSize.Level1)
{
    RawAddress address(std::array<uint8_t, RawAddress::kLength>{0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x08});
    const char *name = "Reloaded";
    ::CoreDeviceStore::RememberName(address, reinterpret_cast<const uint8_t *>(name), std::strlen(name));
    ::CoreDeviceStore::Load();
    ::CoreDeviceStore::Load();

    std::string stored;
    ASSERT_TRUE(::CoreDeviceStore::GetName(address, stored));
    EXPECT_EQ(stored, name);
}
