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

#ifndef HCI_LE_CONTROLLER_5_0_H
#define HCI_LE_CONTROLLER_5_0_H

#include <stdint.h>

#include "hci_def.h"
#include "hci_def_le_cmd.h"

#ifdef __cplusplus
extern "C" {
#endif

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.33 LE Set Data Length Command
// The command returns an immediate status via Command Complete; the resulting
// data-length change is delivered asynchronously via the LE Data Length Change event.
int HCI_LeSetDataLength(const HciLeSetDataLengthParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.34 LE Read Suggested Default Data Length Command
int HCI_LeReadSuggestedDefaultDataLength(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.35 LE Write Suggested Default Data Length Command
int HCI_LeWriteSuggestedDefaultDataLength(const HciLeWriteSuggestedDefaultDataLengthParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.46 LE Read Maximum Data Length Command
int HCI_LeReadMaximumDataLength(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.47 LE Read PHY Command
int HCI_LeReadPhy(const HciLeReadPhyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.48 LE Set Default PHY Command
int HCI_LeSetDefaultPhy(const HciLeSetDefaultPhyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.49 LE Set PHY Command
// The command returns an immediate status via Command Complete; the final PHY
// update result is delivered asynchronously via the LE PHY Update Complete event.
int HCI_LeSetPhy(const HciLeSetPhyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.50 LE Enhanced Receiver Test Command
// Note: the command starts a test mode; results are retrieved asynchronously
// by later calling HCI_LeTestEnd().
int HCI_LeEnhancedReceiverTest(const HciLeEnhancedReceiverTestParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.51 LE Enhanced Transmitter Test Command
// Note: the command starts a test mode; results are retrieved asynchronously
// by later calling HCI_LeTestEnd().
int HCI_LeEnhancedTransmitterTest(const HciLeEnhancedTransmitterTestParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.61 LE Set Periodic Advertising Parameters Command
int HCI_LeSetPeriodicAdvertisingParameters(const HciLeSetPeriodicAdvertisingParametersParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.62 LE Set Periodic Advertising Data Command
// Host-side view of the command parameters. Because this struct contains a pointer,
// it must never be passed directly to HciAllocCmd / memcpy'd as a flat payload.
// HCI_LeSetPeriodicAdvertisingData serializes advertisingHandle, operation,
// advertisingDataLength, and the data pointed to by advertisingData manually, and
// performs a deep copy internally. The caller does not need to keep @c advertisingData
// valid after HCI_LeSetPeriodicAdvertisingData returns.
//
// @return BT_SUCCESS on success; BT_BAD_PARAM, BT_NO_MEMORY, or BT_OPERATION_FAILED on error.
typedef struct {
    uint8_t advertisingHandle;
    uint8_t operation;
    uint8_t advertisingDataLength;
    const uint8_t *advertisingData;
} HciLeSetPeriodicAdvertisingDataHostParam;

int HCI_LeSetPeriodicAdvertisingData(const HciLeSetPeriodicAdvertisingDataHostParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.63 LE Set Periodic Advertising Enable Command
int HCI_LeSetPeriodicAdvertisingEnable(const HciLeSetPeriodicAdvertisingEnableParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.67 LE Periodic Advertising Create Sync Command
// The command returns an immediate status via Command Complete; sync establishment
// is reported asynchronously via the LE Periodic Advertising Sync Established event.
int HCI_LePeriodicAdvertisingCreateSync(const HciLePeriodicAdvertisingCreateSyncParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.68 LE Periodic Advertising Create Sync Cancel Command
int HCI_LePeriodicAdvertisingCreateSyncCancel(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.69 LE Periodic Advertising Terminate Sync Command
int HCI_LePeriodicAdvertisingTerminateSync(const HciLePeriodicAdvertisingTerminateSyncParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.70 LE Add Device To Periodic Advertiser List Command
int HCI_LeAddDeviceToPeriodicAdvertiserList(const HciLeAddDeviceToPeriodicAdvertiserListParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.71 LE Remove Device From Periodic Advertiser List Command
int HCI_LeRemoveDeviceFromPeriodicAdvertiserList(const HciLeRemoveDeviceFromPeriodicAdvertiserListParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.72 LE Clear Periodic Advertiser List Command
int HCI_LeClearPeriodicAdvertiserList(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.73 LE Read Periodic Advertiser List Size Command
int HCI_LeReadPeriodicAdvertiserListSize(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.74 LE Read Transmit Power Command
int HCI_LeReadTransmitPower(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.75 LE Read RF Path Compensation Command
int HCI_LeReadRfPathCompensation(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.76 LE Write RF Path Compensation Command
int HCI_LeWriteRfPathCompensation(const HciLeWriteRfPathCompensationParam *param);

#ifdef __cplusplus
}
#endif

#endif
