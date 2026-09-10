/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
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

#ifndef HCI_DEF_LE_CMD_5_4_H
#define HCI_DEF_LE_CMD_5_4_H

#include <stdint.h>

// Included first so HCI_COMMAND_OGF_LE_CONTROLLER, MAKE_OPCODE and the shared
// HciStatusParam types are available. hci_def_le_cmd.h re-includes this header
// at its tail; the include guard turns that into a no-op.
#include "hci_def_le_cmd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.53 LE Set Extended Advertising Parameters Command [v2]
// Same command as OCF 0x0036 plus Primary_Advertising_PHY_Options /
// Secondary_Advertising_PHY_Options (Advertising Coding Selection, Vol 6
// Part B 4.6.37). The [v2] opcode is used only when the Controller reports the
// LE Feature Advertising Coding Selection (bit 40); otherwise the Host keeps
// using the [v1] command with both options implicitly 0x00. A Controller
// without the feature that receives a non-zero option returns Unsupported
// Feature or Parameter Value (0x11) - the Host-side gate is
// BTM_IsControllerSupportLeAdvCodingSel (btm_controller.c).
#define HCI_LE_SET_EXTENDED_ADVERTISING_PARAMETERS_V2 \
    MAKE_OPCODE(0x007F, HCI_COMMAND_OGF_LE_CONTROLLER)

// Advertising Coding Selection coding preferences (Primary/Secondary_
// Advertising_PHY_Options, 7.8.53). 0x05-0xFF are reserved for future use.
// The options are ignored by the Controller when the corresponding PHY is not
// the LE Coded PHY, and a "require" option (0x03/0x04) that cannot be satisfied
// returns Command Disallowed (0x0C).
#define LE_EXT_ADV_PHY_OPTIONS_NONE 0x00
#define LE_EXT_ADV_PHY_OPTIONS_PREFER_S2 0x01
#define LE_EXT_ADV_PHY_OPTIONS_PREFER_S8 0x02
#define LE_EXT_ADV_PHY_OPTIONS_REQUIRE_S2 0x03
#define LE_EXT_ADV_PHY_OPTIONS_REQUIRE_S8 0x04
#define LE_EXT_ADV_PHY_OPTIONS_MAX 0x04

typedef struct {
    uint8_t advertisingHandle;
    uint16_t advertisingEventProperties;
    uint8_t priAdvertisingIntervalMin[3];
    uint8_t priAdvertisingIntervalMax[3];
    uint8_t priAdvertisingChannelMap;
    uint8_t ownAddressType;
    uint8_t peerAddressType;
    uint8_t peerAddress[6];
    uint8_t advertisingFilterPolicy;
    uint8_t advertisingTxPower;
    uint8_t priAdvertisingPHY;
    uint8_t secondaryAdvertisingMaxSkip;
    uint8_t secondaryAdvertisingPHY;
    uint8_t advertisingSID;
    uint8_t scanRequestNotificationEnable;
    // Advertising Coding Selection options: the [v1] parameter list (OCF
    // 0x0036, see HciLeSetExtendedAdvertisingParametersParam) plus the two
    // appended octets; keep the field order above in sync with the [v1] struct.
    uint8_t primaryAdvertisingPhyOptions;
    uint8_t secondaryAdvertisingPhyOptions;
} HciLeSetExtendedAdvertisingParametersV2Param;

// The [v2] return parameters are unchanged from [v1] (Status +
// Selected_TX_Power): HciLeSetExtendedAdvertisingParametersReturnParam
// (hci_def_le_cmd_5_0.h) is reused for the completion callbacks.

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.125 LE Set Periodic Advertising Subevent Data Command
// 5.4 PAwR (periodic advertising with responses) advertiser command: sets the
// data for one or more subevents of a PAwR train, in reply to the HCI_LE_
// Periodic_Advertising_Subevent_Data_Request event (0x27). The data for a
// subevent shall be transmitted only once and is then discarded by the
// Controller. Error codes on completion: 0x42 (advertising set does not
// exist), 0x0C (subevent outside the range requested by the 0x27 event),
// 0x45 (combined data too long for the current subevent interval - all data
// discarded), 0x46/0x47 (subevent already passed / too early to transmit -
// data discarded). On the LE Coded PHY the Controller assumes S=8 coding
// unless the current advertising parameters require S=2.
#define HCI_LE_SET_PERIODIC_ADVERTISING_SUBEVENT_DATA \
    MAKE_OPCODE(0x0082, HCI_COMMAND_OGF_LE_CONTROLLER)

// One subevent's data block (Subevent[i]/Response_Slot_Start[i]/
// Response_Slot_Count[i]/Subevent_Data_Length[i]/Subevent_Data[i], 7.8.125).
typedef struct {
    uint8_t subevent;            // Subevent whose data is being set: 0x00-0x7F
    uint8_t responseSlotStart;   // First response slot to be used in this subevent
    uint8_t responseSlotCount;   // Number of response slots to be used in this subevent
    uint8_t subeventDataLength;  // 0-251; 252-0xFF are reserved for future use
    // Data to transmit once in this subevent (valid only for the duration of
    // the call when subeventDataLength > 0).
    const uint8_t *subeventData;
} HciLeSetPeriodicAdvertisingSubeventDataSet;

typedef struct {
    uint8_t advertisingHandle; // Periodic advertising train: 0x00-0xEF
    uint8_t numSubevents;      // 0x01-0x0F; all other values reserved
    // sets must point to an array with one entry per subevent, in ascending
    // Subevent order. There is no length field in the command; the number of
    // entries is numSubevents.
    const HciLeSetPeriodicAdvertisingSubeventDataSet *sets;
} HciLeSetPeriodicAdvertisingSubeventDataParam;

typedef struct {
    uint8_t status;
    uint8_t advertisingHandle; // Echo of the command parameter (7.8.125)
} HciLeSetPeriodicAdvertisingSubeventDataReturnParam;

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.126 LE Set Periodic Advertising Response Data Command
// PAwR sync-side: sets the data for one response slot of one subevent.
// Request_Event (the paEventCounter of the packet responded to) and
// Request_Subevent point back to the periodic advertising packet that the
// response answers; Response_Subevent and Response_Slot name the slot the
// data is actually transmitted in and may differ from the packet that
// triggered the response. The data shall be transmitted only once. Error
// codes on completion: 0x45 (data too long for the response slot - discarded),
// 0x46 (response slot already passed - discarded).
#define HCI_LE_SET_PERIODIC_ADVERTISING_RESPONSE_DATA \
    MAKE_OPCODE(0x0083, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t syncHandle;        // Sync_Handle identifying the PAwR train: 0x0000-0x0EFF
    uint16_t requestEvent;      // paEventCounter of the packet being responded to
    uint8_t requestSubevent;    // Subevent the responded-to packet was received in
    uint8_t responseSubevent;   // Subevent the response is sent in: 0x00-0x7F
    uint8_t responseSlot;       // Response slot of the response subevent: 0x00-0xFF
    uint8_t responseDataLength; // 0-251; 252-0xFF are reserved for future use
    // Data to transmit once in the response slot (valid only for the duration
    // of the call when responseDataLength > 0).
    const uint8_t *responseData;
} HciLeSetPeriodicAdvertisingResponseDataParam;

typedef struct {
    uint8_t status;
    uint16_t syncHandle; // Echo of the command parameter (7.8.126)
} HciLeSetPeriodicAdvertisingResponseDataReturnParam;

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.127 LE Set Periodic Sync Subevent Command
// Restricts the synchronization with a PAwR train to a subset of its
// subevents: the Controller keeps listening only to those subevents and stops
// synchronizing any subevent that is not part of the subset. The
// Periodic_Advertising_Properties bit 6 asks the Controller to include
// TxPower in the AUX_SYNC_SUBEVENT_RSP PDUs it transmits.
#define HCI_LE_SET_PERIODIC_SYNC_SUBEVENT \
    MAKE_OPCODE(0x0084, HCI_COMMAND_OGF_LE_CONTROLLER)

#define LE_SET_PERIODIC_SYNC_SUBEVENT_PROPERTIES_TX_POWER 0x0040

typedef struct {
    uint16_t syncHandle;                  // Sync_Handle identifying the PAwR train: 0x0000-0x0EFF
    uint16_t periodicAdvertisingProperties; // Bit 6: include TxPower in AUX_SYNC_SUBEVENT_RSP; other bits reserved
    uint8_t numSubevents;                 // 0x01-0x80
    // subevents must point to an array with one subevent per entry
    // (0x00-0x7F each). There is no length field in the command; the number of
    // entries is numSubevents.
    const uint8_t *subevents;
} HciLeSetPeriodicSyncSubeventParam;

typedef struct {
    uint8_t status;
    uint16_t syncHandle; // Echo of the command parameter (7.8.127)
} HciLeSetPeriodicSyncSubeventReturnParam;

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.66 LE Extended Create Connection Command [v2]
// Same command as OCF 0x0043 plus the leading Advertising_Handle and Subevent
// parameters that create an ACL connection from a PAwR train: the connection
// request is initiated from the identified subevent of the train. When both
// parameters are 0xFF they are not used and the command behaves like [v1];
// a 0xFF on exactly one of the two is invalid. When both are valid the
// Controller ignores Initiator_Filter_Policy, Initiating_PHYs, Scan_Interval
// and Scan_Window (the connection parameter sets still apply, selected by the
// PHY of the train). No return parameters: the outcome arrives as the LE
// Enhanced Connection Complete [v2] event (0x29).
#define HCI_LE_EXTENDED_CREATE_CONNECTION_V2 \
    MAKE_OPCODE(0x0085, HCI_COMMAND_OGF_LE_CONTROLLER)

// Same arrayed-parameters contract as the [v1] command (OCF 0x0043): sets
// must point to one HciLeConnectionParamSet per set bit in initiatingPhys, in
// ascending PHY bit order.
typedef struct {
    uint8_t advertisingHandle; // Periodic advertising train to connect from: 0x00-0xEF, or 0xFF when not used
    uint8_t subevent;          // Subevent the connection request is initiated from: 0x00-0x7F, or 0xFF when not used
    uint8_t initiatingFilterPolicy; // Same encoding as the [v1] command
    uint8_t ownAddressType;         // Same encoding as the [v1] command
    uint8_t peerAddressType;        // Same encoding as the [v1] command
    HciBdAddr peerAddress;          // Same encoding as the [v1] command
    uint8_t initiatingPhys;         // Same encoding as the [v1] command
    HciLeConnectionParamSet *sets;  // Same contract as the [v1] command
} HciLeExtendedCreateConnectionV2Param;

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.61 LE Set Periodic Advertising Parameters Command [v2]
// Same command as OCF 0x003E plus the trailing PAwR timing parameters. A
// Num_Subevents of 0x00 turns the train into a plain periodic advertising
// train and the Controller ignores Subevent_Interval, Response_Slot_Delay,
// Response_Slot_Spacing and Num_Response_Slots; a Num_Response_Slots of 0x00
// means the train has no response slots and the Controller ignores
// Response_Slot_Delay and Response_Slot_Spacing. Error codes on completion
// include 0x0C (the [v1]-side check: periodic advertising already enabled)
// and the [v2] timing rules are checked by the Controller against the
// advertising interval.
#define HCI_LE_SET_PERIODIC_ADVERTISING_PARAMETERS_V2 \
    MAKE_OPCODE(0x0086, HCI_COMMAND_OGF_LE_CONTROLLER)

// The [v2] periodic advertising properties keep the [v1] encoding: only bit 6
// (include TxPower in the advertising PDU) is defined. The timing units of the
// 5.4 PAwR tail follow the 7.8.61 [v2] field table:
//   Subevent_Interval:      N x 1.25 ms, 0x06-0xFF (7.5 ms - 318.75 ms)
//   Response_Slot_Delay:    0x00 none, else N x 1.25 ms, 0x01-0xFE
//   Response_Slot_Spacing:  0x00 none, else N x 0.125 ms, 0x02-0xFF
//   Num_Response_Slots:     0x00 none, else 0x01-0xFF
typedef struct {
    uint8_t advertisingHandle; // Periodic advertising train: 0x00-0xEF
    uint16_t periodicAdvertisingIntervalMin; // 0x0006-0xFFFF, N x 1.25 ms
    uint16_t periodicAdvertisingIntervalMax; // 0x0006-0xFFFF, N x 1.25 ms
    uint16_t periodicAdvertisingProperties;  // Bit 6: include TxPower; other bits reserved
    // 5.4 PAwR tail; keep the order above in sync with the [v1] struct
    // (HciLeSetPeriodicAdvertisingParametersParam, hci_def_le_cmd_5_0.h).
    uint8_t numSubevents;        // 0x00-0x80; 0x00 = plain periodic advertising train
    uint8_t subeventInterval;    // 0x06-0xFF; ignored when numSubevents is 0x00
    uint8_t responseSlotDelay;   // 0x00 or 0x01-0xFE; ignored when numSubevents is 0x00
    uint8_t responseSlotSpacing; // 0x00 or 0x02-0xFF; ignored when numSubevents is 0x00
    uint8_t numResponseSlots;    // 0x00 or 0x01-0xFF
} HciLeSetPeriodicAdvertisingParametersV2Param;

typedef struct {
    uint8_t status;
    uint8_t advertisingHandle; // Echo of the command parameter (7.8.61 [v2])
} HciLeSetPeriodicAdvertisingParametersV2ReturnParam;

#pragma pack(0)

#ifdef __cplusplus
}
#endif

#endif
