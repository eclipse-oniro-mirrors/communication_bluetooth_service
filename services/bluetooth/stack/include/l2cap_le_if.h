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
 * @file l2cap_le_if.h
 *
 * @brief Interface of bluetooth l2cap protocol LE part
 *
 */

#ifndef L2CAP_LE_IF_H
#define L2CAP_LE_IF_H

#include "l2cap_def.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * @brief Register l2cap le_psm for LE Credit Based Connection
 *
 * @param lpsm protocol psm
 * @param svc callback for protocol psm
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
int BTSTACK_API L2CIF_LeRegisterService(
    uint16_t lpsm, const L2capLeService *svc, void *context, void (*cb)(uint16_t lpsm, int result));

/**
 * @brief Deregister l2cap le_psm for LE Credit Based Connection
 *
 * @param lpsm protocol psm
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
void BTSTACK_API L2CIF_LeDeregisterService(uint16_t lpsm, void (*cb)(uint16_t lpsm, int result));

/**
 * @brief Send LE Credit Based Connection Request packet
 *
 * @param addr remote bluetooth address
 * @param cfg config parameter
 * @param lcid OUT parameter, local channel id
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
int BTSTACK_API L2CIF_LeCreditBasedConnectionReq(const BtAddr *addr, uint16_t lpsm, uint16_t rpsm,
    const L2capLeConfigInfo *cfg, void (*cb)(const BtAddr *addr, uint16_t lcid, int result));

/**
 * @briefSend LE Credit Based Connection Response packet
 *
 * @param lcid local channel id
 * @param id identifier of l2cap command
 * @param cfg config parameter
 * @param result result of connection
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
int BTSTACK_API L2CIF_LeCreditBasedConnectionRsp(
    uint16_t lcid, uint8_t id, const L2capLeConfigInfo *cfg, uint16_t result, void (*cb)(uint16_t lcid, int result));

/**
 * @brief Send Disconnection Request packet
 *
 * @param lcid local channel id
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
void BTSTACK_API L2CIF_LeDisconnectionReq(uint16_t lcid, void (*cb)(uint16_t lcid, int result));

/**
 * @brief Send Disconnection Response packet
 *
 * @param lcid local channel id
 * @param id identifier of l2cap command
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
void BTSTACK_API L2CIF_LeDisconnectionRsp(uint16_t lcid, uint8_t id, void (*cb)(uint16_t lcid, int result));

/**
 * @brief Send Le data packet for LE Credit Based Connection
 *
 * @param lcid local channel id
 * @param pkt packet of data
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
int BTSTACK_API L2CIF_LeSendData(uint16_t lcid, const Packet *pkt, void (*cb)(uint16_t lcid, int result));

/**
 * @brief Send an Enhanced Credit Based Connection Request (0x17) for the EATT PSM 0x0027
 *
 * Runs on the L2CAP processing queue to stay serialized with the inbound-signal handling.
 * The allocated source CIDs are returned through the callback: lcids points to an internal
 * array of n entries valid only during the callback, so the caller must consume them (or
 * copy them) synchronously. The callback reports only the local dispatch result; the real
 * connection result is delivered asynchronously per 0x18 through the registered service's
 * recvLeEattConnectionRsp. Refer to charter 4.25 of Core 5.2.
 *
 * @param addr remote bluetooth address
 * @param cfg local config (MTU/MPS/credit), mtu and mps not less than 64
 * @param n number of channels to create, in [1, L2CAP_LE_EATT_MAX_CHANNEL]
 * @param cb dispatch callback (result, allocated source CIDs, count, user context)
 * @param ctx user context passed to cb
 * @return Returns <b>BT_SUCCESS</b> if the request is dispatched, otherwise the operation fails.
 */
int BTSTACK_API L2CIF_LeEattConnectionReq(const BtAddr *addr, const L2capLeConfigInfo *cfg, uint16_t n,
    void (*cb)(int result, const uint16_t *lcids, uint16_t n, void *ctx), void *ctx);

/**
 * @brief Send an Enhanced Credit Based Reconfigure Request (0x19) for the EATT PSM 0x0027
 *
 * Runs on the L2CAP processing queue to stay serialized with the inbound-signal handling.
 * The target local CIDs are copied into the L2CIF context, so the caller's array need not outlive
 * this call. The callback reports only the local dispatch result (parameter checks in the L2CAP
 * layer, charter 4.27); the real per-channel result is delivered asynchronously per 0x1A through
 * the registered service's recvLeEattReconfigured. Refer to charter 4.27 of Core 5.2.
 *
 * @param params pointer to the reconfigure parameters (target CIDs, new MTU/MPS and
 *               the dispatch callback); the target CIDs are copied into the L2CIF context
 * @return Returns <b>BT_SUCCESS</b> if the request is dispatched, otherwise the operation fails.
 */
typedef struct {
    const uint16_t *lcids;             // target local CIDs to reconfigure, n entries
    uint16_t n;                        // number of CIDs, in [1, L2CAP_LE_EATT_MAX_CHANNEL]
    uint16_t mtu;                      // new receive MTU, range validated in the L2CAP layer
    uint16_t mps;                      // new receive MPS, range validated in the L2CAP layer
    void (*cb)(int result, void *ctx); // dispatch callback (result, user context), may be NULL
    void *ctx;                         // user context passed to cb
} L2CIF_LeEattReconfigureParams;

int BTSTACK_API L2CIF_LeEattReconfigureReq(const L2CIF_LeEattReconfigureParams *params);

/**
 * @brief Register LE Fix Channel data callback
 *
 * @param cid fix channel id
 * @param chan callback of fix channel
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
int BTSTACK_API L2CIF_LeRegisterFixChannel(
    uint16_t cid, const L2capLeFixChannel *chan, void (*cb)(uint16_t cid, int result));

/**
 * @brief Deregister LE Fix Channel data callback
 *
 * @param cid fix channel id
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
void BTSTACK_API L2CIF_LeDeregisterFixChannel(uint16_t cid, void (*cb)(uint16_t cid, int result));

/**
 * @brief Create Le ACL connection
 *
 * @param addr remote bluetooth address
 * @param param connection parameter
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
int BTSTACK_API L2CIF_LeConnect(
    const BtAddr *addr, const L2capLeConnectionParameter *param, void (*cb)(const BtAddr *addr, int result));

/**
 * @brief Cancel Le ACL connection
 *
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
int BTSTACK_API L2CIF_LeConnectCancel(const BtAddr *addr);

/**
 * @brief Destroy Le ACL connection
 *
 * @param aclHandle ACL handle
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
void BTSTACK_API L2CIF_LeDisconnect(uint16_t aclHandle, void (*cb)(uint16_t aclHandle, int result));

/**
 * @brief Send LE Fix Channel data
 *
 * @param aclHandle ACL handle
 * @param cid fix channel id
 * @param pkt packet of data
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
int BTSTACK_API L2CIF_LeSendFixChannelData(
    uint16_t aclHandle, uint16_t cid, Packet *pkt, void (*cb)(uint16_t aclHandle, int result));

/**
 * @brief Register LE connection parameter update
 *
 * @param cb callback for connection parameter update
 * @param context context of caller
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
int BTSTACK_API L2CIF_LeRegisterConnectionParameterUpdate(const L2capLeConnectionParameterUpdate *cb, void *context);

/**
 * @brief Deregister LE connection parameter update
 *
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
void BTSTACK_API L2CIF_LeDeregisterConnectionParameterUpdate();

/**
 * @brief Send Connection Parameter Update Request packet,
 *        This command shall only be sent from the LE peripheral device to the LE center .
 *
 * @param aclHandle ACL handle
 * @param param connection parameter
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
int BTSTACK_API L2CIF_LeConnectionParameterUpdateReq(
    uint16_t aclHandle, const L2capLeConnectionParameter *param, void (*cb)(uint16_t aclHandle, int result));

/**
 * @brief Send Connection Parameter Update Response packet
 *
 * @param aclHandle ACL handle
 * @param id cidentifier of l2cap command
 * @param result result of the request
 * @return Returns <b>BT_SUCCESS</b> if the operation is successful, otherwise the operation fails.
 */
void BTSTACK_API L2CIF_LeConnectionParameterUpdateRsp(
    uint16_t aclHandle, uint8_t id, uint16_t result, void (*cb)(uint16_t aclHandle, int result));

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // L2CAP_LE_IF_H
