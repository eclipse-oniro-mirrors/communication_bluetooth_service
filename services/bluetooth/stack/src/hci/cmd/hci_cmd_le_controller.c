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

#include "hci/hci.h"
#include "hci/hci_le_controller_5_0.h"

#include <securec.h>

#include "btstack.h"
#include "platform/include/allocator.h"

#include "hci_cmd.h"

#define BITS_IN_BYTE 8

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// Spec limits for LE 5.0 command parameter validation.
#define LE_PHY_ALL_PHYS_MASK 0x03
// ALL_PHYS bit 0: host has no preference for the transmitter PHY.
#define LE_PHY_ALL_PHYS_TX_NO_PREFERENCE 0x01
// ALL_PHYS bit 1: host has no preference for the receiver PHY.
#define LE_PHY_ALL_PHYS_RX_NO_PREFERENCE 0x02
#define LE_PHY_PREFERENCES_MASK 0x07
#define LE_PHY_OPTIONS_MASK 0x0003

#define LE_TEST_CHANNEL_MAX 0x27
#define LE_TEST_PHY_MIN 0x01
#define LE_TEST_PHY_MAX 0x04
#define LE_TEST_RX_PHY_MAX 0x03
#define LE_TEST_MODULATION_INDEX_MAX 0x01
#define LE_TEST_TX_DATA_LENGTH_MAX 0xFF
#define LE_TEST_TX_PAYLOAD_MAX 0x07

#define PERIODIC_ADV_HANDLE_MAX 0xEF

// LE Set Extended Scan Parameters (7.8.64) and LE Extended Create Connection
// (7.8.66) accept LE 1M (bit0), LE 2M (bit1) and LE Coded (bit2) PHYs.
#define LE_SCANNING_PHYS_MASK (LE_1M_PHY | LE_2M_PHY | LE_CODED_PHY)
#define LE_INITIATING_PHYS_MASK (LE_1M_PHY | LE_2M_PHY | LE_CODED_PHY)
#define PERIODIC_ADV_INTERVAL_MIN 0x0006
#define PERIODIC_ADV_INTERVAL_MAX 0xFFFF
#define PERIODIC_ADV_DATA_LENGTH_MAX 0xFC
#define PERIODIC_ADV_PROPERTIES_MASK 0x0001
#define PERIODIC_ADV_SYNC_HANDLE_MAX 0x0EFF
#define PERIODIC_ADV_CREATE_SYNC_SKIP_MAX 0x01F3
#define PERIODIC_ADV_CREATE_SYNC_TIMEOUT_MIN 0x000A
#define PERIODIC_ADV_CREATE_SYNC_TIMEOUT_MAX 0x4000
#define RF_PATH_COMPENSATION_MIN (-1280)
#define RF_PATH_COMPENSATION_MAX 1280

// Maximum length of data that can be sent in a single HCI_LE_Set_Extended_Advertising_Data
// or HCI_LE_Set_Extended_Scan_Response_Data command (Core Spec v5.0, Vol 2, Part E, 7.8.54/7.8.55).
#define LE_EXTENDED_ADV_DATA_LENGTH_MAX 251
#define LE_EXTENDED_ADV_HANDLE_MAX 0xEF

// Common command parameter limits reused across multiple LE commands.
#define LE_CONNECTION_HANDLE_MAX 0x0EFF
#define LE_ENABLE_MAX 0x01
// Peer/Advertiser Address_Type values include public identity (0x02) and random identity (0x03).
#define LE_PEER_ADDRESS_TYPE_MAX 0x03
#define LE_ADVERTISING_SID_MAX 0x0F
#define LE_OPERATION_MAX 0x03
#define LE_PERIODIC_ADV_OPERATION_MAX 0x04
#define LE_PERIODIC_ADV_OPERATION_UNCHANGED_DATA 0x04
// Extended scan (7.8.64) and extended create connection (7.8.66) accept
// Filter_Policy 0x00-0x02; periodic advertising create sync (7.8.67)
// accepts only 0x00-0x01 (use-periodic-advertiser-list).
#define LE_FILTER_POLICY_MAX 0x02
#define LE_FILTER_POLICY_MAX_CREATE_SYNC 0x01

// LE Set Extended Advertising Parameters (7.8.53) field limits.
#define LE_EXT_ADV_EVENT_PROPERTIES_RESERVED_MASK 0xFF00 // bits 8-15 are RFU
#define LE_EXT_ADV_CHANNEL_MAP_MASK 0x07 // Primary_Advertising_Channel_Map is 3 bits
#define LE_EXT_ADV_PEER_ADDRESS_TYPE_MAX 0x01 // identity types (0x02/0x03) are not valid here
#define LE_EXT_ADV_FILTER_POLICY_MAX 0x03
#define LE_EXT_ADV_PHY_MIN 0x01 // LE 1M
#define LE_EXT_ADV_PHY_MAX 0x03 // LE Coded
#define LE_EXT_ADV_PHY_2M 0x02 // LE 2M; valid only for Secondary_Advertising_PHY (7.8.53)

// LE Set Extended Scan Enable (7.8.65) Filter_Duplicates: 0x00 Disable,
// 0x01 Enable, 0x02 Enable and reset; all other values are RFU.
#define LE_FILTER_DUPLICATES_MAX 0x02

// LE Periodic Advertising Create Sync (7.8.67) Advertiser_Address_Type accepts
// only 0x00 (Public Device Address) and 0x01 (Random Device Address); the
// identity types (0x02/0x03) are reserved for the events and other commands.
#define LE_PERIODIC_ADV_CREATE_SYNC_ADDR_TYPE_MAX 0x01

// Extended scan/connect parameter limits (Core Spec v5.0, Vol 2, Part E).
#define LE_OWN_ADDRESS_TYPE_MAX 0x03
#define LE_SCAN_TYPE_MAX 0x01
#define LE_SCAN_INTERVAL_MIN 0x0004
#define LE_SCAN_INTERVAL_MAX 0x4000
#define LE_SCAN_WINDOW_MIN 0x0004
#define LE_SCAN_WINDOW_MAX 0x4000
#define LE_CONN_INTERVAL_MIN 0x0006
#define LE_CONN_INTERVAL_MAX 0x0C80
#define LE_CONN_LATENCY_MAX 0x01F3
#define LE_CONN_SUPERVISION_TIMEOUT_MIN 0x000A
#define LE_CONN_SUPERVISION_TIMEOUT_MAX 0x0C80
#define LE_CE_LENGTH_MAX 0xFFFF

// Minimum txTime for a payload of N octets on the LE 1M PHY = (14 + N) * 8 us,
// i.e. preamble(1) + Access Address(4) + LL header(2) + MIC(4) + CRC(3) fixed
// overhead of 14 octets = 112 us (Vol 6 Part B 2.1: PDU up to 257 octets incl.
// MIC; 4.5.10 Table 4.3: min 328 us = (27+14)*8; 7.8.33: TxTime min 0x0148).
// TxOctets excludes the MIC, but the Controller counts the MIC on the air.
#define LE_DATA_LENGTH_TIME_PER_OCTET 8
#define LE_DATA_LENGTH_TIME_OVERHEAD 112

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.1 LE Set Event Mask Command
int HCI_LeSetEventMask(const HciLeSetEventMaskParam *param)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_EVENT_MASK, (void *)param, sizeof(HciLeSetEventMaskParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.2 LE Read Buffer Size Command
int HCI_LeReadBufferSize(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_BUFFER_SIZE, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.3 LE Read Local Supported Features Command
int HCI_LeReadLocalSupportedFeatures(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_LOCAL_SUPPORTED_FEATURES, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.4 LE Set Random Address Command
int HCI_LeSetRandomAddress(const HciLeSetRandomAddressParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_RANDOM_ADDRESS, (void *)param, sizeof(HciLeSetRandomAddressParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.5 LE Set Advertising Parameters Command
int HCI_LeSetAdvertisingParameters(const HciLeSetAdvertisingParametersParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd =
        HciAllocCmd(HCI_LE_SET_ADVERTISING_PARAMETERS, (void *)param, sizeof(HciLeSetAdvertisingParametersParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.6 LE Read Advertising Channel Tx Power Command
int HCI_LeReadAdvertisingChannelTxPower(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_ADVERTISING_CHANNEL_TX_POWER, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.7 LE Set Advertising Data Command
int HCI_LeSetAdvertisingData(const HciLeSetAdvertisingDataParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_ADVERTISING_DATA, (void *)param, sizeof(HciLeSetAdvertisingDataParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.8 LE Set Scan Response Data Command
int HCI_LeSetScanResponseData(const HciLeSetScanResponseDataParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_SCAN_RESPONSE_DATA, (void *)param, sizeof(HciLeSetScanResponseDataParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.9 LE Set Advertising Enable Command
int HCI_LeSetAdvertisingEnable(const HciLeSetAdvertisingEnableParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_ADVERTISING_ENABLE, (void *)param, sizeof(HciLeSetAdvertisingEnableParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.10 LE Set Scan Parameters Command
int HCI_LeSetScanParameters(const HciLeSetScanParametersParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_SCAN_PARAMETERS, (void *)param, sizeof(HciLeSetScanParametersParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.11 LE Set Scan Enable Command
int HCI_LeSetScanEnable(const HciLeSetScanEnableParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_SCAN_ENABLE, (void *)param, sizeof(HciLeSetScanEnableParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.12 LE Create Connection Command
int HCI_LeCreateConnection(const HciLeCreateConnectionParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_CREATE_CONNECTION, (void *)param, sizeof(HciLeCreateConnectionParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.13 LE Create Connection Cancel Command
int HCI_LeCreateConnectionCancel(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_CREATE_CONNECTION_CANCEL, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.14 LE Read WL Size Command
int HCI_LeReadWhiteListSize(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_WHITE_LIST_SIZE, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.15 LE Clear WL Command
int HCI_LeClearWhiteList(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_CLEAR_WHITE_LIST, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.16 LE Add Device To WL Command
int HCI_LeAddDeviceToWhiteList(const HciLeAddDeviceToWhiteListParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_ADD_DEVICE_TO_WHITE_LIST, (void *)param, sizeof(HciLeAddDeviceToWhiteListParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.17 LE Remove Device From WL Command
int HCI_LeRemoveDeviceFromWhiteList(const HciLeRemoveDeviceFromWhiteListParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd =
        HciAllocCmd(HCI_LE_REMOVE_DEVICE_FROM_WHITE_LIST, (void *)param, sizeof(HciLeRemoveDeviceFromWhiteListParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.18 LE Connection Update Command
int HCI_LeConnectionUpdate(const HciLeConnectionUpdateParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_CONNECTION_UPDATE, (void *)param, sizeof(HciLeConnectionUpdateParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.19 LE Set Host Channel Classification Command
int HCI_LeSetHostChannelClassification(const HciLeSetHostChannelClassificationParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(
        HCI_LE_SET_HOST_CHANNEL_CLASSIFICATION, (void *)param, sizeof(HciLeSetHostChannelClassificationParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.20 LE Read Channel Map Command
int HCI_LeReadChannelMap(const HciLeReadChannelMapParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_CHANNEL_MAP, (void *)param, sizeof(HciLeReadChannelMapParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.21 LE Read Remote Features Command
int HCI_LeReadRemoteFeatures(const HciLeReadRemoteFeaturesParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_REMOTE_FEATURES, (void *)param, sizeof(HciLeReadRemoteFeaturesParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.22 LE Encrypt Command
int HCI_LeEncrypt(const HciLeEncryptParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_ENCRYPT, (void *)param, sizeof(HciLeEncryptParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.23 LE Rand Command
int HCI_LeRand(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_RAND, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.24 LE Start Encryption Command
int HCI_LeStartEncryption(const HciLeStartEncryptionParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_START_ENCRYPTION, (void *)param, sizeof(HciLeStartEncryptionParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.25 LE Long Term Key Request Reply Command
int HCI_LeLongTermKeyRequestReply(const HciLeLongTermKeyRequestReplyParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd =
        HciAllocCmd(HCI_LE_LONG_TERM_KEY_REQUEST_REPLY, (void *)param, sizeof(HciLeLongTermKeyRequestReplyParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.26 LE Long Term Key Request Negative Reply Command
int HCI_LeLongTermKeyRequestNegativeReply(const HciLeLongTermKeyRequestNegativeReplyParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(
        HCI_LE_LONG_TERM_KEY_REQUEST_NEGATIVE_REPLY, (void *)param, sizeof(HciLeLongTermKeyRequestNegativeReplyParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.30 LE Test End Command
int HCI_LeTestEnd(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_TEST_END, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.31 LE Remote Connection Parameter Request Reply Command
int HCI_LeRemoteConnectionParameterRequestReply(const HciLeRemoteConnectionParameterRequestReplyParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST_REPLY,
        (void *)param,
        sizeof(HciLeRemoteConnectionParameterRequestReplyParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.32 LE Remote Connection Parameter Request Negative Reply Command
int HCI_LeRemoteConnectionParameterRequestNegativeReply(
    const HciLeRemoteConnectionParameterRequestNegativeReplyParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_REMOTE_CONNECTION_PARAMETER_REQUEST_NEGATIVE_REPLY,
        (void *)param,
        sizeof(HciLeRemoteConnectionParameterRequestNegativeReplyParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.36 LE Read Local P-256 Public Key Command
int HCI_LeReadLocalP256PublicKey(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_LOCAL_P256_PUBLIC_KEY, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.37 LE Generate DHKey Command
int HCI_LeGenerateDHKey(const HciLeGenerateDHKeyParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_GENERATE_DHKEY, (void *)param, sizeof(HciLeGenerateDHKeyParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.38 LE Add Device To Resolving List Command
int HCI_LeAddDeviceToResolvingList(const HciLeAddDeviceToResolvingListParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd =
        HciAllocCmd(HCI_LE_ADD_DEVICE_TO_RESOLVING_LIST, (void *)param, sizeof(HciLeAddDeviceToResolvingListParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.39 LE Remove Device From Resolving List Command
int HCI_LeRemoveDeviceFromResolvingList(const HciLeRemoveDeviceFromResolvingListParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(
        HCI_LE_REMOVE_DEVICE_FROM_RESOLVING_LIST, (void *)param, sizeof(HciLeRemoveDeviceFromResolvingListParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.40 LE Clear Resolving List Command
int HCI_LeClearResolvingList(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_CLEAR_RESOLVING_LIST, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.41 LE Read Resolving List Size Command
int HCI_LeReadResolvingListSize(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_RESOLVING_LIST_SIZE, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.44 LE Set Address Resolution Enable Command
int HCI_LeSetAddressResolutionEnable(const HciLeSetAddressResolutionEnableParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd =
        HciAllocCmd(HCI_LE_SET_ADDRESS_RESOLUTION_ENABLE, (void *)param, sizeof(HciLeSetAddressResolutionEnableParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.52 LE Set Advertising Set Random Address Command
int HCI_LeSetAdvertisingSetRandomAddress(const HciLeSetAdvertisingSetRandomAddressParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(
        HCI_LE_SET_ADVERTISING_SET_RANDOM_ADDRESS, (void *)param, sizeof(HciLeSetAdvertisingSetRandomAddressParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.53 LE Set Extended Advertising Parameters Command
int HCI_LeSetExtendedAdvertisingParameters(const HciLeSetExtendedAdvertisingParametersParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    // Field ranges from 7.8.53; the Controller itself validates the supported
    // combinations (e.g. legacy PDU constraints on Advertising_Event_Properties).
    if (param->advertisingHandle > LE_EXTENDED_ADV_HANDLE_MAX ||
        (param->advertisingEventProperties & LE_EXT_ADV_EVENT_PROPERTIES_RESERVED_MASK) != 0 ||
        (param->priAdvertisingChannelMap & ~LE_EXT_ADV_CHANNEL_MAP_MASK) != 0 ||
        param->ownAddressType > LE_OWN_ADDRESS_TYPE_MAX ||
        param->peerAddressType > LE_EXT_ADV_PEER_ADDRESS_TYPE_MAX ||
        param->advertisingFilterPolicy > LE_EXT_ADV_FILTER_POLICY_MAX ||
        // Primary_Advertising_PHY accepts only 0x01 (LE 1M) and 0x03 (LE Coded);
        // 0x02 (LE 2M) is Reserved in v5.0 (7.8.53).
        param->priAdvertisingPHY < LE_EXT_ADV_PHY_MIN || param->priAdvertisingPHY > LE_EXT_ADV_PHY_MAX ||
        param->priAdvertisingPHY == LE_EXT_ADV_PHY_2M ||
        param->secondaryAdvertisingPHY < LE_EXT_ADV_PHY_MIN || param->secondaryAdvertisingPHY > LE_EXT_ADV_PHY_MAX ||
        param->advertisingSID > LE_ADVERTISING_SID_MAX ||
        param->scanRequestNotificationEnable > LE_ENABLE_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(
        HCI_LE_SET_EXTENDED_ADVERTISING_PARAMETERS, (void *)param, sizeof(HciLeSetExtendedAdvertisingParametersParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.54 LE Set Extended Advertising Data Command
int HCI_LeSetExtendedAdvertisingData(const HciLeSetExtendedAdvertisingDataParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->advertisingDataLength > LE_EXTENDED_ADV_DATA_LENGTH_MAX ||
        (param->advertisingDataLength > 0 && param->advertisingData == NULL)) {
        return BT_BAD_PARAM;
    }

    const size_t length =
        sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + param->advertisingDataLength;
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    size_t index = 0;
    buf[index] = param->advertisingHandle;
    index += sizeof(uint8_t);
    buf[index] = param->operation;
    index += sizeof(uint8_t);
    buf[index] = param->fragmentPreference;
    index += sizeof(uint8_t);
    buf[index] = param->advertisingDataLength;
    index += sizeof(uint8_t);

    if (param->advertisingDataLength > 0) {
        (void)memcpy_s(buf + index, length - index, param->advertisingData, param->advertisingDataLength);
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_EXTENDED_ADVERTISING_DATA, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }

    int result = HciSendCmd(cmd);
    MEM_MALLOC.free(buf);
    return result;
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.55 LE Set Extended Scan Response Data Command
int HCI_LeSetExtendedScanResponseData(const HciLeSetExtendedScanResponseDataParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->scanResponseDataLength > LE_EXTENDED_ADV_DATA_LENGTH_MAX ||
        (param->scanResponseDataLength > 0 && param->scanResponseData == NULL)) {
        return BT_BAD_PARAM;
    }

    const size_t length =
        sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + param->scanResponseDataLength;
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    size_t index = 0;
    buf[index] = param->advertisingHandle;
    index += sizeof(uint8_t);
    buf[index] = param->operation;
    index += sizeof(uint8_t);
    buf[index] = param->fragmentPreference;
    index += sizeof(uint8_t);
    buf[index] = param->scanResponseDataLength;
    index += sizeof(uint8_t);
    if (param->scanResponseDataLength > 0) {
        (void)memcpy_s(buf + index, length - index, param->scanResponseData, param->scanResponseDataLength);
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_EXTENDED_SCAN_RESPONSE_DATA, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }

    int result = HciSendCmd(cmd);

    MEM_MALLOC.free(buf);
    return result;
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.56 LE Set Extended Advertising Enable Command
int HCI_LeSetExtendedAdvertisingEnable(const HciLeSetExtendedAdvertisingEnableParam *param)
{
    if (param == NULL || param->enable > LE_ENABLE_MAX) {
        return BT_BAD_PARAM;
    }

    // Core Spec v5.0 7.8.56: when enabling, at least one set is required. The
    // service layer, however, always sends Number_of_Sets=1 even when disabling
    // (where the spec says it must be 0); rk3568 controllers accept this and
    // disable only the listed set, so keep the disable-with-sets passthrough.
    // The HCI command payload length is stored in a single octet, so the number
    // of sets is also bounded by the maximum payload size.
    const size_t maxSets =
        (UINT8_MAX - sizeof(uint8_t) - sizeof(uint8_t)) / sizeof(HciLeExtendedAdvertisingParamSet);
    if (param->enable != 0 && param->numberofSets == 0) {
        return BT_BAD_PARAM;
    }
    if (param->numberofSets > maxSets || (param->numberofSets > 0 && param->sets == NULL)) {
        return BT_BAD_PARAM;
    }

    for (uint8_t i = 0; i < param->numberofSets; i++) {
        if (param->sets[i].adverHandle > LE_EXTENDED_ADV_HANDLE_MAX) {
            return BT_BAD_PARAM;
        }
    }

    const size_t length =
        sizeof(uint8_t) + sizeof(uint8_t) + sizeof(HciLeExtendedAdvertisingParamSet) * param->numberofSets;
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    uint16_t index = 0;
    buf[index] = param->enable;
    index += sizeof(uint8_t);
    buf[index] = param->numberofSets;
    index += sizeof(uint8_t);
    for (uint8_t i = 0; i < param->numberofSets; i++) {
        (void)memcpy_s(buf + index, length - index, param->sets + i, sizeof(HciLeExtendedAdvertisingParamSet));
        index += sizeof(HciLeExtendedAdvertisingParamSet);
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_EXTENDED_ADVERTISING_ENABLE, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }

    int result = HciSendCmd(cmd);

    MEM_MALLOC.free(buf);

    return result;
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.57 LE Read Maximum Advertising Data Length Command
int HCI_LeReadMaximumAdvertisingDataLength(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_MAXIMUM_ADVERTISING_DATA_LENGTH, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.58 LE Read Number of Supported Advertising Sets Command
int HCI_LeReadNumberofSupportedAdvertisingSets(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_NUMBER_OF_SUPPORTED_ADVERTISING_SETS, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.59 LE Remove Advertising Set Command
int HCI_LeRemoveAdvertisingSet(const HciLeRemoveAdvertisingSetParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_REMOVE_ADVERTISING_SET, (void *)param, sizeof(HciLeRemoveAdvertisingSetParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.60 LE Clear Advertising Sets Command
int HCI_LeClearAdvertisingSets(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_CLEAR_ADVERTISING_SETS, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

static int HciLeSetExtendedScanParametersCheck(
    const HciLeSetExtendedScanParametersParam *param, uint8_t *numberOfSets)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    // Extended scan supports LE 1M (bit0), LE 2M (bit1) and LE Coded (bit2).
    // The caller must provide at least numberOfSets entries in param->sets.
    if ((param->scanningPhYs & ~LE_SCANNING_PHYS_MASK) != 0 || param->scanningPhYs == 0 || param->sets == NULL) {
        return BT_BAD_PARAM;
    }

    uint8_t numSets = 0;
    for (uint8_t i = 0; i < BITS_IN_BYTE; i++) {
        if ((param->scanningPhYs >> i) & 0x01) {
            numSets++;
        }
    }

    if (param->ownAddressType > LE_OWN_ADDRESS_TYPE_MAX ||
        param->scanningFilterPolicy > LE_FILTER_POLICY_MAX) {
        return BT_BAD_PARAM;
    }

    for (uint8_t i = 0; i < numSets; i++) {
        if (param->sets[i].scanType > LE_SCAN_TYPE_MAX ||
            param->sets[i].scanInterval < LE_SCAN_INTERVAL_MIN ||
            param->sets[i].scanInterval > LE_SCAN_INTERVAL_MAX ||
            param->sets[i].scanWindow < LE_SCAN_WINDOW_MIN ||
            param->sets[i].scanWindow > LE_SCAN_WINDOW_MAX ||
            param->sets[i].scanWindow > param->sets[i].scanInterval) {
            return BT_BAD_PARAM;
        }
    }

    *numberOfSets = numSets;
    return BT_SUCCESS;
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.64 LE Set Extended Scan Parameters Command
int HCI_LeSetExtendedScanParameters(const HciLeSetExtendedScanParametersParam *param)
{
    uint8_t numberOfSets = 0;
    int ret = HciLeSetExtendedScanParametersCheck(param, &numberOfSets);
    if (ret != BT_SUCCESS) {
        return ret;
    }

    const size_t length =
        sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(HciLeExtendedScanParametersSet) * numberOfSets;
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    uint16_t index = 0;
    buf[index] = param->ownAddressType;
    index += sizeof(uint8_t);
    buf[index] = param->scanningFilterPolicy;
    index += sizeof(uint8_t);
    buf[index] = param->scanningPhYs;
    index += sizeof(uint8_t);

    for (uint8_t i = 0; i < numberOfSets; i++) {
        (void)memcpy_s(buf + index, length - index, param->sets + i, sizeof(HciLeExtendedScanParametersSet));
        index += sizeof(HciLeExtendedScanParametersSet);
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_EXTENDED_SCAN_PARAMETERS, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }
    int result = HciSendCmd(cmd);

    MEM_MALLOC.free(buf);

    return result;
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.65 LE Set Extended Scan Enable Command
int HCI_LeSetExtendedScanEnable(const HciLeSetExtendedScanEnableParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    // 7.8.65: Enable is 0x00/0x01, Filter_Duplicates is 0x00-0x02.
    if (param->enable > LE_ENABLE_MAX || param->filterDuplicates > LE_FILTER_DUPLICATES_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_EXTENDED_SCAN_ENABLE, (void *)param, sizeof(HciLeSetExtendedScanEnableParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

static int HciLeExtendedCreateConnectionCheck(
    const HciLeExtendedCreateConnectionParam *param, uint8_t *countOfSets)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    // Extended create connection supports LE 1M (bit0), LE 2M (bit1) and
    // LE Coded (bit2). The caller must provide at least countOfSets entries
    // in param->sets, one per PHY bit set, in ascending PHY bit order.
    if ((param->initiatingPhys & ~LE_INITIATING_PHYS_MASK) != 0 || param->initiatingPhys == 0 || param->sets == NULL) {
        return BT_BAD_PARAM;
    }

    uint8_t numSets = 0;
    if (param->initiatingPhys & LE_1M_PHY) {
        numSets++;
    }
    if (param->initiatingPhys & LE_2M_PHY) {
        numSets++;
    }
    if (param->initiatingPhys & LE_CODED_PHY) {
        numSets++;
    }

    if (param->initiatingFilterPolicy > LE_FILTER_POLICY_MAX ||
        param->ownAddressType > LE_OWN_ADDRESS_TYPE_MAX ||
        param->peerAddressType > LE_PEER_ADDRESS_TYPE_MAX) {
        return BT_BAD_PARAM;
    }

    *countOfSets = numSets;
    return BT_SUCCESS;
}

static int HciLeExtendedCreateConnectionCheckSets(
    const HciLeExtendedCreateConnectionParam *param, uint8_t countOfSets)
{
    for (uint8_t i = 0; i < countOfSets; i++) {
        if (param->sets[i].scanInterval < LE_SCAN_INTERVAL_MIN ||
            param->sets[i].scanInterval > LE_SCAN_INTERVAL_MAX ||
            param->sets[i].scanWindow < LE_SCAN_WINDOW_MIN ||
            param->sets[i].scanWindow > LE_SCAN_WINDOW_MAX ||
            param->sets[i].scanWindow > param->sets[i].scanInterval ||
            param->sets[i].connIntervalMin < LE_CONN_INTERVAL_MIN ||
            param->sets[i].connIntervalMin > LE_CONN_INTERVAL_MAX ||
            param->sets[i].connIntervalMax < LE_CONN_INTERVAL_MIN ||
            param->sets[i].connIntervalMax > LE_CONN_INTERVAL_MAX ||
            param->sets[i].connIntervalMin > param->sets[i].connIntervalMax ||
            param->sets[i].connLatency > LE_CONN_LATENCY_MAX ||
            param->sets[i].supervisionTimeout < LE_CONN_SUPERVISION_TIMEOUT_MIN ||
            param->sets[i].supervisionTimeout > LE_CONN_SUPERVISION_TIMEOUT_MAX ||
            param->sets[i].minimumCELength > param->sets[i].maximumCELength) {
            return BT_BAD_PARAM;
        }
    }
    return BT_SUCCESS;
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.66 LE Extended Create Connection Command
int HCI_LeExtendedCreateConnection(const HciLeExtendedCreateConnectionParam *param)
{
    uint8_t countOfSets = 0;
    int ret = HciLeExtendedCreateConnectionCheck(param, &countOfSets);
    if (ret == BT_SUCCESS) {
        ret = HciLeExtendedCreateConnectionCheckSets(param, countOfSets);
    }
    if (ret != BT_SUCCESS) {
        return ret;
    }

    const size_t length = sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(HciBdAddr) + sizeof(uint8_t) +
                          sizeof(HciLeConnectionParamSet) * countOfSets;
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    uint16_t index = 0;
    buf[index] = param->initiatingFilterPolicy;
    index += sizeof(uint8_t);
    buf[index] = param->ownAddressType;
    index += sizeof(uint8_t);
    buf[index] = param->peerAddressType;
    index += sizeof(uint8_t);

    (void)memcpy_s(buf + index, length - index, param->peerAddress.raw, sizeof(HciBdAddr));
    index += sizeof(HciBdAddr);

    buf[index] = param->initiatingPhys;
    index += sizeof(uint8_t);

    for (uint8_t i = 0; i < countOfSets; i++) {
        if (memcpy_s(buf + index, length - index, param->sets + i, sizeof(HciLeConnectionParamSet)) != EOK) {
            MEM_MALLOC.free(buf);
            return BT_OPERATION_FAILED;
        }
        index += sizeof(HciLeConnectionParamSet);
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_EXTENDED_CREATE_CONNECTION, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }
    int result = HciSendCmd(cmd);

    MEM_MALLOC.free(buf);

    return result;
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.77 LE Set Privacy Mode Command
int HCI_LeSetPrivacyMode(const HciLeSetPrivacyModeParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->privacyMode != HCI_NETWORK_PRIVACY_MODE && param->privacyMode != HCI_DEVICE_PRIVACY_MODE) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_PRIVACY_MODE, (void *)param, sizeof(HciLeSetPrivacyModeParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.33 LE Set Data Length Command
int HCI_LeSetDataLength(const HciLeSetDataLengthParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->connectionHandle > LE_CONNECTION_HANDLE_MAX ||
        param->txOctets < LE_DATA_LENGTH_MIN_OCTETS || param->txOctets > LE_DATA_LENGTH_MAX_OCTETS) {
        return BT_BAD_PARAM;
    }
    uint16_t minTime = (param->txOctets * LE_DATA_LENGTH_TIME_PER_OCTET) + LE_DATA_LENGTH_TIME_OVERHEAD;
    if (param->txTime < LE_DATA_LENGTH_MIN_TIME || param->txTime > LE_DATA_LENGTH_MAX_TIME ||
        param->txTime < minTime) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_DATA_LENGTH, (void *)param, sizeof(HciLeSetDataLengthParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.34 LE Read Suggested Default Data Length Command
int HCI_LeReadSuggestedDefaultDataLength(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_SUGGESTED_DEFAULT_DATA_LENGTH, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.35 LE Write Suggested Default Data Length Command
int HCI_LeWriteSuggestedDefaultDataLength(const HciLeWriteSuggestedDefaultDataLengthParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->suggestedMaxTxOctets < LE_DATA_LENGTH_MIN_OCTETS ||
        param->suggestedMaxTxOctets > LE_DATA_LENGTH_MAX_OCTETS) {
        return BT_BAD_PARAM;
    }
    uint16_t minTime = (param->suggestedMaxTxOctets * LE_DATA_LENGTH_TIME_PER_OCTET) + LE_DATA_LENGTH_TIME_OVERHEAD;
    if (param->suggestedMaxTxTime < LE_DATA_LENGTH_MIN_TIME ||
        param->suggestedMaxTxTime > LE_DATA_LENGTH_MAX_TIME ||
        param->suggestedMaxTxTime < minTime) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(
        HCI_LE_WRITE_SUGGESTED_DEFAULT_DATA_LENGTH, (void *)param, sizeof(HciLeWriteSuggestedDefaultDataLengthParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.46 LE Read Maximum Data Length Command
int HCI_LeReadMaximumDataLength(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_MAXIMUM_DATA_LENGTH, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.47 LE Read PHY Command
int HCI_LeReadPhy(const HciLeReadPhyParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->connectionHandle > LE_CONNECTION_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_PHY, (void *)param, sizeof(HciLeReadPhyParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.48 LE Set Default PHY Command
int HCI_LeSetDefaultPhy(const HciLeSetDefaultPhyParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if ((param->allPhys & ~LE_PHY_ALL_PHYS_MASK) != 0 ||
        (param->txPhys & ~LE_PHY_PREFERENCES_MASK) != 0 ||
        (param->rxPhys & ~LE_PHY_PREFERENCES_MASK) != 0 ||
        // Spec 7.8.48: if ALL_PHYS does not mark a direction as "no
        // preference", the corresponding PHYs field is used, in which case
        // at least one bit shall be set to 1.
        ((param->allPhys & LE_PHY_ALL_PHYS_TX_NO_PREFERENCE) == 0 && param->txPhys == 0) ||
        ((param->allPhys & LE_PHY_ALL_PHYS_RX_NO_PREFERENCE) == 0 && param->rxPhys == 0)) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_DEFAULT_PHY, (void *)param, sizeof(HciLeSetDefaultPhyParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.49 LE Set PHY Command
int HCI_LeSetPhy(const HciLeSetPhyParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->connectionHandle > LE_CONNECTION_HANDLE_MAX ||
        (param->allPhys & ~LE_PHY_ALL_PHYS_MASK) != 0 ||
        (param->txPhys & ~LE_PHY_PREFERENCES_MASK) != 0 ||
        (param->rxPhys & ~LE_PHY_PREFERENCES_MASK) != 0 ||
        (param->phyOptions & ~LE_PHY_OPTIONS_MASK) != 0 ||
        // Spec 7.8.49: if ALL_PHYS does not mark a direction as "no
        // preference", the corresponding PHYs field is used, in which case
        // at least one bit shall be set to 1.
        ((param->allPhys & LE_PHY_ALL_PHYS_TX_NO_PREFERENCE) == 0 && param->txPhys == 0) ||
        ((param->allPhys & LE_PHY_ALL_PHYS_RX_NO_PREFERENCE) == 0 && param->rxPhys == 0)) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_PHY, (void *)param, sizeof(HciLeSetPhyParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.50 LE Enhanced Receiver Test Command
int HCI_LeEnhancedReceiverTest(const HciLeEnhancedReceiverTestParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->rxChannel > LE_TEST_CHANNEL_MAX || param->phy < LE_TEST_PHY_MIN ||
        param->phy > LE_TEST_RX_PHY_MAX || param->modulationIndex > LE_TEST_MODULATION_INDEX_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_ENHANCED_RECEIVER_TEST, (void *)param, sizeof(HciLeEnhancedReceiverTestParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.51 LE Enhanced Transmitter Test Command
int HCI_LeEnhancedTransmitterTest(const HciLeEnhancedTransmitterTestParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->txChannel > LE_TEST_CHANNEL_MAX || param->phy < LE_TEST_PHY_MIN ||
        param->phy > LE_TEST_PHY_MAX || param->lengthOfTestData > LE_TEST_TX_DATA_LENGTH_MAX ||
        param->packetPayload > LE_TEST_TX_PAYLOAD_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd =
        HciAllocCmd(HCI_LE_ENHANCED_TRANSMITTER_TEST, (void *)param, sizeof(HciLeEnhancedTransmitterTestParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.61 LE Set Periodic Advertising Parameters Command
int HCI_LeSetPeriodicAdvertisingParameters(const HciLeSetPeriodicAdvertisingParametersParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->advertisingHandle > PERIODIC_ADV_HANDLE_MAX ||
        param->periodicAdvertisingIntervalMin < PERIODIC_ADV_INTERVAL_MIN ||
        param->periodicAdvertisingIntervalMin > PERIODIC_ADV_INTERVAL_MAX ||
        param->periodicAdvertisingIntervalMax < PERIODIC_ADV_INTERVAL_MIN ||
        param->periodicAdvertisingIntervalMax > PERIODIC_ADV_INTERVAL_MAX ||
        param->periodicAdvertisingIntervalMin > param->periodicAdvertisingIntervalMax ||
        (param->periodicAdvertisingProperties & ~PERIODIC_ADV_PROPERTIES_MASK) != 0) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(
        HCI_LE_SET_PERIODIC_ADVERTISING_PARAMETERS, (void *)param, sizeof(HciLeSetPeriodicAdvertisingParametersParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.62 LE Set Periodic Advertising Data Command
int HCI_LeSetPeriodicAdvertisingData(const HciLeSetPeriodicAdvertisingDataHostParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->advertisingHandle > PERIODIC_ADV_HANDLE_MAX || param->operation > LE_PERIODIC_ADV_OPERATION_MAX ||
        param->advertisingDataLength > PERIODIC_ADV_DATA_LENGTH_MAX ||
        (param->advertisingDataLength > 0 && param->advertisingData == NULL) ||
        (param->operation == LE_PERIODIC_ADV_OPERATION_UNCHANGED_DATA && param->advertisingDataLength != 0)) {
        return BT_BAD_PARAM;
    }

    const size_t length = sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + param->advertisingDataLength;
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    size_t index = 0;
    buf[index] = param->advertisingHandle;
    index += sizeof(uint8_t);
    buf[index] = param->operation;
    index += sizeof(uint8_t);
    buf[index] = param->advertisingDataLength;
    index += sizeof(uint8_t);

    if (param->advertisingDataLength > 0) {
        if (memcpy_s(buf + index, length - index, param->advertisingData, param->advertisingDataLength) != EOK) {
            MEM_MALLOC.free(buf);
            return BT_OPERATION_FAILED;
        }
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_PERIODIC_ADVERTISING_DATA, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }
    int result = HciSendCmd(cmd);

    MEM_MALLOC.free(buf);

    return result;
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.63 LE Set Periodic Advertising Enable Command
int HCI_LeSetPeriodicAdvertisingEnable(const HciLeSetPeriodicAdvertisingEnableParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->enable > LE_ENABLE_MAX || param->advertisingHandle > PERIODIC_ADV_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(
        HCI_LE_SET_PERIODIC_ADVERTISING_ENABLE, (void *)param, sizeof(HciLeSetPeriodicAdvertisingEnableParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.67 LE Periodic Advertising Create Sync Command
int HCI_LePeriodicAdvertisingCreateSync(const HciLePeriodicAdvertisingCreateSyncParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->filterPolicy > LE_FILTER_POLICY_MAX_CREATE_SYNC || param->advertisingSid > LE_ADVERTISING_SID_MAX ||
        param->advertiserAddressType > LE_PERIODIC_ADV_CREATE_SYNC_ADDR_TYPE_MAX ||
        param->skip > PERIODIC_ADV_CREATE_SYNC_SKIP_MAX ||
        param->syncTimeout < PERIODIC_ADV_CREATE_SYNC_TIMEOUT_MIN ||
        param->syncTimeout > PERIODIC_ADV_CREATE_SYNC_TIMEOUT_MAX || param->reserved != 0) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(
        HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC, (void *)param, sizeof(HciLePeriodicAdvertisingCreateSyncParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.68 LE Periodic Advertising Create Sync Cancel Command
int HCI_LePeriodicAdvertisingCreateSyncCancel(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_CANCEL, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.69 LE Periodic Advertising Terminate Sync Command
int HCI_LePeriodicAdvertisingTerminateSync(const HciLePeriodicAdvertisingTerminateSyncParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->syncHandle > PERIODIC_ADV_SYNC_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(
        HCI_LE_PERIODIC_ADVERTISING_TERMINATE_SYNC, (void *)param, sizeof(HciLePeriodicAdvertisingTerminateSyncParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.70 LE Add Device To Periodic Advertiser List Command
int HCI_LeAddDeviceToPeriodicAdvertiserList(const HciLeAddDeviceToPeriodicAdvertiserListParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->advertiserAddressType > LE_PEER_ADDRESS_TYPE_MAX ||
        param->advertisingSid > LE_ADVERTISING_SID_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_ADD_DEVICE_TO_PERIODIC_ADVERTISER_LIST, (void *)param,
        sizeof(HciLeAddDeviceToPeriodicAdvertiserListParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.71 LE Remove Device From Periodic Advertiser List Command
int HCI_LeRemoveDeviceFromPeriodicAdvertiserList(const HciLeRemoveDeviceFromPeriodicAdvertiserListParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->advertiserAddressType > LE_PEER_ADDRESS_TYPE_MAX ||
        param->advertisingSid > LE_ADVERTISING_SID_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_REMOVE_DEVICE_FROM_PERIODIC_ADVERTISER_LIST, (void *)param,
        sizeof(HciLeRemoveDeviceFromPeriodicAdvertiserListParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.72 LE Clear Periodic Advertiser List Command
int HCI_LeClearPeriodicAdvertiserList(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_CLEAR_PERIODIC_ADVERTISER_LIST, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.73 LE Read Periodic Advertiser List Size Command
int HCI_LeReadPeriodicAdvertiserListSize(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_PERIODIC_ADVERTISER_LIST_SIZE, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.74 LE Read Transmit Power Command
int HCI_LeReadTransmitPower(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_TRANSMIT_POWER, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.75 LE Read RF Path Compensation Command
int HCI_LeReadRfPathCompensation(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_RF_PATH_COMPENSATION, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.76 LE Write RF Path Compensation Command
int HCI_LeWriteRfPathCompensation(const HciLeWriteRfPathCompensationParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    if (param->rfTxPathCompensationValue < RF_PATH_COMPENSATION_MIN ||
        param->rfTxPathCompensationValue > RF_PATH_COMPENSATION_MAX ||
        param->rfRxPathCompensationValue < RF_PATH_COMPENSATION_MIN ||
        param->rfRxPathCompensationValue > RF_PATH_COMPENSATION_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd =
        HciAllocCmd(HCI_LE_WRITE_RF_PATH_COMPENSATION, (void *)param, sizeof(HciLeWriteRfPathCompensationParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}
