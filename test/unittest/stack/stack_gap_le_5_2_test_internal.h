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

/**
 * @file stack_gap_le_5_2_test_internal.h
 *
 * @brief Shared declarations of the BT 5.2 stack tests
 *
 * The two test translation units (stack_gap_le_5_2_test.cpp and
 * stack_att_52_test.cpp) run in one binary: the EATT / two-device helpers, the
 * mock EATT service state and the shared waiters and constants live here so the
 * split keeps both files below the source-file line limit.
 */

#ifndef STACK_GAP_LE_5_2_TEST_INTERNAL_H
#define STACK_GAP_LE_5_2_TEST_INTERNAL_H

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <string>

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

namespace OHOS {
namespace Bluetooth {

// BT 5.2 spec, Vol 2, Part E, §7.8.115
//   Bit_Number (1 byte)  : 0x00-0x3F, FeatureSet bit index
//   Bit_Value  (1 byte)  : 0x00 or 0x01
//   Only bit 32 (Isochronous Channels (Host Support)) is Host Controlled (Vol 6 Part B, Table 4.6).
constexpr uint8_t HOST_FEATURE_BIT_ISO_HOST_SUPPORT = 0x20;
constexpr uint8_t HOST_FEATURE_BIT_VALUE_SUPPORT = 0x01;
constexpr uint8_t HOST_FEATURE_BIT_VALUE_NOT_SUPPORT = 0x00;

constexpr int WAIT_CALLBACK_TIMEOUT_MS = 5000;
constexpr uint8_t HCI_STATUS_SUCCESS = 0x00;
constexpr uint8_t HCI_STATUS_UNKNOWN_COMMAND = 0x01; // Unknown HCI Command (Vol 1 Part F, 2.1)

// EATT over-the-air exchanges (connect + pair + 0x17/0x18) take longer than the
// single-device cases, so the EATT callbacks scale the timeout up to 10 seconds.
constexpr uint32_t EATT_WAIT_TIMEOUT_MS = 10000;
constexpr uint8_t IO_CAP_NO_INPUT_NO_OUTPUT = 0x03;
constexpr uint8_t PAIR_STATUS_SUCCESS = 0x00; // SMP_PAIR_STATUS_SUCCESS, src/smp/smp.h
// EATT limit constants are kept local here rather than exported from l2cap_le.h,
// following the 5.0/5.1 test convention (e.g. HCI_STATUS_SUCCESS duplicated next to
// the production HCI_SUCCESS): a .c-side #define is not moved into a header just so
// a test can reference it. Keep the values in sync with l2cap_le.c's
// L2CAP_LE_EATT_{MAX_CHANNEL,MIN_MTU,MAX_MPS}.
constexpr uint16_t EATT_MIN_MTU = 64;
constexpr uint16_t EATT_MAX_MPS = 65533;
constexpr uint8_t EATT_MAX_CHANNEL = 0x05;
constexpr uint16_t EATT_TEST_MTU = 247;  // local receive MTU of the 0x17 batch
constexpr uint16_t EATT_TEST_MPS = 251;  // local receive MPS of the 0x17 batch
constexpr uint16_t EATT_TEST_CREDIT = 8; // initial credits of the 0x17 batch
constexpr uint16_t SERVER_MTU = 128;     // responder default local MTU (0x18)
// Connection parameters ConnectToPeer requests for the LE link (interval max in 1.25 ms units,
// slave latency in connection events). The Vol 3 Part G 5.4 retry delay is seeded from these
// requested values and refreshed from LE Connection Update Complete.
constexpr uint16_t EATT_CONN_INTERVAL_MAX = 0x0018;
constexpr uint16_t EATT_CONN_LATENCY = 0x0000;

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

// LE ACL connection state of the two-device tests, filled by the callbacks
// registered through g_aclCallbacks.
struct LeConnContext : CallbackWaiter {
    uint8_t status = 0xFF;
    uint16_t handle = 0xFFFF;
    BtAddr peerAddr = { };
    uint8_t role = 0xFF;
};

// The EATT public APIs are L2CAP state mutations. In the stack the L2CIF_ wrappers
// run them on the L2CAP processing queue (PROCESSING_QUEUE_ID_LA2CAP), where the
// inbound-signal processing also runs. The EATT wrapper layer is not implemented
// yet (BT 5.2 plan chapter 4), so the tests post the calls themselves through
// L2capAsynchronousProcess to stay serialized with the signal processing.
using L2capFn = int (*)(void *arg);

struct EattConnReqArg {
    BtAddr addr = { };
    bool nullAddr = false;  // pass NULL for addr to exercise the guard
    bool nullLcids = false; // pass NULL for lcids to exercise the guard
    bool nullCfg = false;   // pass NULL for cfg to exercise the guard
    uint16_t mtu = 0;
    uint16_t mps = 0;
    uint16_t credit = 0;
    uint16_t lcids[EATT_MAX_CHANNEL] = { 0 };
    uint16_t n = 0;
};

struct EattReconfigArg {
    uint16_t lcids[EATT_MAX_CHANNEL] = { 0 };
    bool nullLcids = false; // pass NULL for lcids to exercise the guard
    uint16_t n = 0;
    uint16_t mtu = 0;
    uint16_t mps = 0;
};

// Send a 0x17 batch, then synchronously expire its pending request on the same
// L2CAP-queue thread, before the peer's 0x18 is processed (a live peer answers a
// valid 0x17 within milliseconds, so the real 30 s RTX cannot be waited in a
// two-device test). The composite runs in ONE CallL2cap slot, so the expire is
// atomic w.r.t. the peer response; it drives the same L2capLeEattDiscardTimedOutBatch
// path the RTX timer would take.
struct EattReqTimeoutArg {
    BtAddr addr = { };
    uint16_t handle = 0;
    uint16_t mtu = 0;
    uint16_t mps = 0;
    uint16_t credit = 0;
    uint16_t lcids[EATT_MAX_CHANNEL] = { 0 };
    uint16_t n = 0;
};

// Send a valid 0x19 reconfigure request, then complete it with a failure 0x1A in the same
// L2CAP-queue slot. A live peer only ever answers a valid 0x19 with success (the initiator
// refuses MTU/MPS reductions locally before sending), so the failure branch is unreachable
// over the air; the composite drives the real 0x1A processing path with the injected result
// while the pending request is still the first node of the reconfigList.
struct EattReconfigInjectArg {
    uint16_t aclHandle = 0;
    uint16_t lcids[EATT_MAX_CHANNEL] = { 0 };
    uint16_t n = 0;
    uint16_t mtu = 0;
    uint16_t mps = 0;
    uint16_t result = 0; // Table 4.22 result code delivered in the 0x1A
};

// Read the connection parameters that drive the Vol 3 Part G 5.4 slave collision retry on the
// L2CAP-queue thread (the HCI event handler writes them on that same thread, so reading from the
// test thread would race).
struct EattGetConnParamsArg {
    uint16_t aclHandle = 0;
    uint16_t interval = 0;
    uint16_t latency = 0;
};

// Deliver a real LE Connection Update Complete HCI event (0x3E LE Meta, subevent 0x03) for the
// connection, on the L2CAP-queue thread, so the actual .leConnectionUpdateComplete callback
// registration and the event parsing path are exercised.
struct EattConnUpdateArg {
    uint16_t aclHandle = 0;
    uint16_t interval = 0;
    uint16_t latency = 0;
    uint16_t timeout = 0;
};

struct EattSetServiceConfigArg {
    uint16_t lpsm = 0;
    L2capLeConfigInfo cfg = { };
    bool nullCfg = false; // pass NULL for cfg to exercise the guard
};

struct EattSetSecLevelArg {
    uint16_t lpsm = 0;
    uint8_t secRequirement = 0;
};

struct EattSetSecInfoArg {
    BtAddr addr = { };
    bool nullAddr = false; // pass NULL for addr to exercise the guard
    uint8_t keySize = 0;
    uint8_t authLevel = 0;
    uint8_t authzGranted = 0;
};

struct EattRegisterServiceArg {
    uint16_t lpsm = 0;
    const L2capLeService *svc = nullptr;
    void *ctx = nullptr;
};

struct EattDeregisterServiceArg {
    uint16_t lpsm = 0;
};

struct EattSendDataArg {
    uint16_t lcid = 0;
    Packet *pkt = nullptr;
};

// mock EATT service callbacks
struct EattConnRspResult {
    CallbackWaiter waiter;
    uint16_t result = 0xFFFF;
    uint8_t attempted = 0;
    uint8_t succeeded = 0;
};

// recvLeEattConnected (once per established channel) / recvLeEattReconfigured
struct EattMultiResult {
    std::mutex mtx;
    std::condition_variable cv;
    uint16_t count = 0;
    uint16_t values[EATT_MAX_CHANNEL] = { 0 };
    uint16_t results[EATT_MAX_CHANNEL] = { 0 };

    void Add(uint16_t value, uint16_t result = 0)
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (count < EATT_MAX_CHANNEL) {
            values[count] = value;
            results[count] = result;
            count++;
        }
        cv.notify_all();
    }

    bool WaitFor(uint16_t target)
    {
        std::unique_lock<std::mutex> lock(mtx);
        return cv.wait_for(lock, std::chrono::milliseconds(EATT_WAIT_TIMEOUT_MS), [this, target] {
            return count >= target;
        });
    }

    void Reset()
    {
        std::lock_guard<std::mutex> lock(mtx);
        count = 0;
    }

    // Locked read: Add runs on the L2CAP queue (mock service callbacks) while assertions run
    // on the test thread, so reading count directly is a data race (review v3 Minor-4). A
    // WaitFor on the same object orders the write, but a direct EXPECT_EQ on a field does not.
    uint16_t Count()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return count;
    }
};

// recvLeData (data plane)
struct EattDataResult {
    CallbackWaiter waiter;
    uint16_t lcid = 0;
    uint8_t data[256] = { 0 };
    uint16_t dataLen = 0;
};

struct MockEattServiceCtx {
    EattConnRspResult connRsp;
    EattMultiResult connected;
    EattMultiResult reconfig;
    EattMultiResult disconnected; // leDisconnectAbnormal, fired once per channel on 0x17 timeout
    EattDataResult data;
};

// ----- two-device state, defined in stack_gap_le_5_2_test.cpp -----
// BTM_RegisterAclCallbacks stores the struct pointer (not a copy), so it must be
// static, otherwise BTM holds a dangling pointer after this function returns.
extern BtmAclCallbacks g_aclCallbacks;
// Peer address for two-device tests, read from env BT52_PEER_ADDR: the string form
// "XX:XX:XX:XX:XX:XX" is 17 chars plus the NUL terminator. Fixed-size buffer: a
// std::string global would violate the no-non-POD-global red line (rule 1).
extern char g_peerAddrArg[18];
extern BtAddr g_peerAddr;
extern bool g_peerAddrValid;
extern LeConnContext g_connCtx;
extern bool g_connected;

// ----- mock EATT service state, defined in stack_att_52_test.cpp -----
extern MockEattServiceCtx g_mockCtx;
// Callbacks must be wired at definition: a zeroed L2capLeService would silently
// drop every EATT notification.
extern L2capLeService g_mockService;
// Tracks whether the mock EATT service is registered on PSM 0x0027 (EnsureMockEattService
// skips the registration when set). Kept in sync manually: a deregister alone does not
// clear it.
extern bool g_mockRegistered;

// ----- helpers defined in stack_gap_le_5_2_test.cpp -----
bool ParsePeerAddr(const char *str, BtAddr *addr);
bool ConnectToPeer(uint16_t &connHandle);
// Copy BT52_PEER_ADDR into g_peerAddrArg; call before ParsePeerAddr, safe to repeat.
void InitPeerAddrArg();

// ----- helpers defined in stack_att_52_test.cpp -----
int CallL2cap(L2capFn fn, void *arg);
int EattConnReqFn(void *arg);
int EattReconfigFn(void *arg);
int EattReqTimeoutFn(void *arg);
int EattReconfigInjectFn(void *arg);
int EattGetConnParamsFn(void *arg);
int EattConnUpdateFn(void *arg);
int EattSetServiceConfigFn(void *arg);
int EattSetSecLevelFn(void *arg);
int EattSetSecInfoFn(void *arg);
int EattRegisterServiceFn(void *arg);
int EattDeregisterServiceFn(void *arg);
int EattSendDataFn(void *arg);
bool EnsureMockEattService();
bool EstablishEattChannels(uint16_t lcids[], uint16_t n);
void ResetConnection();
void InitPairResponder();
void ShutdownPairResponder();
int InitAttPeerSetup();
int InitEattPeerSetup();

} // namespace Bluetooth
} // namespace OHOS

#endif // STACK_GAP_LE_5_2_TEST_INTERNAL_H
