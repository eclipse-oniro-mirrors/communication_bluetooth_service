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
  testDisableBluetooth ,
  testEnableBluetooth ,
  testGetBluetoothScanMode ,
  testGetBtConnectionState ,
  testGetConnectionStrategy ,
  testGetLocalName ,
  testGetPairedDevices ,
  testGetProfileConnState1 ,
  testGetProfileConnState2 ,
  testGetRemoteDeviceClass ,
  testGetRemoteDeviceName ,
  testGetState ,
  testOnBondStateChange ,
  testOnPinRequired ,
  testOnStateChange ,
  testPairDevice ,
  testSetBluetoothScanMode ,
  testSetConnectionStrategy ,
  testSetLocalName ,
  testStartBluetoothDiscovery ,
  testStopBluetoothDiscovery ,
  testUnPairedDevice
} from './brInterface'
import {
  testCreateGattClientDevice ,
  testCreateGattServerDevice ,
  testGetConnectedBLEDevices ,
  testOnBLEDeviceFind ,
  testStartBLEScan ,
  testStopBLEScan
} from './bleInterface'
import {
  testConnect ,
  testCreateGattClient ,
  testDisconnect ,
  testGattClientClose ,
  testGetDeviceNameCallback ,
  testGetDeviceNamePromise ,
  testGetRssiValueCallback ,
  testGetRssiValuePromise ,
  testGetServicesCallback ,
  testGetServicesPromise ,
  testOnBLECharacteristicChange ,
  testOnBLEConnectionStateChange ,
  testReadCharacteristicValueCallback ,
  testReadCharacteristicValuePromise ,
  testReadDescriptorValueCallback ,
  testReadDescriptorValuePromise ,
  testSetBLEMtuSize ,
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
  testSppClientConnectUnSafe ,
  testSppClientReadOff ,
  testSppClientReadOn ,
  testSppClientWrite
} from './sppClientInterface'
import {
  testSppCloseClientSocket ,
  testSppCloseServerSocket ,
  testSppServerAccept ,
  testSppServerCreateSafe ,
  testSppServerCreateUnSafe ,
  testSppServerReadOff ,
  testSppServerReadOn ,
  testSppServerWrite
} from './sppServerInterface'

import {
  testCancelTransfer ,
  testGetCurrentTransferInformation ,
  testOnReceiveIncomingFile ,
  testOnTransferStateChange ,
  testProfileConnect ,
  testProfileDisConnect ,
  testProfileGetConnectionDevices ,
  testProfileGetDeviceState ,
  testProfileGetPlayingState ,
  testSetIncomingFileConfirmation
} from './a2dpSinkProfileInterface'

import bluetooth from '@ohos.bluetooth';

interface BRTest {
  name: Resource | string;
  detail: Resource | string;
  method: string;
  api: Function;
  result: string;
  category: CaseCategory;
}

interface BRTestComposition {
  name: Resource | string;
  detail: Resource | string;
  url: string;
  image: Resource;
  category: Category;
}

const BRTestCase: BRTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_gly') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetooth' ,
    api : testDisableBluetooth ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_zcztbh') ,
    detail : $r('app.string.bluetoothtest_str_zcztbh') ,
    method : 'on.stateChange' ,
    api : testOnStateChange ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_hqdqljzt') ,
    detail : $r('app.string.bluetoothtest_str_hqdqlyljzt') ,
    method : 'getBtConnectionState' ,
    api : testGetBtConnectionState ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmc') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalName' ,
    api : testSetLocalName ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmc') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalName' ,
    api : testGetLocalName ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmms') ,
    detail : $r('app.string.bluetoothtest_str_szlysmms') ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmms') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_zcpmbh') ,
    detail : $r('app.string.bluetoothtest_str_zcpmbh') ,
    method : 'on.pinRequired' ,
    api : testOnPinRequired ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_zcbdztbh') ,
    detail : $r('app.string.bluetoothtest_str_zcbdztbh') ,
    method : 'on.bondStateChange' ,
    api : testOnBondStateChange ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_kslyfx') ,
    detail : $r('app.string.bluetoothtest_str_fxkpdlysb') ,
    method : 'startBluetoothDiscovery' ,
    api : testStartBluetoothDiscovery ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_tzlyfx') ,
    detail : $r('app.string.bluetoothtest_str_tzfx') ,
    method : 'stopBluetoothDiscovery' ,
    api : testStopBluetoothDiscovery ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_pdsb') ,
    detail : $r('app.string.bluetoothtest_str_jxlysbpd') ,
    method : 'pairDevice' ,
    api : testPairDevice ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_qxpdsb_1') ,
    detail : $r('app.string.bluetoothtest_str_qxlysbpd') ,
    method : 'unpairDevice' ,
    api : testUnPairedDevice ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_hqycsbmc') ,
    detail : $r('app.string.bluetoothtest_str_hqpdlysbdmc') ,
    method : 'getDeviceName' ,
    api : testGetRemoteDeviceName ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_hqycsblx') ,
    detail : $r('app.string.bluetoothtest_str_hqpdlysbdlx') ,
    method : 'getDeviceType' ,
    api : testGetRemoteDeviceClass ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_hqpdsb') ,
    detail : $r('app.string.bluetoothtest_str_hqpdlysbdxx') ,
    method : 'getPairedDeviceIds' ,
    api : testGetPairedDevices ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_szpljcl') ,
    detail : $r('app.string.bluetoothtest_str_szpljcl') ,
    method : 'setConnectionStrategy' ,
    api : testSetConnectionStrategy ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_hqpljcl') ,
    detail : $r('app.string.bluetoothtest_str_hqpljcl') ,
    method : 'getConnectionStrategy' ,
    api : testGetConnectionStrategy ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_hqpzwjljzt1') ,
    detail : $r('app.string.bluetoothtest_str_hqpzwjdljzt') ,
    method : 'getProfileConnState' ,
    api : testGetProfileConnState1 ,
    result : 'None' ,
    category : CaseCategory.BR
  },
  {
    name : $r('app.string.bluetoothtest_str_hqpzwjljzt2') ,
    detail : $r('app.string.bluetoothtest_str_hqsbdpzwjzt') ,
    method : 'getDeviceState' ,
    api : testGetProfileConnState2 ,
    result : 'None' ,
    category : CaseCategory.BR
  }
]

const BleTestCase: BRTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_cjgfwq_1') ,
    detail : $r('app.string.bluetoothtest_str_cjgfwq_1') ,
    method : 'createGattServer' ,
    api : testCreateGattServerDevice ,
    result : 'None' ,
    category : CaseCategory.BLE
  },
  {
    name : $r('app.string.bluetoothtest_str_cjgkhd_1') ,
    detail : $r('app.string.bluetoothtest_str_cjgkhd_1') ,
    method : 'createGattClient' ,
    api : testCreateGattClientDevice ,
    result : 'None' ,
    category : CaseCategory.BLE
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljdbsb') ,
    detail : $r('app.string.bluetoothtest_str_hqljdbsbxx') ,
    method : 'getConnectedBLEDevices' ,
    api : testGetConnectedBLEDevices ,
    result : 'None' ,
    category : CaseCategory.BLE
  },
  {
    name : $r('app.string.bluetoothtest_str_ksbsm') ,
    detail : $r('app.string.bluetoothtest_str_kssmbsb_1') ,
    method : 'startBLEScan' ,
    api : testStartBLEScan ,
    result : 'None' ,
    category : CaseCategory.BLE
  },
  {
    name : $r('app.string.bluetoothtest_str_tzbsm') ,
    detail : $r('app.string.bluetoothtest_str_tzsm') ,
    method : 'stopBLEScan' ,
    api : testStopBLEScan ,
    result : 'None' ,
    category : CaseCategory.BLE
  },
  {
    name : $r('app.string.bluetoothtest_str_zcbczztbh') ,
    detail : $r('app.string.bluetoothtest_str_zcbsbczztbh') ,
    method : 'on.BLEDeviceFind' ,
    api : testOnBLEDeviceFind ,
    result : 'None' ,
    category : CaseCategory.BLE
  }
]

const GattTestComposition: BRTestComposition[] = [
  {
    name : 'GattClient' ,
    detail : $r('app.string.bluetoothtest_str_gkhdcs') ,
    url : 'pages/subManualApiTest/subGattTest/gattClient' ,
    image : $r('app.media.Gatt') ,
    category : Category.Gatt
  },
  {
    name : 'GattServer' ,
    detail : $r('app.string.bluetoothtest_str_gfwqcs') ,
    url : 'pages/subManualApiTest/subGattTest/gattServer' ,
    image : $r('app.media.Gatt') ,
    category : Category.Gatt
  },
]

const SppTestComposition: BRTestComposition[] = [
  {
    name : 'SppClient' ,
    detail : $r('app.string.bluetoothtest_str_skhdcs') ,
    url : 'pages/subManualApiTest/subSppTest/sppClientTest' ,
    image : $r('app.media.spp') ,
    category : Category.SPP
  },
  {
    name : 'SppServer' ,
    detail : $r('app.string.bluetoothtest_str_sfwqcs') ,
    url : 'pages/subManualApiTest/subSppTest/sppServerTest' ,
    image : $r('app.media.spp') ,
    category : Category.SPP
  },
]

const GattClientTestCase: BRTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_czgkhd') ,
    detail : $r('app.string.bluetoothtest_str_czgkhd') ,
    method : 'createGattClient' ,
    api : testCreateGattClient ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_lj') ,
    detail : $r('app.string.bluetoothtest_str_glj') ,
    method : 'connect' ,
    api : testConnect ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_qxlj') ,
    detail : $r('app.string.bluetoothtest_str_qxglj') ,
    method : 'disconnect' ,
    api : testDisconnect ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_bljzt') ,
    detail : $r('app.string.bluetoothtest_str_ckbljzt') ,
    method : 'on.BLEConnectionStateChange' ,
    api : testOnBLEConnectionStateChange ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_gb') ,
    detail : $r('app.string.bluetoothtest_str_gbgkhd') ,
    method : 'close' ,
    api : testGattClientClose ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_hqfwc_1') ,
    detail : $r('app.string.bluetoothtest_str_hqfwc_1') ,
    method : 'getServices(Callback)' ,
    api : testGetServicesCallback ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_hqfwp_1') ,
    detail : $r('app.string.bluetoothtest_str_hqfwp_1') ,
    method : 'getServices(promise)' ,
    api : testGetServicesPromise ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_dqtzzc_1') ,
    detail : $r('app.string.bluetoothtest_str_dqtzzc_1') ,
    method : 'readCharacteristicValue(callback)' ,
    api : testReadCharacteristicValueCallback ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_dqtzzp_1') ,
    detail : $r('app.string.bluetoothtest_str_dqtzzp_1') ,
    method : 'readCharacteristicValue(promise)' ,
    api : testReadCharacteristicValuePromise ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_dqmsfzc_1') ,
    detail : $r('app.string.bluetoothtest_str_dqmsfzc_1') ,
    method : 'readDescriptorValue(callback)' ,
    api : testReadDescriptorValueCallback ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_dqmsfzp_1') ,
    detail : $r('app.string.bluetoothtest_str_dqmsfzp_1') ,
    method : 'readDescriptorValue(promise)' ,
    api : testReadDescriptorValuePromise ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_xrtzz') ,
    detail : $r('app.string.bluetoothtest_str_xrtzz') ,
    method : 'writeCharacteristicValue' ,
    api : testWriteCharacteristicValue ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_xrmsfz') ,
    detail : $r('app.string.bluetoothtest_str_xrmsfz') ,
    method : 'writeDescriptorValue' ,
    api : testWriteDescriptorValue ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_btzzbh') ,
    detail : $r('app.string.bluetoothtest_str_ckbtzzbh') ,
    method : 'on.BLECharacteristicChange' ,
    api : testOnBLECharacteristicChange ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_sdbcc') ,
    detail : $r('app.string.bluetoothtest_str_sdbcc') ,
    method : 'setBLEMtuSize' ,
    api : testSetBLEMtuSize ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_sztztxgg') ,
    detail : $r('app.string.bluetoothtest_str_sztztxgg') ,
    method : 'setNotifyCharacteristicChanged' ,
    api : testSetNotifyCharacteristicChanged ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbmcc_1') ,
    detail : $r('app.string.bluetoothtest_str_hqsbmcc_1') ,
    method : 'getDeviceName(callback)' ,
    api : testGetDeviceNameCallback ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbmcp_1') ,
    detail : $r('app.string.bluetoothtest_str_hqsbmcp_1') ,
    method : 'getDeviceName(promise)' ,
    api : testGetDeviceNamePromise ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_hqrzc_1') ,
    detail : $r('app.string.bluetoothtest_str_hqrzc_1') ,
    method : 'getRssiValue(callback)' ,
    api : testGetRssiValueCallback ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
  {
    name : $r('app.string.bluetoothtest_str_hqrzp_1') ,
    detail : $r('app.string.bluetoothtest_str_hqrzp_1') ,
    method : 'getRssiValue(promise)' ,
    api : testGetRssiValuePromise ,
    result : 'None' ,
    category : CaseCategory.GattClient
  },
]

const GattServerTestCase: BRTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_cjgfwq_1') ,
    detail : $r('app.string.bluetoothtest_str_cjgfwq_1') ,
    method : 'createGattServer' ,
    api : testCreateGattServer ,
    result : 'None' ,
    category : CaseCategory.GattServer
  },
  {
    name : $r('app.string.bluetoothtest_str_tjfw') ,
    detail : $r('app.string.bluetoothtest_str_tjgfw') ,
    method : 'addService' ,
    api : testAddService ,
    result : 'None' ,
    category : CaseCategory.GattServer
  },
  {
    name : $r('app.string.bluetoothtest_str_scfw') ,
    detail : $r('app.string.bluetoothtest_str_scgfw') ,
    method : 'removeService' ,
    api : testRemoveService ,
    result : 'None' ,
    category : CaseCategory.GattServer
  },
  {
    name : $r('app.string.bluetoothtest_str_ksgb') ,
    detail : $r('app.string.bluetoothtest_str_ksgbbb') ,
    method : 'startAdvertising' ,
    api : testStartAdvertising ,
    result : 'None' ,
    category : CaseCategory.GattServer
  },
  {
    name : $r('app.string.bluetoothtest_str_tzgb') ,
    detail : $r('app.string.bluetoothtest_str_tzgbbb') ,
    method : 'stopAdvertising' ,
    api : testStopAdvertising ,
    result : 'None' ,
    category : CaseCategory.GattServer
  },
  {
    name : $r('app.string.bluetoothtest_str_gb') ,
    detail : $r('app.string.bluetoothtest_str_gbgfwq') ,
    method : 'close' ,
    api : testGattServerClose ,
    result : 'None' ,
    category : CaseCategory.GattServer
  },
  {
    name : $r('app.string.bluetoothtest_str_ljzt') ,
    detail : $r('app.string.bluetoothtest_str_ckgljzt') ,
    method : 'on.connectStateChange' ,
    api : testOnConnectStateChange ,
    result : 'None' ,
    category : CaseCategory.GattServer
  },
  {
    name : $r('app.string.bluetoothtest_str_dqtzz') ,
    detail : $r('app.string.bluetoothtest_str_dqtzz') ,
    method : 'on.characteristicRead' ,
    api : testOnCharacteristicRead ,
    result : 'None' ,
    category : CaseCategory.GattServer
  },
  {
    name : $r('app.string.bluetoothtest_str_xrtzz') ,
    detail : $r('app.string.bluetoothtest_str_xrtzz') ,
    method : 'on.characteristicWrite' ,
    api : testOnCharacteristicWrite ,
    result : 'None' ,
    category : CaseCategory.GattServer
  },
  {
    name : $r('app.string.bluetoothtest_str_dqmsf') ,
    detail : $r('app.string.bluetoothtest_str_dqmsf') ,
    method : 'on.descriptorRead' ,
    api : testOnDescriptorRead ,
    result : 'None' ,
    category : CaseCategory.GattServer
  },
  {
    name : $r('app.string.bluetoothtest_str_xrmsf') ,
    detail : $r('app.string.bluetoothtest_str_xrmsf') ,
    method : 'on.descriptorWrite' ,
    api : testOnDescriptorWrite ,
    result : 'None' ,
    category : CaseCategory.GattServer
  },
]

const SppClientTestCase: BRTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : CaseCategory.SPPClient
  },
  {
    name : $r('app.string.bluetoothtest_str_gly') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetooth' ,
    api : testDisableBluetooth ,
    result : 'None' ,
    category : CaseCategory.SPPClient
  },
  {
    name : $r('app.string.bluetoothtest_str_szssmms') ,
    detail : $r('app.string.bluetoothtest_str_szssmms') ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : CaseCategory.SPPClient
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : CaseCategory.SPPClient
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmms') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : CaseCategory.SPPClient
  },
  {
    name : $r('app.string.bluetoothtest_str_hqdqljzt') ,
    detail : $r('app.string.bluetoothtest_str_hqdqlyljzt') ,
    method : 'getBtConnectionState' ,
    api : testGetBtConnectionState ,
    result : 'None' ,
    category : CaseCategory.SPPClient
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdljaqms') ,
    detail : $r('app.string.bluetoothtest_str_skhdaqmslj') ,
    method : 'sppConnect' ,
    api : testSppClientConnectSafe ,
    result : 'None' ,
    category : CaseCategory.SPPClient
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdljfaqms') ,
    detail : $r('app.string.bluetoothtest_str_skhdfaqmslj') ,
    method : 'sppConnect' ,
    api : testSppClientConnectUnSafe ,
    result : 'None' ,
    category : CaseCategory.SPPClient
  },
  {
    name : $r('app.string.bluetoothtest_str_pdsb') ,
    detail : $r('app.string.bluetoothtest_str_jxlysbpd') ,
    method : 'pairDevice' ,
    api : testPairDevice ,
    result : 'None' ,
    category : CaseCategory.SPPClient
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdxr') ,
    detail : $r('app.string.bluetoothtest_str_skhdxr') ,
    method : 'sppWrite' ,
    api : testSppClientWrite ,
    result : 'None' ,
    category : CaseCategory.SPPClient
  },
  {
    name : $r('app.string.bluetoothtest_str_skhddqdk') ,
    detail : $r('app.string.bluetoothtest_str_skhddqdk') ,
    method : 'on.sppRead' ,
    api : testSppClientReadOn ,
    result : 'None' ,
    category : CaseCategory.SPPClient
  },
  {
    name : $r('app.string.bluetoothtest_str_skhddqgb') ,
    detail : $r('app.string.bluetoothtest_str_skhddqgb') ,
    method : 'off.sppRead' ,
    api : testSppClientReadOff ,
    result : 'None' ,
    category : CaseCategory.SPPClient
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdgb') ,
    detail : $r('app.string.bluetoothtest_str_skhdgb') ,
    method : 'sppCloseClientSocket' ,
    api : testSppClientClose ,
    result : 'None' ,
    category : CaseCategory.SPPClient
  },
]

const SppServerTestCase: BRTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetooth' ,
    api : testEnableBluetooth ,
    result : 'None' ,
    category : CaseCategory.SPPServer
  },
  {
    name : $r('app.string.bluetoothtest_str_gly') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetooth' ,
    api : testDisableBluetooth ,
    result : 'None' ,
    category : CaseCategory.SPPServer
  },
  {
    name : $r('app.string.bluetoothtest_str_szssmms') ,
    detail : $r('app.string.bluetoothtest_str_szssmms') ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanMode ,
    result : 'None' ,
    category : CaseCategory.SPPServer
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getState' ,
    api : testGetState ,
    result : 'None' ,
    category : CaseCategory.SPPServer
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmms') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanMode ,
    result : 'None' ,
    category : CaseCategory.SPPServer
  },
  {
    name : $r('app.string.bluetoothtest_str_hqdqljzt') ,
    detail : $r('app.string.bluetoothtest_str_hqdqlyljzt') ,
    method : 'getBtConnectionState' ,
    api : testGetBtConnectionState ,
    result : 'None' ,
    category : CaseCategory.SPPServer
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqcjaqms') ,
    detail : $r('app.string.bluetoothtest_str_sfwqaqmscj') ,
    method : 'sppListen' ,
    api : testSppServerCreateSafe ,
    result : 'None' ,
    category : CaseCategory.SPPServer
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqcjfaqms') ,
    detail : $r('app.string.bluetoothtest_str_sfwqfaqmscj') ,
    method : 'sppListen' ,
    api : testSppServerCreateUnSafe ,
    result : 'None' ,
    category : CaseCategory.SPPServer
  },
  {
    name : $r('app.string.bluetoothtest_str_zcpmbh') ,
    detail : $r('app.string.bluetoothtest_str_zcpmbh') ,
    method : 'on.pinRequired' ,
    api : testOnPinRequired ,
    result : 'None' ,
    category : CaseCategory.SPPServer
  },
  //  { name: 'SPP客户端写入', detail: '使能蓝牙', method: 'spp_Client_Write', api: testSppClientWrite, result: 'None', category: CaseCategory.SPPServer },
  {
    name : $r('app.string.bluetoothtest_str_sfwqxr') ,
    detail : $r('app.string.bluetoothtest_str_sfwqxr') ,
    method : 'sppWrite' ,
    api : testSppServerWrite ,
    result : 'None' ,
    category : CaseCategory.SPPServer
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqjs') ,
    detail : $r('app.string.bluetoothtest_str_sfwqjs') ,
    method : 'sppAccept' ,
    api : testSppServerAccept ,
    result : 'None' ,
    category : CaseCategory.SPPServer
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqdqdk') ,
    detail : $r('app.string.bluetoothtest_str_sfwqdqdk') ,
    method : 'on.sppRead' ,
    api : testSppServerReadOn ,
    result : 'None' ,
    category : CaseCategory.SPPServer
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqdqgb') ,
    detail : $r('app.string.bluetoothtest_str_sfwqdqgb') ,
    method : 'off.sppRead' ,
    api : testSppServerReadOff ,
    result : 'None' ,
    category : CaseCategory.SPPServer
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqgb') ,
    detail : $r('app.string.bluetoothtest_str_sfwqgb') ,
    method : 'sppCloseServerSocket' ,
    api : testSppCloseServerSocket ,
    result : 'None' ,
    category : CaseCategory.SPPServer
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdgb') ,
    detail : $r('app.string.bluetoothtest_str_skhdgb') ,
    method : 'sppCloseClientSocket' ,
    api : testSppCloseClientSocket ,
    result : 'None' ,
    category : CaseCategory.SPPServer
  },
]

const A2dpSourceProfileTestCase: BRTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_lj') ,
    detail : $r('app.string.bluetoothtest_str_plj') ,
    method : 'connect' ,
    api : testProfileConnect ,
    result : 'None' ,
    category : CaseCategory.ProfileA2dpSource
  },
  {
    name : $r('app.string.bluetoothtest_str_qxlj') ,
    detail : $r('app.string.bluetoothtest_str_qxplj') ,
    method : 'disconnect' ,
    api : testProfileDisConnect ,
    result : 'None' ,
    category : CaseCategory.ProfileA2dpSource
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbfzt') ,
    detail : $r('app.string.bluetoothtest_str_hqpbfzt') ,
    method : 'getPlayingState' ,
    api : testProfileGetPlayingState ,
    result : 'None' ,
    category : CaseCategory.ProfileA2dpSource
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljsb') ,
    detail : $r('app.string.bluetoothtest_str_hqpljsb') ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevices ,
    result : 'None' ,
    category : CaseCategory.ProfileA2dpSource
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbzt') ,
    detail : $r('app.string.bluetoothtest_str_hqpsbzt') ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceState ,
    result : 'None' ,
    category : CaseCategory.ProfileA2dpSource
  },
]

const HandsFreeAudioGatewayProfileTestCase: BRTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_lj') ,
    detail : $r('app.string.bluetoothtest_str_plj') ,
    method : 'connect' ,
    api : testProfileConnect ,
    result : 'None' ,
    category : CaseCategory.ProfileHandsFreeAudioGateway
  },
  {
    name : $r('app.string.bluetoothtest_str_qxlj') ,
    detail : $r('app.string.bluetoothtest_str_qxplj') ,
    method : 'disconnect' ,
    api : testProfileDisConnect ,
    result : 'None' ,
    category : CaseCategory.ProfileHandsFreeAudioGateway
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljsb') ,
    detail : $r('app.string.bluetoothtest_str_hqpljsb') ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevices ,
    result : 'None' ,
    category : CaseCategory.ProfileHandsFreeAudioGateway ,
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbzt') ,
    detail : $r('app.string.bluetoothtest_str_hqpsbzt') ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceState ,
    result : 'None' ,
    category : CaseCategory.ProfileHandsFreeAudioGateway ,
  },
]

const HitHostProfileTestCase: BRTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_lj') ,
    detail : $r('app.string.bluetoothtest_str_plj') ,
    method : 'connect' ,
    api : testProfileConnect ,
    result : 'None' ,
    category : CaseCategory.ProfileHitHost
  },
  {
    name : $r('app.string.bluetoothtest_str_qxlj') ,
    detail : $r('app.string.bluetoothtest_str_qxplj') ,
    method : 'disconnect' ,
    api : testProfileDisConnect ,
    result : 'None' ,
    category : CaseCategory.ProfileHitHost
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljsb') ,
    detail : $r('app.string.bluetoothtest_str_hqpljsb') ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevices ,
    result : 'None' ,
    category : CaseCategory.ProfileHitHost
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbzt') ,
    detail : $r('app.string.bluetoothtest_str_hqpsbzt') ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceState ,
    result : 'None' ,
    category : CaseCategory.ProfileHitHost
  },
]

const PanNetworkProfileTestCase: BRTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_qxlj') ,
    detail : $r('app.string.bluetoothtest_str_qxplj') ,
    method : 'disconnect' ,
    api : testProfileDisConnect ,
    result : 'None' ,
    category : CaseCategory.ProfilePanNetwork
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljsb') ,
    detail : $r('app.string.bluetoothtest_str_hqpljsb') ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevices ,
    result : 'None' ,
    category : CaseCategory.ProfilePanNetwork
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbzt') ,
    detail : $r('app.string.bluetoothtest_str_hqpsbzt') ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceState ,
    result : 'None' ,
    category : CaseCategory.ProfilePanNetwork
  },
]

const OppProfileTestCase: BRTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_csztbhdk') ,
    detail : $r('app.string.bluetoothtest_str_csztggdk') ,
    method : 'on.transferStateChange' ,
    api : testOnTransferStateChange ,
    result : 'None' ,
    category : CaseCategory.ProfileOpp
  },
  {
    name : $r('app.string.bluetoothtest_str_jscrwjbhdk') ,
    detail : $r('app.string.bluetoothtest_str_jscrwjbhdk') ,
    method : 'on.receiveIncomingFile' ,
    api : testOnReceiveIncomingFile ,
    result : 'None' ,
    category : CaseCategory.ProfileOpp
  },
  // {
  //   name : '发送文件' ,
  //   detail : '发送文件' ,
  //   method : 'sendFile' ,
  //   api :  testSendFile ,
  //   result : 'None' ,
  //   category : CaseCategory.ProfileOpp
  // },
  {
    name : $r('app.string.bluetoothtest_str_szcrwjqr') ,
    detail : $r('app.string.bluetoothtest_str_szcrwjqr') ,
    method : 'setIncomingFileConfirmation' ,
    api : testSetIncomingFileConfirmation ,
    result : 'None' ,
    category : CaseCategory.ProfileOpp
  },
  {
    name : $r('app.string.bluetoothtest_str_hqdqcsxx') ,
    detail : $r('app.string.bluetoothtest_str_hqdqcsxx') ,
    method : 'getCurrentTransferInformation' ,
    api : testGetCurrentTransferInformation ,
    result : 'None' ,
    category : CaseCategory.ProfileOpp
  },
  {
    name : $r('app.string.bluetoothtest_str_qxcs_1') ,
    detail : $r('app.string.bluetoothtest_str_qxcs_1') ,
    method : 'cancelTransfer' ,
    api : testCancelTransfer ,
    result : 'None' ,
    category : CaseCategory.ProfileOpp
  },
]

const TestComposition: BRTestComposition[] = [
  {
    name : 'Classic' ,
    detail : $r('app.string.bluetoothtest_str_jdlycs') ,
    url : 'pages/subManualApiTest/BrInterfaceTest' ,
    image : $r('app.media.Bticon') ,
    category : Category.API
  },
  {
    name : 'BLE' ,
    detail : $r('app.string.bluetoothtest_str_dghlycs') ,
    url : 'pages/subManualApiTest/bleTest' ,
    image : $r('app.media.ble40') ,
    category : Category.API
  },
  {
    name : 'Gatt' ,
    detail : $r('app.string.bluetoothtest_str_gcs_2') ,
    url : 'pages/subManualApiTest/gattTest' ,
    image : $r('app.media.Gatt') ,
    category : Category.API
  },
  {
    name : 'SPP' ,
    detail : $r('app.string.bluetoothtest_str_scs_1') ,
    url : 'pages/subManualApiTest/sppTest' ,
    image : $r('app.media.spp') ,
    category : Category.API
  },

  {
    name : 'Classic_M' ,
    detail : $r('app.string.bluetoothtest_str_jdlymcs') ,
    url : 'pages/subManualApiTestManager/brTestManager' ,
    image : $r('app.media.Bticon') ,
    category : Category.APIManager
  },
  {
    name : 'BLE_M' ,
    detail : $r('app.string.bluetoothtest_str_dghlymcs') ,
    url : 'pages/subManualApiTestManager/bleTestManager' ,
    image : $r('app.media.ble40') ,
    category : Category.APIManager
  },
  {
    name : 'Gatt_M' ,
    detail : $r('app.string.bluetoothtest_str_gcs_1') ,
    url : 'pages/subManualApiTestManager/gattTestManager' ,
    image : $r('app.media.Gatt') ,
    category : Category.APIManager
  },
  {
    name : 'SPP_M' ,
    detail : $r('app.string.bluetoothtest_str_scs') ,
    url : 'pages/subManualApiTestManager/sppTestManager' ,
    image : $r('app.media.spp') ,
    category : Category.APIManager
  },

  {
    name : 'BrAutoTest' ,
    detail : $r('app.string.bluetoothtest_str_bzdcs_3') ,
    url : 'pages/subAutoTest/brAutoTest' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.AutoTest
  },
  {
    name : 'BleAutoTest' ,
    detail : $r('app.string.bluetoothtest_str_bzdcs_2') ,
    url : 'pages/subAutoTest/bleAutoTest' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.AutoTest
  },
  {
    name : 'GattAutoTest' ,
    detail : $r('app.string.bluetoothtest_str_gzdcs_5') ,
    url : 'pages/subAutoTest/gattAutoTest' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.AutoTest
  },
  {
    name : 'SppAutoTest' ,
    detail : $r('app.string.bluetoothtest_str_szdcs_4') ,
    url : 'pages/subAutoTest/sppAutoTest' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.AutoTest
  },
  {
    name : 'ProfileAutoTest' ,
    detail : $r('app.string.bluetoothtest_str_pzdcs_3') ,
    url : 'pages/subAutoTest/profileAutoTest' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.AutoTest
  },

  {
    name : 'Br_M_AutoTest' ,
    detail : $r('app.string.bluetoothtest_str_bzdcs_1') ,
    url : 'pages/subAutoTestManager/brAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.AutoTestManager
  },
  {
    name : 'Ble_M_AutoTest' ,
    detail : $r('app.string.bluetoothtest_str_bzdcs') ,
    url : 'pages/subAutoTestManager/bleAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.AutoTestManager
  },
  {
    name : 'Gatt_M_AutoTest' ,
    detail : $r('app.string.bluetoothtest_str_gzdcs_2') ,
    url : 'pages/subAutoTestManager/gattAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.AutoTestManager
  },
  {
    name : 'Spp_M_AutoTest' ,
    detail : $r('app.string.bluetoothtest_str_szdcs_1') ,
    url : 'pages/subAutoTestManager/sppAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.AutoTestManager
  },
  {
    name : 'Profile_M_AutoTest' ,
    detail : $r('app.string.bluetoothtest_str_pzdcs_1') ,
    url : 'pages/subAutoTestManager/profileAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.AutoTestManager
  },


  {
    name : 'BrFoundScenarioTest' ,
    detail : $r('app.string.bluetoothtest_str_jdlyfxcjcs') ,
    url : 'pages/subScenarioTest/brFoundScenarioTest' ,
    image : $r('app.media.ScenarioTest') ,
    category : Category.Scenario
  },
  {
    name : 'BleFoundScenarioTest' ,
    detail : $r('app.string.bluetoothtest_str_blyfxcjcs') ,
    url : 'pages/subScenarioTest/bleFoundScenarioTest' ,
    image : $r('app.media.ScenarioTest') ,
    category : Category.Scenario
  },
  {
    name : 'GattClientScenarioTest' ,
    detail : $r('app.string.bluetoothtest_str_gcjcs_2') ,
    url : 'pages/subScenarioTest/gattClientScenarioTest' ,
    image : $r('app.media.ScenarioTest') ,
    category : Category.Scenario
  },
  {
    name : 'GattServerScenarioTest' ,
    detail : $r('app.string.bluetoothtest_str_gcjcs_3') ,
    url : 'pages/subScenarioTest/gattServerScenarioTest' ,
    image : $r('app.media.ScenarioTest') ,
    category : Category.Scenario
  },
  {
    name : 'SppClientScenarioTest' ,
    detail : $r('app.string.bluetoothtest_str_scjcs_2') ,
    url : 'pages/subScenarioTest/sppClientScenarioTest' ,
    image : $r('app.media.ScenarioTest') ,
    category : Category.Scenario
  },
  {
    name : 'SppServerScenarioTest' ,
    detail : $r('app.string.bluetoothtest_str_scjcs_3') ,
    url : 'pages/subScenarioTest/sppServerScenarioTest' ,
    image : $r('app.media.ScenarioTest') ,
    category : Category.Scenario
  },
  {
    name : 'OppSendScenarioTest' ,
    detail : $r('app.string.bluetoothtest_str_ofsdcjcs') ,
    url : 'pages/subScenarioTest/oppSendScenarioTest' ,
    image : $r('app.media.ScenarioTest') ,
    category : Category.Scenario
  },
  {
    name : 'OppRecvScenarioTest' ,
    detail : $r('app.string.bluetoothtest_str_ojsdcjcs') ,
    url : 'pages/subScenarioTest/oppRecvScenarioTest' ,
    image : $r('app.media.ScenarioTest') ,
    category : Category.Scenario
  },

  {
    name : 'BrFound_M_ScenarioTest' ,
    detail : $r('app.string.bluetoothtest_str_jdlymfxcjcs') ,
    url : 'pages/subScenarioTestManager/brFoundScenarioTestManager' ,
    image : $r('app.media.ScenarioTest') ,
    category : Category.ScenarioManager
  },
  {
    name : 'BleFound_M_ScenarioTest' ,
    detail : $r('app.string.bluetoothtest_str_blymfxcjcs') ,
    url : 'pages/subScenarioTestManager/bleFoundScenarioTestManager' ,
    image : $r('app.media.ScenarioTest') ,
    category : Category.ScenarioManager
  },
  {
    name : 'GattClient_M_ScenarioTest' ,
    detail : $r('app.string.bluetoothtest_str_gcjcs') ,
    url : 'pages/subScenarioTestManager/gattClientScenarioTestManager' ,
    image : $r('app.media.ScenarioTest') ,
    category : Category.ScenarioManager
  },
  {
    name : 'GattServer_M_ScenarioTest' ,
    detail : $r('app.string.bluetoothtest_str_gcjcs_1') ,
    url : 'pages/subScenarioTestManager/gattServerScenarioTestManager' ,
    image : $r('app.media.ScenarioTest') ,
    category : Category.ScenarioManager
  },
  {
    name : 'SppClient_M_ScenarioTest' ,
    detail : $r('app.string.bluetoothtest_str_scjcs') ,
    url : 'pages/subScenarioTestManager/sppClientScenarioTestManager' ,
    image : $r('app.media.ScenarioTest') ,
    category : Category.ScenarioManager
  },
  {
    name : 'SppServer_M_ScenarioTest' ,
    detail : $r('app.string.bluetoothtest_str_scjcs_1') ,
    url : 'pages/subScenarioTestManager/sppServerScenarioTestManager' ,
    image : $r('app.media.ScenarioTest') ,
    category : Category.ScenarioManager
  },

  {
    name : 'A2dpSource' ,
    detail : $r('app.string.bluetoothtest_str_apzcs_1') ,
    url : 'pages/subProfileTest/a2dpSourceProfileTest' ,
    image : $r('app.media.profile') ,
    category : Category.Profile
  },
  {
    name : 'HFAudioGateway' ,
    detail : $r('app.string.bluetoothtest_str_hpzcs_1') ,
    url : 'pages/subProfileTest/handsFreeAudioGatewayProfileTest' ,
    image : $r('app.media.profile') ,
    category : Category.Profile
  },
  {
    name : 'HidHost' ,
    detail : $r('app.string.bluetoothtest_str_hpzcs_3') ,
    url : 'pages/subProfileTest/hidHostProfileTest' ,
    image : $r('app.media.profile') ,
    category : Category.Profile
  },
  {
    name : 'PanNetwork' ,
    detail : $r('app.string.bluetoothtest_str_ppzcs_1') ,
    url : 'pages/subProfileTest/panNetworkProfileTest' ,
    image : $r('app.media.profile') ,
    category : Category.Profile
  },
  {
    name : 'Opp' ,
    detail : $r('app.string.bluetoothtest_str_opzcs') ,
    url : 'pages/subProfileTest/oppProfileTest' ,
    image : $r('app.media.profile') ,
    category : Category.Profile
  },

  {
    name : 'A2dpSource_M' ,
    detail : $r('app.string.bluetoothtest_str_apzcs') ,
    url : 'pages/subProfileTestManager/a2dpSourceProfileTestManager' ,
    image : $r('app.media.profile') ,
    category : Category.ProfileManager
  },
  {
    name : 'HFAudioGateway_M' ,
    detail : $r('app.string.bluetoothtest_str_hpzcs') ,
    url : 'pages/subProfileTestManager/handsFreeAudioGatewayProfileTestManager' ,
    image : $r('app.media.profile') ,
    category : Category.ProfileManager
  },
  {
    name : 'HidHost_M' ,
    detail : $r('app.string.bluetoothtest_str_hpzcs_2') ,
    url : 'pages/subProfileTestManager/hidHostProfileTestManager' ,
    image : $r('app.media.profile') ,
    category : Category.ProfileManager
  },
  {
    name : 'PanNetwork_M' ,
    detail : $r('app.string.bluetoothtest_str_ppzcs') ,
    url : 'pages/subProfileTestManager/panNetworkProfileTestManager' ,
    image : $r('app.media.profile') ,
    category : Category.ProfileManager
  },

  {
    name : 'BrBenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_jdlyxncs') ,
    url : 'pages/subBenchmarkTest/brBenchmarkTest' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMark
  },
  {
    name : 'BleBenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_blyxncs') ,
    url : 'pages/subBenchmarkTest/bleBenchmarkTest' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMark
  },
  {
    name : 'GattClientVelocityBenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_gslxncs') ,
    url : 'pages/subBenchmarkTest/gattClientVelocityBenchmarkTest' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMark
  },
  {
    name : 'GattServerVelocityBenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_gslxncs_1') ,
    url : 'pages/subBenchmarkTest/gattServerVelocityBenchmarkTest' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMark
  },
  {
    name : 'SppClientVelocityBenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_sslxncs') ,
    url : 'pages/subBenchmarkTest/sppClientVelocityBenchmarkTest' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMark
  },
  {
    name : 'SppServerVelocityBenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_sslxncs_1') ,
    url : 'pages/subBenchmarkTest/sppServerVelocityBenchmarkTest' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMark
  },
  {
    name : 'SppClientBandWidthBenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_sdkxncs') ,
    url : 'pages/subBenchmarkTest/sppClientBandWidthBenchmarkTest' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMark
  },
  {
    name : 'SppServerBandWidthBenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_sdkxncs_1') ,
    url : 'pages/subBenchmarkTest/sppServerBandWidthBenchmarkTest' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMark
  },

  {
    name : 'Br_M_BenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_jdlyxncs') ,
    url : 'pages/subBenchmarkTestManager/brBenchmarkTestManager' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMarkManager
  },
  {
    name : 'Ble_M_BenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_blyxncs') ,
    url : 'pages/subBenchmarkTestManager/bleBenchmarkTestManager' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMarkManager
  },
  {
    name : 'GattClient_M_VelocityBenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_gslxncs') ,
    url : 'pages/subBenchmarkTestManager/gattClientVelocityBenchmarkTestManager' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMarkManager
  },
  {
    name : 'GattServer_M_VelocityBenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_gslxncs_1') ,
    url : 'pages/subBenchmarkTestManager/gattServerVelocityBenchmarkTestManager' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMarkManager
  },
  {
    name : 'SppClient_M_VelocityBenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_sslxncs') ,
    url : 'pages/subBenchmarkTestManager/sppClientVelocityBenchmarkTestManager' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMarkManager
  },
  {
    name : 'SppServer_M_VelocityBenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_sslxncs_1') ,
    url : 'pages/subBenchmarkTestManager/sppServerVelocityBenchmarkTestManager' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMarkManager
  },
  {
    name : 'SppClient_M_BandWidthBenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_sdkxncs') ,
    url : 'pages/subBenchmarkTestManager/sppClientBandWidthBenchmarkTestManager' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMarkManager
  },
  {
    name : 'SppServer_M_BandWidthBenchmarkTest' ,
    detail : $r('app.string.bluetoothtest_str_sdkxncs_1') ,
    url : 'pages/subBenchmarkTestManager/sppServerBandWidthBenchmarkTestManager' ,
    image : $r('app.media.Benchmark') ,
    category : Category.BenchMarkManager
  }
]

const GattAutoTestComposition: BRTestComposition[] = [
  {
    name : 'GattClientAutoTest' ,
    detail : $r('app.string.bluetoothtest_str_gzdcs_3') ,
    url : 'pages/subAutoTest/subGattAutoTest/gattClientAutoTest' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.GattAutoTest
  },
  {
    name : 'GattServerAutoTest' ,
    detail : $r('app.string.bluetoothtest_str_gzdcs_4') ,
    url : 'pages/subAutoTest/subGattAutoTest/gattServerAutoTest' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.GattAutoTest
  },
]

const SppAutoTestComposition: BRTestComposition[] = [
  {
    name : 'SppClientAutoTest' ,
    detail : $r('app.string.bluetoothtest_str_szdcs_2') ,
    url : 'pages/subAutoTest/subSppAutoTest/sppClientAutoTest' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.SppAutoTest
  },
  {
    name : 'SppServerAutoTest' ,
    detail : $r('app.string.bluetoothtest_str_szdcs_3') ,
    url : 'pages/subAutoTest/subSppAutoTest/sppServerAutoTest' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.SppAutoTest
  },
]

const ProfileAutoTestComposition: BRTestComposition[] = [
  {
    name : 'A2dpSourceAutoTest' ,
    detail : $r('app.string.bluetoothtest_str_azdcs_1') ,
    url : 'pages/subAutoTest/subProfileAutoTest/a2dpSourceAutoTest' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.ProfileAutoTest
  },
  {
    name : 'HFAudioGatewayAutoTest' ,
    detail : $r('app.string.bluetoothtest_str_hzdcs_1') ,
    url : 'pages/subAutoTest/subProfileAutoTest/handsFreeAudioGatewayAutoTest' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.ProfileAutoTest
  },
  {
    name : 'HitHostAutoTest' ,
    detail : $r('app.string.bluetoothtest_str_hzdcs_3') ,
    url : 'pages/subAutoTest/subProfileAutoTest/hitHostAutoTest' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.ProfileAutoTest
  },
  {
    name : 'PanNetworkAutoTest' ,
    detail : $r('app.string.bluetoothtest_str_pzdcs_2') ,
    url : 'pages/subAutoTest/subProfileAutoTest/panNetworkAutoTest' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.ProfileAutoTest
  },
]

export function initTestData(): Array<TestData> {
  let TestDataArray: Array<TestData> = []
  TestComposition.forEach(item => {
    TestDataArray.push(new TestData(item.name , item.detail , item.url , item.image , item.category));
  })
  return TestDataArray;
}

export function initBRApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  BRTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initBleApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  BleTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initGattApiTestData(): Array<TestData> {
  let TestDataArray: Array<TestData> = []
  GattTestComposition.forEach(item => {
    TestDataArray.push(new TestData(item.name , item.detail , item.url , item.image , item.category));
  })
  return TestDataArray;
}

export function initSppApiTestData(): Array<TestData> {
  let TestDataArray: Array<TestData> = []
  SppTestComposition.forEach(item => {
    TestDataArray.push(new TestData(item.name , item.detail , item.url , item.image , item.category));
  })
  return TestDataArray;
}


export function initGattClientApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  GattClientTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initGattServerApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  GattServerTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initSppClientApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  SppClientTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initSppServerApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  SppServerTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initA2dpSourceProfileApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  A2dpSourceProfileTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initHandsFreeAudioGatewayProfileApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  HandsFreeAudioGatewayProfileTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initHitHostProfileApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  HitHostProfileTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initPanNetworkProfileApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  PanNetworkProfileTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initOppProfileApiData(): Array<TestApi> {
  let TestApiArray: Array<TestApi> = []
  OppProfileTestCase.forEach(item => {
    TestApiArray.push(new TestApi(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestApiArray;
}

export function initGattAutoTestData(): Array<TestData> {
  let TestDataArray: Array<TestData> = []
  GattAutoTestComposition.forEach(item => {
    TestDataArray.push(new TestData(item.name , item.detail , item.url , item.image , item.category));
  })
  return TestDataArray;
}

export function initSppAutoTestData(): Array<TestData> {
  let TestDataArray: Array<TestData> = []
  SppAutoTestComposition.forEach(item => {
    TestDataArray.push(new TestData(item.name , item.detail , item.url , item.image , item.category));
  })
  return TestDataArray;
}

export function initProfileAutoTestData(): Array<TestData> {
  let TestDataArray: Array<TestData> = []
  ProfileAutoTestComposition.forEach(item => {
    TestDataArray.push(new TestData(item.name , item.detail , item.url , item.image , item.category));
  })
  return TestDataArray;
}