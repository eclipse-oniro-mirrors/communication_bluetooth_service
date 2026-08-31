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

static bool IsoFindBigByBigHandle(void *data, void *parameter)
{
    IsoBigInfo *bigInfo = data;
    uint8_t bigHandle = *(uint8_t *)parameter;
    return bigInfo->bigHandle == bigHandle;
}

static IsoBigInfo *IsoFindBig(IsoLeMng *mng, uint8_t bigHandle)
{
    return ListForEachData(mng->bigBlock.bigList, IsoFindBigByBigHandle, &bigHandle);
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
    // BLUETOOTH SPECIFICATION Version 5.2 | Vol 4, Part E
    // 7.8.103 LE Create BIG: BIG_Handle 0x00-0xEF, RTN 0x00-0x1F, PHY 0x00-0x02,
    // Packing/Framing 0x00-0x01, Encryption 0x00-0x01; Advertising_Handle is validated
    // by the controller.
    if (bigHandle > 0xEF || bigParam->rtn > 0x1F || bigParam->phy > 0x02 || bigParam->packing > 0x01 ||
        bigParam->framing > 0x01 || bigParam->encryption > 0x01) {
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
    // BLUETOOTH SPECIFICATION Version 5.2 | Vol 4, Part E
    // 7.8.104 LE Create BIG Test: BIG_Handle 0x00-0xEF, RTN 0x00-0x1F, PHY 0x00-0x02,
    // Packing/Framing 0x00-0x01, Encryption 0x00-0x01; Advertising_Handle is validated
    // by the controller.
    if (bigHandle > 0xEF || bigParam->rtn > 0x1F || bigParam->phy > 0x02 || bigParam->packing > 0x01 ||
        bigParam->framing > 0x01 || bigParam->encryption > 0x01) {
        return BT_BAD_PARAM;
    }

    HciLeCreateBigTestParam hciParam = { 0 };
    hciParam.bigHandle = bigHandle;
    hciParam.advertisingHandle = advertisingHandle;
    hciParam.numBis = numBis;
    IsoWriteUint24(hciParam.sduInterval, bigParam->sduInterval);
    hciParam.isoInterval = bigParam->isoInterval;
    hciParam.numberOfSdu = bigParam->numberOfSdu;
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

// Track a successfully created BIG. A duplicate success of an already-tracked handle
// (a retransmitted event, or a late one after the Create BIG command timed out at the
// HCI layer, i5) is dropped: the tracking entry already exists (only a Terminate BIG
// Complete removes it), so a second createBigResult success must not reach the upper
// layer. Returns false when the duplicate must not be reported. On tracking failures
// (allocation/insert error) the event is still reported, only the tracking is lost.
static bool IsoCreateBigTrack(IsoLeMng *mng, const HciLeCreateBigCompleteEventParam *param, uint8_t numBis)
{
    IsoBigInfo *bigInfo = IsoFindBig(mng, param->bigHandle);
    if (bigInfo != NULL) {
        HILOGW("%{public}s: duplicate Create BIG Complete, drop, bigHandle:0x%02x", __FUNCTION__,
            param->bigHandle);
        return false;
    }
    bigInfo = MEM_MALLOC.alloc(sizeof(IsoBigInfo));
    if (bigInfo == NULL) {
        // Not tracked: a later termination of this BIG is neither filtered nor reported
        // (IsoLeTerminateBigComplete / IsoLeBigSyncLostEvent).
        HILOGE("%{public}s: alloc IsoBigInfo failed, bigHandle:0x%04x not tracked", __FUNCTION__, param->bigHandle);
        return true;
    }
    (void)memset_s(bigInfo, sizeof(IsoBigInfo), 0x00, sizeof(IsoBigInfo));
    bigInfo->bigHandle = param->bigHandle;
    if (!ListAddLast(mng->bigBlock.bigList, bigInfo)) {
        // Not tracked: a later termination of this BIG is neither filtered nor reported
        // (IsoLeTerminateBigComplete / IsoLeBigSyncLostEvent).
        HILOGE("%{public}s: tracking insert failed, bigHandle:0x%04x not tracked", __FUNCTION__, param->bigHandle);
        MEM_MALLOC.free(bigInfo);
        return true;
    }
    bigInfo->bisCount = numBis;
    (void)memcpy_s(bigInfo->bisHandles, sizeof(bigInfo->bisHandles), param->bisHandles, numBis * sizeof(uint16_t));
    return true;
}

void IsoLeCreateBigComplete(const HciLeCreateBigCompleteEventParam *param)
{
    LOG_INFO("%{public}s: status:0x%02x, bigHandle:0x%02x, numBis:%hhu", __FUNCTION__, param->status, param->bigHandle,
        param->numBis);

    // Defense-in-depth: the HCI parser already clamps numBis, but clamp locally as well
    // before using it as a memcpy length, so a malformed event cannot overflow bisHandles.
    uint8_t numBis = (param->numBis > ISO_LE_BIS_COUNT_MAX) ? ISO_LE_BIS_COUNT_MAX : param->numBis;
    if (param->numBis > ISO_LE_BIS_COUNT_MAX) {
        HILOGE("%{public}s: invalid numBis:%hhu, clamp to %hhu", __FUNCTION__, param->numBis, numBis);
    }

    IsoLeMng *mng = IsoGetMng();
    if (param->status == HCI_SUCCESS && !IsoCreateBigTrack(mng, param, numBis)) {
        return;
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
        mng->bigCallback->createBigResult(param->status, &info, mng->bigCallbackContext);
    }
}

void IsoLeTerminateBigComplete(const HciLeTerminateBigCompleteEventParam *param)
{
    LOG_INFO("%{public}s: status:0x%02x, bigHandle:0x%02x", __FUNCTION__, param->status, param->bigHandle);
    IsoLeMng *mng = IsoGetMng();
    if (param->status == HCI_SUCCESS) {
        IsoBigInfo *bigInfo = IsoFindBig(mng, param->bigHandle);
        if (bigInfo != NULL) {
            ListRemoveNode(mng->bigBlock.bigList, bigInfo);
        }
    }

    if (mng->bigCallback != NULL && mng->bigCallback->terminateBigResult != NULL) {
        mng->bigCallback->terminateBigResult(param->status, mng->bigCallbackContext);
    }
}

void IsoLeBigSyncEstablishedEvent(const HciLeBigSyncEstablishedEventParam *param)
{
    LOG_INFO("%{public}s: status:0x%02x, bigHandle:0x%02x, numBis:%hhu", __FUNCTION__, param->status, param->bigHandle,
        param->numBis);

    // Defense-in-depth: clamp numBis before using it as a memcpy length (see
    // IsoLeCreateBigComplete), a malformed event must not overflow bisHandles.
    uint8_t numBis = (param->numBis > ISO_LE_BIS_COUNT_MAX) ? ISO_LE_BIS_COUNT_MAX : param->numBis;
    if (param->numBis > ISO_LE_BIS_COUNT_MAX) {
        HILOGE("%{public}s: invalid numBis:%hhu, clamp to %hhu", __FUNCTION__, param->numBis, numBis);
    }

    IsoLeMng *mng = IsoGetMng();

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

    if (mng->bigCallback != NULL && mng->bigCallback->bigSyncEstablished != NULL) {
        mng->bigCallback->bigSyncEstablished(param->status, &info, mng->bigCallbackContext);
    }
}

void IsoLeBigSyncLostEvent(const HciLeBigSyncLostEventParam *param)
{
    LOG_INFO("%{public}s: bigHandle:0x%02x, reason:0x%02x", __FUNCTION__, param->bigHandle, param->reason);
    IsoLeMng *mng = IsoGetMng();
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
    if (mng->bigCallback != NULL && mng->bigCallback->bigTerminateSyncResult != NULL) {
        mng->bigCallback->bigTerminateSyncResult(param->status, mng->bigCallbackContext);
    }
}
