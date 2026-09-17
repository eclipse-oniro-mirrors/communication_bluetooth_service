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

#ifndef BTSTACK_CS_INTERNAL_H
#define BTSTACK_CS_INTERNAL_H

#include <stdbool.h>
#include <stdint.h>

#include "cs_defs.h"
#include "hci/hci_def_le_cmd_6_0.h"
#include "hci/hci_def_le_evt_6_0.h"

#ifdef __cplusplus
extern "C" {
#endif

// Engine-internal seam between cs.c (HCI event routing, connection table) and
// cs_result.c (the 0x31/0x32 stream: Table 7.2 validation, step decode, reassembly).
// The normalized types a report is made of are in cs_defs.h; this header is not
// installed for consumers outside the CS engine.

// The consumer of a completed subevent report. The report is the engine's own
// storage, reused for the next subevent of the same connection, so a consumer that
// keeps anything must copy it out before returning. A report is delivered on the
// thread that carries the CS HCI event, synchronously inside the LE Meta dispatch, so
// a consumer that closes the session or re-registers the sink from inside this call is
// on that same thread and reaches the stream's release: cs_result.c holds the report
// back until the call returns rather than freeing it under the reader.
typedef void (*CsResultSink)(const CsSubeventReport *report);

// Register (sink non-NULL) or clear (NULL) the one sink. Clearing releases every
// stream, so a later sink starts from empty buffers rather than inheriting a
// half-absorbed subevent. Declared here rather than in cs.h because the consumer is
// a sibling file of the same module: a public registration API would re-create the
// module boundary inside the module. Defined in cs.c, which owns the connection table
// the streams hang off.
void CsResultRegisterSink(CsResultSink sink);

// The sink slot itself, set by CsResultRegisterSink after it has released the streams.
// cs_result.c reads it to answer the one question the whole result path turns on: is
// there a consumer to hand a report to? With none, an event is dropped rather than
// decoded and stored, so a CS-active connection costs no report buffer until a
// consumer exists.
void CsResultSetSink(CsResultSink sink);

// The subevent currently being assembled for one connection (a CsConnection member).
// open is set by an accepted 0x31 and cleared once its subevent has been handed over;
// count is the number of step records decoded so far, which continue to accumulate
// across the 0x32 events that continue the same subevent. report is allocated once,
// on the first accepted 0x31 of the connection, and held for every later subevent of
// that connection - it is not the per-subevent reset that releases it. role and
// rttType are the configuration's (7.7.65,42 p2438), needed because the step payload
// length alone does not always name the template (see cs_result.c).
typedef struct {
    bool open;
    uint8_t count;
    uint8_t role;
    uint8_t rttType;
    CsSubeventReport *report;
} CsResultStream;

// What one absorbed event yielded. CS_RESULT_DROPPED covers every refusal - no sink
// registered, a done-status combination outside Table 7.2, a step region running past
// the event, a 0x32 with no subevent open - and carries no report, so a consumer is
// never handed a partial subevent.
typedef enum {
    CS_RESULT_DROPPED,
    CS_RESULT_CONTINUES,  // Subevent_Done_Status 0x1: more 0x32 events follow
    CS_RESULT_ENDED,      // Subevent_Done_Status 0x0 or 0xF: the report is complete
} CsResultOutcome;

// Absorb the 0x31 that opens a subevent. role and rttType come from the CS
// configuration the report is attributed to (its 0x2F mirror); a subevent whose slot
// the engine does not hold is refused by the caller before this point, because without
// the role a mode-0 step cannot be decoded at all.
CsResultOutcome CsResultAbsorbResult(CsResultStream *stream,
    const HciLeCsSubeventResultEnvelope *envelope, uint8_t role, uint8_t rttType);

// Absorb a 0x32 continuing the open subevent. The stream keeps the opening event's
// counters and the role/rttType it was opened with; only the done statuses, the abort
// reason and the antenna-path count are refreshed from the continuation.
CsResultOutcome CsResultAbsorbContinue(CsResultStream *stream,
    const HciLeCsSubeventResultContinueEnvelope *envelope);

// Hand stream->report to the registered sink. A no-op without a sink or a report, so
// the caller does not repeat either check. A release the sink makes of this same stream
// while it runs is deferred to the return of this call, so the buffer is not freed under
// the sink reading it.
void CsResultEmit(const CsResultStream *stream);

// Clear the open subevent, keeping the report for the next one. Used after a report
// has been handed over, and on any refusal that leaves the stream's contents suspect.
void CsResultResetStream(CsResultStream *stream);

// Release the report itself. Used when the connection goes away and when the module
// shuts down - never between subevents of a live connection, but a consumer's own
// callback may reach it (a close or a re-registration made from inside the sink, which
// the CSIF_ entry's inline fast path runs on this thread) and then it parks the buffer
// until that sink returns instead of freeing it mid-call.
void CsResultReleaseStream(CsResultStream *stream);

// --- CS test path (C-8) -------------------------------------------------------
// The seam between cs.c (HCI event routing) and cs_test.c (the 0x0095/0x0096 test:
// its request record, its outcome, and its own 0x0FFF report stream). A CS test is
// not tied to any connection - both commands stand alone, the results arrive under
// Connection_Handle 0x0FFF (7.7.65,44 p2448), and no Disconnection Complete will
// ever arrive to close it - so none of it hangs off the connection table.

// The outcome of a CS test, as the Controller reported it. testComplete carries the
// Status of the 0x0095 Command_Complete (7.8.142 p2840) and testEndComplete the Status
// of the 0x33 LE CS Test End Complete subevent (7.7.65,46 p2463). Zero means the test
// started or ended as asked; every other value is the error the Controller raised
// (0x0C a test is already enabled / no test to end, 7.8.142 p2843 and 7.8.143 p2855)
// or the local send failure HCI reported, passed through untouched.
typedef struct {
    void (*testComplete)(uint8_t status);
    void (*testEndComplete)(uint8_t status);
} CsTestSink;

// Register the one test sink; a NULL sink clears it. Each member may be NULL on its
// own, so a consumer that only cares whether the test started leaves testEndComplete
// clear. Declared here rather than in cs.h for the same reason as
// CsResultRegisterSink: the consumer is a sibling file of this module. The struct is
// copied, so the caller's storage need not outlive the call.
void CsTestRegisterSink(const CsTestSink *sink);

// The 0x31 opening a test subevent, diverted by cs.c before the connection lookup.
// Decoded into the test's own stream with the Role and RTT_Type of the 0x0095 that
// started the test - the event carries neither, and only a running test's parameters
// give a mode-0 step its meaning - then handed to the CS result sink under its 0x0FFF
// handle, the same report shape a connection's subevent is delivered as.
void CsTestOnSubeventResult(const HciLeCsSubeventResultEnvelope *envelope);

// The 0x32 continuing the open test subevent. The stream is the whole of the routing
// state, exactly as on the connection path.
void CsTestOnSubeventResultContinue(const HciLeCsSubeventResultContinueEnvelope *envelope);

// The 0x0095 Command_Complete: commit the request (status 0x00 makes its Role and
// RTT_Type the running test's) or discard it, then notify the sink. Reached from the
// failure row on a command timeout too, so a lost 0x0095 cannot wedge the record.
void CsTestOnStartComplete(const HciLeCsTestReturnParam *returnParam);

// The 0x33 LE CS Test End Complete: status 0x00 is the end of the test - the Controller
// sends it only after every result of that test has been reported (7.8.143 p2855), so
// the record is dropped here and not on the 0x0096 Command_Status. A nonzero status
// leaves a test running that is still running. Also synthesized from the 0x0096
// failure row. Notifies the sink on both outcomes.
void CsTestOnTestEndComplete(const HciLeCsTestEndCompleteEventParam *eventParam);

// Release the test's buffered report, keeping the record: a sink change must not cost
// a test the Controller is still running its decode context. Called by
// CsResultRegisterSink alongside the per-connection releases.
void CsTestReleaseReport(void);

// Drop the whole test record, report included: the module is starting or has stopped,
// so no event will close a test that was in flight and the pending latch must go or
// every later CsTestStart would refuse with BT_ALREADY.
void CsTestReset(void);

// --- CS session path (C-9.2) --------------------------------------------------
// The seam between cs.c, which routes the HCI events, and the file that will forward
// them to a consumer outside the engine. It carries every completion that belongs to a
// connection and that a consumer has a reason to wait on: the five subevents a peer
// exchange produces (0x2C, 0x2D, 0x2E, 0x2F, 0x30); the five Command_Completes whose
// refusals the Host cannot fully predict locally - 0x0093 (7.8.140, also changes Host
// state), 0x008D (7.8.134), 0x0092 (7.8.139), and the write-backs 0x008B/0x008F
// (7.8.132/7.8.136: "CS configuration already created" is Controller-owned 0x0C, so
// a non-success return must reach the sink); and the end of the session itself.
// CsWriteBackRemoteCaps/CsWriteBackRemoteFae still answer BT_ALREADY when this
// connection's cache row already names the same handle (a prior successful write-back
// or exchange), but they do not mirror "configuration exists". The other two seams
// stay where they are: subevent measurements go to CsResultSink and the CS test's
// two outcomes to CsTestSink, so no event is reported twice.
// Every member is called on the Stack thread, synchronously inside the HCI event
// dispatch (the LE Meta dispatch or the Command_Complete one) and with the event
// callback-list lock held (see CsResultRegisterSink for
// why that lock and not another), so a consumer must not block and must copy anything
// it keeps. eventParam is the engine's view of the frame and is valid only for the
// duration of the call. Each is called after the engine has recorded the event, so a
// consumer that reads back through cs.h - CsGetConfig, CsGetProcedureState - sees the
// state the event just produced rather than the state before it.

typedef struct {
    // 0x2C, 7.7.65,39 p2428. Status 0x00 is what caches the peer's capability body;
    // any other status means nothing was cached and CsGetRemoteCapabilities still
    // reports BT_NOT_SUPPORT for the peer.
    void (*remoteCapabilitiesComplete)(
        const HciLeCsReadRemoteSupportedCapabilitiesCompleteEventParam *eventParam);
    // 0x2D, 7.7.65,40 pp2433-2434. Only Status 0x00 carries a table; 0x11 is the
    // No_FAE answer and every other status a failure, and neither caches anything.
    void (*remoteFaeTableComplete)(const HciLeCsReadRemoteFaeTableCompleteEventParam *eventParam);
    // 0x008B Command_Complete, 7.8.132. Status 0x00 records learnedOnHandle; any
    // other status (including Controller 0x0C when a CS configuration already
    // exists) is reported so the caller sees the refusal that the sync return
    // deliberately left to the Controller.
    void (*writeCachedRemoteCapabilitiesComplete)(
        const HciLeCsWriteCachedRemoteSupportedCapabilitiesReturnParam *returnParam);
    // 0x008F Command_Complete, 7.8.136. FAE counterpart of the 0x008B member above.
    void (*writeCachedRemoteFaeTableComplete)(
        const HciLeCsWriteCachedRemoteFaeTableReturnParam *returnParam);
    // 0x2E, 7.7.65,41 p2434. Reported to the session sink; the engine records nothing
    // from it - Security Start completion is a Controller/LL gate (Vol 6 Part B §5.1.26).
    void (*securityEnableComplete)(const HciLeCsSecurityEnableCompleteEventParam *eventParam);
    // 0x2F, 7.7.65,42 p2437. Action 0x01 stores the config mirror, Action 0x00
    // drops the slot; a reserved Config_ID or Action is dropped by the engine and
    // never reaches here.
    void (*configComplete)(const HciLeCsConfigCompleteEventParam *eventParam);
    // 0x30, 7.7.65,43 pp2442-2445. State 0x01 stores the schedule, 0x00 clears it; a
    // Config_ID the engine holds no slot for is still reported, only its record is
    // skipped, because the outcome belongs to whoever is waiting for it.
    void (*procedureEnableComplete)(const HciLeCsProcedureEnableCompleteEventParam *eventParam);
    // The 0x0093 Command_Complete, 7.8.140 pp2836-2837. A nonzero status leaves the
    // slot's procParamsSet clear, so a later CsProcedureEnable on that Config_ID
    // reports 0x0C through procedureEnableComplete rather than reaching the wire. The
    // return names only Connection_Handle - Config_ID is not echoed - so a consumer
    // correlates it with its own request, and a completion with no request outstanding
    // is reported as well rather than swallowed, because it is still a result of some
    // 0x0093. A frame naming no connection (CS_HANDLE_NONE) is not reported: the engine
    // clears every pending record and there is nothing to attribute it to. The sentinel
    // means the handle could not be recovered - the return was too short to carry one, or
    // the command's parameter block was never allocated.
    void (*procedureParametersComplete)(const HciLeCsSetProcedureParametersReturnParam *returnParam);
    // The 0x008D Command_Complete, 7.8.134 p2816. The engine records nothing from it -
    // the defaults live in the Controller and are connection-scoped - but it is reported
    // all the same, because 0x11 (a value the Controller will not take) and 0x12 (a role
    // it will not accept in the connection's current state) are its answers to give and
    // no Host-side copy predicts them. Connection_Handle is CS_HANDLE_NONE / LE_CS_-
    // INVALID_HANDLE when the handle could not be recovered: the failure row (a timeout,
    // or a Command_Complete whose return body was empty or too short to hold a handle)
    // rebuilds or rewrites the return so the waiter is still unblocked. 0x008D keeps no
    // engine state, so there is nothing to clear on it and the return is passed on as it
    // stands.
    void (*setDefaultSettingsComplete)(const HciLeCsSetDefaultSettingsReturnParam *returnParam);
    // The 0x0092 Command_Complete, 7.8.139 pp2830-2831. Nothing is recorded from it
    // either: the classification is Controller-wide and outlives the link. Its status is
    // the whole result and no Connection_Handle rides along, so there is nothing to
    // attribute it to. 0x0C is a re-issue inside the Controller's 1-second window and
    // 0x12 an invalid map; D5-1 decided not to track the window locally, so this
    // completion is the only place those answers appear.
    void (*setChannelClassificationComplete)(const HciLeCsSetChannelClassificationReturnParam *returnParam);
    // The CS session ended because the ACL behind it did. Carries no reason, because
    // the ACL layer reports that itself and what the engine did here is the same
    // either way - it follows the row. Fires on a completed Disconnection Complete
    // that matched a CS session and not on CsCloseSession, which is the Host ending
    // the session this seam does not have to announce to itself.
    void (*sessionClosed)(uint16_t connectionHandle);
} CsSessionSink;

// Register (sink non-NULL) or clear (NULL) the session sink. Registration swaps a
// plain function-pointer struct and touches no other state, so unlike
// CsResultRegisterSink there is nothing to release under the lock; it takes the lock
// for the same reason that one does - a dispatch must never read a half-applied
// struct - and refuses when the HCI event layer is not up, the same shape kept in step
// rather than a hazard of its own. Declared here rather than in cs.h for the same
// reason as CsResultRegisterSink: the consumer is a sibling file of this module.
// Each member may be NULL on its own.
void CsSessionRegisterSink(const CsSessionSink *sink);

// Run func(ctx) on the CS processing queue and return after it completes; ctx stays owned
// by the caller, which must not release it until this returns. Declared here for the same
// reason as the sink registrations above: the one consumer is cs_if.c, a sibling file of
// this module, and a public registration API would re-create the module boundary inside
// the module. cs_if.c wraps every CSIF_ entry in this, which is what puts a reader on the
// Stack thread beside the HCI callbacks that write the same state (option A of the plan's
// threading decision) instead of leaving it on the caller's thread. See cs_task_common.c
// for the two contracts that come with it: the inline fast path taken when the caller is
// already on the Stack thread, and its no-ordering, no-queue-check caveats.
int CsRunTaskBlockProcess(void (*func)(void *), void *ctx);

#ifdef __cplusplus
}
#endif

#endif
