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
  testOnStateChangeManager,
  testEnableBluetoothManager,
  testGetStateManager,
  testSetLocalNameManager,
  testGetLocalNameManager,
  testSetBluetoothScanModeManager,
  testGetBluetoothScanModeManager,
  testStartBluetoothDiscoveryManager,
  testStopBluetoothDiscoveryManager,
  testDisableBluetoothManager,
  testGetBtConnectionStateManager,
  testOnPinRequiredManager,
  testPairDeviceManager
} from './brManagerInterface'
import {
  testGetConnectedBLEDevicesManager,
  testStartBLEScanManager,
  testStopBLEScanManager,
  testOnBLEDeviceFindManager
} from './bleManagerInterface'
import {
  testCreateGattClientManager,
  testConnectManager,
  testDisconnectManager,
  testOnBLEConnectionStateChangeManager,
  testGattClientCloseManager,
  testGetServicesCallbackManager,
  testReadCharacteristicValueCallbackManager,
  testReadDescriptorValueCallbackManager,
  testWriteCharacteristicValueManager,
  testWriteDescriptorValueManager,
  testOnBLECharacteristicChangeManager,
  testSetNotifyCharacteristicChangedManager,
  testGetDeviceNameCallbackManager,
  testGetRssiValueCallbackManager
} from './gattClientManagerInterface'
import {
  testCreateGattServerManager,
  testStartAdvertisingManager,
  testStopAdvertisingManager,
  testAddServiceManager,
  testRemoveServiceManager,
  testGattServerCloseManager,
  testOnConnectStateChangeManager,
  testOnCharacteristicReadManager,
  testOnCharacteristicWriteManager,
  testOnDescriptorReadManager,
  testOnDescriptorWriteManager
} from './gattServerManagerInterface'
import {
  testSppClientConnectSafeManager,
  testSppClientWriteManager,
  testSppClientReadOnManager,
  testSppClientReadOffManager,
  testSppClientCloseManager
} from './sppClientManagerInterface'
import {
  testSppServerCreateSafeManager,
  testSppServerAcceptManager,
  testSppServerWriteManager,
  testSppServerReadOnManager,
  testSppServerReadOffManager,
  testSppCloseServerSocketManager,
  testSppCloseClientSocketManager
} from './sppServerManagerInterface'

interface BenchmarkManagerTest {
  name: Resource | string;
  detail: Resource | string;
  method: string;
  api: Function;
  result: string;
  category: BenchmarkCategory;
}

export const BrManagerBenchmarkTestCase: BenchmarkManagerTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_zcztbhm'),
    detail: '1',
    method: 'on.stateChangeManager',
    api: testOnStateChangeManager,
    result: 'None',
    category: BenchmarkCategory.BrBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_klym'),
    detail: '2',
    method: 'enableBluetoothManager',
    api: testEnableBluetoothManager,
    result: 'None',
    category: BenchmarkCategory.BrBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqztm'),
    detail: '3',
    method: 'getStateManager',
    api: testGetStateManager,
    result: 'None',
    category: BenchmarkCategory.BrBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_szbdmcm'),
    detail: '4',
    method: 'setLocalNameManager',
    api: testSetLocalNameManager,
    result: 'None',
    category: BenchmarkCategory.BrBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqbdmcm'),
    detail: '5',
    method: 'getLocalNameManager',
    api: testGetLocalNameManager,
    result: 'None',
    category: BenchmarkCategory.BrBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_szsmmsm'),
    detail: '6',
    method: 'setBluetoothScanModeManager',
    api: testSetBluetoothScanModeManager,
    result: 'None',
    category: BenchmarkCategory.BrBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqsmmsm'),
    detail: '7',
    method: 'getBluetoothScanModeManager',
    api: testGetBluetoothScanModeManager,
    result: 'None',
    category: BenchmarkCategory.BrBenchmarkManager
},
]

export const BleManagerBenchmarkTestCase: BenchmarkManagerTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_zcztbhm'),
    detail: '1',
    method: 'on.stateChangeManager',
    api: testOnStateChangeManager,
    result: 'None',
    category: BenchmarkCategory.BleBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_klym'),
    detail: '2',
    method: 'enableBluetoothManager',
    api: testEnableBluetoothManager,
    result: 'None',
    category: BenchmarkCategory.BleBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqztm'),
    detail: '3',
    method: 'getStateManager',
    api: testGetStateManager,
    result: 'None',
    category: BenchmarkCategory.BleBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_szbdmcm'),
    detail: '5',
    method: 'setLocalNameManager',
    api: testSetLocalNameManager,
    result: 'None',
    category: BenchmarkCategory.BleBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqbdmcm'),
    detail: '6',
    method: 'getLocalNameManager',
    api: testGetLocalNameManager,
    result: 'None',
    category: BenchmarkCategory.BleBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_szsmmsm'),
    detail: '7',
    method: 'setBluetoothScanModeManager',
    api: testSetBluetoothScanModeManager,
    result: 'None',
    category: BenchmarkCategory.BleBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqsmmsm'),
    detail: '8',
    method: 'getBluetoothScanModeManager',
    api: testGetBluetoothScanModeManager,
    result: 'None',
    category: BenchmarkCategory.BleBenchmarkManager
},
]

export const GattClientManagerBenchmarkTestCase: BenchmarkManagerTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_cjgkhdm'),
    detail: '4',
    method: 'createGattClientManager',
    api: testCreateGattClientManager,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_bljztm_1'),
    detail: '5',
    method: 'on.BleConnectionStateChangeManager',
    api: testOnBLEConnectionStateChangeManager,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_ljm'),
    detail: '6',
    method: 'connectManager',
    api: testConnectManager,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_sztztxggm'),
    detail: '7',
    method: 'setNotifyCharacteristicChangedManager',
    api: testSetNotifyCharacteristicChangedManager,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_btzzbhm_1'),
    detail: '8',
    method: 'on.BleCharacteristicChangeManager',
    api: testOnBLECharacteristicChangeManager,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqfwc'),
    detail: '9',
    method: 'getServices(Callback)Manager',
    api: testGetServicesCallbackManager,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_dqtzzc'),
    detail: '10',
    method: 'readCharacteristicValue(callback)Manager',
    api: testReadCharacteristicValueCallbackManager,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_xrtzzm'),
    detail: '11',
    method: 'writeCharacteristicValueManager',
    api: testWriteCharacteristicValueManager,
    result: 'None',
    category: BenchmarkCategory.GattClientBenchmarkManager
},
]

export const GattServerManagerBenchmarkTestCase: BenchmarkManagerTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_cjgfwqm'),
    detail: '4',
    method: 'createGattServerManager',
    api: testCreateGattServerManager,
    result: 'None',
    category: BenchmarkCategory.GattServerBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_tjfwm'),
    detail: '5',
    method: 'addServiceManager',
    api: testAddServiceManager,
    result: 'None',
    category: BenchmarkCategory.GattServerBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_ljztm'),
    detail: '6',
    method: 'on.connectStateChangeManager',
    api: testOnConnectStateChangeManager,
    result: 'None',
    category: BenchmarkCategory.GattServerBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_ksgbm'),
    detail: '7',
    method: 'startAdvertisingManager',
    api: testStartAdvertisingManager,
    result: 'None',
    category: BenchmarkCategory.GattServerBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_dqtzzm'),
    detail: '8',
    method: 'on.characteristicReadManager',
    api: testOnCharacteristicReadManager,
    result: 'None',
    category: BenchmarkCategory.GattServerBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_xrtzzm'),
    detail: '9',
    method: 'on.characteristicWriteManager',
    api: testOnCharacteristicWriteManager,
    result: 'None',
    category: BenchmarkCategory.GattServerBenchmarkManager
},
]

export const SppClientManagerBenchmarkTestCase: BenchmarkManagerTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_zcztbhm'),
    detail: '1',
    method: 'on.stateChangeManager',
    api: testOnStateChangeManager,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_klym'),
    detail: '2',
    method: 'enableBluetoothManager',
    api: testEnableBluetoothManager,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqztm'),
    detail: '3',
    method: 'getStateManager',
    api: testGetStateManager,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_szbdmcm'),
    detail: '5',
    method: 'setLocalNameManager',
    api: testSetLocalNameManager,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqbdmcm'),
    detail: '6',
    method: 'getLocalNameManager',
    api: testGetLocalNameManager,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmarkManager
},

  {
    name: $r('app.string.bluetoothtest_str_szssmmsm'),
    detail: '4',
    method: 'setBluetoothScanModeManager',
    api: testSetBluetoothScanModeManager,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqsmmsm'),
    detail: '5',
    method: 'getBluetoothScanModeManager',
    api: testGetBluetoothScanModeManager,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_skhdljaqmsm'),
    detail: '6',
    method: 'sppConnectManager',
    api: testSppClientConnectSafeManager,
    result: 'None',
    category: BenchmarkCategory.SppClientBenchmarkManager
},
]

export const SppServerManagerBenchmarkTestCase: BenchmarkManagerTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_zcztbhm'),
    detail: '1',
    method: 'on.stateChangeManager',
    api: testOnStateChangeManager,
    result: 'None',
    category: BenchmarkCategory.SppServerBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_klym'),
    detail: '2',
    method: 'enableBluetoothManager',
    api: testEnableBluetoothManager,
    result: 'None',
    category: BenchmarkCategory.SppServerBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqztm'),
    detail: '3',
    method: 'getStateManager',
    api: testGetStateManager,
    result: 'None',
    category: BenchmarkCategory.SppServerBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_szbdmcm'),
    detail: '5',
    method: 'setLocalNameManager',
    api: testSetLocalNameManager,
    result: 'None',
    category: BenchmarkCategory.SppServerBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqbdmcm'),
    detail: '6',
    method: 'getLocalNameManager',
    api: testGetLocalNameManager,
    result: 'None',
    category: BenchmarkCategory.SppServerBenchmarkManager
},

  {
    name: $r('app.string.bluetoothtest_str_szssmmsm'),
    detail: '4',
    method: 'setBluetoothScanModeManager',
    api: testSetBluetoothScanModeManager,
    result: 'None',
    category: BenchmarkCategory.SppServerBenchmarkManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqsmmsm'),
    detail: '5',
    method: 'getBluetoothScanModeManager',
    api: testGetBluetoothScanModeManager,
    result: 'None',
    category: BenchmarkCategory.SppServerBenchmarkManager
},
]

export function initBrManagerBenchmarkData(): Array<TestBenchmark> {
  let TestBenchmarkArray: Array<TestBenchmark> = []
  BrManagerBenchmarkTestCase.forEach(item => {
    TestBenchmarkArray.push(new TestBenchmark(item.name, item.detail, item.method, item.api,
      item.result, item.category));
  })
  return TestBenchmarkArray;
}

export function initBleManagerBenchmarkData(): Array<TestBenchmark> {
  let TestBenchmarkArray: Array<TestBenchmark> = []
  BleManagerBenchmarkTestCase.forEach(item => {
    TestBenchmarkArray.push(new TestBenchmark(item.name, item.detail, item.method, item.api,
      item.result, item.category));
  })
  return TestBenchmarkArray;
}

export function initGattClientManagerBenchmarkData(): Array<TestBenchmark> {
  let TestBenchmarkArray: Array<TestBenchmark> = []
  GattClientManagerBenchmarkTestCase.forEach(item => {
    TestBenchmarkArray.push(new TestBenchmark(item.name, item.detail, item.method, item.api,
      item.result, item.category));
  })
  return TestBenchmarkArray;
}

export function initGattServerManagerBenchmarkData(): Array<TestBenchmark> {
  let TestBenchmarkArray: Array<TestBenchmark> = []
  GattServerManagerBenchmarkTestCase.forEach(item => {
    TestBenchmarkArray.push(new TestBenchmark(item.name, item.detail, item.method, item.api,
      item.result, item.category));
  })
  return TestBenchmarkArray;
}

export function initSppClientManagerBenchmarkData(): Array<TestBenchmark> {
  let TestBenchmarkArray: Array<TestBenchmark> = []
  SppClientManagerBenchmarkTestCase.forEach(item => {
    TestBenchmarkArray.push(new TestBenchmark(item.name, item.detail, item.method, item.api,
      item.result, item.category));
  })
  return TestBenchmarkArray;
}

export function initSppServerManagerBenchmarkData(): Array<TestBenchmark> {
  let TestBenchmarkArray: Array<TestBenchmark> = []
  SppServerManagerBenchmarkTestCase.forEach(item => {
    TestBenchmarkArray.push(new TestBenchmark(item.name, item.detail, item.method, item.api,
      item.result, item.category));
  })
  return TestBenchmarkArray;
}