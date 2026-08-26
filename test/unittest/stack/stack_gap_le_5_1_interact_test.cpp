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

// 蓝牙 5.1 新增 HCI 命令 → 对外接口 → 回调 对照表（表二：互动 = 是）
// 本文件覆盖需要与其他蓝牙设备互动才能获得有效结果的 9 条 5.1 命令：
//   0x0053 Set Connectionless IQ Sampling Enable（需已建周期广播同步）
//   0x0054 Set Connection CTE Receive Parameters（需已建 LE 连接）
//   0x0055 Set Connection CTE Transmit Parameters（需已建 LE 连接）
//   0x0056 Connection CTE Request Enable（需已建 LE 连接）
//   0x0057 Connection CTE Response Enable（需已建 LE 连接）
//   0x0059 Set Periodic Advertising Receive Enable（需已有 sync handle）
//   0x005A Periodic Advertising Sync Transfer（需已建 LE 连接）
//   0x005B Periodic Advertising Set Info Transfer（需已建 LE 连接）
//   0x005C Set PA Sync Transfer Parameters（需已建 LE 连接）
// 本文件覆盖 L1 可测部分：参数校验（BT_BAD_PARAM）、能力 gate（BT_NOT_SUPPORT），
// 以及在无连接/无同步对象时验证「命令发出 → Command Complete 回调通路」——
// 控制器对无效 handle 返回错误 status（0x02 Unknown Connection Identifier /
// 0x12 Invalid HCI Command Parameters 等），从而证明命令发送与回调接线正确。
// 成功路径需要真实双机互动（对端同样支持 5.1 测向/PAST），归入 L3 真机/PTS 验证，
// 不在本文件覆盖；如需双机联测可参照 stack_gap_le_interact_test.cpp 的 peer 模式。

#include <gtest/gtest.h>

#include <chrono>
#include <condition_variable>
#include <cstring>
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
constexpr uint16_t INVALID_CONNECTION_HANDLE = 0x0001;  // 合法范围但无对应连接
constexpr uint16_t INVALID_SYNC_HANDLE = 0x0001;        // 合法范围但无对应同步
constexpr uint8_t HCI_STATUS_UNKNOWN = 0xFF;
constexpr uint16_t PAST_SYNC_TIMEOUT_DEFAULT = 0x2000;

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

struct StatusOnlyResult : CallbackWaiter {
    uint8_t status = HCI_STATUS_UNKNOWN;
};

void OnStatusOnlyResult(uint8_t status, void *context)
{
    auto *result = static_cast<StatusOnlyResult *>(context);
    result->status = status;
    result->Notify();
}

// RAII guard：用例结束（含断言失败）时自动注销 GapLeCteCallback。
class GapLeCteCallbackGuard {
public:
    ~GapLeCteCallbackGuard()
    {
        if (GAPIF_DeregisterLeCteCallback() != BT_SUCCESS) {
            printf("GapLeCteCallbackGuard: deregister failed; callback may remain registered\n");
        }
    }
    GapLeCteCallbackGuard(const GapLeCteCallbackGuard &) = delete;
    GapLeCteCallbackGuard &operator=(const GapLeCteCallbackGuard &) = delete;
    GapLeCteCallbackGuard() = default;
};

// 发送需要互动的命令并验证回调通路。命令发送函数返回 BT_SUCCESS 时，
// 注册的回调必须在超时前到达且 status 非成功（控制器拒绝无效 handle）。
static void RunRejectedCommand(int sendResult, StatusOnlyResult &result)
{
    ASSERT_EQ(sendResult, BT_SUCCESS) << "command must be sent when the controller supports the feature";
    bool received = result.Wait();

    ASSERT_TRUE(received) << "command complete callback not received";
    if (result.status == HCI_UNKNOWN_HCI_COMMAND || result.status == HCI_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE) {
        // 控制器未实现该 5.1 命令时跳过（与现有测试一致）。
        GTEST_SKIP() << "controller does not implement this 5.1 command";
    }
    // 无连接/无同步对象时控制器必然拒绝：0x02 Unknown Connection Identifier 等。
    EXPECT_NE(result.status, HCI_SUCCESS);
}
} // namespace

class StackGapLe51InteractTest : public testing::Test {
public:
    StackGapLe51InteractTest() {}
    ~StackGapLe51InteractTest() {}

    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void StackGapLe51InteractTest::SetUpTestCase(void)
{
    ASSERT_EQ(BTM_Initialize(), BT_SUCCESS);
    ASSERT_EQ(BTM_Enable(LE_CONTROLLER), BT_SUCCESS);
    ASSERT_TRUE(BTM_IsEnabled(LE_CONTROLLER));
    // 表二命令需要 CENTRAL|PERIPHERAL（连接类）或 OBSERVER|CENTRAL（同步类）角色。
    ASSERT_EQ(
        GAPIF_LeSetRole(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_PERIPHERAL | GAP_LE_ROLE_CENTRAL),
        BT_SUCCESS);
    // 本文件不建立真实连接：成功路径需要双机互动（peer 设备 + 5.1 控制器），
    // 无对端时仅能验证失败路径与命令/回调接线。
    printf("no peer device is required: unconnected failure-path tests only\n");
}

void StackGapLe51InteractTest::TearDownTestCase(void)
{
    EXPECT_EQ(BTM_Disable(LE_CONTROLLER), BT_SUCCESS);
    EXPECT_EQ(BTM_Close(), BT_SUCCESS);
}

void StackGapLe51InteractTest::SetUp() {}

void StackGapLe51InteractTest::TearDown() {}

/**
 * @tc.number: StackGapLe5_1Interact_SetConnectionlessIqSamplingEnable_ParamCheck_00100
 * @tc.name: GAPIF_LeSetConnectionlessIqSamplingEnable parameter validation
 * @tc.desc: invalid parameters must return BT_BAD_PARAM without sending the command
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_SetConnectionlessIqSamplingEnable_ParamCheck_00100,
    TestSize.Level1)
{
    // syncHandle 越界（0x1000 > 0x0EFF 且 != 0x0FFF 接收测试保留值）。
    GapLeSetConnectionlessIqSamplingEnableParam badSyncParam = {0x1000, 0x01, GAP_LE_CTE_SLOT_DURATIONS_1US,
        GAP_LE_CTE_COUNT_MIN, 0, nullptr};
    EXPECT_EQ(GAPIF_LeSetConnectionlessIqSamplingEnable(&badSyncParam), BT_BAD_PARAM);
    // maxSampledCtes 越界（0x11 > 0x10）。
    GapLeSetConnectionlessIqSamplingEnableParam highCountParam = {INVALID_SYNC_HANDLE, 0x01,
        GAP_LE_CTE_SLOT_DURATIONS_1US, GAP_LE_CTE_COUNT_MAX + 1, 0, nullptr};
    EXPECT_EQ(GAPIF_LeSetConnectionlessIqSamplingEnable(&highCountParam), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe5_1Interact_SetConnectionlessIqSamplingEnable_NoSync_00200
 * @tc.name: GAPIF_LeSetConnectionlessIqSamplingEnable without a sync object
 * @tc.desc: no sync established (interaction required): command is sent when Bit 20 is
 *           supported and the controller rejects the invalid sync handle
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_SetConnectionlessIqSamplingEnable_NoSync_00200,
    TestSize.Level1)
{
    GapLeSetConnectionlessIqSamplingEnableParam invalidSyncParam = {INVALID_SYNC_HANDLE, 0x01,
        GAP_LE_CTE_SLOT_DURATIONS_1US, GAP_LE_CTE_COUNT_MIN, 0, nullptr};
    if (!BTM_IsControllerSupportConnectionlessCteReceiver()) {
        EXPECT_EQ(GAPIF_LeSetConnectionlessIqSamplingEnable(&invalidSyncParam), BT_NOT_SUPPORT);
        return;
    }

    StatusOnlyResult result;
    GapLeCteCallback callback = {};
    callback.setConnectionlessIqSamplingEnableResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterLeCteCallback(&callback, &result), BT_SUCCESS);
    GapLeCteCallbackGuard guard;

    RunRejectedCommand(GAPIF_LeSetConnectionlessIqSamplingEnable(&invalidSyncParam), result);
}

/**
 * @tc.number: StackGapLe5_1Interact_SetConnectionCteReceiveParameters_ParamCheck_00100
 * @tc.name: GAPIF_LeSetConnectionCteReceiveParameters parameter validation
 * @tc.desc: invalid parameters must return BT_BAD_PARAM without sending the command
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_SetConnectionCteReceiveParameters_ParamCheck_00100,
    TestSize.Level1)
{
    // connectionHandle 越界（0x0F00 > 0x0EFF）。
    EXPECT_EQ(GAPIF_LeSetConnectionCteReceiveParameters(
                  0x0F00, 0x01, GAP_LE_CTE_SLOT_DURATIONS_1US, 0, nullptr),
        BT_BAD_PARAM);
    // slotDurations 非法（0x00）。
    EXPECT_EQ(GAPIF_LeSetConnectionCteReceiveParameters(
                  INVALID_CONNECTION_HANDLE, 0x01, 0x00, 0, nullptr),
        BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe5_1Interact_SetConnectionCteReceiveParameters_NoConnection_00200
 * @tc.name: GAPIF_LeSetConnectionCteReceiveParameters without a connection
 * @tc.desc: no LE connection (interaction required): command is sent when Bit 17 is
 *           supported and the controller rejects the invalid connection handle
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_SetConnectionCteReceiveParameters_NoConnection_00200,
    TestSize.Level1)
{
    if (!BTM_IsControllerSupportConnectionCteRequest()) {
        EXPECT_EQ(GAPIF_LeSetConnectionCteReceiveParameters(
                      INVALID_CONNECTION_HANDLE, 0x01, GAP_LE_CTE_SLOT_DURATIONS_1US, 0, nullptr),
            BT_NOT_SUPPORT);
        return;
    }

    StatusOnlyResult result;
    GapLeCteCallback callback = {};
    callback.setConnectionCteReceiveParametersResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterLeCteCallback(&callback, &result), BT_SUCCESS);
    GapLeCteCallbackGuard guard;

    RunRejectedCommand(GAPIF_LeSetConnectionCteReceiveParameters(
                           INVALID_CONNECTION_HANDLE, 0x01, GAP_LE_CTE_SLOT_DURATIONS_1US, 0, nullptr),
        result);
}

/**
 * @tc.number: StackGapLe5_1Interact_SetConnectionCteTransmitParameters_ParamCheck_00100
 * @tc.name: GAPIF_LeSetConnectionCteTransmitParameters parameter validation
 * @tc.desc: invalid parameters must return BT_BAD_PARAM without sending the command
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_SetConnectionCteTransmitParameters_ParamCheck_00100,
    TestSize.Level1)
{
    // connectionHandle 越界。
    EXPECT_EQ(GAPIF_LeSetConnectionCteTransmitParameters(0x0F00, 0x00, 0, nullptr), BT_BAD_PARAM);
    // cteTypes 含保留位（bit 3）。
    EXPECT_EQ(GAPIF_LeSetConnectionCteTransmitParameters(INVALID_CONNECTION_HANDLE, 0x08, 0, nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe5_1Interact_SetConnectionCteTransmitParameters_NoConnection_00200
 * @tc.name: GAPIF_LeSetConnectionCteTransmitParameters without a connection
 * @tc.desc: no LE connection (interaction required): command is sent when Bit 18 is
 *           supported and the controller rejects the invalid connection handle
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_SetConnectionCteTransmitParameters_NoConnection_00200,
    TestSize.Level1)
{
    if (!BTM_IsControllerSupportConnectionCteResponse()) {
        EXPECT_EQ(GAPIF_LeSetConnectionCteTransmitParameters(INVALID_CONNECTION_HANDLE, 0x00, 0, nullptr),
            BT_NOT_SUPPORT);
        return;
    }

    StatusOnlyResult result;
    GapLeCteCallback callback = {};
    callback.setConnectionCteTransmitParametersResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterLeCteCallback(&callback, &result), BT_SUCCESS);
    GapLeCteCallbackGuard guard;

    RunRejectedCommand(
        GAPIF_LeSetConnectionCteTransmitParameters(INVALID_CONNECTION_HANDLE, 0x00, 0, nullptr), result);
}

/**
 * @tc.number: StackGapLe5_1Interact_ConnectionCteRequestEnable_ParamCheck_00100
 * @tc.name: GAPIF_LeConnectionCteRequestEnable parameter validation
 * @tc.desc: invalid parameters must return BT_BAD_PARAM without sending the command
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_ConnectionCteRequestEnable_ParamCheck_00100,
    TestSize.Level1)
{
    // requestedCteLength 越界（0x15 > 0x14）。
    EXPECT_EQ(GAPIF_LeConnectionCteRequestEnable(
                  INVALID_CONNECTION_HANDLE, 0x01, 0x0001, 0x15, GAP_LE_CTE_TYPE_AOD_1US),
        BT_BAD_PARAM);
    // enable 越界（0x02）。
    EXPECT_EQ(GAPIF_LeConnectionCteRequestEnable(
                  INVALID_CONNECTION_HANDLE, 0x02, 0x0001, GAP_LE_CTE_LENGTH_MAX, GAP_LE_CTE_TYPE_AOD_1US),
        BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe5_1Interact_ConnectionCteRequestEnable_NoConnection_00200
 * @tc.name: GAPIF_LeConnectionCteRequestEnable without a connection
 * @tc.desc: no LE connection (interaction required): command is sent when Bit 17 is
 *           supported and the controller rejects the invalid connection handle
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_ConnectionCteRequestEnable_NoConnection_00200,
    TestSize.Level1)
{
    if (!BTM_IsControllerSupportConnectionCteRequest()) {
        EXPECT_EQ(GAPIF_LeConnectionCteRequestEnable(
                      INVALID_CONNECTION_HANDLE, 0x01, 0x0001, GAP_LE_CTE_LENGTH_MAX, GAP_LE_CTE_TYPE_AOD_1US),
            BT_NOT_SUPPORT);
        return;
    }

    StatusOnlyResult result;
    GapLeCteCallback callback = {};
    callback.connectionCteRequestEnableResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterLeCteCallback(&callback, &result), BT_SUCCESS);
    GapLeCteCallbackGuard guard;

    RunRejectedCommand(GAPIF_LeConnectionCteRequestEnable(
                           INVALID_CONNECTION_HANDLE, 0x01, 0x0001, GAP_LE_CTE_LENGTH_MAX, GAP_LE_CTE_TYPE_AOD_1US),
        result);
}

/**
 * @tc.number: StackGapLe5_1Interact_ConnectionCteResponseEnable_ParamCheck_00100
 * @tc.name: GAPIF_LeConnectionCteResponseEnable parameter validation
 * @tc.desc: invalid parameters must return BT_BAD_PARAM without sending the command
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_ConnectionCteResponseEnable_ParamCheck_00100,
    TestSize.Level1)
{
    // connectionHandle 越界。
    EXPECT_EQ(GAPIF_LeConnectionCteResponseEnable(0x0F00, 0x01), BT_BAD_PARAM);
    // enable 越界。
    EXPECT_EQ(GAPIF_LeConnectionCteResponseEnable(INVALID_CONNECTION_HANDLE, 0x02), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe5_1Interact_ConnectionCteResponseEnable_NoConnection_00200
 * @tc.name: GAPIF_LeConnectionCteResponseEnable without a connection
 * @tc.desc: no LE connection (interaction required): command is sent when Bit 18 is
 *           supported and the controller rejects the invalid connection handle
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_ConnectionCteResponseEnable_NoConnection_00200,
    TestSize.Level1)
{
    if (!BTM_IsControllerSupportConnectionCteResponse()) {
        EXPECT_EQ(GAPIF_LeConnectionCteResponseEnable(INVALID_CONNECTION_HANDLE, 0x01), BT_NOT_SUPPORT);
        return;
    }

    StatusOnlyResult result;
    GapLeCteCallback callback = {};
    callback.connectionCteResponseEnableResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterLeCteCallback(&callback, &result), BT_SUCCESS);
    GapLeCteCallbackGuard guard;

    RunRejectedCommand(GAPIF_LeConnectionCteResponseEnable(INVALID_CONNECTION_HANDLE, 0x01), result);
}

/**
 * @tc.number: StackGapLe5_1Interact_SetPeriodicAdvertisingReceiveEnable_ParamCheck_00100
 * @tc.name: GAPIF_LeSetPeriodicAdvertisingReceiveEnable parameter validation
 * @tc.desc: invalid parameters must return BT_BAD_PARAM without sending the command
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_SetPeriodicAdvertisingReceiveEnable_ParamCheck_00100,
    TestSize.Level1)
{
    // syncHandle 越界（0x0F00 > 0x0EFF）。
    EXPECT_EQ(GAPIF_LeSetPeriodicAdvertisingReceiveEnable(0x0F00, 0x01), BT_BAD_PARAM);
    // enable 越界。
    EXPECT_EQ(GAPIF_LeSetPeriodicAdvertisingReceiveEnable(INVALID_SYNC_HANDLE, 0x02), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe5_1Interact_SetPeriodicAdvertisingReceiveEnable_NoSync_00200
 * @tc.name: GAPIF_LeSetPeriodicAdvertisingReceiveEnable without a sync object
 * @tc.desc: no sync established (interaction required): command is sent when Bit 25 is
 *           supported and the controller rejects the invalid sync handle
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_SetPeriodicAdvertisingReceiveEnable_NoSync_00200,
    TestSize.Level1)
{
    if (!BTM_IsControllerSupportPeriodicAdvertisingSyncTransferRecipient()) {
        EXPECT_EQ(GAPIF_LeSetPeriodicAdvertisingReceiveEnable(INVALID_SYNC_HANDLE, 0x01), BT_NOT_SUPPORT);
        return;
    }

    StatusOnlyResult result;
    GapLeCteCallback callback = {};
    callback.setPeriodicAdvertisingReceiveEnableResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterLeCteCallback(&callback, &result), BT_SUCCESS);
    GapLeCteCallbackGuard guard;

    RunRejectedCommand(GAPIF_LeSetPeriodicAdvertisingReceiveEnable(INVALID_SYNC_HANDLE, 0x01), result);
}

/**
 * @tc.number: StackGapLe5_1Interact_PeriodicAdvertisingSyncTransfer_ParamCheck_00100
 * @tc.name: GAPIF_LePeriodicAdvertisingSyncTransfer parameter validation
 * @tc.desc: invalid parameters must return BT_BAD_PARAM without sending the command
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_PeriodicAdvertisingSyncTransfer_ParamCheck_00100,
    TestSize.Level1)
{
    // connectionHandle 越界。
    EXPECT_EQ(GAPIF_LePeriodicAdvertisingSyncTransfer(0x0F00, 0x0000, INVALID_SYNC_HANDLE), BT_BAD_PARAM);
    // syncHandle 越界。
    EXPECT_EQ(GAPIF_LePeriodicAdvertisingSyncTransfer(INVALID_CONNECTION_HANDLE, 0x0000, 0x0F00), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe5_1Interact_PeriodicAdvertisingSyncTransfer_NoConnection_00200
 * @tc.name: GAPIF_LePeriodicAdvertisingSyncTransfer without a connection
 * @tc.desc: no LE connection (interaction required): command is sent when Bit 24 is
 *           supported and the controller rejects the invalid connection handle
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_PeriodicAdvertisingSyncTransfer_NoConnection_00200,
    TestSize.Level1)
{
    if (!BTM_IsControllerSupportPeriodicAdvertisingSyncTransferSender()) {
        EXPECT_EQ(GAPIF_LePeriodicAdvertisingSyncTransfer(INVALID_CONNECTION_HANDLE, 0x0000, INVALID_SYNC_HANDLE),
            BT_NOT_SUPPORT);
        return;
    }

    StatusOnlyResult result;
    GapLeCteCallback callback = {};
    callback.periodicAdvertisingSyncTransferResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterLeCteCallback(&callback, &result), BT_SUCCESS);
    GapLeCteCallbackGuard guard;

    RunRejectedCommand(
        GAPIF_LePeriodicAdvertisingSyncTransfer(INVALID_CONNECTION_HANDLE, 0x0000, INVALID_SYNC_HANDLE), result);
}

/**
 * @tc.number: StackGapLe5_1Interact_PeriodicAdvertisingSetInfoTransfer_ParamCheck_00100
 * @tc.name: GAPIF_LePeriodicAdvertisingSetInfoTransfer parameter validation
 * @tc.desc: invalid parameters must return BT_BAD_PARAM without sending the command
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_PeriodicAdvertisingSetInfoTransfer_ParamCheck_00100,
    TestSize.Level1)
{
    // connectionHandle 越界。
    EXPECT_EQ(GAPIF_LePeriodicAdvertisingSetInfoTransfer(0x0F00, 0x0000, 0x00), BT_BAD_PARAM);
    // advertisingHandle 越界（0xF0 > 0xEF）。
    EXPECT_EQ(GAPIF_LePeriodicAdvertisingSetInfoTransfer(INVALID_CONNECTION_HANDLE, 0x0000, 0xF0), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe5_1Interact_PeriodicAdvertisingSetInfoTransfer_NoConnection_00200
 * @tc.name: GAPIF_LePeriodicAdvertisingSetInfoTransfer without a connection
 * @tc.desc: no LE connection (interaction required): command is sent when Bit 24 is
 *           supported and the controller rejects the invalid connection handle
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_PeriodicAdvertisingSetInfoTransfer_NoConnection_00200,
    TestSize.Level1)
{
    if (!BTM_IsControllerSupportPeriodicAdvertisingSyncTransferSender()) {
        EXPECT_EQ(GAPIF_LePeriodicAdvertisingSetInfoTransfer(INVALID_CONNECTION_HANDLE, 0x0000, 0x00),
            BT_NOT_SUPPORT);
        return;
    }

    StatusOnlyResult result;
    GapLeCteCallback callback = {};
    callback.periodicAdvertisingSetInfoTransferResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterLeCteCallback(&callback, &result), BT_SUCCESS);
    GapLeCteCallbackGuard guard;

    RunRejectedCommand(GAPIF_LePeriodicAdvertisingSetInfoTransfer(INVALID_CONNECTION_HANDLE, 0x0000, 0x00), result);
}

/**
 * @tc.number: StackGapLe5_1Interact_SetPastSyncTransferParameters_ParamCheck_00100
 * @tc.name: GAPIF_LeSetPeriodicAdvertisingSyncTransferParameters parameter validation
 * @tc.desc: invalid parameters must return BT_BAD_PARAM without sending the command
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_SetPastSyncTransferParameters_ParamCheck_00100,
    TestSize.Level1)
{
    // mode 越界（0x03 > 0x02）。
    EXPECT_EQ(GAPIF_LeSetPeriodicAdvertisingSyncTransferParameters(
                  INVALID_CONNECTION_HANDLE, 0x03, 0x0000, PAST_SYNC_TIMEOUT_DEFAULT, 0x00),
        BT_BAD_PARAM);
    // syncTimeout 低于下限（0x0009 < 0x000A）。
    EXPECT_EQ(GAPIF_LeSetPeriodicAdvertisingSyncTransferParameters(
                  INVALID_CONNECTION_HANDLE, GAP_LE_PAST_MODE_NO_SYNC, 0x0000, 0x0009, 0x00),
        BT_BAD_PARAM);
    // cteType 含保留位（bit 3）。
    EXPECT_EQ(GAPIF_LeSetPeriodicAdvertisingSyncTransferParameters(
                  INVALID_CONNECTION_HANDLE, GAP_LE_PAST_MODE_NO_SYNC, 0x0000, PAST_SYNC_TIMEOUT_DEFAULT, 0x08),
        BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe5_1Interact_SetPastSyncTransferParameters_NoConnection_00200
 * @tc.name: GAPIF_LeSetPeriodicAdvertisingSyncTransferParameters without a connection
 * @tc.desc: no LE connection (interaction required): command is sent when Bit 24 is
 *           supported and the controller rejects the invalid connection handle
 */
HWTEST_F(StackGapLe51InteractTest, StackGapLe5_1Interact_SetPastSyncTransferParameters_NoConnection_00200,
    TestSize.Level1)
{
    if (!BTM_IsControllerSupportPeriodicAdvertisingSyncTransferSender()) {
        EXPECT_EQ(GAPIF_LeSetPeriodicAdvertisingSyncTransferParameters(
                      INVALID_CONNECTION_HANDLE, GAP_LE_PAST_MODE_NO_SYNC, 0x0000, PAST_SYNC_TIMEOUT_DEFAULT, 0x00),
            BT_NOT_SUPPORT);
        return;
    }

    StatusOnlyResult result;
    GapLeCteCallback callback = {};
    callback.setPeriodicAdvertisingSyncTransferParametersResult = OnStatusOnlyResult;
    ASSERT_EQ(GAPIF_RegisterLeCteCallback(&callback, &result), BT_SUCCESS);
    GapLeCteCallbackGuard guard;

    RunRejectedCommand(GAPIF_LeSetPeriodicAdvertisingSyncTransferParameters(
                           INVALID_CONNECTION_HANDLE, GAP_LE_PAST_MODE_NO_SYNC, 0x0000, PAST_SYNC_TIMEOUT_DEFAULT,
                           0x00),
        result);
}
} // namespace Bluetooth
} // namespace OHOS
