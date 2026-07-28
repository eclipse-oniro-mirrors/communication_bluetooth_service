/**
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
 * Real-logic asserts for core Bluetooth modules (1A: single board, BT ON, no peer).
 * - Local APIs: prefer errCode === 0; capability may allow {0,201,801}.
 * - Peer APIs: errCode must be in documented @throws whitelist (no errCode >= 0).
 */
import { expect } from '@ohos/hypium';
import access from '@ohos.bluetooth.access';
import { assertPerfBudget, assertStressHasSuccess } from './ApitestCoverageHelpers';
import LogUtil from '../../../main/ets/Utils/LogUtil';

/** Documented Bluetooth BusinessError codes commonly listed in jsapi/26 @throws. */
export const BT_DOC_ERR_CODES: number[] = [
  0, 201, 202, 203, 401, 801,
  2900001, 2900003, 2900004, 2900007, 2900008, 2900009, 2900010,
  2900011, 2900013, 2900014, 2900099,
  9200001, 9200002, 9200010
];

/**
 * Local / capability path (1A): success, permission, invalid-param, capability, or
 * documented service/BT-disabled/operation failures — NOT an open 2900xxx range.
 */
export const BT_LOCAL_OR_CAP: number[] = [
  0, 201, 202, 203, 401, 801, 2900001, 2900003, 2900099,
  9200001, 9200002, 9200010
];

/** Peer / remote path: documented env failures (no open-ended range). */
export const BT_PEER_EXPECT: number[] = [
  0, 201, 202, 203, 401, 801, 2900001, 2900003, 2900004, 2900007, 2900008,
  2900009, 2900010, 2900011, 2900013, 2900014, 2900099,
  9200001, 9200002, 9200010
];

export function assertErrIn(errCode: number, allowed: number[]): void {
  let ok = false;
  for (let i = 0; i < allowed.length; i++) {
    if (errCode === allowed[i]) {
      ok = true;
      break;
    }
  }
  expect(ok).assertTrue();
}

/** Local main path: success or documented capability/service failure. */
export function assertLocalOk(errCode: number): void {
  assertErrIn(errCode, BT_LOCAL_OR_CAP);
}

/** Peer / remote main path: whitelist from d.ts @throws (+0 if sync no-throw). */
export function assertPeerExpect(errCode: number): void {
  assertErrIn(errCode, BT_PEER_EXPECT);
}

/**
 * Exception / boundary path:
 * - If platform throws BusinessError: code must be non-zero and in whitelist.
 * - If platform does not throw (returns false / no-op): errCode stays 0 — acceptable on 1A board.
 */
export function assertThrownErrIn(threw: boolean, errCode: number, allowed: number[]): void {
  if (threw) {
    expect(errCode !== 0).assertTrue();
    assertErrIn(errCode, allowed);
  } else {
    expect(errCode).assertEqual(0);
  }
}

/**
 * Ensure Bluetooth is STATE_ON (1A precondition). Best-effort enable + poll.
 * Does not fail the suite if enable is denied (assertLocalOk still applies per case).
 */
export function ensureBtOn(): void {
  try {
    const st = access.getState();
    if (st === access.BluetoothState.STATE_ON) {
      return;
    }
  } catch (_e) {
    LogUtil.error(JSON.stringify(_e));
    // fall through to enable
  }
  try {
    access.enableBluetooth();
  } catch (_e2) {
    LogUtil.error(JSON.stringify(_e2));
    // permission / already enabling
  }
  const deadline = Date.now() + 8000;
  while (Date.now() < deadline) {
    try {
      if (access.getState() === access.BluetoothState.STATE_ON) {
        return;
      }
    } catch (_e3) {
      LogUtil.error(JSON.stringify(_e3));
      // continue poll
    }
  }
}

export function runPerf(tag: string, budgetMs: number, fn: () => void): void {
  assertPerfBudget(tag, 8, budgetMs, fn);
}

export function runStress(tag: string, iterations: number, fn: () => void): void {
  assertStressHasSuccess(tag, iterations, fn);
}

/**
 * Register a one-shot timer then invoke done(). Hypium async cases pass done.
 * Callers should register on/off listeners before invoking the triggering API.
 */
export function waitCallback(done: Function, timeoutMs: number): void {
  setTimeout(() => {
    done();
  }, timeoutMs);
}
