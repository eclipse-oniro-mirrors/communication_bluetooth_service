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

#ifndef HCI_LE_CONTROLLER_6_0_H
#define HCI_LE_CONTROLLER_6_0_H

#include <stdint.h>

#include "hci_def.h"

#ifdef __cplusplus
extern "C" {
#endif

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.128 LE Read All Local Supported Features Command (Command_Complete, no params)
int HCI_LeReadAllLocalSupportedFeatures(void);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.129 LE Read All Remote Features Command (Command_Status, -> subevent 0x2B)
int HCI_LeReadAllRemoteFeatures(const HciLeReadAllRemoteFeaturesParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.144
// LE Set Decision Data Command (Command_Complete)
int HCI_LeSetDecisionData(const HciLeSetDecisionDataParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.145
// LE Set Decision Instructions Command (Command_Complete)
int HCI_LeSetDecisionInstructions(const HciLeSetDecisionInstructionsParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.146 LE Add Device To Monitored Advertisers List Command (Command_Complete, 0x0098)
int HCI_LeAddDeviceToMonitoredAdvertisersList(const HciLeAddDeviceToMonitoredAdvertisersListParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.147 LE Remove Device From Monitored Advertisers List Command (Command_Complete, 0x0099)
int HCI_LeRemoveDeviceFromMonitoredAdvertisersList(const HciLeRemoveDeviceFromMonitoredAdvertisersListParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.148 LE Clear Monitored Advertisers List Command (Command_Complete, 0x009A)
int HCI_LeClearMonitoredAdvertisersList(void);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.150 LE Read Monitored Advertisers List Size Command (Command_Complete, 0x009B)
int HCI_LeReadMonitoredAdvertisersListSize(void);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.149 LE Enable Monitoring Advertisers Command (Command_Complete, 0x009C)
int HCI_LeEnableMonitoringAdvertisers(const HciLeEnableMonitoringAdvertisersParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E, 7.8.151
// LE Frame Space Update Command (Command_Status, -> subevent 0x35)
int HCI_LeFrameSpaceUpdate(const HciLeFrameSpaceUpdateParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.130 LE Read Local Supported Capabilities Command (Command_Complete, no params)
int HCI_LeCsReadLocalSupportedCapabilities(void);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.131 LE Read Remote Supported Capabilities Command (Command_Status, -> subevent 0x2C)
int HCI_LeCsReadRemoteSupportedCapabilities(const HciLeCsReadRemoteSupportedCapabilitiesParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.132 LE Write Cached Remote Supported Capabilities Command (Command_Complete)
int HCI_LeCsWriteCachedRemoteSupportedCapabilities(
    const HciLeCsWriteCachedRemoteSupportedCapabilitiesParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.133 LE CS Security Enable Command (Command_Status, -> subevent 0x2E)
int HCI_LeCsSecurityEnable(const HciLeCsSecurityEnableParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.134 LE CS Set Default Settings Command (Command_Complete)
int HCI_LeCsSetDefaultSettings(const HciLeCsSetDefaultSettingsParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.135 LE CS Read Remote FAE Table Command (Command_Status, -> subevent 0x2D)
int HCI_LeCsReadRemoteFaeTable(const HciLeCsReadRemoteFaeTableParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.136 LE CS Write Cached Remote FAE Table Command (Command_Complete)
int HCI_LeCsWriteCachedRemoteFaeTable(const HciLeCsWriteCachedRemoteFaeTableParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.137 LE CS Create Config Command (Command_Status, -> subevent 0x2F)
int HCI_LeCsCreateConfig(const HciLeCsCreateConfigParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.138 LE CS Remove Config Command (Command_Status, -> subevent 0x2F)
int HCI_LeCsRemoveConfig(const HciLeCsRemoveConfigParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.139 LE CS Set Channel Classification Command (Command_Complete)
int HCI_LeCsSetChannelClassification(const HciLeCsSetChannelClassificationParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.140 LE CS Set Procedure Parameters Command (Command_Complete)
int HCI_LeCsSetProcedureParameters(const HciLeCsSetProcedureParametersParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.141 LE CS Procedure Enable Command (Command_Status, -> subevent 0x30)
int HCI_LeCsProcedureEnable(const HciLeCsProcedureEnableParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.142 LE CS Test Command (Command_Complete, Status only). Sends the 30 B fixed prefix
// followed by the caller-owned override tail of prefix.overrideParametersLength octets.
int HCI_LeCsTest(const HciLeCsTestParam *param);

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.143 LE CS Test End Command (Command_Status, -> subevent 0x33). No parameters.
int HCI_LeCsTestEnd(void);

#ifdef __cplusplus
}
#endif

#endif
