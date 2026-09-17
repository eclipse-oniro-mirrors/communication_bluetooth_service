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

#include "hci/hci.h"
#include "hci/hci_error.h"
#include "hci/iso/hci_iso.h"

static bool IsoFindBigByBigHandle(void *data, void *parameter)
{
    IsoBigInfo *bigInfo = data;
    uint8_t bigHandle = *(uint8_t *)parameter;
    return bigInfo->bigHandle == bigHandle;
}

static IsoBigInfo *IsoFindBigInList(List *bigList, uint8_t bigHandle)
{
    return ListForEachData(bigList, IsoFindBigByBigHandle, &bigHandle);
}

static IsoBigInfo *IsoFindBig(IsoLeMng *mng, uint8_t bigHandle)
{
    return IsoFindBigInList(mng->bigBlock.bigList, bigHandle);
}

static IsoBigInfo *IsoFindBigSync(IsoLeMng *mng, uint8_t bigHandle)
{
    return IsoFindBigInList(mng->bigBlock.syncList, bigHandle);
}

// Sync termination (BIG Sync Lost / Terminate Sync success): the controller destroyed
// the BIS handles, so release their RX reassembly contexts and HCI ISO flow-control
// tracking with them (symmetric with the broadcaster path in IsoLeTerminateBigComplete).
static void IsoRemoveBigSyncEntry(IsoLeMng *mng, uint8_t bigHandle)
{
    IsoBigInfo *bigInfo = IsoFindBigSync(mng, bigHandle);
    if (bigInfo == NULL) {
        return;
    }
    for (uint8_t i = 0; i < bigInfo->bisCount; i++) {
        IsoDataRemoveRxContext(bigInfo->bisHandles[i]);
        HciIsoDeregisterHandle(bigInfo->bisHandles[i]);
    }
    ListRemoveNode(mng->bigBlock.syncList, bigInfo);
}

int IsoRegisterBigCallback(const IsoLeBigCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "NULL");
    IsoLeMng *mng = IsoGetMng();
    mng->bigCallback = callback;
    mng->bigCallbackContext = context;
    return BT_SUCCESS;
}

int IsoDeregisterBigCallback(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    IsoLeMng *mng = IsoGetMng();
    mng->bigCallback = NULL;
    mng->bigCallbackContext = NULL;
    return BT_SUCCESS;
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.103 LE Create BIG: BIG_Handle 0x00-0xEF, RTN 0x00-0x1F, PHY 0x00-0x02,
// Packing 0x00-0x01; Framing 0x00-0x02 (0x02 Unsegmented gated by bit 45, Vol 6
// Part B 4.6.44); Encryption 0x00-0x01; Advertising_Handle is validated by the
// controller.
int IsoLeCreateBig(uint8_t bigHandle, uint8_t advertisingHandle, uint8_t numBis, const IsoLeBigParam *bigParam,
    const uint8_t *broadcastCode)
{
    LOG_INFO("%{public}s: bigHandle:0x%02x, advertisingHandle:0x%02x, numBis:%hhu", __FUNCTION__, bigHandle,
        advertisingHandle, numBis);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }
    if (bigParam == NULL || numBis == 0 || numBis > ISO_LE_BIS_COUNT_MAX) {
        return BT_BAD_PARAM;
    }
    if (bigParam->sduInterval > 0xFFFFFF) {
        return BT_BAD_PARAM;
    }
    if (bigHandle > 0xEF || bigParam->rtn > 0x1F || bigParam->phy > 0x02 || bigParam->packing > 0x01 ||
        !IsoIsFramingRequestAllowed(bigParam->framing) || bigParam->encryption > 0x01) {
        return BT_BAD_PARAM;
    }
    /* Vol 3 Part C 3.2.6,1: Broadcast_Code is mandatory once BIG encryption is set */
    if (bigParam->encryption && (broadcastCode == NULL)) {
        return BT_BAD_PARAM;
    }

    HciLeCreateBigParam hciParam = { 0 };
    hciParam.bigHandle = bigHandle;
    hciParam.advertisingHandle = advertisingHandle;
    hciParam.numBis = numBis;
    IsoWriteUint24(hciParam.sduInterval, bigParam->sduInterval);
    hciParam.maxSdu = bigParam->maxSdu;
    hciParam.maxTransportLatency = bigParam->maxTransportLatency;
    hciParam.rtn = bigParam->rtn;
    hciParam.phy = bigParam->phy;
    hciParam.packing = bigParam->packing;
    hciParam.framing = bigParam->framing;
    hciParam.encryption = bigParam->encryption;
    if (broadcastCode != NULL) {
        (void)memcpy_s(
            hciParam.broadcastCode, sizeof(hciParam.broadcastCode), broadcastCode, sizeof(hciParam.broadcastCode));
    }
    return HCI_LeCreateBig(&hciParam);
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.104 LE Create BIG Test: BIG_Handle 0x00-0xEF, PHY 0x00-0x02, Packing
// 0x00-0x01; Framing 0x00-0x02 (0x02 Unsegmented gated by bit 45, Vol 6 Part B
// 4.6.44); Encryption 0x00-0x01, BN 0x01-0x07, IRC 1 to (NSE/BN), PTO 0x00-0x0F;
// Advertising_Handle is validated by the controller.
int IsoLeCreateBigTest(uint8_t bigHandle, uint8_t advertisingHandle, uint8_t numBis, const IsoLeBigTestParam *bigParam,
    const uint8_t *broadcastCode)
{
    LOG_INFO("%{public}s: bigHandle:0x%02x, advertisingHandle:0x%02x, numBis:%hhu", __FUNCTION__, bigHandle,
        advertisingHandle, numBis);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }
    if (bigParam == NULL || numBis == 0 || numBis > ISO_LE_BIS_COUNT_MAX) {
        return BT_BAD_PARAM;
    }
    if (bigParam->sduInterval > 0xFFFFFF) {
        return BT_BAD_PARAM;
    }
    if (bigHandle > 0xEF || bigParam->phy > 0x02 || bigParam->packing > 0x01 ||
        !IsoIsFramingRequestAllowed(bigParam->framing) || bigParam->encryption > 0x01) {
        return BT_BAD_PARAM;
    }
    /* Vol 4 Part E 7.8.104: BN 0x01-0x07, IRC 1 to (NSE/BN), PTO 0x00-0x0F */
    if (bigParam->bn < 0x01 || bigParam->bn > 0x07 || bigParam->irc < 0x01 || bigParam->irc > 0x0F ||
        bigParam->pto > 0x0F || bigParam->irc * bigParam->bn > bigParam->nse) {
        return BT_BAD_PARAM;
    }
    /* Vol 3 Part C 3.2.6,1: Broadcast_Code is mandatory once BIG encryption is set */
    if (bigParam->encryption && (broadcastCode == NULL)) {
        return BT_BAD_PARAM;
    }

    HciLeCreateBigTestParam hciParam = { 0 };
    hciParam.bigHandle = bigHandle;
    hciParam.advertisingHandle = advertisingHandle;
    hciParam.numBis = numBis;
    IsoWriteUint24(hciParam.sduInterval, bigParam->sduInterval);
    hciParam.isoInterval = bigParam->isoInterval;
    hciParam.nse = bigParam->nse;
    hciParam.maxSdu = bigParam->maxSdu;
    hciParam.maxPdu = bigParam->maxPdu;
    hciParam.phy = bigParam->phy;
    hciParam.packing = bigParam->packing;
    hciParam.framing = bigParam->framing;
    hciParam.bn = bigParam->bn;
    hciParam.irc = bigParam->irc;
    hciParam.pto = bigParam->pto;
    hciParam.encryption = bigParam->encryption;
    if (broadcastCode != NULL) {
        (void)memcpy_s(
            hciParam.broadcastCode, sizeof(hciParam.broadcastCode), broadcastCode, sizeof(hciParam.broadcastCode));
    }
    return HCI_LeCreateBigTest(&hciParam);
}

int IsoLeTerminateBig(uint8_t bigHandle, uint8_t reason)
{
    LOG_INFO("%{public}s: bigHandle:0x%02x, reason:0x%02x", __FUNCTION__, bigHandle, reason);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }

    HciLeTerminateBigParam param = {
        .bigHandle = bigHandle,
        .reason = reason,
    };
    return HCI_LeTerminateBig(&param);
}

int IsoLeBigCreateSync(const IsoLeBigCreateSyncParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: bigHandle:0x%02x, syncHandle:0x%04x, numBis:%hhu", __FUNCTION__, param->bigHandle,
        param->syncHandle, param->numBis);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }
    // Vol 4 Part E 7.8.106: BIG_Handle 0x00-0xEF, Encryption 0x00-0x01, MSE 0x00-0x1F,
    // BIG_Sync_Timeout 0x000A-0x4000. Num_BIS 0x00 is valid and requests synchronization
    // to all BISes of the BIG: the bis array is then ignored (may be NULL) and the
    // synchronized count is reported by the LE BIG Sync Established event.
    if (param->bigHandle > 0xEF || param->encryption > 0x01 || param->mse > 0x1F ||
        param->bigSyncTimeout < 0x000A || param->bigSyncTimeout > 0x4000) {
        return BT_BAD_PARAM;
    }
    if (param->numBis > ISO_LE_BIS_COUNT_MAX || (param->numBis > 0 && param->bis == NULL)) {
        return BT_BAD_PARAM;
    }

    HciLeBigCreateSyncParam hciParam = { 0 };
    hciParam.bigHandle = param->bigHandle;
    hciParam.syncHandle = param->syncHandle;
    hciParam.encryption = param->encryption;
    (void)memcpy_s(
        hciParam.broadcastCode, sizeof(hciParam.broadcastCode), param->broadcastCode, sizeof(hciParam.broadcastCode));
    hciParam.mse = param->mse;
    IsoWriteUint16(hciParam.bigSyncTimeout, param->bigSyncTimeout);
    hciParam.numBis = param->numBis;
    hciParam.bis = param->bis;
    return HCI_LeBigCreateSync(&hciParam);
}

int IsoLeBigTerminateSync(uint8_t bigHandle)
{
    LOG_INFO("%{public}s: bigHandle:0x%02x", __FUNCTION__, bigHandle);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }

    HciLeBigTerminateSyncParam param = {
        .bigHandle = bigHandle,
    };
    return HCI_LeBigTerminateSync(&param);
}

// Outcome of inserting a broadcast BIG into the Host tracking list. Create Complete
// uses this to decide whether to drop a duplicate, abort an untracked Controller BIG,
// or register the BIS handles for the ISO data path.
typedef enum {
    ISO_BIG_TRACK_OK = 0,
    ISO_BIG_TRACK_DUP,
    ISO_BIG_TRACK_FAIL,
} IsoBigTrackResult;

// Track a successfully created BIG. A duplicate success of an already-tracked handle
// (a retransmitted event, or a late one after the Create BIG command timed out at the
// HCI layer, i5) is dropped: the tracking entry already exists (only a Terminate BIG
// Complete removes it), so a second createBigResult success must not reach the upper
// layer. ISO_BIG_TRACK_FAIL: alloc / list insert failed - the caller tears down the
// Controller BIG (no RegisterHandle) and still delivers createBigResult with a Host
// failure status (same policy as IsoCisEstablishedTrack).
static IsoBigTrackResult IsoCreateBigTrack(IsoLeMng *mng, const HciLeCreateBigCompleteEventParam *param,
    uint8_t numBis)
{
    IsoBigInfo *bigInfo = IsoFindBig(mng, param->bigHandle);
    if (bigInfo != NULL) {
        HILOGW("%{public}s: duplicate Create BIG Complete, drop, bigHandle:0x%02x", __FUNCTION__,
            param->bigHandle);
        return ISO_BIG_TRACK_DUP;
    }
    bigInfo = MEM_MALLOC.alloc(sizeof(IsoBigInfo));
    if (bigInfo == NULL) {
        HILOGE("%{public}s: alloc IsoBigInfo failed, bigHandle:0x%02x", __FUNCTION__, param->bigHandle);
        return ISO_BIG_TRACK_FAIL;
    }
    (void)memset_s(bigInfo, sizeof(IsoBigInfo), 0x00, sizeof(IsoBigInfo));
    bigInfo->bigHandle = param->bigHandle;
    if (!ListAddLast(mng->bigBlock.bigList, bigInfo)) {
        HILOGE("%{public}s: tracking insert failed, bigHandle:0x%02x", __FUNCTION__, param->bigHandle);
        MEM_MALLOC.free(bigInfo);
        return ISO_BIG_TRACK_FAIL;
    }
    bigInfo->bisCount = numBis;
    (void)memcpy_s(bigInfo->bisHandles, sizeof(bigInfo->bisHandles), param->bisHandles, numBis * sizeof(uint16_t));
    return ISO_BIG_TRACK_OK;
}

// Shared defense-in-depth clamp for the BIG events that feed numBis into a memcpy
// length: a malformed event must not overflow bisHandles (the HCI parser already
// clamps numBis; this local clamp is the last gate before the copy).
static uint8_t IsoClampBisCount(const char *eventName, uint8_t numBis)
{
    if (numBis > ISO_LE_BIS_COUNT_MAX) {
        HILOGE("%{public}s: invalid numBis:%hhu from %{public}s, clamp to %hhu", __FUNCTION__, numBis, eventName,
            (uint8_t)ISO_LE_BIS_COUNT_MAX);
        return ISO_LE_BIS_COUNT_MAX;
    }
    return numBis;
}

void IsoLeCreateBigComplete(const HciLeCreateBigCompleteEventParam *param)
{
    LOG_INFO("%{public}s: status:0x%02x, bigHandle:0x%02x, numBis:%hhu", __FUNCTION__, param->status, param->bigHandle,
        param->numBis);

    uint8_t numBis = IsoClampBisCount("create BIG complete", param->numBis);
    IsoLeMng *mng = IsoGetMng();
    uint8_t notifyStatus = param->status;

    if (param->status == HCI_SUCCESS) {
        IsoBigTrackResult trackResult = IsoCreateBigTrack(mng, param, numBis);
        if (trackResult == ISO_BIG_TRACK_DUP) {
            return;
        }
        if (trackResult == ISO_BIG_TRACK_FAIL) {
            // Host cannot own later Terminate / credit recovery without a bigList entry:
            // tear down the Controller BIG (Vol 4 Part E 7.8.105). Reason is any Controller
            // Error Code here; 0x13 matches the CIS abort path and existing Create/Terminate
            // tests. Still notify the upper with a Host-side failure so Create waiters are
            // not left believing the BIG is usable.
            HciLeTerminateBigParam termParam = {
                .bigHandle = param->bigHandle,
                .reason = HCI_REMOTE_USER_TERMINATED_CONNECTION,
            };
            (void)HCI_LeTerminateBig(&termParam);
            notifyStatus = HCI_MEMORY_CAPACITY_EXCEEDED;
        } else {
            // The BIS handles are established ISO data handles; register them for the HCI ISO
            // data path (Number of Completed Packets filtering / credit recovery).
            for (uint8_t i = 0; i < numBis; i++) {
                HciIsoRegisterHandle(param->bisHandles[i]);
            }
        }
    }

    IsoLeBigCreatedInfo info = {
        .bigHandle = param->bigHandle,
        .bigSyncDelay = IsoReadUint24(param->bigSyncDelay),
        .transportLatencyBig = IsoReadUint24(param->transportLatencyBig),
        .phy = param->phy,
        .nse = param->nse,
        .bn = param->bn,
        .pto = param->pto,
        .irc = param->irc,
        .maxPdu = param->maxPdu,
        .isoInterval = param->isoInterval,
        .numBis = numBis,
    };
    (void)memcpy_s(info.bisHandles, sizeof(info.bisHandles), param->bisHandles, numBis * sizeof(uint16_t));

    if (mng->bigCallback != NULL && mng->bigCallback->createBigResult != NULL) {
        mng->bigCallback->createBigResult(notifyStatus, &info, mng->bigCallbackContext);
    }
}

void IsoLeTerminateBigComplete(const HciLeTerminateBigCompleteEventParam *param)
{
    LOG_INFO("%{public}s: status:0x%02x, bigHandle:0x%02x", __FUNCTION__, param->status, param->bigHandle);
    IsoLeMng *mng = IsoGetMng();
    if (param->status == HCI_SUCCESS) {
        IsoBigInfo *bigInfo = IsoFindBig(mng, param->bigHandle);
        if (bigInfo == NULL) {
            // Untracked BIG (e.g. Create tracking failed and Host already aborted): no
            // RegisterHandle was taken, and createBigResult already carried the Host
            // failure - mirror IsoLeDisconnectComplete's untracked early return.
            return;
        }
        // Broadcast BIG termination: BIS handles are destroyed by the controller,
        // release their RX reassembly contexts (symmetric with IsoRemoveBigSyncEntry).
        for (uint8_t i = 0; i < bigInfo->bisCount; i++) {
            IsoDataRemoveRxContext(bigInfo->bisHandles[i]);
            HciIsoDeregisterHandle(bigInfo->bisHandles[i]);
        }
        ListRemoveNode(mng->bigBlock.bigList, bigInfo);
    }

    if (mng->bigCallback != NULL && mng->bigCallback->terminateBigResult != NULL) {
        mng->bigCallback->terminateBigResult(param->status, mng->bigCallbackContext);
    }
}

// Track a synchronized BIG's BIS handles under the sync handle. Reuses an
// existing sync entry - dropping the previous session's BIS handle registrations
// and RX contexts first, since a repeated Established event before the Terminate
// Sync completed would otherwise leak them and misroute ISO flow control on a
// reused handle (symmetric with IsoRemoveBigSyncEntry). Registers each BIS
// handle for HCI ISO flow control and stores the current set in the entry.
// Returns false when a new entry could not be allocated/inserted: the caller then
// aborts the Controller sync and delivers a Host failure (no RegisterHandle).
static bool IsoTrackBigSyncEstablished(IsoLeMng *mng, const HciLeBigSyncEstablishedEventParam *param, uint8_t numBis)
{
    IsoBigInfo *bigInfo = IsoFindBigSync(mng, param->bigHandle);
    if (bigInfo == NULL) {
        bigInfo = MEM_MALLOC.alloc(sizeof(IsoBigInfo));
        if (bigInfo == NULL) {
            HILOGE("%{public}s: sync tracking alloc failed, bigHandle:0x%02x", __FUNCTION__, param->bigHandle);
            return false;
        }
        (void)memset_s(bigInfo, sizeof(IsoBigInfo), 0x00, sizeof(IsoBigInfo));
        bigInfo->bigHandle = param->bigHandle;
        if (!ListAddLast(mng->bigBlock.syncList, bigInfo)) {
            HILOGE("%{public}s: sync tracking insert failed, bigHandle:0x%02x", __FUNCTION__, param->bigHandle);
            MEM_MALLOC.free(bigInfo);
            return false;
        }
    } else {
        for (uint8_t i = 0; i < bigInfo->bisCount; i++) {
            IsoDataRemoveRxContext(bigInfo->bisHandles[i]);
            HciIsoDeregisterHandle(bigInfo->bisHandles[i]);
        }
    }
    bigInfo->bisCount = numBis;
    (void)memcpy_s(bigInfo->bisHandles, sizeof(bigInfo->bisHandles), param->bisHandles, numBis * sizeof(uint16_t));
    for (uint8_t i = 0; i < numBis; i++) {
        HciIsoRegisterHandle(param->bisHandles[i]);
    }
    return true;
}

// Report a BIG Sync Established event to the registered upper-layer callback.
static void IsoNotifyBigSyncEstablished(uint8_t status, const HciLeBigSyncEstablishedEventParam *param, uint8_t numBis)
{
    IsoLeMng *mng = IsoGetMng();
    if (mng->bigCallback == NULL || mng->bigCallback->bigSyncEstablished == NULL) {
        return;
    }
    IsoLeBigSyncEstablishedInfo info = {
        .bigHandle = param->bigHandle,
        .transportLatencyBig = IsoReadUint24(param->transportLatencyBig),
        .nse = param->nse,
        .bn = param->bn,
        .pto = param->pto,
        .irc = param->irc,
        .maxPdu = param->maxPdu,
        .isoInterval = param->isoInterval,
        .numBis = numBis,
    };
    (void)memcpy_s(info.bisHandles, sizeof(info.bisHandles), param->bisHandles, numBis * sizeof(uint16_t));
    mng->bigCallback->bigSyncEstablished(status, &info, mng->bigCallbackContext);
}

void IsoLeBigSyncEstablishedEvent(const HciLeBigSyncEstablishedEventParam *param)
{
    LOG_INFO("%{public}s: status:0x%02x, bigHandle:0x%02x, numBis:%hhu", __FUNCTION__, param->status, param->bigHandle,
        param->numBis);

    // numBis feeds a memcpy length below (see IsoClampBisCount).
    uint8_t numBis = IsoClampBisCount("BIG sync established", param->numBis);
    uint8_t notifyStatus = param->status;
    if (param->status == HCI_SUCCESS) {
        IsoLeMng *mng = IsoGetMng();
        if (!IsoTrackBigSyncEstablished(mng, param, numBis)) {
            HciLeBigTerminateSyncParam termParam = {
                .bigHandle = param->bigHandle,
            };
            (void)HCI_LeBigTerminateSync(&termParam);
            notifyStatus = HCI_MEMORY_CAPACITY_EXCEEDED;
        }
    }
    IsoNotifyBigSyncEstablished(notifyStatus, param, numBis);
}

void IsoLeBigSyncLostEvent(const HciLeBigSyncLostEventParam *param)
{
    LOG_INFO("%{public}s: bigHandle:0x%02x, reason:0x%02x", __FUNCTION__, param->bigHandle, param->reason);
    IsoLeMng *mng = IsoGetMng();
    // Untracked sync (e.g. Established tracking failed and Host already aborted): the
    // upper already received bigSyncEstablished with a Host failure - skip the lost
    // notify, mirroring IsoLeTerminateBigComplete's untracked early return.
    if (IsoFindBigSync(mng, param->bigHandle) == NULL) {
        return;
    }
    IsoRemoveBigSyncEntry(mng, param->bigHandle);
    if (mng->bigCallback != NULL && mng->bigCallback->bigSyncLost != NULL) {
        mng->bigCallback->bigSyncLost(param->bigHandle, param->reason, mng->bigCallbackContext);
    }
}

void IsoLeBigInfoAdvertisingReportEvent(const HciLeBigInfoAdvertisingReportEventParam *param)
{
    LOG_INFO("%{public}s: syncHandle:0x%04x, numBis:%hhu", __FUNCTION__, param->syncHandle, param->numBis);
    IsoLeMng *mng = IsoGetMng();

    IsoLeBigInfoReportInfo info = {
        .syncHandle = param->syncHandle,
        .numBis = param->numBis,
        .nse = param->nse,
        .isoInterval = param->isoInterval,
        .bn = param->bn,
        .pto = param->pto,
        .irc = param->irc,
        .maxPdu = param->maxPdu,
        .sduInterval = IsoReadUint24(param->sduInterval),
        .maxSdu = param->maxSdu,
        .phy = param->phy,
        // Framing also carries 0x02 (Framed Unsegmented) since BT6.0 and is passed
        // through verbatim to the upper layer, Vol 4 Part E 7.7.65,34 p2415.
        .framing = param->framing,
        .encryption = param->encryption,
    };

    if (mng->bigCallback != NULL && mng->bigCallback->bigInfoReport != NULL) {
        mng->bigCallback->bigInfoReport(&info, mng->bigCallbackContext);
    }
}

void IsoLeBigTerminateSyncComplete(const HciLeBigTerminateSyncReturnParam *param)
{
    LOG_INFO("%{public}s: status:0x%02x", __FUNCTION__, param->status);
    IsoLeMng *mng = IsoGetMng();
    if (param->status == HCI_SUCCESS) {
        // Host abort after sync tracking failure: no RegisterHandle taken, failure
        // already delivered on bigSyncEstablished - skip terminate-sync result.
        if (IsoFindBigSync(mng, param->bigHandle) == NULL) {
            return;
        }
        IsoRemoveBigSyncEntry(mng, param->bigHandle);
    }
    if (mng->bigCallback != NULL && mng->bigCallback->bigTerminateSyncResult != NULL) {
        mng->bigCallback->bigTerminateSyncResult(param->status, mng->bigCallbackContext);
    }
}
