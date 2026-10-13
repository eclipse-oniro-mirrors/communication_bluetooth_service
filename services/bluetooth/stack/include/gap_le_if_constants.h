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

#ifndef GAP_LE_IF_CONSTANTS_H
#define GAP_LE_IF_CONSTANTS_H

// GAP LE interface constants. This header is included by gap_le_if.h;
// do not include it directly.
#include "gap_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

/// BLE Roles
#define GAP_LE_ROLE_BROADCASTER (1 << 0)
#define GAP_LE_ROLE_OBSERVER (1 << 1)
#define GAP_LE_ROLE_PERIPHERAL (1 << 2)
#define GAP_LE_ROLE_CENTRAL (1 << 3)
// Deprecated: legacy misspelling kept for source compatibility with earlier releases.
#define GAP_LE_ROLE_PREIPHERAL GAP_LE_ROLE_PERIPHERAL

#define GAP_IRK_SIZE 0x10
#define GAP_CSRK_SIZE 0x10
#define GAP_SIGNATURE_SIZE 12

typedef void (*GenResPriAddrResult)(uint8_t result, const uint8_t addr[BT_ADDRESS_SIZE], void *context);

#define GAP_ADVERTISING_DATA_LENGTH_MAX 31

/// Advertising channel
#define GAP_ADVERTISING_CHANNEL_37 (1 << 0)
#define GAP_ADVERTISING_CHANNEL_38 (1 << 1)
#define GAP_ADVERTISING_CHANNEL_39 (1 << 2)

/// Advertising filter policy
#define GAP_ADVERTISING_NOT_USE_WL (0x00)
#define GAP_ADVERTISING_USE_WL_FOR_SCAN (0x01)
#define GAP_ADVERTISING_USE_WL_FOR_CONNECTION (0x02)
#define GAP_ADVERTISING_USE_WL_FOR_BOTH (0x03)

/// Intended Advertising property
#define GAP_ADVERTISING_PROPERTY_CONNECTABLE (1 << 0)
#define GAP_ADVERTISING_PROPERTY_SCANABLE (1 << 1)
#define GAP_ADVERTISING_PROPERTY_DIRECTED (1 << 2)
#define GAP_ADVERTISING_PROPERTY_HIGH_DUTY_CYCLE_DIRECTED (1 << 3)
#define GAP_ADVERTISING_PROPERTY_USE_LEGACY_PDUS (1 << 4)
#define GAP_ADVERTISING_PROPERTY_ANONYMOUS (1 << 5)
#define GAP_ADVERTISING_PROPERTY_INCLUDE_TXPOWER (1 << 6)

/// Intended Advertising data operation
#define GAP_ADVERTISING_DATA_OPERATION_INTERMEDIATE (0x00)
#define GAP_ADVERTISING_DATA_OPERATION_FIRST (0x01)
#define GAP_ADVERTISING_DATA_OPERATION_LAST (0x02)
#define GAP_ADVERTISING_DATA_OPERATION_COMPLETE (0x03)
#define GAP_ADVERTISING_DATA_OPERATION_UNCHANGED (0x04)

/// Intended Periodic Advertising data operation
#define GAP_PERIODIC_ADV_DATA_OPERATION_INTERMEDIATE (0x00)
#define GAP_PERIODIC_ADV_DATA_OPERATION_FIRST (0x01)
#define GAP_PERIODIC_ADV_DATA_OPERATION_LAST (0x02)
#define GAP_PERIODIC_ADV_DATA_OPERATION_COMPLETE (0x03)
#define GAP_PERIODIC_ADV_DATA_OPERATION_UNCHANGED (0x04)

/// Intended Advertising PHY
#define GAP_ADVERTISEMENT_PHY_NONE (0x00)
#define GAP_ADVERTISEMENT_PHY_1M (0x01)
#define GAP_ADVERTISEMENT_PHY_2M (0x02)
#define GAP_ADVERTISEMENT_PHY_CODED (0x03)

/// LE PHY bit mask used by GAPIF_LeSetPhy/GAPIF_LeSetDefaultPhy
#define GAP_LE_PHY_BIT_1M (1 << 0)
#define GAP_LE_PHY_BIT_2M (1 << 1)
#define GAP_LE_PHY_BIT_CODED (1 << 2)
#define GAP_LE_PHY_BIT_ALL (GAP_LE_PHY_BIT_1M | GAP_LE_PHY_BIT_2M | GAP_LE_PHY_BIT_CODED)

/// allPhys parameter used by GAPIF_LeSetPhy/GAPIF_LeSetDefaultPhy
#define GAP_LE_ALL_PHY_TX_NO_PREFERENCE (1 << 0)
#define GAP_LE_ALL_PHY_RX_NO_PREFERENCE (1 << 1)
#define GAP_LE_ALL_PHY_VALID_MASK (GAP_LE_ALL_PHY_TX_NO_PREFERENCE | GAP_LE_ALL_PHY_RX_NO_PREFERENCE)

#ifdef __cplusplus
}
#endif

#endif // GAP_LE_IF_CONSTANTS_H
