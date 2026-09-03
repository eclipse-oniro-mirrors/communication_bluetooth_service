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

#ifndef HCI_DEF_SUPPORTED_CMDS_H
#define HCI_DEF_SUPPORTED_CMDS_H

#ifdef __cplusplus
extern "C" {
#endif

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E
// 6.27 SUPPORTED COMMANDS (bit positions verified against the spec table;
// the LE section starts at Octet 25, not Octet 33)
static inline int HciGetCommandFlag(const uint8_t *cmds, uint8_t byteIndex, uint8_t bitIndex)
{
    return cmds[byteIndex] & (0x01 << bitIndex);
}

// Byte indexes of the Supported Commands bit field (Vol 2, Part E 6.27).
enum {
    SUPPORTED_CMDS_OCTET_29 = 29,
    SUPPORTED_CMDS_OCTET_34 = 34,
    SUPPORTED_CMDS_OCTET_35 = 35,
    SUPPORTED_CMDS_OCTET_38 = 38,
    SUPPORTED_CMDS_OCTET_39 = 39,
    SUPPORTED_CMDS_OCTET_41 = 41,
    SUPPORTED_CMDS_OCTET_42 = 42,
    SUPPORTED_CMDS_OCTET_43 = 43,
    SUPPORTED_CMDS_OCTET_44 = 44,
    SUPPORTED_CMDS_OCTET_45 = 45,
    SUPPORTED_CMDS_OCTET_46 = 46,
};

// Bit indexes within a Supported Commands octet (Vol 2, Part E 6.27).
enum {
    SUPPORTED_CMDS_BIT_0 = 0,
    SUPPORTED_CMDS_BIT_1 = 1,
    SUPPORTED_CMDS_BIT_2 = 2,
    SUPPORTED_CMDS_BIT_3 = 3,
    SUPPORTED_CMDS_BIT_4 = 4,
    SUPPORTED_CMDS_BIT_5 = 5,
    SUPPORTED_CMDS_BIT_6 = 6,
    SUPPORTED_CMDS_BIT_7 = 7,
};

static inline int HciSupportEnhancedSetupSynchronousConnection(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_29, SUPPORTED_CMDS_BIT_3);
}

static inline int HciSupportEnhancedAcceptSynchronousConnection(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_29, SUPPORTED_CMDS_BIT_4);
}

static inline int HciSupportReadLocalSupportedCodecs(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_29, SUPPORTED_CMDS_BIT_5);
}

static inline int HciSupportLeReadLocalP256PublicKey(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_34, SUPPORTED_CMDS_BIT_1);
}

static inline int HciSupportLeGenerateDhKey(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_34, SUPPORTED_CMDS_BIT_2);
}

static inline int HciSupportLeSetPrivacyMode(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_39, SUPPORTED_CMDS_BIT_2);
}

static inline int HciSupportLeReadTransmitPower(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_38, SUPPORTED_CMDS_BIT_7);
}

static inline int HciSupportLeReadRfPathCompensation(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_39, SUPPORTED_CMDS_BIT_0);
}

static inline int HciSupportLeWriteRfPathCompensation(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_39, SUPPORTED_CMDS_BIT_1);
}

static inline int HciSupportLeReadPhy(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_35, SUPPORTED_CMDS_BIT_4);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 4, Part E
// 6.27 SUPPORTED COMMANDS (LE 5.2 commands, octet 41-44)
static inline int HciSupportLeReadBufferSizeV2(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_41, SUPPORTED_CMDS_BIT_5);
}

static inline int HciSupportLeReadIsoTxSync(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_41, SUPPORTED_CMDS_BIT_6);
}

static inline int HciSupportLeSetCigParameters(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_41, SUPPORTED_CMDS_BIT_7);
}

static inline int HciSupportLeSetCigParametersTest(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_42, SUPPORTED_CMDS_BIT_0);
}

static inline int HciSupportLeCreateCis(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_42, SUPPORTED_CMDS_BIT_1);
}

static inline int HciSupportLeRemoveCig(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_42, SUPPORTED_CMDS_BIT_2);
}

static inline int HciSupportLeAcceptCisRequest(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_42, SUPPORTED_CMDS_BIT_3);
}

static inline int HciSupportLeRejectCisRequest(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_42, SUPPORTED_CMDS_BIT_4);
}

static inline int HciSupportLeCreateBig(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_42, SUPPORTED_CMDS_BIT_5);
}

static inline int HciSupportLeCreateBigTest(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_42, SUPPORTED_CMDS_BIT_6);
}

static inline int HciSupportLeTerminateBig(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_42, SUPPORTED_CMDS_BIT_7);
}

static inline int HciSupportLeBigCreateSync(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_43, SUPPORTED_CMDS_BIT_0);
}

static inline int HciSupportLeBigTerminateSync(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_43, SUPPORTED_CMDS_BIT_1);
}

static inline int HciSupportLeRequestPeerSca(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_43, SUPPORTED_CMDS_BIT_2);
}

static inline int HciSupportLeSetupIsoDataPath(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_43, SUPPORTED_CMDS_BIT_3);
}

static inline int HciSupportLeRemoveIsoDataPath(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_43, SUPPORTED_CMDS_BIT_4);
}

static inline int HciSupportLeIsoTransmitTest(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_43, SUPPORTED_CMDS_BIT_5);
}

static inline int HciSupportLeIsoReceiveTest(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_43, SUPPORTED_CMDS_BIT_6);
}

static inline int HciSupportLeIsoReadTestCounters(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_43, SUPPORTED_CMDS_BIT_7);
}

static inline int HciSupportLeIsoTestEnd(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_44, SUPPORTED_CMDS_BIT_0);
}

static inline int HciSupportLeSetHostFeature(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_44, SUPPORTED_CMDS_BIT_1);
}

static inline int HciSupportLeReadIsoLinkQuality(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_44, SUPPORTED_CMDS_BIT_2);
}

static inline int HciSupportLeEnhancedReadTransmitPowerLevel(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_44, SUPPORTED_CMDS_BIT_3);
}

static inline int HciSupportLeReadRemoteTransmitPowerLevel(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_44, SUPPORTED_CMDS_BIT_4);
}

static inline int HciSupportLeSetPathLossReportingParameters(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_44, SUPPORTED_CMDS_BIT_5);
}

static inline int HciSupportLeSetPathLossReportingEnable(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_44, SUPPORTED_CMDS_BIT_6);
}

static inline int HciSupportLeSetTransmitPowerReportingEnable(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_44, SUPPORTED_CMDS_BIT_7);
}

// BLUETOOTH SPECIFICATION Version 5.3 | Vol 4, Part E, 6.27 (amended 2024)
// Bit positions verified against the spec table. Note: octets 45/46 are NOT in
// OCF order - octet 45 hosts mixed 5.3 additions (bit 6 = HCI_LE_Set_Data_
// Related_Address_Changes 0x7C, bit 7 = HCI_Set_Min_Encryption_Key_Size
// 0x0084) and the two subrate commands sit in octet 46 (bit 0 = 0x7D,
// bit 1 = 0x7E).
static inline int HciSupportLeSetDataRelatedAddressChanges(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_45, SUPPORTED_CMDS_BIT_6);
}

static inline int HciSupportLeSetDefaultSubrate(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_46, SUPPORTED_CMDS_BIT_0);
}

static inline int HciSupportLeSubrateRequest(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_46, SUPPORTED_CMDS_BIT_1);
}

static inline int HciSupportSetMinEncryptionKeySize(const uint8_t *cmds)
{
    return !!HciGetCommandFlag(cmds, SUPPORTED_CMDS_OCTET_45, SUPPORTED_CMDS_BIT_7);
}

#ifdef __cplusplus
}
#endif

#endif
