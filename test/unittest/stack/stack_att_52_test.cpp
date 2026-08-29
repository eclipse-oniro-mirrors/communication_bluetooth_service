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

#include <gtest/gtest.h>

#include <atomic>
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
#include "btm/btm_thread.h"
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
#include "src/l2cap/l2cap_le_test.h"
#include "stack_gap_le_5_2_test_internal.h"

using namespace testing::ext;

namespace OHOS {
namespace Bluetooth {

// ===================== EATT L2CAP layer helpers (plan chapter 3) =====================

// The EATT public APIs are L2CAP state mutations. In the stack the L2CIF_ wrappers
// run them on the L2CAP processing queue (PROCESSING_QUEUE_ID_LA2CAP), where the
// inbound-signal processing also runs. The EATT wrapper layer is not implemented
// yet (BT 5.2 plan chapter 4), so the tests post the calls themselves through
// L2capAsynchronousProcess to stay serialized with the signal processing.
struct L2capCall {
    CallbackWaiter done;
    L2capFn fn = nullptr;
    void *arg = nullptr;
    int result = BT_BAD_STATUS;
    // Ownership flag for the timeout path: touched only inside a done.mtx critical
    // section (OnL2capCall's load / CallL2cap's store), so both sides observe the
    // same total order and exactly one of them frees the object.
    std::atomic<bool> abandoned = false;
};

static void OnL2capCall(const void *context)
{
    L2capCall *call = static_cast<L2capCall *>(const_cast<void *>(context));
    call->result = call->fn(call->arg);
    // Decide ownership and finish the wait inside one done.mtx critical section:
    // the abandoned load is totally ordered against CallL2cap's store on the
    // timeout path, and received + notify inside the same section prove that once
    // CallL2cap observes received == true the callback is fully finished and will
    // not touch the object again (its freeSelf load then necessarily saw false,
    // because the store only happens after Wait has returned false).
    bool freeSelf;
    {
        std::lock_guard<std::mutex> lock(call->done.mtx);
        freeSelf = call->abandoned.load();
        call->done.received = true;
        call->done.cv.notify_all();
    }
    if (freeSelf) {
        delete call;
    }
}

int CallL2cap(L2capFn fn, void *arg)
{
    // Heap-allocate the call: OnL2capCall runs on the L2CAP processing queue and can
    // outlive this frame when the queue is backlogged past the wait budget (Wait
    // returns BT_TIMEOUT and the caller's stack object would already be destroyed).
    // On timeout ownership of the object is handed to the callback via abandoned.
    L2capCall *call = new L2capCall;
    call->fn = fn;
    call->arg = arg;
    if (L2capAsynchronousProcess(OnL2capCall, nullptr, call) != BT_SUCCESS) {
        delete call; // never queued: the callback will not run
        return BT_BAD_STATUS;
    }
    if (call->done.Wait()) {
        int result = call->result;
        delete call; // the callback observed abandoned == false, so it did not free
        return result;
    }
    // Timed out while the call is still queued: take done.mtx to order the
    // ownership decision against the callback's load. If the callback has already
    // run (received == true), it observed abandoned == false and will not free the
    // object, so this side must free it (safe: the callback's last touch happened
    // inside the same critical section). Otherwise the callback will see
    // abandoned == true and free the object itself. The caller must not touch the
    // object after this point.
    bool callbackWillFree;
    {
        std::lock_guard<std::mutex> lock(call->done.mtx);
        call->abandoned.store(true);
        callbackWillFree = !call->done.received;
    }
    if (!callbackWillFree) {
        delete call;
    }
    return BT_TIMEOUT;
}

int EattConnReqFn(void *arg)
{
    EattConnReqArg *a = static_cast<EattConnReqArg *>(arg);
    const BtAddr *addr = a->nullAddr ? nullptr : &a->addr;
    uint16_t *lcids = a->nullLcids ? nullptr : a->lcids;
    L2capLeConfigInfo cfg = { a->mtu, a->mps, a->credit };
    return L2CAP_LeEattConnectionReq(addr, a->nullCfg ? nullptr : &cfg, lcids, a->n);
}

int EattReconfigFn(void *arg)
{
    EattReconfigArg *a = static_cast<EattReconfigArg *>(arg);
    L2capLeEattCidList lcidList = { a->nullLcids ? nullptr : a->lcids, (uint8_t)a->n };
    return L2CAP_LeEattReconfigureReq(&lcidList, a->mtu, a->mps);
}

int EattReqTimeoutFn(void *arg)
{
    EattReqTimeoutArg *a = static_cast<EattReqTimeoutArg *>(arg);
    L2capLeConfigInfo cfg = { a->mtu, a->mps, a->credit };
    int ret = L2CAP_LeEattConnectionReq(&a->addr, &cfg, a->lcids, a->n);
    if (ret != BT_SUCCESS) {
        return ret;
    }
    return L2capLeEattExpirePendingRequest(a->handle);
}

int EattReconfigInjectFn(void *arg)
{
    EattReconfigInjectArg *a = static_cast<EattReconfigInjectArg *>(arg);
    L2capLeEattCidList lcidList = { a->lcids, (uint8_t)a->n };
    int ret = L2CAP_LeEattReconfigureReq(&lcidList, a->mtu, a->mps);
    if (ret != BT_SUCCESS) {
        return ret;
    }
    return L2capLeEattInjectReconfigureRsp(a->aclHandle, a->result);
}

int EattGetConnParamsFn(void *arg)
{
    EattGetConnParamsArg *a = static_cast<EattGetConnParamsArg *>(arg);
    return L2capLeEattGetConnectionParams(a->aclHandle, &a->interval, &a->latency);
}

// Byte offsets of the injected HCI LE Connection Update Complete event (0x3E, subevent 0x03).
constexpr size_t HCI_EVT_CODE_OFFSET = 0;
constexpr size_t HCI_EVT_LENGTH_OFFSET = 1;
constexpr size_t HCI_EVT_SUBEVENT_OFFSET = 2;
constexpr size_t HCI_EVT_STATUS_OFFSET = 3;
constexpr size_t HCI_EVT_ACL_HANDLE_OFFSET = 4;
constexpr size_t HCI_EVT_INTERVAL_OFFSET = 6;
constexpr size_t HCI_EVT_LATENCY_OFFSET = 8;
constexpr size_t HCI_EVT_TIMEOUT_OFFSET = 10;
constexpr size_t HCI_CONN_UPDATE_EVENT_SIZE = 12;

int EattConnUpdateFn(void *arg)
{
    EattConnUpdateArg *a = static_cast<EattConnUpdateArg *>(arg);
    // 0x3E (LE Meta) + length 0x0A + subevent 0x03 + status/handle/interval/latency/timeout
    uint8_t data[HCI_CONN_UPDATE_EVENT_SIZE] = { 0 };
    data[HCI_EVT_CODE_OFFSET] = 0x3E;
    data[HCI_EVT_LENGTH_OFFSET] = 0x0A;
    data[HCI_EVT_SUBEVENT_OFFSET] = 0x03; // HCI_LE_CONNECTION_UPDATE_COMPLETE_EVENT
    data[HCI_EVT_STATUS_OFFSET] = 0x00; // status success
    L2capCpuToLe16(data + HCI_EVT_ACL_HANDLE_OFFSET, a->aclHandle);
    L2capCpuToLe16(data + HCI_EVT_INTERVAL_OFFSET, a->interval);
    L2capCpuToLe16(data + HCI_EVT_LATENCY_OFFSET, a->latency);
    L2capCpuToLe16(data + HCI_EVT_TIMEOUT_OFFSET, a->timeout);
    Packet *pkt = PacketMalloc(0, 0, sizeof(data));
    if (pkt == nullptr) {
        return BT_NO_MEMORY;
    }
    (void)PacketPayloadWrite(pkt, data, 0, sizeof(data));
    HciOnEvent(pkt);
    PacketFree(pkt);
    return BT_SUCCESS;
}

int EattSetServiceConfigFn(void *arg)
{
    EattSetServiceConfigArg *a = static_cast<EattSetServiceConfigArg *>(arg);
    const L2capLeConfigInfo *cfg = a->nullCfg ? nullptr : &a->cfg;
    return L2CAP_LeSetServiceConfig(a->lpsm, cfg);
}

int EattSetSecLevelFn(void *arg)
{
    EattSetSecLevelArg *a = static_cast<EattSetSecLevelArg *>(arg);
    return L2CAP_LeSetServiceSecLevel(a->lpsm, a->secRequirement);
}

int EattSetSecInfoFn(void *arg)
{
    EattSetSecInfoArg *a = static_cast<EattSetSecInfoArg *>(arg);
    const BtAddr *addr = a->nullAddr ? nullptr : &a->addr;
    return L2CAP_LeSetSecurityInfo(addr, a->keySize, a->authLevel, a->authzGranted);
}

int EattRegisterServiceFn(void *arg)
{
    EattRegisterServiceArg *a = static_cast<EattRegisterServiceArg *>(arg);
    return L2CAP_LeRegisterService(a->lpsm, a->svc, a->ctx);
}

int EattDeregisterServiceFn(void *arg)
{
    EattDeregisterServiceArg *a = static_cast<EattDeregisterServiceArg *>(arg);
    return L2CAP_LeDeregisterService(a->lpsm);
}

int EattSendDataFn(void *arg)
{
    EattSendDataArg *a = static_cast<EattSendDataArg *>(arg);
    return L2CAP_LeSendData(a->lcid, a->pkt);
}

static void OnEattConnectionRsp(
    const L2capConnectionInfo *info, uint16_t result, uint8_t attempted, uint8_t succeeded, void *context)
{
    MockEattServiceCtx *svc = static_cast<MockEattServiceCtx *>(context);
    if (svc == nullptr) {
        return;
    }
    svc->connRsp.result = result;
    svc->connRsp.attempted = attempted;
    svc->connRsp.succeeded = succeeded;
    svc->connRsp.waiter.Notify();
    printf("EATT conn rsp: result = 0x%04X, attempted = %u, succeeded = %u\n", result,
        static_cast<unsigned int>(attempted), static_cast<unsigned int>(succeeded));
}

static void OnEattConnected(uint16_t lcid, const L2capConnectionInfo *info, const L2capLeConfigInfo *cfg, void *context)
{
    MockEattServiceCtx *svc = static_cast<MockEattServiceCtx *>(context);
    if (svc == nullptr) {
        return;
    }
    svc->connected.Add(lcid);
    if (cfg != nullptr) {
        printf("EATT connected: lcid = 0x%04X, mtu = %u, mps = %u\n", lcid, static_cast<unsigned int>(cfg->mtu),
            static_cast<unsigned int>(cfg->mps));
    }
}

static void OnEattReconfigured(uint16_t lcid, uint16_t newMtu, uint16_t result, void *context)
{
    MockEattServiceCtx *svc = static_cast<MockEattServiceCtx *>(context);
    if (svc == nullptr) {
        return;
    }
    svc->reconfig.Add(newMtu, result);
    printf("EATT reconfigured: lcid = 0x%04X, newMtu = %u, result = 0x%04X\n", lcid, static_cast<unsigned int>(newMtu),
        static_cast<unsigned int>(result));
}

// L2CAP calls this once per channel the stack gives up on without a disconnect
// exchange: 0x17 RTX timeout cleanup (the case under test), 0x14 timeout, short 0x18.
static void OnEattDisconnectAbnormal(uint16_t lcid, uint8_t reason, void *context)
{
    MockEattServiceCtx *svc = static_cast<MockEattServiceCtx *>(context);
    if (svc == nullptr) {
        return;
    }
    svc->disconnected.Add(lcid);
    printf("EATT disconnect abnormal: lcid = 0x%04X, reason = %u\n", lcid, static_cast<unsigned int>(reason));
}

static void OnEattDataReceived(uint16_t lcid, Packet *pkt, void *context)
{
    MockEattServiceCtx *svc = static_cast<MockEattServiceCtx *>(context);
    if (svc == nullptr || pkt == nullptr) {
        return;
    }
    uint16_t len = static_cast<uint16_t>(PacketSize(pkt));
    if (len > sizeof(svc->data.data)) {
        len = static_cast<uint16_t>(sizeof(svc->data.data));
    }
    (void)PacketPayloadRead(pkt, svc->data.data, 0, len);
    svc->data.dataLen = len;
    svc->data.lcid = lcid;
    svc->data.waiter.Notify();
    printf("EATT data received: lcid = 0x%04X, len = %u\n", lcid, static_cast<unsigned int>(len));
}

// Responder data callback: record and echo the SDU back on the same channel so the
// initiator can observe the loop. Runs on the L2CAP queue, so the direct
// L2CAP_LeSendData call here is on its expected thread.
static void OnEattDataEcho(uint16_t lcid, Packet *pkt, void *context)
{
    OnEattDataReceived(lcid, pkt, context);
    if (pkt == nullptr) {
        return;
    }
    if (L2CAP_LeSendData(lcid, pkt) != BT_SUCCESS) {
        printf("EATT data echo send failed, lcid = 0x%04X\n", lcid);
    }
}

// Build a mock EATT service struct. The ctx (if any) is default-constructed by the
// caller and reset per test via the waiter Reset(), never memset: it embeds
// std::mutex/std::condition_variable, which memset would corrupt.
static void InitMockEattService(L2capLeService &svc, bool echo)
{
    svc = { };
    svc.recvLeEattConnected = OnEattConnected;
    svc.recvLeEattConnectionRsp = OnEattConnectionRsp;
    svc.recvLeEattReconfigured = OnEattReconfigured;
    svc.leDisconnectAbnormal = OnEattDisconnectAbnormal;
    svc.recvLeData = echo ? OnEattDataEcho : OnEattDataReceived;
}

// mock EATT service registration (initiator side)
MockEattServiceCtx g_mockCtx;
// Callbacks must be wired at definition: a zeroed L2capLeService would silently
// drop every EATT notification.
L2capLeService g_mockService = {
    .recvLeEattConnected = OnEattConnected,
    .recvLeEattConnectionRsp = OnEattConnectionRsp,
    .recvLeEattReconfigured = OnEattReconfigured,
    .leDisconnectAbnormal = OnEattDisconnectAbnormal,
    .recvLeData = OnEattDataReceived,
};
bool g_mockRegistered = false;

// The ATT startup registration (AttEattRegisterService, att_init.c ATT_StartUpAsync)
// holds PSM 0x0027 in a fresh process; take it over so the mock service drives the
// L2CAP ECRED tests. Both run on the L2CAP processing queue (FIFO), so this
// deregister always runs after the startup registration, no race.
static void TakeOverEattPsm()
{
    EattDeregisterServiceArg dereg = { };
    dereg.lpsm = L2CAP_LE_EATT_PSM;
    (void)CallL2cap(EattDeregisterServiceFn, &dereg);
}

static bool RegisterMockEattService()
{
    TakeOverEattPsm();
    EattRegisterServiceArg arg = { };
    arg.lpsm = L2CAP_LE_EATT_PSM;
    arg.svc = &g_mockService;
    arg.ctx = &g_mockCtx;
    if (CallL2cap(EattRegisterServiceFn, &arg) != BT_SUCCESS) {
        printf("RegisterMockEattService: register failed\n");
        return false;
    }
    g_mockRegistered = true;
    return true;
}

bool EnsureMockEattService()
{
    if (g_mockRegistered) {
        return true;
    }
    return RegisterMockEattService();
}

// SMP pairing helpers (legacy Just Works, auto-accept via a dedicated responder thread)
enum class PairRspType {
    FEATURE,
    USER_CONFIRM,
    PASS_KEY
};

struct PairRsp {
    PairRspType type = PairRspType::FEATURE;
    BtAddr addr = { };
};

std::mutex g_pairMtx;
std::condition_variable g_pairCv;
std::queue<PairRsp> g_pairQueue;
// Read outside g_pairMtx in PairResponderThread::while-condition and the wait
// predicate, written under the mutex in ShutdownPairResponder: must be atomic.
std::atomic<bool> g_pairResponderExit = false;

// Current pairing-initiation epoch (review m19): incremented by OnPairEpochArm on the
// GAP queue, read by Notify and by the Wait predicate. Atomic: the test thread reads it
// in the predicate while the Stack thread writes it in the arm task. Declared before
// PairCompleteResult: in-class member function bodies cannot see namespace-scope names
// that are declared after the class.
std::atomic<uint64_t> g_pairEpoch{0};

struct PairCompleteResult {
    std::mutex mtx;
    std::condition_variable cv;
    bool received = false;
    uint8_t result = 0xFF;
    // Pairing-initiation epoch (review m19): recorded by Notify from g_pairEpoch at
    // delivery time, compared by Wait against the current epoch. A "late complete" of
    // a PREVIOUS pairing (e.g. the SMP 0x05 failure the disconnect in ResetConnection
    // asynchronously delivers) is then rejected by the next test's Wait even if it
    // arrives after that test's Reset, so it cannot be mis-consumed as its own result.
    uint64_t epoch = 0;

    bool Wait()
    {
        std::unique_lock<std::mutex> lock(mtx);
        return cv.wait_for(lock, std::chrono::milliseconds(EATT_WAIT_TIMEOUT_MS), [this] {
            return received && epoch == g_pairEpoch.load();
        });
    }

    void Reset()
    {
        std::lock_guard<std::mutex> lock(mtx);
        received = false;
        result = 0xFF;
    }

    void Notify(uint8_t pairResult)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            received = true;
            result = pairResult;
            epoch = g_pairEpoch.load();
        }
        cv.notify_all();
    }
};

PairCompleteResult g_pairResult;

// The GAP pair callbacks run on the GAP queue; the response APIs (GAPIF_LePairRsp
// etc.) block on that same queue, so the responses are deferred to a dedicated
// thread to avoid a self-deadlock.
static void PairResponderThread()
{
    while (!g_pairResponderExit) {
        PairRsp rsp;
        {
            std::unique_lock<std::mutex> lock(g_pairMtx);
            g_pairCv.wait(lock, [] {
                return g_pairResponderExit || !g_pairQueue.empty();
            });
            if (g_pairResponderExit) {
                return;
            }
            rsp = g_pairQueue.front();
            g_pairQueue.pop();
        }
        switch (rsp.type) {
            case PairRspType::FEATURE: {
                GapLePairFeature feature = { };
                feature.ioCapability = IO_CAP_NO_INPUT_NO_OUTPUT; // both sides -> Just Works
                feature.oobDataFlag = 0;
                feature.authReq = GAP_LE_AUTH_REQ_BONDING; // legacy, no MITM -> no user interaction
                feature.maxEncKeySize = GAP_LINKKEY_SIZE;
                feature.initKeyDis = GAP_LE_KEY_DIST_ENC_KEY | GAP_LE_KEY_DIST_ID_KEY;
                feature.respKeyDis = GAP_LE_KEY_DIST_ENC_KEY | GAP_LE_KEY_DIST_ID_KEY;
                (void)GAPIF_LePairFeatureRsp(&rsp.addr, feature);
                break;
            }
            case PairRspType::USER_CONFIRM:
                (void)GAPIF_LePairScUserConfirmRsp(&rsp.addr, 1);
                break;
            case PairRspType::PASS_KEY:
                (void)GAPIF_LePairPassKeyRsp(&rsp.addr, 1, 0);
                break;
        }
    }
}

static void OnLePairFeatureReq(const BtAddr *addr, bool localPair, void *context)
{
    PairRsp rsp = { };
    rsp.type = PairRspType::FEATURE;
    if (addr != nullptr) {
        rsp.addr = *addr;
    }
    std::lock_guard<std::mutex> lock(g_pairMtx);
    g_pairQueue.push(rsp);
    g_pairCv.notify_one();
}

static void OnLePairScUserConfirmReq(const BtAddr *addr, uint32_t number, void *context)
{
    PairRsp rsp = { };
    rsp.type = PairRspType::USER_CONFIRM;
    if (addr != nullptr) {
        rsp.addr = *addr;
    }
    std::lock_guard<std::mutex> lock(g_pairMtx);
    g_pairQueue.push(rsp);
    g_pairCv.notify_one();
}

static void OnLePairPassKeyReq(const BtAddr *addr, void *context)
{
    PairRsp rsp = { };
    rsp.type = PairRspType::PASS_KEY;
    if (addr != nullptr) {
        rsp.addr = *addr;
    }
    std::lock_guard<std::mutex> lock(g_pairMtx);
    g_pairQueue.push(rsp);
    g_pairCv.notify_one();
}

static void OnLePairComplete(const BtAddr *addr, uint8_t result, uint8_t keyType, void *context)
{
    printf("SMP pair complete, result = %u, keyType = %u\n", static_cast<unsigned int>(result),
        static_cast<unsigned int>(keyType));
    g_pairResult.Notify(result);
}

GapLePairCallback g_pairCallback = {
    .lePairFeatureReq = OnLePairFeatureReq,
    .lePairPassKeyReq = OnLePairPassKeyReq,
    .lePairScUserConfirmReq = OnLePairScUserConfirmReq,
    .lePairComplete = OnLePairComplete,
};

std::thread g_pairResponder;

void InitPairResponder()
{
    g_pairResult.Reset();
    (void)GAPIF_RegisterLePairCallback(&g_pairCallback, nullptr);
    g_pairResponderExit = false;
    g_pairResponder = std::thread(PairResponderThread);
}

void ShutdownPairResponder()
{
    {
        std::lock_guard<std::mutex> lock(g_pairMtx);
        g_pairResponderExit = true;
    }
    g_pairCv.notify_all();
    if (g_pairResponder.joinable()) {
        g_pairResponder.join();
    }
    (void)GAPIF_DeregisterLePairCallback();
}

// pair + establish helpers (initiator side)
bool g_paired = false;

// Tear down the current link so the next EATT test starts from an unencrypted
// state. The EATT reject case depends on this: a reused encrypted link would
// accept the 0x17 instead of returning 0x0008.
void ResetConnection()
{
    if (g_connected) {
        // L2CIF_LeDisconnect is void: the L2CAP result and the link-down are both
        // delivered asynchronously, the ACL disconnection-complete is what the
        // next test's "unencrypted link" assumption depends on, so wait for it and
        // check its status instead of ignoring the outcome.
        g_connCtx.Reset();
        L2CIF_LeDisconnect(g_connCtx.handle, nullptr);
        if (!g_connCtx.Wait(EATT_WAIT_TIMEOUT_MS)) {
            ADD_FAILURE() << "ResetConnection: wait disconnect complete timeout";
        } else if (g_connCtx.status != HCI_STATUS_SUCCESS) {
            ADD_FAILURE() << "ResetConnection: disconnect complete status = 0x" << std::hex
                          << static_cast<int>(g_connCtx.status) << std::dec;
        }
        g_connected = false;
    }
    g_paired = false;
}

// Queued GAP-queue call helper, mirroring CallL2cap: heap-allocated call object, wait
// bounded by EATT_WAIT_TIMEOUT_MS, ownership handed to the callback via abandoned on
// timeout. Used to arm the pairing epoch on the GAP queue.
struct GapQueueCall {
    CallbackWaiter done;
    void (*fn)(void *arg) = nullptr;
    void *arg = nullptr;
    std::atomic<bool> abandoned = false;
};

static void OnGapQueueCall(void *ctx)
{
    GapQueueCall *call = static_cast<GapQueueCall *>(ctx);
    call->fn(call->arg);
    bool freeSelf;
    {
        std::lock_guard<std::mutex> lock(call->done.mtx);
        freeSelf = call->abandoned.load();
        call->done.received = true;
        call->done.cv.notify_all();
    }
    if (freeSelf) {
        delete call;
    }
}

static bool CallOnGapQueue(void (*fn)(void *arg), void *arg)
{
    GapQueueCall *call = new GapQueueCall;
    call->fn = fn;
    call->arg = arg;
    if (BTM_RunTaskInProcessingQueue(PROCESSING_QUEUE_ID_GAP, OnGapQueueCall, call) != BT_SUCCESS) {
        delete call; // never queued: the callback will not run
        return false;
    }
    if (call->done.Wait(EATT_WAIT_TIMEOUT_MS)) {
        delete call; // the callback observed abandoned == false, so it did not free
        return true;
    }
    // Timed out while the call is still queued: same ownership hand-over as CallL2cap.
    bool callbackWillFree;
    {
        std::lock_guard<std::mutex> lock(call->done.mtx);
        call->abandoned.store(true);
        callbackWillFree = !call->done.received;
    }
    if (!callbackWillFree) {
        delete call;
    }
    return false;
}

// Pairing-initiation epoch arm (review m19): increments the epoch and resets the pair
// result ON the GAP queue, so it is FIFO-ordered before the GAPIF_LePair request of the
// same pairing. Any pair-complete task already queued for a PREVIOUS pairing (the 0x05
// failure delivered asynchronously by the disconnect in ResetConnection) executes before
// this arm, records the OLD epoch in its Notify, and is rejected by the Wait predicate;
// the real complete of this pairing is queued after GAPIF_LePair, hence after this arm,
// and records the NEW epoch.
static void OnPairEpochArm(void *arg)
{
    (void)arg;
    g_pairEpoch.fetch_add(1);
    g_pairResult.Reset();
}

// Start legacy Just Works pairing. Both sides auto-respond with NoInputNoOutput,
// so no passkey or numeric-comparison confirmation is required.
static bool PairWithPeer()
{
    if (g_paired) {
        return true;
    }
    if (!g_peerAddrValid) {
        return false;
    }
    if (!CallOnGapQueue(OnPairEpochArm, nullptr)) {
        printf("PairWithPeer: arm pair epoch failed\n");
        return false;
    }
    if (GAPIF_LePair(&g_peerAddr) != BT_SUCCESS) {
        printf("PairWithPeer: GAPIF_LePair failed\n");
        return false;
    }
    if (!g_pairResult.Wait()) {
        printf("PairWithPeer: wait pair complete timeout\n");
        return false;
    }
    g_paired = (g_pairResult.result == PAIR_STATUS_SUCCESS);
    // The responder's security check on the 0x17 needs its HCI 0x08 encryption-
    // change event processed first; that ordering is peer-side and cannot be polled
    // from this board. EstablishEattChannels() therefore retries once on a 0x0008
    // rejection instead of parking on a fixed delay here.
    return g_paired;
}

// Open a batch of n EATT channels over the current (encrypted) link and wait for
// the 0x18 result plus the per-channel connected notifications. The responder's
// security check needs its 0x08 processed before the 0x17 lands; that ordering is
// peer-side and not observable from this board, so a 0x0008 rejection is retried
// once (the reject round-trip bounds the responder's 0x08 processing) instead of
// sleeping a fixed time before the first send.
constexpr int EATT_REQ_MAX_ATTEMPTS = 2;

// Number of EATT channels the EATT-preference scenarios open over the (encrypted) link:
// the idle-pick / busy-bearer-skip / UATT-fallback walk of Vol 3 Part F 3.3.2 needs two
// real bearer slots to exercise "every EATT bearer busy" (send #3 falls back to UATT).
constexpr int EATT_CHANNEL_COUNT = 2;

bool EstablishEattChannels(uint16_t lcids[], uint16_t n)
{
    uint16_t dummyHandle = 0;
    if (!ConnectToPeer(dummyHandle) || !PairWithPeer() || !EnsureMockEattService()) {
        return false;
    }
    EattConnReqArg arg = { };
    arg.addr = g_peerAddr;
    arg.mtu = EATT_TEST_MTU;
    arg.mps = EATT_TEST_MPS;
    arg.credit = EATT_TEST_CREDIT;
    arg.n = n;
    for (int attempt = 0; attempt < EATT_REQ_MAX_ATTEMPTS; attempt++) {
        g_mockCtx.connRsp.waiter.Reset();
        g_mockCtx.connected.Reset();
        int ret = CallL2cap(EattConnReqFn, &arg);
        if (ret != BT_SUCCESS) {
            printf("EstablishEattChannels: L2CAP_LeEattConnectionReq = %d\n", ret);
            return false;
        }
        for (uint16_t i = 0; i < n; i++) {
            lcids[i] = arg.lcids[i];
        }
        if (!g_mockCtx.connRsp.waiter.Wait(EATT_WAIT_TIMEOUT_MS)) {
            printf("EstablishEattChannels: wait 0x18 timeout\n");
            return false;
        }
        if (g_mockCtx.connRsp.result == L2CAP_LE_INSUFFICIENT_ENCRYPTION) {
            printf("EstablishEattChannels: 0x18 result = 0x0008 (link not yet encrypted), retrying\n");
            continue;
        }
        if (g_mockCtx.connRsp.result != L2CAP_LE_CONNECTION_SUCCESSFUL) {
            printf("EstablishEattChannels: 0x18 result = 0x%04X\n", g_mockCtx.connRsp.result);
            return false;
        }
        break;
    }
    if (g_mockCtx.connRsp.result != L2CAP_LE_CONNECTION_SUCCESSFUL) {
        printf("EstablishEattChannels: 0x18 result = 0x%04X after retry\n", g_mockCtx.connRsp.result);
        return false;
    }
    if (!g_mockCtx.connected.WaitFor(n)) {
        printf("EstablishEattChannels: wait connected callbacks timeout, got %u/%u\n",
            static_cast<unsigned int>(g_mockCtx.connected.Count()), static_cast<unsigned int>(n));
        return false;
    }
    return true;
}

// ===================== ATT UATT-bearer helpers (plan chapters 4/5, D/E units) =====================
// The 30 s transaction timeout (Vol 3 Part F 3.3.3) plus margin for the stack to
// report 0x020E and tear the bearer down.
constexpr uint32_t ATT_TRANSACTION_TIMEOUT_WAIT_MS = 40000;
// Attribute handle the peer leaves unconfirmed, to drive the indication-gate and
// the 30 s indication-timeout cases. Must match TEST_IGNORE_CONFIRM_HANDLE on the
// companion side (InitAttPeerSetup).
constexpr uint16_t ATT_IND_HANDLE_IGNORE = 0xF0F0;
// UATT bearer cid (LE_CID 0x0004, att_common.h); used to exercise the non-zero
// *Cid routing branch (AndLeCid) in single-bearer tests.
constexpr uint16_t ATT_LE_CID = 0x0004;
// Window to prove an *Cid call on an unknown connection/cid is a no-op: the async
// path resolves connect == nullptr and posts no send-complete callback.
constexpr uint32_t CID_INVALID_WAIT_MS = 1500;

// Server-side events (ATT_ServerDataRegister): the confirmation ends the
// indication-confirmation transaction (0x020D), the indication timeout closes it
// (0x020E, Vol 3 Part F 3.3.3).
struct AttServerEventCtx : CallbackWaiter {
    uint16_t event = 0;
};
AttServerEventCtx g_attServerConfCtx;    // 0x020D received from the peer
AttServerEventCtx g_attServerTimeoutCtx; // 0x020E server indication timeout
// Client-side events (ATT_ClientDataRegister): 0x020E client request timeout.
struct AttClientEventCtx : CallbackWaiter {
    uint16_t event = 0;
};
AttClientEventCtx g_attClientTimeoutCtx;
// Send-complete result (ATT_ServerSendDataRegister): BT_SUCCESS when queued,
// BT_ALREADY while an indication is still pending.
struct AttSendResultCtx : CallbackWaiter {
    int result = -1;
};
AttSendResultCtx g_attSendResultCtx;
// UATT connect handle from attLEConnectCompleted.
struct AttConnectCtx : CallbackWaiter {
    uint16_t connectHandle = 0;
};
AttConnectCtx g_attConnectCtx;

void OnAttConnectCompleted(uint16_t connectHandle, AttLeConnectCallback *data, void *context)
{
    (void)data;
    (void)context;
    g_attConnectCtx.connectHandle = connectHandle;
    g_attConnectCtx.Notify();
}

void OnAttServerData(uint16_t connectHandle, uint16_t event, void *eventData, Buffer *buffer, void *context)
{
    (void)connectHandle;
    (void)eventData;
    (void)buffer;
    (void)context;
    if (event == ATT_HANDLE_VALUE_CONFIRMATION_ID) {
        g_attServerConfCtx.event = event;
        g_attServerConfCtx.Notify();
    } else if (event == ATT_TRANSACTION_TIME_OUT_ID) {
        g_attServerTimeoutCtx.event = event;
        g_attServerTimeoutCtx.Notify();
    }
}

void OnAttClientData(uint16_t connectHandle, uint16_t event, void *eventData, Buffer *buffer, void *context)
{
    (void)connectHandle;
    (void)eventData;
    (void)buffer;
    (void)context;
    if (event == ATT_TRANSACTION_TIME_OUT_ID) {
        g_attClientTimeoutCtx.event = event;
        g_attClientTimeoutCtx.Notify();
    }
}

void OnAttServerSendData(uint16_t connectHandle, int result, void *context)
{
    (void)connectHandle;
    (void)context;
    g_attSendResultCtx.result = result;
    g_attSendResultCtx.Notify();
}

// Establish the LE ACL (via ConnectToPeer) and wait for the UATT bearer's
// attLEConnectCompleted to hand back the ATT connect handle.
bool EnsureAttConnected()
{
    if (!g_connected) {
        g_attConnectCtx.Reset();
        uint16_t dummyHandle = 0;
        if (!ConnectToPeer(dummyHandle)) {
            return false;
        }
        if (!g_attConnectCtx.Wait()) {
            printf("EnsureAttConnected: wait attLEConnectCompleted timeout\n");
            return false;
        }
    }
    return g_attConnectCtx.connectHandle != 0;
}

// Queue an indication and wait for the send-complete callback; returns the result
// (BT_SUCCESS / BT_ALREADY) in *result. ATT_HandleValueIndication copies the value
// (BufferRefMalloc), so the caller keeps ownership of the buffer.
bool SendAttIndication(uint16_t connectHandle, uint16_t attHandle, int &result)
{
    const uint8_t payload[] = { 0xAA, 0x55 };
    Buffer *buf = BufferMalloc(sizeof(payload));
    if (buf == nullptr) {
        return false;
    }
    if (memcpy_s(BufferPtr(buf), sizeof(payload), payload, sizeof(payload)) != EOK) {
        BufferFree(buf);
        return false;
    }
    g_attSendResultCtx.Reset();
    ATT_HandleValueIndication(connectHandle, attHandle, buf);
    BufferFree(buf);
    if (!g_attSendResultCtx.Wait()) {
        printf("SendAttIndication: wait send result timeout\n");
        return false;
    }
    result = g_attSendResultCtx.result;
    return true;
}

// Same as SendAttIndication but the bearer is pinned by cid (H-3): a non-zero cid
// resolves via AndLeCid to that exact bearer, 0 routes via AttNtfIndResolveConnect
// (first idle EATT bearer, else UATT).
bool SendAttIndicationCid(uint16_t connectHandle, uint16_t cid, uint16_t attHandle, int &result)
{
    const uint8_t payload[] = { 0xAA, 0x55 };
    Buffer *buf = BufferMalloc(sizeof(payload));
    if (buf == nullptr) {
        return false;
    }
    if (memcpy_s(BufferPtr(buf), sizeof(payload), payload, sizeof(payload)) != EOK) {
        BufferFree(buf);
        return false;
    }
    g_attSendResultCtx.Reset();
    ATT_HandleValueIndicationCid(connectHandle, cid, attHandle, buf);
    BufferFree(buf);
    if (!g_attSendResultCtx.Wait()) {
        printf("SendAttIndicationCid: wait send result timeout\n");
        return false;
    }
    result = g_attSendResultCtx.result;
    return true;
}

uint16_t g_peerAttConnectHandle = 0;
constexpr uint16_t TEST_IGNORE_CONFIRM_HANDLE = 0xF0F0;

static void OnPeerAttConnectCompleted(uint16_t connectHandle, AttLeConnectCallback *data, void *context)
{
    (void)data;
    (void)context;
    g_peerAttConnectHandle = connectHandle;
}

// AttCallbackCid (att.h) fixes the six-parameter callback signature; the
// captureless lambda keeps the handler out of the named-function parameter
// count (G.FUN.01) while preserving the exact ATT API type.
static AttCallbackCid g_onPeerAttClientDataCid = [](uint16_t connectHandle, uint16_t cid, uint16_t event,
    void *eventData, Buffer *buffer, void *context) {
    (void)buffer;
    (void)context;
    if (event != ATT_HANDLE_VALUE_INDICATION_ID) {
        return;
    }
    AttHandleValue *ind = static_cast<AttHandleValue *>(eventData);
    if (ind->attHandle == TEST_IGNORE_CONFIRM_HANDLE) {
        return; // leave unconfirmed: drives the 30 s server indication-timeout case
    }
    // Confirm on the bearer the indication arrived on (Vol 3 Part F 3.3.3: a
    // transaction shall be performed on one ATT bearer); on UATT cid is LE_CID.
    ATT_HandleValueConfirmationCid(connectHandle, cid);
};

static void OnPeerAttServerData(uint16_t connectHandle, uint16_t event, void *eventData, Buffer *buffer, void *context)
{
    // The peer never answers attribute requests: silence drives the client
    // request-timeout case on the DUT.
    (void)connectHandle;
    (void)event;
    (void)eventData;
    (void)buffer;
    (void)context;
}

// ATT companion setup (device B side), called from the interact test's existing
// RunPeerMode ("server" argv). Registers the ATT connect / client / server
// callbacks: the peer records the UATT connect handle, confirms every received
// indication except TEST_IGNORE_CONFIRM_HANDLE, and stays silent on requests.
// External linkage: called across translation units from
// stack_gap_le_interact_test.cpp.
int InitAttPeerSetup()
{
    AttConnectCallback connectCb = { };
    connectCb.attLEConnectCompleted = OnPeerAttConnectCompleted;
    ATT_ConnectRegister(connectCb, nullptr);
    ATT_ClientDataRegisterCid(g_onPeerAttClientDataCid, nullptr);
    ATT_ServerDataRegister(OnPeerAttServerData, nullptr);
    return 0;
}

// EATT companion setup (device B side), called from the interact test's existing
// RunPeerMode ("server" argv) so ISO / power-control / EATT two-device cases all
// share one companion. The board is already initialized, set to peripheral +
// broadcaster and advertising by the caller; here we register the EATT PSM with an
// echoing mock service, set the default config, and start the SMP auto-answer
// responder thread. External linkage: called across translation units from
// stack_gap_le_interact_test.cpp.
int InitEattPeerSetup()
{
    // Take over PSM 0x0027 from the ATT startup registration (AttEattRegisterService,
    // att_init.c ATT_StartUpAsync), then register the echo service and set its default
    // config: the config check in L2CAP_LeSetServiceConfig requires the PSM slot to be
    // registered already.
    TakeOverEattPsm();
    L2capLeService peerService = { };
    // echo by default; with BT52_EATT_NO_CONF set the mock receives but never echoes
    // EATT data, so the DUT's serverSendFlag stays set for the layer-2 real-send case.
    const bool eattNoConf = std::getenv("BT52_EATT_NO_CONF") != nullptr;
    InitMockEattService(peerService, !eattNoConf);
    EattRegisterServiceArg regArg = { };
    regArg.lpsm = L2CAP_LE_EATT_PSM;
    regArg.svc = &peerService;
    regArg.ctx = &g_mockCtx;
    if (CallL2cap(EattRegisterServiceFn, &regArg) != BT_SUCCESS) {
        printf("eatt peer: register EATT PSM failed\n");
        return 1;
    }
    EattSetServiceConfigArg cfgArg = { };
    cfgArg.lpsm = L2CAP_LE_EATT_PSM;
    cfgArg.cfg.mtu = SERVER_MTU;
    cfgArg.cfg.mps = SERVER_MTU;
    cfgArg.cfg.credit = 1;
    if (CallL2cap(EattSetServiceConfigFn, &cfgArg) != BT_SUCCESS) {
        printf("eatt peer: set service config failed\n");
        return 1;
    }

    InitPairResponder();
    return 0;
}
// ATT server/client two-device cases on the legacy ATT bearer (LE_CID 0x0004):
// indication send / confirm / timeout handling and the client request-timeout
// path (plan chapters 4/5, D/E units). Also covers the EATT bearer-selection
// logic (AttGetConnectInfoByConnectHandlePreferEattInd, incl. an invalid-cid
// guard and a cid-pinned send) and a real EATT-bearer send against a peer in
// BT52_EATT_NO_CONF mode, where the EATT channels never confirm so the third
// indication falls back to UATT.
// Companion side: InitAttPeerSetup() in this file, invoked from the interact test's
// RunPeerMode — the peer confirms every indication except handle 0xF0F0 and never
// answers attribute requests.
class StackAtt52Test : public testing::Test {
public:
    StackAtt52Test() { }
    ~StackAtt52Test() { }

    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp() { }
    // Drop the link after each test so a pending indication (E-U-2) or a timed-out
    // bearer (E-U-5 / D-U-1) cannot leak into the next case's assumptions.
    void TearDown()
    {
        ResetConnection();
    }
};

void StackAtt52Test::SetUpTestCase(void)
{
    ASSERT_EQ(BTM_Initialize(), BT_SUCCESS);
    ASSERT_EQ(BTM_Enable(LE_CONTROLLER), BT_SUCCESS);
    ASSERT_TRUE(BTM_IsEnabled(LE_CONTROLLER));
    ASSERT_EQ(GAPIF_LeSetRole(GAP_LE_ROLE_CENTRAL), BT_SUCCESS);

    // Two-device tests need a real peer: parse BT52_PEER_ADDR and register the
    // ACL connection-complete callback. BTM stores the callback struct pointer, so
    // g_aclCallbacks must be static.
    InitPeerAddrArg();
    g_peerAddrValid = ParsePeerAddr(g_peerAddrArg, &g_peerAddr);
    if (!g_peerAddrValid) {
        printf("peer address not set (BT52_PEER_ADDR=XX:XX:XX:XX:XX:XX), two-device tests will be skipped\n");
    }
    ASSERT_EQ(BTM_RegisterAclCallbacks(&g_aclCallbacks, &g_connCtx), BT_SUCCESS);

    // SMP pairing responder thread for the EATT two-device tests (auto-answer).
    InitPairResponder();

    // ATT callbacks: connect (UATT connect handle), server data (0x020D / 0x020E)
    // and send-complete (result of ATT_HandleValueIndication).
    AttConnectCallback connectCb = { };
    connectCb.attLEConnectCompleted = OnAttConnectCompleted;
    ATT_ConnectRegister(connectCb, nullptr);
    ATT_ServerDataRegister(OnAttServerData, nullptr);
    ATT_ServerSendDataRegister(OnAttServerSendData, nullptr);
    ATT_ClientDataRegister(OnAttClientData, nullptr);
}

void StackAtt52Test::TearDownTestCase(void)
{
    ResetConnection();
    // Deregister the ATT callbacks so they cannot fire into this fixture after the
    // group ends (other fixtures never register ATT callbacks).
    ATT_ConnectDeregister();
    ATT_ServerDataDeregister();
    ATT_ServerSendDataDeRegister();
    ATT_ClientDataDeregister();
    (void)BTM_DeregisterAclCallbacks(&g_aclCallbacks);
    ShutdownPairResponder();
    (void)BTM_Disable(LE_CONTROLLER);
    (void)BTM_Close();
}

/**
 * @tc.number: StackAtt52_UattIndicationSend_00100
 * @tc.name: ATT_HandleValueIndication send success on the UATT bearer
 * @tc.desc: queue an indication the peer confirms; the send-complete callback must
 *           report BT_SUCCESS (Vol 3 Part F 3.3.2 allows one outstanding indication)
 */
HWTEST_F(StackAtt52Test, StackAtt52_UattIndicationSend_00100, TestSize.Level1)
{
    if (!EnsureAttConnected()) {
        GTEST_SKIP() << "peer not connected (pass peer addr as BT52_PEER_ADDR, start "
                        "server mode on device B)";
    }

    int result = -1;
    ASSERT_TRUE(SendAttIndication(g_attConnectCtx.connectHandle, 0x0010, result));
    EXPECT_EQ(result, BT_SUCCESS);
}

/**
 * @tc.number: StackAtt52_UattIndicationPendingReject_00200
 * @tc.name: second indication rejected while one is pending
 * @tc.desc: with an unconfirmed indication outstanding (handle 0xF0F0 the peer does
 *           not confirm), a further indication must be rejected with BT_ALREADY
 *           (Vol 3 Part F 3.3.2: no other indications until a confirmation PDU)
 */
HWTEST_F(StackAtt52Test, StackAtt52_UattIndicationPendingReject_00200, TestSize.Level1)
{
    if (!EnsureAttConnected()) {
        GTEST_SKIP() << "peer not connected (pass peer addr as BT52_PEER_ADDR, start "
                        "server mode on device B)";
    }

    // First indication stays pending: handle 0xF0F0 is left unconfirmed by the peer.
    int result = -1;
    ASSERT_TRUE(SendAttIndication(g_attConnectCtx.connectHandle, ATT_IND_HANDLE_IGNORE, result));
    EXPECT_EQ(result, BT_SUCCESS);

    // Second indication while the first is still pending: rejected by the gate.
    ASSERT_TRUE(SendAttIndication(g_attConnectCtx.connectHandle, 0x0010, result));
    EXPECT_EQ(result, BT_ALREADY);
}

/**
 * @tc.number: StackAtt52_UattConfirmationEvent_00300
 * @tc.name: confirmation event reported to the server callback
 * @tc.desc: the peer's confirmation ends the indication-confirmation transaction
 *           (Vol 3 Part F 3.3.3); the server callback must receive 0x020D. The old
 *           ATT_HandleValueIndication delegates to ATT_HandleValueIndicationCid with
 *           cid 0, so this case also covers the *Cid zero-cid selection branch
 *           (AttNtfIndResolveConnect -> PreferEatt).
 */
HWTEST_F(StackAtt52Test, StackAtt52_UattConfirmationEvent_00300, TestSize.Level1)
{
    if (!EnsureAttConnected()) {
        GTEST_SKIP() << "peer not connected (pass peer addr as BT52_PEER_ADDR, start "
                        "server mode on device B)";
    }

    g_attServerConfCtx.Reset();
    int result = -1;
    ASSERT_TRUE(SendAttIndication(g_attConnectCtx.connectHandle, 0x0010, result));
    ASSERT_EQ(result, BT_SUCCESS);

    ASSERT_TRUE(g_attServerConfCtx.Wait()) << "no confirmation event within timeout";
    EXPECT_EQ(g_attServerConfCtx.event, ATT_HANDLE_VALUE_CONFIRMATION_ID);
}

/**
 * @tc.number: StackAtt52_UattIndicationAfterConfirm_00400
 * @tc.name: indication allowed again after the confirmation
 * @tc.desc: once the peer confirms the first indication, a further indication must
 *           be accepted (the pending gate is cleared by 0x020D)
 */
HWTEST_F(StackAtt52Test, StackAtt52_UattIndicationAfterConfirm_00400, TestSize.Level1)
{
    if (!EnsureAttConnected()) {
        GTEST_SKIP() << "peer not connected (pass peer addr as BT52_PEER_ADDR, start "
                        "server mode on device B)";
    }

    g_attServerConfCtx.Reset();
    int result = -1;
    ASSERT_TRUE(SendAttIndication(g_attConnectCtx.connectHandle, 0x0010, result));
    ASSERT_EQ(result, BT_SUCCESS);

    ASSERT_TRUE(g_attServerConfCtx.Wait()) << "no confirmation event within timeout";

    // After the confirmation the gate is open: the second indication must queue.
    ASSERT_TRUE(SendAttIndication(g_attConnectCtx.connectHandle, 0x0010, result));
    EXPECT_EQ(result, BT_SUCCESS);
}

/**
 * @tc.number: StackAtt52_UattIndicationTimeout_00500
 * @tc.name: unconfirmed indication times out and drops the bearer
 * @tc.desc: an indication left unconfirmed must time out after 30 s, report 0x020E
 *           to the server callback and drop the bearer (Vol 3 Part F 3.3.3)
 */
HWTEST_F(StackAtt52Test, StackAtt52_UattIndicationTimeout_00500, TestSize.Level1)
{
    if (!EnsureAttConnected()) {
        GTEST_SKIP() << "peer not connected (pass peer addr as BT52_PEER_ADDR, start "
                        "server mode on device B)";
    }

    // The peer never confirms handle 0xF0F0: the indication-confirmation transaction
    // times out after 30 s (INSTRUCTIONTIMEOUT) and the bearer is dropped. The bearer
    // drop then tears the whole ACL down (AttDisconnectBearer) on the BTM callback,
    // so both waiters are armed before the indication is sent.
    g_attServerTimeoutCtx.Reset();
    g_connCtx.Reset();
    int result = -1;
    ASSERT_TRUE(SendAttIndication(g_attConnectCtx.connectHandle, ATT_IND_HANDLE_IGNORE, result));
    ASSERT_EQ(result, BT_SUCCESS);

    ASSERT_TRUE(g_attServerTimeoutCtx.Wait(ATT_TRANSACTION_TIMEOUT_WAIT_MS)) <<
        "no server indication-timeout event within 40 s";
    EXPECT_EQ(g_attServerTimeoutCtx.event, ATT_TRANSACTION_TIME_OUT_ID);

    ASSERT_TRUE(g_connCtx.Wait(ATT_TRANSACTION_TIMEOUT_WAIT_MS)) << "no ACL disconnect after timeout";
    g_connected = false;
}

/**
 * @tc.number: StackAtt52_UattClientRequestTimeout_00600
 * @tc.name: client request with no response times out and drops the bearer
 * @tc.desc: an attribute request the peer never answers must time out after 30 s,
 *           report 0x020E to the client callback and drop the bearer
 *           (Vol 3 Part F 3.3.3, client transaction)
 */
HWTEST_F(StackAtt52Test, StackAtt52_UattClientRequestTimeout_00600, TestSize.Level1)
{
    if (!EnsureAttConnected()) {
        GTEST_SKIP() << "peer not connected (pass peer addr as BT52_PEER_ADDR, start "
                        "server mode on device B)";
    }

    // The peer's server callback never answers attribute requests: the read request
    // transaction has no response and times out after 30 s. The bearer drop then
    // tears the whole ACL down (AttDisconnectBearer) on the BTM callback, so both
    // waiters are armed before the request is sent.
    g_attClientTimeoutCtx.Reset();
    g_connCtx.Reset();
    ATT_ReadRequest(g_attConnectCtx.connectHandle, 0x0010);

    ASSERT_TRUE(g_attClientTimeoutCtx.Wait(ATT_TRANSACTION_TIMEOUT_WAIT_MS)) <<
        "no client request-timeout event within 40 s";
    EXPECT_EQ(g_attClientTimeoutCtx.event, ATT_TRANSACTION_TIME_OUT_ID);

    ASSERT_TRUE(g_connCtx.Wait(ATT_TRANSACTION_TIMEOUT_WAIT_MS)) << "no ACL disconnect after timeout";
    g_connected = false;
}

/**
 * @tc.number: StackAtt52_AttCidInvalidNoCrash_01000
 * @tc.name: *Cid calls with an unknown connection/cid are safe no-ops
 * @tc.desc: an invalid connect handle + cid resolves no bearer; each *Cid async
 *           path bails on connect == nullptr, so no send-complete callback fires
 *           within the wait window (no crash, no side effects)
 */
HWTEST_F(StackAtt52Test, StackAtt52_AttCidInvalidNoCrash_01000, TestSize.Level1)
{
    const uint16_t bogusHandle = 0xBEEF;
    const uint16_t bogusCid = 0x00EE;
    const uint8_t payload[] = { 0xAA, 0x55 };
    Buffer *buf = BufferMalloc(sizeof(payload));
    ASSERT_TRUE(buf != nullptr);
    ASSERT_EQ(memcpy_s(BufferPtr(buf), sizeof(payload), payload, sizeof(payload)), EOK);

    g_attSendResultCtx.Reset();
    ATT_HandleValueNotificationCid(bogusHandle, bogusCid, 0x0010, buf);
    ATT_HandleValueIndicationCid(bogusHandle, bogusCid, 0x0010, buf);
    BufferFree(buf);
    ATT_HandleValueConfirmationCid(bogusHandle, bogusCid);
    ATT_ReadResponseCid(bogusHandle, bogusCid, nullptr);

    // None of the async paths can reach a live ATT connect: the wait must time out.
    EXPECT_FALSE(g_attSendResultCtx.Wait(CID_INVALID_WAIT_MS));
}

/**
 * @tc.number: StackAtt52_AttCidPinnedSend_01100
 * @tc.name: *Cid indication with a non-zero cid pins the bearer (AndLeCid)
 * @tc.desc: a non-zero cid (here LE_CID) resolves the exact UATT bearer instead of
 *           the legacy first-match path; the peer confirms on that bearer and the
 *           server callback receives 0x020D (Vol 3 Part F 3.3.3)
 */
HWTEST_F(StackAtt52Test, StackAtt52_AttCidPinnedSend_01100, TestSize.Level1)
{
    if (!EnsureAttConnected()) {
        GTEST_SKIP() << "peer not connected (pass peer addr as BT52_PEER_ADDR, start "
                        "server mode on device B)";
    }

    g_attServerConfCtx.Reset();
    int result = -1;
    ASSERT_TRUE(SendAttIndicationCid(g_attConnectCtx.connectHandle, ATT_LE_CID, 0x0010, result));
    EXPECT_EQ(result, BT_SUCCESS);

    ASSERT_TRUE(g_attServerConfCtx.Wait()) << "no confirmation event within timeout";
    EXPECT_EQ(g_attServerConfCtx.event, ATT_HANDLE_VALUE_CONFIRMATION_ID);
}

// PSM 0x0027 is normally held by the mock EATT service (TakeOverEattPsm). Hand it
// to the ATT layer so ECRED connect events build real EATT bearer slots
// (AttEattConnected), and restore the mock afterwards.
static void UseAttEattService()
{
    TakeOverEattPsm();
    AttEattRegisterService();
}

static void RestoreMockEattService()
{
    TakeOverEattPsm();
    g_mockRegistered = false;
    EnsureMockEattService();
}

// Scope guard: PSM -> ATT on entry; drop the link and restore the mock on exit,
// even when an ASSERT aborts the test body.
class EattAttServiceGuard {
public:
    EattAttServiceGuard()
    {
        UseAttEattService();
    }

    ~EattAttServiceGuard()
    {
        ResetConnection();
        RestoreMockEattService();
    }
};

struct EattEstablishCtx : CallbackWaiter {
    int result = -1;
    uint16_t lcids[2] = { 0 };
    uint16_t n = 0;
};

static void OnEattEstablish(int result, const uint16_t *lcids, uint16_t n, void *ctx)
{
    EattEstablishCtx *c = static_cast<EattEstablishCtx *>(ctx);
    c->result = result;
    c->n = n;
    size_t k = (n > 2) ? 2 : n;
    for (size_t i = 0; i < k; ++i) {
        c->lcids[i] = lcids[i];
    }
    c->Notify();
}

// AttEattConnected runs on the L2CAP queue after the 0x17/0x18 exchange; poll the
// bearer slot until it appears (short timeout), then return its AttConnectInfo.
// The slot is written on the L2CAP queue, so the lookup is issued as a queued task
// (CallL2cap) instead of a lock-free test-thread read of the global connect table
// (data race, review m18); the sleep keeps the polling bounded as before.
constexpr uint32_t ATT_SLOT_POLL_INTERVAL_MS = 50;

struct EattSlotCheckArg {
    uint16_t connectHandle = 0;
    uint16_t lcid = 0;
    AttConnectInfo *connect = nullptr;
};

int EattSlotCheckFn(void *arg)
{
    EattSlotCheckArg *a = static_cast<EattSlotCheckArg *>(arg);
    a->connect = AttGetConnectInfoByConnectHandleAndLeCid(a->connectHandle, a->lcid);
    return (a->connect != nullptr) ? BT_SUCCESS : BT_BAD_STATUS;
}

struct EattPreferPickArg {
    uint16_t connectHandle = 0;
    AttConnectInfo *picked = nullptr;
};

// AttGetConnectInfoByConnectHandlePreferEattInd reads the connect table and serverSendFlag,
// both owned by the ATT/L2CAP queue; issue it as a queued call like EattSlotCheckFn instead
// of a lock-free test-thread read (data race, review m18).
int EattPreferPickFn(void *arg)
{
    EattPreferPickArg *a = static_cast<EattPreferPickArg *>(arg);
    a->picked = AttGetConnectInfoByConnectHandlePreferEattInd(a->connectHandle);
    return (a->picked != nullptr) ? BT_SUCCESS : BT_BAD_STATUS;
}

struct EattSlotFlagArg {
    AttConnectInfo *connect = nullptr;
    bool value = false;
};

// serverSendFlag is written and read on the ATT/L2CAP queue (indication send / confirmation
// handling); set it through the queue so the write is ordered with those readers (m18 pattern).
int EattSlotSetFlagFn(void *arg)
{
    EattSlotFlagArg *a = static_cast<EattSlotFlagArg *>(arg);
    a->connect->serverSendFlag = a->value;
    return BT_SUCCESS;
}

int EattSlotGetFlagFn(void *arg)
{
    EattSlotFlagArg *a = static_cast<EattSlotFlagArg *>(arg);
    a->value = a->connect->serverSendFlag;
    return BT_SUCCESS;
}

static bool WaitEattSlot(uint16_t connectHandle, uint16_t lcid, AttConnectInfo **out, uint32_t timeoutMs = 3000)
{
    uint32_t waited = 0;
    while (waited < timeoutMs) {
        EattSlotCheckArg arg = { };
        arg.connectHandle = connectHandle;
        arg.lcid = lcid;
        if (CallL2cap(EattSlotCheckFn, &arg) == BT_SUCCESS && arg.connect != nullptr) {
            *out = arg.connect;
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(ATT_SLOT_POLL_INTERVAL_MS));
        waited += ATT_SLOT_POLL_INTERVAL_MS;
    }
    *out = nullptr;
    return false;
}

struct EattEstablishArg {
    uint16_t connectHandle = 0;
    const L2capLeConfigInfo *cfg = nullptr;
    uint16_t n = 0;
    void (*cb)(int result, const uint16_t *lcids, uint16_t n, void *ctx) = nullptr;
    void *ctx = nullptr;
};

// AttEattEstablish lock-free-reads the ATT connect table and writes connect->eattEstablishCb,
// both owned by the ATT/L2CAP queue (the 0x18 settlement callback runs there); its documented
// contract requires invoking it from the Stack thread, never from a foreign thread. Issue it
// as a queued call like EattSlotCheckFn instead of a direct test-thread call (data race,
// review m18 / v3 Major-1). The 0x18 result still arrives later on the same queue and wakes
// EattEstablishCtx::Wait on the test thread.
int EattEstablishFn(void *arg)
{
    EattEstablishArg *a = static_cast<EattEstablishArg *>(arg);
    return AttEattEstablish(a->connectHandle, a->cfg, a->n, a->cb, a->ctx);
}

// Establish EATT channels through the ATT layer, retrying once on a rejection: the
// responder's security check needs its HCI 0x08 encryption-change event processed
// before the 0x17 lands, an ordering that is peer-side and cannot be polled from
// this board (same strategy as EstablishEattChannels).
static bool EstablishEattViaAtt(uint16_t connectHandle, L2capLeConfigInfo *cfg, uint16_t n, EattEstablishCtx *ectx)
{
    for (int attempt = 0; attempt < EATT_REQ_MAX_ATTEMPTS; attempt++) {
        ectx->Reset();
        EattEstablishArg arg = { };
        arg.connectHandle = connectHandle;
        arg.cfg = cfg;
        arg.n = n;
        arg.cb = OnEattEstablish;
        arg.ctx = ectx;
        // The establishment itself runs on the L2CAP queue (the lock-free contract above);
        // CallL2cap blocks until the slot completes, and the callback follows later from the
        // same queue when the 0x18 lands.
        int ret = CallL2cap(EattEstablishFn, &arg);
        if (ret != BT_SUCCESS) {
            printf("EstablishEattViaAtt: AttEattEstablish = %d\n", ret);
            return false;
        }
        if (!ectx->Wait()) {
            printf("EstablishEattViaAtt: wait establish result timeout\n");
            return false;
        }
        if (ectx->result != BT_OPERATION_FAILED) {
            return true;
        }
        printf("EstablishEattViaAtt: establishment rejected, retrying\n");
    }
    return false;
}

/**
 * @tc.number: StackAtt52_EattPreferIndMultiEatt_01200
 * @tc.name: PreferEattInd picks the idle EATT bearer among several (O3)
 * @tc.desc: build two real EATT bearers via 0x17/0x18, then assert the selection
 *           helper skips a bearer with a pending indication and falls back to UATT
 *           when every EATT bearer is busy (Vol 3 Part F 3.3.2)
 */
// Scenario of StackAtt52_EattPreferIndMultiEatt_01200: exercise the PreferEattInd
// selection with two real EATT bearers - idle pick, busy-bearer skip, UATT fallback,
// and re-pick after the first bearer is freed (Vol 3 Part F 3.3.2).
static void EattPreferIndMultiEattScenario(uint16_t connectHandle, AttConnectInfo *eatt1, AttConnectInfo *eatt2)
{
    // both idle: PreferEattInd picks one of the two EATT bearers (queued calls, see
    // EattPreferPickFn: the connect table and the flags are owned by the ATT/L2CAP queue)
    EattPreferPickArg pick = { };
    pick.connectHandle = connectHandle;
    ASSERT_TRUE(CallL2cap(EattPreferPickFn, &pick) == BT_SUCCESS && pick.picked != nullptr);
    AttConnectInfo *first = pick.picked;
    ASSERT_TRUE(first == eatt1 || first == eatt2);

    // that bearer now has a pending indication: the other EATT bearer is picked
    EattSlotFlagArg flag = { };
    flag.connect = first;
    flag.value = true;
    ASSERT_EQ(CallL2cap(EattSlotSetFlagFn, &flag), BT_SUCCESS);
    pick.picked = nullptr;
    ASSERT_TRUE(CallL2cap(EattPreferPickFn, &pick) == BT_SUCCESS && pick.picked != nullptr);
    AttConnectInfo *second = pick.picked;
    ASSERT_NE(second, first);
    ASSERT_TRUE(second == eatt1 || second == eatt2);

    // both EATT bearers busy: fall back to the UATT bearer
    flag.connect = second;
    flag.value = true;
    ASSERT_EQ(CallL2cap(EattSlotSetFlagFn, &flag), BT_SUCCESS);
    EattSlotCheckArg uattArg = { };
    uattArg.connectHandle = connectHandle;
    uattArg.lcid = LE_CID;
    ASSERT_TRUE(CallL2cap(EattSlotCheckFn, &uattArg) == BT_SUCCESS && uattArg.connect != nullptr);
    AttConnectInfo *uatt = uattArg.connect;
    pick.picked = nullptr;
    ASSERT_TRUE(CallL2cap(EattPreferPickFn, &pick) == BT_SUCCESS);
    EXPECT_EQ(pick.picked, uatt);

    // free the first one again: it is picked again
    flag.connect = first;
    flag.value = false;
    ASSERT_EQ(CallL2cap(EattSlotSetFlagFn, &flag), BT_SUCCESS);
    pick.picked = nullptr;
    ASSERT_TRUE(CallL2cap(EattPreferPickFn, &pick) == BT_SUCCESS);
    EXPECT_EQ(pick.picked, first);
}

HWTEST_F(StackAtt52Test, StackAtt52_EattPreferIndMultiEatt_01200, TestSize.Level1)
{
    if (!EnsureAttConnected()) {
        GTEST_SKIP() << "peer not connected (pass peer addr as BT52_PEER_ADDR, start "
                        "server mode on device B)";
    }

    // EATT needs an encrypted link (Vol 3 Part G 5.3.2): pair before establishing.
    if (!PairWithPeer()) {
        GTEST_SKIP() << "pairing with the peer failed";
    }

    // PSM -> ATT for real bearer slots; restored + link dropped on scope exit.
    EattAttServiceGuard psmGuard;

    L2capLeConfigInfo cfg = { };
    cfg.mtu = 247;
    cfg.mps = 247;
    cfg.credit = 8;
    EattEstablishCtx ectx;
    ASSERT_TRUE(EstablishEattViaAtt(g_attConnectCtx.connectHandle, &cfg, EATT_CHANNEL_COUNT, &ectx));
    ASSERT_EQ(ectx.result, BT_SUCCESS);
    ASSERT_EQ(ectx.n, EATT_CHANNEL_COUNT);

    AttConnectInfo *eatt1 = nullptr;
    AttConnectInfo *eatt2 = nullptr;
    ASSERT_TRUE(WaitEattSlot(g_attConnectCtx.connectHandle, ectx.lcids[0], &eatt1));
    ASSERT_TRUE(WaitEattSlot(g_attConnectCtx.connectHandle, ectx.lcids[1], &eatt2));
    ASSERT_NE(eatt1, nullptr);
    ASSERT_NE(eatt2, nullptr);

    EattPreferIndMultiEattScenario(g_attConnectCtx.connectHandle, eatt1, eatt2);
}

/**
 * @tc.number: StackAtt52_EattPreferIndRealSend_01210
 * @tc.name: real cid=0 indications pick idle EATT bearers, then fall back to UATT
 * @tc.desc: with the peer in BT52_EATT_NO_CONF mode (EATT channels never confirm),
 *           three send-allowed indications land on the two EATT bearers then UATT;
 *           each send keeps serverSendFlag set on its bearer (no 0x0E on EATT)
 *           (Vol 3 Part F 3.3.2 indication order protocol / 3.4.7.2)
 */
// Scenario of StackAtt52_EattPreferIndRealSend_01210: three real cid=0 indication sends
// with the peer in BT52_EATT_NO_CONF mode land on the two EATT bearers then UATT; each
// send keeps serverSendFlag set on its bearer (no 0x0E on EATT), and the UATT send is
// the only one the peer confirms (Vol 3 Part F 3.3.2 indication order protocol).
static void EattPreferIndRealSendScenario(
    uint16_t connectHandle, AttConnectInfo *eatt1, AttConnectInfo *eatt2, int &result)
{
    // send #1: PreferEattInd picks the first idle EATT bearer, its flag is set
    ASSERT_TRUE(SendAttIndication(connectHandle, 0x0010, result));
    EXPECT_EQ(result, BT_SUCCESS);
    // queued flag reads (EattSlotGetFlagFn): serverSendFlag is owned by the ATT/L2CAP queue
    EattSlotFlagArg flag = { };
    flag.connect = eatt1;
    ASSERT_EQ(CallL2cap(EattSlotGetFlagFn, &flag), BT_SUCCESS);
    int busy = flag.value ? 1 : 0;
    flag.connect = eatt2;
    ASSERT_EQ(CallL2cap(EattSlotGetFlagFn, &flag), BT_SUCCESS);
    busy += flag.value ? 1 : 0;
    EXPECT_EQ(busy, 1);

    // send #2: the busy bearer is skipped, the other EATT bearer is picked
    ASSERT_TRUE(SendAttIndication(connectHandle, 0x0010, result));
    EXPECT_EQ(result, BT_SUCCESS);
    flag.connect = eatt1;
    ASSERT_EQ(CallL2cap(EattSlotGetFlagFn, &flag), BT_SUCCESS);
    busy = flag.value ? 1 : 0;
    flag.connect = eatt2;
    ASSERT_EQ(CallL2cap(EattSlotGetFlagFn, &flag), BT_SUCCESS);
    busy += flag.value ? 1 : 0;
    EXPECT_EQ(busy, EATT_CHANNEL_COUNT);

    // send #3: every EATT bearer busy -> fall back to UATT; the peer confirms there
    g_attServerConfCtx.Reset();
    ASSERT_TRUE(SendAttIndication(connectHandle, 0x0010, result));
    EXPECT_EQ(result, BT_SUCCESS);
    ASSERT_TRUE(g_attServerConfCtx.Wait()) << "no confirmation for the UATT indication";
    EXPECT_EQ(g_attServerConfCtx.event, ATT_HANDLE_VALUE_CONFIRMATION_ID);

    // EATT bearers stayed busy: the peer never echoed a confirmation on them
    flag.connect = eatt1;
    ASSERT_EQ(CallL2cap(EattSlotGetFlagFn, &flag), BT_SUCCESS);
    EXPECT_TRUE(flag.value);
    flag.connect = eatt2;
    ASSERT_EQ(CallL2cap(EattSlotGetFlagFn, &flag), BT_SUCCESS);
    EXPECT_TRUE(flag.value);
}

HWTEST_F(StackAtt52Test, StackAtt52_EattPreferIndRealSend_01210, TestSize.Level1)
{
    // The third send must fall back to UATT, which only happens when the peer runs
    // in BT52_EATT_NO_CONF mode (InitEattPeerSetup): the EATT channels then never
    // confirm, so serverSendFlag stays set on both EATT bearers. Without that mode
    // the peer echoes the indications on EATT, the flags are cleared and the send
    // does not land on UATT — the assertions below would fail with no hint why.
    if (std::getenv("BT52_EATT_NO_CONF") == nullptr) {
        GTEST_SKIP() << "peer must run with BT52_EATT_NO_CONF (EATT channels never confirm)";
    }

    if (!EnsureAttConnected()) {
        GTEST_SKIP() << "peer not connected (pass peer addr as BT52_PEER_ADDR, start "
                        "server mode on device B)";
    }

    // EATT needs an encrypted link (Vol 3 Part G 5.3.2): pair before establishing.
    if (!PairWithPeer()) {
        GTEST_SKIP() << "pairing with the peer failed";
    }

    // PSM -> ATT for real bearer slots; restored + link dropped on scope exit.
    EattAttServiceGuard psmGuard;

    L2capLeConfigInfo cfg = { };
    cfg.mtu = 247;
    cfg.mps = 247;
    cfg.credit = 8;
    EattEstablishCtx ectx;
    ASSERT_TRUE(EstablishEattViaAtt(g_attConnectCtx.connectHandle, &cfg, EATT_CHANNEL_COUNT, &ectx));
    ASSERT_EQ(ectx.result, BT_SUCCESS);
    ASSERT_EQ(ectx.n, EATT_CHANNEL_COUNT);

    AttConnectInfo *eatt1 = nullptr;
    AttConnectInfo *eatt2 = nullptr;
    ASSERT_TRUE(WaitEattSlot(g_attConnectCtx.connectHandle, ectx.lcids[0], &eatt1));
    ASSERT_TRUE(WaitEattSlot(g_attConnectCtx.connectHandle, ectx.lcids[1], &eatt2));
    ASSERT_NE(eatt1, nullptr);
    ASSERT_NE(eatt2, nullptr);

    int result = 0;
    EattPreferIndRealSendScenario(g_attConnectCtx.connectHandle, eatt1, eatt2, result);
}

} // namespace Bluetooth
} // namespace OHOS

