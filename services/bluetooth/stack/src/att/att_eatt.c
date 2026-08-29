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

#include "att_eatt.h"

#include "att_connect.h"
#include "l2cap/l2cap_le.h"
#include "log.h"
#include "platform/include/allocator.h"

// Vol 3 Part G 5.3.1: the EATT bearer ATT_MTU shall be at least 64 octets, the same
// minimum as the ECRED channel MTU/MPS (chapter 4.25, L2CAP_LE_EATT_MIN_MTU/MPS).
#define ATT_EATT_MIN_CONFIG (64)

// Local EATT responder config declared in the 0x18 response, charter 4.26. The MTU/MPS ceiling
// matches the plan's EATT bearer ATT_MTU operating range (64-247); both must be >= 64, Vol 3
// Part G 5.3.1. The credit is the initial credit granted to the peer on our channels.
static const L2capLeConfigInfo g_eattLocalCfg = {
    .mtu = 247,
    .mps = 247,
    .credit = 8,
};

static void AttEattRegisterServiceCallback(uint16_t lpsm, int result)
{
    int cfgResult;

    if (result != BT_SUCCESS) {
        LOG_WARN("%{public}s: register EATT PSM 0x%04X failed, result = %{public}d", __FUNCTION__, lpsm, result);
        return;
    }

    // Runs on the L2CAP processing queue after the PSM slot is registered, so the PSM is findable
    // (same single Stack thread as the ATT processing queue, no data race).
    cfgResult = L2CAP_LeSetServiceConfig(lpsm, &g_eattLocalCfg);
    if (cfgResult != BT_SUCCESS) {
        LOG_WARN("%{public}s: L2CAP_LeSetServiceConfig failed, result = %{public}d", __FUNCTION__, cfgResult);
    }
    return;
}

void AttEattRegisterService(void)
{
    L2capLeService eattService;

    LOG_INFO("%{public}s enter", __FUNCTION__);

    (void)memset_s(&eattService, sizeof(eattService), 0, sizeof(eattService));
    eattService.recvLeEattConnected = AttEattConnected;
    eattService.recvLeEattConnectionRsp = AttEattConnectionRsp;
    eattService.recvLeEattReconfigured = AttEattReconfigured;
    eattService.recvLeDisconnectionReq = AttEattRecvLeDisconnectionReq;
    eattService.recvLeDisconnectionRsp = AttEattRecvLeDisconnectionRsp;
    eattService.leDisconnectAbnormal = AttEattDisconnected;
    eattService.recvLeData = AttEattRecvLeData;
    L2CIF_LeRegisterService(L2CAP_LE_EATT_PSM, &eattService, NULL, AttEattRegisterServiceCallback);

    return;
}

// Settle a pending establishment: notify the AttEattEstablish caller exactly once and drop the
// retained state. The callback is invoked outside the cleared state so a re-entrant establish is
// not blocked by its own resolution. Also cancels the collision-retry fallback alarm and clears
// the orphaned-channel record of the settled batch.
void AttEattResolveEstablish(AttConnectInfo *parent, int result)
{
    void (*cb)(int result, const uint16_t *lcids, uint16_t n, void *ctx) = parent->eattEstablishCb;
    void *ctx = parent->eattEstablishCtx;
    uint16_t lcids[L2CAP_LE_EATT_MAX_CHANNEL] = {0};
    uint16_t n = parent->eattLcidCount;

    if (cb == NULL) {
        return;
    }
    (void)memcpy_s(lcids, sizeof(lcids), parent->eattLcids, sizeof(parent->eattLcids));
    parent->eattEstablishCb = NULL;
    parent->eattEstablishCtx = NULL;
    parent->eattLcidCount = 0;
    parent->eattSlotlessCount = 0;
    if (parent->eattEstablishAlarm != NULL) {
        AlarmCancel(parent->eattEstablishAlarm);
    }
    cb(result, lcids, n, ctx);
    return;
}

// Context of the 0x17 dispatch callback: carries the ACL handle captured at dispatch so the
// callback can re-resolve the parent connection instead of trusting a raw slot pointer that may
// have been cleared or reused while the request was in flight (the L2CAP dispatch task runs on
// the same Stack thread, but the parent slot can be released by a teardown task that runs first).
typedef struct {
    uint16_t aclHandle;
    AttConnectInfo *parent;
} AttEattEstablishDispatchContext;

// L2CAP dispatch result of the 0x17: on a send failure the establishment is over before any 0x18
// can arrive, so the caller is notified immediately; on success the allocated source CIDs are
// retained and the caller is notified by AttEattConnectionRsp once the 0x18 settles the batch.
// The callback owns (and frees) its dispatch context; L2CAP invokes it exactly once when the
// request was dispatched.
static void AttEattEstablishSendResult(int result, const uint16_t *lcids, uint16_t n, void *ctx)
{
    AttEattEstablishDispatchContext *dispatchCtx = (AttEattEstablishDispatchContext *)ctx;
    AttConnectInfo *parent = NULL;
    uint16_t count = (n < L2CAP_LE_EATT_MAX_CHANNEL) ? n : L2CAP_LE_EATT_MAX_CHANNEL;
    uint16_t i;

    if (dispatchCtx == NULL) {
        return;
    }

    // The parent slot may have been cleared or reused while the 0x17 was in flight: re-resolve it
    // by the ACL handle captured at dispatch and drop the stale result instead of writing into a
    // newer connection's slots. A cleared parent settles nothing here: AttEattResolveEstablish
    // was already invoked by the clearing path (AttClearConnectInfo) or the establishment is
    // simply gone (its callback may be lost only together with the connection, which is fine).
    // Known residual: an identity check cannot recognize a *same-address* slot reuse (a new
    // connection on the same ACL allocated into the same slot of g_connectInfo), which would
    // misdeliver the stale result to the new connection. Reaching it requires the teardown and
    // re-establishment of the same ACL to both complete while this 0x17 dispatch callback is
    // still queued on the Stack thread - practically unreachable, and the misdelivery is then a
    // single spurious establishment callback, not a use-after-free (the dispatch context itself
    // is not reused).
    parent = AttGetConnectInfoByAclHandleAndLeCid(dispatchCtx->aclHandle, LE_CID);
    if ((parent == NULL) || (parent != dispatchCtx->parent)) {
        LOG_WARN("%{public}s: parent connection gone, drop stale 0x17 dispatch result", __FUNCTION__);
        MEM_MALLOC.free(dispatchCtx);
        return;
    }

    if (result != BT_SUCCESS) {
        AttEattResolveEstablish(parent, result);
        MEM_MALLOC.free(dispatchCtx);
        return;
    }
    for (i = 0; i < count; i++) {
        parent->eattLcids[i] = lcids[i];
    }
    parent->eattLcidCount = count;
    MEM_MALLOC.free(dispatchCtx);
    return;
}

int AttEattEstablish(uint16_t connectHandle, const L2capLeConfigInfo *localCfg, uint16_t n,
    void (*cb)(int result, const uint16_t *lcids, uint16_t n, void *ctx), void *ctx)
{
    AttConnectInfo *connect = NULL;
    AttEattEstablishDispatchContext *dispatchCtx = NULL;
    int result;

    // n in [1, L2CAP_LE_EATT_MAX_CHANNEL] and MTU/MPS at least 64 (Vol 3 Part G 5.3.1): a channel
    // count beyond the retained batch arrays would misjudge the extra channels as responder-side.
    if ((localCfg == NULL) || (n == 0) || (n > L2CAP_LE_EATT_MAX_CHANNEL) ||
        (localCfg->mtu < ATT_EATT_MIN_CONFIG) || (localCfg->mps < ATT_EATT_MIN_CONFIG)) {
        return BT_BAD_PARAM;
    }

    // The EATT channels ride on the ACL that hosts the UATT bearer, so the parent connection must
    // exist first; LE_CID selects the UATT bearer of this connection exactly.
    connect = AttGetConnectInfoByConnectHandleAndLeCid(connectHandle, LE_CID);
    if (connect == NULL) {
        return BT_BAD_PARAM;
    }
    // This check is lock-free: its correctness relies on the caller and the L2CAP dispatch/0x18
    // callbacks all running on the Stack thread (the only writers of eattEstablishCb), so no
    // other thread can concurrently start or settle an establishment. There is no in-repo
    // caller today; the contract documented for a future caller is: invoke from the Stack
    // thread or via a processing-queue task, never from a foreign thread. A caller can verify
    // the contract with ThreadIsSelf(BTM_GetProcessingThread()) == 0 (see iso_task_common.c for
    // the established pattern); the state machine is not thread-safe, so an assertion is the
    // correct reaction to a violation.
    if (connect->eattEstablishCb != NULL) {
        LOG_WARN("%{public}s: an EATT establishment is already in flight on this connection", __FUNCTION__);
        return BT_OPERATION_FAILED;
    }

    // The dispatch callback resolves the parent by the ACL handle captured here instead of a
    // raw slot pointer, because the parent slot may be cleared or reused by a teardown task
    // while the 0x17 is in flight (see AttEattEstablishSendResult). The context is allocated
    // before the retained state so a failure cannot leave eattEstablishCb set without the
    // caller's callback ever firing.
    dispatchCtx = MEM_MALLOC.alloc(sizeof(AttEattEstablishDispatchContext));
    if (dispatchCtx == NULL) {
        return BT_NO_MEMORY;
    }
    dispatchCtx->aclHandle = connect->aclHandle;
    dispatchCtx->parent = connect;

    // Retain the caller's completion callback and local config: L2CIF_LeEattConnectionReq only
    // reports that the 0x17 was dispatched (AttEattEstablishSendResult), the 0x18 response
    // settles the transaction later (AttEattConnectionRsp); the local config is also the base of
    // the per-bearer eattMtu negotiation in AttEattConnected.
    connect->eattEstablishCb = cb;
    connect->eattEstablishCtx = ctx;
    connect->eattLocalCfg = *localCfg;
    connect->eattLcidCount = 0;
    connect->eattSlotlessCount = 0;

    result = L2CIF_LeEattConnectionReq(&connect->addr, localCfg, n, AttEattEstablishSendResult, dispatchCtx);
    if (result != BT_SUCCESS) {
        // The 0x17 was never dispatched: L2CAP will not invoke the dispatch callback, so its
        // context is released here (on success the callback owns and frees it), and the
        // caller's callback must be invoked immediately (exactly once) - dropping the state
        // silently would leak the pending establishment forever.
        MEM_MALLOC.free(dispatchCtx);
        AttEattResolveEstablish(connect, result);
    }
    return result;
}

void AttEattConnected(uint16_t lcid, const L2capConnectionInfo *info, const L2capLeConfigInfo *cfg, void *ctx)
{
    AttConnectInfo *parent = NULL;
    AttConnectInfo *connect = NULL;
    uint16_t index = 0;
    uint16_t i = 0;
    bool ownBatch = false;
    uint16_t localMtu;
    uint16_t eattMtu;

    LOG_INFO("%{public}s enter, lcid = %hu", __FUNCTION__, lcid);

    if ((info == NULL) || (cfg == NULL)) {
        LOG_WARN("%{public}s: info or cfg is NULL", __FUNCTION__);
        return;
    }

    // The parent is the UATT bearer (lecid == LE_CID); aclHandle first-match may hit an existing
    // EATT slot when several EATT channels ride the same ACL (Vol 3 Part G 5.3).
    parent = AttGetConnectInfoByAclHandleAndLeCid(info->handle, LE_CID);
    if (parent == NULL) {
        LOG_WARN("%{public}s: parent connection not found, aclHandle = %hu", __FUNCTION__, info->handle);
        return;
    }

    // A channel belongs to our own batch only when its lcid is in eattLcids (filled by
    // AttEattEstablishSendResult at dispatch): the initiator's config came from AttEattEstablish
    // (retained until the 0x18 settles), while in a collision both sides send 0x17 and the
    // peer's batch channels are responder-side, using the registered g_eattLocalCfg. Computed
    // before the slot allocation: it decides how a slot-less channel is handled.
    for (i = 0; i < parent->eattLcidCount; i++) {
        if (parent->eattLcids[i] == lcid) {
            ownBatch = true;
            break;
        }
    }

    // Allocate an empty slot the same way the UATT bearer is added (retGattConnectHandle == 0).
    AttGetConnectInfoIndexByConnectHandle(0, &index, &connect);
    if (connect == NULL) {
        // No free bearer slot: an initiator-batch channel would otherwise report success while
        // its data is silently dropped (AttEattRecvLeDataAsync cannot resolve the bearer).
        // Record the orphaned channel so AttEattConnectionRsp fails the batch and disconnects
        // it; a responder-side channel (peer batch) is merely logged.
        if (ownBatch && (parent->eattSlotlessCount < L2CAP_LE_EATT_MAX_CHANNEL)) {
            parent->eattSlotlessLcids[parent->eattSlotlessCount] = lcid;
            parent->eattSlotlessCount++;
        }
        LOG_WARN("%{public}s: no free bearer slot, lcid = %hu", __FUNCTION__, lcid);
        return;
    }

    // eattMtu = min(our declared MTU, peer declared MTU), negotiated per bearer, must be >= 64,
    // Vol 3 Part G 5.3.1. Both declared MTUs are pre-validated by the L2CAP ECRED layer.
    localMtu = ownBatch ? parent->eattLocalCfg.mtu : g_eattLocalCfg.mtu;
    eattMtu = (localMtu < cfg->mtu) ? localMtu : cfg->mtu;

    connect->aclHandle = info->handle;
    connect->AttConnectID.lecid = lcid;
    connect->retGattConnectHandle = parent->retGattConnectHandle;
    connect->transportType = BT_TRANSPORT_LE;
    connect->mtuFlag = false;
    connect->mtu = eattMtu;
    connect->sendMtu = eattMtu;
    connect->receiveMtu = eattMtu;
    (void)memcpy_s(&connect->addr, sizeof(connect->addr), &info->addr, sizeof(info->addr));

    LOG_INFO("%{public}s return: bearer lcid = %hu, eattMtu = %hu", __FUNCTION__, lcid, eattMtu);
    return;
}

// Bound of a deferred establishment (collision retry): must exceed the longest slave retry delay
// (100 ms or 2 * (connSlaveLatency + 1) * connInterval, Vol 3 Part G 5.4) with margin, and stay
// below the L2CAP RTX so the fallback settles a wrongly deferred final refusal well before any
// related teardown could.
#define EATT_ESTABLISH_TIMEOUT 20000

typedef struct AttEattEstablishTimeOutContext {
    uint16_t connectHandle;
    uint16_t batchLcid;
} AttEattEstablishTimeOutContext;

static void AttEattEstablishTimeOutAsync(const void *context)
{
    AttEattEstablishTimeOutContext *timeOutPtr = (AttEattEstablishTimeOutContext *)context;
    AttConnectInfo *parent = NULL;

    LOG_INFO("%{public}s enter", __FUNCTION__);

    parent = AttGetConnectInfoByConnectHandleAndLeCid(timeOutPtr->connectHandle, LE_CID);
    if (parent == NULL) {
        goto ATT_EATT_ESTABLISH_TIMEOUT_END;
    }

    // Resolve only when the deferred batch is still pending: the slot may have been cleared or
    // reused since the alarm was armed, and a newer establishment carries a different batch lcid.
    if ((parent->eattEstablishCb == NULL) || (parent->eattLcidCount == 0) ||
        (parent->eattLcids[0] != timeOutPtr->batchLcid)) {
        goto ATT_EATT_ESTABLISH_TIMEOUT_END;
    }

    // No retry report (or any other batch signal) settled the establishment in time: it must
    // have been a final refusal, so the caller's callback fires exactly once with a failure.
    // The batch channels are torn down proactively (review m9): a deferred 0x18 (or a retry
    // batch still queued in L2CAP) can complete their establishment after this settle and
    // leave them half-open with no owner (AttEattConnected drops them by lcid, but the
    // channel would linger on the link). The lcids are snapshotted before the resolve, which
    // clears the retained batch state, and the disconnects run after it so a re-entrant
    // establishment cannot reuse the CIDs while the old channels still exist.
    // L2CIF_LeDisconnectionReq is a safe no-op for a channel that is missing or not in the
    // CONNECTED state. Residual narrow race (documented): a 0x18 processed by L2CAP after
    // this teardown can still bring a channel up; AttEattConnected drops it by lcid (no
    // bearer slot), and the connection teardown cleans the channel - not reachable through
    // any in-repo path, since the fallback only fires when no retry is actually pending.
    uint16_t teardownLcids[L2CAP_LE_EATT_MAX_CHANNEL] = {0};
    uint16_t teardownCount = parent->eattLcidCount;
    uint16_t i;
    if (teardownCount > L2CAP_LE_EATT_MAX_CHANNEL) {
        teardownCount = L2CAP_LE_EATT_MAX_CHANNEL;
    }
    (void)memcpy_s(teardownLcids, sizeof(teardownLcids), parent->eattLcids, sizeof(teardownLcids));
    AttEattResolveEstablish(parent, BT_OPERATION_FAILED);
    for (i = 0; i < teardownCount; i++) {
        L2CIF_LeDisconnectionReq(teardownLcids[i], NULL);
    }

ATT_EATT_ESTABLISH_TIMEOUT_END:
    MEM_MALLOC.free(timeOutPtr);
    return;
}

static void AttEattEstablishTimeOutAsyncDestroy(const void *context)
{
    AttEattEstablishTimeOutContext *timeOutPtr = (AttEattEstablishTimeOutContext *)context;

    MEM_MALLOC.free(timeOutPtr);
    return;
}

/* Runs on the alarm thread: reads only the immutable snapshot of the arm that fired (the
 * alternated eattEstablishAlarmCtx slots; the connection state itself may have been cleared or
 * reused meanwhile) and re-validates it on the ATT processing queue. */
static void AttEattEstablishTimeOut(const void *parameter)
{
    AttEattEstablishAlarmContext *alarmCtx = (AttEattEstablishAlarmContext *)parameter;
    AttEattEstablishTimeOutContext *timeOutPtr = NULL;

    LOG_INFO("%{public}s enter", __FUNCTION__);

    timeOutPtr = MEM_MALLOC.alloc(sizeof(AttEattEstablishTimeOutContext));
    if (timeOutPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    timeOutPtr->connectHandle = __atomic_load_n(&alarmCtx->connectHandle, __ATOMIC_RELAXED);
    timeOutPtr->batchLcid = __atomic_load_n(&alarmCtx->batchLcid, __ATOMIC_RELAXED);

    AttAsyncProcess(AttEattEstablishTimeOutAsync, AttEattEstablishTimeOutAsyncDestroy, timeOutPtr);
    return;
}

// Arm the fallback bound of a deferred establishment: captures the batch identity in the next
// alternated immutable snapshot (the alarm parameter; see ATT_ALARM_SNAPSHOT_SLOTS) and starts
// the timer on the ATT processing queue caller.
static void AttEattStartEstablishTimeOut(AttConnectInfo *parent)
{
    AttEattEstablishAlarmContext *snap = NULL;

    parent->eattEstablishAlarmSlot ^= 1;
    snap = &parent->eattEstablishAlarmCtx[parent->eattEstablishAlarmSlot];
    // Relaxed atomics keep the alarm thread's reads defined; the slot alternation guarantees it
    // reads exactly the arm that fired.
    __atomic_store_n(&snap->connectHandle, parent->retGattConnectHandle, __ATOMIC_RELAXED);
    __atomic_store_n(&snap->batchLcid, parent->eattLcids[0], __ATOMIC_RELAXED);
    AlarmSet(parent->eattEstablishAlarm, (uint64_t)EATT_ESTABLISH_TIMEOUT, (void (*)(void *))AttEattEstablishTimeOut,
        snap);
    return;
}

void AttEattConnectionRsp(
    const L2capConnectionInfo *info, uint16_t result, uint8_t attempted, uint8_t succeeded, void *ctx)
{
    AttConnectInfo *parent = NULL;
    int establishResult;

    LOG_INFO("%{public}s enter, result = 0x%04X, attempted = %hhu, succeeded = %hhu", __FUNCTION__, result, attempted,
        succeeded);

    if (info == NULL) {
        LOG_WARN("%{public}s: info is NULL", __FUNCTION__);
        return;
    }

    // The parent is the UATT bearer (lecid == LE_CID); see AttEattConnected for the aclHandle
    // first-match reasoning when several EATT channels ride the same ACL.
    parent = AttGetConnectInfoByAclHandleAndLeCid(info->handle, LE_CID);
    if (parent == NULL) {
        LOG_WARN("%{public}s: parent connection not found, aclHandle = %hu", __FUNCTION__, info->handle);
        return;
    }

    // The transaction-level outcome reaches the AttEattEstablish caller here, once per batch
    // (Vol 3 Part G 4.26): a non-zero 0x18 Result or a short grant (succeeded < attempted) is a
    // failure; the L2CAP layer keeps only the established channels of a partial batch. attempted
    // is the channel count of the 0x18 as sent, not necessarily this caller's own batch size:
    // when several L2CAP-level batches were coalesced into a single 0x17 (ACL-down deferred
    // send, or a 5.4 collision retry), attempted counts all merged channels, so a partial merged
    // grant fails this establishment even when its own channels were all granted (conservative).
    establishResult = ((result == L2CAP_LE_CONNECTION_SUCCESSFUL) && (succeeded == attempted)) ? BT_SUCCESS
                                                                                               : BT_OPERATION_FAILED;

    // Channels of the batch that were established in L2CAP but could not take a bearer slot
    // (AttEattConnected logged and dropped them): reporting success for them would silently
    // discard their data, so fail the batch and tear the orphaned L2CAP channels down.
    if ((parent->eattEstablishCb != NULL) && (parent->eattSlotlessCount > 0)) {
        uint8_t slotlessIndex = 0;

        for (; slotlessIndex < parent->eattSlotlessCount; slotlessIndex++) {
            L2CIF_LeDisconnectionReq(parent->eattSlotlessLcids[slotlessIndex], NULL);
        }
        parent->eattSlotlessCount = 0;
        establishResult = BT_OPERATION_FAILED;
    }

    // 5.4 collision retry: a 0x18 answered with L2CAP_LE_NO_RESOURCES_AVAILABLE (succeeded == 0)
    // means the peer collided with our in-flight 0x17 and the L2CAP ECRED layer retries the batch
    // on the same lcids. Deferring the resolution keeps the retained batch state (lcids/count)
    // valid, so the retried channels still resolve as our own batch (correct ATT_MTU), and keeps
    // the single-shot callback pending for the retry's 0x18 report - or for any teardown of the
    // batch channels (AttEattDisconnected, AttClearConnectInfo). The fallback alarm bounds the
    // corner where the report actually was a final refusal that no retry follows.
    if ((parent->eattEstablishCb != NULL) && (parent->eattLcidCount > 0) &&
        (result == L2CAP_LE_NO_RESOURCES_AVAILABLE) && (succeeded == 0)) {
        AttEattStartEstablishTimeOut(parent);
        return;
    }

    AttEattResolveEstablish(parent, establishResult);
    return;
}

void AttEattReconfigured(uint16_t lcid, uint16_t newMtu, uint16_t result, void *ctx)
{
    AttConnectInfo *connect = NULL;

    LOG_INFO("%{public}s enter, lcid = %hu, newMtu = %hu, result = %hu", __FUNCTION__, lcid, newMtu, result);

    connect = AttGetConnectInfoByLeCid(lcid);
    if (connect == NULL) {
        LOG_WARN("%{public}s: bearer not found, lcid = %hu", __FUNCTION__, lcid);
        return;
    }

    if (result != BT_SUCCESS) {
        LOG_WARN("%{public}s: reconfigure failed, result = %hu, old MTU stays in effect", __FUNCTION__, result);
        return;
    }

    connect->mtu = newMtu;
    connect->sendMtu = newMtu;
    connect->receiveMtu = newMtu;
    return;
}

/* Release an EATT bearer: abort outstanding transactions as 0x020E in both directions (client req
 * and unconfirmed indication can coexist, Vol 3 Part F 3.3.2), then drain the queue and free slot. */
static void AttEattReleaseBearer(AttConnectInfo *connect)
{
    int listSize = 0;
    int drainSize = 0;

    if (connect->serverSendFlag) {
        // Unconfirmed indication: report the transaction closure to the server side once.
        AttServerCallbackDispatch(connect, ATT_TRANSACTION_TIME_OUT_ID, NULL, NULL);
        connect->serverSendFlag = false;
    }

    // In-flight client requests: report one transaction closure per outstanding request. The
    // count is captured once before the dispatch loop (review m7): a client callback can
    // re-enter this release (e.g. fail the bearer from the timeout report), and that nested
    // release reports and drains its own snapshot - re-reading the size after the dispatch
    // would steal packets the nested release already removed or freed, or drain packets a
    // re-entered path enqueued without reporting them.
    listSize = ListGetSize(connect->instruct);
    drainSize = listSize;
    for (; listSize > 0; --listSize) {
        AttClientCallbackDispatch(connect, ATT_TRANSACTION_TIME_OUT_ID, NULL, NULL);
    }

    // Drain exactly the pre-callback in-flight number (drainSize was captured together with
    // listSize above); packets a re-entered release reported and drained are gone already
    // (ListRemoveFirst no-ops on the empty tail), and anything the callbacks enqueued was
    // reported by the re-entered path that owns it. The list entries are the request packets
    // owned by this queue, released by the list's free callback on ListRemoveFirst (no extra
    // PacketFree - that would double free). The per-bearer alarm is cancelled inside
    // AttClearConnectInfo, which releases the slot.
    for (; drainSize > 0; --drainSize) {
        ListRemoveFirst(connect->instruct);
    }
    AttClearConnectInfo(connect);
    return;
}

/* Async receive context of the EATT data path: the received packet is ref'd here
 * (L2CAP does not own it) and dispatched on the ATT processing queue. */
typedef struct AttEattRecvLeDataAsyncContext {
    uint16_t lecid;
    Packet *packet;
} AttEattRecvLeDataAsyncContext;

/* Answer an EATT MTU exchange request (0x02) with 0x01 ERROR_RSP. An Enhanced
 * ATT bearer has no MTU exchange (Vol 3 Part F §3.2.8, dynamic CID → ATT_MTU = L2CAP
 * MTU); as server we reply Request Not Supported, Handle In Error = 0x0000 (§3.3).
 * ERROR_RSP format (Table 3.3): [0] 0x01 PDU opcode, [1] Request Opcode In Error,
 * [2:4] Attribute Handle In Error, [4] Error Code 0x06. */
static void AttEattReplyMtuExchangeError(const AttConnectInfo *connect)
{
    uint8_t *data = NULL;
    Packet *packet = NULL;
    int ret = BT_OPERATION_FAILED;

    if (connect == NULL) {
        LOG_WARN("%{public}s: connect is NULL", __FUNCTION__);
        return;
    }

    packet = PacketMalloc(0, 0, sizeof(uint8_t) + STEP_FOUR);
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    data = BufferPtr(PacketContinuousPayload(packet));
    data[0] = ERROR_RESPONSE;
    data[1] = EXCHANGE_MTU_REQUEST;
    data[STEP_TWO] = 0x00;  // Attribute Handle In Error = 0x0000, byte-wise to stay alignment-safe
    data[STEP_THREE] = 0x00;
    data[STEP_FOUR] = ATT_REQUEST_NOT_SUPPORTED;

    // Sent on the source bearer (EATT branch of AttResponseSendData); the error
    // response is a plain response, no transaction tracking needed.
    ret = AttResponseSendData(connect, packet);
    if (ret != BT_SUCCESS) {
        LOG_WARN("%{public}s: send MTU error response failed, ret = %{public}d", __FUNCTION__, ret);
    }
    PacketFree(packet);
    return;
}

static void AttEattRecvLeDataAsync(const void *context)
{
    uint8_t opcode = 0;
    AttConnectInfo *connect = NULL;
    AttEattRecvLeDataAsyncContext *asyncPtr = (AttEattRecvLeDataAsyncContext *)context;

    // EATT dynamic CIDs are globally unique, so the lcid resolves the sending
    // bearer exactly (the fixed LE_CID is shared by every UATT bearer). The FIFO
    // ordering of this ATT queue rules out resolving to a different bearer that
    // reused the lcid (see LeEattRecvSendDataCallbackAsync in att_common.c).
    connect = AttGetConnectInfoByLeCid(asyncPtr->lecid);
    if (connect == NULL) {
        LOG_WARN("%{public}s: bearer not found, lcid = %hu", __FUNCTION__, asyncPtr->lecid);
        goto ATT_EATT_RECV_END;
    }

    // An ATT PDU is at least the 1-octet Attribute Opcode (Vol 3 Part F §3.3.1
    // Table 3.2); drop an empty SDU that cannot form a valid PDU.
    if (PacketPayloadSize(asyncPtr->packet) < sizeof(uint8_t)) {
        LOG_WARN("%{public}s: empty ATT PDU on EATT bearer, lcid = %hu, drop", __FUNCTION__, asyncPtr->lecid);
        goto ATT_EATT_RECV_END;
    }

    PacketExtractHead(asyncPtr->packet, &opcode, sizeof(uint8_t));

    // The constraint interception runs before the FunctionList dispatch: 0xD2 is
    // banned on an Enhanced ATT bearer and a command is never answered (Vol 3
    // Part F §3.4.5,4 + §3.3), 0x02 is answered with the ERROR_RSP above.
    if (opcode == SIGNED_WRITE_COMMAND) {
        LOG_WARN("%{public}s: SIGNED_WRITE_CMD 0xD2 on EATT bearer, drop", __FUNCTION__);
    } else if (opcode == EXCHANGE_MTU_REQUEST) {
        AttEattReplyMtuExchangeError(connect);
    } else {
        Buffer *buffer = PacketContinuousPayload(asyncPtr->packet);
        recvDataFunction functionPtr = GetFunctionArrayDress()[opcode];
        if (functionPtr != NULL) {
            functionPtr(connect, buffer);
        } else {
            LOG_WARN("UnKnow OpCode : %hhu", opcode);
            if ((opcode & 0b01000000) == 0) {
                AttErrorCode(connect, opcode);
            }
        }
    }

ATT_EATT_RECV_END:
    PacketFree(asyncPtr->packet);
    MEM_MALLOC.free(asyncPtr);
    return;
}

static void AttEattRecvLeDataAsyncDestroy(const void *context)
{
    AttEattRecvLeDataAsyncContext *asyncPtr = (AttEattRecvLeDataAsyncContext *)context;

    PacketFree(asyncPtr->packet);
    MEM_MALLOC.free(asyncPtr);
    return;
}

void AttEattRecvLeData(uint16_t lcid, Packet *pkt, void *ctx)
{
    Packet *packetPtr = NULL;
    AttEattRecvLeDataAsyncContext *asyncPtr = NULL;

    LOG_INFO("%{public}s enter, lcid = %hu", __FUNCTION__, lcid);

    packetPtr = PacketRefMalloc(pkt);
    if (packetPtr == NULL) {
        LOG_ERROR("PacketRefMalloc failed");
        return;
    }
    asyncPtr = MEM_MALLOC.alloc(sizeof(AttEattRecvLeDataAsyncContext));
    if (asyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        PacketFree(packetPtr);
        return;
    }
    asyncPtr->lecid = lcid;
    asyncPtr->packet = packetPtr;

    AttAsyncProcess(AttEattRecvLeDataAsync, AttEattRecvLeDataAsyncDestroy, asyncPtr);
    return;
}

void AttEattRecvLeDisconnectionReq(uint16_t lcid, uint8_t id, void *ctx)
{
    AttConnectInfo *connect = NULL;

    LOG_INFO("%{public}s enter, lcid = %hu, id = %hu", __FUNCTION__, lcid, id);

    // The peer asks to disconnect this bearer (0x06): answer 0x07 so it can release its channel,
    // then release the bearer; respond even with no local bearer, the channel still exists in L2CAP.
    connect = AttGetConnectInfoByLeCid(lcid);
    L2CIF_LeDisconnectionRsp(lcid, id, NULL);
    if (connect != NULL) {
        AttEattReleaseBearer(connect);
    }
    return;
}

void AttEattRecvLeDisconnectionRsp(uint16_t lcid, void *ctx)
{
    AttConnectInfo *connect = NULL;

    LOG_INFO("%{public}s enter, lcid = %hu", __FUNCTION__, lcid);

    // The peer answered our 0x06: L2CAP has already freed the channel, so only the ATT bearer is
    // released here.
    connect = AttGetConnectInfoByLeCid(lcid);
    if (connect == NULL) {
        LOG_WARN("%{public}s: bearer not found, lcid = %hu", __FUNCTION__, lcid);
        return;
    }

    AttEattReleaseBearer(connect);
    return;
}

// Find the connection with an in-flight EATT establishment whose batch includes this lcid (the
// source CIDs were retained at dispatch, AttEattEstablishSendResult). Only the initiator keeps
// pending state, so eattEstablishCb != NULL gates the search.
static AttConnectInfo *AttEattFindEstablishParentByLcid(uint16_t lcid)
{
    AttConnectInfo *start = AttGetConnectStart();
    AttConnectInfo *connect = NULL;
    uint16_t index = 0;
    uint16_t i = 0;

    for (index = 0; index < MAXCONNECT; index++) {
        connect = start + index;
        if ((connect->eattEstablishCb == NULL) || (connect->eattLcidCount == 0)) {
            continue;
        }
        for (i = 0; i < connect->eattLcidCount; i++) {
            if (connect->eattLcids[i] == lcid) {
                return connect;
            }
        }
    }
    return NULL;
}

void AttEattDisconnected(uint16_t lcid, uint8_t reason, void *ctx)
{
    AttConnectInfo *connect = NULL;
    AttConnectInfo *pendingParent = NULL;

    LOG_INFO("%{public}s enter, lcid = %hu, reason = %hu", __FUNCTION__, lcid, reason);

    connect = AttGetConnectInfoByLeCid(lcid);
    if (connect == NULL) {
        // The channel never became a bearer: its batch's 0x18 never settled (RTX timeout, short
        // grant, link loss) or the peer tore the channel down during establishment. Settle the
        // pending establishment so AttEattEstablish's caller is not left hanging; a no-op when
        // no batch is pending.
        pendingParent = AttEattFindEstablishParentByLcid(lcid);
        if (pendingParent != NULL) {
            AttEattResolveEstablish(pendingParent, BT_OPERATION_FAILED);
        } else {
            LOG_WARN("%{public}s: bearer not found, lcid = %hu", __FUNCTION__, lcid);
        }
        return;
    }

    // Abnormal disconnect (ACL teardown, link loss, protocol violation): abort the bearer. When
    // this bearer belongs to an in-flight establishment batch, the batch is dead as well — its
    // remaining channels are being torn down by L2CAP right after this callback.
    pendingParent = AttEattFindEstablishParentByLcid(lcid);
    if (pendingParent != NULL) {
        AttEattResolveEstablish(pendingParent, BT_OPERATION_FAILED);
    }
    AttEattReleaseBearer(connect);
    return;
}
