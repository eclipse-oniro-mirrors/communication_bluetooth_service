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

#include "gap_def.h"
#include "gap_task_internal.h"

#include "allocator.h"

#include "btm/btm_thread.h"
#include "log.h"

// Initial reference count of a block task context: one reference held by the
// task in the processing queue, one held by the caller until the wait returns.
#define TASK_REF_INITIAL_COUNT 2

void GapBlockInTaskProcess(void *ctx)
{
    GapRunTaskBlockInfo *info = ctx;
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

NO_SANITIZE("cfi") void GapUnBlockInTaskProcess(void *ctx)
{
    GapRunTaskUnBlockInfo *info = ctx;
    if (info != NULL) {
        if (info->func != NULL) {
            info->func(info->ctx);
        }
        if (info->free != NULL) {
            // |cleanup| owns the entire context, including |ctx| itself.
            info->free(info->ctx);
        } else if (info->ctx != NULL) {
            MEM_MALLOC.free(info->ctx);
        }
        MEM_MALLOC.free(info);
    }
}

int GapRunTaskBlockProcess(void (*func)(void *), void *ctx)
{
    // All BTM processing queues (GAP included) are drained by the single Stack thread
    // (BTM_GetProcessingThread). A caller already running on that thread (e.g. a GAP
    // result callback invoking a GAPIF_* API) must not enqueue the task and wait on the
    // event: the thread would block forever waiting for itself to drain the queue.
    // Execute the task directly instead (same reentrant guard as RunAllTaskInQueue in
    // btm_thread.c). The task functions only send fire-and-forget HCI commands and read
    // the result asynchronously via registered callbacks, so no blocking wait is
    // re-entered.
    // NOTE: ThreadIsSelf returns 0 when the calling thread IS the processing thread (see
    // thread_linux.c), so the fast path below runs the task inline on the Stack thread.
    //
    // Ordering contract of this fast path: the inline execution bypasses the GAP
    // processing queue, so it can reorder tasks of the same module - a task enqueued
    // earlier by an external thread T1 may still sit in the queue while this task runs
    // ahead of it. Callers MUST therefore ensure the task has no ordering dependency on
    // other queued GAP tasks and never blocks (the fast path runs synchronously on the
    // caller's stack, and no queue-level gate protects a teardown window). This holds
    // for all current call sites (see gap_le_if.c, gap.c, gap_le_power.c): each task is
    // self-contained - it only sends fire-and-forget HCI commands, copies parameters
    // into the task context, or reads a result written by the task itself.
    //
    // Defensive notes (contract of the fast path, not enforced by code):
    // - func must not be NULL: unlike the slow path, whose queued wrapper
    //   (GapBlockInTaskProcess) tolerates a NULL func, the fast path invokes
    //   func(ctx) directly and would crash on a NULL function pointer.
    // - Queue availability is NOT checked: the slow path returns the queue's error
    //   (BTM_RunTaskInProcessingQueue) when the GAP queue is unavailable or being torn
    //   down, while the fast path always reports success. Callers must not assume a
    //   BT_SUCCESS here implies the task was queued when running on the Stack thread.
    Thread *processingThread = BTM_GetProcessingThread();
    if (processingThread != NULL && ThreadIsSelf(processingThread) == 0) {
        func(ctx);
        return BT_SUCCESS;
    }

    GapRunTaskBlockInfo *info = MEM_MALLOC.alloc(sizeof(GapRunTaskBlockInfo));
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
    info->ref = TASK_REF_INITIAL_COUNT;  // One reference held by the task, one by the caller.

    int postRet = BTM_RunTaskInProcessingQueue(PROCESSING_QUEUE_ID_GAP, GapBlockInTaskProcess, info);
    int ret = postRet;
    if (postRet == BT_SUCCESS) {
        ret = EventWait(info->event, WAIT_TIME);
        // EVENT_WAIT_OTHER_ERR is an OS-level wait failure (e.g. EINTR from a
        // signal). The task is already in the queue and always EventSets before
        // finishing, so the wait always converges. This error must never be
        // surfaced to callers: they would free ctx while the task may still
        // execute on it (use-after-free), which is why the wait below is
        // unbounded rather than retry-capped.
        while (ret == EVENT_WAIT_OTHER_ERR) {
            HILOGW("EventWait result is wait err, retrying");
            ret = EventWait(info->event, WAIT_TIME);
        }
        if (ret == 0) {
            // Success: clear the signal explicitly (EventCreate(false)). The queued
            // task's EventSet happens-before this wait returned, so no race.
            EventClear(info->event);
        }
        if (ret == EVENT_WAIT_TIMEOUT_ERR) {
            // Unreachable while WAIT_TIME is -1 (infinite wait); kept as a
            // defensive guard. If WAIT_TIME ever becomes finite, callers that
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
        // GapBlockInTaskProcess is still pending or running. It owns the final reference
        // and will release event/info once it completes. This is the normal interleaving
        // when the caller decrements its reference before the task does (e.g. right
        // after EventWait), not an error.
        HILOGD("Leave task resources to be freed by pending task.");
    }
    return ret;
}

int GapRunTaskUnBlockProcess(void (*const func)(void *), void *ctx, void (*const cleanup)(void *))
{
    GapRunTaskUnBlockInfo *info = MEM_MALLOC.alloc(sizeof(GapRunTaskUnBlockInfo));
    if (info == NULL) {
        // Ownership of |ctx| could not be transferred to the queue.
        // |cleanup| (if provided) releases all resources owned by |ctx|, including |ctx| itself.
        if (cleanup != NULL) {
            cleanup(ctx);
        } else {
            MEM_MALLOC.free(ctx);
        }
        return BT_NO_MEMORY;
    }

    info->ctx = ctx;
    info->func = func;
    info->free = cleanup;

    int ret = BTM_RunTaskInProcessingQueue(PROCESSING_QUEUE_ID_GAP, GapUnBlockInTaskProcess, info);
    if (ret != BT_SUCCESS) {
        // The queue did not take ownership of |info|; release it along with the
        // attached context. |cleanup| (if provided) releases all resources owned by |ctx|,
        // including |ctx| itself.
        if (info->free != NULL) {
            info->free(info->ctx);
        } else {
            MEM_MALLOC.free(info->ctx);
        }
        MEM_MALLOC.free(info);
    }

    return ret;
}
