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

#include "gap_le.h"
#include "gap_internal.h"

#include <securec.h>

#include "allocator.h"
#include "log.h"
#include "thread.h"
#include "platform/include/event.h"
#include "platform/include/mutex.h"

#include <sched.h>

#include "btm/btm_le_sec.h"
#include "btm/btm_thread.h"
#include "hci/hci.h"
#include "hci/hci_error.h"
#include "smp/smp.h"

typedef enum {
    ADV_REPORT,
    EXTENDED_ADV_REPORT,
    DIRECTED_ADV_REPORT,
} AdvReportType;

typedef struct {
    AdvReportType reportType;
    void *report;
    bool processing;
    BtmIdentityResolvingKey *IRKList;
    uint16_t listCount;
    uint16_t resolveIndex;
    BtAddr addr;
    bool doCallback;
} AdvReportRPAResolveInfo;

typedef struct {
    GapScanCallback callback;
    void *context;
} LeScanCallback;

typedef struct {
    GapExScanCallback callback;
    void *context;
} LeExScanCallback;

static LeScanCallback g_leScanCallback;
static LeExScanCallback g_leExScanCallback;

void GapFreeReportRPAResolveInfo(void *data)
{
    AdvReportRPAResolveInfo *info = data;
    switch (info->reportType) {
        case ADV_REPORT: {
            HciLeAdvertisingReport *report = info->report;
            MEM_MALLOC.free(report->data);
            break;
        }
        case EXTENDED_ADV_REPORT: {
            HciLeExtendedAdvertisingReport *report = info->report;
            MEM_MALLOC.free(report->data);
            break;
        }
        case DIRECTED_ADV_REPORT:
        default:
            break;
    }
    MEM_MALLOC.free(info->report);
    MEM_MALLOC.free(info->IRKList);
    MEM_MALLOC.free(info);
}

int GAP_RegisterScanCallback(const GapScanCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "NULL");
    if (callback == NULL) {
        (void)memset_s(
            &g_leScanCallback.callback, sizeof(g_leScanCallback.callback), 0x00, sizeof(g_leScanCallback.callback));
    } else {
        g_leScanCallback.callback = *callback;
    }
    g_leScanCallback.context = context;
    return GAP_SUCCESS;
}

int GAP_DeregisterScanCallback(void)
{
    (void)memset_s(
        &g_leScanCallback.callback, sizeof(g_leScanCallback.callback), 0x00, sizeof(g_leScanCallback.callback));
    g_leScanCallback.context = NULL;
    return GAP_SUCCESS;
}

int GAP_LeScanSetParam(GapLeScanParam param, uint8_t scanFilterPolity)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        HciLeSetScanParametersParam hciCmdParam = {
            .leScanType = param.scanType,
            .leScanInterval = param.param.scanInterval,
            .leScanWindow = param.param.scanWindow,
            .ownAddressType = BTM_GetOwnAddressType(),
            .scanningFilterPolicy = scanFilterPolity,
        };
        ret = HCI_LeSetScanParameters(&hciCmdParam);
    }

    return ret;
}

void GapLeScanSetParamComplete(const HciLeSetExtendedScanParametersReturnParam *param)
{
    if (g_leScanCallback.callback.scanSetParamResult) {
        g_leScanCallback.callback.scanSetParamResult(param->status, g_leScanCallback.context);
    }
}

static void GapLeSetExAdvReportParam(GapExAdvReportParam *advParam, const HciLeExtendedAdvertisingReport *report)
{
    if (advParam != NULL && report != NULL) {
        advParam->advertisingSid = report->advertisingSID;
        advParam->data = report->data;
        advParam->dataLen = report->dataLength;
        advParam->periodicAdvInterval = report->periodicAdvertisingInterval;
        advParam->primaryPhy = report->primaryPHY;
        advParam->rssi = report->rssi;
        advParam->secondaryPhy = report->secondaryPHY;
        advParam->txPower = report->txPower;
    }
}

static void GapCallbackRPAAdvertisingReport(const AdvReportRPAResolveInfo *info, const BtAddr *currentAddr)
{
    if (info == NULL) {
        LOG_WARN("%{public}s: miss info", __FUNCTION__);
        return;
    }

    HciLeAdvertisingReport *report = info->report;
    if (report == NULL || (report->lengthData != 0 && report->data == NULL)) {
        LOG_WARN("%{public}s: miss report or data", __FUNCTION__);
        return;
    }
    LOG_INFO("%{public}s:" BT_ADDR_FMT " -> " BT_ADDR_FMT,
        __FUNCTION__,
        BT_ADDR_FMT_OUTPUT(report->address.raw),
        BT_ADDR_FMT_OUTPUT(info->addr.addr));
    if (g_leScanCallback.callback.advertisingReport) {
        GapAdvReportParam reportParam = {
            .dataLen = report->lengthData,
            .data = report->data,
            .rssi = report->rssi,
        };
        g_leScanCallback.callback.advertisingReport(
            report->eventType, &info->addr, reportParam, currentAddr, g_leScanCallback.context);
    }
}

static void GapCallbackRPAExtendedAdvertisingReport(const AdvReportRPAResolveInfo *info, const BtAddr *currentAddr)
{
    if (info == NULL) {
        LOG_WARN("%{public}s: miss info.", __FUNCTION__);
        return;
    }

    HciLeExtendedAdvertisingReport *report = info->report;
    GapExAdvReportParam advParam;
    BtAddr directAddr;

    if (report == NULL || ((report->dataLength != 0) && (report->data == NULL))) {
        LOG_WARN("%{public}s: miss report or data", __FUNCTION__);
        return;
    }

    GapLeSetExAdvReportParam(&advParam, report);
    advParam.directAddr = &directAddr;
    GapChangeHCIAddr(&directAddr, &report->directAddress, report->directAddressType);

    LOG_INFO("%{public}s:" BT_ADDR_FMT " -> " BT_ADDR_FMT,
        __FUNCTION__,
        BT_ADDR_FMT_OUTPUT(report->address.raw),
        BT_ADDR_FMT_OUTPUT(info->addr.addr));
    if (g_leExScanCallback.callback.exAdvertisingReport) {
        g_leExScanCallback.callback.exAdvertisingReport(
            report->eventType, &info->addr, advParam, currentAddr, g_leExScanCallback.context);
    }
}

static void GapCallbackRPADirectedAdvertisingReport(const AdvReportRPAResolveInfo *info, const BtAddr *currentAddr)
{
    if (info == NULL || info->report == NULL) {
        LOG_WARN("%{public}s: miss info or report", __FUNCTION__);
        return;
    }

    HciLeDirectedAdvertisingReport *report = info->report;
    BtAddr directAddr;
    GapChangeHCIAddr(&directAddr, &report->directAddress, report->directAddressType);
    LOG_INFO("%{public}s:" BT_ADDR_FMT " -> " BT_ADDR_FMT,
        __FUNCTION__,
        BT_ADDR_FMT_OUTPUT(report->address.raw),
        BT_ADDR_FMT_OUTPUT(info->addr.addr));
    if (g_leExScanCallback.callback.directedAdvertisingReport) {
        GapDirectedAdvReportParam reportParam = {
            .directAddr = &directAddr,
            .rssi = report->rssi,
        };
        g_leExScanCallback.callback.directedAdvertisingReport(
            report->eventType, &info->addr, reportParam, currentAddr, g_leExScanCallback.context);
    }
}

void GapDoCallbackRPAAdvertisingReport(void *data, const BtAddr *currentAddr)
{
    AdvReportRPAResolveInfo *info = data;
    switch (info->reportType) {
        case ADV_REPORT: {
            GapCallbackRPAAdvertisingReport(info, currentAddr);
            break;
        }
        case EXTENDED_ADV_REPORT: {
            GapCallbackRPAExtendedAdvertisingReport(info, currentAddr);
            break;
        }
        case DIRECTED_ADV_REPORT: {
            GapCallbackRPADirectedAdvertisingReport(info, currentAddr);
            break;
        }
        default:
            break;
    }
    ListRemoveNode(GapGetLeRandomAddressBlock()->reportRPAResolveList, data);
}

void GapRPAResolveProcess(void)
{
    int ret;

    ListNode *node = ListGetFirstNode(GapGetLeRandomAddressBlock()->reportRPAResolveList);
    while (node != 0) {
        AdvReportRPAResolveInfo *info = ListGetNodeData(node);

        if (info->processing) {
            break;
        }

        if (!info->doCallback) {
            LOG_DEBUG("%{public}s: " BT_ADDR_FMT " start resolve RPA",
                __FUNCTION__, BT_ADDR_FMT_OUTPUT(info->addr.addr));

            uint8_t addr[BT_ADDRESS_SIZE];
            (void)memcpy_s(addr, BT_ADDRESS_SIZE, info->addr.addr, BT_ADDRESS_SIZE);
            const uint8_t *addrPtr = addr;
            const uint8_t *keyPtr = info->IRKList[info->resolveIndex].irk.key;
            ret = SMP_AsyncResolveRPA(addrPtr, keyPtr);
            if (ret != BT_SUCCESS) {
                info->doCallback = true;
                GapDoCallbackRPAAdvertisingReport(info, NULL);
            } else {
                info->processing = true;
            }
            break;
        }
        node = ListGetNextNode(node);
    }
}

void GapResolveRPAResult(uint8_t status, bool result, const uint8_t *addr, const uint8_t *irk)
{
    LOG_INFO("%{public}s: status:%02x, result:%02x", __FUNCTION__, status, result);
    ListNode *node = ListGetFirstNode(GapGetLeRandomAddressBlock()->reportRPAResolveList);
    while (node != 0) {
        AdvReportRPAResolveInfo *info = ListGetNodeData(node);
        BtAddr currentAddr = info->addr;
        node = ListGetNextNode(node);
        if (!info->processing) {
            continue;
        }

        info->processing = false;
        if (status == SMP_PAIR_STATUS_SUCCESS && result == true) {
            if (memcmp(info->IRKList[info->resolveIndex].irk.key, irk, GAP_IRK_SIZE)) {
                LOG_ERROR("%{public}s: IRK mismatch", __FUNCTION__);
            } else {
                BTM_UpdateCurrentRemoteAddress(&info->IRKList[info->resolveIndex].addr, &info->addr);
                (void)memcpy_s(&info->addr, sizeof(BtAddr), &info->IRKList[info->resolveIndex].addr, sizeof(BtAddr));
            }
            info->doCallback = true;
        } else {
            info->resolveIndex++;
            if (info->resolveIndex == info->listCount) {
                info->doCallback = true;
            }
        }
        if (info->doCallback) {
            GapDoCallbackRPAAdvertisingReport(info, result ? &currentAddr : NULL);
        }
        break;
    }

    GapRPAResolveProcess();
}

// Duplicate the advertising data payload of an adv report event. Returns a
// freshly allocated copy, or NULL when the event carries no payload; on
// allocation failure sets *dupFailed so the caller can abort the resolve-info
// allocation exactly like the original inline code did.
static uint8_t *GapLeDupAdvData(uint8_t dataLen, const uint8_t *data, bool *dupFailed)
{
    *dupFailed = false;
    // Guard against a malformed event where dataLen > 0 but data == NULL:
    // skipping the copy keeps the report data NULL so consumers treat it as
    // no data instead of copying garbage into an uninitialized buffer.
    if (dataLen == 0 || data == NULL) {
        return NULL;
    }
    uint8_t *dupData = MEM_MALLOC.alloc(dataLen);
    if (dupData == NULL) {
        *dupFailed = true;
        return NULL;
    }
    (void)memcpy_s(dupData, dataLen, data, dataLen);
    return dupData;
}

// Allocate and fill the report struct for one report type, including a private
// copy of its payload. Returns GAP_SUCCESS on success; on failure the caller
// frees the resolve-info block.
static int GapLeAllocAdvReportData(AdvReportRPAResolveInfo *info, AdvReportType type, const void *advReport)
{
    if (type == ADV_REPORT) {
        const HciLeAdvertisingReport *src = (const HciLeAdvertisingReport *)advReport;
        HciLeAdvertisingReport *report = MEM_MALLOC.alloc(sizeof(HciLeAdvertisingReport));
        if (report == NULL) {
            return GAP_ERR_OUT_OF_RES;
        }
        (void)memcpy_s(report, sizeof(HciLeAdvertisingReport), src, sizeof(HciLeAdvertisingReport));
        bool dupFailed = false;
        report->data = GapLeDupAdvData(src->lengthData, src->data, &dupFailed);
        if (dupFailed) {
            LOG_ERROR("%{public}s: alloc advData failed", __FUNCTION__);
            MEM_MALLOC.free(report);
            return GAP_ERR_OUT_OF_RES;
        }
        info->report = report;
    } else if (type == EXTENDED_ADV_REPORT) {
        const HciLeExtendedAdvertisingReport *src = (const HciLeExtendedAdvertisingReport *)advReport;
        HciLeExtendedAdvertisingReport *report = MEM_MALLOC.alloc(sizeof(HciLeExtendedAdvertisingReport));
        if (report == NULL) {
            return GAP_ERR_OUT_OF_RES;
        }
        (void)memcpy_s(report, sizeof(HciLeExtendedAdvertisingReport), src, sizeof(HciLeExtendedAdvertisingReport));
        bool dupFailed = false;
        report->data = GapLeDupAdvData(src->dataLength, src->data, &dupFailed);
        if (dupFailed) {
            LOG_ERROR("%{public}s: alloc advData failed", __FUNCTION__);
            MEM_MALLOC.free(report);
            return GAP_ERR_OUT_OF_RES;
        }
        info->report = report;
    } else if (type == DIRECTED_ADV_REPORT) {
        HciLeDirectedAdvertisingReport *report = MEM_MALLOC.alloc(sizeof(HciLeDirectedAdvertisingReport));
        if (report == NULL) {
            return GAP_ERR_OUT_OF_RES;
        }
        (void)memcpy_s(report, sizeof(HciLeDirectedAdvertisingReport), advReport,
            sizeof(HciLeDirectedAdvertisingReport));
        info->report = report;
    } else {
        return GAP_ERR_INVAL_PARAM;
    }
    return GAP_SUCCESS;
}

static AdvReportRPAResolveInfo *GapLeAllocAdvReportRPAResolveInfo(
    BtAddr addr, AdvReportType type, const void *advReport)
{
    AdvReportRPAResolveInfo *info = MEM_MALLOC.alloc(sizeof(AdvReportRPAResolveInfo));
    if (info == NULL) {
        LOG_ERROR("%{public}s: alloc info failed", __FUNCTION__);
        return NULL;
    }
    (void)memset_s(info, sizeof(AdvReportRPAResolveInfo), 0x00, sizeof(AdvReportRPAResolveInfo));
    (void)memcpy_s(&info->addr, sizeof(BtAddr), &addr, sizeof(BtAddr));
    info->reportType = type;
    info->resolveIndex = 0;
    info->processing = false;
    info->doCallback = false;

    if (GapLeAllocAdvReportData(info, type, advReport) != GAP_SUCCESS) {
        MEM_MALLOC.free(info);
        return NULL;
    }

    return info;
}

static bool GapTryChangeAddressForIdentityAddress(BtAddr *addr)
{
    BtAddr pairedAddr = {0};
    int ret = BTM_GetPairdAddressFromRemoteIdentityAddress(addr, &pairedAddr);
    if (ret == BT_SUCCESS) {
        LOG_INFO("%{public}s:" BT_ADDR_FMT " -> " BT_ADDR_FMT,
            __FUNCTION__,
            BT_ADDR_FMT_OUTPUT(addr->addr),
            BT_ADDR_FMT_OUTPUT(pairedAddr.addr));
        *addr = pairedAddr;
        return true;
    }
    return false;
}

static void GapOnLeAdvertisingReportEventProcessOnce(const HciLeAdvertisingReport *report)
{
    BtAddr addr;
    GapChangeHCIAddr(&addr, &report->address, report->addressType);
    BtAddr currentAddr = addr;
    uint8_t advType = report->eventType;
    int8_t rssi = report->rssi;
    uint8_t dataLen = report->lengthData;
    uint8_t *data = report->data;

    if (GapAddrIsResolvablePrivateAddress(&addr)) {
        BtmIdentityResolvingKey *deviceIRKList = NULL;
        uint16_t listCount = 0;
        int ret = BTM_GetAllRemoteIdentityResolvingKey(&deviceIRKList, &listCount);
        if (ret == BT_SUCCESS && listCount != 0) {
            AdvReportRPAResolveInfo *info = GapLeAllocAdvReportRPAResolveInfo(addr, ADV_REPORT, report);
            if (info != NULL) {
                info->IRKList = deviceIRKList;
                info->listCount = listCount;
                ListAddLast(GapGetLeRandomAddressBlock()->reportRPAResolveList, info);
                GapRPAResolveProcess();
                return;
            }
        }
        if (deviceIRKList != NULL) {
            MEM_MALLOC.free(deviceIRKList);
        }
    } else if (GapAddrIsIdentityAddress(&addr)) {
        GapTryChangeAddressForIdentityAddress(&addr);
    } else if (GapAddrIsStaticAddress(&addr) || GapAddrIsPublicAddress(&addr)) {
        if (GapTryChangeAddressForIdentityAddress(&addr)) {
            BTM_UpdateCurrentRemoteAddress(&addr, &currentAddr);
        }
    }

    LOG_INFO("%{public}s:" BT_ADDR_FMT " type=%hhu", __FUNCTION__, BT_ADDR_FMT_OUTPUT(addr.addr), addr.type);
    if (g_leScanCallback.callback.advertisingReport) {
        GapAdvReportParam reportParam = {
            .dataLen = dataLen,
            .data = data,
            .rssi = rssi,
        };
        g_leScanCallback.callback.advertisingReport(advType, &addr, reportParam, NULL, g_leScanCallback.context);
    }
}

void GapOnLeAdvertisingReportEvent(const HciLeAdvertisingReportEventParam *eventParam)
{
    for (uint8_t i = 0; i < eventParam->numReports; i++) {
        GapOnLeAdvertisingReportEventProcessOnce(&eventParam->reports[i]);
    }
}

int GAP_LeScanSetEnable(uint8_t scanEnable, uint8_t filterDuplicates)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        HciLeSetScanEnableParam hciCmdParam = {
            .leScanEnable = scanEnable,
            .filterDuplicates = filterDuplicates,
        };
        ret = HCI_LeSetScanEnable(&hciCmdParam);
    }

    return ret;
}

void GapLeScanSetEnableComplete(const HciLeSetScanEnableReturnParam *param)
{
    if (g_leScanCallback.callback.scanSetEnableResult) {
        g_leScanCallback.callback.scanSetEnableResult(param->status, g_leScanCallback.context);
    }
}

int GAP_RegisterExScanCallback(const GapExScanCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "NULL");
    if (callback == NULL) {
        (void)memset_s(&g_leExScanCallback.callback,
            sizeof(g_leExScanCallback.callback),
            0x00,
            sizeof(g_leExScanCallback.callback));
    } else {
        g_leExScanCallback.callback = *callback;
    }
    g_leExScanCallback.context = context;
    return GAP_SUCCESS;
}

int GAP_DeregisterExScanCallback(void)
{
    (void)memset_s(
        &g_leExScanCallback.callback, sizeof(g_leExScanCallback.callback), 0x00, sizeof(g_leExScanCallback.callback));
    g_leExScanCallback.context = NULL;
    return GAP_SUCCESS;
}

NO_SANITIZE("cfi")
static void GapOnLeExtendedAdvertisingReportEventProcessOnce(const HciLeExtendedAdvertisingReport *report)
{
    BtAddr addr;
    GapChangeHCIAddr(&addr, &report->address, report->addressType);
    BtAddr currentAddr = addr;
    uint8_t advType = report->eventType;
    GapExAdvReportParam advParam;
    BtAddr directAddr;
    GapLeSetExAdvReportParam(&advParam, report);
    GapChangeHCIAddr(&directAddr, &report->directAddress, report->directAddressType);
    advParam.directAddr = &directAddr;

    if (GapAddrIsResolvablePrivateAddress(&addr)) {
        BtmIdentityResolvingKey *deviceIRKList = NULL;
        uint16_t listCount = 0;
        int ret = BTM_GetAllRemoteIdentityResolvingKey(&deviceIRKList, &listCount);
        if (ret == BT_SUCCESS && listCount != 0) {
            AdvReportRPAResolveInfo *info = GapLeAllocAdvReportRPAResolveInfo(addr, EXTENDED_ADV_REPORT, report);
            if (info != NULL) {
                info->IRKList = deviceIRKList;
                info->listCount = listCount;
                ListAddLast(GapGetLeRandomAddressBlock()->reportRPAResolveList, info);
                GapRPAResolveProcess();
                return;
            }
        }
        if (deviceIRKList != NULL) {
            MEM_MALLOC.free(deviceIRKList);
        }
    } else if (GapAddrIsIdentityAddress(&addr)) {
        GapTryChangeAddressForIdentityAddress(&addr);
    } else if (GapAddrIsStaticAddress(&addr) || GapAddrIsPublicAddress(&addr)) {
        if (GapTryChangeAddressForIdentityAddress(&addr)) {
            BTM_UpdateCurrentRemoteAddress(&addr, &currentAddr);
        }
    }

    LOG_INFO("%{public}s:" BT_ADDR_FMT " type=%hhu", __FUNCTION__, BT_ADDR_FMT_OUTPUT(addr.addr), addr.type);
    if (g_leExScanCallback.callback.exAdvertisingReport) {
        g_leExScanCallback.callback.exAdvertisingReport(advType, &addr, advParam, NULL, g_leExScanCallback.context);
    }
}

void GapOnLeExtendedAdvertisingReportEvent(const HciLeExtendedAdvertisingReportEventParam *eventParam)
{
    for (uint8_t i = 0; i < eventParam->numReports; i++) {
        GapOnLeExtendedAdvertisingReportEventProcessOnce(&eventParam->reports[i]);
    }
}

static void GapOnLeDirectedAdvertisingReportProcessOnce(const HciLeDirectedAdvertisingReport *report)
{
    BtAddr addr;
    GapChangeHCIAddr(&addr, &report->address, report->addressType);
    BtAddr currentAddr = addr;
    uint8_t advType = report->eventType;
    BtAddr directAddr;
    GapChangeHCIAddr(&directAddr, &report->directAddress, report->directAddressType);
    int8_t rssi = report->rssi;

    if (GapAddrIsResolvablePrivateAddress(&addr)) {
        BtmIdentityResolvingKey *deviceIRKList = NULL;
        uint16_t listCount = 0;
        int ret = BTM_GetAllRemoteIdentityResolvingKey(&deviceIRKList, &listCount);
        if (ret == BT_SUCCESS && listCount != 0) {
            AdvReportRPAResolveInfo *info = GapLeAllocAdvReportRPAResolveInfo(addr, DIRECTED_ADV_REPORT, report);
            if (info != NULL) {
                info->IRKList = deviceIRKList;
                info->listCount = listCount;
                ListAddLast(GapGetLeRandomAddressBlock()->reportRPAResolveList, info);
                GapRPAResolveProcess();
                return;
            }
        }
        if (deviceIRKList != NULL) {
            MEM_MALLOC.free(deviceIRKList);
        }
    } else if (GapAddrIsIdentityAddress(&addr)) {
        GapTryChangeAddressForIdentityAddress(&addr);
    } else if (GapAddrIsStaticAddress(&addr) || GapAddrIsPublicAddress(&addr)) {
        if (GapTryChangeAddressForIdentityAddress(&addr)) {
            BTM_UpdateCurrentRemoteAddress(&addr, &currentAddr);
        }
    }

    LOG_INFO("%{public}s:" BT_ADDR_FMT " type=%hhu", __FUNCTION__, BT_ADDR_FMT_OUTPUT(addr.addr), addr.type);
    if (g_leExScanCallback.callback.directedAdvertisingReport) {
        GapDirectedAdvReportParam reportParam = {
            .directAddr = &directAddr,
            .rssi = rssi,
        };
        g_leExScanCallback.callback.directedAdvertisingReport(
            advType, &addr, reportParam, NULL, g_leExScanCallback.context);
    }
}

void GapOnLeDirectedAdvertisingReport(const HciLeDirectedAdvertisingReportEventParam *eventParam)
{
    for (uint8_t i = 0; i < eventParam->numReports; i++) {
        GapOnLeDirectedAdvertisingReportProcessOnce(&eventParam->reports[i]);
    }
}

void GapOnLeScanTimeoutEvent(const void *eventParam)
{
    if (g_leExScanCallback.callback.scanTimeoutEvent) {
        g_leExScanCallback.callback.scanTimeoutEvent(g_leExScanCallback.context);
    }
}

#define LE_EXTENDED_SCAN_PHYS_MASK ((1 << EXTENDED_SCAN_PHY_MAX_NUM) - 1)

static int GapLeSetExtendedScanParameters(
    uint8_t ownAddrType, uint8_t scanFilterPolity, uint8_t scanPhys, const GapLeScanParam param[])
{
    int ret;
    HciLeSetExtendedScanParametersParam hciCmdParam;
    hciCmdParam.ownAddressType = ownAddrType;
    hciCmdParam.scanningFilterPolicy = scanFilterPolity;
    hciCmdParam.scanningPhYs = scanPhys;

    if (scanPhys == 0 || (scanPhys & ~LE_EXTENDED_SCAN_PHYS_MASK) != 0) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (scanPhys != 0 && param == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    hciCmdParam.sets = MEM_MALLOC.alloc(EXTENDED_SCAN_PHY_MAX_NUM * sizeof(HciLeExtendedScanParametersSet));

    if (hciCmdParam.sets) {
        for (uint8_t i = 0, jj = 0; i < EXTENDED_SCAN_PHY_MAX_NUM; i++) {
            if ((scanPhys >> i) & 0x01) {
                hciCmdParam.sets[jj].scanInterval = param[jj].param.scanInterval;
                hciCmdParam.sets[jj].scanWindow = param[jj].param.scanWindow;
                hciCmdParam.sets[jj].scanType = param[jj].scanType;
                jj++;
            }
        }

        ret = HCI_LeSetExtendedScanParameters(&hciCmdParam);
        MEM_MALLOC.free(hciCmdParam.sets);
    } else {
        ret = GAP_ERR_OUT_OF_RES;
    }

    return ret;
}

NO_SANITIZE("cfi") void GapLeSetExtendedScanParametersComplete(const HciLeSetExtendedScanParametersReturnParam *param)
{
    if (g_leExScanCallback.callback.scanExSetParamResult) {
        g_leExScanCallback.callback.scanExSetParamResult(param->status, g_leExScanCallback.context);
    }
}

int GAP_LeExScanSetParam(uint8_t scanFilterPolity, uint8_t scanPhys, const GapLeScanParam param[])
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_CENTRAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeSetExtendedScanParameters(BTM_GetOwnAddressType(), scanFilterPolity, scanPhys, param);
    }

    return ret;
}

static int GapLeSetExtendedScanEnable(uint8_t scanEnable, uint8_t filterDuplicates, uint16_t duration, uint16_t period)
{
    HciLeSetExtendedScanEnableParam hciCmdParam = {
        .duration = duration,
        .enable = scanEnable,
        .filterDuplicates = filterDuplicates,
        .period = period,
    };

    return HCI_LeSetExtendedScanEnable(&hciCmdParam);
}

NO_SANITIZE("cfi") void GapLeSetExtendedScanEnableComplete(const HciLeSetExtendedScanEnableReturnParam *param)
{
    if (g_leExScanCallback.callback.scanExSetEnableResult) {
        g_leExScanCallback.callback.scanExSetEnableResult(param->status, g_leExScanCallback.context);
    }
}

int GAP_LeExScanSetEnable(uint8_t scanEnable, uint8_t filterDuplicates, uint16_t duration, uint16_t period)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_BROADCASTER | GAP_LE_ROLE_CENTRAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = GapLeSetExtendedScanEnable(scanEnable, filterDuplicates, duration, period);
    }

    return ret;
}

// Timeout for waiting on the periodic-adv-sync callback release event, in ms.
#define PERIODIC_SYNC_WAIT_TIMEOUT_MS 5000

typedef struct {
    GapPeriodicAdvSyncCallback callback;
    void *context;
} LePeriodicAdvSyncCallbackBlock;

static LePeriodicAdvSyncCallbackBlock g_lePeriodicAdvSyncCallback;
static Mutex *g_lePeriodicAdvSyncCallbackMutex = NULL;
// Number of threads currently holding the periodic-adv-sync mutex.
static int32_t g_lePeriodicAdvSyncCallbackRef = 0;
// Number of in-flight periodic-adv-sync callbacks. Deinit waits for this to reach zero
// before clearing callback state so that upper-layer context pointers remain valid.
static int32_t g_lePeriodicAdvSyncCallbackInFlight = 0;
// Signaled when the reference count reaches zero so Deinit can wait without spinning.
static Event *g_lePeriodicAdvSyncCallbackRefEvent = NULL;
// Serializes Init/Deinit and prevents a new Init from racing with an in-progress Deinit.
static Mutex *g_lePeriodicAdvSyncLifecycleMutex = NULL;

static bool GapLePeriodicAdvSyncTryLock(Mutex **outMutex)
{
    if (outMutex == NULL) {
        return false;
    }

    Mutex *mtx = __atomic_load_n(&g_lePeriodicAdvSyncCallbackMutex, __ATOMIC_ACQUIRE);
    if (mtx == NULL) {
        return false;
    }

    __atomic_fetch_add(&g_lePeriodicAdvSyncCallbackRef, 1, __ATOMIC_SEQ_CST);
    mtx = __atomic_load_n(&g_lePeriodicAdvSyncCallbackMutex, __ATOMIC_ACQUIRE);
    if (mtx == NULL) {
        __atomic_fetch_sub(&g_lePeriodicAdvSyncCallbackRef, 1, __ATOMIC_SEQ_CST);
        return false;
    }

    MutexLock(mtx);
    *outMutex = mtx;
    return true;
}

// Signal Deinit that a reference or in-flight counter reached zero. The
// lifecycle mutex serializes the event pointer load against
// GapLePeriodicAdvSyncClearState, which exchanges the pointer to NULL and
// deletes the event while holding the same lock. Without this, a signaller
// could load the event pointer just before Deinit deletes it and then call
// EventSet on freed memory (use-after-free). Taking the lock makes the signal
// either complete before the deletion or observe NULL and skip it.
static void GapLePeriodicAdvSyncSignalRefZero(void)
{
    Mutex *lifecycleMutex = __atomic_load_n(&g_lePeriodicAdvSyncLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        return;
    }

    MutexLock(lifecycleMutex);
    Event *event = __atomic_load_n(&g_lePeriodicAdvSyncCallbackRefEvent, __ATOMIC_ACQUIRE);
    if (event != NULL) {
        EventSet(event);
    }
    MutexUnlock(lifecycleMutex);
}

static void GapLePeriodicAdvSyncUnlock(Mutex *mtx)
{
    MutexUnlock(mtx);
    if (__atomic_fetch_sub(&g_lePeriodicAdvSyncCallbackRef, 1, __ATOMIC_SEQ_CST) == 1) {
        // Last reference released; wake any Deinit waiter.
        GapLePeriodicAdvSyncSignalRefZero();
    }
}

static void GapLePeriodicAdvSyncCallbackRelease(void)
{
    if (__atomic_fetch_sub(&g_lePeriodicAdvSyncCallbackInFlight, 1, __ATOMIC_SEQ_CST) == 1) {
        // Last in-flight callback finished; wake any Deinit waiter.
        GapLePeriodicAdvSyncSignalRefZero();
    }
}

// Returns the lifecycle mutex, creating it on first use via CAS so exactly
// one thread publishes it. Returns NULL when the mutex cannot be created.
static Mutex *GapLePeriodicAdvSyncEnsureLifecycleMutex(void)
{
    Mutex *lifecycleMutex = __atomic_load_n(&g_lePeriodicAdvSyncLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex != NULL) {
        return lifecycleMutex;
    }

    Mutex *newLifecycleMutex = MutexCreate();
    if (newLifecycleMutex == NULL) {
        LOG_ERROR("%{public}s: Lifecycle MutexCreate failed", __FUNCTION__);
        return NULL;
    }
    Mutex *expected = NULL;
    if (!__atomic_compare_exchange_n(&g_lePeriodicAdvSyncLifecycleMutex, &expected, newLifecycleMutex,
        false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
        MutexDelete(newLifecycleMutex);
        lifecycleMutex = expected;
    } else {
        lifecycleMutex = newLifecycleMutex;
    }
    return lifecycleMutex;
}

// Publish the callback mutex and its release event exactly once, under the
// lifecycle lock. All lock/unlock pairing lives in this helper.
static int GapLePeriodicAdvSyncEnsureCallbackResources(Mutex *lifecycleMutex)
{
    MutexLock(lifecycleMutex);
    if (__atomic_load_n(&g_lePeriodicAdvSyncCallbackMutex, __ATOMIC_ACQUIRE) != NULL) {
        MutexUnlock(lifecycleMutex);
        return GAP_SUCCESS;
    }

    // Create the reference-count event before publishing the mutex so that
    // no thread can observe a mutex without a working release signal.
    bool eventCreated = false;
    if (__atomic_load_n(&g_lePeriodicAdvSyncCallbackRefEvent, __ATOMIC_ACQUIRE) == NULL) {
        Event *newEvent = EventCreate(true);
        if (newEvent == NULL) {
            LOG_ERROR("%{public}s: EventCreate failed", __FUNCTION__);
            MutexUnlock(lifecycleMutex);
            return GAP_ERR_OUT_OF_RES;
        }
        Event *expectedEvent = NULL;
        eventCreated = __atomic_compare_exchange_n(&g_lePeriodicAdvSyncCallbackRefEvent, &expectedEvent, newEvent,
            false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
        if (!eventCreated) {
            EventDelete(newEvent);
        }
    }

    Mutex *newMutex = MutexCreate();
    if (newMutex == NULL) {
        LOG_ERROR("%{public}s: MutexCreate failed", __FUNCTION__);
        // If we just published a brand-new event and no mutex is published yet,
        // roll back the event so a later Init retry can recreate both resources
        // together. No thread can be using the event because no mutex exists.
        if (eventCreated) {
            Event *deletedEvent = __atomic_exchange_n(
                &g_lePeriodicAdvSyncCallbackRefEvent, NULL, __ATOMIC_ACQ_REL);
            if (deletedEvent != NULL) {
                EventDelete(deletedEvent);
            }
        }
        MutexUnlock(lifecycleMutex);
        return GAP_ERR_OUT_OF_RES;
    }

    Mutex *expected = NULL;
    bool mutexInstalled = __atomic_compare_exchange_n(&g_lePeriodicAdvSyncCallbackMutex, &expected, newMutex,
        false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    if (!mutexInstalled) {
        MutexDelete(newMutex);
    }

    MutexUnlock(lifecycleMutex);
    return GAP_SUCCESS;
}

// Wait until the atomic counter reaches zero, using the release event when
// available and a bounded spin fallback otherwise. Returns true when drained,
// false on timeout so the caller can decide whether resources may be freed.
static bool GapLePeriodicAdvSyncWaitDrain(const int32_t *counter, Event *event, const char *counterName)
{
    if (event != NULL) {
        int32_t waitRetries = 12; // 12 * PERIODIC_SYNC_WAIT_TIMEOUT_MS = 60 s
        while (__atomic_load_n(counter, __ATOMIC_ACQUIRE) != 0 && waitRetries > 0) {
            (void)EventWait(event, PERIODIC_SYNC_WAIT_TIMEOUT_MS);
            waitRetries--;
        }
        if (waitRetries == 0) {
            LOG_ERROR("%{public}s: timeout waiting for %{public}s to release", __FUNCTION__, counterName);
        }
    } else {
        int32_t spinRetries = 1000000;
        while (__atomic_load_n(counter, __ATOMIC_ACQUIRE) != 0 && spinRetries > 0) {
            sched_yield();
            spinRetries--;
        }
        if (spinRetries == 0) {
            LOG_ERROR("%{public}s: spin timeout waiting for %{public}s to release", __FUNCTION__, counterName);
        }
    }
    return (__atomic_load_n(counter, __ATOMIC_ACQUIRE) == 0);
}

// Clear the registered callback state under the callback mutex and, when all
// references have drained, delete the mutex and event. The lifecycle lock is
// held by the caller and must remain held throughout.
static void GapLePeriodicAdvSyncClearState(Mutex *mtx, Event *event, bool refsDrained, bool inFlightDrained)
{
    MutexLock(mtx);

    (void)memset_s(&g_lePeriodicAdvSyncCallback,
        sizeof(g_lePeriodicAdvSyncCallback),
        0x00,
        sizeof(g_lePeriodicAdvSyncCallback));
    MutexUnlock(mtx);

    // Only delete the mutex and event if all references and in-flight callbacks have drained.
    // Deleting an in-use mutex or an event with waiters is undefined behavior;
    // leaking them on timeout is safer than crashing.
    if (refsDrained && inFlightDrained) {
        MutexDelete(mtx);
        if (event != NULL) {
            Event *deletedEvent = __atomic_exchange_n(&g_lePeriodicAdvSyncCallbackRefEvent, NULL, __ATOMIC_ACQ_REL);
            if (deletedEvent != NULL) {
                EventDelete(deletedEvent);
            }
        }
    } else {
        LOG_ERROR("%{public}s: leaving periodic adv sync mutex/event allocated due to "
            "unreleased references or in-flight callbacks",
            __FUNCTION__);
    }
}

int GapLePeriodicAdvSyncInit(void)
{
    Mutex *lifecycleMutex = GapLePeriodicAdvSyncEnsureLifecycleMutex();
    if (lifecycleMutex == NULL) {
        return GAP_ERR_OUT_OF_RES;
    }

    return GapLePeriodicAdvSyncEnsureCallbackResources(lifecycleMutex);
}

void GapLePeriodicAdvSyncDeinit(void)
{
    Mutex *lifecycleMutex = __atomic_load_n(&g_lePeriodicAdvSyncLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        return;
    }

    MutexLock(lifecycleMutex);

    Mutex *mtx = __atomic_exchange_n(&g_lePeriodicAdvSyncCallbackMutex, NULL, __ATOMIC_ACQ_REL);
    if (mtx == NULL) {
        MutexUnlock(lifecycleMutex);
        return;
    }

    // No new reader can acquire the mutex now that the global pointer is NULL.
    // Wait until every reader that observed a non-NULL pointer has released its
    // reference. Prefer the event to avoid CPU spinning; fall back to yield if
    // the event was not created. Use a finite timeout so a leaked reference
    // cannot block teardown forever.
    Event *event = __atomic_load_n(&g_lePeriodicAdvSyncCallbackRefEvent, __ATOMIC_ACQUIRE);
    bool refsDrained = GapLePeriodicAdvSyncWaitDrain(
        &g_lePeriodicAdvSyncCallbackRef, event, "periodic adv sync callbacks");

    // Wait for any in-flight callbacks (copied but not yet finished by the upper layer)
    // to complete before clearing the callback state.
    bool inFlightDrained = GapLePeriodicAdvSyncWaitDrain(
        &g_lePeriodicAdvSyncCallbackInFlight, event, "in-flight periodic adv sync callbacks");

    GapLePeriodicAdvSyncClearState(mtx, event, refsDrained, inFlightDrained);

    MutexUnlock(lifecycleMutex);
    // Intentionally keep g_lePeriodicAdvSyncLifecycleMutex alive: other threads may
    // still be racing through register/deregister/get paths and need the lifecycle lock
    // to observe that the inner mutex is gone.
}

static bool GapLePeriodicAdvSyncGetCallback(GapPeriodicAdvSyncCallback *callback, void **context)
{
    if (callback == NULL || context == NULL) {
        return false;
    }

    Mutex *lifecycleMutex = __atomic_load_n(&g_lePeriodicAdvSyncLifecycleMutex, __ATOMIC_ACQUIRE);
    if (lifecycleMutex == NULL) {
        (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        *context = NULL;
        return false;
    }

    MutexLock(lifecycleMutex);
    Mutex *mtx = __atomic_load_n(&g_lePeriodicAdvSyncCallbackMutex, __ATOMIC_ACQUIRE);
    if (mtx == NULL) {
        MutexUnlock(lifecycleMutex);
        (void)memset_s(callback, sizeof(*callback), 0x00, sizeof(*callback));
        *context = NULL;
        return false;
    }

    MutexLock(mtx);
    *callback = g_lePeriodicAdvSyncCallback.callback;
    *context = g_lePeriodicAdvSyncCallback.context;
    (void)__atomic_fetch_add(&g_lePeriodicAdvSyncCallbackInFlight, 1, __ATOMIC_SEQ_CST);
    MutexUnlock(mtx);
    MutexUnlock(lifecycleMutex);
    return true;
}

int GAP_LeRegisterPeriodicAdvSyncCallback(const GapPeriodicAdvSyncCallback *callback, void *context)
{
    LOG_INFO("%{public}s:%{public}s", __FUNCTION__, callback ? "register" : "deregister");

    Mutex *mtx = NULL;
    if (!GapLePeriodicAdvSyncTryLock(&mtx)) {
        return GAP_ERR_OUT_OF_RES;
    }

    if (callback == NULL) {
        (void)memset_s(&g_lePeriodicAdvSyncCallback.callback,
            sizeof(g_lePeriodicAdvSyncCallback.callback),
            0x00,
            sizeof(g_lePeriodicAdvSyncCallback.callback));
        g_lePeriodicAdvSyncCallback.context = NULL;
    } else {
        LOG_INFO("%{public}s: replacing existing periodic adv sync callback", __FUNCTION__);
        // New registration replaces any previous one; this matches the
        // single-callback-slot design used by the unit tests.
        g_lePeriodicAdvSyncCallback.callback = *callback;
        g_lePeriodicAdvSyncCallback.context = context;
    }

    GapLePeriodicAdvSyncUnlock(mtx);
    return GAP_SUCCESS;
}

int GAP_LeDeregisterPeriodicAdvSyncCallback(void)
{
    Mutex *mtx = NULL;
    if (!GapLePeriodicAdvSyncTryLock(&mtx)) {
        return GAP_ERR_OUT_OF_RES;
    }

    (void)memset_s(&g_lePeriodicAdvSyncCallback.callback,
        sizeof(g_lePeriodicAdvSyncCallback.callback),
        0x00,
        sizeof(g_lePeriodicAdvSyncCallback.callback));
    g_lePeriodicAdvSyncCallback.context = NULL;

    GapLePeriodicAdvSyncUnlock(mtx);
    return GAP_SUCCESS;
}

// Validate the parameters of GAP_LePeriodicAdvCreateSync. Returns GAP_SUCCESS
// when the parameters are acceptable, or the matching GAP error code.
static int GapLePeriodicAdvCreateSyncValidate(uint8_t filterPolicy, uint8_t advSid, const BtAddr *advAddr,
    uint16_t skip, uint16_t syncTimeout)
{
    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (filterPolicy > 0x01) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (filterPolicy == 0) {
        if (advAddr == NULL) {
            return GAP_ERR_INVAL_PARAM;
        }

        if (GapIsEmptyAddr(advAddr->addr)) {
            return GAP_ERR_INVAL_PARAM;
        }

        if (advAddr->type != BT_PUBLIC_DEVICE_ADDRESS && advAddr->type != BT_RANDOM_DEVICE_ADDRESS) {
            return GAP_ERR_INVAL_PARAM;
        }
    }

    if (advSid > GAP_PERIODIC_ADV_SID_MAX) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (skip > GAP_PERIODIC_ADV_SKIP_MAX) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (syncTimeout < GAP_PERIODIC_ADV_SYNC_TIMEOUT_MIN || syncTimeout > GAP_PERIODIC_ADV_SYNC_TIMEOUT_MAX) {
        return GAP_ERR_INVAL_PARAM;
    }

    return GAP_SUCCESS;
}

int GAP_LePeriodicAdvCreateSync(uint8_t filterPolicy, uint8_t advSid, const BtAddr *advAddr, uint16_t skip,
    uint16_t syncTimeout)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    ret = GapLePeriodicAdvCreateSyncValidate(filterPolicy, advSid, advAddr, skip, syncTimeout);
    if (ret != GAP_SUCCESS) {
        return ret;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        // HCI Filter_Policy (Core Spec 5.0 Vol 2 Part E 7.8.67):
        // 0x00 = use Periodic Advertiser List, 0x01 = use Advertiser Address and SID.
        // GAP API semantics are opposite: DISABLED(0x00) means direct to the specified
        // advertiser (address required, see validation above), so map the GAP value
        // to the HCI value before sending.
        uint8_t hciFilterPolicy = (filterPolicy == GAP_PERIODIC_ADV_SYNC_FILTER_POLICY_DISABLED) ?
            HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_FILTER_POLICY_ENABLED :
            HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_FILTER_POLICY_DISABLED;
        HciLePeriodicAdvertisingCreateSyncParam hciCmdParam = {
            .filterPolicy = hciFilterPolicy,
            .advertisingSid = advSid,
            .advertiserAddressType = (advAddr != NULL) ? advAddr->type : 0,
            .skip = skip,
            .syncTimeout = syncTimeout,
            .reserved = 0,
        };
        if (advAddr != NULL) {
            int copyRet = memcpy_s(hciCmdParam.advertiserAddress.raw, BT_ADDRESS_SIZE, advAddr->addr, BT_ADDRESS_SIZE);
            if (copyRet != EOK) {
                return GAP_ERR_INVAL_PARAM;
            }
        }
        ret = HCI_LePeriodicAdvertisingCreateSync(&hciCmdParam);
    }

    return ret;
}

int GAP_LePeriodicAdvCreateSyncCancel(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else {
        ret = HCI_LePeriodicAdvertisingCreateSyncCancel();
    }

    return ret;
}

void GapLePeriodicAdvertisingCreateSyncCancelComplete(const HciLePeriodicAdvertisingCreateSyncCancelReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    GapPeriodicAdvSyncCallback cb;
    void *ctx = NULL;
    if (GapLePeriodicAdvSyncGetCallback(&cb, &ctx)) {
        if (cb.createSyncCancelResult != NULL) {
            cb.createSyncCancelResult(param->status, ctx);
        }
        GapLePeriodicAdvSyncCallbackRelease();
    }
}

int GAP_LePeriodicAdvTerminateSync(uint16_t syncHandle)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL) == false) {
        ret = GAP_ERR_INVAL_STATE;
    } else if (syncHandle > GAP_PERIODIC_ADV_SYNC_HANDLE_MAX) {
        ret = GAP_ERR_INVAL_PARAM;
    } else {
        HciLePeriodicAdvertisingTerminateSyncParam hciCmdParam = {
            .syncHandle = syncHandle,
        };
        ret = HCI_LePeriodicAdvertisingTerminateSync(&hciCmdParam);
    }

    return ret;
}

void GapLePeriodicAdvertisingTerminateSyncComplete(const HciLePeriodicAdvertisingTerminateSyncReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    GapPeriodicAdvSyncCallback cb;
    void *ctx = NULL;
    if (GapLePeriodicAdvSyncGetCallback(&cb, &ctx)) {
        if (cb.terminateSyncResult != NULL) {
            cb.terminateSyncResult(param->status, ctx);
        }
        GapLePeriodicAdvSyncCallbackRelease();
    }
}

void GapOnLePeriodicAdvertisingSyncEstablishedEvent(const HciLePeriodicAdvertisingSyncEstablishedEventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }

    LOG_INFO("%{public}s:status:0x%02x, syncHandle:0x%04x, sid:%hhu",
        __FUNCTION__,
        eventParam->status,
        eventParam->syncHandle,
        eventParam->advertisingSid);

    GapPeriodicAdvSyncCallback cb;
    void *ctx = NULL;
    if (!GapLePeriodicAdvSyncGetCallback(&cb, &ctx)) {
        return;
    }

    if (cb.syncEstablished == NULL) {
        GapLePeriodicAdvSyncCallbackRelease();
        return;
    }

    if (eventParam->status != HCI_SUCCESS) {
        // When synchronization fails, the remaining event parameters are invalid.
        // Report the failure to the upper layer with clearly invalid values.
        cb.syncEstablished(eventParam->status, 0xFFFF, 0xFF, NULL, 0x00, 0xFFFF, ctx);
        GapLePeriodicAdvSyncCallbackRelease();
        return;
    }

    if (eventParam->advertiserAddressType != BT_PUBLIC_DEVICE_ADDRESS &&
        eventParam->advertiserAddressType != BT_RANDOM_DEVICE_ADDRESS) {
        LOG_ERROR("%{public}s: invalid advertiserAddressType %hhu", __FUNCTION__, eventParam->advertiserAddressType);
        cb.syncEstablished(HCI_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE, 0xFFFF, 0xFF, NULL, 0x00, 0xFFFF, ctx);
        GapLePeriodicAdvSyncCallbackRelease();
        return;
    }

    BtAddr addr = {
        .type = eventParam->advertiserAddressType,
    };
    (void)memcpy_s(addr.addr, BT_ADDRESS_SIZE, eventParam->advertiserAddress.raw, BT_ADDRESS_SIZE);

    cb.syncEstablished(eventParam->status,
        eventParam->syncHandle,
        eventParam->advertisingSid,
        &addr,
        eventParam->advertiserPhy,
        eventParam->periodicAdvertisingInterval,
        ctx);
    GapLePeriodicAdvSyncCallbackRelease();
}

void GapOnLePeriodicAdvertisingReportEvent(const HciLePeriodicAdvertisingReportEventParam *eventParam)
{
    if (eventParam == NULL || (eventParam->dataLength > 0 && eventParam->data == NULL) ||
        eventParam->dataLength > GAP_PERIODIC_ADV_DATA_LENGTH_MAX) {
        return;
    }

    GapPeriodicAdvSyncCallback cb;
    void *ctx = NULL;
    if (!GapLePeriodicAdvSyncGetCallback(&cb, &ctx)) {
        return;
    }

    if (cb.syncReport == NULL) {
        GapLePeriodicAdvSyncCallbackRelease();
        return;
    }

    uint8_t *dataBuf = NULL;
    if (eventParam->dataLength > 0) {
        dataBuf = (uint8_t *)MEM_MALLOC.alloc(eventParam->dataLength);
        if (dataBuf == NULL) {
            LOG_ERROR("%{public}s: failed to allocate periodic adv data buffer", __FUNCTION__);
            GapLePeriodicAdvSyncCallbackRelease();
            return;
        }
        (void)memcpy_s(dataBuf, eventParam->dataLength, eventParam->data, eventParam->dataLength);
    }

    // dataBuf is borrowed by the callback and remains valid only until
    // cb.syncReport returns. The callback must not take ownership of it.
    cb.syncReport(eventParam->syncHandle,
        (int8_t)eventParam->txPower,
        (int8_t)eventParam->rssi,
        eventParam->dataStatus,
        eventParam->dataLength,
        dataBuf,
        ctx);

    if (dataBuf != NULL) {
        MEM_MALLOC.free(dataBuf);
    }
    GapLePeriodicAdvSyncCallbackRelease();
}

void GapOnLePeriodicAdvertisingSyncLostEvent(const HciLePeriodicAdvertisingSyncLostEventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }

    LOG_INFO("%{public}s:syncHandle:0x%04x", __FUNCTION__, eventParam->syncHandle);

    GapPeriodicAdvSyncCallback cb;
    void *ctx = NULL;
    if (GapLePeriodicAdvSyncGetCallback(&cb, &ctx)) {
        if (cb.syncLost != NULL) {
            cb.syncLost(eventParam->syncHandle, ctx);
        }
        GapLePeriodicAdvSyncCallbackRelease();
    }
}

int GAP_LeAddDeviceToPeriodicAdvertiserList(uint8_t addrType, const BtAddr *addr, uint8_t advSid)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL) == false) {
        return GAP_ERR_INVAL_STATE;
    }

    if (addr == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (addrType != BT_PUBLIC_DEVICE_ADDRESS && addrType != BT_RANDOM_DEVICE_ADDRESS) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (addrType != addr->type) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (advSid > GAP_PERIODIC_ADV_SID_MAX) {
        return GAP_ERR_INVAL_PARAM;
    }

    HciLeAddDeviceToPeriodicAdvertiserListParam hciCmdParam = {
        .advertiserAddressType = addrType,
        .advertisingSid = advSid,
    };
    int copyRet = memcpy_s(hciCmdParam.advertiserAddress, BT_ADDRESS_SIZE, addr->addr, BT_ADDRESS_SIZE);
    if (copyRet != EOK) {
        return GAP_ERR_INVAL_PARAM;
    }
    ret = HCI_LeAddDeviceToPeriodicAdvertiserList(&hciCmdParam);

    return ret;
}

void GapLeAddDeviceToPeriodicAdvertiserListComplete(const HciLeAddDeviceToPeriodicAdvertiserListReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    GapPeriodicAdvSyncCallback cb;
    void *ctx = NULL;
    if (GapLePeriodicAdvSyncGetCallback(&cb, &ctx)) {
        if (cb.addDeviceToPeriodicAdvertiserListResult != NULL) {
            cb.addDeviceToPeriodicAdvertiserListResult(param->status, ctx);
        }
        GapLePeriodicAdvSyncCallbackRelease();
    }
}

int GAP_LeRemoveDeviceFromPeriodicAdvertiserList(uint8_t addrType, const BtAddr *addr, uint8_t advSid)
{
    LOG_INFO("%{public}s:", __FUNCTION__);
    int ret = GAP_SUCCESS;

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL) == false) {
        return GAP_ERR_INVAL_STATE;
    }

    if (addr == NULL) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (addrType != BT_PUBLIC_DEVICE_ADDRESS && addrType != BT_RANDOM_DEVICE_ADDRESS) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (addrType != addr->type) {
        return GAP_ERR_INVAL_PARAM;
    }

    if (advSid > GAP_PERIODIC_ADV_SID_MAX) {
        return GAP_ERR_INVAL_PARAM;
    }

    HciLeRemoveDeviceFromPeriodicAdvertiserListParam hciCmdParam = {
        .advertiserAddressType = addrType,
        .advertisingSid = advSid,
    };
    int copyRet = memcpy_s(hciCmdParam.advertiserAddress, BT_ADDRESS_SIZE, addr->addr, BT_ADDRESS_SIZE);
    if (copyRet != EOK) {
        return GAP_ERR_INVAL_PARAM;
    }
    ret = HCI_LeRemoveDeviceFromPeriodicAdvertiserList(&hciCmdParam);

    return ret;
}

void GapLeRemoveDeviceFromPeriodicAdvertiserListComplete(
    const HciLeRemoveDeviceFromPeriodicAdvertiserListReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    GapPeriodicAdvSyncCallback cb;
    void *ctx = NULL;
    if (GapLePeriodicAdvSyncGetCallback(&cb, &ctx)) {
        if (cb.removeDeviceFromPeriodicAdvertiserListResult != NULL) {
            cb.removeDeviceFromPeriodicAdvertiserListResult(param->status, ctx);
        }
        GapLePeriodicAdvSyncCallbackRelease();
    }
}

int GAP_LeClearPeriodicAdvertiserList(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL) == false) {
        return GAP_ERR_INVAL_STATE;
    }

    return HCI_LeClearPeriodicAdvertiserList();
}

void GapLeClearPeriodicAdvertiserListComplete(const HciLeClearPeriodicAdvertiserListReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    GapPeriodicAdvSyncCallback cb;
    void *ctx = NULL;
    if (GapLePeriodicAdvSyncGetCallback(&cb, &ctx)) {
        if (cb.clearPeriodicAdvertiserListResult != NULL) {
            cb.clearPeriodicAdvertiserListResult(param->status, ctx);
        }
        GapLePeriodicAdvSyncCallbackRelease();
    }
}

int GAP_LeReadPeriodicAdvertiserListSize(void)
{
    LOG_INFO("%{public}s:", __FUNCTION__);

    if (GapIsLeEnable() == false) {
        return GAP_ERR_NOT_ENABLE;
    }

    if (GapLeRolesCheck(GAP_LE_ROLE_OBSERVER | GAP_LE_ROLE_CENTRAL) == false) {
        return GAP_ERR_INVAL_STATE;
    }

    return HCI_LeReadPeriodicAdvertiserListSize();
}

void GapLeReadPeriodicAdvertiserListSizeComplete(const HciLeReadPeriodicAdvertiserListSizeReturnParam *param)
{
    if (param == NULL) {
        return;
    }

    GapPeriodicAdvSyncCallback cb;
    void *ctx = NULL;
    if (GapLePeriodicAdvSyncGetCallback(&cb, &ctx)) {
        if (cb.readPeriodicAdvertiserListSizeResult != NULL) {
            cb.readPeriodicAdvertiserListSizeResult(param->status, param->periodicAdvertiserListSize, ctx);
        }
        GapLePeriodicAdvSyncCallbackRelease();
    }
}
