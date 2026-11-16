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

#include "btif/profiles/hfp/hfp.h"
#include "btif/profiles/profile_interfaces.h"

namespace {

using namespace bluetooth::headset;

class BtifHfpAgInterface final : public bluetooth::headset::Interface {
public:
    bt_status_t Init(bluetooth::headset::Callbacks *callbacks, int maxHfClients, bool inbandRingingEnabled) override
    {
        (void)callbacks;
        (void)maxHfClients;
        (void)inbandRingingEnabled;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t Connect(RawAddress *bdAddr) override
    {
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t Disconnect(RawAddress *bdAddr) override
    {
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t ConnectAudio(RawAddress *bdAddr) override
    {
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t DisconnectAudio(RawAddress *bdAddr) override
    {
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t isNoiseReductionSupported(RawAddress *bdAddr) override
    {
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t isVoiceRecognitionSupported(RawAddress *bdAddr) override
    {
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t StartVoiceRecognition(RawAddress *bdAddr) override
    {
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t StopVoiceRecognition(RawAddress *bdAddr) override
    {
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t VolumeControl(bthf_volume_type_t type, int volume, RawAddress *bdAddr) override
    {
        (void)type;
        (void)volume;
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t DeviceStatusNotification(bthf_network_state_t ntkState, bthf_service_type_t svcType,
        int signal, int battChg, RawAddress *bdAddr) override
    {
        (void)ntkState;
        (void)svcType;
        (void)signal;
        (void)battChg;
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t CopsResponse(const char *cops, RawAddress *bdAddr) override
    {
        (void)cops;
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t CindResponse(int svc, int numActive, int numHeld, bthf_call_state_t callSetupState,
        int signal, int roam, int battChg, RawAddress *bdAddr) override
    {
        (void)svc;
        (void)numActive;
        (void)numHeld;
        (void)callSetupState;
        (void)signal;
        (void)roam;
        (void)battChg;
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t FormattedAtResponse(const char *rsp, RawAddress *bdAddr) override
    {
        (void)rsp;
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t AtResponse(bthf_at_response_t responseCode, int errorCode, RawAddress *bdAddr) override
    {
        (void)responseCode;
        (void)errorCode;
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t ClccResponse(int index, bthf_call_direction_t dir, bthf_call_state_t state,
        bthf_call_mode_t mode, bthf_call_mpty_type_t mpty, const char *number,
        bthf_call_addrtype_t type, RawAddress *bdAddr) override
    {
        (void)index;
        (void)dir;
        (void)state;
        (void)mode;
        (void)mpty;
        (void)number;
        (void)type;
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t PhoneStateChange(int numActive, int numHeld, bthf_call_state_t callSetupState,
        const char *number, bthf_call_addrtype_t type, const char *name, RawAddress *bdAddr) override
    {
        (void)numActive;
        (void)numHeld;
        (void)callSetupState;
        (void)number;
        (void)type;
        (void)name;
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    void Cleanup() override
    {
    }

    bt_status_t SetScoAllowed(bool value) override
    {
        (void)value;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t SendBsir(bool value, RawAddress *bdAddr) override
    {
        (void)value;
        (void)bdAddr;
        return BT_STATUS_SUCCESS;
    }

    bt_status_t SetActiveDevice(RawAddress *activeDeviceAddr) override
    {
        (void)activeDeviceAddr;
        return BT_STATUS_SUCCESS;
    }
};

} // namespace

const void *ProfileInterfaces::GetHfpAg()
{
    static BtifHfpAgInterface instance;
    return &instance;
}
