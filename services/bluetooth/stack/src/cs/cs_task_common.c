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

#include <stdint.h>

#include "btstack.h"

#include "allocator.h"
#include "log.h"

// Event/EventCreate/EventWait and the EVENT_WAIT_* results. The sibling task_common
// files get these through their module-internal header (gap_task_internal.h:19); the CS
// engine's cs_internal.h is the engine seam and holds no Event, so it is included here.
#include "platform/include/event.h"

#include "btm/btm_thread.h"

#include "cs_internal.h"

// The dispatch the CSIF_ entries run on (the plan's option A for the CS threading
// question). The CS engine's state - the connection table, the capability caches, the
// test record - is written by the HCI event callbacks, and those run on the Stack thread
// inside the LE Meta dispatch. A consumer calling a CSIF_ entry from its own thread would
// touch that same state from a second thread; running the entry here instead puts it on
// the Stack thread, so the state has one thread and needs no lock. cs_if.c wraps every
// CSIF_ entry in this, and CSIF_RegisterLeCsCallback documents the same for the callbacks
// that come back the other way.
// The shape is GapRunTaskBlockProcess's (src/gap/gap_task_common.c): the caller blocks
// until the task has run, and a caller already on the Stack thread runs the task inline
// instead of waiting for a queue this very thread would have to drain.

// Initial reference count of a block task context: one reference held by the task in the
// processing queue, one held by the caller until its wait returns.
#define TASK_REF_INITIAL_COUNT 2
#define CS_TASK_WAIT_TIME (-1)

typedef struct {
    Event *event;
    void *ctx;
    void (*func)(void *);
    uint32_t ref;
} CsRunTaskBlockInfo;

// The queued half: run the caller's function, signal, then drop the task's reference. A
// caller whose wait already returned has dropped its own, so the last reference out frees
// the box.
static void CsBlockInTaskProcess(void *ctx)
{
    CsRunTaskBlockInfo *info = ctx;
    if (info->func != NULL) {
        info->func(info->ctx);
    }
    if (info->event != NULL) {
        EventSet(info->event);
    }
    if (__atomic_sub_fetch(&info->ref, 1, __ATOMIC_SEQ_CST) == 0) {
        EventDelete(info->event);
        MEM_MALLOC.free(info);
    }
}

// Contract of the inline fast path below, none of it enforced by code:
// - func must not be NULL. The queued path tolerates it (CsBlockInTaskProcess checks),
//   the fast path calls it directly. Every caller is a static function in cs_if.c.
// - It bypasses the queue, so it can reorder CS tasks: one enqueued earlier by another
//   thread may still be queued while this one runs ahead of it. No CSIF_ task has an
//   ordering dependency on another - each reads or writes module state and, at most,
//   sends one HCI command, and none of them waits on anything but its own result.
// - The queue is not checked, so a BT_SUCCESS here does not mean the task was queued.
int CsRunTaskBlockProcess(void (*func)(void *), void *ctx)
{
    // ThreadIsSelf returns 0 when the calling thread IS the processing thread, so this is
    // the reentrant case: a consumer reading back from inside a CS callback. Enqueuing and
    // waiting here would deadlock - the thread would wait for itself to drain the queue.
    Thread *processingThread = BTM_GetProcessingThread();
    if (processingThread != NULL && ThreadIsSelf(processingThread) == 0) {
        func(ctx);
        return BT_SUCCESS;
    }

    CsRunTaskBlockInfo *info = MEM_MALLOC.alloc(sizeof(CsRunTaskBlockInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    // Non-autoClear on purpose: if an EVENT_WAIT_OTHER_ERR return raced with the task's
    // EventSet, an autoClear variant would consume the signal inside the failed wait and
    // the retry below would then block forever (the task never sets it again).
    info->event = EventCreate(false);
    if (info->event == NULL) {
        MEM_MALLOC.free(info);
        return BT_NO_MEMORY;
    }
    info->ctx = ctx;
    info->func = func;
    info->ref = TASK_REF_INITIAL_COUNT;

    int postRet = BTM_RunTaskInProcessingQueue(PROCESSING_QUEUE_ID_CS, CsBlockInTaskProcess, info);
    int ret = postRet;
    if (postRet == BT_SUCCESS) {
        ret = EventWait(info->event, CS_TASK_WAIT_TIME);
        // An OS-level wait failure (e.g. EINTR) is transient and must be retried rather
        // than surfaced: the task is already queued, so returning an error here would let
        // the caller free ctx while the task is still running on it. The task always sets
        // the event before finishing, so the retry converges.
        while (ret == EVENT_WAIT_OTHER_ERR) {
            ret = EventWait(info->event, CS_TASK_WAIT_TIME);
        }
        if (ret == 0) {
            // Success: clear the signal explicitly (EventCreate(false)). The queued task's
            // EventSet happens-before this wait returned, so no race.
            EventClear(info->event);
        }
        if (ret == EVENT_WAIT_TIMEOUT_ERR) {
            // Unreachable while CS_TASK_WAIT_TIME is -1. It is kept because it is the one
            // return that does hand the caller an error with the task possibly still
            // running - the caller must not reuse or free ctx on this return. Reporting it
            // beats pretending the call completed; GapRunTaskBlockProcess keeps the same
            // branch.
            LOG_ERROR("%{public}s: EventWait result is timeout", __FUNCTION__);
            ret = BT_TIMEOUT;
        }
    }

    if (postRet != BT_SUCCESS || __atomic_sub_fetch(&info->ref, 1, __ATOMIC_SEQ_CST) == 0) {
        EventDelete(info->event);
        MEM_MALLOC.free(info);
    }
    return ret;
}
