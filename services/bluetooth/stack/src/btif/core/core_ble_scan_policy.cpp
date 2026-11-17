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

#include "btif/core/core_ble_scan_policy.h"

#include <algorithm>

namespace bluetooth {
namespace btif {

CoreBleScanPolicy::CoreBleScanPolicy(uint16_t defaultInterval, uint16_t defaultWindow, uint16_t noCallerWindow)
    : defaultInterval_(defaultInterval), noCallerWindow_(noCallerWindow)
{
    gatt_ = { defaultInterval, defaultWindow, true, true };
    discovery_ = { defaultInterval, defaultWindow, false, true };
}

void CoreBleScanPolicy::AddCaller(Caller caller)
{
    callers_ |= static_cast<uint8_t>(caller);
}

void CoreBleScanPolicy::RemoveCaller(Caller caller)
{
    callers_ &= ~static_cast<uint8_t>(caller);
}

bool CoreBleScanPolicy::HasCaller(Caller caller) const
{
    return (callers_ & static_cast<uint8_t>(caller)) != 0;
}

bool CoreBleScanPolicy::NeedScan() const
{
    return callers_ != 0;
}

uint8_t CoreBleScanPolicy::GetCallerMask() const
{
    return callers_;
}

void CoreBleScanPolicy::SetGattParam(const Param &param)
{
    gatt_ = param;
}

void CoreBleScanPolicy::SetDiscoveryParam(const Param &param)
{
    discovery_ = param;
}

CoreBleScanPolicy::OptimalParam CoreBleScanPolicy::GetOptimalParam() const
{
    uint16_t interval = defaultInterval_;
    uint16_t window = 0;
    bool filterDuplicates = true;
    bool hasParam = false;

    if ((HasCaller(Caller::GATT) || HasCaller(Caller::FRAMEWORK)) && gatt_.valid) {
        interval = std::min(interval, gatt_.interval);
        window = std::max(window, gatt_.window);
        filterDuplicates = filterDuplicates && gatt_.filterDuplicates;
        hasParam = true;
    }
    if (HasCaller(Caller::DISCOVERY)) {
        interval = std::min(interval, discovery_.interval);
        window = std::max(window, discovery_.window);
        filterDuplicates = filterDuplicates && discovery_.filterDuplicates;
        hasParam = true;
    }
    if (!hasParam) {
        return { defaultInterval_, noCallerWindow_, true };
    }
    if (window > interval) {
        window = interval;
    }
    return { interval, window, filterDuplicates };
}

} // namespace btif
} // namespace bluetooth
