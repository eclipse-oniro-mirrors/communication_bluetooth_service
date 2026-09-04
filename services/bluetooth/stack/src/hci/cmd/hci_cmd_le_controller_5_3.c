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

// LE command senders for the Bluetooth 5.3 additions: LE Set Data Related
// Address Changes (7.8.122) and Connection Subrating (7.8.123-7.8.124). Split
// out of hci_cmd_le_controller.c (and not merged into
// hci_cmd_le_controller_5_2.c) to keep those files under the source size limit,
// mirroring the 5.2 split.

#include "hci/hci.h"

#include <securec.h>

#include "btstack.h"
#include "platform/include/allocator.h"

#include "hci_cmd.h"

// BLUETOOTH SPECIFICATION Version 5.3 | Vol 4, Part E
// Connection Subrating parameter limits (7.8.123-7.8.124, verified against
// the amended 2024 spec tables) are shared with the GAP-side gate via
// hci_def_le_cmd.h; do not redefine them here.

// Shared validation of the four subrating value fields plus supervision
// timeout. Returns false and leaves the gate to the caller on the first
// violated restriction so the sending layer rejects what the Controller would
// refuse with 0x12 (invalid command parameters) anyway.
// Deliberately duplicated as GapLeSubrateParamsValid in gap_le_subrate.c
// (defense in depth); both consume the shared LE_SUBRATE_* limits from
// hci_def_le_cmd.h, so only the predicate shape needs to stay in sync.
static bool HciSubrateParamValid(uint16_t subrateMin, uint16_t subrateMax, uint16_t maxLatency,
    uint16_t continuationNumber, uint16_t supervisionTimeout)
{
    if (subrateMin < LE_SUBRATE_FACTOR_MIN || subrateMax < LE_SUBRATE_FACTOR_MIN) {
        return false; // 0 is reserved
    }
    if (subrateMax > LE_SUBRATE_FACTOR_MAX || subrateMin > subrateMax) {
        return false;
    }
    if (maxLatency > LE_SUBRATE_MAX_LATENCY_MAX) {
        return false;
    }
    // Subrate_Max x (Max_Latency + 1) <= 500 and Continuation_Number < Subrate_Max.
    if (subrateMax > LE_SUBRATE_MAX_LATENCY_PRODUCT_MAX / (maxLatency + 1)) {
        return false;
    }
    if (continuationNumber > LE_SUBRATE_CONTINUATION_MAX || continuationNumber >= subrateMax) {
        return false;
    }
    if (supervisionTimeout < LE_SUBRATE_SUPERVISION_TIMEOUT_MIN ||
        supervisionTimeout > LE_SUBRATE_SUPERVISION_TIMEOUT_MAX) {
        return false;
    }
    return true;
}

// BLUETOOTH SPECIFICATION Version 5.3 | Vol 4, Part E
// 7.8.122 LE Set Data Related Address Changes Command. Host-issued, optional
// RPA-refresh policy: when the enabled reason occurs, the Controller refreshes
// the Resolvable Private Address of the advertising set regardless of the
// address timeout. Only a Command_Complete is generated (no event); a
// controller without the command (5.2 or earlier) rejects it, which the caller
// tolerates like any best-effort privacy step. No upper-stack consumer exists
// today (RPA lifecycle is driven above the stack), so the command stays at the
// HCI layer with its completion fan-out available to future callers and tests.
int HCI_LeSetDataRelatedAddressChanges(const HciLeSetDataRelatedAddressChangesParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->advertisingHandle > 0xEF) {
        return BT_BAD_PARAM;
    }
    if ((param->changeReasons & ~0x03) != 0) {
        return BT_BAD_PARAM; // bits 2-7 are reserved for future use
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_DATA_RELATED_ADDRESS_CHANGES, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.3 | Vol 4, Part E
// 7.8.123 LE Set Default Subrate Command
int HCI_LeSetDefaultSubrate(const HciLeSetDefaultSubrateParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (!HciSubrateParamValid(param->defaultSubrateMin, param->defaultSubrateMax, param->defaultMaxLatency,
        param->defaultContinuationNumber, param->defaultSupervisionTimeout)) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_DEFAULT_SUBRATE, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// BLUETOOTH SPECIFICATION Version 5.3 | Vol 4, Part E
// 7.8.124 LE Subrate Request Command (issuable by both Central and Peripheral)
int HCI_LeSubrateRequest(const HciLeSubrateRequestParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->connectionHandle > 0x0EFF) {
        return BT_BAD_PARAM;
    }
    if (!HciSubrateParamValid(param->subrateMin, param->subrateMax, param->maxLatency, param->continuationNumber,
        param->supervisionTimeout)) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SUBRATE_REQUEST, (void *)param, sizeof(*param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}
