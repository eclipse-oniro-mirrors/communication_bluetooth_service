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
    GapRunTaskBlockInfo *info = MEM_MALLOC.alloc(sizeof(GapRunTaskBlockInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }

    info->event = EventCreate(true);
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
