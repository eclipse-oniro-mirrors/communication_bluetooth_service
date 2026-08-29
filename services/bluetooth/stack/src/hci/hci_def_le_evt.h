/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#ifndef HCI_DEF_LE_EVT_H
#define HCI_DEF_LE_EVT_H

#include <stdint.h>

#include "hci_def_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.1 LE Connection Complete Event
#define HCI_LE_CONNECTION_COMPLETE_EVENT 0x01

#define HCI_PEER_ADDR_TYPE_PUBLIC 0x00
#define HCI_PEER_ADDR_TYPE_RANDOM 0x01

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint8_t role;
    uint8_t peerAddressType;
    HciBdAddr peerAddress;
    uint16_t connInterval;
    uint16_t connLatency;
    uint16_t supervisionTimeout;
    uint8_t masterClockAccuracy;
} HciLeConnectionCompleteEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.2 LE Advertising Report Event
#define HCI_LE_ADVERTISING_REPORT_EVENT 0x02

typedef struct {
    uint8_t eventType;
    uint8_t addressType;
    HciBdAddr address;
    uint8_t lengthData;
    uint8_t *data;
    int8_t rssi;
} HciLeAdvertisingReport;

typedef struct {
    uint8_t numReports;
    HciLeAdvertisingReport *reports;
} HciLeAdvertisingReportEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.3 LE Connection Update Complete Event
#define HCI_LE_CONNECTION_UPDATE_COMPLETE_EVENT 0x03

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint16_t connInterval;
    uint16_t connLatency;
    uint16_t supervisionTimeout;
} HciLeConnectionUpdateCompleteEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.4 LE Read Remote Features Complete Event
#define HCI_LE_READ_REMOTE_FEATURES_COMPLETE_EVENT 0x04

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    HciLeFeatures leFeatures;
} HciLeReadRemoteFeaturesCompleteEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.5 LE Long Term Key Request Event
#define HCI_LE_LONG_TERM_KEYREQUEST_EVENT 0x05

typedef struct {
    uint16_t connectionHandle;
    uint8_t randomNumber[8];
    uint16_t encryptedDiversifier;
} HciLeLongTermKeyRequestEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.6 LE Remote Connection Parameter Request Event
#define HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST_EVENT 0x06

typedef struct {
    uint16_t connectionHandle;
    uint16_t intervalMin;
    uint16_t intervalMax;
    uint16_t latency;
    uint16_t timeout;
} HciLeRemoteConnectionParameterRequestEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.7 LE Data Length Change Event
#define HCI_LE_DATA_LENGTH_CHANGE_EVENT 0x07

typedef struct {
    uint16_t connectionHandle;
    uint16_t maxTxOctets;
    uint16_t maxTxTime;
    uint16_t maxRxOctets;
    uint16_t maxRxTime;
} HciLeDataLengthChangeEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.8 LE Read Local P-256 Public Key Complete Event
#define HCI_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_EVENT 0x08

typedef struct {
    uint8_t status;
    uint8_t localP256PublicKey[64];
} HciLeReadLocalP256PublicKeyCompleteEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.9 LE Generate DHKey Complete Event
#define HCI_LE_GENERATE_DHKEY_COMPLETE_EVENT 0x09

typedef struct {
    uint8_t status;
    uint8_t DHKey[32];
} HciLeGenerateDHKeyCompleteEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.10 LE Enhanced Connection Complete Event
#define HCI_LE_ENHANCED_CONNECTION_COMPLETE_EVENT 0x0A

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint8_t role;
    uint8_t peerAddressType;
    HciBdAddr peerAddress;
    HciBdAddr localResolvablePrivateAddress;
    HciBdAddr peerResolvablePrivateAddress;
    uint16_t connInterval;
    uint16_t connLatency;
    uint16_t supervisionTimeout;
    uint8_t masterClockAccuracy;
} HciLeEnhancedConnectionCompleteEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.11 LE Directed Advertising Report Event
#define HCI_LE_DIRECTED_ADVERTISING_REPORT_EVENT 0x0B

typedef struct {
    uint8_t eventType;
    uint8_t addressType;
    HciBdAddr address;
    uint8_t directAddressType;
    HciBdAddr directAddress;
    int8_t rssi;
} HciLeDirectedAdvertisingReport;

typedef struct {
    uint8_t numReports;
    HciLeDirectedAdvertisingReport *reports;
} HciLeDirectedAdvertisingReportEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.12 LE PHY Update Complete Event
#define HCI_LE_PHY_UPDATE_COMPLETE_EVENT 0x0C

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint8_t txPhy;
    uint8_t rxPhy;
} HciLePhyUpdateCompleteEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.13 LE Extended Advertising Report Event
#define HCI_LE_EXTENDED_ADVERTISING_REPORT_EVENT 0x0D

#define HCI_PDU_TYPE_ADV_IND 4105
#define HCI_PDU_TYPE_ADV_DIRECT_IND 4161
#define HCI_PDU_TYPE_ADV_SCAN_IND 4104
#define HCI_PDU_TYPE_ADV_NONCONN_IND 4096
#define HCI_PDU_TYPE_SCAN_RSP_TO_AN_ADV_IND 4617
#define HCI_PDU_TYPE_SCAN_RSP_TO_AN_ADV_SCAN_IND 4616

typedef struct {
    uint16_t eventType;
    uint8_t addressType;
    HciBdAddr address;
    uint8_t primaryPHY;
    uint8_t secondaryPHY;
    uint8_t advertisingSID;
    int8_t txPower;
    int8_t rssi;
    uint16_t periodicAdvertisingInterval;
    uint8_t directAddressType;
    HciBdAddr directAddress;
    uint8_t dataLength;
    uint8_t *data;
} HciLeExtendedAdvertisingReport;

typedef struct {
    uint8_t numReports;
    HciLeExtendedAdvertisingReport *reports;
} HciLeExtendedAdvertisingReportEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.14 LE Periodic Advertising Sync Established Event
#define HCI_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED_EVENT 0x0E

typedef struct {
    uint8_t status;
    uint16_t syncHandle;
    uint8_t advertisingSid;
    uint8_t advertiserAddressType;
    HciBdAddr advertiserAddress;
    uint8_t advertiserPhy;
    uint16_t periodicAdvertisingInterval;
    uint8_t advertiserClockAccuracy;
} HciLePeriodicAdvertisingSyncEstablishedEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.15 LE Periodic Advertising Report Event
#define HCI_LE_PERIODIC_ADVERTISING_REPORT_EVENT 0x0F

typedef struct {
    uint16_t syncHandle;
    int8_t txPower;
    int8_t rssi;
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.7.65,15 LE Periodic Advertising Report Event: CTE_Type was added between
    // RSSI and Data_Status in 5.1. Controllers without the Connectionless CTE
    // Receiver feature (Bit 20) omit the field; HciEventOnLEPeriodicAdvertisingReportEvent
    // fills HCI_LE_CTE_TYPE_NONE for them.
    uint8_t cteType;
    uint8_t dataStatus;
    uint8_t dataLength;
    const uint8_t *data;
} HciLePeriodicAdvertisingReportEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.16 LE Periodic Advertising Sync Lost Event
#define HCI_LE_PERIODIC_ADVERTISING_SYNC_LOST_EVENT 0x10

typedef struct {
    uint16_t syncHandle;
} HciLePeriodicAdvertisingSyncLostEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.17 LE Scan Timeout Event
#define HCI_LE_SCAN_TIMEOUT_EVENT 0x11

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.18 LE Advertising Set Terminated Event
#define HCI_LE_ADVERTISING_SET_TERMINATED_EVENT 0x12

typedef struct {
    uint8_t status;
    uint8_t advertisingHandle;
    uint16_t connectionHandle;
    uint8_t numCompletedExtendedAdvertisingEvents;
} HciLeAdvertisingSetTerminatedEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.19 LE Scan Request Received Event
#define HCI_LE_SCAN_REQUEST_RECEIVED_EVENT 0x13

typedef struct {
    uint8_t advertisingHandle;
    uint8_t scannerAddressType;
    HciBdAddr scannerAddress;
} HciLeScanRequestReceivedEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.65.20 LE Channel Selection Algorithm Event
#define HCI_LE_CHANNEL_SELECTION_ALGORITHM_EVENT 0x14

typedef struct {
    uint16_t connectionHandle;
    uint8_t ChannelSelectionAlgorithm;
} HciLeChannelSelectionAlgorithmEventParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.7.65,21 LE Connectionless IQ Report Event
#define HCI_LE_CONNECTIONLESS_IQ_REPORT_EVENT 0x15

// Each I/Q sample is 1 I octet + 1 Q octet, so an IQ report carries
// 2 * Sample_Count octets as interleaved (I, Q) pairs (I0, Q0, I1, Q1, ...).
#define HCI_LE_IQ_SAMPLE_OCTETS 2

// Fixed wire part: Sync_Handle(2) + Channel_Index(1) + RSSI(2) + RSSI_Antenna_ID(1) +
// CTE_Type(1) + Slot_Durations(1) + Packet_Status(1) + paEventCounter(2) + Sample_Count(1) = 12 bytes.
typedef struct HciLeConnectionlessIqReportEventParamTag {
    uint16_t syncHandle;  // 0x0FFF = Receiver Test
    uint8_t channelIndex;
    int16_t rssi;  // -1270..+200, units of 0.1 dBm
    uint8_t rssiAntennaId;
    uint8_t cteType;
    uint8_t slotDurations;
    uint8_t packetStatus;  // 0x00 = CRC OK / 0x01 / 0x02 / 0xFF = insufficient resources
    uint16_t paEventCounter;
    uint8_t sampleCount;  // 0x00 (only when Packet_Status = 0xFF) or 0x09..0x52
    // 2 * sampleCount entries as interleaved (I, Q) pairs (I0, Q0, I1, Q1, ...),
    // 1 octet each per 7.7.65,21. Zero-copy into the event packet; valid only
    // during the callback.
    const int8_t *iqSamples;
} HciLeConnectionlessIqReportEventParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.7.65,22 LE Connection IQ Report Event
#define HCI_LE_CONNECTION_IQ_REPORT_EVENT 0x16

// Fixed wire part: Connection_Handle(2) + RX_PHY(1) + Data_Channel_Index(1) + RSSI(2) +
// RSSI_Antenna_ID(1) + CTE_Type(1) + Slot_Durations(1) + Packet_Status(1) +
// connEventCounter(2) + Sample_Count(1) = 13 bytes.
typedef struct HciLeConnectionIqReportEventParamTag {
    uint16_t connectionHandle;
    uint8_t rxPhy;  // 0x01 LE 1M / 0x02 LE 2M
    uint8_t dataChannelIndex;  // 0x00-0x24
    int16_t rssi;
    uint8_t rssiAntennaId;
    uint8_t cteType;
    uint8_t slotDurations;
    uint8_t packetStatus;
    uint16_t connEventCounter;
    uint8_t sampleCount;
    // 2 * sampleCount entries as interleaved (I, Q) pairs (I0, Q0, I1, Q1, ...),
    // 1 octet each per 7.7.65,22. Zero-copy into the event packet; valid only
    // during the callback.
    const int8_t *iqSamples;
} HciLeConnectionIqReportEventParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.7.65,23 LE CTE Request Failed Event
#define HCI_LE_CTE_REQUEST_FAILED_EVENT 0x17

typedef struct {
    uint8_t status;  // 0x00 = LL_CTE_RSP received without CTE; otherwise peer rejected
    uint16_t connectionHandle;
} HciLeCteRequestFailedEventParam;

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.7.65,24 LE Periodic Advertising Sync Transfer Received Event
#define HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECEIVED_EVENT 0x18

// Fixed wire part = 19 bytes: Status(1) + Connection_Handle(2) + Service_Data(2) +
// Sync_Handle(2) + Advertising_SID(1) + Advertiser_Address_Type(1) +
// Advertiser_Address(6) + Advertiser_PHY(1) + Periodic_Advertising_Interval(2) +
// Advertiser_Clock_Accuracy(1).
typedef struct HciLePeriodicAdvertisingSyncTransferReceivedEventParamTag {
    uint8_t status;
    uint16_t connectionHandle;
    uint16_t serviceData;
    uint16_t syncHandle;  // ignored when Status != 0
    uint8_t advertisingSid;
    uint8_t advertiserAddressType;
    HciBdAddr advertiserAddress;
    uint8_t advertiserPhy;  // 0x01 LE 1M / 0x02 LE 2M / 0x03 LE Coded
    uint16_t periodicAdvertisingInterval;
    uint8_t advertiserClockAccuracy;
} HciLePeriodicAdvertisingSyncTransferReceivedEventParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.7.65,25 LE CIS Established Event
#define HCI_LE_CIS_ESTABLISHED_EVENT 0x19

typedef struct HciLeCisEstablishedEventParamTag {
    uint8_t status;
    uint16_t connectionHandle;
    uint8_t cigSyncDelay[3];
    uint8_t cisSyncDelay[3];
    uint8_t transportLatencyMToS[3];
    uint8_t transportLatencySToM[3];
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
} HciLeCisEstablishedEventParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.7.65,26 LE CIS Request Event
#define HCI_LE_CIS_REQUEST_EVENT 0x1A

typedef struct {
    uint16_t aclHandle;
    uint16_t cisHandle;
    uint8_t cigId;
    uint8_t cisId;
} HciLeCisRequestEventParam;

#ifndef HCI_LE_BIS_COUNT_MAX
#define HCI_LE_BIS_COUNT_MAX 31
#endif

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.7.65,27 LE Create BIG Complete Event
#define HCI_LE_CREATE_BIG_COMPLETE_EVENT 0x1B

typedef struct {
    uint8_t status;
    uint8_t bigHandle;
    uint8_t bigSyncDelay[3];
    uint8_t transportLatencyBig[3];
    uint8_t phy;
    uint8_t nse;
    uint8_t bn;
    uint8_t pto;
    uint8_t irc;
    uint16_t maxPdu;
    uint16_t isoInterval;
    uint8_t numBis;
    uint16_t bisHandles[HCI_LE_BIS_COUNT_MAX];
} HciLeCreateBigCompleteEventParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.7.65,28 LE Terminate BIG Complete Event
#define HCI_LE_TERMINATE_BIG_COMPLETE_EVENT 0x1C

typedef struct {
    uint8_t bigHandle;
    uint8_t status;
} HciLeTerminateBigCompleteEventParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.7.65,29 LE BIG Sync Established Event
#define HCI_LE_BIG_SYNC_ESTABLISHED_EVENT 0x1D

typedef struct {
    uint8_t status;
    uint8_t bigHandle;
    uint8_t transportLatencyBig[3];
    uint8_t nse;
    uint8_t bn;
    uint8_t pto;
    uint8_t irc;
    uint16_t maxPdu;
    uint16_t isoInterval;
    uint8_t numBis;
    uint16_t bisHandles[HCI_LE_BIS_COUNT_MAX];
} HciLeBigSyncEstablishedEventParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.7.65,30 LE BIG Sync Lost Event
#define HCI_LE_BIG_SYNC_LOST_EVENT 0x1E

typedef struct {
    uint8_t bigHandle;
    uint8_t reason;
} HciLeBigSyncLostEventParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.7.65,31 LE Request Peer SCA Complete Event
#define HCI_LE_REQUEST_PEER_SCA_COMPLETE_EVENT 0x1F

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint8_t peerClockAccuracy;
} HciLeRequestPeerScaCompleteEventParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.7.65,32 LE Path Loss Threshold Event
#define HCI_LE_PATH_LOSS_THRESHOLD_EVENT 0x20

typedef struct {
    uint16_t connectionHandle;
    uint8_t currentPathLoss;
    uint8_t zoneEntered;
} HciLePathLossThresholdEventParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.7.65,33 LE Transmit Power Reporting Event
#define HCI_LE_TRANSMIT_POWER_REPORTING_EVENT 0x21

// The event payload is 7 bytes: Handle(2) + Reason(1) + Phy(1) + Power(1) +
// Flag(1) + Delta(1) (Vol 4 Part E 7.7.65,33). The leading status field is NOT
// on the wire: it is filled only by synthetic failure paths
// (HciCmdOnLeReadRemoteTransmitPowerLevelFailed and the 0x0077 Command_Complete
// error reply) so callers can be woken with an error. Real events leave it 0.
typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint8_t reason;
    uint8_t phy;
    int8_t transmitPowerLevel;
    uint8_t transmitPowerLevelFlag;
    int8_t delta;
} HciLeTransmitPowerReportingEventParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.7.65,34 LE BIGInfo Advertising Report Event
#define HCI_LE_BIGINFO_ADVERTISING_REPORT_EVENT 0x22

typedef struct {
    uint16_t syncHandle;
    uint8_t numBis;
    uint8_t nse;
    uint16_t isoInterval;
    uint8_t bn;
    uint8_t pto;
    uint8_t irc;
    uint16_t maxPdu;
    uint8_t sduInterval[3];
    uint16_t maxSdu;
    uint8_t phy;
    uint8_t framing;
    uint8_t encryption;
} HciLeBigInfoAdvertisingReportEventParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.7.75 Authenticated Payload Timeout Expired Event
#define HCI_AUTHENTICATED_PAYLOAD_TIMEOUT_EXPIRED_EVENT 0x57

typedef struct {
    uint16_t connectionHandle;
} HciAuthenticatedPayloadTimeoutExpiredEventParam;

#pragma pack(0)

#ifdef __cplusplus
}
#endif

#endif