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

#ifndef HCI_DEF_LE_CMD_5_1_H
#define HCI_DEF_LE_CMD_5_1_H

#include <stdint.h>

// Included first so HCI_COMMAND_OGF_LE_CONTROLLER, MAKE_OPCODE and the shared
// HciStatusParam/HciBdAddr types are available. hci_def_le_cmd.h re-includes
// this header at its tail; the include guard turns that into a no-op.
#include "hci_def_le_cmd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

// Common 5.1 LE constants (Direction Finding, Vol 6 Part B 2.5 / Vol 2 Part E)
#define HCI_LE_CTE_TYPE_AOA 0x00
#define HCI_LE_CTE_TYPE_AOD_1US 0x01
#define HCI_LE_CTE_TYPE_AOD_2US 0x02
#define HCI_LE_CTE_TYPE_NONE 0xFF

#define HCI_LE_CTE_LENGTH_MIN 0x02  // 8 us units
#define HCI_LE_CTE_LENGTH_MAX 0x14

#define HCI_LE_CTE_COUNT_MIN 0x01
#define HCI_LE_CTE_COUNT_MAX 0x10

#define HCI_LE_SWITCHING_PATTERN_LENGTH_MIN 0x02
#define HCI_LE_SWITCHING_PATTERN_LENGTH_MAX 0x4B

#define HCI_LE_CTE_SLOT_DURATIONS_1US 0x01
#define HCI_LE_CTE_SLOT_DURATIONS_2US 0x02

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.78 LE Receiver Test Command [v3]
#define HCI_LE_RECEIVER_TEST_V3 MAKE_OPCODE(0x004F, HCI_COMMAND_OGF_LE_CONTROLLER)

#define HCI_LE_RX_TEST_MODULATION_STANDARD 0x00
#define HCI_LE_RX_TEST_MODULATION_STABLE 0x01

// Caution: contains a pointer; must be serialized manually, never passed to
// HciAllocCmd directly (same convention as HciLeSetExtendedAdvertisingDataParam).
typedef struct {
    uint8_t rxChannel;
    uint8_t phy;
    uint8_t modulationIndex;
    uint8_t expectedCteLength;
    uint8_t expectedCteType;
    uint8_t slotDurations;
    uint8_t lengthOfSwitchingPattern;
    const uint8_t *antennaIds;  // lengthOfSwitchingPattern entries
} HciLeReceiverTestV3Param;

typedef HciStatusParam HciLeReceiverTestV3ReturnParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.79 LE Transmitter Test Command [v3]
#define HCI_LE_TRANSMITTER_TEST_V3 MAKE_OPCODE(0x0050, HCI_COMMAND_OGF_LE_CONTROLLER)

// Packet_Payload values
#define HCI_LE_TX_TEST_PAYLOAD_PRBS9 0x00
#define HCI_LE_TX_TEST_PAYLOAD_11110000 0x01
#define HCI_LE_TX_TEST_PAYLOAD_10101010 0x02
#define HCI_LE_TX_TEST_PAYLOAD_PRBS15 0x03
#define HCI_LE_TX_TEST_PAYLOAD_11111111 0x04
#define HCI_LE_TX_TEST_PAYLOAD_00000000 0x05
#define HCI_LE_TX_TEST_PAYLOAD_00001111 0x06
#define HCI_LE_TX_TEST_PAYLOAD_01010101 0x07

typedef struct {
    uint8_t txChannel;
    uint8_t lengthOfTestData;
    uint8_t packetPayload;
    uint8_t phy;  // 0x01 LE 1M / 0x02 LE 2M / 0x03 Coded S8 / 0x04 Coded S2
    uint8_t cteLength;
    uint8_t cteType;
    uint8_t lengthOfSwitchingPattern;
    const uint8_t *antennaIds;  // lengthOfSwitchingPattern entries
} HciLeTransmitterTestV3Param;

typedef HciStatusParam HciLeTransmitterTestV3ReturnParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.80 LE Set Connectionless CTE Transmit Parameters Command
#define HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_PARAMETERS MAKE_OPCODE(0x0051, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t advertisingHandle;  // Range 0x00-0xEF
    uint8_t cteLength;
    uint8_t cteType;
    uint8_t cteCount;
    uint8_t lengthOfSwitchingPattern;
    const uint8_t *antennaIds;  // lengthOfSwitchingPattern entries
} HciLeSetConnectionlessCteTransmitParametersParam;

typedef HciStatusParam HciLeSetConnectionlessCteTransmitParametersReturnParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.81 LE Set Connectionless CTE Transmit Enable Command
#define HCI_LE_SET_CONNECTIONLESS_CTE_TRANSMIT_ENABLE MAKE_OPCODE(0x0052, HCI_COMMAND_OGF_LE_CONTROLLER)

#define HCI_LE_CTE_ENABLE_DISABLE 0x00
#define HCI_LE_CTE_ENABLE_ENABLE 0x01

typedef struct {
    uint8_t advertisingHandle;  // Range 0x00-0xEF
    uint8_t cteEnable;
} HciLeSetConnectionlessCteTransmitEnableParam;

typedef HciStatusParam HciLeSetConnectionlessCteTransmitEnableReturnParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.82 LE Set Connectionless IQ Sampling Enable Command
#define HCI_LE_SET_CONNECTIONLESS_IQ_SAMPLING_ENABLE MAKE_OPCODE(0x0053, HCI_COMMAND_OGF_LE_CONTROLLER)

#define HCI_LE_MAX_SAMPLED_CTES_ALL 0x00
#define HCI_LE_MAX_SAMPLED_CTES_MAX 0x10

typedef struct {
    uint16_t syncHandle;  // Range 0x0000-0x0EFF; 0x0FFF = Receiver Test
    uint8_t samplingEnable;
    uint8_t slotDurations;
    uint8_t maxSampledCtes;
    uint8_t lengthOfSwitchingPattern;
    const uint8_t *antennaIds;  // lengthOfSwitchingPattern entries
} HciLeSetConnectionlessIqSamplingEnableParam;

typedef struct {
    uint8_t status;
    uint16_t syncHandle;
} HciLeSetConnectionlessIqSamplingEnableReturnParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.83 LE Set Connection CTE Receive Parameters Command
#define HCI_LE_SET_CONNECTION_CTE_RECEIVE_PARAMETERS MAKE_OPCODE(0x0054, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint8_t samplingEnable;
    uint8_t slotDurations;
    uint8_t lengthOfSwitchingPattern;
    const uint8_t *antennaIds;  // lengthOfSwitchingPattern entries
} HciLeSetConnectionCteReceiveParametersParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeSetConnectionCteReceiveParametersReturnParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.84 LE Set Connection CTE Transmit Parameters Command
#define HCI_LE_SET_CONNECTION_CTE_TRANSMIT_PARAMETERS MAKE_OPCODE(0x0055, HCI_COMMAND_OGF_LE_CONTROLLER)

// CTE_Types bits
#define HCI_LE_CTE_TYPES_AOA (0x01 << 0)
#define HCI_LE_CTE_TYPES_AOD_1US (0x01 << 1)
#define HCI_LE_CTE_TYPES_AOD_2US (0x01 << 2)
#define HCI_LE_CTE_TYPES_VALID_MASK (HCI_LE_CTE_TYPES_AOA | HCI_LE_CTE_TYPES_AOD_1US | HCI_LE_CTE_TYPES_AOD_2US)

typedef struct {
    uint16_t connectionHandle;
    uint8_t cteTypes;
    uint8_t lengthOfSwitchingPattern;
    const uint8_t *antennaIds;  // lengthOfSwitchingPattern entries
} HciLeSetConnectionCteTransmitParametersParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeSetConnectionCteTransmitParametersReturnParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.85 LE Connection CTE Request Enable Command
#define HCI_LE_CONNECTION_CTE_REQUEST_ENABLE MAKE_OPCODE(0x0056, HCI_COMMAND_OGF_LE_CONTROLLER)

#define HCI_LE_CTE_REQUEST_INTERVAL_ONCE 0x0000

typedef struct {
    uint16_t connectionHandle;
    uint8_t enable;
    uint16_t cteRequestInterval;  // 0x0000 = request once, else number of connection events
    uint8_t requestedCteLength;
    uint8_t requestedCteType;
} HciLeConnectionCteRequestEnableParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeConnectionCteRequestEnableReturnParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.86 LE Connection CTE Response Enable Command
#define HCI_LE_CONNECTION_CTE_RESPONSE_ENABLE MAKE_OPCODE(0x0057, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint8_t enable;
} HciLeConnectionCteResponseEnableParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeConnectionCteResponseEnableReturnParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.87 LE Read Antenna Information Command
#define HCI_LE_READ_ANTENNA_INFORMATION MAKE_OPCODE(0x0058, HCI_COMMAND_OGF_LE_CONTROLLER)

// Supported_Switching_Sampling_Rates bits
#define HCI_LE_SWITCHING_RATE_1US_AOD_TX (0x01 << 0)
#define HCI_LE_SAMPLING_RATE_1US_AOD_RX (0x01 << 1)
#define HCI_LE_SWITCHING_SAMPLING_1US_AOA_RX (0x01 << 2)

typedef struct {
    uint8_t status;
    uint8_t supportedSwitchingSamplingRates;
    uint8_t numberOfAntennae;
    uint8_t maxLengthOfSwitchingPattern;
    uint8_t maxCteLength;
} HciLeReadAntennaInformationReturnParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.88 LE Set Periodic Advertising Receive Enable Command
#define HCI_LE_SET_PERIODIC_ADVERTISING_RECEIVE_ENABLE MAKE_OPCODE(0x0059, HCI_COMMAND_OGF_LE_CONTROLLER)

#define HCI_LE_PERIODIC_ADV_RECEIVE_ENABLE_DISABLE 0x00
#define HCI_LE_PERIODIC_ADV_RECEIVE_ENABLE_ENABLE 0x01

typedef struct {
    uint16_t syncHandle;
    uint8_t enable;
} HciLeSetPeriodicAdvertisingReceiveEnableParam;

typedef HciStatusParam HciLeSetPeriodicAdvertisingReceiveEnableReturnParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.89 LE Periodic Advertising Sync Transfer Command
#define HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER MAKE_OPCODE(0x005A, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint16_t serviceData;
    uint16_t syncHandle;  // Range 0x0000-0x0EFF
} HciLePeriodicAdvertisingSyncTransferParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLePeriodicAdvertisingSyncTransferReturnParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.90 LE Periodic Advertising Set Info Transfer Command
#define HCI_LE_PERIODIC_ADVERTISING_SET_INFO_TRANSFER MAKE_OPCODE(0x005B, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint16_t serviceData;
    uint8_t advertisingHandle;  // Range 0x00-0xEF
} HciLePeriodicAdvertisingSetInfoTransferParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLePeriodicAdvertisingSetInfoTransferReturnParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.91 LE Set Periodic Advertising Sync Transfer Parameters Command
#define HCI_LE_SET_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS MAKE_OPCODE(0x005C, HCI_COMMAND_OGF_LE_CONTROLLER)

// Mode values
#define HCI_LE_PAST_MODE_NO_SYNC 0x00
#define HCI_LE_PAST_MODE_SYNC_REPORT_DISABLED 0x01
#define HCI_LE_PAST_MODE_SYNC_REPORT_ENABLED 0x02

// CTE_Type bits
#define HCI_LE_PAST_CTE_TYPE_NO_AOA (0x01 << 0)
#define HCI_LE_PAST_CTE_TYPE_NO_AOD_1US (0x01 << 1)
#define HCI_LE_PAST_CTE_TYPE_NO_AOD_2US (0x01 << 2)
#define HCI_LE_PAST_CTE_TYPE_NO_CTE (0x01 << 4)

typedef struct {
    uint16_t connectionHandle;
    uint8_t mode;
    uint16_t skip;         // Range 0x0000-0x01F3
    uint16_t syncTimeout;  // Range 0x000A-0x4000
    uint8_t cteType;
} HciLeSetPeriodicAdvertisingSyncTransferParametersParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeSetPeriodicAdvertisingSyncTransferParametersReturnParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.92 LE Set Default Periodic Advertising Sync Transfer Parameters Command
#define HCI_LE_SET_DEFAULT_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS MAKE_OPCODE(0x005D, \
    HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t mode;
    uint16_t skip;         // Range 0x0000-0x01F3
    uint16_t syncTimeout;  // Range 0x000A-0x4000
    uint8_t cteType;
} HciLeSetDefaultPeriodicAdvertisingSyncTransferParametersParam;

typedef HciStatusParam HciLeSetDefaultPeriodicAdvertisingSyncTransferParametersReturnParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.93 LE Generate DHKey Command [v2]
#define HCI_LE_GENERATE_DHKEY_V2 MAKE_OPCODE(0x005E, HCI_COMMAND_OGF_LE_CONTROLLER)

// Key_Type values
#define HCI_LE_DHKEY_KEY_TYPE_GENERATE 0x00  // Generate DHKey using the private key generated with P-256
#define HCI_LE_DHKEY_KEY_TYPE_DEBUG 0x01  // Generate DHKey using the debug private key (Vol 3, Part H, 2.3.5,6,1)

typedef struct {
    uint8_t remoteP256PublicKey[64];
    uint8_t keyType;
} HciLeGenerateDhKeyV2Param;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.94 LE Modify Sleep Clock Accuracy Command
#define HCI_LE_MODIFY_SLEEP_CLOCK_ACCURACY MAKE_OPCODE(0x005F, HCI_COMMAND_OGF_LE_CONTROLLER)

// Action values
#define HCI_LE_SLEEP_CLOCK_ACCURACY_MORE 0x00  // Switch to a more accurate clock
#define HCI_LE_SLEEP_CLOCK_ACCURACY_LESS 0x01  // Switch to a less accurate clock

typedef struct {
    uint8_t action;
} HciLeModifySleepClockAccuracyParam;

typedef HciStatusParam HciLeModifySleepClockAccuracyReturnParam;

#pragma pack(0)

#ifdef __cplusplus
}
#endif

#endif
