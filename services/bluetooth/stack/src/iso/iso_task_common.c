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

#include "iso_task_internal.h"

#include "btstack.h"

#include "allocator.h"
#include "module.h"

#include "btm/btm_thread.h"
#include "log.h"

// Initial reference count of a block task context: one reference held by the
// task in the processing queue, one held by the caller until the wait returns.
#define ISO_TASK_REF_INITIAL_COUNT 2

void IsoBlockInTaskProcess(void *ctx)
{
    IsoRunTaskBlockInfo *info = ctx;
    if (info->func != NULL) {
        info->func(info->ctx);
    }
    if (info->event != NULL) {
        EventSet(info->event);
    }
    // Drop the task's reference; caller may have already dropped its reference on timeout.
    if (__atomic_sub_fetch(&info->ref, 1, __ATOMIC_SEQ_CST) == 0) {
        EventDelete(info->event);
        MEM_MALLOC.free(info);
    }
}

NO_SANITIZE("cfi") void IsoUnBlockInTaskProcess(void *ctx)
{
    IsoRunTaskUnBlockInfo *info = ctx;
    if (info != NULL) {
        if (info->func != NULL) {
            info->func(info->ctx);
        }
        if (info->freeCtx != NULL) {
            // freeCtx owns the context release (it may also handle non-heap contexts)
            info->freeCtx(info->ctx);
        } else if (info->ctx != NULL) {
            MEM_MALLOC.free(info->ctx);
        }
        MEM_MALLOC.free(info);
    }
}

int IsoRunTaskBlockProcess(void (*func)(void *), void *ctx)
{
    // All BTM processing queues (ISO included) are drained by the single Stack thread
    // (BTM_GetProcessingThread). A caller already running on that thread (e.g. an ISO
    // result callback invoking an ISOIF_* API) must not enqueue the task and wait on the
    // event: the thread would block forever waiting for itself to drain the queue.
    // Execute the task directly instead (same reentrant guard as RunAllTaskInQueue in
    // btm_thread.c). The task functions only send fire-and-forget HCI commands, so no
    // blocking wait is re-entered.
    //
    // The wait below is unbounded (ISO_WAIT_TIME == -1) and has no watchdog by design:
    // the queued task always EventSets before finishing, so the wait only hangs if the
    // Stack thread itself is wedged, which no bounded wait could recover from either.
    // Accepted pattern, mirrors GapRunTaskBlockProcess.
    Thread *processingThread = BTM_GetProcessingThread();
    if (processingThread != NULL && ThreadIsSelf(processingThread) == 0) {
        func(ctx);
        return BT_SUCCESS;
    }

    IsoRunTaskBlockInfo *info = MEM_MALLOC.alloc(sizeof(IsoRunTaskBlockInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }

    // Non-autoClear on purpose: if an EVENT_WAIT_OTHER_ERR return raced with the task's
    // EventSet, the autoClear variant would consume the signal inside the failed wait and
    // the retry below would then block forever (the task never sets it again). With
    // autoClear off the signal survives the failed wait, so the retry converges.
    info->event = EventCreate(false);
    if (info->event == NULL) {
        MEM_MALLOC.free(info);
        return BT_NO_MEMORY;
    }
    info->ctx = ctx;
    info->func = func;
    info->ref = ISO_TASK_REF_INITIAL_COUNT;  // One reference held by the task, one by the caller.

    int postRet = BTM_RunTaskInProcessingQueue(PROCESSING_QUEUE_ID_ISO, IsoBlockInTaskProcess, info);
    int ret = postRet;
    if (postRet == BT_SUCCESS) {
        ret = EventWait(info->event, ISO_WAIT_TIME);
        // EVENT_WAIT_OTHER_ERR is an OS-level wait failure (e.g. EINTR from a
        // signal). The task is already in the queue and always EventSets before
        // finishing, so the wait always converges. This error must never be
        // surfaced to callers: they would free ctx while the task may still
        // execute on it (use-after-free), which is why the wait below is
        // unbounded rather than retry-capped.
        while (ret == EVENT_WAIT_OTHER_ERR) {
            HILOGW("EventWait result is wait err, retrying");
            ret = EventWait(info->event, ISO_WAIT_TIME);
        }
        if (ret == 0) {
            // Success: clear the signal explicitly (EventCreate(false)). The queued
            // task's EventSet happens-before this wait returned, so no race.
            EventClear(info->event);
        }
        if (ret == EVENT_WAIT_TIMEOUT_ERR) {
            // Unreachable while ISO_WAIT_TIME is -1 (infinite wait); kept as a
            // defensive guard. If ISO_WAIT_TIME ever becomes finite, callers that
            // free ctx on this error would need to stop doing so, because the
            // task is still queued and will run.
            HILOGE("EventWait result is timeout");
            ret = BT_TIMEOUT;
        }
    }

    if (postRet != BT_SUCCESS || __atomic_sub_fetch(&info->ref, 1, __ATOMIC_SEQ_CST) == 0) {
        EventDelete(info->event);
        MEM_MALLOC.free(info);
    } else {
        // IsoBlockInTaskProcess is still pending or running. It owns the final reference
        // and will release event/info once it completes. This is the normal interleaving
        // when the caller decrements its reference before the task does (e.g. right
        // after EventWait), not an error.
        HILOGD("Leave task resources to be freed by pending task.");
    }
    return ret;
}

int IsoRunTaskUnBlockProcess(void (* const func)(void *), void *ctx, void (* const freeCtx)(void *))
{
    // Same-thread short-circuit (mirror IsoRunTaskBlockProcess): the ISO queue is
    // drained by the single Stack thread, and callers already running on that thread
    // (documented usage: ISOIF_LeSendIsoData from the sduReceivedInd callback) must
    // not block on the enqueue semaphore when the queue is full - the thread would
    // wait forever for itself to drain it. Execute inline instead, matching
    // IsoUnBlockInTaskProcess's ownership: func runs first, then freeCtx (or a plain
    // free when freeCtx is NULL) releases ctx. The task bodies only build packets
    // and enqueue HCI data, so no blocking wait is re-entered.
    Thread *processingThread = BTM_GetProcessingThread();
    if (processingThread != NULL && ThreadIsSelf(processingThread) == 0) {
        if (func != NULL) {
            func(ctx);
        }
        if (freeCtx != NULL) {
            freeCtx(ctx);
        } else if (ctx != NULL) {
            MEM_MALLOC.free(ctx);
        }
        return BT_SUCCESS;
    }

    IsoRunTaskUnBlockInfo *info = MEM_MALLOC.alloc(sizeof(IsoRunTaskUnBlockInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }

    info->ctx = ctx;
    info->func = func;
    info->freeCtx = freeCtx;

    int ret = BTM_RunTaskInProcessingQueue(PROCESSING_QUEUE_ID_ISO, IsoUnBlockInTaskProcess, info);
    if (ret != BT_SUCCESS) {
        MEM_MALLOC.free(info);
    }
    return ret;
}

int IsoRunTaskUnBlockProcessNoBlock(void (* const func)(void *), void *ctx, void (* const freeCtx)(void *))
{
    IsoRunTaskUnBlockInfo *info = MEM_MALLOC.alloc(sizeof(IsoRunTaskUnBlockInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }

    info->ctx = ctx;
    info->func = func;
    info->freeCtx = freeCtx;

    // Non-blocking enqueue: a full ISO queue drops the task (see the NoBlock declaration
    // in iso_task_internal.h). The HCI event dispatch runs on the Stack thread that also
    // drains this queue, so dropping events under a storm is preferred over hanging the
    // whole Stack thread on the enqueue semaphore.
    int ret = BTM_RunTaskInProcessingQueueNoBlock(PROCESSING_QUEUE_ID_ISO, IsoUnBlockInTaskProcess, info);
    if (ret != BT_SUCCESS) {
        MEM_MALLOC.free(info);
    }
    return ret;
}
