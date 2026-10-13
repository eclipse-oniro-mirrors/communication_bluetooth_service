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

// The public CS interface (CSIF_*, cs_le_if.h) as a thin layer over the CS engine
// (Cs*, cs.h). Three things happen here and nothing else:
// 1. Every entry runs its engine call on the CS processing queue through
//    CsRunTaskBlockProcess (cs_task_common.c), never on the caller's thread; the caller
//    blocks until it has run. That is what makes cs_le_if.h's two threading promises the
//    same single-threaded module: an entry touches the engine state on the Stack thread,
//    and every CS callback fires from that same thread as it drains the queue, so no
//    entry can run concurrently with a callback.
// 2. Every parameter and result body is converted field by field, never by memcpy of
//    the struct. The HCI types are #pragma pack(1) wire images (hci_def_le_cmd_6_0.h)
//    while the CsLe* mirrors are compiler-laid-out (cs_le_if.h, the iso_le_if.h
//    convention), so the two layouts differ and a whole-struct copy would be wrong
//    even where it compiles. Field assignment also makes the mirror self-checking: a
//    one-sided rename in either header stops the build here.
// 3. The one callback slot the CSIF_ entries expose is unrolled into the three engine
//    sinks (CsResultSink, CsSessionSink, CsTestSink), each of which the engine already
//    guards per member. A member the caller left NULL stays NULL here: see
//    CsIfRegisterCallbackTask.

#include "cs_le_if.h"

#include <securec.h>

#include "allocator.h"
#include "log.h"

#include "cs.h"
#include "cs_internal.h"

#include "hci/evt/hci_evt.h"

// One box per entry shape. The caller allocates it, blocks until its task has run, and
// frees it on return, so nothing in here outlives the call - which is also why
// CSIF_LeCsTestStart can hand the caller's override tail straight to the engine.
typedef struct {
    int result;
    uint16_t connectionHandle;
} CsIfHandleInfo;

typedef struct {
    int result;
    CsLeCapabilities caps;
} CsIfCapsInfo;

typedef struct {
    int result;
    HciLeCsSetDefaultSettingsParam param;
} CsIfSetDefaultSettingsInfo;

typedef struct {
    int result;
    HciLeCsCreateConfigParam param;
} CsIfCreateConfigInfo;

typedef struct {
    int result;
    HciLeCsRemoveConfigParam param;
} CsIfRemoveConfigInfo;

typedef struct {
    int result;
    HciLeCsSetChannelClassificationParam param;
} CsIfSetChannelClassificationInfo;

typedef struct {
    int result;
    HciLeCsSetProcedureParametersParam param;
} CsIfSetProcedureParametersInfo;

typedef struct {
    int result;
    HciLeCsProcedureEnableParam param;
} CsIfProcedureEnableInfo;

typedef struct {
    int result;
    HciLeCsTestParam param;
} CsIfTestStartInfo;

typedef struct {
    int result;
} CsIfGeneralInfo;

typedef struct {
    int result;
    CsLeCallback callback;
    void *context;
} CsIfCallbackInfo;

// The registered slot. Written only by CsIfRegisterCallbackTask and
// CsIfDeregisterCallbackTask, read only by the forwarders below - all of which run on
// the Stack thread, so the pair needs no lock of its own. The engine's own sinks are
// what a caller's thread could otherwise race, and those are locked inside the engine.
static CsLeCallback g_csLeCallback;
static void *g_csLeContext;

// --- parameter and result conversion ---------------------------------------------

static void CsIfCopyCaps(CsLeCapabilities *out, const HciLeCsCapabilities *in)
{
    out->numConfigSupported = in->numConfigSupported;
    out->maxConsecutiveProceduresSupported = in->maxConsecutiveProceduresSupported;
    out->numAntennasSupported = in->numAntennasSupported;
    out->maxAntennaPathsSupported = in->maxAntennaPathsSupported;
    out->rolesSupported = in->rolesSupported;
    out->modesSupported = in->modesSupported;
    out->rttCapability = in->rttCapability;
    out->rttAaOnlyN = in->rttAaOnlyN;
    out->rttSoundingN = in->rttSoundingN;
    out->rttRandomPayloadN = in->rttRandomPayloadN;
    out->nadmSoundingCapability = in->nadmSoundingCapability;
    out->nadmRandomCapability = in->nadmRandomCapability;
    out->csSyncPhysSupported = in->csSyncPhysSupported;
    out->subfeaturesSupported = in->subfeaturesSupported;
    out->tIp1TimesSupported = in->tIp1TimesSupported;
    out->tIp2TimesSupported = in->tIp2TimesSupported;
    out->tFcsTimesSupported = in->tFcsTimesSupported;
    out->tPmTimesSupported = in->tPmTimesSupported;
    out->tSwTimeSupported = in->tSwTimeSupported;
    out->txSnrCapability = in->txSnrCapability;
}

static void CsIfToHciSetDefaultSettings(
    const CsLeSetDefaultSettingsParam *in, HciLeCsSetDefaultSettingsParam *out)
{
    out->connectionHandle = in->connectionHandle;
    out->roleEnable = in->roleEnable;
    out->csSyncAntennaSelection = in->csSyncAntennaSelection;
    out->maxTxPower = in->maxTxPower;
}

static void CsIfToHciCreateConfig(const CsLeCreateConfigParam *in, HciLeCsCreateConfigParam *out)
{
    out->connectionHandle = in->connectionHandle;
    out->configId = in->configId;
    out->createContext = in->createContext;
    out->mainModeType = in->mainModeType;
    out->subModeType = in->subModeType;
    out->minMainModeSteps = in->minMainModeSteps;
    out->maxMainModeSteps = in->maxMainModeSteps;
    out->mainModeRepetition = in->mainModeRepetition;
    out->mode0Steps = in->mode0Steps;
    out->role = in->role;
    out->rttType = in->rttType;
    out->csSyncPhy = in->csSyncPhy;
    (void)memcpy_s(out->channelMap, sizeof(out->channelMap), in->channelMap, sizeof(in->channelMap));
    out->channelMapRepetition = in->channelMapRepetition;
    out->channelSelectionType = in->channelSelectionType;
    out->ch3cShape = in->ch3cShape;
    out->ch3cJump = in->ch3cJump;
    out->reserved = in->reserved;
}

static void CsIfToHciRemoveConfig(const CsLeRemoveConfigParam *in, HciLeCsRemoveConfigParam *out)
{
    out->connectionHandle = in->connectionHandle;
    out->configId = in->configId;
}

static void CsIfToHciSetChannelClassification(
    const CsLeSetChannelClassificationParam *in, HciLeCsSetChannelClassificationParam *out)
{
    (void)memcpy_s(out->channelClassification, sizeof(out->channelClassification),
        in->channelClassification, sizeof(in->channelClassification));
}

static void CsIfToHciSetProcedureParameters(
    const CsLeSetProcedureParametersParam *in, HciLeCsSetProcedureParametersParam *out)
{
    out->connectionHandle = in->connectionHandle;
    out->configId = in->configId;
    out->maxProcedureLen = in->maxProcedureLen;
    out->minProcedureInterval = in->minProcedureInterval;
    out->maxProcedureInterval = in->maxProcedureInterval;
    out->maxProcedureCount = in->maxProcedureCount;
    (void)memcpy_s(out->minSubeventLen, sizeof(out->minSubeventLen), in->minSubeventLen, sizeof(in->minSubeventLen));
    (void)memcpy_s(out->maxSubeventLen, sizeof(out->maxSubeventLen), in->maxSubeventLen, sizeof(in->maxSubeventLen));
    out->toneAntennaConfigSelection = in->toneAntennaConfigSelection;
    out->phy = in->phy;
    out->txPowerDelta = in->txPowerDelta;
    out->preferredPeerAntenna = in->preferredPeerAntenna;
    out->snrControlInitiator = in->snrControlInitiator;
    out->snrControlReflector = in->snrControlReflector;
}

static void CsIfToHciProcedureEnable(const CsLeProcedureEnableParam *in, HciLeCsProcedureEnableParam *out)
{
    out->connectionHandle = in->connectionHandle;
    out->configId = in->configId;
    out->enable = in->enable;
}

static void CsIfToHciTestPrefix(const CsLeTestPrefix *in, HciLeCsTestPrefix *out)
{
    out->mainModeType = in->mainModeType;
    out->subModeType = in->subModeType;
    out->mainModeRepetition = in->mainModeRepetition;
    out->mode0Steps = in->mode0Steps;
    out->role = in->role;
    out->rttType = in->rttType;
    out->csSyncPhy = in->csSyncPhy;
    out->csSyncAntennaSelection = in->csSyncAntennaSelection;
    (void)memcpy_s(out->subeventLen, sizeof(out->subeventLen), in->subeventLen, sizeof(in->subeventLen));
    out->subeventInterval = in->subeventInterval;
    out->maxNumSubevents = in->maxNumSubevents;
    out->transmitPowerLevel = in->transmitPowerLevel;
    out->tIp1Time = in->tIp1Time;
    out->tIp2Time = in->tIp2Time;
    out->tFcsTime = in->tFcsTime;
    out->tPmTime = in->tPmTime;
    out->tSwTime = in->tSwTime;
    out->toneAntennaConfigSelection = in->toneAntennaConfigSelection;
    out->reserved = in->reserved;
    out->snrControlInitiator = in->snrControlInitiator;
    out->snrControlReflector = in->snrControlReflector;
    out->drbgNonce = in->drbgNonce;
    out->channelMapRepetition = in->channelMapRepetition;
    out->overrideConfig = in->overrideConfig;
    out->overrideParametersLength = in->overrideParametersLength;
}

static void CsIfCopyConfig(const HciLeCsConfigCompleteEventParam *in, CsLeConfig *out)
{
    out->connectionHandle = in->connectionHandle;
    out->configId = in->configId;
    out->action = in->action;
    out->mainModeType = in->mainModeType;
    out->subModeType = in->subModeType;
    out->minMainModeSteps = in->minMainModeSteps;
    out->maxMainModeSteps = in->maxMainModeSteps;
    out->mainModeRepetition = in->mainModeRepetition;
    out->mode0Steps = in->mode0Steps;
    out->role = in->role;
    out->rttType = in->rttType;
    out->csSyncPhy = in->csSyncPhy;
    (void)memcpy_s(out->channelMap, sizeof(out->channelMap), in->channelMap, sizeof(in->channelMap));
    out->channelMapRepetition = in->channelMapRepetition;
    out->channelSelectionType = in->channelSelectionType;
    out->ch3cShape = in->ch3cShape;
    out->ch3cJump = in->ch3cJump;
    out->reserved = in->reserved;
    out->tIp1Time = in->tIp1Time;
    out->tIp2Time = in->tIp2Time;
    out->tFcsTime = in->tFcsTime;
    out->tPmTime = in->tPmTime;
}

static void CsIfCopyProcedureSchedule(const HciLeCsProcedureEnableCompleteEventParam *in, CsLeProcedureSchedule *out)
{
    out->connectionHandle = in->connectionHandle;
    out->configId = in->configId;
    out->state = in->state;
    out->toneAntennaConfigSelection = in->toneAntennaConfigSelection;
    out->selectedTxPower = in->selectedTxPower;
    (void)memcpy_s(out->subeventLen, sizeof(out->subeventLen), in->subeventLen, sizeof(in->subeventLen));
    out->subeventsPerEvent = in->subeventsPerEvent;
    out->subeventInterval = in->subeventInterval;
    out->eventInterval = in->eventInterval;
    out->procedureInterval = in->procedureInterval;
    out->procedureCount = in->procedureCount;
    out->maxProcedureLen = in->maxProcedureLen;
}

// --- callback forwarders and the callback registration ----------------------------
// One forwarder per sink member, each lifting the registered slot's context onto the
// engine's call. The three sinks partition the CS events between them (cs_internal.h),
// so each forwarder is entered for exactly one event and no event is reported twice.
// The status each public callback takes is the HCI Status of the event that produced
// it; the result bodies are built here rather than passed through, because the engine's
// types are the packed wire ones and the public ones are not.

static void CsIfOnRemoteCapabilitiesComplete(
    const HciLeCsReadRemoteSupportedCapabilitiesCompleteEventParam *eventParam)
{
    CsLeRemoteCapabilities caps = {0};
    caps.connectionHandle = eventParam->connectionHandle;
    CsIfCopyCaps(&caps.capabilities, &eventParam->capabilities);
    g_csLeCallback.remoteCapabilitiesComplete(eventParam->status, &caps, g_csLeContext);
}

static void CsIfOnRemoteFaeTableComplete(const HciLeCsReadRemoteFaeTableCompleteEventParam *eventParam)
{
    CsLeRemoteFaeTable table = {0};
    table.connectionHandle = eventParam->connectionHandle;
    (void)memcpy_s(table.remoteFaeTable, sizeof(table.remoteFaeTable),
        eventParam->remoteFaeTable, sizeof(eventParam->remoteFaeTable));
    g_csLeCallback.remoteFaeTableComplete(eventParam->status, &table, g_csLeContext);
}

static void CsIfOnWriteCachedRemoteCapabilitiesComplete(
    const HciLeCsWriteCachedRemoteSupportedCapabilitiesReturnParam *returnParam)
{
    g_csLeCallback.writeCachedRemoteCapabilitiesComplete(
        returnParam->status, returnParam->connectionHandle, g_csLeContext);
}

static void CsIfOnWriteCachedRemoteFaeTableComplete(
    const HciLeCsWriteCachedRemoteFaeTableReturnParam *returnParam)
{
    g_csLeCallback.writeCachedRemoteFaeTableComplete(
        returnParam->status, returnParam->connectionHandle, g_csLeContext);
}

static void CsIfOnSecurityEnableComplete(const HciLeCsSecurityEnableCompleteEventParam *eventParam)
{
    g_csLeCallback.securityEnableComplete(eventParam->status, eventParam->connectionHandle, g_csLeContext);
}

static void CsIfOnConfigComplete(const HciLeCsConfigCompleteEventParam *eventParam)
{
    CsLeConfig config = {0};
    CsIfCopyConfig(eventParam, &config);
    g_csLeCallback.configComplete(eventParam->status, &config, g_csLeContext);
}

static void CsIfOnProcedureEnableComplete(const HciLeCsProcedureEnableCompleteEventParam *eventParam)
{
    CsLeProcedureSchedule schedule = {0};
    CsIfCopyProcedureSchedule(eventParam, &schedule);
    g_csLeCallback.procedureEnableComplete(eventParam->status, &schedule, g_csLeContext);
}

static void CsIfOnProcedureParametersComplete(const HciLeCsSetProcedureParametersReturnParam *returnParam)
{
    g_csLeCallback.procedureParametersComplete(returnParam->status, returnParam->connectionHandle, g_csLeContext);
}

static void CsIfOnSetDefaultSettingsComplete(const HciLeCsSetDefaultSettingsReturnParam *returnParam)
{
    g_csLeCallback.setDefaultSettingsComplete(returnParam->status, returnParam->connectionHandle, g_csLeContext);
}

// No handle to carry: the 0x0092 return is Status alone, because the classification is
// Controller-wide rather than per-connection.
static void CsIfOnSetChannelClassificationComplete(const HciLeCsSetChannelClassificationReturnParam *returnParam)
{
    g_csLeCallback.setChannelClassificationComplete(returnParam->status, g_csLeContext);
}

// The report is the engine's own storage and is reused for this connection's next
// subevent, so it goes straight through: copying it here would cost 7 KB per subevent
// for a consumer that has to copy it out itself anyway.
static void CsIfOnSubeventResult(const CsSubeventReport *report)
{
    g_csLeCallback.subeventResult(report, g_csLeContext);
}

static void CsIfOnSessionClosed(uint16_t connectionHandle)
{
    g_csLeCallback.sessionClosed(connectionHandle, g_csLeContext);
}

static void CsIfOnTestComplete(uint8_t status)
{
    g_csLeCallback.testComplete(status, g_csLeContext);
}

static void CsIfOnTestEndComplete(uint8_t status)
{
    g_csLeCallback.testEndComplete(status, g_csLeContext);
}

// Every sink member the caller left NULL is registered as NULL, not as a forwarder that
// would drop the result. The engine keys on it: CsResultRegisterSink(NULL) is what "no
// consumer" means, and with none the engine neither decodes a subevent nor allocates its
// 7 KB report, so a caller that only wants the completion callbacks does not pay for the
// measurement path. A drop-forwarder would defeat exactly that.
// Built with one plain assignment per member rather than a conditional in an initializer,
// so the rule reads the same on every member it applies to.
static CsSessionSink CsIfBuildSessionSink(void)
{
    CsSessionSink sink;
    (void)memset_s(&sink, sizeof(sink), 0, sizeof(sink));
    if (g_csLeCallback.remoteCapabilitiesComplete != NULL) {
        sink.remoteCapabilitiesComplete = CsIfOnRemoteCapabilitiesComplete;
    }
    if (g_csLeCallback.remoteFaeTableComplete != NULL) {
        sink.remoteFaeTableComplete = CsIfOnRemoteFaeTableComplete;
    }
    if (g_csLeCallback.writeCachedRemoteCapabilitiesComplete != NULL) {
        sink.writeCachedRemoteCapabilitiesComplete = CsIfOnWriteCachedRemoteCapabilitiesComplete;
    }
    if (g_csLeCallback.writeCachedRemoteFaeTableComplete != NULL) {
        sink.writeCachedRemoteFaeTableComplete = CsIfOnWriteCachedRemoteFaeTableComplete;
    }
    if (g_csLeCallback.securityEnableComplete != NULL) {
        sink.securityEnableComplete = CsIfOnSecurityEnableComplete;
    }
    if (g_csLeCallback.configComplete != NULL) {
        sink.configComplete = CsIfOnConfigComplete;
    }
    if (g_csLeCallback.procedureEnableComplete != NULL) {
        sink.procedureEnableComplete = CsIfOnProcedureEnableComplete;
    }
    if (g_csLeCallback.procedureParametersComplete != NULL) {
        sink.procedureParametersComplete = CsIfOnProcedureParametersComplete;
    }
    if (g_csLeCallback.setDefaultSettingsComplete != NULL) {
        sink.setDefaultSettingsComplete = CsIfOnSetDefaultSettingsComplete;
    }
    if (g_csLeCallback.setChannelClassificationComplete != NULL) {
        sink.setChannelClassificationComplete = CsIfOnSetChannelClassificationComplete;
    }
    if (g_csLeCallback.sessionClosed != NULL) {
        sink.sessionClosed = CsIfOnSessionClosed;
    }
    return sink;
}

static CsTestSink CsIfBuildTestSink(void)
{
    CsTestSink sink;
    (void)memset_s(&sink, sizeof(sink), 0, sizeof(sink));
    if (g_csLeCallback.testComplete != NULL) {
        sink.testComplete = CsIfOnTestComplete;
    }
    if (g_csLeCallback.testEndComplete != NULL) {
        sink.testEndComplete = CsIfOnTestEndComplete;
    }
    return sink;
}

// The three registrations are done together here, in one task on the Stack thread, so a
// dispatch cannot read a half-applied set: the reactor does not run tasks while this one
// is executing, and every sink the engine holds is read from that same dispatch.
static void CsIfRegisterCallbackTask(void *ctx)
{
    CsIfCallbackInfo *info = ctx;
    if (HciGetEventCallbackListLock() == NULL) {
        info->result = BT_NOT_SUPPORT;
        return;
    }
    g_csLeCallback = info->callback;
    g_csLeContext = info->context;

    CsResultSink resultSink = NULL;
    if (g_csLeCallback.subeventResult != NULL) {
        resultSink = CsIfOnSubeventResult;
    }
    CsSessionSink sessionSink = CsIfBuildSessionSink();
    CsTestSink testSink = CsIfBuildTestSink();

    // CsSessionRegisterSink and CsTestRegisterSink copy the struct, so these two locals
    // need not outlive the call.
    CsResultRegisterSink(resultSink);
    CsSessionRegisterSink(&sessionSink);
    CsTestRegisterSink(&testSink);
    info->result = BT_SUCCESS;
}

// Clears all three sinks and the slot. Only the result registration releases buffered
// reports - every connection's stream and the test report, inside CsResultRegisterSink -
// so a later registration starts from empty buffers. The other two only swap their structs.
// The register task's gate is mirrored here: with the event layer already closed the slot
// cannot be reached again, so the task refuses - it does not half-remove - and the caller
// keeps a failure code to deregister again once the event layer is back.
static void CsIfDeregisterCallbackTask(void *ctx)
{
    CsIfCallbackInfo *info = ctx;
    if (HciGetEventCallbackListLock() == NULL) {
        LOG_WARN("%{public}s: event layer is closed, the CS callback slot stays as it is", __FUNCTION__);
        info->result = BT_NOT_SUPPORT;
        return;
    }
    (void)memset_s(&g_csLeCallback, sizeof(g_csLeCallback), 0, sizeof(g_csLeCallback));
    g_csLeContext = NULL;
    CsResultRegisterSink(NULL);
    CsSessionRegisterSink(NULL);
    CsTestRegisterSink(NULL);
    info->result = BT_SUCCESS;
}

int CSIF_RegisterLeCsCallback(const CsLeCallback *callback, void *context)
{
    if (callback == NULL) {
        return CSIF_DeregisterLeCsCallback();
    }
    LOG_INFO("%{public}s: ", __FUNCTION__);
    CsIfCallbackInfo *info = MEM_MALLOC.alloc(sizeof(CsIfCallbackInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    info->callback = *callback;
    info->context = context;

    int ret = CsRunTaskBlockProcess(CsIfRegisterCallbackTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

int CSIF_DeregisterLeCsCallback(void)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    CsIfCallbackInfo *info = MEM_MALLOC.alloc(sizeof(CsIfCallbackInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }

    int ret = CsRunTaskBlockProcess(CsIfDeregisterCallbackTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

// --- session ----------------------------------------------------------------------

static void CsIfEnableSessionTask(void *ctx)
{
    CsIfHandleInfo *info = ctx;
    info->result = CsOpenSession(info->connectionHandle);
}

int CSIF_LeCsEnableSession(uint16_t connectionHandle)
{
    if (connectionHandle > LE_CS_CONNECTION_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: handle:0x%04x", __FUNCTION__, connectionHandle);
    CsIfHandleInfo *info = MEM_MALLOC.alloc(sizeof(CsIfHandleInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    info->connectionHandle = connectionHandle;

    int ret = CsRunTaskBlockProcess(CsIfEnableSessionTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

static void CsIfDisableSessionTask(void *ctx)
{
    CsIfHandleInfo *info = ctx;
    info->result = CsCloseSession(info->connectionHandle);
}

int CSIF_LeCsDisableSession(uint16_t connectionHandle)
{
    if (connectionHandle > LE_CS_CONNECTION_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: handle:0x%04x", __FUNCTION__, connectionHandle);
    CsIfHandleInfo *info = MEM_MALLOC.alloc(sizeof(CsIfHandleInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    info->connectionHandle = connectionHandle;

    int ret = CsRunTaskBlockProcess(CsIfDisableSessionTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

// The engine fills the caps only on success, so the copy-back is gated on the same
// success rather than leaving the caller's struct holding whatever the box was
// allocated with.
static void CsIfReadLocalCapsTask(void *ctx)
{
    CsIfCapsInfo *info = ctx;
    HciLeCsCapabilities hciCaps = {0};
    info->result = CsGetLocalCapabilities(&hciCaps);
    if (info->result == BT_SUCCESS) {
        CsIfCopyCaps(&info->caps, &hciCaps);
    }
}

int CSIF_LeCsReadLocalCaps(CsLeCapabilities *caps)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    if (caps == NULL) {
        return BT_BAD_PARAM;
    }
    CsIfCapsInfo *info = MEM_MALLOC.alloc(sizeof(CsIfCapsInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }

    int ret = CsRunTaskBlockProcess(CsIfReadLocalCapsTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    if (ret == BT_SUCCESS) {
        *caps = info->caps;
    }
    MEM_MALLOC.free(info);
    return ret;
}

static void CsIfReadRemoteCapsTask(void *ctx)
{
    CsIfHandleInfo *info = ctx;
    info->result = CsReadRemoteCapabilities(info->connectionHandle);
}

int CSIF_LeCsReadRemoteCaps(uint16_t connectionHandle)
{
    if (connectionHandle > LE_CS_CONNECTION_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: handle:0x%04x", __FUNCTION__, connectionHandle);
    CsIfHandleInfo *info = MEM_MALLOC.alloc(sizeof(CsIfHandleInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    info->connectionHandle = connectionHandle;

    int ret = CsRunTaskBlockProcess(CsIfReadRemoteCapsTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

static void CsIfReadRemoteFaeTask(void *ctx)
{
    CsIfHandleInfo *info = ctx;
    info->result = CsReadRemoteFae(info->connectionHandle);
}

int CSIF_LeCsReadRemoteFae(uint16_t connectionHandle)
{
    if (connectionHandle > LE_CS_CONNECTION_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: handle:0x%04x", __FUNCTION__, connectionHandle);
    CsIfHandleInfo *info = MEM_MALLOC.alloc(sizeof(CsIfHandleInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    info->connectionHandle = connectionHandle;

    int ret = CsRunTaskBlockProcess(CsIfReadRemoteFaeTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

static void CsIfWriteBackRemoteCapsTask(void *ctx)
{
    CsIfHandleInfo *info = ctx;
    info->result = CsWriteBackRemoteCapabilities(info->connectionHandle);
}

int CSIF_LeCsWriteBackRemoteCaps(uint16_t connectionHandle)
{
    if (connectionHandle > LE_CS_CONNECTION_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: handle:0x%04x", __FUNCTION__, connectionHandle);
    CsIfHandleInfo *info = MEM_MALLOC.alloc(sizeof(CsIfHandleInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    info->connectionHandle = connectionHandle;

    int ret = CsRunTaskBlockProcess(CsIfWriteBackRemoteCapsTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

static void CsIfWriteBackRemoteFaeTask(void *ctx)
{
    CsIfHandleInfo *info = ctx;
    info->result = CsWriteBackRemoteFaeTable(info->connectionHandle);
}

int CSIF_LeCsWriteBackRemoteFae(uint16_t connectionHandle)
{
    if (connectionHandle > LE_CS_CONNECTION_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: handle:0x%04x", __FUNCTION__, connectionHandle);
    CsIfHandleInfo *info = MEM_MALLOC.alloc(sizeof(CsIfHandleInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    info->connectionHandle = connectionHandle;

    int ret = CsRunTaskBlockProcess(CsIfWriteBackRemoteFaeTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

static void CsIfSecurityEnableTask(void *ctx)
{
    CsIfHandleInfo *info = ctx;
    info->result = CsSecurityEnable(info->connectionHandle);
}

int CSIF_LeCsSecurityEnable(uint16_t connectionHandle)
{
    if (connectionHandle > LE_CS_CONNECTION_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: handle:0x%04x", __FUNCTION__, connectionHandle);
    CsIfHandleInfo *info = MEM_MALLOC.alloc(sizeof(CsIfHandleInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    info->connectionHandle = connectionHandle;

    int ret = CsRunTaskBlockProcess(CsIfSecurityEnableTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

// --- configuration ----------------------------------------------------------------

static void CsIfSetDefaultSettingsTask(void *ctx)
{
    CsIfSetDefaultSettingsInfo *info = ctx;
    info->result = CsSetDefaultSettings(&info->param);
}

int CSIF_LeCsSetDefaultSettings(const CsLeSetDefaultSettingsParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: handle:0x%04x, roleEnable:0x%02x", __FUNCTION__, param->connectionHandle, param->roleEnable);
    CsIfSetDefaultSettingsInfo *info = MEM_MALLOC.alloc(sizeof(CsIfSetDefaultSettingsInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    CsIfToHciSetDefaultSettings(param, &info->param);

    int ret = CsRunTaskBlockProcess(CsIfSetDefaultSettingsTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

static void CsIfCreateConfigTask(void *ctx)
{
    CsIfCreateConfigInfo *info = ctx;
    info->result = CsCreateConfig(&info->param);
}

int CSIF_LeCsCreateConfig(const CsLeCreateConfigParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: handle:0x%04x, configId:%hhu", __FUNCTION__, param->connectionHandle, param->configId);
    CsIfCreateConfigInfo *info = MEM_MALLOC.alloc(sizeof(CsIfCreateConfigInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    CsIfToHciCreateConfig(param, &info->param);

    int ret = CsRunTaskBlockProcess(CsIfCreateConfigTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

static void CsIfRemoveConfigTask(void *ctx)
{
    CsIfRemoveConfigInfo *info = ctx;
    info->result = CsRemoveConfig(&info->param);
}

int CSIF_LeCsRemoveConfig(const CsLeRemoveConfigParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: handle:0x%04x, configId:%hhu", __FUNCTION__, param->connectionHandle, param->configId);
    CsIfRemoveConfigInfo *info = MEM_MALLOC.alloc(sizeof(CsIfRemoveConfigInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    CsIfToHciRemoveConfig(param, &info->param);

    int ret = CsRunTaskBlockProcess(CsIfRemoveConfigTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

static void CsIfSetChannelClassificationTask(void *ctx)
{
    CsIfSetChannelClassificationInfo *info = ctx;
    info->result = CsSetChannelClassification(&info->param);
}

int CSIF_LeCsSetChannelClassification(const CsLeSetChannelClassificationParam *param)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    CsIfSetChannelClassificationInfo *info = MEM_MALLOC.alloc(sizeof(CsIfSetChannelClassificationInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    CsIfToHciSetChannelClassification(param, &info->param);

    int ret = CsRunTaskBlockProcess(CsIfSetChannelClassificationTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

// --- procedure scheduling ---------------------------------------------------------

static void CsIfSetProcedureParametersTask(void *ctx)
{
    CsIfSetProcedureParametersInfo *info = ctx;
    info->result = CsSetProcedureParameters(&info->param);
}

int CSIF_LeCsSetProcedureParameters(const CsLeSetProcedureParametersParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: handle:0x%04x, configId:%hhu", __FUNCTION__, param->connectionHandle, param->configId);
    CsIfSetProcedureParametersInfo *info = MEM_MALLOC.alloc(sizeof(CsIfSetProcedureParametersInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    CsIfToHciSetProcedureParameters(param, &info->param);

    int ret = CsRunTaskBlockProcess(CsIfSetProcedureParametersTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

static void CsIfProcedureEnableTask(void *ctx)
{
    CsIfProcedureEnableInfo *info = ctx;
    info->result = CsProcedureEnable(&info->param);
}

int CSIF_LeCsProcedureEnable(const CsLeProcedureEnableParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: handle:0x%04x, configId:%hhu, enable:%hhu", __FUNCTION__,
        param->connectionHandle, param->configId, param->enable);
    CsIfProcedureEnableInfo *info = MEM_MALLOC.alloc(sizeof(CsIfProcedureEnableInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    CsIfToHciProcedureEnable(param, &info->param);

    int ret = CsRunTaskBlockProcess(CsIfProcedureEnableTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

// --- CS test ----------------------------------------------------------------------

static void CsIfTestStartTask(void *ctx)
{
    CsIfTestStartInfo *info = ctx;
    info->result = CsTestStart(&info->param);
}

int CSIF_LeCsTestStart(const CsLeTestParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    LOG_INFO("%{public}s: mainModeType:%hhu, role:%hhu", __FUNCTION__,
        param->prefix.mainModeType, param->prefix.role);
    CsIfTestStartInfo *info = MEM_MALLOC.alloc(sizeof(CsIfTestStartInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }
    CsIfToHciTestPrefix(&param->prefix, &info->param.prefix);
    // The override tail is deliberately not copied: it is a pointer into the caller's
    // own storage, only its owner can lay it out, and the caller blocks here until the
    // engine has appended it to the command.
    info->param.overrideParametersData = param->overrideParametersData;

    int ret = CsRunTaskBlockProcess(CsIfTestStartTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}

static void CsIfTestEndTask(void *ctx)
{
    CsIfGeneralInfo *info = ctx;
    info->result = CsTestEnd();
}

int CSIF_LeCsTestEnd(void)
{
    LOG_INFO("%{public}s: ", __FUNCTION__);
    CsIfGeneralInfo *info = MEM_MALLOC.alloc(sizeof(CsIfGeneralInfo));
    if (info == NULL) {
        return BT_NO_MEMORY;
    }

    int ret = CsRunTaskBlockProcess(CsIfTestEndTask, info);
    if (ret == BT_SUCCESS) {
        ret = info->result;
    }
    MEM_MALLOC.free(info);
    return ret;
}
