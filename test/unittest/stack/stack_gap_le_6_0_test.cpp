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

// BT 6.0 stack unit tests: LL Extended Feature Set (module 1), Decision-Based
// Advertising Filtering (module DBAF), Monitoring Advertisers (module MA),
// Frame Space Update (module FSU) and ISOAL Unsegmented Framed Mode (module 6).
// Channel Sounding single-board cases have been split into
// stack_gap_le_6_0_cs_test.cpp to keep each source file within the size limit.

#include <gtest/gtest.h>

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <mutex>

#include "hci/evt/hci_evt.h"
#include "hci/hci.h"
#include "hci/hci_def_common.h"
#include "hci/hci_def_le_cmd.h"
#include "hci/hci_def_le_evt.h"
#include "hci/hci_def_le_feature.h"
#include "hci/hci_le_controller_6_0.h"
#include "securec.h"
// Direct-drive entries for the MA Command_Complete and failure tables (5.3 pattern).
#include "src/hci/cmd/hci_cmd_failure_le_controller.h"
#include "src/hci/evt/hci_evt_le_cmd_complete.h"

#include "btm.h"
#include "btstack.h"
#include "cs_le_if.h"
#include "gap_le_if.h"
#include "gap_le_if_5_4.h"
#include "iso_le_if.h"
// Module-6 (ISOAL UFM) iso-layer seam: iso.h self-includes the hci command/event defs.
#include "src/iso/iso.h"
#include "stack_gap_le_6_0_test.h"

using namespace testing::ext;

namespace OHOS {
namespace Bluetooth {
namespace {

// ---- 8.1 compile-time layout and macro-value checks -------------------------
static_assert(sizeof(HciLeFeatures) == 8, "legacy page-0 container stays 8 bytes");
static_assert(sizeof(HciLeFeaturesAll) == 248, "pages 0..10 feature container");
static_assert(sizeof(HciLeReadAllLocalSupportedFeaturesReturnParam) == 250,
    "Status + Max_Page + 248-octet features");
static_assert(sizeof(HciLeReadAllRemoteFeaturesParam) == 3,
    "Connection_Handle + Pages_Requested");
static_assert(sizeof(HciLeReadAllRemoteFeaturesCompleteEventParam) == 253,
    "event payload excludes Subevent_Code");
static_assert(LE_FEATURE_BIT_LL_EXTENDED_FEATURE_SET == 63);
static_assert(LE_FEATURE_BIT_MONITORING_ADVERTISERS == 64);
static_assert(LE_FEATURE_BIT_FRAME_SPACE_UPDATE == 65);
static_assert(LE_MAX_FEATURE_PAGE_NUMBER == 0x0A);
static_assert(LE_EVENT_MASK_LE_READ_ALL_REMOTE_FEATURES_COMPLETE_EVENT ==
    0x0000040000000000ULL);
// Vol 2, Part E, 7.8.1: event-mask bit N selects LE Meta subevent N+1, so
// subevent 0x2B (LE Read All Remote Features Complete) is bit 42.
static_assert((HCI_LE_READ_ALL_REMOTE_FEATURES_COMPLETE_EVENT - 1) == 42);
static_assert(LE_EVENT_MASK_LE_READ_ALL_REMOTE_FEATURES_COMPLETE_EVENT ==
    (1ULL << (HCI_LE_READ_ALL_REMOTE_FEATURES_COMPLETE_EVENT - 1)));

// ---- 8.1 DBAF compile-time layout and macro-value checks -------------------
// Decision-based advertising filtering (Vol 6 Part B Table 4.10) sits at LL
// feature bit 42; the two DBAF commands live on the LE Controller OGF.
static_assert(HCI_LE_SET_DECISION_DATA == MAKE_OPCODE(0x0080, HCI_COMMAND_OGF_LE_CONTROLLER));
static_assert(HCI_LE_SET_DECISION_INSTRUCTIONS == MAKE_OPCODE(0x0081, HCI_COMMAND_OGF_LE_CONTROLLER));
// HciLeSetDecisionDataParam has no wire-fixed size (pointer model, 0x0037-style);
// the 3 + decisionDataLength wire layout is exercised by the 0x0080 sender in 8.4.
static_assert(LE_DECISION_DATA_LEN_MAX == 8);
static_assert(LE_DECISION_RESOLVABLE_TAG_LEN == 6);
static_assert(LE_DECISION_TEST_PARAMS_LEN == 16);
static_assert(sizeof(HciLeDecisionTestParam) == 18);  // 1 + 1 + 16, pack(1)
static_assert(LE_FEATURE_BIT_DECISION_BASED_ADVERTISING_FILTERING == 42);
static_assert(LE_DECISION_TESTS_HOST_MAX == 14);
static_assert(1 + LE_DECISION_TESTS_HOST_MAX * 18 <= 255);  // HCI command length cap
static_assert(LE_SCAN_DECISION_MODE_NONE == 0x00);
static_assert(LE_SCAN_DECISION_MODE_ALL_PDUS == 0x04);
static_assert(LE_SCAN_DECISION_MODE_RFU == 0x08);
static_assert(LE_SCAN_DECISION_MODE_DECISIONS_ONLY == 0x0C);
static_assert(LE_SCAN_DECISION_MODE_MASK == 0x0C);
static_assert(LE_INITIATOR_FILTER_POLICY_MAX == 0x04);
// LE_EXT_ADV_EVENT_PROPERTIES_RESERVED_MASK is defined once, in
// hci_def_le_cmd_6_0.h, and referenced by both 7.8.53 senders; the decision bits
// are exactly bits 7-9 (0x0380) and live wholly below the reserved band.
static_assert(LE_EXT_ADV_EVENT_PROPERTIES_RESERVED_MASK == 0xFC00);
static_assert((LE_EXT_ADV_EVENT_PROP_DECISION_PDU | LE_EXT_ADV_EVENT_PROP_DECISION_ADVA |
    LE_EXT_ADV_EVENT_PROP_DECISION_ADI) == 0x0380);
static_assert(((LE_EXT_ADV_EVENT_PROP_DECISION_PDU | LE_EXT_ADV_EVENT_PROP_DECISION_ADVA |
    LE_EXT_ADV_EVENT_PROP_DECISION_ADI) & LE_EXT_ADV_EVENT_PROPERTIES_RESERVED_MASK) == 0);
// The [v2] GAP entry (GAPIF_LeExAdvSetParamV2) carries the very same 2-octet
// Advertising_Event_Properties field as the [v1] one, so its host type has to
// reach the decision bits - bit 9 sits outside an 8-bit carrier, which would
// leave the [v2] entry unable to express it. Trips if the field is narrowed.
static_assert(sizeof(GapExAdvParamV2::properties) == 2);
static_assert(LE_EXT_ADV_EVENT_PROP_DECISION_ADI < (1U << (sizeof(GapExAdvParamV2::properties) * 8)));
// The widening moved properties off offset 1. This header is parsed at default
// alignment (every #pragma pack region in its include chain closes), so the field
// now starts at its natural offset 2 and txPower follows it at 4 with no gap. The
// first assert is the packing-independent half (it catches a reorder or an inserted
// member under either alignment); the second pins the absolute layout and also trips
// if a pack region is ever left open around this header - a real defect for a by-value
// parameter struct, the packed types being the HCI wire images only.
static_assert(offsetof(GapExAdvParamV2, txPower) ==
    offsetof(GapExAdvParamV2, properties) + sizeof(GapExAdvParamV2::properties));
static_assert(offsetof(GapExAdvParamV2, properties) == 2);

// ---- 8.1 Frame Space Update compile-time layout and macro-value checks -------
// Vol 4, Part E 7.8.151 command / 7.7.65,48 event both carry a fixed 9-byte
// payload; the sizeof asserts are the pack-region tripwire (plan 8.1, rule 2).
static_assert(sizeof(HciLeFrameSpaceUpdateParam) == 9, "command payload stays 9 bytes");
static_assert(sizeof(HciLeFrameSpaceUpdateCompleteEventParam) == 9, "event payload stays 9 bytes");
static_assert(HCI_LE_FRAME_SPACE_UPDATE == MAKE_OPCODE(0x009D, HCI_COMMAND_OGF_LE_CONTROLLER));
static_assert(HCI_LE_FRAME_SPACE_UPDATE_COMPLETE_EVENT == 0x35);
static_assert(LE_FRAME_SPACE_MAX_US == 0x2710);
static_assert(LE_FRAME_SPACE_PHYS_MASK == 0x07);
static_assert(LE_FRAME_SPACE_SPACING_TYPES_MASK == 0x1F);
// Vol 4, Part E, 7.8.1: event-mask bit 52 selects LE Meta subevent 0x35 (7.7.65,48).
static_assert((HCI_LE_FRAME_SPACE_UPDATE_COMPLETE_EVENT - 1) == 52);
static_assert(LE_EVENT_MASK_LE_FRAME_SPACE_UPDATE_COMPLETE_EVENT == 0x0010000000000000ULL);
static_assert(LE_EVENT_MASK_LE_FRAME_SPACE_UPDATE_COMPLETE_EVENT == (1ULL << 52));

// ---- 8.1 ISOAL UFM compile-time layout and macro-value checks ---------------
// Subevent 0x2A LE CIS Established v2 (Vol 4 Part E 7.7.65,25 pp2387-2393): the
// 42-octet payload after Subevent_Code equals the packed struct (pack(1) guard).
static_assert(sizeof(HciLeCisEstablishedV2EventParam) == 42, "v2 CIS Established payload stays 42 bytes");
static_assert(HCI_LE_CIS_ESTABLISHED_V2_EVENT == 0x2A);
// Vol 2 Part E 7.8.1: event-mask bit N selects LE Meta subevent N+1, so
// subevent 0x2A is bit 41.
static_assert((HCI_LE_CIS_ESTABLISHED_V2_EVENT - 1) == 41);
static_assert(LE_EVENT_MASK_LE_CIS_ESTABLISHED_V2_EVENT == 0x0000020000000000ULL);
static_assert(LE_EVENT_MASK_LE_CIS_ESTABLISHED_V2_EVENT == (1ULL << 41));
// The [v1] 0x19 event sits at bit 24; bit 41 must not alias it.
static_assert(LE_EVENT_MASK_LE_CIS_ESTABLISHED_EVENT == 0x0000000001000000ULL);
static_assert((LE_EVENT_MASK_LE_CIS_ESTABLISHED_V2_EVENT & LE_EVENT_MASK_LE_CIS_ESTABLISHED_EVENT) == 0);
static_assert(LE_EVENT_MASK_LE_CIS_ESTABLISHED_V2_EVENT != LE_EVENT_MASK_LE_CIS_ESTABLISHED_EVENT);
// Unsegmented Framed Mode is a page-0 LL feature (Vol 6 Part B Table 4.10).
static_assert(LE_FEATURE_BIT_UNSEGMENTED_FRAMED_MODE == 45);

// ---- 8.1 Channel Sounding compile-time layout and macro-value checks --------
// Channel Sounding commands (Vol 4 Part E 7.8.130-133, 7.8.142-143) and their
// subevents (7.7.65,39-46): the 13 bodies below are read and written through
// pack(1) structs, so sizeof is the wire length (plan 8.1). Sizes recomputed
// from the wire order of Vol 4 Part E pp2798-2855.
static_assert(sizeof(HciLeCsCapabilities) == 28, "CS capability body 28 B");
static_assert(sizeof(HciLeCsReadLocalSupportedCapabilitiesReturnParam) == 29, "status+28");
static_assert(sizeof(HciLeCsWriteCachedRemoteSupportedCapabilitiesParam) == 30, "handle+28");
static_assert(sizeof(HciLeCsReadRemoteSupportedCapabilitiesCompleteEventParam) == 31,
    "status+handle+28");
static_assert(sizeof(HciLeCsReadRemoteFaeTableCompleteEventParam) == 75, "status+handle+72");
static_assert(sizeof(HciLeCsConfigCompleteEventParam) == 33, "0x2F body");
static_assert(sizeof(HciLeCsSetChannelClassificationParam) == 10, "0x0092 param");
static_assert(sizeof(HciLeCsSetProcedureParametersParam) == 23, "0x0093 param");
static_assert(sizeof(HciLeCsSetProcedureParametersReturnParam) == 3,
    "status+handle, no Config_ID");
static_assert(sizeof(HciLeCsProcedureEnableParam) == 4, "0x0094 param");
static_assert(sizeof(HciLeCsProcedureEnableCompleteEventParam) == 21, "0x30 body");
static_assert(sizeof(HciLeCsSubeventResultEventParam) == 15, "0x31 fixed head");
static_assert(sizeof(HciLeCsSubeventResultContinueEventParam) == 8, "0x32 fixed head");
static_assert(sizeof(HciLeCsTestPrefix) == 30, "0x0095 fixed prefix");
// Channel Sounding is LL feature bit 46; host support is the separate bit 47
// (Vol 6 Part B Table 4.10). Vol 2 Part E 7.8.1: mask bit N selects LE Meta
// subevent N+1, so subevents 0x2C..0x33 pair with bits 43..50 in order.
static_assert(LE_FEATURE_BIT_CHANNEL_SOUNDING == 46);
static_assert(LE_FEATURE_BIT_CHANNEL_SOUNDING_HOST_SUPPORT == 47);
static_assert((HCI_LE_CS_READ_REMOTE_SUPPORTED_CAPABILITIES_COMPLETE_EVENT - 1) == 43);
static_assert((HCI_LE_CS_READ_REMOTE_FAE_TABLE_COMPLETE_EVENT - 1) == 44);
static_assert((HCI_LE_CS_SECURITY_ENABLE_COMPLETE_EVENT - 1) == 45);
static_assert((HCI_LE_CS_CONFIG_COMPLETE_EVENT - 1) == 46);
static_assert((HCI_LE_CS_PROCEDURE_ENABLE_COMPLETE_EVENT - 1) == 47);
static_assert((HCI_LE_CS_SUBEVENT_RESULT_EVENT - 1) == 48);
static_assert((HCI_LE_CS_SUBEVENT_RESULT_CONTINUE_EVENT - 1) == 49);
static_assert((HCI_LE_CS_TEST_END_COMPLETE_EVENT - 1) == 50);
static_assert(LE_EVENT_MASK_LE_CS_READ_REMOTE_SUPPORTED_CAPABILITIES_COMPLETE_EVENT == (1ULL << 43));
static_assert(LE_EVENT_MASK_LE_CS_READ_REMOTE_FAE_TABLE_COMPLETE_EVENT == (1ULL << 44));
static_assert(LE_EVENT_MASK_LE_CS_SECURITY_ENABLE_COMPLETE_EVENT == (1ULL << 45));
static_assert(LE_EVENT_MASK_LE_CS_CONFIG_COMPLETE_EVENT == (1ULL << 46));
static_assert(LE_EVENT_MASK_LE_CS_PROCEDURE_ENABLE_COMPLETE_EVENT == (1ULL << 47));
static_assert(LE_EVENT_MASK_LE_CS_SUBEVENT_RESULT_EVENT == (1ULL << 48));
static_assert(LE_EVENT_MASK_LE_CS_SUBEVENT_RESULT_CONTINUE_EVENT == (1ULL << 49));
static_assert(LE_EVENT_MASK_LE_CS_TEST_END_COMPLETE_EVENT == (1ULL << 50));

// The public CS_LE_* macros are a second copy of values the HCI layer enforces
// with its own LE_CS_* macros, and nothing else ties the two together. Pin the
// pairs here so a caller sizing from the public header cannot drift from what
// HCI_LeCs* accepts: a mismatch would show as BT_BAD_PARAM with no visible cause.
static_assert(CS_LE_CONFIG_ID_MAX == LE_CS_CONFIG_ID_MAX);
static_assert(CS_LE_CHANNEL_MAP_LEN == LE_CS_CHANNEL_MAP_LEN);
static_assert(CS_LE_REMOTE_FAE_TABLE_LEN == LE_CS_REMOTE_FAE_TABLE_LEN);
static_assert(CS_LE_SUB_MODE_UNUSED == LE_CS_SUB_MODE_UNUSED);
static_assert(CS_LE_TEST_PREFIX_LEN == LE_CS_TEST_PREFIX_LEN);
static_assert(CS_LE_TEST_OVERRIDE_MIN == LE_CS_TEST_OVERRIDE_MIN);
static_assert(CS_LE_TEST_OVERRIDE_MAX == LE_CS_TEST_OVERRIDE_MAX);
static_assert(CS_LE_TEST_OVERRIDE_CONFIG_MASK == LE_CS_TEST_OVERRIDE_CONFIG_MASK);

// LE Meta event wire prefix: Event_Code + Parameter_Total_Length + Subevent_Code.
constexpr size_t LE_META_EVT_HEAD_LEN = 3;
constexpr size_t LE_META_EVT_PREFIX_LEN = 2;
constexpr size_t LE_META_EVT_EVENT_CODE_OFFSET = 0;
constexpr size_t LE_META_EVT_LENGTH_OFFSET = 1;
constexpr size_t LE_META_EVT_SUBEVENT_CODE_OFFSET = 2;

// Subevent 0x2B LE Read All Remote Features Complete (7.7.65,38) payload after
// the Subevent_Code byte: 253 octets, of which octets 5..252 are LE_Features.
constexpr size_t ALL_REMOTE_FEATURES_EVT_PAYLOAD = 253;

// Fixture values for the 0x2B LE Read All Remote Features Complete case.
constexpr uint16_t ALL_REMOTE_FEATURES_TEST_HANDLE = 0x0008;
constexpr uint8_t ALL_REMOTE_FEATURES_TEST_MAX_REMOTE_PAGE = 0x02;
constexpr uint8_t ALL_REMOTE_FEATURES_TEST_MAX_VALID_PAGE = 0x02;

// Condition field of the 0x34 LE Monitored Advertisers Report event (7.7.65,47 p2464).
constexpr uint8_t MA_CONDITION_BELOW_LOW_TIMEOUT = 0x00;
constexpr uint8_t MA_CONDITION_RSSI_ABOVE_HIGH = 0x01;

// Fixture values for the 0x35 LE Frame Space Update Complete case.
constexpr uint16_t FSU_TEST_HANDLE = 0x0042;
constexpr uint16_t FSU_TEST_FRAME_SPACE_US = 0x0064;   // 100 us
constexpr uint8_t FSU_TEST_PHYS = 0x07;                // LE 1M | LE 2M | LE Coded
constexpr uint16_t FSU_TEST_SPACING_TYPES = 0x001F;
constexpr uint8_t FSU_INITIATOR_MAX = 0x02;            // 0x00-0x02 inclusive

// ---- wire -> HciOnEvent capture callbacks -------------------------------------
// g_capture, g_v2ParseCapture and g_bigInfoParseCapture are declared in
// stack_gap_le_6_0_test.h so the split CS test file can read them.

static void OnAllRemoteFeaturesComplete(const HciLeReadAllRemoteFeaturesCompleteEventParam *eventParam)
{
    g_capture.param = *eventParam;
    g_capture.received = true;
}

static void OnLeCisEstablishedV2Parse(const HciLeCisEstablishedV2EventParam *eventParam)
{
    g_v2ParseCapture.param = *eventParam;
    g_v2ParseCapture.received = true;
}

static void OnLeBigInfoParse(const HciLeBigInfoAdvertisingReportEventParam *eventParam)
{
    g_bigInfoParseCapture.param = *eventParam;
    g_bigInfoParseCapture.received = true;
}

// The ISO/CIG/BIG helpers live in stack_gap_le_6_0_test.h so that the split-out
// CS test source can also use them. OnCisEstablishedIso is defined here (not
// inline in the header) to satisfy G.FUD.06.
}  // namespace

void OnCisEstablishedIso(uint8_t status, const IsoLeCisEstablishedInfo *info, void *context)
{
    auto *ctx = static_cast<IsoCisCtx *>(context);
    ctx->establishedCount++;
    ctx->estStatus = status;
    ctx->estHandle = (info != nullptr) ? info->cisHandle : 0xFFFF;
    if (info != nullptr && info->v2Ext != nullptr) {
        ctx->hasV2Ext = true;
        ctx->v2Ext = *info->v2Ext;
    } else {
        ctx->hasV2Ext = false;
    }
}

void InitParseCallbacks()
{
    g_parseCallbacks = {};
    g_parseCallbacks.leReadAllRemoteFeaturesComplete = OnAllRemoteFeaturesComplete;
    g_parseCallbacks.leCisEstablishedV2 = OnLeCisEstablishedV2Parse;
    g_parseCallbacks.leBigInfoAdvertisingReport = OnLeBigInfoParse;
}

// Wrap hand-built wire bytes into an HCI event packet and feed it to the
// synchronous parser HciOnEvent (same layout as HciOnReceivedHciPacket).
void InjectWire(const uint8_t *wire, size_t size)
{
    Packet *packet = PacketMalloc(0, 0, size);
    ASSERT_NE(packet, nullptr);
    PacketPayloadWrite(packet, wire, 0, size);
    HciOnEvent(packet);
    PacketFree(packet);
}

// Build and inject a complete 0x2A LE CIS Established v2 event from a filled
// param struct (Vol 4 Part E 5.4.4 + 7.7.65 LE Meta envelope).
static void InjectCisEstablishedV2Event(const HciLeCisEstablishedV2EventParam &p)
{
    uint8_t wire[LE_META_EVT_HEAD_LEN + sizeof(p)];
    wire[LE_META_EVT_EVENT_CODE_OFFSET] = HCI_LE_META_EVENT;  // Event_Code: LE Meta
    wire[LE_META_EVT_LENGTH_OFFSET] = static_cast<uint8_t>(1 + sizeof(p));  // Subevent_Code + body
    wire[LE_META_EVT_SUBEVENT_CODE_OFFSET] = HCI_LE_CIS_ESTABLISHED_V2_EVENT;  // Subevent 0x2A
    if (memcpy_s(wire + LE_META_EVT_HEAD_LEN, sizeof(p), &p, sizeof(p)) != EOK) {
        ADD_FAILURE() << "InjectCisEstablishedV2Event memcpy_s failed";
        return;
    }
    InjectWire(wire, sizeof(wire));
}

void StackGapLe60Test::SetUpTestCase(void)
{
    HciInitEvent();
    InitParseCallbacks();
    // Guard only: always returns BT_SUCCESS (hci_evt.c:2044) - no verification power.
    ASSERT_EQ(HCI_RegisterEventCallbacks(&g_parseCallbacks), BT_SUCCESS);
}

void StackGapLe60Test::TearDownTestCase(void)
{
    // Guard only: always returns BT_SUCCESS (hci_evt.c:2054) - no verification power.
    EXPECT_EQ(HCI_DeregisterEventCallbacks(&g_parseCallbacks), BT_SUCCESS);
    HciCloseEvent();
}

void StackGapLe60Test::SetUp()
{
    g_capture.Reset();
    g_v2ParseCapture.Reset();
    g_bigInfoParseCapture.Reset();
}

void StackGapLe60Test::TearDown() {}

// @tc.number: StackGapLe6_0_StaticLayout_00100
// @tc.name:  8.1 layout and macro values echo
// @tc.desc:  Runtime echo of the compile-time static_asserts for diagnosability.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_StaticLayout_00100, TestSize.Level1)
{
    EXPECT_EQ(sizeof(HciLeFeatures), LE_FEATURES_PAGE0_SIZE);
    EXPECT_EQ(sizeof(HciLeFeaturesAll), LE_FEATURES_SIZE_ALL);
    EXPECT_EQ(sizeof(HciLeReadAllLocalSupportedFeaturesReturnParam), 250u);
    EXPECT_EQ(sizeof(HciLeReadAllRemoteFeaturesParam), 3u);
    EXPECT_EQ(sizeof(HciLeReadAllRemoteFeaturesCompleteEventParam), 253u);
    EXPECT_EQ(LE_FEATURE_BIT_LL_EXTENDED_FEATURE_SET, 63);
    EXPECT_EQ(LE_FEATURE_BIT_MONITORING_ADVERTISERS, 64);
    EXPECT_EQ(LE_FEATURE_BIT_FRAME_SPACE_UPDATE, 65);
    EXPECT_EQ(LE_MAX_FEATURE_PAGE_NUMBER, 0x0A);
    EXPECT_EQ(LE_EVENT_MASK_LE_READ_ALL_REMOTE_FEATURES_COMPLETE_EVENT,
        0x0000040000000000ULL);
}

// @tc.number: StackGapLe6_0_Page0Bit63_00200
// @tc.name:  page-0 bit 63 mapping on the 8-byte and 248-byte containers
// @tc.desc:  bit 63 = octet 7 bit 7; neighbours and octet boundary stay exact.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_Page0Bit63_00200, TestSize.Level1)
{
    // 8-byte page-0 (0x0003 result) container.
    HciLeFeatures page0 = {};
    page0.raw[7] = 0x80;                 // bit 63
    EXPECT_TRUE(GetLinkLayerFeatureFlag(page0.raw, 63));
    EXPECT_TRUE(HciSupportLeExtendedFeatureSet(&page0));

    // Bit 62 sits one bit lower in the same octet and must not alias bit 63.
    page0.raw[7] = 0x40;                 // bit 62
    EXPECT_FALSE(GetLinkLayerFeatureFlag(page0.raw, 63));
    EXPECT_FALSE(HciSupportLeExtendedFeatureSet(&page0));

    // Same bit read through the 248-byte container prefix.
    HciLeFeaturesAll all = {};
    all.raw[7] = 0x80;                   // page-0 octet 7 of the 248-byte container
    EXPECT_TRUE(GetLinkLayerFeatureFlag(all.raw, LE_FEATURE_BIT_LL_EXTENDED_FEATURE_SET));

    // Octet-boundary indexing inside a byte array: bit 7 = octet 0 bit 7,
    // bit 8 = octet 1 bit 0 (GetLinkLayerFeatureFlag indexes byte bitIndex/8).
    HciLeFeatures cross = {};
    cross.raw[0] = 0x80;
    EXPECT_TRUE(GetLinkLayerFeatureFlag(cross.raw, 7));
    EXPECT_FALSE(GetLinkLayerFeatureFlag(cross.raw, 8));
    cross.raw[0] = 0;
    cross.raw[1] = 0x01;
    EXPECT_TRUE(GetLinkLayerFeatureFlag(cross.raw, 8));
    EXPECT_FALSE(GetLinkLayerFeatureFlag(cross.raw, 7));
}

// @tc.number: StackGapLe6_0_Page1Bit64Bit65_00300
// @tc.name:  page-1 bits 64/65 mapping on the 248-byte container
// @tc.desc:  bit 64 = octet 8 bit 0, bit 65 = octet 8 bit 1, single-bit exact.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_Page1Bit64Bit65_00300, TestSize.Level1)
{
    HciLeReadAllLocalSupportedFeaturesReturnParam local = {};  // 0x0087 shape
    local.leFeatures.raw[8] = 0x03;      // bit 64 + bit 65
    EXPECT_TRUE(GetLinkLayerFeatureFlag(local.leFeatures.raw, 64));
    EXPECT_TRUE(GetLinkLayerFeatureFlag(local.leFeatures.raw, 65));
    EXPECT_TRUE(HciSupportLeMonitoringAdvertisers(&local.leFeatures));
    EXPECT_TRUE(HciSupportLeFrameSpaceUpdate(&local.leFeatures));

    local.leFeatures.raw[8] = 0x01;      // bit 64 only
    EXPECT_TRUE(HciSupportLeMonitoringAdvertisers(&local.leFeatures));
    EXPECT_FALSE(HciSupportLeFrameSpaceUpdate(&local.leFeatures));

    local.leFeatures.raw[8] = 0x02;      // bit 65 only
    EXPECT_FALSE(HciSupportLeMonitoringAdvertisers(&local.leFeatures));
    EXPECT_TRUE(HciSupportLeFrameSpaceUpdate(&local.leFeatures));

    local.leFeatures.raw[8] = 0x04;      // bit 66: neither helper
    EXPECT_FALSE(HciSupportLeMonitoringAdvertisers(&local.leFeatures));
    EXPECT_FALSE(HciSupportLeFrameSpaceUpdate(&local.leFeatures));
}

// @tc.number: StackGapLe6_0_HelperIndependence_00400
// @tc.name:  three 6.0 helpers are mutually independent on a zeroed/mixed 248B
// @tc.desc:  bit 63 gates only ExtendedFeatureSet; bits 64/65 the others.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_HelperIndependence_00400, TestSize.Level1)
{
    HciLeFeaturesAll all = {};
    EXPECT_FALSE(GetLinkLayerFeatureFlag(all.raw, LE_FEATURE_BIT_LL_EXTENDED_FEATURE_SET));
    EXPECT_FALSE(HciSupportLeMonitoringAdvertisers(&all));
    EXPECT_FALSE(HciSupportLeFrameSpaceUpdate(&all));

    all.raw[7] = 0x80;                   // page 0, bit 63
    EXPECT_TRUE(GetLinkLayerFeatureFlag(all.raw, LE_FEATURE_BIT_LL_EXTENDED_FEATURE_SET));
    EXPECT_FALSE(HciSupportLeMonitoringAdvertisers(&all));
    EXPECT_FALSE(HciSupportLeFrameSpaceUpdate(&all));

    all.raw[7] = 0;
    all.raw[8] = 0x03;                   // page 1, bit 64 + bit 65
    EXPECT_FALSE(GetLinkLayerFeatureFlag(all.raw, LE_FEATURE_BIT_LL_EXTENDED_FEATURE_SET));
    EXPECT_TRUE(HciSupportLeMonitoringAdvertisers(&all));
    EXPECT_TRUE(HciSupportLeFrameSpaceUpdate(&all));
}

// @tc.number: StackGapLe6_0_EventMaskBit42_00500
// @tc.name:  bit-42 event mask and subevent 0x2B self consistency
// @tc.desc:  mask == 1 << (0x2B - 1); the 5.x masks keep the same convention.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_EventMaskBit42_00500, TestSize.Level1)
{
    EXPECT_EQ((HCI_LE_READ_ALL_REMOTE_FEATURES_COMPLETE_EVENT - 1), 42);
    EXPECT_EQ(LE_EVENT_MASK_LE_READ_ALL_REMOTE_FEATURES_COMPLETE_EVENT,
        static_cast<uint64_t>(1) << (HCI_LE_READ_ALL_REMOTE_FEATURES_COMPLETE_EVENT - 1));
    EXPECT_EQ(LE_EVENT_MASK_LE_READ_ALL_REMOTE_FEATURES_COMPLETE_EVENT,
        0x0000040000000000ULL);
    // Existing 5.x masks: LE Connection Complete (subevent 0x01) sits at bit 0.
    EXPECT_EQ(LE_EVENT_MASK_LE_CONNECTION_COMPLETE_EVENT, 0x0000000000000001ULL);
}

// @tc.number: StackGapLe6_0_InitRoutingGate_00600
// @tc.name:  0x0087 init gate predicate and page-1 cache baseline semantics
// @tc.desc:  BtmInitLeFeature sends 0x0087 iff bit 63 of the legacy 8-byte
//            0x0003 result; when clear, the zeroed 248-byte cache keeps page-1
//            helpers false (pre-BT6.0 baseline).
HWTEST_F(StackGapLe60Test, StackGapLe6_0_InitRoutingGate_00600, TestSize.Level1)
{
    HciLeReadLocalSupportedFeaturesReturnParam gateClear = {};  // 0x0003, bit 63 clear
    HciLeReadLocalSupportedFeaturesReturnParam gateSet = {};
    gateSet.leFeatures.raw[7] = 0x80;    // 0x0003, bit 63 set

    EXPECT_FALSE(HciSupportLeExtendedFeatureSet(&gateClear.leFeatures));
    EXPECT_TRUE(HciSupportLeExtendedFeatureSet(&gateSet.leFeatures));

    // bit 63 clear => init skips 0x0087, the 248-byte cache stays zeroed and
    // the page-1 getters read false exactly like the pre-BT6.0 baseline.
    HciLeReadAllLocalSupportedFeaturesReturnParam zeroCache = {};
    EXPECT_FALSE(HciSupportLeMonitoringAdvertisers(&zeroCache.leFeatures));
    EXPECT_FALSE(HciSupportLeFrameSpaceUpdate(&zeroCache.leFeatures));

    // bit 63 set => 0x0087 fills the 248-byte cache; page-0 and page-1 bits
    // read from the same container are honoured independently.
    HciLeReadAllLocalSupportedFeaturesReturnParam filledCache = {};
    filledCache.leFeatures.raw[7] = 0x80;    // bit 63 (page 0)
    filledCache.leFeatures.raw[8] = 0x01;    // bit 64 (page 1)
    EXPECT_TRUE(GetLinkLayerFeatureFlag(filledCache.leFeatures.raw, LE_FEATURE_BIT_LL_EXTENDED_FEATURE_SET));
    EXPECT_TRUE(HciSupportLeMonitoringAdvertisers(&filledCache.leFeatures));
    EXPECT_FALSE(HciSupportLeFrameSpaceUpdate(&filledCache.leFeatures));
}

// @tc.number: StackGapLe6_0_AllRemoteFeaturesParse_00700
// @tc.name:  0x2B LE Read All Remote Features Complete full-field parse
// @tc.desc:  Inject the 253-octet subevent payload; assert each field and that
//            the 248-octet features read through the 6.0 page helpers.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_AllRemoteFeaturesParse_00700, TestSize.Level1)
{
    HciLeReadAllRemoteFeaturesCompleteEventParam featuresParam = {};
    featuresParam.status = 0x00;
    featuresParam.connectionHandle = ALL_REMOTE_FEATURES_TEST_HANDLE;
    featuresParam.maxRemotePage = ALL_REMOTE_FEATURES_TEST_MAX_REMOTE_PAGE;
    featuresParam.maxValidPage = ALL_REMOTE_FEATURES_TEST_MAX_VALID_PAGE;
    featuresParam.leFeatures.raw[7] = 0x80;  // page-0 bit 63
    featuresParam.leFeatures.raw[8] = 0x03;  // page-1 bits 64+65

    uint8_t wire[LE_META_EVT_HEAD_LEN + ALL_REMOTE_FEATURES_EVT_PAYLOAD];
    // Event_Code: LE Meta
    wire[LE_META_EVT_EVENT_CODE_OFFSET] = HCI_LE_META_EVENT;
    wire[LE_META_EVT_LENGTH_OFFSET] = static_cast<uint8_t>((LE_META_EVT_HEAD_LEN - LE_META_EVT_PREFIX_LEN) +
        ALL_REMOTE_FEATURES_EVT_PAYLOAD);  // Parameter_Total_Length
    // Subevent 0x2B
    wire[LE_META_EVT_SUBEVENT_CODE_OFFSET] = HCI_LE_READ_ALL_REMOTE_FEATURES_COMPLETE_EVENT;
    ASSERT_EQ(memcpy_s(wire + LE_META_EVT_HEAD_LEN, sizeof(wire) - LE_META_EVT_HEAD_LEN,
        &featuresParam, sizeof(featuresParam)), EOK);
    InjectWire(wire, sizeof(wire));

    EXPECT_TRUE(g_capture.received);
    EXPECT_EQ(g_capture.param.status, 0x00);
    EXPECT_EQ(g_capture.param.connectionHandle, 0x0008);
    EXPECT_EQ(g_capture.param.maxRemotePage, 0x02);
    EXPECT_EQ(g_capture.param.maxValidPage, 0x02);
    EXPECT_TRUE(GetLinkLayerFeatureFlag(g_capture.param.leFeatures.raw, LE_FEATURE_BIT_LL_EXTENDED_FEATURE_SET));
    EXPECT_TRUE(HciSupportLeMonitoringAdvertisers(&g_capture.param.leFeatures));
    EXPECT_TRUE(HciSupportLeFrameSpaceUpdate(&g_capture.param.leFeatures));
}

// @tc.number: StackGapLe6_0_AllRemoteFeaturesTruncatedDrop_00800
// @tc.name:  truncated 0x2B payload is silently dropped
// @tc.desc:  handler requires the full 253 octets after the Subevent_Code.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_AllRemoteFeaturesTruncatedDrop_00800, TestSize.Level1)
{
    const uint8_t truncated[] = {
        0x3E, 0x04, HCI_LE_READ_ALL_REMOTE_FEATURES_COMPLETE_EVENT,
        0x00, 0x08, 0x00,  // only 3 octets of the 253 required
    };
    InjectWire(truncated, sizeof(truncated));
    EXPECT_FALSE(g_capture.received);
}

// @tc.number: StackGapLe6_0_DbafConstants_00900
// @tc.name:  8.1 DBAF macro/layout values echo
// @tc.desc:  Runtime echo of the DBAF static_asserts for diagnosability.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_DbafConstants_00900, TestSize.Level1)
{
    EXPECT_EQ(HCI_LE_SET_DECISION_DATA, MAKE_OPCODE(0x0080, HCI_COMMAND_OGF_LE_CONTROLLER));
    EXPECT_EQ(HCI_LE_SET_DECISION_INSTRUCTIONS, MAKE_OPCODE(0x0081, HCI_COMMAND_OGF_LE_CONTROLLER));
    EXPECT_EQ(LE_DECISION_DATA_LEN_MAX, 8);
    EXPECT_EQ(LE_DECISION_RESOLVABLE_TAG_LEN, 6);
    EXPECT_EQ(LE_DECISION_TEST_PARAMS_LEN, 16);
    EXPECT_EQ(sizeof(HciLeDecisionTestParam), 18u);
    EXPECT_EQ(LE_FEATURE_BIT_DECISION_BASED_ADVERTISING_FILTERING, 42);
    EXPECT_EQ(LE_DECISION_TESTS_HOST_MAX, 14);
    EXPECT_EQ(LE_SCAN_DECISION_MODE_NONE, 0x00);
    EXPECT_EQ(LE_SCAN_DECISION_MODE_ALL_PDUS, 0x04);
    EXPECT_EQ(LE_SCAN_DECISION_MODE_RFU, 0x08);
    EXPECT_EQ(LE_SCAN_DECISION_MODE_DECISIONS_ONLY, 0x0C);
    EXPECT_EQ(LE_SCAN_DECISION_MODE_MASK, 0x0C);
    EXPECT_EQ(LE_INITIATOR_FILTER_POLICY_MAX, 0x04);
    EXPECT_EQ(LE_EXT_ADV_EVENT_PROP_DECISION_PDU, 0x80);
    EXPECT_EQ(LE_EXT_ADV_EVENT_PROP_DECISION_ADVA, 0x100);
    EXPECT_EQ(LE_EXT_ADV_EVENT_PROP_DECISION_ADI, 0x200);
}

// @tc.number: StackGapLe6_0_DbafBit42_01000
// @tc.name:  page-0 bit 42 (DBAF) mapping on the 8-byte and 248-byte containers
// @tc.desc:  bit 42 = octet 5 bit 2; neighbours bits 41/43 stay clear.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_DbafBit42_01000, TestSize.Level1)
{
    // 8-byte page-0 (0x0003 result) container.
    HciLeFeatures page0 = {};
    page0.raw[5] = 0x04;                 // bit 42
    EXPECT_TRUE(GetLinkLayerFeatureFlag(page0.raw, 42));
    EXPECT_TRUE(HciSupportLeDecisionBasedAdvertisingFiltering(page0.raw));

    // Bits 41 (0x02) and 43 (0x08) in the same octet must not alias bit 42.
    page0.raw[5] = 0x02;                 // bit 41
    EXPECT_FALSE(GetLinkLayerFeatureFlag(page0.raw, 42));
    EXPECT_FALSE(HciSupportLeDecisionBasedAdvertisingFiltering(page0.raw));
    page0.raw[5] = 0x08;                 // bit 43
    EXPECT_FALSE(GetLinkLayerFeatureFlag(page0.raw, 42));
    EXPECT_FALSE(HciSupportLeDecisionBasedAdvertisingFiltering(page0.raw));

    // Same bit read through the 248-byte container prefix.
    HciLeFeaturesAll all = {};
    all.raw[5] = 0x04;
    EXPECT_TRUE(HciSupportLeDecisionBasedAdvertisingFiltering(all.raw));
    all.raw[5] = 0x00;
    EXPECT_FALSE(HciSupportLeDecisionBasedAdvertisingFiltering(all.raw));
}

// @tc.number: StackGapLe6_0_DecisionDataInvalid_01100
// @tc.name:  0x0080 LE Set Decision Data invalid-param pre-checks
// @tc.desc:  Each rejection returns BT_BAD_PARAM before any HCI command is built.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_DecisionDataInvalid_01100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeSetDecisionData(nullptr), BT_BAD_PARAM);

    uint8_t data[LE_DECISION_DATA_LEN_MAX] = {0};
    HciLeSetDecisionDataParam param = {};

    // Advertising_Handle out of range (max 0xEF).
    param = {};
    param.advertisingHandle = 0xF0;
    EXPECT_EQ(HCI_LeSetDecisionData(&param), BT_BAD_PARAM);

    // Decision_Data_Length above the 8-octet cap.
    param = {};
    param.advertisingHandle = 0x01;
    param.decisionDataLength = LE_DECISION_DATA_LEN_MAX + 1;
    param.decisionData = data;
    EXPECT_EQ(HCI_LeSetDecisionData(&param), BT_BAD_PARAM);

    // Decision_Type_Flags has an RFU bit above bit 0.
    param = {};
    param.advertisingHandle = 0x01;
    param.decisionTypeFlags = 0x02;
    EXPECT_EQ(HCI_LeSetDecisionData(&param), BT_BAD_PARAM);

    // Non-zero length with a NULL data pointer.
    param = {};
    param.advertisingHandle = 0x01;
    param.decisionDataLength = 1;
    param.decisionData = nullptr;
    EXPECT_EQ(HCI_LeSetDecisionData(&param), BT_BAD_PARAM);
}

// @tc.number: StackGapLe6_0_DecisionInstructionsInvalid_01200
// @tc.name:  0x0081 LE Set Decision Instructions invalid-param pre-checks
// @tc.desc:  Each rejection returns BT_BAD_PARAM before any HCI command is built.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_DecisionInstructionsInvalid_01200, TestSize.Level1)
{
    EXPECT_EQ(HCI_LeSetDecisionInstructions(nullptr), BT_BAD_PARAM);

    HciLeDecisionTestParam decisionTest = {};
    HciLeSetDecisionInstructionsParam param = {};

    // Num_Tests of zero is rejected.
    param = {};
    param.numTests = 0;
    param.tests = &decisionTest;
    EXPECT_EQ(HCI_LeSetDecisionInstructions(&param), BT_BAD_PARAM);

    // Num_Tests above the host structural cap (14) is rejected here.
    param = {};
    param.numTests = LE_DECISION_TESTS_HOST_MAX + 1;
    param.tests = &decisionTest;
    EXPECT_EQ(HCI_LeSetDecisionInstructions(&param), BT_BAD_PARAM);

    // A NULL tests array is rejected.
    param = {};
    param.numTests = 1;
    param.tests = nullptr;
    EXPECT_EQ(HCI_LeSetDecisionInstructions(&param), BT_BAD_PARAM);

    // Test_Flags[0] bit 0 (new-group marker) must be set (7.8.145).
    decisionTest = {};
    param = {};
    param.numTests = 1;
    param.tests = &decisionTest;   // decisionTest.flags == 0x00, bit 0 clear
    EXPECT_EQ(HCI_LeSetDecisionInstructions(&param), BT_BAD_PARAM);
}

// @tc.number: StackGapLe6_0_ScanDecisionModeGate_01300
// @tc.name:  0x0041 scanning filter-policy RFU/bit4-7 bytes are rejected
// @tc.desc:  D-5 allow set is {0x00..0x0F} minus {0x08..0x0B}; the reject runs
//            in the sender pre-check, so it is safe on the single-board harness.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_ScanDecisionModeGate_01300, TestSize.Level1)
{
    HciLeExtendedScanParametersSet sets[1] = {};
    HciLeSetExtendedScanParametersParam param = {};
    param.ownAddressType = 0x00;
    param.scanningPhYs = LE_1M_PHY;
    param.sets = sets;

    // 0x08..0x0B: decision-mode subfield 0b10 is RFU; 0x10/0xF0: bits 4-7 RFU.
    const uint8_t rejected[] = {0x08, 0x09, 0x0A, 0x0B, 0x10, 0xF0};
    for (uint8_t policy : rejected) {
        param.scanningFilterPolicy = policy;
        EXPECT_EQ(HCI_LeSetExtendedScanParameters(&param), BT_BAD_PARAM);
    }
}

// @tc.number: StackGapLe6_0_InitiatorFilterPolicyGate_01400
// @tc.name:  0x0043 initiator filter-policy above 0x04 is rejected
// @tc.desc:  D-5 range is 0x00-0x04; the reject runs in the sender pre-check,
//            so it is safe on the single-board harness.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_InitiatorFilterPolicyGate_01400, TestSize.Level1)
{
    HciLeConnectionParamSet connSets[1] = {};
    HciLeExtendedCreateConnectionParam param = {};
    param.initiatingPhys = LE_1M_PHY;
    param.sets = connSets;

    const uint8_t rejected[] = {0x05, 0x06, 0xFF};
    for (uint8_t policy : rejected) {
        param.initiatingFilterPolicy = policy;
        EXPECT_EQ(HCI_LeExtendedCreateConnection(&param), BT_BAD_PARAM);
    }
}
// ---- MA (BT 6.0 Monitoring Advertisers) unit cases, plan 8.1 / 8.2(1)(2) -----
// Same dispatch-only contract as the DBAF cases above: the MA HCI callback members
// are bound into the shared parse set for the duration of each case only, so this
// fixture still never boots GAP/BTM. 8.2(1) is the 0x34 wire parse plus the five
// 0x0098-0x009C Command_Complete dispatches; 8.2(2) is the controller-failure path.

static_assert(HCI_LE_ADD_DEVICE_TO_MONITORED_ADVERTISERS_LIST ==
    MAKE_OPCODE(0x0098, HCI_COMMAND_OGF_LE_CONTROLLER));
static_assert(HCI_LE_REMOVE_DEVICE_FROM_MONITORED_ADVERTISERS_LIST ==
    MAKE_OPCODE(0x0099, HCI_COMMAND_OGF_LE_CONTROLLER));
static_assert(HCI_LE_CLEAR_MONITORED_ADVERTISERS_LIST ==
    MAKE_OPCODE(0x009A, HCI_COMMAND_OGF_LE_CONTROLLER));
static_assert(HCI_LE_READ_MONITORED_ADVERTISERS_LIST_SIZE ==
    MAKE_OPCODE(0x009B, HCI_COMMAND_OGF_LE_CONTROLLER));
static_assert(HCI_LE_ENABLE_MONITORING_ADVERTISERS ==
    MAKE_OPCODE(0x009C, HCI_COMMAND_OGF_LE_CONTROLLER));
static_assert(sizeof(HciLeAddDeviceToMonitoredAdvertisersListParam) == 10);
static_assert(sizeof(HciLeRemoveDeviceFromMonitoredAdvertisersListParam) == 7);
static_assert(sizeof(HciLeEnableMonitoringAdvertisersParam) == 1);
static_assert(sizeof(HciLeReadMonitoredAdvertisersListSizeReturnParam) == 2);
static_assert(sizeof(HciLeMonitoredAdvertisersReportEventParam) == 8);
// Vol 4, Part E, 7.7.65,47 (pp.2464-2465): subevent 0x34 maps to mask bit 51.
static_assert(LE_EVENT_MASK_LE_MONITORED_ADVERTISERS_REPORT_EVENT == 0x0008000000000000ULL);
static_assert(HCI_LE_MONITORED_ADVERTISERS_REPORT_EVENT - 1 == 51);

// 0x34 payload after the Subevent_Code: Address_Type(1) + Address(6) + Condition(1).
constexpr size_t MA_REPORT_EVT_PAYLOAD = 8;

struct MaStatusCapture {
    bool received = false;
    uint8_t status = 0xFF;

    void Reset()
    {
        *this = MaStatusCapture{};
    }
};

struct MaReadSizeCapture {
    bool received = false;
    uint8_t status = 0xFF;
    uint8_t number = 0xFF;

    void Reset()
    {
        *this = MaReadSizeCapture{};
    }
};

struct MaReportCapture {
    bool received = false;
    HciLeMonitoredAdvertisersReportEventParam param = {};

    void Reset()
    {
        *this = MaReportCapture{};
    }
};

static MaStatusCapture g_maAddCapture;
static MaStatusCapture g_maRemoveCapture;
static MaStatusCapture g_maClearCapture;
static MaReadSizeCapture g_maReadSizeCapture;
static MaStatusCapture g_maEnableCapture;
static MaReportCapture g_maReportCapture;

static void OnMaAddComplete(const HciLeAddDeviceToMonitoredAdvertisersListReturnParam *returnParam)
{
    if (returnParam == nullptr) {
        return;
    }
    g_maAddCapture.status = returnParam->status;
    g_maAddCapture.received = true;
}

static void OnMaRemoveComplete(const HciLeRemoveDeviceFromMonitoredAdvertisersListReturnParam *returnParam)
{
    if (returnParam == nullptr) {
        return;
    }
    g_maRemoveCapture.status = returnParam->status;
    g_maRemoveCapture.received = true;
}

static void OnMaClearComplete(const HciLeClearMonitoredAdvertisersListReturnParam *returnParam)
{
    if (returnParam == nullptr) {
        return;
    }
    g_maClearCapture.status = returnParam->status;
    g_maClearCapture.received = true;
}

static void OnMaReadSizeComplete(const HciLeReadMonitoredAdvertisersListSizeReturnParam *returnParam)
{
    if (returnParam == nullptr) {
        return;
    }
    g_maReadSizeCapture.status = returnParam->status;
    g_maReadSizeCapture.number = returnParam->number;
    g_maReadSizeCapture.received = true;
}

static void OnMaEnableComplete(const HciLeEnableMonitoringAdvertisersReturnParam *returnParam)
{
    if (returnParam == nullptr) {
        return;
    }
    g_maEnableCapture.status = returnParam->status;
    g_maEnableCapture.received = true;
}

static void OnMaMonitoredAdvertisersReport(const HciLeMonitoredAdvertisersReportEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_maReportCapture.param = *eventParam;
    g_maReportCapture.received = true;
}

// Bind the six MA members for the duration of one case, then restore to NULL so
// the shared parse set behaves exactly as it did before (only the 0x2B member set).
static void BindMaParseCallbacks()
{
    g_parseCallbacks.leAddDeviceToMonitoredAdvertisersListComplete = OnMaAddComplete;
    g_parseCallbacks.leRemoveDeviceFromMonitoredAdvertisersListComplete = OnMaRemoveComplete;
    g_parseCallbacks.leClearMonitoredAdvertisersListComplete = OnMaClearComplete;
    g_parseCallbacks.leReadMonitoredAdvertisersListSizeComplete = OnMaReadSizeComplete;
    g_parseCallbacks.leEnableMonitoringAdvertisersComplete = OnMaEnableComplete;
    g_parseCallbacks.leMonitoredAdvertisersReport = OnMaMonitoredAdvertisersReport;
}

static void UnbindMaParseCallbacks()
{
    g_parseCallbacks.leAddDeviceToMonitoredAdvertisersListComplete = nullptr;
    g_parseCallbacks.leRemoveDeviceFromMonitoredAdvertisersListComplete = nullptr;
    g_parseCallbacks.leClearMonitoredAdvertisersListComplete = nullptr;
    g_parseCallbacks.leReadMonitoredAdvertisersListSizeComplete = nullptr;
    g_parseCallbacks.leEnableMonitoringAdvertisersComplete = nullptr;
    g_parseCallbacks.leMonitoredAdvertisersReport = nullptr;
}

class MaParseCallbackGuard {
public:
    MaParseCallbackGuard() = default;
    ~MaParseCallbackGuard() { UnbindMaParseCallbacks(); }
    MaParseCallbackGuard(const MaParseCallbackGuard &) = delete;
    MaParseCallbackGuard &operator=(const MaParseCallbackGuard &) = delete;
};

static void ResetMaCaptures()
{
    g_maAddCapture.Reset();
    g_maRemoveCapture.Reset();
    g_maClearCapture.Reset();
    g_maReadSizeCapture.Reset();
    g_maEnableCapture.Reset();
    g_maReportCapture.Reset();
}

// @tc.number: StackGapLe6_0_MaConstants_01500
// @tc.name:  8.1 MA macro/layout values echo
// @tc.desc:  Runtime echo of the MA static_asserts for diagnosability.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_MaConstants_01500, TestSize.Level1)
{
    EXPECT_EQ(HCI_LE_ADD_DEVICE_TO_MONITORED_ADVERTISERS_LIST,
        MAKE_OPCODE(0x0098, HCI_COMMAND_OGF_LE_CONTROLLER));
    EXPECT_EQ(HCI_LE_ENABLE_MONITORING_ADVERTISERS,
        MAKE_OPCODE(0x009C, HCI_COMMAND_OGF_LE_CONTROLLER));
    EXPECT_EQ(sizeof(HciLeAddDeviceToMonitoredAdvertisersListParam), 10u);
    EXPECT_EQ(sizeof(HciLeRemoveDeviceFromMonitoredAdvertisersListParam), 7u);
    EXPECT_EQ(sizeof(HciLeEnableMonitoringAdvertisersParam), 1u);
    EXPECT_EQ(sizeof(HciLeReadMonitoredAdvertisersListSizeReturnParam), 2u);
    EXPECT_EQ(sizeof(HciLeMonitoredAdvertisersReportEventParam), 8u);
    EXPECT_EQ(LE_EVENT_MASK_LE_MONITORED_ADVERTISERS_REPORT_EVENT, 0x0008000000000000ULL);
    EXPECT_EQ(HCI_LE_MONITORED_ADVERTISERS_REPORT_EVENT - 1, 51);
}

// @tc.number: StackGapLe6_0_MaReportParse_01600
// @tc.name:  0x34 LE Monitored Advertisers Report full-field parse per address type
// @tc.desc:  Inject the 8-byte subevent payload with Address_Type 0x00..0x03; each
//            lands on leMonitoredAdvertisersReport with addrType/addr/condition intact.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_MaReportParse_01600, TestSize.Level1)
{
    BindMaParseCallbacks();
    MaParseCallbackGuard maGuard;
    const uint8_t addr[BD_ADDR_LENGTH] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
    for (uint8_t type = 0x00; type <= 0x03; ++type) {
        ResetMaCaptures();
        // Condition octet (7.7.65,47, p2464): 0x00 below-low past timeout, 0x01 RSSI >= high.
        const uint8_t condition = (type & 0x01u) ? MA_CONDITION_RSSI_ABOVE_HIGH : MA_CONDITION_BELOW_LOW_TIMEOUT;
        uint8_t wire[LE_META_EVT_HEAD_LEN + MA_REPORT_EVT_PAYLOAD] = {0};
        wire[LE_META_EVT_EVENT_CODE_OFFSET] = HCI_LE_META_EVENT;                          // Event_Code: LE Meta
        // Parameter_Total_Length
        wire[LE_META_EVT_LENGTH_OFFSET] = static_cast<uint8_t>(
            (LE_META_EVT_HEAD_LEN - LE_META_EVT_PREFIX_LEN) + MA_REPORT_EVT_PAYLOAD);
        wire[LE_META_EVT_SUBEVENT_CODE_OFFSET] = HCI_LE_MONITORED_ADVERTISERS_REPORT_EVENT;  // Subevent 0x34
        HciLeMonitoredAdvertisersReportEventParam maParam = {};
        maParam.advertiserAddressType = type;
        ASSERT_EQ(memcpy_s(maParam.advertiserAddress.raw, BD_ADDR_LENGTH, addr, BD_ADDR_LENGTH), EOK);
        maParam.condition = condition;
        ASSERT_EQ(memcpy_s(wire + LE_META_EVT_HEAD_LEN, sizeof(wire) - LE_META_EVT_HEAD_LEN,
            &maParam, sizeof(maParam)), EOK);
        InjectWire(wire, sizeof(wire));

        EXPECT_TRUE(g_maReportCapture.received);
        EXPECT_EQ(g_maReportCapture.param.advertiserAddressType, type);
        EXPECT_EQ(memcmp(g_maReportCapture.param.advertiserAddress.raw, addr, BD_ADDR_LENGTH), 0);
        EXPECT_EQ(g_maReportCapture.param.condition, condition);
    }
}

// @tc.number: StackGapLe6_0_MaReportTruncatedDrop_01700
// @tc.name:  truncated 0x34 payload is silently dropped
// @tc.desc:  handler requires the full 8 octets after the Subevent_Code.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_MaReportTruncatedDrop_01700, TestSize.Level1)
{
    BindMaParseCallbacks();
    MaParseCallbackGuard maGuard;
    ResetMaCaptures();
    const uint8_t truncated[] = {
        0x3E, 0x08, HCI_LE_MONITORED_ADVERTISERS_REPORT_EVENT,
        0x01, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,  // only 7 octets of the 8 required
    };
    InjectWire(truncated, sizeof(truncated));
    EXPECT_FALSE(g_maReportCapture.received);
}

// @tc.number: StackGapLe6_0_MaCommandComplete_01800
// @tc.name:  five 0x0098-0x009C Command_Complete dispatches deliver Status (and Number)
// @tc.desc:  Direct-drive each CC table entry (5.3 pattern, bypassing the pending
//            front door); Read Size additionally carries the list capacity.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_MaCommandComplete_01800, TestSize.Level1)
{
    BindMaParseCallbacks();
    MaParseCallbackGuard maGuard;
    ResetMaCaptures();

    uint8_t status = 0x00;
    HciEventOnLeCommandComplete(HCI_LE_ADD_DEVICE_TO_MONITORED_ADVERTISERS_LIST, &status, sizeof(status));
    HciEventOnLeCommandComplete(HCI_LE_REMOVE_DEVICE_FROM_MONITORED_ADVERTISERS_LIST, &status, sizeof(status));
    HciEventOnLeCommandComplete(HCI_LE_CLEAR_MONITORED_ADVERTISERS_LIST, &status, sizeof(status));
    HciEventOnLeCommandComplete(HCI_LE_ENABLE_MONITORING_ADVERTISERS, &status, sizeof(status));

    uint8_t readSize[2] = {0x00, 0x05};
    HciEventOnLeCommandComplete(HCI_LE_READ_MONITORED_ADVERTISERS_LIST_SIZE, readSize, sizeof(readSize));

    EXPECT_TRUE(g_maAddCapture.received);
    EXPECT_EQ(g_maAddCapture.status, 0x00);
    EXPECT_TRUE(g_maRemoveCapture.received);
    EXPECT_EQ(g_maRemoveCapture.status, 0x00);
    EXPECT_TRUE(g_maClearCapture.received);
    EXPECT_EQ(g_maClearCapture.status, 0x00);
    EXPECT_TRUE(g_maEnableCapture.received);
    EXPECT_EQ(g_maEnableCapture.status, 0x00);
    EXPECT_TRUE(g_maReadSizeCapture.received);
    EXPECT_EQ(g_maReadSizeCapture.status, 0x00);
    EXPECT_EQ(g_maReadSizeCapture.number, 0x05);
}

// @tc.number: StackGapLe6_0_MaCommandFailed_01900
// @tc.name:  controller-failure path of the five MA commands closes with the error Status
// @tc.desc:  HciCmdOnLe*Failed synthesize the Command_Complete Return-Status; on a
//            send timeout the Read Size capacity is unknown and reports Number 0.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_MaCommandFailed_01900, TestSize.Level1)
{
    BindMaParseCallbacks();
    MaParseCallbackGuard maGuard;
    ResetMaCaptures();

    const uint8_t errorStatus = 0x03;  // HCI_ERR_UNKNOWN_COMMAND not needed: any error
    HciOnLeControllerCmdFailed(HCI_LE_ADD_DEVICE_TO_MONITORED_ADVERTISERS_LIST, errorStatus, nullptr);
    HciOnLeControllerCmdFailed(HCI_LE_REMOVE_DEVICE_FROM_MONITORED_ADVERTISERS_LIST, errorStatus, nullptr);
    HciOnLeControllerCmdFailed(HCI_LE_CLEAR_MONITORED_ADVERTISERS_LIST, errorStatus, nullptr);
    HciOnLeControllerCmdFailed(HCI_LE_READ_MONITORED_ADVERTISERS_LIST_SIZE, errorStatus, nullptr);
    HciOnLeControllerCmdFailed(HCI_LE_ENABLE_MONITORING_ADVERTISERS, errorStatus, nullptr);

    EXPECT_TRUE(g_maAddCapture.received);
    EXPECT_EQ(g_maAddCapture.status, errorStatus);
    EXPECT_TRUE(g_maRemoveCapture.received);
    EXPECT_EQ(g_maRemoveCapture.status, errorStatus);
    EXPECT_TRUE(g_maClearCapture.received);
    EXPECT_EQ(g_maClearCapture.status, errorStatus);
    EXPECT_TRUE(g_maEnableCapture.received);
    EXPECT_EQ(g_maEnableCapture.status, errorStatus);
    EXPECT_TRUE(g_maReadSizeCapture.received);
    EXPECT_EQ(g_maReadSizeCapture.status, errorStatus);
    EXPECT_EQ(g_maReadSizeCapture.number, 0x00);
}

// ---- 8.2 Frame Space Update dispatch (plan 8.2): subevent 0x35 parse ---------
// Capture the LE Frame Space Update Complete event by value: the 0x35 handler
// hands the registered callback a zero-copy pointer into the event packet, so the
// packed struct is copied out before the packet is freed (5.1 event-parse suite).
constexpr size_t FSU_EVT_PAYLOAD = 9;  // bytes after the Subevent_Code

struct FsuCapture {
    bool received = false;
    HciLeFrameSpaceUpdateCompleteEventParam param = {};

    void Reset()
    {
        *this = FsuCapture{};
    }
};

static FsuCapture g_fsuCapture;

static void OnFsuComplete(const HciLeFrameSpaceUpdateCompleteEventParam *eventParam)
{
    if (eventParam == nullptr) {
        return;
    }
    g_fsuCapture.param = *eventParam;
    g_fsuCapture.received = true;
}

static void BindFsuParseCallbacks()
{
    g_parseCallbacks.leFrameSpaceUpdateComplete = OnFsuComplete;
}

static void UnbindFsuParseCallbacks()
{
    g_parseCallbacks.leFrameSpaceUpdateComplete = nullptr;
}

class FsuParseCallbackGuard {
public:
    FsuParseCallbackGuard() = default;
    ~FsuParseCallbackGuard() { UnbindFsuParseCallbacks(); }
    FsuParseCallbackGuard(const FsuParseCallbackGuard &) = delete;
    FsuParseCallbackGuard &operator=(const FsuParseCallbackGuard &) = delete;
};

static void ResetFsuCaptures()
{
    g_fsuCapture.Reset();
}

// @tc.number: StackGapLe6_0_FsuConstants_02100
// @tc.name:  8.1 FSU macro/layout values echo
// @tc.desc:  Runtime echo of the FSU static_asserts for diagnosability.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_FsuConstants_02100, TestSize.Level1)
{
    EXPECT_EQ(HCI_LE_FRAME_SPACE_UPDATE, MAKE_OPCODE(0x009D, HCI_COMMAND_OGF_LE_CONTROLLER));
    EXPECT_EQ(sizeof(HciLeFrameSpaceUpdateParam), 9u);
    EXPECT_EQ(sizeof(HciLeFrameSpaceUpdateCompleteEventParam), 9u);
    EXPECT_EQ(HCI_LE_FRAME_SPACE_UPDATE_COMPLETE_EVENT, 0x35);
    EXPECT_EQ(HCI_LE_FRAME_SPACE_UPDATE_COMPLETE_EVENT - 1, 52);
    EXPECT_EQ(LE_EVENT_MASK_LE_FRAME_SPACE_UPDATE_COMPLETE_EVENT, 0x0010000000000000ULL);
    EXPECT_EQ(LE_EVENT_MASK_LE_FRAME_SPACE_UPDATE_COMPLETE_EVENT, (1ULL << 52));
    EXPECT_EQ(LE_FRAME_SPACE_MAX_US, 0x2710);
    EXPECT_EQ(LE_FRAME_SPACE_PHYS_MASK, 0x07);
    EXPECT_EQ(LE_FRAME_SPACE_SPACING_TYPES_MASK, 0x1F);
}

// @tc.number: StackGapLe6_0_FsuReportParse_02101
// @tc.name:  0x35 LE Frame Space Update Complete full-field parse per Initiator
// @tc.desc:  Inject the 9-byte subevent payload for Initiator 0x00/0x01/0x02; the
//            handler broadcasts host, local-controller and peer completions alike,
//            so status/connectionHandle/initiator/frameSpace/phys/spacingTypes land
//            intact (7.7.65,48, p2466-2468).
HWTEST_F(StackGapLe60Test, StackGapLe6_0_FsuReportParse_02101, TestSize.Level1)
{
    BindFsuParseCallbacks();
    FsuParseCallbackGuard fsuGuard;
    const uint16_t handle = FSU_TEST_HANDLE;
    for (uint8_t initiator = 0x00; initiator <= FSU_INITIATOR_MAX; ++initiator) {
        ResetFsuCaptures();
        uint8_t wire[LE_META_EVT_HEAD_LEN + FSU_EVT_PAYLOAD] = {0};
        wire[LE_META_EVT_EVENT_CODE_OFFSET] = HCI_LE_META_EVENT;                          // Event_Code: LE Meta
        // Parameter_Total_Length
        wire[LE_META_EVT_LENGTH_OFFSET] = static_cast<uint8_t>(
            (LE_META_EVT_HEAD_LEN - LE_META_EVT_PREFIX_LEN) + FSU_EVT_PAYLOAD);
        wire[LE_META_EVT_SUBEVENT_CODE_OFFSET] = HCI_LE_FRAME_SPACE_UPDATE_COMPLETE_EVENT;   // Subevent 0x35
        HciLeFrameSpaceUpdateCompleteEventParam fsuParam = {};
        fsuParam.status = 0x00;
        fsuParam.connectionHandle = handle;
        fsuParam.initiator = initiator;
        fsuParam.frameSpace = FSU_TEST_FRAME_SPACE_US;        // 100 us
        fsuParam.phys = FSU_TEST_PHYS;                        // LE 1M | LE 2M | LE Coded
        fsuParam.spacingTypes = FSU_TEST_SPACING_TYPES;
        ASSERT_EQ(memcpy_s(wire + LE_META_EVT_HEAD_LEN, sizeof(wire) - LE_META_EVT_HEAD_LEN,
            &fsuParam, sizeof(fsuParam)), EOK);
        InjectWire(wire, sizeof(wire));

        EXPECT_TRUE(g_fsuCapture.received);
        EXPECT_EQ(g_fsuCapture.param.status, 0x00);
        EXPECT_EQ(g_fsuCapture.param.connectionHandle, handle);
        EXPECT_EQ(g_fsuCapture.param.initiator, initiator);
        EXPECT_EQ(g_fsuCapture.param.frameSpace, FSU_TEST_FRAME_SPACE_US);
        EXPECT_EQ(g_fsuCapture.param.phys, FSU_TEST_PHYS);
        EXPECT_EQ(g_fsuCapture.param.spacingTypes, FSU_TEST_SPACING_TYPES);
    }
}

// @tc.number: StackGapLe6_0_FsuReportTruncatedDrop_02102
// @tc.name:  truncated 0x35 payload is silently dropped
// @tc.desc:  the handler requires the full 9 octets after the Subevent_Code; a
//            8-octet payload never reaches the registered callback.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_FsuReportTruncatedDrop_02102, TestSize.Level1)
{
    BindFsuParseCallbacks();
    FsuParseCallbackGuard fsuGuard;
    ResetFsuCaptures();
    const uint8_t truncated[] = {
        0x3E, 0x09, HCI_LE_FRAME_SPACE_UPDATE_COMPLETE_EVENT,
        0x00, 0x42, 0x00, 0x00, 0x64, 0x00, 0x07, 0x1F,  // only 8 octets of the 9 required
    };
    InjectWire(truncated, sizeof(truncated));
    EXPECT_FALSE(g_fsuCapture.received);
}

// @tc.number: StackGapLe6_0_FsuSenderInvalidParam_02103
// @tc.name:  0x009D sender host checks reject every reserved/boundary violation
// @tc.desc:  HCI_LeFrameSpaceUpdate rejects a NULL param, min > max, max above the
//            0x2710 ceiling and zero/reserved PHYS and Spacing_Types. A rejected
//            parameter never builds an HCI command, so it is safe to drive on the
//            dispatch fixture with no controller (5.3 pattern).
HWTEST_F(StackGapLe60Test, StackGapLe6_0_FsuSenderInvalidParam_02103, TestSize.Level1)
{
    HciLeFrameSpaceUpdateParam cmd = {};
    cmd.connectionHandle = 0x0042;
    cmd.frameSpaceMin = 0x0064;
    cmd.frameSpaceMax = 0x0064;
    cmd.phys = 0x01;
    cmd.spacingTypes = 0x01;

    EXPECT_EQ(HCI_LeFrameSpaceUpdate(nullptr), BT_BAD_PARAM);

    HciLeFrameSpaceUpdateParam bad = cmd;
    bad.frameSpaceMin = 0x00C8;  // min > max
    EXPECT_EQ(HCI_LeFrameSpaceUpdate(&bad), BT_BAD_PARAM);

    bad = cmd;
    bad.frameSpaceMax = 0x2711;  // above the 0x2710 ceiling
    EXPECT_EQ(HCI_LeFrameSpaceUpdate(&bad), BT_BAD_PARAM);

    bad = cmd;
    bad.phys = 0x00;  // no PHY selected
    EXPECT_EQ(HCI_LeFrameSpaceUpdate(&bad), BT_BAD_PARAM);

    bad = cmd;
    bad.phys = 0x08;  // reserved PHY bit only
    EXPECT_EQ(HCI_LeFrameSpaceUpdate(&bad), BT_BAD_PARAM);

    bad = cmd;
    bad.phys = 0x09;  // reserved PHY bit alongside a valid PHY
    EXPECT_EQ(HCI_LeFrameSpaceUpdate(&bad), BT_BAD_PARAM);

    bad = cmd;
    bad.spacingTypes = 0x00;  // no spacing type
    EXPECT_EQ(HCI_LeFrameSpaceUpdate(&bad), BT_BAD_PARAM);

    bad = cmd;
    bad.spacingTypes = 0x20;  // reserved spacing bit 5 only
    EXPECT_EQ(HCI_LeFrameSpaceUpdate(&bad), BT_BAD_PARAM);

    bad = cmd;
    bad.spacingTypes = 0x21;  // reserved spacing bit alongside a valid type
    EXPECT_EQ(HCI_LeFrameSpaceUpdate(&bad), BT_BAD_PARAM);
}

// @tc.number: StackGapLe6_0_FsuFailureSynth_02104
// @tc.name:  CS failure close-out synthesizes the 0x35 event on the failure table
// @tc.desc:  a 0x009D Command_Status rejection or send timeout produces no
//            controller 0x35, so HciOnLeControllerCmdFailed synthesizes one: the
//            status is carried, initiator reports Local Host (0x00) and the frame
//            space fields are zeroed. A present command param restores its handle.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_FsuFailureSynth_02104, TestSize.Level1)
{
    BindFsuParseCallbacks();
    FsuParseCallbackGuard fsuGuard;

    const uint8_t errorStatus = 0x02;  // HCI_ERR_UNKNOWN_CONNECTION_ID
    HciLeFrameSpaceUpdateParam cmd = {};
    cmd.connectionHandle = 0x0042;
    cmd.frameSpaceMin = 0x0064;
    cmd.frameSpaceMax = 0x00C8;
    cmd.phys = 0x01;
    cmd.spacingTypes = 0x01;

    ResetFsuCaptures();
    HciOnLeControllerCmdFailed(HCI_LE_FRAME_SPACE_UPDATE, errorStatus, &cmd);
    EXPECT_TRUE(g_fsuCapture.received);
    EXPECT_EQ(g_fsuCapture.param.status, errorStatus);
    EXPECT_EQ(g_fsuCapture.param.connectionHandle, 0x0042);  // restored from the command
    EXPECT_EQ(g_fsuCapture.param.initiator, 0x00);
    EXPECT_EQ(g_fsuCapture.param.frameSpace, 0x00);
    EXPECT_EQ(g_fsuCapture.param.phys, 0x00);
    EXPECT_EQ(g_fsuCapture.param.spacingTypes, 0x00);

    // Defensive NULL-param branch: the close-out still fires with the invalid
    // handle sentinel (0xFFFF) and zeroed frame space fields, so the upper layer
    // is never left hanging by a send failure.
    ResetFsuCaptures();
    HciOnLeControllerCmdFailed(HCI_LE_FRAME_SPACE_UPDATE, errorStatus, nullptr);
    EXPECT_TRUE(g_fsuCapture.received);
    EXPECT_EQ(g_fsuCapture.param.status, errorStatus);
    EXPECT_EQ(g_fsuCapture.param.connectionHandle, 0xFFFF);
    EXPECT_EQ(g_fsuCapture.param.initiator, 0x00);
    EXPECT_EQ(g_fsuCapture.param.frameSpace, 0x00);
    EXPECT_EQ(g_fsuCapture.param.phys, 0x00);
    EXPECT_EQ(g_fsuCapture.param.spacingTypes, 0x00);
}


// ---- 8.1 / 8.2 module 6: ISOAL Unsegmented Framed Mode ------------------------
// The module-6 single-board cases (plan 8) fold into the two fixtures above. The
// 0x2A / 0x22 dispatch-level parses join StackGapLe60Test (their captures are
// registered in the shared SetUpTestCase and reset in SetUp); the iso-layer seam
// cases join StackGapLe60BtmTest, where each test self-registers its CIG / BIG
// callback through the guard objects - the suite leaves LE enabled with the role
// OBSERVER|CENTRAL, which is exactly the module-6 boot contract. The harness
// deliberately duplicates the 5.2 iso receive pattern instead of sharing it, so
// this battery never touches stack_gap_le_5_2_test (placement decision I-4).

// @tc.number: StackGapLe6_0_IsoalLayout_00100
// @tc.name:  plan 8.1 ISOAL layout/macro values echo
// @tc.desc:  Runtime echo of the module-6 static_asserts for diagnosability.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_IsoalLayout_00100, TestSize.Level1)
{
    EXPECT_EQ(sizeof(HciLeCisEstablishedV2EventParam), 42u);
    EXPECT_EQ(HCI_LE_CIS_ESTABLISHED_V2_EVENT, 0x2A);
    // 8.2(1): mask bit 41 == subevent 0x2A - 1, disjoint from the [v1] bit-24 mask.
    EXPECT_EQ((HCI_LE_CIS_ESTABLISHED_V2_EVENT - 1), 41);
    EXPECT_EQ(LE_EVENT_MASK_LE_CIS_ESTABLISHED_V2_EVENT, 0x0000020000000000ULL);
    EXPECT_EQ(LE_EVENT_MASK_LE_CIS_ESTABLISHED_V2_EVENT, (1ULL << 41));
    EXPECT_EQ(LE_EVENT_MASK_LE_CIS_ESTABLISHED_EVENT, 0x0000000001000000ULL);
    EXPECT_EQ((LE_EVENT_MASK_LE_CIS_ESTABLISHED_V2_EVENT & LE_EVENT_MASK_LE_CIS_ESTABLISHED_EVENT), 0ULL);
    EXPECT_NE(LE_EVENT_MASK_LE_CIS_ESTABLISHED_V2_EVENT, LE_EVENT_MASK_LE_CIS_ESTABLISHED_EVENT);
    EXPECT_EQ(LE_FEATURE_BIT_UNSEGMENTED_FRAMED_MODE, 45);
}

// @tc.number: StackGapLe6_0_IsoalUfmBit45_00200
// @tc.name:  page-0 bit 45 (UFM) mapping on the 8-byte and 248-byte containers
// @tc.desc:  bit 45 = octet 5 bit 5; neighbour bits 44/46 and bit 63 stay clear.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_IsoalUfmBit45_00200, TestSize.Level1)
{
    // 8-byte page-0 (0x0003 result) container, same mapping as the MA/FSU bit tests.
    HciLeFeatures page0 = {};
    page0.raw[5] = 0x20;  // bit 45
    EXPECT_TRUE(HciSupportLeUnsegmentedFramedMode(page0.raw));
    EXPECT_TRUE(GetLinkLayerFeatureFlag(page0.raw, LE_FEATURE_BIT_UNSEGMENTED_FRAMED_MODE));

    // Bits 44 (0x10) and 46 (0x40) in the same octet must not alias bit 45.
    page0.raw[5] = 0x10;  // bit 44
    EXPECT_FALSE(HciSupportLeUnsegmentedFramedMode(page0.raw));
    page0.raw[5] = 0x40;  // bit 46
    EXPECT_FALSE(HciSupportLeUnsegmentedFramedMode(page0.raw));

    // An out-of-page-0 bit (63, the LL Extended Feature Set marker) must not set bit 45.
    HciLeFeatures high = {};
    high.raw[7] = 0x80;  // bit 63
    EXPECT_FALSE(HciSupportLeUnsegmentedFramedMode(high.raw));
    EXPECT_FALSE(GetLinkLayerFeatureFlag(high.raw, LE_FEATURE_BIT_UNSEGMENTED_FRAMED_MODE));

    // Same bit read through the 248-byte container prefix.
    HciLeFeaturesAll all = {};
    all.raw[5] = 0x20;
    EXPECT_TRUE(HciSupportLeUnsegmentedFramedMode(all.raw));
    all.raw[5] = 0x00;
    EXPECT_FALSE(HciSupportLeUnsegmentedFramedMode(all.raw));
}

// @tc.number: StackGapLe6_0_IsoalCisEstablishedV2Parse_00400
// @tc.name:  0x2A LE CIS Established v2 full-field parse
// @tc.desc:  Inject the 42-octet subevent payload; assert the shared-prefix and the
//            v2-tail fields all land in the callback param unchanged.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_IsoalCisEstablishedV2Parse_00400, TestSize.Level1)
{
    // The wire layout after Subevent_Code equals the packed 42-octet struct, so build
    // the payload from a filled struct; a parse offset error then shows up as a mismatch.
    HciLeCisEstablishedV2EventParam p = {};
    p.status = 0x00;
    p.connectionHandle = 0x1234;
    p.cigSyncDelay[0] = 0x34;
    p.cigSyncDelay[1] = 0x56;
    p.cigSyncDelay[2] = 0x12;
    p.cisSyncDelay[0] = 0x78;
    p.cisSyncDelay[1] = 0x9A;
    p.cisSyncDelay[2] = 0x34;
    p.transportLatencyMToS[0] = 0x10;
    p.transportLatencySToM[0] = 0x20;
    p.phyMToS = 0x02;
    p.phySToM = 0x02;
    p.nse = 0x02;
    p.bnMToS = 0x01;
    p.bnSToM = 0x01;
    p.ftMToS = 0x02;
    p.ftSToM = 0x02;
    p.maxPduMToS = 0x0100;
    p.maxPduSToM = 0x0200;
    p.isoInterval = 0x0010;
    p.subInterval[0] = 0x90;
    p.subInterval[1] = 0x01;
    p.maxSduMToS = 0x0123;
    p.maxSduSToM = 0x0045;
    p.sduIntervalMToS[0] = 0xFF;
    p.sduIntervalSToM[0] = 0x10;
    p.framing = 0x01;

    InjectCisEstablishedV2Event(p);

    EXPECT_TRUE(g_v2ParseCapture.received);
    const HciLeCisEstablishedV2EventParam &par = g_v2ParseCapture.param;
    EXPECT_EQ(par.status, p.status);
    EXPECT_EQ(par.connectionHandle, p.connectionHandle);
    EXPECT_EQ(par.nse, 0x02);
    EXPECT_EQ(par.bnMToS, 0x01);
    EXPECT_EQ(par.maxPduMToS, 0x0100);
    EXPECT_EQ(par.isoInterval, 0x0010);
    EXPECT_EQ(par.maxSduMToS, 0x0123);
    EXPECT_EQ(par.maxSduSToM, 0x0045);
    EXPECT_EQ(par.framing, 0x01);
    EXPECT_EQ(0, memcmp(par.cigSyncDelay, p.cigSyncDelay, sizeof(p.cigSyncDelay)));
    EXPECT_EQ(0, memcmp(par.subInterval, p.subInterval, sizeof(p.subInterval)));
    EXPECT_EQ(0, memcmp(par.sduIntervalMToS, p.sduIntervalMToS, sizeof(p.sduIntervalMToS)));
    // Whole-struct equality: no field drifted.
    EXPECT_EQ(0, memcmp(&par, &p, sizeof(p)));
}

// @tc.number: StackGapLe6_0_IsoalCisEstablishedV2ShortPayload_00500
// @tc.name:  a short 0x2A payload is non-conformant and dropped
// @tc.desc:  LE CIS Established [v2] has a fixed 42-octet parameter set (Vol 4 Part E
//            7.7.65,25); controllers report failure with the same length. A 3-octet
//            (Status + Handle) event must not notify the leCisEstablishedV2 callback.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_IsoalCisEstablishedV2ShortPayload_00500, TestSize.Level1)
{
    const uint8_t wire[] = {
        0x3E, 0x04, HCI_LE_CIS_ESTABLISHED_V2_EVENT,  // Event_Code, len, Subevent
        0x11, 0x42, 0x00,                            // Status(Unsupported) + Handle 0x0042
    };
    InjectWire(wire, sizeof(wire));

    EXPECT_FALSE(g_v2ParseCapture.received) << "short 0x2A must be dropped, not notified";
}

// @tc.number: StackGapLe6_0_IsoalBigInfoReportParse_00700
// @tc.name:  0x22 LE BIGInfo Advertising Report carries Framing verbatim (HCI layer)
// @tc.desc:  Inject the 19-octet 0x22 payload with framing 0x02; the existing parse must
//            copy it unchanged (Vol 4 Part E 7.7.65,34 p2415).
HWTEST_F(StackGapLe60Test, StackGapLe6_0_IsoalBigInfoReportParse_00700, TestSize.Level1)
{
    HciLeBigInfoAdvertisingReportEventParam p = {};
    p.syncHandle = 0x0042;
    p.numBis = 0x02;
    p.nse = 0x02;
    p.isoInterval = 0x0010;
    p.maxPdu = 0x00FB;
    p.sduInterval[0] = 0x10;
    p.sduInterval[1] = 0x27;
    p.maxSdu = 0x0100;
    p.phy = 0x02;
    p.framing = 0x02;  // ISO_LE_FRAMING_UNSEGMENTED, RFU pre-BT6.0
    p.encryption = 0x00;

    uint8_t wire[LE_META_EVT_HEAD_LEN + sizeof(p)];
    wire[LE_META_EVT_EVENT_CODE_OFFSET] = HCI_LE_META_EVENT;
    wire[LE_META_EVT_LENGTH_OFFSET] = static_cast<uint8_t>(1 + sizeof(p));
    wire[LE_META_EVT_SUBEVENT_CODE_OFFSET] = HCI_LE_BIGINFO_ADVERTISING_REPORT_EVENT;  // Subevent 0x22
    ASSERT_EQ(memcpy_s(wire + LE_META_EVT_HEAD_LEN, sizeof(p), &p, sizeof(p)), EOK);
    InjectWire(wire, sizeof(wire));

    EXPECT_TRUE(g_bigInfoParseCapture.received);
    const HciLeBigInfoAdvertisingReportEventParam &par = g_bigInfoParseCapture.param;
    EXPECT_EQ(par.syncHandle, 0x0042);
    EXPECT_EQ(par.numBis, 0x02);
    EXPECT_EQ(par.isoInterval, 0x0010);
    EXPECT_EQ(par.maxSdu, 0x0100);
    EXPECT_EQ(par.framing, 0x02) << "framing 0x02 must be copied verbatim, not gated";
    EXPECT_EQ(0, memcmp(&par, &p, sizeof(p)));
}

// @tc.number: StackGapLe6_0_IsoalBigInfoShortReport_00701
// @tc.name:  a 0x22 report one octet short of its structure is dropped
// @tc.desc:  Round 4 F9: the 0x22 BIGInfo Advertising Report guard (hci_evt_le.c:1501) had
//            no truncated case, so of its two doors only the parse side was ever driven.
//            The report is a fixed 19 octets (Vol 4 Part E 7.7.65,34, p2415); a body one
//            octet short of it must not reach the consumer.
HWTEST_F(StackGapLe60Test, StackGapLe6_0_IsoalBigInfoShortReport_00701, TestSize.Level1)
{
    // Subevent_Code + 18 octets: one short of the 19 the structure takes. The Length octet
    // is derived from the array, so the frame is self-consistent and only the handler's own
    // lower bound can reject it.
    uint8_t wire[LE_META_EVT_HEAD_LEN + sizeof(HciLeBigInfoAdvertisingReportEventParam) - 1] = {0};
    wire[LE_META_EVT_EVENT_CODE_OFFSET] = HCI_LE_META_EVENT;
    wire[LE_META_EVT_LENGTH_OFFSET] = static_cast<uint8_t>(sizeof(wire) - LE_META_EVT_PREFIX_LEN);
    wire[LE_META_EVT_SUBEVENT_CODE_OFFSET] = HCI_LE_BIGINFO_ADVERTISING_REPORT_EVENT;  // Subevent 0x22
    InjectWire(wire, sizeof(wire));

    EXPECT_FALSE(g_bigInfoParseCapture.received) << "short 0x22 must be dropped, not notified";
}

// @tc.number: StackGapLe6_0_IsoalCisEstablishedV2Merge_00600
// @tc.name:  v2 success notifies once; a late v1 (or v2) duplicate is deduped
// @tc.desc:  A successful 0x2A attaches v2Ext with the six extended fields; the same
//            handle arriving again as 0x19 or 0x2A must not notify a second time, and a
//            single 0x05 afterwards reports once - proving one tracking entry.
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_IsoalCisEstablishedV2Merge_00600, TestSize.Level1)
{
    constexpr uint16_t handle = 0x0042;
    IsoCisCtx ctx;
    CigCallbackGuard guard(&ctx);
    ASSERT_EQ(guard.registered, BT_SUCCESS);

    // v2 (0x2A) success: 24-bit intervals in wire order (LSB first, the octets the
    // HCI parse copies and IsoReadUint24 later decodes).
    HciLeCisEstablishedV2EventParam p = {};
    p.status = 0x00;
    p.connectionHandle = handle;
    p.cigSyncDelay[0] = 0x90;
    p.cigSyncDelay[1] = 0x01;
    p.nse = 0x02;
    p.isoInterval = 0x0010;
    p.subInterval[0] = 0x90;  // Sub_Interval 0x190 us (NSE != 1)
    p.subInterval[1] = 0x01;
    p.maxSduMToS = 0x0123;
    p.maxSduSToM = 0x0045;
    p.sduIntervalMToS[0] = 0xFF;  // 0x000100FF
    p.sduIntervalMToS[2] = 0x01;
    p.sduIntervalSToM[0] = 0x10;  // 0x00000010
    p.framing = 0x01;  // framed; the [v2] event Framing field only reports 0x00/0x01 (D-5)
    IsoLeCisEstablishedEventV2(&p);

    EXPECT_EQ(ctx.establishedCount, 1u);
    EXPECT_EQ(ctx.estStatus, 0x00);
    EXPECT_EQ(ctx.estHandle, handle);
    ASSERT_TRUE(ctx.hasV2Ext) << "v2Ext must be attached on a successful 0x2A";
    EXPECT_EQ(ctx.v2Ext.subInterval, 0x190u);
    EXPECT_EQ(ctx.v2Ext.maxSduMToS, 0x0123);
    EXPECT_EQ(ctx.v2Ext.maxSduSToM, 0x0045);
    EXPECT_EQ(ctx.v2Ext.sduIntervalMToS, 0x100FFu);
    EXPECT_EQ(ctx.v2Ext.sduIntervalSToM, 0x10u);
    EXPECT_EQ(ctx.v2Ext.framing, 0x01);

    // A late [v1] 0x19 success for the already-established handle is a duplicate: the
    // dedup drops it before the notify (iso_cig.c IsoCisEstablishedBegin), so the upper
    // layer still sees exactly one cisEstablished. A repeated v2 behaves the same.
    HciLeCisEstablishedEventParam est = {};
    est.status = 0x00;
    est.connectionHandle = handle;
    IsoLeCisEstablishedEvent(&est);
    EXPECT_EQ(ctx.establishedCount, 1u) << "v1 duplicate of a v2-established CIS must be deduped";
    IsoLeCisEstablishedEventV2(&p);
    EXPECT_EQ(ctx.establishedCount, 1u) << "repeated 0x2A must be deduped too";

    // One 0x05 removes the single tracking entry and reports once; a second is silent,
    // which is the observable proof that the duplicates kept only one entry.
    HciDisconnectCompleteEventParam disc = {};
    disc.status = 0x00;
    disc.connectionHandle = handle;
    disc.reason = 0x16;  // Connection Terminated by Local Host
    IsoLeDisconnectComplete(&disc);
    EXPECT_EQ(ctx.disconnectedCount, 1u);
    EXPECT_EQ(ctx.discHandle, handle);
    EXPECT_EQ(ctx.discReason, 0x16);
    IsoLeDisconnectComplete(&disc);
    EXPECT_EQ(ctx.disconnectedCount, 1u) << "entry is removed after the disconnect";
}

// @tc.number: StackGapLe6_0_IsoalCisEstablishedV2MergeV1First_00601
// @tc.name:  v1 success first, then a v2 duplicate: still exactly one notify
// @tc.desc:  Cross-version dedup works in both orders (0x2A then 0x19, and here 0x19 then
//            0x2A); the [v2] tail of the duplicate is consumed by the drop path.
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_IsoalCisEstablishedV2MergeV1First_00601, TestSize.Level1)
{
    constexpr uint16_t handle = 0x0043;
    IsoCisCtx ctx;
    CigCallbackGuard guard(&ctx);
    ASSERT_EQ(guard.registered, BT_SUCCESS);

    HciLeCisEstablishedEventParam est = {};
    est.status = 0x00;
    est.connectionHandle = handle;
    IsoLeCisEstablishedEvent(&est);
    EXPECT_EQ(ctx.establishedCount, 1u);
    EXPECT_FALSE(ctx.hasV2Ext) << "a 0x19 delivery never carries v2Ext";

    // The same handle arriving as a successful 0x2A is the duplicate here: dropped, and
    // the v2 tail is never surfaced.
    HciLeCisEstablishedV2EventParam p = {};
    p.status = 0x00;
    p.connectionHandle = handle;
    p.subInterval[0] = 0x90;
    p.subInterval[1] = 0x01;
    p.framing = 0x01;
    IsoLeCisEstablishedEventV2(&p);
    EXPECT_EQ(ctx.establishedCount, 1u) << "v2 duplicate of a v1-established CIS must be deduped";

    HciDisconnectCompleteEventParam disc = {};
    disc.status = 0x00;
    disc.connectionHandle = handle;
    disc.reason = 0x16;
    IsoLeDisconnectComplete(&disc);
    EXPECT_EQ(ctx.disconnectedCount, 1u);
    IsoLeDisconnectComplete(&disc);
    EXPECT_EQ(ctx.disconnectedCount, 1u);
}

// @tc.number: StackGapLe6_0_IsoalBigInfoReportIso_00750
// @tc.name:  BIGInfo Framing=0x02 reaches the upper layer (iso path)
// @tc.desc:  Drive the iso BIG callback seam with a report whose framing is 0x02; the
//            existing passthrough must not gate the value (Vol 4 Part E 7.7.65,34 p2415).
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_IsoalBigInfoReportIso_00750, TestSize.Level1)
{
    IsoBigInfoCtx ctx;
    BigCallbackGuard guard(&ctx);
    ASSERT_EQ(guard.registered, BT_SUCCESS);

    HciLeBigInfoAdvertisingReportEventParam param = {};
    param.syncHandle = 0x0042;
    param.numBis = 0x02;
    param.nse = 0x02;
    param.isoInterval = 0x0010;
    param.framing = 0x02;  // ISO_LE_FRAMING_UNSEGMENTED
    IsoLeBigInfoAdvertisingReportEvent(&param);

    EXPECT_EQ(ctx.count, 1u);
    EXPECT_EQ(ctx.syncHandle, 0x0042);
    EXPECT_EQ(ctx.numBis, 0x02);
    EXPECT_EQ(ctx.framing, 0x02) << "framing 0x02 must pass through un-gated";
}

// @tc.number: StackGapLe6_0_IsoalFramingGateRows_00500
// @tc.name:  IsoIsFramingRequestAllowed admission rows
// @tc.desc:  0x00/0x01 always admitted; 0x03/0xFF always rejected; 0x02 mirrors the local
//            controller bit 45 (Vol 6 Part B 4.6.44).
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_IsoalFramingGateRows_00500, TestSize.Level1)
{
    EXPECT_TRUE(IsoIsFramingRequestAllowed(ISO_LE_FRAMING_UNFRAMED));
    EXPECT_TRUE(IsoIsFramingRequestAllowed(ISO_LE_FRAMING_SEGMENTABLE));
    EXPECT_FALSE(IsoIsFramingRequestAllowed(0x03));
    EXPECT_FALSE(IsoIsFramingRequestAllowed(0xFF));
    // The 0x02 branch is controller-dependent, so assert the consistency with the getter
    // rather than an absolute value: on a pre-6.0 controller both are false, on a UFM
    // controller both are true.
    EXPECT_EQ(IsoIsFramingRequestAllowed(ISO_LE_FRAMING_UNSEGMENTED),
        BTM_IsControllerSupportLeUnsegmentedFramedMode());
}

// Drive ISOIF_LeCreateCig with the given framing and return the admission result. A
// non-rejected create is undone so later cases start from a clean controller state.
static int ExpectCigFraming(uint8_t framing, uint8_t cigId, const IsoLeCisParam &cisParam)
{
    IsoLeCigParam cigParam = {};
    cigParam.sduIntervalMToS = 0x2710;
    cigParam.sduIntervalSToM = 0x2710;
    cigParam.slaveClockAccuracy = 0x00;
    cigParam.packing = 0x01;
    cigParam.framing = framing;
    cigParam.maxTransportLatencyMToS = 0x000A;
    cigParam.maxTransportLatencySToM = 0x000A;
    int ret = ISOIF_LeCreateCig(cigId, &cigParam, 1, &cisParam);
    if (ret != BT_BAD_PARAM) {
        (void)ISOIF_LeRemoveCig(cigId);
    }
    return ret;
}

// Drive ISOIF_LeCreateBig with the given framing and return the admission result. A
// non-rejected create is terminated so later cases start from a clean controller state.
static int ExpectBigFraming(uint8_t framing, uint8_t bigHandle)
{
    IsoLeBigParam bigParam = {};
    bigParam.sduInterval = 0x2710;
    bigParam.maxSdu = 0x0100;
    bigParam.maxTransportLatency = 0x000A;
    bigParam.rtn = 0x02;
    bigParam.phy = 0x02;
    bigParam.packing = 0x00;
    bigParam.framing = framing;
    bigParam.encryption = 0x00;
    int ret = ISOIF_LeCreateBig(bigHandle, 0xE0, 1, &bigParam, nullptr);
    if (ret != BT_BAD_PARAM) {
        (void)ISOIF_LeTerminateBig(bigHandle, 0x13);
    }
    return ret;
}

// Drive ISOIF_LeCreateBigTest with the two framing values the gate distinguishes. If the
// local Controller supports UFM, the 0x02 variant is admitted and has to be terminated.
static void ExpectBigTestFraming(bool ufm)
{
    IsoLeBigTestParam bigTest = {};
    bigTest.sduInterval = 0x2710;
    bigTest.isoInterval = 0x0004;
    bigTest.nse = 0x02;
    bigTest.maxSdu = 0x0100;
    bigTest.maxPdu = 0x00FB;
    bigTest.phy = 0x02;
    bigTest.packing = 0x00;
    bigTest.framing = 0x03;
    bigTest.bn = 0x01;
    bigTest.irc = 0x01;
    bigTest.pto = 0x00;
    bigTest.encryption = 0x00;
    EXPECT_EQ(ISOIF_LeCreateBigTest(0xE3, 0xE0, 1, &bigTest, nullptr), BT_BAD_PARAM);
    bigTest.framing = ISO_LE_FRAMING_UNSEGMENTED;
    int ret = ISOIF_LeCreateBigTest(0xE3, 0xE0, 1, &bigTest, nullptr);
    if (ufm) {
        EXPECT_NE(ret, BT_BAD_PARAM);
    } else {
        EXPECT_EQ(ret, BT_BAD_PARAM) << "framing 0x02 needs a UFM-capable controller";
    }
    if (ret != BT_BAD_PARAM) {
        (void)ISOIF_LeTerminateBig(0xE3, 0x13);
    }
}

// @tc.number: StackGapLe6_0_IsoalFramingGateAssembly_00501
// @tc.name:  CIG / BIG / BIG Test assemblies enforce the framing admission gate
// @tc.desc:  framing 0x03/0xFF is rejected with BT_BAD_PARAM before any HCI command; the
//            0x02 branch depends on the local bit 45 (0x00/0x01 stay the 5.2 baseline).
HWTEST_F(StackGapLe60BtmTest, StackGapLe6_0_IsoalFramingGateAssembly_00501, TestSize.Level1)
{
    const bool ufm = BTM_IsControllerSupportLeUnsegmentedFramedMode();

    IsoLeCisParam cisParam = {};
    cisParam.cisId = 0x00;
    cisParam.maxSduMToS = 0x0100;
    cisParam.maxSduSToM = 0x0100;
    cisParam.phyMToS = 0x02;
    cisParam.phySToM = 0x02;
    cisParam.rtnMToS = 0x02;
    cisParam.rtnSToM = 0x02;
    EXPECT_EQ(ExpectCigFraming(0x03, 0xE1, cisParam), BT_BAD_PARAM);
    EXPECT_EQ(ExpectCigFraming(0xFF, 0xE2, cisParam), BT_BAD_PARAM);
    EXPECT_NE(ExpectCigFraming(0x00, 0xE3, cisParam), BT_BAD_PARAM);  // 5.2 baseline stays admitted
    EXPECT_NE(ExpectCigFraming(0x01, 0xE4, cisParam), BT_BAD_PARAM);
    int retCig02 = ExpectCigFraming(ISO_LE_FRAMING_UNSEGMENTED, 0xE5, cisParam);
    if (ufm) {
        EXPECT_NE(retCig02, BT_BAD_PARAM);
    } else {
        EXPECT_EQ(retCig02, BT_BAD_PARAM) << "framing 0x02 needs a UFM-capable controller";
    }

    // LE Create BIG: same admission rule, distinct handle so the cleanups do not collide.
    EXPECT_EQ(ExpectBigFraming(0x03, 0xE1), BT_BAD_PARAM);
    EXPECT_NE(ExpectBigFraming(0x00, 0xE2), BT_BAD_PARAM);

    // LE Create BIG Test: framing validated identically on the test-mode payload.
    ExpectBigTestFraming(ufm);
}


}  // namespace Bluetooth
}  // namespace OHOS
