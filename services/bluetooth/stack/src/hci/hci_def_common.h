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

#ifndef HCI_DEF_COMMON_H
#define HCI_DEF_COMMON_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

typedef struct {
    uint8_t status;
} HciStatusParam;

#define BD_ADDR_LENGTH 6

typedef struct {
    uint8_t raw[BD_ADDR_LENGTH];
} HciBdAddr;

#define LMP_FEATURES_SIZE 8

typedef struct {
    uint8_t raw[LMP_FEATURES_SIZE];
} HciLmpFeatures;

#define MAX_EXTENED_FEATURES_PAGE_NUMBER 2
#define MAX_EXTENED_FEATURES_PAGE_COUNT (MAX_EXTENED_FEATURES_PAGE_NUMBER + 1)

typedef struct {
    uint8_t page[MAX_EXTENED_FEATURES_PAGE_COUNT][LMP_FEATURES_SIZE];
} HciExtendedLmpFeatures;

#define LE_FEATURES_SIZE 8

typedef struct {
    uint8_t raw[LE_FEATURES_SIZE];
} HciLeFeatures;

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 6, Part B, 4.6
// LE FeatureSet extended to 1984 bits: page 0 (octets 0-7, 8 bytes) plus
// pages 1..10 (24 bytes each, page 1 starts at octet 8). HciLeFeaturesAll keeps
// HciLeFeatures page 0 as a byte-identical prefix.
#define LE_FEATURES_PAGE0_SIZE 8
#define LE_FEATURES_PAGE_SIZE 24
#define LE_EXTENDED_FEATURE_PAGE_COUNT 10
#define LE_MAX_FEATURE_PAGE_NUMBER 0x0A
#define LE_FEATURES_SIZE_ALL \
    (LE_FEATURES_PAGE0_SIZE + LE_EXTENDED_FEATURE_PAGE_COUNT * LE_FEATURES_PAGE_SIZE)  // 248

typedef struct {
    uint8_t raw[LE_FEATURES_SIZE_ALL];
} HciLeFeaturesAll;

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// LE Channel Sounding capability body: 20 fields, 28 octets (7.8.130 pp2798-2804).
// The same field order appears on three wire sites, so it lives here as a shared
// packed container embedded by value in the 0x0089 return, the 0x008B parameter
// (hci_def_le_cmd_6_0.h) and the 0x2C event (hci_def_le_evt_6_0.h).
typedef struct {
    uint8_t numConfigSupported;
    uint16_t maxConsecutiveProceduresSupported;
    uint8_t numAntennasSupported;
    uint8_t maxAntennaPathsSupported;
    uint8_t rolesSupported;
    uint8_t modesSupported;
    uint8_t rttCapability;
    uint8_t rttAaOnlyN;
    uint8_t rttSoundingN;
    uint8_t rttRandomPayloadN;
    uint16_t nadmSoundingCapability;
    uint16_t nadmRandomCapability;
    uint8_t csSyncPhysSupported;
    uint16_t subfeaturesSupported;
    uint16_t tIp1TimesSupported;
    uint16_t tIp2TimesSupported;
    uint16_t tFcsTimesSupported;
    uint16_t tPmTimesSupported;
    uint8_t tSwTimeSupported;
    uint8_t txSnrCapability;
} HciLeCsCapabilities;  // 28 octets

// CS Channel_Map: 10 octets = 80 one-bit fields (7.8.137 p2826). The nth field
// (n in 0..78) is channel n; n = 0, 1, 23, 24, 25, 77 and 78 are reserved and
// shall be set to zero, at least 15 channels shall be enabled, and bit 79 is RFU.
// Shared by the 0x0090 parameter, the 0x0092 parameter and the 0x2F event.
#define LE_CS_CHANNEL_MAP_LEN 10
#define LE_CS_CHANNEL_MAP_MIN_CHANNELS 15

// Remote_FAE_Table: 72 octets, the per-channel mode-0 Frequency Actuation Error
// table of a Controller (7.8.135 p2819 / 7.8.136 p2821). One octet per allowed CS
// channel, the reserved channels left out and the rest shifted down to close the
// gaps, so 72 = the 79 channels 0-78 minus the 7 the Channel_Map above also
// reserves. Shared by the 0x008F parameter and the 0x2D event, and it is the element
// type of the array that matters: each octet is an 8-bit signed value in 1/32 ppm
// units (-128 = -4 ppm, 127 = +3.96875 ppm, 0 = -0.015625..+0.015625 ppm) per
// [Vol 6] Part B 2.4.2,52 p3008, so int8_t, not uint8_t. Both commands cross-
// reference this table as "[Vol 6] Part B 2.4.2,53"; that section is
// LL_CS_CHANNEL_MAP_IND, and the table is the ChFAE field of LL_CS_FAE_RSP in
// 2.4.2,52.
#define LE_CS_REMOTE_FAE_TABLE_LEN 72

// HCI LE wire fields that encode a 24-bit value as three octets (Subevent_Len,
// CIG_Sync_Delay, CIS_Sync_Delay, SDU_Interval, etc). Shared by command and event structs.
#define HCI_LE_24BIT_FIELD_LEN 3

// LE CS Config Complete event Action field (7.7.65,42 p2437). When Action is
// LE_CS_CONFIG_ACTION_REMOVED the spec ignores every remaining event parameter.
#define LE_CS_CONFIG_ACTION_REMOVED 0x00
#define LE_CS_CONFIG_ACTION_CREATED 0x01

// LE CS Procedure Enable command Enable field (7.8.141 p2838) and LE CS Procedure
// Enable Complete event State field (7.7.65,43 p2442) share this 0x00/0x01 pair.
// The codes are shared, the values are not echoed: State reports the state that
// stands (p2444 "CS procedures are disabled" / "CS procedures are enabled"), not
// the Enable that was asked for.
#define LE_CS_PROCEDURE_DISABLED 0x00
#define LE_CS_PROCEDURE_ENABLED 0x01

// LE CS Procedure Enable Complete event Selected_TX_Power "no value" code
// (7.7.65,43 p2444). The field is signed dBm over -127 to 20, so 0 is a real level
// (0 dBm) and only this code means "Transmit power level is unavailable".
#define LE_CS_SELECTED_TX_POWER_UNAVAILABLE 0x7F

// No connection: the same value cs.c's CS_HANDLE_NONE and the HCI command-failure
// rows already use for "not attributable to a link". Outside the valid Connection_Handle
// range 0x0000-0x0EFF, so it can never collide with a live handle.
#define LE_CS_INVALID_HANDLE 0xFFFF

// CS test: the Connection_Handle an LE CS Test run reports its results under
// (7.7.65,44 p2446 and 7.7.65,45 p2459, where it also makes Config_ID and, on 0x31,
// Start_ACL_Conn_Event_Counter ignored; the same value the 0x008F parameter table
// offers as "Use for CS Test command"). Outside the valid Connection_Handle range
// 0x0000-0x0EFF, so a test result can never be read as a link's - which is also why
// the CS engine must divert it before any connection lookup.
#define LE_CS_TEST_HANDLE 0x0FFF

#pragma pack(0)

#ifdef __cplusplus
}
#endif

#endif
