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

// BT 6.0 dual-board Channel Sounding interaction tests.

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <thread>

#include "btm.h"
#include "btstack.h"
#include "cs_defs.h"
#include "cs_le_if.h"
#include "gap_le_if.h"
#include "hci/hci.h"
#include "hci/hci_def_common.h"
#include "hci/hci_def_le_cmd.h"
#include "hci/hci_def_le_evt.h"
#include "hci/evt/hci_evt.h"
#include "hci/hci_def_evt.h"
#include "l2cap_def.h"
#include "l2cap_le_if.h"
#include "securec.h"
#include "stack_gap_le_6_0_interact_test.h"

using namespace testing::ext;

// BT 6.0 stack dual-board interaction tests: Channel Sounding cases (00600-00640).
// Split out of stack_gap_le_6_0_interact_test.cpp to keep the source file within
// the size limit; shares helper types, peer setup and the fixture through
// stack_gap_le_6_0_interact_test.h.

namespace OHOS {
namespace Bluetooth {

namespace {

// ---- BT 6.0 Channel Sounding dual-board case (00600) -------------------------------
// The peer runs the "cs-reflector" role of stack_gap_le_interact_test.cpp: it answers
// the initiator's pairing and owes its own Controller one 0x008D with Role_Enable bit 1
// (7.8.134 pp2816-2817), which is what gives that Controller a reflector role to take
// when Create_Context makes it create the mirror of this configuration (7.8.137 p2822).
// Every command below is the initiator Host's, in the order 5.1.26 (p3200) requires -
// CS Security Start, and both the Capability Exchange and the Configuration, have to be
// done before a procedure may be enabled - and the configuration, the procedure
// parameters and the tuning numbers come from a reference 6.0 initiator, i.e. values a
// real 6.0 Controller runs.
// The 0x0089 Command_Complete has no CSIF_ member: the local capability body is the
// engine's own cache rather than a session result, so the case captures that one at HCI
// level through its own additive callback table.

// 0x008D values (7.8.134 pp2816-2818). Role_Enable bit 0 is the initiator role, all
// roles starting disabled; 0xFE in CS_SYNC_Antenna_Selection repeats the antenna
// elements in order (0xFF would be no recommendation); Max_TX_Power is dBm EIRP over
// -127..20, and 20 is the ceiling the sample asks for.
constexpr uint8_t CS_DEFAULT_ROLE_ENABLE_INITIATOR = 0x01;
// Both roles at once, which is what 00640's two configurations need: Role_Enable bit 0 is the
// initiator role and bit 1 the reflector role (7.8.134 p2817), all roles starting disabled,
// and pp2816-2817 make a create for a role that was never enabled a 0x12 while disabling a
// role that already holds a configuration is the other 0x12 - so both bits go out once,
// ahead of the first create, and are never re-issued.
constexpr uint8_t CS_DEFAULT_ROLE_ENABLE_BOTH = 0x03;
constexpr uint8_t CS_DEFAULT_SYNC_ANTENNA_SELECTION_REPEAT = 0xFE;
constexpr int8_t CS_DEFAULT_MAX_TX_POWER_DBM = 20;

// 0x0090 values (7.8.137 pp2822-2827). Create_Context 0x01 has the peer's Controller
// create the mirror image of this configuration, which is what its 0x008D role enable
// is for; Role is this side's, so the steps come back in the initiator layout; RTT_Type
// 0x00 is the AA-only form, whose step data carries no sounding sequence.
constexpr uint8_t CS_CONFIG_ID_0 = 0x00;
constexpr uint8_t CS_CREATE_CONTEXT_LOCAL_AND_PEER = 0x01;
// 00640's second configuration and the two Config_IDs it drives the decode gate with: one
// the engine holds no slot for, one outside the 0x00-0x03 range the field has (7.8.137 p2824).
constexpr uint8_t CS_CONFIG_ID_1 = 0x01;
constexpr uint8_t CS_CONFIG_ID_2 = 0x02;
constexpr uint8_t CS_CONFIG_ID_OUT_OF_RANGE = 0x04;
// Create_Context 0x00 writes the configuration into the local Controller alone, with no
// Channel Sounding Configuration procedure against the peer (7.8.137 p2824); that creation
// still produces its 0x2F (p2827), which is what makes the slot addressable without the peer.
constexpr uint8_t CS_CREATE_CONTEXT_LOCAL_ONLY = 0x00;
constexpr uint8_t CS_MAIN_MODE_TYPE_2 = 0x02;
constexpr uint8_t CS_SUB_MODE_TYPE_1 = 0x01;
constexpr uint8_t CS_MAIN_MODE_STEPS_MIN = 2;
constexpr uint8_t CS_MAIN_MODE_STEPS_MAX = 10;
constexpr uint8_t CS_MAIN_MODE_REPETITION_0 = 0x00;
constexpr uint8_t CS_MODE0_STEPS_1 = 0x01;
constexpr uint8_t CS_RTT_TYPE_AA_ONLY = 0x00;
constexpr uint8_t CS_SYNC_PHY_LE_1M = 0x01;
constexpr uint8_t CS_CHANNEL_MAP_REPETITION_1 = 0x01;
constexpr uint8_t CS_CHANNEL_SELECTION_TYPE_3B = 0x00;
constexpr uint8_t CS_CH3C_SHAPE_HAT = 0x00;
constexpr uint8_t CS_CH3C_JUMP_MIN = 0x02;
// Channels 26..61 enabled, the map the sample builds after skipping 23/24/25, which the
// Channel_Map rule reserves along with 0, 1, 77 and 78 (7.8.137 p2826). Octet 3 holds
// channels 24-31 with 26-31 set and octet 7 holds 56-63 with 56-61 set.
constexpr uint8_t CS_CHANNEL_MAP_26_TO_61[CS_LE_CHANNEL_MAP_LEN] = {
    0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x00
};

// 0x0093 values (7.8.140 pp2832-2837). 50000 us is inside the 1250 us to 4 s subevent
// window (pp2834-2835) and large enough that the procedure runs out of unused channels
// inside its first subevent, so one report is expected rather than a stream of them;
// Max_Procedure_Count 0x0000 runs the procedure until it is disabled; Tx_Power_Delta
// 0x80 is no recommendation and SnR_Control 0xFF is not to be applied.
constexpr uint16_t CS_PROCEDURE_MAX_LEN = 0xFFFF;
constexpr uint16_t CS_PROCEDURE_INTERVAL_EVENTS = 100;
constexpr uint16_t CS_PROCEDURE_MAX_COUNT_UNTIL_DISABLED = 0x0000;
constexpr uint8_t CS_SUBEVENT_LEN_50000_US[3] = { 0x50, 0xC3, 0x00 };
constexpr uint8_t CS_TONE_ANTENNA_CONFIG_SELECTION_ACI_0 = 0x00;
constexpr uint8_t CS_PROCEDURE_PHY_LE_1M = 0x01;
constexpr int8_t CS_TX_POWER_DELTA_NONE = -128;  // wire 0x80
constexpr uint8_t CS_PREFERRED_PEER_ANTENNA_ANT1 = 0x01;
constexpr uint8_t CS_SNR_CONTROL_NOT_USED = 0xFF;

// 0x0094 (7.8.141 pp2838-2839) and the State the 0x30 reports back (7.7.65,43 p2442).
constexpr uint8_t CS_PROCEDURE_DISABLE = 0x00;
constexpr uint8_t CS_PROCEDURE_ENABLE = 0x01;
constexpr uint8_t CS_PROCEDURE_STATE_DISABLED = 0x00;
constexpr uint8_t CS_PROCEDURE_STATE_ENABLED = 0x01;
// 0x2F Action (7.7.65,42 p2435).
constexpr uint8_t CS_CONFIG_ACTION_REMOVED = 0x00;
constexpr uint8_t CS_CONFIG_ACTION_CREATED = 0x01;
// 0x2D Status 0x11 - Unsupported Feature or Parameter Value - is how a Controller
// answers for a peer that reports a zero FAE: an answer, not a failure (7.8.135 p2819,
// 5.1.29 p3208). The capability body the 0x2C carries has no No_FAE field to gate on -
// the bit belongs to the link layer's own CS capabilities (2.4.2,44 p2997) - so the
// Host cannot tell the two outcomes apart in advance and the Controller decides.
constexpr uint8_t CS_FAE_STATUS_NO_FAE = 0x11;

// Capability-body shapes any conformant Controller satisfies: Modes_Supported bit 0 is
// Mode-3 with every other bit reserved and Roles_Supported only bits 0 and 1
// (7.7.65,39 p2429); a step's Channel is a CS channel index 0x00-0x4E (7.7.65,44 p2451).
constexpr uint8_t CS_MODES_SUPPORTED_MAX = 0x01;
constexpr uint8_t CS_ROLES_SUPPORTED_MAX = 0x03;
constexpr uint8_t CS_ROLES_SUPPORTED_INITIATOR = 0x01;
constexpr uint8_t CS_ROLES_SUPPORTED_REFLECTOR = 0x02;
constexpr uint8_t CS_STEP_CHANNEL_MAX = 0x4E;

// The 0x0089 command is sent only once per stack lifetime and only while the local
// capability cache is empty, so the completion is observed rather than required: a
// miss means an earlier case in this process already filled the cache.
constexpr uint32_t CS_LOCAL_CAPS_CC_WINDOW_MS = 2000;

// The 0x0089 Command_Complete. It is reached through its own HCI callback table
// because the CSIF_ callback set has no member for it: the local capability body is the
// engine's cache, not a session result.
struct CsLocalCapsCapture : CallbackWaiter {
    uint8_t status = 0xFF;
    HciLeCsCapabilities caps = { };
    bool capsValid = false;

    void ResetAll()
    {
        Reset();
        status = 0xFF;
        capsValid = false;
    }
};

// 0x2C body. Status 0x00 is what caches the peer's capability body, so only that branch
// carries one and the handler keeps the rest zero.
struct CsRemoteCapsCapture : CallbackWaiter {
    uint8_t status = 0xFF;
    CsLeRemoteCapabilities caps = { };
    bool capsValid = false;

    void ResetAll()
    {
        Reset();
        status = 0xFF;
        capsValid = false;
    }
};

// 0x2D body. The table container is delivered on both branches, but its 72 octets carry
// no value unless Status 0x00 (7.7.65,40 pp2433-2434), so only status and the raw first
// octet are kept.
struct CsFaeCapture : CallbackWaiter {
    uint8_t status = 0xFF;
    bool tableDelivered = false;
    uint8_t firstOctet = 0;

    void ResetAll()
    {
        Reset();
        status = 0xFF;
        tableDelivered = false;
        firstOctet = 0;
    }
};

// 0x2E body (7.7.65,41 p2434).
struct CsSecurityEnableCapture : CallbackWaiter {
    uint8_t status = 0xFF;
    uint16_t connectionHandle = 0xFFFF;

    void ResetAll()
    {
        Reset();
        status = 0xFF;
        connectionHandle = 0xFFFF;
    }
};

// 0x008D Command_Complete (7.8.134 p2816). The defaults live in the Controller and the
// Host keeps no copy, so this completion is the only place its refusal can appear.
struct CsDefaultSettingsCapture : CallbackWaiter {
    uint8_t status = 0xFF;
    uint16_t connectionHandle = 0xFFFF;

    void ResetAll()
    {
        Reset();
        status = 0xFF;
        connectionHandle = 0xFFFF;
    }
};

// 0x2F body (7.7.65,42 p2437). One event closes both the 0x0090 create and the
// 0x0091 remove, and Action is the only field telling them apart; the case rearms this
// latch with ResetAll() between the two.
struct CsConfigCapture : CallbackWaiter {
    uint8_t status = 0xFF;
    CsLeConfig config = { };
    bool configValid = false;

    void ResetAll()
    {
        Reset();
        status = 0xFF;
        configValid = false;
    }
};

// The 0x0093 Command_Complete (7.8.140 pp2836-2837). Its return names only the
// connection handle - Config_ID is not echoed - so the case correlates it with its own
// request rather than reading a Config_ID back.
struct CsProcParamsCapture : CallbackWaiter {
    uint8_t status = 0xFF;
    uint16_t connectionHandle = 0xFFFF;

    void ResetAll()
    {
        Reset();
        status = 0xFF;
        connectionHandle = 0xFFFF;
    }
};

// 0x30 body (7.7.65,43 pp2442-2445). Rearmed between the enable and the disable; only
// State is read on the disable, because the spec ignores every parameter after a State
// of 0x00 (p2442).
struct CsProcedureEnableCapture : CallbackWaiter {
    uint8_t status = 0xFF;
    CsLeProcedureSchedule schedule = { };
    bool scheduleValid = false;

    void ResetAll()
    {
        Reset();
        status = 0xFF;
        scheduleValid = false;
    }
};

// The first reassembled subevent report. The report is the engine's own storage, reused
// for that connection's next subevent, so it is copied out here as the callback contract
// requires; the procedure keeps running until it is disabled, so later subevents only
// count.
struct CsResultCapture : CallbackWaiter {
    CsSubeventReport report = { };
    bool latched = false;
    uint32_t count = 0;

    void ResetAll()
    {
        Reset();
        latched = false;
        count = 0;
    }
};

// The GAP security request's completion. GAP calls it with the request's own result and
// the link's standing security; the first is GAP_STATUS_SUCCESS once the request was
// carried out, the second at least GAP_LE_UNAUTHENTICATED_ENCRYPTION once the link is
// encrypted, which 5.1.23 (p3195) makes the precondition of CS Security Start.
struct CsLinkSecurityCapture : CallbackWaiter {
    uint8_t result = 0xFF;
    GAP_LeSecurityStatus status = GAP_LE_NO_ENCRYPTION;

    void ResetAll()
    {
        Reset();
        result = 0xFF;
        status = GAP_LE_NO_ENCRYPTION;
    }
};

// The 0x33 LE CS Test End Complete (7.7.65,46 p2463). Status is the whole payload - the
// subevent carries nothing else - so the latch holds that and nothing more.
struct CsTestEndCapture : CallbackWaiter {
    uint8_t status = 0xFF;

    void ResetAll()
    {
        Reset();
        status = 0xFF;
    }
};

// The Command_Status fan-out (hci_evt_callbacks.h:532), recorded as a list of
// (opcode, status) pairs. It is the HCI layer's witness for what the Controller answered a
// command the Host refused locally would never show: the CS engine's own refusals call
// HciOnCmdFailed straight out of cs.c without sending anything, so a Command_Status for a
// CS opcode proves the command reached the wire. A list rather than one latch because one
// case issues several CS commands on top of the ACL connect and the encryption, and a
// predicate to wait on has to be able to name the opcode; that is also why it is not a
// CallbackWaiter. Being a list, it also counts: a case that knows how many times it issued
// a command can wait for that many entries, and one entry short is the difference between a
// refusal the Controller sent and one the Host never put on the wire.
constexpr size_t CS_CMD_STATUS_LOG_SIZE = 32;

struct CsCommandStatusCapture {
    std::mutex mtx;
    std::condition_variable cv;
    uint16_t opCodes[CS_CMD_STATUS_LOG_SIZE] = { };
    uint8_t statuses[CS_CMD_STATUS_LOG_SIZE] = { };
    size_t count = 0;
    size_t dropped = 0;

    void Record(uint16_t opCode, uint8_t status)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (count < CS_CMD_STATUS_LOG_SIZE) {
                opCodes[count] = opCode;
                statuses[count] = status;
                count++;
            } else {
                dropped++;
            }
        }
        cv.notify_all();
    }

    // Waits until an opcode has been sent at least `expected` times, rather than asserting on
    // the spot: the fan-out for a rejected command runs after the failure row that unblocks
    // the case's sink wait (hci_cmd.c:583, then the caller's fan-out), so a sink assertion
    // says nothing about what is recorded here yet. Returns false on timeout.
    bool WaitForOpCodeCount(uint16_t opCode, size_t expected, uint32_t timeoutMs = WAIT_CALLBACK_TIMEOUT_MS)
    {
        std::unique_lock<std::mutex> lock(mtx);
        return cv.wait_for(lock, std::chrono::milliseconds(timeoutMs), [this, opCode, expected] {
            return CountOpCodeLocked(opCode) >= expected;
        });
    }

    bool SawOpCodeWithStatus(uint16_t opCode, uint8_t status)
    {
        std::lock_guard<std::mutex> lock(mtx);
        for (size_t i = 0; i < count; i++) {
            if (opCodes[i] == opCode && statuses[i] == status) {
                return true;
            }
        }
        return false;
    }

    // How many times an opcode was sent. A case that knows how many times it issued a
    // command can use this to tell "the Controller refused it" from "the Host never sent it".
    size_t CountOpCode(uint16_t opCode)
    {
        std::lock_guard<std::mutex> lock(mtx);
        return CountOpCodeLocked(opCode);
    }

    void ResetAll()
    {
        std::lock_guard<std::mutex> lock(mtx);
        count = 0;
        dropped = 0;
    }

    // The case asserts this is zero: a full log would make the lookups above report a miss
    // for a pair that did arrive.
    size_t DroppedCount()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return dropped;
    }

private:
    size_t CountOpCodeLocked(uint16_t opCode) const
    {
        size_t seen = 0;
        for (size_t i = 0; i < count; i++) {
            if (opCodes[i] == opCode) {
                seen++;
            }
        }
        return seen;
    }
};

struct CsE2eContext {
    CsRemoteCapsCapture remoteCaps;
    CsFaeCapture fae;
    CsSecurityEnableCapture securityEnable;
    CsDefaultSettingsCapture defaults;
    CsConfigCapture config;
    CsProcParamsCapture procParams;
    CsProcedureEnableCapture procedureEnable;
    CsResultCapture result;
    CsLinkSecurityCapture linkSecurity;
};

// HCI_RegisterEventCallbacks stores the struct pointer; it must be static. Two members: the
// 0x0089 Command_Complete, which is the one CS result the CSIF_ callback set has no member
// for, and the Command_Status fan-out.
HciEventCallbacks g_csHciWatchCallbacks = { };
CsLocalCapsCapture g_csLocalCaps;
CsCommandStatusCapture g_csCommandStatus;
CsTestEndCapture g_csTestEnd;

void OnCsLocalCapsComplete(const HciLeCsReadLocalSupportedCapabilitiesReturnParam *returnParam)
{
    if (returnParam == nullptr) {
        return;
    }
    g_csLocalCaps.status = returnParam->status;
    g_csLocalCaps.caps = returnParam->capabilities;
    g_csLocalCaps.capsValid = true;
    g_csLocalCaps.Notify();
}

void OnCsCommandStatus(uint8_t status, uint16_t commandOpcode)
{
    g_csCommandStatus.Record(commandOpcode, status);
}

void OnCsRemoteCaps(uint8_t status, const CsLeRemoteCapabilities *caps, void *context)
{
    auto *ctx = static_cast<CsE2eContext *>(context);
    ctx->remoteCaps.status = status;
    if (caps != nullptr) {
        ctx->remoteCaps.caps = *caps;
        ctx->remoteCaps.capsValid = true;
    }
    ctx->remoteCaps.Notify();
}

void OnCsFaeTable(uint8_t status, const CsLeRemoteFaeTable *table, void *context)
{
    auto *ctx = static_cast<CsE2eContext *>(context);
    ctx->fae.status = status;
    if (table != nullptr) {
        // Kept as the raw wire octet: it is a signed 1/32 ppm value, printed as sent.
        ctx->fae.firstOctet = static_cast<uint8_t>(table->remoteFaeTable[0]);
        ctx->fae.tableDelivered = true;
    }
    ctx->fae.Notify();
}

void OnCsSecurityEnable(uint8_t status, uint16_t connectionHandle, void *context)
{
    auto *ctx = static_cast<CsE2eContext *>(context);
    ctx->securityEnable.status = status;
    ctx->securityEnable.connectionHandle = connectionHandle;
    ctx->securityEnable.Notify();
}

void OnCsDefaultSettings(uint8_t status, uint16_t connectionHandle, void *context)
{
    auto *ctx = static_cast<CsE2eContext *>(context);
    ctx->defaults.status = status;
    ctx->defaults.connectionHandle = connectionHandle;
    ctx->defaults.Notify();
}

void OnCsConfig(uint8_t status, const CsLeConfig *config, void *context)
{
    auto *ctx = static_cast<CsE2eContext *>(context);
    ctx->config.status = status;
    if (config != nullptr) {
        ctx->config.config = *config;
        ctx->config.configValid = true;
    }
    ctx->config.Notify();
}

void OnCsProcedureParameters(uint8_t status, uint16_t connectionHandle, void *context)
{
    auto *ctx = static_cast<CsE2eContext *>(context);
    ctx->procParams.status = status;
    ctx->procParams.connectionHandle = connectionHandle;
    ctx->procParams.Notify();
}

void OnCsProcedureEnable(uint8_t status, const CsLeProcedureSchedule *schedule, void *context)
{
    auto *ctx = static_cast<CsE2eContext *>(context);
    ctx->procedureEnable.status = status;
    if (schedule != nullptr) {
        ctx->procedureEnable.schedule = *schedule;
        ctx->procedureEnable.scheduleValid = true;
    }
    ctx->procedureEnable.Notify();
}

void OnCsSubeventResult(const CsSubeventReport *report, void *context)
{
    auto *ctx = static_cast<CsE2eContext *>(context);
    if (report == nullptr) {
        return;
    }
    ctx->result.count++;
    if (ctx->result.latched) {
        return;
    }
    ctx->result.report = *report;
    ctx->result.latched = true;
    ctx->result.Notify();
}

void OnCsLinkSecurityResult(const BtAddr *addr, uint8_t result, GAP_LeSecurityStatus status, void *context)
{
    (void)addr;
    auto *ctx = static_cast<CsE2eContext *>(context);
    ctx->linkSecurity.result = result;
    ctx->linkSecurity.status = status;
    ctx->linkSecurity.Notify();
}

// The CSIF_ callback table the case registers. CSIF_RegisterLeCsCallback copies the
// structure, so this file-scope instance only has to outlive the register call.
CsLeCallback g_csCallback = {
    .remoteCapabilitiesComplete = OnCsRemoteCaps,
    .remoteFaeTableComplete = OnCsFaeTable,
    .securityEnableComplete = OnCsSecurityEnable,
    .configComplete = OnCsConfig,
    .procedureEnableComplete = OnCsProcedureEnable,
    .procedureParametersComplete = OnCsProcedureParameters,
    .setDefaultSettingsComplete = OnCsDefaultSettings,
    .subeventResult = OnCsSubeventResult,
};

// Registers the CS session callback for the case and clears it on scope exit.
class CsCallbackGuard {
public:
    explicit CsCallbackGuard(CsE2eContext *ctx)
    {
        registered_ = CSIF_RegisterLeCsCallback(&g_csCallback, ctx) == BT_SUCCESS;
    }

    ~CsCallbackGuard()
    {
        if (registered_) {
            (void)CSIF_DeregisterLeCsCallback();
            registered_ = false;
        }
    }

    CsCallbackGuard(const CsCallbackGuard &) = delete;
    CsCallbackGuard &operator=(const CsCallbackGuard &) = delete;

    bool Registered() const
    {
        return registered_;
    }

private:
    bool registered_ = false;
};

// One counter per completion field of the CSIF callback table. Every capture beside this one
// latches: it holds the first completion of its kind and ResetAll() clears it, so a second
// arrival - the "both paths answered" failure - is invisible to it. A case that knows how many
// times it issued each command reads the difference here.
// The counters sit at the public surface, not at the HCI layer 02301 counts: what a duplicate
// would confuse is the consumer, and cs.c's own refusals reach this surface through
// HciOnCmdFailed without anything reaching the wire.
struct CsCallbackTally {
    std::atomic<uint32_t> remoteCaps = 0;
    std::atomic<uint32_t> remoteFae = 0;
    std::atomic<uint32_t> securityEnable = 0;
    std::atomic<uint32_t> config = 0;
    std::atomic<uint32_t> procedureEnable = 0;
    std::atomic<uint32_t> procedureParameters = 0;
    std::atomic<uint32_t> defaultSettings = 0;
    std::atomic<uint32_t> channelClassification = 0;
    std::atomic<uint32_t> subeventResult = 0;
    std::atomic<uint32_t> sessionClosed = 0;
    std::atomic<uint32_t> testComplete = 0;
    std::atomic<uint32_t> testEndComplete = 0;

    // Every completion of every field - the ones a case does not drive included, so a run that
    // lands the right field and a second, wrong one fails a case that reads this. subeventResult
    // is outside it: 0x31 is a procedure result a single enabled procedure reports repeatedly
    // (7.7.65,44 p2448), not a command's close, and counting it would make the total depend on
    // how long the procedure happened to run.
    uint32_t Total() const
    {
        return remoteCaps.load() + remoteFae.load() + securityEnable.load() + config.load() +
            procedureEnable.load() + procedureParameters.load() + defaultSettings.load() +
            channelClassification.load() + sessionClosed.load() + testComplete.load() +
            testEndComplete.load();
    }

    void ResetAll()
    {
        remoteCaps.store(0);
        remoteFae.store(0);
        securityEnable.store(0);
        config.store(0);
        procedureEnable.store(0);
        procedureParameters.store(0);
        defaultSettings.store(0);
        channelClassification.store(0);
        subeventResult.store(0);
        sessionClosed.store(0);
        testComplete.store(0);
        testEndComplete.store(0);
    }
};

CsCallbackTally g_csCallbackTally;

// The counting table is the capture table plus one counter per field, so a case that registers
// it keeps every CsE2eContext behaviour unchanged. Nothing here decides anything: counting and
// forwarding are the whole body.
void OnTallyRemoteCaps(uint8_t status, const CsLeRemoteCapabilities *caps, void *context)
{
    g_csCallbackTally.remoteCaps++;
    OnCsRemoteCaps(status, caps, context);
}

void OnTallyRemoteFae(uint8_t status, const CsLeRemoteFaeTable *table, void *context)
{
    g_csCallbackTally.remoteFae++;
    OnCsFaeTable(status, table, context);
}

void OnTallySecurityEnable(uint8_t status, uint16_t connectionHandle, void *context)
{
    g_csCallbackTally.securityEnable++;
    OnCsSecurityEnable(status, connectionHandle, context);
}

void OnTallyConfig(uint8_t status, const CsLeConfig *config, void *context)
{
    g_csCallbackTally.config++;
    OnCsConfig(status, config, context);
}

void OnTallyProcedureEnable(uint8_t status, const CsLeProcedureSchedule *schedule, void *context)
{
    g_csCallbackTally.procedureEnable++;
    OnCsProcedureEnable(status, schedule, context);
}

void OnTallyProcedureParameters(uint8_t status, uint16_t connectionHandle, void *context)
{
    g_csCallbackTally.procedureParameters++;
    OnCsProcedureParameters(status, connectionHandle, context);
}

void OnTallyDefaultSettings(uint8_t status, uint16_t connectionHandle, void *context)
{
    g_csCallbackTally.defaultSettings++;
    OnCsDefaultSettings(status, connectionHandle, context);
}

// 0x0092 and 0x0095 have no capture beside them: no case here drives them, and the counter is
// what turns a stray completion into a counted one instead of a silent one.
void OnTallyChannelClassification(uint8_t status, void *context)
{
    g_csCallbackTally.channelClassification++;
    (void)status;
    (void)context;
}

void OnTallySubeventResult(const CsSubeventReport *report, void *context)
{
    g_csCallbackTally.subeventResult++;
    OnCsSubeventResult(report, context);
}

void OnTallySessionClosed(uint16_t connectionHandle, void *context)
{
    g_csCallbackTally.sessionClosed++;
    (void)connectionHandle;
    (void)context;
}

void OnTallyTestComplete(uint8_t status, void *context)
{
    g_csCallbackTally.testComplete++;
    (void)status;
    (void)context;
}

// 0x33, the test's own close. Filling this member is also what makes CSIF_RegisterLeCsCallback
// wire the CS test sink (cs_if.c:418).
void OnTallyTestEndComplete(uint8_t status, void *context)
{
    g_csCallbackTally.testEndComplete++;
    g_csTestEnd.status = status;
    g_csTestEnd.Notify();
    (void)context;
}

// The counting callback table. Every member is filled, including the three no case drives: a
// NULL member is a silent drop, and "exactly one" cannot be asserted about a field nothing
// reports.
CsLeCallback g_csCallbackTallyTable = {
    .remoteCapabilitiesComplete = OnTallyRemoteCaps,
    .remoteFaeTableComplete = OnTallyRemoteFae,
    .securityEnableComplete = OnTallySecurityEnable,
    .configComplete = OnTallyConfig,
    .procedureEnableComplete = OnTallyProcedureEnable,
    .procedureParametersComplete = OnTallyProcedureParameters,
    .setDefaultSettingsComplete = OnTallyDefaultSettings,
    .setChannelClassificationComplete = OnTallyChannelClassification,
    .subeventResult = OnTallySubeventResult,
    .sessionClosed = OnTallySessionClosed,
    .testComplete = OnTallyTestComplete,
    .testEndComplete = OnTallyTestEndComplete,
};

// Registers the counting table for the case and deregisters it on scope exit. The counters are
// cleared on the way in rather than out, where the clear cannot race a callback still in flight
// from the case that just finished. Separate from CsCallbackGuard because
// CSIF_RegisterLeCsCallback owns one slot: a case registers one of the two, never both.
class CsCallbackTallyGuard {
public:
    explicit CsCallbackTallyGuard(CsE2eContext *ctx)
    {
        g_csCallbackTally.ResetAll();
        g_csTestEnd.ResetAll();
        registered_ = CSIF_RegisterLeCsCallback(&g_csCallbackTallyTable, ctx) == BT_SUCCESS;
    }

    ~CsCallbackTallyGuard()
    {
        if (registered_) {
            (void)CSIF_DeregisterLeCsCallback();
            registered_ = false;
        }
    }

    CsCallbackTallyGuard(const CsCallbackTallyGuard &) = delete;
    CsCallbackTallyGuard &operator=(const CsCallbackTallyGuard &) = delete;

    bool Registered() const
    {
        return registered_;
    }

private:
    bool registered_ = false;
};

// Registers the CS HCI observation table for the case and deregisters it on scope exit.
// HCI_RegisterEventCallbacks is additive (it appends to the event layer's list), so this
// table sits beside the file's read-features table and beside the CS engine's own one
// rather than replacing either.
class CsHciCallbackGuard {
public:
    CsHciCallbackGuard()
    {
        g_csHciWatchCallbacks = { };
        g_csHciWatchCallbacks.leCsReadLocalSupportedCapabilitiesComplete = OnCsLocalCapsComplete;
        g_csHciWatchCallbacks.commandStatus = OnCsCommandStatus;
        // The witness is per-case: an entry left by an earlier case would satisfy an
        // assertion made here about the Controller's answer to this case's command.
        g_csCommandStatus.ResetAll();
        registered_ = HCI_RegisterEventCallbacks(&g_csHciWatchCallbacks) == BT_SUCCESS;
    }

    ~CsHciCallbackGuard()
    {
        if (registered_) {
            if (HCI_DeregisterEventCallbacks(&g_csHciWatchCallbacks) != BT_SUCCESS) {
                printf("CsHciCallbackGuard: deregister failed; callback may remain registered\n");
            }
            registered_ = false;
        }
    }

    CsHciCallbackGuard(const CsHciCallbackGuard &) = delete;
    CsHciCallbackGuard &operator=(const CsHciCallbackGuard &) = delete;

    bool Registered() const
    {
        return registered_;
    }

private:
    bool registered_ = false;
};

// Starts the shared SMP auto-answer responder for the life of the case and joins it on
// scope exit. The link is encrypted before any CS command and this side is its Central,
// so the responder is what answers the pairing the encryption starts (5.1.23 p3195).
class CsPairResponderGuard {
public:
    CsPairResponderGuard()
    {
        InitPairResponder();
    }

    ~CsPairResponderGuard()
    {
        ShutdownPairResponder();
    }

    CsPairResponderGuard(const CsPairResponderGuard &) = delete;
    CsPairResponderGuard &operator=(const CsPairResponderGuard &) = delete;
};

enum CsDualGate {
    K_CS_READY,
    K_CS_NO_PEER_ADDR,
    K_CS_LOCAL_NO_CS,
    K_CS_CONNECT_FAILED,
    K_CS_READ_FAILED,
    K_CS_PEER_NO_CS,
};

const char *CsGateSkipMessage(CsDualGate gate)
{
    switch (gate) {
        case K_CS_READY:
            return "";
        case K_CS_NO_PEER_ADDR:
            return "peer address not set (BT60_PEER_ADDR)";
        case K_CS_LOCAL_NO_CS:
            return "local controller lacks LE feature bit 46 (Channel Sounding)";
        case K_CS_CONNECT_FAILED:
            return "direct connect to the peer failed";
        case K_CS_READ_FAILED:
            return "peer remote-features read failed";
        case K_CS_PEER_NO_CS:
            return "peer controller lacks LE feature bit 46 (Channel Sounding)";
        default:
            return "";
    }
}

// The peer's page-0 feature bits, read from whichever completion the connect-time read
// produced. Bit 46 sits in page 0, and both forms carry page 0 - the legacy 0x04 read
// reports page 0 alone and the 0x2B all-features read keeps page 0 as its first 8 octets
// (hci_def_common.h) - so the FSU gate's "page 1 unreadable" outcome has no counterpart
// here. Returns false when no successful completion arrived at all.
bool CsReadPeerPage0(uint8_t bitIndex, bool *set)
{
    if (g_readFeaturesCapture.AllFeaturesReceived()) {
        const auto param = g_readFeaturesCapture.AllParam();
        if (param.status != HCI_STATUS_SUCCESS) {
            return false;
        }
        *set = GetLinkLayerFeatureFlag(param.leFeatures.raw, bitIndex);
        return true;
    }
    if (g_readFeaturesCapture.LegacyReceived()) {
        const auto param = g_readFeaturesCapture.LegacyParam();
        if (param.status != HCI_STATUS_SUCCESS) {
            return false;
        }
        *set = GetLinkLayerFeatureFlag(param.leFeatures.raw, bitIndex);
        return true;
    }
    return false;
}

// Probe both boards for Channel Sounding and report the outcome as a skip gate. No
// ASSERT is used here: the case maps the enum onto a visible GTEST_SKIP instead.
CsDualGate CsProbePeerSupport()
{
    if (!g_peerAddrValid) {
        return K_CS_NO_PEER_ADDR;
    }
    if (!BTM_IsControllerSupportLeChannelSounding()) {
        return K_CS_LOCAL_NO_CS;
    }
    g_readFeaturesCapture.Reset();
    uint16_t connHandle = 0;
    if (!ConnectToPeer(connHandle)) {
        return K_CS_CONNECT_FAILED;
    }
    bool peerSupportsCs = false;
    const bool read = CsReadPeerPage0(LE_FEATURE_BIT_CHANNEL_SOUNDING, &peerSupportsCs);
    DisconnectFromPeer();
    if (!read) {
        return K_CS_READ_FAILED;
    }
    return peerSupportsCs ? K_CS_READY : K_CS_PEER_NO_CS;
}

// --- the 5.1.26 (p3200) prerequisite chain -----------------------------------------------
// 5.1.26 (p3200) names four prerequisites of a CS Start: a completed CS Security Start
// procedure, a completed capability exchange, a completed configuration, and a known
// mode-0 FAE table. 00600 runs the whole chain over a live peer; 00610 runs it too, after
// proving the Controller refuses a CS Security Start on a link that is not encrypted yet.
// Both cases call the steps here rather than carrying a copy each.
// The helpers are void and assert with ASSERT_*, which needs a void return, and they are
// called through ASSERT_NO_FATAL_FAILURE: a failing ASSERT_* would otherwise leave the
// helper and let the case run on, which is not what the inlined original did.

// 0x0089 + the local capability body. 0x0089 is the engine's own once-per-stack-lifetime
// read, so the completion is observed and the cache read below is the invariant.
void CsPrereqOpenSession(uint16_t connHandle)
{
    g_csLocalCaps.ResetAll();
    ASSERT_EQ(CSIF_LeCsEnableSession(connHandle), BT_SUCCESS) << "CS session open failed";
    if (g_csLocalCaps.Wait(CS_LOCAL_CAPS_CC_WINDOW_MS)) {
        EXPECT_EQ(g_csLocalCaps.status, HCI_STATUS_SUCCESS) << "the 0x0089 read reported a controller error";
        EXPECT_TRUE(g_csLocalCaps.capsValid) << "the 0x0089 completion carried no capability body";
    } else {
        printf("CS: no 0x0089 completion in %u ms (local capabilities already cached)\n",
            CS_LOCAL_CAPS_CC_WINDOW_MS);
    }
    CsLeCapabilities localCaps = { };
    ASSERT_EQ(CSIF_LeCsReadLocalCaps(&localCaps), BT_SUCCESS) << "no local capability body was cached";
    EXPECT_LE(localCaps.modesSupported, CS_MODES_SUPPORTED_MAX) <<
        "reserved Modes_Supported bits set in the local capability body";

    EXPECT_LE(localCaps.rolesSupported, CS_ROLES_SUPPORTED_MAX) <<
        "reserved Roles_Supported bits set in the local capability body";

    ASSERT_NE(localCaps.rolesSupported & CS_ROLES_SUPPORTED_INITIATOR, 0) <<
        "the local controller cannot take the initiator role of this configuration";
}

// 5.1.23 (p3195): a completed Encryption Start procedure is the precondition of CS Security
// Start, and this side is the Central of the pairing, so the SMP responder must be up
// before this runs (CsPairResponderGuard).
void CsPrereqEncryptLink(CsE2eContext *ctx)
{
    ctx->linkSecurity.ResetAll();
    ASSERT_EQ(GAPIF_LeRequestSecurity(&g_peerAddr, GAP_LE_UNAUTHENTICATED_ENCRYPTION,
        OnCsLinkSecurityResult, ctx), BT_SUCCESS) << "security request refused";
    ASSERT_TRUE(ctx->linkSecurity.Wait()) << "no result for the CS link's security request";
    ASSERT_EQ(ctx->linkSecurity.result, GAP_STATUS_SUCCESS) << "the security request failed";
    EXPECT_GE(ctx->linkSecurity.status, GAP_LE_UNAUTHENTICATED_ENCRYPTION) <<
        "the request completed with an unencrypted link";
}

// 0x008A, the Capability Exchange 5.1.26 (p3200) requires before a CS Start. The peer's
// roles are only known from the 0x2C, and the earliest point they are known is here, so a
// peer without the reflector role is reported back through *peerReflector rather than
// skipped: GTEST_SKIP inside a helper would leave only the helper.
void CsPrereqExchangeCapabilities(CsE2eContext *ctx, uint16_t connHandle, bool *peerReflector)
{
    ctx->remoteCaps.ResetAll();
    ASSERT_EQ(CSIF_LeCsReadRemoteCaps(connHandle), BT_SUCCESS) << "0x008A dispatch failed";
    ASSERT_TRUE(ctx->remoteCaps.Wait()) << "no 0x2C for the capability exchange";
    ASSERT_EQ(ctx->remoteCaps.status, HCI_STATUS_SUCCESS) << "the peer refused the capability exchange";
    ASSERT_TRUE(ctx->remoteCaps.capsValid) << "the 0x2C carried no capability body";
    EXPECT_EQ(ctx->remoteCaps.caps.connectionHandle, connHandle) << "the 0x2C named another link";
    EXPECT_LE(ctx->remoteCaps.caps.capabilities.rolesSupported, CS_ROLES_SUPPORTED_MAX) <<
        "reserved Roles_Supported bits set in the peer's capability body";

    *peerReflector =
        (ctx->remoteCaps.caps.capabilities.rolesSupported & CS_ROLES_SUPPORTED_REFLECTOR) != 0;
}

// 0x008D, the per-connection defaults a later 0x0090 is judged against (p2816).
void CsPrereqSetDefaultSettings(CsE2eContext *ctx, uint16_t connHandle)
{
    CsLeSetDefaultSettingsParam defaults = { };
    defaults.connectionHandle = connHandle;
    defaults.roleEnable = CS_DEFAULT_ROLE_ENABLE_INITIATOR;
    defaults.csSyncAntennaSelection = CS_DEFAULT_SYNC_ANTENNA_SELECTION_REPEAT;
    defaults.maxTxPower = CS_DEFAULT_MAX_TX_POWER_DBM;
    ctx->defaults.ResetAll();
    ASSERT_EQ(CSIF_LeCsSetDefaultSettings(&defaults), BT_SUCCESS) << "0x008D dispatch failed";
    ASSERT_TRUE(ctx->defaults.Wait()) << "no 0x008D Command_Complete";
    ASSERT_EQ(ctx->defaults.status, HCI_STATUS_SUCCESS) << "the Controller refused the default settings";
    EXPECT_EQ(ctx->defaults.connectionHandle, connHandle) << "the 0x008D named another link";
}

// 0x0090. Create_Context 0x01 is what turns this into two mirror-image configurations, one
// on each Controller, with the roles the two 0x008D enabled. The 0x2F is compared field by
// field against the request - the Channel_Map octets included - because the mirror is the
// only place the case can see what the Controller holds.
void CsPrereqCreateConfig(CsE2eContext *ctx, uint16_t connHandle)
{
    CsLeCreateConfigParam config = { };
    config.connectionHandle = connHandle;
    config.configId = CS_CONFIG_ID_0;
    config.createContext = CS_CREATE_CONTEXT_LOCAL_AND_PEER;
    config.mainModeType = CS_MAIN_MODE_TYPE_2;
    config.subModeType = CS_SUB_MODE_TYPE_1;
    config.minMainModeSteps = CS_MAIN_MODE_STEPS_MIN;
    config.maxMainModeSteps = CS_MAIN_MODE_STEPS_MAX;
    config.mainModeRepetition = CS_MAIN_MODE_REPETITION_0;
    config.mode0Steps = CS_MODE0_STEPS_1;
    config.role = CS_ROLE_INITIATOR;
    config.rttType = CS_RTT_TYPE_AA_ONLY;
    config.csSyncPhy = CS_SYNC_PHY_LE_1M;
    ASSERT_EQ(memcpy_s(config.channelMap, sizeof(config.channelMap),
        CS_CHANNEL_MAP_26_TO_61, sizeof(CS_CHANNEL_MAP_26_TO_61)), EOK);
    config.channelMapRepetition = CS_CHANNEL_MAP_REPETITION_1;
    config.channelSelectionType = CS_CHANNEL_SELECTION_TYPE_3B;
    config.ch3cShape = CS_CH3C_SHAPE_HAT;
    config.ch3cJump = CS_CH3C_JUMP_MIN;
    config.reserved = 0x00;
    ctx->config.ResetAll();
    ASSERT_EQ(CSIF_LeCsCreateConfig(&config), BT_SUCCESS) << "0x0090 dispatch failed";
    ASSERT_TRUE(ctx->config.Wait()) << "no 0x2F for the configuration create";
    ASSERT_EQ(ctx->config.status, HCI_STATUS_SUCCESS) << "the peer refused the configuration";
    ASSERT_TRUE(ctx->config.configValid) << "the 0x2F carried no configuration body";
    EXPECT_EQ(ctx->config.config.connectionHandle, connHandle) << "the 0x2F named another link";
    EXPECT_EQ(ctx->config.config.configId, CS_CONFIG_ID_0) << "the 0x2F named another Config_ID";
    EXPECT_EQ(ctx->config.config.action, CS_CONFIG_ACTION_CREATED) << "the 0x2F reports a remove";
    EXPECT_EQ(ctx->config.config.mainModeType, config.mainModeType) << "the mirror holds another Main_Mode_Type";
    EXPECT_EQ(ctx->config.config.subModeType, config.subModeType) << "the mirror holds another Sub_Mode_Type";
    EXPECT_EQ(ctx->config.config.minMainModeSteps, config.minMainModeSteps) <<
        "the mirror holds another lower step bound";

    EXPECT_EQ(ctx->config.config.maxMainModeSteps, config.maxMainModeSteps) <<
        "the mirror holds another upper step bound";

    EXPECT_EQ(ctx->config.config.mainModeRepetition, config.mainModeRepetition) <<
        "the mirror holds another repetition";

    EXPECT_EQ(ctx->config.config.mode0Steps, config.mode0Steps) << "the mirror holds another Mode_0_Steps";
    EXPECT_EQ(ctx->config.config.role, config.role) << "the mirror holds another Role";
    EXPECT_EQ(ctx->config.config.rttType, config.rttType) << "the mirror holds another RTT_Type";
    EXPECT_EQ(ctx->config.config.csSyncPhy, config.csSyncPhy) << "the mirror holds another CS_SYNC_PHY";
    EXPECT_EQ(memcmp(ctx->config.config.channelMap, config.channelMap, CS_LE_CHANNEL_MAP_LEN), 0) <<
        "the mirror holds another Channel_Map";

    EXPECT_EQ(ctx->config.config.channelMapRepetition, config.channelMapRepetition) <<
        "the mirror holds another map repetition";

    EXPECT_EQ(ctx->config.config.channelSelectionType, config.channelSelectionType) <<
        "the mirror holds another Channel_Selection_Type";
}

// 0x008C, the CS Security Start procedure. expectedStatus is the answer the caller has to
// see from the Controller: 0x00 over an encrypted link, 0x2F over one that is not encrypted
// (7.8.133 p2815). Status + Connection_Handle are the whole 0x2E payload on both branches
// (7.7.65,41 p2434), so nothing else is asserted here.
void CsPrereqSecurityEnable(CsE2eContext *ctx, uint16_t connHandle, uint8_t expectedStatus)
{
    ctx->securityEnable.ResetAll();
    ASSERT_EQ(CSIF_LeCsSecurityEnable(connHandle), BT_SUCCESS) << "0x008C dispatch failed";
    ASSERT_TRUE(ctx->securityEnable.Wait()) << "no 0x2E for the CS Security Start";
    ASSERT_EQ(ctx->securityEnable.status, expectedStatus) <<
        "the CS Security Start answered 0x" <<
        static_cast<int>(ctx->securityEnable.status) <<
        ", the case needs 0x" <<
        static_cast<int>(expectedStatus);

    EXPECT_EQ(ctx->securityEnable.connectionHandle, connHandle) << "the 0x2E named another link";
}

// 0x008E, the mode-0 FAE table 5.1.26 (p3200) requires known before a CS Start. 0x11 is the
// Controller answering for a peer with a zero FAE (5.1.29 p3208), which is a normal outcome
// and not a failure.
void CsPrereqReadRemoteFae(CsE2eContext *ctx, uint16_t connHandle)
{
    ctx->fae.ResetAll();
    ASSERT_EQ(CSIF_LeCsReadRemoteFae(connHandle), BT_SUCCESS) << "0x008E dispatch failed";
    ASSERT_TRUE(ctx->fae.Wait()) << "no 0x2D for the FAE table request";
    EXPECT_TRUE(ctx->fae.status == HCI_STATUS_SUCCESS || ctx->fae.status == CS_FAE_STATUS_NO_FAE) <<
        "the FAE table request failed, status=0x" <<
        static_cast<int>(ctx->fae.status);

    EXPECT_TRUE(ctx->fae.tableDelivered) << "the 0x2D delivered no table container";
    if (ctx->fae.status == HCI_STATUS_SUCCESS) {
        printf("CS: 0x2D carried the peer's mode-0 FAE table (first octet 0x%02X)\n", ctx->fae.firstOctet);
    } else {
        printf("CS: 0x2D status 0x%02X - the peer reports a zero FAE\n", ctx->fae.status);
    }
}

// 0x0093, the schedule a later 0x0094 enables. Its Command_Complete names only the handle,
// so the Config_ID the case later asserts is the one it asked for.
void CsPrereqSetProcedureParameters(CsE2eContext *ctx, uint16_t connHandle)
{
    CsLeSetProcedureParametersParam procParams = { };
    procParams.connectionHandle = connHandle;
    procParams.configId = CS_CONFIG_ID_0;
    procParams.maxProcedureLen = CS_PROCEDURE_MAX_LEN;
    procParams.minProcedureInterval = CS_PROCEDURE_INTERVAL_EVENTS;
    procParams.maxProcedureInterval = CS_PROCEDURE_INTERVAL_EVENTS;
    procParams.maxProcedureCount = CS_PROCEDURE_MAX_COUNT_UNTIL_DISABLED;
    ASSERT_EQ(memcpy_s(procParams.minSubeventLen, sizeof(procParams.minSubeventLen),
        CS_SUBEVENT_LEN_50000_US, sizeof(CS_SUBEVENT_LEN_50000_US)), EOK);
    ASSERT_EQ(memcpy_s(procParams.maxSubeventLen, sizeof(procParams.maxSubeventLen),
        CS_SUBEVENT_LEN_50000_US, sizeof(CS_SUBEVENT_LEN_50000_US)), EOK);
    procParams.toneAntennaConfigSelection = CS_TONE_ANTENNA_CONFIG_SELECTION_ACI_0;
    procParams.phy = CS_PROCEDURE_PHY_LE_1M;
    procParams.txPowerDelta = CS_TX_POWER_DELTA_NONE;
    procParams.preferredPeerAntenna = CS_PREFERRED_PEER_ANTENNA_ANT1;
    procParams.snrControlInitiator = CS_SNR_CONTROL_NOT_USED;
    procParams.snrControlReflector = CS_SNR_CONTROL_NOT_USED;
    ctx->procParams.ResetAll();
    ASSERT_EQ(CSIF_LeCsSetProcedureParameters(&procParams), BT_SUCCESS) << "0x0093 dispatch failed";
    ASSERT_TRUE(ctx->procParams.Wait()) << "no 0x0093 Command_Complete";
    ASSERT_EQ(ctx->procParams.status, HCI_STATUS_SUCCESS) << "the Controller refused the procedure parameters";
    EXPECT_EQ(ctx->procParams.connectionHandle, connHandle) << "the 0x0093 named another link";
}

// 0x0094. Asserts the Controller's answer and the body every 0x30 carries; State is left to
// the caller because it says where the procedure stands rather than what was asked for
// (7.7.65,43 p2442).
void CsRunProcedureEnable(CsE2eContext *ctx, uint16_t connHandle, uint8_t enable, uint8_t expectedStatus)
{
    CsLeProcedureEnableParam param = { };
    param.connectionHandle = connHandle;
    param.configId = CS_CONFIG_ID_0;
    param.enable = enable;
    ctx->procedureEnable.ResetAll();
    ASSERT_EQ(CSIF_LeCsProcedureEnable(&param), BT_SUCCESS) << "0x0094 dispatch failed";
    ASSERT_TRUE(ctx->procedureEnable.Wait()) << "no 0x30 for the procedure enable or disable";
    ASSERT_EQ(ctx->procedureEnable.status, expectedStatus) <<
        "the 0x30 answered 0x" <<
        static_cast<int>(ctx->procedureEnable.status) <<
        ", the case needs 0x" <<
        static_cast<int>(expectedStatus);

    ASSERT_TRUE(ctx->procedureEnable.scheduleValid) << "the 0x30 carried no schedule body";
    EXPECT_EQ(ctx->procedureEnable.schedule.connectionHandle, connHandle) << "the 0x30 named another link";
    EXPECT_EQ(ctx->procedureEnable.schedule.configId, CS_CONFIG_ID_0) << "the 0x30 named another Config_ID";
}

// 0x0091. The 0x2F carries Action, which is what tells a remove from a create; on a
// Controller rejection the Host synthesizes the same event with the action that was asked
// for, so the field is there to assert on both branches. A rejected remove leaves the
// engine's slot alone - CsOnConfigComplete writes it only on status 0x00 (cs.c) - so the
// configuration stays addressable, which is what 00610 goes on to check.
void CsRunRemoveConfig(CsE2eContext *ctx, uint16_t connHandle, uint8_t expectedStatus)
{
    CsLeRemoveConfigParam removeParam = { };
    removeParam.connectionHandle = connHandle;
    removeParam.configId = CS_CONFIG_ID_0;
    ctx->config.ResetAll();
    ASSERT_EQ(CSIF_LeCsRemoveConfig(&removeParam), BT_SUCCESS) << "0x0091 dispatch failed";
    ASSERT_TRUE(ctx->config.Wait()) << "no 0x2F for the configuration remove";
    ASSERT_EQ(ctx->config.status, expectedStatus) <<
        "the 0x2F answered 0x" <<
        static_cast<int>(ctx->config.status) <<
        ", the case needs 0x" <<
        static_cast<int>(expectedStatus);

    ASSERT_TRUE(ctx->config.configValid) << "the 0x2F carried no body";
    EXPECT_EQ(ctx->config.config.configId, CS_CONFIG_ID_0) << "the 0x2F named another Config_ID";
    EXPECT_EQ(ctx->config.config.action, CS_CONFIG_ACTION_REMOVED) << "the 0x2F reports a create";
}

// --- the 00640 injection layer -----------------------------------------------------------
// 00640 drives the connection channel's decode gate, and a Controller will not produce a
// subevent on demand, so the case puts subevent frames on the receive path itself: the wire
// a real one arrives on is [0x3E][Length][Subevent_Code][fixed head][step records] and
// HciOnEvent is the synchronous entry the HCI receive task calls, filtering on no event mask,
// so a hand-built frame reaches the CS engine exactly where a real one does (hci_evt.c:2037
// dispatches on the event code, hci_evt_le.c:1904 strips the subevent code and calls the
// handler). These are the interact file's own copies of the helpers stack_gap_le_6_0_test.cpp
// uses on the test channel: that file's InjectCsSubeventFrame and CsStepBlob have internal
// linkage and are not visible here, and the head this case needs is the connection channel's,
// whose handle and Config_ID are the two keys under test.

constexpr size_t CS_INJECT_WIRE_HEAD_LEN = 3;       // Event_Code, Length, Subevent_Code
constexpr size_t CS_INJECT_WIRE_PREFIX_LEN = 2;     // Event_Code + Length
constexpr size_t CS_INJECT_WIRE_EVENT_CODE_OFFSET = 0;
constexpr size_t CS_INJECT_WIRE_LENGTH_OFFSET = 1;
constexpr size_t CS_INJECT_WIRE_SUBEVENT_CODE_OFFSET = 2;
constexpr size_t CS_INJECT_HEAD_CAP = 15;           // the 0x31 fixed head, the larger of the two
constexpr size_t CS_INJECT_STEP_HEAD_LEN = 3;       // Step_Mode, Step_Channel, Step_Data_Length
constexpr size_t CS_INJECT_BLOB_CAP = 64;
// A mode-0 step's two role templates (7.7.65,44 p2452): the only place in the four modes where
// the role decides the length, so the length is what says which template the Controller sent.
constexpr size_t CS_MODE0_INITIATOR_LEN = 5;
constexpr size_t CS_MODE0_REFLECTOR_LEN = 3;
// The longest record this file appends is the step head plus the initiator's five-octet
// template, and it has to stay appendable: Append refuses a record that would not fit rather
// than truncating it, so a template grown past the cap would drop a step from the frame.
static_assert(CS_INJECT_STEP_HEAD_LEN + CS_MODE0_INITIATOR_LEN <= CS_INJECT_BLOB_CAP);
// The two mode-0 bodies this case injects, the values stack_gap_le_6_0_test.cpp's 02302 drives
// the same decode with: Packet_Quality, Packet_RSSI, Packet_Antenna, then - initiator only -
// the two-octet Measured_Freq_Offset. Read back they are quality 0x0A, RSSI -20, antenna 0x01
// and, on the initiator's template, a compensation of 10 in 0.01 ppm units.
constexpr uint8_t CS_MODE0_INITIATOR_STEP[CS_MODE0_INITIATOR_LEN] = { 0x0A, 0xEC, 0x01, 0x0A, 0x00 };
constexpr uint8_t CS_MODE0_REFLECTOR_STEP[CS_MODE0_REFLECTOR_LEN] = { 0x0A, 0xEC, 0x01 };
// The step Channel of an injected frame, inside the 0x00-0x4E CS channel index range
// (7.7.65,44 p2451). Distinct per drive so a report's steps[0] and steps[1] are tellable apart.
constexpr uint8_t CS_INJECT_CHANNEL_FIRST = 0x05;
constexpr uint8_t CS_INJECT_CHANNEL_SECOND = 0x06;
constexpr uint8_t CS_INJECT_CHANNEL_THIRD = 0x07;
constexpr uint8_t CS_INJECT_CHANNEL_FOURTH = 0x08;
// The 0x31 this case injects head values, the values 02302 gives the same fields: the two
// counters and the compensation pairwise distinct so a swap between them shows up, and a
// Reference_Power_Level of 0xF6, which is -10 dBm.
constexpr uint16_t CS_INJECT_ACL_EVENT_COUNTER = 0x1234;
constexpr uint16_t CS_INJECT_PROCEDURE_COUNTER = 0x0042;
constexpr uint16_t CS_INJECT_FREQ_COMPENSATION = 0x000A;
constexpr int8_t CS_INJECT_REFERENCE_POWER_DBM = -10;
constexpr uint8_t CS_INJECT_REFERENCE_POWER_WIRE = 0xF6;

// Command_Status counts the cases intentionally issue.
constexpr size_t CS_EXPECTED_SECURITY_ENABLE_CMD_STATUS = 2;
constexpr size_t CS_EXPECTED_REMOVE_CONFIG_CMD_STATUS = 2;
constexpr size_t CS_EXPECTED_PROCEDURE_ENABLE_CMD_STATUS = 3;
constexpr size_t CS_EXPECTED_PROCEDURE_ENABLE_PAIR_CMD_STATUS = 2;
constexpr size_t CS_EXPECTED_TEST_END_CMD_STATUS = 2;
constexpr size_t CS_EXPECTED_CREATE_CONFIG_CMD_STATUS = 2;

// Public-surface callback tallies expected in 00630.
constexpr uint32_t CS_EXPECTED_CONFIG_CALLBACK_COUNT = 2;
constexpr uint32_t CS_EXPECTED_PROCEDURE_ENABLE_CALLBACK_COUNT = 4;
constexpr uint32_t CS_EXPECTED_TEST_END_CALLBACK_COUNT = 2;
constexpr uint32_t CS_EXPECTED_TOTAL_CALLBACK_COUNT = 13;

// Decoded step field expectations shared with the single-board 02302 cases.
constexpr int16_t CS_MEASURED_FREQ_OFFSET_0_01PPM = 10;
constexpr int8_t CS_PACKET_RSSI_DBM = -20;

// Step-count expectations for continuation / report-count assertions.
constexpr uint8_t CS_NUM_STEPS_REPORTED_ONE = 0x01;
constexpr uint8_t CS_NUM_STEPS_TWO = 0x02;
constexpr uint8_t CS_SUBEVENT_DONE_STATUS_OK = 0x00;

// The step records of one frame, in the order the Controller sends them. An append that would
// not fit is refused rather than truncated, so an over-long drive shows up as a report with
// fewer steps instead of as a write past the buffer.
struct CsInjectBlob {
    uint8_t bytes[CS_INJECT_BLOB_CAP] = {};
    size_t used = 0;
    uint8_t count = 0;

    // One record: Step_Mode, Step_Channel, Step_Data_Length, then that step's own Step_Data
    // (7.7.65,44 p2451). count is the number of records appended, which is what the frame's
    // Num_Steps_Reported has to be: the engine walks exactly that many of them (cs_result.c:288).
    void Append(uint8_t mode, uint8_t channel, const uint8_t *data, size_t length)
    {
        if (used + CS_INJECT_STEP_HEAD_LEN + length > sizeof(bytes)) {
            return;
        }
        bytes[used++] = mode;
        bytes[used++] = channel;
        bytes[used++] = static_cast<uint8_t>(length);
        if (length != 0 && data != nullptr) {
            ASSERT_EQ(memcpy_s(bytes + used, sizeof(bytes) - used, data, length), EOK);
            used += length;
        }
        count++;
    }
};

// Wrap hand-built wire octets into an HCI event packet and hand them to the synchronous event
// entry (the same layout HciOnReceivedHciPacket builds before it dispatches).
// Injection runs on the calling (test) thread: HciOnEvent dispatches inline, and the
// bypassed g_hciRxQueue is drained by the real receive path on the BTM processing
// thread, so the two are not mutually excluded. Use this only in windows where no
// real traffic can reach the same engine state the injection drives.
void CsInjectWire(const uint8_t *wire, size_t size)
{
    Packet *packet = PacketMalloc(0, 0, size);
    ASSERT_NE(packet, nullptr) << "no packet buffer for the injected frame";
    PacketPayloadWrite(packet, wire, 0, size);
    HciOnEvent(packet);
    PacketFree(packet);
}

// One 0x31 or 0x32 frame on the wire. Length counts the Subevent_Code and everything behind
// it, which is what the dispatcher subtracts before the handler sees the body
// (hci_evt_le.c:1904), so the handler's step blob is exactly what was appended here. The
// packet carries the two header octets plus that body - the receive layer strips the header
// and then refuses anything whose remainder differs from Parameter_Total_Length
// (hci_evt.c:2032), so the cap-sized array itself would be dropped for its zero tail.
void CsInjectSubeventFrame(uint8_t subeventCode, const void *head, size_t headLen,
    const CsInjectBlob &steps)
{
    uint8_t wire[CS_INJECT_WIRE_HEAD_LEN + CS_INJECT_HEAD_CAP + CS_INJECT_BLOB_CAP] = {0};
    const size_t bodyLen = 1 + headLen + steps.used;
    // Event_Code and Length, then the body: the one length both the Length octet and the
    // packet handed over are built from.
    const size_t wireLen = CS_INJECT_WIRE_PREFIX_LEN + bodyLen;
    // Refuse rather than truncate: a body past 0xFF would wrap the Length octet, and one
    // past the array would leave CsInjectWire reading beyond wire[].
    if (bodyLen > 0xFF || wireLen > sizeof(wire)) {
        return;
    }
    wire[CS_INJECT_WIRE_EVENT_CODE_OFFSET] = HCI_LE_META_EVENT;   // Event_Code: LE Meta
    wire[CS_INJECT_WIRE_LENGTH_OFFSET] = static_cast<uint8_t>(bodyLen);
    wire[CS_INJECT_WIRE_SUBEVENT_CODE_OFFSET] = subeventCode;
    if (memcpy_s(wire + CS_INJECT_WIRE_HEAD_LEN, sizeof(wire) - CS_INJECT_WIRE_HEAD_LEN, head, headLen) !=
        EOK) {
        ADD_FAILURE() << "CsInjectSubeventFrame head memcpy_s failed";
        return;
    }
    if (memcpy_s(wire + CS_INJECT_WIRE_HEAD_LEN + headLen,
        sizeof(wire) - CS_INJECT_WIRE_HEAD_LEN - headLen, steps.bytes, steps.used) != EOK) {
        ADD_FAILURE() << "CsInjectSubeventFrame steps memcpy_s failed";
        return;
    }
    CsInjectWire(wire, wireLen);
}

// The constant part of a 0x31 head addressed at a handle and Config_ID of the caller's: the
// two keys the case varies. The done statuses and the step count are the caller's too, so a
// frame that closes its subevent and one that leaves it open share this builder.
HciLeCsSubeventResultEventParam CsInjectResultHead(uint16_t connectionHandle, uint8_t configId,
    uint8_t procedureDoneStatus, uint8_t subeventDoneStatus, uint8_t numStepsReported)
{
    HciLeCsSubeventResultEventParam head = {};
    head.connectionHandle = connectionHandle;
    head.configId = configId;
    head.startAclConnEventCounter = CS_INJECT_ACL_EVENT_COUNTER;
    head.procedureCounter = CS_INJECT_PROCEDURE_COUNTER;
    head.frequencyCompensation = CS_INJECT_FREQ_COMPENSATION;
    head.referencePowerLevel = CS_INJECT_REFERENCE_POWER_WIRE;
    head.procedureDoneStatus = procedureDoneStatus;
    head.subeventDoneStatus = subeventDoneStatus;
    head.abortReason = 0x00;
    head.numAntennaPaths = 0x00;   // no phase measurement, so no tone tail is expected
    head.numStepsReported = numStepsReported;
    return head;
}

// A 0x32 head carries neither the counters nor the frequency compensation nor the power level
// (7.7.65,45 p2459): the report keeps the opening 0x31's values for all four. Its Config_ID is
// on the wire but is not a lookup key on this path (see CsInjectMode0Continue).
HciLeCsSubeventResultContinueEventParam CsInjectContinueHead(uint16_t connectionHandle, uint8_t configId,
    uint8_t procedureDoneStatus, uint8_t subeventDoneStatus, uint8_t numStepsReported)
{
    HciLeCsSubeventResultContinueEventParam head = {};
    head.connectionHandle = connectionHandle;
    head.configId = configId;
    head.procedureDoneStatus = procedureDoneStatus;
    head.subeventDoneStatus = subeventDoneStatus;
    head.abortReason = 0x00;
    head.numAntennaPaths = 0x00;
    head.numStepsReported = numStepsReported;
    return head;
}

// One 0x31 whose single mode-0 step is `stepData`, a subevent that closes with the frame:
// both done statuses are 0x00, the pair Table 7.2 allows for a complete subevent
// (7.7.65,44 p2446). A mode-0 step body carries no length of its own, so `stepLength` is
// what the caller chose to send - and therefore which role's template the bytes have to
// match for the step to decode (p2452).
void CsInjectMode0Result(uint16_t connectionHandle, uint8_t configId, uint8_t channel,
    const uint8_t *stepData, size_t stepLength)
{
    CsInjectBlob steps;
    steps.Append(CS_STEP_MODE_0, channel, stepData, stepLength);
    const HciLeCsSubeventResultEventParam head = CsInjectResultHead(
        connectionHandle, configId, 0x00, 0x00, steps.count);
    CsInjectSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), steps);
}

// The same frame with Subevent_Done_Status 0x01, so the subevent it opens stays open for a
// 0x32 to continue. Both statuses have to be partial together: Procedure_Done 0x00 with
// Subevent_Done 0x01 is outside Table 7.2 and the pair is refused before any step is read
// (cs_result.c:73).
void CsInjectMode0PartialResult(uint16_t connectionHandle, uint8_t configId, uint8_t channel,
    const uint8_t *stepData, size_t stepLength)
{
    CsInjectBlob steps;
    steps.Append(CS_STEP_MODE_0, channel, stepData, stepLength);
    const HciLeCsSubeventResultEventParam head = CsInjectResultHead(
        connectionHandle, configId, 0x01, 0x01, steps.count);
    CsInjectSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), steps);
}

// One 0x32 carrying one mode-0 step and closing the subevent the 0x31 above opened. The
// Config_ID it names is deliberately not the one the report will carry: a 0x32 is routed by
// the connection row's open stream and the report keeps the opening 0x31's Config_ID
// (cs.c:655-675, cs_result.c:441).
void CsInjectMode0Continue(uint16_t connectionHandle, uint8_t configId, uint8_t channel,
    const uint8_t *stepData, size_t stepLength)
{
    CsInjectBlob steps;
    steps.Append(CS_STEP_MODE_0, channel, stepData, stepLength);
    const HciLeCsSubeventResultContinueEventParam head = CsInjectContinueHead(
        connectionHandle, configId, 0x00, 0x00, steps.count);
    CsInjectSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_CONTINUE_EVENT, &head, sizeof(head), steps);
}

// 0x008D with both roles enabled. 00640's two configurations take one role each out of the
// same per-connection defaults, and pp2816-2817 make the order load-bearing: a create for a
// role that was never enabled is a 0x12, and so is disabling a role that already holds a
// configuration, so both bits have to be on before the first 0x0090 and cannot be adjusted
// after it. The shared CsPrereqSetDefaultSettings enables the initiator alone and cannot be
// used here, which is why this case carries its own copy.
void CsRunDefaultSettingsBothRoles(CsE2eContext *ctx, uint16_t connHandle)
{
    CsLeSetDefaultSettingsParam defaults = { };
    defaults.connectionHandle = connHandle;
    defaults.roleEnable = CS_DEFAULT_ROLE_ENABLE_BOTH;
    defaults.csSyncAntennaSelection = CS_DEFAULT_SYNC_ANTENNA_SELECTION_REPEAT;
    defaults.maxTxPower = CS_DEFAULT_MAX_TX_POWER_DBM;
    ctx->defaults.ResetAll();
    ASSERT_EQ(CSIF_LeCsSetDefaultSettings(&defaults), BT_SUCCESS) << "0x008D dispatch failed";
    ASSERT_TRUE(ctx->defaults.Wait()) << "no 0x008D Command_Complete";
    ASSERT_EQ(ctx->defaults.status, HCI_STATUS_SUCCESS) <<
        "the Controller refused the default settings, status=0x" <<
        static_cast<int>(ctx->defaults.status);

    EXPECT_EQ(ctx->defaults.connectionHandle, connHandle) << "the 0x008D named another link";
}

// 0x0090 into the local Controller alone. The shared CsPrereqCreateConfig asks for
// Create_Context 0x01, which makes the peer's Controller create the mirror image under its own
// role enable; this one asks for 0x00, which the spec defines as a write into the local
// Controller only and which still answers with a 0x2F (7.8.137 p2824 and p2827). That is what
// lets the case hold two configurations with two different roles without the peer taking part.
// The mirror is compared on the fields the decode reads: Role above all, plus the RTT type and
// the mode-0 step count the same report is read against.
void CsRunCreateConfigLocal(CsE2eContext *ctx, uint16_t connHandle, uint8_t configId, uint8_t role)
{
    CsLeCreateConfigParam config = { };
    config.connectionHandle = connHandle;
    config.configId = configId;
    config.createContext = CS_CREATE_CONTEXT_LOCAL_ONLY;
    config.mainModeType = CS_MAIN_MODE_TYPE_2;
    config.subModeType = CS_SUB_MODE_TYPE_1;
    config.minMainModeSteps = CS_MAIN_MODE_STEPS_MIN;
    config.maxMainModeSteps = CS_MAIN_MODE_STEPS_MAX;
    config.mainModeRepetition = CS_MAIN_MODE_REPETITION_0;
    config.mode0Steps = CS_MODE0_STEPS_1;
    config.role = role;
    config.rttType = CS_RTT_TYPE_AA_ONLY;
    config.csSyncPhy = CS_SYNC_PHY_LE_1M;
    ASSERT_EQ(memcpy_s(config.channelMap, sizeof(config.channelMap),
        CS_CHANNEL_MAP_26_TO_61, sizeof(CS_CHANNEL_MAP_26_TO_61)), EOK);
    config.channelMapRepetition = CS_CHANNEL_MAP_REPETITION_1;
    config.channelSelectionType = CS_CHANNEL_SELECTION_TYPE_3B;
    config.ch3cShape = CS_CH3C_SHAPE_HAT;
    config.ch3cJump = CS_CH3C_JUMP_MIN;
    config.reserved = 0x00;
    ctx->config.ResetAll();
    ASSERT_EQ(CSIF_LeCsCreateConfig(&config), BT_SUCCESS) << "0x0090 dispatch failed";
    ASSERT_TRUE(ctx->config.Wait()) << "no 0x2F for the configuration create";
    ASSERT_EQ(ctx->config.status, HCI_STATUS_SUCCESS) <<
        "the Controller refused the configuration, status=0x" <<
        static_cast<int>(ctx->config.status);

    ASSERT_TRUE(ctx->config.configValid) << "the 0x2F carried no configuration body";
    EXPECT_EQ(ctx->config.config.connectionHandle, connHandle) << "the 0x2F named another link";
    EXPECT_EQ(ctx->config.config.configId, configId) << "the 0x2F named another Config_ID";
    EXPECT_EQ(ctx->config.config.action, CS_CONFIG_ACTION_CREATED) << "the 0x2F reports a remove";
    EXPECT_EQ(ctx->config.config.role, role) << "the mirror holds another Role";
    EXPECT_EQ(ctx->config.config.rttType, config.rttType) << "the mirror holds another RTT_Type";
    EXPECT_EQ(ctx->config.config.mode0Steps, config.mode0Steps) << "the mirror holds another Mode_0_Steps";
    EXPECT_EQ(ctx->config.config.mainModeType, config.mainModeType) << "the mirror holds another Main_Mode_Type";
    EXPECT_EQ(ctx->config.config.subModeType, config.subModeType) << "the mirror holds another Sub_Mode_Type";
}

// 0x0091 for a named Config_ID. The shared CsRunRemoveConfig takes the status to expect and
// names Config 0; 00640 removes two configurations and expects 0x00 from both, so it carries
// its own copy rather than editing a helper four other cases call.
void CsRunRemoveConfigById(CsE2eContext *ctx, uint16_t connHandle, uint8_t configId)
{
    CsLeRemoveConfigParam removeParam = { };
    removeParam.connectionHandle = connHandle;
    removeParam.configId = configId;
    ctx->config.ResetAll();
    ASSERT_EQ(CSIF_LeCsRemoveConfig(&removeParam), BT_SUCCESS) << "0x0091 dispatch failed";
    ASSERT_TRUE(ctx->config.Wait()) << "no 0x2F for the configuration remove";
    ASSERT_EQ(ctx->config.status, HCI_STATUS_SUCCESS) << "the 0x2F answered 0x" << static_cast<int>(ctx->config.status);

    ASSERT_TRUE(ctx->config.configValid) << "the 0x2F carried no body";
    EXPECT_EQ(ctx->config.config.configId, configId) << "the 0x2F named another Config_ID";
    EXPECT_EQ(ctx->config.config.action, CS_CONFIG_ACTION_REMOVED) << "the 0x2F reports a create";
}
}  // namespace

/**
 * @tc.number: StackGapLe60Interact_CsProcedureEndToEnd_00600
 * @tc.name: A full CS procedure runs over a live encrypted ACL and its 0x31 report decodes
 * @tc.desc: Gate K_CS_READY (local bit 46 and a peer whose bit 46 the connect-time read
 *           revealed). Register the CS session callback, the SMP auto-answer responder
 *           and the CS HCI observation table, then connect and drive the 5.1.26 (p3200)
 *           chain with the vectors of a reference 6.0 initiator: encrypt
 *           (5.1.23 p3195 requires it before CS Security Start), 0x0089 through
 *           CSIF_LeCsEnableSession with the local capability body asserted through
 *           CSIF_LeCsReadLocalCaps, 0x008A -> 0x2C, 0x008D -> Command_Complete, 0x0090
 *           -> 0x2F action 0x01 with its configuration mirror compared octet for octet,
 *           0x008C -> 0x2E, 0x008E -> 0x2D in either of its two legitimate answers, and
 *           0x0093 -> Command_Complete. 0x0094 enable is then asserted to leave the
 *           procedure enabled (0x30 state 0x01) and the first 0x31 report the CS engine
 *           hands up is asserted field by field: this link's handle, Config_ID 0, the
 *           role the steps were decoded against, the RTT type in force, at least one
 *           decoded step, and a first step that is mode-0 with its measurement decoded
 *           out of the initiator's five-octet template. The case closes down in the
 *           reverse order the Controller demands: 0x0094 disable (0x30 state 0x00),
 *           0x0091 remove (0x2F action 0x00) and CSIF_LeCsDisableSession. The chain runs
 *           through the shared CsPrereq helpers, plus CsRunProcedureEnable and
 *           CsRunRemoveConfig, which 00610 drives too.
 */
static void CsAssertFirstMode0Report(CsE2eContext *ctx, uint16_t connHandle)
{
    ASSERT_TRUE(ctx->result.Wait()) << "no 0x31 subevent result for the enabled procedure";
    ASSERT_TRUE(ctx->result.latched) << "the 0x31 handler latched no report";
    const CsSubeventReport &report = ctx->result.report;
    EXPECT_EQ(report.connectionHandle, connHandle) << "the report belongs to another link";
    EXPECT_EQ(report.configId, CS_CONFIG_ID_0) << "the report belongs to another Config_ID";
    EXPECT_EQ(report.role, CS_ROLE_INITIATOR) << "the steps were decoded in the wrong role";
    EXPECT_EQ(report.rttType, CS_RTT_TYPE_AA_ONLY) << "the report names another RTT type";
    ASSERT_GE(report.numSteps, 1) << "the subevent carried no decoded step";
    EXPECT_EQ(report.steps[0].mode, CS_STEP_MODE_0) << "a CS subevent has to open with mode-0 steps (4.5.x p3770)";

    EXPECT_TRUE(report.steps[0].hasMeasurement) <<
        "the first mode-0 step's data did not fit the initiator role template";

    EXPECT_LE(report.steps[0].channel, CS_STEP_CHANNEL_MAX) << "the first step's channel is out of range";
    printf("CS 0x31: handle=0x%04X configId=%u role=0x%02X rttType=0x%02X steps=%u "
           "subeventDone=0x%02X procedureDone=0x%02X abortReason=0x%02X antennaPaths=%u\n",
        report.connectionHandle, report.configId, report.role, report.rttType, report.numSteps,
        report.subeventDoneStatus, report.procedureDoneStatus, report.abortReason, report.numAntennaPaths);
    printf("CS 0x31: first step mode=%u channel=%u dataLength=%u measurement=%u\n",
        report.steps[0].mode, report.steps[0].channel, report.steps[0].stepDataLength,
        report.steps[0].hasMeasurement ? 1 : 0);
}

HWTEST_F(StackGapLe60InteractTest, StackGapLe60Interact_CsProcedureEndToEnd_00600, TestSize.Level1)
{
    CsDualGate gate = CsProbePeerSupport();
    if (gate != K_CS_READY) {
        GTEST_SKIP() << CsGateSkipMessage(gate);
    }

    CsE2eContext ctx;
    CsCallbackGuard callbackGuard(&ctx);
    if (!callbackGuard.Registered()) {
        GTEST_SKIP() << "could not register the CS session callback";
    }
    CsHciCallbackGuard hciGuard;
    if (!hciGuard.Registered()) {
        GTEST_SKIP() << "could not register the 0x0089 completion table";
    }
    CsPairResponderGuard pairGuard;

    uint16_t connHandle = 0;
    ASSERT_TRUE(ConnectToPeer(connHandle)) << "direct connect to the CS peer failed";

    ASSERT_NO_FATAL_FAILURE(CsPrereqEncryptLink(&ctx));
    ASSERT_NO_FATAL_FAILURE(CsPrereqOpenSession(connHandle));

    bool peerReflector = false;
    ASSERT_NO_FATAL_FAILURE(CsPrereqExchangeCapabilities(&ctx, connHandle, &peerReflector));
    if (!peerReflector) {
        GTEST_SKIP() << "peer controller reports no reflector role (Roles_Supported bit 1)";
    }

    ASSERT_NO_FATAL_FAILURE(CsPrereqSetDefaultSettings(&ctx, connHandle));
    ASSERT_NO_FATAL_FAILURE(CsPrereqCreateConfig(&ctx, connHandle));
    ASSERT_NO_FATAL_FAILURE(CsPrereqSecurityEnable(&ctx, connHandle, HCI_STATUS_SUCCESS));
    ASSERT_NO_FATAL_FAILURE(CsPrereqReadRemoteFae(&ctx, connHandle));
    ASSERT_NO_FATAL_FAILURE(CsPrereqSetProcedureParameters(&ctx, connHandle));

    ASSERT_NO_FATAL_FAILURE(CsRunProcedureEnable(
        &ctx, connHandle, CS_PROCEDURE_ENABLE, HCI_STATUS_SUCCESS));
    EXPECT_EQ(ctx.procedureEnable.schedule.state, CS_PROCEDURE_STATE_ENABLED) <<
        "the 0x30 reports a disabled procedure";

    CsAssertFirstMode0Report(&ctx, connHandle);

    ASSERT_NO_FATAL_FAILURE(CsRunProcedureEnable(
        &ctx, connHandle, CS_PROCEDURE_DISABLE, HCI_STATUS_SUCCESS));
    EXPECT_EQ(ctx.procedureEnable.schedule.state, CS_PROCEDURE_STATE_DISABLED) <<
        "the 0x30 leaves the procedure enabled";

    ASSERT_NO_FATAL_FAILURE(CsRunRemoveConfig(&ctx, connHandle, HCI_STATUS_SUCCESS));

    EXPECT_EQ(CSIF_LeCsDisableSession(connHandle), BT_SUCCESS) << "CS session close failed";
}


/**
 * @tc.number: StackGapLe60Interact_CsProcedurePreconditionReject_00610
 * @tc.name: The Controller's refusals of a CS command reach the caller as the event a
 *           success would, and a refused 0x0091 leaves the configuration addressable
 * @tc.desc: Gate K_CS_READY. Set up as 00600 (session callback, SMP responder, CS HCI
 *           observation table), but the connect is left unencrypted and three refusals are
 *           driven: (A) 0x008C on a link with no encryption - 7.8.133 (p2815) makes that a
 *           pre-command check, so the answer is 0x2F Insufficient Security and the link
 *           stays useable; the whole 5.1.26 (p3200) chain then runs, encryption included,
 *           and the same 0x008C issued again completes with 0x00. (B) 0x0091 while a
 *           procedure is still enabled - 7.8.138 (p2828) makes that 0x0C Command
 *           Disallowed, and the refusal must not take the local mirror with it.
 *           (C) a second 0x0094 enable, the discriminator for (B): 7.8.141 (p2838) answers
 *           0x0C for an already-enabled Config_ID, where a dropped mirror would draw the
 *           Host's own 0x12 out of CsProcedureEnable (cs.c) before anything is sent - and
 *           0x0C is also what the Host answers when no 0x0093 has completed, which the one
 *           above rules out. The State on that branch comes from the failure row's "state
 *           that stands" (Vol 6 Part D 6.37 p3629), not from the Controller. The normal
 *           teardown follows (0x0094 disable, 0x0091 remove) - the remove being the very
 *           request (B) had refused - and the Command_Status witness is asserted last: for
 *           a CS opcode it proves the command reached the Controller, which a Host-side
 *           refusal never does (cs.c refuses through HciOnCmdFailed without sending). The
 *           counts carry that claim, since the case knows exactly how many times it issued
 *           each command: 0x008C twice, 0x0091 twice, 0x0094 three times.
 */
static void CsAssertPreconditionRejectWitness(void)
{
    EXPECT_TRUE(g_csCommandStatus.WaitForOpCodeCount(
        HCI_LE_CS_SECURITY_ENABLE, CS_EXPECTED_SECURITY_ENABLE_CMD_STATUS)) <<
        "the unencrypted 0x008C never reached the Controller";

    EXPECT_TRUE(g_csCommandStatus.SawOpCodeWithStatus(HCI_LE_CS_SECURITY_ENABLE,
        HCI_STATUS_INSUFFICIENT_SECURITY)) <<
            "no Command_Status carried 0x2F for the unencrypted 0x008C (7.8.133 p2815)";

    EXPECT_TRUE(g_csCommandStatus.WaitForOpCodeCount(
        HCI_LE_CS_REMOVE_CONFIG, CS_EXPECTED_REMOVE_CONFIG_CMD_STATUS)) <<
        "the refused 0x0091 never reached the Controller";

    EXPECT_TRUE(g_csCommandStatus.SawOpCodeWithStatus(HCI_LE_CS_REMOVE_CONFIG,
        HCI_STATUS_COMMAND_DISALLOWED)) <<
            "no Command_Status carried 0x0C for the 0x0091 on the enabled slot (7.8.138 p2828)";

    EXPECT_TRUE(g_csCommandStatus.WaitForOpCodeCount(
        HCI_LE_CS_PROCEDURE_ENABLE, CS_EXPECTED_PROCEDURE_ENABLE_CMD_STATUS)) <<
        "the second 0x0094 never reached the Controller";

    EXPECT_TRUE(g_csCommandStatus.SawOpCodeWithStatus(HCI_LE_CS_PROCEDURE_ENABLE,
        HCI_STATUS_COMMAND_DISALLOWED)) << "no Command_Status carried 0x0C for the second 0x0094 (7.8.141 p2838)";

    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_SECURITY_ENABLE),
        CS_EXPECTED_SECURITY_ENABLE_CMD_STATUS) <<
        "0x008C went out a number of times other than the two this case intends";

    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_REMOVE_CONFIG),
        CS_EXPECTED_REMOVE_CONFIG_CMD_STATUS) <<
        "0x0091 went out a number of times other than the two this case intends";

    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_PROCEDURE_ENABLE),
        CS_EXPECTED_PROCEDURE_ENABLE_CMD_STATUS) <<
        "0x0094 went out a number of times other than the three this case intends";

    EXPECT_EQ(g_csCommandStatus.DroppedCount(), 0) << "the Command_Status log overflowed";
}

HWTEST_F(StackGapLe60InteractTest, StackGapLe60Interact_CsProcedurePreconditionReject_00610, TestSize.Level1)
{
    CsDualGate gate = CsProbePeerSupport();
    if (gate != K_CS_READY) {
        GTEST_SKIP() << CsGateSkipMessage(gate);
    }

    CsE2eContext ctx;
    CsCallbackGuard callbackGuard(&ctx);
    if (!callbackGuard.Registered()) {
        GTEST_SKIP() << "could not register the CS session callback";
    }
    CsHciCallbackGuard hciGuard;
    if (!hciGuard.Registered()) {
        GTEST_SKIP() << "could not register the CS HCI observation table";
    }
    CsPairResponderGuard pairGuard;

    uint16_t connHandle = 0;
    ASSERT_TRUE(ConnectToPeer(connHandle)) << "direct connect to the CS peer failed";

    ASSERT_NO_FATAL_FAILURE(CsPrereqOpenSession(connHandle));
    ASSERT_NO_FATAL_FAILURE(CsPrereqSecurityEnable(&ctx, connHandle, HCI_STATUS_INSUFFICIENT_SECURITY));

    ASSERT_NO_FATAL_FAILURE(CsPrereqEncryptLink(&ctx));
    bool peerReflector = false;
    ASSERT_NO_FATAL_FAILURE(CsPrereqExchangeCapabilities(&ctx, connHandle, &peerReflector));
    if (!peerReflector) {
        GTEST_SKIP() << "peer controller reports no reflector role (Roles_Supported bit 1)";
    }
    ASSERT_NO_FATAL_FAILURE(CsPrereqSetDefaultSettings(&ctx, connHandle));
    ASSERT_NO_FATAL_FAILURE(CsPrereqCreateConfig(&ctx, connHandle));
    ASSERT_NO_FATAL_FAILURE(CsPrereqSecurityEnable(&ctx, connHandle, HCI_STATUS_SUCCESS));
    ASSERT_NO_FATAL_FAILURE(CsPrereqReadRemoteFae(&ctx, connHandle));
    ASSERT_NO_FATAL_FAILURE(CsPrereqSetProcedureParameters(&ctx, connHandle));
    ASSERT_NO_FATAL_FAILURE(CsRunProcedureEnable(
        &ctx, connHandle, CS_PROCEDURE_ENABLE, HCI_STATUS_SUCCESS));
    EXPECT_EQ(ctx.procedureEnable.schedule.state, CS_PROCEDURE_STATE_ENABLED) <<
        "the procedure was not enabled";

    ASSERT_NO_FATAL_FAILURE(CsRunRemoveConfig(
        &ctx, connHandle, HCI_STATUS_COMMAND_DISALLOWED));

    ASSERT_NO_FATAL_FAILURE(CsRunProcedureEnable(
        &ctx, connHandle, CS_PROCEDURE_ENABLE, HCI_STATUS_COMMAND_DISALLOWED));
    EXPECT_EQ(ctx.procedureEnable.schedule.state, CS_PROCEDURE_STATE_DISABLED) <<
        "the rejected enable did not report the state that stands";

    ASSERT_NO_FATAL_FAILURE(CsRunProcedureEnable(
        &ctx, connHandle, CS_PROCEDURE_DISABLE, HCI_STATUS_SUCCESS));
    EXPECT_EQ(ctx.procedureEnable.schedule.state, CS_PROCEDURE_STATE_DISABLED) <<
        "the 0x30 leaves the procedure enabled";

    ASSERT_NO_FATAL_FAILURE(CsRunRemoveConfig(&ctx, connHandle, HCI_STATUS_SUCCESS));

    EXPECT_EQ(CSIF_LeCsDisableSession(connHandle), BT_SUCCESS) << "CS session close failed";

    CsAssertPreconditionRejectWitness();
}


/**
 * @tc.number: StackGapLe60Interact_CsProcedureEnablePreconditionReject_00620
 * @tc.name: A 0x0094 the engine refuses before the wire reaches the caller as the one 0x30
 *           shape the Controller's own refusal has, and the disable path is not gated
 * @tc.desc: Gate K_CS_READY. Set up as 00600 up to the 0x008D (session callback, SMP responder,
 *           CS HCI observation table, encryption, session, capability exchange, default
 *           settings), then three 0x0094 are driven. (A) enable on a Config_ID the session
 *           holds no slot for - the engine's own 0x12. (B) enable after a successful 0x0090
 *           but before any 0x0093 - the engine's own 0x0C. 7.8.141 (pp2838-2839) hands the
 *           Controller the same two statuses for the same two states (0x12 when the
 *           configuration does not exist, 0x0C for an enable ahead of that Config_ID's
 *           0x0093), and the failure row is built to answer in the Controller's shape, so the
 *           callback cannot tell the two sides apart. What tells them apart is that nothing
 *           was sent: p2839 makes the Command_Status mandatory for a 0x0094 that reaches the
 *           wire, while the engine's refusals call HciOnCmdFailed without issuing anything.
 *           The witness count is therefore 0 across (A) and (B), and 1 at (C) - the disable,
 *           which cs.c does not guard at all. No rejection condition on p2838 covers a
 *           disable (the 0x12 and the two 0x0C are scoped to a configuration that does not
 *           exist and to an enable), so 0x00 with State 0x00 is the conformant answer; that
 *           status is checked with EXPECT rather than through the shared helper's ASSERT so
 *           the witness still reports when a board answers otherwise. The engine's own slot
 *           view (CsGetProcedureState) has no CSIF entry and lives in src/cs/cs.h, so why each
 *           refusal is the right one is anchored on the status moving 0x12 -> 0x0C exactly
 *           when the 0x0090 lands, and on that 0x2F being the mirror compare of 00600's
 *           helper. Teardown: the 0x0091 remove, then the session close.
 */
static void CsAssertProcedureDisableReachesController(CsE2eContext *ctx, uint16_t connHandle)
{
    CsLeProcedureEnableParam disableParam = { };
    disableParam.connectionHandle = connHandle;
    disableParam.configId = CS_CONFIG_ID_0;
    disableParam.enable = CS_PROCEDURE_DISABLE;
    ctx->procedureEnable.ResetAll();
    ASSERT_EQ(CSIF_LeCsProcedureEnable(&disableParam), BT_SUCCESS) << "0x0094 dispatch failed";
    ASSERT_TRUE(ctx->procedureEnable.Wait()) << "no 0x30 for the procedure disable";
    EXPECT_EQ(ctx->procedureEnable.status, HCI_STATUS_SUCCESS) <<
        "the disable answered 0x" <<
        static_cast<int>(ctx->procedureEnable.status) <<
        "; 7.8.141 pp2838-2839 lists no rejection for it";

    EXPECT_TRUE(ctx->procedureEnable.scheduleValid) << "the 0x30 carried no schedule body";
    EXPECT_EQ(ctx->procedureEnable.schedule.connectionHandle, connHandle) << "the 0x30 named another link";
    EXPECT_EQ(ctx->procedureEnable.schedule.configId, CS_CONFIG_ID_0) << "the 0x30 named another Config_ID";
    EXPECT_EQ(ctx->procedureEnable.schedule.state, CS_PROCEDURE_STATE_DISABLED) <<
        "the 0x30 reports the procedure as standing enabled";
}

static void CsAssertEnablePreconditionWitness(void)
{
    EXPECT_TRUE(g_csCommandStatus.WaitForOpCodeCount(HCI_LE_CS_PROCEDURE_ENABLE, 1)) <<
        "the disable never reached the Controller";

    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_PROCEDURE_ENABLE), 1) <<
        "0x0094 went out a number of times other than the one this case intends";

    EXPECT_TRUE(g_csCommandStatus.SawOpCodeWithStatus(HCI_LE_CS_PROCEDURE_ENABLE, HCI_STATUS_SUCCESS)) <<
        "the one 0x0094 that went out was not the disable";

    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_REMOVE_CONFIG), 1) <<
        "0x0091 went out a number of times other than the one this case intends";

    EXPECT_EQ(g_csCommandStatus.DroppedCount(), 0) << "the Command_Status log overflowed";
}

HWTEST_F(StackGapLe60InteractTest, StackGapLe60Interact_CsProcedureEnablePreconditionReject_00620, TestSize.Level1)
{
    CsDualGate gate = CsProbePeerSupport();
    if (gate != K_CS_READY) {
        GTEST_SKIP() << CsGateSkipMessage(gate);
    }

    CsE2eContext ctx;
    CsCallbackGuard callbackGuard(&ctx);
    if (!callbackGuard.Registered()) {
        GTEST_SKIP() << "could not register the CS session callback";
    }
    CsHciCallbackGuard hciGuard;
    if (!hciGuard.Registered()) {
        GTEST_SKIP() << "could not register the CS HCI observation table";
    }
    CsPairResponderGuard pairGuard;

    uint16_t connHandle = 0;
    ASSERT_TRUE(ConnectToPeer(connHandle)) << "direct connect to the CS peer failed";

    ASSERT_NO_FATAL_FAILURE(CsPrereqEncryptLink(&ctx));
    ASSERT_NO_FATAL_FAILURE(CsPrereqOpenSession(connHandle));
    bool peerReflector = false;
    ASSERT_NO_FATAL_FAILURE(CsPrereqExchangeCapabilities(&ctx, connHandle, &peerReflector));
    if (!peerReflector) {
        GTEST_SKIP() << "peer controller reports no reflector role (Roles_Supported bit 1)";
    }
    ASSERT_NO_FATAL_FAILURE(CsPrereqSetDefaultSettings(&ctx, connHandle));

    // (A) No slot for this Config_ID yet: the engine refuses the enable itself, 0x12, and
    // nothing leaves the Host.
    ASSERT_NO_FATAL_FAILURE(CsRunProcedureEnable(&ctx, connHandle, CS_PROCEDURE_ENABLE,
        HCI_STATUS_INVALID_HCI_COMMAND_PARAMETERS));
    EXPECT_EQ(ctx.procedureEnable.schedule.state, CS_PROCEDURE_STATE_DISABLED) <<
        "the refused enable did not report the state that stands";

    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_PROCEDURE_ENABLE), 0) <<
        "the refused enable reached the Controller";

    ASSERT_NO_FATAL_FAILURE(CsPrereqCreateConfig(&ctx, connHandle));

    // (B) The slot is in use now but no 0x0093 has been accepted on it: the engine's other
    // refusal, 0x0C, and still nothing on the wire.
    ASSERT_NO_FATAL_FAILURE(CsRunProcedureEnable(&ctx, connHandle, CS_PROCEDURE_ENABLE,
        HCI_STATUS_COMMAND_DISALLOWED));
    EXPECT_EQ(ctx.procedureEnable.schedule.state, CS_PROCEDURE_STATE_DISABLED) <<
        "the refused enable did not report the state that stands";

    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_PROCEDURE_ENABLE), 0) <<
        "the second refused enable reached the Controller";

    // (C) Disabling is not gated, so this request has to reach the Controller. EXPECT, not
    // the helper's ASSERT, so a board answering otherwise still leaves the witness readable.
    ASSERT_NO_FATAL_FAILURE(CsAssertProcedureDisableReachesController(&ctx, connHandle));

    ASSERT_NO_FATAL_FAILURE(CsRunRemoveConfig(&ctx, connHandle, HCI_STATUS_SUCCESS));
    EXPECT_EQ(CSIF_LeCsDisableSession(connHandle), BT_SUCCESS) << "CS session close failed";

    // The witness, read last: the one 0x0094 that may show up here is the disable, and its
    // Command_Status is what says the two refusals above were the Host's, not a Controller's.
    CsAssertEnablePreconditionWitness();
}


/**
 * @tc.number: StackGapLe60Interact_CsCompletionUniquenessEngine_00630
 * @tc.name: Each CS-type command closes with exactly one completion
 * @tc.desc: Gate K_CS_READY. On a live encrypted ACL, each CS-type command
 *           (0x008A/0x008C/0x008E/0x0090/0x0091/0x0094/0x0096 — completion is a
 *           subevent, not Command_Complete) yields exactly one completion, whether
 *           the issue reached the Controller or was refused in cs.c.
 *           Instruments: (a) CsCallbackTally per CSIF callback vs issue count
 *           (0x0094: 4 completions / 2 on wire — two slot probes refused locally);
 *           (b) g_csCommandStatus for the seven CS-type opcodes (hit = reached
 *           Controller, miss = local refuse).
 *           Engine records probed where CSIF can read them: 0x008A/0x008E write-back
 *           cache (BT_ALREADY / No_FAE -> BT_NOT_SUPPORT); 0x0090/0x0091 slot via
 *           0x0094 precondition (local 0x0C / 0x12); 0x0096 endPending. 0x008C has no
 *           Host latch (Security Start is a Controller/LL gate); slot enabled is
 *           completion-side only. 7.8.132 "config already created" is not mirrored
 *           on the Host — write-back goes out; Controller answers 0x0C if it still
 *           holds. Cache markers still yield BT_ALREADY on this single-connection
 *           chain. 0x0096's 0x33 here is failure-row synthesis (no CS test running).
 */
static void CsAssertWriteBackRemoteCapsAlready(uint16_t connHandle)
{
    EXPECT_EQ(CSIF_LeCsWriteBackRemoteCaps(connHandle), BT_ALREADY) <<
        "the engine holds no capability body for this handle: the 0x2C took no effect";
}

static void CsAssertEnableRejectedBeforeConfig(CsE2eContext *ctx, uint16_t connHandle)
{
    ASSERT_NO_FATAL_FAILURE(CsRunProcedureEnable(ctx, connHandle, CS_PROCEDURE_ENABLE,
        HCI_STATUS_COMMAND_DISALLOWED));
    EXPECT_EQ(ctx->procedureEnable.schedule.state, CS_PROCEDURE_STATE_DISABLED) <<
        "the refused enable did not report the state that stands";

    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_PROCEDURE_ENABLE), 0) <<
        "the engine's own refusal let the 0x0094 reach the Controller";
}

static void CsAssertWriteBackRemoteFaeReadsCache(CsE2eContext *ctx, uint16_t connHandle)
{
    ASSERT_NO_FATAL_FAILURE(CsPrereqReadRemoteFae(ctx, connHandle));
    if (ctx->fae.status == HCI_STATUS_SUCCESS) {
        EXPECT_EQ(CSIF_LeCsWriteBackRemoteFae(connHandle), BT_ALREADY) <<
            "the engine holds no FAE table for this handle: the 0x2D took no effect";
    } else {
        EXPECT_EQ(CSIF_LeCsWriteBackRemoteFae(connHandle), BT_NOT_SUPPORT) <<
            "a 0x2D that cached nothing still reads as a cached table";
    }
}

static void CsAssertEnableDisablePair(CsE2eContext *ctx, uint16_t connHandle)
{
    ASSERT_NO_FATAL_FAILURE(CsRunProcedureEnable(ctx, connHandle, CS_PROCEDURE_ENABLE, HCI_STATUS_SUCCESS));
    EXPECT_EQ(ctx->procedureEnable.schedule.state, CS_PROCEDURE_STATE_ENABLED) <<
        "the 0x30 reports a disabled procedure";

    ASSERT_NO_FATAL_FAILURE(CsRunProcedureEnable(ctx, connHandle, CS_PROCEDURE_DISABLE, HCI_STATUS_SUCCESS));
    EXPECT_EQ(ctx->procedureEnable.schedule.state, CS_PROCEDURE_STATE_DISABLED) <<
        "the 0x30 leaves the procedure enabled";
}

static void CsAssertRemoveConfigThenEnableRejected(CsE2eContext *ctx, uint16_t connHandle)
{
    ASSERT_NO_FATAL_FAILURE(CsRunRemoveConfig(ctx, connHandle, HCI_STATUS_SUCCESS));
    ASSERT_NO_FATAL_FAILURE(CsRunProcedureEnable(ctx, connHandle, CS_PROCEDURE_ENABLE,
        HCI_STATUS_INVALID_HCI_COMMAND_PARAMETERS));
    EXPECT_EQ(ctx->procedureEnable.schedule.state, CS_PROCEDURE_STATE_DISABLED) <<
        "the refused enable did not report the state that stands";

    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_PROCEDURE_ENABLE),
        CS_EXPECTED_PROCEDURE_ENABLE_PAIR_CMD_STATUS) <<
        "0x0094 went out a number of times other than the enable and the disable this case intends";
}

static void CsAssertTestEndDuplicateRefused(void)
{
    g_csTestEnd.ResetAll();
    ASSERT_EQ(CSIF_LeCsTestEnd(), BT_SUCCESS) << "0x0096 dispatch failed";
    EXPECT_EQ(CSIF_LeCsTestEnd(), BT_ALREADY) <<
        "the second 0x0096 was not refused on the record the engine keeps for the first";

    ASSERT_TRUE(g_csTestEnd.Wait()) << "no 0x33 for the test end";
    EXPECT_EQ(g_csTestEnd.status, HCI_STATUS_COMMAND_DISALLOWED) <<
        "the 0x33 reports 0x" <<
        static_cast<int>(g_csTestEnd.status) <<
        "; 7.8.143 p2855 makes a 0x0096 with no test running 0x0C";

    g_csTestEnd.ResetAll();
    ASSERT_EQ(CSIF_LeCsTestEnd(), BT_SUCCESS) << "the 0x0096 after the first 0x33 was refused";
    ASSERT_TRUE(g_csTestEnd.Wait()) << "no 0x33 for the second test end";
    EXPECT_EQ(g_csTestEnd.status, HCI_STATUS_COMMAND_DISALLOWED) << "the second 0x33 reports another status";
}

static void CsAssertCallbackTallyForUniqueness(void)
{
    EXPECT_EQ(g_csCallbackTally.remoteCaps.load(), 1) << "0x008A closed a number of times other than once";
    EXPECT_EQ(g_csCallbackTally.remoteFae.load(), 1) << "0x008E closed a number of times other than once";
    EXPECT_EQ(g_csCallbackTally.securityEnable.load(), 1) << "0x008C closed a number of times other than once";
    EXPECT_EQ(g_csCallbackTally.config.load(), CS_EXPECTED_CONFIG_CALLBACK_COUNT) <<
        "0x0090 and 0x0091 closed a number of times other than twice";
    EXPECT_EQ(g_csCallbackTally.procedureEnable.load(), CS_EXPECTED_PROCEDURE_ENABLE_CALLBACK_COUNT) <<
        "the four 0x0094 of this case closed a number of times other than four";

    EXPECT_EQ(g_csCallbackTally.procedureParameters.load(), 1) << "0x0093 closed a number of times other than once";
    EXPECT_EQ(g_csCallbackTally.defaultSettings.load(), 1) << "0x008D closed a number of times other than once";
    EXPECT_EQ(g_csCallbackTally.testEndComplete.load(), CS_EXPECTED_TEST_END_CALLBACK_COUNT) <<
        "the two 0x0096 that reached the Controller closed a number of times other than twice";

    EXPECT_EQ(g_csCallbackTally.channelClassification.load(), 0) <<
        "a 0x0092 completion arrived although no 0x0092 was issued";

    EXPECT_EQ(g_csCallbackTally.testComplete.load(), 0) << "a 0x0095 completion arrived although no 0x0095 was issued";
    EXPECT_EQ(g_csCallbackTally.sessionClosed.load(), 0) <<
        "the session reported itself closed although this case closed it";

    EXPECT_EQ(g_csCallbackTally.Total(), CS_EXPECTED_TOTAL_CALLBACK_COUNT) <<
        "the case produced a number of completions other than the thirteen it intends";
}

static void CsAssertCommandStatusForUniqueness(void)
{
    EXPECT_TRUE(g_csCommandStatus.WaitForOpCodeCount(HCI_LE_CS_READ_REMOTE_SUPPORTED_CAPABILITIES, 1)) <<
        "0x008A never reached the Controller";
    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_READ_REMOTE_SUPPORTED_CAPABILITIES), 1) <<
        "0x008A went out a number of times other than the one this case intends";

    EXPECT_TRUE(g_csCommandStatus.WaitForOpCodeCount(HCI_LE_CS_READ_REMOTE_FAE_TABLE, 1)) <<
        "0x008E never reached the Controller";
    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_READ_REMOTE_FAE_TABLE), 1) <<
        "0x008E went out a number of times other than the one this case intends";

    EXPECT_TRUE(g_csCommandStatus.WaitForOpCodeCount(HCI_LE_CS_SECURITY_ENABLE, 1)) <<
        "0x008C never reached the Controller";
    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_SECURITY_ENABLE), 1) <<
        "0x008C went out a number of times other than the one this case intends";

    EXPECT_TRUE(g_csCommandStatus.WaitForOpCodeCount(HCI_LE_CS_CREATE_CONFIG, 1)) <<
        "0x0090 never reached the Controller";
    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_CREATE_CONFIG), 1) <<
        "0x0090 went out a number of times other than the one this case intends";

    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_REMOVE_CONFIG), 1) <<
        "0x0091 went out a number of times other than the one this case intends";

    EXPECT_TRUE(g_csCommandStatus.WaitForOpCodeCount(
        HCI_LE_CS_PROCEDURE_ENABLE, CS_EXPECTED_PROCEDURE_ENABLE_PAIR_CMD_STATUS)) <<
        "the enable and the disable never both reached the Controller";
    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_PROCEDURE_ENABLE),
        CS_EXPECTED_PROCEDURE_ENABLE_PAIR_CMD_STATUS) <<
        "0x0094 went out a number of times other than the enable and the disable this case intends";

    EXPECT_TRUE(g_csCommandStatus.WaitForOpCodeCount(
        HCI_LE_CS_TEST_END, CS_EXPECTED_TEST_END_CMD_STATUS)) <<
        "the two 0x0096 this case sent never both reached the Controller";
    EXPECT_TRUE(g_csCommandStatus.SawOpCodeWithStatus(HCI_LE_CS_TEST_END, HCI_STATUS_COMMAND_DISALLOWED)) <<
        "no Command_Status carried 0x0C for a 0x0096 with no test running (7.8.143 p2855)";
    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_TEST_END),
        CS_EXPECTED_TEST_END_CMD_STATUS) <<
        "0x0096 went out a number of times other than the two this case intends";

    EXPECT_EQ(g_csCommandStatus.DroppedCount(), 0) << "the Command_Status log overflowed";
}

HWTEST_F(StackGapLe60InteractTest, StackGapLe60Interact_CsCompletionUniquenessEngine_00630, TestSize.Level1)
{
    CsDualGate gate = CsProbePeerSupport();
    if (gate != K_CS_READY) {
        GTEST_SKIP() << CsGateSkipMessage(gate);
    }

    CsE2eContext ctx;
    CsCallbackTallyGuard callbackGuard(&ctx);
    if (!callbackGuard.Registered()) {
        GTEST_SKIP() << "could not register the CS session callback";
    }
    CsHciCallbackGuard hciGuard;
    if (!hciGuard.Registered()) {
        GTEST_SKIP() << "could not register the CS HCI observation table";
    }
    CsPairResponderGuard pairGuard;

    uint16_t connHandle = 0;
    ASSERT_TRUE(ConnectToPeer(connHandle)) << "direct connect to the CS peer failed";
    ASSERT_NO_FATAL_FAILURE(CsPrereqEncryptLink(&ctx));
    ASSERT_NO_FATAL_FAILURE(CsPrereqOpenSession(connHandle));

    bool peerReflector = false;
    ASSERT_NO_FATAL_FAILURE(CsPrereqExchangeCapabilities(&ctx, connHandle, &peerReflector));
    if (!peerReflector) {
        GTEST_SKIP() << "peer controller reports no reflector role (Roles_Supported bit 1)";
    }
    CsAssertWriteBackRemoteCapsAlready(connHandle);

    ASSERT_NO_FATAL_FAILURE(CsPrereqSetDefaultSettings(&ctx, connHandle));

    ASSERT_NO_FATAL_FAILURE(CsPrereqCreateConfig(&ctx, connHandle));
    CsAssertEnableRejectedBeforeConfig(&ctx, connHandle);

    ASSERT_NO_FATAL_FAILURE(CsPrereqSecurityEnable(&ctx, connHandle, HCI_STATUS_SUCCESS));
    CsAssertWriteBackRemoteFaeReadsCache(&ctx, connHandle);

    ASSERT_NO_FATAL_FAILURE(CsPrereqSetProcedureParameters(&ctx, connHandle));
    CsAssertEnableDisablePair(&ctx, connHandle);
    CsAssertRemoveConfigThenEnableRejected(&ctx, connHandle);
    CsAssertTestEndDuplicateRefused();

    EXPECT_EQ(CSIF_LeCsDisableSession(connHandle), BT_SUCCESS) << "CS session close failed";

    CsAssertCallbackTallyForUniqueness();
    CsAssertCommandStatusForUniqueness();
}


/**
 * @tc.number: StackGapLe60Interact_CsResultReassemblyConnection_00640
 * @tc.name: The connection channel decodes a subevent only past the row and slot lookups, and
 *           decodes its steps against the role of the Config_ID it names
 * @tc.desc: Gate K_CS_READY. Plan 8.3's result reassembly on the connection channel, over a live
 *           encrypted ACL: a 0x31 carrying a real link's handle is decoded only once the engine
 *           holds both a row for that handle and a configuration for the Config_ID it names
 *           (cs.c:622 and cs.c:628), where the test channel's 0x0FFF is diverted ahead of both
 *           (cs.c:618). 02302 drives the same decode matrix on 0x0FFF, so the gate is what is
 *           left, and this case drives it by injecting frames on the connection channel:
 *           HciOnEvent is the synchronous entry a real event takes and filters on no mask, so
 *           a hand-built frame is dispatched exactly as a received one is.
 *           The blocks walk the gate's two keys one at a time and in both states. (A) a live
 *           ACL with no CS session - no row - is dropped; (B) the row after CSIF_LeCsEnable-
 *           Session, still with no configuration, is dropped, and 0x008A and 0x008D do not
 *           change that; (C) after Config 0 is created the same frame is reported, field by
 *           field - handle, Config_ID, the role and RTT type the configuration and not the
 *           event decides, the four head values only the opening 0x31 carries, and the
 *           initiator's five-octet mode-0 template decoded; (D) a Config_ID with no slot and
 *           one outside the 0x00-0x03 range are both dropped and neither disturbs the slot in
 *           use; (E) the role flip; (F) the 0x32 routing rule; (G) an orphan 0x32; (H) and (I)
 *           the two keys coming apart on the way out - 0x0091 for one Config_ID closes that
 *           slot alone, the second 0x0091 closes the other; (J) the session close takes the row
 *           and with it every slot, which is the state (A) started from.
 *           (E) is the case's second subject and the reason it holds two configurations. The
 *           decode reads the role from the 0x2F mirror of the slot the report is attributed to
 *           (cs.c:639), so Config 0 is created with the initiator role (through the shared
 *           CsPrereqCreateConfig, Create_Context 0x01, the peer creating the mirror image as
 *           in 00600) and Config 1 with the reflector role and Create_Context 0x00, which the
 *           spec defines as a write into the local Controller alone and which still answers
 *           with a 0x2F (7.8.137 p2824 and p2827). Role is the local Controller's CS role and
 *           not the link's, and either role is allowed in either link role ([Vol 6] Part B
 *           5.1.26 p3200), so a Central holding a reflector-role configuration is legal. Mode 0
 *           is the one mode whose template the role decides - five octets for the initiator,
 *           three for the reflector (7.7.65,44 p2452) - so the same five octets are decoded on
 *           Config 0 and left undecoded on Config 1, and the same three octets the other way
 *           round; a single-role case would read that same difference out of a hardcoded role,
 *           which is why one side cannot carry it. 0x008D goes out once with Role_Enable 0x03
 *           and only before the first 0x0090: pp2816-2817 make a create for a role that was
 *           never enabled a 0x12, and disabling a role that already holds a configuration the
 *           other 0x12, so the bits cannot be added after a configuration exists.
 *           (F) is the other routing rule. A 0x32 continues the subevent the last 0x31 opened
 *           on the handle and never reads its own Config_ID (cs.c:655-675), so the report
 *           keeps the opening 0x31's Config_ID and counters and its steps are decoded with that
 *           0x31's configuration role. The case fixes the engine's choice there rather than a
 *           spec rule, and says so: 7.7.65,45 p2459 makes the Config_ID ignored only on the
 *           test channel's 0x0FFF and leaves the field unconstrained on a connection handle, so
 *           a 0x32 naming a Config_ID outside the field's range is passed through and its steps
 *           are decoded against the stream's role all the same. (G) reads the same rule the
 *           other way: with no subevent open on the handle there is nothing to continue, so the
 *           frame is dropped rather than read as the start of one.
 *           Honest limits. The two Config_IDs of (D) are dropped by one check, so a pass there
 *           does not separate "no slot" from "outside the range" - (I) and (J) are what read
 *           the range check's other side, both on IDs the field allows. The role flip needs a
 *           local Controller able to hold a reflector-role configuration of its own; that is
 *           read from the local capability body's Roles_Supported bit 1 and a Controller
 *           without it is skipped here rather than driven into the 0x11 that 7.8.134 p2816
 *           makes its answer - a Controller limit, not this case's subject. The witness is the
 *           Command_Status log: 0x008A once and 0x0090/0x0091 twice each are what says the slot
 *           states the frames were judged against were the Controller's, not the engine's own
 *           guess.
 */

static void CsAssertNoSessionDropsFrame(CsE2eContext *ctx, uint16_t connHandle)
{
    CsInjectMode0Result(connHandle, CS_CONFIG_ID_0, CS_INJECT_CHANNEL_FIRST,
        CS_MODE0_INITIATOR_STEP, CS_MODE0_INITIATOR_LEN);
    EXPECT_EQ(ctx->result.count, 0) << "a subevent was reported on a handle with no CS session";
}

static void CsAssertNoConfigDropsFrame(CsE2eContext *ctx, uint16_t connHandle)
{
    CsInjectMode0Result(connHandle, CS_CONFIG_ID_0, CS_INJECT_CHANNEL_FIRST,
        CS_MODE0_INITIATOR_STEP, CS_MODE0_INITIATOR_LEN);
    EXPECT_EQ(ctx->result.count, 0) << "a subevent was reported for a Config_ID with no configuration";
}

static void CsAssertConfig0DecodesInitiator(CsE2eContext *ctx, uint16_t connHandle)
{
    ctx->result.ResetAll();
    CsInjectMode0Result(connHandle, CS_CONFIG_ID_0, CS_INJECT_CHANNEL_FIRST,
        CS_MODE0_INITIATOR_STEP, CS_MODE0_INITIATOR_LEN);
    ASSERT_TRUE(ctx->result.Wait()) << "a subevent on a handle and Config_ID the engine holds was not reported";

    EXPECT_EQ(ctx->result.count, 1) << "one frame produced a number of reports other than one";
    const CsSubeventReport &first = ctx->result.report;
    EXPECT_EQ(first.connectionHandle, connHandle) << "the report named another link";
    EXPECT_EQ(first.configId, CS_CONFIG_ID_0) << "the report named another Config_ID";
    EXPECT_EQ(first.role, CS_ROLE_INITIATOR) << "the steps were decoded against another role";
    EXPECT_EQ(first.rttType, CS_RTT_TYPE_AA_ONLY) << "the steps were decoded against another RTT type";
    EXPECT_EQ(first.startAclConnEventCounter, CS_INJECT_ACL_EVENT_COUNTER) << "another event counter";
    EXPECT_EQ(first.procedureCounter, CS_INJECT_PROCEDURE_COUNTER) << "another procedure counter";
    EXPECT_EQ(first.frequencyCompensation, CS_INJECT_FREQ_COMPENSATION) << "another compensation";
    EXPECT_EQ(first.referencePowerLevel, CS_INJECT_REFERENCE_POWER_DBM) << "another power level";
    EXPECT_EQ(first.procedureDoneStatus, 0x00) << "another procedure status";
    EXPECT_EQ(first.subeventDoneStatus, 0x00) << "another subevent status";
    EXPECT_EQ(first.abortReason, 0x00) << "another abort reason";
    EXPECT_EQ(first.numAntennaPaths, 0x00) << "another antenna-path count";
    EXPECT_EQ(first.numStepsReported, 0x01) << "another reported step count";
    EXPECT_EQ(first.numSteps, 0x01) << "the decoded step count is not the one that was sent";
    EXPECT_EQ(first.steps[0].mode, CS_STEP_MODE_0) << "another step mode";
    EXPECT_EQ(first.steps[0].channel, CS_INJECT_CHANNEL_FIRST) << "another step channel";
    EXPECT_EQ(first.steps[0].stepDataLength, CS_MODE0_INITIATOR_LEN) << "another Step_Data_Length";
    EXPECT_TRUE(first.steps[0].hasMeasurement) << "the initiator's five-octet mode-0 template was not decoded";
    EXPECT_TRUE(first.steps[0].stepData.mode0.hasFreqOffset) <<
        "the initiator's template carries a Measured_Freq_Offset and none was decoded";
    EXPECT_EQ(first.steps[0].stepData.mode0.packetQuality, 0x0A) << "another Packet_Quality";
    EXPECT_EQ(first.steps[0].stepData.mode0.packetRssi, CS_PACKET_RSSI_DBM) << "another Packet_RSSI";
    EXPECT_EQ(first.steps[0].stepData.mode0.packetAntenna, 0x01) << "another Packet_Antenna";
    EXPECT_EQ(first.steps[0].stepData.mode0.measuredFreqOffset, CS_MEASURED_FREQ_OFFSET_0_01PPM) <<
        "another Measured_Freq_Offset";
}

static void CsAssertConfigIdGateHolds(CsE2eContext *ctx, uint16_t connHandle)
{
    CsInjectMode0Result(connHandle, CS_CONFIG_ID_2, CS_INJECT_CHANNEL_SECOND,
        CS_MODE0_INITIATOR_STEP, CS_MODE0_INITIATOR_LEN);
    CsInjectMode0Result(connHandle, CS_CONFIG_ID_OUT_OF_RANGE, CS_INJECT_CHANNEL_SECOND,
        CS_MODE0_INITIATOR_STEP, CS_MODE0_INITIATOR_LEN);
    EXPECT_EQ(ctx->result.count, 1) << "a subevent was reported for a Config_ID with no slot";
}

static void CsAssertRoleFlip(CsE2eContext *ctx, uint16_t connHandle)
{
    CsInjectMode0Result(connHandle, CS_CONFIG_ID_1, CS_INJECT_CHANNEL_THIRD,
        CS_MODE0_INITIATOR_STEP, CS_MODE0_INITIATOR_LEN);
    EXPECT_EQ(ctx->result.count, 1) << "a subevent was reported for a Config_ID with no slot yet";

    ASSERT_NO_FATAL_FAILURE(CsRunCreateConfigLocal(ctx, connHandle, CS_CONFIG_ID_1, CS_ROLE_REFLECTOR));

    ctx->result.ResetAll();
    CsInjectMode0Result(connHandle, CS_CONFIG_ID_1, CS_INJECT_CHANNEL_THIRD,
        CS_MODE0_INITIATOR_STEP, CS_MODE0_INITIATOR_LEN);
    ASSERT_TRUE(ctx->result.Wait()) << "a subevent on Config 1 was not reported";
    EXPECT_EQ(ctx->result.count, 1) << "one frame produced a number of reports other than one";
    EXPECT_EQ(ctx->result.report.configId, CS_CONFIG_ID_1) << "the report named another Config_ID";
    EXPECT_EQ(ctx->result.report.role, CS_ROLE_REFLECTOR) << "the steps were decoded against another role";
    EXPECT_EQ(ctx->result.report.numSteps, 0x01) << "the undecoded step was not kept";
    EXPECT_EQ(ctx->result.report.steps[0].stepDataLength, CS_MODE0_INITIATOR_LEN) << "another Step_Data_Length";
    EXPECT_FALSE(ctx->result.report.steps[0].hasMeasurement) <<
        "a five-octet mode-0 step decoded under the reflector role: the role did not decide the template";

    ctx->result.ResetAll();
    CsInjectMode0Result(connHandle, CS_CONFIG_ID_1, CS_INJECT_CHANNEL_FOURTH,
        CS_MODE0_REFLECTOR_STEP, CS_MODE0_REFLECTOR_LEN);
    ASSERT_TRUE(ctx->result.Wait()) << "a subevent on Config 1 was not reported";
    EXPECT_EQ(ctx->result.count, 1) << "one frame produced a number of reports other than one";
    EXPECT_EQ(ctx->result.report.role, CS_ROLE_REFLECTOR) << "the steps were decoded against another role";
    EXPECT_EQ(ctx->result.report.steps[0].stepDataLength, CS_MODE0_REFLECTOR_LEN) << "another Step_Data_Length";
    EXPECT_TRUE(ctx->result.report.steps[0].hasMeasurement) <<
        "the reflector's three-octet mode-0 template was not decoded";
    EXPECT_FALSE(ctx->result.report.steps[0].stepData.mode0.hasFreqOffset) <<
        "the reflector's template carries no Measured_Freq_Offset";
    EXPECT_EQ(ctx->result.report.steps[0].stepData.mode0.packetQuality, 0x0A) << "another Packet_Quality";
    EXPECT_EQ(ctx->result.report.steps[0].stepData.mode0.packetRssi, CS_PACKET_RSSI_DBM) << "another Packet_RSSI";
    EXPECT_EQ(ctx->result.report.steps[0].stepData.mode0.packetAntenna, 0x01) << "another Packet_Antenna";

    ctx->result.ResetAll();
    CsInjectMode0Result(connHandle, CS_CONFIG_ID_0, CS_INJECT_CHANNEL_FOURTH,
        CS_MODE0_REFLECTOR_STEP, CS_MODE0_REFLECTOR_LEN);
    ASSERT_TRUE(ctx->result.Wait()) << "a subevent on Config 0 was not reported";
    EXPECT_EQ(ctx->result.count, 1) << "one frame produced a number of reports other than one";
    EXPECT_EQ(ctx->result.report.role, CS_ROLE_INITIATOR) << "the steps were decoded against another role";
    EXPECT_EQ(ctx->result.report.steps[0].stepDataLength, CS_MODE0_REFLECTOR_LEN) << "another Step_Data_Length";
    EXPECT_FALSE(ctx->result.report.steps[0].hasMeasurement) <<
        "a three-octet mode-0 step decoded under the initiator role: the role did not decide the template";
}

static void CsAssertContinuationRouting(CsE2eContext *ctx, uint16_t connHandle)
{
    ctx->result.ResetAll();
    CsInjectMode0PartialResult(connHandle, CS_CONFIG_ID_0, CS_INJECT_CHANNEL_FIRST,
        CS_MODE0_INITIATOR_STEP, CS_MODE0_INITIATOR_LEN);
    EXPECT_EQ(ctx->result.count, 0) << "a 0x31 whose Subevent_Done_Status says it continues emitted a report";

    CsInjectMode0Continue(connHandle, CS_CONFIG_ID_OUT_OF_RANGE, CS_INJECT_CHANNEL_SECOND,
        CS_MODE0_REFLECTOR_STEP, CS_MODE0_REFLECTOR_LEN);
    ASSERT_TRUE(ctx->result.Wait()) << "the continued subevent was not reported";
    EXPECT_EQ(ctx->result.count, 1) << "the 0x31 and its 0x32 produced a number of reports other than one";
    const CsSubeventReport &joined = ctx->result.report;
    EXPECT_EQ(joined.configId, CS_CONFIG_ID_0) << "the 0x32's Config_ID reached the report";
    EXPECT_EQ(joined.role, CS_ROLE_INITIATOR) << "the 0x32's steps were decoded against another role";
    EXPECT_EQ(joined.numStepsReported, CS_NUM_STEPS_REPORTED_ONE) <<
        "the reported step count is not the opening 0x31's";
    EXPECT_EQ(joined.numSteps, CS_NUM_STEPS_TWO) << "the 0x32's step was not appended to the open subevent";
    EXPECT_EQ(joined.procedureCounter, CS_INJECT_PROCEDURE_COUNTER) << "the 0x32 has no counter of its own";
    EXPECT_EQ(joined.frequencyCompensation, CS_INJECT_FREQ_COMPENSATION) << "the 0x32 has no compensation of its own";
    EXPECT_EQ(joined.referencePowerLevel, CS_INJECT_REFERENCE_POWER_DBM) << "the 0x32 has no power level of its own";
    EXPECT_EQ(joined.subeventDoneStatus, CS_SUBEVENT_DONE_STATUS_OK) << "the statuses were not refreshed from the 0x32";
    EXPECT_EQ(joined.steps[0].channel, CS_INJECT_CHANNEL_FIRST) << "another first step channel";
    EXPECT_TRUE(joined.steps[0].hasMeasurement) << "the opening 0x31's step lost its measurement";
    EXPECT_EQ(joined.steps[1].channel, CS_INJECT_CHANNEL_SECOND) << "another second step channel";
    EXPECT_EQ(joined.steps[1].stepDataLength, CS_MODE0_REFLECTOR_LEN) << "another Step_Data_Length";
    EXPECT_FALSE(joined.steps[1].hasMeasurement) <<
        "the 0x32's three octets decoded under the stream's initiator role: the 0x32's Config_ID decided the role";
}

static void CsAssertOrphanContinueDropped(CsE2eContext *ctx, uint16_t connHandle)
{
    CsInjectMode0Continue(connHandle, CS_CONFIG_ID_0, CS_INJECT_CHANNEL_SECOND,
        CS_MODE0_REFLECTOR_STEP, CS_MODE0_REFLECTOR_LEN);
    EXPECT_EQ(ctx->result.count, 1) << "a 0x32 with no subevent open was reported";
}

static void CsAssertRemoveOneSlotKeepsOther(CsE2eContext *ctx, uint16_t connHandle,
    uint16_t removedId, uint16_t keptId)
{
    ASSERT_NO_FATAL_FAILURE(CsRunRemoveConfigById(ctx, connHandle, removedId));
    CsInjectMode0Result(connHandle, removedId, CS_INJECT_CHANNEL_FIRST,
        CS_MODE0_INITIATOR_STEP, CS_MODE0_INITIATOR_LEN);
    EXPECT_EQ(ctx->result.count, 1) << "a subevent was reported for a removed configuration";

    ctx->result.ResetAll();
    CsInjectMode0Result(connHandle, keptId, CS_INJECT_CHANNEL_THIRD,
        CS_MODE0_REFLECTOR_STEP, CS_MODE0_REFLECTOR_LEN);
    ASSERT_TRUE(ctx->result.Wait()) << "the configuration that was not removed stopped reporting";
    EXPECT_EQ(ctx->result.count, 1) << "one frame produced a number of reports other than one";
    EXPECT_EQ(ctx->result.report.configId, keptId) << "the report named another Config_ID";
    EXPECT_EQ(ctx->result.report.role, CS_ROLE_REFLECTOR) << "the steps were decoded against another role";
    EXPECT_TRUE(ctx->result.report.steps[0].hasMeasurement) << "the surviving slot lost its decode";
}

static void CsAssertRemoveBothSlotsDrops(CsE2eContext *ctx, uint16_t connHandle)
{
    ASSERT_NO_FATAL_FAILURE(CsRunRemoveConfigById(ctx, connHandle, CS_CONFIG_ID_1));
    ctx->result.ResetAll();
    CsInjectMode0Result(connHandle, CS_CONFIG_ID_1, CS_INJECT_CHANNEL_THIRD,
        CS_MODE0_REFLECTOR_STEP, CS_MODE0_REFLECTOR_LEN);
    EXPECT_EQ(ctx->result.count, 0) << "a subevent was reported for a removed configuration";
}

static void CsAssertSessionCloseDrops(CsE2eContext *ctx, uint16_t connHandle)
{
    EXPECT_EQ(CSIF_LeCsDisableSession(connHandle), BT_SUCCESS) << "CS session close failed";
    CsInjectMode0Result(connHandle, CS_CONFIG_ID_1, CS_INJECT_CHANNEL_THIRD,
        CS_MODE0_REFLECTOR_STEP, CS_MODE0_REFLECTOR_LEN);
    EXPECT_EQ(ctx->result.count, 0) << "a subevent was reported after the CS session was closed";
}

static void CsAssertResultReassemblyWitness(void)
{
    EXPECT_TRUE(g_csCommandStatus.WaitForOpCodeCount(HCI_LE_CS_READ_REMOTE_SUPPORTED_CAPABILITIES, 1)) <<
        "0x008A never reached the Controller";
    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_READ_REMOTE_SUPPORTED_CAPABILITIES), 1) <<
        "0x008A went out a number of times other than the one this case intends";

    EXPECT_TRUE(g_csCommandStatus.WaitForOpCodeCount(
        HCI_LE_CS_CREATE_CONFIG, CS_EXPECTED_CREATE_CONFIG_CMD_STATUS)) <<
        "the two 0x0090 this case sent never both reached the Controller";
    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_CREATE_CONFIG),
        CS_EXPECTED_CREATE_CONFIG_CMD_STATUS) <<
        "0x0090 went out a number of times other than the two this case intends";

    EXPECT_TRUE(g_csCommandStatus.WaitForOpCodeCount(
        HCI_LE_CS_REMOVE_CONFIG, CS_EXPECTED_REMOVE_CONFIG_CMD_STATUS)) <<
        "the two 0x0091 this case sent never both reached the Controller";
    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_REMOVE_CONFIG),
        CS_EXPECTED_REMOVE_CONFIG_CMD_STATUS) <<
        "0x0091 went out a number of times other than the two this case intends";

    EXPECT_TRUE(g_csCommandStatus.SawOpCodeWithStatus(HCI_LE_CS_CREATE_CONFIG, HCI_STATUS_SUCCESS)) <<
        "no 0x0090 of this case was accepted by the Controller, so no slot was ever in use";

    EXPECT_EQ(g_csCommandStatus.CountOpCode(HCI_LE_CS_PROCEDURE_ENABLE), 0) <<
        "a procedure was enabled although this case drives no procedure";

    EXPECT_EQ(g_csCommandStatus.DroppedCount(), 0) << "the Command_Status log overflowed";
}

HWTEST_F(StackGapLe60InteractTest, StackGapLe60Interact_CsResultReassemblyConnection_00640, TestSize.Level1)
{
    CsDualGate gate = CsProbePeerSupport();
    if (gate != K_CS_READY) {
        GTEST_SKIP() << CsGateSkipMessage(gate);
    }

    CsE2eContext ctx;
    CsCallbackGuard callbackGuard(&ctx);
    if (!callbackGuard.Registered()) {
        GTEST_SKIP() << "could not register the CS session callback";
    }
    CsHciCallbackGuard hciGuard;
    if (!hciGuard.Registered()) {
        GTEST_SKIP() << "could not register the CS HCI observation table";
    }
    CsPairResponderGuard pairGuard;

    uint16_t connHandle = 0;
    ASSERT_TRUE(ConnectToPeer(connHandle)) << "direct connect to the CS peer failed";

    CsAssertNoSessionDropsFrame(&ctx, connHandle);

    ASSERT_NO_FATAL_FAILURE(CsPrereqEncryptLink(&ctx));
    ASSERT_NO_FATAL_FAILURE(CsPrereqOpenSession(connHandle));

    CsLeCapabilities localCaps = { };
    ASSERT_EQ(CSIF_LeCsReadLocalCaps(&localCaps), BT_SUCCESS) << "no local capability body was cached";
    if ((localCaps.rolesSupported & CS_ROLES_SUPPORTED_REFLECTOR) == 0) {
        GTEST_SKIP() << "local controller reports no reflector role (Roles_Supported bit 1)";
    }

    CsAssertNoConfigDropsFrame(&ctx, connHandle);

    bool peerReflector = false;
    ASSERT_NO_FATAL_FAILURE(CsPrereqExchangeCapabilities(&ctx, connHandle, &peerReflector));
    if (!peerReflector) {
        GTEST_SKIP() << "peer controller reports no reflector role (Roles_Supported bit 1)";
    }
    ASSERT_NO_FATAL_FAILURE(CsRunDefaultSettingsBothRoles(&ctx, connHandle));

    CsAssertNoConfigDropsFrame(&ctx, connHandle);

    ASSERT_NO_FATAL_FAILURE(CsPrereqCreateConfig(&ctx, connHandle));
    CsAssertConfig0DecodesInitiator(&ctx, connHandle);
    CsAssertConfigIdGateHolds(&ctx, connHandle);
    CsAssertRoleFlip(&ctx, connHandle);
    CsAssertContinuationRouting(&ctx, connHandle);
    CsAssertOrphanContinueDropped(&ctx, connHandle);
    CsAssertRemoveOneSlotKeepsOther(&ctx, connHandle, CS_CONFIG_ID_0, CS_CONFIG_ID_1);
    CsAssertRemoveBothSlotsDrops(&ctx, connHandle);
    CsAssertSessionCloseDrops(&ctx, connHandle);

    CsAssertResultReassemblyWitness();
}
}  // namespace Bluetooth
}  // namespace OHOS
