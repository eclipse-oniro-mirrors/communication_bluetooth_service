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

#ifndef GAP_LE_IF_5_3_H
#define GAP_LE_IF_5_3_H

#include "gap_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

// LE Connection Subrating change event information (HCI_LE_Subrate_Change
// subevent 0x23 payload, BLUETOOTH SPECIFICATION Version 5.3 Vol 4 Part E
// 7.7.65,35). Delivered to GapLeSubrateCallback::subrateChange after the link
// layer applies a new subrate factor (either from a local HCI_LE_Subrate_Request
// or from the peer). Delivered on the Stack thread.
typedef struct {
    uint8_t status;
    uint16_t connectionHandle;
    uint16_t subrateFactor;
    uint16_t peripheralLatency;
    uint16_t continuationNumber;
    uint16_t supervisionTimeout;
} GapLeSubrateChangeInfo;

// Default subrate parameters of HCI_LE_Set_Default_Subrate (7.8.123): the host
// advertises these as its default subrating capabilities on new connections.
typedef struct {
    uint16_t defaultSubrateMin;
    uint16_t defaultSubrateMax;
    uint16_t defaultMaxLatency;
    uint16_t defaultContinuationNumber;
    uint16_t defaultSupervisionTimeout;
} GapLeSubrateDefaultParams;

// Subrate parameters of one HCI_LE_Subrate_Request (7.8.124): the requested
// subrate factor window, latency and continuation cap for the connection. Field
// ranges are the command parameter limits (shared with the HCI layer through
// hci_def_le_cmd.h); an out-of-range field is rejected by the GAPIF gate.
typedef struct {
    uint16_t connectionHandle;   // connection handle (0x0000-0x0EFF)
    uint16_t subrateMin;         // minimum subrate factor (0x0001-0x01F4)
    uint16_t subrateMax;         // maximum subrate factor, >= subrateMin and
                                 // subrateMax x (maxLatency + 1) <= 500
    uint16_t maxLatency;         // maximum peripheral latency (0x0000-0x01F3)
    uint16_t continuationNumber; // maximum continuation number, < subrateMax (0x0000-0x01F3)
    uint16_t supervisionTimeout; // supervision timeout in 10 ms units (0x000A-0x0C80)
} GapLeSubrateRequestParams;

typedef struct {
    // LE Subrate Change event: subrating parameters actually applied by the
    // link layer, keyed by connection handle (no address lookup is required;
    // the payload carries the handle).
    void (*subrateChange)(const GapLeSubrateChangeInfo *info, void *context);
    // Completion of HCI_LE_Set_Default_Subrate (7.8.123), status only.
    void (*setDefaultSubrateResult)(uint8_t status, void *context);
    // Completion of HCI_LE_Subrate_Request (7.8.124), status only (the command
    // complete carries no connection handle; the applied parameters arrive
    // later through subrateChange).
    void (*subrateRequestResult)(uint8_t status, void *context);
} GapLeSubrateCallback;

/**
 * @brief       Set the default Connection Subrating parameters applied to
 *              subsequent connections (HCI_LE_Set_Default_Subrate, Vol 4
 *              Part E 7.8.123).
 * @param[in]   params              subrate parameters (NULL rejected)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 * @note        Requires LE Connection Subrating controller support (bit 37 of
 *              the LE features) for the Controller to honor the defaults; the
 *              result is reported through GapLeSubrateCallback::setDefaultSubrateResult.
 */
BTSTACK_API int GAPIF_LeSetDefaultSubrate(const GapLeSubrateDefaultParams *params);

/**
 * @brief       Request a new subrate factor on a connection, Central or
 *              Peripheral (HCI_LE_Subrate_Request, Vol 4 Part E 7.8.124; both
 *              roles may issue the request).
 * @param[in]   params              subrate request parameters (NULL rejected;
 *                                  field ranges documented in GapLeSubrateRequestParams)
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 * @note        The request acceptance is reported through
 *              GapLeSubrateCallback::subrateRequestResult; the parameters
 *              actually applied arrive through GapLeSubrateCallback::subrateChange.
 */
BTSTACK_API int GAPIF_LeSubrateRequest(const GapLeSubrateRequestParams *params);

/**
 * @brief       Register LE Connection Subrating callback.
 * @param[in]   callback            LE Connection Subrating callback
 * @param[in]   context             callback context parameter
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 * @note        Single-slot registration, the callback structure is copied by value during the
 *              call (stack-local structures are safe); the context pointer is stored and must
 *              stay valid until deregistered. All callbacks run on the Stack thread, the same
 *              thread that processes the HCI events they originate from; callers must not
 *              block inside a callback on a resource owned by the Stack thread.
 */
BTSTACK_API int GAPIF_RegisterLeSubrateCallback(const GapLeSubrateCallback *callback, void *context);

/**
 * @brief       Deregister LE Connection Subrating callback.
 * @return      @c BT_SUCCESS      : The function is executed successfully.
 *              @c otherwise        : The function is not executed successfully.
 * @note        Safe to call from inside a subrate callback (i.e. from within any
 *              member of GapLeSubrateCallback): the implementation detects the
 *              re-entrant call through a thread-local dispatch flag and returns
 *              immediately, skipping the reference-drain wait that would otherwise
 *              block the Stack thread (the dispatch has already copied the callback by
 *              value, so the cleared registration cannot affect the ongoing dispatch).
 *              A non-success return (GAP_ERR_REMOTE_ACTION) means the
 *              deregistration did not complete cleanly: the 60 s drain window
 *              elapsed while dispatches were still in flight, the callback
 *              registration was already cleared, and the callback context mutex
 *              is deliberately leaked (it may still be locked by the in-flight
 *              dispatch). The outstanding reference then makes every later
 *              (re-)Init return GAP_ERR_OUT_OF_RES until the module itself is
 *              torn down, so the leak cannot be repaired in-process. Under the
 *              current single-Stack-thread model no dispatch can overlap
 *              Deregister and this path is not reachable; it only guards a
 *              future cross-thread dispatcher.
 */
BTSTACK_API int GAPIF_LeDeregisterSubrateCallback(void);

#ifdef __cplusplus
}
#endif

#endif // GAP_LE_IF_5_3_H
