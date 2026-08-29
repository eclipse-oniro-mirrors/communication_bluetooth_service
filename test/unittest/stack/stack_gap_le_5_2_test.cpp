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

// BT 5.2 stack unit tests.
// Coverage targets:
//   1) HCI command layer : HCI_LeSetCigParameters / LeSetCigParametersTest / LeCreateCis / LeRemoveCig /
//   LeSetHostFeature 2) Public API layer  : ISOIF_LeRegisterCigCallback / LeCreateCig / LeCreateCis / LeRemoveCig 3)
//   EATT (Enhanced Credit Based Flow Control, plan chapter 3) L2CAP layer:
//      L2CAP_LeEattConnectionReq / ReconfigureReq / SetServiceConfig / SetServiceSecLevel /
//      SetSecurityInfo / RegisterService, plus the 0x17/0x18/0x19/0x1A two-device flows
// NOTE: This file does NOT provide main() — it shares the gtest executable
// with stack_gap_le_interact_test.cpp, whose main() already exists. The EATT
// companion setup (InitEattPeerSetup, external linkage) is called from that
// file's existing RunPeerMode ("server" argv), which serves all 5.2 two-device
// cases.
// EATT two-device setup:
//   device B (companion, responder): ./btfw_stack_unit_test server
//       registers the mock EATT service (PSM 0x0027), auto-accepts SMP Just Works
//       pairing (NoInputNoOutput), starts connectable advertising, then blocks.
//   device A (DUT, initiator): ./btfw_stack_unit_test <B-addr>
//       connects over LE ACL, pairs, and drives the EATT 0x17/0x18/0x19/0x1A flows.
//   When BT52_PEER_ADDR is not set, the two-device cases are skipped and only the
//   single-device EATT API validation cases run.

#include <gtest/gtest.h>

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "att.h"
#include "att/att_common.h"
#include "att/att_eatt.h"
#include "btm.h"
#include "btstack.h"
#include "buffer.h"
#include "gap_if.h"
#include "gap_le_if.h"
#include "iso_le_if.h"
#include "l2cap_def.h"
#include "l2cap_le_if.h"
#include "securec.h"
#include "src/hci/evt/hci_evt.h"
#include "src/hci/hci.h"
#include "src/hci/hci_def_le_cmd.h"
#include "src/iso/iso.h"
#include "src/l2cap/l2cap_cmn.h"
#include "src/l2cap/l2cap_le.h"
#include "stack_gap_le_5_2_test_internal.h"

using namespace testing::ext;
namespace OHOS {
namespace Bluetooth {

// CIG create result, captured from IsoLeCigCallback::createCigResult.
struct CigCreateResult : CallbackWaiter {
    uint8_t status = 0xFF;
    uint8_t cigId = 0;
    uint8_t cisCount = 0;
    uint16_t cisHandles[ISO_LE_CIS_COUNT_MAX] = { 0 };
};

void OnCigCreateResult(uint8_t status, uint8_t cigId, uint8_t cisCount, const uint16_t *cisHandles, void *context)
{
    auto *result = static_cast<CigCreateResult *>(context);
    result->status = status;
    result->cigId = cigId;
    result->cisCount = cisCount;
    if (cisHandles != nullptr && cisCount > 0) {
        uint8_t copyLen = (cisCount < ISO_LE_CIS_COUNT_MAX) ? cisCount : ISO_LE_CIS_COUNT_MAX;
        for (uint8_t i = 0; i < copyLen; i++) {
            result->cisHandles[i] = cisHandles[i];
        }
    }
    result->Notify();
}

// CIS established / reject result, captured from IsoLeCigCallback::cisEstablished / rejectCisResult.
struct CisStatusResult : CallbackWaiter {
    uint8_t status = 0xFF;
};

void OnCisEstablished(uint8_t status, const IsoLeCisEstablishedInfo *info, void *context)
{
    (void)info;
    auto *result = static_cast<CisStatusResult *>(context);
    result->status = status;
    result->Notify();
}

void OnRejectCisResult(uint8_t status, void *context)
{
    auto *result = static_cast<CisStatusResult *>(context);
    result->status = status;
    result->Notify();
}

// CIS disconnected result, captured from IsoLeCigCallback::cisDisconnected.
// status == HCI_STATUS_SUCCESS means the termination completed; on failure the
// reason shall be ignored (BT 5.2 Vol 4 Part E 7.7.5).
struct CisDisconnectedResult : CallbackWaiter {
    uint8_t status = 0xFF;
    uint16_t cisHandle = 0;
    uint8_t reason = 0xFF;
};

void OnCisDisconnected(uint8_t status, uint16_t cisHandle, uint8_t reason, void *context)
{
    auto *result = static_cast<CisDisconnectedResult *>(context);
    result->status = status;
    result->cisHandle = cisHandle;
    result->reason = reason;
    result->Notify();
}

// Combined two-device CIS context: ISOIF_LeRegisterCigCallback carries a single
// void* context, so one registration needs one container plus forwarding callbacks
// that dispatch to the per-result waiters.
struct IsoCisTdContext {
    CigCreateResult cig;
    CisStatusResult est;
    CisDisconnectedResult disc;
};

void OnCisTdCigCreateResult(uint8_t status, uint8_t cigId, uint8_t cisCount, const uint16_t *cisHandles, void *context)
{
    auto *td = static_cast<IsoCisTdContext *>(context);
    td->cig.status = status;
    td->cig.cigId = cigId;
    td->cig.cisCount = cisCount;
    if (cisHandles != nullptr && cisCount > 0) {
        uint8_t copyLen = (cisCount < ISO_LE_CIS_COUNT_MAX) ? cisCount : ISO_LE_CIS_COUNT_MAX;
        for (uint8_t i = 0; i < copyLen; i++) {
            td->cig.cisHandles[i] = cisHandles[i];
        }
    }
    td->cig.Notify();
}

void OnCisTdEstablished(uint8_t status, const IsoLeCisEstablishedInfo *info, void *context)
{
    (void)info;
    auto *td = static_cast<IsoCisTdContext *>(context);
    td->est.status = status;
    td->est.Notify();
}

void OnCisTdDisconnected(uint8_t status, uint16_t cisHandle, uint8_t reason, void *context)
{
    auto *td = static_cast<IsoCisTdContext *>(context);
    td->disc.status = status;
    td->disc.cisHandle = cisHandle;
    td->disc.reason = reason;
    td->disc.Notify();
}

// RAII guard for the CIG callback registration. ISOIF_LeRegisterCigCallback keeps the callback
// pointer, not a copy (IsoRegisterCigCallback), and the context is typically a test-local struct
// on the stack: an early test exit would leave both dangling. The destructor deregisters
// unconditionally (mirrors EattAttServiceGuard in stack_att_52_test.cpp); deregistration is
// idempotent, and TearDownTestCase re-deregisters as a safety net.
class CigCallbackGuard {
public:
    CigCallbackGuard() = default;
    ~CigCallbackGuard()
    {
        (void)ISOIF_LeDeregisterCigCallback();
    }
};

// RAII guard for the ISO status-query callback registration. Like CigCallbackGuard:
// ISOIF_LeRegisterStatusQueryCallback keeps the callback pointer, not a copy, and the
// context is typically a test-local IsoStatusQueryResult on the stack — an early test
// exit (ASSERT failure or GTEST_SKIP) would leave both dangling and the next complete
// event would write into destroyed stack memory. The destructor deregisters
// unconditionally; deregistration is idempotent. Declare it last among the locals so
// it is destroyed before the callback/result objects it protects.
class StatusQueryCallbackGuard {
public:
    StatusQueryCallbackGuard() = default;
    ~StatusQueryCallbackGuard()
    {
        (void)ISOIF_LeDeregisterStatusQueryCallback();
    }
};

// BIG/BIS management result, captured from IsoLeBigCallback callbacks.
struct BigStatusResult : CallbackWaiter {
    uint8_t status = 0xFF;
    uint8_t bigHandle = 0;
};

void OnBigCreateResult(uint8_t status, const IsoLeBigCreatedInfo *info, void *context)
{
    auto *result = static_cast<BigStatusResult *>(context);
    result->status = status;
    result->bigHandle = (info != nullptr) ? info->bigHandle : 0;
    result->Notify();
}

void OnBigTerminateResult(uint8_t status, void *context)
{
    auto *result = static_cast<BigStatusResult *>(context);
    result->status = status;
    result->Notify();
}

void OnBigSyncEstablished(uint8_t status, const IsoLeBigSyncEstablishedInfo *info, void *context)
{
    auto *result = static_cast<BigStatusResult *>(context);
    result->status = status;
    result->bigHandle = (info != nullptr) ? info->bigHandle : 0;
    result->Notify();
}

void OnBigSyncLost(uint8_t bigHandle, uint8_t reason, void *context)
{
    auto *result = static_cast<BigStatusResult *>(context);
    result->bigHandle = bigHandle;
    result->status = reason;
    result->Notify();
}

void OnBigInfoReport(const IsoLeBigInfoReportInfo *info, void *context)
{
    auto *result = static_cast<BigStatusResult *>(context);
    result->status = (info != nullptr) ? info->numBis : 0xFF;
    result->Notify();
}

void OnBigTerminateSyncResult(uint8_t status, void *context)
{
    auto *result = static_cast<BigStatusResult *>(context);
    result->status = status;
    result->Notify();
}

// ISO data path result, captured from IsoLeDataPathCallback callbacks.
struct IsoDataPathStatusResult : CallbackWaiter {
    uint8_t status = 0xFF;
    uint16_t connectionHandle = 0;
};

void OnSetupIsoDataPathResult(uint8_t status, uint16_t connectionHandle, void *context)
{
    auto *result = static_cast<IsoDataPathStatusResult *>(context);
    result->status = status;
    result->connectionHandle = connectionHandle;
    result->Notify();
}

void OnRemoveIsoDataPathResult(uint8_t status, uint16_t connectionHandle, void *context)
{
    auto *result = static_cast<IsoDataPathStatusResult *>(context);
    result->status = status;
    result->connectionHandle = connectionHandle;
    result->Notify();
}

// ISO test result, captured from IsoLeTestCallback callbacks.
struct IsoTestStatusResult : CallbackWaiter {
    uint8_t status = 0xFF;
    uint16_t connectionHandle = 0;
    uint32_t receivedPacketCount = 0;
    uint32_t missedPacketCount = 0;
    uint32_t failedPacketCount = 0;
};

void OnIsoTransmitTestResult(uint8_t status, uint16_t connectionHandle, void *context)
{
    auto *result = static_cast<IsoTestStatusResult *>(context);
    result->status = status;
    result->connectionHandle = connectionHandle;
    result->Notify();
}

void OnIsoReceiveTestResult(uint8_t status, uint16_t connectionHandle, void *context)
{
    auto *result = static_cast<IsoTestStatusResult *>(context);
    result->status = status;
    result->connectionHandle = connectionHandle;
    result->Notify();
}

void OnIsoReadTestCountersResult(uint8_t status, const IsoLeTestCountersInfo *info, void *context)
{
    auto *result = static_cast<IsoTestStatusResult *>(context);
    result->status = status;
    result->connectionHandle = (info != nullptr) ? info->connectionHandle : 0;
    result->receivedPacketCount = (info != nullptr) ? info->receivedPacketCount : 0;
    result->missedPacketCount = (info != nullptr) ? info->missedPacketCount : 0;
    result->failedPacketCount = (info != nullptr) ? info->failedPacketCount : 0;
    result->Notify();
}

void OnIsoTestEndResult(uint8_t status, const IsoLeTestCountersInfo *info, void *context)
{
    auto *result = static_cast<IsoTestStatusResult *>(context);
    result->status = status;
    result->connectionHandle = (info != nullptr) ? info->connectionHandle : 0;
    result->receivedPacketCount = (info != nullptr) ? info->receivedPacketCount : 0;
    result->missedPacketCount = (info != nullptr) ? info->missedPacketCount : 0;
    result->failedPacketCount = (info != nullptr) ? info->failedPacketCount : 0;
    result->Notify();
}

// ISO status query result, captured from IsoLeStatusQueryCallback callbacks.
struct IsoStatusQueryResult : CallbackWaiter {
    uint8_t status = 0xFF;
    uint16_t connectionHandle = 0;
    uint32_t txUnackedPackets = 0;
    uint32_t txFlushedPackets = 0;
    uint32_t txLastSubeventPackets = 0;
    uint32_t retransmittedPackets = 0;
    uint32_t crcErrorPackets = 0;
    uint32_t rxUnreceivedPackets = 0;
    uint32_t duplicatePackets = 0;
    uint16_t packetSequenceNumber = 0;
    uint32_t timeStamp = 0;
    uint32_t timeOffset = 0;
    uint8_t peerClockAccuracy = 0;
};

void OnReadIsoLinkQualityResult(uint8_t status, const IsoLeLinkQualityInfo *info, void *context)
{
    auto *result = static_cast<IsoStatusQueryResult *>(context);
    result->status = status;
    result->connectionHandle = (info != nullptr) ? info->connectionHandle : 0;
    result->txUnackedPackets = (info != nullptr) ? info->txUnackedPackets : 0;
    result->txFlushedPackets = (info != nullptr) ? info->txFlushedPackets : 0;
    result->txLastSubeventPackets = (info != nullptr) ? info->txLastSubeventPackets : 0;
    result->retransmittedPackets = (info != nullptr) ? info->retransmittedPackets : 0;
    result->crcErrorPackets = (info != nullptr) ? info->crcErrorPackets : 0;
    result->rxUnreceivedPackets = (info != nullptr) ? info->rxUnreceivedPackets : 0;
    result->duplicatePackets = (info != nullptr) ? info->duplicatePackets : 0;
    result->Notify();
}

void OnReadIsoTxSyncResult(uint8_t status, const IsoLeTxSyncInfo *info, void *context)
{
    auto *result = static_cast<IsoStatusQueryResult *>(context);
    result->status = status;
    result->connectionHandle = (info != nullptr) ? info->connectionHandle : 0;
    result->packetSequenceNumber = (info != nullptr) ? info->packetSequenceNumber : 0;
    result->timeStamp = (info != nullptr) ? info->timeStamp : 0;
    result->timeOffset = (info != nullptr) ? info->timeOffset : 0;
    result->Notify();
}

void OnRequestPeerScaResult(uint8_t status, const IsoLePeerScaInfo *info, void *context)
{
    auto *result = static_cast<IsoStatusQueryResult *>(context);
    result->status = status;
    result->connectionHandle = (info != nullptr) ? info->connectionHandle : 0;
    result->peerClockAccuracy = (info != nullptr) ? info->peerClockAccuracy : 0;
    result->Notify();
}

// BT 5.2 LE Power Control (0x0076-0x007A) results.
// 0x0076 enhanced read transmit power result, captured from GapLePowerControlCallback::enhancedReadTransmitPowerResult.
struct EnhancedReadTxPowerResult : CallbackWaiter {
    uint8_t status = 0xFF;
    uint16_t connectionHandle = 0;
    uint8_t phy = 0;
    int8_t currentTransmitPowerLevel = 0;
    int8_t maxTransmitPowerLevel = 0;
};

void OnEnhancedReadTransmitPowerResult(uint8_t status, const GapLeEnhancedReadTxPowerInfo *info, void *context)
{
    if (context == nullptr) {
        return;
    }
    auto *result = static_cast<EnhancedReadTxPowerResult *>(context);
    result->status = status;
    result->connectionHandle = (info != nullptr) ? info->connectionHandle : 0;
    result->phy = (info != nullptr) ? info->phy : 0;
    result->currentTransmitPowerLevel = (info != nullptr) ? info->currentTransmitPowerLevel : 0;
    result->maxTransmitPowerLevel = (info != nullptr) ? info->maxTransmitPowerLevel : 0;
    result->Notify();
}

// 0x21 LE Transmit Power Reporting event, captured from GapLePowerControlCallback::transmitPowerReporting.
// 0x0077 completes via this event with Reason 0x02.
struct TransmitPowerReportingResult : CallbackWaiter {
    uint8_t status = 0xFF;
    uint16_t connectionHandle = 0;
    uint8_t reason = 0xFF;
    uint8_t phy = 0;
    int8_t transmitPowerLevel = 0;
    uint8_t transmitPowerLevelFlag = 0;
    int8_t delta = 0;
};

void OnTransmitPowerReporting(const GapLeTransmitPowerReportingInfo *info, void *context)
{
    if (context == nullptr) {
        return;
    }
    auto *result = static_cast<TransmitPowerReportingResult *>(context);
    result->status = (info != nullptr) ? info->status : 0xFF;
    result->connectionHandle = (info != nullptr) ? info->connectionHandle : 0;
    result->reason = (info != nullptr) ? info->reason : 0xFF;
    result->phy = (info != nullptr) ? info->phy : 0;
    result->transmitPowerLevel = (info != nullptr) ? info->transmitPowerLevel : 0;
    result->transmitPowerLevelFlag = (info != nullptr) ? info->transmitPowerLevelFlag : 0;
    result->delta = (info != nullptr) ? info->delta : 0;
    result->Notify();
}

// 0x0078 command complete, captured from GapLePowerControlCallback::setPathLossReportingParamsResult.
struct PathLossSetParamsResult : CallbackWaiter {
    uint8_t status = 0xFF;
    uint16_t connectionHandle = 0;
};

void OnSetPathLossReportingParamsResult(uint8_t status, const GapLeStatusHandleInfo *info, void *context)
{
    if (context == nullptr) {
        return;
    }
    auto *result = static_cast<PathLossSetParamsResult *>(context);
    result->status = status;
    result->connectionHandle = (info != nullptr) ? info->connectionHandle : 0;
    result->Notify();
}

// 0x0079 command complete, captured from GapLePowerControlCallback::setPathLossReportingEnableResult.
struct PathLossSetEnableResult : CallbackWaiter {
    uint8_t status = 0xFF;
    uint16_t connectionHandle = 0;
};

void OnSetPathLossReportingEnableResult(uint8_t status, const GapLeStatusHandleInfo *info, void *context)
{
    if (context == nullptr) {
        return;
    }
    auto *result = static_cast<PathLossSetEnableResult *>(context);
    result->status = status;
    result->connectionHandle = (info != nullptr) ? info->connectionHandle : 0;
    result->Notify();
}

// 0x007A command complete, captured from GapLePowerControlCallback::setTransmitPowerReportingEnableResult.
struct TransmitPowerSetEnableResult : CallbackWaiter {
    uint8_t status = 0xFF;
    uint16_t connectionHandle = 0;
};

void OnSetTransmitPowerReportingEnableResult(uint8_t status, const GapLeStatusHandleInfo *info, void *context)
{
    if (context == nullptr) {
        return;
    }
    auto *result = static_cast<TransmitPowerSetEnableResult *>(context);
    result->status = status;
    result->connectionHandle = (info != nullptr) ? info->connectionHandle : 0;
    result->Notify();
}

// 0x20 LE Path Loss Threshold event, captured from GapLePowerControlCallback::pathLossThreshold.
struct PathLossThresholdResult : CallbackWaiter {
    uint16_t connectionHandle = 0;
    uint8_t currentPathLoss = 0;
    uint8_t zoneEntered = 0xFF;
};

void OnPathLossThreshold(const GapLePathLossThresholdInfo *info, void *context)
{
    if (context == nullptr) {
        return;
    }
    auto *result = static_cast<PathLossThresholdResult *>(context);
    result->connectionHandle = (info != nullptr) ? info->connectionHandle : 0;
    result->currentPathLoss = (info != nullptr) ? info->currentPathLoss : 0;
    result->zoneEntered = (info != nullptr) ? info->zoneEntered : 0xFF;
    result->Notify();
}

// ===================== two-device helpers (peer via BT52_PEER_ADDR) =====================
// Follows the 5.1 test pattern: the peer runs the interact test's server mode
// (btfw_stack_unit_test server, connectable advertising), this side connects over
// LE ACL and reuses the real connection handle for the ISO status-query commands
// that need a live link (0x006D). No shared test file is modified.

void OnLeConnectionComplete(uint8_t status, uint16_t connectionHandle, const BtAddr *addr, uint8_t role, void *context)
{
    LeConnContext *ctx = static_cast<LeConnContext *>(context);
    ctx->status = status;
    ctx->handle = connectionHandle;
    ctx->role = role;
    if (addr != nullptr) {
        ctx->peerAddr = *addr;
    }
    ctx->Notify();
}

// Used by ResetConnection() to tear the link down between EATT tests, so each
// two-device case starts from an unencrypted link (deterministic 0x0008 reject).
void OnLeDisconnectionComplete(uint8_t status, uint16_t connectionHandle, uint8_t reason, void *context)
{
    LeConnContext *ctx = static_cast<LeConnContext *>(context);
    ctx->status = status;
    ctx->Notify();
}

// BTM_RegisterAclCallbacks stores the struct pointer (not a copy), so it must be
// static, otherwise BTM holds a dangling pointer after this function returns.
BtmAclCallbacks g_aclCallbacks = {
    .leConnectionComplete = OnLeConnectionComplete,
    .leDisconnectionComplete = OnLeDisconnectionComplete,
};

// Peer address for two-device tests, read from env BT52_PEER_ADDR: "XX:XX:XX:XX:XX:XX"
// (17 chars) + NUL. Fixed-size buffer: a std::string global would violate the
// no-non-POD-global red line (rule 1).
char g_peerAddrArg[18] = { 0 };

// Fill g_peerAddrArg from the BT52_PEER_ADDR env var. An over-long value is not a
// valid address and leaves the buffer empty (ParsePeerAddr rejects it either way).
// Called from both SetUpTestCase bodies before ParsePeerAddr; safe to repeat.
void InitPeerAddrArg()
{
    const char *env = std::getenv("BT52_PEER_ADDR");
    if (env == nullptr) {
        return;
    }
    size_t len = strlen(env);
    if (len >= sizeof(g_peerAddrArg)) {
        return;
    }
    errno_t ret = memcpy_s(g_peerAddrArg, sizeof(g_peerAddrArg), env, len + 1);
    if (ret != EOK) {
        // Defensive: on copy failure leave the buffer empty (same as an over-long
        // env value above), ParsePeerAddr rejects an empty address either way.
        g_peerAddrArg[0] = '\0';
    }
}
BtAddr g_peerAddr = { };
bool g_peerAddrValid = false;
LeConnContext g_connCtx;
bool g_connected = false;

// "XX:XX:XX:XX:XX:XX" (17 chars incl. the separators, 3 chars per byte pair)
constexpr size_t BT_ADDR_STRING_LEN = 17;
constexpr size_t BT_ADDR_PAIR_STRIDE = 3;

// BtAddr.addr[] is little-endian (addr[5] printed first). Parse the conventional
// "11:22:33:44:55:66" form, leftmost byte goes to addr[5].
bool ParsePeerAddr(const char *str, BtAddr *addr)
{
    unsigned int b[BT_ADDRESS_SIZE] = { 0 };
    if (str == nullptr || strlen(str) != BT_ADDR_STRING_LEN) {
        return false;
    }
    // sscanf_s is the bounds-checking variant required by the coding red line; the %x conversions
    // here take no buffer-size argument (only %c/%s/%[ do)
    for (int i = 0; i < BT_ADDRESS_SIZE; i++) {
        if (sscanf_s(str + (i * BT_ADDR_PAIR_STRIDE), "%02x", &b[i]) != 1) {
            return false;
        }
    }
    // reverse into the little-endian address: the leftmost byte is stored last
    int index = BT_ADDRESS_SIZE;
    for (int i = 0; i < BT_ADDRESS_SIZE; i++) {
        index--;
        addr->addr[index] = static_cast<uint8_t>(b[i]);
    }
    addr->type = BT_PUBLIC_DEVICE_ADDRESS;
    return true;
}

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


class StackGapLe52Test : public testing::Test {
public:
    StackGapLe52Test() { }
    ~StackGapLe52Test() { }

    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp() { }
    // Tear the link down after each test so a test that leaves it up (e.g. the SCA
    // two-device case) cannot break the next test's "no connection" / "unencrypted"
    // assumptions. A no-op for the single-board cases that never connect.
    void TearDown()
    {
        ResetConnection();
    }
};

void StackGapLe52Test::SetUpTestCase(void)
{
    ASSERT_EQ(BTM_Initialize(), BT_SUCCESS);
    ASSERT_EQ(BTM_Enable(LE_CONTROLLER), BT_SUCCESS);
    ASSERT_TRUE(BTM_IsEnabled(LE_CONTROLLER));
    ASSERT_EQ(GAPIF_LeSetRole(GAP_LE_ROLE_CENTRAL), BT_SUCCESS);

    // Two-device tests need a real peer: parse BT52_PEER_ADDR and register the
    // ACL connection-complete callback. BTM stores the callback struct pointer,
    // so g_aclCallbacks must be static.
    InitPeerAddrArg();
    g_peerAddrValid = ParsePeerAddr(g_peerAddrArg, &g_peerAddr);
    if (!g_peerAddrValid) {
        printf("peer address not set (BT52_PEER_ADDR=XX:XX:XX:XX:XX:XX), two-device tests will be skipped\n");
    }
    ASSERT_EQ(BTM_RegisterAclCallbacks(&g_aclCallbacks, &g_connCtx), BT_SUCCESS);
    // SMP pairing responder thread for the EATT two-device tests (auto-answer).
    InitPairResponder();
}

void StackGapLe52Test::TearDownTestCase(void)
{
    // Tear the current link down so the EATT reject-before-pairing case starts
    // from an unencrypted state in the next run.
    ResetConnection();
    (void)BTM_DeregisterAclCallbacks(&g_aclCallbacks);
    (void)ISOIF_LeDeregisterCigCallback();
    ShutdownPairResponder();
    (void)BTM_Disable(LE_CONTROLLER);
    (void)BTM_Close();
}

// BT 5.2 spec, Vol 2, Part E, §7.8.97
//   LE Set CIG Parameters Command

/**
 * @tc.number: StackGapLe52_HciSetCigParameters_00100
 * @tc.name: HCI_LeSetCigParameters NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciSetCigParameters_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeSetCigParameters(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciSetCigParameters_00200
 * @tc.name: HCI_LeSetCigParameters valid param: single CIS
 * @tc.desc: cisCount=1 with valid CIS_Config -> command accepted, returns non BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciSetCigParameters_00200, TestSize.Level1)
{
    HciLeCisConfigParam cisConfig = { };
    cisConfig.cisId = 0x00;
    cisConfig.maxSduMToS = 0x0100;
    cisConfig.maxSduSToM = 0x0100;
    cisConfig.phyMToS = 0x02;
    cisConfig.phySToM = 0x02;
    cisConfig.rtnMToS = 0x02;
    cisConfig.rtnSToM = 0x02;

    HciLeSetCigParametersParam param = { };
    param.cigId = 0x00;
    param.sduIntervalMToS[0] = 0x10;
    param.sduIntervalSToM[0] = 0x10;
    param.slaveClockAccuracy = 0x00;
    param.packing = 0x01;
    param.framing = 0x00;
    param.cisCount = 1;
    param.cisConfig = &cisConfig;

    int ret = HCI_LeSetCigParameters(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);

    // Clean up the controller-side CIG 0x00 right away: later cases (e.g. the two-device
    // CIS tests) create CIG 0x00 again, and relying on HciRemoveCig_00200 running first
    // breaks under --gtest_filter or reordering. HCI commands on the same controller are
    // executed in send order, so this remove always lands before any later create.
    HciLeRemoveCigParam removeParam = { };
    removeParam.cigId = 0x00;
    (void)HCI_LeRemoveCig(&removeParam);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.98
//   LE Set CIG Parameters Test Command

/**
 * @tc.number: StackGapLe52_HciSetCigParametersTest_00100
 * @tc.name: HCI_LeSetCigParametersTest NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciSetCigParametersTest_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeSetCigParametersTest(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciSetCigParametersTest_00200
 * @tc.name: HCI_LeSetCigParametersTest valid param: single CIS
 * @tc.desc: cisCount=1 with valid CIS_Config -> command accepted, returns non BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciSetCigParametersTest_00200, TestSize.Level1)
{
    HciLeSetCigParametersTestCisConfig cisConfig = { };
    cisConfig.cisId = 0x00;
    cisConfig.nse = 0x02;
    cisConfig.maxSduMToS = 0x0100;
    cisConfig.maxSduSToM = 0x0100;
    cisConfig.maxPduMToS = 0x0010;
    cisConfig.maxPduSToM = 0x0010;
    cisConfig.phyMToS = 0x02;
    cisConfig.phySToM = 0x02;
    cisConfig.bnMToS = 0x01;
    cisConfig.bnSToM = 0x01;

    HciLeSetCigParametersTestParam param = { };
    param.cigId = 0x00;
    param.sduIntervalMToS[0] = 0x10;
    param.sduIntervalSToM[0] = 0x10;
    param.ftMToS = 0x02;
    param.ftSToM = 0x02;
    param.isoInterval[0] = 0x10;
    param.slaveClockAccuracy = 0x00;
    param.packing = 0x01;
    param.framing = 0x00;
    param.cisCount = 1;
    param.cisConfig = &cisConfig;

    int ret = HCI_LeSetCigParametersTest(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.99
//   LE Create CIS Command

/**
 * @tc.number: StackGapLe52_HciCreateCis_00100
 * @tc.name: HCI_LeCreateCis NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciCreateCis_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeCreateCis(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciCreateCis_00200
 * @tc.name: HCI_LeCreateCis valid param: single CIS
 * @tc.desc: cisCount=1 with valid CIS_Config -> command accepted, returns non BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciCreateCis_00200, TestSize.Level1)
{
    HciLeCreateCisConfigParam cisConfig = { };
    cisConfig.cisHandle = 0x0000;
    cisConfig.aclHandle = 0x0040;

    HciLeCreateCisParam param = { };
    param.cisCount = 1;
    param.cisConfig = &cisConfig;

    int ret = HCI_LeCreateCis(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.100
//   CIG_ID (1 byte): CIG Identifier

/**
 * @tc.number: StackGapLe52_HciRemoveCig_00100
 * @tc.name: HCI_LeRemoveCig NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciRemoveCig_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeRemoveCig(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciRemoveCig_00200
 * @tc.name: HCI_LeRemoveCig valid CIG ID
 * @tc.desc: cigId=0x00 -> command accepted, returns non BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciRemoveCig_00200, TestSize.Level1)
{
    HciLeRemoveCigParam param = { };
    param.cigId = 0x00;

    int ret = HCI_LeRemoveCig(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.101
//   LE Accept CIS Request Command

/**
 * @tc.number: StackGapLe52_HciAcceptCisRequest_00100
 * @tc.name: HCI_LeAcceptCisRequest NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciAcceptCisRequest_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeAcceptCisRequest(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciAcceptCisRequest_00200
 * @tc.name: HCI_LeAcceptCisRequest valid param
 * @tc.desc: cisHandle=0x0000 -> command accepted, returns non BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciAcceptCisRequest_00200, TestSize.Level1)
{
    HciLeAcceptCisRequestParam param = { };
    param.cisHandle = 0x0000;

    int ret = HCI_LeAcceptCisRequest(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.102
//   LE Reject CIS Request Command

/**
 * @tc.number: StackGapLe52_HciRejectCisRequest_00100
 * @tc.name: HCI_LeRejectCisRequest NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciRejectCisRequest_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeRejectCisRequest(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciRejectCisRequest_00200
 * @tc.name: HCI_LeRejectCisRequest valid param
 * @tc.desc: cisHandle=0x0000, reason=0x3A -> command accepted, returns non BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciRejectCisRequest_00200, TestSize.Level1)
{
    HciLeRejectCisRequestParam param = { };
    param.cisHandle = 0x0000;
    param.reason = 0x3A; // HCI_ERR_ACCEPT_TIMEOUT

    int ret = HCI_LeRejectCisRequest(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_HciSetHostFeature_00100
 * @tc.name: HCI_LeSetHostFeature NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciSetHostFeature_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeSetHostFeature(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciSetHostFeature_00200
 * @tc.name: HCI_LeSetHostFeature valid param: ISO host support enabled
 * @tc.desc: bitNumber=0x20, bitValue=1 -> command accepted, returns non BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciSetHostFeature_00200, TestSize.Level1)
{
    HciLeSetHostFeatureParam param = { };
    param.bitNumber = HOST_FEATURE_BIT_ISO_HOST_SUPPORT;
    param.bitValue = HOST_FEATURE_BIT_VALUE_SUPPORT;

    int ret = HCI_LeSetHostFeature(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_HciSetHostFeature_00300
 * @tc.name: HCI_LeSetHostFeature valid param: ISO host support disabled
 * @tc.desc: bitNumber=0x20, bitValue=0 -> command accepted, returns non BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciSetHostFeature_00300, TestSize.Level1)
{
    HciLeSetHostFeatureParam param = { };
    param.bitNumber = HOST_FEATURE_BIT_ISO_HOST_SUPPORT;
    param.bitValue = HOST_FEATURE_BIT_VALUE_NOT_SUPPORT;

    int ret = HCI_LeSetHostFeature(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
}

// ===================== Layer 5: ISOIF public API layer =====================

/**
 * @tc.number: StackGapLe52_IsoifRegisterCigCallback_00100
 * @tc.name: ISOIF_LeRegisterCigCallback/Deregister
 * @tc.desc: register/deregister return BT_SUCCESS, and deregister is idempotent (repeat call)
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifRegisterCigCallback_00100, TestSize.Level1)
{
    IsoLeCigCallback cb = { };
    cb.createCigResult = OnCigCreateResult;
    EXPECT_EQ(ISOIF_LeRegisterCigCallback(&cb, nullptr), BT_SUCCESS);
    EXPECT_EQ(ISOIF_LeDeregisterCigCallback(), BT_SUCCESS);
    EXPECT_EQ(ISOIF_LeDeregisterCigCallback(), BT_SUCCESS);
}

/**
 * @tc.number: StackGapLe52_IsoifCreateCig_00100
 * @tc.name: ISOIF_LeCreateCig invalid params
 * @tc.desc: NULL cigParam, cisCount out of range, or SDU interval overflow -> BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifCreateCig_00100, TestSize.Level1)
{
    EXPECT_EQ(ISOIF_LeCreateCig(0x00, nullptr, 0, nullptr), BT_BAD_PARAM);

    IsoLeCigParam cigParam = { };
    EXPECT_EQ(ISOIF_LeCreateCig(0x00, &cigParam, ISO_LE_CIS_COUNT_MAX + 1, nullptr), BT_BAD_PARAM);

    IsoLeCisParam cisParam = { };
    cigParam.sduIntervalMToS = 0x1000000; // > 0xFFFFFF, exceeds 24-bit range
    EXPECT_EQ(ISOIF_LeCreateCig(0x00, &cigParam, 1, &cisParam), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_IsoifCreateCis_00100
 * @tc.name: ISOIF_LeCreateCis NULL params check
 * @tc.desc: NULL params must return BT_BAD_PARAM (checked before the enable guard)
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifCreateCis_00100, TestSize.Level1)
{
    EXPECT_EQ(ISOIF_LeCreateCis(1, nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_IsoifCreateCig_00200
 * @tc.name: ISOIF_LeCreateCig valid param: single CIS
 * @tc.desc: valid CIG + CIS params pass validation; command is sent to the controller
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifCreateCig_00200, TestSize.Level1)
{
    IsoLeCigParam cigParam = { };
    cigParam.sduIntervalMToS = 0x10;
    cigParam.sduIntervalSToM = 0x10;
    cigParam.slaveClockAccuracy = 0x00;
    cigParam.packing = 0x01;
    cigParam.framing = 0x00;
    cigParam.maxTransportLatencyMToS = 0x0010;
    cigParam.maxTransportLatencySToM = 0x0010;

    IsoLeCisParam cisParam = { };
    cisParam.cisId = 0x00;
    cisParam.maxSduMToS = 0x0100;
    cisParam.maxSduSToM = 0x0100;
    cisParam.phyMToS = 0x02;
    cisParam.phySToM = 0x02;
    cisParam.rtnMToS = 0x02;
    cisParam.rtnSToM = 0x02;

    int ret = ISOIF_LeCreateCig(0x00, &cigParam, 1, &cisParam);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);

    // Clean up the controller-side CIG 0x00 right away: later cases (e.g.
    // TwoDeviceCisDisconnect_00100) re-create CIG 0x00, and relying on
    // IsoifRemoveCig_00200 running first breaks under --gtest_filter or reordering.
    // The HCI command is sent in order, so the remove lands before any later create.
    (void)ISOIF_LeRemoveCig(0x00);
}

/**
 * @tc.number: StackGapLe52_IsoifCreateCis_00200
 * @tc.name: ISOIF_LeCreateCis valid param: single CIS
 * @tc.desc: valid params pass validation; command is sent to the controller
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifCreateCis_00200, TestSize.Level1)
{
    IsoLeCreateCisParam params[1] = { };
    params[0].cisHandle = 0x0000;
    params[0].aclHandle = 0x0040;

    int ret = ISOIF_LeCreateCis(1, params);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_IsoifRemoveCig_00200
 * @tc.name: ISOIF_LeRemoveCig valid CIG ID
 * @tc.desc: cigId=0x00; no param validation in path, only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifRemoveCig_00200, TestSize.Level1)
{
    int ret = ISOIF_LeRemoveCig(0x00);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.101
//   LE Accept CIS Request Command

/**
 * @tc.number: StackGapLe52_IsoifAcceptCisRequest_00200
 * @tc.name: ISOIF_LeAcceptCisRequest valid CIS handle
 * @tc.desc: cisHandle=0x0000; scalar params, only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifAcceptCisRequest_00200, TestSize.Level1)
{
    int ret = ISOIF_LeAcceptCisRequest(0x0000);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.102
//   LE Reject CIS Request Command

/**
 * @tc.number: StackGapLe52_IsoifRejectCisRequest_00200
 * @tc.name: ISOIF_LeRejectCisRequest valid CIS handle and reason
 * @tc.desc: cisHandle=0x0000, reason=0x3A; only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifRejectCisRequest_00200, TestSize.Level1)
{
    int ret = ISOIF_LeRejectCisRequest(0x0000, 0x3A);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec, Vol 2, Part E, §7.1.6
//   Disconnect Command: no dedicated CIS disconnect command; the master terminates
//   a CIS with the generic HCI_Disconnect (Vol 6, Part D, §6.25). No Command_Complete
//   is returned for it - the result arrives via the Disconnection Complete (0x05) event.

/**
 * @tc.number: StackGapLe52_IsoifLeDisconnectCis_00200
 * @tc.name: ISOIF_LeDisconnectCis valid params
 * @tc.desc: cisHandle=0x0040, reason=0x13 (Remote User Terminated Connection); no pointer
 *           or range validation in the path, only transport/allocation errors matter.
 *           ISO module is started with BTM_Enable, so the enable guard must not reject it.
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifLeDisconnectCis_00200, TestSize.Level1)
{
    int ret = ISOIF_LeDisconnectCis(0x0040, 0x13);
    EXPECT_NE(ret, BT_BAD_STATUS);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_E2eCisDisconnectComplete_00100
 * @tc.name: Disconnection Complete (0x05) filtered and reported for a tracked CIS
 * @tc.desc: inject LE CIS Established (0x19) to register the handle, then inject 0x05 for
 *           it -> cisDisconnected fires with status/reason passthrough and the tracking
 *           entry is removed; a second 0x05 and an unregistered ACL/SCO-style handle
 *           report nothing (Vol 4, Part E, 7.7.5 broadcast + cisList filter)
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_E2eCisDisconnectComplete_00100, TestSize.Level1)
{
    CisDisconnectedResult result;
    IsoLeCigCallback cb = { };
    cb.cisDisconnected = OnCisDisconnected;
    CigCallbackGuard guard;
    ASSERT_EQ(ISOIF_LeRegisterCigCallback(&cb, &result), BT_SUCCESS);

    // Register handle 0x0042 by simulating the LE CIS Established (0x19) event.
    HciLeCisEstablishedEventParam est = { };
    est.status = HCI_STATUS_SUCCESS;
    est.connectionHandle = 0x0042;
    IsoRecvLeCisEstablished(&est);

    // Disconnect that CIS: 0x05 with status 0, reason 0x16 (Connection Terminated by
    // Local Host, the value the local host observes on an HCI_Disconnect, Vol 2 E 7.1.6).
    HciDisconnectCompleteEventParam disc = { };
    disc.status = HCI_STATUS_SUCCESS;
    disc.connectionHandle = 0x0042;
    disc.reason = 0x16;
    IsoRecvLeDisconnectComplete(&disc);
    ASSERT_TRUE(result.Wait()) << "cisDisconnected not fired for tracked CIS";
    EXPECT_EQ(result.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(result.cisHandle, 0x0042);
    EXPECT_EQ(result.reason, 0x16);

    // The tracking entry was removed: a repeated 0x05 for the same handle is silent.
    result.Reset();
    IsoRecvLeDisconnectComplete(&disc);
    EXPECT_FALSE(result.Wait(500)) << "cisDisconnected must not fire after the entry is removed";

    // An ACL/SCO-style handle (never registered in cisList) must not notify.
    result.Reset();
    HciDisconnectCompleteEventParam acl = { };
    acl.status = HCI_STATUS_SUCCESS;
    acl.connectionHandle = 0x0005;
    acl.reason = 0x16;
    IsoRecvLeDisconnectComplete(&acl);
    EXPECT_FALSE(result.Wait(500)) << "cisDisconnected must not fire for ACL/SCO handle";

    // CigCallbackGuard deregisters on exit.
}

/**
 * @tc.number: StackGapLe52_E2eCisEstablishedTracking_00100
 * @tc.name: duplicate LE CIS Established (0x19) registers the handle exactly once
 * @tc.desc: two 0x19 events for the same handle -> cisList holds a single entry; the first
 *           0x05 removes it and reports once, a second 0x05 reports nothing, proving the
 *           dedup kept only one tracking entry
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_E2eCisEstablishedTracking_00100, TestSize.Level1)
{
    CisDisconnectedResult result;
    IsoLeCigCallback cb = { };
    cb.cisDisconnected = OnCisDisconnected;
    CigCallbackGuard guard;
    ASSERT_EQ(ISOIF_LeRegisterCigCallback(&cb, &result), BT_SUCCESS);

    // Two 0x19 events for the same handle; the tracking list must stay deduplicated.
    HciLeCisEstablishedEventParam est = { };
    est.status = HCI_STATUS_SUCCESS;
    est.connectionHandle = 0x0042;
    IsoRecvLeCisEstablished(&est);
    IsoRecvLeCisEstablished(&est);

    // One 0x05 removes the single entry and reports exactly once.
    HciDisconnectCompleteEventParam disc = { };
    disc.status = HCI_STATUS_SUCCESS;
    disc.connectionHandle = 0x0042;
    disc.reason = 0x16;
    IsoRecvLeDisconnectComplete(&disc);
    ASSERT_TRUE(result.Wait()) << "cisDisconnected not fired after duplicate 0x19";
    EXPECT_EQ(result.cisHandle, 0x0042);
    EXPECT_EQ(result.reason, 0x16);

    // A second 0x05 must be silent: the dedup kept only one tracking entry.
    result.Reset();
    IsoRecvLeDisconnectComplete(&disc);
    EXPECT_FALSE(result.Wait(500)) << "second 0x05 must not fire (duplicate 0x19 was deduped)";

    // CigCallbackGuard deregisters on exit.
}

// CIG parameters must match the peer's pre-configured CIG (stack_gap_le_interact_test.cpp
// InitIsoCisPeerSetup), otherwise the Link Layer cannot establish the CIS.
constexpr uint8_t TD_CIG_ID = 0x00;
constexpr uint8_t TD_CIS_ID = 0x00;
constexpr uint32_t TD_SDU_INTERVAL = 0x2710; // 10000 us = 10 ms
constexpr uint16_t TD_MAX_LATENCY = 0x000A;  // 10 ms == SDU interval (LL: latency = N x ISO interval)
constexpr uint16_t TD_MAX_SDU = 0x0100;      // 256 bytes
constexpr uint32_t TD_CIG_RESULT_TIMEOUT_MS = 5000;
constexpr uint32_t TD_CIS_TIMEOUT_MS = 10000;

// Register the CIG callbacks and create the CIG with the fixed two-device parameters.
// Returns false when the controller does not support CIS or the CIG cannot be created;
// the caller is expected to GTEST_SKIP() on failure. The CIS handle is returned through
// td.cig.cisHandles[0]. ASSERT_* macros would only skip the helper itself, so failures
// are reported through the return value (the caller's CigCallbackGuard deregisters the
// callbacks on the skip path).
static bool RegisterCisCallbacksAndCreateCig(IsoCisTdContext &td)
{
    // The registration keeps the pointer, not a copy (IsoRegisterCigCallback), so the callback
    // struct must outlive the function; a static mirrors stack_gap_le_interact_test.cpp's isoCb.
    static IsoLeCigCallback cb = { };
    cb.createCigResult = OnCisTdCigCreateResult;
    cb.cisEstablished = OnCisTdEstablished;
    cb.cisDisconnected = OnCisTdDisconnected;
    if (ISOIF_LeRegisterCigCallback(&cb, &td) != BT_SUCCESS) {
        return false;
    }

    IsoLeCigParam cigParam = { };
    cigParam.sduIntervalMToS = TD_SDU_INTERVAL;
    cigParam.sduIntervalSToM = TD_SDU_INTERVAL;
    cigParam.slaveClockAccuracy = 0x00;
    cigParam.packing = 0x01; // interleaved
    cigParam.framing = 0x00; // unframed
    cigParam.maxTransportLatencyMToS = TD_MAX_LATENCY;
    cigParam.maxTransportLatencySToM = TD_MAX_LATENCY;

    IsoLeCisParam cisParam = { };
    cisParam.cisId = TD_CIS_ID;
    cisParam.maxSduMToS = TD_MAX_SDU;
    cisParam.maxSduSToM = TD_MAX_SDU;
    cisParam.phyMToS = 0x02; // LE 2M
    cisParam.phySToM = 0x02;
    cisParam.rtnMToS = 0x02;
    cisParam.rtnSToM = 0x02;

    int ret = ISOIF_LeCreateCig(TD_CIG_ID, &cigParam, 1, &cisParam);
    if (ret == BT_NO_MEMORY) {
        printf("RegisterCisCallbacksAndCreateCig: ISOIF_LeCreateCig = %d\n", ret);
        return false;
    }
    if (!td.cig.Wait(TD_CIG_RESULT_TIMEOUT_MS)) {
        printf("RegisterCisCallbacksAndCreateCig: createCigResult not received\n");
        return false;
    }
    if (td.cig.status != HCI_STATUS_SUCCESS) {
        // A controller without CIS support reports an error (e.g. 0x01 Unknown HCI
        // Command); the rest of the flow cannot run, skip with a note.
        printf("createCigResult status=0x%02X, controller does not support CIS\n", td.cig.status);
        return false;
    }
    return (td.cig.cisCount == 1) && (td.cig.cisHandles[0] != 0);
}
// BT 5.2 spec, Vol 6, Part B, §4.5.14,3: "The Link Layer shall not create a CIS until
// the Host has configured the CIG that that CIS will belong to" -- the peer (slave) must
// configure its own CIG and answer LE CIS Request (0x1A) before the CIS can be
// established. The interact server's RunPeerMode does exactly that (InitIsoCisPeerSetup).

/**
 * @tc.number: StackGapLe52_TwoDeviceCisDisconnect_00100
 * @tc.name: two-device CIS established then disconnected with HCI_Disconnect
 * @tc.desc: connect the ACL, create a CIG/CIS on this (master) device; the peer's
 *           cisRequestInd auto-accepts; wait for LE CIS Established (0x19); then
 *           ISOIF_LeDisconnectCis (HCI_Disconnect, Vol 6 D §6.25) and wait for the
 *           Disconnection Complete (0x05) with reason 0x16 (Connection Terminated by
 *           Local Host, Vol 2 E §7.1.6); finally remove the CIG. Skipped when
 *           BT52_PEER_ADDR is unset or the controller does not support CIS. CIG params
 *           are a hardware-tuning point and may need adjustment on a specific board.
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_TwoDeviceCisDisconnect_00100, TestSize.Level1)
{
    if (g_peerAddrArg[0] == '\0') {
        GTEST_SKIP() << "peer address not set (BT52_PEER_ADDR=XX:XX:XX:XX:XX:XX)";
    }

    uint16_t connHandle = 0;
    if (!ConnectToPeer(connHandle)) {
        GTEST_SKIP() << "peer not connected";
    }

    IsoCisTdContext td;
    CigCallbackGuard guard;
    if (!RegisterCisCallbacksAndCreateCig(td)) {
        GTEST_SKIP() << "controller does not support CIS";
    }
    uint16_t cisHandle = td.cig.cisHandles[0];

    // Ask the LL to establish the CIS with the peer (LE Create CIS, 0x0064); the peer's
    // cisRequestInd auto-accepts, then both hosts observe LE CIS Established (0x19).
    IsoLeCreateCisParam createCis[1] = { };
    createCis[0].cisHandle = cisHandle;
    createCis[0].aclHandle = connHandle;
    int ret = ISOIF_LeCreateCis(1, createCis);
    EXPECT_NE(ret, BT_NO_MEMORY);
    ASSERT_TRUE(td.est.Wait(TD_CIS_TIMEOUT_MS)) << "cisEstablished (0x19) not received";
    EXPECT_EQ(td.est.status, HCI_STATUS_SUCCESS);

    // Master terminates the CIS with HCI_Disconnect (0x0406), reason 0x13 (Remote User
    // Terminated Connection); the local Disconnection Complete carries reason 0x16.
    td.disc.Reset();
    EXPECT_NE(ISOIF_LeDisconnectCis(cisHandle, 0x13), BT_BAD_STATUS);
    ASSERT_TRUE(td.disc.Wait(TD_CIS_TIMEOUT_MS)) << "cisDisconnected (0x05) not received";
    EXPECT_EQ(td.disc.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(td.disc.cisHandle, cisHandle);
    EXPECT_EQ(td.disc.reason, 0x16) << "local disconnect observes reason 0x16 (Vol 2 E 7.1.6)";

    // Tear the CIG down.
    EXPECT_NE(ISOIF_LeRemoveCig(TD_CIG_ID), BT_BAD_STATUS);

    // CigCallbackGuard deregisters on exit.
}

/**
 * @tc.number: StackGapLe52_IsoifRegisterCigCallback_00200
 * @tc.name: ISOIF_LeRegisterCigCallback with Established/Reject callbacks
 * @tc.desc: register callbacks whose handlers share the single registration context type
 *           (CisStatusResult), then deregister (idempotent). createCigResult/cisRequestInd
 *           are omitted here because their handlers cast the context to another type
 *           (CigCreateResult/CisRequestResult) and would corrupt the context if fired.
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifRegisterCigCallback_00200, TestSize.Level1)
{
    CisStatusResult cisStatusResult = { };

    IsoLeCigCallback cb = { };
    cb.cisEstablished = OnCisEstablished;
    cb.rejectCisResult = OnRejectCisResult;

    EXPECT_EQ(ISOIF_LeRegisterCigCallback(&cb, &cisStatusResult), BT_SUCCESS);
    EXPECT_EQ(ISOIF_LeDeregisterCigCallback(), BT_SUCCESS);
    EXPECT_EQ(ISOIF_LeDeregisterCigCallback(), BT_SUCCESS);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.103
//   LE Create BIG Command

/**
 * @tc.number: StackGapLe52_HciCreateBig_00100
 * @tc.name: HCI_LeCreateBig NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciCreateBig_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeCreateBig(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciCreateBig_00200
 * @tc.name: HCI_LeCreateBig valid param
 * @tc.desc: numBis=1 -> command accepted, returns non BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciCreateBig_00200, TestSize.Level1)
{
    HciLeCreateBigParam param = { };
    param.bigHandle = 0x00;
    param.advertisingHandle = 0x00;
    param.numBis = 1;
    param.sduInterval[0] = 0x64;
    param.maxSdu = 0x0100;
    param.maxTransportLatency = 0x0FFF;
    param.rtn = 0x02;
    param.phy = 0x01;
    param.packing = 0x00;
    param.framing = 0x00;
    param.encryption = 0x00;

    int ret = HCI_LeCreateBig(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.104
//   LE Create BIG Test Command

/**
 * @tc.number: StackGapLe52_HciCreateBigTest_00100
 * @tc.name: HCI_LeCreateBigTest NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciCreateBigTest_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeCreateBigTest(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciCreateBigTest_00200
 * @tc.name: HCI_LeCreateBigTest valid param
 * @tc.desc: numBis=1 -> command accepted, returns non BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciCreateBigTest_00200, TestSize.Level1)
{
    HciLeCreateBigTestParam param = { };
    param.bigHandle = 0x00;
    param.advertisingHandle = 0x00;
    param.numBis = 1;
    param.sduInterval[0] = 0x64;
    param.isoInterval = 0x10;
    param.numberOfSdu = 0x01;
    param.maxSdu = 0x0100;
    param.maxTransportLatency = 0x0FFF;
    param.rtn = 0x02;
    param.phy = 0x01;
    param.packing = 0x00;
    param.framing = 0x00;
    param.encryption = 0x00;

    int ret = HCI_LeCreateBigTest(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.105
//   LE Terminate BIG Command

/**
 * @tc.number: StackGapLe52_HciTerminateBig_00100
 * @tc.name: HCI_LeTerminateBig NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciTerminateBig_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeTerminateBig(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciTerminateBig_00200
 * @tc.name: HCI_LeTerminateBig valid param
 * @tc.desc: bigHandle=0x00, reason=0x16 -> command accepted, returns non BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciTerminateBig_00200, TestSize.Level1)
{
    HciLeTerminateBigParam param = { };
    param.bigHandle = 0x00;
    param.reason = 0x16; // HCI_ERR_TERMINATED_BY_LOCAL_HOST

    int ret = HCI_LeTerminateBig(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.106
//   LE BIG Create Sync Command

/**
 * @tc.number: StackGapLe52_HciBigCreateSync_00100
 * @tc.name: HCI_LeBigCreateSync NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciBigCreateSync_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeBigCreateSync(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciBigCreateSync_00200
 * @tc.name: HCI_LeBigCreateSync valid param
 * @tc.desc: numBis=1 with a single BIS index -> command accepted
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciBigCreateSync_00200, TestSize.Level1)
{
    uint8_t bis[1] = { 0x00 };
    HciLeBigCreateSyncParam param = { };
    param.bigHandle = 0x00;
    param.syncHandle = 0x0000;
    param.encryption = 0x00;
    param.mse = 0x00;
    param.bigSyncTimeout[0] = 0x10;
    param.numBis = 1;
    param.bis = bis;

    int ret = HCI_LeBigCreateSync(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.107
//   LE BIG Terminate Sync Command

/**
 * @tc.number: StackGapLe52_HciBigTerminateSync_00100
 * @tc.name: HCI_LeBigTerminateSync NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciBigTerminateSync_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeBigTerminateSync(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciBigTerminateSync_00200
 * @tc.name: HCI_LeBigTerminateSync valid param
 * @tc.desc: bigHandle=0x00 -> command accepted, returns non BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciBigTerminateSync_00200, TestSize.Level1)
{
    HciLeBigTerminateSyncParam param = { };
    param.bigHandle = 0x00;

    int ret = HCI_LeBigTerminateSync(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.103
//   LE Create BIG Command

/**
 * @tc.number: StackGapLe52_IsoifCreateBig_00200
 * @tc.name: ISOIF_LeCreateBig valid parameters
 * @tc.desc: bigHandle=0x00, numBis=1; only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifCreateBig_00200, TestSize.Level1)
{
    IsoLeBigParam bigParam = { };
    bigParam.sduInterval = 0x64;
    bigParam.maxSdu = 0x0100;
    bigParam.maxTransportLatency = 0x0FFF;
    bigParam.rtn = 0x02;
    bigParam.phy = 0x01;
    bigParam.packing = 0x00;
    bigParam.framing = 0x00;
    bigParam.encryption = 0x00;

    int ret = ISOIF_LeCreateBig(0x00, 0x00, 1, &bigParam, nullptr);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.104
//   LE Create BIG Test Command

/**
 * @tc.number: StackGapLe52_IsoifCreateBigTest_00200
 * @tc.name: ISOIF_LeCreateBigTest valid parameters
 * @tc.desc: bigHandle=0x00, numBis=1; only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifCreateBigTest_00200, TestSize.Level1)
{
    IsoLeBigTestParam bigParam = { };
    bigParam.sduInterval = 0x64;
    bigParam.isoInterval = 0x10;
    bigParam.numberOfSdu = 0x01;
    bigParam.maxSdu = 0x0100;
    bigParam.maxTransportLatency = 0x0FFF;
    bigParam.rtn = 0x02;
    bigParam.phy = 0x01;
    bigParam.packing = 0x00;
    bigParam.framing = 0x00;
    bigParam.encryption = 0x00;

    int ret = ISOIF_LeCreateBigTest(0x00, 0x00, 1, &bigParam, nullptr);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.105
//   LE Terminate BIG Command

/**
 * @tc.number: StackGapLe52_IsoifTerminateBig_00200
 * @tc.name: ISOIF_LeTerminateBig valid BIG handle and reason
 * @tc.desc: bigHandle=0x00, reason=0x16; only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifTerminateBig_00200, TestSize.Level1)
{
    int ret = ISOIF_LeTerminateBig(0x00, 0x16);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.106
//   LE BIG Create Sync Command

/**
 * @tc.number: StackGapLe52_IsoifBigCreateSync_00200
 * @tc.name: ISOIF_LeBigCreateSync valid parameters
 * @tc.desc: numBis=1 with a single BIS index; only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifBigCreateSync_00200, TestSize.Level1)
{
    uint8_t bis[1] = { 0x00 };
    IsoLeBigCreateSyncParam param = { };
    param.bigHandle = 0x00;
    param.syncHandle = 0x0000;
    param.encryption = 0x00;
    param.mse = 0x00;
    param.bigSyncTimeout = 0x10;
    param.numBis = 1;
    param.bis = bis;

    int ret = ISOIF_LeBigCreateSync(&param);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_IsoifBigCreateSync_00100
 * @tc.name: ISOIF_LeBigCreateSync NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM without crashing. Regression for the
 *           review-round-1 finding F2: the entry LOG_INFO used to dereference param
 *           before the NULL check, crashing on NULL input.
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifBigCreateSync_00100, TestSize.Level1)
{
    EXPECT_EQ(ISOIF_LeBigCreateSync(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_IsoifBigCreateSync_00300
 * @tc.name: ISOIF_LeBigCreateSync numBis range
 * @tc.desc: numBis=0 (sync to all BISes of the BIG, Vol 4 Part E 7.8.106) is accepted
 *           and bis may then be NULL; numBis=32 (above ISO_LE_BIS_COUNT_MAX=31) must
 *           return BT_BAD_PARAM. Regression for the review-round-1 finding F3: the
 *           missing bound check used to forward a spec-invalid command (and an
 *           unbounded source read of the bis array) to the HCI layer.
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifBigCreateSync_00300, TestSize.Level1)
{
    uint8_t bis[1] = { 0x00 };

    IsoLeBigCreateSyncParam paramAll = { };
    paramAll.bigHandle = 0x00;
    paramAll.encryption = 0x00;
    paramAll.mse = 0x00;
    paramAll.bigSyncTimeout = 0x10;
    paramAll.numBis = 0;
    paramAll.bis = NULL;
    EXPECT_NE(ISOIF_LeBigCreateSync(&paramAll), BT_BAD_PARAM);

    IsoLeBigCreateSyncParam paramOver = { };
    paramOver.bigHandle = 0x00;
    paramOver.encryption = 0x00;
    paramOver.mse = 0x00;
    paramOver.bigSyncTimeout = 0x10;
    paramOver.numBis = ISO_LE_BIS_COUNT_MAX + 1; // 32 > 31
    paramOver.bis = bis;
    EXPECT_EQ(ISOIF_LeBigCreateSync(&paramOver), BT_BAD_PARAM);
}

// BT 5.2 spec, Vol 2, Part E, §7.8.107
//   LE BIG Terminate Sync Command

/**
 * @tc.number: StackGapLe52_IsoifBigTerminateSync_00200
 * @tc.name: ISOIF_LeBigTerminateSync valid BIG handle
 * @tc.desc: bigHandle=0x00; only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifBigTerminateSync_00200, TestSize.Level1)
{
    int ret = ISOIF_LeBigTerminateSync(0x00);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_IsoifRegisterBigCallback_00200
 * @tc.name: ISOIF_LeRegisterBigCallback with the full BIG callback set
 * @tc.desc: register a callback covering all 6 BIG/BIS fields, then deregister (idempotent)
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifRegisterBigCallback_00200, TestSize.Level1)
{
    BigStatusResult statusResult = { };

    IsoLeBigCallback cb = { };
    cb.createBigResult = OnBigCreateResult;
    cb.terminateBigResult = OnBigTerminateResult;
    cb.bigSyncEstablished = OnBigSyncEstablished;
    cb.bigSyncLost = OnBigSyncLost;
    cb.bigInfoReport = OnBigInfoReport;
    cb.bigTerminateSyncResult = OnBigTerminateSyncResult;

    EXPECT_EQ(ISOIF_LeRegisterBigCallback(&cb, &statusResult), BT_SUCCESS);
    EXPECT_EQ(ISOIF_LeDeregisterBigCallback(), BT_SUCCESS);
    EXPECT_EQ(ISOIF_LeDeregisterBigCallback(), BT_SUCCESS);

    (void)statusResult;
}

// BT 5.2 spec (amended), Vol 2, Part E, §7.8.109
//   LE Setup ISO Data Path Command

/**
 * @tc.number: StackGapLe52_HciSetupIsoDataPath_00100
 * @tc.name: HCI_LeSetupIsoDataPath NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciSetupIsoDataPath_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeSetupIsoDataPath(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciSetupIsoDataPath_00200
 * @tc.name: HCI_LeSetupIsoDataPath valid param
 * @tc.desc: 1-byte codec configuration -> command accepted, returns non BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciSetupIsoDataPath_00200, TestSize.Level1)
{
    uint8_t codecConfiguration[1] = { 0x00 };
    HciLeSetupIsoDataPathParam param = { };
    param.connectionHandle = 0x0000;
    param.dataPathDirection = 0x00;
    param.dataPathId = 0x00;
    param.codecConfigurationLength = 1;
    param.codecConfiguration = codecConfiguration;

    int ret = HCI_LeSetupIsoDataPath(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

// BT 5.2 spec (amended), Vol 2, Part E, §7.8.110
//   LE Remove ISO Data Path Command

/**
 * @tc.number: StackGapLe52_HciRemoveIsoDataPath_00100
 * @tc.name: HCI_LeRemoveIsoDataPath NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciRemoveIsoDataPath_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeRemoveIsoDataPath(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciRemoveIsoDataPath_00200
 * @tc.name: HCI_LeRemoveIsoDataPath valid param
 * @tc.desc: connectionHandle=0x0000, direction=0x00 -> command accepted
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciRemoveIsoDataPath_00200, TestSize.Level1)
{
    HciLeRemoveIsoDataPathParam param = { };
    param.connectionHandle = 0x0000;
    param.dataPathDirection = 0x00;

    int ret = HCI_LeRemoveIsoDataPath(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_IsoifSetupIsoDataPath_00200
 * @tc.name: ISOIF_LeSetupIsoDataPath valid parameters
 * @tc.desc: 1-byte codec configuration; only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifSetupIsoDataPath_00200, TestSize.Level1)
{
    uint8_t codecConfiguration[1] = { 0x00 };
    IsoLeSetupIsoDataPathParam param = { };
    param.connectionHandle = 0x0000;
    param.dataPathDirection = 0x00;
    param.dataPathId = 0x00;
    param.controllerDelay = 0x000000;
    param.codecConfigurationLength = 1;
    param.codecConfiguration = codecConfiguration;

    int ret = ISOIF_LeSetupIsoDataPath(&param);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_IsoifRemoveIsoDataPath_00200
 * @tc.name: ISOIF_LeRemoveIsoDataPath valid parameters
 * @tc.desc: connectionHandle=0x0000, direction=0x00; only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifRemoveIsoDataPath_00200, TestSize.Level1)
{
    int ret = ISOIF_LeRemoveIsoDataPath(0x0000, 0x00);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_IsoifRegisterDataPathCallback_00200
 * @tc.name: ISOIF_LeRegisterDataPathCallback with the full data path callback set
 * @tc.desc: register a callback covering both data path result fields, then deregister (idempotent)
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifRegisterDataPathCallback_00200, TestSize.Level1)
{
    IsoDataPathStatusResult statusResult = { };

    IsoLeDataPathCallback cb = { };
    cb.setupIsoDataPathResult = OnSetupIsoDataPathResult;
    cb.removeIsoDataPathResult = OnRemoveIsoDataPathResult;

    EXPECT_EQ(ISOIF_LeRegisterDataPathCallback(&cb, &statusResult), BT_SUCCESS);
    EXPECT_EQ(ISOIF_LeDeregisterDataPathCallback(), BT_SUCCESS);
    EXPECT_EQ(ISOIF_LeDeregisterDataPathCallback(), BT_SUCCESS);

    (void)statusResult;
}

// BT 5.2 spec (amended), Vol 2, Part E, §7.8.111-7.8.114
//   LE ISO Transmit Test / Receive Test / Read Test Counters / Test End

/**
 * @tc.number: StackGapLe52_HciIsoTransmitTest_00100
 * @tc.name: HCI_LeIsoTransmitTest NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciIsoTransmitTest_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeIsoTransmitTest(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciIsoTransmitTest_00200
 * @tc.name: HCI_LeIsoTransmitTest valid param
 * @tc.desc: payloadType=0x01 variable length -> command accepted
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciIsoTransmitTest_00200, TestSize.Level1)
{
    HciLeIsoTransmitTestParam param = { };
    param.connectionHandle = 0x0000;
    param.payloadType = 0x01;

    int ret = HCI_LeIsoTransmitTest(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_HciIsoReceiveTest_00100
 * @tc.name: HCI_LeIsoReceiveTest NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciIsoReceiveTest_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeIsoReceiveTest(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciIsoReceiveTest_00200
 * @tc.name: HCI_LeIsoReceiveTest valid param
 * @tc.desc: payloadType=0x01 variable length -> command accepted
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciIsoReceiveTest_00200, TestSize.Level1)
{
    HciLeIsoReceiveTestParam param = { };
    param.connectionHandle = 0x0000;
    param.payloadType = 0x01;

    int ret = HCI_LeIsoReceiveTest(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_HciIsoReadTestCounters_00100
 * @tc.name: HCI_LeIsoReadTestCounters NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciIsoReadTestCounters_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeIsoReadTestCounters(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciIsoReadTestCounters_00200
 * @tc.name: HCI_LeIsoReadTestCounters valid param
 * @tc.desc: connectionHandle=0x0000 -> command accepted
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciIsoReadTestCounters_00200, TestSize.Level1)
{
    HciLeIsoReadTestCountersParam param = { };
    param.connectionHandle = 0x0000;

    int ret = HCI_LeIsoReadTestCounters(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_HciIsoTestEnd_00100
 * @tc.name: HCI_LeIsoTestEnd NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciIsoTestEnd_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeIsoTestEnd(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciIsoTestEnd_00200
 * @tc.name: HCI_LeIsoTestEnd valid param
 * @tc.desc: connectionHandle=0x0000 -> command accepted
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciIsoTestEnd_00200, TestSize.Level1)
{
    HciLeIsoTestEndParam param = { };
    param.connectionHandle = 0x0000;

    int ret = HCI_LeIsoTestEnd(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_IsoifIsoTransmitTest_00200
 * @tc.name: ISOIF_LeIsoTransmitTest valid parameters
 * @tc.desc: payloadType=0x01; only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifIsoTransmitTest_00200, TestSize.Level1)
{
    int ret = ISOIF_LeIsoTransmitTest(0x0000, 0x01);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_IsoifIsoReceiveTest_00200
 * @tc.name: ISOIF_LeIsoReceiveTest valid parameters
 * @tc.desc: payloadType=0x01; only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifIsoReceiveTest_00200, TestSize.Level1)
{
    int ret = ISOIF_LeIsoReceiveTest(0x0000, 0x01);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_IsoifIsoReadTestCounters_00200
 * @tc.name: ISOIF_LeIsoReadTestCounters valid connection handle
 * @tc.desc: connectionHandle=0x0000; only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifIsoReadTestCounters_00200, TestSize.Level1)
{
    int ret = ISOIF_LeIsoReadTestCounters(0x0000);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_IsoifIsoTestEnd_00200
 * @tc.name: ISOIF_LeIsoTestEnd valid connection handle
 * @tc.desc: connectionHandle=0x0000; only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifIsoTestEnd_00200, TestSize.Level1)
{
    int ret = ISOIF_LeIsoTestEnd(0x0000);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_IsoifRegisterTestCallback_00200
 * @tc.name: ISOIF_LeRegisterTestCallback with the full ISO test callback set
 * @tc.desc: register a callback covering all 4 test result fields, then deregister (idempotent)
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifRegisterTestCallback_00200, TestSize.Level1)
{
    IsoTestStatusResult statusResult = { };

    IsoLeTestCallback cb = { };
    cb.transmitTestResult = OnIsoTransmitTestResult;
    cb.receiveTestResult = OnIsoReceiveTestResult;
    cb.readTestCountersResult = OnIsoReadTestCountersResult;
    cb.testEndResult = OnIsoTestEndResult;

    EXPECT_EQ(ISOIF_LeRegisterTestCallback(&cb, &statusResult), BT_SUCCESS);
    EXPECT_EQ(ISOIF_LeDeregisterTestCallback(), BT_SUCCESS);
    EXPECT_EQ(ISOIF_LeDeregisterTestCallback(), BT_SUCCESS);

    (void)statusResult;
}

/**
 * @tc.number: StackGapLe52_HciReadIsoLinkQuality_00100
 * @tc.name: HCI_LeReadIsoLinkQuality NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciReadIsoLinkQuality_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeReadIsoLinkQuality(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciReadIsoLinkQuality_00200
 * @tc.name: HCI_LeReadIsoLinkQuality valid param
 * @tc.desc: connectionHandle=0x0000 -> command accepted
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciReadIsoLinkQuality_00200, TestSize.Level1)
{
    HciLeReadIsoLinkQualityParam param = { };
    param.connectionHandle = 0x0000;

    int ret = HCI_LeReadIsoLinkQuality(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_HciReadIsoTxSync_00100
 * @tc.name: HCI_LeReadIsoTxSync NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciReadIsoTxSync_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeReadIsoTxSync(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciReadIsoTxSync_00200
 * @tc.name: HCI_LeReadIsoTxSync valid param
 * @tc.desc: connectionHandle=0x0000 -> command accepted
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciReadIsoTxSync_00200, TestSize.Level1)
{
    HciLeReadIsoTxSyncParam param = { };
    param.connectionHandle = 0x0000;

    int ret = HCI_LeReadIsoTxSync(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_HciRequestPeerSca_00100
 * @tc.name: HCI_LeRequestPeerSca NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciRequestPeerSca_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeRequestPeerSca(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciRequestPeerSca_00200
 * @tc.name: HCI_LeRequestPeerSca valid param
 * @tc.desc: connectionHandle=0x0000 -> command accepted
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciRequestPeerSca_00200, TestSize.Level1)
{
    HciLeRequestPeerScaParam param = { };
    param.connectionHandle = 0x0000;

    int ret = HCI_LeRequestPeerSca(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_IsoifReadIsoLinkQuality_00200
 * @tc.name: ISOIF_LeReadIsoLinkQuality valid connection handle
 * @tc.desc: connectionHandle=0x0000; only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifReadIsoLinkQuality_00200, TestSize.Level1)
{
    int ret = ISOIF_LeReadIsoLinkQuality(0x0000);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_IsoifReadIsoTxSync_00200
 * @tc.name: ISOIF_LeReadIsoTxSync valid connection handle
 * @tc.desc: connectionHandle=0x0000; only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifReadIsoTxSync_00200, TestSize.Level1)
{
    int ret = ISOIF_LeReadIsoTxSync(0x0000);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_IsoifRequestPeerSca_00200
 * @tc.name: ISOIF_LeRequestPeerSca valid connection handle
 * @tc.desc: connectionHandle=0x0000; only transport/allocation errors matter
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifRequestPeerSca_00200, TestSize.Level1)
{
    int ret = ISOIF_LeRequestPeerSca(0x0000);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_IsoifRegisterStatusQueryCallback_00200
 * @tc.name: ISOIF_LeRegisterStatusQueryCallback with the full status query callback set
 * @tc.desc: register a callback covering all 3 status query result fields, then deregister (idempotent)
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_IsoifRegisterStatusQueryCallback_00200, TestSize.Level1)
{
    IsoStatusQueryResult statusResult = { };

    IsoLeStatusQueryCallback cb = { };
    cb.readIsoLinkQualityResult = OnReadIsoLinkQualityResult;
    cb.readIsoTxSyncResult = OnReadIsoTxSyncResult;
    cb.requestPeerScaResult = OnRequestPeerScaResult;

    // Safety net for an early exit; the explicit deregisters below keep testing
    // the idempotent-deregistration branch.
    StatusQueryCallbackGuard guard;

    EXPECT_EQ(ISOIF_LeRegisterStatusQueryCallback(&cb, &statusResult), BT_SUCCESS);
    EXPECT_EQ(ISOIF_LeDeregisterStatusQueryCallback(), BT_SUCCESS);
    EXPECT_EQ(ISOIF_LeDeregisterStatusQueryCallback(), BT_SUCCESS);

    (void)statusResult;
}

// Read ISO Link Quality complete (0x0075): 7 x 32-bit counters.
static void CheckReadIsoLinkQualityComplete(IsoStatusQueryResult &result)
{
    HciLeReadIsoLinkQualityReturnParam p1 = { };
    p1.status = HCI_STATUS_SUCCESS;
    p1.connectionHandle = 0x0042;
    p1.txUnackedPackets = 0x11111111;
    p1.txFlushedPackets = 0x22222222;
    p1.txLastSubeventPackets = 0x33333333;
    p1.retransmittedPackets = 0x44444444;
    p1.crcErrorPackets = 0x55555555;
    p1.rxUnreceivedPackets = 0x66666666;
    p1.duplicatePackets = 0x77777777;
    result.Reset();
    IsoLeReadIsoLinkQualityComplete(&p1);
    ASSERT_TRUE(result.Wait());
    EXPECT_EQ(result.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(result.connectionHandle, 0x0042);
    EXPECT_EQ(result.txUnackedPackets, 0x11111111);
    EXPECT_EQ(result.txFlushedPackets, 0x22222222);
    EXPECT_EQ(result.txLastSubeventPackets, 0x33333333);
    EXPECT_EQ(result.retransmittedPackets, 0x44444444);
    EXPECT_EQ(result.crcErrorPackets, 0x55555555);
    EXPECT_EQ(result.rxUnreceivedPackets, 0x66666666);
    EXPECT_EQ(result.duplicatePackets, 0x77777777);
}

// Read ISO TX Sync complete (0x0061): the 24-bit TX_Time_Stamp and Time_Offset
// fields are LSB-first on the wire, index 2 holds the most-significant byte.
constexpr size_t TIME_OFFSET_BYTE_MSB = 2;

static void CheckReadIsoTxSyncComplete(IsoStatusQueryResult &result)
{
    HciLeReadIsoTxSyncReturnParam p2 = { };
    p2.status = HCI_STATUS_SUCCESS;
    p2.connectionHandle = 0x0042;
    p2.packetSequenceNumber = 0x0102;
    p2.timeStamp[0] = 0x40;
    p2.timeStamp[1] = 0x30;
    p2.timeStamp[TIME_OFFSET_BYTE_MSB] = 0x20;   // 0x203040 (24-bit)
    p2.timeOffset[0] = 0x34;
    p2.timeOffset[1] = 0x12;
    p2.timeOffset[TIME_OFFSET_BYTE_MSB] = 0x00;
    result.Reset();
    IsoLeReadIsoTxSyncComplete(&p2);
    ASSERT_TRUE(result.Wait());
    EXPECT_EQ(result.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(result.connectionHandle, 0x0042);
    EXPECT_EQ(result.packetSequenceNumber, 0x0102);
    EXPECT_EQ(result.timeStamp, 0x203040);
    EXPECT_EQ(result.timeOffset, 0x00001234);
}

// Request Peer SCA complete (0x006D, event-based): inject through the HCI
// receive layer, dispatched to the ISO task thread.
static void CheckRequestPeerScaComplete(IsoStatusQueryResult &result)
{
    HciLeRequestPeerScaCompleteEventParam p3 = { };
    p3.status = HCI_STATUS_SUCCESS;
    p3.connectionHandle = 0x0042;
    p3.peerClockAccuracy = 0x03;
    result.Reset();
    IsoRecvLeRequestPeerScaComplete(&p3);
    ASSERT_TRUE(result.Wait());
    EXPECT_EQ(result.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(result.connectionHandle, 0x0042);
    EXPECT_EQ(result.peerClockAccuracy, 0x03);
}

/**
 * @tc.name: status query complete callbacks registered and unregistered branches
 * @tc.desc: covers if(callback) branch of the 3 status query complete handlers in
 *           iso_status_query.c, incl. the 24-bit timeOffset decode and the event-based
 *           0x006D injection through the HCI receive layer
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_StatusQueryCompleteCallbackBranch_00100, TestSize.Level1)
{
    IsoStatusQueryResult result;
    IsoLeStatusQueryCallback cb = { };
    cb.readIsoLinkQualityResult = OnReadIsoLinkQualityResult;
    cb.readIsoTxSyncResult = OnReadIsoTxSyncResult;
    cb.requestPeerScaResult = OnRequestPeerScaResult;
    StatusQueryCallbackGuard guard; // deregisters on early exit (Check* ASSERTs below)
    ASSERT_EQ(ISOIF_LeRegisterStatusQueryCallback(&cb, &result), BT_SUCCESS);

    CheckReadIsoLinkQualityComplete(result);
    CheckReadIsoTxSyncComplete(result);
    CheckRequestPeerScaComplete(result);

    // Unregistered: calling complete handlers after deregister must not crash.
    ASSERT_EQ(ISOIF_LeDeregisterStatusQueryCallback(), BT_SUCCESS);
    HciLeReadIsoLinkQualityReturnParam p1 = { };
    HciLeReadIsoTxSyncReturnParam p2 = { };
    HciLeRequestPeerScaCompleteEventParam p3 = { };
    IsoLeReadIsoLinkQualityComplete(&p1);
    IsoLeReadIsoTxSyncComplete(&p2);
    IsoRecvLeRequestPeerScaComplete(&p3);
}

/**
 * @tc.number: StackGapLe52_E2eReadIsoLinkQuality_00100
 * @tc.name: end-to-end 0x0075 with controller reply
 * @tc.desc: dispatch ISOIF_LeReadIsoLinkQuality, wait for readIsoLinkQualityResult.
 *           handle 0x0000 is not a CIS/BIS stream, so the controller replies an error,
 *           but the HCI command/complete path still round-trips through the stack
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_E2eReadIsoLinkQuality_00100, TestSize.Level1)
{
    IsoStatusQueryResult result;
    IsoLeStatusQueryCallback cb = { };
    cb.readIsoLinkQualityResult = OnReadIsoLinkQualityResult;
    StatusQueryCallbackGuard guard; // deregisters on early exit (EXPECT_TRUE below)
    ASSERT_EQ(ISOIF_LeRegisterStatusQueryCallback(&cb, &result), BT_SUCCESS);

    int ret = ISOIF_LeReadIsoLinkQuality(0x0000);
    EXPECT_NE(ret, BT_NO_MEMORY);
    EXPECT_TRUE(result.Wait()) << "readIsoLinkQualityResult not received from controller";
}

/**
 * @tc.number: StackGapLe52_E2eReadIsoTxSync_00100
 * @tc.name: end-to-end 0x0061 with controller reply
 * @tc.desc: dispatch ISOIF_LeReadIsoTxSync, wait for readIsoTxSyncResult.
 *           handle 0x0000 is not a CIS/BIS stream, so the controller replies an error,
 *           but the HCI command/complete path still round-trips through the stack
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_E2eReadIsoTxSync_00100, TestSize.Level1)
{
    IsoStatusQueryResult result;
    IsoLeStatusQueryCallback cb = { };
    cb.readIsoTxSyncResult = OnReadIsoTxSyncResult;
    StatusQueryCallbackGuard guard; // deregisters on early exit (EXPECT_TRUE below)
    ASSERT_EQ(ISOIF_LeRegisterStatusQueryCallback(&cb, &result), BT_SUCCESS);

    int ret = ISOIF_LeReadIsoTxSync(0x0000);
    EXPECT_NE(ret, BT_NO_MEMORY);
    EXPECT_TRUE(result.Wait()) << "readIsoTxSyncResult not received from controller";
}

/**
 * @tc.number: StackGapLe52_E2eRequestPeerSca_00100
 * @tc.name: end-to-end 0x006D with controller reply
 * @tc.desc: dispatch ISOIF_LeRequestPeerSca, wait for requestPeerScaResult.
 *           handle 0x0000 is not a connected ACL, so the controller replies an error,
 *           but the Command_Status/event path still round-trips through the stack
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_E2eRequestPeerSca_00100, TestSize.Level1)
{
    IsoStatusQueryResult result;
    IsoLeStatusQueryCallback cb = { };
    cb.requestPeerScaResult = OnRequestPeerScaResult;
    StatusQueryCallbackGuard guard; // deregisters on early exit (EXPECT_TRUE below)
    ASSERT_EQ(ISOIF_LeRegisterStatusQueryCallback(&cb, &result), BT_SUCCESS);

    int ret = ISOIF_LeRequestPeerSca(0x0000);
    EXPECT_NE(ret, BT_NO_MEMORY);
    EXPECT_TRUE(result.Wait()) << "requestPeerScaResult not received from controller";
}

/**
 * @tc.number: StackGapLe52_TwoDeviceRequestPeerSca_00100
 * @tc.name: two-device 0x006D with peer ACL connection
 * @tc.desc: connect to the peer over LE ACL, dispatch ISOIF_LeRequestPeerSca with
 *           the real connection handle, wait for requestPeerScaResult. The peer runs
 *           the interact test's server mode; skipped when BT52_PEER_ADDR is not set
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_TwoDeviceRequestPeerSca_00100, TestSize.Level1)
{
    if (g_peerAddrArg[0] == '\0') {
        GTEST_SKIP() << "peer address not set (BT52_PEER_ADDR=XX:XX:XX:XX:XX:XX)";
    }

    uint16_t connHandle = 0;
    if (!ConnectToPeer(connHandle)) {
        GTEST_SKIP() << "peer not connected";
    }

    IsoStatusQueryResult result;
    IsoLeStatusQueryCallback cb = { };
    cb.requestPeerScaResult = OnRequestPeerScaResult;
    StatusQueryCallbackGuard guard; // deregisters on GTEST_SKIP / early exit
    ASSERT_EQ(ISOIF_LeRegisterStatusQueryCallback(&cb, &result), BT_SUCCESS);

    int ret = ISOIF_LeRequestPeerSca(connHandle);
    EXPECT_NE(ret, BT_NO_MEMORY);
    EXPECT_TRUE(result.Wait()) << "requestPeerScaResult not received from peer controller";
    // Status 0x00 means the peer's controller actually reported a clock accuracy (spec 7.7.65,31):
    // verify the real value. Status 0x01 (Unknown HCI Command, Vol 1 Part F 2.1) means the local
    // controller does not implement 0x006D, so no value can be obtained -- skip with a note.
    if (result.status == HCI_STATUS_SUCCESS) {
        EXPECT_EQ(result.connectionHandle, connHandle) << "result for wrong connection handle";
        printf("peerClockAccuracy = 0x%02X (0x00-0x07 valid, 0xFF = unknown)\n", result.peerClockAccuracy);
        EXPECT_NE(result.peerClockAccuracy, 0xFF) << "peer clock accuracy not available";
    } else if (result.status == HCI_STATUS_UNKNOWN_COMMAND) {
        printf("controller does not implement HCI_LE_Request_Peer_SCA (0x006D), status=0x01\n");
        GTEST_SKIP() << "controller does not implement HCI_LE_Request_Peer_SCA (0x006D)";
    } else {
        ADD_FAILURE() << "unexpected Request Peer SCA status " << static_cast<int>(result.status);
    }
}

// ===================== Layer 6: GAP LE Power Control (0x0076/0x0077) =====================

/**
 * @tc.number: StackGapLe52_HciEnhancedReadTransmitPowerLevel_00100
 * @tc.name: HCI_LeEnhancedReadTransmitPowerLevel NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciEnhancedReadTransmitPowerLevel_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeEnhancedReadTransmitPowerLevel(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciEnhancedReadTransmitPowerLevel_00200
 * @tc.name: HCI_LeEnhancedReadTransmitPowerLevel valid param
 * @tc.desc: handle + phy -> command accepted, returns non BT_BAD_PARAM / BT_NO_MEMORY
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciEnhancedReadTransmitPowerLevel_00200, TestSize.Level1)
{
    HciLeEnhancedReadTransmitPowerLevelParam param = { };
    param.connectionHandle = 0x0000;
    param.phy = 0x01;

    int ret = HCI_LeEnhancedReadTransmitPowerLevel(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_HciReadRemoteTransmitPowerLevel_00100
 * @tc.name: HCI_LeReadRemoteTransmitPowerLevel NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciReadRemoteTransmitPowerLevel_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeReadRemoteTransmitPowerLevel(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciReadRemoteTransmitPowerLevel_00200
 * @tc.name: HCI_LeReadRemoteTransmitPowerLevel valid param
 * @tc.desc: handle + phy -> command accepted, returns non BT_BAD_PARAM / BT_NO_MEMORY
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciReadRemoteTransmitPowerLevel_00200, TestSize.Level1)
{
    HciLeReadRemoteTransmitPowerLevelParam param = { };
    param.connectionHandle = 0x0000;
    param.phy = 0x01;

    int ret = HCI_LeReadRemoteTransmitPowerLevel(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_GapifEnhancedReadTransmitPowerLevel_00100
 * @tc.name: GAPIF_LeEnhancedReadTransmitPowerLevel valid param
 * @tc.desc: command accepted (task + HCI send path), returns non BT_NO_MEMORY
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_GapifEnhancedReadTransmitPowerLevel_00100, TestSize.Level1)
{
    int ret = GAPIF_LeEnhancedReadTransmitPowerLevel(0x0000, 0x01);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_GapifReadRemoteTransmitPowerLevel_00100
 * @tc.name: GAPIF_LeReadRemoteTransmitPowerLevel valid param
 * @tc.desc: command accepted (task + HCI send path), returns non BT_NO_MEMORY
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_GapifReadRemoteTransmitPowerLevel_00100, TestSize.Level1)
{
    int ret = GAPIF_LeReadRemoteTransmitPowerLevel(0x0000, 0x01);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_GapifRegisterPowerControlCallback_00100
 * @tc.name: GAPIF_LeRegisterPowerControlCallback/Deregister
 * @tc.desc: register/deregister return BT_SUCCESS, and deregister is idempotent (repeat call)
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_GapifRegisterPowerControlCallback_00100, TestSize.Level1)
{
    GapLePowerControlCallback cb = { };
    cb.enhancedReadTransmitPowerResult = OnEnhancedReadTransmitPowerResult;
    cb.setPathLossReportingParamsResult = OnSetPathLossReportingParamsResult;
    cb.setPathLossReportingEnableResult = OnSetPathLossReportingEnableResult;
    cb.setTransmitPowerReportingEnableResult = OnSetTransmitPowerReportingEnableResult;
    cb.pathLossThreshold = OnPathLossThreshold;
    cb.transmitPowerReporting = OnTransmitPowerReporting;
    EXPECT_EQ(GAPIF_LeRegisterPowerControlCallback(&cb, nullptr), BT_SUCCESS);
    EXPECT_EQ(GAPIF_LeDeregisterPowerControlCallback(), BT_SUCCESS);
    EXPECT_EQ(GAPIF_LeDeregisterPowerControlCallback(), BT_SUCCESS);
}

// ===================== Layer 7: GAP LE Path Loss Reporting (0x0078/0x0079) =====================

/**
 * @tc.number: StackGapLe52_HciSetPathLossReportingParameters_00100
 * @tc.name: HCI_LeSetPathLossReportingParameters NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciSetPathLossReportingParameters_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeSetPathLossReportingParameters(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciSetPathLossReportingParameters_00200
 * @tc.name: HCI_LeSetPathLossReportingParameters valid param
 * @tc.desc: valid thresholds -> command accepted, returns non BT_BAD_PARAM / BT_NO_MEMORY
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciSetPathLossReportingParameters_00200, TestSize.Level1)
{
    HciLeSetPathLossReportingParametersParam param = { };
    param.connectionHandle = 0x0000;
    param.highThreshold = 80;
    param.highHysteresis = 10;
    param.lowThreshold = 40;
    param.lowHysteresis = 10;
    param.minTimeSpent = 0x0000;

    int ret = HCI_LeSetPathLossReportingParameters(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_HciSetPathLossReportingEnable_00100
 * @tc.name: HCI_LeSetPathLossReportingEnable NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciSetPathLossReportingEnable_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeSetPathLossReportingEnable(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciSetPathLossReportingEnable_00200
 * @tc.name: HCI_LeSetPathLossReportingEnable valid param
 * @tc.desc: handle + enable -> command accepted, returns non BT_BAD_PARAM / BT_NO_MEMORY
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciSetPathLossReportingEnable_00200, TestSize.Level1)
{
    HciLeSetPathLossReportingEnableParam param = { };
    param.connectionHandle = 0x0000;
    param.enable = 0x01;

    int ret = HCI_LeSetPathLossReportingEnable(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_GapifSetPathLossReportingParameters_00100
 * @tc.name: GAPIF_LeSetPathLossReportingParameters valid param
 * @tc.desc: valid thresholds -> command accepted (task + HCI send path), returns non BT_NO_MEMORY
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_GapifSetPathLossReportingParameters_00100, TestSize.Level1)
{
    GapLePathLossReportingParams params = { 0x0000, 80, 10, 40, 10, 0x0000 };
    int ret = GAPIF_LeSetPathLossReportingParameters(&params);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_GapifSetPathLossReportingParameters_00200
 * @tc.name: GAPIF_LeSetPathLossReportingParameters semantic validation
 * @tc.desc: highThreshold + highHysteresis > 0xFF must return BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_GapifSetPathLossReportingParameters_00200, TestSize.Level1)
{
    GapLePathLossReportingParams params = { 0x0000, 250, 10, 40, 10, 0x0000 };
    int ret = GAPIF_LeSetPathLossReportingParameters(&params);
    EXPECT_EQ(ret, BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_GapifSetPathLossReportingEnable_00100
 * @tc.name: GAPIF_LeSetPathLossReportingEnable valid param
 * @tc.desc: handle + enable 0x01 -> command accepted, returns non BT_NO_MEMORY
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_GapifSetPathLossReportingEnable_00100, TestSize.Level1)
{
    int ret = GAPIF_LeSetPathLossReportingEnable(0x0000, 0x01);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_GapifSetPathLossReportingEnable_00200
 * @tc.name: GAPIF_LeSetPathLossReportingEnable semantic validation
 * @tc.desc: enable other than 0x00/0x01 must return BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_GapifSetPathLossReportingEnable_00200, TestSize.Level1)
{
    int ret = GAPIF_LeSetPathLossReportingEnable(0x0000, 0x02);
    EXPECT_EQ(ret, BT_BAD_PARAM);
}

// ===================== Layer 8: LE Transmit Power Reporting Enable (0x007A) =====================

/**
 * @tc.number: StackGapLe52_HciSetTransmitPowerReportingEnable_00100
 * @tc.name: HCI_LeSetTransmitPowerReportingEnable NULL param check
 * @tc.desc: NULL param must return BT_BAD_PARAM; the command must not be sent
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciSetTransmitPowerReportingEnable_00100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeSetTransmitPowerReportingEnable(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe52_HciSetTransmitPowerReportingEnable_00200
 * @tc.name: HCI_LeSetTransmitPowerReportingEnable valid param
 * @tc.desc: handle + localEnable + remoteEnable -> command accepted, returns non BT_BAD_PARAM / BT_NO_MEMORY
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_HciSetTransmitPowerReportingEnable_00200, TestSize.Level1)
{
    HciLeSetTransmitPowerReportingEnableParam param = { };
    param.connectionHandle = 0x0000;
    param.localEnable = 0x01;
    param.remoteEnable = 0x01;

    int ret = HCI_LeSetTransmitPowerReportingEnable(&param);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_GapifSetTransmitPowerReportingEnable_00100
 * @tc.name: GAPIF_LeSetTransmitPowerReportingEnable valid param
 * @tc.desc: handle + localEnable/remoteEnable 0x01 -> command accepted, returns non BT_NO_MEMORY
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_GapifSetTransmitPowerReportingEnable_00100, TestSize.Level1)
{
    int ret = GAPIF_LeSetTransmitPowerReportingEnable(0x0000, 0x01, 0x01);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe52_GapifSetTransmitPowerReportingEnable_00200
 * @tc.name: GAPIF_LeSetTransmitPowerReportingEnable semantic validation
 * @tc.desc: localEnable/remoteEnable other than 0x00/0x01 must return BT_BAD_PARAM
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_GapifSetTransmitPowerReportingEnable_00200, TestSize.Level1)
{
    EXPECT_EQ(GAPIF_LeSetTransmitPowerReportingEnable(0x0000, 0x02, 0x01), BT_BAD_PARAM);
    EXPECT_EQ(GAPIF_LeSetTransmitPowerReportingEnable(0x0000, 0x01, 0x02), BT_BAD_PARAM);
}

// ===================== EATT L2CAP layer tests (plan chapter 3) =====================
// Limitation: the EATT tests assert on the L2CAP callbacks (result code, attempted/
// succeeded, effective MTU, echoed payload), not on the raw 0x17/0x18/0x19/0x1A
// frame bytes. Field offsets and byte order are therefore only exercised
// indirectly: the result codes and min(request, peer local config) semantics prove
// the responder decodes our frames, but a wrong field offset that both boards
// share would still pass. Pin the wire format with an HCI capture if needed; none
// is wired up here.

// Per-API guard batches of StackGapLe52_EattApiValidation_00100, extracted into helpers so each
// function stays under the coding-style function length limit. The helpers must run in this order: the
// connection/reconfigure guards need no PSM, the register helper (re)creates the PSM, the config /
// sec-level / sec-info helpers act on the registered PSM, and the test deregisters at the end.

static void EattCheckConnReqGuards()
{
    EattConnReqArg connArg = { };
    connArg.addr = g_peerAddr;
    connArg.mtu = EATT_MIN_MTU;
    connArg.mps = EATT_MIN_MTU;
    connArg.credit = 1;
    connArg.n = 1;
    connArg.nullAddr = true;
    EXPECT_EQ(CallL2cap(EattConnReqFn, &connArg), BT_BAD_PARAM); // addr == nullptr
    connArg.nullAddr = false;
    connArg.nullLcids = true;
    EXPECT_EQ(CallL2cap(EattConnReqFn, &connArg), BT_BAD_PARAM); // lcids == nullptr
    connArg.nullLcids = false;
    connArg.nullCfg = true;
    EXPECT_EQ(CallL2cap(EattConnReqFn, &connArg), BT_BAD_PARAM); // cfg == nullptr
    connArg.nullCfg = false;
    connArg.n = 0;
    EXPECT_EQ(CallL2cap(EattConnReqFn, &connArg), BT_BAD_PARAM); // n == 0
    connArg.n = EATT_MAX_CHANNEL + 1;
    EXPECT_EQ(CallL2cap(EattConnReqFn, &connArg), BT_BAD_PARAM); // n > 5
    connArg.n = 1;
    connArg.mtu = EATT_MIN_MTU - 1;
    EXPECT_EQ(CallL2cap(EattConnReqFn, &connArg), BT_BAD_PARAM); // mtu < 64
    connArg.mtu = EATT_MIN_MTU;
    connArg.mps = EATT_MIN_MTU - 1;
    EXPECT_EQ(CallL2cap(EattConnReqFn, &connArg), BT_BAD_PARAM); // mps < 64
    connArg.mps = EATT_MAX_MPS + 1;
    EXPECT_EQ(CallL2cap(EattConnReqFn, &connArg), BT_BAD_PARAM); // mps > 65533
    connArg.mps = EATT_MIN_MTU;
    EXPECT_EQ(CallL2cap(EattConnReqFn, &connArg), BT_BAD_PARAM); // PSM 0x0027 not registered
}

static void EattCheckReconfigReqGuards()
{
    EattReconfigArg reconfigArg = { };
    reconfigArg.mtu = EATT_MIN_MTU;
    reconfigArg.mps = EATT_MIN_MTU;
    reconfigArg.n = 1;
    reconfigArg.nullLcids = true;
    EXPECT_EQ(CallL2cap(EattReconfigFn, &reconfigArg), BT_BAD_PARAM); // lcids == nullptr
    reconfigArg.nullLcids = false;
    reconfigArg.lcids[0] = 0x0001;
    reconfigArg.n = 0;
    EXPECT_EQ(CallL2cap(EattReconfigFn, &reconfigArg), BT_BAD_PARAM); // n == 0
    reconfigArg.n = 1;
    reconfigArg.mtu = EATT_MIN_MTU - 1;
    EXPECT_EQ(CallL2cap(EattReconfigFn, &reconfigArg), BT_BAD_PARAM); // mtu < 64
    reconfigArg.mtu = EATT_MIN_MTU;
    reconfigArg.mps = EATT_MIN_MTU - 1;
    EXPECT_EQ(CallL2cap(EattReconfigFn, &reconfigArg), BT_BAD_PARAM); // mps < 64
    reconfigArg.mps = EATT_MAX_MPS + 1;
    EXPECT_EQ(CallL2cap(EattReconfigFn, &reconfigArg), BT_BAD_PARAM); // mps > 65533
    reconfigArg.mps = EATT_MIN_MTU;
    EXPECT_EQ(CallL2cap(EattReconfigFn, &reconfigArg), BT_BAD_PARAM); // invalid DCID (no channel)
}

static void EattCheckRegisterServiceGuards()
{
    EattRegisterServiceArg regArg = { };
    regArg.lpsm = L2CAP_LE_EATT_PSM;
    regArg.svc = nullptr;
    EXPECT_EQ(CallL2cap(EattRegisterServiceFn, &regArg), BT_BAD_PARAM); // svc == nullptr
    regArg.svc = &g_mockService;
    regArg.lpsm = 0x0026;
    EXPECT_EQ(CallL2cap(EattRegisterServiceFn, &regArg), BT_BAD_PARAM); // even PSM (bit0 clear)
    regArg.lpsm = 0x0127;
    EXPECT_EQ(CallL2cap(EattRegisterServiceFn, &regArg), BT_BAD_PARAM); // PSM with 0x0100 bit set
    regArg.lpsm = L2CAP_LE_EATT_PSM;
    EXPECT_EQ(CallL2cap(EattRegisterServiceFn, &regArg), BT_SUCCESS);
    EXPECT_EQ(CallL2cap(EattRegisterServiceFn, &regArg), BT_BAD_STATUS); // duplicate registration
}

static void EattCheckSetServiceConfig()
{
    EattSetServiceConfigArg cfgArg = { };
    cfgArg.lpsm = L2CAP_LE_EATT_PSM;
    cfgArg.cfg.mtu = SERVER_MTU;
    cfgArg.cfg.mps = SERVER_MTU;
    cfgArg.cfg.credit = 1;
    cfgArg.nullCfg = true;
    EXPECT_EQ(CallL2cap(EattSetServiceConfigFn, &cfgArg), BT_BAD_PARAM); // cfg == nullptr
    cfgArg.nullCfg = false;
    cfgArg.cfg.mtu = EATT_MIN_MTU - 1;
    EXPECT_EQ(CallL2cap(EattSetServiceConfigFn, &cfgArg), BT_BAD_PARAM); // mtu < 64
    cfgArg.cfg.mtu = SERVER_MTU;
    cfgArg.cfg.mps = EATT_MIN_MTU - 1;
    EXPECT_EQ(CallL2cap(EattSetServiceConfigFn, &cfgArg), BT_BAD_PARAM); // mps < 64
    cfgArg.cfg.mps = EATT_MAX_MPS + 1;
    EXPECT_EQ(CallL2cap(EattSetServiceConfigFn, &cfgArg), BT_BAD_PARAM); // mps > 65533
    cfgArg.cfg.mps = SERVER_MTU;
    EXPECT_EQ(CallL2cap(EattSetServiceConfigFn, &cfgArg), BT_SUCCESS);
}

static void EattCheckSetSecLevel()
{
    EattSetSecLevelArg secLevelArg = { };
    secLevelArg.lpsm = L2CAP_LE_EATT_PSM;
    secLevelArg.secRequirement = 0x04; // neither authentication (0x01) nor authorization (0x02)
    EXPECT_EQ(CallL2cap(EattSetSecLevelFn, &secLevelArg), BT_BAD_PARAM); // invalid bits
    secLevelArg.secRequirement = 0x00;
    EXPECT_EQ(CallL2cap(EattSetSecLevelFn, &secLevelArg), BT_SUCCESS);
}

// LE encryption key size range (Core 5.2 Vol 3 Part H 2.4.4): 7..16 octets.
constexpr uint8_t LE_MIN_KEY_SIZE = 7;
constexpr uint8_t LE_MAX_KEY_SIZE = 16;

static void EattCheckSetSecurityInfo()
{
    EattSetSecInfoArg secInfoArg = { };
    secInfoArg.addr = g_peerAddr;
    secInfoArg.keySize = LE_MAX_KEY_SIZE;
    secInfoArg.nullAddr = true;
    EXPECT_EQ(CallL2cap(EattSetSecInfoFn, &secInfoArg), BT_BAD_PARAM); // addr == nullptr
    secInfoArg.nullAddr = false;
    secInfoArg.keySize = LE_MIN_KEY_SIZE - 1;
    EXPECT_EQ(CallL2cap(EattSetSecInfoFn, &secInfoArg), BT_BAD_PARAM); // keySize < LE_MIN_KEY_SIZE
    secInfoArg.keySize = LE_MAX_KEY_SIZE + 1;
    EXPECT_EQ(CallL2cap(EattSetSecInfoFn, &secInfoArg), BT_BAD_PARAM); // keySize > LE_MAX_KEY_SIZE
    secInfoArg.keySize = LE_MAX_KEY_SIZE;
    EXPECT_EQ(CallL2cap(EattSetSecInfoFn, &secInfoArg), BT_BAD_PARAM); // no connection
}

/**
 * @tc.number: StackGapLe52_EattApiValidation_00100
 * @tc.name: EATT public API parameter validation
 * @tc.desc: NULL / range / PSM-state guards of the five EATT L2CAP APIs; runs on a
 *           single board (no peer required). The connection-request guards are
 *           exercised while the PSM is still unregistered so no ACL connect is
 *           attempted. A link left over from an earlier two-device test would make
 *           the L2CAP_LeSetSecurityInfo "no connection" guard return BT_SUCCESS,
 *           so the test drops any existing link first. Every guard returns
 *           BT_BAD_PARAM, so each EXPECT is tied to the branch named by its inline
 *           comment; reordering the production checks would silently shift an
 *           expectation onto a different branch.
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_EattApiValidation_00100, TestSize.Level1)
{
    // Start from a clean state: no link, PSM not registered (guards below assert
    // both). The fixture TearDown also disconnects, but dropping it here keeps the
    // guard assumptions explicit even if the test ordering changes.
    ResetConnection();

    // The ATT startup registration (AttEattRegisterService, att_init.c
    // ATT_StartUpAsync) already holds the PSM in a fresh process; deregister it so
    // the guard batches below start from a clean slot. The register guard then
    // (re)creates it, and the cleanup at the end tears it down again.
    EattDeregisterServiceArg deregArg = { };
    deregArg.lpsm = L2CAP_LE_EATT_PSM;
    EXPECT_EQ(CallL2cap(EattDeregisterServiceFn, &deregArg), BT_SUCCESS);

    // Guard batches, one helper per public API, in the dependency order the guards
    // rely on: connection/reconfigure need no PSM, the register helper (re)creates
    // it, config / sec-level / sec-info act on the registered PSM, deregister tears
    // it down again. Every guard returns BT_BAD_PARAM, so each EXPECT is tied to the
    // branch named by its inline comment; reordering the production checks would
    // silently shift an expectation onto a different branch.
    EattCheckConnReqGuards();
    EattCheckReconfigReqGuards();
    EattCheckRegisterServiceGuards();
    EattCheckSetServiceConfig();
    EattCheckSetSecLevel();
    EattCheckSetSecurityInfo();

    // Cleanup: deregister the PSM (no channel holds it).
    EXPECT_EQ(CallL2cap(EattDeregisterServiceFn, &deregArg), BT_SUCCESS);
    // Re-sync the mock flag with the actual registration state: the deregister above
    // does not touch g_mockRegistered, and a stale true (left over from a previous
    // test's EattAttServiceGuard) would make EnsureMockEattService() skip the
    // registration for every following EATT test (initiator-side 0x17 would then
    // fail with BT_BAD_PARAM, L2capLeEattValidateConnParams).
    g_mockRegistered = false;
}

/**
 * @tc.number: StackGapLe52_EattRejectWithoutEncryption_00100
 * @tc.name: EATT 0x17 on an unencrypted link is rejected with 0x0008
 * @tc.desc: connect to the peer, register the mock EATT service, send one 0x17
 *           batch of 2 channels before pairing. The responder's security check
 *           (link not encrypted) must reject the whole batch with
 *           L2CAP_LE_INSUFFICIENT_ENCRYPTION (0x0008), attempted=2, succeeded=0,
 *           and no connected notification. Skipped when BT52_PEER_ADDR is unset.
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_EattRejectWithoutEncryption_00100, TestSize.Level1)
{
    if (g_peerAddrArg[0] == '\0') {
        GTEST_SKIP() << "peer address not set (BT52_PEER_ADDR=XX:XX:XX:XX:XX:XX)";
    }

    // The link must be unencrypted for the 0x0008 rejection: drop any link left
    // over from a previous test (pairing and the EATT channels are tied to it).
    ResetConnection();
    uint16_t connHandle = 0;
    if (!ConnectToPeer(connHandle)) {
        GTEST_SKIP() << "peer not connected";
    }
    ASSERT_TRUE(EnsureMockEattService());

    EattConnReqArg arg = { };
    arg.addr = g_peerAddr;
    arg.mtu = EATT_TEST_MTU;
    arg.mps = EATT_TEST_MPS;
    arg.credit = EATT_TEST_CREDIT;
    arg.n = 2;
    g_mockCtx.connRsp.waiter.Reset();
    g_mockCtx.connected.Reset();
    ASSERT_EQ(CallL2cap(EattConnReqFn, &arg), BT_SUCCESS);
    ASSERT_TRUE(g_mockCtx.connRsp.waiter.Wait(EATT_WAIT_TIMEOUT_MS)) << "no 0x18 response";

    EXPECT_EQ(g_mockCtx.connRsp.result, L2CAP_LE_INSUFFICIENT_ENCRYPTION);
    EXPECT_EQ(g_mockCtx.connRsp.attempted, 2);
    EXPECT_EQ(g_mockCtx.connRsp.succeeded, 0);
    EXPECT_EQ(g_mockCtx.connected.Count(), 0);
}

/**
 * @tc.number: StackGapLe52_EattEstablishAfterPairing_00100
 * @tc.name: EATT channels established over an encrypted link
 * @tc.desc: connect + legacy Just Works pairing + one 0x17 batch of 2 channels.
 *           The 0x18 must carry L2CAP_LE_CONNECTION_SUCCESSFUL (0x0000) with
 *           attempted/succeeded = 2, and one connected notification per channel
 *           with distinct local CIDs. Skipped when BT52_PEER_ADDR is unset.
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_EattEstablishAfterPairing_00100, TestSize.Level1)
{
    if (g_peerAddrArg[0] == '\0') {
        GTEST_SKIP() << "peer address not set (BT52_PEER_ADDR=XX:XX:XX:XX:XX:XX)";
    }

    ResetConnection();
    uint16_t lcids[2] = { 0, 0 };
    ASSERT_TRUE(EstablishEattChannels(lcids, 2)) << "EATT channels not established";

    EXPECT_EQ(g_mockCtx.connRsp.result, L2CAP_LE_CONNECTION_SUCCESSFUL);
    EXPECT_EQ(g_mockCtx.connRsp.attempted, 2);
    EXPECT_EQ(g_mockCtx.connRsp.succeeded, 2);
    EXPECT_EQ(g_mockCtx.connected.Count(), 2);
    EXPECT_NE(lcids[0], 0);
    EXPECT_NE(lcids[1], 0);
    EXPECT_NE(lcids[0], lcids[1]);
}

/**
 * @tc.number: StackGapLe52_EattReconfigureAfterEstablish_00100
 * @tc.name: EATT 0x19 reconfigure after channels are established
 * @tc.desc: establish 2 channels, then reconfigure to mtu=512/mps=251. Requested
 *           mtu is not below the current 247 (no MTU_REDUCTION) and n=2 with mps
 *           251 is not below the current 251 (no MPS_REDUCTION), so the responder
 *           accepts; each channel reports the effective ATT_MTU =
 *           min(requested 512, peer local config 128 = SERVER_MTU) = 128.
 *           Skipped when BT52_PEER_ADDR is unset.
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_EattReconfigureAfterEstablish_00100, TestSize.Level1)
{
    if (g_peerAddrArg[0] == '\0') {
        GTEST_SKIP() << "peer address not set (BT52_PEER_ADDR=XX:XX:XX:XX:XX:XX)";
    }

    ResetConnection();
    uint16_t lcids[2] = { 0, 0 };
    ASSERT_TRUE(EstablishEattChannels(lcids, 2)) << "EATT channels not established";

    g_mockCtx.reconfig.Reset();
    EattReconfigArg reconfigArg = { };
    reconfigArg.n = 2;
    reconfigArg.mtu = 512;
    reconfigArg.mps = 251;
    reconfigArg.lcids[0] = lcids[0];
    reconfigArg.lcids[1] = lcids[1];
    ASSERT_EQ(CallL2cap(EattReconfigFn, &reconfigArg), BT_SUCCESS);
    ASSERT_TRUE(g_mockCtx.reconfig.WaitFor(2)) << "no reconfigured notification";

    EXPECT_EQ(g_mockCtx.reconfig.count, 2);
    EXPECT_EQ(g_mockCtx.reconfig.values[0], SERVER_MTU);
    EXPECT_EQ(g_mockCtx.reconfig.values[1], SERVER_MTU);
    EXPECT_EQ(g_mockCtx.reconfig.results[0], L2CAP_LE_RECONFIGURE_SUCCESS);
    EXPECT_EQ(g_mockCtx.reconfig.results[1], L2CAP_LE_RECONFIGURE_SUCCESS);
}

/**
 * @tc.number: StackGapLe52_EattReconfigureFailed_00100
 * @tc.name: 0x1A failure result reported per channel with the unchanged effective ATT_MTU
 * @tc.desc: establish 2 channels, then send a valid 0x19 reconfigure (mtu=512/mps=251) and
 *           complete it with a failure 0x1A in one queue slot. A live peer only answers a valid
 *           0x19 with success, so the failure branch is injected via the reconfigList. Each
 *           channel must report recvLeEattReconfigured with the Table 4.22 result code and the
 *           unchanged effective ATT_MTU (old config stays in effect). Skipped when BT52_PEER_ADDR
 *           is unset.
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_EattReconfigureFailed_00100, TestSize.Level1)
{
    if (g_peerAddrArg[0] == '\0') {
        GTEST_SKIP() << "peer address not set (BT52_PEER_ADDR=XX:XX:XX:XX:XX:XX)";
    }

    ResetConnection();
    uint16_t lcids[2] = { 0, 0 };
    ASSERT_TRUE(EstablishEattChannels(lcids, 2)) << "EATT channels not established";
    uint16_t connHandle = g_connCtx.handle;

    // send a valid 0x19 and complete it with a failure 0x1A in the same queue slot, atomic w.r.t.
    // the peer's real 0x1A (which would carry success)
    g_mockCtx.reconfig.Reset();
    EattReconfigInjectArg inject = { };
    inject.aclHandle = connHandle;
    inject.n = 2;
    inject.mtu = 512;
    inject.mps = 251;
    inject.lcids[0] = lcids[0];
    inject.lcids[1] = lcids[1];
    inject.result = L2CAP_LE_RECONFIG_OTHER;
    ASSERT_EQ(CallL2cap(EattReconfigInjectFn, &inject), BT_SUCCESS);
    ASSERT_TRUE(g_mockCtx.reconfig.WaitFor(2)) << "no reconfigured notification";

    EXPECT_EQ(g_mockCtx.reconfig.count, 2);
    EXPECT_EQ(g_mockCtx.reconfig.results[0], L2CAP_LE_RECONFIG_OTHER);
    EXPECT_EQ(g_mockCtx.reconfig.results[1], L2CAP_LE_RECONFIG_OTHER);
    EXPECT_EQ(g_mockCtx.reconfig.values[0], SERVER_MTU);
    EXPECT_EQ(g_mockCtx.reconfig.values[1], SERVER_MTU);
}

/**
 * @tc.number: StackGapLe52_EattConnectionParamsRefresh_00100
 * @tc.name: slave collision retry params refreshed on LE Connection Update Complete
 * @tc.desc: the Vol 3 Part G 5.4 retry delay is computed from the current negotiated
 *           connInterval/connSlaveLatency. After establishment the stored values equal the
 *           requested ones; drive a real LE Connection Update Complete HCI event granting a
 *           different interval/latency through the actual HCI event dispatch and read back the
 *           refreshed values. Skipped when BT52_PEER_ADDR is unset.
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_EattConnectionParamsRefresh_00100, TestSize.Level1)
{
    if (g_peerAddrArg[0] == '\0') {
        GTEST_SKIP() << "peer address not set (BT52_PEER_ADDR=XX:XX:XX:XX:XX:XX)";
    }

    ResetConnection();
    uint16_t lcids[2] = { 0, 0 };
    ASSERT_TRUE(EstablishEattChannels(lcids, 2)) << "EATT channels not established";
    uint16_t connHandle = g_connCtx.handle;

    // initial values: the requested ones captured at connect time
    EattGetConnParamsArg before = { };
    before.aclHandle = connHandle;
    ASSERT_EQ(CallL2cap(EattGetConnParamsFn, &before), BT_SUCCESS);
    EXPECT_EQ(before.interval, EATT_CONN_INTERVAL_MAX);
    EXPECT_EQ(before.latency, EATT_CONN_LATENCY);

    // a granted parameter update (interval/latency larger than the request) must refresh the
    // values the 5.4 retry delay is computed from
    EattConnUpdateArg update = { };
    update.aclHandle = connHandle;
    update.interval = EATT_CONN_INTERVAL_MAX * 2;
    update.latency = EATT_CONN_LATENCY + 2;
    update.timeout = 0x01F4;
    ASSERT_EQ(CallL2cap(EattConnUpdateFn, &update), BT_SUCCESS);

    EattGetConnParamsArg after = { };
    after.aclHandle = connHandle;
    ASSERT_EQ(CallL2cap(EattGetConnParamsFn, &after), BT_SUCCESS);
    EXPECT_EQ(after.interval, EATT_CONN_INTERVAL_MAX * 2);
    EXPECT_EQ(after.latency, EATT_CONN_LATENCY + 2);
}

/**
 * @tc.number: StackGapLe52_EattDataPlane_00100
 * @tc.name: EATT SDU send and echo over an established channel
 * @tc.desc: establish 2 channels, send a small SDU on channel 0. The peer echoes
 *           the SDU back on the same channel; the data callback must report the
 *           same lcid, the full payload length and identical bytes.
 *           Skipped when BT52_PEER_ADDR is unset.
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_EattDataPlane_00100, TestSize.Level1)
{
    if (g_peerAddrArg[0] == '\0') {
        GTEST_SKIP() << "peer address not set (BT52_PEER_ADDR=XX:XX:XX:XX:XX:XX)";
    }

    ResetConnection();
    uint16_t lcids[2] = { 0, 0 };
    ASSERT_TRUE(EstablishEattChannels(lcids, 2)) << "EATT channels not established";

    const uint8_t payload[] = { 'E', 'A', 'T', 'T', '5', '2' };
    Packet *pkt = PacketMalloc(0, 0, sizeof(payload));
    ASSERT_NE(pkt, nullptr);
    (void)PacketPayloadWrite(pkt, payload, 0, sizeof(payload));

    EattSendDataArg sendArg = { };
    sendArg.lcid = lcids[0];
    sendArg.pkt = pkt;
    g_mockCtx.data.waiter.Reset();
    int sendRet = CallL2cap(EattSendDataFn, &sendArg);
    // L2CAP_LeSendData shares the buffer (PacketInheritMalloc) but never takes ownership of pkt,
    // so the caller-owned packet must be freed on both the failure and the success path; the echo
    // already proves the TX side copied/flushed the buffer, freeing here is safe
    PacketFree(pkt);
    ASSERT_EQ(sendRet, BT_SUCCESS);

    ASSERT_TRUE(g_mockCtx.data.waiter.Wait(EATT_WAIT_TIMEOUT_MS)) << "no echoed data";
    EXPECT_EQ(g_mockCtx.data.lcid, lcids[0]);
    EXPECT_EQ(g_mockCtx.data.dataLen, sizeof(payload));
    EXPECT_EQ(std::memcmp(g_mockCtx.data.data, payload, sizeof(payload)), 0);
}

/**
 * @tc.number: StackGapLe52_EattRequestTimeout_00100
 * @tc.name: 0x17 batch whose response never arrives is cleaned up on RTX timeout
 * @tc.desc: connect to the peer, register the mock EATT service, send one 0x17
 *           batch of 3 channels and expire its pending request synchronously on
 *           the L2CAP queue, before the peer's 0x18 is processed (a live peer
 *           answers within milliseconds, so the real 30 s RTX cannot be waited).
 *           The timeout cleanup must report leDisconnectAbnormal exactly once per
 *           batch channel and free them; the deferred 0x18 must find no pending
 *           request and be ignored without a crash. Skipped when BT52_PEER_ADDR
 *           is unset.
 */
HWTEST_F(StackGapLe52Test, StackGapLe52_EattRequestTimeout_00100, TestSize.Level1)
{
    if (g_peerAddrArg[0] == '\0') {
        GTEST_SKIP() << "peer address not set (BT52_PEER_ADDR=XX:XX:XX:XX:XX:XX)";
    }

    ResetConnection();
    uint16_t connHandle = 0;
    if (!ConnectToPeer(connHandle)) {
        GTEST_SKIP() << "peer not connected";
    }
    ASSERT_TRUE(EnsureMockEattService());

    EattReqTimeoutArg arg = { };
    arg.addr = g_peerAddr;
    arg.handle = connHandle;
    arg.mtu = EATT_TEST_MTU;
    arg.mps = EATT_TEST_MPS;
    arg.credit = EATT_TEST_CREDIT;
    arg.n = 3;
    g_mockCtx.disconnected.Reset();
    // the composite (send 0x17 + expire) runs in one queue slot, so it is atomic
    // w.r.t. the peer's 0x18 and every batch channel must be reported exactly once
    ASSERT_EQ(CallL2cap(EattReqTimeoutFn, &arg), BT_SUCCESS);
    ASSERT_TRUE(g_mockCtx.disconnected.WaitFor(3)) << "0x17 timeout cleanup incomplete";
    EXPECT_EQ(g_mockCtx.disconnected.Count(), 3);
}

} // namespace Bluetooth
} // namespace OHOS
