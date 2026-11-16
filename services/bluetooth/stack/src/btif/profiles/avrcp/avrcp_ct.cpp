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

#include "btif/profiles/avrcp/avrcp.h"
#include "btif/profiles/profile_interfaces.h"

namespace {

static bt_status_t BtifAvrcpCtInit(btrc_ctrl_callbacks_t* callbacks)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpCtSendPassThroughCmd(const RawAddress& bd_addr, uint8_t key_code, uint8_t key_state)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpCtSendGroupNavigationCmd(const RawAddress& bd_addr, uint8_t key_code, uint8_t key_state)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpCtSetPlayerAppSettingCmd(const RawAddress& bd_addr, uint8_t num_attrib, uint8_t* attrib_ids,
    uint8_t* attrib_vals)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpCtPlayItemCmd(const RawAddress& bd_addr, uint8_t scope, uint8_t* uid, uint16_t uid_counter)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpCtGetCurrentMetadataCmd(const RawAddress& bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpCtGetPlaybackStateCmd(const RawAddress& bd_addr)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpCtGetNowPlayingListCmd(const RawAddress& bd_addr, uint32_t start, uint32_t end)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpCtGetFolderListCmd(const RawAddress& bd_addr, uint32_t start, uint32_t end)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpCtGetPlayerListCmd(const RawAddress& bd_addr, uint32_t start, uint32_t end)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpCtChangeFolderPathCmd(const RawAddress& bd_addr, uint8_t direction, uint8_t* uid)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpCtSetBrowsedPlayerCmd(const RawAddress& bd_addr, uint16_t player_id)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpCtSetAddressedPlayerCmd(const RawAddress& bd_addr, uint16_t player_id)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpCtSetVolumeRsp(const RawAddress& bd_addr, uint8_t abs_vol, uint8_t label)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpCtRegisterAbsVolRsp(const RawAddress& bd_addr, btrc_notification_type_t rsp_type,
    uint8_t abs_vol, uint8_t label)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static void BtifAvrcpCtCleanup(void)
{
    // Stub: the stack API for this profile is not wired up yet.
}

} // namespace

const void *ProfileInterfaces::GetAvrcpCt()
{
    static const btrc_ctrl_interface_t table = {
        sizeof(btrc_ctrl_interface_t),
        BtifAvrcpCtInit,
        BtifAvrcpCtSendPassThroughCmd,
        BtifAvrcpCtSendGroupNavigationCmd,
        BtifAvrcpCtSetPlayerAppSettingCmd,
        BtifAvrcpCtPlayItemCmd,
        BtifAvrcpCtGetCurrentMetadataCmd,
        BtifAvrcpCtGetPlaybackStateCmd,
        BtifAvrcpCtGetNowPlayingListCmd,
        BtifAvrcpCtGetFolderListCmd,
        BtifAvrcpCtGetPlayerListCmd,
        BtifAvrcpCtChangeFolderPathCmd,
        BtifAvrcpCtSetBrowsedPlayerCmd,
        BtifAvrcpCtSetAddressedPlayerCmd,
        BtifAvrcpCtSetVolumeRsp,
        BtifAvrcpCtRegisterAbsVolRsp,
        BtifAvrcpCtCleanup,
    };
    return &table;
}
