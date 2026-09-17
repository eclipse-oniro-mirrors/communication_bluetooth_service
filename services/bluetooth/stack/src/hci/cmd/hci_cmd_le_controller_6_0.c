/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

// LE command senders for the Bluetooth 6.0 additions: LL Extended Feature Set
// (7.8.128 LE Read All Local Supported Features, 7.8.129 LE Read All Remote Features),
// DBAF (7.8.144 LE Set Decision Data, 7.8.145 LE Set Decision Instructions),
// Monitoring Advertisers (7.8.146-150), Frame Space Update (7.8.151), and Channel
// Sounding (7.8.130-133, 7.8.142-143). Kept out of hci_cmd_le_controller.c so the 5.x
// base file merges cleanly with 5.4 work.

#include <securec.h>

#include "hci/hci.h"
#include "hci/hci_le_controller_6_0.h"

#include "btstack.h"
#include "platform/include/allocator.h"

#include "hci_cmd.h"

#define LE_DECISION_ADV_HANDLE_MAX 0xEF   // Advertising_Handle range (7.8.144)
#define LE_DECISION_TEST_RECORD_HEAD_LEN 2   // Test_Flags + Test_Field, before Test_Parameters
#define HCI_CS_CHANNEL_MAP_BITS_PER_BYTE 8   // bits in one octet of the channel map

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.128 LE Read All Local Supported Features Command
int HCI_LeReadAllLocalSupportedFeatures(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_ALL_LOCAL_SUPPORTED_FEATURES, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.129 LE Read All Remote Features Command
int HCI_LeReadAllRemoteFeatures(const HciLeReadAllRemoteFeaturesParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_ALL_REMOTE_FEATURES, (void *)param, sizeof(HciLeReadAllRemoteFeaturesParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.144
// LE Set Decision Data Command (Command_Complete). Only the octets declared by
// Decision_Data_Length go on the wire (3 header octets + data), so the command is
// assembled into an exact-length buffer like the 0x0081/0x0037 senders.
int HCI_LeSetDecisionData(const HciLeSetDecisionDataParam *param)
{
    if (param == NULL || param->advertisingHandle > LE_DECISION_ADV_HANDLE_MAX ||
        param->decisionDataLength > LE_DECISION_DATA_LEN_MAX ||
        (param->decisionTypeFlags & ~LE_DECISION_TYPE_FLAG_RESOLVABLE_TAG) != 0 ||
        (param->decisionDataLength > 0 && param->decisionData == NULL)) {
        return BT_BAD_PARAM;
    }

    // A Resolvable Tag is a fixed 6-octet leading subfield (Vol 6 Part B 2.3.1,11).
    if ((param->decisionTypeFlags & LE_DECISION_TYPE_FLAG_RESOLVABLE_TAG) &&
        param->decisionDataLength < LE_DECISION_RESOLVABLE_TAG_LEN) {
        return BT_BAD_PARAM;
    }

    const size_t length = 1 + 1 + 1 + param->decisionDataLength;
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    size_t index = 0;
    buf[index++] = param->advertisingHandle;
    buf[index++] = param->decisionTypeFlags;
    buf[index++] = param->decisionDataLength;
    if (param->decisionDataLength > 0) {
        (void)memcpy_s(buf + index, length - index, param->decisionData, param->decisionDataLength);
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_DECISION_DATA, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }
    int result = HciSendCmd(cmd);
    MEM_MALLOC.free(buf);
    return result;
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.145
// LE Set Decision Instructions Command (Command_Complete). Variable length: the
// caller owns the numTests records; each 18 B record is copied in per-test
// interleaved wire order into a freshly allocated command buffer. Per 7.8.145,
// Num_Tests shall be non-zero and bit 0 of Test_Flags[0] shall be 1, else the
// Controller returns 0x12; only values above the host structural cap (14) are
// rejected here -- 9..14 are sent and the Controller answers 0x43 if it
// supports fewer.
int HCI_LeSetDecisionInstructions(const HciLeSetDecisionInstructionsParam *param)
{
    if (param == NULL || param->numTests == 0 || param->numTests > LE_DECISION_TESTS_HOST_MAX ||
        param->tests == NULL || (param->tests[0].flags & LE_DECISION_TEST_FLAG_NEW_GROUP) == 0) {
        return BT_BAD_PARAM;
    }

    const size_t length = 1 + param->numTests * (LE_DECISION_TEST_RECORD_HEAD_LEN + LE_DECISION_TEST_PARAMS_LEN);
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    uint16_t index = 0;
    buf[index++] = param->numTests;
    for (uint8_t i = 0; i < param->numTests; i++) {
        // Each test is one interleaved 18 B record: flags || field || parameters.
        buf[index++] = param->tests[i].flags;
        buf[index++] = param->tests[i].field;
        (void)memcpy_s(buf + index, length - index, param->tests[i].parameters,
            LE_DECISION_TEST_PARAMS_LEN);
        index += LE_DECISION_TEST_PARAMS_LEN;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_DECISION_INSTRUCTIONS, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }
    int result = HciSendCmd(cmd);

    MEM_MALLOC.free(buf);

    return result;
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.146-150
// Monitoring Advertisers (Vol 6 Part B 4.6.45) commands, all Command_Complete.
// pack(1) struct size is the exact wire length; host-side value checks are done
// at the GAP command entry, so the senders only guard the NULL parameter.
int HCI_LeAddDeviceToMonitoredAdvertisersList(const HciLeAddDeviceToMonitoredAdvertisersListParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_ADD_DEVICE_TO_MONITORED_ADVERTISERS_LIST, (void *)param,
        sizeof(HciLeAddDeviceToMonitoredAdvertisersListParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.147 LE Remove Device From Monitored Advertisers List Command
int HCI_LeRemoveDeviceFromMonitoredAdvertisersList(const HciLeRemoveDeviceFromMonitoredAdvertisersListParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_REMOVE_DEVICE_FROM_MONITORED_ADVERTISERS_LIST, (void *)param,
        sizeof(HciLeRemoveDeviceFromMonitoredAdvertisersListParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.148 LE Clear Monitored Advertisers List Command (Command_Complete, no params)
int HCI_LeClearMonitoredAdvertisersList(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_CLEAR_MONITORED_ADVERTISERS_LIST, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.150 LE Read Monitored Advertisers List Size Command (Command_Complete, no params)
int HCI_LeReadMonitoredAdvertisersListSize(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_MONITORED_ADVERTISERS_LIST_SIZE, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.149 LE Enable Monitoring Advertisers Command (Command_Complete, 1 B param)
int HCI_LeEnableMonitoringAdvertisers(const HciLeEnableMonitoringAdvertisersParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_ENABLE_MONITORING_ADVERTISERS, (void *)param,
        sizeof(HciLeEnableMonitoringAdvertisersParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.151
// LE Frame Space Update Command (Command_Status, -> subevent 0x35). Param is the
// fixed 9 B wire payload and is sent as-is. Pre-checking rejects inputs that would
// draw an Invalid HCI Command Parameters (0x12) reply: Min > Max, Max above 0x2710,
// or PHYS / Spacing_Types empty or carrying reserved bits. A Max below the value in
// use is intentionally not rejected: the Controller ignores it and uses the value in
// use (7.8.151). Handle liveness is not checked here -- an inactive ACL returns
// Unknown Connection Identifier (0x02) from the Controller (GAP lookup covers it).
int HCI_LeFrameSpaceUpdate(const HciLeFrameSpaceUpdateParam *param)
{
    if (param == NULL || param->frameSpaceMin > param->frameSpaceMax ||
        param->frameSpaceMax > LE_FRAME_SPACE_MAX_US ||
        (param->phys & ~LE_FRAME_SPACE_PHYS_MASK) != 0 || param->phys == 0 ||
        (param->spacingTypes & ~LE_FRAME_SPACE_SPACING_TYPES_MASK) != 0 || param->spacingTypes == 0) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_FRAME_SPACE_UPDATE, (void *)param, sizeof(HciLeFrameSpaceUpdateParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.130 LE Read Local Supported Capabilities Command (Command_Complete, no params)
int HCI_LeCsReadLocalSupportedCapabilities(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_CS_READ_LOCAL_SUPPORTED_CAPABILITIES, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.131 LE Read Remote Supported Capabilities Command (Command_Status, -> subevent 0x2C)
int HCI_LeCsReadRemoteSupportedCapabilities(const HciLeCsReadRemoteSupportedCapabilitiesParam *param)
{
    if (param == NULL || param->connectionHandle > LE_CS_CONNECTION_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd =
        HciAllocCmd(HCI_LE_CS_READ_REMOTE_SUPPORTED_CAPABILITIES, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.132 LE Write Cached Remote Supported Capabilities Command (Command_Complete).
// The 28 B body is written back verbatim as the Controller's cached value.
int HCI_LeCsWriteCachedRemoteSupportedCapabilities(
    const HciLeCsWriteCachedRemoteSupportedCapabilitiesParam *param)
{
    if (param == NULL || param->connectionHandle > LE_CS_CONNECTION_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd =
        HciAllocCmd(HCI_LE_CS_WRITE_CACHED_REMOTE_SUPPORTED_CAPABILITIES, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.133 LE CS Security Enable Command (Command_Status, -> subevent 0x2E)
int HCI_LeCsSecurityEnable(const HciLeCsSecurityEnableParam *param)
{
    if (param == NULL || param->connectionHandle > LE_CS_CONNECTION_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_CS_SECURITY_ENABLE, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.134 LE CS Set Default Settings Command (Command_Complete). The antenna
// selector admits 0x01-0x04, 0xFE (repeat) and 0xFF (no recommendation) and
// nothing else; the remaining Role_Enable and Max_TX_Power behaviour belongs to
// the Controller (a role without support or a bad antenna draws 0x11, a role
// disabled while a valid config exists draws 0x12, p2816).
int HCI_LeCsSetDefaultSettings(const HciLeCsSetDefaultSettingsParam *param)
{
    if (param == NULL || param->connectionHandle > LE_CS_CONNECTION_HANDLE_MAX ||
        (param->roleEnable & ~LE_CS_ROLE_ENABLE_MASK) != 0 ||
        !((param->csSyncAntennaSelection >= LE_CS_ANTENNA_ID_MIN &&
            param->csSyncAntennaSelection <= LE_CS_ANTENNA_ID_MAX) ||
            param->csSyncAntennaSelection == LE_CS_ANTENNA_ID_REPEAT ||
            param->csSyncAntennaSelection == LE_CS_ANTENNA_ID_NONE)) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_CS_SET_DEFAULT_SETTINGS, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.135 LE CS Read Remote FAE Table Command (Command_Status, -> subevent 0x2D)
int HCI_LeCsReadRemoteFaeTable(const HciLeCsReadRemoteFaeTableParam *param)
{
    if (param == NULL || param->connectionHandle > LE_CS_CONNECTION_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_CS_READ_REMOTE_FAE_TABLE, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.136 LE CS Write Cached Remote FAE Table Command (Command_Complete). The handle
// admits the CS test value beside the ACL range, because the p2820 parameter table
// offers 0x0FFF; the 72-B table is carried as given, its per-channel content being the
// caller's and the spec naming no value that would make one table malformed.
int HCI_LeCsWriteCachedRemoteFaeTable(const HciLeCsWriteCachedRemoteFaeTableParam *param)
{
    if (param == NULL || (param->connectionHandle > LE_CS_CONNECTION_HANDLE_MAX &&
        param->connectionHandle != LE_CS_TEST_HANDLE)) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_CS_WRITE_CACHED_REMOTE_FAE_TABLE, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// The CS Channel_Map shape rule of hci_def_common.h as a per-octet mask. The rule is
// one paragraph of the spec used by two commands - 7.8.137 p2825 for 0x0090 and
// 7.8.139 p2831 for 0x0092 - so the mask and the check below are shared by both
// senders rather than owned by either. Channels 0, 1, 23, 24, 25, 77 and 78 are
// reserved and shall be zero, and bit 79 is RFU, so octet 9 keeps only bits 0-4
// (channels 72-76).
static const uint8_t HCI_CS_CHANNEL_MAP_RESERVED[LE_CS_CHANNEL_MAP_LEN] = {
    0x03, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0,
};

// The Controller answers a map with fewer than 15 enabled channels, or with a
// reserved channel set, with 0x12 for both commands. Rejecting the same shape locally
// keeps a malformed map off the wire; everything else about the map - and every
// relationship it has to the rest of the configuration - is the Controller's call.
static bool HciCsChannelMapValid(const uint8_t map[LE_CS_CHANNEL_MAP_LEN])
{
    uint8_t enabled = 0;

    for (uint8_t i = 0; i < LE_CS_CHANNEL_MAP_LEN; i++) {
        if ((map[i] & HCI_CS_CHANNEL_MAP_RESERVED[i]) != 0) {
            return false;
        }
        for (uint8_t bit = 0; bit < HCI_CS_CHANNEL_MAP_BITS_PER_BYTE; bit++) {
            if ((map[i] & (1u << bit)) != 0) {
                enabled++;
            }
        }
    }
    // The reserved and RFU bits were forced zero above, so every remaining set bit
    // is one of the 72 usable channels (80 one-bit fields - 7 reserved - bit 79 RFU).
    return enabled >= LE_CS_CHANNEL_MAP_MIN_CHANNELS;
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.137 LE CS Create Config Command (Command_Status, -> subevent 0x2F). The checks
// here are the self-contained fields the Host can decide on its own - the connection
// handle, Config_ID > 3, a nonzero Reserved octet and the Channel_Map shape the same
// rule 0x0092 carries. The remaining field ranges are left to the Controller, as is
// the config's relationship to the local capability set, to the roles enabled by
// 0x008D and to an already enabled slot (0x11/0x12/0x0C, pp2823-2824) and, for the
// cases the CS engine can decide from its own session state, by the engine before
// sending.
int HCI_LeCsCreateConfig(const HciLeCsCreateConfigParam *param)
{
    if (param == NULL || param->connectionHandle > LE_CS_CONNECTION_HANDLE_MAX ||
        param->configId > LE_CS_CONFIG_ID_MAX || param->reserved != 0 ||
        !HciCsChannelMapValid(param->channelMap)) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_CS_CREATE_CONFIG, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.138 LE CS Remove Config Command (Command_Status, -> subevent 0x2F). Whether
// Config_ID exists and whether a CS procedure is still enabled on it is the
// Controller's call (0x12/0x0C, p2828).
int HCI_LeCsRemoveConfig(const HciLeCsRemoveConfigParam *param)
{
    if (param == NULL || param->connectionHandle > LE_CS_CONNECTION_HANDLE_MAX ||
        param->configId > LE_CS_CONFIG_ID_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_CS_REMOVE_CONFIG, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.139 LE CS Set Channel Classification Command (Command_Complete). The
// classification is Controller-wide: there is no handle to check against an ACL,
// and the Controller may combine the map with its own information (p2830). Only
// the map's shape is checked here, by the rule shared with 0x0090 above, which is
// the same rule behind the Controller's 0x12 (p2831). A re-issue less than 1 second
// after the previous one is left to the Controller, which is the side that answers
// 0x0C (p2830); the Host keeps no rate-limit state for it, so nothing here can drift
// from the Controller's view.
int HCI_LeCsSetChannelClassification(const HciLeCsSetChannelClassificationParam *param)
{
    if (param == NULL || !HciCsChannelMapValid(param->channelClassification)) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_CS_SET_CHANNEL_CLASSIFICATION, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// SNR_Control_* admit 0x00-0x04 or 0xFF ("not applied"); anything else is reserved (p2836).
static bool HciCsSnrControlValid(uint8_t snr)
{
    return snr <= LE_CS_SNR_CONTROL_MAX || snr == LE_CS_SNR_CONTROL_NONE;
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.140 LE CS Set Procedure Parameters Command (Command_Complete). Only the
// per-field ranges of pp2833-2837 are checked here; whether Config_ID exists, is
// already enabled or fits the Controller's scheduling is the Controller's answer
// (0x12/0x0D/0x0C, p2832) and, for the cases the engine can decide from its own
// session state, the engine's before sending. The Preferred_Peer_Antenna bit count
// against the antenna elements of Tone_Antenna_Config_Selection needs the
// [Vol 6] Part A antenna-element table and is not judged here.
int HCI_LeCsSetProcedureParameters(const HciLeCsSetProcedureParametersParam *param)
{
    if (param == NULL || param->connectionHandle > LE_CS_CONNECTION_HANDLE_MAX ||
        param->configId > LE_CS_CONFIG_ID_MAX || param->maxProcedureLen == 0 ||
        param->toneAntennaConfigSelection > LE_CS_TONE_ANTENNA_CONFIG_SELECTION_MAX ||
        param->phy < LE_CS_PROCEDURE_PHY_MIN || param->phy > LE_CS_PROCEDURE_PHY_MAX ||
        (param->preferredPeerAntenna & ~LE_CS_PREFERRED_PEER_ANTENNA_MASK) != 0 ||
        !HciCsSnrControlValid(param->snrControlInitiator) ||
        !HciCsSnrControlValid(param->snrControlReflector)) {
        return BT_BAD_PARAM;
    }
    // Count==1: Min/Max_Procedure_Interval shall be ignored (p2832); otherwise Range
    // is 0x0001-0xFFFF and a zero interval is refused before the wire.
    if (param->maxProcedureCount != 1 &&
        (param->minProcedureInterval == 0 || param->maxProcedureInterval == 0)) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_CS_SET_PROCEDURE_PARAMETERS, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.141 LE CS Procedure Enable Command (Command_Status, -> subevent 0x30). The
// command carries no return: the outcome is the 0x30 subevent. Whether the
// Config_ID has procedure parameters set and whether it is already in the
// requested state is the Controller's answer (0x0C/0x12, p2838) and, for the cases
// the engine can decide from its own session state, the engine's before sending.
int HCI_LeCsProcedureEnable(const HciLeCsProcedureEnableParam *param)
{
    if (param == NULL || param->connectionHandle > LE_CS_CONNECTION_HANDLE_MAX ||
        param->configId > LE_CS_CONFIG_ID_MAX || param->enable > LE_CS_PROCEDURE_ENABLED) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_CS_PROCEDURE_ENABLE, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// The five interlude and switch timings of 0x0095 are enumerated value sets rather than
// ranges (p2846): each field's table lists the microseconds the Controller accepts
// and marks the rest reserved for future use, so a value outside its set is never sent.
// The sets are file-local because only the check below uses them, the way
// HCI_CS_CHANNEL_MAP_RESERVED serves only HciCsChannelMapValid.
static const uint8_t HCI_CS_TEST_TIP_TIME[] = {0x0A, 0x14, 0x1E, 0x28, 0x32, 0x3C, 0x50, 0x91};
static const uint8_t HCI_CS_TEST_TFCS_TIME[] = {0x0F, 0x14, 0x1E, 0x28, 0x32, 0x3C, 0x50, 0x64, 0x78, 0x96};
static const uint8_t HCI_CS_TEST_TPM_TIME[] = {0x0A, 0x14, 0x28};
static const uint8_t HCI_CS_TEST_TSW_TIME[] = {0x00, 0x01, 0x02, 0x04, 0x0A};

static bool HciCsTestTimeValid(uint8_t value, const uint8_t *set, size_t count)
{
    for (size_t i = 0; i < count; i++) {
        if (value == set[i]) {
            return true;
        }
    }
    return false;
}

// Subevent_Len is 1250 us to 4 s (p2845) as a 24-bit little-endian count of microseconds,
// and anything outside that is reserved. The value is assembled from the frame's own
// octets rather than read as an integer, so this holds whatever the host byte order is.
#define CS_TEST_SUBEVENT_LEN_BYTE0 0
#define CS_TEST_SUBEVENT_LEN_BYTE1 1
#define CS_TEST_SUBEVENT_LEN_BYTE2 2
#define CS_TEST_SUBEVENT_LEN_SHIFT1 8
#define CS_TEST_SUBEVENT_LEN_SHIFT2 16

static bool HciCsTestSubeventLenValid(const uint8_t length[HCI_LE_24BIT_FIELD_LEN])
{
    const uint32_t microseconds =
        (uint32_t)length[CS_TEST_SUBEVENT_LEN_BYTE0] |
        ((uint32_t)length[CS_TEST_SUBEVENT_LEN_BYTE1] << CS_TEST_SUBEVENT_LEN_SHIFT1) |
        ((uint32_t)length[CS_TEST_SUBEVENT_LEN_BYTE2] << CS_TEST_SUBEVENT_LEN_SHIFT2);
    return microseconds >= LE_CS_TEST_SUBEVENT_LEN_MIN && microseconds <= LE_CS_TEST_SUBEVENT_LEN_MAX;
}

// Transmit_Power_Level is -127..+20 dBm, plus 0x7E and 0x7F asking for the lowest and the
// highest power the Controller can reach (p2846). Those two are octets the signed range
// cannot produce, so they are compared as octets; every other value is reserved.
static bool HciCsTestPowerValid(int8_t level)
{
    if (level >= LE_CS_TEST_TX_POWER_MIN && level <= LE_CS_TEST_TX_POWER_MAX) {
        return true;
    }
    return (uint8_t)level == LE_CS_TEST_TX_POWER_MINIMUM ||
        (uint8_t)level == LE_CS_TEST_TX_POWER_MAXIMUM;
}

// Mode/role/PHY/antenna selection and the reserved octet of 0x0095 (p2843-2848).
static bool HciCsTestModeAndRoleValid(const HciLeCsTestPrefix *p)
{
    return p->mainModeType >= LE_CS_MAIN_MODE_TYPE_MIN && p->mainModeType <= LE_CS_MAIN_MODE_TYPE_MAX &&
        (p->subModeType == LE_CS_SUB_MODE_UNUSED ||
            (p->subModeType >= LE_CS_SUB_MODE_TYPE_MIN && p->subModeType <= LE_CS_SUB_MODE_TYPE_MAX)) &&
        p->mainModeRepetition <= LE_CS_MAIN_MODE_REPETITION_MAX &&
        p->mode0Steps >= LE_CS_MODE0_STEPS_MIN && p->mode0Steps <= LE_CS_MODE0_STEPS_MAX &&
        p->role <= LE_CS_ROLE_MAX && p->rttType <= LE_CS_RTT_TYPE_MAX &&
        p->csSyncPhy >= LE_CS_SYNC_PHY_MIN && p->csSyncPhy <= LE_CS_SYNC_PHY_MAX &&
        p->csSyncAntennaSelection >= LE_CS_SYNC_ANTENNA_SELECTION_MIN &&
        p->csSyncAntennaSelection <= LE_CS_SYNC_ANTENNA_SELECTION_MAX;
}

// Subevent length, subevent count, transmit power and the five timing enumerations.
static bool HciCsTestTimingAndPowerValid(const HciLeCsTestPrefix *p)
{
    return HciCsTestSubeventLenValid(p->subeventLen) &&
        p->maxNumSubevents <= LE_CS_NUM_SUBEVENTS_MAX &&
        HciCsTestPowerValid(p->transmitPowerLevel) &&
        HciCsTestTimeValid(p->tIp1Time, HCI_CS_TEST_TIP_TIME, sizeof(HCI_CS_TEST_TIP_TIME)) &&
        HciCsTestTimeValid(p->tIp2Time, HCI_CS_TEST_TIP_TIME, sizeof(HCI_CS_TEST_TIP_TIME)) &&
        HciCsTestTimeValid(p->tFcsTime, HCI_CS_TEST_TFCS_TIME, sizeof(HCI_CS_TEST_TFCS_TIME)) &&
        HciCsTestTimeValid(p->tPmTime, HCI_CS_TEST_TPM_TIME, sizeof(HCI_CS_TEST_TPM_TIME)) &&
        HciCsTestTimeValid(p->tSwTime, HCI_CS_TEST_TSW_TIME, sizeof(HCI_CS_TEST_TSW_TIME));
}

// SNR control, channel map repetition, override config/length and the reserved octet.
static bool HciCsTestOverrideAndSnrValid(const HciLeCsTestPrefix *p)
{
    return p->toneAntennaConfigSelection <= LE_CS_TONE_ANTENNA_CONFIG_SELECTION_MAX &&
        p->reserved == 0 &&
        HciCsSnrControlValid(p->snrControlInitiator) &&
        HciCsSnrControlValid(p->snrControlReflector) &&
        p->channelMapRepetition >= LE_CS_CHANNEL_MAP_REPETITION_MIN &&
        (p->overrideConfig & ~LE_CS_TEST_OVERRIDE_CONFIG_MASK) == 0 &&
        p->overrideParametersLength >= LE_CS_TEST_OVERRIDE_MIN &&
        p->overrideParametersLength <= LE_CS_TEST_OVERRIDE_MAX;
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.142 LE CS Test Command (Command_Complete, Status only). The checks below are the
// self-contained field shapes of pp2843-2848: the enumerated values, the ranges, the
// reserved octet and bits, and the two host structural caps on the override tail
// (LE_CS_TEST_OVERRIDE_MAX and LE_CS_TEST_OVERRIDE_MIN). Whether a test is already
// enabled is the Controller's answer (0x0C, p2843) and is passed through, so a repeated
// 0x0095 does reach the wire. The wire body is one exact-length buffer - the 30 B prefix
// then the caller-owned tail - the shape HCI_LeSetDecisionData above uses.
int HCI_LeCsTest(const HciLeCsTestParam *param)
{
    if (param == NULL ||
        !HciCsTestModeAndRoleValid(&param->prefix) ||
        !HciCsTestTimingAndPowerValid(&param->prefix) ||
        !HciCsTestOverrideAndSnrValid(&param->prefix) ||
        param->overrideParametersData == NULL) {
        return BT_BAD_PARAM;
    }

    const size_t prefixLen = sizeof(param->prefix);
    const size_t length = prefixLen + param->prefix.overrideParametersLength;
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memcpy_s(buf, length, &param->prefix, prefixLen);
    (void)memcpy_s(buf + prefixLen, length - prefixLen,
        param->overrideParametersData, param->prefix.overrideParametersLength);

    HciCmd *cmd = HciAllocCmd(HCI_LE_CS_TEST, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }
    int result = HciSendCmd(cmd);
    MEM_MALLOC.free(buf);
    return result;
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.143 LE CS Test End Command (Command_Status, -> subevent 0x33). The command has a
// Command_Status and no parameter, so there is nothing to guard: whether a test is
// running is the Controller's answer, and the stop's outcome is the 0x33 subevent.
int HCI_LeCsTestEnd(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_CS_TEST_END, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}
