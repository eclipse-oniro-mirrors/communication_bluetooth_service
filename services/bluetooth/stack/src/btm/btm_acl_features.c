/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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

/*
 * Remote feature support requests and their HCI event callbacks. Split out of
 * btm_acl.c to keep the source file within the size limit. The shared
 * connection list/state lives in btm_acl.c and is exported through
 * btm_acl_def.h.
 */

#include "btm_acl.h"

#include <securec.h>

#include "hci/hci.h"
#include "hci/hci_error.h"
#include "platform/include/allocator.h"
#include "platform/include/list.h"
#include "platform/include/mutex.h"

#include "btm_acl_def.h"

static void BtmGetRemoteDeviceSupportRequests(
    uint16_t connectionHandle, BtmRemoteDeviceSupportEvent event, List *requests)
{
    BtmRemoteDeviceSupportRequest *request = NULL;
    BtmRemoteDeviceSupportRequest *duplicated = NULL;

    ListNode *node = ListGetFirstNode(g_remoteSupportRequestList);
    while (node != NULL) {
        request = ListGetNodeData(node);
        node = ListGetNextNode(node);

        if (request->event == event && request->connectionHandle == connectionHandle) {
            duplicated = MEM_MALLOC.alloc(sizeof(BtmRemoteDeviceSupportRequest));
            if (duplicated != NULL) {
                *duplicated = *request;
                ListAddLast(requests, duplicated);
            }

            ListRemoveNode(g_remoteSupportRequestList, request);
        }
    }
}

static void BtmOnLeRemoteFeatureComplete(const BtAddr *addr, const List *requests, const HciLeFeatures *leFeatures)
{
    BtmRemoteDeviceSupportRequest *request = NULL;
    ListNode *node = ListGetFirstNode(requests);
    while (node != NULL) {
        request = ListGetNodeData(node);

        switch (request->feature.leFreature) {
            case CONNECTION_PARAMETER_REQUEST:
                if (request->callback != NULL) {
                    request->callback(addr, HCI_SUPPORT_CONNECTION_PARAMETERS_REQUEST_PROCEDURE(leFeatures->raw));
                }
                break;
            default:
                break;
        }

        node = ListGetNextNode(node);
    }
}

void BtmOnLeReadRemoteFeaturesComplete(const HciLeReadRemoteFeaturesCompleteEventParam *eventParam)
{
    BtAddr addr = {0};
    HciLeFeatures leFeatures = {0};
    List *requests = ListCreate(MEM_MALLOC.free);

    MutexLock(g_aclListLock);
    BtmAclConnection *connection = BtmAclFindConnectionByHandle(eventParam->connectionHandle);
    if (connection != NULL) {
        connection->remoteFeatures.le.featureStatus = eventParam->status;
        if (eventParam->status == HCI_SUCCESS) {
            connection->remoteFeatures.le.leFeatures = eventParam->leFeatures;
        }

        addr = connection->addr;
        leFeatures = connection->remoteFeatures.le.leFeatures;
        BtmGetRemoteDeviceSupportRequests(eventParam->connectionHandle, REMOTE_LE_FEATURE_COMPLETE, requests);
    }
    MutexUnlock(g_aclListLock);

    if (eventParam->status == HCI_SUCCESS) {
        HciReadRemoteVersionInformationParam cmdParam = {
            .connectionHandle = eventParam->connectionHandle,
        };
        HCI_ReadRemoteVersionInformation(&cmdParam);
    }

    if (ListGetSize(requests)) {
        BtmOnLeRemoteFeatureComplete(&addr, requests, &leFeatures);
    }

    ListDelete(requests);
}

void BtmOnReadRemoteVersionInformationComplete(
    const HciReadRemoteVersionInformationCompleteEventParam *eventParam)
{
    uint8_t transport = 0;

    MutexLock(g_aclListLock);
    BtmAclConnection *connection = BtmAclFindConnectionByHandle(eventParam->connectionHandle);
    if (connection != NULL) {
        if (eventParam->status == HCI_SUCCESS) {
            connection->remoteVersion.version = eventParam->version;
            connection->remoteVersion.manufactureName = eventParam->manufacturerName;
            connection->remoteVersion.subVersion = eventParam->subVersion;
        }

        transport = connection->transport;
    }
    MutexUnlock(g_aclListLock);

    if (transport == TRANSPORT_BREDR_STACK) {
        HciReadRemoteSupportedFeaturesParam cmdParam = {
            .connectionHandle = eventParam->connectionHandle,
        };
        HCI_ReadRemoteSupportedFeatures(&cmdParam);
    }
}

static void BtmOnRemoteFeatureComplete(const BtAddr *addr, const List *requests, const HciLmpFeatures *lmpFeatures)
{
    BtmRemoteDeviceSupportRequest *request = NULL;
    ListNode *node = ListGetFirstNode(requests);
    while (node != NULL) {
        request = ListGetNodeData(node);

        switch (request->feature.feature) {
            case EDR_ACL_2MB_MODE:
                if (request->callback != NULL) {
                    request->callback(addr, HCI_SUPPORT_EDR_ACL_2MBS_MODE(lmpFeatures->raw));
                }
                break;
            case EDR_ACL_3MB_MODE:
                if (request->callback != NULL) {
                    request->callback(addr, HCI_SUPPORT_EDR_ACL_3MBS_MODE(lmpFeatures->raw));
                }
                break;
            default:
                break;
        }

        node = ListGetNextNode(node);
    }
}

void BtmOnReadRemoteSupportedFeaturesComplete(const HciReadRemoteSupportedFeaturesCompleteEventParam *eventParam)
{
    BtAddr addr = {0};
    HciLmpFeatures lmpFeatures = {0};
    List *requests = ListCreate(MEM_MALLOC.free);

    MutexLock(g_aclListLock);
    BtmAclConnection *connection = BtmAclFindConnectionByHandle(eventParam->connectionHandle);
    if (connection != NULL) {
        connection->remoteFeatures.bredr.featureStatus = eventParam->status;
        if (eventParam->status == HCI_SUCCESS) {
            connection->remoteFeatures.bredr.lmpFeatures = eventParam->lmpFeatures;
        }

        addr = connection->addr;
        lmpFeatures = connection->remoteFeatures.bredr.lmpFeatures;
        BtmGetRemoteDeviceSupportRequests(eventParam->connectionHandle, REMOTE_FEATURE_COMPLETE, requests);
    }
    MutexUnlock(g_aclListLock);

    HciReadRemoteExtendedFeaturesParam cmdParam = {
        .connectionHandle = eventParam->connectionHandle,
        .pageNumber = 1,
    };
    HCI_ReadRemoteExtendedFeatures(&cmdParam);

    if (ListGetSize(requests)) {
        BtmOnRemoteFeatureComplete(&addr, requests, &lmpFeatures);
    }

    ListDelete(requests);
}

static void BtmOnRemoteExtendedFeatureComplete(
    const BtAddr *addr, const List *requests, const HciExtendedLmpFeatures *extendedFeatures)
{
    BtmRemoteDeviceSupportRequest *request = NULL;

    ListNode *node = ListGetFirstNode(requests);
    while (node != NULL) {
        request = ListGetNodeData(node);

        switch (request->feature.extendedFeature) {
            case SECURE_SIMPLE_PAIRING_HOST_SUPPORT:
                if (request->callback != NULL) {
                    request->callback(addr, HCI_SUPPORT_SECURE_SIMPLE_PAIRING_HOST(extendedFeatures->page[1]));
                }
                break;
            default:
                break;
        }

        node = ListGetNextNode(node);
    }
}

void BtmOnReadRemoteExtendedFeaturesComplete(const HciReadRemoteExtendedFeaturesCompleteEventParam *eventParam)
{
    uint8_t nextPageNumber = 0;
    BtAddr addr = {0};
    HciExtendedLmpFeatures extendedFeatures = {0};
    List *requests = ListCreate(MEM_MALLOC.free);

    MutexLock(g_aclListLock);
    BtmAclConnection *connection = BtmAclFindConnectionByHandle(eventParam->connectionHandle);
    if (connection != NULL) {
        uint8_t status = REQUEST_NOT_COMPLETED;
        if (eventParam->status == HCI_SUCCESS) {
            connection->remoteFeatures.bredr.maxPageNumber = eventParam->maximumPageNumber;
            if (eventParam->pageNumber <= MAX_EXTENED_FEATURES_PAGE_NUMBER) {
                (void)memcpy_s(connection->remoteFeatures.bredr.extendedLmpFeatures.page[eventParam->pageNumber],
                    LMP_FEATURES_SIZE,
                    eventParam->extendedLMPFeatures,
                    LMP_FEATURES_SIZE);
            }

            // Cap the poll chain at the last page this stack stores; a bogus
            // maximumPageNumber from the controller must not trigger an
            // unbounded Read Remote Extended Features walk.
            if (eventParam->pageNumber < MAX_EXTENED_FEATURES_PAGE_NUMBER &&
                eventParam->pageNumber < eventParam->maximumPageNumber) {
                nextPageNumber = eventParam->pageNumber + 1;
            } else {
                connection->remoteFeatures.bredr.extendedFeatureStatus = eventParam->status;
                status = eventParam->status;
            }
        } else {
            connection->remoteFeatures.bredr.extendedFeatureStatus = eventParam->status;
            status = eventParam->status;
        }

        if (status != REQUEST_NOT_COMPLETED) {
            BtmGetRemoteDeviceSupportRequests(eventParam->connectionHandle, REMOTE_EXTENDED_FEATURE_COMPLETE, requests);

            addr = connection->addr;
            extendedFeatures = connection->remoteFeatures.bredr.extendedLmpFeatures;
        }
    }
    MutexUnlock(g_aclListLock);

    if (nextPageNumber > 0) {
        HciReadRemoteExtendedFeaturesParam cmdParam = {
            .connectionHandle = eventParam->connectionHandle,
            .pageNumber = nextPageNumber,
        };
        HCI_ReadRemoteExtendedFeatures(&cmdParam);
    }

    if (ListGetSize(requests)) {
        BtmOnRemoteExtendedFeatureComplete(&addr, requests, &extendedFeatures);
    }

    ListDelete(requests);
}
