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
 * @file att_common.h
 *
 * @brief declare common function to be called.
 *
 */

#ifndef ATT_COMMON_H
#define ATT_COMMON_H

#include "att.h"

#include <stdint.h>
#include <string.h>

#include "alarm.h"
#include "btstack.h"
#include "list.h"
#include "packet.h"

#include "l2cap_if.h"
#include "l2cap_le_if.h"

#include "gap_if.h"
#include "gap_le_if.h"

#include "securec.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BT_PSM_ATT 0x001F

#define LE_CID 0x04

#define MAXCONNECT 22

#define GAPSIGNATURESIZE 12

#define ADDRESSLEN 6
#define UUID128LEN 16

#define DEFAULTBREDRMTU 48
#define DEFAULTLEATTMTU 23

#define STEP_TWO 2
#define STEP_THREE 3
#define STEP_FOUR 4
#define STEP_FIVE 5
#define STEP_SIX 6
#define STEP_FIFTEEN 15

#define MAXREADBYTYPERESLEN 253
#define MAXREADBYGROUPRESLEN 251

// initiative connect and passive connect flag
#define INITIATIVECONNECT 1
#define PASSIVECONNECT 2

// timeout
#define CONNECTTIMEOUT 40000
#define INSTRUCTIONTIMEOUT 30000

// attribute type len
#define UUID16BITTYPELEN 2
#define UUID128BITTYPELEN 16

#define HANDLEAND16BITBLUETOOTHUUID 0x01
#define HANDLEAND128BITUUID 0x02

#define READBYTYPERESPONSEFREE 1
#define READBYGROUPTYPERESPONSEFREE 2

#define FINDINFORRESINFOR16BITLEN 4
#define FINDINFORRESINFOR128BITLEN 18

// execute write request
#define IMMEDIATELY_WRITE_ALL_PENDING_PREPARED_VALUES 1
#define CANCEL_ALL_PREPARED_WRITES 0

/* Alternated per-arm snapshot slots. The alarm thread reads the snapshot that was armed at the
 * firing it answers, so the content must be immutable between the arm and the expiry. Each arm
 * writes the next slot in rotation, which guarantees immutability without heap allocation: a
 * slot is rewritten only by the arm two steps later, and the arm in between (other slot) resets
 * the timerfd, so a notify that read the fd before that reset runs before the rewrite (it only
 * copies the snapshot to the ATT queue). */
#define ATT_ALARM_SNAPSHOT_SLOTS 2

/* Snapshot handed to the indication-confirmation timeout alarm (AttStartIndicationAlarm): the
 * alarm thread never dereferences the shared connection slot, the expiry handler re-validates
 * this snapshot against the current slots on the ATT processing queue. lecid is 0 for BR/EDR,
 * LE_CID for UATT, the EATT dynamic CID for an EATT bearer. generation identifies the indication
 * this timer was armed for: each arm advances connect->indicationGeneration, so an expiry of an
 * earlier indication is distinguishable from the currently pending one. */
typedef struct {
    uint16_t connectHandle;
    uint16_t lecid;
    uint16_t generation;
} AttIndicationAlarmContext;

/* Snapshot handed to the EATT establishment fallback alarm (5.4 collision retry bound): the
 * alarm thread only reads these two values; batchLcid identifies the deferred batch so a stale
 * expiry can never settle a newer establishment on a reused slot. */
typedef struct {
    uint16_t connectHandle;
    uint16_t batchLcid;
} AttEattEstablishAlarmContext;

/* Snapshot handed to the client-request timeout alarm (AttStartTransactionAlarm): the alarm
 * thread never dereferences the shared connection slot, the expiry handler re-validates this
 * snapshot against the current slots on the ATT processing queue. lecid is 0 for BR/EDR, LE_CID
 * for UATT, the EATT dynamic CID for an EATT bearer. seq is the monotonic arm sequence
 * (connect->transactionSeq) captured at arm time: every arm advances it, so an expiry of an
 * earlier arm is distinguishable from the currently armed request even when the allocator
 * reuses a freed packet address (ABA). headPacket is the identity of the in-flight request at
 * arm time (the instruct head): a stale expiry whose request was already answered and the queue
 * drained without a re-arm (no seq advance) finds a different head (or an empty queue) and is
 * dropped, so the expiry can neither tear down a healthy connection nor remove a newer request's
 * packet (Vol 3 Part F 3.3.3). */
typedef struct {
    uint16_t connectHandle;
    uint16_t lecid;
    uint32_t seq;
    Packet *headPacket;
} AttTransactionAlarmContext;

typedef struct AttConnectInfo {
    uint16_t aclHandle;
    union {
        uint16_t bredrcid;
        uint16_t lecid;
    } AttConnectID;
    uint16_t retGattConnectHandle;
    uint8_t transportType;
    BtAddr addr;
    uint16_t mtu;
    uint16_t sendMtu;
    uint16_t receiveMtu;
    bool mtuFlag;
    uint8_t initPassConnFlag;
    List *instruct;
    Alarm *alarm;
    Alarm *indicationAlarm;
    bool serverSendFlag;
    // EATT establishment in flight on this connection (initiator side): the caller's
    // completion callback and local config, resolved by the 0x18 connection response
    // (AttEattConnectionRsp) or by an abnormal teardown of the batch channels
    // (AttEattDisconnected). eattEstablishCb == NULL means no pending request.
    L2capLeConfigInfo eattLocalCfg;
    uint16_t eattLcids[L2CAP_LE_EATT_MAX_CHANNEL];
    uint16_t eattLcidCount;
    void (*eattEstablishCb)(int result, const uint16_t *lcids, uint16_t n, void *ctx);
    void *eattEstablishCtx;
    // Channels of the pending batch that L2CAP established but that could not take a bearer slot
    // (AttEattConnected dropped them): recorded so AttEattConnectionRsp fails the batch and
    // disconnects the orphaned channels instead of reporting success for data that would be
    // silently discarded. eattSlotlessCount == 0 means no orphaned channel.
    uint16_t eattSlotlessLcids[L2CAP_LE_EATT_MAX_CHANNEL];
    uint8_t eattSlotlessCount;
    // Fallback bound of a deferred establishment (collision retry in progress): armed on the
    // deferral, cancelled by AttEattResolveEstablish and AttClearConnectInfo. The alarm parameter
    // is one of the alternated immutable snapshots eattEstablishAlarmCtx[], never the connection
    // state itself (see ATT_ALARM_SNAPSHOT_SLOTS). Inert when no alarm is armed.
    Alarm *eattEstablishAlarm;
    AttEattEstablishAlarmContext eattEstablishAlarmCtx[ATT_ALARM_SNAPSHOT_SLOTS];
    uint8_t eattEstablishAlarmSlot;
    // Alternated immutable snapshots of the bearer identity + indication generation captured at
    // AttStartIndicationAlarm; see AttIndicationAlarmContext and ATT_ALARM_SNAPSHOT_SLOTS. The
    // fields are inert when no alarm is armed; indicationGeneration advances at every arm, so it
    // also separates indications across connection generations in a slot. It is reset only by the
    // full shutdown reset (AttShutDownClearConnectInfo): ATT_ShutDownAsync cancels every alarm
    // first, and the reset clears the snapshot fields with relaxed atomic stores paired with the
    // alarm thread's relaxed atomic loads (an alarm may still fire after the non-waiting
    // AlarmCancel, and must never observe a torn snapshot).
    AttIndicationAlarmContext indicationAlarmCtx[ATT_ALARM_SNAPSHOT_SLOTS];
    uint8_t indicationAlarmSlot;
    uint16_t indicationGeneration;
    // Alternated immutable snapshots of the bearer identity + in-flight request (instruct head)
    // captured at AttStartTransactionAlarm; see AttTransactionAlarmContext and
    // ATT_ALARM_SNAPSHOT_SLOTS. The fields are inert when no alarm is armed.
    AttTransactionAlarmContext transactionAlarmCtx[ATT_ALARM_SNAPSHOT_SLOTS];
    uint8_t transactionAlarmSlot;
    // Monotonic arm sequence of AttStartTransactionAlarm: advances at every arm, so a stale
    // expiry can never match a newer arm by an address-reused head packet (ABA) - the stale
    // check compares this against the seq captured in the snapshot. It is reset only by the
    // full shutdown reset (AttShutDownClearConnectInfo), where ATT_ShutDownAsync cancels every
    // alarm first and the reset clears the snapshot fields with relaxed atomic stores paired with
    // the alarm thread's relaxed atomic loads: a slot cleared and reused by a fresh connection
    // must not re-match a stale snapshot's seq (the same reasoning as indicationGeneration).
    uint32_t transactionSeq;
} AttConnectInfo;

typedef struct AttConnectingInfo {
    uint16_t aclHandle;
    uint16_t cid;
    uint8_t id;
    uint16_t mtu;
    L2capConfigInfo locall2capConfigInfoObj;
    L2capConfigInfo remotel2capConfigInfoObj;
    BtAddr addr;
    uint16_t connectHandle;
    uint8_t initiativeConnectStatus;
    uint8_t passiveConnectSatatus;
    uint8_t initPassConnFlag;  // 1:initiative connect; 2:passive connect
    Alarm *bredrAlarm;
    Alarm *leAlarm;
    uint8_t transportType;
} AttConnectingInfo;

typedef void (*recvDataFunction)(AttConnectInfo *connect, const Buffer *buffer);

typedef struct {
    attCallback attClientCallback;
    void *context;
} AttClientDataCallback;

typedef struct {
    attCallback attServerCallback;
    void *context;
} AttServerDataCallback;

typedef struct {
    AttCallbackCid attClientCallback;
    void *context;
} AttClientDataCallbackCid;

typedef struct {
    AttCallbackCid attServerCallback;
    void *context;
} AttServerDataCallbackCid;

typedef struct {
    AttConnectCallback attConnect;
    void *context;
} AttConnectedCallback;

typedef struct {
    uint16_t connectHandle;
    AttError *ATTErrorPtr;
    uint16_t cid;
} ErrorResponseAsync;

typedef struct {
    uint16_t connectHandle;
    uint16_t mtu;
} ExchangeMTUAsync;

typedef struct {
    uint16_t connectHandle;
    AttHandleRange attHandleRange;
} FindInformationRequestAsync;

typedef struct {
    uint16_t connectHandle;
    AttFindInformationRsp attFindInformationResContext;
    uint16_t cid;
} FindInformationResponseAsync;

typedef struct {
    uint16_t connectHandle;
    AttFindByTypeValueReq *attFindByTypePtreve;
    Buffer *attValue;
} FindByTypeValueRequestAsync;

typedef struct {
    uint16_t connectHandle;
    AttFindByTypeValueRsp attFindByTypeResContext;
    uint16_t cid;
} FindByTypeValueResponseAsync;

typedef struct {
    uint16_t connectHandle;
    AttReadByTypeReq attReadByTypeReqContext;
} ReadByTypeRequestAsync;

typedef struct {
    uint16_t connectHandle;
    AttReadByTypeRsp attReadByTypeRspContext;
    uint16_t cid;
} ReadByTypeResponseAsync;

typedef struct {
    uint16_t connectHandle;
    uint16_t attHandle;
} ReadRequestAsync;

typedef struct {
    uint16_t connectHandle;
    Buffer *attValue;
    uint16_t cid;
} ReadResponseAsync;  // readresponse / readblobresponse / readmultipleresponse / readmultiplerequest

typedef struct {
    uint16_t connectHandle;
    AttReadBlobReqPrepareWriteValue attReadBlobContext;
} ReadBlobRequestAsync;

typedef struct {
    uint16_t connectHandle;
    AttReadByTypeReq attReadGroupContext;
} ReadByGroupTypeRequesAsync;

typedef struct {
    uint16_t connectHandle;
    AttReadGroupRes attReadGroupResContext;
    uint16_t cid;
} ReadByGroupTypeResponseAsync;

typedef struct {
    uint16_t connectHandle;
    uint16_t cid;
    uint16_t attHandle;
    Buffer *attValue;
} WriteAsync;  // writerequest / writecommand / signedwritecommand / handlenotification / handleindication

typedef struct {
    uint16_t connectHandle;
    uint16_t cid;
} WriteResponseAsync;  // writeresponse / executewriterresponse / handleconfirmation

typedef struct {
    uint16_t connectHandle;
    AttReadBlobReqPrepareWriteValue attReadBlobObj;
    Buffer *attValue;
    uint16_t cid;
} PrepareWriteAsync;  // preparewriterequest / preparewriteresponse

typedef struct {
    uint16_t connectHandle;
    uint8_t flag;
} ExecuteWriteRequestAsync;

/* Context of an in-flight signed write (SIGNED_WRITE_CMD): carries the bearer identity and a
 * private copy of the peer address instead of a raw AttConnectInfo pointer, because the signature
 * is generated on the GAP thread (GAPIF_LeDataSignatureGenerationAsync keeps the address pointer
 * for the duration of the request) and the connection slot may be cleared or reused before the
 * result callback runs on the ATT processing queue; the callback re-resolves the connection by
 * connectHandle + lecid. lecid is 0 for BR/EDR, LE_CID for UATT (signed writes never ride an EATT
 * bearer, see AttSignedWriteCommandAsync). */
typedef struct SigedWriteCommandGenerationContext {
    uint16_t connectHandle;
    uint16_t lecid;
    Packet *packet;
    uint8_t *data;
    uint16_t bufferSize;
    BtAddr addr;
} SigedWriteCommandGenerationContext;

typedef struct AttGapSignatureGenerationContext {
    GAP_SignatureResult result;
    uint8_t *signaturePtr;
    uint16_t signatureLen;
    void *context;
} AttGapSignatureGenerationContext;

/**
 * @brief get AttConnectInfo information.
 *
 * @return Returns the pointer to AttConnectInfo.
 */
AttConnectInfo *AttGetConnectStart();

/**
 * @brief lookup AttConnectInfo info by aclHandle (first match).
 *
 * DEPRECATED: with EATT the ACL handle is shared by the UATT bearer and every EATT bearer,
 * so a bare aclHandle lookup is ambiguous. Use AttGetConnectInfoByAclHandleAndLeCid.
 *
 * @param1 aclHandle Indicates the aclHandle.
 * @param2 connect Indicates the second rank pointer to AttConnectInfo.
 */
void AttGetConnectInfoIndexByAclHandle(uint16_t aclHandle, AttConnectInfo **connect);

/**
 * @brief lookup AttConnectInfo info by cid.
 *
 * @param1 cid Indicates the cid.
 * @param2 connect Indicates the second rank pointer to AttConnectInfo.
 */
void AttGetConnectInfoIndexByCid(uint16_t cid, AttConnectInfo **connect);

/**
 * @brief lookup AttConnectInfo info by cid and output parameter index.
 *
 * @param1 cid Indicates the cid.
 * @param2 index Indicates the pointer to index.
 * @param3 connect Indicates second rank pointer to AttConnectInfo.
 */
void AttGetConnectInfoIndexByCidOutIndex(uint16_t cid, uint16_t *index, AttConnectInfo **connect);

/**
 * @brief lookup AttConnectInfo info by connectHandle and output parameter index.
 *
 * @param1 connectHandle Indicates the connectHandle.
 * @param2 index Indicates the pointer to index.
 * @param3 connect Indicates second rank pointer to AttConnectInfo.
 */
void AttGetConnectInfoIndexByConnectHandle(uint16_t connectHandle, uint16_t *index, AttConnectInfo **connect);

/**
 * @brief lookup AttConnectInfo info by an LE cid.
 *
 * @param1 lecid Indicates the LE cid (EATT dynamic CID only).
 * @return Returns the matched AttConnectInfo pointer, or NULL when absent.
 */
AttConnectInfo *AttGetConnectInfoByLeCid(uint16_t lecid);

/**
 * @brief lookup AttConnectInfo info by ACL handle and LE cid.
 *
 * @param1 aclHandle Indicates the ACL handle.
 * @param2 lecid Indicates the LE cid.
 * @return Returns the matched AttConnectInfo pointer, or NULL when absent.
 */
AttConnectInfo *AttGetConnectInfoByAclHandleAndLeCid(uint16_t aclHandle, uint16_t lecid);

/**
 * @brief lookup AttConnectInfo info by connectHandle and LE cid.
 *
 * @param1 connectHandle Indicates the connectHandle.
 * @param2 lecid Indicates the LE cid (0 for the BR/EDR bearer, LE_CID for UATT, EATT dynamic CID).
 * @return Returns the matched AttConnectInfo pointer, or NULL when the connection is absent.
 */
AttConnectInfo *AttGetConnectInfoByConnectHandleAndLeCid(uint16_t connectHandle, uint16_t lecid);

/**
 * @brief pick the EATT-preferred bearer for a client request/command or a server notification.
 *
 * The idle criterion is that bearer's client in-flight request queue (one request per bearer,
 * Vol 3 Part F 3.3.2); commands (WRITE_CMD) and notifications share the heuristic since they
 * impose no flow control.
 *
 * @param1 connectHandle Indicates the connectHandle.
 * @return Returns the selected bearer pointer, or NULL when the connection is absent.
 */
AttConnectInfo *AttGetConnectInfoByConnectHandlePreferEattRequestOrNtf(uint16_t connectHandle);

/**
 * @brief pick the EATT-preferred bearer for a server indication.
 *
 * The idle criterion is no outstanding indication awaiting its confirmation on that bearer
 * (one indication per bearer until confirmed, Vol 3 Part F 3.3.2).
 *
 * @param1 connectHandle Indicates the connectHandle.
 * @return Returns the selected bearer pointer, or NULL when the connection is absent.
 */
AttConnectInfo *AttGetConnectInfoByConnectHandlePreferEattInd(uint16_t connectHandle);

/**
 * @brief pick the bearer that has an unconfirmed indication pending for the confirmation path.
 *
 * Serves the legacy confirmation (cid == 0), which carries no bearer identity: the confirmation
 * must close the indication on the bearer it was sent on (serverSendFlag), or the EATT bearer's
 * flag never resets and the bearer is torn down at the 3.3.3 timeout.
 *
 * @param1 connectHandle Indicates the connectHandle.
 * @return Returns the matched bearer pointer, or NULL when no bearer has a pending indication.
 */
AttConnectInfo *AttGetConnectInfoByConnectHandlePendingInd(uint16_t connectHandle);

/**
 * @brief get AttConnectingInfo information.
 *
 * @return Returns the pointer to AttConnectingInfo.
 */
AttConnectingInfo *AttGetConnectingStart();

/**
 * @brief lookup AttConnectingInfo info by cid.
 *
 * @param1 cid Indicates the cid.
 * @param2 connecting Indicates the second rank pointer to AttConnectingInfo.
 */
void AttGetConnectingIndexByCid(uint16_t cid, AttConnectingInfo **connecting);

/**
 * @brief lookup AttConnectingInfo info by cid and output parameter index.
 *
 * @param1 cid Indicates the cid.
 * @param2 index Indicates the pointer to index.
 * @param3 connecting Indicates the second rank pointer to AttConnectingInfo.
 */
void AttGetConnectingIndexByCidOutIndex(uint16_t cid, uint16_t *index, AttConnectingInfo **connecting);

/**
 * @brief lookup AttConnectingInfo info by connectHandle.
 *
 * @param1 connectHandle Indicates the connectHandle.
 * @param2 connecting Indicates the second rank pointer to AttConnectingInfo.
 */
void AttGetConnectingIndexByConnectHandle(uint16_t connectHandle, AttConnectingInfo **connecting);

/**
 * @brief lookup AttConnectingInfo info by cid and connectHandle, result to output parameter index.
 *
 * @param1 cid Indicates the cid.
 * @param2 connectHandle Indicates the connectHandle.
 * @param3 index Indicates the pointer to index.
 * @param4 connecting Indicates second rank pointer to AttConnectingInfo.
 */
void AttGetConnectingIndexByCidConnectHandle(
    uint16_t cid, uint16_t connectHandle, uint16_t *index, AttConnectingInfo **connecting);

/**
 * @brief lookup AttConnectingInfo info by addr.
 *
 * @param1 addr Indicates pointer to addr.
 * @param2 connecting Indicates the second rank pointer to AttConnectingInfo.
 */
void AttGetConnectingIndexByAddr(const BtAddr *addr, AttConnectingInfo **connecting);

/**
 * @brief lookup AttConnectingInfo info by addr uninitialized cid.
 *
 * @param1 addr Indicates pointer to addr.
 * @param2 connecting Indicates the second rank pointer to AttConnectingInfo.
 */
void AttGetConnectingIndexByAddrUninitializedCid(const BtAddr *addr, AttConnectingInfo **connecting);

/**
 * @brief lookup AttConnectingInfo info by addr aclhandle cid.
 *
 * @param1 addr Indicates pointer to addr.
 * @param2 addr Indicates the aclHandle.
 * @param3 addr Indicates the cid.
 * @param4 connecting Indicates the second rank pointer to AttConnectingInfo.
 */
void AttGetConnectingIndexByAddrAclhandleCid(
    const BtAddr *addr, uint16_t aclHandle, uint16_t cid, AttConnectingInfo **connecting);

/**
 * @brief get AttClientDataCallback information.
 *
 * @return Returns the pointer to AttClientDataCallback.
 */
AttClientDataCallback *AttGetATTClientCallback();

/**
 * @brief get AttServerDataCallback information.
 *
 * @return Returns the pointer to AttServerDataCallback.
 */
AttServerDataCallback *AttGetATTServerCallback();

/**
 * @brief get AttClientDataCallbackCid information.
 *
 * @return Returns the pointer to AttClientDataCallbackCid.
 */
AttClientDataCallbackCid *AttGetATTClientCallbackCid();

/**
 * @brief get AttServerDataCallbackCid information.
 *
 * @return Returns the pointer to AttServerDataCallbackCid.
 */
AttServerDataCallbackCid *AttGetATTServerCallbackCid();

/**
 * @brief dispatch a received event to the registered data callbacks.
 *
 * @param1 connect Indicates the pointer to AttConnectInfo.
 * @param2 event Indicates the event id.
 * @param3 eventData Indicates the pointer to event data.
 * @param4 buffer Indicates the pointer to Buffer.
 */
void AttClientCallbackDispatch(AttConnectInfo *connect, uint16_t event, void *eventData, Buffer *buffer);

void AttServerCallbackDispatch(AttConnectInfo *connect, uint16_t event, void *eventData, Buffer *buffer);

/**
 * @brief initiative execut instructions by Scheduling.
 *
 * @param connect Indicates the pointer to AttConnectInfo.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
int AttSendSequenceScheduling(const AttConnectInfo *connect);

/**
 * @brief execut instructions by Scheduling after receiving response.
 *
 * @param connect Indicates the pointer to AttConnectInfo.
 */
void AttReceiveSequenceScheduling(const AttConnectInfo *connect);

/**
 * @brief client call back copy.
 *
 * @param1 attSendDataCB Indicates the pointer to attSendDataCallback.
 * @param2 context Indicates the pointer to context.
 */
void AttClientCallBackCopyToCommon(attSendDataCallback attSendDataCB, const void *context);

/**
 * @brief server call back copy.
 *
 * @param1 attSendDataCB Indicates the pointer to attSendDataCallback.
 * @param2 context Indicates the pointer to context.
 */
void AttServerCallBackCopyToCommon(attSendDataCallback attSendDataCB, const void *context);
/**
 * @brief receive senddata callback.
 *
 * @param1 aclHandle Indicates the aclHandle.
 * @param2 result Indicates the result.
 */
void LeRecvSendDataCallback(uint16_t aclHandle, int result);

/**
 * @brief receive EATT senddata callback.
 *
 * @param1 lcid Indicates the EATT channel lcid.
 * @param2 result Indicates the result.
 */
void LeEattRecvSendDataCallback(uint16_t lcid, int result);

/**
 * @brief bredr receive senddata callback.
 *
 * @param1 aclHandle Indicates the aclHandle.
 * @param2 result Indicates the result.
 * @param3 cb Indicates the pointer to cb.
 */
void BREDRRecvSendDataCallback(uint16_t lcid, int result);

/**
 * @brief receive delect callback.
 *
 */
void AttCallBackDelectCopyToCommon();

/**
 * @brief switch thread.
 *
 * @param1 callback Indicates the pointer to function pointer.
 * @param2 destroyCallback Indicates the pointer to function pointer.
 * @param3 context Indicates the pointer to context.
 */
void AttAsyncProcess(
    void (*callback)(const void *context), void (*destroyCallback)(const void *context), const void *context);

/**
 * @brief shut down clear connect information.
 *
 * @param connectInfo Indicates the pointer to AttConnectInfo.
 */
void AttShutDownClearConnectInfo(AttConnectInfo *connectInfo);

/**
 * @brief client call back btbadparam.
 *
 * @param connect Indicates the pointer to AttConnectInfo.
 */
void ClientCallbackBTBADPARAM(const AttConnectInfo *connect);

/**
 * @brief server call back btbadparam.
 *
 * @param connect Indicates the pointer to AttConnectInfo.
 */
void ServerCallbackBTBADPARAM(const AttConnectInfo *connect);

/**
 * @brief client call back return value.
 *
 * @param1 ret Indicates the ret.
 * @param2 connect Indicates the pointer to AttConnectInfo.
 */
void ClientCallbackReturnValue(int ret, const AttConnectInfo *connect);

/**
 * @brief server call back return value.
 *
 * @param1 ret Indicates the ret.
 * @param2 connect Indicates the pointer to AttConnectInfo.
 */
void ServerCallbackReturnValue(int ret, const AttConnectInfo *connect);

/**
 * @brief arm the per-bearer indication-confirmation timeout.
 *
 * Called after an indication is queued on the bearer; the timer runs until the confirmation or
 * the Vol 3 Part F 3.3.3 timeout. Exported for AttHandleValueIndicationAsync.
 *
 * @param connect Indicates the pointer to AttConnectInfo.
 */
void AttStartIndicationAlarm(AttConnectInfo *connect);

/**
 * @brief received bredr connect instructions data information.
 *
 * @param1 lcid Indicates the lcid.
 * @param2 packet Indicates the pointer to Packet.
 * @param3 ctx Indicates the pointer to context.
 */
void AttRecvData(uint16_t lcid, const Packet *packet, const void *ctx);

/**
 * @brief received le connect instructions data information.
 *
 * @param1 aclHandle Indicates the aclHandle.
 * @param2 packet Indicates the pointer to Packet.
 */
void AttRecvLeData(uint16_t aclHandle, const Packet *packet);

AttConnectedCallback *AttGetATTConnectCallback();

int AttResponseSendData(const AttConnectInfo *connect, const Packet *packet);

/**
 * @brief received error opcode.
 *
 * @deprecated Replaced by AttReplyNotSupported (cid-aware); kept for API compatibility.
 *
 * @param1 connect Indicates the AttConnectInfo.
 * @param2 error opcode.
 */
void AttErrorCode(const AttConnectInfo *connect, uint8_t opcode);

/**
 * @brief reply ATT_ERROR_RSP with Request Not Supported for an unknown request
 *        opcode (Vol 3 Part F 3.3).
 *
 * @param1 connect Indicates the AttConnectInfo.
 * @param2 opcode Indicates the opcode of the received request.
 */
void AttReplyNotSupported(AttConnectInfo *connect, uint8_t opcode);

/**
 * @brief get function array dress.
 *
 * @return Returns the pointer to recvDataFunction.
 */
recvDataFunction *GetFunctionArrayDress();

/**
 * @brief return minute number
 *
 * @param1 number1
 * @param2 number2
 */
uint16_t Min(uint16_t param1, uint16_t param2);

void AttShutDownClearConnectInfo(AttConnectInfo *connectInfo);

#ifdef __cplusplus
}
#endif

#endif  // ATT_COMMON_H
