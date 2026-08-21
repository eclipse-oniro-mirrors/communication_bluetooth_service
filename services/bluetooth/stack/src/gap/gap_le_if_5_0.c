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

#include "gap_internal.h"
#include "gap_def.h"
#include "gap_le_if.h"
#include "gap_task_internal.h"

#include <securec.h>

#include "allocator.h"
#include "log.h"

#ifdef GAP_LE_SUPPORT

typedef struct {
    int result;
    BtAddr addr;
    uint8_t allPhys;
    uint8_t txPhys;
    uint8_t rxPhys;
    uint16_t phyOptions;
} GapLeSetPhyInfo;

typedef struct {
    int result;
    BtAddr addr;
} GapLeAddrInfo;

typedef struct {
    int result;
    uint8_t allPhys;
    uint8_t txPhys;
    uint8_t rxPhys;
} GapLeSetDefaultPhyInfo;

typedef struct {
    int result;
    BtAddr addr;
    uint16_t txOctets;
    uint16_t txTime;
} GapLeSetDataLengthInfo;

typedef struct {
    int result;
    uint8_t advHandle;
    uint16_t intervalMin;
    uint16_t intervalMax;
    uint16_t properties;
} GapLePeriodicAdvSetParamInfo;

typedef struct {
    int result;
    uint8_t advHandle;
    uint8_t operation;
    uint8_t advDataLength;
    uint8_t advData[GAP_PERIODIC_ADV_DATA_LENGTH_MAX];
} GapLePeriodicAdvSetDataInfo;

typedef struct {
    int result;
    uint8_t enable;
    uint8_t advHandle;
} GapLePeriodicAdvSetEnableInfo;

typedef struct {
    int result;
    uint8_t filterPolicy;
    uint8_t advSid;
    BtAddr advAddr;
    uint16_t skip;
    uint16_t syncTimeout;
} GapLePeriodicAdvCreateSyncInfo;

typedef struct {
    int result;
    uint16_t syncHandle;
} GapLePeriodicAdvTerminateSyncInfo;

typedef struct {
    int result;
    uint8_t addrType;
    BtAddr addr;
    uint8_t advSid;
} GapLePeriodicAdvertiserListInfo;

typedef struct {
    int result;
    int16_t txPathCompensation;
    int16_t rxPathCompensation;
} GapLeWriteRfPathCompensationInfo;

typedef struct {
    int result;
    uint16_t suggestedMaxTxOctets;
    uint16_t suggestedMaxTxTime;
} GapLeWriteSuggestedDefaultDataLengthInfo;

typedef struct {
    int result;
    uint8_t rxChannel;
    uint8_t phy;
    uint8_t modulationIndex;
} GapLeEnhancedReceiverTestInfo;

typedef struct {
    int result;
    uint8_t txChannel;
    uint8_t lengthOfTestData;
    uint8_t packetPayload;
    uint8_t phy;
} GapLeEnhancedTransmitterTestInfo;

static void GapLeSetPhyTask(void *ctx)
{
    GapLeSetPhyInfo *info = ctx;
    info->result = GAP_LeSetPhy(&info->addr, info->allPhys, info->txPhys, info->rxPhys, info->phyOptions);
}

int GAPIF_LeSetPhy(const BtAddr *addr, uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys, uint16_t phyOptions)
{
    if (addr == NULL || (allPhys & ~GAP_LE_ALL_PHY_VALID_MASK) != 0 ||
        (txPhys & ~GAP_LE_PHY_BIT_ALL) != 0 || (rxPhys & ~GAP_LE_PHY_BIT_ALL) != 0 ||
        (phyOptions != GAP_LE_PHY_OPTIONS_CODED_NO_PREFERENCE &&
         phyOptions != GAP_LE_PHY_OPTIONS_CODED_S2 &&
         phyOptions != GAP_LE_PHY_OPTIONS_CODED_S8) ||
        ((allPhys & GAP_LE_ALL_PHY_TX_NO_PREFERENCE) && txPhys != 0) ||
        ((allPhys & GAP_LE_ALL_PHY_RX_NO_PREFERENCE) && rxPhys != 0) ||
        ((!(allPhys & GAP_LE_ALL_PHY_TX_NO_PREFERENCE)) && txPhys == 0) ||
        ((!(allPhys & GAP_LE_ALL_PHY_RX_NO_PREFERENCE)) && rxPhys == 0)) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: addrType:%hhu (address redacted)", __FUNCTION__, addr->type);
    GapLeSetPhyInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeSetPhyInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeSetPhyInfo), 0x00, sizeof(GapLeSetPhyInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    if (memcpy_s(&ctx->addr, sizeof(BtAddr), addr, sizeof(BtAddr)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }
    ctx->allPhys = allPhys;
    ctx->txPhys = txPhys;
    ctx->rxPhys = rxPhys;
    ctx->phyOptions = phyOptions;

    int ret = GapRunTaskBlockProcess(GapLeSetPhyTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeReadPhyTask(void *ctx)
{
    GapLeAddrInfo *info = ctx;
    info->result = GAP_LeReadPhy(&info->addr);
}

int GAPIF_LeReadPhy(const BtAddr *addr)
{
    if (addr == NULL) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: addrType:%hhu (address redacted)", __FUNCTION__, addr->type);
    GapLeAddrInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeAddrInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeAddrInfo), 0x00, sizeof(GapLeAddrInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    if (memcpy_s(&ctx->addr, sizeof(BtAddr), addr, sizeof(BtAddr)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    int ret = GapRunTaskBlockProcess(GapLeReadPhyTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeSetDefaultPhyTask(void *ctx)
{
    GapLeSetDefaultPhyInfo *info = ctx;
    info->result = GAP_LeSetDefaultPhy(info->allPhys, info->txPhys, info->rxPhys);
}

int GAPIF_LeSetDefaultPhy(uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys)
{
    if ((allPhys & ~GAP_LE_ALL_PHY_VALID_MASK) != 0 || (txPhys & ~GAP_LE_PHY_BIT_ALL) != 0 ||
        (rxPhys & ~GAP_LE_PHY_BIT_ALL) != 0 ||
        ((allPhys & GAP_LE_ALL_PHY_TX_NO_PREFERENCE) && txPhys != 0) ||
        ((allPhys & GAP_LE_ALL_PHY_RX_NO_PREFERENCE) && rxPhys != 0) ||
        ((!(allPhys & GAP_LE_ALL_PHY_TX_NO_PREFERENCE)) && txPhys == 0) ||
        ((!(allPhys & GAP_LE_ALL_PHY_RX_NO_PREFERENCE)) && rxPhys == 0)) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s:", __FUNCTION__);
    GapLeSetDefaultPhyInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeSetDefaultPhyInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeSetDefaultPhyInfo), 0x00, sizeof(GapLeSetDefaultPhyInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->allPhys = allPhys;
    ctx->txPhys = txPhys;
    ctx->rxPhys = rxPhys;

    int ret = GapRunTaskBlockProcess(GapLeSetDefaultPhyTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeSetDataLengthTask(void *ctx)
{
    GapLeSetDataLengthInfo *info = ctx;
    info->result = GAP_LeSetDataLength(&info->addr, info->txOctets, info->txTime);
}

int GAPIF_LeSetDataLength(const BtAddr *addr, uint16_t txOctets, uint16_t txTime)
{
    if (addr == NULL ||
        txOctets < GAP_LE_DATA_LENGTH_OCTETS_MIN || txOctets > GAP_LE_DATA_LENGTH_OCTETS_MAX ||
        txTime < GAP_LE_DATA_LENGTH_TIME_MIN || txTime > GAP_LE_DATA_LENGTH_TIME_MAX ||
        txTime < (txOctets * GAP_LE_DATA_LENGTH_TIME_PER_OCTET + GAP_LE_DATA_LENGTH_TIME_OVERHEAD)) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: addrType:%hhu (address redacted)", __FUNCTION__, addr->type);
    GapLeSetDataLengthInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeSetDataLengthInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeSetDataLengthInfo), 0x00, sizeof(GapLeSetDataLengthInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    if (memcpy_s(&ctx->addr, sizeof(BtAddr), addr, sizeof(BtAddr)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }
    ctx->txOctets = txOctets;
    ctx->txTime = txTime;

    int ret = GapRunTaskBlockProcess(GapLeSetDataLengthTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLePeriodicAdvSetParamTask(void *ctx)
{
    GapLePeriodicAdvSetParamInfo *info = ctx;
    info->result = GAP_LePeriodicAdvSetParam(info->advHandle, info->intervalMin, info->intervalMax, info->properties);
}

int GAPIF_LePeriodicAdvSetParam(uint8_t advHandle, uint16_t intervalMin, uint16_t intervalMax, uint16_t properties)
{
    if (advHandle > GAP_LE_ADV_HANDLE_MAX || intervalMin < GAP_PERIODIC_ADV_INTERVAL_MIN ||
        intervalMax < GAP_PERIODIC_ADV_INTERVAL_MIN || intervalMin > intervalMax ||
        (properties & ~GAP_PERIODIC_ADV_PROPERTIES_MASK) != 0) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: advHandle:%hhu", __FUNCTION__, advHandle);
    GapLePeriodicAdvSetParamInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLePeriodicAdvSetParamInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLePeriodicAdvSetParamInfo), 0x00, sizeof(GapLePeriodicAdvSetParamInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->advHandle = advHandle;
    ctx->intervalMin = intervalMin;
    ctx->intervalMax = intervalMax;
    ctx->properties = properties;

    int ret = GapRunTaskBlockProcess(GapLePeriodicAdvSetParamTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLePeriodicAdvSetDataTask(void *ctx)
{
    GapLePeriodicAdvSetDataInfo *info = ctx;
    info->result = GAP_LePeriodicAdvSetData(info->advHandle, info->operation, info->advDataLength, info->advData);
}

int GAPIF_LePeriodicAdvSetData(uint8_t advHandle, uint8_t operation, uint8_t advDataLength, const uint8_t *advData)
{
    if (advHandle > GAP_LE_ADV_HANDLE_MAX || operation > GAP_PERIODIC_ADV_OPERATION_MAX ||
        advDataLength > GAP_PERIODIC_ADV_DATA_LENGTH_MAX ||
        (advDataLength != 0 && advData == NULL) ||
        (operation == GAP_PERIODIC_ADV_DATA_OPERATION_UNCHANGED && advDataLength != 0)) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: advHandle:%hhu", __FUNCTION__, advHandle);
    GapLePeriodicAdvSetDataInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLePeriodicAdvSetDataInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLePeriodicAdvSetDataInfo), 0x00, sizeof(GapLePeriodicAdvSetDataInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->advHandle = advHandle;
    ctx->operation = operation;
    ctx->advDataLength = advDataLength;
    if (advDataLength != 0) {
        if (memcpy_s(ctx->advData, GAP_PERIODIC_ADV_DATA_LENGTH_MAX, advData, advDataLength) != EOK) {
            MEM_MALLOC.free(ctx);
            ctx = NULL;
            return BT_OPERATION_FAILED;
        }
    }

    int ret = GapRunTaskBlockProcess(GapLePeriodicAdvSetDataTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLePeriodicAdvSetEnableTask(void *ctx)
{
    GapLePeriodicAdvSetEnableInfo *info = ctx;
    info->result = GAP_LePeriodicAdvSetEnable(info->enable, info->advHandle);
}

int GAPIF_LePeriodicAdvSetEnable(uint8_t enable, uint8_t advHandle)
{
    if (enable > GAP_PERIODIC_ADV_ENABLE_TRUE || advHandle > GAP_LE_ADV_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: advHandle:%hhu enable:%hhu", __FUNCTION__, advHandle, enable);
    GapLePeriodicAdvSetEnableInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLePeriodicAdvSetEnableInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLePeriodicAdvSetEnableInfo), 0x00, sizeof(GapLePeriodicAdvSetEnableInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->enable = enable;
    ctx->advHandle = advHandle;

    int ret = GapRunTaskBlockProcess(GapLePeriodicAdvSetEnableTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

typedef struct {
    int result;
    const GapPeriodicAdvSyncCallback *callback;
    void *context;
} GapLePeriodicAdvSyncCallbackInfo;

static void GapRegisterPeriodicAdvSyncCallbackTask(void *ctx)
{
    GapLePeriodicAdvSyncCallbackInfo *info = ctx;
    info->result = GAP_LeRegisterPeriodicAdvSyncCallback(info->callback, info->context);
}

int GAPIF_RegisterPeriodicAdvSyncCallback(const GapPeriodicAdvSyncCallback *callback, void *context)
{
    if (callback == NULL) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s:", __FUNCTION__);
    GapLePeriodicAdvSyncCallbackInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLePeriodicAdvSyncCallbackInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLePeriodicAdvSyncCallbackInfo), 0x00,
        sizeof(GapLePeriodicAdvSyncCallbackInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->callback = callback;
    ctx->context = context;

    int ret = GapRunTaskBlockProcess(GapRegisterPeriodicAdvSyncCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapDeregisterPeriodicAdvSyncCallbackTask(void *ctx)
{
    GapGeneralVoidInfo *info = ctx;
    info->result = GAP_LeDeregisterPeriodicAdvSyncCallback();
}

int GAPIF_DeregisterPeriodicAdvSyncCallback(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    GapGeneralVoidInfo *ctx = MEM_MALLOC.alloc(sizeof(GapGeneralVoidInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapGeneralVoidInfo), 0x00, sizeof(GapGeneralVoidInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    int ret = GapRunTaskBlockProcess(GapDeregisterPeriodicAdvSyncCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLePeriodicAdvCreateSyncTask(void *ctx)
{
    GapLePeriodicAdvCreateSyncInfo *info = ctx;
    const BtAddr *addr = (info->filterPolicy == GAP_PERIODIC_ADV_SYNC_FILTER_POLICY_DISABLED) ? &info->advAddr : NULL;
    info->result = GAP_LePeriodicAdvCreateSync(info->filterPolicy, info->advSid, addr, info->skip, info->syncTimeout);
}

int GAPIF_LePeriodicAdvCreateSync(uint8_t filterPolicy, uint8_t advSid, const BtAddr *advAddr, uint16_t skip,
                                  uint16_t syncTimeout)
{
    if (filterPolicy > GAP_PERIODIC_ADV_SYNC_FILTER_POLICY_ENABLED || advSid > GAP_LE_ADV_SID_MAX ||
        skip > GAP_PERIODIC_ADV_SKIP_MAX || syncTimeout < GAP_PERIODIC_ADV_SYNC_TIMEOUT_MIN ||
        syncTimeout > GAP_PERIODIC_ADV_SYNC_TIMEOUT_MAX) {
        return BT_BAD_PARAM;
    }

    if (filterPolicy == GAP_PERIODIC_ADV_SYNC_FILTER_POLICY_DISABLED &&
        (advAddr == NULL || GapIsEmptyAddr(advAddr->addr))) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: sid:%hhu", __FUNCTION__, advSid);
    GapLePeriodicAdvCreateSyncInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLePeriodicAdvCreateSyncInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLePeriodicAdvCreateSyncInfo), 0x00, sizeof(GapLePeriodicAdvCreateSyncInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->filterPolicy = filterPolicy;
    ctx->advSid = advSid;
    if (filterPolicy == GAP_PERIODIC_ADV_SYNC_FILTER_POLICY_DISABLED) {
        if (memcpy_s(&ctx->advAddr, sizeof(BtAddr), advAddr, sizeof(BtAddr)) != EOK) {
            MEM_MALLOC.free(ctx);
            ctx = NULL;
            return BT_OPERATION_FAILED;
        }
    }
    ctx->skip = skip;
    ctx->syncTimeout = syncTimeout;

    int ret = GapRunTaskBlockProcess(GapLePeriodicAdvCreateSyncTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLePeriodicAdvCreateSyncCancelTask(void *ctx)
{
    GapGeneralVoidInfo *info = ctx;
    info->result = GAP_LePeriodicAdvCreateSyncCancel();
}

int GAPIF_LePeriodicAdvCreateSyncCancel(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    GapGeneralVoidInfo *ctx = MEM_MALLOC.alloc(sizeof(GapGeneralVoidInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapGeneralVoidInfo), 0x00, sizeof(GapGeneralVoidInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    int ret = GapRunTaskBlockProcess(GapLePeriodicAdvCreateSyncCancelTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLePeriodicAdvTerminateSyncTask(void *ctx)
{
    GapLePeriodicAdvTerminateSyncInfo *info = ctx;
    info->result = GAP_LePeriodicAdvTerminateSync(info->syncHandle);
}

int GAPIF_LePeriodicAdvTerminateSync(uint16_t syncHandle)
{
    if (syncHandle > GAP_PERIODIC_ADV_SYNC_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: syncHandle:0x%04x", __FUNCTION__, syncHandle);
    GapLePeriodicAdvTerminateSyncInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLePeriodicAdvTerminateSyncInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLePeriodicAdvTerminateSyncInfo), 0x00,
        sizeof(GapLePeriodicAdvTerminateSyncInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->syncHandle = syncHandle;

    int ret = GapRunTaskBlockProcess(GapLePeriodicAdvTerminateSyncTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeAddDeviceToPeriodicAdvertiserListTask(void *ctx)
{
    GapLePeriodicAdvertiserListInfo *info = ctx;
    info->result = GAP_LeAddDeviceToPeriodicAdvertiserList(info->addrType, &info->addr, info->advSid);
}

int GAPIF_LeAddDeviceToPeriodicAdvertiserList(uint8_t addrType, const BtAddr *addr, uint8_t advSid)
{
    if (addr == NULL || advSid > GAP_LE_ADV_SID_MAX ||
        (addrType != BT_PUBLIC_DEVICE_ADDRESS && addrType != BT_RANDOM_DEVICE_ADDRESS) ||
        addrType != addr->type) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: addrType:%hhu (address redacted)", __FUNCTION__, addr->type);
    GapLePeriodicAdvertiserListInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLePeriodicAdvertiserListInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLePeriodicAdvertiserListInfo), 0x00, sizeof(GapLePeriodicAdvertiserListInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->addrType = addrType;
    if (memcpy_s(&ctx->addr, sizeof(BtAddr), addr, sizeof(BtAddr)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }
    ctx->advSid = advSid;

    int ret = GapRunTaskBlockProcess(GapLeAddDeviceToPeriodicAdvertiserListTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeRemoveDeviceFromPeriodicAdvertiserListTask(void *ctx)
{
    GapLePeriodicAdvertiserListInfo *info = ctx;
    info->result = GAP_LeRemoveDeviceFromPeriodicAdvertiserList(info->addrType, &info->addr, info->advSid);
}

int GAPIF_LeRemoveDeviceFromPeriodicAdvertiserList(uint8_t addrType, const BtAddr *addr, uint8_t advSid)
{
    if (addr == NULL || advSid > GAP_LE_ADV_SID_MAX ||
        (addrType != BT_PUBLIC_DEVICE_ADDRESS && addrType != BT_RANDOM_DEVICE_ADDRESS) ||
        addrType != addr->type) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: addrType:%hhu (address redacted)", __FUNCTION__, addr->type);
    GapLePeriodicAdvertiserListInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLePeriodicAdvertiserListInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLePeriodicAdvertiserListInfo), 0x00, sizeof(GapLePeriodicAdvertiserListInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->addrType = addrType;
    if (memcpy_s(&ctx->addr, sizeof(BtAddr), addr, sizeof(BtAddr)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }
    ctx->advSid = advSid;

    int ret = GapRunTaskBlockProcess(GapLeRemoveDeviceFromPeriodicAdvertiserListTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeClearPeriodicAdvertiserListTask(void *ctx)
{
    GapGeneralVoidInfo *info = ctx;
    info->result = GAP_LeClearPeriodicAdvertiserList();
}

int GAPIF_LeClearPeriodicAdvertiserList(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    GapGeneralVoidInfo *ctx = MEM_MALLOC.alloc(sizeof(GapGeneralVoidInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapGeneralVoidInfo), 0x00, sizeof(GapGeneralVoidInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    int ret = GapRunTaskBlockProcess(GapLeClearPeriodicAdvertiserListTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeReadPeriodicAdvertiserListSizeTask(void *ctx)
{
    GapGeneralVoidInfo *info = ctx;
    info->result = GAP_LeReadPeriodicAdvertiserListSize();
}

int GAPIF_LeReadPeriodicAdvertiserListSize(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    GapGeneralVoidInfo *ctx = MEM_MALLOC.alloc(sizeof(GapGeneralVoidInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapGeneralVoidInfo), 0x00, sizeof(GapGeneralVoidInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    int ret = GapRunTaskBlockProcess(GapLeReadPeriodicAdvertiserListSizeTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

typedef struct {
    int result;
    const GapLeControllerCallback *callback;
    void *context;
} GapLeControllerCallbackInfo;

static void GapRegisterLeControllerCallbackTask(void *ctx)
{
    GapLeControllerCallbackInfo *info = ctx;
    info->result = GAP_LeRegisterLeControllerCallback(info->callback, info->context);
}

int GAPIF_RegisterLeControllerCallback(const GapLeControllerCallback *callback, void *context)
{
    if (callback == NULL) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s:", __FUNCTION__);
    GapLeControllerCallbackInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeControllerCallbackInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeControllerCallbackInfo), 0x00, sizeof(GapLeControllerCallbackInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->callback = callback;
    ctx->context = context;

    int ret = GapRunTaskBlockProcess(GapRegisterLeControllerCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapDeregisterLeControllerCallbackTask(void *ctx)
{
    GapGeneralVoidInfo *info = ctx;
    info->result = GAP_LeDeregisterLeControllerCallback();
}

int GAPIF_DeregisterLeControllerCallback(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    GapGeneralVoidInfo *ctx = MEM_MALLOC.alloc(sizeof(GapGeneralVoidInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapGeneralVoidInfo), 0x00, sizeof(GapGeneralVoidInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    int ret = GapRunTaskBlockProcess(GapDeregisterLeControllerCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeReadTransmitPowerTask(void *ctx)
{
    GapGeneralVoidInfo *info = ctx;
    info->result = GAP_LeReadTransmitPower();
}

int GAPIF_LeReadTransmitPower(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    GapGeneralVoidInfo *ctx = MEM_MALLOC.alloc(sizeof(GapGeneralVoidInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapGeneralVoidInfo), 0x00, sizeof(GapGeneralVoidInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    int ret = GapRunTaskBlockProcess(GapLeReadTransmitPowerTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeReadRfPathCompensationTask(void *ctx)
{
    GapGeneralVoidInfo *info = ctx;
    info->result = GAP_LeReadRfPathCompensation();
}

int GAPIF_LeReadRfPathCompensation(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    GapGeneralVoidInfo *ctx = MEM_MALLOC.alloc(sizeof(GapGeneralVoidInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapGeneralVoidInfo), 0x00, sizeof(GapGeneralVoidInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    int ret = GapRunTaskBlockProcess(GapLeReadRfPathCompensationTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeWriteRfPathCompensationTask(void *ctx)
{
    GapLeWriteRfPathCompensationInfo *info = ctx;
    info->result = GAP_LeWriteRfPathCompensation(info->txPathCompensation, info->rxPathCompensation);
}

int GAPIF_LeWriteRfPathCompensation(int16_t txPathCompensation, int16_t rxPathCompensation)
{
    if (txPathCompensation < GAP_LE_RF_PATH_COMPENSATION_MIN ||
        txPathCompensation > GAP_LE_RF_PATH_COMPENSATION_MAX ||
        rxPathCompensation < GAP_LE_RF_PATH_COMPENSATION_MIN ||
        rxPathCompensation > GAP_LE_RF_PATH_COMPENSATION_MAX) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s:", __FUNCTION__);
    GapLeWriteRfPathCompensationInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeWriteRfPathCompensationInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeWriteRfPathCompensationInfo), 0x00,
        sizeof(GapLeWriteRfPathCompensationInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->txPathCompensation = txPathCompensation;
    ctx->rxPathCompensation = rxPathCompensation;

    int ret = GapRunTaskBlockProcess(GapLeWriteRfPathCompensationTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeReadSuggestedDefaultDataLengthTask(void *ctx)
{
    GapGeneralVoidInfo *info = ctx;
    info->result = GAP_LeReadSuggestedDefaultDataLength();
}

int GAPIF_LeReadSuggestedDefaultDataLength(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    GapGeneralVoidInfo *ctx = MEM_MALLOC.alloc(sizeof(GapGeneralVoidInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapGeneralVoidInfo), 0x00, sizeof(GapGeneralVoidInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    int ret = GapRunTaskBlockProcess(GapLeReadSuggestedDefaultDataLengthTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeWriteSuggestedDefaultDataLengthTask(void *ctx)
{
    GapLeWriteSuggestedDefaultDataLengthInfo *info = ctx;
    info->result = GAP_LeWriteSuggestedDefaultDataLength(info->suggestedMaxTxOctets, info->suggestedMaxTxTime);
}

int GAPIF_LeWriteSuggestedDefaultDataLength(uint16_t suggestedMaxTxOctets, uint16_t suggestedMaxTxTime)
{
    if (suggestedMaxTxOctets < GAP_LE_DATA_LENGTH_OCTETS_MIN || suggestedMaxTxOctets > GAP_LE_DATA_LENGTH_OCTETS_MAX ||
        suggestedMaxTxTime < GAP_LE_DATA_LENGTH_TIME_MIN || suggestedMaxTxTime > GAP_LE_DATA_LENGTH_TIME_MAX ||
        suggestedMaxTxTime < (suggestedMaxTxOctets * GAP_LE_DATA_LENGTH_TIME_PER_OCTET +
            GAP_LE_DATA_LENGTH_TIME_OVERHEAD)) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s:", __FUNCTION__);
    GapLeWriteSuggestedDefaultDataLengthInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeWriteSuggestedDefaultDataLengthInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeWriteSuggestedDefaultDataLengthInfo), 0x00,
                 sizeof(GapLeWriteSuggestedDefaultDataLengthInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->suggestedMaxTxOctets = suggestedMaxTxOctets;
    ctx->suggestedMaxTxTime = suggestedMaxTxTime;

    int ret = GapRunTaskBlockProcess(GapLeWriteSuggestedDefaultDataLengthTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeReadMaximumDataLengthTask(void *ctx)
{
    GapGeneralVoidInfo *info = ctx;
    info->result = GAP_LeReadMaximumDataLength();
}

int GAPIF_LeReadMaximumDataLength(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    GapGeneralVoidInfo *ctx = MEM_MALLOC.alloc(sizeof(GapGeneralVoidInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapGeneralVoidInfo), 0x00, sizeof(GapGeneralVoidInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    int ret = GapRunTaskBlockProcess(GapLeReadMaximumDataLengthTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeEnhancedReceiverTestTask(void *ctx)
{
    GapLeEnhancedReceiverTestInfo *info = ctx;
    info->result = GAP_LeEnhancedReceiverTest(info->rxChannel, info->phy, info->modulationIndex);
}

int GAPIF_LeEnhancedReceiverTest(uint8_t rxChannel, uint8_t phy, uint8_t modulationIndex)
{
    if (rxChannel > GAP_LE_TEST_CHANNEL_MAX ||
        (phy != GAP_LE_PHY_1M && phy != GAP_LE_PHY_2M && phy != GAP_LE_PHY_CODED) ||
        modulationIndex > GAP_LE_TEST_MODULATION_INDEX_MAX) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s:", __FUNCTION__);
    GapLeEnhancedReceiverTestInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeEnhancedReceiverTestInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeEnhancedReceiverTestInfo), 0x00, sizeof(GapLeEnhancedReceiverTestInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->rxChannel = rxChannel;
    ctx->phy = phy;
    ctx->modulationIndex = modulationIndex;

    int ret = GapRunTaskBlockProcess(GapLeEnhancedReceiverTestTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeEnhancedTransmitterTestTask(void *ctx)
{
    GapLeEnhancedTransmitterTestInfo *info = ctx;
    info->result =
        GAP_LeEnhancedTransmitterTest(info->txChannel, info->lengthOfTestData, info->packetPayload, info->phy);
}

int GAPIF_LeEnhancedTransmitterTest(uint8_t txChannel, uint8_t lengthOfTestData, uint8_t packetPayload, uint8_t phy)
{
    if (txChannel > GAP_LE_TEST_CHANNEL_MAX ||
        packetPayload > GAP_LE_TEST_PACKET_PAYLOAD_MAX ||
        (phy != GAP_LE_PHY_1M && phy != GAP_LE_PHY_2M && phy != GAP_LE_PHY_CODED && phy != GAP_LE_TEST_PHY_CODED_S2) ||
        lengthOfTestData > GAP_LE_TEST_DATA_LENGTH_MAX) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s:", __FUNCTION__);
    GapLeEnhancedTransmitterTestInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeEnhancedTransmitterTestInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeEnhancedTransmitterTestInfo), 0x00,
        sizeof(GapLeEnhancedTransmitterTestInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->txChannel = txChannel;
    ctx->lengthOfTestData = lengthOfTestData;
    ctx->packetPayload = packetPayload;
    ctx->phy = phy;

    int ret = GapRunTaskBlockProcess(GapLeEnhancedTransmitterTestTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}
#endif
