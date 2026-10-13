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

// BT 6.0 dual-board interaction tests (00100-00510). Channel Sounding dual-board
// cases live in stack_gap_le_6_0_interact_cs_test.cpp. This file does NOT provide
// main(): it shares the gtest executable with stack_gap_le_interact_test.cpp.


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
#include "l2cap_def.h"
#include "l2cap_le_if.h"
#include "securec.h"
#include "stack_gap_le_6_0_interact_test.h"

using namespace testing::ext;

// BT 6.0 stack dual-board interaction tests: Read Remote Features, DBAF,
// Frame Space Update and ACL setup helpers. Channel Sounding dual-board cases
// have been split into stack_gap_le_6_0_interact_cs_test.cpp to keep each
// source file within the size limit.

namespace OHOS {
namespace Bluetooth {

namespace StackGapLe60InteractTestInternal {


// Two-device state, all anonymous-namespace internal linkage so the same names
// used by the interact / 5.2 translation units cannot collide at link time.
char g_peerAddrArg[BT60_ADDR_ARG_CAP] = { 0 };
BtAddr g_peerAddr = { };
bool g_peerAddrValid = false;
LeConnContext g_connCtx;
bool g_connected = false;
bool g_btmInitialized = false;
bool g_btmEnabled = false;
bool g_aclCallbacksRegistered = false;
bool g_hciCallbacksRegistered = false;

ReadFeaturesCapture g_readFeaturesCapture;

// Copy BT60_PEER_ADDR into g_peerAddrArg; an over-long value leaves the buffer
// empty and ParsePeerAddr rejects it either way. Fixed array: a std::string
// global would violate the no-non-POD-global red line (rule 1).
void InitPeerAddrArg()
{
    const char *env = std::getenv("BT60_PEER_ADDR");
    if (env == nullptr) {
        return;
    }
    size_t len = strlen(env);
    if (len == 0 || len >= sizeof(g_peerAddrArg)) {
        return;
    }
    errno_t ret = memcpy_s(g_peerAddrArg, sizeof(g_peerAddrArg), env, len + 1);
    if (ret != EOK) {
        g_peerAddrArg[0] = '\0';
    }
}

// BtAddr.addr[] is little-endian (addr[5] printed first). Accept an optional
// "public:" / "random:" prefix followed by the conventional "11:22:33:44:55:66"
// form, with the leftmost ASCII byte stored into addr[BT_ADDRESS_SIZE - 1].
bool ParsePeerAddr(const char *str, BtAddr *addr)
{
    if (str == nullptr || addr == nullptr) {
        return false;
    }
    uint8_t type = BT_PUBLIC_DEVICE_ADDRESS;
    const char *value = str;
    if (strncmp(str, "public:", BT60_ADDR_PREFIX_LEN) == 0) {
        value = str + BT60_ADDR_PREFIX_LEN;
    } else if (strncmp(str, "random:", BT60_ADDR_PREFIX_LEN) == 0) {
        type = BT_RANDOM_DEVICE_ADDRESS;
        value = str + BT60_ADDR_PREFIX_LEN;
    }
    if (strlen(value) != BT60_ADDR_STRING_LEN) {
        return false;
    }
    for (size_t i = BT60_ADDR_SEPARATOR_STEP - 1; i < BT60_ADDR_STRING_LEN;
         i += BT60_ADDR_SEPARATOR_STEP) {
        if (value[i] != ':') {
            return false;
        }
    }
    auto hexValue = [](char c) -> int {
        if (c >= '0' && c <= '9') {
            return c - '0';
        }
        if (c >= 'a' && c <= 'f') {
            return c - 'a' + 0x0A;
        }
        if (c >= 'A' && c <= 'F') {
            return c - 'A' + 0x0A;
        }
        return -1;
    };
    for (int i = 0; i < BT_ADDRESS_SIZE; i++) {
        const char *byteStr = value + i * BT60_ADDR_SEPARATOR_STEP;
        int high = hexValue(byteStr[0]);
        int low = hexValue(byteStr[1]);
        if (high < 0 || low < 0) {
            return false;
        }
        addr->addr[BT_ADDRESS_SIZE - 1 - i] = static_cast<uint8_t>((high << HEX_DIGIT_SHIFT) | low);
    }
    addr->type = type;
    return true;
}

// ---- HCI read-completion capture (handlers must not block or (de)register) ----
void OnLegacyReadRemoteFeaturesComplete(const HciLeReadRemoteFeaturesCompleteEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_readFeaturesCapture.OnLegacy(eventParam);
}

void OnAllReadRemoteFeaturesComplete(const HciLeReadAllRemoteFeaturesCompleteEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_readFeaturesCapture.OnAll(eventParam);
}

// HCI_RegisterEventCallbacks stores the struct pointer; it must be static.
HciEventCallbacks g_hciReadCallbacks = { };

void OnLeConnectionComplete(uint8_t status, uint16_t connectionHandle, const BtAddr *addr, uint8_t role,
    void *context)
{
    (void)addr;
    (void)role;
    LeConnContext *ctx = static_cast<LeConnContext *>(context);
    ctx->status = status;
    ctx->handle = connectionHandle;
    ctx->Notify();
}

void OnLeDisconnectionComplete(uint8_t status, uint16_t connectionHandle, uint8_t reason, void *context)
{
    (void)connectionHandle;
    (void)reason;
    LeConnContext *ctx = static_cast<LeConnContext *>(context);
    ctx->status = status;
    ctx->Notify();
}

// BTM_RegisterAclCallbacks stores the struct pointer; it must be static.
BtmAclCallbacks g_aclCallbacks = {
    .leConnectionComplete = OnLeConnectionComplete,
    .leDisconnectionComplete = OnLeDisconnectionComplete,
};

// Establish a real LE connection to the peer and return the actual connection handle.
bool ConnectToPeer(uint16_t &connHandle)
{
    if (g_connected) {
        connHandle = g_connCtx.handle;
        return true;
    }
    if (!g_peerAddrValid) {
        return false;
    }
    g_connCtx.Reset();
    // 30 ms interval, 0 latency, 5 s supervision (same as interact/5.2).
    L2capLeConnectionParameter connParam = { 0x0018, 0x0018, 0, 0x01F4 };
    if (L2CIF_LeConnect(&g_peerAddr, &connParam, nullptr) != BT_SUCCESS) {
        printf("ConnectToPeer: L2CIF_LeConnect failed\n");
        return false;
    }
    if (!g_connCtx.Wait()) {
        printf("ConnectToPeer: wait leConnectionComplete timeout\n");
        return false;
    }
    g_connected = (g_connCtx.status == HCI_STATUS_SUCCESS);
    if (!g_connected) {
        printf("ConnectToPeer: connection failed, status=0x%02X\n", g_connCtx.status);
    }
    connHandle = g_connCtx.handle;
    return g_connected;
}

// Tear the link down; L2CIF_LeDisconnect is void, so wait for the ACL
// disconnection-complete through the same g_connCtx waiter as the connect.
void DisconnectFromPeer()
{
    if (!g_connected) {
        return;
    }
    g_connCtx.Reset();
    L2CIF_LeDisconnect(g_connCtx.handle, nullptr);
    if (!g_connCtx.Wait()) {
        printf("DisconnectFromPeer: wait disconnection-complete timeout\n");
    }
    g_connected = false;
}

// Page 0 = octets 0..7 of the reported LE_Features.
bool Page0NonZero(const uint8_t *leFeaturesRaw)
{
    for (int i = 0; i < LE_FEATURES_PAGE0_SIZE; i++) {
        if (leFeaturesRaw[i] != 0) {
            return true;
        }
    }
    return false;
}

void OnDbafScanSetParamResult(uint8_t status, void *context)
{
    auto *ctx = static_cast<DbafScanCtx *>(context);
    ctx->paramStatus = status;
    ctx->paramWaiter.Notify();
}

void OnDbafScanSetEnableResult(uint8_t status, void *context)
{
    auto *ctx = static_cast<DbafScanCtx *>(context);
    ctx->enableStatus = status;
    ctx->enableWaiter.Notify();
}

// 0x0081 LE Set Decision Instructions completes through the extended-scan
// callback (gap_le_scan.c GapLeSetDecisionInstructionsComplete).
void OnDbafDecisionInstructionsResult(uint8_t status, void *context)
{
    auto *ctx = static_cast<DbafScanCtx *>(context);
    ctx->instrStatus = status;
    ctx->instrWaiter.Notify();
}

// True when the report data carries the plain-set "BTPEER" marker.
bool DbafReportHasMarker(const uint8_t *data, uint8_t dataLen)
{
    if (data == nullptr || dataLen < DBAF_PLAIN_MARKER_LEN) {
        return false;
    }
    for (uint8_t i = 0; static_cast<size_t>(i) + DBAF_PLAIN_MARKER_LEN <= dataLen; ++i) {
        if (memcmp(data + i, DBAF_PLAIN_MARKER, DBAF_PLAIN_MARKER_LEN) == 0) {
            return true;
        }
    }
    return false;
}

// Count peer reports by the plain-set marker. Runs on the GAP thread with the data
// valid only for the callback duration, so the marker test happens in place and only
// the counters are shared (atomic).
void OnDbafExAdvReport(uint8_t advType, const BtAddr *addr, GapExAdvReportParam reportParam,
    const BtAddr *currentAddr, void *context)
{
    (void)advType;
    (void)currentAddr;
    if (addr == nullptr || !g_peerAddrValid) {
        return;
    }
    if (memcmp(addr->addr, g_peerAddr.addr, BT_ADDRESS_SIZE) != 0) {
        return;
    }
    auto *ctx = static_cast<DbafScanCtx *>(context);
    if (DbafReportHasMarker(reportParam.data, reportParam.dataLen)) {
        ctx->connReports++;
    } else {
        ctx->decisionReports++;
    }
}

bool DbafStopScan(DbafScanCtx *ctx);  // defined below; used by the guard's destructor

// Stops any scan and deregisters the extended-scan callback on scope exit so no
// late report can reach a destroyed context (deregister only after a stop).
class DbafScanCallbackGuard {
public:
    explicit DbafScanCallbackGuard(DbafScanCtx *ctx) : ctx_(ctx) {}
    ~DbafScanCallbackGuard()
    {
        if (!registered_) {
            return;
        }
        (void)DbafStopScan(ctx_);
        if (GAPIF_DeregisterExScanCallback() != BT_SUCCESS) {
            printf("DbafScanCallbackGuard: deregister failed; callback may remain registered\n");
        }
        registered_ = false;
    }
    DbafScanCallbackGuard(const DbafScanCallbackGuard &) = delete;
    DbafScanCallbackGuard &operator=(const DbafScanCallbackGuard &) = delete;

    bool registered_ = false;

private:
    DbafScanCtx *ctx_;
};

bool DbafRegisterScanCallback(DbafScanCtx *ctx, DbafScanCallbackGuard &guard)
{
    GapExScanCallback callback = { };
    callback.exAdvertisingReport = OnDbafExAdvReport;
    callback.scanExSetParamResult = OnDbafScanSetParamResult;
    callback.scanExSetEnableResult = OnDbafScanSetEnableResult;
    callback.scanSetDecisionInstructionsResult = OnDbafDecisionInstructionsResult;
    if (GAPIF_RegisterExScanCallback(&callback, ctx) != BT_SUCCESS) {
        printf("DbafRegisterScanCallback: register failed\n");
        return false;
    }
    guard.registered_ = true;
    return true;
}

// Install one decision test for Arbitrary Data of exactly 2 octets and wait for
// its command-complete. mask/target are the 2 used octets of the 8-octet fields.
bool DbafInstallArbitrary2(DbafScanCtx *ctx, uint8_t mask0, uint8_t mask1, uint8_t target0, uint8_t target1)
{
    HciLeDecisionTestParam test = { };
    test.flags = LE_DECISION_TEST_FLAG_NEW_GROUP | LE_DECISION_TEST_FLAG_PASS_IF_CHECK;
    test.field = DBAF_INSTR_FIELD_EXACT2;
    test.parameters[DBAF_INSTR_PARAM_MASK0] = mask0;
    test.parameters[DBAF_INSTR_PARAM_MASK1] = mask1;
    test.parameters[DBAF_INSTR_PARAM_TARGET0] = target0;
    test.parameters[DBAF_INSTR_PARAM_TARGET1] = target1;
    ctx->instrWaiter.Reset();
    if (GAPIF_LeSetDecisionInstructions(1, &test) != BT_SUCCESS) {
        printf("DbafInstallArbitrary2: GAPIF send failed\n");
        return false;
    }
    if (!ctx->instrWaiter.Wait()) {
        printf("DbafInstallArbitrary2: no command-complete\n");
        return false;
    }
    if (ctx->instrStatus != HCI_STATUS_SUCCESS) {
        printf("DbafInstallArbitrary2: status=0x%02X\n", ctx->instrStatus);
        return false;
    }
    return true;
}

bool DbafStopScan(DbafScanCtx *ctx)
{
    if (!ctx->scanEnabled.load()) {
        return true;
    }
    ctx->enableWaiter.Reset();
    if (GAPIF_LeExScanSetEnable(0x00, 0x00, 0, 0) != BT_SUCCESS) {
        return false;
    }
    if (!ctx->enableWaiter.Wait() || ctx->enableStatus != HCI_STATUS_SUCCESS) {
        return false;
    }
    ctx->scanEnabled = false;
    return true;
}

bool DbafStartScan(DbafScanCtx *ctx)
{
    ctx->enableWaiter.Reset();
    if (GAPIF_LeExScanSetEnable(0x01, 0x00, 0, 0) != BT_SUCCESS) {
        return false;
    }
    if (!ctx->enableWaiter.Wait() || ctx->enableStatus != HCI_STATUS_SUCCESS) {
        return false;
    }
    ctx->scanEnabled = true;
    return true;
}

bool DbafSetScanMode(DbafScanCtx *ctx, uint8_t scanFilterPolicy)
{
    // Passive scan, 10 ms interval / window, over the air without PHY preference.
    GapLeScanParam scanParam = { 0x00, { 0x0010, 0x0010 } };
    ctx->paramWaiter.Reset();
    if (GAPIF_LeExScanSetParam(scanFilterPolicy, GAP_EX_SCAN_PHY_1M, &scanParam) != BT_SUCCESS) {
        return false;
    }
    return ctx->paramWaiter.Wait() && ctx->paramStatus == HCI_STATUS_SUCCESS;
}

// Scan for windowMs under the given decision mode (0x0041 bits 2-3) and snapshot
// the per-set report counts. A mode change needs a scan stop in between.
bool DbafRunScanSweep(DbafScanCtx *ctx, uint8_t mode, uint32_t windowMs, DbafSweepResult &out)
{
    if (!DbafStopScan(ctx)) {
        printf("DbafRunScanSweep: stop failed before mode 0x%02X\n", mode);
        return false;
    }
    if (!DbafSetScanMode(ctx, mode)) {
        printf("DbafRunScanSweep: set mode 0x%02X failed\n", mode);
        return false;
    }
    ctx->connReports = 0;
    ctx->decisionReports = 0;
    if (!DbafStartScan(ctx)) {
        printf("DbafRunScanSweep: start scan failed\n");
        return false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(windowMs));
    if (!DbafStopScan(ctx)) {
        printf("DbafRunScanSweep: stop failed after window\n");
        return false;
    }
    out.conn = ctx->connReports.load();
    out.decision = ctx->decisionReports.load();
    printf("DbafRunScanSweep: mode=0x%02X window=%u ms conn=%d decision=%d\n", mode, windowMs, out.conn,
        out.decision);
    return true;
}

const char *DbafGateSkipMessage(DbafGate gate)
{
    switch (gate) {
        case K_NO_PEER_ADDR:
            return "no peer address (BT60_PEER_ADDR=XX:XX:XX:XX:XX:XX [public:|random:] prefix)";
        case K_LOCAL_NO_DBAF:
            return "local controller lacks LE feature bit 42 (Decision-Based Advertising Filtering)";
        case K_PEER_CONNECT_FAILED:
            return "could not connect to peer (run './btfw_stack_unit_test dbaf-server' on board B)";
        case K_PEER_READ_FAILED:
            return "peer read-remote-features completion did not arrive";
        case K_PEER_NO_DBAF:
            return "peer controller lacks LE feature bit 42 (Decision-Based Advertising Filtering)";
        default:
            return "";
    }
}

// Probe both boards for DBAF support and report the outcome as a skip gate. No
// ASSERT is used here: the case maps the enum onto a visible GTEST_SKIP instead.
DbafGate DbafCheckCapability()
{
    if (!g_peerAddrValid) {
        return K_NO_PEER_ADDR;
    }
    if (!BTM_IsControllerSupportLeDecisionBasedAdvertisingFiltering()) {
        return K_LOCAL_NO_DBAF;
    }
    g_readFeaturesCapture.Reset();
    uint16_t connHandle = 0;
    if (!ConnectToPeer(connHandle)) {
        return K_PEER_CONNECT_FAILED;
    }
    if (!g_readFeaturesCapture.WaitForAny(WAIT_CALLBACK_TIMEOUT_MS)) {
        DisconnectFromPeer();
        return K_PEER_READ_FAILED;
    }
    uint8_t status = HCI_STATUS_SUCCESS;
    const uint8_t *features = nullptr;
    if (g_readFeaturesCapture.LegacyReceived()) {
        const auto param = g_readFeaturesCapture.LegacyParam();
        status = param.status;
        features = param.leFeatures.raw;
    } else {
        const auto param = g_readFeaturesCapture.AllParam();
        status = param.status;
        features = param.leFeatures.raw;
    }
    // bit 42 sits in page-0 octet 5, so both the 8-byte legacy and the 248-byte
    // all-features containers index it the same way.
    bool peerDbaf = (status == HCI_STATUS_SUCCESS) &&
        GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_DECISION_BASED_ADVERTISING_FILTERING);
    DisconnectFromPeer();
    if (status != HCI_STATUS_SUCCESS) {
        return K_PEER_READ_FAILED;
    }
    return peerDbaf ? K_DBAF_READY : K_PEER_NO_DBAF;
}

// Issue a decision-based connect (7.8.66) and wait for a successful connection.
bool DbafDecisionConnectSucceed(uint8_t policy)
{
    g_connCtx.Reset();
    if (GAPIF_LeExtendedConnectByDecision(policy) != BT_SUCCESS) {
        printf("DbafDecisionConnectSucceed: GAPIF send failed\n");
        return false;
    }
    if (!g_connCtx.Wait() || g_connCtx.status != HCI_STATUS_SUCCESS) {
        printf("DbafDecisionConnectSucceed: no successful connection-complete\n");
        return false;
    }
    g_connected = true;
    return true;
}

// Drain window after a decision connect whose instruction cannot match. Returns 0
// when no completion arrived (initiator still pending), 1 when it ended with an
// error-status connection-complete, and -1 when a success unexpectedly completed.
int DbafDrainNoConnectWindow(uint32_t windowMs)
{
    if (!g_connCtx.Wait(windowMs)) {
        return 0;
    }
    return (g_connCtx.status == HCI_STATUS_SUCCESS) ? -1 : 1;
}
}  // namespace StackGapLe60InteractTestInternal


void StackGapLe60InteractTest::SetUpTestCase(void)
{
    ASSERT_EQ(BTM_Initialize(), BT_SUCCESS);
    g_btmInitialized = true;
    ASSERT_EQ(BTM_Enable(LE_CONTROLLER), BT_SUCCESS);
    g_btmEnabled = true;
    ASSERT_TRUE(BTM_IsEnabled(LE_CONTROLLER));
    ASSERT_EQ(GAPIF_LeSetRole(GAP_LE_ROLE_CENTRAL), BT_SUCCESS);

    InitPeerAddrArg();
    g_peerAddrValid = ParsePeerAddr(g_peerAddrArg, &g_peerAddr);
    if (!g_peerAddrValid) {
        printf("peer address not set (BT60_PEER_ADDR=XX:XX:XX:XX:XX:XX [public:|random:] prefix, or "
               "--peer-addr= on the command line), case will be skipped\n");
    }

    g_hciReadCallbacks = { };
    g_hciReadCallbacks.leReadRemoteFeaturesComplete = OnLegacyReadRemoteFeaturesComplete;
    g_hciReadCallbacks.leReadAllRemoteFeaturesComplete = OnAllReadRemoteFeaturesComplete;
    ASSERT_EQ(HCI_RegisterEventCallbacks(&g_hciReadCallbacks), BT_SUCCESS);
    g_hciCallbacksRegistered = true;

    ASSERT_EQ(BTM_RegisterAclCallbacks(&g_aclCallbacks, &g_connCtx), BT_SUCCESS);
    g_aclCallbacksRegistered = true;
}

void StackGapLe60InteractTest::TearDownTestCase(void)
{
    DisconnectFromPeer();
    if (g_hciCallbacksRegistered) {
        EXPECT_EQ(HCI_DeregisterEventCallbacks(&g_hciReadCallbacks), BT_SUCCESS);
        g_hciCallbacksRegistered = false;
    }
    if (g_aclCallbacksRegistered) {
        EXPECT_EQ(BTM_DeregisterAclCallbacks(&g_aclCallbacks), BT_SUCCESS);
        g_aclCallbacksRegistered = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(TEARDOWN_DRAIN_MS));
    }
    if (g_btmEnabled) {
        EXPECT_EQ(BTM_Disable(LE_CONTROLLER), BT_SUCCESS);
        g_btmEnabled = false;
    }
    if (g_btmInitialized) {
        EXPECT_EQ(BTM_Close(), BT_SUCCESS);
        g_btmInitialized = false;
    }
}

/**
 * @tc.number: StackGapLe60Interact_ReadRemoteFeaturesOverAir_00100
 * @tc.name: LE Read (All) Remote Features auto-issued on connect follows local bit63 gate
 * @tc.desc: Over a real LE link the connect-complete path issues the read selected by
 *           LOCAL controller page0 bit63 (0x0016 -> 0x04, or 0x0088 -> 0x2B). The arriving
 *           completion proves the end-to-end wiring; assert handle/page0 content and that
 *           the non-selected HCI slot never fires.
 */
HWTEST_F(StackGapLe60InteractTest, StackGapLe60Interact_ReadRemoteFeaturesOverAir_00100, TestSize.Level1)
{
    if (!g_peerAddrValid) {
        GTEST_SKIP() << "peer address not set (BT60_PEER_ADDR=XX:XX:XX:XX:XX:XX [public:|random:] prefix)";
    }

    g_readFeaturesCapture.Reset();
    uint16_t connHandle = 0;
    if (!ConnectToPeer(connHandle)) {
        GTEST_SKIP() << "could not connect to peer (run './btfw_stack_unit_test server' on device B)";
    }

    const bool extendedFeatureSet = BTM_IsControllerSupportLeExtendedFeatureSet();
    printf("controller LE extended-feature-set (page0 bit63) = %s\n", extendedFeatureSet ? "set" : "clear");
    ASSERT_TRUE(g_readFeaturesCapture.WaitForAny(WAIT_CALLBACK_TIMEOUT_MS)) <<
        "no LE read remote features completion arrived after connect";

    if (!extendedFeatureSet) {
        // bit63 clear: only the legacy 0x0016 read was issued (subevent 0x04).
        ASSERT_TRUE(g_readFeaturesCapture.LegacyReceived()) <<
            "0x2B arrived although local bit63 selects the legacy 0x04 read";

        const auto legacyParam = g_readFeaturesCapture.LegacyParam();
        EXPECT_EQ(legacyParam.status, HCI_STATUS_SUCCESS);
        EXPECT_EQ(legacyParam.connectionHandle, connHandle);
        EXPECT_TRUE(Page0NonZero(legacyParam.leFeatures.raw));
        std::this_thread::sleep_for(std::chrono::milliseconds(NO_ALL_FEATURES_QUIET_MS));
        EXPECT_FALSE(g_readFeaturesCapture.AllFeaturesReceived()) <<
            "0x2B leReadAllRemoteFeaturesComplete fired on a bit63-clear controller";

        return;
    }

    // bit63 set: the 0x2B arrival itself proves the new path over the air. It is a failure
    // and not a skip when it is missing - the fallback would leave that branch unexercised.
    if (!g_readFeaturesCapture.AllFeaturesReceived()) {
        ADD_FAILURE() << "0x0088 was not sent: the 0x2B path never ran";
        return;
    }
    const auto allParam = g_readFeaturesCapture.AllParam();
    if (allParam.status != HCI_STATUS_SUCCESS) {
        GTEST_SKIP() <<
            "0x2B status=0x" <<
            std::hex <<
            static_cast<unsigned int>(allParam.status) <<
            ": peer likely pre-6.0 (LL read-all failed), skipping content asserts";
    }
    EXPECT_EQ(allParam.connectionHandle, connHandle);
    EXPECT_TRUE(Page0NonZero(allParam.leFeatures.raw));
}

/**
 * @tc.number: StackGapLe60Interact_DbafDecisionConnect_00200
 * @tc.name: Decision connect honors local decision instructions (bit 42)
 * @tc.desc: Peer "dbaf-server" advertises a connectable decision set carrying
 *           decision data "bt". Install an exact-match instruction (mask FF FF /
 *           target 62 74) and connect via LE Extended Create Connection by Decision
 *           (7.8.66, Initiator_Filter_Policy 0x02). Then prove mask semantics with a
 *           case-insensitive vector (DF DF / 42 54). Then install a never-match
 *           instruction: no connection completes inside the drain window, and swapping
 *           back to a match makes the same initiator connect. Every connectable phase
 *           ends with a real connect + disconnect; the decision-initiate cancel API
 *           (7.8.13) is exercised separately by case 00400.
 */
HWTEST_F(StackGapLe60InteractTest, StackGapLe60Interact_DbafDecisionConnect_00200, TestSize.Level1)
{
    DbafScanCtx scanCtx;
    DbafScanCallbackGuard scanGuard(&scanCtx);
    if (!DbafRegisterScanCallback(&scanCtx, scanGuard)) {
        GTEST_SKIP() << "could not register extended-scan callback";
    }

    DbafGate gate = DbafCheckCapability();
    if (gate != K_DBAF_READY) {
        GTEST_SKIP() << DbafGateSkipMessage(gate);
    }

    // Exact match: peer decision data "bt" == target 62 74 under mask FF FF.
    ASSERT_TRUE(DbafInstallArbitrary2(&scanCtx, 0xFF, 0xFF, 0x62, 0x74));
    std::this_thread::sleep_for(std::chrono::milliseconds(DBAF_PEER_SETTLE_MS));
    ASSERT_TRUE(DbafDecisionConnectSucceed(LE_INITIATOR_FILTER_POLICY_DECISION_PDU_ONLY)) <<
        "decision connect with exact-match instruction failed";

    DisconnectFromPeer();
    std::this_thread::sleep_for(std::chrono::milliseconds(DBAF_PEER_SETTLE_MS));

    // Masked compare: mask DF DF clears bit 5 of both octets, so the uppercase
    // target 42 54 ("BT") matches the lowercase peer data "bt".
    ASSERT_TRUE(DbafInstallArbitrary2(&scanCtx, 0xDF, 0xDF, 0x42, 0x54));
    std::this_thread::sleep_for(std::chrono::milliseconds(DBAF_PEER_SETTLE_MS));
    ASSERT_TRUE(DbafDecisionConnectSucceed(LE_INITIATOR_FILTER_POLICY_DECISION_PDU_ONLY)) <<
        "decision connect with case-insensitive instruction failed";

    DisconnectFromPeer();
    std::this_thread::sleep_for(std::chrono::milliseconds(DBAF_PEER_SETTLE_MS));

    // Never-match (mask FF FF / target 00 00): fire a decision connect and prove no
    // successful connection completes inside the drain window. A still-pending
    // initiator (drain 0) and an error-status completion (drain 1) both pass; only a
    // success (drain -1) fails the conformance point.
    ASSERT_TRUE(DbafInstallArbitrary2(&scanCtx, 0xFF, 0xFF, 0x00, 0x00));
    std::this_thread::sleep_for(std::chrono::milliseconds(DBAF_PEER_SETTLE_MS));
    g_connCtx.Reset();
    ASSERT_EQ(GAPIF_LeExtendedConnectByDecision(LE_INITIATOR_FILTER_POLICY_DECISION_PDU_ONLY), BT_SUCCESS) <<
        "never-match decision connect send failed";

    const int drain = DbafDrainNoConnectWindow(DBAF_NO_CONNECT_WINDOW_MS);
    EXPECT_NE(drain, -1) << "a connection completed although the decision instruction never matches";

    // Swap to a match. A pending initiator (drain 0) connects on the next accepted
    // decision PDU with no second command; an ended initiator (drain 1) needs a fresh
    // decision connect. Both paths must converge on a successful link.
    ASSERT_TRUE(DbafInstallArbitrary2(&scanCtx, 0xFF, 0xFF, 0x62, 0x74));
    if (drain == 0) {
        EXPECT_TRUE(g_connCtx.Wait()) << "pending initiator did not connect after the instruction swap";

        g_connected = (g_connCtx.status == HCI_STATUS_SUCCESS);
        ASSERT_TRUE(g_connected) << "pending initiator completed with an error status after the swap";
    } else {
        ASSERT_TRUE(DbafDecisionConnectSucceed(LE_INITIATOR_FILTER_POLICY_DECISION_PDU_ONLY)) <<
            "fresh decision connect after the swap did not complete";
    }
    DisconnectFromPeer();
}

/**
 * @tc.number: StackGapLe60Interact_DbafScanReport_00300
 * @tc.name: Extended scan report policy filters decision PDUs (bit 42)
 * @tc.desc: Against the same "dbaf-server" two sets, sweep the 0x0041
 *           Scanning_Filter_Policy decision bits 2-3: No-decisions reports only the
 *           plain set, All-PDUs reports both, Decisions-only reports only accepted
 *           decision PDUs, and a never-match instruction silences the decision set.
 *           Each mode change stops the scan first (a filter-policy change is only
 *           valid while scanning is disabled). The two sets share one address and the
 *           report carries no decision-PDU bit, so a report is counted as the decision
 *           set only when its data lacks the plain set's "BTPEER" marker.
 */
HWTEST_F(StackGapLe60InteractTest, StackGapLe60Interact_DbafScanReport_00300, TestSize.Level1)
{
    DbafScanCtx scanCtx;
    DbafScanCallbackGuard scanGuard(&scanCtx);
    if (!DbafRegisterScanCallback(&scanCtx, scanGuard)) {
        GTEST_SKIP() << "could not register extended-scan callback";
    }

    DbafGate gate = DbafCheckCapability();
    if (gate != K_DBAF_READY) {
        GTEST_SKIP() << DbafGateSkipMessage(gate);
    }

    // A matching instruction stays installed for the first three sweeps.
    ASSERT_TRUE(DbafInstallArbitrary2(&scanCtx, 0xFF, 0xFF, 0x62, 0x74));
    std::this_thread::sleep_for(std::chrono::milliseconds(DBAF_PEER_SETTLE_MS));

    DbafSweepResult sweep = { };
    // No-decisions: decision PDUs are ignored, the plain connectable set is reported.
    ASSERT_TRUE(DbafRunScanSweep(&scanCtx, LE_SCAN_DECISION_MODE_NONE, DBAF_SCAN_WINDOW_MS, sweep));
    EXPECT_GT(sweep.conn, 0) << "no plain-set report in no-decisions mode";
    EXPECT_EQ(sweep.decision, 0) << "decision PDU reported in no-decisions mode";

    // All-PDUs: both sets are reported.
    ASSERT_TRUE(DbafRunScanSweep(&scanCtx, LE_SCAN_DECISION_MODE_ALL_PDUS, DBAF_SCAN_WINDOW_MS, sweep));
    EXPECT_GT(sweep.conn, 0);
    EXPECT_GT(sweep.decision, 0) << "decision PDU not reported in all-PDUs mode";

    // Decisions-only: the accepted decision PDU is reported, the plain set is not.
    ASSERT_TRUE(DbafRunScanSweep(&scanCtx, LE_SCAN_DECISION_MODE_DECISIONS_ONLY, DBAF_SCAN_WINDOW_MS, sweep));
    EXPECT_EQ(sweep.conn, 0) << "plain PDU reported in decisions-only mode";
    EXPECT_GT(sweep.decision, 0) << "matching decision PDU not reported in decisions-only mode";

    // Decisions-only with a never-match instruction silences the decision set.
    ASSERT_TRUE(DbafInstallArbitrary2(&scanCtx, 0xFF, 0xFF, 0x00, 0x00));
    ASSERT_TRUE(DbafRunScanSweep(&scanCtx, LE_SCAN_DECISION_MODE_DECISIONS_ONLY, DBAF_SCAN_WINDOW_MS, sweep));
    EXPECT_EQ(sweep.conn, 0);
    EXPECT_EQ(sweep.decision, 0) << "never-match decision PDU reported in decisions-only mode";
}

/**
 * @tc.number: StackGapLe60Interact_DbafDecisionCancel_00400
 * @tc.name: Decision initiate cancel releases the single initiator slot (bit 42)
 * @tc.desc: Issue a decision connect under a never-match instruction and prove the
 *           initiator stays pending (no success inside the drain window). Then cancel
 *           it via GAPIF_LeExtendedConnectByDecisionCancel (7.8.13; LE Create
 *           Connection Cancel serves the Extended path too). The pending decision
 *           initiator is the only user of the Controller initiator slot, so a direct
 *           connect to the peer must now succeed - without the cancel it would be
 *           answered with Command Disallowed (0x0C). The cancel completion (status
 *           0x02) has no host-side callback for a decision-initiated connect, so only
 *           the send result and the follow-up direct connect are asserted. If the
 *           initiator self-ended with an error inside the window (drain == 1) the
 *           cancel degrades to a harmless no-op and the direct connect still proves
 *           the slot is free.
 */
HWTEST_F(StackGapLe60InteractTest, StackGapLe60Interact_DbafDecisionCancel_00400, TestSize.Level1)
{
    DbafScanCtx scanCtx;
    DbafScanCallbackGuard scanGuard(&scanCtx);
    if (!DbafRegisterScanCallback(&scanCtx, scanGuard)) {
        GTEST_SKIP() << "could not register extended-scan callback";
    }

    DbafGate gate = DbafCheckCapability();
    if (gate != K_DBAF_READY) {
        GTEST_SKIP() << DbafGateSkipMessage(gate);
    }

    // Never-match: the peer decision set can never pass, so the decision initiator
    // stays pending and occupies the single Controller initiator slot.
    ASSERT_TRUE(DbafInstallArbitrary2(&scanCtx, 0xFF, 0xFF, 0x00, 0x00));
    std::this_thread::sleep_for(std::chrono::milliseconds(DBAF_PEER_SETTLE_MS));
    g_connCtx.Reset();
    ASSERT_EQ(GAPIF_LeExtendedConnectByDecision(LE_INITIATOR_FILTER_POLICY_DECISION_PDU_ONLY), BT_SUCCESS) <<
        "never-match decision connect send failed";

    const int drain = DbafDrainNoConnectWindow(DBAF_NO_CONNECT_WINDOW_MS);
    EXPECT_NE(drain, -1) << "a connection completed although the decision instruction never matches";

    ASSERT_EQ(GAPIF_LeExtendedConnectByDecisionCancel(), BT_SUCCESS) << "decision connect cancel send failed";
    // Let the Controller finish stopping the initiator before the follow-up connect.
    std::this_thread::sleep_for(std::chrono::milliseconds(DBAF_PEER_SETTLE_MS));

    // A still-pending decision initiator would reject this direct 0x0043 with 0x0C.
    uint16_t connHandle = 0;
    ASSERT_TRUE(ConnectToPeer(connHandle)) << "direct connect still 0x0C-blocked after decision-initiate cancel";

    DisconnectFromPeer();
}

namespace {

// FSU request window for both over-air cases. min 100 us / max 10000 us (spec
// ceiling) never puts both bounds below a peer's in-use frame space and never
// exceeds the spec range, so a conformant FSU pair answers with a value inside
// [min, max] (Vol 4 Part E 7.8.151 p2876; Vol 6 Part B 5.1.30 p3209).
constexpr uint16_t FSU_REQ_FRAME_SPACE_MIN_US = 100;
constexpr uint16_t FSU_REQ_FRAME_SPACE_MAX_US = 10000;
// LE 1M: the default ACL PHY of the plain "server" peer role.
constexpr uint8_t FSU_REQ_PHY_LE_1M = 0x01;
// T_IFS_ACL_CP | T_IFS_ACL_PC (Spacing_Types bits 0-1).
constexpr uint16_t FSU_REQ_SPACING_ACL_IFS = 0x0003;
// A host-issued 0x009D always reports Initiator = Local Host (0x00).
constexpr uint8_t FSU_INITIATOR_LOCAL_HOST = 0x00;

// 0x35 completion content, filled by the conn-update group. Content fields are
// zeroed by GAP on a non-zero status and are read only on success.
struct FsuResult : CallbackWaiter {
    uint8_t status = 0xFF;
    BtAddr addr = { };
    bool addrValid = false;
    uint8_t initiator = 0xFF;
    uint16_t frameSpace = 0;
    uint8_t phys = 0;
    uint16_t spacingTypes = 0;

    void ResetAll()
    {
        Reset();
        status = 0xFF;
        addrValid = false;
        initiator = 0xFF;
        frameSpace = 0;
        phys = 0;
        spacingTypes = 0;
    }
};

// Read-PHY completion, the link-liveness probe after a tolerant outcome.
struct FsuPhyResult : CallbackWaiter {
    uint8_t status = 0xFF;
};

struct FsuE2eContext {
    FsuResult fsu;
    FsuPhyResult phy;
};

void OnFsuUpdateComplete(const GapLeFrameSpaceUpdateInfo *info, void *context)
{
    auto *ctx = static_cast<FsuE2eContext *>(context);
    if (info == nullptr) {
        ctx->fsu.Notify();
        return;
    }
    ctx->fsu.status = info->status;
    ctx->fsu.addr = info->addr;
    ctx->fsu.addrValid = true;
    ctx->fsu.initiator = info->initiator;
    ctx->fsu.frameSpace = info->frameSpace;
    ctx->fsu.phys = info->phys;
    ctx->fsu.spacingTypes = info->spacingTypes;
    ctx->fsu.Notify();
}

void OnFsuReadPhyResult(uint8_t status, const BtAddr *addr, uint8_t txPhy, uint8_t rxPhy, void *context)
{
    (void)addr;
    (void)txPhy;
    (void)rxPhy;
    auto *ctx = static_cast<FsuE2eContext *>(context);
    ctx->phy.status = status;
    ctx->phy.Notify();
}

// Registers the FSU and read-PHY members with the single conn-update slot and
// deregisters on scope exit. GAP copies the callback struct at registration, so
// the function-local instance only has to outlive the register call.
class FsuConnCallbackGuard {
public:
    explicit FsuConnCallbackGuard(FsuE2eContext *ctx)
    {
        GapLeConnCallback callback = { };
        callback.leFrameSpaceUpdateComplete = OnFsuUpdateComplete;
        callback.leReadPhyResult = OnFsuReadPhyResult;
        registered_ = GAPIF_RegisterLeConnCallback(&callback, ctx) == BT_SUCCESS;
    }

    ~FsuConnCallbackGuard()
    {
        if (registered_) {
            if (GAPIF_DeregisterLeConnCallback() != BT_SUCCESS) {
                printf("FsuConnCallbackGuard: deregister failed; callback may remain registered\n");
            }
            registered_ = false;
        }
    }

    FsuConnCallbackGuard(const FsuConnCallbackGuard &) = delete;
    FsuConnCallbackGuard &operator=(const FsuConnCallbackGuard &) = delete;

    bool Registered() const
    {
        return registered_;
    }

private:
    bool registered_ = false;
};

enum FsuDualGate {
    K_FSU_READY,
    K_FSU_NO_PEER_ADDR,
    K_FSU_LOCAL_NO_FSU,
    K_FSU_CONNECT_FAILED,
    K_FSU_READ_FAILED,
    K_FSU_PEER_PAGE1_UNKNOWN,
    K_FSU_PEER_NO_FSU,
};

const char *FsuGateSkipMessage(FsuDualGate gate)
{
    switch (gate) {
        case K_FSU_READY:
            return "";
        case K_FSU_NO_PEER_ADDR:
            return "peer address not set (BT60_PEER_ADDR)";
        case K_FSU_LOCAL_NO_FSU:
            return "local controller lacks LE feature bit 65 (Frame Space Update)";
        case K_FSU_CONNECT_FAILED:
            return "direct connect to the peer failed";
        case K_FSU_READ_FAILED:
            return "peer remote-features read failed";
        case K_FSU_PEER_PAGE1_UNKNOWN:
            return "peer page-1 features unreadable (local page-0 bit 63 clear)";
        case K_FSU_PEER_NO_FSU:
            return "peer controller lacks LE feature bit 65 (Frame Space Update)";
        default:
            return "";
    }
}

// Probe both boards for FSU support and report the outcome as a skip gate. No
// ASSERT is used here: the case maps the enum onto a visible GTEST_SKIP instead.
// The peer page-1 bit 65 is readable only when the connect-time read is the 0x2B
// all-features form, i.e. only when the local page-0 bit 63 made the controller
// issue it; a bit63-clear board (legacy 0x04 only) cannot reveal the peer bit.
FsuDualGate FsuProbeCapability()
{
    if (!g_peerAddrValid) {
        return K_FSU_NO_PEER_ADDR;
    }
    if (!BTM_IsControllerSupportLeFrameSpaceUpdate()) {
        return K_FSU_LOCAL_NO_FSU;
    }
    g_readFeaturesCapture.Reset();
    uint16_t connHandle = 0;
    if (!ConnectToPeer(connHandle)) {
        return K_FSU_CONNECT_FAILED;
    }
    if (!g_readFeaturesCapture.WaitForAny(WAIT_CALLBACK_TIMEOUT_MS)) {
        DisconnectFromPeer();
        return K_FSU_READ_FAILED;
    }
    if (!g_readFeaturesCapture.AllFeaturesReceived()) {
        DisconnectFromPeer();
        return K_FSU_PEER_PAGE1_UNKNOWN;
    }
    const auto param = g_readFeaturesCapture.AllParam();
    const uint8_t *features =
        (param.status == HCI_STATUS_SUCCESS) ? param.leFeatures.raw : nullptr;
    bool peerFsu = (features != nullptr) &&
        GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_FRAME_SPACE_UPDATE);
    DisconnectFromPeer();
    if (features == nullptr) {
        return K_FSU_READ_FAILED;
    }
    return peerFsu ? K_FSU_READY : K_FSU_PEER_NO_FSU;
}
}  // namespace

/**
 * @tc.number: StackGapLe60Interact_FsuUpdateOverAir_00500
 * @tc.name: Host-issued 0x009D completes over a live ACL on an FSU-capable pair
 * @tc.desc: Gate K_FSU_READY (local + peer bit 65 readable). Register the FSU and
 *           read-PHY callbacks, connect to the peer, then drive
 *           GAPIF_LeFrameSpaceUpdate (7.8.151) with min 100 us / max 10000 us on
 *           LE 1M for the ACL IFS spacing pair. 0x009D is a Command_Status
 *           command, so BT_SUCCESS means dispatched; the Controller answers with
 *           an LE Frame Space Update Complete event (0x35) and GAP fires
 *           GapLeConnCallback.leFrameSpaceUpdateComplete. Initiator is Local Host
 *           (0x00), the effective frame space lies in [min, max] (responder picks
 *           its lowest supportable value, 5.1.30), and phys / spacingTypes are
 *           non-zero.
 */
HWTEST_F(StackGapLe60InteractTest, StackGapLe60Interact_FsuUpdateOverAir_00500, TestSize.Level1)
{
    FsuDualGate gate = FsuProbeCapability();
    if (gate != K_FSU_READY) {
        GTEST_SKIP() << FsuGateSkipMessage(gate);
    }

    FsuE2eContext ctx;
    FsuConnCallbackGuard guard(&ctx);
    if (!guard.Registered()) {
        GTEST_SKIP() << "could not register the FSU conn-update callback";
    }

    uint16_t connHandle = 0;
    ASSERT_TRUE(ConnectToPeer(connHandle)) << "direct connect to the FSU peer failed";

    ctx.fsu.ResetAll();
    ASSERT_EQ(GAPIF_LeFrameSpaceUpdate(&g_peerAddr, FSU_REQ_FRAME_SPACE_MIN_US,
        FSU_REQ_FRAME_SPACE_MAX_US, FSU_REQ_PHY_LE_1M, FSU_REQ_SPACING_ACL_IFS), BT_SUCCESS) <<
            "0x009D dispatch failed";

    ASSERT_TRUE(ctx.fsu.Wait()) << "no 0x35 completion for the host-issued 0x009D";

    EXPECT_EQ(ctx.fsu.status, HCI_STATUS_SUCCESS) << "FSU completion reported a controller error";
    EXPECT_EQ(ctx.fsu.initiator, FSU_INITIATOR_LOCAL_HOST) << "host-issued request must report Initiator Local Host";

    ASSERT_TRUE(ctx.fsu.addrValid);
    EXPECT_EQ(memcmp(ctx.fsu.addr.addr, g_peerAddr.addr, BT_ADDRESS_SIZE), 0) << "completion reported a different link";

    EXPECT_GE(ctx.fsu.frameSpace, FSU_REQ_FRAME_SPACE_MIN_US) << "responder picked a frame space below the request min";

    EXPECT_LE(ctx.fsu.frameSpace, FSU_REQ_FRAME_SPACE_MAX_US) << "responder picked a frame space above the request max";

    EXPECT_NE(ctx.fsu.phys, 0) << "phys mask empty on the success completion";
    EXPECT_NE(ctx.fsu.spacingTypes, 0) << "spacing mask empty on the success completion";
    printf("FSU over-air: status=0x%02x initiator=0x%02x frameSpace=%u us phys=0x%02x spacing=0x%04x\n",
        ctx.fsu.status, ctx.fsu.initiator, ctx.fsu.frameSpace, ctx.fsu.phys, ctx.fsu.spacingTypes);
}

/**
 * @tc.number: StackGapLe60Interact_FsuPeerUnsupportedTolerant_00510
 * @tc.name: 0x009D against a peer without bit 65 ends tolerant and leaves the link up
 * @tc.desc: Gate K_FSU_PEER_NO_FSU (local bit 65, peer without it). Drive the same 0x009D
 *           over a live ACL. A peer without FSU either rejects the request (LL
 *           REJECT_EXT_IND, surfacing as an error-status 0x35 completion) or, where
 *           the LL answer is implementation-defined, produces no event; both are
 *           acceptable, and only a status==0 completion is a genuine failure. A
 *           follow-up read PHY on the same link proves the exchange did not break
 *           the ACL.
 */
HWTEST_F(StackGapLe60InteractTest, StackGapLe60Interact_FsuPeerUnsupportedTolerant_00510, TestSize.Level1)
{
    FsuDualGate gate = FsuProbeCapability();
    if (gate != K_FSU_PEER_NO_FSU) {
        GTEST_SKIP() << FsuGateSkipMessage(gate);
    }

    FsuE2eContext ctx;
    FsuConnCallbackGuard guard(&ctx);
    if (!guard.Registered()) {
        GTEST_SKIP() << "could not register the FSU conn-update callback";
    }

    uint16_t connHandle = 0;
    ASSERT_TRUE(ConnectToPeer(connHandle)) << "direct connect to the peer failed";

    ctx.fsu.ResetAll();
    ASSERT_EQ(GAPIF_LeFrameSpaceUpdate(&g_peerAddr, FSU_REQ_FRAME_SPACE_MIN_US,
        FSU_REQ_FRAME_SPACE_MAX_US, FSU_REQ_PHY_LE_1M, FSU_REQ_SPACING_ACL_IFS), BT_SUCCESS) <<
            "0x009D dispatch failed";

    // Both tolerated outcomes pass: an error-status completion (reject) or no
    // completion at all (implementation-defined unknown-command path). A status==0
    // completion from a known non-FSU peer is the only real failure.
    if (ctx.fsu.Wait()) {
        EXPECT_NE(ctx.fsu.status, HCI_STATUS_SUCCESS) <<
            "peer without bit 65 accepted the 0x009D with a success completion";

        printf("FSU peer-unsupported: completion status=0x%02x (reject path)\n", ctx.fsu.status);
    } else {
        printf("FSU peer-unsupported: no completion (implementation-defined unknown-command path)\n");
    }

    // Prove the exchange left the ACL usable: a read PHY on the same link succeeds.
    ctx.phy.Reset();
    ASSERT_EQ(GAPIF_LeReadPhy(&g_peerAddr), BT_SUCCESS) << "read-PHY dispatch failed";
    ASSERT_TRUE(ctx.phy.Wait()) << "no read-PHY completion after the FSU exchange";
    EXPECT_EQ(ctx.phy.status, HCI_STATUS_SUCCESS) << "link unusable after the FSU exchange";
}
}  // namespace Bluetooth
}  // namespace OHOS
