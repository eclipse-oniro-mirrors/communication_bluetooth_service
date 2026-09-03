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

#ifndef HCI_DEF_LE_CMD_H
#define HCI_DEF_LE_CMD_H

#include <stdint.h>

#include "hci_def_cmd_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8 LE CONTROLLER COMMANDS
#define HCI_COMMAND_OGF_LE_CONTROLLER 0x08

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.1 LE Set Event Mask Command
#define HCI_LE_SET_EVENT_MASK MAKE_OPCODE(0x0001, HCI_COMMAND_OGF_LE_CONTROLLER)

#define LE_EVENT_MASK_LE_CONNECTION_COMPLETE_EVENT 0x0000000000000001
#define LE_EVENT_MASK_LE_ADVERTISING_REPORT_EVENT 0x0000000000000002
#define LE_EVENT_MASK_LE_CONNECTION_UPDATE_COMPLETE_EVENT 0x0000000000000004
#define LE_EVENT_MASK_LE_READ_REMOTE_FEATURES_COMPLETE_EVENT 0x0000000000000008
#define LE_EVENT_MASK_LE_LONG_TERM_KEY_REQUEST_EVENT 0x0000000000000010
#define LE_EVENT_MASK_LE_REMOTE_CONNECTION_PARAMETER_REQUEST_EVENT 0x0000000000000020
#define LE_EVENT_MASK_LE_DATA_LENGTH_CHANGE_EVENT 0x0000000000000040
#define LE_EVENT_MASK_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_EVENT 0x0000000000000080
#define LE_EVENT_MASK_LE_GENERATE_DHKEY_COMPLETE_EVENT 0x0000000000000100
#define LE_EVENT_MASK_LE_ENHANCED_CONNECTION_COMPLETE_EVENT 0x0000000000000200
#define LE_EVENT_MASK_LE_DIRECTED_ADVERTISING_REPORT_EVENT 0x0000000000000400
#define LE_EVENT_MASK_LE_PHY_UPDATE_COMPLETE_EVENT 0x0000000000000800
#define LE_EVENT_MASK_LE_EXTENDED_ADVERTISING_REPORT_EVENT 0x0000000000001000
#define LE_EVENT_MASK_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED_EVENT 0x0000000000002000
#define LE_EVENT_MASK_LE_PERIODIC_ADVERTISING_REPORT_EVENT 0x0000000000004000
#define LE_EVENT_MASK_LE_PERIODIC_ADVERTISING_SYNC_LOST_EVENT 0x0000000000008000
#define LE_EVENT_MASK_LE_EXTENDED_SCAN_TIMEOUT_EVENT 0x0000000000010000
#define LE_EVENT_MASK_LE_EXTENDED_ADVERTISING_SET_TERMINATED_EVENT 0x0000000000020000
#define LE_EVENT_MASK_LE_SCAN_REQUEST_RECEIVED_EVENT 0x0000000000040000
#define LE_EVENT_MASK_LE_CHANNEL_SELECTION_ALGORITHM_EVENT 0x0000000000080000

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.1 LE Set Event Mask Command (subevent code 0x15-0x18)
#define LE_EVENT_MASK_LE_CONNECTIONLESS_IQ_REPORT_EVENT 0x0000000000100000
#define LE_EVENT_MASK_LE_CONNECTION_IQ_REPORT_EVENT 0x0000000000200000
#define LE_EVENT_MASK_LE_CTE_REQUEST_FAILED_EVENT 0x0000000000400000
#define LE_EVENT_MASK_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECEIVED_EVENT 0x0000000000800000

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.1 LE Set Event Mask Command (subevent code 0x19-0x22)
#define LE_EVENT_MASK_LE_CIS_ESTABLISHED_EVENT            0x0000000001000000
#define LE_EVENT_MASK_LE_CIS_REQUEST_EVENT                0x0000000002000000
#define LE_EVENT_MASK_LE_CREATE_BIG_COMPLETE_EVENT        0x0000000004000000
#define LE_EVENT_MASK_LE_TERMINATE_BIG_COMPLETE_EVENT     0x0000000008000000
#define LE_EVENT_MASK_LE_BIG_SYNC_ESTABLISHED_EVENT       0x0000000010000000
#define LE_EVENT_MASK_LE_BIG_SYNC_LOST_EVENT              0x0000000020000000
#define LE_EVENT_MASK_LE_REQUEST_PEER_SCA_COMPLETE_EVENT  0x0000000040000000
#define LE_EVENT_MASK_LE_PATH_LOSS_THRESHOLD_EVENT        0x0000000080000000
#define LE_EVENT_MASK_LE_TRANSMIT_POWER_REPORTING_EVENT   0x0000000100000000
#define LE_EVENT_MASK_LE_BIGINFO_ADVERTISING_REPORT_EVENT 0x0000000200000000

// BLUETOOTH SPECIFICATION Version 5.3 | Vol 4, Part E
// 7.8.1 LE Set Event Mask Command (bit 34, subevent code 0x23)
#define LE_EVENT_MASK_LE_SUBRATE_CHANGE_EVENT            0x0000000400000000

#define LE_EVENT_MASK_DEFAULT 0x000000000000001F

#define LE_EVENT_MASK_CORE_4_0 LE_EVENT_MASK_DEFAULT

#define LE_EVENT_MASK_CORE_4_1 (LE_EVENT_MASK_CORE_4_0 | LE_EVENT_MASK_LE_REMOTE_CONNECTION_PARAMETER_REQUEST_EVENT)

#define LE_EVENT_MASK_CORE_4_2                                                                                        \
    (LE_EVENT_MASK_CORE_4_1 | LE_EVENT_MASK_LE_DATA_LENGTH_CHANGE_EVENT |                                             \
        LE_EVENT_MASK_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_EVENT | LE_EVENT_MASK_LE_GENERATE_DHKEY_COMPLETE_EVENT | \
        LE_EVENT_MASK_LE_ENHANCED_CONNECTION_COMPLETE_EVENT | LE_EVENT_MASK_LE_DIRECTED_ADVERTISING_REPORT_EVENT)

#define LE_EVENT_MASK_CORE_5_0                                                                                       \
    (LE_EVENT_MASK_CORE_4_2 | LE_EVENT_MASK_LE_PHY_UPDATE_COMPLETE_EVENT |                                           \
        LE_EVENT_MASK_LE_EXTENDED_ADVERTISING_REPORT_EVENT |                                                         \
        LE_EVENT_MASK_LE_PERIODIC_ADVERTISING_SYNC_ESTABLISHED_EVENT |                                               \
        LE_EVENT_MASK_LE_PERIODIC_ADVERTISING_REPORT_EVENT | LE_EVENT_MASK_LE_PERIODIC_ADVERTISING_SYNC_LOST_EVENT | \
        LE_EVENT_MASK_LE_EXTENDED_SCAN_TIMEOUT_EVENT | LE_EVENT_MASK_LE_EXTENDED_ADVERTISING_SET_TERMINATED_EVENT |  \
        LE_EVENT_MASK_LE_SCAN_REQUEST_RECEIVED_EVENT | LE_EVENT_MASK_LE_CHANNEL_SELECTION_ALGORITHM_EVENT)

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.1 LE Set Event Mask Command
#define LE_EVENT_MASK_CORE_5_1                                                                                       \
    (LE_EVENT_MASK_CORE_5_0 | LE_EVENT_MASK_LE_CONNECTIONLESS_IQ_REPORT_EVENT |                                      \
        LE_EVENT_MASK_LE_CONNECTION_IQ_REPORT_EVENT | LE_EVENT_MASK_LE_CTE_REQUEST_FAILED_EVENT |                    \
        LE_EVENT_MASK_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECEIVED_EVENT)

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.33 LE Set Data Length Command / 7.8.35 LE Write Suggested Default Data Length Command
#define LE_DATA_LENGTH_MIN_OCTETS 0x001B
#define LE_DATA_LENGTH_MAX_OCTETS 0x00FB
#define LE_DATA_LENGTH_MIN_TIME 0x0148
#define LE_DATA_LENGTH_MAX_TIME 0x4290

typedef struct {
    uint64_t leEventMask;
} HciLeSetEventMaskParam;

typedef HciStatusParam HciLeSetEventMaskReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.2 LE Read Buffer Size Command
#define HCI_LE_READ_BUFFER_SIZE MAKE_OPCODE(0x0002, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint16_t hcLeAclDataPacketLength;
    uint8_t hcTotalNumLeDataPackets;
} HciLeReadBufferSizeReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.72 LE Read Buffer Size V2 Command
#define HCI_LE_READ_BUFFER_SIZE_V2 MAKE_OPCODE(0x0060, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint16_t hcLeAclDataPacketLength;
    uint8_t hcTotalNumLeAclDataPackets;
    uint16_t hcLeIsoDataPacketLength;
    uint8_t hcTotalNumLeIsoDataPackets;
} HciLeReadBufferSizeV2ReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.3 LE Read Local Supported Features Command
#define HCI_LE_READ_LOCAL_SUPPORTED_FEATURES MAKE_OPCODE(0x0003, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    HciLeFeatures leFeatures;
} HciLeReadLocalSupportedFeaturesReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.4 LE Set Random Address Command
#define HCI_LE_SET_RANDOM_ADDRESS MAKE_OPCODE(0x0005, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t randomAddress[6];
} HciLeSetRandomAddressParam;

typedef HciStatusParam HciLeSetRandomAddressReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.5 LE Set Advertising Parameters Command
#define HCI_LE_SET_ADVERTISING_PARAMETERS MAKE_OPCODE(0x0006, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t advertisingIntervalMin;
    uint16_t advertisingIntervalMax;
    uint8_t advertisingType;
    uint8_t ownAddressType;
    uint8_t peerAddressType;
    HciBdAddr peerAddress;
    uint8_t advertisingChannelMap;
    uint8_t advertisingFilterPolicy;
} HciLeSetAdvertisingParametersParam;

typedef HciStatusParam HciLeSetAdvertisingParametersReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.6 LE Read Advertising Channel Tx Power Command
#define HCI_LE_READ_ADVERTISING_CHANNEL_TX_POWER MAKE_OPCODE(0x0007, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint8_t transmitPowerLevel;
} HciLeReadAdvertisingChannelTxPowerReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.7 LE Set Advertising Data Command
#define HCI_LE_SET_ADVERTISING_DATA MAKE_OPCODE(0x0008, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t advertisingDataLen;
    uint8_t advertisingData[31];
} HciLeSetAdvertisingDataParam;

typedef HciStatusParam HciLeSetAdvertisingDataReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.8 LE Set Scan Response Data Command
#define HCI_LE_SET_SCAN_RESPONSE_DATA MAKE_OPCODE(0x0009, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t scanResponseDataLength;
    uint8_t scanResponseData[31];
} HciLeSetScanResponseDataParam;

typedef HciStatusParam HciLeSetScanResponseDataReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.9 LE Set Advertising Enable Command
#define HCI_LE_SET_ADVERTISING_ENABLE MAKE_OPCODE(0x000A, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t advertisingEnable;
} HciLeSetAdvertisingEnableParam;

typedef HciStatusParam HciLeSetAdvertisingEnableReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.10 LE Set Scan Parameters Command
#define HCI_LE_SET_SCAN_PARAMETERS MAKE_OPCODE(0x000B, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t leScanType;
    uint16_t leScanInterval;
    uint16_t leScanWindow;
    uint8_t ownAddressType;
    uint8_t scanningFilterPolicy;
} HciLeSetScanParametersParam;

typedef HciStatusParam HciLeSetScanParametersReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.11 LE Set Scan Enable Command
#define HCI_LE_SET_SCAN_ENABLE MAKE_OPCODE(0x000C, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t leScanEnable;
    uint8_t filterDuplicates;
} HciLeSetScanEnableParam;

typedef HciStatusParam HciLeSetScanEnableReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.12 LE Create Connection Command
#define HCI_LE_CREATE_CONNECTION MAKE_OPCODE(0x000D, HCI_COMMAND_OGF_LE_CONTROLLER)

#define INITIATOR_FILTER_NO_WHITE_LIST 0x00
#define INITIATOR_FILTER_USE_WHITE_LIST 0x01

#define PEER_ADDR_TYPE_PUBLIC 0x00
#define PEER_ADDR_TYPE_RANDOM 0x01
#define PEER_ADDR_TYPE_PUBLIC_IDENTITY 0x02
#define PEER_ADDR_TYPE_RANDOM_IDENTITY 0x03

#define LOCAL_ADDR_TYPE_PUBLIC 0x00
#define LOCAL_ADDR_TYPE_RANDOM 0x01
#define LOCAL_ADDR_TYPE_RPA_OR_PUBLIC 0x02
#define LOCAL_ADDR_TYPE_RPA_OR_RANDOM 0x03

typedef struct {
    uint16_t leScanInterval;
    uint16_t leScanWindow;
    uint8_t initiatorFilterPolicy;
    uint8_t peerAddressType;
    HciBdAddr peerAddress;
    uint8_t ownAddressType;
    uint16_t connIntervalMin;
    uint16_t connIntervalMax;
    uint16_t connLatency;
    uint16_t supervisionTimeout;
    uint16_t minimumCELength;
    uint16_t maximumCELength;
} HciLeCreateConnectionParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.13 LE Create Connection Cancel Command
#define HCI_LE_CREATE_CONNECTION_CANCEL MAKE_OPCODE(0x000E, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef HciStatusParam HciLeCreateConnectionCancelReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.14 LE Read WL Size Command
#define HCI_LE_READ_WHITE_LIST_SIZE MAKE_OPCODE(0x000F, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint8_t whiteListSize;
} HciLeReadWhiteListSizeReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.15 LE Clear WL Command
#define HCI_LE_CLEAR_WHITE_LIST MAKE_OPCODE(0x0010, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef HciStatusParam HciLeClearWhiteListReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.16 LE Add Device To WL Command
#define HCI_LE_ADD_DEVICE_TO_WHITE_LIST MAKE_OPCODE(0x0011, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t addressType;
    HciBdAddr address;
} HciLeAddDeviceToWhiteListParam;

typedef HciStatusParam HciLeAddDeviceToWhiteListReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.17 LE Remove Device From WL Command
#define HCI_LE_REMOVE_DEVICE_FROM_WHITE_LIST MAKE_OPCODE(0x0012, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t addressType;
    HciBdAddr address;
} HciLeRemoveDeviceFromWhiteListParam;

typedef HciStatusParam HciLeRemoveDeviceFromWhiteListReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.18 LE Connection Update Command
#define HCI_LE_CONNECTION_UPDATE MAKE_OPCODE(0x0013, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint16_t connIntervalMin;
    uint16_t connIntervalMax;
    uint16_t connLatency;
    uint16_t supervisionTimeout;
    uint16_t minimumCELength;
    uint16_t maximumCELength;
} HciLeConnectionUpdateParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.19 LE Set Host Channel Classification Command
#define HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION MAKE_OPCODE(0x0014, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t channelMap[5];
} HciLeSetHostChannelClassificationParam;

typedef HciStatusParam HciLeSetHostChannelClassificationReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.20 LE Read Channel Map Command
#define HCI_LE_READ_CHANNEL_MAP MAKE_OPCODE(0x0015, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
} HciLeReadChannelMapParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint8_t channelMap[5];
} HciLeReadChannelMapReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.21 LE Read Remote Features Command
#define HCI_LE_READ_REMOTE_FEATURES MAKE_OPCODE(0x0016, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
} HciLeReadRemoteFeaturesParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.22 LE Encrypt Command
#define HCI_LE_ENCRYPT MAKE_OPCODE(0x0017, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t key[16];
    uint8_t plaintextData[16];
} HciLeEncryptParam;

typedef struct {
    uint8_t status;
    uint8_t encryptedData[16];
} HciLeEncryptReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.23 LE Rand Command
#define HCI_LE_RAND MAKE_OPCODE(0x0018, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint8_t randomNumber[8];
} HciLeRandReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.24 LE Start Encryption Command
#define HCI_LE_START_ENCRYPTION MAKE_OPCODE(0x0019, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint8_t randomNumber[8];
    uint16_t encryptDiversifier;
    uint8_t longTermKey[16];
} HciLeStartEncryptionParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.25 LE Long Term Key Request Reply Command
#define HCI_LE_LONG_TERM_KEY_REQUEST_REPLY MAKE_OPCODE(0x001A, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint8_t longTermKey[16];
} HciLeLongTermKeyRequestReplyParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeLongTermKeyRequestReplyReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.26 LE Long Term Key Request Negative Reply Command
#define HCI_LE_LONG_TERM_KEY_REQUEST_NEGATIVE_REPLY MAKE_OPCODE(0x001B, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
} HciLeLongTermKeyRequestNegativeReplyParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeLongTermKeyRequestNegativeReplyReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.27 LE Read Supported States Command
#define HCI_LE_READ_SUPPORTED_STATES MAKE_OPCODE(0x001C, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint64_t leStates;
} HciLeReadSupportedStatesReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.28 LE Receiver Test Command
#define HCI_LE_RECEIVER_TEST MAKE_OPCODE(0x001D, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t rxChannel;
} HciLeReceiverTestParam;

typedef HciStatusParam HciLeReceiverTestReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.29 LE Transmitter Test Command
#define HCI_LE_TRANSMITTER_TEST MAKE_OPCODE(0x001E, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t txChannel;
    uint8_t lengthOfTestData;
    uint8_t packetPayload;
} HciLeTransmitterTestParam;

typedef HciStatusParam HciLeTransmitterTestReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.30 LE Test End Command
#define HCI_LE_TEST_END MAKE_OPCODE(0x001F, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint16_t numberOfPackets;
} HciLeTestEndReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.31 LE Remote Connection Parameter Request Reply Command
#define HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST_REPLY MAKE_OPCODE(0x0020, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint16_t intervalMin;
    uint16_t intervalMax;
    uint16_t latency;
    uint16_t timeout;
    uint16_t minimumCELength;
    uint16_t maximumCELength;
} HciLeRemoteConnectionParameterRequestReplyParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeRemoteConnectionParameterRequestReplyReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.32 LE Remote Connection Parameter Request Negative Reply Command
#define HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST_NEGATIVE_REPLY MAKE_OPCODE(0x0021, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint8_t reason;
} HciLeRemoteConnectionParameterRequestNegativeReplyParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeRemoteConnectionParameterRequestNegativeReplyReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.33 LE Set Data Length Command
#define HCI_LE_SET_DATA_LENGTH MAKE_OPCODE(0x0022, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint16_t txOctets;
    uint16_t txTime;
} HciLeSetDataLengthParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeSetDataLengthReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.34 LE Read Suggested Default Data Length Command
#define HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LENGTH MAKE_OPCODE(0x0023, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint16_t suggestedMaxTxOctets;
    uint16_t suggestedMaxTxTime;
} HciLeReadSuggestedDefaultDataLengthReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.35 LE Write Suggested Default Data Length Command
#define HCI_LE_WRITE_SUGGESTED_DEFAULT_DATA_LENGTH MAKE_OPCODE(0x0024, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t suggestedMaxTxOctets;
    uint16_t suggestedMaxTxTime;
} HciLeWriteSuggestedDefaultDataLengthParam;

typedef HciStatusParam HciLeWriteSuggestedDefaultDataLengthReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.36 LE Read Local P-256 Public Key Command
#define HCI_LE_READ_LOCAL_P256_PUBLIC_KEY MAKE_OPCODE(0x0025, HCI_COMMAND_OGF_LE_CONTROLLER)

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.37 LE Generate DHKey Command
#define HCI_LE_GENERATE_DHKEY MAKE_OPCODE(0x0026, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t remoteP256PublicKey[64];
} HciLeGenerateDHKeyParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.38 LE Add Device To Resolving List Command
#define HCI_LE_ADD_DEVICE_TO_RESOLVING_LIST MAKE_OPCODE(0x0027, HCI_COMMAND_OGF_LE_CONTROLLER)

#define HCI_PEER_IDENTITY_ADDRESS_TYPE_PUBLIC 0x00
#define HCI_PEER_IDENTITY_ADDRESS_TYPE_RANDOM 0x01

typedef struct {
    uint8_t peerIdentityAddrType;
    HciBdAddr peerIdentityAddress;
    uint8_t peerIrk[16];
    uint8_t localIrk[16];
} HciLeAddDeviceToResolvingListParam;

typedef HciStatusParam HciLeAddDeviceToResolvingListReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.39 LE Remove Device From Resolving List Command
#define HCI_LE_REMOVE_DEVICE_FROM_RESOLVING_LIST MAKE_OPCODE(0x0028, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t peerIdentityAddrType;
    HciBdAddr peerIdentityAddress;
} HciLeRemoveDeviceFromResolvingListParam;

typedef HciStatusParam HciLeRemoveDeviceFromResolvingListReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.40 LE Clear Resolving List Command
#define HCI_LE_CLEAR_RESOLVING_LIST MAKE_OPCODE(0x0029, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef HciStatusParam HciLeClearResolvingListReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.41 LE Read Resolving List Size Command
#define HCI_LE_READ_RESOLVING_LIST_SIZE MAKE_OPCODE(0x002A, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint8_t resolvingListSize;
} HciLeReadResolvingListSizeReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.42 LE Read Peer Resolvable Address Command
#define HCI_LE_READ_PEER_RESOLVABLE_ADDRESS MAKE_OPCODE(0x002B, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t peerIdentityAddressType;
    HciBdAddr peerIdentityAddress;
} HciLeReadPeerResolvableAddressParam;

typedef struct {
    uint8_t status;
    HciBdAddr peerResolvableAddress;
} HciLeReadPeerResolvableAddressReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.43 LE Read Local Resolvable Address Command
#define HCI_LE_READ_LOCAL_RESOLVABLE_ADDRESS MAKE_OPCODE(0x002C, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t peerIdentityAddressType;
    HciBdAddr peerIdentityAddress;
} HciLeReadLocalResolvableAddressParam;

typedef struct {
    uint8_t status;
    HciBdAddr localResolvableAddress;
} HciLeReadLocalResolvableAddressReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.44 LE Set Address Resolution Enable Command
#define HCI_LE_SET_ADDRESS_RESOLUTION_ENABLE MAKE_OPCODE(0x002D, HCI_COMMAND_OGF_LE_CONTROLLER)

#define ADDRESS_RESOLUTION_IN_CONTROLLER_DISABLED 0x00
#define ADDRESS_RESOLUTION_IN_CONTROLLER_ENABLEED 0x01

typedef struct {
    uint8_t addressResolutionEnable;
} HciLeSetAddressResolutionEnableParam;

typedef HciStatusParam HciLeSetAddressResolutionEnableReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.45 LE Set Resolvable Private Address Timeout Command
#define HCI_LE_SET_RESOLVABLE_PRIVATE_ADDRESS_TIMEOUT MAKE_OPCODE(0x002E, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t rpaTimeout;
} HciLeSetResolvablePrivateAddressTimeoutParam;

typedef HciStatusParam HciLeSetResolvablePrivateAddressTimeoutReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.46 LE Read Maximum Data Length Command
#define HCI_LE_READ_MAXIMUM_DATA_LENGTH MAKE_OPCODE(0x002F, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint16_t supportedMaxTxOctets;
    uint16_t supportedMaxTxTime;
    uint16_t supportedMaxRxOctets;
    uint16_t supportedMaxRxTime;
} HciLeReadMaximumDataLengthReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.47 LE Read PHY Command
#define HCI_LE_READ_PHY MAKE_OPCODE(0x0030, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
} HciLeReadPhyParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint8_t txPhy;
    uint8_t rxPhy;
} HciLeReadPhyReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.48 LE Set Default PHY Command
#define HCI_LE_SET_DEFAULT_PHY MAKE_OPCODE(0x0031, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t allPhys;
    uint8_t txPhys;
    uint8_t rxPhys;
} HciLeSetDefaultPhyParam;

typedef HciStatusParam HciLeSetDefaultPhyReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.49 LE Set PHY Command
#define HCI_LE_SET_PHY MAKE_OPCODE(0x0032, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint8_t allPhys;
    uint8_t txPhys;
    uint8_t rxPhys;
    uint16_t phyOptions;
} HciLeSetPhyParam;

typedef HciStatusParam HciLeSetPhyReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.50 LE Enhanced Receiver Test Command
#define HCI_LE_ENHANCED_RECEIVER_TEST MAKE_OPCODE(0x0033, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t rxChannel;
    uint8_t phy;
    uint8_t modulationIndex;
} HciLeEnhancedReceiverTestParam;

typedef HciStatusParam HciLeEnhancedReceiverTestReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.51 LE Enhanced Transmitter Test Command
#define HCI_LE_ENHANCED_TRANSMITTER_TEST MAKE_OPCODE(0x0034, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t txChannel;
    uint8_t lengthOfTestData;
    uint8_t packetPayload;
    uint8_t phy;
} HciLeEnhancedTransmitterTestParam;

typedef HciStatusParam HciLeEnhancedTransmitterTestReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.52 LE Set Advertising Set Random Address Command
#define HCI_LE_SET_ADVERTISING_SET_RANDOM_ADDRESS MAKE_OPCODE(0x0035, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t advertisingHandle;
    uint8_t randomAddress[6];
} HciLeSetAdvertisingSetRandomAddressParam;

typedef HciStatusParam HciLeSetAdvertisingSetRandomAddressReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.96 LE Read ISO TX Sync Command
#define HCI_LE_READ_ISO_TX_SYNC MAKE_OPCODE(0x0061, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
} HciLeReadIsoTxSyncParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint16_t packetSequenceNumber;
    // TX_Time_Stamp is a 24-bit value on the wire (Vol 4 Part E 7.8.96): keep the
    // struct byte-exact (11 octets) and let the consumer widen it to 32 bits.
    uint8_t timeStamp[3];
    uint8_t timeOffset[3];
} HciLeReadIsoTxSyncReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.97 LE Set CIG Parameters Command
#define HCI_LE_SET_CIG_PARAMETERS MAKE_OPCODE(0x0062, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t cisId;
    uint16_t maxSduMToS;
    uint16_t maxSduSToM;
    uint8_t phyMToS;
    uint8_t phySToM;
    uint8_t rtnMToS;
    uint8_t rtnSToM;
} HciLeCisConfigParam;

typedef struct {
    uint8_t cigId;
    uint8_t sduIntervalMToS[3];
    uint8_t sduIntervalSToM[3];
    uint8_t slaveClockAccuracy;
    uint8_t packing;
    uint8_t framing;
    uint8_t maxTransportLatencyMToS[2];
    uint8_t maxTransportLatencySToM[2];
    uint8_t cisCount;
    const HciLeCisConfigParam *cisConfig;
} HciLeSetCigParametersParam;

#define HCI_LE_CIS_COUNT_MAX 16
#define HCI_LE_BIS_COUNT_MAX 31
typedef struct {
    uint8_t status;
    uint8_t cigId;
    uint8_t cisCount;
    uint16_t cisHandles[HCI_LE_CIS_COUNT_MAX];
} HciLeSetCigParametersReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.98 LE Set CIG Parameters Test Command
#define HCI_LE_SET_CIG_PARAMS_TEST MAKE_OPCODE(0x0063, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t cisId;
    uint8_t nse;
    uint16_t maxSduMToS;
    uint16_t maxSduSToM;
    uint16_t maxPduMToS;
    uint16_t maxPduSToM;
    uint8_t phyMToS;
    uint8_t phySToM;
    uint8_t bnMToS;
    uint8_t bnSToM;
} HciLeSetCigParametersTestCisConfig;

typedef struct {
    uint8_t cigId;
    uint8_t sduIntervalMToS[3];
    uint8_t sduIntervalSToM[3];
    uint8_t ftMToS;
    uint8_t ftSToM;
    uint8_t isoInterval[2];
    uint8_t slaveClockAccuracy;
    uint8_t packing;
    uint8_t framing;
    uint8_t cisCount;
    const HciLeSetCigParametersTestCisConfig *cisConfig;
} HciLeSetCigParametersTestParam;

typedef struct {
    uint8_t status;
    uint8_t cigId;
    uint8_t cisCount;
    uint16_t cisHandles[HCI_LE_CIS_COUNT_MAX];
} HciLeSetCigParametersTestReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.99 LE Create CIS Command
#define HCI_LE_CREATE_CIS MAKE_OPCODE(0x0064, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t cisHandle;
    uint16_t aclHandle;
} HciLeCreateCisConfigParam;

typedef struct {
    uint8_t cisCount;
    const HciLeCreateCisConfigParam *cisConfig;
} HciLeCreateCisParam;

typedef HciStatusParam HciLeCreateCisReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.100 LE Remove CIG Command
#define HCI_LE_REMOVE_CIG MAKE_OPCODE(0x0065, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t cigId; // CIG Identifier
} HciLeRemoveCigParam;

// Vol 4 Part E 7.8.100: the Command Complete echoes the CIG_ID; it lets the
// receiver tell a stale Complete of a previous remove from the pending one.
typedef struct {
    uint8_t status;
    uint8_t cigId;
} HciLeRemoveCigReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.101 LE Accept CIS Request Command
#define HCI_LE_ACCEPT_CIS_REQUEST MAKE_OPCODE(0x0066, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t cisHandle;
} HciLeAcceptCisRequestParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.102 LE Reject CIS Request Command
#define HCI_LE_REJECT_CIS_REQUEST MAKE_OPCODE(0x0067, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t cisHandle;
    uint8_t reason;
} HciLeRejectCisRequestParam;

typedef struct {
    uint8_t status;
    uint16_t cisHandle;
} HciLeRejectCisRequestReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.103 LE Create BIG Command
#define HCI_LE_CREATE_BIG MAKE_OPCODE(0x0068, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t bigHandle;
    uint8_t advertisingHandle;
    uint8_t numBis;
    uint8_t sduInterval[3];
    uint16_t maxSdu;
    uint16_t maxTransportLatency;
    uint8_t rtn;
    uint8_t phy;
    uint8_t packing;
    uint8_t framing;
    uint8_t encryption;
    uint8_t broadcastCode[16];
} HciLeCreateBigParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 4 Part E
// 7.8.104 LE Create BIG Test Command
#define HCI_LE_CREATE_BIG_TEST MAKE_OPCODE(0x0069, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t bigHandle;
    uint8_t advertisingHandle;
    uint8_t numBis;
    uint8_t sduInterval[3];
    uint16_t isoInterval;
    uint8_t nse;
    uint16_t maxSdu;
    uint16_t maxPdu;
    uint8_t phy;
    uint8_t packing;
    uint8_t framing;
    uint8_t bn;
    uint8_t irc;
    uint8_t pto;
    uint8_t encryption;
    uint8_t broadcastCode[16];
} HciLeCreateBigTestParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.105 LE Terminate BIG Command
#define HCI_LE_TERMINATE_BIG MAKE_OPCODE(0x006A, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t bigHandle;
    uint8_t reason;
} HciLeTerminateBigParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.106 LE BIG Create Sync Command
#define HCI_LE_BIG_CREATE_SYNC MAKE_OPCODE(0x006B, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t bigHandle;
    uint16_t syncHandle;
    uint8_t encryption;
    uint8_t broadcastCode[16];
    uint8_t mse;
    uint8_t bigSyncTimeout[2];
    uint8_t numBis;
    // Array of BIS_Handle values of the BISes to synchronize to (Vol 4 Part E 7.8.106).
    // Each entry occupies sizeof(uint16_t) bytes on the wire, so the array must contain
    // numBis * 2 bytes total (HCI_LeBigCreateSync builds the command with
    // sizeof(uint16_t) * numBis bytes). The pointer stays uint8_t to stay assignable from
    // the ISO-layer API (IsoLeBigCreateSyncParam.bis, "array of BIS indices").
    const uint8_t *bis;
} HciLeBigCreateSyncParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.107 LE BIG Terminate Sync Command
#define HCI_LE_BIG_TERMINATE_SYNC MAKE_OPCODE(0x006C, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t bigHandle;
} HciLeBigTerminateSyncParam;

typedef struct {
    uint8_t status;
    uint8_t bigHandle;
} HciLeBigTerminateSyncReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.108 LE Request Peer SCA Command
#define HCI_LE_REQUEST_PEER_SCA MAKE_OPCODE(0x006D, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
} HciLeRequestPeerScaParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.109 LE Setup ISO Data Path Command
#define HCI_LE_SETUP_ISO_DATA_PATH MAKE_OPCODE(0x006E, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint8_t dataPathDirection;
    uint8_t dataPathId;
    uint8_t codecId[5];
    uint8_t controllerDelay[3];
    uint8_t codecConfigurationLength;
    const uint8_t *codecConfiguration;
} HciLeSetupIsoDataPathParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeSetupIsoDataPathReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.110 LE Remove ISO Data Path Command
#define HCI_LE_REMOVE_ISO_DATA_PATH MAKE_OPCODE(0x006F, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint8_t dataPathDirection;
} HciLeRemoveIsoDataPathParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeRemoveIsoDataPathReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.111 LE ISO Transmit Test Command
#define HCI_LE_ISO_TRANSMIT_TEST MAKE_OPCODE(0x0070, HCI_COMMAND_OGF_LE_CONTROLLER)
typedef struct {
    uint16_t connectionHandle;
    uint8_t payloadType;
} HciLeIsoTransmitTestParam;
typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeIsoTransmitTestReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.112 LE ISO Receive Test Command
#define HCI_LE_ISO_RECEIVE_TEST MAKE_OPCODE(0x0071, HCI_COMMAND_OGF_LE_CONTROLLER)
typedef struct {
    uint16_t connectionHandle;
    uint8_t payloadType;
} HciLeIsoReceiveTestParam;
typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeIsoReceiveTestReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.113 LE ISO Read Test Counters Command
#define HCI_LE_ISO_READ_TEST_COUNTERS MAKE_OPCODE(0x0072, HCI_COMMAND_OGF_LE_CONTROLLER)
typedef struct {
    uint16_t connectionHandle;
} HciLeIsoReadTestCountersParam;
typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint32_t receivedPacketCount;
    uint32_t missedPacketCount;
    uint32_t failedPacketCount;
} HciLeIsoReadTestCountersReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.114 LE ISO Test End Command
#define HCI_LE_ISO_TEST_END MAKE_OPCODE(0x0073, HCI_COMMAND_OGF_LE_CONTROLLER)
typedef struct {
    uint16_t connectionHandle;
} HciLeIsoTestEndParam;
typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint32_t receivedPacketCount;
    uint32_t missedPacketCount;
    uint32_t failedPacketCount;
} HciLeIsoTestEndReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.115 LE Set Host Feature Command
#define HCI_LE_SET_HOST_FEATURE MAKE_OPCODE(0x0074, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t bitNumber;
    uint8_t bitValue;
} HciLeSetHostFeatureParam;

typedef HciStatusParam HciLeSetHostFeatureReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.116 LE Read ISO Link Quality Command
#define HCI_LE_READ_ISO_LINK_QUALITY MAKE_OPCODE(0x0075, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
} HciLeReadIsoLinkQualityParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint32_t txUnackedPackets;
    uint32_t txFlushedPackets;
    uint32_t txLastSubeventPackets;
    uint32_t retransmittedPackets;
    uint32_t crcErrorPackets;
    uint32_t rxUnreceivedPackets;
    uint32_t duplicatePackets;
} HciLeReadIsoLinkQualityReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.117 LE Enhanced Read Transmit Power Level Command
#define HCI_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL MAKE_OPCODE(0x0076, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint8_t phy;
} HciLeEnhancedReadTransmitPowerLevelParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint8_t phy;
    int8_t currentTransmitPowerLevel;
    int8_t maxTransmitPowerLevel;
} HciLeEnhancedReadTransmitPowerLevelReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.118 LE Read Remote Transmit Power Level Command
#define HCI_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL MAKE_OPCODE(0x0077, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint8_t phy;
} HciLeReadRemoteTransmitPowerLevelParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.119 LE Set Path Loss Reporting Parameters Command
#define HCI_LE_SET_PATH_LOSS_REPORTING_PARAMETERS MAKE_OPCODE(0x0078, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint8_t highThreshold;
    uint8_t highHysteresis;
    uint8_t lowThreshold;
    uint8_t lowHysteresis;
    uint16_t minTimeSpent;
} HciLeSetPathLossReportingParametersParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeSetPathLossReportingParametersReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.120 LE Set Path Loss Reporting Enable Command
#define HCI_LE_SET_PATH_LOSS_REPORTING_ENABLE MAKE_OPCODE(0x0079, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint8_t enable;
} HciLeSetPathLossReportingEnableParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeSetPathLossReportingEnableReturnParam;

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.121 LE Set Transmit Power Reporting Enable Command
#define HCI_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE MAKE_OPCODE(0x007A, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint8_t localEnable;
    uint8_t remoteEnable;
} HciLeSetTransmitPowerReportingEnableParam;

typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
} HciLeSetTransmitPowerReportingEnableReturnParam;

// BLUETOOTH SPECIFICATION Version 5.3 | Vol 4, Part E
// 7.8.122 LE Set Data Related Address Changes command (RPA refresh trigger
// tied to advertising/scan-response data changes; see the amended 2024 spec).
#define HCI_LE_SET_DATA_RELATED_ADDRESS_CHANGES MAKE_OPCODE(0x007C, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t advertisingHandle; // Advertising_Handle 0x00~0xEF
    uint8_t changeReasons;     // bit 0: adv data changed; bit 1: scan resp data changed
} HciLeSetDataRelatedAddressChangesParam;

typedef struct {
    uint8_t status;
} HciLeSetDataRelatedAddressChangesReturnParam;

// BLUETOOTH SPECIFICATION Version 5.3 | Vol 4, Part E
// 7.8.123-7.8.124 Connection Subrating commands. Parameter layout verified
// against the amended 2024 spec tables: Continuation_Number is 2 octets with
// range 0x0000-0x01F3 (not 1 octet).
#define HCI_LE_SET_DEFAULT_SUBRATE MAKE_OPCODE(0x007D, HCI_COMMAND_OGF_LE_CONTROLLER)

// Shared Connection Subrating parameter limits (7.8.123/7.8.124, verified
// against the amended 2024 spec tables). Single maintenance point: the GAP
// and HCI validation gates (gap_le_subrate.c / hci_cmd_le_controller_5_3.c)
// reference these, so a limit change needs no cross-layer sync.
#define LE_SUBRATE_FACTOR_MIN              0x0001
#define LE_SUBRATE_FACTOR_MAX              0x01F4
#define LE_SUBRATE_MAX_LATENCY_MAX         0x01F3
#define LE_SUBRATE_CONTINUATION_MAX        0x01F3
#define LE_SUBRATE_SUPERVISION_TIMEOUT_MIN 0x000A
#define LE_SUBRATE_SUPERVISION_TIMEOUT_MAX 0x0C80
// Subrate_Max x (Max_Latency + 1) must not exceed 500 (7.8.124); beyond that
// the Controller returns Invalid HCI Command Parameters (0x12).
#define LE_SUBRATE_MAX_LATENCY_PRODUCT_MAX 500

typedef struct {
    uint16_t defaultSubrateMin;         // Subrate_Min 1~500 (0x0001~0x01F4)
    uint16_t defaultSubrateMax;         // Subrate_Max >= Subrate_Min
    uint16_t defaultMaxLatency;         // Max_Latency 0~499 (0x0000~0x01F3)
    uint16_t defaultContinuationNumber; // Continuation_Number 0~499 (0x0000~0x01F3)
    uint16_t defaultSupervisionTimeout; // Supervision_Timeout 100ms~32s, unit 10ms (0x000A~0x0C80)
} HciLeSetDefaultSubrateParam;

typedef struct {
    uint8_t status;
} HciLeSetDefaultSubrateReturnParam;

#define HCI_LE_SUBRATE_REQUEST MAKE_OPCODE(0x007E, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t connectionHandle;
    uint16_t subrateMin;
    uint16_t subrateMax;
    uint16_t maxLatency;
    uint16_t continuationNumber;        // Continuation_Number 2 octets, < Subrate_Max
    uint16_t supervisionTimeout;        // Supervision_Timeout 100ms~32s, unit 10ms
} HciLeSubrateRequestParam;

typedef struct {
    uint8_t status;
} HciLeSubrateRequestReturnParam;

#pragma pack(0)

#ifdef __cplusplus
}
#endif

// Included outside extern "C": the 5.0/5.1 definitions own their extern "C"
// wrapper and must not be nested inside another one.
#include "hci_def_le_cmd_5_0.h"
#include "hci_def_le_cmd_5_1.h"

#endif
