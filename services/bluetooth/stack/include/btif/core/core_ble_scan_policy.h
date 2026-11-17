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

#ifndef CORE_BLE_SCAN_POLICY_H
#define CORE_BLE_SCAN_POLICY_H

#include <cstdint>

namespace bluetooth {
namespace btif {

// Pure caller and parameter aggregation for the BLE scanner. The scanner owns
// locking and GAP calls; this class only decides whether scanning is needed and
// how the GATT and discovery callers combine their parameters.
class CoreBleScanPolicy {
public:
    enum class Caller : uint8_t {
        NONE = 0,
        GATT = 1,
        DISCOVERY = 1 << 1,
        FRAMEWORK = 1 << 1 << 1,
    };

    struct Param {
        uint16_t interval = 0;
        uint16_t window = 0;
        bool filterDuplicates = true;
        bool valid = false;
    };

    struct OptimalParam {
        uint16_t interval;
        uint16_t window;
        bool filterDuplicates;
    };

    CoreBleScanPolicy(uint16_t defaultInterval, uint16_t defaultWindow, uint16_t noCallerWindow);

    void AddCaller(Caller caller);
    void RemoveCaller(Caller caller);
    bool HasCaller(Caller caller) const;
    bool NeedScan() const;
    uint8_t GetCallerMask() const;
    void SetGattParam(const Param &param);
    void SetDiscoveryParam(const Param &param);
    OptimalParam GetOptimalParam() const;

private:
    uint16_t defaultInterval_;
    uint16_t noCallerWindow_;
    uint8_t callers_ = 0;
    Param gatt_;
    Param discovery_;
};

} // namespace btif
} // namespace bluetooth

#endif // CORE_BLE_SCAN_POLICY_H
