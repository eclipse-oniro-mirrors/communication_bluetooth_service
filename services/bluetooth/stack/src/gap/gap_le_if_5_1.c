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

// Bluetooth 5.1 (Vol 2, Part E, 7.8.78-7.8.92) GAPIF_* wrappers. The antenna
// switching pattern arrays are carried in a fixed-size buffer of
// GAP_LE_SWITCHING_PATTERN_LENGTH_MAX entries; a zero length with NULL pointer
// is passed through to the GAP layer.

// Each task info struct embeds the caller-supplied parameter struct plus a
// private antenna ID buffer: the caller's antennaIds pointer is only valid for
// the duration of the GAPIF_* call, so the pattern is copied into the buffer
// and the embedded param's pointer is rebound to it before the task runs.
typedef struct {
    int result;
    GapLeReceiverTestV3Param param;
    uint8_t antennaIds[GAP_LE_SWITCHING_PATTERN_LENGTH_MAX];
} GapLeReceiverTestV3Info;

typedef struct {
    int result;
    GapLeTransmitterTestV3Param param;
    uint8_t antennaIds[GAP_LE_SWITCHING_PATTERN_LENGTH_MAX];
} GapLeTransmitterTestV3Info;

typedef struct {
    int result;
    GapLeSetConnectionlessCteTransmitParametersParam param;
    uint8_t antennaIds[GAP_LE_SWITCHING_PATTERN_LENGTH_MAX];
} GapLeSetConnectionlessCteTransmitParametersInfo;

typedef struct {
    int result;
    uint8_t advHandle;
    uint8_t cteEnable;
} GapLeSetConnectionlessCteTransmitEnableInfo;

typedef struct {
    int result;
    GapLeSetConnectionlessIqSamplingEnableParam param;
    uint8_t antennaIds[GAP_LE_SWITCHING_PATTERN_LENGTH_MAX];
} GapLeSetConnectionlessIqSamplingEnableInfo;

typedef struct {
    int result;
    uint16_t connectionHandle;
    uint8_t samplingEnable;
    uint8_t slotDurations;
    uint8_t lengthOfSwitchingPattern;
    uint8_t antennaIds[GAP_LE_SWITCHING_PATTERN_LENGTH_MAX];
} GapLeSetConnectionCteReceiveParametersInfo;

typedef struct {
    int result;
    uint16_t connectionHandle;
    uint8_t cteTypes;
    uint8_t lengthOfSwitchingPattern;
    uint8_t antennaIds[GAP_LE_SWITCHING_PATTERN_LENGTH_MAX];
} GapLeSetConnectionCteTransmitParametersInfo;

typedef struct {
    int result;
    uint16_t connectionHandle;
    uint8_t enable;
    uint16_t cteRequestInterval;
    uint8_t requestedCteLength;
    uint8_t requestedCteType;
} GapLeConnectionCteRequestEnableInfo;

typedef struct {
    int result;
    uint16_t connectionHandle;
    uint8_t enable;
} GapLeConnectionCteResponseEnableInfo;

typedef struct {
    int result;
} GapLeReadAntennaInformationInfo;

typedef struct {
    int result;
    uint16_t syncHandle;
    uint8_t enable;
} GapLeSetPeriodicAdvertisingReceiveEnableInfo;

typedef struct {
    int result;
    uint16_t connectionHandle;
    uint16_t serviceData;
    uint16_t syncHandle;
} GapLePeriodicAdvertisingSyncTransferInfo;

typedef struct {
    int result;
    uint16_t connectionHandle;
    uint16_t serviceData;
    uint8_t advertisingHandle;
} GapLePeriodicAdvertisingSetInfoTransferInfo;

typedef struct {
    int result;
    uint16_t connectionHandle;
    uint8_t mode;
    uint16_t skip;
    uint16_t syncTimeout;
    uint8_t cteType;
} GapLeSetPeriodicAdvertisingSyncTransferParametersInfo;

typedef struct {
    int result;
    uint8_t mode;
    uint16_t skip;
    uint16_t syncTimeout;
    uint8_t cteType;
} GapLeSetDefaultPeriodicAdvertisingSyncTransferParametersInfo;

typedef struct {
    int result;
    const GapLeCteCallback *callback;
    void *context;
} GapLeCteCallbackInfo;

// Returns true when the switching pattern length and pointer are consistent:
// either length 0 with NULL antennaIds, or a valid length (0x02-0x4B) with a
// non-NULL antennaIds array. Rules match GapLeCteAntennaIdsCheck.
static bool GapLeIf5_1AntennaIdsValid(uint8_t lengthOfSwitchingPattern, const uint8_t *antennaIds)
{
    if (lengthOfSwitchingPattern > GAP_LE_SWITCHING_PATTERN_LENGTH_MAX) {
        return false;
    }

    if (lengthOfSwitchingPattern > 0 &&
        (lengthOfSwitchingPattern < GAP_LE_SWITCHING_PATTERN_LENGTH_MIN || antennaIds == NULL)) {
        return false;
    }

    if (lengthOfSwitchingPattern == 0 && antennaIds != NULL) {
        return false;
    }

    return true;
}

static void GapLeReceiverTestV3Task(void *ctx)
{
    GapLeReceiverTestV3Info *info = ctx;
    info->result = GAP_LeReceiverTestV3(&info->param);
}

int GAPIF_LeReceiverTestV3(const GapLeReceiverTestV3Param *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    if (param->rxChannel > GAP_LE_TEST_CHANNEL_MAX ||
        (param->phy != GAP_LE_PHY_1M && param->phy != GAP_LE_PHY_2M && param->phy != GAP_LE_PHY_CODED) ||
        param->modulationIndex > GAP_LE_TEST_MODULATION_INDEX_MAX ||
        param->expectedCteLength > GAP_LE_CTE_LENGTH_MAX ||
        (param->expectedCteType != GAP_LE_CTE_TYPE_AOA && param->expectedCteType != GAP_LE_CTE_TYPE_AOD_1US &&
         param->expectedCteType != GAP_LE_CTE_TYPE_AOD_2US) ||
        (param->slotDurations != GAP_LE_CTE_SLOT_DURATIONS_1US &&
         param->slotDurations != GAP_LE_CTE_SLOT_DURATIONS_2US) ||
        !GapLeIf5_1AntennaIdsValid(param->lengthOfSwitchingPattern, param->antennaIds)) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: rxChannel:%hhu, phy:%hhu", __FUNCTION__, param->rxChannel, param->phy);
    GapLeReceiverTestV3Info *ctx = MEM_MALLOC.alloc(sizeof(GapLeReceiverTestV3Info));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeReceiverTestV3Info), 0x00, sizeof(GapLeReceiverTestV3Info)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->param = *param;
    if (param->lengthOfSwitchingPattern > 0) {
        if (memcpy_s(ctx->antennaIds, sizeof(ctx->antennaIds), param->antennaIds,
            param->lengthOfSwitchingPattern) != EOK) {
            MEM_MALLOC.free(ctx);
            ctx = NULL;
            return BT_OPERATION_FAILED;
        }
        ctx->param.antennaIds = ctx->antennaIds;
    } else {
        ctx->param.antennaIds = NULL;
    }

    int ret = GapRunTaskBlockProcess(GapLeReceiverTestV3Task, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeTransmitterTestV3Task(void *ctx)
{
    GapLeTransmitterTestV3Info *info = ctx;
    info->result = GAP_LeTransmitterTestV3(&info->param);
}

int GAPIF_LeTransmitterTestV3(const GapLeTransmitterTestV3Param *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    if (param->txChannel > GAP_LE_TEST_CHANNEL_MAX || param->lengthOfTestData > GAP_LE_TEST_DATA_LENGTH_MAX ||
        param->packetPayload > GAP_LE_TEST_PACKET_PAYLOAD_MAX ||
        (param->phy != GAP_LE_PHY_1M && param->phy != GAP_LE_PHY_2M && param->phy != GAP_LE_PHY_CODED &&
         param->phy != GAP_LE_TEST_PHY_CODED_S2) ||
        param->cteLength > GAP_LE_CTE_LENGTH_MAX ||
        (param->cteType != GAP_LE_CTE_TYPE_AOA && param->cteType != GAP_LE_CTE_TYPE_AOD_1US &&
         param->cteType != GAP_LE_CTE_TYPE_AOD_2US) ||
        !GapLeIf5_1AntennaIdsValid(param->lengthOfSwitchingPattern, param->antennaIds)) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: txChannel:%hhu, phy:%hhu", __FUNCTION__, param->txChannel, param->phy);
    GapLeTransmitterTestV3Info *ctx = MEM_MALLOC.alloc(sizeof(GapLeTransmitterTestV3Info));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeTransmitterTestV3Info), 0x00, sizeof(GapLeTransmitterTestV3Info)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->param = *param;
    if (param->lengthOfSwitchingPattern > 0) {
        if (memcpy_s(ctx->antennaIds, sizeof(ctx->antennaIds), param->antennaIds,
            param->lengthOfSwitchingPattern) != EOK) {
            MEM_MALLOC.free(ctx);
            ctx = NULL;
            return BT_OPERATION_FAILED;
        }
        ctx->param.antennaIds = ctx->antennaIds;
    } else {
        ctx->param.antennaIds = NULL;
    }

    int ret = GapRunTaskBlockProcess(GapLeTransmitterTestV3Task, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeSetConnectionlessCteTransmitParametersTask(void *ctx)
{
    GapLeSetConnectionlessCteTransmitParametersInfo *info = ctx;
    info->result = GAP_LeSetConnectionlessCteTransmitParameters(&info->param);
}

int GAPIF_LeSetConnectionlessCteTransmitParameters(
    const GapLeSetConnectionlessCteTransmitParametersParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    if (param->advHandle > GAP_LE_ADV_HANDLE_MAX ||
        param->cteLength < GAP_LE_CTE_LENGTH_MIN || param->cteLength > GAP_LE_CTE_LENGTH_MAX ||
        (param->cteType != GAP_LE_CTE_TYPE_AOA && param->cteType != GAP_LE_CTE_TYPE_AOD_1US &&
         param->cteType != GAP_LE_CTE_TYPE_AOD_2US) ||
        param->cteCount < GAP_LE_CTE_COUNT_MIN || param->cteCount > GAP_LE_CTE_COUNT_MAX ||
        !GapLeIf5_1AntennaIdsValid(param->lengthOfSwitchingPattern, param->antennaIds)) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: advHandle:%hhu, cteType:%hhu", __FUNCTION__, param->advHandle, param->cteType);
    GapLeSetConnectionlessCteTransmitParametersInfo *ctx =
        MEM_MALLOC.alloc(sizeof(GapLeSetConnectionlessCteTransmitParametersInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeSetConnectionlessCteTransmitParametersInfo), 0x00,
        sizeof(GapLeSetConnectionlessCteTransmitParametersInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->param = *param;
    if (param->lengthOfSwitchingPattern > 0) {
        if (memcpy_s(ctx->antennaIds, sizeof(ctx->antennaIds), param->antennaIds,
            param->lengthOfSwitchingPattern) != EOK) {
            MEM_MALLOC.free(ctx);
            ctx = NULL;
            return BT_OPERATION_FAILED;
        }
        ctx->param.antennaIds = ctx->antennaIds;
    } else {
        ctx->param.antennaIds = NULL;
    }

    int ret = GapRunTaskBlockProcess(GapLeSetConnectionlessCteTransmitParametersTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeSetConnectionlessCteTransmitEnableTask(void *ctx)
{
    GapLeSetConnectionlessCteTransmitEnableInfo *info = ctx;
    info->result = GAP_LeSetConnectionlessCteTransmitEnable(info->advHandle, info->cteEnable);
}

int GAPIF_LeSetConnectionlessCteTransmitEnable(uint8_t advHandle, uint8_t cteEnable)
{
    if (advHandle > GAP_LE_ADV_HANDLE_MAX || cteEnable > GAP_PERIODIC_ADV_ENABLE_TRUE) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: advHandle:%hhu, enable:%hhu", __FUNCTION__, advHandle, cteEnable);
    GapLeSetConnectionlessCteTransmitEnableInfo *ctx =
        MEM_MALLOC.alloc(sizeof(GapLeSetConnectionlessCteTransmitEnableInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeSetConnectionlessCteTransmitEnableInfo), 0x00,
        sizeof(GapLeSetConnectionlessCteTransmitEnableInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->advHandle = advHandle;
    ctx->cteEnable = cteEnable;

    int ret = GapRunTaskBlockProcess(GapLeSetConnectionlessCteTransmitEnableTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeSetConnectionlessIqSamplingEnableTask(void *ctx)
{
    GapLeSetConnectionlessIqSamplingEnableInfo *info = ctx;
    info->result = GAP_LeSetConnectionlessIqSamplingEnable(&info->param);
}

int GAPIF_LeSetConnectionlessIqSamplingEnable(const GapLeSetConnectionlessIqSamplingEnableParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    if ((param->syncHandle > GAP_PERIODIC_ADV_SYNC_HANDLE_MAX && param->syncHandle != GAP_LE_RX_TEST_SYNC_HANDLE) ||
        param->samplingEnable > GAP_PERIODIC_ADV_ENABLE_TRUE ||
        (param->slotDurations != GAP_LE_CTE_SLOT_DURATIONS_1US &&
         param->slotDurations != GAP_LE_CTE_SLOT_DURATIONS_2US) ||
        // Max_Sampled_CTEs is 0x00 (sample all CTEs) or 0x01-0x10 (7.8.82).
        param->maxSampledCtes > GAP_LE_CTE_COUNT_MAX ||
        !GapLeIf5_1AntennaIdsValid(param->lengthOfSwitchingPattern, param->antennaIds)) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: syncHandle:0x%04x", __FUNCTION__, param->syncHandle);
    GapLeSetConnectionlessIqSamplingEnableInfo *ctx =
        MEM_MALLOC.alloc(sizeof(GapLeSetConnectionlessIqSamplingEnableInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeSetConnectionlessIqSamplingEnableInfo), 0x00,
        sizeof(GapLeSetConnectionlessIqSamplingEnableInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->param = *param;
    if (param->lengthOfSwitchingPattern > 0) {
        if (memcpy_s(ctx->antennaIds, sizeof(ctx->antennaIds), param->antennaIds,
            param->lengthOfSwitchingPattern) != EOK) {
            MEM_MALLOC.free(ctx);
            ctx = NULL;
            return BT_OPERATION_FAILED;
        }
        ctx->param.antennaIds = ctx->antennaIds;
    } else {
        ctx->param.antennaIds = NULL;
    }

    int ret = GapRunTaskBlockProcess(GapLeSetConnectionlessIqSamplingEnableTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeSetConnectionCteReceiveParametersTask(void *ctx)
{
    GapLeSetConnectionCteReceiveParametersInfo *info = ctx;
    const uint8_t *antennaIds = (info->lengthOfSwitchingPattern > 0) ? info->antennaIds : NULL;
    info->result = GAP_LeSetConnectionCteReceiveParameters(info->connectionHandle,
        info->samplingEnable,
        info->slotDurations,
        info->lengthOfSwitchingPattern,
        antennaIds);
}

int GAPIF_LeSetConnectionCteReceiveParameters(uint16_t connectionHandle, uint8_t samplingEnable,
    uint8_t slotDurations, uint8_t lengthOfSwitchingPattern, const uint8_t *antennaIds)
{
    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX || samplingEnable > GAP_PERIODIC_ADV_ENABLE_TRUE ||
        (slotDurations != GAP_LE_CTE_SLOT_DURATIONS_1US && slotDurations != GAP_LE_CTE_SLOT_DURATIONS_2US) ||
        !GapLeIf5_1AntennaIdsValid(lengthOfSwitchingPattern, antennaIds)) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: connHandle:0x%04x", __FUNCTION__, connectionHandle);
    GapLeSetConnectionCteReceiveParametersInfo *ctx =
        MEM_MALLOC.alloc(sizeof(GapLeSetConnectionCteReceiveParametersInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeSetConnectionCteReceiveParametersInfo), 0x00,
        sizeof(GapLeSetConnectionCteReceiveParametersInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->connectionHandle = connectionHandle;
    ctx->samplingEnable = samplingEnable;
    ctx->slotDurations = slotDurations;
    ctx->lengthOfSwitchingPattern = lengthOfSwitchingPattern;
    if (lengthOfSwitchingPattern > 0) {
        if (memcpy_s(ctx->antennaIds, sizeof(ctx->antennaIds), antennaIds, lengthOfSwitchingPattern) != EOK) {
            MEM_MALLOC.free(ctx);
            ctx = NULL;
            return BT_OPERATION_FAILED;
        }
    }

    int ret = GapRunTaskBlockProcess(GapLeSetConnectionCteReceiveParametersTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeSetConnectionCteTransmitParametersTask(void *ctx)
{
    GapLeSetConnectionCteTransmitParametersInfo *info = ctx;
    const uint8_t *antennaIds = (info->lengthOfSwitchingPattern > 0) ? info->antennaIds : NULL;
    info->result = GAP_LeSetConnectionCteTransmitParameters(
        info->connectionHandle, info->cteTypes, info->lengthOfSwitchingPattern, antennaIds);
}

int GAPIF_LeSetConnectionCteTransmitParameters(uint16_t connectionHandle, uint8_t cteTypes,
    uint8_t lengthOfSwitchingPattern, const uint8_t *antennaIds)
{
    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX ||
        (cteTypes & ~GAP_LE_PAST_CTE_TYPE_NO_CTE_MASK_ALL) != 0 ||
        !GapLeIf5_1AntennaIdsValid(lengthOfSwitchingPattern, antennaIds)) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: connHandle:0x%04x, cteTypes:0x%02x", __FUNCTION__, connectionHandle, cteTypes);
    GapLeSetConnectionCteTransmitParametersInfo *ctx =
        MEM_MALLOC.alloc(sizeof(GapLeSetConnectionCteTransmitParametersInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeSetConnectionCteTransmitParametersInfo), 0x00,
        sizeof(GapLeSetConnectionCteTransmitParametersInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->connectionHandle = connectionHandle;
    ctx->cteTypes = cteTypes;
    ctx->lengthOfSwitchingPattern = lengthOfSwitchingPattern;
    if (lengthOfSwitchingPattern > 0) {
        if (memcpy_s(ctx->antennaIds, sizeof(ctx->antennaIds), antennaIds, lengthOfSwitchingPattern) != EOK) {
            MEM_MALLOC.free(ctx);
            ctx = NULL;
            return BT_OPERATION_FAILED;
        }
    }

    int ret = GapRunTaskBlockProcess(GapLeSetConnectionCteTransmitParametersTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeConnectionCteRequestEnableTask(void *ctx)
{
    GapLeConnectionCteRequestEnableInfo *info = ctx;
    info->result = GAP_LeConnectionCteRequestEnable(info->connectionHandle,
        info->enable,
        info->cteRequestInterval,
        info->requestedCteLength,
        info->requestedCteType);
}

int GAPIF_LeConnectionCteRequestEnable(
    uint16_t connectionHandle, uint8_t enable, uint16_t cteRequestInterval, uint8_t requestedCteLength,
    uint8_t requestedCteType)
{
    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX || enable > GAP_PERIODIC_ADV_ENABLE_TRUE ||
        requestedCteLength > GAP_LE_CTE_LENGTH_MAX ||
        (requestedCteType != GAP_LE_CTE_TYPE_AOA && requestedCteType != GAP_LE_CTE_TYPE_AOD_1US &&
         requestedCteType != GAP_LE_CTE_TYPE_AOD_2US)) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: connHandle:0x%04x", __FUNCTION__, connectionHandle);
    GapLeConnectionCteRequestEnableInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeConnectionCteRequestEnableInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeConnectionCteRequestEnableInfo), 0x00,
        sizeof(GapLeConnectionCteRequestEnableInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->connectionHandle = connectionHandle;
    ctx->enable = enable;
    ctx->cteRequestInterval = cteRequestInterval;
    ctx->requestedCteLength = requestedCteLength;
    ctx->requestedCteType = requestedCteType;

    int ret = GapRunTaskBlockProcess(GapLeConnectionCteRequestEnableTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeConnectionCteResponseEnableTask(void *ctx)
{
    GapLeConnectionCteResponseEnableInfo *info = ctx;
    info->result = GAP_LeConnectionCteResponseEnable(info->connectionHandle, info->enable);
}

int GAPIF_LeConnectionCteResponseEnable(uint16_t connectionHandle, uint8_t enable)
{
    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX || enable > GAP_PERIODIC_ADV_ENABLE_TRUE) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: connHandle:0x%04x", __FUNCTION__, connectionHandle);
    GapLeConnectionCteResponseEnableInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeConnectionCteResponseEnableInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeConnectionCteResponseEnableInfo), 0x00,
        sizeof(GapLeConnectionCteResponseEnableInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->connectionHandle = connectionHandle;
    ctx->enable = enable;

    int ret = GapRunTaskBlockProcess(GapLeConnectionCteResponseEnableTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeReadAntennaInformationTask(void *ctx)
{
    GapLeReadAntennaInformationInfo *info = ctx;
    info->result = GAP_LeReadAntennaInformation();
}

int GAPIF_LeReadAntennaInformation(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    GapLeReadAntennaInformationInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeReadAntennaInformationInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeReadAntennaInformationInfo), 0x00, sizeof(GapLeReadAntennaInformationInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    int ret = GapRunTaskBlockProcess(GapLeReadAntennaInformationTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeSetPeriodicAdvertisingReceiveEnableTask(void *ctx)
{
    GapLeSetPeriodicAdvertisingReceiveEnableInfo *info = ctx;
    info->result = GAP_LeSetPeriodicAdvertisingReceiveEnable(info->syncHandle, info->enable);
}

int GAPIF_LeSetPeriodicAdvertisingReceiveEnable(uint16_t syncHandle, uint8_t enable)
{
    if (syncHandle > GAP_PERIODIC_ADV_SYNC_HANDLE_MAX || enable > GAP_PERIODIC_ADV_ENABLE_TRUE) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: syncHandle:0x%04x", __FUNCTION__, syncHandle);
    GapLeSetPeriodicAdvertisingReceiveEnableInfo *ctx =
        MEM_MALLOC.alloc(sizeof(GapLeSetPeriodicAdvertisingReceiveEnableInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeSetPeriodicAdvertisingReceiveEnableInfo), 0x00,
        sizeof(GapLeSetPeriodicAdvertisingReceiveEnableInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->syncHandle = syncHandle;
    ctx->enable = enable;

    int ret = GapRunTaskBlockProcess(GapLeSetPeriodicAdvertisingReceiveEnableTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLePeriodicAdvertisingSyncTransferTask(void *ctx)
{
    GapLePeriodicAdvertisingSyncTransferInfo *info = ctx;
    info->result = GAP_LePeriodicAdvertisingSyncTransfer(info->connectionHandle, info->serviceData, info->syncHandle);
}

int GAPIF_LePeriodicAdvertisingSyncTransfer(uint16_t connectionHandle, uint16_t serviceData, uint16_t syncHandle)
{
    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX || syncHandle > GAP_PERIODIC_ADV_SYNC_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: connHandle:0x%04x, syncHandle:0x%04x", __FUNCTION__, connectionHandle, syncHandle);
    GapLePeriodicAdvertisingSyncTransferInfo *ctx =
        MEM_MALLOC.alloc(sizeof(GapLePeriodicAdvertisingSyncTransferInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLePeriodicAdvertisingSyncTransferInfo), 0x00,
        sizeof(GapLePeriodicAdvertisingSyncTransferInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->connectionHandle = connectionHandle;
    ctx->serviceData = serviceData;
    ctx->syncHandle = syncHandle;

    int ret = GapRunTaskBlockProcess(GapLePeriodicAdvertisingSyncTransferTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLePeriodicAdvertisingSetInfoTransferTask(void *ctx)
{
    GapLePeriodicAdvertisingSetInfoTransferInfo *info = ctx;
    info->result = GAP_LePeriodicAdvertisingSetInfoTransfer(
        info->connectionHandle, info->serviceData, info->advertisingHandle);
}

int GAPIF_LePeriodicAdvertisingSetInfoTransfer(uint16_t connectionHandle, uint16_t serviceData,
    uint8_t advertisingHandle)
{
    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX || advertisingHandle > GAP_LE_ADV_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: connHandle:0x%04x, advHandle:%hhu", __FUNCTION__, connectionHandle, advertisingHandle);
    GapLePeriodicAdvertisingSetInfoTransferInfo *ctx =
        MEM_MALLOC.alloc(sizeof(GapLePeriodicAdvertisingSetInfoTransferInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLePeriodicAdvertisingSetInfoTransferInfo), 0x00,
        sizeof(GapLePeriodicAdvertisingSetInfoTransferInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->connectionHandle = connectionHandle;
    ctx->serviceData = serviceData;
    ctx->advertisingHandle = advertisingHandle;

    int ret = GapRunTaskBlockProcess(GapLePeriodicAdvertisingSetInfoTransferTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeSetPeriodicAdvertisingSyncTransferParametersTask(void *ctx)
{
    GapLeSetPeriodicAdvertisingSyncTransferParametersInfo *info = ctx;
    info->result = GAP_LeSetPeriodicAdvertisingSyncTransferParameters(
        info->connectionHandle, info->mode, info->skip, info->syncTimeout, info->cteType);
}

int GAPIF_LeSetPeriodicAdvertisingSyncTransferParameters(
    uint16_t connectionHandle, uint8_t mode, uint16_t skip, uint16_t syncTimeout, uint8_t cteType)
{
    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX || mode > GAP_LE_PAST_MODE_SYNC_REPORT ||
        skip > GAP_PERIODIC_ADV_SKIP_MAX ||
        syncTimeout < GAP_PERIODIC_ADV_SYNC_TIMEOUT_MIN || syncTimeout > GAP_PERIODIC_ADV_SYNC_TIMEOUT_MAX ||
        (cteType & ~GAP_LE_PAST_CTE_TYPE_NO_CTE_MASK_ALL) != 0) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: connHandle:0x%04x, mode:%hhu", __FUNCTION__, connectionHandle, mode);
    GapLeSetPeriodicAdvertisingSyncTransferParametersInfo *ctx =
        MEM_MALLOC.alloc(sizeof(GapLeSetPeriodicAdvertisingSyncTransferParametersInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeSetPeriodicAdvertisingSyncTransferParametersInfo), 0x00,
        sizeof(GapLeSetPeriodicAdvertisingSyncTransferParametersInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->connectionHandle = connectionHandle;
    ctx->mode = mode;
    ctx->skip = skip;
    ctx->syncTimeout = syncTimeout;
    ctx->cteType = cteType;

    int ret = GapRunTaskBlockProcess(GapLeSetPeriodicAdvertisingSyncTransferParametersTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapLeSetDefaultPeriodicAdvertisingSyncTransferParametersTask(void *ctx)
{
    GapLeSetDefaultPeriodicAdvertisingSyncTransferParametersInfo *info = ctx;
    info->result = GAP_LeSetDefaultPeriodicAdvertisingSyncTransferParameters(
        info->mode, info->skip, info->syncTimeout, info->cteType);
}

int GAPIF_LeSetDefaultPeriodicAdvertisingSyncTransferParameters(uint8_t mode, uint16_t skip, uint16_t syncTimeout,
    uint8_t cteType)
{
    if (mode > GAP_LE_PAST_MODE_SYNC_REPORT || skip > GAP_PERIODIC_ADV_SKIP_MAX ||
        syncTimeout < GAP_PERIODIC_ADV_SYNC_TIMEOUT_MIN || syncTimeout > GAP_PERIODIC_ADV_SYNC_TIMEOUT_MAX ||
        (cteType & ~GAP_LE_PAST_CTE_TYPE_NO_CTE_MASK_ALL) != 0) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: mode:%hhu", __FUNCTION__, mode);
    GapLeSetDefaultPeriodicAdvertisingSyncTransferParametersInfo *ctx =
        MEM_MALLOC.alloc(sizeof(GapLeSetDefaultPeriodicAdvertisingSyncTransferParametersInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeSetDefaultPeriodicAdvertisingSyncTransferParametersInfo), 0x00,
        sizeof(GapLeSetDefaultPeriodicAdvertisingSyncTransferParametersInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->mode = mode;
    ctx->skip = skip;
    ctx->syncTimeout = syncTimeout;
    ctx->cteType = cteType;

    int ret = GapRunTaskBlockProcess(GapLeSetDefaultPeriodicAdvertisingSyncTransferParametersTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapRegisterLeCteCallbackTask(void *ctx)
{
    GapLeCteCallbackInfo *info = ctx;
    info->result = GAP_LeRegisterCteCallback(info->callback, info->context);
}

int GAPIF_RegisterLeCteCallback(const GapLeCteCallback *callback, void *context)
{
    if (callback == NULL) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s:", __FUNCTION__);
    GapLeCteCallbackInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeCteCallbackInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeCteCallbackInfo), 0x00, sizeof(GapLeCteCallbackInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    ctx->callback = callback;
    ctx->context = context;

    int ret = GapRunTaskBlockProcess(GapRegisterLeCteCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

static void GapDeregisterLeCteCallbackTask(void *ctx)
{
    GapLeCteCallbackInfo *info = ctx;
    info->result = GAP_LeDeregisterCteCallback();
}

int GAPIF_DeregisterLeCteCallback(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    GapLeCteCallbackInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeCteCallbackInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    if (memset_s(ctx, sizeof(GapLeCteCallbackInfo), 0x00, sizeof(GapLeCteCallbackInfo)) != EOK) {
        MEM_MALLOC.free(ctx);
        ctx = NULL;
        return BT_OPERATION_FAILED;
    }

    int ret = GapRunTaskBlockProcess(GapDeregisterLeCteCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    ctx = NULL;
    return ret;
}

#endif
