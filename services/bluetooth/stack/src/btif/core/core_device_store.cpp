/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "btif/core/core_device_store.h"

#include <fcntl.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include "btif/common/btif_constants.h"
#include "gap_if.h"
#include "log.h"
#include "securec.h"

namespace bluetooth {
namespace btif {
namespace {

constexpr char DEVICE_STORE_MAGIC[4] = {'B', 'T', 'D', 'S'};
constexpr uint8_t DEVICE_STORE_VERSION_V1 = 1;
constexpr uint8_t DEVICE_STORE_VERSION = 2;
constexpr size_t DEVICE_STORE_HEADER_SIZE = 4 + 1 + 2;
constexpr uint8_t FLAG_PAIRED = 0x01;
constexpr uint8_t FLAG_BOND = 0x02;
constexpr uint8_t FLAG_NAME = 0x04;
constexpr uint8_t FLAG_UUIDS = 0x08;

}  // namespace
}  // namespace btif
}  // namespace bluetooth

struct StoreState {
    std::mutex mutex;
    std::map<std::string, bluetooth::btif::DeviceStoreRecord> records;
    bool loaded = false;
};

struct CoreDeviceStore::State {
    StoreState store;
    std::mutex remoteNameMutex;
    std::map<std::string, std::string> remoteNames;
    bool remoteNamesLoaded = false;
};

CoreDeviceStore::CoreDeviceStore() : state_(std::make_unique<State>())
{
}

CoreDeviceStore::~CoreDeviceStore() = default;

CoreDeviceStore &::CoreDeviceStore::GetInstance()
{
    static CoreDeviceStore instance;
    return instance;
}

CoreDeviceStore::State &CoreDeviceStore::GetState()
{
    return *state_;
}

namespace bluetooth {
namespace btif {
namespace {

std::string AddressKey(const std::array<uint8_t, BTIF_DEVICE_ADDRESS_SIZE> &address)
{
    return std::string(reinterpret_cast<const char *>(address.data()), address.size());
}

bool ReadAll(const char *path, std::vector<uint8_t> &data)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    data.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    return true;
}

void SyncPath(const char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd >= 0) {
        (void)fsync(fd);
        close(fd);
    }

    std::string dir(path);
    size_t separator = dir.find_last_of('/');
    if (separator != std::string::npos) {
        dir.resize(separator);
        int dirFd = open(dir.c_str(), O_RDONLY);
        if (dirFd >= 0) {
            (void)fsync(dirFd);
            close(dirFd);
        }
    }
}

void Put16(std::vector<uint8_t> &data, uint16_t value)
{
    data.push_back(static_cast<uint8_t>(value & bluetooth::btif::kByteMask));
    data.push_back(static_cast<uint8_t>((value >> bluetooth::btif::kBitsPerByte) & bluetooth::btif::kByteMask));
}

uint16_t Get16(const std::vector<uint8_t> &data, size_t offset)
{
    return static_cast<uint16_t>(data[offset]) |
           (static_cast<uint16_t>(data[offset + 1]) << bluetooth::btif::kBitsPerByte);
}

std::vector<uint8_t> Encode(const std::vector<DeviceStoreRecord> &records)
{
    std::vector<uint8_t> data;
    data.insert(data.end(), DEVICE_STORE_MAGIC, DEVICE_STORE_MAGIC + sizeof(DEVICE_STORE_MAGIC));
    data.push_back(DEVICE_STORE_VERSION);
    Put16(data, static_cast<uint16_t>(records.size()));

    for (const auto &record : records) {
        data.insert(data.end(), record.address.begin(), record.address.end());
        uint8_t flags = 0;
        if (record.paired) {
            flags |= FLAG_PAIRED;
        }
        if (record.hasBond) {
            flags |= FLAG_BOND;
        }
        if (!record.name.empty()) {
            flags |= FLAG_NAME;
        }
        if (!record.uuids.empty()) {
            flags |= FLAG_UUIDS;
        }
        data.push_back(flags);
        data.push_back(record.transport);
        data.push_back(record.addressType);
        data.push_back(record.keyType);
        if (record.hasBond) {
            data.insert(data.end(), record.linkKey.begin(), record.linkKey.end());
        }

        if ((flags & FLAG_NAME) != 0) {
            uint8_t nameLength = static_cast<uint8_t>(std::min(record.name.size(), BTIF_DEVICE_MAX_NAME));
            data.push_back(nameLength);
            data.insert(data.end(), record.name.begin(), record.name.begin() + nameLength);
        }

        uint8_t uuidCount = static_cast<uint8_t>(std::min(record.uuids.size(), BTIF_DEVICE_MAX_UUIDS));
        data.push_back(uuidCount);
        for (size_t index = 0; index < uuidCount; index++) {
            data.insert(data.end(), record.uuids[index].begin(), record.uuids[index].end());
        }
    }
    return data;
}

bool DecodeRecordHeader(const std::vector<uint8_t> &data, size_t &offset, uint8_t &flags, DeviceStoreRecord &record)
{
    if (offset + BTIF_DEVICE_ADDRESS_SIZE + bluetooth::btif::kUint32Size > data.size()) {
        return false;
    }
    if (memcpy_s(record.address.data(), record.address.size(), data.data() + offset, BTIF_DEVICE_ADDRESS_SIZE) != EOK) {
        return false;
    }
    offset += BTIF_DEVICE_ADDRESS_SIZE;
    flags = data[offset++];
    record.paired = (flags & FLAG_PAIRED) != 0;
    record.hasBond = (flags & FLAG_BOND) != 0;
    record.transport = data[offset++];
    record.addressType = data[offset++];
    record.keyType = data[offset++];
    if (record.hasBond) {
        if (offset + BTIF_DEVICE_LINK_KEY_SIZE > data.size()) {
            return false;
        }
        if (memcpy_s(record.linkKey.data(), record.linkKey.size(), data.data() + offset, BTIF_DEVICE_LINK_KEY_SIZE) !=
            EOK) {
            return false;
        }
        offset += BTIF_DEVICE_LINK_KEY_SIZE;
    }
    return true;
}

bool DecodeRecordBody(
    const std::vector<uint8_t> &data, uint8_t version, uint8_t flags, size_t &offset, DeviceStoreRecord &record)
{
    if (version == DEVICE_STORE_VERSION_V1 || (flags & FLAG_NAME) != 0) {
        if (offset >= data.size()) {
            return false;
        }
        uint8_t nameLength = data[offset++];
        if (nameLength > BTIF_DEVICE_MAX_NAME || offset + nameLength > data.size()) {
            return false;
        }
        record.name.assign(reinterpret_cast<const char *>(data.data() + offset), nameLength);
        offset += nameLength;
    }

    if (offset >= data.size()) {
        return false;
    }
    uint8_t uuidCount = data[offset++];
    if (uuidCount > BTIF_DEVICE_MAX_UUIDS ||
        offset + static_cast<size_t>(uuidCount) * BTIF_DEVICE_UUID_SIZE > data.size()) {
        return false;
    }
    for (size_t uuidIndex = 0; uuidIndex < uuidCount; uuidIndex++) {
        std::array<uint8_t, BTIF_DEVICE_UUID_SIZE> uuid = {};
        if (memcpy_s(uuid.data(), uuid.size(), data.data() + offset, BTIF_DEVICE_UUID_SIZE) != EOK) {
            return false;
        }
        offset += BTIF_DEVICE_UUID_SIZE;
        record.uuids.push_back(uuid);
    }
    return true;
}

bool Decode(const std::vector<uint8_t> &data, std::vector<DeviceStoreRecord> &records)
{
    if (data.size() < DEVICE_STORE_HEADER_SIZE ||
        std::memcmp(data.data(), DEVICE_STORE_MAGIC, sizeof(DEVICE_STORE_MAGIC)) != 0) {
        return false;
    }
    const uint8_t version = data[4];
    if (version != DEVICE_STORE_VERSION_V1 && version != DEVICE_STORE_VERSION) {
        return false;
    }

    size_t count = Get16(data, 5);
    if (count > BTIF_DEVICE_MAX_RECORDS) {
        return false;
    }

    size_t offset = DEVICE_STORE_HEADER_SIZE;
    for (size_t index = 0; index < count; index++) {
        DeviceStoreRecord record;
        uint8_t flags = 0;
        if (!DecodeRecordHeader(data, offset, flags, record)) {
            return false;
        }
        if (!DecodeRecordBody(data, version, flags, offset, record)) {
            return false;
        }
        records.push_back(record);
    }
    return offset == data.size();
}

std::vector<DeviceStoreRecord> SortedRecords(const std::map<std::string, DeviceStoreRecord> &records)
{
    std::vector<DeviceStoreRecord> result;
    result.reserve(records.size());
    for (const auto &entry : records) {
        result.push_back(entry.second);
    }
    std::sort(result.begin(), result.end(), [](const DeviceStoreRecord &left, const DeviceStoreRecord &right) {
        return AddressKey(left.address) < AddressKey(right.address);
    });
    return result;
}

bool SaveLocked()
{
    std::vector<DeviceStoreRecord> records = SortedRecords(::CoreDeviceStore::GetInstance().GetState().store.records);
    if (records.empty()) {
        (void)std::remove(bluetooth::btif::kDeviceStoreTmpPath);
        (void)std::remove(bluetooth::btif::kDeviceStorePath);
        return true;
    }

    std::vector<uint8_t> data = Encode(records);
    std::ofstream file(bluetooth::btif::kDeviceStoreTmpPath, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        HILOGE("open device store temp failed path=%{public}s", bluetooth::btif::kDeviceStoreTmpPath);
        return false;
    }
    file.write(reinterpret_cast<const char *>(data.data()), static_cast<std::streamsize>(data.size()));
    file.close();
    if (std::rename(bluetooth::btif::kDeviceStoreTmpPath, bluetooth::btif::kDeviceStorePath) != 0) {
        HILOGE("rename device store failed errno=%{public}d", errno);
        return false;
    }
    SyncPath(bluetooth::btif::kDeviceStorePath);
    HILOGI("device store saved count=%{public}d", static_cast<int>(records.size()));
    return true;
}

void ReplaceLocked(std::vector<DeviceStoreRecord> records, NameUpdatePolicy namePolicy)
{
    for (auto &record : records) {
        auto existing = ::CoreDeviceStore::GetInstance().GetState().store.records.find(AddressKey(record.address));
        if (existing == ::CoreDeviceStore::GetInstance().GetState().store.records.end()) {
            continue;
        }
        if (namePolicy == NameUpdatePolicy::kPreserve || record.name.empty()) {
            record.name = existing->second.name;
        }
    }

    ::CoreDeviceStore::GetInstance().GetState().store.records.clear();
    for (const auto &record : records) {
        if (::CoreDeviceStore::GetInstance().GetState().store.records.size() >= BTIF_DEVICE_MAX_RECORDS) {
            break;
        }
        ::CoreDeviceStore::GetInstance().GetState().store.records[AddressKey(record.address)] = record;
    }
}

void LoadLocked()
{
    if (::CoreDeviceStore::GetInstance().GetState().store.loaded) {
        return;
    }
    ::CoreDeviceStore::GetInstance().GetState().store.loaded = true;

    std::vector<uint8_t> data;
    if (ReadAll(bluetooth::btif::kDeviceStorePath, data)) {
        std::vector<DeviceStoreRecord> records;
        if (!Decode(data, records)) {
            HILOGE("decode device store failed size=%{public}d", static_cast<int>(data.size()));
            return;
        }
        for (const auto &record : records) {
            ::CoreDeviceStore::GetInstance().GetState().store.records[AddressKey(record.address)] = record;
        }
        HILOGI("device store loaded count=%{public}d", static_cast<int>(records.size()));
        return;
    }
}

}  // namespace

static void CoreDeviceStoreLoad()
{
    std::lock_guard<std::mutex> lock(::CoreDeviceStore::GetInstance().GetState().store.mutex);
    LoadLocked();
}

static std::vector<DeviceStoreRecord> CoreDeviceStoreSnapshot()
{
    CoreDeviceStoreLoad();
    std::lock_guard<std::mutex> lock(::CoreDeviceStore::GetInstance().GetState().store.mutex);
    return SortedRecords(::CoreDeviceStore::GetInstance().GetState().store.records);
}

static bool CoreDeviceStoreReplace(const std::vector<DeviceStoreRecord> &records, NameUpdatePolicy namePolicy)
{
    CoreDeviceStoreLoad();
    std::lock_guard<std::mutex> lock(::CoreDeviceStore::GetInstance().GetState().store.mutex);
    ReplaceLocked(records, namePolicy);
    return SaveLocked();
}

static bool CoreDeviceStoreUpdate(
    const std::function<void(std::vector<DeviceStoreRecord> &)> &updater, NameUpdatePolicy namePolicy)
{
    if (!updater) {
        return false;
    }
    CoreDeviceStoreLoad();
    std::lock_guard<std::mutex> lock(::CoreDeviceStore::GetInstance().GetState().store.mutex);
    std::vector<DeviceStoreRecord> records = SortedRecords(::CoreDeviceStore::GetInstance().GetState().store.records);
    updater(records);
    ReplaceLocked(std::move(records), namePolicy);
    return SaveLocked();
}

static void CoreDeviceStoreRemove(const std::array<uint8_t, BTIF_DEVICE_ADDRESS_SIZE> &address)
{
    CoreDeviceStoreLoad();
    std::lock_guard<std::mutex> lock(::CoreDeviceStore::GetInstance().GetState().store.mutex);
    if (::CoreDeviceStore::GetInstance().GetState().store.records.erase(AddressKey(address)) > 0) {
        (void)SaveLocked();
    }
}

}  // namespace btif
}  // namespace bluetooth

namespace {

void LoadRemoteNamesLocked()
{
    if (::CoreDeviceStore::GetInstance().GetState().remoteNamesLoaded) {
        return;
    }
    ::CoreDeviceStore::GetInstance().GetState().remoteNamesLoaded = true;
    auto records = bluetooth::btif::CoreDeviceStoreSnapshot();
    int count = 0;
    for (const auto &record : records) {
        if (record.name.empty()) {
            continue;
        }
        std::string key(reinterpret_cast<const char *>(record.address.data()), RawAddress::kLength);
        ::CoreDeviceStore::GetInstance().GetState().remoteNames[key] = record.name;
        count++;
    }
    HILOGI("remote name store loaded count=%{public}d source=unified", count);
}

void SaveRemoteNames(const std::map<std::string, std::string> &names)
{
    (void)bluetooth::btif::CoreDeviceStoreUpdate(
        [names](std::vector<bluetooth::btif::DeviceStoreRecord> &records) {
            std::set<std::string> existing;
            for (auto &record : records) {
                std::string key(reinterpret_cast<const char *>(record.address.data()), RawAddress::kLength);
                existing.insert(key);
                auto iter = names.find(key);
                if (iter != names.end()) {
                    record.name = iter->second;
                }
            }
            for (const auto &entry : names) {
                if (existing.find(entry.first) != existing.end() || entry.first.size() != RawAddress::kLength) {
                    continue;
                }
                bluetooth::btif::DeviceStoreRecord record;
                if (memcpy_s(record.address.data(), record.address.size(), entry.first.data(), RawAddress::kLength) !=
                    EOK) {
                    HILOGE("copy device store address failed");
                    continue;
                }
                record.name = entry.second;
                records.push_back(record);
            }
        },
        bluetooth::btif::NameUpdatePolicy::kUpdateIfNonEmpty);
}

}  // namespace

static void CoreForgetRemoteName(const RawAddress &address)
{
    std::string key(reinterpret_cast<const char *>(address.address), RawAddress::kLength);
    std::lock_guard<std::mutex> lock(::CoreDeviceStore::GetInstance().GetState().remoteNameMutex);
    LoadRemoteNamesLocked();
    ::CoreDeviceStore::GetInstance().GetState().remoteNames.erase(key);
}

static void CoreRememberRemoteName(const RawAddress &address, const uint8_t *name, size_t length)
{
    if (name == nullptr || length == 0) {
        return;
    }
    size_t actualLength = 0;
    while (actualLength < length && actualLength < GAP_NAME_SIZE_MAX && name[actualLength] != 0) {
        actualLength++;
    }
    if (actualLength == 0 || actualLength > GAP_NAME_SIZE_MAX) {
        return;
    }
    std::string key(reinterpret_cast<const char *>(address.address), RawAddress::kLength);
    std::string value(reinterpret_cast<const char *>(name), actualLength);
    std::map<std::string, std::string> names;
    {
        std::lock_guard<std::mutex> lock(::CoreDeviceStore::GetInstance().GetState().remoteNameMutex);
        LoadRemoteNamesLocked();
        auto iter = ::CoreDeviceStore::GetInstance().GetState().remoteNames.find(key);
        if (iter != ::CoreDeviceStore::GetInstance().GetState().remoteNames.end() && iter->second == value) {
            return;
        }
        ::CoreDeviceStore::GetInstance().GetState().remoteNames[key] = value;
        names = ::CoreDeviceStore::GetInstance().GetState().remoteNames;
    }
    SaveRemoteNames(names);
}

static bool CoreGetRemoteName(const RawAddress &address, std::string &name)
{
    std::string key(reinterpret_cast<const char *>(address.address), RawAddress::kLength);
    std::lock_guard<std::mutex> lock(::CoreDeviceStore::GetInstance().GetState().remoteNameMutex);
    LoadRemoteNamesLocked();
    auto iter = ::CoreDeviceStore::GetInstance().GetState().remoteNames.find(key);
    if (iter == ::CoreDeviceStore::GetInstance().GetState().remoteNames.end() || iter->second.empty()) {
        return false;
    }
    name = iter->second;
    return true;
}
using bluetooth::btif::CoreDeviceStoreLoad;
using bluetooth::btif::CoreDeviceStoreRemove;
using bluetooth::btif::CoreDeviceStoreReplace;
using bluetooth::btif::CoreDeviceStoreSnapshot;
using bluetooth::btif::CoreDeviceStoreUpdate;

void CoreDeviceStore::Load()
{
    CoreDeviceStoreLoad();
}
std::vector<DeviceStoreRecord> CoreDeviceStore::Snapshot()
{
    return CoreDeviceStoreSnapshot();
}
bool CoreDeviceStore::Replace(const std::vector<DeviceStoreRecord> &records, NameUpdatePolicy namePolicy)
{
    return CoreDeviceStoreReplace(records, namePolicy);
}
bool CoreDeviceStore::Update(
    const std::function<void(std::vector<DeviceStoreRecord> &)> &updater, NameUpdatePolicy namePolicy)
{
    return CoreDeviceStoreUpdate(updater, namePolicy);
}
void CoreDeviceStore::Remove(const std::array<uint8_t, BTIF_DEVICE_ADDRESS_SIZE> &address)
{
    CoreDeviceStoreRemove(address);
}
void CoreDeviceStore::RememberName(const RawAddress &address, const uint8_t *name, size_t length)
{
    CoreRememberRemoteName(address, name, length);
}
bool CoreDeviceStore::GetName(const RawAddress &address, std::string &name)
{
    return CoreGetRemoteName(address, name);
}
void CoreDeviceStore::ForgetName(const RawAddress &address)
{
    CoreForgetRemoteName(address);
}
