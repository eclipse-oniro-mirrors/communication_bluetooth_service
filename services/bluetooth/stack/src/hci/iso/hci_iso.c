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

#include "hci_iso.h"

#include <stdbool.h>

#include "btstack.h"
#include "log.h"
#include "platform/include/allocator.h"
#include "platform/include/list.h"
#include "platform/include/mutex.h"
#include "platform/include/queue.h"

#include "hci/acl/hci_acl.h"
#include "hci/hci.h"
#include "hci/hci_internal.h"

// 4-octet HCI ISO Data packet header: Connection_Handle (12b) + PB_Flag (2b)
// + TS_Flag (1b) + RFU (1b) + ISO_Data_Load_Length (14b) + RFU (2b).
// Vol 4, Part E, 5.4.5.
#pragma pack(1)
typedef struct {
    uint16_t handle : 12;
    uint16_t pbFlag : 2;
    uint16_t tsFlag : 1;
    uint16_t rfu : 1;
    uint16_t loadLength : 14;
    uint16_t rfu2 : 2;
} HciIsoDataHeader;
#pragma pack()

typedef struct {
    uint16_t connectionHandle;
    uint16_t count;
} HciIsoTxPackets;

static List *g_hciIsoCallbackList = NULL;
static Mutex *g_hciIsoCallbackListLock = NULL;

// Free Controller buffer space for HCI ISO Data packets (LE-S/LE-F), one credit
// per packet; drained by HCI_SendIsoData and refilled by Number of Completed Packets.
static uint16_t g_numOfIsoDataPackets = 0;
static Mutex *g_numOfIsoDataPacketsLock = NULL;
// The Controller-reported LE-S/LE-F buffer count (Read Buffer Size V2), written
// under g_numOfIsoDataPacketsLock together with the running credit. Refills are
// clamped to this ceiling so an errant Number of Completed Packets (duplicate
// report, or a late NCP after disconnect recovery) can never inflate the credit
// beyond what the Controller actually owns.
static uint16_t g_isoDataPacketsTotal = 0;

// ISO connection handles that are currently established, used to filter the
// Number of Completed Packets event list down to ISO handles.
static List *g_isoHandleList = NULL;
static Mutex *g_isoHandleListLock = NULL;

// Per-handle count of in-flight ISO packets, recovered on disconnect.
// All access points (send, completion, disconnect recovery) hold g_numOfIsoDataPacketsLock.
static List *g_txIsoPackets = NULL;

static bool HciIsoFindHandleByHandle(void *data, void *parameter)
{
    uint16_t *handle = data;
    uint16_t connectionHandle = *(uint16_t *)parameter;
    return *handle == connectionHandle;
}

static bool HciIsoFindTxPacketsByHandle(void *data, void *parameter)
{
    HciIsoTxPackets *entity = data;
    uint16_t connectionHandle = *(uint16_t *)parameter;
    return entity->connectionHandle == connectionHandle;
}

static void HciIsoFreeHandle(void *data)
{
    MEM_MALLOC.free(data);
}

static void HciIsoFreeTxPackets(void *data)
{
    MEM_MALLOC.free(data);
}

static void HciCleanupIso(void);

void HciInitIso(void)
{
    g_hciIsoCallbackList = ListCreate(NULL);
    g_hciIsoCallbackListLock = MutexCreate();

    g_numOfIsoDataPacketsLock = MutexCreate();
    g_isoHandleList = ListCreate(HciIsoFreeHandle);
    g_isoHandleListLock = MutexCreate();
    g_txIsoPackets = ListCreate(HciIsoFreeTxPackets);
    if (g_hciIsoCallbackList == NULL || g_hciIsoCallbackListLock == NULL ||
        g_numOfIsoDataPacketsLock == NULL || g_isoHandleList == NULL ||
        g_isoHandleListLock == NULL || g_txIsoPackets == NULL) {
        // Resource creation failed (out of memory): tear down whatever was
        // created so a later HciCloseIso/HciInitIso cannot double-create or
        // operate on a half-initialized module.
        LOG_ERROR("%{public}s: list/mutex creation failed, ISO module unavailable", __FUNCTION__);
        HciCleanupIso();
    }
}

static void HciCleanupIso(void)
{
    if (g_hciIsoCallbackList != NULL) {
        ListDelete(g_hciIsoCallbackList);
        g_hciIsoCallbackList = NULL;
    }
    if (g_hciIsoCallbackListLock != NULL) {
        MutexDelete(g_hciIsoCallbackListLock);
        g_hciIsoCallbackListLock = NULL;
    }
    if (g_numOfIsoDataPacketsLock != NULL) {
        MutexDelete(g_numOfIsoDataPacketsLock);
        g_numOfIsoDataPacketsLock = NULL;
    }
    if (g_isoHandleList != NULL) {
        ListDelete(g_isoHandleList);
        g_isoHandleList = NULL;
    }
    if (g_isoHandleListLock != NULL) {
        MutexDelete(g_isoHandleListLock);
        g_isoHandleListLock = NULL;
    }
    if (g_txIsoPackets != NULL) {
        ListDelete(g_txIsoPackets);
        g_txIsoPackets = NULL;
    }
}

void HciCloseIso(void)
{
    HciCleanupIso();
}

// HciInitIso failure (out of memory) tears the module down to the all-NULL
// state without disabling its entry points, so every path that reaches a lock
// or list must check readiness instead of MutexLock(NULL) (which asserts in
// the platform layer and would take the whole stack down). Mirrors the
// "failed -> disabled, NULL-guarded entries" pattern of iso.c.
static bool HciIsoIsReady(void)
{
    return g_hciIsoCallbackList != NULL && g_hciIsoCallbackListLock != NULL &&
        g_isoHandleList != NULL && g_isoHandleListLock != NULL &&
        g_numOfIsoDataPacketsLock != NULL && g_txIsoPackets != NULL;
}

int HCI_RegisterIsoCallbacks(const HciIsoCallbacks *callbacks)
{
    if (callbacks == NULL) {
        return BT_BAD_PARAM;
    }
    if (!HciIsoIsReady()) {
        LOG_ERROR("%{public}s: ISO module unavailable (init failed)", __FUNCTION__);
        return BT_OPERATION_FAILED;
    }
    MutexLock(g_hciIsoCallbackListLock);
    ListAddLast(g_hciIsoCallbackList, (void *)callbacks);
    MutexUnlock(g_hciIsoCallbackListLock);

    return BT_SUCCESS;
}

int HCI_DeregisterIsoCallbacks(const HciIsoCallbacks *callbacks)
{
    if (callbacks == NULL) {
        return BT_BAD_PARAM;
    }
    if (!HciIsoIsReady()) {
        LOG_ERROR("%{public}s: ISO module unavailable (init failed)", __FUNCTION__);
        return BT_OPERATION_FAILED;
    }
    MutexLock(g_hciIsoCallbackListLock);
    ListRemoveNode(g_hciIsoCallbackList, (void *)callbacks);
    MutexUnlock(g_hciIsoCallbackListLock);

    return BT_SUCCESS;
}

void HciIsoSetIsoDataPackets(uint16_t totalPackets)
{
    if (!HciIsoIsReady()) {
        LOG_ERROR("%{public}s: ISO module unavailable (init failed)", __FUNCTION__);
        return;
    }
    MutexLock(g_numOfIsoDataPacketsLock);
    g_isoDataPacketsTotal = totalPackets;
    g_numOfIsoDataPackets = totalPackets;
    MutexUnlock(g_numOfIsoDataPacketsLock);
}

// Refills the credit with completed packets, clamped to the Controller-reported
// total (caller must hold g_numOfIsoDataPacketsLock).
static void HciIsoRestoreCredit(uint16_t completed)
{
    uint32_t credited = (uint32_t)g_numOfIsoDataPackets + completed;
    g_numOfIsoDataPackets = (credited > g_isoDataPacketsTotal) ? g_isoDataPacketsTotal : (uint16_t)credited;
}

uint16_t HciIsoGetAvailableIsoDataPackets(void)
{
    if (!HciIsoIsReady()) {
        return 0;
    }
    MutexLock(g_numOfIsoDataPacketsLock);
    uint16_t available = g_numOfIsoDataPackets;
    MutexUnlock(g_numOfIsoDataPacketsLock);
    return available;
}

void HciIsoRegisterHandle(uint16_t connectionHandle)
{
    if (!HciIsoIsReady()) {
        return;
    }
    MutexLock(g_isoHandleListLock);
    if (ListForEachData(g_isoHandleList, HciIsoFindHandleByHandle, &connectionHandle) == NULL) {
        uint16_t *handle = MEM_MALLOC.alloc(sizeof(uint16_t));
        if (handle != NULL) {
            *handle = connectionHandle;
            ListAddLast(g_isoHandleList, handle);
        }
    }
    MutexUnlock(g_isoHandleListLock);
}

static void HciIsoRecoveryNumOfIsoDataPackets(uint16_t connectionHandle)
{
    MutexLock(g_numOfIsoDataPacketsLock);
    HciIsoTxPackets *entity = ListForEachData(g_txIsoPackets, HciIsoFindTxPacketsByHandle, &connectionHandle);
    if (entity != NULL) {
        // Recover the credit for packets the Controller never completed for a
        // disconnected link; otherwise the credit would be lost permanently.
        HciIsoRestoreCredit(entity->count);
        ListRemoveNode(g_txIsoPackets, entity);
    }
    MutexUnlock(g_numOfIsoDataPacketsLock);
}

void HciIsoDeregisterHandle(uint16_t connectionHandle)
{
    if (!HciIsoIsReady()) {
        return;
    }
    MutexLock(g_isoHandleListLock);
    uint16_t *handle = ListForEachData(g_isoHandleList, HciIsoFindHandleByHandle, &connectionHandle);
    if (handle != NULL) {
        ListRemoveNode(g_isoHandleList, handle);
    }
    MutexUnlock(g_isoHandleListLock);

    HciIsoRecoveryNumOfIsoDataPackets(connectionHandle);
}

static bool HciIsoAddTxPacket(uint16_t connectionHandle)
{
    HciIsoTxPackets *entity = ListForEachData(g_txIsoPackets, HciIsoFindTxPacketsByHandle, &connectionHandle);
    if (entity == NULL) {
        entity = MEM_MALLOC.alloc(sizeof(HciIsoTxPackets));
        if (entity != NULL) {
            entity->connectionHandle = connectionHandle;
            entity->count = 0;
            ListAddLast(g_txIsoPackets, entity);
        }
    }
    if (entity == NULL) {
        return false;
    }
    entity->count++;
    return true;
}

static void HciIsoOnTxPacketComplete(uint16_t connectionHandle, uint16_t count)
{
    HciIsoTxPackets *entity = ListForEachData(g_txIsoPackets, HciIsoFindTxPacketsByHandle, &connectionHandle);
    if (entity != NULL) {
        entity->count = (entity->count >= count) ? (uint16_t)(entity->count - count) : 0;
    }
}

void HciIsoOnNumberOfCompletedPackets(uint8_t numberOfHandles, const HciNumberOfCompletedPackets *list)
{
    if (list == NULL) {
        return;
    }
    if (!HciIsoIsReady()) {
        return;
    }
    for (uint8_t i = 0; i < numberOfHandles; i++) {
        uint16_t connectionHandle = list[i].connectionHandle;
        // Membership check and credit refill share one critical section (fixed
        // lock order: g_isoHandleListLock -> g_numOfIsoDataPacketsLock). A
        // concurrent HciIsoDeregisterHandle must not run between the check and
        // the refill: it recovers the full in-flight credit of the handle and
        // deletes its tracking entry, and a refill after that would credit the
        // same packets twice. All callers run on the single Stack thread today,
        // but the locks document the contract for a future threaded model.
        MutexLock(g_isoHandleListLock);
        bool isIsoHandle = (ListForEachData(g_isoHandleList, HciIsoFindHandleByHandle, &connectionHandle) != NULL);
        if (isIsoHandle) {
            MutexLock(g_numOfIsoDataPacketsLock);
            // Vol 4, Part E, 4.1.1: the Controller sends Number of Completed
            // Packets once a buffer becomes free; the Host adds the completed
            // count back (clamped to the reported total).
            HciIsoRestoreCredit(list[i].numOfCompletedPackets);
            HciIsoOnTxPacketComplete(connectionHandle, list[i].numOfCompletedPackets);
            MutexUnlock(g_numOfIsoDataPacketsLock);
        }
        MutexUnlock(g_isoHandleListLock);
    }
}

int HCI_SendIsoData(uint16_t handle, uint8_t pbFlag, uint8_t tsFlag, Packet *packet)
{
    // Reject out-of-range fields instead of letting the header bit fields below
    // silently truncate them: 12-bit connection handle range, PB_Flag 0b00-0b11
    // and TS_Flag 0-1 (Vol 4, Part E, 5.4.5); ISO_Data_Load_Length is 14 bits,
    // so an oversized load is rejected instead of truncated.
    if (packet == NULL || handle > 0x0EFF || pbFlag > 0x03 || tsFlag > 0x01 ||
        PacketPayloadSize(packet) > ISO_DATA_LOAD_LENGTH_MAX) {
        return BT_BAD_PARAM;
    }
    if (!HciIsoIsReady()) {
        LOG_ERROR("%{public}s: ISO module unavailable (init failed)", __FUNCTION__);
        return BT_OPERATION_FAILED;
    }

    HciPacket *hciPacket = MEM_MALLOC.alloc(sizeof(HciPacket));
    if (hciPacket == NULL) {
        return BT_NO_MEMORY;
    }

    Packet *isoPacket = PacketInheritMalloc(packet, sizeof(HciIsoDataHeader), 0);
    if (isoPacket == NULL) {
        MEM_MALLOC.free(hciPacket);
        return BT_NO_MEMORY;
    }

    Buffer *headerBuffer = PacketHead(isoPacket);
    HciIsoDataHeader *header = BufferPtr(headerBuffer);
    header->handle = handle;
    header->pbFlag = pbFlag;
    header->tsFlag = tsFlag;
    header->loadLength = PacketPayloadSize(isoPacket);

    MutexLock(g_numOfIsoDataPacketsLock);
    if (g_numOfIsoDataPackets == 0) {
        // Vol 4, Part E, 4.1.1: the Host shall assume the free buffer space for LE-S/LE-F
        // has decreased by one per packet sent; reject when no Controller buffer remains.
        MutexUnlock(g_numOfIsoDataPacketsLock);
        MEM_MALLOC.free(hciPacket);
        PacketFree(isoPacket);
        return BT_OPERATION_FAILED;
    }
    g_numOfIsoDataPackets--;
    if (!HciIsoAddTxPacket(handle)) {
        // Without in-flight tracking the disconnected-link credit could not be
        // recovered; do not send an untracked packet (Vol 4, Part E, 4.1.1).
        g_numOfIsoDataPackets++;
        MutexUnlock(g_numOfIsoDataPacketsLock);
        MEM_MALLOC.free(hciPacket);
        PacketFree(isoPacket);
        return BT_NO_MEMORY;
    }
    MutexUnlock(g_numOfIsoDataPacketsLock);

    hciPacket->type = H2C_ISODATA;
    hciPacket->packet = isoPacket;
    HciPushToTxQueue(hciPacket);

    return BT_SUCCESS;
}

void HciOnIsoData(Packet *packet)
{
    // HCI ISO Data packet header is 4 octets; drop shorter packets (Vol 4, Part E, 5.4.5).
    if (packet == NULL || PacketPayloadSize(packet) < sizeof(HciIsoDataHeader)) {
        return;
    }
    if (!HciIsoIsReady()) {
        return;
    }
    HciIsoDataHeader header;
    PacketExtractHead(packet, (uint8_t *)&header, sizeof(header));

    // The controller-stated ISO_Data_Load_Length (14-bit header field) must
    // equal the load that actually follows the header (Vol 4, Part E, 5.4.5);
    // a mismatch means a truncated/corrupt HCI packet, and forwarding it would
    // feed the SDU reassembler a wrong-length segment. Drop with a log instead.
    if (header.loadLength != PacketPayloadSize(packet)) {
        LOG_WARN("%{public}s: ISO data load length mismatch: header=%u, payload=%u, dropping",
            __FUNCTION__, header.loadLength, PacketPayloadSize(packet));
        return;
    }

    // g_hciIsoCallbackListLock is not recursive and the traversal runs the
    // callbacks inline under the lock: a callback must neither call
    // HCI_DeregisterIsoCallbacks (self-deadlock) nor register/unregister any
    // entry while running (the node under it could be freed). Deregister outside
    // the callback. The sole registered module (ISO data path) never
    // unregisters from a callback; if a future module needs to, collect a
    // snapshot of the list under the lock and run the callbacks after release.
    MutexLock(g_hciIsoCallbackListLock);

    HciIsoCallbacks *callback = NULL;
    ListNode *node = ListGetFirstNode(g_hciIsoCallbackList);
    while (node != NULL) {
        callback = ListGetNodeData(node);
        if (callback != NULL) {
            if (callback->onIsoData != NULL) {
                callback->onIsoData(header.handle, header.pbFlag & 0x3, header.tsFlag & 0x1, packet);
            }
        }
        node = ListGetNextNode(node);
    }

    MutexUnlock(g_hciIsoCallbackListLock);
}
