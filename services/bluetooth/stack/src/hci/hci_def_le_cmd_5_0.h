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

#ifndef HCI_DEF_LE_CMD_5_0_H
#define HCI_DEF_LE_CMD_5_0_H

#include <stdint.h>

// Included first so HCI_COMMAND_OGF_LE_CONTROLLER, MAKE_OPCODE and the shared
// HciStatusParam/HciBdAddr types are available. hci_def_le_cmd.h re-includes
// this header at its tail; the include guard turns that into a no-op.
#include "hci_def_le_cmd.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.53 LE Set Extended Advertising Parameters Command
#define HCI_LE_SET_EXTENDED_ADVERTISING_PARAMETERS MAKE_OPCODE(0x0036, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t advertisingHandle;
    uint16_t advertisingEventProperties;
    uint8_t priAdvertisingIntervalMin[3];
    uint8_t priAdvertisingIntervalMax[3];
    uint8_t priAdvertisingChannelMap;
    uint8_t ownAddressType;
    uint8_t peerAddressType;
    uint8_t peerAddress[6];
    uint8_t advertisingFilterPolicy;
    uint8_t advertisingTxPower;
    uint8_t priAdvertisingPHY;
    uint8_t secondaryAdvertisingMaxSkip;
    uint8_t secondaryAdvertisingPHY;
    uint8_t advertisingSID;
    uint8_t scanRequestNotificationEnable;
} HciLeSetExtendedAdvertisingParametersParam;

typedef struct {
    uint8_t status;
    uint8_t selectedTxPower;
} HciLeSetExtendedAdvertisingParametersReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.54 LE Set Extended Advertising Data Command
#define HCI_LE_SET_EXTENDED_ADVERTISING_DATA MAKE_OPCODE(0x0037, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t advertisingHandle;
    uint8_t operation;
    uint8_t fragmentPreference;
    uint8_t advertisingDataLength;
    const uint8_t *advertisingData;
} HciLeSetExtendedAdvertisingDataParam;

typedef HciStatusParam HciLeSetExtendedAdvertisingDataReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.55 LE Set Extended Scan Response Data Command
#define HCI_LE_SET_EXTENDED_SCAN_RESPONSE_DATA MAKE_OPCODE(0x0038, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t advertisingHandle;
    uint8_t operation;
    uint8_t fragmentPreference;
    uint8_t scanResponseDataLength;
    const uint8_t *scanResponseData;
} HciLeSetExtendedScanResponseDataParam;

typedef HciStatusParam HciLeSetExtendedScanResponseDataReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.56 LE Set Extended Advertising Enable Command
#define HCI_LE_SET_EXTENDED_ADVERTISING_ENABLE MAKE_OPCODE(0x0039, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t adverHandle;
    uint16_t duration;
    uint8_t maxExtendAdvertisingEvents;
} HciLeExtendedAdvertisingParamSet;

typedef struct {
    uint8_t enable;
    uint8_t numberofSets;
    HciLeExtendedAdvertisingParamSet *sets;
} HciLeSetExtendedAdvertisingEnableParam;

typedef HciStatusParam HciLeSetExtendedAdvertisingEnableReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.57 LE Read Maximum Advertising Data Length Command
#define HCI_LE_READ_MAXIMUM_ADVERTISING_DATA_LENGTH MAKE_OPCODE(0x003A, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint16_t maximumAdvertisingDataLength;
} HciLeReadMaximumAdvertisingDataLengthReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.58 LE Read Number of Supported Advertising Sets Command
#define HCI_LE_READ_NUMBER_OF_SUPPORTED_ADVERTISING_SETS MAKE_OPCODE(0x003B, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint8_t numSupportedAdvertisingSets;
} HciLeReadNumberofSupportedAdvertisingSetsReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.59 LE Remove Advertising Set Command
#define HCI_LE_REMOVE_ADVERTISING_SET MAKE_OPCODE(0x003C, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t advertisingHandle;
} HciLeRemoveAdvertisingSetParam;

typedef HciStatusParam HciLeRemoveAdvertisingSetReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.60 LE Clear Advertising Sets Command
#define HCI_LE_CLEAR_ADVERTISING_SETS MAKE_OPCODE(0x003D, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef HciStatusParam HciLeClearAdvertisingSetsReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.61 LE Set Periodic Advertising Parameters Command
#define HCI_LE_SET_PERIODIC_ADVERTISING_PARAMETERS MAKE_OPCODE(0x003E, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t advertisingHandle;
    uint16_t periodicAdvertisingIntervalMin;
    uint16_t periodicAdvertisingIntervalMax;
    uint16_t periodicAdvertisingProperties;
} HciLeSetPeriodicAdvertisingParametersParam;

typedef HciStatusParam HciLeSetPeriodicAdvertisingParametersReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.62 LE Set Periodic Advertising Data Command
#define HCI_LE_SET_PERIODIC_ADVERTISING_DATA MAKE_OPCODE(0x003F, HCI_COMMAND_OGF_LE_CONTROLLER)

// Caution: HciLeSetPeriodicAdvertisingDataHostParam is defined as a non-packed,
// host-only struct in hci_le_controller_5_0.h. The advertisingData pointer must
// be serialized manually; do not pass sizeof(HciLeSetPeriodicAdvertisingDataHostParam)
// to HciAllocCmd.

typedef HciStatusParam HciLeSetPeriodicAdvertisingDataReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.63 LE Set Periodic Advertising Enable Command
#define HCI_LE_SET_PERIODIC_ADVERTISING_ENABLE MAKE_OPCODE(0x0040, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t enable;
    uint8_t advertisingHandle;
} HciLeSetPeriodicAdvertisingEnableParam;

typedef HciStatusParam HciLeSetPeriodicAdvertisingEnableReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.64 LE Set Extended Scan Parameters Command
#define HCI_LE_SET_EXTENDED_SCAN_PARAMETERS MAKE_OPCODE(0x0041, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t scanType;
    uint16_t scanInterval;
    uint16_t scanWindow;
} HciLeExtendedScanParametersSet;

// sets must point to an array with one entry per PHY bit set in scanningPhYs
// (i.e. popcount(scanningPhYs)), in ascending PHY bit order (LE 1M, LE 2M,
// LE Coded). There is no length field in the command, so the implementation
// reads exactly that many entries.
typedef struct {
    uint8_t ownAddressType;
    uint8_t scanningFilterPolicy;
    uint8_t scanningPhYs;
    HciLeExtendedScanParametersSet *sets;
} HciLeSetExtendedScanParametersParam;

typedef HciStatusParam HciLeSetExtendedScanParametersReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.65 LE Set Extended Scan Enable Command
#define HCI_LE_SET_EXTENDED_SCAN_ENABLE MAKE_OPCODE(0x0042, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t enable;
    uint8_t filterDuplicates;
    uint16_t duration;
    uint16_t period;
} HciLeSetExtendedScanEnableParam;

typedef HciStatusParam HciLeSetExtendedScanEnableReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.66 LE Extended Create Connection Command
#define HCI_LE_EXTENDED_CREATE_CONNECTION MAKE_OPCODE(0x0043, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t scanInterval;
    uint16_t scanWindow;
    uint16_t connIntervalMin;
    uint16_t connIntervalMax;
    uint16_t connLatency;
    uint16_t supervisionTimeout;
    uint16_t minimumCELength;
    uint16_t maximumCELength;
} HciLeConnectionParamSet;

#define LE_1M_PHY 0x01
#define LE_2M_PHY 0x02
#define LE_CODED_PHY 0x04

// sets must point to an array with one entry per PHY bit set in initiatingPhys
// (i.e. popcount(initiatingPhys)), in ascending PHY bit order (LE 1M, LE 2M,
// LE Coded). There is no length field in the command, so the implementation
// reads exactly that many entries.
typedef struct {
    uint8_t initiatingFilterPolicy;
    uint8_t ownAddressType;
    uint8_t peerAddressType;
    HciBdAddr peerAddress;
    uint8_t initiatingPhys;
    HciLeConnectionParamSet *sets;
} HciLeExtendedCreateConnectionParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.67 LE Periodic Advertising Create Sync Command
#define HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC MAKE_OPCODE(0x0044, HCI_COMMAND_OGF_LE_CONTROLLER)

// Filter policy for LE Periodic Advertising Create Sync Command
#define HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_FILTER_POLICY_DISABLED 0x00
#define HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_FILTER_POLICY_ENABLED 0x01

typedef struct {
    uint8_t filterPolicy;
    uint8_t advertisingSid;
    uint8_t advertiserAddressType;
    HciBdAddr advertiserAddress;
    uint16_t skip;
    uint16_t syncTimeout;
    uint8_t reserved;  // Reserved for future use, must be set to 0x00
} HciLePeriodicAdvertisingCreateSyncParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.68 LE Periodic Advertising Create Sync Cancel Command
#define HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_CANCEL MAKE_OPCODE(0x0045, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef HciStatusParam HciLePeriodicAdvertisingCreateSyncCancelReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.69 LE Periodic Advertising Terminate Sync Command
#define HCI_LE_PERIODIC_ADVERTISING_TERMINATE_SYNC MAKE_OPCODE(0x0046, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint16_t syncHandle;
} HciLePeriodicAdvertisingTerminateSyncParam;

typedef HciStatusParam HciLePeriodicAdvertisingTerminateSyncReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.70 LE Add Device To Periodic Advertiser List Command
#define HCI_LE_ADD_DEVICE_TO_PERIODIC_ADVERTISER_LIST MAKE_OPCODE(0x0047, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t advertiserAddressType;
    uint8_t advertiserAddress[6];
    uint8_t advertisingSid;
} HciLeAddDeviceToPeriodicAdvertiserListParam;

typedef HciStatusParam HciLeAddDeviceToPeriodicAdvertiserListReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.71 LE Remove Device From Periodic Advertiser List Command
#define HCI_LE_REMOVE_DEVICE_FROM_PERIODIC_ADVERTISER_LIST MAKE_OPCODE(0x0048, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t advertiserAddressType;
    uint8_t advertiserAddress[6];
    uint8_t advertisingSid;
} HciLeRemoveDeviceFromPeriodicAdvertiserListParam;

typedef HciStatusParam HciLeRemoveDeviceFromPeriodicAdvertiserListReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.72 LE Clear Periodic Advertiser List Command
#define HCI_LE_CLEAR_PERIODIC_ADVERTISER_LIST MAKE_OPCODE(0x0049, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef HciStatusParam HciLeClearPeriodicAdvertiserListReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.73 LE Read Periodic Advertiser List Size Command
#define HCI_LE_READ_PERIODIC_ADVERTISER_LIST_SIZE MAKE_OPCODE(0x004A, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint8_t periodicAdvertiserListSize;
} HciLeReadPeriodicAdvertiserListSizeReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.74 LE Read Transmit Power Command
#define HCI_LE_READ_TRANSMIT_POWER MAKE_OPCODE(0x004B, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    uint8_t minTxPower;
    uint8_t maxTxPower;
} HciLeReadTransmitPowerReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.75 LE Read RF Path Compensation Command
#define HCI_LE_READ_RF_PATH_COMPENSATION MAKE_OPCODE(0x004C, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    uint8_t status;
    int16_t rfTxPathCompensationValue;
    int16_t rfRxPathCompensationValue;
} HciLeReadRfPathCompensationReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.76 LE Write RF Path Compensation Command
#define HCI_LE_WRITE_RF_PATH_COMPENSATION MAKE_OPCODE(0x004D, HCI_COMMAND_OGF_LE_CONTROLLER)

typedef struct {
    int16_t rfTxPathCompensationValue;
    int16_t rfRxPathCompensationValue;
} HciLeWriteRfPathCompensationParam;

typedef HciStatusParam HciLeWriteRfPathCompensationReturnParam;

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.77 LE Set Privacy Mode Command
#define HCI_LE_SET_PRIVACY_MODE MAKE_OPCODE(0x004E, HCI_COMMAND_OGF_LE_CONTROLLER)

#define HCI_NETWORK_PRIVACY_MODE 0x00
#define HCI_DEVICE_PRIVACY_MODE 0x01

typedef struct {
    uint8_t peerIdentityAddressType;
    HciBdAddr peerIdentityAddress;
    uint8_t privacyMode;
} HciLeSetPrivacyModeParam;

typedef HciStatusParam HciLeSetPrivacyModeReturnParam;
#pragma pack(0)

#ifdef __cplusplus
}
#endif

#endif
