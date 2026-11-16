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

static bt_status_t BtifAvrcpTgInit(btrc_callbacks_t* callbacks)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgGetPlayStatusRsp(const RawAddress& bd_addr, btrc_play_status_t play_status,
    uint32_t song_len, uint32_t song_pos)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgListPlayerAppAttrRsp(const RawAddress& bd_addr, int num_attr, btrc_player_attr_t* p_attrs)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgListPlayerAppValueRsp(const RawAddress& bd_addr, int num_val, uint8_t* p_vals)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgGetPlayerAppValueRsp(const RawAddress& bd_addr, btrc_player_settings_t* p_vals)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgGetPlayerAppAttrTextRsp(const RawAddress& bd_addr, int num_attr,
    btrc_player_setting_text_t* p_attrs)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgGetPlayerAppValueTextRsp(const RawAddress& bd_addr, int num_val,
    btrc_player_setting_text_t* p_vals)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgGetElementAttrRsp(const RawAddress& bd_addr, uint8_t num_attr,
    btrc_element_attr_val_t* p_attrs)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgSetPlayerAppValueRsp(const RawAddress& bd_addr, btrc_status_t rsp_status)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgRegisterNotificationRsp(btrc_event_id_t event_id, btrc_notification_type_t type,
    btrc_register_notification_t* p_param)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgSetVolume(uint8_t volume)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgSetAddressedPlayerRsp(const RawAddress& bd_addr, btrc_status_t rsp_status)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgSetBrowsedPlayerRsp(const RawAddress& bd_addr, btrc_status_t rsp_status,
    uint32_t num_items, uint16_t charset_id, uint8_t folder_depth, btrc_br_folder_name_t* p_folders)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgGetFolderItemsListRsp(const RawAddress& bd_addr, btrc_status_t rsp_status,
    uint16_t uid_counter, uint8_t num_items, btrc_folder_items_t* p_items)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgChangePathRsp(const RawAddress& bd_addr, btrc_status_t rsp_status, uint32_t num_items)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgGetItemAttrRsp(const RawAddress& bd_addr, btrc_status_t rsp_status, uint8_t num_attr,
    btrc_element_attr_val_t* p_attrs)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgPlayItemRsp(const RawAddress& bd_addr, btrc_status_t rsp_status)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgGetTotalNumOfItemsRsp(const RawAddress& bd_addr, btrc_status_t rsp_status,
    uint32_t uid_counter, uint32_t num_items)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgSearchRsp(const RawAddress& bd_addr, btrc_status_t rsp_status, uint32_t uid_counter,
    uint32_t num_items)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static bt_status_t BtifAvrcpTgAddToNowPlayingRsp(const RawAddress& bd_addr, btrc_status_t rsp_status)
{
    // Stub: the stack API for this profile is not wired up yet.
    return {};
}

static void BtifAvrcpTgCleanup(void)
{
    // Stub: the stack API for this profile is not wired up yet.
}

} // namespace

const void *ProfileInterfaces::GetAvrcpTg()
{
    static const btrc_interface_t table = {
        sizeof(btrc_interface_t),
        BtifAvrcpTgInit,
        BtifAvrcpTgGetPlayStatusRsp,
        BtifAvrcpTgListPlayerAppAttrRsp,
        BtifAvrcpTgListPlayerAppValueRsp,
        BtifAvrcpTgGetPlayerAppValueRsp,
        BtifAvrcpTgGetPlayerAppAttrTextRsp,
        BtifAvrcpTgGetPlayerAppValueTextRsp,
        BtifAvrcpTgGetElementAttrRsp,
        BtifAvrcpTgSetPlayerAppValueRsp,
        BtifAvrcpTgRegisterNotificationRsp,
        BtifAvrcpTgSetVolume,
        BtifAvrcpTgSetAddressedPlayerRsp,
        BtifAvrcpTgSetBrowsedPlayerRsp,
        BtifAvrcpTgGetFolderItemsListRsp,
        BtifAvrcpTgChangePathRsp,
        BtifAvrcpTgGetItemAttrRsp,
        BtifAvrcpTgPlayItemRsp,
        BtifAvrcpTgGetTotalNumOfItemsRsp,
        BtifAvrcpTgSearchRsp,
        BtifAvrcpTgAddToNowPlayingRsp,
        BtifAvrcpTgCleanup,
    };
    return &table;
}
