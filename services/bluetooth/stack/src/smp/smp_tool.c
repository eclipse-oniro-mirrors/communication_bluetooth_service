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

#include "smp_tool.h"

#include <string.h>

#include "log.h"
#include "platform/include/allocator.h"
#include "smp.h"
#include "smp_aes_encryption.h"
#include "smp_common.h"
#include "smp_def.h"
#include "smp_send.h"

static const uint8_t CONST_RB[CRYPT_AESCMAC_CONST_RB_LEN] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87
};

const uint8_t CRYPT_H6_KEYID_TMP1[CRYPT_H6_KEYID_LEN] = {0x74, 0x6D, 0x70, 0x31}; /* "tmp1" */
const uint8_t CRYPT_H6_KEYID_TMP2[CRYPT_H6_KEYID_LEN] = {0x74, 0x6D, 0x70, 0x32}; /* "tmp2" */
const uint8_t CRYPT_H6_KEYID_LEBR[CRYPT_H6_KEYID_LEN] = {0x6C, 0x65, 0x62, 0x72}; /* "lebr" */
const uint8_t CRYPT_H6_KEYID_BRLE[CRYPT_H6_KEYID_LEN] = {0x62, 0x72, 0x6C, 0x65}; /* "brle" */

static const uint8_t MASTER_LEGACY_IO_TABLE[IO_COUNTS][IO_COUNTS] = {
    {SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_ENTRY},
    {SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_ENTRY},
    {SMP_PAIR_METHOD_PASSKEY_DISPLAY,
        SMP_PAIR_METHOD_PASSKEY_DISPLAY,
        SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_DISPLAY},
    {SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK},
    {SMP_PAIR_METHOD_PASSKEY_DISPLAY,
        SMP_PAIR_METHOD_PASSKEY_DISPLAY,
        SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_DISPLAY},
};

static const uint8_t SLAVE_LEGACY_IO_TABLE[IO_COUNTS][IO_COUNTS] = {
    {SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_DISPLAY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_DISPLAY},
    {SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_DISPLAY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_DISPLAY},
    {SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_ENTRY},
    {SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK},
    {SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_PASSKEY_DISPLAY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_ENTRY},
};

static const uint8_t MASTER_SCIO_TABLE[IO_COUNTS][IO_COUNTS] = {
    {SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_ENTRY},
    {SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_NUMERIC_COMPARISON,
        SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_NUMERIC_COMPARISON},
    {SMP_PAIR_METHOD_PASSKEY_DISPLAY,
        SMP_PAIR_METHOD_PASSKEY_DISPLAY,
        SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_DISPLAY},
    {SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK},
    {SMP_PAIR_METHOD_PASSKEY_DISPLAY,
        SMP_PAIR_METHOD_NUMERIC_COMPARISON,
        SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_NUMERIC_COMPARISON},
};

static const uint8_t SLAVE_SCIO_TABLE[IO_COUNTS][IO_COUNTS] = {
    {SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_DISPLAY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_DISPLAY},
    {SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_NUMERIC_COMPARISON,
        SMP_PAIR_METHOD_PASSKEY_DISPLAY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_NUMERIC_COMPARISON},
    {SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_PASSKEY_ENTRY},
    {SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_JUST_WORK},
    {SMP_PAIR_METHOD_PASSKEY_ENTRY,
        SMP_PAIR_METHOD_NUMERIC_COMPARISON,
        SMP_PAIR_METHOD_PASSKEY_DISPLAY,
        SMP_PAIR_METHOD_JUST_WORK,
        SMP_PAIR_METHOD_NUMERIC_COMPARISON},
};

#define CRYPT_XOR128_OUT_LEN 16
typedef struct {
    const uint8_t *a;
    const uint8_t *b;
    uint8_t *out;
} SMP_CryptXor128Param;

#define CRYPT_LEFTSHIFTONEBIT_OUT_LEN 16
typedef struct {
    uint8_t *input;
    uint8_t *output;
} SMP_CryptLeftShiftOneBitParam;

#define CRYPT_PADDING_COUNT 16
typedef struct {
    const uint8_t *lastb;
    uint8_t *pad;
    int length;
} SMP_CryptPaddingParam;

typedef enum {
    PAIR_REQ_0,
    PAIR_REQ_1,
    PAIR_REQ_2,
    PAIR_REQ_3,
    PAIR_REQ_4,
    PAIR_REQ_5,
    PAIR_REQ_6,
} SMP_PairReqInput;

typedef enum {
    PAIR_RES_0,
    PAIR_RES_1,
    PAIR_RES_2,
    PAIR_RES_3,
    PAIR_RES_4,
    PAIR_RES_5,
    PAIR_RES_6,
} SMP_PairResInput;

static void SMP_CryptographicLeftShiftOneBit(SMP_CryptLeftShiftOneBitParam *param);
static void SMP_CryptographicXor128(SMP_CryptXor128Param *param);
static void SMP_CryptographicPadding(SMP_CryptPaddingParam *param);
static void SMP_CalcPairMethodLegacyPair(SMP_PairMng *mng);
static void SMP_CalcPairMethodScPair(SMP_PairMng *mng);
static void SMP_SetkeyDistCmdFlag(SMP_PairMng *mng);
static void SMP_WriteToRow(uint8_t *des, uint8_t desMax, uint8_t offset, const uint8_t *src, uint8_t length);

static void SMP_WriteToRow(uint8_t *des, uint8_t desMax, uint8_t offset, const uint8_t *src, uint8_t length)
{
    (void)memcpy_s(des + offset, desMax - offset, src, length);
}

static void SMP_CryptographicLeftShiftOneBit(SMP_CryptLeftShiftOneBitParam *param)
{
    int i;
    uint8_t overflow = 0x00;

    for (i = CRYPT_LEFTSHIFTONEBIT_OUT_LEN - 1; i >= 0x00; i--) {
        param->output[i] = (uint8_t)(param->input[i] << 0x01);
        param->output[i] |= overflow;
        overflow = (param->input[i] & 0x80) ? 0x01 : 0x00;
    }
}

static void SMP_CryptographicXor128(SMP_CryptXor128Param *param)
{
    int i;

    for (i = 0x00; i < CRYPT_XOR128_OUT_LEN; i++) {
        param->out[i] = param->a[i] ^ param->b[i];
    }
}

static void SMP_CryptographicPadding(SMP_CryptPaddingParam *param)
{
    int j;

    for (j = 0x00; j < CRYPT_PADDING_COUNT; j++) {
        if (j < param->length) {
            param->pad[j] = param->lastb[j];
        } else if (j == param->length) {
            param->pad[j] = 0x80;
        } else {
            param->pad[j] = 0x00;
        }
    }
}

void SMP_ConstituteC1Step1Param(uint8_t role, const uint8_t *random, uint8_t randomLen, SMP_CryptC1Step1Param *param)
{
    if (role == SMP_ROLE_MASTER) {
        param->preq[PAIR_REQ_6] = SMP_CODE_PAIRING_REQ;
        param->preq[PAIR_REQ_5] = SMP_GetPairMng()->local.pairParam.ioCapability;
        param->preq[PAIR_REQ_4] = SMP_GetPairMng()->local.pairParam.oobDataFlag;
        param->preq[PAIR_REQ_3] = SMP_GetPairMng()->local.pairParam.authReq;
        param->preq[PAIR_REQ_2] = SMP_GetPairMng()->local.pairParam.maxEncKeySize;
        param->preq[PAIR_REQ_1] = SMP_GetPairMng()->local.pairParam.initKeyDist;
        param->preq[PAIR_REQ_0] = SMP_GetPairMng()->local.pairParam.respKeyDist;
        param->pres[PAIR_RES_6] = SMP_CODE_PAIRING_RSP;
        param->pres[PAIR_RES_5] = SMP_GetPairMng()->peer.pairParam.ioCapability;
        param->pres[PAIR_RES_4] = SMP_GetPairMng()->peer.pairParam.oobDataFlag;
        param->pres[PAIR_RES_3] = SMP_GetPairMng()->peer.pairParam.authReq;
        param->pres[PAIR_RES_2] = SMP_GetPairMng()->peer.pairParam.maxEncKeySize;
        param->pres[PAIR_RES_1] = SMP_GetPairMng()->peer.pairParam.initKeyDist;
        param->pres[PAIR_RES_0] = SMP_GetPairMng()->peer.pairParam.respKeyDist;
        param->iat = SMP_GetPairMng()->local.addr.type;
        param->rat = SMP_GetPairMng()->peer.addr.type;
    } else {
        param->preq[PAIR_REQ_6] = SMP_CODE_PAIRING_REQ;
        param->preq[PAIR_REQ_5] = SMP_GetPairMng()->peer.pairParam.ioCapability;
        param->preq[PAIR_REQ_4] = SMP_GetPairMng()->peer.pairParam.oobDataFlag;
        param->preq[PAIR_REQ_3] = SMP_GetPairMng()->peer.pairParam.authReq;
        param->preq[PAIR_REQ_2] = SMP_GetPairMng()->peer.pairParam.maxEncKeySize;
        param->preq[PAIR_REQ_1] = SMP_GetPairMng()->peer.pairParam.initKeyDist;
        param->preq[PAIR_REQ_0] = SMP_GetPairMng()->peer.pairParam.respKeyDist;
        param->pres[PAIR_RES_6] = SMP_CODE_PAIRING_RSP;
        param->pres[PAIR_RES_5] = SMP_GetPairMng()->local.pairParam.ioCapability;
        param->pres[PAIR_RES_4] = SMP_GetPairMng()->local.pairParam.oobDataFlag;
        param->pres[PAIR_RES_3] = SMP_GetPairMng()->local.pairParam.authReq;
        param->pres[PAIR_RES_2] = SMP_GetPairMng()->local.pairParam.maxEncKeySize;
        param->pres[PAIR_RES_1] = SMP_GetPairMng()->local.pairParam.initKeyDist;
        param->pres[PAIR_RES_0] = SMP_GetPairMng()->local.pairParam.respKeyDist;
        param->iat = SMP_GetPairMng()->peer.addr.type;
        param->rat = SMP_GetPairMng()->local.addr.type;
    }
    (void)memcpy_s(param->r, SMP_RANDOM_DATA_LEN, random, randomLen);
}

int SMP_CryptographicC1Step1(uint16_t step, SMP_CryptC1Step1Param *param)
{
    HciLeEncryptParam encryptParam;
    uint8_t iat, rat, p1[CRYPT_C1_P1_LEN];
    uint8_t i;
    LOG_DEBUG("%{public}s", __FUNCTION__);

    (void)memset_s(p1, CRYPT_C1_P1_LEN, 0x00, CRYPT_C1_P1_LEN);
    (void)memset_s(param->output, CRYPT_C1_OUT_LEN, 0x00, CRYPT_C1_OUT_LEN);
    iat = param->iat & 0x01;
    rat = param->rat & 0x01;
    uint8_t offset = 0x00;
    SMP_WriteToRow(p1, CRYPT_C1_P1_LEN, offset, param->pres, SMP_CMD_PAIR_RSP_DATA_LEN);
    offset += SMP_CMD_PAIR_RSP_DATA_LEN;
    SMP_WriteToRow(p1, CRYPT_C1_P1_LEN, offset, param->preq, SMP_CMD_PAIR_REQ_DATA_LEN);
    offset += SMP_CMD_PAIR_REQ_DATA_LEN;
    p1[offset] = rat;
    offset += sizeof(rat);
    p1[offset] = iat;
    for (i = 0x00; i < CRYPT_C1_OUT_LEN; i++) {
        param->output[i] = param->r[i] ^ p1[i];
    }
    SMP_MemoryReverseCopy(encryptParam.key, SMP_GetPairMng()->TK, SMP_TK_LEN);
    SMP_MemoryReverseCopy(encryptParam.plaintextData, param->output, sizeof(param->output));
    int ret = SMP_SendLeEncryptCmd(&encryptParam, step, NULL, SMP_USING_HW_AES128_PAIR);
    return ret;
}

int SMP_ConstituteC1Step2Param(const SMP_EncData *encData, uint8_t role, SMP_CryptC1Step2Param *param)
{
    const HciLeEncryptReturnParam *returnParam = encData->encRetParam;
    int ret = SMP_EncryptCompleteJudgeException(returnParam->status, role);
    if (ret != SMP_SUCCESS) {
        return ret;
    }
    SMP_MemoryReverseCopy(param->input, returnParam->encryptedData, sizeof(param->input));
    if (role == SMP_ROLE_MASTER) {
        (void)memcpy_s(param->ia, CRYPT_C1_IA_LEN, SMP_GetPairMng()->local.addr.addr, CRYPT_C1_IA_LEN);
        (void)memcpy_s(param->ra, CRYPT_C1_RA_LEN, SMP_GetPairMng()->peer.addr.addr, CRYPT_C1_RA_LEN);
    } else {
        (void)memcpy_s(param->ia, CRYPT_C1_IA_LEN, SMP_GetPairMng()->peer.addr.addr, CRYPT_C1_IA_LEN);
        (void)memcpy_s(param->ra, CRYPT_C1_RA_LEN, SMP_GetPairMng()->local.addr.addr, CRYPT_C1_RA_LEN);
    }
    return ret;
}

int SMP_CryptographicC1Step2(uint16_t step, SMP_CryptC1Step2Param *param)
{
    HciLeEncryptParam encryptParam;
    uint8_t p2[CRYPT_C1_P2_LEN], padding[CRYPT_C1_PADDING_LEN];
    uint8_t i;
    LOG_DEBUG("%{public}s", __FUNCTION__);

    (void)memset_s(p2, CRYPT_C1_P2_LEN, 0x00, CRYPT_C1_P2_LEN);
    (void)memset_s(padding, CRYPT_C1_PADDING_LEN, 0x00, CRYPT_C1_PADDING_LEN);
    (void)memset_s(param->output, CRYPT_C1_OUT_LEN, 0x00, CRYPT_C1_OUT_LEN);

    uint8_t offset = 0x00;
    SMP_WriteToRow(p2, CRYPT_C1_P2_LEN, offset, padding, CRYPT_C1_PADDING_LEN);
    offset += CRYPT_C1_PADDING_LEN;
    SMP_WriteToRow(p2, CRYPT_C1_P2_LEN, offset, param->ia, CRYPT_C1_IA_LEN);
    offset += CRYPT_C1_IA_LEN;
    SMP_WriteToRow(p2, CRYPT_C1_P2_LEN, offset, param->ra, CRYPT_C1_RA_LEN);
    for (i = 0x00; i < CRYPT_C1_OUT_LEN; i++) {
        param->output[i] = param->input[i] ^ p2[i];
    }
    SMP_MemoryReverseCopy(encryptParam.key, SMP_GetPairMng()->TK, SMP_TK_LEN);
    SMP_MemoryReverseCopy(encryptParam.plaintextData, param->output, sizeof(param->output));
    int ret = SMP_SendLeEncryptCmd(&encryptParam, step, NULL, SMP_USING_HW_AES128_PAIR);
    return ret;
}

void SMP_CryptographicS1(SMP_CryptS1Param *param)
{
    LOG_DEBUG("%{public}s", __FUNCTION__);
    (void)memset_s(param->output, CRYPT_S1_OUT_LEN, 0x00, CRYPT_S1_OUT_LEN);
    uint8_t offset = 0x00;
    SMP_WriteToRow(param->output, CRYPT_S1_OUT_LEN, offset, &param->r1[SMP_RAND_NUM_LEN], SMP_RAND_NUM_LEN);
    offset += SMP_RAND_NUM_LEN;
    SMP_WriteToRow(param->output, CRYPT_S1_OUT_LEN, offset, &param->r2[SMP_RAND_NUM_LEN], SMP_RAND_NUM_LEN);
}

void SMP_ConstituteF4Param(bool isPasskey, SMP_CryptF4Param *param)
{
    uint32_t passkey = 0x00;
    if (isPasskey) {
        SMP_MemoryReverseCopy((uint8_t *)(&passkey), (SMP_GetPairMng()->TK + 0x0C), sizeof(uint32_t));
        if (((uint32_t)(1 << SMP_GetPairMng()->scConfirmCheckCounter)) & passkey) {
            param->Z[0x00] = 0x81;
        } else {
            param->Z[0x00] = 0x80;
        }
    } else {
        param->Z[0x00] = 0x00;
    }
}

void SMP_CryptographicF4(SMP_CryptF4Param *param)
{
    uint8_t offset = 0x00;
    SMP_WriteToRow(param->output, CRYPT_F4_OUT_LEN, offset, param->U, SMP_PUBLICKEY_X_LEN);
    offset += SMP_PUBLICKEY_X_LEN;
    SMP_WriteToRow(param->output, CRYPT_F4_OUT_LEN, offset, param->V, SMP_PUBLICKEY_X_LEN);
    offset += SMP_PUBLICKEY_X_LEN;
    SMP_WriteToRow(param->output, CRYPT_F4_OUT_LEN, offset, param->Z, 0x01);
}

void SMP_ConstituteF5Param(uint8_t role, SMP_CryptF5Param *cryptF5Param)
{
    if (role == SMP_ROLE_MASTER) {
        cryptF5Param->A1[0x00] = SMP_GetPairMng()->local.addr.type;
        cryptF5Param->A2[0x00] = SMP_GetPairMng()->peer.addr.type;
        (void)memcpy_s(&cryptF5Param->A1[0x01], BT_ADDRESS_SIZE, SMP_GetPairMng()->local.addr.addr, BT_ADDRESS_SIZE);
        (void)memcpy_s(&cryptF5Param->A2[0x01], BT_ADDRESS_SIZE, SMP_GetPairMng()->peer.addr.addr, BT_ADDRESS_SIZE);
        (void)memcpy_s(cryptF5Param->N1, SMP_RANDOM_DATA_LEN, SMP_GetPairMng()->local.random, sizeof(cryptF5Param->N1));
        (void)memcpy_s(cryptF5Param->N2, SMP_RANDOM_DATA_LEN, SMP_GetPairMng()->peer.random, sizeof(cryptF5Param->N2));
    } else {
        cryptF5Param->A1[0x00] = SMP_GetPairMng()->peer.addr.type;
        cryptF5Param->A2[0x00] = SMP_GetPairMng()->local.addr.type;
        (void)memcpy_s(&cryptF5Param->A1[0x01], BT_ADDRESS_SIZE, SMP_GetPairMng()->peer.addr.addr, BT_ADDRESS_SIZE);
        (void)memcpy_s(&cryptF5Param->A2[0x01], BT_ADDRESS_SIZE, SMP_GetPairMng()->local.addr.addr, BT_ADDRESS_SIZE);
        (void)memcpy_s(cryptF5Param->N1, SMP_RANDOM_DATA_LEN, SMP_GetPairMng()->peer.random, sizeof(cryptF5Param->N1));
        (void)memcpy_s(cryptF5Param->N2, SMP_RANDOM_DATA_LEN, SMP_GetPairMng()->local.random, sizeof(cryptF5Param->N2));
    }
}

void SMP_CryptographicF5(SMP_CryptF5Param *param)
{
    uint8_t keyID[CRYPT_F5_KEYID_LEN] = {0x62, 0x74, 0x6C, 0x65};
    uint8_t counter0[0x01] = {0x00};
    uint8_t length[CRYPT_F5_LENGTH_LEN] = {0x01, 0x00};
    uint8_t offset = 0x00;
    SMP_WriteToRow(param->output, CRYPT_F5_OUT_LEN, offset, counter0, 0x01);
    offset += 0x01;
    SMP_WriteToRow(param->output, CRYPT_F5_OUT_LEN, offset, keyID, CRYPT_F5_KEYID_LEN);
    offset += CRYPT_F5_KEYID_LEN;
    SMP_WriteToRow(param->output, CRYPT_F5_OUT_LEN, offset, param->N1, SMP_RANDOM_DATA_LEN);
    offset += SMP_RANDOM_DATA_LEN;
    SMP_WriteToRow(param->output, CRYPT_F5_OUT_LEN, offset, param->N2, SMP_RANDOM_DATA_LEN);
    offset += SMP_RANDOM_DATA_LEN;
    SMP_WriteToRow(param->output, CRYPT_F5_OUT_LEN, offset, param->A1, CRYPT_F5_A1_LEN);
    offset += CRYPT_F5_A1_LEN;
    SMP_WriteToRow(param->output, CRYPT_F5_OUT_LEN, offset, param->A2, CRYPT_F5_A2_LEN);
    offset += CRYPT_F5_A2_LEN;
    SMP_WriteToRow(param->output, CRYPT_F5_OUT_LEN, offset, length, CRYPT_F5_LENGTH_LEN);
}

void SMP_ConstituteF6Param(bool isCalculatePeer, SMP_CryptF6Param *cryptF6Param)
{
    if (isCalculatePeer) {
        int offset = 0;
        cryptF6Param->IOcap[offset] = SMP_GetPairMng()->peer.pairParam.authReq;
        offset += sizeof(SMP_GetPairMng()->peer.pairParam.authReq);
        cryptF6Param->IOcap[offset] = SMP_GetPairMng()->peer.pairParam.oobDataFlag;
        offset += sizeof(SMP_GetPairMng()->peer.pairParam.oobDataFlag);
        cryptF6Param->IOcap[offset] = SMP_GetPairMng()->peer.pairParam.ioCapability;
        cryptF6Param->A1[0] = SMP_GetPairMng()->peer.addr.type;
        cryptF6Param->A2[0] = SMP_GetPairMng()->local.addr.type;
        (void)memcpy_s(&cryptF6Param->A1[1], BT_ADDRESS_SIZE, SMP_GetPairMng()->peer.addr.addr, BT_ADDRESS_SIZE);
        (void)memcpy_s(&cryptF6Param->A2[1], BT_ADDRESS_SIZE, SMP_GetPairMng()->local.addr.addr, BT_ADDRESS_SIZE);
        (void)memset_s(cryptF6Param->R, SMP_RANDOM_DATA_LEN, 0x00, sizeof(cryptF6Param->R));
        if (SMP_IsScOobPair(SMP_GetPairMng()->local.pairMethod)) {
            (void)memcpy_s(cryptF6Param->R, SMP_RANDOM_DATA_LEN, SMP_GetPairMng()->local.oobRand, SMP_RANDOM_DATA_LEN);
        }
        (void)memcpy_s(cryptF6Param->N1, SMP_RANDOM_DATA_LEN, SMP_GetPairMng()->peer.random, sizeof(cryptF6Param->N1));
        (void)memcpy_s(cryptF6Param->N2, SMP_RANDOM_DATA_LEN, SMP_GetPairMng()->local.random, sizeof(cryptF6Param->N2));
    } else {
        int offset = 0;
        cryptF6Param->IOcap[offset] = SMP_GetPairMng()->local.pairParam.authReq;
        offset += sizeof(SMP_GetPairMng()->local.pairParam.authReq);
        cryptF6Param->IOcap[offset] = SMP_GetPairMng()->local.pairParam.oobDataFlag;
        offset += sizeof(SMP_GetPairMng()->local.pairParam.oobDataFlag);
        cryptF6Param->IOcap[offset] = SMP_GetPairMng()->local.pairParam.ioCapability;
        cryptF6Param->A1[0] = SMP_GetPairMng()->local.addr.type;
        cryptF6Param->A2[0] = SMP_GetPairMng()->peer.addr.type;
        (void)memcpy_s(&cryptF6Param->A1[1], BT_ADDRESS_SIZE, SMP_GetPairMng()->local.addr.addr, BT_ADDRESS_SIZE);
        (void)memcpy_s(&cryptF6Param->A2[1], BT_ADDRESS_SIZE, SMP_GetPairMng()->peer.addr.addr, BT_ADDRESS_SIZE);
        (void)memset_s(cryptF6Param->R, SMP_RANDOM_DATA_LEN, 0x00, sizeof(cryptF6Param->R));
        if (SMP_IsScOobPair(SMP_GetPairMng()->local.pairMethod)) {
            (void)memcpy_s(cryptF6Param->R, SMP_RANDOM_DATA_LEN, SMP_GetPairMng()->peer.oobRand, SMP_RANDOM_DATA_LEN);
        }
        (void)memcpy_s(cryptF6Param->N1, SMP_RANDOM_DATA_LEN, SMP_GetPairMng()->local.random, sizeof(cryptF6Param->N1));
        (void)memcpy_s(cryptF6Param->N2, SMP_RANDOM_DATA_LEN, SMP_GetPairMng()->peer.random, sizeof(cryptF6Param->N2));
    }
    if (SMP_IsPasskeyEntryPair(SMP_GetPairMng()->local.pairMethod)) {
        (void)memcpy_s(cryptF6Param->R, SMP_RANDOM_DATA_LEN, SMP_GetPairMng()->TK, SMP_TK_LEN);
    }
}

void SMP_CryptographicF6(SMP_CryptF6Param *param)
{
    uint8_t offset = 0x00;
    SMP_WriteToRow(param->output, CRYPT_F6_OUT_LEN, offset, param->N1, SMP_RANDOM_DATA_LEN);
    offset += SMP_RANDOM_DATA_LEN;
    SMP_WriteToRow(param->output, CRYPT_F6_OUT_LEN, offset, param->N2, SMP_RANDOM_DATA_LEN);
    offset += SMP_RANDOM_DATA_LEN;
    SMP_WriteToRow(param->output, CRYPT_F6_OUT_LEN, offset, param->R, SMP_RANDOM_DATA_LEN);
    offset += SMP_RANDOM_DATA_LEN;
    SMP_WriteToRow(param->output, CRYPT_F6_OUT_LEN, offset, param->IOcap, CRYPT_F6_IO_LEN);
    offset += CRYPT_F6_IO_LEN;
    SMP_WriteToRow(param->output, CRYPT_F6_OUT_LEN, offset, param->A1, CRYPT_F6_A1_LEN);
    offset += CRYPT_F6_A1_LEN;
    SMP_WriteToRow(param->output, CRYPT_F6_OUT_LEN, offset, param->A2, CRYPT_F6_A2_LEN);
}

void SMP_CryptographicG2(SMP_CryptG2Param *param)
{
    uint8_t offset = 0x00;
    SMP_WriteToRow(param->output, CRYPT_G2_OUT_LEN, offset, param->U, SMP_PUBLICKEY_X_LEN);
    offset += SMP_PUBLICKEY_X_LEN;
    SMP_WriteToRow(param->output, CRYPT_G2_OUT_LEN, offset, param->V, SMP_PUBLICKEY_X_LEN);
    offset += SMP_PUBLICKEY_X_LEN;
    SMP_WriteToRow(param->output, CRYPT_G2_OUT_LEN, offset, param->Y, SMP_RANDOM_DATA_LEN);
}

void SMP_CryptographicAesCmacStep1(SMP_CryptAesCmacStep1Param *param)
{
    uint8_t tmp[CRYPT_AESCMAC_TMP_LEN] = {0x00};
    SMP_CryptXor128Param cryptXor128param = {
        .a = tmp,
        .b = CONST_RB,
    };
    SMP_CryptLeftShiftOneBitParam cryptLeftShiftOneBitParam;

    if ((param->input[0x00] & 0x80) == 0x00) {
        cryptLeftShiftOneBitParam.input = param->input;
        cryptLeftShiftOneBitParam.output = param->output1;
        SMP_CryptographicLeftShiftOneBit(&cryptLeftShiftOneBitParam);
    } else {
        cryptLeftShiftOneBitParam.input = param->input;
        cryptLeftShiftOneBitParam.output = tmp;
        SMP_CryptographicLeftShiftOneBit(&cryptLeftShiftOneBitParam);
        cryptXor128param.out = param->output1;
        SMP_CryptographicXor128(&cryptXor128param);
    }

    if ((param->output1[0x00] & 0x80) == 0x00) {
        cryptLeftShiftOneBitParam.input = param->output1;
        cryptLeftShiftOneBitParam.output = param->output2;
        SMP_CryptographicLeftShiftOneBit(&cryptLeftShiftOneBitParam);
    } else {
        cryptLeftShiftOneBitParam.input = param->output1;
        cryptLeftShiftOneBitParam.output = tmp;
        SMP_CryptographicLeftShiftOneBit(&cryptLeftShiftOneBitParam);
        cryptXor128param.out = param->output2;
        SMP_CryptographicXor128(&cryptXor128param);
    }
}

void SMP_CryptographicAesCmacStep2(SMP_CryptAesCmacStep2Param *param)
{
    uint8_t padded[CRYPT_AESCMAC_PADDED_LEN];
    int flag;
    SMP_CryptXor128Param cryptXor128param;
    SMP_CryptPaddingParam cryptPaddingParam;

    if (param == NULL) {
        return;
    }

    (void)memset_s(padded, CRYPT_AESCMAC_PADDED_LEN, 0x00, CRYPT_AESCMAC_PADDED_LEN);

    // An empty, negative-length, or missing message is handled by the top-level CMAC
    // routine, which builds the final padded block directly. Step2 only operates on
    // non-empty input. If called with empty/negative input, preserve the legacy
    // n == 1 convention so Step3 enters the final-block branch instead of looping
    // on an unsigned wrap-around.
    if (param->length <= 0x00 || param->input3 == NULL) {
        (void)memset_s(param->output, CRYPT_AESCMAC_OUT_LEN, 0x00, CRYPT_AESCMAC_OUT_LEN);
        param->n = 0x01;
        return;
    }

    param->n = (param->length + CRYPT_AESCMAC_TMP_LEN - 1) / CRYPT_AESCMAC_TMP_LEN;

    if (param->n == 0x00) {
        param->n = 0x01;
        flag = 0x00;
    } else {
        if ((param->length % CRYPT_AESCMAC_TMP_LEN) == 0x00) {
            flag = 0x01;
        } else {
            flag = 0x00;
        }
    }

    if (flag) {
        cryptXor128param.a = &param->input3[CRYPT_AESCMAC_TMP_LEN * (param->n - 0x01)];
        cryptXor128param.b = param->input1;
        cryptXor128param.out = param->output;
        SMP_CryptographicXor128(&cryptXor128param);
    } else {
        cryptPaddingParam.lastb = &param->input3[CRYPT_AESCMAC_TMP_LEN * (param->n - 0x01)];
        cryptPaddingParam.pad = padded;
        cryptPaddingParam.length = param->length % CRYPT_AESCMAC_TMP_LEN;
        SMP_CryptographicPadding(&cryptPaddingParam);
        cryptXor128param.a = padded;
        cryptXor128param.b = param->input2;
        cryptXor128param.out = param->output;
        SMP_CryptographicXor128(&cryptXor128param);
    }
}

void SMP_ConstituteAesCmacStep3Param(
    const SMP_EncData *encData, const uint8_t *data, uint8_t dataLen, SMP_CryptAesCmacStep3Param *param)
{
    SMP_CryptAesCmacStep1Param cryptAesCmacStep1Param;
    SMP_CryptAesCmacStep2Param cryptAesCmacStep2Param;
    SMP_MemoryReverseCopy(cryptAesCmacStep1Param.input, encData->encRetParam->encryptedData, CRYPT_AESCMAC_IN_LEN);
    SMP_CryptographicAesCmacStep1(&cryptAesCmacStep1Param);
    (void)memcpy_s(
        cryptAesCmacStep2Param.input1, CRYPT_AESCMAC_IN_LEN, cryptAesCmacStep1Param.output1, CRYPT_AESCMAC_IN_LEN);
    (void)memcpy_s(
        cryptAesCmacStep2Param.input2, CRYPT_AESCMAC_IN_LEN, cryptAesCmacStep1Param.output2, CRYPT_AESCMAC_IN_LEN);
    cryptAesCmacStep2Param.input3 = data;
    cryptAesCmacStep2Param.length = dataLen;
    SMP_CryptographicAesCmacStep2(&cryptAesCmacStep2Param);
    param->message = data;
    param->messageSize = dataLen;
    (void)memcpy_s(
        param->aesCmacOutput, SMP_AES_CMAC_OUTPUT_LEN, cryptAesCmacStep2Param.output, SMP_AES_CMAC_OUTPUT_LEN);
    (void)memcpy_s(param->key, CRYPT_AESCMAC_KEY_LEN, encData->encCmd->key, CRYPT_AESCMAC_KEY_LEN);
    param->n = cryptAesCmacStep2Param.n;
    param->signCounter = 0x00;
    param->isUsingHwAes128 = SMP_USING_HW_AES128_PAIR;
}

int SMP_CryptographicAesCmacStep3(const SMP_CryptAesCmacStep3Param *param)
{
    int ret;
    SMP_EncCmd *encCmd = SMP_AllocEncCmd();
    if (encCmd == NULL) {
        LOG_ERROR("%{public}s: Alloc error.", __FUNCTION__);
        return SMP_ERR_OUT_OF_RES;
    }
    HciLeEncryptParam encryptParam;
    SMP_CryptXor128Param cryptXor128param = {
        .a = encCmd->X,
        .out = encCmd->Y,
    };

    (void)memset_s(encCmd->X, sizeof(encCmd->X), 0x00, sizeof(encCmd->X));
    (void)memset_s(encCmd->Y, sizeof(encCmd->Y), 0x00, sizeof(encCmd->Y));
    if (param->messageSize != 0) {
        encCmd->length = param->messageSize;
        encCmd->M = MEM_MALLOC.alloc(encCmd->length);
        (void)memcpy_s(encCmd->M, param->messageSize, param->message, param->messageSize);
    }
    (void)memcpy_s(encCmd->M_last, sizeof(encCmd->M_last), param->aesCmacOutput, sizeof(encCmd->M_last));
    (void)memcpy_s(encCmd->key, sizeof(encCmd->key), param->key, sizeof(encCmd->key));
    encCmd->n = param->n;
    encCmd->i = 0x00;
    encCmd->signCounter = param->signCounter;

    if (encCmd->i < (encCmd->n - 0x01)) {
        cryptXor128param.b = &param->message[CRYPT_AESCMAC_TMP_LEN * encCmd->i];
        SMP_CryptographicXor128(&cryptXor128param);
        encCmd->i++;

        (void)memcpy_s(encryptParam.key, sizeof(encryptParam.key), param->key, CRYPT_AESCMAC_KEY_LEN);
        SMP_MemoryReverseCopy(encryptParam.plaintextData, encCmd->Y, CRYPT_AESCMAC_Y_LEN);
        LOG_DEBUG("step %04X started.", param->stepA);
        ret = SMP_SendLeEncryptCmd(&encryptParam, param->stepA, encCmd, param->isUsingHwAes128);
    } else {
        cryptXor128param.b = encCmd->M_last;
        SMP_CryptographicXor128(&cryptXor128param);

        (void)memcpy_s(encryptParam.key, sizeof(encryptParam.key), param->key, CRYPT_AESCMAC_KEY_LEN);
        SMP_MemoryReverseCopy(encryptParam.plaintextData, encCmd->Y, CRYPT_AESCMAC_Y_LEN);
        LOG_DEBUG("step %04X started.", param->stepB);
        ret = SMP_SendLeEncryptCmd(&encryptParam, param->stepB, encCmd, param->isUsingHwAes128);
    }

    SMP_FreeEncCmd(encCmd);

    return ret;
}

int SMP_ConstituteAesCmacStep4Param(
    const SMP_EncData *encData, uint16_t stepA, uint16_t stepB, uint8_t role, SMP_CryptAesCmacStep4Param *param)
{
    int ret = SMP_EncryptCompleteJudgeException(encData->encRetParam->status, role);
    if (ret != SMP_SUCCESS) {
        return ret;
    }
    param->stepA = stepA;
    param->stepB = stepB;
    param->pEncCmdData = encData->encCmd;
    (void)memcpy_s(param->X, CRYPT_AESCMAC_X_LEN, encData->encRetParam->encryptedData, CRYPT_AESCMAC_X_LEN);
    param->isUsingHwAes128 = SMP_USING_HW_AES128_PAIR;
    return ret;
}

int SMP_CryptographicAesCmacStep4(SMP_CryptAesCmacStep4Param *param)
{
    int ret;
    HciLeEncryptParam encryptParam;
    SMP_CryptXor128Param cryptXor128param = {
        .a = param->pEncCmdData->X,
        .out = param->pEncCmdData->Y,
    };

    SMP_MemoryReverseCopy(param->pEncCmdData->X, param->X, CRYPT_AESCMAC_X_LEN);

    if (param->pEncCmdData->i < (param->pEncCmdData->n - 0x01)) {
        cryptXor128param.b = &param->pEncCmdData->M[CRYPT_AESCMAC_TMP_LEN * param->pEncCmdData->i];
        SMP_CryptographicXor128(&cryptXor128param);
        param->pEncCmdData->i++;

        (void)memcpy_s(encryptParam.key, sizeof(encryptParam.key), param->pEncCmdData->key, CRYPT_AESCMAC_KEY_LEN);
        SMP_MemoryReverseCopy(encryptParam.plaintextData, param->pEncCmdData->Y, CRYPT_AESCMAC_Y_LEN);
        LOG_DEBUG("step %04X started.", param->stepA);
        ret = SMP_SendLeEncryptCmd(&encryptParam, param->stepA, param->pEncCmdData, param->isUsingHwAes128);
    } else {
        cryptXor128param.b = param->pEncCmdData->M_last;
        SMP_CryptographicXor128(&cryptXor128param);

        (void)memcpy_s(encryptParam.key, sizeof(encryptParam.key), param->pEncCmdData->key, CRYPT_AESCMAC_KEY_LEN);
        SMP_MemoryReverseCopy(encryptParam.plaintextData, param->pEncCmdData->Y, CRYPT_AESCMAC_Y_LEN);
        LOG_DEBUG("step %04X started.", param->stepB);
        ret = SMP_SendLeEncryptCmd(&encryptParam, param->stepB, param->pEncCmdData, param->isUsingHwAes128);
    }
    return ret;
}

void SMP_MemoryReverseCopy(uint8_t *dest, const uint8_t *source, uint16_t length)
{
    for (uint16_t temp = 0x00; temp < length; temp++) {
        *(dest + temp) = *(source + length - 0x01 - temp);
    }
}

void SMP_ReverseMemoryOrder(uint8_t *buf, uint16_t length)
{
    uint16_t index;

    for (index = 0x00; index < (length / 0x02); index++) {
        uint8_t temp = buf[index];
        buf[index] = buf[length - 0x01 - index];
        buf[length - 0x01 - index] = temp;
    }
}

void SMP_ClearPairState(SMP_PairMng *mng)
{
    if (mng->alarm != NULL) {
        AlarmCancel(mng->alarm);
        AlarmDelete(mng->alarm);
    }
    (void)memset_s(mng, sizeof(SMP_PairMng), 0x00, sizeof(SMP_PairMng));
}

void SMP_CalculatePairType(SMP_PairMng *mng)
{
    LOG_INFO("%{public}s", __FUNCTION__);

    if ((mng->local.pairParam.authReq & SMP_AUTH_REQ_BIT_SC) && (mng->peer.pairParam.authReq & SMP_AUTH_REQ_BIT_SC)) {
        mng->pairType = SMP_PAIR_TYPE_SECURE_CONNECTION;
    } else {
        mng->pairType = SMP_PAIR_TYPE_LEGACY;
    }
}

void SMP_CalculateEncKeySize(SMP_PairMng *mng)
{
    LOG_INFO("%{public}s", __FUNCTION__);

    if (mng->local.pairParam.maxEncKeySize >= mng->peer.pairParam.maxEncKeySize) {
        mng->encKeySize = mng->peer.pairParam.maxEncKeySize;
    } else {
        mng->encKeySize = mng->local.pairParam.maxEncKeySize;
    }
}

void SMP_CalculatePairMethod(SMP_PairMng *mng)
{
    LOG_INFO("%{public}s", __FUNCTION__);

    if (SMP_PAIR_TYPE_LEGACY == mng->pairType) {
        SMP_CalcPairMethodLegacyPair(mng);
    } else {
        SMP_CalcPairMethodScPair(mng);
    }
}

void SMP_CalculateKeyDistribution(SMP_PairMng *mng)
{
    LOG_INFO("%{public}s", __FUNCTION__);

    if (SMP_ROLE_MASTER == mng->role) {
        mng->local.keyDist = mng->local.pairParam.initKeyDist & mng->peer.pairParam.initKeyDist;
        mng->peer.keyDist = mng->local.pairParam.respKeyDist & mng->peer.pairParam.respKeyDist;
    } else {
        mng->local.keyDist = mng->local.pairParam.respKeyDist & mng->peer.pairParam.respKeyDist;
        mng->peer.keyDist = mng->local.pairParam.initKeyDist & mng->peer.pairParam.initKeyDist;
        mng->local.pairParam.initKeyDist = mng->peer.keyDist;
        mng->local.pairParam.respKeyDist = mng->local.keyDist;
    }

    if (SMP_PAIR_TYPE_SECURE_CONNECTION == mng->pairType) {
        mng->local.keyDist = mng->local.keyDist & (~SMP_KEY_DIST_BIT_ENC_KEY);
        mng->peer.keyDist = mng->peer.keyDist & (~SMP_KEY_DIST_BIT_ENC_KEY);
    }

    SMP_SetkeyDistCmdFlag(mng);
}

static void SMP_SetkeyDistCmdFlag(SMP_PairMng *mng)
{
    mng->local.keyDistCmdFlag = 0x00;
    mng->peer.keyDistCmdFlag = 0x00;

    if (mng->local.keyDist & SMP_KEY_DIST_BIT_ENC_KEY) {
        mng->local.keyDistCmdFlag = mng->local.keyDistCmdFlag | SMP_KEY_DIST_CMD_FLAG_BIT_ENC_INFO;
        mng->local.keyDistCmdFlag = mng->local.keyDistCmdFlag | SMP_KEY_DIST_CMD_FLAG_BIT_MASTER_IDENT;
    }

    if (mng->local.keyDist & SMP_KEY_DIST_BIT_ID_KEY) {
        mng->local.keyDistCmdFlag = mng->local.keyDistCmdFlag | SMP_KEY_DIST_CMD_FLAG_BIT_IDENT_INFO;
        mng->local.keyDistCmdFlag = mng->local.keyDistCmdFlag | SMP_KEY_DIST_CMD_FLAG_BIT_IDENT_ADDR;
    }

    if (mng->local.keyDist & SMP_KEY_DIST_BIT_SIGN_KEY) {
        mng->local.keyDistCmdFlag = mng->local.keyDistCmdFlag | SMP_KEY_DIST_CMD_FLAG_BIT_SIGN_INFO;
    }

    if (mng->peer.keyDist & SMP_KEY_DIST_BIT_ENC_KEY) {
        mng->peer.keyDistCmdFlag = mng->peer.keyDistCmdFlag | SMP_KEY_DIST_CMD_FLAG_BIT_ENC_INFO;
        mng->peer.keyDistCmdFlag = mng->peer.keyDistCmdFlag | SMP_KEY_DIST_CMD_FLAG_BIT_MASTER_IDENT;
    }

    if (mng->peer.keyDist & SMP_KEY_DIST_BIT_ID_KEY) {
        mng->peer.keyDistCmdFlag = mng->peer.keyDistCmdFlag | SMP_KEY_DIST_CMD_FLAG_BIT_IDENT_INFO;
        mng->peer.keyDistCmdFlag = mng->peer.keyDistCmdFlag | SMP_KEY_DIST_CMD_FLAG_BIT_IDENT_ADDR;
    }

    if (mng->peer.keyDist & SMP_KEY_DIST_BIT_SIGN_KEY) {
        mng->peer.keyDistCmdFlag = mng->peer.keyDistCmdFlag | SMP_KEY_DIST_CMD_FLAG_BIT_SIGN_INFO;
    }
}

static void SMP_CalcPairMethodLegacyPair(SMP_PairMng *mng)
{
    if (mng->local.pairParam.oobDataFlag && mng->peer.pairParam.oobDataFlag) {
        mng->local.pairMethod = SMP_PAIR_METHOD_OOB_LEGACY;
        mng->peer.pairMethod = SMP_PAIR_METHOD_OOB_LEGACY;
    } else if ((!(SMP_AUTH_REQ_BIT_MITM & mng->local.pairParam.authReq)) &&
               (!(SMP_AUTH_REQ_BIT_MITM & mng->peer.pairParam.authReq))) {
        mng->peer.pairMethod = SMP_PAIR_METHOD_JUST_WORK;
        mng->local.pairMethod = SMP_PAIR_METHOD_JUST_WORK;
    } else {
        if (mng->role == SMP_ROLE_MASTER) {
            mng->local.pairMethod =
                MASTER_LEGACY_IO_TABLE[mng->peer.pairParam.ioCapability][mng->local.pairParam.ioCapability];
            mng->peer.pairMethod =
                SLAVE_LEGACY_IO_TABLE[mng->peer.pairParam.ioCapability][mng->local.pairParam.ioCapability];
        } else {
            mng->local.pairMethod =
                SLAVE_LEGACY_IO_TABLE[mng->local.pairParam.ioCapability][mng->peer.pairParam.ioCapability];
            mng->peer.pairMethod =
                MASTER_LEGACY_IO_TABLE[mng->local.pairParam.ioCapability][mng->peer.pairParam.ioCapability];
        }
    }
}

static void SMP_CalcPairMethodScPair(SMP_PairMng *mng)
{
    if (mng->local.pairParam.oobDataFlag && mng->peer.pairParam.oobDataFlag) {
        mng->local.pairMethod = SMP_PAIR_METHOD_OOB_SC_BOTH_SIDE_SEND_RECV;
        mng->peer.pairMethod = SMP_PAIR_METHOD_OOB_SC_BOTH_SIDE_SEND_RECV;
    } else if (mng->local.pairParam.oobDataFlag && (!mng->peer.pairParam.oobDataFlag)) {
        mng->local.pairMethod = SMP_PAIR_METHOD_OOB_SC_LOCAL_RECV_PEER_SEND;
        mng->peer.pairMethod = SMP_PAIR_METHOD_OOB_SC_LOCAL_RECV_PEER_SEND;
    } else if ((!mng->local.pairParam.oobDataFlag) && mng->peer.pairParam.oobDataFlag) {
        mng->local.pairMethod = SMP_PAIR_METHOD_OOB_SC_LOCAL_SEND_PEER_RECV;
        mng->peer.pairMethod = SMP_PAIR_METHOD_OOB_SC_LOCAL_SEND_PEER_RECV;
    } else if ((!(mng->local.pairParam.authReq & SMP_AUTH_REQ_BIT_MITM)) &&
               (!(mng->peer.pairParam.authReq & SMP_AUTH_REQ_BIT_MITM))) {
        mng->local.pairMethod = SMP_PAIR_METHOD_JUST_WORK;
        mng->peer.pairMethod = SMP_PAIR_METHOD_JUST_WORK;
    } else {
        if (mng->role == SMP_ROLE_MASTER) {
            mng->local.pairMethod =
                MASTER_SCIO_TABLE[mng->peer.pairParam.ioCapability][mng->local.pairParam.ioCapability];
            mng->peer.pairMethod =
                SLAVE_SCIO_TABLE[mng->peer.pairParam.ioCapability][mng->local.pairParam.ioCapability];
        } else {
            mng->local.pairMethod =
                SLAVE_SCIO_TABLE[mng->local.pairParam.ioCapability][mng->peer.pairParam.ioCapability];
            mng->peer.pairMethod =
                MASTER_SCIO_TABLE[mng->local.pairParam.ioCapability][mng->peer.pairParam.ioCapability];
        }
    }
}

int SMP_CheckRemotePairParam(const SMP_PairParam *pairParam, uint8_t *reason)
{
    int ret = SMP_SUCCESS;

    if ((pairParam->maxEncKeySize < SMP_ENC_KEY_SIZE_MIN) || (pairParam->maxEncKeySize > SMP_ENC_KEY_SIZE_MAX)) {
        *reason = SMP_PAIR_FAILED_ENC_KEY_SIZE;
        ret = SMP_ERR_INVAL_PARAM;
        return ret;
    }

    return ret;
}

List *SMP_ListCreate(FreeDataCb cb)
{
    List *list = NULL;

    list = ListCreate(cb);

    return list;
}

void SMP_ListDelete(List *list)
{
    if (list != NULL) {
        ListDelete(list);
    }
}

void SMP_ListAddLast(List *list, void *data)
{
    if (list != NULL) {
        ListAddLast(list, data);
    }
}

bool SMP_ListRemoveNode(List *list, void *data)
{
    bool result = false;

    if (list != NULL) {
        result = ListRemoveNode(list, data);
    }

    return result;
}

ListNode *SMP_ListGetFirstNode(const List *list)
{
    ListNode *listNode = NULL;

    if (list != NULL) {
        listNode = ListGetFirstNode(list);
    }

    return listNode;
}

void *SMP_ListGetNodeData(const ListNode *node)
{
    void *data = NULL;

    if (node != NULL) {
        data = ListGetNodeData(node);
    }

    return data;
}

bool SMP_IsScOobPair(uint8_t pairMethod)
{
    bool ret = false;

    if ((pairMethod == SMP_PAIR_METHOD_OOB_SC_BOTH_SIDE_SEND_RECV) ||
        (pairMethod == SMP_PAIR_METHOD_OOB_SC_LOCAL_SEND_PEER_RECV) ||
        (pairMethod == SMP_PAIR_METHOD_OOB_SC_LOCAL_RECV_PEER_SEND)) {
        ret = true;
    }

    return ret;
}

bool SMP_IsPasskeyEntryPair(uint8_t pairMethod)
{
    bool ret = false;

    if ((pairMethod == SMP_PAIR_METHOD_PASSKEY_ENTRY) || (pairMethod == SMP_PAIR_METHOD_PASSKEY_DISPLAY)) {
        ret = true;
    }

    return ret;
}

bool SMP_IsJustworkOrNumericPair(uint8_t pairMethod)
{
    bool ret = false;

    if ((pairMethod == SMP_PAIR_METHOD_JUST_WORK) || (pairMethod == SMP_PAIR_METHOD_NUMERIC_COMPARISON)) {
        ret = true;
    }

    return ret;
}

void SMP_GetPublicKeyX(const uint8_t *pubKey, uint8_t *x, uint8_t length)
{
    for (uint8_t index = 0x00; index < length; index++) {
        x[index] = pubKey[length - 0x01 - index];
    }
}

void SMP_LongTermKeyCopy(uint8_t *destLtk, const uint8_t *sourceLtk, uint8_t encKeySize)
{
    if (destLtk != sourceLtk) {
        (void)memcpy_s(destLtk, SMP_LTK_LEN, sourceLtk, SMP_LTK_LEN);
    }

    if (encKeySize < SMP_LTK_LEN) {
        (void)memset_s(destLtk + encKeySize, (SMP_LTK_LEN - encKeySize), 0x00, (SMP_LTK_LEN - encKeySize));
    }
}

// SMP_Aes128 operates on Bluetooth little-endian AES-128 blocks: it reverses input
// bytes before calling the raw AES implementation and reverses the output back.
// h6/h7 are defined over standard (big-endian) AES-CMAC, so SMP_Aes128Be performs
// an additional full reversal around SMP_Aes128, yielding the standard big-endian
// AES-128 used by RFC 4493. Callers must supply key/material in standard big-endian
// order (as required by RFC 4493 / Bluetooth spec); the result is also returned in
// big-endian AES-CMAC output.
static int SMP_Aes128Be(const uint8_t *key, const uint8_t *in, uint8_t *out)
{
    uint8_t reverseKey[CRYPT_AESCMAC_KEY_LEN] = {0};
    uint8_t reverseIn[CRYPT_AESCMAC_IN_LEN] = {0};
    uint8_t reverseOut[CRYPT_AESCMAC_OUT_LEN] = {0};

    SMP_MemoryReverseCopy(reverseKey, key, CRYPT_AESCMAC_KEY_LEN);
    SMP_MemoryReverseCopy(reverseIn, in, CRYPT_AESCMAC_IN_LEN);

    int ret = SMP_Aes128(reverseKey, CRYPT_AESCMAC_KEY_LEN, reverseIn, CRYPT_AESCMAC_IN_LEN, reverseOut);
    if (ret == 0) {
        SMP_MemoryReverseCopy(out, reverseOut, CRYPT_AESCMAC_OUT_LEN);
    }

    // Fail the call if any key-material buffer could not be erased, so callers
    // never treat a partially-wiped secret as successfully used.
    if (memset_s(reverseKey, CRYPT_AESCMAC_KEY_LEN, 0x00, CRYPT_AESCMAC_KEY_LEN) != EOK ||
        memset_s(reverseIn, CRYPT_AESCMAC_IN_LEN, 0x00, CRYPT_AESCMAC_IN_LEN) != EOK ||
        memset_s(reverseOut, CRYPT_AESCMAC_OUT_LEN, 0x00, CRYPT_AESCMAC_OUT_LEN) != EOK) {
        LOG_ERROR("%{public}s: failed to wipe key material", __FUNCTION__);
        return BT_OPERATION_FAILED;
    }
    return ret;
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part H
// 2.2.5 AES-CMAC: RFC 4493 with a 128-bit key.

// Groups the crypto inputs shared by the CMAC helpers so that each helper
// stays within the parameter-count and nesting limits.
typedef struct {
    const uint8_t *key;
    const uint8_t *message;
    uint16_t messageLen;
    uint8_t *output;
} SmpCmacCryptoParam;

// Aggregates the intermediate CMAC buffers that are wiped before returning.
typedef struct {
    uint8_t x[CRYPT_AESCMAC_TMP_LEN];
    uint8_t finalBlock[CRYPT_AESCMAC_TMP_LEN];
    SMP_CryptAesCmacStep1Param step1Param;
    uint8_t subKeyL[CRYPT_AESCMAC_TMP_LEN];
    uint8_t zeroBlock[CRYPT_AESCMAC_TMP_LEN];
} SmpCmacState;

// RFC 4493 subkey generation: L = AES_K(0^128), then Step1 derives K1/K2
// (output1/output2). Returns BT_SUCCESS or BT_OPERATION_FAILED.
static int SmpCmacGenerateSubkeys(const uint8_t *key, const uint8_t zeroBlock[CRYPT_AESCMAC_TMP_LEN],
    uint8_t subKeyL[CRYPT_AESCMAC_TMP_LEN], SMP_CryptAesCmacStep1Param *step1Param)
{
    if (SMP_Aes128Be(key, zeroBlock, subKeyL) != 0) {
        return BT_OPERATION_FAILED;
    }

    (void)memcpy_s(step1Param->input, CRYPT_AESCMAC_IN_LEN, subKeyL, CRYPT_AESCMAC_TMP_LEN);
    SMP_CryptographicAesCmacStep1(step1Param);
    return BT_SUCCESS;
}

// RFC 4493: an empty message is treated as one final padded block (10* padding)
// and XORed with subkey K2 (output2 of step 1).
static int SmpCmacHandleEmptyMessage(const uint8_t *key, const SMP_CryptAesCmacStep1Param *step1Param,
    uint8_t finalBlock[CRYPT_AESCMAC_TMP_LEN], uint8_t output[CRYPT_AESCMAC_OUT_LEN])
{
    finalBlock[0x00] = 0x80;
    for (int j = 0; j < CRYPT_AESCMAC_TMP_LEN; j++) {
        finalBlock[j] ^= step1Param->output2[j];
    }
    if (SMP_Aes128Be(key, finalBlock, output) != 0) {
        return BT_OPERATION_FAILED;
    }
    return BT_SUCCESS;
}

// XORs each full message block into x and AES-encrypts it in place, mirroring
// the RFC 4493 loop over the first (n - 1) message blocks.
static int SmpCmacXorAndEncryptBlocks(const SmpCmacCryptoParam *param, size_t fullBlocks,
    uint8_t x[CRYPT_AESCMAC_TMP_LEN])
{
    for (size_t i = 0; i < fullBlocks; i++) {
        for (size_t j = 0; j < CRYPT_AESCMAC_TMP_LEN; j++) {
            x[j] ^= param->message[i * CRYPT_AESCMAC_TMP_LEN + j];
        }
        if (SMP_Aes128Be(param->key, x, x) != 0) {
            return BT_OPERATION_FAILED;
        }
    }
    return BT_SUCCESS;
}

// Process the message blocks: Step2 prepares the final (possibly padded) block,
// then every full block is XORed and AES-encrypted before the final block is
// combined with K2-derived state. Returns BT_SUCCESS, BT_BAD_PARAM or
// BT_OPERATION_FAILED; wipes its own step2Param copy before returning.
static int SmpCmacProcessBlocks(const SmpCmacCryptoParam *param, SmpCmacState *state)
{
    SMP_CryptAesCmacStep2Param step2Param = {0};
    int ret = BT_SUCCESS;

    // Step2 computes the final (possibly padded) message block and the expected
    // number of full blocks (n - 1). The loop below processes the full blocks
    // independently so that Step2 only has to prepare the last-block input; this
    // keeps the number of AES operations identical to a single-pass CMAC while
    // reusing the existing Step1/Step2 helpers.
    (void)memcpy_s(step2Param.input1, CRYPT_AESCMAC_IN_LEN, state->step1Param.output1, CRYPT_AESCMAC_IN_LEN);
    (void)memcpy_s(step2Param.input2, CRYPT_AESCMAC_IN_LEN, state->step1Param.output2, CRYPT_AESCMAC_IN_LEN);
    step2Param.input3 = param->message;
    step2Param.length = param->messageLen;
    SMP_CryptographicAesCmacStep2(&step2Param);

    const size_t fullBlocks = (size_t)(step2Param.n - 1);
    const size_t expectedBlocks = ((size_t)param->messageLen + CRYPT_AESCMAC_TMP_LEN - 1) / CRYPT_AESCMAC_TMP_LEN;

    if (step2Param.n < 1) {
        ret = BT_OPERATION_FAILED;
    } else if ((size_t)step2Param.n != expectedBlocks || param->messageLen < fullBlocks * CRYPT_AESCMAC_TMP_LEN) {
        ret = BT_BAD_PARAM;
    } else {
        ret = SmpCmacXorAndEncryptBlocks(param, fullBlocks, state->x);
        if (ret == BT_SUCCESS) {
            for (size_t j = 0; j < CRYPT_AESCMAC_TMP_LEN; j++) {
                state->x[j] ^= step2Param.output[j];
            }
            if (SMP_Aes128Be(param->key, state->x, param->output) != 0) {
                ret = BT_OPERATION_FAILED;
            }
        }
    }

    if (memset_s(&step2Param, sizeof(step2Param), 0x00, sizeof(step2Param)) != EOK) {
        LOG_ERROR("%{public}s: failed to wipe step2Param", __FUNCTION__);
        ret = BT_OPERATION_FAILED;
    }
    return ret;
}

// Wipe one intermediate buffer; a wipe failure is reported as an error so
// callers never treat a partially-wiped secret as successfully used.
static int SmpCmacWipeBlock(void *block, size_t len, const char *name)
{
    if (memset_s(block, len, 0x00, len) != EOK) {
        LOG_ERROR("%{public}s: failed to wipe %s", __FUNCTION__, name);
        return BT_OPERATION_FAILED;
    }
    return BT_SUCCESS;
}

// Wipe all remaining intermediate state in a fixed order. Wipe failures are
// merged into ret BEFORE deciding whether to wipe output: if a state wipe
// failed while the crypto succeeded, the function reports failure, so the
// caller must not receive the full CMAC result either.
static int SmpCmacWipeAndFinish(int ret, SmpCmacState *state, const SmpCmacCryptoParam *param)
{
    if (SmpCmacWipeBlock(state->x, sizeof(state->x), "x") != BT_SUCCESS) {
        ret = BT_OPERATION_FAILED;
    }
    if (SmpCmacWipeBlock(state->finalBlock, sizeof(state->finalBlock), "finalBlock") != BT_SUCCESS) {
        ret = BT_OPERATION_FAILED;
    }
    if (SmpCmacWipeBlock(&state->step1Param, sizeof(state->step1Param), "step1Param") != BT_SUCCESS) {
        ret = BT_OPERATION_FAILED;
    }
    if (SmpCmacWipeBlock(state->subKeyL, sizeof(state->subKeyL), "subKeyL") != BT_SUCCESS) {
        ret = BT_OPERATION_FAILED;
    }
    if (SmpCmacWipeBlock(state->zeroBlock, sizeof(state->zeroBlock), "zeroBlock") != BT_SUCCESS) {
        ret = BT_OPERATION_FAILED;
    }
    if (ret != BT_SUCCESS) {
        if (SmpCmacWipeBlock(param->output, CRYPT_AESCMAC_OUT_LEN, "output") != BT_SUCCESS) {
            ret = BT_OPERATION_FAILED;
        }
    }
    return ret;
}

int SMP_CryptographicAesCmac(const uint8_t key[CRYPT_AESCMAC_KEY_LEN], const uint8_t *message, uint16_t messageLen,
                             uint8_t output[CRYPT_AESCMAC_OUT_LEN])
{
    if (key == NULL || output == NULL || (message == NULL && messageLen != 0)) {
        return BT_BAD_PARAM;
    }

    int ret = BT_SUCCESS;
    SmpCmacCryptoParam param = {
        .key = key,
        .message = message,
        .messageLen = messageLen,
        .output = output,
    };
    SmpCmacState state = {0};

    ret = SmpCmacGenerateSubkeys(param.key, state.zeroBlock, state.subKeyL, &state.step1Param);
    if (ret == BT_SUCCESS) {
        if (messageLen == 0) {
            ret = SmpCmacHandleEmptyMessage(param.key, &state.step1Param, state.finalBlock, param.output);
        } else {
            ret = SmpCmacProcessBlocks(&param, &state);
        }
    }

    return SmpCmacWipeAndFinish(ret, &state, &param);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part H
// 2.2.10 Link key conversion function h6
int SMP_CryptographicH6(const uint8_t w[CRYPT_H6_W_LEN], const uint8_t keyId[CRYPT_H6_KEYID_LEN],
                        uint8_t output[CRYPT_H6_OUT_LEN])
{
    if (w == NULL || keyId == NULL || output == NULL) {
        return BT_BAD_PARAM;
    }

    return SMP_CryptographicAesCmac(w, keyId, CRYPT_H6_KEYID_LEN, output);
}

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part H
// 2.2.11 Link key conversion function h7
int SMP_CryptographicH7(
    const uint8_t salt[CRYPT_H7_SALT_LEN], const uint8_t w[CRYPT_H7_W_LEN], uint8_t output[CRYPT_H7_OUT_LEN])
{
    if (salt == NULL || w == NULL || output == NULL) {
        return BT_BAD_PARAM;
    }

    return SMP_CryptographicAesCmac(salt, w, CRYPT_H7_W_LEN, output);
}
