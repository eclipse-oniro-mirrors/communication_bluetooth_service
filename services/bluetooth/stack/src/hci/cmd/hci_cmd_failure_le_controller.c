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

#include "hci_cmd_failure_le_controller.h"

#include <securec.h>

#include "btstack.h"
#include "platform/include/list.h"

#include "hci/evt/hci_evt.h"
#include "hci/hci.h"
#include "hci/hci_def.h"

#include "hci_cmd_failure.h"
#include "log.h"

#define INVALID_CONNECTION_HANDLE 0xFFFF

static void HciCmdOnLeSetEventMaskFailed(uint8_t status, const void *param)
{
    HciLeSetEventMaskReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetEventMaskComplete != NULL) {
        callbacks->leSetEventMaskComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadBufferSizeFailed(uint8_t status, const void *param)
{
    HciLeReadBufferSizeReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadBufferSizeComplete != NULL) {
        callbacks->leReadBufferSizeComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadLocalSupportedFeaturesFailed(uint8_t status, const void *param)
{
    HciLeReadLocalSupportedFeaturesReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadLocalSupportedFeaturesComplete != NULL) {
        callbacks->leReadLocalSupportedFeaturesComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetRandomAddressFailed(uint8_t status, const void *param)
{
    HciLeSetRandomAddressReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetRandomAddressComplete != NULL) {
        callbacks->leSetRandomAddressComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetAdvertisingParametersFailed(uint8_t status, const void *param)
{
    HciLeSetAdvertisingParametersReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetAdvertisingParametersComplete != NULL) {
        callbacks->leSetAdvertisingParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnReadAdvertisingChannelTxPowerFailed(uint8_t status, const void *param)
{
    HciLeReadAdvertisingChannelTxPowerReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadAdvertisingChannelTxPowerComplete != NULL) {
        callbacks->leReadAdvertisingChannelTxPowerComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetAdvertisingDataFailed(uint8_t status, const void *param)
{
    HciLeSetAdvertisingDataReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetAdvertisingDataComplete != NULL) {
        callbacks->leSetAdvertisingDataComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetScanResponseDataFailed(uint8_t status, const void *param)
{
    HciLeSetScanResponseDataReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetScanResponseDataComplete != NULL) {
        callbacks->leSetScanResponseDataComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetAdvertisingEnableFailed(uint8_t status, const void *param)
{
    HciLeSetAdvertisingEnableReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetAdvertisingEnableComplete != NULL) {
        callbacks->leSetAdvertisingEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetScanParametersFailed(uint8_t status, const void *param)
{
    HciLeSetScanParametersReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetScanParametersComplete != NULL) {
        callbacks->leSetScanParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetScanEnableFailed(uint8_t status, const void *param)
{
    HciLeSetScanEnableReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetScanEnableComplete != NULL) {
        callbacks->leSetScanEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeCreateConnectionFailed(uint8_t status, const void *param)
{
    HciLeConnectionCompleteEventParam eventParam = {
        .status = status,
        .peerAddress = {0},
        .peerAddressType = 0xFF,
    };

    if (param != NULL) {
        const HciLeCreateConnectionParam *cmdParam = (const HciLeCreateConnectionParam *)param;
        eventParam.peerAddress = cmdParam->peerAddress;
        eventParam.peerAddressType = cmdParam->peerAddressType;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid peer address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leConnectionComplete != NULL) {
        callbacks->leConnectionComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeCreateConnectionCancelFailed(uint8_t status, const void *param)
{
    HciLeCreateConnectionCancelReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCreateConnectionCancelComplete != NULL) {
        callbacks->leCreateConnectionCancelComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadWhiteListSizeFailed(uint8_t status, const void *param)
{
    HciLeReadWhiteListSizeReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadWhiteListSizeComplete != NULL) {
        callbacks->leReadWhiteListSizeComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeClearWhiteListFailed(uint8_t status, const void *param)
{
    HciLeClearWhiteListReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leClearWhiteListComplete != NULL) {
        callbacks->leClearWhiteListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeAddDeviceToWhiteListFailed(uint8_t status, const void *param)
{
    HciLeAddDeviceToWhiteListReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leAddDeviceToWhiteListComplete != NULL) {
        callbacks->leAddDeviceToWhiteListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeRemoveDeviceFromWhiteListFailed(uint8_t status, const void *param)
{
    HciLeRemoveDeviceFromWhiteListReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoveDeviceFromWhiteListComplete != NULL) {
        callbacks->leRemoveDeviceFromWhiteListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeConnectionUpdateFailed(uint8_t status, const void *param)
{
    HciLeConnectionUpdateCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
        .connInterval = 0,
        .connLatency = 0,
        .supervisionTimeout = 0,
    };

    if (param != NULL) {
        const HciLeConnectionUpdateParam *cmdParam = (const HciLeConnectionUpdateParam *)param;
        eventParam.connectionHandle = cmdParam->connectionHandle;
        eventParam.connLatency = cmdParam->connLatency;
        eventParam.supervisionTimeout = cmdParam->supervisionTimeout;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leConnectionUpdateComplete != NULL) {
        callbacks->leConnectionUpdateComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetHostChannelClassificationFailed(uint8_t status, const void *param)
{
    HciLeSetHostChannelClassificationReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetHostChannelClassificationComplete != NULL) {
        callbacks->leSetHostChannelClassificationComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadChannelMapFailed(uint8_t status, const void *param)
{
    HciLeReadChannelMapReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
        .channelMap = {0},
    };

    if (param != NULL) {
        returnParam.connectionHandle = ((const HciLeReadChannelMapParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadChannelMapComplete != NULL) {
        callbacks->leReadChannelMapComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadRemoteFeaturesFailed(uint8_t status, const void *param)
{
    HciLeReadRemoteFeaturesCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
        .leFeatures = {0},
    };

    if (param != NULL) {
        eventParam.connectionHandle = ((const HciLeReadRemoteFeaturesParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadRemoteFeaturesComplete != NULL) {
        callbacks->leReadRemoteFeaturesComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeEncryptFailed(uint8_t status, const void *param)
{
    HciLeEncryptReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leEncryptComplete != NULL) {
        callbacks->leEncryptComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeRandFailed(uint8_t status, const void *param)
{
    HciLeRandReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRandComplete != NULL) {
        callbacks->leRandComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeStartEncryptionFailed(uint8_t status, const void *param)
{
    HciEncryptionKeyRefreshCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        eventParam.connectionHandle = ((const HciLeStartEncryptionParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->encryptionKeyRefreshComplete != NULL) {
        callbacks->encryptionKeyRefreshComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeLongTermKeyRequestReplyFailed(uint8_t status, const void *param)
{
    HciLeLongTermKeyRequestReplyReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle = ((const HciLeLongTermKeyRequestReplyParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leLongTermKeyRequestReplyComplete != NULL) {
        callbacks->leLongTermKeyRequestReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeLongTermKeyRequestNegativeReplyFailed(uint8_t status, const void *param)
{
    HciLeLongTermKeyRequestNegativeReplyReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle =
            ((const HciLeLongTermKeyRequestNegativeReplyParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leLongTermKeyRequestNegativeReplyComplete != NULL) {
        callbacks->leLongTermKeyRequestNegativeReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadSupportedStatesFailed(uint8_t status, const void *param)
{
    HciLeReadSupportedStatesReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadSupportedStatesComplete != NULL) {
        callbacks->leReadSupportedStatesComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReceiverTestFailed(uint8_t status, const void *param)
{
    HciLeReceiverTestReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReceiverTestComplete != NULL) {
        callbacks->leReceiverTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeTransmitterTestFailed(uint8_t status, const void *param)
{
    HciLeTransmitterTestReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leTransmitterTestComplete != NULL) {
        callbacks->leTransmitterTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeTestEndFailed(uint8_t status, const void *param)
{
    HciLeTestEndReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leTestEndComplete != NULL) {
        callbacks->leTestEndComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeRemoteConnectionParameterRequestFailed(uint8_t status, const void *param)
{
    HciLeRemoteConnectionParameterRequestReplyReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle =
            ((const HciLeRemoteConnectionParameterRequestReplyParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoteConnectionParameterRequestReplyComplete != NULL) {
        callbacks->leRemoteConnectionParameterRequestReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeRemoteConnectionParameterRequestNegativeReplyFailed(uint8_t status, const void *param)
{
    HciLeRemoteConnectionParameterRequestNegativeReplyReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle =
            ((const HciLeRemoteConnectionParameterRequestNegativeReplyParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoteConnectionParameterRequestNegativeReplyComplete != NULL) {
        callbacks->leRemoteConnectionParameterRequestNegativeReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// These failure callbacks are invoked by HciOnCmdFailed after g_lockProcessingCmds is released.
// They do not (and must not) hold the event-callback list lock; HCI_FOREACH_EVT_CALLBACKS_START
// acquires it internally and releases it at END, so no MutexUnlock/MutexLock pairing is needed.
static void HciCmdOnLeSetDataLengthFailed(uint8_t status, const void *param)
{
    HciLeSetDataLengthReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle = ((const HciLeSetDataLengthParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetDataLengthComplete != NULL) {
        callbacks->leSetDataLengthComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadSuggestedDefaultDataLengthFailed(uint8_t status, const void *param)
{
    HciLeReadSuggestedDefaultDataLengthReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadSuggestedDefaultDataLengthComplete != NULL) {
        callbacks->leReadSuggestedDefaultDataLengthComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeWriteSuggestedDefaultDataLengthFailed(uint8_t status, const void *param)
{
    HciLeWriteSuggestedDefaultDataLengthReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leWriteSuggestedDefaultDataLengthComplete != NULL) {
        callbacks->leWriteSuggestedDefaultDataLengthComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadLocalP256PublicKeyFailed(uint8_t status, const void *param)
{
    HciLeReadLocalP256PublicKeyCompleteEventParam eventParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadLocalP256PublicKeyComplete != NULL) {
        callbacks->leReadLocalP256PublicKeyComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeGenerateDhKeyFailed(uint8_t status, const void *param)
{
    HciLeGenerateDHKeyCompleteEventParam eventParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leGenerateDHKeyComplete != NULL) {
        callbacks->leGenerateDHKeyComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeAddDeviceToResolvingListFailed(uint8_t status, const void *param)
{
    HciLeAddDeviceToResolvingListReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leAddDeviceToResolvingListComplete != NULL) {
        callbacks->leAddDeviceToResolvingListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeRemoveDeviceFromResolvingListFailed(uint8_t status, const void *param)
{
    HciLeRemoveDeviceFromResolvingListReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoveDeviceFromResolvingListComplete != NULL) {
        callbacks->leRemoveDeviceFromResolvingListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeClearResolvingListFailed(uint8_t status, const void *param)
{
    HciLeClearResolvingListReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leClearResolvingListComplete != NULL) {
        callbacks->leClearResolvingListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadResolvingListSizeFailed(uint8_t status, const void *param)
{
    HciLeReadResolvingListSizeReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadResolvingListSizeComplete != NULL) {
        callbacks->leReadResolvingListSizeComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadPeerResolvableAddressFailed(uint8_t status, const void *param)
{
    HciLeReadPeerResolvableAddressReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadPeerResolvableAddressComplete != NULL) {
        callbacks->leReadPeerResolvableAddressComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadLocalResolvableAddressFailed(uint8_t status, const void *param)
{
    HciLeReadLocalResolvableAddressReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadLocalResolvableAddressComplete != NULL) {
        callbacks->leReadLocalResolvableAddressComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetAddressResolutionEnableFailed(uint8_t status, const void *param)
{
    HciLeSetAddressResolutionEnableReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetAddressResolutionEnableComplete != NULL) {
        callbacks->leSetAddressResolutionEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetResolvablePrivateAddressTimeoutFailed(uint8_t status, const void *param)
{
    HciLeSetResolvablePrivateAddressTimeoutReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetResolvablePrivateAddressTimeoutComplete != NULL) {
        callbacks->leSetResolvablePrivateAddressTimeoutComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadMaximumDataLengthFailed(uint8_t status, const void *param)
{
    HciLeReadMaximumDataLengthReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadMaximumDataLengthComplete != NULL) {
        callbacks->leReadMaximumDataLengthComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadPhyFailed(uint8_t status, const void *param)
{
    HciLeReadPhyReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
        .txPhy = 0,
        .rxPhy = 0,
    };

    if (param != NULL) {
        returnParam.connectionHandle = ((const HciLeReadPhyParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadPhyComplete != NULL) {
        callbacks->leReadPhyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetDefaultPhyFailed(uint8_t status, const void *param)
{
    HciLeSetDefaultPhyReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetDefaultPhyComplete != NULL) {
        callbacks->leSetDefaultPhyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetPhyFailed(uint8_t status, const void *param)
{
    HciLeSetPhyReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPhyComplete != NULL) {
        callbacks->leSetPhyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeEnhancedReceiverTestFailed(uint8_t status, const void *param)
{
    HciLeEnhancedReceiverTestReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leEnhancedReceiverTestComplete != NULL) {
        callbacks->leEnhancedReceiverTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeEnhancedTransmitterTestFailed(uint8_t status, const void *param)
{
    HciLeEnhancedTransmitterTestReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leEnhancedTransmitterTestComplete != NULL) {
        callbacks->leEnhancedTransmitterTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetAdvertisingSetRandomAddressFailed(uint8_t status, const void *param)
{
    HciLeSetAdvertisingSetRandomAddressReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetAdvertisingSetRandomAddressComplete != NULL) {
        callbacks->leSetAdvertisingSetRandomAddressComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetExtendedAdvertisingParametersFailed(uint8_t status, const void *param)
{
    HciLeSetExtendedAdvertisingParametersReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetExtendedAdvertisingParametersComplete != NULL) {
        callbacks->leSetExtendedAdvertisingParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetExtendedAdvertisingDataFailed(uint8_t status, const void *param)
{
    HciLeSetExtendedAdvertisingDataReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetExtendedAdvertisingDataComplete != NULL) {
        callbacks->leSetExtendedAdvertisingDataComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetExtendedScanResponseDataFailed(uint8_t status, const void *param)
{
    HciLeSetExtendedScanResponseDataReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetExtendedScanResponseDataComplete != NULL) {
        callbacks->leSetExtendedScanResponseDataComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetExtendedAdvertisingEnableFailed(uint8_t status, const void *param)
{
    HciLeSetExtendedAdvertisingEnableReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetExtendedAdvertisingEnableComplete != NULL) {
        callbacks->leSetExtendedAdvertisingEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadMaximumAdvertisingDataLengthFailed(uint8_t status, const void *param)
{
    HciLeReadMaximumAdvertisingDataLengthReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadMaximumAdvertisingDataLengthComplete != NULL) {
        callbacks->leReadMaximumAdvertisingDataLengthComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadNumberofSupportedAdvertisingSetsFailed(uint8_t status, const void *param)
{
    HciLeReadNumberofSupportedAdvertisingSetsReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadNumberofSupportedAdvertisingSetsComplete != NULL) {
        callbacks->leReadNumberofSupportedAdvertisingSetsComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeRemoveAdvertisingSetFailed(uint8_t status, const void *param)
{
    HciLeRemoveAdvertisingSetReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoveAdvertisingSetComplete != NULL) {
        callbacks->leRemoveAdvertisingSetComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeClearAdvertisingSetsFailed(uint8_t status, const void *param)
{
    HciLeClearAdvertisingSetsReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leClearAdvertisingSetsComplete != NULL) {
        callbacks->leClearAdvertisingSetsComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetPeriodicAdvertisingParametersFailed(uint8_t status, const void *param)
{
    HciLeSetPeriodicAdvertisingParametersReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPeriodicAdvertisingParametersComplete != NULL) {
        callbacks->leSetPeriodicAdvertisingParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetPeriodicAdvertisingDataFailed(uint8_t status, const void *param)
{
    HciLeSetPeriodicAdvertisingDataReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPeriodicAdvertisingDataComplete != NULL) {
        callbacks->leSetPeriodicAdvertisingDataComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetPeriodicAdvertisingEnableFailed(uint8_t status, const void *param)
{
    HciLeSetPeriodicAdvertisingEnableReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPeriodicAdvertisingEnableComplete != NULL) {
        callbacks->leSetPeriodicAdvertisingEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetExtendedScanParametersFailed(uint8_t status, const void *param)
{
    HciLeSetExtendedScanParametersReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetExtendedScanParametersComplete != NULL) {
        callbacks->leSetExtendedScanParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetExtendedScanEnableFailed(uint8_t status, const void *param)
{
    HciLeSetExtendedScanEnableReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetExtendedScanEnableComplete != NULL) {
        callbacks->leSetExtendedScanEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeExtendedCreateConnectionFailed(uint8_t status, const void *param)
{
    HciLeEnhancedConnectionCompleteEventParam eventParam = {
        .status = status,
        .peerAddressType = 0xFF,
        .peerAddress = {0},
    };

    if (param != NULL) {
        const HciLeExtendedCreateConnectionParam *cmdParam = (const HciLeExtendedCreateConnectionParam *)param;
        eventParam.peerAddressType = cmdParam->peerAddressType;
        eventParam.peerAddress = cmdParam->peerAddress;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid peer address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leEnhancedConnectionComplete != NULL) {
        callbacks->leEnhancedConnectionComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLePeriodicAdvertisingCreateSyncCancelFailed(uint8_t status, const void *param)
{
    HciLePeriodicAdvertisingCreateSyncCancelReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->lePeriodicAdvertisingCreateSyncCancelComplete != NULL) {
        callbacks->lePeriodicAdvertisingCreateSyncCancelComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLePeriodicAdvertisingCreateSyncFailed(uint8_t status, const void *param)
{
    HciLePeriodicAdvertisingSyncEstablishedEventParam eventParam = {
        .status = status,
        .syncHandle = 0xFFFF,
        .advertisingSid = 0xFF,
        .advertiserAddressType = 0xFF,
        .advertiserAddress = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .advertiserPhy = 0x00,
        .periodicAdvertisingInterval = 0xFFFF,
        .advertiserClockAccuracy = 0xFF,
    };

    if (param != NULL) {
        const HciLePeriodicAdvertisingCreateSyncParam *createParam =
            (const HciLePeriodicAdvertisingCreateSyncParam *)param;
        eventParam.advertisingSid = createParam->advertisingSid;
        eventParam.advertiserAddressType = createParam->advertiserAddressType;
        if (memcpy_s(eventParam.advertiserAddress.raw, sizeof(eventParam.advertiserAddress.raw),
                 createParam->advertiserAddress.raw, sizeof(createParam->advertiserAddress.raw)) != EOK) {
            LOG_WARN("%{public}s: memcpy_s advertiserAddress failed", __FUNCTION__);
        }
        // HCI LE Periodic Advertising Create Sync command does not carry advertiserPhy; keep the invalid default.
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid target info", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->lePeriodicAdvertisingSyncEstablished != NULL) {
        callbacks->lePeriodicAdvertisingSyncEstablished(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLePeriodicAdvertisingTerminateSyncFailed(uint8_t status, const void *param)
{
    HciLePeriodicAdvertisingTerminateSyncReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->lePeriodicAdvertisingTerminateSyncComplete != NULL) {
        callbacks->lePeriodicAdvertisingTerminateSyncComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeAddDeviceToPeriodicAdvertiserListFailed(uint8_t status, const void *param)
{
    HciLeAddDeviceToPeriodicAdvertiserListReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leAddDeviceToPeriodicAdvertiserListComplete != NULL) {
        callbacks->leAddDeviceToPeriodicAdvertiserListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeRemoveDeviceFromPeriodicAdvertiserListFailed(uint8_t status, const void *param)
{
    HciLeRemoveDeviceFromPeriodicAdvertiserListReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoveDeviceFromPeriodicAdvertiserListComplete != NULL) {
        callbacks->leRemoveDeviceFromPeriodicAdvertiserListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeClearPeriodicAdvertiserListFailed(uint8_t status, const void *param)
{
    HciLeClearPeriodicAdvertiserListReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leClearPeriodicAdvertiserListComplete != NULL) {
        callbacks->leClearPeriodicAdvertiserListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadPeriodicAdvertiserListSizeFailed(uint8_t status, const void *param)
{
    HciLeReadPeriodicAdvertiserListSizeReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadPeriodicAdvertiserListSizeComplete != NULL) {
        callbacks->leReadPeriodicAdvertiserListSizeComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadTransmitPowerFailed(uint8_t status, const void *param)
{
    HciLeReadTransmitPowerReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadTransmitPowerComplete != NULL) {
        callbacks->leReadTransmitPowerComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadRfPathCompensationFailed(uint8_t status, const void *param)
{
    HciLeReadRfPathCompensationReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadRfPathCompensationComplete != NULL) {
        callbacks->leReadRfPathCompensationComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeWriteRfPathCompensationParamFailed(uint8_t status, const void *param)
{
    HciLeWriteRfPathCompensationReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leWriteRfPathCompensationComplete != NULL) {
        callbacks->leWriteRfPathCompensationComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetPrivacyModeFailed(uint8_t status, const void *param)
{
    HciLeSetPrivacyModeReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPrivacyModeComplete != NULL) {
        callbacks->leSetPrivacyModeComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReceiverTestV3Failed(uint8_t status, const void *param)
{
    HciLeReceiverTestV3ReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReceiverTestV3Complete != NULL) {
        callbacks->leReceiverTestV3Complete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeTransmitterTestV3Failed(uint8_t status, const void *param)
{
    HciLeTransmitterTestV3ReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leTransmitterTestV3Complete != NULL) {
        callbacks->leTransmitterTestV3Complete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetConnectionlessCteTransmitParametersFailed(uint8_t status, const void *param)
{
    HciLeSetConnectionlessCteTransmitParametersReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetConnectionlessCteTransmitParametersComplete != NULL) {
        callbacks->leSetConnectionlessCteTransmitParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetConnectionlessCteTransmitEnableFailed(uint8_t status, const void *param)
{
    HciLeSetConnectionlessCteTransmitEnableReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetConnectionlessCteTransmitEnableComplete != NULL) {
        callbacks->leSetConnectionlessCteTransmitEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetConnectionlessIqSamplingEnableFailed(uint8_t status, const void *param)
{
    HciLeSetConnectionlessIqSamplingEnableReturnParam returnParam = {
        .status = status,
        .syncHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.syncHandle = ((const HciLeSetConnectionlessIqSamplingEnableParam *)param)->syncHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetConnectionlessIqSamplingEnableComplete != NULL) {
        callbacks->leSetConnectionlessIqSamplingEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetConnectionCteReceiveParametersFailed(uint8_t status, const void *param)
{
    HciLeSetConnectionCteReceiveParametersReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle =
            ((const HciLeSetConnectionCteReceiveParametersParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetConnectionCteReceiveParametersComplete != NULL) {
        callbacks->leSetConnectionCteReceiveParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetConnectionCteTransmitParametersFailed(uint8_t status, const void *param)
{
    HciLeSetConnectionCteTransmitParametersReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle =
            ((const HciLeSetConnectionCteTransmitParametersParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetConnectionCteTransmitParametersComplete != NULL) {
        callbacks->leSetConnectionCteTransmitParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeConnectionCteRequestEnableFailed(uint8_t status, const void *param)
{
    HciLeConnectionCteRequestEnableReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle = ((const HciLeConnectionCteRequestEnableParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leConnectionCteRequestEnableComplete != NULL) {
        callbacks->leConnectionCteRequestEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeConnectionCteResponseEnableFailed(uint8_t status, const void *param)
{
    HciLeConnectionCteResponseEnableReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle = ((const HciLeConnectionCteResponseEnableParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leConnectionCteResponseEnableComplete != NULL) {
        callbacks->leConnectionCteResponseEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadAntennaInformationFailed(uint8_t status, const void *param)
{
    HciLeReadAntennaInformationReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadAntennaInformationComplete != NULL) {
        callbacks->leReadAntennaInformationComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetPeriodicAdvertisingReceiveEnableFailed(uint8_t status, const void *param)
{
    HciLeSetPeriodicAdvertisingReceiveEnableReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPeriodicAdvertisingReceiveEnableComplete != NULL) {
        callbacks->leSetPeriodicAdvertisingReceiveEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLePeriodicAdvertisingSyncTransferFailed(uint8_t status, const void *param)
{
    HciLePeriodicAdvertisingSyncTransferReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle =
            ((const HciLePeriodicAdvertisingSyncTransferParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->lePeriodicAdvertisingSyncTransferComplete != NULL) {
        callbacks->lePeriodicAdvertisingSyncTransferComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLePeriodicAdvertisingSetInfoTransferFailed(uint8_t status, const void *param)
{
    HciLePeriodicAdvertisingSetInfoTransferReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle =
            ((const HciLePeriodicAdvertisingSetInfoTransferParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->lePeriodicAdvertisingSetInfoTransferComplete != NULL) {
        callbacks->lePeriodicAdvertisingSetInfoTransferComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetPeriodicAdvertisingSyncTransferParametersFailed(uint8_t status, const void *param)
{
    HciLeSetPeriodicAdvertisingSyncTransferParametersReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle =
            ((const HciLeSetPeriodicAdvertisingSyncTransferParametersParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPeriodicAdvertisingSyncTransferParametersComplete != NULL) {
        callbacks->leSetPeriodicAdvertisingSyncTransferParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetDefaultPeriodicAdvertisingSyncTransferParametersFailed(uint8_t status, const void *param)
{
    HciLeSetDefaultPeriodicAdvertisingSyncTransferParametersReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetDefaultPeriodicAdvertisingSyncTransferParametersComplete != NULL) {
        callbacks->leSetDefaultPeriodicAdvertisingSyncTransferParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeModifySleepClockAccuracyFailed(uint8_t status, const void *param)
{
    HciLeModifySleepClockAccuracyReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leModifySleepClockAccuracyComplete != NULL) {
        callbacks->leModifySleepClockAccuracyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadIsoTxSyncFailed(uint8_t status, const void *param)
{
    // Failure events carry no return parameters, so the original command parameter
    // is reused to recover the handle. This relies on the first field of the
    // command parameter struct (Connection/BIG handle or CIG Identifier) being at
    // offset 0 with no padding, which matches the first octet(s) of the serialized
    // command. New failure handlers must keep this convention, or read the handle
    // from the wire payload explicitly instead.
    HciLeReadIsoTxSyncReturnParam returnParam = {
        .status = status,
        .connectionHandle = (param != NULL) ? ((const HciLeReadIsoTxSyncParam *)param)->connectionHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadIsoTxSyncComplete != NULL) {
        callbacks->leReadIsoTxSyncComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetCigParametersFailed(uint8_t status, const void *param)
{
    HciLeSetCigParametersReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        // The original command parameter carries the CIG Identifier as its first
        // octet, which is also the first octet of the wire payload.
        returnParam.cigId = ((const HciLeSetCigParametersParam *)param)->cigId;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, cigId left 0", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetCigParametersComplete != NULL) {
        callbacks->leSetCigParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetCigParametersTestFailed(uint8_t status, const void *param)
{
    HciLeSetCigParametersTestReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        // The original command parameter carries the CIG Identifier as its first
        // octet, which is also the first octet of the wire payload.
        returnParam.cigId = ((const HciLeSetCigParametersTestParam *)param)->cigId;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, cigId left 0", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetCigParametersTestComplete != NULL) {
        callbacks->leSetCigParametersTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeCreateCisFailed(uint8_t status, const void *param)
{
    HciLeCreateCisReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCreateCisComplete != NULL) {
        callbacks->leCreateCisComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeRemoveCigFailed(uint8_t status, const void *param)
{
    HciLeRemoveCigReturnParam returnParam = {
        .status = status,
        // echo the command's CIG_ID so the receiver can match it against the
        // pending remove; a synthesized failure must behave like the real
        // Command Complete of the same command
        .cigId = (param != NULL) ? ((const HciLeRemoveCigParam *)param)->cigId : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoveCigComplete != NULL) {
        callbacks->leRemoveCigComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// LE Accept CIS Request has no Command_Complete; its result is delivered by the
// LE CIS Established event. On failure, notify through the same event path.
static void HciCmdOnLeAcceptCisRequestFailed(uint8_t status, const void *param)
{
    HciLeCisEstablishedEventParam eventParam = {
        .status = status,
        .connectionHandle = (param != NULL) ? ((const HciLeAcceptCisRequestParam *)param)->cisHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCisEstablished != NULL) {
        callbacks->leCisEstablished(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeRejectCisRequestFailed(uint8_t status, const void *param)
{
    HciLeRejectCisRequestReturnParam returnParam = {
        .status = status,
        .cisHandle = (param != NULL) ? ((const HciLeRejectCisRequestParam *)param)->cisHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRejectCisRequestComplete != NULL) {
        callbacks->leRejectCisRequestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetHostFeatureFailed(uint8_t status, const void *param)
{
    HciLeSetHostFeatureReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetHostFeatureComplete != NULL) {
        callbacks->leSetHostFeatureComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeReadIsoLinkQualityFailed(uint8_t status, const void *param)
{
    HciLeReadIsoLinkQualityReturnParam returnParam = {
        .status = status,
        .connectionHandle = (param != NULL) ? ((const HciLeReadIsoLinkQualityParam *)param)->connectionHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadIsoLinkQualityComplete != NULL) {
        callbacks->leReadIsoLinkQualityComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeEnhancedReadTransmitPowerLevelFailed(uint8_t status, const void *param)
{
    HciLeEnhancedReadTransmitPowerLevelReturnParam returnParam = {
        .status = status,
        .connectionHandle =
            (param != NULL) ? ((const HciLeEnhancedReadTransmitPowerLevelParam *)param)->connectionHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leEnhancedReadTransmitPowerLevelComplete != NULL) {
        callbacks->leEnhancedReadTransmitPowerLevelComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x0077 has no Command_Complete; its result is delivered by the LE Transmit Power
// Reporting event (7.7.65,33) with Reason 0x02. On failure, notify through the same
// event path so callers do not wait forever.
static void HciCmdOnLeReadRemoteTransmitPowerLevelFailed(uint8_t status, const void *param)
{
    HciLeTransmitPowerReportingEventParam eventParam = {
        .status = status,
        .connectionHandle =
            (param != NULL) ? ((const HciLeReadRemoteTransmitPowerLevelParam *)param)->connectionHandle : 0,
        .reason = 0x02,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leTransmitPowerReporting != NULL) {
        callbacks->leTransmitPowerReporting(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetPathLossReportingParametersFailed(uint8_t status, const void *param)
{
    HciLeSetPathLossReportingParametersReturnParam returnParam = {
        .status = status,
        .connectionHandle =
            (param != NULL) ? ((const HciLeSetPathLossReportingParametersParam *)param)->connectionHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPathLossReportingParametersComplete != NULL) {
        callbacks->leSetPathLossReportingParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetPathLossReportingEnableFailed(uint8_t status, const void *param)
{
    HciLeSetPathLossReportingEnableReturnParam returnParam = {
        .status = status,
        .connectionHandle =
            (param != NULL) ? ((const HciLeSetPathLossReportingEnableParam *)param)->connectionHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPathLossReportingEnableComplete != NULL) {
        callbacks->leSetPathLossReportingEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetTransmitPowerReportingEnableFailed(uint8_t status, const void *param)
{
    HciLeSetTransmitPowerReportingEnableReturnParam returnParam = {
        .status = status,
        .connectionHandle =
            (param != NULL) ? ((const HciLeSetTransmitPowerReportingEnableParam *)param)->connectionHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetTransmitPowerReportingEnableComplete != NULL) {
        callbacks->leSetTransmitPowerReportingEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// LE Create BIG / Create BIG Test have no Command_Complete; their result is delivered by the
// LE Create BIG Complete event. On failure, notify through the same event path.
static void HciCmdOnLeCreateBigFailed(uint8_t status, const void *param)
{
    HciLeCreateBigCompleteEventParam eventParam = {
        .status = status,
        .bigHandle = (param != NULL) ? ((const HciLeCreateBigParam *)param)->bigHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCreateBigComplete != NULL) {
        callbacks->leCreateBigComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeCreateBigTestFailed(uint8_t status, const void *param)
{
    HciLeCreateBigCompleteEventParam eventParam = {
        .status = status,
        .bigHandle = (param != NULL) ? ((const HciLeCreateBigTestParam *)param)->bigHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCreateBigComplete != NULL) {
        callbacks->leCreateBigComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// LE Terminate BIG has no Command_Complete; its result is delivered by the
// LE Terminate BIG Complete event. On failure, notify through the same event path.
static void HciCmdOnLeTerminateBigFailed(uint8_t status, const void *param)
{
    HciLeTerminateBigCompleteEventParam eventParam = {
        .bigHandle = (param != NULL) ? ((const HciLeTerminateBigParam *)param)->bigHandle : 0,
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leTerminateBigComplete != NULL) {
        callbacks->leTerminateBigComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// LE Request Peer SCA has no Command_Complete; its result is delivered by the
// LE Request Peer SCA Complete event. On failure, notify through the same event path.
static void HciCmdOnLeRequestPeerScaFailed(uint8_t status, const void *param)
{
    HciLeRequestPeerScaCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = (param != NULL) ? ((const HciLeRequestPeerScaParam *)param)->connectionHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRequestPeerScaComplete != NULL) {
        callbacks->leRequestPeerScaComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// LE BIG Create Sync has no Command_Complete; its result is delivered by the
// LE BIG Sync Established event. On failure, notify through the same event path.
static void HciCmdOnLeBigCreateSyncFailed(uint8_t status, const void *param)
{
    HciLeBigSyncEstablishedEventParam eventParam = {
        .status = status,
        .bigHandle = (param != NULL) ? ((const HciLeBigCreateSyncParam *)param)->bigHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leBigSyncEstablished != NULL) {
        callbacks->leBigSyncEstablished(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeBigTerminateSyncFailed(uint8_t status, const void *param)
{
    HciLeBigTerminateSyncReturnParam returnParam = {
        .status = status,
        .bigHandle = (param != NULL) ? ((const HciLeBigTerminateSyncParam *)param)->bigHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leBigTerminateSyncComplete != NULL) {
        callbacks->leBigTerminateSyncComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeSetupIsoDataPathFailed(uint8_t status, const void *param)
{
    HciLeSetupIsoDataPathReturnParam returnParam = {
        .status = status,
        .connectionHandle = (param != NULL) ? ((const HciLeSetupIsoDataPathParam *)param)->connectionHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetupIsoDataPathComplete != NULL) {
        callbacks->leSetupIsoDataPathComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeRemoveIsoDataPathFailed(uint8_t status, const void *param)
{
    HciLeRemoveIsoDataPathReturnParam returnParam = {
        .status = status,
        .connectionHandle = (param != NULL) ? ((const HciLeRemoveIsoDataPathParam *)param)->connectionHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoveIsoDataPathComplete != NULL) {
        callbacks->leRemoveIsoDataPathComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeIsoTransmitTestFailed(uint8_t status, const void *param)
{
    HciLeIsoTransmitTestReturnParam returnParam = {
        .status = status,
        .connectionHandle = (param != NULL) ? ((const HciLeIsoTransmitTestParam *)param)->connectionHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leIsoTransmitTestComplete != NULL) {
        callbacks->leIsoTransmitTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeIsoReceiveTestFailed(uint8_t status, const void *param)
{
    HciLeIsoReceiveTestReturnParam returnParam = {
        .status = status,
        .connectionHandle = (param != NULL) ? ((const HciLeIsoReceiveTestParam *)param)->connectionHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leIsoReceiveTestComplete != NULL) {
        callbacks->leIsoReceiveTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeIsoReadTestCountersFailed(uint8_t status, const void *param)
{
    HciLeIsoReadTestCountersReturnParam returnParam = {
        .status = status,
        .connectionHandle = (param != NULL) ? ((const HciLeIsoReadTestCountersParam *)param)->connectionHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leIsoReadTestCountersComplete != NULL) {
        callbacks->leIsoReadTestCountersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLeIsoTestEndFailed(uint8_t status, const void *param)
{
    HciLeIsoTestEndReturnParam returnParam = {
        .status = status,
        .connectionHandle = (param != NULL) ? ((const HciLeIsoTestEndParam *)param)->connectionHandle : 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leIsoTestEndComplete != NULL) {
        callbacks->leIsoTestEndComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static HciCmdOnFailedFunc g_funcMap[] = {
    NULL,                                                                // 0x0000
    HciCmdOnLeSetEventMaskFailed,                                        // 0x0001
    HciCmdOnLeReadBufferSizeFailed,                                      // 0x0002
    HciCmdOnLeReadLocalSupportedFeaturesFailed,                          // 0x0003
    NULL,                                                                // 0x0004
    HciCmdOnLeSetRandomAddressFailed,                                    // 0x0005
    HciCmdOnLeSetAdvertisingParametersFailed,                            // 0x0006
    HciCmdOnReadAdvertisingChannelTxPowerFailed,                         // 0x0007
    HciCmdOnLeSetAdvertisingDataFailed,                                  // 0x0008
    HciCmdOnLeSetScanResponseDataFailed,                                 // 0x0009
    HciCmdOnLeSetAdvertisingEnableFailed,                                // 0x000A
    HciCmdOnLeSetScanParametersFailed,                                   // 0x000B
    HciCmdOnLeSetScanEnableFailed,                                       // 0x000C
    HciCmdOnLeCreateConnectionFailed,                                    // 0x000D
    HciCmdOnLeCreateConnectionCancelFailed,                              // 0x000E
    HciCmdOnLeReadWhiteListSizeFailed,                                   // 0x000F
    HciCmdOnLeClearWhiteListFailed,                                      // 0x0010
    HciCmdOnLeAddDeviceToWhiteListFailed,                                // 0x0011
    HciCmdOnLeRemoveDeviceFromWhiteListFailed,                           // 0x0012
    HciCmdOnLeConnectionUpdateFailed,                                    // 0x0013
    HciCmdOnLeSetHostChannelClassificationFailed,                        // 0x0014
    HciCmdOnLeReadChannelMapFailed,                                      // 0x0015
    HciCmdOnLeReadRemoteFeaturesFailed,                                  // 0x0016
    HciCmdOnLeEncryptFailed,                                             // 0x0017
    HciCmdOnLeRandFailed,                                                // 0x0018
    HciCmdOnLeStartEncryptionFailed,                                     // 0x0019
    HciCmdOnLeLongTermKeyRequestReplyFailed,                             // 0x001A
    HciCmdOnLeLongTermKeyRequestNegativeReplyFailed,                     // 0x001B
    HciCmdOnLeReadSupportedStatesFailed,                                 // 0x001C
    HciCmdOnLeReceiverTestFailed,                                        // 0x001D
    HciCmdOnLeTransmitterTestFailed,                                     // 0x001E
    HciCmdOnLeTestEndFailed,                                             // 0x001F
    HciCmdOnLeRemoteConnectionParameterRequestFailed,                    // 0x0020
    HciCmdOnLeRemoteConnectionParameterRequestNegativeReplyFailed,       // 0x0021
    HciCmdOnLeSetDataLengthFailed,                                       // 0x0022
    HciCmdOnLeReadSuggestedDefaultDataLengthFailed,                      // 0x0023
    HciCmdOnLeWriteSuggestedDefaultDataLengthFailed,                     // 0x0024
    HciCmdOnLeReadLocalP256PublicKeyFailed,                              // 0x0025
    HciCmdOnLeGenerateDhKeyFailed,                                       // 0x0026
    HciCmdOnLeAddDeviceToResolvingListFailed,                            // 0x0027
    HciCmdOnLeRemoveDeviceFromResolvingListFailed,                       // 0x0028
    HciCmdOnLeClearResolvingListFailed,                                  // 0x0029
    HciCmdOnLeReadResolvingListSizeFailed,                               // 0x002A
    HciCmdOnLeReadPeerResolvableAddressFailed,                           // 0x002B
    HciCmdOnLeReadLocalResolvableAddressFailed,                          // 0x002C
    HciCmdOnLeSetAddressResolutionEnableFailed,                          // 0x002D
    HciCmdOnLeSetResolvablePrivateAddressTimeoutFailed,                  // 0x002E
    HciCmdOnLeReadMaximumDataLengthFailed,                               // 0x002F
    HciCmdOnLeReadPhyFailed,                                             // 0x0030
    HciCmdOnLeSetDefaultPhyFailed,                                       // 0x0031
    HciCmdOnLeSetPhyFailed,                                              // 0x0032
    HciCmdOnLeEnhancedReceiverTestFailed,                                // 0x0033
    HciCmdOnLeEnhancedTransmitterTestFailed,                             // 0x0034
    HciCmdOnLeSetAdvertisingSetRandomAddressFailed,                      // 0x0035
    HciCmdOnLeSetExtendedAdvertisingParametersFailed,                    // 0x0036
    HciCmdOnLeSetExtendedAdvertisingDataFailed,                          // 0x0037
    HciCmdOnLeSetExtendedScanResponseDataFailed,                         // 0x0038
    HciCmdOnLeSetExtendedAdvertisingEnableFailed,                        // 0x0039
    HciCmdOnLeReadMaximumAdvertisingDataLengthFailed,                    // 0x003A
    HciCmdOnLeReadNumberofSupportedAdvertisingSetsFailed,                // 0x003B
    HciCmdOnLeRemoveAdvertisingSetFailed,                                // 0x003C
    HciCmdOnLeClearAdvertisingSetsFailed,                                // 0x003D
    HciCmdOnLeSetPeriodicAdvertisingParametersFailed,                    // 0x003E
    HciCmdOnLeSetPeriodicAdvertisingDataFailed,                          // 0x003F
    HciCmdOnLeSetPeriodicAdvertisingEnableFailed,                        // 0x0040
    HciCmdOnLeSetExtendedScanParametersFailed,                           // 0x0041
    HciCmdOnLeSetExtendedScanEnableFailed,                               // 0x0042
    HciCmdOnLeExtendedCreateConnectionFailed,                            // 0x0043
    HciCmdOnLePeriodicAdvertisingCreateSyncFailed,                       // 0x0044
    HciCmdOnLePeriodicAdvertisingCreateSyncCancelFailed,                 // 0x0045
    HciCmdOnLePeriodicAdvertisingTerminateSyncFailed,                    // 0x0046
    HciCmdOnLeAddDeviceToPeriodicAdvertiserListFailed,                   // 0x0047
    HciCmdOnLeRemoveDeviceFromPeriodicAdvertiserListFailed,              // 0x0048
    HciCmdOnLeClearPeriodicAdvertiserListFailed,                         // 0x0049
    HciCmdOnLeReadPeriodicAdvertiserListSizeFailed,                      // 0x004A
    HciCmdOnLeReadTransmitPowerFailed,                                   // 0x004B
    HciCmdOnLeReadRfPathCompensationFailed,                              // 0x004C
    HciCmdOnLeWriteRfPathCompensationParamFailed,                        // 0x004D
    HciCmdOnLeSetPrivacyModeFailed,                                      // 0x004E
    HciCmdOnLeReceiverTestV3Failed,                                      // 0x004F
    HciCmdOnLeTransmitterTestV3Failed,                                   // 0x0050
    HciCmdOnLeSetConnectionlessCteTransmitParametersFailed,              // 0x0051
    HciCmdOnLeSetConnectionlessCteTransmitEnableFailed,                  // 0x0052
    HciCmdOnLeSetConnectionlessIqSamplingEnableFailed,                   // 0x0053
    HciCmdOnLeSetConnectionCteReceiveParametersFailed,                   // 0x0054
    HciCmdOnLeSetConnectionCteTransmitParametersFailed,                  // 0x0055
    HciCmdOnLeConnectionCteRequestEnableFailed,                          // 0x0056
    HciCmdOnLeConnectionCteResponseEnableFailed,                         // 0x0057
    HciCmdOnLeReadAntennaInformationFailed,                              // 0x0058
    HciCmdOnLeSetPeriodicAdvertisingReceiveEnableFailed,                 // 0x0059
    HciCmdOnLePeriodicAdvertisingSyncTransferFailed,                     // 0x005A
    HciCmdOnLePeriodicAdvertisingSetInfoTransferFailed,                  // 0x005B
    HciCmdOnLeSetPeriodicAdvertisingSyncTransferParametersFailed,        // 0x005C
    HciCmdOnLeSetDefaultPeriodicAdvertisingSyncTransferParametersFailed, // 0x005D
    HciCmdOnLeGenerateDhKeyFailed,                                       // 0x005E
    HciCmdOnLeModifySleepClockAccuracyFailed,                            // 0x005F
    NULL,                                                                // 0x0060
    HciCmdOnLeReadIsoTxSyncFailed,                                       // 0x0061
    HciCmdOnLeSetCigParametersFailed,                                    // 0x0062
    HciCmdOnLeSetCigParametersTestFailed,                                // 0x0063
    HciCmdOnLeCreateCisFailed,                                           // 0x0064
    HciCmdOnLeRemoveCigFailed,                                           // 0x0065
    HciCmdOnLeAcceptCisRequestFailed,                                    // 0x0066
    HciCmdOnLeRejectCisRequestFailed,                                    // 0x0067
    HciCmdOnLeCreateBigFailed,                                           // 0x0068
    HciCmdOnLeCreateBigTestFailed,                                       // 0x0069
    HciCmdOnLeTerminateBigFailed,                                        // 0x006A
    HciCmdOnLeBigCreateSyncFailed,                                       // 0x006B
    HciCmdOnLeBigTerminateSyncFailed,                                    // 0x006C
    HciCmdOnLeRequestPeerScaFailed,                                      // 0x006D
    HciCmdOnLeSetupIsoDataPathFailed,                                    // 0x006E
    HciCmdOnLeRemoveIsoDataPathFailed,                                   // 0x006F
    HciCmdOnLeIsoTransmitTestFailed,                                     // 0x0070
    HciCmdOnLeIsoReceiveTestFailed,                                      // 0x0071
    HciCmdOnLeIsoReadTestCountersFailed,                                 // 0x0072
    HciCmdOnLeIsoTestEndFailed,                                          // 0x0073
    HciCmdOnLeSetHostFeatureFailed,                                      // 0x0074
    HciCmdOnLeReadIsoLinkQualityFailed,                                  // 0x0075
    HciCmdOnLeEnhancedReadTransmitPowerLevelFailed,                      // 0x0076
    HciCmdOnLeReadRemoteTransmitPowerLevelFailed,                        // 0x0077
    HciCmdOnLeSetPathLossReportingParametersFailed,                      // 0x0078
    HciCmdOnLeSetPathLossReportingEnableFailed,                          // 0x0079
    HciCmdOnLeSetTransmitPowerReportingEnableFailed,                     // 0x007A
};

// 0x005E is 7.8.93 LE Generate DHKey [v2] (Key_Type variant): the failure path
// dispatches the same leGenerateDHKeyComplete callback as v1 (0x0026).
#define LECONTROLLER_OCF_MAX 0x007A

void HciOnLeControllerCmdFailed(uint16_t opCode, uint8_t status, const void *param)
{
    uint16_t ocf = GET_OCF(opCode);
    if (ocf > LECONTROLLER_OCF_MAX) {
        return;
    }

    HciCmdOnFailedFunc func = g_funcMap[ocf];
    if (func != NULL) {
        func(status, param);
    }
}