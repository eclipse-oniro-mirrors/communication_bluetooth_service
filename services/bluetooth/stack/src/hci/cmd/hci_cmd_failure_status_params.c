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

#include "hci_cmd_failure_status_params.h"

#include "btstack.h"
#include "platform/include/list.h"

#include "hci/evt/hci_evt.h"
#include "hci/hci.h"
#include "hci/hci_def.h"

#include "hci_cmd_failure.h"
#include "log.h"

static void HciCmdOnReadFailedContactCounterFailed(uint8_t status, const void *param)
{
    HciReadFailedContactCounterReturnParam returnParam = {
        .status = status,
        .handle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.handle = ((HciReadFailedContactCounterParam *)param)->handle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->readFailedContactCounterComplete != NULL) {
        callbacks->readFailedContactCounterComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnResetFailedContactCounterFailed(uint8_t status, const void *param)
{
    HciResetFailedContactCounterReturnParam returnParam = {
        .status = status,
        .handle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.handle = ((HciResetFailedContactCounterParam *)param)->handle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->resetFailedContactCounterComplete != NULL) {
        callbacks->resetFailedContactCounterComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnReadLinkQualityFailed(uint8_t status, const void *param)
{
    HciReadLinkQualityReturnParam returnParam = {
        .status = status,
        .handle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.handle = ((HciReadLinkQualityParam *)param)->handle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->readLinkQualityComplete != NULL) {
        callbacks->readLinkQualityComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnReadRssiFailed(uint8_t status, const void *param)
{
    HciReadRssiReturnParam returnParam = {
        .status = status,
        .handle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.handle = ((HciReadRssiParam *)param)->handle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->readRssiComplete != NULL) {
        callbacks->readRssiComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnReadAfhChannelMapFailed(uint8_t status, const void *param)
{
    HciReadAfhChannelMapReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle = ((HciReadAfhChannelMapParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->readAfhChannelMapComplete != NULL) {
        callbacks->readAfhChannelMapComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnReadClockFailed(uint8_t status, const void *param)
{
    HciReadClockReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle = ((HciReadClockParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->readClockComplete != NULL) {
        callbacks->readClockComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnReadEncryptionKeySizeFailed(uint8_t status, const void *param)
{
    HciReadEncryptionKeySizeReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle = ((HciReadEncryptionKeySizeParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->readEncryptionKeySizeComplete != NULL) {
        callbacks->readEncryptionKeySizeComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnReadLocalAmpInfoFailed(uint8_t status, const void *param)
{
    HciReadLocalAMPInfoReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->readLocalAmpInfoComplete != NULL) {
        callbacks->readLocalAmpInfoComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnReadLocalAmpAssocFailed(uint8_t status, const void *param)
{
    HciReadLocalAmpAssocReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->readLocalAmpAssocComplete != NULL) {
        callbacks->readLocalAmpAssocComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnWriteRemoteAmpAssocFailed(uint8_t status, const void *param)
{
    HciWriteRemoteAmpAssocReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->writeRemoteAmpAssocComplete != NULL) {
        callbacks->writeRemoteAmpAssocComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnSetTriggeredClockCaptureFailed(uint8_t status, const void *param)
{
    HciSetTriggeredClockCaptureReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->setTriggeredClockCaptureComplete != NULL) {
        callbacks->setTriggeredClockCaptureComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static HciCmdOnFailedFunc g_funcMap[] = {
    NULL,                                     // 0x0000
    HciCmdOnReadFailedContactCounterFailed,   // 0x0001
    HciCmdOnResetFailedContactCounterFailed,  // 0x0002
    HciCmdOnReadLinkQualityFailed,            // 0x0003
    NULL,                                     // 0x0004
    HciCmdOnReadRssiFailed,                   // 0x0005
    HciCmdOnReadAfhChannelMapFailed,          // 0x0006
    HciCmdOnReadClockFailed,                  // 0x0007
    HciCmdOnReadEncryptionKeySizeFailed,      // 0x0008
    HciCmdOnReadLocalAmpInfoFailed,           // 0x0009
    HciCmdOnReadLocalAmpAssocFailed,          // 0x000A
    HciCmdOnWriteRemoteAmpAssocFailed,        // 0x000B
    NULL,                                     // 0x000C
    HciCmdOnSetTriggeredClockCaptureFailed,   // 0x000D
};

#define STATUSPARAMETERS_OCF_MAX 0x000D

void HciOnStatusParametersCmdFailed(uint16_t opCode, uint8_t status, const void *param)
{
    uint16_t ocf = GET_OCF(opCode);
    if (ocf > STATUSPARAMETERS_OCF_MAX) {
        return;
    }

    HciCmdOnFailedFunc func = g_funcMap[ocf];
    if (func == NULL) {
        // No failure row for this OCF: the command's failure has nowhere to be reported and would
        // otherwise be dropped without a trace.
        LOG_WARN("%{public}s: opCode 0x%04x has no failure row", __FUNCTION__, opCode);
        return;
    }
    func(status, param);
}