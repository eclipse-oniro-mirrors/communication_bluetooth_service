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
import LogUtil from '../../Utils/LogUtil';
import { resStr} from '../../Utils/resString';



/**
 * BLE Test interface Of Bluetooth test
 */

import BluetoothModel from './BluetoothModel'
import bluetooth from '@ohos.bluetooth';
import promptAction from '@ohos.promptAction';
import { BleScanFilter, CurScanOptions} from '../../Utils/common'

let gattServerInstance: bluetooth.GattServer;

export function testCreateGattServerDevice(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ct'))
    let btState = bluetooth.getState();
    if ( btState == bluetooth.BluetoothState.STATE_ON || btState == bluetooth.BluetoothState.STATE_BLE_ON ){
      message = resStr($r('app.string.bluetoothtest_str_cs_6'));
      if ( !gattServerInstance ){
        gattServerInstance = BluetoothModel.createGattServer();
        message = resStr($r('app.string.bluetoothtest_str_cs_2'));
      } else {
        message = resStr($r('app.string.bluetoothtest_str_ge'));
      }
    } else if ( btState == bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
    } else {
      message = resStr($r('app.string.bluetoothtest_str_spw'));
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testCreateGattServerDevice. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testCreateGattServerDevice. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

let gattClientDeviceInstance: bluetooth.GattClientDevice

export function testCreateGattClientDevice(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_tc'))
    let btState = bluetooth.getState()
    let peripheralDeviceId: string = AppStorage.get('peripheralDeviceId') !;
    if ( btState == bluetooth.BluetoothState.STATE_ON || btState == bluetooth.BluetoothState.STATE_BLE_ON ){
      message = resStr($r('app.string.bluetoothtest_str_cs_7'))
      if ( !gattClientDeviceInstance ){
        gattClientDeviceInstance = BluetoothModel.createGattClient(peripheralDeviceId);
        message = resStr($r('app.string.bluetoothtest_str_cs_5'));
      } else {
        message = resStr($r('app.string.bluetoothtest_str_ge_1'));
      }
    } else if ( btState == bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
    } else {
      message = resStr($r('app.string.bluetoothtest_str_spw'));
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testCreateGattClientDevice. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testCreateGattClientDevice. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testGetConnectedBLEDevices(): string {
  try {
    let connectedBleDevices = BluetoothModel.getConnectedBLEDevices()
    let message = "getConnectedBLEDevices" + JSON.stringify(connectedBleDevices);
    LogUtil.info("getConnectedBLEDevices" + JSON.stringify(connectedBleDevices))
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testGetConnectedBLEDevices. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetConnectedBLEDevices. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testStartBLEScan(): string {
  try {
    LogUtil.info("StartBLEScan -------------------------------")
    let btState = bluetooth.getState()
    let message = resStr($r('app.string.bluetoothtest_str_ts_1'))
    if ( btState == bluetooth.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      return message;
    }
    let txtScanFilter_deviceId: string = AppStorage.get('txtScanFilterDeviceId') !;
    let txtScanFilter_name: string = AppStorage.get('txtScanFilterName') !;
    let txtScanFilter_serviceUuid: string = AppStorage.get('txtScanFilterServiceUuid') !;
    let txtScanOptions_interval: string = AppStorage.get('txtScanOptions_interval') !;
    let txtScanOptions_dutyMode: string = AppStorage.get('txtScanOptionsDutyMode') !;
    let txtScanOptions_matchMode: string = AppStorage.get('txtScanOptionsMatchMode') !;
    let cbxBleScanFilter: boolean = AppStorage.get('cbxBleScanFilter') !;
    let cbxBleScanOptions: boolean = AppStorage.get('cbxBleScanOptions') !;
    let curScanFilters: Array<bluetooth.ScanFilter> = [];
    let bleScanFilter: BleScanFilter = {
      deviceId: txtScanFilter_deviceId,
      name: txtScanFilter_name,
      serviceUuid: txtScanFilter_serviceUuid
    }
    let curScanOptions: CurScanOptions = {
      interval: parseInt(txtScanOptions_interval),
      dutyMode: parseInt(txtScanOptions_dutyMode),
      matchMode: parseInt(txtScanOptions_matchMode)
    }
    if ( cbxBleScanFilter ){
      curScanFilters.push(bleScanFilter);
    }

    if ( !cbxBleScanOptions ){
      if ( cbxBleScanFilter ){
        BluetoothModel.startBLEScan(curScanFilters);
        message = resStr($r('app.string.bluetoothtest_str_sf')) + JSON.stringify(curScanFilters);
      } else {
        BluetoothModel.startBLEScan(null);
        message = resStr($r('app.string.bluetoothtest_str_sn'));
      }
    } else {
      BluetoothModel.startBLEScan(curScanFilters, curScanOptions);
      message = resStr($r('app.string.bluetoothtest_str_sf')) + JSON.stringify(curScanFilters) + "\n" + resStr($r('app.string.bluetoothtest_str_o')) + JSON.stringify(curScanOptions);
    }
    LogUtil.info("StartBLEScan end-------------------------------")
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testStartBLEScan. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testStartBLEScan. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testStopBLEScan(): string {
  try {
    BluetoothModel.stopBLEScan()
    let message = resStr($r('app.string.bluetoothtest_str_so'));
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testStopBLEScan. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testStopBLEScan. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

let bleDeviceFind = false

export function testOnBLEDeviceFind(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot_10'))
    if ( bleDeviceFind ){
      BluetoothModel.unsubscribeBLEDeviceFind()
      message = resStr($r('app.string.bluetoothtest_str_gbbczztbh'))
      promptAction.showToast({message: message})
    } else {
      BluetoothModel.subscribeBLEDeviceFind((deviceItems: Array<bluetooth.ScanResult>) => {
        LogUtil.info("BLE Test:" + 'available ble devices changed.');
        let message = ""
        let availBleDeviceIds: Array<string> = []
        deviceItems?.forEach(deviceItem => {
          let newDevice = deviceItem.deviceId;
          message += resStr($r('app.string.bluetoothtest_str_m')) + newDevice;
          LogUtil.info("BLE Test:" + 'available bluetooth new device : ' + newDevice);
          availBleDeviceIds.push(newDevice);
        })
        AppStorage.setOrCreate('bluetoothAvailableDevices', availBleDeviceIds);
        promptAction.showToast({message: message})
      })
      message = resStr($r('app.string.bluetoothtest_str_dkbczztbh'))
      promptAction.showToast({message: message})
    }
    bleDeviceFind = !bleDeviceFind
    return message
  } catch( e ){
    LogUtil.info(`Failed to testOnBLEDeviceFind. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnBLEDeviceFind. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}
