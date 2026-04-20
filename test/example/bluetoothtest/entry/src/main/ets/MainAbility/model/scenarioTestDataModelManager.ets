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

import { ScenarioCategory, TestScenario} from './testData'
import {
  subscribeBluetoothDeviceFindManager,
  testDisableBluetoothManager,
  testEnableBluetoothManager,
  testGetBluetoothScanModeManager,
  testGetBtConnectionStateManager,
  testGetLocalNameManager,
  testGetStateManager,
  testOnPinRequiredManager,
  testOnStateChangeManager,
  testPairDeviceManager,
  testSetBluetoothScanModeManager,
  testSetLocalNameManager,
  testStopBluetoothDiscoveryManager
} from './brManagerInterface'
import {
  testGetConnectedBLEDevicesManager,
  testOnBLEDeviceFindManager,
  testStartBLEScanManager,
  testStopBLEScanManager
} from './bleManagerInterface'
import {
  testConnectManager,
  testCreateGattClientManager,
  testDisconnectManager,
  testGattClientCloseManager,
  testGetDeviceNameCallbackManager,
  testGetRssiValueCallbackManager,
  testGetServicesCallbackManager,
  testOnBLECharacteristicChangeManager,
  testOnBLEConnectionStateChangeManager,
  testReadCharacteristicValueCallbackManager,
  testReadDescriptorValueCallbackManager,
  testSetNotifyCharacteristicChangedManager,
  testWriteCharacteristicValueManager,
  testWriteDescriptorValueManager
} from './gattClientManagerInterface'
import {
  testAddServiceManager,
  testCreateGattServerManager,
  testGattServerCloseManager,
  testOnCharacteristicReadManager,
  testOnCharacteristicWriteManager,
  testOnConnectStateChangeManager,
  testOnDescriptorReadManager,
  testOnDescriptorWriteManager,
  testRemoveServiceManager,
  testStartAdvertisingManager,
  testStopAdvertisingManager
} from './gattServerManagerInterface'
import {
  testSppClientCloseManager,
  testSppClientConnectSafeManager,
  testSppClientReadOffManager,
  testSppClientReadOnManager,
  testSppClientWriteManager
} from './sppClientManagerInterface'
import {
  testSppCloseClientSocketManager,
  testSppCloseServerSocketManager,
  testSppServerAcceptManager,
  testSppServerCreateSafeManager,
  testSppServerReadOffManager,
  testSppServerReadOnManager,
  testSppServerWriteManager
} from './sppServerManagerInterface'
import BluetoothModelManager from './BluetoothModelManager'

interface ScenarioManagerTest {
  name: Resource | string;
  detail: Resource | string;
  method: string;
  api: Function;
  result: string;
  category: ScenarioCategory;
}

export const BrFoundManagerScenarioTestCase: ScenarioManagerTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_zcztbhm'),
    detail: '1',
    method: 'on.stateChangeManager',
    api: testOnStateChangeManager,
    result: 'None',
    category: ScenarioCategory.BrFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_klym'),
    detail: '2',
    method: 'enableBluetoothManager',
    api: testEnableBluetoothManager,
    result: 'None',
    category: ScenarioCategory.BrFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqztm'),
    detail: '3',
    method: 'getStateManager',
    api: testGetStateManager,
    result: 'None',
    category: ScenarioCategory.BrFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_szbdmcm'),
    detail: '4',
    method: 'setLocalNameManager',
    api: testSetLocalNameManager,
    result: 'None',
    category: ScenarioCategory.BrFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqbdmcm'),
    detail: '5',
    method: 'getLocalNameManager',
    api: testGetLocalNameManager,
    result: 'None',
    category: ScenarioCategory.BrFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_szsmmsm'),
    detail: '6',
    method: 'setBluetoothScanModeManager',
    api: testSetBluetoothScanModeManager,
    result: 'None',
    category: ScenarioCategory.BrFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqsmmsm'),
    detail: '7',
    method: 'getBluetoothScanModeManager',
    api: testGetBluetoothScanModeManager,
    result: 'None',
    category: ScenarioCategory.BrFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_dylyfxm'),
    detail: '8',
    method: 'subscribeBluetoothDeviceFindManager',
    api: subscribeBluetoothDeviceFindManager,
    result: 'None',
    category: ScenarioCategory.BrFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_kslyfxm'),
    detail: '9',
    method: 'startBluetoothDiscoveryManager',
    api: BluetoothModelManager.startBluetoothDiscovery,
    result: 'None',
    category: ScenarioCategory.BrFoundScenarioManager
},
]

export const BleFoundManagerScenarioTestCase: ScenarioManagerTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_zcztbhm'),
    detail: '1',
    method: 'on.stateChangeManager',
    api: testOnStateChangeManager,
    result: 'None',
    category: ScenarioCategory.BleFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_klym'),
    detail: '2',
    method: 'enableBluetoothManager',
    api: testEnableBluetoothManager,
    result: 'None',
    category: ScenarioCategory.BleFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqztm'),
    detail: '3',
    method: 'getStateManager',
    api: testGetStateManager,
    result: 'None',
    category: ScenarioCategory.BleFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_szbdmcm'),
    detail: '4',
    method: 'setLocalNameManager',
    api: testSetLocalNameManager,
    result: 'None',
    category: ScenarioCategory.BleFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqbdmcm'),
    detail: '5',
    method: 'getLocalNameManager',
    api: testGetLocalNameManager,
    result: 'None',
    category: ScenarioCategory.BleFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_szsmmsm'),
    detail: '6',
    method: 'setBluetoothScanModeManager',
    api: testSetBluetoothScanModeManager,
    result: 'None',
    category: ScenarioCategory.BleFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqsmmsm'),
    detail: '7',
    method: 'getBluetoothScanModeManager',
    api: testGetBluetoothScanModeManager,
    result: 'None',
    category: ScenarioCategory.BleFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_zcbczztbhm_1'),
    detail: '8',
    method: 'on.BleDeviceFindManager',
    api: testOnBLEDeviceFindManager,
    result: 'None',
    category: ScenarioCategory.BleFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_ksbsmm_1'),
    detail: '9',
    method: 'startBleScanManager',
    api: testStartBLEScanManager,
    result: 'None',
    category: ScenarioCategory.BleFoundScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqljdbsbm_1'),
    detail: '10',
    method: 'getConnectedBleDevicesManager',
    api: testGetConnectedBLEDevicesManager,
    result: 'None',
    category: ScenarioCategory.BleFoundScenarioManager
},
]

export const GattClientManagerScenarioTestCase: ScenarioManagerTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_zcztbhm'),
    detail: '1',
    method: 'on.stateChangeManager',
    api: testOnStateChangeManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_klym'),
    detail: '2',
    method: 'enableBluetoothManager',
    api: testEnableBluetoothManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqztm'),
    detail: '3',
    method: 'getStateManager',
    api: testGetStateManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_cjgkhdm'),
    detail: '4',
    method: 'createGattClientManager',
    api: testCreateGattClientManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_bljztm_1'),
    detail: '5',
    method: 'on.BleConnectionStateChangeManager',
    api: testOnBLEConnectionStateChangeManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_ljm'),
    detail: '6',
    method: 'connectManager',
    api: testConnectManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_sztztxggm'),
    detail: '7',
    method: 'setNotifyCharacteristicChangedManager',
    api: testSetNotifyCharacteristicChangedManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_btzzbhm_1'),
    detail: '8',
    method: 'on.BleCharacteristicChangeManager',
    api: testOnBLECharacteristicChangeManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqfwc'),
    detail: '9',
    method: 'getServices(Callback)Manager',
    api: testGetServicesCallbackManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_dqtzzc'),
    detail: '10',
    method: 'readCharacteristicValue(callback)Manager',
    api: testReadCharacteristicValueCallbackManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_dqmsfzc'),
    detail: '11',
    method: 'readDescriptorValue(callback)Manager',
    api: testReadDescriptorValueCallbackManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqsbmcc'),
    detail: '12',
    method: 'getDeviceName(callback)Manager',
    api: testGetDeviceNameCallbackManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqrzc'),
    detail: '13',
    method: 'getRssiValue(Callback)Manager',
    api: testGetRssiValueCallbackManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_qxljm'),
    detail: '14',
    method: 'disconnectManager',
    api: testDisconnectManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_gbm'),
    detail: '15',
    method: 'closeManager',
    api: testGattClientCloseManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_glym'),
    detail: '16',
    method: 'disableBluetoothManager',
    api: testDisableBluetoothManager,
    result: 'None',
    category: ScenarioCategory.GattClientScenarioManager
  }
]

export const GattServerManagerScenarioTestCase: ScenarioManagerTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_zcztbhm'),
    detail: '1',
    method: 'on.stateChangeManager',
    api: testOnStateChangeManager,
    result: 'None',
    category: ScenarioCategory.GattServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_klym'),
    detail: '2',
    method: 'enableBluetoothManager',
    api: testEnableBluetoothManager,
    result: 'None',
    category: ScenarioCategory.GattServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqztm'),
    detail: '3',
    method: 'getStateManager',
    api: testGetStateManager,
    result: 'None',
    category: ScenarioCategory.GattServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_cjgfwqm'),
    detail: '4',
    method: 'createGattServerManager',
    api: testCreateGattServerManager,
    result: 'None',
    category: ScenarioCategory.GattServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_tjfwm'),
    detail: '5',
    method: 'addServiceManager',
    api: testAddServiceManager,
    result: 'None',
    category: ScenarioCategory.GattServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_ljztm'),
    detail: '6',
    method: 'on.connectStateChangeManager',
    api: testOnConnectStateChangeManager,
    result: 'None',
    category: ScenarioCategory.GattServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_ksgbm'),
    detail: '7',
    method: 'startAdvertisingManager',
    api: testStartAdvertisingManager,
    result: 'None',
    category: ScenarioCategory.GattServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_dqtzzm'),
    detail: '8',
    method: 'on.characteristicReadManager',
    api: testOnCharacteristicReadManager,
    result: 'None',
    category: ScenarioCategory.GattServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_xrtzzm'),
    detail: '9',
    method: 'on.characteristicWriteManager',
    api: testOnCharacteristicWriteManager,
    result: 'None',
    category: ScenarioCategory.GattServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_dqmsfm'),
    detail: '9',
    method: 'on.descriptorReadManager',
    api: testOnDescriptorReadManager,
    result: 'None',
    category: ScenarioCategory.GattServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_xrmsfm'),
    detail: '11',
    method: 'on.descriptorWriteManager',
    api: testOnDescriptorWriteManager,
    result: 'None',
    category: ScenarioCategory.GattServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_tzgbm'),
    detail: '10',
    method: 'stopAdvertisingManager',
    api: testStopAdvertisingManager,
    result: 'None',
    category: ScenarioCategory.GattServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_scfwm'),
    detail: '11',
    method: 'removeServiceManager',
    api: testRemoveServiceManager,
    result: 'None',
    category: ScenarioCategory.GattServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_gbm'),
    detail: '12',
    method: 'closeManager',
    api: testGattServerCloseManager,
    result: 'None',
    category: ScenarioCategory.GattServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_glym'),
    detail: '13',
    method: 'disableBluetoothManager',
    api: testDisableBluetoothManager,
    result: 'None',
    category: ScenarioCategory.GattServerScenarioManager
  }
]

export const SppClientManagerScenarioTestCase: ScenarioManagerTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_zcztbhm'),
    detail: '1',
    method: 'on.stateChangeManager',
    api: testOnStateChangeManager,
    result: 'None',
    category: ScenarioCategory.SppClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_klym'),
    detail: '2',
    method: 'enableBluetoothManager',
    api: testEnableBluetoothManager,
    result: 'None',
    category: ScenarioCategory.SppClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqztm'),
    detail: '3',
    method: 'getStateManager',
    api: testGetStateManager,
    result: 'None',
    category: ScenarioCategory.SppClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_szssmmsm'),
    detail: '4',
    method: 'setBluetoothScanModeManager',
    api: testSetBluetoothScanModeManager,
    result: 'None',
    category: ScenarioCategory.SppClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqsmmsm'),
    detail: '5',
    method: 'getBluetoothScanModeManager',
    api: testGetBluetoothScanModeManager,
    result: 'None',
    category: ScenarioCategory.SppClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_skhdljaqmsm'),
    detail: '6',
    method: 'sppConnectManager',
    api: testSppClientConnectSafeManager,
    result: 'None',
    category: ScenarioCategory.SppClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqdqljztm'),
    detail: '7',
    method: 'getBtConnectionStateManager',
    api: testGetBtConnectionStateManager,
    result: 'None',
    category: ScenarioCategory.SppClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_skhddqdkm'),
    detail: '8',
    method: 'on.sppReadManager',
    api: testSppClientReadOnManager,
    result: 'None',
    category: ScenarioCategory.SppClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_skhdxrm'),
    detail: '9',
    method: 'sppWriteManager',
    api: testSppClientWriteManager,
    result: 'None',
    category: ScenarioCategory.SppClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_skhddqgbm'),
    detail: '10',
    method: 'off.sppReadManager',
    api: testSppClientReadOffManager,
    result: 'None',
    category: ScenarioCategory.SppClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_skhdgbm'),
    detail: '11',
    method: 'sppCloseClientSocketManager',
    api: testSppClientCloseManager,
    result: 'None',
    category: ScenarioCategory.SppClientScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_glym'),
    detail: '12',
    method: 'disableBluetoothManager',
    api: testDisableBluetoothManager,
    result: 'None',
    category: ScenarioCategory.SppClientScenarioManager
},
]

export const SppServerManagerScenarioTestCase: ScenarioManagerTest[] = [
  {
    name: $r('app.string.bluetoothtest_str_zcztbhm'),
    detail: '1',
    method: 'on.stateChangeManager',
    api: testOnStateChangeManager,
    result: 'None',
    category: ScenarioCategory.SppServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_klym'),
    detail: '2',
    method: 'enableBluetoothManager',
    api: testEnableBluetoothManager,
    result: 'None',
    category: ScenarioCategory.SppServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqztm'),
    detail: '3',
    method: 'getStateManager',
    api: testGetStateManager,
    result: 'None',
    category: ScenarioCategory.SppServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_szssmmsm'),
    detail: '4',
    method: 'setBluetoothScanModeManager',
    api: testSetBluetoothScanModeManager,
    result: 'None',
    category: ScenarioCategory.SppServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqsmmsm'),
    detail: '5',
    method: 'getBluetoothScanModeManager',
    api: testGetBluetoothScanModeManager,
    result: 'None',
    category: ScenarioCategory.SppServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_zcpmbhm'),
    detail: '6',
    method: 'on.pinRequiredManager',
    api: testOnPinRequiredManager,
    result: 'None',
    category: ScenarioCategory.SppServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_sfwqcjaqmsm'),
    detail: '7',
    method: 'sppListenManager',
    api: testSppServerCreateSafeManager,
    result: 'None',
    category: ScenarioCategory.SppServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_hqdqljztm'),
    detail: '8',
    method: 'getBtConnectionStateManager',
    api: testGetBtConnectionStateManager,
    result: 'None',
    category: ScenarioCategory.SppServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_sfwqxrm'),
    detail: '9',
    method: 'sppWriteManager',
    api: testSppServerWriteManager,
    result: 'None',
    category: ScenarioCategory.SppServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_sfwqjsm'),
    detail: '10',
    method: 'sppAcceptManager',
    api: testSppServerAcceptManager,
    result: 'None',
    category: ScenarioCategory.SppServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_sfwqdqdkm'),
    detail: '11',
    method: 'on.sppReadManager',
    api: testSppServerReadOnManager,
    result: 'None',
    category: ScenarioCategory.SppServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_sfwqdqgbm'),
    detail: '12',
    method: 'off.sppReadManager',
    api: testSppServerReadOffManager,
    result: 'None',
    category: ScenarioCategory.SppServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_sfwqgbm'),
    detail: '13',
    method: 'sppCloseServerSocketManager',
    api: testSppCloseServerSocketManager,
    result: 'None',
    category: ScenarioCategory.SppServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_skhdgbm'),
    detail: '14',
    method: 'sppCloseClientSocketManager',
    api: testSppCloseClientSocketManager,
    result: 'None',
    category: ScenarioCategory.SppServerScenarioManager
},
  {
    name: $r('app.string.bluetoothtest_str_glym'),
    detail: '15',
    method: 'disableBluetoothManager',
    api: testDisableBluetoothManager,
    result: 'None',
    category: ScenarioCategory.SppServerScenarioManager
},
]

export function initBRFoundManagerScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  BrFoundManagerScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name, item.detail, item.method, item.api, item.result, item.category));
  })
  return TestScenarioArray;
}

export function initBleFoundManagerScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  BleFoundManagerScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name, item.detail, item.method, item.api, item.result, item.category));
  })
  return TestScenarioArray;
}

export function initGattClientManagerScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  GattClientManagerScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name, item.detail, item.method, item.api, item.result, item.category));
  })
  return TestScenarioArray;
}

export function initGattServerManagerScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  GattServerManagerScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name, item.detail, item.method, item.api, item.result, item.category));
  })
  return TestScenarioArray;
}

export function initSppClientManagerScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  SppClientManagerScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name, item.detail, item.method, item.api, item.result, item.category));
  })
  return TestScenarioArray;
}

export function initSppServerManagerScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  SppServerManagerScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name, item.detail, item.method, item.api, item.result, item.category));
  })
  return TestScenarioArray;
}