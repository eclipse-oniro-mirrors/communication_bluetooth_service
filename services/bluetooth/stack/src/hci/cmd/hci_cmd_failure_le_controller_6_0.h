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

#ifndef HCI_CMD_FAILURE_LE_CONTROLLER_6_0_H
#define HCI_CMD_FAILURE_LE_CONTROLLER_6_0_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void HciCmdOnLeReadAllLocalSupportedFeaturesFailed(uint8_t status, const void *param);
void HciCmdOnLeReadAllRemoteFeaturesFailed(uint8_t status, const void *param);
void HciCmdOnLeFrameSpaceUpdateFailed(uint8_t status, const void *param);
void HciCmdOnLeSetDecisionDataFailed(uint8_t status, const void *param);
void HciCmdOnLeSetDecisionInstructionsFailed(uint8_t status, const void *param);
void HciCmdOnLeAddDeviceToMonitoredAdvertisersListFailed(uint8_t status, const void *param);
void HciCmdOnLeRemoveDeviceFromMonitoredAdvertisersListFailed(uint8_t status, const void *param);
void HciCmdOnLeClearMonitoredAdvertisersListFailed(uint8_t status, const void *param);
void HciCmdOnLeReadMonitoredAdvertisersListSizeFailed(uint8_t status, const void *param);
void HciCmdOnLeEnableMonitoringAdvertisersFailed(uint8_t status, const void *param);
void HciCmdOnLeCsReadLocalSupportedCapabilitiesFailed(uint8_t status, const void *param);
void HciCmdOnLeCsReadRemoteSupportedCapabilitiesFailed(uint8_t status, const void *param);
void HciCmdOnLeCsWriteCachedRemoteSupportedCapabilitiesFailed(uint8_t status, const void *param);
void HciCmdOnLeCsSecurityEnableFailed(uint8_t status, const void *param);
void HciCmdOnLeCsSetDefaultSettingsFailed(uint8_t status, const void *param);
void HciCmdOnLeCsReadRemoteFaeTableFailed(uint8_t status, const void *param);
void HciCmdOnLeCsWriteCachedRemoteFaeTableFailed(uint8_t status, const void *param);
void HciCmdOnLeCsCreateConfigFailed(uint8_t status, const void *param);
void HciCmdOnLeCsRemoveConfigFailed(uint8_t status, const void *param);
void HciCmdOnLeCsSetChannelClassificationFailed(uint8_t status, const void *param);
void HciCmdOnLeCsSetProcedureParametersFailed(uint8_t status, const void *param);
void HciCmdOnLeCsProcedureEnableFailed(uint8_t status, const void *param);
void HciCmdOnLeCsTestFailed(uint8_t status, const void *param);
void HciCmdOnLeCsTestEndFailed(uint8_t status, const void *param);

#ifdef __cplusplus
}
#endif

#endif
