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

#include "iso.h"

#include <securec.h>

#include "allocator.h"
#include "log.h"
#include "module.h"

#include "btm.h"
#include "btm/btm_thread.h"

#include "hci/hci.h"

#include "iso_task_internal.h"

static IsoLeMng g_isoMng;

static void IsoFreeNode(void *data)
{
    MEM_MALLOC.free(data);
}

static void IsoInitializeTask(void *ctx)
{
    LOG_DEBUG("%{public}s:", __FUNCTION__);

    (void)ctx;
    (void)memset_s(&g_isoMng, sizeof(IsoLeMng), 0x00, sizeof(IsoLeMng));
    g_isoMng.cigBlock.cigList = ListCreate(IsoFreeNode);
    g_isoMng.cisList = ListCreate(IsoFreeNode);
    g_isoMng.bigBlock.bigList = ListCreate(IsoFreeNode);
    g_isoMng.bigBlock.syncList = ListCreate(IsoFreeNode);
    // watchdog of the LE Remove CIG pending slot (see IsoLeRemoveCig); an alarm failure only
    // degrades to the old forever-BT_ALREADY behavior, unlike the lists it is not fatal
    g_isoMng.removePending.timer = AlarmCreate("isoRemoveCig", false);
    // ListClear/ListAddLast dereference the list without a NULL check; on allocation
    // failure the module would crash at the first use, so fail the initialization:
    // the module stays disabled (all public APIs check IsoIsEnable) and the disabled
    // path null-guards the lists below.
    if ((g_isoMng.cigBlock.cigList == NULL) || (g_isoMng.cisList == NULL) ||
        (g_isoMng.bigBlock.bigList == NULL) || (g_isoMng.bigBlock.syncList == NULL)) {
        LOG_ERROR("%{public}s: ListCreate failed", __FUNCTION__);
        if (g_isoMng.cigBlock.cigList != NULL) {
            ListClear(g_isoMng.cigBlock.cigList);
            g_isoMng.cigBlock.cigList = NULL;
        }
        if (g_isoMng.cisList != NULL) {
            ListClear(g_isoMng.cisList);
            g_isoMng.cisList = NULL;
        }
        if (g_isoMng.bigBlock.bigList != NULL) {
            ListClear(g_isoMng.bigBlock.bigList);
            g_isoMng.bigBlock.bigList = NULL;
        }
        if (g_isoMng.bigBlock.syncList != NULL) {
            ListClear(g_isoMng.bigBlock.syncList);
            g_isoMng.bigBlock.syncList = NULL;
        }
        g_isoMng.isEnable = false;
    }
}

static void IsoInitialize(int traceLevel)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    (void)traceLevel;
    // Fail the initialization when the queue cannot be created (e.g. a repeated init before
    // shutdown): running the init task anyway would rebuild the lists over the live ones and
    // leak the previous instances.
    if (BTM_CreateProcessingQueue(PROCESSING_QUEUE_ID_ISO, BTM_PROCESSING_QUEUE_SIZE_DEFAULT) != BT_SUCCESS) {
        LOG_ERROR("%{public}s: CreateProcessingQueue failed", __FUNCTION__);
        return;
    }

    int ret = IsoRunTaskBlockProcess(IsoInitializeTask, NULL);
    if (ret != BT_SUCCESS) {
        LOG_ERROR("%{public}s: Run task error.", __FUNCTION__);
    }
}

static void IsoEnableTask(void *ctx)
{
    LOG_DEBUG("%{public}s:", __FUNCTION__);

    (void)ctx;
    // A repeated enable without a shutdown in between must not register the HCI callbacks
    // twice, or every event would be double-dispatched.
    if (g_isoMng.isEnable) {
        HILOGW("%{public}s: already enabled, skip", __FUNCTION__);
        return;
    }
    // The lists must exist: their consumers run whenever the module is enabled.
    if (BTM_IsControllerSupportLe() && (g_isoMng.cigBlock.cigList != NULL) && (g_isoMng.cisList != NULL) &&
        (g_isoMng.bigBlock.bigList != NULL) && (g_isoMng.bigBlock.syncList != NULL)) {
        g_isoMng.isEnable = true;
        IsoRegisterHciEventCallbacks();
        IsoRegisterHciDataCallbacks();
        // 0x0060: read the ISO buffer size; drives the HCI ISO data fragmentation granularity
        HCI_LeReadBufferSizeV2();
    }
}

static void IsoEnable(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    int ret = IsoRunTaskBlockProcess(IsoEnableTask, NULL);
    if (ret != BT_SUCCESS) {
        LOG_ERROR("%{public}s: Run task error.", __FUNCTION__);
    }
}

static void IsoDisableTask(void *ctx)
{
    LOG_DEBUG("%{public}s:", __FUNCTION__);

    (void)ctx;
    IsoDeregisterHciEventCallbacks();
    IsoDeregisterHciDataCallbacks();
    if (g_isoMng.cigBlock.cigList != NULL) {
        ListClear(g_isoMng.cigBlock.cigList);
    }
    if (g_isoMng.cisList != NULL) {
        ListClear(g_isoMng.cisList);
    }
    if (g_isoMng.bigBlock.bigList != NULL) {
        ListClear(g_isoMng.bigBlock.bigList);
    }
    if (g_isoMng.bigBlock.syncList != NULL) {
        ListClear(g_isoMng.bigBlock.syncList);
    }
    // A RemoveCIG whose Complete event never arrived (callbacks are deregistered
    // above) would otherwise block every later ISOIF_LeRemoveCig with BT_ALREADY.
    g_isoMng.removePending.valid = false;
    // Disarm the Accept CIS watchdogs so a pending entry neither synthesizes a
    // callback after deregistration nor blocks a re-enabled module; the timers are
    // kept for reuse and deleted in IsoFinalizeTask.
    for (uint8_t i = 0; i < ISO_LE_CIS_COUNT_MAX; i++) {
        if (g_isoMng.acceptCisPendings[i].valid) {
            g_isoMng.acceptCisPendings[i].valid = false;
            if (g_isoMng.acceptCisPendings[i].timer != NULL) {
                AlarmCancel(g_isoMng.acceptCisPendings[i].timer);
            }
        }
    }
    g_isoMng.callback = NULL;
    g_isoMng.callbackContext = NULL;
    g_isoMng.bigCallback = NULL;
    g_isoMng.bigCallbackContext = NULL;
    g_isoMng.dataPathCallback = NULL;
    g_isoMng.dataPathCallbackContext = NULL;
    g_isoMng.testCallback = NULL;
    g_isoMng.testCallbackContext = NULL;
    g_isoMng.statusQueryCallback = NULL;
    g_isoMng.statusQueryCallbackContext = NULL;
    g_isoMng.sduCallback = NULL;
    g_isoMng.sduCallbackContext = NULL;
    g_isoMng.isEnable = false;
}

static void IsoDisable(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    int ret = IsoRunTaskBlockProcess(IsoDisableTask, NULL);
    if (ret != BT_SUCCESS) {
        LOG_ERROR("%{public}s: Run task error.", __FUNCTION__);
    }
}

static void IsoFinalizeTask(void *ctx)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    (void)ctx;
    // Disable first: tasks drained by BTM_DeleteProcessingQueue below (e.g. queued HCI event
    // dispatches or ISOIF_* tasks) observe isEnable == false, so their entry points return
    // BT_BAD_STATUS instead of touching the lists that are released right after the drain.
    g_isoMng.isEnable = false;
    // Disarm both watchdogs first, then drain the queue, then delete the alarms. Deleting
    // before the drain would deadlock: AlarmDelete waits for an in-flight dispatch to finish
    // (ReactorUnregister takes the alarm's item lock), and an in-flight watchdog callback
    // blocks in the *blocking* enqueue of BTM_RunTaskInProcessingQueue when the ISO queue is
    // full - the queue slot can only be freed by this thread draining it, while this thread
    // waits for the callback to finish. A timer that fires in the finalize window (the
    // removePending timer is not cancelled in IsoDisableTask, so it is the trigger source) is
    // disarmed by the Cancel; the drain below then runs the at most one queued callback task
    // (its SemaphoreWait is satisfied as the drain pops slots) and it releases the alarm lock,
    // so the Delete after the drain cannot block.
    if (g_isoMng.removePending.timer != NULL) {
        AlarmCancel(g_isoMng.removePending.timer);
    }
    for (uint8_t i = 0; i < ISO_LE_CIS_COUNT_MAX; i++) {
        if (g_isoMng.acceptCisPendings[i].timer != NULL) {
            AlarmCancel(g_isoMng.acceptCisPendings[i].timer);
        }
    }
    // Drain the queue before releasing the lists: BTM_DeleteProcessingQueue runs the pending
    // tasks (e.g. queued HCI event dispatches) synchronously while draining, and those tasks
    // may still touch the lists below. The timeout tasks only touch the entries' valid flags
    // (never the timers), so a task drained here cannot touch a deleted alarm.
    BTM_DeleteProcessingQueue(PROCESSING_QUEUE_ID_ISO);

    if (g_isoMng.removePending.timer != NULL) {
        AlarmDelete(g_isoMng.removePending.timer);
        g_isoMng.removePending.timer = NULL;
    }
    // Release the Accept CIS watchdog timers; the synthesized callback is gated on isEnable,
    // already false here.
    for (uint8_t i = 0; i < ISO_LE_CIS_COUNT_MAX; i++) {
        if (g_isoMng.acceptCisPendings[i].timer != NULL) {
            AlarmDelete(g_isoMng.acceptCisPendings[i].timer);
            g_isoMng.acceptCisPendings[i].timer = NULL;
        }
    }

    ListDelete(g_isoMng.cigBlock.cigList);
    ListDelete(g_isoMng.cisList);
    ListDelete(g_isoMng.bigBlock.bigList);
    ListDelete(g_isoMng.bigBlock.syncList);

    (void)memset_s(&g_isoMng, sizeof(IsoLeMng), 0x00, sizeof(IsoLeMng));
}

static void IsoFinalize(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    int ret = IsoRunTaskBlockProcess(IsoFinalizeTask, NULL);
    if (ret != BT_SUCCESS) {
        LOG_ERROR("%{public}s: Run task error.", __FUNCTION__);
    }
}

bool IsoIsEnable(void)
{
    return g_isoMng.isEnable;
}

IsoLeMng *IsoGetMng(void)
{
    return &g_isoMng;
}

Module g_iso = {
    .name = MODULE_NAME_ISO,
    .init = IsoInitialize,
    .startup = IsoEnable,
    .shutdown = IsoDisable,
    .cleanup = IsoFinalize,
    .dependencies = { MODULE_NAME_HCI },
};

MODULE_DECL(g_iso)
