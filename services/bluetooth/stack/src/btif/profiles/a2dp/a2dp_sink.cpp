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

#include "btif/profiles/a2dp/a2dp.h"
#include "btif/profiles/profile_interfaces.h"

namespace {

static bt_status_t BtifA2dpSinkInit(btav_sink_callbacks_t* callbacks, int max_connected_audio_devices)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifA2dpSinkConnect(const RawAddress& bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifA2dpSinkDisconnect(const RawAddress& bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static void BtifA2dpSinkCleanup(void)
{
    // Stub: the stack API for this profile is not wired up yet.
}

static void BtifA2dpSinkSetAudioFocusState(int focusState)
{
    // Stub: the stack API for this profile is not wired up yet.
}

static void BtifA2dpSinkSetAudioTrackGain(float gain)
{
    // Stub: the stack API for this profile is not wired up yet.
}

static bt_status_t BtifA2dpSinkSetActiveDevice(const RawAddress& bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

} // namespace

const void *ProfileInterfaces::GetA2dpSink()
{
    static const btav_sink_interface_t table = {
        sizeof(btav_sink_interface_t),
        BtifA2dpSinkInit,
        BtifA2dpSinkConnect,
        BtifA2dpSinkDisconnect,
        BtifA2dpSinkCleanup,
        BtifA2dpSinkSetAudioFocusState,
        BtifA2dpSinkSetAudioTrackGain,
        BtifA2dpSinkSetActiveDevice,
    };
    return &table;
}
