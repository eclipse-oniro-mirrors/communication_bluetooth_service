/*
 * Copyright (C) 2021-2026 Huawei Device Co., Ltd.
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
 * Decision-Based Advertising Filtering (DBAF) LE Extended Create Connection
 * helpers. Split out of btm_acl.c to keep the source file within the size
 * limit. The shared auto-connect lock and Extended Create Connection helper
 * live in btm_acl.c and are exported through btm_acl_def.h.
 */

#include "btm_acl.h"

#include "hci/hci.h"
#include "hci/hci_le_controller_6_0.h"
#include "platform/include/mutex.h"

#include "btm.h"
#include "btm_acl_def.h"
#include "btm_controller.h"
#include "btm_wl.h"

// Bluetooth 6.0 | Vol 4, Part E, 7.8.66 (p2637): single-shot LE Extended Create
// Connection issued with one of the decision Initiator_Filter_Policy values
// 0x02-0x04 (field meanings in the 7.8.66 value table; the backing Link-Layer
// initiator modes are listed in Vol 6 Part B 4.3.4, p3035-3036). The Controller
// picks the peer from decision PDUs / the accept list, so no specific BtAddr and
// no CONNECTING ACL record exist up front; the existing LE Enhanced Connection
// Complete handler creates the record when the connection succeeds. Like the
// whitelist auto-connect above (0x00/0x01) it is serialized under
// g_autoConnectLock; otherwise this path is fire-and-forget on the Controller
// state. Policy 0x03 (FAL All-PDUs) selects advertisers from the accept list
// only; with an empty accept list no peer can ever connect, so the command is
// not submitted and BT_SUCCESS is returned (mirroring the whitelist
// auto-connect); 0x02/0x04 process decision PDUs from any advertiser and stay
// valid with an empty accept list.
int BtmLeExtendedConnectByDecision(uint8_t initiatorFilterPolicy)
{
    if (initiatorFilterPolicy < LE_INITIATOR_FILTER_POLICY_DECISION_PDU_ONLY ||
        initiatorFilterPolicy > LE_INITIATOR_FILTER_POLICY_DECISION_ALL_PDU) {
        return BT_BAD_PARAM;
    }

    if (!BtmAclIsInitialized()) {
        return BT_BAD_STATUS;
    }

    if (!BTM_IsControllerSupportLeDecisionBasedAdvertisingFiltering()) {
        return BT_NOT_SUPPORT;
    }

    int result;
    MutexLock(g_autoConnectLock);
    if (initiatorFilterPolicy == LE_INITIATOR_FILTER_POLICY_FAL_ALL_PDU &&
        BtmGetDeviceCountInWhiteList() == 0) {
        MutexUnlock(g_autoConnectLock);
        return BT_SUCCESS;
    }
    result = BtmLeExtendedCreateConnection(initiatorFilterPolicy);
    MutexUnlock(g_autoConnectLock);
    return result;
}

// Bluetooth 6.0 | Vol 4, Part E, 7.8.13 (p2514): LE_Create_Connection_Cancel
// covers the Extended Create Connection too, so this cancels the decision
// initiator started by BtmLeExtendedConnectByDecision. The decision initiator
// has no BtAddr and no CONNECTING ACL record, so BTM_LeCancelConnect cannot name
// it and 0x000E is sent unconditionally. When no initiator is pending the
// Controller answers Command Disallowed (0x0C), whose completion event has no
// registrant in this stack and is dropped, so a stray cancel cannot error the
// caller, touch an established connection, or wedge the stack.
// It does not stop a whitelist auto-connect either. The initiator slot is shared
// and the Host cannot tell the two kinds apart, so cancelling either one is
// reported as a Connection Complete with status 0x02, where 0x02 means
// restart of whitelist auto-connect when the accept list is non-empty:
// with a non-empty accept list the auto-connect is started again, aimed at a
// device from that list. That restart is this stack's own way of reprogramming
// the list (BtmLeCreateConnection cancels on purpose through the same path), so
// stopping an auto-connection means emptying the accept list - another cancel,
// through this entry point or through the whitelist-gated BtmStopAutoConnection,
// only repeats it.
int BtmLeExtendedConnectByDecisionCancel(void)
{
    if (!BtmAclIsInitialized()) {
        return BT_BAD_STATUS;
    }

    if (!BTM_IsControllerSupportLeDecisionBasedAdvertisingFiltering()) {
        return BT_NOT_SUPPORT;
    }

    int result;
    MutexLock(g_autoConnectLock);
    result = HCI_LeCreateConnectionCancel();
    MutexUnlock(g_autoConnectLock);
    return result;
}
