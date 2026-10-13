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

#include "cs_internal.h"

#include <securec.h>
#include <stddef.h>

#include "log.h"
#include "platform/include/allocator.h"

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.7.65,44 LE CS Subevent Result Event (subevent 0x31, pp2446-2458) and
// 7.7.65,45 LE CS Subevent Result Continue Event (subevent 0x32, pp2459-2462).
// The two subevents share one step region: after the head come Num_Steps_Reported
// records, each a three-octet Step_Mode[i], Step_Channel[i], Step_Data_Length[i] head
// with that step's own Step_Data[i] immediately behind it. The spec lists the four
// parameters by name - each of the first three as a Num_Steps_Reported-octet array,
// the data as the sum of their lengths (p2451) - but the wire is per step, in the same
// way the tone tail of p2458 is per tone; reading three parallel arrays followed by one
// concatenated blob is a different frame, and only the per-step reading accounts for
// every byte the event carries. A subevent holds at most N_MAX_STEPS_PER_SUBEVENT = 160
// steps, more than one HCI event can carry, so the Controller opens it with a 0x31 and
// continues it with 0x32 events until Subevent_Done_Status stops saying 0x1.
// This file is the whole of that: the Table 7.2 combination check, the per-step
// decode against the mode/role templates below, and the aggregation into one
// CsSubeventReport. It sees only events cs.c has already attributed to a connection
// the engine holds a CS configuration for - the CS test handle and the lookup itself
// are cs.c's, because they are decisions about which table to consult, not about the
// step stream.

// The RTT_Type field of the CS configuration (7.7.65,42 p2438, 0x00-0x06). Only the
// two sounding values add Packet_PCT1/Packet_PCT2 to a mode-1 or mode-3 step.
#define CS_RTT_TYPE_SOUNDING_32 0x01
#define CS_RTT_TYPE_SOUNDING_96 0x02

// Mode_Role_Specific_Info lengths, in the order the spec lists them. They are named
// rather than inlined because modes 1 and 3 share a head and 2 and 3 share a tail.
#define CS_STEP_HEAD_LEN 6         // Packet_Quality .. Packet_Antenna, modes 1 and 3
#define CS_MODE1_PCT_LEN 14        // head + Packet_PCT1/2
#define CS_MODE3_BASE_LEN 7        // head + Antenna_Permutation_Index
#define CS_MODE3_PCT_LEN 15        // + Packet_PCT1/2, inserted before the permutation index
#define CS_STEP_RECORD_HEAD_LEN 3  // Step_Mode, Step_Channel, Step_Data_Length, per step
#define CS_STEP_RECORD_MODE_OFFSET 0
#define CS_STEP_RECORD_CHANNEL_OFFSET 1
#define CS_STEP_RECORD_LENGTH_OFFSET 2
#define CS_TONE_ENTRY_LEN 4        // one tone: its Tone_PCT then its Tone_Quality_Indicator

// Per-byte and per-step layout constants for the decoder. CS is a 6.0 feature, so these
// are local to this file; they follow the same naming style as the _LEN macros above.
#define CS_BITS_PER_BYTE 8
#define CS_DONE_STATUS_MASK 0x0F

// Procedure_Done_Status and Subevent_Done_Status values from Table 7.2 (p2447).
#define CS_PROCEDURE_DONE_STATUS_COMPLETE   0x0
#define CS_PROCEDURE_DONE_STATUS_PARTIAL    0x1
#define CS_PROCEDURE_DONE_STATUS_ABORTED    0xF
#define CS_SUBEVENT_DONE_STATUS_COMPLETE    0x0
#define CS_SUBEVENT_DONE_STATUS_PARTIAL    0x1
#define CS_SUBEVENT_DONE_STATUS_ABORTED     0xF

#define CS_UINT32_BYTES 4
#define CS_FREQ_SIGN_BIT 0x4000
#define CS_FREQ_SIGN_EXTENSION 0x8000

// Mode 0 step layout (Vol 4, Part E 7.7.65,44 p2452).
#define CS_STEP_MODE0_INITIATOR_LEN 5
#define CS_STEP_MODE0_REFLECTOR_LEN 3
#define CS_STEP_MODE0_PACKET_QUALITY_OFFSET 0
#define CS_STEP_MODE0_PACKET_RSSI_OFFSET    1
#define CS_STEP_MODE0_PACKET_ANTENNA_OFFSET 2
#define CS_STEP_MODE0_FREQ_OFFSET_OFFSET    3

// Shared head for modes 1 and 3 (p2453/p2457).
#define CS_STEP_MODE1_PACKET_QUALITY_OFFSET 0
#define CS_STEP_MODE1_PACKET_NADM_OFFSET    1
#define CS_STEP_MODE1_PACKET_RSSI_OFFSET    2
#define CS_STEP_MODE1_TIME_DIFFERENCE_OFFSET 3
#define CS_STEP_MODE1_PACKET_ANTENNA_OFFSET 5

// Packet_PCT block inside a sounding-sequence step.
#define CS_PACKET_PCT1_OFFSET 0
#define CS_PACKET_PCT2_OFFSET 4
#define CS_PACKET_PCT_TOTAL_LEN 8

// Mode 2 step layout (p2456).
#define CS_STEP_MODE2_BASE_LEN 1
#define CS_ANTENNA_PERMUTATION_INDEX_LEN 1
#define CS_STEP_MODE2_ANTENNA_PERMUTATION_OFFSET 0

static CsResultSink g_resultSink = NULL;

// The report being handed to the sink right now, and the buffer a release deferred because
// of it. CsResultEmit gives the sink the engine's own report, which is the same buffer the
// connection's stream keeps for its later subevents, and a sink that closes the session -
// or re-registers the sink, or calls CsResultSetSink through the registration path - reaches
// CsResultReleaseStream while that report is still being read. The CSIF_-reached entry
// points make that reachable from inside the callback: CsRunTaskBlockProcess runs the task
// inline when the caller is already on the Stack thread (cs_task_common.c), which is exactly
// the thread the sink is called on. So the release must not free the buffer it is standing
// in: g_emittingStream names the stream whose report is out, and a release of that one parks
// the buffer in g_deferredRelease for CsResultEmit to free once the sink has returned.
// One deferral slot is enough because delivery is not re-entrant - a report is only handed
// over from an HCI event dispatch, and a sink cannot make another one arrive from inside a
// callback - so at most one stream is in that state at a time. A release of any other stream
// is not standing in the buffer it frees and is freed on the spot, which is why this is one
// named pointer rather than reactor_linux.c's pendingFree list: that one has to cover any
// item of its dispatch loop, and it allocates a node per deferral (list.c), an allocation
// this path cannot afford to fail.
static const CsResultStream *g_emittingStream = NULL;
static CsSubeventReport *g_deferredRelease = NULL;

void CsResultSetSink(CsResultSink sink)
{
    g_resultSink = sink;
}

// Table 7.2 of p2447 as a predicate. The two fields are independent: Procedure_Done
// -Status describes the CS procedure, Subevent_Done_Status the subevent being reported
// now, and only the pairs below are defined. A pair outside the table is a
// non-conforming frame, and its two statuses cannot be told apart from a stale one, so
// the caller drops whatever the stream held rather than reporting it.
static bool CsDoneStatusPairValid(uint8_t procedureDoneStatus, uint8_t subeventDoneStatus)
{
    uint8_t procedureDone = procedureDoneStatus & CS_DONE_STATUS_MASK;
    uint8_t subeventDone = subeventDoneStatus & CS_DONE_STATUS_MASK;

    switch (procedureDone) {
        case CS_PROCEDURE_DONE_STATUS_COMPLETE:
        case CS_PROCEDURE_DONE_STATUS_ABORTED:
            return subeventDone == CS_SUBEVENT_DONE_STATUS_COMPLETE ||
                subeventDone == CS_SUBEVENT_DONE_STATUS_ABORTED;
        case CS_PROCEDURE_DONE_STATUS_PARTIAL:
            return subeventDone == CS_SUBEVENT_DONE_STATUS_COMPLETE ||
                subeventDone == CS_SUBEVENT_DONE_STATUS_PARTIAL ||
                subeventDone == CS_SUBEVENT_DONE_STATUS_ABORTED;
        default:
            return false;  // reserved, and outside Table 7.2 as well
    }
}

// Subevent_Done_Status 0x1 is the only value that leaves a subevent open; 0x0 and 0xF
// both end it (p2447).
static bool CsSubeventContinues(uint8_t subeventDoneStatus)
{
    return (subeventDoneStatus & CS_DONE_STATUS_MASK) == CS_SUBEVENT_DONE_STATUS_PARTIAL;
}

static uint16_t CsReadUint16(const uint8_t *data)
{
    return (uint16_t)((uint16_t)data[0] | ((uint16_t)data[1] << CS_BITS_PER_BYTE));
}

static uint32_t CsReadUint32(const uint8_t *data)
{
    uint32_t value = 0;
    for (uint8_t i = 0; i < CS_UINT32_BYTES; i++) {
        value |= (uint32_t)data[i] << (CS_BITS_PER_BYTE * i);
    }
    return value;
}

// The two frequency fields of the CS results are 15-bit signed: bit 15 is not part of
// the value and the sign is bit 14, so the spec's own -100 ppm endpoint 0x58F0 reads as
// +22768 in 16 bits (7.7.65,44 p2449 and p2457). The 0xC000 sentinel masks to 0x4000
// and comes out -16384, outside the -100..+100 ppm range, so it stays tellable apart
// from a measurement.
static int16_t CsSignExtend15(uint16_t raw)
{
    return (int16_t)((raw & CS_FREQ_SIGN_BIT) ? (raw | CS_FREQ_SIGN_EXTENSION) : raw);
}

// The tone tail shared by mode 2 and mode 3: k entries of Tone_PCT[k] (3 octets each)
// and k entries of Tone_Quality_Indicator[k] (1 octet each), where k is
// Num_Antenna_Paths + 1 and therefore never exceeds CS_RESULT_MAX_TONE_PATHS. The spec
// lists the two by name with their own sizes (p2458), but the wire is one entry per
// tone, the indicator behind its own phase term - the k indicators do not form a block
// after the k phase terms. Vol 6, Part H, Section 4.6 (p3774-3775) says the same thing
// from the measurement side: the tone extension slot contributes one phase measurement
// and "a tone quality indication to each" of them.
static void CsDecodeTones(const uint8_t *data, uint8_t k, CsStepMode2 *tones)
{
    tones->numTonePaths = k;
    for (uint8_t i = 0; i < k; i++) {
        const uint8_t *tone = data + CS_TONE_ENTRY_LEN * i;
        for (uint8_t j = 0; j < CS_RESULT_TONE_PCT_LEN; j++) {
            tones->tonePct[i][j] = tone[j];
        }
        tones->toneQuality[i] = tone[CS_RESULT_TONE_PCT_LEN];
    }
}

// Mode 0: Packet_Quality, Packet_RSSI, Packet_Antenna and - initiator only - the
// Measured_Freq_Offset (pp2451-2452). The two roles differ in length, which is the only
// place in the four modes where the role decides the template.
static bool CsDecodeMode0(uint8_t role, const uint8_t *data, uint8_t length, CsStepMode0 *mode0)
{
    if (role != CS_ROLE_INITIATOR && role != CS_ROLE_REFLECTOR) {
        return false;
    }
    uint8_t expected = (role == CS_ROLE_INITIATOR) ? CS_STEP_MODE0_INITIATOR_LEN : CS_STEP_MODE0_REFLECTOR_LEN;
    if (length != expected) {
        return false;
    }
    mode0->packetQuality = data[CS_STEP_MODE0_PACKET_QUALITY_OFFSET];
    mode0->packetRssi = (int8_t)data[CS_STEP_MODE0_PACKET_RSSI_OFFSET];
    mode0->packetAntenna = data[CS_STEP_MODE0_PACKET_ANTENNA_OFFSET];
    if (role == CS_ROLE_INITIATOR) {
        mode0->hasFreqOffset = true;
        mode0->measuredFreqOffset = CsSignExtend15(CsReadUint16(data + CS_STEP_MODE0_FREQ_OFFSET_OFFSET));
    }
    return true;
}

// Modes 1 and 3 share a head - Packet_Quality, Packet_NADM, Packet_RSSI, the two-octet
// time difference, Packet_Antenna - and differ only in the name the spec gives the
// time field for the two roles (ToA_ToD_Initiator / ToD_ToA_Reflector, p2457),
// which is not a layout difference. Mode 3 continues into the tone tail directly, or
// after the phase correction terms when the RTT type carries a sounding sequence.
static void CsDecodeMode1Or3Head(const uint8_t *data, CsStepMode1 *head)
{
    head->packetQuality = data[CS_STEP_MODE1_PACKET_QUALITY_OFFSET];
    head->packetNadm = data[CS_STEP_MODE1_PACKET_NADM_OFFSET];
    head->packetRssi = (int8_t)data[CS_STEP_MODE1_PACKET_RSSI_OFFSET];
    head->timeDifference = (int16_t)CsReadUint16(data + CS_STEP_MODE1_TIME_DIFFERENCE_OFFSET);
    head->packetAntenna = data[CS_STEP_MODE1_PACKET_ANTENNA_OFFSET];
}

// The two Packet_PCT values a sounding-sequence RTT type adds after Packet_Antenna.
static void CsDecodePacketPct(const uint8_t *data, CsStepMode1 *head)
{
    head->hasPct = true;
    head->packetPct1 = CsReadUint32(data + CS_PACKET_PCT1_OFFSET);
    head->packetPct2 = CsReadUint32(data + CS_PACKET_PCT2_OFFSET);
}

// A tone-carrying step is 4k octets of tone data behind its head, one 4-octet entry per
// tone (see CsDecodeTones), where k is Num_Antenna_Paths + 1 by the size the spec gives
// both arrays (Vol 4 Part E 7.7.65,44 p2458). The event's Num_Antenna_Paths therefore
// fixes k, and a length whose implied k disagrees with it describes a step other than the
// one the event header declares. 0x00 still fixes k to 1 because Tone_PCT[k] and
// Tone_Quality_Indicator[k] are present with that size even when phase measurement did
// not occur; the reserved 0x05-0xFF are rejected rather than guessed at. base is the head
// length of the template being tried.
static bool CsToneCountMatches(uint8_t length, uint8_t base, uint8_t numAntennaPaths)
{
    if (numAntennaPaths >= CS_RESULT_MAX_TONE_PATHS) {
        return false;
    }
    if (length < base || ((length - base) % CS_TONE_ENTRY_LEN) != 0) {
        return false;
    }
    uint8_t k = (uint8_t)((length - base) / CS_TONE_ENTRY_LEN);
    if (k == 0 || k > CS_RESULT_MAX_TONE_PATHS) {
        return false;
    }
    return k == (uint8_t)(numAntennaPaths + 1);
}

// Mode 1: a shared head plus an optional pair of Packet_PCT values when the RTT type
// carries a sounding sequence (p2453/p2457).
static bool CsDecodeMode1(const uint8_t *data, uint8_t length, CsStepMode1 *mode1)
{
    if (length != CS_STEP_HEAD_LEN && length != CS_MODE1_PCT_LEN) {
        return false;
    }
    CsDecodeMode1Or3Head(data, mode1);
    if (length == CS_MODE1_PCT_LEN) {
        CsDecodePacketPct(data + CS_STEP_HEAD_LEN, mode1);
    }
    return true;
}

// Mode 3: the Mode-1-shaped head, optional Packet_PCT values, then the tone tail.
// The two possible lengths (7 + 4k and 15 + 4k) are disambiguated by Num_Antenna_Paths
// when it fixes k, and by the RTT preference otherwise (p2453/p2454).
static bool CsDecodeMode3(const uint8_t *data, uint8_t length, uint8_t rttType,
    uint8_t numAntennaPaths, CsStepMode3 *mode3)
{
    bool sounding = rttType == CS_RTT_TYPE_SOUNDING_32 || rttType == CS_RTT_TYPE_SOUNDING_96;
    uint8_t preferred = sounding ? CS_MODE3_PCT_LEN : CS_MODE3_BASE_LEN;
    uint8_t alternate = (preferred == CS_MODE3_PCT_LEN) ? CS_MODE3_BASE_LEN : CS_MODE3_PCT_LEN;
    uint8_t base = CsToneCountMatches(length, preferred, numAntennaPaths) ? preferred : 0;
    if (base == 0) {
        base = CsToneCountMatches(length, alternate, numAntennaPaths) ? alternate : 0;
    }
    if (base == 0) {
        return false;
    }
    CsDecodeMode1Or3Head(data, &mode3->head);
    uint8_t cursor = CS_STEP_HEAD_LEN;
    if (base == CS_MODE3_PCT_LEN) {
        CsDecodePacketPct(data + cursor, &mode3->head);
        cursor += CS_PACKET_PCT_TOTAL_LEN;
    }
    mode3->tones.antennaPermutationIndex = data[cursor];
    CsDecodeTones(data + cursor + CS_ANTENNA_PERMUTATION_INDEX_LEN,
        (uint8_t)((length - base) / CS_TONE_ENTRY_LEN), &mode3->tones);
    return true;
}

// Decode one step's Mode_Role_Specific_Info into step, after the caller has set mode,
// channel and stepDataLength. The template is selected by the reported length, not by
// the capabilities that predict it: the length is what the Controller actually sent,
// and a Controller that omits the optional phase correction terms is describing its
// own report, not misbehaving. Mode 3 is the one mode whose length fits two templates
// - 7 + 4k and 15 + 4k overlap at k = 3..5 - and those are told apart by the tone
// count the event's Num_Antenna_Paths fixes (both templates imply a different one),
// with the RTT type standing in only when it is 0x00. Returns false when no template
// fits, and writes nothing to stepData on any path that does - the caller leaves
// hasMeasurement clear rather than guessing at the bytes.
typedef struct {
    uint8_t mode;
    uint8_t role;
    uint8_t rttType;
    uint8_t numAntennaPaths;
    const uint8_t *data;
    uint8_t length;
} CsDecodeStepInput;

static bool CsDecodeStep(const CsDecodeStepInput *input, CsStepResult *step)
{
    bool result = false;
    switch (input->mode) {
        case CS_STEP_MODE_0:
            result = CsDecodeMode0(input->role, input->data, input->length, &step->stepData.mode0);
            break;
        case CS_STEP_MODE_1:
            result = CsDecodeMode1(input->data, input->length, &step->stepData.mode1);
            break;
        case CS_STEP_MODE_2: {
            // Antenna_Permutation_Index + 4k. It has no head of its own, and it
            // is the one mode the spec gives both roles the same shape.
            if (!CsToneCountMatches(input->length, CS_STEP_MODE2_BASE_LEN, input->numAntennaPaths)) {
                result = false;
                break;
            }
            CsStepMode2 *tones = &step->stepData.mode2;
            tones->antennaPermutationIndex = input->data[CS_STEP_MODE2_ANTENNA_PERMUTATION_OFFSET];
            CsDecodeTones(input->data + CS_STEP_MODE2_BASE_LEN,
                (uint8_t)((input->length - CS_STEP_MODE2_BASE_LEN) / CS_TONE_ENTRY_LEN), tones);
            result = true;
            break;
        }
        case CS_STEP_MODE_3:
            result = CsDecodeMode3(input->data, input->length, input->rttType, input->numAntennaPaths,
                &step->stepData.mode3);
            break;
        default:
            result = false;  // Step_Mode 0x04-0xFF is reserved (p2451)
            break;
    }
    return result;
}

// Walk the step region of one event into the stream. The region is identical for the
// opening 0x31 and a continuing 0x32, so both go through here. Each step is walked as
// the wire carries it: its own three-octet head, then its own Step_Data - the spec's
// by-name listing of the four parameters is not an array layout (p2451, and see the
// file head). Every bound is checked against blobLen before it is used, and a region
// that runs past the event fails the whole event rather than reporting the steps that
// did fit: a partial step set is not a measurement, and the caller drops the subevent
// it was accumulating. numAntennaPaths is the event's own field, passed through to the
// per-step tone count check. A step whose Step_Data_Length is 0 is recorded with
// hasMeasurement clear rather than skipped, so the recorded index stays lined up with
// the Controller's step numbering.
static bool CsAbsorbSteps(CsResultStream *stream, const uint8_t *blob, uint16_t blobLen,
    uint8_t numStepsReported, uint8_t numAntennaPaths)
{
    if (numStepsReported > CS_RESULT_MAX_STEPS) {
        return false;
    }
    uint16_t cursor = 0;
    for (uint8_t i = 0; i < numStepsReported; i++) {
        if (blobLen - cursor < CS_STEP_RECORD_HEAD_LEN) {
            return false;  // the head of this step is not all here
        }
        uint8_t mode = blob[cursor + CS_STEP_RECORD_MODE_OFFSET];
        uint8_t channel = blob[cursor + CS_STEP_RECORD_CHANNEL_OFFSET];
        uint8_t dataLength = blob[cursor + CS_STEP_RECORD_LENGTH_OFFSET];
        cursor += CS_STEP_RECORD_HEAD_LEN;
        if (dataLength > blobLen - cursor) {
            return false;
        }
        if (stream->count >= CS_RESULT_MAX_STEPS) {
            return false;  // more than N_MAX_STEPS_PER_SUBEVENT steps across the subevent
        }
        CsStepResult *step = &stream->report->steps[stream->count];
        (void)memset_s(step, sizeof(*step), 0, sizeof(*step));
        step->mode = mode;
        step->channel = channel;
        step->stepDataLength = dataLength;
        // Zero length means the Controller sent no data for this step (p2447, it may
        // have been aborted), so hasMeasurement stays clear and stepData stays zero.
        if (dataLength != 0) {
            CsDecodeStepInput input = {
                mode, stream->role, stream->rttType, numAntennaPaths,
                blob + cursor, dataLength
            };
            if (CsDecodeStep(&input, step)) {
                step->hasMeasurement = true;
            }
        }
        stream->count++;
        cursor += dataLength;
    }
    return true;
}

// Clear the head a report carries. Every field above the steps array is reset together,
// so a reused report cannot leak the previous subevent's identity into this one.
static void CsClearReportHead(CsSubeventReport *report)
{
    (void)memset_s(report, offsetof(CsSubeventReport, steps), 0, offsetof(CsSubeventReport, steps));
}

// Clear the step records the previous subevent of this connection wrote. Nothing is ever
// written past numSteps and the buffer itself comes from MEM_CALLOC, so clearing just the
// written prefix is what keeps steps[numSteps..] zero for every subevent of a connection.
static void CsClearReportSteps(CsSubeventReport *report, uint8_t count)
{
    (void)memset_s(report->steps, sizeof(report->steps[0]) * count, 0, sizeof(report->steps[0]) * count);
}

void CsResultResetStream(CsResultStream *stream)
{
    if (stream == NULL) {
        return;
    }
    if (stream->report != NULL) {
        CsClearReportHead(stream->report);
        CsClearReportSteps(stream->report, stream->count);
    }
    stream->open = false;
    stream->count = 0;
}

void CsResultReleaseStream(CsResultStream *stream)
{
    if (stream == NULL) {
        return;
    }
    if (stream->report != NULL) {
        if (stream == g_emittingStream) {
            // The sink is reading this very buffer (see the statics above): hand it over
            // rather than free it under the reader.
            g_deferredRelease = stream->report;
        } else {
            MEM_CALLOC.free(stream->report);
        }
    }
    stream->report = NULL;
    stream->open = false;
    stream->count = 0;
}

void CsResultEmit(const CsResultStream *stream)
{
    if (g_resultSink == NULL || stream == NULL || stream->report == NULL) {
        return;
    }
    // A release made from inside the sink parks its buffer here: see the statics above.
    g_emittingStream = stream;
    g_resultSink(stream->report);
    g_emittingStream = NULL;
    if (g_deferredRelease != NULL) {
        MEM_CALLOC.free(g_deferredRelease);
        g_deferredRelease = NULL;
    }
}

// The report buffer is allocated on the first subevent of a connection and kept for
// that connection's later subevents, so the cost of the CS result path is one report
// per CS-active connection rather than one per subevent. It is zero-filled rather than
// freed per subevent because the decode writes only the records it decoded: the sink is
// handed the whole struct, so the tail past numSteps must carry neither the previous
// subevent's measurements nor untouched heap.
static bool CsEnsureReport(CsResultStream *stream)
{
    if (stream->report != NULL) {
        return true;
    }
    stream->report = (CsSubeventReport *)MEM_CALLOC.alloc(sizeof(CsSubeventReport));
    if (stream->report == NULL) {
        LOG_WARN("%{public}s: report allocation failed, subevent dropped", __FUNCTION__);
        return false;
    }
    return true;
}

CsResultOutcome CsResultAbsorbResult(CsResultStream *stream,
    const HciLeCsSubeventResultEnvelope *envelope, uint8_t role, uint8_t rttType)
{
    if (stream == NULL || envelope == NULL || g_resultSink == NULL) {
        return CS_RESULT_DROPPED;
    }
    if (!CsDoneStatusPairValid(envelope->head.procedureDoneStatus, envelope->head.subeventDoneStatus)) {
        LOG_WARN("%{public}s: Procedure_Done_Status 0x%02x / Subevent_Done_Status 0x%02x outside Table 7.2, dropped",
            __FUNCTION__, envelope->head.procedureDoneStatus, envelope->head.subeventDoneStatus);
        CsResultResetStream(stream);
        return CS_RESULT_DROPPED;
    }
    if (!CsEnsureReport(stream)) {
        return CS_RESULT_DROPPED;
    }
    // A 0x31 opens a subevent: anything the stream still held was superseded by this
    // event, so the report starts empty rather than being appended to.
    CsResultResetStream(stream);
    stream->open = true;
    stream->role = role;
    stream->rttType = rttType;

    CsSubeventReport *report = stream->report;
    report->connectionHandle = envelope->head.connectionHandle;
    report->configId = envelope->head.configId;
    report->startAclConnEventCounter = envelope->head.startAclConnEventCounter;
    report->procedureCounter = envelope->head.procedureCounter;
    report->frequencyCompensation = CsSignExtend15(envelope->head.frequencyCompensation);
    report->referencePowerLevel = (int8_t)envelope->head.referencePowerLevel;
    report->procedureDoneStatus = envelope->head.procedureDoneStatus;
    report->subeventDoneStatus = envelope->head.subeventDoneStatus;
    report->abortReason = envelope->head.abortReason;
    report->numAntennaPaths = envelope->head.numAntennaPaths;
    report->numStepsReported = envelope->head.numStepsReported;
    // The decode context, not on the wire: it is what the steps below were decoded
    // against, and the result cannot be read back without it (see cs_defs.h).
    report->role = role;
    report->rttType = rttType;

    if (!CsAbsorbSteps(stream, envelope->stepBlob, envelope->stepBlobLen,
        envelope->head.numStepsReported, envelope->head.numAntennaPaths)) {
        LOG_WARN("%{public}s: step region of handle 0x%04x Config_ID 0x%02x is incomplete, subevent dropped",
            __FUNCTION__, report->connectionHandle, report->configId);
        CsResultResetStream(stream);
        return CS_RESULT_DROPPED;
    }
    report->numSteps = stream->count;
    if (CsSubeventContinues(envelope->head.subeventDoneStatus)) {
        return CS_RESULT_CONTINUES;
    }
    return CS_RESULT_ENDED;
}

CsResultOutcome CsResultAbsorbContinue(CsResultStream *stream,
    const HciLeCsSubeventResultContinueEnvelope *envelope)
{
    if (stream == NULL || envelope == NULL || g_resultSink == NULL) {
        return CS_RESULT_DROPPED;
    }
    // A 0x32 belongs to a subevent a 0x31 opened on this same connection. Without one
    // there is nothing to continue and no role to decode a mode-0 step with, so the
    // event is dropped rather than read as the start of something.
    if (!stream->open || stream->report == NULL) {
        LOG_WARN("%{public}s: 0x32 on handle 0x%04x with no subevent open, dropped",
            __FUNCTION__, envelope->head.connectionHandle);
        return CS_RESULT_DROPPED;
    }
    // On a live ACL, Config_ID names the subevent being continued (7.7.65,45): only
    // handle 0x0FFF may ignore it. A mismatch would append steps under the open
    // stream's role/rttType, so it is refused rather than decoded as a continuation.
    if (envelope->head.connectionHandle != LE_CS_TEST_HANDLE &&
        envelope->head.configId != stream->report->configId) {
        LOG_WARN("%{public}s: 0x32 Config_ID 0x%02x on handle 0x%04x does not match open "
            "subevent Config_ID 0x%02x, dropped",
            __FUNCTION__, envelope->head.configId, envelope->head.connectionHandle,
            stream->report->configId);
        return CS_RESULT_DROPPED;
    }
    if (!CsDoneStatusPairValid(envelope->head.procedureDoneStatus, envelope->head.subeventDoneStatus)) {
        LOG_WARN("%{public}s: Procedure_Done_Status 0x%02x / Subevent_Done_Status 0x%02x outside Table 7.2, dropped",
            __FUNCTION__, envelope->head.procedureDoneStatus, envelope->head.subeventDoneStatus);
        CsResultResetStream(stream);
        return CS_RESULT_DROPPED;
    }
    CsSubeventReport *report = stream->report;
    report->procedureDoneStatus = envelope->head.procedureDoneStatus;
    report->subeventDoneStatus = envelope->head.subeventDoneStatus;
    report->abortReason = envelope->head.abortReason;
    report->numAntennaPaths = envelope->head.numAntennaPaths;

    if (!CsAbsorbSteps(stream, envelope->stepBlob, envelope->stepBlobLen,
        envelope->head.numStepsReported, envelope->head.numAntennaPaths)) {
        LOG_WARN("%{public}s: step region of handle 0x%04x Config_ID 0x%02x is incomplete, subevent dropped",
            __FUNCTION__, report->connectionHandle, report->configId);
        CsResultResetStream(stream);
        return CS_RESULT_DROPPED;
    }
    report->numSteps = stream->count;
    if (CsSubeventContinues(envelope->head.subeventDoneStatus)) {
        return CS_RESULT_CONTINUES;
    }
    return CS_RESULT_ENDED;
}
