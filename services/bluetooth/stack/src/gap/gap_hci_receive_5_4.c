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

/* Bluetooth 5.4 PAwR (periodic advertising with responses) LE event and
 * command-complete receivers, Core Spec 5.4 Vol 4 Part E:
 *   - 7.7.65.14   LE Periodic Advertising Sync Established event (0x24, [v2])
 *   - 7.7.65.15   LE Periodic Advertising Report event (0x25, [v2])
 *   - 7.7.65.36   LE Periodic Advertising Subevent Data Request event (0x27)
 *   - 7.7.65.37   LE Periodic Advertising Response Report event (0x28)
 *   - 7.7.65.24   LE Periodic Advertising Sync Transfer Received event (0x26, [v2])
 *   - 7.8.61 [v2] LE Set Periodic Advertising Parameters [v2] complete (OCF 0x0086)
 *   - 7.8.118     LE Set Periodic Advertising Subevent Data complete (OCF 0x0082)
 *   - 7.8.119     LE Set Periodic Advertising Response Data complete (OCF 0x0083)
 *   - 7.8.122     LE Set Periodic Sync Subevent complete (OCF 0x0084)
 *
 * Split out of gap_hci_receive.c to keep that file under the source size
 * limit; the receivers keep the bridging model of the pre-split code and
 * are referenced by the static callback table in gap_hci_receive.c (their
 * prototypes live in that file's forward-declaration block). Every 0x24/0x25/
 * 0x26 event carries the [v1] parameters first, so the legacy sync state
 * machine (gap_le_scan.c) keeps consuming the [v1] prefix through the shared
 * task handlers, and the PAwR handling (gap_le_pawr_adv.c / gap_le_pawr_sync.c)
 * receives the full [v2] parameters through a second task post.
 */

#include "gap_internal.h"
#include "gap_task_internal.h"

#include <securec.h>

#include "allocator.h"
#include "log.h"
#include "thread.h"

#include "btm/btm_thread.h"

// Task-post bridging helper and shared [v1] report-event free func, defined
// in gap_hci_receive.c and exported for the receivers below; their contracts
// are documented at the definitions.
int GapProcessHciEventInTask(TaskFunc run, const void *ctx, uint32_t ctxLen, TaskFunc freeFunc);
void GapFreeLePeriodicAdvertisingReportEvent(void *ctx);

#ifdef GAP_LE_SUPPORT
// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E 7.7.65.14: subevent
// code 0x24 carries the [v1] parameters (0x0E) first, followed by
// Num_Subevents, Subevent_Interval, Response_Slot_Delay and
// Response_Slot_Spacing. A 5.4 Controller with the [v2] periodic event bits
// enabled reports every sync establishment - PAwR train or not - as 0x24, so
// the legacy periodic-sync state machine keeps consuming the [v1] prefix via
// the shared task handler; the trailing subevent/response-slot parameters
// are the PAwR material consumed by the sync-side PAwR handling
// (gap_le_pawr_sync.c), which receives the full [v2] parameters through a
// second task post.
void GapRecvLePeriodicAdvertisingSyncEstablishedV2Event(
    const HciLePeriodicAdvertisingSyncEstablishedV2EventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }

    HILOGI("status: 0x%{public}02x, syncHandle: 0x%{public}04x, sid: %{public}hhu, numSubevents: %{public}hhu",
        eventParam->status,
        eventParam->syncHandle,
        eventParam->advertisingSid,
        eventParam->numSubevents);
    const HciLePeriodicAdvertisingSyncEstablishedEventParam *v1Param =
        (const HciLePeriodicAdvertisingSyncEstablishedEventParam *)eventParam;
    int ret = GapProcessHciEventInTask(
        (TaskFunc)GapOnLePeriodicAdvertisingSyncEstablishedEvent, v1Param, sizeof(*v1Param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
    ret = GapProcessHciEventInTask(
        (TaskFunc)GapOnLePawrSyncEstablishedEvent, eventParam, sizeof(*eventParam), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

static void GapFreeLePeriodicAdvertisingReportV2Event(void *ctx)
{
    // ctx is always a heap copy of HciLePeriodicAdvertisingReportV2EventParam
    // produced by GapProcessHciEventInTask, so it is safe to free the struct
    // itself here (the cleanup fully owns the context).
    if (ctx == NULL) {
        return;
    }
    HciLePeriodicAdvertisingReportV2EventParam *hciParam = ctx;
    if (hciParam->data != NULL) {
        uint8_t *data = (uint8_t *)hciParam->data;
        hciParam->data = NULL;
        MEM_MALLOC.free(data);
    }
    MEM_MALLOC.free(hciParam);
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E 7.7.65.15: subevent
// code 0x25 inserts Periodic_Event_Counter and Subevent between CTE_Type and
// Data_Status of the [v1] report (0x0F). A 5.4 Controller with the [v2]
// periodic event bits enabled reports every periodic advertising event as
// 0x25, so the payload is deep-copied and handed to the legacy sync flow as
// the [v1] shape; the event is additionally forwarded with its full [v2]
// layout to the sync-side PAwR handling (gap_le_pawr_sync.c), which needs
// its own copy of the payload. Subevent carries 0xFF for trains without
// subevents.
// Deep-copies the report payload into one heap buffer owned by the [v1] or
// [v2] post below (each post hands its copy to its free func). Returns NULL
// for a payload-less report; when the payload cannot be copied the function
// logs and returns NULL and the report is dropped.
static uint8_t *GapCopyLePeriodicAdvertisingReportPayload(
    const HciLePeriodicAdvertisingReportV2EventParam *eventParam)
{
    if (eventParam->dataLength == 0) {
        return NULL;
    }
    uint8_t *data = MEM_MALLOC.alloc(eventParam->dataLength);
    if (data == NULL) {
        HILOGE("Alloc report data error.");
        return NULL;
    }
    if (memcpy_s(data, eventParam->dataLength, eventParam->data, eventParam->dataLength) != EOK) {
        MEM_MALLOC.free(data);
        HILOGE("Copy report data error.");
        return NULL;
    }
    return data;
}

// First task post: the [v1] report shape consumed by the legacy periodic
// sync flow. Returns false when the payload copy failed (the caller then
// drops the report, skipping the [v2] post as well); the reason was already
// logged by the copy helper above.
static bool GapPostLePeriodicAdvertisingReportV1(const HciLePeriodicAdvertisingReportV2EventParam *eventParam)
{
    HciLePeriodicAdvertisingReportEventParam hciParam = {
        .syncHandle = eventParam->syncHandle,
        .txPower = eventParam->txPower,
        .rssi = eventParam->rssi,
        .cteType = eventParam->cteType,
        .dataStatus = eventParam->dataStatus,
        .dataLength = eventParam->dataLength,
        .data = NULL,
    };
    hciParam.data = GapCopyLePeriodicAdvertisingReportPayload(eventParam);
    if (eventParam->dataLength > 0 && hciParam.data == NULL) {
        return false;
    }
    int ret = GapProcessHciEventInTask((TaskFunc)GapOnLePeriodicAdvertisingReportEvent,
        &hciParam,
        sizeof(hciParam),
        GapFreeLePeriodicAdvertisingReportEvent);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
    return true;
}

// Second task post for the sync-side PAwR handling with the full [v2] layout;
// the report payload of this post is an independent copy (the [v1] post takes
// its copy with it).
static void GapPostLePeriodicAdvertisingReportV2(const HciLePeriodicAdvertisingReportV2EventParam *eventParam)
{
    HciLePeriodicAdvertisingReportV2EventParam hciV2Param = {
        .syncHandle = eventParam->syncHandle,
        .txPower = eventParam->txPower,
        .rssi = eventParam->rssi,
        .cteType = eventParam->cteType,
        .periodicEventCounter = eventParam->periodicEventCounter,
        .subevent = eventParam->subevent,
        .dataStatus = eventParam->dataStatus,
        .dataLength = eventParam->dataLength,
        .data = NULL,
    };
    hciV2Param.data = GapCopyLePeriodicAdvertisingReportPayload(eventParam);
    if (eventParam->dataLength > 0 && hciV2Param.data == NULL) {
        return;
    }
    int ret = GapProcessHciEventInTask((TaskFunc)GapOnLePawrSyncReportEvent,
        &hciV2Param,
        sizeof(hciV2Param),
        GapFreeLePeriodicAdvertisingReportV2Event);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void GapRecvLePeriodicAdvertisingReportV2Event(const HciLePeriodicAdvertisingReportV2EventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }

    HILOGD("syncHandle: 0x%{public}04x, subevent: 0x%{public}02x, dataLen: %{public}hhu",
        eventParam->syncHandle,
        eventParam->subevent,
        eventParam->dataLength);

    if (eventParam->dataLength > GAP_PERIODIC_ADV_DATA_LENGTH_MAX) {
        HILOGE("Periodic advertising report data length too large: %{public}hhu.", eventParam->dataLength);
        return;
    }

    if (eventParam->dataLength > 0 && eventParam->data == NULL) {
        HILOGW("Malformed periodic advertising report: non-zero length but no payload. Dropping.");
        return;
    }

    if (!GapPostLePeriodicAdvertisingReportV1(eventParam)) {
        return;
    }
    GapPostLePeriodicAdvertisingReportV2(eventParam);
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E 7.8.61 [v2]: command
// completion of the PAwR parameters command (OCF 0x0086), forwarded to the
// advertiser-side PAwR handling (gap_le_pawr_adv.c).
void GapRecvLeSetPeriodicAdvertisingParametersV2Complete(
    const HciLeSetPeriodicAdvertisingParametersV2ReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    HILOGD("status: 0x%{public}02x, advHandle: 0x%{public}02x", param->status, param->advertisingHandle);
    int ret = GapProcessHciEventInTask(
        (TaskFunc)GapLeSetPeriodicAdvertisingParametersV2Complete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E 7.8.125: command
// completion of LE Set Periodic Advertising Subevent Data (OCF 0x0082),
// forwarded to the advertiser-side PAwR handling (gap_le_pawr_adv.c).
void GapRecvLeSetPeriodicAdvertisingSubeventDataComplete(
    const HciLeSetPeriodicAdvertisingSubeventDataReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    HILOGD("status: 0x%{public}02x, advHandle: 0x%{public}02x", param->status, param->advertisingHandle);
    int ret = GapProcessHciEventInTask(
        (TaskFunc)GapLeSetPeriodicAdvertisingSubeventDataComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E 7.8.126: command
// completion of LE Set Periodic Advertising Response Data (OCF 0x0083),
// forwarded to the sync-side PAwR handling (gap_le_pawr_sync.c).
void GapRecvLeSetPeriodicAdvertisingResponseDataComplete(
    const HciLeSetPeriodicAdvertisingResponseDataReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    HILOGD("status: 0x%{public}02x, syncHandle: 0x%{public}04x", param->status, param->syncHandle);
    int ret = GapProcessHciEventInTask(
        (TaskFunc)GapLeSetPeriodicAdvertisingResponseDataComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E 7.8.127: command
// completion of LE Set Periodic Sync Subevent (OCF 0x0084), forwarded to the
// sync-side PAwR handling (gap_le_pawr_sync.c).
void GapRecvLeSetPeriodicSyncSubeventComplete(const HciLeSetPeriodicSyncSubeventReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    HILOGD("status: 0x%{public}02x, syncHandle: 0x%{public}04x", param->status, param->syncHandle);
    int ret = GapProcessHciEventInTask(
        (TaskFunc)GapLeSetPeriodicSyncSubeventComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E 7.7.65.36: the flat
// LE Periodic Advertising Subevent Data Request event (0x27), forwarded to
// the advertiser-side PAwR handling (gap_le_pawr_adv.c).
void GapRecvLePeriodicAdvertisingSubeventDataRequestEvent(
    const HciLePeriodicAdvertisingSubeventDataRequestEventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }

    HILOGD("advHandle: 0x%{public}02x, start: 0x%{public}02x, count: 0x%{public}02x",
        eventParam->advertisingHandle, eventParam->subeventStart, eventParam->subeventDataCount);
    int ret = GapProcessHciEventInTask(
        (TaskFunc)GapOnLePeriodicAdvertisingSubeventDataRequestEvent, eventParam, sizeof(*eventParam), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

// Release the per-record payload slices of a response report. Usable on the
// receiver's stack copy as well as on the heap copy owned by the free func
// below; the struct itself is only released by the free func.
static void GapFreeLePeriodicAdvertisingResponseReportSlices(
    HciLePeriodicAdvertisingResponseReportEventParam *hciParam)
{
    for (uint8_t i = 0; i < hciParam->numResponses; i++) {
        if (hciParam->response[i].data != NULL) {
            uint8_t *data = (uint8_t *)hciParam->response[i].data;
            hciParam->response[i].data = NULL;
            MEM_MALLOC.free(data);
        }
    }
}

static void GapFreeLePeriodicAdvertisingResponseReportEvent(void *ctx)
{
    // ctx is always a heap copy of HciLePeriodicAdvertisingResponseReportEventParam
    // produced by GapProcessHciEventInTask, with one heap slice per record
    // payload; the cleanup releases the slices and the struct itself.
    if (ctx == NULL) {
        return;
    }
    HciLePeriodicAdvertisingResponseReportEventParam *hciParam = ctx;
    GapFreeLePeriodicAdvertisingResponseReportSlices(hciParam);
    MEM_MALLOC.free(hciParam);
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E 7.7.65.37: LE
// Periodic Advertising Response Report event (0x28). The event carries up to
// 0x19 records whose payload pointers reference the received event bytes, so
// the records are deep-copied (one heap slice per payload) before the event
// is posted to the GAP task.
void GapRecvLePeriodicAdvertisingResponseReportEvent(
    const HciLePeriodicAdvertisingResponseReportEventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }

    HILOGD("advHandle: 0x%{public}02x, subevent: 0x%{public}02x, numResponses: %{public}hhu",
        eventParam->advertisingHandle, eventParam->subevent, eventParam->numResponses);

    if (eventParam->numResponses > HCI_LE_PERIODIC_ADVERTISING_RESPONSE_REPORT_NUM_RESPONSES_MAX) {
        HILOGE("Too many responses: %{public}hhu. Dropping.", eventParam->numResponses);
        return;
    }
    for (uint8_t i = 0; i < eventParam->numResponses; i++) {
        if (eventParam->response[i].dataLength > 0 && eventParam->response[i].data == NULL) {
            HILOGW("Malformed response report: non-zero length but no payload. Dropping.");
            return;
        }
    }

    HciLePeriodicAdvertisingResponseReportEventParam hciParam = *eventParam;
    // Detach every record payload from the received event bytes up front: the
    // shallow copy above still aliases the event buffer, and the error
    // branches below release the slices of the records already deep-copied.
    // Freeing an alias of the event buffer as if it were a heap slice would
    // corrupt the heap.
    for (uint8_t i = 0; i < hciParam.numResponses; i++) {
        hciParam.response[i].data = NULL;
    }
    for (uint8_t i = 0; i < hciParam.numResponses; i++) {
        if (hciParam.response[i].dataLength > 0) {
            uint8_t *data = MEM_MALLOC.alloc(hciParam.response[i].dataLength);
            if (data == NULL) {
                HILOGE("Alloc response data error.");
                GapFreeLePeriodicAdvertisingResponseReportSlices(&hciParam);
                return;
            }
            if (memcpy_s(data, hciParam.response[i].dataLength, eventParam->response[i].data,
                hciParam.response[i].dataLength) != EOK) {
                MEM_MALLOC.free(data);
                HILOGE("Copy response data error.");
                GapFreeLePeriodicAdvertisingResponseReportSlices(&hciParam);
                return;
            }
            hciParam.response[i].data = data;
        }
    }

    int ret = GapProcessHciEventInTask((TaskFunc)GapOnLePeriodicAdvertisingResponseReportEvent,
        &hciParam,
        sizeof(hciParam),
        GapFreeLePeriodicAdvertisingResponseReportEvent);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E 7.7.65.24: subevent
// code 0x26 carries the [v1] parameters (0x18) first, followed by the same
// four subevent/response-slot parameters as the 0x24 event. The legacy sync
// state machine consumes the [v1] prefix via the shared task handler; the
// sync-side PAwR handling (gap_le_pawr_sync.c) receives the full [v2]
// parameters through a second task post. When Num_Subevents is zero the
// trailing three values are unspecified by the spec and are never
// interpreted at this layer.
void GapRecvLePeriodicAdvertisingSyncTransferReceivedV2Event(
    const HciLePeriodicAdvertisingSyncTransferReceivedV2EventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }

    HILOGI("status: 0x%{public}02x, syncHandle: 0x%{public}04x, numSubevents: %{public}hhu",
        eventParam->status, eventParam->syncHandle, eventParam->numSubevents);
    const HciLePeriodicAdvertisingSyncTransferReceivedEventParam *v1Param =
        (const HciLePeriodicAdvertisingSyncTransferReceivedEventParam *)eventParam;
    int ret = GapProcessHciEventInTask((TaskFunc)GapOnLePeriodicAdvertisingSyncTransferReceivedEvent,
        v1Param,
        sizeof(*v1Param),
        NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
    ret = GapProcessHciEventInTask((TaskFunc)GapOnLePawrSyncTransferReceivedEvent,
        eventParam,
        sizeof(*eventParam),
        NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

#endif /* GAP_LE_SUPPORT */
