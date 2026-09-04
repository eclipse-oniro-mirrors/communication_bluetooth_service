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
 * @addtogroup Bluetooth
 * @{
 *
 * @brief isochronous channel (ISO) management interface
 *
 */

/**
 * @file iso_le_if.h
 *
 * @brief bluetooth iso interface
 *
 */

#ifndef ISO_LE_IF_H
#define ISO_LE_IF_H

#include "btstack.h"

#ifdef __cplusplus
extern "C" {
#endif

/// BT 5.2 isochronous channel management.
///
/// Layer: public API (ISOIF_) -> internal ISO module (Iso*) -> HCI command (HCI_Le*).
/// CIG (Connected Isochronous Group) and CIS (Connected Isochronous Stream) are managed
/// in the ISO module. Broadcast isochronous (BIG/BIS) and ISO data transmission will be
/// added under the same module.
///
/// Threading: every ISOIF_* entry point runs on the caller's thread (the internal Iso*
/// work is posted to the ISO processing queue, drained by the single Stack thread). All
/// result callbacks registered via the ISOIF_LeRegister*Callback functions run on the
/// Stack thread, exactly like the HCI events that produce them; callers must not block
/// inside a callback on a resource owned by the Stack thread.

#define ISO_LE_CIS_COUNT_MAX 16
#define ISO_LE_BIS_COUNT_MAX 31

/**
 * @brief       CIG-level parameters, mirrors HCI_LeSetCigParameters fields (except CIG_ID / CIS_Count)
 */
typedef struct {
    uint32_t sduIntervalMToS;         ///< SDU_Interval_M_To_S (24-bit, unit: us, max 0xFFFFFF)
    uint32_t sduIntervalSToM;         ///< SDU_Interval_S_To_M (24-bit, unit: us, max 0xFFFFFF)
    uint8_t slaveClockAccuracy;       ///< Slaves_Clock_Accuracy
    uint8_t packing;                  ///< Packing: 0x00 sequential, 0x01 interleaved
    uint8_t framing;                  ///< Framing: 0x00 unframed, 0x01 framed
    uint16_t maxTransportLatencyMToS; ///< Max_Transport_Latency_M_To_S (0xFFFF: no constraint)
    uint16_t maxTransportLatencySToM; ///< Max_Transport_Latency_S_To_M (0xFFFF: no constraint)
} IsoLeCigParam;

/**
 * @brief       Per-CIS configuration, mirrors HciLeCisConfigParam
 */
typedef struct {
    uint8_t cisId;       ///< CIS Identifier, unique within the CIG (0x00-0x0F)
    uint16_t maxSduMToS; ///< Maximum size of SDU from Master to Slave
    uint16_t maxSduSToM; ///< Maximum size of SDU from Slave to Master
    uint8_t phyMToS;     ///< PHY used for transmission from Master to Slave
    uint8_t phySToM;     ///< PHY used for transmission from Slave to Master
    uint8_t rtnMToS;     ///< Number of retransmission attempts from Master to Slave
    uint8_t rtnSToM;     ///< Number of retransmission attempts from Slave to Master
} IsoLeCisParam;

/**
 * @brief       Per-CIS entry for LE Create CIS (0x0064), mirrors HciLeCreateCisConfigParam
 */
typedef struct {
    uint16_t cisHandle; ///< CIS Connection Handle, returned by LE Set CIG Parameters Complete
    uint16_t aclHandle; ///< ACL Connection Handle of the peer device
} IsoLeCreateCisParam;

/**
 * @brief       Negotiated CIS parameters, delivered by the LE CIS Established event (subevent 0x19)
 *
 * cisHandle is the CIS Connection Handle echoed from the Accept/Reject response.
 * cigSyncDelay / cisSyncDelay / transportLatency* are 24-bit on-the-air values,
 * stored as uint32_t following the IsoLeCigParam::sduInterval* convention.
 */
typedef struct {
    uint16_t cisHandle;
    uint32_t cigSyncDelay;
    uint32_t cisSyncDelay;
    uint32_t transportLatencyMToS;
    uint32_t transportLatencySToM;
    uint8_t phyMToS;
    uint8_t phySToM;
    uint8_t nse;
    uint8_t bnMToS;
    uint8_t bnSToM;
    uint8_t ftMToS;
    uint8_t ftSToM;
    uint16_t maxPduMToS;
    uint16_t maxPduSToM;
    uint16_t isoInterval;
} IsoLeCisEstablishedInfo;

/**
 * @brief       CIG/CIS management result callback structure
 *
 * status is the HCI command completion status (HCI_SUCCESS = 0x00).
 * Commands whose return is status-only (Create CIS / Remove CIG / Reject CIS Request)
 * do not echo the cisHandle / cigId back to the caller, following the GAP status-only convention.
 * cisRequestInd notifies the app of an incoming LE CIS Request event (subevent 0x1A),
 * so it can answer with Accept/Reject CIS Request.
 *
 * All callbacks run on the ISO processing thread. ISOIF_* APIs are safe to call from
 * inside a callback: the request is then executed inline on the same thread instead
 * of blocking on the ISO queue (calls from any other thread block until the ISO
 * thread completes the request).
 */
typedef struct {
    void (*createCigResult)(uint8_t status, uint8_t cigId, uint8_t cisCount, const uint16_t *cisHandles, void *context);
    void (*createCisResult)(uint8_t status, void *context);
    void (*removeCigResult)(uint8_t status, void *context);
    void (*cisRequestInd)(uint16_t cisHandle, uint16_t aclHandle, uint8_t cigId, uint8_t cisId, void *context);
    void (*cisEstablished)(uint8_t status, const IsoLeCisEstablishedInfo *info, void *context);
    void (*rejectCisResult)(uint8_t status, void *context);
    ///< CIS termination complete (status == HCI_SUCCESS). Reported on both the master and
    ///< slave side regardless of which side initiated (Vol 6, Part B, 4.5.12). On the local
    ///< host reason is Connection Terminated by Local Host (0x16), on the peer it is the
    ///< Reason command parameter of HCI_Disconnect (Vol 2, Part E, 7.1.6). When status is
    ///< not HCI_SUCCESS the disconnection failed and reason shall be ignored (Vol 4, Part E,
    ///< 7.7.5).
    void (*cisDisconnected)(uint8_t status, uint16_t cisHandle, uint8_t reason, void *context);
} IsoLeCigCallback;

/**
 * @brief       BIG-level parameters, mirrors HciLeCreateBigParam fields
 *              (except BIG_Handle / Advertising_Handle / Num_BIS / Broadcast_Code)
 */
typedef struct {
    uint32_t sduInterval;         ///< SDU_Interval (24-bit, unit: us, max 0xFFFFFF)
    uint16_t maxSdu;              ///< Max_SDU
    uint16_t maxTransportLatency; ///< Max_Transport_Latency (0xFFFF: no constraint)
    uint8_t rtn;                  ///< RTN: number of retransmission attempts
    uint8_t phy;                  ///< PHY used for transmission
    uint8_t packing;              ///< Packing: 0x00 sequential, 0x01 interleaved
    uint8_t framing;              ///< Framing: 0x00 unframed, 0x01 framed
    uint8_t encryption;           ///< Encryption: 0x00 unencrypted, 0x01 encrypted
} IsoLeBigParam;

/**
 * @brief       BIG-level parameters for LE Create BIG Test, adds ISO_Interval / NSE
 * @note        Breaking layout change (5.3 migration, HCI 7.8.104 field set):
 *              the former maxTransportLatency/rtn fields were replaced by
 *              nse/maxPdu/bn/irc/pto. No in-repo consumer existed at the
 *              change, but downstream code compiled against the old layout
 *              will silently misparse this structure - recompile every user.
 */
typedef struct {
    uint32_t sduInterval;         ///< SDU_Interval (24-bit, unit: us, max 0xFFFFFF)
    uint16_t isoInterval;         ///< ISO_Interval (unit: 1.25ms)
    uint8_t nse;                  ///< NSE: number of subevents (Vol 4 Part E 7.8.104)
    uint16_t maxSdu;              ///< Max_SDU
    uint16_t maxPdu;              ///< Max_PDU: maximum size of every BIS Data PDU
    uint8_t phy;                  ///< PHY used for transmission
    uint8_t packing;              ///< Packing: 0x00 sequential, 0x01 interleaved
    uint8_t framing;              ///< Framing: 0x00 unframed, 0x01 framed
    uint8_t bn;                   ///< BN: number of new payloads per BIS event (0x01-0x07)
    uint8_t irc;                  ///< IRC: 1 to (NSE / BN)
    uint8_t pto;                  ///< PTO: pre-transmission offset (0x00-0x0F)
    uint8_t encryption;           ///< Encryption: 0x00 unencrypted, 0x01 encrypted
} IsoLeBigTestParam;

/**
 * @brief       BIG Create Sync parameters, mirrors HciLeBigCreateSyncParam (0x006B)
 */
typedef struct {
    uint8_t bigHandle;         ///< BIG Handle
    uint16_t syncHandle;       ///< Sync Handle of the periodic advertising train
    uint8_t encryption;        ///< Encryption: 0x00 unencrypted, 0x01 encrypted
    uint8_t broadcastCode[16]; ///< Broadcast_Code, 16 bytes
    uint8_t mse;               ///< MSE: maximum number of subevents
    uint16_t bigSyncTimeout;   ///< BIG_Sync_Timeout (unit: 10ms)
    uint8_t numBis;            ///< Number of BIS to synchronize (0 = all BISes of the BIG, 1-31)
    const uint8_t *bis;        ///< Array of BIS indices, numBis entries (ignored, may be NULL, when numBis == 0).
                               ///< Only dereferenced during the ISOIF_LeBigCreateSync call (copied into the HCI
                               ///< command before it is sent), see codecConfiguration for the same lifetime rule.
} IsoLeBigCreateSyncParam;

/**
 * @brief       ISO data path setup parameters, mirrors HciLeSetupIsoDataPathParam (0x006E)
 *
 * controllerDelay is a 24-bit on-the-air value (unit: us, max 0x3D0900), stored as
 * uint32_t following the IsoLeCigParam::sduInterval* convention. codecConfiguration is
 * only dereferenced when codecConfigurationLength > 0; it stays valid for the duration
 * of the ISOIF_LeSetupIsoDataPath call (synchronously copied before the command is sent).
 */
typedef struct {
    uint16_t connectionHandle;         ///< CIS/BIS Connection Handle
    uint8_t dataPathDirection;         ///< Data_Path_Direction: 0x00 input (Host->Controller), 0x01 output
    uint8_t dataPathId;                ///< Data_Path_ID: 0x00 HCI, 0x01-0xFE vendor, 0xFF disabled
    uint8_t codecId[5];                ///< Codec_ID: Coding_Format(1)+Company_ID(2)+Vendor_Specific_Codec_ID(2)
    uint32_t controllerDelay;          ///< Controller_Delay (24-bit, unit: us, max 0x3D0900)
    uint8_t codecConfigurationLength;  ///< Length of codecConfiguration
    const uint8_t *codecConfiguration; ///< Codec configuration, codecConfigurationLength bytes
} IsoLeSetupIsoDataPathParam;

/**
 * @brief       Established BIG parameters, delivered by the LE Create BIG Complete event (subevent 0x1B)
 *
 * bigSyncDelay / transportLatencyBig are 24-bit on-the-air values, stored as uint32_t
 * following the IsoLeCigParam::sduInterval* convention.
 */
typedef struct {
    uint8_t bigHandle;
    uint32_t bigSyncDelay;
    uint32_t transportLatencyBig;
    uint8_t phy;
    uint8_t nse;
    uint8_t bn;
    uint8_t pto;
    uint8_t irc;
    uint16_t maxPdu;
    uint16_t isoInterval;
    uint8_t numBis;
    uint16_t bisHandles[ISO_LE_BIS_COUNT_MAX];
} IsoLeBigCreatedInfo;

/**
 * @brief       Established BIG sync parameters, delivered by the LE BIG Sync Established event
 *              (subevent 0x1D)
 */
typedef struct {
    uint8_t bigHandle;
    uint32_t transportLatencyBig;
    uint8_t nse;
    uint8_t bn;
    uint8_t pto;
    uint8_t irc;
    uint16_t maxPdu;
    uint16_t isoInterval;
    uint8_t numBis;
    uint16_t bisHandles[ISO_LE_BIS_COUNT_MAX];
} IsoLeBigSyncEstablishedInfo;

/**
 * @brief       BIGInfo parameters, delivered by the LE BIGInfo Advertising Report event
 *              (subevent 0x22). sduInterval is a 24-bit value stored as uint32_t.
 */
typedef struct {
    uint16_t syncHandle;
    uint8_t numBis;
    uint8_t nse;
    uint16_t isoInterval;
    uint8_t bn;
    uint8_t pto;
    uint8_t irc;
    uint16_t maxPdu;
    uint32_t sduInterval;
    uint16_t maxSdu;
    uint8_t phy;
    uint8_t framing;
    uint8_t encryption;
} IsoLeBigInfoReportInfo;

/**
 * @brief       BIG/BIS management result callback structure
 *
 * status is the HCI status (HCI_SUCCESS = 0x00). Commands whose completion is event-driven
 * (Create BIG / Create BIG Test / Terminate BIG / BIG Create Sync) report through the matching
 * event callback; BIG Terminate Sync has a normal Command_Complete.
 *
 * All callbacks run on the ISO processing thread. ISOIF_* APIs are safe to call from
 * inside a callback: the request is then executed inline on the same thread instead
 * of blocking on the ISO queue (calls from any other thread block until the ISO
 * thread completes the request).
 */
typedef struct {
    void (*createBigResult)(uint8_t status, const IsoLeBigCreatedInfo *info, void *context);
    void (*terminateBigResult)(uint8_t status, void *context);
    void (*bigSyncEstablished)(uint8_t status, const IsoLeBigSyncEstablishedInfo *info, void *context);
    void (*bigSyncLost)(uint8_t bigHandle, uint8_t reason, void *context);
    void (*bigInfoReport)(const IsoLeBigInfoReportInfo *info, void *context);
    void (*bigTerminateSyncResult)(uint8_t status, void *context);
} IsoLeBigCallback;

/**
 * @brief       ISO data path result callback structure (0x006E/0x006F)
 *
 * Data path setup applies to the shared CIS/BIS connection handle, so results are
 * reported through a dedicated callback rather than the CIG/BIG callbacks.
 */
typedef struct {
    void (*setupIsoDataPathResult)(uint8_t status, uint16_t connectionHandle, void *context);
    void (*removeIsoDataPathResult)(uint8_t status, uint16_t connectionHandle, void *context);
} IsoLeDataPathCallback;

/**
 * @brief       ISO test counters, delivered by readTestCountersResult / testEndResult
 *
 * receivedPacketCount / missedPacketCount / failedPacketCount are the 32-bit
 * Received_Packet_Count / Missed_Packet_Count / Failed_Packet_Count read from the
 * Controller (see [Vol 6] Part B, Section 7).
 */
typedef struct {
    uint16_t connectionHandle;    ///< CIS/BIS Connection Handle
    uint32_t receivedPacketCount; ///< Received_Packet_Count (32-bit)
    uint32_t missedPacketCount;   ///< Missed_Packet_Count (32-bit)
    uint32_t failedPacketCount;   ///< Failed_Packet_Count (32-bit)
} IsoLeTestCountersInfo;

/**
 * @brief       ISO test result callback structure (0x0070-0x0073)
 *
 * ISO Transmit/Receive Test and their counter reads apply to the shared CIS/BIS
 * connection handle, so results are reported through a dedicated callback rather
 * than the CIG/BIG/data-path callbacks.
 */
typedef struct {
    void (*transmitTestResult)(uint8_t status, uint16_t connectionHandle, void *context);
    void (*receiveTestResult)(uint8_t status, uint16_t connectionHandle, void *context);
    void (*readTestCountersResult)(uint8_t status, const IsoLeTestCountersInfo *info, void *context);
    void (*testEndResult)(uint8_t status, const IsoLeTestCountersInfo *info, void *context);
} IsoLeTestCallback;

/**
 * @brief       ISO link quality counters, delivered by readIsoLinkQualityResult (0x0075)
 *
 * txUnackedPackets / txFlushedPackets / txLastSubeventPackets / retransmittedPackets /
 * crcErrorPackets / rxUnreceivedPackets / duplicatePackets are the 32-bit counters read
 * from the Controller (see [Vol 4] Part E, Section 7.8.116 and [Vol 6] Part B, Table 7.3).
 * Counters not associated with the stream type are ignored.
 */
typedef struct {
    uint16_t connectionHandle;      ///< CIS/BIS Connection Handle
    uint32_t txUnackedPackets;      ///< Tx_UnACKed_Packets (CIS)
    uint32_t txFlushedPackets;      ///< Tx_Flushed_Packets (CIS)
    uint32_t txLastSubeventPackets; ///< Tx_Last_Subevent_Packets (CIS in slave role)
    uint32_t retransmittedPackets;  ///< Retransmitted_Packets (CIS)
    uint32_t crcErrorPackets;       ///< CRC_Error_Packets (CIS and BIS)
    uint32_t rxUnreceivedPackets;   ///< Rx_Unreceived_Packets (CIS and BIS)
    uint32_t duplicatePackets;      ///< Duplicate_Packets (CIS)
} IsoLeLinkQualityInfo;

/**
 * @brief       ISO TX sync timing, delivered by readIsoTxSyncResult (0x0061)
 *
 * timeStamp is a 32-bit value (unit: us). timeOffset is a 24-bit on-the-air value
 * (unit: us), stored as uint32_t following the IsoLeCigParam::sduInterval convention;
 * it is zero when the stream transmits unframed PDUs.
 */
typedef struct {
    uint16_t connectionHandle;     ///< CIS/BIS Connection Handle
    uint16_t packetSequenceNumber; ///< Packet_Sequence_Number of the transmitted SDU
    uint32_t timeStamp;            ///< Time_Stamp of the reference anchor point (unit: us)
    uint32_t timeOffset;           ///< Time_Offset of the transmitted SDU (24-bit, unit: us)
} IsoLeTxSyncInfo;

/**
 * @brief       Peer sleep clock accuracy, delivered by requestPeerScaResult (0x006D)
 *
 * connectionHandle is the ACL connection handle on which the SCA was requested.
 */
typedef struct {
    uint16_t connectionHandle; ///< ACL Connection Handle
    uint8_t peerClockAccuracy; ///< Peer_Clock_Accuracy (Sleep Clock Accuracy of the peer)
} IsoLePeerScaInfo;

/**
 * @brief       ISO status query result callback structure (0x0061/0x006D/0x0075)
 *
 * Link quality counters, TX sync timing, and peer SCA are status queries on a stream,
 * reported through a dedicated callback rather than the CIG/BIG/data-path/test callbacks.
 */
typedef struct {
    void (*readIsoLinkQualityResult)(uint8_t status, const IsoLeLinkQualityInfo *info, void *context);
    void (*readIsoTxSyncResult)(uint8_t status, const IsoLeTxSyncInfo *info, void *context);
    void (*requestPeerScaResult)(uint8_t status, const IsoLePeerScaInfo *info, void *context);
} IsoLeStatusQueryCallback;

/**
 * @brief       Description of one received ISO SDU, delivered by sduReceivedInd
 *
 * data is valid only during the callback. timestamp is the Time_Stamp (us) when
 * TS_Flag was set, 0 otherwise. packetStatus (Table 5.2): 0 valid, 1 possibly
 * invalid, 2 lost; non-zero is delivered for the upper layer to discard at its
 * discretion.
 */
typedef struct {
    uint16_t connectionHandle; ///< CIS/BIS Connection Handle
    uint16_t seqNum;           ///< Packet_Sequence_Number of the received SDU
    uint32_t timestamp;        ///< Time_Stamp (us); 0 when TS_Flag was not set
    uint8_t packetStatus;      ///< 0 valid, 1 possibly invalid, 2 lost (Table 5.2)
    const uint8_t *data;       ///< ISO_SDU data; valid only during the callback
    uint16_t length;           ///< ISO_SDU length (0 for a lost complete packet)
} IsoLeSduReceivedInfo;

/**
 * @brief       ISO SDU receive callback structure (HCI ISO Data packets, 0x14/0x15)
 *
 * Invoked once per complete reassembled ISO SDU.
 *
 * Lost-SDU delivery contract (matches IsoSduRxOnPacket; Vol 4, Part E, 5.4.5): a lost
 * SDU carried in one complete packet (PB 0b10) is reported with packetStatus 2. The
 * spec has the sender set ISO_SDU_Length to zero and send no data, so the report
 * carries data == NULL and length == 0; a non-conforming sender that marks such a
 * packet lost but still fills the payload gets it delivered as-is with packetStatus 2
 * (length > 0, data valid). A malformed lost packet (length mismatch) is rejected
 * without any report. A fragmented SDU (PB 0b00...) whose first fragment is marked
 * lost carries ISO_SDU_Length zero, which the reassembler rejects: no synthetic lost
 * report is generated for it, and the loss surfaces as a gap in the seqNum sequence.
 *
 * Runs on the stack thread. ISOIF_* entry points detect a same-thread caller and
 * execute inline instead of queueing (see IsoRunTaskBlockProcess), so calling them
 * from this callback neither deadlocks nor must be avoided: blocking ones simply run
 * synchronously. ISOIF_LeSendIsoData is the common case.
 */
typedef struct {
    void (*sduReceivedInd)(const IsoLeSduReceivedInfo *info, void *context);
} IsoLeSduCallback;

/**
 * @brief       Register CIG management result callback function
 * @note        Single-slot registration: a later register silently replaces the previous
 *              callback (no error is returned). The callback structure is NOT copied: a
 *              pointer to it is stored (see iso.h IsoLeMng::callback), so it and the
 *              context must both stay valid until deregistered. Callbacks run on the Stack
 *              thread (same as the HCI events), see module header.
 * @param[in]   callback            CIG management result callback structure
 * @param[in]   context             CIG management result callback context parameter
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int ISOIF_LeRegisterCigCallback(const IsoLeCigCallback *callback, void *context);

/**
 * @brief       Deregister CIG management result callback function
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int ISOIF_LeDeregisterCigCallback(void);

/**
 * @brief       Create a CIG, send LE Set CIG Parameters command
 * @param[in]   cigId               CIG Identifier, chosen by the host (0x00-0xEF)
 * @param[in]   cigParam            CIG-level parameters
 * @param[in]   cisCount            Number of CIS in the CIG (1-16)
 * @param[in]   cisParams           Per-CIS configuration array, cisCount entries
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int ISOIF_LeCreateCig(
    uint8_t cigId, const IsoLeCigParam *cigParam, uint8_t cisCount, const IsoLeCisParam *cisParams);

/**
 * @brief       Create one or more CIS, send LE Create CIS command
 * @param[in]   cisCount            Number of CIS to establish (1-16)
 * @param[in]   params              Per-CIS array of {CIS handle, ACL handle}, cisCount entries
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int ISOIF_LeCreateCis(uint8_t cisCount, const IsoLeCreateCisParam *params);

/**
 * @brief       Remove a CIG, send LE Remove CIG command
 * @param[in]   cigId               CIG Identifier (0x00-0xEF)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int ISOIF_LeRemoveCig(uint8_t cigId);

/**
 * @brief       Accept an incoming CIS establishment request, send LE Accept CIS Request (0x0066)
 * @param[in]   cisHandle           CIS Connection Handle from the LE CIS Request event
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 *
 * There is no Command_Complete for this command; success/failure is reported via
 * the cisEstablished callback (LE CIS Established event).
 */
BTSTACK_API int ISOIF_LeAcceptCisRequest(uint16_t cisHandle);

/**
 * @brief       Reject an incoming CIS establishment request, send LE Reject CIS Request (0x0067)
 * @param[in]   cisHandle           CIS Connection Handle from the LE CIS Request event
 * @param[in]   reason              HCI status code for the rejection reason
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int ISOIF_LeRejectCisRequest(uint16_t cisHandle, uint8_t reason);

/**
 * @brief       Disconnect a CIS, send the generic HCI Disconnect command (0x0406)
 * @param[in]   cisHandle           CIS Connection Handle
 * @param[in]   reason              HCI reason code (e.g. 0x13 Remote User Terminated)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 *
 * There is no dedicated CIS disconnect command in the spec; the master terminates a
 * CIS with the generic HCI Disconnect (Vol 6, Part D, 6.25). The Controller returns
 * Command Disallowed (0x0C) if the CIS is not established, so no handle validation
 * is done here. The termination result is reported via the cisDisconnected callback
 * (Disconnection Complete event, filtered to CIS handles).
 */
BTSTACK_API int ISOIF_LeDisconnectCis(uint16_t cisHandle, uint8_t reason);

/**
 * @brief       Register BIG/BIS management result callback function
 * @param[in]   callback            BIG/BIS management result callback structure
 * @param[in]   context             BIG/BIS management result callback context parameter
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
// Single-slot registration semantics identical to ISOIF_LeRegisterCigCallback: the callback
// structure is NOT copied (a pointer is stored), so it and the context must stay valid until
// deregistered.
BTSTACK_API int ISOIF_LeRegisterBigCallback(const IsoLeBigCallback *callback, void *context);

/**
 * @brief       Deregister BIG/BIS management result callback function
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int ISOIF_LeDeregisterBigCallback(void);

/**
 * @brief       Create a BIG (broadcast), send LE Create BIG command (0x0068)
 * @param[in]   bigHandle           BIG Handle, chosen by the host (0x00-0xEF)
 * @param[in]   advertisingHandle   Advertising Handle of the periodic advertising train
 * @param[in]   numBis              Number of BIS in the BIG (1-31)
 * @param[in]   bigParam            BIG-level parameters
 * @param[in]   broadcastCode       Broadcast_Code, 16 bytes (NULL when unencrypted)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 *
 * There is no Command_Complete; success/failure is reported via createBigResult
 * (LE Create BIG Complete event).
 */
BTSTACK_API int ISOIF_LeCreateBig(uint8_t bigHandle, uint8_t advertisingHandle, uint8_t numBis,
    const IsoLeBigParam *bigParam, const uint8_t *broadcastCode);

/**
 * @brief       Create a BIG in test mode, send LE Create BIG Test command (0x0069)
 * @param[in]   bigHandle           BIG Handle, chosen by the host (0x00-0xEF)
 * @param[in]   advertisingHandle   Advertising Handle of the periodic advertising train
 * @param[in]   numBis              Number of BIS in the BIG (1-31)
 * @param[in]   bigParam            BIG-level test parameters
 * @param[in]   broadcastCode       Broadcast_Code, 16 bytes (NULL when unencrypted)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int ISOIF_LeCreateBigTest(uint8_t bigHandle, uint8_t advertisingHandle, uint8_t numBis,
    const IsoLeBigTestParam *bigParam, const uint8_t *broadcastCode);

/**
 * @brief       Terminate a BIG, send LE Terminate BIG command (0x006A)
 * @param[in]   bigHandle           BIG Handle to terminate
 * @param[in]   reason              Reason for termination (HCI status code)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 *
 * Success/failure is reported via terminateBigResult (LE Terminate BIG Complete event).
 */
BTSTACK_API int ISOIF_LeTerminateBig(uint8_t bigHandle, uint8_t reason);

/**
 * @brief       Synchronize to a BIG, send LE BIG Create Sync command (0x006B)
 * @param[in]   param               BIG Create Sync parameters
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 *
 * Success/failure is reported via bigSyncEstablished (LE BIG Sync Established event).
 */
BTSTACK_API int ISOIF_LeBigCreateSync(const IsoLeBigCreateSyncParam *param);

/**
 * @brief       Terminate a BIG sync, send LE BIG Terminate Sync command (0x006C)
 * @param[in]   bigHandle           BIG Handle to terminate sync
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int ISOIF_LeBigTerminateSync(uint8_t bigHandle);

/**
 * @brief       Register ISO data path result callback function (0x006E/0x006F)
 * @param[in]   callback            ISO data path result callback structure
 * @param[in]   context             ISO data path result callback context parameter
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
// Single-slot registration semantics identical to ISOIF_LeRegisterCigCallback: the callback
// structure is NOT copied (a pointer is stored), so it and the context must stay valid until
// deregistered.
BTSTACK_API int ISOIF_LeRegisterDataPathCallback(const IsoLeDataPathCallback *callback, void *context);

/**
 * @brief       Deregister ISO data path result callback function
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int ISOIF_LeDeregisterDataPathCallback(void);

/**
 * @brief       Configure the ISO data path of a CIS/BIS, send LE Setup ISO Data Path (0x006E)
 * @param[in]   param               ISO data path setup parameters
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 *
 * Result is reported via setupIsoDataPathResult (Command_Complete). The Controller is the
 * authoritative source of data path state: re-setting an already-set path, or setting on a
 * handle with no active stream, returns Command Disallowed (0x0C) from the Controller.
 */
BTSTACK_API int ISOIF_LeSetupIsoDataPath(const IsoLeSetupIsoDataPathParam *param);

/**
 * @brief       Remove the ISO data path of a CIS/BIS, send LE Remove ISO Data Path (0x006F)
 * @param[in]   connectionHandle    CIS/BIS Connection Handle
 * @param[in]   dataPathDirection   Data_Path_Direction: bit0 remove input, bit1 remove output
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 *
 * Result is reported via removeIsoDataPathResult (Command_Complete). Removing a path that
 * was never set returns Command Disallowed (0x0C) from the Controller.
 */
BTSTACK_API int ISOIF_LeRemoveIsoDataPath(uint16_t connectionHandle, uint8_t dataPathDirection);

/**
 * @brief       Register ISO test result callback function (0x0070-0x0073)
 * @param[in]   callback            ISO test result callback structure
 * @param[in]   context             ISO test result callback context parameter
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
// Single-slot registration semantics identical to ISOIF_LeRegisterCigCallback: the callback
// structure is NOT copied (a pointer is stored), so it and the context must stay valid until
// deregistered.
BTSTACK_API int ISOIF_LeRegisterTestCallback(const IsoLeTestCallback *callback, void *context);

/**
 * @brief       Deregister ISO test result callback function
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int ISOIF_LeDeregisterTestCallback(void);

/**
 * @brief       Configure an established CIS/BIS to transmit test payloads, send LE ISO Transmit Test (0x0070)
 * @param[in]   connectionHandle    CIS/BIS Connection Handle
 * @param[in]   payloadType         Payload_Type: 0x00 zero length, 0x01 variable length, 0x02 maximum length
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 *
 * Result is reported via transmitTestResult (Command_Complete). The Controller rejects
 * (Command Disallowed 0x0C) if an input data path is already set for the handle.
 */
BTSTACK_API int ISOIF_LeIsoTransmitTest(uint16_t connectionHandle, uint8_t payloadType);

/**
 * @brief       Configure an established CIS/BIS to receive test payloads, send LE ISO Receive Test (0x0071)
 * @param[in]   connectionHandle    CIS/BIS Connection Handle
 * @param[in]   payloadType         Payload_Type: 0x00 zero length, 0x01 variable length, 0x02 maximum length
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 *
 * Result is reported via receiveTestResult (Command_Complete).
 */
BTSTACK_API int ISOIF_LeIsoReceiveTest(uint16_t connectionHandle, uint8_t payloadType);

/**
 * @brief       Read the ISO test counters, send LE ISO Read Test Counters (0x0072)
 * @param[in]   connectionHandle    CIS/BIS Connection Handle
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 *
 * Reading does not reset the counters. Result is reported via readTestCountersResult
 * (Command_Complete). The Controller rejects (Unsupported Feature 0x11) if the stream is
 * not in ISO Receive Test mode.
 */
BTSTACK_API int ISOIF_LeIsoReadTestCounters(uint16_t connectionHandle);

/**
 * @brief       Terminate ISO Transmit/Receive Test mode, send LE ISO Test End (0x0073)
 * @param[in]   connectionHandle    CIS/BIS Connection Handle
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 *
 * Result is reported via testEndResult (Command_Complete) with the final test counters
 * (zeroed when the stream was in Transmit Test mode only).
 */
BTSTACK_API int ISOIF_LeIsoTestEnd(uint16_t connectionHandle);

/**
 * @brief       Register ISO status query result callback function (0x0061/0x006D/0x0075)
 * @param[in]   callback            ISO status query result callback structure
 * @param[in]   context             ISO status query result callback context parameter
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
// Single-slot registration semantics identical to ISOIF_LeRegisterCigCallback: the callback
// structure is NOT copied (a pointer is stored), so it and the context must stay valid until
// deregistered.
BTSTACK_API int ISOIF_LeRegisterStatusQueryCallback(const IsoLeStatusQueryCallback *callback, void *context);

/**
 * @brief       Deregister ISO status query result callback function
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int ISOIF_LeDeregisterStatusQueryCallback(void);

/**
 * @brief       Read the link quality counters of a CIS/BIS, send LE Read ISO Link Quality (0x0075)
 * @param[in]   connectionHandle    CIS/BIS Connection Handle
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 *
 * Result is reported via readIsoLinkQualityResult (Command_Complete). The Controller returns
 * Unknown Connection Identifier (0x02) if the handle does not identify a current CIS or a
 * synchronized BIS.
 */
BTSTACK_API int ISOIF_LeReadIsoLinkQuality(uint16_t connectionHandle);

/**
 * @brief       Read the TX sync timing of a transmitted SDU, send LE Read ISO TX Sync (0x0061)
 * @param[in]   connectionHandle    CIS/BIS Connection Handle
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 *
 * Result is reported via readIsoTxSyncResult (Command_Complete). The Controller returns
 * Unknown Connection Identifier (0x02) for an invalid handle, and Command Disallowed (0x0C)
 * if no SDU has been transmitted yet.
 */
BTSTACK_API int ISOIF_LeReadIsoTxSync(uint16_t connectionHandle);

/**
 * @brief       Read the peer's Sleep Clock Accuracy, send LE Request Peer SCA (0x006D)
 * @param[in]   connectionHandle    ACL Connection Handle
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 *
 * Result is reported via requestPeerScaResult (LE Request Peer SCA Complete event); there is
 * no Command_Complete. The Controller returns Unknown Connection Identifier (0x02) for an
 * invalid handle, and Unsupported Feature (0x11 / 0x1A) if the peer does not support the
 * Sleep Clock Accuracy Update feature.
 */
BTSTACK_API int ISOIF_LeRequestPeerSca(uint16_t connectionHandle);

/**
 * @brief       Register ISO SDU receive callback function (HCI ISO Data packets, 0x14/0x15)
 * @param[in]   callback            ISO SDU receive callback structure
 * @param[in]   context             ISO SDU receive callback context parameter
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int ISOIF_LeRegisterSduCallback(const IsoLeSduCallback *callback, void *context);

/**
 * @brief       Deregister ISO SDU receive callback function
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 */
BTSTACK_API int ISOIF_LeDeregisterSduCallback(void);

/**
 * @brief       Parameters of one ISO SDU to transmit (ISOIF_LeSendIsoData)
 *
 * The SDU bytes are copied by the call and need not outlive it. timestamp is
 * carried in the ISO_Data_Load header only when timestampFlag is set.
 */
typedef struct {
    uint16_t connectionHandle; ///< CIS/BIS Connection Handle
    uint16_t seqNum;           ///< Packet_Sequence_Number of the SDU
    uint32_t timestamp;        ///< Time_Stamp of the SDU (unit: us)
    uint8_t timestampFlag;     ///< Whether to include the Time_Stamp in the ISO_Data_Load header
    const uint8_t *data;       ///< ISO_SDU data, length bytes
    uint16_t length;           ///< ISO_SDU length
} IsoLeSendIsoDataParam;

/**
 * @brief       Send an ISO SDU, fragmenting it into HCI ISO data packets (0x14/0x15)
 * @param[in]   param               ISO SDU to transmit (see IsoLeSendIsoDataParam)
 * @return      @c BT_SUCCESS      : The SDU was queued for transmission.
 *              @c otherwise        : The SDU was rejected (invalid parameter or queue failure).
 * @note        The SDU is copied and the call returns immediately; the return value is only the
 *              enqueue result, not the send result. A later send failure (no ISO data buffer, or
 *              before LE Read Buffer Size V2 completes) is dropped and logged; data stays valid.
 */
BTSTACK_API int ISOIF_LeSendIsoData(const IsoLeSendIsoDataParam *param);

#ifdef __cplusplus
}
#endif

#endif // ISO_LE_IF_H
