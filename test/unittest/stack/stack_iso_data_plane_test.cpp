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

// ISO data plane core-logic tests (BT 5.2 stack).
// Coverage targets:
//   1) RX reassembly state machine : IsoSduRxOnPacket / IsoSduRxResetState (self-contained,
//      per-connection, driven without a live CIS)
//   2) TX fragmentation builder   : IsoIsoDataSegmentCount / IsoBuildIsoDataFragments
// These cases were split out of stack_gap_le_5_2_test.cpp so both files stay below the
// source-file line limit (they run in the same btfw_stack_unit_test binary, whose main()
// lives in stack_gap_le_interact_test.cpp). The public entry points that need a real CIS +
// HDI transport (ISOIF_LeSendIsoData / sduReceivedInd) stay in stack_gap_le_5_2_test.cpp.

#include <gtest/gtest.h>

#include <cstring>

#include "btstack.h"
#include "securec.h"
#include "src/iso/iso.h"

using namespace testing::ext;
namespace OHOS {
namespace Bluetooth {

// ===================== ISO data plane core-logic cases (plan chapter: stack data plane) =====================
// These drive the self-contained SDU reassembly state machine (IsoSduRxOnPacket) and the
// HCI ISO data fragmentation builder (IsoBuildIsoDataFragments / IsoIsoDataSegmentCount)
// directly. They are pure functions with no dependency on a live CIS, HDI, or peer, so
// they run single-device. The public entry points (ISOIF_LeSendIsoData / sduReceivedInd)
// that need a real CIS + HDI transport are deferred to the two-device cases (see the
// "后期待做" notes in the plan document).
//
// Framing rules under test (Vol 4, Part E, 5.4.5):
//   - ISO_Data_Load header (PB 0b00/0b10 only): [TS 4B if TS_Flag] + PSN 2B +
//     ISO_SDU_Length 2B (top 2 bits = Packet_Status_Flag).
//   - PB_Flag: 0b00 first / 0b01 continuation / 0b10 complete SDU / 0b11 last fragment.
//   - ISO_SDU_Length is the whole SDU length (12-bit field, max 0x0FFF).
namespace {
constexpr uint8_t PB_FIRST_FRAGMENT = 0x00;
constexpr uint8_t PB_CONTINUATION = 0x01;
constexpr uint8_t PB_COMPLETE_SDU = 0x02;
constexpr uint8_t PB_LAST_FRAGMENT = 0x03;

// ISO_Data_Load header size of a first/complete fragment without (with) the Time_Stamp,
// in octets (Vol 4, Part E, 5.4.5, Figure 5.6): [TS 4B if TS_Flag][PSN 2B][ISO_SDU_Length 2B].
constexpr uint16_t ISO_DATA_LOAD_HEADER_LEN_NO_TS = ISO_UINT16_BYTES + ISO_UINT16_BYTES;
constexpr uint16_t ISO_DATA_LOAD_HEADER_LEN_TS =
    ISO_UINT32_BYTES + ISO_UINT16_BYTES + ISO_UINT16_BYTES;

// Parameters of one ISO_Data_Load to serialize with IsoTestBuildLoad; the member order
// mirrors the arguments of the former BuildLoad helper of stack_gap_le_5_2_test.cpp.
struct IsoTestSduLoad {
    uint32_t timestamp;   // Time_Stamp (us); written only when tsFlag is set
    uint8_t tsFlag;       // whether the load carries the Time_Stamp field
    uint16_t seqNum;      // Packet_Sequence_Number of the SDU
    uint16_t sduLength;   // ISO_SDU_Length of the whole SDU
    uint8_t packetStatus; // Packet_Status_Flag of the SDU (Table 5.2)
    const uint8_t *data;  // payload bytes, dataLen of them; NULL when dataLen is 0
    uint16_t dataLen;     // payload size
};

// Serialize [TS 4B if tsFlag][PSN 2B][ISO_SDU_Length 2B with PSF] + payload into dst and
// return the load length in bytes.
uint16_t IsoTestBuildLoad(uint8_t *dst, const IsoTestSduLoad *load)
{
    uint16_t offset = 0;
    if (load->tsFlag) {
        for (uint8_t i = 0; i < ISO_UINT32_BYTES; i++) {
            dst[i] = static_cast<uint8_t>((load->timestamp >> (ISO_UINT8_BITS * i)) & 0xFF);
        }
        offset = ISO_UINT32_BYTES;
    }
    uint16_t lenField = static_cast<uint16_t>((load->sduLength & ISO_SDU_LENGTH_MAX) |
        ((load->packetStatus & ISO_SDU_PACKET_STATUS_MASK) << ISO_SDU_PACKET_STATUS_SHIFT));
    IsoWriteUint16(dst + offset, load->seqNum);
    IsoWriteUint16(dst + offset + ISO_UINT16_BYTES, lenField);
    if (load->dataLen > 0) {
        (void)memcpy_s(dst + offset + ISO_UINT16_BYTES + ISO_UINT16_BYTES, load->dataLen, load->data,
            load->dataLen);
    }
    return static_cast<uint16_t>(offset + ISO_UINT16_BYTES + ISO_UINT16_BYTES + load->dataLen);
}

class StackGapLe52IsoDataPlaneTest : public testing::Test {
public:
    void SetUp() override
    {
        (void)memset_s(&m_state, sizeof(m_state), 0x00, sizeof(m_state));
    }

protected:
    IsoSduRxState m_state;
    IsoSduRxResult m_result;
    uint8_t m_load[128];
};
} // namespace

// --- RX: single complete SDU (PB 0b10, no TS) ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_RxCompleteSduNoTs, TestSize.Level1)
{
    uint8_t data[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    IsoTestSduLoad loadToWrite = { 0, 0, 0x1234, sizeof(data), 0, data, sizeof(data) };
    uint16_t loadLen = IsoTestBuildLoad(m_load, &loadToWrite);

    IsoSduFragmentParam fragment = { PB_COMPLETE_SDU, 0, m_load, loadLen };
    int ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_SUCCESS);
    EXPECT_TRUE(m_result.sduComplete);
    EXPECT_EQ(m_result.seqNum, 0x1234);
    EXPECT_EQ(m_result.timestamp, 0);
    EXPECT_EQ(m_result.packetStatus, 0);
    EXPECT_EQ(m_result.length, sizeof(data));
    EXPECT_EQ(memcmp(m_result.data, data, sizeof(data)), 0);

    // State stays idle after delivery; nothing is held across packets.
    EXPECT_FALSE(m_state.inProgress);
    IsoSduRxResetState(&m_state);
}

// --- RX: fragmented SDU (PB 0b00 -> 0b01 -> 0b11), reassembled in order ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_RxFragmentedSdu, TestSize.Level1)
{
    uint8_t payload[17];
    for (size_t i = 0; i < sizeof(payload); i++) {
        payload[i] = static_cast<uint8_t>(i + 1);
    }
    const uint16_t sduLen = sizeof(payload);
    uint8_t part0[8];
    uint8_t part1[8];
    uint8_t part2[1];
    (void)memcpy_s(part0, sizeof(part0), payload, sizeof(part0));
    (void)memcpy_s(part1, sizeof(part1), payload + 8, sizeof(part1));
    (void)memcpy_s(part2, sizeof(part2), payload + 16, sizeof(part2));

    IsoTestSduLoad loadToWrite = { 0, 0, 0x0007, sduLen, 0, part0, sizeof(part0) };
    uint16_t loadLen = IsoTestBuildLoad(m_load, &loadToWrite);
    IsoSduFragmentParam fragment = { PB_FIRST_FRAGMENT, 0, m_load, loadLen };
    int ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_SUCCESS);
    EXPECT_FALSE(m_result.sduComplete);

    fragment = { PB_CONTINUATION, 0, part1, sizeof(part1) };
    ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_SUCCESS);
    EXPECT_FALSE(m_result.sduComplete);

    fragment = { PB_LAST_FRAGMENT, 0, part2, sizeof(part2) };
    ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_SUCCESS);
    EXPECT_TRUE(m_result.sduComplete);
    EXPECT_EQ(m_result.seqNum, 0x0007);
    EXPECT_EQ(m_result.length, sduLen);
    EXPECT_EQ(memcmp(m_result.data, payload, sduLen), 0);

    IsoSduRxResetState(&m_state);
    EXPECT_FALSE(m_state.inProgress);
}

// --- RX: TS_Flag carries the 4-byte Time_Stamp through the state machine ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_RxTimestampPassthrough, TestSize.Level1)
{
    uint8_t data[] = { 0xAA, 0xBB };
    const uint32_t ts = 0x12345678;
    IsoTestSduLoad loadToWrite = { ts, 1, 0x0022, sizeof(data), 0, data, sizeof(data) };
    uint16_t loadLen = IsoTestBuildLoad(m_load, &loadToWrite);

    IsoSduFragmentParam fragment = { PB_COMPLETE_SDU, 1, m_load, loadLen };
    int ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_SUCCESS);
    EXPECT_TRUE(m_result.sduComplete);
    EXPECT_EQ(m_result.timestamp, ts);
    EXPECT_EQ(m_result.length, sizeof(data));

    IsoSduRxResetState(&m_state);
}

// --- RX: PSF 0b10 (lost SDU) on a complete packet yields an empty delivery ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_RxLostPacketStatus, TestSize.Level1)
{
    IsoTestSduLoad loadToWrite = { 0, 0, 0x0009, 0, 0x02, nullptr, 0 };
    uint16_t loadLen = IsoTestBuildLoad(m_load, &loadToWrite);

    IsoSduFragmentParam fragment = { PB_COMPLETE_SDU, 0, m_load, loadLen };
    int ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_SUCCESS);
    EXPECT_TRUE(m_result.sduComplete);
    EXPECT_EQ(m_result.packetStatus, 0x02);
    EXPECT_EQ(m_result.length, 0);
    EXPECT_EQ(m_result.data, nullptr);

    IsoSduRxResetState(&m_state);
}

// --- RX: PSF 0b10 carrying self-consistent data is reported with LOST, not rejected ---
// A compliant sender never sends this (Vol 4, Part E, 5.4.5), but a non-compliant
// peer must not tear reassembly down; the LOST flag tells the upper layer to discard.

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_RxLostPacketStatusWithData, TestSize.Level1)
{
    uint8_t sduData[] = { 0x11, 0x22, 0x33, 0x44 };
    IsoTestSduLoad loadToWrite = { 0, 0, 0x0009, sizeof(sduData), 0x02, sduData, sizeof(sduData) };
    uint16_t loadLen = IsoTestBuildLoad(m_load, &loadToWrite);

    IsoSduFragmentParam fragment = { PB_COMPLETE_SDU, 0, m_load, loadLen };
    int ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_SUCCESS);
    EXPECT_TRUE(m_result.sduComplete);
    EXPECT_EQ(m_result.packetStatus, 0x02);
    EXPECT_EQ(m_result.length, sizeof(sduData));
    ASSERT_NE(m_result.data, nullptr);
    EXPECT_EQ(memcmp(m_result.data, sduData, sizeof(sduData)), 0);

    IsoSduRxResetState(&m_state);
}

// --- RX: continuation without a preceding first fragment is rejected ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_RxContinuationWithoutFirst, TestSize.Level1)
{
    uint8_t part[] = { 0x01, 0x02, 0x03 };
    IsoSduFragmentParam fragment = { PB_CONTINUATION, 0, part, sizeof(part) };
    int ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_BAD_PARAM);
    EXPECT_FALSE(m_result.sduComplete);
    EXPECT_FALSE(m_state.inProgress);
}

// --- RX: continuation overflowing the declared SDU length is rejected ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_RxContinuationOverflow, TestSize.Level1)
{
    // sduLength = 4, first fragment carries 2 of them, so 3 more would overflow.
    uint8_t part0[2] = { 1, 2 };
    IsoTestSduLoad loadToWrite = { 0, 0, 0x0001, 4, 0, part0, sizeof(part0) };
    uint16_t loadLen = IsoTestBuildLoad(m_load, &loadToWrite);
    IsoSduFragmentParam fragment = { PB_FIRST_FRAGMENT, 0, m_load, loadLen };
    int ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_SUCCESS);

    uint8_t tooMuch[3] = { 0x05, 0x06, 0x07 };
    fragment = { PB_CONTINUATION, 0, tooMuch, sizeof(tooMuch) };
    ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_BAD_PARAM);
    EXPECT_FALSE(m_state.inProgress);
}

// --- RX: last fragment that under-fills the SDU is rejected ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_RxLastFragmentShort, TestSize.Level1)
{
    // sduLength = 7, first fragment carries 4, last fragment 2 -> 6 < 7 under-fills.
    uint8_t part0[4] = { 1, 2, 3, 4 };
    uint8_t part1[2] = { 5, 6 };
    IsoTestSduLoad loadToWrite = { 0, 0, 0x0002, 7, 0, part0, sizeof(part0) };
    uint16_t loadLen = IsoTestBuildLoad(m_load, &loadToWrite);
    IsoSduFragmentParam fragment = { PB_FIRST_FRAGMENT, 0, m_load, loadLen };
    int ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_SUCCESS);

    fragment = { PB_LAST_FRAGMENT, 0, part1, sizeof(part1) };
    ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_BAD_PARAM);
    EXPECT_FALSE(m_state.inProgress);
}

// --- RX: truncated first fragment (load shorter than the header) is rejected ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_RxTruncatedHeader, TestSize.Level1)
{
    IsoTestSduLoad loadToWrite = { 0, 0, 0x0003, 8, 0, nullptr, 0 };
    uint16_t loadLen = IsoTestBuildLoad(m_load, &loadToWrite);
    IsoSduFragmentParam fragment = { PB_FIRST_FRAGMENT, 0, m_load, loadLen - 1 };
    int ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_BAD_PARAM);
    EXPECT_FALSE(m_state.inProgress);
}

// --- RX: a 0b00 first fragment carrying the whole SDU is rejected (Table 5.1) ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_RxFirstFragmentFillsExact, TestSize.Level1)
{
    uint8_t payload[4] = { 1, 2, 3, 4 };
    IsoTestSduLoad loadToWrite = { 0, 0, 0x0005, sizeof(payload), 0, payload, sizeof(payload) };
    uint16_t loadLen = IsoTestBuildLoad(m_load, &loadToWrite);
    IsoSduFragmentParam fragment = { PB_FIRST_FRAGMENT, 0, m_load, loadLen };
    int ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_BAD_PARAM);
    EXPECT_FALSE(m_result.sduComplete);
    EXPECT_FALSE(m_state.inProgress);
}

// --- RX: a complete SDU arriving mid-reassembly restarts cleanly ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_RxCompleteSduMidReassembly, TestSize.Level1)
{
    uint8_t part0[3] = { 1, 2, 3 };
    IsoTestSduLoad loadToWrite = { 0, 0, 0x0004, 6, 0, part0, sizeof(part0) };
    uint16_t loadLen = IsoTestBuildLoad(m_load, &loadToWrite);
    IsoSduFragmentParam fragment = { PB_FIRST_FRAGMENT, 0, m_load, loadLen };
    int ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_SUCCESS);
    EXPECT_TRUE(m_state.inProgress);

    uint8_t whole[] = { 0x11, 0x22 };
    loadToWrite = { 0, 0, 0x0005, sizeof(whole), 0, whole, sizeof(whole) };
    loadLen = IsoTestBuildLoad(m_load, &loadToWrite);
    fragment = { PB_COMPLETE_SDU, 0, m_load, loadLen };
    ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_SUCCESS);
    EXPECT_TRUE(m_result.sduComplete);
    EXPECT_EQ(m_result.seqNum, 0x0005);
    EXPECT_EQ(m_result.length, sizeof(whole));

    IsoSduRxResetState(&m_state);
}

// --- RX: a continuation that fills the SDU is not delivered until the 0b11 ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_RxContinuationFillsExact, TestSize.Level1)
{
    uint8_t part0[4] = { 1, 2, 3, 4 };
    IsoTestSduLoad loadToWrite = { 0, 0, 0x0006, 8, 0, part0, sizeof(part0) };
    uint16_t loadLen = IsoTestBuildLoad(m_load, &loadToWrite);
    IsoSduFragmentParam fragment = { PB_FIRST_FRAGMENT, 0, m_load, loadLen };
    int ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_SUCCESS);
    EXPECT_FALSE(m_result.sduComplete);

    uint8_t part1[4] = { 5, 6, 7, 8 };
    fragment = { PB_CONTINUATION, 0, part1, sizeof(part1) };
    ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_SUCCESS);
    EXPECT_FALSE(m_result.sduComplete); // 0b01 that fills the SDU stays in progress
    EXPECT_TRUE(m_state.inProgress);

    fragment = { PB_LAST_FRAGMENT, 0, nullptr, 0 };
    ret = IsoSduRxOnPacket(&m_state, &fragment, &m_result);
    EXPECT_EQ(ret, BT_SUCCESS);
    EXPECT_TRUE(m_result.sduComplete); // the trailing 0b11 completes the SDU
    EXPECT_EQ(m_result.length, 8);
    uint8_t expected[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    EXPECT_EQ(memcmp(m_result.data, expected, sizeof(expected)), 0);

    IsoSduRxResetState(&m_state);
}

// --- TX: single-segment SDU (fits in one packet) -> PB 0b10, header carries the full length ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_TxSingleSegment, TestSize.Level1)
{
    uint8_t sdu[] = { 1, 2, 3, 4 };
    IsoIsoDataSegment segments[8];
    (void)memset_s(segments, sizeof(segments), 0, sizeof(segments));

    uint16_t count = IsoIsoDataSegmentCount(sizeof(sdu), 40, 0);
    EXPECT_EQ(count, 1);
    IsoLeSendIsoDataParam param = { 0 };
    param.length = sizeof(sdu);
    param.seqNum = 0x1000;
    int ret = IsoBuildIsoDataFragments(&param, 40, segments);
    EXPECT_EQ(ret, BT_SUCCESS);
    EXPECT_EQ(segments[0].pbFlag, PB_COMPLETE_SDU);
    EXPECT_EQ(segments[0].loadHeaderLength, ISO_DATA_LOAD_HEADER_LEN_NO_TS);
    EXPECT_EQ(segments[0].dataOffset, 0);
    EXPECT_EQ(segments[0].dataLength, sizeof(sdu));
}

// --- TX: multi-segment SDU -> PB pattern 0b00/0b01/0b11, ISO_SDU_Length is the whole SDU ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_TxMultiSegment, TestSize.Level1)
{
    const uint16_t sduLen = 60;
    const uint16_t maxPacket = 20; // 16 bytes of data per fragment (no TS)
    IsoIsoDataSegment segments[8];
    (void)memset_s(segments, sizeof(segments), 0, sizeof(segments));

    uint16_t count = IsoIsoDataSegmentCount(sduLen, maxPacket, 0);
    EXPECT_EQ(count, 4); // ceil(60 / 16)

    IsoLeSendIsoDataParam param = { 0 };
    param.length = sduLen;
    param.seqNum = 0xABCD;
    int ret = IsoBuildIsoDataFragments(&param, maxPacket, segments);
    EXPECT_EQ(ret, BT_SUCCESS);

    EXPECT_EQ(segments[0].pbFlag, PB_FIRST_FRAGMENT);
    for (uint16_t i = 1; i < count - 1; i++) {
        EXPECT_EQ(segments[i].pbFlag, PB_CONTINUATION);
    }
    EXPECT_EQ(segments[count - 1].pbFlag, PB_LAST_FRAGMENT);

    // Every first/complete fragment carries ISO_SDU_Length = whole SDU.
    EXPECT_EQ(segments[0].loadHeaderLength, ISO_DATA_LOAD_HEADER_LEN_NO_TS);
    EXPECT_EQ(segments[0].dataLength, 16);
    EXPECT_EQ(segments[1].dataOffset, 16);
    EXPECT_EQ(segments[1].loadHeaderLength, 0);    // continuation carries data only
    EXPECT_EQ(segments[count - 1].dataLength, 12); // 60 - 3 * 16
}

// --- TX: TS_Flag adds the 4-byte timestamp to the first/complete header ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_TxTimestampHeader, TestSize.Level1)
{
    uint8_t sdu[] = { 0x01 };
    const uint32_t ts = 0xDEADBEEF;
    IsoIsoDataSegment segments[8];
    (void)memset_s(segments, sizeof(segments), 0, sizeof(segments));

    uint16_t count = IsoIsoDataSegmentCount(sizeof(sdu), 30, 1);
    EXPECT_EQ(count, 1);
    IsoLeSendIsoDataParam param = { 0 };
    param.length = sizeof(sdu);
    param.timestamp = ts;
    param.timestampFlag = 1;
    int ret = IsoBuildIsoDataFragments(&param, 30, segments);
    EXPECT_EQ(ret, BT_SUCCESS);
    EXPECT_EQ(segments[0].pbFlag, PB_COMPLETE_SDU);
    EXPECT_EQ(segments[0].loadHeaderLength, ISO_DATA_LOAD_HEADER_LEN_TS);

    // The serialized header begins with the timestamp (LE).
    uint32_t encodedTs = (uint32_t)segments[0].loadHeader[0] | ((uint32_t)segments[0].loadHeader[1] << 8) |
        ((uint32_t)segments[0].loadHeader[2] << 16) | ((uint32_t)segments[0].loadHeader[3] << 24);
    EXPECT_EQ(encodedTs, ts);
}

// --- TX: multi-segment with TS_Flag: timestamp only in the first-fragment header ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_TxMultiSegmentTimestamp, TestSize.Level1)
{
    const uint16_t sduLen = 40;
    const uint16_t maxPacket = 20; // 12 data bytes per fragment (TS header takes 8)
    const uint32_t ts = 0xCAFEBABE;
    IsoIsoDataSegment segments[8];
    (void)memset_s(segments, sizeof(segments), 0, sizeof(segments));

    uint16_t count = IsoIsoDataSegmentCount(sduLen, maxPacket, 1);
    EXPECT_EQ(count, 4); // ceil(40 / 12)

    IsoLeSendIsoDataParam param = { 0 };
    param.length = sduLen;
    param.seqNum = 0x00FF;
    param.timestamp = ts;
    param.timestampFlag = 1;
    int ret = IsoBuildIsoDataFragments(&param, maxPacket, segments);
    EXPECT_EQ(ret, BT_SUCCESS);

    // First fragment carries the TS header; continuation/last carry data only.
    EXPECT_EQ(segments[0].pbFlag, PB_FIRST_FRAGMENT);
    EXPECT_EQ(segments[0].loadHeaderLength, ISO_DATA_LOAD_HEADER_LEN_TS);
    for (uint16_t i = 1; i < count; i++) {
        EXPECT_EQ(segments[i].loadHeaderLength, 0);
    }
    EXPECT_EQ(segments[0].dataLength, 12);
    EXPECT_EQ(segments[count - 1].dataLength, 4); // 40 - 3 * 12
}

// --- TX: zero-length SDU is a single empty complete packet ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_TxZeroLengthSdu, TestSize.Level1)
{
    IsoIsoDataSegment segments[8];
    (void)memset_s(segments, sizeof(segments), 0, sizeof(segments));

    uint16_t count = IsoIsoDataSegmentCount(0, 40, 0);
    EXPECT_EQ(count, 1);
    IsoLeSendIsoDataParam param = { 0 };
    int ret = IsoBuildIsoDataFragments(&param, 40, segments);
    EXPECT_EQ(ret, BT_SUCCESS);
    EXPECT_EQ(segments[0].pbFlag, PB_COMPLETE_SDU);
    EXPECT_EQ(segments[0].dataLength, 0);
}

// --- TX: maxPacketLength too small to hold the load header -> no segments ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_TxMaxPacketTooSmall, TestSize.Level1)
{
    EXPECT_EQ(IsoIsoDataSegmentCount(16, 3, 0), 0);
    EXPECT_EQ(IsoIsoDataSegmentCount(16, 3, 1), 0);

    IsoIsoDataSegment segments[8];
    IsoLeSendIsoDataParam param = { 0 };
    param.length = 0x0010;
    int ret = IsoBuildIsoDataFragments(&param, 3, segments);
    EXPECT_EQ(ret, BT_BAD_PARAM);
}

// --- TX: SDU length beyond the 12-bit ISO_SDU_Length field is rejected ---

HWTEST_F(StackGapLe52IsoDataPlaneTest, IsoDataPlane_TxSduLengthTooLarge, TestSize.Level1)
{
    IsoIsoDataSegment segments[8];
    IsoLeSendIsoDataParam param = { 0 };
    param.length = 0x4000;
    int ret = IsoBuildIsoDataFragments(&param, 40, segments);
    EXPECT_EQ(ret, BT_BAD_PARAM);
}

} // namespace Bluetooth
} // namespace OHOS

