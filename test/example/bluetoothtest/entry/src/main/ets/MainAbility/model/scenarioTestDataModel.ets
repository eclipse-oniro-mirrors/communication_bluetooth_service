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

import { ScenarioCategory , TestScenario } from './testData'
import {
  subscribeBluetoothDeviceFind ,
  testDisableBluetooth ,
  testEnableBluetooth ,
  testGetBluetoothScanMode ,
  testGetBtConnectionState ,
  testGetLocalName ,
  testGetState ,
  testOnPinRequired ,
  testOnStateChange ,
  testPairDevice ,
  testSetBluetoothScanMode ,
  testSetLocalName ,
  testStopBluetoothDiscovery
} from './brInterface'
import { testGetConnectedBLEDevices , testOnBLEDeviceFind , testStartBLEScan , testStopBLEScan } from './bleInterface'
import {
  testConnect ,
  testCreateGattClient ,
  testDisconnect ,
  testGattClientClose ,
  testGetDeviceNameCallback ,
  testGetRssiValueCallback ,
  testGetServicesCallback ,
  testOnBLECharacteristicChange ,
  testOnBLEConnectionStateChange ,
  testReadCharacteristicValueCallback ,
  testReadDescriptorValueCallback ,
  testSetNotifyCharacteristicChanged ,
  testWriteCharacteristicValue ,
  testWriteDescriptorValue
} from './gattClientInterface'
import {
  testAddService ,
  testCreateGattServer ,
  testGattServerClose ,
  testOnCharacteristicRead ,
  testOnCharacteristicWrite ,
  testOnConnectStateChange ,
  testOnDescriptorRead ,
  testOnDescriptorWrite ,
  testRemoveService ,
  testStartAdvertising ,
  testStopAdvertising
} from './gattServerInterface'
import {
  testSppClientClose ,
  testSppClientConnectSafe ,
  testSppClientReadOff ,
  testSppClientReadOn ,
  testSppClientWrite
} from './sppClientInterface'
import {
  testSppCloseClientSocket ,
  testSppCloseServerSocket ,
  testSppServerAccept ,
  testSppServerCreateSafe ,
  testSppServerReadOff ,
  testSppServerReadOn ,
  testSppServerWrite
} from './sppServerInterface'
import BluetoothModel from './BluetoothModel'

interface ScenarioTest {
  name: string;
  detail: string;
  method: string;
  api: Function;
  result: string;
  category: ScenarioCategory;
}

export const BrFoundScenarioTestCase: ScenarioTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_zcztbh') ,
    detail : '1' ,
    method : 'on.stateChange' ,
    api : testOnStateChange ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : '2' ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : '3' ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmc') ,
    detail : '4' ,
    method : 'setLocalName' ,
    api : testSetLocalName ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmc') ,
    detail : '5' ,
    method : 'getLocalName' ,
    api : testGetLocalName ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmms') ,
    detail : '6' ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmms') ,
    detail : '7' ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_dylyfx') ,
    detail : '8' ,
    method : 'subscribeBluetoothDeviceFind' ,
    api : subscribeBluetoothDeviceFind ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_kslyfx') ,
    detail : '9' ,
    method : 'startBluetoothDiscovery' ,
    api : BluetoothModel.startBluetoothDiscovery ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenario
  },
]

export const BleFoundScenarioTestCase: ScenarioTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_zcztbh') ,
    detail : '1' ,
    method : 'on.stateChange' ,
    api : testOnStateChange ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : '2' ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : '3' ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmc') ,
    detail : '4' ,
    method : 'setLocalName' ,
    api : testSetLocalName ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmc') ,
    detail : '5' ,
    method : 'getLocalName' ,
    api : testGetLocalName ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmms') ,
    detail : '6' ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmms') ,
    detail : '7' ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_zcbczztbh_1') ,
    detail : '8' ,
    method : 'on.BleDeviceFind' ,
    api : testOnBLEDeviceFind ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_ksbsm_1') ,
    detail : '9' ,
    method : 'startBleScan' ,
    api : testStartBLEScan ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljdbsb_1') ,
    detail : '10' ,
    method : 'getConnectedBleDevices' ,
    api : testGetConnectedBLEDevices ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenario
  },
]

export const GattClientScenarioTestCase: ScenarioTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_zcztbh') ,
    detail : '1' ,
    method : 'on.stateChange' ,
    api : testOnStateChange ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : '2' ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : '3' ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_cjgkhd_1') ,
    detail : '4' ,
    method : 'createGattClient' ,
    api : testCreateGattClient ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_bljzt_1') ,
    detail : '5' ,
    method : 'on.BleConnectionStateChange' ,
    api : testOnBLEConnectionStateChange ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_lj') ,
    detail : '6' ,
    method : 'connect' ,
    api : testConnect ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_sztztxgg') ,
    detail : '7' ,
    method : 'setNotifyCharacteristicChanged' ,
    api : testSetNotifyCharacteristicChanged ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_btzzbh_1') ,
    detail : '8' ,
    method : 'on.BleCharacteristicChange' ,
    api : testOnBLECharacteristicChange ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqfwc_1') ,
    detail : '9' ,
    method : 'getServices(Callback)' ,
    api : testGetServicesCallback ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_dqtzzc_1') ,
    detail : '10' ,
    method : 'readCharacteristicValue(callback)' ,
    api : testReadCharacteristicValueCallback ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_dqmsfzc_1') ,
    detail : '11' ,
    method : 'readDescriptorValue(callback)' ,
    api : testReadDescriptorValueCallback ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbmcc_1') ,
    detail : '12' ,
    method : 'getDeviceName()callback' ,
    api : testGetDeviceNameCallback ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqrzc_1') ,
    detail : '13' ,
    method : 'getRssiValue()Callback' ,
    api : testGetRssiValueCallback ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_qxlj') ,
    detail : '14' ,
    method : 'disconnect' ,
    api : testDisconnect ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_gb') ,
    detail : '15' ,
    method : 'close' ,
    api : testGattClientClose ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_gly') ,
    detail : '16' ,
    method : 'disableBluetooth' ,
    api : testDisableBluetooth ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenario
  }
]

export const GattServerScenarioTestCase: ScenarioTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_zcztbh') ,
    detail : '1' ,
    method : 'on.stateChange' ,
    api : testOnStateChange ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : '2' ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : '3' ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_cjgfwq_1') ,
    detail : '4' ,
    method : 'createGattServer' ,
    api : testCreateGattServer ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_tjfw') ,
    detail : '5' ,
    method : 'addService' ,
    api : testAddService ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_ljzt') ,
    detail : '6' ,
    method : 'on.connectStateChange' ,
    api : testOnConnectStateChange ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_ksgb') ,
    detail : '7' ,
    method : 'startAdvertising' ,
    api : testStartAdvertising ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_dqtzz') ,
    detail : '8' ,
    method : 'on.characteristicRead' ,
    api : testOnCharacteristicRead ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_xrtzz') ,
    detail : '9' ,
    method : 'on.characteristicWrite' ,
    api : testOnCharacteristicWrite ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_dqmsf') ,
    detail : '9' ,
    method : 'on.descriptorRead' ,
    api : testOnDescriptorRead ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_xrmsf') ,
    detail : '11' ,
    method : 'on.descriptorWrite' ,
    api : testOnDescriptorWrite ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_tzgb') ,
    detail : '10' ,
    method : 'stopAdvertising' ,
    api : testStopAdvertising ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_scfw') ,
    detail : '11' ,
    method : 'removeService' ,
    api : testRemoveService ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_gb') ,
    detail : '12' ,
    method : 'close' ,
    api : testGattServerClose ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_gly') ,
    detail : '13' ,
    method : 'disableBluetooth' ,
    api : testDisableBluetooth ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenario
  }
]

export const SppClientScenarioTestCase: ScenarioTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_zcztbh') ,
    detail : '1' ,
    method : 'on.stateChange' ,
    api : testOnStateChange ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : '2' ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : '3' ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_szssmms') ,
    detail : '4' ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmms') ,
    detail : '5' ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdljaqms') ,
    detail : '6' ,
    method : 'sppConnect' ,
    api : testSppClientConnectSafe ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqdqljzt') ,
    detail : '7' ,
    method : 'getBtConnectionState' ,
    api : testGetBtConnectionState ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_skhddqdk') ,
    detail : '8' ,
    method : 'on.sppRead' ,
    api : testSppClientReadOn ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdxr') ,
    detail : '9' ,
    method : 'sppWrite' ,
    api : testSppClientWrite ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_skhddqgb') ,
    detail : '10' ,
    method : 'off.sppRead' ,
    api : testSppClientReadOff ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdgb') ,
    detail : '11' ,
    method : 'sppCloseClientSocket' ,
    api : testSppClientClose ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_gly') ,
    detail : '12' ,
    method : 'disableBluetooth' ,
    api : testDisableBluetooth ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenario
  },
]

export const SppServerScenarioTestCase: ScenarioTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_zcztbh') ,
    detail : '1' ,
    method : 'on.stateChange' ,
    api : testOnStateChange ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : '2' ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : '3' ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_szssmms') ,
    detail : '4' ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmms') ,
    detail : '5' ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_zcpmbh') ,
    detail : '6' ,
    method : 'on.pinRequired' ,
    api : testOnPinRequired ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqcjaqms') ,
    detail : '7' ,
    method : 'sppListen' ,
    api : testSppServerCreateSafe ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_hqdqljzt') ,
    detail : '8' ,
    method : 'getBtConnectionState' ,
    api : testGetBtConnectionState ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqxr') ,
    detail : '9' ,
    method : 'sppWrite' ,
    api : testSppServerWrite ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqjs') ,
    detail : '10' ,
    method : 'sppAccept' ,
    api : testSppServerAccept ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqdqdk') ,
    detail : '11' ,
    method : 'on.sppRead' ,
    api : testSppServerReadOn ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqdqgb') ,
    detail : '12' ,
    method : 'off.sppRead' ,
    api : testSppServerReadOff ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqgb') ,
    detail : '13' ,
    method : 'sppCloseServerSocket' ,
    api : testSppCloseServerSocket ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdgb') ,
    detail : '14' ,
    method : 'sppCloseClientSocket' ,
    api : testSppCloseClientSocket ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenario
  },
  {
    name : $r('app.string.bluetoothtest_str_gly') ,
    detail : '15' ,
    method : 'disableBluetooth' ,
    api : testDisableBluetooth ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenario
  },
]

export function initBRFoundScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  BrFoundScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestScenarioArray;
}

export function initBleFoundScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  BleFoundScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestScenarioArray;
}

export function initGattClientScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  GattClientScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestScenarioArray;
}

export function initGattServerScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  GattServerScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestScenarioArray;
}

export function initSppClientScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  SppClientScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestScenarioArray;
}

export function initSppServerScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  SppServerScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestScenarioArray;
}