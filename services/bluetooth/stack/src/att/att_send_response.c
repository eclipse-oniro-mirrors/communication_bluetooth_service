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

/**
 * @file att_send.c
 *
 * @brief implement send function to be called.
 *
 */

#include "att_common.h"
#include "att_receive.h"

#include <memory.h>

#include "buffer.h"
#include "list.h"
#include "packet.h"

#include "log.h"

#include "gap_if.h"
#include "gap_le_if.h"

#include "platform/include/allocator.h"

static AttServerSendDataCallback g_attServerSendDataCB;

/* Resolve the connection a response targets. cid 0 keeps the legacy lookup: prefer the
 * UATT bearer — the Exchange MTU request only arrives on the fixed channel, and plain
 * first-match could hit an EATT slot on a multi-bearer connection — then fall back to
 * first-match for legacy callers on non-LE links; a non-zero cid pins the LE bearer so
 * the response returns on the request's source bearer (Vol 3 Part F 3.3.3). */
static AttConnectInfo *AttResponseResolveConnect(uint16_t connectHandle, uint16_t cid)
{
    AttConnectInfo *connect = NULL;
    uint16_t index = 0;

    if (cid == 0) {
        connect = AttGetConnectInfoByConnectHandleAndLeCid(connectHandle, LE_CID);
        if (connect == NULL) {
            AttGetConnectInfoIndexByConnectHandle(connectHandle, &index, &connect);
        }
    } else {
        connect = AttGetConnectInfoByConnectHandleAndLeCid(connectHandle, cid);
    }
    return connect;
}

void ATT_ErrorResponseCid(uint16_t connectHandle, uint16_t cid, const AttError *attErrorPtr);
void ATT_FindInformationResponseCid(
    uint16_t connectHandle, uint16_t cid, uint8_t format, AttHandleUuid *handleUUIDPairs, uint16_t pairNum);
void ATT_FindByTypeValueResponseCid(
    uint16_t connectHandle, uint16_t cid, const AttHandleInfo *handleInfoList, uint16_t listNum);
void ATT_ReadByTypeResponseCid(uint16_t connectHandle, uint16_t cid, uint8_t length,
    const AttReadByTypeRspDataList *valueList, uint16_t attrValueNum);
void ATT_ReadResponseCid(uint16_t connectHandle, uint16_t cid, const Buffer *attValue);
void ATT_ReadBlobResponseCid(uint16_t connectHandle, uint16_t cid, const Buffer *attReadBlobResObj);
void ATT_ReadMultipleResponseCid(uint16_t connectHandle, uint16_t cid, const Buffer *valueList);
void ATT_ReadMultipleVariableResponseCid(uint16_t connectHandle, uint16_t cid, const Buffer *valueList);
void ATT_ReadByGroupTypeResponseCid(uint16_t connectHandle, uint16_t cid, uint8_t length,
    const AttReadGoupAttributeData *serviceList, uint16_t serviceNum);
void ATT_WriteResponseCid(uint16_t connectHandle, uint16_t cid);
void ATT_PrepareWriteResponseCid(
    uint16_t connectHandle, uint16_t cid, AttReadBlobReqPrepareWriteValue attReadBlobObj, const Buffer *attValue);
void ATT_ExecuteWriteResponseCid(uint16_t connectHandle, uint16_t cid);

static void AttAssignMTU(uint16_t *mtu, AttConnectInfo *connect);
static void AttFindInformationResDataAssign(
    uint8_t format, uint16_t *inforDataLenPtr, uint16_t *dataLenPtr, uint16_t pairNum);

static void AttErrorResponseAsync(const void *context);
static void AttErrorResponseAsyncDestroy(const void *context);
static void AttExchangeMTUResponseAsync(const void *context);
static void AttExchangeMTUResponseAsyncDestroy(const void *context);
static void AttFindInformationResponseAsync(const void *context);
static void AttFindInformationResponseAsyncDestroy(const void *context);
static void AttFindByTypeValueResponseAsync(const void *context);
static void AttFindByTypeValueResponseAsyncDestroy(const void *context);
static void AttReadByTypeResponseAsync(const void *context);
static void AttReadByTypeResponseAsyncDestroy(const void *context);
static void AttReadResponseAsync(const void *context);
static void AttReadResponseAsyncDestroy(const void *context);
static void AttReadBlobResponseAsync(const void *context);
static void AttReadBlobResponseAsyncDestroy(const void *context);
static void AttReadMultipleResponseAsync(const void *context);
static void AttReadMultipleResponseAsyncDestroy(const void *context);
static void AttReadByGroupTypeResponseAsync(const void *context);
static void AttReadByGroupTypeResponseAsyncDestroy(const void *context);
static void AttWriteResponseAsync(const void *context);
static void AttWriteResponseAsyncDestroy(const void *context);
static void AttPrepareWriteResponseAsync(const void *context);
static void AttPrepareWriteResponseAsyncDestroy(const void *context);
static void AttExecuteWriteResponseAsync(const void *context);
static void AttExecuteWriteResponseAsyncDestroy(const void *context);
static void AttHandleValueNotificationAsync(const void *context);
static void AttHandleValueNotificationAsyncDestroy(const void *context);
static void AttHandleValueIndicationAsync(const void *context);
static void AttHandleValueIndicationAsyncDestroy(const void *context);

static void AttReadByTypeResponseFree(ReadByTypeResponseAsync *readByTypeResAsyncPtr);
static void AttFindInformationResponsePacketDataAssign(
    uint8_t *data, FindInformationResponseAsync *findInforPtr, uint16_t dataLen);
static void AttReadByGroupTypeResponseAsyncFree(
    ReadByGroupTypeResponseAsync *attReadByGroupResponseAsyncPtr, uint16_t num);

static void AttServerSendDataRegisterAsync(const void *context);
static void AttServerSendDataRegisterAsyncDestroy(const void *context);
static void AttServerSendDataDeRegisterAsync(const void *context);
static void AttServerSendDataDeRegisterAsyncDestroy(const void *context);

/**
 * @brief Assign to mtu.
 *
 * @param1 mtu Indicates the pointer to mtu.
 * @param2 connect Indicates the pointer to AttConnectInfo.
 */
void AttAssignMTU(uint16_t *mtu, AttConnectInfo *connect)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL", __FUNCTION__);
        return;
    }

    if (connect->mtuFlag) {
        connect->mtuFlag = false;
        if (connect->transportType == BT_TRANSPORT_BR_EDR) {
            *mtu = DEFAULTBREDRMTU;
        }
        if (connect->transportType == BT_TRANSPORT_LE) {
            *mtu = DEFAULTLEATTMTU;
        }
    } else {
        *mtu = connect->mtu;
    }

    return;
}

/**
 * @brief error response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttErrorResponseAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    int ret;
    uint8_t *data = NULL;
    Packet *packet = NULL;
    AttConnectInfo *connect = NULL;
    ErrorResponseAsync *errorResAsyncPtr = NULL;

    errorResAsyncPtr = (ErrorResponseAsync *)context;

    connect = AttResponseResolveConnect(errorResAsyncPtr->connectHandle, errorResAsyncPtr->cid);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL, and goto ATT_ERRORRESPONSE_END", __FUNCTION__);
        goto ATT_ERRORRESPONSE_END;
    }

    packet = PacketMalloc(0, 0, sizeof(uint8_t) + STEP_FOUR);
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    data = BufferPtr(PacketContinuousPayload(packet));
    data[0] = ERROR_RESPONSE;
    data[1] = errorResAsyncPtr->ATTErrorPtr->reqOpcode;
    ((uint16_t *)(data + STEP_TWO))[0] = errorResAsyncPtr->ATTErrorPtr->attHandleInError;
    data[STEP_FOUR] = errorResAsyncPtr->ATTErrorPtr->errorCode;

    ret = AttResponseSendData(connect, packet);
    ServerCallbackReturnValue(ret, connect);
    PacketFree(packet);

ATT_ERRORRESPONSE_END:
    MEM_MALLOC.free(errorResAsyncPtr->ATTErrorPtr);
    MEM_MALLOC.free(errorResAsyncPtr);
    return;
}

/**
 * @brief destroy error response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttErrorResponseAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    ErrorResponseAsync *errorResAsyncPtr = (ErrorResponseAsync *)context;

    MEM_MALLOC.free(errorResAsyncPtr->ATTErrorPtr);
    MEM_MALLOC.free(errorResAsyncPtr);

    return;
}

/**
 * @brief gatt send error response to att.
 *
 * @param1 connectHandle Indicates the connect handle.
 * @param2 attErrorPtr Indicates the pointer to const error response parameter.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
void ATT_ErrorResponse(uint16_t connectHandle, const AttError *attErrorPtr)
{
    ATT_ErrorResponseCid(connectHandle, 0, attErrorPtr);
    return;
}

/**
 * @brief exchange mtu response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttExchangeMTUResponseAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    int ret;
    ExchangeMTUAsync *exchangeMtuResPtr = (ExchangeMTUAsync *)context;
    AttConnectInfo *connect = NULL;
    Packet *packet = NULL;
    uint8_t *data = NULL;

    // cid 0 keeps the UATT-first lookup (the Exchange MTU request only arrives on the fixed
    // channel), the same semantics every other response path uses - plain first-match could
    // resolve an EATT slot on a multi-bearer connection and answer on the wrong bearer.
    connect = AttResponseResolveConnect(exchangeMtuResPtr->connectHandle, 0);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATTEXCHANGEMTURESPONSE_END", __FUNCTION__);
        goto ATTEXCHANGEMTURESPONSE_END;
    }

    packet = PacketMalloc(0, 0, sizeof(uint8_t) + sizeof(exchangeMtuResPtr->mtu));
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    data = BufferPtr(PacketContinuousPayload(packet));
    data[0] = EXCHANGE_MTU_RESPONSE;
    ((uint16_t *)(data + 1))[0] = exchangeMtuResPtr->mtu;
    connect->mtu = Min(connect->receiveMtu, exchangeMtuResPtr->mtu);

    ret = AttResponseSendData(connect, packet);
    ServerCallbackReturnValue(ret, connect);

    PacketFree(packet);

ATTEXCHANGEMTURESPONSE_END:
    MEM_MALLOC.free(exchangeMtuResPtr);
    return;
}

/**
 * @brief destroy exchange mtu response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttExchangeMTUResponseAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    ExchangeMTUAsync *exchangeMtuResPtr = (ExchangeMTUAsync *)context;

    MEM_MALLOC.free(exchangeMtuResPtr);

    return;
}

/**
 * @brief gatt send exchangeMTU response to att.
 *
 * @param1 connectHandle Indicates the connect handle.
 * @param2 serverRxMTU Indicates the attribute server receive MTU size.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
void ATT_ExchangeMTUResponse(uint16_t connectHandle, uint16_t serverRxMTU)
{
    LOG_INFO("%{public}s enter, connectHandle = %hu, serverRxMTU = %hu", __FUNCTION__, connectHandle, serverRxMTU);

    ExchangeMTUAsync *exchangeMtuResPtr = MEM_MALLOC.alloc(sizeof(ExchangeMTUAsync));
    if (exchangeMtuResPtr == NULL) {
        // The send callback is registered for invocation on the ATT processing queue; calling it
        // directly from this caller thread would violate that contract (same reasoning as the
        // failure path of ATT_ErrorResponseCid). Report the transient OOM only in the log: the
        // client's request stays pending and is eventually settled by its own transaction timeout.
        LOG_ERROR("point to NULL");
        return;
    }
    exchangeMtuResPtr->connectHandle = connectHandle;
    exchangeMtuResPtr->mtu = serverRxMTU;
    AttAsyncProcess(AttExchangeMTUResponseAsync, AttExchangeMTUResponseAsyncDestroy, exchangeMtuResPtr);

    return;
}

/**
 * @brief find information response data assign.
 *
 * @param1 inforDataLenPtr Indicates the pointer of uint16_t.
 * @param2 dataLenPtr Indicates the pointer of uint16_t.
 */
void AttFindInformationResDataAssign(uint8_t format, uint16_t *inforDataLenPtr, uint16_t *dataLenPtr, uint16_t pairNum)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    if (format == HANDLEAND16BITBLUETOOTHUUID) {
        *inforDataLenPtr = FINDINFORRESINFOR16BITLEN * pairNum;
        *dataLenPtr = FINDINFORRESINFOR16BITLEN;
    } else if (format == HANDLEAND128BITUUID) {
        *inforDataLenPtr = FINDINFORRESINFOR128BITLEN * pairNum;
        *dataLenPtr = FINDINFORRESINFOR128BITLEN;
    }

    return;
}

void AttFindInformationResponsePacketDataAssign(
    uint8_t *data, FindInformationResponseAsync *findInforPtr, uint16_t dataLen)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    uint16_t index;

    data[0] = FIND_INFORMATION_RESPONSE;
    data[1] = findInforPtr->attFindInformationResContext.format;
    data += STEP_TWO;
    for (index = 0; index < findInforPtr->attFindInformationResContext.pairNum; ++index) {
        ((uint16_t *)(data))[0] = findInforPtr->attFindInformationResContext.handleUuidPairs[index].attHandle;
        if (findInforPtr->attFindInformationResContext.format == HANDLEAND16BITBLUETOOTHUUID) {
            ((uint16_t *)(data + STEP_TWO))[0] =
                findInforPtr->attFindInformationResContext.handleUuidPairs[index].uuid.uuid16;
        } else if (findInforPtr->attFindInformationResContext.format == HANDLEAND128BITUUID) {
            if (memcpy_s(data + STEP_TWO,
                UUID128BITTYPELEN,
                findInforPtr->attFindInformationResContext.handleUuidPairs[index].uuid.uuid128,
                UUID128BITTYPELEN) != EOK) {
                    LOG_ERROR("%{public}s memcpy_s fail", __FUNCTION__);
                    return;
                }
        }
        data += dataLen;
    }

    return;
}

/**
 * @brief find information response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttFindInformationResponseAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    int ret;
    FindInformationResponseAsync *findInforPtr = (FindInformationResponseAsync *)context;
    Packet *packet = NULL;
    uint8_t *data = NULL;
    uint16_t mtu = 0;
    uint16_t inforDataLen = 0;
    AttConnectInfo *connect = NULL;
    uint16_t dataLen = 0;

    connect = AttResponseResolveConnect(findInforPtr->connectHandle, findInforPtr->cid);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATTFINDINFORMATIONRESPONSE_END", __FUNCTION__);
        goto ATTFINDINFORMATIONRESPONSE_END;
    }
    AttAssignMTU(&mtu, connect);
    AttFindInformationResDataAssign(findInforPtr->attFindInformationResContext.format,
        &inforDataLen,
        &dataLen,
        findInforPtr->attFindInformationResContext.pairNum);

    if (inforDataLen > (mtu - STEP_TWO)) {
        LOG_INFO("%{public}s inforDataLen > (mtu - 2)", __FUNCTION__);
        ServerCallbackBTBADPARAM(connect);
        goto ATTFINDINFORMATIONRESPONSE_END;
    }

    packet = PacketMalloc(0, 0, sizeof(uint8_t) + sizeof(uint8_t) + inforDataLen);
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    data = BufferPtr(PacketContinuousPayload(packet));
    AttFindInformationResponsePacketDataAssign(data, findInforPtr, dataLen);

    ret = AttResponseSendData(connect, packet);
    ServerCallbackReturnValue(ret, connect);
    PacketFree(packet);

ATTFINDINFORMATIONRESPONSE_END:
    MEM_MALLOC.free(findInforPtr->attFindInformationResContext.handleUuidPairs);
    MEM_MALLOC.free(findInforPtr);
    return;
}

/**
 * @brief find information response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttFindInformationResponseAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    FindInformationResponseAsync *findInforPtr = (FindInformationResponseAsync *)context;

    MEM_MALLOC.free(findInforPtr->attFindInformationResContext.handleUuidPairs);
    MEM_MALLOC.free(findInforPtr);

    return;
}

/**
 * @brief gatt send findInformation response to att.
 *
 * @param1 connectHandle Indicates the connect handle.
 * @param2 format Indicates the format of the information data.
 * @param3 handleUUIDPairs Indicates the pointer to const information data whose format is determined by the Format
 * field.
 * @param4 pairNum Indicates the paris number of the Information Data.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
void ATT_FindInformationResponse(
    uint16_t connectHandle, uint8_t format, AttHandleUuid *handleUUIDPairs, uint16_t pairNum)
{
    ATT_FindInformationResponseCid(connectHandle, 0, format, handleUUIDPairs, pairNum);
    return;
}

/**
 * @brief find by type value response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttFindByTypeValueResponseAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    uint16_t mtu = 0;
    AttConnectInfo *connect = NULL;
    FindByTypeValueResponseAsync *findByTypeResAsyncPtr = (FindByTypeValueResponseAsync *)context;
    uint16_t handleInfoListLen = (findByTypeResAsyncPtr->attFindByTypeResContext.listNum) * sizeof(AttHandleInfo);
    int ret;

    connect = AttResponseResolveConnect(findByTypeResAsyncPtr->connectHandle, findByTypeResAsyncPtr->cid);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATTFINDBYTYPEVALUERESPONSE_END", __FUNCTION__);
        goto ATTFINDBYTYPEVALUERESPONSE_END;
    }

    AttAssignMTU(&mtu, connect);

    if (handleInfoListLen > (mtu - 1)) {
        ServerCallbackBTBADPARAM(connect);
        goto ATTFINDBYTYPEVALUERESPONSE_END;
    }

    uint16_t handlesInforLen = sizeof(AttHandleInfo) * (findByTypeResAsyncPtr->attFindByTypeResContext.listNum);
    Packet *packet = PacketMalloc(0, 0, sizeof(uint8_t) + handlesInforLen);
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    uint8_t *data = BufferPtr(PacketContinuousPayload(packet));
    data[0] = FIND_BY_TYPE_VALUE_RESPONSE;
    (void)memcpy_s(
        data + 1, handleInfoListLen, findByTypeResAsyncPtr->attFindByTypeResContext.handleInfoList, handleInfoListLen);

    ret = AttResponseSendData(connect, packet);
    ServerCallbackReturnValue(ret, connect);
    PacketFree(packet);

ATTFINDBYTYPEVALUERESPONSE_END:
    MEM_MALLOC.free(findByTypeResAsyncPtr->attFindByTypeResContext.handleInfoList);
    MEM_MALLOC.free(findByTypeResAsyncPtr);
    return;
}

/**
 * @brief destroy find by type value response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttFindByTypeValueResponseAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    FindByTypeValueResponseAsync *findByTypeResAsyncDesPtr = (FindByTypeValueResponseAsync *)context;

    MEM_MALLOC.free(findByTypeResAsyncDesPtr->attFindByTypeResContext.handleInfoList);
    MEM_MALLOC.free(findByTypeResAsyncDesPtr);

    return;
}

/**
 * @brief gatt send findbytypevalue response to att.
 *
 * @param1 connectHandle Indicates the connect handle.
 * @param2 handleInfoList Indicates the pointer to const a list of 1 or more Handle Informations.
 * @param3 listNum Indicates the number of handles information list.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
void ATT_FindByTypeValueResponse(uint16_t connectHandle, const AttHandleInfo *handleInfoList, uint16_t listNum)
{
    ATT_FindByTypeValueResponseCid(connectHandle, 0, handleInfoList, listNum);
    return;
}

static void AttReadByTypeResponseFree(ReadByTypeResponseAsync *readByTypeResAsyncPtr)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    uint16_t index = 0;

    for (; index < readByTypeResAsyncPtr->attReadByTypeRspContext.valueNum; ++index) {
        MEM_MALLOC.free(readByTypeResAsyncPtr->attReadByTypeRspContext.valueList[index].attributeValue);
    }
    MEM_MALLOC.free(readByTypeResAsyncPtr->attReadByTypeRspContext.valueList);
    MEM_MALLOC.free(readByTypeResAsyncPtr);

    return;
}

/**
 * @brief read by type response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttReadByTypeResponseAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    uint16_t index = 0;
    uint16_t mtu = 0;

    ReadByTypeResponseAsync *readByTypeResAsyncPtr = (ReadByTypeResponseAsync *)context;
    Buffer *bufferPtr = NULL;
    AttConnectInfo *connect = NULL;

    uint8_t len = readByTypeResAsyncPtr->attReadByTypeRspContext.len;
    uint16_t num = readByTypeResAsyncPtr->attReadByTypeRspContext.valueNum;
    connect = AttResponseResolveConnect(readByTypeResAsyncPtr->connectHandle, readByTypeResAsyncPtr->cid);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATT_READBYTYPERESPONSE_END", __FUNCTION__);
        goto ATT_READBYTYPERESPONSE_END;
    }

    AttAssignMTU(&mtu, connect);

    if ((len * num) > (mtu - STEP_TWO)) {
        ServerCallbackBTBADPARAM(connect);
        goto ATT_READBYTYPERESPONSE_END;
    }
    if (((len - STEP_TWO) > (mtu - STEP_FOUR)) || ((len - STEP_TWO) > MAXREADBYTYPERESLEN)) {
        len = Min(mtu - STEP_FOUR, MAXREADBYTYPERESLEN);
    }

    Packet *packet = PacketMalloc(0, 0, sizeof(uint8_t) + sizeof(len));
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    *(uint8_t *)BufferPtr(PacketContinuousPayload(packet)) = READ_BY_TYPE_RESPONSE;
    *((uint8_t *)BufferPtr(PacketContinuousPayload(packet)) + 1) = len;

    for (index = 0; index < num; index++) {
        bufferPtr = BufferMalloc(len);
        (void)memcpy_s(BufferPtr(bufferPtr),
            BufferGetSize(bufferPtr),
            &(readByTypeResAsyncPtr->attReadByTypeRspContext.valueList[index]),
            STEP_TWO);
        (void)memcpy_s((uint8_t *)BufferPtr(bufferPtr) + STEP_TWO,
            BufferGetSize(bufferPtr) - STEP_TWO,
            readByTypeResAsyncPtr->attReadByTypeRspContext.valueList[index].attributeValue,
            len - STEP_TWO);
        PacketPayloadAddLast(packet, bufferPtr);
        BufferFree(bufferPtr);
    }

    int ret = AttResponseSendData(connect, packet);
    ServerCallbackReturnValue(ret, connect);
    PacketFree(packet);

ATT_READBYTYPERESPONSE_END:
    AttReadByTypeResponseFree(readByTypeResAsyncPtr);
    return;
}

/**
 * @brief destroy read by type response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttReadByTypeResponseAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    ReadByTypeResponseAsync *readByTypeResAsyncPtr = (ReadByTypeResponseAsync *)context;

    AttReadByTypeResponseFree(readByTypeResAsyncPtr);

    return;
}

/**
 * @brief gatt send readbytype response to att.
 *
 * @param1 connectHandle Indicates the connect handle.
 * @param2 length Indicates the size of each attribute handlevalue pair.
 * @param3 valueList Indicates the pointer to const a list of attribute data.
 * @param4 attrValueNum Indicates the value of attribute value number.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
void ATT_ReadByTypeResponse(
    uint16_t connectHandle, uint8_t length, const AttReadByTypeRspDataList *valueList, uint16_t attrValueNum)
{
    ATT_ReadByTypeResponseCid(connectHandle, 0, length, valueList, attrValueNum);
    return;
}

/**
 * @brief read response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttReadResponseAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    uint16_t mtu = 0;
    int ret;
    uint16_t bufferSize;
    ReadResponseAsync *readResAsyncPtr = (ReadResponseAsync *)context;
    Packet *packet = NULL;
    Buffer *bufferNew = NULL;
    AttConnectInfo *connect = NULL;
    uint8_t *data = NULL;

    connect = AttResponseResolveConnect(readResAsyncPtr->connectHandle, readResAsyncPtr->cid);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATT_READRESPONSE_END", __FUNCTION__);
        goto ATT_READRESPONSE_END;
    }

    AttAssignMTU(&mtu, connect);

    bufferSize = BufferGetSize(readResAsyncPtr->attValue);
    packet = PacketMalloc(0, 0, sizeof(uint8_t));
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    data = BufferPtr(PacketContinuousPayload(packet));
    data[0] = READ_RESPONSE;

    if (bufferSize > (mtu - 1)) {
        bufferNew = BufferSliceMalloc(readResAsyncPtr->attValue, 0, mtu - 1);
        PacketPayloadAddLast(packet, bufferNew);
        BufferFree(bufferNew);
    } else if ((bufferSize <= (mtu - 1)) && (bufferSize > 0)) {
        PacketPayloadAddLast(packet, readResAsyncPtr->attValue);
    }

    ret = AttResponseSendData(connect, packet);
    ServerCallbackReturnValue(ret, connect);
    PacketFree(packet);

ATT_READRESPONSE_END:
    BufferFree(readResAsyncPtr->attValue);
    MEM_MALLOC.free(readResAsyncPtr);
    return;
}

/**
 * @brief destroy read response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttReadResponseAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    ReadResponseAsync *readResAsyncPtr = (ReadResponseAsync *)context;

    BufferFree(readResAsyncPtr->attValue);
    MEM_MALLOC.free(readResAsyncPtr);

    return;
}

/**
 * @brief gatt send read response to att.
 *
 * @param1 connectHandle Indicates the connect handle.
 * @param2 attValue Indicates the pointer to the value of the attribute with the handle given.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
void ATT_ReadResponse(uint16_t connectHandle, const Buffer *attValue)
{
    ATT_ReadResponseCid(connectHandle, 0, attValue);
    return;
}

/**
 * @brief read blob response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttReadBlobResponseAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    int ret;
    uint16_t mtu = 0;
    uint8_t *data = NULL;
    ReadResponseAsync *readBlobResAsyncPtr = NULL;
    AttConnectInfo *connect = NULL;
    uint16_t bufferSize;
    Packet *packet = NULL;
    Buffer *bufferNew = NULL;

    readBlobResAsyncPtr = (ReadResponseAsync *)context;
    connect = AttResponseResolveConnect(readBlobResAsyncPtr->connectHandle, readBlobResAsyncPtr->cid);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATT_READBLOBRESPONSE_END", __FUNCTION__);
        goto ATT_READBLOBRESPONSE_END;
    }

    AttAssignMTU(&mtu, connect);

    bufferSize = BufferGetSize(readBlobResAsyncPtr->attValue);
    packet = PacketMalloc(0, 0, sizeof(uint8_t));
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    data = BufferPtr(PacketContinuousPayload(packet));
    data[0] = READ_BLOB_RESPONSE;

    if (bufferSize > (mtu - 1)) {
        bufferNew = BufferSliceMalloc(readBlobResAsyncPtr->attValue, 0, mtu - 1);
        PacketPayloadAddLast(packet, bufferNew);
        BufferFree(bufferNew);
    } else if ((bufferSize <= (mtu - 1)) && (bufferSize > 0)) {
        PacketPayloadAddLast(packet, readBlobResAsyncPtr->attValue);
    }

    ret = AttResponseSendData(connect, packet);
    ServerCallbackReturnValue(ret, connect);
    PacketFree(packet);

ATT_READBLOBRESPONSE_END:
    BufferFree(readBlobResAsyncPtr->attValue);
    MEM_MALLOC.free(readBlobResAsyncPtr);
    return;
}

/**
 * @brief destroy read blob response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttReadBlobResponseAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    ReadResponseAsync *readBlobResAsyncPtr = (ReadResponseAsync *)context;

    BufferFree(readBlobResAsyncPtr->attValue);
    MEM_MALLOC.free(readBlobResAsyncPtr);

    return;
}

/**
 * @brief gatt send readblob response to att.
 *
 * @param1 connectHandle Indicates the connect handle.
 * @param2 attReadBlobResObj Indicates the pointer to part of the value of the attribute with the handle given.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
void ATT_ReadBlobResponse(uint16_t connectHandle, const Buffer *attReadBlobResObj)
{
    ATT_ReadBlobResponseCid(connectHandle, 0, attReadBlobResObj);
    return;
}

/**
 * @brief read multiple response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttReadMultipleResponseAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    int ret;
    uint16_t bufferSize;
    Packet *packet = NULL;
    uint16_t mtu = 0;
    ReadResponseAsync *readMultipleResponseAsyncPtr = NULL;
    uint8_t *data = NULL;
    Buffer *bufferNew = NULL;
    AttConnectInfo *connect = NULL;

    readMultipleResponseAsyncPtr = (ReadResponseAsync *)context;

    connect = AttResponseResolveConnect(readMultipleResponseAsyncPtr->connectHandle, readMultipleResponseAsyncPtr->cid);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATT_READMULTIPLERESPONSE_END", __FUNCTION__);
        goto ATT_READMULTIPLERESPONSE_END;
    }

    AttAssignMTU(&mtu, connect);

    bufferSize = BufferGetSize(readMultipleResponseAsyncPtr->attValue);
    packet = PacketMalloc(0, 0, sizeof(uint8_t));
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    data = BufferPtr(PacketContinuousPayload(packet));
    data[0] = READ_MULTIPLE_RESPONSE;

    if (bufferSize > (mtu - 1)) {
        bufferNew = BufferSliceMalloc(readMultipleResponseAsyncPtr->attValue, 0, mtu - 1);
        PacketPayloadAddLast(packet, bufferNew);
        BufferFree(bufferNew);
    } else if ((bufferSize <= (mtu - 1)) && (bufferSize > 0)) {
        PacketPayloadAddLast(packet, readMultipleResponseAsyncPtr->attValue);
    }

    ret = AttResponseSendData(connect, packet);
    ServerCallbackReturnValue(ret, connect);
    PacketFree(packet);

ATT_READMULTIPLERESPONSE_END:
    BufferFree(readMultipleResponseAsyncPtr->attValue);
    MEM_MALLOC.free(readMultipleResponseAsyncPtr);
    return;
}

/**
 * @brief destroy read multiple response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttReadMultipleResponseAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    ReadResponseAsync *readMultipleResponseAsyncPtr = (ReadResponseAsync *)context;

    BufferFree(readMultipleResponseAsyncPtr->attValue);
    MEM_MALLOC.free(readMultipleResponseAsyncPtr);

    return;
}

/**
 * @brief gatt send readmultiple response to att.
 *
 * @param1 connectHandle Indicates the connect handle.
 * @param2 valueList Indicates the pointer to a set of two or more values.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
void ATT_ReadMultipleResponse(uint16_t connectHandle, const Buffer *valueList)
{
    ATT_ReadMultipleResponseCid(connectHandle, 0, valueList);
    return;
}

void AttReadByGroupTypeResponseAsyncFree(ReadByGroupTypeResponseAsync *attReadByGroupResponseAsyncPtr, uint16_t num)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    uint16_t index;

    for (index = 0; index < num; index++) {
        MEM_MALLOC.free(attReadByGroupResponseAsyncPtr->attReadGroupResContext.attributeData[index].attributeValue);
    }
    MEM_MALLOC.free(attReadByGroupResponseAsyncPtr->attReadGroupResContext.attributeData);
    MEM_MALLOC.free(attReadByGroupResponseAsyncPtr);

    return;
}

/**
 * @brief read by group type response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttReadByGroupTypeResponseAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    uint16_t index = 0;
    AttConnectInfo *connect = NULL;
    uint16_t mtu = 0;

    ReadByGroupTypeResponseAsync *attReadByGroupResponseAsyncPtr = (ReadByGroupTypeResponseAsync *)context;

    uint8_t len = attReadByGroupResponseAsyncPtr->attReadGroupResContext.length;
    uint16_t num = attReadByGroupResponseAsyncPtr->attReadGroupResContext.num;
    connect = AttResponseResolveConnect(attReadByGroupResponseAsyncPtr->connectHandle,
        attReadByGroupResponseAsyncPtr->cid);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATTREADBYGROUPTYPERESONSE_END", __FUNCTION__);
        goto ATTREADBYGROUPTYPERESONSE_END;
    }

    AttAssignMTU(&mtu, connect);

    if ((len * num) > (mtu - STEP_TWO)) {
        ServerCallbackBTBADPARAM(connect);
        goto ATTREADBYGROUPTYPERESONSE_END;
    }
    if (((len - STEP_FOUR) > (mtu - STEP_SIX)) || ((len - STEP_FOUR) > MAXREADBYGROUPRESLEN)) {
        len = Min(mtu - STEP_SIX, MAXREADBYGROUPRESLEN);
    }

    Packet *packet = PacketMalloc(0, 0, sizeof(uint8_t) + sizeof(len));
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    *(uint8_t *)BufferPtr(PacketContinuousPayload(packet)) = READ_BY_GROUP_TYPE_RESPONSE;
    *((uint8_t *)BufferPtr(PacketContinuousPayload(packet)) + 1) = len;

    for (index = 0; index < num; index++) {
        Buffer *bufferNew = BufferMalloc(len);
        (void)memcpy_s(BufferPtr(bufferNew),
            BufferGetSize(bufferNew),
            &(attReadByGroupResponseAsyncPtr->attReadGroupResContext.attributeData[index]),
            STEP_FOUR);
        (void)memcpy_s(((uint8_t *)BufferPtr(bufferNew) + STEP_FOUR),
            BufferGetSize(bufferNew) - STEP_FOUR,
            attReadByGroupResponseAsyncPtr->attReadGroupResContext.attributeData[index].attributeValue,
            len - STEP_FOUR);
        PacketPayloadAddLast(packet, bufferNew);
        BufferFree(bufferNew);
    }

    int ret = AttResponseSendData(connect, packet);
    ServerCallbackReturnValue(ret, connect);
    PacketFree(packet);

ATTREADBYGROUPTYPERESONSE_END:
    AttReadByGroupTypeResponseAsyncFree(attReadByGroupResponseAsyncPtr, num);
    return;
}

/**
 * @brief destroy read by group type response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttReadByGroupTypeResponseAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    uint16_t num = 0;

    ReadByGroupTypeResponseAsync *attReadByGroupResponseAsyncPtr = NULL;

    attReadByGroupResponseAsyncPtr = (ReadByGroupTypeResponseAsync *)context;
    num = attReadByGroupResponseAsyncPtr->attReadGroupResContext.num;

    AttReadByGroupTypeResponseAsyncFree(attReadByGroupResponseAsyncPtr, num);

    return;
}

/**
 * @brief gatt send readbygrouptype response to att.
 *
 * @param1 connectHandle Indicates the connect handle.
 * @param2 length Indicates the size of each attribute data.
 * @param3 serviceList Indicates the pointer to const a list of attribute data.
 * @param4 serviceNum Indicates the number of attribute data.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
void ATT_ReadByGroupTypeResponse(
    uint16_t connectHandle, uint8_t length, const AttReadGoupAttributeData *serviceList, uint16_t serviceNum)
{
    ATT_ReadByGroupTypeResponseCid(connectHandle, 0, length, serviceList, serviceNum);
    return;
}

/**
 * @brief write response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttWriteResponseAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    int ret;
    Packet *packet = NULL;
    WriteResponseAsync *writeResponseAsyncPtr = NULL;
    uint8_t *data = NULL;
    uint16_t mtu = 0;
    AttConnectInfo *connect = NULL;

    writeResponseAsyncPtr = (WriteResponseAsync *)context;

    connect = AttResponseResolveConnect(writeResponseAsyncPtr->connectHandle, writeResponseAsyncPtr->cid);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATTWRITERESPONSE_END", __FUNCTION__);
        goto ATTWRITERESPONSE_END;
    }

    AttAssignMTU(&mtu, connect);

    packet = PacketMalloc(0, 0, sizeof(uint8_t));
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    data = BufferPtr(PacketContinuousPayload(packet));
    data[0] = WRITE_RESPONSE;

    ret = AttResponseSendData(connect, packet);
    ServerCallbackReturnValue(ret, connect);
    PacketFree(packet);

ATTWRITERESPONSE_END:
    MEM_MALLOC.free(writeResponseAsyncPtr);
    return;
}

/**
 * @brief destroy write response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttWriteResponseAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    WriteResponseAsync *writeResponseAsyncPtr = (WriteResponseAsync *)context;

    MEM_MALLOC.free(writeResponseAsyncPtr);

    return;
}

/**
 * @brief gatt send write response to att.
 *
 * @param connectHandle Indicates the connect handle.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
void ATT_WriteResponse(uint16_t connectHandle)
{
    ATT_WriteResponseCid(connectHandle, 0);
    return;
}

/**
 * @brief prepare write response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttPrepareWriteResponseAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    int ret;
    uint16_t bufferSize;
    Packet *packet = NULL;
    uint8_t *data = NULL;
    uint16_t mtu = 0;
    AttConnectInfo *connect = NULL;
    PrepareWriteAsync *prepareWriteResAsyncPtr = NULL;

    prepareWriteResAsyncPtr = (PrepareWriteAsync *)context;
    bufferSize = BufferGetSize(prepareWriteResAsyncPtr->attValue);

    connect = AttResponseResolveConnect(prepareWriteResAsyncPtr->connectHandle, prepareWriteResAsyncPtr->cid);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATTPREPAREWRITERESPONSE_END", __FUNCTION__);
        goto ATTPREPAREWRITERESPONSE_END;
    }

    AttAssignMTU(&mtu, connect);

    if (bufferSize > (mtu - STEP_FIVE)) {
        LOG_INFO("%{public}s bufferSize > (mtu - 5)", __FUNCTION__);
        ServerCallbackBTBADPARAM(connect);
        goto ATTPREPAREWRITERESPONSE_END;
    }

    packet = PacketMalloc(0, 0, sizeof(uint8_t) + sizeof(prepareWriteResAsyncPtr->attReadBlobObj));
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    data = BufferPtr(PacketContinuousPayload(packet));
    data[0] = PREPARE_WRITE_RESPONSE;
    ((uint16_t *)(data + 1))[0] = prepareWriteResAsyncPtr->attReadBlobObj.attHandle;
    ((uint16_t *)(data + STEP_THREE))[0] = prepareWriteResAsyncPtr->attReadBlobObj.offset;

    if ((bufferSize < (mtu - STEP_FIVE)) && (bufferSize > 0)) {
        PacketPayloadAddLast(packet, prepareWriteResAsyncPtr->attValue);
    }

    ret = AttResponseSendData(connect, packet);
    ServerCallbackReturnValue(ret, connect);
    PacketFree(packet);

ATTPREPAREWRITERESPONSE_END:
    BufferFree(prepareWriteResAsyncPtr->attValue);
    MEM_MALLOC.free(prepareWriteResAsyncPtr);
    return;
}

/**
 * @brief destroy prepare write response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttPrepareWriteResponseAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    PrepareWriteAsync *prepareWriteResAsyncPtr = (PrepareWriteAsync *)context;

    BufferFree(prepareWriteResAsyncPtr->attValue);
    MEM_MALLOC.free(prepareWriteResAsyncPtr);

    return;
}

/**
 * @brief gatt send preparewrite response to att.
 *
 * @param1 connectHandle Indicates the connect handle.
 * @param2 attReadBlobObj Indicates the value of the struct AttReadBlobReqPrepareWriteValue.
 * @param3 attValue Indicates the pointer to the value of the attribute to be written.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
void ATT_PrepareWriteResponse(
    uint16_t connectHandle, AttReadBlobReqPrepareWriteValue attReadBlobObj, const Buffer *attValue)
{
    ATT_PrepareWriteResponseCid(connectHandle, 0, attReadBlobObj, attValue);
    return;
}

/**
 * @brief execute write response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttExecuteWriteResponseAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    int ret;
    Packet *packet = NULL;
    uint8_t *data = NULL;
    uint16_t mtu = 0;
    AttConnectInfo *connect = NULL;
    WriteResponseAsync *executeWriteResAsyncPtr = (WriteResponseAsync *)context;

    connect = AttResponseResolveConnect(executeWriteResAsyncPtr->connectHandle, executeWriteResAsyncPtr->cid);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATTEXECUTEWRITERESPONSE_END", __FUNCTION__);
        goto ATTEXECUTEWRITERESPONSE_END;
    }

    AttAssignMTU(&mtu, connect);

    packet = PacketMalloc(0, 0, sizeof(uint8_t));
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    data = BufferPtr(PacketContinuousPayload(packet));
    data[0] = EXECUTE_WRITE_RESPONSE;

    ret = AttResponseSendData(connect, packet);
    ServerCallbackReturnValue(ret, connect);
    PacketFree(packet);

ATTEXECUTEWRITERESPONSE_END:
    MEM_MALLOC.free(executeWriteResAsyncPtr);
    return;
}

/**
 * @brief destroy execute write response in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttExecuteWriteResponseAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    WriteResponseAsync *executeWriteResAsyncPtr = (WriteResponseAsync *)context;

    MEM_MALLOC.free(executeWriteResAsyncPtr);

    return;
}

/**
 * @brief gatt send executewrite response to att.
 *
 * @param connectHandle Indicates the connect handle.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
void ATT_ExecuteWriteResponse(uint16_t connectHandle)
{
    ATT_ExecuteWriteResponseCid(connectHandle, 0);
    return;
}

/* Resolve the bearer a server notification or indication targets. cid 0 selects the first idle
 * EATT bearer (no in-flight client request for a notification, no pending confirmation for an
 * indication, Vol 3 Part F 3.3.2) and falls back to UATT; the response helper instead pins the
 * source bearer. */
static AttConnectInfo *AttNtfIndResolveConnect(uint16_t connectHandle, uint16_t cid, bool isIndication)
{
    if (cid == 0) {
        return isIndication ? AttGetConnectInfoByConnectHandlePreferEattInd(connectHandle)
                            : AttGetConnectInfoByConnectHandlePreferEattRequestOrNtf(connectHandle);
    }
    return AttGetConnectInfoByConnectHandleAndLeCid(connectHandle, cid);
}

/**
 * @brief handle value notification in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttHandleValueNotificationAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    int ret;
    Packet *packet = NULL;
    Buffer *bufferNew = NULL;
    AttConnectInfo *connect = NULL;
    WriteAsync *handleNotificationAsyncPtr = NULL;
    uint16_t bufferSizenoti;
    uint8_t *data = NULL;

    handleNotificationAsyncPtr = (WriteAsync *)context;

    connect = AttNtfIndResolveConnect(
        handleNotificationAsyncPtr->connectHandle, handleNotificationAsyncPtr->cid, false);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATT_HANDLEVALUENOTIFICATION_END", __FUNCTION__);
        goto ATT_HANDLEVALUENOTIFICATION_END;
    }

    bufferSizenoti = BufferGetSize(handleNotificationAsyncPtr->attValue);
    packet = PacketMalloc(0, 0, sizeof(uint8_t) + sizeof(uint16_t));
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    data = BufferPtr(PacketContinuousPayload(packet));
    data[0] = HANDLE_VALUE_NOTIFICATION;
    ((uint16_t *)(data + 1))[0] = handleNotificationAsyncPtr->attHandle;

    if ((bufferSizenoti > 0) && (bufferSizenoti <= (connect->mtu - STEP_THREE))) {
        PacketPayloadAddLast(packet, handleNotificationAsyncPtr->attValue);
    } else if (bufferSizenoti > (connect->mtu - STEP_THREE)) {
        uint16_t len = connect->mtu - STEP_THREE;
        bufferNew = BufferSliceMalloc(handleNotificationAsyncPtr->attValue, 0, len);
        PacketPayloadAddLast(packet, bufferNew);
        BufferFree(bufferNew);
    }

    ret = AttResponseSendData(connect, packet);
    ServerCallbackReturnValue(ret, connect);
    PacketFree(packet);

ATT_HANDLEVALUENOTIFICATION_END:
    BufferFree(handleNotificationAsyncPtr->attValue);
    MEM_MALLOC.free(handleNotificationAsyncPtr);
    return;
}

/**
 * @brief destroy handle value notification in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttHandleValueNotificationAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    WriteAsync *handleNotificationAsyncPtr = (WriteAsync *)context;

    BufferFree(handleNotificationAsyncPtr->attValue);
    MEM_MALLOC.free(handleNotificationAsyncPtr);

    return;
}

/**
 * @brief gatt send handlevalue notification to att.
 *
 * @param1 connectHandle Indicates the connect handle.
 * @param2 attHandle Indicates the handle of the attribute.
 * @param3 attValue Indicates the pointer to the current value of the attribute.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
void ATT_HandleValueNotification(uint16_t connectHandle, uint16_t attHandle, const Buffer *attValue)
{
    ATT_HandleValueNotificationCid(connectHandle, 0, attHandle, attValue);
    return;
}

void ATT_HandleValueNotificationCid(
    uint16_t connectHandle, uint16_t cid, uint16_t attHandle, const Buffer *attValue)
{
    LOG_INFO("%{public}s enter, connectHandle = %hu, cid = %hu, attHandle = %{public}d",
        __FUNCTION__, connectHandle, cid, attHandle);

    if (attValue == NULL) {
        LOG_WARN("%{public}s: attValue is NULL", __FUNCTION__);
        return;
    }

    Buffer *bufferPtr = NULL;
    WriteAsync *handleNotificationAsyncPtr = NULL;

    bufferPtr = BufferRefMalloc(attValue);
    handleNotificationAsyncPtr = MEM_MALLOC.alloc(sizeof(WriteAsync));
    if (handleNotificationAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        BufferFree(bufferPtr);
        bufferPtr = NULL;
        return;
    }
    handleNotificationAsyncPtr->connectHandle = connectHandle;
    handleNotificationAsyncPtr->cid = cid;
    handleNotificationAsyncPtr->attHandle = attHandle;
    handleNotificationAsyncPtr->attValue = bufferPtr;

    AttAsyncProcess(
        AttHandleValueNotificationAsync, AttHandleValueNotificationAsyncDestroy, handleNotificationAsyncPtr);

    return;
}

/**
 * @brief handle value indication in self thread..
 *
 * @param context Indicates the pointer to context.
 */
/**
 * @brief build the indication packet: header, handle and a payload limited to the negotiated MTU.
 *
 * @param handleIndicationAsyncPtr Indicates the pending indication context.
 * @param mtu Indicates the negotiated MTU of the bearer.
 * @return Packet pointer on success, NULL on allocation failure.
 */
static Packet *AttBuildIndicationPacket(WriteAsync *handleIndicationAsyncPtr, uint16_t mtu)
{
    uint16_t bufferSize = BufferGetSize(handleIndicationAsyncPtr->attValue);
    uint16_t len;
    Buffer *bufferNew = NULL;
    uint8_t *data = NULL;
    Packet *packet = PacketMalloc(0, 0, sizeof(uint8_t) + sizeof(handleIndicationAsyncPtr->attHandle));
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        return NULL;
    }
    data = BufferPtr(PacketContinuousPayload(packet));
    data[0] = HANDLE_VALUE_INDICATION;
    ((uint16_t *)(data + 1))[0] = handleIndicationAsyncPtr->attHandle;
    if ((bufferSize > 0) && (bufferSize <= (mtu - STEP_THREE))) {
        PacketPayloadAddLast(packet, handleIndicationAsyncPtr->attValue);
    } else if (bufferSize > (mtu - STEP_THREE)) {
        len = mtu - STEP_THREE;
        bufferNew = BufferSliceMalloc(handleIndicationAsyncPtr->attValue, 0, len);
        PacketPayloadAddLast(packet, bufferNew);
        BufferFree(bufferNew);
    }

    return packet;
}

static void AttHandleValueIndicationAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    int ret;
    Packet *packet = NULL;
    WriteAsync *handleIndicationAsyncPtr = NULL;
    AttConnectInfo *connect = NULL;

    handleIndicationAsyncPtr = (WriteAsync *)context;

    connect = AttNtfIndResolveConnect(
        handleIndicationAsyncPtr->connectHandle, handleIndicationAsyncPtr->cid, true);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATTHANDLEVALUEINDICATION_END", __FUNCTION__);
        goto ATTHANDLEVALUEINDICATION_END;
    }

    // Vol 3 Part F 3.3.2: one outstanding indication per bearer until it is confirmed, so an
    // indication sent while one is pending is rejected.
    if (connect->serverSendFlag) {
        LOG_INFO("%{public}s pending indication, reject, connectHandle = %hu", __FUNCTION__,
            connect->retGattConnectHandle);
        ServerCallbackReturnValue(BT_ALREADY, connect);
        goto ATTHANDLEVALUEINDICATION_END;
    }

    packet = AttBuildIndicationPacket(handleIndicationAsyncPtr, connect->mtu);
    if (packet == NULL) {
        goto ATTHANDLEVALUEINDICATION_END;
    }

    // The indication transaction starts when queued and ends at the confirmation (Vol 3 Part F
    // 3.3.3); the per-bearer timeout is armed only on a successful queue.
    ret = AttResponseSendData(connect, packet);
    if (ret == BT_SUCCESS) {
        connect->serverSendFlag = true;
        AttStartIndicationAlarm(connect);
    }
    ServerCallbackReturnValue(ret, connect);
    PacketFree(packet);

ATTHANDLEVALUEINDICATION_END:
    BufferFree(handleIndicationAsyncPtr->attValue);
    MEM_MALLOC.free(handleIndicationAsyncPtr);
    return;
}

/**
 * @brief destroy handle value indication in self thread..
 *
 * @param context Indicates the pointer to context.
 */
static void AttHandleValueIndicationAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    WriteAsync *handleIndicationAsyncPtr = (WriteAsync *)context;

    BufferFree(handleIndicationAsyncPtr->attValue);
    MEM_MALLOC.free(handleIndicationAsyncPtr);

    return;
}

/**
 * @brief gatt send handlevalue indication to att.
 *
 * @param1 connectHandle Indicates the connect handle.
 * @param2 attHandle Indicates the handle of the attribute.
 * @param3 attValue Indicates the pointer to the current value of the attribute.
 * @return Returns <b>0</b> if the operation is successful; returns <b>!0</b> if the operation fails.
 */
void ATT_HandleValueIndication(uint16_t connectHandle, uint16_t attHandle, const Buffer *attValue)
{
    ATT_HandleValueIndicationCid(connectHandle, 0, attHandle, attValue);
    return;
}

void ATT_HandleValueIndicationCid(
    uint16_t connectHandle, uint16_t cid, uint16_t attHandle, const Buffer *attValue)
{
    LOG_INFO("%{public}s enter, connectHandle = %hu, cid = %hu, attHandle = %{public}d",
        __FUNCTION__, connectHandle, cid, attHandle);

    if (attValue == NULL) {
        LOG_WARN("%{public}s: attValue is NULL", __FUNCTION__);
        return;
    }

    Buffer *bufferPtr = NULL;
    WriteAsync *handleIndicationAsyncPtr = NULL;

    bufferPtr = BufferRefMalloc(attValue);
    handleIndicationAsyncPtr = MEM_MALLOC.alloc(sizeof(WriteAsync));
    if (handleIndicationAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        BufferFree(bufferPtr);
        bufferPtr = NULL;
        return;
    }
    handleIndicationAsyncPtr->connectHandle = connectHandle;
    handleIndicationAsyncPtr->cid = cid;
    handleIndicationAsyncPtr->attHandle = attHandle;
    handleIndicationAsyncPtr->attValue = bufferPtr;

    AttAsyncProcess(AttHandleValueIndicationAsync, AttHandleValueIndicationAsyncDestroy, handleIndicationAsyncPtr);

    return;
}

/**
 * @brief server send data register..
 *
 * @param context Indicates the pointer to context.
 */
void AttServerSendDataRegisterAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttServerSendDataCallback *attServerSendDataCallbackPtr = (AttServerSendDataCallback *)context;

    g_attServerSendDataCB.attSendDataCB = attServerSendDataCallbackPtr->attSendDataCB;
    g_attServerSendDataCB.context = attServerSendDataCallbackPtr->context;

    AttServerCallBackCopyToCommon(attServerSendDataCallbackPtr->attSendDataCB, attServerSendDataCallbackPtr->context);

    MEM_MALLOC.free(attServerSendDataCallbackPtr);

    return;
}

/**
 * @brief server send data register.destroy.
 *
 * @param context Indicates the pointer to context.
 */
void AttServerSendDataRegisterAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttServerSendDataCallback *attServerSendDataCallbackPtr = (AttServerSendDataCallback *)context;

    MEM_MALLOC.free(attServerSendDataCallbackPtr);

    return;
}

/**
 * @brief server gatt send data register.
 *
 * @param1 attSendDataCB Indicates the pointer of attSendDataCallback.
 * @param2 context Indicates the pointer of context.
 */
void ATT_ServerSendDataRegister(attSendDataCallback attSendDataCB, void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttServerSendDataCallback *attServerSendDataCallbackPtr = MEM_MALLOC.alloc(sizeof(AttServerSendDataCallback));
    if (attServerSendDataCallbackPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    attServerSendDataCallbackPtr->attSendDataCB = attSendDataCB;
    attServerSendDataCallbackPtr->context = context;

    AttAsyncProcess(
        AttServerSendDataRegisterAsync, AttServerSendDataRegisterAsyncDestroy, attServerSendDataCallbackPtr);

    return;
}

/**
 * @brief server send data deregister.
 *
 * @param context Indicates the pointer to context.
 */
void AttServerSendDataDeRegisterAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    g_attServerSendDataCB.attSendDataCB = NULL;
    g_attServerSendDataCB.context = NULL;

    AttCallBackDelectCopyToCommon();

    return;
}

/**
 * @brief server send data deregister.destroy.
 *
 * @param context Indicates the pointer to context.
 */
void AttServerSendDataDeRegisterAsyncDestroy(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    return;
}

/**
 * @brief server gatt send data deregister.
 *
 */
void ATT_ServerSendDataDeRegister()
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    AttAsyncProcess(AttServerSendDataDeRegisterAsync, AttServerSendDataDeRegisterAsyncDestroy, NULL);

    return;
}

void ATT_ErrorResponseCid(uint16_t connectHandle, uint16_t cid, const AttError *attErrorPtr)
{
    if (attErrorPtr == NULL) {
        LOG_WARN("%{public}s: attErrorPtr is NULL", __FUNCTION__);
        return;
    }

    LOG_INFO("%{public}s enter, connectHandle = %hu, cid = %hu, reqOpcode = %{public}d, "
             "attHandleInError = %{public}d, errorCode = %{public}d",
        __FUNCTION__, connectHandle, cid, attErrorPtr->reqOpcode, attErrorPtr->attHandleInError,
        attErrorPtr->errorCode);

    AttError *attErrorAsyncPtr = NULL;
    ErrorResponseAsync *errorResAsyncPtr = NULL;

    attErrorAsyncPtr = MEM_MALLOC.alloc(sizeof(AttError));
    if (attErrorAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    attErrorAsyncPtr->reqOpcode = attErrorPtr->reqOpcode;
    attErrorAsyncPtr->attHandleInError = attErrorPtr->attHandleInError;
    attErrorAsyncPtr->errorCode = attErrorPtr->errorCode;

    errorResAsyncPtr = MEM_MALLOC.alloc(sizeof(ErrorResponseAsync));
    if (errorResAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        MEM_MALLOC.free(attErrorAsyncPtr);
        return;
    }
    errorResAsyncPtr->connectHandle = connectHandle;
    errorResAsyncPtr->cid = cid;
    errorResAsyncPtr->ATTErrorPtr = attErrorAsyncPtr;

    AttAsyncProcess(AttErrorResponseAsync, AttErrorResponseAsyncDestroy, errorResAsyncPtr);

    return;
}

void ATT_FindInformationResponseCid(
    uint16_t connectHandle, uint16_t cid, uint8_t format, AttHandleUuid *handleUUIDPairs, uint16_t pairNum)
{
    LOG_INFO("%{public}s enter, connectHandle = %hu, cid = %hu, format=%hhu, pairNum=%hu", __FUNCTION__, connectHandle,
        cid, format, pairNum);

    if (handleUUIDPairs == NULL) {
        LOG_WARN("%{public}s: handleUUIDPairs is NULL", __FUNCTION__);
        return;
    }

    AttHandleUuid *attHandleUuidPtr = MEM_MALLOC.alloc(sizeof(AttHandleUuid) * pairNum);
    if (attHandleUuidPtr == NULL) {
        LOG_WARN("attHandleUuidPtr is null");
        return;
    }

    for (int index = 0; index < pairNum; ++index) {
        attHandleUuidPtr[index].attHandle = handleUUIDPairs[index].attHandle;
        if (format == HANDLEAND16BITBLUETOOTHUUID) {
            attHandleUuidPtr[index].uuid.uuid16 = handleUUIDPairs[index].uuid.uuid16;
        } else if (format == HANDLEAND128BITUUID) {
            (void)memcpy_s(attHandleUuidPtr[index].uuid.uuid128, UUID128BITTYPELEN, handleUUIDPairs[index].uuid.uuid128,
                UUID128BITTYPELEN);
        }
    }
    FindInformationResponseAsync *findInfor = MEM_MALLOC.alloc(sizeof(FindInformationResponseAsync));
    if (findInfor == NULL) {
        LOG_ERROR("point to NULL");
        MEM_MALLOC.free(attHandleUuidPtr);
        return;
    }
    findInfor->connectHandle = connectHandle;
    findInfor->cid = cid;
    findInfor->attFindInformationResContext.format = format;
    findInfor->attFindInformationResContext.pairNum = pairNum;
    findInfor->attFindInformationResContext.handleUuidPairs = attHandleUuidPtr;

    AttAsyncProcess(AttFindInformationResponseAsync, AttFindInformationResponseAsyncDestroy, findInfor);

    return;
}

void ATT_FindByTypeValueResponseCid(
    uint16_t connectHandle, uint16_t cid, const AttHandleInfo *handleInfoList, uint16_t listNum)
{
    LOG_INFO("%{public}s enter,connectHandle = %hu, cid = %hu, listNum=%hu", __FUNCTION__, connectHandle, cid, listNum);

    if (handleInfoList == NULL) {
        LOG_WARN("%{public}s: handleInfoList is NULL", __FUNCTION__);
        return;
    }

    AttHandleInfo *attHandleInfoPtr = MEM_MALLOC.alloc(sizeof(AttHandleInfo) * listNum);
    if (attHandleInfoPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    (void)memcpy_s(attHandleInfoPtr, sizeof(AttHandleInfo) * listNum, handleInfoList, sizeof(AttHandleInfo) * listNum);
    FindByTypeValueResponseAsync *findByTypeResAsyncPtr = MEM_MALLOC.alloc(sizeof(FindByTypeValueResponseAsync));
    if (findByTypeResAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        MEM_MALLOC.free(attHandleInfoPtr);
        return;
    }
    findByTypeResAsyncPtr->connectHandle = connectHandle;
    findByTypeResAsyncPtr->cid = cid;
    findByTypeResAsyncPtr->attFindByTypeResContext.listNum = listNum;
    findByTypeResAsyncPtr->attFindByTypeResContext.handleInfoList = attHandleInfoPtr;

    AttAsyncProcess(AttFindByTypeValueResponseAsync, AttFindByTypeValueResponseAsyncDestroy, findByTypeResAsyncPtr);

    return;
}

void ATT_ReadByTypeResponseCid(uint16_t connectHandle, uint16_t cid, uint8_t length,
    const AttReadByTypeRspDataList *valueList, uint16_t attrValueNum)
{
    LOG_INFO("%{public}s enter, connectHandle = %hu, cid = %hu, length=%hhu, attrValueNum = %hu", __FUNCTION__,
        connectHandle, cid, length, attrValueNum);

    // length is the size of each handle-value pair, at least the 2-octet attribute handle
    // (Vol 3 Part F Table 3.4): below that length - STEP_TWO would underflow the allocation.
    if ((valueList == NULL) || (length < STEP_TWO)) {
        LOG_WARN("%{public}s: valueList is NULL or length < STEP_TWO, length = %hhu", __FUNCTION__, length);
        return;
    }

    uint16_t index = 0;
    uint16_t i = 0;
    AttReadByTypeRspDataList *attReadByTypeDataPtr = MEM_MALLOC.alloc(sizeof(AttReadByTypeRspDataList) * attrValueNum);
    if (attReadByTypeDataPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }

    for (; index < attrValueNum; ++index) {
        attReadByTypeDataPtr[index].attHandle = valueList[index].attHandle;
        attReadByTypeDataPtr[index].attributeValue = MEM_MALLOC.alloc(length - STEP_TWO);
        if (attReadByTypeDataPtr[index].attributeValue == NULL) {
            LOG_WARN("point is null");
            // free the array and the values allocated so far before bailing
            for (i = 0; i < index; ++i) {
                MEM_MALLOC.free(attReadByTypeDataPtr[i].attributeValue);
            }
            MEM_MALLOC.free(attReadByTypeDataPtr);
            return;
        }

        (void)memcpy_s(attReadByTypeDataPtr[index].attributeValue, length - STEP_TWO, valueList[index].attributeValue,
            length - STEP_TWO);
    }

    ReadByTypeResponseAsync *readByTypeResAsyncPtr = MEM_MALLOC.alloc(sizeof(ReadByTypeResponseAsync));
    if (readByTypeResAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        for (i = 0; i < attrValueNum; ++i) {
            MEM_MALLOC.free(attReadByTypeDataPtr[i].attributeValue);
        }
        MEM_MALLOC.free(attReadByTypeDataPtr);
        return;
    }
    readByTypeResAsyncPtr->connectHandle = connectHandle;
    readByTypeResAsyncPtr->cid = cid;
    readByTypeResAsyncPtr->attReadByTypeRspContext.len = length;
    readByTypeResAsyncPtr->attReadByTypeRspContext.valueNum = attrValueNum;
    readByTypeResAsyncPtr->attReadByTypeRspContext.valueList = attReadByTypeDataPtr;

    AttAsyncProcess(AttReadByTypeResponseAsync, AttReadByTypeResponseAsyncDestroy, readByTypeResAsyncPtr);

    return;
}

void ATT_ReadResponseCid(uint16_t connectHandle, uint16_t cid, const Buffer *attValue)
{
    LOG_INFO("%{public}s enter,connectHandle = %hu, cid = %hu", __FUNCTION__, connectHandle, cid);

    if (attValue == NULL) {
        LOG_WARN("%{public}s: attValue is NULL", __FUNCTION__);
        return;
    }

    Buffer *bufferPtr = NULL;
    ReadResponseAsync *readResAsyncPtr = NULL;

    bufferPtr = BufferRefMalloc(attValue);
    readResAsyncPtr = MEM_MALLOC.alloc(sizeof(ReadResponseAsync));
    if (readResAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        BufferFree(bufferPtr);
        bufferPtr = NULL;
        return;
    }
    readResAsyncPtr->connectHandle = connectHandle;
    readResAsyncPtr->cid = cid;
    readResAsyncPtr->attValue = bufferPtr;

    AttAsyncProcess(AttReadResponseAsync, AttReadResponseAsyncDestroy, readResAsyncPtr);

    return;
}

void ATT_ReadBlobResponseCid(uint16_t connectHandle, uint16_t cid, const Buffer *attReadBlobResObj)
{
    LOG_INFO("%{public}s enter,connectHandle = %hu, cid = %hu", __FUNCTION__, connectHandle, cid);

    if (attReadBlobResObj == NULL) {
        LOG_WARN("%{public}s: attReadBlobResObj is NULL", __FUNCTION__);
        return;
    }

    Buffer *bufferPtr = NULL;
    ReadResponseAsync *readBlobResAsyncPtr = NULL;

    bufferPtr = BufferRefMalloc(attReadBlobResObj);
    readBlobResAsyncPtr = MEM_MALLOC.alloc(sizeof(ReadResponseAsync));
    if (readBlobResAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        BufferFree(bufferPtr);
        bufferPtr = NULL;
        return;
    }
    readBlobResAsyncPtr->connectHandle = connectHandle;
    readBlobResAsyncPtr->cid = cid;
    readBlobResAsyncPtr->attValue = bufferPtr;

    AttAsyncProcess(AttReadBlobResponseAsync, AttReadBlobResponseAsyncDestroy, readBlobResAsyncPtr);

    return;
}

void ATT_ReadMultipleResponseCid(uint16_t connectHandle, uint16_t cid, const Buffer *valueList)
{
    LOG_INFO("%{public}s enter,connectHandle = %hu, cid = %hu", __FUNCTION__, connectHandle, cid);

    if (valueList == NULL) {
        LOG_WARN("%{public}s: valueList is NULL", __FUNCTION__);
        return;
    }

    Buffer *bufferPtr = NULL;
    ReadResponseAsync *readMultipleResAsyncPtr = NULL;

    bufferPtr = BufferRefMalloc(valueList);
    readMultipleResAsyncPtr = MEM_MALLOC.alloc(sizeof(ReadResponseAsync));
    if (readMultipleResAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        BufferFree(bufferPtr);
        bufferPtr = NULL;
        return;
    }
    readMultipleResAsyncPtr->connectHandle = connectHandle;
    readMultipleResAsyncPtr->cid = cid;
    readMultipleResAsyncPtr->attValue = bufferPtr;

    AttAsyncProcess(AttReadMultipleResponseAsync, AttReadMultipleResponseAsyncDestroy, readMultipleResAsyncPtr);

    return;
}

/**
 * @brief gatt send read multiple variable response to att.
 *
 * @param context Indicates the pointer to context.
 */
static void AttSendReadMultipleVariableResponseAsync(const void *context)
{
    LOG_INFO("%{public}s enter", __FUNCTION__);

    int ret;
    uint16_t mtu = 0;
    size_t bufferSize;
    Packet *packet = NULL;
    uint8_t *data = NULL;
    Buffer *bufferNew = NULL;
    AttConnectInfo *connect = NULL;
    ReadResponseAsync *readMultipleVariableResponseAsyncPtr = NULL;

    readMultipleVariableResponseAsyncPtr = (ReadResponseAsync *)context;

    connect = AttResponseResolveConnect(
        readMultipleVariableResponseAsyncPtr->connectHandle, readMultipleVariableResponseAsyncPtr->cid);
    if (connect == NULL) {
        LOG_INFO("%{public}s connect == NULL and goto ATT_READMULTIPLEVARIABLERESPONSE_END", __FUNCTION__);
        goto ATT_READMULTIPLEVARIABLERESPONSE_END;
    }

    AttAssignMTU(&mtu, connect);

    bufferSize = BufferGetSize(readMultipleVariableResponseAsyncPtr->attValue);
    packet = PacketMalloc(0, 0, sizeof(uint8_t));
    if (packet == NULL) {
        LOG_ERROR("point to NULL");
        goto ATT_READMULTIPLEVARIABLERESPONSE_END;
    }
    data = BufferPtr(PacketContinuousPayload(packet));
    data[0] = READ_MULTIPLE_VARIABLE_RESPONSE;

    if (bufferSize > (mtu - 1)) {
        bufferNew = BufferSliceMalloc(readMultipleVariableResponseAsyncPtr->attValue, 0, mtu - 1);
        PacketPayloadAddLast(packet, bufferNew);
        BufferFree(bufferNew);
    } else if ((bufferSize <= (mtu - 1)) && (bufferSize > 0)) {
        PacketPayloadAddLast(packet, readMultipleVariableResponseAsyncPtr->attValue);
    }

    ret = AttResponseSendData(connect, packet);
    ServerCallbackReturnValue(ret, connect);
    PacketFree(packet);

ATT_READMULTIPLEVARIABLERESPONSE_END:
    BufferFree(readMultipleVariableResponseAsyncPtr->attValue);
    MEM_MALLOC.free(readMultipleVariableResponseAsyncPtr);
    return;
}

void ATT_ReadMultipleVariableResponseCid(uint16_t connectHandle, uint16_t cid, const Buffer *valueList)
{
    LOG_INFO("%{public}s enter,connectHandle = %hu, cid = %hu", __FUNCTION__, connectHandle, cid);

    if (valueList == NULL) {
        LOG_WARN("%{public}s: valueList is NULL", __FUNCTION__);
        return;
    }

    Buffer *bufferPtr = NULL;
    ReadResponseAsync *readMultipleVariableResAsyncPtr = NULL;

    bufferPtr = BufferRefMalloc(valueList);
    readMultipleVariableResAsyncPtr = MEM_MALLOC.alloc(sizeof(ReadResponseAsync));
    if (readMultipleVariableResAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        BufferFree(bufferPtr);
        bufferPtr = NULL;
        return;
    }
    readMultipleVariableResAsyncPtr->connectHandle = connectHandle;
    readMultipleVariableResAsyncPtr->cid = cid;
    readMultipleVariableResAsyncPtr->attValue = bufferPtr;

    AttAsyncProcess(
        AttSendReadMultipleVariableResponseAsync, AttReadMultipleResponseAsyncDestroy, readMultipleVariableResAsyncPtr);

    return;
}

void ATT_ReadByGroupTypeResponseCid(uint16_t connectHandle, uint16_t cid, uint8_t length,
    const AttReadGoupAttributeData *serviceList, uint16_t serviceNum)
{
    LOG_INFO("%{public}s enter, connectHandle = %hu, cid = %hu,length = %{public}d,serviceNum = %{public}d",
        __FUNCTION__, connectHandle, cid, length, serviceNum);

    if ((serviceList == NULL) || (length < STEP_FOUR)) {
        LOG_WARN("%{public}s: serviceList is NULL or length < STEP_FOUR, length = %hhu", __FUNCTION__, length);
        return;
    }

    uint16_t index = 0;
    uint16_t i = 0;
    AttReadGoupAttributeData *attReadGroupAttrDataPtr = NULL;
    ReadByGroupTypeResponseAsync *attReadByGroupResAsyncPtr = NULL;

    attReadGroupAttrDataPtr = MEM_MALLOC.alloc(sizeof(AttReadGoupAttributeData) * serviceNum);
    if (attReadGroupAttrDataPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    for (; index < serviceNum; ++index) {
        attReadGroupAttrDataPtr[index].attributeValue = MEM_MALLOC.alloc(length - STEP_FOUR);
        if (attReadGroupAttrDataPtr[index].attributeValue == NULL) {
            LOG_WARN("point is null");
            // free the array and the values allocated so far before bailing
            for (i = 0; i < index; ++i) {
                MEM_MALLOC.free(attReadGroupAttrDataPtr[i].attributeValue);
            }
            MEM_MALLOC.free(attReadGroupAttrDataPtr);
            return;
        }
        attReadGroupAttrDataPtr[index].attHandle = serviceList[index].attHandle;
        attReadGroupAttrDataPtr[index].groupEndHandle = serviceList[index].groupEndHandle;
        (void)memcpy_s(attReadGroupAttrDataPtr[index].attributeValue, length - STEP_FOUR,
            serviceList[index].attributeValue, length - STEP_FOUR);
    }

    attReadByGroupResAsyncPtr = MEM_MALLOC.alloc(sizeof(ReadByGroupTypeResponseAsync));
    if (attReadByGroupResAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        for (i = 0; i < serviceNum; ++i) {
            MEM_MALLOC.free(attReadGroupAttrDataPtr[i].attributeValue);
        }
        MEM_MALLOC.free(attReadGroupAttrDataPtr);
        return;
    }
    attReadByGroupResAsyncPtr->connectHandle = connectHandle;
    attReadByGroupResAsyncPtr->cid = cid;
    attReadByGroupResAsyncPtr->attReadGroupResContext.length = length;
    attReadByGroupResAsyncPtr->attReadGroupResContext.num = serviceNum;
    attReadByGroupResAsyncPtr->attReadGroupResContext.attributeData = attReadGroupAttrDataPtr;

    AttAsyncProcess(AttReadByGroupTypeResponseAsync, AttReadByGroupTypeResponseAsyncDestroy, attReadByGroupResAsyncPtr);

    return;
}

void ATT_WriteResponseCid(uint16_t connectHandle, uint16_t cid)
{
    LOG_INFO("%{public}s enter, connectHandle = %hu, cid = %hu", __FUNCTION__, connectHandle, cid);

    WriteResponseAsync *writeResAsyncPtr = MEM_MALLOC.alloc(sizeof(WriteResponseAsync));
    if (writeResAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    writeResAsyncPtr->connectHandle = connectHandle;
    writeResAsyncPtr->cid = cid;

    AttAsyncProcess(AttWriteResponseAsync, AttWriteResponseAsyncDestroy, writeResAsyncPtr);

    return;
}

void ATT_PrepareWriteResponseCid(
    uint16_t connectHandle, uint16_t cid, AttReadBlobReqPrepareWriteValue attReadBlobObj, const Buffer *attValue)
{
    LOG_INFO("%{public}s enter, connectHandle = %hu, cid = %hu, attHandle = %{public}d, offset = %{public}d",
        __FUNCTION__, connectHandle, cid, attReadBlobObj.attHandle, attReadBlobObj.offset);

    if (attValue == NULL) {
        LOG_WARN("%{public}s: attValue is NULL", __FUNCTION__);
        return;
    }

    Buffer *bufferPtr = NULL;
    PrepareWriteAsync *prepareWriteResAsyncPtr = NULL;

    bufferPtr = BufferRefMalloc(attValue);
    prepareWriteResAsyncPtr = MEM_MALLOC.alloc(sizeof(PrepareWriteAsync));
    if (prepareWriteResAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        BufferFree(bufferPtr);
        bufferPtr = NULL;
        return;
    }
    prepareWriteResAsyncPtr->connectHandle = connectHandle;
    prepareWriteResAsyncPtr->cid = cid;
    prepareWriteResAsyncPtr->attReadBlobObj.attHandle = attReadBlobObj.attHandle;
    prepareWriteResAsyncPtr->attReadBlobObj.offset = attReadBlobObj.offset;
    prepareWriteResAsyncPtr->attValue = bufferPtr;

    AttAsyncProcess(AttPrepareWriteResponseAsync, AttPrepareWriteResponseAsyncDestroy, prepareWriteResAsyncPtr);

    return;
}

void ATT_ExecuteWriteResponseCid(uint16_t connectHandle, uint16_t cid)
{
    LOG_INFO("%{public}s enter,connectHandle = %hu, cid = %hu", __FUNCTION__, connectHandle, cid);

    WriteResponseAsync *executeWriteResAsyncPtr = MEM_MALLOC.alloc(sizeof(WriteResponseAsync));
    if (executeWriteResAsyncPtr == NULL) {
        LOG_ERROR("point to NULL");
        return;
    }
    executeWriteResAsyncPtr->connectHandle = connectHandle;
    executeWriteResAsyncPtr->cid = cid;

    AttAsyncProcess(AttExecuteWriteResponseAsync, AttExecuteWriteResponseAsyncDestroy, executeWriteResAsyncPtr);

    return;
}
