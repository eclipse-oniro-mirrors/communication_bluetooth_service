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

#include "cs_internal.h"

#include "log.h"

#include "hci/evt/hci_evt.h"
#include "hci/hci_error.h"
#include "hci/hci_le_controller_6_0.h"
#include "platform/include/mutex.h"

// BLUETOOTH SPECIFICATION Version 6.0 | Vol 4, Part E
// 7.8.142 LE CS Test Command (0x0095, pp2840-2854), 7.8.143 LE CS Test End Command
// (0x0096, p2855) and 7.7.65,46 LE CS Test End Complete subevent (0x33, p2463).
// A CS test is a CS procedure the Host drives without a link: the local Controller
// runs it against a tester instead of a peer, reports its subevents under
// Connection_Handle 0x0FFF (7.7.65,44 p2448, an identifier no ACL handle uses and no
// Disconnection Complete will ever release), and closes the test with a Status-only
// subevent once every result has been reported (7.8.143 p2855). Neither command is
// connection-scoped - 0x0095 carries no Connection_Handle at all - so none of this
// hangs off the connection table, and the one test that can exist at a time
// (7.8.142 p2843: a second 0x0095 while one is enabled draws Command Disallowed) is
// held in a single record rather than a table.
// The record carries two things the events cannot. The first is the decode context:
// a test's 0x31/0x32 carry neither Role nor RTT_Type - just as a connection's do not
// (see cs_result.c) - so the two parameters the test was started with (0x0095 Role
// and RTT_Type) are what a step is decoded against, and they have to be remembered
// until the test ends. The second is the request itself: the 0x0095 Command_Complete
// returns Status alone (7.8.142 p2840), so a request is recorded when it is sent and
// becomes the running test's only when Status 0x00 comes back. Without that split a
// refused second 0x0095 - 0x0C on p2843, sent while a test is genuinely running -
// would overwrite the Role and RTT_Type of the test already in progress.
// The result stream below is the same CsResultStream the connection path uses, with
// the same step decode and the same one-report-per-stream buffer, so a test
// measurement reaches the CS result sink as an ordinary CsSubeventReport whose
// connectionHandle is 0x0FFF.

// The one test record. pendingRole/pendingRttType and role/rttType are deliberately
// two pairs: a rejected 0x0095 leaves the running test's decode context untouched.
// endPending is the 0x0096 counterpart of requestPending, and it exists because the
// 0x33 carries a status octet and nothing else (7.7.65,46 p2463): it cannot say which
// test it ended. Without it, a 0x0095 recorded while a 0x0096 is still on the wire
// loses its Role and RTT_Type to the 0x33 handler's reset - and a later Command_Complete
// with Status 0x00 then finds nothing to commit, so the whole new test is discarded
// while the Controller runs it.
typedef struct {
    bool requestPending;    // a 0x0095 is on the wire, its Command_Complete not yet in
    uint8_t pendingRole;    // Role of the pending 0x0095, read only while the above is set
    uint8_t pendingRttType;
    bool endPending;        // a 0x0096 is on the wire, its 0x33 not yet in
    bool running;           // the Controller accepted a 0x0095 and the test is enabled
    uint8_t role;           // Role of the running test, as its 0x0095 asked for
    uint8_t rttType;
    CsResultStream stream;  // the running test's subevent assembly (handle 0x0FFF)
} CsTestState;

static CsTestState g_test;
static CsTestSink g_testSink;

// Registration swaps the pointer pair and touches no other state, so unlike
// CsResultRegisterSink there is nothing to release under the lock. The lock is taken for
// the reason that one documents: it is the one the LE Meta dispatch holds across every
// callback it makes, so a registration either completes before a dispatch starts or waits
// it out, and no dispatch ever reads a half-copied struct - the same shape as
// CsResultRegisterSink and CsSessionRegisterSink in cs.c, refused the same way when the
// event layer is not up. The caller must have reached HciInitEvent: CsInitialize runs
// earlier, on BTM_Initialize's module loop, and a registration placed there would find no
// lock at all. Like the other two sinks this one is not cleared by CsInitialize, so a
// consumer that goes away unregisters.
void CsTestRegisterSink(const CsTestSink *sink)
{
    Mutex *lock = HciGetEventCallbackListLock();
    if (lock == NULL) {
        LOG_WARN("%{public}s: HCI event layer not up, sink not registered", __FUNCTION__);
        return;
    }
    MutexLock(lock);
    if (sink == NULL) {
        (void)memset_s(&g_testSink, sizeof(g_testSink), 0, sizeof(g_testSink));
    } else {
        g_testSink = *sink;
    }
    MutexUnlock(lock);
}

// The test's buffered report is released here and not kept across a startup or a
// shutdown: a test in flight would otherwise hold a report buffer for the rest of the
// process lifetime, and its pending latch would make every later CsTestStart fail.
void CsTestReset(void)
{
    CsResultReleaseStream(&g_test.stream);
    (void)memset_s(&g_test, sizeof(g_test), 0, sizeof(g_test));
}

// A sink change releases every buffered report so a new consumer starts from an empty
// buffer (CsResultRegisterSink). The record itself must survive it: the Controller
// keeps running the test, and without the Role and RTT_Type the results still to come
// could not be decoded.
void CsTestReleaseReport(void)
{
    CsResultReleaseStream(&g_test.stream);
}

// 0x0095 is sent with the test's Role and RTT_Type recorded first, because the only
// thing that will ever come back to name the accepted test is a Status. BT_ALREADY
// while a request is outstanding is not a Controller rule but an attribution one: two
// 0x0095 Command_Completes carry the same shape, so a second request would leave both
// results unattributable - the same reason CsSetProcedureParameters refuses a second
// 0x0093. A 0x0096 on the wire refuses the start for the same reason: its 0x33 can
// arrive on either side of this 0x0095's Command_Complete, and if it arrives after,
// its CsTestReset() wipes the pendingRole/pendingRttType this call just wrote and the
// Command_Complete that follows finds no request outstanding. A test already enabled
// is not refused here: the Controller answers that one itself, with the 0x0C of
// 7.8.142 p2843, and the discard-pending rule below then leaves the running test's
// parameters where they were.
int CsTestStart(const HciLeCsTestParam *param)
{
    if (param == NULL) {
        return BT_BAD_PARAM;
    }
    if (g_test.requestPending) {
        LOG_WARN("%{public}s: a 0x0095 is already outstanding", __FUNCTION__);
        return BT_ALREADY;
    }
    if (g_test.endPending) {
        LOG_WARN("%{public}s: a 0x0096 is outstanding, the previous test is not torn down yet", __FUNCTION__);
        return BT_ALREADY;
    }
    g_test.pendingRole = param->prefix.role;
    g_test.pendingRttType = param->prefix.rttType;
    g_test.requestPending = true;
    int result = HCI_LeCsTest(param);
    if (result != BT_SUCCESS) {
        g_test.requestPending = false;
    }
    return result;
}

// 0x0096 has no parameter to record, so unlike CsTestStart there is nothing to write
// before the send - but the latch still has to go in before it, or a 0x0095 that
// started right after would be the one the 0x33 handler's reset destroyed. BT_ALREADY
// while one is outstanding tells the caller to wait for the testEndComplete sink
// rather than to fire a second end. Sending it with no test running is left to the
// Controller, whose 0x0C (7.8.143 p2855) arrives as a synthesized 0x33 the way every
// other command-level rejection of a Command_Status command does, so the caller learns
// the outcome on the same path as a success.
int CsTestEnd(void)
{
    if (g_test.endPending) {
        LOG_WARN("%{public}s: a 0x0096 is already outstanding", __FUNCTION__);
        return BT_ALREADY;
    }
    g_test.endPending = true;
    int result = HCI_LeCsTestEnd();
    if (result != BT_SUCCESS) {
        g_test.endPending = false;
    }
    return result;
}

// The 0x0095 Command_Complete. A request that is not outstanding cannot be attributed
// to anything and is dropped, as on the 0x0093 path. Status 0x00 commits the request:
// its Role and RTT_Type become the running test's, and the stream is cleared of
// anything a previous test left in it. Every other status - 0x0C a test is already
// enabled, 0x11/0x12 an unsupported parameter value, or a local send failure reported
// by the failure row on timeout - discards the pending request and leaves a running
// test exactly as it was.
void CsTestOnStartComplete(const HciLeCsTestReturnParam *returnParam)
{
    if (returnParam == NULL) {
        return;
    }
    if (!g_test.requestPending) {
        LOG_WARN("%{public}s: no 0x0095 outstanding, result dropped", __FUNCTION__);
        return;
    }
    g_test.requestPending = false;
    if (returnParam->status == HCI_SUCCESS) {
        g_test.running = true;
        g_test.role = g_test.pendingRole;
        g_test.rttType = g_test.pendingRttType;
        CsResultResetStream(&g_test.stream);
    }
    if (g_testSink.testComplete != NULL) {
        g_testSink.testComplete(returnParam->status);
    }
}

// The 0x33 LE CS Test End Complete. The Controller sends it only after every result of
// the test it ended has been reported (7.8.143 p2855), so a successful one is the end
// of the test as far as the Host is concerned and the record goes with it; a test that
// ends on its own - the parameters of 0x0095 schedule a finite procedure - reaches this
// same clear only by way of a later test's 0x0095 commit, because no 0x33 follows a
// test the Host never asked to end. A nonzero status means the end did not take
// effect, so a running test stays running and its results keep being decoded - but the
// 0x0096 that was outstanding is over either way, so endPending drops before the branch
// (a rejected end clears it too, and a latched one would refuse every later CsTestEnd).
// A 0x33 with no 0x0096 outstanding is not attributable to anything and is dropped, as on
// the 0x0095 path (CsTestOnStartComplete): a second 0x33 for one end, or the late arrival
// of an end that already timed out, must not clear a record it does not own.
void CsTestOnTestEndComplete(const HciLeCsTestEndCompleteEventParam *eventParam)
{
    if (eventParam == NULL) {
        return;
    }
    // Only a real end may clear the record.
    if (!g_test.endPending) {
        LOG_WARN("%{public}s: no 0x0096 outstanding, result dropped", __FUNCTION__);
        return;
    }
    g_test.endPending = false;
    if (eventParam->status == HCI_SUCCESS) {
        CsTestReset();
    } else {
        LOG_WARN("%{public}s: CS test end failed with status 0x%02x", __FUNCTION__, eventParam->status);
    }
    if (g_testSink.testEndComplete != NULL) {
        g_testSink.testEndComplete(eventParam->status);
    }
}

// 7.7.65,44 LE CS Subevent Result subevent (0x31), diverted here by cs.c before the
// connection lookup because 0x0FFF is outside the ACL handle range. The result is
// decoded and handed over exactly as a connection's is, under the running test's Role
// and RTT_Type; a 0x31 that arrives with no test running has no parameters to decode
// against, so it is dropped rather than guessed at.
void CsTestOnSubeventResult(const HciLeCsSubeventResultEnvelope *envelope)
{
    if (envelope == NULL) {
        return;
    }
    if (!g_test.running) {
        LOG_WARN("%{public}s: CS test subevent result with no test running, dropped", __FUNCTION__);
        return;
    }
    if (CsResultAbsorbResult(&g_test.stream, envelope, g_test.role, g_test.rttType) != CS_RESULT_ENDED) {
        return;
    }
    CsResultEmit(&g_test.stream);
    CsResultResetStream(&g_test.stream);
}

// 7.7.65,45 LE CS Subevent Result Continue subevent (0x32). The stream's open flag is
// the whole routing rule, as on the connection path: only a 0x31 accepted for a
// running test lets a 0x32 be read as a continuation.
void CsTestOnSubeventResultContinue(const HciLeCsSubeventResultContinueEnvelope *envelope)
{
    if (envelope == NULL) {
        return;
    }
    if (CsResultAbsorbContinue(&g_test.stream, envelope) != CS_RESULT_ENDED) {
        return;
    }
    CsResultEmit(&g_test.stream);
    CsResultResetStream(&g_test.stream);
}
