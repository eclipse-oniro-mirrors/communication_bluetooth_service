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
import LogUtil from '../../Utils/LogUtil';
import { resStr} from '../../Utils/resString';


/**
 * GattClient Interface Of Bluetooth test
 */

import bluetooth from '@ohos.bluetooth';
import BluetoothModel from './BluetoothModel'
import { BusinessError} from '@ohos.base';
import promptAction from '@ohos.promptAction';

interface Descriptor {
  serviceUuid: string;
  characteristicUuid: string;
  descriptorUuid: string;
  descriptorValue: ArrayBufferLike;
}

interface BleCharacteristicDataIn {
  serviceUuid: string;
  characteristicUuid: string;
  characteristicValue: ArrayBuffer;
  descriptors: Descriptor[];
}

interface Characteristic {
  serviceUuid: string;
  characteristicUuid: string;
  characteristicValue: ArrayBufferLike;
}

let gattClientInstance: bluetooth.GattClientDevice

export function testCreateGattClient(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ct_1'))
    let peripheralDeviceId: string = AppStorage.get('peripheralDeviceId') !;
    let btState = bluetooth.getState();
    if ( btState === bluetooth.BluetoothState.STATE_ON || btState === bluetooth.BluetoothState.STATE_BLE_ON ){
      if ( gattClientInstance ){
        message = resStr($r('app.string.bluetoothtest_str_ge_2'));
        promptAction.showToast({message: message})
        return message;
      }
      //注册回调
      bluetooth.BLE.on("BLEDeviceFind", (scanResult) => {
        scanResult.forEach(device => {
          let devId = device.deviceId;//6c:96:d7:3d:87:6f
          let rssi = device.rssi;
          let data = device.data;
          promptAction.showToast({message: "findDevice:" + devId})
          if ( devId == peripheralDeviceId ){
            bluetooth.BLE.stopBLEScan();
            // create ble client instance
            if ( !gattClientInstance ){
              gattClientInstance = BluetoothModel.createGattClient(devId);
              message = resStr($r('app.string.bluetoothtest_str_cs_4'));
              LogUtil.info("createGattClient succeeds----------")
            }
          }
        })
      })
      if ( peripheralDeviceId && peripheralDeviceId.length > 15 ){
        gattClientInstance = BluetoothModel.createGattClient(peripheralDeviceId.toUpperCase())
        if ( gattClientInstance ){
          message = resStr($r('app.string.bluetoothtest_str_c')) + peripheralDeviceId;
          LogUtil.info("createGattClient peripheralDeviceId succeeds----------")
        } else {
          message = resStr($r('app.string.bluetoothtest_str_cf'))
        }
      } else {
        message = resStr($r('app.string.bluetoothtest_str_pi'));
      }
      promptAction.showToast({message: message})
    } else if ( btState === bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
    } else {
      message = resStr($r('app.string.bluetoothtest_str_spw_1'))
      promptAction.showToast({message: message})
    }
    return message
  } catch( e ){
    LogUtil.info(`Failed to testCreateGattClient. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testCreateGattClient. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

let bleConnectionState = 0;

export function testConnect(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ct_4'));
    let btState = bluetooth.getState();
    if ( btState === bluetooth.BluetoothState.STATE_ON || btState === bluetooth.BluetoothState.STATE_BLE_ON ){
      if ( gattClientInstance ){
        if ( bleConnectionState == 0 ){
          LogUtil.info("GattClient.connect: start----------------")
          let ret: boolean = gattClientInstance.connect()
          message = resStr($r('app.string.bluetoothtest_str_gc_1')) + ret;
          LogUtil.info("GattClient.connect: end----------------" + ret)
          bleConnectionState = 2
        } else {
          message = resStr($r('app.string.bluetoothtest_str_tdind'))
        }
      } else {
        message = "NO_GATT_CLIENT_OBJECT";
      }
    } else if ( btState === bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
    } else {
      message = resStr($r('app.string.bluetoothtest_str_spw'));
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testConnect. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testConnect. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testDisconnect(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_td'))
    let btState = bluetooth.getState();
    if ( btState == bluetooth.BluetoothState.STATE_ON || btState == bluetooth.BluetoothState.STATE_BLE_ON ){
      if ( !gattClientInstance ){
        message = "NO_GATT_CLIENT_OBJECT";
        promptAction.showToast({message: message})
        return message;
      }
      if ( bleConnectionState == 2 ){
        let ret: boolean = gattClientInstance.disconnect();
        message = resStr($r('app.string.bluetoothtest_str_gd')) + ret;
      } else {
        message = resStr($r('app.string.bluetoothtest_str_bincanntdi'));
      }
    } else if ( btState == bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
    } else {
      message = resStr($r('app.string.bluetoothtest_str_spw'));
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testDisconnect. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testDisconnect. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

let BLEConnectionStateChange = false

export function testOnBLEConnectionStateChange(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot'))
    let btStatus = bluetooth.getState();
    if ( btStatus === bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
      return message;
    }
    if ( !gattClientInstance ){
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({message: message})
      return message;
    }
    message = " HAVE_GATT_CLIENT_OBJECT "
    if ( BLEConnectionStateChange ){
      gattClientInstance.off('BLEConnectionStateChange', () => {
        promptAction.showToast({message: "off:BLEConnectionStateChange"})
      })
      message = resStr($r('app.string.bluetoothtest_str_gbbljztbh'))
    } else {
      let bleConnectionStateInfo = "xxxx"
      gattClientInstance.on('BLEConnectionStateChange', (bleConnectionState: bluetooth.BLEConnectChangedState) => {
        switch ( bleConnectionState.state ){
          case bluetooth.ProfileConnectionState.STATE_DISCONNECTED:
            bleConnectionStateInfo = "DISCONNECTED";
            break;
          case bluetooth.ProfileConnectionState.STATE_CONNECTING:
            bleConnectionStateInfo = "CONNECTING";
            break;
          case bluetooth.ProfileConnectionState.STATE_CONNECTED:
            bleConnectionStateInfo = "STATE_CONNECTED";
            break;
          case bluetooth.ProfileConnectionState.STATE_DISCONNECTING:
            bleConnectionStateInfo = "STATE_DISCONNECTING";
            break;
          default:
            bleConnectionStateInfo = "UNDEFINED!";
            break;
        }
        LogUtil.info("gattClientInstance.onBLEConnectionStateChange is ------- " + JSON.stringify(bleConnectionState))
        AppStorage.setOrCreate('bleConnectionStateInfo', bleConnectionStateInfo)
        promptAction.showToast({message: resStr($r('app.string.bluetoothtest_str_zt')) + bleConnectionStateInfo})
      })
      message = resStr($r('app.string.bluetoothtest_str_dkbljztbh'))
    }
    BLEConnectionStateChange = !BLEConnectionStateChange
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testOnBLEConnectionStateChange. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnBLEConnectionStateChange. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testGattClientClose(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_gt_9'))
    let btState = bluetooth.getState()
    if ( btState == bluetooth.BluetoothState.STATE_ON || btState == bluetooth.BluetoothState.STATE_BLE_ON ){
      if ( !gattClientInstance ){
        message = "NO_GATT_CLIENT_OBJECT";
        promptAction.showToast({message: message})
        return message;
      }
      let ret: boolean = gattClientInstance.close();
      // if ( ret ) {
      //   gattClientInstance = null;
      // }
      message = resStr($r('app.string.bluetoothtest_str_gc_3')) + ret;
    } else if ( btState == bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
    } else {
      message = resStr($r('app.string.bluetoothtest_str_spw'));
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testGattClientClose. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGattClientClose. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testGetServicesCallback(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_gt_6'))
    let btState = bluetooth.getState()
    if ( btState == bluetooth.BluetoothState.STATE_ON || btState == bluetooth.BluetoothState.STATE_BLE_ON ){
      if ( !gattClientInstance ){
        message = "NO_GATT_CLIENT_OBJECT";
        promptAction.showToast({message: message})
        return message;
      }
      LogUtil.info("connectState ")
      LogUtil.info("getServices(call back) start-----------------")
      gattClientInstance.getServices((err: BusinessError, gattServices: Array<bluetooth.GattService>) => {
        if ( err.code != 0 ){
          message = resStr($r('app.string.bluetoothtest_str_gcbec')) + err.code + resStr($r('app.string.bluetoothtest_str_s_2')) + gattServices;
          LogUtil.info("getServices(call back) error code:-----------------" + err.code + ", services :" + gattServices)
          promptAction.showToast({message: message})
        //        return message;
        } else {
          let services: Array<bluetooth.GattService> = gattServices;
          message = resStr($r('app.string.bluetoothtest_str_gcis')) + "\n"
          LogUtil.info("bluetooth services size is " + String(services.length))
          message += resStr($r('app.string.bluetoothtest_str_bssi_1')) + services.length + '\n';
          for(let i = 0 ; i<services.length ; i++ ){
            message += resStr($r('app.string.bluetoothtest_str_si')) + services[ i ].serviceUuid + '\n';
          }
          promptAction.showToast({message: message})
          AppStorage.setOrCreate("getServices", message)
          LogUtil.info("getServices(Callback)----------" + message)
        }
      })
      LogUtil.info("getServices(call back) end-----------------")
    } else if ( btState == bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'))
    } else {
      message = resStr($r('app.string.bluetoothtest_str_spw'));
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testGetServicesCallback. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetServicesCallback. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testGetServicesPromise(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_gt_7'))
    if ( !gattClientInstance ){
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({message: message})
      return message;
    }
    message = resStr($r('app.string.bluetoothtest_str_gps'));
    let btState = bluetooth.getState();
    if ( btState == bluetooth.BluetoothState.STATE_ON || btState == bluetooth.BluetoothState.STATE_BLE_ON ){
      LogUtil.info("gattClientInstance.getServices,promise start----------")
      gattClientInstance.getServices().then((result: Array<bluetooth.GattService>) => {
        LogUtil.info("getServices(): Promise<Array<GattService>> start --------------")
        message = resStr($r('app.string.bluetoothtest_str_gsn'))
        hilog.info(0x0000, 'BtTest', '%{public}s', "getServices successfully:" + JSON.stringify(result))
        let services: Array<bluetooth.GattService> = result;
        message = resStr($r('app.string.bluetoothtest_str_bssi_1')) + services.length + "\n";
        LogUtil.info("bluetooth services size is " + services.length)
        for(let i = 0 ; i<services.length ; i++ ){
          message += resStr($r('app.string.bluetoothtest_str_si')) + services[ i ].serviceUuid + '\n';
        }
        LogUtil.info("getServices(): Promise<Array<GattService>> end --------------")
        promptAction.showToast({message: message})
      }).catch((err: BusinessError) => {
        LogUtil.info("testGetServicesPromise err:" + err)
      })
      LogUtil.info("gattClientInstance.getServices, promise end----------")
      message = resStr($r('app.string.bluetoothtest_str_gpis'))
    } else if ( btState == bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
    } else {
      message = resStr($r('app.string.bluetoothtest_str_spw'));
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testGetServicesPromise. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetServicesPromise. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testReadCharacteristicValueCallback(): string {
  try {
    let serviceUUID: string = AppStorage.get('serviceUUID') !;
    let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
    let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
    let descriptorValue: string = AppStorage.get('descriptorValue') !;
    let message = resStr($r('app.string.bluetoothtest_str_rt'))
    let btState = bluetooth.getState();
    if ( btState === bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
      return message;
    }
    if ( !gattClientInstance ){
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({message: message})
      return message;
    }
    let descriptors: Array<bluetooth.BLEDescriptor> = [];
    let bufferCCC = new ArrayBuffer(8);
    let cccV = new Uint8Array(bufferCCC);
    cccV[ 0 ] = 66;
    let descriptor: Descriptor = {
      serviceUuid: serviceUUID,
      characteristicUuid: characteristicUUID,
      descriptorUuid: descriptorUUID,
      descriptorValue: string2ArrayBuffer(descriptorValue)
    }
    descriptors.push(descriptor);
    let bleCharacteristicDataIn: BleCharacteristicDataIn = {
      serviceUuid: serviceUUID,
      characteristicUuid: characteristicUUID,
      characteristicValue: bufferCCC,
      descriptors: descriptors
    };
    LogUtil.info("gattClientInstance.readCharacteristicValue, callback start--------------")
    gattClientInstance.readCharacteristicValue(bleCharacteristicDataIn, (err: BusinessError, bleCharacteristicDataOut: bluetooth.BLECharacteristic) => {
      if ( err.code != 0 ){
        message = resStr($r('app.string.bluetoothtest_str_rec')) + err.code + resStr($r('app.string.bluetoothtest_str_i')) + serviceUUID;
        LogUtil.info(message)
        promptAction.showToast({message: message})
      }
      message = resStr($r('app.string.bluetoothtest_str_cs_8')) + "\n";
      message += resStr($r('app.string.bluetoothtest_str_cu')) + bleCharacteristicDataOut.characteristicUuid + "\n";
      LogUtil.info(message)
      let value = new Uint8Array(bleCharacteristicDataOut.characteristicValue)
      for(let i = 0 ; i<bleCharacteristicDataOut.characteristicValue.byteLength ; i++ ){
        message += resStr($r('app.string.bluetoothtest_str_cv')) + value[ i ] + "\n";
      }
      AppStorage.setOrCreate("read_CharacteristicValue", message)
      promptAction.showToast({message: message})
    });
    LogUtil.info("gattClientInstance.readCharacteristicValue ,callback end--------------")
    message = resStr($r('app.string.bluetoothtest_str_rcis'))
    LogUtil.info(message)
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testReadCharacteristicValueCallback. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testReadCharacteristicValueCallback. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testReadCharacteristicValuePromise(): string {
  try {
    let serviceUUID: string = AppStorage.get('serviceUUID') !;
    let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
    let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
    let descriptorValue: string = AppStorage.get('descriptorValue') !;
    let message = resStr($r('app.string.bluetoothtest_str_rt_1'))
    let btState = bluetooth.getState();
    if ( btState === bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
      return message;
    }
    if ( !gattClientInstance ){
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({message: message})
      return message;
    }
    let descriptors: Array<bluetooth.BLEDescriptor> = [];
    let bufferCCC = new ArrayBuffer(8);
    let cccV = new Uint8Array(bufferCCC);
    cccV[ 0 ] = 66;
    let descriptor: bluetooth.BLEDescriptor = {
      serviceUuid: serviceUUID,
      characteristicUuid: characteristicUUID,
      descriptorUuid: descriptorUUID,
      descriptorValue: string2ArrayBuffer(descriptorValue)
    }
    descriptors.push(descriptor);
    let bleCharacteristicDataIn: bluetooth.BLECharacteristic = {
      serviceUuid: serviceUUID,
      characteristicUuid: characteristicUUID,
      characteristicValue: bufferCCC,
      descriptors: descriptors
    };
    LogUtil.info("gattClientInstance.readCharacteristicValue, promise start--------------")
    gattClientInstance.readCharacteristicValue(bleCharacteristicDataIn)
      .then((bleCharacteristicDataOut: bluetooth.BLECharacteristic) => {
        message = resStr($r('app.string.bluetoothtest_str_psn'))
        message += resStr($r('app.string.bluetoothtest_str_cu')) + bleCharacteristicDataOut.characteristicUuid + "\n";
        LogUtil.info(message)
        let value = new Uint8Array(bleCharacteristicDataOut.characteristicValue)
        for(let i = 0 ; i<bleCharacteristicDataOut.characteristicValue.byteLength ; i++ ){
          message += resStr($r('app.string.bluetoothtest_str_cv')) + value[ i ] + "\n";
        }
        promptAction.showToast({message: message})
      })
      .catch((err: BusinessError) => {
        LogUtil.info("testReadCharacteristicValuePromise error :" + err)
      })
    LogUtil.info("gattClientInstance.readCharacteristicValue ,promise end--------------")
    message = resStr($r('app.string.bluetoothtest_str_rpis'))
    LogUtil.info(message)
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testReadCharacteristicValuePromise. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testReadCharacteristicValuePromise. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testReadDescriptorValueCallback(): string {
  try {
    let serviceUUID: string = AppStorage.get('serviceUUID') !;
    let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
    let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
    let descriptorValue: string = AppStorage.get('descriptorValue') !;
    let message = resStr($r('app.string.bluetoothtest_str_rt_2'))
    let btState = bluetooth.getState();
    if ( btState === bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
    }
    if ( !gattClientInstance ){
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({message: message})
    }
    let bufferDesc = new ArrayBuffer(8);
    let descV = new Uint8Array(bufferDesc);
    descV[ 0 ] = 88;
    let descriptorIn: Descriptor = {
      serviceUuid: serviceUUID,
      characteristicUuid: characteristicUUID,
      descriptorUuid: descriptorUUID,
      descriptorValue: string2ArrayBuffer(descriptorValue)
    };
    LogUtil.info("gattClientInstance.readDescriptorValue, callback start--------------")
    gattClientInstance.readDescriptorValue(descriptorIn, (err: BusinessError, descriptorOut: bluetooth.BLEDescriptor) => {
      let message = ""
      if ( err.code != 0 ){
        message = resStr($r('app.string.bluetoothtest_str_re')) + err.code.toString()
        LogUtil.info(message)
        promptAction.showToast({message: message})
      }
      message = "\n" + resStr($r('app.string.bluetoothtest_str_cdu')) + descriptorOut.descriptorUuid + "\n";
      let value = new Uint8Array(descriptorOut.descriptorValue);
      for(let i = 0 ; i<descriptorOut.descriptorValue.byteLength ; i++ ){
        message += resStr($r('app.string.bluetoothtest_str_dv')) + value[ i ] + "\n";
      }
      AppStorage.setOrCreate("read_DescriptorValue", message)
      promptAction.showToast({message: message})
    });
    LogUtil.info("gattClientInstance.readDescriptorValue ,callback end--------------")
    message = resStr($r('app.string.bluetoothtest_str_rcis_1'))
    LogUtil.info(message)
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testReadDescriptorValueCallback. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testReadDescriptorValueCallback. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testReadDescriptorValuePromise(): string {
  try {
    let serviceUUID: string = AppStorage.get('serviceUUID') !;
    let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
    let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
    let descriptorValue: string = AppStorage.get('descriptorValue') !;
    let message = resStr($r('app.string.bluetoothtest_str_rt_3'))
    let btState = bluetooth.getState();
    if ( btState === bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
      return message;
    }
    if ( !gattClientInstance ){
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({message: message})
      return message;
    }
    let bufferDesc = new ArrayBuffer(8);
    let descV = new Uint8Array(bufferDesc);
    descV[ 0 ] = 99;
    let descriptorIn: Descriptor = {
      serviceUuid: serviceUUID,
      characteristicUuid: characteristicUUID,
      descriptorUuid: descriptorUUID,
      //    descriptorValue: bufferDesc
      descriptorValue: string2ArrayBuffer(descriptorValue)
    };
    LogUtil.info("gattClientInstance.readDescriptorValue, promise start--------------")
    gattClientInstance.readDescriptorValue(descriptorIn).then((descriptorOut: bluetooth.BLEDescriptor) => {
      message = "\n" + resStr($r('app.string.bluetoothtest_str_pdu')) + descriptorOut.descriptorUuid + "\n";
      let value = new Uint8Array(descriptorOut.descriptorValue);
      for(let i = 0 ; i<descriptorOut.descriptorValue.byteLength ; i++ ){
        message += resStr($r('app.string.bluetoothtest_str_dv')) + value[ i ] + "\n";
      }
      AppStorage.setOrCreate("read_DescriptorValue", message)
      promptAction.showToast({message: message})
    }).catch((err: BusinessError) => {
      LogUtil.info("testReadDescriptorValuePromise error :" + err)
    })
    LogUtil.info("gattClientInstance.readDescriptorValue,promise end--------------")
    message = resStr($r('app.string.bluetoothtest_str_rpis_1'))
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testReadDescriptorValuePromise. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testReadDescriptorValuePromise. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testWriteCharacteristicValue(): string {
  try {
    let serviceUUID: string = AppStorage.get('serviceUUID') !;
    let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
    let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
    let descriptorValue: string = AppStorage.get('descriptorValue') !;
    let message: string = "writeCharacteristicValue test"
    let btState: number = bluetooth.getState();
    if ( btState === bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
      return message;
    }
    if ( !gattClientInstance ){
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({message: message})
      return message;
    }
    let descriptors: Array<bluetooth.BLEDescriptor> = [];
    let bufferCCC = new ArrayBuffer(8);
    let cccV = new Uint8Array(bufferCCC);
    cccV[ 0 ] = 66;
    let descriptor: Descriptor = {
      serviceUuid: serviceUUID,
      characteristicUuid: characteristicUUID,
      descriptorUuid: descriptorUUID,
      descriptorValue: string2ArrayBuffer(descriptorValue)
    }
    descriptors.push(descriptor);
    let bleCharacteristicDataIn: BleCharacteristicDataIn = {
      serviceUuid: serviceUUID,
      characteristicUuid: characteristicUUID,
      characteristicValue: bufferCCC,
      descriptors: descriptors
    };
    if ( gattClientInstance.writeCharacteristicValue(bleCharacteristicDataIn) ){
      message = resStr($r('app.string.bluetoothtest_str_ws'));
    } else {
      message = resStr($r('app.string.bluetoothtest_str_wf'));
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testWriteCharacteristicValue. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testWriteCharacteristicValue. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function string2ArrayBuffer(str: string){
  let array = new Uint8Array(str.length)
  for(let i = 0 ; i<str.length ; i++ ){
    array[ i ] = str.charCodeAt(i);
  }
  return array.buffer
}

export function testWriteDescriptorValue(): string {
  try {
    let serviceUUID: string = AppStorage.get('serviceUUID') !;
    let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
    let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
    let descriptorValue: string = AppStorage.get('descriptorValue') !;
    let message = resStr($r('app.string.bluetoothtest_str_wt_1'))
    let btState = bluetooth.getState();
    if ( btState === bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
      return message;
    }
    if ( !gattClientInstance ){
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({message: message})
      return message;
    }
    let bufferDesc = string2ArrayBuffer(descriptorValue);
    let descriptor: Descriptor = {
      serviceUuid: serviceUUID,
      characteristicUuid: characteristicUUID,
      descriptorUuid: descriptorUUID,
      descriptorValue: bufferDesc
    };
    if ( gattClientInstance.writeDescriptorValue(descriptor) ){
      message = resStr($r('app.string.bluetoothtest_str_ws_1'));
    }
    else {
      message = resStr($r('app.string.bluetoothtest_str_wf_1'));
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testWriteDescriptorValue. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testWriteDescriptorValue. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

//回调函数
let BLECharacteristicChange = false

export function testOnBLECharacteristicChange(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot_1'))
    let btStatus = bluetooth.getState()
    if ( btStatus === bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
      return message;
    }
    if ( !gattClientInstance ){
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({message: message})
      return message;
    }

    if ( BLECharacteristicChange ){
      gattClientInstance.off('BLECharacteristicChange', () => {
        message = resStr($r('app.string.bluetoothtest_str_ob'))
        promptAction.showToast({message: message})
      })
    } else {
      gattClientInstance.on('BLECharacteristicChange', (data: bluetooth.BLECharacteristic) => {
        let serviceUuid: string = data.serviceUuid;
        let characteristicUuid: string = data.characteristicUuid;
        let bleCharacteristicChangeInfo = "serviceUuid:" + serviceUuid + "\n";
        bleCharacteristicChangeInfo += "characteristicUuid:" + characteristicUuid + "\n";
        let value = new Uint8Array(data.characteristicValue);
        for(let i = 0 ; i<data.characteristicValue.byteLength ; i++ ){
          bleCharacteristicChangeInfo += "\n" + 'value:' + value[ i ]
        }
        AppStorage.setOrCreate("bleCharacteristicChangeInfo", bleCharacteristicChangeInfo)
        promptAction.showToast({message: "on:BLECharacteristicChange"})
      })
      message = resStr($r('app.string.bluetoothtest_str_ob_1'))
    }
    BLECharacteristicChange = !BLECharacteristicChange
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testOnBLECharacteristicChange. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnBLECharacteristicChange. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSetBLEMtuSize(): String {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_21'))
    let btStatus = bluetooth.getState()
    if ( btStatus === bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
      return message;
    }
    if ( !gattClientInstance ){
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({message: message})
      return message;
    }

    message = resStr($r('app.string.bluetoothtest_str_sn_1'));
    let mtuSize = 256;
    let ret: boolean = gattClientInstance.setBLEMtuSize(mtuSize);
    message += resStr($r('app.string.bluetoothtest_str_r_1')) + ret;
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testSetBLEMtuSize. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSetBLEMtuSize. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSetNotifyCharacteristicChanged(): String {
  try {
    let serviceUUID: string = AppStorage.get('serviceUUID') !;
    let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
    let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
    let descriptorValue: string = AppStorage.get('descriptorValue') !;
    let message = resStr($r('app.string.bluetoothtest_str_st'))
    let btStatus = bluetooth.getState();
    if ( btStatus === bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
      return message;
    }
    if ( !gattClientInstance ){
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({message: message})
      return message;
    }

    let descriptors: Array<bluetooth.BLEDescriptor> = [];
    let bufferCCC = new ArrayBuffer(8);
    let cccV = new Uint8Array(bufferCCC);
    cccV[ 0 ] = 66;
    let descriptor: Descriptor = {
      serviceUuid: serviceUUID,
      characteristicUuid: characteristicUUID,
      descriptorUuid: descriptorUUID,
      descriptorValue: string2ArrayBuffer(descriptorValue)
    }
    descriptors.push(descriptor);
    let characteristicIn: BleCharacteristicDataIn = {
      serviceUuid: serviceUUID,
      characteristicUuid: characteristicUUID,
      characteristicValue: bufferCCC,
      descriptors: descriptors
    };
    if ( gattClientInstance.setNotifyCharacteristicChanged(characteristicIn, true) ){
      message = resStr($r('app.string.bluetoothtest_str_sts'))
    } else {
      message = resStr($r('app.string.bluetoothtest_str_stf'))
      if ( gattClientInstance.setNotifyCharacteristicChanged(characteristicIn, false) ){
        message = resStr($r('app.string.bluetoothtest_str_sft'))
      } else {
        message = resStr($r('app.string.bluetoothtest_str_sff'))
      }
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testSetNotifyCharacteristicChanged. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSetNotifyCharacteristicChanged. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testGetDeviceNameCallback(): String {
  try {
    let deviceName = ""
    let message = resStr($r('app.string.bluetoothtest_str_gt_2'))
    if ( !gattClientInstance ){
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({message: message})
      return message;
    }
    //currentClick = 15;
    LogUtil.info("gattClientInstance.getDeviceName,callback start-----------------")
    gattClientInstance.getDeviceName((err: BusinessError, asyncRemoteDeviceName: string) => {
      message = resStr($r('app.string.bluetoothtest_str_rc_1'))
      message += JSON.stringify(err)
      message += $r('app.string.bluetoothtest_str_adi')
      message += JSON.stringify(asyncRemoteDeviceName);
      deviceName = JSON.stringify(asyncRemoteDeviceName)
      AppStorage.setOrCreate("deviceName", deviceName)
      promptAction.showToast({message: message})
    });
    LogUtil.info("gattClientInstance.getDeviceName, callback end-----------------")
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testGetDeviceNameCallback. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetDeviceNameCallback. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testGetDeviceNamePromise(): String {
  try {
    let deviceName = ""
    let message = resStr($r('app.string.bluetoothtest_str_gt_3'))
    if ( !gattClientInstance ){
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({message: message})
      return message;
    }
    LogUtil.info("gattClientInstance.getDeviceName, promise start-----------------")
    gattClientInstance.getDeviceName().then((peripheralDeviceName: string) => {
      message = resStr($r('app.string.bluetoothtest_str_gp')) + peripheralDeviceName;
      deviceName = peripheralDeviceName
      AppStorage.setOrCreate("deviceName", deviceName)
      promptAction.showToast({message: message})
    }).catch((err: BusinessError) => {
      LogUtil.info("testGetDeviceNamePromise error :" + err)
    })
    LogUtil.info("gattClientInstance.getDeviceName, promise end-----------------")
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testGetDeviceNamePromise. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetDeviceNamePromise. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testGetRssiValueCallback(): String {
  try {
    let Rssi = ""
    let message = resStr($r('app.string.bluetoothtest_str_gt_4'))
    if ( !gattClientInstance ){
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({message: message})
      return message;
    }

    message = resStr($r('app.string.bluetoothtest_str_gc'));
    LogUtil.info("gattClientInstance.getRssiValue,callback start-----------------")
    gattClientInstance.getRssiValue((err: BusinessError, cbRssi: number) => {
      message = resStr($r('app.string.bluetoothtest_str_rc_1'))
      message += JSON.stringify(err)
      message += $r('app.string.bluetoothtest_str_ar')
      message += JSON.stringify(cbRssi);
      Rssi = JSON.stringify(cbRssi);
      AppStorage.setOrCreate("Rssi", Rssi)
      promptAction.showToast({message: message})
    });
    LogUtil.info("gattClientInstance.getRssiValue, callback end-----------------")
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testGetRssiValueCallback. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetRssiValueCallback. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testGetRssiValuePromise(): String {
  try {
    let Rssi = ""
    let message = resStr($r('app.string.bluetoothtest_str_gt_5'))
    if ( !gattClientInstance ){
      message = "NO_GATT_CLIENT_OBJECT";
      promptAction.showToast({message: message})
      return message;
    }
    message = resStr($r('app.string.bluetoothtest_str_gp_1'));
    LogUtil.info("gattClientInstance.getRssiValue,promise start-----------------")
    gattClientInstance.getRssiValue().then((rssiValue: number) => {
      message += 'rssi' + JSON.stringify(rssiValue)
      Rssi = JSON.stringify(rssiValue)
      AppStorage.setOrCreate("Rssi", Rssi)
      promptAction.showToast({message: message})
    }).catch((err: BusinessError) => {
      LogUtil.info("testGetRssiValuePromise error :" + err)
    })
    LogUtil.info("gattClientInstance.getRssiValue, promise  end-----------------")
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testGetRssiValuePromise. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetRssiValuePromise. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}