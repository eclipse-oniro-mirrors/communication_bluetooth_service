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
  testEnableBluetoothManager ,
  testDisableBluetoothManager ,
  testGetStateManager ,
  testGetBtConnectionStateManager ,
  testSetLocalNameManager ,
  testGetLocalNameManager ,
  testSetBluetoothScanModeManager ,
  testGetBluetoothScanModeManager ,
  subscribeBluetoothDeviceFindManager ,
  testStopBluetoothDiscoveryManager ,
  testPairDeviceManager ,
  testGetRemoteDeviceNameManager ,
  testGetRemoteDeviceClassManager ,
  testGetPairedDevicesManager ,
  testUnPairedDeviceManager ,
  testGetProfileConnStateManager1 ,
  testGetProfileConnStateManager2 ,
  testOnStateChangeManager ,
  testOnPinRequiredManager ,
  testOnBondStateChangeManager
} from './brManagerInterface'
import {
  testCreateGattServerDeviceManager ,
  testCreateGattClientDeviceManager ,
  testGetConnectedBLEDevicesManager ,
  testStartBLEScanManager ,
  testStopBLEScanManager ,
  testOnBLEDeviceFindManager
} from './bleManagerInterface'
import {
  testCreateGattClientManager ,
  testConnectManager ,
  testDisconnectManager ,
  testOnBLEConnectionStateChangeManager ,
  testGattClientCloseManager ,
  testGetServicesCallbackManager ,
  testGetServicesPromiseManager ,
  testReadCharacteristicValueCallbackManager ,
  testReadCharacteristicValuePromiseManager ,
  testReadDescriptorValueCallbackManager ,
  testReadDescriptorValuePromiseManager ,
  testWriteCharacteristicValueManager ,
  testWriteDescriptorValueManager ,
  testOnBLECharacteristicChangeManager ,
  testSetBLEMtuSizeManager ,
  testSetNotifyCharacteristicChangedManager ,
  testGetDeviceNameCallbackManager ,
  testGetDeviceNamePromiseManager ,
  testGetRssiValueCallbackManager ,
  testGetRssiValuePromiseManager
} from './gattClientManagerInterface'
import {
  testCreateGattServerManager ,
  testStartAdvertisingManager ,
  testStopAdvertisingManager ,
  testAddServiceManager ,
  testRemoveServiceManager ,
  testGattServerCloseManager ,
  testOnConnectStateChangeManager ,
  testOnCharacteristicReadManager ,
  testOnCharacteristicWriteManager ,
  testOnDescriptorReadManager ,
  testOnDescriptorWriteManager
} from './gattServerManagerInterface'
import {
  testSppClientConnectSafeManager ,
  testSppClientConnectUnSafeManager ,
  testSppClientWriteManager ,
  testSppClientReadOnManager ,
  testSppClientReadOffManager ,
  testSppClientCloseManager
} from './sppClientManagerInterface'
import {
  testSppServerCreateSafeManager ,
  testSppServerCreateUnSafeManager ,
  testSppServerAcceptManager ,
  testSppServerWriteManager ,
  testSppServerReadOnManager ,
  testSppServerReadOffManager ,
  testSppCloseServerSocketManager ,
  testSppCloseClientSocketManager
} from './sppServerManagerInterface'
import {
  testProfileConnectManager ,
  testProfileDisConnectManager ,
  testProfileGetPlayingStateManager ,
  testProfileGetConnectionDevicesManager ,
  testProfileGetDeviceStateManager
} from './a2dpSinkProfileManagerInterface'
import BluetoothModelManager from './BluetoothModelManager'

interface AutoManagerTest {
  name: Resource | string;
  detail: Resource | string;
  method: string;
  api: Function;
  result: string;
  category: AutoCategory;
}

export const BrManagerAutoTestCase: AutoManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_zcztbhm') ,
    detail : $r('app.string.bluetoothtest_str_zcztbh') ,
    method : 'on.stateChangeManager' ,
    api : testOnStateChangeManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_zcpmbhm') ,
    detail : $r('app.string.bluetoothtest_str_zcpmbh') ,
    method : 'on.pinRequiredManager' ,
    api : testOnPinRequiredManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_zcbdztbhm') ,
    detail : $r('app.string.bluetoothtest_str_zcbdztbh') ,
    method : 'on.bondStateChange' ,
    api : testOnBondStateChangeManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_klym') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqztm') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqdqljztm') ,
    detail : $r('app.string.bluetoothtest_str_hqdqlyljzt') ,
    method : 'getBtConnectionStateManager' ,
    api : testGetBtConnectionStateManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalNameManager' ,
    api : testSetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalNameManager' ,
    api : testGetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_szlysmms') ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_dylyfxm') ,
    detail : $r('app.string.bluetoothtest_str_dyfxlysb') ,
    method : 'subscribeBluetoothDeviceFindManager' ,
    api : subscribeBluetoothDeviceFindManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_kslyfxm') ,
    detail : $r('app.string.bluetoothtest_str_ksfx') ,
    method : 'startBluetoothDiscoveryManager' ,
    api : BluetoothModelManager.startBluetoothDiscovery ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_tzlyfxm') ,
    detail : $r('app.string.bluetoothtest_str_tzfx') ,
    method : 'stopBluetoothDiscoveryManager' ,
    api : testStopBluetoothDiscoveryManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_pdsbm') ,
    detail : $r('app.string.bluetoothtest_str_jxlysbpd') ,
    method : 'pairDeviceManager' ,
    api : testPairDeviceManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqycsbmcm') ,
    detail : $r('app.string.bluetoothtest_str_hqpdlysbdmc') ,
    method : 'getDeviceNameManager' ,
    api : testGetRemoteDeviceNameManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqycsblxm') ,
    detail : $r('app.string.bluetoothtest_str_hqpdlysbdlx') ,
    method : 'getDeviceTypeManager' ,
    api : testGetRemoteDeviceClassManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqpdsbm') ,
    detail : $r('app.string.bluetoothtest_str_hqpdlysbdxx') ,
    method : 'getPairedDeviceIdsManager' ,
    api : testGetPairedDevicesManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_qxpdsbm') ,
    detail : $r('app.string.bluetoothtest_str_qxlysbpd') ,
    method : 'unpairDeviceManager' ,
    api : testUnPairedDeviceManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_glym') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetoothManager' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.BrAutoManager
  },
]

export const BleManagerAutoTestCase: AutoManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_klym') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.BleAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqztm') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : AutoCategory.BleAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalNameManager' ,
    api : testSetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.BleAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalNameManager' ,
    api : testGetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.BleAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_szlysmms') ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.BleAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.BleAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljdbsbm_1') ,
    detail : $r('app.string.bluetoothtest_str_hqljdbsbxx_1') ,
    method : 'getConnectedBleDevices' ,
    api : testGetConnectedBLEDevicesManager ,
    result : 'None' ,
    category : AutoCategory.BleAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_zcbczztbhm_1') ,
    detail : $r('app.string.bluetoothtest_str_zcbsbczztbh_1') ,
    method : 'on.BleDeviceFindManager' ,
    api : testOnBLEDeviceFindManager ,
    result : 'None' ,
    category : AutoCategory.BleAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_ksbsmm_1') ,
    detail : $r('app.string.bluetoothtest_str_kssmbsb_2') ,
    method : 'startBleScanManager' ,
    api : testStartBLEScanManager ,
    result : 'None' ,
    category : AutoCategory.BleAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_tzbsmm_1') ,
    detail : $r('app.string.bluetoothtest_str_tzsm') ,
    method : 'stopBleScanManager' ,
    api : testStopBLEScanManager ,
    result : 'None' ,
    category : AutoCategory.BleAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_glym') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetoothManager' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.BleAutoManager
  },
]

export const GattClientManagerAutoTestCase: AutoManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_klym') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqztm') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalNameManager' ,
    api : testSetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalNameManager' ,
    api : testGetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_szlysmms') ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_czgkhdm') ,
    detail : $r('app.string.bluetoothtest_str_czgkhd') ,
    method : 'createGattClientManager' ,
    api : testCreateGattClientManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_bljztm_1') ,
    detail : $r('app.string.bluetoothtest_str_ckbljzt_1') ,
    method : 'on.BleConnectionStateChangeManager' ,
    api : testOnBLEConnectionStateChangeManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_ljm') ,
    detail : $r('app.string.bluetoothtest_str_glj') ,
    method : 'connectManager' ,
    api : testConnectManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqfwc') ,
    detail : $r('app.string.bluetoothtest_str_hqfwc_1') ,
    method : 'getServices(Callback)Manager' ,
    api : testGetServicesCallbackManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqfwp') ,
    detail : $r('app.string.bluetoothtest_str_hqfwp_1') ,
    method : 'getServices(promise)Manager' ,
    api : testGetServicesPromiseManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_dqtzzc') ,
    detail : $r('app.string.bluetoothtest_str_dqtzzc_1') ,
    method : 'readCharacteristicValue(callback)Manager' ,
    api : testReadCharacteristicValueCallbackManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_dqtzzp') ,
    detail : $r('app.string.bluetoothtest_str_dqtzzp_1') ,
    method : 'readCharacteristicValue(promise)Manager' ,
    api : testReadCharacteristicValuePromiseManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_dqmsfzc') ,
    detail : $r('app.string.bluetoothtest_str_dqmsfzc_1') ,
    method : 'readDescriptorValue(callback)Manager' ,
    api : testReadDescriptorValueCallbackManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_dqmsfzp') ,
    detail : $r('app.string.bluetoothtest_str_dqmsfzp_1') ,
    method : 'readDescriptorValue(promise)Manager' ,
    api : testReadDescriptorValuePromiseManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_xrtzzm') ,
    detail : $r('app.string.bluetoothtest_str_xrtzz') ,
    method : 'writeCharacteristicValueManager' ,
    api : testWriteCharacteristicValueManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_xrmsfzm') ,
    detail : $r('app.string.bluetoothtest_str_xrmsfz') ,
    method : 'writeDescriptorValueManager' ,
    api : testWriteDescriptorValueManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_btzzbhm_1') ,
    detail : $r('app.string.bluetoothtest_str_ckbtzzbh_1') ,
    method : 'on.BleCharacteristicChangeManager' ,
    api : testOnBLECharacteristicChangeManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sdbccm_1') ,
    detail : $r('app.string.bluetoothtest_str_sdbcc_1') ,
    method : 'setBleMtuSizeManager' ,
    api : testSetBLEMtuSizeManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sztztxggm') ,
    detail : $r('app.string.bluetoothtest_str_sztztxgg') ,
    method : 'setNotifyCharacteristicChangedManager' ,
    api : testSetNotifyCharacteristicChangedManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbmcc') ,
    detail : $r('app.string.bluetoothtest_str_hqsbmcc_1') ,
    method : 'getDeviceName(callback)Manager' ,
    api : testGetDeviceNameCallbackManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbmcp') ,
    detail : $r('app.string.bluetoothtest_str_hqsbmcp_1') ,
    method : 'getDeviceName(Promise)Manager' ,
    api : testGetDeviceNamePromiseManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqrzc') ,
    detail : $r('app.string.bluetoothtest_str_hqrzc_1') ,
    method : 'getRssiValue(Callback)Manager' ,
    api : testGetRssiValueCallbackManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqrzp') ,
    detail : $r('app.string.bluetoothtest_str_hqrzp_1') ,
    method : 'getRssiValue(Promise)Manager' ,
    api : testGetRssiValuePromiseManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_qxljm') ,
    detail : $r('app.string.bluetoothtest_str_qxglj') ,
    method : 'disconnectManager' ,
    api : testDisconnectManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_gbm') ,
    detail : $r('app.string.bluetoothtest_str_gbgkhd') ,
    method : 'closeManager' ,
    api : testGattClientCloseManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_glym') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetoothManager' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.GattClientAutoManager
  },
]

export const GattServerManagerAutoTestCase: AutoManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_klym') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqztm') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalNameManager' ,
    api : testSetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalNameManager' ,
    api : testGetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_szlysmms') ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_cjgfwqm') ,
    detail : $r('app.string.bluetoothtest_str_cjgfwq_1') ,
    method : 'createGattServerManager' ,
    api : testCreateGattServerManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_tjfwm') ,
    detail : $r('app.string.bluetoothtest_str_tjgfw') ,
    method : 'addServiceManager' ,
    api : testAddServiceManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_ksgbm') ,
    detail : $r('app.string.bluetoothtest_str_ksgbbb') ,
    method : 'startAdvertisingManager' ,
    api : testStartAdvertisingManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_ljztm') ,
    detail : $r('app.string.bluetoothtest_str_ckgljzt') ,
    method : 'on.connectStateChangeManager' ,
    api : testOnConnectStateChangeManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_dqtzzm') ,
    detail : $r('app.string.bluetoothtest_str_dqtzz') ,
    method : 'on.characteristicReadManager' ,
    api : testOnCharacteristicReadManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_xrtzzm') ,
    detail : $r('app.string.bluetoothtest_str_xrtzz') ,
    method : 'on.characteristicWriteManager' ,
    api : testOnCharacteristicWriteManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_dqmsfm') ,
    detail : $r('app.string.bluetoothtest_str_dqmsf') ,
    method : 'on.descriptorReadManager' ,
    api : testOnDescriptorReadManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_xrmsfm') ,
    detail : $r('app.string.bluetoothtest_str_xrmsf') ,
    method : 'on.descriptorWriteManager' ,
    api : testOnDescriptorWriteManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_tzgbm') ,
    detail : $r('app.string.bluetoothtest_str_tzgbbb') ,
    method : 'stopAdvertisingManager' ,
    api : testStopAdvertisingManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_scfwm') ,
    detail : $r('app.string.bluetoothtest_str_scgfw') ,
    method : 'removeServiceManager' ,
    api : testRemoveServiceManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_gbm') ,
    detail : $r('app.string.bluetoothtest_str_gbgfwq') ,
    method : 'closeManager' ,
    api : testGattServerCloseManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_glym') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetoothManager' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.GattServerAutoManager
  },
]

export const SppClientManagerAutoTestCase: AutoManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_kly') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetooth' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.SppClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szssmms') ,
    detail : $r('app.string.bluetoothtest_str_szssmms') ,
    method : 'setBluetoothScanMode' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.SppClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmc') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalName' ,
    api : testSetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.SppClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmc') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalName' ,
    api : testGetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.SppClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqzt') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getState' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : AutoCategory.SppClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmms') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanMode' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.SppClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdljaqms') ,
    detail : $r('app.string.bluetoothtest_str_skhdaqmslj') ,
    method : 'sppConnect' ,
    api : testSppClientConnectSafeManager ,
    result : 'None' ,
    category : AutoCategory.SppClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdljfaqms') ,
    detail : $r('app.string.bluetoothtest_str_skhdfaqmslj') ,
    method : 'sppConnect' ,
    api : testSppClientConnectUnSafeManager ,
    result : 'None' ,
    category : AutoCategory.SppClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_pdsb') ,
    detail : $r('app.string.bluetoothtest_str_jxlysbpd') ,
    method : 'pairDevice' ,
    api : testPairDeviceManager ,
    result : 'None' ,
    category : AutoCategory.SppClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdxr') ,
    detail : $r('app.string.bluetoothtest_str_skhdxr') ,
    method : 'sppWrite' ,
    api : testSppClientWriteManager ,
    result : 'None' ,
    category : AutoCategory.SppClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_skhddqdk') ,
    detail : $r('app.string.bluetoothtest_str_skhddqdk') ,
    method : 'on.sppRead' ,
    api : testSppClientReadOnManager ,
    result : 'None' ,
    category : AutoCategory.SppClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_skhddqgb') ,
    detail : $r('app.string.bluetoothtest_str_skhddqgb') ,
    method : 'off.sppRead' ,
    api : testSppClientReadOffManager ,
    result : 'None' ,
    category : AutoCategory.SppClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdgb') ,
    detail : $r('app.string.bluetoothtest_str_skhdgb') ,
    method : 'sppCloseClientSocket' ,
    api : testSppClientCloseManager ,
    result : 'None' ,
    category : AutoCategory.SppClientAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_gly') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetooth' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.SppClientAutoManager
  },
]

export const SppServerManagerAutoTestCase: AutoManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_klym') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szssmmsm') ,
    detail : $r('app.string.bluetoothtest_str_szssmms') ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_zcpmbhm') ,
    detail : $r('app.string.bluetoothtest_str_zcpmbh') ,
    method : 'on.pinRequiredManager' ,
    api : testOnPinRequiredManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalNameManager' ,
    api : testSetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalNameManager' ,
    api : testGetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqztm') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqcjaqmsm') ,
    detail : $r('app.string.bluetoothtest_str_sfwqaqmscj') ,
    method : 'sppListenManager' ,
    api : testSppServerCreateSafeManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqcjfaqmsm') ,
    detail : $r('app.string.bluetoothtest_str_sfwqfaqmscj') ,
    method : 'sppListenManager' ,
    api : testSppServerCreateUnSafeManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqxrm') ,
    detail : $r('app.string.bluetoothtest_str_sfwqxr') ,
    method : 'sppWriteManager' ,
    api : testSppServerWriteManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqjsm') ,
    detail : $r('app.string.bluetoothtest_str_sfwqjs') ,
    method : 'sppAcceptManager' ,
    api : testSppServerAcceptManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqdqdkm') ,
    detail : $r('app.string.bluetoothtest_str_sfwqdqdk') ,
    method : 'on.sppReadManager' ,
    api : testSppServerReadOnManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqdqgbm') ,
    detail : $r('app.string.bluetoothtest_str_sfwqdqgb') ,
    method : 'off.sppReadManager' ,
    api : testSppServerReadOffManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_sfwqgbm') ,
    detail : $r('app.string.bluetoothtest_str_sfwqgb') ,
    method : 'sppCloseServerSocketManager' ,
    api : testSppCloseServerSocketManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_skhdgbm') ,
    detail : $r('app.string.bluetoothtest_str_skhdgb') ,
    method : 'sppCloseClientSocketManager' ,
    api : testSppCloseClientSocketManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_glym') ,
    detail : $r('app.string.bluetoothtest_str_qsnly') ,
    method : 'disableBluetoothManager' ,
    api : testDisableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.SppServerAutoManager
  },
]

export const A2dpSourceProfileManagerAutoTestCase: AutoManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_klym') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqztm') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalNameManager' ,
    api : testSetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalNameManager' ,
    api : testGetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_szlysmms') ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_ljm') ,
    detail : $r('app.string.bluetoothtest_str_plj') ,
    method : 'connectManager' ,
    api : testProfileConnectManager ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbfztm') ,
    detail : $r('app.string.bluetoothtest_str_hqpbfzt') ,
    method : 'getPlayingStateManager' ,
    api : testProfileGetPlayingStateManager ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljsbm') ,
    detail : $r('app.string.bluetoothtest_str_hqpljsb') ,
    method : 'getConnectionDevicesManager' ,
    api : testProfileGetConnectionDevicesManager ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbztm') ,
    detail : $r('app.string.bluetoothtest_str_hqpsbzt') ,
    method : 'getDeviceStateManager' ,
    api : testProfileGetDeviceStateManager ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_qxljm') ,
    detail : $r('app.string.bluetoothtest_str_qxplj') ,
    method : 'disconnectManager' ,
    api : testProfileDisConnectManager ,
    result : 'None' ,
    category : AutoCategory.A2dpSourceAutoManager
  },
]

export const HandsFreeAudioGatewayProfileManagerAutoTestCase: AutoManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_klym') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqztm') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalNameManager' ,
    api : testSetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalNameManager' ,
    api : testGetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_szlysmms') ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_ljm') ,
    detail : $r('app.string.bluetoothtest_str_plj') ,
    method : 'connectManager' ,
    api : testProfileConnectManager ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljsbm') ,
    detail : $r('app.string.bluetoothtest_str_hqpljsb') ,
    method : 'getConnectionDevicesManager' ,
    api : testProfileGetConnectionDevicesManager ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbztm') ,
    detail : $r('app.string.bluetoothtest_str_hqpsbzt') ,
    method : 'getDeviceStateManager' ,
    api : testProfileGetDeviceStateManager ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_qxljm') ,
    detail : $r('app.string.bluetoothtest_str_qxplj') ,
    method : 'disconnectManager' ,
    api : testProfileDisConnectManager ,
    result : 'None' ,
    category : AutoCategory.HandsFreeAudioGatewayAutoManager
  },
]

export const HitHostProfileManagerAutoTestCase: AutoManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_klym') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.HitHostAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqztm') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : AutoCategory.HitHostAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalNameManager' ,
    api : testSetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.HitHostAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalNameManager' ,
    api : testGetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.HitHostAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_szlysmms') ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.HitHostAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.HitHostAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_ljm') ,
    detail : $r('app.string.bluetoothtest_str_plj') ,
    method : 'connectManager' ,
    api : testProfileConnectManager ,
    result : 'None' ,
    category : AutoCategory.HitHostAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljsbm') ,
    detail : $r('app.string.bluetoothtest_str_hqpljsb') ,
    method : 'getConnectionDevicesManager' ,
    api : testProfileGetConnectionDevicesManager ,
    result : 'None' ,
    category : AutoCategory.HitHostAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbztm') ,
    detail : $r('app.string.bluetoothtest_str_hqpsbzt') ,
    method : 'getDeviceStateManager' ,
    api : testProfileGetDeviceStateManager ,
    result : 'None' ,
    category : AutoCategory.HitHostAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_qxljm') ,
    detail : $r('app.string.bluetoothtest_str_qxplj') ,
    method : 'disconnectManager' ,
    api : testProfileDisConnectManager ,
    result : 'None' ,
    category : AutoCategory.HitHostAutoManager
  },
]

export const PanNetworkProfileManagerAutoTestCase: AutoManagerTest[] = [
  {
    name : $r('app.string.bluetoothtest_str_klym') ,
    detail : $r('app.string.bluetoothtest_str_snly') ,
    method : 'enableBluetoothManager' ,
    api : testEnableBluetoothManager ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqztm') ,
    detail : $r('app.string.bluetoothtest_str_hqlyzt') ,
    method : 'getStateManager' ,
    api : testGetStateManager ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_szlybdmc') ,
    method : 'setLocalNameManager' ,
    api : testSetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqbdmcm') ,
    detail : $r('app.string.bluetoothtest_str_hqlybdmc') ,
    method : 'getLocalNameManager' ,
    api : testGetLocalNameManager ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_szsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_szlysmms') ,
    method : 'setBluetoothScanModeManager' ,
    api : testSetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsmmsm') ,
    detail : $r('app.string.bluetoothtest_str_hqlysmms') ,
    method : 'getBluetoothScanModeManager' ,
    api : testGetBluetoothScanModeManager ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqljsbm') ,
    detail : $r('app.string.bluetoothtest_str_hqpljsb') ,
    method : 'getConnectionDevicesManager' ,
    api : testProfileGetConnectionDevicesManager ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_hqsbztm') ,
    detail : $r('app.string.bluetoothtest_str_hqpsbzt') ,
    method : 'getDeviceStateManager' ,
    api : testProfileGetDeviceStateManager ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAutoManager
  },
  {
    name : $r('app.string.bluetoothtest_str_qxljm') ,
    detail : $r('app.string.bluetoothtest_str_qxplj') ,
    method : 'disconnectManager' ,
    api : testProfileDisConnectManager ,
    result : 'None' ,
    category : AutoCategory.PanNetworkAutoManager
  },
]

export function initBrManagerAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  BrManagerAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

export function initBleManagerAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  BleManagerAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

export function initGattClientManagerAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  GattClientManagerAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

export function initGattServerManagerAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  GattServerManagerAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

export function initSppClientManagerAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  SppClientManagerAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

export function initSppServerManagerAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  SppServerManagerAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

export function initA2dpSourceProfileManagerAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  A2dpSourceProfileManagerAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

export function initHandsFreeAudioGatewayProfileManagerAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  HandsFreeAudioGatewayProfileManagerAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

export function initHitHostProfileManagerAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  HitHostProfileManagerAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}

export function initPanNetworkProfileManagerAutoData(): Array<TestAuto> {
  let TestAutoArray: Array<TestAuto> = []
  PanNetworkProfileManagerAutoTestCase.forEach(item => {
    TestAutoArray.push(new TestAuto(item.name , item.detail , item.method , item.api , item.result , item.category));
  })
  return TestAutoArray;
}