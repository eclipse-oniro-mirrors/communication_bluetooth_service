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

#include "hci/hci.h"
#include "hci/hci_def_link_ctrl_cmd.h"
#include "hci/hci_error.h"
#include "hci/iso/hci_iso.h"

#include "btm/btm_thread.h"

static bool IsoFindCigByCigId(void *data, void *parameter)
{
    IsoCigInfo *cigInfo = data;
    uint8_t cigId = *(uint8_t *)parameter;
    return cigInfo->cigId == cigId;
}

static IsoCigInfo *IsoFindCig(IsoLeMng *mng, uint8_t cigId)
{
    return ListForEachData(mng->cigBlock.cigList, IsoFindCigByCigId, &cigId);
}

static bool IsoFindCisByHandle(void *data, void *parameter)
{
    IsoCisInfo *cisInfo = data;
    uint16_t cisHandle = *(uint16_t *)parameter;
    return cisInfo->cisHandle == cisHandle;
}

static IsoCisInfo *IsoFindCis(IsoLeMng *mng, uint16_t cisHandle)
{
    return ListForEachData(mng->cisList, IsoFindCisByHandle, &cisHandle);
}

int IsoRegisterCigCallback(const IsoLeCigCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "NULL");
    IsoLeMng *mng = IsoGetMng();
    mng->callback = callback;
    mng->callbackContext = context;
    return BT_SUCCESS;
}

int IsoDeregisterCigCallback(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    IsoLeMng *mng = IsoGetMng();
    mng->callback = NULL;
    mng->callbackContext = NULL;
    return BT_SUCCESS;
}

// Validate the LE Set CIG Parameters payload per BLUETOOTH SPECIFICATION Version 5.2
// Vol 4, Part E 7.8.97: CIG_ID 0x00-0xEF, Slave_Clock_Accuracy 0x00-0x07, Packing/Framing
// 0x00-0x01, SDUs up to 0xFFFFFF, per-CIS PHY 0x00-0x02 and RTN 0x00-0x1F.
static bool IsoLeCreateCigParamValid(uint8_t cigId, const IsoLeCigParam *cigParam, uint8_t cisCount,
    const IsoLeCisParam *cisParams)
{
    if (cigParam == NULL || cisCount == 0 || cisCount > ISO_LE_CIS_COUNT_MAX || cisParams == NULL) {
        return false;
    }
    if (cigParam->sduIntervalMToS > 0xFFFFFF || cigParam->sduIntervalSToM > 0xFFFFFF) {
        return false;
    }
    if (cigId > 0xEF || cigParam->slaveClockAccuracy > 0x07 || cigParam->packing > 0x01 ||
        cigParam->framing > 0x01) {
        return false;
    }
    for (uint8_t i = 0; i < cisCount; i++) {
        if (cisParams[i].phyMToS > 0x02 || cisParams[i].phySToM > 0x02 || cisParams[i].rtnMToS > 0x1F ||
            cisParams[i].rtnSToM > 0x1F) {
            return false;
        }
    }
    return true;
}

int IsoLeCreateCig(uint8_t cigId, const IsoLeCigParam *cigParam, uint8_t cisCount, const IsoLeCisParam *cisParams)
{
    LOG_INFO("%{public}s: cigId:0x%02x, cisCount:%hhu", __FUNCTION__, cigId, cisCount);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }
    if (!IsoLeCreateCigParamValid(cigId, cigParam, cisCount, cisParams)) {
        return BT_BAD_PARAM;
    }

    HciLeSetCigParametersParam hciParam = { 0 };
    hciParam.cigId = cigId;
    IsoWriteUint24(hciParam.sduIntervalMToS, cigParam->sduIntervalMToS);
    IsoWriteUint24(hciParam.sduIntervalSToM, cigParam->sduIntervalSToM);
    hciParam.slaveClockAccuracy = cigParam->slaveClockAccuracy;
    hciParam.packing = cigParam->packing;
    hciParam.framing = cigParam->framing;
    IsoWriteUint16(hciParam.maxTransportLatencyMToS, cigParam->maxTransportLatencyMToS);
    IsoWriteUint16(hciParam.maxTransportLatencySToM, cigParam->maxTransportLatencySToM);
    hciParam.cisCount = cisCount;
    HciLeCisConfigParam *cisConfigArray = MEM_MALLOC.alloc(cisCount * sizeof(HciLeCisConfigParam));
    if (cisConfigArray == NULL) {
        return BT_NO_MEMORY;
    }
    (void)memset_s(
        cisConfigArray, cisCount * sizeof(HciLeCisConfigParam), 0x00, cisCount * sizeof(HciLeCisConfigParam));
    for (uint8_t i = 0; i < cisCount; i++) {
        cisConfigArray[i].cisId = cisParams[i].cisId;
        cisConfigArray[i].maxSduMToS = cisParams[i].maxSduMToS;
        cisConfigArray[i].maxSduSToM = cisParams[i].maxSduSToM;
        cisConfigArray[i].phyMToS = cisParams[i].phyMToS;
        cisConfigArray[i].phySToM = cisParams[i].phySToM;
        cisConfigArray[i].rtnMToS = cisParams[i].rtnMToS;
        cisConfigArray[i].rtnSToM = cisParams[i].rtnSToM;
    }
    hciParam.cisConfig = cisConfigArray;

    int ret = HCI_LeSetCigParameters(&hciParam);
    MEM_MALLOC.free(cisConfigArray);
    return ret;
}

int IsoLeCreateCis(uint8_t cisCount, const IsoLeCreateCisParam *params)
{
    LOG_INFO("%{public}s: cisCount:%hhu", __FUNCTION__, cisCount);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }
    if (cisCount == 0 || cisCount > ISO_LE_CIS_COUNT_MAX || params == NULL) {
        return BT_BAD_PARAM;
    }

    HciLeCreateCisParam hciParam = { 0 };
    hciParam.cisCount = cisCount;
    HciLeCreateCisConfigParam *cisConfigArray = MEM_MALLOC.alloc(cisCount * sizeof(HciLeCreateCisConfigParam));
    if (cisConfigArray == NULL) {
        return BT_NO_MEMORY;
    }
    (void)memset_s(cisConfigArray, cisCount * sizeof(HciLeCreateCisConfigParam), 0x00,
        cisCount * sizeof(HciLeCreateCisConfigParam));
    for (uint8_t i = 0; i < cisCount; i++) {
        cisConfigArray[i].cisHandle = params[i].cisHandle;
        cisConfigArray[i].aclHandle = params[i].aclHandle;
    }
    hciParam.cisConfig = cisConfigArray;

    int ret = HCI_LeCreateCis(&hciParam);
    MEM_MALLOC.free(cisConfigArray);
    return ret;
}

// HCI command timeout of the hci layer (CMD_TIMEOUT, hci_cmd.c): when the Controller never
// answers an LE Remove CIG, no Complete event arrives to clear removePending.valid and every
// later ISOIF_LeRemoveCig would return BT_ALREADY forever. A watchdog of the same length
// resets the pending slot, so the caller can retry.
#define ISO_LE_REMOVE_CIG_TIMEOUT_MS (10 * 1000)

// Alarm parameter packing: the generation (sequence) is shifted left by the width of the
// carried identifier and OR-ed with the identifier itself, so the timeout task can resolve
// the pending slot by number (8 bits for CIG_ID, 16 bits for CIS Connection Handle).
#define ISO_LE_ALARM_CIG_SEQ_SHIFT (8)
#define ISO_LE_ALARM_HANDLE_SEQ_SHIFT (16)

static void IsoRemoveCigTimeoutProcess(void *ctx)
{
    IsoLeMng *mng = IsoGetMng();
    // The alarm carries the generation (sequence) and the cigId of the remove that was
    // in flight when the watchdog was armed, packed as ((sequence << 8) | cigId). A
    // stale timeout task of a previous remove must not clear the pending slot of the
    // current remove: it is only cleared when valid && cigId match && sequence match.
    uintptr_t param = (uintptr_t)ctx;
    uint8_t cigId = (uint8_t)(param & 0xFF);
    uint16_t sequence = (uint16_t)((param >> ISO_LE_ALARM_CIG_SEQ_SHIFT) & 0xFFFF);
    if (mng->removePending.valid && mng->removePending.cigId == cigId &&
        mng->removePending.sequence == sequence) {
        HILOGW("%{public}s: LE Remove CIG timed out, reset pending state", __FUNCTION__);
        mng->removePending.valid = false;
    }
}

// Alarm thread: never touch queue state here, hand the reset to the ISO queue, the same thread
// that clears removePending.valid on the Complete event. The sequence and the cigId are passed
// through as the alarm parameter and resolved by number in the task (numeric-handle pattern).
static void IsoRemoveCigTimeoutCallback(void *parameter)
{
    BTM_RunTaskInProcessingQueue(PROCESSING_QUEUE_ID_ISO, IsoRemoveCigTimeoutProcess, parameter);
}

int IsoLeRemoveCig(uint8_t cigId)
{
    LOG_INFO("%{public}s: cigId:0x%02x", __FUNCTION__, cigId);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }

    IsoLeMng *mng = IsoGetMng();
    if (mng->removePending.valid) {
        HILOGW("%{public}s: remove in progress, reject cigId:0x%02x", __FUNCTION__, cigId);
        return BT_ALREADY;
    }

    // Arm the watchdog before sending; a Controller that never answers leaves it to fire
    // and reset the pending slot (see IsoRemoveCigTimeoutProcess). The timer is reused
    // across generations (created once at init): reuse is safe because the Complete
    // handler never cancels it (IsoLeRemoveCigComplete) and the timeout task resolves
    // the slot only on a valid && cigId && sequence match. A new generation is assigned
    // per issue: the watchdog parameter carries it next to the cigId, so a stale timeout
    // task of a previous remove of the same CIG_ID (e.g. its Complete arrived after a
    // re-issue) cannot clear the pending slot of the current remove.
    uint16_t sequence = (uint16_t)(mng->removePending.sequence + 1);
    mng->removePending.sequence = sequence;
    if (mng->removePending.timer != NULL) {
        if (AlarmSet(mng->removePending.timer, ISO_LE_REMOVE_CIG_TIMEOUT_MS, IsoRemoveCigTimeoutCallback,
            (void *)(uintptr_t)(((uint32_t)sequence << ISO_LE_ALARM_CIG_SEQ_SHIFT) | cigId)) != 0) {
            // The watchdog could not be armed: without it a Controller that never answers
            // would leave the pending slot armed forever and block every later remove with
            // BT_ALREADY, so reject the request instead of sending the command.
            HILOGE("%{public}s: arm watchdog failed, cigId:0x%02x", __FUNCTION__, cigId);
            AlarmCancel(mng->removePending.timer);
            mng->removePending.valid = false;
            return BT_OPERATION_FAILED;
        }
    }

    HciLeRemoveCigParam param = {
        .cigId = cigId,
    };
    mng->removePending.cigId = cigId;
    mng->removePending.valid = true;
    int ret = HCI_LeRemoveCig(&param);
    if (ret != BT_SUCCESS) {
        mng->removePending.valid = false;
        if (mng->removePending.timer != NULL) {
            AlarmCancel(mng->removePending.timer);
        }
    }
    return ret;
}

// Watchdog of an outstanding LE Accept CIS request. The Controller answers Accept
// CIS either with an error Command Complete (HciEventOnLeAcceptCisRequestCommandComplete
// is a deliberate no-op, so nothing is reported) or, on success, with the LE CIS
// Established event; a Controller that never answers (or a command rejected without
// an event) would leave the upper layer's cisEstablished callback pending forever.
// A watchdog of the same length as the HCI command timeout (CMD_TIMEOUT, hci_cmd.c)
// synthesizes a failure notification, so the caller is never left hanging (i6).
#define ISO_LE_ACCEPT_CIS_TIMEOUT_MS (10 * 1000)

static void IsoAcceptCisTimeoutProcess(void *ctx)
{
    IsoLeMng *mng = IsoGetMng();
    // The alarm carries the generation (sequence) and the CIS Connection Handle of the
    // accepted request, packed as ((sequence << 16) | cisHandle). A stale timeout task
    // of a previous accept of the same handle resolves the entry only when valid &&
    // handle match && sequence match.
    uintptr_t param = (uintptr_t)ctx;
    uint16_t cisHandle = (uint16_t)(param & 0xFFFF);
    uint16_t sequence = (uint16_t)((param >> ISO_LE_ALARM_HANDLE_SEQ_SHIFT) & 0xFFFF);
    for (uint8_t i = 0; i < ISO_LE_CIS_COUNT_MAX; i++) {
        if (mng->acceptCisPendings[i].valid && mng->acceptCisPendings[i].cisHandle == cisHandle &&
            mng->acceptCisPendings[i].sequence == sequence) {
            if (mng->acceptCisPendings[i].received) {
                // The Established answer already arrived (IsoLeCisEstablishedEvent only
                // marks the entry received): this timeout is the trailing cleanup of the
                // entry, nothing to synthesize. The alarm has already fired; the expired
                // timer stays attached to the entry for reuse by a later request.
                mng->acceptCisPendings[i].valid = false;
                mng->acceptCisPendings[i].received = false;
                return;
            }
            mng->acceptCisPendings[i].valid = false;
            // The alarm has already fired; the expired timer stays attached to the
            // entry for reuse by a later request. The synthesize only when the module
            // is still enabled: a timeout task drained during finalize must not report
            // to a shutting-down upper layer.
            HILOGW("%{public}s: LE Accept CIS timed out, notify failure, cisHandle:0x%04x", __FUNCTION__, cisHandle);
            if (mng->isEnable && mng->callback != NULL && mng->callback->cisEstablished != NULL) {
                IsoLeCisEstablishedInfo info = { 0 };
                info.cisHandle = cisHandle;
                mng->callback->cisEstablished(HCI_CONNECTION_ACCEPT_TIMEOUT_EXCEEDED, &info, mng->callbackContext);
            }
            return;
        }
    }
}

// Alarm thread: never touch queue state here, hand the reset to the ISO queue, the same
// thread that resolves the pending entry on the LE CIS Established event. The cisHandle
// is passed through as the alarm parameter (numeric-handle pattern).
static void IsoAcceptCisTimeoutCallback(void *parameter)
{
    BTM_RunTaskInProcessingQueue(PROCESSING_QUEUE_ID_ISO, IsoAcceptCisTimeoutProcess, parameter);
}

int IsoLeAcceptCisRequest(uint16_t cisHandle)
{
    LOG_INFO("%{public}s: cisHandle:0x%04x", __FUNCTION__, cisHandle);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }

    // Find the watchdog entry: reuse an entry already pending for this handle (a
    // duplicate Accept CIS, e.g. a retry) and re-arm it, otherwise take the first
    // free slot. The watchdog is only the timeout fallback, the LE CIS Established
    // event remains the primary answer.
    IsoLeMng *mng = IsoGetMng();
    IsoAcceptCisPending *pending = NULL;
    for (uint8_t i = 0; i < ISO_LE_CIS_COUNT_MAX; i++) {
        if (mng->acceptCisPendings[i].valid && mng->acceptCisPendings[i].cisHandle == cisHandle) {
            pending = &mng->acceptCisPendings[i];
            break;
        }
    }
    if (pending == NULL) {
        for (uint8_t i = 0; i < ISO_LE_CIS_COUNT_MAX; i++) {
            if (!mng->acceptCisPendings[i].valid) {
                pending = &mng->acceptCisPendings[i];
                break;
            }
        }
        if (pending == NULL) {
            // All watchdog slots in use (more than ISO_LE_CIS_COUNT_MAX outstanding
            // accepts): send the command anyway, degraded without the timeout fallback.
            HILOGW("%{public}s: no free watchdog slot, cisHandle:0x%04x", __FUNCTION__, cisHandle);
        }
    }
    if (pending != NULL) {
        // A new generation per arm: the watchdog parameter carries it next to the
        // handle, so a stale timeout task of a previous accept of the same handle
        // cannot resolve this entry (see IsoAcceptCisTimeoutProcess). The received
        // flag of the previous generation is cleared as well: the new generation
        // awaits its own Established answer (see IsoLeCisEstablishedEvent).
        uint16_t sequence = (uint16_t)(pending->sequence + 1);
        pending->sequence = sequence;
        pending->received = false;
        if (pending->timer == NULL) {
            pending->timer = AlarmCreate("isoAcceptCis", false);
        }
        if (pending->timer != NULL) {
            if (AlarmSet(pending->timer, ISO_LE_ACCEPT_CIS_TIMEOUT_MS, IsoAcceptCisTimeoutCallback,
                (void *)(uintptr_t)(((uint32_t)sequence << ISO_LE_ALARM_HANDLE_SEQ_SHIFT) | cisHandle)) != 0) {
                // The watchdog could not be armed: without the timeout fallback a lost
                // answer would leave the upper layer's cisEstablished callback pending
                // forever, so reject the request instead of sending the command. The
                // slot stays free for a retry.
                HILOGE("%{public}s: arm watchdog failed, cisHandle:0x%04x", __FUNCTION__, cisHandle);
                pending->valid = false;
                return BT_OPERATION_FAILED;
            }
        }
        pending->cisHandle = cisHandle;
        pending->valid = true;
    }

    HciLeAcceptCisRequestParam param = {
        .cisHandle = cisHandle,
    };
    return HCI_LeAcceptCisRequest(&param);
}

int IsoLeRejectCisRequest(uint16_t cisHandle, uint8_t reason)
{
    LOG_INFO("%{public}s: cisHandle:0x%04x, reason:0x%02x", __FUNCTION__, cisHandle, reason);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }

    HciLeRejectCisRequestParam param = {
        .cisHandle = cisHandle,
        .reason = reason,
    };
    return HCI_LeRejectCisRequest(&param);
}

int IsoLeDisconnectCis(uint16_t cisHandle, uint8_t reason)
{
    LOG_INFO("%{public}s: cisHandle:0x%04x, reason:0x%02x", __FUNCTION__, cisHandle, reason);
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }

    // BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
    // 7.1.6 Disconnect Command: "This command is used to terminate an existing connection".
    // There is no dedicated CIS disconnect command, the master terminates a CIS with the
    // generic HCI_Disconnect (see Vol 6, Part D, 6.25). The Controller is authoritative:
    // it returns Command Disallowed (0x0C) when the CIS is not established yet, so no
    // local handle pre-validation is performed here (same convention as Remove CIG).
    HciDisconnectParam param = {
        .connectionHandle = cisHandle,
        .reason = reason,
    };
    return HCI_Disconnect(&param);
}

// Whether the CIS handle at cigInfo->cisHandles[idx] is part of the new handle set of a
// rebuilt CIG (see IsoDropUnretainedCisEntries).
static bool IsoIsCisHandleRetained(const IsoCigInfo *cigInfo, uint8_t idx, uint8_t cisCount,
    const uint16_t *cisHandles)
{
    for (uint8_t j = 0; j < cisCount; j++) {
        if (cigInfo->cisHandles[idx] == cisHandles[j]) {
            return true;
        }
    }
    return false;
}

// Drop the tracking entries of CIS handles that are not part of the new set when a CIG
// is rebuilt on the same CIG_ID: such entries would survive in cisList, mis-report their
// Disconnection Complete and evade the Remove CIG cleanup (IsoRemoveCigCisEntries walks
// only the new handles). A zeroed slot (IsoClearCisHandleInCig) matches nothing.
static void IsoDropUnretainedCisEntries(IsoLeMng *mng, IsoCigInfo *cigInfo, uint8_t cisCount,
    const uint16_t *cisHandles)
{
    for (uint8_t i = 0; i < cigInfo->cisCount; i++) {
        if (IsoIsCisHandleRetained(cigInfo, i, cisCount, cisHandles)) {
            continue;
        }
        IsoCisInfo *cisInfo = IsoFindCis(mng, cigInfo->cisHandles[i]);
        if (cisInfo == NULL) {
            continue;
        }
        ListRemoveNode(mng->cisList, cisInfo);
    }
}

// Create the tracking entry of a CIG. On failure (allocation/insert error) the entry is
// not tracked: a later Disconnection Complete of this CIG's CIS is neither filtered nor
// reported to the upper layer (IsoLeDisconnectComplete). Errors are logged here.
static IsoCigInfo *IsoAllocCigEntry(IsoLeMng *mng, uint8_t cigId)
{
    IsoCigInfo *cigInfo = MEM_MALLOC.alloc(sizeof(IsoCigInfo));
    if (cigInfo == NULL) {
        HILOGE("%{public}s: alloc IsoCigInfo failed, cigId:0x%02x not tracked", __FUNCTION__, cigId);
        return NULL;
    }
    (void)memset_s(cigInfo, sizeof(IsoCigInfo), 0x00, sizeof(IsoCigInfo));
    cigInfo->cigId = cigId;
    if (!ListAddLast(mng->cigBlock.cigList, cigInfo)) {
        HILOGE("%{public}s: tracking insert failed, cigId:0x%02x not tracked", __FUNCTION__, cigId);
        MEM_MALLOC.free(cigInfo);
        return NULL;
    }
    return cigInfo;
}

// Rebuild the tracking entry of a CIG after a successful LE Set CIG Parameters: create
// the entry on a new CIG_ID, drop stale CIS tracking entries on a rebuild, then fill in
// the new handle set. Returns the entry, or NULL when tracking failed.
static IsoCigInfo *IsoSetCigTracking(IsoLeMng *mng, const HciLeSetCigParametersReturnParam *param, uint8_t cisCount)
{
    IsoCigInfo *cigInfo = IsoFindCig(mng, param->cigId);
    if (cigInfo != NULL) {
        IsoDropUnretainedCisEntries(mng, cigInfo, cisCount, param->cisHandles);
    } else {
        cigInfo = IsoAllocCigEntry(mng, param->cigId);
    }
    if (cigInfo != NULL) {
        cigInfo->cisCount = cisCount;
        (void)memcpy_s(
            cigInfo->cisHandles, sizeof(cigInfo->cisHandles), param->cisHandles, cisCount * sizeof(uint16_t));
    }
    return cigInfo;
}

void IsoLeSetCigParametersComplete(const HciLeSetCigParametersReturnParam *param)
{
    LOG_INFO("%{public}s: status:0x%02x, cigId:0x%02x, cisCount:%hhu", __FUNCTION__, param->status, param->cigId,
        param->cisCount);

    uint8_t cisCount = (param->cisCount > ISO_LE_CIS_COUNT_MAX) ? ISO_LE_CIS_COUNT_MAX : param->cisCount;
    if (param->cisCount > ISO_LE_CIS_COUNT_MAX) {
        HILOGE("%{public}s: invalid cisCount:%hhu, clamp to %hhu", __FUNCTION__, param->cisCount, cisCount);
    }

    IsoLeMng *mng = IsoGetMng();
    if (param->status == HCI_SUCCESS) {
        (void)IsoSetCigTracking(mng, param, cisCount);
    }

    if (mng->callback != NULL && mng->callback->createCigResult != NULL) {
        mng->callback->createCigResult(param->status, param->cigId, cisCount, param->cisHandles, mng->callbackContext);
    }
}

void IsoLeCreateCisComplete(const HciLeCreateCisReturnParam *param)
{
    LOG_INFO("%{public}s: status:0x%02x", __FUNCTION__, param->status);
    IsoLeMng *mng = IsoGetMng();
    if (mng->callback != NULL && mng->callback->createCisResult != NULL) {
        mng->callback->createCisResult(param->status, mng->callbackContext);
    }
}

/**
 * @brief drop the CIS tracking entries of a removed CIG.
 *
 * LE Remove CIG also terminates every CIS of the CIG, so their tracking entries are
 * dropped here to keep a later Disconnection Complete for them from being mis-reported.
 *
 * @param mng Indicates the ISO manager.
 * @param cigInfo Indicates the CIG being removed.
 */
static void IsoRemoveCigCisEntries(IsoLeMng *mng, IsoCigInfo *cigInfo)
{
    for (uint8_t i = 0; i < cigInfo->cisCount; i++) {
        IsoCisInfo *cisInfo = IsoFindCis(mng, cigInfo->cisHandles[i]);
        if (cisInfo != NULL) {
            ListRemoveNode(mng->cisList, cisInfo);
            // The CIG removal has torn the CIS down: drop the HCI handle
            // registration and the RX reassembly context alongside the entry.
            HciIsoDeregisterHandle(cigInfo->cisHandles[i]);
            IsoDataRemoveRxContext(cigInfo->cisHandles[i]);
        }
    }
}

void IsoLeRemoveCigComplete(const HciLeRemoveCigReturnParam *param)
{
    LOG_INFO("%{public}s: status:0x%02x, cigId:0x%02x", __FUNCTION__, param->status, param->cigId);
    IsoLeMng *mng = IsoGetMng();

    // A Complete whose CIG_ID does not match the pending remove is a stale
    // answer of a previous remove (e.g. it timed out and a new remove started):
    // it must not clear the pending slot, cancel the new watchdog, or touch the
    // new CIG's table entries. The result callback is still delivered below.
    //
    // The Command Complete payload carries no generation, so a stale answer of a
    // previous remove with the SAME CIG_ID (issued after a timeout) cannot be told
    // apart from the current remove's own answer here. In the FIFO model that
    // misattribution is benign (the Controller answers commands in order; both
    // answers refer to the same CIG), and it can no longer disarm the watchdog:
    // this handler never cancels the timer, so the current remove's watchdog stays
    // armed until it fires naturally. The generation-matched timeout task
    // (IsoRemoveCigTimeoutProcess) is the only one that resets the slot on
    // timeout; on a completed remove it fires against an already-reset slot
    // (valid == false) and is a no-op. The callback below is delivered for the
    // stale answer as well, so the upper layer may observe two removeCigResult
    // notifications with different statuses; it recovers by timing out the stale
    // remove itself and retrying (same upper-layer retry the watchdog exists for).
    if (mng->removePending.cigId != param->cigId) {
        HILOGW("%{public}s: stale Complete for cigId:0x%02x, pending cigId:0x%02x", __FUNCTION__, param->cigId,
            mng->removePending.cigId);
    } else {
        // Clean up on success even when the watchdog already reset the pending slot:
        // the Controller's answer may arrive after the timeout and the CIG must not
        // stay tracked forever (leak + later Disconnection events misreported).
        if (param->status == HCI_SUCCESS) {
            uint8_t cigId = mng->removePending.cigId;
            mng->removePending.valid = false;
            IsoCigInfo *cigInfo = IsoFindCig(mng, cigId);
            if (cigInfo != NULL) {
                IsoRemoveCigCisEntries(mng, cigInfo);
                ListRemoveNode(mng->cigBlock.cigList, cigInfo);
            }
        } else {
            mng->removePending.valid = false;
        }
    }

    if (mng->callback != NULL && mng->callback->removeCigResult != NULL) {
        mng->callback->removeCigResult(param->status, mng->callbackContext);
    }
}

void IsoLeRejectCisRequestComplete(const HciLeRejectCisRequestReturnParam *param)
{
    LOG_INFO("%{public}s: status:0x%02x", __FUNCTION__, param->status);
    IsoLeMng *mng = IsoGetMng();
    if (mng->callback != NULL && mng->callback->rejectCisResult != NULL) {
        mng->callback->rejectCisResult(param->status, mng->callbackContext);
    }
}

void IsoLeCisRequestEvent(const HciLeCisRequestEventParam *param)
{
    LOG_INFO("%{public}s: cisHandle:0x%04x, aclHandle:0x%04x, cigId:0x%02x, cisId:0x%02x", __FUNCTION__,
        param->cisHandle, param->aclHandle, param->cigId, param->cisId);
    IsoLeMng *mng = IsoGetMng();
    if (mng->callback != NULL && mng->callback->cisRequestInd != NULL) {
        mng->callback->cisRequestInd(
            param->cisHandle, param->aclHandle, param->cigId, param->cisId, mng->callbackContext);
    }
}

// Mark the accept-CIS watchdog entry of the handle as received. The LE CIS Established
// event is the primary answer to an LE Accept CIS request (any status, success or
// failure, resolves the outstanding accept, i6): the entry is only marked here, the
// timer is left armed and released by the generation-matched timeout task when it fires
// (IsoAcceptCisTimeoutProcess). The event payload carries no generation, so a late
// answer of a PREVIOUS accept of the same handle must not be able to cancel or delete
// the CURRENT generation's watchdog - it can only set the received flag.
static void IsoCisEstablishedMarkAccepted(IsoLeMng *mng, uint16_t connectionHandle)
{
    for (uint8_t i = 0; i < ISO_LE_CIS_COUNT_MAX; i++) {
        if (mng->acceptCisPendings[i].valid && mng->acceptCisPendings[i].cisHandle == connectionHandle) {
            mng->acceptCisPendings[i].received = true;
            break;
        }
    }
}

// Track every established CIS (the master and the slave both receive the 0x19 event),
// so the Disconnection Complete (0x05) can be filtered and the upper layer notified.
// Vol 6 Part B, 4.5.12 requires the notification irrespective of who initiated the
// termination. Returns false for a duplicate success of an already established handle:
// the caller then drops the event instead of delivering a second cisEstablished
// success to the upper layer.
static bool IsoCisEstablishedTrack(IsoLeMng *mng, const HciLeCisEstablishedEventParam *param)
{
    IsoCisInfo *cisInfo = IsoFindCis(mng, param->connectionHandle);
    if (cisInfo != NULL) {
        // A second LE CIS Established success for an already-established handle is a
        // duplicate (a retransmitted event, or a late one after the Create CIS command
        // timed out at the HCI layer, i5): the tracking entry already exists (only a
        // Disconnection Complete removes it), so drop the duplicate.
        HILOGW("%{public}s: duplicate CIS Established, drop, cisHandle:0x%04x", __FUNCTION__,
            param->connectionHandle);
        return false;
    }
    cisInfo = MEM_MALLOC.alloc(sizeof(IsoCisInfo));
    if (cisInfo == NULL) {
        // Not tracked: the Disconnection Complete (0x05) of this CIS is neither
        // filtered nor reported to the upper layer.
        HILOGE("%{public}s: alloc IsoCisInfo failed, cisHandle:0x%04x not tracked", __FUNCTION__,
            param->connectionHandle);
        return true;
    }
    (void)memset_s(cisInfo, sizeof(IsoCisInfo), 0x00, sizeof(IsoCisInfo));
    cisInfo->cisHandle = param->connectionHandle;
    if (!ListAddLast(mng->cisList, cisInfo)) {
        // Not tracked: the Disconnection Complete (0x05) of this CIS is neither
        // filtered nor reported to the upper layer.
        HILOGE("%{public}s: tracking insert failed, cisHandle:0x%04x not tracked", __FUNCTION__,
            param->connectionHandle);
        MEM_MALLOC.free(cisInfo);
    }
    return true;
}

void IsoLeCisEstablishedEvent(const HciLeCisEstablishedEventParam *param)
{
    LOG_INFO("%{public}s: status:0x%02x, cisHandle:0x%04x", __FUNCTION__, param->status, param->connectionHandle);
    IsoLeMng *mng = IsoGetMng();

    IsoCisEstablishedMarkAccepted(mng, param->connectionHandle);
    if (param->status == HCI_SUCCESS && !IsoCisEstablishedTrack(mng, param)) {
        return;
    }
    if (param->status == HCI_SUCCESS) {
        // Established CIS: register the handle for the HCI ISO data path
        // (Number of Completed Packets filtering / credit recovery).
        HciIsoRegisterHandle(param->connectionHandle);
    }

    IsoLeCisEstablishedInfo info = {
        .cisHandle = param->connectionHandle,
        .cigSyncDelay = IsoReadUint24(param->cigSyncDelay),
        .cisSyncDelay = IsoReadUint24(param->cisSyncDelay),
        .transportLatencyMToS = IsoReadUint24(param->transportLatencyMToS),
        .transportLatencySToM = IsoReadUint24(param->transportLatencySToM),
        .phyMToS = param->phyMToS,
        .phySToM = param->phySToM,
        .nse = param->nse,
        .bnMToS = param->bnMToS,
        .bnSToM = param->bnSToM,
        .ftMToS = param->ftMToS,
        .ftSToM = param->ftSToM,
        .maxPduMToS = param->maxPduMToS,
        .maxPduSToM = param->maxPduSToM,
        .isoInterval = param->isoInterval,
    };

    if (mng->callback != NULL && mng->callback->cisEstablished != NULL) {
        mng->callback->cisEstablished(param->status, &info, mng->callbackContext);
    }
}

// Clear the CIS Connection Handle slot from the CIG echo that owns it (master side).
// The handle may be reused to recreate a CIS once terminated (Vol 2, Part E, 7.1.6).
// The slot is zeroed to 0x0000 to invalidate it rather than left stale: a handle reused
// by a later CIS must not match this dead entry on a subsequent Remove CIG cleanup.
// Connection Handle 0x0000 is never assigned by the Controller, so a zeroed slot cannot
// collide with a real CIS.
static void IsoClearCisHandleInCig(IsoLeMng *mng, uint16_t cisHandle)
{
    ListNode *node = ListGetFirstNode(mng->cigBlock.cigList);
    while (node != NULL) {
        IsoCigInfo *cigInfo = ListGetNodeData(node);
        for (uint8_t i = 0; i < cigInfo->cisCount; i++) {
            if (cigInfo->cisHandles[i] == cisHandle) {
                cigInfo->cisHandles[i] = 0x0000;
                return;
            }
        }
        node = ListGetNextNode(node);
    }
}

void IsoLeDisconnectComplete(const HciDisconnectCompleteEventParam *param)
{
    LOG_INFO("%{public}s: status:0x%02x, connectionHandle:0x%04x, reason:0x%02x", __FUNCTION__, param->status,
        param->connectionHandle, param->reason);
    IsoLeMng *mng = IsoGetMng();

    // BLUETOOTH SPECIFICATION Version 5.2 | Vol 4, Part E
    // 7.7.5 Disconnection Complete Event: the event is broadcast for every logical
    // connection (ACL/SCO/CIS); only CIS handles tracked at establishment are reported
    // here, ACL/SCO disconnects stay with the BTM/ACL layer. The Note under 7.7.5
    // ("for a physical link failure, one event is sent for each logical channel")
    // resolves the ACL-disconnect cascade without extra handling here.
    IsoCisInfo *cisInfo = IsoFindCis(mng, param->connectionHandle);
    if (cisInfo == NULL) {
        return;
    }

    // A non-success status (Vol 2, Part E, 7.1.6: e.g. 0x0C Command Disallowed) means the
    // termination did not complete: the CIS may still exist at the Link Layer, so the tracking
    // entry and the CIG slot are kept for a later successful Disconnection Complete; only a
    // completed termination invalidates them.
    if (param->status == HCI_SUCCESS) {
        ListRemoveNode(mng->cisList, cisInfo);
        IsoClearCisHandleInCig(mng, param->connectionHandle);
        HciIsoDeregisterHandle(param->connectionHandle);
        // Drop the RX reassembly context for this CIS; its handle is gone for good.
        IsoDataRemoveRxContext(param->connectionHandle);
    }

    // BLUETOOTH SPECIFICATION Version 5.2 | Vol 6, Part B
    // 4.5.12: "The Host shall be notified when the termination procedure completes,
    // irrespective of whether the master or slave initiated it." The failure itself is
    // reported as well, so the app sees the termination did not finish.
    if (mng->callback != NULL && mng->callback->cisDisconnected != NULL) {
        mng->callback->cisDisconnected(param->status, param->connectionHandle, param->reason, mng->callbackContext);
    }
}
