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
import { resStr } from '../../Utils/resString';



import hilog from '@ohos.hilog';
/**
 *BR Manager Test Page Of Bluetooth test
 */

import BluetoothModelManager from './BluetoothModelManager'
import router from '@ohos.router';
import BluetoothDeviceController from '../controller/BluetoothDeviceController';
import bluetoothManager from '@ohos.bluetoothManager';
import promptAction from '@ohos.promptAction';
import { RetObj } from '../../Utils/common'

const PAGE_URI_DEVICE_FOUND_MODE = 'pages/subManualApiTest/subBrTest/deviceFound';

export function testInputBluetoothManager(): string {
  //  let ret = btApi.enableBluetooth();
  //  let btManagerEnable = ret;
  //  AppStorage.setOrCreate('bluetoothManagerIsOn', btManagerEnable);
  //  let message = "蓝牙使能执行结果：" + ret;
  let message = "testInputBluetooth"
  promptAction.showToast({ message : message })
  return message
}

let btManagerEnable = false

export function testEnableBluetoothManager() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_et'))
    if ( btManagerEnable ) {
      message = resStr($r('app.string.bluetoothtest_str_lyyjsn'));
    } else {
      BluetoothModelManager.enableBluetooth();
      message = resStr($r('app.string.bluetoothtest_str_lyydkhdksbqqr'))
      btManagerEnable = true;
      AppStorage.setOrCreate('bluetoothManagerIsOn' , btManagerEnable);
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testEnableBluetoothManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testEnableBluetoothManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testDisableBluetoothManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_dt'))
    if ( !btManagerEnable ) {
      message = resStr($r('app.string.bluetoothtest_str_lyhwsn'));
    } else {
      BluetoothModelManager.disableBluetooth();
      message = resStr($r('app.string.bluetoothtest_str_lyygbhgbsbqqr'))
      btManagerEnable = false;
      AppStorage.setOrCreate('bluetoothManagerIsOn' , btManagerEnable);
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testDisableBluetoothManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testDisableBluetoothManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetStateManager(): string {
  try {
    let ret = BluetoothModelManager.getState();
    let stateManagerBT = "getState test"
    switch ( ret ) {
      case 0:
        stateManagerBT = 'STATE_OFF';
        break;
      case 1:
        stateManagerBT = 'STATE_TURNING_ON';
        break;
      case 2:
        stateManagerBT = 'STATE_ON';
        break;
      case 3:
        stateManagerBT = 'STATE_TURNING_OFF';
        break;
      case 4:
        stateManagerBT = 'STATE_BLE_TURNING_ON';
        break;
      case 5:
        stateManagerBT = 'STATE_BLE_ON';
        break;
      case 6:
        stateManagerBT = 'STATE_BLE_TURNING_OFF';
        break;
      default:
        stateManagerBT = resStr($r('app.string.bluetoothtest_str_wzzt'));
        break;
    }
    let message = resStr($r('app.string.bluetoothtest_str_dqlydzts')) + stateManagerBT;
    if ( ret == 0 || ret == 3 ) {
      btManagerEnable = false
    } else if ( ret == 1 || ret == 2 ) {
      btManagerEnable = true
    } else {
      hilog.info(0x0000, 'BtTest', '%{public}s', resStr($r('app.string.bluetoothtest_str_ckbzt')))
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetStateManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetStateManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetBtConnectionStateManager(): string {
  try {
    let ret = BluetoothModelManager.getBtConnectionState();
    let message = resStr($r('app.string.bluetoothtest_str_dqljzts')) + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetBtConnectionStateManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetBtConnectionStateManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testSetLocalNameManager(name: string): string {
  try {
    let ret = BluetoothModelManager.setLocalName(name);
    let message = resStr($r('app.string.bluetoothtest_str_sz')) + ret + resStr($r('app.string.bluetoothtest_str_sbmcw')) + name;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testSetLocalNameManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSetLocalNameManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetLocalNameManager(): string {
  try {
    let ret = BluetoothModelManager.getLocalName();
    let message = resStr($r('app.string.bluetoothtest_str_bsbmcs')) + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetLocalNameManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetLocalNameManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testSetBluetoothScanModeManager(mode: number , dur: number): string {
  try {
    let retObj: RetObj = { mod : 0 , duration : - 1 }
    let ret = BluetoothModelManager.setBluetoothScanMode(mode , dur);
    AppStorage.setOrCreate('setScanModeRet' , ret);
    retObj.mod = mode;
    retObj.duration = dur;
    hilog.info(0x0000, 'BtTest', '%{public}s', "BluetoothModelManager.setBluetoothScanMode success!" + JSON.stringify(retObj))
    // let message = ret ? "setBluetoothScanMode执行成功" + JSON.stringify( retObj ) : "setBluetoothScanMode执行失败"    //dialog会报错
    let message = resStr($r('app.string.bluetoothtest_str_szxcg_1'))
    let showToastMessage = resStr($r('app.string.bluetoothtest_str_szxcg_1'))
    promptAction.showToast({ message : showToastMessage })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testSetBluetoothScanModeManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSetBluetoothScanModeManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetBluetoothScanModeManager(): string {
  try {
    let ret = BluetoothModelManager.getBluetoothScanMode();
    let message = resStr($r('app.string.bluetoothtest_str_ms')) + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetBluetoothScanModeManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetBluetoothScanModeManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testStartBluetoothDiscoveryManager() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_3'))
    let btManagerEnable: boolean = AppStorage.get('bluetoothManagerIsOn') !
    if ( !btManagerEnable ) {
      message = resStr($r('app.string.bluetoothtest_str_lyhwsn'));
      return message;
    } else {
      router.pushUrl({ url : PAGE_URI_DEVICE_FOUND_MODE });
      message = resStr($r('app.string.bluetoothtest_str_lyfxcg'))
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testStartBluetoothDiscoveryManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testStartBluetoothDiscoveryManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testStopBluetoothDiscoveryManager(): string {
  try {
    let ret = BluetoothModelManager.stopBluetoothDiscovery();
    let message = resStr($r('app.string.bluetoothtest_str_szx_1')) + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testStopBluetoothDiscoveryManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testStopBluetoothDiscoveryManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testPairDeviceManager(deviceId: string): string {
  try {
    let controller = new BluetoothDeviceController;
    bluetoothManager.pairDevice(deviceId);
    let message = ""
    controller.pair(deviceId , (pinCode: string) => {
      promptAction.showToast({ message : pinCode })
      AppStorage.setOrCreate('pairedMac' , deviceId);
    } , () => {
      promptAction.showToast({ message : "testPairDevice Failed" })
    })
    message = resStr($r('app.string.bluetoothtest_str_pd')) + deviceId + resStr($r('app.string.bluetoothtest_str_cg'))
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testPairDeviceManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testPairDeviceManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testUnPairedDeviceManager(deviceId: string): string {
  //  let deviceId = "123123"
  try {
    let ret = BluetoothModelManager.unpairDevice(deviceId);
    let message = resStr($r('app.string.bluetoothtest_str_sb')) + deviceId + resStr($r('app.string.bluetoothtest_str_dk')) + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testUnPairedDeviceManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testUnPairedDeviceManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetRemoteDeviceNameManager(deviceId: string): string {
  try {
    let ret = BluetoothModelManager.getDeviceName(deviceId);
    let message = resStr($r('app.string.bluetoothtest_str_ycsbm')) + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetRemoteDeviceNameManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetRemoteDeviceNameManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetRemoteDeviceClassManager(deviceId: string): string {
  //  let deviceId= "12312fsdf";
  try {
    let ret = BluetoothModelManager.getDeviceType(deviceId);
    let message = resStr($r('app.string.bluetoothtest_str_yccm')) + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetRemoteDeviceClassManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetRemoteDeviceClassManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetPairedDevicesManager(): string {
  try {
    let ret = BluetoothModelManager.getPairedDeviceIds();
    let message = resStr($r('app.string.bluetoothtest_str_gt_8'))
    hilog.info(0x0000, 'BtTest', '%{public}s', "BluetoothModelManager.getPairedDeviceIds:" + ret)
    if ( ret.length == 0 ) {
      message = resStr($r('app.string.bluetoothtest_str_dqmyrhpdsb'));
    } else {
      message = resStr($r('app.string.bluetoothtest_str_sbw')) + ret[ 0 ]
      AppStorage.setOrCreate('pairedMac' , ret[ 0 ]);
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetPairedDevicesManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetPairedDevicesManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}


export function testGetProfileConnStateManager1(profileId: number): string {
  try {
    let ret = BluetoothModelManager.getProfileConnState(profileId);
    let message = ret
    promptAction.showToast({ message : message })
    hilog.info(0x0000, 'BtTest', '%{public}s', "BluetoothModelManager.testGetProfileConnState1:" + ret)
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetProfileConnStateManager1. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetProfileConnStateManager1. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetProfileConnStateManager2(profileId: number , deviceId: string): string {
  try {
    let ret = bluetoothManager.getProfileInstance(profileId).getDeviceState(deviceId);
    let stateProfile = ""
    hilog.info(0x0000, 'BtTest', '%{public}s', 'testGetProfileConnState2 state is ' + ret);
    switch ( ret ) {
      case 0:
        stateProfile = 'STATE_DISCONNECTED';
        break;
      case 1:
        stateProfile = 'STATE_CONNECTING';
        break;
      case 2:
        stateProfile = 'STATE_CONNECTED';
        break;
      case 3:
        stateProfile = 'STATE_DISCONNECTING';
        break;
      default:
        stateProfile = resStr($r('app.string.bluetoothtest_str_wz'));
        break;
    }
    let message = stateProfile;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetProfileConnStateManager2. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetProfileConnStateManager2. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

let stateChangeManager = false

export function testOnStateChangeManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot_12'))
    hilog.info(0x0000, 'BtTest', '%{public}s', 'testOnStateChange test');
    if ( stateChangeManager ) {
      bluetoothManager.off('stateChange' , () => {
        promptAction.showToast({ message : resStr($r('app.string.bluetoothtest_str_osjtygb')) })
      })
      message = resStr($r('app.string.bluetoothtest_str_gbzcztbh'))
      hilog.info(0x0000, 'BtTest', '%{public}s', 'testOffStateChange test');
    } else {
      bluetoothManager.on('stateChange' , (data: bluetoothManager.BluetoothState) => {
        let btStateMessageManager = ""
        switch ( data ) {
          case 0:
            btStateMessageManager += 'STATE_OFF';
            break;
          case 1:
            btStateMessageManager += 'STATE_TURNING_ON';
            break;
          case 2:
            btStateMessageManager += 'STATE_ON';
            break;
          case 3:
            btStateMessageManager += 'STATE_TURNING_OFF';
            break;
          case 4:
            btStateMessageManager += 'STATE_BLE_TURNING_ON';
            break;
          case 5:
            btStateMessageManager += 'STATE_BLE_ON';
            break;
          case 6:
            btStateMessageManager += 'STATE_BLE_TURNING_OFF';
            break;
          default:
            btStateMessageManager += resStr($r('app.string.bluetoothtest_str_wzzt'));
            break;
        }
        promptAction.showToast({ message : resStr($r('app.string.bluetoothtest_str_zt')) + btStateMessageManager })
      })
      message = resStr($r('app.string.bluetoothtest_str_dkzcztbh'))
      promptAction.showToast({ message : message })
    }
    stateChangeManager = !stateChangeManager
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testOnStateChangeManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnStateChangeManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

let pinRequiredManager = false

export function testOnPinRequiredManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot_11'))
    hilog.info(0x0000, 'BtTest', '%{public}s', 'testOnPinRequired test');
    if ( pinRequiredManager ) {
      bluetoothManager.off('pinRequired' , () => {
        promptAction.showToast({ message : resStr($r('app.string.bluetoothtest_str_opjtygb')) })
      })
      message = resStr($r('app.string.bluetoothtest_str_gbzcpmbh'))
    } else {
      bluetoothManager.on('pinRequired' , (data: bluetoothManager.PinRequiredParam) => {
        promptAction.showToast({ message : "mac:" + data.deviceId + "pin:" + data.pinCode })
      })
      message = resStr($r('app.string.bluetoothtest_str_dkzcpmbh'))
    }
    pinRequiredManager = !pinRequiredManager
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testOnPinRequiredManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnPinRequiredManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

let bondStateFlagManager = false

export function testOnBondStateChangeManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot_7'))
    hilog.info(0x0000, 'BtTest', '%{public}s', 'testOnBondStateChange test');
    if ( bondStateFlagManager ) {
      bluetoothManager.off('bondStateChange' , () => {
        promptAction.showToast({ message : resStr($r('app.string.bluetoothtest_str_objtygb')) })
      })
      message = resStr($r('app.string.bluetoothtest_str_gbzcbdztbh'))
    } else {
      bluetoothManager.on('bondStateChange' , (data: bluetoothManager.BondStateParam) => {
        promptAction.showToast({ message : "mac:" + data.deviceId + "state:" + data.state })
      })
      message = resStr($r('app.string.bluetoothtest_str_dkzcbdztbh'))
    }
    bondStateFlagManager = !bondStateFlagManager
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testOnBondStateChangeManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnBondStateChangeManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function subscribeBluetoothDeviceFindManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_1'))
    hilog.info(0x0000, 'BtTest', '%{public}s', "subscribeBluetoothDeviceFind")
    BluetoothModelManager.subscribeBluetoothDeviceFind((deviceIds: Array<string>) => {
      hilog.info(0x0000, 'BtTest', '%{public}s', 'available bluetooth devices changed.');
      deviceIds?.forEach(async(deviceId) => {
        hilog.info(0x0000, 'BtTest', '%{public}s', 'available bluetooth find [' + deviceId + '] ')
        let message = (resStr($r('app.string.bluetoothtest_str_abf')) + deviceId + resStr($r('app.string.bluetoothtest_str_str')))
        promptAction.showToast({ message : message })
        AppStorage.setOrCreate('bluetoothAvailableDevices' , message);
      })
    });
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to subscribeBluetoothDeviceFindManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to subscribeBluetoothDeviceFindManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}