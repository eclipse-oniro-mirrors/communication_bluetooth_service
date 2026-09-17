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

#ifndef STACK_GAP_LE_6_0_INTERACT_TEST_H
#define STACK_GAP_LE_6_0_INTERACT_TEST_H

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <thread>

#include <gtest/gtest.h>

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

namespace OHOS {
namespace Bluetooth {

// on scope exit; the "cs-reflector" peer role of stack_gap_le_interact_test.cpp uses the
// same pair.
void InitPairResponder();
void ShutdownPairResponder();

namespace StackGapLe60InteractTestInternal {


constexpr uint32_t WAIT_CALLBACK_TIMEOUT_MS = 10000;
// Drain window that proves no 0x2B follows the 0x04 on a bit63-clear controller.
constexpr uint32_t NO_ALL_FEATURES_QUIET_MS = 1000;
// Drain window that lets in-flight HCI events settle before tearing down the controller.
constexpr uint32_t TEARDOWN_DRAIN_MS = 100;
// A hex digit occupies a nibble, so the high digit shifts left by four bits.
constexpr uint32_t HEX_DIGIT_SHIFT = 4;
constexpr uint8_t HCI_STATUS_SUCCESS = 0x00;
// The three Controller error codes the CS rejection cases assert against. Written out here,
// as HCI_STATUS_SUCCESS above is, instead of including the stack's own hci_error.h: 0x0C
// is Command Disallowed, 0x12 Invalid HCI Command Parameters, 0x2F Insufficient Security.
constexpr uint8_t HCI_STATUS_COMMAND_DISALLOWED = 0x0C;
constexpr uint8_t HCI_STATUS_INVALID_HCI_COMMAND_PARAMETERS = 0x12;
constexpr uint8_t HCI_STATUS_INSUFFICIENT_SECURITY = 0x2F;
// Peer address, read from env BT60_PEER_ADDR. Optional "public:" / "random:"
// prefix (7 chars) + "XX:XX:XX:XX:XX:XX" (17 chars) + NUL.
constexpr size_t BT60_ADDR_ARG_CAP = 32;
constexpr size_t BT60_ADDR_STRING_LEN = 17;
constexpr size_t BT60_ADDR_PREFIX_LEN = 7;
constexpr size_t BT60_ADDR_SEPARATOR_STEP = 3;

// Synchronization contract for every capture struct that inherits this waiter: the
// callback (Stack thread) writes the payload fields first and calls Notify()
// afterwards, with no lock of its own - the lock inside Notify() and the matching
// lock inside Wait() are what carry the happens-before. A reader may therefore touch
// the payload fields only after Wait() returns true; reading them without a completed
// Wait() is a data race. Captures that need concurrent field access instead (e.g.
// ReadFeaturesCapture) take their own lock on every field.
struct CallbackWaiter {
    std::mutex mtx;
    std::condition_variable cv;
    bool received = false;

    bool Wait(uint32_t timeoutMs = WAIT_CALLBACK_TIMEOUT_MS)
    {
        std::unique_lock<std::mutex> lock(mtx);
        return cv.wait_for(lock, std::chrono::milliseconds(timeoutMs), [this] {
            return received;
        });
    }

    void Reset()
    {
        std::lock_guard<std::mutex> lock(mtx);
        received = false;
    }

    void Notify()
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            received = true;
        }
        cv.notify_all();
    }
};

// LE ACL connection state, filled by the callbacks registered through g_aclCallbacks.
struct LeConnContext : CallbackWaiter {
    uint8_t status = 0xFF;
    uint16_t handle = 0xFFFF;
};

// Shared capture for the two HCI read-completion slots. The event handlers hand
// a zero-copy pointer valid only for the callback duration, so each handler
// stores a full struct copy. Per-slot flags (not one shared received) let the
// bit63-clear branch prove the 0x2B slot never fired after the 0x04 did.
struct ReadFeaturesCapture {
    std::mutex mtx;
    std::condition_variable cv;
    bool legacyReceived = false;
    bool allReceived = false;
    HciLeReadRemoteFeaturesCompleteEventParam legacyParam = {};
    HciLeReadAllRemoteFeaturesCompleteEventParam allParam = {};

    bool WaitForAny(uint32_t timeoutMs)
    {
        std::unique_lock<std::mutex> lock(mtx);
        return cv.wait_for(lock, std::chrono::milliseconds(timeoutMs), [this] {
            return legacyReceived || allReceived;
        });
    }

    void Reset()
    {
        std::lock_guard<std::mutex> lock(mtx);
        legacyReceived = false;
        allReceived = false;
    }

    bool LegacyReceived()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return legacyReceived;
    }

    bool AllFeaturesReceived()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return allReceived;
    }

    HciLeReadRemoteFeaturesCompleteEventParam LegacyParam()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return legacyParam;
    }

    HciLeReadAllRemoteFeaturesCompleteEventParam AllParam()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return allParam;
    }

    void OnLegacy(const HciLeReadRemoteFeaturesCompleteEventParam *eventParam)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            legacyParam = *eventParam;
            legacyReceived = true;
        }
        cv.notify_all();
    }

    void OnAll(const HciLeReadAllRemoteFeaturesCompleteEventParam *eventParam)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            allParam = *eventParam;
            allReceived = true;
        }
        cv.notify_all();
    }
};

// Two-device state, all anonymous-namespace internal linkage so the same names
// used by the interact / 5.2 translation units cannot collide at link time.
extern char g_peerAddrArg[BT60_ADDR_ARG_CAP];
extern BtAddr g_peerAddr;
extern bool g_peerAddrValid;
extern LeConnContext g_connCtx;
extern bool g_connected;
extern bool g_btmInitialized;
extern bool g_btmEnabled;
extern bool g_aclCallbacksRegistered;
extern bool g_hciCallbacksRegistered;

extern ReadFeaturesCapture g_readFeaturesCapture;

// Copy BT60_PEER_ADDR into g_peerAddrArg; an over-long value leaves the buffer
// empty and ParsePeerAddr rejects it either way. Fixed array: a std::string
// global would violate the no-non-POD-global red line (rule 1).
void InitPeerAddrArg();

// BtAddr.addr[] is little-endian (addr[5] printed first). Accept an optional
// "public:" / "random:" prefix followed by the conventional "11:22:33:44:55:66"
// form, with the leftmost ASCII byte stored into addr[BT_ADDRESS_SIZE - 1].
bool ParsePeerAddr(const char *str, BtAddr *addr);

// ---- HCI read-completion capture (handlers must not block or (de)register) ----
void OnLegacyReadRemoteFeaturesComplete(const HciLeReadRemoteFeaturesCompleteEventParam *eventParam);

void OnAllReadRemoteFeaturesComplete(const HciLeReadAllRemoteFeaturesCompleteEventParam *eventParam);

// HCI_RegisterEventCallbacks stores the struct pointer; it must be static.
extern HciEventCallbacks g_hciReadCallbacks;

void OnLeConnectionComplete(uint8_t status, uint16_t connectionHandle, const BtAddr *addr, uint8_t role,
    void *context);

void OnLeDisconnectionComplete(uint8_t status, uint16_t connectionHandle, uint8_t reason, void *context);

// BTM_RegisterAclCallbacks stores the struct pointer; it must be static.
extern BtmAclCallbacks g_aclCallbacks;

// Establish a real LE connection to the peer and return the actual connection handle.
bool ConnectToPeer(uint16_t &connHandle);

// Tear the link down; L2CIF_LeDisconnect is void, so wait for the ACL
// disconnection-complete through the same g_connCtx waiter as the connect.
void DisconnectFromPeer();

// Page 0 = octets 0..7 of the reported LE_Features.
bool Page0NonZero(const uint8_t *leFeaturesRaw);

// ---- BT 6.0 DBAF dual-board cases (00200 decision connect / 00300 scan) -----
// The peer role is "dbaf-server" in stack_gap_le_interact_test.cpp: handle 0 is a
// plain connectable set (AdvData with the "BTPEER" tail marker, SID 0) and handle 2
// a connectable decision set (ADV_DECISION_IND, SID 2) carrying decision data "bt".
// Both sets share one public address, and the extended report Event_Type has no DBAF
// indicator bit (7.7.65,13, p2353), so the scan classifier below tells them apart by
// the marker: the plain set's report data contains it and a decision report (whose
// data is the raw decision payload, if any) never can. The decision-test vector
// mirrors spec Example 3 test 4 (7.8.145, p2864): Test_Field 0x12 = Arbitrary Data of
// exactly 2 octets, Mask in parameters[0..7], Target in parameters[8..15],
// zero-padded; flags 0b0011 starts a group and passes on check.
constexpr uint32_t DBAF_SCAN_WINDOW_MS = 2000;
constexpr uint32_t DBAF_NO_CONNECT_WINDOW_MS = 3000;
constexpr uint32_t DBAF_PEER_SETTLE_MS = 1000;
constexpr uint8_t DBAF_INSTR_FIELD_EXACT2 = 0x12;
constexpr uint8_t DBAF_INSTR_PARAM_MASK0 = 0;
constexpr uint8_t DBAF_INSTR_PARAM_MASK1 = 1;
constexpr uint8_t DBAF_INSTR_PARAM_TARGET0 = 8;
constexpr uint8_t DBAF_INSTR_PARAM_TARGET1 = 9;
constexpr uint8_t DBAF_PLAIN_MARKER[] = { 'B', 'T', 'P', 'E', 'E', 'R' };
constexpr size_t DBAF_PLAIN_MARKER_LEN = sizeof(DBAF_PLAIN_MARKER);

// Decision-test result + scan report state, shared with the GAP thread callbacks.
struct DbafScanCtx {
    CallbackWaiter paramWaiter;
    uint8_t paramStatus = 0xFF;
    CallbackWaiter enableWaiter;
    uint8_t enableStatus = 0xFF;
    CallbackWaiter instrWaiter;
    uint8_t instrStatus = 0xFF;
    std::atomic<int> connReports = 0;      // peer plain set (marker present)
    std::atomic<int> decisionReports = 0;  // peer decision set (marker absent)
    std::atomic<bool> scanEnabled = false; // reflects the controller scan state
};

void OnDbafScanSetParamResult(uint8_t status, void *context);

void OnDbafScanSetEnableResult(uint8_t status, void *context);

// 0x0081 LE Set Decision Instructions completes through the extended-scan
// callback (gap_le_scan.c GapLeSetDecisionInstructionsComplete).
void OnDbafDecisionInstructionsResult(uint8_t status, void *context);

// True when the report data carries the plain-set "BTPEER" marker.
bool DbafReportHasMarker(const uint8_t *data, uint8_t dataLen);

// Count peer reports by the plain-set marker. Runs on the GAP thread with the data
// valid only for the callback duration, so the marker test happens in place and only
// the counters are shared (atomic).
void OnDbafExAdvReport(uint8_t advType, const BtAddr *addr, GapExAdvReportParam reportParam,
    const BtAddr *currentAddr, void *context);

bool DbafStopScan(DbafScanCtx *ctx);  // defined below; used by the guard's destructor

// Stops any scan and deregisters the extended-scan callback on scope exit so no
// late report can reach a destroyed context (deregister only after a stop).
class DbafScanCallbackGuard;

bool DbafRegisterScanCallback(DbafScanCtx *ctx, DbafScanCallbackGuard &guard);

// Install one decision test for Arbitrary Data of exactly 2 octets and wait for
// its command-complete. mask/target are the 2 used octets of the 8-octet fields.
bool DbafInstallArbitrary2(DbafScanCtx *ctx, uint8_t mask0, uint8_t mask1, uint8_t target0, uint8_t target1);

bool DbafStopScan(DbafScanCtx *ctx);

bool DbafStartScan(DbafScanCtx *ctx);

bool DbafSetScanMode(DbafScanCtx *ctx, uint8_t scanFilterPolicy);

struct DbafSweepResult {
    int conn = 0;
    int decision = 0;
};

// Scan for windowMs under the given decision mode (0x0041 bits 2-3) and snapshot
// the per-set report counts. A mode change needs a scan stop in between.
bool DbafRunScanSweep(DbafScanCtx *ctx, uint8_t mode, uint32_t windowMs, DbafSweepResult &out);

enum DbafGate {
    K_DBAF_READY = 0,
    K_NO_PEER_ADDR,
    K_LOCAL_NO_DBAF,
    K_PEER_CONNECT_FAILED,
    K_PEER_READ_FAILED,
    K_PEER_NO_DBAF,
};

const char *DbafGateSkipMessage(DbafGate gate);

// Probe both boards for DBAF support and report the outcome as a skip gate. No
// ASSERT is used here: the case maps the enum onto a visible GTEST_SKIP instead.
DbafGate DbafCheckCapability();

// Issue a decision-based connect (7.8.66) and wait for a successful connection.
bool DbafDecisionConnectSucceed(uint8_t policy);

// Drain window after a decision connect whose instruction cannot match. Returns 0
// when no completion arrived (initiator still pending), 1 when it ended with an
// error-status connection-complete, and -1 when a success unexpectedly completed.
int DbafDrainNoConnectWindow(uint32_t windowMs);
}  // namespace StackGapLe60InteractTestInternal

using namespace StackGapLe60InteractTestInternal;

class StackGapLe60InteractTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp() override {}
    void TearDown() override
    {
        DisconnectFromPeer();
    }
};
}  // namespace Bluetooth
}  // namespace OHOS

#endif
