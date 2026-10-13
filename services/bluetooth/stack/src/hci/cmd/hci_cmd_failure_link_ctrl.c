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

#include "hci_cmd_failure_link_ctrl.h"

#include <securec.h>

#include "btm/btm_acl.h"
#include "btstack.h"
#include "platform/include/list.h"

#include "hci/evt/hci_evt.h"
#include "hci/hci.h"
#include "hci/hci_def.h"

#include "hci_cmd_failure.h"
#include "log.h"

static void HciCmdOnInquiryFailed(uint8_t status, const void *param)
{
    HciInquiryCompleteEventParam eventParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->inquiryComplete != NULL) {
        callbacks->inquiryComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnInquiryCancelFailed(uint8_t status, const void *param)
{
    HciInquiryCancelReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->inquiryCancelComplete != NULL) {
        callbacks->inquiryCancelComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnPeriodicInquiryModeFailed(uint8_t status, const void *param)
{
    HciPeriodicInquiryModeReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->periodicInquiryModeComplete != NULL) {
        callbacks->periodicInquiryModeComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnExitPeriodicInquiryModeFailed(uint8_t status, const void *param)
{
    HciExitPeriodicInquiryModeReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->exitPeriodicInquiryModeComplete != NULL) {
        callbacks->exitPeriodicInquiryModeComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnCreateConnectionFailed(uint8_t status, const void *param)
{
    HciConnectionCompleteEventParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
        .linkType = HCI_LINK_TYPE_ACL,
        .encryptionEnabled = 0,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciCreateConnectionParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->connectionComplete != NULL) {
        callbacks->connectionComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnDisconnectFailed(uint8_t status, const void *param)
{
    HciDisconnectCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        eventParam.connectionHandle = ((HciDisconnectParam *)param)->connectionHandle;
        eventParam.reason = ((HciDisconnectParam *)param)->reason;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->disconnectComplete != NULL) {
        callbacks->disconnectComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnCreateConnectionCancelFailed(uint8_t status, const void *param)
{
    HciCreateConnectionCancelReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciCreateConnectionCancelParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->createConnectionCancelComplete != NULL) {
        callbacks->createConnectionCancelComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnAcceptConnectionRequestFailed(uint8_t status, const void *param)
{
    HciConnectionCompleteEventParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciAcceptConnectionReqestParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->connectionComplete != NULL) {
        callbacks->connectionComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnRejectConnectionRequestFailed(uint8_t status, const void *param)
{
    HciConnectionCompleteEventParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciRejectConnectionRequestParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->connectionComplete != NULL) {
        callbacks->connectionComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLinkKeyRequestReplyFailed(uint8_t status, const void *param)
{
    HciLinkKeyRequestReplyReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciLinkKeyRequestReplyParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->linkKeyRequestReplyComplete != NULL) {
        callbacks->linkKeyRequestReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLinkKeyRequestNegativeReplyFailed(uint8_t status, const void *param)
{
    HciLinkKeyRequestNegativeReplyReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciLinkKeyRequestNegativeReplyParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->linkKeyRequestNegativeReplyComplete != NULL) {
        callbacks->linkKeyRequestNegativeReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnPinCodeRequestReplyFailed(uint8_t status, const void *param)
{
    HciPinCodeRequestReplyReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciPinCodeRequestReplyParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->pinCodeRequestReplyComplete != NULL) {
        callbacks->pinCodeRequestReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnPinCodeRequestNegativeReplyFailed(uint8_t status, const void *param)
{
    HciPinCodeRequestNegativeReplyReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciPinCodeRequestNegativeReplyParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->pinCodeRequestNegativeReplyComplete != NULL) {
        callbacks->pinCodeRequestNegativeReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnAuthenticationRequestedFailed(uint8_t status, const void *param)
{
    HciAuthenticationCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        eventParam.connectionHandle = ((HciAuthenticationRequestedParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->authenticationComplete != NULL) {
        callbacks->authenticationComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnSetConnectionEncryptionFailed(uint8_t status, const void *param)
{
    HciEncryptionChangeEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        eventParam.connectionHandle = ((HciSetConnectionEncryptionParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->encryptionChange != NULL) {
        callbacks->encryptionChange(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnRemoteNameRequestFailed(uint8_t status, const void *param)
{
    HciRemoteNameRequestCompleteEventParam eventParam = {
        .status = status,
    };

    if (param != NULL) {
        eventParam.bdAddr = ((HciRemoteNameRequestParam *)param)->addr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->remoteNameRequestComplete != NULL) {
        callbacks->remoteNameRequestComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnRemoteNameRequestCancelFailed(uint8_t status, const void *param)
{
    HciRemoteNameRequestCancelReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.addr = ((HciRemoteNameRequestCancelParam *)param)->addr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->remoteNameRequestCancelComplete != NULL) {
        callbacks->remoteNameRequestCancelComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnReadRemoteSupportedFeaturesFailed(uint8_t status, const void *param)
{
    HciReadRemoteSupportedFeaturesCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        eventParam.connectionHandle = ((HciReadRemoteSupportedFeaturesParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->readRemoteSupportedFeaturesComplete != NULL) {
        callbacks->readRemoteSupportedFeaturesComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnReadRemoteExtendedFeaturesFailed(uint8_t status, const void *param)
{
    HciReadRemoteExtendedFeaturesCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        eventParam.connectionHandle = ((HciReadRemoteExtendedFeaturesParam *)param)->connectionHandle;
        eventParam.pageNumber = ((HciReadRemoteExtendedFeaturesParam *)param)->pageNumber;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->readRemoteExtendedFeaturesComplete != NULL) {
        callbacks->readRemoteExtendedFeaturesComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnReadRemoteVersionInformationFailed(uint8_t status, const void *param)
{
    HciReadRemoteVersionInformationCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        eventParam.connectionHandle = ((HciReadRemoteVersionInformationParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->readRemoteVersionInformationComplete != NULL) {
        callbacks->readRemoteVersionInformationComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnReadLmpHandleFailed(uint8_t status, const void *param)
{
    HciReadLmpHandleReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle = ((HciReadLmpHandleParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->readLmpHandleComplete != NULL) {
        callbacks->readLmpHandleComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnSetupSynchronousConnectionFailed(uint8_t status, const void *param)
{
    BtAddr addr = {0};
    if (param != NULL) {
        BtmGetAclAddressByHandle(((HciSetupSynchronousConnectionParam *)param)->connectionHandle, &addr);
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciSynchronousConnectionCompleteEventParam eventParam = {
        .status = status,
        .bdAddr = {.raw = {0}},
    };
    (void)memcpy_s(eventParam.bdAddr.raw, BT_ADDRESS_SIZE, addr.addr, BT_ADDRESS_SIZE);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->synchronousConnectionComplete != NULL) {
        callbacks->synchronousConnectionComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnAcceptSynchronousConnectionRequestFailed(uint8_t status, const void *param)
{
    HciSynchronousConnectionCompleteEventParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciAcceptSynchronousConnectionRequestParam *)param)->addr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->synchronousConnectionComplete != NULL) {
        callbacks->synchronousConnectionComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnRejectSynchronousConnectionRequestFailed(uint8_t status, const void *param)
{
    HciSynchronousConnectionCompleteEventParam eventParam = {
        .status = status,
    };

    if (param != NULL) {
        eventParam.bdAddr = ((HciRejectSynchronousConnectionRequestParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->synchronousConnectionComplete != NULL) {
        callbacks->synchronousConnectionComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnIoCapabilityRequestReplyFailed(uint8_t status, const void *param)
{
    HciIOCapabilityRequestReplyReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciIOCapabilityRequestReplyParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->ioCapabilityRequestReplyComplete != NULL) {
        callbacks->ioCapabilityRequestReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnUserConfirmationRequestReplyFailed(uint8_t status, const void *param)
{
    HciUserConfirmationRequestReplyReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciUserConfirmationRequestReplyParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->userConfirmationRequestReplyComplete != NULL) {
        callbacks->userConfirmationRequestReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnUserPasskeyRequestReplyFailed(uint8_t status, const void *param)
{
    HciUserPasskeyRequestReplyReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciUserPasskeyRequestReplyParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->userPasskeyRequestReplyComplete != NULL) {
        callbacks->userPasskeyRequestReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnUserPasskeyRequestNegativeReplyFailed(uint8_t status, const void *param)
{
    HciUserPasskeyRequestNegativeReplyReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciUserPasskeyRequestNegativeReplyParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->userPasskeyRequestNegativeReplyComplete != NULL) {
        callbacks->userPasskeyRequestNegativeReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnRemoteOobDataRequestReplyFailed(uint8_t status, const void *param)
{
    HciRemoteOobDataRequestReplyReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciRemoteOobDataRequestReplyParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->remoteOOBDataRequestReplyComplete != NULL) {
        callbacks->remoteOOBDataRequestReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnRemoteOobDataRequestNegativeReplyFailed(uint8_t status, const void *param)
{
    HciRemoteOobDataRequestNegativeReplyReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciRemoteOobDataRequestNegativeReplyParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->remoteOOBDataRequestNegativeReplyComplete != NULL) {
        callbacks->remoteOOBDataRequestNegativeReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnIoCapabilityRequestNegativeReplyFailed(uint8_t status, const void *param)
{
    HciIoCapabilityRequestNegativeReplyReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciIoCapabilityRequestNegativeReplyParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->iOCapabilityRequestNegativeReplyComplete != NULL) {
        callbacks->iOCapabilityRequestNegativeReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnLogicalLinkCancelFailed(uint8_t status, const void *param)
{
    HciLogicalLinkCancelReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->logicalLinkCancelComplete != NULL) {
        callbacks->logicalLinkCancelComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnUserConfirmationRequestNegativeReplyFailed(uint8_t status, const void *param)
{
    HciUserConfirmationRequestNegativeReplyReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciUserConfirmationRequestNegativeReplyParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->userConfirmationRequestNegativeReplyComplete != NULL) {
        callbacks->userConfirmationRequestNegativeReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnEnhancedSetupSynchronousConnectionFailed(uint8_t status, const void *param)
{
    BtAddr addr = {0};
    if (param != NULL) {
        BtmGetAclAddressByHandle(((HciEnhancedSetupSynchronousConnectionParam *)param)->connectionHandle, &addr);
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciSynchronousConnectionCompleteEventParam eventParam = {
        .status = status,
        .bdAddr = {.raw = {0}},
    };
    (void)memcpy_s(eventParam.bdAddr.raw, BT_ADDRESS_SIZE, addr.addr, BT_ADDRESS_SIZE);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->synchronousConnectionComplete != NULL) {
        callbacks->synchronousConnectionComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnEnhancedAcceptSynchronousConnectionRequestFailed(uint8_t status, const void *param)
{
    HciSynchronousConnectionCompleteEventParam eventParam = {
        .status = status,
    };

    if (param != NULL) {
        eventParam.bdAddr = ((HciEnhancedAcceptSynchronousConnectionRequestParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->synchronousConnectionComplete != NULL) {
        callbacks->synchronousConnectionComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciCmdOnRemoteOobExtendedDataRequestReplyFailed(uint8_t status, const void *param)
{
    HciRemoteOobExtendedDataRequestReplyReturnParam returnParam = {
        .status = status,
    };

    if (param != NULL) {
        returnParam.bdAddr = ((HciRemoteOobExtendedDataRequestReplyParam *)param)->bdAddr;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid address", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->remoteOOBExtendedDataRequestReplyComplete != NULL) {
        callbacks->remoteOOBExtendedDataRequestReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static HciCmdOnFailedFunc g_funcMap[] = {
    NULL,                                                      // 0x0000
    HciCmdOnInquiryFailed,                                     // 0x0001
    HciCmdOnInquiryCancelFailed,                               // 0x0002
    HciCmdOnPeriodicInquiryModeFailed,                         // 0x0003
    HciCmdOnExitPeriodicInquiryModeFailed,                     // 0x0004
    HciCmdOnCreateConnectionFailed,                            // 0x0005
    HciCmdOnDisconnectFailed,                                  // 0x0006
    NULL,                                                      // 0x0007
    HciCmdOnCreateConnectionCancelFailed,                      // 0x0008
    HciCmdOnAcceptConnectionRequestFailed,                     // 0x0009
    HciCmdOnRejectConnectionRequestFailed,                     // 0x000A
    HciCmdOnLinkKeyRequestReplyFailed,                         // 0x000B
    HciCmdOnLinkKeyRequestNegativeReplyFailed,                 // 0x000C
    HciCmdOnPinCodeRequestReplyFailed,                         // 0x000D
    HciCmdOnPinCodeRequestNegativeReplyFailed,                 // 0x000E
    NULL,                                                      // 0x000F
    NULL,                                                      // 0x0010
    HciCmdOnAuthenticationRequestedFailed,                     // 0x0011
    NULL,                                                      // 0x0012
    HciCmdOnSetConnectionEncryptionFailed,                     // 0x0013
    NULL,                                                      // 0x0014
    NULL,                                                      // 0x0015
    NULL,                                                      // 0x0016
    NULL,                                                      // 0x0017
    NULL,                                                      // 0x0018
    HciCmdOnRemoteNameRequestFailed,                           // 0x0019
    HciCmdOnRemoteNameRequestCancelFailed,                     // 0x001A
    HciCmdOnReadRemoteSupportedFeaturesFailed,                 // 0x001B
    HciCmdOnReadRemoteExtendedFeaturesFailed,                  // 0x001C
    HciCmdOnReadRemoteVersionInformationFailed,                // 0x001D
    NULL,                                                      // 0x001E
    NULL,                                                      // 0x001F
    HciCmdOnReadLmpHandleFailed,                               // 0x0020
    NULL,                                                      // 0x0021
    NULL,                                                      // 0x0022
    NULL,                                                      // 0x0023
    NULL,                                                      // 0x0024
    NULL,                                                      // 0x0025
    NULL,                                                      // 0x0026
    NULL,                                                      // 0x0027
    HciCmdOnSetupSynchronousConnectionFailed,                  // 0x0028
    HciCmdOnAcceptSynchronousConnectionRequestFailed,          // 0x0029
    HciCmdOnRejectSynchronousConnectionRequestFailed,          // 0x002A
    HciCmdOnIoCapabilityRequestReplyFailed,                    // 0x002B
    HciCmdOnUserConfirmationRequestReplyFailed,                // 0x002C
    HciCmdOnUserConfirmationRequestNegativeReplyFailed,        // 0x002D
    HciCmdOnUserPasskeyRequestReplyFailed,                     // 0x002E
    HciCmdOnUserPasskeyRequestNegativeReplyFailed,             // 0x002F
    HciCmdOnRemoteOobDataRequestReplyFailed,                   // 0x0030
    NULL,                                                      // 0x0031
    NULL,                                                      // 0x0032
    HciCmdOnRemoteOobDataRequestNegativeReplyFailed,           // 0x0033
    HciCmdOnIoCapabilityRequestNegativeReplyFailed,            // 0x0034
    NULL,                                                      // 0x0035
    NULL,                                                      // 0x0036
    NULL,                                                      // 0x0037
    NULL,                                                      // 0x0038
    NULL,                                                      // 0x0039
    NULL,                                                      // 0x003A
    HciCmdOnLogicalLinkCancelFailed,                           // 0x003B
    NULL,                                                      // 0x003C
    HciCmdOnEnhancedSetupSynchronousConnectionFailed,          // 0x003D
    HciCmdOnEnhancedAcceptSynchronousConnectionRequestFailed,  // 0x003E
    NULL,                                                      // 0x003F
    NULL,                                                      // 0x0040
    NULL,                                                      // 0x0041
    NULL,                                                      // 0x0042
    NULL,                                                      // 0x0043
    NULL,                                                      // 0x0044
    HciCmdOnRemoteOobExtendedDataRequestReplyFailed,           // 0x0045
};

#define LINKCONTROL_OCF_MAX 0x0045

void HciOnLinkControlCmdFailed(uint16_t opCode, uint8_t status, const void *param)
{
    uint16_t ocf = GET_OCF(opCode);
    if (ocf > LINKCONTROL_OCF_MAX) {
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