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

#ifndef HCI_CMD_FAILURE_H
#define HCI_CMD_FAILURE_H

#include <stdint.h>

// Fallback handle a row carries when it is handed no parameter block (see the contract below),
// for the two-octet handle fields a synthesized completion can carry (Connection_Handle,
// CIS_Handle, Sync_Handle). 0xFFFF is outside their valid 0x0000-0x0EFF range, so a lookup by it
// always misses and the completion reaches no live link. A row must not leave the zero-initialized
// 0x0000 in place instead: 0x0000 is a valid handle and the report would land on an unrelated link.
#define INVALID_CONNECTION_HANDLE 0xFFFF

// The same fallback for a one-octet handle field (BIG_Handle and CIG_ID are both 0x00-0xEF in the
// spec). 0xFF is outside that range, while 0x00 is a valid value and must not be left in place for
// the reason above - nor may INVALID_CONNECTION_HANDLE be used here: it would be truncated to 0xFF.
#define INVALID_HANDLE_8BIT 0xFF

// The same fallback for a one-octet address type field (Peer_Address_Type, Advertiser_Address_Type).
// Valid address types are 0x00-0x03; 0xFF is reserved and signals "unknown/unavailable" in
// synthesized failure events.
#define INVALID_ADDRESS_TYPE 0xFF

#ifdef __cplusplus
extern "C" {
#endif

// Contract for a per-opcode failure row. The row runs on a command-level rejection
// (Command_Status carrying a nonzero status), on a command timeout, or when a unit
// refuses the command before the wire and has to report it in the same shape the
// Controller would have (the 0x0094 enable preconditions the CS engine checks with
// HCI_LE_CS_PROCEDURE_ENABLE). param is the original command's parameter block, and it
// is NULL when the command was sent without one (HciAllocCmd(opCode, NULL, 0)): on the
// timeout path that is every parameterless Command_Complete command. A row must
// therefore check param for NULL before dereferencing it; every row in this stack does
// so, so a new row must follow the same pattern. A caller reporting its own refusal
// passes the parameter block it refused, which the row reads synchronously.
typedef void (*HciCmdOnFailedFunc)(uint8_t status, const void *param);

// Dispatch the row for opCode. Rows are invoked without g_lockProcessingCmds held, so
// a caller that already holds that lock must not reach here.
void HciOnCmdFailed(uint16_t opCode, uint8_t status, const void *param);

#ifdef __cplusplus
}
#endif

#endif