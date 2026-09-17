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

#ifndef HCI_DEF_LE_EVT_6_0_H
#define HCI_DEF_LE_EVT_6_0_H

#include <stdint.h>

// Included first so HciLeFeaturesAll (hci_def_common.h) and the LE Meta subevent
// conventions are available. hci_def_le_evt.h re-includes this header at its
// tail; the include guard turns that into a no-op.
#include "hci_def_le_evt.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.7.65,25 (pp2387-2393)
// LE CIS Established [v2] Event, subevent 0x2A. Payload after the Subevent_Code is
// the [v1] field set (Status .. ISO_Interval, 28 octets) followed by the v2 tail:
// Sub_Interval(3) + Max_SDU_C_To_P(2) + Max_SDU_P_To_C(2) +
// SDU_Interval_C_To_P(3) + SDU_Interval_P_To_C(3) + Framing(1) = 42 octets total.
// 24-bit intervals are kept as raw wire octets (parsed with memcpy + IsoReadUint24
// downstream), matching the [v1] struct style in hci_def_le_evt.h. Note: the v2
// event Framing field reports only 0x00/0x01 (0x02 is RFU here), unlike the
// 3-value Framing of the CIG/BIG commands.
#define HCI_LE_CIS_ESTABLISHED_V2_EVENT 0x2A

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint8_t cigSyncDelay[HCI_LE_24BIT_FIELD_LEN];
    uint8_t cisSyncDelay[HCI_LE_24BIT_FIELD_LEN];
    uint8_t transportLatencyMToS[HCI_LE_24BIT_FIELD_LEN];
    uint8_t transportLatencySToM[HCI_LE_24BIT_FIELD_LEN];
    uint8_t phyMToS;
    uint8_t phySToM;
    uint8_t nse;
    uint8_t bnMToS;
    uint8_t bnSToM;
    uint8_t ftMToS;
    uint8_t ftSToM;
    uint16_t maxPduMToS;
    uint16_t maxPduSToM;
    uint16_t isoInterval;
    // v2-only tail (spec pp2392-2393); not significant when Status is non-zero
    uint8_t subInterval[HCI_LE_24BIT_FIELD_LEN];        // 0x000000 when NSE == 1
    uint16_t maxSduMToS;           // Max_SDU_C_To_P
    uint16_t maxSduSToM;           // Max_SDU_P_To_C
    uint8_t sduIntervalMToS[HCI_LE_24BIT_FIELD_LEN];
    uint8_t sduIntervalSToM[HCI_LE_24BIT_FIELD_LEN];
    uint8_t framing;               // 0x00 unframed / 0x01 framed (0x02 RFU here)
} HciLeCisEstablishedV2EventParam;  // 42 B, excludes Subevent_Code

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.7.65,38 LE Read All Remote Features Complete Event. Payload after the
// Subevent_Code: Status(1) + Connection_Handle(2) + Max_Remote_Page(1) +
// Max_Valid_Page(1) + LE_Features(248).
#define HCI_LE_READ_ALL_REMOTE_FEATURES_COMPLETE_EVENT 0x2B

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint8_t maxRemotePage;
    uint8_t maxValidPage;
    HciLeFeaturesAll leFeatures;   // 248 octets
} HciLeReadAllRemoteFeaturesCompleteEventParam;  // 253 B, excludes Subevent_Code

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.7.65,47 LE Monitored Advertisers Report (LE Meta subevent 0x34, 8 B payload).
// Address_Type may be an identity address (0x02/0x03) for a resolved RPA, wider
// than the Add/Remove command (0x00/0x01); payload carries no SID and no RSSI.
#define HCI_LE_MONITORED_ADVERTISERS_REPORT_EVENT 0x34

typedef struct {
    uint8_t advertiserAddressType;   // 0x00..0x03
    HciBdAddr advertiserAddress;
    uint8_t condition;               // 0x00 below-low past timeout; 0x01 RSSI >= high
} HciLeMonitoredAdvertisersReportEventParam;  // 8 B, excludes Subevent_Code

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.7.65,48 LE Frame Space Update Complete Event. Payload after the Subevent_Code:
// Status(1) + Connection_Handle(2) + Initiator(1) + Frame_Space(2) + PHYS(1) +
// Spacing_Types(2).
#define HCI_LE_FRAME_SPACE_UPDATE_COMPLETE_EVENT 0x35

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint8_t initiator;       // 0x00 Local Host / 0x01 Local Controller / 0x02 Peer
    uint16_t frameSpace;     // new frame space value in use, 0x0000-0x2710 us
    uint8_t phys;
    uint16_t spacingTypes;
} HciLeFrameSpaceUpdateCompleteEventParam;  // 9 B, excludes Subevent_Code

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.7.65,39 LE CS Read Remote Supported Capabilities Complete Event (LE Meta
// subevent 0x2C). Payload after the Subevent_Code: Status(1) + Connection_Handle(2)
// + the 28-B capability body shared with the 0x0089 return and the 0x008B
// parameter (7.8.130 pp2798-2804); the container type is in hci_def_common.h.
#define HCI_LE_CS_READ_REMOTE_SUPPORTED_CAPABILITIES_COMPLETE_EVENT 0x2C

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    HciLeCsCapabilities capabilities;  // 28 octets
} HciLeCsReadRemoteSupportedCapabilitiesCompleteEventParam;  // 31 B, excludes Subevent_Code

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.7.65,40 LE CS Read Remote FAE Table Complete Event (LE Meta subevent 0x2D).
// Payload after the Subevent_Code: Status(1) + Connection_Handle(2) +
// Remote_FAE_Table(72). Status 0x11 is the Controller's "the peer reports a zero
// FAE" answer given in place of a table (7.8.135 p2819) and is not a failure; on it
// and on every other nonzero status the 72 octets that follow carry no value, so only
// Status 0x00 holds a table. The table container is in hci_def_common.h, signed.
#define HCI_LE_CS_READ_REMOTE_FAE_TABLE_COMPLETE_EVENT 0x2D

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    int8_t remoteFaeTable[LE_CS_REMOTE_FAE_TABLE_LEN];  // 72 B, 1/32 ppm, signed
} HciLeCsReadRemoteFaeTableCompleteEventParam;  // 75 B, excludes Subevent_Code

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.7.65,41 LE CS Security Enable Complete Event (LE Meta subevent 0x2E).
// Payload after the Subevent_Code: Status(1) + Connection_Handle(2) (p2434). The
// event is emitted when the 0x008C CS Security Start procedure completes: Status
// 0 marks it done, 0x01-0xFF means the security parameter exchange failed. (Command
// -level rejections such as 0x2F never appear here; they return in Command_Status
// and are funneled into this same completion callback by the command-failure path.)
#define HCI_LE_CS_SECURITY_ENABLE_COMPLETE_EVENT 0x2E

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeCsSecurityEnableCompleteEventParam;  // 3 B, excludes Subevent_Code

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.7.65,42 LE CS Config Complete Event (LE Meta subevent 0x2F). One event closes
// both a 0x0090 Create Config and a 0x0091 Remove Config on the same Config_ID;
// Action tells the two apart. It is also generated when a CS configuration the
// peer requested has been answered by the local Controller, so it can arrive with
// no Host command outstanding. When Action is 0x00 every parameter after Action is
// ignored by the spec (p2435), so only Action 0x01 carries a usable config mirror.
#define HCI_LE_CS_CONFIG_COMPLETE_EVENT 0x2F

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint8_t configId;
    uint8_t action;                 // 0x00 removed, 0x01 created (hci_def_common.h)
    uint8_t mainModeType;           // config mirror starts here, valid when action 0x01
    uint8_t subModeType;
    uint8_t minMainModeSteps;
    uint8_t maxMainModeSteps;
    uint8_t mainModeRepetition;
    uint8_t mode0Steps;
    uint8_t role;
    uint8_t rttType;
    uint8_t csSyncPhy;
    uint8_t channelMap[LE_CS_CHANNEL_MAP_LEN];
    uint8_t channelMapRepetition;
    uint8_t channelSelectionType;
    uint8_t ch3cShape;
    uint8_t ch3cJump;
    uint8_t reserved;
    uint8_t tIp1Time;               // tail: durations actually in use, chosen by the Controller
    uint8_t tIp2Time;
    uint8_t tFcsTime;
    uint8_t tPmTime;
} HciLeCsConfigCompleteEventParam;  // 33 B, excludes Subevent_Code

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.7.65,43 LE CS Procedure Enable Complete Event (LE Meta subevent 0x30). Closes
// a 0x0094 on the Config_ID it named, and is also raised when a CS procedure the
// Controller started on its own (LL_CS_IND) ends, so it can arrive with no Host
// command outstanding. On the disable path it is held back until the pending
// subevent results and the LL_CS_TERMINATE_RSP are done (pp2442-2445). When State
// is 0x00 all the parameters after it are ignored by the spec (p2442), so State
// 0x01 is the only case that carries a usable schedule mirror.
#define HCI_LE_CS_PROCEDURE_ENABLE_COMPLETE_EVENT 0x30

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint8_t configId;
    uint8_t state;                      // 0x00 disabled, 0x01 enabled (hci_def_common.h)
    uint8_t toneAntennaConfigSelection; // schedule mirror starts here, valid when state 0x01
    int8_t selectedTxPower;             // signed dBm of the enabled procedure
    uint8_t subeventLen[HCI_LE_24BIT_FIELD_LEN];             // microseconds, 3-octet LE value
    uint8_t subeventsPerEvent;
    uint16_t subeventInterval;          // connection events
    uint16_t eventInterval;             // connection events
    uint16_t procedureInterval;         // connection events
    uint16_t procedureCount;
    uint16_t maxProcedureLen;           // N x 0.625 ms
} HciLeCsProcedureEnableCompleteEventParam;  // 21 B, excludes Subevent_Code

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.7.65,44 LE CS Subevent Result Event (LE Meta subevent 0x31). Unlike its
// siblings this payload is not fixed length: after the 15-B head below come
// Num_Steps_Reported step records, each a three-octet Step_Mode[i], Step_Channel[i],
// Step_Data_Length[i] head with that step's own Step_Data[i] immediately behind it
// (p2451). A subevent of the spec's maximum 160 steps cannot fit one HCI event, so the
// Controller spreads it over this subevent and the Continue subevent below; the CS
// engine walks the records with the received event length as the cursor bound (see the
// envelope).
// Connection_Handle 0x0FFF marks a CS test result and makes Config_ID and
// Start_ACL_Conn_Event_Counter ignored by the spec (p2446).
#define HCI_LE_CS_SUBEVENT_RESULT_EVENT 0x31

typedef struct {
    uint16_t connectionHandle;         // 0x0000-0x0EFF, or 0x0FFF for CS test
    uint8_t configId;                  // ignored when connectionHandle is 0x0FFF
    uint16_t startAclConnEventCounter; // ignored when connectionHandle is 0x0FFF
    uint16_t procedureCounter;
    uint16_t frequencyCompensation;    // 0.01 ppm, signed; 0xC000 not available
    uint8_t referencePowerLevel;       // dBm, -127..20; 0x7F not applicable
    uint8_t procedureDoneStatus;       // bits 0-3: 0x0 complete, 0x1 partial, 0xF aborted
    uint8_t subeventDoneStatus;        // bits 0-3: 0x0 complete, 0x1 partial, 0xF aborted
    uint8_t abortReason;               // bits 0-3 procedure, bits 4-7 subevent
    uint8_t numAntennaPaths;           // 0x00 no phase measurement, 0x01-0x04 paths
    uint8_t numStepsReported;          // 0x00-0xA0
} HciLeCsSubeventResultEventParam;  // 15 B, excludes Subevent_Code

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.7.65,45 LE CS Subevent Result Continue Event (LE Meta subevent 0x32). Carries
// the same step tail as 0x31 behind an 8-B head: the four fields only the opening
// 0x31 has (Start_ACL_Conn_Event_Counter, Procedure_Counter, Frequency_Compensation,
// Reference_Power_Level) are dropped, and the continued steps keep the procedure
// counter the opening 0x31 reported. Connection_Handle 0x0FFF is CS test here too
// and Config_ID is ignored with it (p2459).
#define HCI_LE_CS_SUBEVENT_RESULT_CONTINUE_EVENT 0x32

typedef struct {
    uint16_t connectionHandle;         // 0x0000-0x0EFF, or 0x0FFF for CS test
    uint8_t configId;                  // ignored when connectionHandle is 0x0FFF
    uint8_t procedureDoneStatus;
    uint8_t subeventDoneStatus;
    uint8_t abortReason;
    uint8_t numAntennaPaths;
    uint8_t numStepsReported;          // 0x00-0xA0
} HciLeCsSubeventResultContinueEventParam;  // 8 B, excludes Subevent_Code

// Callback-side envelopes for the two variable-tail subevents above. These are not
// wire layouts: the HCI handler copies the fixed head and leaves the tail as a view
// into the received buffer, because stepBlob has no fixed accessor in the shared
// callback shape and the tail's own width is known only from numStepsReported and
// the running total of Step_Data_Length. stepBlob therefore aliases the event buffer
// and is valid only for the synchronous callback, so the consumer must finish parsing
// into its own records before returning.
typedef struct {
    HciLeCsSubeventResultEventParam head;
    const uint8_t *stepBlob;
    uint16_t stepBlobLen;
} HciLeCsSubeventResultEnvelope;

typedef struct {
    HciLeCsSubeventResultContinueEventParam head;
    const uint8_t *stepBlob;
    uint16_t stepBlobLen;
} HciLeCsSubeventResultContinueEnvelope;

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.7.65,46 LE CS Test End Complete Event (LE Meta subevent 0x33). Status 0x00 is a
// successful stop and 0x01-0xFF a failed one (p2463). The Controller sends it only after
// every pending test result event has been reported, so it closes a CS test the way a
// procedure-complete status closes a 0x31/0x32 run. Subevent_Code is stripped by the
// dispatcher, so the callback parameter is this Status alone.
#define HCI_LE_CS_TEST_END_COMPLETE_EVENT 0x33

typedef struct {
    uint8_t status;
} HciLeCsTestEndCompleteEventParam;  // 1 B, excludes Subevent_Code

#pragma pack(0)

#ifdef __cplusplus
}
#endif

#endif
