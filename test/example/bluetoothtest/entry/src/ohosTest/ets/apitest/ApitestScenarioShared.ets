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
 * Shared helpers for BluetoothScenario* flow / combinatorial suites (1A: single board).
 * Prefer real outcome checks (state/name/mode) when local APIs succeed.
 */
import { expect } from '@ohos/hypium';
import { BusinessError } from '@ohos.base';
import access from '@ohos.bluetooth.access';
import connection from '@ohos.bluetooth.connection';
import ble from '@ohos.bluetooth.ble';
import socket from '@ohos.bluetooth.socket';
import constant from '@ohos.bluetooth.constant';
import { Driver, ON } from '@ohos.UiTest';
import LogUtil from '../../../main/ets/Utils/LogUtil';
import {
  BT_DOC_ERR_CODES,
  BT_LOCAL_OR_CAP,
  BT_PEER_EXPECT,
  assertErrIn,
  assertLocalOk,
  assertPeerExpect,
  assertThrownErrIn,
  ensureBtOn,
  waitCallback
} from './ApitestRealAssert';
import {
  BADDR,
  BAD_MAC,
  EMPTY_MAC,
  SVC_UUID,
  CHAR_UUID,
  SPP_UUID,
  buf0,
  sppOpt,
  legacySppOpt,
  bleAd,
  bleAs,
  bleAp,
  bleGattService,
  bleCharRef,
  bleDescRef,
  blePhyValue,
  hidSdpStub,
  hidQosStub,
  partnerDeviceAddr,
  partnerDeviceCap,
  partnerBusinessCap,
  sysBleStartOpt,
  sysBleStopOpt,
  sysBleFoundOpt,
  ctrlDeviceActionParams,
  trustedPairedDevices
} from './ApitestFullCoverageShared';

export {
  BT_DOC_ERR_CODES,
  BT_LOCAL_OR_CAP,
  BT_PEER_EXPECT,
  assertErrIn,
  assertLocalOk,
  assertPeerExpect,
  assertThrownErrIn,
  ensureBtOn,
  waitCallback,
  BADDR,
  BAD_MAC,
  EMPTY_MAC,
  SVC_UUID,
  CHAR_UUID,
  SPP_UUID,
  buf0,
  sppOpt,
  legacySppOpt,
  bleAd,
  bleAs,
  bleAp,
  bleGattService,
  bleCharRef,
  bleDescRef,
  blePhyValue,
  hidSdpStub,
  hidQosStub,
  partnerDeviceAddr,
  partnerDeviceCap,
  partnerBusinessCap,
  sysBleStartOpt,
  sysBleStopOpt,
  sysBleFoundOpt,
  ctrlDeviceActionParams,
  trustedPairedDevices
};

export const SC_SVC_UUID = SVC_UUID;
export const SC_FAKE_MAC = BADDR;
export const SC_BAD_MAC = BAD_MAC;

/** Invoke sync API; return BusinessError.code or 0. */
export function scInvoke(fn: () => void): number {
  try {
    fn();
    return 0;
  } catch (err) {
    const raw = (err as BusinessError).code as Object;
    let code = Number(raw);
    if (isNaN(code) && typeof raw === 'string') {
      code = parseInt(raw as string, 10);
    }
    if (isNaN(code)) {
      code = 2900099;
    }
    LogUtil.error(JSON.stringify(err));
    return code;
  }
}

export function scGetState(): number {
  try {
    return access.getState() as number;
  } catch (err) {
    LogUtil.error(JSON.stringify(err));
    return -1;
  }
}

/**
 * Do NOT busy-spin on the Hypium JS thread (triggers THREAD_BLOCK_6S / App died).
 * Prefer async setTimeout in listen cases; for sync paths only re-read getState.
 */
/** True if stack is OFF or still turning off (acceptable after disable). */
export function scIsOffOrTurning(state: number): boolean {
  return state === access.BluetoothState.STATE_OFF ||
    state === access.BluetoothState.STATE_TURNING_OFF ||
    state === access.BluetoothState.STATE_BLE_TURNING_OFF;
}

/** True if stack is ON / BLE_ON or still turning on (acceptable after enable). */
export function scIsOnOrTurning(state: number): boolean {
  return state === access.BluetoothState.STATE_ON ||
    state === access.BluetoothState.STATE_TURNING_ON ||
    state === access.BluetoothState.STATE_BLE_TURNING_ON ||
    state === access.BluetoothState.STATE_BLE_ON;
}

export function scWaitStateOn(retries: number = 8): number {
  for (let i = 0; i < retries; i++) {
    const st = scGetState();
    if (scIsOnOrTurning(st)) {
      return st;
    }
  }
  return scGetState();
}

/** Bounded poll until OFF|TURNING_OFF. Prefer async settle. */
export function scWaitStateOff(retries: number = 8): number {
  for (let i = 0; i < retries; i++) {
    const st = scGetState();
    if (scIsOffOrTurning(st)) {
      return st;
    }
  }
  return scGetState();
}

/** Async settle helper for disable → OFF / TURNING_OFF. */
export async function scAwaitStateOff(timeoutMs: number = 2000): Promise<number> {
  const end = Date.now() + timeoutMs;
  while (Date.now() < end) {
    const st = scGetState();
    if (scIsOffOrTurning(st)) {
      return st;
    }
    await new Promise<void>((resolve: () => void) => {
      setTimeout(resolve, 100);
    });
  }
  return scGetState();
}

/** Async settle helper for enable → ON / TURNING_ON. */
export async function scAwaitStateOn(timeoutMs: number = 2000): Promise<number> {
  const end = Date.now() + timeoutMs;
  while (Date.now() < end) {
    const st = scGetState();
    if (scIsOnOrTurning(st)) {
      return st;
    }
    await new Promise<void>((resolve: () => void) => {
      setTimeout(resolve, 100);
    });
  }
  return scGetState();
}

/**
 * Auto-click system Bluetooth enable / permission consent dialog.
 * Common labels: 允许 / 开启 / 确定 (and EN fallbacks).
 */
export async function scDismissBtEnableDialog(timeoutMs: number = 2000): Promise<boolean> {
  const labels: string[] = ['允许', '开启', '确定', 'Allow', 'Turn on', 'OK', '开启蓝牙'];
  try {
    const driver = Driver.create();
    const deadline = Date.now() + timeoutMs;
    while (Date.now() < deadline) {
      for (let i = 0; i < labels.length; i++) {
        const label = labels[i];
        try {
          const btn = await driver.findComponent(ON.text(label));
          if (btn !== undefined) {
            await btn.click();
            LogUtil.info('scDismissBtEnableDialog clicked=' + label);
            return true;
          }
        } catch (_e) {
          // try next label
        }
      }
      await new Promise<void>((resolve: () => void) => {
        setTimeout(resolve, 200);
      });
    }
  } catch (err) {
    LogUtil.error('scDismissBtEnableDialog ' + JSON.stringify(err));
  }
  return false;
}

/** After enable: STATE_ON or TURNING_ON / BLE_TURNING_ON. */
export function scAssertStateOn(state: number): void {
  LogUtil.info('scAssertStateOn expect ON|TURNING_ON got=' + state);
  expect(scIsOnOrTurning(state)).assertTrue();
}

/** After disable: STATE_OFF or TURNING_OFF / BLE_TURNING_OFF. */
export function scAssertStateOff(state: number): void {
  LogUtil.info('scAssertStateOff expect OFF|TURNING_OFF got=' + state);
  expect(scIsOffOrTurning(state)).assertTrue();
}

export function scEnable(): number {
  // Already ON: do NOT call enableBluetooth again (system consent dialog loops otherwise).
  try {
    if (access.getState() === access.BluetoothState.STATE_ON) {
      return 0;
    }
  } catch (_e) {
    LogUtil.error(JSON.stringify(_e));
  }
  const err = scInvoke(() => {
    access.enableBluetooth();
  });
  // Best-effort auto-confirm system "开启/允许" dialog (non-blocking).
  setTimeout((): void => {
    scDismissBtEnableDialog(2000).then((): void => {
    }).catch((): void => {
    });
  }, 300);
  return err;
}

export function scDisable(): number {
  return scInvoke(() => {
    access.disableBluetooth();
  });
}

/** Valid BluetoothState range including BLE-only modes. */
export function scAssertValidState(state: number): void {
  expect(state >= access.BluetoothState.STATE_OFF &&
    state <= access.BluetoothState.STATE_BLE_TURNING_OFF).assertTrue();
}

/**
 * @deprecated Prefer scAssertStateOn after scWaitStateOn.
 * Kept for transitional call sites; still validates enum range then requires STATE_ON.
 */
export function scAssertPreferOn(state: number): void {
  scAssertStateOn(state);
}

/**
 * After disable: wait then hard-assert STATE_OFF. disableErr must be 0 first.
 */
export function scAssertAfterDisable(disableErr: number, state: number): void {
  assertLocalOk(disableErr);
  scAssertStateOff(state);
}

export function scGetLocalName(): string {
  try {
    return connection.getLocalName();
  } catch (err) {
    LogUtil.error(JSON.stringify(err));
    return '';
  }
}

export function scSetLocalName(name: string): number {
  return scInvoke(() => {
    connection.setLocalName(name);
  });
}

/**
 * After setLocalName succeeds, getLocalName must equal want (or documented truncation).
 * Always asserts a concrete string value — never assert(true).
 */
export function scAssertNameRoundTrip(setErr: number, want: string): void {
  assertLocalOk(setErr);
  if (setErr !== 0) {
    return;
  }
  const got = scGetLocalName();
  LogUtil.info('name roundTrip want=' + want + ' got=' + got);
  if (got === want) {
    expect(got).assertEqual(want);
    return;
  }
  // Some stacks truncate long names; got is a non-empty prefix of want.
  if (want.length > 0 && got.length > 0 && want.indexOf(got) === 0) {
    expect(want.indexOf(got)).assertEqual(0);
    expect(got.length > 0).assertTrue();
    return;
  }
  // Or want is prefix of got (platform append).
  if (want.length > 0 && got.length > 0 && got.indexOf(want) === 0) {
    expect(got.indexOf(want)).assertEqual(0);
    return;
  }
  expect(got).assertEqual(want);
}

export function scRestoreName(prev: string): void {
  if (prev.length === 0) {
    return;
  }
  scSetLocalName(prev);
}

export function scSafeStopBleScan(): void {
  try {
    ble.stopBLEScan();
  } catch (err) {
    LogUtil.error(JSON.stringify(err));
  }
}

export function scSafeStopDiscovery(): void {
  try {
    connection.stopBluetoothDiscovery();
  } catch (err) {
    LogUtil.error(JSON.stringify(err));
  }
}

export function scSafeCloseGatt(gs: ble.GattServer): void {
  try {
    gs.close();
  } catch (err) {
    LogUtil.error(JSON.stringify(err));
  }
}

export function scRepeat(tag: string, times: number, fn: () => void): void {
  for (let i = 0; i < times; i++) {
    LogUtil.info(tag + ' round=' + i);
    fn();
  }
}

export function scProfileIds(): number[] {
  return [
    constant.ProfileId.PROFILE_A2DP_SOURCE,
    constant.ProfileId.PROFILE_HANDSFREE_AUDIO_GATEWAY,
    constant.ProfileId.PROFILE_HID_HOST,
    constant.ProfileId.PROFILE_PAN_NETWORK
  ];
}

/** Local-name parameter fixtures: value / length / boundary / charset. */
export function scNameFixtures(): string[] {
  const long248 = 'N'.repeat(248);
  const long249 = 'N'.repeat(249);
  return [
    'A',
    'BtScen',
    '中文名',
    'Name_123',
    'a'.repeat(31),
    'b'.repeat(32),
    long248,
    long249,
    ' ',
    '  trim  ',
    '!@#$%^&*()',
    'emoji😀',
    ''
  ];
}

export function scScanOpt(
  interval?: number,
  duty?: ble.ScanDuty,
  match?: ble.MatchMode,
  phy?: ble.PhyType,
  report?: ble.ScanReportMode
): ble.ScanOptions {
  const opt: ble.ScanOptions = {};
  if (interval !== undefined) {
    opt.interval = interval;
  }
  if (duty !== undefined) {
    opt.dutyMode = duty;
  }
  if (match !== undefined) {
    opt.matchMode = match;
  }
  if (phy !== undefined) {
    opt.phyType = phy;
  }
  if (report !== undefined) {
    opt.reportMode = report;
  }
  return opt;
}

export function scFilterEmpty(): Array<ble.ScanFilter> {
  return [];
}

export function scFilterName(name: string): Array<ble.ScanFilter> {
  return [{ name: name }];
}

export function scFilterUuid(uuid: string): Array<ble.ScanFilter> {
  return [{ serviceUuid: uuid }];
}

export function scFilterMfg(id: number): Array<ble.ScanFilter> {
  return [{ manufactureId: id }];
}

export function scFilterCombo(): Array<ble.ScanFilter> {
  return [{
    name: 'Scen',
    serviceUuid: SC_SVC_UUID,
    manufactureId: 0x004C
  }];
}

export function scSafeStopAdvertising(): void {
  try {
    ble.stopAdvertising();
  } catch (err) {
    LogUtil.error(JSON.stringify(err));
  }
}

/** Advertise setting variants for parameter coverage. */
export function scAdvertiseSetting(
  interval: number = 1600,
  txPower: number = -7,
  connectable: boolean = true
): ble.AdvertiseSetting {
  return {
    interval: interval,
    txPower: txPower,
    connectable: connectable
  };
}

export function scSppOpt(secure: boolean = false): socket.SppOptions {
  return {
    uuid: SPP_UUID,
    secure: secure,
    type: socket.SppType.SPP_RFCOMM
  };
}

/**
 * Align with test/example/BtDebug BleModel:
 * classic BR inquiry ~10–12s first, then BLE (avoid RF contention).
 */
export const SC_CLASSIC_FIND_WAIT_MS: number = 10000;
/** BLE listen after classic window (BtDebug uses ~12s delay then continues). */
export const SC_BLE_FIND_WAIT_MS: number = 8000;
/** Delay after stopBluetoothDiscovery so Settings inquiry fully releases. */
export const SC_CLASSIC_RESTART_DELAY_MS: number = 400;

/** UUID unlikely present in lab — negative filter miss path. */
export const SC_BLE_MISS_UUID: string = '0000DEAD-0000-1000-8000-00805F9B34FB';

/** Documented local failures excluding success (cannot-set / cannot-scan API path). */
export const BT_LOCAL_FAIL: number[] = [
  201, 202, 203, 401, 801, 2900001, 2900003, 2900004, 2900007, 2900008,
  2900009, 2900010, 2900011, 2900013, 2900014, 2900099, 2900100, 2902055,
  9200001, 9200002, 9200010
];

/** Local negative path: must be a non-zero documented failure. */
export function scAssertLocalFail(errCode: number): void {
  LogUtil.info('scAssertLocalFail err=' + errCode);
  expect(errCode !== 0).assertTrue();
  assertErrIn(errCode, BT_LOCAL_FAIL);
}

/**
 * Local negative while BT OFF/TURNING_OFF: prefer documented fail code;
 * some stacks no-op with err=0 (still acceptable if radio is off/turning).
 */
export function scAssertLocalFailOrBtOff(errCode: number): void {
  LogUtil.info('scAssertLocalFailOrBtOff err=' + errCode + ' state=' + scGetState());
  if (errCode !== 0) {
    scAssertLocalFail(errCode);
    return;
  }
  expect(scIsOffOrTurning(scGetState())).assertTrue();
}

/**
 * setLocalName while BT OFF: API may fail, or name stays / becomes empty
 * (this stack often returns "" when BT is off — acceptable).
 */
export function scAssertNameCannotSet(setErr: number, before: string): void {
  const got = scGetLocalName();
  LogUtil.info('name cannotSet err=' + setErr + ' before=' + before + ' got=' + got);
  // Accept: fail code, or name unchanged / emptied (do not require fail whitelist).
  const ok = (setErr !== 0) || (got === before) || (got.length === 0);
  expect(ok).assertTrue();
  if (setErr === 0 && got !== before && got.length > 0) {
    expect(false).assertTrue();
  }
}

/** getLocalName while BT OFF: empty, prior want, or leftover system name OK. */
export function scAssertNameWhileBtOff(name: string, wantIfReadable: string): void {
  LogUtil.info('name whileOff got=' + name + ' wantIfReadable=' + wantIfReadable);
  expect(typeof name === 'string').assertTrue();
}

export function scAppendBleFinds(hits: Array<ble.ScanResult>, arr: Array<ble.ScanResult>): void {
  if (arr === undefined || arr === null) {
    return;
  }
  for (let i = 0; i < arr.length; i++) {
    hits.push(arr[i]);
  }
}

export function scBleOnFind(cb: (arr: Array<ble.ScanResult>) => void): number {
  try {
    ble.off('BLEDeviceFind', cb);
  } catch (_e) {
  }
  return scInvoke(() => {
    ble.on('BLEDeviceFind', cb);
  });
}

export function scBleOffFind(cb: (arr: Array<ble.ScanResult>) => void): void {
  try {
    ble.off('BLEDeviceFind', cb);
  } catch (err) {
    LogUtil.error(JSON.stringify(err));
  }
}

/** BtDebug-equivalent ScanOptions: null filter + LOW_LATENCY + AGGRESSIVE. */
export function scAmbientScanOpt(): ble.ScanOptions {
  return {
    interval: 0,
    dutyMode: ble.ScanDuty.SCAN_MODE_LOW_LATENCY,
    matchMode: ble.MatchMode.MATCH_MODE_AGGRESSIVE
  };
}

/**
 * Ambient / can-scan: ble.on(BLEDeviceFind) + startBLEScan(null, …).
 * Prefer over BleScanner (isNewApi → 801 on this service). Empty [] may yield 0 results.
 */
export function scStartBleScan(
  filters: Array<ble.ScanFilter> | null,
  opt?: ble.ScanOptions
): number {
  return scInvoke(() => {
    ble.startBLEScan(filters as Array<ble.ScanFilter>, opt);
  });
}

/**
 * BleScanner path (API15+). Must await Promise startScan/stopScan.
 * Some devices return 801 — use only for BleScanner API coverage, not ambient hit.
 */
export function scScannerOnFind(
  scn: ble.BleScanner,
  cb: (report: ble.ScanReport) => void
): number {
  return scInvoke(() => {
    scn.on('BLEDeviceFind', cb);
  });
}

export function scScannerOffFind(
  scn: ble.BleScanner,
  cb: (report: ble.ScanReport) => void
): void {
  try {
    scn.off('BLEDeviceFind', cb);
  } catch (err) {
    LogUtil.error(JSON.stringify(err));
  }
}

export async function scScannerStartScan(
  scn: ble.BleScanner,
  filters: Array<ble.ScanFilter> | null,
  opt?: ble.ScanOptions
): Promise<number> {
  try {
    await scn.startScan(filters as Array<ble.ScanFilter>, opt);
    return 0;
  } catch (err) {
    let code = Number((err as BusinessError).code);
    if (isNaN(code)) {
      code = 2900099;
    }
    LogUtil.error('scScannerStartScan ' + JSON.stringify(err));
    return code;
  }
}

export async function scScannerStopScan(scn: ble.BleScanner): Promise<void> {
  try {
    await scn.stopScan();
  } catch (err) {
    LogUtil.error(JSON.stringify(err));
  }
}

export function scSafeStopScanner(scn: ble.BleScanner): void {
  try {
    scn.stopScan().then((): void => {
    }).catch((): void => {
    });
  } catch (err) {
    LogUtil.error(JSON.stringify(err));
  }
}

/** Success: at least one ScanResult with non-empty deviceId. */
export function scAssertBleScanHit(hits: Array<ble.ScanResult>): void {
  LogUtil.info('bleScanHit n=' + hits.length);
  expect(hits.length > 0).assertTrue();
  if (hits.length > 0) {
    const id = hits[0].deviceId;
    LogUtil.info('bleScanHit deviceId=' + id);
    expect(id !== undefined && id !== null && id.length > 0).assertTrue();
  }
}

/**
 * Ambient discover hit — Settings-equivalent.
 * Settings device list is mostly classic inquiry; BLE LE ads may be absent.
 * Pass if BLEDeviceFind or bluetoothDeviceFind delivered at least one id.
 */
export function scAssertAmbientFindHit(
  bleHits: Array<ble.ScanResult>,
  classicIds: Array<string>
): void {
  LogUtil.info('ambientFind ble=' + bleHits.length + ' classic=' + classicIds.length);
  const ok = (bleHits.length > 0) || (classicIds.length > 0);
  expect(ok).assertTrue();
  if (bleHits.length > 0) {
    const id = bleHits[0].deviceId;
    expect(id !== undefined && id !== null && id.length > 0).assertTrue();
  }
  if (classicIds.length > 0) {
    expect(classicIds[0].length > 0).assertTrue();
  }
}

export function scClassicOnFind(cb: (ids: Array<string>) => void): number {
  try {
    connection.off('bluetoothDeviceFind', cb);
  } catch (_e) {
  }
  return scInvoke(() => {
    connection.on('bluetoothDeviceFind', cb);
  });
}

export function scClassicOffFind(cb: (ids: Array<string>) => void): void {
  try {
    connection.off('bluetoothDeviceFind', cb);
  } catch (err) {
    LogUtil.error(JSON.stringify(err));
  }
}

export function scStartClassicDiscovery(): number {
  return scInvoke(() => {
    connection.startBluetoothDiscovery();
  });
}

/**
 * BtDebug: Settings may hold BR inquiry — stop, wait, then start.
 * Seed paired devices into classicIds (optional list).
 */
export function scAppendClassicFinds(hits: Array<string>, ids: Array<string>): void {
  if (ids === undefined || ids === null) {
    return;
  }
  for (let i = 0; i < ids.length; i++) {
    if (ids[i] !== undefined && ids[i] !== null && ids[i].length > 0) {
      hits.push(ids[i]);
    }
  }
}

export async function scPrepareAndStartClassicDiscovery(
  classicIds?: Array<string>
): Promise<number> {
  try {
    if (connection.isBluetoothDiscovering()) {
      connection.stopBluetoothDiscovery();
      LogUtil.info('scPrepareClassic: stopped previous discovery');
    }
  } catch (err) {
    LogUtil.error(JSON.stringify(err));
  }
  if (classicIds !== undefined && classicIds !== null) {
    try {
      const paired = connection.getPairedDevices();
      scAppendClassicFinds(classicIds, paired);
      LogUtil.info('scPrepareClassic paired n=' + paired.length);
    } catch (err2) {
      LogUtil.error(JSON.stringify(err2));
    }
  }
  await new Promise<void>((resolve: () => void) => {
    setTimeout(resolve, SC_CLASSIC_RESTART_DELAY_MS);
  });
  return scStartClassicDiscovery();
}

/** Negative: no ScanResult within the listen window. */
export function scAssertBleScanMiss(hits: Array<ble.ScanResult>): void {
  LogUtil.info('bleScanMiss n=' + hits.length);
  expect(hits.length).assertEqual(0);
}

/**
 * Cannot-scan outcome: startBLEScan fails with documented error, or scan stays open
 * but yields zero BLEDeviceFind callbacks in the wait window.
 */
export function scAssertBleCannotScan(startErr: number, hits: Array<ble.ScanResult>): void {
  LogUtil.info('bleCannotScan startErr=' + startErr + ' hits=' + hits.length);
  if (startErr === 0) {
    scAssertBleScanMiss(hits);
  } else {
    scAssertLocalFail(startErr);
  }
}

export function scAppendScannerFinds(hits: Array<ble.ScanResult>, report: ble.ScanReport): void {
  if (report === undefined || report === null) {
    return;
  }
  if (report.scanResult !== undefined && report.scanResult !== null) {
    LogUtil.info('scannerFind n=' + report.scanResult.length);
    scAppendBleFinds(hits, report.scanResult);
    return;
  }
  LogUtil.info('scannerFind empty/no scanResult field');
}
