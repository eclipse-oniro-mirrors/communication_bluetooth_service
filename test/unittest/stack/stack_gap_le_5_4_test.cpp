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

// 蓝牙 5.4 PAwR/ACS 新增内容单测。与 5.1-5.3 一样遵循"真 HDI 回包不可依赖"的
// 策略（见开发计划 §6.1 R6）：命令层用例只验参数门控，不等待控制器回包；事件
// 层走字节级 wire 注入（HCI 层同步解析 + 活栈异步全链路）。
//
// 覆盖范围：
//   1) P1 六类 [v2]/新 LE 事件（0x24-0x29）HCI 层全字段线格式解析，含
//      各事件的空参数规则（0x24 无子事件 → 尾四值全 0；0x25 无子事件 →
//      Subevent=0xFF；0x26 无子事件 → Num_Subevents=0 而其余三值规范未指定、
//      必须原样透传不得假定 0；0x29 失败状态 → No 值 0xFF/0xFFFF 尾）与
//      截断/长度不符/未知子事件 0x2A 负向丢弃。
//   2) v1↔v2 双轨隔离：0x0A/0x0E/0x0F/0x18 只触发 v1 成员，0x24-0x29 只
//      触发 v2 成员；[v1] 路径解析保持不变（0x0F 的 CTE_Type 字节按运行时
//      控制器能力 6/7 字节线头自适应）。
//   3) A3：0x0D Ext Adv Report 的 Primary/Secondary_PHY 原样透传
//      （0x04=LE Coded S=2 的语义解码在 gap 层按本地 bit 41 进行，本仓库
//      HCI 层只透传——见 hci_evt_le.c 的 A3 注释；gap 层解码依赖活扫描
//      状态，无 wire 单测缝，与 C0 文档局限声明一致）。
//   4) P3/P4 活栈 GAP 全链路：0x24-0x26 wire 注入 → gap_hci_receive →
//      GAP 任务 → GapPawrSyncCallback 字段逐一断言（含失败状态以
//      clearly-invalid 约定值上报、0x26 失败仍保留连接句柄/服务数据原值、
//      0x26 无子事件非零尾原样透传）；0x27/0x28 注入 → GapPawrAdvCallback
//      （数据请求窗口、双记录响应报告，记录数据回调内拷贝）。
//   5) 公共 API 门控：P3（GAPIF_LePawrSetSubeventParams/SetSubeventData/
//      RegisterPawrAdvCallback）、P4（GAPIF_LePawrSetSyncSubevent/
//      SetResponseData/RegisterPawrSyncCallback）、P5（GAPIF_LeExtCreateConn-
//      FromPawr/BTM_LeConnectFromPawr/BTM_GetLeConnectionPawrAssociation）、
//      A2（GAPIF_LeExAdvSetParamV2）与六个 5.4 HCI 发送器的 NULL/越界参数
//      校验。能力分支（bit 43/44）按运行控制器分支断言：不支持 → 合法调用
//      BT_NOT_SUPPORT（P8）；支持 → 时序关系负向 BT_BAD_PARAM + 合法下发。
//
// 局限（与开发计划 §6.1/§7 一致）：
//   - 0x29 事件在活栈只注入失败状态（0x3E）：成功状态会进入真实连接记录路径
//     （HciAclOnConnectionComplete/BtmOnLeEnhancedConnectionComplete），
//     没有可回滚的建链状态可借用；“连接对象可查询 PAwR 句柄”的成功路径由
//     控制器真实建链覆盖，不在 wire 单测范围。0x3E 注入断言走普通失败路径
//     且无异常状态残留（P5 验收）。
//   - 建链命令（GAPIF_LeExtCreateConnFromPawr/BTM_LeConnectFromPawr 合法
//     参数 + 0xFF 回落）在支持 PAwR 的控制器上会真实下发，无 hermetic 取消
//     手段：此类用例只跑"能力缺失 → NOT_SUPPORT"分支与全部门控负向
//     （混合 0xFF、越界、NULL 地址均在发送前被拒）。
//   - P3 合法 v2 配置 / numSubevents=0 的 [v1] 委托 / 0x27 触发的数据块
//     flush 会对真实控制器下发命令（R6：命令层不等待回包，返回非参数错误
//     即视为接受）；0x27 请求窗口与预填充窗口相交的用例用于验证
//     "数据只传一次、传后丢弃"（P7）。
//   - P7 核对中无宿主承载的行为（RspAA 无主机命令、EAD 无栈改动、无
//     AUX_SYNC_SUBEVENT_IND 时无数据请求触发源——0x27 是唯一请求源）已在
//     代码实现时确认，此处不再设用例；C0 文档 §6 的 4 条回归建议已并入
//     套件二（Ext Adv Report 0x04 分支透传、Set CIG 错误码透传属 iso 域，
//     事件掩码位 35-40 门控属 btm_controller 位表，见 C0 清单）。

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstring>
#include <mutex>

#include <gtest/gtest.h>
#include "securec.h"

#include "btm.h"
#include "btstack.h"
#include "buffer.h"
#include "gap_le_if.h"
#include "gap_le_if_5_4.h"
#include "src/hci/evt/hci_evt.h"
#include "src/hci/hci.h"
#include "src/hci/hci_def_le_evt.h"

using namespace testing::ext;

namespace OHOS {
namespace Bluetooth {
namespace {
constexpr int WAIT_CALLBACK_TIMEOUT_MS = 5000;
// 负向探针窗口：断言某事件"不应被送达"时的等待时长，比正向超时短一个量级，
// 避免丢弃类用例每次都拖满 WAIT_CALLBACK_TIMEOUT_MS。
constexpr int DROP_PROBE_TIMEOUT_MS = 500;
constexpr uint8_t HCI_STATUS_SUCCESS = 0x00;
constexpr uint16_t TEST_SYNC_HANDLE = 0x0ABC;
constexpr uint16_t TEST_CONN_HANDLE = 0x0E42;
constexpr uint16_t TEST_SERVICE_DATA = 0xBEEF;

// ---------------- wire 字节（完整 HCI 事件包：Event_Code + Parameter_Total_Length + parameters） ----------------

// Subevent 0x24 LE Periodic Advertising Sync Established [v2]（7.7.65,14）。
// 子事件码后 19 字节 = [v1] 15 字节 + 尾四值（Num_Subevents(1) +
// Subevent_Interval(1) + Response_Slot_Delay(1) + Response_Slot_Spacing(1)）：
//   Status=0x00 | Sync_Handle=0x0ABC | Advertising_SID=0x05 |
//   Advertiser_Address_Type=0x00(public) | Address=11:22:33:44:55:66 |
//   Advertiser_PHY=0x03(LE Coded) | Interval=0x01F4(500) | Clock_Accuracy=0x02 |
//   Num_Subevents=0x04 | Subevent_Interval=0x0A | Response_Slot_Delay=0x02 |
//   Response_Slot_Spacing=0x08
constexpr uint8_t PERIODIC_SYNC_ESTABLISHED_V2_WIRE[] = {
    0x3E,
    0x14, // LE Meta, Parameter_Total_Length = 20（子事件码 1 + 事件参数 19）
    0x24,
    0x00, // Status
    0xBC,
    0x0A, // Sync_Handle = 0x0ABC
    0x05, // Advertising_SID
    0x00, // Advertiser_Address_Type = Public Device Address
    0x11,
    0x22,
    0x33,
    0x44,
    0x55,
    0x66, // Advertiser_Address
    0x03, // Advertiser_PHY = LE Coded
    0xF4,
    0x01, // Periodic_Advertising_Interval = 500
    0x02, // Advertiser_Clock_Accuracy
    0x04, // Num_Subevents
    0x0A, // Subevent_Interval
    0x02, // Response_Slot_Delay
    0x08, // Response_Slot_Spacing
};

// 同事件、无子事件的纯周期广播训练：规范约定尾四值全部为 0x00。
constexpr uint8_t PERIODIC_SYNC_ESTABLISHED_V2_PLAIN_WIRE[] = {
    0x3E,
    0x14,
    0x24,
    0x00, // Status
    0xBC,
    0x0A, // Sync_Handle = 0x0ABC
    0x05, // Advertising_SID
    0x00, // Advertiser_Address_Type
    0x11,
    0x22,
    0x33,
    0x44,
    0x55,
    0x66,
    0x03, // Advertiser_PHY
    0xF4,
    0x01, // Periodic_Advertising_Interval = 500
    0x02, // Advertiser_Clock_Accuracy
    0x00, // Num_Subevents = 0（无子事件）
    0x00, // Subevent_Interval = 0
    0x00, // Response_Slot_Delay = 0
    0x00, // Response_Slot_Spacing = 0
};

// 畸形：子事件码后只有 18 字节（规范要求 19）→ 子事件解析器长度门控丢弃。
constexpr uint8_t PERIODIC_SYNC_ESTABLISHED_V2_TRUNCATED_WIRE[] = {
    0x3E,
    0x13, // 声明长度 = 19（子事件码 1 + 事件参数 18）
    0x24,
    0x00,
    0xBC,
    0x0A,
    0x05,
    0x00,
    0x11,
    0x22,
    0x33,
    0x44,
    0x55,
    0x66,
    0x03,
    0xF4,
    0x01,
    0x02,
    0x04,
    0x0A,
    0x02, // 缺 Response_Slot_Spacing
};

// 失败状态（0x11）事件：HCI 解析层对身份/尾值只透传不改写（"clearly-invalid
// 约定值"是 GAP 消费层的上报约定，见套件二 0x24 失败用例）。身份字段取与
// 成功线不同的值以证明确实来自 wire。
constexpr uint8_t PERIODIC_SYNC_ESTABLISHED_V2_FAILURE_WIRE[] = {
    0x3E,
    0x14,
    0x24,
    0x11, // Status = Unsupported Feature or Parameter Value
    0x34,
    0x12, // Sync_Handle = 0x1234（失败时无效，但 HCI 层透传）
    0x0F, // Advertising_SID
    0x02, // Advertiser_Address_Type（非法值也不被 HCI 层过滤）
    0x0A,
    0x0B,
    0x0C,
    0x0D,
    0x0E,
    0x0F, // Advertiser_Address
    0x01, // Advertiser_PHY
    0xC8,
    0x00, // Periodic_Advertising_Interval = 200
    0x07, // Advertiser_Clock_Accuracy
    0x05, // Num_Subevents
    0x1E, // Subevent_Interval
    0x09, // Response_Slot_Delay
    0x40, // Response_Slot_Spacing
};

// Subevent 0x25 LE Periodic Advertising Report [v2]（7.7.65,15）。固定 10 字节
// 线头 = Sync_Handle(2) + TX_Power(1) + RSSI(1) + CTE_Type(1) +
// Periodic_Event_Counter(2) + Subevent(1) + Data_Status(1) + Data_Length(1)，
// 后随 Data。注意 [v2] 中 Periodic_Event_Counter 与 Subevent 插在 CTE_Type
// 与 Data_Status 之间（与 [v1] 的偏移不同）：
//   Sync_Handle=0x0042 | TX_Power=0x05 | RSSI=0xF1(-15) | CTE_Type=0xFF |
//   Periodic_Event_Counter=0x1234 | Subevent=0x03 | Data_Status=0x00 |
//   Data_Length=0x05 | Data="pawr!"
constexpr uint8_t PERIODIC_ADV_REPORT_V2_WIRE[] = {
    0x3E,
    0x10, // 声明长度 = 16（子事件码 1 + 事件参数 15）
    0x25,
    0x42,
    0x00, // Sync_Handle = 0x0042
    0x05, // TX_Power = +5 dBm
    0xF1, // RSSI = -15 dBm
    0xFF, // CTE_Type = no CTE
    0x34,
    0x12, // Periodic_Event_Counter = 0x1234
    0x03, // Subevent = 3
    0x00, // Data_Status = complete
    0x05, // Data_Length
    0x70,
    0x61,
    0x77,
    0x72,
    0x21, // "pawr!"
};

// 无子事件的纯训练：Subevent=0xFF（[v2] 约定），空 Data。
constexpr uint8_t PERIODIC_ADV_REPORT_V2_PLAIN_WIRE[] = {
    0x3E,
    0x0B, // 声明长度 = 11（子事件码 1 + 固定头 10）
    0x25,
    0x42,
    0x00, // Sync_Handle = 0x0042
    0x7F, // TX_Power = not available
    0x7F, // RSSI = not available
    0xFF, // CTE_Type = no CTE
    0x07,
    0x00, // Periodic_Event_Counter = 7
    0xFF, // Subevent = 0xFF（无子事件）
    0x00, // Data_Status
    0x00, // Data_Length = 0（无 Data）
};

// 畸形：固定头只有 9 字节（规范要求 10）→ 丢弃。
constexpr uint8_t PERIODIC_ADV_REPORT_V2_TRUNCATED_WIRE[] = {
    0x3E,
    0x0A, // 声明长度 = 10
    0x25,
    0x42,
    0x00,
    0x05,
    0xF1,
    0xFF,
    0x34,
    0x12,
    0x03,
    0x00, // 缺 Data_Length
};

// 畸形：Data_Length=0x05 但实际只剩 4 字节 → 丢弃。
constexpr uint8_t PERIODIC_ADV_REPORT_V2_BAD_DATA_LEN_WIRE[] = {
    0x3E,
    0x0F, // 声明长度 = 15（子事件码 1 + 固定头 10 + 实际 Data 4）
    0x25,
    0x42,
    0x00, // Sync_Handle
    0x05, // TX_Power
    0xF1, // RSSI
    0xFF, // CTE_Type
    0x34,
    0x12, // Periodic_Event_Counter
    0x03, // Subevent
    0x00, // Data_Status
    0x05, // Data_Length（声称 5，实际只有 4）
    0x11,
    0x22,
    0x33,
    0x44,
};

// Subevent 0x26 LE Periodic Advertising Sync Transfer Received [v2]
// （7.7.65,24）。子事件码后 23 字节 = [v1](0x18) 19 字节 + 同 0x24 的尾四值：
//   Status=0x00 | Connection_Handle=0x0E42 | Service_Data=0xBEEF |
//   Sync_Handle=0x0ABC | Advertising_SID=0x05 | Address_Type=0x01(random) |
//   Address=66:55:44:33:22:11 | PHY=0x02(LE 2M) | Interval=0x0064(100) |
//   Clock_Accuracy=0x04 | Num_Subevents=0x03 | Subevent_Interval=0x14 |
//   Response_Slot_Delay=0x05 | Response_Slot_Spacing=0x10
constexpr uint8_t PERIODIC_SYNC_TRANSFER_RECEIVED_V2_WIRE[] = {
    0x3E,
    0x18, // 声明长度 = 24（子事件码 1 + 事件参数 23）
    0x26,
    0x00, // Status
    0x42,
    0x0E, // Connection_Handle = 0x0E42
    0xEF,
    0xBE, // Service_Data = 0xBEEF
    0xBC,
    0x0A, // Sync_Handle = 0x0ABC
    0x05, // Advertising_SID
    0x01, // Advertiser_Address_Type = Random Device Address
    0x66,
    0x55,
    0x44,
    0x33,
    0x22,
    0x11, // Advertiser_Address
    0x02, // Advertiser_PHY = LE 2M
    0x64,
    0x00, // Periodic_Advertising_Interval = 100
    0x04, // Advertiser_Clock_Accuracy
    0x03, // Num_Subevents
    0x14, // Subevent_Interval
    0x05, // Response_Slot_Delay
    0x10, // Response_Slot_Spacing
};

// 无子事件的纯训练（经 PAST 建立）：Num_Subevents=0 时其余三个尾值规范未
// 指定——解析与上报都必须原样透传，不得假定 0x00。这里放三个非零值以证明
// 没有"清零改写"。
constexpr uint8_t PERIODIC_SYNC_TRANSFER_RECEIVED_V2_NOSUBEVENT_WIRE[] = {
    0x3E,
    0x18,
    0x26,
    0x00, // Status
    0x42,
    0x0E, // Connection_Handle = 0x0E42
    0xEF,
    0xBE, // Service_Data = 0xBEEF
    0xBC,
    0x0A, // Sync_Handle = 0x0ABC
    0x05, // Advertising_SID
    0x00, // Advertiser_Address_Type = Public
    0x66,
    0x55,
    0x44,
    0x33,
    0x22,
    0x11, // Advertiser_Address
    0x02, // Advertiser_PHY
    0x64,
    0x00, // Periodic_Advertising_Interval = 100
    0x04, // Advertiser_Clock_Accuracy
    0x00, // Num_Subevents = 0（无子事件）
    0x77, // Subevent_Interval：规范未指定，透传
    0x88, // Response_Slot_Delay：规范未指定，透传
    0x99, // Response_Slot_Spacing：规范未指定，透传
};

// 失败状态（0x3E）事件：连接句柄与服务数据仍然有效并透传；其余身份字段
// 在 GAP 消费层按 clearly-invalid 约定值上报（见套件二），HCI 层只透传。
constexpr uint8_t PERIODIC_SYNC_TRANSFER_RECEIVED_V2_FAILURE_WIRE[] = {
    0x3E,
    0x18,
    0x26,
    0x3E, // Status = Connection Failed to be Established
    0x42,
    0x0E, // Connection_Handle = 0x0E42（保留原值）
    0xEF,
    0xBE, // Service_Data = 0xBEEF（保留原值）
    0x21,
    0x43, // Sync_Handle = 0x4321（失败时无效，透传）
    0x0A, // Advertising_SID
    0x00, // Advertiser_Address_Type
    0xAA,
    0xBB,
    0xCC,
    0xDD,
    0xEE,
    0xFF, // Advertiser_Address
    0x01, // Advertiser_PHY
    0x0F,
    0x00, // Periodic_Advertising_Interval = 15
    0x01, // Advertiser_Clock_Accuracy
    0x00, // Num_Subevents
    0x00,
    0x00,
    0x00, // 尾三值
};

// 畸形：子事件码后只有 22 字节（规范要求 23）→ 丢弃。
constexpr uint8_t PERIODIC_SYNC_TRANSFER_RECEIVED_V2_TRUNCATED_WIRE[] = {
    0x3E,
    0x17, // 声明长度 = 23（子事件码 1 + 事件参数 22）
    0x26,
    0x00,
    0x42,
    0x0E,
    0xEF,
    0xBE,
    0xBC,
    0x0A,
    0x05,
    0x01,
    0x66,
    0x55,
    0x44,
    0x33,
    0x22,
    0x11,
    0x02,
    0x64,
    0x00,
    0x04,
    0x03,
    0x14,
    0x05, // 缺 Response_Slot_Spacing
};

// Subevent 0x27 LE Periodic Advertising Subevent Data Request（7.7.65,36）：
//   Advertising_Handle=0x0A | Subevent_Start=0x02 | Subevent_Data_Count=0x06
constexpr uint8_t SUBEVENT_DATA_REQUEST_WIRE[] = {
    0x3E,
    0x04, // 声明长度 = 4（子事件码 1 + 事件参数 3）
    0x27,
    0x0A, // Advertising_Handle
    0x02, // Subevent_Start
    0x06, // Subevent_Data_Count
};

// 畸形：子事件码后只有 2 字节（规范要求 3）→ 丢弃。
constexpr uint8_t SUBEVENT_DATA_REQUEST_TRUNCATED_WIRE[] = {
    0x3E,
    0x03,
    0x27,
    0x0A,
    0x02, // 缺 Subevent_Data_Count
};

// Subevent 0x28 LE Periodic Advertising Response Report（7.7.65,37）。固定
// 前缀 4 字节 + 交错记录（每条记录 6 字节固定 + Data），解析器解交错到
// response[i]：
//   Advertising_Handle=0x0A | Subevent=0x04 | Tx_Status=0x00 |
//   Num_Responses=0x02
//   记录 0：Tx_Power=0x0F | RSSI=0xD8(-40) | CTE_Type=0xFF | Response_Slot=0x03
//          | Data_Status=0x00(complete) | Data_Length=0x03 | Data={01,02,03}
//   记录 1：Tx_Power=0x7F | RSSI=0x7F | CTE_Type=0x00(AoA) |
//          Response_Slot=0x1A | Data_Status=0x01(incomplete) | 空 Data
constexpr uint8_t RESPONSE_REPORT_WIRE[] = {
    0x3E,
    0x14, // 声明长度 = 20（子事件码 1 + 事件参数 19）
    0x28,
    0x0A, // Advertising_Handle
    0x04, // Subevent
    0x00, // Tx_Status = AUX_SYNC_SUBEVENT_IND transmitted
    0x02, // Num_Responses
    0x0F, // 记录 0 Tx_Power = +15 dBm
    0xD8, // 记录 0 RSSI = -40 dBm
    0xFF, // 记录 0 CTE_Type = no CTE
    0x03, // 记录 0 Response_Slot
    0x00, // 记录 0 Data_Status = complete
    0x03, // 记录 0 Data_Length
    0x01,
    0x02,
    0x03, // 记录 0 Data
    0x7F, // 记录 1 Tx_Power = not available
    0x7F, // 记录 1 RSSI = not available
    0x00, // 记录 1 CTE_Type = AoA
    0x1A, // 记录 1 Response_Slot
    0x01, // 记录 1 Data_Status = incomplete
    0x00, // 记录 1 Data_Length = 0
};

// Num_Responses=0x00：合法，无记录。
constexpr uint8_t RESPONSE_REPORT_EMPTY_WIRE[] = {
    0x3E,
    0x05,
    0x28,
    0x0A, // Advertising_Handle
    0x04, // Subevent
    0x00, // Tx_Status
    0x00, // Num_Responses = 0
};

// Num_Responses=0x1A 超过上限 0x19 → 丢弃。
constexpr uint8_t RESPONSE_REPORT_NUM_TOO_HIGH_WIRE[] = {
    0x3E,
    0x05,
    0x28,
    0x0A,
    0x04,
    0x00,
    0x1A, // Num_Responses = 26（上限 25）
};

// 畸形：记录 Data_Length=0x04 但实际只剩 2 字节 → 丢弃。
constexpr uint8_t RESPONSE_REPORT_BAD_DATA_LEN_WIRE[] = {
    0x3E,
    0x0D, // 声明长度 = 13（子事件码 1 + 前缀 4 + 记录 6 + 实际 Data 2）
    0x28,
    0x0A,
    0x04,
    0x00,
    0x01, // Num_Responses = 1
    0x0F, // Tx_Power
    0xD8, // RSSI
    0xFF, // CTE_Type
    0x03, // Response_Slot
    0x00, // Data_Status
    0x04, // Data_Length（声称 4，实际只有 2）
    0x01,
    0x02,
};

// 畸形：固定前缀只有 3 字节（规范要求 4）→ 丢弃。
constexpr uint8_t RESPONSE_REPORT_TRUNCATED_PREFIX_WIRE[] = {
    0x3E,
    0x04,
    0x28,
    0x0A,
    0x04,
    0x00, // 缺 Num_Responses
};

// Subevent 0x29 LE Enhanced Connection Complete [v2]（7.7.65,10）。子事件码
// 后 33 字节 = [v1](0x0A) 30 字节 + Advertising_Handle(1) + Sync_Handle(2)。
// 此线为失败状态（0x3E，Connection Failed to be Established——从 PAwR 建链
// 失败时的典型回包）：身份尾为 No 值（0xFF / 0xFFFF，规范要求该情况下
// Controller 如此上报且 Host 忽略）。
constexpr uint8_t ENHANCED_CONNECTION_COMPLETE_V2_WIRE[] = {
    0x3E,
    0x22, // 声明长度 = 34（子事件码 1 + 事件参数 33）
    0x29,
    0x3E, // Status = Connection Failed to be Established
    0x42,
    0x00, // Connection_Handle = 0x0042
    0x00, // Role = Central
    0x00, // Peer_Address_Type = Public
    0xA1,
    0xB2,
    0xC3,
    0xD4,
    0xE5,
    0xF6, // Peer_Address
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00, // Local_Resolvable_Private_Address
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00, // Peer_Resolvable_Private_Address
    0x0C,
    0x00, // Connection_Interval = 12
    0x00,
    0x00, // Connection_Latency = 0
    0x80,
    0x0C, // Supervision_Timeout = 3200 (32 s)
    0x00, // Master_Clock_Accuracy
    0xFF, // Advertising_Handle = 0xFF（No Advertising_Handle）
    0xFF,
    0xFF, // Sync_Handle = 0xFFFF（No Sync_Handle）
};

// 畸形：事件参数只有 31 字节（规范要求 33，缺 Sync_Handle 2 字节）→ 丢弃。
// 声明长度 0x20（32 = 子事件码 1 + 事件参数 31）与物理长度一致，使该线能
// 穿过 hci_evt.c 的外层等值门，抵达 0x29 解析器的 33 字节结构门控并被其
// 丢弃（30 字节 [v1] 尺寸的解析器会误收此线——这是该结构门控的回归探针）。
constexpr uint8_t ENHANCED_CONNECTION_COMPLETE_V2_TRUNCATED_WIRE[] = {
    0x3E,
    0x20, // 声明长度 = 32（子事件码 1 + 事件参数 31）
    0x29,
    0x3E,
    0x42,
    0x00,
    0x00,
    0x00,
    0xA1,
    0xB2,
    0xC3,
    0xD4,
    0xE5,
    0xF6,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x00,
    0x00,
    0x00,
    0x80,
    0x0C,
    0x00,
    0xFF, // Advertising_Handle = 0xFF（No Advertising_Handle）——Sync_Handle 2 字节整体缺失
};

// v1 对照线（回归对照：旧码路径无改动）。0x0A 失败状态 30 字节。
constexpr uint8_t V1_ENHANCED_CONNECTION_COMPLETE_WIRE[] = {
    0x3E,
    0x1F, // 声明长度 = 31（子事件码 1 + 事件参数 30）
    0x0A,
    0x3E, // Status（失败，避免进入连接记录路径）
    0x42,
    0x00, // Connection_Handle = 0x0042
    0x00, // Role
    0x00, // Peer_Address_Type
    0xA1,
    0xB2,
    0xC3,
    0xD4,
    0xE5,
    0xF6, // Peer_Address
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00, // Local_Resolvable_Private_Address
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00, // Peer_Resolvable_Private_Address
    0x0C,
    0x00, // Connection_Interval
    0x00,
    0x00, // Connection_Latency
    0x80,
    0x0C, // Supervision_Timeout
    0x00, // Master_Clock_Accuracy
};

// v1 0x0E 成功状态 15 字节（同 0x24 的前 15 字节）。
constexpr uint8_t V1_PERIODIC_SYNC_ESTABLISHED_WIRE[] = {
    0x3E,
    0x10, // 声明长度 = 16（子事件码 1 + 事件参数 15）
    0x0E,
    0x00, // Status
    0xBC,
    0x0A, // Sync_Handle = 0x0ABC
    0x05, // Advertising_SID
    0x00, // Advertiser_Address_Type
    0x11,
    0x22,
    0x33,
    0x44,
    0x55,
    0x66, // Advertiser_Address
    0x03, // Advertiser_PHY
    0xF4,
    0x01, // Periodic_Advertising_Interval = 500
    0x02, // Advertiser_Clock_Accuracy
};

// v1 0x18 成功状态 19 字节（同 0x26 的前 19 字节）。
constexpr uint8_t V1_PERIODIC_SYNC_TRANSFER_RECEIVED_WIRE[] = {
    0x3E,
    0x14, // 声明长度 = 20（子事件码 1 + 事件参数 19）
    0x18,
    0x00, // Status
    0x42,
    0x0E, // Connection_Handle = 0x0E42
    0xEF,
    0xBE, // Service_Data = 0xBEEF
    0xBC,
    0x0A, // Sync_Handle = 0x0ABC
    0x05, // Advertising_SID
    0x01, // Advertiser_Address_Type = Random
    0x66,
    0x55,
    0x44,
    0x33,
    0x22,
    0x11, // Advertiser_Address
    0x02, // Advertiser_PHY
    0x64,
    0x00, // Periodic_Advertising_Interval = 100
    0x04, // Advertiser_Clock_Accuracy
};

// 未知子事件 0x2A 超出 LESUBEVENTCODE_MAX（0x29）→ LE Meta 分发丢弃。
constexpr uint8_t UNKNOWN_SUBEVENT_0X2A_WIRE[] = {
    0x3E,
    0x02,
    0x2A,
    0x00,
};

// A3：Subevent 0x0D LE Extended Advertising Report（7.7.65,13），PHY 字段
// 原样透传（0x04 = LE Coded S=2 / 0x03 = LE Coded S=8 的语义解码按本地 LL
// bit 41 在 gap 层进行，HCI 层不改写）。一个事件带两份报告以证明 24 字节
// 报告步长解析正确：
//   Num_Reports=0x02
//   报告 0：Event_Type=0x0000 | Address_Type=0x00 | Address=11:22:33:44:55:66
//          | Primary_PHY=0x04(LE Coded S=2) | Secondary_PHY=0x03(LE Coded
//          S=8) | SID=0x00 | TX_Power=0x7F | RSSI=0xC8(-56) | Interval=0 |
//          Direct_Type=0x00 | Direct_Address=0 | Data_Length=0
//   报告 1：Event_Type=0x0004 | Address_Type=0x01 | Address=2A:2B:2C:2D:2E:2F
//          | Primary_PHY=0x01(LE 1M) | Secondary_PHY=0x00 | SID=0x0F |
//          TX_Power=0x7F | RSSI=0x7F | Interval=0 | 其余 0 | Data_Length=0
constexpr uint8_t EXT_ADV_REPORT_A3_WIRE[] = {
    0x3E,
    0x32, // 声明长度 = 50（子事件码 1 + Num_Reports 1 + 报告 2×24）
    0x0D,
    0x02, // Num_Reports
    // 报告 0
    0x00,
    0x00, // Event_Type
    0x00, // Address_Type
    0x11,
    0x22,
    0x33,
    0x44,
    0x55,
    0x66, // Address
    0x04, // Primary_PHY = 0x04（raw 透传）
    0x03, // Secondary_PHY = 0x03（raw 透传）
    0x00, // Advertising_SID
    0x7F, // TX_Power = not available
    0xC8, // RSSI = -56 dBm
    0x00,
    0x00, // Periodic_Advertising_Interval = 0
    0x00, // Direct_Address_Type
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00, // Direct_Address
    0x00, // Data_Length
    // 报告 1
    0x04,
    0x00, // Event_Type
    0x01, // Address_Type
    0x2A,
    0x2B,
    0x2C,
    0x2D,
    0x2E,
    0x2F, // Address
    0x01, // Primary_PHY = 0x01（LE 1M）
    0x00, // Secondary_PHY = 0x00
    0x0F, // Advertising_SID
    0x7F, // TX_Power
    0x7F, // RSSI
    0x00,
    0x00, // Periodic_Advertising_Interval
    0x00, // Direct_Address_Type
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00, // Direct_Address
    0x00, // Data_Length
};
} // namespace

// =====================================================================
// 套件一：HCI 层同步解析（无栈 fixture，仿 stack_gap_le_5_3_test.cpp）
// HCI 回调表是函数指针结构、不带 context 参数，故用文件级全局捕获对象
// （本套件串行执行，无并发）。HciOnEvent 为同步分发，回调在 InjectWire
// 返回前必然已执行完毕，无需条件变量等待。带 Data 指针的事件（0x25/0x28/
// 0x0D）必须在回调内拷贝数据（指针仅在回调期间有效）。
// =====================================================================

namespace {

struct SyncEstablishedV2Capture {
    bool received = false;
    uint8_t status = 0;
    uint16_t syncHandle = 0;
    uint8_t advertisingSid = 0;
    uint8_t advertiserAddressType = 0;
    uint8_t advertiserAddress[6] = { 0 };
    uint8_t advertiserPhy = 0;
    uint16_t periodicAdvertisingInterval = 0;
    uint8_t advertiserClockAccuracy = 0;
    uint8_t numSubevents = 0;
    uint8_t subeventInterval = 0;
    uint8_t responseSlotDelay = 0;
    uint8_t responseSlotSpacing = 0;
    void Reset()
    {
        *this = SyncEstablishedV2Capture { };
    }
};

struct ReportV2Capture {
    bool received = false;
    uint16_t syncHandle = 0;
    int8_t txPower = 0;
    int8_t rssi = 0;
    uint8_t cteType = 0;
    uint16_t periodicEventCounter = 0;
    uint8_t subevent = 0;
    uint8_t dataStatus = 0;
    uint8_t dataLength = 0;
    uint8_t data[32] = { 0 };
    void Reset()
    {
        *this = ReportV2Capture { };
    }
};

struct SyncTransferReceivedV2Capture {
    bool received = false;
    uint8_t status = 0;
    uint16_t connectionHandle = 0;
    uint16_t serviceData = 0;
    uint16_t syncHandle = 0;
    uint8_t advertisingSid = 0;
    uint8_t advertiserAddressType = 0;
    uint8_t advertiserAddress[6] = { 0 };
    uint8_t advertiserPhy = 0;
    uint16_t periodicAdvertisingInterval = 0;
    uint8_t advertiserClockAccuracy = 0;
    uint8_t numSubevents = 0;
    uint8_t subeventInterval = 0;
    uint8_t responseSlotDelay = 0;
    uint8_t responseSlotSpacing = 0;
    void Reset()
    {
        *this = SyncTransferReceivedV2Capture { };
    }
};

struct SubeventDataRequestCapture {
    bool received = false;
    uint8_t advertisingHandle = 0;
    uint8_t subeventStart = 0;
    uint8_t subeventDataCount = 0;
    void Reset()
    {
        *this = SubeventDataRequestCapture { };
    }
};

struct ResponseReportRecordCapture {
    int8_t txPower = 0;
    int8_t rssi = 0;
    uint8_t cteType = 0;
    uint8_t responseSlot = 0;
    uint8_t dataStatus = 0;
    uint8_t dataLength = 0;
    uint8_t data[32] = { 0 };
};

struct ResponseReportCapture {
    bool received = false;
    uint8_t advertisingHandle = 0;
    uint8_t subevent = 0;
    uint8_t txStatus = 0;
    uint8_t numResponses = 0;
    ResponseReportRecordCapture response[2];
    void Reset()
    {
        *this = ResponseReportCapture { };
    }
};

struct EnhancedConnectionCompleteV2Capture {
    bool received = false;
    uint8_t status = 0;
    uint16_t connectionHandle = 0;
    uint8_t role = 0;
    uint8_t peerAddress[6] = { 0 };
    uint8_t advertisingHandle = 0;
    uint16_t syncHandle = 0;
    void Reset()
    {
        *this = EnhancedConnectionCompleteV2Capture { };
    }
};

// [v1] 捕获（隔离对照）。
struct V1SyncEstablishedCapture {
    bool received = false;
    uint16_t syncHandle = 0;
    void Reset()
    {
        *this = V1SyncEstablishedCapture { };
    }
};

struct V1ReportCapture {
    bool received = false;
    uint16_t syncHandle = 0;
    uint8_t cteType = 0;
    uint8_t dataStatus = 0;
    uint8_t dataLength = 0;
    uint8_t data[32] = { 0 };
    void Reset()
    {
        *this = V1ReportCapture { };
    }
};

struct V1SyncTransferReceivedCapture {
    bool received = false;
    uint16_t syncHandle = 0;
    void Reset()
    {
        *this = V1SyncTransferReceivedCapture { };
    }
};

struct V1EnhancedConnectionCompleteCapture {
    bool received = false;
    uint16_t connectionHandle = 0;
    void Reset()
    {
        *this = V1EnhancedConnectionCompleteCapture { };
    }
};

struct ExtAdvReportRecordCapture {
    uint16_t eventType = 0;
    uint8_t addressType = 0;
    uint8_t address[6] = { 0 };
    uint8_t primaryPHY = 0;
    uint8_t secondaryPHY = 0;
    uint8_t advertisingSID = 0;
    int8_t txPower = 0;
    int8_t rssi = 0;
};

struct ExtAdvReportCapture {
    bool received = false;
    uint8_t numReports = 0;
    ExtAdvReportRecordCapture report[2];
    void Reset()
    {
        *this = ExtAdvReportCapture { };
    }
};

SyncEstablishedV2Capture g_syncEstablishedV2Capture;
ReportV2Capture g_reportV2Capture;
SyncTransferReceivedV2Capture g_syncTransferReceivedV2Capture;
SubeventDataRequestCapture g_subeventDataRequestCapture;
ResponseReportCapture g_responseReportCapture;
EnhancedConnectionCompleteV2Capture g_enhancedConnectionCompleteV2Capture;
V1SyncEstablishedCapture g_v1SyncEstablishedCapture;
V1ReportCapture g_v1ReportCapture;
V1SyncTransferReceivedCapture g_v1SyncTransferReceivedCapture;
V1EnhancedConnectionCompleteCapture g_v1EnhancedConnectionCompleteCapture;
ExtAdvReportCapture g_extAdvReportCapture;

static void OnLeSyncEstablishedV2(const HciLePeriodicAdvertisingSyncEstablishedV2EventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_syncEstablishedV2Capture.received = true;
    g_syncEstablishedV2Capture.status = eventParam->status;
    g_syncEstablishedV2Capture.syncHandle = eventParam->syncHandle;
    g_syncEstablishedV2Capture.advertisingSid = eventParam->advertisingSid;
    g_syncEstablishedV2Capture.advertiserAddressType = eventParam->advertiserAddressType;
    (void)memcpy_s(g_syncEstablishedV2Capture.advertiserAddress, sizeof(g_syncEstablishedV2Capture.advertiserAddress),
        eventParam->advertiserAddress.raw, sizeof(eventParam->advertiserAddress.raw));
    g_syncEstablishedV2Capture.advertiserPhy = eventParam->advertiserPhy;
    g_syncEstablishedV2Capture.periodicAdvertisingInterval = eventParam->periodicAdvertisingInterval;
    g_syncEstablishedV2Capture.advertiserClockAccuracy = eventParam->advertiserClockAccuracy;
    g_syncEstablishedV2Capture.numSubevents = eventParam->numSubevents;
    g_syncEstablishedV2Capture.subeventInterval = eventParam->subeventInterval;
    g_syncEstablishedV2Capture.responseSlotDelay = eventParam->responseSlotDelay;
    g_syncEstablishedV2Capture.responseSlotSpacing = eventParam->responseSlotSpacing;
}

static void OnLeReportV2(const HciLePeriodicAdvertisingReportV2EventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_reportV2Capture.received = true;
    g_reportV2Capture.syncHandle = eventParam->syncHandle;
    g_reportV2Capture.txPower = eventParam->txPower;
    g_reportV2Capture.rssi = eventParam->rssi;
    g_reportV2Capture.cteType = eventParam->cteType;
    g_reportV2Capture.periodicEventCounter = eventParam->periodicEventCounter;
    g_reportV2Capture.subevent = eventParam->subevent;
    g_reportV2Capture.dataStatus = eventParam->dataStatus;
    g_reportV2Capture.dataLength = eventParam->dataLength;
    if (eventParam->dataLength > 0 && eventParam->data != nullptr &&
        eventParam->dataLength <= sizeof(g_reportV2Capture.data)) {
        (void)memcpy_s(g_reportV2Capture.data, sizeof(g_reportV2Capture.data), eventParam->data,
            eventParam->dataLength);
    }
}

static void OnLeSyncTransferReceivedV2(
    const HciLePeriodicAdvertisingSyncTransferReceivedV2EventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_syncTransferReceivedV2Capture.received = true;
    g_syncTransferReceivedV2Capture.status = eventParam->status;
    g_syncTransferReceivedV2Capture.connectionHandle = eventParam->connectionHandle;
    g_syncTransferReceivedV2Capture.serviceData = eventParam->serviceData;
    g_syncTransferReceivedV2Capture.syncHandle = eventParam->syncHandle;
    g_syncTransferReceivedV2Capture.advertisingSid = eventParam->advertisingSid;
    g_syncTransferReceivedV2Capture.advertiserAddressType = eventParam->advertiserAddressType;
    (void)memcpy_s(g_syncTransferReceivedV2Capture.advertiserAddress,
        sizeof(g_syncTransferReceivedV2Capture.advertiserAddress),
        eventParam->advertiserAddress.raw, sizeof(eventParam->advertiserAddress.raw));
    g_syncTransferReceivedV2Capture.advertiserPhy = eventParam->advertiserPhy;
    g_syncTransferReceivedV2Capture.periodicAdvertisingInterval = eventParam->periodicAdvertisingInterval;
    g_syncTransferReceivedV2Capture.advertiserClockAccuracy = eventParam->advertiserClockAccuracy;
    g_syncTransferReceivedV2Capture.numSubevents = eventParam->numSubevents;
    g_syncTransferReceivedV2Capture.subeventInterval = eventParam->subeventInterval;
    g_syncTransferReceivedV2Capture.responseSlotDelay = eventParam->responseSlotDelay;
    g_syncTransferReceivedV2Capture.responseSlotSpacing = eventParam->responseSlotSpacing;
}

static void OnLeSubeventDataRequest(const HciLePeriodicAdvertisingSubeventDataRequestEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_subeventDataRequestCapture.received = true;
    g_subeventDataRequestCapture.advertisingHandle = eventParam->advertisingHandle;
    g_subeventDataRequestCapture.subeventStart = eventParam->subeventStart;
    g_subeventDataRequestCapture.subeventDataCount = eventParam->subeventDataCount;
}

static void OnLeResponseReport(const HciLePeriodicAdvertisingResponseReportEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_responseReportCapture.received = true;
    g_responseReportCapture.advertisingHandle = eventParam->advertisingHandle;
    g_responseReportCapture.subevent = eventParam->subevent;
    g_responseReportCapture.txStatus = eventParam->txStatus;
    g_responseReportCapture.numResponses = eventParam->numResponses;
    uint8_t copied = (eventParam->numResponses < 2) ? eventParam->numResponses : 2;
    for (uint8_t i = 0; i < copied; i++) {
        const HciLePeriodicAdvertisingResponseReportRecord *record = &eventParam->response[i];
        g_responseReportCapture.response[i].txPower = record->txPower;
        g_responseReportCapture.response[i].rssi = record->rssi;
        g_responseReportCapture.response[i].cteType = record->cteType;
        g_responseReportCapture.response[i].responseSlot = record->responseSlot;
        g_responseReportCapture.response[i].dataStatus = record->dataStatus;
        g_responseReportCapture.response[i].dataLength = record->dataLength;
        if (record->dataLength > 0 && record->data != nullptr &&
            record->dataLength <= sizeof(g_responseReportCapture.response[i].data)) {
            (void)memcpy_s(g_responseReportCapture.response[i].data,
                sizeof(g_responseReportCapture.response[i].data), record->data, record->dataLength);
        }
    }
}

static void OnLeEnhancedConnectionCompleteV2(const HciLeEnhancedConnectionCompleteV2EventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_enhancedConnectionCompleteV2Capture.received = true;
    g_enhancedConnectionCompleteV2Capture.status = eventParam->status;
    g_enhancedConnectionCompleteV2Capture.connectionHandle = eventParam->connectionHandle;
    g_enhancedConnectionCompleteV2Capture.role = eventParam->role;
    (void)memcpy_s(g_enhancedConnectionCompleteV2Capture.peerAddress,
        sizeof(g_enhancedConnectionCompleteV2Capture.peerAddress),
        eventParam->peerAddress.raw, sizeof(eventParam->peerAddress.raw));
    g_enhancedConnectionCompleteV2Capture.advertisingHandle = eventParam->advertisingHandle;
    g_enhancedConnectionCompleteV2Capture.syncHandle = eventParam->syncHandle;
}

static void OnV1LeSyncEstablished(const HciLePeriodicAdvertisingSyncEstablishedEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_v1SyncEstablishedCapture.received = true;
    g_v1SyncEstablishedCapture.syncHandle = eventParam->syncHandle;
}

static void OnV1LeReport(const HciLePeriodicAdvertisingReportEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_v1ReportCapture.received = true;
    g_v1ReportCapture.syncHandle = eventParam->syncHandle;
    g_v1ReportCapture.cteType = eventParam->cteType;
    g_v1ReportCapture.dataStatus = eventParam->dataStatus;
    g_v1ReportCapture.dataLength = eventParam->dataLength;
    if (eventParam->dataLength > 0 && eventParam->data != nullptr &&
        eventParam->dataLength <= sizeof(g_v1ReportCapture.data)) {
        (void)memcpy_s(g_v1ReportCapture.data, sizeof(g_v1ReportCapture.data), eventParam->data,
            eventParam->dataLength);
    }
}

static void OnV1LeSyncTransferReceived(const HciLePeriodicAdvertisingSyncTransferReceivedEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_v1SyncTransferReceivedCapture.received = true;
    g_v1SyncTransferReceivedCapture.syncHandle = eventParam->syncHandle;
}

static void OnV1LeEnhancedConnectionComplete(const HciLeEnhancedConnectionCompleteEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_v1EnhancedConnectionCompleteCapture.received = true;
    g_v1EnhancedConnectionCompleteCapture.connectionHandle = eventParam->connectionHandle;
}

static void OnLeExtendedAdvertisingReport(const HciLeExtendedAdvertisingReportEventParam *eventParam)
{
    if (eventParam == nullptr || eventParam->reports == nullptr) {
        return;
    }
    g_extAdvReportCapture.received = true;
    g_extAdvReportCapture.numReports = eventParam->numReports;
    uint8_t copied = (eventParam->numReports < 2) ? eventParam->numReports : 2;
    for (uint8_t i = 0; i < copied; i++) {
        const HciLeExtendedAdvertisingReport *report = &eventParam->reports[i];
        g_extAdvReportCapture.report[i].eventType = report->eventType;
        g_extAdvReportCapture.report[i].addressType = report->addressType;
        (void)memcpy_s(g_extAdvReportCapture.report[i].address, sizeof(g_extAdvReportCapture.report[i].address),
            report->address.raw, sizeof(report->address.raw));
        g_extAdvReportCapture.report[i].primaryPHY = report->primaryPHY;
        g_extAdvReportCapture.report[i].secondaryPHY = report->secondaryPHY;
        g_extAdvReportCapture.report[i].advertisingSID = report->advertisingSID;
        g_extAdvReportCapture.report[i].txPower = report->txPower;
        g_extAdvReportCapture.report[i].rssi = report->rssi;
    }
}

static HciEventCallbacks g_parseCallbacks;

static void InitParseCallbacks()
{
    g_parseCallbacks = { };
    g_parseCallbacks.lePeriodicAdvertisingSyncEstablishedV2 = OnLeSyncEstablishedV2;
    g_parseCallbacks.lePeriodicAdvertisingReportV2 = OnLeReportV2;
    g_parseCallbacks.lePeriodicAdvertisingSyncTransferReceivedV2 = OnLeSyncTransferReceivedV2;
    g_parseCallbacks.lePeriodicAdvertisingSubeventDataRequest = OnLeSubeventDataRequest;
    g_parseCallbacks.lePeriodicAdvertisingResponseReport = OnLeResponseReport;
    g_parseCallbacks.leEnhancedConnectionCompleteV2 = OnLeEnhancedConnectionCompleteV2;
    g_parseCallbacks.lePeriodicAdvertisingSyncEstablished = OnV1LeSyncEstablished;
    g_parseCallbacks.lePeriodicAdvertisingReport = OnV1LeReport;
    g_parseCallbacks.lePeriodicAdvertisingSyncTransferReceived = OnV1LeSyncTransferReceived;
    g_parseCallbacks.leEnhancedConnectionComplete = OnV1LeEnhancedConnectionComplete;
    g_parseCallbacks.leExtendedAdvertisingReport = OnLeExtendedAdvertisingReport;
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

constexpr uint8_t REPORT_V2_TEST_DATA[] = { 0x70, 0x61, 0x77, 0x72, 0x21 }; // "pawr!"
} // namespace

class StackGapLe54EventParseTest : public testing::Test {
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
        g_syncEstablishedV2Capture.Reset();
        g_reportV2Capture.Reset();
        g_syncTransferReceivedV2Capture.Reset();
        g_subeventDataRequestCapture.Reset();
        g_responseReportCapture.Reset();
        g_enhancedConnectionCompleteV2Capture.Reset();
        g_v1SyncEstablishedCapture.Reset();
        g_v1ReportCapture.Reset();
        g_v1SyncTransferReceivedCapture.Reset();
        g_v1EnhancedConnectionCompleteCapture.Reset();
        g_extAdvReportCapture.Reset();
    }
    void TearDown() override { }
};

/**
 * @tc.number: StackGapLe54_EventParse_SyncEstablishedV2_00100
 * @tc.name:  LE Periodic Advertising Sync Established [v2]（0x24）全字段解析
 * @tc.desc:  [v1] 15 字节 + 尾四值共 19 字节逐字段断言（Sync_Handle/地址/
 *            PHY/Interval 均跨字节序，任何偏移错误都会显形）
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_SyncEstablishedV2_00100, TestSize.Level1)
{
    InjectWire(PERIODIC_SYNC_ESTABLISHED_V2_WIRE, sizeof(PERIODIC_SYNC_ESTABLISHED_V2_WIRE));

    EXPECT_TRUE(g_syncEstablishedV2Capture.received);
    EXPECT_EQ(g_syncEstablishedV2Capture.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(g_syncEstablishedV2Capture.syncHandle, TEST_SYNC_HANDLE);
    EXPECT_EQ(g_syncEstablishedV2Capture.advertisingSid, 0x05);
    EXPECT_EQ(g_syncEstablishedV2Capture.advertiserAddressType, 0x00);
    constexpr uint8_t expectedAddr[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };
    EXPECT_EQ(memcmp(g_syncEstablishedV2Capture.advertiserAddress, expectedAddr, sizeof(expectedAddr)), 0);
    EXPECT_EQ(g_syncEstablishedV2Capture.advertiserPhy, 0x03);
    EXPECT_EQ(g_syncEstablishedV2Capture.periodicAdvertisingInterval, 500);
    EXPECT_EQ(g_syncEstablishedV2Capture.advertiserClockAccuracy, 0x02);
    EXPECT_EQ(g_syncEstablishedV2Capture.numSubevents, 0x04);
    EXPECT_EQ(g_syncEstablishedV2Capture.subeventInterval, 0x0A);
    EXPECT_EQ(g_syncEstablishedV2Capture.responseSlotDelay, 0x02);
    EXPECT_EQ(g_syncEstablishedV2Capture.responseSlotSpacing, 0x08);
}

/**
 * @tc.number: StackGapLe54_EventParse_SyncEstablishedV2Plain_00200
 * @tc.name:  0x24 无子事件：尾四值全 0（7.7.65.14 空参数规则）
 * @tc.desc:  Num_Subevents=0 时 Controller 把其余三个尾值也置 0，解析如实上报
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_SyncEstablishedV2Plain_00200, TestSize.Level1)
{
    InjectWire(PERIODIC_SYNC_ESTABLISHED_V2_PLAIN_WIRE, sizeof(PERIODIC_SYNC_ESTABLISHED_V2_PLAIN_WIRE));

    EXPECT_TRUE(g_syncEstablishedV2Capture.received);
    EXPECT_EQ(g_syncEstablishedV2Capture.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(g_syncEstablishedV2Capture.syncHandle, TEST_SYNC_HANDLE); // 身份字段不变
    EXPECT_EQ(g_syncEstablishedV2Capture.periodicAdvertisingInterval, 500);
    EXPECT_EQ(g_syncEstablishedV2Capture.numSubevents, 0x00);
    EXPECT_EQ(g_syncEstablishedV2Capture.subeventInterval, 0x00);
    EXPECT_EQ(g_syncEstablishedV2Capture.responseSlotDelay, 0x00);
    EXPECT_EQ(g_syncEstablishedV2Capture.responseSlotSpacing, 0x00);
}

/**
 * @tc.number: StackGapLe54_EventParse_SyncEstablishedV2FailureAndTruncated_00300
 * @tc.name:  0x24 失败状态原样透传 + 18 字节截断丢弃
 * @tc.desc:  HCI 解析层对失败事件不带任何改写（clearly-invalid 约定值是 GAP
 *            消费层的上报约定）；19 字节结构门控在 18 字节时拒绝
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_SyncEstablishedV2FailureAndTruncated_00300,
    TestSize.Level1)
{
    InjectWire(PERIODIC_SYNC_ESTABLISHED_V2_FAILURE_WIRE, sizeof(PERIODIC_SYNC_ESTABLISHED_V2_FAILURE_WIRE));

    EXPECT_TRUE(g_syncEstablishedV2Capture.received);
    EXPECT_EQ(g_syncEstablishedV2Capture.status, 0x11);
    EXPECT_EQ(g_syncEstablishedV2Capture.syncHandle, 0x1234); // wire 值原样
    EXPECT_EQ(g_syncEstablishedV2Capture.advertisingSid, 0x0F);
    EXPECT_EQ(g_syncEstablishedV2Capture.advertiserPhy, 0x01);
    EXPECT_EQ(g_syncEstablishedV2Capture.periodicAdvertisingInterval, 200);
    EXPECT_EQ(g_syncEstablishedV2Capture.numSubevents, 0x05);
    EXPECT_EQ(g_syncEstablishedV2Capture.subeventInterval, 0x1E);
    EXPECT_EQ(g_syncEstablishedV2Capture.responseSlotDelay, 0x09);
    EXPECT_EQ(g_syncEstablishedV2Capture.responseSlotSpacing, 0x40);

    g_syncEstablishedV2Capture.Reset();
    InjectWire(PERIODIC_SYNC_ESTABLISHED_V2_TRUNCATED_WIRE, sizeof(PERIODIC_SYNC_ESTABLISHED_V2_TRUNCATED_WIRE));
    EXPECT_FALSE(g_syncEstablishedV2Capture.received);
}

/**
 * @tc.number: StackGapLe54_EventParse_ReportV2_00500
 * @tc.name:  LE Periodic Advertising Report [v2]（0x25）全字段解析
 * @tc.desc:  固定 10 字节头含 PEC/Subevent 的 [v2] 插位（与 [v1] 不同偏移），
 *            Data 载荷在回调内拷贝断言
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_ReportV2_00500, TestSize.Level1)
{
    InjectWire(PERIODIC_ADV_REPORT_V2_WIRE, sizeof(PERIODIC_ADV_REPORT_V2_WIRE));

    EXPECT_TRUE(g_reportV2Capture.received);
    EXPECT_EQ(g_reportV2Capture.syncHandle, 0x0042);
    EXPECT_EQ(g_reportV2Capture.txPower, 0x05);
    EXPECT_EQ(g_reportV2Capture.rssi, static_cast<int8_t>(0xF1));
    EXPECT_EQ(g_reportV2Capture.cteType, 0xFF);
    EXPECT_EQ(g_reportV2Capture.periodicEventCounter, 0x1234);
    EXPECT_EQ(g_reportV2Capture.subevent, 0x03);
    EXPECT_EQ(g_reportV2Capture.dataStatus, 0x00);
    EXPECT_EQ(g_reportV2Capture.dataLength, sizeof(REPORT_V2_TEST_DATA));
    EXPECT_EQ(memcmp(g_reportV2Capture.data, REPORT_V2_TEST_DATA, sizeof(REPORT_V2_TEST_DATA)), 0);
}

/**
 * @tc.number: StackGapLe54_EventParse_ReportV2Plain_00600
 * @tc.name:  0x25 无子事件：Subevent=0xFF、空 Data（7.7.65.15 空参数规则）
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_ReportV2Plain_00600, TestSize.Level1)
{
    InjectWire(PERIODIC_ADV_REPORT_V2_PLAIN_WIRE, sizeof(PERIODIC_ADV_REPORT_V2_PLAIN_WIRE));

    EXPECT_TRUE(g_reportV2Capture.received);
    EXPECT_EQ(g_reportV2Capture.syncHandle, 0x0042);
    EXPECT_EQ(g_reportV2Capture.txPower, 0x7F);
    EXPECT_EQ(g_reportV2Capture.rssi, 0x7F);
    EXPECT_EQ(g_reportV2Capture.periodicEventCounter, 0x0007);
    EXPECT_EQ(g_reportV2Capture.subevent, 0xFF);
    EXPECT_EQ(g_reportV2Capture.dataLength, 0x00);
    EXPECT_EQ(g_reportV2Capture.dataStatus, 0x00);
}

/**
 * @tc.number: StackGapLe54_EventParse_ReportV2Malformed_00700
 * @tc.name:  畸形 0x25 被静默丢弃
 * @tc.desc:  固定头截断（9 字节）、Data_Length 超实际剩余 → 均不触发回调
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_ReportV2Malformed_00700, TestSize.Level1)
{
    InjectWire(PERIODIC_ADV_REPORT_V2_TRUNCATED_WIRE, sizeof(PERIODIC_ADV_REPORT_V2_TRUNCATED_WIRE));
    EXPECT_FALSE(g_reportV2Capture.received);

    g_reportV2Capture.Reset();
    InjectWire(PERIODIC_ADV_REPORT_V2_BAD_DATA_LEN_WIRE, sizeof(PERIODIC_ADV_REPORT_V2_BAD_DATA_LEN_WIRE));
    EXPECT_FALSE(g_reportV2Capture.received);
}

/**
 * @tc.number: StackGapLe54_EventParse_SyncTransferReceivedV2_00800
 * @tc.name:  LE Periodic Advertising Sync Transfer Received [v2]（0x26）全字段解析
 * @tc.desc:  [v1] 19 字节 + 尾四值共 23 字节逐字段断言
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_SyncTransferReceivedV2_00800, TestSize.Level1)
{
    InjectWire(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_WIRE, sizeof(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_WIRE));

    EXPECT_TRUE(g_syncTransferReceivedV2Capture.received);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.connectionHandle, TEST_CONN_HANDLE);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.serviceData, TEST_SERVICE_DATA);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.syncHandle, TEST_SYNC_HANDLE);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.advertisingSid, 0x05);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.advertiserAddressType, 0x01);
    constexpr uint8_t expectedAddr[] = { 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 };
    EXPECT_EQ(memcmp(g_syncTransferReceivedV2Capture.advertiserAddress, expectedAddr, sizeof(expectedAddr)), 0);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.advertiserPhy, 0x02);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.periodicAdvertisingInterval, 100);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.advertiserClockAccuracy, 0x04);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.numSubevents, 0x03);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.subeventInterval, 0x14);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.responseSlotDelay, 0x05);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.responseSlotSpacing, 0x10);
}

/**
 * @tc.number: StackGapLe54_EventParse_SyncTransferReceivedV2NoSubevent_00900
 * @tc.name:  0x26 无子事件：仅 Num_Subevents=0，其余三个尾值原样透传
 * @tc.desc:  与 0x24 不同，0x26 对无子事件训练的其余尾值规范未指定（7.7.65.24
 *            "Host shall ignore"）——解析不得假定 0x00。0x77/0x88/0x99 三个
 *            非零值即为"未清零改写"的回归探针
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_SyncTransferReceivedV2NoSubevent_00900, TestSize.Level1)
{
    InjectWire(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_NOSUBEVENT_WIRE,
        sizeof(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_NOSUBEVENT_WIRE));

    EXPECT_TRUE(g_syncTransferReceivedV2Capture.received);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.connectionHandle, TEST_CONN_HANDLE);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.numSubevents, 0x00);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.subeventInterval, 0x77);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.responseSlotDelay, 0x88);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.responseSlotSpacing, 0x99);
}

/**
 * @tc.number: StackGapLe54_EventParse_SyncTransferReceivedV2FailureAndTruncated_01000
 * @tc.name:  0x26 失败状态原样透传 + 22 字节截断丢弃
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_SyncTransferReceivedV2FailureAndTruncated_01000,
    TestSize.Level1)
{
    InjectWire(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_FAILURE_WIRE,
        sizeof(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_FAILURE_WIRE));

    EXPECT_TRUE(g_syncTransferReceivedV2Capture.received);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.status, 0x3E);
    EXPECT_EQ(g_syncTransferReceivedV2Capture.connectionHandle, TEST_CONN_HANDLE); // 原值保留
    EXPECT_EQ(g_syncTransferReceivedV2Capture.serviceData, TEST_SERVICE_DATA); // 原值保留
    EXPECT_EQ(g_syncTransferReceivedV2Capture.syncHandle, 0x4321); // wire 值原样
    EXPECT_EQ(g_syncTransferReceivedV2Capture.numSubevents, 0x00);

    g_syncTransferReceivedV2Capture.Reset();
    InjectWire(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_TRUNCATED_WIRE,
        sizeof(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_TRUNCATED_WIRE));
    EXPECT_FALSE(g_syncTransferReceivedV2Capture.received);
}

/**
 * @tc.number: StackGapLe54_EventParse_SubeventDataRequest_01100
 * @tc.name:  LE Periodic Advertising Subevent Data Request（0x27）解析与截断丢弃
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_SubeventDataRequest_01100, TestSize.Level1)
{
    InjectWire(SUBEVENT_DATA_REQUEST_WIRE, sizeof(SUBEVENT_DATA_REQUEST_WIRE));

    EXPECT_TRUE(g_subeventDataRequestCapture.received);
    EXPECT_EQ(g_subeventDataRequestCapture.advertisingHandle, 0x0A);
    EXPECT_EQ(g_subeventDataRequestCapture.subeventStart, 0x02);
    EXPECT_EQ(g_subeventDataRequestCapture.subeventDataCount, 0x06);

    g_subeventDataRequestCapture.Reset();
    InjectWire(SUBEVENT_DATA_REQUEST_TRUNCATED_WIRE, sizeof(SUBEVENT_DATA_REQUEST_TRUNCATED_WIRE));
    EXPECT_FALSE(g_subeventDataRequestCapture.received);
}

/**
 * @tc.number: StackGapLe54_EventParse_ResponseReport_01200
 * @tc.name:  LE Periodic Advertising Response Report（0x28）双记录交错解包
 * @tc.desc:  前缀 4 字节 + 每记录（6 字节固定 + Data）交错排布，解析器解交错
 *            到 response[i]；记录数据回调内拷贝
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_ResponseReport_01200, TestSize.Level1)
{
    InjectWire(RESPONSE_REPORT_WIRE, sizeof(RESPONSE_REPORT_WIRE));

    EXPECT_TRUE(g_responseReportCapture.received);
    EXPECT_EQ(g_responseReportCapture.advertisingHandle, 0x0A);
    EXPECT_EQ(g_responseReportCapture.subevent, 0x04);
    EXPECT_EQ(g_responseReportCapture.txStatus, 0x00);
    EXPECT_EQ(g_responseReportCapture.numResponses, 0x02);
    EXPECT_EQ(g_responseReportCapture.response[0].txPower, 0x0F);
    EXPECT_EQ(g_responseReportCapture.response[0].rssi, static_cast<int8_t>(0xD8));
    EXPECT_EQ(g_responseReportCapture.response[0].cteType, 0xFF);
    EXPECT_EQ(g_responseReportCapture.response[0].responseSlot, 0x03);
    EXPECT_EQ(g_responseReportCapture.response[0].dataStatus, 0x00);
    EXPECT_EQ(g_responseReportCapture.response[0].dataLength, 0x03);
    constexpr uint8_t expectedData[] = { 0x01, 0x02, 0x03 };
    EXPECT_EQ(memcmp(g_responseReportCapture.response[0].data, expectedData, sizeof(expectedData)), 0);
    EXPECT_EQ(g_responseReportCapture.response[1].txPower, 0x7F);
    EXPECT_EQ(g_responseReportCapture.response[1].rssi, 0x7F);
    EXPECT_EQ(g_responseReportCapture.response[1].cteType, 0x00);
    EXPECT_EQ(g_responseReportCapture.response[1].responseSlot, 0x1A);
    EXPECT_EQ(g_responseReportCapture.response[1].dataStatus, 0x01);
    EXPECT_EQ(g_responseReportCapture.response[1].dataLength, 0x00);
}

/**
 * @tc.number: StackGapLe54_EventParse_ResponseReportMalformed_01300
 * @tc.name:  畸形 0x28 被静默丢弃
 * @tc.desc:  Num_Responses=0（合法无记录）、>0x19 上限、记录 Data_Length 超
 *            实际剩余、前缀截断（3 字节）四类
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_ResponseReportMalformed_01300, TestSize.Level1)
{
    InjectWire(RESPONSE_REPORT_EMPTY_WIRE, sizeof(RESPONSE_REPORT_EMPTY_WIRE));
    EXPECT_TRUE(g_responseReportCapture.received);
    EXPECT_EQ(g_responseReportCapture.numResponses, 0x00);

    g_responseReportCapture.Reset();
    InjectWire(RESPONSE_REPORT_NUM_TOO_HIGH_WIRE, sizeof(RESPONSE_REPORT_NUM_TOO_HIGH_WIRE));
    EXPECT_FALSE(g_responseReportCapture.received);

    g_responseReportCapture.Reset();
    InjectWire(RESPONSE_REPORT_BAD_DATA_LEN_WIRE, sizeof(RESPONSE_REPORT_BAD_DATA_LEN_WIRE));
    EXPECT_FALSE(g_responseReportCapture.received);

    g_responseReportCapture.Reset();
    InjectWire(RESPONSE_REPORT_TRUNCATED_PREFIX_WIRE, sizeof(RESPONSE_REPORT_TRUNCATED_PREFIX_WIRE));
    EXPECT_FALSE(g_responseReportCapture.received);
}

/**
 * @tc.number: StackGapLe54_EventParse_EnhancedConnectionCompleteV2_01400
 * @tc.name:  LE Enhanced Connection Complete [v2]（0x29）33 字节解析
 * @tc.desc:  失败状态（0x3E）下 [v1] 前缀字段与 No 值尾（0xFF/0xFFFF）如实
 *            上报；31 字节截断（声明长度与物理一致，缺 Sync_Handle 2 字节）
 *            被 33 字节结构门控丢弃（30 字节 [v1] 尺寸的解析器会误收此线——
 *            该截断线是结构门控的回归探针）
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_EnhancedConnectionCompleteV2_01400, TestSize.Level1)
{
    InjectWire(ENHANCED_CONNECTION_COMPLETE_V2_WIRE, sizeof(ENHANCED_CONNECTION_COMPLETE_V2_WIRE));

    EXPECT_TRUE(g_enhancedConnectionCompleteV2Capture.received);
    EXPECT_EQ(g_enhancedConnectionCompleteV2Capture.status, 0x3E);
    EXPECT_EQ(g_enhancedConnectionCompleteV2Capture.connectionHandle, 0x0042);
    EXPECT_EQ(g_enhancedConnectionCompleteV2Capture.role, 0x00);
    constexpr uint8_t expectedPeerAddr[] = { 0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6 };
    EXPECT_EQ(memcmp(g_enhancedConnectionCompleteV2Capture.peerAddress, expectedPeerAddr,
        sizeof(expectedPeerAddr)), 0);
    EXPECT_EQ(g_enhancedConnectionCompleteV2Capture.advertisingHandle, 0xFF); // No Advertising_Handle
    EXPECT_EQ(g_enhancedConnectionCompleteV2Capture.syncHandle, 0xFFFF); // No Sync_Handle

    g_enhancedConnectionCompleteV2Capture.Reset();
    InjectWire(ENHANCED_CONNECTION_COMPLETE_V2_TRUNCATED_WIRE, sizeof(ENHANCED_CONNECTION_COMPLETE_V2_TRUNCATED_WIRE));
    EXPECT_FALSE(g_enhancedConnectionCompleteV2Capture.received);
}

/**
 * @tc.number: StackGapLe54_EventParse_V1WireIsolation_01500
 * @tc.name:  v1 事件（0x0A/0x0E/0x0F/0x18）不触发任何 v2 成员
 * @tc.desc:  [v1] 路径解析保持 5.3 行为不变（P1 兼容项）；0x0F 的线头按运行时
 *            控制器 CTE 能力为 6/7 字节（与解析器同一判据 HciLeController
 *            SupportsCteType，保证线格式与解析一致）
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_V1WireIsolation_01500, TestSize.Level1)
{
    InjectWire(V1_ENHANCED_CONNECTION_COMPLETE_WIRE, sizeof(V1_ENHANCED_CONNECTION_COMPLETE_WIRE));
    EXPECT_TRUE(g_v1EnhancedConnectionCompleteCapture.received);
    EXPECT_EQ(g_v1EnhancedConnectionCompleteCapture.connectionHandle, 0x0042);
    EXPECT_FALSE(g_enhancedConnectionCompleteV2Capture.received);

    g_v1EnhancedConnectionCompleteCapture.Reset();
    InjectWire(V1_PERIODIC_SYNC_ESTABLISHED_WIRE, sizeof(V1_PERIODIC_SYNC_ESTABLISHED_WIRE));
    EXPECT_TRUE(g_v1SyncEstablishedCapture.received);
    EXPECT_EQ(g_v1SyncEstablishedCapture.syncHandle, TEST_SYNC_HANDLE);
    EXPECT_FALSE(g_syncEstablishedV2Capture.received);

    g_v1SyncEstablishedCapture.Reset();
    InjectWire(V1_PERIODIC_SYNC_TRANSFER_RECEIVED_WIRE, sizeof(V1_PERIODIC_SYNC_TRANSFER_RECEIVED_WIRE));
    EXPECT_TRUE(g_v1SyncTransferReceivedCapture.received);
    EXPECT_EQ(g_v1SyncTransferReceivedCapture.syncHandle, TEST_SYNC_HANDLE);
    EXPECT_FALSE(g_syncTransferReceivedV2Capture.received);

    // v1 0x0F：[v1] 6 字节固定头（Sync/…/Data_Length）+ Data，支持 CTE 时
    // CTE_Type 插在 RSSI 与 Data_Status 之间（7 字节）。
    g_v1SyncTransferReceivedCapture.Reset();
    uint8_t v1ReportWire[12] = { 0x3E, 0x09, 0x0F, 0x42, 0x00, 0x7F, 0xF0, 0x00, 0x02, 0xDE, 0xAD };
    if (HciLeControllerSupportsCteType()) {
        v1ReportWire[1] = 0x0A; // 参数 +1（CTE_Type）
        // 把 CTE_Type=0xFF 插入 RSSI(0xF0) 与 Data_Status(0x00) 之间：其后
        // 的 {Data_Status, Data_Length, Data} 各右移一格。
        v1ReportWire[11] = v1ReportWire[10];
        v1ReportWire[10] = v1ReportWire[9];
        v1ReportWire[9] = v1ReportWire[8];
        v1ReportWire[7] = 0xFF;
        v1ReportWire[8] = 0x00;
    }
    InjectWire(v1ReportWire, v1ReportWire[1] + 2);
    EXPECT_TRUE(g_v1ReportCapture.received);
    EXPECT_EQ(g_v1ReportCapture.syncHandle, 0x0042);
    EXPECT_EQ(g_v1ReportCapture.cteType, 0xFF); // 无 CTE 线值或解析器补的 0xFF
    EXPECT_EQ(g_v1ReportCapture.dataStatus, 0x00);
    EXPECT_EQ(g_v1ReportCapture.dataLength, 0x02);
    constexpr uint8_t expectedData[] = { 0xDE, 0xAD };
    EXPECT_EQ(memcmp(g_v1ReportCapture.data, expectedData, sizeof(expectedData)), 0);
    EXPECT_FALSE(g_reportV2Capture.received);
}

/**
 * @tc.number: StackGapLe54_EventParse_V2WireIsolation_01600
 * @tc.name:  v2 事件（0x24/0x25/0x26）不触发 v1 成员
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_V2WireIsolation_01600, TestSize.Level1)
{
    InjectWire(PERIODIC_SYNC_ESTABLISHED_V2_WIRE, sizeof(PERIODIC_SYNC_ESTABLISHED_V2_WIRE));
    EXPECT_TRUE(g_syncEstablishedV2Capture.received);
    EXPECT_FALSE(g_v1SyncEstablishedCapture.received);

    g_syncEstablishedV2Capture.Reset();
    InjectWire(PERIODIC_ADV_REPORT_V2_WIRE, sizeof(PERIODIC_ADV_REPORT_V2_WIRE));
    EXPECT_TRUE(g_reportV2Capture.received);
    EXPECT_FALSE(g_v1ReportCapture.received);

    g_reportV2Capture.Reset();
    InjectWire(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_WIRE, sizeof(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_WIRE));
    EXPECT_TRUE(g_syncTransferReceivedV2Capture.received);
    EXPECT_FALSE(g_v1SyncTransferReceivedCapture.received);
}

/**
 * @tc.number: StackGapLe54_EventParse_UnknownSubevent_01700
 * @tc.name:  未知子事件 0x2A（> LESUBEVENTCODE_MAX）被 LE Meta 分发丢弃
 * @tc.desc:  v1/v2 回调均不触发
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_UnknownSubevent_01700, TestSize.Level1)
{
    InjectWire(UNKNOWN_SUBEVENT_0X2A_WIRE, sizeof(UNKNOWN_SUBEVENT_0X2A_WIRE));

    EXPECT_FALSE(g_syncEstablishedV2Capture.received);
    EXPECT_FALSE(g_reportV2Capture.received);
    EXPECT_FALSE(g_syncTransferReceivedV2Capture.received);
    EXPECT_FALSE(g_subeventDataRequestCapture.received);
    EXPECT_FALSE(g_responseReportCapture.received);
    EXPECT_FALSE(g_enhancedConnectionCompleteV2Capture.received);
    EXPECT_FALSE(g_v1SyncEstablishedCapture.received);
    EXPECT_FALSE(g_v1ReportCapture.received);
    EXPECT_FALSE(g_v1SyncTransferReceivedCapture.received);
    EXPECT_FALSE(g_v1EnhancedConnectionCompleteCapture.received);
}

/**
 * @tc.number: StackGapLe54_EventParse_ExtAdvReportPhyPassThrough_01800
 * @tc.name:  A3：0x0D Ext Adv Report 的 PHY 原样透传（0x04/0x03 与 0x01/0x00）
 * @tc.desc:  7.7.65.13 的 0x04 = LE Coded S=2 值扩展由 Controller 在 bit 41
 *            （ACS Host Support）置位时上报，HCI 层只透传不改写；两个 24 字节
 *            报告步长内的字段逐一断言，任何偏移错误都会在第二份报告上显形
 */
HWTEST_F(StackGapLe54EventParseTest, StackGapLe54_EventParse_ExtAdvReportPhyPassThrough_01800, TestSize.Level1)
{
    InjectWire(EXT_ADV_REPORT_A3_WIRE, sizeof(EXT_ADV_REPORT_A3_WIRE));

    EXPECT_TRUE(g_extAdvReportCapture.received);
    EXPECT_EQ(g_extAdvReportCapture.numReports, 0x02);
    EXPECT_EQ(g_extAdvReportCapture.report[0].eventType, 0x0000);
    EXPECT_EQ(g_extAdvReportCapture.report[0].primaryPHY, 0x04); // LE Coded S=2（raw）
    EXPECT_EQ(g_extAdvReportCapture.report[0].secondaryPHY, 0x03); // LE Coded S=8（raw）
    EXPECT_EQ(g_extAdvReportCapture.report[0].rssi, static_cast<int8_t>(0xC8));
    constexpr uint8_t expectedAddr0[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };
    EXPECT_EQ(memcmp(g_extAdvReportCapture.report[0].address, expectedAddr0, sizeof(expectedAddr0)), 0);
    EXPECT_EQ(g_extAdvReportCapture.report[1].eventType, 0x0004);
    EXPECT_EQ(g_extAdvReportCapture.report[1].addressType, 0x01);
    EXPECT_EQ(g_extAdvReportCapture.report[1].primaryPHY, 0x01); // LE 1M
    EXPECT_EQ(g_extAdvReportCapture.report[1].secondaryPHY, 0x00);
    EXPECT_EQ(g_extAdvReportCapture.report[1].advertisingSID, 0x0F);
    constexpr uint8_t expectedAddr1[] = { 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F };
    EXPECT_EQ(memcmp(g_extAdvReportCapture.report[1].address, expectedAddr1, sizeof(expectedAddr1)), 0);
}

// =====================================================================
// 套件二：活栈 GAP 全链路（BTM_Initialize/BTM_Enable + wire 注入，仿
// stack_gap_le_5_3_test.cpp 的 StackGapLe53Test）。事件经 HCI RX 线程异步
// 处理，用 CallbackWaiter 等待 GAP 任务上的回调。
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

// 同步侧（P4）回调结果：一个结构收 syncEstablished / syncReport /
// syncTransferReceived 三路（按测试只置一路）。
struct PawrSyncResult : CallbackWaiter {
    uint8_t status = 0;
    uint16_t syncHandle = 0;
    uint8_t advSid = 0;
    BtAddr advAddr = { };
    bool advAddrValid = false;
    uint8_t advPhy = 0;
    uint16_t periodicAdvInterval = 0;
    uint8_t numSubevents = 0;
    uint8_t subeventInterval = 0;
    uint8_t responseSlotDelay = 0;
    uint8_t responseSlotSpacing = 0;
    uint16_t connectionHandle = 0;
    uint16_t serviceData = 0;
    uint8_t clockAccuracy = 0;
    uint16_t periodicEventCounter = 0;
    int8_t txPower = 0;
    int8_t rssi = 0;
    uint8_t cteType = 0;
    uint8_t subevent = 0;
    uint8_t dataStatus = 0;
    uint8_t dataLength = 0;
    uint8_t data[32] = { 0 };
};

void OnPawrSyncEstablished(const GapPawrSyncEstablishedReport *report, void *context)
{
    auto *result = static_cast<PawrSyncResult *>(context);
    result->status = report->status;
    result->syncHandle = report->syncHandle;
    result->advSid = report->advSid;
    result->advAddrValid = (report->advAddr != nullptr);
    if (report->advAddr != nullptr) {
        result->advAddr = *report->advAddr;
    }
    result->advPhy = report->advPhy;
    result->periodicAdvInterval = report->periodicAdvInterval;
    result->numSubevents = report->numSubevents;
    result->subeventInterval = report->subeventInterval;
    result->responseSlotDelay = report->responseSlotDelay;
    result->responseSlotSpacing = report->responseSlotSpacing;
    result->Notify();
}

void OnPawrSyncReport(const GapPawrSyncReport *report, void *context)
{
    auto *result = static_cast<PawrSyncResult *>(context);
    result->syncHandle = report->syncHandle;
    result->txPower = report->txPower;
    result->rssi = report->rssi;
    result->cteType = report->cteType;
    result->periodicEventCounter = report->periodicEventCounter;
    result->subevent = report->subevent;
    result->dataStatus = report->dataStatus;
    result->dataLength = report->dataLength;
    if (report->dataLength > 0 && report->data != nullptr && report->dataLength <= sizeof(result->data)) {
        (void)memcpy_s(result->data, sizeof(result->data), report->data, report->dataLength);
    }
    result->Notify();
}

void OnPawrSyncTransferReceived(const GapPawrSyncTransferReceivedReport *report, void *context)
{
    auto *result = static_cast<PawrSyncResult *>(context);
    result->status = report->status;
    result->connectionHandle = report->connectionHandle;
    result->serviceData = report->serviceData;
    result->syncHandle = report->syncHandle;
    result->advSid = report->advSid;
    result->advAddrValid = (report->advAddr != nullptr);
    if (report->advAddr != nullptr) {
        result->advAddr = *report->advAddr;
    }
    result->advPhy = report->advPhy;
    result->periodicAdvInterval = report->periodicAdvInterval;
    result->clockAccuracy = report->clockAccuracy;
    result->numSubevents = report->numSubevents;
    result->subeventInterval = report->subeventInterval;
    result->responseSlotDelay = report->responseSlotDelay;
    result->responseSlotSpacing = report->responseSlotSpacing;
    result->Notify();
}

// 广告侧（P3）回调结果：0x27 数据请求窗口 + 0x28 响应报告（记录数据回调内拷贝）。
struct PawrAdvResult : CallbackWaiter {
    uint8_t advHandle = 0;
    uint8_t subeventStart = 0;
    uint8_t subeventDataCount = 0;
    uint8_t rAdvertisingHandle = 0;
    uint8_t rSubevent = 0;
    uint8_t rTxStatus = 0;
    uint8_t rNumResponses = 0;
    struct {
        int8_t txPower = 0;
        int8_t rssi = 0;
        uint8_t cteType = 0;
        uint8_t responseSlot = 0;
        uint8_t dataStatus = 0;
        uint8_t dataLength = 0;
        uint8_t data[32] = { 0 };
    } response[2];
};

void OnPawrSubeventDataRequest(uint8_t advertisingHandle, uint8_t subeventStart, uint8_t subeventDataCount,
    void *context)
{
    auto *result = static_cast<PawrAdvResult *>(context);
    result->advHandle = advertisingHandle;
    result->subeventStart = subeventStart;
    result->subeventDataCount = subeventDataCount;
    result->Notify();
}

void OnPawrResponseReport(const GapPawrResponseReport *report, void *context)
{
    if (report == nullptr) {
        return;
    }
    auto *result = static_cast<PawrAdvResult *>(context);
    result->rAdvertisingHandle = report->advertisingHandle;
    result->rSubevent = report->subevent;
    result->rTxStatus = report->txStatus;
    result->rNumResponses = report->numResponses;
    uint8_t copied = (report->numResponses < 2) ? report->numResponses : 2;
    for (uint8_t i = 0; i < copied; i++) {
        const GapPawrResponseReportRecord *record = &report->response[i];
        result->response[i].txPower = record->txPower;
        result->response[i].rssi = record->rssi;
        result->response[i].cteType = record->cteType;
        result->response[i].responseSlot = record->responseSlot;
        result->response[i].dataStatus = record->dataStatus;
        result->response[i].dataLength = record->dataLength;
        if (record->dataLength > 0 && record->data != nullptr &&
            record->dataLength <= sizeof(result->response[i].data)) {
            (void)memcpy_s(result->response[i].data, sizeof(result->response[i].data), record->data,
                record->dataLength);
        }
    }
    result->Notify();
}

// RAII guard：用例结束（含断言失败）时自动注销 PAwR 回调组（单槽注册，
// 与本文件外各 5.x 套件的回调 guard 同一模式）。
class PawrSyncCallbackGuard {
public:
    ~PawrSyncCallbackGuard()
    {
        if (GAPIF_DeregisterPawrSyncCallback() != BT_SUCCESS) {
            printf("PawrSyncCallbackGuard: deregister failed; callback may remain registered\n");
        }
    }
    PawrSyncCallbackGuard(const PawrSyncCallbackGuard &) = delete;
    PawrSyncCallbackGuard &operator=(const PawrSyncCallbackGuard &) = delete;
    PawrSyncCallbackGuard() = default;
};

class PawrAdvCallbackGuard {
public:
    ~PawrAdvCallbackGuard()
    {
        if (GAPIF_DeregisterPawrAdvCallback() != BT_SUCCESS) {
            printf("PawrAdvCallbackGuard: deregister failed; callback may remain registered\n");
        }
    }
    PawrAdvCallbackGuard(const PawrAdvCallbackGuard &) = delete;
    PawrAdvCallbackGuard &operator=(const PawrAdvCallbackGuard &) = delete;
    PawrAdvCallbackGuard() = default;
};

constexpr uint8_t TEST_ADDR_1[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };
constexpr uint8_t TEST_ADDR_2[] = { 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 };

// 0x27 请求窗口使用与预填充（0x02/0x03）相交的窗口来触发"flush + 传后丢弃"。
constexpr uint8_t SUBEVENT_DATA_REQUEST_FLUSH_WIRE[] = {
    0x3E,
    0x04,
    0x27,
    0x0A, // Advertising_Handle = 0x0A
    0x02, // Subevent_Start = 2
    0x02, // Subevent_Data_Count = 2
};
} // namespace

class StackGapLe54Test : public testing::Test {
public:
    static void SetUpTestCase(void)
    {
        ASSERT_EQ(BTM_Initialize(), BT_SUCCESS);
        ASSERT_EQ(BTM_Enable(LE_CONTROLLER), BT_SUCCESS);
        ASSERT_TRUE(BTM_IsEnabled(LE_CONTROLLER));
        // PAwR 门控需要全部四种 LE 角色：P3 检查 BROADCASTER|PERIPHERAL、
        // P4 检查 OBSERVER|CENTRAL、P5 建链检查 BROADCASTER|CENTRAL。
        ASSERT_EQ(GAPIF_LeSetRole(GAP_LE_ROLE_CENTRAL | GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_BROADCASTER |
                      GAP_LE_ROLE_PERIPHERAL),
            BT_SUCCESS);
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
 * @tc.number: StackGapLe54_PawrSetSubeventParamsParamGate_00100
 * @tc.name:  P3 GAPIF_LePawrSetSubeventParams 入口与任务层参数门控
 * @tc.desc:  入口：广播句柄 >0xEF、Num_Subevents >0x80 → BT_BAD_PARAM；
 *            任务层（能力无关）：周期广播间隔越界/Min>Max → BT_BAD_PARAM
 *            （间隔检查在能力门之前，5.3 控制器上也成立）
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_PawrSetSubeventParamsParamGate_00100, TestSize.Level1)
{
    // 入口：params 为 NULL。
    EXPECT_EQ(GAPIF_LePawrSetSubeventParams(0x00, nullptr), BT_BAD_PARAM);
    // 入口：advHandle 越界（0xF0 > 0xEF）。
    GapPawrSubeventParams handleOver = { 0x0014, 0x0014, 0x02, 0x06, 0x00, 0x00, 0x00 };
    EXPECT_EQ(GAPIF_LePawrSetSubeventParams(0xF0, &handleOver), BT_BAD_PARAM);
    // 入口：numSubevents 越界（0x81 > 0x80）。
    GapPawrSubeventParams subeventsOver = { 0x0014, 0x0014, 0x81, 0x06, 0x00, 0x00, 0x00 };
    EXPECT_EQ(GAPIF_LePawrSetSubeventParams(0x00, &subeventsOver), BT_BAD_PARAM);
    // 任务层：Interval_Min < 0x0006（能力无关，在角色/能力门之前被拒）。
    GapPawrSubeventParams minBelow = { 0x0005, 0x0014, 0x01, 0x06, 0x00, 0x00, 0x00 };
    EXPECT_EQ(GAPIF_LePawrSetSubeventParams(0x00, &minBelow), BT_BAD_PARAM);
    // 任务层：Interval_Max < 0x0006（min 在界内时即命中 Max 范围检查，
    // gap_le_pawr_adv.c 中该检查先于 Min > Max）。
    GapPawrSubeventParams maxBelow = { 0x0014, 0x0005, 0x01, 0x06, 0x00, 0x00, 0x00 };
    EXPECT_EQ(GAPIF_LePawrSetSubeventParams(0x00, &maxBelow), BT_BAD_PARAM);
    // 任务层：Interval_Min > Interval_Max（双方均在 0x0006-0xFFFF 界内，
    // 专门命中 Min > Max 检查而非任何越界检查）。
    GapPawrSubeventParams minGtMax = { 0x0010, 0x0008, 0x01, 0x06, 0x00, 0x00, 0x00 };
    EXPECT_EQ(GAPIF_LePawrSetSubeventParams(0x00, &minGtMax), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe54_PawrSetSubeventParamsCapabilityBranch_00200
 * @tc.name:  P3 v2 路径能力分支（bit 43/44 门控）
 * @tc.desc:  控制器无 Pawr Advertiser（5.3 及以下，P8）→ 合法 v2 参数返回
 *            BT_NOT_SUPPORT；支持时 → 时序/范围负向 BT_BAD_PARAM + 合法配置
 *            下发（R6：返回非参数错误即视为接受，不等待回包）
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_PawrSetSubeventParamsCapabilityBranch_00200, TestSize.Level1)
{
    constexpr uint16_t interval = 0x0014; // 20 x 1.25 ms = 25 ms
    constexpr uint8_t subInt = 0x06; // 7.5 ms
    if (!BTM_IsControllerSupportPawrAdvertiser()) {
        GapPawrSubeventParams params = { interval, interval, 0x02, subInt, 0x00, 0x00, 0x00 };
        EXPECT_EQ(GAPIF_LePawrSetSubeventParams(0x00, &params), BT_NOT_SUPPORT);
        return;
    }

    // Subevent_Interval < 0x06（范围检查在能力门之后）。
    GapPawrSubeventParams subIntLow = { interval, interval, 0x02, 0x05, 0x00, 0x00, 0x00 };
    EXPECT_EQ(GAPIF_LePawrSetSubeventParams(0x00, &subIntLow), BT_BAD_PARAM);
    // Response_Slot_Delay/Spacing 在 Num_Response_Slots=0x00 时被 Controller
    // 忽略（7.8.61 [v2]）——保留值 0xFF/0x01 此刻不得被本地拒绝。
    GapPawrSubeventParams delayReservedIgnored = { interval, interval, 0x02, subInt, 0xFF, 0x00, 0x00 };
    EXPECT_NE(GAPIF_LePawrSetSubeventParams(0x00, &delayReservedIgnored), BT_BAD_PARAM);
    GapPawrSubeventParams spacingReservedIgnored = { interval, interval, 0x02, subInt, 0x00, 0x01, 0x00 };
    EXPECT_NE(GAPIF_LePawrSetSubeventParams(0x00, &spacingReservedIgnored), BT_BAD_PARAM);
    // 保留值在字段有意义时仍被拒绝：Response_Slot_Delay=0xFF、Spacing=0x01
    //（0x01 亦在 spacing 的关系/范围校验之外单列保留，7.8.61 字段表）。
    GapPawrSubeventParams delayReserved = { interval, interval, 0x02, subInt, 0xFF, 0x00, 0x02 };
    EXPECT_EQ(GAPIF_LePawrSetSubeventParams(0x00, &delayReserved), BT_BAD_PARAM);
    GapPawrSubeventParams spacingReserved = { interval, interval, 0x02, subInt, 0x00, 0x01, 0x02 };
    EXPECT_EQ(GAPIF_LePawrSetSubeventParams(0x00, &spacingReserved), BT_BAD_PARAM);
    // Subevent_Interval x Num_Subevents > Interval_Min（6 x 4 = 24 > 20）。
    GapPawrSubeventParams windowOver = { interval, interval, 0x04, subInt, 0x00, 0x00, 0x00 };
    EXPECT_EQ(GAPIF_LePawrSetSubeventParams(0x00, &windowOver), BT_BAD_PARAM);
    // Response_Slot_Delay >= Subevent_Interval（使用响应槽时需严格小于）。
    GapPawrSubeventParams delayGeSubInt = { interval, interval, 0x02, subInt, subInt, 0x02, 0x01 };
    EXPECT_EQ(GAPIF_LePawrSetSubeventParams(0x00, &delayGeSubInt), BT_BAD_PARAM);
    // Spacing x Num_Response_Slots > 10 x (Subevent_Interval - Delay)：0x28 x 2
    // = 80 > 10 x (6 - 0) = 60。
    GapPawrSubeventParams spacingOver = { interval, interval, 0x02, subInt, 0x00, 0x28, 0x02 };
    EXPECT_EQ(GAPIF_LePawrSetSubeventParams(0x00, &spacingOver), BT_BAD_PARAM);

    // 合法配置：2 个子事件 x 0x06 <= 0x14，无响应槽 → 下发。
    GapPawrSubeventParams params = { interval, interval, 0x02, subInt, 0x00, 0x00, 0x00 };
    int ret = GAPIF_LePawrSetSubeventParams(0x00, &params);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe54_PawrSetSubeventParamsPlainTrainDelegate_00300
 * @tc.name:  P3 numSubevents=0 → [v1] 命令委托（7.8.61 [v2] 语义）
 * @tc.desc:  纯周期广播训练走 [v1]（OCF 0x003E），PAwR 尾参数被忽略（此处
 *            故意给保留值 0x01 的 spacing 也不报错）；无需角色与 PAwR 能力
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_PawrSetSubeventParamsPlainTrainDelegate_00300, TestSize.Level1)
{
    constexpr uint16_t interval = 0x0014;
    // tail（spacing=0x01 保留值）在 [v1] 委托路径被忽略——[v2] 的 0x01 拒绝
    // 只在 numSubevents > 0 时可达。
    GapPawrSubeventParams params = { interval, interval, 0x00, 0x06, 0x00, 0x01, 0x00 };
    int ret = GAPIF_LePawrSetSubeventParams(0x00, &params);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe54_PawrSetSubeventDataGate_00400
 * @tc.name:  P3 GAPIF_LePawrSetSubeventData 门控与本地缓冲
 * @tc.desc:  越界/窗口溢出/超长条目 → BT_BAD_PARAM；合法窗口在 0x0A 句柄上
 *            缓冲（无发送、无能力门——本地状态，任何控制器上都返回
 *            BT_SUCCESS）；重复调用为替换语义
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_PawrSetSubeventDataGate_00400, TestSize.Level1)
{
    uint8_t payload[3] = { 0xAA, 0xBB, 0xCC };
    GapPawrSubeventData entries[2] = {
        { 0x05, 0x01, 3, payload },
        { 0x00, 0x00, 0, nullptr },
    };

    // 入口负向。
    EXPECT_EQ(GAPIF_LePawrSetSubeventData(0xF0, 0x02, 0x02, entries), BT_BAD_PARAM); // 句柄越界
    EXPECT_EQ(GAPIF_LePawrSetSubeventData(0x0A, 0x80, 0x02, entries), BT_BAD_PARAM); // Subevent_Start 越界
    EXPECT_EQ(GAPIF_LePawrSetSubeventData(0x0A, 0x02, 0x00, entries), BT_BAD_PARAM); // 空窗口
    EXPECT_EQ(GAPIF_LePawrSetSubeventData(0x0A, 0x02, 0x81, entries), BT_BAD_PARAM); // 条目数越界
    EXPECT_EQ(GAPIF_LePawrSetSubeventData(0x0A, 0x41, 0x40, entries), BT_BAD_PARAM); // 窗口溢出 0x80
    EXPECT_EQ(GAPIF_LePawrSetSubeventData(0x0A, 0x02, 0x02, nullptr), BT_BAD_PARAM); // data 数组为 NULL
    GapPawrSubeventData badLen[1] = { { 0x05, 0x01, 0xFC, payload } }; // 252 > 251 保留
    EXPECT_EQ(GAPIF_LePawrSetSubeventData(0x0A, 0x02, 0x01, badLen), BT_BAD_PARAM);
    // 250/251 规范合法（7.8.125 线格式 0-251）但单条无法装入任何 0x82 命令
    // 帧（one-octet 参数长度字段：255 - 2 - 4 = 249）→ 入口拒绝，不得滞留在
    // 缓冲中等到每次 0x27 flush 都失败（见 GAP_PAWR_SUBEVENT_DATA_SEND_MAX）。
    GapPawrSubeventData unframable[1] = { { 0x05, 0x01, 0xFA, payload } };
    EXPECT_EQ(GAPIF_LePawrSetSubeventData(0x0A, 0x02, 0x01, unframable), BT_BAD_PARAM);
    unframable[0].dataLength = 0xFB; // 251
    EXPECT_EQ(GAPIF_LePawrSetSubeventData(0x0A, 0x02, 0x01, unframable), BT_BAD_PARAM);
    GapPawrSubeventData badData[1] = { { 0x05, 0x01, 3, nullptr } }; // 长度非 0 而 data 为 NULL
    EXPECT_EQ(GAPIF_LePawrSetSubeventData(0x0A, 0x02, 0x01, badData), BT_BAD_PARAM);

    // 合法窗口在 0x0A 上缓冲子事件 0x02/0x03（用例 00600 现自建同样内容的
    // 预填，不依赖这里的执行顺序；此调用保留本用例的缓冲语义覆盖）。
    EXPECT_EQ(GAPIF_LePawrSetSubeventData(0x0A, 0x02, 0x02, entries), BT_SUCCESS);
    // 替换语义：再调一次仍成功。
    EXPECT_EQ(GAPIF_LePawrSetSubeventData(0x0A, 0x02, 0x02, entries), BT_SUCCESS);
    // 上限 249（单条恰好能装入一个 0x82 命令帧）仍可缓冲；随后用 0 长条目
    // 替换回空，不改变 0x02/0x03 的预填充状态。
    uint8_t maxPayload[0xF9] = { 0 };
    GapPawrSubeventData maxLen[1] = { { 0x05, 0x01, 0xF9, maxPayload } };
    EXPECT_EQ(GAPIF_LePawrSetSubeventData(0x0A, 0x10, 0x01, maxLen), BT_SUCCESS);
    GapPawrSubeventData clear[1] = { { 0x05, 0x01, 0x00, nullptr } };
    EXPECT_EQ(GAPIF_LePawrSetSubeventData(0x0A, 0x10, 0x01, clear), BT_SUCCESS);
}

/**
 * @tc.number: StackGapLe54_RegisterPawrAdvCallback_00500
 * @tc.name:  P3 GAPIF_RegisterPawrAdvCallback / Deregister
 * @tc.desc:  NULL 组 → BT_BAD_PARAM；注册/替换/注销均 BT_SUCCESS，注销幂等
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_RegisterPawrAdvCallback_00500, TestSize.Level1)
{
    EXPECT_EQ(GAPIF_RegisterPawrAdvCallback(nullptr, nullptr), BT_BAD_PARAM);

    GapPawrAdvCallback cb = { };
    EXPECT_EQ(GAPIF_RegisterPawrAdvCallback(&cb, nullptr), BT_SUCCESS);
    PawrAdvCallbackGuard guard; // 覆盖注销幂等与之后的清理
    EXPECT_EQ(GAPIF_RegisterPawrAdvCallback(&cb, nullptr), BT_SUCCESS); // 替换
    EXPECT_EQ(GAPIF_DeregisterPawrAdvCallback(), BT_SUCCESS);
    EXPECT_EQ(GAPIF_DeregisterPawrAdvCallback(), BT_SUCCESS);
}

/**
 * @tc.number: StackGapLe54_PawrSetSyncSubeventParamGate_01000
 * @tc.name:  P4 GAPIF_LePawrSetSyncSubevent / SetResponseData 入口参数门控
 * @tc.desc:  越界句柄、保留 properties、NULL/越界子事件、空/越界计数、
 *            超长数据、data=NULL 配非零长 → BT_BAD_PARAM（能力无关）
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_PawrSetSyncSubeventParamGate_01000, TestSize.Level1)
{
    const uint8_t subevents[] = { 0x02, 0x04 };
    // Sync_Handle 越界（0x0F00 > 0x0EFF）。
    EXPECT_EQ(GAPIF_LePawrSetSyncSubevent(0x0F00, 0x0000, subevents, 2), BT_BAD_PARAM);
    // properties 保留位（bit 7）。
    EXPECT_EQ(GAPIF_LePawrSetSyncSubevent(TEST_SYNC_HANDLE, 0x0080, subevents, 2), BT_BAD_PARAM);
    // subevents 为 NULL。
    EXPECT_EQ(GAPIF_LePawrSetSyncSubevent(TEST_SYNC_HANDLE, 0x0000, nullptr, 2), BT_BAD_PARAM);
    // 空窗口 / 窗口越界。
    EXPECT_EQ(GAPIF_LePawrSetSyncSubevent(TEST_SYNC_HANDLE, 0x0000, subevents, 0), BT_BAD_PARAM);
    EXPECT_EQ(GAPIF_LePawrSetSyncSubevent(TEST_SYNC_HANDLE, 0x0000, subevents, 0x81), BT_BAD_PARAM);
    // 子事件条目越界（0x80 > 0x7F）。
    const uint8_t badSubevent[] = { 0x80 };
    EXPECT_EQ(GAPIF_LePawrSetSyncSubevent(TEST_SYNC_HANDLE, 0x0000, badSubevent, 1), BT_BAD_PARAM);

    // GAPIF_LePawrSetResponseData 入口负向。
    EXPECT_EQ(GAPIF_LePawrSetResponseData(TEST_SYNC_HANDLE, nullptr), BT_BAD_PARAM); // response 为 NULL
    uint8_t data[1] = { 0xAA };
    GapPawrResponseData handleOver = { 0x1234, 0x01, 0x02, 0x03, data, 1 };
    EXPECT_EQ(GAPIF_LePawrSetResponseData(0x0F00, &handleOver), BT_BAD_PARAM); // 句柄越界
    GapPawrResponseData subeventOver = { 0x1234, 0x01, 0x80, 0x03, data, 1 };
    EXPECT_EQ(GAPIF_LePawrSetResponseData(TEST_SYNC_HANDLE, &subeventOver), BT_BAD_PARAM); // responseSubevent 越界
    GapPawrResponseData reserved252 = { 0x1234, 0x01, 0x02, 0x03, data, 0xFC };
    EXPECT_EQ(GAPIF_LePawrSetResponseData(TEST_SYNC_HANDLE, &reserved252), BT_BAD_PARAM); // 252 > 251 保留
    // 248/251 规范合法（7.8.126 线格式 0-251）但单条无法装入任何 0x83 命令帧
    //（one-octet 参数长度字段：255 - 8 固定字段 = 247）→ 入口拒绝
    //（见 GAP_PAWR_RESPONSE_DATA_SEND_MAX）。
    GapPawrResponseData unframable248 = { 0x1234, 0x01, 0x02, 0x03, data, 0xF8 };
    EXPECT_EQ(GAPIF_LePawrSetResponseData(TEST_SYNC_HANDLE, &unframable248), BT_BAD_PARAM); // 248
    GapPawrResponseData unframable251 = { 0x1234, 0x01, 0x02, 0x03, data, 0xFB };
    EXPECT_EQ(GAPIF_LePawrSetResponseData(TEST_SYNC_HANDLE, &unframable251), BT_BAD_PARAM); // 251
    GapPawrResponseData nullData = { 0x1234, 0x01, 0x02, 0x03, nullptr, 1 };
    EXPECT_EQ(GAPIF_LePawrSetResponseData(TEST_SYNC_HANDLE, &nullData), BT_BAD_PARAM); // 长度非 0 而 data 为 NULL
}

/**
 * @tc.number: StackGapLe54_PawrSyncSideCapabilityBranch_01100
 * @tc.name:  P4 sync 侧能力分支（bit 44 门控）
 * @tc.desc:  控制器无 Pawr Scanner（5.3 及以下，P8）→ 合法调用返回
 *            BT_NOT_SUPPORT；支持时 → 命令下发（R6：不等待回包）
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_PawrSyncSideCapabilityBranch_01100, TestSize.Level1)
{
    const uint8_t subevents[] = { 0x02, 0x04 };
    if (!BTM_IsControllerSupportPawrScanner()) {
        EXPECT_EQ(GAPIF_LePawrSetSyncSubevent(TEST_SYNC_HANDLE, 0x0000, subevents, 2), BT_NOT_SUPPORT);
        uint8_t data[1] = { 0xAA };
        GapPawrResponseData response = { 0x1234, 0x01, 0x02, 0x03, data, 1 };
        EXPECT_EQ(GAPIF_LePawrSetResponseData(TEST_SYNC_HANDLE, &response), BT_NOT_SUPPORT);
        return;
    }

    int ret = GAPIF_LePawrSetSyncSubevent(TEST_SYNC_HANDLE, 0x0000, subevents, 2);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
    uint8_t data[1] = { 0xAA };
    GapPawrResponseData response = { 0x1234, 0x01, 0x02, 0x03, data, 1 };
    ret = GAPIF_LePawrSetResponseData(TEST_SYNC_HANDLE, &response);
    EXPECT_NE(ret, BT_BAD_PARAM);
    EXPECT_NE(ret, BT_NO_MEMORY);
}

/**
 * @tc.number: StackGapLe54_RegisterPawrSyncCallback_01200
 * @tc.name:  P4 GAPIF_RegisterPawrSyncCallback / Deregister
 * @tc.desc:  NULL 组 → BT_BAD_PARAM；注册/注销均 BT_SUCCESS，注销幂等
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_RegisterPawrSyncCallback_01200, TestSize.Level1)
{
    EXPECT_EQ(GAPIF_RegisterPawrSyncCallback(nullptr, nullptr), BT_BAD_PARAM);

    GapPawrSyncCallback cb = { };
    EXPECT_EQ(GAPIF_RegisterPawrSyncCallback(&cb, nullptr), BT_SUCCESS);
    PawrSyncCallbackGuard guard;
    EXPECT_EQ(GAPIF_DeregisterPawrSyncCallback(), BT_SUCCESS);
    EXPECT_EQ(GAPIF_DeregisterPawrSyncCallback(), BT_SUCCESS);
}

/**
 * @tc.number: StackGapLe54_SyncEstablishedV2E2E_01300
 * @tc.name:  P4 活栈注入 0x24 成功 → syncEstablished 十参数逐一断言
 * @tc.desc:  全生产路径：HCI RX 注入 wire → gap_hci_receive → GAP 任务 →
 *            GapPawrSyncCallback.syncEstablished（地址与 [v1] 相同的 6 字节）
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_SyncEstablishedV2E2E_01300, TestSize.Level1)
{
    PawrSyncResult result;
    GapPawrSyncCallback cb = { };
    cb.syncEstablished = OnPawrSyncEstablished;
    ASSERT_EQ(GAPIF_RegisterPawrSyncCallback(&cb, &result), BT_SUCCESS);
    PawrSyncCallbackGuard guard;

    ASSERT_EQ(HCI_InjectReceivedEvent(PERIODIC_SYNC_ESTABLISHED_V2_WIRE,
                  sizeof(PERIODIC_SYNC_ESTABLISHED_V2_WIRE)),
        BT_SUCCESS);

    ASSERT_TRUE(result.Wait()) << "syncEstablished not delivered through the GAP task";
    EXPECT_EQ(result.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(result.syncHandle, TEST_SYNC_HANDLE);
    EXPECT_EQ(result.advSid, 0x05);
    EXPECT_TRUE(result.advAddrValid);
    EXPECT_EQ(result.advAddr.type, 0x00);
    EXPECT_EQ(memcmp(result.advAddr.addr, TEST_ADDR_1, sizeof(TEST_ADDR_1)), 0);
    EXPECT_EQ(result.advPhy, 0x03);
    EXPECT_EQ(result.periodicAdvInterval, 500);
    EXPECT_EQ(result.numSubevents, 0x04);
    EXPECT_EQ(result.subeventInterval, 0x0A);
    EXPECT_EQ(result.responseSlotDelay, 0x02);
    EXPECT_EQ(result.responseSlotSpacing, 0x08);
}

/**
 * @tc.number: StackGapLe54_SyncEstablishedV2PlainE2E_01400
 * @tc.name:  P4 活栈注入 0x24 无子事件 → 尾四值全 0
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_SyncEstablishedV2PlainE2E_01400, TestSize.Level1)
{
    PawrSyncResult result;
    GapPawrSyncCallback cb = { };
    cb.syncEstablished = OnPawrSyncEstablished;
    ASSERT_EQ(GAPIF_RegisterPawrSyncCallback(&cb, &result), BT_SUCCESS);
    PawrSyncCallbackGuard guard;

    ASSERT_EQ(HCI_InjectReceivedEvent(PERIODIC_SYNC_ESTABLISHED_V2_PLAIN_WIRE,
                  sizeof(PERIODIC_SYNC_ESTABLISHED_V2_PLAIN_WIRE)),
        BT_SUCCESS);

    ASSERT_TRUE(result.Wait()) << "plain-train syncEstablished not delivered";
    EXPECT_EQ(result.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(result.syncHandle, TEST_SYNC_HANDLE);
    EXPECT_EQ(result.periodicAdvInterval, 500);
    EXPECT_EQ(result.numSubevents, 0x00);
    EXPECT_EQ(result.subeventInterval, 0x00);
    EXPECT_EQ(result.responseSlotDelay, 0x00);
    EXPECT_EQ(result.responseSlotSpacing, 0x00);
}

/**
 * @tc.number: StackGapLe54_SyncEstablishedV2FailureE2E_01500
 * @tc.name:  P4 活栈注入 0x24 失败 → clearly-invalid 约定值上报
 * @tc.desc:  失败时身份字段无效，按 5.4 前 established 处理约定上报
 *            Sync_Handle=0xFFFF/SID=0xFF/addr=NULL/PHY=0x00/Interval=0xFFFF/
 *            尾值全 0，且非法 Address_Type 在失败路径不被检查
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_SyncEstablishedV2FailureE2E_01500, TestSize.Level1)
{
    PawrSyncResult result;
    GapPawrSyncCallback cb = { };
    cb.syncEstablished = OnPawrSyncEstablished;
    ASSERT_EQ(GAPIF_RegisterPawrSyncCallback(&cb, &result), BT_SUCCESS);
    PawrSyncCallbackGuard guard;

    ASSERT_EQ(HCI_InjectReceivedEvent(PERIODIC_SYNC_ESTABLISHED_V2_FAILURE_WIRE,
                  sizeof(PERIODIC_SYNC_ESTABLISHED_V2_FAILURE_WIRE)),
        BT_SUCCESS);

    ASSERT_TRUE(result.Wait()) << "failed syncEstablished not delivered";
    EXPECT_EQ(result.status, 0x11);
    EXPECT_EQ(result.syncHandle, 0xFFFF);
    EXPECT_EQ(result.advSid, 0xFF);
    EXPECT_FALSE(result.advAddrValid);
    EXPECT_EQ(result.advPhy, 0x00);
    EXPECT_EQ(result.periodicAdvInterval, 0xFFFF);
    EXPECT_EQ(result.numSubevents, 0x00);
    EXPECT_EQ(result.subeventInterval, 0x00);
    EXPECT_EQ(result.responseSlotDelay, 0x00);
    EXPECT_EQ(result.responseSlotSpacing, 0x00);
}

/**
 * @tc.number: StackGapLe54_SyncReportV2E2E_01600
 * @tc.name:  P4 活栈注入 0x25 → syncReport（PEC/Subevent/Data 回调内拷贝）
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_SyncReportV2E2E_01600, TestSize.Level1)
{
    PawrSyncResult result;
    GapPawrSyncCallback cb = { };
    cb.syncReport = OnPawrSyncReport;
    ASSERT_EQ(GAPIF_RegisterPawrSyncCallback(&cb, &result), BT_SUCCESS);
    PawrSyncCallbackGuard guard;

    ASSERT_EQ(HCI_InjectReceivedEvent(PERIODIC_ADV_REPORT_V2_WIRE, sizeof(PERIODIC_ADV_REPORT_V2_WIRE)),
        BT_SUCCESS);

    ASSERT_TRUE(result.Wait()) << "syncReport not delivered through the GAP task";
    EXPECT_EQ(result.syncHandle, 0x0042);
    EXPECT_EQ(result.txPower, 0x05);
    EXPECT_EQ(result.rssi, static_cast<int8_t>(0xF1));
    EXPECT_EQ(result.cteType, 0xFF);
    EXPECT_EQ(result.periodicEventCounter, 0x1234);
    EXPECT_EQ(result.subevent, 0x03);
    EXPECT_EQ(result.dataStatus, 0x00);
    EXPECT_EQ(result.dataLength, sizeof(REPORT_V2_TEST_DATA));
    EXPECT_EQ(memcmp(result.data, REPORT_V2_TEST_DATA, sizeof(REPORT_V2_TEST_DATA)), 0);
}

/**
 * @tc.number: StackGapLe54_SyncReportV2PlainE2E_01700
 * @tc.name:  P4 活栈注入 0x25 无子事件 → Subevent=0xFF、空数据
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_SyncReportV2PlainE2E_01700, TestSize.Level1)
{
    PawrSyncResult result;
    GapPawrSyncCallback cb = { };
    cb.syncReport = OnPawrSyncReport;
    ASSERT_EQ(GAPIF_RegisterPawrSyncCallback(&cb, &result), BT_SUCCESS);
    PawrSyncCallbackGuard guard;

    ASSERT_EQ(HCI_InjectReceivedEvent(PERIODIC_ADV_REPORT_V2_PLAIN_WIRE, sizeof(PERIODIC_ADV_REPORT_V2_PLAIN_WIRE)),
        BT_SUCCESS);

    ASSERT_TRUE(result.Wait()) << "plain-train syncReport not delivered";
    EXPECT_EQ(result.syncHandle, 0x0042);
    EXPECT_EQ(result.periodicEventCounter, 0x0007);
    EXPECT_EQ(result.subevent, 0xFF);
    EXPECT_EQ(result.dataLength, 0x00);
}

/**
 * @tc.number: StackGapLe54_SyncTransferReceivedV2E2E_01800
 * @tc.name:  P4 活栈注入 0x26 成功 → syncTransferReceived 全字段
 * @tc.desc:  含 Clock_Accuracy 与尾四值；随机地址类型原样上报
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_SyncTransferReceivedV2E2E_01800, TestSize.Level1)
{
    PawrSyncResult result;
    GapPawrSyncCallback cb = { };
    cb.syncTransferReceived = OnPawrSyncTransferReceived;
    ASSERT_EQ(GAPIF_RegisterPawrSyncCallback(&cb, &result), BT_SUCCESS);
    PawrSyncCallbackGuard guard;

    ASSERT_EQ(HCI_InjectReceivedEvent(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_WIRE,
                  sizeof(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_WIRE)),
        BT_SUCCESS);

    ASSERT_TRUE(result.Wait()) << "syncTransferReceived not delivered through the GAP task";
    EXPECT_EQ(result.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(result.connectionHandle, TEST_CONN_HANDLE);
    EXPECT_EQ(result.serviceData, TEST_SERVICE_DATA);
    EXPECT_EQ(result.syncHandle, TEST_SYNC_HANDLE);
    EXPECT_EQ(result.advSid, 0x05);
    EXPECT_TRUE(result.advAddrValid);
    EXPECT_EQ(result.advAddr.type, 0x01);
    EXPECT_EQ(memcmp(result.advAddr.addr, TEST_ADDR_2, sizeof(TEST_ADDR_2)), 0);
    EXPECT_EQ(result.advPhy, 0x02);
    EXPECT_EQ(result.periodicAdvInterval, 100);
    EXPECT_EQ(result.clockAccuracy, 0x04);
    EXPECT_EQ(result.numSubevents, 0x03);
    EXPECT_EQ(result.subeventInterval, 0x14);
    EXPECT_EQ(result.responseSlotDelay, 0x05);
    EXPECT_EQ(result.responseSlotSpacing, 0x10);
}

/**
 * @tc.number: StackGapLe54_SyncTransferReceivedV2NoSubeventE2E_01900
 * @tc.name:  P4 活栈注入 0x26 无子事件 → 非零尾原样透传（不得假定 0）
 * @tc.desc:  与 0x24 的空参数规则不同（7.7.65.24）：无子事件时仅
 *            Num_Subevents=0 有定义，其余三个尾值透传（0x77/0x88/0x99），
 *            消费层必须忽略而非清零
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_SyncTransferReceivedV2NoSubeventE2E_01900, TestSize.Level1)
{
    PawrSyncResult result;
    GapPawrSyncCallback cb = { };
    cb.syncTransferReceived = OnPawrSyncTransferReceived;
    ASSERT_EQ(GAPIF_RegisterPawrSyncCallback(&cb, &result), BT_SUCCESS);
    PawrSyncCallbackGuard guard;

    ASSERT_EQ(HCI_InjectReceivedEvent(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_NOSUBEVENT_WIRE,
                  sizeof(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_NOSUBEVENT_WIRE)),
        BT_SUCCESS);

    ASSERT_TRUE(result.Wait()) << "no-subevent syncTransferReceived not delivered";
    EXPECT_EQ(result.status, HCI_STATUS_SUCCESS);
    EXPECT_EQ(result.connectionHandle, TEST_CONN_HANDLE);
    EXPECT_EQ(result.serviceData, TEST_SERVICE_DATA);
    EXPECT_EQ(result.syncHandle, TEST_SYNC_HANDLE);
    EXPECT_EQ(result.periodicAdvInterval, 100);
    EXPECT_EQ(result.clockAccuracy, 0x04);
    EXPECT_EQ(result.numSubevents, 0x00);
    EXPECT_EQ(result.subeventInterval, 0x77);
    EXPECT_EQ(result.responseSlotDelay, 0x88);
    EXPECT_EQ(result.responseSlotSpacing, 0x99);
}

/**
 * @tc.number: StackGapLe54_SyncTransferReceivedV2FailureE2E_02000
 * @tc.name:  P4 活栈注入 0x26 失败 → 身份与列车参数仍有效原样上报，仅 Sync_Handle 无效
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_SyncTransferReceivedV2FailureE2E_02000, TestSize.Level1)
{
    PawrSyncResult result;
    GapPawrSyncCallback cb = { };
    cb.syncTransferReceived = OnPawrSyncTransferReceived;
    ASSERT_EQ(GAPIF_RegisterPawrSyncCallback(&cb, &result), BT_SUCCESS);
    PawrSyncCallbackGuard guard;

    ASSERT_EQ(HCI_InjectReceivedEvent(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_FAILURE_WIRE,
                  sizeof(PERIODIC_SYNC_TRANSFER_RECEIVED_V2_FAILURE_WIRE)),
        BT_SUCCESS);

    ASSERT_TRUE(result.Wait()) << "failed syncTransferReceived not delivered";
    EXPECT_EQ(result.status, 0x3E);
    // 7.7.65,24：Status 非零时除 Sync_Handle 外全部参数仍有效——
    // 源连接、服务数据、广告者身份与列车参数原样上报。
    EXPECT_EQ(result.connectionHandle, TEST_CONN_HANDLE);
    EXPECT_EQ(result.serviceData, TEST_SERVICE_DATA);
    // 失败时 Sync_Handle 无效，归一为 0xFFFF（Host shall ignore）。
    EXPECT_EQ(result.syncHandle, 0xFFFF);
    EXPECT_EQ(result.advSid, 0x0A);
    EXPECT_TRUE(result.advAddrValid);
    EXPECT_EQ(result.advAddr.type, BT_PUBLIC_DEVICE_ADDRESS);
    EXPECT_EQ(result.advAddr.addr[0], 0xAA);
    EXPECT_EQ(result.advAddr.addr[BT_ADDRESS_SIZE - 1], 0xFF);
    EXPECT_EQ(result.advPhy, 0x01);
    EXPECT_EQ(result.periodicAdvInterval, 15);
    EXPECT_EQ(result.clockAccuracy, 0x01);
    EXPECT_EQ(result.numSubevents, 0x00);
    EXPECT_EQ(result.subeventInterval, 0x00);
    EXPECT_EQ(result.responseSlotDelay, 0x00);
    EXPECT_EQ(result.responseSlotSpacing, 0x00);
}

// 00600 的阶段 2/3：同一句柄再次请求同一窗口（缓冲已丢弃，请求仍上报，
// 不再有 flush 命令）与非法窗口（Start + Count > 0x80）被 GAP 层丢弃。
static void CheckRepeatedRequestAndInvalidWindow(PawrAdvResult *result)
{
    result->Reset();
    ASSERT_EQ(HCI_InjectReceivedEvent(SUBEVENT_DATA_REQUEST_FLUSH_WIRE, sizeof(SUBEVENT_DATA_REQUEST_FLUSH_WIRE)),
        BT_SUCCESS);
    ASSERT_TRUE(result->Wait()) << "second subeventDataRequest not delivered";
    EXPECT_EQ(result->advHandle, 0x0A);
    EXPECT_EQ(result->subeventStart, 0x02);
    EXPECT_EQ(result->subeventDataCount, 0x02);

    // 非法窗口（Start + Count > 0x80）被 GAP 层丢弃：500ms 内无回调。
    result->Reset();
    constexpr uint8_t invalidWindowWire[] = {
        0x3E,
        0x04,
        0x27,
        0x0A, // Advertising_Handle
        0x00, // Subevent_Start
        0x81, // Subevent_Data_Count（> 0x80）
    };
    ASSERT_EQ(HCI_InjectReceivedEvent(invalidWindowWire, sizeof(invalidWindowWire)), BT_SUCCESS);
    EXPECT_FALSE(result->Wait(DROP_PROBE_TIMEOUT_MS)) << "invalid 0x27 window must be dropped by the GAP layer";
}

// 00600 的阶段 4/5：0x28 双记录报告（回调内拷贝断言）与超上限报告被丢弃。
static void CheckPawrResponseReportAndLimit(PawrAdvResult *result)
{
    result->Reset();
    ASSERT_EQ(HCI_InjectReceivedEvent(RESPONSE_REPORT_WIRE, sizeof(RESPONSE_REPORT_WIRE)), BT_SUCCESS);
    ASSERT_TRUE(result->Wait()) << "responseReport not delivered through the GAP task";
    EXPECT_EQ(result->rAdvertisingHandle, 0x0A);
    EXPECT_EQ(result->rSubevent, 0x04);
    EXPECT_EQ(result->rTxStatus, 0x00);
    EXPECT_EQ(result->rNumResponses, 0x02);
    EXPECT_EQ(result->response[0].txPower, 0x0F);
    EXPECT_EQ(result->response[0].rssi, static_cast<int8_t>(0xD8));
    EXPECT_EQ(result->response[0].cteType, 0xFF);
    EXPECT_EQ(result->response[0].responseSlot, 0x03);
    EXPECT_EQ(result->response[0].dataStatus, 0x00);
    EXPECT_EQ(result->response[0].dataLength, 0x03);
    constexpr uint8_t expectedRecordData[] = { 0x01, 0x02, 0x03 };
    EXPECT_EQ(memcmp(result->response[0].data, expectedRecordData, sizeof(expectedRecordData)), 0);
    EXPECT_EQ(result->response[1].txPower, 0x7F);
    EXPECT_EQ(result->response[1].rssi, 0x7F);
    EXPECT_EQ(result->response[1].cteType, 0x00);
    EXPECT_EQ(result->response[1].responseSlot, 0x1A);
    EXPECT_EQ(result->response[1].dataStatus, 0x01);
    EXPECT_EQ(result->response[1].dataLength, 0x00);

    // 超上限 0x19 的响应报告被 GAP 层丢弃。
    result->Reset();
    ASSERT_EQ(HCI_InjectReceivedEvent(RESPONSE_REPORT_NUM_TOO_HIGH_WIRE, sizeof(RESPONSE_REPORT_NUM_TOO_HIGH_WIRE)),
        BT_SUCCESS);
    EXPECT_FALSE(result->Wait(DROP_PROBE_TIMEOUT_MS)) << "0x28 with numResponses above 0x19 must be dropped";
}

/**
 * @tc.number: StackGapLe54_SubeventDataRequestE2E_00600
 * @tc.name:  P3 活栈注入 0x27/0x28 → subeventDataRequest / responseReport
 * @tc.desc:  0x27 请求窗口与自建预填充（句柄 0x0A 上 0x02/0x03，内容与用例
 *            00400 的缓冲相同）相交 → 触发 flush 把数据经 HCI 命令传给控制
 *            器并丢弃缓冲（P7"数据只传一次"路径）；无缓冲后的窗口直接上报。
 *            0x28 双记录报告在回调内拷贝断言
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_SubeventDataRequestE2E_00600, TestSize.Level1)
{
    PawrAdvResult result;
    GapPawrAdvCallback cb = { };
    cb.subeventDataRequest = OnPawrSubeventDataRequest;
    cb.responseReport = OnPawrResponseReport;
    ASSERT_EQ(GAPIF_RegisterPawrAdvCallback(&cb, &result), BT_SUCCESS);
    PawrAdvCallbackGuard guard;

    // 自建预填，使本用例不依赖用例 00400 的执行顺序或缓冲残留：在句柄 0x0A
    // 上缓冲子事件 0x02/0x03（内容与 00400 相同），先于本用例对 0x02/0x03
    // 的重填同样会命中下面的 flush。
    uint8_t payload[3] = { 0xAA, 0xBB, 0xCC };
    GapPawrSubeventData entries[2] = {
        { 0x05, 0x01, 3, payload },
        { 0x00, 0x00, 0, nullptr },
    };
    EXPECT_EQ(GAPIF_LePawrSetSubeventData(0x0A, 0x02, 0x02, entries), BT_SUCCESS);

    // 阶段 1：请求窗口与缓冲相交 → flush 经 HCI 命令下发并丢弃缓冲。
    ASSERT_EQ(HCI_InjectReceivedEvent(SUBEVENT_DATA_REQUEST_FLUSH_WIRE, sizeof(SUBEVENT_DATA_REQUEST_FLUSH_WIRE)),
        BT_SUCCESS);
    ASSERT_TRUE(result.Wait()) << "subeventDataRequest not delivered through the GAP task";
    EXPECT_EQ(result.advHandle, 0x0A);
    EXPECT_EQ(result.subeventStart, 0x02);
    EXPECT_EQ(result.subeventDataCount, 0x02);

    CheckRepeatedRequestAndInvalidWindow(&result);
    CheckPawrResponseReportAndLimit(&result);
}

/**
 * @tc.number: StackGapLe54_BtmPawrConnectGates_03000
 * @tc.name:  P5 BTM_LeConnectFromPawr / BTM_GetLeConnectionPawrAssociation 门控
 * @tc.desc:  混合 0xFF、句柄/子事件越界、NULL 地址、both-0xFF+NULL（回落
 *            BTM_LeConnect 的 NULL 门）→ BT_BAD_PARAM（全部在发送前被拒，
 *            hermetic）；关联查询 NULL 输出 → BT_BAD_PARAM、未知句柄 →
 *            BT_BAD_STATUS；合法对在无 PAwR 能力时 → BT_NOT_SUPPORT
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_BtmPawrConnectGates_03000, TestSize.Level1)
{
    const BtAddr addr = {
        { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 },
        0x00, // BT_PUBLIC_DEVICE_ADDRESS
    };
    const BtAddr *nullAddr = nullptr;

    // 0xFF 只出现其一（7.8.66 [v2]：要么都 0xFF 要么都有效）。
    EXPECT_EQ(BTM_LeConnectFromPawr(&addr, 0xFF, 0x00), BT_BAD_PARAM);
    EXPECT_EQ(BTM_LeConnectFromPawr(&addr, 0x00, 0xFF), BT_BAD_PARAM);
    // 句柄越界（0xF0 > 0xEF）/ 子事件越界（0x80 > 0x7F）。
    EXPECT_EQ(BTM_LeConnectFromPawr(&addr, 0xF0, 0x00), BT_BAD_PARAM);
    EXPECT_EQ(BTM_LeConnectFromPawr(&addr, 0x00, 0x80), BT_BAD_PARAM);
    // 有效对 + NULL 地址。
    EXPECT_EQ(BTM_LeConnectFromPawr(nullAddr, 0x00, 0x00), BT_BAD_PARAM);
    // both-0xFF 回落：NULL 地址在 BTM_LeConnect 的地址门被拒（无发送）。
    EXPECT_EQ(BTM_LeConnectFromPawr(nullAddr, 0xFF, 0xFF), BT_BAD_PARAM);

    // 关联查询门控。
    uint8_t advertisingHandle = 0;
    uint16_t syncHandle = 0;
    EXPECT_EQ(BTM_GetLeConnectionPawrAssociation(0x0042, nullptr, &syncHandle), BT_BAD_PARAM);
    EXPECT_EQ(BTM_GetLeConnectionPawrAssociation(0x0042, &advertisingHandle, nullptr), BT_BAD_PARAM);
    // 无此连接（活栈未建链）→ BT_BAD_STATUS。
    EXPECT_EQ(BTM_GetLeConnectionPawrAssociation(0x0042, &advertisingHandle, &syncHandle), BT_BAD_STATUS);

    // 能力分支：合法对（0x00,0x00）在无 PAwR Advertiser 能力时被拒。
    if (!BTM_IsControllerSupportPawrAdvertiser()) {
        EXPECT_EQ(BTM_LeConnectFromPawr(&addr, 0x00, 0x00), BT_NOT_SUPPORT);
    }
    // （有能力的控制器上合法对会真实下发建链命令，无 hermetic 取消——超出
    // wire 单测范围，见文件头局限说明。）
}

/**
 * @tc.number: StackGapLe54_GapifExtCreateConnFromPawrGates_03100
 * @tc.name:  P5 GAPIF_LeExtCreateConnFromPawr 门控与能力分支
 * @tc.desc:  NULL 地址 / 混合 0xFF / 越界 → BT_BAD_PARAM（发送前被拒）；
 *            合法对在无 PAwR 能力时 → BT_NOT_SUPPORT（P8）
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_GapifExtCreateConnFromPawrGates_03100, TestSize.Level1)
{
    const BtAddr addr = {
        { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 },
        0x00, // BT_PUBLIC_DEVICE_ADDRESS
    };
    EXPECT_EQ(GAPIF_LeExtCreateConnFromPawr(0x00, 0x01, nullptr), BT_BAD_PARAM);
    EXPECT_EQ(GAPIF_LeExtCreateConnFromPawr(0xFF, 0x00, &addr), BT_BAD_PARAM); // 0xFF 只现其一
    EXPECT_EQ(GAPIF_LeExtCreateConnFromPawr(0x00, 0xFF, &addr), BT_BAD_PARAM);
    EXPECT_EQ(GAPIF_LeExtCreateConnFromPawr(0xF0, 0x00, &addr), BT_BAD_PARAM); // 句柄越界
    EXPECT_EQ(GAPIF_LeExtCreateConnFromPawr(0x00, 0x80, &addr), BT_BAD_PARAM); // 子事件越界

    if (!BTM_IsControllerSupportPawrAdvertiser()) {
        EXPECT_EQ(GAPIF_LeExtCreateConnFromPawr(0x00, 0x01, &addr), BT_NOT_SUPPORT);
    }
    // 有能力的控制器上合法对会真实下发（BTM_LeConnectFromPawr）——见文件头
    // 局限说明；both-0xFF 回落（BTM_LeConnect）同理不在此调用。
}

/**
 * @tc.number: StackGapLe54_EnhancedConnectionCompleteV2FailureE2E_03200
 * @tc.name:  P5 活栈注入 0x29 失败（0x3E）→ 普通失败路径、无异常状态残留
 * @tc.desc:  0x3E 是"从 PAwR 建链失败"的典型回包：走既有连接失败路径（无
 *            CONNECTING 记录时完全 inert），不产生 PAwR 回调、不创建连接
 *            记录——关联查询前后一致（BT_BAD_STATUS），无残留
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_EnhancedConnectionCompleteV2FailureE2E_03200, TestSize.Level1)
{
    uint8_t advertisingHandle = 0;
    uint16_t syncHandle = 0;
    // 注入前：该连接句柄不存在。
    EXPECT_EQ(BTM_GetLeConnectionPawrAssociation(0x0042, &advertisingHandle, &syncHandle), BT_BAD_STATUS);

    // 注册 sync 回调组：断言 0x29 失败不产生任何 PAwR 回调。
    PawrSyncResult result;
    GapPawrSyncCallback cb = { };
    cb.syncEstablished = OnPawrSyncEstablished;
    ASSERT_EQ(GAPIF_RegisterPawrSyncCallback(&cb, &result), BT_SUCCESS);
    PawrSyncCallbackGuard guard;

    ASSERT_EQ(HCI_InjectReceivedEvent(ENHANCED_CONNECTION_COMPLETE_V2_WIRE,
                  sizeof(ENHANCED_CONNECTION_COMPLETE_V2_WIRE)),
        BT_SUCCESS);

    EXPECT_FALSE(result.Wait(DROP_PROBE_TIMEOUT_MS)) << "0x29 failure must not fire any PAwR callback";
    // 无异常状态残留：连接记录未被创建，查询结果与注入前一致。
    EXPECT_EQ(BTM_GetLeConnectionPawrAssociation(0x0042, &advertisingHandle, &syncHandle), BT_BAD_STATUS);
    EXPECT_TRUE(BTM_IsEnabled(LE_CONTROLLER));
}

/**
 * @tc.number: StackGapLe54_HciCmdParamGate_04000
 * @tc.name:  5.4 HCI 发送器 NULL 参数门控
 * @tc.desc:  六个 5.4 发送器（0x007F/0x0082-0x0086）NULL → BT_BAD_PARAM，
 *            命令不下发
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_HciCmdParamGate_04000, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeSetExtendedAdvertisingParametersV2(nullptr), BT_BAD_PARAM);
    EXPECT_EQ(HCI_LeSetPeriodicAdvertisingSubeventData(nullptr), BT_BAD_PARAM);
    EXPECT_EQ(HCI_LeSetPeriodicAdvertisingResponseData(nullptr), BT_BAD_PARAM);
    EXPECT_EQ(HCI_LeSetPeriodicSyncSubevent(nullptr), BT_BAD_PARAM);
    EXPECT_EQ(HCI_LeExtendedCreateConnectionV2(nullptr), BT_BAD_PARAM);
    EXPECT_EQ(HCI_LeSetPeriodicAdvertisingParametersV2(nullptr), BT_BAD_PARAM);
}

/**
 * @tc.number: StackGapLe54_GapifExAdvSetParamV2Gate_04100
 * @tc.name:  A2 GAPIF_LeExAdvSetParamV2 NULL 门控
 * @tc.desc:  NULL 参数 → BT_BAD_PARAM（入口检查，无命令下发）
 */
HWTEST_F(StackGapLe54Test, StackGapLe54_GapifExAdvSetParamV2Gate_04100, TestSize.Level1)
{
    EXPECT_EQ(GAPIF_LeExAdvSetParamV2(nullptr), BT_BAD_PARAM);
}
} // namespace Bluetooth
} // namespace OHOS
