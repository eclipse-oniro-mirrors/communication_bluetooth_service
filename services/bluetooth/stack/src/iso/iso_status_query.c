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

#include "log.h"

#include "hci/hci.h"

int IsoRegisterStatusQueryCallback(const IsoLeStatusQueryCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "NULL");
    IsoLeMng *mng = IsoGetMng();
    mng->statusQueryCallback = callback;
    mng->statusQueryCallbackContext = context;
    return BT_SUCCESS;
}

int IsoDeregisterStatusQueryCallback(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    IsoLeMng *mng = IsoGetMng();
    mng->statusQueryCallback = NULL;
    mng->statusQueryCallbackContext = NULL;
    return BT_SUCCESS;
}

int IsoLeReadIsoLinkQuality(uint16_t connectionHandle)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x", __FUNCTION__, connectionHandle);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }

    HciLeReadIsoLinkQualityParam param = {
        .connectionHandle = connectionHandle,
    };
    return HCI_LeReadIsoLinkQuality(&param);
}

int IsoLeReadIsoTxSync(uint16_t connectionHandle)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x", __FUNCTION__, connectionHandle);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }

    HciLeReadIsoTxSyncParam param = {
        .connectionHandle = connectionHandle,
    };
    return HCI_LeReadIsoTxSync(&param);
}

int IsoLeRequestPeerSca(uint16_t connectionHandle)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x", __FUNCTION__, connectionHandle);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }

    HciLeRequestPeerScaParam param = {
        .connectionHandle = connectionHandle,
    };
    return HCI_LeRequestPeerSca(&param);
}

void IsoLeReadIsoLinkQualityComplete(const HciLeReadIsoLinkQualityReturnParam *param)
{
    LOG_INFO(
        "%{public}s: status:0x%02x, connectionHandle:0x%04x", __FUNCTION__, param->status, param->connectionHandle);
    IsoLeMng *mng = IsoGetMng();
    IsoLeLinkQualityInfo info = {
        .connectionHandle = param->connectionHandle,
        .txUnackedPackets = param->txUnackedPackets,
        .txFlushedPackets = param->txFlushedPackets,
        .txLastSubeventPackets = param->txLastSubeventPackets,
        .retransmittedPackets = param->retransmittedPackets,
        .crcErrorPackets = param->crcErrorPackets,
        .rxUnreceivedPackets = param->rxUnreceivedPackets,
        .duplicatePackets = param->duplicatePackets,
    };
    if (mng->statusQueryCallback != NULL && mng->statusQueryCallback->readIsoLinkQualityResult != NULL) {
        mng->statusQueryCallback->readIsoLinkQualityResult(param->status, &info, mng->statusQueryCallbackContext);
    }
}

void IsoLeReadIsoTxSyncComplete(const HciLeReadIsoTxSyncReturnParam *param)
{
    LOG_INFO(
        "%{public}s: status:0x%02x, connectionHandle:0x%04x", __FUNCTION__, param->status, param->connectionHandle);
    IsoLeMng *mng = IsoGetMng();
    IsoLeTxSyncInfo info = {
        .connectionHandle = param->connectionHandle,
        .packetSequenceNumber = param->packetSequenceNumber,
        .timeStamp = IsoReadUint24(param->timeStamp),
        .timeOffset = IsoReadUint24(param->timeOffset),
    };
    if (mng->statusQueryCallback != NULL && mng->statusQueryCallback->readIsoTxSyncResult != NULL) {
        mng->statusQueryCallback->readIsoTxSyncResult(param->status, &info, mng->statusQueryCallbackContext);
    }
}

void IsoLeRequestPeerScaComplete(const HciLeRequestPeerScaCompleteEventParam *param)
{
    LOG_INFO(
        "%{public}s: status:0x%02x, connectionHandle:0x%04x", __FUNCTION__, param->status, param->connectionHandle);
    IsoLeMng *mng = IsoGetMng();
    IsoLePeerScaInfo info = {
        .connectionHandle = param->connectionHandle,
        .peerClockAccuracy = param->peerClockAccuracy,
    };
    if (mng->statusQueryCallback != NULL && mng->statusQueryCallback->requestPeerScaResult != NULL) {
        mng->statusQueryCallback->requestPeerScaResult(param->status, &info, mng->statusQueryCallbackContext);
    }
}
