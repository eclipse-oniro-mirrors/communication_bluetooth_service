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

#include "iso.h"

#include <securec.h>

#include "allocator.h"
#include "log.h"

#include "hci/hci.h"
#include "hci/hci_error.h"
#include "hci/iso/hci_iso.h"

// ISO_Data_Load header without / with Time_Stamp: [PSN 2 octets][ISO_SDU_Length 2 octets] /
// [TS 4 octets][PSN 2 octets][ISO_SDU_Length 2 octets]. Vol 4, Part E, 5.4.5, Figure 5.6.
// The lengths are composed from the byte-count constants of iso.h so the packing
// arithmetic below stays readable and cannot drift from the spec table.
#define ISO_DATA_LOAD_HEADER_LEN_NO_TS (ISO_UINT16_BYTES + ISO_UINT16_BYTES)
#define ISO_DATA_LOAD_HEADER_LEN_TS (ISO_UINT32_BYTES + ISO_DATA_LOAD_HEADER_LEN_NO_TS)

// Drop-counter cadence of untracked handles (see IsoOnIsoData): warn on the first
// drop and on every ISO_DROP_LOG_PERIODth drop thereafter.
#define ISO_DROP_LOG_PERIOD 0x400

#define PB_FIRST_FRAGMENT 0x00
#define PB_CONTINUATION   0x01
#define PB_COMPLETE_SDU   0x02
#define PB_LAST_FRAGMENT  0x03

// Packet_Status_Flag of a transmitted SDU; the ISO_Data_Load header field carries
// 0b10 for a lost SDU, which the reassembler reports as-is (see IsoSduRxDeliverComplete).
#define PACKET_STATUS_VALID 0x00

// Per-connection RX context, created on the first ISO data packet of a live stream.
typedef struct {
    uint16_t connectionHandle;
    IsoSduRxState state;
} IsoDataRxHandle;

static List *g_isoDataRxList = NULL;

// Drop counter for ISO data arriving on handles the ISO layer does not track
// (see IsoOnIsoData): the first drop and every 1024th are logged as warnings so
// an abnormal flood surfaces without per-packet log spam.
static uint32_t g_isoUntrackedDataDrops = 0;

static HciIsoCallbacks g_hciIsoCallbacks = {
    .onIsoData = IsoOnIsoData,
};

// Byte-ordered little-endian accessors. Written as byte loops over the shared
// ISO_UINTn_BYTES / ISO_UINT8_BITS constants (same shape as the iso.h inline
// helpers, Vol 2, Part E, 5.1.1) instead of shift chains with bare counts.
static uint16_t IsoReadUint16Le(const uint8_t *src)
{
    uint16_t value = 0;
    for (uint8_t i = 0; i < ISO_UINT16_BYTES; i++) {
        value |= (uint16_t)src[i] << (ISO_UINT8_BITS * i);
    }
    return value;
}

static uint32_t IsoReadUint32Le(const uint8_t *src)
{
    uint32_t value = 0;
    for (uint8_t i = 0; i < ISO_UINT32_BYTES; i++) {
        value |= (uint32_t)src[i] << (ISO_UINT8_BITS * i);
    }
    return value;
}

static void IsoWriteUint16Le(uint8_t *dst, uint16_t value)
{
    for (uint8_t i = 0; i < ISO_UINT16_BYTES; i++) {
        dst[i] = (uint8_t)((value >> (ISO_UINT8_BITS * i)) & 0xFF);
    }
}

static void IsoWriteUint32Le(uint8_t *dst, uint32_t value)
{
    for (uint8_t i = 0; i < ISO_UINT32_BYTES; i++) {
        dst[i] = (uint8_t)((value >> (ISO_UINT8_BITS * i)) & 0xFF);
    }
}

static void IsoWriteSduLengthField(uint8_t *dst, uint16_t sduLength, uint8_t packetStatus)
{
    uint16_t value = (sduLength & ISO_SDU_LENGTH_MAX) |
        ((uint16_t)(packetStatus & ISO_SDU_PACKET_STATUS_MASK) << ISO_SDU_PACKET_STATUS_SHIFT);
    IsoWriteUint16Le(dst, value);
}

void IsoSduRxResetState(IsoSduRxState *state)
{
    if (state == NULL) {
        return;
    }
    if (state->buffer != NULL) {
        MEM_MALLOC.free(state->buffer);
        state->buffer = NULL;
    }
    state->inProgress = false;
    state->sduLength = 0;
    state->receivedLength = 0;
    state->seqNum = 0;
    state->timestamp = 0;
    state->packetStatus = PACKET_STATUS_VALID;
}

// Parse the ISO_Data_Load header of a first/complete fragment: [TS if tsFlag] + PSN +
// ISO_SDU_Length (bits 14-15 Packet_Status_Flag), Vol 4, Part E, 5.4.5.
static void IsoSduRxParseHeader(const uint8_t *load, uint8_t tsFlag, IsoSduRxHeader *header)
{
    uint16_t offset = 0;
    if (tsFlag) {
        header->timestamp = IsoReadUint32Le(load);
        offset = ISO_UINT32_BYTES;
    } else {
        header->timestamp = 0;
    }
    header->seqNum = IsoReadUint16Le(load + offset);
    uint16_t sduLenRaw = IsoReadUint16Le(load + offset + ISO_UINT16_BYTES);
    header->sduLength = sduLenRaw & ISO_SDU_LENGTH_MAX;
    header->packetStatus =
        (uint8_t)((sduLenRaw >> ISO_SDU_PACKET_STATUS_SHIFT) & ISO_SDU_PACKET_STATUS_MASK);
}

static void IsoDeliverSduFromState(IsoSduRxState *state, IsoSduRxResult *result)
{
    result->sduComplete = true;
    result->seqNum = state->seqNum;
    result->timestamp = state->timestamp;
    result->packetStatus = state->packetStatus;
    result->data = state->buffer;
    result->length = state->sduLength;
}

// Deliver a 0b10 complete SDU straight from the packet payload. A packet marked
// lost (PSF 0b10) is reported, not rejected: the spec has the sender set
// ISO_SDU_Length to zero and send no data, so the report then carries data NULL
// and length 0; a non-conforming sender that fills the payload gets it delivered
// as-is with packetStatus 2. A length mismatch is malformed and rejected without
// any report (lost-SDU contract, see iso_le_if.h).
static int IsoSduRxDeliverComplete(const IsoSduRxHeader *header, const uint8_t *data, uint16_t dataLen,
    IsoSduRxResult *result)
{
    if (header->sduLength != dataLen) {
        return BT_BAD_PARAM;
    }
    result->sduComplete = true;
    result->seqNum = header->seqNum;
    result->timestamp = header->timestamp;
    result->packetStatus = header->packetStatus;
    result->data = dataLen == 0 ? NULL : data;
    result->length = header->sduLength;
    return BT_SUCCESS;
}

// Start a new fragmented SDU from a 0b00 first fragment; the caller already
// dropped any interrupted one.
static int IsoSduRxStartReassembly(IsoSduRxState *state, const IsoSduRxHeader *header, const uint8_t *data,
    uint16_t dataLen)
{
    // ISO_SDU_Length is already masked to 12 bits (IsoSduRxParseHeader), so only 0 is invalid.
    if (header->sduLength == 0) {
        return BT_BAD_PARAM;
    }
    if (dataLen >= header->sduLength) {
        // A 0b00 first fragment shall not carry the whole SDU; that is what
        // 0b10 is for (Vol 4, Part E, 5.4.5, Table 5.1).
        return BT_BAD_PARAM;
    }
    state->buffer = MEM_MALLOC.alloc(header->sduLength);
    if (state->buffer == NULL) {
        return BT_NO_MEMORY;
    }
    state->inProgress = true;
    state->sduLength = header->sduLength;
    state->seqNum = header->seqNum;
    state->timestamp = header->timestamp;
    state->packetStatus = header->packetStatus;
    if (dataLen > 0) {
        (void)memcpy_s(state->buffer, header->sduLength, data, dataLen);
    }
    state->receivedLength = dataLen;
    return BT_SUCCESS;
}

// Continue an in-progress SDU with a 0b01/0b11 fragment (data only); only the
// 0b11 last fragment terminates the SDU (Table 5.1). Malformed continuations
// drop the whole SDU and clear the state.
static int IsoSduRxContinueReassembly(IsoSduRxState *state, const IsoSduFragmentParam *fragment, uint16_t dataLen,
    IsoSduRxResult *result)
{
    if (!state->inProgress || state->buffer == NULL) {
        IsoSduRxResetState(state);
        return BT_BAD_PARAM;
    }
    uint16_t remaining = (uint16_t)(state->sduLength - state->receivedLength);
    if (dataLen > remaining) {
        // More data than the ISO_SDU_Length allows: malformed, drop the SDU.
        IsoSduRxResetState(state);
        return BT_BAD_PARAM;
    }
    if (dataLen > 0) {
        (void)memcpy_s(state->buffer + state->receivedLength, remaining, fragment->load, dataLen);
        state->receivedLength = (uint16_t)(state->receivedLength + dataLen);
    }
    if (fragment->pbFlag == PB_LAST_FRAGMENT) {
        if (state->receivedLength != state->sduLength) {
            // 0b11 marks the last fragment: an SDU that does not fill up by then is malformed.
            IsoSduRxResetState(state);
            return BT_BAD_PARAM;
        }
        // A 0b01 that filled the SDU stays in progress until the (empty) 0b11 arrives.
        IsoDeliverSduFromState(state, result);
    }
    return BT_SUCCESS;
}

int IsoSduRxOnPacket(IsoSduRxState *state, const IsoSduFragmentParam *fragment, IsoSduRxResult *result)
{
    if (state == NULL || fragment == NULL || result == NULL) {
        return BT_BAD_PARAM;
    }
    const uint8_t *load = fragment->load;
    uint16_t loadLength = fragment->loadLength;
    if (load == NULL && loadLength > 0) {
        return BT_BAD_PARAM;
    }
    (void)memset_s(result, sizeof(IsoSduRxResult), 0x00, sizeof(IsoSduRxResult));

    uint16_t dataLen = loadLength;
    const uint8_t *data = load;
    IsoSduRxHeader header;
    if (fragment->pbFlag == PB_FIRST_FRAGMENT || fragment->pbFlag == PB_COMPLETE_SDU) {
        uint16_t headerLen =
            fragment->tsFlag ? ISO_DATA_LOAD_HEADER_LEN_TS : ISO_DATA_LOAD_HEADER_LEN_NO_TS;
        if (loadLength < headerLen) {
            IsoSduRxResetState(state);
            return BT_BAD_PARAM;
        }
        IsoSduRxParseHeader(load, fragment->tsFlag, &header);
        data = load + headerLen;
        dataLen = (uint16_t)(loadLength - headerLen);
    }

    if (fragment->pbFlag == PB_COMPLETE_SDU) {
        // A complete SDU in one packet; deliver directly from the packet payload.
        IsoSduRxResetState(state);
        return IsoSduRxDeliverComplete(&header, data, dataLen, result);
    }
    if (fragment->pbFlag == PB_FIRST_FRAGMENT) {
        // Start a new fragmented SDU; an interrupted one is dropped.
        IsoSduRxResetState(state);
        return IsoSduRxStartReassembly(state, &header, data, dataLen);
    }
    // PB 0b01 continuation / 0b11 last fragment carry data only.
    return IsoSduRxContinueReassembly(state, fragment, dataLen, result);
}

uint16_t IsoIsoDataSegmentCount(uint16_t sduLength, uint16_t maxPacketLength, uint8_t tsFlag)
{
    uint16_t maxLoad = (maxPacketLength > ISO_DATA_LOAD_LENGTH_MAX) ? ISO_DATA_LOAD_LENGTH_MAX : maxPacketLength;
    uint16_t headerLen = tsFlag ? ISO_DATA_LOAD_HEADER_LEN_TS : ISO_DATA_LOAD_HEADER_LEN_NO_TS;
    if (maxLoad < headerLen) {
        return 0;
    }
    if (sduLength == 0) {
        return 1;
    }
    uint16_t capacity = (uint16_t)(maxLoad - headerLen);
    if (capacity == 0) {
        return 0;
    }
    return (uint16_t)((sduLength + capacity - 1) / capacity);
}

// PB_Flag of fragment |index| of |count| (Vol 4, Part E, 5.4.5, Table 5.1).
static uint8_t IsoPbFlagOfFragment(uint16_t count, uint16_t index)
{
    if (count == 1) {
        return PB_COMPLETE_SDU;
    }
    if (index == 0) {
        return PB_FIRST_FRAGMENT;
    }
    if (index == count - 1) {
        return PB_LAST_FRAGMENT;
    }
    return PB_CONTINUATION;
}

// Serialize the ISO_Data_Load header (TS + PSN + ISO_SDU_Length) of a first/complete
// fragment into segment->loadHeader; the TS field is present when timestampFlag is set.
static void IsoWriteSegmentHeader(IsoIsoDataSegment *segment, const IsoLeSendIsoDataParam *param)
{
    uint16_t headerOffset = 0;
    if (param->timestampFlag) {
        IsoWriteUint32Le(segment->loadHeader, param->timestamp);
        headerOffset = ISO_UINT32_BYTES;
    }
    IsoWriteUint16Le(segment->loadHeader + headerOffset, param->seqNum);
    IsoWriteSduLengthField(segment->loadHeader + headerOffset + ISO_UINT16_BYTES, param->length, PACKET_STATUS_VALID);
    segment->loadHeaderLength =
        (uint8_t)(param->timestampFlag ? ISO_DATA_LOAD_HEADER_LEN_TS : ISO_DATA_LOAD_HEADER_LEN_NO_TS);
}

int IsoBuildIsoDataFragments(const IsoLeSendIsoDataParam *param, uint16_t maxPacketLength,
    IsoIsoDataSegment *segmentList)
{
    if (param == NULL || segmentList == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->length > ISO_SDU_LENGTH_MAX) {
        return BT_BAD_PARAM;
    }
    if (param->timestampFlag > 1) {
        return BT_BAD_PARAM;
    }
    uint16_t count = IsoIsoDataSegmentCount(param->length, maxPacketLength, param->timestampFlag);
    if (count == 0) {
        return BT_BAD_PARAM;
    }

    uint16_t maxLoad = (maxPacketLength > ISO_DATA_LOAD_LENGTH_MAX) ? ISO_DATA_LOAD_LENGTH_MAX : maxPacketLength;
    uint16_t headerLen =
        param->timestampFlag ? ISO_DATA_LOAD_HEADER_LEN_TS : ISO_DATA_LOAD_HEADER_LEN_NO_TS;
    uint16_t capacity = (uint16_t)(maxLoad - headerLen);

    for (uint16_t i = 0; i < count; i++) {
        IsoIsoDataSegment *segment = &segmentList[i];
        uint16_t offset = (uint16_t)(i * capacity);
        uint16_t dataLength = (uint16_t)(param->length - offset);
        if (dataLength > capacity) {
            dataLength = capacity;
        }

        segment->pbFlag = IsoPbFlagOfFragment(count, i);
        if (segment->pbFlag == PB_FIRST_FRAGMENT || segment->pbFlag == PB_COMPLETE_SDU) {
            IsoWriteSegmentHeader(segment, param);
        } else {
            segment->loadHeaderLength = 0;
        }

        segment->dataOffset = offset;
        segment->dataLength = dataLength;
    }
    return BT_SUCCESS;
}

int IsoRegisterSduCallback(const IsoLeSduCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "NULL");
    IsoLeMng *mng = IsoGetMng();
    mng->sduCallback = callback;
    mng->sduCallbackContext = context;
    return BT_SUCCESS;
}

int IsoDeregisterSduCallback(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    IsoLeMng *mng = IsoGetMng();
    mng->sduCallback = NULL;
    mng->sduCallbackContext = NULL;
    return BT_SUCCESS;
}

void IsoLeReadBufferSizeV2Complete(const HciLeReadBufferSizeV2ReturnParam *param)
{
    LOG_INFO("%{public}s: status:0x%02x, isoDataPacketLength:0x%04x, totalNumIsoDataPackets:0x%02x", __FUNCTION__,
        param->status, param->hcLeIsoDataPacketLength, param->hcTotalNumLeIsoDataPackets);
    IsoLeMng *mng = IsoGetMng();
    if (param->status != HCI_SUCCESS) {
        // Command failed (unsupported controller); keep the data plane unavailable.
        mng->isoDataPacketLength = 0;
        HciIsoSetIsoDataPackets(0);
        return;
    }
    mng->isoDataPacketLength = param->hcLeIsoDataPacketLength;
    // Seed the HCI ISO flow control credit with the Controller's buffer count.
    HciIsoSetIsoDataPackets(param->hcTotalNumLeIsoDataPackets);
}

static bool IsoFindCisByHandle(void *data, void *parameter)
{
    IsoCisInfo *cisInfo = data;
    uint16_t cisHandle = *(uint16_t *)parameter;
    return cisInfo->cisHandle == cisHandle;
}

static bool IsoFindBisHandleInBigList(List *bigList, uint16_t connectionHandle)
{
    IsoBigInfo *bigInfo = NULL;
    ListNode *node = ListGetFirstNode(bigList);
    while (node != NULL) {
        bigInfo = ListGetNodeData(node);
        for (uint8_t i = 0; i < bigInfo->bisCount; i++) {
            if (bigInfo->bisHandles[i] == connectionHandle) {
                return true;
            }
        }
        node = ListGetNextNode(node);
    }
    return false;
}

static bool IsoIsIsoHandleActive(uint16_t connectionHandle)
{
    IsoLeMng *mng = IsoGetMng();
    if (ListForEachData(mng->cisList, IsoFindCisByHandle, &connectionHandle) != NULL) {
        return true;
    }
    // 广播 BIG(bigList)与同步 BIG(syncList)的 BIS handles 都是活跃 ISO 数据 handle
    if (IsoFindBisHandleInBigList(mng->bigBlock.bigList, connectionHandle)) {
        return true;
    }
    if (IsoFindBisHandleInBigList(mng->bigBlock.syncList, connectionHandle)) {
        return true;
    }
    return false;
}

static bool IsoFindDataRxByHandle(void *data, void *parameter)
{
    IsoDataRxHandle *rx = data;
    uint16_t handle = *(uint16_t *)parameter;
    return rx->connectionHandle == handle;
}

static IsoDataRxHandle *IsoFindDataRx(uint16_t connectionHandle)
{
    return ListForEachData(g_isoDataRxList, IsoFindDataRxByHandle, &connectionHandle);
}

static void IsoFreeDataRxHandle(void *data)
{
    IsoDataRxHandle *rx = data;
    if (rx != NULL) {
        IsoSduRxResetState(&rx->state);
        MEM_MALLOC.free(rx);
    }
}

void IsoDataRemoveRxContext(uint16_t connectionHandle)
{
    IsoDataRxHandle *rx = IsoFindDataRx(connectionHandle);
    if (rx != NULL) {
        ListRemoveNode(g_isoDataRxList, rx);
    }
}

static void IsoNotifySduReceived(IsoDataRxHandle *rx, const IsoSduRxResult *result)
{
    IsoLeMng *mng = IsoGetMng();
    if (mng->sduCallback != NULL && mng->sduCallback->sduReceivedInd != NULL) {
        // Delivered per callback contract: the info (and its data pointer) are only
        // valid for the duration of the call; the upper layer copies what it keeps.
        IsoLeSduReceivedInfo info = {
            .connectionHandle = rx->connectionHandle,
            .seqNum = result->seqNum,
            .timestamp = result->timestamp,
            .packetStatus = result->packetStatus,
            .data = result->data,
            .length = result->length,
        };
        mng->sduCallback->sduReceivedInd(&info, mng->sduCallbackContext);
    }
}

void IsoOnIsoData(uint16_t handle, uint8_t pbFlag, uint8_t tsFlag, Packet *packet)
{
    if (!IsoIsEnable()) {
        return;
    }
    if (!IsoIsIsoHandleActive(handle)) {
        // ISO data for a handle the ISO layer does not track: either packets
        // arriving in the short window before the CIS/BIG-established event ran,
        // or a data plane that never became trackable (e.g. the established event
        // itself was dropped from a full ISO queue). The former is a few packets
        // per connection; the latter floods forever while staying silent at
        // LOG_DEBUG. Count both, and warn on the first drop and on every
        // ISO_DROP_LOG_PERIODth so a permanently dead data plane still surfaces.
        uint32_t drops = ++g_isoUntrackedDataDrops;
        if (drops == 1 || (drops % ISO_DROP_LOG_PERIOD) == 0) {
            LOG_WARN("%{public}s: ISO data dropped for untracked handle: 0x%04x, total drops: %u", __FUNCTION__,
                handle, drops);
        }
        return;
    }

    IsoDataRxHandle *rx = IsoFindDataRx(handle);
    if (rx == NULL) {
        rx = MEM_MALLOC.alloc(sizeof(IsoDataRxHandle));
        if (rx == NULL) {
            return;
        }
        (void)memset_s(rx, sizeof(IsoDataRxHandle), 0x00, sizeof(IsoDataRxHandle));
        rx->connectionHandle = handle;
        if (!ListAddLast(g_isoDataRxList, rx)) {
            // List node allocation failed: the rx context cannot be tracked and
            // would never be freed, and a later packet would allocate another
            // one that fails the same way - a leak per packet. Drop the packet
            // and free the context now.
            LOG_WARN("%{public}s: cannot track rx context for handle: 0x%04x, dropping", __FUNCTION__, handle);
            MEM_MALLOC.free(rx);
            return;
        }
    }

    uint32_t loadSize = PacketPayloadSize(packet);
    Buffer *payload = PacketContinuousPayload(packet);
    uint8_t *load = (payload != NULL) ? (uint8_t *)BufferPtr(payload) : NULL;
    if (loadSize > 0 && load == NULL) {
        return;
    }

    IsoSduRxResult result;
    IsoSduFragmentParam fragment = {
        .pbFlag = pbFlag,
        .tsFlag = tsFlag,
        .load = load,
        .loadLength = (uint16_t)loadSize,
    };
    int ret = IsoSduRxOnPacket(&rx->state, &fragment, &result);
    if (ret != BT_SUCCESS || !result.sduComplete) {
        // Malformed packet already reset the state; a partial SDU stays in progress.
        return;
    }
    IsoNotifySduReceived(rx, &result);
    IsoSduRxResetState(&rx->state);
}

// Size the SDU into segments against the Controller's ISO data packet length and
// build the segment list (IsoBuildIsoDataFragments). Returns the segment count in
// |count| and the allocated list in |segments|; the caller frees the list.
static int IsoPrepareSegments(const IsoLeSendIsoDataParam *param, uint16_t *count, IsoIsoDataSegment **segments)
{
    IsoLeMng *mng = IsoGetMng();
    if (mng->isoDataPacketLength == 0) {
        // 0x0060 reported no dedicated ISO buffer: controller-offload, data plane unavailable.
        return BT_OPERATION_FAILED;
    }

    uint16_t segmentCount = IsoIsoDataSegmentCount(param->length, mng->isoDataPacketLength, param->timestampFlag);
    if (segmentCount == 0) {
        return BT_OPERATION_FAILED;
    }
    IsoIsoDataSegment *segmentList = MEM_MALLOC.alloc(sizeof(IsoIsoDataSegment) * segmentCount);
    if (segmentList == NULL) {
        return BT_NO_MEMORY;
    }
    int ret = IsoBuildIsoDataFragments(param, mng->isoDataPacketLength, segmentList);
    if (ret != BT_SUCCESS) {
        MEM_MALLOC.free(segmentList);
        return ret;
    }

    *count = segmentCount;
    *segments = segmentList;
    return BT_SUCCESS;
}

// Build every fragment packet before sending any, so an allocation failure cannot
// leave a partial SDU behind (Vol 4, Part E, 4.1.1). On failure the caller frees
// packets[0, *built): later slots are untouched.
static int IsoBuildFragmentPackets(const IsoLeSendIsoDataParam *param, uint16_t count,
    const IsoIsoDataSegment *segments, Packet **packets, uint16_t *built)
{
    for (uint16_t i = 0; i < count; i++) {
        const IsoIsoDataSegment *segment = &segments[i];
        uint16_t loadLength = (uint16_t)(segment->loadHeaderLength + segment->dataLength);
        Packet *packet = PacketMalloc(0, 0, loadLength);
        if (packet == NULL) {
            return BT_NO_MEMORY;
        }
        if (segment->loadHeaderLength > 0) {
            (void)PacketPayloadWrite(packet, segment->loadHeader, 0, segment->loadHeaderLength);
        }
        if (segment->dataLength > 0) {
            (void)PacketPayloadWrite(packet, param->data + segment->dataOffset, segment->loadHeaderLength,
                segment->dataLength);
        }
        packets[i] = packet;
        (*built)++;
    }
    return BT_SUCCESS;
}

// Send the fully built fragment packets. TS_Flag applies only to fragments
// carrying a Time_Stamp field, i.e. the first (0b00) or complete (0b10) fragment
// (Table 5.1). On a mid-way enqueue failure the already queued fragments [0, i)
// stay as an incomplete SDU (see the credit pre-check note in IsoLeSendIsoData):
// the rest are dropped here and the failure is reported so the upper layer
// retries the whole SDU, never resuming from the middle.
static int IsoSendFragmentPackets(uint16_t connectionHandle, uint16_t count, Packet **packets,
    const IsoIsoDataSegment *segments, uint8_t timestampFlag)
{
    int ret = BT_SUCCESS;
    for (uint16_t i = 0; i < count; i++) {
        const IsoIsoDataSegment *segment = &segments[i];
        uint8_t tsFlag =
            (segment->pbFlag == PB_FIRST_FRAGMENT || segment->pbFlag == PB_COMPLETE_SDU) ? timestampFlag : 0;
        ret = HCI_SendIsoData(connectionHandle, segment->pbFlag, tsFlag, packets[i]);
        PacketFree(packets[i]);
        if (ret != BT_SUCCESS) {
            for (uint16_t j = i + 1; j < count; j++) {
                PacketFree(packets[j]);
            }
            break;
        }
    }
    return ret;
}

int IsoLeSendIsoData(const IsoLeSendIsoDataParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (!IsoIsEnable()) {
        return BT_BAD_STATUS;
    }
    if (param->timestampFlag > 1 || param->length > ISO_SDU_LENGTH_MAX ||
        (param->length > 0 && param->data == NULL)) {
        return BT_BAD_PARAM;
    }
    if (!IsoIsIsoHandleActive(param->connectionHandle)) {
        return BT_BAD_PARAM;
    }

    uint16_t count = 0;
    IsoIsoDataSegment *segments = NULL;
    int ret = IsoPrepareSegments(param, &count, &segments);
    if (ret != BT_SUCCESS) {
        return ret;
    }

    // Pre-check (Vol 4, Part E, 4.1.1): reject before any fragment is queued when
    // the credit is already short, instead of knowingly emitting a truncated SDU.
    // This is NOT an atomic reservation: the credit is a shared budget drained by
    // HCI_SendIsoData per fragment, so a send below can still fail part-way (buffer
    // allocation failure), leaving earlier fragments queued as an incomplete SDU.
    // The peer's reassembler drops that SDU and the stream self-heals on the next
    // one; a non-success return below means "the SDU was not fully queued" and the
    // caller must retry the whole SDU, never resume from the middle.
    if (HciIsoGetAvailableIsoDataPackets() < count) {
        MEM_MALLOC.free(segments);
        return BT_OPERATION_FAILED;
    }

    Packet **packets = MEM_MALLOC.alloc(sizeof(Packet *) * count);
    if (packets == NULL) {
        MEM_MALLOC.free(segments);
        return BT_NO_MEMORY;
    }
    uint16_t built = 0;
    ret = IsoBuildFragmentPackets(param, count, segments, packets, &built);
    if (ret != BT_SUCCESS) {
        for (uint16_t i = 0; i < built; i++) {
            PacketFree(packets[i]);
        }
        MEM_MALLOC.free(packets);
        MEM_MALLOC.free(segments);
        return ret;
    }

    ret = IsoSendFragmentPackets(param->connectionHandle, count, packets, segments, param->timestampFlag);
    MEM_MALLOC.free(packets);
    MEM_MALLOC.free(segments);
    return ret;
}

void IsoRegisterHciDataCallbacks(void)
{
    if (g_isoDataRxList == NULL) {
        g_isoDataRxList = ListCreate(IsoFreeDataRxHandle);
    }
    HCI_RegisterIsoCallbacks(&g_hciIsoCallbacks);
}

void IsoDeregisterHciDataCallbacks(void)
{
    HCI_DeregisterIsoCallbacks(&g_hciIsoCallbacks);
    if (g_isoDataRxList != NULL) {
        ListDelete(g_isoDataRxList);
        g_isoDataRxList = NULL;
    }
}
