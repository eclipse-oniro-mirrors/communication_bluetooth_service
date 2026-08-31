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

#include "gap_def.h"
#include "gap_internal.h"
#include "gap_le.h"
#include "gap_task_internal.h"

#include <sched.h>
#include <securec.h>

#include "allocator.h"
#include "log.h"
#include "platform/include/event.h"
#include "platform/include/mutex.h"

#include "btm/btm_thread.h"

typedef struct {
    GapLePowerControlCallback callback;
    void *context;
} GapLePowerControlCallbackBlock;

static GapLePowerControlCallbackBlock g_powerControlCallback;
static Mutex *g_powerControlCallbackMutex = NULL;
// Forward declaration: GAP_DeregisterLePowerControlCallback waits for in-flight
// references to drain, and the drain helper is defined below it.
static bool GapLePowerControlCallbackWaitRefsDrain(void);
// Number of in-flight power-control callbacks. Incremented in the getter while
// the lifecycle mutex is still held (at inner-mutex pointer capture), decremented
// by the caller after the callback returns. Deinit waits for this to reach zero
// before clearing the callback state and deleting the mutex, so upper-layer
// context pointers stay valid.
static int32_t g_powerControlCallbackRef = 0;
// Set while a power-control dispatch is invoking the upper-layer callback (the
// window between GapLePowerControlCallbackGet and GapLePowerControlCallbackRelease).
// Thread-local: only the dispatching thread observes it. Used by Deregister to
// detect a re-entrant call made from inside a power-control callback, where the
// only outstanding reference is the dispatcher's own and draining it would spin
// for 60 s on the single Stack thread.
static __thread bool g_powerControlCallbackDispatching = false;

// Protects creation/destruction of the callback mutex and concurrent reads of
// its global pointer. Created once in GapLePowerControlCallbackInit and
// intentionally never destroyed to avoid use-after-free during teardown races.
// Contract: GapLePowerControlCallbackInit and GapLePowerControlCallbackDeinit
// must be called serially by a single thread/task. After Deinit, registration
// and lookup APIs return GAP_ERR_OUT_OF_RES until Init is called again.
static Mutex *g_powerControlCallbackLifecycleMutex = NULL;
// Set while GapLePowerControlCallbackDeinit is tearing the callback mutex down.
// Registration and lookup APIs must fail instead of re-creating a mutex that is
// about to be deleted.
static bool g_powerControlCallbackDeinitInProgress = false;
// Signaled when the reference count reaches zero so Deinit can avoid spinning.
static Event *g_powerControlCallbackDeinitEvent = NULL;

// Number of drain wait iterations: 12 * 5000 ms = 60 s.
#define GAP_LE_POWER_CALLBACK_DRAIN_WAIT_RETRIES (12)
#define GAP_LE_POWER_CALLBACK_DRAIN_WAIT_MS (5000)
// GAP_LE_CONNECTION_HANDLE_MAX (0x0EFF, ACL 12-bit handle range) comes from gap_internal.h.

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
// time while the lifecycle lock is still held (see GapLePowerControlCallbackGet),
// so Deinit's drain covers every path that observed the inner mutex pointer,
// not only dispatches that already acquired the inner mutex; after Deinit
// NULLs the pointer under the lifecycle lock, no new reference can be taken
// and the counter only decreases.
static bool GapLePowerControlCallbackGet(GapLePowerControlCallback *callback, void **context)
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

    Mutex *lifecycleMutex = __atomic_load_n(&g_powerControlCallbackLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        *context = NULL;
        return false;
    }

    MutexLock(lifecycleMutex);
    if (g_powerControlCallbackDeinitInProgress) {
        MutexUnlock(g_powerControlCallbackLifecycleMutex);
        (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        *context = NULL;
        return false;
    }

    Mutex *mutex = g_powerControlCallbackMutex;
    if (mutex == NULL) {
        MutexUnlock(g_powerControlCallbackLifecycleMutex);
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
    (void)__atomic_fetch_add(&g_powerControlCallbackRef, 1, __ATOMIC_SEQ_CST);

    MutexLock(mutex);
    *callback = g_powerControlCallback.callback;
    *context = g_powerControlCallback.context;
    MutexUnlock(mutex);
    MutexUnlock(g_powerControlCallbackLifecycleMutex);
    return true;
}

static void GapLePowerControlCallbackRelease(void)
{
    if (__atomic_fetch_sub(&g_powerControlCallbackRef, 1, __ATOMIC_SEQ_CST) == 1) {
        Event *event = __atomic_load_n(&g_powerControlCallbackDeinitEvent, __ATOMIC_ACQUIRE);
        if (event != NULL) {
            EventSet(event);
        }
    }
}

// Marks the start of the upper-layer callback invocation inside a dispatch
// (see g_powerControlCallbackDispatching). Called right after the getter
// succeeds, before the callback is invoked.
static void GapLePowerControlDispatchBegin(void)
{
    g_powerControlCallbackDispatching = true;
}

// Clears the dispatch window and releases the reference held by the getter.
// Called right after the callback returns; the flag must be cleared before the
// release so that a Deregister invoked from inside the callback (which runs
// before this point on the same thread) observes the flag while it is still set.
static void GapLePowerControlDispatchEnd(void)
{
    g_powerControlCallbackDispatching = false;
    GapLePowerControlCallbackRelease();
}

int GAP_LeEnhancedReadTransmitPowerLevel(uint16_t connectionHandle, uint8_t phy)
{
    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    if (phy != GAP_LE_PHY_1M && phy != GAP_LE_PHY_2M && phy != GAP_LE_PHY_CODED && phy != GAP_LE_TEST_PHY_CODED_S2) {
        return BT_BAD_PARAM;
    }

    HciLeEnhancedReadTransmitPowerLevelParam hciCmdParam = {
        .connectionHandle = connectionHandle,
        .phy = phy,
    };

    return HCI_LeEnhancedReadTransmitPowerLevel(&hciCmdParam);
}

int GAP_LeReadRemoteTransmitPowerLevel(uint16_t connectionHandle, uint8_t phy)
{
    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    if (phy != GAP_LE_PHY_1M && phy != GAP_LE_PHY_2M && phy != GAP_LE_PHY_CODED && phy != GAP_LE_TEST_PHY_CODED_S2) {
        return BT_BAD_PARAM;
    }

    HciLeReadRemoteTransmitPowerLevelParam hciCmdParam = {
        .connectionHandle = connectionHandle,
        .phy = phy,
    };

    return HCI_LeReadRemoteTransmitPowerLevel(&hciCmdParam);
}

int GAP_LeSetPathLossReportingParameters(const GapLePathLossReportingParams *params)
{
    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (params == NULL) {
        return BT_BAD_PARAM;
    }

    if ((params->highThreshold + params->highHysteresis) > 0xFF ||
        params->lowThreshold < params->lowHysteresis || params->lowThreshold > params->highThreshold ||
        (params->lowThreshold + params->lowHysteresis) > (params->highThreshold - params->highHysteresis)) {
        return BT_BAD_PARAM;
    }

    HciLeSetPathLossReportingParametersParam hciCmdParam = {
        .connectionHandle = params->connectionHandle,
        .highThreshold = params->highThreshold,
        .highHysteresis = params->highHysteresis,
        .lowThreshold = params->lowThreshold,
        .lowHysteresis = params->lowHysteresis,
        .minTimeSpent = params->minTimeSpent,
    };

    return HCI_LeSetPathLossReportingParameters(&hciCmdParam);
}

int GAP_LeSetPathLossReportingEnable(uint16_t connectionHandle, uint8_t enable)
{
    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (enable != 0x00 && enable != 0x01) {
        return BT_BAD_PARAM;
    }

    HciLeSetPathLossReportingEnableParam hciCmdParam = {
        .connectionHandle = connectionHandle,
        .enable = enable,
    };

    return HCI_LeSetPathLossReportingEnable(&hciCmdParam);
}

int GAP_LeSetTransmitPowerReportingEnable(uint16_t connectionHandle, uint8_t localEnable, uint8_t remoteEnable)
{
    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if ((localEnable != 0x00 && localEnable != 0x01) || (remoteEnable != 0x00 && remoteEnable != 0x01)) {
        return BT_BAD_PARAM;
    }

    HciLeSetTransmitPowerReportingEnableParam hciCmdParam = {
        .connectionHandle = connectionHandle,
        .localEnable = localEnable,
        .remoteEnable = remoteEnable,
    };

    return HCI_LeSetTransmitPowerReportingEnable(&hciCmdParam);
}

void GapLeEnhancedReadTransmitPowerLevelComplete(const HciLeEnhancedReadTransmitPowerLevelReturnParam *param)
{
    GapLeEnhancedReadTxPowerInfo info = {
        .connectionHandle = param->connectionHandle,
        .phy = param->phy,
        .currentTransmitPowerLevel = param->currentTransmitPowerLevel,
        .maxTransmitPowerLevel = param->maxTransmitPowerLevel,
    };

    GapLePowerControlCallback callback;
    void *context = NULL;
    if (!GapLePowerControlCallbackGet(&callback, &context)) {
        return;
    }
    GapLePowerControlDispatchBegin();
    if (callback.enhancedReadTransmitPowerResult != NULL) {
        callback.enhancedReadTransmitPowerResult(param->status, &info, context);
    }
    GapLePowerControlDispatchEnd();
}

void GapLeTransmitPowerReportingEvent(const HciLeTransmitPowerReportingEventParam *param)
{
    // Delivered on the Stack thread, like every GAP HCI event. Events already queued when
    // reporting is disabled (GAP_LeSetTransmitPowerReportingEnable(enable=0)) are NOT revoked:
    // every event enqueued before the disable is delivered after the disable completes, so the
    // number of late deliveries is not bounded to one. This is safe (no state is kept in this
    // module; the upper layer keys on connectionHandle). Deliberate stateless trade-off, no
    // filtering is performed here.
    GapLeTransmitPowerReportingInfo info = {
        .status = param->status,
        .connectionHandle = param->connectionHandle,
        .reason = param->reason,
        .phy = param->phy,
        .transmitPowerLevel = param->transmitPowerLevel,
        .transmitPowerLevelFlag = param->transmitPowerLevelFlag,
        .delta = param->delta,
    };

    GapLePowerControlCallback callback;
    void *context = NULL;
    if (!GapLePowerControlCallbackGet(&callback, &context)) {
        return;
    }
    GapLePowerControlDispatchBegin();
    if (callback.transmitPowerReporting != NULL) {
        callback.transmitPowerReporting(&info, context);
    }
    GapLePowerControlDispatchEnd();
}

void GapLeSetPathLossReportingParametersComplete(const HciLeSetPathLossReportingParametersReturnParam *param)
{
    GapLeStatusHandleInfo info = {
        .status = param->status,
        .connectionHandle = param->connectionHandle,
    };

    GapLePowerControlCallback callback;
    void *context = NULL;
    if (!GapLePowerControlCallbackGet(&callback, &context)) {
        return;
    }
    GapLePowerControlDispatchBegin();
    if (callback.setPathLossReportingParamsResult != NULL) {
        callback.setPathLossReportingParamsResult(param->status, &info, context);
    }
    GapLePowerControlDispatchEnd();
}

void GapLeSetPathLossReportingEnableComplete(const HciLeSetPathLossReportingEnableReturnParam *param)
{
    GapLeStatusHandleInfo info = {
        .status = param->status,
        .connectionHandle = param->connectionHandle,
    };

    GapLePowerControlCallback callback;
    void *context = NULL;
    if (!GapLePowerControlCallbackGet(&callback, &context)) {
        return;
    }
    GapLePowerControlDispatchBegin();
    if (callback.setPathLossReportingEnableResult != NULL) {
        callback.setPathLossReportingEnableResult(param->status, &info, context);
    }
    GapLePowerControlDispatchEnd();
}

void GapLeSetTransmitPowerReportingEnableComplete(const HciLeSetTransmitPowerReportingEnableReturnParam *param)
{
    GapLeStatusHandleInfo info = {
        .status = param->status,
        .connectionHandle = param->connectionHandle,
    };

    GapLePowerControlCallback callback;
    void *context = NULL;
    if (!GapLePowerControlCallbackGet(&callback, &context)) {
        return;
    }
    GapLePowerControlDispatchBegin();
    if (callback.setTransmitPowerReportingEnableResult != NULL) {
        callback.setTransmitPowerReportingEnableResult(param->status, &info, context);
    }
    GapLePowerControlDispatchEnd();
}

void GapLePathLossThresholdEvent(const HciLePathLossThresholdEventParam *param)
{
    // Delivered on the Stack thread, like every GAP HCI event. Events already queued when
    // reporting is disabled (GAP_LeSetPathLossReportingEnable(enable=0)) are NOT revoked:
    // every event enqueued before the disable is delivered after the disable completes, so
    // the number of late deliveries is not bounded to one. This is safe (no state is kept in
    // this module; the upper layer keys on connectionHandle). This is the deliberate
    // stateless trade-off, no filtering is performed here.
    GapLePathLossThresholdInfo info = {
        .connectionHandle = param->connectionHandle,
        .currentPathLoss = param->currentPathLoss,
        .zoneEntered = param->zoneEntered,
    };

    GapLePowerControlCallback callback;
    void *context = NULL;
    if (!GapLePowerControlCallbackGet(&callback, &context)) {
        return;
    }
    GapLePowerControlDispatchBegin();
    if (callback.pathLossThreshold != NULL) {
        callback.pathLossThreshold(&info, context);
    }
    GapLePowerControlDispatchEnd();
}

int GAP_RegisterLePowerControlCallback(const GapLePowerControlCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "NULL");
    // Read the lifecycle mutex atomically: GapLePowerControlCallbackInit
    // publishes it with __atomic_compare_exchange_n, so a plain load here is a
    // data race (benign in practice since the mutex is never destroyed, but
    // flagged by static analyzers). Once obtained, the pointer stays valid for
    // the function's lifetime; the subsequent Unlock sites still reference the
    // global for consistency with the rest of the file.
    Mutex *lifecycleMutex = __atomic_load_n(&g_powerControlCallbackLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(lifecycleMutex);
    if (g_powerControlCallbackDeinitInProgress) {
        MutexUnlock(g_powerControlCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }
    Mutex *mutex = g_powerControlCallbackMutex;
    if (mutex == NULL) {
        MutexUnlock(g_powerControlCallbackLifecycleMutex);
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
    // GAPIF_LeRegisterPowerControlCallback entry point dispatches this call
    // synchronously via GapRunTaskBlockProcess, so the copy is complete before
    // the entry point returns and a caller-owned structure stays valid for the
    // whole call; nothing is retained past the copy. A NULL callback clears the
    // registered structure, matching GAP_RegisterLeConnCallback.
    MutexLock(mutex);
    if (callback == NULL) {
        (void)memset_s(&g_powerControlCallback.callback,
            sizeof(g_powerControlCallback.callback),
            0x00,
            sizeof(g_powerControlCallback.callback));
        // a NULL callback clears the context as well, otherwise register(NULL, staleCtx)
        // would keep delivering a stale context that no callback can consume
        g_powerControlCallback.context = NULL;
    } else {
        g_powerControlCallback.callback = *callback;
        g_powerControlCallback.context = context;
    }
    MutexUnlock(mutex);
    MutexUnlock(g_powerControlCallbackLifecycleMutex);
    return GAP_SUCCESS;
}

int GAP_DeregisterLePowerControlCallback(void)
{
    Mutex *lifecycleMutex = __atomic_load_n(&g_powerControlCallbackLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(lifecycleMutex);
    if (g_powerControlCallbackDeinitInProgress) {
        MutexUnlock(g_powerControlCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }
    Mutex *mutex = g_powerControlCallbackMutex;
    if (mutex == NULL) {
        MutexUnlock(g_powerControlCallbackLifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(mutex);
    (void)memset_s(&g_powerControlCallback.callback,
        sizeof(g_powerControlCallback.callback),
        0x00,
        sizeof(g_powerControlCallback.callback));
    g_powerControlCallback.context = NULL;
    MutexUnlock(mutex);
    // An in-flight dispatch has already copied the callback by value under the same mutex, so
    // clearing it is safe; waiting for the references to drain guarantees no dispatch of the
    // cleared callback can still be in flight when this returns. Under the current
    // single-Stack-thread model the reference count is already zero here and the wait returns
    // immediately (see GapLePowerControlCallbackWaitRefsDrain).
    bool inCallbackDispatch = g_powerControlCallbackDispatching;
    bool refsDrained = inCallbackDispatch || GapLePowerControlCallbackWaitRefsDrain();
    if (inCallbackDispatch) {
        // Re-entrant call made from inside a power-control callback (the GAPIF entry point is
        // dispatched inline on the Stack thread via the GapRunTaskBlockProcess fast path): the
        // only outstanding reference is the one held by this very dispatch, which can only be
        // released after this call returns - draining would spin for 60 s on the Stack thread.
        // The dispatch has already copied the callback by value before invoking it, so the
        // cleared registration can no longer affect this dispatch.
        LOG_INFO("%{public}s: called from inside a power-control callback dispatch; ref drain skipped "
            "(the in-flight dispatch owns the only reference)", __FUNCTION__);
    }
    MutexUnlock(g_powerControlCallbackLifecycleMutex);
    if (!refsDrained) {
        // The wait window elapsed while dispatches were still in flight. Deregistration is not
        // clean yet: do not report success so the upper layer does not assume its context
        // pointer can be freed.
        LOG_ERROR("%{public}s: timeout waiting for in-flight power-control callbacks to release; "
            "deregistration incomplete", __FUNCTION__);
        return GAP_ERR_REMOTE_ACTION;
    }
    return GAP_SUCCESS;
}

// Waits until all in-flight callbacks have released their references, or the
// wait window elapses (event-based wait, falling back to a bounded spin when
// the deinit event is not yet published). Returns true when the reference count
// reached zero, i.e. the callback mutex can be safely deleted.
// Under the current single-Stack-thread model (see GapLePowerControlCallbackGet)
// the reference is always zero here, so the wait never blocks; the timeout path
// only exists for a future cross-thread dispatcher and deliberately leaks the
// mutex instead of deleting it while references may still be outstanding.
static bool GapLePowerControlCallbackWaitRefsDrain(void)
{
    Event *event = __atomic_load_n(&g_powerControlCallbackDeinitEvent, __ATOMIC_ACQUIRE);
    if (event != NULL) {
        int32_t waitRetries = GAP_LE_POWER_CALLBACK_DRAIN_WAIT_RETRIES;
        while (__atomic_load_n(&g_powerControlCallbackRef, __ATOMIC_SEQ_CST) > 0 && waitRetries > 0) {
            (void)EventWait(event, GAP_LE_POWER_CALLBACK_DRAIN_WAIT_MS);
            waitRetries--;
        }
        if (waitRetries == 0) {
            LOG_ERROR("%{public}s: timeout waiting for power-control callbacks to release", __FUNCTION__);
        }
    } else {
        int32_t spinRetries = 1000000;
        while (__atomic_load_n(&g_powerControlCallbackRef, __ATOMIC_SEQ_CST) > 0 && spinRetries > 0) {
            sched_yield();
            spinRetries--;
        }
        if (spinRetries == 0) {
            LOG_ERROR("%{public}s: spin timeout waiting for power-control callbacks to release", __FUNCTION__);
        }
    }
    return __atomic_load_n(&g_powerControlCallbackRef, __ATOMIC_SEQ_CST) == 0;
}

// Ensure the lifecycle mutex exists before locking. Once created, it is never
// destroyed so that racing registration/lookup paths can always observe the
// initialized/deinitialized state safely. The pointer is read atomically (same as
// GapLePowerControlCallbackGet) so the check is defined even if another thread
// creates the mutex concurrently. Returns false when the mutex could not be created.
static bool GapLePowerControlEnsureLifecycleMutex(void)
{
    if (__atomic_load_n(&g_powerControlCallbackLifecycleMutex, __ATOMIC_ACQUIRE) != NULL) {
        return true;
    }
    Mutex *newMutex = MutexCreate();
    if (newMutex == NULL) {
        LOG_ERROR("%{public}s: Lifecycle MutexCreate failed", __FUNCTION__);
        return false;
    }
    Mutex *expected = NULL;
    if (!__atomic_compare_exchange_n(&g_powerControlCallbackLifecycleMutex, &expected, newMutex, false,
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
static void GapLePowerControlRollbackCallbackMutex(void)
{
    Mutex *innerMutex = g_powerControlCallbackMutex;
    g_powerControlCallbackMutex = NULL;
    MutexUnlock(g_powerControlCallbackLifecycleMutex);
    if (GapLePowerControlCallbackWaitRefsDrain()) {
        MutexDelete(innerMutex);
    } else {
        LOG_ERROR("%{public}s: leaving callback mutex allocated due to unreleased references", __FUNCTION__);
    }
}

// Ensure the deinit drain event exists (called with the lifecycle lock held).
// Returns false when the event could not be created; the caller's rollback of the
// callback mutex is performed here, mirroring Deinit's teardown order.
static bool GapLePowerControlEnsureDeinitEvent(void)
{
    if (__atomic_load_n(&g_powerControlCallbackDeinitEvent, __ATOMIC_ACQUIRE) != NULL) {
        return true;
    }
    Event *newEvent = EventCreate(true);
    if (newEvent == NULL) {
        LOG_ERROR("%{public}s: EventCreate failed", __FUNCTION__);
        GapLePowerControlRollbackCallbackMutex();
        return false;
    }
    Event *expectedEvent = NULL;
    if (!__atomic_compare_exchange_n(&g_powerControlCallbackDeinitEvent, &expectedEvent, newEvent, false,
        __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
        EventDelete(newEvent);
    }
    return true;
}

int GapLePowerControlCallbackInit(void)
{
    if (!GapLePowerControlEnsureLifecycleMutex()) {
        return GAP_ERR_OUT_OF_RES;
    }

    MutexLock(g_powerControlCallbackLifecycleMutex);
    if (g_powerControlCallbackDeinitInProgress) {
        MutexUnlock(g_powerControlCallbackLifecycleMutex);
        LOG_ERROR("%{public}s: deinit in progress", __FUNCTION__);
        return GAP_ERR_OUT_OF_RES;
    }

    // Clear callback state under the lifecycle lock so that concurrent
    // registration/lookup paths cannot observe partially initialized memory.
    (void)memset_s(&g_powerControlCallback, sizeof(g_powerControlCallback), 0x00, sizeof(g_powerControlCallback));

    if (g_powerControlCallbackMutex == NULL) {
        g_powerControlCallbackMutex = MutexCreate();
    }
    if (g_powerControlCallbackMutex == NULL) {
        MutexUnlock(g_powerControlCallbackLifecycleMutex);
        LOG_ERROR("%{public}s: MutexCreate failed", __FUNCTION__);
        return GAP_ERR_OUT_OF_RES;
    }
    __atomic_store_n(&g_powerControlCallbackRef, 0, __ATOMIC_SEQ_CST);

    if (!GapLePowerControlEnsureDeinitEvent()) {
        return GAP_ERR_OUT_OF_RES;
    }

    MutexUnlock(g_powerControlCallbackLifecycleMutex);
    return GAP_SUCCESS;
}

void GapLePowerControlCallbackDeinit(void)
{
    Mutex *lifecycleMutex = __atomic_load_n(&g_powerControlCallbackLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        return;
    }

    MutexLock(lifecycleMutex);
    if (g_powerControlCallbackDeinitInProgress) {
        MutexUnlock(g_powerControlCallbackLifecycleMutex);
        return;
    }
    g_powerControlCallbackDeinitInProgress = true;

    Mutex *mutex = g_powerControlCallbackMutex;
    g_powerControlCallbackMutex = NULL;
    MutexUnlock(g_powerControlCallbackLifecycleMutex);

    if (mutex != NULL) {
        // No new reader can acquire the mutex now that the global pointer is
        // NULL. Wait until every in-flight callback that observed a non-NULL
        // pointer has released its reference, then clear the callback state
        // under the mutex so no dispatcher can observe stale pointers after
        // GAP deinitialization. Every holder of the captured pointer has
        // already counted itself (the ref is taken at capture time under the
        // lifecycle lock), so once the drain reaches zero no code path can
        // still lock this mutex and MutexDelete below is safe.
        bool refsDrained = GapLePowerControlCallbackWaitRefsDrain();
        MutexLock(mutex);
        (void)memset_s(&g_powerControlCallback, sizeof(g_powerControlCallback), 0x00, sizeof(g_powerControlCallback));
        MutexUnlock(mutex);
        if (refsDrained) {
            MutexDelete(mutex);
        } else {
            LOG_ERROR("%{public}s: leaving power-control callback mutex allocated due to unreleased references",
                __FUNCTION__);
        }
    }

    MutexLock(g_powerControlCallbackLifecycleMutex);
    g_powerControlCallbackDeinitInProgress = false;
    MutexUnlock(g_powerControlCallbackLifecycleMutex);
    // Intentionally keep g_powerControlCallbackLifecycleMutex alive: other
    // threads may still be racing through register/deregister/get paths and
    // need the lifecycle lock to observe that the inner mutex is gone.
}

typedef struct {
    int result;
    uint16_t connectionHandle;
    uint8_t phy;
} GapLePowerReadContext;

static void GapLeEnhancedReadTransmitPowerLevelTask(void *ctx)
{
    GapLePowerReadContext *info = ctx;
    info->result = GAP_LeEnhancedReadTransmitPowerLevel(info->connectionHandle, info->phy);
}

int GAPIF_LeEnhancedReadTransmitPowerLevel(uint16_t connectionHandle, uint8_t phy)
{
    LOG_INFO("%{public}s: handle: 0x%{public}04x, phy: 0x%{public}02x", __FUNCTION__, connectionHandle, phy);
    GapLePowerReadContext *ctx = MEM_MALLOC.alloc(sizeof(GapLePowerReadContext));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(GapLePowerReadContext), 0x00, sizeof(GapLePowerReadContext));

    ctx->connectionHandle = connectionHandle;
    ctx->phy = phy;

    int ret = GapRunTaskBlockProcess(GapLeEnhancedReadTransmitPowerLevelTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

static void GapLeReadRemoteTransmitPowerLevelTask(void *ctx)
{
    GapLePowerReadContext *info = ctx;
    info->result = GAP_LeReadRemoteTransmitPowerLevel(info->connectionHandle, info->phy);
}

int GAPIF_LeReadRemoteTransmitPowerLevel(uint16_t connectionHandle, uint8_t phy)
{
    LOG_INFO("%{public}s: handle: 0x%{public}04x, phy: 0x%{public}02x", __FUNCTION__, connectionHandle, phy);
    GapLePowerReadContext *ctx = MEM_MALLOC.alloc(sizeof(GapLePowerReadContext));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(GapLePowerReadContext), 0x00, sizeof(GapLePowerReadContext));

    ctx->connectionHandle = connectionHandle;
    ctx->phy = phy;

    int ret = GapRunTaskBlockProcess(GapLeReadRemoteTransmitPowerLevelTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

typedef struct {
    int result;
    const GapLePowerControlCallback *callback;
    void *context;
} GapLePowerRegisterCallbackInfo;

static void GapRegisterLePowerControlCallbackTask(void *ctx)
{
    GapLePowerRegisterCallbackInfo *info = ctx;
    info->result = GAP_RegisterLePowerControlCallback(info->callback, info->context);
}

int GAPIF_LeRegisterPowerControlCallback(const GapLePowerControlCallback *callback, void *context)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    GapLePowerRegisterCallbackInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLePowerRegisterCallbackInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(GapLePowerRegisterCallbackInfo), 0x00, sizeof(GapLePowerRegisterCallbackInfo));

    ctx->callback = callback;
    ctx->context = context;

    int ret = GapRunTaskBlockProcess(GapRegisterLePowerControlCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

typedef struct {
    int result;
} GapLePowerDeregisterCallbackInfo;

static void GapDeregisterLePowerControlCallbackTask(void *ctx)
{
    GapLePowerDeregisterCallbackInfo *info = ctx;
    info->result = GAP_DeregisterLePowerControlCallback();
}

int GAPIF_LeDeregisterPowerControlCallback(void)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    GapLePowerDeregisterCallbackInfo *ctx = MEM_MALLOC.alloc(sizeof(GapLePowerDeregisterCallbackInfo));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(GapLePowerDeregisterCallbackInfo), 0x00, sizeof(GapLePowerDeregisterCallbackInfo));

    int ret = GapRunTaskBlockProcess(GapDeregisterLePowerControlCallbackTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

typedef struct {
    int result;
    GapLePathLossReportingParams params;
} GapLePowerSetParamsContext;

static void GapLeSetPathLossReportingParametersTask(void *ctx)
{
    GapLePowerSetParamsContext *info = ctx;
    info->result = GAP_LeSetPathLossReportingParameters(&info->params);
}

int GAPIF_LeSetPathLossReportingParameters(const GapLePathLossReportingParams *params)
{
    if (params == NULL) {
        return BT_BAD_PARAM;
    }

    LOG_INFO("%{public}s: handle: 0x%{public}04x", __FUNCTION__, params->connectionHandle);
    GapLePowerSetParamsContext *ctx = MEM_MALLOC.alloc(sizeof(GapLePowerSetParamsContext));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(GapLePowerSetParamsContext), 0x00, sizeof(GapLePowerSetParamsContext));

    ctx->params = *params;

    int ret = GapRunTaskBlockProcess(GapLeSetPathLossReportingParametersTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

typedef struct {
    int result;
    uint16_t connectionHandle;
    uint8_t enable;
} GapLePowerSetEnableContext;

static void GapLeSetPathLossReportingEnableTask(void *ctx)
{
    GapLePowerSetEnableContext *info = ctx;
    info->result = GAP_LeSetPathLossReportingEnable(info->connectionHandle, info->enable);
}

int GAPIF_LeSetPathLossReportingEnable(uint16_t connectionHandle, uint8_t enable)
{
    LOG_INFO("%{public}s: handle: 0x%{public}04x, enable: 0x%{public}02x", __FUNCTION__, connectionHandle, enable);
    GapLePowerSetEnableContext *ctx = MEM_MALLOC.alloc(sizeof(GapLePowerSetEnableContext));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(ctx, sizeof(GapLePowerSetEnableContext), 0x00, sizeof(GapLePowerSetEnableContext));

    ctx->connectionHandle = connectionHandle;
    ctx->enable = enable;

    int ret = GapRunTaskBlockProcess(GapLeSetPathLossReportingEnableTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}

typedef struct {
    int result;
    uint16_t connectionHandle;
    uint8_t localEnable;
    uint8_t remoteEnable;
} GapLePowerSetTransmitReportingContext;

static void GapLeSetTransmitPowerReportingEnableTask(void *ctx)
{
    GapLePowerSetTransmitReportingContext *info = ctx;
    info->result = GAP_LeSetTransmitPowerReportingEnable(info->connectionHandle, info->localEnable, info->remoteEnable);
}

int GAPIF_LeSetTransmitPowerReportingEnable(uint16_t connectionHandle, uint8_t localEnable, uint8_t remoteEnable)
{
    LOG_INFO("%{public}s: handle: 0x%{public}04x, localEnable: 0x%{public}02x, remoteEnable: 0x%{public}02x",
        __FUNCTION__, connectionHandle, localEnable, remoteEnable);
    GapLePowerSetTransmitReportingContext *ctx = MEM_MALLOC.alloc(sizeof(GapLePowerSetTransmitReportingContext));
    if (ctx == NULL) {
        return BT_NO_MEMORY;
    }

    (void)memset_s(
        ctx, sizeof(GapLePowerSetTransmitReportingContext), 0x00, sizeof(GapLePowerSetTransmitReportingContext));

    ctx->connectionHandle = connectionHandle;
    ctx->localEnable = localEnable;
    ctx->remoteEnable = remoteEnable;

    int ret = GapRunTaskBlockProcess(GapLeSetTransmitPowerReportingEnableTask, ctx);
    if (ret == BT_SUCCESS) {
        ret = ctx->result;
    }

    MEM_MALLOC.free(ctx);
    return ret;
}
