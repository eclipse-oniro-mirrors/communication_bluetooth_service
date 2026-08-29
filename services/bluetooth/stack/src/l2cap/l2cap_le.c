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

#include "l2cap_le.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "hci/hci.h"
#include "l2cap_le_internal.h"

L2capLeInstance g_l2capLeInst;

int L2capLeInitialized()
{
    L2capLeInstance *inst = &g_l2capLeInst;

    if (inst->connList != NULL) {
        return BT_SUCCESS;
    }

    return BT_BAD_STATUS;
}

L2capLePsm *L2capLeGetPsm(uint16_t lpsm)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    L2capLePsm *lepsm = NULL;
    ListNode *node = NULL;

    node = ListGetFirstNode(inst->psmList);
    while (node != NULL) {
        lepsm = ListGetNodeData(node);
        if (lepsm->lpsm == lpsm) {
            return lepsm;
        }

        node = ListGetNextNode(node);
    }

    return NULL;
}

L2capLeConnection *L2capLeGetConnection(uint16_t aclHandle)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    L2capLeConnection *leconn = NULL;
    ListNode *node = NULL;

    node = ListGetFirstNode(inst->connList);
    while (node != NULL) {
        leconn = ListGetNodeData(node);
        if (leconn->aclHandle == aclHandle) {
            return leconn;
        }

        node = ListGetNextNode(node);
    }

    return NULL;
}

L2capLeConnection *L2capLeGetConnection2(const BtAddr *addr)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    L2capLeConnection *conn = NULL;
    ListNode *node = NULL;

    node = ListGetFirstNode(inst->connList);
    while (node != NULL) {
        conn = ListGetNodeData(node);
        if (memcmp(&(conn->addr), addr, sizeof(BtAddr)) == 0) {
            return conn;
        }

        node = ListGetNextNode(node);
    }

    return NULL;
}

// Listen for the HCI 0x08 encryption change event to maintain the link encryption state.
// Vol 3 Part G 5.3.2 requires EATT channels to be encrypted; the 0x08 parameters carry only
// Encryption_Enabled (no key size or auth level), the latter two are injected by the upper layer.
static void L2capLeRecvEncryptionChange(const HciEncryptionChangeEventParam *eventParam)
{
    L2capLeConnection *conn = NULL;

    if (eventParam == NULL) {
        return;
    }

    conn = L2capLeGetConnection(eventParam->connectionHandle);
    if (conn == NULL) {
        return;
    }

    if (eventParam->status != 0) {
        // A failed encryption change leaves the link unencrypted (HCI spec: the
        // encryption is off after ENCRYPTION_FAILED_TO_BE_ESTABLISHED); a stale
        // encrypt=1 would let inbound 0x17 through the Vol 3 Part G 5.3.2 check.
        conn->eatt.encrypt = 0;
        return;
    }

    conn->eatt.encrypt = (eventParam->encryptionEnabled != LINK_LEVEL_ENCRYPTION_OFF) ? 1 : 0;
    return;
}

// Refresh the stored connection parameters with the actual granted values on an LE Connection Update
// Complete. Vol 3 Part G 5.4 computes the slave collision retry delay from the current connSlaveLatency
// and connInterval, so a later parameter update must not leave the initial request values stale; the
// initial values come from the L2CAP_LeConnect request (an upper bound of the granted interval), which
// is why only updates are corrected here.
static void L2capLeRecvConnectionUpdateComplete(const HciLeConnectionUpdateCompleteEventParam *eventParam)
{
    L2capLeConnection *conn = NULL;

    if ((eventParam == NULL) || (eventParam->status != 0)) {
        return;
    }

    conn = L2capLeGetConnection(eventParam->connectionHandle);
    if (conn == NULL) {
        return;
    }

    conn->connIntervalUnits = eventParam->connInterval;
    conn->connSlaveLatency = eventParam->connLatency;
    return;
}

// Stash the granted connection parameters of an LE Connection Complete for the Vol 3 Part G 5.4 slave
// retry delay. The L2CAP connection object is created asynchronously on the L2CAP queue after this event
// (the BTM ACL callback posts L2capLeConnectComplete), so an inbound connection established without a
// L2CAP_LeConnect request would otherwise keep connIntervalUnits == 0; L2capLeConnectComplete applies the
// stash (matched by handle) to the new connection object. The stash is a small per-handle slot array:
// two connections completing back-to-back must not overwrite each other's unconsumed parameters
static void L2capLeRecvConnectionComplete(const HciLeConnectionCompleteEventParam *eventParam)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    L2capLeGrantedConnParam *slot = NULL;
    uint8_t i;

    if ((eventParam == NULL) || (eventParam->status != 0)) {
        return;
    }

    // replace the entry of the same handle first, then a free slot, otherwise the first occupied slot
    // (evicting a not-yet-consumed entry only when all slots are occupied, which needs more concurrent
    // connection completions than the ring can hold; the evicted connection degrades to the requested
    // parameters, same as the old single slot)
    for (i = 0; i < L2CAP_LE_GRANTED_CONN_PARAM_MAX; i++) {
        if (inst->grantedConnParams[i].valid && (inst->grantedConnParams[i].handle == eventParam->connectionHandle)) {
            slot = &inst->grantedConnParams[i];
            break;
        }
    }
    if (slot == NULL) {
        for (i = 0; i < L2CAP_LE_GRANTED_CONN_PARAM_MAX; i++) {
            if (!inst->grantedConnParams[i].valid) {
                slot = &inst->grantedConnParams[i];
                break;
            }
        }
    }
    if (slot == NULL) {
        LOG_WARN("L2capLeRecvConnectionComplete: granted parameter slots exhausted, handle = 0x%04X",
            eventParam->connectionHandle);
        slot = &inst->grantedConnParams[0];
    }

    slot->handle = eventParam->connectionHandle;
    slot->interval = eventParam->connInterval;
    slot->latency = eventParam->connLatency;
    slot->valid = 1;
    return;
}

// HCI event callback registration object: HCI_RegisterEventCallbacks stores the pointer in the event
// callback table (list-based in hci_evt.c), so a static object keeps the module lifetime stable.
//
// THREAD CONTRACT: the handlers above run on the Stack thread as HCI callbacks, which is the same
// thread that drains the L2CAP processing queue (see btm_thread.c). They therefore touch shared
// connection state directly (conn->eatt.encrypt, connIntervalUnits, the granted parameter stash)
// without a queue hop; this invariant is load-bearing and must not be broken by moving the HCI
// reactor to another thread without re-dispatching these handlers through the queue.
static const HciEventCallbacks g_l2capLeHciCallbacks = {
    .encryptionChange = L2capLeRecvEncryptionChange,
    .leConnectionComplete = L2capLeRecvConnectionComplete,
    .leConnectionUpdateComplete = L2capLeRecvConnectionUpdateComplete,
};

L2capLeChannel *L2capLeGetChannel(L2capLeConnection *conn, int16_t lcid)
{
    L2capLeChannel *lechan = NULL;
    ListNode *node = NULL;

    node = ListGetFirstNode(conn->chanList);
    while (node != NULL) {
        lechan = ListGetNodeData(node);
        if (lechan->lcid == lcid) {
            return lechan;
        }

        node = ListGetNextNode(node);
    }

    return NULL;
}

static void L2capLeGetChannel2(uint16_t lcid, L2capLeConnection **conn, L2capLeChannel **chan)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    ListNode *node = NULL;

    node = ListGetFirstNode(inst->connList);
    while (node != NULL) {
        *conn = ListGetNodeData(node);
        *chan = L2capLeGetChannel(*conn, lcid);
        if ((*chan) != NULL) {
            break;
        }

        node = ListGetNextNode(node);
    }

    return;
}

static void L2capLeGetChannel3(uint16_t aclHandle, uint16_t lcid, L2capLeConnection **conn, L2capLeChannel **chan)
{
    *conn = L2capLeGetConnection(aclHandle);
    if ((*conn) == NULL) {
        return;
    }

    *chan = L2capLeGetChannel(*conn, lcid);
    return;
}

static L2capLeChannel *L2capLeGetChannel4(L2capLeConnection *conn, int16_t ident)
{
    L2capLeChannel *chan = NULL;
    ListNode *node = NULL;

    node = ListGetFirstNode(conn->chanList);
    while (node != NULL) {
        chan = ListGetNodeData(node);
        // The 0x15 handler only answers a legacy 0x14 request this side sent: never
        // match EATT 0x17 channels (they share the per-connection identifier space
        // via L2capLeGetNewIdentifier) or channels that already completed (the
        // identifier survives a successful handshake).
        if ((chan->connIdentifier == ident) && (chan->lpsm != L2CAP_LE_EATT_PSM) &&
            (chan->state == L2CAP_CHANNEL_CONNECT_OUT_REQ)) {
            return chan;
        }

        node = ListGetNextNode(node);
    }

    return NULL;
}

static uint16_t L2capLeGetNewLcid()
{
    L2capLeInstance *inst = &g_l2capLeInst;
    uint16_t lcid = L2CAP_LE_MIN_CID;

    if (inst->nextLcid == 0) {
        L2capLeConnection *conn = NULL;
        L2capLeChannel *chan = NULL;

        while (1) {
            L2capLeGetChannel2(lcid, &conn, &chan);
            if (chan == NULL) {
                break;
            }

            lcid += 1;
        }
    } else {
        lcid = inst->nextLcid;

        if (lcid == L2CAP_LE_MAX_CID) {
            inst->nextLcid = 0;
        } else {
            inst->nextLcid += 1;
        }
    }

    return lcid;
}

L2capLeChannel *L2capLeNewChannel(L2capLeConnection *conn, uint16_t lpsm, uint16_t rpsm)
{
    L2capLeChannel *chan = NULL;
    if (conn == NULL) {
        LOG_ERROR("%{public}s: invalid null connection", __FUNCTION__);
        return NULL;
    }
    chan = L2capAlloc(sizeof(L2capLeChannel));
    if (chan == NULL) {
        return NULL;
    }

    chan->lcid = L2capLeGetNewLcid();
    chan->lpsm = lpsm;
    chan->rpsm = rpsm;
    // keep rcid/connIdentifier cleared until assigned so that
    // L2capLeEattGetChannelByRcid / L2capLeEattGetPendingChannels never match a not-yet-established channel
    chan->rcid = 0;
    chan->connIdentifier = 0;
    chan->lcfg.mps = L2capGetRxBufferSize() - L2CAP_SIZE_6;
    chan->lcfg.credit = L2CAP_LE_DEFAULT_CREDIT;
    // peerCredits must track the granted receive credit, read by the zero-credit gate
    chan->peerCredits = chan->lcfg.credit;
    chan->state = L2CAP_CHANNEL_IDLE;
    chan->rxSarPacket = NULL;

    if (!ListAddLast(conn->chanList, chan)) {
        // OOM: the channel must not leave this function as if it were trackable. The rollback
        // paths look the channel up by lcid (L2capLeGetChannel) and delete it through the list
        // (L2capLeDeleteChannel -> ListRemoveNode asserts on NULL), so an untracked channel
        // would crash the rollback and otherwise leak until the process exits. Free it here and
        // return NULL, the callers' existing NULL checks then apply.
        L2capFree(chan);
        return NULL;
    }
    return chan;
}

static void L2capLeDestroyChannel(L2capLeChannel *chan)
{
    if (chan->txList != NULL) {
        ListNode *node = NULL;
        Packet *pkt = NULL;

        while (1) {
            node = ListGetFirstNode(chan->txList);
            if (node == NULL) {
                break;
            }

            pkt = ListGetNodeData(node);
            ListRemoveNode(chan->txList, pkt);
            PacketFree(pkt);
        }

        ListDelete(chan->txList);
    }

    if (chan->rxSarPacket != NULL) {
        PacketFree(chan->rxSarPacket);
        chan->rxSarPacket = NULL;
    }

    L2capFree(chan);
    return;
}

void L2capLeDeleteChannel(L2capLeConnection *conn, L2capLeChannel *chan, uint16_t removeAcl)
{
    ListRemoveNode(conn->chanList, chan);
    L2capLeDestroyChannel(chan);

    if (removeAcl) {
        if (ListGetFirstNode(conn->chanList) == NULL) {
            // Reason: REMOTE USER TERMINATED CONNECTION
            L2capDisconnect(conn->aclHandle, 0x13);
        }
    }

    return;
}

L2capLeConnection *L2capLeNewConnection(const BtAddr *addr, uint16_t aclHandle, uint8_t role)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    L2capLeConnection *conn = NULL;

    conn = L2capAlloc(sizeof(L2capLeConnection));
    if (conn == NULL) {
        return NULL;
    }

    (void)memcpy_s(&(conn->addr), sizeof(BtAddr), addr, sizeof(BtAddr));
    conn->aclHandle = aclHandle;
    conn->nextIdentifier = L2CAP_MIN_IDENTIFIER;
    conn->role = role;
    conn->connIntervalUnits = 0;
    conn->connSlaveLatency = 0;

    conn->chanList = ListCreate(NULL);
    conn->pendingList = ListCreate(NULL);
    conn->eatt.reconfigList = ListCreate(NULL);
    conn->eatt.retryTimer = AlarmCreate("", false);
    ListAddFirst(inst->connList, conn);
    return conn;
}

void L2capLeDeleteConnection(L2capLeConnection *conn)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    ListNode *node = NULL;

    // Vol 3 Part G 5.4: stop the slave collision retry timer; the pending entries below already
    // cover the other in-flight timers, this one is a standalone connection-level alarm
    if (conn->eatt.retryTimer != NULL) {
        AlarmCancel(conn->eatt.retryTimer);
        AlarmDelete(conn->eatt.retryTimer);
        conn->eatt.retryTimer = NULL;
    }

    if (conn->chanList != NULL) {
        L2capLeChannel *chan = NULL;

        while (1) {
            node = ListGetFirstNode(conn->chanList);
            if (node == NULL) {
                break;
            }

            chan = ListGetNodeData(node);
            ListRemoveNode(conn->chanList, chan);
            L2capLeDestroyChannel(chan);
        }

        ListDelete(conn->chanList);
    }

    if (conn->pendingList != NULL) {
        L2capClearPendingRequest(conn->pendingList);
        ListDelete(conn->pendingList);
    }

    // clean up the in-flight 0x19 reconfigure requests, they become meaningless after link teardown
    if (conn->eatt.reconfigList != NULL) {
        L2capLeReconfigReq *req = NULL;
        ListNode *rnode = NULL;

        while (1) {
            rnode = ListGetFirstNode(conn->eatt.reconfigList);
            if (rnode == NULL) {
                break;
            }

            req = ListGetNodeData(rnode);
            ListRemoveNode(conn->eatt.reconfigList, req);
            L2capFree(req);
        }

        ListDelete(conn->eatt.reconfigList);
    }

    ListRemoveNode(inst->connList, conn);
    L2capFree(conn);

    // if no connection exists, reset nextLcid value
    if (ListGetFirstNode(inst->connList) == NULL) {
        inst->nextLcid = L2CAP_LE_MIN_CID;
    }

    return;
}

uint8_t L2capLeGetNewIdentifier(L2capLeConnection *leconn)
{
    uint8_t ident;

    ident = leconn->nextIdentifier;
    if (ident == L2CAP_MAX_IDENTIFIER) {
        leconn->nextIdentifier = L2CAP_MIN_IDENTIFIER;
    } else {
        leconn->nextIdentifier += 1;
    }

    return ident;
}


// 0x17 collision retry, Vol 3 Part G 5.4: after the slave retry delay elapsed, resend the batch as a
// fresh request; clear the batch request identifier first so L2capLeEattGetPendingChannels() recollects
// the channels with a new identifier, then free the pending entry (the caller must not touch lereq)
static void L2capLeEattRetryCollision(L2capLeConnection *conn, L2capPendingRequest *lereq)
{
    L2capLeChannel *chan = NULL;
    ListNode *chanNode = NULL;

    chanNode = ListGetFirstNode(conn->chanList);
    while (chanNode != NULL) {
        chan = ListGetNodeData(chanNode);
        // lpsm filter: the identifier space is shared with legacy 0x14 channels, a
        // 0x14 channel that reused the value must not be detached from its own request.
        // batchSeq filter: within the RTX window the identifier may wrap onto a newer batch of
        // the same connection (255 allocations), whose channels must keep their own in-flight
        // state; only the batch this pending entry created is released.
        if ((chan->connIdentifier == lereq->identifier) && (chan->lpsm == L2CAP_LE_EATT_PSM) &&
            (chan->batchSeq == lereq->seq)) {
            chan->connIdentifier = 0;
        }
        chanNode = ListGetNextNode(chanNode);
    }

    // the per-entry collision marker dies with lereq; other in-flight batches keep their own markers
    AlarmCancel(lereq->timer);
    AlarmDelete(lereq->timer);
    L2capFree(lereq);
    L2capLeEattSendPendingRequest(conn);
}

// 0x17 request timeout: remove all in-flight channels of this request, matched by the request's
// batch sequence (and identifier); a 0x17 is a batch request whose single pending entry covers
// multiple channels, unlike 0x14 which removes a single channel by lcid
static void L2capLeEattDiscardTimedOutBatch(L2capLeConnection *conn, uint8_t identifier, uint32_t seq)
{
    ListNode *node = NULL;
    ListNode *next = NULL;
    L2capLeChannel *chan = NULL;
    L2capLePsm *psm = NULL;

    node = ListGetFirstNode(conn->chanList);
    while (node != NULL) {
        // save the successor before the delete, L2capLeDeleteChannel frees the current node
        next = ListGetNextNode(node);
        chan = ListGetNodeData(node);
        // lpsm filter: a 0x17 timeout only discards its own EATT batch, a legacy 0x14 channel that
        // reused the identifier must keep its own 0x14 timeout handling. batchSeq filter: within
        // the RTX window the identifier may wrap onto a newer batch of the same connection, whose
        // channels must not be discarded by this stale expiry
        if ((chan->state == L2CAP_CHANNEL_CONNECT_OUT_REQ) && (chan->connIdentifier == identifier) &&
            (chan->batchSeq == seq) && (chan->lpsm == L2CAP_LE_EATT_PSM)) {
            psm = L2capLeGetPsm(chan->lpsm);
            if ((psm != NULL) && (psm->service.leDisconnectAbnormal != NULL)) {
                LOG_DEBUG("L2capCallback leDisconnectAbnormal:%{public}d begin, cid = 0x%04X, reason = 0", __LINE__,
                    chan->lcid);
                psm->service.leDisconnectAbnormal(chan->lcid, 0, psm->ctx);
                LOG_DEBUG("L2capCallback leDisconnectAbnormal:%{public}d end", __LINE__);
            }

            L2capLeDeleteChannel(conn, chan, 0);
        }
        node = next;
    }
}

// 0x14 LE credit based request timeout: disconnect the single channel by lcid
static void L2capLeTimeoutDisconnectChannel(L2capLeConnection *conn, uint16_t lcid)
{
    L2capLeChannel *chan = NULL;
    L2capLePsm *psm = NULL;

    chan = L2capLeGetChannel(conn, lcid);
    if (chan != NULL) {
        psm = L2capLeGetPsm(chan->lpsm);
        if ((psm != NULL) && (psm->service.leDisconnectAbnormal != NULL)) {
            LOG_DEBUG("L2capCallback leDisconnectAbnormal:%{public}d begin, cid = 0x%04X, reason = 0", __LINE__, lcid);
            psm->service.leDisconnectAbnormal(lcid, 0, psm->ctx);
            LOG_DEBUG("L2capCallback leDisconnectAbnormal:%{public}d end", __LINE__);
        }

        // EATT rides the ACL that hosts the UATT fixed channel (not in chanList), so
        // the last EATT close must not drop the ACL, mirroring the 0x07 response path
        // (Vol 3 Part G 6.1.1).
        L2capLeDeleteChannel(conn, chan, (chan->lpsm == L2CAP_LE_EATT_PSM) ? 0 : 1);
    }
}

static void L2capLeResponseTimeout(const void *parameter)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    L2capLeConnection *conn = NULL;
    L2capPendingRequest *lereq = NULL;
    ListNode *connNode = NULL;
    uint32_t key = (uint32_t)(uintptr_t)parameter;

    if (inst->connList == NULL) {
        return;
    }

    // The RTX alarm carries the pending entry's own 32-bit sequence, never a raw pointer: the
    // entry may have been freed by the response (or its connection torn down) between arming
    // and expiry. The sequence is allocated globally (not per connection) and fits the
    // pointer-width alarm parameter on every target, 32-bit ARM included, so the connection
    // cannot be derived from the key and the entry is searched across the connection list.
    // Resolving by the unique sequence (instead of the wrap-around 8-bit identifier) makes a
    // stale expiry task resolve to exactly the request whose timer fired, never to a newer
    // request of the same connection (ABA, see L2capPendingRequest::seq). A miss means the
    // request already completed, a safe no-op.
    connNode = ListGetFirstNode(inst->connList);
    while (connNode != NULL) {
        conn = ListGetNodeData(connNode);
        lereq = L2capGetPendingRequestByKey(conn->pendingList, key);
        if (lereq != NULL) {
            break;
        }

        connNode = ListGetNextNode(connNode);
    }
    if (lereq == NULL) {
        return;
    }
    ListRemoveNode(conn->pendingList, lereq);

    if (lereq->code == L2CAP_CREDIT_BASED_CONNECTION_REQUEST) {
        if (lereq->collision) {
            L2capLeEattRetryCollision(conn, lereq);
            return;
        }

        // 0x17 request timeout: discard the in-flight batch channels matched by batch sequence
        L2capLeEattDiscardTimedOutBatch(conn, lereq->identifier, lereq->seq);
    } else if (lereq->code == L2CAP_CREDIT_BASED_RECONFIGURE_REQUEST) {
        // 0x19 reconfigure request timeout: channels keep the old config, only the in-flight request is removed
        L2capLeEattDestroyReconfig(conn, lereq->identifier);
    } else if (lereq->lcid > 0) {
        L2capLeTimeoutDisconnectChannel(conn, lereq->lcid);
    }

    AlarmDelete(lereq->timer);
    L2capFree(lereq);

    return;
}

void L2capLeResponseTimeoutCallback(void *parameter)
{
    L2capAsynchronousProcess(L2capLeResponseTimeout, NULL, parameter);
    return;
}

// Test seam: expire the in-flight 0x17 batch of a connection as if its RTX timer fired. A live peer always
// answers a valid 0x17 within milliseconds, so the 30 s RTX (L2CAP_DEFAULT_RTX) cannot be waited in a
// two-device test; calling this from the L2CAP queue runs the same L2capLeResponseTimeout dispatch the real
// timer would, before the peer's 0x18 is processed. Returns BT_SUCCESS when a pending 0x17 was expired.
int L2capLeEattExpirePendingRequest(uint16_t aclHandle)
{
    L2capLeConnection *conn = NULL;
    L2capPendingRequest *lereq = NULL;
    ListNode *node = NULL;

    conn = L2capLeGetConnection(aclHandle);
    if (conn == NULL) {
        return BT_BAD_PARAM;
    }

    node = ListGetFirstNode(conn->pendingList);
    while (node != NULL) {
        lereq = ListGetNodeData(node);
        if (lereq->code == L2CAP_CREDIT_BASED_CONNECTION_REQUEST) {
            // run the same key-based dispatch a real RTX expiry would: the timeout resolves the
            // pending entry by its sequence and removes it itself. The entry's own sequence is
            // exactly the value the alarm parameter carries (the identifier alone is ambiguous
            // after wrap-around, see L2capPendingRequest::seq)
            L2capLeResponseTimeout((void *)(uintptr_t)lereq->seq);
            return BT_SUCCESS;
        }
        node = ListGetNextNode(node);
    }

    return BT_BAD_PARAM;
}

static int L2capSendCreditBasedConnectionReq(L2capLeConnection *conn, L2capLeChannel *chan)
{
    Packet *pkt = NULL;
    uint8_t buff[10] = {0};
    L2capSignalHeader signal = {0};

    L2capCpuToLe16(buff + 0, chan->rpsm);
    L2capCpuToLe16(buff + L2CAP_OFFSET_2, chan->lcid);
    L2capCpuToLe16(buff + L2CAP_OFFSET_4, chan->lcfg.mtu);
    L2capCpuToLe16(buff + L2CAP_OFFSET_6, chan->lcfg.mps);
    L2capCpuToLe16(buff + L2CAP_OFFSET_8, chan->lcfg.credit);

    signal.code = L2CAP_LE_CREDIT_BASED_CONNECTION_REQUEST;
    signal.identifier = L2capLeGetNewIdentifier(conn);
    signal.length = sizeof(buff);

    chan->connIdentifier = signal.identifier;
    pkt = L2capBuildSignalPacket(L2CAP_LE_SIGNALING_CHANNEL, &signal, buff);
    if (L2capCreatePendingRequest(
        conn->pendingList, chan->lcid, &signal, L2CAP_DEFAULT_RTX, L2capLeResponseTimeoutCallback) != BT_SUCCESS) {
        // no pending entry and no RTX timer: the peer's 0x15 would have nothing to match and the
        // channel could never be recycled by the timeout, so the request must not be sent
        PacketFree(pkt);
        return BT_NO_MEMORY;
    }
    return L2capLeSendPacket(conn->aclHandle, pkt);
}

static int L2capSendCreditBasedConnectionRsp(
    L2capLeConnection *conn, L2capLeChannel *chan, uint8_t ident, uint16_t result)
{
    Packet *pkt = NULL;
    uint8_t buff[10] = {0};
    L2capSignalHeader signal = {0};

    L2capCpuToLe16(buff + 0, chan->lcid);
    L2capCpuToLe16(buff + L2CAP_OFFSET_2, chan->lcfg.mtu);
    L2capCpuToLe16(buff + L2CAP_OFFSET_4, chan->lcfg.mps);
    L2capCpuToLe16(buff + L2CAP_OFFSET_6, chan->lcfg.credit);
    L2capCpuToLe16(buff + L2CAP_OFFSET_8, result);

    signal.code = L2CAP_LE_CREDIT_BASED_CONNECTION_RESPONSE;
    signal.identifier = ident;
    signal.length = sizeof(buff);

    pkt = L2capBuildSignalPacket(L2CAP_LE_SIGNALING_CHANNEL, &signal, buff);
    return L2capLeSendPacket(conn->aclHandle, pkt);
}

int L2capLeSendDisconnectionReq(L2capLeConnection *conn, const L2capLeChannel *chan)
{
    Packet *pkt = NULL;
    uint8_t buff[4] = {0};
    L2capSignalHeader signal = {0};

    L2capCpuToLe16(buff + 0, chan->rcid);
    L2capCpuToLe16(buff + L2CAP_OFFSET_2, chan->lcid);

    signal.code = L2CAP_DISCONNECTION_REQUEST;
    signal.identifier = L2capLeGetNewIdentifier(conn);
    signal.length = sizeof(buff);

    pkt = L2capBuildSignalPacket(L2CAP_LE_SIGNALING_CHANNEL, &signal, buff);
    if (L2capCreatePendingRequest(
        conn->pendingList, chan->lcid, &signal, L2CAP_DEFAULT_RTX, L2capLeResponseTimeoutCallback) != BT_SUCCESS) {
        // no pending entry and no RTX timer: the peer's 0x07 would have nothing to match, so the
        // request must not be sent
        PacketFree(pkt);
        return BT_NO_MEMORY;
    }
    return L2capLeSendPacket(conn->aclHandle, pkt);
}

static int L2capLeSendDisconnectionRsp(const L2capLeConnection *conn, const L2capLeChannel *chan, uint8_t ident)
{
    Packet *pkt = NULL;
    uint8_t buff[4] = {0};
    L2capSignalHeader signal = {0};

    L2capCpuToLe16(buff + 0, chan->lcid);
    L2capCpuToLe16(buff + L2CAP_OFFSET_2, chan->rcid);

    signal.code = L2CAP_DISCONNECTION_RESPONSE;
    signal.identifier = ident;
    signal.length = sizeof(buff);

    pkt = L2capBuildSignalPacket(L2CAP_LE_SIGNALING_CHANNEL, &signal, buff);
    return L2capLeSendPacket(conn->aclHandle, pkt);
}

static int L2capLeSendFlowControlCredit(L2capLeConnection *conn, const L2capLeChannel *chan, uint16_t credit)
{
    Packet *pkt = NULL;
    uint8_t buff[4] = {0};
    L2capSignalHeader signal = {0};

    L2capCpuToLe16(buff + 0, chan->lcid);
    L2capCpuToLe16(buff + L2CAP_OFFSET_2, credit);

    signal.code = L2CAP_LE_FLOW_CONTROL_CREDIT;
    signal.identifier = L2capLeGetNewIdentifier(conn);
    signal.length = sizeof(buff);

    pkt = L2capBuildSignalPacket(L2CAP_LE_SIGNALING_CHANNEL, &signal, buff);
    return L2capLeSendPacket(conn->aclHandle, pkt);
}

// chapter 3.4.2/4.24: the first K-frame of an SDU carries the L2CAP SDU Length field, so its L2CAP
// header is two octets wider than the continuation K-frames (6 vs 4 octets, as built by
// L2capLeSegmentPacketWithCredit and L2CAP_LeSendData); that header width marks the SDU boundary
// for the per-SDU credit gate
static bool L2capLeIsFirstSduFrame(const Packet *pkt)
{
    return BufferGetSize(PacketHead(pkt)) == (L2CAP_HEADER_LENGTH + L2CAP_SIZE_2);
}

// chapter 4.24: notify the peer service once per credit-gate state change, leRemoteBusy
// is delivered when the local credit gate closes (credit == 0) or re-opens after a drain.
static void L2capLeTxNotifyBusyState(L2capLeChannel *chan, uint8_t newState)
{
    L2capLePsm *psm = NULL;

    if (chan->busyState == newState) {
        return;
    }
    chan->busyState = newState;
    psm = L2capLeGetPsm(chan->lpsm);
    if ((psm != NULL) && (psm->service.leRemoteBusy != NULL)) {
        LOG_DEBUG("L2capCallback leRemoteBusy:%{public}d begin, cid = 0x%04X, busyState = %hhu", __LINE__,
            chan->lcid, chan->busyState);
        psm->service.leRemoteBusy(chan->lcid, chan->busyState, psm->ctx);
        LOG_DEBUG("L2capCallback leRemoteBusy:%{public}d end", __LINE__);
    }
}

static void L2capLeTxWithCredit(const L2capLeConnection *conn, L2capLeChannel *chan)
{
    ListNode *node = NULL;
    Packet *pkt = NULL;

    // txList is created lazily on the first send (L2capLeSegmentPacketWithCredit), so a channel
    // that never sent (e.g. it receives 0x16 credits right after connection) has no list at all.
    if (chan->txList == NULL) {
        return;
    }

    while (1) {
        node = ListGetFirstNode(chan->txList);
        if (node == NULL) {
            break;
        }
        pkt = ListGetNodeData(node);
        /*
         * chapter 4.24: one credit is consumed per SDU, not per K-frame. The gate and the decrement
         * apply only when the K-frame starts a new SDU; the continuation K-frames of a started SDU
         * pass unconditionally so an SDU is never left half-sent on a zero credit
         */
        if (L2capLeIsFirstSduFrame(pkt)) {
            if (chan->rcfg.credit == 0) {
                break;
            }
            chan->rcfg.credit -= 1;
        }
        ListRemoveNode(chan->txList, pkt);
        L2capLeSendPacket(conn->aclHandle, pkt);
    }

    if (chan->rcfg.credit == 0) {
        L2capLeTxNotifyBusyState(chan, L2CAP_LE_CHANNEL_CREDIT_FULL);
    } else {
        L2capLeTxNotifyBusyState(chan, L2CAP_LE_CHANNEL_CREDIT_NOT_FULL);
    }
}

static void L2capLeSegmentPacketWithCredit(const L2capLeChannel *chan, Packet *pkt)
{
    Packet *frag = NULL;
    Packet *fragPkt = NULL;
    uint16_t totalLength;
    uint8_t *header = NULL;
    uint16_t headerLength;

    totalLength = PacketSize(pkt);
    headerLength = L2CAP_HEADER_LENGTH + L2CAP_SIZE_2;
    while (1) {
        frag = PacketMalloc(0, 0, 0);
        int remainLength = PacketFragment(pkt, frag, chan->rcfg.mps);

        fragPkt = PacketInheritMalloc(frag, headerLength, 0);
        PacketFree(frag);

        header = BufferPtr(PacketHead(fragPkt));
        L2capCpuToLe16(header + 0, PacketSize(fragPkt) - L2CAP_HEADER_LENGTH);
        L2capCpuToLe16(header + L2CAP_OFFSET_2, chan->rcid);

        if (headerLength > L2CAP_HEADER_LENGTH) {
            L2capCpuToLe16(header + L2CAP_OFFSET_4, totalLength);
        }

        ListAddLast(chan->txList, fragPkt);
        if (remainLength == 0) {
            break;
        }

        headerLength = L2CAP_HEADER_LENGTH;
    }

    return;
}

static void L2capLeProcessConnectionParameterUpdateReq(
    uint16_t aclHandle, L2capSignalHeader *signal, const uint8_t *data)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    L2capLeConnection *conn = NULL;
    L2capLeConnectionParameter param = {0};

    conn = L2capLeGetConnection(aclHandle);
    if (conn == NULL) {
        return;
    }

    if (conn->role == L2CAP_LE_ROLE_SLAVE) {
        L2capSendCommandReject(
            aclHandle, L2CAP_LE_SIGNALING_CHANNEL, signal->identifier, L2CAP_COMMAND_NOT_UNDERSTOOD, NULL);
        return;
    }

    param.connIntervalMin = L2capLe16ToCpu(data + 0);
    param.connIntervalMax = L2capLe16ToCpu(data + L2CAP_OFFSET_2);
    param.connLatency = L2capLe16ToCpu(data + L2CAP_OFFSET_4);
    param.supervisionTimeout = L2capLe16ToCpu(data + L2CAP_OFFSET_6);

    if (inst->connParamUpdate.cb.recvLeConnectionParameterUpdateReq != NULL) {
        LOG_DEBUG("L2capCallback recvLeConnectionParameterUpdateReq:%{public}d begin", __LINE__);
        inst->connParamUpdate.cb.recvLeConnectionParameterUpdateReq(
            aclHandle, signal->identifier, &param, inst->connParamUpdate.ctx);
        LOG_DEBUG("L2capCallback recvLeConnectionParameterUpdateReq:%{public}d end", __LINE__);
    }

    return;
}

static void L2capLeProcessConnectionParameterUpdateRsp(
    uint16_t aclHandle, L2capSignalHeader *signal, const uint8_t *data)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    L2capLeConnection *conn = NULL;
    L2capPendingRequest *pending = NULL;
    uint16_t result;

    conn = L2capLeGetConnection(aclHandle);
    if (conn == NULL) {
        return;
    }

    // Code-filtered destroy: identifiers are shared across signal types on this
    // connection, a pending entry of another type (e.g. an EATT 0x17 batch) that
    // reused the identifier must survive.
    pending = L2capGetPendingRequest(conn->pendingList, signal->identifier);
    if ((pending != NULL) && (pending->code == L2CAP_CONNECTION_PARAMETER_UPDATE_REQUEST)) {
        L2capDestroyPendingRequest(conn->pendingList, signal->identifier);
    }
    result = L2capLe16ToCpu(data + 0);

    if (inst->connParamUpdate.cb.recvLeConnectionParameterUpdateRsp != NULL) {
        LOG_DEBUG("L2capCallback recvLeConnectionParameterUpdateRsp:%{public}d begin", __LINE__);
        inst->connParamUpdate.cb.recvLeConnectionParameterUpdateRsp(aclHandle, result, inst->connParamUpdate.ctx);
        LOG_DEBUG("L2capCallback recvLeConnectionParameterUpdateRsp:%{public}d end", __LINE__);
    }

    return;
}

// Resolve the peer's 0x14 with a rejection when the PSM is not registered or a channel
// could not be allocated (OOM): leaving it unanswered would make the client retry after
// its own transaction timeout, chapter 4.24.
static void L2capLeRejectCreditBasedConnectionReq(
    L2capLeConnection *conn, const L2capSignalHeader *signal, const L2capLeConfigInfo *cfg, uint16_t result)
{
    L2capLeChannel tchan = {0};

    tchan.lcid = 0;
    (void)memcpy_s(&(tchan.lcfg), sizeof(L2capLeConfigInfo), cfg, sizeof(L2capLeConfigInfo));
    L2capSendCreditBasedConnectionRsp(conn, &tchan, signal->identifier, result);
}

// Notify the upper layer of an inbound LE Credit Based Connection Request (0x14): the
// connection info is built from the connection and forwarded together with the remote
// configuration the responder has negotiated. Runs on the L2CAP processing queue, the
// same thread that executes the rest of the connection's L2CAP logic.
static void L2capLeNotifyCreditBasedConnectionReq(
    L2capLeChannel *chan, const L2capSignalHeader *signal, const L2capLeConnection *conn,
    const L2capLeConfigInfo *cfg, const L2capLePsm *psm)
{
    L2capConnectionInfo connInfo = {0};

    if (psm->service.recvLeCreditBasedConnectionReq == NULL) {
        return;
    }

    LOG_DEBUG("L2capCallback recvLeCreditBasedConnectionReq:%{public}d begin, aclHandle = %hu, cid = 0x%04X, id = "
              "%hhu, lpsm = %hu",
        __LINE__, conn->aclHandle, chan->lcid, signal->identifier, psm->lpsm);

    connInfo.handle = conn->aclHandle;
    (void)memcpy_s(&(connInfo.addr), sizeof(BtAddr), &(conn->addr), sizeof(BtAddr));
    psm->service.recvLeCreditBasedConnectionReq(chan->lcid, signal->identifier, &connInfo, cfg, psm->ctx);
    LOG_DEBUG("L2capCallback recvLeCreditBasedConnectionReq:%{public}d end", __LINE__);
}

static void L2capLeProcessCreditBasedConnectionReq(
    uint16_t aclHandle, const L2capSignalHeader *signal, const uint8_t *data)
{
    L2capLeConnection *conn = NULL;
    L2capLeChannel *chan = NULL;
    L2capLePsm *psm = NULL;
    uint16_t lpsm;
    uint16_t rcid;
    L2capLeConfigInfo cfg = {0};

    conn = L2capLeGetConnection(aclHandle);
    if (conn == NULL) {
        return;
    }

    lpsm = L2capLe16ToCpu(data + 0);
    rcid = L2capLe16ToCpu(data + L2CAP_OFFSET_2);
    cfg.mtu = L2capLe16ToCpu(data + L2CAP_OFFSET_4);
    cfg.mps = L2capLe16ToCpu(data + L2CAP_OFFSET_6);
    cfg.credit = L2capLe16ToCpu(data + L2CAP_OFFSET_8);

    psm = L2capLeGetPsm(lpsm);
    if (psm == NULL) {
        L2capLeRejectCreditBasedConnectionReq(conn, signal, &cfg, L2CAP_LE_PSM_NOT_SUPPORTED);
        return;
    }

    chan = L2capLeNewChannel(conn, lpsm, lpsm);
    if (chan == NULL) {
        // OOM: resolve the peer's 0x14 with NO_RESOURCES instead of leaving it unanswered
        // (the client retries after its own transaction timeout, chapter 4.24)
        L2capLeRejectCreditBasedConnectionReq(conn, signal, &cfg, L2CAP_LE_NO_RESOURCES_AVAILABLE);
        return;
    }
    chan->rcid = rcid;
    chan->connIdentifier = signal->identifier;

    chan->rcfg.mtu = cfg.mtu;
    chan->rcfg.mps = cfg.mps;
    chan->rcfg.credit = cfg.credit;
    if (chan->rcfg.mps > (L2capLeGetTxBufferSize() - L2CAP_SIZE_6)) {
        chan->rcfg.mps = L2capLeGetTxBufferSize() - L2CAP_SIZE_6;
    }

    chan->state = L2CAP_CHANNEL_CONNECT_IN_REQ;

    L2capLeNotifyCreditBasedConnectionReq(chan, signal, conn, &cfg, psm);

    return;
}

static void L2capLeProcessCreditBasedConnectionRsp(
    uint16_t aclHandle, const L2capSignalHeader *signal, const uint8_t *data)
{
    L2capLeConnection *conn = NULL;
    L2capLeChannel *chan = NULL;
    L2capLePsm *psm = NULL;
    uint16_t lcid = 0;
    uint16_t rcid;
    uint16_t result;
    L2capLeConfigInfo cfg = {0};
    L2capConnectionInfo connInfo = {0};
    L2capPendingRequest *pending = NULL;

    conn = L2capLeGetConnection(aclHandle);
    if (conn == NULL) {
        return;
    }

    // Only the pending entry of our own 0x14 request may be removed: identifiers are
    // shared with EATT 0x17 batches (L2capLeGetNewIdentifier), so a hostile 0x15 with a
    // reused identifier must not tear down a pending 0x17 batch and its RTX timer.
    pending = L2capGetPendingRequest(conn->pendingList, signal->identifier);
    if ((pending != NULL) && (pending->code == L2CAP_LE_CREDIT_BASED_CONNECTION_REQUEST)) {
        L2capDestroyPendingRequest(conn->pendingList, signal->identifier);
    }

    rcid = L2capLe16ToCpu(data + 0);
    cfg.mtu = L2capLe16ToCpu(data + L2CAP_OFFSET_2);
    cfg.mps = L2capLe16ToCpu(data + L2CAP_OFFSET_4);
    cfg.credit = L2capLe16ToCpu(data + L2CAP_OFFSET_6);
    result = L2capLe16ToCpu(data + L2CAP_OFFSET_8);

    chan = L2capLeGetChannel4(conn, signal->identifier);
    if (chan == NULL) {
        return;
    }

    chan->rcid = rcid;
    lcid = chan->lcid;

    chan->rcfg.mtu = cfg.mtu;
    chan->rcfg.mps = cfg.mps;
    chan->rcfg.credit = cfg.credit;
    if (chan->rcfg.mps > (L2capLeGetTxBufferSize() - L2CAP_SIZE_6)) {
        chan->rcfg.mps = L2capLeGetTxBufferSize() - L2CAP_SIZE_6;
    }

    psm = L2capLeGetPsm(chan->lpsm);

    if (result == L2CAP_LE_CONNECTION_SUCCESSFUL) {
        chan->state = L2CAP_CHANNEL_CONNECTED;
    } else {
        L2capLeDeleteChannel(conn, chan, 1);
    }

    if ((psm != NULL) && (psm->service.recvLeCreditBasedConnectionRsp != NULL)) {
        LOG_DEBUG("L2capCallback recvLeCreditBasedConnectionRsp:begin, cid = 0x%04X, result = %hu", lcid, result);

        connInfo.handle = aclHandle;
        (void)memcpy_s(&(connInfo.addr), sizeof(BtAddr), &(conn->addr), sizeof(BtAddr));
        psm->service.recvLeCreditBasedConnectionRsp(lcid, &connInfo, &cfg, result, psm->ctx);
        LOG_DEBUG("L2capCallback recvLeCreditBasedConnectionRsp:%{public}d end", __LINE__);
    }

    return;
}

static void L2capLeProcessDisconnectionReq(uint16_t aclHandle, const L2capSignalHeader *signal, const uint8_t *data)
{
    L2capLeConnection *conn = NULL;
    L2capLeChannel *chan = NULL;
    uint16_t lcid;
    uint16_t rcid;
    L2capLePsm *psm = NULL;

    conn = L2capLeGetConnection(aclHandle);
    if (conn == NULL) {
        return;
    }

    lcid = L2capLe16ToCpu(data + 0);
    rcid = L2capLe16ToCpu(data + L2CAP_OFFSET_2);

    chan = L2capLeGetChannel(conn, lcid);
    if (chan == NULL) {
        uint16_t rejCid[L2CAP_SIZE_2] = {lcid, rcid};

        L2capSendCommandReject(
            aclHandle, L2CAP_LE_SIGNALING_CHANNEL, signal->identifier, L2CAP_INVALID_CID_IN_REQUEST, rejCid);
        return;
    }

    // this case is for both side call disconnect at same time
    if (chan->state == L2CAP_CHANNEL_DISCONNECT_OUT_REQ) {
        L2capLeSendDisconnectionRsp(conn, chan, signal->identifier);
        return;
    }

    chan->state = L2CAP_CHANNEL_DISCONNECT_IN_REQ;

    psm = L2capLeGetPsm(chan->lpsm);
    if ((psm != NULL) && (psm->service.recvLeDisconnectionReq != NULL)) {
        LOG_DEBUG("L2capCallback recvLeDisconnectionReq:%{public}d begin, cid = 0x%04X, id = %hhu",
            __LINE__,
            lcid,
            signal->identifier);
        psm->service.recvLeDisconnectionReq(lcid, signal->identifier, psm->ctx);
        LOG_DEBUG("L2capCallback recvLeDisconnectionReq:%{public}d end", __LINE__);
    }

    return;
}

static void L2capLeProcessDisconnectionRsp(uint16_t aclHandle, const L2capSignalHeader *signal, const uint8_t *data)
{
    L2capLeConnection *conn = NULL;
    L2capLeChannel *chan = NULL;
    L2capLePsm *psm = NULL;
    L2capPendingRequest *pending = NULL;
    uint16_t lcid;
    uint16_t rcid;

    conn = L2capLeGetConnection(aclHandle);
    if (conn == NULL) {
        return;
    }

    // Code-filtered destroy: identifiers are shared across signal types on this
    // connection, a pending entry of another type (e.g. an EATT 0x17 batch) that
    // reused the identifier must survive.
    pending = L2capGetPendingRequest(conn->pendingList, signal->identifier);
    if ((pending != NULL) && (pending->code == L2CAP_DISCONNECTION_REQUEST)) {
        L2capDestroyPendingRequest(conn->pendingList, signal->identifier);
    }

    rcid = L2capLe16ToCpu(data + 0);
    lcid = L2capLe16ToCpu(data + L2CAP_OFFSET_2);

    chan = L2capLeGetChannel(conn, lcid);
    if (chan == NULL) {
        return;
    }

    if (chan->rcid != rcid) {
        return;
    }

    psm = L2capLeGetPsm(chan->lpsm);

    // EATT rides the ACL that hosts the UATT fixed channel (not in chanList), so the last EATT close
    // must not drop the ACL (Vol 3 Part G 6.1.1); keep the link while the UATT is up.
    L2capLeDeleteChannel(conn, chan, (chan->lpsm == L2CAP_LE_EATT_PSM) ? 0 : 1);

    if ((psm != NULL) && (psm->service.recvLeDisconnectionRsp != NULL)) {
        LOG_DEBUG("L2capCallback recvLeDisconnectionRsp:%{public}d begin, cid = 0x%04X", __LINE__, lcid);
        psm->service.recvLeDisconnectionRsp(lcid, psm->ctx);
        LOG_DEBUG("L2capCallback recvLeDisconnectionRsp:%{public}d end", __LINE__);
    }

    return;
}

static void L2capLeProcessFlowControlCredit(uint16_t aclHandle, const L2capSignalHeader *signal, const uint8_t *data)
{
    L2capLeConnection *conn = NULL;
    L2capLeChannel *chan = NULL;
    uint16_t rcid;
    uint16_t credit;

    conn = L2capLeGetConnection(aclHandle);
    if (conn == NULL) {
        return;
    }

    /*
     * chapter 4.24: the Credit Based Flow Control Credit signal carries exactly one CID and one
     * credit field (4 octets); a shorter signal is a malformed command and is rejected
     */
    if (signal->length != L2CAP_SIZE_4) {
        L2capSendCommandReject(
            aclHandle, L2CAP_LE_SIGNALING_CHANNEL, signal->identifier, L2CAP_COMMAND_NOT_UNDERSTOOD, NULL);
        return;
    }

    /*
     * chapter 4.24: the 0x16 CID field is the peer CID mapping to our rcid; a lookup by our lcid only
     * works while the CID sequences stay symmetric (rcfg.credit exhaustion once they diverge)
     */
    rcid = L2capLe16ToCpu(data + 0);
    credit = L2capLe16ToCpu(data + L2CAP_OFFSET_2);
    /*
     * chapter 4.24: a credit of 0 violates the 1-65535 range and has no effect on the credit pool,
     * so the packet is ignored
     */
    if (credit == 0) {
        return;
    }

    chan = L2capLeEattGetChannelByRcid(conn, rcid);
    if (chan == NULL) {
        return;
    }

    if (chan->rcfg.credit > (UINT16_MAX - credit)) {
        L2capLeSendDisconnectionReq(conn, chan);
        return;
    }

    chan->rcfg.credit += credit;

    L2capLeTxWithCredit(conn, chan);
    return;
}

static void L2capLeSignal(uint16_t aclHandle, L2capSignalHeader *signal, const uint8_t *data)
{
    switch (signal->code) {
        case L2CAP_LE_CREDIT_BASED_CONNECTION_REQUEST:
            L2capLeProcessCreditBasedConnectionReq(aclHandle, signal, data);
            break;
        case L2CAP_LE_CREDIT_BASED_CONNECTION_RESPONSE:
            L2capLeProcessCreditBasedConnectionRsp(aclHandle, signal, data);
            break;
        case L2CAP_DISCONNECTION_REQUEST:
            L2capLeProcessDisconnectionReq(aclHandle, signal, data);
            break;
        case L2CAP_DISCONNECTION_RESPONSE:
            L2capLeProcessDisconnectionRsp(aclHandle, signal, data);
            break;
        case L2CAP_LE_FLOW_CONTROL_CREDIT:
            L2capLeProcessFlowControlCredit(aclHandle, signal, data);
            break;
        case L2CAP_CREDIT_BASED_CONNECTION_REQUEST:
            L2capLeEattProcessConnectionReq(aclHandle, signal, data);
            break;
        case L2CAP_CREDIT_BASED_CONNECTION_RESPONSE:
            L2capLeEattProcessConnectionRsp(aclHandle, signal, data);
            break;
        case L2CAP_CREDIT_BASED_RECONFIGURE_REQUEST:
            L2capLeEattProcessReconfigureReq(aclHandle, signal, data);
            break;
        case L2CAP_CREDIT_BASED_RECONFIGURE_RESPONSE:
            L2capLeEattProcessReconfigureRsp(aclHandle, signal, data);
            break;
        case L2CAP_CONNECTION_PARAMETER_UPDATE_REQUEST:
            L2capLeProcessConnectionParameterUpdateReq(aclHandle, signal, data);
            break;
        case L2CAP_CONNECTION_PARAMETER_UPDATE_RESPONSE:
            L2capLeProcessConnectionParameterUpdateRsp(aclHandle, signal, data);
            break;
        case L2CAP_COMMAND_REJECT:
            break;
        default:
            L2capSendCommandReject(
                aclHandle, L2CAP_LE_SIGNALING_CHANNEL, signal->identifier, L2CAP_COMMAND_NOT_UNDERSTOOD, NULL);
            break;
    }
    return;
}

static void L2capLeProcessSignal(uint16_t aclHandle, const Packet *pkt)
{
    uint8_t buff[L2CAP_SIGNAL_MTU] = {0};
    uint16_t length;
    L2capSignalHeader signal = {0};

    length = PacketSize(pkt);
    if (length > L2CAP_SIGNAL_MTU) {
        PacketRead(pkt, buff, 0, L2CAP_SIGNAL_HEADER_LENGTH);
        L2capSendCommandReject(aclHandle, L2CAP_LE_SIGNALING_CHANNEL, buff[1], L2CAP_SIGNAL_MTU_EXCEEDED, NULL);
        return;
    }

    if (length < L2CAP_SIGNAL_HEADER_LENGTH) {
        PacketRead(pkt, buff, 0, L2CAP_SIGNAL_HEADER_LENGTH);
        L2capSendCommandReject(aclHandle, L2CAP_LE_SIGNALING_CHANNEL, buff[1], L2CAP_COMMAND_NOT_UNDERSTOOD, NULL);
        return;
    }

    PacketRead(pkt, buff, 0, length);
    signal.code = buff[0];
    signal.identifier = buff[1];
    signal.length = L2capLe16ToCpu(buff + L2CAP_OFFSET_2);

    if (signal.length != (length - L2CAP_SIGNAL_HEADER_LENGTH)) {
        L2capSendCommandReject(
            aclHandle, L2CAP_LE_SIGNALING_CHANNEL, signal.identifier, L2CAP_COMMAND_NOT_UNDERSTOOD, NULL);
        return;
    }

    L2capLeSignal(aclHandle, &signal, buff + L2CAP_SIGNAL_HEADER_LENGTH);
    return;
}

static void L2capLeProcessFixChannelData(uint16_t aclHandle, uint16_t cid, const Packet *pkt)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    L2capLeConnection *conn = NULL;

    conn = L2capLeGetConnection(aclHandle);
    if (conn == NULL) {
        LOG_ERROR("L2cap fix channel recvData but connection not found");
        return;
    }

    LOG_DEBUG("L2capCallback fix channel recvData:%{public}d begin, cid = 0x%04X", __LINE__, cid);
    if (cid == L2CAP_LE_ATTRIBUTE_PROTOCOL) {
        if (inst->chanAtt.recvLeData != NULL) {
            inst->chanAtt.recvLeData(aclHandle, pkt);
        }
    } else if (cid == L2CAP_LE_SECURITY_MANAGER_PROTOCOL) {
        if (inst->chanSm.recvLeData != NULL) {
            inst->chanSm.recvLeData(aclHandle, pkt);
        }
    }
    LOG_DEBUG("L2capCallback fix channel recvData:%{public}d end", __LINE__);

    return;
}

static void L2capLeDataCallback(const L2capLeChannel *chan, Packet *pkt)
{
    L2capLePsm *psm = NULL;

    psm = L2capLeGetPsm(chan->lpsm);
    if (psm == NULL) {
        return;
    }

    if (psm->service.recvLeData != NULL) {
        LOG_DEBUG("L2capCallback recvLeData:%{public}d begin, cid = 0x%04X", __LINE__, chan->lcid);
        psm->service.recvLeData(chan->lcid, pkt, psm->ctx);
        LOG_DEBUG("L2capCallback recvLeData:%{public}d end", __LINE__);
    }
    return;
}

// chapter 3.4.3 violation in the data plane: drop the partial SDU and disconnect the channel.
// The current K-frame pkt is owned by the caller (l2cap_cmn.c) and is not freed here.
// No credit is refunded for the rejected data; the channel is released when the peer's
// 0x15 Disconnection Response arrives or the RTX timer fires, not here.
static void L2capLeDisconnectOnDataViolation(L2capLeConnection *conn, L2capLeChannel *chan)
{
    if (chan->rxSarPacket != NULL) {
        PacketFree(chan->rxSarPacket);
        chan->rxSarPacket = NULL;
    }
    chan->state = L2CAP_CHANNEL_DISCONNECT_OUT_REQ;
    L2capLeSendDisconnectionReq(conn, chan);
    return;
}

// chapter 3.4.3: append a continuation K-frame to the SAR buffer; returns the completed SDU when the
// payload sum reaches the declared SDU length (owned by the caller), NULL while still incomplete. The
// SDU length and MPS violations disconnect the channel per chapter 3.4.3
static Packet *L2capLeProcessSarContinuation(L2capLeConnection *conn, L2capLeChannel *chan, Packet *pkt)
{
    uint8_t header[L2CAP_SIZE_2] = {0};
    uint16_t length;
    Packet *sdu;

    if (PacketSize(pkt) > chan->lcfg.mps) {
        L2capLeDisconnectOnDataViolation(conn, chan);
        return NULL;
    }
    PacketAssemble(chan->rxSarPacket, pkt);
    PacketRead(chan->rxSarPacket, header, 0, sizeof(header));
    length = L2capLe16ToCpu(header + 0);
    if (PacketSize(chan->rxSarPacket) > (size_t)(length + L2CAP_SIZE_2)) {
        L2capLeDisconnectOnDataViolation(conn, chan);
        return NULL;
    }
    if (PacketSize(chan->rxSarPacket) < (size_t)(length + L2CAP_SIZE_2)) {
        return NULL;
    }
    sdu = chan->rxSarPacket;
    PacketExtractHead(sdu, header, sizeof(header));
    // detach before the callback: a teardown inside recvLeData would double-free otherwise
    chan->rxSarPacket = NULL;
    return sdu;
}

// chapter 3.4.2/3.4.3: the first K-frame of the SDU carries the SDU length field; a complete SDU is
// delivered immediately, a short one starts the SAR buffer with a reference of the K-frame
static void L2capLeProcessFirstSduFrame(L2capLeConnection *conn, L2capLeChannel *chan, Packet *pkt)
{
    uint8_t header[L2CAP_SIZE_2] = {0};
    uint16_t length;

    if (PacketSize(pkt) < L2CAP_SIZE_2) {
        /* chapter 3.4.2: the first K-frame of the SDU shall contain the L2CAP SDU Length field */
        L2capLeDisconnectOnDataViolation(conn, chan);
        return;
    }
    PacketRead(pkt, header, 0, sizeof(header));
    length = L2capLe16ToCpu(header + 0);
    if (length > chan->lcfg.mtu ||
        PacketSize(pkt) > (size_t)(chan->lcfg.mps + L2CAP_SIZE_2) ||
        PacketSize(pkt) > (size_t)(length + L2CAP_SIZE_2)) {
        /*
         * chapter 3.4.3: "If the SDU length field value exceeds the receiver's MTU, the receiver
         * shall disconnect the channel." The MPS and payload-sum violations disconnect the same way
         */
        L2capLeDisconnectOnDataViolation(conn, chan);
        return;
    }
    if (PacketSize(pkt) == (size_t)(length + L2CAP_SIZE_2)) {
        PacketExtractHead(pkt, header, sizeof(header));
        L2capLeDataCallback(chan, pkt);
        return;
    }
    chan->rxSarPacket = PacketRefMalloc(pkt);
    return;
}

static void L2capLeProcessLeData(uint16_t aclHandle, uint16_t cid, Packet *pkt)
{
    L2capLeConnection *conn = NULL;
    L2capLeChannel *chan = NULL;
    Packet *sdu = NULL;

    L2capLeGetChannel3(aclHandle, cid, &conn, &chan);
    if ((chan == NULL) || (chan->state != L2CAP_CHANNEL_CONNECTED)) {
        return;
    }

    /*
     * chapter 4.24: one credit is consumed per SDU, not per K-frame. The zero-credit gate and the
     * decrement apply only when the K-frame starts a new SDU (no SAR in progress); the continuation
     * K-frames of a started SDU neither consume a credit nor can trip the gate
     */
    if (chan->rxSarPacket == NULL) {
        if (chan->peerCredits == 0) {
            L2capLeDisconnectOnDataViolation(conn, chan);
            return;
        }
        chan->peerCredits--;
    }

    if (chan->rxSarPacket != NULL) {
        sdu = L2capLeProcessSarContinuation(conn, chan, pkt);
        if (sdu != NULL) {
            L2capLeDataCallback(chan, sdu);
            PacketFree(sdu);
        }
    } else {
        L2capLeProcessFirstSduFrame(conn, chan, pkt);
    }

    // the recvLeData callback may have torn the channel down; verify it is still registered
    if (L2capLeGetChannel(conn, cid) != chan) {
        return;
    }

    // chapter 3.4.3 violation paths only move the channel to DISCONNECT_OUT_REQ without
    // removing it; no credit is refunded for the rejected data (L2capLeDisconnectOnDataViolation)
    if (chan->state != L2CAP_CHANNEL_CONNECTED) {
        return;
    }

    /*
     * chapter 4.24: refund one credit (0x16) per completed SDU and replenish the peer's budget.
     * rxSarPacket is non-NULL while the SDU is still being reassembled and is cleared only when the
     * SDU completed on this K-frame; a failed 0x16 send is not refunded
     */
    if (chan->rxSarPacket == NULL) {
        if (L2capLeSendFlowControlCredit(conn, chan, 1) == BT_SUCCESS) {
            if (chan->peerCredits < UINT16_MAX) {
                chan->peerCredits++;
            }
        }
    }
    return;
}

static int L2capLeReceivePacket(uint16_t handle, uint16_t cid, Packet *pkt)
{
    uint8_t header[4] = {0};

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    PacketExtractHead(pkt, header, sizeof(header));
    switch (cid) {
        case L2CAP_LE_SIGNALING_CHANNEL:
            L2capLeProcessSignal(handle, pkt);
            break;
        case L2CAP_LE_ATTRIBUTE_PROTOCOL:  // dummy
        case L2CAP_LE_SECURITY_MANAGER_PROTOCOL:
            L2capLeProcessFixChannelData(handle, cid, pkt);
            break;
        default:
            L2capLeProcessLeData(handle, cid, pkt);
            break;
    }

    return 0;
}

static void L2capLeCleanAllChannels(L2capLeConnection *conn, uint8_t status)
{
    L2capLeChannel *chan = NULL;
    L2capLePsm *psm = NULL;
    ListNode *node = NULL;

    while (1) {
        node = ListGetFirstNode(conn->chanList);
        if (node == NULL) {
            break;
        }

        chan = ListGetNodeData(node);

        psm = L2capLeGetPsm(chan->lpsm);
        if ((psm != NULL) && (psm->service.leDisconnectAbnormal != NULL)) {
            LOG_DEBUG("L2capCallback leDisconnectAbnormal:%{public}d begin, cid = 0x%04X, reason = %hhu",
                __LINE__,
                chan->lcid,
                status);
            psm->service.leDisconnectAbnormal(chan->lcid, status, psm->ctx);
            LOG_DEBUG("L2capCallback leDisconnectAbnormal:%{public}d end", __LINE__);
        }

        ListRemoveNode(conn->chanList, chan);
        L2capLeDestroyChannel(chan);
    }

    return;
}

static void L2capLeAclDisconnectProcess(L2capLeConnection *conn, uint8_t status, uint8_t reason)
{
    L2capLeInstance *inst = &g_l2capLeInst;

    LOG_DEBUG("L2capCallback leDisconnected:%{public}d begin, aclHandle = 0x%04X, status = %hhu, reason = %hhu",
        __LINE__,
        conn->aclHandle,
        status,
        reason);
    if (inst->chanAtt.leDisconnected != NULL) {
        inst->chanAtt.leDisconnected(conn->aclHandle, status, reason);
    }

    if (inst->chanSm.leDisconnected != NULL) {
        inst->chanSm.leDisconnected(conn->aclHandle, status, reason);
    }
    LOG_DEBUG("L2capCallback leDisconnected:%{public}d end", __LINE__);

    if (status == 0) {
        L2capLeCleanAllChannels(conn, status);
    }
    return;
}

static void L2capLeAclConnectProcess(L2capLeConnection *conn)
{
    L2capLeChannel *chan = NULL;
    ListNode *node = NULL;

    node = ListGetFirstNode(conn->chanList);
    while (node != NULL) {
        chan = ListGetNodeData(node);
        if ((chan->state == L2CAP_CHANNEL_CONNECT_OUT_REQ) && (chan->lpsm != L2CAP_LE_EATT_PSM)) {
            L2capSendCreditBasedConnectionReq(conn, chan);
        }

        node = ListGetNextNode(node);
    }

    // all pending EATT channels are sent in one 0x17
    int eattResult = L2capLeEattSendPendingRequest(conn);
    if (eattResult != BT_SUCCESS) {
        // The batch was rolled back inside (channels deleted and reported via leDisconnectAbnormal),
        // but the ATT establishment callback was already notified with BT_SUCCESS at dispatch time:
        // log loudly instead of silently dropping the deferred batch
        LOG_ERROR("%{public}s: deferred 0x17 send failed, result = %{public}d, EATT batch rolled back",
            __FUNCTION__, eattResult);
    }
    return;
}

// Vol 3 Part G 5.4: the granted values stashed from LE Connection Complete win (the actual
// negotiated parameters); without them the slave retry delay would use the 100 ms floor. The
// stash is matched per handle in the slot array, so the parameters of a connection completing
// back-to-back with another one are not misapplied. Returns true when the slot was found.
static bool L2capLeApplyGrantedConnParams(L2capLeConnection *conn, uint16_t handle)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    for (uint8_t i = 0; i < L2CAP_LE_GRANTED_CONN_PARAM_MAX; i++) {
        if (inst->grantedConnParams[i].valid && (inst->grantedConnParams[i].handle == handle)) {
            conn->connIntervalUnits = inst->grantedConnParams[i].interval;
            conn->connSlaveLatency = inst->grantedConnParams[i].latency;
            inst->grantedConnParams[i].valid = 0;
            return true;
        }
    }
    return false;
}

// Vol 3 Part G 5.4: apply the connection parameters requested by L2CAP_LeConnect,
// matched per address in the slot array.
static void L2capLeApplyPendingConnParams(L2capLeConnection *conn, const BtAddr *addr)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    for (uint8_t i = 0; i < L2CAP_LE_GRANTED_CONN_PARAM_MAX; i++) {
        if (inst->pendingConnParams[i].valid &&
            (memcmp(&(inst->pendingConnParams[i].addr), addr, sizeof(BtAddr)) == 0)) {
            conn->connIntervalUnits = inst->pendingConnParams[i].connIntervalMax;
            conn->connSlaveLatency = inst->pendingConnParams[i].connLatency;
            inst->pendingConnParams[i].valid = 0;
            break;
        }
    }
}

static int L2capLeConnectComplete(const BtAddr *addr, uint16_t handle, uint8_t role, uint8_t status)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    L2capLeConnection *conn = NULL;

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    LOG_DEBUG("L2capCallback leConnected:%{public}d begin, aclHandle = 0x%04X, role = %hhu, status = %hhu",
        __LINE__,
        handle,
        role,
        status);

    if (inst->chanAtt.leConnected != NULL) {
        inst->chanAtt.leConnected(addr, handle, role, status);
    }
    if (inst->chanSm.leConnected != NULL) {
        inst->chanSm.leConnected(addr, handle, role, status);
    }
    LOG_DEBUG("L2capCallback leConnected:%{public}d end", __LINE__);

    if (status != 0) {
        conn = L2capLeGetConnection2(addr);
        if (conn != NULL) {
            L2capLeCleanAllChannels(conn, status);
            L2capLeDeleteConnection(conn);
        }
        // The grantedConnParams slot stashed by L2capLeRecvConnectionComplete is deliberately left
        // unconsumed on failure: the connection is typically retried with the same controller handle,
        // and the retry's own successful completion consumes the slot then. When repeated failures
        // exhaust all L2CAP_LE_GRANTED_CONN_PARAM_MAX slots, the stash cannot grow and the retry
        // delay of Vol 3 Part G 5.4 degrades to its 100 ms floor (connIntervalUnits stays 0) - a
        // minor timing change only, no protocol or safety impact, hence not worth evicting entries
        // here.
        return BT_BAD_STATUS;
    }

    conn = L2capLeGetConnection2(addr);
    if (conn == NULL) {
        conn = L2capLeNewConnection(addr, handle, role);
        if (conn == NULL) {
            return BT_BAD_STATUS;
        }
    }

    conn->aclHandle = handle;
    conn->role = role;
    if (!L2capLeApplyGrantedConnParams(conn, handle)) {
        L2capLeApplyPendingConnParams(conn, addr);
    }
    L2capAddConnectionRef(handle);
    L2capLeAclConnectProcess(conn);

    return BT_SUCCESS;
}

static int L2capLeDisconnectComplete(uint16_t handle, uint8_t status, uint8_t reason)
{
    L2capLeConnection *conn = NULL;

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    conn = L2capLeGetConnection(handle);
    if (conn == NULL) {
        LOG_ERROR("L2capLeDisconnectComplete but connection not found");
        return BT_BAD_PARAM;
    }

    L2capLeAclDisconnectProcess(conn, status, reason);

    if (status == 0) {
        L2capLeDeleteConnection(conn);
    }

    return BT_SUCCESS;
}

static int L2capLeStartCreditBasedConnect(const BtAddr *addr, L2capLeConnection *conn, L2capLeChannel *chan)
{
    if (conn->aclHandle == 0) {
        int result;

        result = L2capConnectLe(addr);
        if (result != BT_SUCCESS) {
            LOG_ERROR("%{public}s: L2capConnectLe failed, result = %{public}d.", __FUNCTION__, result);
            L2capLeDeleteConnection(conn);
        }

        return result;
    }

    L2capSendCreditBasedConnectionReq(conn, chan);
    return BT_SUCCESS;
}

static L2capLeChannel *L2capLeCreateChannelForConnectReq(
    L2capLeConnection *conn, uint16_t lpsm, uint16_t rpsm, const L2capLeConfigInfo *cfg, uint16_t *lcid)
{
    L2capLeChannel *chan = L2capLeNewChannel(conn, lpsm, rpsm);
    if (chan == NULL) {
        LOG_ERROR("%{public}s: L2capLeNewChannel failed", __FUNCTION__);
        return NULL;
    }
    chan->state = L2CAP_CHANNEL_CONNECT_OUT_REQ;

    chan->lcfg.mtu = cfg->mtu;
    if (cfg->credit != 0) {
        chan->lcfg.credit = cfg->credit;
    }
    // peerCredits must track the granted receive credit, read by the zero-credit gate
    chan->peerCredits = chan->lcfg.credit;
    if (chan->lcfg.mps > cfg->mtu) {
        chan->lcfg.mps = cfg->mtu;
    }

    *lcid = chan->lcid;
    return chan;
}

int L2CAP_LeCreditBasedConnectionReq(
    const BtAddr *addr, uint16_t lpsm, uint16_t rpsm, const L2capLeConfigInfo *cfg, uint16_t *lcid)
{
    L2capLeConnection *conn = NULL;
    L2capLeChannel *chan = NULL;
    L2capLePsm *psm = NULL;
    bool isNewConn = false;

    LOG_INFO("%{public}s:%{public}d enter, lpsm = 0x%04X, rpsm = 0x%04X", __FUNCTION__, __LINE__, lpsm, rpsm);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    if ((addr == NULL) || (lcid == NULL) || (cfg == NULL)) {
        return BT_BAD_PARAM;
    }

    if (cfg->mtu < L2CAP_LE_MIN_MTU) {
        return BT_BAD_PARAM;
    }

    psm = L2capLeGetPsm(lpsm);
    if (psm == NULL) {
        return BT_BAD_PARAM;
    }

    conn = L2capLeGetConnection2(addr);
    if (conn == NULL) {
        conn = L2capLeNewConnection(addr, 0, 0);
        isNewConn = true;
    }
    if (conn == NULL) {
        LOG_ERROR("%{public}s: L2capLeNewConnection failed", __FUNCTION__);
        return BT_OPERATION_FAILED;
    }

    chan = L2capLeCreateChannelForConnectReq(conn, lpsm, rpsm, cfg, lcid);
    if (chan == NULL) {
        if (isNewConn) {
            L2capLeDeleteConnection(conn);
        }
        return BT_OPERATION_FAILED;
    }

    return L2capLeStartCreditBasedConnect(addr, conn, chan);
}

int L2CAP_LeCreditBasedConnectionRsp(uint16_t lcid, uint8_t id, const L2capLeConfigInfo *cfg, uint16_t result)
{
    L2capLeConnection *conn = NULL;
    L2capLeChannel *chan = NULL;

    LOG_INFO("%{public}s:%{public}d enter, lcid = 0x%04X, id = %hhu, result = %hu", __FUNCTION__, __LINE__, lcid, id, result);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    if (cfg == NULL) {
        return BT_BAD_PARAM;
    }

    if (cfg->mtu < L2CAP_LE_MIN_MTU) {
        return BT_BAD_PARAM;
    }

    L2capLeGetChannel2(lcid, &conn, &chan);
    if (chan == NULL) {
        return BT_BAD_PARAM;
    }

    if (chan->state != L2CAP_CHANNEL_CONNECT_IN_REQ) {
        return BT_BAD_STATUS;
    }

    chan->lcfg.mtu = cfg->mtu;
    if (cfg->credit != 0) {
        chan->lcfg.credit = cfg->credit;
    }
    // peerCredits must track the granted receive credit, read by the zero-credit gate
    chan->peerCredits = chan->lcfg.credit;
    if (chan->lcfg.mps > cfg->mtu) {
        chan->lcfg.mps = cfg->mtu;
    }

    chan->state = L2CAP_CHANNEL_CONNECTED;
    L2capSendCreditBasedConnectionRsp(conn, chan, id, result);

    if (result != L2CAP_LE_CONNECTION_SUCCESSFUL) {
        L2capLeDeleteChannel(conn, chan, 0);
    }

    return BT_SUCCESS;
}

// EATT initiator: create the batch of EATT channels and send one 0x17 L2CAP_CREDIT_BASED_CONNECTION_REQUEST.
// The local CIDs are returned to the caller through lcids[]. When the ACL is not yet up, the 0x17 is
// deferred and sent by L2capLeAclConnectProcess after the ACL is established. Charter 4.25.
int L2CAP_LeEattConnectionReq(const BtAddr *addr, const L2capLeConfigInfo *cfg, uint16_t lcids[], uint16_t n)
{
    L2capLeConnection *conn = NULL;
    int result;

    if ((addr == NULL) || (cfg == NULL) || (lcids == NULL)) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s:%{public}d enter, mtu = %hu, mps = %hu, credit = %hu, n = %hu", __FUNCTION__, __LINE__,
        cfg->mtu, cfg->mps, cfg->credit, n);

    result = L2capLeEattValidateConnParams(cfg, n);
    if (result != BT_SUCCESS) {
        return result;
    }

    result = L2capLeEattGetOrNewConnection(addr, n, &conn);
    if (result != BT_SUCCESS) {
        return result;
    }

    result = L2capLeEattCheckPendingBatchConfig(conn, cfg);
    if (result != BT_SUCCESS) {
        return result;
    }

    result = L2capLeEattCreateReqChannels(conn, cfg, lcids, n);
    if (result != BT_SUCCESS) {
        // a connection that ended up with no channel at all was created empty by this call and would
        // linger in connList until the process exits; delete it to mirror the 0x14 connect-fail cleanup.
        // A pre-existing connection (e.g. a concurrent 0x14 batch still waiting on the ACL) hosts legacy
        // channels, so the chanList emptiness check keeps it alive, chapter 4.25
        if ((conn->aclHandle == 0) && (ListGetFirstNode(conn->chanList) == NULL)) {
            L2capLeDeleteConnection(conn);
        }
        return result;
    }

    return L2capLeEattConnectOrSendPending(conn, addr, lcids, n);
}

// validate the reconfigure targets: all shall be established EATT channels of the same connection and the
// new MTU/MPS shall respect the chapter 4.27 non-decrease rules; fills conn on success
static int L2capLeEattValidateReconfigureTargets(
    const L2capLeEattCidList *lcids, uint16_t mtu, uint16_t mps, L2capLeConnection **connOut, L2capLeChannel *chans[])
{
    L2capLeConnection *conn = NULL;
    L2capLeChannel *chan = NULL;
    const uint16_t *cids = lcids->cids;
    const uint8_t n = lcids->n;
    uint16_t i;

    // all channels shall be established EATT channels of the same connection
    L2capLeGetChannel2(cids[0], &conn, &chan);
    if ((conn == NULL) || (chan == NULL) || (chan->state != L2CAP_CHANNEL_CONNECTED) ||
        (chan->lpsm != L2CAP_LE_EATT_PSM)) {
        return BT_BAD_PARAM;
    }
    chans[0] = chan;

    for (i = 1; i < n; i++) {
        chan = L2capLeGetChannel(conn, cids[i]);
        if ((chan == NULL) || (chan->state != L2CAP_CHANNEL_CONNECTED) || (chan->lpsm != L2CAP_LE_EATT_PSM)) {
            return BT_BAD_PARAM;
        }
        chans[i] = chan;
    }

    /*
     * chapter 4.27: the sender shall not reduce its receive MTU below the greatest current MTU of
     * the target channels; reject locally so a certain refusal does not round-trip the 0x19
     */
    for (i = 0; i < n; i++) {
        if (mtu < chans[i]->lcfg.mtu) {
            return BT_BAD_PARAM;
        }
    }

    /*
     * chapter 4.27: with more than one channel the sender shall not reduce its receive MPS either;
     * a single channel may reduce MPS, the peer refuses otherwise (result 0x0002)
     */
    if (n > 1) {
        for (i = 0; i < n; i++) {
            if (mps < chans[i]->lcfg.mps) {
                return BT_BAD_PARAM;
            }
        }
    }

    *connOut = conn;
    return BT_SUCCESS;
}

// EATT initiator: reconfigure the local receive MTU/MPS of the established channels and send one 0x19
// L2CAP_CREDIT_BASED_RECONFIGURE_REQUEST. The 0x19 Destination CIDs are the sender's local CIDs, chapter 4.27.
int L2CAP_LeEattReconfigureReq(const L2capLeEattCidList *lcids, uint16_t mtu, uint16_t mps)
{
    L2capLeConnection *conn = NULL;
    L2capLeChannel *chans[L2CAP_LE_EATT_MAX_CHANNEL] = { 0 };

    LOG_INFO("%{public}s:%{public}d enter, n = %hhu, mtu = %hu, mps = %hu", __FUNCTION__, __LINE__,
        (uint8_t)((lcids != NULL) ? lcids->n : 0), mtu, mps);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    if ((lcids == NULL) || (lcids->cids == NULL)) {
        return BT_BAD_PARAM;
    }

    if ((lcids->n < 1) || (lcids->n > L2CAP_LE_EATT_MAX_CHANNEL)) {
        return BT_BAD_PARAM;
    }

    if ((mtu < L2CAP_LE_EATT_MIN_MTU) || (mps < L2CAP_LE_EATT_MIN_MPS)) {
        return BT_BAD_PARAM;
    }

    /* chapter 4.25: the MPS of an ECRED channel is at most 65533 octets */
    if (mps > L2CAP_LE_EATT_MAX_MPS) {
        return BT_BAD_PARAM;
    }

    if (L2capLeEattValidateReconfigureTargets(lcids, mtu, mps, &conn, chans) != BT_SUCCESS) {
        return BT_BAD_PARAM;
    }

    return L2capLeEattSendReconfigureReq(conn, mtu, mps, lcids);
}

// Set the EATT responder default local config of a registered LE PSM. The values are used for the local
// MTU/MPS/Credits fields of the 0x18 response to an inbound 0x17, chapter 4.26.
int L2CAP_LeSetServiceConfig(uint16_t lpsm, const L2capLeConfigInfo *cfg)
{
    L2capLePsm *psm = NULL;

    LOG_INFO("%{public}s:%{public}d enter, psm = 0x%04X", __FUNCTION__, __LINE__, lpsm);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    if (cfg == NULL) {
        return BT_BAD_PARAM;
    }

    if ((cfg->mtu < L2CAP_LE_EATT_MIN_MTU) || (cfg->mps < L2CAP_LE_EATT_MIN_MPS) ||
        (cfg->mps > L2CAP_LE_EATT_MAX_MPS)) {
        return BT_BAD_PARAM;
    }
    // the responder advertises this MPS in the 0x18 and the peer fragments by it, so cap it at the
    // stack PDU ceiling (same expression the connection paths clamp the negotiated MPS to,
    // L2capLeEattEstablishReqChannels / L2capLeEattEstablishChannels); a larger value would make the
    // peer send K-frames the stack cannot buffer
    if (cfg->mps > (L2capLeGetTxBufferSize() - L2CAP_SIZE_6)) {
        return BT_BAD_PARAM;
    }

    psm = L2capLeGetPsm(lpsm);
    if (psm == NULL) {
        return BT_BAD_PARAM;
    }

    psm->lcfg = *cfg;
    return BT_SUCCESS;
}

// Set the EATT security requirement bits of a registered LE PSM. The bits are checked against the link
// security state for an inbound 0x17, and the insufficient ones reject the request, table 4.21.
int L2CAP_LeSetServiceSecLevel(uint16_t lpsm, uint8_t secRequirement)
{
    L2capLePsm *psm = NULL;

    LOG_INFO("%{public}s:%{public}d enter, psm = 0x%04X, secRequirement = %hhu", __FUNCTION__, __LINE__, lpsm,
        secRequirement);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    // only the authentication and authorization bits are valid
    if (secRequirement & ~(L2CAP_LE_EATT_SEC_REQUIRE_AUTHENTICATION | L2CAP_LE_EATT_SEC_REQUIRE_AUTHORIZATION)) {
        return BT_BAD_PARAM;
    }

    psm = L2capLeGetPsm(lpsm);
    if (psm == NULL) {
        return BT_BAD_PARAM;
    }

    psm->secRequirement = secRequirement;
    return BT_SUCCESS;
}

// Inject the link-level security state of an LE connection for the inbound 0x17 security check.
// The encryption state is maintained by the HCI 0x08 event, while the key size, authentication and
// authorization are provided by the upper layer after the link is secured, Vol 3 Part G 5.3.2.
int L2CAP_LeSetSecurityInfo(const BtAddr *addr, uint8_t keySize, uint8_t authLevel, uint8_t authzGranted)
{
    L2capLeConnection *conn = NULL;

    LOG_INFO("%{public}s:%{public}d enter, keySize = %hhu, authLevel = %hhu, authzGranted = %hhu", __FUNCTION__,
        __LINE__, keySize, authLevel, authzGranted);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    if (addr == NULL) {
        return BT_BAD_PARAM;
    }

    // LE key sizes range from 7 to 16 bytes, 0 means the upper layer did not provide the value
    if ((keySize > 16) || ((keySize != 0) && (keySize < 7))) {
        return BT_BAD_PARAM;
    }

    conn = L2capLeGetConnection2(addr);
    if (conn == NULL) {
        return BT_BAD_PARAM;
    }

    conn->eatt.keySize = keySize;
    conn->eatt.authLevel = authLevel;
    conn->eatt.authzGranted = authzGranted;
    return BT_SUCCESS;
}

int L2CAP_LeDisconnectionReq(uint16_t lcid)
{
    L2capLeChannel *chan = NULL;
    L2capLeConnection *conn = NULL;

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    L2capLeGetChannel2(lcid, &conn, &chan);
    if (chan == NULL) {
        return BT_BAD_PARAM;
    }

    if (chan->state != L2CAP_CHANNEL_CONNECTED) {
        return BT_BAD_STATUS;
    }

    chan->state = L2CAP_CHANNEL_DISCONNECT_OUT_REQ;
    L2capLeSendDisconnectionReq(conn, chan);
    return BT_SUCCESS;
}

int L2CAP_LeDisconnectionRsp(uint16_t lcid, uint8_t id)
{
    L2capLeConnection *conn = NULL;
    L2capLeChannel *chan = NULL;

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    L2capLeGetChannel2(lcid, &conn, &chan);
    if (chan == NULL) {
        return BT_BAD_PARAM;
    }

    if (chan->state != L2CAP_CHANNEL_DISCONNECT_IN_REQ) {
        return BT_BAD_STATUS;
    }

    L2capLeSendDisconnectionRsp(conn, chan, id);
    // The peer's 0x06 is answered, the channel is terminated (Vol 3 Part A 4.6): delete it so a closed
    // channel no longer counts toward the ECRED budget.
    L2capLeDeleteChannel(conn, chan, (chan->lpsm == L2CAP_LE_EATT_PSM) ? 0 : 1);
    return BT_SUCCESS;
}

int L2CAP_LeSendData(uint16_t lcid, Packet *pkt)
{
    L2capLeConnection *conn = NULL;
    L2capLeChannel *chan = NULL;
    uint16_t length;

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    L2capLeGetChannel2(lcid, &conn, &chan);
    if (chan == NULL) {
        return BT_BAD_PARAM;
    }

    if (pkt == NULL) {
        return BT_BAD_PARAM;
    }

    if (chan->state != L2CAP_CHANNEL_CONNECTED) {
        return BT_BAD_STATUS;
    }

    length = PacketSize(pkt);
    if (length > chan->rcfg.mtu) {
        return BT_BAD_PARAM;
    }

    if (chan->txList == NULL) {
        chan->txList = ListCreate(NULL);
    }

    if (length > chan->rcfg.mps) {
        L2capLeSegmentPacketWithCredit(chan, pkt);
    } else {
        Packet *tpkt = NULL;
        uint8_t *header = NULL;

        tpkt = PacketInheritMalloc(pkt, L2CAP_SIZE_6, 0);
        header = BufferPtr(PacketHead(tpkt));

        L2capCpuToLe16(header + 0, length + L2CAP_SIZE_2);
        L2capCpuToLe16(header + L2CAP_OFFSET_2, chan->rcid);
        L2capCpuToLe16(header + L2CAP_OFFSET_4, length);

        ListAddLast(chan->txList, tpkt);
    }

    L2capLeTxWithCredit(conn, chan);
    return BT_SUCCESS;
}

int L2CAP_LeRegisterService(uint16_t lpsm, const L2capLeService *svc, void *context)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    L2capLePsm *psm = NULL;

    LOG_INFO("%{public}s:%{public}d enter, psm = 0x%04X", __FUNCTION__, __LINE__, lpsm);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    if (svc == NULL) {
        return BT_BAD_PARAM;
    }

    // check whether the psm is valid
    if (!(lpsm & 0x0001) || (lpsm & 0x0100)) {
        return BT_BAD_PARAM;
    }

    psm = L2capLeGetPsm(lpsm);
    if (psm != NULL) {
        return BT_BAD_STATUS;
    }

    psm = L2capAlloc(sizeof(L2capLePsm));
    if (psm == NULL) {
        return BT_NO_MEMORY;
    }

    psm->lpsm = lpsm;
    psm->ctx = context;
    (void)memcpy_s(&(psm->service), sizeof(L2capLeService), svc, sizeof(L2capLeService));
    ListAddFirst(inst->psmList, psm);

    return BT_SUCCESS;
}

int L2CAP_LeDeregisterService(uint16_t lpsm)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    L2capLePsm *psm = NULL;
    L2capLeConnection *conn = NULL;
    ListNode *nodeConnection = NULL;

    LOG_INFO("%{public}s:%{public}d enter, psm = 0x%04X", __FUNCTION__, __LINE__, lpsm);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    psm = L2capLeGetPsm(lpsm);
    if (psm == NULL) {
        return BT_BAD_PARAM;
    }

    nodeConnection = ListGetFirstNode(inst->connList);
    while (nodeConnection != NULL) {
        L2capLeChannel *chan = NULL;
        ListNode *nodeChannel = NULL;

        conn = ListGetNodeData(nodeConnection);
        nodeChannel = ListGetFirstNode(conn->chanList);
        while (nodeChannel != NULL) {
            chan = ListGetNodeData(nodeChannel);
            // if any channel used the psm, return error
            if (chan->lpsm == lpsm) {
                return BT_BAD_STATUS;
            }

            nodeChannel = ListGetNextNode(nodeChannel);
        }

        nodeConnection = ListGetNextNode(nodeConnection);
    }

    ListRemoveNode(inst->psmList, psm);
    L2capFree(psm);
    return BT_SUCCESS;
}

int L2CAP_LeRegisterFixChannel(uint16_t cid, const L2capLeFixChannel *chan)
{
    L2capLeInstance *inst = &g_l2capLeInst;

    LOG_INFO("%{public}s:%{public}d enter, cid = 0x%04X", __FUNCTION__, __LINE__, cid);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    if (cid == L2CAP_LE_ATT_CHANNEL) {
        inst->chanAtt.leConnected = chan->leConnected;
        inst->chanAtt.leDisconnected = chan->leDisconnected;
        inst->chanAtt.recvLeData = chan->recvLeData;
    } else if (cid == L2CAP_LE_SMP_CHANNEL) {
        inst->chanSm.leConnected = chan->leConnected;
        inst->chanSm.leDisconnected = chan->leDisconnected;
        inst->chanSm.recvLeData = chan->recvLeData;
    } else {
        return BT_BAD_PARAM;
    }

    return BT_SUCCESS;
}

int L2CAP_LeDeregisterFixChannel(uint16_t cid)
{
    L2capLeInstance *inst = &g_l2capLeInst;

    LOG_INFO("%{public}s:%{public}d enter, cid = 0x%04X", __FUNCTION__, __LINE__, cid);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    if (cid == L2CAP_LE_ATT_CHANNEL) {
        inst->chanAtt.leConnected = NULL;
        inst->chanAtt.leDisconnected = NULL;
        inst->chanAtt.recvLeData = NULL;
    } else if (cid == L2CAP_LE_SMP_CHANNEL) {
        inst->chanSm.leConnected = NULL;
        inst->chanSm.leDisconnected = NULL;
        inst->chanSm.recvLeData = NULL;
    } else {
        return BT_BAD_PARAM;
    }

    return BT_SUCCESS;
}

// Find the pending parameter slot of the address, else the first free slot, else the
// first slot (more concurrent connects than the ring can hold; the evicted connection
// degrades to the 100 ms retry floor, same as the old single slot).
static L2capLePendingConnParam *L2capLeGetPendingConnParamSlot(L2capLeInstance *inst, const BtAddr *addr)
{
    for (uint8_t i = 0; i < L2CAP_LE_GRANTED_CONN_PARAM_MAX; i++) {
        if (inst->pendingConnParams[i].valid &&
            (memcmp(&(inst->pendingConnParams[i].addr), addr, sizeof(BtAddr)) == 0)) {
            return &inst->pendingConnParams[i];
        }
    }
    for (uint8_t i = 0; i < L2CAP_LE_GRANTED_CONN_PARAM_MAX; i++) {
        if (!inst->pendingConnParams[i].valid) {
            return &inst->pendingConnParams[i];
        }
    }
    LOG_WARN("L2CAP_LeConnect: pending parameter slots exhausted, evicting slot 0");
    return &inst->pendingConnParams[0];
}

int L2CAP_LeConnect(const BtAddr *addr, const L2capLeConnectionParameter *param)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    L2capLeConnection *conn = NULL;

    LOG_INFO("%{public}s:%{public}d enter", __FUNCTION__, __LINE__);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    if (addr == NULL) {
        return BT_BAD_PARAM;
    }

    // Vol 3 Part G 5.4: keep the requested connection parameters for the slave EATT retry delay;
    // the connection may not exist until connect-complete, so stash them and apply them there.
    // A per-address slot array keeps two connects in flight at the same time from overwriting
    // each other's unconsumed parameters (replace the entry of the same address first, then a
    // free slot, mirroring L2capLeRecvConnectionComplete).
    if (param != NULL) {
        conn = L2capLeGetConnection2(addr);
        if (conn != NULL) {
            conn->connIntervalUnits = param->connIntervalMax;
            conn->connSlaveLatency = param->connLatency;
        } else {
            L2capLePendingConnParam *slot = L2capLeGetPendingConnParamSlot(inst, addr);
            (void)memcpy_s(&(slot->addr), sizeof(BtAddr), addr, sizeof(BtAddr));
            slot->connIntervalMax = param->connIntervalMax;
            slot->connLatency = param->connLatency;
            slot->valid = 1;
        }
    }

    return L2capConnectLe(addr);
}

int L2CAP_LeConnectCancel(const BtAddr *addr)
{
    LOG_INFO("%{public}s:%{public}d enter", __FUNCTION__, __LINE__);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    return L2capConnectLeCancel(addr);
}

int L2CAP_LeDisconnect(uint16_t aclHandle)
{
    LOG_INFO("%{public}s:%{public}d enter, handle = 0x%04X", __FUNCTION__, __LINE__, aclHandle);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    // Reason: REMOTE USER TERMINATED CONNECTION
    return L2capDisconnect(aclHandle, 0x13);
}

int L2CAP_LeSendFixChannelData(uint16_t aclHandle, uint16_t cid, const Packet *pkt)
{
    L2capLeConnection *conn = NULL;
    uint16_t length;
    Packet *tpkt = NULL;
    uint8_t *header = NULL;

    LOG_INFO("%{public}s:%{public}d enter, cid = 0x%04X, pktLength = %u", __FUNCTION__, __LINE__, cid, PacketSize(pkt));

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    conn = L2capLeGetConnection(aclHandle);
    if (conn == NULL) {
        return BT_BAD_PARAM;
    }

    length = PacketSize(pkt);

    tpkt = PacketInheritMalloc(pkt, L2CAP_HEADER_LENGTH, 0);
    header = BufferPtr(PacketHead(tpkt));

    L2capCpuToLe16(header + 0, length);
    L2capCpuToLe16(header + L2CAP_OFFSET_2, cid);

    L2capLeSendPacket(aclHandle, tpkt);

    return BT_SUCCESS;
}

int L2CAP_LeRegisterConnectionParameterUpdate(const L2capLeConnectionParameterUpdate *cb, void *context)
{
    L2capLeInstance *inst = &g_l2capLeInst;

    LOG_INFO("%{public}s:%{public}d enter", __FUNCTION__, __LINE__);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    if (cb == NULL) {
        return BT_BAD_PARAM;
    }

    inst->connParamUpdate.ctx = context;
    (void)memcpy_s(&(inst->connParamUpdate.cb),
        sizeof(L2capLeConnectionParameterUpdate),
        cb,
        sizeof(L2capLeConnectionParameterUpdate));

    return BT_SUCCESS;
}

int L2CAP_LeDeregisterConnectionParameterUpdate()
{
    L2capLeInstance *inst = &g_l2capLeInst;

    LOG_INFO("%{public}s:%{public}d enter", __FUNCTION__, __LINE__);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    (void)memset_s(&(inst->connParamUpdate),
        sizeof(L2capLeConnectionParameterUpdate),
        0,
        sizeof(L2capLeConnectionParameterUpdate));
    return BT_SUCCESS;
}

int L2CAP_LeConnectionParameterUpdateReq(uint16_t aclHandle, const L2capLeConnectionParameter *param)
{
    L2capLeConnection *conn = NULL;
    uint8_t buff[8] = {0};
    L2capSignalHeader signal = {0};
    Packet *pkt = NULL;

    LOG_INFO("%{public}s:%{public}d enter, handle = 0x%04X", __FUNCTION__, __LINE__, aclHandle);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    if ((param->connIntervalMin < 0x0006) || (param->connIntervalMin > 0x0C80)) {
        return BT_BAD_PARAM;
    }

    if ((param->connIntervalMax < 0x0006) || (param->connIntervalMax > 0x0C80)) {
        return BT_BAD_PARAM;
    }

    if ((param->supervisionTimeout < 0x000A) || (param->supervisionTimeout > 0x0C80)) {
        return BT_BAD_PARAM;
    }

    if (param->connLatency > 0x01F3) {
        return BT_BAD_PARAM;
    }

    if (param->connLatency > ((param->supervisionTimeout / (param->connIntervalMax * L2CAP_SIZE_2)) - 1)) {
        return BT_BAD_PARAM;
    }

    conn = L2capLeGetConnection(aclHandle);
    if (conn == NULL) {
        return BT_BAD_PARAM;
    }

    if (conn->role != L2CAP_LE_ROLE_SLAVE) {
        return BT_BAD_PARAM;
    }

    L2capCpuToLe16(buff + 0, param->connIntervalMin);
    L2capCpuToLe16(buff + L2CAP_OFFSET_2, param->connIntervalMax);
    L2capCpuToLe16(buff + L2CAP_OFFSET_4, param->connLatency);
    L2capCpuToLe16(buff + L2CAP_OFFSET_6, param->supervisionTimeout);

    signal.code = L2CAP_CONNECTION_PARAMETER_UPDATE_REQUEST;
    signal.identifier = L2capLeGetNewIdentifier(conn);
    signal.length = sizeof(buff);

    pkt = L2capBuildSignalPacket(L2CAP_LE_SIGNALING_CHANNEL, &signal, buff);
    if (L2capCreatePendingRequest(
        conn->pendingList, 0, &signal, L2CAP_DEFAULT_RTX, L2capLeResponseTimeoutCallback) != BT_SUCCESS) {
        // no pending entry and no RTX timer: the peer's 0x13 would have nothing to match, so the
        // request must not be sent
        PacketFree(pkt);
        return BT_NO_MEMORY;
    }
    return L2capLeSendPacket(aclHandle, pkt);
}

int L2CAP_LeConnectionParameterUpdateRsp(uint16_t aclHandle, uint8_t id, uint16_t result)
{
    L2capLeConnection *conn = NULL;
    uint8_t buff[2] = {0};
    L2capSignalHeader signal = {0};
    Packet *pkt = NULL;

    LOG_INFO("%{public}s:%{public}d enter, handle = 0x%04X", __FUNCTION__, __LINE__, aclHandle);

    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }

    conn = L2capLeGetConnection(aclHandle);
    if (conn == NULL) {
        return BT_BAD_PARAM;
    }

    L2capCpuToLe16(buff + 0, result);

    signal.code = L2CAP_CONNECTION_PARAMETER_UPDATE_RESPONSE;
    signal.identifier = id;
    signal.length = sizeof(buff);

    pkt = L2capBuildSignalPacket(L2CAP_LE_SIGNALING_CHANNEL, &signal, buff);

    return L2capLeSendPacket(aclHandle, pkt);
}

void L2CAP_LeInitialize(int traceLevel)
{
    L2capLeInstance *inst = &g_l2capLeInst;
    L2capLeCallback cmnCallback = {0};

    LOG_INFO("%{public}s:%{public}d enter", __FUNCTION__, __LINE__);

    cmnCallback.aclConnected = L2capLeConnectComplete;
    cmnCallback.aclDisconnected = L2capLeDisconnectComplete;
    cmnCallback.recvL2capPacket = L2capLeReceivePacket;
    L2capRegisterLe(&cmnCallback);

    // listen for the HCI 0x08 encryption change event to maintain the link encryption state
    (void)HCI_RegisterEventCallbacks(&g_l2capLeHciCallbacks);

    inst->psmList = ListCreate(NULL);
    inst->connList = ListCreate(NULL);
    inst->nextLcid = L2CAP_LE_MIN_CID;
    return;
}

void L2CAP_LeFinalize()
{
    L2capLeInstance *leinst = &g_l2capLeInst;
    ListNode *node = NULL;
    L2capLeConnection *conn = NULL;
    void *psm = NULL;

    LOG_INFO("%{public}s:%{public}d enter", __FUNCTION__, __LINE__);

    (void)HCI_DeregisterEventCallbacks(&g_l2capLeHciCallbacks);

    node = ListGetFirstNode(leinst->connList);
    while (node != NULL) {
        conn = ListGetNodeData(node);
        L2capLeDeleteConnection(conn);

        node = ListGetFirstNode(leinst->connList);
    }

    ListDelete(leinst->connList);
    leinst->connList = NULL;

    node = ListGetFirstNode(leinst->psmList);
    while (node != NULL) {
        psm = ListGetNodeData(node);
        ListRemoveNode(leinst->psmList, psm);
        L2capFree(psm);

        node = ListGetFirstNode(leinst->psmList);
    }

    ListDelete(leinst->psmList);
    leinst->psmList = NULL;

    return;
}
