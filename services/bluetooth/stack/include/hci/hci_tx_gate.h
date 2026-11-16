/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HCI_HCI_TX_GATE_H
#define HCI_HCI_TX_GATE_H

#include <stdbool.h>

// Serializes HCI sends: at most one packet may be in flight. The caller owns
// the lock; this type only holds the decision state.
typedef struct {
    bool inFlight;
    bool stopped;
} HciTxGate;

static inline void HciTxGateReset(HciTxGate *gate)
{
    if (gate == NULL) {
        return;
    }
    gate->inFlight = false;
    gate->stopped = false;
}

static inline void HciTxGateStop(HciTxGate *gate)
{
    if (gate != NULL) {
        gate->stopped = true;
    }
}

static inline bool HciTxGateTryBegin(HciTxGate *gate)
{
    if (gate == NULL || gate->stopped || gate->inFlight) {
        return false;
    }
    gate->inFlight = true;
    return true;
}

static inline void HciTxGateComplete(HciTxGate *gate)
{
    if (gate != NULL) {
        gate->inFlight = false;
    }
}

static inline bool HciTxGateIsInFlight(const HciTxGate *gate)
{
    return gate != NULL && gate->inFlight;
}

static inline bool HciTxGateIsStopped(const HciTxGate *gate)
{
    return gate != NULL && gate->stopped;
}

static inline bool HciTxGateCanSend(const HciTxGate *gate)
{
    return gate != NULL && !gate->stopped && !gate->inFlight;
}

#endif // HCI_HCI_TX_GATE_H
