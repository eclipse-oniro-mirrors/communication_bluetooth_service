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

#ifndef HCI_DEF_LE_CMD_6_0_H
#define HCI_DEF_LE_CMD_6_0_H

#include <stdint.h>

// Included first so HCI_COMMAND_OGF_LE_CONTROLLER, MAKE_OPCODE and the shared
// HciLeFeaturesAll container are available. hci_def_le_cmd.h re-includes this
// header at its tail; the include guard turns that into a no-op.
#include "hci_def_le_cmd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.128 (p2795) LE Read All Local Supported Features Command (Command_Complete, no params)
#define HCI_LE_READ_ALL_LOCAL_SUPPORTED_FEATURES MAKE_OPCODE(0x0087, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint8_t maxPage;
    HciLeFeaturesAll leFeatures;   // 248 octets; page 0 is the 0x0003 prefix
} HciLeReadAllLocalSupportedFeaturesReturnParam;  // 250 B

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.129 (p2796) LE Read All Remote Features Command (Command_Status, -> subevent 0x2B)
#define HCI_LE_READ_ALL_REMOTE_FEATURES MAKE_OPCODE(0x0088, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint8_t pagesRequested;
} HciLeReadAllRemoteFeaturesParam;  // 3 B

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.1 (pp2484-2485)
// LE Set Event Mask: subevent 0x2B (LE Read All Remote Features Complete) = bit 42.
#define LE_EVENT_MASK_LE_READ_ALL_REMOTE_FEATURES_COMPLETE_EVENT 0x0000040000000000

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.1 (pp2484-2485)
// LE Set Event Mask: subevent 0x2A (LE CIS Established [v2]) = bit 41
// (bit = subevent - 1 = 0x2A - 1). Same mask bit is listed at 7.7.65,25 as the
// [v2] variant of the 0x19 [v1] event (mask bit 24, hci_def_le_cmd.h).
#define LE_EVENT_MASK_LE_CIS_ESTABLISHED_V2_EVENT 0x0000020000000000

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.144 (p2856)
// LE Set Decision Data Command (Decision-Based Advertising Filtering, Vol 6 Part B 4.6.43).
// Wire order: Advertising_Handle(1) || Decision_Type_Flags(1) || Decision_Data_Length(1) ||
// Decision_Data(0..8). Decision_Type_Flags bit 0 marks a leading Resolvable Tag subfield;
// the tag is a fixed 6 octets (Vol 6 Part B 2.3.1,11, Fig 2.11/2.12), so with the bit set
// Decision_Data_Length must be 6..8 and the remaining octets are Arbitrary Data.
#define HCI_LE_SET_DECISION_DATA MAKE_OPCODE(0x0080, HCI_COMMAND_OGF_LE_CONTROLLER)

#define LE_DECISION_DATA_LEN_MAX 8
#define LE_DECISION_TYPE_FLAG_RESOLVABLE_TAG 0x01   // bit 0
#define LE_DECISION_RESOLVABLE_TAG_LEN 6            // Hash(3) || Prand(3)

// Pointer model (same as HciLeSetExtendedAdvertisingDataParam, 0x0037): the data is
// caller-owned and the sender packs exactly 3 + decisionDataLength octets on the wire.
typedef struct {
    uint8_t advertisingHandle;
    uint8_t decisionTypeFlags;
    uint8_t decisionDataLength;      // 0..8
    const uint8_t *decisionData;     // caller-owned, 0..8 octets; sent verbatim
} HciLeSetDecisionDataParam;

// Command_Complete return carries only Status (7.8.144); the CC handler strips 1 B.
typedef struct {
    uint8_t status;
} HciLeSetDecisionDataReturnParam;

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.145 (p2858)
// LE Set Decision Instructions Command (Decision-Based Advertising Filtering,
// Vol 6 Part B 4.6.43). Variable length: after Num_Tests(1) the tests follow in
// per-test interleaved order (Vol 4 Part E 5.2 default), each test being one
// 18 B record, so the total command length is 1 + Num_Tests*18.
#define HCI_LE_SET_DECISION_INSTRUCTIONS MAKE_OPCODE(0x0081, HCI_COMMAND_OGF_LE_CONTROLLER)

#define LE_DECISION_TEST_PARAMS_LEN 16
// Controller conformance floor (7.8.145) is 8, but this is not a host send-floor:
// the send path only rejects numTests==0 / > LE_DECISION_TESTS_HOST_MAX / bit0==0;
// values 1..7 are valid HCI and the Controller answers 0x43 if it supports fewer.
#define LE_DECISION_TESTS_HOST_MAX 14   // Host structural cap: 1 + 18*N <= 255 (HCI command length)

// Test_Flags[i] bits (7.8.145, Vol 4 Part E p2858).
#define LE_DECISION_TEST_FLAG_NEW_GROUP 0x01       // bit 0: this test starts a new test group
#define LE_DECISION_TEST_FLAG_PASS_IF_CHECK 0x02   // bit 1: pass if the check passes
#define LE_DECISION_TEST_FLAG_PASS_IF_FAIL 0x04    // bit 2: pass if the check fails
#define LE_DECISION_TEST_FLAG_PASS_IF_ABSENT 0x08  // bit 3: pass if the field is not present

// One decision test: 18 B on the wire. Members stay in wire order so the sender
// copies each record verbatim; the struct carries a tag so the public GAP headers
// can forward-declare the type (only pointers cross the GAPIF boundary).
typedef struct HciLeDecisionTestParamTag {
    uint8_t flags;      // Test_Flags; bit 0 of the first test must be 1 (7.8.145)
    uint8_t field;      // Test_Field selects which subfield the test targets
    uint8_t parameters[LE_DECISION_TEST_PARAMS_LEN];  // meaning depends on Test_Field
} HciLeDecisionTestParam;   // 18 B

// Embedded pointer to a caller-owned array of numTests records (same pattern as
// HciLeSetCigParametersParam.cisConfig). numTests <= LE_DECISION_TESTS_HOST_MAX.
typedef struct {
    uint8_t numTests;
    const HciLeDecisionTestParam *tests;
} HciLeSetDecisionInstructionsParam;

// Command_Complete return carries only Status (7.8.145); the CC handler strips 1 B.
typedef struct {
    uint8_t status;
} HciLeSetDecisionInstructionsReturnParam;

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.53 (p2595): DBAF
// extension bits of the 0x0036 Advertising_Event_Properties field, shared by the
// HCI sender and the GAP set-param path. Bit 7 asks the set to emit ADV_DECISION_IND;
// bits 8/9 force AdvA / ADI into the decision-PDU extended header (Vol 6 Part B
// 2.3.1,11 Table 2.10). RFU is bits 10-15 (kept locked by the 0xFC00 send mask).
#define LE_EXT_ADV_EVENT_PROP_DECISION_PDU 0x80    // bit 7: use decision PDUs when advertising
#define LE_EXT_ADV_EVENT_PROP_DECISION_ADVA 0x100  // bit 8: include AdvA in decision PDUs
#define LE_EXT_ADV_EVENT_PROP_DECISION_ADI 0x200   // bit 9: include ADI in decision PDUs

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.64 (p2626-2627): the
// decision-mode subfield occupies bits 2-3 of the 0x0041 Scanning_Filter_Policy
// byte. Modes are 0b00 No-decisions, 0b01 All-PDUs, 0b11 Decisions-only and
// 0b10 RFU; bits 4-7 stay RFU. Valid bytes are {0x00..0x0F} minus {0x08..0x0B}.
#define LE_SCAN_DECISION_MODE_NONE 0x00            // 0b00: process no decision PDUs
#define LE_SCAN_DECISION_MODE_ALL_PDUS 0x04        // 0b01: process all advertising PDUs
#define LE_SCAN_DECISION_MODE_RFU 0x08             // 0b10: reserved
#define LE_SCAN_DECISION_MODE_DECISIONS_ONLY 0x0C  // 0b11: process only decision PDUs
#define LE_SCAN_DECISION_MODE_MASK 0x0C            // bits 2-3 select the mode

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.66 (p2637): BT6.0
// values of the 0x0043 Initiator_Filter_Policy field. 0x00/0x01 are the pre-6.0
// values (FAL not used/used, decisions ignored); 0x02-0x04 are the decision
// values mapped to Link-Layer initiator modes 2/3/5 (Vol 6 Part B 4.3.4).
#define LE_INITIATOR_FILTER_POLICY_DECISION_PDU_ONLY 0x02
#define LE_INITIATOR_FILTER_POLICY_FAL_ALL_PDU 0x03
#define LE_INITIATOR_FILTER_POLICY_DECISION_ALL_PDU 0x04
#define LE_INITIATOR_FILTER_POLICY_MAX 0x04        // 0x00-0x04 valid; above is RFU

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.53 (p2595):
// Advertising_Event_Properties defines bits 0-9 (bit 7 Use decision PDUs, bit 8
// Include AdvA in decision PDUs, bit 9 Include ADI in decision PDUs), so the RFU
// set moved from bits 7-15 (5.4, p2450-2451: 0xFF80) down to bits 10-15. 7.8.53
// carries this one field in both command forms - 0x0036 [v1] and 0x007F [v2] -
// so both senders reference this definition instead of a per-file copy.
#define LE_EXT_ADV_EVENT_PROPERTIES_RESERVED_MASK 0xFC00

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.146 (p2867) LE Add Device To Monitored Advertisers List (Command_Complete, 10 B params).
#define HCI_LE_ADD_DEVICE_TO_MONITORED_ADVERTISERS_LIST MAKE_OPCODE(0x0098, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t advertiserAddressType;   // 0x00 public / 0x01 random only (command width)
    uint8_t advertiserAddress[BD_ADDR_LENGTH];
    int8_t rssiThresholdLow;         // -127..+20 dBm
    int8_t rssiThresholdHigh;        // -127..+20 dBm
    uint8_t timeout;                 // 0x00 reserved; 0x01..0xFF seconds
} HciLeAddDeviceToMonitoredAdvertisersListParam;  // 10 B

typedef HciStatusParam HciLeAddDeviceToMonitoredAdvertisersListReturnParam;

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.147 (p2870) LE Remove Device From Monitored Advertisers List (Command_Complete, 7 B params).
#define HCI_LE_REMOVE_DEVICE_FROM_MONITORED_ADVERTISERS_LIST MAKE_OPCODE(0x0099, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t advertiserAddressType;
    uint8_t advertiserAddress[BD_ADDR_LENGTH];
} HciLeRemoveDeviceFromMonitoredAdvertisersListParam;  // 7 B

typedef HciStatusParam HciLeRemoveDeviceFromMonitoredAdvertisersListReturnParam;

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.148 (p2872) LE Clear Monitored Advertisers List (Command_Complete, no params).
#define HCI_LE_CLEAR_MONITORED_ADVERTISERS_LIST MAKE_OPCODE(0x009A, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef HciStatusParam HciLeClearMonitoredAdvertisersListReturnParam;

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.149 (p2873) LE Enable Monitoring Advertisers (Command_Complete, 1 B param).
#define HCI_LE_ENABLE_MONITORING_ADVERTISERS MAKE_OPCODE(0x009C, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t enable;                  // 0x00 disable, 0x01 enable/restart
} HciLeEnableMonitoringAdvertisersParam;  // 1 B

typedef HciStatusParam HciLeEnableMonitoringAdvertisersReturnParam;

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.150 (p2875) LE Read Monitored Advertisers List Size (Command_Complete, no params).
// Note: OCF 0x009B Read Size is section 7.8.150 while OCF 0x009C Enable is 7.8.149.
#define HCI_LE_READ_MONITORED_ADVERTISERS_LIST_SIZE MAKE_OPCODE(0x009B, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint8_t number;                  // list capacity, may change at run time; 0x00 reserved
} HciLeReadMonitoredAdvertisersListSizeReturnParam;  // 2 B

// 7.8.1 LE Set Event Mask (p2485): LE Meta subevent 0x34 -> mask bit 51.
// No ULL suffix: mirrors the module-1 mask macro
// LE_EVENT_MASK_LE_READ_ALL_REMOTE_FEATURES_COMPLETE_EVENT (bit 42).
#define LE_EVENT_MASK_LE_MONITORED_ADVERTISERS_REPORT_EVENT 0x0008000000000000

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.151 (p2876) LE Frame Space Update Command (Command_Status, -> subevent 0x35).
// Wire order: Connection_Handle(2) || Frame_Space_Min(2) || Frame_Space_Max(2) ||
// PHYS(1) || Spacing_Types(2), 9 B in total.
#define HCI_LE_FRAME_SPACE_UPDATE MAKE_OPCODE(0x009D, HCI_COMMAND_OGF_LE_CONTROLLER)

// Frame_Space wire range is 0x0000-0x2710 us (7.8.151), 0x2710 being the 10 ms
// FS_Max ceiling of the LL request (Vol 6 Part B 2.4.2,54). A Frame_Space_Max below
// the value in use is not an error: the Controller ignores it and uses the value in
// use (7.8.151). PHYS/Spacing_Types are non-zero masks of the supported bits below;
// reserved bits would draw an Invalid HCI Command Parameters (0x12) reply, rejected
// by the sender (5.3). No MIN constant: the lower bound 0x0000 is the uint16_t wire
// width and no host check tests it.
#define LE_FRAME_SPACE_MAX_US 0x2710      // 10000 us = 10 ms
#define LE_FRAME_SPACE_PHYS_MASK 0x07     // PHYS bits 0-2: LE 1M / LE 2M / LE Coded
#define LE_FRAME_SPACE_SPACING_TYPES_MASK 0x1F  // Spacing_Types bits 0-4 (Table 2.40)

typedef struct {
    uint16_t connectionHandle;
    uint16_t frameSpaceMin;
    uint16_t frameSpaceMax;
    uint8_t phys;           // PHYS: bit0 LE 1M, bit1 LE 2M, bit2 LE Coded (7.8.151)
    uint16_t spacingTypes;  // Spacing_Types: bits 0-4 T_IFS_ACL_CP/PC, T_MCES, T_IFS_CIS, T_MSS_CIS
} HciLeFrameSpaceUpdateParam;  // 9 B

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.1 LE Set Event Mask
// subevent 0x35 (LE Frame Space Update Complete) = bit 52 of the LE_Event_Mask.
#define LE_EVENT_MASK_LE_FRAME_SPACE_UPDATE_COMPLETE_EVENT 0x0010000000000000

// Connection_Handle range shared by all Channel Sounding commands that name an
// ACL (Vol 4, Part E, 7.1). CS test uses LE_CS_TEST_HANDLE (0x0FFF) as a special
// value and is checked separately.
#define LE_CS_CONNECTION_HANDLE_MAX 0x0EFF

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.130 (p2798) LE Read Local Supported Capabilities Command (Command_Complete, no params).
// The 28-B capability body (HciLeCsCapabilities, hci_def_common.h) is shared in
// identical wire order with the 0x008B parameter and the 0x2C event (pp2798-2804).
#define HCI_LE_CS_READ_LOCAL_SUPPORTED_CAPABILITIES MAKE_OPCODE(0x0089, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    HciLeCsCapabilities capabilities;  // 28 octets
} HciLeCsReadLocalSupportedCapabilitiesReturnParam;  // 29 B

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.131 (p2805) LE Read Remote Supported Capabilities Command (Command_Status, -> subevent 0x2C)
#define HCI_LE_CS_READ_REMOTE_SUPPORTED_CAPABILITIES MAKE_OPCODE(0x008A, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;  // LE ACL handle carrying the CS session
} HciLeCsReadRemoteSupportedCapabilitiesParam;  // 2 B

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.132 (p2807) LE Write Cached Remote Supported Capabilities Command (Command_Complete).
// The 28-B body is the caller's cached copy of the remote capability container.
#define HCI_LE_CS_WRITE_CACHED_REMOTE_SUPPORTED_CAPABILITIES MAKE_OPCODE(0x008B, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    HciLeCsCapabilities capabilities;  // 28 B write-back body
} HciLeCsWriteCachedRemoteSupportedCapabilitiesParam;  // 30 B

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeCsWriteCachedRemoteSupportedCapabilitiesReturnParam;  // 3 B

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.133 (p2815) LE CS Security Enable Command (Command_Status, -> subevent 0x2E).
// "Start or restart the Channel Sounding Security Start procedure" (p2815). This
// command has no return params; its result is the LE CS Security Enable Complete
// event (bit 45, subevent 0x2E). Pre-command rejections (0x0C when the CS Host
// Support bit 47 is not set or the Host is a Peripheral, 0x2F for an unencrypted
// link, 0x02 for a nonexistent handle) come back in the Command_Status event and
// are funneled into the 0x2E-shaped completion callback by the command-failure
// path (7.8.133).
#define HCI_LE_CS_SECURITY_ENABLE MAKE_OPCODE(0x008C, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;  // LE ACL handle; 0x0000-0x0EFF
} HciLeCsSecurityEnableParam;  // 2 B

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.134 (p2816) LE CS Set Default Settings Command (Command_Complete). The Controller
// starts from "all roles disabled, CS_SYNC_Antenna_Selection = 0x01" and this
// command replaces that default for one connection (p2816). Return = Status +
// Connection_Handle (p2818).
#define HCI_LE_CS_SET_DEFAULT_SETTINGS MAKE_OPCODE(0x008D, HCI_COMMAND_OGF_LE_CONTROLLER)

#define LE_CS_ROLE_ENABLE_MASK 0x03     // Role_Enable bits 0/1 = Initiator/Reflector
#define LE_CS_ANTENNA_ID_MIN 0x01       // CS_SYNC_Antenna_Selection 0x01-0x04
#define LE_CS_ANTENNA_ID_MAX 0x04       // CS_SYNC_Antenna_Selection 0x01-0x04
#define LE_CS_ANTENNA_ID_REPEAT 0xFE    // 0xFE: repeat 0x01-0x04 in order
#define LE_CS_ANTENNA_ID_NONE 0xFF      // 0xFF: Host has no recommendation
// Shared by every command and event carrying a CS Config_ID (0x0090/0x0091/0x2F).
#define LE_CS_CONFIG_ID_MAX 0x03

typedef struct {
    uint16_t connectionHandle;
    uint8_t roleEnable;              // bits 0/1; disabling a role with a valid config -> 0x12
    uint8_t csSyncAntennaSelection;  // 0x01-0x04 id, 0xFE repeat, 0xFF no recommendation
    int8_t maxTxPower;               // dBm, EIRP, -127..20; Controller clamps to its own max
} HciLeCsSetDefaultSettingsParam;  // 5 B

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeCsSetDefaultSettingsReturnParam;  // 3 B

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.135 (p2819) LE CS Read Remote FAE Table Command (Command_Status, -> subevent 0x2D).
// Reads the peer's per-channel mode-0 FAE table over the CS Mode-0 FAE Table Request
// procedure. A peer whose mode-0 transmissions carry no Frequency Actuation Error
// (the No_FAE bit of [Vol 6] Part B 2.4.2,44) is answered by the Controller as a 0x2D
// with Status 0x11 instead of a table (p2819); that answer is the Controller's and
// this layer never pre-empts it from cached capabilities.
#define HCI_LE_CS_READ_REMOTE_FAE_TABLE MAKE_OPCODE(0x008E, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;  // Connection_Handle, range 0x0000-0x0EFF (p2819)
} HciLeCsReadRemoteFaeTableParam;  // 2 B

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.136 (p2820) LE CS Write Cached Remote FAE Table Command (Command_Complete). Writes a
// cached copy of the peer's 72-B table into the local Controller, which is what lets a
// reconnect skip the FAE Table Request procedure the 0x2D read would otherwise run
// again (7.8.135 p2819). Its parameter table is the only CS handle field that offers
// 0x0FFF beside the ACL range (p2820), because the write-back is one a tester's own
// Controller can take with no connection.
#define HCI_LE_CS_WRITE_CACHED_REMOTE_FAE_TABLE MAKE_OPCODE(0x008F, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;  // 0x0000-0x0EFF, or 0x0FFF: "Use for CS Test command"
    int8_t remoteFaeTable[LE_CS_REMOTE_FAE_TABLE_LEN];  // 72 B, 1/32 ppm, signed
} HciLeCsWriteCachedRemoteFaeTableParam;  // 74 B

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeCsWriteCachedRemoteFaeTableReturnParam;  // 3 B

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.137 (p2822) LE CS Create Config Command (Command_Status, -> subevent 0x2F).
// Create_Context 0x00 writes the config in the local Controller only, 0x01 also
// writes it in the peer through the Channel Sounding Configuration procedure;
// both paths complete with one LE_CS_Config_Complete event (p2822). The wire
// order below is the parameter order of 7.8.137 pp2824-2827 verbatim.
#define HCI_LE_CS_CREATE_CONFIG MAKE_OPCODE(0x0090, HCI_COMMAND_OGF_LE_CONTROLLER)

// Main_Mode_Type 0x01-0x03; Sub_Mode_Type 0x01-0x03 or 0xFF for "unused", which
// makes Min/Max_Main_Mode_Steps reserved (p2824/2825).
#define LE_CS_SUB_MODE_UNUSED 0xFF

typedef struct {
    uint16_t connectionHandle;
    uint8_t configId;               // 0..3
    uint8_t createContext;          // 0x00 local only, 0x01 local + remote
    uint8_t mainModeType;           // 0x01-0x03
    uint8_t subModeType;            // 0x01-0x03, or 0xFF unused
    uint8_t minMainModeSteps;       // 0x02-0xFF (reserved when subModeType is 0xFF)
    uint8_t maxMainModeSteps;       // 0x02-0xFF (reserved when subModeType is 0xFF)
    uint8_t mainModeRepetition;     // 0x00-0x03
    uint8_t mode0Steps;             // 0x01-0x03
    uint8_t role;                   // 0x00 Initiator, 0x01 Reflector
    uint8_t rttType;                // 0x00-0x06
    uint8_t csSyncPhy;              // 0x01 LE 1M, 0x02 LE 2M, 0x03 LE 2M 2BT
    uint8_t channelMap[LE_CS_CHANNEL_MAP_LEN];  // 72 usable of 80 one-bit fields, see hci_def_common.h
    uint8_t channelMapRepetition;   // 0x01-0xFF
    uint8_t channelSelectionType;   // 0x00 algorithm #3b, 0x01 algorithm #3c
    uint8_t ch3cShape;              // 0x00 hat, 0x01 X (only when 0x01 uses #3c)
    uint8_t ch3cJump;               // 0x02-0x08 (same condition as ch3cShape)
    uint8_t reserved;               // shall be 0x00
} HciLeCsCreateConfigParam;  // 28 B

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.138 (p2828) LE CS Remove Config Command (Command_Status, -> subevent 0x2F). Also
// deletes the CS procedure parameters set for this Config_ID (p2828).
#define HCI_LE_CS_REMOVE_CONFIG MAKE_OPCODE(0x0091, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint8_t configId;               // 0..3
} HciLeCsRemoveConfigParam;  // 3 B

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.139 (p2830) LE CS Set Channel Classification Command (Command_Complete, 10 B param).
// The only CS command with no Connection_Handle: the classification is
// Controller-wide, persists until overwritten or a Controller reset, and the
// Controller may combine it with its own local information (p2830). Re-issuing it
// less than 1 second after the previous issue is answered by the Controller with
// 0x0C, and RFU channels or fewer than 15 enabled channels with 0x12 (pp2830-2831).
#define HCI_LE_CS_SET_CHANNEL_CLASSIFICATION MAKE_OPCODE(0x0092, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t channelClassification[LE_CS_CHANNEL_MAP_LEN];  // 72 usable of 80 one-bit fields, see hci_def_common.h
} HciLeCsSetChannelClassificationParam;  // 10 B

// Command_Complete return carries only Status (7.8.139 p2831); the CC handler strips 1 B.
typedef HciStatusParam HciLeCsSetChannelClassificationReturnParam;

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.140 (p2832) LE CS Set Procedure Parameters Command (Command_Complete). The wire order
// below is the parameter order of 7.8.140 pp2833-2837 verbatim; the command sets the
// scheduling parameters a later 0x0094 enables, for the CS configuration named by
// Config_ID (p2832). The Min/Max_Procedure_Interval and Min/Max_Subevent_Len values
// are recommendations the Controller may ignore, and both intervals are ignored when
// Max_Procedure_Count is 1 (p2832).
#define HCI_LE_CS_SET_PROCEDURE_PARAMETERS MAKE_OPCODE(0x0093, HCI_COMMAND_OGF_LE_CONTROLLER)

#define LE_CS_TONE_ANTENNA_CONFIG_SELECTION_MAX 0x07  // Tone_Antenna_Config_Selection 0x00-0x07
#define LE_CS_PROCEDURE_PHY_MIN 0x01                  // PHY 0x01-0x04 (pp2835-2836)
#define LE_CS_PROCEDURE_PHY_MAX 0x04                  // PHY 0x01-0x04 (pp2835-2836)
#define LE_CS_PREFERRED_PEER_ANTENNA_MASK 0x0F        // bits 0-3: the four ordered antenna elements
#define LE_CS_SNR_CONTROL_MAX 0x04                    // 0x00-0x04, plus 0xFF for "not applied"
#define LE_CS_SNR_CONTROL_NONE 0xFF                   // SNR control is not to be applied

typedef struct {
    uint16_t connectionHandle;      // 0x0000-0x0EFF
    uint8_t configId;               // 0..3
    uint16_t maxProcedureLen;       // 0x0001-0xFFFF, N x 0.625 ms
    uint16_t minProcedureInterval;  // connection events, 0x0001-0xFFFF; ignored when count is 1
    uint16_t maxProcedureInterval;  // connection events, 0x0001-0xFFFF; ignored when count is 1
    uint16_t maxProcedureCount;     // 0x0000: continue until disabled
    uint8_t minSubeventLen[HCI_LE_24BIT_FIELD_LEN];      // microseconds, 1250 us to 4 s (raw wire octets)
    uint8_t maxSubeventLen[HCI_LE_24BIT_FIELD_LEN];      // microseconds, 1250 us to 4 s (raw wire octets)
    uint8_t toneAntennaConfigSelection;  // 0x00-0x07
    uint8_t phy;                    // 0x01 LE 1M, 0x02 LE 2M, 0x03/0x04 LE Coded S=8/S=2
    int8_t txPowerDelta;            // signed dB; 0x80 = no recommendation
    uint8_t preferredPeerAntenna;   // bits 0-3; set-bit count >= antennas of the config above
    uint8_t snrControlInitiator;    // 0x00-0x04 or 0xFF
    uint8_t snrControlReflector;    // 0x00-0x04 or 0xFF
} HciLeCsSetProcedureParametersParam;  // 23 B

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeCsSetProcedureParametersReturnParam;  // 3 B

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.141 (p2838) LE CS Procedure Enable Command (Command_Status, -> subevent 0x30). The
// command has no return parameters: the Controller answers with Command_Status and
// the result arrives as the 0x30 LE CS Procedure Enable Complete subevent. Enabling
// without a preceding 0x0093 for the same Config_ID draws 0x0C, an unknown Config_ID
// 0x12, scheduling/coexistence overflow 0x0D and a missing ACL 0x02 (p2838).
#define HCI_LE_CS_PROCEDURE_ENABLE MAKE_OPCODE(0x0094, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;  // 0x0000-0x0EFF
    uint8_t configId;           // 0..3
    uint8_t enable;             // 0x00 disable, 0x01 enable
} HciLeCsProcedureEnableParam;  // 4 B

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.142 (p2840) LE CS Test Command (Command_Complete, Status only). A test is a CS procedure
// with no Connection_Handle: its results arrive as ordinary 0x31/0x32 subevents under
// the reserved Connection_Handle 0x0FFF (7.7.65,44 p2448), which is why the return here
// is Status and nothing else (p2840). Issuing it while a previous test is enabled and
// has not completed draws 0x0C from the Controller (p2843); the Host keeps no refusal of
// its own for that, so the command still reaches the wire.
#define HCI_LE_CS_TEST MAKE_OPCODE(0x0095, HCI_COMMAND_OGF_LE_CONTROLLER)

// The 30 B fixed prefix of 7.8.142: the parameter list of p2840 in the order given
// there, which is also the wire order. It is a prefix rather than the whole
// parameter list - Override_Parameters_Data follows it, laid out as the parameter groups
// selected by the bits of Override_Config (p2849) - but keeping it a pointer-free packed
// struct makes sizeof() the wire length, the same guarantee the fixed-size command
// structs of this header rely on.
#define LE_CS_TEST_PREFIX_LEN 30

// Host structural cap on the override tail: Parameter_Total_Length is one octet (Vol 4
// Part E 5.4.1 p1873), so the 30 B prefix leaves 225. An oversized tail is rejected
// rather than truncated, the way LE_DECISION_TESTS_HOST_MAX caps 0x0081's test list.
#define LE_CS_TEST_OVERRIDE_MAX (0xFF - LE_CS_TEST_PREFIX_LEN)  // 225

// p2849: bit 0 set selects Channel_Length/Channel[i]; bit 0 clear selects the
// Channel_Map (10 B) / Channel_Selection_Type / Ch3c_Shape / Ch3c_Jump group. The two
// branches are exclusive and one is always present, so the tail is never empty: 2 (a
// one-channel bit-0-set tail) is its floor, and the only one a Host can apply without
// parsing the tail, which stays opaque to it.
#define LE_CS_TEST_OVERRIDE_MIN 2

// Override_Config bits 0, 2-8 and 10 are defined (p2848); every other bit is reserved
// for future use, so a word setting one is refused the way 0x0093 refuses the bits above
// Preferred_Peer_Antenna. Bit 10 carries no parameter group - it replaces the procedure
// with the Stable Phase test (p2843).
#define LE_CS_TEST_OVERRIDE_CONFIG_MASK 0x05FD

#define LE_CS_TEST_SUBEVENT_LEN_MIN 1250U     // microseconds (p2845)
#define LE_CS_TEST_SUBEVENT_LEN_MAX 4000000U  // 4 s (p2845)
#define LE_CS_TEST_TX_POWER_MIN (-127)        // dBm (p2845)
#define LE_CS_TEST_TX_POWER_MAX 20            // dBm (p2845)
#define LE_CS_TEST_TX_POWER_MINIMUM 0x7E      // "set transmitter to minimum power" (p2845)
#define LE_CS_TEST_TX_POWER_MAXIMUM 0x7F      // "set transmitter to maximum power" (p2845)

// Mode/type/role/PHY/antenna ranges shared by the 0x0090 Create Config and 0x0095 Test
// commands (7.8.137 pp2823-2827 and 7.8.142 pp2843-2848).
#define LE_CS_MAIN_MODE_TYPE_MIN                0x01
#define LE_CS_MAIN_MODE_TYPE_MAX                0x03
#define LE_CS_SUB_MODE_TYPE_MIN                 0x01
#define LE_CS_SUB_MODE_TYPE_MAX                 0x03
#define LE_CS_MAIN_MODE_REPETITION_MAX          0x03
#define LE_CS_MODE0_STEPS_MIN                   0x01
#define LE_CS_MODE0_STEPS_MAX                   0x03
#define LE_CS_ROLE_MAX                          0x01
#define LE_CS_RTT_TYPE_MAX                      0x06
#define LE_CS_SYNC_PHY_MIN                      0x01
#define LE_CS_SYNC_PHY_MAX                      0x03
#define LE_CS_SYNC_ANTENNA_SELECTION_MIN        0x01
#define LE_CS_SYNC_ANTENNA_SELECTION_MAX        0x04
#define LE_CS_NUM_SUBEVENTS_MAX                 0x20
#define LE_CS_CHANNEL_MAP_REPETITION_MIN        0x01  // 0x00 is reserved (p2848)

typedef struct {
    uint8_t mainModeType;               // 0x01 Mode-1, 0x02 Mode-2, 0x03 Mode-3
    uint8_t subModeType;                // 0x01-0x03, or 0xFF (LE_CS_SUB_MODE_UNUSED)
    uint8_t mainModeRepetition;         // 0x00-0x03
    uint8_t mode0Steps;                 // 0x01-0x03
    uint8_t role;                       // 0x00 Initiator, 0x01 Reflector
    uint8_t rttType;                    // 0x00-0x06
    uint8_t csSyncPhy;                  // 0x01 LE 1M, 0x02 LE 2M, 0x03 LE 2M 2BT
    uint8_t csSyncAntennaSelection;     // 0x01-0x04
    uint8_t subeventLen[HCI_LE_24BIT_FIELD_LEN];             // microseconds, 1250 us to 4 s (raw wire octets)
    uint16_t subeventInterval;          // 0x0000 single subevent, else units of 0.625 ms
    uint8_t maxNumSubevents;            // 0x00 ignored, 0x01-0x20
    int8_t transmitPowerLevel;          // dBm -127..20; 0x7E minimum, 0x7F maximum
    uint8_t tIp1Time;                   // enumerated, see the T_IP1/T_IP2 set below
    uint8_t tIp2Time;                   // enumerated, see the T_IP1/T_IP2 set below
    uint8_t tFcsTime;                   // enumerated, see the T_FCS set below
    uint8_t tPmTime;                    // enumerated, see the T_PM set below
    uint8_t tSwTime;                    // enumerated, see the T_SW set below
    uint8_t toneAntennaConfigSelection; // 0x00-0x07
    uint8_t reserved;                   // shall be set to 0x00 (p2847)
    uint8_t snrControlInitiator;        // 0x00-0x04 or 0xFF
    uint8_t snrControlReflector;        // 0x00-0x04 or 0xFF
    uint16_t drbgNonce;                 // octets 14 and 15 of the initial DRBG nonce
    uint8_t channelMapRepetition;       // 0x01-0xFF; 0x00 is reserved (p2848)
    uint16_t overrideConfig;            // bits 0, 2-8, 10; see LE_CS_TEST_OVERRIDE_CONFIG_MASK
    uint8_t overrideParametersLength;   // octets of Override_Parameters_Data on the wire
} HciLeCsTestPrefix;  // 30 B

// The whole 0x0095 parameter list: the 30 B prefix followed by the caller-owned override
// tail. The tail is a sequence of parameter groups selected by the prefix's
// Override_Config bits, so only its owner can lay it out; the sender appends it verbatim
// behind the prefix and never interprets it, and prefix.overrideParametersLength is what
// says how many octets of it go on the wire.
typedef struct {
    HciLeCsTestPrefix prefix;
    const uint8_t *overrideParametersData;
} HciLeCsTestParam;

// Command_Complete return carries only Status (7.8.142 p2840); the CC handler strips 1 B.
typedef HciStatusParam HciLeCsTestReturnParam;

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.143 (p2855) LE CS Test End Command (Command_Status, -> subevent 0x33). No parameter and no
// return: the stop is acknowledged by the 0x33 subevent, which the Controller sends only
// after every pending test result event (p2855). Whether a test is running at all is the
// Controller's answer (0x0C, p2855), so the Host checks nothing before sending.
#define HCI_LE_CS_TEST_END MAKE_OPCODE(0x0096, HCI_COMMAND_OGF_LE_CONTROLLER)

// Vol 4, Part E, 7.8.1 (p2485): subevent 0x2F (LE CS Config Complete) is mask
// bit 46. Set together with the other CS bits once CS (feature bit 46) is known.
#define LE_EVENT_MASK_LE_CS_CONFIG_COMPLETE_EVENT 0x0000400000000000

// Vol 4, Part E, 7.8.1 (p2485): subevent 0x30 (LE CS Procedure Enable Complete)
// = LE_Event_Mask bit 47 (bit = subevent - 1). Set with the other CS bits once CS
// (feature bit 46) makes the 0x0094 command usable.
#define LE_EVENT_MASK_LE_CS_PROCEDURE_ENABLE_COMPLETE_EVENT 0x0000800000000000

// Vol 4, Part E, 7.8.1 (p2485): subevent 0x31 (LE CS Subevent Result) is mask bit
// 48 and 0x32 (LE CS Subevent Result Continue) is mask bit 49. The two always travel
// together - a subevent too long for one event is continued in the other - so the mask
// sets them as a pair once CS (feature bit 46) is known.
#define LE_EVENT_MASK_LE_CS_SUBEVENT_RESULT_EVENT 0x0001000000000000
#define LE_EVENT_MASK_LE_CS_SUBEVENT_RESULT_CONTINUE_EVENT 0x0002000000000000

// Vol 4, Part E, 7.8.1 (p2485): subevent 0x2E (LE CS Security Enable Complete)
// = LE_Event_Mask bit 45, set together with bit 43 (0x2C) once CS (bit 46) is known.
#define LE_EVENT_MASK_LE_CS_SECURITY_ENABLE_COMPLETE_EVENT 0x0000200000000000

// Vol 4, Part E, 7.8.1 (p2485): subevent 0x2C (LE CS Read Remote Supported
// Capabilities Complete) = LE_Event_Mask bit 43. Mirrors the module-1 mask macro
// location (bit 42, 0x2B) and drops the ULL suffix like the FSU/MA ones.
#define LE_EVENT_MASK_LE_CS_READ_REMOTE_SUPPORTED_CAPABILITIES_COMPLETE_EVENT 0x0000080000000000

// Vol 4, Part E, 7.8.1 (p2485): subevent 0x2D (LE CS Read Remote FAE Table
// Complete) = LE_Event_Mask bit 44 (bit = subevent - 1), the read-pair of bit 43
// above and set with the other CS bits once CS (feature bit 46) is known. Without it
// a locally issued 0x008E has no completion notice at all: the command is answered by
// Command_Status and the 0x2D is the result.
#define LE_EVENT_MASK_LE_CS_READ_REMOTE_FAE_TABLE_COMPLETE_EVENT 0x0000100000000000

// Vol 4, Part E, 7.8.1 (p2485): subevent 0x33 (LE CS Test End Complete) = LE_Event_Mask
// bit 50, set with the other CS bits once CS (feature bit 46) is known. It is the only
// result 0x0096 has, so without the bit a CS test could never be seen to end.
#define LE_EVENT_MASK_LE_CS_TEST_END_COMPLETE_EVENT 0x0004000000000000

#pragma pack(0)

#ifdef __cplusplus
}
#endif

#endif
