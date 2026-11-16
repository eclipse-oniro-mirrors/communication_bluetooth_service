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

#ifndef CORE_DEVICE_H
#define CORE_DEVICE_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "types/raw_address.h"

namespace bluetooth {
namespace btif {

constexpr size_t BTIF_DEVICE_ADDRESS_SIZE = 6;
constexpr size_t BTIF_DEVICE_LINK_KEY_SIZE = 16;
constexpr size_t BTIF_DEVICE_UUID_SIZE = 16;
constexpr size_t BTIF_DEVICE_MAX_RECORDS = 128;
constexpr size_t BTIF_DEVICE_MAX_UUIDS = 32;
constexpr size_t BTIF_DEVICE_MAX_NAME = 248;

struct DeviceStoreRecord {
    std::array<uint8_t, BTIF_DEVICE_ADDRESS_SIZE> address = {};
    bool paired = false;
    bool hasBond = false;
    uint8_t keyType = 0;
    std::array<uint8_t, BTIF_DEVICE_LINK_KEY_SIZE> linkKey = {};
    std::string name;
    std::vector<std::array<uint8_t, BTIF_DEVICE_UUID_SIZE>> uuids;
    uint8_t transport = 0;
    uint8_t addressType = 0;
};

enum class NameUpdatePolicy : uint8_t {
    kPreserve,
    kUpdateIfNonEmpty,
};

} // namespace btif
} // namespace bluetooth

// Runtime device name view.


using bluetooth::btif::DeviceStoreRecord;
using bluetooth::btif::NameUpdatePolicy;
using bluetooth::btif::BTIF_DEVICE_ADDRESS_SIZE;

class CoreDeviceStore final {
public:
    struct State;

    CoreDeviceStore();
    ~CoreDeviceStore();
    static CoreDeviceStore &GetInstance();
    State &GetState();

    static void Load();
    static std::vector<DeviceStoreRecord> Snapshot();
    static bool Replace(const std::vector<DeviceStoreRecord> &records,
        NameUpdatePolicy namePolicy = NameUpdatePolicy::kPreserve);
    static bool Update(const std::function<void(std::vector<DeviceStoreRecord> &)> &updater,
        NameUpdatePolicy namePolicy = NameUpdatePolicy::kPreserve);
    static void Remove(const std::array<uint8_t, BTIF_DEVICE_ADDRESS_SIZE> &address);
    static void RememberName(const RawAddress &address, const uint8_t *name, size_t length);
    static bool GetName(const RawAddress &address, std::string &name);
    static void ForgetName(const RawAddress &address);

private:
    CoreDeviceStore(const CoreDeviceStore &) = delete;
    CoreDeviceStore &operator=(const CoreDeviceStore &) = delete;
    std::unique_ptr<State> state_;
};

#endif // CORE_DEVICE_H
