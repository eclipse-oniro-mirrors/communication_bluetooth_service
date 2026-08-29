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

#include "log.h"

#include "hci/hci.h"
#include "hci/hci_error.h"

int IsoRegisterDataPathCallback(const IsoLeDataPathCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "NULL");
    IsoLeMng *mng = IsoGetMng();
    mng->dataPathCallback = callback;
    mng->dataPathCallbackContext = context;
    return BT_SUCCESS;
}

int IsoDeregisterDataPathCallback(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    IsoLeMng *mng = IsoGetMng();
    mng->dataPathCallback = NULL;
    mng->dataPathCallbackContext = NULL;
    return BT_SUCCESS;
}

int IsoLeSetupIsoDataPath(const IsoLeSetupIsoDataPathParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: connectionHandle:0x%04x, dataPathDirection:0x%02x, dataPathId:0x%02x", __FUNCTION__,
        param->connectionHandle, param->dataPathDirection, param->dataPathId);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }
    if (param->dataPathDirection > 0x01) {
        return BT_BAD_PARAM;
    }
    if (param->controllerDelay > 0xFFFFFF) {
        return BT_BAD_PARAM;
    }
    if (param->codecConfigurationLength > 0 && param->codecConfiguration == NULL) {
        return BT_BAD_PARAM;
    }

    HciLeSetupIsoDataPathParam hciParam = { 0 };
    hciParam.connectionHandle = param->connectionHandle;
    hciParam.dataPathDirection = param->dataPathDirection;
    hciParam.dataPathId = param->dataPathId;
    (void)memcpy_s(hciParam.codecId, sizeof(hciParam.codecId), param->codecId, sizeof(hciParam.codecId));
    IsoWriteUint24(hciParam.controllerDelay, param->controllerDelay);
    hciParam.codecConfigurationLength = param->codecConfigurationLength;
    hciParam.codecConfiguration = param->codecConfiguration;
    return HCI_LeSetupIsoDataPath(&hciParam);
}

int IsoLeRemoveIsoDataPath(uint16_t connectionHandle, uint8_t dataPathDirection)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x, dataPathDirection:0x%02x", __FUNCTION__, connectionHandle,
        dataPathDirection);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }

    HciLeRemoveIsoDataPathParam hciParam = {
        .connectionHandle = connectionHandle,
        .dataPathDirection = dataPathDirection,
    };
    return HCI_LeRemoveIsoDataPath(&hciParam);
}

void IsoLeSetupIsoDataPathComplete(const HciLeSetupIsoDataPathReturnParam *param)
{
    LOG_INFO(
        "%{public}s: status:0x%02x, connectionHandle:0x%04x", __FUNCTION__, param->status, param->connectionHandle);
    IsoLeMng *mng = IsoGetMng();
    if (mng->dataPathCallback != NULL && mng->dataPathCallback->setupIsoDataPathResult != NULL) {
        mng->dataPathCallback->setupIsoDataPathResult(
            param->status, param->connectionHandle, mng->dataPathCallbackContext);
    }
}

void IsoLeRemoveIsoDataPathComplete(const HciLeRemoveIsoDataPathReturnParam *param)
{
    LOG_INFO(
        "%{public}s: status:0x%02x, connectionHandle:0x%04x", __FUNCTION__, param->status, param->connectionHandle);
    IsoLeMng *mng = IsoGetMng();
    if (mng->dataPathCallback != NULL && mng->dataPathCallback->removeIsoDataPathResult != NULL) {
        mng->dataPathCallback->removeIsoDataPathResult(
            param->status, param->connectionHandle, mng->dataPathCallbackContext);
    }
}
