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

#ifdef __cplusplus
}
#endif

#endif
