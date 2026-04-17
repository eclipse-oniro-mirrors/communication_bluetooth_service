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


import hilog from '@ohos.hilog';
/**
 * GattClient Test Of Bluetooth test
 */

import bluetoothManager from '@ohos.bluetoothManager';
import BluetoothModelManager from './BluetoothModelManager'
import { BusinessError } from '@ohos.base';
import bluetooth from '@ohos.bluetooth';
import promptAction from '@ohos.promptAction';
import { BleCharacteristicDataIn , Descriptor } from '../../Utils/common'

let gattClientInstance: bluetoothManager.GattClientDevice

export function testCreateGattClientManager(): string {
  try {
    let message = $r('app.string.bluetoothtest_str_ct_1')
    let peripheralDeviceId: string = AppStorage.get('peripheralDeviceId') !;
    let btState = bluetoothManager.getState();
    if ( btState === bluetoothManager.BluetoothState.STATE_ON ) {
      if ( gattClientInstance ) {
        message = $r('app.string.bluetoothtest_str_ge_2');
        promptAction.showToast({ message : message })
        return message;
      }
      //注册回调
      bluetoothManager.BLE.on("BLEDeviceFind" , (scanResult) => {
        scanResult.forEach(device => {
          let devId = device.deviceId; //6c:96:d7:3d:87:6f
          let rssi = device.rssi;
          let data = device.data;
          promptAction.showToast({ message : "findDevice:" + devId })
          if ( devId == peripheralDeviceId ) {
            bluetoothManager.BLE.stopBLEScan();
            // create ble client instance
            if ( !gattClientInstance ) {
              gattClientInstance = BluetoothModelManager.createGattClient(devId);
              message = $r('app.string.bluetoothtest_str_cs_4');
              hilog.info(0x0000, 'BtTest', '%{public}s', "createGattClient succeeds----------")
            }
          }
        })
      })
      if ( peripheralDeviceId && peripheralDeviceId.length > 15 ) {
        gattClientInstance = BluetoothModelManager.createGattClient(peripheralDeviceId.toUpperCase());
        if ( gattClientInstance ) {
          message = $r('app.string.bluetoothtest_str_c') + peripheralDeviceId;
          hilog.info(0x0000, 'BtTest', '%{public}s', "createGattClient peripheralDeviceId succeeds----------")
        } else {
          message = $r('app.string.bluetoothtest_str_cf');
        }
      } else {
        message = $r('app.string.bluetoothtest_str_pi');
      }
      promptAction.showToast({ message : message })
    } else if ( btState === bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
      promptAction.showToast({ message : message })
    } else {
      message = $r('app.string.bluetoothtest_str_spw_1')
      promptAction.showToast({ message : message })
    }
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testCreateGattClientManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testCreateGattClientManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

let bleConnectionState = 0;

export function testConnectManager(): string {
  try {
    let message = $r('app.string.bluetoothtest_str_ct_4');
    let btState = bluetoothManager.getState();
    if ( btState === bluetoothManager.BluetoothState.STATE_ON ) {
      if ( gattClientInstance ) {
        if ( bleConnectionState == 0 ) {
          hilog.info(0x0000, 'BtTest', '%{public}s', "GattClient.connect: start----------------")
          gattClientInstance.connect();
          message = $r('app.string.bluetoothtest_str_gc_2');
          hilog.info(0x0000, 'BtTest', '%{public}s', "GattClient.connect: end----------------")
          bleConnectionState = 2
        } else {
          message = $r('app.string.bluetoothtest_str_tdind');
        }
      } else {
        message = "NO_GATT_CLIENT_OBJECT";
      }
    } else if ( btState === bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
    } else {
      message = $r('app.string.bluetoothtest_str_spw');
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testConnectManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testConnectManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testDisconnectManager(): string {
  try {
    let message = $r('app.string.bluetoothtest_str_td');
    let btState = bluetoothManager.getState();
    if ( btState == bluetoothManager.BluetoothState.STATE_ON ) {
      if ( !gattClientInstance ) {
        message = "NO_GATT_CLIENT_OBJECT";
        promptAction.showToast({ message : message })
        return message;
      }
      if ( bleConnectionState == 2 ) {
        gattClientInstance.disconnect();
        message = $r('app.string.bluetoothtest_str_gd');
      } else {
        message = $r('app.string.bluetoothtest_str_bincanntdi');
      }
    } else if ( btState == bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
    } else {
      message = $r('app.string.bluetoothtest_str_spw');
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testDisconnectManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testDisconnectManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

let BLEConnectionStateChange = false

export function testOnBLEConnectionStateChangeManager(): string {
  try {
    let message = $r('app.string.bluetoothtest_str_ot');
    let btStatus = bluetoothManager.getState();
    if ( btStatus === bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
      promptAction.showToast({ message : message })
      return message;
    }
    if ( !gattClientInstance ) {
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({ message : message })
      return message;
    }
    message = " HAVE_GATT_CLIENT_OBJECT "
    if ( BLEConnectionStateChange ) {
      gattClientInstance.off('BLEConnectionStateChange' , () => {
        promptAction.showToast({ message : "off:BLEConnectionStateChange" })
      })
      message = $r('app.string.bluetoothtest_str_gbbljztbh')
    } else {
      let bleConnectionStateInfo = "xxxx"
      gattClientInstance.on('BLEConnectionStateChange' , (bleConnectionState: bluetooth.BLEConnectChangedState) => {
        switch ( bleConnectionState.state ) {
          case bluetoothManager.ProfileConnectionState.STATE_DISCONNECTED:
            bleConnectionStateInfo = "DISCONNECTED";
            break;
          case bluetoothManager.ProfileConnectionState.STATE_CONNECTING:
            bleConnectionStateInfo = "CONNECTING";
            break;
          case bluetoothManager.ProfileConnectionState.STATE_CONNECTED:
            bleConnectionStateInfo = "STATE_CONNECTED";
            break;
          case bluetoothManager.ProfileConnectionState.STATE_DISCONNECTING:
            bleConnectionStateInfo = "STATE_DISCONNECTING";
            break;
          default:
            bleConnectionStateInfo = "UNDEFINED!";
            break;
        }
        hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.onBLEConnectionStateChange is ------- " + JSON.stringify(bleConnectionState))
        AppStorage.setOrCreate('bleConnectionStateInfo' , bleConnectionStateInfo);
        promptAction.showToast({ message : $r('app.string.bluetoothtest_str_zt') + bleConnectionStateInfo })
      })
      message = $r('app.string.bluetoothtest_str_dkbljztbh')
    }
    BLEConnectionStateChange = !BLEConnectionStateChange
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testOnBLEConnectionStateChangeManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnBLEConnectionStateChangeManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGattClientCloseManager(): string {
  try {
    let message = $r('app.string.bluetoothtest_str_gt_9')
    let btState = bluetoothManager.getState();
    if ( btState == bluetoothManager.BluetoothState.STATE_ON ) {
      if ( !gattClientInstance ) {
        message = "NO_GATT_CLIENT_OBJECT";
        promptAction.showToast({ message : message })
        return message;
      }
      gattClientInstance.close();
      message = $r('app.string.bluetoothtest_str_gc_3');
    } else if ( btState == bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
    } else {
      message = $r('app.string.bluetoothtest_str_spw');
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGattClientCloseManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGattClientCloseManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetServicesCallbackManager(): string {
  try {
    let message = $r('app.string.bluetoothtest_str_gt_6')
    let btState = bluetoothManager.getState();
    if ( btState == bluetoothManager.BluetoothState.STATE_ON ) {
      if ( !gattClientInstance ) {
        message = "NO_GATT_CLIENT_OBJECT";
        promptAction.showToast({ message : message })
        return message;
      }
      hilog.info(0x0000, 'BtTest', '%{public}s', "connectState ")
      hilog.info(0x0000, 'BtTest', '%{public}s', "getServices(call back) start-----------------")
      gattClientInstance.getServices((err , gattServices) => {
        if ( err.code != 0 ) {
          message = $r('app.string.bluetoothtest_str_gcbec') + err.code + $r('app.string.bluetoothtest_str_s_2') + gattServices;
          hilog.info(0x0000, 'BtTest', '%{public}s', "getServices(call back) error code:-----------------" + err.code + ",services :" + gattServices)
          promptAction.showToast({ message : message })
          //        return message;
        } else {
          message = $r('app.string.bluetoothtest_str_gcis') + "\n"
          message += $r('app.string.bluetoothtest_str_bssi') + gattServices.length + '\n';
          for ( let i = 0 ; i < gattServices.length ; i ++ ) {
            message += $r('app.string.bluetoothtest_str_si') + gattServices[ i ].serviceUuid + '\n';
          }
          promptAction.showToast({ message : message })
          AppStorage.setOrCreate("getServices" , message)
          hilog.info(0x0000, 'BtTest', '%{public}s', "getServices(Callback)----------" + message)
        }
      })
      hilog.info(0x0000, 'BtTest', '%{public}s', "getServices(call back) end-----------------")
    } else if ( btState == bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
    } else {
      message = $r('app.string.bluetoothtest_str_spw');
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetServicesCallbackManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetServicesCallbackManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetServicesPromiseManager(): string {
  try {
    let message = $r('app.string.bluetoothtest_str_gt_7')
    if ( !gattClientInstance ) {
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({ message : message })
      return message;
    }
    message = $r('app.string.bluetoothtest_str_gps');
    let btState = bluetoothManager.getState();
    if ( btState == bluetoothManager.BluetoothState.STATE_ON ) {
      hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.getServices,promise start----------")
      gattClientInstance.getServices().then(gattServices => {
        hilog.info(0x0000, 'BtTest', '%{public}s', "getServices(): Promise<Array<GattService>> start --------------")
        message = $r('app.string.bluetoothtest_str_gsn')
        message = $r('app.string.bluetoothtest_str_bssi') + gattServices.length + "\n";
        hilog.info(0x0000, 'BtTest', '%{public}s', "bluetoothManager services size is " + gattServices.length)
        for ( let i = 0 ; i < gattServices.length ; i ++ ) {
          message += $r('app.string.bluetoothtest_str_si') + gattServices[ i ].serviceUuid + '\n';
        }
        hilog.info(0x0000, 'BtTest', '%{public}s', "getServices(): Promise<Array<GattService>> end --------------")
        promptAction.showToast({ message : message })
      }).catch((err: BusinessError) => {
        hilog.info(0x0000, 'BtTest', '%{public}s', "testGetServicesPromiseManager err:" + err)
      })
      hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.getServices,promise end----------")
      message = $r('app.string.bluetoothtest_str_gpis');
    } else if ( btState == bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
    } else {
      message = $r('app.string.bluetoothtest_str_spw');
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetServicesPromiseManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetServicesPromiseManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testReadCharacteristicValueCallbackManager(): string {
  try {
    let serviceUUID: string = AppStorage.get('serviceUUID') !;
    let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
    let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
    let descriptorValue: string = AppStorage.get('descriptorValue') !;
    let message = $r('app.string.bluetoothtest_str_rt')
    let btState = bluetoothManager.getState();
    if ( btState === bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
      promptAction.showToast({ message : message })
      return message;
    }
    if ( !gattClientInstance ) {
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({ message : message })
      return message;
    }
    let descriptors: Array<bluetooth.BLEDescriptor> = [];
    let bufferCCC = new ArrayBuffer(8);
    let cccV = new Uint8Array(bufferCCC);
    cccV[ 0 ] = 66;
    let descriptor: bluetooth.BLEDescriptor = {
      serviceUuid : serviceUUID ,
      characteristicUuid : characteristicUUID ,
      descriptorUuid : descriptorUUID ,
      descriptorValue : string2ArrayBuffer(descriptorValue)
    }
    descriptors.push(descriptor);
    let bleCharacteristicDataIn: bluetooth.BLECharacteristic = {
      serviceUuid : serviceUUID ,
      characteristicUuid : characteristicUUID ,
      characteristicValue : bufferCCC ,
      descriptors : descriptors
    };
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.readCharacteristicValue ,callback start--------------")
    gattClientInstance.readCharacteristicValue(bleCharacteristicDataIn , (err , bleCharacteristicDataOut) => {
      if ( err.code != 0 ) {
        message = $r('app.string.bluetoothtest_str_rec') + err.code + $r('app.string.bluetoothtest_str_i') + serviceUUID;
        hilog.info(0x0000, 'BtTest', '%{public}s', message)
        promptAction.showToast({ message : message })
      }
      message = $r('app.string.bluetoothtest_str_cs_8') + "\n";
      message += $r('app.string.bluetoothtest_str_cu') + bleCharacteristicDataOut.characteristicUuid + "\n";
      hilog.info(0x0000, 'BtTest', '%{public}s', message)
      let value = new Uint8Array(bleCharacteristicDataOut.characteristicValue);
      for ( let i = 0 ; i < bleCharacteristicDataOut.characteristicValue.byteLength ; i ++ ) {
        message += $r('app.string.bluetoothtest_str_cv') + value[ i ] + "\n";
      }
      AppStorage.setOrCreate("read_CharacteristicValue" , message)
      promptAction.showToast({ message : message })
    });
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.readCharacteristicValue ,callback end--------------")
    message = $r('app.string.bluetoothtest_str_rcis');
    hilog.info(0x0000, 'BtTest', '%{public}s', message)
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testReadCharacteristicValueCallbackManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testReadCharacteristicValueCallbackManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testReadCharacteristicValuePromiseManager(): string {
  try {
    let serviceUUID: string = AppStorage.get('serviceUUID') !;
    let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
    let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
    let descriptorValue: string = AppStorage.get('descriptorValue') !;
    let message = $r('app.string.bluetoothtest_str_rt_1')
    let btState = bluetoothManager.getState();
    if ( btState === bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
      promptAction.showToast({ message : message })
      return message;
    }
    if ( !gattClientInstance ) {
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({ message : message })
      return message;
    }
    let descriptors: Array<bluetooth.BLEDescriptor> = [];
    let bufferCCC = new ArrayBuffer(8);
    let cccV = new Uint8Array(bufferCCC);
    cccV[ 0 ] = 66;
    let descriptor: bluetoothManager.BLEDescriptor = {
      serviceUuid : serviceUUID ,
      characteristicUuid : characteristicUUID ,
      descriptorUuid : descriptorUUID ,
      descriptorValue : string2ArrayBuffer(descriptorValue)
    }
    descriptors.push(descriptor);
    let bleCharacteristicDataIn: bluetoothManager.BLECharacteristic = {
      serviceUuid : serviceUUID ,
      characteristicUuid : characteristicUUID ,
      characteristicValue : bufferCCC ,
      descriptors : descriptors
    };
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.readCharacteristicValue ,promise start--------------")
    gattClientInstance.readCharacteristicValue(bleCharacteristicDataIn).then((bleCharacteristicDataOut) => {
      message = $r('app.string.bluetoothtest_str_psn')
      message += $r('app.string.bluetoothtest_str_cu') + bleCharacteristicDataOut.characteristicUuid + "\n";
      hilog.info(0x0000, 'BtTest', '%{public}s', message)
      let value = new Uint8Array(bleCharacteristicDataOut.characteristicValue);
      for ( let i = 0 ; i < bleCharacteristicDataOut.characteristicValue.byteLength ; i ++ ) {
        message += $r('app.string.bluetoothtest_str_cv') + value[ i ] + "\n";
      }
      promptAction.showToast({ message : message })
    }).catch((err: BusinessError) => {
      hilog.info(0x0000, 'BtTest', '%{public}s', "testReadCharacteristicValuePromiseManager err:" + err)
    });
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.readCharacteristicValue ,promise end--------------")
    message = $r('app.string.bluetoothtest_str_rpis');
    hilog.info(0x0000, 'BtTest', '%{public}s', message)
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testReadCharacteristicValuePromiseManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testReadCharacteristicValuePromiseManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testReadDescriptorValueCallbackManager(): string {
  try {
    let serviceUUID: string = AppStorage.get('serviceUUID') !;
    let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
    let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
    let descriptorValue: string = AppStorage.get('descriptorValue') !;
    let message = $r('app.string.bluetoothtest_str_rt_2')
    let btState = bluetoothManager.getState();
    if ( btState === bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
      promptAction.showToast({ message : message })
      return message;
    }
    if ( !gattClientInstance ) {
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({ message : message })
      return message;
    }
    let bufferDesc = new ArrayBuffer(8);
    let descV = new Uint8Array(bufferDesc);
    descV[ 0 ] = 88;
    let descriptorIn: Descriptor = {
      serviceUuid : serviceUUID ,
      characteristicUuid : characteristicUUID ,
      descriptorUuid : descriptorUUID ,
      //    descriptorValue: bufferDesc
      descriptorValue : string2ArrayBuffer(descriptorValue)
    };
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.readDescriptorValue,callback start--------------")
    gattClientInstance.readDescriptorValue(descriptorIn , (err: BusinessError , descriptorOut: bluetooth.BLEDescriptor) => {
      let message = ""
      if ( err.code != 0 ) {
        message = $r('app.string.bluetoothtest_str_re') + err.code.toString()
        hilog.info(0x0000, 'BtTest', '%{public}s', message)
        promptAction.showToast({ message : message })
      }
      message = "\n" + $r('app.string.bluetoothtest_str_cdu') + descriptorOut.descriptorUuid + "\n";
      let value = new Uint8Array(descriptorOut.descriptorValue);
      for ( let i = 0 ; i < descriptorOut.descriptorValue.byteLength ; i ++ ) {
        message += $r('app.string.bluetoothtest_str_dv') + value[ i ] + "\n";
      }
      AppStorage.setOrCreate("read_DescriptorValue" , message)
      promptAction.showToast({ message : message })
    });
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.readDescriptorValue ,callback end--------------")
    message = $r('app.string.bluetoothtest_str_rcis_1');
    hilog.info(0x0000, 'BtTest', '%{public}s', message)
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testReadDescriptorValueCallbackManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testReadDescriptorValueCallbackManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testReadDescriptorValuePromiseManager(): string {
  try {
    let serviceUUID: string = AppStorage.get('serviceUUID') !;
    let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
    let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
    let descriptorValue: string = AppStorage.get('descriptorValue') !;
    let message = $r('app.string.bluetoothtest_str_rt_3')
    let btState = bluetoothManager.getState();
    if ( btState === bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
      promptAction.showToast({ message : message })
      return message;
    }
    if ( !gattClientInstance ) {
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({ message : message })
      return message;
    }
    let bufferDesc = new ArrayBuffer(8);
    let descV = new Uint8Array(bufferDesc);
    descV[ 0 ] = 99;
    let descriptorIn: bluetoothManager.BLEDescriptor = {
      serviceUuid : serviceUUID ,
      characteristicUuid : characteristicUUID ,
      descriptorUuid : descriptorUUID ,
      descriptorValue : string2ArrayBuffer(descriptorValue)
    };
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.readDescriptorValue,promise start--------------")
    gattClientInstance.readDescriptorValue(descriptorIn).then((descriptorOut) => {
      message = "\n" + $r('app.string.bluetoothtest_str_pdu') + descriptorOut.descriptorUuid + "\n";
      let value = new Uint8Array(descriptorOut.descriptorValue);
      for ( let i = 0 ; i < descriptorOut.descriptorValue.byteLength ; i ++ ) {
        message += $r('app.string.bluetoothtest_str_dv') + value[ i ] + "\n";
      }
      AppStorage.setOrCreate("read_DescriptorValue" , message)
      promptAction.showToast({ message : message })
    }).catch((err: BusinessError) => {
      hilog.info(0x0000, 'BtTest', '%{public}s', "testReadDescriptorValuePromiseManager err:" + err)
    });
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.readDescriptorValue,promise end--------------")
    message = $r('app.string.bluetoothtest_str_rpis_1');
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testReadDescriptorValuePromiseManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testReadDescriptorValuePromiseManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testWriteCharacteristicValueManager(): string {
  try {
    let serviceUUID: string = AppStorage.get('serviceUUID') !;
    let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
    let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
    let descriptorValue: string = AppStorage.get('descriptorValue') !;
    let message = $r('app.string.bluetoothtest_str_wt')
    let btState = bluetoothManager.getState();
    if ( btState === bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
      promptAction.showToast({ message : message })
      return message;
    }
    if ( !gattClientInstance ) {
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({ message : message })
      return message;
    }
    let descriptors: Array<bluetooth.BLEDescriptor> = [];
    let bufferCCC = new ArrayBuffer(8);
    let cccV = new Uint8Array(bufferCCC);
    cccV[ 0 ] = 66;
    let descriptor: Descriptor = {
      serviceUuid : serviceUUID ,
      characteristicUuid : characteristicUUID ,
      descriptorUuid : descriptorUUID ,
      descriptorValue : string2ArrayBuffer(descriptorValue)
    }
    descriptors.push(descriptor);
    let bleCharacteristicDataIn: BleCharacteristicDataIn = {
      serviceUuid : serviceUUID ,
      characteristicUuid : characteristicUUID ,
      characteristicValue : bufferCCC ,
      descriptors : descriptors
    };
    gattClientInstance.writeCharacteristicValue(bleCharacteristicDataIn)
    message = $r('app.string.bluetoothtest_str_ws');
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testWriteCharacteristicValueManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testWriteCharacteristicValueManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function string2ArrayBuffer(str: string) {
  let array = new Uint8Array(str.length);
  for ( let i = 0 ; i < str.length ; i ++ ) {
    array[ i ] = str.charCodeAt(i);
  }
  return array.buffer
}

export function testWriteDescriptorValueManager(): string {
  try {
    let serviceUUID: string = AppStorage.get('serviceUUID') !;
    let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
    let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
    let descriptorValue: string = AppStorage.get('descriptorValue') !;
    let message: string = "writeDescriptorValue test"
    let btState = bluetoothManager.getState();
    if ( btState === bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
      promptAction.showToast({ message : message })
      return message;
    }
    if ( !gattClientInstance ) {
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({ message : message })
      return message;
    }
    let bufferDesc = string2ArrayBuffer(descriptorValue);
    let descriptor: Descriptor = {
      serviceUuid : serviceUUID ,
      characteristicUuid : characteristicUUID ,
      descriptorUuid : descriptorUUID ,
      descriptorValue : bufferDesc
    };
    gattClientInstance.writeDescriptorValue(descriptor)
    message = $r('app.string.bluetoothtest_str_ws_1');
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testWriteDescriptorValueManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testWriteDescriptorValueManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

//回调函数
let BLECharacteristicChange = false

export function testOnBLECharacteristicChangeManager(): string {
  try {
    let message = $r('app.string.bluetoothtest_str_ot_1')
    let btStatus = bluetoothManager.getState();
    if ( btStatus === bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
      promptAction.showToast({ message : message })
      return message;
    }
    if ( !gattClientInstance ) {
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({ message : message })
      return message;
    }

    if ( BLECharacteristicChange ) {
      gattClientInstance.off('BLECharacteristicChange' , () => {
        message = $r('app.string.bluetoothtest_str_ob')
        promptAction.showToast({ message : message })
      })
    } else {
      gattClientInstance.on('BLECharacteristicChange' , (data) => {
        let serviceUuid = data.serviceUuid;
        let characteristicUuid = data.characteristicUuid;
        let bleCharacteristicChangeInfo = "serviceUuid:" + serviceUuid + "\n";
        bleCharacteristicChangeInfo += "characteristicUuid:" + characteristicUuid + "\n";
        let value = new Uint8Array(data.characteristicValue);
        for ( let i = 0 ; i < data.characteristicValue.byteLength ; i ++ ) {
          bleCharacteristicChangeInfo += "\n" + 'value:' + value[ i ]
        }
        AppStorage.setOrCreate("bleCharacteristicChangeInfo" , bleCharacteristicChangeInfo)
        promptAction.showToast({ message : "on:BLECharacteristicChange" })
      })
      message = $r('app.string.bluetoothtest_str_ob_1')
    }
    BLECharacteristicChange = !BLECharacteristicChange
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testOnBLECharacteristicChangeManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnBLECharacteristicChangeManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testSetBLEMtuSizeManager(): string {
  try {
    let message = $r('app.string.bluetoothtest_str_st_21')
    let btStatus = bluetoothManager.getState();
    if ( btStatus === bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
      promptAction.showToast({ message : message })
      return message;
    }
    if ( !gattClientInstance ) {
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({ message : message })
      return message;
    }

    message = $r('app.string.bluetoothtest_str_sn_1');
    let mtuSize = 256;
    let ret = gattClientInstance.setBLEMtuSize(mtuSize);
    message += $r('app.string.bluetoothtest_str_r_1') + ret;
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testSetBLEMtuSizeManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSetBLEMtuSizeManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testSetNotifyCharacteristicChangedManager(): string {
  try {
    let serviceUUID: string = AppStorage.get('serviceUUID') !;
    let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
    let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
    let descriptorValue: string = AppStorage.get('descriptorValue') !;
    let message = $r('app.string.bluetoothtest_str_st')
    let btStatus = bluetoothManager.getState();
    if ( btStatus === bluetoothManager.BluetoothState.STATE_OFF ) {
      message = $r('app.string.bluetoothtest_str_bine');
      promptAction.showToast({ message : message })
      return message;
    }
    if ( !gattClientInstance ) {
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({ message : message })
      return message;
    }

    let descriptors: Array<bluetoothManager.BLEDescriptor> = [];
    let bufferCCC = new ArrayBuffer(8);
    let cccV = new Uint8Array(bufferCCC);
    cccV[ 0 ] = 66;
    let descriptor: bluetoothManager.BLEDescriptor = {
      serviceUuid : serviceUUID ,
      characteristicUuid : characteristicUUID ,
      descriptorUuid : descriptorUUID ,
      descriptorValue : string2ArrayBuffer(descriptorValue)
    }
    descriptors.push(descriptor);
    let characteristicIn: bluetoothManager.BLECharacteristic = {
      serviceUuid : serviceUUID ,
      characteristicUuid : characteristicUUID ,
      characteristicValue : bufferCCC ,
      descriptors : descriptors
    };
    if ( true ) {
      gattClientInstance.setNotifyCharacteristicChanged(characteristicIn , true)
      message = $r('app.string.bluetoothtest_str_sts')
    } else {
      message = $r('app.string.bluetoothtest_str_stf')
      if ( false ) {
        gattClientInstance.setNotifyCharacteristicChanged(characteristicIn , false)
        message = $r('app.string.bluetoothtest_str_sft')
      } else {
        message = $r('app.string.bluetoothtest_str_sff')
      }
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testSetNotifyCharacteristicChangedManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSetNotifyCharacteristicChangedManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetDeviceNameCallbackManager(): String {
  try {
    let deviceName = ""
    let message = $r('app.string.bluetoothtest_str_gt_2')
    if ( !gattClientInstance ) {
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({ message : message })
      return message;
    }
    //currentClick = 15;
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.getDeviceName,callback start-----------------")
    gattClientInstance.getDeviceName((err: BusinessError , asyncRemoteDeviceName) => {
      message = $r('app.string.bluetoothtest_str_rc_1')
      message += JSON.stringify(err);
      message += $r('app.string.bluetoothtest_str_adi')
      message += JSON.stringify(asyncRemoteDeviceName);
      deviceName = JSON.stringify(asyncRemoteDeviceName)
      AppStorage.setOrCreate("deviceName" , deviceName)
      promptAction.showToast({ message : message })
    });
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.getDeviceName,callback end-----------------")
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetDeviceNameCallbackManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetDeviceNameCallbackManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetDeviceNamePromiseManager(): String {
  try {
    let deviceName = ""
    let message = $r('app.string.bluetoothtest_str_gt_3')
    if ( !gattClientInstance ) {
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({ message : message })
      return message;
    }
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.getDeviceName,promise start-----------------")
    gattClientInstance.getDeviceName().then((peripheralDeviceName) => {
      message = $r('app.string.bluetoothtest_str_gp') + peripheralDeviceName;
      deviceName = peripheralDeviceName
      AppStorage.setOrCreate("deviceName" , deviceName)
      promptAction.showToast({ message : message })
    }).catch((err: BusinessError) => {
      hilog.info(0x0000, 'BtTest', '%{public}s', "testGetDeviceNamePromiseManager err:" + err)
    });
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.getDeviceName,promise end-----------------")
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetDeviceNamePromiseManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetDeviceNamePromiseManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetRssiValueCallbackManager(): String {
  try {
    let Rssi = ""
    let message = $r('app.string.bluetoothtest_str_gt_4')
    if ( !gattClientInstance ) {
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({ message : message })
      return message;
    }

    message = $r('app.string.bluetoothtest_str_gc');
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.getRssiValue,callback start-----------------")
    gattClientInstance.getRssiValue((err , cbRssi) => {
      message = $r('app.string.bluetoothtest_str_rc_1')
      message += JSON.stringify(err);
      message += $r('app.string.bluetoothtest_str_ar')
      message += JSON.stringify(cbRssi);
      Rssi = JSON.stringify(cbRssi);
      AppStorage.setOrCreate("Rssi" , Rssi)
      promptAction.showToast({ message : message })
    });
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.getRssiValue,callback end-----------------")
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetRssiValueCallbackManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetRssiValueCallbackManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetRssiValuePromiseManager(): String {
  try {
    let Rssi = ""
    let message = $r('app.string.bluetoothtest_str_gt_5')
    if ( !gattClientInstance ) {
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({ message : message })
      return message;
    }
    message = $r('app.string.bluetoothtest_str_gp_1');
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.getRssiValue,promise start-----------------")
    gattClientInstance.getRssiValue().then((rssiValue) => {
      message += 'rssi' + JSON.stringify(rssiValue)
      Rssi = JSON.stringify(rssiValue)
      AppStorage.setOrCreate("Rssi" , Rssi)
      promptAction.showToast({ message : message })
    }).catch((err: BusinessError) => {
      hilog.info(0x0000, 'BtTest', '%{public}s', "testGetRssiValuePromiseManager err:" + err)
    });
    hilog.info(0x0000, 'BtTest', '%{public}s', "gattClientInstance.getRssiValue,promise  end-----------------")
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGetRssiValuePromiseManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetRssiValuePromiseManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}