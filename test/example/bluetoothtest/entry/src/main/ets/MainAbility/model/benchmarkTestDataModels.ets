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

import { TestBenchmark, BenchmarkCategory} from './testData'
import {
  testOnStateChange,
  testEnableBluetooth,
  testGetState,
  testSetLocalName,
  testGetLocalName,
  testSetBluetoothScanMode,
  testGetBluetoothScanMode
} from './brInterface'
import {
  testCreateGattClient,
  testConnect,
  testOnBLEConnectionStateChange,
  testGetServicesCallback,
  testReadCharacteristicValueCallback,
  testWriteCharacteristicValue,
  testOnBLECharacteristicChange,
  testSetNotifyCharacteristicChanged
} from './gattClientInterface'
import {
  testCreateGattServer,
  testStartAdvertising,
  testAddService,
  testOnConnectStateChange,
  testOnCharacteristicRead,
  testOnCharacteristicWrite
} from './gattServerInterface'
import { testSppClientConnectSafe} from './sppClientInterface'

interface BenchmarkTest {
  name: Resource | string;
  detail: Resource | string;
  method: string;
  api: Function;
  result: string;
  category: BenchmarkCategory;
}

export const BrBenchmarkTestCase: BenchmarkTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_zcztbh'),
    detail: '1',
    method: 'on.stateChange',
    api: testOnStateChange,
    result: 'None',
    category: BenchmarkCategory.BrBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_kly'),
    detail: '2',
    method: 'enableBluetooth',
    api: testEnableBluetooth,
    result: 'None',
    category: BenchmarkCategory.BrBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_hqzt'),
    detail: '3',
    method: 'getState',
    api: testGetState,
    result: 'None',
    category: BenchmarkCategory.BrBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_szbdmc'),
    detail: '4',
    method: 'setLocalName',
    api: testSetLocalName,
    result: 'None',
    category: BenchmarkCategory.BrBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_hqbdmc'),
    detail: '5',
    method: 'getLocalName',
    api: testGetLocalName,
    result: 'None',
    category: BenchmarkCategory.BrBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_szsmms'),
    detail: '6',
    method: 'setBluetoothScanMode',
    api: testSetBluetoothScanMode,
    result: 'None',
    category: BenchmarkCategory.BrBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_hqsmms'),
    detail: '7',
    method: 'getBluetoothScanMode',
    api: testGetBluetoothScanMode,
    result: 'None',
    category: BenchmarkCategory.BrBenchmark
},
]

export const BleBenchmarkTestCase: BenchmarkTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_zcztbh'),
    detail: '1',
    method: 'on.stateChange',
    api: testOnStateChange,
    result: 'None',
    category: BenchmarkCategory.BleBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_kly'),
    detail: '2',
    method: 'enableBluetooth',
    api: testEnableBluetooth,
    result: 'None',
    category: BenchmarkCategory.BleBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_hqzt'),
    detail: '3',
    method: 'getState',
    api: testGetState,
    result: 'None',
    category: BenchmarkCategory.BleBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_szbdmc'),
    detail: '4',
    method: 'setLocalName',
    api: testSetLocalName,
    result: 'None',
    category: BenchmarkCategory.BleBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_hqbdmc'),
    detail: '5',
    method: 'getLocalName',
    api: testGetLocalName,
    result: 'None',
    category: BenchmarkCategory.BleBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_szsmms'),
    detail: '6',
    method: 'setBluetoothScanMode',
    api: testSetBluetoothScanMode,
    result: 'None',
    category: BenchmarkCategory.BleBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_hqsmms'),
    detail: '7',
    method: 'getBluetoothScanMode',
    api: testGetBluetoothScanMode,
    result: 'None',
    category: BenchmarkCategory.BleBenchmark
},

]

export const GattClientBenchmarkTestCase: BenchmarkTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_cjgkhd_1'),
    detail: '1',
    method: 'createGattClient',
    api: testCreateGattClient,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_bljzt_1'),
    detail: '2',
    method: 'on.BleConnectionStateChange',
    api: testOnBLEConnectionStateChange,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_lj'),
    detail: '3',
    method: 'connect',
    api: testConnect,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_sztztxgg'),
    detail: '4',
    method: 'setNotifyCharacteristicChanged',
    api: testSetNotifyCharacteristicChanged,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_btzzbh_1'),
    detail: '5',
    method: 'on.BleCharacteristicChange',
    api: testOnBLECharacteristicChange,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_hqfwc_1'),
    detail: '6',
    method: 'getServices(Callback)',
    api: testGetServicesCallback,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_dqtzzc_1'),
    detail: '7',
    method: 'readCharacteristicValue(callback)',
    api: testReadCharacteristicValueCallback,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_xrtzz'),
    detail: '8',
    method: 'writeCharacteristicValue',
    api: testWriteCharacteristicValue,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmark
},
]

export const GattServerBenchmarkTestCase: BenchmarkTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_cjgfwq_1'),
    detail: '1',
    method: 'createGattServer',
    api: testCreateGattServer,
    result: 'None',
    category: BenchmarkCategory.GattServerBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_tjfw'),
    detail: '2',
    method: 'addService',
    api: testAddService,
    result: 'None',
    category: BenchmarkCategory.GattServerBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_ljzt'),
    detail: '3',
    method: 'on.connectStateChange',
    api: testOnConnectStateChange,
    result: 'None',
    category: BenchmarkCategory.GattServerBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_ksgb'),
    detail: '4',
    method: 'startAdvertising',
    api: testStartAdvertising,
    result: 'None',
    category: BenchmarkCategory.GattServerBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_dqtzz'),
    detail: '5',
    method: 'on.characteristicRead',
    api: testOnCharacteristicRead,
    result: 'None',
    category: BenchmarkCategory.GattServerBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_xrtzz'),
    detail: '6',
    method: 'on.characteristicWrite',
    api: testOnCharacteristicWrite,
    result: 'None',
    category: BenchmarkCategory.GattServerBenchmark
},
]

export const SppClientBenchmarkTestCase: BenchmarkTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_zcztbh'),
    detail: '1',
    method: 'on.stateChange',
    api: testOnStateChange,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_kly'),
    detail: '2',
    method: 'enableBluetooth',
    api: testEnableBluetooth,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_hqzt'),
    detail: '3',
    method: 'getState',
    api: testGetState,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_szbdmc'),
    detail: '4',
    method: 'setLocalName',
    api: testSetLocalName,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_hqbdmc'),
    detail: '5',
    method: 'getLocalName',
    api: testGetLocalName,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_szssmms'),
    detail: '6',
    method: 'setBluetoothScanMode',
    api: testSetBluetoothScanMode,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_hqsmms'),
    detail: '7',
    method: 'getBluetoothScanMode',
    api: testGetBluetoothScanMode,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_skhdljaqms'),
    detail: '8',
    method: 'sppConnect',
    api: testSppClientConnectSafe,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmark
},
]

export const SppServerBenchmarkTestCase: BenchmarkTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_zcztbh'),
    detail: '1',
    method: 'on.stateChange',
    api: testOnStateChange,
    result: 'None',
    category: BenchmarkCategory.SppServerBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_kly'),
    detail: '2',
    method: 'enableBluetooth',
    api: testEnableBluetooth,
    result: 'None',
    category: BenchmarkCategory.SppServerBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_hqzt'),
    detail: '3',
    method: 'getState',
    api: testGetState,
    result: 'None',
    category: BenchmarkCategory.SppServerBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_szbdmc'),
    detail: '4',
    method: 'setLocalName',
    api: testSetLocalName,
    result: 'None',
    category: BenchmarkCategory.SppServerBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_hqbdmc'),
    detail: '5',
    method: 'getLocalName',
    api: testGetLocalName,
    result: 'None',
    category: BenchmarkCategory.SppServerBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_szssmms'),
    detail: '6',
    method: 'setBluetoothScanMode',
    api: testSetBluetoothScanMode,
    result: 'None',
    category: BenchmarkCategory.SppServerBenchmark
},
  {
    name: $r('app.string.bluetoothtest_str_hqsmms'),
    detail: '7',
    method: 'getBluetoothScanMode',
    api: testGetBluetoothScanMode,
    result: 'None',
    category: BenchmarkCategory.SppServerBenchmark
},
]

export function initBrBenchmarkData(): Array<TestBenchmark> {
  let TestBenchmarkArray: Array<TestBenchmark> = []
  BrBenchmarkTestCase.forEach(item => {
    TestBenchmarkArray.push(new TestBenchmark(item.name, item.detail, item.method, item.api,
      item.result, item.category));
  })
  return TestBenchmarkArray;
}

export function initBleBenchmarkData(): Array<TestBenchmark> {
  let TestBenchmarkArray: Array<TestBenchmark> = []
  BleBenchmarkTestCase.forEach(item => {
    TestBenchmarkArray.push(new TestBenchmark(item.name, item.detail, item.method, item.api,
      item.result, item.category));
  })
  return TestBenchmarkArray;
}

export function initGattClientBenchmarkData(): Array<TestBenchmark> {
  let TestBenchmarkArray: Array<TestBenchmark> = []
  GattClientBenchmarkTestCase.forEach(item => {
    TestBenchmarkArray.push(new TestBenchmark(item.name, item.detail, item.method, item.api,
      item.result, item.category));
  })
  return TestBenchmarkArray;
}

export function initGattServerBenchmarkData(): Array<TestBenchmark> {
  let TestBenchmarkArray: Array<TestBenchmark> = []
  GattServerBenchmarkTestCase.forEach(item => {
    TestBenchmarkArray.push(new TestBenchmark(item.name, item.detail, item.method, item.api,
      item.result, item.category));
  })
  return TestBenchmarkArray;
}

export function initSppClientBenchmarkData(): Array<TestBenchmark> {
  let TestBenchmarkArray: Array<TestBenchmark> = []
  SppClientBenchmarkTestCase.forEach(item => {
    TestBenchmarkArray.push(new TestBenchmark(item.name, item.detail, item.method, item.api,
      item.result, item.category));
  })
  return TestBenchmarkArray;
}

export function initSppServerBenchmarkData(): Array<TestBenchmark> {
  let TestBenchmarkArray: Array<TestBenchmark> = []
  SppServerBenchmarkTestCase.forEach(item => {
    TestBenchmarkArray.push(new TestBenchmark(item.name, item.detail, item.method, item.api,
      item.result, item.category));
  })
  return TestBenchmarkArray;
}