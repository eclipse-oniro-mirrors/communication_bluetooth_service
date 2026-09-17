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

#include "hci_evt_le_cmd_complete.h"

#include <securec.h>

#include "platform/include/list.h"
#include "platform/include/mutex.h"

#include "hci/hci.h"
#include "hci/hci_error.h"

#include "hci_evt.h"
#include "log.h"

typedef void (*HciLeCmdCompleteFunc)(const void *param, uint8_t length);

static void HciEventOnLeSetEventMaskComplete(const void *param, uint8_t length)
{
    HciLeSetEventMaskReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetEventMaskComplete != NULL) {
        callbacks->leSetEventMaskComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadBufferSizeComplete(const void *param, uint8_t length)
{
    HciLeReadBufferSizeReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadBufferSizeComplete != NULL) {
        callbacks->leReadBufferSizeComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 6, Part B, 4.6.23
// Caches LE Feature Bit 23 (Receiving Constant Tone Extensions). The LE
// Periodic Advertising Report parser consults this to decide whether the
// CTE_Type byte (added in 5.1, Vol 2, Part E, 7.7.65,15) is present on the
// wire: controllers without Bit 23 never send it, and a length-based heuristic
// would misparse 5.0 report payloads.
static bool g_hciLeControllerSupportsCteType = false;

static void HciEventOnLeReadLocalSupportedFeaturesComplete(const void *param, uint8_t length)
{
    HciLeReadLocalSupportedFeaturesReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    g_hciLeControllerSupportsCteType = HciSupportReceivingConstantToneExtensions(returnParam.leFeatures.raw);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadLocalSupportedFeaturesComplete != NULL) {
        callbacks->leReadLocalSupportedFeaturesComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

bool HciLeControllerSupportsCteType(void)
{
    return g_hciLeControllerSupportsCteType;
}

// A short body may not be dropped here: BTM_Enable waits on this completion
// (WAIT_CMD_TIMEOUT, btm_controller.c) to finish controller setup, so dropping it
// stalls the bring-up for the whole timeout and then fails it, while a zeroed 248 B
// feature body reads as "no 6.0 features supported". The body is therefore delivered
// with the octets that did arrive kept. A short body is malformed only when it
// reports success: a Status-only error return is legal and its Status is the
// Controller's own error code, so a non-zero one is preserved (Vol 4, Part E, 4.5,
// p1866). The same Status-preserving short-body rule is used by the CS Command_Complete
// handlers for 0x0089/0x008B/0x008D/0x008F below.
static void HciEventOnLeReadAllLocalSupportedFeaturesComplete(const void *param, uint8_t length)
{
    HciLeReadAllLocalSupportedFeaturesReturnParam returnParam = {0};
    if (param != NULL) {
        (void)memcpy_s(
            &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);
    }
    if (param == NULL || (length < sizeof(returnParam) && returnParam.status == HCI_SUCCESS)) {
        LOG_WARN("%{public}s: body of %{public}u B is short of the 250 B return, reported as a failure",
            __FUNCTION__, (unsigned int)length);
        returnParam.status = HCI_MALFORMED_EVENT;
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadAllLocalSupportedFeaturesComplete != NULL) {
        callbacks->leReadAllLocalSupportedFeaturesComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetDecisionDataComplete(const void *param, uint8_t length)
{
    HciLeSetDecisionDataReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetDecisionDataComplete != NULL) {
        callbacks->leSetDecisionDataComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetDecisionInstructionsComplete(const void *param, uint8_t length)
{
    HciLeSetDecisionInstructionsReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetDecisionInstructionsComplete != NULL) {
        callbacks->leSetDecisionInstructionsComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeAddDeviceToMonitoredAdvertisersListComplete(const void *param, uint8_t length)
{
    HciLeAddDeviceToMonitoredAdvertisersListReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leAddDeviceToMonitoredAdvertisersListComplete != NULL) {
        callbacks->leAddDeviceToMonitoredAdvertisersListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeRemoveDeviceFromMonitoredAdvertisersListComplete(const void *param, uint8_t length)
{
    HciLeRemoveDeviceFromMonitoredAdvertisersListReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoveDeviceFromMonitoredAdvertisersListComplete != NULL) {
        callbacks->leRemoveDeviceFromMonitoredAdvertisersListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeClearMonitoredAdvertisersListComplete(const void *param, uint8_t length)
{
    HciLeClearMonitoredAdvertisersListReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leClearMonitoredAdvertisersListComplete != NULL) {
        callbacks->leClearMonitoredAdvertisersListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadMonitoredAdvertisersListSizeComplete(const void *param, uint8_t length)
{
    HciLeReadMonitoredAdvertisersListSizeReturnParam returnParam = {0};
    if (param != NULL) {
        (void)memcpy_s(
            &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);
    }

    // A Status-only body is a legal error return (Vol 4, Part E, 4.5, p1866); only a short
    // body reporting success contradicts itself, since success carries the whole 2 B return.
    if (param == NULL || (length < sizeof(returnParam) && returnParam.status == HCI_SUCCESS)) {
        LOG_WARN("%{public}s: %{public}u B body cannot carry the 2 B success return, reported as malformed",
            __FUNCTION__, (unsigned int)length);
        returnParam.status = HCI_MALFORMED_EVENT;
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadMonitoredAdvertisersListSizeComplete != NULL) {
        callbacks->leReadMonitoredAdvertisersListSizeComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeEnableMonitoringAdvertisersComplete(const void *param, uint8_t length)
{
    HciLeEnableMonitoringAdvertisersReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leEnableMonitoringAdvertisersComplete != NULL) {
        callbacks->leEnableMonitoringAdvertisersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetRandomAddressComplete(const void *param, uint8_t length)
{
    HciLeSetRandomAddressReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetRandomAddressComplete != NULL) {
        callbacks->leSetRandomAddressComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetAdvertisingParametersComplete(const void *param, uint8_t length)
{
    HciLeSetAdvertisingParametersReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetAdvertisingParametersComplete != NULL) {
        callbacks->leSetAdvertisingParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadAdvertisingChannelTxPowerComplete(const void *param, uint8_t length)
{
    HciLeReadAdvertisingChannelTxPowerReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadAdvertisingChannelTxPowerComplete != NULL) {
        callbacks->leReadAdvertisingChannelTxPowerComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetAdvertisingDataComplete(const void *param, uint8_t length)
{
    HciLeSetAdvertisingDataReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetAdvertisingDataComplete != NULL) {
        callbacks->leSetAdvertisingDataComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetScanResponseDataComplete(const void *param, uint8_t length)
{
    HciLeSetScanResponseDataReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetScanResponseDataComplete != NULL) {
        callbacks->leSetScanResponseDataComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetAdvertisingEnableComplete(const void *param, uint8_t length)
{
    HciLeSetAdvertisingEnableReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetAdvertisingEnableComplete != NULL) {
        callbacks->leSetAdvertisingEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetScanParametersComplete(const void *param, uint8_t length)
{
    HciLeSetScanParametersReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetScanParametersComplete != NULL) {
        callbacks->leSetScanParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetScanEnableComplete(const void *param, uint8_t length)
{
    HciLeSetScanEnableReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetScanEnableComplete != NULL) {
        callbacks->leSetScanEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeCreateConnectionCancelComplete(const void *param, uint8_t length)
{
    HciLeCreateConnectionCancelReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCreateConnectionCancelComplete != NULL) {
        callbacks->leCreateConnectionCancelComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadWhiteListSizeComplete(const void *param, uint8_t length)
{
    HciLeReadWhiteListSizeReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadWhiteListSizeComplete != NULL) {
        callbacks->leReadWhiteListSizeComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeClearWhiteListComplete(const void *param, uint8_t length)
{
    HciLeClearWhiteListReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leClearWhiteListComplete != NULL) {
        callbacks->leClearWhiteListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeAddDeviceToWhiteListComplete(const void *param, uint8_t length)
{
    HciLeAddDeviceToWhiteListReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leAddDeviceToWhiteListComplete != NULL) {
        callbacks->leAddDeviceToWhiteListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeRemoveDeviceFromWhiteListComplete(const void *param, uint8_t length)
{
    HciLeRemoveDeviceFromWhiteListReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoveDeviceFromWhiteListComplete != NULL) {
        callbacks->leRemoveDeviceFromWhiteListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetHostChannelClassificationComplete(const void *param, uint8_t length)
{
    HciLeSetHostChannelClassificationReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetHostChannelClassificationComplete != NULL) {
        callbacks->leSetHostChannelClassificationComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadChannelMapComplete(const void *param, uint8_t length)
{
    HciLeReadChannelMapReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadChannelMapComplete != NULL) {
        callbacks->leReadChannelMapComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeEncryptComplete(const void *param, uint8_t length)
{
    HciLeEncryptReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leEncryptComplete != NULL) {
        callbacks->leEncryptComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeRandComplete(const void *param, uint8_t length)
{
    HciLeRandReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRandComplete != NULL) {
        callbacks->leRandComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeLongTermKeyRequestReplyComplete(const void *param, uint8_t length)
{
    HciLeLongTermKeyRequestReplyReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leLongTermKeyRequestReplyComplete != NULL) {
        callbacks->leLongTermKeyRequestReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeLongTermKeyRequestNegativeReplyComplete(const void *param, uint8_t length)
{
    HciLeLongTermKeyRequestNegativeReplyReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leLongTermKeyRequestNegativeReplyComplete != NULL) {
        callbacks->leLongTermKeyRequestNegativeReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeRemoteConnectionParameterRequestReplyComplete(const void *param, uint8_t length)
{
    HciLeRemoteConnectionParameterRequestReplyReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoteConnectionParameterRequestReplyComplete != NULL) {
        callbacks->leRemoteConnectionParameterRequestReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeRemoteConnectionParameterRequestNegativeReplyComplete(const void *param, uint8_t length)
{
    HciLeRemoteConnectionParameterRequestNegativeReplyReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoteConnectionParameterRequestNegativeReplyComplete != NULL) {
        callbacks->leRemoteConnectionParameterRequestNegativeReplyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeAddDeviceToResolvingListComplete(const void *param, uint8_t length)
{
    HciLeAddDeviceToResolvingListReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leAddDeviceToResolvingListComplete != NULL) {
        callbacks->leAddDeviceToResolvingListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeRemoveDeviceFromResolvingListComplete(const void *param, uint8_t length)
{
    HciLeRemoveDeviceFromResolvingListReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoveDeviceFromResolvingListComplete != NULL) {
        callbacks->leRemoveDeviceFromResolvingListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeClearResolvingListComplete(const void *param, uint8_t length)
{
    HciLeClearResolvingListReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leClearResolvingListComplete != NULL) {
        callbacks->leClearResolvingListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadResolvingListSizeComplete(const void *param, uint8_t length)
{
    HciLeReadResolvingListSizeReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadResolvingListSizeComplete != NULL) {
        callbacks->leReadResolvingListSizeComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadPeerResolvableAddressComplete(const void *param, uint8_t length)
{
    HciLeReadPeerResolvableAddressReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadPeerResolvableAddressComplete != NULL) {
        callbacks->leReadPeerResolvableAddressComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadLocalResolvableAddressComplete(const void *param, uint8_t length)
{
    HciLeReadLocalResolvableAddressReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadLocalResolvableAddressComplete != NULL) {
        callbacks->leReadLocalResolvableAddressComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetAddressResolutionEnableComplete(const void *param, uint8_t length)
{
    HciLeSetAddressResolutionEnableReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetAddressResolutionEnableComplete != NULL) {
        callbacks->leSetAddressResolutionEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnSetResolvablePrivateAddressTimeoutComplete(const void *param, uint8_t length)
{
    HciLeSetResolvablePrivateAddressTimeoutReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetResolvablePrivateAddressTimeoutComplete != NULL) {
        callbacks->leSetResolvablePrivateAddressTimeoutComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetAdvertisingSetRandomAddressComplete(const void *param, uint8_t length)
{
    HciLeSetAdvertisingSetRandomAddressReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetAdvertisingSetRandomAddressComplete != NULL) {
        callbacks->leSetAdvertisingSetRandomAddressComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetExtendedAdvertisingParametersComplete(const void *param, uint8_t length)
{
    HciLeSetExtendedAdvertisingParametersReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetExtendedAdvertisingParametersComplete != NULL) {
        callbacks->leSetExtendedAdvertisingParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.53 LE Set Extended Advertising Parameters Command [v2] Complete
// Return parameters are unchanged from [v1], so the v1 return struct is reused.
static void HciEventOnLeSetExtendedAdvertisingParametersV2Complete(const void *param, uint8_t length)
{
    HciLeSetExtendedAdvertisingParametersReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetExtendedAdvertisingParametersV2Complete != NULL) {
        callbacks->leSetExtendedAdvertisingParametersV2Complete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetExtendedAdvertisingDataComplete(const void *param, uint8_t length)
{
    HciLeSetExtendedAdvertisingDataReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetExtendedAdvertisingDataComplete != NULL) {
        callbacks->leSetExtendedAdvertisingDataComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetExtendedScanResponseDataComplete(const void *param, uint8_t length)
{
    HciLeSetExtendedScanResponseDataReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetExtendedScanResponseDataComplete != NULL) {
        callbacks->leSetExtendedScanResponseDataComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetExtendedAdvertisingEnableComplete(const void *param, uint8_t length)
{
    HciLeSetExtendedAdvertisingEnableReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetExtendedAdvertisingEnableComplete != NULL) {
        callbacks->leSetExtendedAdvertisingEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadMaximumAdvertisingDataLengthComplete(const void *param, uint8_t length)
{
    HciLeReadMaximumAdvertisingDataLengthReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadMaximumAdvertisingDataLengthComplete != NULL) {
        callbacks->leReadMaximumAdvertisingDataLengthComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadNumberofSupportedAdvertisingSetsComplete(const void *param, uint8_t length)
{
    HciLeReadNumberofSupportedAdvertisingSetsReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadNumberofSupportedAdvertisingSetsComplete != NULL) {
        callbacks->leReadNumberofSupportedAdvertisingSetsComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeRemoveAdvertisingSetComplete(const void *param, uint8_t length)
{
    HciLeRemoveAdvertisingSetReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoveAdvertisingSetComplete != NULL) {
        callbacks->leRemoveAdvertisingSetComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeClearAdvertisingSetsComplete(const void *param, uint8_t length)
{
    HciLeClearAdvertisingSetsReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leClearAdvertisingSetsComplete != NULL) {
        callbacks->leClearAdvertisingSetsComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetExtendedScanParametersComplete(const void *param, uint8_t length)
{
    HciLeSetExtendedScanParametersReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetExtendedScanParametersComplete != NULL) {
        callbacks->leSetExtendedScanParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetExtendedScanEnableComplete(const void *param, uint8_t length)
{
    HciLeSetExtendedScanEnableReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetExtendedScanEnableComplete != NULL) {
        callbacks->leSetExtendedScanEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadSupportedStatesComplete(const void *param, uint8_t length)
{
    HciLeReadSupportedStatesReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadSupportedStatesComplete != NULL) {
        callbacks->leReadSupportedStatesComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReceiverTestComplete(const void *param, uint8_t length)
{
    HciLeReceiverTestReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReceiverTestComplete != NULL) {
        callbacks->leReceiverTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeTransmitterTestComplete(const void *param, uint8_t length)
{
    HciLeTransmitterTestReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leTransmitterTestComplete != NULL) {
        callbacks->leTransmitterTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeTestEndComplete(const void *param, uint8_t length)
{
    HciLeTestEndReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leTestEndComplete != NULL) {
        callbacks->leTestEndComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetDataLengthComplete(const void *param, uint8_t length)
{
    HciLeSetDataLengthReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetDataLengthComplete != NULL) {
        callbacks->leSetDataLengthComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadSuggestedDefaultDataLengthComplete(const void *param, uint8_t length)
{
    HciLeReadSuggestedDefaultDataLengthReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadSuggestedDefaultDataLengthComplete != NULL) {
        callbacks->leReadSuggestedDefaultDataLengthComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeWriteSuggestedDefaultDataLengthComplete(const void *param, uint8_t length)
{
    HciLeWriteSuggestedDefaultDataLengthReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leWriteSuggestedDefaultDataLengthComplete != NULL) {
        callbacks->leWriteSuggestedDefaultDataLengthComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadMaximumDataLengthComplete(const void *param, uint8_t length)
{
    HciLeReadMaximumDataLengthReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadMaximumDataLengthComplete != NULL) {
        callbacks->leReadMaximumDataLengthComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadPhyComplete(const void *param, uint8_t length)
{
    HciLeReadPhyReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadPhyComplete != NULL) {
        callbacks->leReadPhyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetDefaultPhyComplete(const void *param, uint8_t length)
{
    HciLeSetDefaultPhyReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetDefaultPhyComplete != NULL) {
        callbacks->leSetDefaultPhyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetPhyComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeSetPhyReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPhyComplete != NULL) {
        callbacks->leSetPhyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeEnhancedReceiverTestComplete(const void *param, uint8_t length)
{
    HciLeEnhancedReceiverTestReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leEnhancedReceiverTestComplete != NULL) {
        callbacks->leEnhancedReceiverTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeEnhancedTransmitterTestComplete(const void *param, uint8_t length)
{
    HciLeEnhancedTransmitterTestReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leEnhancedTransmitterTestComplete != NULL) {
        callbacks->leEnhancedTransmitterTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetPeriodicAdvertisingParametersComplete(const void *param, uint8_t length)
{
    HciLeSetPeriodicAdvertisingParametersReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPeriodicAdvertisingParametersComplete != NULL) {
        callbacks->leSetPeriodicAdvertisingParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetPeriodicAdvertisingDataComplete(const void *param, uint8_t length)
{
    HciLeSetPeriodicAdvertisingDataReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPeriodicAdvertisingDataComplete != NULL) {
        callbacks->leSetPeriodicAdvertisingDataComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetPeriodicAdvertisingEnableComplete(const void *param, uint8_t length)
{
    HciLeSetPeriodicAdvertisingEnableReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPeriodicAdvertisingEnableComplete != NULL) {
        callbacks->leSetPeriodicAdvertisingEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLePeriodicAdvertisingCreateSyncCancelComplete(const void *param, uint8_t length)
{
    HciLePeriodicAdvertisingCreateSyncCancelReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->lePeriodicAdvertisingCreateSyncCancelComplete != NULL) {
        callbacks->lePeriodicAdvertisingCreateSyncCancelComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLePeriodicAdvertisingTerminateSyncComplete(const void *param, uint8_t length)
{
    HciLePeriodicAdvertisingTerminateSyncReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->lePeriodicAdvertisingTerminateSyncComplete != NULL) {
        callbacks->lePeriodicAdvertisingTerminateSyncComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeAddDeviceToPeriodicAdvertiserListComplete(const void *param, uint8_t length)
{
    HciLeAddDeviceToPeriodicAdvertiserListReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leAddDeviceToPeriodicAdvertiserListComplete != NULL) {
        callbacks->leAddDeviceToPeriodicAdvertiserListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeRemoveDeviceFromPeriodicAdvertiserListComplete(const void *param, uint8_t length)
{
    HciLeRemoveDeviceFromPeriodicAdvertiserListReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoveDeviceFromPeriodicAdvertiserListComplete != NULL) {
        callbacks->leRemoveDeviceFromPeriodicAdvertiserListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeClearPeriodicAdvertiserListComplete(const void *param, uint8_t length)
{
    HciLeClearPeriodicAdvertiserListReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leClearPeriodicAdvertiserListComplete != NULL) {
        callbacks->leClearPeriodicAdvertiserListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadPeriodicAdvertiserListSizeComplete(const void *param, uint8_t length)
{
    HciLeReadPeriodicAdvertiserListSizeReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadPeriodicAdvertiserListSizeComplete != NULL) {
        callbacks->leReadPeriodicAdvertiserListSizeComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadTransmitPowerComplete(const void *param, uint8_t length)
{
    HciLeReadTransmitPowerReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadTransmitPowerComplete != NULL) {
        callbacks->leReadTransmitPowerComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadRFPathCompensationComplete(const void *param, uint8_t length)
{
    HciLeReadRfPathCompensationReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadRfPathCompensationComplete != NULL) {
        callbacks->leReadRfPathCompensationComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeWriteRFPathCompensationComplete(const void *param, uint8_t length)
{
    HciLeWriteRfPathCompensationReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leWriteRfPathCompensationComplete != NULL) {
        callbacks->leWriteRfPathCompensationComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetPrivacyModeComplete(const void *param, uint8_t length)
{
    HciLeSetPrivacyModeReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPrivacyModeComplete != NULL) {
        callbacks->leSetPrivacyModeComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReceiverTestV3Complete(const void *param, uint8_t length)
{
    HciLeReceiverTestV3ReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReceiverTestV3Complete != NULL) {
        callbacks->leReceiverTestV3Complete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeTransmitterTestV3Complete(const void *param, uint8_t length)
{
    HciLeTransmitterTestV3ReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leTransmitterTestV3Complete != NULL) {
        callbacks->leTransmitterTestV3Complete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetConnectionlessCteTransmitParametersComplete(const void *param, uint8_t length)
{
    HciLeSetConnectionlessCteTransmitParametersReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetConnectionlessCteTransmitParametersComplete != NULL) {
        callbacks->leSetConnectionlessCteTransmitParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetConnectionlessCteTransmitEnableComplete(const void *param, uint8_t length)
{
    HciLeSetConnectionlessCteTransmitEnableReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetConnectionlessCteTransmitEnableComplete != NULL) {
        callbacks->leSetConnectionlessCteTransmitEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetConnectionlessIqSamplingEnableComplete(const void *param, uint8_t length)
{
    HciLeSetConnectionlessIqSamplingEnableReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetConnectionlessIqSamplingEnableComplete != NULL) {
        callbacks->leSetConnectionlessIqSamplingEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetConnectionCteReceiveParametersComplete(const void *param, uint8_t length)
{
    HciLeSetConnectionCteReceiveParametersReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetConnectionCteReceiveParametersComplete != NULL) {
        callbacks->leSetConnectionCteReceiveParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetConnectionCteTransmitParametersComplete(const void *param, uint8_t length)
{
    HciLeSetConnectionCteTransmitParametersReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetConnectionCteTransmitParametersComplete != NULL) {
        callbacks->leSetConnectionCteTransmitParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeConnectionCteRequestEnableComplete(const void *param, uint8_t length)
{
    HciLeConnectionCteRequestEnableReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leConnectionCteRequestEnableComplete != NULL) {
        callbacks->leConnectionCteRequestEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeConnectionCteResponseEnableComplete(const void *param, uint8_t length)
{
    HciLeConnectionCteResponseEnableReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leConnectionCteResponseEnableComplete != NULL) {
        callbacks->leConnectionCteResponseEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadAntennaInformationComplete(const void *param, uint8_t length)
{
    HciLeReadAntennaInformationReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadAntennaInformationComplete != NULL) {
        callbacks->leReadAntennaInformationComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetPeriodicAdvertisingReceiveEnableComplete(const void *param, uint8_t length)
{
    HciLeSetPeriodicAdvertisingReceiveEnableReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPeriodicAdvertisingReceiveEnableComplete != NULL) {
        callbacks->leSetPeriodicAdvertisingReceiveEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLePeriodicAdvertisingSyncTransferComplete(const void *param, uint8_t length)
{
    HciLePeriodicAdvertisingSyncTransferReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->lePeriodicAdvertisingSyncTransferComplete != NULL) {
        callbacks->lePeriodicAdvertisingSyncTransferComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLePeriodicAdvertisingSetInfoTransferComplete(const void *param, uint8_t length)
{
    HciLePeriodicAdvertisingSetInfoTransferReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->lePeriodicAdvertisingSetInfoTransferComplete != NULL) {
        callbacks->lePeriodicAdvertisingSetInfoTransferComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetPeriodicAdvertisingSyncTransferParametersComplete(const void *param, uint8_t length)
{
    HciLeSetPeriodicAdvertisingSyncTransferParametersReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPeriodicAdvertisingSyncTransferParametersComplete != NULL) {
        callbacks->leSetPeriodicAdvertisingSyncTransferParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetDefaultPeriodicAdvertisingSyncTransferParametersComplete(const void *param, uint8_t length)
{
    HciLeSetDefaultPeriodicAdvertisingSyncTransferParametersReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetDefaultPeriodicAdvertisingSyncTransferParametersComplete != NULL) {
        callbacks->leSetDefaultPeriodicAdvertisingSyncTransferParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeModifySleepClockAccuracyComplete(const void *param, uint8_t length)
{
    HciLeModifySleepClockAccuracyReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leModifySleepClockAccuracyComplete != NULL) {
        callbacks->leModifySleepClockAccuracyComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadIsoTxSyncComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeReadIsoTxSyncReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadIsoTxSyncComplete != NULL) {
        callbacks->leReadIsoTxSyncComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadBufferSizeV2Complete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeReadBufferSizeV2ReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadBufferSizeV2Complete != NULL) {
        callbacks->leReadBufferSizeV2Complete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetCigParametersComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeSetCigParametersReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetCigParametersComplete != NULL) {
        callbacks->leSetCigParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetCigParametersTestComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeSetCigParametersTestReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetCigParametersTestComplete != NULL) {
        callbacks->leSetCigParametersTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeRemoveCigComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeRemoveCigReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoveCigComplete != NULL) {
        callbacks->leRemoveCigComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeRejectCisRequestComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeRejectCisRequestReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRejectCisRequestComplete != NULL) {
        callbacks->leRejectCisRequestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeBigTerminateSyncComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeBigTerminateSyncReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leBigTerminateSyncComplete != NULL) {
        callbacks->leBigTerminateSyncComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetupIsoDataPathComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeSetupIsoDataPathReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetupIsoDataPathComplete != NULL) {
        callbacks->leSetupIsoDataPathComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeRemoveIsoDataPathComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeRemoveIsoDataPathReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoveIsoDataPathComplete != NULL) {
        callbacks->leRemoveIsoDataPathComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeIsoTransmitTestComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeIsoTransmitTestReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leIsoTransmitTestComplete != NULL) {
        callbacks->leIsoTransmitTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeIsoReceiveTestComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeIsoReceiveTestReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leIsoReceiveTestComplete != NULL) {
        callbacks->leIsoReceiveTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeIsoReadTestCountersComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeIsoReadTestCountersReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leIsoReadTestCountersComplete != NULL) {
        callbacks->leIsoReadTestCountersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeIsoTestEndComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeIsoTestEndReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leIsoTestEndComplete != NULL) {
        callbacks->leIsoTestEndComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetHostFeatureComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeSetHostFeatureReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetHostFeatureComplete != NULL) {
        callbacks->leSetHostFeatureComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeReadIsoLinkQualityComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeReadIsoLinkQualityReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadIsoLinkQualityComplete != NULL) {
        callbacks->leReadIsoLinkQualityComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x006D normally completes via Command_Status + the LE Request Peer SCA Complete
// event (7.7.65,31), not Command_Complete. A Controller that does not implement the
// command replies Command_Complete + error (Vol 1 Part F, Section 2.1); deliver that
// error through the same complete path so callers do not wait forever on the event.
static void HciEventOnLeRequestPeerScaCommandComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeRequestPeerScaCompleteEventParam eventParam = { 0 };
    (void)memcpy_s(&eventParam, sizeof(eventParam), param, (length > sizeof(eventParam)) ? sizeof(eventParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRequestPeerScaComplete != NULL) {
        callbacks->leRequestPeerScaComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeEnhancedReadTransmitPowerLevelComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeEnhancedReadTransmitPowerLevelReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leEnhancedReadTransmitPowerLevelComplete != NULL) {
        callbacks->leEnhancedReadTransmitPowerLevelComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x0077 normally completes via Command_Status + the LE Transmit Power Reporting
// event (7.7.65,33) with Reason 0x02, not Command_Complete. A Controller that does
// not implement the command replies Command_Complete + error (Vol 1 Part F, Section 2.1);
// deliver that error through the same event path so callers do not wait forever.
static void HciEventOnLeReadRemoteTransmitPowerLevelCommandComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    const uint8_t *payload = (const uint8_t *)param;
    HciLeTransmitPowerReportingEventParam eventParam = { 0 };
    // command complete 7.8.77: status(1) + connection handle(2) + phy(1) + power level(1),
    // the offsets below are derived from the wire layout, Vol 2 Part E 7.8.77
    const size_t handleOffset = sizeof(eventParam.status);
    const size_t phyOffset = handleOffset + sizeof(eventParam.connectionHandle);
    const size_t phyEnd = phyOffset + sizeof(eventParam.phy);
    const size_t powerEnd = phyEnd + sizeof(eventParam.transmitPowerLevel);
    eventParam.reason = 0x02;
    // payload is non-NULL (checked above); each field is parsed independently
    // against its own end boundary, so a truncated return parameter keeps the
    // zero-initialized default of the missing tail fields. Note the power level
    // sits at phyEnd and is guarded by powerEnd (phyEnd + 1).
    if (length >= sizeof(eventParam.status)) {
        eventParam.status = payload[0];
    }
    if (length >= phyOffset) {
        (void)memcpy_s(&eventParam.connectionHandle, sizeof(eventParam.connectionHandle), payload + handleOffset,
            sizeof(eventParam.connectionHandle));
    }
    if (length >= phyEnd) {
        eventParam.phy = payload[phyOffset];
    }
    if (length >= powerEnd) {
        eventParam.transmitPowerLevel = (int8_t)payload[phyEnd];
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leTransmitPowerReporting != NULL) {
        callbacks->leTransmitPowerReporting(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetPathLossReportingParametersComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeSetPathLossReportingParametersReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPathLossReportingParametersComplete != NULL) {
        callbacks->leSetPathLossReportingParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetPathLossReportingEnableComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeSetPathLossReportingEnableReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPathLossReportingEnableComplete != NULL) {
        callbacks->leSetPathLossReportingEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static void HciEventOnLeSetTransmitPowerReportingEnableComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeSetTransmitPowerReportingEnableReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetTransmitPowerReportingEnableComplete != NULL) {
        callbacks->leSetTransmitPowerReportingEnableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// BLUETOOTH SPECIFICATION Version 5.3 | Vol 4, Part E
// 7.8.123 LE Set Default Subrate Command Complete (status-only payload)
static void HciEventOnLeSetDefaultSubrateComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeSetDefaultSubrateReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetDefaultSubrateComplete != NULL) {
        callbacks->leSetDefaultSubrateComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// BLUETOOTH SPECIFICATION Version 5.3 | Vol 4, Part E
// 7.8.124 LE Subrate Request Command Complete (status-only payload)
static void HciEventOnLeSubrateRequestComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeSubrateRequestReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSubrateRequestComplete != NULL) {
        callbacks->leSubrateRequestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// BLUETOOTH SPECIFICATION Version 5.3 | Vol 4, Part E
// 7.8.122 LE Set Data Related Address Changes Command Complete (status-only
// payload)
static void HciEventOnLeSetDataRelatedAddressChangesComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    HciLeSetDataRelatedAddressChangesReturnParam returnParam = { 0 };
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetDataRelatedAddressChangesComplete != NULL) {
        callbacks->leSetDataRelatedAddressChangesComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x0064 LE Create CIS normally completes via Command_Status + the LE CIS Established
// event (7.7.65,25), not Command_Complete. A Controller that does not implement the
// command replies Command_Complete + error (Vol 1 Part F, Section 2.1); deliver that
// error through the leCreateCisComplete path so callers do not wait forever on the
// 0x19. The 0x19-style event is NOT fanned out here: a Command_Complete payload only
// carries the status byte, so a leCisEstablished event synthesized from it would be a
// half-initialized fake with connectionHandle 0 that no subscriber can correlate; the
// complete path already carries the status. A Command_Complete + error is strictly
// the reply to a locally issued LE Create CIS, so this cannot fire for
// remote-initiated CIS.
static void HciEventOnLeCreateCisCommandComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    const uint8_t *payload = (const uint8_t *)param;
    HciEventCallbacks *callbacks = NULL;

    HciLeCreateCisReturnParam returnParam = {
        .status = (length >= sizeof(returnParam.status)) ? payload[0] : 0,
    };
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCreateCisComplete != NULL) {
        callbacks->leCreateCisComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x0066 LE Accept CIS Request: same asynchronous shape as 0x0064 - the outcome is the
// LE CIS Established event (7.7.65,25), so an unsupported-command Command_Complete +
// error carries only the status byte and no connection handle. Synthesizing a
// leCisEstablished event from it would deliver a half-initialized fake with
// connectionHandle 0 (see HciEventOnLeCreateCisCommandComplete), and unlike 0x0064
// there is no complete-type callback that could carry the failure instead, so nothing
// is fanned out here: the failed accept is observable by the remote side, which
// receives the error response to its CIS request.
static void HciEventOnLeAcceptCisRequestCommandComplete(const void *param, uint8_t length)
{
    (void)param;
    (void)length;
}

// 0x0068 LE Create BIG / 0x0069 LE Create BIG Test normally complete via Command_Status +
// the LE Create BIG Complete event (7.7.65,27), not Command_Complete; the unsupported-
// command Command_Complete + error is delivered through the same event path.
static void HciEventOnLeCreateBigCommandComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    const uint8_t *payload = (const uint8_t *)param;
    HciLeCreateBigCompleteEventParam eventParam = { 0 };
    HciEventCallbacks *callbacks = NULL;

    if (payload != NULL && length >= sizeof(eventParam.status)) {
        eventParam.status = payload[0];
    }

    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCreateBigComplete != NULL) {
        callbacks->leCreateBigComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x006A LE Terminate BIG normally completes via Command_Status + the LE Terminate BIG
// Complete event (7.7.65,28), not Command_Complete; the unsupported-command
// Command_Complete + error is delivered through the same event path.
static void HciEventOnLeTerminateBigCommandComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    const uint8_t *payload = (const uint8_t *)param;
    HciLeTerminateBigCompleteEventParam eventParam = { 0 };
    HciEventCallbacks *callbacks = NULL;

    if (payload != NULL && length >= sizeof(eventParam.status)) {
        eventParam.status = payload[0];
    }

    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leTerminateBigComplete != NULL) {
        callbacks->leTerminateBigComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x006B LE Big Create Sync normally completes via Command_Status + the LE BIG Sync
// Established event (7.7.65,29), not Command_Complete; the unsupported-command
// Command_Complete + error is delivered through the same event path.
static void HciEventOnLeBigCreateSyncCommandComplete(const void *param, uint8_t length)
{
    if (param == NULL) {
        return;
    }
    const uint8_t *payload = (const uint8_t *)param;
    HciLeBigSyncEstablishedEventParam eventParam = { 0 };
    HciEventCallbacks *callbacks = NULL;

    if (payload != NULL && length >= sizeof(eventParam.status)) {
        eventParam.status = payload[0];
    }

    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leBigSyncEstablished != NULL) {
        callbacks->leBigSyncEstablished(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.125 LE Set Periodic Advertising Subevent Data Command Complete
// Return parameters: Status + Advertising_Handle (echo).
static void HciEventOnLeSetPeriodicAdvertisingSubeventDataComplete(const void *param, uint8_t length)
{
    HciLeSetPeriodicAdvertisingSubeventDataReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPeriodicAdvertisingSubeventDataComplete != NULL) {
        callbacks->leSetPeriodicAdvertisingSubeventDataComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.126 LE Set Periodic Advertising Response Data Command Complete
// Return parameters: Status + Sync_Handle (echo).
static void HciEventOnLeSetPeriodicAdvertisingResponseDataComplete(const void *param, uint8_t length)
{
    HciLeSetPeriodicAdvertisingResponseDataReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPeriodicAdvertisingResponseDataComplete != NULL) {
        callbacks->leSetPeriodicAdvertisingResponseDataComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.127 LE Set Periodic Sync Subevent Command Complete
// Return parameters: Status + Sync_Handle (echo).
static void HciEventOnLeSetPeriodicSyncSubeventComplete(const void *param, uint8_t length)
{
    HciLeSetPeriodicSyncSubeventReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPeriodicSyncSubeventComplete != NULL) {
        callbacks->leSetPeriodicSyncSubeventComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.61 LE Set Periodic Advertising Parameters Command [v2] Complete
// The [v2] return parameters add Advertising_Handle (echo) to the [v1]
// Status-only pair, hence the dedicated struct and callback.
static void HciEventOnLeSetPeriodicAdvertisingParametersV2Complete(const void *param, uint8_t length)
{
    HciLeSetPeriodicAdvertisingParametersV2ReturnParam returnParam = {0};
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetPeriodicAdvertisingParametersV2Complete != NULL) {
        callbacks->leSetPeriodicAdvertisingParametersV2Complete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x0089 completes with Command_Complete; the 29 B return (Status + 28 B
// capability body) is broadcast to the CS engine, which caches it module-wide
// (Vol 4, Part E, 7.8.130, p2798).
// Same short-body rule as 0x0087: a Status-only error return is legal to deliver
// (Vol 4, Part E, 4.5, p1866); only a short body that still claims success is
// rewritten as HCI_MALFORMED_EVENT, so a zeroed capability body is never cached.
static void HciEventOnLeCsReadLocalSupportedCapabilitiesComplete(const void *param, uint8_t length)
{
    HciLeCsReadLocalSupportedCapabilitiesReturnParam returnParam = {0};
    if (param != NULL) {
        (void)memcpy_s(
            &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);
    }
    if (param == NULL || (length < sizeof(returnParam) && returnParam.status == HCI_SUCCESS)) {
        LOG_WARN("%{public}s: body of %{public}u B is short of the 29 B return, reported as a failure",
            __FUNCTION__, (unsigned int)length);
        returnParam.status = HCI_MALFORMED_EVENT;
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsReadLocalSupportedCapabilitiesComplete != NULL) {
        callbacks->leCsReadLocalSupportedCapabilitiesComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x008B completes with Command_Complete; the 3 B return (Status +
// Connection_Handle) closes the write-back request. Only the CS engine consumes
// it (Vol 4, Part E, 7.8.132).
// Short-body rule matches 0x0087 for Status; a truncated return cannot name a
// handle reliably (0x0000 is valid), so Connection_Handle is LE_CS_INVALID_HANDLE.
static void HciEventOnLeCsWriteCachedRemoteSupportedCapabilitiesComplete(const void *param, uint8_t length)
{
    HciLeCsWriteCachedRemoteSupportedCapabilitiesReturnParam returnParam = {0};
    if (param != NULL) {
        (void)memcpy_s(
            &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);
    }
    if (param == NULL || (length < sizeof(returnParam) && returnParam.status == HCI_SUCCESS)) {
        LOG_WARN("%{public}s: body of %{public}u B is short of the 3 B return, reported as a failure",
            __FUNCTION__, (unsigned int)length);
        returnParam.status = HCI_MALFORMED_EVENT;
    }
    if (param == NULL || length < sizeof(returnParam)) {
        returnParam.connectionHandle = LE_CS_INVALID_HANDLE;
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsWriteCachedRemoteSupportedCapabilitiesComplete != NULL) {
        callbacks->leCsWriteCachedRemoteSupportedCapabilitiesComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x008D completes with Command_Complete; the 3 B return (Status +
// Connection_Handle) reports only whether the Controller accepted the defaults.
// The engine mirrors nothing from it - the defaults live in the Controller, are
// connection-scoped, and no Host-side decision reads them (Vol 4, Part E, 7.8.134).
// Same short-body Status / INVALID_HANDLE rule as 0x008B.
static void HciEventOnLeCsSetDefaultSettingsComplete(const void *param, uint8_t length)
{
    HciLeCsSetDefaultSettingsReturnParam returnParam = {0};
    if (param != NULL) {
        (void)memcpy_s(
            &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);
    }
    if (param == NULL || (length < sizeof(returnParam) && returnParam.status == HCI_SUCCESS)) {
        LOG_WARN("%{public}s: body of %{public}u B is short of the 3 B return, reported as a failure",
            __FUNCTION__, (unsigned int)length);
        returnParam.status = HCI_MALFORMED_EVENT;
    }
    if (param == NULL || length < sizeof(returnParam)) {
        returnParam.connectionHandle = LE_CS_INVALID_HANDLE;
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsSetDefaultSettingsComplete != NULL) {
        callbacks->leCsSetDefaultSettingsComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x008F completes with Command_Complete; the 3 B return (Status +
// Connection_Handle) closes the write-back request. Only the CS engine consumes it
// (Vol 4, Part E, 7.8.136, pp2820-2821).
// Same short-body Status / INVALID_HANDLE rule as 0x008B.
static void HciEventOnLeCsWriteCachedRemoteFaeTableComplete(const void *param, uint8_t length)
{
    HciLeCsWriteCachedRemoteFaeTableReturnParam returnParam = {0};
    if (param != NULL) {
        (void)memcpy_s(
            &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);
    }
    if (param == NULL || (length < sizeof(returnParam) && returnParam.status == HCI_SUCCESS)) {
        LOG_WARN("%{public}s: body of %{public}u B is short of the 3 B return, reported as a failure",
            __FUNCTION__, (unsigned int)length);
        returnParam.status = HCI_MALFORMED_EVENT;
    }
    if (param == NULL || length < sizeof(returnParam)) {
        returnParam.connectionHandle = LE_CS_INVALID_HANDLE;
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsWriteCachedRemoteFaeTableComplete != NULL) {
        callbacks->leCsWriteCachedRemoteFaeTableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x0092 completes with Command_Complete and a 1 B return holding only Status: the
// classification is Controller-wide, so no Connection_Handle identifies the
// affected link. 0x0C reports a re-issue inside the 1-second window and 0x12 an
// invalid classification (Vol 4, Part E, 7.8.139, pp2830-2831).
static void HciEventOnLeCsSetChannelClassificationComplete(const void *param, uint8_t length)
{
    HciLeCsSetChannelClassificationReturnParam returnParam = {0};
    // The one shorter frame (empty) is turned away before dispatch, so this guard only
    // keeps the CS handlers uniform.
    if (param == NULL || length < sizeof(returnParam)) {
        return;
    }
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsSetChannelClassificationComplete != NULL) {
        callbacks->leCsSetChannelClassificationComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x0093 Command_Complete returns Status + Connection_Handle only (Vol 4, Part E,
// 7.8.140); Config_ID is tracked by the engine, not echoed by the Controller.
// DESIGN NOTE: a truncated return carries no readable handle, but dropping it would
// leave the engine's in-flight 0x0093 record wedged, blocking later requests on that
// handle with BT_ALREADY. The frame is therefore passed on as LE_CS_INVALID_HANDLE
// so the engine can conservatively clear every pending record. This is intentional.
static void HciEventOnLeCsSetProcedureParametersComplete(const void *param, uint8_t length)
{
    HciLeCsSetProcedureParametersReturnParam returnParam = {0};
    // Pass a no-handle completion to the engine so the in-flight record is cleared.
    if (param == NULL || length < sizeof(returnParam)) {
        HciLeCsSetProcedureParametersReturnParam unattributable = {
            .status = HCI_MALFORMED_EVENT,
            .connectionHandle = LE_CS_INVALID_HANDLE,
        };

        HciEventCallbacks *callbacks = NULL;
        HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
        if (callbacks->leCsSetProcedureParametersComplete != NULL) {
            callbacks->leCsSetProcedureParametersComplete(&unattributable);
        }
        HCI_FOREACH_EVT_CALLBACKS_END;
        return;
    }
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsSetProcedureParametersComplete != NULL) {
        callbacks->leCsSetProcedureParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x0095 completes with Command_Complete and a Status-only return (Vol 4, Part E,
// 7.8.142, p2840). A CS test reports through the 0x31/0x32 stream under the reserved
// Connection_Handle 0x0FFF, so this completion says one thing only: whether the test was
// started. Like the 0x0093 above, a truncated return is not dropped but passed on as a
// malformed completion, because the engine holds a pending record for the outstanding
// 0x0095 that only this completion clears.
static void HciEventOnLeCsTestComplete(const void *param, uint8_t length)
{
    HciLeCsTestReturnParam returnParam = {0};
    if (param == NULL || length < sizeof(returnParam)) {
        HciLeCsTestReturnParam unattributable = {
            .status = HCI_MALFORMED_EVENT,
        };

        HciEventCallbacks *callbacks = NULL;
        HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
        if (callbacks->leCsTestComplete != NULL) {
            callbacks->leCsTestComplete(&unattributable);
        }
        HCI_FOREACH_EVT_CALLBACKS_END;
        return;
    }
    (void)memcpy_s(
        &returnParam, sizeof(returnParam), param, (length > sizeof(returnParam)) ? sizeof(returnParam) : length);

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsTestComplete != NULL) {
        callbacks->leCsTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

static HciLeCmdCompleteFunc g_leControllerCommandCompleteMap[] = {
    NULL,                                                                    // 0x0000
    HciEventOnLeSetEventMaskComplete,                                        // 0x0001
    HciEventOnLeReadBufferSizeComplete,                                      // 0x0002
    HciEventOnLeReadLocalSupportedFeaturesComplete,                          // 0x0003
    NULL,                                                                    // 0x0004
    HciEventOnLeSetRandomAddressComplete,                                    // 0x0005
    HciEventOnLeSetAdvertisingParametersComplete,                            // 0x0006
    HciEventOnLeReadAdvertisingChannelTxPowerComplete,                       // 0x0007
    HciEventOnLeSetAdvertisingDataComplete,                                  // 0x0008
    HciEventOnLeSetScanResponseDataComplete,                                 // 0x0009
    HciEventOnLeSetAdvertisingEnableComplete,                                // 0x000A
    HciEventOnLeSetScanParametersComplete,                                   // 0x000B
    HciEventOnLeSetScanEnableComplete,                                       // 0x000C
    NULL,                                                                    // 0x000D
    HciEventOnLeCreateConnectionCancelComplete,                              // 0x000E
    HciEventOnLeReadWhiteListSizeComplete,                                   // 0x000F
    HciEventOnLeClearWhiteListComplete,                                      // 0x0010
    HciEventOnLeAddDeviceToWhiteListComplete,                                // 0x0011
    HciEventOnLeRemoveDeviceFromWhiteListComplete,                           // 0x0012
    NULL,                                                                    // 0x0013
    HciEventOnLeSetHostChannelClassificationComplete,                        // 0x0014
    HciEventOnLeReadChannelMapComplete,                                      // 0x0015
    NULL,                                                                    // 0x0016
    HciEventOnLeEncryptComplete,                                             // 0x0017
    HciEventOnLeRandComplete,                                                // 0x0018
    NULL,                                                                    // 0x0019
    HciEventOnLeLongTermKeyRequestReplyComplete,                             // 0x001A
    HciEventOnLeLongTermKeyRequestNegativeReplyComplete,                     // 0x001B
    HciEventOnLeReadSupportedStatesComplete,                                 // 0x001C
    HciEventOnLeReceiverTestComplete,                                        // 0x001D
    HciEventOnLeTransmitterTestComplete,                                     // 0x001E
    HciEventOnLeTestEndComplete,                                             // 0x001F
    HciEventOnLeRemoteConnectionParameterRequestReplyComplete,               // 0x0020
    HciEventOnLeRemoteConnectionParameterRequestNegativeReplyComplete,       // 0x0021
    HciEventOnLeSetDataLengthComplete,                                       // 0x0022
    HciEventOnLeReadSuggestedDefaultDataLengthComplete,                      // 0x0023
    HciEventOnLeWriteSuggestedDefaultDataLengthComplete,                     // 0x0024
    NULL,                                                                    // 0x0025
    NULL,                                                                    // 0x0026
    HciEventOnLeAddDeviceToResolvingListComplete,                            // 0x0027
    HciEventOnLeRemoveDeviceFromResolvingListComplete,                       // 0x0028
    HciEventOnLeClearResolvingListComplete,                                  // 0x0029
    HciEventOnLeReadResolvingListSizeComplete,                               // 0x002A
    HciEventOnLeReadPeerResolvableAddressComplete,                           // 0x002B
    HciEventOnLeReadLocalResolvableAddressComplete,                          // 0x002C
    HciEventOnLeSetAddressResolutionEnableComplete,                          // 0x002D
    HciEventOnSetResolvablePrivateAddressTimeoutComplete,                    // 0x002E
    HciEventOnLeReadMaximumDataLengthComplete,                               // 0x002F
    HciEventOnLeReadPhyComplete,                                             // 0x0030
    HciEventOnLeSetDefaultPhyComplete,                                       // 0x0031
    HciEventOnLeSetPhyComplete,                                              // 0x0032
    HciEventOnLeEnhancedReceiverTestComplete,                                // 0x0033
    HciEventOnLeEnhancedTransmitterTestComplete,                             // 0x0034
    HciEventOnLeSetAdvertisingSetRandomAddressComplete,                      // 0x0035
    HciEventOnLeSetExtendedAdvertisingParametersComplete,                    // 0x0036
    HciEventOnLeSetExtendedAdvertisingDataComplete,                          // 0x0037
    HciEventOnLeSetExtendedScanResponseDataComplete,                         // 0x0038
    HciEventOnLeSetExtendedAdvertisingEnableComplete,                        // 0x0039
    HciEventOnLeReadMaximumAdvertisingDataLengthComplete,                    // 0x003A
    HciEventOnLeReadNumberofSupportedAdvertisingSetsComplete,                // 0x003B
    HciEventOnLeRemoveAdvertisingSetComplete,                                // 0x003C
    HciEventOnLeClearAdvertisingSetsComplete,                                // 0x003D
    HciEventOnLeSetPeriodicAdvertisingParametersComplete,                    // 0x003E
    HciEventOnLeSetPeriodicAdvertisingDataComplete,                          // 0x003F
    HciEventOnLeSetPeriodicAdvertisingEnableComplete,                        // 0x0040
    HciEventOnLeSetExtendedScanParametersComplete,                           // 0x0041
    HciEventOnLeSetExtendedScanEnableComplete,                               // 0x0042
    NULL,                                                                    // 0x0043
    NULL,                                                                    // 0x0044
    HciEventOnLePeriodicAdvertisingCreateSyncCancelComplete,                 // 0x0045
    HciEventOnLePeriodicAdvertisingTerminateSyncComplete,                    // 0x0046
    HciEventOnLeAddDeviceToPeriodicAdvertiserListComplete,                   // 0x0047
    HciEventOnLeRemoveDeviceFromPeriodicAdvertiserListComplete,              // 0x0048
    HciEventOnLeClearPeriodicAdvertiserListComplete,                         // 0x0049
    HciEventOnLeReadPeriodicAdvertiserListSizeComplete,                      // 0x004A
    HciEventOnLeReadTransmitPowerComplete,                                   // 0x004B
    HciEventOnLeReadRFPathCompensationComplete,                              // 0x004C
    HciEventOnLeWriteRFPathCompensationComplete,                             // 0x004D
    HciEventOnLeSetPrivacyModeComplete,                                      // 0x004E
    HciEventOnLeReceiverTestV3Complete,                                      // 0x004F
    HciEventOnLeTransmitterTestV3Complete,                                   // 0x0050
    HciEventOnLeSetConnectionlessCteTransmitParametersComplete,              // 0x0051
    HciEventOnLeSetConnectionlessCteTransmitEnableComplete,                  // 0x0052
    HciEventOnLeSetConnectionlessIqSamplingEnableComplete,                   // 0x0053
    HciEventOnLeSetConnectionCteReceiveParametersComplete,                   // 0x0054
    HciEventOnLeSetConnectionCteTransmitParametersComplete,                  // 0x0055
    HciEventOnLeConnectionCteRequestEnableComplete,                          // 0x0056
    HciEventOnLeConnectionCteResponseEnableComplete,                         // 0x0057
    HciEventOnLeReadAntennaInformationComplete,                              // 0x0058
    HciEventOnLeSetPeriodicAdvertisingReceiveEnableComplete,                 // 0x0059
    HciEventOnLePeriodicAdvertisingSyncTransferComplete,                     // 0x005A
    HciEventOnLePeriodicAdvertisingSetInfoTransferComplete,                  // 0x005B
    HciEventOnLeSetPeriodicAdvertisingSyncTransferParametersComplete,        // 0x005C
    HciEventOnLeSetDefaultPeriodicAdvertisingSyncTransferParametersComplete, // 0x005D
    NULL,                                                                    // 0x005E
    HciEventOnLeModifySleepClockAccuracyComplete,                            // 0x005F
    HciEventOnLeReadBufferSizeV2Complete,                                    // 0x0060
    HciEventOnLeReadIsoTxSyncComplete,                                       // 0x0061
    HciEventOnLeSetCigParametersComplete,                                    // 0x0062
    HciEventOnLeSetCigParametersTestComplete,                                // 0x0063
    HciEventOnLeCreateCisCommandComplete,                                    // 0x0064
    HciEventOnLeRemoveCigComplete,                                           // 0x0065
    HciEventOnLeAcceptCisRequestCommandComplete,                             // 0x0066
    HciEventOnLeRejectCisRequestComplete,                                    // 0x0067
    HciEventOnLeCreateBigCommandComplete,                                    // 0x0068
    HciEventOnLeCreateBigCommandComplete,                                    // 0x0069
    HciEventOnLeTerminateBigCommandComplete,                                 // 0x006A
    HciEventOnLeBigCreateSyncCommandComplete,                                // 0x006B
    HciEventOnLeBigTerminateSyncComplete,                                    // 0x006C
    HciEventOnLeRequestPeerScaCommandComplete,                               // 0x006D
    HciEventOnLeSetupIsoDataPathComplete,                                    // 0x006E
    HciEventOnLeRemoveIsoDataPathComplete,                                   // 0x006F
    HciEventOnLeIsoTransmitTestComplete,                                     // 0x0070
    HciEventOnLeIsoReceiveTestComplete,                                      // 0x0071
    HciEventOnLeIsoReadTestCountersComplete,                                 // 0x0072
    HciEventOnLeIsoTestEndComplete,                                          // 0x0073
    HciEventOnLeSetHostFeatureComplete,                                      // 0x0074
    HciEventOnLeReadIsoLinkQualityComplete,                                  // 0x0075
    HciEventOnLeEnhancedReadTransmitPowerLevelComplete,                      // 0x0076
    HciEventOnLeReadRemoteTransmitPowerLevelCommandComplete,                 // 0x0077
    HciEventOnLeSetPathLossReportingParametersComplete,                      // 0x0078
    HciEventOnLeSetPathLossReportingEnableComplete,                          // 0x0079
    HciEventOnLeSetTransmitPowerReportingEnableComplete,                     // 0x007A
    // Rows 0x007B-0x007E: 5.3 handlers kept; 0x007F-0x0086: 5.4 handlers.
    NULL,                                                                    // 0x007B
    HciEventOnLeSetDataRelatedAddressChangesComplete,                        // 0x007C
    HciEventOnLeSetDefaultSubrateComplete,                                   // 0x007D
    HciEventOnLeSubrateRequestComplete,                                      // 0x007E
    HciEventOnLeSetExtendedAdvertisingParametersV2Complete,                  // 0x007F
    HciEventOnLeSetDecisionDataComplete,                                      // 0x0080
    HciEventOnLeSetDecisionInstructionsComplete,                              // 0x0081
    HciEventOnLeSetPeriodicAdvertisingSubeventDataComplete,                  // 0x0082
    HciEventOnLeSetPeriodicAdvertisingResponseDataComplete,                  // 0x0083
    HciEventOnLeSetPeriodicSyncSubeventComplete,                             // 0x0084
    NULL,                                                                    // 0x0085
    HciEventOnLeSetPeriodicAdvertisingParametersV2Complete,                  // 0x0086
    // Rows 0x0087-0x009D are BT6.0 LE Controller commands. 0x0087 is filled by
    // the LL Extended Feature Set local read; 0x0088 stays NULL (Command_Status,
    // result arrives as the 0x2B subevent). 0x0089/0x008B are the Channel Sounding
    // capability Command_Complete handlers; 0x008A stays NULL as Command_Status, result 0x2C;
    // 0x008C stays NULL as Command_Status, result 0x2E;
    // 0x008D/0x008F are the CS Set Default Settings and Write Cached Remote FAE Table
    // Command_Complete handlers; 0x0092/0x0093/0x0095 are the CS Set Channel Classification,
    // Set Procedure Parameters and CS Test Command_Complete handlers; the remaining rows
    // 0x008E, 0x0090/0x0091, 0x0094, 0x0096-0x0097 stay NULL until owned - those CS
    // commands are Command_Status (their results arrive as the 0x2D/0x2F/0x30 subevents,
    // and 0x0096's as the 0x33) and 0x0097 is the [v2] Set Host Feature, not sent; rows
    // 0x0098-0x009C are the Monitoring Advertisers (MA) handlers, sections 7.8.146-7.8.150
    // (pp2867-2875; the last two are not in OCF order - see hci_def_le_cmd_6_0.h); 0x009D
    // stays NULL (Frame Space Update result arrives as the 0x35 subevent).
    HciEventOnLeReadAllLocalSupportedFeaturesComplete,                     // 0x0087
    NULL,                                                                    // 0x0088
    HciEventOnLeCsReadLocalSupportedCapabilitiesComplete,                 // 0x0089
    NULL,                                                                    // 0x008A
    HciEventOnLeCsWriteCachedRemoteSupportedCapabilitiesComplete,         // 0x008B
    NULL,                                                                    // 0x008C
    HciEventOnLeCsSetDefaultSettingsComplete,                             // 0x008D
    NULL,                                                                    // 0x008E
    HciEventOnLeCsWriteCachedRemoteFaeTableComplete,                      // 0x008F
    NULL,                                                                    // 0x0090
    NULL,                                                                    // 0x0091
    HciEventOnLeCsSetChannelClassificationComplete,                       // 0x0092
    HciEventOnLeCsSetProcedureParametersComplete,                         // 0x0093
    NULL,                                                                    // 0x0094
    HciEventOnLeCsTestComplete,                                           // 0x0095
    NULL,                                                                    // 0x0096
    NULL,                                                                    // 0x0097
    HciEventOnLeAddDeviceToMonitoredAdvertisersListComplete,                  // 0x0098
    HciEventOnLeRemoveDeviceFromMonitoredAdvertisersListComplete,             // 0x0099
    HciEventOnLeClearMonitoredAdvertisersListComplete,                        // 0x009A
    HciEventOnLeReadMonitoredAdvertisersListSizeComplete,                     // 0x009B
    HciEventOnLeEnableMonitoringAdvertisersComplete,                          // 0x009C
    NULL,                                                                    // 0x009D
};

// 0x005E is 7.8.93 LE Generate DHKey [v2]: asynchronous command, completion is
// reported by the LE Generate DHKey Complete event (Subevent 0x09, handled in
// hci_evt_le.c) - no Command Complete is generated, consistent with v1 (0x0026).
// 0x0085 (7.8.66 [v2]) is asynchronous like its [v1] counterpart (0x0043): no
// Command Complete, the outcome arrives as the LE Enhanced Connection Complete
// [v2] event (0x29) or as a Command_Status error handled in
// hci_cmd_failure_le_controller.c. OCFs 0x0080/0x0081 were unassigned in the
// 5.4 LE Controller command space (verified against the 5.4 command tables); since
// BT 6.0 they are the DBAF pair (0x0080 Set Decision Data, 0x0081 Set Decision
// Instructions), which the rows above fill.
#define LECONTROLLER_OCF_MAX 0x009D

void HciEventOnLeCommandComplete(uint16_t opCode, const void *param, uint8_t length)
{
    uint16_t ocf = GET_OCF(opCode);
    if (ocf > LECONTROLLER_OCF_MAX) {
        return;
    }

    HciLeCmdCompleteFunc func = g_leControllerCommandCompleteMap[ocf];
    if (func != NULL) {
        func(param, length);
    }
}