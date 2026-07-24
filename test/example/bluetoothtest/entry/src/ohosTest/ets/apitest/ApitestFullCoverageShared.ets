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
 * Shared stubs for generated BluetoothFullCoverage*.test.ets suites.
 * Case bodies call APIs directly (no fcRun wrappers) with hilog + real assertions.
 */
import ble from '@ohos.bluetooth.ble';
import socket from '@ohos.bluetooth.socket';
import bluetooth from '@ohos.bluetooth';
import hid from '@ohos.bluetooth.hid';
import partnerAgent from '@ohos.FusionConnectivity.partnerAgent';
import common from '@ohos.bluetooth.common';
import { StartBLEScanOptions, StopBLEScanOptions, SubscribeBLEFoundOptions } from '@system.bluetooth';
import {
  BT_DOC_ERR_CODES,
  BT_LOCAL_OR_CAP,
  BT_PEER_EXPECT,
  assertErrIn,
  assertLocalOk,
  assertPeerExpect,
  assertThrownErrIn,
  ensureBtOn,
  runPerf,
  runStress,
  waitCallback
} from './ApitestRealAssert';

export {
  BT_DOC_ERR_CODES,
  BT_LOCAL_OR_CAP,
  BT_PEER_EXPECT,
  assertErrIn,
  assertLocalOk,
  assertPeerExpect,
  assertThrownErrIn,
  ensureBtOn,
  runPerf,
  runStress,
  waitCallback
};

export const BADDR = '00:11:22:33:44:55';
export const BAD_MAC = 'ZZ:ZZ:ZZ:ZZ:ZZ:ZZ';
export const EMPTY_MAC = '';
export const SVC_UUID = '0000180A-0000-1000-8000-00805F9B34FB';
export const CHAR_UUID = '00002A00-0000-1000-8000-00805F9B34FB';
export const SPP_UUID = '00001101-0000-1000-8000-00805F9B34FB';

export function buf0(): ArrayBuffer {
  return new ArrayBuffer(0);
}

export function sppOpt(): socket.SppOptions {
  return {
    uuid: SPP_UUID,
    secure: false,
    type: socket.SppType.SPP_RFCOMM
  };
}

export function legacySppOpt(): bluetooth.SppOption {
  return {
    uuid: SPP_UUID,
    secure: false,
    type: 0
  };
}

export function bleAd(): ble.AdvertiseData {
  return {
    serviceUuids: [SVC_UUID],
    manufactureData: [],
    serviceData: []
  };
}

export function bleAs(): ble.AdvertiseSetting {
  return { interval: 1600, txPower: -7, connectable: true };
}

export function bleAp(): ble.AdvertisingParams {
  return {
    advertisingSettings: bleAs(),
    advertisingData: bleAd()
  };
}

export function bleCharRef(): ble.BLECharacteristic {
  return {
    serviceUuid: SVC_UUID,
    characteristicUuid: CHAR_UUID,
    characteristicValue: buf0(),
    descriptors: []
  };
}

export function bleDescRef(): ble.BLEDescriptor {
  return {
    serviceUuid: SVC_UUID,
    characteristicUuid: CHAR_UUID,
    descriptorUuid: '00002902-0000-1000-8000-00805F9B34FB',
    descriptorValue: buf0()
  };
}

export function bleGattService(): ble.GattService {
  return {
    serviceUuid: SVC_UUID,
    isPrimary: true,
    characteristics: [{
      serviceUuid: SVC_UUID,
      characteristicUuid: CHAR_UUID,
      characteristicValue: buf0(),
      descriptors: []
    }],
    includeServices: []
  };
}

export function hidSdpStub(): hid.HidDeviceSdp {
  return {
    name: 'hypium',
    description: 'test',
    provider: 'test',
    subclass: hid.Subclass.SUBCLASS_UNCATEGORIZED,
    descriptors: new Uint8Array(0)
  };
}

export function hidQosStub(): hid.HidDeviceQos {
  return { serviceType: 1, tokenRate: 800, tokenBucketSize: 800, peakBandwidth: 0, latency: 0,
    delayVariation: 0 };
}

export function partnerDeviceAddr(): partnerAgent.PartnerDeviceAddress {
  const addr: common.BluetoothAddress = { address: BADDR, addressType: common.BluetoothAddressType.REAL };
  return { bluetoothAddress: addr };
}

export function partnerDeviceCap(): partnerAgent.DeviceCapability {
  return { supportBR: true, supportBleAdvertiser: false };
}

export function partnerBusinessCap(): partnerAgent.BusinessCapability {
  return { supportMediaControl: true, supportTelephonyControl: false };
}

/** @system.bluetooth StartBLEScanOptions stub. */
export function sysBleStartOpt(): StartBLEScanOptions {
  return { interval: 0, success: () => {}, fail: () => {}, complete: () => {} };
}

/** @system.bluetooth StopBLEScanOptions stub. */
export function sysBleStopOpt(): StopBLEScanOptions {
  return { success: () => {}, fail: () => {}, complete: () => {} };
}

/** @system.bluetooth SubscribeBLEFoundOptions stub. */
export function sysBleFoundOpt(): SubscribeBLEFoundOptions {
  return { success: () => {}, fail: () => {} };
}

export function blePhyValue(): ble.PhyValue {
  return { txPhy: ble.BlePhy.BLE_PHY_1M, rxPhy: ble.BlePhy.BLE_PHY_1M };
}

export function gattConnParam(): ble.ConnectionParam {
  return ble.ConnectionParam.LOW_POWER;
}

export function bleNotifyChar(): ble.NotifyCharacteristic {
  return {
    serviceUuid: SVC_UUID,
    characteristicUuid: CHAR_UUID,
    characteristicValue: buf0(),
    confirm: false
  };
}

export function gattServerResponse(): ble.ServerResponse {
  return { deviceId: BADDR, transId: 0, status: 0, offset: 0, value: buf0() };
}

/** @deprecated Prefer assertErrIn / assertLocalOk / assertPeerExpect (no open 2900xxx range). */
export function isAcceptableBtErrCode(errCode: number): boolean {
  for (let i = 0; i < BT_DOC_ERR_CODES.length; i++) {
    if (errCode === BT_DOC_ERR_CODES[i]) {
      return true;
    }
  }
  return false;
}
