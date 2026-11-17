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

#include "btif/entry/btif_dispatch.h"

#include <cstring>

#include "hardware/bluetooth.h"
#include "btif/profiles/a2dp/a2dp.h"
#include "btif/profiles/profile_interfaces.h"
#include "btif/profiles/avrcp/avrcp.h"
#include "btif/profiles/ble/ble.h"
#include "btif/profiles/gatt/gatt.h"
#include "btif/profiles/hearing_aid/hearing_aid.h"
#include "btif/profiles/hfp/hfp.h"
#include "btif/profiles/hid/hid.h"
#include "btif/profiles/pan/pan.h"
#include "btif/profiles/sdp/sdp.h"
#include "btif/profiles/socket/socket.h"
#include "btif/profiles/vendor/vendor.h"
#include "log.h"


const void *BtifGetProfileInterface(const char *profileId)
{
    if (profileId == nullptr) {
        HILOGW("%{public}s: called with null profileId", __FUNCTION__);
        return nullptr;
    }
    if (strcmp(profileId, BT_PROFILE_GATT_ID) == 0) {
        return ProfileInterfaces::GetGatt();
    }
    if (strcmp(profileId, BT_PROFILE_ADVANCED_AUDIO_ID) == 0) {
        return ProfileInterfaces::GetA2dpSource();
    }
    if (strcmp(profileId, BT_PROFILE_ADVANCED_AUDIO_SINK_ID) == 0) {
        return ProfileInterfaces::GetA2dpSink();
    }
    if (strcmp(profileId, BT_PROFILE_AV_RC_ID) == 0) {
        return ProfileInterfaces::GetAvrcpTg();
    }
    if (strcmp(profileId, BT_PROFILE_AV_RC_CTRL_ID) == 0) {
        return ProfileInterfaces::GetAvrcpCt();
    }
    if (strcmp(profileId, BT_PROFILE_HIDHOST_ID) == 0) {
        return ProfileInterfaces::GetHidHost();
    }
    if (strcmp(profileId, BT_PROFILE_HIDDEV_ID) == 0) {
        return ProfileInterfaces::GetHidDevice();
    }
    if (strcmp(profileId, BT_PROFILE_PAN_ID) == 0) {
        return ProfileInterfaces::GetPan();
    }
    if (strcmp(profileId, BT_PROFILE_SDP_CLIENT_ID) == 0) {
        return ProfileInterfaces::GetSdp();
    }
    if (strcmp(profileId, BT_PROFILE_SOCKETS_ID) == 0) {
        return ProfileInterfaces::GetSocket();
    }
    if (strcmp(profileId, BT_PROFILE_HEARING_AID_ID) == 0) {
        return ProfileInterfaces::GetHearingAid();
    }
    if (strcmp(profileId, BT_PROFILE_HANDSFREE_ID) == 0) {
        return ProfileInterfaces::GetHfpAg();
    }
    if (strcmp(profileId, BT_VENDOR_INTERFACE_ID) == 0) {
        return ProfileInterfaces::GetVendor();
    }
    HILOGW("%{public}s: unknown profileId=%{public}s", __FUNCTION__, profileId);
    return nullptr;
}
