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

// ---------------------------------------------------------------------------
// Bluetooth 5.1: CTE (Constant Tone Extension) direction finding and PAST.
// Command completion handlers in this file dispatch through the CTE callback
// group below; gap_le_adv.c and gap_le_scan.c fetch the same group through
// GapLeCteCallbackGet/Release.
// ---------------------------------------------------------------------------

#define GAP_LE_CTE_CALLBACK_DRAIN_WAIT_RETRIES (12)
#define GAP_LE_CTE_CALLBACK_DRAIN_WAIT_MS (5000)

typedef struct {
    GapLeCteCallback callback;
    void *context;
} LeCteCallbackBlock;

static LeCteCallbackBlock g_leCteCallback;
static Mutex *g_leCteCallbackMutex = NULL;
// Number of threads currently holding the CTE callback mutex.
static int32_t g_leCteCallbackRef = 0;
// Number of in-flight CTE callbacks. Deinit waits for this to reach zero before
// clearing callback state so that upper-layer context pointers remain valid.
static int32_t g_leCteCallbackInFlight = 0;
// Signaled when the reference count reaches zero so Deinit can wait without spinning.
static Event *g_leCteCallbackRefEvent = NULL;
// Serializes Init/Deinit and prevents a new Init from racing with an in-progress Deinit.
static Mutex *g_leCteLifecycleMutex = NULL;
// Serializes the ref-zero event pointer load/exchange so the signal path never
// blocks behind the lifecycle mutex that Deinit holds during its drain waits
// (blocking there would defeat the event wakeup and degrade Deinit to full
// 5s-timeout polling). Kept alive for the module lifetime like the lifecycle mutex.
static Mutex *g_leCteCallbackEventMutex = NULL;

static bool GapLeCteCallbackTryLock(Mutex **outMutex)
{
    if (outMutex == NULL) {
        return false;
    }

    Mutex *mtx = __atomic_load_n(&g_leCteCallbackMutex, __ATOMIC_ACQUIRE);
    if (mtx == NULL) {
        return false;
    }

    __atomic_fetch_add(&g_leCteCallbackRef, 1, __ATOMIC_SEQ_CST);
    mtx = __atomic_load_n(&g_leCteCallbackMutex, __ATOMIC_ACQUIRE);
    if (mtx == NULL) {
        __atomic_fetch_sub(&g_leCteCallbackRef, 1, __ATOMIC_SEQ_CST);
        return false;
    }

    MutexLock(mtx);
    *outMutex = mtx;
    return true;
}

// Signal Deinit that a reference or in-flight counter reached zero. The event
// mutex (not the lifecycle mutex) serializes the event pointer load against
// GapLeCteCallbackClearState's exchange-and-delete: Deinit holds the lifecycle
// mutex while waiting for the drains, so taking that lock here would block the
// signal until Deinit finishes and defeat the event wakeup, degrading Deinit
// to full 5s-timeout polling on every race.
static void GapLeCteCallbackSignalRefZero(void)
{
    Mutex *eventMutex = __atomic_load_n(&g_leCteCallbackEventMutex, __ATOMIC_ACQUIRE);
    if (eventMutex == NULL) {
        return;
    }

    MutexLock(eventMutex);
    Event *event = __atomic_load_n(&g_leCteCallbackRefEvent, __ATOMIC_ACQUIRE);
    if (event != NULL) {
        EventSet(event);
    }
    MutexUnlock(eventMutex);
}

static void GapLeCteCallbackUnlock(Mutex *mtx)
{
    MutexUnlock(mtx);
    if (__atomic_fetch_sub(&g_leCteCallbackRef, 1, __ATOMIC_SEQ_CST) == 1) {
        // Last reference released; wake any Deinit waiter.
        GapLeCteCallbackSignalRefZero();
    }
}

void GapLeCteCallbackRelease(void)
{
    if (__atomic_fetch_sub(&g_leCteCallbackInFlight, 1, __ATOMIC_SEQ_CST) == 1) {
        // Last in-flight callback finished; wake any Deinit waiter.
        GapLeCteCallbackSignalRefZero();
    }
}

// Returns the lifecycle mutex, creating it (and the ref-zero event mutex) on
// first use via CAS so exactly one thread publishes them. Returns NULL when a
// mutex cannot be created. The event mutex is created first so no thread can
// observe a lifecycle mutex without its companion.
static Mutex *GapLeCteCallbackEnsureLifecycleMutex(void)
{
    Mutex *lifecycleMutex = __atomic_load_n(&g_leCteLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex != NULL) {
        return lifecycleMutex;
    }

    Mutex *newEventMutex = MutexCreate();
    if (newEventMutex == NULL) {
        LOG_ERROR("%{public}s: Event MutexCreate failed", __FUNCTION__);
        return NULL;
    }
    Mutex *expectedEventMutex = NULL;
    if (!__atomic_compare_exchange_n(&g_leCteCallbackEventMutex, &expectedEventMutex, newEventMutex,
        false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
        MutexDelete(newEventMutex);
    }

    Mutex *newLifecycleMutex = MutexCreate();
    if (newLifecycleMutex == NULL) {
        LOG_ERROR("%{public}s: Lifecycle MutexCreate failed", __FUNCTION__);
        return NULL;
    }
    Mutex *expected = NULL;
    if (!__atomic_compare_exchange_n(&g_leCteLifecycleMutex, &expected, newLifecycleMutex,
        false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
        MutexDelete(newLifecycleMutex);
        lifecycleMutex = expected;
    } else {
        lifecycleMutex = newLifecycleMutex;
    }
    return lifecycleMutex;
}

// Create the reference-count release event exactly once, returning whether this
// call published a brand-new event (the caller rolls it back on later failure).
// The publish takes the event mutex so the pointer never races the load in
// GapLeCteCallbackSignalRefZero: a signal left over from a previous cycle could
// otherwise load the brand-new event and EventSet it after the rollback deleted it.
static bool GapLeCteCallbackEnsureRefEvent(void)
{
    if (__atomic_load_n(&g_leCteCallbackRefEvent, __ATOMIC_ACQUIRE) != NULL) {
        return false;
    }
    Event *newEvent = EventCreate(true);
    if (newEvent == NULL) {
        LOG_ERROR("%{public}s: EventCreate failed", __FUNCTION__);
        return false;
    }
    Mutex *eventMutex = __atomic_load_n(&g_leCteCallbackEventMutex, __ATOMIC_ACQUIRE);
    if (eventMutex != NULL) {
        MutexLock(eventMutex);
    }
    Event *expectedEvent = NULL;
    bool eventCreated = __atomic_compare_exchange_n(&g_leCteCallbackRefEvent, &expectedEvent, newEvent,
        false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    if (eventMutex != NULL) {
        MutexUnlock(eventMutex);
    }
    if (!eventCreated) {
        EventDelete(newEvent);
    }
    return eventCreated;
}

// Exchange the reference event to NULL and delete it, serialized against
// GapLeCteCallbackSignalRefZero by the event mutex. Shared by the Init rollback
// path (after a failed MutexCreate) and the teardown path (after full drain).
static void GapLeCteCallbackClearRefEvent(void)
{
    Mutex *eventMutex = __atomic_load_n(&g_leCteCallbackEventMutex, __ATOMIC_ACQUIRE);
    if (eventMutex != NULL) {
        MutexLock(eventMutex);
    }
    Event *deletedEvent = __atomic_exchange_n(&g_leCteCallbackRefEvent, NULL, __ATOMIC_ACQ_REL);
    if (eventMutex != NULL) {
        MutexUnlock(eventMutex);
    }
    if (deletedEvent != NULL) {
        EventDelete(deletedEvent);
    }
}

// Publish the callback mutex and its release event exactly once, under the
// lifecycle lock.
static int GapLeCteCallbackEnsureResources(Mutex *lifecycleMutex)
{
    MutexLock(lifecycleMutex);
    if (__atomic_load_n(&g_leCteCallbackMutex, __ATOMIC_ACQUIRE) != NULL) {
        MutexUnlock(lifecycleMutex);
        return GAP_SUCCESS;
    }

    // Create the reference-count event before publishing the mutex so that
    // no thread can observe a mutex without a working release signal.
    bool eventCreated = GapLeCteCallbackEnsureRefEvent();
    if (__atomic_load_n(&g_leCteCallbackRefEvent, __ATOMIC_ACQUIRE) == NULL) {
        MutexUnlock(lifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }

    Mutex *newMutex = MutexCreate();
    if (newMutex == NULL) {
        LOG_ERROR("%{public}s: MutexCreate failed", __FUNCTION__);
        // If we just published a brand-new event and no mutex is published yet,
        // roll back the event so a later Init retry can recreate both resources.
        if (eventCreated) {
            GapLeCteCallbackClearRefEvent();
        }
        MutexUnlock(lifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }

    Mutex *expected = NULL;
    bool mutexInstalled = __atomic_compare_exchange_n(&g_leCteCallbackMutex, &expected, newMutex,
        false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    if (!mutexInstalled) {
        MutexDelete(newMutex);
    }

    MutexUnlock(lifecycleMutex);
    return GAP_SUCCESS;
}

// Wait until the atomic counter reaches zero, using the release event when
// available and a bounded spin fallback otherwise. Returns true when drained,
// false on timeout so the caller can decide whether resources may be freed.
static bool GapLeCteCallbackWaitDrain(const int32_t *counter, Event *event, const char *counterName)
{
    if (event != NULL) {
        int32_t waitRetries = GAP_LE_CTE_CALLBACK_DRAIN_WAIT_RETRIES;
        while (__atomic_load_n(counter, __ATOMIC_ACQUIRE) != 0 && waitRetries > 0) {
            (void)EventWait(event, GAP_LE_CTE_CALLBACK_DRAIN_WAIT_MS);
            waitRetries--;
        }
        if (waitRetries == 0) {
            LOG_ERROR("%{public}s: timeout waiting for %{public}s to release", __FUNCTION__, counterName);
        }
    } else {
        int32_t spinRetries = 1000000;
        while (__atomic_load_n(counter, __ATOMIC_ACQUIRE) != 0 && spinRetries > 0) {
            sched_yield();
            spinRetries--;
        }
        if (spinRetries == 0) {
            LOG_ERROR("%{public}s: spin timeout waiting for %{public}s to release", __FUNCTION__, counterName);
        }
    }
    return (__atomic_load_n(counter, __ATOMIC_ACQUIRE) == 0);
}

// Clear the registered callback state under the callback mutex and, when all
// references have drained, delete the mutex and event. The lifecycle lock is
// held by the caller and must remain held throughout (it serializes this
// against EnsureResources), while the event pointer exchange is serialized
// against GapLeCteCallbackSignalRefZero by the event mutex.
static void GapLeCteCallbackClearState(Mutex *mtx, Event *event, bool refsDrained, bool inFlightDrained)
{
    MutexLock(mtx);

    (void)memset_s(&g_leCteCallback, sizeof(g_leCteCallback), 0x00, sizeof(g_leCteCallback));
    MutexUnlock(mtx);

    // Only delete the mutex and event if all references and in-flight callbacks have drained.
    // Deleting an in-use mutex or an event with waiters is undefined behavior;
    // leaking them on timeout is safer than crashing.
    if (refsDrained && inFlightDrained) {
        MutexDelete(mtx);
        if (event != NULL) {
            GapLeCteCallbackClearRefEvent();
        }
    } else {
        LOG_ERROR("%{public}s: leaving CTE callback mutex/event allocated due to "
            "unreleased references or in-flight callbacks",
            __FUNCTION__);
    }
}

int GapLeCteCallbackInit(void)
{
    Mutex *lifecycleMutex = GapLeCteCallbackEnsureLifecycleMutex();
    if (lifecycleMutex == NULL) {
        return GAP_ERR_OUT_OF_RES;
    }

    return GapLeCteCallbackEnsureResources(lifecycleMutex);
}

void GapLeCteCallbackDeinit(void)
{
    Mutex *lifecycleMutex = __atomic_load_n(&g_leCteLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        return;
    }

    MutexLock(lifecycleMutex);

    Mutex *mtx = __atomic_exchange_n(&g_leCteCallbackMutex, NULL, __ATOMIC_ACQ_REL);
    if (mtx == NULL) {
        MutexUnlock(lifecycleMutex);
        return;
    }

    // No new reader can acquire the mutex now that the global pointer is NULL.
    // Wait until every reader that observed a non-NULL pointer has released its
    // reference, and every in-flight callback has finished. The drains run
    // while the lifecycle lock is held here; the ref-zero signal takes only the
    // event mutex (see GapLeCteCallbackSignalRefZero), so the event wakeup is
    // not blocked and the waits converge at event speed.
    Event *event = __atomic_load_n(&g_leCteCallbackRefEvent, __ATOMIC_ACQUIRE);
    bool refsDrained = GapLeCteCallbackWaitDrain(&g_leCteCallbackRef, event, "CTE callbacks");
    bool inFlightDrained = GapLeCteCallbackWaitDrain(&g_leCteCallbackInFlight, event, "in-flight CTE callbacks");

    GapLeCteCallbackClearState(mtx, event, refsDrained, inFlightDrained);

    MutexUnlock(lifecycleMutex);
    // Intentionally keep g_leCteLifecycleMutex alive: other threads may
    // still be racing through register/deregister/get paths and need the lifecycle lock
    // to observe that the inner mutex is gone.
}

bool GapLeCteCallbackGet(GapLeCteCallback *callback, void **context)
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

    Mutex *lifecycleMutex = __atomic_load_n(&g_leCteLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        *context = NULL;
        return false;
    }

    MutexLock(lifecycleMutex);
    Mutex *mtx = __atomic_load_n(&g_leCteCallbackMutex, __ATOMIC_ACQUIRE);
    if (mtx == NULL) {
        MutexUnlock(lifecycleMutex);
        (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        *context = NULL;
        return false;
    }

    MutexLock(mtx);
    *callback = g_leCteCallback.callback;
    *context = g_leCteCallback.context;
    (void)__atomic_fetch_add(&g_leCteCallbackInFlight, 1, __ATOMIC_SEQ_CST);
    MutexUnlock(mtx);
    MutexUnlock(lifecycleMutex);
    return true;
}

int GAP_LeRegisterCteCallback(const GapLeCteCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "deregister");

    Mutex *mtx = NULL;
    if (!GapLeCteCallbackTryLock(&mtx)) {
        return GAP_ERR_OUT_OF_RES;
    }

    if (callback == NULL) {
        (void)memset_s(&g_leCteCallback.callback, sizeof(g_leCteCallback.callback), 0x00,
            sizeof(g_leCteCallback.callback));
        g_leCteCallback.context = NULL;
    } else {
        LOG_INFO("%{public}s: replacing existing CTE callback", __FUNCTION__);
        // New registration replaces any previous one; this matches the
        // single-callback-slot design used by the other GAP callback groups.
        g_leCteCallback.callback = *callback;
        g_leCteCallback.context = context;
    }

    GapLeCteCallbackUnlock(mtx);
    return GAP_SUCCESS;
}

int GAP_LeDeregisterCteCallback(void)
{
    Mutex *mtx = NULL;
    if (!GapLeCteCallbackTryLock(&mtx)) {
        return GAP_ERR_OUT_OF_RES;
    }

    (void)memset_s(&g_leCteCallback.callback, sizeof(g_leCteCallback.callback), 0x00,
        sizeof(g_leCteCallback.callback));
    g_leCteCallback.context = NULL;

    GapLeCteCallbackUnlock(mtx);
    return GAP_SUCCESS;
}

static int GapLeSetConnectionCteReceiveParams(uint16_t connectionHandle, uint8_t samplingEnable,
    uint8_t slotDurations, uint8_t lengthOfSwitchingPattern, const uint8_t *antennaIds)
{
    HciLeSetConnectionCteReceiveParametersParam hciCmdParam = {
        .connectionHandle = connectionHandle,
        .samplingEnable = samplingEnable,
        .slotDurations = slotDurations,
        .lengthOfSwitchingPattern = lengthOfSwitchingPattern,
        .antennaIds = antennaIds,
    };

    return HCI_LeSetConnectionCteReceiveParameters(&hciCmdParam);
}

int GAP_LeSetConnectionCteReceiveParameters(uint16_t connectionHandle, uint8_t samplingEnable,
    uint8_t slotDurations, uint8_t lengthOfSwitchingPattern, const uint8_t *antennaIds)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX ||
        samplingEnable > GAP_PERIODIC_ADV_ENABLE_TRUE ||
        (slotDurations != GAP_LE_CTE_SLOT_DURATIONS_1US && slotDurations != GAP_LE_CTE_SLOT_DURATIONS_2US)) {
        return GAP_ERR_INVAL_PARAM;
    }

    ret = GapLeCteAntennaIdsCheck(lengthOfSwitchingPattern, antennaIds);
    if (ret != GAP_SUCCESS) {
        return ret;
    }

    if (!BTM_IsControllerSupportConnectionCteRequest()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeSetConnectionCteReceiveParams(
            connectionHandle, samplingEnable, slotDurations, lengthOfSwitchingPattern, antennaIds);
    }

    return ret;
}

NO_SANITIZE("cfi")
void GapLeSetConnectionCteReceiveParametersComplete(
    const HciLeSetConnectionCteReceiveParametersReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeCteCallback callback;
    void *context = NULL;
    if (GapLeCteCallbackGet(&callback, &context)) {
        if (callback.setConnectionCteReceiveParametersResult) {
            callback.setConnectionCteReceiveParametersResult(param->status, context);
        }
        GapLeCteCallbackRelease();
    }
}

static int GapLeSetConnectionCteTransmitParams(uint16_t connectionHandle, uint8_t cteTypes,
    uint8_t lengthOfSwitchingPattern, const uint8_t *antennaIds)
{
    HciLeSetConnectionCteTransmitParametersParam hciCmdParam = {
        .connectionHandle = connectionHandle,
        .cteTypes = cteTypes,
        .lengthOfSwitchingPattern = lengthOfSwitchingPattern,
        .antennaIds = antennaIds,
    };

    return HCI_LeSetConnectionCteTransmitParameters(&hciCmdParam);
}

int GAP_LeSetConnectionCteTransmitParameters(uint16_t connectionHandle, uint8_t cteTypes,
    uint8_t lengthOfSwitchingPattern, const uint8_t *antennaIds)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    // CTE_Types is a bit mask: bit 0 no AoA, bit 1 no AoD 1us, bit 2 no AoD 2us,
    // bit 4 no CTE (Bluetooth 5.1, Vol 2, Part E, 7.8.84).
    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX ||
        (cteTypes & ~GAP_LE_PAST_CTE_TYPE_NO_CTE_MASK_ALL) != 0) {
        return GAP_ERR_INVAL_PARAM;
    }

    ret = GapLeCteAntennaIdsCheck(lengthOfSwitchingPattern, antennaIds);
    if (ret != GAP_SUCCESS) {
        return ret;
    }

    if (!BTM_IsControllerSupportConnectionCteResponse()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeSetConnectionCteTransmitParams(
            connectionHandle, cteTypes, lengthOfSwitchingPattern, antennaIds);
    }

    return ret;
}

NO_SANITIZE("cfi")
void GapLeSetConnectionCteTransmitParametersComplete(
    const HciLeSetConnectionCteTransmitParametersReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeCteCallback callback;
    void *context = NULL;
    if (GapLeCteCallbackGet(&callback, &context)) {
        if (callback.setConnectionCteTransmitParametersResult) {
            callback.setConnectionCteTransmitParametersResult(param->status, context);
        }
        GapLeCteCallbackRelease();
    }
}

static int GapLeConnectionCteRequestEnableCmd(uint16_t connectionHandle, uint8_t enable,
    uint16_t cteRequestInterval, uint8_t requestedCteLength, uint8_t requestedCteType)
{
    HciLeConnectionCteRequestEnableParam hciCmdParam = {
        .connectionHandle = connectionHandle,
        .enable = enable,
        .cteRequestInterval = cteRequestInterval,
        .requestedCteLength = requestedCteLength,
        .requestedCteType = requestedCteType,
    };

    return HCI_LeConnectionCteRequestEnable(&hciCmdParam);
}

int GAP_LeConnectionCteRequestEnable(
    uint16_t connectionHandle, uint8_t enable, uint16_t cteRequestInterval, uint8_t requestedCteLength,
    uint8_t requestedCteType)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX ||
        enable > GAP_PERIODIC_ADV_ENABLE_TRUE ||
        requestedCteLength > GAP_LE_CTE_LENGTH_MAX ||
        (requestedCteType != GAP_LE_CTE_TYPE_AOA && requestedCteType != GAP_LE_CTE_TYPE_AOD_1US &&
         requestedCteType != GAP_LE_CTE_TYPE_AOD_2US)) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (!BTM_IsControllerSupportConnectionCteRequest()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeConnectionCteRequestEnableCmd(
            connectionHandle, enable, cteRequestInterval, requestedCteLength, requestedCteType);
    }

    return ret;
}

NO_SANITIZE("cfi")
void GapLeConnectionCteRequestEnableComplete(const HciLeConnectionCteRequestEnableReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeCteCallback callback;
    void *context = NULL;
    if (GapLeCteCallbackGet(&callback, &context)) {
        if (callback.connectionCteRequestEnableResult) {
            callback.connectionCteRequestEnableResult(param->status, context);
        }
        GapLeCteCallbackRelease();
    }
}

static int GapLeConnectionCteResponseEnableCmd(uint16_t connectionHandle, uint8_t enable)
{
    HciLeConnectionCteResponseEnableParam hciCmdParam = {
        .connectionHandle = connectionHandle,
        .enable = enable,
    };

    return HCI_LeConnectionCteResponseEnable(&hciCmdParam);
}

int GAP_LeConnectionCteResponseEnable(uint16_t connectionHandle, uint8_t enable)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX || enable > GAP_PERIODIC_ADV_ENABLE_TRUE) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (!BTM_IsControllerSupportConnectionCteResponse()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeConnectionCteResponseEnableCmd(connectionHandle, enable);
    }

    return ret;
}

NO_SANITIZE("cfi")
void GapLeConnectionCteResponseEnableComplete(const HciLeConnectionCteResponseEnableReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeCteCallback callback;
    void *context = NULL;
    if (GapLeCteCallbackGet(&callback, &context)) {
        if (callback.connectionCteResponseEnableResult) {
            callback.connectionCteResponseEnableResult(param->status, context);
        }
        GapLeCteCallbackRelease();
    }
}

int GAP_LeReadAntennaInformation(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (!BTM_IsControllerSupportAntennaSwitchingDuringCteTransmissionAod() &&
        !BTM_IsControllerSupportAntennaSwitchingDuringCteReceptionAoa()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    return HCI_LeReadAntennaInformation();
}

NO_SANITIZE("cfi")
void GapLeReadAntennaInformationComplete(const HciLeReadAntennaInformationReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeCteCallback callback;
    void *context = NULL;
    if (GapLeCteCallbackGet(&callback, &context)) {
        if (callback.readAntennaInformationResult) {
            callback.readAntennaInformationResult(param->status,
                param->supportedSwitchingSamplingRates,
                param->numberOfAntennae,
                param->maxLengthOfSwitchingPattern,
                param->maxCteLength,
                context);
        }
        GapLeCteCallbackRelease();
    }
}

static int GapLePeriodicAdvSyncTransferCmd(uint16_t connectionHandle, uint16_t serviceData, uint16_t syncHandle)
{
    HciLePeriodicAdvertisingSyncTransferParam hciCmdParam = {
        .connectionHandle = connectionHandle,
        .serviceData = serviceData,
        .syncHandle = syncHandle,
    };

    return HCI_LePeriodicAdvertisingSyncTransfer(&hciCmdParam);
}

int GAP_LePeriodicAdvertisingSyncTransfer(uint16_t connectionHandle, uint16_t serviceData, uint16_t syncHandle)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX || syncHandle > GAP_PERIODIC_ADV_SYNC_HANDLE_MAX) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (!BTM_IsControllerSupportPeriodicAdvertisingSyncTransferSender()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLePeriodicAdvSyncTransferCmd(connectionHandle, serviceData, syncHandle);
    }

    return ret;
}

NO_SANITIZE("cfi")
void GapLePeriodicAdvertisingSyncTransferComplete(const HciLePeriodicAdvertisingSyncTransferReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeCteCallback callback;
    void *context = NULL;
    if (GapLeCteCallbackGet(&callback, &context)) {
        if (callback.periodicAdvertisingSyncTransferResult) {
            callback.periodicAdvertisingSyncTransferResult(param->status, context);
        }
        GapLeCteCallbackRelease();
    }
}

static int GapLePeriodicAdvSetInfoTransferCmd(uint16_t connectionHandle, uint16_t serviceData,
    uint8_t advertisingHandle)
{
    HciLePeriodicAdvertisingSetInfoTransferParam hciCmdParam = {
        .connectionHandle = connectionHandle,
        .serviceData = serviceData,
        .advertisingHandle = advertisingHandle,
    };

    return HCI_LePeriodicAdvertisingSetInfoTransfer(&hciCmdParam);
}

int GAP_LePeriodicAdvertisingSetInfoTransfer(uint16_t connectionHandle, uint16_t serviceData,
    uint8_t advertisingHandle)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX || advertisingHandle > GAP_LE_ADV_HANDLE_MAX) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (!BTM_IsControllerSupportPeriodicAdvertisingSyncTransferSender()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLePeriodicAdvSetInfoTransferCmd(connectionHandle, serviceData, advertisingHandle);
    }

    return ret;
}

NO_SANITIZE("cfi")
void GapLePeriodicAdvertisingSetInfoTransferComplete(
    const HciLePeriodicAdvertisingSetInfoTransferReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeCteCallback callback;
    void *context = NULL;
    if (GapLeCteCallbackGet(&callback, &context)) {
        if (callback.periodicAdvertisingSetInfoTransferResult) {
            callback.periodicAdvertisingSetInfoTransferResult(param->status, context);
        }
        GapLeCteCallbackRelease();
    }
}

static int GapLeSetPeriodicAdvSyncTransferParamsCmd(
    uint16_t connectionHandle, uint8_t mode, uint16_t skip, uint16_t syncTimeout, uint8_t cteType)
{
    HciLeSetPeriodicAdvertisingSyncTransferParametersParam hciCmdParam = {
        .connectionHandle = connectionHandle,
        .mode = mode,
        .skip = skip,
        .syncTimeout = syncTimeout,
        .cteType = cteType,
    };

    return HCI_LeSetPeriodicAdvertisingSyncTransferParameters(&hciCmdParam);
}

int GAP_LeSetPeriodicAdvertisingSyncTransferParameters(
    uint16_t connectionHandle, uint8_t mode, uint16_t skip, uint16_t syncTimeout, uint8_t cteType)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (connectionHandle > GAP_LE_CONNECTION_HANDLE_MAX || mode > GAP_LE_PAST_MODE_SYNC_REPORT ||
        skip > GAP_PERIODIC_ADV_SKIP_MAX ||
        syncTimeout < GAP_PERIODIC_ADV_SYNC_TIMEOUT_MIN || syncTimeout > GAP_PERIODIC_ADV_SYNC_TIMEOUT_MAX ||
        (cteType & ~GAP_LE_PAST_CTE_TYPE_NO_CTE_MASK_ALL) != 0) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (!BTM_IsControllerSupportPeriodicAdvertisingSyncTransferSender()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeSetPeriodicAdvSyncTransferParamsCmd(connectionHandle, mode, skip, syncTimeout, cteType);
    }

    return ret;
}

NO_SANITIZE("cfi")
void GapLeSetPeriodicAdvertisingSyncTransferParametersComplete(
    const HciLeSetPeriodicAdvertisingSyncTransferParametersReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeCteCallback callback;
    void *context = NULL;
    if (GapLeCteCallbackGet(&callback, &context)) {
        if (callback.setPeriodicAdvertisingSyncTransferParametersResult) {
            callback.setPeriodicAdvertisingSyncTransferParametersResult(param->status, context);
        }
        GapLeCteCallbackRelease();
    }
}

static int GapLeSetDefaultPeriodicAdvSyncTransferParamsCmd(
    uint8_t mode, uint16_t skip, uint16_t syncTimeout, uint8_t cteType)
{
    HciLeSetDefaultPeriodicAdvertisingSyncTransferParametersParam hciCmdParam = {
        .mode = mode,
        .skip = skip,
        .syncTimeout = syncTimeout,
        .cteType = cteType,
    };

    return HCI_LeSetDefaultPeriodicAdvertisingSyncTransferParameters(&hciCmdParam);
}

int GAP_LeSetDefaultPeriodicAdvertisingSyncTransferParameters(uint8_t mode, uint16_t skip, uint16_t syncTimeout,
    uint8_t cteType)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (mode > GAP_LE_PAST_MODE_SYNC_REPORT || skip > GAP_PERIODIC_ADV_SKIP_MAX ||
        syncTimeout < GAP_PERIODIC_ADV_SYNC_TIMEOUT_MIN || syncTimeout > GAP_PERIODIC_ADV_SYNC_TIMEOUT_MAX ||
        (cteType & ~GAP_LE_PAST_CTE_TYPE_NO_CTE_MASK_ALL) != 0) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (!BTM_IsControllerSupportPeriodicAdvertisingSyncTransferSender()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_PERIPHERAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeSetDefaultPeriodicAdvSyncTransferParamsCmd(mode, skip, syncTimeout, cteType);
    }

    return ret;
}

NO_SANITIZE("cfi")
void GapLeSetDefaultPeriodicAdvertisingSyncTransferParametersComplete(
    const HciLeSetDefaultPeriodicAdvertisingSyncTransferParametersReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeCteCallback callback;
    void *context = NULL;
    if (GapLeCteCallbackGet(&callback, &context)) {
        if (callback.setDefaultPeriodicAdvertisingSyncTransferParametersResult) {
            callback.setDefaultPeriodicAdvertisingSyncTransferParametersResult(param->status, context);
        }
        GapLeCteCallbackRelease();
    }
}

static int GapLeReceiverTestV3Cmd(const GapLeReceiverTestV3Param *param)
{
    HciLeReceiverTestV3Param hciCmdParam = {
        .rxChannel = param->rxChannel,
        .phy = param->phy,
        .modulationIndex = param->modulationIndex,
        .expectedCteLength = param->expectedCteLength,
        .expectedCteType = param->expectedCteType,
        .slotDurations = param->slotDurations,
        .lengthOfSwitchingPattern = param->lengthOfSwitchingPattern,
        .antennaIds = param->antennaIds,
    };

    return HCI_LeReceiverTestV3(&hciCmdParam);
}

int GAP_LeReceiverTestV3(const GapLeReceiverTestV3Param *param)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (param == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (param->rxChannel > GAP_LE_TEST_CHANNEL_MAX ||
        (param->phy != GAP_LE_PHY_1M && param->phy != GAP_LE_PHY_2M && param->phy != GAP_LE_PHY_CODED) ||
        param->modulationIndex > GAP_LE_TEST_MODULATION_INDEX_MAX ||
        (param->expectedCteLength != 0 &&
         (param->expectedCteLength < GAP_LE_CTE_LENGTH_MIN || param->expectedCteLength > GAP_LE_CTE_LENGTH_MAX)) ||
        (param->expectedCteType != GAP_LE_CTE_TYPE_AOA && param->expectedCteType != GAP_LE_CTE_TYPE_AOD_1US &&
         param->expectedCteType != GAP_LE_CTE_TYPE_AOD_2US) ||
        (param->slotDurations != GAP_LE_CTE_SLOT_DURATIONS_1US &&
         param->slotDurations != GAP_LE_CTE_SLOT_DURATIONS_2US)) {
        return GAP_ERR_INVAL_PARAM;
    }

    ret = GapLeCteAntennaIdsCheck(param->lengthOfSwitchingPattern, param->antennaIds);
    if (ret != GAP_SUCCESS) {
        return ret;
    }

    if (!BTM_IsControllerSupportReceivingConstantToneExtensions()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    return GapLeReceiverTestV3Cmd(param);
}

NO_SANITIZE("cfi")
void GapLeReceiverTestV3Complete(const HciLeReceiverTestV3ReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeCteCallback callback;
    void *context = NULL;
    if (GapLeCteCallbackGet(&callback, &context)) {
        if (callback.receiverTestV3Result) {
            callback.receiverTestV3Result(param->status, context);
        }
        GapLeCteCallbackRelease();
    }
}

static int GapLeTransmitterTestV3Cmd(const GapLeTransmitterTestV3Param *param)
{
    HciLeTransmitterTestV3Param hciCmdParam = {
        .txChannel = param->txChannel,
        .lengthOfTestData = param->lengthOfTestData,
        .packetPayload = param->packetPayload,
        .phy = param->phy,
        .cteLength = param->cteLength,
        .cteType = param->cteType,
        .lengthOfSwitchingPattern = param->lengthOfSwitchingPattern,
        .antennaIds = param->antennaIds,
    };

    return HCI_LeTransmitterTestV3(&hciCmdParam);
}

int GAP_LeTransmitterTestV3(const GapLeTransmitterTestV3Param *param)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (param == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (param->txChannel > GAP_LE_TEST_CHANNEL_MAX || param->lengthOfTestData > GAP_LE_TEST_DATA_LENGTH_MAX ||
        param->packetPayload > GAP_LE_TEST_PACKET_PAYLOAD_MAX ||
        (param->phy != GAP_LE_PHY_1M && param->phy != GAP_LE_PHY_2M && param->phy != GAP_LE_PHY_CODED &&
         param->phy != GAP_LE_TEST_PHY_CODED_S2) ||
        param->cteLength > GAP_LE_CTE_LENGTH_MAX ||
        (param->cteType != GAP_LE_CTE_TYPE_AOA && param->cteType != GAP_LE_CTE_TYPE_AOD_1US &&
         param->cteType != GAP_LE_CTE_TYPE_AOD_2US)) {
        return GAP_ERR_INVAL_PARAM;
    }

    ret = GapLeCteAntennaIdsCheck(param->lengthOfSwitchingPattern, param->antennaIds);
    if (ret != GAP_SUCCESS) {
        return ret;
    }

    if (!BTM_IsControllerSupportConnectionlessCteTransmitter()) {
        return GAP_ERR_NOT_SUPPORT;
    }

    return GapLeTransmitterTestV3Cmd(param);
}

NO_SANITIZE("cfi")
void GapLeTransmitterTestV3Complete(const HciLeTransmitterTestV3ReturnParam *param)
{
    if (param == NULL) {
        LOG_WARN("%{public}s: invalid param", __FUNCTION__);
        return;
    }

    GapLeCteCallback callback;
    void *context = NULL;
    if (GapLeCteCallbackGet(&callback, &context)) {
        if (callback.transmitterTestV3Result) {
            callback.transmitterTestV3Result(param->status, context);
        }
        GapLeCteCallbackRelease();
    }
}

void GapOnLeConnectionIqReportEvent(const HciLeConnectionIqReportEventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }

    LOG_INFO("%{public}s:connHandle:0x%04x, sampleCount:%hhu", __FUNCTION__, eventParam->connectionHandle,
        eventParam->sampleCount);

    GapLeCteCallback callback;
    void *context = NULL;
    if (GapLeCteCallbackGet(&callback, &context)) {
        if (callback.connectionIqReport != NULL) {
            // The I/Q samples (interleaved (I, Q) pairs in iqSamples) are only
            // valid during the callback.
            callback.connectionIqReport(eventParam, context);
        }
        GapLeCteCallbackRelease();
    }
}

void GapOnLeCteRequestFailedEvent(const HciLeCteRequestFailedEventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }

    LOG_INFO("%{public}s:connHandle:0x%04x, status:0x%02x", __FUNCTION__, eventParam->connectionHandle,
        eventParam->status);

    GapLeCteCallback callback;
    void *context = NULL;
    if (GapLeCteCallbackGet(&callback, &context)) {
        if (callback.cteRequestFailed != NULL) {
            callback.cteRequestFailed(eventParam->status, eventParam->connectionHandle, context);
        }
        GapLeCteCallbackRelease();
    }
}

#endif
