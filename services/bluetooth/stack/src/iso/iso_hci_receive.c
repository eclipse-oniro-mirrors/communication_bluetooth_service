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

#include "iso.h"

#include <securec.h>

#include "allocator.h"
#include "log.h"
#include "thread.h"

#include "btm/btm_thread.h"
#include "hci/hci.h"
#include "hci/hci_error.h"

#include "iso_task_internal.h"

// The HCI event handlers take typed const payload pointers, while the task dispatcher runs
// void (*)(void *). Casting a handler to TaskFunc directly would be an incompatible indirect
// call (which is why the dispatch used to carry a NO_SANITIZE("cfi") escape); each event is
// instead dispatched through a typed adapter that restores the payload type before calling.
#define ISO_DEFINE_HCI_EVENT_ADAPTER(handler, paramType) \
    static void IsoHciEventAdapter_##handler(void *ctx)  \
    {                                                    \
        handler((const paramType *)ctx);                 \
    }

ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeSetCigParametersComplete, HciLeSetCigParametersReturnParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeCreateCisComplete, HciLeCreateCisReturnParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeRemoveCigComplete, HciLeRemoveCigReturnParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeRejectCisRequestComplete, HciLeRejectCisRequestReturnParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeCisRequestEvent, HciLeCisRequestEventParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeCisEstablishedEvent, HciLeCisEstablishedEventParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeDisconnectComplete, HciDisconnectCompleteEventParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeCreateBigComplete, HciLeCreateBigCompleteEventParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeTerminateBigComplete, HciLeTerminateBigCompleteEventParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeBigSyncEstablishedEvent, HciLeBigSyncEstablishedEventParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeBigSyncLostEvent, HciLeBigSyncLostEventParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeBigInfoAdvertisingReportEvent, HciLeBigInfoAdvertisingReportEventParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeBigTerminateSyncComplete, HciLeBigTerminateSyncReturnParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeSetupIsoDataPathComplete, HciLeSetupIsoDataPathReturnParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeRemoveIsoDataPathComplete, HciLeRemoveIsoDataPathReturnParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeIsoTransmitTestComplete, HciLeIsoTransmitTestReturnParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeIsoReceiveTestComplete, HciLeIsoReceiveTestReturnParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeIsoReadTestCountersComplete, HciLeIsoReadTestCountersReturnParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeIsoTestEndComplete, HciLeIsoTestEndReturnParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeReadIsoLinkQualityComplete, HciLeReadIsoLinkQualityReturnParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeReadIsoTxSyncComplete, HciLeReadIsoTxSyncReturnParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeRequestPeerScaComplete, HciLeRequestPeerScaCompleteEventParam)
ISO_DEFINE_HCI_EVENT_ADAPTER(IsoLeReadBufferSizeV2Complete, HciLeReadBufferSizeV2ReturnParam)

static int IsoProcessHciEventInTask(TaskFunc run, const void *ctx, uint32_t ctxLen, TaskFunc freeCtx)
{
    void *hciParam = NULL;
    if (ctx != NULL && ctxLen != 0) {
        hciParam = MEM_MALLOC.alloc(ctxLen);
        if (hciParam == NULL) {
            return BT_NO_MEMORY;
        }
        (void)memcpy_s(hciParam, ctxLen, ctx, ctxLen);
    }

    // Non-blocking enqueue on purpose: the HCI event dispatch runs on the Stack thread,
    // the very thread that drains the ISO processing queue, so a blocking enqueue on a
    // full queue would self-deadlock (the drain can never progress while this thread is
    // blocked). Under an event storm the task is dropped instead - losing events is
    // preferable to hanging the whole Stack thread. The loss degrades gracefully at the
    // consumers: a dropped LE CIS Established leaves the Accept CIS watchdog armed, which
    // synthesizes a failure callback after its 10 s timeout (the upper layer retries); a
    // dropped Remove CIG Complete leaves removePending occupied, so later removes return
    // BT_ALREADY until the 10 s remove watchdog resets the slot. On failure hciParam is
    // released here (the event task never took ownership, see IsoRunTaskUnBlockProcessNoBlock).
    int ret = IsoRunTaskUnBlockProcessNoBlock(run, hciParam, freeCtx);
    if (ret != BT_SUCCESS) {
        if (hciParam != NULL) {
            MEM_MALLOC.free(hciParam);
        }
    }
    return ret;
}

void IsoRecvLeSetCigParametersComplete(const HciLeSetCigParametersReturnParam *param)
{
    HILOGI("status: 0x%{public}02x, cigId:%hhu, cisCount:%hhu", param->status, param->cigId, param->cisCount);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeSetCigParametersComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeCreateCisComplete(const HciLeCreateCisReturnParam *param)
{
    HILOGI("status: 0x%{public}02x", param->status);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeCreateCisComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeRemoveCigComplete(const HciLeRemoveCigReturnParam *param)
{
    HILOGI("status: 0x%{public}02x", param->status);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeRemoveCigComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeRejectCisRequestComplete(const HciLeRejectCisRequestReturnParam *param)
{
    HILOGI("status: 0x%{public}02x", param->status);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeRejectCisRequestComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeCisRequest(const HciLeCisRequestEventParam *param)
{
    HILOGI("cisHandle: 0x%{public}04x, aclHandle: 0x%{public}04x", param->cisHandle, param->aclHandle);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeCisRequestEvent, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeCisEstablished(const HciLeCisEstablishedEventParam *param)
{
    HILOGI("status: 0x%{public}02x, cisHandle: 0x%{public}04x", param->status, param->connectionHandle);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeCisEstablishedEvent, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeDisconnectComplete(const HciDisconnectCompleteEventParam *param)
{
    HILOGI("status: 0x%{public}02x, connectionHandle: 0x%{public}04x, reason: 0x%{public}02x", param->status,
        param->connectionHandle, param->reason);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeDisconnectComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeCreateBigComplete(const HciLeCreateBigCompleteEventParam *param)
{
    HILOGI("status: 0x%{public}02x, bigHandle: 0x%{public}02x", param->status, param->bigHandle);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeCreateBigComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeTerminateBigComplete(const HciLeTerminateBigCompleteEventParam *param)
{
    HILOGI("status: 0x%{public}02x, bigHandle: 0x%{public}02x", param->status, param->bigHandle);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeTerminateBigComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeBigSyncEstablished(const HciLeBigSyncEstablishedEventParam *param)
{
    HILOGI("status: 0x%{public}02x, bigHandle: 0x%{public}02x", param->status, param->bigHandle);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeBigSyncEstablishedEvent, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeBigSyncLost(const HciLeBigSyncLostEventParam *param)
{
    HILOGI("bigHandle: 0x%{public}02x, reason: 0x%{public}02x", param->bigHandle, param->reason);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeBigSyncLostEvent, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeBigInfoAdvertisingReport(const HciLeBigInfoAdvertisingReportEventParam *param)
{
    HILOGI("syncHandle: 0x%{public}04x, numBis: %hhu", param->syncHandle, param->numBis);
    int ret = IsoProcessHciEventInTask(
        IsoHciEventAdapter_IsoLeBigInfoAdvertisingReportEvent, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeBigTerminateSyncComplete(const HciLeBigTerminateSyncReturnParam *param)
{
    HILOGI("status: 0x%{public}02x", param->status);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeBigTerminateSyncComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeSetupIsoDataPathComplete(const HciLeSetupIsoDataPathReturnParam *param)
{
    HILOGI("status: 0x%{public}02x, connectionHandle: 0x%{public}04x", param->status, param->connectionHandle);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeSetupIsoDataPathComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeRemoveIsoDataPathComplete(const HciLeRemoveIsoDataPathReturnParam *param)
{
    HILOGI("status: 0x%{public}02x, connectionHandle: 0x%{public}04x", param->status, param->connectionHandle);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeRemoveIsoDataPathComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeIsoTransmitTestComplete(const HciLeIsoTransmitTestReturnParam *param)
{
    HILOGI("status: 0x%{public}02x, connectionHandle: 0x%{public}04x", param->status, param->connectionHandle);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeIsoTransmitTestComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeIsoReceiveTestComplete(const HciLeIsoReceiveTestReturnParam *param)
{
    HILOGI("status: 0x%{public}02x, connectionHandle: 0x%{public}04x", param->status, param->connectionHandle);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeIsoReceiveTestComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeIsoReadTestCountersComplete(const HciLeIsoReadTestCountersReturnParam *param)
{
    HILOGI("status: 0x%{public}02x, connectionHandle: 0x%{public}04x", param->status, param->connectionHandle);
    int ret = IsoProcessHciEventInTask(
        IsoHciEventAdapter_IsoLeIsoReadTestCountersComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeIsoTestEndComplete(const HciLeIsoTestEndReturnParam *param)
{
    HILOGI("status: 0x%{public}02x, connectionHandle: 0x%{public}04x", param->status, param->connectionHandle);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeIsoTestEndComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeReadIsoLinkQualityComplete(const HciLeReadIsoLinkQualityReturnParam *param)
{
    HILOGI("status: 0x%{public}02x, connectionHandle: 0x%{public}04x", param->status, param->connectionHandle);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeReadIsoLinkQualityComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeReadIsoTxSyncComplete(const HciLeReadIsoTxSyncReturnParam *param)
{
    HILOGI("status: 0x%{public}02x, connectionHandle: 0x%{public}04x", param->status, param->connectionHandle);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeReadIsoTxSyncComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeRequestPeerScaComplete(const HciLeRequestPeerScaCompleteEventParam *param)
{
    HILOGI("status: 0x%{public}02x, connectionHandle: 0x%{public}04x", param->status, param->connectionHandle);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeRequestPeerScaComplete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

void IsoRecvLeReadBufferSizeV2Complete(const HciLeReadBufferSizeV2ReturnParam *param)
{
    HILOGI(
        "status: 0x%{public}02x, isoDataPacketLength: 0x%{public}04x", param->status, param->hcLeIsoDataPacketLength);
    int ret = IsoProcessHciEventInTask(IsoHciEventAdapter_IsoLeReadBufferSizeV2Complete, param, sizeof(*param), NULL);
    if (ret != BT_SUCCESS) {
        HILOGE("Task error: %{public}d.", ret);
    }
}

static HciEventCallbacks g_hciEventCallbacks = {
    .leReadBufferSizeV2Complete = IsoRecvLeReadBufferSizeV2Complete,
    .leSetCigParametersComplete = IsoRecvLeSetCigParametersComplete,
    .leCreateCisComplete = IsoRecvLeCreateCisComplete,
    .leRemoveCigComplete = IsoRecvLeRemoveCigComplete,
    .leRejectCisRequestComplete = IsoRecvLeRejectCisRequestComplete,
    .leCisRequest = IsoRecvLeCisRequest,
    .leCisEstablished = IsoRecvLeCisEstablished,
    .disconnectComplete = IsoRecvLeDisconnectComplete,
    .leCreateBigComplete = IsoRecvLeCreateBigComplete,
    .leTerminateBigComplete = IsoRecvLeTerminateBigComplete,
    .leBigSyncEstablished = IsoRecvLeBigSyncEstablished,
    .leBigSyncLost = IsoRecvLeBigSyncLost,
    .leBigInfoAdvertisingReport = IsoRecvLeBigInfoAdvertisingReport,
    .leBigTerminateSyncComplete = IsoRecvLeBigTerminateSyncComplete,
    .leSetupIsoDataPathComplete = IsoRecvLeSetupIsoDataPathComplete,
    .leRemoveIsoDataPathComplete = IsoRecvLeRemoveIsoDataPathComplete,
    .leIsoTransmitTestComplete = IsoRecvLeIsoTransmitTestComplete,
    .leIsoReceiveTestComplete = IsoRecvLeIsoReceiveTestComplete,
    .leIsoReadTestCountersComplete = IsoRecvLeIsoReadTestCountersComplete,
    .leIsoTestEndComplete = IsoRecvLeIsoTestEndComplete,
    .leReadIsoLinkQualityComplete = IsoRecvLeReadIsoLinkQualityComplete,
    .leReadIsoTxSyncComplete = IsoRecvLeReadIsoTxSyncComplete,
    .leRequestPeerScaComplete = IsoRecvLeRequestPeerScaComplete,
};

void IsoRegisterHciEventCallbacks(void)
{
    HILOGI("enter");
    HCI_RegisterEventCallbacks(&g_hciEventCallbacks);
}

void IsoDeregisterHciEventCallbacks(void)
{
    HILOGI("enter");
    HCI_DeregisterEventCallbacks(&g_hciEventCallbacks);
}
