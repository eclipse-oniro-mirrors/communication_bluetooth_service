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

#ifndef HCI_EVT_CALLBACKS_H
#define HCI_EVT_CALLBACKS_H

#include <stdbool.h>
#include <stdint.h>

#include "hci_def.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    // Cmds

    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.2 Inquiry Cancel Command
    void (*inquiryCancelComplete)(const HciInquiryCancelReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.3 Periodic Inquiry Mode Command
    void (*periodicInquiryModeComplete)(const HciPeriodicInquiryModeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.4 Exit Periodic Inquiry Mode Command
    void (*exitPeriodicInquiryModeComplete)(const HciExitPeriodicInquiryModeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.7 Create Connection Cancel Command
    void (*createConnectionCancelComplete)(const HciCreateConnectionCancelReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.10 Link Key Request Reply Command
    void (*linkKeyRequestReplyComplete)(const HciLinkKeyRequestReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.11 Link Key Request Negative Reply Command
    void (*linkKeyRequestNegativeReplyComplete)(const HciLinkKeyRequestNegativeReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.12 PIN Code Request Reply Command
    void (*pinCodeRequestReplyComplete)(const HciPinCodeRequestReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.13 PIN Code Request Negative Reply Command
    void (*pinCodeRequestNegativeReplyComplete)(const HciPinCodeRequestNegativeReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.20 Remote Name Request Cancel Command
    void (*remoteNameRequestCancelComplete)(const HciRemoteNameRequestCancelReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.25 Read LMP Handle Command
    void (*readLmpHandleComplete)(const HciReadLmpHandleReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.29 IO Capability Request Reply Command
    void (*ioCapabilityRequestReplyComplete)(const HciIOCapabilityRequestReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.30 User Confirmation Request Reply Command
    void (*userConfirmationRequestReplyComplete)(const HciUserConfirmationRequestReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.31 User Confirmation Request Negative Reply Command
    void (*userConfirmationRequestNegativeReplyComplete)(
        const HciUserConfirmationRequestNegativeReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.32 User Passkey Request Reply Command
    void (*userPasskeyRequestReplyComplete)(const HciUserPasskeyRequestReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.33 User Passkey Request Negative Reply Command
    void (*userPasskeyRequestNegativeReplyComplete)(const HciUserPasskeyRequestNegativeReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.34 Remote OOB Data Request Reply Command
    void (*remoteOOBDataRequestReplyComplete)(const HciRemoteOobDataRequestReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.35 Remote OOB Data Request Negative Reply Command
    void (*remoteOOBDataRequestNegativeReplyComplete)(
        const HciRemoteOobDataRequestNegativeReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.36 IO Capability Request Negative Reply Command
    void (*iOCapabilityRequestNegativeReplyComplete)(const HciIoCapabilityRequestNegativeReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.43 Logical Link Cancel Command
    void (*logicalLinkCancelComplete)(const HciLogicalLinkCancelReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.48 Truncated Page Cancel Command
    void (*truncatedPageCancelComplete)(const HciTruncatedPageCancelReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.49 Set Connectionless Slave Broadcast Command
    void (*setConnectionlessSlaveBroadcastComplete)(const HciSetConnectionlessSlaveBroadcastReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.50 Set Connectionless Slave Broadcast Receive Command
    void (*setConnectionlessSlaveBroadcastReceiveComplete)(
        const HciSetConnectionlessSlaveBroadcastReceiveReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.1.53 Remote OOB Extended Data Request Reply Command
    void (*remoteOOBExtendedDataRequestReplyComplete)(
        const HciRemoteOobExtendedDataRequestReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.2.7 Role Discovery Command
    void (*roleDiscoveryComplete)(const HciRoleDiscoveryReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.2.9 Read Link Policy Settings Command
    void (*readLinkPolicySettingsComplete)(const HciReadLinkPolicySettingsReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.2.10 Write Link Policy Settings Command
    void (*writeLinkPolicySettingsComplete)(const HciWriteLinkPolicySettingsReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.2.11 Read Default Link Policy Settings Command
    void (*readDefaultLinkPolicySettingsComplete)(const HciReadDefaultLinkPolicySettingsReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.2.12 Write Default Link Policy Settings Command
    void (*writeDefaultLinkPolicySettingsComplete)(const HciWriteDefaultLinkPolicySettingsReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.2.14 Sniff Subrating Command
    void (*sniffSubratingComplete)(const HciSniffSubratingReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.1 Set Event Mask Command
    void (*setEventMaskComplete)(const HciSetEventMaskReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.2 Reset Command
    void (*resetComplete)(const HciResetReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.3 Set Event Filter Command
    void (*setEventFilterComplete)(const HciSetEventFilterReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.4 Flush Command
    void (*flushComplete)(const HciFlushReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.5 Read PIN Type Command
    void (*readPinTypeComplete)(const HciReadPinTypeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.6 Write PIN Type Command
    void (*writePinTypeComplete)(const HciWritePinTypeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.7 Create New Unit Key Command
    void (*createNewUnitKeyComplete)(const HciCreateNewUnitKeyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.8 Read Stored Link Key Command
    void (*readStoredLinkKeyComplete)(const HciReadStoredLinkKeyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.9 Write Stored Link Key Command
    void (*writeStoredLinkKeyComplete)(const HciWriteStoredLinkKeyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.10 Delete Stored Link Key Command
    void (*deleteStoredLinkKeyComplete)(const HciDeleteStoredLinkKeyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.11 Write Local Name Command
    void (*writeLocalNameComplete)(const HciWriteLocalNameReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.12 Read Local Name Command
    void (*readLocalNameComplete)(const HciReadLocalNameReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.13 Read Connection Accept Timeout Command
    void (*readConnectionAcceptTimeoutComplete)(const HciReadConnectionAcceptTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.14 Write Connection Accept Timeout Command
    void (*writeConnectionAcceptTimeoutComplete)(const HciWriteConnectionAcceptTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.15 Read Page Timeout Command
    void (*readPageTimeoutComplete)(const HciReadPageTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.16 Write Page Timeout Command
    void (*writePageTimeoutComplete)(const HciWritePageTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.17 Read Scan Enable Command
    void (*readScanEnableComplete)(const HciReadScanEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.18 Write Scan Enable Command
    void (*writeScanEnableComplete)(const HciWriteScanEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.19 Read Page Scan Command
    void (*readPageScanActivityComplete)(const HciReadPageScanActivityReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.20 Write Page Scan Command
    void (*writePageScanActivityComplete)(const HciWritePageScanActivityReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.21 Read Inquiry Scan Command
    void (*readInquiryScanActivityComplete)(const HciReadInquiryScanActivityReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.22 Write Inquiry Scan Command
    void (*writeInquiryScanActivityComplete)(const HciWriteInquiryScanActivityReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.23 Read Authentication Enable Command
    void (*readAuthenticationEnableComplete)(const HciReadAuthenticationEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.24 Write Authentication Enable Command
    void (*writeAuthenticationEnableComplete)(const HciWriteAuthenticationEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.25 Read Class of Device Command
    void (*readClassofDeviceComplete)(const HciReadClassofDeviceReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.26 Write Class of Device Command
    void (*writeClassofDeviceComplete)(const HciWriteClassofDeviceReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.27 Read Voice Setting Command
    void (*readVoiceSettingComplete)(const HciReadVoiceSettingReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.28 Write Voice Setting Command
    void (*writeVoiceSettingComplete)(const HciWriteVoiceSettingParamReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.29 Read Automatic Flush Timeout Command
    void (*readAutomaticFlushTimeoutComplete)(const HciReadAutomaticFlushTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.30 Write Automatic Flush Timeout Command
    void (*writeAutomaticFlushTimeoutComplete)(const HciWriteAutomaticFlushTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.31 Read Num Broadcast Retransmissions Command
    void (*readNumBroadcastRetransmissionsComplete)(const HciReadNumBroadcastRetransmissionsReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.32 Write Num Broadcast Retransmissions Command
    void (*writeNumBroadcastRetransmissionsComplete)(const HciWriteNumBroadcastRetransmissionsReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.33 Read Hold Mode Command
    void (*readHoldModeActivityComplete)(const HciReadHoldModeActivityReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.34 Write Hold Mode Command
    void (*writeHoldModeActivityComplete)(const HciWriteHoldModeActivityReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.35 Read Transmit Power Level Command
    void (*readTransmitPowerLevelComplete)(const HciReadTransmitPowerLevelReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.36 Read Synchronous Flow Control Enable Command
    void (*readSynchronousFlowControlEnableComplete)(const HciReadSynchronousFlowControlEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.37 Write Synchronous Flow Control Enable Command
    void (*writeSynchronousFlowControlEnableComplete)(
        const HciWriteSynchronousFlowControlEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.38 Set Controller To Host Flow Control Command
    void (*setControllerToHostFlowControlComplete)(const HciSetControllerToHostFlowControlReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.39 Host Buffer Size Command
    void (*hostBufferSizeComplete)(const HciHostBufferSizeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.41 Read Link Supervision Timeout Command
    void (*readLinkSupervisionTimeoutComplete)(const HciReadLinkSupervisionTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.42 Write Link Supervision Timeout Command
    void (*writeLinkSupervisionTimeoutComplete)(const HciWriteLinkSupervisionTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.43 Read Number Of Supported IAC Command
    void (*readNumberOfSupportedIacComplete)(const HciReadNumberOfSupportedIacReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.44 Read Current IAC LAP Command
    void (*readCurrentIacLapComplete)(const HciReadCurrentIacLapReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.45 Write Current IAC LAP Command
    void (*writeCurrentIacLapComplete)(const HciWriteCurrentIacLapReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.46 Set AFH Host Channel Classification Command
    void (*setAfhHostChannelClassificationComplete)(const HciSetAfhHostChannelClassificationReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.47 Read Inquiry Scan Type Command
    void (*readInquiryScanTypeComplete)(const HciReadInquiryScanTypeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.48 Write Inquiry Scan Type Command
    void (*writeInquiryScanTypeComplete)(const HciWriteInquiryScanTypeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.49 Read Inquiry Mode Command
    void (*readInquiryModeComplete)(const HciReadInquiryModeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.50 Write Inquiry Mode Command
    void (*writeInquiryModeComplete)(const HciWriteInquiryModeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.51 Read Page Scan Type Command
    void (*readPageScanTypeComplete)(const HciReadPageScanTypeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.52 Write Page Scan Type Command
    void (*writePageScanTypeComplete)(const HciWritePageScanTypeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.53 Read AFH Channel Assessment Mode Command
    void (*readAfhChannelAssessmentModeComplete)(const HciReadAfhChannelAssessmentModeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.54 Write AFH Channel Assessment Mode Command
    void (*writeAfhChannelAssessmentModeComplete)(const HciWriteAfhChannelAssessmentModeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.55 Read Extended Inquiry Response Command
    void (*readExtendedInquiryResponseComplete)(const HciReadExtendedInquiryResponseReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.56 Write Extended Inquiry Response Command
    void (*writeExtendedInquiryResponseComplete)(const HciWriteExtendedInquiryResponseReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.58 Read Simple Pairing Mode Command
    void (*readSimplePairingModeComplete)(const HciReadSimplePairingModeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.59 Write Simple Pairing Mode Command
    void (*writeSimplePairingModeComplete)(const HciWriteSimplePairingModeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.60 Read Local OOB Data Command
    void (*readLocalOOBDataComplete)(const HciReadLocalOOBDataReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.61 Read Inquiry Response Transmit Power Level Command
    void (*readInquiryResponseTransmitPowerLevelComplete)(
        const HciReadInquiryResponseTransmitPowerLevelReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.62 Write Inquiry Transmit Power Level Command
    void (*writeInquiryTransmitPowerLevelComplete)(const HciWriteInquiryTransmitPowerLevelReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.63 Send Keypress Notification Command
    void (*sendKeypressNotificationComplete)(const HciSendKeypressNotificationReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.64 Read Default Erroneous Data Reporting Command
    void (*readDefaultErroneousDataReportingComplete)(
        const HciReadDefaultErroneousDataReportingReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.65 Write Default Erroneous Data Reporting Command
    void (*writeDefaultErroneousDataReportingComplete)(
        const HciWriteDefaultErroneousDataReportingReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.67 Read Logical Link Accept Timeout Command
    void (*readLogicalLinkAcceptTimeoutComplete)(const HciReadLogicalLinkAcceptTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.68 Write Logical Link Accept Timeout Command
    void (*writeLogicalLinkAcceptTimeoutComplete)(const HciWriteLogicalLinkAcceptTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.69 Set Event Mask Page 2 Command
    void (*setEventMaskPage2Complete)(const HciSetEventMaskPage2ReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.70 Read Location Data Command
    void (*readLocationDataComplete)(const HciReadLocationDataReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.71 Write Location Data Command
    void (*writeLocationDataComplete)(const HciWriteLocationDataReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.72 Read Flow Control Mode Command
    void (*readFlowControlModeComplete)(const HciReadFlowControlModeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.73 Write Flow Control Mode Command
    void (*writeFlowControlModeComplete)(const HciWriteFlowControlModeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.74 Read Enhanced Transmit Power Level Command
    void (*readEnhancedTransmitPowerLevelComplete)(const HciReadEnhancedTransmitPowerLevelReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.75 Read Best Effort Flush Timeout Command
    void (*readBestEffortFlushTimeoutComplete)(const HciReadBestEffortFlushTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.76 Write Best Effort Flush Timeout Command
    void (*writeBestEffortFlushTimeoutComplete)(const HciWriteBestEffortFlushTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.78 Read LE Host Support Command
    void (*readLeHostSupportComplete)(const HciReadLeHostSupportReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.79 Write LE Host Support Command
    void (*writeLeHostSupportComplete)(const HciWriteLeHostSupportReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.80 Set MWS Channel Parameters Command
    void (*setMwsChannelParametersComplete)(const HciSetMwsChannelParametersReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.81 Set External Frame Configuration Command
    void (*setExternalFrameConfigurationComplete)(const HciSetExternalFrameConfigurationReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.82 Set MWS Signaling Command
    void (*setMwsSignalingComplete)(const HciSetMwsSignalingReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.83 Set MWS Transport Layer Command
    void (*setMwsTransportLayerComplete)(const HciSetMwsTransportLayerReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.84 Set MWS Scan Frequency Table Command
    void (*setMwsScanFrequencyTableComplete)(const HciSetMwsScanFrequencyTableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.85 Set MWS_PATTERN Configuration Command
    void (*setMwsPatternConfigurationComplete)(const HciSetMwsPatternConfigurationReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.86 Set Reserved LT_ADDR Command
    void (*setReservedLtAddrComplete)(const HciSetReservedLtAddrReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.87 Delete Reserved LT_ADDR Command
    void (*deleteReservedLtAddrComplete)(const HciDeleteReservedLtAddrReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.88 Set Connectionless Slave Broadcast Data Command
    void (*setConnectionlessSlaveBroadcastDataComplete)(
        const HciSetConnectionlessSlaveBroadcastDataReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.89 Read Synchronization Train Parameters Command
    void (*readSynchronizationTrainParametersComplete)(
        const HciReadSynchronizationTrainParametersReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.90 Write Synchronization Train Parameters Command
    void (*writeSynchronizationTrainParametersComplete)(
        const HciWriteSynchronizationTrainParametersReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.91 Read Secure Connections Host Support Command
    void (*readSecureConnectionsHostSupportComplete)(const HciReadSecureConnectionsHostSupportReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.92 Write Secure Connections Host Support Command
    void (*writeSecureConnectionsHostSupportComplete)(
        const HciWriteSecureConnectionsHostSupportReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.93 Read Authenticated Payload Timeout Command
    void (*readAuthenticatedPayloadTimeoutComplete)(const HciReadAuthenticatedPayloadTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.94 Write Authenticated Payload Timeout Command
    void (*writeAuthenticatedPayloadTimeoutComplete)(const HciWriteAuthenticatedPayloadTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.95 Read Local OOB Extended Data Command
    void (*readLocalOOBExtendedDataComplete)(const HciReadLocalOobExtendedDataReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.96 Read Extended Page Timeout Command
    void (*readExtendedPageTimeoutComplete)(const HciReadExtendedPageTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.97 Write Extended Page Timeout Command
    void (*writeExtendedPageTimeoutComplete)(const HciWriteExtendedPageTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.98 Read Extended Inquiry Length Command
    void (*readExtendedInquiryLengthComplete)(const HciReadExtendedInquiryLengthReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.3.99 Write Extended Inquiry Length Command
    void (*writeExtendedInquiryLengthComplete)(const HciWriteExtendedInquiryLengthReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.4.1 Read Local Version Information Command
    void (*readLocalVersionInformationComplete)(const HciReadLocalVersionInformationReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.4.2 Read Local Supported Commands Command
    void (*readLocalSupportedCommandsComplete)(const HciReadLocalSupportedCommandsReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.4.3 Read Local Supported Features Command
    void (*readLocalSupportedFeaturesComplete)(const HciReadLocalSupportedFeaturesReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.4.4 Read Local Extended Features Command
    void (*readLocalExtendedFeaturesComplete)(const HciReadLocalExtendedFeaturesReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.4.5 Read Buffer Size Command
    void (*readBufferSizeComplete)(const HciReadBufferSizeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.4.6 Read BD_ADDR Command
    void (*readBdAddrComplete)(const HciReadBdAddrReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.4.7 Read Data Block Size Command
    void (*readDataBlockSizeComplete)(const HciReadDataBlockSizeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.4.8 Read Local Supported Codecs Command
    void (*readLocalSupportedCodecsComplete)(const HciReadLocalSupportedCodecsReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.5.1 Read Failed Contact Counter Command
    void (*readFailedContactCounterComplete)(const HciReadFailedContactCounterReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.5.2 Reset Failed Contact Counter Command
    void (*resetFailedContactCounterComplete)(const HciResetFailedContactCounterReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.5.3 Read Link Quality Command
    void (*readLinkQualityComplete)(const HciReadLinkQualityReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.5.4 Read RSSI Command
    void (*readRssiComplete)(const HciReadRssiReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.5.5 Read AFH Channel Map Command
    void (*readAfhChannelMapComplete)(const HciReadAfhChannelMapReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.5.6 Read Clock Command
    void (*readClockComplete)(const HciReadClockReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.5.7 Read Encryption Key Size Command
    void (*readEncryptionKeySizeComplete)(const HciReadEncryptionKeySizeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.5.8 Read Local AMP Info Command
    void (*readLocalAmpInfoComplete)(const HciReadLocalAMPInfoReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.5.9 Read Local AMP ASSOC Command
    void (*readLocalAmpAssocComplete)(const HciReadLocalAmpAssocReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.5.10 Write Remote AMP ASSOC Command
    void (*writeRemoteAmpAssocComplete)(const HciWriteRemoteAmpAssocReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.5.11 Get MWS Transport Layer Configuration Command
    void (*getMwsTransportLayerConfigurationComplete)(
        const HciGetMwsTransportLayerConfigurationReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.5.12 Set Triggered Clock Capture Command
    void (*setTriggeredClockCaptureComplete)(const HciSetTriggeredClockCaptureReturnParam *returnParam);

    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.1 Inquiry Complete Event
    void (*inquiryComplete)(const HciInquiryCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.2 Inquiry Result Event
    void (*inquiryResult)(const HciInquiryResultEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.3 Connection Complete Event
    void (*connectionComplete)(const HciConnectionCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.4 Incoming Connection Event
    void (*connectionIndication)(const HciConnectionIndicationEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.5 Disconnection Complete Event
    void (*disconnectComplete)(const HciDisconnectCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.6 Authentication Complete Event
    void (*authenticationComplete)(const HciAuthenticationCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.7 Remote Name Request Complete Event
    void (*remoteNameRequestComplete)(const HciRemoteNameRequestCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.8 Encryption Change Event
    void (*encryptionChange)(const HciEncryptionChangeEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.9 Change Connection Link Key Complete Event
    void (*changeConnectionLinkKeyComplete)(const HciChangeConnectionLinkKeyCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.10 Master Link Key Complete Event
    void (*masterLinkKeyComplete)(const HciMasterLinkKeyCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.11 Read Remote Supported Features Complete Event
    void (*readRemoteSupportedFeaturesComplete)(const HciReadRemoteSupportedFeaturesCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.12 Read Remote Version Information Complete Event
    void (*readRemoteVersionInformationComplete)(const HciReadRemoteVersionInformationCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.13 QoS Setup Complete Event
    void (*qosSetupComplete)(const HciQosSetupCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.15 Command Status Event
    void (*commandStatus)(uint8_t status, uint16_t commandOpcode);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.16 Hardware Error Event
    void (*hardwareError)(const HciHardwareErrorEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.17 Flush Occurred Event
    void (*flushOccurred)(const HciFlushOccurredEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.18 Role Change Event
    void (*roleChange)(const HciRoleChangeEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.20 Mode Change Event
    void (*modeChange)(const HciModeChangeEventParam *param);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.21 Return Link Keys Event
    void (*returnLinkKeys)(const HciReturnLinkKeysEventParam *param);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.22 PIN Code Request Event
    void (*pinCodeRequest)(const HciPinCodeRequestEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.23 Link Key Request Event
    void (*linkKeyRequest)(const HciLinkKeyRequestEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.24 Link Key Notification Event
    void (*linkKeyNotification)(const HciLinkKeyNotificationEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.26 Data Buffer Overflow Event
    void (*dataBufferOverflow)(const HciDataBufferOverflowEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.27 Max Slots Change Event
    void (*maxSlotsChange)(const HciMaxSlotsChangeEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.28 Read Clock Offset Complete Event
    void (*readClockOffsetComplete)(const HciReadClockOffsetCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.29 Connection Packet Type Changed Event
    void (*connectionPacketTypeChanged)(const HciConnectionPacketTypeChangedEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.30 QoS Violation Event
    void (*qoSViolation)(const HciQosViolationEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.31 Page Scan Repetition Mode Change Event
    void (*pageScanRepetitionModeChange)(const HciPageScanRepetitionModeChangeEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.32 Flow Specification Complete Event
    void (*flowSpecificationComplete)(const HciFlowSpecificationCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.33 Inquiry Result with RSSI Event
    void (*inquiryResultWithRSSI)(const HciInquiryResultWithRssiEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.34 Read Remote Extended Features Complete Event
    void (*readRemoteExtendedFeaturesComplete)(const HciReadRemoteExtendedFeaturesCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.35 Synchronous Connection Complete Event
    void (*synchronousConnectionComplete)(const HciSynchronousConnectionCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.36 Synchronous Connection Changed Event
    void (*synchronousConnectionChanged)(const HciSynchronousConnectionChangedEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.37 Sniff Subrating Event
    void (*sniffSubrating)(const HciSniffSubratingEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.38 Extended Inquiry Result Event
    void (*extendedInquiryResult)(const HciExtendedInquiryResultEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.39 Encryption Key Refresh Complete Event
    void (*encryptionKeyRefreshComplete)(const HciEncryptionKeyRefreshCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.40 IO Capability Request Event
    void (*ioCapabilityRequest)(const HciIoCapabilityRequestEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.41 IO Capability Response Event
    void (*ioCapabilityResponse)(const HciIoCapabilityResponseEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.42 User Confirmation Request Event
    void (*userConfirmationRequest)(const HciUserConfirmationRequestEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.43 User Passkey Request Event
    void (*userPasskeyRequest)(const HciUserPasskeyRequestEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.44 Remote OOB Data Request Event
    void (*remoteOOBDataRequest)(const HciRemoteOobDataRequestEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.45 Simple Pairing Complete Event
    void (*simplePairingComplete)(const HciSimplePairingCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.46 Link Supervision Timeout Changed Event
    void (*linkSupervisionTimeoutChanged)(const HciLinkSupervisionTimeoutChangedEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.47 Enhanced Flush Complete Event
    void (*enhancedFlushComplete)(const HciEnhancedFlushCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.48 User Passkey Notification Event
    void (*userPasskeyNotification)(const HciUserPasskeyNotificationEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.49 Keypress Notification Event
    void (*keypressNotification)(const HciKeypressNotificationEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.50 Remote Host Supported Features Notification Event
    void (*remoteHostSupportedFeaturesNotification)(
        const HciRemoteHostSupportedFeaturesNotificationEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.51 Physical Link Complete Event
    void (*physicalLinkComplete)(const HciPhysicalLinkCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.52 Channel Selected Event
    void (*channelSelected)(const HciChannelSelectedEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.53 Disconnection Physical Link Complete Event
    void (*disconnectionPhysicalLinkComplete)(const HciDisconnectionPhysicalLinkCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.54 Physical Link Loss Early Warning Event
    void (*physicalLinkLossEarlyWarning)(const HciPhysicalLinkLossEarlyWarningEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.55 Physical Link Recovery Event
    void (*physicalLinkRecovery)(const HciPhysicalLinkRecoveryEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.56 Logical Link Complete Event
    void (*logicalLinkComplete)(const HciLogicalLinkCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.57 Disconnection Logical Link Complete Event
    void (*disconnectionLogicalLinkComplete)(const HciDisconnectionLogicalLinkCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.58 Flow Spec Modify Complete Event
    void (*flowSpecModifyComplete)(const HciFlowSpecModifyCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.59 Number Of Completed Data Blocks Event
    void (*numberOfCompletedDataBlocks)(const HciNumberOfCompletedDataBlocksEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.60 Short Range Mode Change Complete Event
    void (*shortRangeModeChangeComplete)(const HciShortRangeModeChangeCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.61 AMP Status Change Event
    void (*ampStatusChange)(const HciAmpStatusChangeEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.62 AMP Start Test Event
    void (*ampStartTest)(const HciAmpStartTestEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.63 AMP Test End Event
    void (*ampTestEnd)(const HciAmpTestEndEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.64 AMP Receiver Report Event
    void (*ampReceiverReport)(const HciAmpReceiverReportEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.66 Triggered Clock Capture Event
    void (*triggeredClockCapture)(const HciTriggeredClockCaptureEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.67 Synchronization Train Complete Event
    void (*synchronizationTrainComplete)(const HciSynchronizationTrainCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.68 Synchronization Train Received Event
    void (*synchronizationTrainReceived)(const HciSynchronizationTrainReceivedEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.69 Connectionless Slave Broadcast Receive Event
    void (*connectionlessSlaveBroadcastReceive)(const HciConnectionlessSlaveBroadcastReceiveEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.70 Connectionless Slave Broadcast Timeout Event
    void (*connectionlessSlaveBroadcastTimeout)(const HciConnectionlessSlaveBroadcastTimeoutEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.71 Truncated Page Complete Event
    void (*truncatedPageComplete)(const HciTruncatedPageCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.73 Connectionless Slave Broadcast Channel Map Change Event
    void (*connectionlessSlaveBroadcastChannelMapChange)(
        const HciConnectionlessSlaveBroadcastChannelMapChangeEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.74 Inquiry Response Notification Event
    void (*inquiryResponseNotification)(const HciInquiryResponseNotificationEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.75 Authenticated Payload Timeout Expired Event
    void (*authenticatedPayloadTimeoutExpired)(const HciAuthenticatedPayloadTimeoutExpiredEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.76 SAM Status Change Event
    void (*samStatusChange)(const HciSamStatusChangeEventParam *eventParam);

    // LE Cmds

    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.1 LE Set Event Mask Command
    void (*leSetEventMaskComplete)(const HciLeSetEventMaskReturnParam *returParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.2 LE Read Buffer Size Command
    void (*leReadBufferSizeComplete)(const HciLeReadBufferSizeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.3 LE Read Local Supported Features Command
    void (*leReadLocalSupportedFeaturesComplete)(const HciLeReadLocalSupportedFeaturesReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.4 LE Set Random Address Command
    void (*leSetRandomAddressComplete)(const HciLeSetRandomAddressReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.5 LE Set Advertising Parameters Command
    void (*leSetAdvertisingParametersComplete)(const HciLeSetAdvertisingParametersReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.6 LE Read Advertising Channel Tx Power Command
    void (*leReadAdvertisingChannelTxPowerComplete)(const HciLeReadAdvertisingChannelTxPowerReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.7 LE Set Advertising Data Command
    void (*leSetAdvertisingDataComplete)(const HciLeSetAdvertisingDataReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.8 LE Set Scan Response Data Command
    void (*leSetScanResponseDataComplete)(const HciLeSetScanResponseDataReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.9 LE Set Advertising Enable Command
    void (*leSetAdvertisingEnableComplete)(const HciLeSetAdvertisingEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.10 LE Set Scan Parameters Command
    void (*leSetScanParametersComplete)(const HciLeSetScanParametersReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.11 LE Set Scan Enable Command
    void (*leSetScanEnableComplete)(const HciLeSetScanEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.13 LE Create Connection Cancel Command
    void (*leCreateConnectionCancelComplete)(const HciLeCreateConnectionCancelReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.14 LE Read WL Size Command
    void (*leReadWhiteListSizeComplete)(const HciLeReadWhiteListSizeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.15 LE Clear WL Command
    void (*leClearWhiteListComplete)(const HciLeClearWhiteListReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.16 LE Add Device To WL Command
    void (*leAddDeviceToWhiteListComplete)(const HciLeAddDeviceToWhiteListReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.17 LE Remove Device From WL Command
    void (*leRemoveDeviceFromWhiteListComplete)(const HciLeRemoveDeviceFromWhiteListReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.19 LE Set Host Channel Classification Command
    void (*leSetHostChannelClassificationComplete)(const HciLeSetHostChannelClassificationReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.20 LE Read Channel Map Command
    void (*leReadChannelMapComplete)(const HciLeReadChannelMapReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.22 LE Encrypt Command
    void (*leEncryptComplete)(const HciLeEncryptReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.23 LE Rand Command
    void (*leRandComplete)(const HciLeRandReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.25 LE Long Term Key Request Reply Command
    void (*leLongTermKeyRequestReplyComplete)(const HciLeLongTermKeyRequestReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.26 LE Long Term Key Request Negative Reply Command
    void (*leLongTermKeyRequestNegativeReplyComplete)(
        const HciLeLongTermKeyRequestNegativeReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.27 LE Read Supported States Command
    void (*leReadSupportedStatesComplete)(const HciLeReadSupportedStatesReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.28 LE Receiver Test Command
    void (*leReceiverTestComplete)(const HciLeReceiverTestReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.29 LE Transmitter Test Command
    void (*leTransmitterTestComplete)(const HciLeTransmitterTestReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.30 LE Test End Command
    void (*leTestEndComplete)(const HciLeTestEndReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.31 LE Remote Connection Parameter Request Reply Command
    void (*leRemoteConnectionParameterRequestReplyComplete)(
        const HciLeRemoteConnectionParameterRequestReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.32 LE Remote Connection Parameter Request Negative Reply Command
    void (*leRemoteConnectionParameterRequestNegativeReplyComplete)(
        const HciLeRemoteConnectionParameterRequestNegativeReplyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.33 LE Set Data Length Command
    void (*leSetDataLengthComplete)(const HciLeSetDataLengthReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.34 LE Read Suggested Default Data Length Command
    void (*leReadSuggestedDefaultDataLengthComplete)(const HciLeReadSuggestedDefaultDataLengthReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.35 LE Write Suggested Default Data Length Command
    void (*leWriteSuggestedDefaultDataLengthComplete)(
        const HciLeWriteSuggestedDefaultDataLengthReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.38 LE Add Device To Resolving List Command
    void (*leAddDeviceToResolvingListComplete)(const HciLeAddDeviceToResolvingListReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.39 LE Remove Device From Resolving List Command
    void (*leRemoveDeviceFromResolvingListComplete)(const HciLeRemoveDeviceFromResolvingListReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.40 LE Clear Resolving List Command
    void (*leClearResolvingListComplete)(const HciLeClearResolvingListReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.41 LE Read Resolving List Size Command
    void (*leReadResolvingListSizeComplete)(const HciLeReadResolvingListSizeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.42 LE Read Peer Resolvable Address Command
    void (*leReadPeerResolvableAddressComplete)(const HciLeReadPeerResolvableAddressReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.43 LE Read Local Resolvable Address Command
    void (*leReadLocalResolvableAddressComplete)(const HciLeReadLocalResolvableAddressReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.44 LE Set Address Resolution Enable Command
    void (*leSetAddressResolutionEnableComplete)(const HciLeSetAddressResolutionEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.45 LE Set Resolvable Private Address Timeout Command
    void (*leSetResolvablePrivateAddressTimeoutComplete)(
        const HciLeSetResolvablePrivateAddressTimeoutReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.46 LE Read Maximum Data Length Command
    void (*leReadMaximumDataLengthComplete)(const HciLeReadMaximumDataLengthReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.47 LE Read PHY Command
    void (*leReadPhyComplete)(const HciLeReadPhyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.48 LE Set Default PHY Command
    void (*leSetDefaultPhyComplete)(const HciLeSetDefaultPhyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.49 LE Set PHY Command
    void (*leSetPhyComplete)(const HciLeSetPhyReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.50 LE Enhanced Receiver Test Command
    void (*leEnhancedReceiverTestComplete)(const HciLeEnhancedReceiverTestReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.51 LE Enhanced Transmitter Test Command
    void (*leEnhancedTransmitterTestComplete)(const HciLeEnhancedTransmitterTestReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.52 LE Set Advertising Set Random Address Command
    void (*leSetAdvertisingSetRandomAddressComplete)(const HciLeSetAdvertisingSetRandomAddressReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.53 LE Set Extended Advertising Parameters Command
    void (*leSetExtendedAdvertisingParametersComplete)(
        const HciLeSetExtendedAdvertisingParametersReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.54 LE Set Extended Advertising Data Command
    void (*leSetExtendedAdvertisingDataComplete)(const HciLeSetExtendedAdvertisingDataReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.55 LE Set Extended Scan Response Data Command
    void (*leSetExtendedScanResponseDataComplete)(const HciLeSetExtendedScanResponseDataReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.56 LE Set Extended Advertising Enable Command
    void (*leSetExtendedAdvertisingEnableComplete)(const HciLeSetExtendedAdvertisingEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.57 LE Read Maximum Advertising Data Length Command
    void (*leReadMaximumAdvertisingDataLengthComplete)(
        const HciLeReadMaximumAdvertisingDataLengthReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.58 LE Read Number of Supported Advertising Sets Command
    void (*leReadNumberofSupportedAdvertisingSetsComplete)(
        const HciLeReadNumberofSupportedAdvertisingSetsReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.59 LE Remove Advertising Set Command
    void (*leRemoveAdvertisingSetComplete)(const HciLeRemoveAdvertisingSetReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.60 LE Clear Advertising Sets Command
    void (*leClearAdvertisingSetsComplete)(const HciLeClearAdvertisingSetsReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.61 LE Set Periodic Advertising Parameters Command
    void (*leSetPeriodicAdvertisingParametersComplete)(
        const HciLeSetPeriodicAdvertisingParametersReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.62 LE Set Periodic Advertising Data Command
    void (*leSetPeriodicAdvertisingDataComplete)(const HciLeSetPeriodicAdvertisingDataReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.63 LE Set Periodic Advertising Enable Command
    void (*leSetPeriodicAdvertisingEnableComplete)(
        const HciLeSetPeriodicAdvertisingEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.64 LE Set Extended Scan Parameters Command
    void (*leSetExtendedScanParametersComplete)(const HciLeSetExtendedScanParametersReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.65 LE Set Extended Scan Enable Command
    void (*leSetExtendedScanEnableComplete)(const HciLeSetExtendedScanEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.68 LE Periodic Advertising Create Sync Cancel Command
    void (*lePeriodicAdvertisingCreateSyncCancelComplete)(
        const HciLePeriodicAdvertisingCreateSyncCancelReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.69 LE Periodic Advertising Terminate Sync Command
    void (*lePeriodicAdvertisingTerminateSyncComplete)(
        const HciLePeriodicAdvertisingTerminateSyncReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.70 LE Add Device To Periodic Advertiser List Command
    void (*leAddDeviceToPeriodicAdvertiserListComplete)(
        const HciLeAddDeviceToPeriodicAdvertiserListReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.71 LE Remove Device From Periodic Advertiser List Command
    void (*leRemoveDeviceFromPeriodicAdvertiserListComplete)(
        const HciLeRemoveDeviceFromPeriodicAdvertiserListReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.72 LE Clear Periodic Advertiser List Command
    void (*leClearPeriodicAdvertiserListComplete)(const HciLeClearPeriodicAdvertiserListReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.73 LE Read Periodic Advertiser List Size Command
    void (*leReadPeriodicAdvertiserListSizeComplete)(const HciLeReadPeriodicAdvertiserListSizeReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.74 LE Read Transmit Power Command
    void (*leReadTransmitPowerComplete)(const HciLeReadTransmitPowerReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.75 LE Read RF Path Compensation Command
    void (*leReadRfPathCompensationComplete)(const HciLeReadRfPathCompensationReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.76 LE Write RF Path Compensation Command
    void (*leWriteRfPathCompensationComplete)(const HciLeWriteRfPathCompensationReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.8.77 LE Set Privacy Mode Command
    void (*leSetPrivacyModeComplete)(const HciLeSetPrivacyModeReturnParam *returnParam);

    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.78 LE Receiver Test Command [v3]
    void (*leReceiverTestV3Complete)(const HciLeReceiverTestV3ReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.79 LE Transmitter Test Command [v3]
    void (*leTransmitterTestV3Complete)(const HciLeTransmitterTestV3ReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.80 LE Set Connectionless CTE Transmit Parameters Command
    void (*leSetConnectionlessCteTransmitParametersComplete)(
        const HciLeSetConnectionlessCteTransmitParametersReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.81 LE Set Connectionless CTE Transmit Enable Command
    void (*leSetConnectionlessCteTransmitEnableComplete)(
        const HciLeSetConnectionlessCteTransmitEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.82 LE Set Connectionless IQ Sampling Enable Command
    void (*leSetConnectionlessIqSamplingEnableComplete)(
        const HciLeSetConnectionlessIqSamplingEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.83 LE Set Connection CTE Receive Parameters Command
    void (*leSetConnectionCteReceiveParametersComplete)(
        const HciLeSetConnectionCteReceiveParametersReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.84 LE Set Connection CTE Transmit Parameters Command
    void (*leSetConnectionCteTransmitParametersComplete)(
        const HciLeSetConnectionCteTransmitParametersReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.85 LE Connection CTE Request Enable Command
    void (*leConnectionCteRequestEnableComplete)(const HciLeConnectionCteRequestEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.86 LE Connection CTE Response Enable Command
    void (*leConnectionCteResponseEnableComplete)(const HciLeConnectionCteResponseEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.87 LE Read Antenna Information Command
    void (*leReadAntennaInformationComplete)(const HciLeReadAntennaInformationReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.88 LE Set Periodic Advertising Receive Enable Command
    void (*leSetPeriodicAdvertisingReceiveEnableComplete)(
        const HciLeSetPeriodicAdvertisingReceiveEnableReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.89 LE Periodic Advertising Sync Transfer Command
    void (*lePeriodicAdvertisingSyncTransferComplete)(
        const HciLePeriodicAdvertisingSyncTransferReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.90 LE Periodic Advertising Set Info Transfer Command
    void (*lePeriodicAdvertisingSetInfoTransferComplete)(
        const HciLePeriodicAdvertisingSetInfoTransferReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.91 LE Set Periodic Advertising Sync Transfer Parameters Command
    void (*leSetPeriodicAdvertisingSyncTransferParametersComplete)(
        const HciLeSetPeriodicAdvertisingSyncTransferParametersReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.92 LE Set Default Periodic Advertising Sync Transfer Parameters Command
    void (*leSetDefaultPeriodicAdvertisingSyncTransferParametersComplete)(
        const HciLeSetDefaultPeriodicAdvertisingSyncTransferParametersReturnParam *returnParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.8.94 LE Modify Sleep Clock Accuracy Command Complete
    void (*leModifySleepClockAccuracyComplete)(const HciLeModifySleepClockAccuracyReturnParam *returnParam);

    // LE Events

    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,1 LE Connection Complete Event
    void (*leConnectionComplete)(const HciLeConnectionCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,2 LE Advertising Report Event
    void (*leAdvertisingReport)(const HciLeAdvertisingReportEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,3 LE Connection Update Complete Event
    void (*leConnectionUpdateComplete)(const HciLeConnectionUpdateCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,4 LE Read Remote Features Complete Event
    void (*leReadRemoteFeaturesComplete)(const HciLeReadRemoteFeaturesCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,5 LE Long Term Key Request Event
    void (*leLongTermKeyRequest)(const HciLeLongTermKeyRequestEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,6 LE Remote Connection Parameter Request Event
    void (*leRemoteConnectionParameterRequest)(const HciLeRemoteConnectionParameterRequestEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,7 LE Data Length Change Event
    void (*leDataLengthChange)(const HciLeDataLengthChangeEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,8 LE Read Local P-256 Public Key Complete Event
    void (*leReadLocalP256PublicKeyComplete)(const HciLeReadLocalP256PublicKeyCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,9 LE Generate DHKey Complete Event
    void (*leGenerateDHKeyComplete)(const HciLeGenerateDHKeyCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,10 LE Enhanced Connection Complete Event
    void (*leEnhancedConnectionComplete)(const HciLeEnhancedConnectionCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,11 LE Directed Advertising Report Event
    void (*leDirectedAdvertisingReport)(const HciLeDirectedAdvertisingReportEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,12 LE PHY Update Complete Event
    void (*lePhyUpdateComplete)(const HciLePhyUpdateCompleteEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,13 LE Extended Advertising Report Event
    void (*leExtendedAdvertisingReport)(const HciLeExtendedAdvertisingReportEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,14 LE Periodic Advertising Sync Established Event
    void (*lePeriodicAdvertisingSyncEstablished)(const HciLePeriodicAdvertisingSyncEstablishedEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,15 LE Periodic Advertising Report Event
    void (*lePeriodicAdvertisingReport)(const HciLePeriodicAdvertisingReportEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,16 LE Periodic Advertising Sync Lost Event
    void (*lePeriodicAdvertisingSyncLost)(const HciLePeriodicAdvertisingSyncLostEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,17 LE Scan Timeout Event
    void (*leScanTimeoutComplete)(void);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,18 LE Advertising Set Terminated Event
    void (*leAdvertisingSetTerminated)(const HciLeAdvertisingSetTerminatedEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,19 LE Scan Request Received Event
    void (*leScanRequestReceived)(const HciLeScanRequestReceivedEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
    // 7.7.65,20 LE Channel Selection Algorithm Event
    void (*leChannelSelectionAlgorithm)(const HciLeChannelSelectionAlgorithmEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.7.65,21 LE Connectionless IQ Report Event
    // The I/Q sample pointers are zero-copy views into the HCI event packet and are
    // valid only for the duration of the callback; consumers must copy them.
    void (*leConnectionlessIqReport)(const HciLeConnectionlessIqReportEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.7.65,22 LE Connection IQ Report Event
    void (*leConnectionIqReport)(const HciLeConnectionIqReportEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.7.65,23 LE CTE Request Failed Event
    void (*leCteRequestFailed)(const HciLeCteRequestFailedEventParam *eventParam);
    // BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
    // 7.7.65,24 LE Periodic Advertising Sync Transfer Received Event
    void (*lePeriodicAdvertisingSyncTransferReceived)(
        const HciLePeriodicAdvertisingSyncTransferReceivedEventParam *eventParam);
} HciEventCallbacks;

int HCI_RegisterEventCallbacks(const HciEventCallbacks *callbacks);
int HCI_DeregisterEventCallbacks(const HciEventCallbacks *callbacks);

typedef struct {
    void (*onAclData)(uint16_t handle, uint8_t pbFlag, uint8_t bcFlag, Packet *packet);
} HciAclCallbacks;

int HCI_RegisterAclCallbacks(const HciAclCallbacks *callbacks);
int HCI_DeregisterAclCallbacks(const HciAclCallbacks *callbacks);

typedef struct {
    void (*onCmdTimeout)();
} HciFailureCallbacks;

int HCI_RegisterFailureCallback(const HciFailureCallbacks *callbacks);
int HCI_DeregisterFailureCallback(const HciFailureCallbacks *callbacks);
#ifdef __cplusplus
}
#endif

#endif
