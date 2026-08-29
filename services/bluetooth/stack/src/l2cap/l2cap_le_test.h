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

/**
 * @file l2cap_le_test.h
 *
 * @brief Test-only seams of the LE part of the l2cap protocol.
 *
 * These declarations are deliberately not part of the production API (l2cap_le.h) so that
 * production code cannot call them: e.g. L2capLeEattExpirePendingRequest bypasses the real RTX
 * and would mark a legitimately in-flight 0x17 as timed out, deleting the whole batch. They are
 * meant to be included by the stack unit tests only.
 */

#ifndef L2CAP_LE_TEST_H
#define L2CAP_LE_TEST_H

#include "l2cap_def.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * @brief Test seam: expire the in-flight 0x17 batch of a connection as if its RTX timer fired
 *
 * Runs the response-timeout path directly, so it must be called from the L2CAP processing queue
 * (e.g. inside an L2capAsynchronousProcess callback) to stay serialized with the signal handlers.
 *
 * @param aclHandle connection handle of the ACL hosting the pending batch
 * @return Returns <b>BT_SUCCESS</b> if a pending 0x17 was expired, otherwise <b>BT_BAD_PARAM</b>.
 */
int L2capLeEattExpirePendingRequest(uint16_t aclHandle);

/**
 * @brief Test seam: deliver a 0x1A reconfigure response with the given Table 4.22 result
 *
 * Targets the in-flight 0x19 request (matched by its signal identifier in the pending list, then the
 * reconfig request with that identifier), not the first reconfigList node, so a concurrent 0x19 is
 * never injected into. Runs the 0x1A processing path directly, so it must be called from the L2CAP
 * processing queue (e.g. inside an L2capAsynchronousProcess callback) to stay serialized with the
 * signal handlers.
 *
 * @param aclHandle connection handle of the ACL hosting the in-flight 0x19 request
 * @param result Table 4.22 result code to deliver (0x0001-0x0004)
 * @return Returns <b>BT_SUCCESS</b> if the 0x1A was processed, otherwise <b>BT_BAD_PARAM</b>.
 */
int L2capLeEattInjectReconfigureRsp(uint16_t aclHandle, uint16_t result);

/**
 * @brief Test accessor: read the current negotiated LE connection parameters that drive the
 *        Vol 3 Part G 5.4 slave collision retry delay
 *
 * @param aclHandle connection handle of the ACL
 * @param intervalUnits [out] current connection interval in 1.25 ms units
 * @param slaveLatency [out] current connection slave latency in connection events
 * @return Returns <b>BT_SUCCESS</b> if the connection was found, otherwise <b>BT_BAD_PARAM</b>.
 */
int L2capLeEattGetConnectionParams(uint16_t aclHandle, uint16_t *intervalUnits, uint16_t *slaveLatency);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // L2CAP_LE_TEST_H
