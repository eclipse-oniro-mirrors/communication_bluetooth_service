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

/**
 * @file l2cap_le_eatt.c
 *
 * @brief EATT enhanced credit based flow control mode (0x17-0x1A) of the LE L2CAP signaling,
 *        chapter 4.25-4.28 of Core 5.2
 *
 */

#include "l2cap_le_internal.h"

#include <stdbool.h>
#include <string.h>

// -------------------- EATT enhanced credit based flow control mode (0x17-0x1A) --------------------
// Signaling and wire format follow chapter 4.25-4.28 of Core 5.2, collision retry follows Vol 3 Part G 5.4.
// Purpose: establish multiple ECRED channels on one ACL, each channel becomes one Enhanced ATT bearer.

// Count the EATT channels of a connection; a connection can host at most L2CAP_LE_EATT_MAX_CHANNEL
// channels, chapter 4.25
static uint8_t L2capLeCountEattChannels(L2capLeConnection *conn)
{
    L2capLeChannel *chan = NULL;
    ListNode *node = NULL;
    uint8_t n = 0;

    node = ListGetFirstNode(conn->chanList);
    while (node != NULL) {
        chan = ListGetNodeData(node);
        if (chan->lpsm == L2CAP_LE_EATT_PSM) {
            n++;
        }
        node = ListGetNextNode(node);
    }
    return n;
}

// Collect the EATT channels that are not yet sent (CONNECT_OUT_REQ and not yet covered by a 0x17),
// used by the batched send and retry
static uint8_t L2capLeEattGetPendingChannels(L2capLeConnection *conn, L2capLeChannel *chans[], uint8_t max)
{
    L2capLeChannel *chan = NULL;
    ListNode *node = NULL;
    uint8_t n = 0;

    node = ListGetFirstNode(conn->chanList);
    while ((node != NULL) && (n < max)) {
        chan = ListGetNodeData(node);
        // connIdentifier == 0 means the channel is not yet covered by an in-flight 0x17, avoid recollecting
        if ((chan->state == L2CAP_CHANNEL_CONNECT_OUT_REQ) && (chan->connIdentifier == 0) &&
            (chan->lpsm == L2CAP_LE_EATT_PSM)) {
            chans[n++] = chan;
        }
        node = ListGetNextNode(node);
    }
    return n;
}

// Whether this side already has an in-flight EATT 0x17 request, used by collision detection of Vol 3 Part G 5.4
static bool L2capLeEattHasPendingRequest(L2capLeConnection *conn)
{
    L2capPendingRequest *req = NULL;
    ListNode *node = NULL;

    node = ListGetFirstNode(conn->pendingList);
    while (node != NULL) {
        req = ListGetNodeData(node);
        if (req->code == L2CAP_CREDIT_BASED_CONNECTION_REQUEST) {
            return true;
        }
        node = ListGetNextNode(node);
    }
    return false;
}

// Mark every in-flight 0x17 batch as collided, Vol 3 Part G 5.4. The marker lives per pending entry,
// not per connection: several batches can be in flight at once, and each batch's all-refused 0x18
// consumes its own marker to retry. A single connection-level value would be consumed by the first
// 0x18 while the later batches miss their 5.4 retry (their NO_RESOURCES 0x18 would settle as a final
// refusal, silently deleting the batch channels and deferring the failure to the ATT fallback).
static void L2capLeEattMarkPendingCollision(L2capLeConnection *conn)
{
    L2capPendingRequest *req = NULL;
    ListNode *node = NULL;

    node = ListGetFirstNode(conn->pendingList);
    while (node != NULL) {
        req = ListGetNodeData(node);
        if (req->code == L2CAP_CREDIT_BASED_CONNECTION_REQUEST) {
            req->collision = 1;
        }
        node = ListGetNextNode(node);
    }
    return;
}

// Destroy an in-flight signaling request only when its code matches the expected type. The pending
// entry is found by identifier (L2capGetPendingRequest matches identifier alone), but identifiers are
// per-connection sequential and a request of another type may reuse the value, so a 0x18/0x1A handler
// must not tear down a pending entry it did not create.
static void L2capLeEattDestroyPendingByCode(L2capLeConnection *conn, uint8_t code, uint8_t identifier)
{
    L2capPendingRequest *req = L2capGetPendingRequest(conn->pendingList, identifier);
    if ((req != NULL) && (req->code == code)) {
        L2capDestroyPendingRequest(conn->pendingList, identifier);
    }
}

// Look up a channel by the peer CID (rcid): both the inbound 0x17 Source CID and the inbound 0x19
// Destination CID are peer-perspective CIDs, matched against our rcid (0x19 DCID = sender's local CID,
// chapter 4.27). Only CONNECTED channels match: an rcid of a channel that is being set up or torn
// down (or already closed) must not receive 0x16 flow control credits or data (a stale rcid could
// otherwise be matched by a fresh channel that reused the peer CID mid-teardown).
L2capLeChannel *L2capLeEattGetChannelByRcid(L2capLeConnection *conn, uint16_t rcid)
{
    L2capLeChannel *chan = NULL;
    ListNode *node = NULL;

    node = ListGetFirstNode(conn->chanList);
    while (node != NULL) {
        chan = ListGetNodeData(node);
        if (chan->rcid == rcid && chan->state == L2CAP_CHANNEL_CONNECTED) {
            return chan;
        }
        node = ListGetNextNode(node);
    }
    return NULL;
}

// Security check of the inbound 0x17, returns the result codes of table 4.21 in priority order.
// Vol 3 Part G 5.3.2 requires EATT channels to be encrypted; the check order is encryption -> key size >= 7
// -> authentication -> authorization. Encryption is maintained by the HCI 0x08 event, the others are injected.
static uint16_t L2capLeEattCheckSecurity(L2capLeConnection *conn, L2capLePsm *psm)
{
    if (!conn->eatt.encrypt) {
        return L2CAP_LE_INSUFFICIENT_ENCRYPTION; // 0x0008 link not encrypted
    }

    if ((conn->eatt.keySize != 0) && (conn->eatt.keySize < L2CAP_LE_EATT_KEY_SIZE_MIN)) {
        return L2CAP_LE_INSUFFICIENT_ENCRYPTION_KEY_SIZE; // 0x0007 key size less than 7 bytes
    }

    if ((psm->secRequirement & L2CAP_LE_EATT_SEC_REQUIRE_AUTHENTICATION) && !conn->eatt.authLevel) {
        return L2CAP_LE_INSUFFICIENT_AUTHENTICATION; // 0x0005 authentication failed
    }

    if ((psm->secRequirement & L2CAP_LE_EATT_SEC_REQUIRE_AUTHORIZATION) && !conn->eatt.authzGranted) {
        return L2CAP_LE_INSUFFICIENT_AUTHORIZATION; // 0x0006 authorization failed
    }

    return 0; // 0x0000 success
}

// Send the 0x17 L2CAP_CREDIT_BASED_CONNECTION_REQUEST, chapter 4.25.
// Wire format: SPSM(2)+MTU(2)+MPS(2)+InitialCredits(2)+SCIDs(2n), length = 8+2n;
// one batch shares the same MTU/MPS/Credits. Mark each channel with this request identifier for 0x18 matching.
static int L2capLeEattSendConnectionReq(
    L2capLeConnection *conn, uint16_t psm, const L2capLeConfigInfo *cfg, const L2capLeEattCidList *scids)
{
    Packet *pkt = NULL;
    uint8_t buff[L2CAP_LE_EATT_CONN_FIXED_LEN + L2CAP_LE_EATT_CID_LEN * L2CAP_LE_EATT_MAX_CHANNEL] = { 0 };
    L2capSignalHeader signal = { 0 };

    if ((cfg == NULL) || (scids == NULL) || (scids->cids == NULL) || (scids->n > L2CAP_LE_EATT_MAX_CHANNEL)) {
        return BT_BAD_PARAM;
    }

    signal.code = L2CAP_CREDIT_BASED_CONNECTION_REQUEST;
    signal.identifier = L2capLeGetNewIdentifier(conn);
    signal.length = L2CAP_LE_EATT_CONN_FIXED_LEN + L2CAP_LE_EATT_CID_LEN * scids->n;

    L2capCpuToLe16(buff + 0, psm);
    L2capCpuToLe16(buff + L2CAP_OFFSET_2, cfg->mtu);
    L2capCpuToLe16(buff + L2CAP_OFFSET_4, cfg->mps);
    L2capCpuToLe16(buff + L2CAP_OFFSET_6, cfg->credit);
    for (uint8_t i = 0; i < scids->n; i++) {
        L2capCpuToLe16(buff + L2CAP_OFFSET_8 + i * L2CAP_LE_EATT_CID_LEN, scids->cids[i]);
    }

    // build the packet before touching any state so an allocation failure cannot leave the batch
    // channels marked (connIdentifier) or an orphaned pending entry behind
    pkt = L2capBuildSignalPacket(L2CAP_LE_SIGNALING_CHANNEL, &signal, buff);
    if (pkt == NULL) {
        return BT_NO_MEMORY;
    }

    // create the RTX pending entry before marking the batch. Its failure is verified by the
    // return value (alloc / AlarmCreate / AlarmSet failures) and by the landing check (the list
    // append itself can fail silently): without the entry the peer's 0x18 would have no pending
    // entry to match and no RTX timer would ever recycle the batch (channels stuck in
    // CONNECT_OUT_REQ until the ACL tears down). Roll back like a send failure: the caller's
    // rollback (L2capLeEattSendPendingRequest) destroys the pending entry by identifier and
    // deletes the batch channels.
    int pendResult = L2capCreatePendingRequest(
        conn->pendingList, 0, &signal, L2CAP_DEFAULT_RTX, L2capLeResponseTimeoutCallback);
    if (pendResult != BT_SUCCESS) {
        PacketFree(pkt);
        return pendResult;
    }
    L2capPendingRequest *pend = L2capGetPendingRequest(conn->pendingList, signal.identifier);
    if (pend == NULL) {
        PacketFree(pkt);
        return BT_NO_MEMORY;
    }

    // mark the request identifier and the entry's batch sequence on the batch, used by the 0x18
    // response and the timeout to match. The sequence ties the channels to exactly this pending
    // entry: within the RTX window the identifier alone can wrap onto a newer batch of the same
    // connection, and a stale expiry of this entry must not clean that batch's channels (see
    // L2capPendingRequest::seq). Marking happens after the entry landed, so a creation failure
    // leaves the channels unmarked (the caller's rollback deletes them by its own pointers).
    for (uint8_t i = 0; i < scids->n; i++) {
        L2capLeChannel *chan = L2capLeGetChannel(conn, scids->cids[i]);
        if (chan != NULL) {
            chan->connIdentifier = signal.identifier;
            chan->batchSeq = pend->seq;
        }
    }
    return L2capLeSendPacket(conn->aclHandle, pkt);
}

int L2capLeEattSendPendingRequest(L2capLeConnection *conn)
{
    L2capLeChannel *chans[L2CAP_LE_EATT_MAX_CHANNEL] = { 0 };
    uint16_t scids[L2CAP_LE_EATT_MAX_CHANNEL] = { 0 };
    L2capLeEattCidList scidList = { 0 };
    uint8_t i;
    uint8_t n;
    int ret;

    n = L2capLeEattGetPendingChannels(conn, chans, L2CAP_LE_EATT_MAX_CHANNEL);
    if (n == 0) {
        return BT_SUCCESS;
    }

    for (i = 0; i < n; i++) {
        scids[i] = chans[i]->lcid;
    }
    scidList.cids = scids;
    scidList.n = n;
    ret = L2capLeEattSendConnectionReq(conn, L2CAP_LE_EATT_PSM, &chans[0]->lcfg, &scidList);
    if (ret != BT_SUCCESS) {
        // chapter 4.25: the 0x17 never left the stack, roll the batch back so the caller's lcids do
        // not reference channels with no request in flight
        L2capLeEattDestroyPendingByCode(conn, L2CAP_CREDIT_BASED_CONNECTION_REQUEST, chans[0]->connIdentifier);
        for (i = 0; i < n; i++) {
            // When this is the deferred send of L2capLeAclConnectProcess, the ATT establishment
            // callback was already notified with BT_SUCCESS at dispatch time (AttEattEstablishSendResult
            // retains the lcids), so the channels must be announced as abnormally disconnected before
            // deletion - otherwise eattEstablishCb would hang until the ACL tears down. Same pattern as
            // the 0x18 send failure (L2capLeEattSendReqResponse) and the timeout cleanup
            // (L2capLeEattDiscardTimedOutBatch): abnormal disconnect per channel, then delete.
            L2capLePsm *psm = L2capLeGetPsm(chans[i]->lpsm);
            if ((psm != NULL) && (psm->service.leDisconnectAbnormal != NULL)) {
                LOG_DEBUG("L2capCallback leDisconnectAbnormal:%{public}d begin, cid = 0x%04X, reason = 0", __LINE__,
                    chans[i]->lcid);
                psm->service.leDisconnectAbnormal(chans[i]->lcid, 0, psm->ctx);
                LOG_DEBUG("L2capCallback leDisconnectAbnormal:%{public}d end", __LINE__);
            }
            L2capLeDeleteChannel(conn, chans[i], 0);
        }
        LOG_WARN("L2capLeEattSendConnectionReq failed, result = %{public}d, n = %hhu, channels rolled back", ret, n);
        return ret;
    }
    return BT_SUCCESS;
}

// slave collision retry (Vol 3 Part G 5.4) running on the L2CAP queue: re-verify that the connection
// is still alive (the timer may have fired just as the link tore down) before resending the batch.
// The retry is armed with the numeric ACL handle instead of a raw connection pointer: a pointer
// captured at arm time can be re-issued to a newer connection after the original one is torn down
// (malloc address reuse), which would mis-deliver the stale retry to the new connection. A
// connection handle is allocated by the controller and handed back per LE connection, so a
// match here identifies a live connection beyond doubt.
static void L2capLeEattRetryProcess(const void *parameter)
{
    uint16_t aclHandle = (uint16_t)(uintptr_t)parameter;
    L2capLeInstance *inst = &g_l2capLeInst;
    L2capLeConnection *conn = NULL;
    ListNode *node = NULL;

    node = ListGetFirstNode(inst->connList);
    while (node != NULL) {
        conn = ListGetNodeData(node);
        if (conn->aclHandle == aclHandle) {
            // chapter 4.25/4.26: the batch channels were released at collision resolution
            // (connIdentifier cleared), so the fresh request recollects them with a new identifier
            L2capLeEattSendPendingRequest(conn);
            return;
        }
        node = ListGetNextNode(node);
    }
    // connection torn down before the retry fired; nothing to do
}

static void L2capLeEattRetryTimerCallback(void *parameter)
{
    L2capAsynchronousProcess(L2capLeEattRetryProcess, NULL, parameter);
    return;
}

// 0x18 L2CAP_CREDIT_BASED_CONNECTION_RESPONSE, chapter 4.26
static int L2capLeEattSendConnectionRsp(L2capLeConnection *conn, uint8_t ident, const L2capLeConfigInfo *cfg,
    uint16_t result, const L2capLeEattCidList *dcids)
{
    Packet *pkt = NULL;
    uint8_t buff[L2CAP_LE_EATT_CONN_FIXED_LEN + L2CAP_LE_EATT_CID_LEN * L2CAP_LE_EATT_MAX_CHANNEL] = { 0 };
    L2capSignalHeader signal = { 0 };

    /*
     * chapter 4.26: a wholesale refusal echoes as many zero DCIDs as the request carried Source CIDs
     * (n zero DCIDs, "2 to 10 octets" for n in 1..5). When the request was too short to determine the
     * count (0x17 length < 8), the refusal carries 0 DCIDs - only the fixed fields, the minimum legal
     * 0x18 payload of chapter 4.26. n is capped for the 2*n DCID field in buff
     */
    if ((cfg == NULL) || (dcids == NULL) || (dcids->n > L2CAP_LE_EATT_MAX_CHANNEL) ||
        ((dcids->cids == NULL) && (dcids->n > 0))) {
        return BT_BAD_PARAM;
    }

    signal.code = L2CAP_CREDIT_BASED_CONNECTION_RESPONSE;
    signal.identifier = ident;
    signal.length = L2CAP_LE_EATT_CONN_FIXED_LEN + L2CAP_LE_EATT_CID_LEN * dcids->n;

    L2capCpuToLe16(buff + 0, cfg->mtu);
    L2capCpuToLe16(buff + L2CAP_OFFSET_2, cfg->mps);
    L2capCpuToLe16(buff + L2CAP_OFFSET_4, cfg->credit);
    L2capCpuToLe16(buff + L2CAP_OFFSET_6, result);
    for (uint8_t i = 0; i < dcids->n; i++) {
        L2capCpuToLe16(buff + L2CAP_OFFSET_8 + i * L2CAP_LE_EATT_CID_LEN, dcids->cids[i]);
    }

    pkt = L2capBuildSignalPacket(L2CAP_LE_SIGNALING_CHANNEL, &signal, buff);
    if (pkt == NULL) {
        return BT_NO_MEMORY;
    }
    return L2capLeSendPacket(conn->aclHandle, pkt);
}

// inbound 0x17 responder: refuse the whole 0x17 batch with a result code of table 4.21. When the
// request carried a countable Source CID list (n in 1..5), the 0x18 echoes n zero Destination CIDs
// ("2 to 10 octets", chapter 4.26); when the request was too short to determine the count (0x17
// length < 8), the 0x18 carries only the fixed fields, i.e. 0 DCIDs - the minimum legal 0x18
// payload of chapter 4.26
static void L2capLeEattRejectConnReq(L2capLeConnection *conn, uint8_t identifier, uint16_t result, uint8_t n)
{
    L2capLeConfigInfo noConfig = { 0 };
    uint16_t noDcid[L2CAP_LE_EATT_MAX_CHANNEL] = { 0 };
    L2capLeEattCidList dcidList = { 0 };

    if (n > L2CAP_LE_EATT_MAX_CHANNEL) {
        n = L2CAP_LE_EATT_MAX_CHANNEL;
    }
    dcidList.cids = noDcid;
    dcidList.n = n;
    (void)L2capLeEattSendConnectionRsp(conn, identifier, &noConfig, result, &dcidList);
    return;
}

// validate the request and the EATT state, returning the result code of table 4.21 (0 means accepted);
// fills ctx->n / ctx->cfgRemote / ctx->psm on success, chapter 4.25 and table 4.21
static uint16_t L2capLeEattValidateConnReq(L2capLeEattConnReqContext *ctx)
{
    if (ctx->length < L2CAP_SIZE_8) {
        return L2CAP_LE_INVALID_PARAMETERS;
    }
    /* chapter 4.25: the Source CID list is 2 octets per CID, an odd length is invalid */
    if (((ctx->length - L2CAP_SIZE_8) % 2) != 0) {
        return L2CAP_LE_INVALID_PARAMETERS;
    }
    ctx->n = (ctx->length - L2CAP_SIZE_8) / L2CAP_LE_EATT_CID_LEN;
    if ((ctx->n < 1) || (ctx->n > L2CAP_LE_EATT_MAX_CHANNEL)) {
        return L2CAP_LE_INVALID_PARAMETERS;
    }
    /* chapter 4.25: at most five channels per 0x17; capping per connection at five is local policy
     * (Vol 3 Part G 5.3 allows multiple EATT channels), refuse a batch that would exceed it -
     * mirror of the initiator-side cap in L2capLeEattGetOrNewConnection, else repeated peer
     * batches could accumulate unbounded channels on one connection */
    if (L2capLeCountEattChannels(ctx->conn) + ctx->n > L2CAP_LE_EATT_MAX_CHANNEL) {
        return L2CAP_LE_NO_RESOURCES_AVAILABLE;
    }

    /*
     * chapter 4.25: the SPSM field shall identify the EATT service; a request for another service is
     * refused with 'SPSM not supported' (0x0002), table 4.21
     */
    if (L2capLe16ToCpu(ctx->data + 0) != L2CAP_LE_EATT_PSM) {
        return L2CAP_LE_PSM_NOT_SUPPORTED;
    }

    ctx->cfgRemote.mtu = L2capLe16ToCpu(ctx->data + L2CAP_OFFSET_2);
    ctx->cfgRemote.mps = L2capLe16ToCpu(ctx->data + L2CAP_OFFSET_4);
    ctx->cfgRemote.credit = L2capLe16ToCpu(ctx->data + L2CAP_OFFSET_6);

    /*
     * chapter 4.25: the peer MTU/MPS shall be in the valid ECRED range (MTU 64..65535, MPS 64..65533)
     * and the peer Initial Credits shall be in the range of 1 to 65535, the peer rejects otherwise
     */
    if ((ctx->cfgRemote.mtu < L2CAP_LE_EATT_MIN_MTU) || (ctx->cfgRemote.mps < L2CAP_LE_EATT_MIN_MPS) ||
        (ctx->cfgRemote.mps > L2CAP_LE_EATT_MAX_MPS) || (ctx->cfgRemote.credit == 0)) {
        return L2CAP_LE_INVALID_PARAMETERS;
    }

    ctx->psm = L2capLeGetPsm(L2CAP_LE_EATT_PSM);
    if (ctx->psm == NULL) {
        return L2CAP_LE_PSM_NOT_SUPPORTED;
    }

    /* Vol 3 Part G 5.4: collision detection, an inbound 0x17 while an EATT request is already in flight */
    if (L2capLeEattHasPendingRequest(ctx->conn)) {
        L2capLeEattMarkPendingCollision(ctx->conn);
        return L2CAP_LE_NO_RESOURCES_AVAILABLE;
    }

    // security check, result codes of table 4.21
    return L2capLeEattCheckSecurity(ctx->conn, ctx->psm);
}

// establish one channel per valid Source CID, partial results on individual failures: an invalid or
// already allocated SCID (0x0009/0x000A) or a channel allocation failure (0x0004) skips that channel
static void L2capLeEattEstablishReqChannels(L2capLeEattConnReqContext *ctx)
{
    L2capLeChannel *chan = NULL;
    uint16_t scid = 0;
    uint8_t i;

    for (i = 0; i < ctx->n; i++) {
        scid = L2capLe16ToCpu(ctx->data + L2CAP_OFFSET_8 + i * L2CAP_LE_EATT_CID_LEN);
        if ((scid < L2CAP_LE_MIN_CID) || (scid > L2CAP_LE_MAX_CID)) {
            ctx->result = L2CAP_LE_INVALID_SOURCE_CID;
            ctx->dcids[i] = 0;
            continue;
        }
        if (L2capLeEattGetChannelByRcid(ctx->conn, scid) != NULL) {
            ctx->result = L2CAP_LE_SOURCE_CID_ALREADY_ALLOCATED;
            ctx->dcids[i] = 0;
            continue;
        }

        chan = L2capLeNewChannel(ctx->conn, L2CAP_LE_EATT_PSM, L2CAP_LE_EATT_PSM);
        if (chan == NULL) {
            ctx->result = L2CAP_LE_NO_RESOURCES_AVAILABLE;
            ctx->dcids[i] = 0;
            continue;
        }

        chan->rcid = scid;
        // the responder never correlates an established channel by the request identifier; the field stays
        // 0 so a reused identifier (or the old 0x15 matching) can not see this channel
        chan->rcfg.mtu = ctx->cfgRemote.mtu;
        chan->rcfg.mps = ctx->cfgRemote.mps;
        chan->rcfg.credit = ctx->cfgRemote.credit;
        if (chan->rcfg.mps > (L2capLeGetTxBufferSize() - L2CAP_SIZE_6)) {
            chan->rcfg.mps = L2capLeGetTxBufferSize() - L2CAP_SIZE_6;
        }
        chan->lcfg = ctx->cfgLocal;
        // peerCredits must track the granted receive credit, read by the zero-credit gate
        chan->peerCredits = chan->lcfg.credit;
        chan->state = L2CAP_CHANNEL_CONNECTED;
        ctx->dcids[i] = chan->lcid;
        ctx->chans[ctx->chanCount++] = chan;
    }
    return;
}

// send the connection response with the local config and the assigned DCIDs, then notify ATT per
// established channel, chapter 4.25/4.26
static void L2capLeEattSendReqResponse(L2capLeEattConnReqContext *ctx)
{
    L2capLeEattCidList dcidList = { 0 };
    uint8_t i;

    dcidList.cids = ctx->dcids;
    dcidList.n = ctx->n;

    /* chapter 4.26: the DCIDs correspond to the Source CIDs in order, 0x0000 means not established */
    int sendResult = L2capLeEattSendConnectionRsp(ctx->conn, ctx->identifier, &ctx->cfgLocal, ctx->result, &dcidList);
    if (sendResult != BT_SUCCESS) {
        // The peer never sees this 0x18: its 0x17 times out and only discards its own channels
        // (L2capLeEattDiscardTimedOutBatch, removeAcl = 0), it does not tear down the ACL, so the
        // channels established above would stay CONNECTED forever and burn the ECRED channel
        // budget. Roll the batch back like the 0x17 send failure (L2capLeEattSendPendingRequest)
        // and the timeout cleanup (L2capLeEattDiscardTimedOutBatch): abnormal disconnect per
        // channel, then delete; ATT never saw recvLeEattConnected for them.
        for (i = 0; i < ctx->chanCount; i++) {
            if (ctx->psm->service.leDisconnectAbnormal != NULL) {
                LOG_DEBUG(
                    "L2capCallback leDisconnectAbnormal:%{public}d begin, cid = 0x%04X, reason = 0", __LINE__,
                    ctx->chans[i]->lcid);
                ctx->psm->service.leDisconnectAbnormal(ctx->chans[i]->lcid, 0, ctx->psm->ctx);
                LOG_DEBUG("L2capCallback leDisconnectAbnormal:%{public}d end", __LINE__);
            }
            L2capLeDeleteChannel(ctx->conn, ctx->chans[i], 0);
        }
        LOG_WARN("L2capLeEattSendConnectionRsp failed, result = %{public}d, n = %hhu, channels rolled back",
            sendResult, dcidList.n);
        return;
    }

    // notify ATT per established channel with the peer config
    if (ctx->psm->service.recvLeEattConnected != NULL) {
        ctx->connInfo.handle = ctx->aclHandle;
        (void)memcpy_s(&(ctx->connInfo.addr), sizeof(BtAddr), &(ctx->conn->addr), sizeof(BtAddr));
        for (i = 0; i < ctx->chanCount; i++) {
            LOG_DEBUG(
                "L2capCallback recvLeEattConnected:%{public}d begin, cid = 0x%04X", __LINE__, ctx->chans[i]->lcid);
            ctx->psm->service.recvLeEattConnected(ctx->chans[i]->lcid, &ctx->connInfo, &ctx->cfgRemote, ctx->psm->ctx);
            LOG_DEBUG("L2capCallback recvLeEattConnected:%{public}d end", __LINE__);
        }
    }
    return;
}

void L2capLeEattProcessConnectionReq(uint16_t aclHandle, const L2capSignalHeader *signal, const uint8_t *data)
{
    L2capLeEattConnReqContext ctx = { 0 };
    uint16_t result;

    ctx.conn = L2capLeGetConnection(aclHandle);
    if (ctx.conn == NULL) {
        return;
    }
    ctx.aclHandle = aclHandle;
    ctx.data = data;
    ctx.length = signal->length;
    ctx.identifier = signal->identifier;

    /* chapter 4.25: the whole batch is refused on a validation failure, table 4.21 */
    result = L2capLeEattValidateConnReq(&ctx);
    if (result != 0) {
        L2capLeEattRejectConnReq(ctx.conn, ctx.identifier, result, ctx.n);
        return;
    }

    // local config for the responder: per-PSM default or ECRED minimums
    ctx.cfgLocal.mtu = (ctx.psm->lcfg.mtu != 0) ? ctx.psm->lcfg.mtu : L2CAP_LE_EATT_MIN_MTU;
    ctx.cfgLocal.mps = (ctx.psm->lcfg.mps != 0) ? ctx.psm->lcfg.mps : (L2capGetRxBufferSize() - L2CAP_SIZE_6);
    ctx.cfgLocal.credit = (ctx.psm->lcfg.credit != 0) ? ctx.psm->lcfg.credit : L2CAP_LE_DEFAULT_CREDIT;

    L2capLeEattEstablishReqChannels(&ctx);
    L2capLeEattSendReqResponse(&ctx);
    return;
}

// collect the pending EATT channels of this 0x17 request, matched by identifier
static void L2capLeEattCollectChannels(L2capLeEattConnRspContext *ctx, uint8_t identifier)
{
    ListNode *node = NULL;
    L2capLeChannel *chan = NULL;

    node = ListGetFirstNode(ctx->conn->chanList);
    while ((node != NULL) && (ctx->chanCount < L2CAP_LE_EATT_MAX_CHANNEL)) {
        chan = ListGetNodeData(node);
        // lpsm filter: only EATT channels are ever matched by a 0x17 identifier, a legacy 0x14 channel
        // that reused the value must not be collected (L2capLeEattGetPendingChannels filters the same way)
        if ((chan->state == L2CAP_CHANNEL_CONNECT_OUT_REQ) && (chan->connIdentifier == identifier) &&
            (chan->lpsm == L2CAP_LE_EATT_PSM)) {
            ctx->chans[ctx->chanCount++] = chan;
        }
        node = ListGetNextNode(node);
    }
    return;
}

// establishment is permitted under a successful result or a partial result ("some connections
// refused", 0x0004/0x0009/0x000A, Table 4.21); an "all connections refused" result carries no
// valid Destination CID and must not establish anything. The responder MTU/MPS/Credits shall be
// valid ECRED ranges (MTU 64..65535, MPS 64..65533, Initial Credits 1..65535), chapter 4.26;
// here an out-of-range batch is refused wholesale (reported as 0x000C invalid parameters) so that
// no channel is left with a degenerate receive config, chapter 4.26
static void L2capLeEattValidateRspConfig(L2capLeEattConnRspContext *ctx)
{
    if ((ctx->result == L2CAP_LE_CONNECTION_SUCCESSFUL) || (ctx->result == L2CAP_LE_NO_RESOURCES_AVAILABLE) ||
        (ctx->result == L2CAP_LE_INVALID_SOURCE_CID) || (ctx->result == L2CAP_LE_SOURCE_CID_ALREADY_ALLOCATED)) {
        if ((ctx->cfg.mtu >= L2CAP_LE_EATT_MIN_MTU) && (ctx->cfg.mps >= L2CAP_LE_EATT_MIN_MPS) &&
            (ctx->cfg.mps <= L2CAP_LE_EATT_MAX_MPS) && (ctx->cfg.credit != 0)) {
            ctx->cfgValid = 1;
        } else {
            ctx->result = L2CAP_LE_INVALID_PARAMETERS;
        }
    }
    return;
}

// Disconnect a previously established channel whose DCID collides with a DCID of a new
// batch, chapter 4.26. On a send failure the 0x06 never left the stack (no pending entry
// / send failure): without a pending entry there is no RTX either, so the channel would
// stay stuck in DISCONNECT_OUT_REQ forever; restore CONNECTED - both sides still agree
// the channel exists and a later normal disconnect flow can take over.
static void L2capLeEattDisconnectCollidingChannel(L2capLeEattConnRspContext *ctx, L2capLeChannel *orig)
{
    if (orig->state != L2CAP_CHANNEL_CONNECTED) {
        return;
    }
    orig->state = L2CAP_CHANNEL_DISCONNECT_OUT_REQ;
    if (L2capLeSendDisconnectionReq(ctx->conn, orig) == BT_SUCCESS) {
        return;
    }
    orig->state = L2CAP_CHANNEL_CONNECTED;
}

// establishment pass 1: mark the batch slots whose Destination CID is already assigned, chapter 4.26;
// a DCID that duplicates an earlier DCID of this batch, or that collides with a channel already holding
// this rcid, forces both the original channel and the new channel to be immediately discarded. Batch
// channels still carry rcid == 0 here (see L2capLeNewChannel), so the lookup only matches a pre-existing
// channel and never leaves a dangling ctx->chans reference
static void L2capLeEattRejectDuplicateDcid(L2capLeEattConnRspContext *ctx, uint8_t rejected[])
{
    L2capLeChannel *orig = NULL;
    uint16_t dcid = 0;
    uint8_t i;
    uint8_t k;

    for (i = 0; i < ctx->chanCount; i++) {
        dcid = (i < ctx->dcidCount) ? L2capLe16ToCpu(ctx->data + L2CAP_OFFSET_8 + i * L2CAP_LE_EATT_CID_LEN) : 0;
        if (dcid == 0) {
            continue;
        }

        // (a) same-batch duplicate DCID: both slots are discarded
        for (k = 0; k < i; k++) {
            if ((k < ctx->dcidCount) &&
                (L2capLe16ToCpu(ctx->data + L2CAP_OFFSET_8 + k * L2CAP_LE_EATT_CID_LEN) == dcid)) {
                rejected[i] = 1;
                rejected[k] = 1;
            }
        }

        // (b) the DCID is already assigned to a previously established channel: disconnect the original
        // channel and discard the new one
        orig = L2capLeEattGetChannelByRcid(ctx->conn, dcid);
        if (orig != NULL) {
            L2capLeEattDisconnectCollidingChannel(ctx, orig);
            rejected[i] = 1;
        }
    }
    return;
}

// write back the peer DCIDs in Source CID order, 0x0000 means not established, chapter 4.26;
// a channel is established when its DCID is non-zero and the responder batch config is valid;
// a partial result (0x0004/0x0009/0x000A, Table 4.21) establishes exactly the channels whose
// DCID is non-zero, a refused result must not leave any channel in CONNECTED state, chapter 4.26
static void L2capLeEattEstablishChannels(L2capLeEattConnRspContext *ctx)
{
    L2capLeChannel *chan = NULL;
    uint8_t rejected[L2CAP_LE_EATT_MAX_CHANNEL] = { 0 };
    uint16_t dcid = 0;
    uint8_t i;

    L2capLeEattRejectDuplicateDcid(ctx, rejected);

    /*
     * chapter 4.26: an out-of-range DCID (LE-U dynamic range, Table 2.1) is a protocol violation;
     * tear down the whole link
     */
    for (i = 0; i < ctx->dcidCount; i++) {
        dcid = L2capLe16ToCpu(ctx->data + L2CAP_OFFSET_8 + i * L2CAP_LE_EATT_CID_LEN);
        if ((dcid != 0) && ((dcid < L2CAP_LE_MIN_CID) || (dcid > L2CAP_LE_MAX_CID))) {
            // chapter 4.26: flag so the transaction-level 0x18 result is not reported for the torn-down link
            ctx->linkTornDown = 1;
            L2capDisconnect(ctx->conn->aclHandle, 0x13);
            return;
        }
    }

    for (i = 0; i < ctx->chanCount; i++) {
        if (rejected[i]) {
            continue;
        }
        chan = ctx->chans[i];
        dcid = (i < ctx->dcidCount) ? L2capLe16ToCpu(ctx->data + L2CAP_OFFSET_8 + i * L2CAP_LE_EATT_CID_LEN) : 0;
        if ((dcid != 0) && ctx->cfgValid) {
            chan->rcid = dcid;
            chan->rcfg.mtu = ctx->cfg.mtu;
            chan->rcfg.mps = ctx->cfg.mps;
            chan->rcfg.credit = ctx->cfg.credit;
            if (chan->rcfg.mps > (L2capLeGetTxBufferSize() - L2CAP_SIZE_6)) {
                chan->rcfg.mps = L2capLeGetTxBufferSize() - L2CAP_SIZE_6;
            }
            chan->state = L2CAP_CHANNEL_CONNECTED;
            chan->connIdentifier = 0; // established channels are no longer matched by the request identifier
            ctx->succeeded++;

            if ((ctx->psm != NULL) && (ctx->psm->service.recvLeEattConnected != NULL)) {
                LOG_DEBUG("L2capCallback recvLeEattConnected:%{public}d begin, cid = 0x%04X", __LINE__, chan->lcid);
                ctx->psm->service.recvLeEattConnected(chan->lcid, &ctx->connInfo, &ctx->cfg, ctx->psm->ctx);
                LOG_DEBUG("L2capCallback recvLeEattConnected:%{public}d end", __LINE__);
            }
        }
    }
    return;
}

// Vol 3 Part G 5.4: release the batch channels and the pending 0x17 entry so the retry is a fresh
// request with a new identifier; the caller decides when to send it (immediately or via the timer).
// The batch's per-entry collision marker dies with the pending entry; other in-flight batches keep
// their own markers (see L2capLeEattMarkPendingCollision)
static void L2capLeEattReleaseBatchForRetry(L2capLeEattConnRspContext *ctx, uint8_t identifier)
{
    uint8_t i;

    for (i = 0; i < ctx->chanCount; i++) {
        ctx->chans[i]->connIdentifier = 0;
    }
    L2capLeEattDestroyPendingByCode(ctx->conn, L2CAP_CREDIT_BASED_CONNECTION_REQUEST, identifier);
    return;
}

// Vol 3 Part G 5.4 collision retry: the master may retry immediately, the slave shall
// wait at least 100 ms before retrying, or 2 x (connSlaveLatency + 1) x connInterval if
// that is longer. On any arm failure (or a missing timer) the retry is sent immediately
// instead of leaving the batch hanging in CONNECT_OUT_REQ with nobody left to resolve it.
static void L2capLeEattScheduleCollisionRetry(L2capLeEattConnRspContext *ctx)
{
    if (ctx->conn->role == L2CAP_LE_ROLE_MASTER) {
        L2capLeEattSendPendingRequest(ctx->conn);
        return;
    }
    uint32_t delay = L2CAP_LE_EATT_RETRY_DELAY_MS;
    // a uint64 intermediate keeps extreme interval/latency values from wrapping
    // the product below the 100 ms floor (Vol 3 Part G 5.4)
    uint64_t intervalDelay64 =
        L2CAP_LE_EATT_SLAVE_RETRY_FACTOR * ((uint64_t)ctx->conn->connSlaveLatency + 1) *
        (uint64_t)ctx->conn->connIntervalUnits;
    uint32_t intervalDelay;
    // interval = connIntervalUnits x 1.25 ms, round up
    intervalDelay64 =
        (intervalDelay64 * L2CAP_LE_EATT_INTERVAL_MS_NUMERATOR + L2CAP_LE_EATT_INTERVAL_MS_ROUNDUP) /
        L2CAP_LE_EATT_INTERVAL_MS_DENOMINATOR;
    intervalDelay = (intervalDelay64 > UINT32_MAX) ? UINT32_MAX : (uint32_t)intervalDelay64;
    if (intervalDelay > delay) {
        delay = intervalDelay;
    }
    if (ctx->conn->eatt.retryTimer == NULL) {
        L2capLeEattSendPendingRequest(ctx->conn);
        return;
    }
    // arm with the numeric ACL handle, see L2capLeEattRetryProcess: a raw connection
    // pointer captured here could be re-issued to a newer connection by the allocator
    // after the original one is torn down, mis-delivering the stale retry
    if (AlarmSet(ctx->conn->eatt.retryTimer, delay, L2capLeEattRetryTimerCallback,
        (void *)(uintptr_t)ctx->conn->aclHandle) != 0) {
        // arm failure: retry immediately instead of leaving the batch hanging in
        // CONNECT_OUT_REQ with nobody left to resolve it (the pending entry's RTX
        // would time out later, but the immediate retry keeps the 5.4 timing)
        LOG_WARN("%{public}s: arm retry timer failed, retry immediately", __FUNCTION__);
        L2capLeEattSendPendingRequest(ctx->conn);
    }
}

// resolve the pending request; all-refused with collision triggers the retry of Vol 3 Part G 5.4,
// then notify ATT once with the transaction-level result, chapter 4.26
static void L2capLeEattResolvePendingRsp(L2capLeEattConnRspContext *ctx, uint8_t identifier)
{
    L2capPendingRequest *pend = NULL;
    uint8_t i;

    if (ctx->linkTornDown) {
        // chapter 4.26: link torn down by the DCID range check, skip cleanup and the pointless report
        return;
    }

    // The collision marker is per pending entry (see L2capLeEattMarkPendingCollision), so this batch
    // consumes only its own marker: an all-refused 0x18 retries when an inbound 0x17 was refused due
    // to collision while this very batch was in flight. The pending entry may already be gone (RTX
    // expiry won the race), treat the refusal as final then. The entry must be a 0x17 (an identifier
    // reused by another request type must not be matched, mirror of L2capLeEattDestroyPendingByCode)
    pend = L2capGetPendingRequest(ctx->conn->pendingList, identifier);
    if ((ctx->succeeded == 0) && (pend != NULL) && (pend->code == L2CAP_CREDIT_BASED_CONNECTION_REQUEST) &&
        pend->collision) {
        L2capLeEattReleaseBatchForRetry(ctx, identifier);
        L2capLeEattScheduleCollisionRetry(ctx);
    } else {
        L2capLeEattDestroyPendingByCode(ctx->conn, L2CAP_CREDIT_BASED_CONNECTION_REQUEST, identifier);
        for (i = 0; i < ctx->chanCount; i++) {
            if (ctx->chans[i]->state != L2CAP_CHANNEL_CONNECTED) {
                L2capLeDeleteChannel(ctx->conn, ctx->chans[i], 0);
            }
        }
    }

    // chapter 4.26 + Vol 3 Part G 5.4: transaction-level result, once per 0x18. attempted
    // (ctx->chanCount) is the channel count of the 0x18 as sent, i.e. one batch per pending
    // entry; when several L2CAP-level batches were coalesced into a single 0x17 (the ACL-down
    // deferred send of L2capLeAclConnectProcess, or a 5.4 collision retry recollecting all
    // pending channels), attempted counts all merged channels rather than any single caller's
    // batch size. The ATT layer compares succeeded == attempted against this merged total, so a
    // partial merged grant is reported as a failure even when a particular caller's own channels
    // were all established (conservative, per-channel grants are exact at this layer: only
    // established channels are kept).
    if ((ctx->psm != NULL) && (ctx->psm->service.recvLeEattConnectionRsp != NULL)) {
        LOG_DEBUG("L2capCallback recvLeEattConnectionRsp:%{public}d begin, result = %hu, attempted = %hhu, "
                  "succeeded = %hhu",
            __LINE__, ctx->result, ctx->chanCount, ctx->succeeded);
        ctx->psm->service.recvLeEattConnectionRsp(
            &ctx->connInfo, ctx->result, ctx->chanCount, ctx->succeeded, ctx->psm->ctx);
        LOG_DEBUG("L2capCallback recvLeEattConnectionRsp:%{public}d end", __LINE__);
    }
    return;
}

// malformed 0x18 (length < 8, no MTU/MPS/Credits/Result): discard the batch channels of this request and
// report the transaction as failed, otherwise the channels stay CONNECT_OUT_REQ with no pending entry or
// RTX timer left to clean them up, chapter 4.26
static void L2capLeEattDiscardShortRsp(L2capLeEattConnRspContext *ctx, uint8_t identifier)
{
    L2capLeChannel *chan = NULL;
    L2capLePsm *psm = NULL;
    uint8_t i;

    ctx->psm = L2capLeGetPsm(L2CAP_LE_EATT_PSM);
    L2capLeEattCollectChannels(ctx, identifier);
    for (i = 0; i < ctx->chanCount; i++) {
        chan = ctx->chans[i];
        psm = L2capLeGetPsm(chan->lpsm);
        if ((psm != NULL) && (psm->service.leDisconnectAbnormal != NULL)) {
            LOG_DEBUG(
                "L2capCallback leDisconnectAbnormal:%{public}d begin, cid = 0x%04X, reason = 0", __LINE__, chan->lcid);
            psm->service.leDisconnectAbnormal(chan->lcid, 0, psm->ctx);
            LOG_DEBUG("L2capCallback leDisconnectAbnormal:%{public}d end", __LINE__);
        }
        L2capLeDeleteChannel(ctx->conn, chan, 0);
    }
    if ((ctx->psm != NULL) && (ctx->psm->service.recvLeEattConnectionRsp != NULL)) {
        LOG_DEBUG("L2capCallback recvLeEattConnectionRsp:%{public}d begin, result = %hu, attempted = %hhu, "
                  "succeeded = %hhu",
            __LINE__, (uint16_t)L2CAP_LE_INVALID_PARAMETERS, (uint8_t)ctx->chanCount, (uint8_t)0);
        ctx->psm->service.recvLeEattConnectionRsp(
            &ctx->connInfo, L2CAP_LE_INVALID_PARAMETERS, ctx->chanCount, 0, ctx->psm->ctx);
        LOG_DEBUG("L2capCallback recvLeEattConnectionRsp:%{public}d end", __LINE__);
    }
    // only a pending 0x17 may be torn down here, an identifier reused by another request type must survive
    L2capLeEattDestroyPendingByCode(ctx->conn, L2CAP_CREDIT_BASED_CONNECTION_REQUEST, identifier);
    // Vol 3 Part G 5.4: the batch is destroyed without a retry, and its per-entry collision marker
    // dies with the pending entry just destroyed; other in-flight batches keep their own markers
    return;
}

// inbound 0x18 initiator
void L2capLeEattProcessConnectionRsp(uint16_t aclHandle, const L2capSignalHeader *signal, const uint8_t *data)
{
    L2capLeEattConnRspContext ctx = { 0 };

    ctx.conn = L2capLeGetConnection(aclHandle);
    if (ctx.conn == NULL) {
        return;
    }
    ctx.connInfo.handle = aclHandle;
    (void)memcpy_s(&(ctx.connInfo.addr), sizeof(BtAddr), &(ctx.conn->addr), sizeof(BtAddr));

    if (signal->length < L2CAP_SIZE_8) {
        L2capLeEattDiscardShortRsp(&ctx, signal->identifier);
        return;
    }
    // chapter 4.26: the DCID list is 2 octets per CID; an odd trailing length cannot be a valid 0x18
    ctx.dcidCount = (signal->length - L2CAP_SIZE_8) / L2CAP_LE_EATT_CID_LEN;
    if (((signal->length - L2CAP_SIZE_8) % L2CAP_LE_EATT_CID_LEN) != 0) {
        L2capLeEattDiscardShortRsp(&ctx, signal->identifier);
        return;
    }
    ctx.cfg.mtu = L2capLe16ToCpu(data + 0);
    ctx.cfg.mps = L2capLe16ToCpu(data + L2CAP_OFFSET_2);
    ctx.cfg.credit = L2capLe16ToCpu(data + L2CAP_OFFSET_4);
    ctx.result = L2capLe16ToCpu(data + L2CAP_OFFSET_6);

    L2capLeEattValidateRspConfig(&ctx);
    L2capLeEattCollectChannels(&ctx, signal->identifier);
    if (ctx.chanCount == 0) {
        // only a pending 0x17 may be torn down here, an identifier reused by another request type must survive
        L2capLeEattDestroyPendingByCode(ctx.conn, L2CAP_CREDIT_BASED_CONNECTION_REQUEST, signal->identifier);
        // Vol 3 Part G 5.4: no retry is arranged here (ResolvePendingRsp skipped), and the per-entry
        // collision marker dies with the pending entry just destroyed; other in-flight batches keep
        // their own markers
        return;
    }
    if (ctx.dcidCount > ctx.chanCount) {
        ctx.dcidCount = ctx.chanCount;
    }

    ctx.data = data;
    ctx.psm = L2capLeGetPsm(L2CAP_LE_EATT_PSM);

    L2capLeEattEstablishChannels(&ctx);
    L2capLeEattResolvePendingRsp(&ctx, signal->identifier);
    return;
}

// 0x19 L2CAP_CREDIT_BASED_RECONFIGURE_REQUEST, chapter 4.27
// Create the tracking entry of a reconfiguration request and append it to the reconfig
// list. On OOM the 0x19 must not be sent while the request is untracked - the 0x1A and
// the RTX both look the request up in eatt.reconfigList and would find nothing, so the
// reconfiguration would silently never complete and the req would leak.
static int L2capLeEattCreateReconfigReq(
    L2capLeConnection *conn, uint8_t identifier, uint16_t mtu, uint16_t mps, const L2capLeEattCidList *dcids)
{
    L2capLeReconfigReq *req = L2capAlloc(sizeof(L2capLeReconfigReq));
    if (req == NULL) {
        return BT_NO_MEMORY;
    }
    req->identifier = identifier;
    req->mtu = mtu;
    req->mps = mps;
    req->n = dcids->n;
    for (uint8_t i = 0; i < dcids->n; i++) {
        req->lcids[i] = dcids->cids[i];
    }
    if (!ListAddLast(conn->eatt.reconfigList, req)) {
        L2capFree(req);
        return BT_NO_MEMORY;
    }
    return BT_SUCCESS;
}

// Verify the pending entry actually landed before sending, see L2capLeEattSendConnectionReq:
// without an RTX pending entry the 0x19 would never resolve and the reconfig request would
// linger in eatt.reconfigList forever. The return value covers timer creation/arming failures
// too, the landing check covers a silent list append failure.
static int L2capLeEattSendReconfigPendingAndPacket(
    L2capLeConnection *conn, const L2capSignalHeader *signal, Packet *pkt)
{
    int pendResult = L2capCreatePendingRequest(
        conn->pendingList, 0, signal, L2CAP_DEFAULT_RTX, L2capLeResponseTimeoutCallback);
    if (pendResult != BT_SUCCESS) {
        L2capLeEattDestroyReconfig(conn, signal->identifier);
        PacketFree(pkt);
        return pendResult;
    }
    if (L2capGetPendingRequest(conn->pendingList, signal->identifier) == NULL) {
        L2capLeEattDestroyReconfig(conn, signal->identifier);
        PacketFree(pkt);
        return BT_NO_MEMORY;
    }
    int result = L2capLeSendPacket(conn->aclHandle, pkt);
    if (result != BT_SUCCESS) {
        // roll the request and its pending entry back on send failure so no reconfiguration is left
        // half-registered, mirroring the 0x17 rollback (chapter 4.27 has no flow-control credit to retry)
        L2capLeEattDestroyPendingByCode(conn, L2CAP_CREDIT_BASED_RECONFIGURE_REQUEST, signal->identifier);
        L2capLeEattDestroyReconfig(conn, signal->identifier);
    }
    return result;
}
int L2capLeEattSendReconfigureReq(
    L2capLeConnection *conn, uint16_t mtu, uint16_t mps, const L2capLeEattCidList *dcids)
{
    Packet *pkt = NULL;
    uint8_t buff[L2CAP_LE_EATT_RECONFIG_FIXED_LEN + L2CAP_LE_EATT_CID_LEN * L2CAP_LE_EATT_MAX_CHANNEL] = { 0 };
    L2capSignalHeader signal = { 0 };

    if ((dcids == NULL) || (dcids->cids == NULL) || (dcids->n < 1) || (dcids->n > L2CAP_LE_EATT_MAX_CHANNEL)) {
        return BT_BAD_PARAM;
    }

    signal.code = L2CAP_CREDIT_BASED_RECONFIGURE_REQUEST;
    signal.identifier = L2capLeGetNewIdentifier(conn);
    signal.length = L2CAP_LE_EATT_RECONFIG_FIXED_LEN + L2CAP_LE_EATT_CID_LEN * dcids->n;

    L2capCpuToLe16(buff + 0, mtu);
    L2capCpuToLe16(buff + L2CAP_OFFSET_2, mps);
    for (uint8_t i = 0; i < dcids->n; i++) {
        L2capCpuToLe16(buff + L2CAP_OFFSET_4 + i * L2CAP_LE_EATT_CID_LEN, dcids->cids[i]);
    }

    // build the packet before allocating the request entry so an allocation failure cannot leave an
    // orphaned reconfig request in the list or a pending entry behind
    pkt = L2capBuildSignalPacket(L2CAP_LE_SIGNALING_CHANNEL, &signal, buff);
    if (pkt == NULL) {
        return BT_NO_MEMORY;
    }

    int reqResult = L2capLeEattCreateReconfigReq(conn, signal.identifier, mtu, mps, dcids);
    if (reqResult != BT_SUCCESS) {
        PacketFree(pkt);
        return reqResult;
    }

    return L2capLeEattSendReconfigPendingAndPacket(conn, &signal, pkt);
}


// 0x1A L2CAP_CREDIT_BASED_RECONFIGURE_RESPONSE, chapter 4.28
static int L2capLeEattSendReconfigureRsp(L2capLeConnection *conn, uint8_t ident, uint16_t result)
{
    Packet *pkt = NULL;
    uint8_t buff[2] = { 0 };
    L2capSignalHeader signal = { 0 };

    signal.code = L2CAP_CREDIT_BASED_RECONFIGURE_RESPONSE;
    signal.identifier = ident;
    signal.length = sizeof(buff);

    L2capCpuToLe16(buff + 0, result);
    pkt = L2capBuildSignalPacket(L2CAP_LE_SIGNALING_CHANNEL, &signal, buff);
    if (pkt == NULL) {
        return BT_NO_MEMORY;
    }
    return L2capLeSendPacket(conn->aclHandle, pkt);
}

void L2capLeEattDestroyReconfig(L2capLeConnection *conn, uint8_t identifier)
{
    L2capLeReconfigReq *req = NULL;
    ListNode *node = NULL;

    if (conn->eatt.reconfigList == NULL) {
        return;
    }

    node = ListGetFirstNode(conn->eatt.reconfigList);
    while (node != NULL) {
        req = ListGetNodeData(node);
        if (req->identifier == identifier) {
            ListRemoveNode(conn->eatt.reconfigList, req);
            L2capFree(req);
            return;
        }
        node = ListGetNextNode(node);
    }
    return;
}

// inbound 0x19 responder
// validate the 0x19 request and return the result code of table 4.22 (0 means accepted); fills
// ctx->n / ctx->mtu / ctx->mps / ctx->chans on success, chapter 4.27
static uint16_t L2capLeEattValidateReconfigReq(L2capLeEattReconfigReqContext *ctx)
{
    uint16_t dcid;
    uint8_t i;

    if (ctx->length < L2CAP_SIZE_4) {
        return L2CAP_LE_RECONFIG_OTHER;
    }
    /* chapter 4.27: the Destination CID list is 2 octets per CID, an odd length is invalid */
    if (((ctx->length - L2CAP_SIZE_4) % 2) != 0) {
        return L2CAP_LE_RECONFIG_OTHER;
    }
    ctx->n = (ctx->length - L2CAP_SIZE_4) / L2CAP_LE_EATT_CID_LEN;
    if ((ctx->n < 1) || (ctx->n > L2CAP_LE_EATT_MAX_CHANNEL)) {
        return L2CAP_LE_RECONFIG_OTHER;
    }

    ctx->mtu = L2capLe16ToCpu(ctx->data + 0);
    ctx->mps = L2capLe16ToCpu(ctx->data + L2CAP_OFFSET_2);

    /* chapter 4.25: the peer MPS shall be in the valid ECRED range 64..65533 octets */
    if ((ctx->mps < L2CAP_LE_EATT_MIN_MPS) || (ctx->mps > L2CAP_LE_EATT_MAX_MPS)) {
        return L2CAP_LE_RECONFIG_OTHER;
    }
    if (ctx->mtu < L2CAP_LE_EATT_MIN_MTU) {
        return L2CAP_LE_RECONFIG_MTU_REDUCTION;
    }

    // collect and validate every Destination CID: the sender's local endpoints mapped by our rcid,
    // all of them established EATT channels (a legacy 0x14 channel's rcid is valid too, but it must
    // not be reconfigurable through the EATT PSM, mirroring L2capLeEattValidateReconfigureTargets)
    for (i = 0; i < ctx->n; i++) {
        dcid = L2capLe16ToCpu(ctx->data + L2CAP_OFFSET_4 + i * L2CAP_LE_EATT_CID_LEN);
        ctx->chans[i] = L2capLeEattGetChannelByRcid(ctx->conn, dcid);
        if ((ctx->chans[i] == NULL) || (ctx->chans[i]->state != L2CAP_CHANNEL_CONNECTED) ||
            (ctx->chans[i]->lpsm != L2CAP_LE_EATT_PSM)) {
            return L2CAP_LE_RECONFIG_INVALID_DCID;
        }
    }

    /* chapter 4.27: MTU shall not be smaller than the greatest current MTU; MPS with multiple channels */
    for (i = 0; i < ctx->n; i++) {
        if (ctx->mtu < ctx->chans[i]->rcfg.mtu) {
            return L2CAP_LE_RECONFIG_MTU_REDUCTION;
        }
        if ((ctx->n > 1) && (ctx->mps < ctx->chans[i]->rcfg.mps)) {
            return L2CAP_LE_RECONFIG_MPS_REDUCTION;
        }
    }
    return 0;
}

// apply the new peer receive config, respond and notify ATT per channel, chapter 4.27
static void L2capLeEattApplyReconfigReq(L2capLeEattReconfigReqContext *ctx)
{
    uint16_t effective = 0;
    uint16_t mps;
    uint8_t i;

    // chapter 4.27: the peer MPS is the maximum payload the peer accepts, clamp it to the stack tx
    // buffer like the connection paths (L2capLeEattEstablishReqChannels / L2capLeEattEstablishChannels)
    // so the local fragmenter (L2capLeSegmentPacketWithCredit) never emits a PDU larger than the
    // stack can send, otherwise a legal 0x19 raising the MPS to 65533 would stall the data path.
    // MPS is a maximum, sending smaller fragments than the peer allows stays protocol-valid, so the
    // clamped value is accepted silently with L2CAP_LE_RECONFIGURE_SUCCESS, mirroring the connection
    // paths which clamp without refusing; table 4.22 has no result code for this case either.
    mps = ctx->mps;
    if (mps > (L2capLeGetTxBufferSize() - L2CAP_SIZE_6)) {
        mps = L2capLeGetTxBufferSize() - L2CAP_SIZE_6;
    }

    for (i = 0; i < ctx->n; i++) {
        ctx->chans[i]->rcfg.mtu = ctx->mtu;
        ctx->chans[i]->rcfg.mps = mps;
    }
    (void)L2capLeEattSendReconfigureRsp(ctx->conn, ctx->identifier, L2CAP_LE_RECONFIGURE_SUCCESS);

    // Vol 3 Part G 5.3.1: notify ATT per channel with the new effective ATT_MTU
    ctx->psm = L2capLeGetPsm(L2CAP_LE_EATT_PSM);
    if ((ctx->psm != NULL) && (ctx->psm->service.recvLeEattReconfigured != NULL)) {
        for (i = 0; i < ctx->n; i++) {
            effective = (ctx->chans[i]->lcfg.mtu < ctx->mtu) ? ctx->chans[i]->lcfg.mtu : ctx->mtu;
            LOG_DEBUG("L2capCallback recvLeEattReconfigured:%{public}d begin, cid = 0x%04X, mtu = %hu, "
                      "result = %hu",
                __LINE__, ctx->chans[i]->lcid, effective, (uint16_t)L2CAP_LE_RECONFIGURE_SUCCESS);
            ctx->psm->service.recvLeEattReconfigured(
                ctx->chans[i]->lcid, effective, L2CAP_LE_RECONFIGURE_SUCCESS, ctx->psm->ctx);
            LOG_DEBUG("L2capCallback recvLeEattReconfigured:%{public}d end", __LINE__);
        }
    }
    return;
}

void L2capLeEattProcessReconfigureReq(uint16_t aclHandle, const L2capSignalHeader *signal, const uint8_t *data)
{
    L2capLeEattReconfigReqContext ctx = { 0 };
    uint16_t result;

    ctx.conn = L2capLeGetConnection(aclHandle);
    if (ctx.conn == NULL) {
        return;
    }
    ctx.data = data;
    ctx.length = signal->length;
    ctx.identifier = signal->identifier;

    // chapter 4.27: the whole request is refused on a validation failure, table 4.22
    result = L2capLeEattValidateReconfigReq(&ctx);
    if (result != 0) {
        (void)L2capLeEattSendReconfigureRsp(ctx.conn, ctx.identifier, result);
        return;
    }

    L2capLeEattApplyReconfigReq(&ctx);
    return;
}

// inbound 0x1A initiator
// find the in-flight 0x19 request by identifier, NULL when not found, chapter 4.28
static L2capLeReconfigReq *L2capLeEattFindReconfigReq(L2capLeConnection *conn, uint8_t identifier)
{
    ListNode *node = NULL;
    L2capLeReconfigReq *req = NULL;

    node = ListGetFirstNode(conn->eatt.reconfigList);
    while (node != NULL) {
        req = ListGetNodeData(node);
        if (req->identifier == identifier) {
            break;
        }
        req = NULL;
        node = ListGetNextNode(node);
    }
    return req;
}

// chapter 4.27: the sender's receive config takes effect on one channel. Clamp the local
// receive MPS to the stack PDU ceiling like the connection paths, the peer fragments by
// the advertised MPS and the receive checks of L2capLeProcessFirstSduFrame /
// L2capLeProcessSarContinuation compare against it.
static void L2capLeEattApplyReconfigChannel(L2capLeConnection *conn, const L2capLeReconfigReq *req, uint8_t idx)
{
    L2capLeChannel *chan = L2capLeGetChannel(conn, req->lcids[idx]);
    if (chan == NULL) {
        return;
    }
    chan->lcfg.mtu = req->mtu;
    chan->lcfg.mps = req->mps;
    if (chan->lcfg.mps > (L2capLeGetTxBufferSize() - L2CAP_SIZE_6)) {
        chan->lcfg.mps = L2capLeGetTxBufferSize() - L2CAP_SIZE_6;
    }
}

// Vol 3 Part G 5.3.1: notify ATT for one channel with the effective ATT_MTU (new on
// success, unchanged on failure) and the Table 4.22 result code, so a failed reconfigure
// is observable.
static void L2capLeEattNotifyReconfigured(const L2capLeEattReconfigRspContext *ctx, L2capLePsm *psm, uint8_t idx)
{
    L2capLeChannel *chan = L2capLeGetChannel(ctx->conn, ctx->req->lcids[idx]);
    uint16_t effective;
    if (chan == NULL) {
        return;
    }
    if (ctx->result == L2CAP_LE_RECONFIGURE_SUCCESS) {
        effective = (ctx->req->mtu < chan->rcfg.mtu) ? ctx->req->mtu : chan->rcfg.mtu;
    } else {
        // failure: the local receive config is unchanged, so the effective ATT_MTU stays the old value
        effective = (chan->lcfg.mtu < chan->rcfg.mtu) ? chan->lcfg.mtu : chan->rcfg.mtu;
    }
    LOG_DEBUG("L2capCallback recvLeEattReconfigured:%{public}d begin, cid = 0x%04X, mtu = %hu, "
              "result = %hu",
        __LINE__, chan->lcid, effective, ctx->result);
    psm->service.recvLeEattReconfigured(chan->lcid, effective, ctx->result, psm->ctx);
    LOG_DEBUG("L2capCallback recvLeEattReconfigured:%{public}d end", __LINE__);
}

// apply the sender's new receive config on success and notify ATT per channel, chapter 4.27;
// on failure the old config stays in effect but ATT is still notified with the unchanged effective
// ATT_MTU and the Table 4.22 result code
static void L2capLeEattApplyReconfigRsp(L2capLeEattReconfigRspContext *ctx)
{
    L2capLePsm *psm = NULL;
    uint8_t i;

    if (ctx->result == L2CAP_LE_RECONFIGURE_SUCCESS) {
        // chapter 4.27: the sender's receive config takes effect
        for (i = 0; i < ctx->req->n; i++) {
            L2capLeEattApplyReconfigChannel(ctx->conn, ctx->req, i);
        }
    }

    // Vol 3 Part G 5.3.1: notify ATT per channel with the effective ATT_MTU (new on success,
    // unchanged on failure) and the result code
    psm = L2capLeGetPsm(L2CAP_LE_EATT_PSM);
    if ((psm != NULL) && (psm->service.recvLeEattReconfigured != NULL)) {
        for (i = 0; i < ctx->req->n; i++) {
            L2capLeEattNotifyReconfigured(ctx, psm, i);
        }
    }
    return;
}

void L2capLeEattProcessReconfigureRsp(uint16_t aclHandle, const L2capSignalHeader *signal, const uint8_t *data)
{
    L2capLeEattReconfigRspContext ctx = { 0 };

    ctx.conn = L2capLeGetConnection(aclHandle);
    if (ctx.conn == NULL) {
        return;
    }

    // only a pending 0x19 may be torn down here, an identifier reused by another request type must survive
    L2capLeEattDestroyPendingByCode(ctx.conn, L2CAP_CREDIT_BASED_RECONFIGURE_REQUEST, signal->identifier);

    if (signal->length < L2CAP_SIZE_2) {
        L2capLeEattDestroyReconfig(ctx.conn, signal->identifier);
        return;
    }
    ctx.result = L2capLe16ToCpu(data + 0);

    // chapter 4.28: match the in-flight 0x19 request by the same signaling identifier
    ctx.req = L2capLeEattFindReconfigReq(ctx.conn, signal->identifier);
    if (ctx.req == NULL) {
        return;
    }

    ListRemoveNode(ctx.conn->eatt.reconfigList, ctx.req);

    L2capLeEattApplyReconfigRsp(&ctx);
    L2capFree(ctx.req);
    return;
}

// Test seam: deliver a 0x1A reconfigure response with the given result to the in-flight 0x19 request of a
// connection, driving the real 0x1A processing path. A live peer only answers a valid 0x19 with success
// (the initiator refuses MTU/MPS reductions locally before sending), so the failure branch of
// L2capLeEattApplyReconfigRsp cannot be reached in a two-device test; calling this from the L2CAP queue
// completes the reconfigure with the given Table 4.22 result as if the peer's 0x1A had arrived.
int L2capLeEattInjectReconfigureRsp(uint16_t aclHandle, uint16_t result)
{
    L2capLeConnection *conn = NULL;
    L2capLeReconfigReq *req = NULL;
    L2capPendingRequest *pend = NULL;
    ListNode *node = NULL;
    L2capSignalHeader signal = { 0 };
    uint8_t data[2] = { 0 };
    uint8_t ident = 0;

    conn = L2capLeGetConnection(aclHandle);
    if ((conn == NULL) || (conn->eatt.reconfigList == NULL)) {
        return BT_BAD_PARAM;
    }

    // Table 4.22 result codes are 0x0000-0x0004; reject anything outside the valid range
    if (result > L2CAP_LE_RECONFIG_OTHER) {
        return BT_BAD_PARAM;
    }

    // select the reconfig request of the in-flight 0x19 by its signal identifier instead of blindly
    // the first list node, so a concurrent 0x19 does not inject the 0x1A into the wrong request
    node = ListGetFirstNode(conn->pendingList);
    while (node != NULL) {
        pend = ListGetNodeData(node);
        if (pend->code == L2CAP_CREDIT_BASED_RECONFIGURE_REQUEST) {
            ident = pend->identifier;
            break;
        }
        node = ListGetNextNode(node);
    }
    if (ident == 0) {
        return BT_BAD_PARAM;
    }

    node = ListGetFirstNode(conn->eatt.reconfigList);
    while (node != NULL) {
        req = ListGetNodeData(node);
        if (req->identifier == ident) {
            break;
        }
        node = ListGetNextNode(node);
    }
    if (node == NULL) {
        return BT_BAD_PARAM;
    }

    signal.code = L2CAP_CREDIT_BASED_RECONFIGURE_RESPONSE;
    signal.identifier = req->identifier;
    signal.length = L2CAP_SIZE_2;
    L2capCpuToLe16(data, result);

    L2capLeEattProcessReconfigureRsp(aclHandle, &signal, data);
    return BT_SUCCESS;
}

// Test accessor: read back the current negotiated connection parameters that drive the Vol 3 Part G 5.4
// slave collision retry delay. The stored values are the requested ones captured at connect time,
// refreshed from the granted LE Connection Update Complete event (L2capLeRecvConnectionUpdateComplete).
int L2capLeEattGetConnectionParams(uint16_t aclHandle, uint16_t *intervalUnits, uint16_t *slaveLatency)
{
    L2capLeConnection *conn = NULL;

    if ((intervalUnits == NULL) || (slaveLatency == NULL)) {
        return BT_BAD_PARAM;
    }

    conn = L2capLeGetConnection(aclHandle);
    if (conn == NULL) {
        return BT_BAD_PARAM;
    }

    *intervalUnits = conn->connIntervalUnits;
    *slaveLatency = conn->connSlaveLatency;
    return BT_SUCCESS;
}

// validate the initiator parameters and the EATT registration, chapter 4.25
int L2capLeEattValidateConnParams(const L2capLeConfigInfo *cfg, uint16_t n)
{
    if (L2capLeInitialized() != BT_SUCCESS) {
        return BT_BAD_STATUS;
    }
    if ((n < 1) || (n > L2CAP_LE_EATT_MAX_CHANNEL)) {
        return BT_BAD_PARAM;
    }
    if ((cfg->mtu < L2CAP_LE_EATT_MIN_MTU) || (cfg->mps < L2CAP_LE_EATT_MIN_MPS)) {
        return BT_BAD_PARAM;
    }
    /* chapter 4.25: the MPS of an ECRED channel is at most 65533 octets */
    if (cfg->mps > L2CAP_LE_EATT_MAX_MPS) {
        return BT_BAD_PARAM;
    }
    // the EATT service shall be registered first, the 0x18 path needs its service callbacks
    if (L2capLeGetPsm(L2CAP_LE_EATT_PSM) == NULL) {
        return BT_BAD_PARAM;
    }
    return BT_SUCCESS;
}

// get the existing connection or create one for the batch, refusing when the channel limit would be
// exceeded, chapter 4.25
int L2capLeEattGetOrNewConnection(const BtAddr *addr, uint16_t n, L2capLeConnection **connOut)
{
    L2capLeConnection *conn = NULL;

    *connOut = NULL;
    conn = L2capLeGetConnection2(addr);
    if (conn != NULL) {
        /*
         * chapter 4.25: at most five channels per 0x17; capping per connection at five is local
         * policy (Vol 3 Part G 5.3 allows multiple EATT channels), refuse a batch that exceeds it
         */
        if (L2capLeCountEattChannels(conn) + n > L2CAP_LE_EATT_MAX_CHANNEL) {
            return BT_BAD_PARAM;
        }
    } else {
        conn = L2capLeNewConnection(addr, 0, 0);
        if (conn == NULL) {
            return BT_NO_MEMORY;
        }
    }
    *connOut = conn;
    return BT_SUCCESS;
}

// create one ECRED channel per local CID, rolling back this call's channels on failure, chapter 4.25
int L2capLeEattCreateReqChannels(
    L2capLeConnection *conn, const L2capLeConfigInfo *cfg, uint16_t lcids[], uint16_t n)
{
    L2capLeChannel *chan = NULL;
    uint16_t j;

    for (uint16_t i = 0; i < n; i++) {
        chan = L2capLeNewChannel(conn, L2CAP_LE_EATT_PSM, L2CAP_LE_EATT_PSM);
        if (chan == NULL) {
            // roll back the channels created in this call
            for (j = 0; j < i; j++) {
                L2capLeDeleteChannel(conn, L2capLeGetChannel(conn, lcids[j]), 0);
            }
            return BT_NO_MEMORY;
        }

        chan->state = L2CAP_CHANNEL_CONNECT_OUT_REQ;
        chan->lcfg.mtu = cfg->mtu;
        chan->lcfg.mps = cfg->mps;
        if (cfg->credit != 0) {
            chan->lcfg.credit = cfg->credit; // 0 keeps the default credit, same convention as the 0x14 LE CB path
        }
        // peerCredits must track the granted receive credit, read by the zero-credit gate
        chan->peerCredits = chan->lcfg.credit;
        lcids[i] = chan->lcid;
    }
    return BT_SUCCESS;
}

// While the ACL is still down, all pending batches are merged into a single 0x17 by
// L2capLeEattSendPendingRequest using only chans[0]->lcfg, so a second batch with a different
// config would be silently dropped; refuse it instead of losing the caller's config, chapter 4.25
int L2capLeEattCheckPendingBatchConfig(L2capLeConnection *conn, const L2capLeConfigInfo *cfg)
{
    L2capLeChannel *pending[L2CAP_LE_EATT_MAX_CHANNEL] = { 0 };
    uint8_t pendingN;
    uint16_t cfgCredit;

    if (conn->aclHandle != 0) {
        return BT_SUCCESS;
    }

    pendingN = L2capLeEattGetPendingChannels(conn, pending, L2CAP_LE_EATT_MAX_CHANNEL);
    if (pendingN == 0) {
        return BT_SUCCESS;
    }

    cfgCredit = (cfg->credit != 0) ? cfg->credit : L2CAP_LE_DEFAULT_CREDIT;
    if ((pending[0]->lcfg.mtu != cfg->mtu) || (pending[0]->lcfg.mps != cfg->mps) ||
        (pending[0]->lcfg.credit != cfgCredit)) {
        return BT_BAD_PARAM;
    }
    return BT_SUCCESS;
}

// Roll back the batch channels of this call when the ACL could not be started. The batch
// never left the stack: its channels are not yet marked with a request identifier (the
// mark happens at 0x17 send) and the ATT establishment was already resolved with this
// failure (AttEattEstablishSendResult), so keeping them would let L2capLeAclConnectProcess
// recollect and send them as a 0x17 once the ACL comes up - lcids half-open with nobody
// left to resolve them. Other channels (a legacy 0x14 batch, a concurrent EATT batch) and
// the connection itself are untouched.
static void L2capLeEattRollbackConnectBatch(L2capLeConnection *conn, const uint16_t lcids[], uint16_t n)
{
    for (uint16_t i = 0; i < n; i++) {
        L2capLeChannel *chan = L2capLeGetChannel(conn, lcids[i]);
        if (chan != NULL) {
            L2capLeDeleteChannel(conn, chan, 0);
        }
    }
    // Delete the connection only when this rollback left it without any channel at all: a
    // connection created by a legacy 0x14 L2CAP_LeCreditBasedConnectionReq (also pending
    // on the ACL, aclHandle == 0) hosts its own channels in chanList, and deleting the
    // connection would silently kill them while their caller still waits for a callback.
    // Mirror of the L2CAP_LeEattConnectionReq rollback pattern (l2cap_le.c: chanList
    // empty before delete).
    if (ListGetFirstNode(conn->chanList) == NULL) {
        L2capLeDeleteConnection(conn);
    }
}

// The ACL is not up yet: start it and defer the 0x17 to L2capLeAclConnectProcess; the ACL already
// exists: send the pending 0x17 now, chapter 4.25. lcids/n are the channels this call created
// (L2capLeEattCreateReqChannels), used to roll the batch back when the ACL cannot be started.
int L2capLeEattConnectOrSendPending(
    L2capLeConnection *conn, const BtAddr *addr, const uint16_t lcids[], uint16_t n)
{
    int result;

    if (conn->aclHandle == 0) {
        result = L2capConnectLe(addr);
        if (result != BT_SUCCESS) {
            L2capLeEattRollbackConnectBatch(conn, lcids, n);
            return result;
        }
        return BT_SUCCESS;
    }

    // chapter 4.25: surface the 0x17 send result, failed channels were rolled back
    return L2capLeEattSendPendingRequest(conn);
}
