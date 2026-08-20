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
#include "gap_task_internal.h"

#include <sched.h>
#include <securec.h>

#include "allocator.h"
#include "log.h"
#include "platform/include/event.h"
#include "platform/include/mutex.h"

#include "btm.h"
#include "btm/btm_controller.h"
#include "btm/btm_thread.h"

#ifdef GAP_LE_SUPPORT

typedef struct {
    GapLeConnCallback callback;
    void *context;
} LeConnUpdateCallback;

static LeConnUpdateCallback g_leConnUpdateCallback;
static Mutex *g_leConnUpdateCallbackMutex = NULL;
// Number of in-flight callbacks for the connection-update callback group.
// Incremented while the inner mutex is held in the getter, decremented by the
// caller after the callback returns. Deinit spins until this reaches zero.
static int32_t g_leConnUpdateCallbackRef = 0;

// Protects creation/destruction of the callback mutexes and concurrent reads of
// their global pointers. Created once in GapLeCallbackInit and intentionally
// never destroyed to avoid use-after-free during teardown races.
// Contract: GapLeCallbackInit and GapLeCallbackDeinit must be called serially
// by a single thread/task. After Deinit, all GAP LE callback registration/lookup
// APIs return GAP_ERR_OUT_OF_RES until Init is called again.
static Mutex *g_leCallbackLifecycleMutex = NULL;
// Set while GapLeCallbackDeinit is tearing the inner mutexes down. Registration
// and lookup APIs must fail instead of re-creating a mutex that is about to be
// deleted.
static bool g_leCallbackDeinitInProgress = false;
// Signaled when either reference count reaches zero so Deinit can avoid spinning.
static Event *g_leCallbackDeinitEvent = NULL;

void GapWriteAuthenticatedPayloadTimeoutComplete(const HciWriteAuthenticatedPayloadTimeoutReturnParam *param)
{
    if (param->status != HCI_SUCCESS) {
        LOG_WARN("%{public}s:handle:0x%04x status:0x%02x", __FUNCTION__, param->connectionHandle, param->status);
    }
}

void GapOnAuthenticatedPayloadTimeoutExpiredEvent(const HciAuthenticatedPayloadTimeoutExpiredEventParam *eventParam)
{
    LOG_WARN("%{public}s:handle:0x%04x", __FUNCTION__, eventParam->connectionHandle);
}

int GAP_RegisterLeConnCallback(const GapLeConnCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "NULL");
    // Read the lifecycle mutex atomically: GapLeCallbackInit publishes it with
    // __atomic_compare_exchange_n, so a plain load here is a data race (benign
    // in practice since the mutex is never destroyed, but flagged by static
    // analyzers). Once obtained, the pointer stays valid for the function's
    // lifetime; the subsequent Unlock sites still reference the global for
    // consistency with the rest of the file.
    Mutex *lifecycleMutex = __atomic_load_n(&g_leCallbackLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(lifecycleMutex);
    if (g_leCallbackDeinitInProgress) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }
    Mutex *mutex = g_leConnUpdateCallbackMutex;
    if (mutex == NULL) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(mutex);
    if (callback == NULL) {
        (void)memset_s(&g_leConnUpdateCallback.callback,
            sizeof(g_leConnUpdateCallback.callback),
            0x00,
            sizeof(g_leConnUpdateCallback.callback));
    } else {
        g_leConnUpdateCallback.callback = *callback;
    }
    g_leConnUpdateCallback.context = context;
    MutexUnlock(mutex);
    MutexUnlock(g_leCallbackLifecycleMutex);
    return GAP_SUCCESS;
}

int GAP_DeregisterLeConnCallback(void)
{
    Mutex *lifecycleMutex = __atomic_load_n(&g_leCallbackLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(lifecycleMutex);
    if (g_leCallbackDeinitInProgress) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }
    Mutex *mutex = g_leConnUpdateCallbackMutex;
    if (mutex == NULL) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(mutex);
    (void)memset_s(&g_leConnUpdateCallback.callback,
        sizeof(g_leConnUpdateCallback.callback),
        0x00,
        sizeof(g_leConnUpdateCallback.callback));
    g_leConnUpdateCallback.context = NULL;
    MutexUnlock(mutex);
    MutexUnlock(g_leCallbackLifecycleMutex);
    return GAP_SUCCESS;
}

static int GapLeConnectionUpdate(uint16_t handle, const GapLeConnectionParameter *connParam)
{
    HciLeConnectionUpdateParam hciCmdParam = {
        .connectionHandle = handle,
        .connIntervalMin = connParam->connIntervalMin,
        .connIntervalMax = connParam->connIntervalMax,
        .connLatency = connParam->connLatency,
        .supervisionTimeout = connParam->timeout,
        .minimumCELength = connParam->minCeLen,
        .maximumCELength = connParam->maxCeLen,
    };

    return HCI_LeConnectionUpdate(&hciCmdParam);
}

// The context pointer returned by the getters is owned by the upper layer and must remain valid
// until the callback is deregistered and all in-flight events have been delivered.
// The lifecycle mutex is only held during lookup; it is released before the callback runs.
static bool GapLeConnUpdateCallbackGet(GapLeConnCallback *callback, void **context)
{
    if (callback == NULL || context == NULL) {
        if (callback != NULL) {
            (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        }
        if (context != NULL) {
            *context = NULL;
        }
        return false;
    }

    Mutex *lifecycleMutex = __atomic_load_n(&g_leCallbackLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        *context = NULL;
        return false;
    }

    MutexLock(lifecycleMutex);
    if (g_leCallbackDeinitInProgress) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        *context = NULL;
        return false;
    }

    Mutex *mutex = g_leConnUpdateCallbackMutex;
    if (mutex == NULL) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        *context = NULL;
        return false;
    }

    MutexLock(mutex);
    *callback = g_leConnUpdateCallback.callback;
    *context = g_leConnUpdateCallback.context;
    (void)__atomic_fetch_add(&g_leConnUpdateCallbackRef, 1, __ATOMIC_SEQ_CST);
    MutexUnlock(mutex);
    MutexUnlock(g_leCallbackLifecycleMutex);
    return true;
}

static void GapLeConnUpdateCallbackRelease(void)
{
    if (__atomic_fetch_sub(&g_leConnUpdateCallbackRef, 1, __ATOMIC_SEQ_CST) == 1) {
        Event *event = __atomic_load_n(&g_leCallbackDeinitEvent, __ATOMIC_ACQUIRE);
        if (event != NULL) {
            EventSet(event);
        }
    }
}

void GapOnLeConnectionUpdateCompleteEvent(const HciLeConnectionUpdateCompleteEventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }

    BtAddr addr = {0};

    LeConnectionInfoBlock *connectionInfoBlock = GapGetLeConnectionInfoBlock();
    if (connectionInfoBlock == NULL) {
        LOG_WARN("%{public}s: connection info block not initialized", __FUNCTION__);
        return;
    }
    LeDeviceInfo *deviceInfo = NULL;
    deviceInfo = ListForEachData(
        connectionInfoBlock->deviceList, GapFindLeConnectionDeviceByHandle, (void *)&eventParam->connectionHandle);
    if (deviceInfo != NULL) {
        (void)memcpy_s(&addr, sizeof(BtAddr), &deviceInfo->addr, sizeof(BtAddr));
        if (deviceInfo->paramUpdateReq != NULL) {
            MEM_MALLOC.free(deviceInfo->paramUpdateReq);
            deviceInfo->paramUpdateReq = NULL;
        }
    }

    if (deviceInfo != NULL) {
        GapLeConnCallback callback;
        void *context = NULL;
        if (GapLeConnUpdateCallbackGet(&callback, &context)) {
            if (callback.leConnectionUpdateComplete) {
                callback.leConnectionUpdateComplete(eventParam->status,
                    &addr,
                    eventParam->connInterval,
                    eventParam->connLatency,
                    eventParam->supervisionTimeout,
                    context);
            }
            GapLeConnUpdateCallbackRelease();
        }
    }
}

int GAP_LeConnParamUpdate(const BtAddr *addr, const GapLeConnectionParameter *connParam)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (addr == NULL || connParam == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_PERIPHERAL) == false) {
        return GAP_ERR_INVAL_STATE;
    }

    LeConnectionInfoBlock *connectionInfoBlock = GapGetLeConnectionInfoBlock();
    if (connectionInfoBlock == NULL) {
        return GAP_ERR_INVAL_STATE;
    }

    LeDeviceInfo *deviceInfo = ListForEachData(
        connectionInfoBlock->deviceList, GapFindLeConnectionDeviceByAddr, (void *)addr);
    if (deviceInfo == NULL) {
        return GAP_ERR_INVAL_STATE;
    }

    if (deviceInfo->role == LE_CONNECTION_ROLE_MASTER) {
        return GapLeConnectionUpdate(deviceInfo->handle, connParam);
    }

    if (deviceInfo->role == LE_CONNECTION_ROLE_SLAVE) {
        // HCI_LE_Connection_Parameter_Request is not implemented in this stack. Peripheral-initiated
        // parameter updates therefore use the L2CAP Connection Parameter Update Request, which is valid
        // regardless of the controller's Connection Parameter Request Procedure support.
        L2capLeConnectionParameter parameter = {
            .connIntervalMax = connParam->connIntervalMax,
            .connIntervalMin = connParam->connIntervalMin,
            .connLatency = connParam->connLatency,
            .supervisionTimeout = connParam->timeout,
        };
        return GapLeConnectionParameterUpdateReq(deviceInfo->handle, &parameter);
    }

    return GAP_ERR_INVAL_STATE;
}

static int GapLeRemoteConnectionParameterRequestNegativeReply(uint16_t handle, uint8_t reason)
{
    HciLeRemoteConnectionParameterRequestNegativeReplyParam hciCmdParam = {
        .connectionHandle = handle,
        .reason = reason,
    };

    return HCI_LeRemoteConnectionParameterRequestNegativeReply(&hciCmdParam);
}

void GapLeRemoteConnectionParameterRequestNegativeReplyComplete(
    const HciLeRemoteConnectionParameterRequestNegativeReplyReturnParam *param)
{
    if (param->status) {
        LOG_ERROR("%{public}s:status:%02x, handle:%04x", __FUNCTION__, param->status, param->connectionHandle);
    }
}

static int GapLeRemoteConnectionParameterRequestReply(uint16_t handle, const GapLeConnectionParameter *connParam)
{
    HciLeRemoteConnectionParameterRequestReplyParam hciCmdParam = {
        .connectionHandle = handle,
        .intervalMin = connParam->connIntervalMin,
        .intervalMax = connParam->connIntervalMax,
        .latency = connParam->connLatency,
        .timeout = connParam->timeout,
        .minimumCELength = connParam->minCeLen,
        .maximumCELength = connParam->maxCeLen,
    };

    return HCI_LeRemoteConnectionParameterRequestReply(&hciCmdParam);
}

void GapLeRemoteConnectionParameterRequestReplyComplete(
    const HciLeRemoteConnectionParameterRequestReplyReturnParam *param)
{
    if (param->status) {
        LOG_WARN("%{public}s:status:%02x, handle:%04x", __FUNCTION__, param->status, param->connectionHandle);
        GapLeRemoteConnectionParameterRequestNegativeReply(param->connectionHandle, HCI_UNSPECIFIED_ERROR);
    }
}

void GapOnLeRemoteConnectionParameterRequestEvent(const HciLeRemoteConnectionParameterRequestEventParam *eventParam)
{
    BtAddr addr = {0};
    bool doReject = false;

    LeConnectionInfoBlock *connectionInfoBlock = GapGetLeConnectionInfoBlock();
    LeDeviceInfo *deviceInfo = NULL;
    deviceInfo = ListForEachData(
        connectionInfoBlock->deviceList, GapFindLeConnectionDeviceByHandle, (void *)&eventParam->connectionHandle);
    if (deviceInfo != NULL) {
        (void)memcpy_s(&addr, sizeof(BtAddr), &deviceInfo->addr, sizeof(BtAddr));
        if (deviceInfo->paramUpdateReq != NULL) {
            doReject = true;
        } else {
            deviceInfo->paramUpdateReq = MEM_MALLOC.alloc(sizeof(LeConnParamUpdateReq));
            if (deviceInfo->paramUpdateReq != NULL) {
                deviceInfo->paramUpdateReq->status = GAP_LE_CONN_PARAM_UPDATE_RECV_HCI;
                deviceInfo->paramUpdateReq->id = 0x00;
            } else {
                doReject = true;
            }
        }
    } else {
        doReject = true;
    }

    if (doReject) {
        GapLeRemoteConnectionParameterRequestNegativeReply(eventParam->connectionHandle, HCI_UNSPECIFIED_ERROR);
    } else {
        GapLeConnCallback callback;
        void *context = NULL;
        if (GapLeConnUpdateCallbackGet(&callback, &context)) {
            if (callback.leConnectionParameterReq) {
                callback.leConnectionParameterReq(&addr,
                    eventParam->intervalMin,
                    eventParam->intervalMax,
                    eventParam->latency,
                    eventParam->timeout,
                    context);
            } else {
                GapLeRemoteConnectionParameterRequestNegativeReply(
                    eventParam->connectionHandle, HCI_UNSPECIFIED_ERROR);
            }
            GapLeConnUpdateCallbackRelease();
        } else {
            GapLeRemoteConnectionParameterRequestNegativeReply(
                eventParam->connectionHandle, HCI_UNSPECIFIED_ERROR);
        }
    }
}

static int GapAcceptConnectionParameterUpdate(const LeDeviceInfo *deviceInfo, const GapLeConnectionParameter *connParam)
{
    int ret;

    if (deviceInfo->paramUpdateReq->status == GAP_LE_CONN_PARAM_UPDATE_RECV_HCI) {
        ret = GapLeRemoteConnectionParameterRequestReply(deviceInfo->handle, connParam);
    } else {
        GapLeConnectionParameterUpdateRsp(
            deviceInfo->handle, deviceInfo->paramUpdateReq->id, L2CAP_LE_CONNECTION_PARAMETERS_ACCEPTED);
        ret = GapLeConnectionUpdate(deviceInfo->handle, connParam);
    }

    return ret;
}

static int GapRejectConnectionParameterUpdate(const LeDeviceInfo *deviceInfo)
{
    int ret = GAP_SUCCESS;

    if (deviceInfo->paramUpdateReq->status == GAP_LE_CONN_PARAM_UPDATE_RECV_HCI) {
        ret = GapLeRemoteConnectionParameterRequestNegativeReply(
            deviceInfo->handle, HCI_UNSUPPORTED_LMP_PARAMETER_VALUE_OR_UNSUPPORTED_LL_PARAMETER_VALUE);
    } else {
        GapLeConnectionParameterUpdateRsp(
            deviceInfo->handle, deviceInfo->paramUpdateReq->id, L2CAP_LE_CONNECTION_PARAMETERS_REJECTED);
    }

    return ret;
}

int GAP_LeConnectionParameterRsp(const BtAddr *addr, uint8_t accept, const GapLeConnectionParameter *connParam)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (addr == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_PERIPHERAL) == false) {
        return GAP_ERR_INVAL_STATE;
    }

    LeConnectionInfoBlock *connectionInfoBlock = GapGetLeConnectionInfoBlock();
    if (connectionInfoBlock == NULL) {
        return GAP_ERR_INVAL_STATE;
    }

    LeDeviceInfo *deviceInfo = ListForEachData(
        connectionInfoBlock->deviceList, GapFindLeConnectionDeviceByAddr, (void *)addr);
    if (deviceInfo == NULL || deviceInfo->paramUpdateReq == NULL) {
        return GAP_ERR_INVAL_STATE;
    }

    if (accept == GAP_ACCEPT) {
        if (connParam == NULL) {
            return GAP_ERR_INVAL_PARAM;
        }
        return GapAcceptConnectionParameterUpdate(deviceInfo, connParam);
    }

    if (accept == GAP_NOT_ACCEPT) {
        return GapRejectConnectionParameterUpdate(deviceInfo);
    }

    return GAP_ERR_INVAL_PARAM;
}

static int GapLeSetHostChannelClassification(uint64_t channelMap)
{
    HciLeSetHostChannelClassificationParam hciCmdParam;
    (void)memcpy_s(hciCmdParam.channelMap, sizeof(hciCmdParam.channelMap), &channelMap, sizeof(hciCmdParam.channelMap));

    return HCI_LeSetHostChannelClassification(&hciCmdParam);
}

void GapLeSetHostChannelClassificationComplete(const HciLeSetHostChannelClassificationReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    GapLeConnCallback callback;
    void *context = NULL;
    if (GapLeConnUpdateCallbackGet(&callback, &context)) {
        if (callback.leSetHostChannelClassificationResult) {
            callback.leSetHostChannelClassificationResult(param->status, context);
        }
        GapLeConnUpdateCallbackRelease();
    }
}

int GAP_LeSetHostChannelClassification(uint64_t channelMap)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeSetHostChannelClassification(channelMap);
    }

    return ret;
}

void GapLeReadChannelMapComplete(const HciLeReadChannelMapReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    BtAddr addr = {0};
    uint64_t channelMap = 0;

    LeConnectionInfoBlock *connectionInfoBlock = GapGetLeConnectionInfoBlock();
    if (connectionInfoBlock == NULL) {
        return;
    }
    LeDeviceInfo *deviceInfo = NULL;
    deviceInfo = ListForEachData(
        connectionInfoBlock->deviceList, GapFindLeConnectionDeviceByHandle, (void *)&param->connectionHandle);
    if (deviceInfo != NULL) {
        (void)memcpy_s(&addr, sizeof(BtAddr), &deviceInfo->addr, sizeof(BtAddr));
        (void)memcpy_s(&channelMap, sizeof(channelMap), param->channelMap, sizeof(param->channelMap));
    }

    if (deviceInfo != NULL) {
        GapLeConnCallback callback;
        void *context = NULL;
        if (GapLeConnUpdateCallbackGet(&callback, &context)) {
            if (callback.leReadChannelMapResult) {
                callback.leReadChannelMapResult(param->status, &addr, channelMap, context);
            }
            GapLeConnUpdateCallbackRelease();
        }
    }
}

int GAP_LeSetBondMode(uint8_t mode)
{
    LOG_INFO("%{public}s: mode[%hhu]", __FUNCTION__, mode);
    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    LeLocalInfo *localInfo = GapGetLeLocalInfo();
    localInfo->bondableMode = mode;

    return GAP_SUCCESS;
}

int GAP_LeSetSecurityMode(GAP_LeSecMode1Level mode1Level, GAP_LeSecMode2Level mode2Level)
{
    LOG_INFO("%{public}s: mode1Level[%{public}d], mode2Level[%{public}d]", __FUNCTION__, mode1Level, mode2Level);
    int ret;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }
    LeLocalInfo *localInfo = GapGetLeLocalInfo();
    localInfo->mode1Level = mode1Level;
    localInfo->mode2Level = mode2Level;
    if (mode1Level == LE_MODE_1_LEVEL_4) {
        BtmLocalVersionInformation version;
        ret = BTM_GetLocalVersionInformation(&version);
        if ((ret == BT_SUCCESS) && (version.hciVersion >= BLUETOOTH_CORE_SPECIFICATION_4_2)) {
            ret = SMP_SetSecureConnOnlyMode(true);
        } else {
            ret = GAP_ERR_NOT_SUPPORT;
        }
    } else {
        ret = SMP_SetSecureConnOnlyMode(false);
    }
    return ret;
}

int GAP_LeGetSecurityStatus(const BtAddr *addr, GAP_LeSecurityStatus *status, uint8_t *encKeySize)
{
    int ret = GAP_SUCCESS;
    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }
    LeConnectionInfoBlock *connectionInfoBlock = GapGetLeConnectionInfoBlock();
    LeDeviceInfo *deviceInfo = NULL;
    deviceInfo = ListForEachData(connectionInfoBlock->deviceList, GapFindLeConnectionDeviceByAddr, (void *)addr);
    if (deviceInfo != NULL) {
        if (status == NULL || encKeySize == NULL) {
            return GAP_ERR_INVAL_PARAM;
        }
        *status = deviceInfo->encryptionStatus;
        *encKeySize = deviceInfo->keySize;
        LOG_INFO("%{public}s:" BT_ADDR_FMT " status:%{public}d, keySize:%hhu",
            __FUNCTION__,
            BT_ADDR_FMT_OUTPUT(addr->addr),
            *status,
            *encKeySize);
    } else {
        ret = GAP_ERR_INVAL_STATE;
    }

    return ret;
}

bool GapLeDeviceNeedBond(const LeDeviceInfo *deviceInfo)
{
    LeBondBlock *leBondBlock = GapGetLeBondBlock();
    return (leBondBlock->isPairing == true) && (GapAddrCompare(&deviceInfo->addr, &leBondBlock->addr));
}

static void GapWaitExAdvTerminatedTimeoutTask(void *ctx)
{
    LeDeviceInfo *deviceInfo = ListForEachData(GapGetLeConnectionInfoBlock()->deviceList, GapFindCmpListData, ctx);
    if (deviceInfo != NULL) {
        deviceInfo->ownAddrUpdated = true;
        GapLeRequestSecurityProcess(deviceInfo);
        if (GapLeDeviceNeedBond(deviceInfo)) {
            GapLeDoPair(&deviceInfo->addr);
        }
    }
}

void GapWaitExAdvTerminatedTimeout(void *dev)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    GapGeneralPointerInfo *ctx = MEM_MALLOC.alloc(sizeof(GapGeneralPointerInfo));
    if (ctx == NULL) {
        LOG_ERROR("%{public}s: Alloc error.", __FUNCTION__);
        return;
    }

    ctx->pointer = dev;

    int ret = GapRunTaskUnBlockProcess(GapWaitExAdvTerminatedTimeoutTask, ctx, NULL);
    if (ret != BT_SUCCESS) {
        LOG_ERROR("%{public}s: Task error:%{public}d.", __FUNCTION__, ret);
    }
}

static void GapLeAddConnectionDevice(
    LeConnectionInfoBlock *block, uint16_t connectionHandle, const BtAddr *addr, uint8_t role)
{
    LeDeviceInfo *deviceInfo = MEM_MALLOC.alloc(sizeof(LeDeviceInfo));
    if (deviceInfo == NULL) {
        LOG_ERROR("%{public}s:alloc failed.", __FUNCTION__);
        return;
    }

    (void)memset_s(deviceInfo, sizeof(LeDeviceInfo), 0x00, sizeof(LeDeviceInfo));
    (void)memcpy_s(&deviceInfo->addr, sizeof(BtAddr), addr, sizeof(BtAddr));
    deviceInfo->handle = connectionHandle;
    BTM_GetLeConnectionAddress(connectionHandle, &deviceInfo->ownAddr, &deviceInfo->peerAddr);
    LOG_DEBUG("%{public}s: own:" BT_ADDR_FMT " %hhu",
        __FUNCTION__,
        BT_ADDR_FMT_OUTPUT(deviceInfo->ownAddr.addr),
        deviceInfo->ownAddr.type);
    LOG_DEBUG("%{public}s: peer:" BT_ADDR_FMT " %hhu",
        __FUNCTION__,
        BT_ADDR_FMT_OUTPUT(deviceInfo->peerAddr.addr),
        deviceInfo->peerAddr.type);
    deviceInfo->role = role;
    deviceInfo->alarm = AlarmCreate("LEWaitExAdvTerminated", false);
    if (deviceInfo->alarm == NULL) {
        // Keep tracking the connection even without the alarm: dropping
        // deviceInfo here would leave the upper layers blind to the link
        // (no record for later disconnect handling). Only the slave
        // wait-ex-adv-terminated timeout is lost; treat it as elapsed.
        LOG_ERROR("%{public}s: AlarmCreate failed, connection tracked without alarm.", __FUNCTION__);
    }
    ListAddLast(block->deviceList, deviceInfo);
    // ListAddLast returns void; verify that the new tail actually holds the
    // just-added node. If the list is in an inconsistent state and the tail
    // does not match, assume insertion failed and free deviceInfo to avoid a
    // leak. This check relies on the list implementation being single-threaded
    // under the GAP task.
    ListNode *lastNode = ListGetLastNode(block->deviceList);
    if (lastNode == NULL || ListGetNodeData(lastNode) != deviceInfo) {
        LOG_ERROR("%{public}s: ListAddLast failed.", __FUNCTION__);
        // Unlink instead of freeing directly: if the tail mismatch is
        // spurious and the node actually made it into the list, a plain
        // free would leave a dangling node for later traversals.
        // ListRemoveNode releases the node via GapFreeLeDeviceInfo
        // (which also cancels the alarm).
        ListRemoveNode(block->deviceList, deviceInfo);
    } else if (role == HCI_ROLE_SLAVE && deviceInfo->alarm != NULL) {
        AlarmSet(deviceInfo->alarm, GAP_WAIT_EX_ADV_TERMINATED, GapWaitExAdvTerminatedTimeout, deviceInfo);
    } else if (role == HCI_ROLE_SLAVE) {
        // Alarm creation failed above; treat the wait-ex-adv-terminated
        // timeout as already elapsed (same semantics as
        // GapWaitExAdvTerminatedTimeoutTask) so the slave still proceeds
        // to the security/pairing flow instead of stalling forever.
        deviceInfo->ownAddrUpdated = true;
        GapLeRequestSecurityProcess(deviceInfo);
        if (GapLeDeviceNeedBond(deviceInfo)) {
            GapLeDoPair(&deviceInfo->addr);
        }
    } else if (role == HCI_ROLE_MASTER && GapLeDeviceNeedBond(deviceInfo)) {
        GapLeDoPair(&deviceInfo->addr);
    } else {
        deviceInfo->ownAddrUpdated = true;
    }
}

void GapLeConnectionComplete(uint8_t status, uint16_t connectionHandle, const BtAddr *addr, uint8_t role, void *context)
{
    LeConnectionInfoBlock *leConnectionInfoBlock = GapGetLeConnectionInfoBlock();

    if (status == HCI_SUCCESS) {
        GapLeAddConnectionDevice(leConnectionInfoBlock, connectionHandle, addr, role);
    } else {
        LeBondBlock *leBondBlock = GapGetLeBondBlock();
        if (leBondBlock->isPairing == true && GapAddrCompare(addr, &leBondBlock->addr)) {
            GapDoPairResultCallback(addr, status);
        }
        GapClearPairingStatus(addr);
    }

    if (status == HCI_SUCCESS) {
        GapRequestSigningAlgorithmInfo(addr);
    }
}

void GapLeDisconnectionComplete(uint8_t status, uint16_t connectionHandle, uint8_t reason, void *context)
{
    LeConnectionInfoBlock *leConnectionInfoBlock = NULL;
    LeDeviceInfo *deviceInfo = NULL;

    leConnectionInfoBlock = GapGetLeConnectionInfoBlock();
    if (status == HCI_SUCCESS) {
        deviceInfo =
            ListForEachData(leConnectionInfoBlock->deviceList, GapFindLeConnectionDeviceByHandle, &connectionHandle);
        if (deviceInfo != NULL) {
            ListRemoveNode(leConnectionInfoBlock->deviceList, deviceInfo);
        }
    }
}

void GapReceiveL2capParameterUpdateReq(
    uint16_t aclHandle, uint8_t id, const L2capLeConnectionParameter *param, void *ctx)
{
    BtAddr addr = {0};
    bool doReject = false;

    LeDeviceInfo *deviceInfo =
        ListForEachData(GapGetLeConnectionInfoBlock()->deviceList, GapFindLeConnectionDeviceByHandle, &aclHandle);
    if (deviceInfo != NULL) {
        (void)memcpy_s(&addr, sizeof(BtAddr), &deviceInfo->addr, sizeof(BtAddr));
        if (deviceInfo->paramUpdateReq != NULL) {
            doReject = true;
        } else {
            deviceInfo->paramUpdateReq = MEM_MALLOC.alloc(sizeof(LeConnParamUpdateReq));
            if (deviceInfo->paramUpdateReq != NULL) {
                deviceInfo->paramUpdateReq->status = GAP_LE_CONN_PARAM_UPDATE_RECV_L2CAP;
                deviceInfo->paramUpdateReq->id = id;
            } else {
                doReject = true;
            }
        }
    } else {
        doReject = true;
    }

    if (doReject) {
        GapLeConnectionParameterUpdateRsp(aclHandle, id, L2CAP_LE_CONNECTION_PARAMETERS_REJECTED);
    } else {
        GapLeConnCallback callback;
        void *context = NULL;
        if (GapLeConnUpdateCallbackGet(&callback, &context)) {
            if (callback.leConnectionParameterReq) {
                callback.leConnectionParameterReq(&addr,
                    param->connIntervalMin,
                    param->connIntervalMax,
                    param->connLatency,
                    param->supervisionTimeout,
                    context);
            } else {
                GapLeConnectionParameterUpdateRsp(aclHandle, id, L2CAP_LE_CONNECTION_PARAMETERS_REJECTED);
            }
            GapLeConnUpdateCallbackRelease();
        } else {
            GapLeConnectionParameterUpdateRsp(aclHandle, id, L2CAP_LE_CONNECTION_PARAMETERS_REJECTED);
        }
    }
}

void GapReceiveL2capParameterUpdateRsp(uint16_t aclHandle, uint16_t result, void *ctx)
{
    BtAddr addr = {0};

    LeDeviceInfo *deviceInfo =
        ListForEachData(GapGetLeConnectionInfoBlock()->deviceList, GapFindLeConnectionDeviceByHandle, &aclHandle);
    if (deviceInfo != NULL) {
        (void)memcpy_s(&addr, sizeof(BtAddr), &deviceInfo->addr, sizeof(BtAddr));
    }

    if (result == L2CAP_LE_CONNECTION_PARAMETERS_REJECTED) {
        GapLeConnCallback callback;
        void *context = NULL;
        if (GapLeConnUpdateCallbackGet(&callback, &context)) {
            if (callback.leConnectionUpdateComplete) {
                callback.leConnectionUpdateComplete(
                    GAP_STATUS_FAILED, &addr, 0, 0, 0, context);
            }
            GapLeConnUpdateCallbackRelease();
        }
    }
}

// Validates the common phy bitmask/zero/support checks shared by
// GAP_LeSetPhy and GAP_LeSetDefaultPhy, keeping both public APIs short.
static int GapLeCheckPhyParams(uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys)
{
    if ((allPhys & ~GAP_LE_ALL_PHY_VALID_MASK) != 0 || (txPhys & ~GAP_LE_PHY_BIT_ALL) != 0 ||
        (rxPhys & ~GAP_LE_PHY_BIT_ALL) != 0) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (!(allPhys & GAP_LE_ALL_PHY_TX_NO_PREFERENCE) && txPhys == 0) {
        return GAP_ERR_INVAL_PARAM;
    }
    if (!(allPhys & GAP_LE_ALL_PHY_RX_NO_PREFERENCE) && rxPhys == 0) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (!(allPhys & GAP_LE_ALL_PHY_TX_NO_PREFERENCE) && (txPhys & GAP_LE_PHY_BIT_2M) &&
        !BTM_IsControllerSupportLe2MPhy()) {
        return GAP_ERR_NOT_SUPPORT;
    }
    if (!(allPhys & GAP_LE_ALL_PHY_TX_NO_PREFERENCE) && (txPhys & GAP_LE_PHY_BIT_CODED) &&
        !BTM_IsControllerSupportLeCodedPhy()) {
        return GAP_ERR_NOT_SUPPORT;
    }
    if (!(allPhys & GAP_LE_ALL_PHY_RX_NO_PREFERENCE) && (rxPhys & GAP_LE_PHY_BIT_2M) &&
        !BTM_IsControllerSupportLe2MPhy()) {
        return GAP_ERR_NOT_SUPPORT;
    }
    if (!(allPhys & GAP_LE_ALL_PHY_RX_NO_PREFERENCE) && (rxPhys & GAP_LE_PHY_BIT_CODED) &&
        !BTM_IsControllerSupportLeCodedPhy()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    return GAP_SUCCESS;
}

static int GapLeSetPhy(uint16_t handle, uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys, uint16_t phyOptions)
{
    HciLeSetPhyParam hciCmdParam = {
        .connectionHandle = handle,
        .allPhys = allPhys,
        .txPhys = txPhys,
        .rxPhys = rxPhys,
        .phyOptions = phyOptions,
    };

    return HCI_LeSetPhy(&hciCmdParam);
}

int GAP_LeSetPhy(const BtAddr *addr, uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys, uint16_t phyOptions)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (addr == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_PERIPHERAL) == false) {
        return GAP_ERR_INVAL_STATE;
    }

    if (phyOptions != GAP_LE_PHY_OPTIONS_CODED_NO_PREFERENCE &&
        phyOptions != GAP_LE_PHY_OPTIONS_CODED_S2 &&
        phyOptions != GAP_LE_PHY_OPTIONS_CODED_S8) {
        return GAP_ERR_INVAL_PARAM;
    }

    int ret = GapLeCheckPhyParams(allPhys, txPhys, rxPhys);
    if (ret != GAP_SUCCESS) {
        return ret;
    }

    LeConnectionInfoBlock *connectionInfoBlock = GapGetLeConnectionInfoBlock();
    if (connectionInfoBlock == NULL) {
        return GAP_ERR_INVAL_STATE;
    }
    LeDeviceInfo *deviceInfo =
        ListForEachData(connectionInfoBlock->deviceList, GapFindLeConnectionDeviceByAddr, (void *)addr);
    if (deviceInfo == NULL) {
        return GAP_ERR_INVAL_STATE;
    }

    return GapLeSetPhy(deviceInfo->handle, allPhys, txPhys, rxPhys, phyOptions);
}

static int GapLeReadPhy(uint16_t handle)
{
    HciLeReadPhyParam hciCmdParam = {
        .connectionHandle = handle,
    };

    return HCI_LeReadPhy(&hciCmdParam);
}

int GAP_LeReadPhy(const BtAddr *addr)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (addr == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_PERIPHERAL) == false) {
        return GAP_ERR_INVAL_STATE;
    }

    if (!BTM_IsControllerSupportLeReadPhy()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    LeConnectionInfoBlock *connectionInfoBlock = GapGetLeConnectionInfoBlock();
    if (connectionInfoBlock == NULL) {
        return GAP_ERR_INVAL_STATE;
    }
    LeDeviceInfo *deviceInfo =
        ListForEachData(connectionInfoBlock->deviceList, GapFindLeConnectionDeviceByAddr, (void *)addr);
    if (deviceInfo == NULL) {
        return GAP_ERR_INVAL_STATE;
    }

    return GapLeReadPhy(deviceInfo->handle);
}

int GAP_LeSetDefaultPhy(uint8_t allPhys, uint8_t txPhys, uint8_t rxPhys)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_PERIPHERAL) == false) {
        return GAP_ERR_INVAL_STATE;
    }

    int ret = GapLeCheckPhyParams(allPhys, txPhys, rxPhys);
    if (ret != GAP_SUCCESS) {
        return ret;
    }

    HciLeSetDefaultPhyParam hciCmdParam = {
        .allPhys = allPhys,
        .txPhys = txPhys,
        .rxPhys = rxPhys,
    };
    return HCI_LeSetDefaultPhy(&hciCmdParam);
}

void GapLeReadPhyComplete(const HciLeReadPhyReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    LeConnectionInfoBlock *connectionInfoBlock = GapGetLeConnectionInfoBlock();
    if (connectionInfoBlock == NULL) {
        LOG_WARN("%{public}s: connection info block not initialized", __FUNCTION__);
        return;
    }
    LeDeviceInfo *deviceInfo = ListForEachData(
        connectionInfoBlock->deviceList, GapFindLeConnectionDeviceByHandle, (void *)&param->connectionHandle);
    if (deviceInfo == NULL) {
        LOG_WARN("%{public}s: device not found, handle:0x%04x", __FUNCTION__, param->connectionHandle);
        GapLeConnCallback callback;
        void *context = NULL;
        if (GapLeConnUpdateCallbackGet(&callback, &context)) {
            if (callback.leReadPhyResult) {
                callback.leReadPhyResult(GAP_STATUS_FAILED, NULL, 0, 0, context);
            }
            GapLeConnUpdateCallbackRelease();
        }
        return;
    }

    BtAddr addr = {0};
    (void)memcpy_s(&addr, sizeof(BtAddr), &deviceInfo->addr, sizeof(BtAddr));

    GapLeConnCallback callback;
    void *context = NULL;
    if (GapLeConnUpdateCallbackGet(&callback, &context)) {
        if (callback.leReadPhyResult) {
            callback.leReadPhyResult(
                param->status, &addr, param->txPhy, param->rxPhy, context);
        }
        GapLeConnUpdateCallbackRelease();
    }
}

void GapLeSetDefaultPhyComplete(const HciLeSetDefaultPhyReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeConnCallback callback;
    void *context = NULL;
    if (GapLeConnUpdateCallbackGet(&callback, &context)) {
        if (callback.leSetDefaultPhyResult) {
            callback.leSetDefaultPhyResult(param->status, context);
        }
        GapLeConnUpdateCallbackRelease();
    }
}

void GapLeSetPhyComplete(const HciLeSetPhyReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    LOG_INFO("%{public}s: status:0x%02x", __FUNCTION__, param->status);

    GapLeConnCallback callback;
    void *context = NULL;
    if (GapLeConnUpdateCallbackGet(&callback, &context)) {
        if (callback.leSetPhyResult) {
            callback.leSetPhyResult(param->status, context);
        }
        GapLeConnUpdateCallbackRelease();
    }
}

void GapOnLePhyUpdateCompleteEvent(const HciLePhyUpdateCompleteEventParam *eventParam)
{
    if (eventParam == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    LeConnectionInfoBlock *connectionInfoBlock = GapGetLeConnectionInfoBlock();
    if (connectionInfoBlock == NULL) {
        LOG_WARN("%{public}s: connection info block not initialized", __FUNCTION__);
        return;
    }
    LeDeviceInfo *deviceInfo = ListForEachData(connectionInfoBlock->deviceList, GapFindLeConnectionDeviceByHandle,
                                               (void *)&eventParam->connectionHandle);
    if (deviceInfo == NULL) {
        LOG_WARN("%{public}s: device not found, handle:0x%04x", __FUNCTION__, eventParam->connectionHandle);
        return;
    }

    BtAddr addr = {0};
    (void)memcpy_s(&addr, sizeof(BtAddr), &deviceInfo->addr, sizeof(BtAddr));

    GapLeConnCallback callback;
    void *context = NULL;
    if (GapLeConnUpdateCallbackGet(&callback, &context)) {
        if (callback.lePhyUpdateComplete) {
            callback.lePhyUpdateComplete(eventParam->status, &addr, eventParam->txPhy,
                eventParam->rxPhy, context);
        }
        GapLeConnUpdateCallbackRelease();
    }
}

int GAP_LeSetDataLength(const BtAddr *addr, uint16_t txOctets, uint16_t txTime)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (addr == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_PERIPHERAL) == false) {
        return GAP_ERR_INVAL_STATE;
    }

    if (!BTM_IsControllerSupportLeDataPacketLengthExtension()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    if (txOctets < GAP_LE_DATA_LENGTH_OCTETS_MIN || txOctets > GAP_LE_DATA_LENGTH_OCTETS_MAX ||
        txTime < GAP_LE_DATA_LENGTH_TIME_MIN || txTime > GAP_LE_DATA_LENGTH_TIME_MAX ||
        txTime < txOctets * GAP_LE_DATA_LENGTH_TIME_PER_OCTET + GAP_LE_DATA_LENGTH_TIME_OVERHEAD) {
        return GAP_ERR_INVAL_PARAM;
    }

    LeConnectionInfoBlock *connectionInfoBlock = GapGetLeConnectionInfoBlock();
    if (connectionInfoBlock == NULL) {
        return GAP_ERR_INVAL_STATE;
    }
    LeDeviceInfo *deviceInfo =
        ListForEachData(connectionInfoBlock->deviceList, GapFindLeConnectionDeviceByAddr, (void *)addr);
    if (deviceInfo == NULL) {
        return GAP_ERR_INVAL_STATE;
    }

    HciLeSetDataLengthParam hciCmdParam = {
        .connectionHandle = deviceInfo->handle,
        .txOctets = txOctets,
        .txTime = txTime,
    };
    return HCI_LeSetDataLength(&hciCmdParam);
}

void GapLeSetDataLengthComplete(const HciLeSetDataLengthReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    // The DLE command result is otherwise only delivered to a registered
    // leSetDataLengthResult callback (often absent for stack-internal DLE requests).
    // Log rejected requests explicitly so controller-side DLE failures (e.g. the
    // rk3568 one-shot-DLE quirk) are visible instead of failing silently.
    if (param->status != HCI_SUCCESS) {
        LOG_WARN("%{public}s: HCI_LeSetDataLength rejected, status:0x%02x, handle:0x%04x",
            __FUNCTION__, param->status, param->connectionHandle);
    }

    LeConnectionInfoBlock *connectionInfoBlock = GapGetLeConnectionInfoBlock();
    if (connectionInfoBlock == NULL) {
        LOG_WARN("%{public}s: connection info block not initialized", __FUNCTION__);
        return;
    }
    LeDeviceInfo *deviceInfo = ListForEachData(
        connectionInfoBlock->deviceList, GapFindLeConnectionDeviceByHandle, (void *)&param->connectionHandle);
    if (deviceInfo == NULL) {
        LOG_WARN("%{public}s: device not found, handle:0x%04x", __FUNCTION__, param->connectionHandle);
        GapLeConnCallback callback;
        void *context = NULL;
        if (GapLeConnUpdateCallbackGet(&callback, &context)) {
            if (callback.leSetDataLengthResult) {
                callback.leSetDataLengthResult(GAP_STATUS_FAILED, NULL, context);
            }
            GapLeConnUpdateCallbackRelease();
        }
        return;
    }

    BtAddr addr = {0};
    (void)memcpy_s(&addr, sizeof(BtAddr), &deviceInfo->addr, sizeof(BtAddr));

    GapLeConnCallback callback;
    void *context = NULL;
    if (GapLeConnUpdateCallbackGet(&callback, &context)) {
        if (callback.leSetDataLengthResult) {
            callback.leSetDataLengthResult(param->status, &addr, context);
        }
        GapLeConnUpdateCallbackRelease();
    }
}

void GapOnLeDataLengthChangeEvent(const HciLeDataLengthChangeEventParam *eventParam)
{
    if (eventParam == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    LeConnectionInfoBlock *connectionInfoBlock = GapGetLeConnectionInfoBlock();
    if (connectionInfoBlock == NULL) {
        LOG_WARN("%{public}s: connection info block not initialized", __FUNCTION__);
        return;
    }
    LeDeviceInfo *deviceInfo = ListForEachData(connectionInfoBlock->deviceList, GapFindLeConnectionDeviceByHandle,
                                               (void *)&eventParam->connectionHandle);
    if (deviceInfo == NULL) {
        LOG_WARN("%{public}s: device not found, handle:0x%04x", __FUNCTION__, eventParam->connectionHandle);
        return;
    }

    BtAddr addr = {0};
    (void)memcpy_s(&addr, sizeof(BtAddr), &deviceInfo->addr, sizeof(BtAddr));

    LOG_INFO("%{public}s:handle:0x%04x, txOctets:%hu, txTime:%hu, rxOctets:%hu, rxTime:%hu", __FUNCTION__,
             eventParam->connectionHandle, eventParam->maxTxOctets, eventParam->maxTxTime, eventParam->maxRxOctets,
             eventParam->maxRxTime);

    GapLeConnCallback callback;
    void *context = NULL;
    if (GapLeConnUpdateCallbackGet(&callback, &context)) {
        if (callback.leDataLengthChange) {
            callback.leDataLengthChange(&addr, eventParam->maxTxOctets, eventParam->maxTxTime,
                eventParam->maxRxOctets, eventParam->maxRxTime,
                context);
        }
        GapLeConnUpdateCallbackRelease();
    }
}

typedef struct {
    GapLeControllerCallback callback;
    void *context;
} LeControllerCallbackBlock;

static LeControllerCallbackBlock g_leControllerCallback;
static Mutex *g_leControllerCallbackMutex = NULL;
// Number of in-flight callbacks for the controller-info callback group.
static int32_t g_leControllerCallbackRef = 0;

// Creates the per-group mutexes for the conn-update and controller callback
// groups. Returns GAP_SUCCESS, or GAP_ERR_OUT_OF_RES after cleaning up any
// mutex created so far. Called with g_leCallbackLifecycleMutex held.
static int GapLeCallbackMutexesInit(Mutex **connMutex, Mutex **controllerMutex)
{
    if (*connMutex == NULL) {
        *connMutex = MutexCreate();
    }
    if (*controllerMutex == NULL) {
        *controllerMutex = MutexCreate();
    }
    if (*connMutex != NULL && *controllerMutex != NULL) {
        return GAP_SUCCESS;
    }
    LOG_ERROR("%{public}s: MutexCreate failed", __FUNCTION__);
    if (*connMutex != NULL) {
        MutexDelete(*connMutex);
        *connMutex = NULL;
    }
    if (*controllerMutex != NULL) {
        MutexDelete(*controllerMutex);
        *controllerMutex = NULL;
    }
    return GAP_ERR_OUT_OF_RES;
}

// Creates (lazily, via CAS) the deinit-completion event used by
// GapLeCallbackDeinit to wait for in-flight callbacks. On failure the
// per-group mutexes created above are torn down. Called with the lifecycle
// mutex held.
static int GapLeCallbackDeinitEventInit(Mutex *connMutex, Mutex *controllerMutex)
{
    if (__atomic_load_n(&g_leCallbackDeinitEvent, __ATOMIC_ACQUIRE) != NULL) {
        return GAP_SUCCESS;
    }
    Event *newEvent = EventCreate(true);
    if (newEvent == NULL) {
        LOG_ERROR("%{public}s: EventCreate failed", __FUNCTION__);
        if (connMutex != NULL) {
            MutexDelete(connMutex);
            g_leConnUpdateCallbackMutex = NULL;
        }
        if (controllerMutex != NULL) {
            MutexDelete(controllerMutex);
            g_leControllerCallbackMutex = NULL;
        }
        __atomic_store_n(&g_leConnUpdateCallbackRef, 0, __ATOMIC_SEQ_CST);
        __atomic_store_n(&g_leControllerCallbackRef, 0, __ATOMIC_SEQ_CST);
        return GAP_ERR_OUT_OF_RES;
    }
    Event *expectedEvent = NULL;
    if (!__atomic_compare_exchange_n(&g_leCallbackDeinitEvent, &expectedEvent, newEvent, false,
        __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
        EventDelete(newEvent);
    }
    return GAP_SUCCESS;
}

int GapLeCallbackInit(void)
{
    // Ensure the lifecycle mutex exists before locking. Once created, it is never
    // destroyed so that racing registration/lookup paths can always observe the
    // initialized/deinitialized state safely.
    if (g_leCallbackLifecycleMutex == NULL) {
        Mutex *newMutex = MutexCreate();
        if (newMutex == NULL) {
            LOG_ERROR("%{public}s: Lifecycle MutexCreate failed", __FUNCTION__);
            return GAP_ERR_OUT_OF_RES;
        }
        Mutex *expected = NULL;
        if (!__atomic_compare_exchange_n(&g_leCallbackLifecycleMutex, &expected, newMutex, false,
            __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
            MutexDelete(newMutex);
        }
    }

    MutexLock(g_leCallbackLifecycleMutex);
    if (g_leCallbackDeinitInProgress) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        LOG_ERROR("%{public}s: deinit in progress", __FUNCTION__);
        return GAP_ERR_OUT_OF_RES;
    }

    // Clear callback state under the lifecycle lock so that concurrent
    // registration/lookup paths cannot observe partially initialized memory.
    (void)memset_s(&g_leConnUpdateCallback, sizeof(g_leConnUpdateCallback), 0x00, sizeof(g_leConnUpdateCallback));
    (void)memset_s(&g_leControllerCallback, sizeof(g_leControllerCallback), 0x00, sizeof(g_leControllerCallback));
    GapLeAdvCallbackInit();

    if (GapLeCallbackMutexesInit(&g_leConnUpdateCallbackMutex, &g_leControllerCallbackMutex) != GAP_SUCCESS) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }
    __atomic_store_n(&g_leConnUpdateCallbackRef, 0, __ATOMIC_SEQ_CST);
    __atomic_store_n(&g_leControllerCallbackRef, 0, __ATOMIC_SEQ_CST);

    if (GapLeCallbackDeinitEventInit(g_leConnUpdateCallbackMutex, g_leControllerCallbackMutex) != GAP_SUCCESS) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }

    MutexUnlock(g_leCallbackLifecycleMutex);
    return GAP_SUCCESS;
}

// Number of drain wait iterations: 12 * 5000 ms = 60 s per callback group.
#define GAP_LE_CALLBACK_DRAIN_WAIT_RETRIES (12)
#define GAP_LE_CALLBACK_DRAIN_WAIT_MS (5000)

// Waits until all in-flight callbacks of one callback group have released
// their references, or the wait window elapses (event-based wait, falling back
// to a bounded spin when the deinit event is not yet published). Returns true
// when the group's reference count reached zero, i.e. its mutex can be safely
// deleted.
static bool GapLeWaitCallbackRefsDrain(int32_t *groupRef, const char *groupName)
{
    Event *event = __atomic_load_n(&g_leCallbackDeinitEvent, __ATOMIC_ACQUIRE);
    if (event != NULL) {
        int32_t waitRetries = GAP_LE_CALLBACK_DRAIN_WAIT_RETRIES;
        while (__atomic_load_n(groupRef, __ATOMIC_SEQ_CST) > 0 && waitRetries > 0) {
            (void)EventWait(event, GAP_LE_CALLBACK_DRAIN_WAIT_MS);
            waitRetries--;
        }
        if (waitRetries == 0) {
            LOG_ERROR("%{public}s: timeout waiting for %s callbacks to release", __FUNCTION__, groupName);
        }
    } else {
        int32_t spinRetries = 1000000;
        while (__atomic_load_n(groupRef, __ATOMIC_SEQ_CST) > 0 && spinRetries > 0) {
            sched_yield();
            spinRetries--;
        }
        if (spinRetries == 0) {
            LOG_ERROR("%{public}s: spin timeout waiting for %s callbacks to release", __FUNCTION__, groupName);
        }
    }
    return __atomic_load_n(groupRef, __ATOMIC_SEQ_CST) == 0;
}

// Describes the callback fields of one callback group to be cleared.
typedef struct {
    void *callbackStruct;
    size_t callbackSize;
    void **contextPtr;
} GapLeCallbackGroupTarget;

// Clears one callback group under its mutex and deletes the mutex once every
// reference has drained. A thread that loaded the mutex pointer before deinit
// NULLed the global may still be about to lock it; deleting an in-use mutex is
// undefined behavior, so leaking it on timeout is safer than crashing.
static void GapLeClearCallbackGroup(
    Mutex *groupMutex, bool refsDrained, const GapLeCallbackGroupTarget *target, const char *groupName)
{
    MutexLock(groupMutex);
    (void)memset_s(target->callbackStruct, target->callbackSize, 0x00, target->callbackSize);
    *target->contextPtr = NULL;
    MutexUnlock(groupMutex);
    if (refsDrained) {
        MutexDelete(groupMutex);
    } else {
        LOG_ERROR("%{public}s: leaving %s callback mutex allocated due to unreleased references",
            __FUNCTION__, groupName);
    }
}

int GapLeCallbackDeinit(void)
{
    Mutex *lifecycleMutex = __atomic_load_n(&g_leCallbackLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        return GAP_SUCCESS;
    }

    MutexLock(lifecycleMutex);
    if (g_leCallbackDeinitInProgress) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        return GAP_SUCCESS;
    }
    g_leCallbackDeinitInProgress = true;

    // Clear the legacy adv/ex-adv callback groups under the lifecycle lock,
    // mirroring GapLeCallbackInit: GAP_RegisterExAdvCallback may be invoked
    // from any thread, so clearing outside the lock would race with a
    // concurrent registration writing the same callback fields.
    GapLeAdvCallbackDeinit();

    Mutex *connMutex = g_leConnUpdateCallbackMutex;
    g_leConnUpdateCallbackMutex = NULL;
    Mutex *controllerMutex = g_leControllerCallbackMutex;
    g_leControllerCallbackMutex = NULL;
    MutexUnlock(g_leCallbackLifecycleMutex);

    if (connMutex != NULL) {
        bool refsDrained = GapLeWaitCallbackRefsDrain(&g_leConnUpdateCallbackRef, "conn update");
        GapLeCallbackGroupTarget target = {
            .callbackStruct = &g_leConnUpdateCallback.callback,
            .callbackSize = sizeof(g_leConnUpdateCallback.callback),
            .contextPtr = &g_leConnUpdateCallback.context,
        };
        GapLeClearCallbackGroup(connMutex, refsDrained, &target, "conn update");
    }

    if (controllerMutex != NULL) {
        bool refsDrained = GapLeWaitCallbackRefsDrain(&g_leControllerCallbackRef, "controller");
        GapLeCallbackGroupTarget target = {
            .callbackStruct = &g_leControllerCallback.callback,
            .callbackSize = sizeof(g_leControllerCallback.callback),
            .contextPtr = &g_leControllerCallback.context,
        };
        GapLeClearCallbackGroup(controllerMutex, refsDrained, &target, "controller");
    }

    MutexLock(g_leCallbackLifecycleMutex);
    g_leCallbackDeinitInProgress = false;
    MutexUnlock(g_leCallbackLifecycleMutex);
    // Intentionally keep g_leCallbackLifecycleMutex alive: other threads may
    // still be racing through GAP_{Register,Deregister,Get}* and need the
    // lifecycle lock to observe that the inner mutexes are gone.
    return GAP_SUCCESS;
}

static bool GapLeControllerCallbackGet(GapLeControllerCallback *callback, void **context)
{
    if (callback == NULL || context == NULL) {
        if (callback != NULL) {
            (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        }
        if (context != NULL) {
            *context = NULL;
        }
        return false;
    }

    Mutex *lifecycleMutex = __atomic_load_n(&g_leCallbackLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        *context = NULL;
        return false;
    }

    MutexLock(lifecycleMutex);
    if (g_leCallbackDeinitInProgress) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        *context = NULL;
        return false;
    }

    Mutex *mutex = g_leControllerCallbackMutex;
    if (mutex == NULL) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        *context = NULL;
        return false;
    }

    MutexLock(mutex);
    *callback = g_leControllerCallback.callback;
    *context = g_leControllerCallback.context;
    (void)__atomic_fetch_add(&g_leControllerCallbackRef, 1, __ATOMIC_SEQ_CST);
    MutexUnlock(mutex);
    MutexUnlock(g_leCallbackLifecycleMutex);
    return true;
}

static void GapLeControllerCallbackRelease(void)
{
    if (__atomic_fetch_sub(&g_leControllerCallbackRef, 1, __ATOMIC_SEQ_CST) == 1) {
        Event *event = __atomic_load_n(&g_leCallbackDeinitEvent, __ATOMIC_ACQUIRE);
        if (event != NULL) {
            EventSet(event);
        }
    }
}

int GAP_LeRegisterLeControllerCallback(const GapLeControllerCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "NULL");
    Mutex *lifecycleMutex = __atomic_load_n(&g_leCallbackLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(lifecycleMutex);
    if (g_leCallbackDeinitInProgress) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }
    Mutex *mutex = g_leControllerCallbackMutex;
    if (mutex == NULL) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(mutex);
    if (callback == NULL) {
        (void)memset_s(&g_leControllerCallback.callback,
            sizeof(g_leControllerCallback.callback),
            0x00,
            sizeof(g_leControllerCallback.callback));
    } else {
        g_leControllerCallback.callback = *callback;
    }
    g_leControllerCallback.context = context;
    MutexUnlock(mutex);
    MutexUnlock(g_leCallbackLifecycleMutex);
    return GAP_SUCCESS;
}

int GAP_LeDeregisterLeControllerCallback(void)
{
    Mutex *lifecycleMutex = __atomic_load_n(&g_leCallbackLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(lifecycleMutex);
    if (g_leCallbackDeinitInProgress) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }
    Mutex *mutex = g_leControllerCallbackMutex;
    if (mutex == NULL) {
        MutexUnlock(g_leCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(mutex);
    (void)memset_s(&g_leControllerCallback.callback,
        sizeof(g_leControllerCallback.callback),
        0x00,
        sizeof(g_leControllerCallback.callback));
    g_leControllerCallback.context = NULL;
    MutexUnlock(mutex);
    MutexUnlock(g_leCallbackLifecycleMutex);
    return GAP_SUCCESS;
}

int GAP_LeReadTransmitPower(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (!BTM_IsControllerSupportLeReadTransmitPower()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    return HCI_LeReadTransmitPower();
}

void GapLeReadTransmitPowerComplete(const HciLeReadTransmitPowerReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeControllerCallback callback;
    void *context = NULL;
    if (GapLeControllerCallbackGet(&callback, &context)) {
        if (callback.readTransmitPowerResult) {
            callback.readTransmitPowerResult(param->status,
                (int8_t)param->minTxPower,
                (int8_t)param->maxTxPower,
                context);
        }
        GapLeControllerCallbackRelease();
    }
}

int GAP_LeReadRfPathCompensation(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (!BTM_IsControllerSupportLeReadRfPathCompensation()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    return HCI_LeReadRfPathCompensation();
}

void GapLeReadRfPathCompensationComplete(const HciLeReadRfPathCompensationReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeControllerCallback callback;
    void *context = NULL;
    if (GapLeControllerCallbackGet(&callback, &context)) {
        if (callback.readRfPathCompensationResult) {
            callback.readRfPathCompensationResult(param->status,
                param->rfTxPathCompensationValue,
                param->rfRxPathCompensationValue,
                context);
        }
        GapLeControllerCallbackRelease();
    }
}

int GAP_LeWriteRfPathCompensation(int16_t txPathCompensation, int16_t rxPathCompensation)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (txPathCompensation < GAP_LE_RF_PATH_COMPENSATION_MIN || txPathCompensation > GAP_LE_RF_PATH_COMPENSATION_MAX ||
        rxPathCompensation < GAP_LE_RF_PATH_COMPENSATION_MIN || rxPathCompensation > GAP_LE_RF_PATH_COMPENSATION_MAX) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (!BTM_IsControllerSupportLeWriteRfPathCompensation()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    HciLeWriteRfPathCompensationParam hciCmdParam = {
        .rfTxPathCompensationValue = txPathCompensation,
        .rfRxPathCompensationValue = rxPathCompensation,
    };
    return HCI_LeWriteRfPathCompensation(&hciCmdParam);
}

void GapLeWriteRfPathCompensationComplete(const HciLeWriteRfPathCompensationReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeControllerCallback callback;
    void *context = NULL;
    if (GapLeControllerCallbackGet(&callback, &context)) {
        if (callback.writeRfPathCompensationResult) {
            callback.writeRfPathCompensationResult(param->status, context);
        }
        GapLeControllerCallbackRelease();
    }
}

int GAP_LeReadSuggestedDefaultDataLength(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (!BTM_IsControllerSupportLeDataPacketLengthExtension()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    return HCI_LeReadSuggestedDefaultDataLength();
}

void GapLeReadSuggestedDefaultDataLengthComplete(const HciLeReadSuggestedDefaultDataLengthReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeControllerCallback callback;
    void *context = NULL;
    if (GapLeControllerCallbackGet(&callback, &context)) {
        if (callback.readSuggestedDefaultDataLengthResult) {
            callback.readSuggestedDefaultDataLengthResult(param->status,
                param->suggestedMaxTxOctets,
                param->suggestedMaxTxTime,
                context);
        }
        GapLeControllerCallbackRelease();
    }
}

int GAP_LeWriteSuggestedDefaultDataLength(uint16_t suggestedMaxTxOctets, uint16_t suggestedMaxTxTime)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    // txTime must also cover the on-air overhead (incl. MIC) of txOctets, else
    // the controller rejects the combination with 0x12 while the stack reports
    // BT_SUCCESS; same rule as GAPIF_LeWriteSuggestedDefaultDataLength and
    // GapLeSetDataLength.
    const uint32_t maxTxTimeLimit =
        suggestedMaxTxOctets * GAP_LE_DATA_LENGTH_TIME_PER_OCTET + GAP_LE_DATA_LENGTH_TIME_OVERHEAD;
    if (suggestedMaxTxOctets < GAP_LE_DATA_LENGTH_OCTETS_MIN || suggestedMaxTxOctets > GAP_LE_DATA_LENGTH_OCTETS_MAX ||
        suggestedMaxTxTime < GAP_LE_DATA_LENGTH_TIME_MIN || suggestedMaxTxTime > GAP_LE_DATA_LENGTH_TIME_MAX ||
        suggestedMaxTxTime < maxTxTimeLimit) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (!BTM_IsControllerSupportLeDataPacketLengthExtension()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    HciLeWriteSuggestedDefaultDataLengthParam hciCmdParam = {
        .suggestedMaxTxOctets = suggestedMaxTxOctets,
        .suggestedMaxTxTime = suggestedMaxTxTime,
    };
    return HCI_LeWriteSuggestedDefaultDataLength(&hciCmdParam);
}

void GapLeWriteSuggestedDefaultDataLengthComplete(const HciLeWriteSuggestedDefaultDataLengthReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeControllerCallback callback;
    void *context = NULL;
    if (GapLeControllerCallbackGet(&callback, &context)) {
        if (callback.writeSuggestedDefaultDataLengthResult) {
            callback.writeSuggestedDefaultDataLengthResult(
                param->status, context);
        }
        GapLeControllerCallbackRelease();
    }
}

int GAP_LeReadMaximumDataLength(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (!BTM_IsControllerSupportLeDataPacketLengthExtension()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    return HCI_LeReadMaximumDataLength();
}

void GapLeReadMaximumDataLengthComplete(const HciLeReadMaximumDataLengthReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeControllerCallback callback;
    void *context = NULL;
    if (GapLeControllerCallbackGet(&callback, &context)) {
        if (callback.readMaxDataLengthResult) {
            callback.readMaxDataLengthResult(param->status,
                param->supportedMaxTxOctets,
                param->supportedMaxTxTime,
                param->supportedMaxRxOctets,
                param->supportedMaxRxTime,
                context);
        }
        GapLeControllerCallbackRelease();
    }
}

int GAP_LeEnhancedReceiverTest(uint8_t rxChannel, uint8_t phy, uint8_t modulationIndex)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (rxChannel > GAP_LE_TEST_CHANNEL_MAX ||
        (phy != GAP_LE_PHY_1M && phy != GAP_LE_PHY_2M && phy != GAP_LE_PHY_CODED) ||
        modulationIndex > GAP_LE_TEST_MODULATION_INDEX_MAX) {
        return GAP_ERR_INVAL_PARAM;
    }

    if ((phy == GAP_LE_PHY_2M && !BTM_IsControllerSupportLe2MPhy()) ||
        (phy == GAP_LE_PHY_CODED && !BTM_IsControllerSupportLeCodedPhy())) {
        return GAP_ERR_NOT_SUPPORT;
    }

    HciLeEnhancedReceiverTestParam hciCmdParam = {
        .rxChannel = rxChannel,
        .phy = phy,
        .modulationIndex = modulationIndex,
    };
    return HCI_LeEnhancedReceiverTest(&hciCmdParam);
}

void GapLeEnhancedReceiverTestComplete(const HciLeEnhancedReceiverTestReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeControllerCallback callback;
    void *context = NULL;
    if (GapLeControllerCallbackGet(&callback, &context)) {
        if (callback.enhancedReceiverTestResult) {
            callback.enhancedReceiverTestResult(param->status, context);
        }
        GapLeControllerCallbackRelease();
    }
}

int GAP_LeEnhancedTransmitterTest(uint8_t txChannel, uint8_t lengthOfTestData, uint8_t packetPayload, uint8_t phy)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (txChannel > GAP_LE_TEST_CHANNEL_MAX || lengthOfTestData > GAP_LE_TEST_DATA_LENGTH_MAX ||
        packetPayload > GAP_LE_TEST_PACKET_PAYLOAD_MAX ||
        (phy != GAP_LE_PHY_1M && phy != GAP_LE_PHY_2M && phy != GAP_LE_PHY_CODED && phy != GAP_LE_TEST_PHY_CODED_S2)) {
        return GAP_ERR_INVAL_PARAM;
    }

    if ((phy == GAP_LE_PHY_2M && !BTM_IsControllerSupportLe2MPhy()) ||
        ((phy == GAP_LE_PHY_CODED || phy == GAP_LE_TEST_PHY_CODED_S2) &&
         !BTM_IsControllerSupportLeCodedPhy())) {
        return GAP_ERR_NOT_SUPPORT;
    }

    HciLeEnhancedTransmitterTestParam hciCmdParam = {
        .txChannel = txChannel,
        .lengthOfTestData = lengthOfTestData,
        .packetPayload = packetPayload,
        .phy = phy,
    };
    return HCI_LeEnhancedTransmitterTest(&hciCmdParam);
}

void GapLeEnhancedTransmitterTestComplete(const HciLeEnhancedTransmitterTestReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeControllerCallback callback;
    void *context = NULL;
    if (GapLeControllerCallbackGet(&callback, &context)) {
        if (callback.enhancedTransmitterTestResult) {
            callback.enhancedTransmitterTestResult(param->status, context);
        }
        GapLeControllerCallbackRelease();
    }
}

#endif
