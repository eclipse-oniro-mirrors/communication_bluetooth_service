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
 * @file l2cap_le_internal.h
 *
 * @brief Internal types and helpers shared between l2cap_le.c and l2cap_le_eatt.c
 *
 */

#ifndef L2CAP_LE_INTERNAL_H
#define L2CAP_LE_INTERNAL_H

#include "alarm.h"
#include "list.h"
#include "packet.h"
#include "l2cap_cmn.h"
#include "l2cap_le.h"
#include "log.h"

#define L2CAP_LE_DEFAULT_CREDIT 0x08

#define L2CAP_LE_CHANNEL_CREDIT_NOT_FULL 0x00
#define L2CAP_LE_CHANNEL_CREDIT_FULL 0x01

// EATT enhanced credit based flow control mode constants, chapter 4.25-4.28 of Core 5.2
// L2CAP_LE_EATT_MAX_CHANNEL now lives in l2cap_def.h (shared with the L2CIF wrapper layer)
#define L2CAP_LE_EATT_MIN_MTU                    64    // minimum MTU of an ECRED channel, 64 bytes, chapter 4.25
#define L2CAP_LE_EATT_MIN_MPS                    64    // minimum MPS of an ECRED channel, 64 bytes, chapter 4.25
#define L2CAP_LE_EATT_MAX_MPS                    65533 // maximum MPS of an ECRED channel, chapter 4.25
#define L2CAP_LE_EATT_RETRY_DELAY_MS             100   // minimum slave retry delay after a collision, Vol 3 Part G 5.4
#define L2CAP_LE_EATT_SEC_REQUIRE_AUTHENTICATION 0x01  // security requirement bit: authentication, result 0x0005
#define L2CAP_LE_EATT_SEC_REQUIRE_AUTHORIZATION  0x02  // security requirement bit: authorization, result 0x0006
#define L2CAP_LE_EATT_KEY_SIZE_MIN               7     // minimum link key size for EATT, Vol 3 Part G 5.3.2
#define L2CAP_LE_GRANTED_CONN_PARAM_MAX          4     // granted LE connection parameter slots,
                                                       // one per concurrent connection
#define L2CAP_LE_EATT_CONN_FIXED_LEN             8     // 0x17/0x18 fixed fields: SPSM+MTU+MPS+Credits, 2 octets each
#define L2CAP_LE_EATT_RECONFIG_FIXED_LEN         4     // 0x19/0x1A fixed fields: MTU+MPS
#define L2CAP_LE_EATT_CID_LEN                    2     // 2 octets per CID in the 0x17-0x1A CID lists
#define L2CAP_LE_EATT_SLAVE_RETRY_FACTOR         2     // Vol 3 Part G 5.4: 2 x (connSlaveLatency + 1) x connInterval
#define L2CAP_LE_EATT_INTERVAL_MS_NUMERATOR      5     // connInterval(ms) = connIntervalUnits x 1.25 ms, numerator
#define L2CAP_LE_EATT_INTERVAL_MS_DENOMINATOR    4     // connInterval(ms) = connIntervalUnits x 1.25 ms, denominator
#define L2CAP_LE_EATT_INTERVAL_MS_ROUNDUP        3     // round up: (n x numerator + roundup) / denominator

typedef struct {
    uint16_t lpsm;
    L2capLeService service;

    uint8_t secRequirement; // EATT security requirement bits, checked for inbound 0x17, table 4.21
    L2capLeConfigInfo lcfg; // EATT responder default local config, set by L2CAP_LeSetServiceConfig

    void *ctx;
} L2capLePsm;

typedef struct {
    uint16_t lcid;
    uint16_t rcid;

    uint16_t lpsm;
    uint16_t rpsm;

    uint8_t connIdentifier;
    // Sequence of the pending 0x17 entry that last covered this channel as an initiator batch
    // member, stamped at send time (L2capLeEattSendConnectionReq). The timeout and collision
    // retry cleanups match a batch by this sequence in addition to the identifier, so an
    // identifier wrap-around within the RTX window cannot make a stale expiry of an old batch
    // delete the channels of a newer batch that reused the value (see L2capPendingRequest::seq).
    // 0 for responder-side channels and channels not yet covered by any 0x17.
    uint32_t batchSeq;
    uint8_t state;

    uint8_t busyState;

    L2capLeConfigInfo lcfg;
    L2capLeConfigInfo rcfg;

    List *txList;
    Packet *rxSarPacket;

    uint16_t peerCredits;
} L2capLeChannel;

typedef struct {
    // link-level security state for the inbound 0x17 security check, Vol 3 Part G 5.3.2 and table 4.21
    uint8_t encrypt;      // link encryption state, maintained by the HCI 0x08 encryption change event
    uint8_t keySize;      // encryption key size, injected by the upper layer, 0 means unknown
    uint8_t authLevel;    // authentication state, injected by the upper layer
    uint8_t authzGranted; // authorization state, injected by the upper layer
    List *reconfigList;   // in-flight 0x19 reconfigure requests, each holds a L2capLeReconfigReq
    Alarm *retryTimer;    // slave collision retry timer, Vol 3 Part G 5.4: one-shot wait before resending
} L2capLeEattContext;

typedef struct {
    uint16_t aclHandle;
    BtAddr addr;

    uint8_t nextIdentifier;
    uint8_t role;

    uint16_t connIntervalUnits; // current negotiated interval in 1.25 ms units, set from the L2CAP_LeConnect
                                // request and refreshed on LE Connection Update Complete, Vol 3 Part G 5.4
    uint16_t connSlaveLatency;  // current negotiated slave latency in connection events, Vol 3 Part G 5.4

    L2capLeEattContext eatt; // EATT enhanced credit based flow control state, chapter 4.25-4.28

    List *chanList;  // Pack struct L2capLeChannel

    List *pendingList;  // Pack struct L2capPendingRequest
} L2capLeConnection;

typedef struct {
    L2capLeConnectionParameterUpdate cb;
    void *ctx;
} L2capLeConnectionParameterUpdateContext;

// in-flight 0x19 reconfigure request context: the initiator matches the 0x1A response by identifier and
// applies the new local receive config to the listed channels on success, chapter 4.27/4.28
typedef struct {
    uint8_t identifier;                        // signaling identifier of this 0x19 request
    uint16_t mtu;                              // requested new local receive MTU
    uint16_t mps;                              // requested new local receive MPS
    uint8_t n;                                 // number of channels being reconfigured
    uint16_t lcids[L2CAP_LE_EATT_MAX_CHANNEL]; // 0x19 DCIDs = sender's local CIDs, chapter 4.27
} L2capLeReconfigReq;

// inbound 0x17 responder: request context shared by the validation/establishment/response helpers,
// chapter 4.25/4.26
typedef struct {
    L2capLeConnection *conn;
    L2capLePsm *psm;
    L2capConnectionInfo connInfo;
    L2capLeConfigInfo cfgLocal;
    L2capLeConfigInfo cfgRemote;
    L2capLeChannel *chans[L2CAP_LE_EATT_MAX_CHANNEL];
    uint16_t dcids[L2CAP_LE_EATT_MAX_CHANNEL];
    const uint8_t *data;
    uint16_t aclHandle;
    uint16_t length;
    uint16_t result;
    uint8_t identifier;
    uint8_t chanCount;
    uint8_t n;
} L2capLeEattConnReqContext;

// inbound 0x18 initiator: response context shared by the collection/validation/establishment helpers,
// chapter 4.26
typedef struct {
    L2capLeConnection *conn;
    L2capLePsm *psm;
    L2capConnectionInfo connInfo;
    L2capLeConfigInfo cfg;
    L2capLeChannel *chans[L2CAP_LE_EATT_MAX_CHANNEL];
    const uint8_t *data;
    uint16_t result;
    uint8_t chanCount;
    uint8_t dcidCount;
    uint8_t cfgValid;
    uint8_t succeeded;
    uint8_t linkTornDown; // chapter 4.26: set when an invalid peer DCID tears down the whole link
} L2capLeEattConnRspContext;

// inbound 0x19 responder: request context shared by the validation/apply helpers, chapter 4.27
typedef struct {
    L2capLeConnection *conn;
    L2capLePsm *psm;
    L2capLeChannel *chans[L2CAP_LE_EATT_MAX_CHANNEL];
    const uint8_t *data;
    uint16_t length;
    uint16_t mtu;
    uint16_t mps;
    uint8_t identifier;
    uint8_t n;
} L2capLeEattReconfigReqContext;

// inbound 0x1A initiator: response context shared by the lookup/apply helpers, chapter 4.28
typedef struct {
    L2capLeConnection *conn;
    L2capLeReconfigReq *req;
    uint16_t result;
} L2capLeEattReconfigRspContext;

// one granted LE connection parameter set stashed from an LE Connection Complete HCI event
typedef struct {
    uint16_t handle;    // ACL handle the parameters belong to, matched by the consumer
    uint16_t interval;  // granted connection interval in 1.25 ms units
    uint16_t latency;   // granted connection slave latency in connection events
    uint8_t valid;      // 1 while unconsumed
} L2capLeGrantedConnParam;

// one requested LE connection parameter set stashed from L2CAP_LeConnect while the ACL is not up yet
typedef struct {
    BtAddr addr;                    // peer address the parameters belong to, matched by the consumer
    uint16_t connIntervalMax;       // requested maximum connection interval in 1.25 ms units
    uint16_t connLatency;           // requested connection slave latency in connection events
    uint8_t valid;                  // 1 while unconsumed
} L2capLePendingConnParam;

typedef struct {
    uint16_t nextLcid;

    L2capLeFixChannel chanAtt;
    L2capLeFixChannel chanSm;

    L2capLeConnectionParameterUpdateContext connParamUpdate;

    // requested LE connection parameters of the in-flight connects, applied when the ACL completes,
    // used by the slave EATT retry delay of Vol 3 Part G 5.4. A per-address slot array keeps two
    // connects in flight at the same time from overwriting each other's unconsumed parameters;
    // L2capLeConnectComplete consumes the slot matched by address
    L2capLePendingConnParam pendingConnParams[L2CAP_LE_GRANTED_CONN_PARAM_MAX];

    // granted LE connection parameters stashed from the LE Connection Complete HCI event and applied when
    // the ACL completes; the L2CAP connection object is created asynchronously on the L2CAP queue after
    // the event, so an inbound connection (established without a L2CAP_LeConnect request) would otherwise
    // keep connIntervalUnits == 0 and degrade the Vol 3 Part G 5.4 slave retry delay to its 100 ms floor.
    // A small per-handle slot array keeps two connections completing back-to-back from overwriting each
    // other's unconsumed parameters; L2capLeConnectComplete consumes the slot matched by handle
    L2capLeGrantedConnParam grantedConnParams[L2CAP_LE_GRANTED_CONN_PARAM_MAX];

    List *psmList;   // Pack struct L2capLePsm
    List *connList;  // Pack struct L2capLeConnection
} L2capLeInstance;

extern L2capLeInstance g_l2capLeInst;

// internal LE helpers of l2cap_le.c used by the EATT signaling of l2cap_le_eatt.c
L2capLePsm *L2capLeGetPsm(uint16_t lpsm);
L2capLeConnection *L2capLeGetConnection(uint16_t aclHandle);
L2capLeConnection *L2capLeGetConnection2(const BtAddr *addr);
L2capLeChannel *L2capLeGetChannel(L2capLeConnection *conn, int16_t lcid);
L2capLeChannel *L2capLeNewChannel(L2capLeConnection *conn, uint16_t lpsm, uint16_t rpsm);
void L2capLeDeleteChannel(L2capLeConnection *conn, L2capLeChannel *chan, uint16_t removeAcl);
L2capLeConnection *L2capLeNewConnection(const BtAddr *addr, uint16_t aclHandle, uint8_t role);
void L2capLeDeleteConnection(L2capLeConnection *conn);
uint8_t L2capLeGetNewIdentifier(L2capLeConnection *leconn);
int L2capLeSendDisconnectionReq(L2capLeConnection *conn, const L2capLeChannel *chan);
void L2capLeResponseTimeoutCallback(void *parameter);

// EATT helpers of l2cap_le_eatt.c used by the timeout and 0x16 handling of l2cap_le.c
int L2capLeEattSendPendingRequest(L2capLeConnection *conn);
void L2capLeEattDestroyReconfig(L2capLeConnection *conn, uint8_t identifier);
L2capLeChannel *L2capLeEattGetChannelByRcid(L2capLeConnection *conn, uint16_t rcid);

// EATT signaling dispatchers of l2cap_le_eatt.c used by the 0x17-0x1A dispatch of l2cap_le.c
void L2capLeEattProcessConnectionReq(uint16_t aclHandle, const L2capSignalHeader *signal, const uint8_t *data);
void L2capLeEattProcessConnectionRsp(uint16_t aclHandle, const L2capSignalHeader *signal, const uint8_t *data);
void L2capLeEattProcessReconfigureReq(uint16_t aclHandle, const L2capSignalHeader *signal, const uint8_t *data);
void L2capLeEattProcessReconfigureRsp(uint16_t aclHandle, const L2capSignalHeader *signal, const uint8_t *data);

// EATT establishment/reconfiguration helpers of l2cap_le_eatt.c used by the L2CIF request entry
// points of l2cap_le.c
int L2capLeEattValidateConnParams(const L2capLeConfigInfo *cfg, uint16_t n);
int L2capLeEattGetOrNewConnection(const BtAddr *addr, uint16_t n, L2capLeConnection **connOut);
int L2capLeEattCreateReqChannels(L2capLeConnection *conn, const L2capLeConfigInfo *cfg, uint16_t lcids[], uint16_t n);
int L2capLeEattCheckPendingBatchConfig(L2capLeConnection *conn, const L2capLeConfigInfo *cfg);
int L2capLeEattConnectOrSendPending(
    L2capLeConnection *conn, const BtAddr *addr, const uint16_t lcids[], uint16_t n);
int L2capLeEattSendReconfigureReq(
    L2capLeConnection *conn, uint16_t mtu, uint16_t mps, const L2capLeEattCidList *dcids);

// initialization state of l2cap_le.c, checked by the EATT entry helpers of l2cap_le_eatt.c
int L2capLeInitialized();

#endif // L2CAP_LE_INTERNAL_H
