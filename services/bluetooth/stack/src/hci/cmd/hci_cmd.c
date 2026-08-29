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

#include "hci_cmd.h"

#include <securec.h>
#include <time.h>

#include "btm/btm_thread.h"
#include "btstack.h"
#include "platform/include/alarm.h"
#include "platform/include/allocator.h"
#include "platform/include/bt_endian.h"
#include "platform/include/list.h"
#include "platform/include/mutex.h"
#include "platform/include/queue.h"

#include "hci/acl/hci_acl.h"
#include "hci/hci.h"
#include "hci/hci_def.h"
#include "hci/hci_error.h"
#include "hci/hci_failure.h"
#include "hci/hci_internal.h"

#include "hci_cmd_failure.h"
#include "log.h"

#define MAX_QUEUE_SIZE 32

#define CMD_TIMEOUT (10 * 1000)

// A timed-out command's late answer stops being absorbable after this age: once the window
// is gone the answer is presumed lost, and keeping the entry would only swallow the answer
// of a NEWER command with the same opcode (the self-sustaining poison case).
#define TIMED_OUT_CMD_MAX_AGE_MS (2 * CMD_TIMEOUT)

// Monotonic time conversion factors for HciGetMonotonicMs (CLOCK_MONOTONIC in seconds).
#define HCI_MS_PER_SEC (1000)
#define HCI_NS_PER_MS (1000 * 1000)

typedef struct {
    uint16_t opCode;
    uint64_t timestampMs;
} HciTimedOutCmd;

#pragma pack(1)
typedef struct {
    uint16_t opCode;
    uint8_t parameterTotalLength;
} HciCmdHeader;
#pragma pack()

static uint8_t g_numberOfHciCmd = 1;
static Mutex *g_lockNumberOfHciCmd = NULL;

// Monotonic source of HciCmd generations (see HciCmd.generation). Written atomically:
// HciAllocCmd may be called from any business thread.
static uint32_t g_cmdGeneration = 0;

static Queue *g_cmdCache = NULL;

static List *g_processingCmds = NULL;
static Mutex *g_lockProcessingCmds = NULL;

// Commands whose CMD_TIMEOUT fired but whose answer may still arrive later. The HCI layer
// dispatches Command_Complete/Status by opcode only (no pending-command identity), so a late
// answer of a timed-out command would otherwise complete a NEWER command with the same opcode
// (silent wrong result). Each matching late answer consumes one entry here instead
// (see HciCmdOnCommandComplete / HciCmdOnCommandStatus). Entries carry the opcode plus a
// timestamp so aged-out entries (TIMED_OUT_CMD_MAX_AGE_MS) are pruned instead of poisoning
// the answers of later commands forever. The HciCmd itself is freed when the timeout
// dispatches the failure.
static List *g_timedOutCmds = NULL;

// Set at the start of HciCloseCmd, reset by HciInitCmd; checked by the timeout path (the alarm
// callback and the task on the Stack thread) so a command alarm that fires while the command
// layer is tearing down neither posts a task behind the teardown nor touches the deleted
// lock/lists. Atomic because the alarm thread reads it concurrently with the close write.
static bool g_hciCmdClosing = false;

// Function declare
static void HciFreeCmd(void *cmd);
static void HciFreeTimedOutCmd(void *data);
static void HciAddTimedOutCmd(uint16_t opCode);

void HciInitCmd()
{
    __atomic_store_n(&g_hciCmdClosing, false, __ATOMIC_RELAXED);
    g_cmdCache = QueueCreate(MAX_QUEUE_SIZE);
    g_processingCmds = ListCreate(HciFreeCmd);
    g_timedOutCmds = ListCreate(HciFreeTimedOutCmd);

    g_numberOfHciCmd = 1;
    g_lockNumberOfHciCmd = MutexCreate();
    g_lockProcessingCmds = MutexCreate();
}

// Shutdown of the command layer. The lock is deleted before the lists so the caller must
// guarantee that no command completion / timeout task is still running (HCI_Close drains the
// RX task and shuts the upper modules down before reaching here); deleting the lock first is
// required because the list destructors below may free commands that reference it.
void HciCloseCmd()
{
    // Tear-down barrier for the command timeout path: from here on an alarm that fires must
    // neither post a timeout task (HciCmdOnCmdTimeout) nor run one against the deleted
    // lock/lists (HciCmdTimeoutTask). The commands still tracked here are freed by the
    // ListDelete below, which also cancels their alarms, so the dropped timeouts leak nothing.
    __atomic_store_n(&g_hciCmdClosing, true, __ATOMIC_RELAXED);

    if (g_lockProcessingCmds != NULL) {
        MutexDelete(g_lockProcessingCmds);
        g_lockProcessingCmds = NULL;
    }

    if (g_lockNumberOfHciCmd != NULL) {
        MutexDelete(g_lockNumberOfHciCmd);
        g_lockNumberOfHciCmd = NULL;
    }

    if (g_cmdCache != NULL) {
        QueueDelete(g_cmdCache, HciFreeCmd);
        g_cmdCache = NULL;
    }

    if (g_processingCmds != NULL) {
        ListDelete(g_processingCmds);
        g_processingCmds = NULL;
    }

    if (g_timedOutCmds != NULL) {
        ListDelete(g_timedOutCmds);
        g_timedOutCmds = NULL;
    }
}

static int HciCmdPushToTxQueue(HciCmd *cmd)
{
    int result = BT_SUCCESS;
    HciPacket *hciPacket = MEM_MALLOC.alloc(sizeof(HciPacket));
    if (hciPacket != NULL) {
        hciPacket->type = H2C_CMD;
        hciPacket->packet = cmd->packet;
        cmd->packet = NULL;
        HciPushToTxQueue(hciPacket);
    } else {
        result = BT_NO_MEMORY;
    }
    return result;
}

static void HciCmdTimeoutTask(void *context)
{
    HciCmd *pCmd = NULL;
    uint16_t opCode = 0;
    void *param = NULL;

    // The alarm carries the numeric generation of the command that timed out, never the
    // HciCmd pointer: a command freed by a late answer may be reallocated at the same
    // address (malloc reuse), which would make a stale timeout task match the NEWER
    // command, steal its param and record its opcode as timed out (ABA). The generation
    // is unique per allocation, so a stale task either finds no matching command or
    // cannot collide with a reallocated one.
    uint32_t generation = (uint32_t)(uintptr_t)context;

    if (__atomic_load_n(&g_hciCmdClosing, __ATOMIC_RELAXED)) {
        // HCI_Close is tearing the command layer down: a task posted before the close (the
        // alarm thread never posts one after it, see HciCmdOnCmdTimeout) must not touch the
        // deleted lock or lists. The command it would have timed out is freed by the
        // ListDelete in HciCloseCmd, so nothing leaks here.
        return;
    }

    MutexLock(g_lockProcessingCmds);
    ListNode *node = ListGetFirstNode(g_processingCmds);
    while (node != NULL) {
        pCmd = ListGetNodeData(node);
        if (pCmd->generation == generation) {
            opCode = pCmd->opCode;
            // steal the param and record the opcode before the command is freed: the late
            // answer (if any) must be recognizable in HciCmdOnCommandComplete/Status so it is
            // absorbed instead of completing a newer command with the same opcode
            param = pCmd->param;
            pCmd->param = NULL;
            AlarmCancel(pCmd->alarm);
            ListRemoveNode(g_processingCmds, pCmd);
            // A bogus 0x0000 opcode must neither pollute the absorb list nor skip the
            // dispatch below while leaking the stolen param: record the timeout only for a
            // real opcode, and the early return below frees the param on the guard path.
            if (opCode != 0) {
                HciAddTimedOutCmd(opCode);
            }
            break;
        } else {
            pCmd = NULL;
        }
        node = ListGetNextNode(node);
    }
    MutexUnlock(g_lockProcessingCmds);

    if (opCode == 0) {
        if (param != NULL) {
            MEM_MALLOC.free(param);
        }
        return;
    }

    if (param != NULL) {
        HciOnCmdFailed(opCode, HCI_TIMEOUT, param);
        MEM_MALLOC.free(param);
    }

    HciOnCmdTimeout();
}

static void HciCmdOnCmdTimeout(void *parameter)
{
    if (__atomic_load_n(&g_hciCmdClosing, __ATOMIC_RELAXED)) {
        // HCI_Close in progress: do not post a timeout task behind the teardown. The command
        // is freed by the ListDelete in HciCloseCmd (which cancels its alarm), and the task
        // itself re-checks the flag on the Stack thread as a second line of defense.
        return;
    }
    Thread *thread = BTM_GetProcessingThread();
    if (thread != NULL) {
        ThreadPostTask(thread, HciCmdTimeoutTask, parameter);
    }
}

// Pushes one cached command into the TX queue and tracks it in the processing list;
// frees @p cmd on any failure. The outstanding-command count is only consumed once the
// command is tracked: a tracking failure restores it, a push failure leaves it untouched
// (the caller's loop condition then consumes it again, same as the original flow).
static void HciCmdStartCachedCmd(HciCmd *cmd)
{
    if (HciCmdPushToTxQueue(cmd) != BT_SUCCESS) {
        HciFreeCmd(cmd);
        return;
    }
    g_numberOfHciCmd--;

    MutexLock(g_lockProcessingCmds);
    AlarmSet(cmd->alarm, CMD_TIMEOUT, HciCmdOnCmdTimeout, (void *)(uintptr_t)cmd->generation);
    if (!ListAddLast(g_processingCmds, cmd)) {
        // OOM: the command is in the TX queue but untracked - its answer would find no
        // pending match (dropped with a warning) and the caller would hang without a
        // failure dispatch. Roll back like the push failure (free the command, restore
        // the outstanding-command count).
        g_numberOfHciCmd++;
        HciFreeCmd(cmd);
    }
    MutexUnlock(g_lockProcessingCmds);
}

void HciSetNumberOfHciCmd(uint8_t numberOfHciCmd)
{
    MutexLock(g_lockNumberOfHciCmd);

    g_numberOfHciCmd = numberOfHciCmd;

    HciCmd *cmd = NULL;

    while (g_numberOfHciCmd > 0) {
        cmd = QueueTryDequeue(g_cmdCache);
        if (cmd == NULL) {
            // No more cmd
            break;
        }
        HciCmdStartCachedCmd(cmd);
    }

    MutexUnlock(g_lockNumberOfHciCmd);
}

static Packet *HciCreateCmdPacket(uint16_t opCode)
{
    Packet *packet = PacketMalloc(sizeof(HciCmdHeader), 0, 0);
    Buffer *headerBuffer = PacketHead(packet);
    HciCmdHeader *header = (HciCmdHeader *)BufferPtr(headerBuffer);
    if (header != NULL) {
        header->opCode = opCode;
        header->parameterTotalLength = 0;
    }

    return packet;
}

static Packet *HciCreateCmdPacketWithParam(uint16_t opCode, const void *param, uint8_t paramLength)
{
    Packet *packet = PacketMalloc(sizeof(HciCmdHeader), 0, paramLength);
    Buffer *headerBuffer = PacketHead(packet);
    HciCmdHeader *header = (HciCmdHeader *)BufferPtr(headerBuffer);
    if (header != NULL) {
        header->opCode = opCode;
        header->parameterTotalLength = paramLength;
    }
    PacketPayloadWrite(packet, param, 0, paramLength);
    return packet;
}

HciCmd *HciAllocCmd(uint16_t opCode, const void *param, size_t paramLength)
{
    HciCmd *cmd = MEM_MALLOC.alloc(sizeof(HciCmd));
    if (cmd != NULL) {
        cmd->opCode = opCode;
        cmd->generation = __atomic_fetch_add(&g_cmdGeneration, 1, __ATOMIC_RELAXED) + 1;
        if (param != NULL && paramLength > 0) {
            cmd->param = MEM_MALLOC.alloc(paramLength);
            if (cmd->param != NULL) {
                (void)memcpy_s(cmd->param, paramLength, param, paramLength);
            }
            cmd->packet = HciCreateCmdPacketWithParam(opCode, param, paramLength);
        } else {
            cmd->packet = HciCreateCmdPacket(opCode);
            cmd->param = NULL;
        }
        cmd->alarm = AlarmCreate(NULL, false);
    }
    return cmd;
}

static void HciFreeCmd(void *cmd)
{
    HciCmd *hciCmd = (HciCmd *)cmd;
    if (hciCmd != NULL) {
        if (hciCmd->alarm != NULL) {
            AlarmCancel(hciCmd->alarm);
            AlarmDelete(hciCmd->alarm);
            hciCmd->alarm = NULL;
        }
        if (hciCmd->param != NULL) {
            MEM_MALLOC.free(hciCmd->param);
            hciCmd->param = NULL;
        }
        if (hciCmd->packet != NULL) {
            PacketFree(hciCmd->packet);
            hciCmd->packet = NULL;
        }
        MEM_MALLOC.free(cmd);
    }
}

static uint64_t HciGetMonotonicMs(void)
{
    struct timespec ts;
    (void)clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * HCI_MS_PER_SEC + (uint64_t)ts.tv_nsec / HCI_NS_PER_MS;
}

static void HciFreeTimedOutCmd(void *data)
{
    MEM_MALLOC.free(data);
}

// Records that a command of @p opCode timed out so a late answer of it can be absorbed
// instead of completing a newer command with the same opcode. The timestamp enables
// HciAbsorbTimedOutResponse to prune entries whose window has expired.
static void HciAddTimedOutCmd(uint16_t opCode)
{
    HciTimedOutCmd *entry = MEM_MALLOC.alloc(sizeof(HciTimedOutCmd));
    if (entry == NULL) {
        // Without an entry the late answer would complete a newer command with the same
        // opcode - the exact hazard the entry exists to prevent. Nothing else to do here.
        LOG_WARN("HciAddTimedOutCmd: alloc entry failed, opcode 0x%04x not tracked", opCode);
        return;
    }
    entry->opCode = opCode;
    entry->timestampMs = HciGetMonotonicMs();
    if (!ListAddLast(g_timedOutCmds, entry)) {
        // OOM: without the entry the late answer would complete a newer command with the
        // same opcode - the exact hazard the entry exists to prevent (same degradation as
        // the alloc failure above). Free the entry instead of leaking it.
        LOG_WARN("HciAddTimedOutCmd: list insert failed, opcode 0x%04x not tracked", opCode);
        MEM_MALLOC.free(entry);
        return;
    }
}

// Ownership contract: HciSendCmd takes ownership of @p cmd regardless of whether
// it returns BT_SUCCESS or an error. The caller must not access or free @p cmd
// after this call.
int HciSendCmd(HciCmd *cmd)
{
    int result = BT_SUCCESS;
    bool push_failed = false;

    MutexLock(g_lockNumberOfHciCmd);

    if (g_numberOfHciCmd > 0) {
        result = HciCmdPushToTxQueue(cmd);
        if (result == BT_SUCCESS) {
            g_numberOfHciCmd--;

            MutexLock(g_lockProcessingCmds);
            AlarmSet(cmd->alarm, CMD_TIMEOUT, HciCmdOnCmdTimeout, (void *)(uintptr_t)cmd->generation);
            if (!ListAddLast(g_processingCmds, cmd)) {
                // OOM: the command is in the TX queue but untracked - its answer would find
                // no pending match (dropped with a warning) and the caller would hang without
                // a failure dispatch. Roll back like the push failure below (free the command,
                // the packet is owned by the TX queue, and restore the slot count).
                g_numberOfHciCmd++;
                push_failed = true;
            }
            MutexUnlock(g_lockProcessingCmds);
        } else {
            push_failed = true;
        }
    } else {
        QueueEnqueue(g_cmdCache, cmd);
    }

    MutexUnlock(g_lockNumberOfHciCmd);

    if (push_failed) {
        HciFreeCmd(cmd);
    }

    return result;
}

/*
 * Absorbs the answer of a command that already timed out: consumes one matching entry of
 * g_timedOutCmds and returns true, so the caller drops the answer instead of dispatching it.
 * Must be called with g_lockProcessingCmds held.
 *
 * Fallback identity rule: the HCI layer dispatches Command_Complete/Status by opcode only and
 * the completion payload does not carry a pending-command identity, so the timed-out command
 * cannot be told apart from a newer command of the same opcode. Trade-off (kept deliberately):
 * the answer is absorbed only when NO pending command of the same opcode is tracked - then it
 * can only be the timed-out command's own late answer. When a same-opcode command is pending,
 * the answer is presumed to belong to it and is dispatched; a genuinely delayed late answer
 * then completes the newer command with stale data (bounded, one-shot), but the alternative -
 * unconditionally swallowing the first same-opcode answer after a timeout - would also swallow
 * the newer command's legitimate answer, and because every swallowed answer re-adds a timeout
 * entry the poison would self-sustain indefinitely. Entries older than
 * TIMED_OUT_CMD_MAX_AGE_MS are pruned here so a lost answer cannot poison later commands
 * forever.
 */
static bool HciAbsorbTimedOutResponse(uint16_t opCode)
{
    uint64_t nowMs = HciGetMonotonicMs();

    // Prune entries whose absorb window has expired: their late answer is presumed lost and
    // must no longer be absorbed (it would be the answer of a newer command anyway).
    ListNode *node = ListGetFirstNode(g_timedOutCmds);
    while (node != NULL) {
        ListNode *next = ListGetNextNode(node);
        HciTimedOutCmd *entry = ListGetNodeData(node);
        if (entry != NULL && nowMs - entry->timestampMs > TIMED_OUT_CMD_MAX_AGE_MS) {
            ListRemoveNode(g_timedOutCmds, entry);
        }
        node = next;
    }

    // A pending command of the same opcode means the answer belongs to it: do not absorb.
    ListNode *pendingNode = ListGetFirstNode(g_processingCmds);
    while (pendingNode != NULL) {
        HciCmd *pCmd = ListGetNodeData(pendingNode);
        if (pCmd != NULL && pCmd->opCode == opCode) {
            return false;
        }
        pendingNode = ListGetNextNode(pendingNode);
    }

    node = ListGetFirstNode(g_timedOutCmds);
    while (node != NULL) {
        HciTimedOutCmd *entry = ListGetNodeData(node);
        if (entry != NULL && entry->opCode == opCode) {
            ListRemoveNode(g_timedOutCmds, entry);
            return true;
        }
        node = ListGetNextNode(node);
    }
    return false;
}

// false when the Status is the late answer of a timed-out command or matches no pending
// command: the caller (HciEventOnCommandStatusEvent) then skips the commandStatus fan-out,
// mirroring the Command_Complete interception (HciCmdOnCommandComplete)
bool HciCmdOnCommandStatus(uint16_t opCode, uint8_t status)
{
    HciCmd *cmd = NULL;

    MutexLock(g_lockProcessingCmds);

    if (HciAbsorbTimedOutResponse(opCode)) {
        MutexUnlock(g_lockProcessingCmds);
        return false;
    }

    ListNode *node = ListGetFirstNode(g_processingCmds);
    while (node != NULL) {
        cmd = ListGetNodeData(node);
        if (cmd != NULL) {
            if (opCode == cmd->opCode) {
                break;
            }
            cmd = NULL;
        }

        node = ListGetNextNode(node);
    }

    void *param = NULL;

    if (cmd != NULL) {
        AlarmCancel(cmd->alarm);

        param = cmd->param;
        cmd->param = NULL;

        ListRemoveNode(g_processingCmds, cmd);
    }

    MutexUnlock(g_lockProcessingCmds);

    if (cmd == NULL) {
        // No pending command of this opcode and nothing absorbed: the Status does not belong
        // to any tracked command (e.g. the late answer of a timed-out command whose entry was
        // already aged out). Drop it from the fan-out like the Command_Complete path does.
        LOG_WARN("HCI Command_Status opcode: 0x%04x has no pending command, dropped", opCode);
        return false;
    }

    if (opCode == HCI_DISCONNECT && status == HCI_SUCCESS) {
        HciDisconnectParam *discParam = (HciDisconnectParam *)param;
        HciAclOnDisconnectStatus(discParam->connectionHandle);
    }

    if (status != HCI_SUCCESS) {
        HciOnCmdFailed(opCode, status, param);
    }

    if (param != NULL) {
        MEM_MALLOC.free(param);
        param = NULL;
    }
    return true;
}

bool HciCmdOnCommandComplete(uint16_t opCode)
{
    HciCmd *cmd = NULL;

    MutexLock(g_lockProcessingCmds);

    if (HciAbsorbTimedOutResponse(opCode)) {
        MutexUnlock(g_lockProcessingCmds);
        return false;
    }

    ListNode *node = ListGetFirstNode(g_processingCmds);
    while (node != NULL) {
        cmd = ListGetNodeData(node);
        if (cmd != NULL) {
            if (opCode == cmd->opCode) {
                break;
            }
            cmd = NULL;
        }

        node = ListGetNextNode(node);
    }

    if (cmd != NULL) {
        AlarmCancel(cmd->alarm);
        ListRemoveNode(g_processingCmds, cmd);
    }

    MutexUnlock(g_lockProcessingCmds);
    if (cmd == NULL) {
        // No pending command of this opcode and nothing absorbed: the Complete does not
        // belong to any tracked command (e.g. the late answer of a timed-out command whose
        // entry was already aged out, or an unexpected event). Dropping it keeps a stale
        // answer from reaching the upper layers; every command sent through HciSendCmd is
        // tracked in g_processingCmds, so a legitimate answer always has a pending match.
        LOG_WARN("HCI Command_Complete opcode: 0x%04x has no pending command, dropped", opCode);
        return false;
    }
    return true;
}
