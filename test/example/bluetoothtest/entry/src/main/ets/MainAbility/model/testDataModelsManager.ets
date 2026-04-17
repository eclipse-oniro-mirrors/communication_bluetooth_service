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
    name : $r('app.string.bluetoothtest_str_klym') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_glym') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetoothManager' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqztm') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqdqljztm') ,
    detail : $r('app.string.bluetoothtest_str_hqdqlyljzt') ,
    method : 'getBtConnectionStateManager' ,
    api : testGetBtConnectionStateManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalNameManager' ,
    api : testSetLocalNameManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalNameManager' ,
    api : testGetLocalNameManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_szlysmms') ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_kslyfxm') ,
    detail : $r('app.string.bluetoothtest_str_fxkpdlysb') ,
    method : 'startBluetoothDiscoveryManager' ,
    api : testStartBluetoothDiscoveryManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_tzlyfxm') ,
    detail : $r('app.string.bluetoothtest_str_tzfx') ,
    method : 'stopBluetoothDiscoveryManager' ,
    api : testStopBluetoothDiscoveryManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_pdsbm') ,
    detail : $r('app.string.bluetoothtest_str_jxlysbpd') ,
    method : 'pairDeviceManager' ,
    api : testPairDeviceManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_qxpdsbm') ,
    detail : $r('app.string.bluetoothtest_str_qxlysbpd') ,
    method : 'unpairDeviceManager' ,
    api : testUnPairedDeviceManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqycsbmcm') ,
    detail : $r('app.string.bluetoothtest_str_hqpdlysbdmc') ,
    method : 'getDeviceNameManager' ,
    api : testGetRemoteDeviceNameManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqycsblxm') ,
    detail : $r('app.string.bluetoothtest_str_hqpdlysbdlx') ,
    method : 'getDeviceTypeManager' ,
    api : testGetRemoteDeviceClassManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqpdsbm') ,
    detail : $r('app.string.bluetoothtest_str_hqpdlysbdxx') ,
    method : 'getPairedDeviceIdsManager' ,
    api : testGetPairedDevicesManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqpzwjljztm') ,
    detail : $r('app.string.bluetoothtest_str_hqpzwjdljzt') ,
    method : 'getProfileConnStateManager' ,
    api : testGetProfileConnStateManager1 ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqpzwjljztm_1') ,
    detail : $r('app.string.bluetoothtest_str_hqsbdpzwjzt') ,
    method : 'getDeviceStateManager' ,
    api : testGetProfileConnStateManager2 ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_zcztbhm') ,
    detail : $r('app.string.bluetoothtest_str_zcztbh') ,
    method : 'on.stateChangeManager' ,
    api : testOnStateChangeManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_zcpmbhm') ,
    detail : $r('app.string.bluetoothtest_str_zcpmbh') ,
    method : 'on.pinRequiredManager' ,
    api : testOnPinRequiredManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  },
  {
    name : $r('app.string.bluetoothtest_str_zcbdztbhm') ,
    detail : $r('app.string.bluetoothtest_str_zcbdztbh') ,
    method : 'on.bondStateChangeManager' ,
    api : testOnBondStateChangeManager ,
    result : 'None' ,
    category : CaseCategory.BRManager
  }
]

const BleManagerTestCase: BRManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_cjgfwqm') ,
    detail : $r('app.string.bluetoothtest_str_cjgfwq') ,
    method : 'createGattServerManager' ,
    api : testCreateGattServerDeviceManager ,
    result : 'None' ,
    category : CaseCategory.BLEManager
  },
  {
    name : $r('app.string.bluetoothtest_str_cjgkhdm') ,
    detail : $r('app.string.bluetoothtest_str_cjgkhd') ,
    method : 'createGattClientManager' ,
    api : testCreateGattClientDeviceManager ,
    result : 'None' ,
    category : CaseCategory.BLEManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljdbsbm') ,
    detail : $r('app.string.bluetoothtest_str_hqljdbsbmxx') ,
    method : 'getConnectedBLEDevicesManager' ,
    api : testGetConnectedBLEDevicesManager ,
    result : 'None' ,
    category : CaseCategory.BLEManager
  },
  {
    name : $r('app.string.bluetoothtest_str_ksbsmm') ,
    detail : $r('app.string.bluetoothtest_str_kssmbsb') ,
    method : 'startBLEScanManager' ,
    api : testStartBLEScanManager ,
    result : 'None' ,
    category : CaseCategory.BLEManager
  },
  {
    name : $r('app.string.bluetoothtest_str_tzbsmm') ,
    detail : $r('app.string.bluetoothtest_str_tzmsm') ,
    method : 'stopBLEScanManager' ,
    api : testStopBLEScanManager ,
    result : 'None' ,
    category : CaseCategory.BLEManager
  },
  {
    name : $r('app.string.bluetoothtest_str_zcbczztbhm') ,
    detail : $r('app.string.bluetoothtest_str_zcvsbczztbhm') ,
    method : 'on.BLEDeviceFindManager' ,
    api : testOnBLEDeviceFindManager ,
    result : 'None' ,
    category : CaseCategory.BLEManager
  }
]

const GattManagerTestComposition: BRManagerTestComposition[] = [
  {
    name : 'GattClientManager' ,
    detail : $r('app.string.bluetoothtest_str_gkhdmcs') ,
    url : 'pages/subManualApiTestManager/subGattTestManager/gattClientManager' ,
    image : $r('app.media.Gatt') ,
    category : Category.GattManager
  },
  {
    name : 'GattServerManager' ,
    detail : $r('app.string.bluetoothtest_str_gfwqmcs') ,
    url : 'pages/subManualApiTestManager/subGattTestManager/gattServerManager' ,
    image : $r('app.media.Gatt') ,
    category : Category.GattManager
  },
]

const SppManagerTestComposition: BRManagerTestComposition[] = [
  {
    name : 'SppClientManager' ,
    detail : $r('app.string.bluetoothtest_str_skhdmcs') ,
    url : 'pages/subManualApiTestManager/subSppTestManager/sppClientManager' ,
    image : $r('app.media.spp') ,
    category : Category.SPPManager
  },
  {
    name : 'SppServerManager' ,
    detail : $r('app.string.bluetoothtest_str_sfwqmcs') ,
    url : 'pages/subManualApiTestManager/subSppTestManager/sppServerManager' ,
    image : $r('app.media.spp') ,
    category : Category.SPPManager
  },
]

const GattClientManagerTestCase: BRManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_czgkhdm') ,
    detail : $r('app.string.bluetoothtest_str_czgkhd') ,
    method : 'createGattClientManager' ,
    api : testCreateGattClientManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_ljm') ,
    detail : $r('app.string.bluetoothtest_str_glj') ,
    method : 'connectManager' ,
    api : testConnectManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_qxljm') ,
    detail : $r('app.string.bluetoothtest_str_qxglj') ,
    method : 'disconnectManager' ,
    api : testDisconnectManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_bljztm') ,
    detail : $r('app.string.bluetoothtest_str_ckbljzt') ,
    method : 'on.BLEConnectionStateChangeManager' ,
    api : testOnBLEConnectionStateChangeManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_gbm') ,
    detail : $r('app.string.bluetoothtest_str_gbgkhd') ,
    method : 'closeManager' ,
    api : testGattClientCloseManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqfwc') ,
    detail : $r('app.string.bluetoothtest_str_hqfwc_1') ,
    method : 'getServices(callback)Manager' ,
    api : testGetServicesCallbackManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqfwp') ,
    detail : $r('app.string.bluetoothtest_str_hqfwp_1') ,
    method : 'getServices(promise)Manager' ,
    api : testGetServicesPromiseManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_dqtzzc') ,
    detail : $r('app.string.bluetoothtest_str_dqtzzc_1') ,
    method : 'readCharacteristicValue(callback)Manager' ,
    api : testReadCharacteristicValueCallbackManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_dqtzzp') ,
    detail : $r('app.string.bluetoothtest_str_dqtzzp_1') ,
    method : 'readCharacteristicValue(promise)Manager' ,
    api : testReadCharacteristicValuePromiseManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_dqmsfzc') ,
    detail : $r('app.string.bluetoothtest_str_dqmsfzc_1') ,
    method : 'readDescriptorValue(callback)Manager' ,
    api : testReadDescriptorValueCallbackManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_dqmsfzp') ,
    detail : $r('app.string.bluetoothtest_str_dqmsfzp_1') ,
    method : 'readDescriptorValue(promise)Manager' ,
    api : testReadDescriptorValuePromiseManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_xrtzzm') ,
    detail : $r('app.string.bluetoothtest_str_xrtzz') ,
    method : 'writeCharacteristicValueManager' ,
    api : testWriteCharacteristicValueManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_xrmsfzm') ,
    detail : $r('app.string.bluetoothtest_str_xrmsfz') ,
    method : 'writeDescriptorValueManager' ,
    api : testWriteDescriptorValueManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_btzzbhm') ,
    detail : $r('app.string.bluetoothtest_str_ckbtzzbh') ,
    method : 'on.BLECharacteristicChangeManager' ,
    api : testOnBLECharacteristicChangeManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sdbccm') ,
    detail : $r('app.string.bluetoothtest_str_sdbcc') ,
    method : 'setBLEMtuSizeManager' ,
    api : testSetBLEMtuSizeManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sztztxggm') ,
    detail : $r('app.string.bluetoothtest_str_sztztxgg') ,
    method : 'setNotifyCharacteristicChangedManager' ,
    api : testSetNotifyCharacteristicChangedManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbmcc') ,
    detail : $r('app.string.bluetoothtest_str_hqsbmcc_1') ,
    method : 'getDeviceName(callback)Manager' ,
    api : testGetDeviceNameCallbackManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbmcp') ,
    detail : $r('app.string.bluetoothtest_str_hqsbmcp_1') ,
    method : 'getDeviceName(promise)Manager' ,
    api : testGetDeviceNamePromiseManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqrzc') ,
    detail : $r('app.string.bluetoothtest_str_hqrzc_1') ,
    method : 'getRssiValue(callback)Manager' ,
    api : testGetRssiValueCallbackManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqrzp') ,
    detail : $r('app.string.bluetoothtest_str_hqrzp_1') ,
    method : 'getRssiValue(promise)Manager' ,
    api : testGetRssiValuePromiseManager ,
    result : 'None' ,
    category : CaseCategory.GattClientManager
  },
]

const GattServerManagerTestCase: BRManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_cjgfwqm') ,
    detail : $r('app.string.bluetoothtest_str_cjgfwq_1') ,
    method : 'createGattServerManager' ,
    api : testCreateGattServerManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_tjfwm') ,
    detail : $r('app.string.bluetoothtest_str_tjgfw') ,
    method : 'addServiceManager' ,
    api : testAddServiceManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_scfwm') ,
    detail : $r('app.string.bluetoothtest_str_scgfw') ,
    method : 'removeServiceManager' ,
    api : testRemoveServiceManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_ksgbm') ,
    detail : $r('app.string.bluetoothtest_str_ksgbbb') ,
    method : 'startAdvertisingManager' ,
    api : testStartAdvertisingManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_tzgbm') ,
    detail : $r('app.string.bluetoothtest_str_tzgbbb') ,
    method : 'stopAdvertisingManager' ,
    api : testStopAdvertisingManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_gbm') ,
    detail : $r('app.string.bluetoothtest_str_gbgfwq') ,
    method : 'closeManager' ,
    api : testGattServerCloseManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_ljztm') ,
    detail : $r('app.string.bluetoothtest_str_ckgljzt') ,
    method : 'on.connectStateChangeManager' ,
    api : testOnConnectStateChangeManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_dqtzzm') ,
    detail : $r('app.string.bluetoothtest_str_dqtzz') ,
    method : 'on.characteristicReadManager' ,
    api : testOnCharacteristicReadManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_xrtzzm') ,
    detail : $r('app.string.bluetoothtest_str_xrtzz') ,
    method : 'on.characteristicWriteManager' ,
    api : testOnCharacteristicWriteManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_dqmsfm') ,
    detail : $r('app.string.bluetoothtest_str_dqmsf') ,
    method : 'on.descriptorRead' ,
    api : testOnDescriptorReadManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_xrmsfm') ,
    detail : $r('app.string.bluetoothtest_str_xrmsf') ,
    method : 'on.descriptorWrite' ,
    api : testOnDescriptorWriteManager ,
    result : 'None' ,
    category : CaseCategory.GattServerManager
  },
]

const SppClientManagerTestCase: BRManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_klym') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_glym') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetoothManager' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szssmmsm') ,
    detail : $r('app.string.bluetoothtest_str_szssmms') ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqztm') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqdqljztm') ,
    detail : $r('app.string.bluetoothtest_str_hqdqlyljzt') ,
    method : 'getBtConnectionStateManager' ,
    api : testGetBtConnectionStateManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdljaqmsm') ,
    detail : $r('app.string.bluetoothtest_str_skhdaqmslj') ,
    method : 'sppConnectManager' ,
    api : testSppClientConnectSafeManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdljfaqmsm') ,
    detail : $r('app.string.bluetoothtest_str_skhdfaqmslj') ,
    method : 'sppConnectManager' ,
    api : testSppClientConnectUnSafeManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_pdsbm') ,
    detail : $r('app.string.bluetoothtest_str_jxlysbpd') ,
    method : 'pairDeviceManager' ,
    api : testPairDeviceManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdxrm') ,
    detail : $r('app.string.bluetoothtest_str_skhdxr') ,
    method : 'sppWriteManager' ,
    api : testSppClientWriteManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_skhddqdkm') ,
    detail : $r('app.string.bluetoothtest_str_skhddqdk') ,
    method : 'on.sppReadManager' ,
    api : testSppClientReadOnManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_skhddqgbm') ,
    detail : $r('app.string.bluetoothtest_str_skhddqgb') ,
    method : 'off.sppReadManager' ,
    api : testSppClientReadOffManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdgbm') ,
    detail : $r('app.string.bluetoothtest_str_skhdgb') ,
    method : 'sppCloseClientSocketManager' ,
    api : testSppClientCloseManager ,
    result : 'None' ,
    category : CaseCategory.SPPClientManager
  },
]

const SppServerManagerTestCase: BRManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_klym') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_glym') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetoothManager' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szssmmsm') ,
    detail : $r('app.string.bluetoothtest_str_szssmms') ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqztm') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqdqljztm') ,
    detail : $r('app.string.bluetoothtest_str_hqdqlyljzt') ,
    method : 'getBtConnectionStateManager' ,
    api : testGetBtConnectionStateManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqcjaqmsm') ,
    detail : $r('app.string.bluetoothtest_str_sfwqaqmscj') ,
    method : 'sppListenManager' ,
    api : testSppServerCreateSafeManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqcjfaqmsm') ,
    detail : $r('app.string.bluetoothtest_str_sfwqfaqmscj') ,
    method : 'sppListenManager' ,
    api : testSppServerCreateUnSafeManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_zcpmbhm') ,
    detail : $r('app.string.bluetoothtest_str_zcpmbh') ,
    method : 'on.pinRequiredManager' ,
    api : testOnPinRequiredManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  //  { name: 'SPP客户端写入', detail: '使能蓝牙', method: 'spp_Client_Write', api: testSppClientWriteManager, result: 'None', category: CaseCategory.SPPServer },
  {
    name : $r('app.string.bluetoothtest_str_sfwqxrm') ,
    detail : $r('app.string.bluetoothtest_str_sfwqxr') ,
    method : 'sppWriteManager' ,
    api : testSppServerWriteManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqjsm') ,
    detail : $r('app.string.bluetoothtest_str_sfwqjs') ,
    method : 'sppAcceptManager' ,
    api : testSppServerAcceptManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqdqdkm') ,
    detail : $r('app.string.bluetoothtest_str_sfwqdqdk') ,
    method : 'on.sppReadManager' ,
    api : testSppServerReadOnManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqdqgbm') ,
    detail : $r('app.string.bluetoothtest_str_sfwqdqgb') ,
    method : 'off.sppReadManager' ,
    api : testSppServerReadOffManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqgbm') ,
    detail : $r('app.string.bluetoothtest_str_sfwqgb') ,
    method : 'sppCloseServerSocketManager' ,
    api : testSppCloseServerSocketManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdgbm') ,
    detail : $r('app.string.bluetoothtest_str_skhdgb') ,
    method : 'sppCloseClientSocketManager' ,
    api : testSppCloseClientSocketManager ,
    result : 'None' ,
    category : CaseCategory.SPPServerManager
  },
]

const A2dpSourceProfileManagerTestCase: BRManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_ljm') ,
    detail : $r('app.string.bluetoothtest_str_pljm') ,
    method : 'connect' ,
    api : testProfileConnectManager ,
    result : 'None' ,
    category : CaseCategory.ProfileA2dpSourceManager
  },
  {
    name : $r('app.string.bluetoothtest_str_qxljm') ,
    detail : $r('app.string.bluetoothtest_str_qxpljm') ,
    method : 'disconnect' ,
    api : testProfileDisConnectManager ,
    result : 'None' ,
    category : CaseCategory.ProfileA2dpSourceManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbfztm') ,
    detail : $r('app.string.bluetoothtest_str_hqpbfztm') ,
    method : 'getPlayingState' ,
    api : testProfileGetPlayingStateManager ,
    result : 'None' ,
    category : CaseCategory.ProfileA2dpSourceManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljsbm') ,
    detail : $r('app.string.bluetoothtest_str_hqpljsbm') ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevicesManager ,
    result : 'None' ,
    category : CaseCategory.ProfileA2dpSourceManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbztm') ,
    detail : $r('app.string.bluetoothtest_str_hqpsbztm') ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceStateManager ,
    result : 'None' ,
    category : CaseCategory.ProfileA2dpSourceManager
  },
]

const HandsFreeAudioGatewayProfileManagerTestCase: BRManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_ljm') ,
    detail : $r('app.string.bluetoothtest_str_pljm') ,
    method : 'connect' ,
    api : testProfileConnectManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHandsFreeAudioGatewayManager
  },
  {
    name : $r('app.string.bluetoothtest_str_qxljm') ,
    detail : $r('app.string.bluetoothtest_str_qxpljm') ,
    method : 'disconnect' ,
    api : testProfileDisConnectManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHandsFreeAudioGatewayManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljsbm') ,
    detail : $r('app.string.bluetoothtest_str_hqpljsbm') ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevicesManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHandsFreeAudioGatewayManager ,
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbztm') ,
    detail : $r('app.string.bluetoothtest_str_hqpsbztm') ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceStateManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHandsFreeAudioGatewayManager ,
  },
]

const HitHostProfileManagerTestCase: BRManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_ljm') ,
    detail : $r('app.string.bluetoothtest_str_pljm') ,
    method : 'connect' ,
    api : testProfileConnectManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHitHostManager
  },
  {
    name : $r('app.string.bluetoothtest_str_qxljm') ,
    detail : $r('app.string.bluetoothtest_str_qxpljm') ,
    method : 'disconnect' ,
    api : testProfileDisConnectManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHitHostManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljsbm') ,
    detail : $r('app.string.bluetoothtest_str_hqpljsbm') ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevicesManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHitHostManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbztm') ,
    detail : $r('app.string.bluetoothtest_str_hqpsbztm') ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceStateManager ,
    result : 'None' ,
    category : CaseCategory.ProfileHitHostManager
  },
]

const PanNetworkProfileManagerTestCase: BRManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_qxljm') ,
    detail : $r('app.string.bluetoothtest_str_qxpljm') ,
    method : 'disconnect' ,
    api : testProfileDisConnectManager ,
    result : 'None' ,
    category : CaseCategory.ProfilePanNetworkManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljsbm') ,
    detail : $r('app.string.bluetoothtest_str_hqpljsbm') ,
    method : 'getConnectionDevices' ,
    api : testProfileGetConnectionDevicesManager ,
    result : 'None' ,
    category : CaseCategory.ProfilePanNetworkManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbztm') ,
    detail : $r('app.string.bluetoothtest_str_hqpsbztm') ,
    method : 'getDeviceState' ,
    api : testProfileGetDeviceStateManager ,
    result : 'None' ,
    category : CaseCategory.ProfilePanNetworkManager
  },
]

const GattManagerAutoTestComposition: BRManagerTestComposition[] = [
  {
    name : 'GattClientAutoTestManager' ,
    detail : $r('app.string.bluetoothtest_str_gzdcs') ,
    url : 'pages/subAutoTestManager/subGattAutoTestManager/gattClientAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.GattAutoTestManager
  },
  {
    name : 'GattServerAutoTestManager' ,
    detail : $r('app.string.bluetoothtest_str_gzdcs_1') ,
    url : 'pages/subAutoTestManager/subGattAutoTestManager/gattServerAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.GattAutoTestManager
  },
]

const SppManagerAutoTestComposition: BRManagerTestComposition[] = [
  {
    name : 'SppClientAutoTestManager' ,
    detail : $r('app.string.bluetoothtest_str_szdcs') ,
    url : 'pages/subAutoTestManager/subSppAutoTestManager/sppClientAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.SppAutoTestManager
  },
  {
    name : 'SppServerAutoTest' ,
    detail : $r('app.string.bluetoothtest_str_szdcs_3') ,
    url : 'pages/subAutoTest/subSppAutoTest/sppServerAutoTest' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.SppAutoTestManager
  },
]

const ProfileManagerAutoTestComposition: BRManagerTestComposition[] = [
  {
    name : 'A2dpSourceAutoTestManager' ,
    detail : $r('app.string.bluetoothtest_str_azdcs') ,
    url : 'pages/subAutoTestManager/subProfileAutoTestManager/a2dpSourceAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.ProfileAutoTestManager
  },
  {
    name : 'HFAudioGatewayAutoTestManager' ,
    detail : $r('app.string.bluetoothtest_str_hzdcs') ,
    url : 'pages/subAutoTestManager/subProfileAutoTestManager/handsFreeAudioGatewayAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.ProfileAutoTestManager
  },
  {
    name : 'HitHostAutoTestManager' ,
    detail : $r('app.string.bluetoothtest_str_hzdcs_2') ,
    url : 'pages/subAutoTestManager/subProfileAutoTestManager/hitHostAutoTestManager' ,
    image : $r("app.media.AutoTestIcon") ,
    category : Category.ProfileAutoTestManager
  },
  {
    name : 'PanNetworkAutoTestManager' ,
    detail : $r('app.string.bluetoothtest_str_pzdcs') ,
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