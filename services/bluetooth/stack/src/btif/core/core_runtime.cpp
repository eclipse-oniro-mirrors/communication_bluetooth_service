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

#include "btif/core/core_runtime.h"

#include "log.h"

namespace bluetooth {
namespace btif {

CoreRuntime &CoreRuntime::GetInstance()
{
    static CoreRuntime instance;
    return instance;
}

CoreCallbackRegistry &CoreRuntime::Callbacks()
{
    return CoreCallbackRegistry::GetInstance();
}

CoreAdapterController &CoreRuntime::Adapter()
{
    return adapter_;
}

CoreBleScanner &CoreRuntime::Scanner()
{
    return scanner_;
}

CoreDeviceStore &CoreRuntime::Devices()
{
    return devices_;
}

CoreSdpDiscovery &CoreRuntime::Sdp()
{
    return sdp_;
}

CoreDiscoveryManager &CoreRuntime::Discovery()
{
    return discovery_;
}

CorePairingManager &CoreRuntime::Pairing()
{
    return pairing_;
}

bool CoreRuntime::Initialize()
{
    if (initialized_) {
        return true;
    }
    initialized_ = true;
    HILOGI("CoreRuntime initialized");
    return true;
}

void CoreRuntime::Shutdown()
{
    if (!initialized_) {
        return;
    }
    CoreCallbackRegistry::GetInstance().Unregister();
    initialized_ = false;
    HILOGI("CoreRuntime shutdown");
}

bool CoreRuntime::IsInitialized() const
{
    return initialized_;
}

} // namespace btif
} // namespace bluetooth
