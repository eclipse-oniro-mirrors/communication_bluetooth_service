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

#ifndef HCI_LE_CONTROLLER_5_1_H
#define HCI_LE_CONTROLLER_5_1_H

#include <stdint.h>

#include "hci_def.h"
#include "hci_def_le_cmd.h"

#ifdef __cplusplus
extern "C" {
#endif

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.78 LE Receiver Test Command [v3]
// Caution: the param struct contains an antennaIds pointer; the implementation
// serializes it manually and performs a deep copy internally.
int HCI_LeReceiverTestV3(const HciLeReceiverTestV3Param *param);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.79 LE Transmitter Test Command [v3]
// Caution: the param struct contains an antennaIds pointer; see HCI_LeReceiverTestV3.
int HCI_LeTransmitterTestV3(const HciLeTransmitterTestV3Param *param);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.80 LE Set Connectionless CTE Transmit Parameters Command
// Caution: the param struct contains an antennaIds pointer; see HCI_LeReceiverTestV3.
int HCI_LeSetConnectionlessCteTransmitParameters(const HciLeSetConnectionlessCteTransmitParametersParam *param);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.81 LE Set Connectionless CTE Transmit Enable Command
int HCI_LeSetConnectionlessCteTransmitEnable(const HciLeSetConnectionlessCteTransmitEnableParam *param);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.82 LE Set Connectionless IQ Sampling Enable Command
// Caution: the param struct contains an antennaIds pointer; see HCI_LeReceiverTestV3.
int HCI_LeSetConnectionlessIqSamplingEnable(const HciLeSetConnectionlessIqSamplingEnableParam *param);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.83 LE Set Connection CTE Receive Parameters Command
// Caution: the param struct contains an antennaIds pointer; see HCI_LeReceiverTestV3.
int HCI_LeSetConnectionCteReceiveParameters(const HciLeSetConnectionCteReceiveParametersParam *param);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.84 LE Set Connection CTE Transmit Parameters Command
// Caution: the param struct contains an antennaIds pointer; see HCI_LeReceiverTestV3.
int HCI_LeSetConnectionCteTransmitParameters(const HciLeSetConnectionCteTransmitParametersParam *param);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.85 LE Connection CTE Request Enable Command
int HCI_LeConnectionCteRequestEnable(const HciLeConnectionCteRequestEnableParam *param);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.86 LE Connection CTE Response Enable Command
int HCI_LeConnectionCteResponseEnable(const HciLeConnectionCteResponseEnableParam *param);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.87 LE Read Antenna Information Command
// Result (antenna count / switching sampling rates / max CTE length) is delivered
// via the leReadAntennaInformationComplete callback.
int HCI_LeReadAntennaInformation(void);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.88 LE Set Periodic Advertising Receive Enable Command
int HCI_LeSetPeriodicAdvertisingReceiveEnable(const HciLeSetPeriodicAdvertisingReceiveEnableParam *param);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.89 LE Periodic Advertising Sync Transfer Command
int HCI_LePeriodicAdvertisingSyncTransfer(const HciLePeriodicAdvertisingSyncTransferParam *param);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.90 LE Periodic Advertising Set Info Transfer Command
int HCI_LePeriodicAdvertisingSetInfoTransfer(const HciLePeriodicAdvertisingSetInfoTransferParam *param);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.91 LE Set Periodic Advertising Sync Transfer Parameters Command
int HCI_LeSetPeriodicAdvertisingSyncTransferParameters(
    const HciLeSetPeriodicAdvertisingSyncTransferParametersParam *param);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.92 LE Set Default Periodic Advertising Sync Transfer Parameters Command
int HCI_LeSetDefaultPeriodicAdvertisingSyncTransferParameters(
    const HciLeSetDefaultPeriodicAdvertisingSyncTransferParametersParam *param);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.93 LE Generate DHKey Command [v2]
// Key_Type 0x00 behaves like v1 (7.8.22); Key_Type 0x01 uses the fixed debug
// private key. Both generate the same "LE Generate DHKey Complete" event
// (Subevent 0x09).
int HCI_LeGenerateDhKeyV2(const HciLeGenerateDhKeyV2Param *param);

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.94 LE Modify Sleep Clock Accuracy Command
// Test-only helper that asks the Controller to switch its sleep clock to a
// more (0x00) or less (0x01) accurate one. If already at the requested extreme
// the Controller returns Limit Reached (0x43).
int HCI_LeModifySleepClockAccuracy(const HciLeModifySleepClockAccuracyParam *param);

#ifdef __cplusplus
}
#endif

#endif
