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

#include "gap_le.h"
#include "gap_internal.h"

#include <securec.h>

#include "allocator.h"
#include "log.h"

#include "btm.h"
#include "hci/hci.h"
#include "hci/hci_error.h"

typedef struct {
    GapAdvCallback callback;
    void *context;
} LeAdvCallback;

typedef struct {
    GapExAdvCallback callback;
    void *context;
} LeExAdvCallback;

static LeAdvCallback g_leAdvCallback;
static LeExAdvCallback g_leExAdvCallback;

void GapLeAdvCallbackInit(void)
{
    (void)memset_s(&g_leAdvCallback, sizeof(g_leAdvCallback), 0x00, sizeof(g_leAdvCallback));
    (void)memset_s(&g_leExAdvCallback, sizeof(g_leExAdvCallback), 0x00, sizeof(g_leExAdvCallback));
}

void GapLeAdvCallbackDeinit(void)
{
    (void)memset_s(&g_leAdvCallback, sizeof(g_leAdvCallback), 0x00, sizeof(g_leAdvCallback));
    (void)memset_s(&g_leExAdvCallback, sizeof(g_leExAdvCallback), 0x00, sizeof(g_leExAdvCallback));
}

static bool GapFindExAdvInfoByAdvHandle(void *nodeData, void *param)
{
    uint8_t advHandle = *(uint8_t *)param;

    return ((LeExAdvInfo *)nodeData)->advHandle == advHandle;
}

void GapLeReadMaximumAdvertisingDataLengthComplete(const HciLeReadMaximumAdvertisingDataLengthReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: complete param is NULL", __FUNCTION__);
        return;
    }

    LeExAdvBlock *exAdvBlock = GapGetLeExAdvBlock();
    if (param->status == HCI_SUCCESS) {
        exAdvBlock->exAdvDataMaxLen = param->maximumAdvertisingDataLength;
    } else {
        // Keep the previous value on failure instead of resetting to 0. A reset
        // would reject every non-empty SetData call (no retry of the read is
        // issued) and lock the extended advertising data feature. 0 means "not
        // read yet" and skips the upper-bound check in GapLeSetExtendedAdvertisingData.
        LOG_WARN("%{public}s: read failed status:0x%02x, keeping previous max length %hu",
            __FUNCTION__, param->status, exAdvBlock->exAdvDataMaxLen);
    }
}

void GapLeReadNumberofSupportedAdvertisingSetsComplete(
    const HciLeReadNumberofSupportedAdvertisingSetsReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: complete param is NULL", __FUNCTION__);
        return;
    }

    LeExAdvBlock *exAdvBlock = GapGetLeExAdvBlock();
    if (param->status == HCI_SUCCESS) {
        exAdvBlock->exAdvMaxNumber = param->numSupportedAdvertisingSets;
    } else {
        exAdvBlock->exAdvMaxNumber = 0;
    }
}

int GAP_LeExAdvGetMaxDataLen(uint16_t *len)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    if (len == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    LeExAdvBlock *exAdvBlock = GapGetLeExAdvBlock();
    *len = exAdvBlock->exAdvDataMaxLen;
    return GAP_SUCCESS;
}

int GAP_LeExAdvGetMaxHandleNum(uint8_t *num)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    if (num == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    LeExAdvBlock *exAdvBlock = GapGetLeExAdvBlock();
    *num = exAdvBlock->exAdvMaxNumber;
    return GAP_SUCCESS;
}

int GAP_RegisterExAdvCallback(const GapExAdvCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "NULL");
    if (callback == NULL) {
        (void)memset_s(
            &g_leExAdvCallback.callback, sizeof(g_leExAdvCallback.callback), 0x00, sizeof(g_leExAdvCallback.callback));
    } else {
        g_leExAdvCallback.callback = *callback;
    }
    g_leExAdvCallback.context = context;
    return GAP_SUCCESS;
}

int GAP_DeregisterExAdvCallback(void)
{
    (void)memset_s(
        &g_leExAdvCallback.callback, sizeof(g_leExAdvCallback.callback), 0x00, sizeof(g_leExAdvCallback.callback));
    g_leExAdvCallback.context = NULL;
    return GAP_SUCCESS;
}

static int GapLeSetAdvertisingSetRandomAddress(uint8_t advHandle, const uint8_t addr[BT_ADDRESS_SIZE])
{
    int ret;
    HciLeSetAdvertisingSetRandomAddressParam hciCmdParam;

    if (advHandle > GAP_LE_ADV_HANDLE_MAX || addr == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    LeExAdvBlock *exAdvBlock = GapGetLeExAdvBlock();

    // Hold the lock across the HCI call to guarantee that the node we are
    // updating cannot be freed or replaced by another thread between lookup
    // and the synchronous command completion. This avoids the use-after-free
    // race that would occur if we released the lock around HCI and kept a
    // stale LeExAdvInfo pointer.
    MutexLock(exAdvBlock->lock);
    bool newAdded = false;
    LeExAdvInfo *info = ListForEachData(exAdvBlock->exAdvInfoList, GapFindExAdvInfoByAdvHandle, &advHandle);
    if (info == NULL) {
        info = MEM_MALLOC.alloc(sizeof(LeExAdvInfo));
        if (info == NULL) {
            MutexUnlock(exAdvBlock->lock);
            return GAP_ERR_OUT_OF_RES;
        }
        (void)memset_s(info, sizeof(LeExAdvInfo), 0x00, sizeof(LeExAdvInfo));
        info->advHandle = advHandle;
        ListAddLast(exAdvBlock->exAdvInfoList, info);
        LeExAdvInfo *found = ListForEachData(exAdvBlock->exAdvInfoList, GapFindExAdvInfoByAdvHandle, &advHandle);
        if (found == NULL) {
            // Unlink instead of freeing directly: if the look-up failure is
            // spurious and the node actually made it into the list, a plain
            // free would leave a dangling node for later traversals.
            // ListRemoveNode releases the node via the list's free callback.
            ListRemoveNode(exAdvBlock->exAdvInfoList, info);
            MutexUnlock(exAdvBlock->lock);
            return GAP_ERR_OUT_OF_RES;
        }
        newAdded = true;
    }

    hciCmdParam.advertisingHandle = advHandle;
    (void)memcpy_s(hciCmdParam.randomAddress, BT_ADDRESS_SIZE, addr, BT_ADDRESS_SIZE);

    ret = HCI_LeSetAdvertisingSetRandomAddress(&hciCmdParam);
    if (ret == BT_SUCCESS) {
        (void)memcpy_s(info->randomAddress, BT_ADDRESS_SIZE, addr, BT_ADDRESS_SIZE);
    } else if (newAdded) {
        // Optimistically added node but HCI failed; remove it. GapFreeListNode
        // registered at ListCreate time will free info.
        ListRemoveNode(exAdvBlock->exAdvInfoList, info);
    }
    MutexUnlock(exAdvBlock->lock);

    return ret;
}

NO_SANITIZE("cfi")
void GapLeSetAdvertisingSetRandomAddressComplete(const HciLeSetAdvertisingSetRandomAddressReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    if (g_leExAdvCallback.callback.exAdvSetRandAddrResult) {
        g_leExAdvCallback.callback.exAdvSetRandAddrResult(param->status, g_leExAdvCallback.context);
    }
}

int GAP_LeExAdvSetRandAddr(uint8_t advHandle, const uint8_t addr[BT_ADDRESS_SIZE])
{
    int ret;
    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (advHandle > GAP_LE_ADV_HANDLE_MAX || addr == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    LOG_INFO("%{public}s:" BT_ADDR_FMT, __FUNCTION__, BT_ADDR_FMT_OUTPUT(addr));

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeSetAdvertisingSetRandomAddress(advHandle, addr);
    }
    return ret;
}

#define GAP_LE_EXT_ADV_PROP_DIRECTED        0x04
#define GAP_LE_EXT_ADV_PROP_HIGH_DUTY_DIR   0x08

#define GAP_LE_EXT_ADV_CHANNEL_MAP_MIN      0x01
#define GAP_LE_EXT_ADV_CHANNEL_MAP_MAX      0x07
#define GAP_LE_EXT_ADV_FILTER_POLICY_MAX    0x03
#define GAP_LE_EXT_ADV_MAX_SKIP_MAX         0xFF
#define GAP_LE_EXT_ADV_SCAN_REQ_NOTIFY_MAX  0x01
#define GAP_LE_EXT_ADV_DATA_OP_MAX          0x04
#define GAP_LE_EXT_ADV_FRAGMENT_PREF_MAX    0x01
#define GAP_LE_EXT_ADV_ENABLE_MAX           0x01

static int GapLeSetExtendedAdvertisingParameters(
    uint8_t advHandle, uint8_t properties, int8_t txPower, GapLeExAdvParam advExParam)
{
    if (advHandle > GAP_LE_ADV_HANDLE_MAX) {
        return GAP_ERR_INVAL_PARAM;
    }

    if ((properties & (GAP_LE_EXT_ADV_PROP_DIRECTED | GAP_LE_EXT_ADV_PROP_HIGH_DUTY_DIR)) &&
        advExParam.peerAddr == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (advExParam.advIntervalMin > advExParam.advIntervalMax ||
        advExParam.advChannelMap < GAP_LE_EXT_ADV_CHANNEL_MAP_MIN ||
        advExParam.advChannelMap > GAP_LE_EXT_ADV_CHANNEL_MAP_MAX ||
        (advExParam.primaryAdvPhy != GAP_LE_PHY_1M && advExParam.primaryAdvPhy != GAP_LE_PHY_CODED) ||
        (advExParam.secondaryAdvPhy != GAP_LE_PHY_1M && advExParam.secondaryAdvPhy != GAP_LE_PHY_2M &&
            advExParam.secondaryAdvPhy != GAP_LE_PHY_CODED) ||
        advExParam.advSid > GAP_LE_ADV_SID_MAX ||
        advExParam.advFilterPolicy > GAP_LE_EXT_ADV_FILTER_POLICY_MAX ||
        advExParam.secondaryAdvMaxSkip > GAP_LE_EXT_ADV_MAX_SKIP_MAX ||
        advExParam.scanRequestNotifyEnable > GAP_LE_EXT_ADV_SCAN_REQ_NOTIFY_MAX) {
        return GAP_ERR_INVAL_PARAM;
    }

    HciLeSetExtendedAdvertisingParametersParam hciCmdParam;
    hciCmdParam.advertisingHandle = advHandle;
    hciCmdParam.advertisingEventProperties = properties;
    (void)memcpy_s(hciCmdParam.priAdvertisingIntervalMin,
        sizeof(hciCmdParam.priAdvertisingIntervalMin),
        &advExParam.advIntervalMin,
        sizeof(hciCmdParam.priAdvertisingIntervalMin));
    (void)memcpy_s(hciCmdParam.priAdvertisingIntervalMax,
        sizeof(hciCmdParam.priAdvertisingIntervalMax),
        &advExParam.advIntervalMax,
        sizeof(hciCmdParam.priAdvertisingIntervalMax));
    hciCmdParam.priAdvertisingChannelMap = advExParam.advChannelMap;
    hciCmdParam.ownAddressType = BTM_GetOwnAddressType();
    if (advExParam.peerAddr != NULL) {
        hciCmdParam.peerAddressType = advExParam.peerAddr->type;
        (void)memcpy_s(hciCmdParam.peerAddress, BT_ADDRESS_SIZE, advExParam.peerAddr->addr, BT_ADDRESS_SIZE);
    } else {
        hciCmdParam.peerAddressType = BT_PUBLIC_DEVICE_ADDRESS;
        (void)memset_s(hciCmdParam.peerAddress, BT_ADDRESS_SIZE, 0x00, BT_ADDRESS_SIZE);
    }
    hciCmdParam.advertisingFilterPolicy = advExParam.advFilterPolicy;
    hciCmdParam.advertisingTxPower = txPower;
    hciCmdParam.priAdvertisingPHY = advExParam.primaryAdvPhy;
    hciCmdParam.secondaryAdvertisingMaxSkip = advExParam.secondaryAdvMaxSkip;
    hciCmdParam.secondaryAdvertisingPHY = advExParam.secondaryAdvPhy;
    hciCmdParam.advertisingSID = advExParam.advSid;
    hciCmdParam.scanRequestNotificationEnable = advExParam.scanRequestNotifyEnable;
    return HCI_LeSetExtendedAdvertisingParameters(&hciCmdParam);
}

NO_SANITIZE("cfi")
void GapLeSetExtendedAdvertisingParametersComplete(const HciLeSetExtendedAdvertisingParametersReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    if (g_leExAdvCallback.callback.exAdvSetParamResult) {
        g_leExAdvCallback.callback.exAdvSetParamResult(
            param->status, param->selectedTxPower, g_leExAdvCallback.context);
    }
}

int GAP_LeExAdvSetParam(uint8_t advHandle, uint8_t properties, int8_t txPower, GapLeExAdvParam advExParam)
{
    int ret;
    LOG_INFO("%{public}s:", __FUNCTION__);
    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeSetExtendedAdvertisingParameters(advHandle, properties, txPower, advExParam);
    }
    return ret;
}

static int GapLeSetExtendedAdvertisingData(
    uint8_t advHandle, uint8_t operation, uint8_t fragmentPreference, uint8_t advDataLength, const uint8_t *advData)
{
    // Skip the upper-bound check while exAdvDataMaxLen is not ready (0 at init or
    // after a failed read): the HCI layer still rejects lengths above the spec
    // maximum and the controller validates its own limit, so rejecting here would
    // only lock the feature when the capability read is late or failed.
    LeExAdvBlock *exAdvBlock = GapGetLeExAdvBlock();
    if (advHandle > GAP_LE_ADV_HANDLE_MAX || operation > GAP_LE_EXT_ADV_DATA_OP_MAX ||
        fragmentPreference > GAP_LE_EXT_ADV_FRAGMENT_PREF_MAX ||
        (exAdvBlock->exAdvDataMaxLen != 0 && advDataLength > exAdvBlock->exAdvDataMaxLen) ||
        (advDataLength > 0 && advData == NULL) ||
        (operation == GAP_LE_EXT_ADV_DATA_OP_MAX && advDataLength != 0)) {
        return GAP_ERR_INVAL_PARAM;
    }

    HciLeSetExtendedAdvertisingDataParam hciCmdParam;
    hciCmdParam.advertisingHandle = advHandle;
    hciCmdParam.fragmentPreference = fragmentPreference;
    hciCmdParam.operation = operation;
    hciCmdParam.advertisingDataLength = advDataLength;
    hciCmdParam.advertisingData = advData;

    return HCI_LeSetExtendedAdvertisingData(&hciCmdParam);
}

NO_SANITIZE("cfi") void GapLeSetExtendedAdvertisingDataComplete(const HciLeSetExtendedAdvertisingDataReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    if (g_leExAdvCallback.callback.exAdvSetDataResult) {
        g_leExAdvCallback.callback.exAdvSetDataResult(param->status, g_leExAdvCallback.context);
    }
}

int GAP_LeExAdvSetData(
    uint8_t advHandle, uint8_t operation, uint8_t fragmentPreference, uint8_t advDataLength, const uint8_t *advData)
{
    int ret;
    LOG_INFO("%{public}s:", __FUNCTION__);
    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeSetExtendedAdvertisingData(advHandle, operation, fragmentPreference, advDataLength, advData);
    }
    return ret;
}

static int GapLeSetExtendedScanResponseData(uint8_t advHandle, uint8_t operation, uint8_t fragmentPreference,
    uint8_t scanResponseDataLength, const uint8_t *scanResponseData)
{
    // Same "not ready" policy as GapLeSetExtendedAdvertisingData: skip the bound
    // check while exAdvDataMaxLen is 0 (init / failed read).
    LeExAdvBlock *exAdvBlock = GapGetLeExAdvBlock();
    if (advHandle > GAP_LE_ADV_HANDLE_MAX || operation > GAP_LE_EXT_ADV_DATA_OP_MAX ||
        fragmentPreference > GAP_LE_EXT_ADV_FRAGMENT_PREF_MAX ||
        (exAdvBlock->exAdvDataMaxLen != 0 && scanResponseDataLength > exAdvBlock->exAdvDataMaxLen) ||
        (scanResponseDataLength > 0 && scanResponseData == NULL) ||
        (operation == GAP_LE_EXT_ADV_DATA_OP_MAX && scanResponseDataLength != 0)) {
        return GAP_ERR_INVAL_PARAM;
    }

    HciLeSetExtendedScanResponseDataParam hciCmdParam;
    hciCmdParam.advertisingHandle = advHandle;
    hciCmdParam.fragmentPreference = fragmentPreference;
    hciCmdParam.operation = operation;
    hciCmdParam.scanResponseDataLength = scanResponseDataLength;
    hciCmdParam.scanResponseData = scanResponseData;

    return HCI_LeSetExtendedScanResponseData(&hciCmdParam);
}

NO_SANITIZE("cfi")
void GapLeSetExtendedScanResponseDataComplete(const HciLeSetExtendedScanResponseDataReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    if (g_leExAdvCallback.callback.exAdvSetScanRspDataResult) {
        g_leExAdvCallback.callback.exAdvSetScanRspDataResult(param->status, g_leExAdvCallback.context);
    }
}

int GAP_LeExAdvSetScanRspData(uint8_t advHandle, uint8_t operation, uint8_t fragmentPreference,
    uint8_t scanResponseDataLen, const uint8_t *scanResponseData)
{
    int ret;
    LOG_INFO("%{public}s:", __FUNCTION__);
    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeSetExtendedScanResponseData(
            advHandle, operation, fragmentPreference, scanResponseDataLen, scanResponseData);
    }
    return ret;
}

static int GapLeSetExtendedAdvertisingEnable(uint8_t enable, uint8_t numberOfSet, const GapExAdvSet *advSet)
{
    int ret;
    HciLeSetExtendedAdvertisingEnableParam hciCmdParam;

    // Core Spec 5.0, Vol 2, Part E 7.8.56: with Enable=0x01, Number_of_Sets must
    // be >= 1. The service layer always passes Number_of_Sets=1 even when
    // disabling (Enable=0x00), where the spec says it must be 0; rk3568
    // controllers accept the combination and disable only the listed set.
    // Rejecting it here made every stopAdvertising fail, leaking the
    // advertising handle pool, so keep the disable-with-sets passthrough.
    if (enable > GAP_LE_EXT_ADV_ENABLE_MAX || (enable != 0x00 && numberOfSet == 0)) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (numberOfSet > 0 && advSet == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    hciCmdParam.enable = enable;
    hciCmdParam.numberofSets = numberOfSet;
    if (numberOfSet == 0) {
        hciCmdParam.sets = NULL;
        return HCI_LeSetExtendedAdvertisingEnable(&hciCmdParam);
    }

    hciCmdParam.sets = MEM_MALLOC.alloc(numberOfSet * sizeof(HciLeExtendedAdvertisingParamSet));
    if (hciCmdParam.sets == NULL) {
        return GAP_ERR_OUT_OF_RES;
    }

    for (int i = 0; i < numberOfSet; i++) {
        if (advSet[i].advHandle > GAP_LE_ADV_HANDLE_MAX) {
            MEM_MALLOC.free(hciCmdParam.sets);
            return GAP_ERR_INVAL_PARAM;
        }
        hciCmdParam.sets[i].adverHandle = advSet[i].advHandle;
        hciCmdParam.sets[i].duration = advSet[i].duration;
        hciCmdParam.sets[i].maxExtendAdvertisingEvents = advSet[i].maxExAdvEvt;
    }

    ret = HCI_LeSetExtendedAdvertisingEnable(&hciCmdParam);
    MEM_MALLOC.free(hciCmdParam.sets);

    return ret;
}

NO_SANITIZE("cfi")
void GapLeSetExtendedAdvertisingEnableComplete(const HciLeSetExtendedAdvertisingEnableReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    if (g_leExAdvCallback.callback.exAdvSetEnableResult) {
        g_leExAdvCallback.callback.exAdvSetEnableResult(param->status, g_leExAdvCallback.context);
    }
}

int GAP_LeExAdvSetEnable(uint8_t enable, uint8_t numberOfSet, const GapExAdvSet *advSet)
{
    int ret;
    LOG_INFO("%{public}s:", __FUNCTION__);
    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeSetExtendedAdvertisingEnable(enable, numberOfSet, advSet);
    }
    return ret;
}

void GapOnLeScanRequestReceivedEvent(const HciLeScanRequestReceivedEventParam *eventParam)
{
    if (g_leExAdvCallback.callback.exAdvScanRequestReceived) {
        BtAddr addr;
        GapChangeHCIAddr(&addr, &eventParam->scannerAddress, eventParam->scannerAddressType);
        g_leExAdvCallback.callback.exAdvScanRequestReceived(
            eventParam->advertisingHandle, &addr, g_leExAdvCallback.context);
    }
}

void GapLeRemoveAdvertisingSetComplete(const HciLeRemoveAdvertisingSetReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    if (g_leExAdvCallback.callback.exAdvRemoveHandleResult) {
        g_leExAdvCallback.callback.exAdvRemoveHandleResult(param->status, g_leExAdvCallback.context);
    }
}

static int GapLeClearAdvertisingSets(void)
{
    // The host-side list is cleared in GapLeClearAdvertisingSetsComplete when
    // the controller confirms the operation succeeded. Do not clear it here,
    // because the synchronous success does not guarantee the command-complete
    // event will also report success.
    return HCI_LeClearAdvertisingSets();
}

NO_SANITIZE("cfi") void GapLeClearAdvertisingSetsComplete(const HciLeClearAdvertisingSetsReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    if (param->status == HCI_SUCCESS) {
        LeExAdvBlock *exAdvBlock = GapGetLeExAdvBlock();
        MutexLock(exAdvBlock->lock);
        // ListClear frees each LeExAdvInfo node via the list's free callback.
        ListClear(exAdvBlock->exAdvInfoList);
        MutexUnlock(exAdvBlock->lock);
    }

    if (g_leExAdvCallback.callback.exAdvClearHandleResult) {
        g_leExAdvCallback.callback.exAdvClearHandleResult(param->status, g_leExAdvCallback.context);
    }
}

int GAP_LeExAdvClearHandle(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeClearAdvertisingSets();
    }
    return ret;
}

static int GapLeRemoveAdvertisingSet(uint8_t advHandle)
{
    HciLeRemoveAdvertisingSetParam hciCmdParam;
    hciCmdParam.advertisingHandle = advHandle;
    return HCI_LeRemoveAdvertisingSet(&hciCmdParam);
}

int GAP_LeExAdvRemoveSet(uint8_t advHandle)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }
    
    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PREIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeRemoveAdvertisingSet(advHandle);
    }
    return ret;
}

void GapOnLeAdvertisingSetTerminated(const HciLeAdvertisingSetTerminatedEventParam *eventParam)
{
    if (eventParam->status == HCI_SUCCESS) {
        LeDeviceInfo *deviceInfo = ListForEachData(GapGetLeConnectionInfoBlock()->deviceList,
            GapFindLeConnectionDeviceByHandle,
            (void *)&eventParam->connectionHandle);
        if (deviceInfo != NULL && deviceInfo->ownAddr.type == BT_RANDOM_DEVICE_ADDRESS) {
            LeExAdvBlock *exAdvBlock = GapGetLeExAdvBlock();
            MutexLock(exAdvBlock->lock);
            LeExAdvInfo *info = ListForEachData(exAdvBlock->exAdvInfoList,
                GapFindExAdvInfoByAdvHandle,
                (void *)&eventParam->advertisingHandle);
            if (info != NULL) {
                LOG_INFO("%{public}s: change own address " BT_ADDR_FMT " -> " BT_ADDR_FMT,
                    __FUNCTION__,
                    BT_ADDR_FMT_OUTPUT(deviceInfo->ownAddr.addr),
                    BT_ADDR_FMT_OUTPUT(info->randomAddress));
                (void)memcpy_s(deviceInfo->ownAddr.addr, BT_ADDRESS_SIZE, info->randomAddress, BT_ADDRESS_SIZE);
            }
            MutexUnlock(exAdvBlock->lock);
        }
        if (deviceInfo != NULL && GapLeDeviceNeedBond(deviceInfo)) {
            GapLeDoPair(&deviceInfo->addr);
        }
        if (eventParam->status == HCI_SUCCESS) {
            if (deviceInfo != NULL) {
                deviceInfo->ownAddrUpdated = true;
            }
            if (deviceInfo != NULL && deviceInfo->securityReq != NULL) {
                GapLeRequestSecurityProcess(deviceInfo);
            }
        }
    }

    if (g_leExAdvCallback.callback.exAdvTerminatedAdvSet) {
        g_leExAdvCallback.callback.exAdvTerminatedAdvSet(eventParam->status,
            eventParam->advertisingHandle,
            eventParam->connectionHandle,
            eventParam->numCompletedExtendedAdvertisingEvents,
            g_leExAdvCallback.context);
    }
}

int GAP_RegisterAdvCallback(const GapAdvCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "NULL");
    if (callback == NULL) {
        (void)memset_s(
            &g_leAdvCallback.callback, sizeof(g_leAdvCallback.callback), 0x00, sizeof(g_leAdvCallback.callback));
    } else {
        g_leAdvCallback.callback = *callback;
    }
    g_leAdvCallback.context = context;
    return GAP_SUCCESS;
}

int GAP_DeregisterAdvCallback(void)
{
    (void)memset_s(&g_leAdvCallback.callback, sizeof(g_leAdvCallback.callback), 0x00, sizeof(g_leAdvCallback.callback));
    g_leAdvCallback.context = NULL;
    return GAP_SUCCESS;
}

int GAP_LeAdvSetParam(uint8_t advType, GapLeAdvParam advParam)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else if (GapLeRolesCheck(GAP_LE_ROLE_PERIPHERAL) == false && advType != GAP_ADV_TYPE_SCAN_UNDIR &&
               advType != GAP_ADV_TYPE_NON_CONN_UNDIR) {
        ret = GAP_ERR_INVAL_PARAM;
    } else if ((advType == GAP_ADV_TYPE_CONN_DIR_HIGH_DUTY || advType == GAP_ADV_TYPE_CONN_DIR_LOW_DUTY) &&
               advParam.peerAddr == NULL) {
        ret = GAP_ERR_INVAL_PARAM;
    } else {
        HciLeSetAdvertisingParametersParam hciCmdParam;

        hciCmdParam.advertisingType = advType;
        hciCmdParam.advertisingIntervalMin = advParam.advIntervalMin;
        hciCmdParam.advertisingIntervalMax = advParam.advIntervalMax;
        hciCmdParam.ownAddressType = BTM_GetOwnAddressType();
        if (advParam.peerAddr != NULL) {
            hciCmdParam.peerAddressType = advParam.peerAddr->type;
            (void)memcpy_s(hciCmdParam.peerAddress.raw, BT_ADDRESS_SIZE, advParam.peerAddr->addr, BT_ADDRESS_SIZE);
        } else {
            hciCmdParam.peerAddressType = BT_PUBLIC_DEVICE_ADDRESS;
            (void)memset_s(hciCmdParam.peerAddress.raw, BT_ADDRESS_SIZE, 0x00, BT_ADDRESS_SIZE);
        }
        hciCmdParam.advertisingChannelMap = advParam.advChannelMap;
        hciCmdParam.advertisingFilterPolicy = advParam.advFilterPolicy;

        ret = HCI_LeSetAdvertisingParameters(&hciCmdParam);
        if (ret != BT_SUCCESS) {
            LOG_ERROR("HCI Command Error ret = %{public}d.", ret);
        }
    }

    return ret;
}

void GapLeAdvSetParamComplete(const HciLeSetAdvertisingParametersReturnParam *param)
{
    if (g_leAdvCallback.callback.advSetParamResult) {
        g_leAdvCallback.callback.advSetParamResult(param->status, g_leAdvCallback.context);
    }
}

int GAP_LeAdvReadTxPower(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = HCI_LeReadAdvertisingChannelTxPower();
    }

    return ret;
}

void GapLeAdvReadTxPowerComplete(const HciLeReadAdvertisingChannelTxPowerReturnParam *param)
{
    if (g_leAdvCallback.callback.advReadTxPower) {
        g_leAdvCallback.callback.advReadTxPower(param->status, param->transmitPowerLevel, g_leAdvCallback.context);
    }
}

int GAP_LeAdvSetData(uint8_t advDataLength, const uint8_t *advData)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret;

    if (advDataLength > GAP_ADVERTISING_DATA_LENGTH_MAX ||
        (advDataLength > 0 && advData == NULL)) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        HciLeSetAdvertisingDataParam hciCmdParam = {
            .advertisingDataLen = advDataLength,
        };
        if (advDataLength > 0) {
            (void)memcpy_s(hciCmdParam.advertisingData, sizeof(hciCmdParam.advertisingData), advData, advDataLength);
        }

        ret = HCI_LeSetAdvertisingData(&hciCmdParam);
    }

    return ret;
}

void GapLeAdvSetDataComplete(const HciLeSetAdvertisingDataReturnParam *param)
{
    if (g_leAdvCallback.callback.advSetDataResult) {
        g_leAdvCallback.callback.advSetDataResult(param->status, g_leAdvCallback.context);
    }
}

int GAP_LeAdvSetScanRspData(uint8_t advDataLength, const uint8_t *advData)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (advDataLength > GAP_ADVERTISING_DATA_LENGTH_MAX ||
        (advDataLength > 0 && advData == NULL)) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        HciLeSetScanResponseDataParam hciCmdParam = {
            .scanResponseDataLength = advDataLength,
        };
        if (advDataLength > 0) {
            (void)memcpy_s(hciCmdParam.scanResponseData, sizeof(hciCmdParam.scanResponseData), advData, advDataLength);
        }

        ret = HCI_LeSetScanResponseData(&hciCmdParam);
    }

    return ret;
}

void GapLeAdvSetScanRspDataComplete(const HciLeSetScanResponseDataReturnParam *param)
{
    if (g_leAdvCallback.callback.advSetScanRspDataResult) {
        g_leAdvCallback.callback.advSetScanRspDataResult(param->status, g_leAdvCallback.context);
    }
}

int GAP_LeAdvSetEnable(uint8_t enable)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        HciLeSetAdvertisingEnableParam hciCmdParam = {
            .advertisingEnable = enable,
        };

        ret = HCI_LeSetAdvertisingEnable(&hciCmdParam);
    }

    return ret;
}

void GapLeAdvSetEnableComplete(const HciLeSetAdvertisingEnableReturnParam *param)
{
    if (g_leAdvCallback.callback.advSetEnableResult) {
        g_leAdvCallback.callback.advSetEnableResult(param->status, g_leAdvCallback.context);
    }
}

static int GapLeSetPeriodicAdvertisingParameters(
    uint8_t advHandle, uint16_t intervalMin, uint16_t intervalMax, uint16_t properties)
{
    HciLeSetPeriodicAdvertisingParametersParam hciCmdParam = {
        .advertisingHandle = advHandle,
        .periodicAdvertisingIntervalMin = intervalMin,
        .periodicAdvertisingIntervalMax = intervalMax,
        .periodicAdvertisingProperties = properties,
    };

    return HCI_LeSetPeriodicAdvertisingParameters(&hciCmdParam);
}

int GAP_LePeriodicAdvSetParam(uint8_t advHandle, uint16_t intervalMin, uint16_t intervalMax, uint16_t properties)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (advHandle > GAP_PERIODIC_ADV_HANDLE_MAX ||
        intervalMin < GAP_PERIODIC_ADV_INTERVAL_MIN ||
        intervalMin > GAP_PERIODIC_ADV_INTERVAL_MAX ||
        intervalMax < GAP_PERIODIC_ADV_INTERVAL_MIN ||
        intervalMax > GAP_PERIODIC_ADV_INTERVAL_MAX ||
        intervalMin > intervalMax ||
        (properties & ~GAP_PERIODIC_ADV_PROPERTIES_MASK) != 0) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeSetPeriodicAdvertisingParameters(advHandle, intervalMin, intervalMax, properties);
    }

    return ret;
}

NO_SANITIZE("cfi")
void GapLeSetPeriodicAdvertisingParametersComplete(const HciLeSetPeriodicAdvertisingParametersReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    if (g_leExAdvCallback.callback.periodicAdvSetParamResult) {
        g_leExAdvCallback.callback.periodicAdvSetParamResult(param->status, g_leExAdvCallback.context);
    }
}

static int GapLeSetPeriodicAdvertisingData(uint8_t advHandle, uint8_t operation, uint8_t advDataLength,
    const uint8_t *advData)
{
    HciLeSetPeriodicAdvertisingDataHostParam hciCmdParam = {
        .advertisingHandle = advHandle,
        .operation = operation,
        .advertisingDataLength = advDataLength,
        .advertisingData = advData,
    };

    return HCI_LeSetPeriodicAdvertisingData(&hciCmdParam);
}

int GAP_LePeriodicAdvSetData(uint8_t advHandle, uint8_t operation, uint8_t advDataLength, const uint8_t *advData)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (advHandle > GAP_PERIODIC_ADV_HANDLE_MAX ||
        operation > GAP_PERIODIC_ADV_OPERATION_MAX ||
        advDataLength > GAP_PERIODIC_ADV_DATA_LENGTH_MAX ||
        (advDataLength > 0 && advData == NULL) ||
        (operation == GAP_PERIODIC_ADV_DATA_OPERATION_UNCHANGED && advDataLength != 0)) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeSetPeriodicAdvertisingData(advHandle, operation, advDataLength, advData);
    }

    return ret;
}

NO_SANITIZE("cfi")
void GapLeSetPeriodicAdvertisingDataComplete(const HciLeSetPeriodicAdvertisingDataReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    if (g_leExAdvCallback.callback.periodicAdvSetDataResult) {
        g_leExAdvCallback.callback.periodicAdvSetDataResult(param->status, g_leExAdvCallback.context);
    }
}

static int GapLeSetPeriodicAdvertisingEnable(uint8_t enable, uint8_t advHandle)
{
    HciLeSetPeriodicAdvertisingEnableParam hciCmdParam = {
        .enable = enable,
        .advertisingHandle = advHandle,
    };

    return HCI_LeSetPeriodicAdvertisingEnable(&hciCmdParam);
}

int GAP_LePeriodicAdvSetEnable(uint8_t enable, uint8_t advHandle)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (enable > GAP_PERIODIC_ADV_ENABLE_TRUE || advHandle > GAP_PERIODIC_ADV_HANDLE_MAX) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeSetPeriodicAdvertisingEnable(enable, advHandle);
    }

    return ret;
}

NO_SANITIZE("cfi")
void GapLeSetPeriodicAdvertisingEnableComplete(const HciLeSetPeriodicAdvertisingEnableReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    if (g_leExAdvCallback.callback.periodicAdvSetEnableResult) {
        g_leExAdvCallback.callback.periodicAdvSetEnableResult(param->status, g_leExAdvCallback.context);
    }
}
