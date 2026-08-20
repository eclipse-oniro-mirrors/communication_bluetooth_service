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

#ifndef BTM_ACL_DEF_H
#define BTM_ACL_DEF_H

#include <stdbool.h>
#include <stdint.h>

#include "btstack.h"
#include "btm.h"
#include "hci/hci.h"
#include "platform/include/alarm.h"
#include "platform/include/list.h"
#include "platform/include/mutex.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BTM_MAX_ACL 13

#define ACL_TIMEOUT (15 * 1000)          // 15s
#define ACL_PASSIVE_TIMEOUT (60 * 1000)  // 60s

#define LE_SCAN_INTERVAL_DEFAULT 96      // 96 * 0.625ms = 60ms
#define LE_SCAN_WINDOW_DEFAULT 48        // 48 * 0.625ms = 30ms
#define LE_SCAN_INTERVAL_FAST 96         // 96 * 0.625ms = 60ms
#define LE_SCAN_WINDOW_FAST 48           // 48 * 0.625ms = 30ms
#define LE_SCAN_INTERVAL_SLOW 2048       // 2048 * 0.625ms = 1.28s
#define LE_SCAN_WINDOW_SLOW 48           // 48 * 0.625ms = 30ms
#define LE_CONN_INTERVAL_MIN_DEFAULT 24  // 24 * 1.25ms = 30ms
#define LE_CONN_INTERVAL_MAX_DEFAULT 40  // 40 * 1.25ms = 50ms
#define LE_CONN_LATENCY_DEFAULT 0
#define LE_SUPERVISION_TIMEOUT_DEFAULT 500  // 50 10ms = 5s
#define LE_MINIMUM_CE_LENGTH_DEFAULT 0
#define LE_MAXIMUM_CE_LENGTH_DEFAULT 0

#define COD_SIZE 3

#define REQUEST_NOT_COMPLETED 0xff

// Shared between btm_acl.c and btm_acl_features.c (see the file split of the
// remote-feature support requests and their HCI event callbacks).

typedef enum {
    CONNECTING,
    CONNECTED,
    DISCONNECTING,
    DISCONNECTED,
} BtmAclConnectionState;

typedef struct {
    uint16_t connectionHandle;
    uint8_t transport;
    BtAddr addr;
    bool isInitiator;
    BtmAclConnectionState state;
    uint8_t refCount;
    uint8_t encryption;
    Alarm *timeoutTimer;
    union {
        struct {
            uint8_t featureStatus;
            HciLmpFeatures lmpFeatures;
            uint8_t extendedFeatureStatus;
            uint8_t maxPageNumber;
            HciExtendedLmpFeatures extendedLmpFeatures;
        } bredr;
        struct {
            uint8_t featureStatus;
            HciLeFeatures leFeatures;
        } le;
    } remoteFeatures;
    struct {
        uint8_t version;
        uint16_t manufactureName;
        uint16_t subVersion;
    } remoteVersion;
    uint8_t remoteCod[COD_SIZE];
    BtAddr leLocalAddr;
    BtAddr lePeerAddr;
} BtmAclConnection;

typedef enum {
    REMOTE_FEATURE_COMPLETE,
    REMOTE_EXTENDED_FEATURE_COMPLETE,
    REMOTE_LE_FEATURE_COMPLETE,
} BtmRemoteDeviceSupportEvent;

typedef enum {
    EDR_ACL_2MB_MODE,
    EDR_ACL_3MB_MODE,
} BtmRemoteDeviceFeature;

typedef enum {
    SECURE_SIMPLE_PAIRING_HOST_SUPPORT,
} BtmRemoteDeviceExtendedFeature;

typedef enum {
    CONNECTION_PARAMETER_REQUEST,
} BtmRemoteDeviceLeFeature;

typedef struct {
    BtAddr addr;
    uint16_t connectionHandle;
    BTMRemoteDeviceSupportCallback callback;
    BtmRemoteDeviceSupportEvent event;
    union {
        BtmRemoteDeviceFeature feature;
        BtmRemoteDeviceExtendedFeature extendedFeature;
        BtmRemoteDeviceLeFeature leFreature;
    } feature;
} BtmRemoteDeviceSupportRequest;

extern Mutex *g_aclListLock;
extern List *g_remoteSupportRequestList;

extern BtmAclConnection *BtmAclFindConnectionByHandle(uint16_t handle);

extern void BtmOnLeReadRemoteFeaturesComplete(const HciLeReadRemoteFeaturesCompleteEventParam *eventParam);
extern void BtmOnReadRemoteVersionInformationComplete(
    const HciReadRemoteVersionInformationCompleteEventParam *eventParam);
extern void BtmOnReadRemoteSupportedFeaturesComplete(
    const HciReadRemoteSupportedFeaturesCompleteEventParam *eventParam);
extern void BtmOnReadRemoteExtendedFeaturesComplete(const HciReadRemoteExtendedFeaturesCompleteEventParam *eventParam);

#ifdef __cplusplus
}
#endif

#endif
