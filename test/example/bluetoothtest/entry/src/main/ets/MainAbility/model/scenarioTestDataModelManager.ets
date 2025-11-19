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
  subscribeBluetoothDeviceFindManager ,
  testDisableBluetoothManager ,
  testEnableBluetoothManager ,
  testGetBluetoothScanModeManager ,
  testGetBtConnectionStateManager ,
  testGetLocalNameManager ,
  testGetStateManager ,
  testOnPinRequiredManager ,
  testOnStateChangeManager ,
  testPairDeviceManager ,
  testSetBluetoothScanModeManager ,
  testSetLocalNameManager ,
  testStopBluetoothDiscoveryManager
} from './brManagerInterface'
import {
  testGetConnectedBLEDevicesManager ,
  testOnBLEDeviceFindManager ,
  testStartBLEScanManager ,
  testStopBLEScanManager
} from './bleManagerInterface'
import {
  testConnectManager ,
  testCreateGattClientManager ,
  testDisconnectManager ,
  testGattClientCloseManager ,
  testGetDeviceNameCallbackManager ,
  testGetRssiValueCallbackManager ,
  testGetServicesCallbackManager ,
  testOnBLECharacteristicChangeManager ,
  testOnBLEConnectionStateChangeManager ,
  testReadCharacteristicValueCallbackManager ,
  testReadDescriptorValueCallbackManager ,
  testSetNotifyCharacteristicChangedManager ,
  testWriteCharacteristicValueManager ,
  testWriteDescriptorValueManager
} from './gattClientManagerInterface'
import {
  testAddServiceManager ,
  testCreateGattServerManager ,
  testGattServerCloseManager ,
  testOnCharacteristicReadManager ,
  testOnCharacteristicWriteManager ,
  testOnConnectStateChangeManager ,
  testOnDescriptorReadManager ,
  testOnDescriptorWriteManager ,
  testRemoveServiceManager ,
  testStartAdvertisingManager ,
  testStopAdvertisingManager
} from './gattServerManagerInterface'
import {
  testSppClientCloseManager ,
  testSppClientConnectSafeManager ,
  testSppClientReadOffManager ,
  testSppClientReadOnManager ,
  testSppClientWriteManager
} from './sppClientManagerInterface'
import {
  testSppCloseClientSocketManager ,
  testSppCloseServerSocketManager ,
  testSppServerAcceptManager ,
  testSppServerCreateSafeManager ,
  testSppServerReadOffManager ,
  testSppServerReadOnManager ,
  testSppServerWriteManager
} from './sppServerManagerInterface'
import BluetoothModelManager from './BluetoothModelManager'

interface ScenarioManagerTest {
  name: string;
  detail: string;
  method: string;
  api: Function;
  result: string;
  category: ScenarioCategory;
}

export const BrFoundManagerScenarioTestCase: ScenarioManagerTest[] = [
  {
    name : '注册状态变化Manager' ,
    detail : '1' ,
    method : 'on.stateChangeManager' ,
    api : testOnStateChangeManager ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenarioManager
  },
  {
    name : '开蓝牙Manager' ,
    detail : '2' ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenarioManager
  },
  {
    name : '获取状态Manager' ,
    detail : '3' ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenarioManager
  },
  {
    name : '设置本地名称Manager' ,
    detail : '4' ,
    method : 'setLocalNameManager' ,
    api : testSetLocalNameManager ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenarioManager
  },
  {
    name : '获取本地名称Manager' ,
    detail : '5' ,
    method : 'getLocalNameManager' ,
    api : testGetLocalNameManager ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenarioManager
  },
  {
    name : '设置扫描模式Manager' ,
    detail : '6' ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenarioManager
  },
  {
    name : '获取扫描模式Manager' ,
    detail : '7' ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenarioManager
  },
  {
    name : '订阅蓝牙发现Manager' ,
    detail : '8' ,
    method : 'subscribeBluetoothDeviceFindManager' ,
    api : subscribeBluetoothDeviceFindManager ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenarioManager
  },
  {
    name : '开始蓝牙发现Manager' ,
    detail : '9' ,
    method : 'startBluetoothDiscoveryManager' ,
    api : BluetoothModelManager.startBluetoothDiscovery ,
    result : 'None' ,
    category : ScenarioCategory.BrFoundScenarioManager
  },
]

export const BleFoundManagerScenarioTestCase: ScenarioManagerTest[] = [
  {
    name : '注册状态变化Manager' ,
    detail : '1' ,
    method : 'on.stateChangeManager' ,
    api : testOnStateChangeManager ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenarioManager
  },
  {
    name : '开蓝牙Manager' ,
    detail : '2' ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenarioManager
  },
  {
    name : '获取状态Manager' ,
    detail : '3' ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenarioManager
  },
  {
    name : '设置本地名称Manager' ,
    detail : '4' ,
    method : 'setLocalNameManager' ,
    api : testSetLocalNameManager ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenarioManager
  },
  {
    name : '获取本地名称Manager' ,
    detail : '5' ,
    method : 'getLocalNameManager' ,
    api : testGetLocalNameManager ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenarioManager
  },
  {
    name : '设置扫描模式Manager' ,
    detail : '6' ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenarioManager
  },
  {
    name : '获取扫描模式Manager' ,
    detail : '7' ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenarioManager
  },
  {
    name : '注册Ble查找状态变化Manager' ,
    detail : '8' ,
    method : 'on.BleDeviceFindManager' ,
    api : testOnBLEDeviceFindManager ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenarioManager
  },
  {
    name : '开始Ble扫描Manager' ,
    detail : '9' ,
    method : 'startBleScanManager' ,
    api : testStartBLEScanManager ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenarioManager
  },
  {
    name : '获取连接的Ble设备Manager' ,
    detail : '10' ,
    method : 'getConnectedBleDevicesManager' ,
    api : testGetConnectedBLEDevicesManager ,
    result : 'None' ,
    category : ScenarioCategory.BleFoundScenarioManager
  },
]

export const GattClientManagerScenarioTestCase: ScenarioManagerTest[] = [
  {
    name : '注册状态变化Manager' ,
    detail : '1' ,
    method : 'on.stateChangeManager' ,
    api : testOnStateChangeManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  },
  {
    name : '开蓝牙Manager' ,
    detail : '2' ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  },
  {
    name : '获取状态Manager' ,
    detail : '3' ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  },
  {
    name : '创建Gatt客户端Manager' ,
    detail : '4' ,
    method : 'createGattClientManager' ,
    api : testCreateGattClientManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  },
  {
    name : 'Ble连接状态Manager' ,
    detail : '5' ,
    method : 'on.BleConnectionStateChangeManager' ,
    api : testOnBLEConnectionStateChangeManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  },
  {
    name : '连接Manager' ,
    detail : '6' ,
    method : 'connectManager' ,
    api : testConnectManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  },
  {
    name : '设置通知特性更改Manager' ,
    detail : '7' ,
    method : 'setNotifyCharacteristicChangedManager' ,
    api : testSetNotifyCharacteristicChangedManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  },
  {
    name : 'Ble特征值变化Manager' ,
    detail : '8' ,
    method : 'on.BleCharacteristicChangeManager' ,
    api : testOnBLECharacteristicChangeManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  },
  {
    name : '获取服务callbackManager' ,
    detail : '9' ,
    method : 'getServices(Callback)Manager' ,
    api : testGetServicesCallbackManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  },
  {
    name : '读取特征值callbackManager' ,
    detail : '10' ,
    method : 'readCharacteristicValue(callback)Manager' ,
    api : testReadCharacteristicValueCallbackManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  },
  {
    name : '读取描述符值callbackManager' ,
    detail : '11' ,
    method : 'readDescriptorValue(callback)Manager' ,
    api : testReadDescriptorValueCallbackManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  },
  {
    name : '获取设备名称callbackManager' ,
    detail : '12' ,
    method : 'getDeviceName(callback)Manager' ,
    api : testGetDeviceNameCallbackManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  },
  {
    name : '获取Rssi值callbackManager' ,
    detail : '13' ,
    method : 'getRssiValue(Callback)Manager' ,
    api : testGetRssiValueCallbackManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  },
  {
    name : '取消连接Manager' ,
    detail : '14' ,
    method : 'disconnectManager' ,
    api : testDisconnectManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  },
  {
    name : '关闭Manager' ,
    detail : '15' ,
    method : 'closeManager' ,
    api : testGattClientCloseManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  },
  {
    name : '关蓝牙Manager' ,
    detail : '16' ,
    method : 'disableBluetoothManager' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : ScenarioCategory.GattClientScenarioManager
  }
]

export const GattServerManagerScenarioTestCase: ScenarioManagerTest[] = [
  {
    name : '注册状态变化Manager' ,
    detail : '1' ,
    method : 'on.stateChangeManager' ,
    api : testOnStateChangeManager ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenarioManager
  },
  {
    name : '开蓝牙Manager' ,
    detail : '2' ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenarioManager
  },
  {
    name : '获取状态Manager' ,
    detail : '3' ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenarioManager
  },
  {
    name : '创建Gatt服务器Manager' ,
    detail : '4' ,
    method : 'createGattServerManager' ,
    api : testCreateGattServerManager ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenarioManager
  },
  {
    name : '添加服务Manager' ,
    detail : '5' ,
    method : 'addServiceManager' ,
    api : testAddServiceManager ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenarioManager
  },
  {
    name : '连接状态Manager' ,
    detail : '6' ,
    method : 'on.connectStateChangeManager' ,
    api : testOnConnectStateChangeManager ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenarioManager
  },
  {
    name : '开始广播Manager' ,
    detail : '7' ,
    method : 'startAdvertisingManager' ,
    api : testStartAdvertisingManager ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenarioManager
  },
  {
    name : '读取特征值Manager' ,
    detail : '8' ,
    method : 'on.characteristicReadManager' ,
    api : testOnCharacteristicReadManager ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenarioManager
  },
  {
    name : '写入特征值Manager' ,
    detail : '9' ,
    method : 'on.characteristicWriteManager' ,
    api : testOnCharacteristicWriteManager ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenarioManager
  },
  {
    name : '读取描述符Manager' ,
    detail : '9' ,
    method : 'on.descriptorReadManager' ,
    api : testOnDescriptorReadManager ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenarioManager
  },
  {
    name : '写入描述符Manager' ,
    detail : '11' ,
    method : 'on.descriptorWriteManager' ,
    api : testOnDescriptorWriteManager ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenarioManager
  },
  {
    name : '停止广播Manager' ,
    detail : '10' ,
    method : 'stopAdvertisingManager' ,
    api : testStopAdvertisingManager ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenarioManager
  },
  {
    name : '删除服务Manager' ,
    detail : '11' ,
    method : 'removeServiceManager' ,
    api : testRemoveServiceManager ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenarioManager
  },
  {
    name : '关闭Manager' ,
    detail : '12' ,
    method : 'closeManager' ,
    api : testGattServerCloseManager ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenarioManager
  },
  {
    name : '关蓝牙Manager' ,
    detail : '13' ,
    method : 'disableBluetoothManager' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : ScenarioCategory.GattServerScenarioManager
  }
]

export const SppClientManagerScenarioTestCase: ScenarioManagerTest[] = [
  {
    name : '注册状态变化Manager' ,
    detail : '1' ,
    method : 'on.stateChangeManager' ,
    api : testOnStateChangeManager ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenarioManager
  },
  {
    name : '开蓝牙Manager' ,
    detail : '2' ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenarioManager
  },
  {
    name : '获取状态Manager' ,
    detail : '3' ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenarioManager
  },
  {
    name : '设置SPP扫描模式Manager' ,
    detail : '4' ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenarioManager
  },
  {
    name : '获取扫描模式Manager' ,
    detail : '5' ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenarioManager
  },
  {
    name : 'Spp客户端连接(安全模式)Manager' ,
    detail : '6' ,
    method : 'sppConnectManager' ,
    api : testSppClientConnectSafeManager ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenarioManager
  },
  {
    name : '获取当前连接状态Manager' ,
    detail : '7' ,
    method : 'getBtConnectionStateManager' ,
    api : testGetBtConnectionStateManager ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenarioManager
  },
  {
    name : 'Spp客户端读取打开Manager' ,
    detail : '8' ,
    method : 'on.sppReadManager' ,
    api : testSppClientReadOnManager ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenarioManager
  },
  {
    name : 'Spp客户端写入Manager' ,
    detail : '9' ,
    method : 'sppWriteManager' ,
    api : testSppClientWriteManager ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenarioManager
  },
  {
    name : 'Spp客户端读取关闭Manager' ,
    detail : '10' ,
    method : 'off.sppReadManager' ,
    api : testSppClientReadOffManager ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenarioManager
  },
  {
    name : 'Spp客户端关闭Manager' ,
    detail : '11' ,
    method : 'sppCloseClientSocketManager' ,
    api : testSppClientCloseManager ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenarioManager
  },
  {
    name : '关蓝牙Manager' ,
    detail : '12' ,
    method : 'disableBluetoothManager' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : ScenarioCategory.SppClientScenarioManager
  },
]

export const SppServerManagerScenarioTestCase: ScenarioManagerTest[] = [
  {
    name : '注册状态变化Manager' ,
    detail : '1' ,
    method : 'on.stateChangeManager' ,
    api : testOnStateChangeManager ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenarioManager
  },
  {
    name : '开蓝牙Manager' ,
    detail : '2' ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenarioManager
  },
  {
    name : '获取状态Manager' ,
    detail : '3' ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenarioManager
  },
  {
    name : '设置SPP扫描模式Manager' ,
    detail : '4' ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenarioManager
  },
  {
    name : '获取扫描模式Manager' ,
    detail : '5' ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenarioManager
  },
  {
    name : '注册pin码变化Manager' ,
    detail : '6' ,
    method : 'on.pinRequiredManager' ,
    api : testOnPinRequiredManager ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenarioManager
  },
  {
    name : 'Spp服务器创建(安全模式)Manager' ,
    detail : '7' ,
    method : 'sppListenManager' ,
    api : testSppServerCreateSafeManager ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenarioManager
  },
  {
    name : '获取当前连接状态Manager' ,
    detail : '8' ,
    method : 'getBtConnectionStateManager' ,
    api : testGetBtConnectionStateManager ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenarioManager
  },
  {
    name : 'Spp服务器写入Manager' ,
    detail : '9' ,
    method : 'sppWriteManager' ,
    api : testSppServerWriteManager ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenarioManager
  },
  {
    name : 'Spp服务器接受Manager' ,
    detail : '10' ,
    method : 'sppAcceptManager' ,
    api : testSppServerAcceptManager ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenarioManager
  },
  {
    name : 'Spp服务器读取打开Manager' ,
    detail : '11' ,
    method : 'on.sppReadManager' ,
    api : testSppServerReadOnManager ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenarioManager
  },
  {
    name : 'Spp服务器读取关闭Manager' ,
    detail : '12' ,
    method : 'off.sppReadManager' ,
    api : testSppServerReadOffManager ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenarioManager
  },
  {
    name : 'Spp服务器关闭Manager' ,
    detail : '13' ,
    method : 'sppCloseServerSocketManager' ,
    api : testSppCloseServerSocketManager ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenarioManager
  },
  {
    name : 'Spp客户端关闭Manager' ,
    detail : '14' ,
    method : 'sppCloseClientSocketManager' ,
    api : testSppCloseClientSocketManager ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenarioManager
  },
  {
    name : '关蓝牙Manager' ,
    detail : '15' ,
    method : 'disableBluetoothManager' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : ScenarioCategory.SppServerScenarioManager
  },
]

export function initBRFoundManagerScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  BrFoundManagerScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestScenarioArray;
}

export function initBleFoundManagerScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  BleFoundManagerScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestScenarioArray;
}

export function initGattClientManagerScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  GattClientManagerScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestScenarioArray;
}

export function initGattServerManagerScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  GattServerManagerScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestScenarioArray;
}

export function initSppClientManagerScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  SppClientManagerScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestScenarioArray;
}

export function initSppServerManagerScenarioData(): Array<TestScenario> {
  let TestScenarioArray: Array<TestScenario> = []
  SppServerManagerScenarioTestCase.forEach(item => {
    TestScenarioArray.push(new TestScenario(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestScenarioArray;
}