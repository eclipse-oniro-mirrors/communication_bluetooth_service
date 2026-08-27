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

#ifndef HCI_DEF_LE_FEATURE_H
#define HCI_DEF_LE_FEATURE_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// BLUETOOTH SPECIFICATION Version 5.0 | Vol 6, Part B
// 4.6 FEATURE SUPPORT
// LE Link Layer feature bit positions (Bluetooth spec Vol 6, Part B, 4.6).
#define LE_FEATURE_BIT_CONNECTION_PARAMETERS_REQUEST_PROCEDURE 1
#define LE_FEATURE_BIT_LE_PING 4
#define LE_FEATURE_BIT_LE_DATA_PACKET_LENGTH_EXTENSION 5
#define LE_FEATURE_BIT_LL_PRIVACY 6
#define LE_FEATURE_BIT_LE_2M_PHY 8
#define LE_FEATURE_BIT_LE_CODED_PHY 11
#define LE_FEATURE_BIT_LE_EXTENDED_ADVERTISING 12
#define LE_FEATURE_BIT_LE_PERIODIC_ADVERTISING 13
#define LE_FEATURE_BIT_CHANNEL_SELECTION_ALGORITHM_2 14
#define LE_FEATURE_BIT_CONNECTION_CTE_REQUEST 17
#define LE_FEATURE_BIT_CONNECTION_CTE_RESPONSE 18
#define LE_FEATURE_BIT_CONNECTIONLESS_CTE_TRANSMITTER 19
#define LE_FEATURE_BIT_CONNECTIONLESS_CTE_RECEIVER 20
#define LE_FEATURE_BIT_ANTENNA_SWITCHING_DURING_CTE_TRANSMISSION 21
#define LE_FEATURE_BIT_ANTENNA_SWITCHING_DURING_CTE_RECEPTION 22
#define LE_FEATURE_BIT_RECEIVING_CONSTANT_TONE_EXTENSIONS 23
#define LE_FEATURE_BIT_PERIODIC_ADVERTISING_SYNC_TRANSFER_SENDER 24
#define LE_FEATURE_BIT_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECIPIENT 25
#define LE_FEATURE_BIT_SLEEP_CLOCK_ACCURACY_UPDATES 26
#define LE_FEATURE_BIT_REMOTE_PUBLIC_KEY_VALIDATION 27

// Number of bits per feature byte (Bluetooth spec Vol 6, Part B, 4.6).
#define LE_FEATURE_BITS_PER_BYTE 8

static inline bool GetLinkLayerFeatureFlag(const uint8_t *features, uint8_t bitIndex)
{
    return ((features)[(bitIndex) / LE_FEATURE_BITS_PER_BYTE] &
        (0x01 << ((bitIndex) % LE_FEATURE_BITS_PER_BYTE))) != 0;
}

static inline int HciSupportConnectionParametersRequestProcedure(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_CONNECTION_PARAMETERS_REQUEST_PROCEDURE);
}

static inline int HciSupportLePing(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_LE_PING);
}

static inline int HciSupportLeDataPacketLengthExtension(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_LE_DATA_PACKET_LENGTH_EXTENSION);
}

static inline int HciSupportLlPrivacy(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_LL_PRIVACY);
}

static inline int HciSupportLe2MPhy(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_LE_2M_PHY);
}

static inline int HciSupportLeCodedPhy(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_LE_CODED_PHY);
}

static inline int HciSupportLeExtendedAdvertising(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_LE_EXTENDED_ADVERTISING);
}

static inline int HciSupportLePeriodicAdvertising(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_LE_PERIODIC_ADVERTISING);
}

static inline int HciSupportChannelSelectionAlgorithm2(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_CHANNEL_SELECTION_ALGORITHM_2);
}

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 6, Part B
// 4.6.16-4.6.25 FEATURE SUPPORT (Direction Finding / PAST / Sleep Clock Accuracy)
static inline int HciSupportConnectionCteRequest(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_CONNECTION_CTE_REQUEST);
}

static inline int HciSupportConnectionCteResponse(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_CONNECTION_CTE_RESPONSE);
}

static inline int HciSupportConnectionlessCteTransmitter(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_CONNECTIONLESS_CTE_TRANSMITTER);
}

static inline int HciSupportConnectionlessCteReceiver(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_CONNECTIONLESS_CTE_RECEIVER);
}

static inline int HciSupportAntennaSwitchingDuringCteTransmissionAod(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_ANTENNA_SWITCHING_DURING_CTE_TRANSMISSION);
}

static inline int HciSupportAntennaSwitchingDuringCteReceptionAoa(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_ANTENNA_SWITCHING_DURING_CTE_RECEPTION);
}

static inline int HciSupportReceivingConstantToneExtensions(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_RECEIVING_CONSTANT_TONE_EXTENSIONS);
}

static inline int HciSupportPeriodicAdvertisingSyncTransferSender(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_PERIODIC_ADVERTISING_SYNC_TRANSFER_SENDER);
}

static inline int HciSupportPeriodicAdvertisingSyncTransferRecipient(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECIPIENT);
}

static inline int HciSupportSleepClockAccuracyUpdates(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_SLEEP_CLOCK_ACCURACY_UPDATES);
}

// BLUETOOTH SPECIFICATION Version 5.1 | Vol 6, Part B
// 4.6.26 Remote Public Key Validation
// If the controller supports it, it validates the remote P-256 public key and
// returns Invalid HCI Command Parameters (0x12) with DHKey set to all 0xFF on
// failure. The Host MUST validate the public key itself regardless (Vol 3,
// Part H, 2.3.5,6,1), and must not perform CTKD when this bit is 0 (Erratum 10734).
static inline int HciSupportRemotePublicKeyValidation(const uint8_t *features)
{
    return GetLinkLayerFeatureFlag(features, LE_FEATURE_BIT_REMOTE_PUBLIC_KEY_VALIDATION);
}

#ifdef __cplusplus
}
#endif

#endif