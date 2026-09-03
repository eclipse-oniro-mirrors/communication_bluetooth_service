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

// 蓝牙 5.3 新增内容单测。与 5.2 一样遵循"真 HDI 回包不可依赖"的策略（见开发计划
// §6.1 R6）：命令层用例只验参数门控，不等待控制器回包；事件层走字节级 wire 注入。
//
// 覆盖范围：
//   1) LE Subrate Change 事件（0x3E/0x23，7.7.65,35）：全字段解析（含 2 字节
//      Continuation_Number 的线格式）、边界值 0x01F4/0x01F3/0x0C80、截断/长度
//      不符/未知子事件 0x24 负向丢弃 —— HCI 层同步解析（无栈 fixture）。
//   2) Encryption Change [v2] 事件（0x59，7.7.8）：全字段解析 + keySize=0x10；
//      载荷长度非 5 丢弃；与 v1（0x08）双轨隔离（v1 只触发 encryptionChange、
//      v2 只触发 encryptionChangeV2）—— HCI 层同步解析。
//   3) Command Complete 解析槽：0x7C/0x7D/0x7E（status-only，直接投递
//      HciEventOnLeCommandComplete，绕过 pending-command 关联以保持确定性）、
//      0x0084（HciEventOnControllerBasebandCommandComplete）。
//   4) 活栈下 GAP 全链路：wire 注入 0x3E/0x23 → gap_hci_receive 表行 → GAP 任务
//      → GapLeSubrateCallback.subrateChange 字段逐一断言；注销后不再投递。
//   5) 公共 API 门控：GAPIF_LeSubrateRequest / GAPIF_LeSetDefaultSubrate /
//      GAPIF_LePeriodicAdvSetEnableWithAdi / HCI_* 发送器参数校验。
//
// 注：LE Subrate Change 事件经 GAP 层是无状态透传（连接句柄即载荷，见
// gap_le_subrate.c），故不依赖真实连接即可断言；0x59 在 GAP/BR 层的消费需已有
// 链路状态（BR 安全状态机 / LE 连接记录），超出 wire 单测范围，仅做 HCI 层覆盖。

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstring>
#include <mutex>

#include <gtest/gtest.h>

#include "btm.h"
#include "btstack.h"
#include "buffer.h"
#include "gap_le_if.h"
#include "src/hci/evt/hci_evt.h"
#include "src/hci/evt/hci_evt_controller_baseband_cmd_complete.h"
#include "src/hci/evt/hci_evt_le_cmd_complete.h"
#include "src/hci/hci.h"
#include "src/hci/hci_def_controller_baseband_cmd.h"
#include "src/hci/hci_def_evt.h"
#include "src/hci/hci_def_le_cmd.h"
#include "src/hci/hci_def_le_evt.h"

using namespace testing::ext;

namespace OHOS {
namespace Bluetooth {
namespace {
constexpr int WAIT_CALLBACK_TIMEOUT_MS = 5000;
constexpr uint8_t HCI_STATUS_SUCCESS = 0x00;
constexpr uint16_t TEST_CONN_HANDLE = 0x0042;

// ---------------- wire 字节（完整 HCI 事件包：Event_Code + Parameter_Total_Length + parameters） ----------------

// Subevent 0x23 LE Subrate Change Event（7.7.65,35）。子事件码后 11 字节：
//   Status(1)=0x00 | Connection_Handle(2)=0x0042 | Subrate_Factor(2)=0x0064(100) |
//   Peripheral_Latency(2)=0x001E(30) | Continuation_Number(2)=0x0005(5，2 字节) |
//   Supervision_Timeout(2)=0x0C80(3200，10ms 单位 = 32 s)
constexpr uint8_t SUBRATE_CHANGE_WIRE[] = {
    0x3E,
    0x0C, // LE Meta, Parameter_Total_Length = 12（子事件码 1 + 事件参数 11）
    0x23, // Subevent: LE Subrate Change
    0x00, // Status
    0x42,
    0x00, // Connection_Handle
    0x64,
    0x00, // Subrate_Factor = 100
    0x1E,
    0x00, // Peripheral_Latency = 30
    0x05,
    0x00, // Continuation_Number = 5 (2 octets)
    0x80,
    0x0C, // Supervision_Timeout = 3200 (32 s)
};

// 同事件的上限边界值：0x01F4/0x01F3/0x01F3/0x0C80 均跨 2 字节（低字节非 0），任何
// 字段偏移错误都会在此显形；Continuation_Number=0x01F3 同时证明其为 2 字节而非
// 1 字节（1 字节实现读到 0xF3=243 会与这里 499 不符）。
constexpr uint8_t SUBRATE_CHANGE_MAX_WIRE[] = {
    0x3E,
    0x0C,
    0x23,
    0x00, // Status
    0x42,
    0x00, // Connection_Handle
    0xF4,
    0x01, // Subrate_Factor = 500 (0x01F4)
    0xF3,
    0x01, // Peripheral_Latency = 499 (0x01F3)
    0xF3,
    0x01, // Continuation_Number = 499 (0x01F3)
    0x80,
    0x0C, // Supervision_Timeout = 3200 (0x0C80)
};

// 畸形：子事件码后只有 10 字节（规范要求 11；声明长度与实际一致 = 11，
// 由子事件解析器长度门控丢弃，而非 HciOnEvent 长度校验）。
constexpr uint8_t SUBRATE_CHANGE_TRUNCATED_WIRE[] = {
    0x3E,
    0x0B,
    0x23,
    0x00,
    0x42,
    0x00,
    0x64,
    0x00,
    0x1E,
    0x00,
    0x05,
    0x00,
    0x80,
};

// 畸形：声明长度 0x0A（10），实际负载 12 字节（子事件码 1 + 事件参数 11）
// → HciOnEvent 长度校验丢弃。
constexpr uint8_t SUBRATE_CHANGE_WRONG_LENGTH_WIRE[] = {
    0x3E,
    0x0A,
    0x23,
    0x00,
    0x42,
    0x00,
    0x64,
    0x00,
    0x1E,
    0x00,
    0x05,
    0x00,
    0x80,
    0x0C,
};

// 畸形：子事件 0x24 超过 LESUBEVENTCODE_MAX（0x23）→ LE Meta 分发丢弃
// （声明长度与实际一致 = 12，先过 HciOnEvent 长度校验再被子事件守卫拒绝）。
constexpr uint8_t SUBRATE_CHANGE_UNKNOWN_SUBEVENT_WIRE[] = {
    0x3E,
    0x0C,
    0x24,
    0x00,
    0x42,
    0x00,
    0x64,
    0x00,
    0x1E,
    0x00,
    0x05,
    0x00,
    0x80,
    0x0C,
};

// 事件 0x59 Encryption Change [v2]（7.7.8）：Parameter_Total_Length = 5：
//   Status(1)=0x00 | Connection_Handle(2)=0x0042 | Encryption_Enabled(1)=0x01 |
//   Encryption_Key_Size(1)=0x10
constexpr uint8_t ENCRYPTION_CHANGE_V2_WIRE[] = {
    0x59,
    0x05,
    0x00, // Status
    0x42,
    0x00, // Connection_Handle
    0x01, // Encryption_Enabled = ON
    0x10, // Encryption_Key_Size = 16 octets
};

// 畸形：0x59 载荷 4 字节（v1 尺寸；v2 必须是 5）→ 丢弃。
constexpr uint8_t ENCRYPTION_CHANGE_V2_SHORT_WIRE[] = {
    0x59,
    0x04,
    0x00,
    0x42,
    0x00,
    0x01,
};

// 事件码 0x5A 超过 EVENTCODE_MAX（0x59）→ HciOnEvent 丢弃。
constexpr uint8_t UNKNOWN_EVENT_CODE_WIRE[] = { 0x5A, 0x01, 0x00 };

// v1 事件 0x08 Encryption Change（回归对照）：Parameter_Total_Length = 4。
constexpr uint8_t ENCRYPTION_CHANGE_V1_WIRE[] = {
    0x08,
    0x04,
    0x00, // Status
    0x42,
    0x00, // Connection_Handle
    0x01, // Encryption_Enabled = ON
};
} // namespace

// =====================================================================
// 套件一：HCI 层同步解析（无栈 fixture，仿 stack_gap_le_5_1_event_parse_test.cpp）
// HCI 回调表是函数指针结构、不带 context 参数，故用文件级全局捕获对象
// （本套件串行执行，无并发）。HciOnEvent 为同步分发，回调在 InjectWire 返回前
// 必然已执行完毕，无需条件变量等待。
// =====================================================================

namespace {

struct SubrateChangeCapture {
    bool received = false;
    HciLeSubrateChangeEventParam param = { };
    void Reset()
    {
        *this = SubrateChangeCapture { };
    }
};

struct SubrateCompleteCapture {
    bool received = false;
    HciLeSetDefaultSubrateReturnParam param = { };
    void Reset()
    {
        *this = SubrateCompleteCapture { };
    }
};

struct SubrateRequestCompleteCapture {
    bool received = false;
    HciLeSubrateRequestReturnParam param = { };
    void Reset()
    {
        *this = SubrateRequestCompleteCapture { };
    }
};

struct EncryptionChangeV2Capture {
    bool received = false;
    HciEncryptionChangeV2EventParam param = { };
    void Reset()
    {
        *this = EncryptionChangeV2Capture { };
    }
};

struct EncryptionChangeV1Capture {
    bool received = false;
    HciEncryptionChangeEventParam param = { };
    void Reset()
    {
        *this = EncryptionChangeV1Capture { };
    }
};

struct MinKeySizeCompleteCapture {
    bool received = false;
    HciSetMinEncryptionKeySizeReturnParam param = { };
    void Reset()
    {
        *this = MinKeySizeCompleteCapture { };
    }
};

struct DataRelatedAddressChangesCompleteCapture {
    bool received = false;
    HciLeSetDataRelatedAddressChangesReturnParam param = { };
    void Reset()
    {
        *this = DataRelatedAddressChangesCompleteCapture { };
    }
};

SubrateChangeCapture g_subrateChangeCapture;
SubrateCompleteCapture g_subrateCompleteCapture;
SubrateRequestCompleteCapture g_subrateRequestCompleteCapture;
EncryptionChangeV2Capture g_encryptionChangeV2Capture;
EncryptionChangeV1Capture g_encryptionChangeV1Capture;
MinKeySizeCompleteCapture g_minKeySizeCompleteCapture;
DataRelatedAddressChangesCompleteCapture g_dataRelatedAddressChangesCompleteCapture;

static void OnLeSubrateChange(const HciLeSubrateChangeEventParam *eventParam)
{
    g_subrateChangeCapture.received = true;
    g_subrateChangeCapture.param = *eventParam;
}

static void OnLeSetDefaultSubrateComplete(const HciLeSetDefaultSubrateReturnParam *returnParam)
{
    g_subrateCompleteCapture.received = true;
    g_subrateCompleteCapture.param = *returnParam;
}

static void OnLeSubrateRequestComplete(const HciLeSubrateRequestReturnParam *returnParam)
{
    g_subrateRequestCompleteCapture.received = true;
    g_subrateRequestCompleteCapture.param = *returnParam;
}

static void OnEncryptionChangeV2(const HciEncryptionChangeV2EventParam *eventParam)
{
    g_encryptionChangeV2Capture.received = true;
    g_encryptionChangeV2Capture.param = *eventParam;
}

static void OnEncryptionChangeV1(const HciEncryptionChangeEventParam *eventParam)
{
    g_encryptionChangeV1Capture.received = true;
    g_encryptionChangeV1Capture.param = *eventParam;
}

static void OnSetMinEncryptionKeySizeComplete(const HciSetMinEncryptionKeySizeReturnParam *returnParam)
{
    g_minKeySizeCompleteCapture.received = true;
    g_minKeySizeCompleteCapture.param = *returnParam;
}

static void OnLeSetDataRelatedAddressChangesComplete(
    const HciLeSetDataRelatedAddressChangesReturnParam *returnParam)
{
    g_dataRelatedAddressChangesCompleteCapture.received = true;
    g_dataRelatedAddressChangesCompleteCapture.param = *returnParam;
}

static HciEventCallbacks g_parseCallbacks;

static void InitParseCallbacks()
{
    g_parseCallbacks = { };
    g_parseCallbacks.leSubrateChange = OnLeSubrateChange;
    g_parseCallbacks.leSetDefaultSubrateComplete = OnLeSetDefaultSubrateComplete;
    g_parseCallbacks.leSubrateRequestComplete = OnLeSubrateRequestComplete;
    g_parseCallbacks.encryptionChangeV2 = OnEncryptionChangeV2;
    g_parseCallbacks.encryptionChange = OnEncryptionChangeV1;
    g_parseCallbacks.setMinEncryptionKeySizeComplete = OnSetMinEncryptionKeySizeComplete;
    g_parseCallbacks.leSetDataRelatedAddressChangesComplete = OnLeSetDataRelatedAddressChangesComplete;
}

// 将 wire 字节构造为 HCI 事件包并送入 HciOnEvent 同步解析（与
// stack_gap_le_5_1_event_parse_test.cpp 的 InjectWire 相同）。
static void InjectWire(const uint8_t *wire, size_t size)
{
    Packet *packet = PacketMalloc(0, 0, size);
    ASSERT_NE(packet, nullptr);
    PacketPayloadWrite(packet, wire, 0, size);
    HciOnEvent(packet);
    PacketFree(packet);
}
} // namespace

class StackGapLe53EventParseTest : public testing::Test {
public:
    static void SetUpTestCase(void)
    {
        HciInitEvent();
        InitParseCallbacks();
        ASSERT_EQ(HCI_RegisterEventCallbacks(&g_parseCallbacks), BT_SUCCESS);
    }
    static void TearDownTestCase(void)
    {
        EXPECT_EQ(HCI_DeregisterEventCallbacks(&g_parseCallbacks), BT_SUCCESS);
        HciCloseEvent();
    }
    void SetUp() override
    {
        g_subrateChangeCapture.Reset();
        g_subrateCompleteCapture.Reset();
        g_subrateRequestCompleteCapture.Reset();
        g_encryptionChangeV2Capture.Reset();
        g_encryptionChangeV1Capture.Reset();
        g_minKeySizeCompleteCapture.Reset();
        g_dataRelatedAddressChangesCompleteCapture.Reset();
    }
    void TearDown() override { }
};

/**
 * @tc.number: StackGapLe53_EventParse_SubrateChange_00100
 * @tc.name:  LE Subrate Change（0x3E/0x23）全字段按线格式解析
 * @tc.desc:  注入 11 字节子事件载荷，断言 6 个字段逐一正确（含 2 字节
 *            Continuation_Number）
 */
HWTEST_F(StackGapLe53EventParseTest, StackGapLe53_EventParse_SubrateChange_00100, TestSize.Level1)
{
    InjectWire(SUBRATE_CHANGE_WIRE, sizeof(SUBRATE_CHANGE_WIRE));

    EXPECT_TRUE(g_subrateChangeCapture.received);
    EXPECT_EQ(g_subrateChangeCapture.param.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(g_subrateChangeCapture.param.connectionHandle, TEST_CONN_HANDLE);
    EXPECT_EQ(g_subrateChangeCapture.param.subrateFactor, 100);
    EXPECT_EQ(g_subrateChangeCapture.param.peripheralLatency, 30);
    EXPECT_EQ(g_subrateChangeCapture.param.continuationNumber, 5);
    EXPECT_EQ(g_subrateChangeCapture.param.supervisionTimeout, 3200);
}

/**
 * @tc.number: StackGapLe53_EventParse_SubrateChangeMax_00200
 * @tc.name:  LE Subrate Change 上限边界值解析（0x01F4/0x01F3/0x0C80）
 * @tc.desc:  全字段高 2 字节值：任何字段偏移或长度错误都会在低字节非 0 时显形；
 *            Continuation_Number=0x01F3(499) 证明其为 2 字节线格式
 */
HWTEST_F(StackGapLe53EventParseTest, StackGapLe53_EventParse_SubrateChangeMax_00200, TestSize.Level1)
{
    InjectWire(SUBRATE_CHANGE_MAX_WIRE, sizeof(SUBRATE_CHANGE_MAX_WIRE));

    EXPECT_TRUE(g_subrateChangeCapture.received);
    EXPECT_EQ(g_subrateChangeCapture.param.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(g_subrateChangeCapture.param.connectionHandle, TEST_CONN_HANDLE);
    EXPECT_EQ(g_subrateChangeCapture.param.subrateFactor, 500);
    EXPECT_EQ(g_subrateChangeCapture.param.peripheralLatency, 499);
    EXPECT_EQ(g_subrateChangeCapture.param.continuationNumber, 499);
    EXPECT_EQ(g_subrateChangeCapture.param.supervisionTimeout, 3200);
}

/**
 * @tc.number: StackGapLe53_EventParse_SubrateChangeMalformed_00300
 * @tc.name:  畸形 Subrate Change 被静默丢弃（回调不触发）
 * @tc.desc:  截断（10 字节）、声明长度与实际负载不符、未知子事件 0x24 三类
 */
HWTEST_F(StackGapLe53EventParseTest, StackGapLe53_EventParse_SubrateChangeMalformed_00300, TestSize.Level1)
{
    InjectWire(SUBRATE_CHANGE_TRUNCATED_WIRE, sizeof(SUBRATE_CHANGE_TRUNCATED_WIRE));
    EXPECT_FALSE(g_subrateChangeCapture.received);

    g_subrateChangeCapture.Reset();
    InjectWire(SUBRATE_CHANGE_WRONG_LENGTH_WIRE, sizeof(SUBRATE_CHANGE_WRONG_LENGTH_WIRE));
    EXPECT_FALSE(g_subrateChangeCapture.received);

    g_subrateChangeCapture.Reset();
    InjectWire(SUBRATE_CHANGE_UNKNOWN_SUBEVENT_WIRE, sizeof(SUBRATE_CHANGE_UNKNOWN_SUBEVENT_WIRE));
    EXPECT_FALSE(g_subrateChangeCapture.received);
}

/**
 * @tc.number: StackGapLe53_EventParse_EncryptionChangeV2_00400
 * @tc.name:  Encryption Change [v2]（0x59）全字段按线格式解析
 * @tc.desc:  5 字节载荷：Status/Connection_Handle/Encryption_Enabled/
 *            Encryption_Key_Size，keySize=0x10（16 octets）
 */
HWTEST_F(StackGapLe53EventParseTest, StackGapLe53_EventParse_EncryptionChangeV2_00400, TestSize.Level1)
{
    InjectWire(ENCRYPTION_CHANGE_V2_WIRE, sizeof(ENCRYPTION_CHANGE_V2_WIRE));

    EXPECT_TRUE(g_encryptionChangeV2Capture.received);
    EXPECT_EQ(g_encryptionChangeV2Capture.param.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(g_encryptionChangeV2Capture.param.connectionHandle, TEST_CONN_HANDLE);
    EXPECT_EQ(g_encryptionChangeV2Capture.param.encryptionEnabled, 0x01);
    EXPECT_EQ(g_encryptionChangeV2Capture.param.encryptionKeySize, 0x10);
}

/**
 * @tc.number: StackGapLe53_EventParse_EncryptionChangeV2Malformed_00500
 * @tc.name:  畸形 0x59（载荷非 5 字节）与未知事件码 0x5A 被丢弃
 */
HWTEST_F(StackGapLe53EventParseTest, StackGapLe53_EventParse_EncryptionChangeV2Malformed_00500, TestSize.Level1)
{
    InjectWire(ENCRYPTION_CHANGE_V2_SHORT_WIRE, sizeof(ENCRYPTION_CHANGE_V2_SHORT_WIRE));
    EXPECT_FALSE(g_encryptionChangeV2Capture.received);

    InjectWire(UNKNOWN_EVENT_CODE_WIRE, sizeof(UNKNOWN_EVENT_CODE_WIRE));
    EXPECT_FALSE(g_encryptionChangeV2Capture.received);
}

/**
 * @tc.number: StackGapLe53_EventParse_V1V2Isolation_00600
 * @tc.name:  0x08(v1) 与 0x59(v2) 双轨隔离
 * @tc.desc:  v1 只触发 encryptionChange、v2 只触发 encryptionChangeV2，互不误触发
 */
HWTEST_F(StackGapLe53EventParseTest, StackGapLe53_EventParse_V1V2Isolation_00600, TestSize.Level1)
{
    InjectWire(ENCRYPTION_CHANGE_V1_WIRE, sizeof(ENCRYPTION_CHANGE_V1_WIRE));
    EXPECT_TRUE(g_encryptionChangeV1Capture.received);
    EXPECT_FALSE(g_encryptionChangeV2Capture.received);
    EXPECT_EQ(g_encryptionChangeV1Capture.param.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(g_encryptionChangeV1Capture.param.connectionHandle, TEST_CONN_HANDLE);
    EXPECT_EQ(g_encryptionChangeV1Capture.param.encryptionEnabled, 0x01);

    g_encryptionChangeV1Capture.Reset();
    InjectWire(ENCRYPTION_CHANGE_V2_WIRE, sizeof(ENCRYPTION_CHANGE_V2_WIRE));
    EXPECT_FALSE(g_encryptionChangeV1Capture.received);
    EXPECT_TRUE(g_encryptionChangeV2Capture.received);
}

/**
 * @tc.number: StackGapLe53_EventParse_SetDefaultSubrateComplete_00700
 * @tc.name:  0x7D LE Set Default Subrate Command Complete 解析槽
 * @tc.desc:  直接投递 HciEventOnLeCommandComplete（status-only 载荷），status 上抛
 *            leSetDefaultSubrateComplete
 */
HWTEST_F(StackGapLe53EventParseTest, StackGapLe53_EventParse_SetDefaultSubrateComplete_00700, TestSize.Level1)
{
    const uint8_t statusOk = HCI_STATUS_SUCCESS;
    HciEventOnLeCommandComplete(HCI_LE_SET_DEFAULT_SUBRATE, &statusOk, sizeof(statusOk));
    EXPECT_TRUE(g_subrateCompleteCapture.received);
    EXPECT_EQ(g_subrateCompleteCapture.param.status, HCI_STATUS_SUCCESS);

    g_subrateCompleteCapture.Reset();
    const uint8_t statusRejected = 0x3F; // HCI_OPERATION_CANCELLED_BY_HOST 等非零状态上抛不变
    HciEventOnLeCommandComplete(HCI_LE_SET_DEFAULT_SUBRATE, &statusRejected, sizeof(statusRejected));
    EXPECT_TRUE(g_subrateCompleteCapture.received);
    EXPECT_EQ(g_subrateCompleteCapture.param.status, statusRejected);
}

/**
 * @tc.number: StackGapLe53_EventParse_SubrateRequestComplete_00800
 * @tc.name:  0x7E LE Subrate Request Command Complete 解析槽
 * @tc.desc:  status 上抛 leSubrateRequestComplete；超出 LECONTROLLER_OCF_MAX 的
 *            opcode（0x7F）不触发任何回调
 */
HWTEST_F(StackGapLe53EventParseTest, StackGapLe53_EventParse_SubrateRequestComplete_00800, TestSize.Level1)
{
    const uint8_t statusRejected = 0x02; // HCI_UNKNOWN_CONNECTION_ID：无连接句柄下发后的典型回包
    HciEventOnLeCommandComplete(HCI_LE_SUBRATE_REQUEST, &statusRejected, sizeof(statusRejected));
    EXPECT_TRUE(g_subrateRequestCompleteCapture.received);
    EXPECT_EQ(g_subrateRequestCompleteCapture.param.status, statusRejected);

    g_subrateRequestCompleteCapture.Reset();
    const uint8_t statusOk = HCI_STATUS_SUCCESS;
    HciEventOnLeCommandComplete(HCI_LE_SUBRATE_REQUEST, &statusOk, sizeof(statusOk));
    EXPECT_TRUE(g_subrateRequestCompleteCapture.received);
    EXPECT_EQ(g_subrateRequestCompleteCapture.param.status, HCI_STATUS_SUCCESS);

    // OCF 0x7F > LECONTROLLER_OCF_MAX（0x7E）：LE Controller OGF 下未实现 opcode，丢弃。
    g_subrateCompleteCapture.Reset();
    g_subrateRequestCompleteCapture.Reset();
    const uint16_t unknownOpcode = 0x207F; // LE OGF << 10 | 0x7F
    HciEventOnLeCommandComplete(unknownOpcode, &statusOk, sizeof(statusOk));
    EXPECT_FALSE(g_subrateCompleteCapture.received);
    EXPECT_FALSE(g_subrateRequestCompleteCapture.received);
}

/**
 * @tc.number: StackGapLe53_EventParse_SetMinEncryptionKeySizeComplete_00900
 * @tc.name:  0x0084 Set Min Encryption Key Size Command Complete 解析槽
 * @tc.desc:  status 上抛 setMinEncryptionKeySizeComplete（BR Command Complete 链）
 */
HWTEST_F(StackGapLe53EventParseTest, StackGapLe53_EventParse_SetMinEncryptionKeySizeComplete_00900, TestSize.Level1)
{
    const uint8_t statusOk = HCI_STATUS_SUCCESS;
    HciEventOnControllerBasebandCommandComplete(HCI_SET_MIN_ENCRYPTION_KEY_SIZE, &statusOk, sizeof(statusOk));
    EXPECT_TRUE(g_minKeySizeCompleteCapture.received);
    EXPECT_EQ(g_minKeySizeCompleteCapture.param.status, HCI_STATUS_SUCCESS);
}

/**
 * @tc.number: StackGapLe53_EventParse_SetDataRelatedAddressChangesComplete_01000
 * @tc.name:  0x7C LE Set Data Related Address Changes Command Complete 解析槽
 * @tc.desc:  直接投递 HciEventOnLeCommandComplete（status-only 载荷），status 上抛
 *            leSetDataRelatedAddressChangesComplete
 */
HWTEST_F(StackGapLe53EventParseTest, StackGapLe53_EventParse_SetDataRelatedAddressChangesComplete_01000,
    TestSize.Level1)
{
    const uint8_t statusOk = HCI_STATUS_SUCCESS;
    HciEventOnLeCommandComplete(HCI_LE_SET_DATA_RELATED_ADDRESS_CHANGES, &statusOk, sizeof(statusOk));
    EXPECT_TRUE(g_dataRelatedAddressChangesCompleteCapture.received);
    EXPECT_EQ(g_dataRelatedAddressChangesCompleteCapture.param.status, HCI_STATUS_SUCCESS);

    g_dataRelatedAddressChangesCompleteCapture.Reset();
    const uint8_t statusRejected = 0x3F; // 非零状态上抛不变
    HciEventOnLeCommandComplete(HCI_LE_SET_DATA_RELATED_ADDRESS_CHANGES, &statusRejected, sizeof(statusRejected));
    EXPECT_TRUE(g_dataRelatedAddressChangesCompleteCapture.received);
    EXPECT_EQ(g_dataRelatedAddressChangesCompleteCapture.param.status, statusRejected);
}

// =====================================================================
// 套件二：活栈（BTM 初始化，仿 stack_gap_le_5_2_test.cpp fixture；不依赖对端）
// =====================================================================

namespace {

struct CallbackWaiter {
    std::mutex mtx;
    std::condition_variable cv;
    bool received = false;

    bool Wait()
    {
        std::unique_lock<std::mutex> lock(mtx);
        return cv.wait_for(lock, std::chrono::milliseconds(WAIT_CALLBACK_TIMEOUT_MS), [this] {
            return received;
        });
    }

    bool Wait(uint32_t timeoutMs)
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

struct SubrateChangeResult : CallbackWaiter {
    GapLeSubrateChangeInfo info = { };
};

void OnSubrateChange(const GapLeSubrateChangeInfo *info, void *context)
{
    if (info == nullptr) {
        return;
    }
    auto *result = static_cast<SubrateChangeResult *>(context);
    result->info = *info;
    result->Notify();
}

// RAII guard：用例结束（含断言失败）时自动注销 GapLeSubrateCallback。
class GapLeSubrateCallbackGuard {
public:
    ~GapLeSubrateCallbackGuard()
    {
        if (GAPIF_LeDeregisterSubrateCallback() != BT_SUCCESS) {
            printf("GapLeSubrateCallbackGuard: deregister failed; callback may remain registered\n");
        }
    }
    GapLeSubrateCallbackGuard(const GapLeSubrateCallbackGuard &) = delete;
    GapLeSubrateCallbackGuard &operator=(const GapLeSubrateCallbackGuard &) = delete;
    GapLeSubrateCallbackGuard() = default;
};

// 合法的 7.8.124 参数组合（Subrate_Max × (Max_Latency+1) = 100×5 = 500 ≤ 500）。
constexpr uint16_t VALID_SUBRATE_MIN = 10;
constexpr uint16_t VALID_SUBRATE_MAX = 100;
constexpr uint16_t VALID_MAX_LATENCY = 4;
constexpr uint16_t VALID_CONTINUATION_NUMBER = 5;
constexpr uint16_t VALID_SUPERVISION_TIMEOUT = 0x0C80; // 32 s
} // namespace

class StackGapLe53Test : public testing::Test {
public:
    static void SetUpTestCase(void)
    {
        ASSERT_EQ(BTM_Initialize(), BT_SUCCESS);
        ASSERT_EQ(BTM_Enable(LE_CONTROLLER), BT_SUCCESS);
        ASSERT_TRUE(BTM_IsEnabled(LE_CONTROLLER));
        // 周期广播类命令（GAP_LePeriodicAdvSetEnableWithAdi 等）需要 BROADCASTER/
        // PERIPHERAL 角色；连接子速率命令无角色门（见 gap_le_subrate.c）。
        ASSERT_EQ(GAPIF_LeSetRole(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_PERIPHERAL), BT_SUCCESS);
    }
    static void TearDownTestCase(void)
    {
        EXPECT_EQ(BTM_Disable(LE_CONTROLLER), BT_SUCCESS);
        EXPECT_EQ(BTM_Close(), BT_SUCCESS);
    }
    void SetUp() override { }
    void TearDown() override { }
};

/**
 * @tc.number: StackGapLe53_GapifSubrateRequestParamGate_00100
 * @tc.name:  GAPIF_LeSubrateRequest 参数门控
 * @tc.desc:  连接句柄越界、7.8.124 全部数值约束违反 → BT_BAD_PARAM，命令不下发
 */
HWTEST_F(StackGapLe53Test, StackGapLe53_GapifSubrateRequestParamGate_00100, TestSize.Level1)
{
    // 连接句柄越界（0x0F00 > 0x0EFF）。
    GapLeSubrateRequestParams pHandleTooHigh = { 0x0F00, VALID_SUBRATE_MIN, VALID_SUBRATE_MAX,
        VALID_MAX_LATENCY, VALID_CONTINUATION_NUMBER, VALID_SUPERVISION_TIMEOUT };
    EXPECT_EQ(GAPIF_LeSubrateRequest(&pHandleTooHigh), BT_BAD_PARAM);
    // Subrate_Min/Subrate_Max 必须 ≥ 1（0 保留）。
    GapLeSubrateRequestParams pMinZero = { 0x0001, 0, VALID_SUBRATE_MAX, VALID_MAX_LATENCY,
        VALID_CONTINUATION_NUMBER, VALID_SUPERVISION_TIMEOUT };
    EXPECT_EQ(GAPIF_LeSubrateRequest(&pMinZero), BT_BAD_PARAM);
    GapLeSubrateRequestParams pMaxZero = { 0x0001, VALID_SUBRATE_MIN, 0, VALID_MAX_LATENCY,
        VALID_CONTINUATION_NUMBER, VALID_SUPERVISION_TIMEOUT };
    EXPECT_EQ(GAPIF_LeSubrateRequest(&pMaxZero), BT_BAD_PARAM);
    // Subrate_Max > 500（0x01F4）。
    GapLeSubrateRequestParams pMaxTooHigh = { 0x0001, VALID_SUBRATE_MIN, 0x01F5, VALID_MAX_LATENCY,
        VALID_CONTINUATION_NUMBER, VALID_SUPERVISION_TIMEOUT };
    EXPECT_EQ(GAPIF_LeSubrateRequest(&pMaxTooHigh), BT_BAD_PARAM);
    // Subrate_Min > Subrate_Max。
    GapLeSubrateRequestParams pMinAboveMax = { 0x0001, VALID_SUBRATE_MIN, 0x0005, VALID_MAX_LATENCY,
        VALID_CONTINUATION_NUMBER, VALID_SUPERVISION_TIMEOUT };
    EXPECT_EQ(GAPIF_LeSubrateRequest(&pMinAboveMax), BT_BAD_PARAM);
    // Max_Latency > 499（0x01F3）。
    GapLeSubrateRequestParams pLatencyTooHigh = { 0x0001, VALID_SUBRATE_MIN, VALID_SUBRATE_MAX, 0x01F4,
        VALID_CONTINUATION_NUMBER, VALID_SUPERVISION_TIMEOUT };
    EXPECT_EQ(GAPIF_LeSubrateRequest(&pLatencyTooHigh), BT_BAD_PARAM);
    // Subrate_Max × (Max_Latency + 1) > 500。
    GapLeSubrateRequestParams pProductTooHigh = { 0x0001, VALID_SUBRATE_MIN, 0x01F4, 1,
        VALID_CONTINUATION_NUMBER, VALID_SUPERVISION_TIMEOUT };
    EXPECT_EQ(GAPIF_LeSubrateRequest(&pProductTooHigh), BT_BAD_PARAM);
    // Continuation_Number ≥ Subrate_Max。
    GapLeSubrateRequestParams pContGeMax = { 0x0001, VALID_SUBRATE_MIN, VALID_SUBRATE_MAX,
        VALID_MAX_LATENCY, VALID_SUBRATE_MAX, VALID_SUPERVISION_TIMEOUT };
    EXPECT_EQ(GAPIF_LeSubrateRequest(&pContGeMax), BT_BAD_PARAM);
    // Continuation_Number > 499。
    GapLeSubrateRequestParams pContTooHigh = { 0x0001, VALID_SUBRATE_MIN, VALID_SUBRATE_MAX,
        VALID_MAX_LATENCY, 0x01F4, VALID_SUPERVISION_TIMEOUT };
    EXPECT_EQ(GAPIF_LeSubrateRequest(&pContTooHigh), BT_BAD_PARAM);
    // Supervision_Timeout 越界（< 100 ms / > 32 s）。
    GapLeSubrateRequestParams pSvTooLow = { 0x0001, VALID_SUBRATE_MIN, VALID_SUBRATE_MAX,
        VALID_MAX_LATENCY, VALID_CONTINUATION_NUMBER, 0x0009 };
    EXPECT_EQ(GAPIF_LeSubrateRequest(&pSvTooLow), BT_BAD_PARAM);
    GapLeSubrateRequestParams pSvTooHigh = { 0x0001, VALID_SUBRATE_MIN, VALID_SUBRATE_MAX,
        VALID_MAX_LATENCY, VALID_CONTINUATION_NUMBER, 0x0C81 };
    EXPECT_EQ(GAPIF_LeSubrateRequest(&pSvTooHigh), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe53_GapifSetDefaultSubrateParamGate_00200
 * @tc.name:  GAPIF_LeSetDefaultSubrate 参数门控
 * @tc.desc:  NULL 参数与非法数值 → BT_BAD_PARAM
 */
HWTEST_F(StackGapLe53Test, StackGapLe53_GapifSetDefaultSubrateParamGate_00200, TestSize.Level1)
{
    EXPECT_EQ(GAPIF_LeSetDefaultSubrate(nullptr), BT_BAD_PARAM);

    GapLeSubrateDefaultParams params = {
        .defaultSubrateMin = 0, // 0 保留
        .defaultSubrateMax = VALID_SUBRATE_MAX,
        .defaultMaxLatency = VALID_MAX_LATENCY,
        .defaultContinuationNumber = VALID_CONTINUATION_NUMBER,
        .defaultSupervisionTimeout = VALID_SUPERVISION_TIMEOUT,
    };
    EXPECT_EQ(GAPIF_LeSetDefaultSubrate(&params), BT_BAD_PARAM);

    params.defaultSubrateMin = VALID_SUBRATE_MIN;
    params.defaultSubrateMax = 600; // > 500
    EXPECT_EQ(GAPIF_LeSetDefaultSubrate(&params), BT_BAD_PARAM);

    params.defaultSubrateMax = VALID_SUBRATE_MAX;
    params.defaultMaxLatency = 0x01F4; // > 499
    EXPECT_EQ(GAPIF_LeSetDefaultSubrate(&params), BT_BAD_PARAM);

    params.defaultMaxLatency = VALID_MAX_LATENCY;
    params.defaultContinuationNumber = VALID_SUBRATE_MAX; // ≥ Subrate_Max
    EXPECT_EQ(GAPIF_LeSetDefaultSubrate(&params), BT_BAD_PARAM);

    params.defaultContinuationNumber = VALID_CONTINUATION_NUMBER;
    params.defaultSupervisionTimeout = 0x0009; // < 100 ms
    EXPECT_EQ(GAPIF_LeSetDefaultSubrate(&params), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe53_GapifRegisterSubrateCallback_00300
 * @tc.name:  GAPIF_RegisterLeSubrateCallback/Deregister
 * @tc.desc:  register/deregister 均返回 BT_SUCCESS，deregister 幂等（重复调用）
 */
HWTEST_F(StackGapLe53Test, StackGapLe53_GapifRegisterSubrateCallback_00300, TestSize.Level1)
{
    GapLeSubrateCallback cb = { };
    cb.subrateChange = OnSubrateChange;
    EXPECT_EQ(GAPIF_RegisterLeSubrateCallback(&cb, nullptr), BT_SUCCESS);
    EXPECT_EQ(GAPIF_LeDeregisterSubrateCallback(), BT_SUCCESS);
    EXPECT_EQ(GAPIF_LeDeregisterSubrateCallback(), BT_SUCCESS);
}

/**
 * @tc.number: StackGapLe53_SubrateChangeE2E_00400
 * @tc.name:  活栈注入 LE Subrate Change → GapLeSubrateCallback.subrateChange
 * @tc.desc:  全生产路径：HCI RX 注入 0x3E/0x23 wire → gap_hci_receive 表行 → GAP
 *            任务 → 注册回调，字段与线格式逐一对应（无真实连接依赖，GAP 层为
 *            无状态透传）
 */
HWTEST_F(StackGapLe53Test, StackGapLe53_SubrateChangeE2E_00400, TestSize.Level1)
{
    SubrateChangeResult result;
    GapLeSubrateCallback cb = { };
    cb.subrateChange = OnSubrateChange;
    ASSERT_EQ(GAPIF_RegisterLeSubrateCallback(&cb, &result), BT_SUCCESS);
    GapLeSubrateCallbackGuard guard;

    ASSERT_EQ(HCI_InjectReceivedEvent(SUBRATE_CHANGE_WIRE, sizeof(SUBRATE_CHANGE_WIRE)), BT_SUCCESS);

    ASSERT_TRUE(result.Wait()) << "subrateChange not delivered through the GAP task";
    EXPECT_EQ(result.info.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(result.info.connectionHandle, TEST_CONN_HANDLE);
    EXPECT_EQ(result.info.subrateFactor, 100);
    EXPECT_EQ(result.info.peripheralLatency, 30);
    EXPECT_EQ(result.info.continuationNumber, 5);
    EXPECT_EQ(result.info.supervisionTimeout, 3200);
}

/**
 * @tc.number: StackGapLe53_SubrateChangeNotDeliveredAfterDeregister_00500
 * @tc.name:  注销后事件不再投递
 * @tc.desc:  注入一次并确认送达 → 注销回调 → 再注入同类事件 → 500ms 内无回调
 */
HWTEST_F(StackGapLe53Test, StackGapLe53_SubrateChangeNotDeliveredAfterDeregister_00500, TestSize.Level1)
{
    SubrateChangeResult result;
    GapLeSubrateCallback cb = { };
    cb.subrateChange = OnSubrateChange;
    ASSERT_EQ(GAPIF_RegisterLeSubrateCallback(&cb, &result), BT_SUCCESS);
    GapLeSubrateCallbackGuard guard;

    ASSERT_EQ(HCI_InjectReceivedEvent(SUBRATE_CHANGE_MAX_WIRE, sizeof(SUBRATE_CHANGE_MAX_WIRE)), BT_SUCCESS);
    ASSERT_TRUE(result.Wait()) << "subrateChange not delivered before deregister";
    EXPECT_EQ(result.info.subrateFactor, 500);

    ASSERT_EQ(GAPIF_LeDeregisterSubrateCallback(), BT_SUCCESS);

    result.Reset();
    ASSERT_EQ(HCI_InjectReceivedEvent(SUBRATE_CHANGE_MAX_WIRE, sizeof(SUBRATE_CHANGE_MAX_WIRE)), BT_SUCCESS);
    EXPECT_FALSE(result.Wait(500)) << "subrateChange must not fire after deregister";
}

/**
 * @tc.number: StackGapLe53_GapifSubrateRequestValid_00600
 * @tc.name:  GAPIF_LeSubrateRequest / GAPIF_LeSetDefaultSubrate 合法参数
 * @tc.desc:  命令接受下发，返回值非参数错误/内存不足（命令层回包不可依赖，沿用
 *            5.2 断言策略，开发计划 §6.1 R6）
 */
HWTEST_F(StackGapLe53Test, StackGapLe53_GapifSubrateRequestValid_00600, TestSize.Level1)
{
    GapLeSubrateRequestParams reqParams = { 0x0001, VALID_SUBRATE_MIN, VALID_SUBRATE_MAX, VALID_MAX_LATENCY,
        VALID_CONTINUATION_NUMBER, VALID_SUPERVISION_TIMEOUT };
    int ret = GAPIF_LeSubrateRequest(&reqParams);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);

    GapLeSubrateDefaultParams params = {
        .defaultSubrateMin = VALID_SUBRATE_MIN,
        .defaultSubrateMax = VALID_SUBRATE_MAX,
        .defaultMaxLatency = VALID_MAX_LATENCY,
        .defaultContinuationNumber = VALID_CONTINUATION_NUMBER,
        .defaultSupervisionTimeout = VALID_SUPERVISION_TIMEOUT,
    };
    ret = GAPIF_LeSetDefaultSubrate(&params);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe53_HciCmdParamGate_00700
 * @tc.name:  5.3 HCI 发送器参数门控（NULL/数值越界）
 * @tc.desc:  非法输入 → BT_BAD_PARAM 且命令不下发；合法 0x0084 输入被接受
 */
HWTEST_F(StackGapLe53Test, StackGapLe53_HciCmdParamGate_00700, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeSetDefaultSubrate(nullptr), BT_BAD_PARAM);
    EXPECT_EQ(HCI_LeSubrateRequest(nullptr), BT_BAD_PARAM);
    EXPECT_EQ(HCI_SetMinEncryptionKeySize(nullptr), BT_BAD_PARAM);

    // 0x0084 取值范围 0x01-0x10（Vol 4 Part E 7.3.102）。
    HciSetMinEncryptionKeySizeParam badSize = { 0x00 };
    EXPECT_EQ(HCI_SetMinEncryptionKeySize(&badSize), BT_BAD_PARAM);
    badSize.minEncryptionKeySize = 0x11;
    EXPECT_EQ(HCI_SetMinEncryptionKeySize(&badSize), BT_BAD_PARAM);

    HciSetMinEncryptionKeySizeParam goodSize = { 0x10 };
    int ret = HCI_SetMinEncryptionKeySize(&goodSize);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe53_GapifPeriodicAdvSetEnableWithAdi_00800
 * @tc.name:  GAPIF_LePeriodicAdvSetEnableWithAdi 门控与 ADI 能力预检
 * @tc.desc:  Enable > 0x03 / 句柄越界 → BT_BAD_PARAM；enable bit1 而控制器无
 *            bit36（Periodic Advertising ADI Support）→ BT_NOT_SUPPORT；控制器
 *            支持时合法下发
 */
HWTEST_F(StackGapLe53Test, StackGapLe53_GapifPeriodicAdvSetEnableWithAdi_00800, TestSize.Level1)
{
    // Enable 0x04 含保留位（bit0 Enable + bit1 Include ADI 之外）。
    EXPECT_EQ(GAPIF_LePeriodicAdvSetEnableWithAdi(0x04, 0x00), BT_BAD_PARAM);
    // 广播句柄越界（0xF0 > 0xEF）。
    EXPECT_EQ(GAPIF_LePeriodicAdvSetEnableWithAdi(0x01, 0xF0), BT_BAD_PARAM);

    if (!BTM_IsControllerSupportLePeriodicAdvAdiSupport()) {
        // 控制器不支持 bit36：请求 ADI 字段同步失败（预检，仿
        // GapLeSetConnectionlessCteTransmitParams 的 GAP_ERR_NOT_SUPPORT 模式）。
        EXPECT_EQ(GAPIF_LePeriodicAdvSetEnableWithAdi(0x03, 0x00), BT_NOT_SUPPORT);
        return;
    }

    int ret = GAPIF_LePeriodicAdvSetEnableWithAdi(0x01, 0x00); // 仅使能，无 ADI
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
    ret = GAPIF_LePeriodicAdvSetEnableWithAdi(0x03, 0x00); // 使能 + 含 ADI
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}
} // namespace Bluetooth
} // namespace OHOS
