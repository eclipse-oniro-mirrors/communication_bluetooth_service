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

#include "btm_controller.h"

#include <securec.h>

#include "btstack.h"
#include "hci/hci.h"
#include "hci/hci_le_controller_5_0.h"
#include "hci/hci_def.h"
#include "hci/hci_error.h"
#include "log.h"
#include "platform/include/allocator.h"
#include "platform/include/event.h"
#include "platform/include/mutex.h"

#include "btm.h"

#define WAIT_CMD_TIMEOUT 15000

#define EXTENDED_FEATURES_PAGE_2 2

#define LE_DEFAULT_MAX_TX_OCTETS 0x001B
#define LE_DEFAULT_MAX_TX_TIME 0x0148
#define LE_DEFAULT_MAX_RX_OCTETS 0x001B
#define LE_DEFAULT_MAX_RX_TIME 0x0148

// Core Spec v5.0 valid ranges for LE Read Maximum Data Length (HCI_LE_Read_Maximum_Data_Length).
#define LE_MAX_TX_OCTETS_MAX 0x00FB
#define LE_MAX_TX_TIME_MAX 0x4290
#define LE_MAX_RX_OCTETS_MAX 0x00FB
#define LE_MAX_RX_TIME_MAX 0x4290

#define CHECK_RESULT_BREAK(x) \
    if ((x) != BT_SUCCESS) { \
        break;                \
    }

// Setup Controller
static HciEventCallbacks g_hciEventCallbacks;

static Event *g_waitSetupController = NULL;

static HciResetReturnParam g_hciResetResult;
static HciReadBufferSizeReturnParam g_readBufferSizeResult;
static HciHostBufferSizeReturnParam g_hostBufferSizeResult;
static HciReadLocalVersionInformationReturnParam g_readLocalVersionResult;
static HciReadBdAddrReturnParam g_readBdAddrResult;
static HciReadLocalSupportedCommandsReturnParam g_readLocalSupportedCommandsResult;
static HciReadLocalSupportedFeaturesReturnParam g_readLocalSupportedFeaturesResult;
static HciSetEventMaskReturnParam g_setEventMaskResult;
static uint8_t g_readLocalSupportedCodecsResult;
static BtmLocalSupportedCodecs g_localSupportedCodecs;
// Deep-copy buffers for BTM_GetLocalSupportedCodecs. The getter copies the codec
// lists under g_btmControllerLock so callers never observe pointers that
// BtmCloseController may free concurrently. The copy is only valid until the next
// call to BTM_GetLocalSupportedCodecs. HCI count fields are uint8, so 255 entries
// (UINT8_MAX) bound both lists.
#define BTM_LOCAL_SUPPORTED_CODECS_MAX (255)
static uint8_t g_localSupportedCodecsCopy[BTM_LOCAL_SUPPORTED_CODECS_MAX];
static BtmVendorSpecificCodec g_localVendorSpecificCodecsCopy[BTM_LOCAL_SUPPORTED_CODECS_MAX];
static HciReadLocalExtendedFeaturesReturnParam g_readLocalExtendedFeaturesResult[MAX_EXTENED_FEATURES_PAGE_COUNT];
static HciWriteLeHostSupportReturnParam g_writeLeHostSupportedResult;

static HciLeReadBufferSizeReturnParam g_leReadBufferSizeResult;
static HciLeReadLocalSupportedFeaturesReturnParam g_leReadLocalSupportedFeaturesResult;
static HciLeSetEventMaskReturnParam g_leSetEventMaskResult;
static HciLeReadWhiteListSizeReturnParam g_leReadWhiteListSizeResult;
static HciLeReadResolvingListSizeReturnParam g_leReadResolvingListSizeResult;

// Serializes BTM_GetLocalSupportedCodecs against BtmCloseController: both hold
// this lock while touching g_localSupportedCodecs / its copy buffers, so callers
// never read codec lists that BtmCloseController is freeing concurrently.
static Mutex *g_btmControllerLock = NULL;

// Guards the g_leReadMaximumDataLength{Pending,Valid,Result} state so concurrent
// BTM_LeReadMaximumDataLength callers cannot issue duplicate HCI reads or observe
// a half-updated result (see BtmLeReadMaximumDataLength).
static Mutex *g_leReadMaximumDataLengthMutex = NULL;
static bool g_leReadMaximumDataLengthPending = false;
static bool g_leReadMaximumDataLengthValid = false;
// Stores the controller's supported max TX/RX PDU octets and air time.
static HciLeReadMaximumDataLengthReturnParam g_leReadMaximumDataLengthResult = {
    .status = HCI_HARDWARE_FAILURE,
};

static void BtmControllerOnResetComplete(const HciResetReturnParam *returnParam)
{
    g_hciResetResult = *returnParam;
    EventSet(g_waitSetupController);
}

static void BtmControllerOnReadBufferSizeComplete(const HciReadBufferSizeReturnParam *returnParam)
{
    g_readBufferSizeResult = *returnParam;
    EventSet(g_waitSetupController);
}

static void BtmControllerOnHostBufferSizeComplete(const HciHostBufferSizeReturnParam *returnParam)
{
    g_hostBufferSizeResult = *returnParam;
    EventSet(g_waitSetupController);
}

static void BtmControllerOnReadLocalVersionInformationComplete(
    const HciReadLocalVersionInformationReturnParam *returnParam)
{
    g_readLocalVersionResult = *returnParam;
    EventSet(g_waitSetupController);
}

static void BtmControllerOnReadBdAddrComplete(const HciReadBdAddrReturnParam *returnParam)
{
    g_readBdAddrResult = *returnParam;
    EventSet(g_waitSetupController);
}

static void BtmControllerOnReadLocalSupportedCommandsComplete(
    const HciReadLocalSupportedCommandsReturnParam *returnParam)
{
    g_readLocalSupportedCommandsResult = *returnParam;
    EventSet(g_waitSetupController);
}

static void BtmControllerOnReadLocalSupportedFeaturesComplete(
    const HciReadLocalSupportedFeaturesReturnParam *returnParam)
{
    g_readLocalSupportedFeaturesResult = *returnParam;
    EventSet(g_waitSetupController);
}

static void BtmControllerOnReadLocalExtendedFeaturesComplete(const HciReadLocalExtendedFeaturesReturnParam *returnParam)
{
    if (returnParam->pageNumber < MAX_EXTENED_FEATURES_PAGE_COUNT) {
        g_readLocalExtendedFeaturesResult[returnParam->pageNumber] = *returnParam;
    }
    EventSet(g_waitSetupController);
}

static void BtmControllerOnSetEventMaskComplete(const HciSetEventMaskReturnParam *returnParam)
{
    g_setEventMaskResult = *returnParam;
    EventSet(g_waitSetupController);
}

static void BtmControllerClearLocalSupportedCodecs(void)
{
    if (g_localSupportedCodecs.supportedCodecs != NULL) {
        MEM_MALLOC.free(g_localSupportedCodecs.supportedCodecs);
        g_localSupportedCodecs.supportedCodecs = NULL;
    }
    if (g_localSupportedCodecs.vendorSpecificCodecs != NULL) {
        MEM_MALLOC.free(g_localSupportedCodecs.vendorSpecificCodecs);
        g_localSupportedCodecs.vendorSpecificCodecs = NULL;
    }
    g_localSupportedCodecs.numberOfSupportedCodecs = 0;
    g_localSupportedCodecs.numberOfSupportedVendorSpecificCodecs = 0;
    g_readLocalSupportedCodecsResult = HCI_HARDWARE_FAILURE;
}

static void BtmControllerCopySupportedCodecs(const HciReadLocalSupportedCodecsReturnParam *returnParam)
{
    if (returnParam == NULL) {
        g_readLocalSupportedCodecsResult = HCI_HARDWARE_FAILURE;
        return;
    }
    if (g_localSupportedCodecs.supportedCodecs != NULL) {
        MEM_MALLOC.free(g_localSupportedCodecs.supportedCodecs);
    }
    g_localSupportedCodecs.supportedCodecs = NULL;
    g_localSupportedCodecs.numberOfSupportedCodecs = 0;

    if (returnParam->numberOfSupportedCodecs == 0) {
        return;
    }

    if (returnParam->supportedCodecs == NULL) {
        g_readLocalSupportedCodecsResult = HCI_HARDWARE_FAILURE;
        return;
    }

    g_localSupportedCodecs.supportedCodecs =
        MEM_MALLOC.alloc(sizeof(uint8_t) * returnParam->numberOfSupportedCodecs);
    if (g_localSupportedCodecs.supportedCodecs == NULL) {
        g_readLocalSupportedCodecsResult = HCI_HARDWARE_FAILURE;
        return;
    }
    g_localSupportedCodecs.numberOfSupportedCodecs = returnParam->numberOfSupportedCodecs;
    for (uint8_t i = 0; i < g_localSupportedCodecs.numberOfSupportedCodecs; i++) {
        g_localSupportedCodecs.supportedCodecs[i] = returnParam->supportedCodecs[i];
    }
}

static void BtmControllerCopyVendorSpecificCodecs(const HciReadLocalSupportedCodecsReturnParam *returnParam)
{
    if (returnParam == NULL) {
        g_readLocalSupportedCodecsResult = HCI_HARDWARE_FAILURE;
        return;
    }
    if (g_localSupportedCodecs.vendorSpecificCodecs != NULL) {
        MEM_MALLOC.free(g_localSupportedCodecs.vendorSpecificCodecs);
    }
    g_localSupportedCodecs.vendorSpecificCodecs = NULL;
    g_localSupportedCodecs.numberOfSupportedVendorSpecificCodecs = 0;

    if (returnParam->numberOfSupportedVendorSpecificCodecs == 0) {
        return;
    }

    if (returnParam->vendorSpecificCodecs == NULL) {
        g_readLocalSupportedCodecsResult = HCI_HARDWARE_FAILURE;
        return;
    }

    g_localSupportedCodecs.vendorSpecificCodecs = MEM_MALLOC.alloc(
        sizeof(BtmVendorSpecificCodec) * returnParam->numberOfSupportedVendorSpecificCodecs);
    if (g_localSupportedCodecs.vendorSpecificCodecs == NULL) {
        g_readLocalSupportedCodecsResult = HCI_HARDWARE_FAILURE;
        return;
    }
    g_localSupportedCodecs.numberOfSupportedVendorSpecificCodecs = returnParam->numberOfSupportedVendorSpecificCodecs;
    for (uint8_t i = 0; i < g_localSupportedCodecs.numberOfSupportedVendorSpecificCodecs; i++) {
        g_localSupportedCodecs.vendorSpecificCodecs[i].companyID =
            returnParam->vendorSpecificCodecs[i].companyID;
        g_localSupportedCodecs.vendorSpecificCodecs[i].vendorDefinedCodecID =
            returnParam->vendorSpecificCodecs[i].vendorDefinedCodecID;
    }
}

static void BtmControllerOnReadLocalSupportedCodecs(const HciReadLocalSupportedCodecsReturnParam *returnParam)
{
    if (g_btmControllerLock != NULL) {
        MutexLock(g_btmControllerLock);
    }

    g_readLocalSupportedCodecsResult = returnParam->status;
    if (g_readLocalSupportedCodecsResult == HCI_SUCCESS) {
        BtmControllerCopySupportedCodecs(returnParam);
        BtmControllerCopyVendorSpecificCodecs(returnParam);
    }

    if (g_btmControllerLock != NULL) {
        MutexUnlock(g_btmControllerLock);
    }

    EventSet(g_waitSetupController);
}

static void BtmControllerOnLeReadBufferSizeComplete(const HciLeReadBufferSizeReturnParam *returnParam)
{
    g_leReadBufferSizeResult = *returnParam;
    EventSet(g_waitSetupController);
}

static void BtmControllerOnLeReadLocalSupportedFeaturesComplete(
    const HciLeReadLocalSupportedFeaturesReturnParam *returnParam)
{
    g_leReadLocalSupportedFeaturesResult = *returnParam;
    EventSet(g_waitSetupController);
}

static void BtmControllerOnLeSetEventMaskComplete(const HciLeSetEventMaskReturnParam *returnParam)
{
    g_leSetEventMaskResult = *returnParam;
    EventSet(g_waitSetupController);
}

static void BtmControllerOnLeReadMaximumDataLengthComplete(const HciLeReadMaximumDataLengthReturnParam *returnParam)
{
    if (returnParam == NULL) {
        return;
    }

    // Hold the controller lifecycle lock while checking and locking the data mutex
    // so that BtmCloseController cannot delete the mutex between the check and the lock.
    if (g_btmControllerLock == NULL) {
        return;
    }
    MutexLock(g_btmControllerLock);
    if (g_leReadMaximumDataLengthMutex == NULL) {
        MutexUnlock(g_btmControllerLock);
        return;
    }
    MutexLock(g_leReadMaximumDataLengthMutex);
    MutexUnlock(g_btmControllerLock);

    if (!g_leReadMaximumDataLengthPending) {
        MutexUnlock(g_leReadMaximumDataLengthMutex);
        return;
    }
    g_leReadMaximumDataLengthResult = *returnParam;
    g_leReadMaximumDataLengthPending = false;
    g_leReadMaximumDataLengthValid = (returnParam->status == HCI_SUCCESS);
    MutexUnlock(g_leReadMaximumDataLengthMutex);
    EventSet(g_waitSetupController);
}

static void BtmControllerOnLeReadWhiteListSizeComplete(const HciLeReadWhiteListSizeReturnParam *returnParam)
{
    g_leReadWhiteListSizeResult = *returnParam;
    EventSet(g_waitSetupController);
}

static void BtmControllerOnLeReadResolvingListSizeComplete(const HciLeReadResolvingListSizeReturnParam *returnParam)
{
    g_leReadResolvingListSizeResult = *returnParam;
    EventSet(g_waitSetupController);
}

static void BtmControllerOnWriteLeHostSupportedComplete(const HciWriteLeHostSupportReturnParam *returnParam)
{
    g_writeLeHostSupportedResult = *returnParam;
    EventSet(g_waitSetupController);
}

bool BtmIsControllerSupportedReadLocalSupportedCodecsCommand()
{
    return HciSupportReadLocalSupportedCodecs(g_readLocalSupportedCommandsResult.supportedCommands);
}

bool BtmIsControllerSupportedEnhancedSetupSynchronousConnection()
{
    return HciSupportEnhancedSetupSynchronousConnection(g_readLocalSupportedCommandsResult.supportedCommands);
}

bool BtmIsControllerSupportedEnhancedAcceptSynchronousConnection()
{
    return HciSupportEnhancedAcceptSynchronousConnection(g_readLocalSupportedCommandsResult.supportedCommands);
}

static bool BtmIsControllerSupportedLeReadLocalP256PublicKey()
{
    return HciSupportLeReadLocalP256PublicKey(g_readLocalSupportedCommandsResult.supportedCommands);
}

static bool BtmIsControllerSupportedLeGenerateDhKey()
{
    return HciSupportLeGenerateDhKey(g_readLocalSupportedCommandsResult.supportedCommands);
}

bool BtmIsControllerSupportedLeSetPrivacyMode()
{
    return HciSupportLeSetPrivacyMode(g_readLocalSupportedCommandsResult.supportedCommands);
}

static HciEventCallbacks g_hciEventCallbacks = {
    .resetComplete = BtmControllerOnResetComplete,
    .readBufferSizeComplete = BtmControllerOnReadBufferSizeComplete,
    .hostBufferSizeComplete = BtmControllerOnHostBufferSizeComplete,
    .readLocalVersionInformationComplete = BtmControllerOnReadLocalVersionInformationComplete,
    .readBdAddrComplete = BtmControllerOnReadBdAddrComplete,
    .readLocalSupportedCommandsComplete = BtmControllerOnReadLocalSupportedCommandsComplete,
    .readLocalSupportedFeaturesComplete = BtmControllerOnReadLocalSupportedFeaturesComplete,
    .readLocalExtendedFeaturesComplete = BtmControllerOnReadLocalExtendedFeaturesComplete,
    .setEventMaskComplete = BtmControllerOnSetEventMaskComplete,
    .readLocalSupportedCodecsComplete = BtmControllerOnReadLocalSupportedCodecs,
    .writeLeHostSupportComplete = BtmControllerOnWriteLeHostSupportedComplete,

    .leSetEventMaskComplete = BtmControllerOnLeSetEventMaskComplete,
    .leReadBufferSizeComplete = BtmControllerOnLeReadBufferSizeComplete,
    .leReadLocalSupportedFeaturesComplete = BtmControllerOnLeReadLocalSupportedFeaturesComplete,
    .leReadMaximumDataLengthComplete = BtmControllerOnLeReadMaximumDataLengthComplete,
    .leReadWhiteListSizeComplete = BtmControllerOnLeReadWhiteListSizeComplete,
    .leReadResolvingListSizeComplete = BtmControllerOnLeReadResolvingListSizeComplete,
};

static uint64_t BtmGetLeEventMask()
{
    uint64_t leEventMask = LE_EVENT_MASK_DEFAULT;

    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // Table 3.2: Bluetooth Controller supporting LE requirements

    // C6
    if (BTM_IsControllerSupportConnectionParametersRequestProcedure()) {
        leEventMask |= LE_EVENT_MASK_LE_REMOTE_CONNECTION_PARAMETER_REQUEST_EVENT;
    }
    // C8
    if (BTM_IsControllerSupportLeDataPacketLengthExtension()) {
        leEventMask |= LE_EVENT_MASK_LE_DATA_LENGTH_CHANGE_EVENT;
    }
    // C9
    if (BTM_IsControllerSupportLlPrivacy()) {
        leEventMask |= LE_EVENT_MASK_LE_DIRECTED_ADVERTISING_REPORT_EVENT;
    }
    // C11
    if (BTM_IsControllerSupportLe2MPhy() || BTM_IsControllerSupportLeCodedPhy()) {
        leEventMask |= LE_EVENT_MASK_LE_PHY_UPDATE_COMPLETE_EVENT;
    }
    // C17
    if (BTM_IsControllerSupportLeExtendedAdvertising()) {
        leEventMask |=
            (LE_EVENT_MASK_LE_SCAN_REQUEST_RECEIVED_EVENT | LE_EVENT_MASK_LE_EXTENDED_ADVERTISING_SET_TERMINATED_EVENT);
    }
    // C19
    if (BTM_IsControllerSupportLeExtendedAdvertising()) {
        leEventMask |=
            (LE_EVENT_MASK_LE_EXTENDED_SCAN_TIMEOUT_EVENT | LE_EVENT_MASK_LE_EXTENDED_ADVERTISING_REPORT_EVENT);
    }
    // C20
    if (BTM_IsControllerSupportLePeriodicAdvertising()) {
        leEventMask |= (LE_EVENT_MASK_LE_PERIODIC_ADVERTISING_REPORT_EVENT |
                        LE_EVENT_MASK_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED_EVENT |
                        LE_EVENT_MASK_LE_PERIODIC_ADVERTISING_SYNC_LOST_EVENT);
    }
    // C22
    if (BTM_IsControllerSupportChannelSelectionAlgorithm2()) {
        leEventMask |= LE_EVENT_MASK_LE_CHANNEL_SELECTION_ALGORITHM_EVENT;
    }
    // C23
    if (BTM_IsControllerSupportLlPrivacy() || BTM_IsControllerSupportLeExtendedAdvertising()) {
        leEventMask |= LE_EVENT_MASK_LE_ENHANCED_CONNECTION_COMPLETE_EVENT;
    }
    // Optional
    if (BtmIsControllerSupportedLeReadLocalP256PublicKey()) {
        leEventMask |= LE_EVENT_MASK_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_EVENT;
    }
    if (BtmIsControllerSupportedLeGenerateDhKey()) {
        leEventMask |= LE_EVENT_MASK_LE_GENERATE_DHKEY_COMPLETE_EVENT;
    }

    return leEventMask;
}

static int BtmHciReset()
{
    int result = HCI_Reset();
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_Reset failed: %{public}d", result);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        if (g_hciResetResult.status != HCI_SUCCESS) {
            LOG_ERROR("HCI_Reset status: 0x%02x", g_hciResetResult.status);
            result = BT_OPERATION_FAILED;
        }
    } else {
        LOG_ERROR("HCI_Reset Timeout");
        result = BT_OPERATION_FAILED;
    }

    return result;
}

static int BtmReadBufferSize()
{
    int result = HCI_ReadBufferSize();
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_ReadBufferSize failed: %{public}d", result);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        switch (g_readBufferSizeResult.status) {
            case HCI_SUCCESS:
            case HCI_UNKNOWN_HCI_COMMAND:
                result = BT_SUCCESS;
                break;
            default:
                LOG_ERROR("HCI_ReadBufferSize status: 0x%02x", g_readBufferSizeResult.status);
                result = BT_OPERATION_FAILED;
                break;
        }
    } else {
        LOG_ERROR("HCI_ReadBufferSize Timeout");
        result = BT_OPERATION_FAILED;
    }
    return result;
}

static int BtmHostBufferSize()
{
    HciHostBufferSizeCmdParam hostBufferSizeParam = {
        .hostAclDataPacketLength = L2CAP_MTU_SIZE,
        .hostSynchronousDataPacketLength = SCO_HOST_BUFFER_SIZE,
        .hostTotalNumAclDataPackets = HOST_ACL_DATA_PACKETS,
        .hostTotalNumSynchronousDataPackets = HOST_SCO_DATA_PACKETS,
    };
    int result = HCI_HostBufferSize(&hostBufferSizeParam);
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_HostBufferSize failed: %{public}d", result);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        switch (g_hostBufferSizeResult.status) {
            case HCI_SUCCESS:
            case HCI_UNKNOWN_HCI_COMMAND:
                result = BT_SUCCESS;
                break;
            default:
                LOG_ERROR("HCI_HostBufferSize status: 0x%02x", g_hostBufferSizeResult.status);
                result = BT_OPERATION_FAILED;
                break;
        }
    } else {
        LOG_ERROR("HCI_HostBufferSize Timeout");
        result = BT_OPERATION_FAILED;
    }
    return result;
}

static int BtmReadLocalVersionInformation()
{
    int result = HCI_ReadLocalVersionInformation();
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_ReadLocalVersionInformation failed: %{public}d", result);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        if (g_readLocalVersionResult.status != HCI_SUCCESS) {
            LOG_ERROR("HCI_ReadLocalVersionInformation status: 0x%02x", g_readLocalVersionResult.status);
            result = BT_OPERATION_FAILED;
        }
    } else {
        LOG_ERROR("HCI_ReadLocalVersionInformation Timeout");
        result = BT_OPERATION_FAILED;
    }
    return result;
}

static int BtmReadBdAddr()
{
    int result = HCI_ReadBdAddr();
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_ReadBdAddr failed: %{public}d", result);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        if (g_readBdAddrResult.status != HCI_SUCCESS) {
            LOG_INFO("No public address");
            result = BT_SUCCESS;
        }
    } else {
        LOG_ERROR("HCI_ReadBdAddr Timeout");
        result = BT_OPERATION_FAILED;
    }
    return result;
}

static int BtmReadLocalSupportedCommands()
{
    int result = HCI_ReadLocalSupportedCommands();
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_ReadLocalSupportedCommands failed: %{public}d", result);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        if (g_readLocalSupportedCommandsResult.status != HCI_SUCCESS) {
            LOG_ERROR("HCI_ReadLocalSupportedCommands status: 0x%02x", g_readLocalSupportedCommandsResult.status);
            result = BT_OPERATION_FAILED;
        }
    } else {
        LOG_ERROR("HCI_ReadLocalSupportedCommands Timeout");
        result = BT_OPERATION_FAILED;
    }
    return result;
}

static int BtmReadLocalSupportedFeatures()
{
    int result = HCI_ReadLocalSupportedFeatures();
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_ReadLocalSupportedFeatures failed: %{public}d", result);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        if (g_readLocalSupportedFeaturesResult.status != HCI_SUCCESS) {
            LOG_ERROR("HCI_ReadLocalSupportedFeatures status: 0x%02x", g_readLocalSupportedFeaturesResult.status);
            result = BT_OPERATION_FAILED;
        }
    } else {
        LOG_ERROR("HCI_ReadLocalSupportedFeatures Timeout");
        result = BT_OPERATION_FAILED;
    }
    return result;
}

static int BtmReadLocalExtendedFeatures()
{
    int result;

    HciReadLocalExtendedFeaturesParam readLocalExtendedFeaturesParam = {
        .pageNumber = 0,
    };

    for (uint8_t i = 0; i < MAX_EXTENED_FEATURES_PAGE_COUNT; i++) {
        readLocalExtendedFeaturesParam.pageNumber = i;

        result = HCI_ReadLocalExtendedFeatures(&readLocalExtendedFeaturesParam);
        if (result != BT_SUCCESS) {
            LOG_ERROR("HCI_ReadLocalExtendedFeatures failed: %{public}d", result);
            break;
        }
        if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) != 0) {
            LOG_ERROR("HCI_ReadLocalExtendedFeatures Timeout");
            result = BT_OPERATION_FAILED;
            break;
        }

        switch (g_readLocalExtendedFeaturesResult[i].status) {
            case HCI_SUCCESS:
            case HCI_UNKNOWN_HCI_COMMAND:
                result = BT_SUCCESS;
                break;
            default:
                LOG_ERROR("HCI_ReadLocalExtendedFeatures status: 0x%02x", g_readLocalExtendedFeaturesResult[i].status);
                result = BT_OPERATION_FAILED;
                break;
        }

        if (result != BT_SUCCESS) {
            break;
        }

        if (g_readLocalExtendedFeaturesResult[i].pageNumber >= g_readLocalExtendedFeaturesResult[i].maximunPageNumber) {
            break;
        }
    }

    return result;
}

static int BtmSetEventMask()
{
    HciSetEventMaskParam setEventMaskParam = {
        .eventMask = HCI_EVENT_MASK_CORE_5_0,
    };
    int result = HCI_SetEventMask(&setEventMaskParam);
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_SetEventMask failed: %{public}d", result);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        if (g_setEventMaskResult.status != HCI_SUCCESS) {
            LOG_ERROR("HCI_SetEventMask status: 0x%02x", g_setEventMaskResult.status);
            result = BT_OPERATION_FAILED;
        }
    } else {
        LOG_ERROR("HCI_SetEventMask Timeout");
        result = BT_OPERATION_FAILED;
    }
    return result;
}

static int BtmReadLocalSupportedCodecs()
{
    int result = HCI_ReadLocalSupportedCodecs();
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_ReadLocalSupportedCodecs failed: %{public}d", result);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        if (g_readLocalSupportedCodecsResult != HCI_SUCCESS) {
            LOG_ERROR("HCI_ReadLocalSupportedCodecs status: 0x%02x", g_readLocalSupportedCodecsResult);
            result = BT_OPERATION_FAILED;
        }
    } else {
        LOG_ERROR("HCI_ReadLocalSupportedCodecs Timeout");
        result = BT_OPERATION_FAILED;
    }
    return result;
}

static int BtmWriteLeHostSupport()
{
    HciWriteLeHostSupportParam writeLeHostSupportParam = {
        .leSupportedHost = HCI_LE_SUPPORTED_HOST_ENABLED,
        .simultaneousLeHost = HCI_SIMULTANEOUS_LE_HOST_DISABLED,
    };
    int result = HCI_WriteLeHostSupport(&writeLeHostSupportParam);
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_WriteLeHostSupport failed: %{public}d", result);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        switch (g_writeLeHostSupportedResult.status) {
            case HCI_SUCCESS:
            case HCI_UNKNOWN_HCI_COMMAND:
                result = BT_SUCCESS;
                break;
            default:
                LOG_ERROR("HCI_WriteLeHostSupport status: 0x%02x", g_writeLeHostSupportedResult.status);
                result = BT_OPERATION_FAILED;
                break;
        }
    } else {
        LOG_ERROR("HCI_WriteLeHostSupport Timeout");
        result = BT_OPERATION_FAILED;
    }
    return result;
}

static int BtmLeReadBufferSize()
{
    int result = HCI_LeReadBufferSize();
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_LeReadBufferSize failed: %{public}d", result);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        if (g_leReadBufferSizeResult.status != HCI_SUCCESS) {
            LOG_ERROR("HCI_LeReadBufferSize status: 0x%02x", g_leReadBufferSizeResult.status);
            result = BT_OPERATION_FAILED;
        }
    } else {
        LOG_ERROR("HCI_LeReadBufferSize Timeout");
        result = BT_OPERATION_FAILED;
    }
    return result;
}

static int BtmLeReadWhiteListSize()
{
    int result = HCI_LeReadWhiteListSize();
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_LeReadWhiteListSize failed: %{public}d", result);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        if (g_leReadWhiteListSizeResult.status != HCI_SUCCESS) {
            LOG_ERROR("HCI_LeReadWhiteListSize status: 0x%02x", g_leReadWhiteListSizeResult.status);
            result = BT_OPERATION_FAILED;
        }
    } else {
        LOG_ERROR("HCI_LeReadWhiteListSize Timeout");
        result = BT_OPERATION_FAILED;
    }
    return result;
}

static int BtmLeReadLocalSupportedFeatures()
{
    int result = HCI_LeReadLocalSupportedFeatures();
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_LeReadLocalSupportedFeatures failed: %{public}d", result);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        if (g_leReadLocalSupportedFeaturesResult.status != HCI_SUCCESS) {
            LOG_ERROR("HCI_LeReadLocalSupportedFeatures status: 0x%02x", g_leReadLocalSupportedFeaturesResult.status);
            result = BT_OPERATION_FAILED;
        }
    } else {
        LOG_ERROR("HCI_LeReadLocalSupportedFeatures Timeout");
        result = BT_OPERATION_FAILED;
    }
    return result;
}

static void BtmSetDefaultLeMaximumDataLength(void)
{
    g_leReadMaximumDataLengthResult.supportedMaxTxOctets = LE_DEFAULT_MAX_TX_OCTETS;
    g_leReadMaximumDataLengthResult.supportedMaxTxTime = LE_DEFAULT_MAX_TX_TIME;
    g_leReadMaximumDataLengthResult.supportedMaxRxOctets = LE_DEFAULT_MAX_RX_OCTETS;
    g_leReadMaximumDataLengthResult.supportedMaxRxTime = LE_DEFAULT_MAX_RX_TIME;
    // Caller owns the status field; do not overwrite it here.
}

static int BtmLeReadMaximumDataLength()
{
    if (g_btmControllerLock == NULL || g_leReadMaximumDataLengthMutex == NULL) {
        return BT_OPERATION_FAILED;
    }

    EventClear(g_waitSetupController);

    // Hold the controller lifecycle lock only while checking and locking the data
    // mutex, then release it before EventWait: the HCI complete callback
    // (BtmControllerOnLeReadMaximumDataLengthComplete) must acquire the same
    // controller lock before it can EventSet, so holding it across the wait would
    // block the callback and force a 15s timeout every time.
    MutexLock(g_btmControllerLock);
    MutexLock(g_leReadMaximumDataLengthMutex);
    MutexUnlock(g_btmControllerLock);

    g_leReadMaximumDataLengthResult.status = HCI_HARDWARE_FAILURE;
    g_leReadMaximumDataLengthPending = true;
    g_leReadMaximumDataLengthValid = false;
    MutexUnlock(g_leReadMaximumDataLengthMutex);

    int result = HCI_LeReadMaximumDataLength();
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_LeReadMaximumDataLength failed: %{public}d", result);
        MutexLock(g_leReadMaximumDataLengthMutex);
        g_leReadMaximumDataLengthPending = false;
        BtmSetDefaultLeMaximumDataLength();
        MutexUnlock(g_leReadMaximumDataLengthMutex);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        MutexLock(g_leReadMaximumDataLengthMutex);
        if (g_leReadMaximumDataLengthResult.status != HCI_SUCCESS) {
            LOG_ERROR("HCI_LeReadMaximumDataLength status: 0x%02x", g_leReadMaximumDataLengthResult.status);
            // Clear the pending flag like the timeout path: a late completion
            // callback from this command (rk3568 firmware may never deliver one)
            // would otherwise leave the flag set and its EventSet would pollute
            // the next command's EventWait in the init sequence.
            g_leReadMaximumDataLengthPending = false;
            BtmSetDefaultLeMaximumDataLength();
            result = BT_OPERATION_FAILED;
        }
        MutexUnlock(g_leReadMaximumDataLengthMutex);
    } else {
        LOG_ERROR("HCI_LeReadMaximumDataLength Timeout");
        MutexLock(g_leReadMaximumDataLengthMutex);
        if (g_leReadMaximumDataLengthPending) {
            g_leReadMaximumDataLengthPending = false;
            BtmSetDefaultLeMaximumDataLength();
            g_leReadMaximumDataLengthResult.status = HCI_HARDWARE_FAILURE;
        }
        MutexUnlock(g_leReadMaximumDataLengthMutex);
        result = BT_OPERATION_FAILED;
    }
    return result;
}

static int BtmLeReadResolvingListSize()
{
    // Clear the event first: a stale signal from a timed-out predecessor command
    // (e.g. BtmLeReadMaximumDataLength) would otherwise make EventWait return
    // immediately and this command would consume a zero-initialized result.
    EventClear(g_waitSetupController);

    int result = HCI_LeReadResolvingListSize();
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_LeReadResolvingListSize failed: %{public}d", result);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        if (g_leReadResolvingListSizeResult.status != HCI_SUCCESS) {
            LOG_ERROR("HCI_LeReadResolvingListSize status: 0x%02x", g_leReadResolvingListSizeResult.status);
            result = BT_OPERATION_FAILED;
        }
    } else {
        LOG_ERROR("HCI_LeReadResolvingListSize Timeout");
        result = BT_OPERATION_FAILED;
    }
    return result;
}

static int BtmLeSetEventMask()
{
    HciLeSetEventMaskParam lsSetEventMaskParam = {
        .leEventMask = BtmGetLeEventMask(),
    };
    int result = HCI_LeSetEventMask(&lsSetEventMaskParam);
    if (result != BT_SUCCESS) {
        LOG_ERROR("HCI_LeSetEventMask failed: %{public}d", result);
        return result;
    }
    if (EventWait(g_waitSetupController, WAIT_CMD_TIMEOUT) == 0) {
        if (g_leSetEventMaskResult.status != HCI_SUCCESS) {
            LOG_ERROR("HCI_LeSetEventMask status: 0x%02x", g_leSetEventMaskResult.status);
            result = BT_OPERATION_FAILED;
        }
    } else {
        LOG_ERROR("HCI_LeSetEventMask Timeout");
        result = BT_OPERATION_FAILED;
    }
    return result;
}

static int BtmInitLeFeature()
{
    int result;

    do {
        result = BtmWriteLeHostSupport();
        CHECK_RESULT_BREAK(result);

        result = BtmLeReadBufferSize();
        CHECK_RESULT_BREAK(result);

        HCI_SetLeBufferSize(
            g_leReadBufferSizeResult.hcLeAclDataPacketLength, g_leReadBufferSizeResult.hcTotalNumLeDataPackets);

        result = BtmLeReadWhiteListSize();
        CHECK_RESULT_BREAK(result);

        result = BtmLeReadLocalSupportedFeatures();
        CHECK_RESULT_BREAK(result);

        if (BTM_IsControllerSupportLeDataPacketLengthExtension()) {
            result = BtmLeReadMaximumDataLength();
            if (result != BT_SUCCESS) {
                LOG_WARN("BtmLeReadMaximumDataLength failed, using defaults: %{public}d", result);
                MutexLock(g_leReadMaximumDataLengthMutex);
                g_leReadMaximumDataLengthResult.status = HCI_HARDWARE_FAILURE;
                g_leReadMaximumDataLengthValid = false;
                MutexUnlock(g_leReadMaximumDataLengthMutex);
                result = BT_SUCCESS;
            }
        }

        if (BTM_IsControllerSupportLlPrivacy()) {
            result = BtmLeReadResolvingListSize();
            CHECK_RESULT_BREAK(result);
        }

        result = BtmLeSetEventMask();
    } while (0);

    return result;
}

// On a codec read failure, reset the codec state under the controller lock so
// a later success can repopulate it from a clean slate.
static void BtmInitControllerHandleCodecResult(int result)
{
    if (result != BT_SUCCESS) {
        LOG_WARN("%{public}s: BtmReadLocalSupportedCodecs failed, reset codec state: %{public}d",
            __FUNCTION__, result);
        g_readLocalSupportedCodecsResult = HCI_HARDWARE_FAILURE;
        if (g_btmControllerLock != NULL) {
            MutexLock(g_btmControllerLock);
        }
        BtmControllerClearLocalSupportedCodecs();
        if (g_btmControllerLock != NULL) {
            MutexUnlock(g_btmControllerLock);
        }
    }
}

// Run the init-time HCI command sequence. Each step checks its result via
// CHECK_RESULT_BREAK and stops the sequence on the first failure; the final
// step (LE feature init) keeps its result as the sequence outcome.
static int BtmInitControllerCommandSequence(void)
{
    int result;

    do {
        // Reset Command
        result = BtmHciReset();
        CHECK_RESULT_BREAK(result);

        // Read Buffer Size Command
        result = BtmReadBufferSize();
        CHECK_RESULT_BREAK(result);

        HCI_SetBufferSize(
            g_readBufferSizeResult.hcAclDataPacketLength, g_readBufferSizeResult.hcTotalNumAclDataPackets);

        // Host Buffer Size Command
        result = BtmHostBufferSize();
        CHECK_RESULT_BREAK(result);

        // Read Local Version Information Command
        result = BtmReadLocalVersionInformation();
        CHECK_RESULT_BREAK(result);

        // Read BD_ADDR Command
        result = BtmReadBdAddr();
        CHECK_RESULT_BREAK(result);

        // Read Local Supported Commands Command
        result = BtmReadLocalSupportedCommands();
        CHECK_RESULT_BREAK(result);

        // Read Local Supported Features Command
        result = BtmReadLocalSupportedFeatures();
        CHECK_RESULT_BREAK(result);

        // Read Local Extended Features Command
        result = BtmReadLocalExtendedFeatures();
        CHECK_RESULT_BREAK(result);

        // Set Event Mask Command
        result = BtmSetEventMask();
        CHECK_RESULT_BREAK(result);

        if (BtmIsControllerSupportedReadLocalSupportedCodecsCommand()) {
            result = BtmReadLocalSupportedCodecs();
            BtmInitControllerHandleCodecResult(result);
            result = BT_SUCCESS;
        }

        if (BTM_IsControllerSupportLe()) {
            result = BtmInitLeFeature();
        }
    } while (0);

    return result;
}

int BtmInitController()
{
    int result;

    if (g_btmControllerLock == NULL) {
        Mutex *newLock = MutexCreate();
        if (newLock == NULL) {
            return BT_NO_MEMORY;
        }
        Mutex *expected = NULL;
        if (!__atomic_compare_exchange_n(
            &g_btmControllerLock, &expected, newLock, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
            MutexDelete(newLock);
        }
    }

    if (g_leReadMaximumDataLengthMutex == NULL) {
        Mutex *newLock = MutexCreate();
        if (newLock == NULL) {
            // Do NOT roll back g_btmControllerLock here: another thread may
            // already hold a reference to it after this call's CAS publish,
            // and deleting it would leave that thread locking freed memory
            // (use-after-free). The lock stays valid and is released once by
            // BtmCloseController. A subsequent init attempt will succeed.
            return BT_NO_MEMORY;
        }
        Mutex *expected = NULL;
        if (!__atomic_compare_exchange_n(&g_leReadMaximumDataLengthMutex, &expected, newLock, false,
            __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
            MutexDelete(newLock);
        }
    }

    HCI_RegisterEventCallbacks(&g_hciEventCallbacks);

    g_waitSetupController = EventCreate(true);

    result = BtmInitControllerCommandSequence();

    // Deregister BEFORE deleting g_waitSetupController. The event dispatcher invokes
    // callbacks while holding the same lock as HCI_DeregisterEventCallbacks, so once
    // deregister returns no setup callback can still run. Otherwise a late
    // command-complete (e.g. a response to a command whose EventWait timed out) could
    // call EventSet() on the deleted (or NULL) event and crash the stack.
    HCI_DeregisterEventCallbacks(&g_hciEventCallbacks);

    EventDelete(g_waitSetupController);
    g_waitSetupController = NULL;

    if (result != BT_SUCCESS) {
        // The deregister above already removed the callbacks; the deregister inside
        // BtmCloseController is then a harmless no-op.
        BtmCloseController();
    }

    return result;
}

bool BTM_IsControllerSupportBrEdr()
{
    return HCI_SUPPORT_BREDR(g_readLocalSupportedFeaturesResult.lmpFeatures.raw);
}

bool BTM_IsControllerSupportLe()
{
    return HCI_SUPPORT_LE(g_readLocalSupportedFeaturesResult.lmpFeatures.raw);
}

bool BTM_IsControllerSupportRssiInquiryResponse()
{
    return HCI_SUPPORT_RSSI_INQUIRY_RESPONSE(g_readLocalSupportedFeaturesResult.lmpFeatures.raw);
}

bool BTM_IsControllerSupportEirInquiryResponse()
{
    return HCI_SUPPORT_EIR_INQUIRY_RESPONSE(g_readLocalSupportedFeaturesResult.lmpFeatures.raw);
}

bool BTM_IsControllerSupportRoleSwitch()
{
    return HCI_SUPPORT_ROLE_SWITCH(g_readLocalSupportedFeaturesResult.lmpFeatures.raw);
}

bool BTM_IsControllerSupportHoldMode()
{
    return HCI_SUPPORT_HOLD_MODE(g_readLocalSupportedFeaturesResult.lmpFeatures.raw);
}

bool BTM_IsControllerSupportSniffMode()
{
    return HCI_SUPPORT_SNIFF_MODE(g_readLocalSupportedFeaturesResult.lmpFeatures.raw);
}

bool BTM_IsControllerSupportEsco()
{
    return HCI_SUPPORT_ESCO_EV3(g_readLocalSupportedFeaturesResult.lmpFeatures.raw) ||
           HCI_SUPPORT_ESCO_EV4(g_readLocalSupportedFeaturesResult.lmpFeatures.raw) ||
           HCI_SUPPORT_ESCO_EV5(g_readLocalSupportedFeaturesResult.lmpFeatures.raw);
}

bool BTM_IsControllerSupportSecureSimplePairing()
{
    return HCI_SUPPORT_SECURE_SIMPLE_PAIRING(g_readLocalSupportedFeaturesResult.lmpFeatures.raw);
}

bool BTM_IsControllerSupportSecureConnections()
{
    return HCI_SUPPORT_SECURE_CONNECTIONS(
        g_readLocalExtendedFeaturesResult[EXTENDED_FEATURES_PAGE_2].extendedLMPFeatures);
}

bool BTM_IsControllerSupportNonFlushablePacketBoundaryFlag()
{
    return HCI_SUPPORT_NON_FLUSHABLE_PACKET_BONDARY_FLAG(g_readLocalSupportedFeaturesResult.lmpFeatures.raw);
}

bool BTM_IsControllerSupportLePing()
{
    return HCI_SUPPORT_LE_PING(g_leReadLocalSupportedFeaturesResult.leFeatures.raw);
}

bool BTM_IsControllerSupportLlPrivacy()
{
    return HCI_SUPPORT_LL_PRIVACY(g_leReadLocalSupportedFeaturesResult.leFeatures.raw);
}

bool BTM_IsControllerSupportLe2MPhy()
{
    return HCI_SUPPORT_LE_2M_PHY(g_leReadLocalSupportedFeaturesResult.leFeatures.raw);
}

bool BTM_IsControllerSupportLeCodedPhy()
{
    return HCI_SUPPORT_LE_CODED_PHY(g_leReadLocalSupportedFeaturesResult.leFeatures.raw);
}

bool BTM_IsControllerSupportLeReadPhy()
{
    return HciSupportLeReadPhy(g_readLocalSupportedCommandsResult.supportedCommands);
}

bool BTM_IsControllerSupportLeExtendedAdvertising()
{
    return HCI_SUPPORT_LE_EXTENDED_ADVERTISING(g_leReadLocalSupportedFeaturesResult.leFeatures.raw);
}

bool BTM_IsControllerSupportLeDataPacketLengthExtension()
{
    return HCI_SUPPORT_LE_DATA_PACKET_LENGTH_EXTENSION(g_leReadLocalSupportedFeaturesResult.leFeatures.raw);
}

bool BTM_IsControllerSupportChannelSelectionAlgorithm2()
{
    return HCI_SUPPURT_CHANNEL_SELECTION_ALGORITHM_2(g_leReadLocalSupportedFeaturesResult.leFeatures.raw);
}

bool BTM_IsControllerSupportLeReadTransmitPower()
{
    return HciSupportLeReadTransmitPower(g_readLocalSupportedCommandsResult.supportedCommands);
}

bool BTM_IsControllerSupportLeReadRfPathCompensation()
{
    return HciSupportLeReadRfPathCompensation(g_readLocalSupportedCommandsResult.supportedCommands);
}

bool BTM_IsControllerSupportLeWriteRfPathCompensation()
{
    return HciSupportLeWriteRfPathCompensation(g_readLocalSupportedCommandsResult.supportedCommands);
}

bool BTM_IsControllerSupportConnectionParametersRequestProcedure()
{
    return HCI_SUPPORT_CONNECTION_PARAMETERS_REQUEST_PROCEDURE(g_leReadLocalSupportedFeaturesResult.leFeatures.raw);
}

bool BTM_IsControllerSupportLePeriodicAdvertising()
{
    return HCI_SUPPORT_LE_PERIODIC_ADVERTISING(g_leReadLocalSupportedFeaturesResult.leFeatures.raw);
}

int BTM_GetLeMaxDataLength(uint16_t *maxTxOctets, uint16_t *maxTxTime, uint16_t *maxRxOctets, uint16_t *maxRxTime)
{
    if (maxTxOctets == NULL || maxTxTime == NULL || maxRxOctets == NULL || maxRxTime == NULL) {
        return BT_BAD_PARAM;
    }

    if (g_btmControllerLock == NULL || g_leReadMaximumDataLengthMutex == NULL) {
        // Controller has not been initialized yet; return Bluetooth LE default values.
        *maxTxOctets = LE_DEFAULT_MAX_TX_OCTETS;
        *maxTxTime = LE_DEFAULT_MAX_TX_TIME;
        *maxRxOctets = LE_DEFAULT_MAX_RX_OCTETS;
        *maxRxTime = LE_DEFAULT_MAX_RX_TIME;
        return BT_SUCCESS;
    }

    MutexLock(g_btmControllerLock);
    if (g_leReadMaximumDataLengthMutex == NULL) {
        MutexUnlock(g_btmControllerLock);
        *maxTxOctets = LE_DEFAULT_MAX_TX_OCTETS;
        *maxTxTime = LE_DEFAULT_MAX_TX_TIME;
        *maxRxOctets = LE_DEFAULT_MAX_RX_OCTETS;
        *maxRxTime = LE_DEFAULT_MAX_RX_TIME;
        return BT_SUCCESS;
    }

    MutexLock(g_leReadMaximumDataLengthMutex);
    MutexUnlock(g_btmControllerLock);

    if (!BTM_IsControllerSupportLeDataPacketLengthExtension() || !g_leReadMaximumDataLengthValid) {
        *maxTxOctets = LE_DEFAULT_MAX_TX_OCTETS;
        *maxTxTime = LE_DEFAULT_MAX_TX_TIME;
        *maxRxOctets = LE_DEFAULT_MAX_RX_OCTETS;
        *maxRxTime = LE_DEFAULT_MAX_RX_TIME;
    } else {
        *maxTxOctets = g_leReadMaximumDataLengthResult.supportedMaxTxOctets;
        *maxTxTime = g_leReadMaximumDataLengthResult.supportedMaxTxTime;
        *maxRxOctets = g_leReadMaximumDataLengthResult.supportedMaxRxOctets;
        *maxRxTime = g_leReadMaximumDataLengthResult.supportedMaxRxTime;

        if (*maxTxOctets < LE_DEFAULT_MAX_TX_OCTETS || *maxTxOctets > LE_MAX_TX_OCTETS_MAX ||
            *maxTxTime < LE_DEFAULT_MAX_TX_TIME || *maxTxTime > LE_MAX_TX_TIME_MAX ||
            *maxRxOctets < LE_DEFAULT_MAX_RX_OCTETS || *maxRxOctets > LE_MAX_RX_OCTETS_MAX ||
            *maxRxTime < LE_DEFAULT_MAX_RX_TIME || *maxRxTime > LE_MAX_RX_TIME_MAX) {
            LOG_WARN("%{public}s: controller returned out-of-range data length, using defaults", __FUNCTION__);
            *maxTxOctets = LE_DEFAULT_MAX_TX_OCTETS;
            *maxTxTime = LE_DEFAULT_MAX_TX_TIME;
            *maxRxOctets = LE_DEFAULT_MAX_RX_OCTETS;
            *maxRxTime = LE_DEFAULT_MAX_RX_TIME;
        }
    }
    MutexUnlock(g_leReadMaximumDataLengthMutex);

    return BT_SUCCESS;
}

bool BtmGetLocalSupportedFeature(HciLmpFeatures *lmpFeature)
{
    *lmpFeature = g_readLocalSupportedFeaturesResult.lmpFeatures;
    return true;
}

int BTM_GetLocalAddr(BtAddr *addr)
{
    if (addr == NULL) {
        return BT_BAD_PARAM;
    }

    int result = BT_SUCCESS;
    if (g_readBdAddrResult.status != HCI_SUCCESS) {
        result = BT_BAD_STATUS;
    } else {
        errno_t err = memcpy_s(addr->addr, BT_ADDRESS_SIZE, g_readBdAddrResult.bdAddr.raw, BT_ADDRESS_SIZE);
        if (err == EOK) {
            addr->type = BT_PUBLIC_DEVICE_ADDRESS;
        } else {
            result = BT_NO_MEMORY;
        }
    }
    return result;
}

int BTM_GetLocalSupportedFeatures(uint8_t features[8])
{
    int result = BT_SUCCESS;

    if (g_readLocalSupportedFeaturesResult.status == HCI_SUCCESS) {
        errno_t err = memcpy_s(
            features, LMP_FEATURES_SIZE, g_readLocalSupportedFeaturesResult.lmpFeatures.raw, LMP_FEATURES_SIZE);
        if (err != EOK) {
            result = BT_NO_MEMORY;
        }
    } else {
        result = BT_BAD_STATUS;
    }

    return result;
}

int BTM_GetLocalVersionInformation(BtmLocalVersionInformation *localVersion)
{
    if (localVersion == NULL) {
        return BT_BAD_PARAM;
    }

    if (g_readLocalVersionResult.status != HCI_SUCCESS) {
        return BT_OPERATION_FAILED;
    }

    localVersion->hciVersion = g_readLocalVersionResult.hciVersion;
    localVersion->hciRevision = g_readLocalVersionResult.hciRevision;
    localVersion->lmpVersion = g_readLocalVersionResult.lmpVersion;
    localVersion->manufacturerName = g_readLocalVersionResult.manufacturerName;
    localVersion->lmpSubversion = g_readLocalVersionResult.lmpSubversion;

    return BT_SUCCESS;
}

int BtmGetWhiteListSize(uint8_t *whiteListSize)
{
    if (g_leReadWhiteListSizeResult.status != HCI_SUCCESS) {
        return BT_OPERATION_FAILED;
    }

    *whiteListSize = g_leReadWhiteListSizeResult.whiteListSize;

    return BT_SUCCESS;
}

int BtmGetResolvingListSize(uint8_t *resolvingListSize)
{
    if (g_leReadResolvingListSizeResult.status != HCI_SUCCESS) {
        return BT_OPERATION_FAILED;
    }

    *resolvingListSize = g_leReadResolvingListSizeResult.resolvingListSize;

    return BT_SUCCESS;
}

// Validate the codec state and deep-copy the lists into the scratch buffers so
// the returned pointers cannot be freed concurrently by BtmCloseController.
// The caller must hold g_btmControllerLock. Returns BT_SUCCESS and the copied
// counts via out-params, or BT_OPERATION_FAILED when the state cannot be copied.
static int BtmCopyLocalSupportedCodecs(size_t *standardCount, size_t *vendorCount)
{
    if (g_readLocalSupportedCodecsResult != HCI_SUCCESS) {
        return BT_OPERATION_FAILED;
    }

    BtmLocalSupportedCodecs *copy = &g_localSupportedCodecs;
    size_t stdCount = copy->numberOfSupportedCodecs;
    size_t venCount = copy->numberOfSupportedVendorSpecificCodecs;
    if ((stdCount > 0 && copy->supportedCodecs == NULL) ||
        (venCount > 0 && copy->vendorSpecificCodecs == NULL)) {
        return BT_OPERATION_FAILED;
    }
    if (stdCount > 0 &&
        memcpy_s(g_localSupportedCodecsCopy, sizeof(g_localSupportedCodecsCopy), copy->supportedCodecs,
            stdCount) != EOK) {
        return BT_OPERATION_FAILED;
    }
    if (venCount > 0 &&
        memcpy_s(g_localVendorSpecificCodecsCopy, sizeof(g_localVendorSpecificCodecsCopy),
            copy->vendorSpecificCodecs, venCount * sizeof(BtmVendorSpecificCodec)) != EOK) {
        return BT_OPERATION_FAILED;
    }
    *standardCount = stdCount;
    *vendorCount = venCount;
    return BT_SUCCESS;
}

int BTM_GetLocalSupportedCodecs(BtmLocalSupportedCodecs **localSupportedCodes)
{
    if (localSupportedCodes == NULL) {
        return BT_BAD_PARAM;
    }

    if (g_btmControllerLock != NULL) {
        MutexLock(g_btmControllerLock);
    }

    size_t standardCount = 0;
    size_t vendorCount = 0;
    int ret = BtmCopyLocalSupportedCodecs(&standardCount, &vendorCount);
    if (ret != BT_SUCCESS) {
        if (g_btmControllerLock != NULL) {
            MutexUnlock(g_btmControllerLock);
        }
        return ret;
    }

    // Function-local static: one instance shared by all calls, written only under
    // g_btmControllerLock. The returned pointer stays valid until the next call to
    // BTM_GetLocalSupportedCodecs, which is the documented API contract.
    static BtmLocalSupportedCodecs scratch = {
        .supportedCodecs = g_localSupportedCodecsCopy,
        .vendorSpecificCodecs = g_localVendorSpecificCodecsCopy,
    };
    scratch.numberOfSupportedCodecs = (uint8_t)standardCount;
    scratch.numberOfSupportedVendorSpecificCodecs = (uint8_t)vendorCount;

    *localSupportedCodes = &scratch;

    if (g_btmControllerLock != NULL) {
        MutexUnlock(g_btmControllerLock);
    }

    return BT_SUCCESS;
}

int BTM_GetAclDataPacketLength(uint16_t *aclDataPacketLength)
{
    if (g_readBufferSizeResult.status != HCI_SUCCESS) {
        return BT_OPERATION_FAILED;
    }

    *aclDataPacketLength = g_readBufferSizeResult.hcAclDataPacketLength;

    return BT_SUCCESS;
}

int BTM_GetLeAclDataPacketLength(uint16_t *leAclDataPacketLength)
{
    if (g_leReadBufferSizeResult.status != HCI_SUCCESS) {
        return BT_OPERATION_FAILED;
    }

    if (g_leReadBufferSizeResult.hcLeAclDataPacketLength) {
        *leAclDataPacketLength = g_leReadBufferSizeResult.hcLeAclDataPacketLength;
    } else {
        *leAclDataPacketLength = g_readBufferSizeResult.hcAclDataPacketLength;
    }

    return BT_SUCCESS;
}

void BtmCloseController()
{
    // Invalidate the cached DLE state. The mutexes are intentionally left alive
    // (and their global pointers are not cleared) to avoid use-after-free races
    // with API calls or HCI callbacks that may still reference them during teardown.
    if (g_btmControllerLock != NULL) {
        MutexLock(g_btmControllerLock);
    }

    if (g_localSupportedCodecs.supportedCodecs) {
        MEM_MALLOC.free(g_localSupportedCodecs.supportedCodecs);
        g_localSupportedCodecs.supportedCodecs = NULL;
    }
    if (g_localSupportedCodecs.vendorSpecificCodecs) {
        MEM_MALLOC.free(g_localSupportedCodecs.vendorSpecificCodecs);
        g_localSupportedCodecs.vendorSpecificCodecs = NULL;
    }
    g_localSupportedCodecs.numberOfSupportedCodecs = 0;
    g_localSupportedCodecs.numberOfSupportedVendorSpecificCodecs = 0;
    g_readLocalSupportedCodecsResult = HCI_HARDWARE_FAILURE;

    if (g_leReadMaximumDataLengthMutex != NULL) {
        MutexLock(g_leReadMaximumDataLengthMutex);
        g_leReadMaximumDataLengthPending = false;
        g_leReadMaximumDataLengthValid = false;
        g_leReadMaximumDataLengthResult.status = HCI_HARDWARE_FAILURE;
        MutexUnlock(g_leReadMaximumDataLengthMutex);
    }

    if (g_btmControllerLock != NULL) {
        MutexUnlock(g_btmControllerLock);
    }

    // Prevent any in-flight HCI callback from being dispatched after the state is invalidated.
    // Deregister outside g_btmControllerLock to avoid lock-order inversion with the HCI
    // event dispatcher, which holds the callback-list lock while invoking callbacks that
    // acquire g_btmControllerLock.
    HCI_DeregisterEventCallbacks(&g_hciEventCallbacks);
}
