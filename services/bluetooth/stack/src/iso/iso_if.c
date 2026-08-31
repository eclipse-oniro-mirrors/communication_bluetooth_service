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

#include "iso_le_if.h"

#include <securec.h>

#include "allocator.h"
#include "log.h"

#include "iso.h"
#include "iso_task_internal.h"

typedef struct {
    int result;
    uint8_t cigId;
    IsoLeCigParam cigParam;
    uint8_t cisCount;
    const IsoLeCisParam *cisParams;
} IsoLeCreateCigInfo;

typedef struct {
    int result;
    uint8_t cisCount;
    const IsoLeCreateCisParam *params;
} IsoLeCreateCisInfo;

typedef struct {
    int result;
    uint8_t cigId;
} IsoLeRemoveCigInfo;

typedef struct {
    int result;
    uint16_t cisHandle;
} IsoLeAcceptCisRequestInfo;

typedef struct {
    int result;
    uint16_t cisHandle;
    uint8_t reason;
} IsoLeRejectCisRequestInfo;

typedef struct {
    int result;
    uint16_t cisHandle;
    uint8_t reason;
} IsoLeDisconnectCisInfo;

typedef struct {
    int result;
    uint8_t bigHandle;
    uint8_t advertisingHandle;
    uint8_t numBis;
    IsoLeBigParam bigParam;
    const uint8_t *broadcastCode;
} IsoLeCreateBigInfo;

typedef struct {
    int result;
    uint8_t bigHandle;
    uint8_t advertisingHandle;
    uint8_t numBis;
    IsoLeBigTestParam bigParam;
    const uint8_t *broadcastCode;
} IsoLeCreateBigTestInfo;

typedef struct {
    int result;
    uint8_t bigHandle;
    uint8_t reason;
} IsoLeTerminateBigInfo;

typedef struct {
    int result;
    IsoLeBigCreateSyncParam param;
} IsoLeBigCreateSyncInfo;

typedef struct {
    int result;
    uint8_t bigHandle;
} IsoLeBigTerminateSyncInfo;

typedef struct {
    int result;
    IsoLeSetupIsoDataPathParam param;
} IsoLeSetupIsoDataPathInfo;

typedef struct {
    int result;
    uint16_t connectionHandle;
    uint8_t dataPathDirection;
} IsoLeRemoveIsoDataPathInfo;

typedef struct {
    int result;
    uint16_t connectionHandle;
    uint8_t payloadType;
} IsoLeIsoTransmitTestInfo;

typedef struct {
    int result;
    uint16_t connectionHandle;
    uint8_t payloadType;
} IsoLeIsoReceiveTestInfo;

typedef struct {
    int result;
    uint16_t connectionHandle;
} IsoLeIsoReadTestCountersInfo;

typedef struct {
    int result;
    uint16_t connectionHandle;
} IsoLeIsoTestEndInfo;

typedef struct {
    int result;
    uint16_t connectionHandle;
} IsoLeReadIsoLinkQualityInfo;

typedef struct {
    int result;
    uint16_t connectionHandle;
} IsoLeReadIsoTxSyncInfo;

typedef struct {
    int result;
    uint16_t connectionHandle;
} IsoLeRequestPeerScaInfo;

static void IsoLeCreateCigTask(void *ctx)
{
    IsoLeCreateCigInfo *info = ctx;
    info->result = IsoLeCreateCig(info->cigId, &info->cigParam, info->cisCount, info->cisParams);
}

static void IsoLeCreateCisTask(void *ctx)
{
    IsoLeCreateCisInfo *info = ctx;
    info->result = IsoLeCreateCis(info->cisCount, info->params);
}

static void IsoLeRemoveCigTask(void *ctx)
{
    IsoLeRemoveCigInfo *info = ctx;
    info->result = IsoLeRemoveCig(info->cigId);
}

static void IsoLeAcceptCisRequestTask(void *ctx)
{
    IsoLeAcceptCisRequestInfo *info = ctx;
    info->result = IsoLeAcceptCisRequest(info->cisHandle);
}

static void IsoLeRejectCisRequestTask(void *ctx)
{
    IsoLeRejectCisRequestInfo *info = ctx;
    info->result = IsoLeRejectCisRequest(info->cisHandle, info->reason);
}

static void IsoLeDisconnectCisTask(void *ctx)
{
    IsoLeDisconnectCisInfo *info = ctx;
    info->result = IsoLeDisconnectCis(info->cisHandle, info->reason);
}

static void IsoLeCreateBigTask(void *ctx)
{
    IsoLeCreateBigInfo *info = ctx;
    info->result =
        IsoLeCreateBig(info->bigHandle, info->advertisingHandle, info->numBis, &info->bigParam, info->broadcastCode);
}

static void IsoLeCreateBigTestTask(void *ctx)
{
    IsoLeCreateBigTestInfo *info = ctx;
    info->result = IsoLeCreateBigTest(
        info->bigHandle, info->advertisingHandle, info->numBis, &info->bigParam, info->broadcastCode);
}

static void IsoLeTerminateBigTask(void *ctx)
{
    IsoLeTerminateBigInfo *info = ctx;
    info->result = IsoLeTerminateBig(info->bigHandle, info->reason);
}

static void IsoLeBigCreateSyncTask(void *ctx)
{
    IsoLeBigCreateSyncInfo *info = ctx;
    info->result = IsoLeBigCreateSync(&info->param);
}

static void IsoLeBigTerminateSyncTask(void *ctx)
{
    IsoLeBigTerminateSyncInfo *info = ctx;
    info->result = IsoLeBigTerminateSync(info->bigHandle);
}

static void IsoLeSetupIsoDataPathTask(void *ctx)
{
    IsoLeSetupIsoDataPathInfo *info = ctx;
    info->result = IsoLeSetupIsoDataPath(&info->param);
}

static void IsoLeRemoveIsoDataPathTask(void *ctx)
{
    IsoLeRemoveIsoDataPathInfo *info = ctx;
    info->result = IsoLeRemoveIsoDataPath(info->connectionHandle, info->dataPathDirection);
}

static void IsoRegisterDataPathCallbackTask(void *ctx)
{
    IsoGeneralCallbackInfo *info = ctx;
    info->result = IsoRegisterDataPathCallback(info->callback, info->context);
}

static void IsoDeregisterDataPathCallbackTask(void *ctx)
{
    IsoGeneralVoidInfo *info = ctx;
    info->result = IsoDeregisterDataPathCallback();
}

static void IsoLeIsoTransmitTestTask(void *ctx)
{
    IsoLeIsoTransmitTestInfo *info = ctx;
    info->result = IsoLeIsoTransmitTest(info->connectionHandle, info->payloadType);
}

static void IsoLeIsoReceiveTestTask(void *ctx)
{
    IsoLeIsoReceiveTestInfo *info = ctx;
    info->result = IsoLeIsoReceiveTest(info->connectionHandle, info->payloadType);
}

static void IsoLeIsoReadTestCountersTask(void *ctx)
{
    IsoLeIsoReadTestCountersInfo *info = ctx;
    info->result = IsoLeIsoReadTestCounters(info->connectionHandle);
}

static void IsoLeIsoTestEndTask(void *ctx)
{
    IsoLeIsoTestEndInfo *info = ctx;
    info->result = IsoLeIsoTestEnd(info->connectionHandle);
}

static void IsoRegisterTestCallbackTask(void *ctx)
{
    IsoGeneralCallbackInfo *info = ctx;
    info->result = IsoRegisterTestCallback(info->callback, info->context);
}

static void IsoDeregisterTestCallbackTask(void *ctx)
{
    IsoGeneralVoidInfo *info = ctx;
    info->result = IsoDeregisterTestCallback();
}

static void IsoLeReadIsoLinkQualityTask(void *ctx)
{
    IsoLeReadIsoLinkQualityInfo *info = ctx;
    info->result = IsoLeReadIsoLinkQuality(info->connectionHandle);
}

static void IsoLeReadIsoTxSyncTask(void *ctx)
{
    IsoLeReadIsoTxSyncInfo *info = ctx;
    info->result = IsoLeReadIsoTxSync(info->connectionHandle);
}

static void IsoLeRequestPeerScaTask(void *ctx)
{
    IsoLeRequestPeerScaInfo *info = ctx;
    info->result = IsoLeRequestPeerSca(info->connectionHandle);
}

static void IsoRegisterStatusQueryCallbackTask(void *ctx)
{
    IsoGeneralCallbackInfo *info = ctx;
    info->result = IsoRegisterStatusQueryCallback(info->callback, info->context);
}

static void IsoDeregisterStatusQueryCallbackTask(void *ctx)
{
    IsoGeneralVoidInfo *info = ctx;
    info->result = IsoDeregisterStatusQueryCallback();
}

static void IsoRegisterBigCallbackTask(void *ctx)
{
    IsoGeneralCallbackInfo *info = ctx;
    info->result = IsoRegisterBigCallback(info->callback, info->context);
}

static void IsoDeregisterBigCallbackTask(void *ctx)
{
    IsoGeneralVoidInfo *info = ctx;
    info->result = IsoDeregisterBigCallback();
}

static void IsoRegisterCigCallbackTask(void *ctx)
{
    IsoGeneralCallbackInfo *info = ctx;
    info->result = IsoRegisterCigCallback(info->callback, info->context);
}

static void IsoDeregisterCigCallbackTask(void *ctx)
{
    IsoGeneralVoidInfo *info = ctx;
    info->result = IsoDeregisterCigCallback();
}

int ISOIF_LeRegisterCigCallback(const IsoLeCigCallback *callback, void *context)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    IsoGeneralCallbackInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoGeneralCallbackInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoGeneralCallbackInfo), 0x00, sizeof(IsoGeneralCallbackInfo));

    ctx->callback = (void *)callback;
    ctx->context = context;

    int ret = IsoRunTaskBlockProcess(IsoRegisterCigCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeDeregisterCigCallback(void)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    IsoGeneralVoidInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoGeneralVoidInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoGeneralVoidInfo), 0x00, sizeof(IsoGeneralVoidInfo));

    int ret = IsoRunTaskBlockProcess(IsoDeregisterCigCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeCreateCig(uint8_t cigId, const IsoLeCigParam *cigParam, uint8_t cisCount, const IsoLeCisParam *cisParams)
{
    LOG_INFO("%{public}s: cigId:0x%02x, cisCount:%hhu", __FUNCTION__, cigId, cisCount);
    if (cigParam == NULL || cisParams == NULL) {
        return BT_BAD_PARAM;
    }
    IsoLeCreateCigInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeCreateCigInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeCreateCigInfo), 0x00, sizeof(IsoLeCreateCigInfo));

    ctx->cigParam = *cigParam;
    ctx->cigId = cigId;
    ctx->cisCount = cisCount;
    ctx->cisParams = cisParams;

    int ret = IsoRunTaskBlockProcess(IsoLeCreateCigTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeCreateCis(uint8_t cisCount, const IsoLeCreateCisParam *params)
{
    LOG_INFO("%{public}s: cisCount:%hhu", __FUNCTION__, cisCount);
    if (params == NULL) {
        return BT_BAD_PARAM;
    }
    IsoLeCreateCisInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeCreateCisInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeCreateCisInfo), 0x00, sizeof(IsoLeCreateCisInfo));

    ctx->cisCount = cisCount;
    ctx->params = params;

    int ret = IsoRunTaskBlockProcess(IsoLeCreateCisTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeRemoveCig(uint8_t cigId)
{
    LOG_INFO("%{public}s: cigId:0x%02x", __FUNCTION__, cigId);
    IsoLeRemoveCigInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeRemoveCigInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeRemoveCigInfo), 0x00, sizeof(IsoLeRemoveCigInfo));

    ctx->cigId = cigId;

    int ret = IsoRunTaskBlockProcess(IsoLeRemoveCigTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeAcceptCisRequest(uint16_t cisHandle)
{
    LOG_INFO("%{public}s: cisHandle:0x%04x", __FUNCTION__, cisHandle);
    IsoLeAcceptCisRequestInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeAcceptCisRequestInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeAcceptCisRequestInfo), 0x00, sizeof(IsoLeAcceptCisRequestInfo));

    ctx->cisHandle = cisHandle;

    int ret = IsoRunTaskBlockProcess(IsoLeAcceptCisRequestTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeRejectCisRequest(uint16_t cisHandle, uint8_t reason)
{
    LOG_INFO("%{public}s: cisHandle:0x%04x, reason:0x%02x", __FUNCTION__, cisHandle, reason);
    IsoLeRejectCisRequestInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeRejectCisRequestInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeRejectCisRequestInfo), 0x00, sizeof(IsoLeRejectCisRequestInfo));

    ctx->cisHandle = cisHandle;
    ctx->reason = reason;

    int ret = IsoRunTaskBlockProcess(IsoLeRejectCisRequestTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeDisconnectCis(uint16_t cisHandle, uint8_t reason)
{
    LOG_INFO("%{public}s: cisHandle:0x%04x, reason:0x%02x", __FUNCTION__, cisHandle, reason);
    IsoLeDisconnectCisInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeDisconnectCisInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeDisconnectCisInfo), 0x00, sizeof(IsoLeDisconnectCisInfo));

    ctx->cisHandle = cisHandle;
    ctx->reason = reason;

    int ret = IsoRunTaskBlockProcess(IsoLeDisconnectCisTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeRegisterBigCallback(const IsoLeBigCallback *callback, void *context)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    IsoGeneralCallbackInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoGeneralCallbackInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoGeneralCallbackInfo), 0x00, sizeof(IsoGeneralCallbackInfo));

    ctx->callback = (void *)callback;
    ctx->context = context;

    int ret = IsoRunTaskBlockProcess(IsoRegisterBigCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeDeregisterBigCallback(void)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    IsoGeneralVoidInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoGeneralVoidInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoGeneralVoidInfo), 0x00, sizeof(IsoGeneralVoidInfo));

    int ret = IsoRunTaskBlockProcess(IsoDeregisterBigCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeCreateBig(uint8_t bigHandle, uint8_t advertisingHandle, uint8_t numBis, const IsoLeBigParam *bigParam,
    const uint8_t *broadcastCode)
{
    LOG_INFO("%{public}s: bigHandle:0x%02x, advertisingHandle:0x%02x, numBis:%hhu", __FUNCTION__, bigHandle,
        advertisingHandle, numBis);
    if (bigParam == NULL) {
        return BT_BAD_PARAM;
    }
    IsoLeCreateBigInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeCreateBigInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeCreateBigInfo), 0x00, sizeof(IsoLeCreateBigInfo));

    ctx->bigHandle = bigHandle;
    ctx->advertisingHandle = advertisingHandle;
    ctx->numBis = numBis;
    ctx->bigParam = *bigParam;
    ctx->broadcastCode = broadcastCode;

    int ret = IsoRunTaskBlockProcess(IsoLeCreateBigTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeCreateBigTest(uint8_t bigHandle, uint8_t advertisingHandle, uint8_t numBis,
    const IsoLeBigTestParam *bigParam, const uint8_t *broadcastCode)
{
    LOG_INFO("%{public}s: bigHandle:0x%02x, advertisingHandle:0x%02x, numBis:%hhu", __FUNCTION__, bigHandle,
        advertisingHandle, numBis);
    if (bigParam == NULL) {
        return BT_BAD_PARAM;
    }
    IsoLeCreateBigTestInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeCreateBigTestInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeCreateBigTestInfo), 0x00, sizeof(IsoLeCreateBigTestInfo));

    ctx->bigHandle = bigHandle;
    ctx->advertisingHandle = advertisingHandle;
    ctx->numBis = numBis;
    ctx->bigParam = *bigParam;
    ctx->broadcastCode = broadcastCode;

    int ret = IsoRunTaskBlockProcess(IsoLeCreateBigTestTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeTerminateBig(uint8_t bigHandle, uint8_t reason)
{
    LOG_INFO("%{public}s: bigHandle:0x%02x, reason:0x%02x", __FUNCTION__, bigHandle, reason);
    IsoLeTerminateBigInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeTerminateBigInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeTerminateBigInfo), 0x00, sizeof(IsoLeTerminateBigInfo));

    ctx->bigHandle = bigHandle;
    ctx->reason = reason;

    int ret = IsoRunTaskBlockProcess(IsoLeTerminateBigTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeBigCreateSync(const IsoLeBigCreateSyncParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: bigHandle:0x%02x, syncHandle:0x%04x, numBis:%hhu", __FUNCTION__, param->bigHandle,
        param->syncHandle, param->numBis);

    IsoLeBigCreateSyncInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeBigCreateSyncInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeBigCreateSyncInfo), 0x00, sizeof(IsoLeBigCreateSyncInfo));

    ctx->param = *param;

    int ret = IsoRunTaskBlockProcess(IsoLeBigCreateSyncTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeBigTerminateSync(uint8_t bigHandle)
{
    LOG_INFO("%{public}s: bigHandle:0x%02x", __FUNCTION__, bigHandle);
    IsoLeBigTerminateSyncInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeBigTerminateSyncInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeBigTerminateSyncInfo), 0x00, sizeof(IsoLeBigTerminateSyncInfo));

    ctx->bigHandle = bigHandle;

    int ret = IsoRunTaskBlockProcess(IsoLeBigTerminateSyncTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeRegisterDataPathCallback(const IsoLeDataPathCallback *callback, void *context)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    IsoGeneralCallbackInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoGeneralCallbackInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoGeneralCallbackInfo), 0x00, sizeof(IsoGeneralCallbackInfo));

    ctx->callback = (void *)callback;
    ctx->context = context;

    int ret = IsoRunTaskBlockProcess(IsoRegisterDataPathCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeDeregisterDataPathCallback(void)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    IsoGeneralVoidInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoGeneralVoidInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoGeneralVoidInfo), 0x00, sizeof(IsoGeneralVoidInfo));

    int ret = IsoRunTaskBlockProcess(IsoDeregisterDataPathCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeSetupIsoDataPath(const IsoLeSetupIsoDataPathParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: connectionHandle:0x%04x, dataPathDirection:0x%02x, dataPathId:0x%02x", __FUNCTION__,
        param->connectionHandle, param->dataPathDirection, param->dataPathId);
    IsoLeSetupIsoDataPathInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeSetupIsoDataPathInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeSetupIsoDataPathInfo), 0x00, sizeof(IsoLeSetupIsoDataPathInfo));

    ctx->param = *param;

    int ret = IsoRunTaskBlockProcess(IsoLeSetupIsoDataPathTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeRemoveIsoDataPath(uint16_t connectionHandle, uint8_t dataPathDirection)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x, dataPathDirection:0x%02x", __FUNCTION__, connectionHandle,
        dataPathDirection);
    IsoLeRemoveIsoDataPathInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeRemoveIsoDataPathInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeRemoveIsoDataPathInfo), 0x00, sizeof(IsoLeRemoveIsoDataPathInfo));

    ctx->connectionHandle = connectionHandle;
    ctx->dataPathDirection = dataPathDirection;

    int ret = IsoRunTaskBlockProcess(IsoLeRemoveIsoDataPathTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeRegisterTestCallback(const IsoLeTestCallback *callback, void *context)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    IsoGeneralCallbackInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoGeneralCallbackInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoGeneralCallbackInfo), 0x00, sizeof(IsoGeneralCallbackInfo));

    ctx->callback = (void *)callback;
    ctx->context = context;

    int ret = IsoRunTaskBlockProcess(IsoRegisterTestCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeDeregisterTestCallback(void)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    IsoGeneralVoidInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoGeneralVoidInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoGeneralVoidInfo), 0x00, sizeof(IsoGeneralVoidInfo));

    int ret = IsoRunTaskBlockProcess(IsoDeregisterTestCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeIsoTransmitTest(uint16_t connectionHandle, uint8_t payloadType)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x, payloadType:0x%02x", __FUNCTION__, connectionHandle, payloadType);
    IsoLeIsoTransmitTestInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeIsoTransmitTestInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeIsoTransmitTestInfo), 0x00, sizeof(IsoLeIsoTransmitTestInfo));

    ctx->connectionHandle = connectionHandle;
    ctx->payloadType = payloadType;

    int ret = IsoRunTaskBlockProcess(IsoLeIsoTransmitTestTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeIsoReceiveTest(uint16_t connectionHandle, uint8_t payloadType)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x, payloadType:0x%02x", __FUNCTION__, connectionHandle, payloadType);
    IsoLeIsoReceiveTestInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeIsoReceiveTestInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeIsoReceiveTestInfo), 0x00, sizeof(IsoLeIsoReceiveTestInfo));

    ctx->connectionHandle = connectionHandle;
    ctx->payloadType = payloadType;

    int ret = IsoRunTaskBlockProcess(IsoLeIsoReceiveTestTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeIsoReadTestCounters(uint16_t connectionHandle)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x", __FUNCTION__, connectionHandle);
    IsoLeIsoReadTestCountersInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeIsoReadTestCountersInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeIsoReadTestCountersInfo), 0x00, sizeof(IsoLeIsoReadTestCountersInfo));

    ctx->connectionHandle = connectionHandle;

    int ret = IsoRunTaskBlockProcess(IsoLeIsoReadTestCountersTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeIsoTestEnd(uint16_t connectionHandle)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x", __FUNCTION__, connectionHandle);
    IsoLeIsoTestEndInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeIsoTestEndInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeIsoTestEndInfo), 0x00, sizeof(IsoLeIsoTestEndInfo));

    ctx->connectionHandle = connectionHandle;

    int ret = IsoRunTaskBlockProcess(IsoLeIsoTestEndTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeRegisterStatusQueryCallback(const IsoLeStatusQueryCallback *callback, void *context)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    IsoGeneralCallbackInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoGeneralCallbackInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoGeneralCallbackInfo), 0x00, sizeof(IsoGeneralCallbackInfo));

    ctx->callback = (void *)callback;
    ctx->context = context;

    int ret = IsoRunTaskBlockProcess(IsoRegisterStatusQueryCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeDeregisterStatusQueryCallback(void)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    IsoGeneralVoidInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoGeneralVoidInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoGeneralVoidInfo), 0x00, sizeof(IsoGeneralVoidInfo));

    int ret = IsoRunTaskBlockProcess(IsoDeregisterStatusQueryCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeReadIsoLinkQuality(uint16_t connectionHandle)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x", __FUNCTION__, connectionHandle);
    IsoLeReadIsoLinkQualityInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeReadIsoLinkQualityInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeReadIsoLinkQualityInfo), 0x00, sizeof(IsoLeReadIsoLinkQualityInfo));

    ctx->connectionHandle = connectionHandle;

    int ret = IsoRunTaskBlockProcess(IsoLeReadIsoLinkQualityTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeReadIsoTxSync(uint16_t connectionHandle)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x", __FUNCTION__, connectionHandle);
    IsoLeReadIsoTxSyncInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeReadIsoTxSyncInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeReadIsoTxSyncInfo), 0x00, sizeof(IsoLeReadIsoTxSyncInfo));

    ctx->connectionHandle = connectionHandle;

    int ret = IsoRunTaskBlockProcess(IsoLeReadIsoTxSyncTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

int ISOIF_LeRequestPeerSca(uint16_t connectionHandle)
{
    LOG_INFO("%{public}s: connectionHandle:0x%04x", __FUNCTION__, connectionHandle);
    IsoLeRequestPeerScaInfo *ctx = MEM_MALLOC.alloc(sizeof(IsoLeRequestPeerScaInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(IsoLeRequestPeerScaInfo), 0x00, sizeof(IsoLeRequestPeerScaInfo));

    ctx->connectionHandle = connectionHandle;

    int ret = IsoRunTaskBlockProcess(IsoLeRequestPeerScaTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}
