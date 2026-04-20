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
 *BR Test interface Of Bluetooth test
 */

import BluetoothModel from './BluetoothModel'
import router from '@ohos.router';
import BluetoothDeviceController from '../controller/BluetoothDeviceController';
import bluetooth from '@ohos.bluetooth';
import baseProfile from '@ohos.bluetooth.baseProfile';
import a2dp from '@ohos.bluetooth.a2dp';
import promptAction from '@ohos.promptAction';
import { RetObj } from '../../Utils/common'

const PAGE_URI_DEVICE_FOUND_MODE = 'pages/subManualApiTest/subBrTest/deviceFound';

export function testInputBluetooth(): string {
  //  let ret = btApi.enableBluetooth();
  //  let btEnable = ret;
  //  AppStorage.setOrCreate('bluetoothIsOn', btEnable);
  //  let message = "蓝牙使能执行结果：" + ret;
  let message = "testInputBluetooth"
  promptAction.showToast({ message : message })
  return message
}

let btEnable = false

export function testEnableBluetooth() {
  try {
    hilog.info(0x0000, 'BtTest', '%{public}s', "testEnableBluetooth start-------------")
    let message = resStr($r('app.string.bluetoothtest_str_et'))
    if ( btEnable ) {
      message = resStr($r('app.string.bluetoothtest_str_lyyjsn'));
    } else {
      let ret = BluetoothModel.enableBluetooth();
      if ( ret ) {
        message = resStr($r('app.string.bluetoothtest_str_lysnzxjg')) + ret
      } else {
        message = resStr($r('app.string.bluetoothtest_str_lysnzxjg')) + ret + "\n"
        message += resStr($r('app.string.bluetoothtest_str_lyydkhdksbqqr'))
      }
      hilog.info(0x0000, 'BtTest', '%{public}s', resStr($r('app.string.bluetoothtest_str_lysnzxjg')) + ret);
      btEnable = true;
      AppStorage.setOrCreate('bluetoothIsOn' , btEnable);
    }
    promptAction.showToast({ message : message })
    return message
    hilog.info(0x0000, 'BtTest', '%{public}s', "testEnableBluetooth end-------------")
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testEnableBluetooth. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testEnableBluetooth. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testDisableBluetooth(): string {
  try {
    hilog.info(0x0000, 'BtTest', '%{public}s', "testDisableBluetooth start-------------")
    let message = resStr($r('app.string.bluetoothtest_str_dt'))
    if ( !btEnable ) {
      message = resStr($r('app.string.bluetoothtest_str_lyhwsn'));
    } else {
      let ret = BluetoothModel.disableBluetooth();
      if ( ret ) {
        message = resStr($r('app.string.bluetoothtest_str_lyqsnzxjg')) + ret;
      } else {
        message = resStr($r('app.string.bluetoothtest_str_lyqsnzxjg')) + ret + "\n"
        message += $r('app.string.bluetoothtest_str_lyygbhgbsbqqr')
      }
      hilog.info(0x0000, 'BtTest', '%{public}s', resStr($r('app.string.bluetoothtest_str_lyqsnzxjg')) + ret);
      btEnable = false;
      AppStorage.setOrCreate('bluetoothIsOn' , btEnable);
    }
    promptAction.showToast({ message : message })
    return message
    hilog.info(0x0000, 'BtTest', '%{public}s', "testDisableBluetooth end-------------")
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testDisableBluetooth. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testDisableBluetooth. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetState(): string {
  try {
    let ret = BluetoothModel.getState();
    hilog.info(0x0000, 'BtTest', '%{public}s', "ret:" + ret)
    let stateBT = "getState test"
    switch ( ret ) {
      case 0:
        stateBT = 'STATE_OFF';
        break;
      case 1:
        stateBT = 'STATE_TURNING_ON';
        break;
      case 2:
        stateBT = 'STATE_ON';
        break;
      case 3:
        stateBT = 'STATE_TURNING_OFF';
        break;
      case 4:
        stateBT = 'STATE_BLE_TURNING_ON';
        break;
      case 5:
        stateBT = 'STATE_BLE_ON';
        break;
      case 6:
        stateBT = 'STATE_BLE_TURNING_OFF';
        break;
      default:
        stateBT = resStr($r('app.string.bluetoothtest_str_wzzt'));
        break;
    }
    let message = resStr($r('app.string.bluetoothtest_str_dqlydzts')) + stateBT;
    if ( ret == 0 || ret == 3 ) {
      btEnable = false
      hilog.info(0x0000, 'BtTest', '%{public}s', "btEnable = false---------------")
    } else if ( ret == 1 || ret == 2 ) {
      btEnable = true
      hilog.info(0x0000, 'BtTest', '%{public}s', "btEnable = true---------------")
    } else {
      hilog.info(0x0000, 'BtTest', '%{public}s', $r('app.string.bluetoothtest_str_ckbzt'))
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetState. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetState. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetBtConnectionState(): string {
  try {
    let ret = BluetoothModel.getBtConnectionState();
    let message = resStr($r('app.string.bluetoothtest_str_dqljzts')) + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetBtConnectionState. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetBtConnectionState. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testSetLocalName(name: string): string {
  try {
    let ret = BluetoothModel.setLocalName(name);
    let message = resStr($r('app.string.bluetoothtest_str_sz')) + ret + resStr($r('app.string.bluetoothtest_str_sbmcw')) + name;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testSetLocalName. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSetLocalName. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetLocalName(): string {
  try {
    let ret = BluetoothModel.getLocalName();
    let message = resStr($r('app.string.bluetoothtest_str_bsbmcs')) + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetLocalName. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetLocalName. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testSetBluetoothScanMode(mode: number , dur: number): string {
  try {
    let retObj: RetObj = { mod : 0 , duration : - 1 }
    let ret = BluetoothModel.setBluetoothScanMode(mode , dur);
    //  let ret = true   调试使用
    if ( ret ) {
      AppStorage.setOrCreate('setScanModeRet' , ret);
      retObj.mod = mode;
      retObj.duration = dur;
    } else {
      hilog.info(0x0000, 'BtTest', '%{public}s', "BluetoothModel.setBluetoothScanMode failed!")
    }
    hilog.info(0x0000, 'BtTest', '%{public}s', "BluetoothModel.setBluetoothScanMode success!" + JSON.stringify(retObj))
    // let message = ret ? "setBluetoothScanMode执行成功" + JSON.stringify( retObj ) : "setBluetoothScanMode执行失败"
    let message = ret ? JSON.stringify(retObj) : resStr($r('app.string.bluetoothtest_str_szxsb'))
    let showToastMessage = ret ? resStr($r('app.string.bluetoothtest_str_szxcg')) + JSON.stringify(retObj) : resStr($r('app.string.bluetoothtest_str_szxsb'))
    promptAction.showToast({ message : showToastMessage })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testSetBluetoothScanMode. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSetBluetoothScanMode. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetBluetoothScanMode(): string {
  try {
    let ret = BluetoothModel.getBluetoothScanMode();
    let message = resStr($r('app.string.bluetoothtest_str_ms')) + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetBluetoothScanMode. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetBluetoothScanMode. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testStartBluetoothDiscovery() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_3'))
    let btEnable: boolean = AppStorage.get('bluetoothIsOn') !
    if ( !btEnable ) {
      message = resStr($r('app.string.bluetoothtest_str_lyhwsn'));
      return message;
    } else {
      router.pushUrl({ url : PAGE_URI_DEVICE_FOUND_MODE });
      message = resStr($r('app.string.bluetoothtest_str_lyfxcg'))
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testStartBluetoothDiscovery. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testStartBluetoothDiscovery. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testStopBluetoothDiscovery(): string {
  try {
    let ret = BluetoothModel.stopBluetoothDiscovery();
    let message = resStr($r('app.string.bluetoothtest_str_szx_1')) + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testStopBluetoothDiscovery. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testStopBluetoothDiscovery. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testPairDevice(deviceId: string): string {
  try {
    let controller = new BluetoothDeviceController;
    let ret = bluetooth.pairDevice(deviceId);
    let message = ""
    controller.pair(deviceId , (pinCode: string) => {
      promptAction.showToast({ message : pinCode })
      AppStorage.setOrCreate('pairedMac' , deviceId);
    } , () => {
      promptAction.showToast({ message : "testPairDevice Failed" })
    })
    message = resStr($r('app.string.bluetoothtest_str_pd')) + deviceId + (ret ? resStr($r('app.string.bluetoothtest_str_cg')) : resStr($r('app.string.bluetoothtest_str_sb_1')))
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testPairDevice. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testPairDevice. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testUnPairedDevice(deviceId: string): string {
  //  let deviceId = "123123"
  try {
    let ret = BluetoothModel.unpairDevice(deviceId);
    let message = resStr($r('app.string.bluetoothtest_str_sb')) + deviceId + resStr($r('app.string.bluetoothtest_str_dk')) + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testUnPairedDevice. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testUnPairedDevice. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetRemoteDeviceName(deviceId: string): string {
  try {
    let ret = BluetoothModel.getDeviceName(deviceId);
    let message = resStr($r('app.string.bluetoothtest_str_ycsbm')) + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetRemoteDeviceName. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetRemoteDeviceName. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetRemoteDeviceClass(deviceId: string): string {
  //  let deviceId= "12312fsdf";
  try {
    let ret = BluetoothModel.getDeviceType(deviceId);
    let message = resStr($r('app.string.bluetoothtest_str_yccm')) + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetRemoteDeviceClass. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetRemoteDeviceClass. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetPairedDevices(): string {
  try {
    let ret = BluetoothModel.getPairedDeviceIds();
    let message = resStr($r('app.string.bluetoothtest_str_gt_8'))
    hilog.info(0x0000, 'BtTest', '%{public}s', "BluetoothModel.getPairedDeviceIds:" + ret)
    if ( ret.length == 0 ) {
      message = resStr($r('app.string.bluetoothtest_str_dqmyrhpdsb'));
    } else {
      message = resStr($r('app.string.bluetoothtest_str_sbw')) + ret[ 0 ]
      AppStorage.setOrCreate('pairedMac' , ret[ 0 ]);
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetPairedDevices. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetPairedDevices. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testSetConnectionStrategy(connectionStrategy: number) {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_6'))
    let a2dpSrc = a2dp.createA2dpSrcProfile();
    let pairedMac: string = AppStorage.get('pairedMac') !
    if ( pairedMac.length == 0 ) {
      message = resStr($r('app.string.bluetoothtest_str_dqmzwkqjcsfypd'));
    } else {
      //[TODO] 接口改了调用方式，需要重新适配
      // a2dpSrc.getConnectionState(pairedMac , (err , connectionStrategy) => {
      //   if ( !err ) {
      //     message = "获取连接策略,mac值为: " + pairedMac + ";策略为: " + connectionStrategy
      //     hilog.info(0x0000, 'BtTest', '%{public}s', "获取连接策略,mac值为: " + pairedMac + ";策略为: " + connectionStrategy)
      //   } else {
      //     hilog.info(0x0000, 'BtTest', '%{public}s', 'getConnectionStrategy, err: ' + JSON.stringify(err));
      //   }
      //   promptAction.showToast({ message : message })
      //   return message
      // });
    }
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testSetConnectionStrategy. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSetConnectionStrategy. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetConnectionStrategy(deviceId: string): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_gt_1'))
    let a2dpSrc = a2dp.createA2dpSrcProfile();
    let pairedMac: string = AppStorage.get('pairedMac') !
    if ( pairedMac.length == 0 ) {
      message = resStr($r('app.string.bluetoothtest_str_dqmzwkqjcsfypd'));
    } else {
      //[TODO] 接口改了调用方式，需要重新适配
      // a2dpSrc.getConnectionState(pairedMac , (err , connectionStrategy) => {
      //   if ( !err ) {
      //     message = "获取连接策略,mac值为: " + pairedMac + ";策略为: " + connectionStrategy
      //     hilog.info(0x0000, 'BtTest', '%{public}s', "获取连接策略,mac值为: " + pairedMac + ";策略为: " + connectionStrategy)
      //   } else {
      //     hilog.info(0x0000, 'BtTest', '%{public}s', 'getConnectionStrategy, err: ' + JSON.stringify(err));
      //   }
      //   promptAction.showToast({ message : message })
      //   return message
      // });
    }
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetConnectionStrategy. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetConnectionStrategy. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetProfileConnState1(profileId: number): string {
  try {
    let ret = BluetoothModel.getProfileConnState(profileId);
    let message = ret
    promptAction.showToast({ message : message })
    hilog.info(0x0000, 'BtTest', '%{public}s', "br.testGetProfileConnState1:" + ret)
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetProfileConnState1. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetProfileConnState1. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetProfileConnState2(profileId: number , deviceId: string): string {
  try {
    let ret = bluetooth.getProfile(profileId).getDeviceState(deviceId);
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
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetProfileConnState2. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetProfileConnState2. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

let stateChange = false

export function testOnStateChange(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot_12'))
    hilog.info(0x0000, 'BtTest', '%{public}s', 'testOnStateChange test');
    if ( stateChange ) {
      bluetooth.off('stateChange' , () => {
        promptAction.showToast({ message : resStr($r('app.string.bluetoothtest_str_osjtygb')) })
      })
      message = resStr($r('app.string.bluetoothtest_str_gbzcztbh'))
      hilog.info(0x0000, 'BtTest', '%{public}s', 'testOffStateChange test');
    } else {
      bluetooth.on('stateChange' , (data: bluetooth.BluetoothState) => {
        let btStateMessage = ""
        switch ( data ) {
          case 0:
            btStateMessage += 'STATE_OFF';
            break;
          case 1:
            btStateMessage += 'STATE_TURNING_ON';
            break;
          case 2:
            btStateMessage += 'STATE_ON';
            break;
          case 3:
            btStateMessage += 'STATE_TURNING_OFF';
            break;
          case 4:
            btStateMessage += 'STATE_BLE_TURNING_ON';
            break;
          case 5:
            btStateMessage += 'STATE_BLE_ON';
            break;
          case 6:
            btStateMessage += 'STATE_BLE_TURNING_OFF';
            break;
          default:
            btStateMessage += resStr($r('app.string.bluetoothtest_str_wzzt'));
            break;
        }
        promptAction.showToast({ message : resStr($r('app.string.bluetoothtest_str_zt')) + btStateMessage })
      })
      message = resStr($r('app.string.bluetoothtest_str_dkzcztbh'))
      promptAction.showToast({ message : message })
    }
    stateChange = !stateChange
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testOnStateChange. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnStateChange. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

let pinRequired = false

export function testOnPinRequired(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot_11'))
    hilog.info(0x0000, 'BtTest', '%{public}s', 'testOnPinRequired test');
    if ( pinRequired ) {
      bluetooth.off('pinRequired' , () => {
        promptAction.showToast({ message : resStr($r('app.string.bluetoothtest_str_opjtygb')) })
      })
      message = resStr($r('app.string.bluetoothtest_str_gbzcpmbh'))
    } else {
      bluetooth.on('pinRequired' , (data: bluetooth.PinRequiredParam) => {
        promptAction.showToast({ message : "mac:" + data.deviceId + "pin:" + data.pinCode })
      })
      message = resStr($r('app.string.bluetoothtest_str_dkzcpmbh'))
    }
    pinRequired = !pinRequired
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testOnPinRequired. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnPinRequired. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

let bondStateFlag = false

export function testOnBondStateChange(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot_7'))
    hilog.info(0x0000, 'BtTest', '%{public}s', 'testOnBondStateChange test');
    if ( bondStateFlag ) {
      bluetooth.off('bondStateChange' , () => {
        promptAction.showToast({ message : resStr($r('app.string.bluetoothtest_str_objtygb')) })
      })
      message = resStr($r('app.string.bluetoothtest_str_gbzcbdztbh'))
    } else {
      bluetooth.on('bondStateChange' , (data: bluetooth.BondStateParam) => {
        promptAction.showToast({ message : "mac:" + data.deviceId + "state:" + data.state })
      })
      message = resStr($r('app.string.bluetoothtest_str_dkzcbdztbh'))
    }
    bondStateFlag = !bondStateFlag
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testOnBondStateChange. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnBondStateChange. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function subscribeBluetoothDeviceFind(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_1'))
    hilog.info(0x0000, 'BtTest', '%{public}s', "subscribeBluetoothDeviceFind")
    BluetoothModel.subscribeBluetoothDeviceFind((deviceIds: Array<string>) => {
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
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to subscribeBluetoothDeviceFind. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to subscribeBluetoothDeviceFind. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}