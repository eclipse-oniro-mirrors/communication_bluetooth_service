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

#ifndef ATT_EATT_H
#define ATT_EATT_H

#include "att_common.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Register the EATT responder service on PSM 0x0027 and declare the local config.
 *
 * Called from ATT_StartUpAsync. The registration itself is asynchronous through the L2CAP
 * processing queue (L2CIF_LeRegisterService); the local config is applied in the registration
 * callback, after the PSM slot exists, because L2CAP_LeSetServiceConfig requires the registered
 * PSM to be findable (l2cap_le.c). Refer to charter 4.22/4.26 of Core 5.2.
 */
void AttEattRegisterService(void);

/**
 * @brief Establish EATT channels on the connection identified by connectHandle.
 *
 * Pure interface: the caller decides when to call it, ATT never initiates on its own.
 * Resolves the UATT bearer of the connection (retGattConnectHandle + LE_CID) and issues the 0x17
 * request through L2CIF_LeEattConnectionReq. The callback fires exactly once with the outcome of
 * the establishment: immediately with the dispatch failure when the 0x17 cannot be sent, otherwise
 * with the transaction-level result of the 0x18 connection response (BT_SUCCESS when every
 * requested channel was granted, BT_OPERATION_FAILED on a rejected or partial batch, or when the
 * batch channels are discarded abnormally) - the caller must not treat the dispatch as success.
 * Refer to charter 4.25/4.26 of Core 5.2.
 *
 * @param connectHandle connect handle (retGattConnectHandle) of the parent connection
 * @param localCfg local config (MTU/MPS/credit) of the channel batch, mtu/mps not less than 64
 * @param n number of channels to create, in [1, L2CAP_LE_EATT_MAX_CHANNEL]
 * @param cb dispatch callback (result, allocated source CIDs, count, user context)
 * @param ctx user context passed to cb
 * @return Returns <b>BT_SUCCESS</b> if the request is dispatched, otherwise the operation fails.
 */
int AttEattEstablish(uint16_t connectHandle, const L2capLeConfigInfo *localCfg, uint16_t n,
    void (*cb)(int result, const uint16_t *lcids, uint16_t n, void *ctx), void *ctx);

/**
 * @brief settle a pending EATT establishment with a single-shot completion callback.
 *
 * Notifies the AttEattEstablish caller exactly once (no-op when no establishment is pending) and
 * drops the retained batch state, cancelling the collision-retry fallback alarm. Exported for the
 * connection teardown path (AttClearConnectInfo): a pending establishment must be resolved before
 * the slot is cleared, or the caller's callback would be lost.
 *
 * @param1 parent Indicates the UATT bearer of the connection with the pending establishment.
 * @param2 result Indicates the outcome delivered to the callback (BT_SUCCESS / BT_OPERATION_FAILED).
 */
void AttEattResolveEstablish(AttConnectInfo *parent, int result);

/* EATT service callbacks registered on PSM 0x0027. */

/**
 * @brief handle an established EATT channel and turn it into an EATT bearer.
 *
 * The EATT channel rides on the ACL that already hosts the UATT bearer, so the parent connection
 * is located by its ACL handle and the new bearer reuses the parent's retGattConnectHandle: all
 * bearers of one physical connection are seen by the service layer as a single connectHandle.
 *
 * @param1 lcid Indicates the EATT dynamic CID.
 * @param2 info Indicates the parent connection (ACL handle + peer address).
 * @param3 cfg Indicates the peer's declared config for this channel.
 * @param4 ctx Indicates the pointer to context.
 */
void AttEattConnected(uint16_t lcid, const L2capConnectionInfo *info, const L2capLeConfigInfo *cfg, void *ctx);

/**
 * @brief record the outcome of an EATT 0x18 connection response.
 *
 * Channel establishment and 5.4 collision retry live in the L2CAP ECRED layer, so the ATT layer
 * only observes the result here; per-channel ATT_MTU setup is driven by AttEattConnected. The
 * transaction-level outcome settles the pending establishment (AttEattEstablish): the caller's
 * callback is invoked exactly once with BT_SUCCESS when every requested channel was granted,
 * BT_OPERATION_FAILED otherwise. Established channels of a partial batch stay usable.
 *
 * @param1 info Indicates the parent connection.
 * @param2 result Indicates the L2CAP result code of the 0x18 response.
 * @param3 attempted Indicates the number of channels the peer attempted.
 * @param4 succeeded Indicates the number of channels the peer granted.
 * @param5 ctx Indicates the pointer to context.
 */
void AttEattConnectionRsp(
    const L2capConnectionInfo *info, uint16_t result, uint8_t attempted, uint8_t succeeded, void *ctx);

/**
 * @brief refresh the bearer ATT_MTU after an EATT 0x1A reconfigure.
 *
 * On success (result == 0x0000) the newly negotiated MTU applies to this bearer; on failure the
 * previous config stays in effect (the L2CAP layer keeps the old values), Vol 3 Part G 5.3.1.
 *
 * @param1 lcid Indicates the EATT dynamic CID.
 * @param2 newMtu Indicates the newly negotiated MTU.
 * @param3 result Indicates the L2CAP result code.
 * @param4 ctx Indicates the pointer to context.
 */
void AttEattReconfigured(uint16_t lcid, uint16_t newMtu, uint16_t result, void *ctx);

/**
 * @brief handle an ATT PDU received on the EATT bearer.
 *
 * Ref'd on the L2CAP call stack (L2CAP does not own the packet), then dispatched on
 * the ATT processing queue: resolve the bearer by lcid, apply the bearer-level constraint
 * interception (0xD2 dropped per Vol 3 Part F §3.4.5.4, 0x02 answered with 0x01
 * ERROR_RSP / 0x06 per §3.2.8 + §3.3), then dispatch the remaining PDUs through the
 * shared FunctionList indexed by opcode.
 *
 * @param1 lcid Indicates the EATT dynamic CID.
 * @param2 pkt Indicates the received ATT packet.
 * @param3 ctx Indicates the pointer to context.
 */
void AttEattRecvLeData(uint16_t lcid, Packet *pkt, void *ctx);

/**
 * @brief handle the peer's L2CAP_DISCONNECTION_REQ (0x06) on the EATT bearer.
 *
 * Answers with the 0x07 L2CAP_DISCONNECTION_RSP (echoing the request id) so the peer can release
 * its channel, then releases the bearer (outstanding transactions aborted as 0x020E). Refer to
 * charter 4.6 of Core 5.2.
 *
 * @param1 lcid Indicates the EATT dynamic CID.
 * @param2 id Indicates the identifier of the received 0x06 request, echoed back in the 0x07 response.
 * @param3 ctx Indicates the pointer to context.
 */
void AttEattRecvLeDisconnectionReq(uint16_t lcid, uint8_t id, void *ctx);

/**
 * @brief handle the peer's L2CAP_DISCONNECTION_RSP (0x07) on the EATT bearer.
 *
 * The L2CAP layer has already freed the channel when this fires, so only the ATT bearer is
 * released (outstanding transactions aborted as 0x020E). Refer to charter 4.7 of Core 5.2.
 *
 * @param1 lcid Indicates the EATT dynamic CID.
 * @param2 ctx Indicates the pointer to context.
 */
void AttEattRecvLeDisconnectionRsp(uint16_t lcid, void *ctx);

/**
 * @brief handle an EATT channel disconnect and release the bearer.
 *
 * Every outstanding transaction of the dying bearer is reported as a transaction timeout (0x020E)
 * rather than dropped silently — pending operations fail visibly on channel close. A single EATT
 * bearer disconnect does not complete the GATT connection (that is reported only when the UATT
 * fixed channel goes away), so attLEDisconnectCompleted is intentionally not triggered here.
 *
 * @param1 lcid Indicates the EATT dynamic CID.
 * @param2 reason Indicates the disconnect reason.
 * @param3 ctx Indicates the pointer to context.
 */
void AttEattDisconnected(uint16_t lcid, uint8_t reason, void *ctx);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ATT_EATT_H
