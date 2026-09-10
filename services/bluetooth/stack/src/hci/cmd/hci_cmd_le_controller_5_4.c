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

// LE command senders for the Bluetooth 5.4 additions: LE Set Extended
// Advertising Parameters [v2] (7.8.53). Split out of hci_cmd_le_controller.c
// (and not merged into hci_cmd_le_controller_5_3.c) to keep those files under
// the source size limit, mirroring the 5.2/5.3 splits. The periodic
// advertising with responses commands (7.8.125-7.8.127) and the remaining
// [v2] commands live here as well.

#include "hci/hci.h"

#include <securec.h>

#include "btstack.h"
#include "platform/include/allocator.h"

#include "hci_cmd.h"

// Parameter limits of LE Set Extended Advertising Parameters (7.8.53),
// mirroring the file-local limits of the [v1] sender in
// hci_cmd_le_controller.c; keep the two in sync.
#define LE_EXTENDED_ADV_HANDLE_MAX 0xEF
#define LE_EXT_ADV_EVENT_PROPERTIES_RESERVED_MASK 0xFF00 // bits 8-15 are RFU
#define LE_EXT_ADV_CHANNEL_MAP_MASK 0x07 // Primary_Advertising_Channel_Map is 3 bits
#define LE_EXT_ADV_PEER_ADDRESS_TYPE_MAX 0x01 // identity types (0x02/0x03) are not valid here
#define LE_EXT_ADV_FILTER_POLICY_MAX 0x03
#define LE_EXT_ADV_PHY_MIN 0x01 // LE 1M
#define LE_EXT_ADV_PHY_MAX 0x03 // LE Coded
#define LE_EXT_ADV_PHY_2M 0x02 // LE 2M; valid only for Secondary_Advertising_PHY (7.8.53)
#define LE_ADVERTISING_SID_MAX 0x0F
#define LE_ENABLE_MAX 0x01
#define LE_OWN_ADDRESS_TYPE_MAX 0x03 // includes identity address types
// HCI 16-bit fields are sent low octet first (Vol 2, Part E 5.1.1); the byte
// shifts below place the high octet of such a field.
#define LE_HIGH_OCTET_SHIFT 8

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.53 LE Set Extended Advertising Parameters Command [v2]
// Field ranges are identical to [v1]; the [v2] sender additionally rejects
// reserved Primary/Secondary_Advertising_PHY_Options values (0x05-0xFF) that
// the Controller would refuse with 0x11. Options naming a non-Coded PHY are
// ignored by the Controller per the spec (not an error), and the unsupported-
// feature / not-satisfiable cases (0x11 / 0x0C) are left to the Controller -
// the caller gates on the Advertising Coding Selection capability
// (BTM_IsControllerSupportLeAdvCodingSel) before using this command.
int HCI_LeSetExtendedAdvertisingParametersV2(const HciLeSetExtendedAdvertisingParametersV2Param *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->advertisingHandle > LE_EXTENDED_ADV_HANDLE_MAX ||
        (param->advertisingEventProperties & LE_EXT_ADV_EVENT_PROPERTIES_RESERVED_MASK) != 0 ||
        (param->priAdvertisingChannelMap & ~LE_EXT_ADV_CHANNEL_MAP_MASK) != 0 ||
        param->ownAddressType > LE_OWN_ADDRESS_TYPE_MAX ||
        param->peerAddressType > LE_EXT_ADV_PEER_ADDRESS_TYPE_MAX ||
        param->advertisingFilterPolicy > LE_EXT_ADV_FILTER_POLICY_MAX ||
        // Primary_Advertising_PHY accepts only 0x01 (LE 1M) and 0x03 (LE Coded);
        // 0x02 (LE 2M) is Reserved in v5.0 (7.8.53).
        param->priAdvertisingPHY < LE_EXT_ADV_PHY_MIN || param->priAdvertisingPHY > LE_EXT_ADV_PHY_MAX ||
        param->priAdvertisingPHY == LE_EXT_ADV_PHY_2M ||
        param->secondaryAdvertisingPHY < LE_EXT_ADV_PHY_MIN || param->secondaryAdvertisingPHY > LE_EXT_ADV_PHY_MAX ||
        param->advertisingSID > LE_ADVERTISING_SID_MAX ||
        param->scanRequestNotificationEnable > LE_ENABLE_MAX ||
        param->primaryAdvertisingPhyOptions > LE_EXT_ADV_PHY_OPTIONS_MAX ||
        param->secondaryAdvertisingPhyOptions > LE_EXT_ADV_PHY_OPTIONS_MAX) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_EXTENDED_ADVERTISING_PARAMETERS_V2, (void *)param,
        sizeof(HciLeSetExtendedAdvertisingParametersV2Param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}

// Parameter limits of the 5.4 PAwR commands below, mirroring the file-local
// limits of the [v1]/5.0 senders in hci_cmd_le_controller.c; keep the two in
// sync. Field ranges follow the 5.4 field tables (7.8.125-7.8.127, 7.8.66 [v2]
// and 7.8.61 [v2]).
#define PERIODIC_ADV_HANDLE_MAX 0xEF
#define PERIODIC_ADV_INTERVAL_MIN 0x0006
#define PERIODIC_ADV_INTERVAL_MAX 0xFFFF
#define PERIODIC_ADV_SUBEVENT_INTERVAL_MIN 0x06
#define LE_PAWR_SUBEVENT_MAX 0x7F
// The spec wire range of one data block is 0-251 octets (7.8.125/7.8.126;
// 252-0xFF reserved), but a block is transmitted in one HCI command whose
// parameters travel in a one-octet length field (hci_cmd.c), so 250/251 can
// never be framed. The entry-level caps below are what the senders actually
// accept; they stay in sync with the GAP-side byte budgets
// (GAP_PAWR_SUBEVENT_DATA_SEND_MAX / GAP_PAWR_RESPONSE_DATA_SEND_MAX).
#define LE_PAWR_SUBEVENT_DATA_FRAME_MAX 0xF9 // 249 octets: largest single 0x82 entry that fits the frame
#define LE_PAWR_RESPONSE_DATA_FRAME_MAX 0xF7 // 247 octets: largest 0x83 data block that fits the frame
// Fixed octets of one 7.8.125 entry before its data: Subevent,
// Response_Slot_Start, Response_Slot_Count and Subevent_Data_Length, one each.
#define LE_PAWR_SUBEVENT_ENTRY_HEADER_LEN 0x04
#define LE_PAWR_SYNC_HANDLE_MAX 0x0EFF // PERIODIC_ADV_SYNC_HANDLE_MAX in the [v1] file
// Initiating_PHYs (7.8.66): LE 1M bit0 / LE 2M bit1 / LE Coded bit2.
#define LE_INITIATING_PHYS_MASK (LE_1M_PHY | LE_2M_PHY | LE_CODED_PHY)
#define LE_FILTER_POLICY_MAX 0x02
// LE_OWN_ADDRESS_TYPE_MAX is already defined in the 7.8.53 limits block above
#define LE_PEER_ADDRESS_TYPE_MAX 0x03
#define LE_SCAN_INTERVAL_MIN 0x0004
// The 5.4 [v2] field table (7.8.66) widened the range to 0x0004-0xFFFF; the
// [v1] sender in hci_cmd_le_controller.c keeps its 0x4000 cap.
#define LE_SCAN_INTERVAL_MAX 0xFFFF
#define LE_SCAN_WINDOW_MIN 0x0004
#define LE_SCAN_WINDOW_MAX 0xFFFF
#define LE_CONN_INTERVAL_MIN 0x0006
#define LE_CONN_INTERVAL_MAX 0x0C80
#define LE_CONN_LATENCY_MAX 0x01F3
#define LE_CONN_SUPERVISION_TIMEOUT_MIN 0x000A
#define LE_CONN_SUPERVISION_TIMEOUT_MAX 0x0C80

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.125 LE Set Periodic Advertising Subevent Data Command
// The data for a subevent is transmitted only once, so this command answers
// each HCI_LE_Periodic_Advertising_Subevent_Data_Request event (0x27) with a
// fresh data block per subevent. Static host-side checks reject parameter
// values that are reserved in the field table; the Controller-side error
// cases (0x42 unknown advertising set, 0x0C subevent outside the 0x27
// request, 0x45 combined data too long, 0x46/0x47 too late/too early) are
// returned in the Command Complete status.
int HCI_LeSetPeriodicAdvertisingSubeventData(const HciLeSetPeriodicAdvertisingSubeventDataParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->advertisingHandle > PERIODIC_ADV_HANDLE_MAX ||
        param->numSubevents < 0x01 || param->numSubevents > 0x0F || param->sets == NULL) {
        return BT_BAD_PARAM;
    }

    size_t length = 2;
    for (uint8_t i = 0; i < param->numSubevents; i++) {
        // subeventDataLength above LE_PAWR_SUBEVENT_DATA_FRAME_MAX can never
        // be framed by this command (see the frame check below); rejecting it
        // per entry keeps the failure at the offending parameter.
        if (param->sets[i].subevent > LE_PAWR_SUBEVENT_MAX ||
            param->sets[i].subeventDataLength > LE_PAWR_SUBEVENT_DATA_FRAME_MAX ||
            (param->sets[i].subeventDataLength > 0 && param->sets[i].subeventData == NULL)) {
            return BT_BAD_PARAM;
        }
        length += LE_PAWR_SUBEVENT_ENTRY_HEADER_LEN + param->sets[i].subeventDataLength;
    }

    // The HCI transport carries command parameters in a one-octet length
    // field (hci_cmd.c HciCreateCmdPacketWithParam), so a parameter block
    // beyond 255 octets would be truncated silently on the wire. 7.8.125
    // allows up to 251 octets of data per subevent, of which only 249 fit in
    // a single-entry command (2 + 4 + 249 = 255); the caller (GAP PAwR
    // advertiser) chunks by byte budget, and an oversized block is rejected
    // here instead of being sent truncated.
    if (length > UINT8_MAX) {
        return BT_BAD_PARAM;
    }

    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    size_t index = 0;
    buf[index++] = param->advertisingHandle;
    buf[index++] = param->numSubevents;
    for (uint8_t i = 0; i < param->numSubevents; i++) {
        buf[index++] = param->sets[i].subevent;
        buf[index++] = param->sets[i].responseSlotStart;
        buf[index++] = param->sets[i].responseSlotCount;
        buf[index++] = param->sets[i].subeventDataLength;
        if (param->sets[i].subeventDataLength > 0) {
            if (memcpy_s(buf + index, length - index, param->sets[i].subeventData,
                param->sets[i].subeventDataLength) != EOK) {
                MEM_MALLOC.free(buf);
                return BT_OPERATION_FAILED;
            }
            index += param->sets[i].subeventDataLength;
        }
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_PERIODIC_ADVERTISING_SUBEVENT_DATA, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }
    int result = HciSendCmd(cmd);

    MEM_MALLOC.free(buf);

    return result;
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.126 LE Set Periodic Advertising Response Data Command
// Answers an AUX_SYNC_SUBEVENT_IND the Host received on the sync side: the
// data is transmitted once in the slot named by Response_Subevent/
// Response_Slot, which may differ from the subevent the request was received
// in (Request_Subevent/Request_Event, the latter being the paEventCounter of
// the packet). Slot timing errors surface as Command Complete status: 0x45
// (data too long for the response slot) and 0x46 (response slot already
// passed).
int HCI_LeSetPeriodicAdvertisingResponseData(const HciLeSetPeriodicAdvertisingResponseDataParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->syncHandle > LE_PAWR_SYNC_HANDLE_MAX ||
        param->responseSubevent > LE_PAWR_SUBEVENT_MAX ||
        param->responseDataLength > LE_PAWR_RESPONSE_DATA_FRAME_MAX ||
        (param->responseDataLength > 0 && param->responseData == NULL)) {
        return BT_BAD_PARAM;
    }

    // Same transport cap as the 0x82 sender: the one-octet HCI parameter
    // length field (hci_cmd.c) truncates blocks beyond 255 octets silently,
    // so a response data block that cannot be framed is rejected here (a
    // real Controller would return Packet Too Long 0x45 for it, 7.8.126).
    // The entry check above already caps the data at
    // LE_PAWR_RESPONSE_DATA_FRAME_MAX so the frame check only guards the
    // fixed header growth.
    const size_t length = 8 + param->responseDataLength;
    if (length > UINT8_MAX) {
        return BT_BAD_PARAM;
    }
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    size_t index = 0;
    buf[index++] = param->syncHandle & 0xFF;
    buf[index++] = param->syncHandle >> LE_HIGH_OCTET_SHIFT;
    buf[index++] = param->requestEvent & 0xFF;
    buf[index++] = param->requestEvent >> LE_HIGH_OCTET_SHIFT;
    buf[index++] = param->requestSubevent;
    buf[index++] = param->responseSubevent;
    buf[index++] = param->responseSlot;
    buf[index++] = param->responseDataLength;
    if (param->responseDataLength > 0) {
        if (memcpy_s(buf + index, length - index, param->responseData, param->responseDataLength) != EOK) {
            MEM_MALLOC.free(buf);
            return BT_OPERATION_FAILED;
        }
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_PERIODIC_ADVERTISING_RESPONSE_DATA, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }
    int result = HciSendCmd(cmd);

    MEM_MALLOC.free(buf);

    return result;
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.127 LE Set Periodic Sync Subevent Command
// Restricts an established PAwR synchronization to a subset of subevents;
// subevents that are synchronized but not listed stop being synchronized.
// Bit 6 of periodicAdvertisingProperties asks the Controller to include
// TxPower in the AUX_SYNC_SUBEVENT_RSP PDUs.
int HCI_LeSetPeriodicSyncSubevent(const HciLeSetPeriodicSyncSubeventParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->syncHandle > LE_PAWR_SYNC_HANDLE_MAX ||
        (param->periodicAdvertisingProperties & ~LE_SET_PERIODIC_SYNC_SUBEVENT_PROPERTIES_TX_POWER) != 0 ||
        param->numSubevents < 0x01 || param->numSubevents > 0x80 || param->subevents == NULL) {
        return BT_BAD_PARAM;
    }
    for (uint8_t i = 0; i < param->numSubevents; i++) {
        if (param->subevents[i] > LE_PAWR_SUBEVENT_MAX) {
            return BT_BAD_PARAM;
        }
    }

    const size_t length = 5 + param->numSubevents;
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    size_t index = 0;
    buf[index++] = param->syncHandle & 0xFF;
    buf[index++] = param->syncHandle >> LE_HIGH_OCTET_SHIFT;
    buf[index++] = param->periodicAdvertisingProperties & 0xFF;
    buf[index++] = param->periodicAdvertisingProperties >> LE_HIGH_OCTET_SHIFT;
    buf[index++] = param->numSubevents;
    if (memcpy_s(buf + index, length - index, param->subevents, param->numSubevents) != EOK) {
        MEM_MALLOC.free(buf);
        return BT_OPERATION_FAILED;
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_SET_PERIODIC_SYNC_SUBEVENT, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }
    int result = HciSendCmd(cmd);

    MEM_MALLOC.free(buf);

    return result;
}

static int HciLeExtendedCreateConnectionV2CheckPair(const HciLeExtendedCreateConnectionV2Param *param)
{
    if ((param->advertisingHandle == 0xFF) != (param->subevent == 0xFF) ||
        (param->advertisingHandle != 0xFF && param->advertisingHandle > PERIODIC_ADV_HANDLE_MAX) ||
        (param->subevent != 0xFF && param->subevent > LE_PAWR_SUBEVENT_MAX)) {
        return BT_BAD_PARAM;
    }
    return BT_SUCCESS;
}

// Range-checks one connection parameter set. Mirrors the set checks of the
// [v1] sender (hci_cmd_le_controller.c); keep the two in sync.
static int HciLeExtendedCreateConnectionV2CheckSet(const HciLeConnectionParamSet *set)
{
    if (set->scanInterval < LE_SCAN_INTERVAL_MIN ||
        set->scanInterval > LE_SCAN_INTERVAL_MAX ||
        set->scanWindow < LE_SCAN_WINDOW_MIN ||
        set->scanWindow > LE_SCAN_WINDOW_MAX ||
        set->scanWindow > set->scanInterval ||
        set->connIntervalMin < LE_CONN_INTERVAL_MIN ||
        set->connIntervalMin > LE_CONN_INTERVAL_MAX ||
        set->connIntervalMax < LE_CONN_INTERVAL_MIN ||
        set->connIntervalMax > LE_CONN_INTERVAL_MAX ||
        set->connIntervalMin > set->connIntervalMax ||
        set->connLatency > LE_CONN_LATENCY_MAX ||
        set->supervisionTimeout < LE_CONN_SUPERVISION_TIMEOUT_MIN ||
        set->supervisionTimeout > LE_CONN_SUPERVISION_TIMEOUT_MAX ||
        set->minimumCELength > set->maximumCELength) {
        return BT_BAD_PARAM;
    }
    return BT_SUCCESS;
}

// Checks the PHY bit field, counts the connection parameter sets it names
// (one per set bit, ascending PHY bit order) and range-checks every set.
static int HciLeExtendedCreateConnectionV2CheckSets(
    const HciLeExtendedCreateConnectionV2Param *param, uint8_t *countOfSets)
{
    // Mirrors HciLeExtendedCreateConnectionCheck/CheckSets of the [v1] sender
    // (hci_cmd_le_controller.c); keep the two in sync.
    if ((param->initiatingPhys & ~LE_INITIATING_PHYS_MASK) != 0 ||
        param->initiatingPhys == 0 || param->sets == NULL) {
        return BT_BAD_PARAM;
    }
    uint8_t numSets = 0;
    if (param->initiatingPhys & LE_1M_PHY) {
        numSets++;
    }
    if (param->initiatingPhys & LE_2M_PHY) {
        numSets++;
    }
    if (param->initiatingPhys & LE_CODED_PHY) {
        numSets++;
    }
    if (param->initiatingFilterPolicy > LE_FILTER_POLICY_MAX ||
        param->ownAddressType > LE_OWN_ADDRESS_TYPE_MAX ||
        param->peerAddressType > LE_PEER_ADDRESS_TYPE_MAX) {
        return BT_BAD_PARAM;
    }
    for (uint8_t i = 0; i < numSets; i++) {
        if (HciLeExtendedCreateConnectionV2CheckSet(&param->sets[i]) != BT_SUCCESS) {
            return BT_BAD_PARAM;
        }
    }
    *countOfSets = numSets;
    return BT_SUCCESS;
}

// Builds and sends the command frame: the [v2] Advertising_Handle/Subevent
// front pair, then the [v1] fields and the connection parameter sets.
static int HciSendLeExtendedCreateConnectionV2Frame(
    const HciLeExtendedCreateConnectionV2Param *param, uint8_t numSets)
{
    const size_t length = 2 + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(HciBdAddr) +
                          sizeof(uint8_t) + sizeof(HciLeConnectionParamSet) * numSets;
    uint8_t *buf = MEM_MALLOC.alloc(length);
    if (buf == NULL) {
        return BT_NO_MEMORY;
    }

    size_t index = 0;
    buf[index++] = param->advertisingHandle;
    buf[index++] = param->subevent;
    buf[index++] = param->initiatingFilterPolicy;
    buf[index++] = param->ownAddressType;
    buf[index++] = param->peerAddressType;
    (void)memcpy_s(buf + index, length - index, param->peerAddress.raw, sizeof(HciBdAddr));
    index += sizeof(HciBdAddr);
    buf[index++] = param->initiatingPhys;

    for (uint8_t i = 0; i < numSets; i++) {
        if (memcpy_s(buf + index, length - index, param->sets + i, sizeof(HciLeConnectionParamSet)) != EOK) {
            MEM_MALLOC.free(buf);
            return BT_OPERATION_FAILED;
        }
        index += sizeof(HciLeConnectionParamSet);
    }

    HciCmd *cmd = HciAllocCmd(HCI_LE_EXTENDED_CREATE_CONNECTION_V2, (void *)buf, length);
    if (cmd == NULL) {
        MEM_MALLOC.free(buf);
        return BT_NO_MEMORY;
    }
    int result = HciSendCmd(cmd);

    MEM_MALLOC.free(buf);

    return result;
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.66 LE Extended Create Connection Command [v2]
// Same command as 0x0043 plus the Advertising_Handle/Subevent pair that
// connects from a PAwR train. Both parameters 0xFF means "not used" and the
// command then behaves like [v1] (create a connection to a connectable
// advertiser). With both valid the Controller ignores Initiator_Filter_
// Policy, Initiating_PHYs and the scan parameters while still using the
// connection parameter sets - so the sets array keeps the [v1] contract (one
// entry per bit set in initiatingPhys) and the caller picks connection
// parameters for the PHY the train operates on. The caller gates on the PAwR
// capabilities before using the PAwR form of this command
// (BTM_IsControllerSupportPawrAdvertiser / BTM_IsControllerSupportPawrScanner,
// which read the periodic-advertising-with-responses feature bits in
// btm_controller.c).
// Frame contract, identical to the [v1] sender: the one-octet
// Initiating_PHYs bit field demands one connection parameter set per set
// bit, so initiatingPhys 0 (and a NULL sets array) is rejected - such a
// frame would name no parameter set and is not interpretable by the
// Controller. Every set is still range-checked below although the PHY, scan
// and filter-policy fields are ignored by the Controller when the PAwR pair
// is valid: the checks mirror the [v1] sender and keep one acceptance rule
// per command form.
// Advertising_Handle/Subevent front pair of the [v2] command: both 0xFF means
// "not used" and both shall then be valid values (7.8.66); a 0xFF on exactly
// one of the pair is not allowed. Mirrors the pair handling of the [v1] check
// split below; keep the two senders in sync.
int HCI_LeExtendedCreateConnectionV2(const HciLeExtendedCreateConnectionV2Param *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    int ret = HciLeExtendedCreateConnectionV2CheckPair(param);
    if (ret != BT_SUCCESS) {
        return ret;
    }
    uint8_t numSets = 0;
    ret = HciLeExtendedCreateConnectionV2CheckSets(param, &numSets);
    if (ret != BT_SUCCESS) {
        return ret;
    }
    return HciSendLeExtendedCreateConnectionV2Frame(param, numSets);
}

// BLUETOOTH SPECIFICATION Version 5.4 | Vol 4, Part E
// 7.8.61 LE Set Periodic Advertising Parameters Command [v2]
// Same checks as the [v1] sender plus the 5.4 PAwR tail. Note the [v2]
// Periodic_Advertising_Properties mask below: the 5.4 field table defines
// only bit 6 (Include TxPower), which is also the [v1] encoding, while the
// [v1] sender in hci_cmd_le_controller.c masks bit 0 - the two are kept
// separate on purpose, the [v1] path is not changed here. Timing relations
// between the PAwR tail and the advertising interval
// (Subevent_Interval <= Periodic_Advertising_Interval_Min / Num_Subevents,
// Response_Slot_Delay < Subevent_Interval) are enforced by the Controller;
// the caller (PAwR advertiser state machine) configures them coherently.
int HCI_LeSetPeriodicAdvertisingParametersV2(const HciLeSetPeriodicAdvertisingParametersV2Param *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (param->advertisingHandle > PERIODIC_ADV_HANDLE_MAX ||
        param->periodicAdvertisingIntervalMin < PERIODIC_ADV_INTERVAL_MIN ||
        param->periodicAdvertisingIntervalMin > PERIODIC_ADV_INTERVAL_MAX ||
        param->periodicAdvertisingIntervalMax < PERIODIC_ADV_INTERVAL_MIN ||
        param->periodicAdvertisingIntervalMax > PERIODIC_ADV_INTERVAL_MAX ||
        param->periodicAdvertisingIntervalMin > param->periodicAdvertisingIntervalMax ||
        (param->periodicAdvertisingProperties & ~LE_SET_PERIODIC_SYNC_SUBEVENT_PROPERTIES_TX_POWER) != 0 ||
        param->numSubevents > 0x80 ||
        // 0x06-0xFF; 0x00-0x05 are only meaningful when numSubevents is 0x00,
        // in which case the whole tail is ignored by the Controller.
        (param->numSubevents > 0 && param->subeventInterval < PERIODIC_ADV_SUBEVENT_INTERVAL_MIN) ||
        // When numSubevents or numResponseSlots is 0x00 the Controller shall
        // ignore the Response_Slot_Delay/Response_Slot_Spacing parameters
        // (7.8.61 [v2]); Response_Slot_Spacing is additionally ignored when
        // Num_Response_Slots is 0x01. Their reserved values - 0xFF for the
        // delay, 0x01 for the spacing (0x00 means no response slots) - are
        // therefore only rejected while the parameters are meaningful.
        (param->numSubevents > 0 && param->numResponseSlots > 0 && param->responseSlotDelay == 0xFF) ||
        (param->numSubevents > 0 && param->numResponseSlots > 0x01 && param->responseSlotSpacing == 0x01)) {
        return BT_BAD_PARAM;
    }

    HciCmd *cmd = HciAllocCmd(
        HCI_LE_SET_PERIODIC_ADVERTISING_PARAMETERS_V2,
        (void *)param, sizeof(HciLeSetPeriodicAdvertisingParametersV2Param));
    if (cmd == NULL) {
        return BT_NO_MEMORY;
    }
    return HciSendCmd(cmd);
}
