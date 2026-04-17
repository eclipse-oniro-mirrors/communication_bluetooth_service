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

import { TestAuto , AutoCategory } from './testData'
import {
  testEnableBluetooth ,
  testDisableBluetooth ,
  testGetState ,
  testGetBtConnectionState ,
  testSetLocalName ,
  testGetLocalName ,
  testSetBluetoothScanMode ,
  testGetBluetoothScanMode ,
  subscribeBluetoothDeviceFind ,
  testStopBluetoothDiscovery ,
  testPairDevice ,
  testGetRemoteDeviceName ,
  testGetRemoteDeviceClass ,
  testGetPairedDevices ,
  testUnPairedDevice ,
  testGetProfileConnState1 ,
  testGetProfileConnState2 ,
  testOnStateChange ,
  testOnPinRequired ,
  testOnBondStateChange
} from './brInterface'
import {
  testCreateGattServerDevice ,
  testCreateGattClientDevice ,
  testGetConnectedBLEDevices ,
  testStartBLEScan ,
  testStopBLEScan ,
  testOnBLEDeviceFind
} from './bleInterface'
import {
  testCreateGattClient ,
  testConnect ,
  testDisconnect ,
  testOnBLEConnectionStateChange ,
  testGattClientClose ,
  testGetServicesCallback ,
  testGetServicesPromise ,
  testReadCharacteristicValueCallback ,
  testReadCharacteristicValuePromise ,
  testReadDescriptorValueCallback ,
  testReadDescriptorValuePromise ,
  testWriteCharacteristicValue ,
  testWriteDescriptorValue ,
  testOnBLECharacteristicChange ,
  testSetBLEMtuSize ,
  testSetNotifyCharacteristicChanged ,
  testGetDeviceNameCallback ,
  testGetDeviceNamePromise ,
  testGetRssiValueCallback ,
  testGetRssiValuePromise
} from './gattClientInterface'
import {
  testCreateGattServer ,
  testStartAdvertising ,
  testStopAdvertising ,
  testAddService ,
  testRemoveService ,
  testGattServerClose ,
  testOnConnectStateChange ,
  testOnCharacteristicRead ,
  testOnCharacteristicWrite ,
  testOnDescriptorRead ,
  testOnDescriptorWrite
} from './gattServerInterface'
import {
  testSppClientConnectSafe ,
  testSppClientConnectUnSafe ,
  testSppClientWrite ,
  testSppClientReadOn ,
  testSppClientReadOff ,
  testSppClientClose
} from './sppClientInterface'
import {
  testSppServerCreateSafe ,
  testSppServerCreateUnSafe ,
  testSppServerAccept ,
  testSppServerWrite ,
  testSppServerReadOn ,
  testSppServerReadOff ,
  testSppCloseServerSocket ,
  testSppCloseClientSocket
} from './sppServerInterface'
import {
  testProfileConnect ,
  testProfileDisConnect ,
  testProfileGetPlayingState ,
  testProfileGetConnectionDevices ,
  testProfileGetDeviceState
} from './a2dpSinkProfileInterface'
import BluetoothModel from './BluetoothModel'

interface AutoTest {
  name: string;
  detail: string;
  method: string;
  api: Function;
  result: string;
  category: AutoCategory;
}

export const BrAutoTestCase: AutoTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_zcztbh') ,
    detail : $r('app.string.bluetoothtest_str_zcztbh') ,
    method : 'on.stateChange' ,
    api : testOnStateChange ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_zcpmbh') ,
    detail : $r('app.string.bluetoothtest_str_zcpmbh') ,
    method : 'on.pinRequired' ,
    api : testOnPinRequired ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_zcbdztbh') ,
    detail : $r('app.string.bluetoothtest_str_zcbdztbh') ,
    method : 'on.bondStateChange' ,
    api : testOnBondStateChange ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqdqljzt') ,
    detail : $r('app.string.bluetoothtest_str_hqdqlyljzt') ,
    method : 'getBtConnectionState' ,
    api : testGetBtConnectionState ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmc') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalName' ,
    api : testSetLocalName ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmc') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalName' ,
    api : testGetLocalName ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmms') ,
    detail : $r('app.string.bluetoothtest_str_szlysmms') ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmms') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_dylyfx') ,
    detail : $r('app.string.bluetoothtest_str_dyfxlysb') ,
    method : 'subscribeBluetoothDeviceFind' ,
    api : subscribeBluetoothDeviceFind ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_kslyfx') ,
    detail : $r('app.string.bluetoothtest_str_ksfx') ,
    method : 'startBluetoothDiscovery' ,
    api : BluetoothModel.startBluetoothDiscovery ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_tzlyfx') ,
    detail : $r('app.string.bluetoothtest_str_tzfx') ,
    method : 'stopBluetoothDiscovery' ,
    api : testStopBluetoothDiscovery ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_pdsb') ,
    detail : $r('app.string.bluetoothtest_str_jxlysbpd') ,
    method : 'pairDevice' ,
    api : testPairDevice ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqycsbmc') ,
    detail : $r('app.string.bluetoothtest_str_hqpdlysbdmc') ,
    method : 'getDeviceName' ,
    api : testGetRemoteDeviceName ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqycsblx') ,
    detail : $r('app.string.bluetoothtest_str_hqpdlysbdlx') ,
    method : 'getDeviceType' ,
    api : testGetRemoteDeviceClass ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqpdsb') ,
    detail : $r('app.string.bluetoothtest_str_hqpdlysbdxx') ,
    method : 'getPairedDeviceIds' ,
    api : testGetPairedDevices ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_qxpdsb_1') ,
    detail : $r('app.string.bluetoothtest_str_qxlysbpd') ,
    method : 'unpairDevice' ,
    api : testUnPairedDevice ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
  //  { name: '获取配置文件连接状态1', detail: '获取配置文件的连接状态', method: 'getProfileConnState', api:testGetProfileConnState1,result: 'None', category: AutoCategory.BrAuto },
  //  { name: '获取配置文件连接状态2', detail: '获取设备的配置文件状态', method: 'getDeviceState',api:testGetProfileConnState2, result: 'None', category: AutoCategory.BrAuto },
  {
    name : $r('app.string.bluetoothtest_str_gly') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetooth' ,
    api : testDisableBluetooth ,
    result : 'None' ,
    category : AutoCategory.BrAuto
  },
]

export const BleAutoTestCase: AutoTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : AutoCategory.BleAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : AutoCategory.BleAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmc') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalName' ,
    api : testSetLocalName ,
    result : 'None' ,
    category : AutoCategory.BleAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmc') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalName' ,
    api : testGetLocalName ,
    result : 'None' ,
    category : AutoCategory.BleAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmms') ,
    detail : $r('app.string.bluetoothtest_str_szlysmms') ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.BleAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmms') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.BleAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljdbsb_1') ,
    detail : $r('app.string.bluetoothtest_str_hqljdbsbxx_1') ,
    method : 'getConnectedBleDevices' ,
    api : testGetConnectedBLEDevices ,
    result : 'None' ,
    category : AutoCategory.BleAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_zcbczztbh_1') ,
    detail : $r('app.string.bluetoothtest_str_zcbsbczztbh_1') ,
    method : 'on.BleDeviceFind' ,
    api : testOnBLEDeviceFind ,
    result : 'None' ,
    category : AutoCategory.BleAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_ksbsm_1') ,
    detail : $r('app.string.bluetoothtest_str_kssmbsb_2') ,
    method : 'startBleScan' ,
    api : testStartBLEScan ,
    result : 'None' ,
    category : AutoCategory.BleAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_tzbsm_1') ,
    detail : $r('app.string.bluetoothtest_str_tzsm') ,
    method : 'stopBleScan' ,
    api : testStopBLEScan ,
    result : 'None' ,
    category : AutoCategory.BleAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_gly') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetooth' ,
    api : testDisableBluetooth ,
    result : 'None' ,
    category : AutoCategory.BleAuto
  },
]

export const GattClientAutoTestCase: AutoTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmc') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalName' ,
    api : testSetLocalName ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmc') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalName' ,
    api : testGetLocalName ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmms') ,
    detail : $r('app.string.bluetoothtest_str_szlysmms') ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmms') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_czgkhd') ,
    detail : $r('app.string.bluetoothtest_str_czgkhd') ,
    method : 'createGattClient' ,
    api : testCreateGattClient ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_bljzt_1') ,
    detail : $r('app.string.bluetoothtest_str_ckbljzt_1') ,
    method : 'on.BleConnectionStateChange' ,
    api : testOnBLEConnectionStateChange ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_lj') ,
    detail : $r('app.string.bluetoothtest_str_glj') ,
    method : 'connect' ,
    api : testConnect ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqfwc_1') ,
    detail : $r('app.string.bluetoothtest_str_hqfwc_1') ,
    method : 'getServices(Callback)' ,
    api : testGetServicesCallback ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqfwp_1') ,
    detail : $r('app.string.bluetoothtest_str_hqfwp_1') ,
    method : 'getServices(promise)' ,
    api : testGetServicesPromise ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_dqtzzc_1') ,
    detail : $r('app.string.bluetoothtest_str_dqtzzc_1') ,
    method : 'readCharacteristicValue(callback)' ,
    api : testReadCharacteristicValueCallback ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_dqtzzp_1') ,
    detail : $r('app.string.bluetoothtest_str_dqtzzp_1') ,
    method : 'readCharacteristicValue(promise)' ,
    api : testReadCharacteristicValuePromise ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_dqmsfzc_1') ,
    detail : $r('app.string.bluetoothtest_str_dqmsfzc_1') ,
    method : 'readDescriptorValue(callback)' ,
    api : testReadDescriptorValueCallback ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_dqmsfzp_1') ,
    detail : $r('app.string.bluetoothtest_str_dqmsfzp_1') ,
    method : 'readDescriptorValue(promise)' ,
    api : testReadDescriptorValuePromise ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_xrtzz') ,
    detail : $r('app.string.bluetoothtest_str_xrtzz') ,
    method : 'writeCharacteristicValue' ,
    api : testWriteCharacteristicValue ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_xrmsfz') ,
    detail : $r('app.string.bluetoothtest_str_xrmsfz') ,
    method : 'writeDescriptorValue' ,
    api : testWriteDescriptorValue ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_btzzbh_1') ,
    detail : $r('app.string.bluetoothtest_str_ckbtzzbh_1') ,
    method : 'on.BleCharacteristicChange' ,
    api : testOnBLECharacteristicChange ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_sdbcc_1') ,
    detail : $r('app.string.bluetoothtest_str_sdbcc_1') ,
    method : 'setBleMtuSize' ,
    api : testSetBLEMtuSize ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_sztztxgg') ,
    detail : $r('app.string.bluetoothtest_str_sztztxgg') ,
    method : 'setNotifyCharacteristicChanged' ,
    api : testSetNotifyCharacteristicChanged ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbmcc_1') ,
    detail : $r('app.string.bluetoothtest_str_hqsbmcc_1') ,
    method : 'getDeviceName()callback' ,
    api : testGetDeviceNameCallback ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbmcp_1') ,
    detail : $r('app.string.bluetoothtest_str_hqsbmcp_1') ,
    method : 'getDeviceName()Promise' ,
    api : testGetDeviceNamePromise ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqrzc_1') ,
    detail : $r('app.string.bluetoothtest_str_hqrzc_1') ,
    method : 'getRssiValue()Callback' ,
    api : testGetRssiValueCallback ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqrzp_1') ,
    detail : $r('app.string.bluetoothtest_str_hqrzp_1') ,
    method : 'getRssiValue()Promise' ,
    api : testGetRssiValuePromise ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_qxlj') ,
    detail : $r('app.string.bluetoothtest_str_qxglj') ,
    method : 'disconnect' ,
    api : testDisconnect ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_gb') ,
    detail : $r('app.string.bluetoothtest_str_gbgkhd') ,
    method : 'close' ,
    api : testGattClientClose ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_gly') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetooth' ,
    api : testDisableBluetooth ,
    result : 'None' ,
    category : AutoCategory.GattClientAuto
  },
]

export const GattServerAutoTestCase: AutoTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmc') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalName' ,
    api : testSetLocalName ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmc') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalName' ,
    api : testGetLocalName ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmms') ,
    detail : $r('app.string.bluetoothtest_str_szlysmms') ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmms') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_cjgfwq_1') ,
    detail : $r('app.string.bluetoothtest_str_cjgfwq_1') ,
    method : 'createGattServer' ,
    api : testCreateGattServer ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_tjfw') ,
    detail : $r('app.string.bluetoothtest_str_tjgfw') ,
    method : 'addService' ,
    api : testAddService ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_ksgb') ,
    detail : $r('app.string.bluetoothtest_str_ksgbbb') ,
    method : 'startAdvertising' ,
    api : testStartAdvertising ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_ljzt') ,
    detail : $r('app.string.bluetoothtest_str_ckgljzt') ,
    method : 'on.connectStateChange' ,
    api : testOnConnectStateChange ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_dqtzz') ,
    detail : $r('app.string.bluetoothtest_str_dqtzz') ,
    method : 'on.characteristicRead' ,
    api : testOnCharacteristicRead ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_xrtzz') ,
    detail : $r('app.string.bluetoothtest_str_xrtzz') ,
    method : 'on.characteristicWrite' ,
    api : testOnCharacteristicWrite ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_dqmsf') ,
    detail : $r('app.string.bluetoothtest_str_dqmsf') ,
    method : 'on.descriptorRead' ,
    api : testOnDescriptorRead ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_xrmsf') ,
    detail : $r('app.string.bluetoothtest_str_xrmsf') ,
    method : 'on.descriptorWrite' ,
    api : testOnDescriptorWrite ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_tzgb') ,
    detail : $r('app.string.bluetoothtest_str_tzgbbb') ,
    method : 'stopAdvertising' ,
    api : testStopAdvertising ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_scfw') ,
    detail : $r('app.string.bluetoothtest_str_scgfw') ,
    method : 'removeService' ,
    api : testRemoveService ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_gb') ,
    detail : $r('app.string.bluetoothtest_str_gbgfwq') ,
    method : 'close' ,
    api : testGattServerClose ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_gly') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetooth' ,
    api : testDisableBluetooth ,
    result : 'None' ,
    category : AutoCategory.GattServerAuto
  },
]

export const SppClientAutoTestCase: AutoTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : AutoCategory.SppClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_szssmms') ,
    detail : $r('app.string.bluetoothtest_str_szssmms') ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.SppClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmc') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalName' ,
    api : testSetLocalName ,
    result : 'None' ,
    category : AutoCategory.SppClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmc') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalName' ,
    api : testGetLocalName ,
    result : 'None' ,
    category : AutoCategory.SppClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : AutoCategory.SppClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmms') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.SppClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdljaqms') ,
    detail : $r('app.string.bluetoothtest_str_skhdaqmslj') ,
    method : 'sppConnect' ,
    api : testSppClientConnectSafe ,
    result : 'None' ,
    category : AutoCategory.SppClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdljfaqms') ,
    detail : $r('app.string.bluetoothtest_str_skhdfaqmslj') ,
    method : 'sppConnect' ,
    api : testSppClientConnectUnSafe ,
    result : 'None' ,
    category : AutoCategory.SppClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_pdsb') ,
    detail : $r('app.string.bluetoothtest_str_jxlysbpd') ,
    method : 'pairDevice' ,
    api : testPairDevice ,
    result : 'None' ,
    category : AutoCategory.SppClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdxr') ,
    detail : $r('app.string.bluetoothtest_str_skhdxr') ,
    method : 'sppWrite' ,
    api : testSppClientWrite ,
    result : 'None' ,
    category : AutoCategory.SppClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_skhddqdk') ,
    detail : $r('app.string.bluetoothtest_str_skhddqdk') ,
    method : 'on.sppRead' ,
    api : testSppClientReadOn ,
    result : 'None' ,
    category : AutoCategory.SppClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_skhddqgb') ,
    detail : $r('app.string.bluetoothtest_str_skhddqgb') ,
    method : 'off.sppRead' ,
    api : testSppClientReadOff ,
    result : 'None' ,
    category : AutoCategory.SppClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdgb') ,
    detail : $r('app.string.bluetoothtest_str_skhdgb') ,
    method : 'sppCloseClientSocket' ,
    api : testSppClientClose ,
    result : 'None' ,
    category : AutoCategory.SppClientAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_gly') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetooth' ,
    api : testDisableBluetooth ,
    result : 'None' ,
    category : AutoCategory.SppClientAuto
  },
]

export const SppServerAutoTestCase: AutoTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_szssmms') ,
    detail : $r('app.string.bluetoothtest_str_szssmms') ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_zcpmbh') ,
    detail : $r('app.string.bluetoothtest_str_zcpmbh') ,
    method : 'on.pinRequired' ,
    api : testOnPinRequired ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmc') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalName' ,
    api : testSetLocalName ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmc') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalName' ,
    api : testGetLocalName ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmms') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqcjaqms') ,
    detail : $r('app.string.bluetoothtest_str_sfwqaqmscj') ,
    method : 'sppListen' ,
    api : testSppServerCreateSafe ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqcjfaqms') ,
    detail : $r('app.string.bluetoothtest_str_sfwqfaqmscj') ,
    method : 'sppListen' ,
    api : testSppServerCreateUnSafe ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqxr') ,
    detail : $r('app.string.bluetoothtest_str_sfwqxr') ,
    method : 'sppWrite' ,
    api : testSppServerWrite ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqjs') ,
    detail : $r('app.string.bluetoothtest_str_sfwqjs') ,
    method : 'sppAccept' ,
    api : testSppServerAccept ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqdqdk') ,
    detail : $r('app.string.bluetoothtest_str_sfwqdqdk') ,
    method : 'on.sppRead' ,
    api : testSppServerReadOn ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqdqgb') ,
    detail : $r('app.string.bluetoothtest_str_sfwqdqgb') ,
    method : 'off.sppRead' ,
    api : testSppServerReadOff ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqgb') ,
    detail : $r('app.string.bluetoothtest_str_sfwqgb') ,
    method : 'sppCloseServerSocket' ,
    api : testSppCloseServerSocket ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdgb') ,
    detail : $r('app.string.bluetoothtest_str_skhdgb') ,
    method : 'sppCloseClientSocket' ,
    api : testSppCloseClientSocket ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_gly') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetooth' ,
    api : testDisableBluetooth ,
    result : 'None' ,
    category : AutoCategory.SppServerAuto
  },
]

export const A2dpSourceProfileAutoTestCase: AutoTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAuto
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmc') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalName' ,
    api : testSetLocalName ,
    result : 'None' ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAuto
  },
  {
    name : '获取本地名称' ,
    detail : '获取蓝牙本地名称' ,
    method : 'getLocalName' ,
    api : testGetLocalName ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAuto
  },
  {
    name : '设置扫描模式' ,
    detail : '设置蓝牙扫描模式' ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAuto
  },
  {
    name : '获取扫描模式' ,
    detail : '获取蓝牙扫描模式' ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAuto
  },
  {
    name : '连接' ,
    detail : 'Profile连接' ,
    method : 'connect' ,
    api : testProfileConnect ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAuto
  },
  {
    name : '获取播放状态' ,
    detail : '获取Profile播放状态' ,
    method : 'getPlayingState' ,
    api : testProfileGetPlayingState ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAuto
  },
  {
    name : '获取连接设备' ,
    detail : '获取Profile连接设备' ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevices ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAuto
  },
  {
    name : '获取设备状态' ,
    detail : '获取Profile设备状态' ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceState ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAuto
  },
  {
    name : '取消连接' ,
    detail : '取消Profile连接' ,
    method : 'disconnect' ,
    api : testProfileDisConnect ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAuto
  },
]

export const HandsFreeAudioGatewayProfileAutoTestCase: AutoTest[] = [
  {
    name : '开蓝牙' ,
    detail : '使能蓝牙' ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAuto
  },
  {
    name : '获取状态' ,
    detail : '获取蓝牙状态' ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAuto
  },
  {
    name : '设置本地名称' ,
    detail : '设置蓝牙本地名称' ,
    method : 'setLocalName' ,
    api : testSetLocalName ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAuto
  },
  {
    name : '获取本地名称' ,
    detail : '获取蓝牙本地名称' ,
    method : 'getLocalName' ,
    api : testGetLocalName ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAuto
  },
  {
    name : '设置扫描模式' ,
    detail : '设置蓝牙扫描模式' ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAuto
  },
  {
    name : '获取扫描模式' ,
    detail : '获取蓝牙扫描模式' ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAuto
  },
  {
    name : '连接' ,
    detail : 'Profile连接' ,
    method : 'connect' ,
    api : testProfileConnect ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAuto
  },
  {
    name : '获取连接设备' ,
    detail : '获取Profile连接设备' ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevices ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAuto ,
  },
  {
    name : '获取设备状态' ,
    detail : '获取Profile设备状态' ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceState ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAuto ,
  },
  {
    name : '取消连接' ,
    detail : '取消Profile连接' ,
    method : 'disconnect' ,
    api : testProfileDisConnect ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAuto
  },
]

export const HitHostProfileAutoTestCase: AutoTest[] = [
  {
    name : '开蓝牙' ,
    detail : '使能蓝牙' ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : AutoCategory.HitHostAuto
  },
  {
    name : '获取状态' ,
    detail : '获取蓝牙状态' ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : AutoCategory.HitHostAuto
  },
  {
    name : '设置本地名称' ,
    detail : '设置蓝牙本地名称' ,
    method : 'setLocalName' ,
    api : testSetLocalName ,
    result : 'None' ,
    category : AutoCategory.HitHostAuto
  },
  {
    name : '获取本地名称' ,
    detail : '获取蓝牙本地名称' ,
    method : 'getLocalName' ,
    api : testGetLocalName ,
    result : 'None' ,
    category : AutoCategory.HitHostAuto
  },
  {
    name : '设置扫描模式' ,
    detail : '设置蓝牙扫描模式' ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.HitHostAuto
  },
  {
    name : '获取扫描模式' ,
    detail : '获取蓝牙扫描模式' ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.HitHostAuto
  },
  {
    name : '连接' ,
    detail : 'Profile连接' ,
    method : 'connect' ,
    api : testProfileConnect ,
    result : 'None' ,
    category : AutoCategory.HitHostAuto
  },
  {
    name : '获取连接设备' ,
    detail : '获取Profile连接设备' ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevices ,
    result : 'None' ,
    category : AutoCategory.HitHostAuto
  },
  {
    name : '获取设备状态' ,
    detail : '获取Profile设备状态' ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceState ,
    result : 'None' ,
    category : AutoCategory.HitHostAuto
  },
  {
    name : '取消连接' ,
    detail : '取消Profile连接' ,
    method : 'disconnect' ,
    api : testProfileDisConnect ,
    result : 'None' ,
    category : AutoCategory.HitHostAuto
  },
]

export const PanNetworkProfileAutoTestCase: AutoTest[] = [
  {
    name : '开蓝牙' ,
    detail : '使能蓝牙' ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAuto
  },
  {
    name : '获取状态' ,
    detail : '获取蓝牙状态' ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAuto
  },
  {
    name : '设置本地名称' ,
    detail : '设置蓝牙本地名称' ,
    method : 'setLocalName' ,
    api : testSetLocalName ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAuto
  },
  {
    name : '获取本地名称' ,
    detail : '获取蓝牙本地名称' ,
    method : 'getLocalName' ,
    api : testGetLocalName ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAuto
  },
  {
    name : '设置扫描模式' ,
    detail : '设置蓝牙扫描模式' ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAuto
  },
  {
    name : '获取扫描模式' ,
    detail : '获取蓝牙扫描模式' ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAuto
  },
  {
    name : '获取连接设备' ,
    detail : '获取Profile连接设备' ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevices ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAuto
  },
  {
    name : '获取设备状态' ,
    detail : '获取Profile设备状态' ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceState ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAuto
  },
  {
    name : '取消连接' ,
    detail : '取消Profile连接' ,
    method : 'disconnect' ,
    api : testProfileDisConnect ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAuto
  },
]

export function initBrAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  BrAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

export function initBleAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  BleAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

export function initGattClientAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  GattClientAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

export function initGattServerAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  GattServerAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

export function initSppClientAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  SppClientAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

export function initSppServerAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  SppServerAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}


export function initA2dpSourceProfileAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  A2dpSourceProfileAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}


export function initHandsFreeAudioGatewayProfileAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  HandsFreeAudioGatewayProfileAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}


export function initHitHostProfileAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  HitHostProfileAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

export function initPanNetworkProfileAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  PanNetworkProfileAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

