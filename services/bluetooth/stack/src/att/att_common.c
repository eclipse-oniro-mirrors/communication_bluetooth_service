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
 * @file att_common.c
 *
 * @brief implement common function to be called.
 *
 */

#include "att_common.h"
#include "att_connect.h"

#include <stdlib.h>

#include "alarm.h"
#include "log.h"

#include "platform/include/allocator.h"

#include "btm/btm_thread.h"

static AttConnectInfo g_connectInfo[MAXCONNECT] = {0};
static AttConnectingInfo g_connecting[MAXCONNECT] = {0};
static AttClientDataCallback g_attClientCallback;
static AttServerDataCallback g_attServerCallback;
static AttClientDataCallbackCid g_attClientCallbackCid;
static AttServerDataCallbackCid g_attServerCallbackCid;
static AttClientSendDataCallback g_attClientSendDataCB;
static AttServerSendDataCallback g_attServerSendDataCB;
static AttConnectedCallback g_attConnect;
#define FUNCTIONLIST 256
recvDataFunction g_functionList[FUNCTIONLIST];

typedef struct LeRecvSendDataCallbackAsyncContext {
    uint16_t aclHandle;
    int result;
    // true when the completed send was the server indication (AttResponseSendData selected the
    // indication completion callback for it): the server response completion handlers clear the
    // pending-indication state only for an indication-send failure, never for a response or
    // error response (Vol 3 Part F 3.3.2, the completion callback carries no PDU identity).
    bool isIndication;
} LeRecvSendDataCallbackAsyncContext;

typedef struct LeEattSendDataCallbackAsyncContext {
    uint16_t lcid;
    int result;
    // see LeRecvSendDataCallbackAsyncContext.isIndication
    bool isIndication;
} LeEattSendDataCallbackAsyncContext;

typedef struct BREDRRecvSendDataCallbackAsyncContext {
    uint16_t lcid;
    int result;
    // see LeRecvSendDataCallbackAsyncContext.isIndication
    bool isIndication;
} BREDRRecvSendDataCallbackAsyncContext;

typedef struct TransactionTimeOutContext {
    uint16_t connectHandle;
    uint16_t lecid;   // 0 for BR/EDR; LE_CID for UATT; the EATT dynamic CID for an EATT bearer
    uint32_t seq;     // the monotonic arm sequence captured at arm time (see AttTransactionAlarmContext)
    Packet *headPacket;  // the in-flight request the alarm was armed for (see AttTransactionAlarmContext)
} TransactionTimeOutContext;

typedef struct AttRecvDataAsyncContext {
    uint16_t lcid;
    Packet *packet;
    void *ctx;
} AttRecvDataAsyncContext;

typedef struct AttRecvLeDataAsyncContext {
    uint16_t aclHandle;
    Packet *packet;
} AttRecvLeDataAsyncContext;

typedef struct AttConnectRegisterContext {
    AttConnectCallback connectBack;
    void *context;
} AttConnectRegisterContext;

static void AttTransactionTimeOut(const void *parameter);

static void AttIndicationTimeOut(const void *parameter);

static void AttDisconnectBearer(AttConnectInfo *connect, uint16_t connectHandle);

static void AttClientDataRegisterAsync(const void *context);
static void AttClientDataRegisterAsyncDestroy(const void *context);
static void AttClientDataDeregisterAsync(const void *context);
static void AttClientDataDeregisterAsyncDestroy(const void *context);

static void AttServerDataRegisterAsync(const void *context);
static void AttServerDataRegisterAsyncDestroy(const void *context);
static void AttServerDataDeregisterAsync(const void *context);
static void AttServerDataDeregisterAsyncDestroy(const void *context);

static void LeRecvSendDataCallbackAsync(const void *context);
static void LeRecvSendDataCallbackAsyncDestroy(const void *context);

static void AttIndicationTimeOutAsync(const void *context);
static void AttIndicationTimeOutAsyncDestroy(const void *context);

static void LeEattRecvSendDataCallbackAsync(const void *context);
static void LeEattSendDataCallbackAsyncDestroy(const void *context);

static void BREDRRecvSendDataCallbackAsync(const void *context);
static void BREDRRecvSendDataCallbackAsyncDestroy(const void *context);

static void AttTransactionTimeOutAsync(const void *context);
static void AttTransactionTimeOutAsyncDestroy(const void *context);

static recvDataFunction GetFunction(uint8_t opcode);

static void AttRecvDataAsync(const void *context);
static void AttRecvDataAsyncDestroy(const void *context);
static void AttRecvLeDataAsync(const void *context);
static void AttRecvLeDataAsyncDestroy(const void *context);

static void AttConnectRegisterAsync(const void *context);
static void AttConnectRegisterAsyncDestroy(const void *context);
static void AttConnectDeregisterAsync(const void *context);
static void AttConnectDeregisterAsyncDestroy(const void *context);

static void AttBREDRSendRespCallbackAsync(const void *context);
static void AttBREDRSendRespCallbackAsyncDestroy(const void *context);
static void AttBREDRSendRespCallback(uint16_t lcid, int result);
static void AttBREDRSendIndicationCallback(uint16_t lcid, int result);

static void AttLeSendRespCallbackAsync(const void *context);
static void AttLeSendRespCallbackAsyncDestroy(const void *context);
static void AttLeSendRespCallback(uint16_t aclHandle, int result);
static void AttLeSendIndicationCallback(uint16_t aclHandle, int result);

static void AttLeEattSendRespCallbackAsync(const void *context);
static void AttLeEattSendRespCallback(uint16_t lcid, int result);
static void AttLeEattSendIndicationCallback(uint16_t lcid, int result);

static void AttTransactionTimeOutAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    TransactionTimeOutContext *transTimeOutPtr = (TransactionTimeOutContext *)context;
    AttConnectInfo *connect = NULL;
    ListNode *node = NULL;
    Packet *headPacket = NULL;

    connect = AttGetConnectInfoByConnectHandleAndLeCid(transTimeOutPtr->connectHandle, transTimeOutPtr->lecid);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATTTRANSACTIONTIMEOUT_END", __FUNCTION__);
        goto ATTTRANSACTIONTIMEOUT_END;
    }

    // Stale expiry: the request this alarm was armed for is no longer at the head of the
    // instruction queue (it was answered and a newer request took the head, or the queue was
    // drained). The 30 s bound applies to the armed request only, so a stale expiry must not
    // report a timeout, drop a newer request's packet, or tear down a healthy bearer
    // (Vol 3 Part F 3.3.3). The monotonic arm sequence is the primary identity: every arm of a
    // newer request advances connect->transactionSeq, so an expiry of an earlier arm is
    // distinguishable even when the packet allocator reuses the freed head address (ABA). The
    // head-packet comparison is kept as an additional validation for the one case the sequence
    // cannot cover - the queue was answered and drained without a re-arm (no seq advance), but
    // the alarm firing raced the cancel: the head is then NULL or a different packet.
    node = ListGetFirstNode(connect->instruct);
    headPacket = (node != NULL) ? ListGetNodeData(node) : NULL;
    if ((transTimeOutPtr->seq != connect->transactionSeq) || (headPacket != transTimeOutPtr->headPacket)) {
        LOG_INFO("%{public}s stale transaction timeout, drop", __FUNCTION__);
        goto ATTTRANSACTIONTIMEOUT_END;
    }

    // Client-request timeout (Vol 3 Part F 3.3.3): report the in-flight request; the server-side
    // indication confirmation is tracked by its own alarm (AttIndicationTimeOut), not by this one.
    AttClientCallbackDispatch(connect, ATT_TRANSACTION_TIME_OUT_ID, NULL, NULL);

    // Drop the timed-out request, whose closure was just reported. The EATT channel teardown
    // (AttEattReleaseBearer) reports and drains the remaining requests; for UATT/BREDR the
    // whole ACL is torn down below (AttDisconnectBearer), which likewise reports the remaining
    // ones - removing the head here keeps every request reported exactly once on both paths
    // (review i10, AttDisconnectBearer reports whatever is still queued). The list entries are
    // the request packets owned by this queue, released by the list's free callback on
    // ListRemoveFirst.
    ListRemoveFirst(connect->instruct);

    // The teardown below also kills an unconfirmed indication pending on the same bearer (UATT
    // tears the whole ACL down; the EATT channel teardown reports it via AttEattReleaseBearer):
    // report its closure once so the server side is not left waiting for a confirmation that can
    // never arrive.
    if (connect->serverSendFlag) {
        AttServerCallbackDispatch(connect, ATT_TRANSACTION_TIME_OUT_ID, NULL, NULL);
        connect->serverSendFlag = false;
    }
    AttDisconnectBearer(connect, transTimeOutPtr->connectHandle);

ATTTRANSACTIONTIMEOUT_END:
    MEM_MALLOC.free(transTimeOutPtr);
    return;
}

static void AttTransactionTimeOutAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    TransactionTimeOutContext *transTimeOutPtr = (TransactionTimeOutContext *)context;

    MEM_MALLOC.free(transTimeOutPtr);

    return;
}

/**
 * @brief att transaction timeout.
 *
 * @param parameter Indicates the pointer to parameter.
 */
static void AttTransactionTimeOut(const void *parameter)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    // Runs on the alarm thread: only the immutable snapshot of the arm that fired
    // (AttTransactionAlarmContext in the alternated slots) is read, never the connection state,
    // which may have been cleared or reused by now; the expiry is re-validated against the
    // current slots on the ATT processing queue.
    AttTransactionAlarmContext *alarmCtx = (AttTransactionAlarmContext *)parameter;
    TransactionTimeOutContext *transTimeOutPtr = MEM_MALLOC.alloc(sizeof(TransactionTimeOutContext));
    if (transTimeOutPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    transTimeOutPtr->connectHandle = __atomic_load_n(&alarmCtx->connectHandle, __ATOMIC_RELAXED);
    transTimeOutPtr->lecid = __atomic_load_n(&alarmCtx->lecid, __ATOMIC_RELAXED);
    transTimeOutPtr->seq = __atomic_load_n(&alarmCtx->seq, __ATOMIC_RELAXED);
    transTimeOutPtr->headPacket = __atomic_load_n(&alarmCtx->headPacket, __ATOMIC_RELAXED);

    AttAsyncProcess(AttTransactionTimeOutAsync, AttTransactionTimeOutAsyncDestroy, transTimeOutPtr);

    return;
}

// Arm the client-request timeout: the alarm parameter is the next alternated immutable snapshot,
// never the connection state, so the alarm thread never dereferences the connection state and a
// stale expiry can never read a newer arm's values (Vol 3 Part F 3.3.3, ATT_ALARM_SNAPSHOT_SLOTS).
static void AttStartTransactionAlarm(AttConnectInfo *connect)
{
    ListNode *node = NULL;
    Packet *headPacket = NULL;
    AttTransactionAlarmContext *snap = NULL;

    // The timeout bounds the in-flight request, i.e. the instruct head at arm time; without one
    // there is nothing to bound.
    node = ListGetFirstNode(connect->instruct);
    headPacket = (node != NULL) ? ListGetNodeData(node) : NULL;
    if (headPacket == NULL) {
        return;
    }

    connect->transactionAlarmSlot ^= 1;
    snap = &connect->transactionAlarmCtx[connect->transactionAlarmSlot];
    // Relaxed atomics keep the alarm thread's reads defined; the slot alternation guarantees it
    // reads exactly the arm that fired, so no tearing or reuse can produce a stale match. The
    // monotonic sequence (advanced on the ATT queue, only ever reset by the full shutdown reset)
    // makes the arm identity unique even when the packet allocator reuses the freed head address
    // (ABA): a stale expiry compares this sequence instead of relying on the pointer alone.
    connect->transactionSeq++;
    __atomic_store_n(&snap->connectHandle, connect->retGattConnectHandle, __ATOMIC_RELAXED);
    __atomic_store_n(&snap->lecid, 0, __ATOMIC_RELAXED);
    if (connect->transportType == BT_TRANSPORT_LE) {
        // AttConnectID is a union: for LE the CID discriminates the bearer.
        __atomic_store_n(&snap->lecid, connect->AttConnectID.lecid, __ATOMIC_RELAXED);
    }
    __atomic_store_n(&snap->seq, connect->transactionSeq, __ATOMIC_RELAXED);
    __atomic_store_n(&snap->headPacket, headPacket, __ATOMIC_RELAXED);
    AlarmSet(connect->alarm, (uint64_t)INSTRUCTIONTIMEOUT, (void (*)(void *))AttTransactionTimeOut, snap);
    return;
}

static void AttIndicationTimeOut(const void *parameter)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    // Runs on the alarm thread: only the immutable snapshot of the arm that fired
    // (AttIndicationAlarmContext in the alternated slots) is read, never the connection state,
    // which may have been cleared or reused by now; the expiry is re-validated against the
    // current slots on the ATT processing queue.
    AttIndicationAlarmContext *alarmCtx = (AttIndicationAlarmContext *)parameter;
    AttIndicationAlarmContext *indicationTimeOutPtr = MEM_MALLOC.alloc(sizeof(AttIndicationAlarmContext));
    if (indicationTimeOutPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    indicationTimeOutPtr->connectHandle = __atomic_load_n(&alarmCtx->connectHandle, __ATOMIC_RELAXED);
    indicationTimeOutPtr->lecid = __atomic_load_n(&alarmCtx->lecid, __ATOMIC_RELAXED);
    indicationTimeOutPtr->generation = __atomic_load_n(&alarmCtx->generation, __ATOMIC_RELAXED);

    AttAsyncProcess(AttIndicationTimeOutAsync, AttIndicationTimeOutAsyncDestroy, indicationTimeOutPtr);

    return;
}

static void AttIndicationTimeOutAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttIndicationAlarmContext *indicationTimeOutPtr = (AttIndicationAlarmContext *)context;
    AttConnectInfo *connect = NULL;

    connect =
        AttGetConnectInfoByConnectHandleAndLeCid(indicationTimeOutPtr->connectHandle, indicationTimeOutPtr->lecid);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATTINDICATIONTIMEOUT_END", __FUNCTION__);
        goto ATTINDICATIONTIMEOUT_END;
    }

    // Stale alarm: the confirmation already arrived and cancelled this timer, so there is no
    // pending indication to close.
    if (!connect->serverSendFlag) {
        LOG_INFO("%{public}s no pending indication, goto ATTINDICATIONTIMEOUT_END", __FUNCTION__);
        goto ATTINDICATIONTIMEOUT_END;
    }

    // Stale alarm: a newer indication was armed after this one was confirmed or the slot reused;
    // the generation is never reset, so it also separates indications across connection
    // generations in a slot.
    if (connect->indicationGeneration != indicationTimeOutPtr->generation) {
        LOG_INFO("%{public}s stale indication timeout, goto ATTINDICATIONTIMEOUT_END", __FUNCTION__);
        goto ATTINDICATIONTIMEOUT_END;
    }

    // Indication-confirmation timeout (Vol 3 Part F 3.3.3): report the closure to the server once,
    // then drop the bearer; a new bearer is needed for further indications.
    AttServerCallbackDispatch(connect, ATT_TRANSACTION_TIME_OUT_ID, NULL, NULL);
    connect->serverSendFlag = false;

    AttDisconnectBearer(connect, indicationTimeOutPtr->connectHandle);

ATTINDICATIONTIMEOUT_END:
    MEM_MALLOC.free(indicationTimeOutPtr);
    return;
}

static void AttIndicationTimeOutAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttIndicationAlarmContext *indicationTimeOutPtr = (AttIndicationAlarmContext *)context;

    MEM_MALLOC.free(indicationTimeOutPtr);

    return;
}

void AttStartIndicationAlarm(AttConnectInfo *connect)
{
    AttIndicationAlarmContext *snap = NULL;

    // The indication transaction runs until the confirmation or the timeout (Vol 3 Part F 3.3.3),
    // armed per bearer so only that bearer is dropped at expiry. The alarm parameter is the next
    // alternated immutable snapshot, never the shared slot: the alarm thread must not
    // dereference the connection state (see ATT_ALARM_SNAPSHOT_SLOTS).
    connect->indicationAlarmSlot ^= 1;
    connect->indicationGeneration++;
    snap = &connect->indicationAlarmCtx[connect->indicationAlarmSlot];
    // Relaxed atomics keep the alarm thread's reads defined; the slot alternation guarantees it
    // reads exactly the arm that fired.
    __atomic_store_n(&snap->connectHandle, connect->retGattConnectHandle, __ATOMIC_RELAXED);
    __atomic_store_n(&snap->lecid, 0, __ATOMIC_RELAXED);
    if (connect->transportType == BT_TRANSPORT_LE) {
        // AttConnectID is a union: for LE the CID discriminates the bearer.
        __atomic_store_n(&snap->lecid, connect->AttConnectID.lecid, __ATOMIC_RELAXED);
    }
    __atomic_store_n(&snap->generation, connect->indicationGeneration, __ATOMIC_RELAXED);
    AlarmSet(connect->indicationAlarm, (uint64_t)INSTRUCTIONTIMEOUT, (void (*)(void *))AttIndicationTimeOut, snap);
    return;
}

/* Drop the timed-out bearer: an EATT channel only, or the whole ACL for UATT/BR/EDR
 * (Vol 3 Part F 3.3.3). The caller handles any per-request cleanup that precedes the drop. */
static void AttDisconnectBearer(AttConnectInfo *connect, uint16_t connectHandle)
{
    int listSize = 0;
    int drainSize = 0;

    if ((connect->transportType == BT_TRANSPORT_LE) && (connect->AttConnectID.lecid != LE_CID)) {
        // EATT: the channel teardown reports the queued requests (AttEattReleaseBearer).
        L2CIF_LeDisconnectionReq(connect->AttConnectID.lecid, NULL);
        return;
    }
    // UATT/BR/EDR: report every stranded request, drain the queue, then disconnect the whole
    // ACL (Part F 3.3.3). The count is captured once before the dispatch loop (same pattern as
    // AttEattReleaseBearer): a client callback can re-enter this teardown or enqueue a new
    // request, and the drain removes exactly the pre-callback in-flight number - packets
    // enqueued during the dispatch belong to a re-entered path that reports and drains them,
    // and anything still left is cleaned by the disconnect-completion drain in att_connect.c.
    // The list entries are the request packets owned by this queue, released by the list's
    // free callback on ListRemoveFirst (no extra PacketFree - that would double free).
    listSize = ListGetSize(connect->instruct);
    drainSize = listSize;
    for (; listSize > 0; --listSize) {
        AttClientCallbackDispatch(connect, ATT_TRANSACTION_TIME_OUT_ID, NULL, NULL);
    }
    for (; drainSize > 0; --drainSize) {
        ListRemoveFirst(connect->instruct);
    }
    InitiativeDisconnect(connectHandle);
    return;
}

/**
 * @brief get AttConnectInfo information.
 *
 * @return Returns the pointer to AttConnectInfo.
 */
AttConnectInfo *AttGetConnectStart()
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    return g_connectInfo;
}

/**
 * @brief lookup AttConnectInfo info by aclHandle.
 *
 * @param1 aclHandle Indicates the aclHandle.
 * @param2 connect Indicates the second rank pointer to AttConnectInfo.
 */
void AttGetConnectInfoIndexByAclHandle(uint16_t aclHandle, AttConnectInfo **connect)
{
    LOG_INFO("%{public}s enter, aclHandle = %hu", __FUNCTION__, aclHandle);

    uint16_t index = 0;

    for (; index < MAXCONNECT; ++index) {
        if (g_connectInfo[index].aclHandle == aclHandle) {
            break;
        }
    }

    if (index != MAXCONNECT) {
        *connect = &g_connectInfo[index];
    } else {
        *connect = NULL;
    }

    LOG_INFO("%{public}s return: index = %hu", __FUNCTION__, index);
    return;
}

/**
 * @brief lookup AttConnectInfo info by cid.
 *
 * @param1 cid Indicates the cid.
 * @param2 connect Indicates the second rank pointer to AttConnectInfo.
 */
void AttGetConnectInfoIndexByCid(uint16_t cid, AttConnectInfo **connect)
{
    LOG_INFO("%{public}s enter, cid = %hu", __FUNCTION__, cid);

    uint16_t index = 0;

    for (; index < MAXCONNECT; ++index) {
        if (g_connectInfo[index].transportType == BT_TRANSPORT_BR_EDR) {
            if (g_connectInfo[index].AttConnectID.bredrcid == cid) {
                break;
            }
        }
        if (g_connectInfo[index].transportType == BT_TRANSPORT_LE) {
            if (g_connectInfo[index].aclHandle == cid) {
                break;
            }
        }
    }

    if (index != MAXCONNECT) {
        *connect = &g_connectInfo[index];
    } else {
        *connect = NULL;
    }

    LOG_INFO("%{public}s return: index = %hu", __FUNCTION__, index);
    return;
}

/**
 * @brief lookup AttConnectInfo info by cid and output parameter index.
 *
 * @param1 cid Indicates the cid.
 * @param2 index Indicates the pointer to index.
 * @param3 connect Indicates second rank pointer to AttConnectInfo.
 */
void AttGetConnectInfoIndexByCidOutIndex(uint16_t cid, uint16_t *index, AttConnectInfo **connect)
{
    LOG_INFO("%{public}s enter,cid = %hu", __FUNCTION__, cid);

    uint16_t indexNumber = 0;

    for (; indexNumber < MAXCONNECT; ++indexNumber) {
        if (g_connectInfo[indexNumber].AttConnectID.bredrcid == cid) {
            break;
        }
    }

    *index = indexNumber;

    if (indexNumber != MAXCONNECT) {
        *connect = &g_connectInfo[indexNumber];
    } else {
        *connect = NULL;
    }

    LOG_INFO("%{public}s return: *index = %hu", __FUNCTION__, *index);
    return;
}

/**
 * @brief lookup AttConnectInfo info by connectHandle and output parameter index.
 *
 * @param1 connectHandle Indicates the connectHandle.
 * @param2 index Indicates the pointer to index.
 * @param3 connect Indicates the Secondary pointer to AttConnectInfo.
 */
void AttGetConnectInfoIndexByConnectHandle(uint16_t connectHandle, uint16_t *index, AttConnectInfo **connect)
{
    LOG_INFO("%{public}s enter, connectHandle = %hu", __FUNCTION__, connectHandle);

    uint16_t inindex = 0;

    for (; inindex < MAXCONNECT; ++inindex) {
        if (g_connectInfo[inindex].retGattConnectHandle == connectHandle) {
            break;
        }
    }

    *index = inindex;

    if (inindex != MAXCONNECT) {
        *connect = &g_connectInfo[inindex];

        goto ATTGETCONNECTINFOINDEXBYCONNECTHANDLE_END;
    } else {
        *connect = NULL;
        *index = MAXCONNECT;
        goto ATTGETCONNECTINFOINDEXBYCONNECTHANDLE_END;
    }

ATTGETCONNECTINFOINDEXBYCONNECTHANDLE_END:
    LOG_INFO("%{public}s return: *index = %hu", __FUNCTION__, *index);
    return;
}

/**
 * @brief lookup AttConnectInfo info by LE cid (LE fixed channel or EATT dynamic CID).
 *
 * @param1 lecid Indicates the LE cid.
 * @return Returns the matched AttConnectInfo pointer, or NULL when absent.
 */
AttConnectInfo *AttGetConnectInfoByLeCid(uint16_t lecid)
{
    AttConnectInfo *connect = NULL;
    uint16_t index = 0;

    LOG_INFO("%{public}s enter, lecid = %hu", __FUNCTION__, lecid);

    for (; index < MAXCONNECT; ++index) {
        if ((g_connectInfo[index].transportType == BT_TRANSPORT_LE) &&
            (g_connectInfo[index].AttConnectID.lecid == lecid)) {
            break;
        }
    }

    if (index != MAXCONNECT) {
        connect = &g_connectInfo[index];
    }

    LOG_INFO("%{public}s return: index = %hu", __FUNCTION__, index);
    return connect;
}

/**
 * @brief lookup AttConnectInfo info by ACL handle and LE cid.
 *
 * @param1 aclHandle Indicates the ACL handle.
 * @param2 lecid Indicates the LE cid.
 * @return Returns the matched AttConnectInfo pointer, or NULL when absent.
 */
AttConnectInfo *AttGetConnectInfoByAclHandleAndLeCid(uint16_t aclHandle, uint16_t lecid)
{
    AttConnectInfo *connect = NULL;
    uint16_t index = 0;

    LOG_INFO("%{public}s enter, aclHandle = %hu, lecid = %hu", __FUNCTION__, aclHandle, lecid);

    for (; index < MAXCONNECT; ++index) {
        if ((g_connectInfo[index].transportType == BT_TRANSPORT_LE) && (g_connectInfo[index].aclHandle == aclHandle) &&
            (g_connectInfo[index].AttConnectID.lecid == lecid)) {
            break;
        }
    }

    if (index != MAXCONNECT) {
        connect = &g_connectInfo[index];
    }

    LOG_INFO("%{public}s return: index = %hu", __FUNCTION__, index);
    return connect;
}

/**
 * @brief lookup AttConnectInfo info by connectHandle and LE cid.
 *
 * @param1 connectHandle Indicates the connectHandle.
 * @param2 lecid Indicates the LE cid.
 * @return Returns the matched AttConnectInfo pointer, or NULL when the connection is absent.
 */
AttConnectInfo *AttGetConnectInfoByConnectHandleAndLeCid(uint16_t connectHandle, uint16_t lecid)
{
    AttConnectInfo *connect = NULL;
    uint16_t index = 0;

    LOG_INFO("%{public}s enter, connectHandle = %hu, lecid = %hu", __FUNCTION__, connectHandle, lecid);

    if (lecid == 0) {
        // lecid == 0 pins the BR/EDR bearer: the cid union member has no lecid on BR/EDR,
        // and a BR/EDR connection holds exactly one ATT bearer, so a handle match is unambiguous.
        for (; index < MAXCONNECT; ++index) {
            if ((g_connectInfo[index].transportType == BT_TRANSPORT_BR_EDR) &&
                (g_connectInfo[index].retGattConnectHandle == connectHandle)) {
                break;
            }
        }
    } else {
        for (; index < MAXCONNECT; ++index) {
            if ((g_connectInfo[index].transportType == BT_TRANSPORT_LE) &&
                (g_connectInfo[index].retGattConnectHandle == connectHandle) &&
                (g_connectInfo[index].AttConnectID.lecid == lecid)) {
                break;
            }
        }
    }

    if (index != MAXCONNECT) {
        connect = &g_connectInfo[index];
    }

    LOG_INFO("%{public}s return: index = %hu", __FUNCTION__, index);
    return connect;
}

AttConnectInfo *AttGetConnectInfoByConnectHandlePreferEattRequestOrNtf(uint16_t connectHandle)
{
    AttConnectInfo *connect = NULL;
    uint16_t index = 0;

    LOG_INFO("%{public}s enter, connectHandle = %hu", __FUNCTION__, connectHandle);

    // Idle EATT bearer: no in-flight client request on it (one request per bearer, Vol 3 Part F
    // 3.3.2); commands (WRITE_CMD) and notifications impose no flow control, so they share the
    // same idle heuristic.
    for (; index < MAXCONNECT; ++index) {
        if ((g_connectInfo[index].transportType == BT_TRANSPORT_LE) &&
            (g_connectInfo[index].retGattConnectHandle == connectHandle) &&
            (g_connectInfo[index].AttConnectID.lecid != LE_CID) && (ListGetSize(g_connectInfo[index].instruct) == 0)) {
            break;
        }
    }

    if (index != MAXCONNECT) {
        connect = &g_connectInfo[index];
    } else {
        // No idle EATT channel: the LE connection carries the send on its UATT bearer (LE_CID);
        // a BR/EDR connection resolves on its single BR/EDR bearer (lecid == 0).
        connect = AttGetConnectInfoByConnectHandleAndLeCid(connectHandle, LE_CID);
        if (connect == NULL) {
            connect = AttGetConnectInfoByConnectHandleAndLeCid(connectHandle, 0);
        }
    }

    LOG_INFO("%{public}s return: index = %hu", __FUNCTION__, index);
    return connect;
}

AttConnectInfo *AttGetConnectInfoByConnectHandlePreferEattInd(uint16_t connectHandle)
{
    AttConnectInfo *connect = NULL;
    uint16_t index = 0;

    LOG_INFO("%{public}s enter, connectHandle = %hu", __FUNCTION__, connectHandle);

    // Idle EATT bearer: no outstanding indication awaiting its confirmation on it (one
    // indication per bearer until confirmed, Vol 3 Part F 3.3.2).
    for (; index < MAXCONNECT; ++index) {
        if ((g_connectInfo[index].transportType == BT_TRANSPORT_LE) &&
            (g_connectInfo[index].retGattConnectHandle == connectHandle) &&
            (g_connectInfo[index].AttConnectID.lecid != LE_CID) && (!g_connectInfo[index].serverSendFlag)) {
            break;
        }
    }

    if (index != MAXCONNECT) {
        connect = &g_connectInfo[index];
    } else {
        // No idle EATT channel: fall back to the UATT bearer (LE_CID), then to the BR/EDR
        // bearer (lecid == 0).
        connect = AttGetConnectInfoByConnectHandleAndLeCid(connectHandle, LE_CID);
        if (connect == NULL) {
            connect = AttGetConnectInfoByConnectHandleAndLeCid(connectHandle, 0);
        }
    }

    LOG_INFO("%{public}s return: index = %hu", __FUNCTION__, index);
    return connect;
}

AttConnectInfo *AttGetConnectInfoByConnectHandlePendingInd(uint16_t connectHandle)
{
    AttConnectInfo *connect = NULL;
    uint16_t index = 0;

    LOG_INFO("%{public}s enter, connectHandle = %hu", __FUNCTION__, connectHandle);

    // The legacy confirmation carries no bearer identity (cid == 0): prefer the bearer that
    // actually has an unconfirmed indication pending (serverSendFlag). A confirmation sent to
    // the first match (UATT) would never close the indication on the EATT bearer it was sent on
    // and the bearer would be torn down at the 3.3.3 timeout (Vol 3 Part F 3.3.2/3.3.3).
    for (; index < MAXCONNECT; ++index) {
        if ((g_connectInfo[index].retGattConnectHandle == connectHandle) && (g_connectInfo[index].serverSendFlag)) {
            break;
        }
    }

    if (index != MAXCONNECT) {
        connect = &g_connectInfo[index];
    }
    return connect;
}

/**
 * @brief gatt register client data to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttClientDataRegisterAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttClientDataCallback *attClientDataCallbackAsyncPtr = (AttClientDataCallback *)context;

    g_attClientCallback.attClientCallback = attClientDataCallbackAsyncPtr->attClientCallback;
    g_attClientCallback.context = attClientDataCallbackAsyncPtr->context;

    MEM_MALLOC.free(attClientDataCallbackAsyncPtr);

    return;
}

/**
 * @brief destroy gatt register client data to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttClientDataRegisterAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttClientDataCallback *attClientDataCallbackAsyncPtr = (AttClientDataCallback *)context;

    MEM_MALLOC.free(attClientDataCallbackAsyncPtr);

    return;
}

/**
 * @brief gatt register client data to att.
 *
 * @param1 dataCallback Indicates the pointer to callback.
 * @param2 context Indicates the pointer to context.
 */
void ATT_ClientDataRegister(attCallback dataCallback, void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttClientDataCallback *attClientDataCallbackAsyncPtr = MEM_MALLOC.alloc(sizeof(AttClientDataCallback));
    if (attClientDataCallbackAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    attClientDataCallbackAsyncPtr->attClientCallback = dataCallback;
    attClientDataCallbackAsyncPtr->context = context;

    AttAsyncProcess(AttClientDataRegisterAsync, AttClientDataRegisterAsyncDestroy, attClientDataCallbackAsyncPtr);

    return;
}

/**
 * @brief gatt deregister client data to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttClientDataDeregisterAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    g_attClientCallback.attClientCallback = NULL;
    g_attClientCallback.context = NULL;

    return;
}

/**
 * @brief destroy gatt deregister client data to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttClientDataDeregisterAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    return;
}

/**
 * @brief gatt deregister client data to att.
 *
 */
void ATT_ClientDataDeregister()
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttAsyncProcess(AttClientDataDeregisterAsync, AttClientDataDeregisterAsyncDestroy, NULL);

    return;
}

/**
 * @brief gatt register client data cid to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttClientDataRegisterCidAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttClientDataCallbackCid *attClientDataCallbackCidAsyncPtr = (AttClientDataCallbackCid *)context;

    g_attClientCallbackCid.attClientCallback = attClientDataCallbackCidAsyncPtr->attClientCallback;
    g_attClientCallbackCid.context = attClientDataCallbackCidAsyncPtr->context;

    MEM_MALLOC.free(attClientDataCallbackCidAsyncPtr);

    return;
}

/**
 * @brief destroy gatt register client data cid to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttClientDataRegisterCidAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttClientDataCallbackCid *attClientDataCallbackCidAsyncPtr = (AttClientDataCallbackCid *)context;

    MEM_MALLOC.free(attClientDataCallbackCidAsyncPtr);

    return;
}

/**
 * @brief gatt register client data cid to att.
 *
 * @param1 dataCallback Indicates the pointer to callback.
 * @param2 context Indicates the pointer to context.
 */
void ATT_ClientDataRegisterCid(AttCallbackCid dataCallback, void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttClientDataCallbackCid *attClientDataCallbackCidAsyncPtr = MEM_MALLOC.alloc(sizeof(AttClientDataCallbackCid));
    if (attClientDataCallbackCidAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    attClientDataCallbackCidAsyncPtr->attClientCallback = dataCallback;
    attClientDataCallbackCidAsyncPtr->context = context;

    AttAsyncProcess(
        AttClientDataRegisterCidAsync, AttClientDataRegisterCidAsyncDestroy, attClientDataCallbackCidAsyncPtr);

    return;
}

/**
 * @brief gatt deregister client data cid to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttClientDataDeregisterCidAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    g_attClientCallbackCid.attClientCallback = NULL;
    g_attClientCallbackCid.context = NULL;

    return;
}

/**
 * @brief destroy gatt deregister client data cid to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttClientDataDeregisterCidAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    return;
}

/**
 * @brief gatt deregister client data cid to att.
 *
 */
void ATT_ClientDataDeregisterCid()
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttAsyncProcess(AttClientDataDeregisterCidAsync, AttClientDataDeregisterCidAsyncDestroy, NULL);

    return;
}

/**
 * @brief gatt register server data to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttServerDataRegisterAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttServerDataCallback *attServerDataCallbackAsyncPtr = (AttServerDataCallback *)context;

    g_attServerCallback.attServerCallback = attServerDataCallbackAsyncPtr->attServerCallback;
    g_attServerCallback.context = attServerDataCallbackAsyncPtr->context;

    MEM_MALLOC.free(attServerDataCallbackAsyncPtr);

    return;
}

/**
 * @brief destroy gatt register server data to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttServerDataRegisterAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttServerDataCallback *attServerDataCallbackAsyncPtr = (AttServerDataCallback *)context;

    MEM_MALLOC.free(attServerDataCallbackAsyncPtr);

    return;
}

/**
 * @brief gatt register server data to att.
 *
 * @param1 dataCallback Indicates the pointer to callback.
 * @param2 context Indicates the pointer to context.
 */
void ATT_ServerDataRegister(attCallback dataCallback, void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttServerDataCallback *attServerDataCallbackAsyncPtr = MEM_MALLOC.alloc(sizeof(AttServerDataCallback));
    if (attServerDataCallbackAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    attServerDataCallbackAsyncPtr->attServerCallback = dataCallback;
    attServerDataCallbackAsyncPtr->context = context;

    AttAsyncProcess(AttServerDataRegisterAsync, AttServerDataRegisterAsyncDestroy, attServerDataCallbackAsyncPtr);

    return;
}

/**
 * @brief destroy gatt deregister server data to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttServerDataDeregisterAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    g_attServerCallback.attServerCallback = NULL;
    g_attServerCallback.context = NULL;

    return;
}

/**
 * @brief destroy gatt deregister server data to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttServerDataDeregisterAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    return;
}

/**
 * @brief gatt deregister server data to att.
 *
 */
void ATT_ServerDataDeregister()
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttAsyncProcess(AttServerDataDeregisterAsync, AttServerDataDeregisterAsyncDestroy, NULL);

    return;
}

/**
 * @brief gatt register server data cid to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttServerDataRegisterCidAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttServerDataCallbackCid *attServerDataCallbackCidAsyncPtr = (AttServerDataCallbackCid *)context;

    g_attServerCallbackCid.attServerCallback = attServerDataCallbackCidAsyncPtr->attServerCallback;
    g_attServerCallbackCid.context = attServerDataCallbackCidAsyncPtr->context;

    MEM_MALLOC.free(attServerDataCallbackCidAsyncPtr);

    return;
}

/**
 * @brief destroy gatt register server data cid to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttServerDataRegisterCidAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttServerDataCallbackCid *attServerDataCallbackCidAsyncPtr = (AttServerDataCallbackCid *)context;

    MEM_MALLOC.free(attServerDataCallbackCidAsyncPtr);

    return;
}

/**
 * @brief gatt register server data cid to att.
 *
 * @param1 dataCallback Indicates the pointer to callback.
 * @param2 context Indicates the pointer to context.
 */
void ATT_ServerDataRegisterCid(AttCallbackCid dataCallback, void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttServerDataCallbackCid *attServerDataCallbackCidAsyncPtr = MEM_MALLOC.alloc(sizeof(AttServerDataCallbackCid));
    if (attServerDataCallbackCidAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    attServerDataCallbackCidAsyncPtr->attServerCallback = dataCallback;
    attServerDataCallbackCidAsyncPtr->context = context;

    AttAsyncProcess(
        AttServerDataRegisterCidAsync, AttServerDataRegisterCidAsyncDestroy, attServerDataCallbackCidAsyncPtr);

    return;
}

/**
 * @brief gatt deregister server data cid to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttServerDataDeregisterCidAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    g_attServerCallbackCid.attServerCallback = NULL;
    g_attServerCallbackCid.context = NULL;

    return;
}

/**
 * @brief destroy gatt deregister server data cid to att in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttServerDataDeregisterCidAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    return;
}

/**
 * @brief gatt deregister server data cid to att.
 *
 */
void ATT_ServerDataDeregisterCid()
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttAsyncProcess(AttServerDataDeregisterCidAsync, AttServerDataDeregisterCidAsyncDestroy, NULL);

    return;
}

/**
 * @brief get AttClientDataCallback information.
 *
 * @return Returns the pointer to AttClientDataCallback.
 */
AttClientDataCallback *AttGetATTClientCallback()
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    return &g_attClientCallback;
}

/**
 * @brief get AttServerDataCallback information.
 *
 * @return Returns the pointer to AttServerDataCallback.
 */
AttServerDataCallback *AttGetATTServerCallback()
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    return &g_attServerCallback;
}

/**
 * @brief get AttClientDataCallbackCid information.
 *
 * @return Returns the pointer to AttClientDataCallbackCid.
 */
AttClientDataCallbackCid *AttGetATTClientCallbackCid()
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    return &g_attClientCallbackCid;
}

/**
 * @brief get AttServerDataCallbackCid information.
 *
 * @return Returns the pointer to AttServerDataCallbackCid.
 */
AttServerDataCallbackCid *AttGetATTServerCallbackCid()
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    return &g_attServerCallbackCid;
}

/**
 * @brief dispatch a received event to the registered data callbacks.
 *
 * @param1 connect Indicates the pointer to AttConnectInfo.
 * @param2 event Indicates the event id.
 * @param3 eventData Indicates the pointer to event data.
 * @param4 buffer Indicates the pointer to Buffer.
 */
void AttClientCallbackDispatch(AttConnectInfo *connect, uint16_t event, void *eventData, Buffer *buffer)
{
    uint16_t cid = 0;
    AttClientDataCallback *callback = NULL;
    AttClientDataCallbackCid *callbackCid = NULL;

    if (connect == NULL) {
        LOG_WARN("%{public}s connect == NULL", __FUNCTION__);
        return;
    }

    callback = AttGetATTClientCallback();
    callbackCid = AttGetATTClientCallbackCid();

    // AttConnectID is a union and cid 0 identifies the BR/EDR bearer; read lecid on LE only.
    if (connect->transportType == BT_TRANSPORT_LE) {
        cid = connect->AttConnectID.lecid;
    }

    if ((callbackCid != NULL) && (callbackCid->attClientCallback != NULL)) {
        callbackCid->attClientCallback(
            connect->retGattConnectHandle, cid, event, eventData, buffer, callbackCid->context);
    } else if ((callback != NULL) && (callback->attClientCallback != NULL)) {
        callback->attClientCallback(connect->retGattConnectHandle, event, eventData, buffer, callback->context);
    }

    return;
}

void AttServerCallbackDispatch(AttConnectInfo *connect, uint16_t event, void *eventData, Buffer *buffer)
{
    uint16_t cid = 0;
    AttServerDataCallback *callback = NULL;
    AttServerDataCallbackCid *callbackCid = NULL;

    if (connect == NULL) {
        LOG_WARN("%{public}s connect == NULL", __FUNCTION__);
        return;
    }

    callback = AttGetATTServerCallback();
    callbackCid = AttGetATTServerCallbackCid();

    // AttConnectID is a union and cid 0 identifies the BR/EDR bearer; read lecid on LE only.
    if (connect->transportType == BT_TRANSPORT_LE) {
        cid = connect->AttConnectID.lecid;
    }

    if ((callbackCid != NULL) && (callbackCid->attServerCallback != NULL)) {
        callbackCid->attServerCallback(
            connect->retGattConnectHandle, cid, event, eventData, buffer, callbackCid->context);
    } else if ((callback != NULL) && (callback->attServerCallback != NULL)) {
        // A legacy (cid-less) callback cannot name the EATT bearer a request arrived on, and the
        // legacy response APIs (cid 0) always resolve to the UATT bearer first, so an EATT
        // request dispatched as-is could never receive its response on the source bearer: the
        // response would leak onto the peer's UATT and be mis-consumed as the reply of an
        // unrelated UATT transaction, while the EATT request times out and tears the channel
        // down. Route EATT server events through the UATT connection instead: the legacy
        // callback and its responses then see one consistent UATT context, and the peer's EATT
        // request simply times out without corrupting any UATT transaction state.
        if ((connect->transportType == BT_TRANSPORT_LE) && (cid != LE_CID)) {
            AttConnectInfo *uattConnect = AttGetConnectInfoByAclHandleAndLeCid(connect->aclHandle, LE_CID);
            if (uattConnect == NULL) {
                LOG_WARN("%{public}s aclHandle:%hu: no UATT bearer to route legacy event to", __FUNCTION__,
                    connect->aclHandle);
                return;
            }
            connect = uattConnect;
        }
        callback->attServerCallback(connect->retGattConnectHandle, event, eventData, buffer, callback->context);
    }

    return;
}

/**
 * @brief initiative execut instructions by Scheduling.
 *
 * @param connect Indicates the pointer to AttConnectInfo.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
int AttSendSequenceScheduling(const AttConnectInfo *connect)
{
    LOG_INFO("%{public}s enter, listsize = %u", __FUNCTION__, ListGetSize(connect->instruct));

    int ret = BT_SUCCESS;

    if (ListGetSize(connect->instruct) == 1) {
        ListNode *listNodePtr = ListGetFirstNode(connect->instruct);
        if (listNodePtr == NULL) {
            LOG_INFO("%{public}s listNodePtr == NULL", __FUNCTION__);
            ret = BT_OPERATION_FAILED;
            goto ATTSENDSEQUENCESCHEDULING_END;
        }
        Packet *packet = ListGetNodeData(listNodePtr);
        if (connect->transportType == BT_TRANSPORT_LE) {
            if (connect->AttConnectID.lecid == LE_CID) {
                ret = L2CIF_LeSendFixChannelData(connect->aclHandle, (uint16_t)LE_CID, packet, LeRecvSendDataCallback);
            } else {
                // EATT: data rides the CoC channel, completion returns the bearer lcid
                ret = L2CIF_LeSendData(connect->AttConnectID.lecid, packet, LeEattRecvSendDataCallback);
            }
        }
        if (connect->transportType == BT_TRANSPORT_BR_EDR) {
            ret = L2CIF_SendData(connect->AttConnectID.bredrcid, packet, BREDRRecvSendDataCallback);
        }
        if (ret != BT_SUCCESS) {
            LOG_INFO("%{public}s call l2cap interface return not success", __FUNCTION__);
            // The packet never reached L2CAP, so its send-completion callback (which owns the
            // release on the success path) will never run: remove it here, the instruct list
            // owns its packets and releases them via its free callback (ListRemoveFirst, no
            // extra PacketFree - that would double free). Leaving it queued would wedge the
            // bearer - no response or timeout could ever retire it, and the EATT idle heuristic
            // (ListGetSize == 0) would mark the channel unusable forever. The failure is
            // reported to the upper layer by the caller (ClientCallbackReturnValue).
            ListRemoveFirst(connect->instruct);
        } else {
            AttStartTransactionAlarm((AttConnectInfo *)connect);
        }
    }

ATTSENDSEQUENCESCHEDULING_END:
    return ret;
}

/**
 * @brief execut instructions by Scheduling after receiving response.
 *
 * @param connect Indicates the pointer to AttConnectInfo.
 */
void AttReceiveSequenceScheduling(const AttConnectInfo *connect)
{
    LOG_INFO("%{public}s enter, listsize = %u, transportType = %hhu",
        __FUNCTION__,
        ListGetSize(connect->instruct),
        connect->transportType);

    int ret = BT_OPERATION_FAILED;

    // On a failed send keep draining the queue (mirror of AttSendSequenceScheduling's wedge
    // handling, per review m8): the packet never reached L2CAP, so its completion callback
    // (which owns the release on the success path) will never run - remove it here (the list's
    // free callback releases the packet, no extra PacketFree - that would double free), report
    // the failed send, and try the next request instead of leaving the bearer dead. A queued
    // packet no response or timeout could retire would also keep the EATT channel busy forever
    // (ListGetSize == 0 idle heuristic). The first success arms the transaction alarm and stops
    // the loop; the requests behind it wait for their own responses as before.
    while (ListGetSize(connect->instruct) > 0) {
        ListNode *listNodePtr = ListGetFirstNode(connect->instruct);
        if (listNodePtr == NULL) {
            LOG_INFO("%{public}s listNodePtr == NULL", __FUNCTION__);
            goto ATTRECEIVESEQUENCESCHEDULING_END;
        }
        Packet *PacketPtr = ListGetNodeData(listNodePtr);
        if (connect->transportType == BT_TRANSPORT_LE) {
            if (connect->AttConnectID.lecid == LE_CID) {
                ret =
                    L2CIF_LeSendFixChannelData(connect->aclHandle, (uint16_t)LE_CID, PacketPtr, LeRecvSendDataCallback);
            } else {
                // EATT: data rides the CoC channel, completion returns the bearer lcid
                ret = L2CIF_LeSendData(connect->AttConnectID.lecid, PacketPtr, LeEattRecvSendDataCallback);
            }
        }
        if (connect->transportType == BT_TRANSPORT_BR_EDR) {
            ret = L2CIF_SendData(connect->AttConnectID.bredrcid, PacketPtr, BREDRRecvSendDataCallback);
        }

        if (ret != BT_SUCCESS) {
            LOG_INFO("%{public}s call l2cap interface return not success", __FUNCTION__);
            ListRemoveFirst(connect->instruct);
            ClientCallbackReturnValue(ret, connect);
        } else {
            AttStartTransactionAlarm((AttConnectInfo *)connect);
            break;
        }
    }

ATTRECEIVESEQUENCESCHEDULING_END:
    return;
}

/**
 * @brief get AttConnectingInfo information.
 *
 * @return Returns the pointer to AttConnectingInfo.
 */
AttConnectingInfo *AttGetConnectingStart()
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    return g_connecting;
}

/**
 * @brief lookup AttConnectingInfo info by cid.
 *
 * @param1 cid Indicates the cid.
 * @param2 connecting Indicates the Secondary pointer to AttConnectingInfo.
 */
void AttGetConnectingIndexByCid(uint16_t cid, AttConnectingInfo **connecting)
{
    LOG_INFO("%{public}s enter, cid = %hu", __FUNCTION__, cid);

    uint16_t index = 0;

    for (; index < MAXCONNECT; ++index) {
        if (g_connecting[index].cid == cid) {
            break;
        }
    }

    if (index != MAXCONNECT) {
        *connecting = &g_connecting[index];
        goto ATTGETCONNECTINGINDEXBYCID_END;
    } else {
        *connecting = NULL;
        goto ATTGETCONNECTINGINDEXBYCID_END;
    }

ATTGETCONNECTINGINDEXBYCID_END:
    LOG_INFO("%{public}s return: index = %hu", __FUNCTION__, index);
    return;
}

/**
 * @brief lookup AttConnectingInfo info by cid and output parameter index.
 *
 * @param1 cid Indicates the cid.
 * @param2 index Indicates the pointer to index.
 * @param3 connecting Indicates the Secondary pointer to AttConnectingInfo.
 */
void AttGetConnectingIndexByCidOutIndex(uint16_t cid, uint16_t *index, AttConnectingInfo **connecting)
{
    LOG_INFO("%{public}s enter,cid = %hu", __FUNCTION__, cid);

    uint16_t indexNumber = 0;

    for (; indexNumber < MAXCONNECT; ++indexNumber) {
        if (g_connecting[indexNumber].cid == cid) {
            break;
        }
    }

    if (indexNumber != MAXCONNECT) {
        *connecting = &g_connecting[indexNumber];
        *index = indexNumber;
        goto ATTGETCONNECTINGINDEXBYCIDOUTINDEX_END;
    } else {
        *connecting = NULL;
        *index = MAXCONNECT;
        goto ATTGETCONNECTINGINDEXBYCIDOUTINDEX_END;
    }

ATTGETCONNECTINGINDEXBYCIDOUTINDEX_END:
    LOG_INFO("%{public}s return: *index = %hu", __FUNCTION__, *index);
    return;
}

/**
 * @brief lookup AttConnectingInfo info by connectHandle.
 *
 * @param1 connectHandle Indicates the connectHandle.
 * @param2 connecting Indicates the Secondary pointer to AttConnectingInfo.
 */
void AttGetConnectingIndexByConnectHandle(uint16_t connectHandle, AttConnectingInfo **connecting)
{
    LOG_INFO("%{public}s enter, connectHandle = %hu", __FUNCTION__, connectHandle);

    uint16_t index = 0;

    for (; index < MAXCONNECT; ++index) {
        if (g_connecting[index].connectHandle == connectHandle) {
            break;
        }
    }

    if (index != MAXCONNECT) {
        *connecting = &g_connecting[index];
        goto ATTGETCONNECTINGINDEXBYCONNECTHANDLE_END;
    } else {
        *connecting = NULL;
        goto ATTGETCONNECTINGINDEXBYCONNECTHANDLE_END;
    }

ATTGETCONNECTINGINDEXBYCONNECTHANDLE_END:
    LOG_INFO("%{public}s return: index = %hu", __FUNCTION__, index);
    return;
}

/**
 * @brief lookup AttConnectingInfo info by cid and connectHandle, result to output parameter index.
 *
 * @param1 cid Indicates the cid.
 * @param2 connectHandle Indicates the connectHandle.
 * @param3 index Indicates the pointer to index.
 * @param4 connecting Indicates the Secondary pointer to AttConnectingInfo.
 */
void AttGetConnectingIndexByCidConnectHandle(
    uint16_t cid, uint16_t connectHandle, uint16_t *index, AttConnectingInfo **connecting)
{
    LOG_INFO("%{public}s enter, cid = %hu,connectHandle = %hu", __FUNCTION__, cid, connectHandle);

    uint16_t inindex = 0;

    for (; inindex < MAXCONNECT; ++inindex) {
        if ((g_connecting[inindex].cid == cid) && (g_connecting[inindex].connectHandle == connectHandle)) {
            break;
        }
    }

    *index = inindex;

    if (inindex != MAXCONNECT) {
        *connecting = &g_connecting[inindex];
        goto ATTGETCONNECTINGINDEXBYCIDCONNECTHANDLE_END;
    } else {
        *connecting = NULL;
        goto ATTGETCONNECTINGINDEXBYCIDCONNECTHANDLE_END;
    }

ATTGETCONNECTINGINDEXBYCIDCONNECTHANDLE_END:
    LOG_INFO("%{public}s return: *index = %hu", __FUNCTION__, *index);
    return;
}

/**
 * @brief lookup AttConnectingInfo info by addr.
 *
 * @param1 addr Indicates the pointer to const BtAddr.
 * @param2 connecting Indicates the second rank pointer to AttConnectingInfo.
 */
void AttGetConnectingIndexByAddr(const BtAddr *addr, AttConnectingInfo **connecting)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    uint16_t index = 0;

    for (; index < MAXCONNECT; ++index) {
        if (!memcmp(g_connecting[index].addr.addr, addr->addr, ADDRESSLEN)) {
            break;
        }
    }

    if (index != MAXCONNECT) {
        *connecting = &g_connecting[index];
        goto ATTGETCONNECTINGINDEXBYADDR_END;
    } else {
        *connecting = NULL;
        goto ATTGETCONNECTINGINDEXBYADDR_END;
    }

ATTGETCONNECTINGINDEXBYADDR_END:
    LOG_INFO("%{public}s return: index = %hu", __FUNCTION__, index);
    return;
}

/**
 * @brief lookup AttConnectingInfo info by addr cid.
 *
 * @param1 addr Indicates the pointer to addr.
 * @param2 addr Indicates the cid.
 * @param3 connect Indicates the second rank pointer to AttConnectingInfo.
 */
void AttGetConnectingIndexByAddrUninitializedCid(const BtAddr *addr, AttConnectingInfo **connecting)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    uint16_t index = 0;

    for (; index < MAXCONNECT; ++index) {
        if ((!memcmp(g_connecting[index].addr.addr, addr->addr, ADDRESSLEN)) && (g_connecting[index].cid == 0)) {
            break;
        }
    }

    LOG_INFO("%{public}s,index = %hu", __FUNCTION__, index);

    if (index != MAXCONNECT) {
        *connecting = &g_connecting[index];
        goto ATTGETCONNECTINGINDEXBYADDRUNINITIALIZEDCID_END;
    } else {
        *connecting = NULL;
        goto ATTGETCONNECTINGINDEXBYADDRUNINITIALIZEDCID_END;
    }

ATTGETCONNECTINGINDEXBYADDRUNINITIALIZEDCID_END:
    LOG_INFO("%{public}s return: index = %hu", __FUNCTION__, index);
    return;
}

/**
 * @brief lookup AttConnectingInfo info by addr aclhandle cid.
 *
 * @param1 addr Indicates pointer to addr.
 * @param2 addr Indicates the aclHandle.
 * @param3 addr Indicates the cid.
 * @param2 connect Indicates the second rank pointer to AttConnectingInfo.
 */
void AttGetConnectingIndexByAddrAclhandleCid(
    const BtAddr *addr, uint16_t aclHandle, uint16_t cid, AttConnectingInfo **connecting)
{
    LOG_INFO("%{public}s enter, aclHandle = %hu, cid = %hu", __FUNCTION__, aclHandle, cid);
    uint16_t index = 0;

    for (; index < MAXCONNECT; ++index) {
        if ((!memcmp(g_connecting[index].addr.addr, addr->addr, ADDRESSLEN)) &&
            (g_connecting[index].aclHandle == aclHandle) && (g_connecting[index].cid == cid)) {
            break;
        }
    }

    if (index != MAXCONNECT) {
        *connecting = &g_connecting[index];
        goto ATTGETCONNECTINGINDEXBYADDRACLHANDLECID_END;
    } else {
        *connecting = NULL;
        goto ATTGETCONNECTINGINDEXBYADDRACLHANDLECID_END;
    }

ATTGETCONNECTINGINDEXBYADDRACLHANDLECID_END:
    LOG_INFO("%{public}s return: index = %hu", __FUNCTION__, index);
    return;
}

/**
 * @brief client call back copy.
 *
 * @param1 attSendDataCb Indicates the pointer to attSendDataCallback.
 * @param2 context Indicates the pointer to context.
 */
void AttClientCallBackCopyToCommon(attSendDataCallback attSendDataCB, const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    g_attClientSendDataCB.attSendDataCB = attSendDataCB;
    g_attClientSendDataCB.context = (void *)context;

    return;
}
/**
 * @brief server call back copy.
 *
 * @param1 attSendDataCb Indicates the pointer to attSendDataCallback.
 * @param2 context Indicates the pointer to context.
 */
void AttServerCallBackCopyToCommon(attSendDataCallback attSendDataCB, const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    g_attServerSendDataCB.attSendDataCB = attSendDataCB;
    g_attServerSendDataCB.context = (void *)context;

    return;
}

/**
 * @brief le receive senddata callback async.
 *
 * @param context Indicates the pointer to context.
 */
static void LeRecvSendDataCallbackAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    LeRecvSendDataCallbackAsyncContext *leRecvSendDataCallPtr = (LeRecvSendDataCallbackAsyncContext *)context;
    AttConnectInfo *connect = NULL;

    // Fixed-channel (0x04) data is UATT-only; EATT bearers share aclHandle, so pin lecid == LE_CID
    // to resolve the UATT bearer exactly (Vol 3 Part G 5.3; fixed channel 0x0004).
    connect = AttGetConnectInfoByAclHandleAndLeCid(leRecvSendDataCallPtr->aclHandle, LE_CID);
    if (connect == NULL) {
        goto RECVSENDDATACALLBACK_END;
    }

    if (g_attClientSendDataCB.attSendDataCB != NULL) {
        g_attClientSendDataCB.attSendDataCB(
            connect->retGattConnectHandle, leRecvSendDataCallPtr->result, g_attClientSendDataCB.context);
    }
    if (leRecvSendDataCallPtr->result == BT_SUCCESS) {
        AttStartTransactionAlarm(connect);
    } else {
        LOG_WARN("L2CAP error code = %{public}d", leRecvSendDataCallPtr->result);
        AlarmCancel(connect->alarm);
        // The request never reached the peer and its send-completion callback owns the release
        // on the success path: remove and free the stranded head request here and report a
        // transaction closure, or the bearer would be wedged forever - no response or timeout
        // could ever retire it, and the EATT idle heuristic (ListGetSize == 0) would mark the
        // channel unusable. Same handling as the scheduling failure paths
        // (AttSendSequenceScheduling); the instruct list owns its packets, ListRemoveFirst
        // releases them via the list free callback.
        if (ListRemoveFirst(connect->instruct)) {
            AttClientCallbackDispatch(connect, ATT_TRANSACTION_TIME_OUT_ID, NULL, NULL);
        }
    }

RECVSENDDATACALLBACK_END:
    MEM_MALLOC.free(leRecvSendDataCallPtr);
    return;
}

/**
 * @brief le receive senddata callback async destroy.
 *
 * @param context Indicates the pointer to context.
 */
static void LeRecvSendDataCallbackAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    LeRecvSendDataCallbackAsyncContext *leRecvSendDataCallPtr = (LeRecvSendDataCallbackAsyncContext *)context;

    MEM_MALLOC.free(leRecvSendDataCallPtr);

    return;
}

/**
 * @brief receive senddata callback.
 *
 * @param1 aclHandle Indicates the aclHandle.
 * @param2 result Indicates the result.
 */
void LeRecvSendDataCallback(uint16_t aclHandle, int result)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    LeRecvSendDataCallbackAsyncContext *leRecvSendDataCallPtr =
        MEM_MALLOC.alloc(sizeof(LeRecvSendDataCallbackAsyncContext));
    if (leRecvSendDataCallPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }

    leRecvSendDataCallPtr->aclHandle = aclHandle;
    leRecvSendDataCallPtr->result = result;
    leRecvSendDataCallPtr->isIndication = false;

    AttAsyncProcess(LeRecvSendDataCallbackAsync, LeRecvSendDataCallbackAsyncDestroy, leRecvSendDataCallPtr);

    return;
}

static void LeEattRecvSendDataCallbackAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    LeEattSendDataCallbackAsyncContext *eattCtx = (LeEattSendDataCallbackAsyncContext *)context;
    AttConnectInfo *connect = NULL;

    // EATT dynamic CIDs are globally unique, so the lcid resolves the sending bearer exactly
    // (the fixed LE_CID is shared by every UATT bearer and never appears here). Even when the
    // L2CAP CID allocator resets (its counter resets when the connection list empties) and a
    // later connection reuses this lcid, the lcid cannot resolve to that new bearer here: this
    // task runs on the ATT queue, and the bearer's teardown and re-establishment tasks were
    // enqueued after this one (FIFO), so the lookup either finds the original bearer or a
    // cleared slot (NULL, safe no-op).
    connect = AttGetConnectInfoByLeCid(eattCtx->lcid);
    if (connect == NULL) {
        goto LEEATTRECVSENDDATA_END;
    }

    if (g_attClientSendDataCB.attSendDataCB != NULL) {
        g_attClientSendDataCB.attSendDataCB(
            connect->retGattConnectHandle, eattCtx->result, g_attClientSendDataCB.context);
    }
    if (eattCtx->result == BT_SUCCESS) {
        AttStartTransactionAlarm(connect);
    } else {
        LOG_WARN("L2CAP error code = %{public}d", eattCtx->result);
        AlarmCancel(connect->alarm);
        // Same wedge risk as LeRecvSendDataCallbackAsync: the request never reached the peer and
        // only its send-completion callback owns the release on success, so remove and free the
        // stranded head request here and report a transaction closure, or the channel stays
        // permanently busy (no response or timeout can retire the packet, and the EATT idle
        // heuristic keeps ListGetSize > 0 forever). The instruct list owns its packets,
        // ListRemoveFirst releases them via the list free callback.
        if (ListRemoveFirst(connect->instruct)) {
            AttClientCallbackDispatch(connect, ATT_TRANSACTION_TIME_OUT_ID, NULL, NULL);
        }
    }

LEEATTRECVSENDDATA_END:
    MEM_MALLOC.free(eattCtx);
    return;
}

static void LeEattSendDataCallbackAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    LeEattSendDataCallbackAsyncContext *eattCtx = (LeEattSendDataCallbackAsyncContext *)context;

    MEM_MALLOC.free(eattCtx);
    return;
}

/**
 * @brief receive EATT senddata callback.
 *
 * @param1 lcid Indicates the EATT channel lcid.
 * @param2 result Indicates the result.
 */
void LeEattRecvSendDataCallback(uint16_t lcid, int result)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    LeEattSendDataCallbackAsyncContext *eattCtx = MEM_MALLOC.alloc(sizeof(LeEattSendDataCallbackAsyncContext));
    if (eattCtx == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }

    eattCtx->lcid = lcid;
    eattCtx->result = result;
    eattCtx->isIndication = false;

    AttAsyncProcess(LeEattRecvSendDataCallbackAsync, LeEattSendDataCallbackAsyncDestroy, eattCtx);
    return;
}

/**
 * @brief BREDR receive senddata callback async.
 *
 * @param context Indicates the pointer to context.
 */
static void BREDRRecvSendDataCallbackAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    BREDRRecvSendDataCallbackAsyncContext *bredrRecvSendDataCallPtr = (BREDRRecvSendDataCallbackAsyncContext *)context;
    AttConnectInfo *connect = NULL;

    AttGetConnectInfoIndexByCid(bredrRecvSendDataCallPtr->lcid, &connect);

    if (connect == NULL) {
        goto BREDRRECVSENDDATACALLBACK_END;
    }

    if (g_attClientSendDataCB.attSendDataCB != NULL) {
        g_attClientSendDataCB.attSendDataCB(
            connect->retGattConnectHandle, bredrRecvSendDataCallPtr->result, g_attClientSendDataCB.context);
    }
    if (bredrRecvSendDataCallPtr->result == BT_SUCCESS) {
        AttStartTransactionAlarm(connect);
    } else {
        LOG_WARN("L2CAP error code = %{public}d", bredrRecvSendDataCallPtr->result);
        AlarmCancel(connect->alarm);
        // Same wedge risk as LeRecvSendDataCallbackAsync: the request never reached the peer and
        // only its send-completion callback owns the release on success, so remove and free the
        // stranded head request here and report a transaction closure, or the bearer stays
        // permanently busy (no response or timeout can retire the packet). The instruct list
        // owns its packets, ListRemoveFirst releases them via the list free callback.
        if (ListRemoveFirst(connect->instruct)) {
            AttClientCallbackDispatch(connect, ATT_TRANSACTION_TIME_OUT_ID, NULL, NULL);
        }
    }

BREDRRECVSENDDATACALLBACK_END:
    MEM_MALLOC.free(bredrRecvSendDataCallPtr);
    return;
}

/**
 * @brief BREDR receive senddata callback async destroy.
 *
 * @param context Indicates the pointer to context.
 */
static void BREDRRecvSendDataCallbackAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    BREDRRecvSendDataCallbackAsyncContext *bredrRecvSendDataCallPtr = (BREDRRecvSendDataCallbackAsyncContext *)context;

    MEM_MALLOC.free(bredrRecvSendDataCallPtr);

    return;
}

/**
 * @brief receive senddata callback.
 *
 * @param1 lcid Indicates the lcid.
 * @param2 result Indicates the result.
 * @param3 context Indicates the pointer to context.
 */
void BREDRRecvSendDataCallback(uint16_t lcid, int result)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    BREDRRecvSendDataCallbackAsyncContext *bredrSendDataCallPtr =
        MEM_MALLOC.alloc(sizeof(BREDRRecvSendDataCallbackAsyncContext));
    if (bredrSendDataCallPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }

    bredrSendDataCallPtr->lcid = lcid;
    bredrSendDataCallPtr->result = result;
    // Client request send: never the server indication; the shared context type also serves
    // the server send path, where isIndication decides the failure handling.
    bredrSendDataCallPtr->isIndication = false;

    AttAsyncProcess(BREDRRecvSendDataCallbackAsync, BREDRRecvSendDataCallbackAsyncDestroy, bredrSendDataCallPtr);

    return;
}

/**
 * @brief receive delect callback.
 *
 */
void AttCallBackDelectCopyToCommon()
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    g_attServerSendDataCB.attSendDataCB = NULL;
    g_attServerSendDataCB.context = NULL;

    return;
}

/**
 * @brief switch thread.
 *
 * @param1 callback Indicates the pointer to function pointer.
 * @param2 destroyCallback Indicates the pointer to function pointer.
 * @param3 context Indicates the pointer to context.
 */
void AttAsyncProcess(
    void (*callback)(const void *context), void (*destroyCallback)(const void *context), const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    int ret;

    ret = BTM_RunTaskInProcessingQueue(PROCESSING_QUEUE_ID_ATT, (void (*)(void *))callback, (void *)context);
    if (ret != BT_SUCCESS) {
        if (destroyCallback != NULL) {
            destroyCallback(context);
        }
    }

    return;
}

/**
 * @brief shut down clear connect information.
 *
 * @param connectInfo Indicates the pointer to AttConnectInfo.
 */
void AttShutDownClearConnectInfo(AttConnectInfo *connectInfo)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    connectInfo->aclHandle = 0;
    (void)memset_s(&connectInfo->AttConnectID, sizeof(connectInfo->AttConnectID), 0, sizeof(connectInfo->AttConnectID));
    connectInfo->retGattConnectHandle = 0;
    connectInfo->transportType = 0;
    (void)memset_s(&connectInfo->addr, sizeof(connectInfo->addr), 0, sizeof(BtAddr));
    connectInfo->mtu = 0;
    connectInfo->sendMtu = 0;
    connectInfo->receiveMtu = 0;
    connectInfo->mtuFlag = false;
    connectInfo->initPassConnFlag = 0;
    connectInfo->serverSendFlag = false;

    // Full slot reset: a shutdown must leave nothing of the EATT establishment or the alarm
    // snapshot machinery behind, or a later connection reusing this slot inherits a stale
    // eattEstablishCb (AttEattEstablish would return BT_OPERATION_FAILED forever) and stale
    // generation/slot state. The Alarm pointers themselves (alarm/indicationAlarm/
    // eattEstablishAlarm) are NOT cleared: ATT_ShutDownAsync cancels them before this reset and
    // AttDelete deletes them afterwards.
    (void)memset_s(&connectInfo->eattLocalCfg, sizeof(connectInfo->eattLocalCfg), 0,
        sizeof(connectInfo->eattLocalCfg));
    (void)memset_s(connectInfo->eattLcids, sizeof(connectInfo->eattLcids), 0, sizeof(connectInfo->eattLcids));
    connectInfo->eattLcidCount = 0;
    connectInfo->eattEstablishCb = NULL;
    connectInfo->eattEstablishCtx = NULL;
    (void)memset_s(connectInfo->eattSlotlessLcids, sizeof(connectInfo->eattSlotlessLcids), 0,
        sizeof(connectInfo->eattSlotlessLcids));
    connectInfo->eattSlotlessCount = 0;
    // The snapshot slots are cleared field-by-field with relaxed atomic stores, paired with the
    // relaxed atomic loads of the alarm-thread readers (AttTransactionTimeOut,
    // AttIndicationTimeOut, AttEattEstablishTimeOut): AlarmCancel is non-waiting, so an alarm
    // armed for this connection may still fire while the slots are being cleared, and its expiry
    // must observe either the old arm values or the cleared ones, never a torn mix. The slot
    // indices and the generation/seq markers below are written plainly: the alarm thread never
    // reads them.
    for (uint8_t index = 0; index < ATT_ALARM_SNAPSHOT_SLOTS; index++) {
        __atomic_store_n(&connectInfo->eattEstablishAlarmCtx[index].connectHandle, 0, __ATOMIC_RELAXED);
        __atomic_store_n(&connectInfo->eattEstablishAlarmCtx[index].batchLcid, 0, __ATOMIC_RELAXED);
        __atomic_store_n(&connectInfo->indicationAlarmCtx[index].connectHandle, 0, __ATOMIC_RELAXED);
        __atomic_store_n(&connectInfo->indicationAlarmCtx[index].lecid, 0, __ATOMIC_RELAXED);
        __atomic_store_n(&connectInfo->indicationAlarmCtx[index].generation, 0, __ATOMIC_RELAXED);
        __atomic_store_n(&connectInfo->transactionAlarmCtx[index].connectHandle, 0, __ATOMIC_RELAXED);
        __atomic_store_n(&connectInfo->transactionAlarmCtx[index].lecid, 0, __ATOMIC_RELAXED);
        __atomic_store_n(&connectInfo->transactionAlarmCtx[index].seq, 0, __ATOMIC_RELAXED);
        __atomic_store_n(&connectInfo->transactionAlarmCtx[index].headPacket, NULL, __ATOMIC_RELAXED);
    }
    connectInfo->eattEstablishAlarmSlot = 0;
    connectInfo->indicationAlarmSlot = 0;
    // Reset the generation marker: with every alarm cancelled first (ATT_ShutDownAsync), no
    // in-flight snapshot can match a fresh generation (see AttStartIndicationAlarm).
    connectInfo->indicationGeneration = 0;
    connectInfo->transactionAlarmSlot = 0;
    // Reset the arm sequence: with every alarm cancelled first (ATT_ShutDownAsync), no in-flight
    // snapshot can match a fresh sequence (see AttStartTransactionAlarm). Unlike per-connection
    // clears (AttClearConnectInfo), the shutdown clears every snapshot slot at once, so resetting
    // here cannot re-match a stale snapshot of a later arm.
    connectInfo->transactionSeq = 0;

    g_attClientCallback.attClientCallback = NULL;
    g_attServerCallback.attServerCallback = NULL;
    g_attClientSendDataCB.attSendDataCB = NULL;
    g_attServerSendDataCB.attSendDataCB = NULL;
    g_attConnect.attConnect.attBREDRConnectCompleted = NULL;
    g_attConnect.attConnect.attBREDRConnectInd = NULL;
    g_attConnect.attConnect.attBREDRDisconnectCompleted = NULL;
    g_attConnect.attConnect.attLEConnectCompleted = NULL;
    g_attConnect.attConnect.attLEDisconnectCompleted = NULL;

    return;
}

/**
 * @brief client callback btbadparam.
 *
 */
void ClientCallbackBTBADPARAM(const AttConnectInfo *connect)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    if (connect == NULL) {
        return;
    }

    if (g_attClientSendDataCB.attSendDataCB != NULL) {
        g_attClientSendDataCB.attSendDataCB(connect->retGattConnectHandle, BT_BAD_PARAM, g_attClientSendDataCB.context);
    }

    return;
}

/**
 * @brief server callback btbadparam.
 *
 */
void ServerCallbackBTBADPARAM(const AttConnectInfo *connect)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    if (g_attServerSendDataCB.attSendDataCB != NULL) {
        g_attServerSendDataCB.attSendDataCB(connect->retGattConnectHandle, BT_BAD_PARAM, g_attServerSendDataCB.context);
    }

    return;
}

/**
 * @brief client callback return value.
 *
 * @param1 ret Indicates the ret.
 * @param2 connect Indicates the pointer of AttConnectInfo.
 */
void ClientCallbackReturnValue(int ret, const AttConnectInfo *connect)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    if (ret != BT_SUCCESS) {
        if (g_attClientSendDataCB.attSendDataCB != NULL) {
            g_attClientSendDataCB.attSendDataCB(connect->retGattConnectHandle, ret, g_attClientSendDataCB.context);
        }
    }

    return;
}

/**
 * @brief server callback return value.
 *
 * @param1 ret Indicates the ret.
 * @param2 connect Indicates the pointer of AttConnectInfo.
 */
void ServerCallbackReturnValue(int ret, const AttConnectInfo *connect)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    if (ret != BT_SUCCESS) {
        if (g_attServerSendDataCB.attSendDataCB != NULL) {
            g_attServerSendDataCB.attSendDataCB(connect->retGattConnectHandle, ret, g_attServerSendDataCB.context);
        }
    }

    return;
}

/**
 * @brief receive  bredr connect instructions data in self thread.
 *
 * @param context Indicates the pointer to context.
 */
static void AttRecvDataAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    uint8_t opcode = 0;
    AttConnectInfo *connect = NULL;
    AttRecvDataAsyncContext *attRecvDataAsyncPtr = (AttRecvDataAsyncContext *)context;
    AttGetConnectInfoIndexByCid(attRecvDataAsyncPtr->lcid, &connect);
    if (connect != NULL) {
        PacketExtractHead(attRecvDataAsyncPtr->packet, &opcode, sizeof(uint8_t));
        Buffer *buffer = PacketContinuousPayload(attRecvDataAsyncPtr->packet);
        recvDataFunction functionPtr = GetFunction(opcode);
        if (functionPtr != NULL) {
            functionPtr(connect, buffer);
        } else {
            LOG_WARN("%{public}s UnKnow OpCode : %hhu", __FUNCTION__, opcode);
            if ((opcode & 0b01000000) == 0) {
                AttErrorCode(connect, opcode);
            }
        }
    }

    PacketFree(attRecvDataAsyncPtr->packet);
    MEM_MALLOC.free(attRecvDataAsyncPtr);

    return;
}

/**
 * @brief destroy receive bredr connect instructions data in self thread.
 *
 * @param context Indicates the pointer to context.
 */
static void AttRecvDataAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttRecvDataAsyncContext *attRecvDataAsyncPtr = (AttRecvDataAsyncContext *)context;

    PacketFree(attRecvDataAsyncPtr->packet);
    MEM_MALLOC.free(attRecvDataAsyncPtr);

    return;
}

/**
 * @brief receive  bredr connect instructions data.
 *
 * @param1 lcid Indicates the lcid.
 * @param2 packet Indicates the pointer to Packet.
 * @param3 ctx Indicates the pointer to  context.
 */
void AttRecvData(uint16_t lcid, const Packet *packet, const void *ctx)
{
    LOG_INFO("%{public}s enter,lcid = %hu", __FUNCTION__, lcid);

    Packet *packetPtr = PacketRefMalloc((Packet *)packet);
    AttRecvDataAsyncContext *attRecvDataAsyncPtr = MEM_MALLOC.alloc(sizeof(AttRecvDataAsyncContext));
    if (attRecvDataAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    attRecvDataAsyncPtr->lcid = lcid;
    attRecvDataAsyncPtr->packet = packetPtr;
    attRecvDataAsyncPtr->ctx = (void *)ctx;

    AttAsyncProcess(AttRecvDataAsync, AttRecvDataAsyncDestroy, attRecvDataAsyncPtr);

    return;
}

/**
 * @brief receive le connect instructions data in self thread.
 *
 * @param context Indicates the pointer to context.
 */
static void AttRecvLeDataAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    uint8_t opcode = 0;
    AttConnectInfo *connect = NULL;
    AttRecvLeDataAsyncContext *attRecvLeDataAsyncPtr = (AttRecvLeDataAsyncContext *)context;
    // Fixed-channel (0x04) data is UATT-only; EATT bearers share aclHandle, so pin lecid == LE_CID
    // to resolve the UATT bearer exactly (Vol 3 Part G 5.3; fixed channel 0x0004).
    connect = AttGetConnectInfoByAclHandleAndLeCid(attRecvLeDataAsyncPtr->aclHandle, LE_CID);
    if (connect != NULL) {
        PacketExtractHead(attRecvLeDataAsyncPtr->packet, &opcode, sizeof(uint8_t));
        Buffer *buffer = PacketContinuousPayload(attRecvLeDataAsyncPtr->packet);
        recvDataFunction functionPtr = GetFunction(opcode);
        if (functionPtr != NULL) {
            functionPtr(connect, buffer);
        } else {
            LOG_WARN("UnKnow OpCode : %hhu", opcode);
            if ((opcode & 0b01000000) == 0) {
                AttErrorCode(connect, opcode);
            }
        }
    }

    PacketFree(attRecvLeDataAsyncPtr->packet);
    MEM_MALLOC.free(attRecvLeDataAsyncPtr);

    return;
}

/**
 * @brief destroy le bredr connect instructions data in self thread.
 *
 * @param context Indicates the pointer to context.
 */
static void AttRecvLeDataAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttRecvLeDataAsyncContext *attRecvLeDataAsyncPtr = (AttRecvLeDataAsyncContext *)context;

    PacketFree(attRecvLeDataAsyncPtr->packet);
    MEM_MALLOC.free(attRecvLeDataAsyncPtr);

    return;
}

/**
 * @brief receive  le connect instructions data.
 *
 * @param1 aclHandle Indicates the aclHandle.
 * @param2 packet Indicates the pointer to Packet.
 */
void AttRecvLeData(uint16_t aclHandle, const Packet *packet)
{
    LOG_INFO("%{public}s enter, aclHandle = %hu", __FUNCTION__, aclHandle);

    Packet *packetPtr = PacketRefMalloc((Packet *)packet);
    if (packetPtr == NULL) {
        LOG_ERROR("PacketRefMalloc failed");
        return;
    }
    AttRecvLeDataAsyncContext *attRecvLeDataAsyncPtr = MEM_MALLOC.alloc(sizeof(AttRecvLeDataAsyncContext));
    if (attRecvLeDataAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        PacketFree(packetPtr);
        packetPtr = NULL;
        return;
    }
    attRecvLeDataAsyncPtr->aclHandle = aclHandle;
    attRecvLeDataAsyncPtr->packet = packetPtr;

    AttAsyncProcess(AttRecvLeDataAsync, AttRecvLeDataAsyncDestroy, attRecvLeDataAsyncPtr);

    return;
}

/**
 * @brief get function.
 *
 * @param1 opcode Indicates the opcode.
 * @return Returns the recvDataFunction.
 */
static recvDataFunction GetFunction(uint8_t opcode)
{
    return g_functionList[opcode];
}

/**
 * @brief get function array dress.
 *
 * @return Returns the pointer to recvDataFunction.
 */
recvDataFunction *GetFunctionArrayDress()
{
    return g_functionList;
}

/**
 * @brief get att connect callback.
 *
 * @return Returns the pointer to AttConnectedCallback.
 */
AttConnectedCallback *AttGetATTConnectCallback()
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    return &g_attConnect;
}

static void AttConnectRegisterAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttConnectRegisterContext *attConnRegPtr = (AttConnectRegisterContext *)context;
    AttConnectedCallback *attConnectCallback = AttGetATTConnectCallback();

    attConnectCallback->attConnect = attConnRegPtr->connectBack;
    attConnectCallback->context = attConnRegPtr->context;

    MEM_MALLOC.free(attConnRegPtr);

    return;
}

static void AttConnectRegisterAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttConnectRegisterContext *attConnRegPtr = (AttConnectRegisterContext *)context;

    MEM_MALLOC.free(attConnRegPtr);

    return;
}

/**
 * @brief gatt register connect to att.
 *
 * @param1 connectBack Indicates the struct to callback.
 * @param2 context Indicates the pointer to context.
 */
void ATT_ConnectRegister(AttConnectCallback connectBack, void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttConnectRegisterContext *attConnRegPtr = MEM_MALLOC.alloc(sizeof(AttConnectRegisterContext));
    if (attConnRegPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }

    (void)memcpy_s(
        &(attConnRegPtr->connectBack), sizeof(attConnRegPtr->connectBack), &connectBack, sizeof(AttConnectCallback));
    attConnRegPtr->context = context;

    AttAsyncProcess(AttConnectRegisterAsync, AttConnectRegisterAsyncDestroy, attConnRegPtr);

    return;
}

static void AttConnectDeregisterAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttConnectedCallback *attConnectCallback = AttGetATTConnectCallback();

    attConnectCallback->attConnect.attLEConnectCompleted = NULL;
    attConnectCallback->attConnect.attLEDisconnectCompleted = NULL;
    attConnectCallback->attConnect.attBREDRConnectCompleted = NULL;
    attConnectCallback->attConnect.attBREDRDisconnectCompleted = NULL;
    attConnectCallback->attConnect.attBREDRConnectInd = NULL;
    attConnectCallback->context = NULL;

    return;
}

static void AttConnectDeregisterAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    return;
}

/**
 * @brief gatt deregister connect to att.
 *
 */
void ATT_ConnectDeregister()
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttAsyncProcess(AttConnectDeregisterAsync, AttConnectDeregisterAsyncDestroy, NULL);

    return;
}

static void AttBREDRSendRespCallbackAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    BREDRRecvSendDataCallbackAsyncContext *attBredrSendRspPtr = (BREDRRecvSendDataCallbackAsyncContext *)context;
    AttConnectInfo *connect = NULL;

    AttGetConnectInfoIndexByCid(attBredrSendRspPtr->lcid, &connect);

    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL", __FUNCTION__);
        goto ATTBREDRSENDRESPCALLBACK_END;
    }

    if (attBredrSendRspPtr->result != BT_SUCCESS) {
        LOG_WARN("L2CAP Send Resp error ,error code = %{public}d", attBredrSendRspPtr->result);
        // The failed send must be the indication itself to clear the pending-indication
        // state: a failure of a response (or error response) to an earlier request can be
        // reported after a later indication was queued and serverSendFlag was set, so the
        // flag alone cannot tell which send failed. The completion callback is selected per
        // PDU type in AttResponseSendData, so isIndication identifies the failed PDU exactly;
        // a failed indication send never reaches the peer, so its confirmation will never
        // arrive and the flag plus confirmation alarm must be released (Vol 3 Part F 3.3.3).
        // serverSendFlag is kept as an additional guard: only a pending indication can own it.
        if (attBredrSendRspPtr->isIndication && connect->serverSendFlag) {
            connect->serverSendFlag = false;
            if (connect->indicationAlarm != NULL) {
                AlarmCancel(connect->indicationAlarm);
            }
        }
    }

    if (g_attServerSendDataCB.attSendDataCB != NULL) {
        g_attServerSendDataCB.attSendDataCB(
            connect->retGattConnectHandle, attBredrSendRspPtr->result, g_attServerSendDataCB.context);
    }

ATTBREDRSENDRESPCALLBACK_END:
    MEM_MALLOC.free(attBredrSendRspPtr);
    return;
}

static void AttBREDRSendRespCallbackAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    BREDRRecvSendDataCallbackAsyncContext *attBredrSendRspPtr = (BREDRRecvSendDataCallbackAsyncContext *)context;

    MEM_MALLOC.free(attBredrSendRspPtr);

    return;
}

/**
 * @brief callback of send response.
 *
 * @param1 lcid Indicates the lcid.
 * @param2 result Indicates the result.
 */
static void AttBREDRSendRespCallback(uint16_t lcid, int result)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    BREDRRecvSendDataCallbackAsyncContext *attBredrSendRspPtr =
        MEM_MALLOC.alloc(sizeof(BREDRRecvSendDataCallbackAsyncContext));
    if (attBredrSendRspPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }

    attBredrSendRspPtr->lcid = lcid;
    attBredrSendRspPtr->result = result;
    attBredrSendRspPtr->isIndication = false;

    AttAsyncProcess(AttBREDRSendRespCallbackAsync, AttBREDRSendRespCallbackAsyncDestroy, attBredrSendRspPtr);

    return;
}

/**
 * @brief callback of send indication (BR/EDR).
 *
 * @param1 lcid Indicates the lcid.
 * @param2 result Indicates the result.
 */
static void AttBREDRSendIndicationCallback(uint16_t lcid, int result)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    BREDRRecvSendDataCallbackAsyncContext *attBredrSendRspPtr =
        MEM_MALLOC.alloc(sizeof(BREDRRecvSendDataCallbackAsyncContext));
    if (attBredrSendRspPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }

    attBredrSendRspPtr->lcid = lcid;
    attBredrSendRspPtr->result = result;
    // Mark the completed send as the server indication: the shared handler clears the
    // pending-indication state only for the indication PDU, never for a response or error
    // response (see AttBREDRSendRespCallbackAsync).
    attBredrSendRspPtr->isIndication = true;

    AttAsyncProcess(AttBREDRSendRespCallbackAsync, AttBREDRSendRespCallbackAsyncDestroy, attBredrSendRspPtr);

    return;
}

static void AttLeSendRespCallbackAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    LeRecvSendDataCallbackAsyncContext *attLeSendRspPtr = (LeRecvSendDataCallbackAsyncContext *)context;
    AttConnectInfo *connect = NULL;

    // Fixed-channel (0x04) data is UATT-only; EATT bearers share aclHandle, so pin lecid ==
    // LE_CID to resolve the UATT bearer exactly (Vol 3 Part G 5.3; fixed channel 0x0004).
    connect = AttGetConnectInfoByAclHandleAndLeCid(attLeSendRspPtr->aclHandle, LE_CID);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL", __FUNCTION__);
        goto ATTLESENDRESPCALLBACK_END;
    }

    if (attLeSendRspPtr->result != BT_SUCCESS) {
        LOG_WARN("L2CAP Send Resp error ,error code = %{public}d", attLeSendRspPtr->result);
        // Same as AttBREDRSendRespCallbackAsync: only the indication send itself may clear
        // the pending-indication state (isIndication is selected per PDU type in
        // AttResponseSendData); a failed response to an earlier request can be reported after
        // a later indication was queued, so the flag alone cannot identify the failed PDU.
        if (attLeSendRspPtr->isIndication && connect->serverSendFlag) {
            connect->serverSendFlag = false;
            if (connect->indicationAlarm != NULL) {
                AlarmCancel(connect->indicationAlarm);
            }
        }
    }

    if (g_attServerSendDataCB.attSendDataCB != NULL) {
        g_attServerSendDataCB.attSendDataCB(
            connect->retGattConnectHandle, attLeSendRspPtr->result, g_attServerSendDataCB.context);
    }

ATTLESENDRESPCALLBACK_END:
    MEM_MALLOC.free(attLeSendRspPtr);
    return;
}

static void AttLeSendRespCallbackAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    LeRecvSendDataCallbackAsyncContext *attLeSendRspPtr = (LeRecvSendDataCallbackAsyncContext *)context;

    MEM_MALLOC.free(attLeSendRspPtr);

    return;
}

static void AttLeSendRespCallback(uint16_t aclHandle, int result)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    LeRecvSendDataCallbackAsyncContext *attLeSendRspPtr = MEM_MALLOC.alloc(sizeof(LeRecvSendDataCallbackAsyncContext));
    if (attLeSendRspPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }

    attLeSendRspPtr->aclHandle = aclHandle;
    attLeSendRspPtr->result = result;
    attLeSendRspPtr->isIndication = false;

    AttAsyncProcess(AttLeSendRespCallbackAsync, AttLeSendRespCallbackAsyncDestroy, attLeSendRspPtr);

    return;
}

/**
 * @brief callback of send indication (UATT fixed channel).
 *
 * @param1 aclHandle Indicates the acl handle.
 * @param2 result Indicates the result.
 */
static void AttLeSendIndicationCallback(uint16_t aclHandle, int result)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    LeRecvSendDataCallbackAsyncContext *attLeSendRspPtr = MEM_MALLOC.alloc(sizeof(LeRecvSendDataCallbackAsyncContext));
    if (attLeSendRspPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }

    attLeSendRspPtr->aclHandle = aclHandle;
    attLeSendRspPtr->result = result;
    // Mark the completed send as the server indication: the shared handler clears the
    // pending-indication state only for the indication PDU, never for a response or error
    // response (see AttLeSendRespCallbackAsync).
    attLeSendRspPtr->isIndication = true;

    AttAsyncProcess(AttLeSendRespCallbackAsync, AttLeSendRespCallbackAsyncDestroy, attLeSendRspPtr);

    return;
}

static void AttLeEattSendRespCallbackAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    LeEattSendDataCallbackAsyncContext *eattCtx = (LeEattSendDataCallbackAsyncContext *)context;
    AttConnectInfo *connect = NULL;

    // lcid resolves the sending bearer exactly; the FIFO ordering of this ATT queue rules out
    // resolving to a different bearer that reused the lcid (see LeEattRecvSendDataCallbackAsync).
    connect = AttGetConnectInfoByLeCid(eattCtx->lcid);
    if (connect == NULL) {
        goto ATTLEEATTSENDRESP_END;
    }

    if (eattCtx->result != BT_SUCCESS) {
        LOG_WARN("L2CAP Send Resp error, error code = %{public}d", eattCtx->result);
        // Same as AttBREDRSendRespCallbackAsync: only the indication send itself may clear
        // the pending-indication state (isIndication is selected per PDU type in
        // AttResponseSendData); a failed response to an earlier request can be reported after
        // a later indication was queued, so the flag alone cannot identify the failed PDU.
        if (eattCtx->isIndication && connect->serverSendFlag) {
            connect->serverSendFlag = false;
            if (connect->indicationAlarm != NULL) {
                AlarmCancel(connect->indicationAlarm);
            }
        }
    }
    if (g_attServerSendDataCB.attSendDataCB != NULL) {
        g_attServerSendDataCB.attSendDataCB(
            connect->retGattConnectHandle, eattCtx->result, g_attServerSendDataCB.context);
    }

ATTLEEATTSENDRESP_END:
    MEM_MALLOC.free(eattCtx);
    return;
}

static void AttLeEattSendRespCallback(uint16_t lcid, int result)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    LeEattSendDataCallbackAsyncContext *eattCtx = MEM_MALLOC.alloc(sizeof(LeEattSendDataCallbackAsyncContext));
    if (eattCtx == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }

    eattCtx->lcid = lcid;
    eattCtx->result = result;
    eattCtx->isIndication = false;

    AttAsyncProcess(AttLeEattSendRespCallbackAsync, LeEattSendDataCallbackAsyncDestroy, eattCtx);
    return;
}

/**
 * @brief callback of send indication (EATT dynamic channel).
 *
 * @param1 lcid Indicates the lcid.
 * @param2 result Indicates the result.
 */
static void AttLeEattSendIndicationCallback(uint16_t lcid, int result)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    LeEattSendDataCallbackAsyncContext *eattCtx = MEM_MALLOC.alloc(sizeof(LeEattSendDataCallbackAsyncContext));
    if (eattCtx == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }

    eattCtx->lcid = lcid;
    eattCtx->result = result;
    // Mark the completed send as the server indication: the shared handler clears the
    // pending-indication state only for the indication PDU, never for a response or error
    // response (see AttLeEattSendRespCallbackAsync).
    eattCtx->isIndication = true;

    AttAsyncProcess(AttLeEattSendRespCallbackAsync, LeEattSendDataCallbackAsyncDestroy, eattCtx);
    return;
}

/**
 * @brief call l2cap interface to send data.
 *
 * @param1 connect Indicates the pointer to const AttConnectInfo.
 * @param2 packet Indicates the pointer to Packet.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
int AttResponseSendData(const AttConnectInfo *connect, const Packet *packet)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    int ret = BT_OPERATION_FAILED;

    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL", __FUNCTION__);
        ret = BT_BAD_PARAM;
        return ret;
    }

    // The completion callback is selected per PDU type: only a failed send of the indication
    // itself may clear the pending-indication state, while a failed response (or error
    // response) to an earlier request must not - its completion can be reported after a later
    // indication was queued and serverSendFlag was set (Vol 3 Part F 3.3.2). The opcode is the
    // first byte of every ATT PDU; it is peeked without consuming the packet
    // (PacketExtractHead removes bytes, the payload must stay intact for L2CAP). Packets sent
    // here are single-part or are joined by PacketContinuousPayload, and the builders all
    // write the opcode at offset 0 (att_send_response.c).
    uint8_t opcode = 0;
    if (packet != NULL) {
        Buffer *buffer = PacketContinuousPayload((Packet *)packet);
        if ((buffer != NULL) && (BufferGetSize(buffer) >= sizeof(uint8_t))) {
            opcode = *(uint8_t *)BufferPtr(buffer);
        }
    }
    bool isIndication = (opcode == HANDLE_VALUE_INDICATION);

    if (connect->transportType == BT_TRANSPORT_BR_EDR) {
        ret = L2CIF_SendData(connect->AttConnectID.bredrcid, (Packet *)packet,
            isIndication ? AttBREDRSendIndicationCallback : AttBREDRSendRespCallback);
    }
    if (connect->transportType == BT_TRANSPORT_LE) {
        if (connect->AttConnectID.lecid == LE_CID) {
            ret = L2CIF_LeSendFixChannelData(connect->aclHandle, (uint16_t)LE_CID, (Packet *)packet,
                isIndication ? AttLeSendIndicationCallback : AttLeSendRespCallback);
        } else {
            // EATT: server response on the source bearer's channel, completion resolves by lcid
            ret = L2CIF_LeSendData(connect->AttConnectID.lecid, (Packet *)packet,
                isIndication ? AttLeEattSendIndicationCallback : AttLeEattSendRespCallback);
        }
    }

    return ret;
}

/**
 * @brief callback error code.
 *
 * @param1 connect Indicates the pointer to const AttConnectInfo.
 * @param2 opcode Indicates the opcode.
 */
void AttErrorCode(const AttConnectInfo *connect, uint8_t opcode)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL", __FUNCTION__);
        return;
    }

    uint8_t errorCode = opcode;
    AttServerCallbackDispatch((AttConnectInfo *)connect, ATT_UNKNOW_OPCODE_ID, &errorCode, NULL);
    return;
}

uint16_t Min(uint16_t param1, uint16_t param2)
{
    if (param1 < param2) {
        return param1;
    } else {
        return param2;
    }
}
