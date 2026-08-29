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

#ifndef ISO_H
#define ISO_H

// little-endian byte packing constants shared by the ISO HCI helpers, Vol 2 Part E 5.1.1
#define ISO_UINT8_BITS 8     // bits per octet, shift amount between consecutive bytes
#define ISO_UINT24_BYTES 3   // octets of a 24-bit little-endian value
#define ISO_UINT16_BYTES 2   // octets of a 16-bit little-endian value

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "btstack.h"
#include "iso_le_if.h"

#include "alarm.h"
#include "list.h"

#include "hci/hci_def_evt.h"
#include "hci/hci_def_le_cmd.h"
#include "hci/hci_def_le_evt.h"

// little-endian byte packing helpers shared by the ISO HCI modules, Vol 2 Part E 5.1.1
static inline void IsoWriteUint24(uint8_t *dst, uint32_t value)
{
    for (uint8_t i = 0; i < ISO_UINT24_BYTES; i++) {
        dst[i] = (uint8_t)((value >> (ISO_UINT8_BITS * i)) & 0xFF);
    }
}

static inline void IsoWriteUint16(uint8_t *dst, uint16_t value)
{
    for (uint8_t i = 0; i < ISO_UINT16_BYTES; i++) {
        dst[i] = (uint8_t)((value >> (ISO_UINT8_BITS * i)) & 0xFF);
    }
}

static inline uint32_t IsoReadUint24(const uint8_t *src)
{
    uint32_t value = 0;
    for (uint8_t i = 0; i < ISO_UINT24_BYTES; i++) {
        value |= (uint32_t)src[i] << (ISO_UINT8_BITS * i);
    }
    return value;
}

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t cigId;
    uint8_t cisCount;
    uint16_t cisHandles[ISO_LE_CIS_COUNT_MAX];
} IsoCigInfo;

typedef struct {
    List *cigList;
} IsoCigBlock;

typedef struct {
    uint8_t bigHandle;
    uint8_t bisCount;
    uint16_t bisHandles[ISO_LE_BIS_COUNT_MAX];
} IsoBigInfo;

typedef struct {
    List *bigList;
} IsoBigBlock;

typedef struct {
    bool valid;
    uint8_t cigId;
    // Monotonic generation of the pending remove: incremented on every issue so the
    // watchdog task of a PREVIOUS remove (posted before its own Complete arrived and
    // already superseded by a new remove of the same CIG_ID) cannot clear the pending
    // slot of the current remove. The timeout task matches valid && cigId && sequence;
    // the Command Complete payload carries no generation, so the Complete handler is
    // gated on the slot being armed instead (see IsoLeRemoveCigComplete). uint16_t
    // wrap needs 65536 removes x 10 s watchdog = ~7.6 days, beyond any run length.
    uint16_t sequence;
    Alarm *timer;  // one-shot watchdog of the LE Remove CIG command, see IsoLeRemoveCig
} IsoRemoveCigPending;

// Watchdog entry of an outstanding LE Accept CIS request, one per accepted CIS
// (the LE CIS Request event can carry several concurrent requests, i6). LE Accept
// CIS is answered either by an error Command Complete (dropped silently by
// HciEventOnLeAcceptCisRequestCommandComplete) or by the LE CIS Established event,
// so a request that never produces either would otherwise leave the upper layer's
// cisEstablished callback pending forever; the watchdog synthesizes a failure
// notification instead (see IsoLeAcceptCisRequest / IsoAcceptCisTimeoutProcess).
typedef struct {
    bool valid;
    uint16_t cisHandle;
    // Monotonic generation of the accept, incremented on every arm so a stale timeout
    // task of a PREVIOUS accept of the same handle (e.g. a re-request after a timeout)
    // cannot resolve the entry of the current accept (see IsoAcceptCisTimeoutProcess).
    uint16_t sequence;
    // Whether the LE CIS Established answer of the current generation already arrived:
    // the Established handler (matched by valid && handle only, the event payload carries
    // no generation) only marks the entry as received and never touches the timer, so a
    // late answer of a PREVIOUS accept of the same handle cannot cancel the CURRENT
    // generation's watchdog. The generation-matched timeout task reads this flag to tell
    // "answer arrived, trailing cleanup" from "answer lost, synthesize failure", then
    // resets both valid and received.
    bool received;
    Alarm *timer;  // one-shot watchdog, created on demand, cancelled on disable, deleted on finalize
} IsoAcceptCisPending;

// Active CIS tracking entry for disconnection notification filtering.
// Only the CIS Connection Handle is needed: the LE CIS Established event (0x19)
// does not carry the CIG/CIS identifiers back, and the Disconnection Complete
// event (0x05) is matched by handle only.
typedef struct {
    uint16_t cisHandle;
} IsoCisInfo;

typedef struct {
    bool isEnable;
    IsoCigBlock cigBlock;
    IsoRemoveCigPending removePending;
    IsoAcceptCisPending acceptCisPendings[ISO_LE_CIS_COUNT_MAX];
    List *cisList;
    const IsoLeCigCallback *callback;
    void *callbackContext;
    IsoBigBlock bigBlock;
    const IsoLeBigCallback *bigCallback;
    void *bigCallbackContext;
    const IsoLeDataPathCallback *dataPathCallback;
    void *dataPathCallbackContext;
    const IsoLeTestCallback *testCallback;
    void *testCallbackContext;
    const IsoLeStatusQueryCallback *statusQueryCallback;
    void *statusQueryCallbackContext;
} IsoLeMng;

bool IsoIsEnable(void);
IsoLeMng *IsoGetMng(void);

/**
 * @brief       Register CIG management result callback function
 * @param[in]   callback            CIG management result callback structure
 * @param[in]   context             CIG management result callback context parameter
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
int IsoRegisterCigCallback(const IsoLeCigCallback *callback, void *context);

/**
 * @brief       Deregister CIG management result callback function
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
int IsoDeregisterCigCallback(void);

/**
 * @brief       Create a CIG, send LE Set CIG Parameters command
 * @param[in]   cigId               CIG Identifier, chosen by the host (0x00-0xEF)
 * @param[in]   cigParam            CIG-level parameters
 * @param[in]   cisCount            Number of CIS in the CIG (1-16)
 * @param[in]   cisParams           Per-CIS configuration array, cisCount entries
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
int IsoLeCreateCig(uint8_t cigId, const IsoLeCigParam *cigParam, uint8_t cisCount, const IsoLeCisParam *cisParams);

/**
 * @brief       Create one or more CIS, send LE Create CIS command
 * @param[in]   cisCount            Number of CIS to establish (1-16)
 * @param[in]   params              Per-CIS array of {CIS handle, ACL handle}, cisCount entries
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
int IsoLeCreateCis(uint8_t cisCount, const IsoLeCreateCisParam *params);

/**
 * @brief       Remove a CIG, send LE Remove CIG command
 * @param[in]   cigId               CIG Identifier (0x00-0xEF)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
int IsoLeRemoveCig(uint8_t cigId);

int IsoLeAcceptCisRequest(uint16_t cisHandle);
int IsoLeRejectCisRequest(uint16_t cisHandle, uint8_t reason);
int IsoLeDisconnectCis(uint16_t cisHandle, uint8_t reason);

int IsoRegisterBigCallback(const IsoLeBigCallback *callback, void *context);
int IsoDeregisterBigCallback(void);

int IsoLeCreateBig(uint8_t bigHandle, uint8_t advertisingHandle, uint8_t numBis, const IsoLeBigParam *bigParam,
    const uint8_t *broadcastCode);
int IsoLeCreateBigTest(uint8_t bigHandle, uint8_t advertisingHandle, uint8_t numBis, const IsoLeBigTestParam *bigParam,
    const uint8_t *broadcastCode);
int IsoLeTerminateBig(uint8_t bigHandle, uint8_t reason);
int IsoLeBigCreateSync(const IsoLeBigCreateSyncParam *param);
int IsoLeBigTerminateSync(uint8_t bigHandle);

int IsoRegisterDataPathCallback(const IsoLeDataPathCallback *callback, void *context);
int IsoDeregisterDataPathCallback(void);
int IsoLeSetupIsoDataPath(const IsoLeSetupIsoDataPathParam *param);
int IsoLeRemoveIsoDataPath(uint16_t connectionHandle, uint8_t dataPathDirection);

int IsoRegisterTestCallback(const IsoLeTestCallback *callback, void *context);
int IsoDeregisterTestCallback(void);
int IsoLeIsoTransmitTest(uint16_t connectionHandle, uint8_t payloadType);
int IsoLeIsoReceiveTest(uint16_t connectionHandle, uint8_t payloadType);
int IsoLeIsoReadTestCounters(uint16_t connectionHandle);
int IsoLeIsoTestEnd(uint16_t connectionHandle);

int IsoRegisterStatusQueryCallback(const IsoLeStatusQueryCallback *callback, void *context);
int IsoDeregisterStatusQueryCallback(void);
int IsoLeReadIsoLinkQuality(uint16_t connectionHandle);
int IsoLeReadIsoTxSync(uint16_t connectionHandle);
int IsoLeRequestPeerSca(uint16_t connectionHandle);

void IsoLeSetCigParametersComplete(const HciLeSetCigParametersReturnParam *param);
void IsoLeCreateCisComplete(const HciLeCreateCisReturnParam *param);
void IsoLeRemoveCigComplete(const HciLeRemoveCigReturnParam *param);
void IsoLeRejectCisRequestComplete(const HciLeRejectCisRequestReturnParam *param);
void IsoLeCisRequestEvent(const HciLeCisRequestEventParam *param);
void IsoLeCisEstablishedEvent(const HciLeCisEstablishedEventParam *param);
void IsoLeDisconnectComplete(const HciDisconnectCompleteEventParam *param);

void IsoLeCreateBigComplete(const HciLeCreateBigCompleteEventParam *param);
void IsoLeTerminateBigComplete(const HciLeTerminateBigCompleteEventParam *param);
void IsoLeBigSyncEstablishedEvent(const HciLeBigSyncEstablishedEventParam *param);
void IsoLeBigSyncLostEvent(const HciLeBigSyncLostEventParam *param);
void IsoLeBigInfoAdvertisingReportEvent(const HciLeBigInfoAdvertisingReportEventParam *param);
void IsoLeBigTerminateSyncComplete(const HciLeBigTerminateSyncReturnParam *param);
void IsoLeSetupIsoDataPathComplete(const HciLeSetupIsoDataPathReturnParam *param);
void IsoLeRemoveIsoDataPathComplete(const HciLeRemoveIsoDataPathReturnParam *param);
void IsoLeIsoTransmitTestComplete(const HciLeIsoTransmitTestReturnParam *param);
void IsoLeIsoReceiveTestComplete(const HciLeIsoReceiveTestReturnParam *param);
void IsoLeIsoReadTestCountersComplete(const HciLeIsoReadTestCountersReturnParam *param);
void IsoLeIsoTestEndComplete(const HciLeIsoTestEndReturnParam *param);
void IsoLeReadIsoLinkQualityComplete(const HciLeReadIsoLinkQualityReturnParam *param);
void IsoLeReadIsoTxSyncComplete(const HciLeReadIsoTxSyncReturnParam *param);
void IsoLeRequestPeerScaComplete(const HciLeRequestPeerScaCompleteEventParam *param);

void IsoRecvLeSetCigParametersComplete(const HciLeSetCigParametersReturnParam *param);
void IsoRecvLeCreateCisComplete(const HciLeCreateCisReturnParam *param);
void IsoRecvLeRemoveCigComplete(const HciLeRemoveCigReturnParam *param);
void IsoRecvLeRejectCisRequestComplete(const HciLeRejectCisRequestReturnParam *param);
void IsoRecvLeCisRequest(const HciLeCisRequestEventParam *param);
void IsoRecvLeCisEstablished(const HciLeCisEstablishedEventParam *param);
void IsoRecvLeDisconnectComplete(const HciDisconnectCompleteEventParam *param);

void IsoRecvLeCreateBigComplete(const HciLeCreateBigCompleteEventParam *param);
void IsoRecvLeTerminateBigComplete(const HciLeTerminateBigCompleteEventParam *param);
void IsoRecvLeBigSyncEstablished(const HciLeBigSyncEstablishedEventParam *param);
void IsoRecvLeBigSyncLost(const HciLeBigSyncLostEventParam *param);
void IsoRecvLeBigInfoAdvertisingReport(const HciLeBigInfoAdvertisingReportEventParam *param);
void IsoRecvLeBigTerminateSyncComplete(const HciLeBigTerminateSyncReturnParam *param);
void IsoRecvLeSetupIsoDataPathComplete(const HciLeSetupIsoDataPathReturnParam *param);
void IsoRecvLeRemoveIsoDataPathComplete(const HciLeRemoveIsoDataPathReturnParam *param);
void IsoRecvLeIsoTransmitTestComplete(const HciLeIsoTransmitTestReturnParam *param);
void IsoRecvLeIsoReceiveTestComplete(const HciLeIsoReceiveTestReturnParam *param);
void IsoRecvLeIsoReadTestCountersComplete(const HciLeIsoReadTestCountersReturnParam *param);
void IsoRecvLeIsoTestEndComplete(const HciLeIsoTestEndReturnParam *param);
void IsoRecvLeReadIsoLinkQualityComplete(const HciLeReadIsoLinkQualityReturnParam *param);
void IsoRecvLeReadIsoTxSyncComplete(const HciLeReadIsoTxSyncReturnParam *param);
void IsoRecvLeRequestPeerScaComplete(const HciLeRequestPeerScaCompleteEventParam *param);

void IsoRegisterHciEventCallbacks(void);
void IsoDeregisterHciEventCallbacks(void);

#ifdef __cplusplus
}
#endif

#endif // ISO_H
