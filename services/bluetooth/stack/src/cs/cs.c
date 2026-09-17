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

#include "cs.h"

#include <securec.h>
#include <string.h>

#include "cs_internal.h"

#include "log.h"
#include "module.h"

#include "btm/btm_acl.h"
#include "btm/btm_acl_def.h"
#include "btm/btm_thread.h"
#include "hci/cmd/hci_cmd_failure.h"
#include "hci/evt/hci_evt.h"
#include "hci/hci.h"
#include "hci/hci_error.h"
#include "hci/hci_le_controller_6_0.h"
#include "platform/include/mutex.h"

// A row is kept past its connection (remembering a peer across a reconnect is what
// the write-back cache is for), so this caps the remembered-peer set rather than the
// concurrent-connection set. Sized to BTM_MAX_ACL so a live connection is never the
// row dropped when the table is full; CsFindOrCreateRemoteEntry recycles a row whose
// connection is gone.
#define CS_REMOTE_CACHE_SIZE BTM_MAX_ACL
// One row per connection that can exist at once: must cover the ACL layer's cap so a
// smaller table cannot drop the C-3 row of a live connection.
#define CS_CONNECTION_TABLE_SIZE BTM_MAX_ACL
#define CS_HANDLE_NONE 0xFFFF  // outside the valid 0x0000-0x0EFF handle range
// Config_ID 0x00-0x03, one slot per identifier (7.8.137 p2823 / 7.8.138 p2829).
#define CS_CONFIG_SLOT_COUNT (LE_CS_CONFIG_ID_MAX + 1)

typedef struct {
    BtAddr addr;
    HciLeCsCapabilities capabilities;  // the 0x2C result, readable only while capValid
    uint16_t learnedOnHandle;  // handle of the 0x2C/write-back that populated it
    bool inUse;                // the row is occupied by this peer address
    bool capValid;             // a capability body is held
    // The peer's mode-0 FAE table, learned by a successful 0x2D and written back with
    // 0x008F. It rides the row rather than a table of its own because it is the same
    // kind of snapshot: remembered per peer address, across a reconnect, so the
    // Controller does not have to run the FAE Table Request procedure again (7.8.135
    // p2819). inUse says the row is taken, capValid/faeValid say which of the two
    // payloads in it are real - they are learned independently, a capability read with
    // no FAE read being an ordinary sequence, so one flag could not stand for both.
    // Each learnedOnHandle is that payload's "the Controller already holds it for this
    // handle" marker, the condition that turns a write-back into 0x0C (7.8.132 p2808 /
    // 7.8.136 p2820).
    int8_t faeTable[LE_CS_REMOTE_FAE_TABLE_LEN];
    bool faeValid;
    uint16_t faeLearnedOnHandle;
} CsRemoteCacheEntry;

// One CS configuration slot. inUse mirrors "the local Controller holds this
// configuration"; lastConfig is the create mirror of 7.7.65,42 p2436, valid only
// while inUse is set (the spec marks every octet after Action as ignored on the
// remove path, so a removed slot has no mirror).
// The C-5 fields describe the CS procedure on the same Config_ID. procParamsSet
// mirrors "0x0093 has been accepted for this Config_ID", the precondition 0x0094
// needs (7.8.141 p2838) and the reason a disable does not clear it - only a 0x0091
// remove deletes the parameters (7.8.138 p2828). procEnable holds what the 0x30
// reported, which is the schedule actually in use, and is valid only while enabled
// is set (State 0x00 leaves the whole tail unspecified, 7.7.65,43 p2442). The
// 0x0093 request itself is not mirrored: the Controller never echoes it and no
// Host-side decision reads it. CsResetSlot is the single clear point, so a remove
// drops all of it at once.
typedef struct {
    bool inUse;
    HciLeCsConfigCompleteEventParam lastConfig;
    bool procParamsSet;
    bool enabled;
    HciLeCsProcedureEnableCompleteEventParam procEnable;
} CsConfigSlot;

// Per-connection CS state table, one slot per active handle (C-3). cs.c owns it: a row is
// claimed by CsOpenSession for a session and created on demand by the per-connection event
// handlers and CsSetProcedureParameters, all through CsFindOrCreateConnection - no query
// path may create one - and returned by CsResetConnection, which is also what the
// Disconnection Complete handler below and CsCloseSession both call - the two ways a CS
// session ends, one because the handle went away and one because the Host said it was
// done. Whichever came first wins and the other becomes a no-op: both take the row out
// of the claimed state, and neither acts on a row that is already out of it, which is
// what sessionOpened below says.
typedef struct {
    uint16_t connectionHandle;
    // A CS session the Host opened on this handle, as opposed to the row itself, which
    // the per-connection event handlers and CsSetProcedureParameters create on demand.
    // The row is not the session: a 0x2C, 0x2F, 0x30 or 0x0093 arrives for a handle the
    // Host never claimed and still takes a row for it. CsCloseSession and the announce in
    // the Disconnection Complete handler read this bit rather than the row, so closing a
    // handle that was only heard from is refused and losing one is not announced as a
    // session ending. Same shape as capValid/faeValid on CsRemoteCacheEntry and running
    // on CsTestState: one bit for the thing the entry point is about.
    bool sessionOpened;
    // A 0x0093 is outstanding on this handle and procParamsPendingId names the
    // Config_ID it carried (C-5). The 0x0093 Command_Complete returns Status +
    // Connection_Handle and does not echo Config_ID (7.8.140 p2836/p2837), so the
    // slot it closes can only be found from what was sent. Per connection, not per
    // engine: the CC is matched by Connection_Handle (hci_evt_le_cmd_complete.c),
    // so two connections can each have one in flight and neither would be confused
    // for the other. A second 0x0093 on the same handle while this is set is
    // refused by CsSetProcedureParameters rather than overwriting the record.
    bool procParamsPending;
    uint8_t procParamsPendingId;
    CsConfigSlot slots[CS_CONFIG_SLOT_COUNT];
    // The CS subevent report being assembled for this connection (C-6). It hangs off
    // the row because a subevent belongs to one connection and its 0x32 continuations
    // arrive interleaved with other connections' events: one module-wide buffer could
    // not tell them apart. The report behind it is allocated on the first subevent and
    // freed with the row, so it costs nothing until the Controller reports one.
    CsResultStream resultStream;
} CsConnection;

static HciLeCsCapabilities g_localCapabilities;
static bool g_localCapabilitiesValid;
static CsRemoteCacheEntry g_remoteCache[CS_REMOTE_CACHE_SIZE];
static CsConnection g_connections[CS_CONNECTION_TABLE_SIZE];
static CsSessionSink g_sessionSink;

static bool CsAddrEqual(const BtAddr *a, const BtAddr *b)
{
    return a->type == b->type && memcmp(a->addr, b->addr, sizeof(a->addr)) == 0;
}

static CsRemoteCacheEntry *CsFindRemoteEntry(const BtAddr *addr)
{
    for (int i = 0; i < CS_REMOTE_CACHE_SIZE; i++) {
        if (g_remoteCache[i].inUse && CsAddrEqual(&g_remoteCache[i].addr, addr)) {
            return &g_remoteCache[i];
        }
    }
    return NULL;
}

// Reset a row before it is reused for a different peer.
// The caller overwrites addr, so the old address is not preserved.
// Clearing the whole row prevents stale capabilities or FAE data from being seen
// or written back for the new peer. Both handle markers return to CS_HANDLE_NONE.
static void CsResetRemoteEntry(CsRemoteCacheEntry *entry)
{
    (void)memset_s(entry, sizeof(*entry), 0, sizeof(*entry));
    entry->learnedOnHandle = CS_HANDLE_NONE;
    entry->faeLearnedOnHandle = CS_HANDLE_NONE;
}

// Find-or-create by peer address. When every row is in use, a row whose connection
// has already gone (both learnedOnHandle markers cleared) is recycled: the Controller
// no longer holds that peer's state, so the row is worth no more than the incoming
// peer's, and the old peer is simply re-learned the next time it connects. Only rows
// still pinned to a live connection are undroppable - they are the ones whose cached
// caps or FAE table the Controller can still be asked to use. Both markers are
// consulted because the two payloads are learned independently: a row can be pinned by
// an FAE read alone, and recycling it would drop a table that Controller still holds.
// Without this recycle the table wedges once CS_REMOTE_CACHE_SIZE distinct peers have
// ever connected, and every later peer silently loses the write-back cache for the
// rest of the service lifetime, which is what the cache exists to provide. Returns a
// row left free of both payloads for the caller to fill in (its own payload, its own
// marker); NULL only when every row is pinned to a live connection.
static CsRemoteCacheEntry *CsFindOrCreateRemoteEntry(const BtAddr *addr)
{
    CsRemoteCacheEntry *entry = CsFindRemoteEntry(addr);
    if (entry != NULL) {
        return entry;
    }
    CsRemoteCacheEntry *recycled = NULL;
    for (int i = 0; i < CS_REMOTE_CACHE_SIZE; i++) {
        if (!g_remoteCache[i].inUse) {
            g_remoteCache[i].addr = *addr;
            return &g_remoteCache[i];
        }
        if (recycled == NULL && g_remoteCache[i].learnedOnHandle == CS_HANDLE_NONE &&
            g_remoteCache[i].faeLearnedOnHandle == CS_HANDLE_NONE) {
            recycled = &g_remoteCache[i];
        }
    }
    if (recycled != NULL) {
        LOG_INFO("%{public}s: remote cache full, recycled a gone connection's row", __FUNCTION__);
        // Hand the row back in the free-slot shape, not as a live entry: until the
        // caller fills it it still holds the evicted peer's payloads.
        CsResetRemoteEntry(recycled);
        recycled->addr = *addr;
        return recycled;
    }
    LOG_WARN("%{public}s: remote cache full, all rows on live connections, dropping update", __FUNCTION__);
    return NULL;
}

// Return a row to the free-list shape. A claimed row, a row released by
// disconnect and the table at init all want every per-connection field back at its
// initial value, so they share this one reset point rather than repeating field
// assignments: a field added to CsConnection cannot be forgotten at one of the
// three call sites. The empty-row sentinel is 0xFFFF, so a plain zero fill is not
// an empty row and the handle is restored afterwards.
// The subevent report is the one member the zero fill cannot dispose of, so it is
// released first: without this the row would drop its only pointer to the buffer and
// the whole result path would leak one report per connection.
// The zero fill is also what clears sessionOpened, and zero is the right value: the row
// in its free-list shape is one no Host session was opened on.
static void CsResetConnection(CsConnection *connection)
{
    CsResultReleaseStream(&connection->resultStream);
    (void)memset_s(connection, sizeof(*connection), 0, sizeof(*connection));
    connection->connectionHandle = CS_HANDLE_NONE;
}

// CS_HANDLE_NONE is the free-row sentinel, never a lookup key: free rows carry it,
// so a lookup for it would match the first free row and hand back a row that is not
// a connection. Callers reach here only after a live handle has been established
// (the ACL lookup, or an event whose status is 0x00), so the sentinel cannot arrive
// today - the guard keeps that from becoming a load-bearing accident.
static CsConnection *CsFindConnection(uint16_t connectionHandle)
{
    if (connectionHandle == CS_HANDLE_NONE) {
        return NULL;
    }
    for (int i = 0; i < CS_CONNECTION_TABLE_SIZE; i++) {
        if (g_connections[i].connectionHandle == connectionHandle) {
            return &g_connections[i];
        }
    }
    return NULL;
}

static CsConnection *CsFindOrCreateConnection(uint16_t connectionHandle)
{
    // The free-row marker is not a connection: a row created under it would carry the
    // marker as its handle, so every later lookup would read it as free.
    if (connectionHandle == CS_HANDLE_NONE) {
        return NULL;
    }
    CsConnection *connection = CsFindConnection(connectionHandle);
    if (connection != NULL) {
        return connection;
    }
    for (int i = 0; i < CS_CONNECTION_TABLE_SIZE; i++) {
        if (g_connections[i].connectionHandle == CS_HANDLE_NONE) {
            CsResetConnection(&g_connections[i]);
            g_connections[i].connectionHandle = connectionHandle;
            return &g_connections[i];
        }
    }
    LOG_WARN("%{public}s: connection table full, dropping handle 0x%04x",
        __FUNCTION__, connectionHandle);
    return NULL;
}

// A removed configuration takes its slot state with it - the mirror and, once C-5
// adds it, that slot's procedure parameters (7.8.138 p2828: the Controller "shall
// delete any CS procedure related parameters set using the
// HCI_LE_CS_Set_Procedure_Parameters command for the CS configuration identified by
// Config_ID", and the Host mirror is dropped in step). This is the only clear
// point for slot state, so the two cannot drift apart.
static void CsResetSlot(CsConfigSlot *slot)
{
    (void)memset_s(slot, sizeof(*slot), 0, sizeof(*slot));
}

static void CsOnReadLocalSupportedCapabilitiesComplete(
    const HciLeCsReadLocalSupportedCapabilitiesReturnParam *returnParam)
{
    if (returnParam == NULL || returnParam->status != HCI_SUCCESS) {
        return;
    }
    g_localCapabilities = returnParam->capabilities;
    g_localCapabilitiesValid = true;
}

// The per-connection CS event handlers share one shape: record what the event means for
// the engine's own state, then report it to the session sink on the way out. One exit,
// so a consumer hears about every event exactly once - including the failures, which are
// exactly the ones no state change records. Two are outside that shape, listed here
// once rather than at each site: the 0x0089 completion above, which is module-wide rather
// than per-connection and has CsGetLocalCapabilities as its reader; and the two
// 0x008D/0x0092 completions below, which record nothing because the Controller owns what
// they set, but do report, because their refusals are the Controller's to make. The
// 0x008B/0x008F write-backs follow the shared shape: success records learnedOnHandle /
// faeLearnedOnHandle, and every status (including Controller 0x0C when a CS configuration
// already exists) is reported through the session sink.
static void CsOnReadRemoteSupportedCapabilitiesComplete(
    const HciLeCsReadRemoteSupportedCapabilitiesCompleteEventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }
    if (eventParam->status == HCI_SUCCESS) {
        BtAddr addr;
        if (BtmGetAclAddressByHandle(eventParam->connectionHandle, &addr) == BT_SUCCESS) {
            CsRemoteCacheEntry *entry = CsFindOrCreateRemoteEntry(&addr);
            if (entry != NULL) {
                entry->capabilities = eventParam->capabilities;
                entry->learnedOnHandle = eventParam->connectionHandle;
                entry->capValid = true;
                entry->inUse = true;
            }
        }
    }
    if (g_sessionSink.remoteCapabilitiesComplete != NULL) {
        g_sessionSink.remoteCapabilitiesComplete(eventParam);
    }
}

static void CsOnWriteCachedRemoteSupportedCapabilitiesComplete(
    const HciLeCsWriteCachedRemoteSupportedCapabilitiesReturnParam *returnParam)
{
    if (returnParam == NULL) {
        return;
    }
    if (returnParam->status == HCI_SUCCESS) {
        BtAddr addr;
        if (BtmGetAclAddressByHandle(returnParam->connectionHandle, &addr) == BT_SUCCESS) {
            CsRemoteCacheEntry *entry = CsFindRemoteEntry(&addr);
            if (entry != NULL) {
                entry->learnedOnHandle = returnParam->connectionHandle;
            }
        }
    }
    if (g_sessionSink.writeCachedRemoteCapabilitiesComplete != NULL) {
        g_sessionSink.writeCachedRemoteCapabilitiesComplete(returnParam);
    }
}

// A successful 0x2D caches the peer's 72-B mode-0 FAE table by peer address, so a later
// reconnect can write it back with 0x008F instead of running the Mode-0 FAE Table
// Request procedure again (7.8.135 p2819). The table is stored only under Status 0x00:
// 0x11 is the Controller saying the peer emits a zero FAE - the No_FAE capability
// (Subfeatures_Supported bit 1, 7.7.65,39 p2431) - and every other status is a failure,
// and 7.7.65,40 gives the 72 octets no value on either, so nothing is cached and no
// zeroed table is invented here. That answer is also not pre-empted: a cached
// capability body naming No_FAE does not stop the read from being a legitimate request
// (p2819), so the Controller owns the answer and its 0x11 is what the caller sees.
static void CsOnReadRemoteFaeTableComplete(
    const HciLeCsReadRemoteFaeTableCompleteEventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }
    if (eventParam->status == HCI_SUCCESS) {
        BtAddr addr;
        if (BtmGetAclAddressByHandle(eventParam->connectionHandle, &addr) == BT_SUCCESS) {
            CsRemoteCacheEntry *entry = CsFindOrCreateRemoteEntry(&addr);
            if (entry != NULL) {
                (void)memcpy_s(entry->faeTable, sizeof(entry->faeTable),
                    eventParam->remoteFaeTable, sizeof(eventParam->remoteFaeTable));
                entry->faeLearnedOnHandle = eventParam->connectionHandle;
                entry->faeValid = true;
                entry->inUse = true;
            }
        }
    }
    if (g_sessionSink.remoteFaeTableComplete != NULL) {
        g_sessionSink.remoteFaeTableComplete(eventParam);
    }
}

// A successful 0x008F means the Controller has taken this peer's table for that
// handle, which is the condition 7.8.136 p2820 answers with 0x0C on a second
// write-back - so the row records the handle the table now sits in the Controller
// under. The FAE counterpart of what the 0x008B handler above records, and read by
// CsWriteBackRemoteFaeTable for the same reason. Non-success (including 0x0C when a
// CS configuration already exists) is reported through the session sink.
static void CsOnWriteCachedRemoteFaeTableComplete(
    const HciLeCsWriteCachedRemoteFaeTableReturnParam *returnParam)
{
    if (returnParam == NULL) {
        return;
    }
    if (returnParam->status == HCI_SUCCESS) {
        BtAddr addr;
        if (BtmGetAclAddressByHandle(returnParam->connectionHandle, &addr) == BT_SUCCESS) {
            CsRemoteCacheEntry *entry = CsFindRemoteEntry(&addr);
            if (entry != NULL) {
                entry->faeLearnedOnHandle = returnParam->connectionHandle;
            }
        }
    }
    if (g_sessionSink.writeCachedRemoteFaeTableComplete != NULL) {
        g_sessionSink.writeCachedRemoteFaeTableComplete(returnParam);
    }
}

// 0x2E (7.7.65,41 p2434): CS Security Start completed or the Controller answered a
// peer security request. The engine keeps no Host-side latch of the outcome - LL
// §5.1.26 requires Security Start complete before CS Start, and that gate is
// enforced in the Controller when a later procedure is enabled. Nonzero status
// (including Command_Status failures funneled here by the failure path) is
// reported to the session sink so the caller can re-issue 0x008C once the link
// state is fixed (7.8.133 p2815).
static void CsOnSecurityEnableComplete(
    const HciLeCsSecurityEnableCompleteEventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }
    if (g_sessionSink.securityEnableComplete != NULL) {
        g_sessionSink.securityEnableComplete(eventParam);
    }
}

// 0x2F drives the config slot table (C-4). Action 0x01 (created) stores the mirror,
// Action 0x00 (removed) clears the slot; either is honored only on Status 0x00,
// because a nonzero status
// means the Controller changed nothing - a failed create created nothing, and a
// failed remove left the configuration in place (7.8.138 p2828 answers 0x12 for a
// Config_ID that does not exist and 0x0C while a procedure is enabled, neither of
// which removes what is held). A reserved Config_ID or Action is rejected outright
// rather than folded into a branch: Vol 1, Part E, 2.4.1 (p398) has a device that
// receives a reserved value reject the value and the structure containing it as
// erroneous. The event also arrives with no Host command outstanding when the
// Controller answers a peer's configuration request locally, and that path updates
// the table the same way - the Host did not ask, but the Controller now holds the
// configuration. A reserved value is dropped before the session sink sees it, unlike
// the 0x0093 completion below: a frame the engine refuses to believe is not one to hand
// a consumer as though it had been believed.
static void CsOnConfigComplete(const HciLeCsConfigCompleteEventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }
    if (eventParam->configId > LE_CS_CONFIG_ID_MAX ||
        (eventParam->action != LE_CS_CONFIG_ACTION_CREATED &&
            eventParam->action != LE_CS_CONFIG_ACTION_REMOVED)) {
        LOG_WARN("%{public}s: reserved Config_ID 0x%02x / Action 0x%02x, event dropped",
            __FUNCTION__, eventParam->configId, eventParam->action);
        return;
    }
    if (eventParam->status == HCI_SUCCESS) {
        CsConnection *connection = CsFindOrCreateConnection(eventParam->connectionHandle);
        if (connection != NULL) {
            CsConfigSlot *slot = &connection->slots[eventParam->configId];
            if (eventParam->action == LE_CS_CONFIG_ACTION_CREATED) {
                slot->inUse = true;
                slot->lastConfig = *eventParam;
            } else {
                CsResetSlot(slot);
            }
        }
    }
    if (g_sessionSink.configComplete != NULL) {
        g_sessionSink.configComplete(eventParam);
    }
}

// 0x0093 Command_Complete carries only Status + Connection_Handle (7.8.140
// p2836/p2837); Config_ID comes from the per-handle record written by
// CsSetProcedureParameters. The record is cleared on both success and failure so a
// corrected 0x0093 can be reissued; the failure row also routes timeouts here to
// prevent a lost completion from wedging the pending state.

// DESIGN NOTE: CS_HANDLE_NONE means the completion could not be attributed to one
// connection (short body from HCI, or allocation failure in the failure row). The
// only safe action is to clear every procParamsPending bit: leaving one set would
// block later 0x0093 requests on that handle with BT_ALREADY until disconnect.
// This global conservative clear is intentional; the frame is not reported to the
// session sink because there is no handle to attribute it to.
static void CsOnSetProcedureParametersComplete(
    const HciLeCsSetProcedureParametersReturnParam *returnParam)
{
    if (returnParam == NULL) {
        return;
    }
    // The frame is not attributable to one connection, so every record is cleared.
    if (returnParam->connectionHandle == CS_HANDLE_NONE) {
        LOG_WARN("%{public}s: unattributable 0x0093 completion, clearing all pending records", __FUNCTION__);
        for (int i = 0; i < CS_CONNECTION_TABLE_SIZE; i++) {
            g_connections[i].procParamsPending = false;
        }
        return;
    }
    CsConnection *connection = CsFindConnection(returnParam->connectionHandle);
    if (connection == NULL || !connection->procParamsPending) {
        LOG_WARN("%{public}s: no 0x0093 outstanding on handle 0x%04x, no record taken",
            __FUNCTION__, returnParam->connectionHandle);
    } else {
        uint8_t configId = connection->procParamsPendingId;
        connection->procParamsPending = false;
        // Only mark set if the slot is still in use; a 0x0091 remove may have cleared it.
        if (returnParam->status == HCI_SUCCESS && connection->slots[configId].inUse) {
            connection->slots[configId].procParamsSet = true;
        }
    }
    if (g_sessionSink.procedureParametersComplete != NULL) {
        g_sessionSink.procedureParametersComplete(returnParam);
    }
}

// The 0x008D completion records nothing: the defaults live in the Controller, are
// connection-scoped, and no Host-side decision reads them back (7.8.134 p2816). It is
// reported anyway, which is where it parts company with the 0x008B/0x008F handlers
// above: a refused 0x008D answers 0x11 for a value the Controller will not take and 0x12
// for a role it will not accept in the connection's current state, and nothing the Host
// holds predicts either, so dropping the completion would leave the caller with no way to
// learn the defaults never applied. A frame reaching here can still name CS_HANDLE_NONE:
// the failure row (a timeout, or a Command_Complete whose return body was empty) rebuilds
// the return from the original command, and a command whose parameter block could not be
// allocated carries no handle to read. A return body merely too short to hold a handle is
// dropped by HciEventOnLeCsSetDefaultSettingsComplete and never gets here at all.
static void CsOnSetDefaultSettingsComplete(const HciLeCsSetDefaultSettingsReturnParam *returnParam)
{
    if (returnParam == NULL) {
        return;
    }
    if (g_sessionSink.setDefaultSettingsComplete != NULL) {
        g_sessionSink.setDefaultSettingsComplete(returnParam);
    }
}

// The 0x0092 completion records nothing either - the classification is Controller-wide
// and outlives the link (7.8.139 p2830), so the Controller owns it and the Host keeps no
// copy - and its status is the whole result, which is why the return body carries no
// Connection_Handle and this handler has no unattributable case to route. It is reported
// for the same reason 0x008D is: 0x0C is a re-issue inside the Controller's 1-second
// window and 0x12 an invalid map, D5-1 decided not to track that window on the Host side,
// and without this report neither answer would reach anyone.
static void CsOnSetChannelClassificationComplete(const HciLeCsSetChannelClassificationReturnParam *returnParam)
{
    if (returnParam == NULL) {
        return;
    }
    if (g_sessionSink.setChannelClassificationComplete != NULL) {
        g_sessionSink.setChannelClassificationComplete(returnParam);
    }
}

// Apply the 0x30 outcome to a slot that is known to be in use. The enable branch copies
// the whole event; the disable branch clears procEnable and stores the identity fields.
// Even if clearing fails, slot->enabled must reflect the Controller-reported state so the
// engine does not present a disabled procedure as still active.
static void CsApplyProcedureEnableToSlot(CsConfigSlot *slot,
    const HciLeCsProcedureEnableCompleteEventParam *eventParam)
{
    bool enabled = (eventParam->state == LE_CS_PROCEDURE_ENABLED);
    if (enabled) {
        slot->procEnable = *eventParam;
    } else {
        errno_t err = memset_s(&slot->procEnable, sizeof(slot->procEnable), 0, sizeof(slot->procEnable));
        if (err != EOK) {
            LOG_WARN("%{public}s: memset_s failed: %{public}d", __FUNCTION__, err);
            slot->enabled = false;
            return;
        }
        slot->procEnable.status = eventParam->status;
        slot->procEnable.connectionHandle = eventParam->connectionHandle;
        slot->procEnable.configId = eventParam->configId;
        slot->procEnable.state = eventParam->state;
    }
    slot->enabled = enabled;
}

// 0x30 closes a 0x0094 and also reports a CS procedure the Controller started on its
// own after LL_CS_IND (7.8.141 p2839), so it is honored whenever it names a
// Config_ID the engine holds a slot for - the Config_ID is in the event body, unlike
// the 0x0093 return. A nonzero status changed nothing: the enable was refused (0x0C
// no procedure parameters set or already enabled, 0x12 unknown Config_ID) or the
// disable failed to take effect, and either way the previous state stands. State 0x00
// makes every field after it unspecified (7.7.65,43 p2442), so a disable stores the
// identity that names the slot and no schedule. A reserved Config_ID or State is
// dropped as erroneous rather than folded into a branch, the same rule
// CsOnConfigComplete applies to a reserved Action (Vol 1, Part E, 2.4.1 p398) - including
// the drop before the sink. A Config_ID the engine holds no slot for is reported all the
// same: the outcome belongs to whoever is waiting for it, and having nowhere to record it
// is the engine's gap and not the caller's.
static void CsOnProcedureEnableComplete(const HciLeCsProcedureEnableCompleteEventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }
    if (eventParam->configId > LE_CS_CONFIG_ID_MAX ||
        (eventParam->state != LE_CS_PROCEDURE_ENABLED && eventParam->state != LE_CS_PROCEDURE_DISABLED)) {
        LOG_WARN("%{public}s: reserved Config_ID 0x%02x / State 0x%02x, event dropped",
            __FUNCTION__, eventParam->configId, eventParam->state);
        return;
    }
    if (eventParam->status == HCI_SUCCESS) {
        CsConnection *connection = CsFindOrCreateConnection(eventParam->connectionHandle);
        if (connection != NULL) {
            CsConfigSlot *slot = &connection->slots[eventParam->configId];
            if (!slot->inUse) {
                LOG_WARN("%{public}s: handle 0x%04x Config_ID 0x%02x not held, 0x30 not recorded",
                    __FUNCTION__, eventParam->connectionHandle, eventParam->configId);
            } else {
                CsApplyProcedureEnableToSlot(slot, eventParam);
            }
        }
    }
    if (g_sessionSink.procedureEnableComplete != NULL) {
        g_sessionSink.procedureEnableComplete(eventParam);
    }
}

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.7.65,44 LE CS Subevent Result subevent (0x31). It opens a CS subevent and is the
// only event carrying the counters the report is labelled with; the step region it
// carries is handed to cs_result.c, which owns the decode and the aggregation.
// Connection_Handle 0x0FFF marks a CS test run (7.7.65,44 p2448) and is outside the
// 0x0000-0x0EFF range every link uses, so it is diverted before the lookup: left to
// CsFindOrCreateConnection it would create a row under a handle no Disconnection
// Complete will ever release, permanently costing one of the connection-table rows. A test result
// belongs to cs_test.c's single test record instead, which holds the Role and RTT_Type
// its 0x0095 was accepted with - the decode context this event does not carry.
static void CsOnSubeventResult(const HciLeCsSubeventResultEnvelope *envelope)
{
    if (envelope == NULL) {
        return;
    }
    if (envelope->head.connectionHandle == LE_CS_TEST_HANDLE) {
        CsTestOnSubeventResult(envelope);
        return;
    }
    CsConnection *connection = CsFindConnection(envelope->head.connectionHandle);
    if (connection == NULL) {
        LOG_WARN("%{public}s: no connection row for handle 0x%04x, subevent dropped",
            __FUNCTION__, envelope->head.connectionHandle);
        return;
    }
    if (envelope->head.configId > LE_CS_CONFIG_ID_MAX ||
        !connection->slots[envelope->head.configId].inUse) {
        LOG_WARN("%{public}s: handle 0x%04x Config_ID 0x%02x not held, subevent dropped",
            __FUNCTION__, envelope->head.connectionHandle, envelope->head.configId);
        return;
    }
    // The role and the RTT type come from the configuration the report is attributed
    // to, not from the event: a mode-0 step's length means different things to the
    // two roles (pp2451-2452), and only the configuration says which one is in
    // force. Both are read from the 0x2F mirror, so a report for a Config_ID the
    // engine holds no configuration for is refused above rather than decoded
    // against a guessed role.
    const HciLeCsConfigCompleteEventParam *config = &connection->slots[envelope->head.configId].lastConfig;
    if (CsResultAbsorbResult(&connection->resultStream, envelope, config->role, config->rttType) !=
        CS_RESULT_ENDED) {
        return;
    }
    CsResultEmit(&connection->resultStream);
    CsResultResetStream(&connection->resultStream);
}

// 7.7.65,45 LE CS Subevent Result Continue subevent (0x32): more steps of the subevent
// the last 0x31 opened, with no counters of its own. It is routed by the connection
// row's open stream. Spec ignores Config_ID only for the test handle 0x0FFF
// (7.7.65,45); on a live ACL the 0x32 Config_ID must match the open 0x31 stream, or
// a mismatched continuation would be decoded with the wrong role/rttType. A 0x32
// whose subevent has already ended (or never started) is dropped in AbsorbContinue.
// Handle 0x0FFF is diverted to the test's own stream, the same diversion the 0x31
// path makes.
static void CsOnSubeventResultContinue(const HciLeCsSubeventResultContinueEnvelope *envelope)
{
    if (envelope == NULL) {
        return;
    }
    if (envelope->head.connectionHandle == LE_CS_TEST_HANDLE) {
        CsTestOnSubeventResultContinue(envelope);
        return;
    }
    CsConnection *connection = CsFindConnection(envelope->head.connectionHandle);
    if (connection == NULL) {
        LOG_WARN("%{public}s: no connection row for handle 0x%04x, subevent dropped",
            __FUNCTION__, envelope->head.connectionHandle);
        return;
    }
    if (CsResultAbsorbContinue(&connection->resultStream, envelope) != CS_RESULT_ENDED) {
        return;
    }
    CsResultEmit(&connection->resultStream);
    CsResultResetStream(&connection->resultStream);
}

// The one sink the CS result path hands reports to. Registration releases every
// stream, so a consumer installed after traffic has run starts from empty buffers
// instead of inheriting a half-absorbed subevent; with no sink registered the engine
// drops subevent events rather than decoding and storing reports for nobody.
// The releases run under the event callback-list lock, because that is the lock the
// LE Meta dispatch holds across every callback it makes (the
// HCI_FOREACH_EVT_CALLBACKS_START/END pair in hci/evt/hci_evt.h): while it is held
// the HCI thread is provably outside CsResultAbsorbResult, CsResultAbsorbContinue and
// CsResultEmit, so no report can be freed under the decode that is still writing it.
// The caller must have reached CsEnable: HciInitEvent is what creates this lock and
// only HCI_Initialize runs it, while CsInitialize runs earlier, on BTM_Initialize's
// module loop. A registration placed there would find no lock and no event layer at
// all. This entry refuses in that case rather than proceed without the lock, because
// releasing a report unprotected is only safe while no dispatch can exist - and that
// no longer follows once MutexCreate has failed.
// A sink may re-register from inside its own callback. The lock is recursive, so that
// call runs, and the release it makes of the report being handed out is held back by
// cs_result.c until the callback returns (CsResultReleaseStream there), which is why
// the report cannot be cleared under the reader. One limit is deliberate: two caller
// threads registering at the same moment are outside this lock's reach - so a caller
// must not already hold a lock that the HCI dispatch path takes (the dispatch holds
// this one while it calls into the modules, which take their own locks underneath it).
void CsResultRegisterSink(CsResultSink sink)
{
    Mutex *lock = HciGetEventCallbackListLock();
    if (lock == NULL) {
        // No event layer yet: register nothing rather than release without the lock.
        LOG_WARN("%{public}s: HCI event layer not up, sink not registered", __FUNCTION__);
        return;
    }
    MutexLock(lock);
    for (int i = 0; i < CS_CONNECTION_TABLE_SIZE; i++) {
        CsResultReleaseStream(&g_connections[i].resultStream);
    }
    // The test stream is not a row's, so it is released here rather than in the loop -
    // and only its report: a test the Controller is still running outlives the sink
    // that was watching it, and its record is what decodes the results still to come.
    CsTestReleaseReport();
    CsResultSetSink(sink);
    MutexUnlock(lock);
}

// The session sink is a plain function-pointer struct: registration swaps it and touches
// nothing else, so there is no stream to release under the lock the way the result
// registration above has one. The lock is taken for the reason just given - the LE Meta
// dispatch holds it across every callback it makes, so a registration either completes
// before a dispatch starts or waits it out, and no dispatch ever reads a half-copied
// struct. The refusal without an event layer is the same shape kept in step rather than a
// hazard of its own: with no dispatch to race, the copy would be safe, but a sink
// installed before HciInitEvent has run is a caller error worth naming - and CsInitialize
// runs earlier still, on BTM_Initialize's module loop. Like the result sink it is not
// cleared by CsInitialize, so a consumer that goes away unregisters.
void CsSessionRegisterSink(const CsSessionSink *sink)
{
    Mutex *lock = HciGetEventCallbackListLock();
    if (lock == NULL) {
        LOG_WARN("%{public}s: HCI event layer not up, sink not registered", __FUNCTION__);
        return;
    }
    MutexLock(lock);
    if (sink == NULL) {
        (void)memset_s(&g_sessionSink, sizeof(g_sessionSink), 0, sizeof(g_sessionSink));
    } else {
        g_sessionSink = *sink;
    }
    MutexUnlock(lock);
}

// Per-connection state dies with the connection. On a completed Disconnection
// Complete the Controller forgets that handle's learned remote caps and FAE table (the
// 7.8.132 and 7.8.136 disallowed conditions are both connection-scoped), so a
// reconnect reusing the numeric handle must be allowed to write the cached payloads
// back and to re-issue 0x008C; clear both remote-cache markers and drop the whole
// connection row here instead of leaving any of it stale. Only HCI_SUCCESS clears, the
// same completed-termination reading as BtmRemoveRemoteDeviceSupportRequestsByHandle
// and IsoLeDisconnectComplete take - but their dispatch is not this one: both run on
// their own module's task after a deep copy (IsoProcessHciEventInTask) and report
// upward from there (iso_cig.c), while this runs in place on the Stack thread, inside
// the HCI event dispatch that holds HciGetEventCallbackListLock (hci_evt.h). Only the
// markers go, not the payloads: remembering a peer across a reconnect is what the
// cache is for. sessionClosed follows the session the Host opened and not the frame: a
// Disconnection Complete for a handle with no CS session is not the end of one, and
// announcing it would report a session that never existed. It carries no reason, because
// the ACL layer reports that itself and what the engine did here is the same on every
// completed disconnect.
static void CsOnDisconnectComplete(const HciDisconnectCompleteEventParam *eventParam)
{
    if (eventParam == NULL || eventParam->status != HCI_SUCCESS) {
        return;
    }
    for (int i = 0; i < CS_REMOTE_CACHE_SIZE; i++) {
        if (!g_remoteCache[i].inUse) {
            continue;
        }
        if (g_remoteCache[i].learnedOnHandle == eventParam->connectionHandle) {
            g_remoteCache[i].learnedOnHandle = CS_HANDLE_NONE;
        }
        if (g_remoteCache[i].faeLearnedOnHandle == eventParam->connectionHandle) {
            g_remoteCache[i].faeLearnedOnHandle = CS_HANDLE_NONE;
        }
    }
    CsConnection *connection = CsFindConnection(eventParam->connectionHandle);
    if (connection == NULL) {
        return;
    }
    // Read before the reset: the zero fill is what clears it, and the sink hears about a
    // session that had been opened rather than about a row that happened to exist.
    bool sessionOpened = connection->sessionOpened;
    CsResetConnection(connection);
    if (sessionOpened && g_sessionSink.sessionClosed != NULL) {
        g_sessionSink.sessionClosed(eventParam->connectionHandle);
    }
}

static HciEventCallbacks g_csHciEventCallbacks = {
    .leCsReadLocalSupportedCapabilitiesComplete = CsOnReadLocalSupportedCapabilitiesComplete,
    .leCsReadRemoteSupportedCapabilitiesComplete = CsOnReadRemoteSupportedCapabilitiesComplete,
    .leCsWriteCachedRemoteSupportedCapabilitiesComplete = CsOnWriteCachedRemoteSupportedCapabilitiesComplete,
    .leCsReadRemoteFaeTableComplete = CsOnReadRemoteFaeTableComplete,
    .leCsWriteCachedRemoteFaeTableComplete = CsOnWriteCachedRemoteFaeTableComplete,
    .leCsSecurityEnableComplete = CsOnSecurityEnableComplete,
    .leCsConfigComplete = CsOnConfigComplete,
    .leCsSetDefaultSettingsComplete = CsOnSetDefaultSettingsComplete,
    .leCsSetChannelClassificationComplete = CsOnSetChannelClassificationComplete,
    .leCsSetProcedureParametersComplete = CsOnSetProcedureParametersComplete,
    .leCsProcedureEnableComplete = CsOnProcedureEnableComplete,
    .leCsSubeventResult = CsOnSubeventResult,
    .leCsSubeventResultContinue = CsOnSubeventResultContinue,
    .leCsTestComplete = CsTestOnStartComplete,
    .leCsTestEndComplete = CsTestOnTestEndComplete,
    .disconnectComplete = CsOnDisconnectComplete,
};

// --- CS session (C-9) ---------------------------------------------------------

// The one place 0x0089 goes on the wire, called from CsOpenSession alone - which is how
// the "one read per stack lifetime" rule is held: not by a flag every future caller has
// to remember, but by there being no other path to this function.
// A send failure is logged and dropped. The read is the engine's own bookkeeping, not
// something a caller asked for, so it does not fail the session; the cache simply stays
// empty and the next CsOpenSession retries. A Controller that answers with a nonzero
// Status is not logged here either - CsOnReadLocalSupportedCapabilitiesComplete returns
// on it - so a missing warning does not mean the read succeeded.
static void CsRequestLocalCapabilities(void)
{
    int result = HCI_LeCsReadLocalSupportedCapabilities();
    if (result != BT_SUCCESS) {
        LOG_WARN("%{public}s: HCI_LeCsReadLocalSupportedCapabilities failed: %{public}d",
            __FUNCTION__, result);
    }
}

// Open a CS session on an existing ACL: claim the handle's CsConnection row and, when no
// local capability body is cached yet, read it. That read is the only 0x0089 this stack
// sends (7.8.130 p2798) and it runs here rather than at startup because the body is
// needed exactly when CS work on a connection begins.
// The ACL is resolved before the row is claimed, so a handle with no live ACL cannot
// take a table slot on its way to being refused. A second open of the same handle is
// idempotent: the row is reused as it stands and, with the body cached, nothing more
// goes to the wire.
// No latch guards the read against a second one in flight, and none is needed: HCI pairs
// a Command_Complete with a pending command by opcode in send order, and refuses to
// absorb a timeout record while a same-opcode command is pending (hci_cmd.c), so two
// 0x0089 outstanding are each answered on their own. Both answer with the same
// Controller constant, so a repeat costs one command and changes nothing.
// Returns the ACL layer's result for a handle with no live ACL, BT_NO_MEMORY when the
// connection table is full, BT_SUCCESS once the row is held and claimed as the session.
int CsOpenSession(uint16_t connectionHandle)
{
    BtAddr addr;
    int result = BtmGetAclAddressByHandle(connectionHandle, &addr);
    if (result != BT_SUCCESS) {
        return result;
    }
    CsConnection *connection = CsFindOrCreateConnection(connectionHandle);
    if (connection == NULL) {
        return BT_NO_MEMORY;
    }
    connection->sessionOpened = true;
    if (!g_localCapabilitiesValid) {
        CsRequestLocalCapabilities();
    }
    return BT_SUCCESS;
}

// Release the CS session held for connectionHandle: only the Host-side session latch and
// the subevent buffer go. The configuration mirrors and procedure state stay, because they
// are the Controller's memory of CS work on this ACL and persist until Remove Config or
// disconnect. BT_NOT_SUPPORT when no session is held. A row the Host never opened a session
// on counts as no session: a 0x2C/0x2F/0x30/0x0093 can leave one behind on the handle, and
// that row is not a session. Nothing goes to the Controller: no command cancels a CS session.
int CsCloseSession(uint16_t connectionHandle)
{
    CsConnection *connection = CsFindConnection(connectionHandle);
    if (connection == NULL || !connection->sessionOpened) {
        return BT_NOT_SUPPORT;
    }
    CsResultReleaseStream(&connection->resultStream);
    connection->sessionOpened = false;
    return BT_SUCCESS;
}

int CsGetLocalCapabilities(HciLeCsCapabilities *capabilities)
{
    if (capabilities == NULL) {
        return BT_BAD_PARAM;  // NULL out-param is a caller error, not "CS unavailable"
    }
    if (!g_localCapabilitiesValid) {
        return BT_NOT_SUPPORT;  // no successful 0x0089 cached yet (bit-47/C-3 prerequisite)
    }
    (void)memcpy_s(capabilities, sizeof(*capabilities), &g_localCapabilities, sizeof(g_localCapabilities));
    return BT_SUCCESS;
}

int CsGetRemoteCapabilities(const BtAddr *addr, HciLeCsCapabilities *capabilities)
{
    if (addr == NULL || capabilities == NULL) {
        return BT_BAD_PARAM;
    }
    CsRemoteCacheEntry *entry = CsFindRemoteEntry(addr);
    if (entry == NULL || !entry->capValid) {
        return BT_NOT_SUPPORT;  // row may be held by an FAE read alone, which is no caps
    }
    (void)memcpy_s(capabilities, sizeof(*capabilities), &entry->capabilities, sizeof(entry->capabilities));
    return BT_SUCCESS;
}

// Start the CS Capability Exchange with the peer (0x008A, Vol 4 Part E 7.8.131 p2805).
// The command is answered in Command_Status and its outcome arrives later as the 0x2C
// subevent, which is what fills the remote cache; this call only puts it on the wire.
// Per p2805 the Controller reuses its own copy once an exchange has run or a 0x008B has
// been issued, so a repeat is cheap rather than a second air exchange.
// The ACL is resolved first for the same reason as CsSecurityEnable: a handle with no
// live ACL is refused locally instead of drawing the Controller's 0x02. Nothing is
// recorded on the way out, and the Controller's own refusals are passed through - 0x0C
// when the Channel Sounding (Host Support) feature bit is not set on either side, which
// is the same bit-47 condition CsSecurityEnable documents. Returns the HCI send result,
// or the ACL layer's result for a handle with no live ACL.
int CsReadRemoteCapabilities(uint16_t connectionHandle)
{
    BtAddr addr;
    int result = BtmGetAclAddressByHandle(connectionHandle, &addr);
    if (result != BT_SUCCESS) {
        return result;
    }
    HciLeCsReadRemoteSupportedCapabilitiesParam param = { .connectionHandle = connectionHandle };
    return HCI_LeCsReadRemoteSupportedCapabilities(&param);
}

int CsWriteBackRemoteCapabilities(uint16_t connectionHandle)
{
    BtAddr addr;
    int result = BtmGetAclAddressByHandle(connectionHandle, &addr);
    if (result != BT_SUCCESS) {
        return result;
    }
    CsRemoteCacheEntry *entry = CsFindRemoteEntry(&addr);
    if (entry == NULL || !entry->capValid) {
        return BT_NOT_SUPPORT;
    }
    if (entry->learnedOnHandle == connectionHandle) {
        return BT_ALREADY;
    }
    // The 7.8.132 disallow condition ("a CS configuration has been created in the
    // local Controller") is left to the Controller: the Host does not mirror that
    // state, and the Controller is the authoritative source of whether the
    // condition still holds after a remove.
    HciLeCsWriteCachedRemoteSupportedCapabilitiesParam param = {
        .connectionHandle = connectionHandle,
        .capabilities = entry->capabilities,
    };
    return HCI_LeCsWriteCachedRemoteSupportedCapabilities(&param);
}

int CsGetRemoteFaeTable(const BtAddr *addr, int8_t *faeTable)
{
    if (addr == NULL || faeTable == NULL) {
        return BT_BAD_PARAM;
    }
    CsRemoteCacheEntry *entry = CsFindRemoteEntry(addr);
    if (entry == NULL || !entry->faeValid) {
        return BT_NOT_SUPPORT;
    }
    (void)memcpy_s(faeTable, LE_CS_REMOTE_FAE_TABLE_LEN, entry->faeTable, sizeof(entry->faeTable));
    return BT_SUCCESS;
}

// Run the CS Mode-0 FAE Table Request procedure against the peer (0x008E, Vol 4 Part E
// 7.8.135 p2819). Like the 0x008A above the command is answered in Command_Status and
// the table arrives as the 0x2D subevent, which is what caches it; this call only puts
// it on the wire, and the ACL is resolved first for the same reason.
// A peer whose mode-0 transmissions carry no Frequency Actuation Error is answered by
// the Controller with a 0x2D whose Status is 0x11 rather than a table (p2819, the No_FAE
// capability) - not a command rejection, and not pre-empted here: a cached capability
// body naming No_FAE does not make the read illegitimate, so the Controller owns the
// answer. The only command-level refusal p2819 gives is 0x02 for a handle that does not
// exist or is not an ACL. Returns the HCI send result, or the ACL layer's result for a
// handle with no live ACL.
int CsReadRemoteFae(uint16_t connectionHandle)
{
    BtAddr addr;
    int result = BtmGetAclAddressByHandle(connectionHandle, &addr);
    if (result != BT_SUCCESS) {
        return result;
    }
    HciLeCsReadRemoteFaeTableParam param = { .connectionHandle = connectionHandle };
    return HCI_LeCsReadRemoteFaeTable(&param);
}

int CsWriteBackRemoteFaeTable(uint16_t connectionHandle)
{
    BtAddr addr;
    int result = BtmGetAclAddressByHandle(connectionHandle, &addr);
    if (result != BT_SUCCESS) {
        return result;
    }
    CsRemoteCacheEntry *entry = CsFindRemoteEntry(&addr);
    if (entry == NULL || !entry->faeValid) {
        return BT_NOT_SUPPORT;
    }
    if (entry->faeLearnedOnHandle == connectionHandle) {
        return BT_ALREADY;
    }
    // The 7.8.136 disallow condition ("a CS configuration has been created in the
    // local Controller") is left to the Controller, mirroring the 0x008B path above.
    HciLeCsWriteCachedRemoteFaeTableParam param = {
        .connectionHandle = connectionHandle,
    };
    (void)memcpy_s(param.remoteFaeTable, sizeof(param.remoteFaeTable),
        entry->faeTable, sizeof(entry->faeTable));
    return HCI_LeCsWriteCachedRemoteFaeTable(&param);
}

int CsSecurityEnable(uint16_t connectionHandle)
{
    BtAddr addr;
    int result = BtmGetAclAddressByHandle(connectionHandle, &addr);
    if (result != BT_SUCCESS) {
        return result;
    }
    HciLeCsSecurityEnableParam param = { .connectionHandle = connectionHandle };
    return HCI_LeCsSecurityEnable(&param);
}

// The three configuration-side senders below share one shape: resolve the handle
// against the ACL table (a handle with no live ACL is rejected locally with
// BT_BAD_STATUS, so the 0x02 the Controller would answer is never reached through
// these APIs), then hand the parameter through. Nothing is recorded on the way out
// - the slot table is written only by the 0x2F that reports what the Controller
// actually did, which is also why a create that the Controller rejects leaves the
// previous mirror of that Config_ID intact.
int CsSetDefaultSettings(const HciLeCsSetDefaultSettingsParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    BtAddr addr;
    int result = BtmGetAclAddressByHandle(param->connectionHandle, &addr);
    if (result != BT_SUCCESS) {
        return result;
    }
    return HCI_LeCsSetDefaultSettings(param);
}

int CsCreateConfig(const HciLeCsCreateConfigParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    BtAddr addr;
    int result = BtmGetAclAddressByHandle(param->connectionHandle, &addr);
    if (result != BT_SUCCESS) {
        return result;
    }
    return HCI_LeCsCreateConfig(param);
}

int CsRemoveConfig(const HciLeCsRemoveConfigParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    BtAddr addr;
    int result = BtmGetAclAddressByHandle(param->connectionHandle, &addr);
    if (result != BT_SUCCESS) {
        return result;
    }
    return HCI_LeCsRemoveConfig(param);
}

int CsGetConfig(uint16_t connectionHandle, uint8_t configId, HciLeCsConfigCompleteEventParam *config)
{
    if (config == NULL || configId > LE_CS_CONFIG_ID_MAX) {
        return BT_BAD_PARAM;
    }
    BtAddr addr;
    int result = BtmGetAclAddressByHandle(connectionHandle, &addr);
    if (result != BT_SUCCESS) {
        return result;
    }
    CsConnection *connection = CsFindConnection(connectionHandle);
    if (connection == NULL || !connection->slots[configId].inUse) {
        return BT_NOT_SUPPORT;
    }
    *config = connection->slots[configId].lastConfig;
    return BT_SUCCESS;
}

// --- CS procedure scheduling (C-5) ----------------------------------------------

int CsSetChannelClassification(const HciLeCsSetChannelClassificationParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    // No ACL lookup: 0x0092 carries no Connection_Handle and its classification is
    // Controller-wide, so there is no link state to gate it on (7.8.139 p2830).
    return HCI_LeCsSetChannelClassification(param);
}

// The record below is what makes the 0x0093 Command_Complete attributable: the return
// names the handle but not the Config_ID, so the engine has to remember which slot it
// asked about (7.8.140 p2836/p2837). Sending is refused while a previous 0x0093 is still
// outstanding on the same handle rather than overwriting the record, because the two
// Command_Completes would arrive identical and the first one would be credited to the
// second's Config_ID. Nothing is recorded unless the command actually went out.
int CsSetProcedureParameters(const HciLeCsSetProcedureParametersParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    BtAddr addr;
    int result = BtmGetAclAddressByHandle(param->connectionHandle, &addr);
    if (result != BT_SUCCESS) {
        return result;
    }
    CsConnection *connection = CsFindOrCreateConnection(param->connectionHandle);
    if (connection == NULL) {
        return BT_NO_MEMORY;  // no row to record the in-flight Config_ID in
    }
    if (connection->procParamsPending) {
        return BT_ALREADY;
    }
    // The record goes in before the command does: the completion is dispatched on the
    // HCI event thread, so a Command_Complete that arrived before the record existed
    // would be dropped as unattributable and the slot would look unset forever. A send
    // that never reached the wire produces no completion, so the record is taken back.
    connection->procParamsPending = true;
    connection->procParamsPendingId = param->configId;
    result = HCI_LeCsSetProcedureParameters(param);
    if (result != BT_SUCCESS) {
        connection->procParamsPending = false;
        return result;
    }
    return BT_SUCCESS;
}

// Report a 0x0094 the engine refused before the wire through the same row the
// Controller's own rejection and a command timeout go through, so the consumer sees
// one shape whichever side answered: the 0x30 is synthesized with Connection_Handle,
// Config_ID and State forced from the command that was asked for. Reusing the row
// instead of hand-building the event keeps this refusal identical to the Controller's,
// including anything the row fills in later. The caller gets BT_SUCCESS the way it
// does for a command that was sent - the outcome is the callback, not the return.
static int CsRefuseProcedureEnable(const HciLeCsProcedureEnableParam *param, uint8_t status)
{
    HciOnCmdFailed(HCI_LE_CS_PROCEDURE_ENABLE, status, param);
    return BT_SUCCESS;
}

// The enable preconditions of 7.8.141 p2838 are the one place the engine refuses a
// command before the wire instead of forwarding it, because both conditions are
// facts about the Host's own CS session that the engine already holds:
//   - no slot for the Config_ID   -> the Controller would answer 0x12
//   - no accepted 0x0093 on it    -> the Controller would answer 0x0C
// Every other guard on this command (0x0D scheduling and coexistence, 0x48 too few
// channels, 0x0C already enabled) is Controller-side state the engine does not
// mirror, so those are left to the Controller and passed through untouched. The
// disable path is not guarded at all: disabling what the Controller does not hold is
// likewise the Controller's 0x12, not the engine's.
int CsProcedureEnable(const HciLeCsProcedureEnableParam *param)
{
    // configId indexes the slot table below, so it is range-checked here even though
    // the HCI sender repeats the check for the wire.
    if (param == NULL || param->configId > LE_CS_CONFIG_ID_MAX) {
        return BT_BAD_PARAM;
    }
    BtAddr addr;
    int result = BtmGetAclAddressByHandle(param->connectionHandle, &addr);
    if (result != BT_SUCCESS) {
        return result;
    }
    if (param->enable == LE_CS_PROCEDURE_ENABLED) {
        CsConnection *connection = CsFindConnection(param->connectionHandle);
        if (connection == NULL || !connection->slots[param->configId].inUse) {
            return CsRefuseProcedureEnable(param, HCI_INVALID_HCI_COMMAND_PARAMETERS);
        }
        if (!connection->slots[param->configId].procParamsSet) {
            return CsRefuseProcedureEnable(param, HCI_COMMAND_DISALLOWED);
        }
    }
    return HCI_LeCsProcedureEnable(param);
}

int CsGetProcedureState(uint16_t connectionHandle, uint8_t configId, CsProcedureState *state)
{
    if (state == NULL || configId > LE_CS_CONFIG_ID_MAX) {
        return BT_BAD_PARAM;
    }
    BtAddr addr;
    int result = BtmGetAclAddressByHandle(connectionHandle, &addr);
    if (result != BT_SUCCESS) {
        return result;
    }
    CsConnection *connection = CsFindConnection(connectionHandle);
    if (connection == NULL || !connection->slots[configId].inUse) {
        return BT_NOT_SUPPORT;
    }
    const CsConfigSlot *slot = &connection->slots[configId];
    state->procParamsSet = slot->procParamsSet;
    state->enabled = slot->enabled;
    state->schedule = slot->procEnable;
    return BT_SUCCESS;
}

static void CsInitialize(int traceLevel)
{
    (void)traceLevel;
    // The queue the CSIF_ entries are dispatched on (cs_task_common.c). BT_BAD_STATUS means
    // one with this id already exists, i.e. a previous init never got its CsFinalize; that
    // is the queue we need either way, so unlike IsoInitialize this does not return - every
    // reset below releases what it owns (CsResetConnection frees the row's report through
    // CsResultReleaseStream, CsTestReset the test's), so running them is what clears the
    // previous session rather than what rebuilds over it.
    if (BTM_CreateProcessingQueue(PROCESSING_QUEUE_ID_CS, BTM_PROCESSING_QUEUE_SIZE_DEFAULT) != BT_SUCCESS) {
        LOG_WARN("%{public}s: CreateProcessingQueue failed, the CS queue already exists", __FUNCTION__);
    }
    (void)memset_s(g_remoteCache, sizeof(g_remoteCache), 0, sizeof(g_remoteCache));
    g_localCapabilitiesValid = false;
    for (int i = 0; i < CS_CONNECTION_TABLE_SIZE; i++) {
        CsResetConnection(&g_connections[i]);
    }
    CsTestReset();
}

static void CsEnable()
{
    LOG_DEBUG("%{public}s:", __FUNCTION__);
    HCI_RegisterEventCallbacks(&g_csHciEventCallbacks);
}

// Deregistering the callbacks drops every completion this module could still act on,
// so a 0x0093 left in flight by the shutdown would keep CsSetProcedureParameters
// refusing every later request on that handle with BT_ALREADY, and a startup after a
// shutdown does not re-run CsInitialize to clear it. ISO's shutdown clears the same
// kind of record for the same reason (a Remove CIG whose Complete never arrived would
// block every later ISOIF_LeRemoveCig). Only the latch is released: the mirror fields
// and the slot state follow the row lifecycle that C-9's CsSession fold owns (see the
// CsConnection comment above), and clearing them here would settle that ownership early.
static void CsDisable()
{
    LOG_DEBUG("%{public}s:", __FUNCTION__);
    HCI_DeregisterEventCallbacks(&g_csHciEventCallbacks);
    // No completion can arrive now, so this is the last point that can release the one
    // record no event will ever close. Same clear as the unattributable case above.
    for (int i = 0; i < CS_CONNECTION_TABLE_SIZE; i++) {
        g_connections[i].procParamsPending = false;
        // Subevent reports are heap, not row state: the next CsInitialize would free
        // them through CsResetConnection anyway, but a shutdown that leaves a buffer
        // allocated for the rest of the process lifetime is a leak, not a lifecycle.
        CsResultReleaseStream(&g_connections[i].resultStream);
    }
    // The same two reasons as the loop above, for the record that is not a row's: its
    // report would leak, and a 0x0095 or a 0x0096 left outstanding would refuse every
    // later CsTestStart and CsTestEnd with BT_ALREADY - neither the missing
    // Command_Complete nor the 0x33 can arrive after the deregistration.
    CsTestReset();
}

// The CS queue created by CsInitialize goes here, and this is the right place rather than
// CsDisable: BTM_DeleteProcessingQueue runs the tasks still queued while it drains them
// (the same call IsoFinalizeTask makes for the same reason), and those tasks must not run
// before CsDisable has deregistered the HCI callbacks. A drained CSIF_ task can still send
// an HCI command whose completion now has no CS handler - that command's result is simply
// never consumed. Nothing worse is reachable: CS keeps no alarms, and every CSIF_ task
// touches only module state that is static and never freed.
static void CsFinalize()
{
    BTM_DeleteProcessingQueue(PROCESSING_QUEUE_ID_CS);
}

Module g_cs = {
    .name = MODULE_NAME_CS,
    .init = CsInitialize,
    .startup = CsEnable,
    .shutdown = CsDisable,
    .cleanup = CsFinalize,
    .dependencies = { MODULE_NAME_HCI },
};

MODULE_DECL(g_cs)
