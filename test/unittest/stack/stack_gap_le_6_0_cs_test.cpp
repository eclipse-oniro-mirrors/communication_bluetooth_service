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

// BT 6.0 stack unit tests: Channel Sounding, Monitoring Advertisers, Decision-Based
// Advertising Filtering and Frame Space Update single-board cases. Split out of
// stack_gap_le_6_0_test.cpp to keep each source file within the size limit.

#include <gtest/gtest.h>

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <mutex>

#include "hci/evt/hci_evt.h"
#include "hci/hci.h"
#include "hci/hci_def_common.h"
#include "hci/hci_error.h"
#include "hci/hci_def_le_cmd.h"
#include "hci/hci_def_le_evt.h"
#include "hci/hci_def_le_feature.h"
#include "hci/hci_le_controller_6_0.h"
#include "hci/hci_def_evt.h"
#include "securec.h"
// Direct-drive entries for the MA Command_Complete and failure tables (5.3 pattern).
#include "src/hci/cmd/hci_cmd_failure_le_controller.h"
#include "src/hci/evt/hci_evt_le_cmd_complete.h"

#include "btm.h"
#include "btstack.h"
#include "cs_le_if.h"
#include "gap_le_if.h"
#include "gap_le_if_5_4.h"
#include "iso_le_if.h"
// Module-6 (ISOAL UFM) iso-layer seam: iso.h self-includes the hci command/event defs.
#include "src/iso/iso.h"
#include "stack_gap_le_6_0_test.h"

using namespace testing::ext;

namespace OHOS {
namespace Bluetooth {

// ---- MA helpers (single-board MA BTM-gate cases) --------------------------------
namespace {

constexpr uint32_t MA_CALLBACK_WAIT_MS = 10000;
constexpr uint32_t MA_QUIET_MS = 600;   // no-op drain window for the slot checks
constexpr int8_t MA_RSSI_LOW = -90;
constexpr int8_t MA_RSSI_HIGH = -60;
constexpr uint8_t MA_LOSS_TIMEOUT_SEC = 5;

// Monitored-list target shared by the slot/command cases. A fixed non-broadcasting
// address is enough: Add/Remove/Clear/Read/Enable are controller-local list
// operations and never need a real advertiser on the air.
BtAddr MaDummyAddr(uint8_t type)
{
    BtAddr addr = {};
    addr.type = type;
    for (int i = 0; i < BT_ADDRESS_SIZE; i++) {
        addr.addr[i] = static_cast<uint8_t>(0xA0 + i);
    }
    if (type == BT_RANDOM_DEVICE_ADDRESS) {
        // Random Static addresses need their two MSBs set to 11b, which 0xA5 is
        // not; pin the bits so the dummy is valid if ever sent to the controller.
        addr.addr[BT_ADDRESS_SIZE - 1] |= 0xC0;
    }
    return addr;
}

class MaWaiter {
public:
    void Reset()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        received_ = false;
    }

    void Notify()
    {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            received_ = true;
        }
        cv_.notify_all();
    }

    bool Wait(uint32_t timeoutMs)
    {
        std::unique_lock<std::mutex> lock(mtx_);
        return cv_.wait_for(lock, std::chrono::milliseconds(timeoutMs), [this] {
            return received_;
        });
    }

private:
    std::mutex mtx_;
    std::condition_variable cv_;
    bool received_ = false;
};

struct MaResult {
    MaWaiter waiter;
    uint8_t status = 0xFF;
    uint8_t listSize = 0xFF;
};

// Per-callback result state. GAP delivers every completion on the GAP thread, so
// the waiter guards the flags; status is written before Notify and read after Wait.
struct MaCtx {
    MaResult add;
    MaResult remove;
    MaResult clear;
    MaResult read;
    MaResult enable;
};

void OnMaAddResult(uint8_t status, void *context)
{
    MaCtx *ctx = static_cast<MaCtx *>(context);
    ctx->add.status = status;
    ctx->add.waiter.Notify();
}

void OnMaRemoveResult(uint8_t status, void *context)
{
    MaCtx *ctx = static_cast<MaCtx *>(context);
    ctx->remove.status = status;
    ctx->remove.waiter.Notify();
}

void OnMaClearResult(uint8_t status, void *context)
{
    MaCtx *ctx = static_cast<MaCtx *>(context);
    ctx->clear.status = status;
    ctx->clear.waiter.Notify();
}

void OnMaReadSizeResult(uint8_t status, uint8_t listSize, void *context)
{
    MaCtx *ctx = static_cast<MaCtx *>(context);
    ctx->read.status = status;
    ctx->read.listSize = listSize;
    ctx->read.waiter.Notify();
}

void OnMaEnableResult(uint8_t status, void *context)
{
    MaCtx *ctx = static_cast<MaCtx *>(context);
    ctx->enable.status = status;
    ctx->enable.waiter.Notify();
}

// Fill every command-result member; monitoredAdvertisersReport stays NULL (no 0x34
// is asserted here: the loss/recover pair belongs to the 8.3 two-device checks).
void MaFillCallback(MaCtx *ctx, GapMonitoredAdvertisersCallback *cb)
{
    *cb = GapMonitoredAdvertisersCallback{};
    cb->addDeviceToMonitoredAdvertisersListResult = OnMaAddResult;
    cb->removeDeviceFromMonitoredAdvertisersListResult = OnMaRemoveResult;
    cb->clearMonitoredAdvertisersListResult = OnMaClearResult;
    cb->readMonitoredAdvertisersListSizeResult = OnMaReadSizeResult;
    cb->enableMonitoringAdvertisersResult = OnMaEnableResult;
}

class MaCallbackGuard {
public:
    MaCallbackGuard() {}
    ~MaCallbackGuard()
    {
        if (registered_) {
            (void)GAPIF_DeregisterMonitoredAdvertisersCallback();
            registered_ = false;
        }
    }
    MaCallbackGuard(const MaCallbackGuard &) = delete;
    MaCallbackGuard &operator=(const MaCallbackGuard &) = delete;
    bool registered_ = false;
};

// Register a context as the single slot (register always replaces). Returns false
// when the GAPIF register itself fails.
bool MaRegister(MaCtx *ctx, MaCallbackGuard &guard)
{
    GapMonitoredAdvertisersCallback cb;
    MaFillCallback(ctx, &cb);
    if (GAPIF_RegisterMonitoredAdvertisersCallback(&cb, ctx) != BT_SUCCESS) {
        printf("MaRegister: register failed\n");
        return false;
    }
    guard.registered_ = true;
    return true;
}

bool MaSendAdd(MaCtx *ctx, uint8_t type, const BtAddr *addr)
{
    ctx->add.waiter.Reset();
    ctx->add.status = 0xFF;
    if (GAPIF_LeAddDeviceToMonitoredAdvertisersList(type, addr, MA_RSSI_LOW, MA_RSSI_HIGH, MA_LOSS_TIMEOUT_SEC) !=
        BT_SUCCESS) {
        return false;
    }
    return ctx->add.waiter.Wait(MA_CALLBACK_WAIT_MS);
}

bool MaSendRemove(MaCtx *ctx, uint8_t type, const BtAddr *addr)
{
    ctx->remove.waiter.Reset();
    ctx->remove.status = 0xFF;
    if (GAPIF_LeRemoveDeviceFromMonitoredAdvertisersList(type, addr) != BT_SUCCESS) {
        return false;
    }
    return ctx->remove.waiter.Wait(MA_CALLBACK_WAIT_MS);
}

bool MaSendClear(MaCtx *ctx)
{
    ctx->clear.waiter.Reset();
    ctx->clear.status = 0xFF;
    if (GAPIF_LeClearMonitoredAdvertisersList() != BT_SUCCESS) {
        return false;
    }
    return ctx->clear.waiter.Wait(MA_CALLBACK_WAIT_MS);
}

bool MaSendReadSize(MaCtx *ctx)
{
    ctx->read.waiter.Reset();
    ctx->read.status = 0xFF;
    ctx->read.listSize = 0xFF;
    if (GAPIF_LeReadMonitoredAdvertisersListSize() != BT_SUCCESS) {
        return false;
    }
    return ctx->read.waiter.Wait(MA_CALLBACK_WAIT_MS);
}

bool MaSendEnable(MaCtx *ctx, uint8_t enable)
{
    ctx->enable.waiter.Reset();
    ctx->enable.status = 0xFF;
    if (GAPIF_LeEnableMonitoringAdvertisers(enable) != BT_SUCCESS) {
        return false;
    }
    return ctx->enable.waiter.Wait(MA_CALLBACK_WAIT_MS);
}
}  // namespace

// ---- 8.3 Channel Sounding: capture and registration -----------------------------
// The CS cases run on the heavy fixture, not on StackGapLe60Test:
// BTM_IsControllerSupportLeChannelSounding reads the LE Read Local Supported Features
// result BTM_Initialize filled in, so on the light fixture it answers "no bit 46"
// unconditionally and every CS case would skip. They register a callback list of their
// own rather than borrowing g_parseCallbacks, so the light fixture's registrations and
// this fixture's other cases are untouched: the fifteen CS fields below are the only
// non-NULL entries in the table.
static HciEventCallbacks g_csParseCallbacks;

constexpr size_t CS_CONFIG_EVT_PAYLOAD = 33;  // bytes after the Subevent_Code

struct CsConfigCapture {
    bool received = false;
    HciLeCsConfigCompleteEventParam param = {};

    void Reset()
    {
        *this = CsConfigCapture{};
    }
};

struct CsProcParamsCapture {
    bool received = false;
    HciLeCsSetProcedureParametersReturnParam param = {};

    void Reset()
    {
        *this = CsProcParamsCapture{};
    }
};

// One counter per CS callback field, so "the path produced exactly one completion" is
// read off the whole CS callback surface instead of off the single field a case
// expects: a run that closes on the right field *and* also fans a second, wrong one is
// caught here. Field names mirror HciEventCallbacks; each comment names the event or
// return that owns the field.
struct CsCompletionTally {
    uint8_t readLocalCaps = 0;             // 0x0089 Command_Complete return
    uint8_t readRemoteCaps = 0;            // 0x2C subevent
    uint8_t writeCachedRemoteCaps = 0;     // 0x008B Command_Complete return
    uint8_t securityEnable = 0;            // 0x2E subevent
    uint8_t setDefaultSettings = 0;        // 0x008D Command_Complete return
    uint8_t readRemoteFae = 0;             // 0x2D subevent
    uint8_t writeCachedRemoteFae = 0;      // 0x008F Command_Complete return
    uint8_t config = 0;                    // 0x2F subevent, shared by 0x0090/0x0091
    uint8_t setChannelClassification = 0;  // 0x0092 Command_Complete return
    uint8_t setProcedureParameters = 0;    // 0x0093 Command_Complete return
    uint8_t procedureEnable = 0;           // 0x30 subevent
    uint8_t subeventResult = 0;            // 0x31 subevent
    uint8_t subeventResultContinue = 0;    // 0x32 subevent
    uint8_t test = 0;                      // 0x0095 Command_Complete return
    uint8_t testEnd = 0;                   // 0x33 subevent

    uint8_t Total() const
    {
        return readLocalCaps + readRemoteCaps + writeCachedRemoteCaps + securityEnable +
            setDefaultSettings + readRemoteFae + writeCachedRemoteFae + config +
            setChannelClassification + setProcedureParameters + procedureEnable +
            subeventResult + subeventResultContinue + test + testEnd;
    }

    void Reset()
    {
        *this = CsCompletionTally{};
    }
};

// The completion last delivered, whichever field carried it. One slot is enough because
// every drive in the 8.3 cases is followed by its own assertions before the next drive
// starts, so two completions are never in flight at once.
struct CsCompletionIdentity {
    uint8_t status = 0;
    uint16_t connectionHandle = 0;
    uint8_t configId = 0;
    uint8_t action = 0;
    uint8_t state = 0;

    // Set by the three failed-read fields (0x0089 / 0x2C / 0x2D) when the whole payload
    // they report is zero: a read that failed has no capabilities or table to hand on,
    // and a consumer reads that field only under Status 0x00 anyway.
    bool payloadAllZero = false;

    void Reset()
    {
        *this = CsCompletionIdentity{};
    }
};

static CsConfigCapture g_csConfigCapture;
static CsProcParamsCapture g_csProcParamsCapture;
static CsCompletionTally g_csTally;
static CsCompletionIdentity g_csIdentity;

static bool CsPayloadAllZero(const void *bytes, size_t size)
{
    const uint8_t *raw = static_cast<const uint8_t *>(bytes);
    for (size_t i = 0; i < size; i++) {
        if (raw[i] != 0) {
            return false;
        }
    }
    return true;
}

static void ResetCsCompletions()
{
    g_csTally.Reset();
    g_csIdentity.Reset();
}

// The uniqueness assertion the 8.3 completion cases rest on: exactly one CS completion
// landed (not zero, not two) and it landed on the field its command owns, not on a
// neighbour. The caller passes that field's own counter and its tally-member name, so a
// failure says which command misfired.
static void ExpectSoleCsCompletion(uint8_t count, const char *field)
{
    EXPECT_EQ(g_csTally.Total(), 1) <<
        "the path did not produce exactly one CS completion (expected on " <<
        field <<
        ")";

    EXPECT_EQ(count, 1) << "the completion did not land on " << field;
}

static void OnCsReadLocalCapsComplete(
    const HciLeCsReadLocalSupportedCapabilitiesReturnParam *returnParam)
{
    if (returnParam == nullptr) {
        return;
    }
    g_csTally.readLocalCaps++;
    g_csIdentity.status = returnParam->status;
    g_csIdentity.payloadAllZero =
        CsPayloadAllZero(&returnParam->capabilities, sizeof(returnParam->capabilities));
}

static void OnCsReadRemoteCapsComplete(
    const HciLeCsReadRemoteSupportedCapabilitiesCompleteEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_csTally.readRemoteCaps++;
    g_csIdentity.status = eventParam->status;
    g_csIdentity.connectionHandle = eventParam->connectionHandle;
    g_csIdentity.payloadAllZero =
        CsPayloadAllZero(&eventParam->capabilities, sizeof(eventParam->capabilities));
}

static void OnCsWriteCachedRemoteCapsComplete(
    const HciLeCsWriteCachedRemoteSupportedCapabilitiesReturnParam *returnParam)
{
    if (returnParam == nullptr) {
        return;
    }
    g_csTally.writeCachedRemoteCaps++;
    g_csIdentity.status = returnParam->status;
    g_csIdentity.connectionHandle = returnParam->connectionHandle;
}

static void OnCsSecurityEnableComplete(const HciLeCsSecurityEnableCompleteEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_csTally.securityEnable++;
    g_csIdentity.status = eventParam->status;
    g_csIdentity.connectionHandle = eventParam->connectionHandle;
}

static void OnCsSetDefaultSettingsComplete(const HciLeCsSetDefaultSettingsReturnParam *returnParam)
{
    if (returnParam == nullptr) {
        return;
    }
    g_csTally.setDefaultSettings++;
    g_csIdentity.status = returnParam->status;
    g_csIdentity.connectionHandle = returnParam->connectionHandle;
}

static void OnCsReadRemoteFaeComplete(const HciLeCsReadRemoteFaeTableCompleteEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_csTally.readRemoteFae++;
    g_csIdentity.status = eventParam->status;
    g_csIdentity.connectionHandle = eventParam->connectionHandle;
    g_csIdentity.payloadAllZero =
        CsPayloadAllZero(eventParam->remoteFaeTable, sizeof(eventParam->remoteFaeTable));
}

static void OnCsWriteCachedRemoteFaeComplete(
    const HciLeCsWriteCachedRemoteFaeTableReturnParam *returnParam)
{
    if (returnParam == nullptr) {
        return;
    }
    g_csTally.writeCachedRemoteFae++;
    g_csIdentity.status = returnParam->status;
    g_csIdentity.connectionHandle = returnParam->connectionHandle;
}

static void OnCsConfigComplete(const HciLeCsConfigCompleteEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_csConfigCapture.param = *eventParam;
    g_csConfigCapture.received = true;
    g_csTally.config++;
    g_csIdentity.status = eventParam->status;
    g_csIdentity.connectionHandle = eventParam->connectionHandle;
    g_csIdentity.configId = eventParam->configId;
    g_csIdentity.action = eventParam->action;
}

static void OnCsSetChannelClassificationComplete(
    const HciLeCsSetChannelClassificationReturnParam *returnParam)
{
    if (returnParam == nullptr) {
        return;
    }
    g_csTally.setChannelClassification++;
    g_csIdentity.status = returnParam->status;
}

static void OnCsSetProcedureParametersComplete(
    const HciLeCsSetProcedureParametersReturnParam *returnParam)
{
    if (returnParam == nullptr) {
        return;
    }
    g_csProcParamsCapture.param = *returnParam;
    g_csProcParamsCapture.received = true;
    g_csTally.setProcedureParameters++;
    g_csIdentity.status = returnParam->status;
    g_csIdentity.connectionHandle = returnParam->connectionHandle;
}

static void OnCsProcedureEnableComplete(const HciLeCsProcedureEnableCompleteEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_csTally.procedureEnable++;
    g_csIdentity.status = eventParam->status;
    g_csIdentity.connectionHandle = eventParam->connectionHandle;
    g_csIdentity.configId = eventParam->configId;
    g_csIdentity.state = eventParam->state;
}

static void OnCsSubeventResult(const HciLeCsSubeventResultEnvelope *envelope)
{
    if (envelope == nullptr) {
        return;
    }
    g_csTally.subeventResult++;
    // 0x31 carries no Status: its outcome is procedureDoneStatus/subeventDoneStatus.
    g_csIdentity.connectionHandle = envelope->head.connectionHandle;
}

static void OnCsSubeventResultContinue(const HciLeCsSubeventResultContinueEnvelope *envelope)
{
    if (envelope == nullptr) {
        return;
    }
    g_csTally.subeventResultContinue++;
    // 0x32 carries no Status either: same two outcome fields as 0x31.
    g_csIdentity.connectionHandle = envelope->head.connectionHandle;
}

static void OnCsTestComplete(const HciLeCsTestReturnParam *returnParam)
{
    if (returnParam == nullptr) {
        return;
    }
    g_csTally.test++;
    g_csIdentity.status = returnParam->status;
}

static void OnCsTestEndComplete(const HciLeCsTestEndCompleteEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_csTally.testEnd++;
    g_csIdentity.status = eventParam->status;
}

static void InitCsParseCallbacks()
{
    g_csParseCallbacks = {};
    g_csParseCallbacks.leCsReadLocalSupportedCapabilitiesComplete = OnCsReadLocalCapsComplete;
    g_csParseCallbacks.leCsReadRemoteSupportedCapabilitiesComplete = OnCsReadRemoteCapsComplete;
    g_csParseCallbacks.leCsWriteCachedRemoteSupportedCapabilitiesComplete =
        OnCsWriteCachedRemoteCapsComplete;
    g_csParseCallbacks.leCsSecurityEnableComplete = OnCsSecurityEnableComplete;
    g_csParseCallbacks.leCsSetDefaultSettingsComplete = OnCsSetDefaultSettingsComplete;
    g_csParseCallbacks.leCsReadRemoteFaeTableComplete = OnCsReadRemoteFaeComplete;
    g_csParseCallbacks.leCsWriteCachedRemoteFaeTableComplete = OnCsWriteCachedRemoteFaeComplete;
    g_csParseCallbacks.leCsConfigComplete = OnCsConfigComplete;
    g_csParseCallbacks.leCsSetChannelClassificationComplete = OnCsSetChannelClassificationComplete;
    g_csParseCallbacks.leCsSetProcedureParametersComplete = OnCsSetProcedureParametersComplete;
    g_csParseCallbacks.leCsProcedureEnableComplete = OnCsProcedureEnableComplete;
    g_csParseCallbacks.leCsSubeventResult = OnCsSubeventResult;
    g_csParseCallbacks.leCsSubeventResultContinue = OnCsSubeventResultContinue;
    g_csParseCallbacks.leCsTestComplete = OnCsTestComplete;
    g_csParseCallbacks.leCsTestEndComplete = OnCsTestEndComplete;
}

// ---- 8.3 Channel Sounding: result reassembly on the test channel --------------------
// A CS test's measurements reach a consumer through the public CSIF_ surface, not through
// the HciEventCallbacks table this fixture registers. Those are the two ends of the engine:
// that table is where the 0x31/0x32 of the HCI event layer arrive, while the consumer side
// is a single CsLeCallback slot. A case that only counted the table would be reading the
// engine's input rather than what a caller of the API is handed.
// The report the callback receives is the engine's own storage, reused for the stream's
// next subevent (cs_le_if.h on subeventResult), so the capture copies it whole.
struct CsResultCapture {
    uint8_t reports = 0;
    CsSubeventReport report = {};

    void Reset()
    {
        *this = CsResultCapture{};
    }
};

static CsResultCapture g_csResultCapture;
static uint8_t g_csLeTestStatus = 0;
static uint8_t g_csLeTestEndStatus = 0;
static CsLeCallback g_csLeCallbackList;

static void OnLeSubeventResult(const CsSubeventReport *report, void *context)
{
    (void)context;
    if (report == nullptr) {
        return;
    }
    g_csResultCapture.reports++;
    g_csResultCapture.report = *report;
}

static void OnLeTestComplete(uint8_t status, void *context)
{
    (void)context;
    g_csLeTestStatus = status;
}

static void OnLeTestEndComplete(uint8_t status, void *context)
{
    (void)context;
    g_csLeTestEndStatus = status;
}

static void InitCsLeCallback()
{
    g_csLeCallbackList = {};
    g_csLeCallbackList.subeventResult = OnLeSubeventResult;
    g_csLeCallbackList.testComplete = OnLeTestComplete;
    g_csLeCallbackList.testEndComplete = OnLeTestEndComplete;
}

// LE Meta event wire prefix: Event_Code + Parameter_Total_Length + Subevent_Code.
constexpr size_t LE_META_EVT_HEAD_LEN = 3;
constexpr size_t LE_META_EVT_PREFIX_LEN = 2;
constexpr size_t LE_META_EVT_EVENT_CODE_OFFSET = 0;
constexpr size_t LE_META_EVT_LENGTH_OFFSET = 1;
constexpr size_t LE_META_EVT_SUBEVENT_CODE_OFFSET = 2;
constexpr size_t CS_SUBEVENT_HEAD_CAP = 15;  // the 0x31 fixed head, the larger of the two
constexpr size_t CS_STEP_RECORD_LEN = 3;     // Step_Mode, Step_Channel, Step_Data_Length
// 79 three-octet records is the most one event holds: the body is capped at 255 octets by
// its own Length octet, 16 of those are the Subevent_Code and the fixed head, and a record
// whose step carries no data is three. The cross-packet step bound below needs 237 of them.
constexpr int CS_STEPS_PER_EVENT_MAX = 79;
constexpr size_t CS_STEP_BLOB_CAP = 256;
// The longest drive the cases build is the 79 empty records above, and it has to stay
// appendable; the blob itself is wider than one event body carries, which is the half
// InjectCsSubeventFrame's guard covers.
static_assert(CS_STEPS_PER_EVENT_MAX * CS_STEP_RECORD_LEN <= CS_STEP_BLOB_CAP);

// Mode-0 / Mode-3 step body lengths exercised in the decode cases.
constexpr size_t CS_MODE0_INITIATOR_LEN = 5;
constexpr size_t CS_MODE0_REFLECTOR_LEN = 3;
constexpr size_t CS_MODE3_STEP_LEN_PCT = 19;
constexpr size_t CS_MODE3_STEP_LEN_NO_PCT_3 = 23;
constexpr size_t CS_MODE3_STEP_LEN_NO_PCT_4 = 27;
constexpr size_t CS_MODE0_OVERSIZED_HAVE_LEN = 2;
constexpr int CS_EMPTY_STEP_RECORDS = 3;

// Physical values injected into mode-0 step bodies and expected back.
constexpr int8_t CS_REFERENCE_POWER_LEVEL_DBM = -10;
constexpr int8_t CS_PACKET_RSSI_DBM = -20;
constexpr int16_t CS_MEASURED_FREQ_OFFSET_0_01PPM = 10;

// Expected decoded tone-path counts for the mode-3 antenna-path values used.
constexpr uint8_t CS_MODE3_TONE_PATHS_PCT = 1;
constexpr uint8_t CS_MODE3_TONE_PATHS_2 = 3;
constexpr uint8_t CS_MODE3_TONE_PATHS_3 = 4;
constexpr uint8_t CS_MODE3_TONE_PATHS_4 = 5;

// Num_Antenna_Paths values fed into the 0x31 head in the mode-3 decode cases.
constexpr uint8_t CS_MODE3_NUM_ANTENNA_PATHS_PCT = 0x00;
constexpr uint8_t CS_MODE3_NUM_ANTENNA_PATHS_2 = 0x02;
constexpr uint8_t CS_MODE3_NUM_ANTENNA_PATHS_3 = 0x03;
constexpr uint8_t CS_MODE3_NUM_ANTENNA_PATHS_4 = 0x04;
constexpr uint8_t CS_MODE3_NUM_ANTENNA_PATHS_INVALID_FOR_PCT = 0x05;

// Procedure / Subevent done-status values from Core Spec v6.0 Table 7.2.
constexpr uint8_t CS_DONE_STATUS_OK = 0x00;
constexpr uint8_t CS_DONE_STATUS_CONTINUES = 0x01;
constexpr uint8_t CS_DONE_STATUS_RESERVED = 0x02;
constexpr uint8_t CS_DONE_STATUS_ALL_FAILED = 0x0F;

// Expected joined report sizes.
constexpr uint8_t CS_JOINED_NUM_STEPS = 3;
constexpr size_t CS_JOINED_THIRD_STEP_INDEX = 2;
constexpr uint8_t CS_JOINED_THIRD_STEP_CHANNEL = 0x07;

// Common report-count expectations.
constexpr uint8_t CS_EXPECTED_REPORTS_AFTER_REFLECTOR_STEP = 2;
constexpr uint8_t CS_EXPECTED_REPORTS_STREAM_TAIL = 2;
constexpr uint8_t CS_EXPECTED_REPORTS_AFTER_FAILED_END = 2;
constexpr uint8_t CS_EXPECTED_REPORTS_AFTER_END_OK = 2;

// Test-start parameter-surface loop.
constexpr int CS_TEST_START_VARIANT_COUNT = 6;
enum CsTestStartVariant {
    CS_TEST_VARIANT_INVALID_MAIN_MODE = 0,
    CS_TEST_VARIANT_INVALID_ROLE,
    CS_TEST_VARIANT_NONZERO_RESERVED,
    CS_TEST_VARIANT_OVERRIDE_TOO_SHORT,
    CS_TEST_VARIANT_INVALID_TIP2,
    CS_TEST_VARIANT_NULL_OVERRIDE_DATA
};

// Default values for the 0x0095 test prefix used by every CsAssertTestStartParameterSurface
// variant and the result-reassembly cases. They are all values 7.8.142 pp2843-2848 allows.
constexpr uint8_t CS_TEST_MAIN_MODE_REPETITION = 0x01;
constexpr uint8_t CS_TEST_MODE0_STEPS = 0x01;
constexpr uint8_t CS_TEST_CS_SYNC_PHY = 0x01;                 // LE 1M
constexpr uint8_t CS_TEST_CS_SYNC_ANTENNA_SELECTION = 0x01;   // antenna id 1
static const uint8_t CS_TEST_SUBEVENT_LEN[HCI_LE_24BIT_FIELD_LEN] = {0xE2, 0x04, 0x00}; // 1250 us
constexpr uint16_t CS_TEST_SUBEVENT_INTERVAL = 0x0000;
constexpr uint8_t CS_TEST_MAX_NUM_SUBEVENTS = 0x01;
constexpr int8_t CS_TEST_TRANSMIT_POWER_LEVEL = 0x00;
constexpr uint8_t CS_TEST_TIME_DEFAULT = 0x14;   // shared default for T_IP1/T_IP2/T_FCS/T_PM
constexpr uint8_t CS_TEST_T_IP1_TIME = CS_TEST_TIME_DEFAULT;
constexpr uint8_t CS_TEST_T_IP2_TIME = CS_TEST_TIME_DEFAULT;
constexpr uint8_t CS_TEST_T_FCS_TIME = CS_TEST_TIME_DEFAULT;
constexpr uint8_t CS_TEST_T_PM_TIME = CS_TEST_TIME_DEFAULT;
constexpr uint8_t CS_TEST_T_SW_TIME = 0x02;
constexpr uint8_t CS_TEST_T_IP2_TIME_ABOVE_MAX = 0x15;   // one past the 0x14 maximum
constexpr uint8_t CS_TEST_RESERVED_NONZERO = 0x01;       // Reserved field must be 0x00
constexpr uint8_t CS_TEST_OVERRIDE_LENGTH_TOO_SHORT = 0x01;  // minimum accepted is 2
constexpr uint8_t CS_ROLE_INVALID = 0x02;                // valid roles are 0x00 / 0x01
constexpr uint8_t CS_TEST_MAIN_MODE_INVALID = 0x00;      // valid main modes start at 0x01
constexpr uint8_t CS_TEST_TONE_ANTENNA_CONFIG_SELECTION = 0x00;
constexpr uint8_t CS_TEST_RESERVED = 0x00;
constexpr uint16_t CS_TEST_DRBG_NONCE = 0x0000;
constexpr uint8_t CS_TEST_CHANNEL_MAP_REPETITION = 0x01;
constexpr uint16_t CS_TEST_OVERRIDE_CONFIG = 0x0000;

// Common injection values shared across the result-reassembly cases.
constexpr uint16_t CS_INJECT_ACL_EVENT_COUNTER = 0x1234;
constexpr uint16_t CS_INJECT_PROCEDURE_COUNTER = 0x0042;
constexpr uint16_t CS_INJECT_FREQ_COMPENSATION = 0x000A;
constexpr uint8_t CS_INJECT_CHANNEL_FIRST = 0x05;
constexpr uint8_t CS_INJECT_CHANNEL_SECOND = 0x06;
constexpr uint8_t CS_INJECT_CHANNEL_THIRD = 0x07;
constexpr uint8_t CS_INJECT_CONFIG_ID = 0x02;

// Mode-0 step body field values.
constexpr uint8_t CS_TEST_PACKET_QUALITY = 0x0A;
constexpr uint8_t CS_TEST_PACKET_RSSI_WIRE = 0xEC;   // -20 dBm
constexpr uint8_t CS_TEST_PACKET_ANTENNA = 0x01;
constexpr int16_t CS_TEST_MEASURED_FREQ_OFFSET_UNAVAILABLE_RAW = 0x4000;

// Reference power level wire encoding used in the 0x31 head.
constexpr uint8_t CS_TEST_REFERENCE_POWER_WIRE = 0xF6;   // -10 dBm

// Common step counts (used for both Num_Steps and Num_Steps_Reported).
constexpr uint8_t CS_NUM_STEPS_NONE = 0x00;
constexpr uint8_t CS_NUM_STEPS_ONE = 0x01;
constexpr uint8_t CS_NUM_STEPS_TWO = 0x02;
constexpr uint8_t CS_NUM_STEPS_THREE = 0x03;
constexpr uint8_t CS_NUM_STEPS_OVER_MAX = 0xA1;  // one beyond the 160-step ceiling

// Common report counts / statuses.
constexpr uint8_t CS_TEST_CONFIG_ID_DEFAULT = 0x00;
constexpr uint8_t CS_TEST_ABORT_REASON_NONE = 0x00;
constexpr uint8_t CS_TEST_ABORT_REASON_SAMPLE = 0x02;
constexpr uint8_t CS_TEST_OVERSIZED_STEP_DATA_LEN = 0x40;
constexpr uint8_t CS_TEST_MODE3_BODY_BASE = 0x10;

// Expected decoded sub-fields for the rising pattern in FillMode3Body.
constexpr uint32_t CS_TEST_MODE3_PACKET_PCT1 = 0x19181716U;
constexpr uint32_t CS_TEST_MODE3_PACKET_PCT2 = 0x1D1C1B1AU;
constexpr uint8_t CS_TEST_MODE3_ANTENNA_PERMUTATION_PCT = 0x1E;
constexpr uint8_t CS_TEST_MODE3_ANTENNA_PERMUTATION_2 = 0x16;

// Sentinel / one-off test values.
constexpr uint8_t CS_STATUS_UNSET = 0xFF;
constexpr uint16_t CS_NON_TEST_HANDLE = 0x0042;
constexpr uint8_t CS_STATUS_TOO_FEW_USABLE_CHANNELS = HCI_INSUFFICIENT_CHANNELS;

// Sounding / not-sounding values passed as the RTT_Type selector in the test cases.
constexpr uint8_t CS_SOUNDING_DISABLED = 0x00;
constexpr uint8_t CS_SOUNDING_ENABLED = 0x01;

// The step records of one event, in the order the Controller sends them. An append that
// would not fit is refused rather than truncated, so an over-long drive shows up as a
// report with fewer steps instead of as a write past the buffer.
struct CsStepBlob {
    uint8_t bytes[CS_STEP_BLOB_CAP] = {};
    size_t used = 0;

    // One record: Step_Mode, Step_Channel, Step_Data_Length, then that step's own Step_Data
    // (7.7.65,44 p2451). A step the Controller sent with no data is three octets.
    void Append(uint8_t mode, uint8_t channel, const uint8_t *data, size_t length)
    {
        if (used + CS_STEP_RECORD_LEN + length > sizeof(bytes)) {
            return;
        }
        bytes[used++] = mode;
        bytes[used++] = channel;
        bytes[used++] = static_cast<uint8_t>(length);
        if (length != 0 && data != nullptr) {
            ASSERT_EQ(memcpy_s(bytes + used, sizeof(bytes) - used, data, length), EOK);
            used += length;
        }
    }

    // A record whose Step_Data_Length claims more octets than follow it, for the case where
    // the event ends inside a step (p2447 allows a step whose data the Controller cut short).
    void AppendOversized(uint8_t mode, uint8_t channel, uint8_t claimed, const uint8_t *data,
        size_t have)
    {
        if (used + CS_STEP_RECORD_LEN + have > sizeof(bytes)) {
            return;
        }
        bytes[used++] = mode;
        bytes[used++] = channel;
        bytes[used++] = claimed;
        if (memcpy_s(bytes + used, sizeof(bytes) - used, data, have) != EOK) {
            ADD_FAILURE() << "AppendOversized memcpy_s failed";
            return;
        }
        used += have;
    }

    // `count` records of three octets, each a step the Controller sent with no data at all.
    void AppendEmpty(uint8_t mode, uint8_t channel, int count)
    {
        for (int i = 0; i < count; i++) {
            Append(mode, channel, nullptr, 0);
        }
    }
};

// Writes one subevent frame and hands it to the event layer. Length counts the Subevent_Code
// and everything behind it, which is what the dispatcher subtracts before the handler sees
// the body (hci_evt_le.c:1904), so the handler's param starts right after the subevent code
// and its step blob is exactly what was appended here. The packet carries the two header
// octets plus that body - the receive layer strips the header and then refuses anything whose
// remainder differs from Parameter_Total_Length (hci_evt.c:2032), so the cap-sized array
// itself would be dropped for its zero tail.
static void InjectCsSubeventFrame(uint8_t subeventCode, const void *head, size_t headLen,
    const CsStepBlob &steps)
{
    uint8_t wire[LE_META_EVT_HEAD_LEN + CS_SUBEVENT_HEAD_CAP + CS_STEP_BLOB_CAP] = {0};
    const size_t bodyLen = 1 + headLen + steps.used;
    // Event_Code and Length, then the body: the one length both the Length octet and the
    // packet handed over are built from.
    const size_t wireLen = LE_META_EVT_PREFIX_LEN + bodyLen;
    // Refuse rather than truncate: a body past 0xFF would wrap the Length octet, and one
    // past the array would leave InjectWire reading beyond wire[].
    if (bodyLen > 0xFF || wireLen > sizeof(wire)) {
        return;
    }
    wire[LE_META_EVT_EVENT_CODE_OFFSET] = HCI_LE_META_EVENT;   // Event_Code: LE Meta
    wire[LE_META_EVT_LENGTH_OFFSET] = static_cast<uint8_t>(bodyLen);
    wire[LE_META_EVT_SUBEVENT_CODE_OFFSET] = subeventCode;
    if (memcpy_s(wire + LE_META_EVT_HEAD_LEN, sizeof(wire) - LE_META_EVT_HEAD_LEN, head, headLen) != EOK) {
        ADD_FAILURE() << "InjectCsSubeventFrame head memcpy_s failed";
        return;
    }
    if (memcpy_s(wire + LE_META_EVT_HEAD_LEN + headLen,
        sizeof(wire) - LE_META_EVT_HEAD_LEN - headLen, steps.bytes, steps.used) != EOK) {
        ADD_FAILURE() << "InjectCsSubeventFrame steps memcpy_s failed";
        return;
    }
    InjectWire(wire, wireLen);
}

// The constant part of a 0x31 head. The two counters and the compensation are given pairwise
// distinct values so a swap between them shows up below; the four fields that decide the
// outcome are the caller's.
static HciLeCsSubeventResultEventParam CsResultHead(
    uint8_t procedureDone, uint8_t subeventDone, uint8_t numAntennaPaths, uint8_t numStepsReported)
{
    HciLeCsSubeventResultEventParam head = {};
    head.connectionHandle = LE_CS_TEST_HANDLE;
    head.configId = CS_TEST_CONFIG_ID_DEFAULT;
    head.startAclConnEventCounter = CS_INJECT_ACL_EVENT_COUNTER;
    head.procedureCounter = CS_INJECT_PROCEDURE_COUNTER;
    head.frequencyCompensation = CS_INJECT_FREQ_COMPENSATION;
    head.referencePowerLevel = CS_TEST_REFERENCE_POWER_WIRE;   // -10 dBm
    head.procedureDoneStatus = procedureDone;
    head.subeventDoneStatus = subeventDone;
    head.abortReason = CS_TEST_ABORT_REASON_NONE;
    head.numAntennaPaths = numAntennaPaths;
    head.numStepsReported = numStepsReported;
    return head;
}

// A 0x32 head carries no counters, no frequency compensation and no power level (p2459):
// the report keeps the opening 0x31's.
static HciLeCsSubeventResultContinueEventParam CsContinueHead(
    uint8_t procedureDone, uint8_t subeventDone, uint8_t numAntennaPaths, uint8_t numStepsReported)
{
    HciLeCsSubeventResultContinueEventParam head = {};
    head.connectionHandle = LE_CS_TEST_HANDLE;
    head.configId = CS_TEST_CONFIG_ID_DEFAULT;
    head.procedureDoneStatus = procedureDone;
    head.subeventDoneStatus = subeventDone;
    head.abortReason = CS_TEST_ABORT_REASON_NONE;
    head.numAntennaPaths = numAntennaPaths;
    head.numStepsReported = numStepsReported;
    return head;
}

// A mode-3 step body of `length` octets filled with a rising pattern, so a decoded sub-field
// can be told from its neighbour: six head octets, the two Packet_PCT values when they belong
// in it, the permutation index, then four octets per tone.
static void FillMode3Body(uint8_t *body, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        body[i] = static_cast<uint8_t>(CS_TEST_MODE3_BODY_BASE + i);
    }
}

// The 0x0095 prefix every drive in the 8.3 CS cases starts from, so a case that varies one
// field varies it against the same shape the accepted ones have. Every value is one that
// 7.8.142 pp2843-2848 allows: the enumerated interlude and switch timings, the shortest
// Subevent_Len the field takes, and a two-octet override tail, which is the shortest tail the
// command accepts whether or not an override bit is set.
static const uint8_t CS_TEST_OVERRIDE_TAIL[LE_CS_TEST_OVERRIDE_MIN] = {0x00, 0x00};

static void FillCsLeTestParam(uint8_t role, uint8_t rttType, uint8_t mainModeType,
    CsLeTestParam *param)
{
    *param = CsLeTestParam{};
    param->prefix.mainModeType = mainModeType;
    param->prefix.subModeType = LE_CS_SUB_MODE_UNUSED;
    param->prefix.mainModeRepetition = CS_TEST_MAIN_MODE_REPETITION;
    // Checked whether or not a mode-0 step is driven, so it is 0x01 for every group here.
    param->prefix.mode0Steps = CS_TEST_MODE0_STEPS;
    param->prefix.role = role;
    param->prefix.rttType = rttType;
    param->prefix.csSyncPhy = CS_TEST_CS_SYNC_PHY;
    param->prefix.csSyncAntennaSelection = CS_TEST_CS_SYNC_ANTENNA_SELECTION;
    ASSERT_EQ(memcpy_s(param->prefix.subeventLen, sizeof(param->prefix.subeventLen),
        CS_TEST_SUBEVENT_LEN, sizeof(CS_TEST_SUBEVENT_LEN)), EOK);
    param->prefix.subeventInterval = CS_TEST_SUBEVENT_INTERVAL;
    param->prefix.maxNumSubevents = CS_TEST_MAX_NUM_SUBEVENTS;
    param->prefix.transmitPowerLevel = CS_TEST_TRANSMIT_POWER_LEVEL;
    param->prefix.tIp1Time = CS_TEST_T_IP1_TIME;
    param->prefix.tIp2Time = CS_TEST_T_IP2_TIME;
    param->prefix.tFcsTime = CS_TEST_T_FCS_TIME;
    param->prefix.tPmTime = CS_TEST_T_PM_TIME;
    param->prefix.tSwTime = CS_TEST_T_SW_TIME;
    param->prefix.toneAntennaConfigSelection = CS_TEST_TONE_ANTENNA_CONFIG_SELECTION;
    param->prefix.reserved = CS_TEST_RESERVED;
    param->prefix.snrControlInitiator = LE_CS_SNR_CONTROL_NONE;
    param->prefix.snrControlReflector = LE_CS_SNR_CONTROL_NONE;
    param->prefix.drbgNonce = CS_TEST_DRBG_NONCE;
    param->prefix.channelMapRepetition = CS_TEST_CHANNEL_MAP_REPETITION;
    param->prefix.overrideConfig = CS_TEST_OVERRIDE_CONFIG;
    param->prefix.overrideParametersLength = LE_CS_TEST_OVERRIDE_MIN;
    param->overrideParametersData = CS_TEST_OVERRIDE_TAIL;
}

// Starts a CS test through the public 0x0095 entry, then commits it with the Command_Complete
// the Controller would return: Status 0x00 is what makes the test running with this Role and
// RTT_Type, and it is those two that the steps of the next 0x31 are decoded against
// (cs_le_if.h on CsLeTestParam; cs_defs.h on CsSubeventReport.role). A second call replaces the
// Role and RTT_Type of a test already running, which is how the 8.3 cases change the decode
// context without ending the test.
// A start the Controller refuses (0x0C, a test already enabled) is driven through the same
// helper: the engine still hands the command over, so the call itself succeeds and the refusal
// is read off the sink instead of the return value.
static bool StartCsLeTest(uint8_t role, uint8_t rttType, uint8_t mainModeType,
    uint8_t completeStatus = HCI_SUCCESS)
{
    CsLeTestParam param = {};
    FillCsLeTestParam(role, rttType, mainModeType, &param);

    EXPECT_EQ(CSIF_LeCsTestStart(&param), BT_SUCCESS) <<
        "the 0x0095 was refused by the engine before anything reached the wire";

    g_csLeTestStatus = CS_STATUS_UNSET;  // so a completion that never arrives is not read as the last one
    HciLeCsTestReturnParam startReturn = {};
    startReturn.status = completeStatus;
    HciEventOnLeCommandComplete(HCI_LE_CS_TEST, &startReturn, sizeof(startReturn));
    EXPECT_EQ(g_csLeTestStatus, completeStatus) << "the 0x0095 outcome did not reach the sink";
    return g_csLeTestStatus == completeStatus;
}

// The CS parse table is registered per case here, not once in SetUpTestCase: BTM_Enable
// runs BtmEnableLeAndSharedModules (btm.c:305) -> HCI_Initialize -> HciInitEvent
// (hci_evt.c:48), which replaces g_eventCallbackList with a fresh empty one and drops
// every table registered before it. The module tables come back because each module's
// enable path re-registers; this file's observer does not. Cases 02002 and 02202
// re-enable LE from inside this fixture, so a registration made once in SetUpTestCase
// is already gone by the time 02303 - the one case here that reads g_csTally - runs.

// RAII guard that records whether LE is enabled on construction and restores both
// the controller and the OBSERVER|CENTRAL role if a test disabled them. Mirrors the
// DefaultDataLengthRestorer pattern in stack_gap_le_test.cpp.
class LeEnableRestorer {
public:
    LeEnableRestorer() : wasEnabled_(BTM_IsEnabled(LE_CONTROLLER)) {}
    ~LeEnableRestorer()
    {
        if (!wasEnabled_ || BTM_IsEnabled(LE_CONTROLLER)) {
            return;
        }
        if (BTM_Enable(LE_CONTROLLER) != BT_SUCCESS) {
            printf("LeEnableRestorer: failed to re-enable LE controller\n");
            return;
        }
        if (GAPIF_LeSetRole(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL) != BT_SUCCESS) {
            printf("LeEnableRestorer: failed to restore role\n");
        }
    }
    LeEnableRestorer(const LeEnableRestorer &) = delete;
    LeEnableRestorer &operator=(const LeEnableRestorer &) = delete;

private:
    bool wasEnabled_;
};

void StackGapLe60BtmTest::SetUpTestCase(void)
{
    ASSERT_EQ(BTM_Initialize(), BT_SUCCESS);
    ASSERT_EQ(BTM_Enable(LE_CONTROLLER), BT_SUCCESS);
    ASSERT_TRUE(BTM_IsEnabled(LE_CONTROLLER));
    ASSERT_EQ(GAPIF_LeSetRole(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL), BT_SUCCESS);
    InitCsParseCallbacks();
}

void StackGapLe60BtmTest::TearDownTestCase(void)
{
    if (BTM_IsEnabled(LE_CONTROLLER)) {
        EXPECT_EQ(BTM_Disable(LE_CONTROLLER), BT_SUCCESS);
    }
    EXPECT_EQ(BTM_Close(), BT_SUCCESS);
}

void StackGapLe60BtmTest::SetUp(void)
{
    // Fail loudly if a previous case left LE disabled, instead of silently
    // skipping registration and letting the next case fail for the wrong reason.
    ASSERT_TRUE(BTM_IsEnabled(LE_CONTROLLER));
    ASSERT_EQ(HCI_RegisterEventCallbacks(&g_csParseCallbacks), BT_SUCCESS);
}

void StackGapLe60BtmTest::TearDown(void)
{
    // Cases that disable LE are required to restore it (see LeEnableRestorer
    // below); guard only against the lock being destroyed if that contract breaks.
    if (BTM_IsEnabled(LE_CONTROLLER)) {
        EXPECT_EQ(HCI_DeregisterEventCallbacks(&g_csParseCallbacks), BT_SUCCESS);
    }
}

// @tc.number: StackGapLe6_0_BtmGateNotSupported_02000
// @tc.name:  MA commands are rejected NOT_SUPPORT when the local bit 64 is clear
// @tc.desc:  8.2(3): GapLeMonitoredAdvCommandGate (Vol 6, Part B, 4.6.45, p3151)
//            checks LE enabled, then role, then bit 64. On a pre-6.0 controller all
//            five entry points return GAP_ERR_NOT_SUPPORT before any HCI command is
//            built; a bit-64 board cannot reproduce this, so it is skipped.
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_BtmGateNotSupported_02000, TestSize.Level1)
{
    if (BTM_IsControllerSupportLeMonitoringAdvertisers()) {
        GTEST_SKIP() << "local controller exposes LE feature bit 64: NOT_SUPPORT unreachable";
    }
    BtAddr addr = MaDummyAddr(BT_PUBLIC_DEVICE_ADDRESS);

    EXPECT_EQ(GAPIF_LeAddDeviceToMonitoredAdvertisersList(
        BT_PUBLIC_DEVICE_ADDRESS, &addr, MA_RSSI_LOW, MA_RSSI_HIGH, MA_LOSS_TIMEOUT_SEC), GAP_ERR_NOT_SUPPORT);
    EXPECT_EQ(GAPIF_LeRemoveDeviceFromMonitoredAdvertisersList(BT_PUBLIC_DEVICE_ADDRESS, &addr), GAP_ERR_NOT_SUPPORT);
    EXPECT_EQ(GAPIF_LeClearMonitoredAdvertisersList(), GAP_ERR_NOT_SUPPORT);
    EXPECT_EQ(GAPIF_LeReadMonitoredAdvertisersListSize(), GAP_ERR_NOT_SUPPORT);
    EXPECT_EQ(GAPIF_LeEnableMonitoringAdvertisers(0x00), GAP_ERR_NOT_SUPPORT);
}

// @tc.number: StackGapLe6_0_BtmGateInvalState_02001
// @tc.name:  MA commands need an OBSERVER|CENTRAL role: narrowing roles is INVAL_STATE
// @tc.desc:  Gate order (8.2(3)): with the role narrowed to BROADCASTER|PERIPHERAL
//            all five entry points return GAP_ERR_INVAL_STATE on any controller.
//            Note GAP_ERR_INVAL_STATE == GAP_ERR_NOT_ENABLE == BT_BAD_STATUS: the
//            value is shared (gap_comm.h, services-era baseline), the configuration
//            that produced it distinguishes the branch.
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_BtmGateInvalState_02001, TestSize.Level1)
{
    ASSERT_EQ(GAPIF_LeSetRole(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL), BT_SUCCESS);
    BtAddr addr = MaDummyAddr(BT_PUBLIC_DEVICE_ADDRESS);

    EXPECT_EQ(GAPIF_LeAddDeviceToMonitoredAdvertisersList(
        BT_PUBLIC_DEVICE_ADDRESS, &addr, MA_RSSI_LOW, MA_RSSI_HIGH, MA_LOSS_TIMEOUT_SEC), GAP_ERR_INVAL_STATE);
    EXPECT_EQ(GAPIF_LeRemoveDeviceFromMonitoredAdvertisersList(BT_PUBLIC_DEVICE_ADDRESS, &addr), GAP_ERR_INVAL_STATE);
    EXPECT_EQ(GAPIF_LeClearMonitoredAdvertisersList(), GAP_ERR_INVAL_STATE);
    EXPECT_EQ(GAPIF_LeReadMonitoredAdvertisersListSize(), GAP_ERR_INVAL_STATE);
    EXPECT_EQ(GAPIF_LeEnableMonitoringAdvertisers(0x00), GAP_ERR_INVAL_STATE);

    EXPECT_EQ(GAPIF_LeSetRole(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL), BT_SUCCESS);
}

// @tc.number: StackGapLe6_0_BtmGateNotEnabled_02002
// @tc.name:  MA commands need LE enabled: the LE-off state returns NOT_ENABLE
// @tc.desc:  GapLeMonitoredAdvCommandGate checks GapIsLeEnable first (8.2(3)).
//            BTM_Disable(LE_CONTROLLER) runs the module shutdown path that clears
//            g_gapMng.le.isEnable, reproducing the state; the controller is
//            re-enabled before the case returns so the fixture stays usable.
//            A NULL-addr Add probes the gate and never queues HCI: if the LE
//            disable did not clear the flag (dual-mode boot), the case skips.
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_BtmGateNotEnabled_02002, TestSize.Level1)
{
    ASSERT_TRUE(BTM_IsEnabled(LE_CONTROLLER));
    // Ensure LE is restored even if an assertion fails after the disable below.
    LeEnableRestorer leRestorer;
    ASSERT_EQ(BTM_Disable(LE_CONTROLLER), BT_SUCCESS);
    if (BTM_IsEnabled(LE_CONTROLLER)) {
        // Disable is deferred/asynchronous on this harness: NOT_ENABLE cannot be
        // reproduced deterministically, so restore and skip the group.
        EXPECT_EQ(BTM_Enable(LE_CONTROLLER), BT_SUCCESS);
        GTEST_SKIP() << "LE controller disable is deferred; cannot reproduce NOT_ENABLE deterministically";
    }
    // Probe with a NULL-addr Add: gate-rejected (NOT_ENABLE) or fast-check
    // rejected (INVAL_PARAM), so it is never queued to HCI.
    if (GAPIF_LeAddDeviceToMonitoredAdvertisersList(
            BT_PUBLIC_DEVICE_ADDRESS, nullptr, MA_RSSI_LOW, MA_RSSI_HIGH,
            MA_LOSS_TIMEOUT_SEC) != GAP_ERR_NOT_ENABLE) {
        EXPECT_EQ(BTM_Enable(LE_CONTROLLER), BT_SUCCESS);
        EXPECT_TRUE(BTM_IsEnabled(LE_CONTROLLER));
        EXPECT_EQ(GAPIF_LeSetRole(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL), BT_SUCCESS);
        GTEST_SKIP() << "GAP LE-enable flag not cleared by LE disable; cannot reproduce NOT_ENABLE";
    }
    BtAddr addr = MaDummyAddr(BT_PUBLIC_DEVICE_ADDRESS);

    EXPECT_EQ(GAPIF_LeAddDeviceToMonitoredAdvertisersList(
        BT_PUBLIC_DEVICE_ADDRESS, &addr, MA_RSSI_LOW, MA_RSSI_HIGH, MA_LOSS_TIMEOUT_SEC), GAP_ERR_NOT_ENABLE);
    EXPECT_EQ(GAPIF_LeRemoveDeviceFromMonitoredAdvertisersList(BT_PUBLIC_DEVICE_ADDRESS, &addr), GAP_ERR_NOT_ENABLE);
    EXPECT_EQ(GAPIF_LeClearMonitoredAdvertisersList(), GAP_ERR_NOT_ENABLE);
    EXPECT_EQ(GAPIF_LeReadMonitoredAdvertisersListSize(), GAP_ERR_NOT_ENABLE);
    EXPECT_EQ(GAPIF_LeEnableMonitoringAdvertisers(0x00), GAP_ERR_NOT_ENABLE);

    ASSERT_EQ(BTM_Enable(LE_CONTROLLER), BT_SUCCESS);
    ASSERT_TRUE(BTM_IsEnabled(LE_CONTROLLER));
    EXPECT_EQ(GAPIF_LeSetRole(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL), BT_SUCCESS);
}

// @tc.number: StackGapLe6_0_BtmRegisterSlot_02003
// @tc.name:  the MA callback slot is single: register replaces, deregister silences
// @tc.desc:  8.2(4): register A -> its Add completion arrives; register B replaces
//            A and only B sees the next completion; after a deregister no
//            completion is delivered at all (silent no-op, GapLe*Complete finds a
//            NULL slot). Needs local bit 64 for a real Command_Complete, so a
//            pre-6.0 controller skips.
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_BtmRegisterSlot_02003, TestSize.Level1)
{
    if (!BTM_IsControllerSupportLeMonitoringAdvertisers()) {
        GTEST_SKIP() << "local controller lacks LE feature bit 64 (Monitoring Advertisers)";
    }
    BtAddr addr = MaDummyAddr(BT_PUBLIC_DEVICE_ADDRESS);

    MaCtx ctxA;
    MaCallbackGuard guardA;
    if (!MaRegister(&ctxA, guardA)) {
        GTEST_SKIP() << "could not register MA callback slot A";
    }
    ASSERT_TRUE(MaSendAdd(&ctxA, BT_PUBLIC_DEVICE_ADDRESS, &addr)) << "no addResult for slot A";
    EXPECT_EQ(ctxA.add.status, 0x00);

    // Replace A with B: the slot is single, so the next completion goes to B only.
    MaCtx ctxB;
    MaCallbackGuard guardB;
    if (!MaRegister(&ctxB, guardB)) {
        GTEST_SKIP() << "could not register MA callback slot B";
    }
    ctxA.add.waiter.Reset();
    ctxA.add.status = 0xFF;
    ASSERT_TRUE(MaSendAdd(&ctxB, BT_PUBLIC_DEVICE_ADDRESS, &addr)) << "no addResult for replacement slot B";
    EXPECT_EQ(ctxB.add.status, 0x00);
    EXPECT_FALSE(ctxA.add.waiter.Wait(MA_QUIET_MS)) << "replaced slot A still received a completion";

    // Deregister clears the slot: the last Add completes on the controller but no
    // host callback is invoked (the module consumer finds a NULL slot and skips).
    // Guard only: the clear returns GAP_SUCCESS unconditionally (gap_le_scan.c:886) - no
    // verification power; the no-completion invariant below is the check that it cleared.
    EXPECT_EQ(GAPIF_DeregisterMonitoredAdvertisersCallback(), BT_SUCCESS);
    ctxB.add.waiter.Reset();
    ctxB.add.status = 0xFF;
    ASSERT_EQ(GAPIF_LeAddDeviceToMonitoredAdvertisersList(
        BT_PUBLIC_DEVICE_ADDRESS, &addr, MA_RSSI_LOW, MA_RSSI_HIGH, MA_LOSS_TIMEOUT_SEC), BT_SUCCESS);
    EXPECT_FALSE(ctxB.add.waiter.Wait(MA_QUIET_MS)) << "completion delivered with no registered slot";
}

// @tc.number: StackGapLe6_0_BtmCommands_02004
// @tc.name:  MA host fast checks reject invalid parameters; the list flow succeeds
// @tc.desc:  8.2(5): after the bit-64 gate the module runs its own INVAL_PARAM fast
//            checks, then Add/Read Size/Remove/Clear/Enable each complete over real
//            HCI with a Status of 0x00. Controller-local list operations need no
//            peer; the over-air 0x34 loss/recover part stays in the 8.3 checks.
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_BtmCommands_02004, TestSize.Level1)
{
    if (!BTM_IsControllerSupportLeMonitoringAdvertisers()) {
        GTEST_SKIP() << "local controller lacks LE feature bit 64 (Monitoring Advertisers)";
    }
    BtAddr dev = MaDummyAddr(BT_PUBLIC_DEVICE_ADDRESS);
    BtAddr devRandom = MaDummyAddr(BT_RANDOM_DEVICE_ADDRESS);

    // Host-side fast checks, each returning INVAL_PARAM without queuing an HCI
    // command: NULL addr / addrType outside {0,1} / addrType != addr->type /
    // High < Low / Timeout 0x00 (Vol 4, Part E, 7.8.146-7.8.149).
    EXPECT_EQ(GAPIF_LeAddDeviceToMonitoredAdvertisersList(
        BT_PUBLIC_DEVICE_ADDRESS, nullptr, MA_RSSI_LOW, MA_RSSI_HIGH, MA_LOSS_TIMEOUT_SEC), GAP_ERR_INVAL_PARAM);
    EXPECT_EQ(GAPIF_LeAddDeviceToMonitoredAdvertisersList(
        0x02, &dev, MA_RSSI_LOW, MA_RSSI_HIGH, MA_LOSS_TIMEOUT_SEC), GAP_ERR_INVAL_PARAM);
    EXPECT_EQ(GAPIF_LeAddDeviceToMonitoredAdvertisersList(
        BT_PUBLIC_DEVICE_ADDRESS, &devRandom, MA_RSSI_LOW, MA_RSSI_HIGH, MA_LOSS_TIMEOUT_SEC), GAP_ERR_INVAL_PARAM);
    EXPECT_EQ(GAPIF_LeAddDeviceToMonitoredAdvertisersList(
        BT_PUBLIC_DEVICE_ADDRESS, &dev, MA_RSSI_HIGH, MA_RSSI_LOW, MA_LOSS_TIMEOUT_SEC), GAP_ERR_INVAL_PARAM);
    EXPECT_EQ(GAPIF_LeAddDeviceToMonitoredAdvertisersList(
        BT_PUBLIC_DEVICE_ADDRESS, &dev, MA_RSSI_LOW, MA_RSSI_HIGH, 0x00), GAP_ERR_INVAL_PARAM);
    EXPECT_EQ(GAPIF_LeEnableMonitoringAdvertisers(0x02), GAP_ERR_INVAL_PARAM);

    MaCtx ctx;
    MaCallbackGuard guard;
    if (!MaRegister(&ctx, guard)) {
        GTEST_SKIP() << "could not register MA callback slot";
    }

    ASSERT_TRUE(MaSendClear(&ctx)) << "no clearResult";
    EXPECT_EQ(ctx.clear.status, 0x00);

    ASSERT_TRUE(MaSendAdd(&ctx, BT_PUBLIC_DEVICE_ADDRESS, &dev)) << "no addResult";
    EXPECT_EQ(ctx.add.status, 0x00);

    ASSERT_TRUE(MaSendReadSize(&ctx)) << "no readMonitoredAdvertisersListSizeResult";
    EXPECT_EQ(ctx.read.status, 0x00);

    ASSERT_TRUE(MaSendRemove(&ctx, BT_PUBLIC_DEVICE_ADDRESS, &dev)) << "no removeResult";
    EXPECT_EQ(ctx.remove.status, 0x00);

    ASSERT_TRUE(MaSendClear(&ctx)) << "no clearResult after remove";
    EXPECT_EQ(ctx.clear.status, 0x00);

    // Re-add an in-list entry, then drive Enable to 1 and back to 0 (7.8.149).
    ASSERT_TRUE(MaSendAdd(&ctx, BT_PUBLIC_DEVICE_ADDRESS, &dev)) << "no re-add addResult";
    EXPECT_EQ(ctx.add.status, 0x00);

    ASSERT_TRUE(MaSendEnable(&ctx, 0x01)) << "no enableResult (enable)";
    EXPECT_EQ(ctx.enable.status, 0x00);

    ASSERT_TRUE(MaSendEnable(&ctx, 0x00)) << "no enableResult (disable)";
    EXPECT_EQ(ctx.enable.status, 0x00);
}

// @tc.number: StackGapLe6_0_DbafGateNotSupported_02400
// @tc.name:  the DBAF entries are rejected NOT_SUPPORT when the local bit 42 is clear
// @tc.desc:  Round 4 F11: the six production gates that answer a DBAF request on a
//            controller without LE feature bit 42 (gap_le_scan.c:685 and :804,
//            gap_le_adv.c:280 and :679, btm_acl.c:999 and :1040) had no case of their own -
//            the interact cases that reach them GTEST_SKIP on exactly this board, so the
//            branch was skipped rather than verified, unlike the MA (02000) and FSU (02200)
//            gates. Seven exported entries reach those six points: both extended-adv
//            parameter setters ([v1] and [v2]) run through gap_le_adv.c:280, which is why
//            seven and not six. On a bit-42 controller the state cannot be reproduced, so
//            the case skips, mirroring 02000 on the other side.
//            Every argument below passes the checks that precede its gate, so what is
//            asserted is the capability gate and not an earlier fast check. The five GAP
//            results and the two BTM results are the same value (GAP_ERR_NOT_SUPPORT is
//            BT_NOT_SUPPORT, gap_comm.h:35); each is written as the layer that returned it.
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_DbafGateNotSupported_02400, TestSize.Level1)
{
    if (BTM_IsControllerSupportLeDecisionBasedAdvertisingFiltering()) {
        GTEST_SKIP() << "local controller exposes LE feature bit 42: NOT_SUPPORT unreachable";
    }

    // The extended-advertising entry points require BROADCASTER|PERIPHERAL, while
    // the scanning/connection entry points require OBSERVER|CENTRAL. Use all four
    // roles so the role gate passes and the DBAF capability gate is what fails.
    ASSERT_EQ(GAPIF_LeSetRole(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL |
                              GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL),
        BT_SUCCESS);

    // 0x0041 LE Set Extended Scanning Parameters (gap_le_scan.c:685): decision mode 0b01
    // in bits 2-3, one PHY, the same 10 ms interval/window the interact sweeps use.
    GapLeScanParam scanParam = {0x00, {0x0010, 0x0010}};
    EXPECT_EQ(GAPIF_LeExScanSetParam(LE_SCAN_DECISION_MODE_ALL_PDUS, GAP_EX_SCAN_PHY_1M, &scanParam),
        GAP_ERR_NOT_SUPPORT);

    // 0x0081 LE Set Decision Instructions (gap_le_scan.c:804): one record, group-opening
    // flag set, carrying the 2-octet Arbitrary Data vector the interact cases install.
    HciLeDecisionTestParam test = {};
    test.flags = LE_DECISION_TEST_FLAG_NEW_GROUP | LE_DECISION_TEST_FLAG_PASS_IF_CHECK;
    test.field = 0x12;  // Test_Field: Arbitrary Data, exactly 2 octets (7.8.145, p2865)
    test.parameters[0] = 0xFF;  // Mask_0/1
    test.parameters[1] = 0xFF;
    test.parameters[8] = 0x62;  // Target_0/1
    test.parameters[9] = 0x74;
    EXPECT_EQ(GAPIF_LeSetDecisionInstructions(1, &test), GAP_ERR_NOT_SUPPORT);

    // 0x0036 [v1] and 0x007F [v2] LE Set Extended Advertising Parameters
    // (gap_le_adv.c:280, shared): decision PDU bit only, which no earlier property check
    // rejects. Both are driven because they are two entries onto one gate.
    GapLeExAdvParam advParam = {};
    advParam.advIntervalMin = 0x000020;
    advParam.advIntervalMax = 0x000020;
    advParam.advChannelMap = 0x07;
    advParam.primaryAdvPhy = GAP_LE_PHY_1M;
    advParam.secondaryAdvPhy = GAP_LE_PHY_1M;
    EXPECT_EQ(GAPIF_LeExAdvSetParam(
        0x00, LE_EXT_ADV_EVENT_PROP_DECISION_PDU, 0x00, advParam), GAP_ERR_NOT_SUPPORT);

    GapExAdvParamV2 advParamV2 = {};
    advParamV2.advHandle = 0x00;
    advParamV2.properties = LE_EXT_ADV_EVENT_PROP_DECISION_PDU;
    advParamV2.advExParam = advParam;
    EXPECT_EQ(GAPIF_LeExAdvSetParamV2(&advParamV2), GAP_ERR_NOT_SUPPORT);

    // 0x0080 LE Set Decision Data (gap_le_adv.c:679): the gate precedes both the role check
    // and the sender's own field checks.
    const uint8_t decisionData[2] = {0x62, 0x74};
    EXPECT_EQ(GAPIF_LeExAdvSetDecisionData(0x00, 0x00, sizeof(decisionData), decisionData),
        GAP_ERR_NOT_SUPPORT);

    // 7.8.66 extended create connection by decision (btm_acl.c:999) and 7.8.13's cancel of
    // it (btm_acl.c:1040): both answer from the btm layer.
    EXPECT_EQ(GAPIF_LeExtendedConnectByDecision(LE_INITIATOR_FILTER_POLICY_DECISION_PDU_ONLY),
        BT_NOT_SUPPORT);
    EXPECT_EQ(GAPIF_LeExtendedConnectByDecisionCancel(), BT_NOT_SUPPORT);

    EXPECT_EQ(GAPIF_LeSetRole(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL), BT_SUCCESS);
}

// ---- 8.2 Frame Space Update GAP gates (heavy fixture, plan 8.2) -------------
// GAP_LeFrameSpaceUpdate walks addr -> LE enabled -> role (CENTRAL|PERIPHERAL,
// any-of) -> local bit 65 -> parameter boundaries -> connection lookup. The
// fixture role OBSERVER|CENTRAL already carries CENTRAL, so the role gate passes
// and the bit-65 state alone decides NOT_SUPPORT vs the later checks. The command
// targets one ACL, so the wire round trip stays in the 8.4 two-device checks.
namespace {

BtAddr FsuDummyAddr()
{
    BtAddr addr = {};
    addr.type = BT_PUBLIC_DEVICE_ADDRESS;
    for (int i = 0; i < BT_ADDRESS_SIZE; i++) {
        addr.addr[i] = static_cast<uint8_t>(0xC0 + i);
    }
    return addr;
}
}  // namespace

// @tc.number: StackGapLe6_0_FsuBtmGateNotSupported_02200
// @tc.name:  0x009D host command is rejected NOT_SUPPORT when local bit 65 is clear
// @tc.desc:  with LE enabled and a CENTRAL-capable role, a controller without LE
//            Frame Space Update (bit 65, page 1) fails the feature gate before any
//            boundary or HCI work. A bit-65 board cannot reproduce it, so skip.
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_FsuBtmGateNotSupported_02200, TestSize.Level1)
{
    if (BTM_IsControllerSupportLeFrameSpaceUpdate()) {
        GTEST_SKIP() << "local controller exposes LE feature bit 65: NOT_SUPPORT unreachable";
    }
    BtAddr addr = FsuDummyAddr();
    EXPECT_EQ(GAPIF_LeFrameSpaceUpdate(&addr, 0x0064, 0x00C8, 0x01, 0x01), GAP_ERR_NOT_SUPPORT);
}

// @tc.number: StackGapLe6_0_FsuBtmGateInvalState_02201
// @tc.name:  0x009D needs a CENTRAL or PERIPHERAL role: narrowing roles is INVAL_STATE
// @tc.desc:  the role check precedes the feature gate, so this fires on any
//            controller. A role without CENTRAL/PERIPHERAL fails the any-of role
//            test (GapLeRolesCheck); the fixture role OBSERVER|CENTRAL is restored
//            before returning. INVAL_STATE shares BT_BAD_STATUS with NOT_ENABLE;
//            the enabled-state configuration distinguishes the branch.
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_FsuBtmGateInvalState_02201, TestSize.Level1)
{
    ASSERT_EQ(GAPIF_LeSetRole(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_OBSERVER), BT_SUCCESS);
    BtAddr addr = FsuDummyAddr();
    EXPECT_EQ(GAPIF_LeFrameSpaceUpdate(&addr, 0x0064, 0x00C8, 0x01, 0x01), GAP_ERR_INVAL_STATE);
    EXPECT_EQ(GAPIF_LeSetRole(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL), BT_SUCCESS);
}

// @tc.number: StackGapLe6_0_FsuBtmGateNotEnabled_02202
// @tc.name:  0x009D needs LE enabled: the LE-off state returns NOT_ENABLE
// @tc.desc:  the command gate checks GapIsLeEnable first. BTM_Disable(LE_CONTROLLER)
//            runs the shutdown path that clears g_gapMng.le.isEnable, reproducing
//            the state; the controller and the OBSERVER|CENTRAL role are restored
//            before returning.
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_FsuBtmGateNotEnabled_02202, TestSize.Level1)
{
    ASSERT_TRUE(BTM_IsEnabled(LE_CONTROLLER));
    // Ensure LE is restored even if an assertion fails after the disable below.
    LeEnableRestorer leRestorer;
    ASSERT_EQ(BTM_Disable(LE_CONTROLLER), BT_SUCCESS);
    if (BTM_IsEnabled(LE_CONTROLLER)) {
        // Disable is deferred/asynchronous on this harness: NOT_ENABLE cannot be
        // reproduced deterministically, so restore and skip the group.
        EXPECT_EQ(BTM_Enable(LE_CONTROLLER), BT_SUCCESS);
        GTEST_SKIP() << "LE controller disable is deferred; cannot reproduce NOT_ENABLE deterministically";
    }
    BtAddr addr = FsuDummyAddr();
    EXPECT_EQ(GAPIF_LeFrameSpaceUpdate(&addr, 0x0064, 0x00C8, 0x01, 0x01), GAP_ERR_NOT_ENABLE);
    ASSERT_EQ(BTM_Enable(LE_CONTROLLER), BT_SUCCESS);
    ASSERT_TRUE(BTM_IsEnabled(LE_CONTROLLER));
    EXPECT_EQ(GAPIF_LeSetRole(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL), BT_SUCCESS);
}

// @tc.number: StackGapLe6_0_FsuBtmBoundary_02203
// @tc.name:  0x009D boundary fast checks reject reserved params once bit 65 opens
// @tc.desc:  on a bit-65 board the feature gate opens, then the shared host
//            boundary predicate returns INVAL_PARAM for min > max, max above the
//            0x2710 ceiling and zero/reserved PHYS or Spacing_Types; a valid
//            request passes the host checks and reaches the connection lookup,
//            which finds no ACL on a single board (that part is the 8.4 two-device
//            check). A pre-6.0 controller cannot reach this group, so it skips.
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_FsuBtmBoundary_02203, TestSize.Level1)
{
    if (!BTM_IsControllerSupportLeFrameSpaceUpdate()) {
        GTEST_SKIP() << "local controller lacks LE feature bit 65 (Frame Space Update)";
    }
    BtAddr addr = FsuDummyAddr();

    EXPECT_EQ(GAPIF_LeFrameSpaceUpdate(&addr, 0x00C8, 0x0064, 0x01, 0x01), GAP_ERR_INVAL_PARAM);
    EXPECT_EQ(GAPIF_LeFrameSpaceUpdate(&addr, 0x0064, 0x2711, 0x01, 0x01), GAP_ERR_INVAL_PARAM);
    EXPECT_EQ(GAPIF_LeFrameSpaceUpdate(&addr, 0x0064, 0x00C8, 0x00, 0x01), GAP_ERR_INVAL_PARAM);
    EXPECT_EQ(GAPIF_LeFrameSpaceUpdate(&addr, 0x0064, 0x00C8, 0x08, 0x01), GAP_ERR_INVAL_PARAM);
    EXPECT_EQ(GAPIF_LeFrameSpaceUpdate(&addr, 0x0064, 0x00C8, 0x01, 0x20), GAP_ERR_INVAL_PARAM);

    // Valid host parameters pass the boundary predicate and fail only at the
    // (empty) connection lookup, proving the boundary runs before any link state.
    EXPECT_EQ(GAPIF_LeFrameSpaceUpdate(&addr, 0x2710, 0x2710, 0x07, 0x1F), GAP_ERR_INVAL_STATE);
}

// ---- 8.3 Channel Sounding behaviour (plan 8.3, single-board reachable groups) ----
// Two things bound these cases. First, the CS engine's session entry CsOpenSession
// opens with BtmGetAclAddressByHandle and returns when the handle has no ACL row, and a
// single-board test cannot create one (BtmAclCreateConnection is static, btm_acl.c:328),
// so they stay on the paths that need no session: the HCI event layer's own dispatch
// and the command-failure synthesis. The groups that need a session row (pre-enable
// refusal, the connection-channel result stream, the engine's in-flight "exactly one
// completion" check, end-to-end) are cases 00600-00640 in
// stack_gap_le_6_0_interact_test.cpp. Second, the four cases split by what they touch:
// 02300 and 02301 inject their own frames into the HCI event and command layers and read
// no Controller state, so they run on any board, in their own fixture below (which owns
// the HCI event layer, for the reason written at its class); 02302 and 02303 put a real
// 0x0095/0x0096 on the wire, so they are the two gated on the local Controller exposing
// LL feature bit 46, reporting GTEST_SKIP without it.

// The fixture for the two cases that inject their own frames. They need the HCI event
// callback list and nothing else, and they do not share StackGapLe60BtmTest: that
// fixture has to re-register the CS table per case, because BTM_Enable runs
// BtmEnableLeAndSharedModules (btm.c:305) -> HCI_Initialize -> HciInitEvent
// (hci_evt.c:48), which replaces g_eventCallbackList with a fresh empty one, dropping
// every table registered before it, and cases 02002/02202 re-enable LE from inside it.
// This fixture performs no BTM_Enable at all, so its single class-level registration
// outlives both cases.
class StackGapLe60CsInjectTest : public testing::Test {
public:
    static void SetUpTestCase(void)
    {
        HciInitEvent();
        InitCsParseCallbacks();
        // Guard only: always returns BT_SUCCESS (hci_evt.c:2044) - no verification power.
        ASSERT_EQ(HCI_RegisterEventCallbacks(&g_csParseCallbacks), BT_SUCCESS);
    }
    static void TearDownTestCase(void)
    {
        // Guard only: always returns BT_SUCCESS (hci_evt.c:2054) - no verification power.
        EXPECT_EQ(HCI_DeregisterEventCallbacks(&g_csParseCallbacks), BT_SUCCESS);
        HciCloseEvent();
    }
    void SetUp() override {}
    void TearDown() override {}
};

// @tc.number: StackGapLe6_0_CsStatusPassthrough_02300
// @tc.name:  0x11/0x12/0x0D/0x48 reach the consumer unchanged on both event shapes
// @tc.desc:  plan 8.3 "error-code passthrough": a capability mismatch (0x11), an
//            unknown Config_ID or bad parameter (0x12), a scheduling/coexistence
//            overflow (0x0D) and too few usable channels (0x48) are the Controller's
//            own answers. The HCI layer is transport-only, so each must arrive at the
//            registered callback with its status byte and identity fields exactly as
//            injected - on the subevent shape (0x2F, 7.7.65,42 p2436) and on the
//            Command_Complete shape (0x0093 return, 7.8.140 p2836).
HWTEST_F(StackGapLe60CsInjectTest, StackGapLe6_0_CsStatusPassthrough_02300, TestSize.Level1)
{
    const uint8_t statuses[] = {
        HCI_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE,
        HCI_INVALID_HCI_COMMAND_PARAMETERS,
        HCI_CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES,
        CS_STATUS_TOO_FEW_USABLE_CHANNELS
    };
    const uint16_t handle = CS_NON_TEST_HANDLE;
    const uint8_t configId = CS_INJECT_CONFIG_ID;

    for (uint8_t status : statuses) {
        // Subevent shape: 0x2F LE CS Config Complete, Action 0x01 (created).
        g_csConfigCapture.Reset();
        uint8_t wire[LE_META_EVT_HEAD_LEN + CS_CONFIG_EVT_PAYLOAD] = {0};
        wire[LE_META_EVT_EVENT_CODE_OFFSET] = HCI_LE_META_EVENT;   // Event_Code: LE Meta
        wire[LE_META_EVT_LENGTH_OFFSET] =
            static_cast<uint8_t>((LE_META_EVT_HEAD_LEN - LE_META_EVT_PREFIX_LEN) + CS_CONFIG_EVT_PAYLOAD);
        wire[LE_META_EVT_SUBEVENT_CODE_OFFSET] = HCI_LE_CS_CONFIG_COMPLETE_EVENT;  // Subevent 0x2F
        HciLeCsConfigCompleteEventParam configParam = {};
        configParam.status = status;
        configParam.connectionHandle = handle;
        configParam.configId = configId;
        configParam.action = LE_CS_CONFIG_ACTION_CREATED;
        ASSERT_EQ(memcpy_s(wire + LE_META_EVT_HEAD_LEN, sizeof(wire) - LE_META_EVT_HEAD_LEN,
            &configParam, sizeof(configParam)), EOK);
        InjectWire(wire, sizeof(wire));

        EXPECT_TRUE(g_csConfigCapture.received);
        EXPECT_EQ(g_csConfigCapture.param.status, status);
        EXPECT_EQ(g_csConfigCapture.param.connectionHandle, handle);
        EXPECT_EQ(g_csConfigCapture.param.configId, configId);
        EXPECT_EQ(g_csConfigCapture.param.action, LE_CS_CONFIG_ACTION_CREATED);

        // Command_Complete shape: 0x0093 LE CS Set Procedure Parameters, whose return
        // is Status + Connection_Handle only (Config_ID is not echoed).
        g_csProcParamsCapture.Reset();
        HciLeCsSetProcedureParametersReturnParam procParamsReturn = {};
        procParamsReturn.status = status;
        procParamsReturn.connectionHandle = handle;
        HciEventOnLeCommandComplete(
            HCI_LE_CS_SET_PROCEDURE_PARAMETERS, &procParamsReturn, sizeof(procParamsReturn));

        EXPECT_TRUE(g_csProcParamsCapture.received);
        EXPECT_EQ(g_csProcParamsCapture.param.status, status);
        EXPECT_EQ(g_csProcParamsCapture.param.connectionHandle, handle);
    }
}

// @tc.number: StackGapLe6_0_CsCompletionUniqueness_02301
// @tc.name:  a CS command closes with exactly one completion, on its own callback
// @tc.desc:  plan 8.3 "completion uniqueness", the side the Host synthesizes. Failure
//            and timeout: HciOnLeControllerCmdFailed is the common downstream of all
//            three production call sites - the 10 s timeout (hci_cmd.c:227, status
//            HCI_TIMEOUT 0xE1), a malformed Command_Complete (:619, 0xE2) and a
//            Command_Status rejection (:551, the Controller's own status) - and each
//            of the 14 CS commands must close there with exactly one completion, on
//            its own callback, carrying the failure status and the identity its
//            original command parameter still holds. Success: one real subevent
//            (0x2C/0x2D/0x2E/0x2F/0x30/0x33) or one real Command_Complete return
//            likewise yields exactly one. "Exactly one" is read off a counter per CS
//            callback field, so a run that fires the right field *and* a second,
//            wrong one fails here; the two rows that must stay silent (0x0097, whose
//            [v2] command is never sent, plan 5.6.2, and an OCF past the table end)
//            are asserted to fire none at all.
//            The 0x31/0x32 result fields are counted but never driven: they are
//            procedure results, not the close of a command, and one procedure emits
//            several of them - driving one would assert uniqueness where the spec
//            promises repetition. Counting them is what makes a stray one visible.
//            What the failure row receives as param is whatever the sender handed to
//            HciAllocCmd, on all three paths (hci_cmd.c:227, :551 param = cmd->param,
//            :619): the command's own HciLeCs*Param struct for eleven of them, NULL
//            for the two sent with no parameter at all, and a serialized wire buffer
//            for 0x0095, whose payload is variable.
static void ExpectFailedReadAndCachedCompletions(uint16_t handle, uint8_t failure)
{
    ResetCsCompletions();
    HciOnLeControllerCmdFailed(HCI_LE_CS_READ_LOCAL_SUPPORTED_CAPABILITIES, failure, nullptr);
    ExpectSoleCsCompletion(g_csTally.readLocalCaps, "readLocalCaps");
    EXPECT_EQ(g_csIdentity.status, failure);
    EXPECT_TRUE(g_csIdentity.payloadAllZero);

    ResetCsCompletions();
    HciLeCsReadRemoteSupportedCapabilitiesParam remoteCaps = {};
    remoteCaps.connectionHandle = handle;
    HciOnLeControllerCmdFailed(HCI_LE_CS_READ_REMOTE_SUPPORTED_CAPABILITIES, failure, &remoteCaps);
    ExpectSoleCsCompletion(g_csTally.readRemoteCaps, "readRemoteCaps");
    EXPECT_EQ(g_csIdentity.status, failure);
    EXPECT_EQ(g_csIdentity.connectionHandle, handle);
    EXPECT_TRUE(g_csIdentity.payloadAllZero);

    ResetCsCompletions();
    HciLeCsWriteCachedRemoteSupportedCapabilitiesParam cachedCaps = {};
    cachedCaps.connectionHandle = handle;
    HciOnLeControllerCmdFailed(
        HCI_LE_CS_WRITE_CACHED_REMOTE_SUPPORTED_CAPABILITIES, failure, &cachedCaps);
    ExpectSoleCsCompletion(g_csTally.writeCachedRemoteCaps, "writeCachedRemoteCaps");
    EXPECT_EQ(g_csIdentity.status, failure);
    EXPECT_EQ(g_csIdentity.connectionHandle, handle);
}

static void ExpectFailedSecurityDefaultAndFaeCompletions(uint16_t handle, uint8_t failure)
{
    ResetCsCompletions();
    HciLeCsSecurityEnableParam security = {};
    security.connectionHandle = handle;
    HciOnLeControllerCmdFailed(HCI_LE_CS_SECURITY_ENABLE, failure, &security);
    ExpectSoleCsCompletion(g_csTally.securityEnable, "securityEnable");
    EXPECT_EQ(g_csIdentity.status, failure);
    EXPECT_EQ(g_csIdentity.connectionHandle, handle);

    ResetCsCompletions();
    HciLeCsSetDefaultSettingsParam defaults = {};
    defaults.connectionHandle = handle;
    HciOnLeControllerCmdFailed(HCI_LE_CS_SET_DEFAULT_SETTINGS, failure, &defaults);
    ExpectSoleCsCompletion(g_csTally.setDefaultSettings, "setDefaultSettings");
    EXPECT_EQ(g_csIdentity.status, failure);
    EXPECT_EQ(g_csIdentity.connectionHandle, handle);

    ResetCsCompletions();
    HciLeCsReadRemoteFaeTableParam fae = {};
    fae.connectionHandle = handle;
    HciOnLeControllerCmdFailed(HCI_LE_CS_READ_REMOTE_FAE_TABLE, failure, &fae);
    ExpectSoleCsCompletion(g_csTally.readRemoteFae, "readRemoteFae");
    EXPECT_EQ(g_csIdentity.status, failure);
    EXPECT_EQ(g_csIdentity.connectionHandle, handle);
    EXPECT_TRUE(g_csIdentity.payloadAllZero);

    ResetCsCompletions();
    HciLeCsWriteCachedRemoteFaeTableParam cachedFae = {};
    cachedFae.connectionHandle = handle;
    HciOnLeControllerCmdFailed(HCI_LE_CS_WRITE_CACHED_REMOTE_FAE_TABLE, failure, &cachedFae);
    ExpectSoleCsCompletion(g_csTally.writeCachedRemoteFae, "writeCachedRemoteFae");
    EXPECT_EQ(g_csIdentity.status, failure);
    EXPECT_EQ(g_csIdentity.connectionHandle, handle);
}

static void ExpectFailedConfigAndProcedureEnableCompletions(
    uint16_t handle, uint8_t configId, uint8_t failure)
{
    ResetCsCompletions();
    HciLeCsCreateConfigParam create = {};
    create.connectionHandle = handle;
    create.configId = configId;
    HciOnLeControllerCmdFailed(HCI_LE_CS_CREATE_CONFIG, failure, &create);
    ExpectSoleCsCompletion(g_csTally.config, "config");
    EXPECT_EQ(g_csIdentity.status, failure);
    EXPECT_EQ(g_csIdentity.connectionHandle, handle);
    EXPECT_EQ(g_csIdentity.configId, configId);
    EXPECT_EQ(g_csIdentity.action, LE_CS_CONFIG_ACTION_CREATED);

    ResetCsCompletions();
    HciLeCsRemoveConfigParam remove = {};
    remove.connectionHandle = handle;
    remove.configId = configId;
    HciOnLeControllerCmdFailed(HCI_LE_CS_REMOVE_CONFIG, failure, &remove);
    ExpectSoleCsCompletion(g_csTally.config, "config");
    EXPECT_EQ(g_csIdentity.status, failure);
    EXPECT_EQ(g_csIdentity.connectionHandle, handle);
    EXPECT_EQ(g_csIdentity.configId, configId);
    EXPECT_EQ(g_csIdentity.action, LE_CS_CONFIG_ACTION_REMOVED);

    ResetCsCompletions();
    HciLeCsSetProcedureParametersParam procParams = {};
    procParams.connectionHandle = handle;
    procParams.configId = configId;
    HciOnLeControllerCmdFailed(HCI_LE_CS_SET_PROCEDURE_PARAMETERS, failure, &procParams);
    ExpectSoleCsCompletion(g_csTally.setProcedureParameters, "setProcedureParameters");
    EXPECT_EQ(g_csIdentity.status, failure);
    EXPECT_EQ(g_csIdentity.connectionHandle, handle);

    ResetCsCompletions();
    HciLeCsProcedureEnableParam enableCmd = {};
    enableCmd.connectionHandle = handle;
    enableCmd.configId = configId;
    enableCmd.enable = LE_CS_PROCEDURE_ENABLED;
    HciOnLeControllerCmdFailed(HCI_LE_CS_PROCEDURE_ENABLE, failure, &enableCmd);
    ExpectSoleCsCompletion(g_csTally.procedureEnable, "procedureEnable");
    EXPECT_EQ(g_csIdentity.status, failure);
    EXPECT_EQ(g_csIdentity.connectionHandle, handle);
    EXPECT_EQ(g_csIdentity.configId, configId);
    EXPECT_EQ(g_csIdentity.state, LE_CS_PROCEDURE_DISABLED);
}

static void ExpectFailedMiscellaneousCompletions(uint8_t failure)
{
    ResetCsCompletions();
    HciLeCsSetChannelClassificationParam classification = {};
    HciOnLeControllerCmdFailed(HCI_LE_CS_SET_CHANNEL_CLASSIFICATION, failure, &classification);
    ExpectSoleCsCompletion(g_csTally.setChannelClassification, "setChannelClassification");
    EXPECT_EQ(g_csIdentity.status, failure);

    ResetCsCompletions();
    uint8_t testWire[sizeof(HciLeCsTestPrefix) + LE_CS_TEST_OVERRIDE_MIN] = {0};
    HciOnLeControllerCmdFailed(HCI_LE_CS_TEST, failure, &testWire);
    ExpectSoleCsCompletion(g_csTally.test, "test");
    EXPECT_EQ(g_csIdentity.status, failure);

    ResetCsCompletions();
    HciOnLeControllerCmdFailed(HCI_LE_CS_TEST_END, failure, nullptr);
    ExpectSoleCsCompletion(g_csTally.testEnd, "testEnd");
    EXPECT_EQ(g_csIdentity.status, failure);
}

static void ExpectUnassignedRowsStaySilent(uint8_t failure)
{
    ResetCsCompletions();
    HciOnLeControllerCmdFailed(
        MAKE_OPCODE(0x0097, HCI_COMMAND_OGF_LE_CONTROLLER), failure, nullptr);
    HciOnLeControllerCmdFailed(
        MAKE_OPCODE(0x009E, HCI_COMMAND_OGF_LE_CONTROLLER), failure, nullptr);
    EXPECT_EQ(g_csTally.Total(), 0) << "an unassigned row answered a command it does not own";
}

static void ExpectSuccessMetaEventReadCompletions(uint8_t success)
{
    ResetCsCompletions();
    const size_t capsBody = sizeof(HciLeCsReadRemoteSupportedCapabilitiesCompleteEventParam);
    uint8_t capsEvt[LE_META_EVT_HEAD_LEN + capsBody] = {0};
    capsEvt[LE_META_EVT_EVENT_CODE_OFFSET] = HCI_LE_META_EVENT;
    capsEvt[LE_META_EVT_LENGTH_OFFSET] = static_cast<uint8_t>(
        (LE_META_EVT_HEAD_LEN - LE_META_EVT_PREFIX_LEN) + capsBody);
    capsEvt[LE_META_EVT_SUBEVENT_CODE_OFFSET] = HCI_LE_CS_READ_REMOTE_SUPPORTED_CAPABILITIES_COMPLETE_EVENT;
    InjectWire(capsEvt, sizeof(capsEvt));
    ExpectSoleCsCompletion(g_csTally.readRemoteCaps, "readRemoteCaps");
    EXPECT_EQ(g_csIdentity.status, success);

    ResetCsCompletions();
    const size_t faeBody = sizeof(HciLeCsReadRemoteFaeTableCompleteEventParam);
    uint8_t faeEvt[LE_META_EVT_HEAD_LEN + faeBody] = {0};
    faeEvt[LE_META_EVT_EVENT_CODE_OFFSET] = HCI_LE_META_EVENT;
    faeEvt[LE_META_EVT_LENGTH_OFFSET] = static_cast<uint8_t>((LE_META_EVT_HEAD_LEN - LE_META_EVT_PREFIX_LEN) + faeBody);
    faeEvt[LE_META_EVT_SUBEVENT_CODE_OFFSET] = HCI_LE_CS_READ_REMOTE_FAE_TABLE_COMPLETE_EVENT;
    InjectWire(faeEvt, sizeof(faeEvt));
    ExpectSoleCsCompletion(g_csTally.readRemoteFae, "readRemoteFae");
    EXPECT_EQ(g_csIdentity.status, success);

    ResetCsCompletions();
    const size_t secBody = sizeof(HciLeCsSecurityEnableCompleteEventParam);
    uint8_t secEvt[LE_META_EVT_HEAD_LEN + secBody] = {0};
    secEvt[LE_META_EVT_EVENT_CODE_OFFSET] = HCI_LE_META_EVENT;
    secEvt[LE_META_EVT_LENGTH_OFFSET] = static_cast<uint8_t>((LE_META_EVT_HEAD_LEN - LE_META_EVT_PREFIX_LEN) + secBody);
    secEvt[LE_META_EVT_SUBEVENT_CODE_OFFSET] = HCI_LE_CS_SECURITY_ENABLE_COMPLETE_EVENT;
    InjectWire(secEvt, sizeof(secEvt));
    ExpectSoleCsCompletion(g_csTally.securityEnable, "securityEnable");
    EXPECT_EQ(g_csIdentity.status, success);
}

static void ExpectSuccessMetaEventConfigAndLaterCompletions(uint8_t success)
{
    ResetCsCompletions();
    uint8_t configEvt[LE_META_EVT_HEAD_LEN + CS_CONFIG_EVT_PAYLOAD] = {0};
    configEvt[LE_META_EVT_EVENT_CODE_OFFSET] = HCI_LE_META_EVENT;
    configEvt[LE_META_EVT_LENGTH_OFFSET] = static_cast<uint8_t>(
        (LE_META_EVT_HEAD_LEN - LE_META_EVT_PREFIX_LEN) + CS_CONFIG_EVT_PAYLOAD);
    configEvt[LE_META_EVT_SUBEVENT_CODE_OFFSET] = HCI_LE_CS_CONFIG_COMPLETE_EVENT;
    HciLeCsConfigCompleteEventParam configParam = {};
    configParam.status = success;
    configParam.action = LE_CS_CONFIG_ACTION_CREATED;
    ASSERT_EQ(memcpy_s(configEvt + LE_META_EVT_HEAD_LEN, sizeof(configEvt) - LE_META_EVT_HEAD_LEN,
        &configParam, sizeof(configParam)), EOK);
    InjectWire(configEvt, sizeof(configEvt));
    ExpectSoleCsCompletion(g_csTally.config, "config");
    EXPECT_EQ(g_csIdentity.status, success);
    EXPECT_EQ(g_csIdentity.action, LE_CS_CONFIG_ACTION_CREATED);

    ResetCsCompletions();
    configParam.action = LE_CS_CONFIG_ACTION_REMOVED;
    ASSERT_EQ(memcpy_s(configEvt + LE_META_EVT_HEAD_LEN, sizeof(configEvt) - LE_META_EVT_HEAD_LEN,
        &configParam, sizeof(configParam)), EOK);
    InjectWire(configEvt, sizeof(configEvt));
    ExpectSoleCsCompletion(g_csTally.config, "config");
    EXPECT_EQ(g_csIdentity.action, LE_CS_CONFIG_ACTION_REMOVED);

    ResetCsCompletions();
    const size_t procBody = sizeof(HciLeCsProcedureEnableCompleteEventParam);
    uint8_t procEvt[LE_META_EVT_HEAD_LEN + procBody] = {0};
    procEvt[LE_META_EVT_EVENT_CODE_OFFSET] = HCI_LE_META_EVENT;
    procEvt[LE_META_EVT_LENGTH_OFFSET] = static_cast<uint8_t>(
        (LE_META_EVT_HEAD_LEN - LE_META_EVT_PREFIX_LEN) + procBody);
    procEvt[LE_META_EVT_SUBEVENT_CODE_OFFSET] = HCI_LE_CS_PROCEDURE_ENABLE_COMPLETE_EVENT;
    InjectWire(procEvt, sizeof(procEvt));
    ExpectSoleCsCompletion(g_csTally.procedureEnable, "procedureEnable");
    EXPECT_EQ(g_csIdentity.status, success);

    ResetCsCompletions();
    const size_t testEndBody = sizeof(HciLeCsTestEndCompleteEventParam);
    uint8_t testEndEvt[LE_META_EVT_HEAD_LEN + testEndBody] = {0};
    testEndEvt[LE_META_EVT_EVENT_CODE_OFFSET] = HCI_LE_META_EVENT;
    testEndEvt[LE_META_EVT_LENGTH_OFFSET] = static_cast<uint8_t>(
        (LE_META_EVT_HEAD_LEN - LE_META_EVT_PREFIX_LEN) + testEndBody);
    testEndEvt[LE_META_EVT_SUBEVENT_CODE_OFFSET] = HCI_LE_CS_TEST_END_COMPLETE_EVENT;
    InjectWire(testEndEvt, sizeof(testEndEvt));
    ExpectSoleCsCompletion(g_csTally.testEnd, "testEnd");
    EXPECT_EQ(g_csIdentity.status, success);
}

static void ExpectSuccessCommandCompleteCompletions(uint16_t handle, uint8_t success)
{
    ResetCsCompletions();
    HciLeCsReadLocalSupportedCapabilitiesReturnParam localCaps = {};
    localCaps.status = success;
    HciEventOnLeCommandComplete(
        HCI_LE_CS_READ_LOCAL_SUPPORTED_CAPABILITIES, &localCaps, sizeof(localCaps));
    ExpectSoleCsCompletion(g_csTally.readLocalCaps, "readLocalCaps");

    ResetCsCompletions();
    HciLeCsWriteCachedRemoteSupportedCapabilitiesReturnParam writeCaps = {};
    writeCaps.status = success;
    writeCaps.connectionHandle = handle;
    HciEventOnLeCommandComplete(
        HCI_LE_CS_WRITE_CACHED_REMOTE_SUPPORTED_CAPABILITIES, &writeCaps, sizeof(writeCaps));
    ExpectSoleCsCompletion(g_csTally.writeCachedRemoteCaps, "writeCachedRemoteCaps");
    EXPECT_EQ(g_csIdentity.connectionHandle, handle);

    ResetCsCompletions();
    HciLeCsSetDefaultSettingsReturnParam setDefaults = {};
    setDefaults.status = success;
    setDefaults.connectionHandle = handle;
    HciEventOnLeCommandComplete(HCI_LE_CS_SET_DEFAULT_SETTINGS, &setDefaults, sizeof(setDefaults));
    ExpectSoleCsCompletion(g_csTally.setDefaultSettings, "setDefaultSettings");
    EXPECT_EQ(g_csIdentity.connectionHandle, handle);

    ResetCsCompletions();
    HciLeCsWriteCachedRemoteFaeTableReturnParam writeFae = {};
    writeFae.status = success;
    writeFae.connectionHandle = handle;
    HciEventOnLeCommandComplete(
        HCI_LE_CS_WRITE_CACHED_REMOTE_FAE_TABLE, &writeFae, sizeof(writeFae));
    ExpectSoleCsCompletion(g_csTally.writeCachedRemoteFae, "writeCachedRemoteFae");
    EXPECT_EQ(g_csIdentity.connectionHandle, handle);

    ResetCsCompletions();
    HciLeCsSetChannelClassificationReturnParam setClassification = {};
    setClassification.status = success;
    HciEventOnLeCommandComplete(
        HCI_LE_CS_SET_CHANNEL_CLASSIFICATION, &setClassification, sizeof(setClassification));
    ExpectSoleCsCompletion(g_csTally.setChannelClassification, "setChannelClassification");

    ResetCsCompletions();
    HciLeCsSetProcedureParametersReturnParam procParamsReturn = {};
    procParamsReturn.status = success;
    procParamsReturn.connectionHandle = handle;
    HciEventOnLeCommandComplete(
        HCI_LE_CS_SET_PROCEDURE_PARAMETERS, &procParamsReturn, sizeof(procParamsReturn));
    ExpectSoleCsCompletion(g_csTally.setProcedureParameters, "setProcedureParameters");
    EXPECT_EQ(g_csIdentity.connectionHandle, handle);

    ResetCsCompletions();
    HciLeCsTestReturnParam testReturn = {};
    testReturn.status = success;
    HciEventOnLeCommandComplete(HCI_LE_CS_TEST, &testReturn, sizeof(testReturn));
    ExpectSoleCsCompletion(g_csTally.test, "test");
}

HWTEST_F(StackGapLe60CsInjectTest, StackGapLe6_0_CsCompletionUniqueness_02301, TestSize.Level1)
{
    const uint8_t failure = HCI_TIMEOUT;   // hci_error.h
    const uint8_t success = HCI_SUCCESS;   // hci_error.h
    const uint16_t handle = CS_NON_TEST_HANDLE;
    const uint8_t configId = CS_INJECT_CONFIG_ID;

    ExpectFailedReadAndCachedCompletions(handle, failure);
    ExpectFailedSecurityDefaultAndFaeCompletions(handle, failure);
    ExpectFailedConfigAndProcedureEnableCompletions(handle, configId, failure);
    ExpectFailedMiscellaneousCompletions(failure);
    ExpectUnassignedRowsStaySilent(failure);
    ExpectSuccessMetaEventReadCompletions(success);
    ExpectSuccessMetaEventConfigAndLaterCompletions(success);
    ExpectSuccessCommandCompleteCompletions(handle, success);
}

// @tc.number: StackGapLe6_0_EvtLengthMismatch_02304
// @tc.name:  a frame whose Length octet disagrees with its body is dropped above the table
// @tc.desc:  Round 4 F8: the exact-length gate HciOnEvent opens with (hci_evt.c:2032) had no
//            negative case - both injectors write the Length octet from the body they build
//            (InjectCsSubeventFrame here and in the interact file), so only hand-written
//            frames can disagree. Both directions are driven, plus one
//            self-consistent frame as the control: without it a green run could be read as
//            "nothing on this path is dispatched anyway".
//            The vehicle is subevent 0x33, whose 1 B body is the smallest that still reaches
//            a registered callback; the gate sits above the subevent table, so the code it
//            carries is arbitrary. The gate is the second door, not the first: the handlers'
//            own lower bounds are what cases 00500/00701 and 02305 drive.
HWTEST_F(StackGapLe60CsInjectTest, StackGapLe6_0_EvtLengthMismatch_02304, TestSize.Level1)
{
    const uint8_t success = HCI_SUCCESS;

    // Declared 0x01, carries 0x02 (Subevent_Code + Status): one octet short.
    const uint8_t declaredShort[] = {0x3E, 0x01, HCI_LE_CS_TEST_END_COMPLETE_EVENT, success};
    ResetCsCompletions();
    InjectWire(declaredShort, sizeof(declaredShort));
    EXPECT_EQ(g_csTally.Total(), 0) << "a frame declaring fewer octets than it carries was dispatched";

    // Declared 0x03, carries 0x02: one octet long.
    const uint8_t declaredLong[] = {0x3E, 0x03, HCI_LE_CS_TEST_END_COMPLETE_EVENT, success};
    InjectWire(declaredLong, sizeof(declaredLong));
    EXPECT_EQ(g_csTally.Total(), 0) << "a frame declaring more octets than it carries was dispatched";

    // Declared 0x02, carries 0x02: the control.
    const uint8_t consistent[] = {0x3E, 0x02, HCI_LE_CS_TEST_END_COMPLETE_EVENT, success};
    InjectWire(consistent, sizeof(consistent));
    EXPECT_EQ(g_csTally.testEnd, 1) << "the self-consistent frame never reached the 0x33 handler";
    EXPECT_EQ(g_csTally.Total(), 1);
}

// @tc.number: StackGapLe6_0_CsSubeventShortHead_02305
// @tc.name:  a CS subevent head shorter than its structure is dropped, on all seven
// @tc.desc:  Round 4 F9: the CS subevents had no truncated case at all - every injection
//            site in the file passes sizeof(head), so the lower bound each handler opens
//            with (hci_evt_le.c:389 for 0x31, :414 for 0x32, and the five above them) was
//            only ever driven positively, unlike MA / FSU / 0x2A / 0x22, which all have one.
//            One octet of head is shorter than every structure parsed here, so a single
//            loop covers the seven; the assertion is on the whole CS callback surface, not
//            on one field, so a head reaching the wrong handler is caught too.
//            0x33 is not in the list: a truncated 0x33 is not dropped but answered with a
//            synthesized malformed completion, which is case 02306.
HWTEST_F(StackGapLe60CsInjectTest, StackGapLe6_0_CsSubeventShortHead_02305, TestSize.Level1)
{
    // The CS subevents that parse a fixed head, in code order (7.7.65,39-7.7.65,45).
    const uint8_t codes[] = {
        HCI_LE_CS_READ_REMOTE_SUPPORTED_CAPABILITIES_COMPLETE_EVENT,  // 0x2C
        HCI_LE_CS_READ_REMOTE_FAE_TABLE_COMPLETE_EVENT,               // 0x2D
        HCI_LE_CS_SECURITY_ENABLE_COMPLETE_EVENT,                    // 0x2E
        HCI_LE_CS_CONFIG_COMPLETE_EVENT,                             // 0x2F
        HCI_LE_CS_PROCEDURE_ENABLE_COMPLETE_EVENT,                   // 0x30
        HCI_LE_CS_SUBEVENT_RESULT_EVENT,                             // 0x31
        HCI_LE_CS_SUBEVENT_RESULT_CONTINUE_EVENT,                    // 0x32
    };
    const uint8_t oneOctetHead[1] = {0x00};
    CsStepBlob noSteps;

    for (size_t i = 0; i < sizeof(codes); i++) {
        ResetCsCompletions();
        InjectCsSubeventFrame(codes[i], oneOctetHead, sizeof(oneOctetHead), noSteps);
        EXPECT_EQ(g_csTally.Total(), 0) <<
            "the subevent at index " <<
            i <<
            " accepted a head shorter than its structure";
    }
}

// @tc.number: StackGapLe6_0_MalformedCompletionSynth_02306
// @tc.name:  an empty 0x33 body and a short 0x0093 return become malformed completions
// @tc.desc:  Round 4 F10, the two HCI-layer halves. A 0x33 with no Status octet behind the
//            Subevent_Code is not dropped but synthesized into HCI_MALFORMED_EVENT
//            (hci_evt_le.c:449), because the engine recorded the 0x0096 when it was sent and
//            this is the only subevent that clears that record. A 0x0093 return too short to
//            hold Status + Connection_Handle is passed on naming no connection
//            (hci_evt_le_cmd_complete.c:2141) for the same reason. Neither drive is stopped
//            by the exact-length gate case 02304 covers: the 0x33 frame declares the octets
//            it carries, and the 0x0093 half is a direct handler call with no frame at all.
//            The 0x0095 half needs the engine's start latch and is case 02307.
HWTEST_F(StackGapLe60CsInjectTest, StackGapLe6_0_MalformedCompletionSynth_02306, TestSize.Level1)
{
    const uint8_t malformed = HCI_MALFORMED_EVENT;  // hci_error.h

    // Length 0x01 = the Subevent_Code alone: a legal frame carrying no Status.
    const uint8_t emptyEnd[] = {0x3E, 0x01, HCI_LE_CS_TEST_END_COMPLETE_EVENT};
    ResetCsCompletions();
    InjectWire(emptyEnd, sizeof(emptyEnd));
    EXPECT_EQ(g_csTally.testEnd, 1) << "the empty 0x33 was dropped instead of synthesized";
    EXPECT_EQ(g_csTally.Total(), 1);
    EXPECT_EQ(g_csIdentity.status, malformed) <<
        "the frame carries no Status, so no other code can stand for its outcome";

    // 0x0093 returns of 1 B and 2 B: both short of Status + Connection_Handle.
    const uint8_t shortReturn[2] = {0x00, 0x42};
    for (uint8_t length = 1; length < sizeof(HciLeCsSetProcedureParametersReturnParam); length++) {
        g_csProcParamsCapture.Reset();
        HciEventOnLeCommandComplete(HCI_LE_CS_SET_PROCEDURE_PARAMETERS, shortReturn, length);
        EXPECT_TRUE(g_csProcParamsCapture.received) <<
            "a " <<
            static_cast<unsigned int>(length) <<
            " B 0x0093 return was dropped";

        EXPECT_EQ(g_csProcParamsCapture.param.status, malformed);
        EXPECT_EQ(g_csProcParamsCapture.param.connectionHandle, LE_CS_INVALID_HANDLE) <<
            "a return too short to hold a handle named one";
    }
}

// @tc.number: StackGapLe6_0_CsResultReassembly_02302
// @tc.name:  a CS test result is reassembled into one report per subevent, or dropped whole
// @tc.desc:  8.3 result reassembly, driven on the test channel (7.7.65,44 pp2448-2458,
//            7.7.65,45 p2459, Table 7.2 p2447).
//            The test channel is what makes this drivable from a single board: a result
//            under the reserved handle 0x0FFF is handed to the test path before any
//            connection lookup, so no ACL row is needed, and the Role and RTT_Type the
//            steps are decoded against come from the 0x0095 that started the test rather
//            than from a configuration lookup - the two things a single board cannot
//            otherwise supply. The case drives the public CSIF_ surface: register a
//            callback, start a test, then feed 0x31/0x32 frames and assert on the
//            CsSubeventReport a consumer is handed.
//            Covered: a 0x31 that closes its own subevent; a 0x31 and the 0x32 that
//            continues it assembled into one report, with the counters kept from the
//            opening 0x31 and the statuses refreshed from the continuation; Subevent 0xF
//            and Procedure 0xF with their abort reason; the status pairs Table 7.2 does
//            not list; the three ways a subevent is dropped whole rather than partly
//            reported; the mode templates as the decode context selects them; and the
//            0x4000 "not available" code in both signed result fields.
//            CSIF_RegisterLeCsCallback is asserted rather than assumed: it refuses with
//            BT_NOT_SUPPORT when the HCI event layer is not up, and a silent refusal
//            would leave every "no report" assertion below passing because nothing was
//            listening.
static void CsAssertSingleSubeventReport(uint8_t noSounding, const uint8_t *mode0Initiator)
{
    g_csResultCapture.Reset();
    CsStepBlob oneStep;
    oneStep.Append(CS_STEP_MODE_0, CS_INJECT_CHANNEL_FIRST, mode0Initiator, CS_MODE0_INITIATOR_LEN);
    HciLeCsSubeventResultEventParam head = CsResultHead(
        CS_DONE_STATUS_OK, CS_DONE_STATUS_OK, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_ONE);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), oneStep);

    EXPECT_EQ(g_csResultCapture.reports, 1) << "a complete subevent did not produce a report";
    const CsSubeventReport &report = g_csResultCapture.report;
    EXPECT_EQ(report.connectionHandle, LE_CS_TEST_HANDLE);
    EXPECT_EQ(report.startAclConnEventCounter, CS_INJECT_ACL_EVENT_COUNTER);
    EXPECT_EQ(report.procedureCounter, CS_INJECT_PROCEDURE_COUNTER);
    EXPECT_EQ(report.frequencyCompensation, CS_INJECT_FREQ_COMPENSATION);
    EXPECT_EQ(report.referencePowerLevel, CS_REFERENCE_POWER_LEVEL_DBM);
    EXPECT_EQ(report.procedureDoneStatus, CS_DONE_STATUS_OK);
    EXPECT_EQ(report.subeventDoneStatus, CS_DONE_STATUS_OK);
    EXPECT_EQ(report.numStepsReported, CS_NUM_STEPS_ONE);
    EXPECT_EQ(report.numSteps, CS_NUM_STEPS_ONE);
    EXPECT_EQ(report.role, CS_ROLE_INITIATOR);
    EXPECT_EQ(report.rttType, noSounding);
    EXPECT_EQ(report.steps[0].mode, CS_STEP_MODE_0);
    EXPECT_EQ(report.steps[0].channel, CS_INJECT_CHANNEL_FIRST);
    EXPECT_TRUE(report.steps[0].hasMeasurement);
    EXPECT_TRUE(report.steps[0].stepData.mode0.hasFreqOffset);
    EXPECT_EQ(report.steps[0].stepData.mode0.packetRssi, CS_PACKET_RSSI_DBM);
    EXPECT_EQ(report.steps[0].stepData.mode0.measuredFreqOffset, CS_MEASURED_FREQ_OFFSET_0_01PPM);

    HciLeCsSubeventResultEventParam foreign = CsResultHead(
        CS_DONE_STATUS_OK, CS_DONE_STATUS_OK, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_ONE);
    foreign.connectionHandle = CS_NON_TEST_HANDLE;
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &foreign, sizeof(foreign), oneStep);
    EXPECT_EQ(g_csResultCapture.reports, 1) << "a subevent on a handle with no CS session was reported";

    CsStepBlob emptySteps;
    HciLeCsSubeventResultContinueEventParam orphan = CsContinueHead(
        CS_DONE_STATUS_OK, CS_DONE_STATUS_OK, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_ONE);
    InjectCsSubeventFrame(
        HCI_LE_CS_SUBEVENT_RESULT_CONTINUE_EVENT, &orphan, sizeof(orphan), emptySteps);
    EXPECT_EQ(g_csResultCapture.reports, 1) << "a 0x32 with no subevent open was reported";
}

static void CsAssertSubeventContinuation(uint8_t noSounding, const uint8_t *mode0Initiator)
{
    g_csResultCapture.Reset();
    HciLeCsSubeventResultEventParam head = CsResultHead(
        CS_DONE_STATUS_CONTINUES, CS_DONE_STATUS_CONTINUES, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_ONE);
    CsStepBlob oneStep;
    oneStep.Append(CS_STEP_MODE_0, CS_INJECT_CHANNEL_FIRST, mode0Initiator, CS_MODE0_INITIATOR_LEN);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), oneStep);
    EXPECT_EQ(g_csResultCapture.reports, 0) <<
        "a 0x31 whose Subevent_Done_Status says it continues emitted a report";

    CsStepBlob twoSteps;
    twoSteps.Append(CS_STEP_MODE_0, CS_INJECT_CHANNEL_SECOND, mode0Initiator, CS_MODE0_INITIATOR_LEN);
    twoSteps.Append(CS_STEP_MODE_0, CS_INJECT_CHANNEL_THIRD, mode0Initiator, CS_MODE0_INITIATOR_LEN);
    HciLeCsSubeventResultContinueEventParam cont = CsContinueHead(
        CS_DONE_STATUS_CONTINUES, CS_DONE_STATUS_OK, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_TWO);
    cont.configId = CS_INJECT_CONFIG_ID;
    InjectCsSubeventFrame(
        HCI_LE_CS_SUBEVENT_RESULT_CONTINUE_EVENT, &cont, sizeof(cont), twoSteps);

    EXPECT_EQ(g_csResultCapture.reports, 1) << "a continued subevent did not produce one report";
    const CsSubeventReport &joined = g_csResultCapture.report;
    EXPECT_EQ(joined.numSteps, CS_JOINED_NUM_STEPS);
    EXPECT_EQ(joined.numStepsReported, CS_NUM_STEPS_ONE);
    EXPECT_EQ(joined.configId, CS_TEST_CONFIG_ID_DEFAULT);
    EXPECT_EQ(joined.startAclConnEventCounter, CS_INJECT_ACL_EVENT_COUNTER);
    EXPECT_EQ(joined.procedureCounter, CS_INJECT_PROCEDURE_COUNTER);
    EXPECT_EQ(joined.frequencyCompensation, CS_INJECT_FREQ_COMPENSATION);
    EXPECT_EQ(joined.referencePowerLevel, CS_REFERENCE_POWER_LEVEL_DBM);
    EXPECT_EQ(joined.procedureDoneStatus, CS_DONE_STATUS_CONTINUES);
    EXPECT_EQ(joined.subeventDoneStatus, CS_DONE_STATUS_OK);
    EXPECT_EQ(joined.steps[0].channel, CS_INJECT_CHANNEL_FIRST);
    EXPECT_EQ(joined.steps[1].channel, CS_INJECT_CHANNEL_SECOND);
    EXPECT_EQ(joined.steps[CS_JOINED_THIRD_STEP_INDEX].channel, CS_JOINED_THIRD_STEP_CHANNEL);
    EXPECT_TRUE(joined.steps[CS_JOINED_THIRD_STEP_INDEX].hasMeasurement);
}

static void CsAssertAbortedAndInvalidSubeventDrops(const uint8_t *mode0Initiator)
{
    CsStepBlob emptySteps;
    CsStepBlob oneStep;
    oneStep.Append(CS_STEP_MODE_0, CS_INJECT_CHANNEL_FIRST, mode0Initiator, CS_MODE0_INITIATOR_LEN);

    g_csResultCapture.Reset();
    HciLeCsSubeventResultEventParam head = CsResultHead(
        CS_DONE_STATUS_OK, CS_DONE_STATUS_ALL_FAILED, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_NONE);
    head.abortReason = CS_TEST_ABORT_REASON_SAMPLE;
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), emptySteps);
    EXPECT_EQ(g_csResultCapture.reports, 1);
    EXPECT_EQ(g_csResultCapture.report.subeventDoneStatus, CS_DONE_STATUS_ALL_FAILED);
    EXPECT_EQ(g_csResultCapture.report.abortReason, CS_TEST_ABORT_REASON_SAMPLE);
    EXPECT_EQ(g_csResultCapture.report.numSteps, CS_NUM_STEPS_NONE);

    g_csResultCapture.Reset();
    head = CsResultHead(
        CS_DONE_STATUS_ALL_FAILED, CS_DONE_STATUS_ALL_FAILED, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_NONE);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), emptySteps);
    EXPECT_EQ(g_csResultCapture.reports, 1);
    EXPECT_EQ(g_csResultCapture.report.procedureDoneStatus, CS_DONE_STATUS_ALL_FAILED);
    EXPECT_EQ(g_csResultCapture.report.subeventDoneStatus, CS_DONE_STATUS_ALL_FAILED);

    g_csResultCapture.Reset();
    head = CsResultHead(CS_DONE_STATUS_OK, CS_DONE_STATUS_CONTINUES, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_ONE);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), oneStep);
    EXPECT_EQ(g_csResultCapture.reports, 0) <<
        "Procedure 0x0 with Subevent 0x1 is outside Table 7.2 and was reported";

    head = CsResultHead(CS_DONE_STATUS_RESERVED, CS_DONE_STATUS_OK, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_ONE);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), oneStep);
    EXPECT_EQ(g_csResultCapture.reports, 0) <<
        "the reserved Procedure_Done_Status 0x2 was reported";

    g_csResultCapture.Reset();
    CsStepBlob cutShort;
    cutShort.Append(CS_STEP_MODE_0, CS_INJECT_CHANNEL_FIRST, mode0Initiator, CS_MODE0_INITIATOR_LEN);
    cutShort.AppendOversized(
        CS_STEP_MODE_0, CS_INJECT_CHANNEL_SECOND, CS_TEST_OVERSIZED_STEP_DATA_LEN,
        mode0Initiator, CS_MODE0_OVERSIZED_HAVE_LEN);
    head = CsResultHead(CS_DONE_STATUS_OK, CS_DONE_STATUS_OK, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_TWO);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), cutShort);
    EXPECT_EQ(g_csResultCapture.reports, 0) <<
        "a subevent whose step region runs past the event was reported";
}

static void CsAssertMaxStepsBoundAndStreamClean(const uint8_t *mode0Initiator)
{
    CsStepBlob emptySteps;
    CsStepBlob oneStep;
    oneStep.Append(CS_STEP_MODE_0, CS_INJECT_CHANNEL_FIRST, mode0Initiator, CS_MODE0_INITIATOR_LEN);

    g_csResultCapture.Reset();
    HciLeCsSubeventResultEventParam head = CsResultHead(
        CS_DONE_STATUS_OK, CS_DONE_STATUS_OK, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_OVER_MAX);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), emptySteps);
    EXPECT_EQ(g_csResultCapture.reports, 0) << "161 steps in one event were reported";

    CsStepBlob full;
    full.AppendEmpty(CS_STEP_MODE_0, CS_INJECT_CHANNEL_FIRST, CS_STEPS_PER_EVENT_MAX);
    head = CsResultHead(
        CS_DONE_STATUS_OK, CS_DONE_STATUS_CONTINUES, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_STEPS_PER_EVENT_MAX);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), full);
    EXPECT_EQ(g_csResultCapture.reports, 0);

    HciLeCsSubeventResultContinueEventParam more = CsContinueHead(
        CS_DONE_STATUS_OK, CS_DONE_STATUS_CONTINUES, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_STEPS_PER_EVENT_MAX);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_CONTINUE_EVENT, &more, sizeof(more), full);
    EXPECT_EQ(g_csResultCapture.reports, 0);

    CsStepBlob crossing;
    crossing.AppendEmpty(CS_STEP_MODE_0, CS_INJECT_CHANNEL_FIRST, CS_EMPTY_STEP_RECORDS);
    more = CsContinueHead(CS_DONE_STATUS_OK, CS_DONE_STATUS_OK, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_THREE);
    InjectCsSubeventFrame(
        HCI_LE_CS_SUBEVENT_RESULT_CONTINUE_EVENT, &more, sizeof(more), crossing);
    EXPECT_EQ(g_csResultCapture.reports, 0) <<
        "the event that crossed 160 steps was reported instead of dropped";

    g_csResultCapture.Reset();
    head = CsResultHead(CS_DONE_STATUS_OK, CS_DONE_STATUS_OK, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_ONE);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), oneStep);
    EXPECT_EQ(g_csResultCapture.reports, 1);
    EXPECT_EQ(g_csResultCapture.report.numSteps, CS_NUM_STEPS_ONE) <<
        "the dropped subevent left steps behind in the stream";
}

static const CsStepResult &CsInjectMode3OneStep(const CsStepBlob &steps, uint8_t numAntennaPaths)
{
    g_csResultCapture.Reset();
    HciLeCsSubeventResultEventParam head = CsResultHead(
        CS_DONE_STATUS_OK, CS_DONE_STATUS_OK, numAntennaPaths, CS_NUM_STEPS_ONE);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), steps);
    EXPECT_EQ(g_csResultCapture.reports, 1);
    return g_csResultCapture.report.steps[0];
}

static void CsAssertMode3DecodeContext(uint8_t sounding)
{
    ASSERT_TRUE(StartCsLeTest(CS_ROLE_INITIATOR, sounding, CS_STEP_MODE_3));
    uint8_t mode3Body[CS_MODE3_STEP_LEN_NO_PCT_4] = {0};
    FillMode3Body(mode3Body, sizeof(mode3Body));

    CsStepBlob mode3Step;
    mode3Step.Append(CS_STEP_MODE_3, CS_INJECT_CHANNEL_FIRST, mode3Body, CS_MODE3_STEP_LEN_PCT);
    const CsStepResult &preferred = CsInjectMode3OneStep(mode3Step, CS_MODE3_NUM_ANTENNA_PATHS_PCT);
    EXPECT_TRUE(preferred.hasMeasurement);
    EXPECT_TRUE(preferred.stepData.mode3.head.hasPct);
    EXPECT_EQ(preferred.stepData.mode3.tones.numTonePaths, CS_MODE3_TONE_PATHS_PCT);
    EXPECT_EQ(preferred.stepData.mode3.head.packetPct1, CS_TEST_MODE3_PACKET_PCT1);
    EXPECT_EQ(preferred.stepData.mode3.head.packetPct2, CS_TEST_MODE3_PACKET_PCT2);
    EXPECT_EQ(preferred.stepData.mode3.tones.antennaPermutationIndex, CS_TEST_MODE3_ANTENNA_PERMUTATION_PCT);

    const CsStepResult &byHead = CsInjectMode3OneStep(mode3Step, CS_MODE3_NUM_ANTENNA_PATHS_2);
    EXPECT_TRUE(byHead.hasMeasurement);
    EXPECT_FALSE(byHead.stepData.mode3.head.hasPct);
    EXPECT_EQ(byHead.stepData.mode3.tones.numTonePaths, CS_MODE3_TONE_PATHS_2);
    EXPECT_EQ(byHead.stepData.mode3.tones.antennaPermutationIndex, CS_TEST_MODE3_ANTENNA_PERMUTATION_2);

    CsStepBlob length23;
    length23.Append(CS_STEP_MODE_3, CS_INJECT_CHANNEL_FIRST, mode3Body, CS_MODE3_STEP_LEN_NO_PCT_3);
    const CsStepResult &len23 = CsInjectMode3OneStep(length23, CS_MODE3_NUM_ANTENNA_PATHS_3);
    EXPECT_EQ(len23.stepData.mode3.tones.numTonePaths, CS_MODE3_TONE_PATHS_3);
    EXPECT_FALSE(len23.stepData.mode3.head.hasPct);

    CsStepBlob length27;
    length27.Append(CS_STEP_MODE_3, CS_INJECT_CHANNEL_FIRST, mode3Body, CS_MODE3_STEP_LEN_NO_PCT_4);
    const CsStepResult &len27 = CsInjectMode3OneStep(length27, CS_MODE3_NUM_ANTENNA_PATHS_4);
    EXPECT_EQ(len27.stepData.mode3.tones.numTonePaths, CS_MODE3_TONE_PATHS_4);
    EXPECT_FALSE(len27.stepData.mode3.head.hasPct);

    const CsStepResult &invalidPct =
        CsInjectMode3OneStep(mode3Step, CS_MODE3_NUM_ANTENNA_PATHS_INVALID_FOR_PCT);
    EXPECT_TRUE(invalidPct.stepData.mode3.head.hasPct);
    EXPECT_EQ(invalidPct.stepData.mode3.tones.numTonePaths, CS_MODE3_TONE_PATHS_PCT);
}

static void CsAssertMode0RoleAndNotAvailable(
    uint8_t noSounding, const uint8_t *mode0Initiator)
{
    const uint8_t mode0Reflector[CS_MODE0_REFLECTOR_LEN] = {
        CS_TEST_PACKET_QUALITY,
        CS_TEST_PACKET_RSSI_WIRE,
        CS_TEST_PACKET_ANTENNA
    };
    const uint8_t offsetUnavailable[CS_MODE0_INITIATOR_LEN] = {
        CS_TEST_PACKET_QUALITY,
        CS_TEST_PACKET_RSSI_WIRE,
        CS_TEST_PACKET_ANTENNA,
        static_cast<uint8_t>(CS_TEST_MEASURED_FREQ_OFFSET_UNAVAILABLE_RAW & 0xFF),
        static_cast<uint8_t>((CS_TEST_MEASURED_FREQ_OFFSET_UNAVAILABLE_RAW >> 8) & 0xFF)
    };

    ASSERT_TRUE(StartCsLeTest(CS_ROLE_REFLECTOR, noSounding, CS_STEP_MODE_0));
    g_csResultCapture.Reset();
    CsStepBlob oneStep;
    oneStep.Append(CS_STEP_MODE_0, CS_INJECT_CHANNEL_FIRST, mode0Initiator, CS_MODE0_INITIATOR_LEN);
    HciLeCsSubeventResultEventParam head = CsResultHead(
        CS_DONE_STATUS_OK, CS_DONE_STATUS_OK, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_ONE);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), oneStep);
    EXPECT_EQ(g_csResultCapture.reports, 1);
    EXPECT_EQ(g_csResultCapture.report.role, CS_ROLE_REFLECTOR);
    EXPECT_FALSE(g_csResultCapture.report.steps[0].hasMeasurement) <<
        "the initiator's five-octet mode-0 step decoded under the reflector Role";

    CsStepBlob reflectorStep;
    reflectorStep.Append(CS_STEP_MODE_0, CS_INJECT_CHANNEL_FIRST, mode0Reflector, CS_MODE0_REFLECTOR_LEN);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), reflectorStep);
    EXPECT_EQ(g_csResultCapture.reports, CS_EXPECTED_REPORTS_AFTER_REFLECTOR_STEP);
    EXPECT_TRUE(g_csResultCapture.report.steps[0].hasMeasurement);
    EXPECT_FALSE(g_csResultCapture.report.steps[0].stepData.mode0.hasFreqOffset) <<
        "the reflector's mode-0 step carried a frequency offset";

    g_csResultCapture.Reset();
    head = CsResultHead(CS_DONE_STATUS_OK, CS_DONE_STATUS_OK, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_ONE);
    head.frequencyCompensation = CS_TEST_MEASURED_FREQ_OFFSET_UNAVAILABLE_RAW;
    CsStepBlob unavailable;
    unavailable.Append(CS_STEP_MODE_0, CS_INJECT_CHANNEL_FIRST, offsetUnavailable, CS_MODE0_INITIATOR_LEN);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), unavailable);
    EXPECT_EQ(g_csResultCapture.reports, 1);
    EXPECT_EQ(g_csResultCapture.report.frequencyCompensation, CS_FREQ_OFFSET_UNAVAILABLE);
    EXPECT_EQ(g_csResultCapture.report.steps[0].stepData.mode0.measuredFreqOffset,
        CS_FREQ_OFFSET_UNAVAILABLE);
}

static void CsTeardownResultTest(void)
{
    CsStepBlob noSteps;
    g_csLeTestEndStatus = CS_STATUS_UNSET;
    EXPECT_EQ(CSIF_LeCsTestEnd(), BT_SUCCESS);
    HciLeCsTestEndCompleteEventParam endParam = {};
    InjectCsSubeventFrame(
        HCI_LE_CS_TEST_END_COMPLETE_EVENT, &endParam, sizeof(endParam), noSteps);
    EXPECT_EQ(g_csLeTestEndStatus, HCI_SUCCESS) << "the test end did not complete";
    EXPECT_EQ(CSIF_DeregisterLeCsCallback(), BT_SUCCESS);
}

HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_CsResultReassembly_02302, TestSize.Level1)
{
    if (!BTM_IsControllerSupportLeChannelSounding()) {
        GTEST_SKIP() << "local controller does not expose LE feature bit 46 (Channel Sounding)";
    }
    const uint8_t sounding = CS_SOUNDING_ENABLED;
    const uint8_t noSounding = CS_SOUNDING_DISABLED;
    const uint8_t mode0Initiator[CS_MODE0_INITIATOR_LEN] = {
        CS_TEST_PACKET_QUALITY,
        CS_TEST_PACKET_RSSI_WIRE,
        CS_TEST_PACKET_ANTENNA,
        static_cast<uint8_t>(CS_MEASURED_FREQ_OFFSET_0_01PPM & 0xFF),
        static_cast<uint8_t>((CS_MEASURED_FREQ_OFFSET_0_01PPM >> 8) & 0xFF)
    };

    InitCsLeCallback();
    ASSERT_EQ(CSIF_RegisterLeCsCallback(&g_csLeCallbackList, nullptr), BT_SUCCESS) <<
        "no CS consumer sink, so nothing below could be observed";

    ASSERT_TRUE(StartCsLeTest(CS_ROLE_INITIATOR, noSounding, CS_STEP_MODE_0));
    CsAssertSingleSubeventReport(noSounding, mode0Initiator);
    CsAssertSubeventContinuation(noSounding, mode0Initiator);
    CsAssertAbortedAndInvalidSubeventDrops(mode0Initiator);
    CsAssertMaxStepsBoundAndStreamClean(mode0Initiator);
    CsAssertMode3DecodeContext(sounding);
    CsAssertMode0RoleAndNotAvailable(noSounding, mode0Initiator);
    CsTeardownResultTest();
}


// @tc.number: StackGapLe6_0_CsTestLifecycle_02303
// @tc.name:  the CS test lifecycle: parameter surface, the two latches, and the 0x33 that ends it
// @tc.desc:  8.3 test lifecycle, driven on the test channel through the public CSIF_ surface
//            (7.8.142 pp2840-2848, 7.8.143 p2855, 7.7.65,46 p2463).
//            cs_test.c holds the one test that can exist at a time in g_test, which no ACL row
//            reaches, so a single board can drive the whole sequence: start with 0x0095, feed
//            the result stream, end with 0x0096, and close with the 0x33 the Controller sends
//            once every result of the ended test has been reported.
//            The case pins down the two sources of a refusal, because they look the
//            same to the caller and are not: while a command is outstanding the engine refuses
//            locally (BT_ALREADY, and the command never reaches the wire), whereas "a test is
//            already enabled" is the Controller's answer - 0x0C in the 0x0095 Command_Complete -
//            and only exists at all once the command was sent. cs_le_if.h says the same on
//            testComplete.
//            Covered: the field families HCI_LeCsTest rejects before a wire buffer is
//            allocated, and that a rejected start leaves no latch behind; a second 0x0095 while
//            one is outstanding; a 0x0096 while one is outstanding, and a 0x0095 arriving while
//            a 0x0096 still is; the 0x0C start, which must not overwrite the running test's
//            Role; the result stream that keeps decoding between the 0x0096 and its 0x33; the
//            two completions that must act on nothing they are not attributable to; and a
//            failed end, which keeps the test running and still ends its 0x0096.
static void CsAssertNoTestContextRejects(
    const CsStepBlob &oneStep, const CsStepBlob &noSteps,
    const HciLeCsSubeventResultEventParam &head, uint8_t disallowed)
{
    ResetCsCompletions();
    g_csResultCapture.Reset();
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), oneStep);
    EXPECT_EQ(g_csTally.subeventResult, 1) << "the 0x31 never reached the event layer";
    EXPECT_EQ(g_csResultCapture.reports, 0) <<
        "a result was decoded with no test running, so it had no Role to decode against";

    const uint8_t unset = CS_STATUS_UNSET;
    g_csLeTestEndStatus = unset;
    EXPECT_EQ(CSIF_LeCsTestEnd(), BT_SUCCESS);
    HciLeCsTestEndCompleteEventParam endRefused = {};
    endRefused.status = disallowed;
    InjectCsSubeventFrame(HCI_LE_CS_TEST_END_COMPLETE_EVENT, &endRefused, sizeof(endRefused),
        noSteps);
    EXPECT_EQ(g_csLeTestEndStatus, disallowed);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), oneStep);
    EXPECT_EQ(g_csResultCapture.reports, 0) << "the failed end created a test";
}

static void CsAssertTestStartParameterSurface(uint8_t noSounding, uint8_t unset)
{
    for (int variant = 0; variant < CS_TEST_START_VARIANT_COUNT; variant++) {
        CsLeTestParam param = {};
        FillCsLeTestParam(CS_ROLE_INITIATOR, noSounding, CS_STEP_MODE_0, &param);
        switch (static_cast<CsTestStartVariant>(variant)) {
            case CS_TEST_VARIANT_INVALID_MAIN_MODE: param.prefix.mainModeType = CS_TEST_MAIN_MODE_INVALID; break;
            case CS_TEST_VARIANT_INVALID_ROLE: param.prefix.role = CS_ROLE_INVALID; break;
            case CS_TEST_VARIANT_NONZERO_RESERVED: param.prefix.reserved = CS_TEST_RESERVED_NONZERO; break;
            case CS_TEST_VARIANT_OVERRIDE_TOO_SHORT:
                param.prefix.overrideParametersLength = CS_TEST_OVERRIDE_LENGTH_TOO_SHORT;
                break;
            case CS_TEST_VARIANT_INVALID_TIP2: param.prefix.tIp2Time = CS_TEST_T_IP2_TIME_ABOVE_MAX; break;
            default: param.overrideParametersData = nullptr; break;
        }
        g_csLeTestStatus = unset;
        EXPECT_EQ(CSIF_LeCsTestStart(&param), BT_BAD_PARAM) << "accepted variant " << variant;
        EXPECT_EQ(g_csLeTestStatus, unset) << "variant " << variant << " reached the wire";
        ASSERT_TRUE(StartCsLeTest(CS_ROLE_INITIATOR, noSounding, CS_STEP_MODE_0)) <<
            "variant " << variant << " left its latch behind";
    }
}

static void CsAssertOutstandingStartRefused(uint8_t noSounding, uint8_t success, uint8_t unset)
{
    CsLeTestParam pending = {};
    FillCsLeTestParam(CS_ROLE_INITIATOR, noSounding, CS_STEP_MODE_0, &pending);
    g_csLeTestStatus = unset;
    EXPECT_EQ(CSIF_LeCsTestStart(&pending), BT_SUCCESS);
    EXPECT_EQ(CSIF_LeCsTestStart(&pending), BT_ALREADY) <<
        "a second 0x0095 was sent while the first one's Command_Complete was outstanding";

    EXPECT_EQ(g_csLeTestStatus, unset) << "the refused second 0x0095 produced a completion";
    HciLeCsTestReturnParam pendingReturn = {};
    HciEventOnLeCommandComplete(HCI_LE_CS_TEST, &pendingReturn, sizeof(pendingReturn));
    EXPECT_EQ(g_csLeTestStatus, success) << "the outstanding 0x0095 lost its Command_Complete";
}

static void CsAssertRefusedStartDoesNotTouchRole(
    uint8_t noSounding, const CsStepBlob &oneStep, const CsStepBlob &reflectorStep,
    const HciLeCsSubeventResultEventParam &head, uint8_t disallowed)
{
    ASSERT_TRUE(StartCsLeTest(CS_ROLE_REFLECTOR, noSounding, CS_STEP_MODE_0, disallowed));
    g_csResultCapture.Reset();
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), oneStep);
    EXPECT_EQ(g_csResultCapture.reports, 1);
    EXPECT_EQ(g_csResultCapture.report.role, CS_ROLE_INITIATOR) <<
        "the refused 0x0095 replaced the running test's Role";

    EXPECT_TRUE(g_csResultCapture.report.steps[0].hasMeasurement);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &head, sizeof(head), reflectorStep);
    EXPECT_EQ(g_csResultCapture.reports, CS_EXPECTED_REPORTS_AFTER_REFLECTOR_STEP);
    EXPECT_FALSE(g_csResultCapture.report.steps[0].hasMeasurement) <<
        "the running test's Role was replaced by the one the refused 0x0095 asked for";
}

// Inputs shared by the lifecycle test helpers that assert end-of-test stream behavior.
struct CsLifecycleInputs {
    uint8_t noSounding;
    const CsStepBlob &oneStep;
    const CsStepBlob &noSteps;
    const HciLeCsSubeventResultEventParam &head;
    uint8_t success;
    uint8_t disallowed;
    uint8_t unset;
};

static void CsAssertResultStreamUntilTestEnd(const CsLifecycleInputs &in)
{
    g_csResultCapture.Reset();
    g_csLeTestEndStatus = in.unset;
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &in.head, sizeof(in.head), in.oneStep);
    EXPECT_EQ(g_csResultCapture.reports, 1);

    EXPECT_EQ(CSIF_LeCsTestEnd(), BT_SUCCESS);
    EXPECT_EQ(CSIF_LeCsTestEnd(), BT_ALREADY) <<
        "a second 0x0096 was sent while the first one's 0x33 was outstanding";

    EXPECT_EQ(g_csLeTestEndStatus, in.unset);
    CsLeTestParam pending = {};
    FillCsLeTestParam(CS_ROLE_INITIATOR, in.noSounding, CS_STEP_MODE_0, &pending);
    EXPECT_EQ(CSIF_LeCsTestStart(&pending), BT_ALREADY) <<
        "a 0x0095 was sent while a 0x0096 was still on the wire";

    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &in.head, sizeof(in.head), in.oneStep);
    EXPECT_EQ(g_csResultCapture.reports, CS_EXPECTED_REPORTS_STREAM_TAIL) <<
       "the tail of the result stream was dropped";

    g_csLeTestEndStatus = in.unset;
    HciLeCsTestEndCompleteEventParam endOk = {};
    endOk.status = in.success;
    InjectCsSubeventFrame(HCI_LE_CS_TEST_END_COMPLETE_EVENT, &endOk, sizeof(endOk), in.noSteps);
    EXPECT_EQ(g_csLeTestEndStatus, in.success);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &in.head, sizeof(in.head), in.oneStep);
    EXPECT_EQ(g_csResultCapture.reports, CS_EXPECTED_REPORTS_STREAM_TAIL) <<
        "the test record survived its own 0x33, so results are still being decoded";
}

static void CsAssertStrayCompletionsDropped(uint8_t success, const CsStepBlob &noSteps)
{
    ResetCsCompletions();
    g_csLeTestStatus = CS_STATUS_UNSET;
    HciLeCsTestReturnParam strayReturn = {};
    HciEventOnLeCommandComplete(HCI_LE_CS_TEST, &strayReturn, sizeof(strayReturn));
    EXPECT_EQ(g_csTally.test, 1);
    EXPECT_EQ(g_csLeTestStatus, CS_STATUS_UNSET) <<
        "an unattributable 0x0095 completion reached the sink";

    ResetCsCompletions();
    g_csLeTestEndStatus = CS_STATUS_UNSET;
    HciLeCsTestEndCompleteEventParam endOk = {};
    endOk.status = success;
    InjectCsSubeventFrame(HCI_LE_CS_TEST_END_COMPLETE_EVENT, &endOk, sizeof(endOk), noSteps);
    EXPECT_EQ(g_csTally.testEnd, 1);
    EXPECT_EQ(g_csLeTestEndStatus, CS_STATUS_UNSET) << "an unattributable 0x33 reached the sink";
}

static void CsAssertFailedEndKeepsStreamRunning(const CsLifecycleInputs &in)
{
    ASSERT_TRUE(StartCsLeTest(CS_ROLE_INITIATOR, in.noSounding, CS_STEP_MODE_0));
    g_csResultCapture.Reset();
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &in.head, sizeof(in.head), in.oneStep);
    EXPECT_EQ(g_csResultCapture.reports, 1);

    g_csLeTestEndStatus = in.unset;
    EXPECT_EQ(CSIF_LeCsTestEnd(), BT_SUCCESS);
    HciLeCsTestEndCompleteEventParam endRefused = {};
    endRefused.status = in.disallowed;
    InjectCsSubeventFrame(HCI_LE_CS_TEST_END_COMPLETE_EVENT, &endRefused, sizeof(endRefused),
        in.noSteps);
    EXPECT_EQ(g_csLeTestEndStatus, in.disallowed);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &in.head, sizeof(in.head), in.oneStep);
    EXPECT_EQ(g_csResultCapture.reports, CS_EXPECTED_REPORTS_AFTER_FAILED_END) <<
     "a failed end stopped the result stream";
    EXPECT_EQ(CSIF_LeCsTestEnd(), BT_SUCCESS) <<
        "the refused end left its latch behind, so no later end could be sent";

    HciLeCsTestEndCompleteEventParam endOk = {};
    endOk.status = in.success;
    InjectCsSubeventFrame(HCI_LE_CS_TEST_END_COMPLETE_EVENT, &endOk, sizeof(endOk), in.noSteps);
    EXPECT_EQ(g_csLeTestEndStatus, in.success);
    InjectCsSubeventFrame(HCI_LE_CS_SUBEVENT_RESULT_EVENT, &in.head, sizeof(in.head), in.oneStep);
    EXPECT_EQ(g_csResultCapture.reports, CS_EXPECTED_REPORTS_AFTER_END_OK);
}

HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_CsTestLifecycle_02303, TestSize.Level1)
{
    if (!BTM_IsControllerSupportLeChannelSounding()) {
        GTEST_SKIP() << "local controller does not expose LE feature bit 46 (Channel Sounding)";
    }
    const uint8_t noSounding = CS_SOUNDING_DISABLED;
    const uint8_t success = HCI_SUCCESS;
    const uint8_t disallowed = HCI_COMMAND_DISALLOWED;
    const uint8_t unset = CS_STATUS_UNSET;

    const uint8_t mode0Initiator[CS_MODE0_INITIATOR_LEN] = {
        CS_TEST_PACKET_QUALITY,
        CS_TEST_PACKET_RSSI_WIRE,
        CS_TEST_PACKET_ANTENNA,
        static_cast<uint8_t>(CS_MEASURED_FREQ_OFFSET_0_01PPM & 0xFF),
        static_cast<uint8_t>((CS_MEASURED_FREQ_OFFSET_0_01PPM >> 8) & 0xFF)
    };
    const uint8_t mode0Reflector[CS_MODE0_REFLECTOR_LEN] = {
        CS_TEST_PACKET_QUALITY,
        CS_TEST_PACKET_RSSI_WIRE,
        CS_TEST_PACKET_ANTENNA
    };

    CsStepBlob oneStep;
    oneStep.Append(CS_STEP_MODE_0, CS_INJECT_CHANNEL_FIRST, mode0Initiator, sizeof(mode0Initiator));
    CsStepBlob reflectorStep;
    reflectorStep.Append(CS_STEP_MODE_0, CS_INJECT_CHANNEL_FIRST, mode0Reflector, CS_MODE0_REFLECTOR_LEN);
    CsStepBlob noSteps;
    HciLeCsSubeventResultEventParam head = CsResultHead(
        CS_DONE_STATUS_OK, CS_DONE_STATUS_OK, CS_MODE3_NUM_ANTENNA_PATHS_PCT, CS_NUM_STEPS_ONE);
    HciLeCsTestEndCompleteEventParam endOk = {};
    endOk.status = success;
    HciLeCsTestEndCompleteEventParam endRefused = {};
    endRefused.status = disallowed;

    InitCsLeCallback();
    ASSERT_EQ(CSIF_RegisterLeCsCallback(&g_csLeCallbackList, nullptr), BT_SUCCESS) <<
        "no CS consumer sink, so nothing below could be observed";

    CsAssertNoTestContextRejects(oneStep, noSteps, head, disallowed);
    CsAssertTestStartParameterSurface(noSounding, unset);
    CsAssertOutstandingStartRefused(noSounding, success, unset);
    CsAssertRefusedStartDoesNotTouchRole(noSounding, oneStep, reflectorStep, head, disallowed);
    CsLifecycleInputs inputs = {noSounding, oneStep, noSteps, head, success, disallowed, unset};
    CsAssertResultStreamUntilTestEnd(inputs);
    CsAssertStrayCompletionsDropped(success, noSteps);
    CsAssertFailedEndKeepsStreamRunning(inputs);

    EXPECT_EQ(CSIF_LeCsTestStart(nullptr), BT_BAD_PARAM);
    EXPECT_EQ(CSIF_DeregisterLeCsCallback(), BT_SUCCESS);
}


// @tc.number: StackGapLe6_0_CsTestMalformedStart_02307
// @tc.name:  a malformed 0x0095 completion still clears the start latch
// @tc.desc:  Round 4 F10, the half that needs the engine. cs_test.c latches the 0x0095 in
//            requestPending and only its own completion clears it (cs_test.c:193-197), so a
//            completion that is never attributed wedges CsTestStart with BT_ALREADY until the
//            module is disabled. A 0x0095 Command_Complete whose return body is empty is
//            answered by the failure row with HCI_MALFORMED_EVENT (hci_cmd.c:663), and that
//            row is what this case drives - the common downstream of all three failure call
//            sites (02301), reached on this one without a controller answer. The
//            interception itself only fires for a command still tracked in g_processingCmds,
//            which needs a real HCI credit round trip, so it is driven one level below.
//            The clearance is read off a second start: BT_ALREADY means the latch is still
//            set, BT_SUCCESS means the malformed completion closed it.
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_CsTestMalformedStart_02307, TestSize.Level1)
{
    if (!BTM_IsControllerSupportLeChannelSounding()) {
        GTEST_SKIP() << "local controller does not expose LE feature bit 46 (Channel Sounding)";
    }
    const uint8_t noSounding = CS_SOUNDING_DISABLED;  // RTT_Type RTT, no sounding sequence
    const uint8_t success = HCI_SUCCESS;
    const uint8_t malformed = HCI_MALFORMED_EVENT;  // hci_error.h

    InitCsLeCallback();
    ASSERT_EQ(CSIF_RegisterLeCsCallback(&g_csLeCallbackList, nullptr), BT_SUCCESS) <<
        "no CS consumer sink, so nothing below could be observed";

    CsLeTestParam pending = {};
    FillCsLeTestParam(CS_ROLE_INITIATOR, noSounding, CS_STEP_MODE_0, &pending);
    g_csLeTestStatus = CS_STATUS_UNSET;  // a value no callback writes, so "not called" is visible
    ResetCsCompletions();
    ASSERT_EQ(CSIF_LeCsTestStart(&pending), BT_SUCCESS) << "the 0x0095 never left the engine";
    EXPECT_EQ(CSIF_LeCsTestStart(&pending), BT_ALREADY) << "the request was not latched";

    HciOnLeControllerCmdFailed(HCI_LE_CS_TEST, malformed, nullptr);
    EXPECT_EQ(g_csTally.test, 1) << "the failure row did not close the 0x0095";
    EXPECT_EQ(g_csIdentity.status, malformed);
    EXPECT_EQ(g_csLeTestStatus, malformed) << "the malformed status did not reach the sink";
    EXPECT_EQ(CSIF_LeCsTestStart(&pending), BT_SUCCESS) <<
        "the latch was left behind, so no later 0x0095 could be sent";

    // Close both latches out, so nothing this case leaves behind is read by a later one: the
    // start above is committed by a normal completion, and the test it commits is ended.
    HciLeCsTestReturnParam startOk = {};
    startOk.status = success;
    HciEventOnLeCommandComplete(HCI_LE_CS_TEST, &startOk, sizeof(startOk));
    EXPECT_EQ(g_csLeTestStatus, success);
    CsStepBlob noSteps;
    HciLeCsTestEndCompleteEventParam endOk = {};
    endOk.status = success;
    g_csLeTestEndStatus = CS_STATUS_UNSET;
    ASSERT_EQ(CSIF_LeCsTestEnd(), BT_SUCCESS);
    InjectCsSubeventFrame(HCI_LE_CS_TEST_END_COMPLETE_EVENT, &endOk, sizeof(endOk), noSteps);
    EXPECT_EQ(g_csLeTestEndStatus, success) << "the 0x0096 was left outstanding";

    EXPECT_EQ(CSIF_DeregisterLeCsCallback(), BT_SUCCESS);
}
}  // namespace Bluetooth
}  // namespace OHOS
