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

/* Sync side (scanner/observer) of Bluetooth 5.4 periodic advertising with
 * responses (PAwR), Core Spec 5.4 Vol 4 Part E:
 *   - 7.8.126      LE Set Periodic Advertising Response Data
 *   - 7.8.127      LE Set Periodic Sync Subevent
 *   - 7.7.65.14    LE Periodic Advertising Sync Established event [v2] (0x24)
 *   - 7.7.65.15    LE Periodic Advertising Report event [v2] (0x25)
 *   - 7.7.65.24    LE Periodic Advertising Sync Transfer Received event [v2] (0x26)
 *
 * The scanner side of PAwR consumes the [v2] periodic advertising events: a
 * 5.4 Controller reports every sync establishment / periodic advertising
 * report / PAST transfer as 0x24 / 0x25 / 0x26 (event-mask bits 35-37), and
 * the four subevent/response-slot parameters of 0x24 and 0x26 plus the
 * Periodic_Event_Counter/Subevent of 0x25 are the PAwR material that the
 * pre-5.4 [v1] flows do not carry. The GAP layer keeps feeding the [v1]
 * prefixes of these events to the pre-5.4 callbacks (the established sync
 * state machine in gap_le_scan.c, the PAST flow in the CTE callback) and
 * hands the full [v2] parameters to the handlers below, which dispatch the
 * registered GapPawrSyncCallback (gap_le_if_5_4.h).
 *
 * Event consumption rules ("入库" of the feature doc, §2.4):
 *   - 0x24: the subevent tail describes the train. A train without subevents
 *     or response slots is reported with all four parameters 0x00; the
 *     consumer distinguishes the two train kinds on numSubevents.
 *   - 0x25: Periodic_Event_Counter and Subevent are passed through; Subevent
 *     carries 0xFF for trains without subevents.
 *   - 0x26: a sync transferred over a connection is established on this
 *     device as if 0x24 had reported it. Zero-value rule difference: a train
 *     without subevents is reported with numSubevents 0x00 only - the values
 *     of Subevent_Interval, Response_Slot_Delay and Response_Slot_Spacing
 *     are unspecified by the spec in that case and are passed through
 *     verbatim (never interpreted, never assumed 0x00).
 *
 * Sync creation still goes through the pre-5.4 GAPIF_LePeriodicAdvCreateSync;
 * on a 5.4 Controller its outcome arrives at syncEstablished through the
 * [v2] event (incl. failures, reported with clearly-invalid identity values
 * like the pre-5.4 handler does). Sync termination has no [v2] counterpart
 * and keeps flowing through the pre-5.4 callbacks (syncLost /
 * terminateSyncResult / createSyncCancelResult of GapPeriodicAdvSyncCallback).
 *
 * GAPIF_LePawrSetResponseData (0x83) schedules response data for one response
 * slot; the data is transmitted once and the result arrives at
 * setResponseDataResult. GAPIF_LePawrSetSyncSubevent (0x84) restricts the
 * synchronization to a subset of the subevents of the train.
 *
 * All state (the registered callback) is owned by the GAP task: the GAPIF_*
 * entries run on the caller's thread and delegate onto the GAP processing
 * queue (GapRunTaskBlockProcess), while the event/completion handlers arrive
 * on the queue through gap_hci_receive.c.
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

// Wire ranges of the sync-side PAwR commands: sync handles are 12-bit
// (0x0000-0x0EFF, 0x0F00-0x0FFF reserved), subevents of a train are
// numbered 0x00-0x7F and a subset numbers 0x01-0x80 entries.
#define GAP_PAWR_SYNC_HANDLE_MAX 0x0EFF
#define GAP_PAWR_SUBEVENT_INDEX_MAX 0x7F
#define GAP_PAWR_MAX_NUM_SUBEVENTS 0x80
// Transmittable length of one response data block of a 0x83 command: the HCI
// command parameter length is one octet (hci_cmd.c), which caps a parameter
// block at 255 octets and leaves 255 - 8 (fixed fields) = 247 octets for the
// payload. The spec wire range is 0-251 (7.8.126, 252-0xFF reserved), but
// 248-251 can never be framed, so they are rejected at the API entry instead
// of being sent to the HCI sender on every call.
#define GAP_PAWR_RESPONSE_DATA_SEND_MAX 0xF7

// Registered callback group; single slot, written on the GAP task through
// the registration entries (no locking needed, see the group comment above).
typedef struct {
    GapPawrSyncCallback callback;
    void *context;
} LePawrSyncCallback;

static LePawrSyncCallback g_lePawrSyncCallback;

int GapLePawrSyncInit(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (memset_s(&g_lePawrSyncCallback, sizeof(LePawrSyncCallback), 0x00, sizeof(LePawrSyncCallback)) != EOK) {
        LOG_ERROR("%{public}s: Clear callback error.", __FUNCTION__);
        return GAP_ERR_OUT_OF_RES;
    }
    return GAP_SUCCESS;
}

void GapLePawrSyncDeinit(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    (void)memset_s(&g_lePawrSyncCallback, sizeof(LePawrSyncCallback), 0x00, sizeof(LePawrSyncCallback));
}

typedef struct {
    int result;
    uint16_t syncHandle;
    uint16_t properties;
    uint8_t numSubevents;
    uint8_t subevents[GAP_PAWR_MAX_NUM_SUBEVENTS];
} LePawrSetSyncSubeventInfo;

// Core of GAPIF_LePawrSetSyncSubevent, running on the GAP task: forward the
// subset to the Controller; subevents that are synchronized but not listed
// stop being synchronized (7.8.127). No state is kept - the outcome arrives
// at the setSyncSubeventResult callback through the command completion.
static void GapLePawrSetSyncSubeventTask(void *ctx)
{
    LePawrSetSyncSubeventInfo *info = ctx;
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        info->result = GAP_ERR_NOT_ENABLE;
        return;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL) == false) {
        info->result = GAP_ERR_INVAL_STATE;
        return;
    }

    if (!BTM_IsControllerSupportPawrScanner()) {
        info->result = GAP_ERR_NOT_SUPPORT;
        return;
    }

    if (info->syncHandle > GAP_PAWR_SYNC_HANDLE_MAX ||
        (info->properties & ~GAP_PAWR_SYNC_SUBEVENT_PROPERTIES_TX_POWER) != 0x0000 ||
        info->numSubevents == 0x00 ||
        info->numSubevents > GAP_PAWR_MAX_NUM_SUBEVENTS) {
        info->result = GAP_ERR_INVAL_PARAM;
        return;
    }
    for (uint8_t i = 0; i < info->numSubevents; i++) {
        if (info->subevents[i] > GAP_PAWR_SUBEVENT_INDEX_MAX) {
            info->result = GAP_ERR_INVAL_PARAM;
            return;
        }
    }

    HciLeSetPeriodicSyncSubeventParam hciCmdParam = {
        .syncHandle = info->syncHandle,
        .periodicAdvertisingProperties = info->properties,
        .numSubevents = info->numSubevents,
        .subevents = info->subevents,
    };

    ret = HCI_LeSetPeriodicSyncSubevent(&hciCmdParam);
    if (ret != BT_SUCCESS) {
        HILOGE("%{public}s: Send set periodic sync subevent error: %{public}d.", __FUNCTION__, ret);
    }
    info->result = ret;
}

int GAPIF_LePawrSetSyncSubevent(uint16_t syncHandle, uint16_t properties,
    const uint8_t subevents[], uint8_t numSubevents)
{
    if (syncHandle > GAP_PAWR_SYNC_HANDLE_MAX ||
        (properties & ~GAP_PAWR_SYNC_SUBEVENT_PROPERTIES_TX_POWER) != 0x0000 ||
        subevents == NULL || numSubevents == 0x00 || numSubevents > GAP_PAWR_MAX_NUM_SUBEVENTS) {
        return BT_BAD_PARAM;
    }
    for (uint8_t i = 0; i < numSubevents; i++) {
        if (subevents[i] > GAP_PAWR_SUBEVENT_INDEX_MAX) {
            return BT_BAD_PARAM;
        }
    }

    LOG_INFO("%{public}s: syncHandle:0x%04x numSubevents:%hhu", __FUNCTION__, syncHandle, numSubevents);
    LePawrSetSyncSubeventInfo *ctx = MEM_MALLOC.alloc(sizeof(LePawrSetSyncSubeventInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(LePawrSetSyncSubeventInfo), 0x00, sizeof(LePawrSetSyncSubeventInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->syncHandle = syncHandle;
    ctx->properties = properties;
    ctx->numSubevents = numSubevents;
    if (memcpy_s(ctx->subevents, sizeof(ctx->subevents), subevents, numSubevents) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    int ret = GapRunTaskBlockProcess(GapLePawrSetSyncSubeventTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

typedef struct {
    int result;
    uint16_t syncHandle;
    uint16_t requestEvent;
    uint8_t requestSubevent;
    uint8_t responseSubevent;
    uint8_t responseSlot;
    uint8_t dataLength;
    // The response data bytes follow this struct; the data pointer of the
    // HCI command is adjusted into that area by the caller.
} LePawrSetResponseDataInfo;

// Core of GAPIF_LePawrSetResponseData, running on the GAP task: schedule the
// data for one transmission in the named response slot (7.8.126). No state is
// kept - the outcome arrives at the setResponseDataResult callback through
// the command completion (0x45/0x46 mean the data was discarded).
static void GapLePawrSetResponseDataTask(void *ctx)
{
    LePawrSetResponseDataInfo *info = ctx;
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        info->result = GAP_ERR_NOT_ENABLE;
        return;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL) == false) {
        info->result = GAP_ERR_INVAL_STATE;
        return;
    }

    if (!BTM_IsControllerSupportPawrScanner()) {
        info->result = GAP_ERR_NOT_SUPPORT;
        return;
    }

    if (info->syncHandle > GAP_PAWR_SYNC_HANDLE_MAX ||
        info->responseSubevent > GAP_PAWR_SUBEVENT_INDEX_MAX ||
        info->dataLength > GAP_PAWR_RESPONSE_DATA_SEND_MAX) {
        info->result = GAP_ERR_INVAL_PARAM;
        return;
    }

    HciLeSetPeriodicAdvertisingResponseDataParam hciCmdParam = {
        .syncHandle = info->syncHandle,
        .requestEvent = info->requestEvent,
        .requestSubevent = info->requestSubevent,
        .responseSubevent = info->responseSubevent,
        .responseSlot = info->responseSlot,
        .responseDataLength = info->dataLength,
        .responseData = info->dataLength != 0x00 ? (uint8_t *)info + sizeof(LePawrSetResponseDataInfo) : NULL,
    };

    ret = HCI_LeSetPeriodicAdvertisingResponseData(&hciCmdParam);
    if (ret != BT_SUCCESS) {
        HILOGE("%{public}s: Send set periodic advertising response data error: %{public}d.", __FUNCTION__, ret);
    }
    info->result = ret;
}

int GAPIF_LePawrSetResponseData(uint16_t syncHandle, const GapPawrResponseData *response)
{
    // dataLength is capped at 247 (GAP_PAWR_RESPONSE_DATA_SEND_MAX): the data
    // travels in a single 0x83 command whose one-octet HCI parameter length
    // field leaves 247 octets for the payload (8 fixed fields precede it).
    if (response == NULL) {
        return BT_BAD_PARAM;
    }
    if (syncHandle > GAP_PAWR_SYNC_HANDLE_MAX || response->responseSubevent > GAP_PAWR_SUBEVENT_INDEX_MAX ||
        response->dataLength > GAP_PAWR_RESPONSE_DATA_SEND_MAX ||
        (response->dataLength != 0x00 && response->data == NULL)) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: syncHandle:0x%04x respSubevent:%hhu respSlot:%hhu len:%hhu",
        __FUNCTION__, syncHandle, response->responseSubevent, response->responseSlot, response->dataLength);
    LePawrSetResponseDataInfo *ctx =
        MEM_MALLOC.alloc(sizeof(LePawrSetResponseDataInfo) + response->dataLength);
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(LePawrSetResponseDataInfo) + response->dataLength,
        0x00, sizeof(LePawrSetResponseDataInfo) + response->dataLength) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->syncHandle = syncHandle;
    ctx->requestEvent = response->requestEvent;
    ctx->requestSubevent = response->requestSubevent;
    ctx->responseSubevent = response->responseSubevent;
    ctx->responseSlot = response->responseSlot;
    ctx->dataLength = response->dataLength;
    if (response->dataLength != 0x00) {
        uint8_t *payloads = (uint8_t *)ctx + sizeof(LePawrSetResponseDataInfo);
        if (memcpy_s(payloads, response->dataLength, response->data, response->dataLength) != EOK) {
            MEM_MALLOC.free(ctx);
            ctx = NULL;
            return BT_OPERATION_FAILED;
        }
    }

    int ret = GapRunTaskBlockProcess(GapLePawrSetResponseDataTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

typedef struct {
    int result;
    GapPawrSyncCallback callback;
    void *context;
} LePawrRegisterSyncCallbackInfo;

static void GapLePawrRegisterSyncCallbackTask(void *ctx)
{
    LePawrRegisterSyncCallbackInfo *info = ctx;
    g_lePawrSyncCallback.callback = info->callback;
    g_lePawrSyncCallback.context = info->context;
    info->result = BT_SUCCESS;
}

int GAPIF_RegisterPawrSyncCallback(const GapPawrSyncCallback *callback, void *context)
{
    if (callback == NULL) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s:", __FUNCTION__);
    LePawrRegisterSyncCallbackInfo *ctx = MEM_MALLOC.alloc(sizeof(LePawrRegisterSyncCallbackInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(LePawrRegisterSyncCallbackInfo), 0x00, sizeof(LePawrRegisterSyncCallbackInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->callback = *callback;
    ctx->context = context;

    int ret = GapRunTaskBlockProcess(GapLePawrRegisterSyncCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLePawrDeregisterSyncCallbackTask(void *ctx)
{
    (void)ctx;
    (void)memset_s(&g_lePawrSyncCallback, sizeof(LePawrSyncCallback), 0x00, sizeof(LePawrSyncCallback));
}

int GAPIF_DeregisterPawrSyncCallback(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    int ret = GapRunTaskBlockProcess(GapLePawrDeregisterSyncCallbackTask, NULL);
    return ret;
}

// HCI_LE_Set_Periodic_Advertising_Response_Data completion (7.8.126): the
// data was either scheduled for its one transmission or discarded (0x45/0x46).
NO_SANITIZE("cfi")
void GapLeSetPeriodicAdvertisingResponseDataComplete(
    const HciLeSetPeriodicAdvertisingResponseDataReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    HILOGD("%{public}s: status:0x%{public}02x", __FUNCTION__, param->status);
    if (g_lePawrSyncCallback.callback.setResponseDataResult) {
        g_lePawrSyncCallback.callback.setResponseDataResult(
            param->status, param->syncHandle, g_lePawrSyncCallback.context);
    }
}

// HCI_LE_Set_Periodic_Sync_Subevent completion (7.8.127): the synchronization
// was restricted to the requested subset of subevents.
NO_SANITIZE("cfi")
void GapLeSetPeriodicSyncSubeventComplete(const HciLeSetPeriodicSyncSubeventReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    HILOGD("%{public}s: status:0x%{public}02x", __FUNCTION__, param->status);
    if (g_lePawrSyncCallback.callback.setSyncSubeventResult) {
        g_lePawrSyncCallback.callback.setSyncSubeventResult(
            param->status, param->syncHandle, g_lePawrSyncCallback.context);
    }
}

// Report a failed or rejected sync establishment (7.7.65,14): the remaining
// event parameters are invalid in that case, so the report carries clearly
// invalid identity values (the same convention as the pre-5.4 established
// handler).
static void GapPawrReportSyncEstablishedFailed(uint8_t status)
{
    GapPawrSyncEstablishedReport report = {
        .status = status,
        .syncHandle = 0xFFFF,
        .advSid = 0xFF,
        .periodicAdvInterval = 0xFFFF,
    };
    if (g_lePawrSyncCallback.callback.syncEstablished != NULL) {
        g_lePawrSyncCallback.callback.syncEstablished(&report, g_lePawrSyncCallback.context);
    }
}

// Report a sync establishment that succeeded (7.7.65,14): build the report
// from the event parameters and the (caller-owned) advertiser identity and
// dispatch it to the registered callback.
static void GapPawrReportSyncEstablished(
    const HciLePeriodicAdvertisingSyncEstablishedV2EventParam *eventParam, const BtAddr *addr)
{
    GapPawrSyncEstablishedReport report = {
        .status = eventParam->status,
        .syncHandle = eventParam->syncHandle,
        .advSid = eventParam->advertisingSid,
        .advAddr = addr,
        .advPhy = eventParam->advertiserPhy,
        .periodicAdvInterval = eventParam->periodicAdvertisingInterval,
        .numSubevents = eventParam->numSubevents,
        .subeventInterval = eventParam->subeventInterval,
        .responseSlotDelay = eventParam->responseSlotDelay,
        .responseSlotSpacing = eventParam->responseSlotSpacing,
    };
    if (g_lePawrSyncCallback.callback.syncEstablished != NULL) {
        g_lePawrSyncCallback.callback.syncEstablished(&report, g_lePawrSyncCallback.context);
    }
}

// LE Periodic Advertising Sync Established event [v2] (7.7.65,14): the [v1]
// parameters followed by the subevent/response-slot tail of the train. When
// the establishment fails the tail is not meaningful; like the pre-5.4
// handler the clearly-invalid identity values are reported. A train without
// subevents carries numSubevents 0x00 (the Controller sets the other three
// parameters to 0x00 as well, so the values below are 0x00 in that case).
NO_SANITIZE("cfi")
void GapOnLePawrSyncEstablishedEvent(const HciLePeriodicAdvertisingSyncEstablishedV2EventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }

    HILOGD("%{public}s: status:0x%{public}02x, syncHandle:0x%{public}04x, numSubevents:%{public}hhu",
        __FUNCTION__, eventParam->status, eventParam->syncHandle, eventParam->numSubevents);

    if (eventParam->status != HCI_SUCCESS) {
        GapPawrReportSyncEstablishedFailed(eventParam->status);
        return;
    }

    if (eventParam->advertiserAddressType != BT_PUBLIC_DEVICE_ADDRESS &&
        eventParam->advertiserAddressType != BT_RANDOM_DEVICE_ADDRESS) {
        HILOGE("%{public}s: invalid advertiserAddressType %hhu", __FUNCTION__, eventParam->advertiserAddressType);
        GapPawrReportSyncEstablishedFailed(HCI_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE);
        return;
    }

    BtAddr addr = {
        .type = eventParam->advertiserAddressType,
    };
    (void)memcpy_s(addr.addr, BT_ADDRESS_SIZE, eventParam->advertiserAddress.raw, BT_ADDRESS_SIZE);

    GapPawrReportSyncEstablished(eventParam, &addr);
}

// LE Periodic Advertising Report event [v2] (7.7.65,15): the [v1] parameters
// with Periodic_Event_Counter and Subevent inserted between CTE_Type and
// Data_Status. The report data is a heap copy made by gap_hci_receive.c and
// stays valid until this handler returns, so the public callback can
// reference it directly.
NO_SANITIZE("cfi")
void GapOnLePawrSyncReportEvent(const HciLePeriodicAdvertisingReportV2EventParam *eventParam)
{
    if (eventParam == NULL || (eventParam->dataLength > 0 && eventParam->data == NULL) ||
        eventParam->dataLength > GAP_PERIODIC_ADV_DATA_LENGTH_MAX) {
        return;
    }

    HILOGD("%{public}s: syncHandle:0x%{public}04x, subevent:0x%{public}02x, dataLen:%{public}hhu",
        __FUNCTION__, eventParam->syncHandle, eventParam->subevent, eventParam->dataLength);

    GapPawrSyncReport report = {
        .syncHandle = eventParam->syncHandle,
        .txPower = eventParam->txPower,
        .rssi = eventParam->rssi,
        .cteType = eventParam->cteType,
        .periodicEventCounter = eventParam->periodicEventCounter,
        .subevent = eventParam->subevent,
        .dataStatus = eventParam->dataStatus,
        .dataLength = eventParam->dataLength,
        .data = eventParam->data,
    };
    if (g_lePawrSyncCallback.callback.syncReport != NULL) {
        g_lePawrSyncCallback.callback.syncReport(&report, g_lePawrSyncCallback.context);
    }
}

// Invoke the syncTransferReceived callback member with the report values
// (7.7.65,24), which the caller filled from the event parameter.
static void GapPawrReportSyncTransferReceived(const GapPawrSyncTransferReceivedReport *report)
{
    if (g_lePawrSyncCallback.callback.syncTransferReceived == NULL) {
        return;
    }
    g_lePawrSyncCallback.callback.syncTransferReceived(report, g_lePawrSyncCallback.context);
}

// Build the report of a received sync transfer (7.7.65,24) from the event
// parameters and the (caller-owned) advertiser identity.
static GapPawrSyncTransferReceivedReport GapPawrMakeSyncTransferReceivedReport(
    const HciLePeriodicAdvertisingSyncTransferReceivedV2EventParam *eventParam, const BtAddr *addr)
{
    GapPawrSyncTransferReceivedReport report = {
        .status = eventParam->status,
        .connectionHandle = eventParam->connectionHandle,
        .serviceData = eventParam->serviceData,
        .syncHandle = eventParam->syncHandle,
        .advSid = eventParam->advertisingSid,
        .advAddr = addr,
        .advPhy = eventParam->advertiserPhy,
        .periodicAdvInterval = eventParam->periodicAdvertisingInterval,
        .clockAccuracy = eventParam->advertiserClockAccuracy,
        .numSubevents = eventParam->numSubevents,
        .subeventInterval = eventParam->subeventInterval,
        .responseSlotDelay = eventParam->responseSlotDelay,
        .responseSlotSpacing = eventParam->responseSlotSpacing,
    };
    return report;
}

// Report a received transfer whose advertiser identity is invalid: the
// identity and the train parameters are neutralized (the fields without a
// designated initializer are zero-initialized) and the status reports the
// rejection, with the same convention as the pre-5.4 established handler;
// the source connection handle and the service data stay reported as on
// every path (7.7.65,24).
static void GapPawrReportSyncTransferReceivedInvalid(
    const HciLePeriodicAdvertisingSyncTransferReceivedV2EventParam *eventParam)
{
    GapPawrSyncTransferReceivedReport report = {
        .status = HCI_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE,
        .connectionHandle = eventParam->connectionHandle,
        .serviceData = eventParam->serviceData,
        .syncHandle = 0xFFFF,
        .advSid = 0xFF,
        .advAddr = NULL,
        .periodicAdvInterval = 0xFFFF,
    };
    GapPawrReportSyncTransferReceived(&report);
}

// LE Periodic Advertising Sync Transfer Received event [v2] (7.7.65,24): a
// synchronization transferred over a connection is established on this
// device; the [v1] parameters are followed by the same subevent/response-slot
// tail as the 0x24 event. Zero-value rule difference to 0x24: when the train
// has no subevents only Num_Subevents is specified (0x00) - the values of the
// other three tail parameters are undefined by the spec and passed through
// verbatim, never interpreted here. The source connection handle and the
// service data of the transfer are reported on every path.
NO_SANITIZE("cfi")
void GapOnLePawrSyncTransferReceivedEvent(
    const HciLePeriodicAdvertisingSyncTransferReceivedV2EventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }

    HILOGD("%{public}s: status:0x%{public}02x, syncHandle:0x%{public}04x, numSubevents:%{public}hhu",
        __FUNCTION__, eventParam->status, eventParam->syncHandle, eventParam->numSubevents);

    // The advertiser identity and train parameters are reported on every
    // path (7.7.65,24: when Status is non-zero all parameter values remain
    // valid except Sync_Handle, which the Host shall ignore).
    BtAddr addr = {
        .type = eventParam->advertiserAddressType,
    };
    (void)memcpy_s(addr.addr, BT_ADDRESS_SIZE, eventParam->advertiserAddress.raw, BT_ADDRESS_SIZE);

    GapPawrSyncTransferReceivedReport report = GapPawrMakeSyncTransferReceivedReport(eventParam, &addr);

    if (eventParam->status != HCI_SUCCESS) {
        // The transfer failed, but the parameters still describe the periodic
        // advertising the transfer was about; only the sync handle is not
        // meaningful and is neutralized (0xFFFF).
        report.syncHandle = 0xFFFF;
        GapPawrReportSyncTransferReceived(&report);
        return;
    }

    if (eventParam->advertiserAddressType != BT_PUBLIC_DEVICE_ADDRESS &&
        eventParam->advertiserAddressType != BT_RANDOM_DEVICE_ADDRESS) {
        HILOGE("%{public}s: invalid advertiserAddressType %hhu", __FUNCTION__, eventParam->advertiserAddressType);
        GapPawrReportSyncTransferReceivedInvalid(eventParam);
        return;
    }

    GapPawrReportSyncTransferReceived(&report);
}

#endif /* GAP_LE_SUPPORT */
