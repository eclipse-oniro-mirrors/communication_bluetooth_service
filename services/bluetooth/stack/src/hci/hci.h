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

#ifndef HCI_H
#define HCI_H

#include <stdbool.h>
#include <stdint.h>

#include "hci_def.h"

#ifdef __cplusplus
extern "C" {
#endif

int HCI_Initialize();
void HCI_Close();

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.1 Inquiry Command
int HCI_Inquiry(const HciInquiryeParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.2 Inquiry Cancel Command
int HCI_InquiryCancel(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.5 Create Connection Command
int HCI_CreateConnection(const HciCreateConnectionParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.6 Disconnect Command
int HCI_Disconnect(const HciDisconnectParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.7 Create Connection Cancel Command
int HCI_CreateConnectionCancel(const HciCreateConnectionCancelParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.8 Accept Incoming Connection Command
int HCI_AcceptConnectionRequest(const HciAcceptConnectionReqestParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.9 Reject Incoming Connection Command
int HCI_RejectConnectionRequest(const HciRejectConnectionRequestParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.10 Link Key Request Reply Command
int HCI_LinkKeyRequestReply(const HciLinkKeyRequestReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.11 Link Key Request Negative Reply Command
int HCI_LinkKeyRequestNegativeReply(const HciLinkKeyRequestNegativeReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.12 PIN Code Request Reply Command
int HCI_PINCodeRequestReply(const HciPinCodeRequestReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.13 PIN Code Request Negative Reply Command
int HCI_PINCodeRequestNegativeReply(const HciPinCodeRequestNegativeReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.14 Change Connection Packet Type Command
int HCI_ChangeConnectionPacketType(const HciChangeConnectionPacketTypeParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.15 Authentication Requested Command
int HCI_AuthenticationRequested(const HciAuthenticationRequestedParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.16 Set Connection Encryption Command
int HCI_SetConnectionEncryption(const HciSetConnectionEncryptionParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.19 Remote Name Request Command
int HCI_RemoteNameRequest(const HciRemoteNameRequestParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.20 Remote Name Request Cancel Command
int HCI_RemoteNameRequestCancel(const HciRemoteNameRequestCancelParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.21 Read Remote Supported Features
int HCI_ReadRemoteSupportedFeatures(const HciReadRemoteSupportedFeaturesParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.22 Read Remote Extended Features Command
int HCI_ReadRemoteExtendedFeatures(const HciReadRemoteExtendedFeaturesParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.23 Read Remote Version Information Command
int HCI_ReadRemoteVersionInformation(const HciReadRemoteVersionInformationParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.26 Setup Synchronous Connection Command
int HCI_SetupSynchronousConnection(const HciSetupSynchronousConnectionParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.27 Accept Synchronous Incoming Connection Command
int HCI_AcceptSynchronousConnectionRequest(const HciAcceptSynchronousConnectionRequestParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.28 Reject Synchronous Incoming Connection Command
int HCI_RejectSynchronousConnectionRequest(const HciRejectSynchronousConnectionRequestParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.29 IO Capability Request Reply Command
int HCI_IOCapabilityRequestReply(const HciIOCapabilityRequestReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.30 User Confirmation Request Reply Command
int HCI_UserConfirmationRequestReply(const HciUserConfirmationRequestReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.31 User Confirmation Request Negative Reply Command
int HCI_UserConfirmationRequestNegativeReply(const HciUserConfirmationRequestNegativeReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.32 User Passkey Request Reply Command
int HCI_UserPasskeyRequestReply(const HciUserPasskeyRequestReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.33 User Passkey Request Negative Reply Command
int HCI_UserPasskeyRequestNegativeReply(const HciUserPasskeyRequestNegativeReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.34 Remote OOB Data Request Reply Command
int HCI_RemoteOOBDataRequestReply(const HciRemoteOobDataRequestReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.35 Remote OOB Data Request Negative Reply Command
int HCI_RemoteOOBDataRequestNegativeReply(const HciRemoteOobDataRequestNegativeReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.36 IO Capability Request Negative Reply Command
int HCI_IOCapabilityRequestNegativeReply(const HciIoCapabilityRequestNegativeReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.45 Enhanced Setup Synchronous Connection Command
int HCI_EnhancedSetupSynchronousConnection(const HciEnhancedSetupSynchronousConnectionParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.46 Enhanced Accept Synchronous Incoming Connection Command
int HCI_EnhancedAcceptSynchronousConnectionRequest(const HciEnhancedAcceptSynchronousConnectionRequestParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.1.53 Remote OOB Extended Data Request Reply Command
int HCI_RemoteOOBExtendedDataRequestReply(const HciRemoteOobExtendedDataRequestReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.2.2 Sniff Mode Command
int HCI_SniffMode(const HciSniffModeParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.2.3 Exit Sniff Mode Command
int HCI_ExitSniffMode(const HciExitSniffModeParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.2.8 Switch Role Command
int HCI_SwitchRole(const HciSwitchRoleParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.2.10 Write Link Policy Settings Command
int HCI_WriteLinkPolicySettings(const HciWriteLinkPolicySettingsParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.2.12 Write Default Link Policy Settings Command
int HCI_WriteDefaultLinkPolicySettings(const HciWriteDefaultLinkPolicySettingsParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.2.14 Sniff Subrating Command
int HCI_SniffSubrating(const HciSniffSubratingParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.1 Set Event Mask Command
int HCI_SetEventMask(const HciSetEventMaskParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.2 Reset Command
int HCI_Reset(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.4 Flush Command
int HCI_Flush(const HciFlushParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.11 Write Local Name Command
int HCI_WriteLocalName(const HciWriteLocalNameParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.18 Write Scan Enable Command
int HCI_WriteScanEnable(const HciWriteScanEnableParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.20 Write Page Scan Command
int HCI_WritePageScanActivity(const HciWritePageScanActivityParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.22 Write Inquiry Scan Command
int HCI_WriteInquiryScanActivity(const HciWriteInquiryScanActivityParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.26 Write Class of Device Command
int HCI_WriteClassofDevice(const HciWriteClassofDeviceParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.28 Write Voice Setting Command
int HCI_WriteVoiceSetting(const HciWriteVoiceSettingParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.39 Host Buffer Size Command
int HCI_HostBufferSize(const HciHostBufferSizeCmdParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.45 Write Current IAC LAP Command
int HCI_WriteCurrentIacLap(const HciWriteCurrentIacLapParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.48 Write Inquiry Scan Type Command
int HCI_WriteInquiryScanType(const HciWriteInquiryScanTypeParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.50 Write Inquiry Mode Command
int HCI_WriteInquiryMode(const HciWriteInquiryModeParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.52 Write Page Scan Type Command
int HCI_WritePageScanType(const HciWritePageScanTypeParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.56 Write Extended Inquiry Response Command
int HCI_WriteExtendedInquiryResponse(const HciWriteExtendedInquiryResponseParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.59 Write Simple Pairing Mode Command
int HCI_WriteSimplePairingMode(const HciWriteSimplePairingModeParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.60 Read Local OOB Data Command
int HCI_ReadLocalOOBData(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.79 Write LE Host Support Command
int HCI_WriteLeHostSupport(const HciWriteLeHostSupportParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.92 Write Secure Connections Host Support Command
int HCI_WriteSecureConnectionsHostSupport(const HciWriteSecureConnectionsHostSupportParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.94 Write Authenticated Payload Timeout Command
int HCI_WriteAuthenticatedPayloadTimeout(const HciWriteAuthenticatedPayloadTimeoutParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.3.95 Read Local OOB Extended Data Command
int HCI_ReadLocalOOBExtendedData(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.4.1 Read Local Version Information Command
int HCI_ReadLocalVersionInformation(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.4.2 Read Local Supported Commands Command
int HCI_ReadLocalSupportedCommands(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.4.3 Read Local Supported Features Command
int HCI_ReadLocalSupportedFeatures(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.4.4 Read Local Extended Features Command
int HCI_ReadLocalExtendedFeatures(const HciReadLocalExtendedFeaturesParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.4.5 Read Buffer Size Command
int HCI_ReadBufferSize(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.4.6 Read BD_ADDR Command
int HCI_ReadBdAddr(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.4.8 Read Local Supported Codecs Command
int HCI_ReadLocalSupportedCodecs(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.5.4 Read RSSI Command
int HCI_ReadRssi(const HciReadRssiParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.1 LE Set Event Mask Command
int HCI_LeSetEventMask(const HciLeSetEventMaskParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.2 LE Read Buffer Size Command
int HCI_LeReadBufferSize(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.3 LE Read Local Supported Features Command
int HCI_LeReadLocalSupportedFeatures(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.4 LE Set Random Address Command
int HCI_LeSetRandomAddress(const HciLeSetRandomAddressParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.5 LE Set Advertising Parameters Command

int HCI_LeSetAdvertisingParameters(const HciLeSetAdvertisingParametersParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.6 LE Read Advertising Channel Tx Power Command
int HCI_LeReadAdvertisingChannelTxPower(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.7 LE Set Advertising Data Command
int HCI_LeSetAdvertisingData(const HciLeSetAdvertisingDataParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.8 LE Set Scan Response Data Command
int HCI_LeSetScanResponseData(const HciLeSetScanResponseDataParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.9 LE Set Advertising Enable Command
int HCI_LeSetAdvertisingEnable(const HciLeSetAdvertisingEnableParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.10 LE Set Scan Parameters Command
int HCI_LeSetScanParameters(const HciLeSetScanParametersParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.11 LE Set Scan Enable Command
int HCI_LeSetScanEnable(const HciLeSetScanEnableParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.12 LE Create Connection Command
int HCI_LeCreateConnection(const HciLeCreateConnectionParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.13 LE Create Connection Cancel Command
int HCI_LeCreateConnectionCancel(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.14 LE Read WL Size Command
int HCI_LeReadWhiteListSize(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.15 LE Clear WL Command
int HCI_LeClearWhiteList(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.16 LE Add Device To WL Command
int HCI_LeAddDeviceToWhiteList(const HciLeAddDeviceToWhiteListParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.17 LE Remove Device From WL Command
int HCI_LeRemoveDeviceFromWhiteList(const HciLeRemoveDeviceFromWhiteListParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.18 LE Connection Update Command
int HCI_LeConnectionUpdate(const HciLeConnectionUpdateParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.19 LE Set Host Channel Classification Command
int HCI_LeSetHostChannelClassification(const HciLeSetHostChannelClassificationParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.20 LE Read Channel Map Command
int HCI_LeReadChannelMap(const HciLeReadChannelMapParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.21 LE Read Remote Features Command
int HCI_LeReadRemoteFeatures(const HciLeReadRemoteFeaturesParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.22 LE Encrypt Command
int HCI_LeEncrypt(const HciLeEncryptParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.23 LE Rand Command
int HCI_LeRand(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.24 LE Start Encryption Command
int HCI_LeStartEncryption(const HciLeStartEncryptionParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.25 LE Long Term Key Request Reply Command
int HCI_LeLongTermKeyRequestReply(const HciLeLongTermKeyRequestReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.26 LE Long Term Key Request Negative Reply Command
int HCI_LeLongTermKeyRequestNegativeReply(const HciLeLongTermKeyRequestNegativeReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.30 LE Test End Command
int HCI_LeTestEnd(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.31 LE Remote Connection Parameter Request Reply Command
int HCI_LeRemoteConnectionParameterRequestReply(const HciLeRemoteConnectionParameterRequestReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.32 LE Remote Connection Parameter Request Negative Reply Command
int HCI_LeRemoteConnectionParameterRequestNegativeReply(
    const HciLeRemoteConnectionParameterRequestNegativeReplyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.36 LE Read Local P-256 Public Key Command
int HCI_LeReadLocalP256PublicKey(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.37 LE Generate DHKey Command
int HCI_LeGenerateDHKey(const HciLeGenerateDHKeyParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.38 LE Add Device To Resolving List Command
int HCI_LeAddDeviceToResolvingList(const HciLeAddDeviceToResolvingListParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.39 LE Remove Device From Resolving List Command
int HCI_LeRemoveDeviceFromResolvingList(const HciLeRemoveDeviceFromResolvingListParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.40 LE Clear Resolving List Command
int HCI_LeClearResolvingList(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.41 LE Read Resolving List Size Command
int HCI_LeReadResolvingListSize(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.44 LE Set Address Resolution Enable Command
int HCI_LeSetAddressResolutionEnable(const HciLeSetAddressResolutionEnableParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.52 LE Set Advertising Set Random Address Command
int HCI_LeSetAdvertisingSetRandomAddress(const HciLeSetAdvertisingSetRandomAddressParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.53 LE Set Extended Advertising Parameters Command
int HCI_LeSetExtendedAdvertisingParameters(const HciLeSetExtendedAdvertisingParametersParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.54 LE Set Extended Advertising Data Command
int HCI_LeSetExtendedAdvertisingData(const HciLeSetExtendedAdvertisingDataParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.55 LE Set Extended Scan Response Data Command
int HCI_LeSetExtendedScanResponseData(const HciLeSetExtendedScanResponseDataParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.56 LE Set Extended Advertising Enable Command
int HCI_LeSetExtendedAdvertisingEnable(const HciLeSetExtendedAdvertisingEnableParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.57 LE Read Maximum Advertising Data Length Command
int HCI_LeReadMaximumAdvertisingDataLength(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.58 LE Read Number of Supported Advertising Sets Command
int HCI_LeReadNumberofSupportedAdvertisingSets(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.59 LE Remove Advertising Set Command
int HCI_LeRemoveAdvertisingSet(const HciLeRemoveAdvertisingSetParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.60 LE Clear Advertising Sets Command
int HCI_LeClearAdvertisingSets(void);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.64 LE Set Extended Scan Parameters Command
int HCI_LeSetExtendedScanParameters(const HciLeSetExtendedScanParametersParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.65 LE Set Extended Scan Enable Command
int HCI_LeSetExtendedScanEnable(const HciLeSetExtendedScanEnableParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.66 LE Extended Create Connection Command
int HCI_LeExtendedCreateConnection(const HciLeExtendedCreateConnectionParam *param);

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 7.8.77 LE Set Privacy Mode Command
int HCI_LeSetPrivacyMode(const HciLeSetPrivacyModeParam *param);


#define NON_FLUSHABLE_PACKET 0
#define FLUSHABLE_PACKET 1
int HCI_SendAclData(uint16_t handle, uint8_t flushable, Packet *packet);

#define TRANSMISSON_TYPE_H2C_CMD 1
#define TRANSMISSON_TYPE_C2H_EVENT 2
#define TRANSMISSON_TYPE_H2C_DATA 3
#define TRANSMISSON_TYPE_C2H_DATA 4

int HCI_SetTransmissionCaptureCallback(void (*onTransmission)(uint8_t type, const uint8_t *data, uint16_t length));

int HCI_EnableTransmissionCapture();
int HCI_DisableTransmissionCapture();

// Test seam: feed a raw HCI event packet (wire bytes, exactly as delivered by
// the transport: Event_Code(1) + Parameter_Total_Length(1) + parameters, no H4
// packet indicator) into the HCI RX path as if the controller had sent it. The
// event is enqueued on the RX queue and parsed on the stack processing thread,
// so event callbacks fire asynchronously on that thread - same semantics as a
// real controller event. This is how unit/interact tests construct wire bytes
// for events no peer device can generate on demand (e.g. PAST / IQ report).
// Returns BT_SUCCESS when accepted, BT_BAD_PARAM on invalid input, and
// BT_OPERATION_FAILED when the stack is not initialized (no RX queue).
int HCI_InjectReceivedEvent(const uint8_t *data, uint16_t length);

void HCI_SetBufferSize(uint16_t packetLength, uint16_t totalPackets);
void HCI_SetLeBufferSize(uint16_t packetLength, uint8_t totalPackets);

#ifdef __cplusplus
}
#endif

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.96-7.8.121 LE Controller command senders (ISO / LE Power Control).
#ifdef __cplusplus
extern "C" {
#endif
// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.96 LE Read ISO TX Sync Command
int HCI_LeReadIsoTxSync(const HciLeReadIsoTxSyncParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.97 LE Set CIG Parameters Command
int HCI_LeSetCigParameters(const HciLeSetCigParametersParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.98 LE Set CIG Parameters Test Command
int HCI_LeSetCigParametersTest(const HciLeSetCigParametersTestParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.99 LE Create CIS Command
int HCI_LeCreateCis(const HciLeCreateCisParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.100 LE Remove CIG Command
int HCI_LeRemoveCig(const HciLeRemoveCigParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.101 LE Accept CIS Request Command
int HCI_LeAcceptCisRequest(const HciLeAcceptCisRequestParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.102 LE Reject CIS Request Command
int HCI_LeRejectCisRequest(const HciLeRejectCisRequestParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.103 LE Create BIG Command
int HCI_LeCreateBig(const HciLeCreateBigParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.104 LE Create BIG Test Command
int HCI_LeCreateBigTest(const HciLeCreateBigTestParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.105 LE Terminate BIG Command
int HCI_LeTerminateBig(const HciLeTerminateBigParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.106 LE BIG Create Sync Command
int HCI_LeBigCreateSync(const HciLeBigCreateSyncParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.107 LE BIG Terminate Sync Command
int HCI_LeBigTerminateSync(const HciLeBigTerminateSyncParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.108 LE Request Peer SCA Command
int HCI_LeRequestPeerSca(const HciLeRequestPeerScaParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.109 LE Setup ISO Data Path Command
int HCI_LeSetupIsoDataPath(const HciLeSetupIsoDataPathParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.110 LE Remove ISO Data Path Command
int HCI_LeRemoveIsoDataPath(const HciLeRemoveIsoDataPathParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.111 LE ISO Transmit Test Command
int HCI_LeIsoTransmitTest(const HciLeIsoTransmitTestParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.112 LE ISO Receive Test Command
int HCI_LeIsoReceiveTest(const HciLeIsoReceiveTestParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.113 LE ISO Read Test Counters Command
int HCI_LeIsoReadTestCounters(const HciLeIsoReadTestCountersParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.114 LE ISO Test End Command
int HCI_LeIsoTestEnd(const HciLeIsoTestEndParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.115 LE Set Host Feature Command
int HCI_LeSetHostFeature(const HciLeSetHostFeatureParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.116 LE Read ISO Link Quality Command
int HCI_LeReadIsoLinkQuality(const HciLeReadIsoLinkQualityParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.117 LE Enhanced Read Transmit Power Level Command
int HCI_LeEnhancedReadTransmitPowerLevel(const HciLeEnhancedReadTransmitPowerLevelParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.118 LE Read Remote Transmit Power Level Command
int HCI_LeReadRemoteTransmitPowerLevel(const HciLeReadRemoteTransmitPowerLevelParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.119 LE Set Path Loss Reporting Parameters Command
int HCI_LeSetPathLossReportingParameters(const HciLeSetPathLossReportingParametersParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.120 LE Set Path Loss Reporting Enable Command
int HCI_LeSetPathLossReportingEnable(const HciLeSetPathLossReportingEnableParam *param);

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.121 LE Set Transmit Power Reporting Enable Command
int HCI_LeSetTransmitPowerReportingEnable(const HciLeSetTransmitPowerReportingEnableParam *param);
#ifdef __cplusplus
}
#endif

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.78-7.8.92 LE Controller command senders (Direction Finding / PAST / test v3).
// The header has its own extern "C" guard.
#include "hci_le_controller_5_1.h"


// Event/ACL/failure callback registration types.
// The header has its own extern "C" guard.
#include "hci_evt_callbacks.h"
#endif