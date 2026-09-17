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

#include "hci_cmd_failure_le_controller_6_0.h"

#include <securec.h>

#include "btstack.h"
#include "platform/include/list.h"

#include "hci/evt/hci_evt.h"
#include "hci/hci.h"
#include "hci/hci_def.h"

#include "hci_cmd_failure.h"
#include "log.h"

void HciCmdOnLeReadAllLocalSupportedFeaturesFailed(uint8_t status, const void *param)
{
    HciLeReadAllLocalSupportedFeaturesReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadAllLocalSupportedFeaturesComplete != NULL) {
        callbacks->leReadAllLocalSupportedFeaturesComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x0088 is Command_Status; its result normally arrives as the 0x2B LE Meta
// subevent, so a failed send synthesizes that same event to close the request.
void HciCmdOnLeReadAllRemoteFeaturesFailed(uint8_t status, const void *param)
{
    HciLeReadAllRemoteFeaturesCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
        .maxRemotePage = 0,
        .maxValidPage = 0,
        .leFeatures = {0},
    };

    if (param != NULL) {
        eventParam.connectionHandle = ((const HciLeReadAllRemoteFeaturesParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadAllRemoteFeaturesComplete != NULL) {
        callbacks->leReadAllRemoteFeaturesComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x009D is Command_Status; its result normally arrives as the 0x35 LE Meta
// subevent, so a failed send or a Command_Status rejection synthesizes that same
// event to close the request. Only a host-issued request can reach this path, so
// initiator is reported as Local Host (0x00); the frame space fields are unknown.
void HciCmdOnLeFrameSpaceUpdateFailed(uint8_t status, const void *param)
{
    HciLeFrameSpaceUpdateCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
        .initiator = 0x00,
        .frameSpace = 0,
        .phys = 0,
        .spacingTypes = 0,
    };

    if (param != NULL) {
        eventParam.connectionHandle = ((const HciLeFrameSpaceUpdateParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leFrameSpaceUpdateComplete != NULL) {
        callbacks->leFrameSpaceUpdateComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// CC-twin convention row (same contract as the 0x0003 failure row): the
// Controller reports 0x0080 failures through the Command_Complete Return-Status,
// which the CC handler above dispatches; this path only closes a send timeout.
void HciCmdOnLeSetDecisionDataFailed(uint8_t status, const void *param)
{
    HciLeSetDecisionDataReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetDecisionDataComplete != NULL) {
        callbacks->leSetDecisionDataComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// CC-twin convention row (same contract as the 0x0080 failure row): the
// Controller reports 0x0081 failures through the Command_Complete Return-Status,
// which the CC handler above dispatches; this path only closes a send timeout.
void HciCmdOnLeSetDecisionInstructionsFailed(uint8_t status, const void *param)
{
    HciLeSetDecisionInstructionsReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leSetDecisionInstructionsComplete != NULL) {
        callbacks->leSetDecisionInstructionsComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// CC-twin convention row (same contract as the 0x0080/0x0081 rows): the
// Controller reports 0x0098 failures through the Command_Complete Return-Status,
// which the CC handler above dispatches; this path only closes a send timeout.
void HciCmdOnLeAddDeviceToMonitoredAdvertisersListFailed(uint8_t status, const void *param)
{
    HciLeAddDeviceToMonitoredAdvertisersListReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leAddDeviceToMonitoredAdvertisersListComplete != NULL) {
        callbacks->leAddDeviceToMonitoredAdvertisersListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// CC-twin convention row (same contract as the 0x0098 row): the Controller
// reports 0x0099 failures through the Command_Complete Return-Status, which the
// CC handler above dispatches; this path only closes a send timeout.
void HciCmdOnLeRemoveDeviceFromMonitoredAdvertisersListFailed(uint8_t status, const void *param)
{
    HciLeRemoveDeviceFromMonitoredAdvertisersListReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leRemoveDeviceFromMonitoredAdvertisersListComplete != NULL) {
        callbacks->leRemoveDeviceFromMonitoredAdvertisersListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// CC-twin convention row (same contract as the 0x0098 row): the Controller
// reports 0x009A failures through the Command_Complete Return-Status, which the
// CC handler above dispatches; this path only closes a send timeout.
void HciCmdOnLeClearMonitoredAdvertisersListFailed(uint8_t status, const void *param)
{
    HciLeClearMonitoredAdvertisersListReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leClearMonitoredAdvertisersListComplete != NULL) {
        callbacks->leClearMonitoredAdvertisersListComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// Read List Size CC carries Status + Number; on send timeout the capacity is
// unknown, so Number is reported as 0 (CC-twin contract as the rows above).
void HciCmdOnLeReadMonitoredAdvertisersListSizeFailed(uint8_t status, const void *param)
{
    HciLeReadMonitoredAdvertisersListSizeReturnParam returnParam = {
        .status = status,
        .number = 0,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leReadMonitoredAdvertisersListSizeComplete != NULL) {
        callbacks->leReadMonitoredAdvertisersListSizeComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// CC-twin convention row (same contract as the 0x0098 row): the Controller
// reports 0x009C failures through the Command_Complete Return-Status, which the
// CC handler above dispatches; this path only closes a send timeout.
void HciCmdOnLeEnableMonitoringAdvertisersFailed(uint8_t status, const void *param)
{
    HciLeEnableMonitoringAdvertisersReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leEnableMonitoringAdvertisersComplete != NULL) {
        callbacks->leEnableMonitoringAdvertisersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x0089 is Command_Complete and carries no parameters, so a timeout reaches this entry
// with param == NULL and synthesizes the CC-shaped return (Status only); a controller
// rejection arrives at the CC handler instead. The row is the one chokepoint for both,
// which is why it must tolerate a NULL param. The 28-B body stays zeroed (7.8.130).
void HciCmdOnLeCsReadLocalSupportedCapabilitiesFailed(uint8_t status, const void *param)
{
    (void)param;
    HciLeCsReadLocalSupportedCapabilitiesReturnParam returnParam = {
        .status = status,
        .capabilities = {0},
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsReadLocalSupportedCapabilitiesComplete != NULL) {
        callbacks->leCsReadLocalSupportedCapabilitiesComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x008A is Command_Status; its result normally arrives as the 0x2C LE Meta subevent.
// A command-level rejection (Command_Status with a nonzero status) and a command
// timeout both synthesize that same event to close the request.
void HciCmdOnLeCsReadRemoteSupportedCapabilitiesFailed(uint8_t status, const void *param)
{
    HciLeCsReadRemoteSupportedCapabilitiesCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
        .capabilities = {0},
    };

    if (param != NULL) {
        eventParam.connectionHandle =
            ((const HciLeCsReadRemoteSupportedCapabilitiesParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsReadRemoteSupportedCapabilitiesComplete != NULL) {
        callbacks->leCsReadRemoteSupportedCapabilitiesComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x008B is Command_Complete, so a controller rejection arrives at the CC handler,
// not here: this entry is reached on a command timeout, or on a Command_Complete whose
// return body is empty (HCI_MALFORMED_EVENT). It mirrors the CC twin's
// return shape (7.8.132) - Status + Connection_Handle; the 28-B write-back body is
// not reported back.
void HciCmdOnLeCsWriteCachedRemoteSupportedCapabilitiesFailed(uint8_t status, const void *param)
{
    HciLeCsWriteCachedRemoteSupportedCapabilitiesReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle =
            ((const HciLeCsWriteCachedRemoteSupportedCapabilitiesParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsWriteCachedRemoteSupportedCapabilitiesComplete != NULL) {
        callbacks->leCsWriteCachedRemoteSupportedCapabilitiesComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x008C is Command_Status; its result normally arrives as the 0x2E LE Meta subevent.
// A command-level rejection (Command_Status with a nonzero status) and a command
// timeout both synthesize that same event to close the request. Only Status +
// Connection_Handle form the 0x2E payload (7.7.65,41 p2434).
void HciCmdOnLeCsSecurityEnableFailed(uint8_t status, const void *param)
{
    HciLeCsSecurityEnableCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        eventParam.connectionHandle =
            ((const HciLeCsSecurityEnableParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsSecurityEnableComplete != NULL) {
        callbacks->leCsSecurityEnableComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x008D is Command_Complete, so a controller rejection arrives at the CC handler,
// not here: this entry is reached on a command timeout, or on a Command_Complete whose
// return body is empty (HCI_MALFORMED_EVENT). It mirrors the CC twin's
// return shape (7.8.134 p2818) - Status + Connection_Handle.
void HciCmdOnLeCsSetDefaultSettingsFailed(uint8_t status, const void *param)
{
    HciLeCsSetDefaultSettingsReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle =
            ((const HciLeCsSetDefaultSettingsParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsSetDefaultSettingsComplete != NULL) {
        callbacks->leCsSetDefaultSettingsComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x008E is Command_Status; its result normally arrives as the 0x2D LE Meta subevent.
// A command-level rejection (Command_Status with a nonzero status) and a command
// timeout both synthesize that same event to close the request. The 72-B table stays
// zeroed: a read that failed has no table, and 7.7.65,40 defines no "unavailable"
// encoding for the field to carry instead.
void HciCmdOnLeCsReadRemoteFaeTableFailed(uint8_t status, const void *param)
{
    HciLeCsReadRemoteFaeTableCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
        .remoteFaeTable = {0},
    };

    if (param != NULL) {
        eventParam.connectionHandle =
            ((const HciLeCsReadRemoteFaeTableParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsReadRemoteFaeTableComplete != NULL) {
        callbacks->leCsReadRemoteFaeTableComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x008F is Command_Complete, so a controller rejection arrives at the CC handler,
// not here: this entry is reached on a command timeout, or on a Command_Complete whose
// return body is empty (HCI_MALFORMED_EVENT). It mirrors the CC twin's
// return shape (7.8.136 p2821) - Status + Connection_Handle; the 72-B body is not
// reported back.
void HciCmdOnLeCsWriteCachedRemoteFaeTableFailed(uint8_t status, const void *param)
{
    HciLeCsWriteCachedRemoteFaeTableReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle =
            ((const HciLeCsWriteCachedRemoteFaeTableParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsWriteCachedRemoteFaeTableComplete != NULL) {
        callbacks->leCsWriteCachedRemoteFaeTableComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x0090 and 0x0091 are Command_Status; their result normally arrives as the 0x2F
// LE Meta subevent. A command-level rejection (Command_Status with a nonzero
// status) and a command timeout both synthesize that same event to close the
// request. Action cannot be recovered from the original command parameter (neither
// 0x0090 nor 0x0091 carries an Action field), so it is forced from the command that
// failed - that is the only thing the two thin wrappers below differ by, and it is
// why they exist rather than one shared row. Synthesizing the mirror as zeroed, not
// from the 0x0090 parameter, is deliberate - a failed create has no
// Controller-chosen Channel_Map/T_IP1_Time/... to report, and spec marks the whole
// tail valid only on success (7.7.65,42 p2436).
static void HciCmdOnLeCsConfigFailed(uint8_t status, const void *param, uint8_t action)
{
    HciLeCsConfigCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
        .configId = 0,
        .action = action,
    };

    if (param != NULL) {
        eventParam.connectionHandle = (action == LE_CS_CONFIG_ACTION_CREATED) ?
            ((const HciLeCsCreateConfigParam *)param)->connectionHandle :
            ((const HciLeCsRemoveConfigParam *)param)->connectionHandle;
        eventParam.configId = (action == LE_CS_CONFIG_ACTION_CREATED) ?
            ((const HciLeCsCreateConfigParam *)param)->configId :
            ((const HciLeCsRemoveConfigParam *)param)->configId;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsConfigComplete != NULL) {
        callbacks->leCsConfigComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

void HciCmdOnLeCsCreateConfigFailed(uint8_t status, const void *param)
{
    HciCmdOnLeCsConfigFailed(status, param, LE_CS_CONFIG_ACTION_CREATED);
}

void HciCmdOnLeCsRemoveConfigFailed(uint8_t status, const void *param)
{
    HciCmdOnLeCsConfigFailed(status, param, LE_CS_CONFIG_ACTION_REMOVED);
}

// 0x0092 is Command_Complete, so a controller rejection arrives at the CC handler,
// not here: this entry is reached on a command timeout, or on a Command_Complete whose
// return body is empty (HCI_MALFORMED_EVENT). It mirrors the CC twin's
// return shape (7.8.139 p2831) - Status alone. The 10-B classification carries no
// handle and no result to recover, so the original parameter is not read.
void HciCmdOnLeCsSetChannelClassificationFailed(uint8_t status, const void *param)
{
    (void)param;
    HciLeCsSetChannelClassificationReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsSetChannelClassificationComplete != NULL) {
        callbacks->leCsSetChannelClassificationComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x0093 is Command_Complete, so a controller rejection arrives at the CC handler,
// not here: this entry is reached on a command timeout, or on a Command_Complete whose
// return body is empty (HCI_MALFORMED_EVENT). It mirrors the CC twin's
// return shape (7.8.140 p2837) - Status + Connection_Handle; Config_ID is not echoed
// by the return either, so nothing beyond the handle can be recovered from param.
void HciCmdOnLeCsSetProcedureParametersFailed(uint8_t status, const void *param)
{
    HciLeCsSetProcedureParametersReturnParam returnParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
    };

    if (param != NULL) {
        returnParam.connectionHandle =
            ((const HciLeCsSetProcedureParametersParam *)param)->connectionHandle;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsSetProcedureParametersComplete != NULL) {
        callbacks->leCsSetProcedureParametersComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x0094 is Command_Status; its result normally arrives as the 0x30 LE Meta subevent.
// A command-level rejection (Command_Status with a nonzero status) and a command
// timeout both synthesize that same event to close the request. Connection_Handle and
// Config_ID come from the original command parameter. State is the state that stands,
// not the Enable that was asked for: a failed command leaves the procedure where it
// was, so a failed enable reports disabled - the value the spec's own rejected-enable
// flow draws (Vol 6, Part D, 6.37 p3629 "LE CS Procedure Enable Complete Event
// (Status=Error, State=disabled)") - and 7.7.65,43 p2442 ("When the State parameter is
// set to 0x00, all the remaining event parameters are ignored.") then drops the tail.
// The tail has no Controller-chosen schedule to report: Selected_TX_Power takes the
// spec's only "no value" code, 0x7F "Transmit power level is unavailable" (p2444),
// while subeventLen / subeventsPerEvent / maxProcedureLen stay zero because p2444 and
// p2445 give those no "no value" encoding and any value would forge a schedule.
void HciCmdOnLeCsProcedureEnableFailed(uint8_t status, const void *param)
{
    HciLeCsProcedureEnableCompleteEventParam eventParam = {
        .status = status,
        .connectionHandle = INVALID_CONNECTION_HANDLE,
        .configId = 0,
        .state = LE_CS_PROCEDURE_DISABLED,
        .toneAntennaConfigSelection = 0,
        .selectedTxPower = LE_CS_SELECTED_TX_POWER_UNAVAILABLE,
        .subeventLen = {0, 0, 0},
        .subeventsPerEvent = 0,
        .subeventInterval = 0,
        .eventInterval = 0,
        .procedureInterval = 0,
        .procedureCount = 0,
        .maxProcedureLen = 0,
    };

    if (param != NULL) {
        const HciLeCsProcedureEnableParam *cmdParam = (const HciLeCsProcedureEnableParam *)param;
        eventParam.connectionHandle = cmdParam->connectionHandle;
        eventParam.configId = cmdParam->configId;
        eventParam.state = (cmdParam->enable == LE_CS_PROCEDURE_ENABLED) ?
            LE_CS_PROCEDURE_DISABLED : LE_CS_PROCEDURE_ENABLED;
    } else {
        LOG_WARN("%{public}s: original command param unavailable, using invalid handle", __FUNCTION__);
    }

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsProcedureEnableComplete != NULL) {
        callbacks->leCsProcedureEnableComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x0095 is Command_Complete, so a controller rejection arrives at the CC handler,
// not here: this entry is reached on a command timeout, or on a Command_Complete whose
// return body is empty (HCI_MALFORMED_EVENT). It mirrors the CC twin's
// return shape (7.8.142 p2840) - Status alone; the test parameters are not echoed and
// nothing of them can be recovered from param.
void HciCmdOnLeCsTestFailed(uint8_t status, const void *param)
{
    (void)param;
    HciLeCsTestReturnParam returnParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsTestComplete != NULL) {
        callbacks->leCsTestComplete(&returnParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}

// 0x0096 is Command_Status; its result normally arrives as the 0x33 LE Meta subevent.
// A command-level rejection (Command_Status with a nonzero status) and a command
// timeout both synthesize that same subevent to close the request, with the failed
// Status forwarded as-is: 7.7.65,46 p2463 gives the body no other field, and every
// nonzero code means the same thing to the Host - the test is not the one that was
// asked for. The command carries no parameter, so nothing further is recoverable.
void HciCmdOnLeCsTestEndFailed(uint8_t status, const void *param)
{
    (void)param;
    HciLeCsTestEndCompleteEventParam eventParam = {
        .status = status,
    };

    HciEventCallbacks *callbacks = NULL;
    HCI_FOREACH_EVT_CALLBACKS_START(callbacks);
    if (callbacks->leCsTestEndComplete != NULL) {
        callbacks->leCsTestEndComplete(&eventParam);
    }
    HCI_FOREACH_EVT_CALLBACKS_END;
}
