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

import router from '@ohos.router';
import { Category, TestData} from '../MainAbility/model/testData'
import bluetooth from '@ohos.bluetooth'

export { resStr} from './resString';

/**
 * GattServer Interface Of Bluetooth test
 */

export interface Descriptor {
  serviceUuid: string;
  characteristicUuid: string;
  descriptorUuid: string;
  descriptorValue: ArrayBufferLike;
}

export interface BleCharacteristicDataIn {
  serviceUuid: string;
  characteristicUuid: string;
  characteristicValue: ArrayBuffer;
  descriptors: Descriptor[];
}

export interface Characteristic {
  serviceUuid: string;
  characteristicUuid: string;
  characteristicValue: ArrayBufferLike;
  descriptors: Descriptor[];
}

export interface CurGattService {
  serviceUuid: string;
  isPrimary: boolean;
  characteristics: Characteristic[];
  includeServices: [];
}

export interface CurAdvSettings {
  interval: number;
  txPower: number;
  connectable: boolean;
}

export interface AdvDataManufactureDataInstance {
  manufactureId: number;
  manufactureValue: ArrayBufferLike;
}

export interface AdvDataServiceDataInstance {
  serviceUuid: string;
  serviceValue: ArrayBufferLike;
}

export interface CurAdvData {
  serviceUuids: string[];
  manufactureData: AdvDataManufactureDataInstance[];
  serviceData: AdvDataServiceDataInstance[];
}

export interface AdvResponseManufactureDataInstance {
  manufactureId: number;
  manufactureValue: ArrayBufferLike;
}

export interface AdvResponseServiceDataInstance {
  serviceUuid: string;
  serviceValue: ArrayBufferLike;
}

export interface CurAdvResponse {
  serviceUuids: string[];
  manufactureData: AdvResponseManufactureDataInstance[];
  serviceData: AdvResponseServiceDataInstance[];
}

export interface ServerResponse {
  deviceId: string;
  transId: number;
  status: number;
  offset: number;
  value: ArrayBufferLike;
}

export interface NotifyCharacteristic {
  serviceUuid: string;
  characteristicUuid: string;
  characteristicValue: ArrayBufferLike;
  confirm: boolean;
}

export interface BTAutoArrayItem {
  name: string;
  para: (string | number)[];
}

export interface BleScanFilter {
  deviceId?: string,
  name?: string,
  serviceUuid?: string
}

export interface RetObj {
  mod: number,
  duration: number
}

export interface CurScanOptions {
  interval: number;
  dutyMode: number;
  matchMode: number;
}

export interface myParams extends Object {
  testItem: TestData
}

/**
 * EntryAbility 经 testTargetPage / loadContent 直达子页时无 router 参数，避免读取 undefined 崩溃（UiTest 等）。
 */
export function testItemFromRouterOrPlaceholder(): TestData {
  const raw = router.getParams() as myParams | undefined;
  if (raw !== undefined && raw !== null && raw.testItem !== undefined && raw.testItem !== null){
    return raw.testItem;
  }
  return new TestData('UiTest', 'UiTest', '', $r('app.media.icon'), Category.All);
}

export interface myParamsManager extends Object {
  testItemManager: TestData
}

export interface PairDevices {
  id: string,
  name: string
}

export interface ScanMode {
  mode: bluetooth.ScanMode,
  duration: number
}

export interface ProfileList {
  value: string;
}

export interface ScanResult {
  deviceId?: string;
  rssi?: number;
  data?: ArrayBuffer;
}