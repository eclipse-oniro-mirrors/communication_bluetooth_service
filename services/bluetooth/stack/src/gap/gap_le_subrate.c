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

// LE Connection Subrating (BLUETOOTH SPECIFICATION Version 5.3, Vol 4, Part E
// 7.8.123/7.8.124 commands and 7.7.65,35 LE Subrate Change subevent 0x23).
//
// Callback-group and lifecycle model mirrors gap_le_power.c: a single
// registered GapLeSubrateCallback, copied by value, with ref-counted dispatch
// and a lifecycle mutex that is created once and intentionally never destroyed.
// The subrating event and command-complete payloads are keyed by connection
// handle (the subrate event) or carry no handle at all (both completions are
// status-only), so no connection-record/address lookup is needed and the group
// is deliberately separate from the address-keyed GapLeConnCallback group.

#include "gap_def.h"
#include "gap_internal.h"
#include "gap_le.h"
#include "gap_task_internal.h"

#include <securec.h>

#include "allocator.h"
#include "log.h"
#include "platform/include/event.h"
#include "platform/include/mutex.h"

#include "btm/btm_thread.h"

// Connection Subrating parameter limits (verified against the amended 2024
// spec tables of 7.8.123/7.8.124) live in hci_def_le_cmd.h as LE_SUBRATE_*:
// single maintenance point shared with the sender-side gates in
// hci_cmd_le_controller_5_3.c. Do not redefine them here.

typedef struct {
    GapLeSubrateCallback callback;
    void *context;
} GapLeSubrateCallbackBlock;

static GapLeSubrateCallbackBlock g_subrateCallback;
static Mutex *g_subrateCallbackMutex = NULL;
// Forward declaration: GAP_DeregisterLeSubrateCallback waits for in-flight
// references to drain, and the drain helper is defined below it.
static bool GapLeSubrateCallbackWaitRefsDrain(void);
// Number of in-flight subrate callbacks. Incremented in the getter while
// the lifecycle mutex is still held (at inner-mutex pointer capture), decremented
// by the caller after the callback returns. Deinit waits for this to reach zero
// before clearing the callback state and deleting the mutex, so upper-layer
// context pointers stay valid.
static int32_t g_subrateCallbackRef = 0;
// Set while a subrate dispatch is invoking the upper-layer callback (the
// window between GapLeSubrateCallbackGet and GapLeSubrateCallbackRelease).
// Thread-local: only the dispatching thread observes it. Used by Deregister to
// detect a re-entrant call made from inside a subrate callback, where the
// only outstanding reference is the dispatcher's own and draining it would spin
// for 60 s on the single Stack thread.
static __thread bool g_subrateCallbackDispatching = false;

// Protects creation/destruction of the callback mutex and concurrent reads of
// its global pointer. Created once in GapLeSubrateCallbackInit and
// intentionally never destroyed to avoid use-after-free during teardown races.
// Contract: GapLeSubrateCallbackInit and GapLeSubrateCallbackDeinit
// must be called serially by a single thread/task. After Deinit, registration
// and lookup APIs return GAP_ERR_OUT_OF_RES until Init is called again.
static Mutex *g_subrateCallbackLifecycleMutex = NULL;
// Set while GapLeSubrateCallbackDeinit is tearing the callback mutex down.
// Registration and lookup APIs must fail instead of re-creating a mutex that is
// about to be deleted.
static bool g_subrateCallbackDeinitInProgress = false;
// Signaled when the reference count reaches zero so Deinit can avoid spinning.
static Event *g_subrateCallbackDeinitEvent = NULL;

// Number of drain wait iterations: 12 * 5000 ms = 60 s.
#define GAP_LE_SUBRATE_CALLBACK_DRAIN_WAIT_RETRIES (12)
#define GAP_LE_SUBRATE_CALLBACK_DRAIN_WAIT_MS (5000)
// GAP_LE_CONNECTION_HANDLE_MAX (0x0EFF, ACL 12-bit handle range) comes from gap_internal.h.

// Shared validation of the four subrating value fields plus supervision
// timeout (7.8.123/7.8.124 field tables). Returns false on the first violated
// restriction so the GAP entry points reject what the Controller would refuse
// with 0x12 (invalid command parameters) anyway. Continuation_Number is
// 2 octets on the wire (verified against the amended 2024 spec) and must stay
// below the requested Subrate_Max.
// Deliberately duplicated as HciSubrateParamValid in hci_cmd_le_controller_5_3.c
// (defense in depth); both consume the shared LE_SUBRATE_* limits from
// hci_def_le_cmd.h, so only the predicate shape needs to stay in sync.
static bool GapLeSubrateParamsValid(uint16_t subrateMin, uint16_t subrateMax, uint16_t maxLatency,
    uint16_t continuationNumber, uint16_t supervisionTimeout)
{
    if (subrateMin < LE_SUBRATE_FACTOR_MIN || subrateMax < LE_SUBRATE_FACTOR_MIN) {
        return false; // 0 is reserved
    }
    if (subrateMax > LE_SUBRATE_FACTOR_MAX || subrateMin > subrateMax) {
        return false;
    }
    if (maxLatency > LE_SUBRATE_MAX_LATENCY_MAX) {
        return false;
    }
    // Subrate_Max x (Max_Latency + 1) <= 500 and Continuation_Number < Subrate_Max.
    if (subrateMax > LE_SUBRATE_MAX_LATENCY_PRODUCT_MAX / (maxLatency + 1)) {
        return false;
    }
    if (continuationNumber > LE_SUBRATE_CONTINUATION_MAX || continuationNumber >= subrateMax) {
        return false;
    }
    if (supervisionTimeout < LE_SUBRATE_SUPERVISION_TIMEOUT_MIN ||
        supervisionTimeout > LE_SUBRATE_SUPERVISION_TIMEOUT_MAX) {
        return false;
    }
    return true;
}

// The context pointer returned by the getter is owned by the upper layer and
// must remain valid until the callback is deregistered and all in-flight events
// have been delivered. The lifecycle mutex is only held during lookup; it is
// released before the callback runs.
// Thread model: under the current single-Stack-thread design, Get/Release are
// called only from HCI callbacks on the Stack thread and Deinit from the GAP
// deinit on the same thread, so the reference is always 0 when Deinit runs and
// the drain below always succeeds immediately - the ref/Event mechanism is
// currently redundant and exists for a future dispatcher that may run
// callbacks off the Stack thread. The invariant that keeps it correct there
// is: Deinit must never delete the mutex while a reference is outstanding
// (the timeout path leaves it allocated), and the reference must have reached
// 0 before the mutex is ever recreated by Init - otherwise a stale Release
// could drive the counter negative and a later Deinit would misjudge a
// re-populated ref count as drained. The reference is taken at pointer-capture
// time while the lifecycle lock is still held (see GapLeSubrateCallbackGet),
// so Deinit's drain covers every path that observed the inner mutex pointer,
// not only dispatches that already acquired the inner mutex; after Deinit
// NULLs the pointer under the lifecycle lock, no new reference can be taken
// and the counter only decreases.
static bool GapLeSubrateCallbackGet(GapLeSubrateCallback *callback, void **context)
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

    Mutex *lifecycleMutex = __atomic_load_n(&g_subrateCallbackLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        *context = NULL;
        return false;
    }

    MutexLock(lifecycleMutex);
    if (g_subrateCallbackDeinitInProgress) {
        MutexUnlock(g_subrateCallbackLifecycleMutex);
        (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        *context = NULL;
        return false;
    }

    Mutex *mutex = g_subrateCallbackMutex;
    if (mutex == NULL) {
        MutexUnlock(g_subrateCallbackLifecycleMutex);
        (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        *context = NULL;
        return false;
    }

    // Take the reference while still holding the lifecycle lock, right at
    // pointer capture. Deinit NULLs the global mutex pointer under this same
    // lock and only drains this counter before MutexDelete, so any Get that
    // observed a non-NULL pointer has already counted itself and Deinit can
    // never delete a mutex that a Get is about to lock. After the pointer is
    // NULLed, new Gets fail above and the counter only decreases.
    (void)__atomic_fetch_add(&g_subrateCallbackRef, 1, __ATOMIC_SEQ_CST);

    MutexLock(mutex);
    *callback = g_subrateCallback.callback;
    *context = g_subrateCallback.context;
    MutexUnlock(mutex);
    MutexUnlock(g_subrateCallbackLifecycleMutex);
    return true;
}

static void GapLeSubrateCallbackRelease(void)
{
    if (__atomic_fetch_sub(&g_subrateCallbackRef, 1, __ATOMIC_SEQ_CST) == 1) {
        Event *event = __atomic_load_n(&g_subrateCallbackDeinitEvent, __ATOMIC_ACQUIRE);
        if (event != NULL) {
            EventSet(event);
        }
    }
}

// Marks the start of the upper-layer callback invocation inside a dispatch
// (see g_subrateCallbackDispatching). Called right after the getter
// succeeds, before the callback is invoked.
static void GapLeSubrateDispatchBegin(void)
{
    g_subrateCallbackDispatching = true;
}

// Clears the dispatch window and releases the reference held by the getter.
// Called right after the callback returns; the flag must be cleared before the
// release so that a Deregister invoked from inside the callback (which runs
// before this point on the same thread) observes the flag while it is still set.
static void GapLeSubrateDispatchEnd(void)
{
    g_subrateCallbackDispatching = false;
    GapLeSubrateCallbackRelease();
}

// 7.8.123 LE Set Default Subrate: stores the default subrating parameters the
// host advertises to the peer as its own default capabilities on connections.
int GAP_LeSetDefaultSubrate(const GapLeSubrateDefaultParams *params)
{
    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (params == NULL) {
        return BT_BAD_PARAM;
    }

    if (!GapLeSubrateParamsValid(params->defaultSubrateMin, params->defaultSubrateMax, params->defaultMaxLatency,
        params->defaultContinuationNumber, params->defaultSupervisionTimeout)) {
        return BT_BAD_PARAM;
    }

    HciLeSetDefaultSubrateParam hciCmdParam = {
        .defaultSubrateMin = params->defaultSubrateMin,
        .defaultSubrateMax = params->defaultSubrateMax,
        .defaultMaxLatency = params->defaultMaxLatency,
        .defaultContinuationNumber = params->defaultContinuationNumber,
        .defaultSupervisionTimeout = params->defaultSupervisionTimeout,
    };

    return HCI_LeSetDefaultSubrate(&hciCmdParam);
}

// 7.8.124 LE Subrate Request: both the Central and the Peripheral may request
// a new subrate factor on an existing connection (no role gate here).
int GAP_LeSubrateRequest(const GapLeSubrateRequestParams *params)
{
    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (params == NULL) {
        return BT_BAD_PARAM;
    }

    if (params->connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    if (!GapLeSubrateParamsValid(params->subrateMin, params->subrateMax, params->maxLatency,
        params->continuationNumber, params->supervisionTimeout)) {
        return BT_BAD_PARAM;
    }

    HciLeSubrateRequestParam hciCmdParam = {
        .connectionHandle = params->connectionHandle,
        .subrateMin = params->subrateMin,
        .subrateMax = params->subrateMax,
        .maxLatency = params->maxLatency,
        .continuationNumber = params->continuationNumber,
        .supervisionTimeout = params->supervisionTimeout,
    };

    return HCI_LeSubrateRequest(&hciCmdParam);
}

void GapLeSetDefaultSubrateComplete(const HciLeSetDefaultSubrateReturnParam *param)
{
    GapLeSubrateCallback callback;
    void *context = NULL;
    if (!GapLeSubrateCallbackGet(&callback, &context)) {
        return;
    }
    GapLeSubrateDispatchBegin();
    if (callback.setDefaultSubrateResult != NULL) {
        callback.setDefaultSubrateResult(param->status, context);
    }
    GapLeSubrateDispatchEnd();
}

NO_SANITIZE("cfi") void GapLeSubrateRequestComplete(const HciLeSubrateRequestReturnParam *param)
{
    GapLeSubrateCallback callback;
    void *context = NULL;
    if (!GapLeSubrateCallbackGet(&callback, &context)) {
        return;
    }
    GapLeSubrateDispatchBegin();
    if (callback.subrateRequestResult != NULL) {
        callback.subrateRequestResult(param->status, context);
    }
    GapLeSubrateDispatchEnd();
}

NO_SANITIZE("cfi") void GapLeSubrateChangeEvent(const HciLeSubrateChangeEventParam *param)
{
    // Delivered on the Stack thread, like every GAP HCI event. Stateless
    // dispatch: the payload carries the connection handle and the values the
    // link layer actually applied, so no state is kept in this module and no
    // filtering is performed here. Delivered for both locally requested and
    // peer/LL-initiated subrate changes (also when the LL resets the factor to
    // 1 with continuation 0 at a connection-update instant, 7.8.124 semantics).
    GapLeSubrateChangeInfo info = {
        .status = param->status,
        .connectionHandle = param->connectionHandle,
        .subrateFactor = param->subrateFactor,
        .peripheralLatency = param->peripheralLatency,
        .continuationNumber = param->continuationNumber,
        .supervisionTimeout = param->supervisionTimeout,
    };

    GapLeSubrateCallback callback;
    void *context = NULL;
    if (!GapLeSubrateCallbackGet(&callback, &context)) {
        return;
    }
    GapLeSubrateDispatchBegin();
    if (callback.subrateChange != NULL) {
        callback.subrateChange(&info, context);
    }
    GapLeSubrateDispatchEnd();
}

int GAP_RegisterLeSubrateCallback(const GapLeSubrateCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "NULL");
    // Read the lifecycle mutex atomically: GapLeSubrateCallbackInit
    // publishes it with __atomic_compare_exchange_n, so a plain load here is a
    // data race (benign in practice since the mutex is never destroyed, but
    // flagged by static analyzers). Once obtained, the pointer stays valid for
    // the function's lifetime; the subsequent Unlock sites still reference the
    // global for consistency with the rest of the file.
    Mutex *lifecycleMutex = __atomic_load_n(&g_subrateCallbackLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(lifecycleMutex);
    if (g_subrateCallbackDeinitInProgress) {
        MutexUnlock(g_subrateCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }
    Mutex *mutex = g_subrateCallbackMutex;
    if (mutex == NULL) {
        MutexUnlock(g_subrateCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }
    // Register (like Deregister and Get) locks the inner mutex while still
    // holding the lifecycle lock - there is no capture-unlock-relock window in
    // which Deinit could delete the inner mutex between the pointer capture
    // above and MutexLock(mutex) below: Deinit NULLs the pointer and waits for
    // the reference drain under the same lifecycle lock.

    // Store a by-value copy of the callback structure: the caller may pass a
    // stack-local structure that is invalidated once the call returns, while
    // HCI events are delivered asynchronously on the GAP task thread. The
    // GAPIF_RegisterLeSubrateCallback entry point dispatches this call
    // synchronously via GapRunTaskBlockProcess, so the copy is complete before
    // the entry point returns and a caller-owned structure stays valid for the
    // whole call; nothing is retained past the copy. A NULL callback clears the
    // registered structure, matching GAP_RegisterLeConnCallback.
    MutexLock(mutex);
    if (callback == NULL) {
        (void)memset_s(&g_subrateCallback.callback,
            sizeof(g_subrateCallback.callback),
            0x00,
            sizeof(g_subrateCallback.callback));
        // a NULL callback clears the context as well, otherwise register(NULL, staleCtx)
        // would keep delivering a stale context that no callback can consume
        g_subrateCallback.context = NULL;
    } else {
        g_subrateCallback.callback = *callback;
        g_subrateCallback.context = context;
    }
    MutexUnlock(mutex);
    MutexUnlock(g_subrateCallbackLifecycleMutex);
    return GAP_SUCCESS;
}

int GAP_DeregisterLeSubrateCallback(void)
{
    Mutex *lifecycleMutex = __atomic_load_n(&g_subrateCallbackLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(lifecycleMutex);
    if (g_subrateCallbackDeinitInProgress) {
        MutexUnlock(g_subrateCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }
    Mutex *mutex = g_subrateCallbackMutex;
    if (mutex == NULL) {
        MutexUnlock(g_subrateCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(mutex);
    (void)memset_s(&g_subrateCallback.callback,
        sizeof(g_subrateCallback.callback),
        0x00,
        sizeof(g_subrateCallback.callback));
    g_subrateCallback.context = NULL;
    MutexUnlock(mutex);
    // An in-flight dispatch has already copied the callback by value under the same mutex, so
    // clearing it is safe; waiting for the references to drain guarantees no dispatch of the
    // cleared callback can still be in flight when this returns. Under the current
    // single-Stack-thread model the reference count is already zero here and the wait returns
    // immediately (see GapLeSubrateCallbackWaitRefsDrain).
    bool inCallbackDispatch = g_subrateCallbackDispatching;
    bool refsDrained = inCallbackDispatch || GapLeSubrateCallbackWaitRefsDrain();
    if (inCallbackDispatch) {
        // Re-entrant call made from inside a subrate callback (the GAPIF entry point is
        // dispatched inline on the Stack thread via the GapRunTaskBlockProcess fast path): the
        // only outstanding reference is the one held by this very dispatch, which can only be
        // released after this call returns - draining would spin for 60 s on the Stack thread.
        // The dispatch has already copied the callback by value before invoking it, so the
        // cleared registration can no longer affect this dispatch.
        LOG_INFO("%{public}s: called from inside a subrate callback dispatch; ref drain skipped "
            "(the in-flight dispatch owns the only reference)", __FUNCTION__);
    }
    MutexUnlock(g_subrateCallbackLifecycleMutex);
    if (!refsDrained) {
        // The wait window elapsed while dispatches were still in flight. Note the
        // registration table above is already cleared: no new event will be
        // delivered from here on. The error only means an in-flight dispatch
        // still holds the upper-layer context, which must not be freed yet - a
        // later Deregister (or the module Deinit) retries the drain.
        LOG_ERROR("%{public}s: timeout waiting for in-flight subrate callbacks to release; "
            "registration cleared, outstanding callback contexts must not be freed", __FUNCTION__);
        return GAP_ERR_REMOTE_ACTION;
    }
    return GAP_SUCCESS;
}

// Waits until all in-flight callbacks have released their references, or the
// wait window elapses (event-based wait, falling back to a bounded spin when
// the deinit event is not yet published). Returns true when the reference count
// reached zero, i.e. the callback mutex can be safely deleted.
// Under the current single-Stack-thread model (see GapLeSubrateCallbackGet)
// the reference is always zero here, so the wait never blocks; the timeout path
// only exists for a future cross-thread dispatcher and deliberately leaks the
// mutex instead of deleting it while references may still be outstanding.
static bool GapLeSubrateCallbackWaitRefsDrain(void)
{
    Event *event = __atomic_load_n(&g_subrateCallbackDeinitEvent, __ATOMIC_ACQUIRE);
    if (event != NULL) {
        int32_t waitRetries = GAP_LE_SUBRATE_CALLBACK_DRAIN_WAIT_RETRIES;
        while (__atomic_load_n(&g_subrateCallbackRef, __ATOMIC_SEQ_CST) > 0 && waitRetries > 0) {
            (void)EventWait(event, GAP_LE_SUBRATE_CALLBACK_DRAIN_WAIT_MS);
            waitRetries--;
        }
        if (__atomic_load_n(&g_subrateCallbackRef, __ATOMIC_SEQ_CST) > 0) {
            // Key off the counter, not the retry budget: the final successful
            // EventWait also decrements waitRetries to zero and would otherwise
            // log a timeout for a drain that just succeeded.
            LOG_ERROR("%{public}s: timeout waiting for subrate callbacks to release", __FUNCTION__);
        }
    } else {
        // No drain event exists only in the rollback path of an Init whose
        // EventCreate failed, before any registration is published; under the
        // serial Init/Deinit contract (see GapLeSubrateCallbackGet) no reference
        // can be outstanding there, so this must never loop. If a concurrent
        // misuse ever breaks that invariant, fail fast and leave the mutex to
        // the leak policy (GapLeSubrateRollbackCallbackMutex) instead of burning
        // a core on a million sched_yield calls.
        if (__atomic_load_n(&g_subrateCallbackRef, __ATOMIC_SEQ_CST) != 0) {
            LOG_ERROR("%{public}s: references outstanding without a drain event; "
                "leaving callback mutex allocated", __FUNCTION__);
            return false;
        }
    }
    return true;
}

// Ensure the lifecycle mutex exists before locking. Once created, it is never
// destroyed so that racing registration/lookup paths can always observe the
// initialized/deinitialized state safely. The pointer is read atomically (same as
// GapLeSubrateCallbackGet) so the check is defined even if another thread
// creates the mutex concurrently. Returns false when the mutex could not be created.
static bool GapLeSubrateEnsureLifecycleMutex(void)
{
    if (__atomic_load_n(&g_subrateCallbackLifecycleMutex, __ATOMIC_ACQUIRE) != NULL) {
        return true;
    }
    Mutex *newMutex = MutexCreate();
    if (newMutex == NULL) {
        LOG_ERROR("%{public}s: Lifecycle MutexCreate failed", __FUNCTION__);
        return false;
    }
    Mutex *expected = NULL;
    if (!__atomic_compare_exchange_n(&g_subrateCallbackLifecycleMutex, &expected, newMutex, false,
        __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
        MutexDelete(newMutex);
    }
    return true;
}

// Roll back an Init that failed after the callback mutex was created: mirror the
// Deinit drain - Gets capture the inner-mutex pointer and take a reference under
// the lifecycle lock, so a dispatch may still be locking this mutex. NULL the
// pointer under the lock (no new captures possible), release the lock, drain the
// outstanding references, then delete - exactly Deinit's order. When the drain
// times out the mutex is deliberately leaked (Deinit's policy) instead of being
// deleted under a possible in-flight MutexLock.
static void GapLeSubrateRollbackCallbackMutex(void)
{
    Mutex *innerMutex = g_subrateCallbackMutex;
    g_subrateCallbackMutex = NULL;
    MutexUnlock(g_subrateCallbackLifecycleMutex);
    if (GapLeSubrateCallbackWaitRefsDrain()) {
        MutexDelete(innerMutex);
    } else {
        LOG_ERROR("%{public}s: leaving callback mutex allocated due to unreleased references", __FUNCTION__);
    }
}

// Ensure the deinit drain event exists (called with the lifecycle lock held).
// Returns false when the event could not be created; the caller's rollback of the
// callback mutex is performed here, mirroring Deinit's teardown order.
static bool GapLeSubrateEnsureDeinitEvent(void)
{
    if (__atomic_load_n(&g_subrateCallbackDeinitEvent, __ATOMIC_ACQUIRE) != NULL) {
        return true;
    }
    Event *newEvent = EventCreate(true);
    if (newEvent == NULL) {
        LOG_ERROR("%{public}s: EventCreate failed", __FUNCTION__);
        GapLeSubrateRollbackCallbackMutex();
        return false;
    }
    Event *expectedEvent = NULL;
    if (!__atomic_compare_exchange_n(&g_subrateCallbackDeinitEvent, &expectedEvent, newEvent, false,
        __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
        EventDelete(newEvent);
    }
    return true;
}

int GapLeSubrateCallbackInit(void)
{
    if (!GapLeSubrateEnsureLifecycleMutex()) {
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(g_subrateCallbackLifecycleMutex);
    if (g_subrateCallbackDeinitInProgress) {
        MutexUnlock(g_subrateCallbackLifecycleMutex);
        LOG_ERROR("%{public}s: deinit in progress", __FUNCTION__);
        return GAP_ERR_OUT_OF_RES;
    }

    // Clear callback state under the lifecycle lock so that concurrent
    // registration/lookup paths cannot observe partially initialized memory.
    (void)memset_s(&g_subrateCallback, sizeof(g_subrateCallback), 0x00, sizeof(g_subrateCallback));

    if (g_subrateCallbackMutex == NULL) {
        g_subrateCallbackMutex = MutexCreate();
    }
    if (g_subrateCallbackMutex == NULL) {
        MutexUnlock(g_subrateCallbackLifecycleMutex);
        LOG_ERROR("%{public}s: MutexCreate failed", __FUNCTION__);
        return GAP_ERR_OUT_OF_RES;
    }
    // Never force the reference counter back to zero (the counter is already
    // zero under the serial Init/Deinit contract; see the invariant in
    // GapLeSubrateCallbackGet): a stale Release after a reset would drive the
    // counter negative and every later Deinit would misjudge its drain. If a
    // previous teardown timed out and leaked references, refuse to re-initialize
    // so the corruption cannot be silently covered up.
    int32_t staleRefs = __atomic_load_n(&g_subrateCallbackRef, __ATOMIC_SEQ_CST);
    if (staleRefs != 0) {
        MutexUnlock(g_subrateCallbackLifecycleMutex);
        LOG_ERROR("%{public}s: %d callback reference(s) outstanding from a previous teardown; "
            "refusing to re-initialize", __FUNCTION__, staleRefs);
        return GAP_ERR_OUT_OF_RES;
    }

    if (!GapLeSubrateEnsureDeinitEvent()) {
        return GAP_ERR_OUT_OF_RES;
    }

    MutexUnlock(g_subrateCallbackLifecycleMutex);
    return GAP_SUCCESS;
}

void GapLeSubrateCallbackDeinit(void)
{
    Mutex *lifecycleMutex = __atomic_load_n(&g_subrateCallbackLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        return;
    }

    MutexLock(lifecycleMutex);
    if (g_subrateCallbackDeinitInProgress) {
        MutexUnlock(g_subrateCallbackLifecycleMutex);
        return;
    }
    g_subrateCallbackDeinitInProgress = true;

    Mutex *mutex = g_subrateCallbackMutex;
    g_subrateCallbackMutex = NULL;
    MutexUnlock(g_subrateCallbackLifecycleMutex);

    if (mutex != NULL) {
        // No new reader can acquire the mutex now that the global pointer is
        // NULL. Wait until every in-flight callback that observed a non-NULL
        // pointer has released its reference, then clear the callback state
        // under the mutex so no dispatcher can observe stale pointers after
        // GAP deinitialization. Every holder of the captured pointer has
        // already counted itself (the ref is taken at capture time under the
        // lifecycle lock), so once the drain reaches zero no code path can
        // still lock this mutex and MutexDelete below is safe.
        bool refsDrained = GapLeSubrateCallbackWaitRefsDrain();
        MutexLock(mutex);
        (void)memset_s(&g_subrateCallback, sizeof(g_subrateCallback), 0x00, sizeof(g_subrateCallback));
        MutexUnlock(mutex);
        if (refsDrained) {
            MutexDelete(mutex);
        } else {
            LOG_ERROR("%{public}s: leaving subrate callback mutex allocated due to unreleased references",
                __FUNCTION__);
        }
    }

    MutexLock(g_subrateCallbackLifecycleMutex);
    g_subrateCallbackDeinitInProgress = false;
    MutexUnlock(g_subrateCallbackLifecycleMutex);
    // Intentionally keep g_subrateCallbackLifecycleMutex alive: other
    // threads may still be racing through register/deregister/get paths and
    // need the lifecycle lock to observe that the inner mutex is gone.
}

// ---- GAPIF bridge: dispatch the GAP_* entry points on the Stack thread ----

typedef struct {
    int result;
    GapLeSubrateDefaultParams params;
} GapLeSubrateSetDefaultContext;

static void GapLeSetDefaultSubrateTask(void *ctx)
{
    GapLeSubrateSetDefaultContext *info = ctx;
    info->result = GAP_LeSetDefaultSubrate(&info->params);
}

int GAPIF_LeSetDefaultSubrate(const GapLeSubrateDefaultParams *params)
{
    if (params == NULL) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: subrateMin: 0x%{public}04x, subrateMax: 0x%{public}04x, maxLatency: "
        "0x%{public}04x, continuationNumber: 0x%{public}04x, supervisionTimeout: 0x%{public}04x", __FUNCTION__,
        params->defaultSubrateMin, params->defaultSubrateMax, params->defaultMaxLatency,
        params->defaultContinuationNumber, params->defaultSupervisionTimeout);
    GapLeSubrateSetDefaultContext *ctx = MEM_MALLOC.alloc(sizeof(GapLeSubrateSetDefaultContext));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(GapLeSubrateSetDefaultContext), 0x00, sizeof(GapLeSubrateSetDefaultContext));

    ctx->params = *params;

    int ret = GapRunTaskBlockProcess(GapLeSetDefaultSubrateTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

typedef struct {
    int result;
    GapLeSubrateRequestParams params;
} GapLeSubrateRequestContext;

static void GapLeSubrateRequestTask(void *ctx)
{
    GapLeSubrateRequestContext *info = ctx;
    info->result = GAP_LeSubrateRequest(&info->params);
}

int GAPIF_LeSubrateRequest(const GapLeSubrateRequestParams *params)
{
    if (params == NULL) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: handle: 0x%{public}04x, subrateMin: 0x%{public}04x, subrateMax: 0x%{public}04x, "
        "maxLatency: 0x%{public}04x, continuationNumber: 0x%{public}04x, supervisionTimeout: 0x%{public}04x",
        __FUNCTION__, params->connectionHandle, params->subrateMin, params->subrateMax, params->maxLatency,
        params->continuationNumber, params->supervisionTimeout);
    GapLeSubrateRequestContext *ctx = MEM_MALLOC.alloc(sizeof(GapLeSubrateRequestContext));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(GapLeSubrateRequestContext), 0x00, sizeof(GapLeSubrateRequestContext));

    ctx->params = *params;

    int ret = GapRunTaskBlockProcess(GapLeSubrateRequestTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

typedef struct {
    int result;
    const GapLeSubrateCallback *callback;
    void *context;
} GapLeSubrateRegisterCallbackInfo;

static void GapRegisterLeSubrateCallbackTask(void *ctx)
{
    GapLeSubrateRegisterCallbackInfo *info = ctx;
    info->result = GAP_RegisterLeSubrateCallback(info->callback, info->context);
}

int GAPIF_RegisterLeSubrateCallback(const GapLeSubrateCallback *callback, void *context)
{
    LOG_INFO("%{public}s: %s", __FUNCTION__, callback != NULL ? "register" : "clear");
    GapLeSubrateRegisterCallbackInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeSubrateRegisterCallbackInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(GapLeSubrateRegisterCallbackInfo), 0x00, sizeof(GapLeSubrateRegisterCallbackInfo));

    ctx->callback = callback;
    ctx->context = context;

    int ret = GapRunTaskBlockProcess(GapRegisterLeSubrateCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

typedef struct {
    int result;
} GapLeSubrateDeregisterCallbackInfo;

static void GapDeregisterLeSubrateCallbackTask(void *ctx)
{
    GapLeSubrateDeregisterCallbackInfo *info = ctx;
    info->result = GAP_DeregisterLeSubrateCallback();
}

int GAPIF_LeDeregisterSubrateCallback(void)
{
    LOG_INFO("%{public}s: deregister", __FUNCTION__);
    GapLeSubrateDeregisterCallbackInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLeSubrateDeregisterCallbackInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(GapLeSubrateDeregisterCallbackInfo), 0x00, sizeof(GapLeSubrateDeregisterCallbackInfo));

    int ret = GapRunTaskBlockProcess(GapDeregisterLeSubrateCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}
