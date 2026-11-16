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

static bt_status_t BtifA2dpSourceInit(btav_source_callbacks_t* callbacks, int max_connected_audio_devices,
    const std::vector<btav_a2dp_codec_config_t>& codec_priorities,
    const std::vector<btav_a2dp_codec_config_t>& offloading_preference)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifA2dpSourceConnect(const RawAddress& bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifA2dpSourceDisconnect(const RawAddress& bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifA2dpSourceSetSilenceDevice(const RawAddress& bd_addr, bool silence)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifA2dpSourceSetActiveDevice(const RawAddress& bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifA2dpSourceConfigCodec(const RawAddress& bd_addr,
    std::vector<btav_a2dp_codec_config_t> codec_preferences)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static void BtifA2dpSourceCleanup(void)
{
    // Stub: the stack API for this profile is not wired up yet.
}

} // namespace

const void *ProfileInterfaces::GetA2dpSource()
{
    static const btav_source_interface_t table = {
        sizeof(btav_source_interface_t),
        BtifA2dpSourceInit,
        BtifA2dpSourceConnect,
        BtifA2dpSourceDisconnect,
        BtifA2dpSourceSetSilenceDevice,
        BtifA2dpSourceSetActiveDevice,
        BtifA2dpSourceConfigCodec,
        BtifA2dpSourceCleanup,
    };
    return &table;
}
