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

#ifndef BTSTACK_CS_H
#define BTSTACK_CS_H

#include <stdbool.h>
#include <stdint.h>

#include "btstack.h"
#include "hci/hci_def_le_cmd.h"
#include "hci/hci_def_le_evt.h"

#ifdef __cplusplus
extern "C" {
#endif

// Module-level Channel Sounding capability caches (C-1/C-2/C-7). The local caps come
// from the 0x0089 Command_Complete and are cached module-wide; the remote cache is
// keyed by peer address and survives disconnect so the Host can write what it holds
// back to the Controller after a reconnection - the capability body with 0x008B
// (Vol 4 Part E 7.8.132 pp2807-2808) and the 72-B mode-0 FAE table with 0x008F
// (7.8.135 p2819 / 7.8.136 pp2820-2821).

// --- CS session (C-9) ---------------------------------------------------------

// Open a CS session on an existing ACL: claim the handle for CS work and read the local
// capabilities if none are cached yet. That read is the only 0x0089 this stack sends
// (7.8.130 p2798); its send failure does not fail the open, and a later open retries it,
// so the session is usable either way. BT_SUCCESS once the handle is claimed, the ACL
// layer's result for a handle with no live ACL, BT_NO_MEMORY when the connection table
// is full. A second open of the same handle is idempotent.
int CsOpenSession(uint16_t connectionHandle);

// Release the CS session held for connectionHandle. The Host-side record goes; the
// Controller's view does not change, because no command cancels a CS session, and the
// peer's cached capabilities and FAE table stay valid for the handle. BT_SUCCESS on a
// session released, BT_NOT_SUPPORT when none was held - including a handle CS holds a
// row for but that CsOpenSession never claimed, which an unsolicited 0x2C/0x2F/0x30/0x0093
// can leave behind: the row is not the session.
int CsCloseSession(uint16_t connectionHandle);

// Send 0x008A (7.8.131 p2805) to start the CS Capability Exchange with the peer. Its
// outcome is the 0x2C subevent, which caches the remote capability body; this call only
// sends. Returns the HCI send result, or the ACL layer's result for a handle with no
// live ACL.
int CsReadRemoteCapabilities(uint16_t connectionHandle);

// Send 0x008E (7.8.135 p2819) to run the CS Mode-0 FAE Table Request procedure. Its
// outcome is the 0x2D subevent - Status 0x11 instead of a table for a No_FAE peer - which
// caches the table; this call only sends. Returns the HCI send result, or the ACL layer's
// result for a handle with no live ACL.
int CsReadRemoteFae(uint16_t connectionHandle);

// Fill capabilities with the local CS capabilities cached from the last 0x0089
// result. BT_SUCCESS + 28 B copy; BT_BAD_PARAM when capabilities is NULL (a
// caller error, checked before the readiness gate); BT_NOT_SUPPORT when no
// successful 0x0089 has been cached (0x0089 itself is only usable after the
// bit-47 Host-Support declaration of the C-3 unit has run, 7.8.130 p2798).
int CsGetLocalCapabilities(HciLeCsCapabilities *capabilities);

// Fill capabilities with the most recent 0x2C result cached for addr. BT_SUCCESS
// on a hit, BT_BAD_PARAM when addr or capabilities is NULL (a caller error, checked
// before the cache lookup), BT_NOT_SUPPORT when the peer has no usable cached entry.
int CsGetRemoteCapabilities(const BtAddr *addr, HciLeCsCapabilities *capabilities);

// Write the cached remote capabilities back to the Controller for an active
// connection (0x008B). BT_ALREADY when this connection already exchanged
// LL_CS_CAPABILITIES_* (a 0x2C arrived on this handle) or wrote back: that is the
// LL_CS_CAPABILITIES_* condition of 7.8.132 (p2808) plus the Host's own "already
// written back" bookkeeping. Whether a CS configuration has been created is left to
// the Controller (0x0C on the Command_Complete) and is reported through the session
// sink. BT_NOT_SUPPORT when no capability body is cached for the peer behind the
// handle - a row held by an FAE read alone reads this way, since that carries no
// capabilities. Returns the HCI send result, or the ACL layer's result for a handle
// with no live ACL.
int CsWriteBackRemoteCapabilities(uint16_t connectionHandle);

// Fill faeTable with the 72-B mode-0 FAE table cached for addr from a 0x2D with
// Status 0x00. BT_SUCCESS + 72 B copy, BT_BAD_PARAM when faeTable is NULL,
// BT_NOT_SUPPORT when no table has been cached for that peer - which is also how a
// No_FAE peer reads, since a 0x11 answer caches nothing (7.8.135 p2819). The octets
// are signed 1/32 ppm values (-128 = -4 ppm, 127 = +3.96875 ppm) per [Vol 6] Part B
// 2.4.2,52 p3008, so the caller reads them as int8_t and not as raw bytes.
int CsGetRemoteFaeTable(const BtAddr *addr, int8_t *faeTable);

// Write the cached remote FAE table back to the Controller for an active connection
// (0x008F, Vol 4 Part E 7.8.136 pp2820-2821). BT_ALREADY when this connection already
// wrote that table back (recorded from a successful 0x008F): the LL_CS_FAE_RSP
// condition of p2820 plus the Host's own bookkeeping. Whether a CS configuration has
// been created is left to the Controller and reported through the session sink.
// BT_NOT_SUPPORT when no table is cached for the peer behind the handle. The
// Controller's own 0x11 for a peer that reports a zero FAE is its answer and travels
// back untouched - this call does not consult the cached capability body ahead of it.
// Returns the HCI send result, or the ACL layer's result for a handle with no live ACL.
int CsWriteBackRemoteFaeTable(uint16_t connectionHandle);

// Start or restart the CS Security Start procedure for an active connection
// (0x008C, Vol 4 Part E 7.8.133 p2815). The caller must be the Central on an
// encrypted link; this function does not check role or encryption. A handle with
// no active ACL is rejected locally with BT_BAD_STATUS (nothing is sent, so the
// 0x02 controller error is never reached through this API). Command-level
// rejections (0x2F link not encrypted, 0x0C peripheral role or CS Host Support
// [bit 47] not declared) come back in Command_Status and are funneled into the
// security-enable-complete callback as a nonzero status; a started exchange that
// fails likewise reports a nonzero 0x2E status (7.7.65,41 p2434). Either way the
// procedure can be re-issued once the link state is fixed. The engine keeps no
// Host-side latch of Security Start completion: LL §5.1.26 enforces that gate in
// the Controller. Returns the HCI send result, or BT_BAD_STATUS for
// a handle with no active ACL.
int CsSecurityEnable(uint16_t connectionHandle);

// --- CS configuration (C-4) ---------------------------------------------------

// Send 0x008D (7.8.134 p2816) for an active connection: the per-connection default
// Role_Enable / CS_SYNC antenna selection / Max_TX_Power a later 0x0090 is judged
// against. The engine keeps no local mirror of these: the Controller owns the
// defaults, they do not outlive the connection, and every relationship guard on
// 0x0090 (a role that was never enabled -> 0x12, a value the Controller cannot meet
// -> 0x11) is answered by the Controller. Only self-contained field checks are done
// before the wire, in the HCI sender. The defaults' own result is the 0x008D
// Command_Complete, reported through the CS session sink's setDefaultSettingsComplete:
// this call returns as soon as the command is on the wire, so 0x11 and 0x12 arrive
// there and not here. Returns the HCI send result, or BT_BAD_STATUS for a handle with
// no active ACL (BT_BAD_PARAM when param is NULL).
int CsSetDefaultSettings(const HciLeCsSetDefaultSettingsParam *param);

// Send 0x0090 (7.8.137 p2822) for an active connection; success is reported by the
// 0x2F LE CS Config Complete subevent with Action 0x01, not by this call, which
// returns as soon as the command is on the wire. No slot state is claimed here - the
// engine cannot know whether the Controller accepted the configuration, so it
// records only what 0x2F reports. The Controller decides every relationship guard
// (0x11 unsupported parameter value, 0x12 invalid configuration or a Role not
// enabled by 0x008D, 0x0C Config_ID already enabled) and the engine passes the
// error through untouched. Returns the HCI send result, or BT_BAD_STATUS for a
// handle with no active ACL. BT_BAD_PARAM comes from the HCI sender's shape checks
// (param NULL, Connection_Handle out of range, Config_ID outside 0x00-0x03,
// Reserved nonzero, and the Channel_Map shape of 7.8.137 p2826) before anything
// reaches the wire.
int CsCreateConfig(const HciLeCsCreateConfigParam *param);

// Send 0x0091 (7.8.138 p2828) for an active connection; success is reported by the
// 0x2F subevent with Action 0x00, which drops that slot's state including the
// procedure parameters a later 0x0093 would have set. As with CsCreateConfig the
// Controller decides whether the Config_ID exists (0x12) or a procedure is still
// enabled on it (0x0C). Returns the HCI send result, or BT_BAD_STATUS for a handle
// with no active ACL. BT_BAD_PARAM comes from the HCI sender's shape checks (param
// NULL, Connection_Handle out of range, Config_ID outside 0x00-0x03).
int CsRemoveConfig(const HciLeCsRemoveConfigParam *param);

// Fill config with the mirror of the last successful 0x2F Action 0x01 for
// (connectionHandle, configId): the CS configuration the local Controller holds,
// with the Controller-chosen fields (Channel_Map, the four T_*_Time values) as
// reported. BT_SUCCESS on a hit, BT_NOT_SUPPORT when no configuration is held for
// that Config_ID, BT_BAD_STATUS for a handle with no active ACL, BT_BAD_PARAM when
// config is NULL or configId is outside 0x00-0x03. A removed configuration has no
// mirror: on the Action 0x00 path every octet after Action is ignored (7.7.65,42
// p2435), so the call reports BT_NOT_SUPPORT rather than the stale create.
int CsGetConfig(uint16_t connectionHandle, uint8_t configId, HciLeCsConfigCompleteEventParam *config);

// --- CS procedure scheduling (C-5) --------------------------------------------

// Send 0x0092 (7.8.139 pp2830-2831) to replace the Controller-wide CS channel
// classification with map. The classification has no Connection_Handle and is not
// connection-scoped: it outlives any link (it is dropped by a Controller reset or a
// later 0x0092) and the Controller may combine it with its own information, so no
// ACL lookup gates this call. The HCI sender checks the map's shape - the reserved
// channels zero and at least 15 channels enabled - and rejects a bad map with
// BT_BAD_PARAM before the wire; that is the same rule behind the Controller's 0x12,
// so a malformed map never reaches it. Everything else is pass-through: a repeat
// within 1 second of the previous issue is the Controller's 0x0C (p2830) and the
// engine keeps no rate-limit state to drift from the Controller's view. That 0x0C, and
// the 0x12 for a map the Controller refuses, are the 0x0092 Command_Complete reported
// through the CS session sink's setChannelClassificationComplete - this call returns as
// soon as the command is on the wire. Returns the HCI send result, or BT_BAD_PARAM for
// a NULL or malformed map.
int CsSetChannelClassification(const HciLeCsSetChannelClassificationParam *param);

// Send 0x0093 (7.8.140 pp2832-2837) for an active connection: the scheduling
// parameters a later 0x0094 enables on the Config_ID. Success is reported by the
// Command_Complete, whose return names only the Connection_Handle - Config_ID is not
// echoed - so the engine records which Config_ID went out and credits the result to
// that slot; without the record the result cannot be attributed. Whether the
// Config_ID exists (0x12), already has a procedure enabled (0x0C) or conflicts with
// the Controller's scheduling (0x0D) is the Controller's answer and is passed
// through untouched. Returns the HCI send result; BT_ALREADY when a previous 0x0093
// is still outstanding on the same handle (issuing a second would make the two
// identical Command_Completes impossible to tell apart), BT_BAD_STATUS for a handle
// with no active ACL, BT_NO_MEMORY when the engine cannot record the request,
// BT_BAD_PARAM when param is NULL.
int CsSetProcedureParameters(const HciLeCsSetProcedureParametersParam *param);

// Send 0x0094 (7.8.141 pp2838-2839) for an active connection to start or stop the
// CS procedure on a Config_ID; the outcome is the 0x30 subevent, not this call. The
// enable preconditions the Controller would answer with 0x0C/0x12 are the one place
// this API refuses before the wire, because they are facts about the Host's own CS
// session: enabling a Config_ID the engine holds no slot for reports 0x12 and one
// whose 0x0093 has not been accepted reports 0x0C. A refusal is reported through the
// CS procedure-enable-complete callback with the same shape a Controller rejection
// has, so the caller still gets BT_SUCCESS and learns the outcome there; the API is
// not able to say "not sent" and the callback is the single outcome path. Every other
// guard (0x0D, 0x48, 0x0C already enabled) is Controller-side and passes through, and
// the disable path is not guarded at all. Returns the HCI send result, BT_BAD_STATUS
// for a handle with no active ACL, or BT_BAD_PARAM for a NULL parameter or a
// Config_ID outside 0x00-0x03.
int CsProcedureEnable(const HciLeCsProcedureEnableParam *param);

// Retained CS procedure state of one configuration slot: what 0x0093 has been
// accepted for it and what 0x30 last reported. schedule is the 0x30 body, so it is
// meaningful only while enabled is set - State 0x00 leaves every field after it
// unspecified (7.7.65,43 p2442).
typedef struct {
    bool procParamsSet;                            // a 0x0093 with Status 0 succeeded
    bool enabled;                                  // the last 0x30 reported State 0x01
    HciLeCsProcedureEnableCompleteEventParam schedule;  // valid when enabled
} CsProcedureState;

// Fill state with the CS procedure state of (connectionHandle, configId). This is
// the reader for what CsProcedureEnable reads back as its preconditions, so a caller
// can tell "no procedure parameters set" from "not enabled". BT_SUCCESS on a hit,
// BT_NOT_SUPPORT when the engine holds no configuration for that Config_ID,
// BT_BAD_STATUS for a handle with no active ACL, BT_BAD_PARAM when state is NULL or
// configId is outside 0x00-0x03.
int CsGetProcedureState(uint16_t connectionHandle, uint8_t configId, CsProcedureState *state);

// --- CS test (C-8) ------------------------------------------------------------

// Send 0x0095 (7.8.142 pp2840-2854) to start a CS test against a tester: one CS
// procedure the Controller runs with no peer, whose subevents are reported under
// Connection_Handle 0x0FFF. The command has no Connection_Handle and is not
// connection-scoped, so no ACL lookup gates this call. Success is the Command_Complete
// with Status 0x00, whose return is Status alone - the parameters are not echoed - so
// the engine records the request's Role and RTT_Type when it sends it and makes them
// the running test's only when that status comes back; a refused start leaves a test
// already in progress untouched, and its Role and RTT_Type keep decoding that test's
// results. A test already enabled is not refused here: the Controller answers the 0x0C
// of p2843 on its own. Subevent results are delivered to the CS result sink as
// ordinary reports with connectionHandle 0x0FFF, and the two test outcomes to the CS
// test sink. Field checks (every parameter's value set, the 2-225 octet override tail)
// are the HCI sender's, ahead of the wire. Returns the HCI send result, BT_ALREADY
// when a previous 0x0095 is still outstanding (two Status-only Command_Completes could
// not be told apart) or when a 0x0096 is outstanding (see CsTestEnd below: the 0x33
// that closes it can land after this 0x0095's Command_Complete and would wipe the Role
// and RTT_Type it recorded), or BT_BAD_PARAM for a NULL parameter. A caller that was
// refused the second way retries after the CS test sink reports the end.
int CsTestStart(const HciLeCsTestParam *param);

// Send 0x0096 (7.8.143 p2855) to end the CS test in progress. The command
// carries no parameter and the engine keeps nothing from it: its outcome is the 0x33
// LE CS Test End Complete subevent, which the Controller sends only once every result
// of that test has been reported (p2855) and which closes the engine's test record.
// Calling it with no test running is left to the Controller, whose 0x0C (p2855) is
// funneled into the same test-end-complete outcome as a start or end that fails.
// Returns the HCI send result, or BT_ALREADY while a previous 0x0096 has neither been
// answered by a 0x33 nor failed locally - the engine refuses the second one because the
// 0x33 says nothing about which test it ended. The caller waits for the CS test sink's
// testEndComplete before starting the next test, and does not have to call this again:
// the refusal means one is already on the wire.
int CsTestEnd(void);

#ifdef __cplusplus
}
#endif

#endif
