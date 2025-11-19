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
    console.log("testEnableBluetooth start-------------")
    let message = "enableBluetooth test"
    if ( btEnable ) {
      message = '蓝牙已经使能';
    } else {
      let ret = BluetoothModel.enableBluetooth();
      if ( ret ) {
        message = "蓝牙使能执行结果：" + ret
      } else {
        message = "蓝牙使能执行结果：" + ret + "\n"
        message += "蓝牙已打开或打开失败,请确认"
      }
      console.log("蓝牙使能执行结果：" + ret);
      btEnable = true;
      AppStorage.setOrCreate('bluetoothIsOn' , btEnable);
    }
    promptAction.showToast({ message : message })
    return message
    console.log("testEnableBluetooth end-------------")
  } catch( e ) {
    console.log(`Failed to testEnableBluetooth. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testEnableBluetooth. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testDisableBluetooth(): string {
  try {
    console.log("testDisableBluetooth start-------------")
    let message = "disableBluetooth test"
    if ( !btEnable ) {
      message = '蓝牙还未使能';
    } else {
      let ret = BluetoothModel.disableBluetooth();
      if ( ret ) {
        message = "蓝牙去使能执行结果：" + ret;
      } else {
        message = "蓝牙去使能执行结果：" + ret + "\n"
        message += "蓝牙已关闭或关闭失败,请确认"
      }
      console.log("蓝牙去使能执行结果：" + ret);
      btEnable = false;
      AppStorage.setOrCreate('bluetoothIsOn' , btEnable);
    }
    promptAction.showToast({ message : message })
    return message
    console.log("testDisableBluetooth end-------------")
  } catch( e ) {
    console.log(`Failed to testDisableBluetooth. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testDisableBluetooth. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetState(): string {
  try {
    let ret = BluetoothModel.getState();
    console.log("ret:" + ret)
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
        stateBT = '未知状态';
        break;
    }
    let message = "当前蓝牙的状态是：" + stateBT;
    if ( ret == 0 || ret == 3 ) {
      btEnable = false
      console.log("btEnable = false---------------")
    } else if ( ret == 1 || ret == 2 ) {
      btEnable = true
      console.log("btEnable = true---------------")
    } else {
      console.log("查看bt状态")
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    console.log(`Failed to testGetState. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetState. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetBtConnectionState(): string {
  try {
    let ret = BluetoothModel.getBtConnectionState();
    let message = "当前连接状态是：" + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    console.log(`Failed to testGetBtConnectionState. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetBtConnectionState. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testSetLocalName(name: string): string {
  try {
    let ret = BluetoothModel.setLocalName(name);
    let message = "设置：" + ret + "设备名称为：" + name;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    console.log(`Failed to testSetLocalName. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSetLocalName. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetLocalName(): string {
  try {
    let ret = BluetoothModel.getLocalName();
    let message = "本设备名称是：" + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    console.log(`Failed to testGetLocalName. message: ${ e.message }, code: ${ e.code }`)
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
      console.log("BluetoothModel.setBluetoothScanMode failed!")
    }
    console.log("BluetoothModel.setBluetoothScanMode success!" + JSON.stringify(retObj))
    // let message = ret ? "setBluetoothScanMode执行成功" + JSON.stringify( retObj ) : "setBluetoothScanMode执行失败"
    let message = ret ? JSON.stringify(retObj) : "setBluetoothScanMode执行失败"
    let showToastMessage = ret ? "setBluetoothScanMode执行成功;" + JSON.stringify(retObj) : "setBluetoothScanMode执行失败"
    promptAction.showToast({ message : showToastMessage })
    return message
  } catch( e ) {
    console.log(`Failed to testSetBluetoothScanMode. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSetBluetoothScanMode. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetBluetoothScanMode(): string {
  try {
    let ret = BluetoothModel.getBluetoothScanMode();
    let message = "模式: " + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    console.log(`Failed to testGetBluetoothScanMode. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetBluetoothScanMode. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testStartBluetoothDiscovery() {
  try {
    let message = "startBluetoothDiscovery test"
    let btEnable: boolean = AppStorage.get('bluetoothIsOn') !
    if ( !btEnable ) {
      message = '蓝牙还未使能';
      return message;
    } else {
      router.pushUrl({ url : PAGE_URI_DEVICE_FOUND_MODE });
      message = "蓝牙发现成功"
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    console.log(`Failed to testStartBluetoothDiscovery. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testStartBluetoothDiscovery. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testStopBluetoothDiscovery(): string {
  try {
    let ret = BluetoothModel.stopBluetoothDiscovery();
    let message = "stopBluetoothDiscovery 执行：" + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    console.log(`Failed to testStopBluetoothDiscovery. message: ${ e.message }, code: ${ e.code }`)
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
    message = "配对：" + deviceId + ret ? "成功" : "失败"
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    console.log(`Failed to testPairDevice. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testPairDevice. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testUnPairedDevice(deviceId: string): string {
  //  let deviceId = "123123"
  try {
    let ret = BluetoothModel.unpairDevice(deviceId);
    let message = " 设备 " + deviceId + " 断开：" + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    console.log(`Failed to testUnPairedDevice. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testUnPairedDevice. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetRemoteDeviceName(deviceId: string): string {
  try {
    let ret = BluetoothModel.getDeviceName(deviceId);
    let message = '远程设备名:' + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    console.log(`Failed to testGetRemoteDeviceName. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetRemoteDeviceName. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetRemoteDeviceClass(deviceId: string): string {
  //  let deviceId= "12312fsdf";
  try {
    let ret = BluetoothModel.getDeviceType(deviceId);
    let message = '远程class名:' + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    console.log(`Failed to testGetRemoteDeviceClass. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetRemoteDeviceClass. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetPairedDevices(): string {
  try {
    let ret = BluetoothModel.getPairedDeviceIds();
    let message = "getPairedDevices test"
    console.log("BluetoothModel.getPairedDeviceIds:" + ret)
    if ( ret.length == 0 ) {
      message = '当前没有任何配对设备';
    } else {
      message = "设备为：" + ret[ 0 ]
      AppStorage.setOrCreate('pairedMac' , ret[ 0 ]);
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    console.log(`Failed to testGetPairedDevices. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetPairedDevices. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testSetConnectionStrategy(connectionStrategy: number) {
  try {
    let message = "setConnectionStrategy test"
    let a2dpSrc = a2dp.createA2dpSrcProfile();
    let pairedMac: string = AppStorage.get('pairedMac') !
    if ( pairedMac.length == 0 ) {
      message = '当前Mac值为空，请检查是否已配对';
    } else {
      //[TODO] 接口改了调用方式，需要重新适配
      // a2dpSrc.getConnectionState(pairedMac , (err , connectionStrategy) => {
      //   if ( !err ) {
      //     message = "获取连接策略,mac值为: " + pairedMac + ";策略为: " + connectionStrategy
      //     console.log("获取连接策略,mac值为: " + pairedMac + ";策略为: " + connectionStrategy)
      //   } else {
      //     console.log('getConnectionStrategy, err: ' + JSON.stringify(err));
      //   }
      //   promptAction.showToast({ message : message })
      //   return message
      // });
    }
    return message
  } catch( e ) {
    console.log(`Failed to testSetConnectionStrategy. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSetConnectionStrategy. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetConnectionStrategy(deviceId: string): string {
  try {
    let message = "getConnectionStrategy test"
    let a2dpSrc = a2dp.createA2dpSrcProfile();
    let pairedMac: string = AppStorage.get('pairedMac') !
    if ( pairedMac.length == 0 ) {
      message = '当前Mac值为空，请检查是否已配对';
    } else {
      //[TODO] 接口改了调用方式，需要重新适配
      // a2dpSrc.getConnectionState(pairedMac , (err , connectionStrategy) => {
      //   if ( !err ) {
      //     message = "获取连接策略,mac值为: " + pairedMac + ";策略为: " + connectionStrategy
      //     console.log("获取连接策略,mac值为: " + pairedMac + ";策略为: " + connectionStrategy)
      //   } else {
      //     console.log('getConnectionStrategy, err: ' + JSON.stringify(err));
      //   }
      //   promptAction.showToast({ message : message })
      //   return message
      // });
    }
    return message
  } catch( e ) {
    console.log(`Failed to testGetConnectionStrategy. message: ${ e.message }, code: ${ e.code }`)
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
    console.log("br.testGetProfileConnState1:" + ret)
    return message
  } catch( e ) {
    console.log(`Failed to testGetProfileConnState1. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetProfileConnState1. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetProfileConnState2(profileId: number , deviceId: string): string {
  try {
    let ret = bluetooth.getProfile(profileId).getDeviceState(deviceId);
    let stateProfile = ""
    console.log('testGetProfileConnState2 state is ' + ret);
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
        stateProfile = '未知';
        break;
    }
    let message = stateProfile;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    console.log(`Failed to testGetProfileConnState2. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetProfileConnState2. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

let stateChange = false

export function testOnStateChange(): string {
  try {
    let message = "onStateChange test"
    console.log('testOnStateChange test');
    if ( stateChange ) {
      bluetooth.off('stateChange' , () => {
        promptAction.showToast({ message : "on.stateChange监听已关闭" })
      })
      message = "关闭注册状态变化"
      console.log('testOffStateChange test');
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
            btStateMessage += '未知状态';
            break;
        }
        promptAction.showToast({ message : "状态:" + btStateMessage })
      })
      message = "打开注册状态变化"
      promptAction.showToast({ message : message })
    }
    stateChange = !stateChange
    return message
  } catch( e ) {
    console.log(`Failed to testOnStateChange. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnStateChange. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

let pinRequired = false

export function testOnPinRequired(): string {
  try {
    let message = "onPinRequired test"
    console.log('testOnPinRequired test');
    if ( pinRequired ) {
      bluetooth.off('pinRequired' , () => {
        promptAction.showToast({ message : "on.pinRequired监听已关闭" })
      })
      message = "关闭注册pin码变化"
    } else {
      bluetooth.on('pinRequired' , (data: bluetooth.PinRequiredParam) => {
        promptAction.showToast({ message : "mac:" + data.deviceId + "pin:" + data.pinCode })
      })
      message = "打开注册pin码变化"
    }
    pinRequired = !pinRequired
    return message
  } catch( e ) {
    console.log(`Failed to testOnPinRequired. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnPinRequired. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

let bondStateFlag = false

export function testOnBondStateChange(): string {
  try {
    let message = "onBondStateChange test"
    console.log('testOnBondStateChange test');
    if ( bondStateFlag ) {
      bluetooth.off('bondStateChange' , () => {
        promptAction.showToast({ message : "on.bondStateChange监听已关闭" })
      })
      message = "关闭注册绑定状态变化"
    } else {
      bluetooth.on('bondStateChange' , (data: bluetooth.BondStateParam) => {
        promptAction.showToast({ message : "mac:" + data.deviceId + "state:" + data.state })
      })
      message = "打开注册绑定状态变化"
    }
    bondStateFlag = !bondStateFlag
    return message
  } catch( e ) {
    console.log(`Failed to testOnBondStateChange. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnBondStateChange. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function subscribeBluetoothDeviceFind(): string {
  try {
    let message = "subscribeBluetoothDeviceFind test"
    console.log("subscribeBluetoothDeviceFind")
    BluetoothModel.subscribeBluetoothDeviceFind((deviceIds: Array<string>) => {
      console.log('available bluetooth devices changed.');
      deviceIds?.forEach(async(deviceId) => {
        console.log('available bluetooth find [' + deviceId + '] ')
        let message = ('available bluetooth find [' + deviceId + ']  ')
        promptAction.showToast({ message : message })
        AppStorage.setOrCreate('bluetoothAvailableDevices' , message);
      })
    });
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    console.log(`Failed to subscribeBluetoothDeviceFind. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to subscribeBluetoothDeviceFind. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}