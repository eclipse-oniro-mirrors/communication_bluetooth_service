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
#include "hci/hci_error.h"

#define ISO_LE_PAYLOAD_TYPE_MAX 0x02

int IsoRegisterTestCallback(const IsoLeTestCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "NULL");
    IsoLeMng *mng = IsoGetMng();
    mng->testCallback = callback;
    mng->testCallbackContext = context;
    return BT_SUCCESS;
}

int IsoDeregisterTestCallback(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    IsoLeMng *mng = IsoGetMng();
    mng->testCallback = NULL;
    mng->testCallbackContext = NULL;
    return BT_SUCCESS;
}

int IsoLeIsoTransmitTest(uint16_t connectionHandle, uint8_t payloadType)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x, payloadType:0x%02x", __FUNCTION__, connectionHandle, payloadType);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }
    if (payloadType > ISO_LE_PAYLOAD_TYPE_MAX) {
        return BT_BAD_PARAM;
    }

    HciLeIsoTransmitTestParam param = {
        .connectionHandle = connectionHandle,
        .payloadType = payloadType,
    };
    return HCI_LeIsoTransmitTest(&param);
}

int IsoLeIsoReceiveTest(uint16_t connectionHandle, uint8_t payloadType)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x, payloadType:0x%02x", __FUNCTION__, connectionHandle, payloadType);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }
    if (payloadType > ISO_LE_PAYLOAD_TYPE_MAX) {
        return BT_BAD_PARAM;
    }

    HciLeIsoReceiveTestParam param = {
        .connectionHandle = connectionHandle,
        .payloadType = payloadType,
    };
    return HCI_LeIsoReceiveTest(&param);
}

int IsoLeIsoReadTestCounters(uint16_t connectionHandle)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x", __FUNCTION__, connectionHandle);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }

    HciLeIsoReadTestCountersParam param = {
        .connectionHandle = connectionHandle,
    };
    return HCI_LeIsoReadTestCounters(&param);
}

int IsoLeIsoTestEnd(uint16_t connectionHandle)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x", __FUNCTION__, connectionHandle);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }

    HciLeIsoTestEndParam param = {
        .connectionHandle = connectionHandle,
    };
    return HCI_LeIsoTestEnd(&param);
}

void IsoLeIsoTransmitTestComplete(const HciLeIsoTransmitTestReturnParam *param)
{
    LOG_INFO(
        "%{public}s: status:0x%02x, connectionHandle:0x%04x", __FUNCTION__, param->status, param->connectionHandle);
    IsoLeMng *mng = IsoGetMng();
    if (mng->testCallback != NULL && mng->testCallback->transmitTestResult != NULL) {
        mng->testCallback->transmitTestResult(param->status, param->connectionHandle, mng->testCallbackContext);
    }
}

void IsoLeIsoReceiveTestComplete(const HciLeIsoReceiveTestReturnParam *param)
{
    LOG_INFO(
        "%{public}s: status:0x%02x, connectionHandle:0x%04x", __FUNCTION__, param->status, param->connectionHandle);
    IsoLeMng *mng = IsoGetMng();
    if (mng->testCallback != NULL && mng->testCallback->receiveTestResult != NULL) {
        mng->testCallback->receiveTestResult(param->status, param->connectionHandle, mng->testCallbackContext);
    }
}

void IsoLeIsoReadTestCountersComplete(const HciLeIsoReadTestCountersReturnParam *param)
{
    LOG_INFO(
        "%{public}s: status:0x%02x, connectionHandle:0x%04x", __FUNCTION__, param->status, param->connectionHandle);
    IsoLeMng *mng = IsoGetMng();
    IsoLeTestCountersInfo info = {
        .connectionHandle = param->connectionHandle,
        .receivedPacketCount = param->receivedPacketCount,
        .missedPacketCount = param->missedPacketCount,
        .failedPacketCount = param->failedPacketCount,
    };
    if (mng->testCallback != NULL && mng->testCallback->readTestCountersResult != NULL) {
        mng->testCallback->readTestCountersResult(param->status, &info, mng->testCallbackContext);
    }
}

void IsoLeIsoTestEndComplete(const HciLeIsoTestEndReturnParam *param)
{
    LOG_INFO(
        "%{public}s: status:0x%02x, connectionHandle:0x%04x", __FUNCTION__, param->status, param->connectionHandle);
    IsoLeMng *mng = IsoGetMng();
    IsoLeTestCountersInfo info = {
        .connectionHandle = param->connectionHandle,
        .receivedPacketCount = param->receivedPacketCount,
        .missedPacketCount = param->missedPacketCount,
        .failedPacketCount = param->failedPacketCount,
    };
    if (mng->testCallback != NULL && mng->testCallback->testEndResult != NULL) {
        mng->testCallback->testEndResult(param->status, &info, mng->testCallbackContext);
    }
}
