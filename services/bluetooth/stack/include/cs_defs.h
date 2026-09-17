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

#ifndef BTSTACK_CS_DEFS_H
#define BTSTACK_CS_DEFS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Normalized Channel Sounding measurement types. They are deliberately free of the
// HCI wire structures in hci/hci_def_le_evt_6_0.h: the CS engine decodes the variable
// step payload of the 0x31/0x32 subevents into these records once, and everything
// above the engine (the CSIF_ API of cs_le_if.h, a ranging algorithm) reads them
// without including hci/. This header is installed, and is included by both the engine
// and that API. One CsSubeventReport is one CS subevent: the steps reported by the
// opening 0x31 and, when that event did not finish the subevent, by the 0x32 events
// continuing it. A CS subevent holds at most N_MAX_STEPS_PER_SUBEVENT = 160 steps
// ([Vol 6] Part B, Section 4.5.18,1, printed p3132), which is also the per-event
// ceiling of Num_Steps_Reported 0x00-0xA0 (7.7.65,44 p2451).

// Step_Mode[i] of 7.7.65,44 p2451, 0x00-0x03. The modes differ in which packets a
// step exchanges and therefore in what the Controller reports back per step.
#define CS_STEP_MODE_0 0x00
#define CS_STEP_MODE_1 0x01
#define CS_STEP_MODE_2 0x02
#define CS_STEP_MODE_3 0x03

// Role of the local device for the CS step, from the Role field of the configuration
// in force (7.7.65,42 p2438, and the 0x0090 parameter of 7.8.137). It is not on the
// wire per step: the same step payload length decodes differently for the two roles,
// so the engine carries the role of the Config_ID down into the decode, and out again
// in CsSubeventReport.role so the result stays readable without it.
#define CS_ROLE_INITIATOR 0x00
#define CS_ROLE_REFLECTOR 0x01

// Upper bound of one subevent's step records (see above). The engine sizes the report
// for a full subevent up front, so a Controller reporting more is turned away rather
// than truncating a measurement.
#define CS_RESULT_MAX_STEPS 160

// Tone_PCT[k] / Tone_Quality_Indicator[k] are reported for k = 0..Num_Antenna_Paths
// (7.7.65,44 p2458), and Num_Antenna_Paths is 0x00-0x04 (p2450), so k is at most 5.
#define CS_RESULT_MAX_TONE_PATHS 5

// A 24-bit phase term is stored as three octets per path.
#define CS_RESULT_TONE_PCT_LEN 3

// The two "no value" codes the Controller writes into the signed CS result fields:
// 0xC000 into the 15-bit frequency fields, 0x8000 into the 16-bit time difference
// (7.7.65,44 pp2449 and p2457). Each stays in the record as the value it parses to,
// outside the range the spec gives its field, so a reader compares against these names
// rather than against a range. The cast puts them in the stored type.
#define CS_FREQ_OFFSET_UNAVAILABLE ((int16_t)0xC000)
#define CS_TIME_DIFF_UNAVAILABLE ((int16_t)0x8000)

// Mode 0 (pp2451-2452): Packet_Quality, Packet_RSSI, Packet_Antenna and - initiator
// only - Measured_Freq_Offset. The two roles differ in length, the only place in the
// four modes where the role decides the template, so the flag is exactly "the 5-octet
// initiator shape was the one that fit".
typedef struct {
    uint8_t packetQuality;              // bits 0-3 AA check, 4-7 payload bit errors
    int8_t packetRssi;                  // dBm, -127..20; 0x7F means not available
    uint8_t packetAntenna;              // 0x01-0x04 antenna identifier
    bool hasFreqOffset;                 // set by the initiator shape only
    int16_t measuredFreqOffset;         // 0.01 ppm, 15-bit signed; 0xC000 means not available
} CsStepMode0;

// The head modes 1 and 3 share (pp2452-2454): Packet_Quality, Packet_NADM,
// Packet_RSSI, the two-octet time difference, Packet_Antenna, and - only when the RTT
// type carries a sounding sequence - the two Packet_PCT values. The spec names the
// time field by the role it was measured in (ToA_ToD_Initiator / ToD_ToA_Reflector,
// p2457); the two are the same two octets, so CsSubeventReport.role is what says which
// one this is.
typedef struct {
    uint8_t packetQuality;
    uint8_t packetNadm;                 // 0x00-0x06, 0xFF unknown
    int8_t packetRssi;
    bool hasPct;                        // set by the sounding shape (14/15 octets) only
    int16_t timeDifference;             // 0.5 ns, signed, nominal offsets excluded; 0x8000 not available
    uint8_t packetAntenna;
    uint32_t packetPct1;                // 4 octets: bits 0-11 I sample, 12-23 Q sample,
    uint32_t packetPct2;                // 24-31 RFU; 0xFFFFFFFF means not available
} CsStepMode1;

// The tone tail modes 2 and 3 carry (pp2457-2458): Antenna_Permutation_Index, then
// k = Num_Antenna_Paths + 1 tones, each a 3-octet Tone_PCT[k] with its own 1-octet
// Tone_Quality_Indicator[k] behind it. Mode 2 is the tail alone; mode 3 reuses it
// behind the mode-1 head. numTonePaths is k as decoded.
// Tone_Quality_Indicator[k] (7.7.65,44, p2458): bits 0-3 = 0x0 high / 0x1 medium /
// 0x2 low / 0x3 not available; bits 4-7 = 0x0 not a tone extension slot / 0x1 tone
// not expected / 0x2 tone expected. Gated by LE feature bit 48.
typedef struct {
    uint8_t antennaPermutationIndex;    // 0x00-0x17
    uint8_t numTonePaths;               // k actually decoded, 1..CS_RESULT_MAX_TONE_PATHS
    uint8_t toneQuality[CS_RESULT_MAX_TONE_PATHS];       // bits 0-3 quality, 4-7 slot
    uint8_t tonePct[CS_RESULT_MAX_TONE_PATHS][CS_RESULT_TONE_PCT_LEN];        // 24-bit LE phase term
} CsStepMode2;

// Mode 3: the mode-1 head, then the tone tail - the two Packet_PCT values, when the
// RTT type carries a sounding sequence, sit between Packet_Antenna and
// Antenna_Permutation_Index (pp2453-2454).
typedef struct {
    CsStepMode1 head;
    CsStepMode2 tones;
} CsStepMode3;

// One CS step of a subevent. mode and the three Step_* envelope fields are what every
// mode has (p2451); stepData is the payload of the mode the envelope names.
// hasMeasurement is the one thing to read before reading stepData: it is set only where
// that step's Step_Data was decoded, so while it is clear every stepData member is zero
// and none of them is a reading. A step is left that way for either of two reasons, and
// stepDataLength tells the two apart: it is 0 when the Controller sent no data for the
// step at all (p2447, the step may have been aborted), and nonzero when it sent data
// whose length fitted no mode template. Either way the step happened and is kept - it
// stays in steps[] and counts in numSteps, so the indices keep lining up with the
// Controller's own step numbering. Byte order is normalized: the 24-bit Tone_PCT[k] and
// the 32-bit Packet_PCT1/2 are held as their little-endian wire value, so only the
// sub-field extraction of the phase correction terms is left to the consumer.
typedef struct {
    uint8_t mode;                       // Step_Mode[i]
    uint8_t channel;                    // Step_Channel[i], 0x00-0x4E
    uint8_t stepDataLength;             // Step_Data_Length[i] as sent; 0 carried no data
    bool hasMeasurement;                // read stepData only when this is set
    union {
        CsStepMode0 mode0;
        CsStepMode1 mode1;
        CsStepMode2 mode2;
        CsStepMode3 mode3;
    } stepData;
} CsStepResult;

// One CS subevent, assembled from its opening 0x31 and every 0x32 continuing it. The
// fields above the statuses are the opening 0x31's alone - a 0x32 head does not carry
// the counter pair, the frequency compensation or the reference power level
// (7.7.65,45 p2459). The three status fields and numAntennaPaths are refreshed from
// each continuation, so they describe the last event absorbed.
// role and rttType are not on the wire of either subevent: they are the Role and
// RTT_Type fields of the configuration the steps were decoded against (7.7.65,42
// p2438). The spec is explicit that the step data needs them - Step_Data[i] is
// defined as mode- and role-specific, and the Mode_Role_Specific_Info object is
// listed once per role and grows when the RTT type contains a sounding sequence
// (p2462, pp2451-2455). The decode is not reversible without them either: a mode-0
// step is 5 octets for one role and 3 for the other, and the same two-octet time
// field is ToA_ToD_Initiator or ToD_ToA_Reflector depending on which (p2457). The
// role gating stops at the byte layout, but the RTT type does not: Packet_Quality
// carries bit errors only when the RTT type is a random or sounding sequence, and is
// ignored by the Host otherwise (p2455), so a decoded field stays unreadable without
// it. They are carried here rather than left to a lookup because a CS test result
// arrives under handle 0x0FFF with no connection and no configuration to look up.
typedef struct {
    uint16_t connectionHandle;          // 0x0000-0x0EFF, or 0x0FFF for a CS test (p2448)
    uint8_t configId;
    uint16_t startAclConnEventCounter;
    uint16_t procedureCounter;
    int16_t frequencyCompensation;      // 0.01 ppm, 15-bit signed; 0xC000 not available or role not initiator
    int8_t referencePowerLevel;         // dBm, -127..20
    uint8_t procedureDoneStatus;        // bits 0-3: 0x0 complete, 0x1 partial, 0xF aborted
    uint8_t subeventDoneStatus;         // bits 0-3: 0x0 complete, 0x1 partial, 0xF aborted
    uint8_t abortReason;                // bits 0-3 procedure; bits 4-7 subevent, 0x2/0x3 in 0x31 only
    uint8_t numAntennaPaths;            // 0x00 no phase measurement, 0x01-0x04 paths
    uint8_t numStepsReported;           // the opening 0x31's own count 0x00-0xA0, not the report's total
    uint8_t numSteps;                   // decoded across the 0x31 and every 0x32; may exceed numStepsReported
    uint8_t role;                       // CS_ROLE_* these steps were decoded against
    uint8_t rttType;                    // RTT_Type in force, 0x00-0x06
    CsStepResult steps[CS_RESULT_MAX_STEPS];  // only the first numSteps are valid; the rest stay zero
} CsSubeventReport;

#ifdef __cplusplus
}
#endif

#endif
