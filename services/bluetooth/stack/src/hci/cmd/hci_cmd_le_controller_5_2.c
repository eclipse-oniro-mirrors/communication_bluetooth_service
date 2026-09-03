/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
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

// LE command senders for the Bluetooth 5.1/5.2 additions: PAST sync transfer
// (7.8.87-7.8.92), Isochronous channels (7.8.97-7.8.114) and LE Power Control
// (7.8.115-7.8.122). Split out of hci_cmd_le_controller.c to keep that file
// under the source size limit.

#include "hci/hci.h"
#include "hci/hci_le_controller_5_0.h"
#include "hci/hci_le_controller_5_1.h"

#include <securec.h>

#include "btstack.h"
#include "platform/include/allocator.h"

#include "hci_cmd.h"

// Common command parameter limits reused across multiple LE commands.
#define LE_CONNECTION_HANDLE_MAX 0x0EFF
#define LE_ENABLE_MAX 0x01

// Periodic advertising (7.8.67-7.8.72) and PAST sync transfer (7.8.87-7.8.92) limits.
#define PERIODIC_ADV_HANDLE_MAX 0xEF
#define PERIODIC_ADV_SYNC_HANDLE_MAX 0x0EFF
#define PERIODIC_ADV_CREATE_SYNC_SKIP_MAX 0x01F3
#define PERIODIC_ADV_CREATE_SYNC_TIMEOUT_MIN 0x000A
#define PERIODIC_ADV_CREATE_SYNC_TIMEOUT_MAX 0x4000

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// Direction Finding / PAST command parameter limits (7.8.78-7.8.92).
// 7.8.91/7.8.92 CTE_Type is a bit mask: bit0 no AoA, bit1 no AoD 1us,
// bit2 no AoD 2us, bit4 no CTE.
#define LE_PAST_CTE_TYPES_MASK                                                                                       \
    (HCI_LE_PAST_CTE_TYPE_NO_AOA | HCI_LE_PAST_CTE_TYPE_NO_AOD_1US | HCI_LE_PAST_CTE_TYPE_NO_AOD_2US |                \
        HCI_LE_PAST_CTE_TYPE_NO_CTE)
#define LE_PAST_MODE_MAX 0x02

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.87 LE Read Antenna Information Command
int HCI_LeReadAntennaInformation(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_ANTENNA_INFORMATION, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.88 LE Set Periodic Advertising Receive Enable Command
int HCI_LeSetPeriodicAdvertisingReceiveEnable(const HciLeSetPeriodicAdvertisingReceiveEnableParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->syncHandle > PERIODIC_ADV_SYNC_HANDLE_MAX || param->enable > LE_ENABLE_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_PERIODIC_ADVERTISING_RECEIVE_ENABLE, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.89 LE Periodic Advertising Sync Transfer Command
int HCI_LePeriodicAdvertisingSyncTransfer(const HciLePeriodicAdvertisingSyncTransferParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->connectionHandle > LE_CONNECTION_HANDLE_MAX || param->syncHandle > PERIODIC_ADV_SYNC_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.90 LE Periodic Advertising Set Info Transfer Command
int HCI_LePeriodicAdvertisingSetInfoTransfer(const HciLePeriodicAdvertisingSetInfoTransferParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->connectionHandle > LE_CONNECTION_HANDLE_MAX || param->advertisingHandle > PERIODIC_ADV_HANDLE_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_PERIODIC_ADVERTISING_SET_INFO_TRANSFER, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.91 LE Set Periodic Advertising Sync Transfer Parameters Command
int HCI_LeSetPeriodicAdvertisingSyncTransferParameters(
    const HciLeSetPeriodicAdvertisingSyncTransferParametersParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->connectionHandle > LE_CONNECTION_HANDLE_MAX || param->mode > LE_PAST_MODE_MAX ||
        param->skip > PERIODIC_ADV_CREATE_SYNC_SKIP_MAX ||
        param->syncTimeout < PERIODIC_ADV_CREATE_SYNC_TIMEOUT_MIN ||
        param->syncTimeout > PERIODIC_ADV_CREATE_SYNC_TIMEOUT_MAX || (param->cteType & ~LE_PAST_CTE_TYPES_MASK) != 0) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 2, Part E
// 7.8.92 LE Set Default Periodic Advertising Sync Transfer Parameters Command
int HCI_LeSetDefaultPeriodicAdvertisingSyncTransferParameters(
    const HciLeSetDefaultPeriodicAdvertisingSyncTransferParametersParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->mode > LE_PAST_MODE_MAX || param->skip > PERIODIC_ADV_CREATE_SYNC_SKIP_MAX ||
        param->syncTimeout < PERIODIC_ADV_CREATE_SYNC_TIMEOUT_MIN ||
        param->syncTimeout > PERIODIC_ADV_CREATE_SYNC_TIMEOUT_MAX || (param->cteType & ~LE_PAST_CTE_TYPES_MASK) != 0) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd =
        HciAllocCmd(HCI_LE_SET_DEFAULT_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.72 LE Read Buffer Size V2 Command
int HCI_LeReadBufferSizeV2(void)
{
    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_BUFFER_SIZE_V2, NULL, 0);
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.96 LE Read ISO TX Sync Command
int HCI_LeReadIsoTxSync(const HciLeReadIsoTxSyncParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_ISO_TX_SYNC, (void *)param, sizeof(HciLeReadIsoTxSyncParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.97 LE Set CIG Parameters Command
int HCI_LeSetCigParameters(const HciLeSetCigParametersParam *param)
{
    if (param == NULL || param->cisCount > HCI_LE_CIS_COUNT_MAX ||
        (param->cisCount > 0 && param->cisConfig == NULL)) {
        return BT_BAD_PARAM;
    }

    const size_t length = sizeof(uint8_t) + sizeof(uint8_t[3]) + sizeof(uint8_t[3]) + sizeof(uint8_t) +
        sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t[2]) + sizeof(uint8_t[2]) + sizeof(uint8_t) +
        param->cisCount * sizeof(HciLeCisConfigParam);
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    uint16_t index = 0;
    buf[index] = param->cigId;
    index += sizeof(uint8_t);
    (void)memcpy_s(buf + index, length - index, param->sduIntervalMToS, sizeof(param->sduIntervalMToS));
    index += sizeof(param->sduIntervalMToS);
    (void)memcpy_s(buf + index, length - index, param->sduIntervalSToM, sizeof(param->sduIntervalSToM));
    index += sizeof(param->sduIntervalSToM);
    buf[index] = param->slaveClockAccuracy;
    index += sizeof(uint8_t);
    buf[index] = param->packing;
    index += sizeof(uint8_t);
    buf[index] = param->framing;
    index += sizeof(uint8_t);
    (void)memcpy_s(buf + index, length - index, param->maxTransportLatencyMToS, sizeof(param->maxTransportLatencyMToS));
    index += sizeof(param->maxTransportLatencyMToS);
    (void)memcpy_s(buf + index, length - index, param->maxTransportLatencySToM, sizeof(param->maxTransportLatencySToM));
    index += sizeof(param->maxTransportLatencySToM);
    buf[index] = param->cisCount;
    index += sizeof(uint8_t);
    if (param->cisCount > 0) {
        (void)memcpy_s(buf + index, length - index, param->cisConfig, param->cisCount * sizeof(HciLeCisConfigParam));
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_CIG_PARAMETERS, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }
    int result = HciSendCmd(cmd);
    MEM_MALLOC.free(buf);
    return result;
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.98 LE Set CIG Parameters Test Command
int HCI_LeSetCigParametersTest(const HciLeSetCigParametersTestParam *param)
{
    if (param == NULL || param->cisCount > HCI_LE_CIS_COUNT_MAX ||
        (param->cisCount > 0 && param->cisConfig == NULL)) {
        return BT_BAD_PARAM;
    }

    const size_t length = sizeof(uint8_t) + sizeof(uint8_t[3]) + sizeof(uint8_t[3]) + sizeof(uint8_t) +
        sizeof(uint8_t) + sizeof(param->isoInterval) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) +
        sizeof(uint8_t) + param->cisCount * sizeof(HciLeSetCigParametersTestCisConfig);
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    uint16_t index = 0;
    buf[index] = param->cigId;
    index += sizeof(uint8_t);
    (void)memcpy_s(buf + index, length - index, param->sduIntervalMToS, sizeof(param->sduIntervalMToS));
    index += sizeof(param->sduIntervalMToS);
    (void)memcpy_s(buf + index, length - index, param->sduIntervalSToM, sizeof(param->sduIntervalSToM));
    index += sizeof(param->sduIntervalSToM);
    buf[index] = param->ftMToS;
    index += sizeof(uint8_t);
    buf[index] = param->ftSToM;
    index += sizeof(uint8_t);
    (void)memcpy_s(buf + index, length - index, param->isoInterval, sizeof(param->isoInterval));
    index += sizeof(param->isoInterval);
    buf[index] = param->slaveClockAccuracy;
    index += sizeof(uint8_t);
    buf[index] = param->packing;
    index += sizeof(uint8_t);
    buf[index] = param->framing;
    index += sizeof(uint8_t);
    buf[index] = param->cisCount;
    index += sizeof(uint8_t);
    if (param->cisCount > 0) {
        (void)memcpy_s(buf + index, length - index, param->cisConfig,
            param->cisCount * sizeof(HciLeSetCigParametersTestCisConfig));
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_CIG_PARAMS_TEST, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }
    int result = HciSendCmd(cmd);
    MEM_MALLOC.free(buf);
    return result;
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.99 LE Create CIS Command
int HCI_LeCreateCis(const HciLeCreateCisParam *param)
{
    if (param == NULL || param->cisCount > HCI_LE_CIS_COUNT_MAX ||
        (param->cisCount > 0 && param->cisConfig == NULL)) {
        return BT_BAD_PARAM;
    }

    const size_t length = sizeof(uint8_t) + param->cisCount * sizeof(HciLeCreateCisConfigParam);
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    uint16_t index = 0;
    buf[index] = param->cisCount;
    index += sizeof(uint8_t);
    if (param->cisCount > 0) {
        (void)memcpy_s(
            buf + index, length - index, param->cisConfig, param->cisCount * sizeof(HciLeCreateCisConfigParam));
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_CREATE_CIS, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }
    int result = HciSendCmd(cmd);
    MEM_MALLOC.free(buf);
    return result;
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.100 LE Remove CIG Command
int HCI_LeRemoveCig(const HciLeRemoveCigParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_REMOVE_CIG, (void *)param, sizeof(HciLeRemoveCigParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.101 LE Accept CIS Request Command
int HCI_LeAcceptCisRequest(const HciLeAcceptCisRequestParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_ACCEPT_CIS_REQUEST, (void *)param, sizeof(HciLeAcceptCisRequestParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.102 LE Reject CIS Request Command
int HCI_LeRejectCisRequest(const HciLeRejectCisRequestParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_REJECT_CIS_REQUEST, (void *)param, sizeof(HciLeRejectCisRequestParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.103 LE Create BIG Command
int HCI_LeCreateBig(const HciLeCreateBigParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_CREATE_BIG, (void *)param, sizeof(HciLeCreateBigParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.104 LE Create BIG Test Command
int HCI_LeCreateBigTest(const HciLeCreateBigTestParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_CREATE_BIG_TEST, (void *)param, sizeof(HciLeCreateBigTestParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.105 LE Terminate BIG Command
int HCI_LeTerminateBig(const HciLeTerminateBigParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_TERMINATE_BIG, (void *)param, sizeof(HciLeTerminateBigParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.106 LE BIG Create Sync Command
int HCI_LeBigCreateSync(const HciLeBigCreateSyncParam *param)
{
    // Vol 4 Part E 7.8.106: Num_BIS 0x00 requests synchronization to all BISes of the
    // BIG, so the bis array is then empty (may be NULL) and no BIS handle bytes follow.
    if (param == NULL || param->numBis > HCI_LE_BIS_COUNT_MAX ||
        (param->numBis > 0 && param->bis == NULL)) {
        return BT_BAD_PARAM;
    }

    // Each BIS_Handle is a 16-bit field on the wire (Vol 4 Part E 7.8.106), so the bis array
    // carries sizeof(uint16_t) bytes per entry, mirroring the LE BIG Sync Established event
    // parsing (hci_evt_le.c, sizeof(uint16_t) * bisCount).
    const size_t length = sizeof(param->bigHandle) + sizeof(param->syncHandle) + sizeof(param->encryption) +
        sizeof(param->broadcastCode) + sizeof(param->mse) + sizeof(param->bigSyncTimeout) + sizeof(param->numBis) +
        sizeof(uint16_t) * param->numBis;

    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    uint16_t index = 0;
    buf[index] = param->bigHandle;
    index += sizeof(uint8_t);
    (void)memcpy_s(buf + index, sizeof(uint16_t), &param->syncHandle, sizeof(uint16_t));
    index += sizeof(uint16_t);
    buf[index] = param->encryption;
    index += sizeof(uint8_t);
    // broadcast code is a fixed 128-bit field, 7.8.106
    (void)memcpy_s(buf + index, sizeof(param->broadcastCode), param->broadcastCode,
        sizeof(param->broadcastCode));
    index += sizeof(param->broadcastCode);
    buf[index] = param->mse;
    index += sizeof(uint8_t);
    (void)memcpy_s(buf + index, sizeof(uint16_t), param->bigSyncTimeout, sizeof(uint16_t));
    index += sizeof(uint16_t);
    buf[index] = param->numBis;
    index += sizeof(uint8_t);

    if (param->numBis > 0) {
        (void)memcpy_s(buf + index, length - index, param->bis, sizeof(uint16_t) * param->numBis);
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_BIG_CREATE_SYNC, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }

    int result = HciSendCmd(cmd);
    MEM_MALLOC.free(buf);
    return result;
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.107 LE BIG Terminate Sync Command
int HCI_LeBigTerminateSync(const HciLeBigTerminateSyncParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_BIG_TERMINATE_SYNC, (void *)param, sizeof(HciLeBigTerminateSyncParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.108 LE Request Peer SCA Command
int HCI_LeRequestPeerSca(const HciLeRequestPeerScaParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_REQUEST_PEER_SCA, (void *)param, sizeof(HciLeRequestPeerScaParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.109 LE Setup ISO Data Path Command
int HCI_LeSetupIsoDataPath(const HciLeSetupIsoDataPathParam *param)
{
    // 7.8.109: the fixed fields occupy 13 octets and the command payload length is
    // a single octet (max 255), so the codec configuration can carry at most 242
    // octets; a larger length would truncate the payload into a malformed command.
    const size_t fixedLength = sizeof(param->connectionHandle) + sizeof(param->dataPathDirection) +
        sizeof(param->dataPathId) + sizeof(param->codecId) + sizeof(param->controllerDelay) +
        sizeof(param->codecConfigurationLength);
    if (param == NULL || param->codecConfigurationLength > (UINT8_MAX - fixedLength) ||
        (param->codecConfigurationLength > 0 && param->codecConfiguration == NULL)) {
        return BT_BAD_PARAM;
    }

    const size_t length = sizeof(param->connectionHandle) + sizeof(param->dataPathDirection) +
        sizeof(param->dataPathId) + sizeof(param->codecId) + sizeof(param->controllerDelay) +
        sizeof(param->codecConfigurationLength) + param->codecConfigurationLength;
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    uint16_t index = 0;
    (void)memcpy_s(
        buf + index, sizeof(param->connectionHandle), &param->connectionHandle, sizeof(param->connectionHandle));
    index += sizeof(param->connectionHandle);
    buf[index] = param->dataPathDirection;
    index += sizeof(param->dataPathDirection);
    buf[index] = param->dataPathId;
    index += sizeof(param->dataPathId);
    (void)memcpy_s(buf + index, sizeof(param->codecId), param->codecId, sizeof(param->codecId));
    index += sizeof(param->codecId);
    (void)memcpy_s(buf + index, sizeof(param->controllerDelay), param->controllerDelay, sizeof(param->controllerDelay));
    index += sizeof(param->controllerDelay);
    buf[index] = param->codecConfigurationLength;
    index += sizeof(param->codecConfigurationLength);

    if (param->codecConfigurationLength > 0) {
        (void)memcpy_s(buf + index, length - index, param->codecConfiguration, param->codecConfigurationLength);
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SETUP_ISO_DATA_PATH, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }

    int result = HciSendCmd(cmd);
    MEM_MALLOC.free(buf);
    return result;
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.110 LE Remove ISO Data Path Command
int HCI_LeRemoveIsoDataPath(const HciLeRemoveIsoDataPathParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_REMOVE_ISO_DATA_PATH, (void *)param, sizeof(HciLeRemoveIsoDataPathParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.111 LE ISO Transmit Test Command
int HCI_LeIsoTransmitTest(const HciLeIsoTransmitTestParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_ISO_TRANSMIT_TEST, (void *)param, sizeof(HciLeIsoTransmitTestParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.112 LE ISO Receive Test Command
int HCI_LeIsoReceiveTest(const HciLeIsoReceiveTestParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_ISO_RECEIVE_TEST, (void *)param, sizeof(HciLeIsoReceiveTestParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.113 LE ISO Read Test Counters Command
int HCI_LeIsoReadTestCounters(const HciLeIsoReadTestCountersParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_ISO_READ_TEST_COUNTERS, (void *)param, sizeof(HciLeIsoReadTestCountersParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.114 LE ISO Test End Command
int HCI_LeIsoTestEnd(const HciLeIsoTestEndParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_ISO_TEST_END, (void *)param, sizeof(HciLeIsoTestEndParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.115 LE Set Host Feature Command
int HCI_LeSetHostFeature(const HciLeSetHostFeatureParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_HOST_FEATURE, (void *)param, sizeof(HciLeSetHostFeatureParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.116 LE Read ISO Link Quality Command
int HCI_LeReadIsoLinkQuality(const HciLeReadIsoLinkQualityParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_READ_ISO_LINK_QUALITY, (void *)param, sizeof(HciLeReadIsoLinkQualityParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.117 LE Enhanced Read Transmit Power Level Command
int HCI_LeEnhancedReadTransmitPowerLevel(const HciLeEnhancedReadTransmitPowerLevelParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(
        HCI_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL, (void *)param, sizeof(HciLeEnhancedReadTransmitPowerLevelParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.118 LE Read Remote Transmit Power Level Command
int HCI_LeReadRemoteTransmitPowerLevel(const HciLeReadRemoteTransmitPowerLevelParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(
        HCI_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL, (void *)param, sizeof(HciLeReadRemoteTransmitPowerLevelParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.119 LE Set Path Loss Reporting Parameters Command
int HCI_LeSetPathLossReportingParameters(const HciLeSetPathLossReportingParametersParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(
        HCI_LE_SET_PATH_LOSS_REPORTING_PARAMETERS, (void *)param, sizeof(HciLeSetPathLossReportingParametersParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.120 LE Set Path Loss Reporting Enable Command
int HCI_LeSetPathLossReportingEnable(const HciLeSetPathLossReportingEnableParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd =
        HciAllocCmd(HCI_LE_SET_PATH_LOSS_REPORTING_ENABLE, (void *)param, sizeof(HciLeSetPathLossReportingEnableParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.2 | Vol 2, Part E
// 7.8.121 LE Set Transmit Power Reporting Enable Command
int HCI_LeSetTransmitPowerReportingEnable(const HciLeSetTransmitPowerReportingEnableParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(
        HCI_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE, (void *)param, sizeof(HciLeSetTransmitPowerReportingEnableParam));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}
