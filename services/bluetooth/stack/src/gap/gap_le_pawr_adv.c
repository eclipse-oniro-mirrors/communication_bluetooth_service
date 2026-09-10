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

/* Advertiser side of Bluetooth 5.4 periodic advertising with responses
 * (PAwR), Core Spec 5.4 Vol 4 Part E:
 *   - 7.8.61 [v2]  LE Set Periodic Advertising Parameters [v2]
 *   - 7.8.125      LE Set Periodic Advertising Subevent Data
 *   - 7.7.65.36    LE Periodic Advertising Subevent Data Request event (0x27)
 *   - 7.7.65.37    LE Periodic Advertising Response Report event (0x28)
 *
 * PAwR turns one periodic advertising train into up to 0x80 subevents per
 * event, each carrying its own payload and a set of response slots that
 * synchronized devices can answer on. The train is created with the [v2]
 * parameters command; from then on the Controller asks for subevent payloads
 * through the 0x27 request event and the Host answers each request with
 * 0x82 commands, one command per subevent or a group of up to 0x0F subevents.
 * The payload of a subevent is transmitted once and then discarded by the
 * Controller; responses arriving on the response slots are reported through
 * the 0x28 event.
 *
 * Layering of the public surface (gap_le_if_5_4.h) with the pre-5.4 flows:
 *   - GAPIF_LePawrSetSubeventParams configures the train (7.8.61 [v2]); a
 *     numSubevents of 0x00 configures a plain periodic advertising train and
 *     the parameters are forwarded through the [v1] command, whose completion
 *     arrives at the legacy periodic advertising sync callback (the pre-5.4
 *     periodic advertising set-up of GAPIF_LePeriodicAdvSetParam /
 *     GAPIF_LePeriodicAdvSetData / GAPIF_LePeriodicAdvSetEnable is still used
 *     for the data and enable steps, in the order of the message sequence
 *     chart of the feature doc, MSC 3.9).
 *   - GAPIF_LePawrSetSubeventData buffers payloads for subevents; the module
 *     answers a 0x27 request with the buffered payloads it covers ("sent
 *     once" semantics, cleared on transmit, not on the completion) and hands
 *     the full requested window to the registered subeventDataRequest
 *     callback so a service can prefill the train for the next request.
 *   - The command completions of the [v2] parameters and of the 0x82 replies
 *     (both module-initiated and GAPIF-initiated) arrive at the
 *     setSubeventParamsResult / subeventDataResult callback members.
 *
 * All state (per-handle payload buffers and the registered callback) is
 * owned by the GAP task: the GAPIF_* entries run on the caller's thread and
 * delegate onto the GAP processing queue (GapRunTaskBlockProcess), while the
 * event/completion handlers arrive on the queue through gap_hci_receive.c.
 */

#include "gap_internal.h"
#include "gap_le_if.h"
#include "gap_le_if_5_4.h"
#include "gap_le.h"
#include "gap_task_internal.h"

#include <securec.h>

#include "allocator.h"
#include "log.h"

#include "btm.h"
#include "hci/hci.h"
#include "hci/hci_error.h"

#ifdef GAP_LE_SUPPORT

// Subevent numbers of a PAwR train (7.8.125): 0x00-0x7F per subevent,
// 0x01-0x80 per request/configuration.
#define GAP_PAWR_SUBEVENT_INDEX_MAX 0x7F
#define GAP_PAWR_MAX_NUM_SUBEVENTS 0x80
// One 0x82 command carries at most 0x0F subevents (7.8.125); a request window
// larger than that is answered in several commands.
#define GAP_PAWR_MAX_SUBEVENTS_PER_COMMAND 0x0F
// ... and its parameter block is further limited by the one-octet HCI
// parameter length field: 255 octets minus the 2 leading octets of the
// command leave 253 octets for the 4 + payload octets of each entry.
#define GAP_PAWR_MAX_SUBEVENT_DATA_BYTES_PER_CHUNK 253
// Wire ranges of the 7.8.61 [v2] timing tail (see the public header for the
// units): Subevent_Interval 0x06-0xFF, Response_Slot_Delay 0x00 or 0x01-0xFE,
// Response_Slot_Spacing 0x00 or 0x02-0xFF.
#define GAP_PAWR_SUBEVENT_INTERVAL_MIN 0x06
#define GAP_PAWR_SUBEVENT_INTERVAL_MAX 0xFF
#define GAP_PAWR_RESPONSE_SLOT_DELAY_MAX 0xFE
#define GAP_PAWR_RESPONSE_SLOT_SPACING_MAX 0xFF
#define GAP_PAWR_RESPONSE_SLOT_SPACING_RESERVED 0x01
// Factor of the response slot window rule: Response_Slot_Spacing x
// Num_Response_Slots <= 10 x (Subevent_Interval - Response_Slot_Delay)
// (7.8.61 [v2]).
#define GAP_PAWR_RESPONSE_SLOT_SPACING_FACTOR 10
// Transmittable payload of one subevent of a 0x82 command: the HCI command
// parameter length is one octet (hci_cmd.c), which caps a parameter block at
// 255 octets and leaves 255 - 2 (advertising handle + num subevents) - 4
// (entry header) = 249 octets for the payload of a lone entry. The spec wire
// range of the data is 0-251 (7.8.125, 252-0xFF reserved), but 250/251 can
// never be framed in any single command, so they are rejected at the API
// entry instead of being buffered until the flush of every 0x27 request
// fails on them.
#define GAP_PAWR_SUBEVENT_DATA_SEND_MAX 0xF9

// One buffered subevent payload (index = subevent number); the payload is
// kept from GAPIF_LePawrSetSubeventData until the Controller transmitted it
// in reply to a 0x27 request ("transmitted once" semantics of 7.8.125). A
// data pointer of NULL means the subevent has no buffered payload.
typedef struct {
    uint8_t responseSlotStart;  // First response slot to be used: 0x00-0xFF
    uint8_t responseSlotCount;  // Number of response slots to be used: 0x00-0xFF
    uint8_t dataLength;         // 0-251
    uint8_t *data;              // Heap payload, NULL when dataLength is 0
} PawrSubeventEntry;

// Buffered payloads of one advertising handle; one list node of
// g_pawrAdvDataList. Nodes are created lazily by the first
// GAPIF_LePawrSetSubeventData call of a handle and dropped again when the
// last payload of the handle has been transmitted.
typedef struct {
    uint8_t advertisingHandle;  // 0x00-0xEF
    PawrSubeventEntry subevent[GAP_PAWR_MAX_NUM_SUBEVENTS];
} PawrAdvDataBlock;

// Registered callback group; single slot, written on the GAP task through
// the registration entries (no locking needed, see the group comment above).
typedef struct {
    GapPawrAdvCallback callback;
    void *context;
} LePawrAdvCallback;

static List *g_pawrAdvDataList;
static LePawrAdvCallback g_lePawrAdvCallback;

static void GapPawrFreeAdvDataBlock(void *data)
{
    PawrAdvDataBlock *block = data;
    if (block == NULL) {
        return;
    }
    for (uint8_t i = 0; i < GAP_PAWR_MAX_NUM_SUBEVENTS; i++) {
        if (block->subevent[i].data != NULL) {
            MEM_MALLOC.free(block->subevent[i].data);
            block->subevent[i].data = NULL;
        }
    }
    MEM_MALLOC.free(block);
}

static bool GapPawrFindAdvDataBlockByHandle(void *nodeData, void *param)
{
    PawrAdvDataBlock *block = nodeData;
    uint8_t handle = *(uint8_t *)param;
    return block->advertisingHandle == handle;
}

static PawrAdvDataBlock *GapPawrFindAdvDataBlock(uint8_t advHandle)
{
    if (g_pawrAdvDataList == NULL) {
        return NULL;
    }
    return (PawrAdvDataBlock *)ListForEachData(g_pawrAdvDataList, GapPawrFindAdvDataBlockByHandle, &advHandle);
}

static bool GapPawrAdvDataBlockIsEmpty(const PawrAdvDataBlock *block)
{
    for (uint8_t i = 0; i < GAP_PAWR_MAX_NUM_SUBEVENTS; i++) {
        if (block->subevent[i].data != NULL) {
            return false;
        }
    }
    return true;
}

// Replace the buffered payload of one subevent with a heap copy of the given
// entry. The old payload is freed ("later calls replace the buffered data",
// documented on GAPIF_LePawrSetSubeventData). The new payload is allocated
// and copied before the old one is freed, so a failed update (out of memory,
// copy error) leaves the previously buffered data in place instead of
// discarding it first.
static bool GapPawrSetSubeventEntry(PawrAdvDataBlock *block, uint8_t subevent, const GapPawrSubeventData *entry)
{
    PawrSubeventEntry *stored = &block->subevent[subevent];

    uint8_t *data = NULL;
    if (entry->dataLength != 0) {
        data = MEM_MALLOC.alloc(entry->dataLength);
        if (data == NULL) {
            HILOGE("%{public}s: Alloc subevent data error.", __FUNCTION__);
            return false;
        }
        if (memcpy_s(data, entry->dataLength, entry->data, entry->dataLength) != EOK) {
            MEM_MALLOC.free(data);
            HILOGE("%{public}s: Copy subevent data error.", __FUNCTION__);
            return false;
        }
    }

    if (stored->data != NULL) {
        MEM_MALLOC.free(stored->data);
    }
    stored->data = data;
    stored->responseSlotStart = entry->responseSlotStart;
    stored->responseSlotCount = entry->responseSlotCount;
    stored->dataLength = (uint8_t)entry->dataLength;
    return true;
}

// Find the block of an advertising handle, creating it when the first payload
// of the handle is buffered. Returns NULL on allocation failure.
static PawrAdvDataBlock *GapPawrGetOrCreateAdvDataBlock(uint8_t advHandle)
{
    PawrAdvDataBlock *block = GapPawrFindAdvDataBlock(advHandle);
    if (block != NULL) {
        return block;
    }
    if (g_pawrAdvDataList == NULL) {
        HILOGE("%{public}s: Module not initialized.", __FUNCTION__);
        return NULL;
    }

    block = MEM_MALLOC.alloc(sizeof(PawrAdvDataBlock));
    if (block == NULL) {
        HILOGE("%{public}s: Alloc adv data block error.", __FUNCTION__);
        return NULL;
    }
    if (memset_s(block, sizeof(PawrAdvDataBlock), 0x00, sizeof(PawrAdvDataBlock)) != EOK) {
        MEM_MALLOC.free(block);
        return NULL;
    }
    block->advertisingHandle = advHandle;
    if (!ListAddLast(g_pawrAdvDataList, block)) {
        MEM_MALLOC.free(block);
        HILOGE("%{public}s: Add adv data block error.", __FUNCTION__);
        return NULL;
    }
    return block;
}

// Transmit one 0x82 command for the collected subevents and release their
// buffered payloads. The data pointers are only read during the HCI call
// (the HCI layer serializes the command synchronously); on a failure the
// payloads stay buffered for a later 0x27 request of the same subevents.
static void GapPawrSendSubeventDataChunk(
    PawrAdvDataBlock *block, const HciLeSetPeriodicAdvertisingSubeventDataSet *sets, uint8_t numSets)
{
    HciLeSetPeriodicAdvertisingSubeventDataParam cmdParam = {
        .advertisingHandle = block->advertisingHandle,
        .numSubevents = numSets,
        .sets = sets,
    };

    int ret = HCI_LeSetPeriodicAdvertisingSubeventData(&cmdParam);
    if (ret != BT_SUCCESS) {
        HILOGE("%{public}s: Send subevent data error: %{public}d.", __FUNCTION__, ret);
        return;
    }

    for (uint8_t i = 0; i < numSets; i++) {
        PawrSubeventEntry *stored = &block->subevent[sets[i].subevent];
        MEM_MALLOC.free(stored->data);
        stored->data = NULL;
        stored->dataLength = 0;
    }
}

// Answer a 0x27 request: transmit the buffered payloads of the requested
// window in 0x82 commands of up to 0x0F subevents each, and drop the node
// when the last payload of its handle has been transmitted.
static void GapPawrFlushSubeventData(PawrAdvDataBlock *block, uint8_t subeventStart, uint8_t subeventDataCount)
{
    HciLeSetPeriodicAdvertisingSubeventDataSet sets[GAP_PAWR_MAX_SUBEVENTS_PER_COMMAND];
    uint8_t numSets = 0;
    // A 0x82 command carries 2 leading octets and, per entry, 4 octets plus
    // its payload; the whole parameter block must fit the one-octet HCI
    // parameter length field (255 octets, hci_cmd.c), so the entries of one
    // command are additionally limited by byte count (7.8.125 only limits
    // the entry count to 0x0F per command).
    uint16_t chunkBytes = 0;
    uint16_t windowEnd = (uint16_t)subeventStart + subeventDataCount;

    for (uint16_t subevent = subeventStart; subevent < windowEnd; subevent++) {
        const PawrSubeventEntry *stored = &block->subevent[subevent];
        if (stored->data == NULL) {
            continue;
        }
        uint16_t entryBytes = 4 + stored->dataLength;
        if (numSets > 0 && chunkBytes + entryBytes > GAP_PAWR_MAX_SUBEVENT_DATA_BYTES_PER_CHUNK) {
            GapPawrSendSubeventDataChunk(block, sets, numSets);
            numSets = 0;
            chunkBytes = 0;
        }
        // GAPIF_LePawrSetSubeventData caps an entry payload at 249 octets
        // (GAP_PAWR_SUBEVENT_DATA_SEND_MAX), so every buffered entry fits a
        // chunk on its own; the send attempt below can still fail on a
        // transport error, in which case the payloads stay buffered for a
        // later 0x27 request of the same subevents.
        sets[numSets].subevent = (uint8_t)subevent;
        sets[numSets].responseSlotStart = stored->responseSlotStart;
        sets[numSets].responseSlotCount = stored->responseSlotCount;
        sets[numSets].subeventDataLength = stored->dataLength;
        sets[numSets].subeventData = stored->data;
        numSets++;
        chunkBytes += entryBytes;

        // One 0x82 command carries at most 0x0F entries (7.8.125); the
        // remaining collected entries are sent after the loop.
        if (numSets == GAP_PAWR_MAX_SUBEVENTS_PER_COMMAND) {
            GapPawrSendSubeventDataChunk(block, sets, numSets);
            numSets = 0;
            chunkBytes = 0;
        }
    }

    // The buffered payloads may cover only part of the requested window
    // (e.g. its trailing subevents have nothing buffered yet): send the
    // collected entries so the Controller receives the data it asked for and
    // re-requests the remainder (7.7.65,36). An all-empty window must not
    // produce an empty 0x82 command (the HCI sender rejects numSubevents
    // 0x00); the Controller then simply transmits the subevents without data
    // and may request again.
    if (numSets > 0) {
        GapPawrSendSubeventDataChunk(block, sets, numSets);
    }

    if (GapPawrAdvDataBlockIsEmpty(block)) {
        // ListRemoveNode already frees the block through the list freeCb
        // installed at ListCreate (GapPawrFreeAdvDataBlock); freeing it again
        // here would release the same block twice.
        ListRemoveNode(g_pawrAdvDataList, block);
    }
}

int GapLePawrAdvInit(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (g_pawrAdvDataList == NULL) {
        g_pawrAdvDataList = ListCreate(GapPawrFreeAdvDataBlock);
        if (g_pawrAdvDataList == NULL) {
            LOG_ERROR("%{public}s: Create adv data list error.", __FUNCTION__);
            return GAP_ERR_OUT_OF_RES;
        }
    }

    if (memset_s(&g_lePawrAdvCallback, sizeof(LePawrAdvCallback), 0x00, sizeof(LePawrAdvCallback)) != EOK) {
        LOG_ERROR("%{public}s: Clear callback error.", __FUNCTION__);
        return GAP_ERR_OUT_OF_RES;
    }
    return GAP_SUCCESS;
}

void GapLePawrAdvDeinit(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (g_pawrAdvDataList != NULL) {
        ListDelete(g_pawrAdvDataList);
        g_pawrAdvDataList = NULL;
    }
    (void)memset_s(&g_lePawrAdvCallback, sizeof(LePawrAdvCallback), 0x00, sizeof(LePawrAdvCallback));
}

typedef struct {
    int result;
    uint8_t advHandle;
    uint16_t intervalMin;
    uint16_t intervalMax;
    uint8_t numSubevents;
    uint16_t subeventInterval;
    uint16_t responseSlotDelay;
    uint16_t responseSlotSpacing;
    uint8_t numResponseSlots;
} LePawrSetSubeventParamsInfo;

// Validate the PAwR timing tail of a [v2] parameter set (7.8.61 [v2]). The
// subevent interval is checked whenever subevents are configured; the
// response slot values are only validated while they are meaningful:
// Response_Slot_Delay is ignored when Num_Response_Slots is 0x00 and
// Response_Slot_Spacing when Num_Response_Slots is 0x00 or 0x01.
static int GapPawrCheckSetSubeventParamsTiming(const LePawrSetSubeventParamsInfo *info)
{
    if (info->subeventInterval < GAP_PAWR_SUBEVENT_INTERVAL_MIN ||
        info->subeventInterval > GAP_PAWR_SUBEVENT_INTERVAL_MAX ||
        (info->numResponseSlots > 0 && info->responseSlotDelay > GAP_PAWR_RESPONSE_SLOT_DELAY_MAX) ||
        (info->numResponseSlots > 0x01 &&
            (info->responseSlotSpacing > GAP_PAWR_RESPONSE_SLOT_SPACING_MAX ||
                info->responseSlotSpacing == GAP_PAWR_RESPONSE_SLOT_SPACING_RESERVED))) {
        return GAP_ERR_INVAL_PARAM;
    }

    // Subevent_Interval x Num_Subevents <= Periodic_Advertising_Interval_Min
    // (7.8.61 [v2]).
    if ((uint32_t)info->subeventInterval * info->numSubevents > (uint32_t)info->intervalMin) {
        return GAP_ERR_INVAL_PARAM;
    }
    // Response_Slot_Delay < Subevent_Interval once response slots are used.
    if (info->numResponseSlots > 0 && info->responseSlotDelay != 0x00 &&
        info->responseSlotDelay >= info->subeventInterval) {
        return GAP_ERR_INVAL_PARAM;
    }
    // Response_Slot_Spacing x Num_Response_Slots <= 10 x
    // (Subevent_Interval - Response_Slot_Delay) once more than one response
    // slot is used; Response_Slot_Spacing is ignored otherwise (7.8.61 [v2]).
    if (info->numResponseSlots >= 0x02) {
        if (info->responseSlotDelay >= info->subeventInterval) {
            return GAP_ERR_INVAL_PARAM;
        }
        uint32_t spacingWindow = (uint32_t)info->responseSlotSpacing * info->numResponseSlots;
        uint32_t delayReducedInterval = (uint32_t)(info->subeventInterval - info->responseSlotDelay);
        if (spacingWindow > GAP_PAWR_RESPONSE_SLOT_SPACING_FACTOR * delayReducedInterval) {
            return GAP_ERR_INVAL_PARAM;
        }
    }
    return GAP_SUCCESS;
}

// Send the [v2] parameters command for the validated parameter set of the
// task context; the HCI status is reported through info->result to the
// synchronous caller and the completion arrives later at
// GapLeSetPeriodicAdvertisingParametersV2Complete.
static void GapPawrSendSetSubeventParamsV2(LePawrSetSubeventParamsInfo *info)
{
    HciLeSetPeriodicAdvertisingParametersV2Param hciCmdParam = {
        .advertisingHandle = info->advHandle,
        .periodicAdvertisingIntervalMin = info->intervalMin,
        .periodicAdvertisingIntervalMax = info->intervalMax,
        .periodicAdvertisingProperties = 0x0000,
        .numSubevents = info->numSubevents,
        .subeventInterval = (uint8_t)info->subeventInterval,
        .responseSlotDelay = (uint8_t)info->responseSlotDelay,
        .responseSlotSpacing = (uint8_t)info->responseSlotSpacing,
        .numResponseSlots = info->numResponseSlots,
    };

    int ret = HCI_LeSetPeriodicAdvertisingParametersV2(&hciCmdParam);
    if (ret != BT_SUCCESS) {
        HILOGE("%{public}s: Send set periodic advertising parameters v2 error: %{public}d.", __FUNCTION__, ret);
    }
    info->result = ret;
}

// Core of GAPIF_LePawrSetSubeventParams, running on the GAP task. Validates
// like the pre-5.4 GAP_LePeriodicAdvSetParam and then selects the command:
// numSubevents of 0x00 delegates to the [v1] command (identical parameter
// set, completion arrives at the legacy callback), numSubevents of 0x01-0x80
// uses the [v2] command with the PAwR timing tail.
static void GapLePawrSetSubeventParamsTask(void *ctx)
{
    LePawrSetSubeventParamsInfo *info = ctx;

    if (GapIsLeEnable() == false) {
        info->result = GAP_ERR_NOT_ENABLE;
        return;
    }

    if (info->advHandle > GAP_PERIODIC_ADV_HANDLE_MAX ||
        info->intervalMin < GAP_PERIODIC_ADV_INTERVAL_MIN ||
        info->intervalMin > GAP_PERIODIC_ADV_INTERVAL_MAX ||
        info->intervalMax < GAP_PERIODIC_ADV_INTERVAL_MIN ||
        info->intervalMax > GAP_PERIODIC_ADV_INTERVAL_MAX ||
        info->intervalMin > info->intervalMax ||
        info->numSubevents > GAP_PAWR_MAX_NUM_SUBEVENTS) {
        info->result = GAP_ERR_INVAL_PARAM;
        return;
    }

    if (info->numSubevents == 0x00) {
        // Plain periodic advertising train: the [v1] command. The PAwR tail
        // is ignored by the Controller (7.8.61 [v2]) and not validated here;
        // the result arrives at the legacy periodic advertising callback.
        info->result = GAP_LePeriodicAdvSetParam(
            info->advHandle, info->intervalMin, info->intervalMax, 0x0000);
        return;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL) == false) {
        info->result = GAP_ERR_INVAL_STATE;
        return;
    }

    if (!BTM_IsControllerSupportPawrAdvertiser()) {
        info->result = GAP_ERR_NOT_SUPPORT;
        return;
    }

    if (GapPawrCheckSetSubeventParamsTiming(info) != GAP_SUCCESS) {
        info->result = GAP_ERR_INVAL_PARAM;
        return;
    }
    GapPawrSendSetSubeventParamsV2(info);
}

int GAPIF_LePawrSetSubeventParams(uint8_t advHandle, const GapPawrSubeventParams *params)
{
    if (params == NULL) {
        return BT_BAD_PARAM;
    }
    if (advHandle > GAP_LE_ADV_HANDLE_MAX || params->numSubevents > GAP_PAWR_MAX_NUM_SUBEVENTS) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: advHandle:%hhu numSubevents:%hhu", __FUNCTION__, advHandle, params->numSubevents);
    LePawrSetSubeventParamsInfo *ctx = MEM_MALLOC.alloc(sizeof(LePawrSetSubeventParamsInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(LePawrSetSubeventParamsInfo), 0x00, sizeof(LePawrSetSubeventParamsInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->advHandle = advHandle;
    ctx->intervalMin = params->intervalMin;
    ctx->intervalMax = params->intervalMax;
    ctx->numSubevents = params->numSubevents;
    ctx->subeventInterval = params->subeventInterval;
    ctx->responseSlotDelay = params->responseSlotDelay;
    ctx->responseSlotSpacing = params->responseSlotSpacing;
    ctx->numResponseSlots = params->numResponseSlots;

    int ret = GapRunTaskBlockProcess(GapLePawrSetSubeventParamsTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

typedef struct {
    int result;
    uint8_t advHandle;
    uint16_t subeventStart;
    uint16_t subeventDataCount;
    GapPawrSubeventData entry[GAP_PAWR_MAX_NUM_SUBEVENTS];
    // The concatenated payload bytes of the entries follow this struct; the
    // entry[i].data pointers are adjusted into that area by the caller.
} LePawrSetSubeventDataInfo;

// Reject entries whose payload can never be framed in a 0x82 command and sum
// their payload bytes. 250/251 are spec-valid but can never fit a single 0x82
// command (see GAP_PAWR_SUBEVENT_DATA_SEND_MAX); higher values are beyond the
// wire range. Rejecting here keeps the buffer flushable. Returns the total
// payload length, or a negative value on an invalid entry.
static int GapPawrCheckSubeventDataEntries(uint16_t subeventDataCount, const GapPawrSubeventData data[])
{
    uint32_t totalDataLength = 0;
    for (uint16_t i = 0; i < subeventDataCount; i++) {
        if (data[i].dataLength > GAP_PAWR_SUBEVENT_DATA_SEND_MAX ||
            (data[i].dataLength != 0 && data[i].data == NULL)) {
            return -1;
        }
        totalDataLength += data[i].dataLength;
    }
    return (int)totalDataLength;
}

// Fill the task context with the caller's entries: copy each payload into the
// tail area behind the struct and point the entry's data field at the copy.
// Returns BT_OPERATION_FAILED when a payload copy fails (the caller then
// discards the context).
static int GapPawrFillSubeventDataContext(
    LePawrSetSubeventDataInfo *ctx, uint16_t subeventDataCount, const GapPawrSubeventData data[])
{
    uint8_t *payloads = (uint8_t *)ctx + sizeof(LePawrSetSubeventDataInfo);
    for (uint16_t i = 0; i < subeventDataCount; i++) {
        ctx->entry[i].responseSlotStart = data[i].responseSlotStart;
        ctx->entry[i].responseSlotCount = data[i].responseSlotCount;
        ctx->entry[i].dataLength = data[i].dataLength;
        ctx->entry[i].data = payloads;
        if (data[i].dataLength != 0) {
            if (memcpy_s(payloads, data[i].dataLength, data[i].data, data[i].dataLength) != EOK) {
                return BT_OPERATION_FAILED;
            }
            payloads += data[i].dataLength;
        }
    }
    return BT_SUCCESS;
}

// Core of GAPIF_LePawrSetSubeventData, running on the GAP task: buffer the
// entries on the advertising handle, replacing the buffered payloads of the
// covered subevents.
static void GapLePawrSetSubeventDataTask(void *ctx)
{
    LePawrSetSubeventDataInfo *info = ctx;
    uint8_t *payloads = (uint8_t *)info + sizeof(LePawrSetSubeventDataInfo);

    if (info->advHandle > GAP_PERIODIC_ADV_HANDLE_MAX ||
        info->subeventStart > GAP_PAWR_SUBEVENT_INDEX_MAX ||
        info->subeventDataCount == 0x00 ||
        info->subeventDataCount > GAP_PAWR_MAX_NUM_SUBEVENTS ||
        info->subeventStart + info->subeventDataCount > GAP_PAWR_MAX_NUM_SUBEVENTS) {
        info->result = GAP_ERR_INVAL_PARAM;
        return;
    }

    PawrAdvDataBlock *block = GapPawrGetOrCreateAdvDataBlock(info->advHandle);
    if (block == NULL) {
        info->result = GAP_ERR_OUT_OF_RES;
        return;
    }

    for (uint16_t i = 0; i < info->subeventDataCount; i++) {
        uint8_t subevent = (uint8_t)(info->subeventStart + i);
        GapPawrSubeventData entry = {
            .responseSlotStart = info->entry[i].responseSlotStart,
            .responseSlotCount = info->entry[i].responseSlotCount,
            .dataLength = info->entry[i].dataLength,
            .data = payloads,
        };
        if (!GapPawrSetSubeventEntry(block, subevent, &entry)) {
            info->result = GAP_ERR_OUT_OF_RES;
            return;
        }
        payloads += info->entry[i].dataLength;
    }

    info->result = GAP_SUCCESS;
}

int GAPIF_LePawrSetSubeventData(uint8_t advHandle, uint16_t subeventStart, uint16_t subeventDataCount,
    const GapPawrSubeventData data[])
{
    if (advHandle > GAP_LE_ADV_HANDLE_MAX || subeventStart > GAP_PAWR_SUBEVENT_INDEX_MAX ||
        subeventDataCount == 0x00 || subeventDataCount > GAP_PAWR_MAX_NUM_SUBEVENTS ||
        subeventStart + subeventDataCount > GAP_PAWR_MAX_NUM_SUBEVENTS || data == NULL) {
        return BT_BAD_PARAM;
    }

    int totalDataLength = GapPawrCheckSubeventDataEntries(subeventDataCount, data);
    if (totalDataLength < 0) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: advHandle:%hhu start:%hu count:%hu", __FUNCTION__, advHandle, subeventStart,
        subeventDataCount);
    LePawrSetSubeventDataInfo *ctx = MEM_MALLOC.alloc(sizeof(LePawrSetSubeventDataInfo) + totalDataLength);
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(LePawrSetSubeventDataInfo) + totalDataLength,
        0x00, sizeof(LePawrSetSubeventDataInfo) + totalDataLength) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->advHandle = advHandle;
    ctx->subeventStart = subeventStart;
    ctx->subeventDataCount = subeventDataCount;

    int ret = GapPawrFillSubeventDataContext(ctx, subeventDataCount, data);
    if (ret != BT_SUCCESS) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return ret;
    }

    ret = GapRunTaskBlockProcess(GapLePawrSetSubeventDataTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

typedef struct {
    int result;
    GapPawrAdvCallback callback;
    void *context;
} LePawrRegisterCallbackInfo;

static void GapLePawrRegisterCallbackTask(void *ctx)
{
    LePawrRegisterCallbackInfo *info = ctx;
    g_lePawrAdvCallback.callback = info->callback;
    g_lePawrAdvCallback.context = info->context;
    info->result = BT_SUCCESS;
}

int GAPIF_RegisterPawrAdvCallback(const GapPawrAdvCallback *callback, void *context)
{
    if (callback == NULL) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s:", __FUNCTION__);
    LePawrRegisterCallbackInfo *ctx = MEM_MALLOC.alloc(sizeof(LePawrRegisterCallbackInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(LePawrRegisterCallbackInfo), 0x00, sizeof(LePawrRegisterCallbackInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->callback = *callback;
    ctx->context = context;

    int ret = GapRunTaskBlockProcess(GapLePawrRegisterCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLePawrDeregisterCallbackTask(void *ctx)
{
    (void)ctx;
    (void)memset_s(&g_lePawrAdvCallback, sizeof(LePawrAdvCallback), 0x00, sizeof(LePawrAdvCallback));
}

int GAPIF_DeregisterPawrAdvCallback(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    int ret = GapRunTaskBlockProcess(GapLePawrDeregisterCallbackTask, NULL);
    return ret;
}

// Completion of the [v2] parameters command sent by GapLePawrSetSubeventParamsTask
// for the PAwR train (7.8.61 [v2]); the result is reported through the
// setSubeventParamsResult callback member.
NO_SANITIZE("cfi")
void GapLeSetPeriodicAdvertisingParametersV2Complete(
    const HciLeSetPeriodicAdvertisingParametersV2ReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    HILOGD("%{public}s: status:0x%{public}02x", __FUNCTION__, param->status);
    if (g_lePawrAdvCallback.callback.setSubeventParamsResult) {
        g_lePawrAdvCallback.callback.setSubeventParamsResult(
            param->status, param->advertisingHandle, g_lePawrAdvCallback.context);
    }
}

// HCI_LE_Set_Periodic_Advertising_Subevent_Data completion (7.8.125), for the
// commands sent in reply to a 0x27 request and for the GAPIF_LePawrSetSubeventData
// path. Data discarded by the Controller (0x45/0x46/0x47) is not re-buffered.
NO_SANITIZE("cfi")
void GapLeSetPeriodicAdvertisingSubeventDataComplete(
    const HciLeSetPeriodicAdvertisingSubeventDataReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    HILOGD("%{public}s: status:0x%{public}02x", __FUNCTION__, param->status);
    if (g_lePawrAdvCallback.callback.subeventDataResult) {
        g_lePawrAdvCallback.callback.subeventDataResult(
            param->status, param->advertisingHandle, g_lePawrAdvCallback.context);
    }
}

// LE Periodic Advertising Subevent Data Request event (7.7.65,36): the
// Controller asks for the payloads of an upcoming subevent window. Reply
// with the buffered payloads the window covers, then report the window to
// the service so it can prefill the train for the following requests.
NO_SANITIZE("cfi")
void GapOnLePeriodicAdvertisingSubeventDataRequestEvent(
    const HciLePeriodicAdvertisingSubeventDataRequestEventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }

    // The buffered subevent data is organized per handle as one 0x00-0x7F
    // ring modeled by a linear window (see GapPawrFindAdvDataBlock), so a
    // request whose window wraps around the ring - Start + Count > 0x80,
    // which the spec (7.7.65,36) allows the Controller to issue - cannot be
    // answered from the buffer and is dropped deliberately: its subevents
    // stay unanswered and the subeventDataRequest callback is not invoked,
    // keeping the buffered entries intact for the next, non-wrapping request.
    if (eventParam->advertisingHandle > GAP_PERIODIC_ADV_HANDLE_MAX ||
        eventParam->subeventStart > GAP_PAWR_SUBEVENT_INDEX_MAX ||
        eventParam->subeventDataCount == 0x00 ||
        (uint16_t)eventParam->subeventStart + eventParam->subeventDataCount > GAP_PAWR_MAX_NUM_SUBEVENTS) {
        HILOGE("%{public}s: Invalid request, dropping.", __FUNCTION__);
        return;
    }

    HILOGD("%{public}s: advHandle:0x%{public}02x start:0x%{public}02x count:0x%{public}02x",
        __FUNCTION__, eventParam->advertisingHandle, eventParam->subeventStart, eventParam->subeventDataCount);

    if (g_pawrAdvDataList != NULL) {
        PawrAdvDataBlock *block = GapPawrFindAdvDataBlock(eventParam->advertisingHandle);
        if (block != NULL) {
            GapPawrFlushSubeventData(
                block, eventParam->subeventStart, eventParam->subeventDataCount);
        }
    }

    if (g_lePawrAdvCallback.callback.subeventDataRequest) {
        g_lePawrAdvCallback.callback.subeventDataRequest(eventParam->advertisingHandle,
            eventParam->subeventStart, eventParam->subeventDataCount, g_lePawrAdvCallback.context);
    }
}

// LE Periodic Advertising Response Report event (7.7.65,37): responses
// received on the response slots of a PAwR train. The record data pointers
// of the event parameter reference the heap copy of the received payloads
// (made by gap_hci_receive.c); they stay valid until the handler returns, so
// the public report can reference them directly.
NO_SANITIZE("cfi")
void GapOnLePeriodicAdvertisingResponseReportEvent(
    const HciLePeriodicAdvertisingResponseReportEventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }

    if (eventParam->numResponses > GAP_PAWR_RESPONSE_REPORT_NUM_MAX) {
        HILOGE("%{public}s: Too many responses: %{public}hhu, dropping.", __FUNCTION__,
            eventParam->numResponses);
        return;
    }

    GapPawrResponseReport report;
    (void)memset_s(&report, sizeof(GapPawrResponseReport), 0x00, sizeof(GapPawrResponseReport));
    report.advertisingHandle = eventParam->advertisingHandle;
    report.subevent = eventParam->subevent;
    report.txStatus = eventParam->txStatus;
    report.numResponses = eventParam->numResponses;
    for (uint8_t i = 0; i < eventParam->numResponses; i++) {
        report.response[i].txPower = eventParam->response[i].txPower;
        report.response[i].rssi = eventParam->response[i].rssi;
        report.response[i].cteType = eventParam->response[i].cteType;
        report.response[i].responseSlot = eventParam->response[i].responseSlot;
        report.response[i].dataStatus = eventParam->response[i].dataStatus;
        report.response[i].dataLength = eventParam->response[i].dataLength;
        report.response[i].data = eventParam->response[i].data;
    }

    if (g_lePawrAdvCallback.callback.responseReport) {
        g_lePawrAdvCallback.callback.responseReport(&report, g_lePawrAdvCallback.context);
    }
}

#endif /* GAP_LE_SUPPORT */
