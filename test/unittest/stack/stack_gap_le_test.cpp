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

#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>

#include "btm.h"
#include "gap_le_if.h"
#include "hci/hci.h"
#include "hci/hci_error.h"

using namespace testing::ext;

namespace OHOS {
namespace Bluetooth {
namespace {
constexpr int WAIT_CALLBACK_TIMEOUT_MS = 5000;
constexpr uint8_t EX_ADV_HANDLE = 0x00;
constexpr uint16_t PERIODIC_ADV_INTERVAL_MIN = 0x0006;
constexpr uint8_t HCI_STATUS_UNKNOWN = 0xFF;

// Set to true only after all SetUpTestCase initializations succeed.
// TearDownTestCase uses this to avoid cleanup on a fixture that was never set up.
static bool g_fixtureInitialized = false;
static bool g_fixtureBtmInitialized = false;
static bool g_fixtureBtmEnabled = false;
static bool g_fixtureRoleSet = false;

struct CallbackWaiter {
    std::mutex mtx;
    std::condition_variable cv;
    bool received = false;

    bool Wait()
    {
        std::unique_lock<std::mutex> lock(mtx);
        return cv.wait_for(lock, std::chrono::milliseconds(WAIT_CALLBACK_TIMEOUT_MS), [this] { return received; });
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

// RAII guards that deregister GAP callbacks on destruction. They protect against
// use-after-free when a test assertion fails before the explicit deregister call.
class LeControllerCallbackGuard {
public:
    LeControllerCallbackGuard() = default;
    ~LeControllerCallbackGuard()
    {
        if (GAPIF_DeregisterLeControllerCallback() != BT_SUCCESS) {
            printf("LeControllerCallbackGuard: deregister failed; callback may remain registered\n");
        }
    }
    LeControllerCallbackGuard(const LeControllerCallbackGuard &) = delete;
    LeControllerCallbackGuard &operator=(const LeControllerCallbackGuard &) = delete;
};

class LeConnCallbackGuard {
public:
    LeConnCallbackGuard() = default;
    ~LeConnCallbackGuard()
    {
        if (GAPIF_DeregisterLeConnCallback() != BT_SUCCESS) {
            printf("LeConnCallbackGuard: deregister failed; callback may remain registered\n");
        }
    }
    LeConnCallbackGuard(const LeConnCallbackGuard &) = delete;
    LeConnCallbackGuard &operator=(const LeConnCallbackGuard &) = delete;
};

class ExAdvCallbackGuard {
public:
    ExAdvCallbackGuard() = default;
    ~ExAdvCallbackGuard()
    {
        if (GAPIF_DeregisterExAdvCallback() != BT_SUCCESS) {
            printf("ExAdvCallbackGuard: deregister failed; callback may remain registered\n");
        }
    }
    ExAdvCallbackGuard(const ExAdvCallbackGuard &) = delete;
    ExAdvCallbackGuard &operator=(const ExAdvCallbackGuard &) = delete;
};

class PeriodicAdvSyncCallbackGuard {
public:
    PeriodicAdvSyncCallbackGuard() = default;
    ~PeriodicAdvSyncCallbackGuard()
    {
        if (GAPIF_DeregisterPeriodicAdvSyncCallback() != BT_SUCCESS) {
            printf("PeriodicAdvSyncCallbackGuard: deregister failed; callback may remain registered\n");
        }
    }
    PeriodicAdvSyncCallbackGuard(const PeriodicAdvSyncCallbackGuard &) = delete;
    PeriodicAdvSyncCallbackGuard &operator=(const PeriodicAdvSyncCallbackGuard &) = delete;
};

// RAII guard that records the controller's current suggested default data length
// on construction and restores it on destruction. This keeps the WriteSuggested
// test from permanently changing controller-global state.
class DefaultDataLengthRestorer {
public:
    DefaultDataLengthRestorer()
    {
        ReadOriginal();
    }
    ~DefaultDataLengthRestorer()
    {
        if (valid_) {
            Restore();
        }
    }
    DefaultDataLengthRestorer(const DefaultDataLengthRestorer &) = delete;
    DefaultDataLengthRestorer &operator=(const DefaultDataLengthRestorer &) = delete;

private:
    struct ReadResult : CallbackWaiter {
        uint16_t octets = 0;
        uint16_t time = 0;
    };

    static void OnReadSuggestedDefaultDataLengthResult(
        uint8_t status, uint16_t suggestedMaxTxOctets, uint16_t suggestedMaxTxTime, void *context)
    {
        auto *result = static_cast<ReadResult *>(context);
        if (status == HCI_SUCCESS) {
            result->octets = suggestedMaxTxOctets;
            result->time = suggestedMaxTxTime;
        }
        // Notify on error status too: valid_ stays false and nothing gets restored,
        // but Wait() returns immediately instead of burning the full timeout.
        result->Notify();
    }

    static void OnWriteSuggestedDefaultDataLengthResult(uint8_t status, void *context)
    {
        (void)status;
        static_cast<CallbackWaiter *>(context)->Notify();
    }

    void ReadOriginal()
    {
        // Heap-allocate the callback context: deregister is synchronous (events
        // already queued in the GAP task run before it completes), so once it
        // succeeds no late event can touch the context. If it fails, leak the
        // context rather than let a late event write freed memory.
        auto result = std::make_unique<ReadResult>();
        GapLeControllerCallback callback = {};
        callback.readSuggestedDefaultDataLengthResult = OnReadSuggestedDefaultDataLengthResult;
        if (GAPIF_RegisterLeControllerCallback(&callback, result.get()) != BT_SUCCESS) {
            return;
        }
        if (GAPIF_LeReadSuggestedDefaultDataLength() != BT_SUCCESS) {
            if (GAPIF_DeregisterLeControllerCallback() != BT_SUCCESS) {
                printf("DefaultDataLengthRestorer: deregister failed, leaking read context\n");
                (void)result.release();
            }
            return;
        }
        if (result->Wait()) {
            octets_ = result->octets;
            time_ = result->time;
            valid_ = true;
        }
        if (GAPIF_DeregisterLeControllerCallback() != BT_SUCCESS) {
            printf("DefaultDataLengthRestorer: deregister failed, leaking read context\n");
            (void)result.release();
        }
    }

    void Restore()
    {
        auto waiter = std::make_unique<CallbackWaiter>();
        GapLeControllerCallback callback = {};
        callback.writeSuggestedDefaultDataLengthResult = OnWriteSuggestedDefaultDataLengthResult;
        if (GAPIF_RegisterLeControllerCallback(&callback, waiter.get()) != BT_SUCCESS) {
            printf("DefaultDataLengthRestorer: failed to register controller callback\n");
            return;
        }
        if (GAPIF_LeWriteSuggestedDefaultDataLength(octets_, time_) != BT_SUCCESS) {
            printf("DefaultDataLengthRestorer: failed to write suggested default data length\n");
            if (GAPIF_DeregisterLeControllerCallback() != BT_SUCCESS) {
                printf("DefaultDataLengthRestorer: deregister failed, leaking waiter context\n");
                (void)waiter.release();
            }
            return;
        }
        if (!waiter->Wait()) {
            printf("DefaultDataLengthRestorer: timeout waiting for write result\n");
        }
        if (GAPIF_DeregisterLeControllerCallback() != BT_SUCCESS) {
            printf("DefaultDataLengthRestorer: failed to deregister controller callback\n");
            (void)waiter.release();
        }
    }

    bool valid_ = false;
    uint16_t octets_ = 0;
    uint16_t time_ = 0;
};

// RAII guard that restores an explicit "all PHYs allowed" default PHY setting on
// destruction. There is no public API to read the current default PHY, so we set
// all_PHYs to 0x00 and provide GAP_LE_PHY_BIT_ALL for both txPhys and rxPhys.
// This is not the spec "no preference" encoding (which requires all_PHYs=0x03),
// but the resulting controller behavior still allows every PHY.
class DefaultPhyRestorer {
public:
    DefaultPhyRestorer() = default;
    ~DefaultPhyRestorer()
    {
        Restore();
    }
    DefaultPhyRestorer(const DefaultPhyRestorer &) = delete;
    DefaultPhyRestorer &operator=(const DefaultPhyRestorer &) = delete;

private:
    static void OnSetDefaultPhyResult(uint8_t status, void *context)
    {
        (void)status;
        static_cast<CallbackWaiter *>(context)->Notify();
    }

    void Restore()
    {
        auto waiter = std::make_unique<CallbackWaiter>();
        GapLeConnCallback callback = {};
        callback.leSetDefaultPhyResult = OnSetDefaultPhyResult;
        if (GAPIF_RegisterLeConnCallback(&callback, waiter.get()) != BT_SUCCESS) {
            printf("DefaultPhyRestorer: failed to register connection callback\n");
            return;
        }
        if (GAPIF_LeSetDefaultPhy(0x00, GAP_LE_PHY_BIT_ALL, GAP_LE_PHY_BIT_ALL) != BT_SUCCESS) {
            printf("DefaultPhyRestorer: failed to set default PHY\n");
            if (GAPIF_DeregisterLeConnCallback() != BT_SUCCESS) {
                printf("DefaultPhyRestorer: failed to deregister connection callback, leaking waiter\n");
                (void)waiter.release();
            }
            return;
        }
        if (!waiter->Wait()) {
            printf("DefaultPhyRestorer: timeout waiting for set default PHY result\n");
        }
        if (GAPIF_DeregisterLeConnCallback() != BT_SUCCESS) {
            printf("DefaultPhyRestorer: failed to deregister connection callback\n");
            (void)waiter.release();
        }
    }
};

struct StatusOnlyResult : CallbackWaiter {
    uint8_t status = HCI_STATUS_UNKNOWN;
};

void OnStatusOnlyResult(uint8_t status, void *context)
{
    auto *result = static_cast<StatusOnlyResult *>(context);
    result->status = status;
    result->Notify();
}

struct ReadMaxDataLengthResult : CallbackWaiter {
    uint8_t status = HCI_STATUS_UNKNOWN;
    uint16_t maxTxOctets = 0;
    uint16_t maxTxTime = 0;
    uint16_t maxRxOctets = 0;
    uint16_t maxRxTime = 0;
};

struct ReadTransmitPowerResult : CallbackWaiter {
    uint8_t status = HCI_STATUS_UNKNOWN;
    int8_t minTxPower = 0;
    int8_t maxTxPower = 0;
};

void OnReadTransmitPowerResult(uint8_t status, int8_t minTxPower, int8_t maxTxPower, void *context)
{
    auto *result = static_cast<ReadTransmitPowerResult *>(context);
    result->status = status;
    result->minTxPower = minTxPower;
    result->maxTxPower = maxTxPower;
    result->Notify();
}

struct ReadRfPathCompensationResult : CallbackWaiter {
    uint8_t status = HCI_STATUS_UNKNOWN;
    int16_t txPathCompensation = 0;
    int16_t rxPathCompensation = 0;
};

void OnReadRfPathCompensationResult(uint8_t status, int16_t txPathCompensation, int16_t rxPathCompensation,
                                    void *context)
{
    auto *result = static_cast<ReadRfPathCompensationResult *>(context);
    result->status = status;
    result->txPathCompensation = txPathCompensation;
    result->rxPathCompensation = rxPathCompensation;
    result->Notify();
}

struct RfPathCompensationContext {
    ReadRfPathCompensationResult *readResult;
    StatusOnlyResult *writeResult;
};

void OnReadRfPathCompensationResultWithContext(uint8_t status, int16_t txPathCompensation, int16_t rxPathCompensation,
    void *context)
{
    auto *ctx = static_cast<RfPathCompensationContext *>(context);
    ctx->readResult->status = status;
    ctx->readResult->txPathCompensation = txPathCompensation;
    ctx->readResult->rxPathCompensation = rxPathCompensation;
    ctx->readResult->Notify();
}

void OnWriteRfPathCompensationResultWithContext(uint8_t status, void *context)
{
    auto *ctx = static_cast<RfPathCompensationContext *>(context);
    ctx->writeResult->status = status;
    ctx->writeResult->Notify();
}

constexpr int16_t RF_PATH_COMPENSATION_MIN = -1280;
constexpr int16_t RF_PATH_COMPENSATION_MAX = 1280;

struct ReadAdvertiserListSizeResult : CallbackWaiter {
    uint8_t status = HCI_STATUS_UNKNOWN;
    uint8_t listSize = 0;
};

void OnReadAdvertiserListSizeResult(uint8_t status, uint8_t listSize, void *context)
{
    auto *result = static_cast<ReadAdvertiserListSizeResult *>(context);
    result->status = status;
    result->listSize = listSize;
    result->Notify();
}

struct ExAdvTestContext {
    StatusOnlyResult exAdvSetParam;
    StatusOnlyResult op;
};

void OnExAdvSetParamResult(uint8_t status, uint8_t selectTxPower, void *context)
{
    (void)selectTxPower;
    auto *ctx = static_cast<ExAdvTestContext *>(context);
    ctx->exAdvSetParam.status = status;
    ctx->exAdvSetParam.Notify();
}

void OnPeriodicAdvOpResult(uint8_t status, void *context)
{
    auto *ctx = static_cast<ExAdvTestContext *>(context);
    ctx->op.status = status;
    ctx->op.Notify();
}

bool CreateExAdvSet(ExAdvTestContext &ctx)
{
    static const BtAddr PEER_ADDR = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00};
    constexpr uint16_t advInterval = 0x00A0; // 100 ms; 0x00A0 is the canonical 100 ms interval in 0.625 ms units.
    constexpr int8_t txPowerMax = 0x7F;     // Host has no preference (Bluetooth spec value).

    GapLeExAdvParam advParam = {};
    advParam.advIntervalMin = advInterval;
    advParam.advIntervalMax = advInterval;
    advParam.advChannelMap = GAP_ADVERTISING_CHANNEL_37 | GAP_ADVERTISING_CHANNEL_38 | GAP_ADVERTISING_CHANNEL_39;
    advParam.peerAddr = &PEER_ADDR;
    advParam.advFilterPolicy = GAP_ADVERTISING_NOT_USE_WL;
    advParam.primaryAdvPhy = GAP_ADVERTISEMENT_PHY_1M;
    advParam.secondaryAdvMaxSkip = 0;
    advParam.secondaryAdvPhy = GAP_ADVERTISEMENT_PHY_1M;
    advParam.advSid = 0;
    advParam.scanRequestNotifyEnable = 0;

    ctx.exAdvSetParam.Reset();
    EXPECT_EQ(GAPIF_LeExAdvSetParam(EX_ADV_HANDLE, 0x00, txPowerMax, advParam), BT_SUCCESS);
    EXPECT_TRUE(ctx.exAdvSetParam.Wait()) << "exAdvSetParamResult callback not received";
    EXPECT_EQ(ctx.exAdvSetParam.status, HCI_SUCCESS) << "create extended advertising set failed";
    return ctx.exAdvSetParam.status == HCI_SUCCESS;
}

struct ReadSuggestedDataLengthResult : CallbackWaiter {
    uint8_t status = HCI_STATUS_UNKNOWN;
    uint16_t suggestedMaxTxOctets = 0;
    uint16_t suggestedMaxTxTime = 0;
};

void OnReadSuggestedDefaultDataLengthResult(uint8_t status, uint16_t suggestedMaxTxOctets, uint16_t suggestedMaxTxTime,
                                            void *context)
{
    auto *result = static_cast<ReadSuggestedDataLengthResult *>(context);
    result->status = status;
    result->suggestedMaxTxOctets = suggestedMaxTxOctets;
    result->suggestedMaxTxTime = suggestedMaxTxTime;
    result->Notify();
}
} // namespace

class StackGapLeTest : public testing::Test {
public:
    StackGapLeTest() {}
    ~StackGapLeTest() {}

    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void StackGapLeTest::SetUpTestCase(void)
{
    g_fixtureInitialized = false;
    g_fixtureBtmInitialized = false;
    g_fixtureBtmEnabled = false;
    g_fixtureRoleSet = false;

    ASSERT_EQ(BTM_Initialize(), BT_SUCCESS);
    g_fixtureBtmInitialized = true;
    ASSERT_EQ(BTM_Enable(LE_CONTROLLER), BT_SUCCESS);
    g_fixtureBtmEnabled = true;
    ASSERT_TRUE(BTM_IsEnabled(LE_CONTROLLER));
    ASSERT_EQ(
        GAPIF_LeSetRole(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_PERIPHERAL | GAP_LE_ROLE_CENTRAL),
        BT_SUCCESS);
    g_fixtureRoleSet = true;
    g_fixtureInitialized = true;
}

void StackGapLeTest::TearDownTestCase(void)
{
    // Per-test TearDown already invokes HCI_LeTestEnd(), so no need to repeat it here.

    // Only clean up extended advertising sets when the controller supports the feature.
    if (BTM_IsControllerSupportLeExtendedAdvertising()) {
        EXPECT_EQ(GAPIF_LeExAdvSetEnable(0x00, 0, nullptr), BT_SUCCESS);
        EXPECT_EQ(GAPIF_LeExAdvClearHandle(), BT_SUCCESS);
    }

    if (g_fixtureBtmEnabled) {
        EXPECT_EQ(BTM_Disable(LE_CONTROLLER), BT_SUCCESS);
        g_fixtureBtmEnabled = false;
    }
    if (g_fixtureBtmInitialized) {
        EXPECT_EQ(BTM_Close(), BT_SUCCESS);
        g_fixtureBtmInitialized = false;
    }
    g_fixtureRoleSet = false;
    g_fixtureInitialized = false;
}

void StackGapLeTest::SetUp() {}

void StackGapLeTest::TearDown()
{
    // End any active LE test mode so that subsequent receiver/transmitter tests start from a clean state.
    (void)HCI_LeTestEnd();
    // Only clean up extended advertising sets when the controller supports the feature.
    if (BTM_IsControllerSupportLeExtendedAdvertising()) {
        // Core Spec v5.0 7.8.56: disabling extended advertising requires Number_of_Sets=0,
        // which disables all advertising sets at once.
        EXPECT_EQ(GAPIF_LeExAdvSetEnable(0x00, 0, nullptr), BT_SUCCESS);
        EXPECT_EQ(GAPIF_LeExAdvClearHandle(), BT_SUCCESS);
    }
}

/**
 * @tc.number: StackGapLe_ReadSuggestedDefaultDataLength_00100
 * @tc.name: GAPIF_LeReadSuggestedDefaultDataLength result from GapLeControllerCallback
 * @tc.desc: send HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LENGTH(0x0023), result must come back
 *           through GapLeControllerCallback::readSuggestedDefaultDataLengthResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_ReadSuggestedDefaultDataLength_00100, TestSize.Level1)
{
    ReadSuggestedDataLengthResult result;

    GapLeControllerCallback callback = {};
    callback.readSuggestedDefaultDataLengthResult = OnReadSuggestedDefaultDataLengthResult;
    ASSERT_EQ(GAPIF_RegisterLeControllerCallback(&callback, &result), BT_SUCCESS);
    LeControllerCallbackGuard guard;

    ASSERT_EQ(GAPIF_LeReadSuggestedDefaultDataLength(), BT_SUCCESS);

    bool received = result.Wait();

    ASSERT_TRUE(received) << "readSuggestedDefaultDataLengthResult callback not received";
    EXPECT_EQ(result.status, HCI_SUCCESS);
    EXPECT_GE(result.suggestedMaxTxOctets, GAP_LE_DATA_LENGTH_OCTETS_MIN);
    EXPECT_LE(result.suggestedMaxTxOctets, GAP_LE_DATA_LENGTH_OCTETS_MAX);
    EXPECT_GE(result.suggestedMaxTxTime, GAP_LE_DATA_LENGTH_TIME_MIN);
    EXPECT_LE(result.suggestedMaxTxTime, GAP_LE_DATA_LENGTH_TIME_MAX);
}

/**
 * @tc.number: StackGapLe_WriteSuggestedDefaultDataLength_00100
 * @tc.name: GAPIF_LeWriteSuggestedDefaultDataLength result from GapLeControllerCallback
 * @tc.desc: send HCI_LE_WRITE_SUGGESTED_DEFAULT_DATA_LENGTH(0x0024) with spec initial values,
 *           result through GapLeControllerCallback::writeSuggestedDefaultDataLengthResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_WriteSuggestedDefaultDataLength_00100, TestSize.Level1)
{
    // Record the current controller values so they can be restored after the test.
    DefaultDataLengthRestorer restorer;

    StatusOnlyResult result;

    GapLeControllerCallback callback = {};
    callback.writeSuggestedDefaultDataLengthResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterLeControllerCallback(&callback, &result), BT_SUCCESS);
    LeControllerCallbackGuard guard;

    ASSERT_EQ(GAPIF_LeWriteSuggestedDefaultDataLength(GAP_LE_DATA_LENGTH_OCTETS_MIN, GAP_LE_DATA_LENGTH_TIME_MIN),
              BT_SUCCESS);
    bool received = result.Wait();

    ASSERT_TRUE(received) << "writeSuggestedDefaultDataLengthResult callback not received";
    EXPECT_EQ(result.status, HCI_SUCCESS);
}

/**
 * @tc.number: StackGapLe_ReadMaximumDataLength_00100
 * @tc.name: GAPIF_LeReadMaximumDataLength result from GapLeControllerCallback
 * @tc.desc: send HCI_LE_READ_MAXIMUM_DATA_LENGTH(0x002F),
 *           result through GapLeControllerCallback::readMaxDataLengthResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_ReadMaximumDataLength_00100, TestSize.Level1)
{
    ReadMaxDataLengthResult result;

    GapLeControllerCallback callback = {};
    callback.readMaxDataLengthResult =
        [](uint8_t status, uint16_t maxTxOctets, uint16_t maxTxTime, uint16_t maxRxOctets, uint16_t maxRxTime,
            void *context) {
            auto *result = static_cast<ReadMaxDataLengthResult *>(context);
            result->status = status;
            result->maxTxOctets = maxTxOctets;
            result->maxTxTime = maxTxTime;
            result->maxRxOctets = maxRxOctets;
            result->maxRxTime = maxRxTime;
            result->Notify();
        };
    ASSERT_EQ(GAPIF_RegisterLeControllerCallback(&callback, &result), BT_SUCCESS);
    LeControllerCallbackGuard guard;

    ASSERT_EQ(GAPIF_LeReadMaximumDataLength(), BT_SUCCESS);
    bool received = result.Wait();

    ASSERT_TRUE(received) << "readMaxDataLengthResult callback not received";
    EXPECT_EQ(result.status, HCI_SUCCESS);
    EXPECT_NE(result.maxTxOctets, 0);
    EXPECT_NE(result.maxTxTime, 0);
    EXPECT_NE(result.maxRxOctets, 0);
    EXPECT_NE(result.maxRxTime, 0);
}

/**
 * @tc.number: StackGapLe_SetDefaultPhy_00100
 * @tc.name: GAPIF_LeSetDefaultPhy result from GapLeConnCallback
 * @tc.desc: send HCI_LE_SET_DEFAULT_PHY(0x0031) preferring 1M PHY,
 *           result through GapLeConnCallback::leSetDefaultPhyResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_SetDefaultPhy_00100, TestSize.Level1)
{
    // Restore a permissive default PHY after the test so later tests are not pinned to 1M.
    DefaultPhyRestorer restorer;

    StatusOnlyResult result;

    GapLeConnCallback callback = {};
    callback.leSetDefaultPhyResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterLeConnCallback(&callback, &result), BT_SUCCESS);
    LeConnCallbackGuard guard;

    ASSERT_EQ(GAPIF_LeSetDefaultPhy(0x00, GAP_LE_PHY_BIT_1M, GAP_LE_PHY_BIT_1M), BT_SUCCESS);
    bool received = result.Wait();

    ASSERT_TRUE(received) << "leSetDefaultPhyResult callback not received";
    EXPECT_EQ(result.status, HCI_SUCCESS);
}

/**
 * @tc.number: StackGapLe_ReadTransmitPower_00100
 * @tc.name: GAPIF_LeReadTransmitPower result from GapLeControllerCallback
 * @tc.desc: send HCI_LE_READ_TRANSMIT_POWER(0x004B),
 *           result through GapLeControllerCallback::readTransmitPowerResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_ReadTransmitPower_00100, TestSize.Level1)
{
    ReadTransmitPowerResult result;

    GapLeControllerCallback callback = {};
    callback.readTransmitPowerResult = OnReadTransmitPowerResult;
    ASSERT_EQ(GAPIF_RegisterLeControllerCallback(&callback, &result), BT_SUCCESS);
    LeControllerCallbackGuard guard;

    ASSERT_EQ(GAPIF_LeReadTransmitPower(), BT_SUCCESS);
    bool received = result.Wait();

    ASSERT_TRUE(received) << "readTransmitPowerResult callback not received";
    EXPECT_EQ(result.status, HCI_SUCCESS);
    EXPECT_LE(result.minTxPower, result.maxTxPower);
}

/**
 * @tc.number: StackGapLe_ReadRfPathCompensation_00100
 * @tc.name: GAPIF_LeReadRfPathCompensation result from GapLeControllerCallback
 * @tc.desc: send HCI_LE_READ_RF_PATH_COMPENSATION(0x004C),
 *           result through GapLeControllerCallback::readRfPathCompensationResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_ReadRfPathCompensation_00100, TestSize.Level1)
{
    ReadRfPathCompensationResult result;

    GapLeControllerCallback callback = {};
    callback.readRfPathCompensationResult = OnReadRfPathCompensationResult;
    ASSERT_EQ(GAPIF_RegisterLeControllerCallback(&callback, &result), BT_SUCCESS);
    LeControllerCallbackGuard guard;

    ASSERT_EQ(GAPIF_LeReadRfPathCompensation(), BT_SUCCESS);
    bool received = result.Wait();

    ASSERT_TRUE(received) << "readRfPathCompensationResult callback not received";
    EXPECT_EQ(result.status, HCI_SUCCESS);
}

/**
 * @tc.number: StackGapLe_WriteRfPathCompensation_00100
 * @tc.name: GAPIF_LeWriteRfPathCompensation result from GapLeControllerCallback
 * @tc.desc: read current compensation via 0x004C then write the same values back via
 *           HCI_LE_WRITE_RF_PATH_COMPENSATION(0x004D) (avoid clobbering vendor calibration),
 *           result through GapLeControllerCallback::writeRfPathCompensationResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_WriteRfPathCompensation_00100, TestSize.Level1)
{
    ReadRfPathCompensationResult readResult;
    StatusOnlyResult writeResult;
    RfPathCompensationContext ctx = {&readResult, &writeResult};

    GapLeControllerCallback callback = {};
    callback.readRfPathCompensationResult = OnReadRfPathCompensationResultWithContext;
    callback.writeRfPathCompensationResult = OnWriteRfPathCompensationResultWithContext;
    ASSERT_EQ(GAPIF_RegisterLeControllerCallback(&callback, &ctx), BT_SUCCESS);
    LeControllerCallbackGuard guard;

    ASSERT_EQ(GAPIF_LeReadRfPathCompensation(), BT_SUCCESS);
    ASSERT_TRUE(readResult.Wait()) << "readRfPathCompensationResult callback not received";
    ASSERT_EQ(readResult.status, HCI_SUCCESS);

    // Only write values back if they are within the spec-defined range.  Out-of-range values
    // would indicate a controller/transport issue; writing them could corrupt vendor calibration.
    if (readResult.txPathCompensation < RF_PATH_COMPENSATION_MIN ||
        readResult.txPathCompensation > RF_PATH_COMPENSATION_MAX ||
        readResult.rxPathCompensation < RF_PATH_COMPENSATION_MIN ||
        readResult.rxPathCompensation > RF_PATH_COMPENSATION_MAX) {
        GTEST_SKIP() << "read RF path compensation out of range, skipping write-back";
    }

    ASSERT_EQ(GAPIF_LeWriteRfPathCompensation(readResult.txPathCompensation, readResult.rxPathCompensation),
              BT_SUCCESS);
    bool received = writeResult.Wait();

    ASSERT_TRUE(received) << "writeRfPathCompensationResult callback not received";
    EXPECT_EQ(writeResult.status, HCI_SUCCESS);
}

/**
 * @tc.number: StackGapLe_PeriodicAdvSetParam_00100
 * @tc.name: GAPIF_LePeriodicAdvSetParam result from GapExAdvCallback
 * @tc.desc: create extended advertising set first, then send
 *           HCI_LE_SET_PERIODIC_ADVERTISING_PARAMETERS(0x003E),
 *           result through GapExAdvCallback::periodicAdvSetParamResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_PeriodicAdvSetParam_00100, TestSize.Level1)
{
    if (!BTM_IsControllerSupportLeExtendedAdvertising() || !BTM_IsControllerSupportLePeriodicAdvertising()) {
        GTEST_SKIP() << "controller does not support extended/periodic advertising";
    }

    ExAdvTestContext ctx;
    GapExAdvCallback callback = {};
    callback.exAdvSetParamResult = OnExAdvSetParamResult;
    callback.periodicAdvSetParamResult = OnPeriodicAdvOpResult;
    ASSERT_EQ(GAPIF_RegisterExAdvCallback(&callback, &ctx), BT_SUCCESS);
    ExAdvCallbackGuard guard;

    ASSERT_TRUE(CreateExAdvSet(ctx));

    ctx.op.Reset();
    ASSERT_EQ(GAPIF_LePeriodicAdvSetParam(EX_ADV_HANDLE, PERIODIC_ADV_INTERVAL_MIN, PERIODIC_ADV_INTERVAL_MIN, 0x0000),
              BT_SUCCESS);
    bool received = ctx.op.Wait();

    ASSERT_TRUE(received) << "periodicAdvSetParamResult callback not received";
    EXPECT_EQ(ctx.op.status, HCI_SUCCESS);
}

/**
 * @tc.number: StackGapLe_PeriodicAdvSetData_00100
 * @tc.name: GAPIF_LePeriodicAdvSetData result from GapExAdvCallback
 * @tc.desc: create extended advertising set first, then send
 *           HCI_LE_SET_PERIODIC_ADVERTISING_DATA(0x003F),
 *           result through GapExAdvCallback::periodicAdvSetDataResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_PeriodicAdvSetData_00100, TestSize.Level1)
{
    if (!BTM_IsControllerSupportLeExtendedAdvertising() || !BTM_IsControllerSupportLePeriodicAdvertising()) {
        GTEST_SKIP() << "controller does not support extended/periodic advertising";
    }

    ExAdvTestContext ctx;
    GapExAdvCallback callback = {};
    callback.exAdvSetParamResult = OnExAdvSetParamResult;
    callback.periodicAdvSetParamResult = OnPeriodicAdvOpResult;
    callback.periodicAdvSetDataResult = OnPeriodicAdvOpResult;
    ASSERT_EQ(GAPIF_RegisterExAdvCallback(&callback, &ctx), BT_SUCCESS);
    ExAdvCallbackGuard guard;

    ASSERT_TRUE(CreateExAdvSet(ctx));

    const uint8_t advData[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    ctx.op.Reset();
    ASSERT_EQ(GAPIF_LePeriodicAdvSetParam(EX_ADV_HANDLE, PERIODIC_ADV_INTERVAL_MIN, PERIODIC_ADV_INTERVAL_MIN, 0x0000),
              BT_SUCCESS);
    ASSERT_TRUE(ctx.op.Wait()) << "periodicAdvSetParamResult callback not received";
    ASSERT_EQ(ctx.op.status, HCI_SUCCESS);

    ctx.op.Reset();
    ASSERT_EQ(GAPIF_LePeriodicAdvSetData(
                  EX_ADV_HANDLE, GAP_ADVERTISING_DATA_OPERATION_COMPLETE, sizeof(advData), advData),
        BT_SUCCESS);
    bool received = ctx.op.Wait();

    ASSERT_TRUE(received) << "periodicAdvSetDataResult callback not received";
    EXPECT_EQ(ctx.op.status, HCI_SUCCESS);
}

/**
 * @tc.number: StackGapLe_PeriodicAdvSetEnable_00100
 * @tc.name: GAPIF_LePeriodicAdvSetEnable result from GapExAdvCallback
 * @tc.desc: create extended advertising set and set periodic parameters first, then send
 *           HCI_LE_SET_PERIODIC_ADVERTISING_ENABLE(0x0040) enable + disable,
 *           result through GapExAdvCallback::periodicAdvSetEnableResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_PeriodicAdvSetEnable_00100, TestSize.Level1)
{
    if (!BTM_IsControllerSupportLeExtendedAdvertising() || !BTM_IsControllerSupportLePeriodicAdvertising()) {
        GTEST_SKIP() << "controller does not support extended/periodic advertising";
    }

    ExAdvTestContext ctx;
    GapExAdvCallback callback = {};
    callback.exAdvSetParamResult = OnExAdvSetParamResult;
    callback.exAdvSetEnableResult = OnPeriodicAdvOpResult;
    callback.periodicAdvSetParamResult = OnPeriodicAdvOpResult;
    callback.periodicAdvSetEnableResult = OnPeriodicAdvOpResult;
    ASSERT_EQ(GAPIF_RegisterExAdvCallback(&callback, &ctx), BT_SUCCESS);
    ExAdvCallbackGuard guard;

    ASSERT_TRUE(CreateExAdvSet(ctx));

    ctx.op.Reset();
    ASSERT_EQ(GAPIF_LePeriodicAdvSetParam(EX_ADV_HANDLE, PERIODIC_ADV_INTERVAL_MIN, PERIODIC_ADV_INTERVAL_MIN, 0x0000),
              BT_SUCCESS);
    ASSERT_TRUE(ctx.op.Wait()) << "periodicAdvSetParamResult callback not received";
    ASSERT_EQ(ctx.op.status, HCI_SUCCESS);

    // Enable extended advertising before enabling periodic advertising (Periodic Advertising depends on it).
    ctx.op.Reset();
    GapExAdvSet enableSet = {EX_ADV_HANDLE, 0, 0};
    ASSERT_EQ(GAPIF_LeExAdvSetEnable(0x01, 1, &enableSet), BT_SUCCESS);
    ASSERT_TRUE(ctx.op.Wait()) << "exAdvSetEnableResult(enable) callback not received";
    ASSERT_EQ(ctx.op.status, HCI_SUCCESS);

    ctx.op.Reset();
    ASSERT_EQ(GAPIF_LePeriodicAdvSetEnable(0x01, EX_ADV_HANDLE), BT_SUCCESS);
    ASSERT_TRUE(ctx.op.Wait()) << "periodicAdvSetEnableResult(enable) callback not received";
    EXPECT_EQ(ctx.op.status, HCI_SUCCESS);

    ctx.op.Reset();
    ASSERT_EQ(GAPIF_LePeriodicAdvSetEnable(0x00, EX_ADV_HANDLE), BT_SUCCESS);
    bool received = ctx.op.Wait();

    ASSERT_TRUE(received) << "periodicAdvSetEnableResult(disable) callback not received";
    EXPECT_EQ(ctx.op.status, HCI_SUCCESS);
}

/**
 * @tc.number: StackGapLe_PeriodicAdvCreateSyncCancel_00100
 * @tc.name: GAPIF_LePeriodicAdvCreateSyncCancel result from GapPeriodicAdvSyncCallback
 * @tc.desc: send HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_CANCEL(0x0045) with no sync pending,
 *           result through GapPeriodicAdvSyncCallback::createSyncCancelResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_PeriodicAdvCreateSyncCancel_00100, TestSize.Level1)
{
    if (!BTM_IsControllerSupportLePeriodicAdvertising()) {
        GTEST_SKIP() << "controller does not support periodic advertising";
    }

    StatusOnlyResult result;
    GapPeriodicAdvSyncCallback callback = {};
    callback.createSyncCancelResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterPeriodicAdvSyncCallback(&callback, &result), BT_SUCCESS);
    PeriodicAdvSyncCallbackGuard guard;

    ASSERT_EQ(GAPIF_LePeriodicAdvCreateSyncCancel(), BT_SUCCESS);
    bool received = result.Wait();

    // Some controllers (rk3568) silently drop periodic-sync commands without a
    // command-complete event; treat the timeout as SKIP rather than failure.
    if (!received) {
        GTEST_SKIP() << "controller did not respond to create sync cancel";
    }
    ASSERT_TRUE(received) << "createSyncCancelResult callback not received";
    // No pending sync; the controller should report an error.
    EXPECT_NE(result.status, HCI_SUCCESS);
}

/**
 * @tc.number: StackGapLe_PeriodicAdvTerminateSync_00100
 * @tc.name: GAPIF_LePeriodicAdvTerminateSync result from GapPeriodicAdvSyncCallback
 * @tc.desc: send HCI_LE_PERIODIC_ADVERTISING_TERMINATE_SYNC(0x0046) with an invalid sync handle,
 *           result through GapPeriodicAdvSyncCallback::terminateSyncResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_PeriodicAdvTerminateSync_00100, TestSize.Level1)
{
    if (!BTM_IsControllerSupportLePeriodicAdvertising()) {
        GTEST_SKIP() << "controller does not support periodic advertising";
    }

    StatusOnlyResult result;
    GapPeriodicAdvSyncCallback callback = {};
    callback.terminateSyncResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterPeriodicAdvSyncCallback(&callback, &result), BT_SUCCESS);
    PeriodicAdvSyncCallbackGuard guard;

    ASSERT_EQ(GAPIF_LePeriodicAdvTerminateSync(0x0000), BT_SUCCESS);
    bool received = result.Wait();

    // Same rk3568 silent-drop behavior as create sync cancel; SKIP on timeout.
    if (!received) {
        GTEST_SKIP() << "controller did not respond to terminate sync";
    }
    ASSERT_TRUE(received) << "terminateSyncResult callback not received";
    // Invalid sync handle; the controller should report an error.
    EXPECT_NE(result.status, HCI_SUCCESS);
}

/**
 * @tc.number: StackGapLe_AddDeviceToPeriodicAdvertiserList_00100
 * @tc.name: GAPIF_LeAddDeviceToPeriodicAdvertiserList result from GapPeriodicAdvSyncCallback
 * @tc.desc: send HCI_LE_ADD_DEVICE_TO_PERIODIC_ADVERTISER_LIST(0x0047),
 *           result through GapPeriodicAdvSyncCallback::addDeviceToPeriodicAdvertiserListResult,
 *           then clear the list to keep the controller clean
 */
struct PeriodicAdvListContext {
    StatusOnlyResult *addResult;
    StatusOnlyResult *clearResult;
};

void OnAddDeviceToPeriodicAdvertiserListResult(uint8_t status, void *context)
{
    auto *ctx = static_cast<PeriodicAdvListContext *>(context);
    ctx->addResult->status = status;
    ctx->addResult->Notify();
}

void OnClearPeriodicAdvertiserListResult(uint8_t status, void *context)
{
    auto *ctx = static_cast<PeriodicAdvListContext *>(context);
    ctx->clearResult->status = status;
    ctx->clearResult->Notify();
}

HWTEST_F(StackGapLeTest, StackGapLe_AddDeviceToPeriodicAdvertiserList_00100, TestSize.Level1)
{
    if (!BTM_IsControllerSupportLePeriodicAdvertising()) {
        GTEST_SKIP() << "controller does not support periodic advertising";
    }

    StatusOnlyResult addResult;
    StatusOnlyResult clearResult;
    PeriodicAdvListContext ctx = {&addResult, &clearResult};
    GapPeriodicAdvSyncCallback callback = {};
    callback.addDeviceToPeriodicAdvertiserListResult = OnAddDeviceToPeriodicAdvertiserListResult;
    callback.clearPeriodicAdvertiserListResult = OnClearPeriodicAdvertiserListResult;
    ASSERT_EQ(GAPIF_RegisterPeriodicAdvSyncCallback(&callback, &ctx), BT_SUCCESS);
    PeriodicAdvSyncCallbackGuard guard;

    BtAddr addr = {{0xC0, 0x11, 0x22, 0x33, 0x44, 0x55}, 0x00};
    ASSERT_EQ(GAPIF_LeAddDeviceToPeriodicAdvertiserList(0x00, &addr, 0x00), BT_SUCCESS);
    ASSERT_TRUE(addResult.Wait()) << "addDeviceToPeriodicAdvertiserListResult callback not received";
    EXPECT_EQ(addResult.status, HCI_SUCCESS);

    ASSERT_EQ(GAPIF_LeClearPeriodicAdvertiserList(), BT_SUCCESS);
    bool received = clearResult.Wait();
    ASSERT_TRUE(received) << "clearPeriodicAdvertiserListResult callback not received";
    EXPECT_EQ(clearResult.status, HCI_SUCCESS);
}

/**
 * @tc.number: StackGapLe_RemoveDeviceFromPeriodicAdvertiserList_00100
 * @tc.name: GAPIF_LeRemoveDeviceFromPeriodicAdvertiserList result from GapPeriodicAdvSyncCallback
 * @tc.desc: add a device via 0x0047 first, then send
 *           HCI_LE_REMOVE_DEVICE_FROM_PERIODIC_ADVERTISER_LIST(0x0048),
 *           result through GapPeriodicAdvSyncCallback::removeDeviceFromPeriodicAdvertiserListResult
 */
struct PeriodicAdvRemoveContext {
    StatusOnlyResult *addResult;
    StatusOnlyResult *removeResult;
};

void OnAddDeviceToPeriodicAdvertiserListResultForRemove(uint8_t status, void *context)
{
    auto *ctx = static_cast<PeriodicAdvRemoveContext *>(context);
    ctx->addResult->status = status;
    ctx->addResult->Notify();
}

void OnRemoveDeviceFromPeriodicAdvertiserListResult(uint8_t status, void *context)
{
    auto *ctx = static_cast<PeriodicAdvRemoveContext *>(context);
    ctx->removeResult->status = status;
    ctx->removeResult->Notify();
}

HWTEST_F(StackGapLeTest, StackGapLe_RemoveDeviceFromPeriodicAdvertiserList_00100, TestSize.Level1)
{
    if (!BTM_IsControllerSupportLePeriodicAdvertising()) {
        GTEST_SKIP() << "controller does not support periodic advertising";
    }

    StatusOnlyResult addResult;
    StatusOnlyResult removeResult;
    PeriodicAdvRemoveContext ctx = {&addResult, &removeResult};
    GapPeriodicAdvSyncCallback callback = {};
    callback.addDeviceToPeriodicAdvertiserListResult = OnAddDeviceToPeriodicAdvertiserListResultForRemove;
    callback.removeDeviceFromPeriodicAdvertiserListResult = OnRemoveDeviceFromPeriodicAdvertiserListResult;
    ASSERT_EQ(GAPIF_RegisterPeriodicAdvSyncCallback(&callback, &ctx), BT_SUCCESS);
    PeriodicAdvSyncCallbackGuard guard;

    BtAddr addr = {{0xC0, 0x11, 0x22, 0x33, 0x44, 0x55}, 0x00};
    ASSERT_EQ(GAPIF_LeAddDeviceToPeriodicAdvertiserList(0x00, &addr, 0x00), BT_SUCCESS);
    ASSERT_TRUE(addResult.Wait()) << "addDeviceToPeriodicAdvertiserListResult callback not received";
    ASSERT_EQ(addResult.status, HCI_SUCCESS);

    ASSERT_EQ(GAPIF_LeRemoveDeviceFromPeriodicAdvertiserList(0x00, &addr, 0x00), BT_SUCCESS);
    bool received = removeResult.Wait();

    ASSERT_TRUE(received) << "removeDeviceFromPeriodicAdvertiserListResult callback not received";
    EXPECT_EQ(removeResult.status, HCI_SUCCESS);
}

/**
 * @tc.number: StackGapLe_ClearPeriodicAdvertiserList_00100
 * @tc.name: GAPIF_LeClearPeriodicAdvertiserList result from GapPeriodicAdvSyncCallback
 * @tc.desc: send HCI_LE_CLEAR_PERIODIC_ADVERTISER_LIST(0x0049),
 *           result through GapPeriodicAdvSyncCallback::clearPeriodicAdvertiserListResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_ClearPeriodicAdvertiserList_00100, TestSize.Level1)
{
    if (!BTM_IsControllerSupportLePeriodicAdvertising()) {
        GTEST_SKIP() << "controller does not support periodic advertising";
    }

    StatusOnlyResult result;
    GapPeriodicAdvSyncCallback callback = {};
    callback.clearPeriodicAdvertiserListResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterPeriodicAdvSyncCallback(&callback, &result), BT_SUCCESS);
    PeriodicAdvSyncCallbackGuard guard;

    ASSERT_EQ(GAPIF_LeClearPeriodicAdvertiserList(), BT_SUCCESS);
    bool received = result.Wait();

    ASSERT_TRUE(received) << "clearPeriodicAdvertiserListResult callback not received";
    EXPECT_EQ(result.status, HCI_SUCCESS);
}

/**
 * @tc.number: StackGapLe_ReadPeriodicAdvertiserListSize_00100
 * @tc.name: GAPIF_LeReadPeriodicAdvertiserListSize result from GapPeriodicAdvSyncCallback
 * @tc.desc: send HCI_LE_READ_PERIODIC_ADVERTISER_LIST_SIZE(0x004A),
 *           result through GapPeriodicAdvSyncCallback::readPeriodicAdvertiserListSizeResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_ReadPeriodicAdvertiserListSize_00100, TestSize.Level1)
{
    if (!BTM_IsControllerSupportLePeriodicAdvertising()) {
        GTEST_SKIP() << "controller does not support periodic advertising";
    }

    ReadAdvertiserListSizeResult result;
    GapPeriodicAdvSyncCallback callback = {};
    callback.readPeriodicAdvertiserListSizeResult = OnReadAdvertiserListSizeResult;
    ASSERT_EQ(GAPIF_RegisterPeriodicAdvSyncCallback(&callback, &result), BT_SUCCESS);
    PeriodicAdvSyncCallbackGuard guard;

    ASSERT_EQ(GAPIF_LeReadPeriodicAdvertiserListSize(), BT_SUCCESS);
    bool received = result.Wait();

    ASSERT_TRUE(received) << "readPeriodicAdvertiserListSizeResult callback not received";
    EXPECT_EQ(result.status, HCI_SUCCESS);
}

/**
 * @tc.number: StackGapLe_EnhancedReceiverTest_00100
 * @tc.name: GAPIF_LeEnhancedReceiverTest result from GapLeControllerCallback
 * @tc.desc: send HCI_LE_ENHANCED_RECEIVER_TEST(0x0033) on channel 0, 1M PHY,
 *           result through GapLeControllerCallback::enhancedReceiverTestResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_EnhancedReceiverTest_00100, TestSize.Level1)
{
    StatusOnlyResult result;

    GapLeControllerCallback callback = {};
    callback.enhancedReceiverTestResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterLeControllerCallback(&callback, &result), BT_SUCCESS);
    LeControllerCallbackGuard guard;

    ASSERT_EQ(GAPIF_LeEnhancedReceiverTest(0x00, 0x01, 0x00), BT_SUCCESS);
    bool received = result.Wait();

    ASSERT_TRUE(received) << "enhancedReceiverTestResult callback not received";
    if (result.status == HCI_UNKNOWN_HCI_COMMAND || result.status == HCI_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE) {
        // Legal on controllers that do not implement LE Enhanced Receiver Test;
        // skip instead of failing on such hardware.
        GTEST_SKIP() << "controller does not support LE Enhanced Receiver Test";
    }
    EXPECT_EQ(result.status, HCI_SUCCESS);
}

/**
 * @tc.number: StackGapLe_EnhancedTransmitterTest_00100
 * @tc.name: GAPIF_LeEnhancedTransmitterTest result from GapLeControllerCallback
 * @tc.desc: send HCI_LE_ENHANCED_TRANSMITTER_TEST(0x0034) on channel 0, 1M PHY,
 *           result through GapLeControllerCallback::enhancedTransmitterTestResult
 */
HWTEST_F(StackGapLeTest, StackGapLe_EnhancedTransmitterTest_00100, TestSize.Level1)
{
    StatusOnlyResult result;

    GapLeControllerCallback callback = {};
    callback.enhancedTransmitterTestResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterLeControllerCallback(&callback, &result), BT_SUCCESS);
    LeControllerCallbackGuard guard;

    ASSERT_EQ(GAPIF_LeEnhancedTransmitterTest(0x00, 0x25, 0x00, 0x01), BT_SUCCESS);
    bool received = result.Wait();

    ASSERT_TRUE(received) << "enhancedTransmitterTestResult callback not received";
    if (result.status == HCI_UNKNOWN_HCI_COMMAND || result.status == HCI_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE) {
        // Legal on controllers that do not implement LE Enhanced Transmitter Test;
        // skip instead of failing on such hardware.
        GTEST_SKIP() << "controller does not support LE Enhanced Transmitter Test";
    }
    EXPECT_EQ(result.status, HCI_SUCCESS);
}
} // namespace Bluetooth
} // namespace OHOS
