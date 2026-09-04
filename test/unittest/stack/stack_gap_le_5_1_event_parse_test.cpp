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

// 蓝牙 5.1 新增 LE 事件解析单测：wire 字节 → HciOnEvent → HciEventCallbacks。
// 与 interact 测试不同，本文件不依赖真实控制器和对端设备：测试进程内直接初始化
// HCI 事件回调表（HciInitEvent），将手工构造的 wire 字节（与规范线格式逐字节一致）
// 喂给事件分发入口 HciOnEvent，同步断言解析结果。这也是 interact 测试新增的
// 测试缝 HCI_InjectReceivedEvent（见 hci.h / hci.c）所走事件解析路径的确定性验证。
//
// 覆盖的 5.1 事件（LE Meta，Event_Code = 0x3E）：
//   0x15 LE Connectionless IQ Report（7.7.65,21，12 固定字节 + 2*Sample_Count 交错 I/Q）
//   0x16 LE Connection IQ Report（7.7.65,22，13 固定字节 + 2*Sample_Count 交错 I/Q）
//   0x17 LE CTE Request Failed（7.7.65,23，3 字节）
//   0x18 LE Periodic Advertising Sync Transfer Received（7.7.65,24，19 字节）
// 0x19+（Set Info Transfer Received / Sync Established (v2) 等）不在本 5.1 升级范围。
// 注意：0x19 LE CIS Established 自 5.2 起已有处理器（hci_evt_le.c），LESUBEVENTCODE_MAX
// 已在 5.3 提升到 0x23，故"未知子事件"负向探针改用 0x24（见 UnknownSubevent 用例）。

#include <gtest/gtest.h>

#include "securec.h"

#include <cstring>

#include "hci/evt/hci_evt.h"
#include "hci/hci.h"

using namespace testing::ext;

namespace OHOS {
namespace Bluetooth {
namespace {
// ---------------- wire 字节（完整 HCI 事件包：Event_Code + Parameter_Total_Length + parameters） ----------------

// Subevent 0x15 LE Connectionless IQ Report（7.7.65,21），12 固定字节 + 4 个交错样本：
//   Sync_Handle(2)=0x1234 | Channel_Index(1)=5 | RSSI(2)=0xFFCE(-50，-5.0 dBm) |
//   RSSI_Antenna_ID(1)=1 | CTE_Type(1)=0x02(AoD 2us) | Slot_Durations(1)=0x01(1us) |
//   Packet_Status(1)=0x00(CRC OK) | paEventCounter(2)=0x0102 | Sample_Count(1)=2 |
//   I/Q 样本(交错 I0,Q0,I1,Q1)：-10, +20, -30, +40
constexpr uint8_t CONNLESS_IQ_REPORT_WIRE[] = {
    0x3E, 0x11,            // LE Meta, Parameter_Total_Length = 17
    0x15,                  // Subevent: LE Connectionless IQ Report
    0x34, 0x12,            // Sync_Handle
    0x05,                  // Channel_Index
    0xCE, 0xFF,            // RSSI = -50 (0.1 dBm)
    0x01,                  // RSSI_Antenna_ID
    0x02,                  // CTE_Type = AoD 2 us
    0x01,                  // Slot_Durations = 1 us
    0x00,                  // Packet_Status = CRC OK
    0x02, 0x01,            // paEventCounter = 0x0102
    0x02,                  // Sample_Count = 2
    0xF6, 0x14, 0xE2, 0x28 // I0=-10 Q0=+20 I1=-30 Q1=+40
};

// Subevent 0x16 LE Connection IQ Report（7.7.65,22），13 固定字节 + 4 个交错样本：
//   Connection_Handle(2)=0x0008 | RX_PHY(1)=0x01(LE 1M) | Data_Channel_Index(1)=9 |
//   RSSI(2)=0xFFC4(-60，-6.0 dBm) | RSSI_Antenna_ID(1)=2 | CTE_Type(1)=0x00(AoA) |
//   Slot_Durations(1)=0x02(2us) | Packet_Status(1)=0x00(CRC OK) | connEventCounter(2)=0x0042 |
//   Sample_Count(1)=2 | I/Q 样本(交错)：+10, -10, +20, -30
constexpr uint8_t CONN_IQ_REPORT_WIRE[] = {
    0x3E, 0x12,            // LE Meta, Parameter_Total_Length = 18
    0x16,                  // Subevent: LE Connection IQ Report
    0x08, 0x00,            // Connection_Handle
    0x01,                  // RX_PHY = LE 1M
    0x09,                  // Data_Channel_Index
    0xC4, 0xFF,            // RSSI = -60 (0.1 dBm)
    0x02,                  // RSSI_Antenna_ID
    0x00,                  // CTE_Type = AoA
    0x02,                  // Slot_Durations = 2 us
    0x00,                  // Packet_Status = CRC OK
    0x42, 0x00,            // connEventCounter = 0x0042
    0x02,                  // Sample_Count = 2
    0x0A, 0xF6, 0x14, 0xE2 // I0=+10 Q0=-10 I1=+20 Q1=-30
};

// Subevent 0x17 LE CTE Request Failed（7.7.65,23）：Status(1)=0x01(对端拒绝) |
// Connection_Handle(2)=0x0008
constexpr uint8_t CTE_REQUEST_FAILED_WIRE[] = {
    0x3E, 0x04,            // LE Meta, Parameter_Total_Length = 4
    0x17,                  // Subevent: LE CTE Request Failed
    0x01,                  // Status = 0x01 (peer rejected)
    0x08, 0x00,            // Connection_Handle
};

// Subevent 0x18 LE Periodic Advertising Sync Transfer Received（7.7.65,24），19 字节：
//   Status(1)=0x00(同步已建立) | Connection_Handle(2)=0x0008 | Service_Data(2)=0x1234 |
//   Sync_Handle(2)=0x0567 | Advertising_SID(1)=3 | Advertiser_Address_Type(1)=0x00(public) |
//   Advertiser_Address(6)=AA:BB:CC:DD:EE:FF（wire 按 LE 序）| Advertiser_PHY(1)=0x01(LE 1M) |
//   Periodic_Advertising_Interval(2)=0x00A0(100 ms) | Advertiser_Clock_Accuracy(1)=1
constexpr uint8_t PAST_SYNC_TRANSFER_RECEIVED_WIRE[] = {
    0x3E, 0x14,            // LE Meta, Parameter_Total_Length = 20
    0x18,                  // Subevent: LE Periodic Advertising Sync Transfer Received
    0x00,                  // Status
    0x08, 0x00,            // Connection_Handle
    0x34, 0x12,            // Service_Data
    0x67, 0x05,            // Sync_Handle
    0x03,                  // Advertising_SID
    0x00,                  // Advertiser_Address_Type = public
    0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, // Advertiser_Address = AA:BB:CC:DD:EE:FF
    0x01,                  // Advertiser_PHY = LE 1M
    0xA0, 0x00,            // Periodic_Advertising_Interval = 100 ms
    0x01,                  // Advertiser_Clock_Accuracy
};

// ---- 畸形 wire 字节（均应被丢弃，回调不得触发） ----

// 0x15 固定部分只有 10 字节（规范要求 >= 12）。
constexpr uint8_t TRUNCATED_CONNLESS_IQ_WIRE[] = {
    0x3E, 0x0B, 0x15,
    0x34, 0x12, 0x05, 0xCE, 0xFF, 0x01, 0x02, 0x01, 0x00, 0x02,
};

// 声明长度 16，实际负载 17（HciOnEvent 长度校验应丢弃）。
constexpr uint8_t WRONG_DECLARED_LENGTH_WIRE[] = {
    0x3E, 0x10, 0x15,
    0x34, 0x12, 0x05, 0xCE, 0xFF, 0x01, 0x02, 0x01, 0x00, 0x02, 0x01, 0x02,
    0xF6, 0x14, 0xE2, 0x28,
};

// 0x15 Sample_Count = 0x53（> HCI_LE_IQ_SAMPLE_COUNT_MAX 0x52）→ 处理器拒绝。
constexpr uint8_t SAMPLE_COUNT_OVERFLOW_WIRE[] = {
    0x3E, 0x11, 0x15,
    0x34, 0x12, 0x05, 0xCE, 0xFF, 0x01, 0x02, 0x01, 0x00, 0x02, 0x01, 0x53,
    0xF6, 0x14, 0xE2, 0x28,
};

// 0x18 只有 18 字节（规范要求 19）。
constexpr uint8_t TRUNCATED_PAST_WIRE[] = {
    0x3E, 0x13, 0x18,
    0x00, 0x08, 0x00, 0x34, 0x12, 0x67, 0x05, 0x03, 0x00,
    0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x01, 0xA0, 0x00,
};

// 子事件 0x24 超过 LESUBEVENTCODE_MAX（0x23，5.3 起 0x23 已被 LE Subrate Change 占用；
// 0x19 LE CIS Established 自 5.2 起已有处理器，见 stack_gap_le_5_3_test.cpp）→ LE Meta 分发丢弃。
constexpr uint8_t UNKNOWN_SUBEVENT_WIRE[] = {
    0x3E, 0x14, 0x24,
    0x00, 0x08, 0x00, 0x34, 0x12, 0x67, 0x05, 0x03, 0x00,
    0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x01, 0xA0, 0x00, 0x01,
};

// 事件码 0x5A 超过 EVENTCODE_MAX（0x59，5.3 起 Encryption Change [v2] 占用了 0x59，
// 见 stack_gap_le_5_3_test.cpp）→ HciOnEvent 丢弃。
constexpr uint8_t UNKNOWN_EVENT_CODE_WIRE[] = {0x5A, 0x01, 0x00};

// ---------------- 回调捕获 ----------------
// HCI 回调表是函数指针结构、不带 context 参数，故用文件级全局捕获对象
// （本套件串行执行，无并发）。HciOnEvent 为同步分发，回调在 InjectWire 返回
// 前必然已执行完毕，无需条件变量等待。

struct EventCapture {
    bool connlessIqReceived = false;
    HciLeConnectionlessIqReportEventParam connlessIq = {};
    int8_t connlessIqSamples[2 * 8] = {};

    bool connIqReceived = false;
    HciLeConnectionIqReportEventParam connIq = {};
    int8_t connIqSamples[2 * 8] = {};

    bool cteRequestFailedReceived = false;
    HciLeCteRequestFailedEventParam cteRequestFailed = {};

    bool pastReceived = false;
    HciLePeriodicAdvertisingSyncTransferReceivedEventParam past = {};

    void Reset()
    {
        *this = EventCapture{};
    }
};

EventCapture g_capture;

static void OnConnlessIqReport(const HciLeConnectionlessIqReportEventParam *eventParam)
{
    g_capture.connlessIqReceived = true;
    g_capture.connlessIq = *eventParam;
    // iqSamples 是事件包缓冲的零拷贝视图，包释放后即失效，必须深拷贝。
    if (eventParam->sampleCount > 0 && eventParam->iqSamples != NULL &&
        eventParam->sampleCount <= sizeof(g_capture.connlessIqSamples) / HCI_LE_IQ_SAMPLE_OCTETS) {
        (void)memcpy_s(g_capture.connlessIqSamples, sizeof(g_capture.connlessIqSamples), eventParam->iqSamples,
            (size_t)eventParam->sampleCount * HCI_LE_IQ_SAMPLE_OCTETS);
    }
    g_capture.connlessIq.iqSamples = NULL; // 悬空指针不应被误用
}

static void OnConnIqReport(const HciLeConnectionIqReportEventParam *eventParam)
{
    g_capture.connIqReceived = true;
    g_capture.connIq = *eventParam;
    if (eventParam->sampleCount > 0 && eventParam->iqSamples != NULL &&
        eventParam->sampleCount <= sizeof(g_capture.connIqSamples) / HCI_LE_IQ_SAMPLE_OCTETS) {
        (void)memcpy_s(g_capture.connIqSamples, sizeof(g_capture.connIqSamples), eventParam->iqSamples,
            (size_t)eventParam->sampleCount * HCI_LE_IQ_SAMPLE_OCTETS);
    }
    g_capture.connIq.iqSamples = NULL;
}

static void OnCteRequestFailed(const HciLeCteRequestFailedEventParam *eventParam)
{
    g_capture.cteRequestFailedReceived = true;
    g_capture.cteRequestFailed = *eventParam;
}

static void OnPastSyncTransferReceived(const HciLePeriodicAdvertisingSyncTransferReceivedEventParam *eventParam)
{
    g_capture.pastReceived = true;
    g_capture.past = *eventParam;
}

static HciEventCallbacks g_parseCallbacks;

static void InitParseCallbacks()
{
    g_parseCallbacks = {};
    g_parseCallbacks.leConnectionlessIqReport = OnConnlessIqReport;
    g_parseCallbacks.leConnectionIqReport = OnConnIqReport;
    g_parseCallbacks.leCteRequestFailed = OnCteRequestFailed;
    g_parseCallbacks.lePeriodicAdvertisingSyncTransferReceived = OnPastSyncTransferReceived;
}

// 将 wire 字节构造为 HCI 事件包（与 HciOnReceivedHciPacket 相同的包布局）并送入
// HciOnEvent 同步解析。
static void InjectWire(const uint8_t *wire, size_t size)
{
    Packet *packet = PacketMalloc(0, 0, size);
    ASSERT_NE(packet, nullptr);
    PacketPayloadWrite(packet, wire, 0, size);
    HciOnEvent(packet);
    PacketFree(packet);
}
} // namespace

class StackGapLe51EventParseTest : public testing::Test {
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
        g_capture.Reset();
    }
    void TearDown() override {}
};

// @tc.number: StackGapLe5_1EventParse_ConnlessIqReport_00100
// @tc.name:  0x15 LE Connectionless IQ Report 全部字段按线格式解析
// @tc.desc:  注入 12 固定字节 + 2 个交错 (I, Q) 对，断言每个字段与 I/Q 交错顺序
HWTEST_F(StackGapLe51EventParseTest, StackGapLe5_1EventParse_ConnlessIqReport_00100, TestSize.Level1)
{
    InjectWire(CONNLESS_IQ_REPORT_WIRE, sizeof(CONNLESS_IQ_REPORT_WIRE));

    EXPECT_TRUE(g_capture.connlessIqReceived);
    EXPECT_EQ(g_capture.connlessIq.syncHandle, 0x1234);
    EXPECT_EQ(g_capture.connlessIq.channelIndex, 0x05);
    EXPECT_EQ(g_capture.connlessIq.rssi, -50); // -5.0 dBm
    EXPECT_EQ(g_capture.connlessIq.rssiAntennaId, 0x01);
    EXPECT_EQ(g_capture.connlessIq.cteType, HCI_LE_CTE_TYPE_AOD_2US);
    EXPECT_EQ(g_capture.connlessIq.slotDurations, 0x01);
    EXPECT_EQ(g_capture.connlessIq.packetStatus, 0x00);
    EXPECT_EQ(g_capture.connlessIq.paEventCounter, 0x0102);
    EXPECT_EQ(g_capture.connlessIq.sampleCount, 0x02);
    // 交错 (I, Q) 对：I0, Q0, I1, Q1
    EXPECT_EQ(g_capture.connlessIqSamples[0], -10);
    EXPECT_EQ(g_capture.connlessIqSamples[1], 20);
    EXPECT_EQ(g_capture.connlessIqSamples[2], -30);
    EXPECT_EQ(g_capture.connlessIqSamples[3], 40);
}

// @tc.number: StackGapLe5_1EventParse_ConnIqReport_00200
// @tc.name:  0x16 LE Connection IQ Report 全部字段按线格式解析
// @tc.desc:  注入 13 固定字节 + 2 个交错 (I, Q) 对，断言每个字段与 I/Q 交错顺序
HWTEST_F(StackGapLe51EventParseTest, StackGapLe5_1EventParse_ConnIqReport_00200, TestSize.Level1)
{
    InjectWire(CONN_IQ_REPORT_WIRE, sizeof(CONN_IQ_REPORT_WIRE));

    EXPECT_TRUE(g_capture.connIqReceived);
    EXPECT_EQ(g_capture.connIq.connectionHandle, 0x0008);
    EXPECT_EQ(g_capture.connIq.rxPhy, 0x01);
    EXPECT_EQ(g_capture.connIq.dataChannelIndex, 0x09);
    EXPECT_EQ(g_capture.connIq.rssi, -60); // -6.0 dBm
    EXPECT_EQ(g_capture.connIq.rssiAntennaId, 0x02);
    EXPECT_EQ(g_capture.connIq.cteType, HCI_LE_CTE_TYPE_AOA);
    EXPECT_EQ(g_capture.connIq.slotDurations, 0x02);
    EXPECT_EQ(g_capture.connIq.packetStatus, 0x00);
    EXPECT_EQ(g_capture.connIq.connEventCounter, 0x0042);
    EXPECT_EQ(g_capture.connIq.sampleCount, 0x02);
    EXPECT_EQ(g_capture.connIqSamples[0], 10);
    EXPECT_EQ(g_capture.connIqSamples[1], -10);
    EXPECT_EQ(g_capture.connIqSamples[2], 20);
    EXPECT_EQ(g_capture.connIqSamples[3], -30);
}

// @tc.number: StackGapLe5_1EventParse_CteRequestFailed_00300
// @tc.name:  0x17 LE CTE Request Failed 按线格式解析
HWTEST_F(StackGapLe51EventParseTest, StackGapLe5_1EventParse_CteRequestFailed_00300, TestSize.Level1)
{
    InjectWire(CTE_REQUEST_FAILED_WIRE, sizeof(CTE_REQUEST_FAILED_WIRE));

    EXPECT_TRUE(g_capture.cteRequestFailedReceived);
    EXPECT_EQ(g_capture.cteRequestFailed.status, 0x01);
    EXPECT_EQ(g_capture.cteRequestFailed.connectionHandle, 0x0008);
}

// @tc.number: StackGapLe5_1EventParse_PastSyncTransferReceived_00400
// @tc.name:  0x18 LE Periodic Advertising Sync Transfer Received 全部 19 字节按线格式解析
HWTEST_F(StackGapLe51EventParseTest, StackGapLe5_1EventParse_PastSyncTransferReceived_00400, TestSize.Level1)
{
    InjectWire(PAST_SYNC_TRANSFER_RECEIVED_WIRE, sizeof(PAST_SYNC_TRANSFER_RECEIVED_WIRE));

    EXPECT_TRUE(g_capture.pastReceived);
    EXPECT_EQ(g_capture.past.status, 0x00);
    EXPECT_EQ(g_capture.past.connectionHandle, 0x0008);
    EXPECT_EQ(g_capture.past.serviceData, 0x1234);
    EXPECT_EQ(g_capture.past.syncHandle, 0x0567);
    EXPECT_EQ(g_capture.past.advertisingSid, 0x03);
    EXPECT_EQ(g_capture.past.advertiserAddressType, 0x00);
    EXPECT_EQ(g_capture.past.advertiserAddress.raw[0], 0xFF);
    EXPECT_EQ(g_capture.past.advertiserAddress.raw[1], 0xEE);
    EXPECT_EQ(g_capture.past.advertiserAddress.raw[2], 0xDD);
    EXPECT_EQ(g_capture.past.advertiserAddress.raw[3], 0xCC);
    EXPECT_EQ(g_capture.past.advertiserAddress.raw[4], 0xBB);
    EXPECT_EQ(g_capture.past.advertiserAddress.raw[5], 0xAA);
    EXPECT_EQ(g_capture.past.advertiserPhy, 0x01);
    EXPECT_EQ(g_capture.past.periodicAdvertisingInterval, 0x00A0);
    EXPECT_EQ(g_capture.past.advertiserClockAccuracy, 0x01);
}

// @tc.number: StackGapLe5_1EventParse_TruncatedDropped_00500
// @tc.name:  畸形事件被静默丢弃（回调不触发）
// @tc.desc:  覆盖固定部分截断、声明长度与实际负载不符、Sample_Count 溢出、PAST 截断、
//            未知子事件 0x24（超过 LESUBEVENTCODE_MAX 0x23）、未知事件码 0x5A 六类畸形 wire 字节
HWTEST_F(StackGapLe51EventParseTest, StackGapLe5_1EventParse_TruncatedDropped_00500, TestSize.Level1)
{
    InjectWire(TRUNCATED_CONNLESS_IQ_WIRE, sizeof(TRUNCATED_CONNLESS_IQ_WIRE));
    EXPECT_FALSE(g_capture.connlessIqReceived);

    g_capture.Reset();
    InjectWire(WRONG_DECLARED_LENGTH_WIRE, sizeof(WRONG_DECLARED_LENGTH_WIRE));
    EXPECT_FALSE(g_capture.connlessIqReceived);

    g_capture.Reset();
    InjectWire(SAMPLE_COUNT_OVERFLOW_WIRE, sizeof(SAMPLE_COUNT_OVERFLOW_WIRE));
    EXPECT_FALSE(g_capture.connlessIqReceived);

    g_capture.Reset();
    InjectWire(TRUNCATED_PAST_WIRE, sizeof(TRUNCATED_PAST_WIRE));
    EXPECT_FALSE(g_capture.pastReceived);

    g_capture.Reset();
    InjectWire(UNKNOWN_SUBEVENT_WIRE, sizeof(UNKNOWN_SUBEVENT_WIRE));
    EXPECT_FALSE(g_capture.pastReceived);

    g_capture.Reset();
    InjectWire(UNKNOWN_EVENT_CODE_WIRE, sizeof(UNKNOWN_EVENT_CODE_WIRE));
    EXPECT_FALSE(g_capture.pastReceived);
}

// @tc.number: StackGapLe5_1EventParse_InjectGuard_00600
// @tc.name:  HCI_InjectReceivedEvent 测试缝的参数校验与未初始化守卫
// @tc.desc:  本套件未初始化整个协议栈（无 RX 队列），注入应返回 BT_OPERATION_FAILED；
//            非法参数返回 BT_BAD_PARAM。完整注入路径（活栈 + 栈线程回调）见
//            stack_gap_le_interact_test.cpp 的 StackGapLe5_1_Inject_* 用例。
HWTEST_F(StackGapLe51EventParseTest, StackGapLe5_1EventParse_InjectGuard_00600, TestSize.Level1)
{
    const uint8_t wire[] = {0x3E, 0x00};
    EXPECT_EQ(HCI_InjectReceivedEvent(nullptr, 1), BT_BAD_PARAM);
    EXPECT_EQ(HCI_InjectReceivedEvent(wire, 0), BT_BAD_PARAM);
    EXPECT_EQ(HCI_InjectReceivedEvent(wire, sizeof(wire)), BT_OPERATION_FAILED);
}
} // namespace Bluetooth
} // namespace OHOS
