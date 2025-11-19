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

import { CaseCategory , Category , TestApi , TestData } from './testData'
import {
  testDisableBluetoothManager ,
  testEnableBluetoothManager ,
  testGetBluetoothScanModeManager ,
  testGetBtConnectionStateManager ,
  testGetLocalNameManager ,
  testGetPairedDevicesManager ,
  testGetProfileConnStateManager1 ,
  testGetProfileConnStateManager2 ,
  testGetRemoteDeviceClassManager ,
  testGetRemoteDeviceNameManager ,
  testGetStateManager ,
  testOnBondStateChangeManager ,
  testOnPinRequiredManager ,
  testOnStateChangeManager ,
  testPairDeviceManager ,
  testSetBluetoothScanModeManager ,
  testSetLocalNameManager ,
  testStartBluetoothDiscoveryManager ,
  testStopBluetoothDiscoveryManager ,
  testUnPairedDeviceManager
} from './brManagerInterface'
import {
  testCreateGattClientDeviceManager ,
  testCreateGattServerDeviceManager ,
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
  testGetDeviceNamePromiseManager ,
  testGetRssiValueCallbackManager ,
  testGetRssiValuePromiseManager ,
  testGetServicesCallbackManager ,
  testGetServicesPromiseManager ,
  testOnBLECharacteristicChangeManager ,
  testOnBLEConnectionStateChangeManager ,
  testReadCharacteristicValueCallbackManager ,
  testReadCharacteristicValuePromiseManager ,
  testReadDescriptorValueCallbackManager ,
  testReadDescriptorValuePromiseManager ,
  testSetBLEMtuSizeManager ,
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
  testSppClientConnectUnSafeManager ,
  testSppClientReadOffManager ,
  testSppClientReadOnManager ,
  testSppClientWriteManager
} from './sppClientManagerInterface'
import {
  testSppCloseClientSocketManager ,
  testSppCloseServerSocketManager ,
  testSppServerAcceptManager ,
  testSppServerCreateSafeManager ,
  testSppServerCreateUnSafeManager ,
  testSppServerReadOffManager ,
  testSppServerReadOnManager ,
  testSppServerWriteManager
} from './sppServerManagerInterface'
import {
  testProfileConnectManager ,
  testProfileDisConnectManager ,
  testProfileGetConnectionDevicesManager ,
  testProfileGetDeviceStateManager ,
  testProfileGetPlayingStateManager
} from './a2dpSinkProfileManagerInterface'

interface BRManagerTest {
  name: string;
  detail: string;
  method: string;
  api: Function;
  result: string;
  category: CaseCategory;
}

interface BRManagerTestComposition {
  name: string;
  detail: string;
  url: string;
  image: Resource;
  category: Category;
}

const BRManagerTestCase: BRManagerTest[] = [
  {
    name : '开蓝牙Manager' ,
    detail : '使能蓝牙' ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '关蓝牙Manager' ,
    detail : '去使能蓝牙' ,
    method : 'disableBluetoothManager' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '获取状态Manager' ,
    detail : '获取蓝牙状态' ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '获取当前连接状态Manager' ,
    detail : '获取当前蓝牙连接状态' ,
    method : 'getBtConnectionStateManager' ,
    api : testGetBtConnectionStateManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '设置本地名称Manager' ,
    detail : '设置蓝牙本地名称' ,
    method : 'setLocalNameManager' ,
    api : testSetLocalNameManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '获取本地名称Manager' ,
    detail : '获取蓝牙本地名称' ,
    method : 'getLocalNameManager' ,
    api : testGetLocalNameManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '设置扫描模式Manager' ,
    detail : '设置蓝牙扫描模式' ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '获取扫描模式Manager' ,
    detail : '获取蓝牙扫描模式' ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '开始蓝牙发现Manager' ,
    detail : '发现可配对蓝牙设备' ,
    method : 'startBluetoothDiscoveryManager' ,
    api : testStartBluetoothDiscoveryManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '停止蓝牙发现Manager' ,
    detail : '停止发现' ,
    method : 'stopBluetoothDiscoveryManager' ,
    api : testStopBluetoothDiscoveryManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '配对设备Manager' ,
    detail : '进行蓝牙设备配对' ,
    method : 'pairDeviceManager' ,
    api : testPairDeviceManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '取消配对设备Manager' ,
    detail : '取消蓝牙设备配对' ,
    method : 'unpairDeviceManager' ,
    api : testUnPairedDeviceManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '获取远程设备名称Manager' ,
    detail : '获取配对蓝牙设备的名称' ,
    method : 'getDeviceNameManager' ,
    api : testGetRemoteDeviceNameManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '获取远程设备类型Manager' ,
    detail : '获取配对蓝牙设备的类型' ,
    method : 'getDeviceTypeManager' ,
    api : testGetRemoteDeviceClassManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '获取配对设备Manager' ,
    detail : '获取配对蓝牙设备的信息' ,
    method : 'getPairedDeviceIdsManager' ,
    api : testGetPairedDevicesManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '获取配置文件连接状态Manager1' ,
    detail : '获取配置文件的连接状态' ,
    method : 'getProfileConnStateManager' ,
    api : testGetProfileConnStateManager1 ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '获取配置文件连接状态Manager2' ,
    detail : '获取设备的配置文件状态' ,
    method : 'getDeviceStateManager' ,
    api : testGetProfileConnStateManager2 ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '注册状态变化Manager' ,
    detail : '注册状态变化' ,
    method : 'on.stateChangeManager' ,
    api : testOnStateChangeManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '注册pin码变化Manager' ,
    detail : '注册pin码变化' ,
    method : 'on.pinRequiredManager' ,
    api : testOnPinRequiredManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : '注册绑定状态变化Manager' ,
    detail : '注册绑定状态变化' ,
    method : 'on.bondStateChangeManager' ,
    api : testOnBondStateChangeManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  }
]

const BleManagerTestCase: BRManagerTest[] = [
  {
    name : '创建Gatt服务器Manager' ,
    detail : '创建GattManager服务器' ,
    method : 'createGattServerManager' ,
    api : testCreateGattServerDeviceManager ,
    result : 'None' ,
    category : CaseCategory.BLEManager
  },
  {
    name : '创建Gatt客户端Manager' ,
    detail : '创建GattManager客户端' ,
    method : 'createGattClientManager' ,
    api : testCreateGattClientDeviceManager ,
    result : 'None' ,
    category : CaseCategory.BLEManager
  },
  {
    name : '获取连接的BLE设备Manager' ,
    detail : '获取连接的BLE设备Manager信息' ,
    method : 'getConnectedBLEDevicesManager' ,
    api : testGetConnectedBLEDevicesManager ,
    result : 'None' ,
    category : CaseCategory.BLEManager
  },
  {
    name : '开始BLE扫描Manager' ,
    detail : '开始扫描BLEManager设备' ,
    method : 'startBLEScanManager' ,
    api : testStartBLEScanManager ,
    result : 'None' ,
    category : CaseCategory.BLEManager
  },
  {
    name : '停止BLE扫描Manager' ,
    detail : '停止Manager扫描' ,
    method : 'stopBLEScanManager' ,
    api : testStopBLEScanManager ,
    result : 'None' ,
    category : CaseCategory.BLEManager
  },
  {
    name : '注册BLE查找状态变化Manager' ,
    detail : '注册vBLE设备查找状态变化Manager' ,
    method : 'on.BLEDeviceFindManager' ,
    api : testOnBLEDeviceFindManager ,
    result : 'None' ,
    category : CaseCategory.BLEManager
  }
]

const GattManagerTestComposition: BRManagerTestComposition[] = [
  {
    name : 'GattClientManager' ,
    detail : 'Gatt客户端Manager测试' ,
    url : 'pages/subManualApiTestManager/subGattTestManager/gattClientManager' ,
    image : $r('app.media.Gatt') ,
    category : Category.GattManager
  },
  {
    name : 'GattServerManager' ,
    detail : 'Gatt服务器Manager测试' ,
    url : 'pages/subManualApiTestManager/subGattTestManager/gattServerManager' ,
    image : $r('app.media.Gatt') ,
    category : Category.GattManager
  },
]

const SppManagerTestComposition: BRManagerTestComposition[] = [
  {
    name : 'SppClientManager' ,
    detail : 'Spp客户端Manager测试' ,
    url : 'pages/subManualApiTestManager/subSppTestManager/sppClientManager' ,
    image : $r('app.media.spp') ,
    category : Category.SPPManager
  },
  {
    name : 'SppServerManager' ,
    detail : 'Spp服务器Manager测试' ,
    url : 'pages/subManualApiTestManager/subSppTestManager/sppServerManager' ,
    image : $r('app.media.spp') ,
    category : Category.SPPManager
  },
]

const GattClientManagerTestCase: BRManagerTest[] = [
  {
    name : '创造Gatt客户端Manager' ,
    detail : '创造Gatt客户端' ,
    method : 'createGattClientManager' ,
    api : testCreateGattClientManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '连接Manager' ,
    detail : 'Gatt连接' ,
    method : 'connectManager' ,
    api : testConnectManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '取消连接Manager' ,
    detail : '取消Gatt连接' ,
    method : 'disconnectManager' ,
    api : testDisconnectManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : 'BLE连接状态Manager' ,
    detail : '查看BLE连接状态' ,
    method : 'on.BLEConnectionStateChangeManager' ,
    api : testOnBLEConnectionStateChangeManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '关闭Manager' ,
    detail : '关闭Gatt客户端' ,
    method : 'closeManager' ,
    api : testGattClientCloseManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '获取服务callbackManager' ,
    detail : '获取服务callback' ,
    method : 'getServices(callback)Manager' ,
    api : testGetServicesCallbackManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '获取服务promiseManager' ,
    detail : '获取服务promise' ,
    method : 'getServices(promise)Manager' ,
    api : testGetServicesPromiseManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '读取特征值callbackManager' ,
    detail : '读取特征值callback' ,
    method : 'readCharacteristicValue(callback)Manager' ,
    api : testReadCharacteristicValueCallbackManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '读取特征值promiseManager' ,
    detail : '读取特征值promise' ,
    method : 'readCharacteristicValue(promise)Manager' ,
    api : testReadCharacteristicValuePromiseManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '读取描述符值callbackManager' ,
    detail : '读取描述符值callback' ,
    method : 'readDescriptorValue(callback)Manager' ,
    api : testReadDescriptorValueCallbackManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '读取描述符值promiseManager' ,
    detail : '读取描述符值promise' ,
    method : 'readDescriptorValue(promise)Manager' ,
    api : testReadDescriptorValuePromiseManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '写入特征值Manager' ,
    detail : '写入特征值' ,
    method : 'writeCharacteristicValueManager' ,
    api : testWriteCharacteristicValueManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '写入描述符值Manager' ,
    detail : '写入描述符值' ,
    method : 'writeDescriptorValueManager' ,
    api : testWriteDescriptorValueManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : 'BLE特征值变化Manager' ,
    detail : '查看BLE特征值变化' ,
    method : 'on.BLECharacteristicChangeManager' ,
    api : testOnBLECharacteristicChangeManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '设定BLEMtu尺寸Manager' ,
    detail : '设定BLEMtu尺寸' ,
    method : 'setBLEMtuSizeManager' ,
    api : testSetBLEMtuSizeManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '设置通知特性更改Manager' ,
    detail : '设置通知特性更改' ,
    method : 'setNotifyCharacteristicChangedManager' ,
    api : testSetNotifyCharacteristicChangedManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '获取设备名称callbackManager' ,
    detail : '获取设备名称callback' ,
    method : 'getDeviceName(callback)Manager' ,
    api : testGetDeviceNameCallbackManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '获取设备名称promiseManager' ,
    detail : '获取设备名称promise' ,
    method : 'getDeviceName(promise)Manager' ,
    api : testGetDeviceNamePromiseManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '获取Rssi值callbackManager' ,
    detail : '获取Rssi值callback' ,
    method : 'getRssiValue(callback)Manager' ,
    api : testGetRssiValueCallbackManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : '获取Rssi值promiseManager' ,
    detail : '获取Rssi值promise' ,
    method : 'getRssiValue(promise)Manager' ,
    api : testGetRssiValuePromiseManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
]

const GattServerManagerTestCase: BRManagerTest[] = [
  {
    name : '创建Gatt服务器Manager' ,
    detail : '创建Gatt服务器' ,
    method : 'createGattServerManager' ,
    api : testCreateGattServerManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : '添加服务Manager' ,
    detail : '添加Gatt服务' ,
    method : 'addServiceManager' ,
    api : testAddServiceManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : '删除服务Manager' ,
    detail : '删除Gatt服务' ,
    method : 'removeServiceManager' ,
    api : testRemoveServiceManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : '开始广播Manager' ,
    detail : '开始广播播报' ,
    method : 'startAdvertisingManager' ,
    api : testStartAdvertisingManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : '停止广播Manager' ,
    detail : '停止广播播报' ,
    method : 'stopAdvertisingManager' ,
    api : testStopAdvertisingManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : '关闭Manager' ,
    detail : '关闭Gatt服务器' ,
    method : 'closeManager' ,
    api : testGattServerCloseManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : '连接状态Manager' ,
    detail : '查看Gatt连接状态' ,
    method : 'on.connectStateChangeManager' ,
    api : testOnConnectStateChangeManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : '读取特征值Manager' ,
    detail : '读取特征值' ,
    method : 'on.characteristicReadManager' ,
    api : testOnCharacteristicReadManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : '写入特征值Manager' ,
    detail : '写入特征值' ,
    method : 'on.characteristicWriteManager' ,
    api : testOnCharacteristicWriteManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : '读取描述符Manager' ,
    detail : '读取描述符' ,
    method : 'on.descriptorRead' ,
    api : testOnDescriptorReadManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : '写入描述符Manager' ,
    detail : '写入描述符' ,
    method : 'on.descriptorWrite' ,
    api : testOnDescriptorWriteManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
]

const SppClientManagerTestCase: BRManagerTest[] = [
  {
    name : '开蓝牙Manager' ,
    detail : '使能蓝牙' ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : '关蓝牙Manager' ,
    detail : '去使能蓝牙' ,
    method : 'disableBluetoothManager' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : '设置SPP扫描模式Manager' ,
    detail : '设置SPP扫描模式' ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : '获取状态Manager' ,
    detail : '获取蓝牙状态' ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : '获取扫描模式Manager' ,
    detail : '获取蓝牙扫描模式' ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : '获取当前连接状态Manager' ,
    detail : '获取当前蓝牙连接状态' ,
    method : 'getBtConnectionStateManager' ,
    api : testGetBtConnectionStateManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : 'Spp客户端连接(安全模式)Manager' ,
    detail : 'Spp客户端(安全模式)连接' ,
    method : 'sppConnectManager' ,
    api : testSppClientConnectSafeManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : 'Spp客户端连接(非安全模式)Manager' ,
    detail : 'Spp客户端(非安全模式)连接' ,
    method : 'sppConnectManager' ,
    api : testSppClientConnectUnSafeManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : '配对设备Manager' ,
    detail : '进行蓝牙设备配对' ,
    method : 'pairDeviceManager' ,
    api : testPairDeviceManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : 'Spp客户端写入Manager' ,
    detail : 'Spp客户端写入' ,
    method : 'sppWriteManager' ,
    api : testSppClientWriteManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : 'Spp客户端读取打开Manager' ,
    detail : 'Spp客户端读取打开' ,
    method : 'on.sppReadManager' ,
    api : testSppClientReadOnManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : 'Spp客户端读取关闭Manager' ,
    detail : 'Spp客户端读取关闭' ,
    method : 'off.sppReadManager' ,
    api : testSppClientReadOffManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : 'Spp客户端关闭Manager' ,
    detail : 'Spp客户端关闭' ,
    method : 'sppCloseClientSocketManager' ,
    api : testSppClientCloseManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
]

const SppServerManagerTestCase: BRManagerTest[] = [
  {
    name : '开蓝牙Manager' ,
    detail : '使能蓝牙' ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : '关蓝牙Manager' ,
    detail : '去使能蓝牙' ,
    method : 'disableBluetoothManager' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : '设置SPP扫描模式Manager' ,
    detail : '设置SPP扫描模式' ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : '获取状态Manager' ,
    detail : '获取蓝牙状态' ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : '获取扫描模式Manager' ,
    detail : '获取蓝牙扫描模式' ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : '获取当前连接状态Manager' ,
    detail : '获取当前蓝牙连接状态' ,
    method : 'getBtConnectionStateManager' ,
    api : testGetBtConnectionStateManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : 'Spp服务器创建(安全模式)Manager' ,
    detail : 'Spp服务器(安全模式)创建' ,
    method : 'sppListenManager' ,
    api : testSppServerCreateSafeManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : 'Spp服务器创建(非安全模式)Manager' ,
    detail : 'Spp服务器(非安全模式)创建' ,
    method : 'sppListenManager' ,
    api : testSppServerCreateUnSafeManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : '注册pin码变化Manager' ,
    detail : '注册pin码变化' ,
    method : 'on.pinRequiredManager' ,
    api : testOnPinRequiredManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  //  { name: 'SPP客户端写入', detail: '使能蓝牙', method: 'spp_Client_Write', api: testSppClientWriteManager, result: 'None', category: CaseCategory.SPPServer },
  {
    name : 'Spp服务器写入Manager' ,
    detail : 'Spp服务器写入' ,
    method : 'sppWriteManager' ,
    api : testSppServerWriteManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : 'Spp服务器接受Manager' ,
    detail : 'Spp服务器接受' ,
    method : 'sppAcceptManager' ,
    api : testSppServerAcceptManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : 'Spp服务器读取打开Manager' ,
    detail : 'Spp服务器读取打开' ,
    method : 'on.sppReadManager' ,
    api : testSppServerReadOnManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : 'Spp服务器读取关闭Manager' ,
    detail : 'Spp服务器读取关闭' ,
    method : 'off.sppReadManager' ,
    api : testSppServerReadOffManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : 'Spp服务器关闭Manager' ,
    detail : 'Spp服务器关闭' ,
    method : 'sppCloseServerSocketManager' ,
    api : testSppCloseServerSocketManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : 'Spp客户端关闭Manager' ,
    detail : 'Spp客户端关闭' ,
    method : 'sppCloseClientSocketManager' ,
    api : testSppCloseClientSocketManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
]

const A2dpSourceProfileManagerTestCase: BRManagerTest[] = [
  {
    name : '连接Manager' ,
    detail : 'Profile连接Manager' ,
    method : 'connect' ,
    api : testProfileConnectManager ,
    result : 'None' ,
    category : CaseCategory.ProfileA2dpSourceManager
  },
  {
    name : '取消连接Manager' ,
    detail : '取消Profile连接Manager' ,
    method : 'disconnect' ,
    api : testProfileDisConnectManager ,
    result : 'None' ,
    category : CaseCategory.ProfileA2dpSourceManager
  },
  {
    name : '获取播放状态Manager' ,
    detail : '获取Profile播放状态Manager' ,
    method : 'getPlayingState' ,
    api : testProfileGetPlayingStateManager ,
    result : 'None' ,
    category : CaseCategory.ProfileA2dpSourceManager
  },
  {
    name : '获取连接设备Manager' ,
    detail : '获取Profile连接设备Manager' ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevicesManager ,
    result : 'None' ,
    category : CaseCategory.ProfileA2dpSourceManager
  },
  {
    name : '获取设备状态Manager' ,
    detail : '获取Profile设备状态Manager' ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceStateManager ,
    result : 'None' ,
    category : CaseCategory.ProfileA2dpSourceManager
  },
]

const HandsFreeAudioGatewayProfileManagerTestCase: BRManagerTest[] = [
  {
    name : '连接Manager' ,
    detail : 'Profile连接Manager' ,
    method : 'connect' ,
    api : testProfileConnectManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHandsFreeAudioGatewayManager
  },
  {
    name : '取消连接Manager' ,
    detail : '取消Profile连接Manager' ,
    method : 'disconnect' ,
    api : testProfileDisConnectManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHandsFreeAudioGatewayManager
  },
  {
    name : '获取连接设备Manager' ,
    detail : '获取Profile连接设备Manager' ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevicesManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHandsFreeAudioGatewayManager ,
  },
  {
    name : '获取设备状态Manager' ,
    detail : '获取Profile设备状态Manager' ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceStateManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHandsFreeAudioGatewayManager ,
  },
]

const HitHostProfileManagerTestCase: BRManagerTest[] = [
  {
    name : '连接Manager' ,
    detail : 'Profile连接Manager' ,
    method : 'connect' ,
    api : testProfileConnectManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHitHostManager
  },
  {
    name : '取消连接Manager' ,
    detail : '取消Profile连接Manager' ,
    method : 'disconnect' ,
    api : testProfileDisConnectManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHitHostManager
  },
  {
    name : '获取连接设备Manager' ,
    detail : '获取Profile连接设备Manager' ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevicesManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHitHostManager
  },
  {
    name : '获取设备状态Manager' ,
    detail : '获取Profile设备状态Manager' ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceStateManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHitHostManager
  },
]

const PanNetworkProfileManagerTestCase: BRManagerTest[] = [
  {
    name : '取消连接Manager' ,
    detail : '取消Profile连接Manager' ,
    method : 'disconnect' ,
    api : testProfileDisConnectManager ,
    result : 'None' ,
    category : CaseCategory.ProfilePanNetworkManager
  },
  {
    name : '获取连接设备Manager' ,
    detail : '获取Profile连接设备Manager' ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevicesManager ,
    result : 'None' ,
    category : CaseCategory.ProfilePanNetworkManager
  },
  {
    name : '获取设备状态Manager' ,
    detail : '获取Profile设备状态Manager' ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceStateManager ,
    result : 'None' ,
    category : CaseCategory.ProfilePanNetworkManager
  },
]

const GattManagerAutoTestComposition: BRManagerTestComposition[] = [
  {
    name : 'GattClientAutoTestManager' ,
    detail : 'GattClientManager自动测试' ,
    url : 'pages/subAutoTestManager/subGattAutoTestManager/gattClientAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.GattAutoTestManager
  },
  {
    name : 'GattServerAutoTestManager' ,
    detail : 'GattServerManager自动测试' ,
    url : 'pages/subAutoTestManager/subGattAutoTestManager/gattServerAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.GattAutoTestManager
  },
]

const SppManagerAutoTestComposition: BRManagerTestComposition[] = [
  {
    name : 'SppClientAutoTestManager' ,
    detail : 'SppClientManager自动测试' ,
    url : 'pages/subAutoTestManager/subSppAutoTestManager/sppClientAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.SppAutoTestManager
  },
  {
    name : 'SppServerAutoTest' ,
    detail : 'SppServer自动测试' ,
    url : 'pages/subAutoTest/subSppAutoTest/sppServerAutoTest' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.SppAutoTestManager
  },
]

const ProfileManagerAutoTestComposition: BRManagerTestComposition[] = [
  {
    name : 'A2dpSourceAutoTestManager' ,
    detail : 'A2dpSourceManager自动测试' ,
    url : 'pages/subAutoTestManager/subProfileAutoTestManager/a2dpSourceAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.ProfileAutoTestManager
  },
  {
    name : 'HFAudioGatewayAutoTestManager' ,
    detail : 'HFAudioGatewayManager自动测试' ,
    url : 'pages/subAutoTestManager/subProfileAutoTestManager/handsFreeAudioGatewayAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.ProfileAutoTestManager
  },
  {
    name : 'HitHostAutoTestManager' ,
    detail : 'HitHostManager自动测试' ,
    url : 'pages/subAutoTestManager/subProfileAutoTestManager/hitHostAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.ProfileAutoTestManager
  },
  {
    name : 'PanNetworkAutoTestManager' ,
    detail : 'PanNetworkManager自动测试' ,
    url : 'pages/subAutoTestManager/subProfileAutoTestManager/panNetworkAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.ProfileAutoTestManager
  },
]

export function initBRManagerApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  BRManagerTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initBleManagerApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  BleManagerTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initGattManagerApiTestData(): Array<TestData> {
  let TestDataArray: Array<TestData> = []
  GattManagerTestComposition.forEach(item => {
    TestDataArray.push(new TestData(item.name , item.detail , item.url , item.image , item.category));
  })
  return TestDataArray;
}

export function initSppManagerApiTestData(): Array<TestData> {
  let TestDataArray: Array<TestData> = []
  SppManagerTestComposition.forEach(item => {
    TestDataArray.push(new TestData(item.name , item.detail , item.url , item.image , item.category));
  })
  return TestDataArray;
}

export function initGattClientManagerApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  GattClientManagerTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initGattServerManagerApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  GattServerManagerTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initSppClientManagerApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  SppClientManagerTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initSppServerManagerApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  SppServerManagerTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initA2dpSourceProfileManagerApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  A2dpSourceProfileManagerTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initHandsFreeAudioGatewayProfileManagerApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  HandsFreeAudioGatewayProfileManagerTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initHitHostProfileManagerApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  HitHostProfileManagerTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initPanNetworkProfileManagerApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  PanNetworkProfileManagerTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initGattManagerAutoTestData(): Array<TestData> {
  let TestDataArray: Array<TestData> = []
  GattManagerAutoTestComposition.forEach(item => {
    TestDataArray.push(new TestData(item.name , item.detail , item.url , item.image , item.category));
  })
  return TestDataArray;
}

export function initSppManagerAutoTestData(): Array<TestData> {
  let TestDataArray: Array<TestData> = []
  SppManagerAutoTestComposition.forEach(item => {
    TestDataArray.push(new TestData(item.name , item.detail , item.url , item.image , item.category));
  })
  return TestDataArray;
}

export function initProfileManagerAutoTestData(): Array<TestData> {
  let TestDataArray: Array<TestData> = []
  ProfileManagerAutoTestComposition.forEach(item => {
    TestDataArray.push(new TestData(item.name , item.detail , item.url , item.image , item.category));
  })
  return TestDataArray;
}