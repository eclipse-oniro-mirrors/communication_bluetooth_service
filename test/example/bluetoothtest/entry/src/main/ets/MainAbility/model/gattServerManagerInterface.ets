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


import bluetoothManager from '@ohos.bluetoothManager';
import BluetoothModelManager from './BluetoothModelManager'
import promptAction from '@ohos.promptAction';
import bluetooth from '@ohos.bluetooth';

let gattServerInstance: bluetoothManager.GattServer

export function testCreateGattServerManager(): string {
  try {
    let btState = bluetoothManager.getState();
    let message = resStr($r('app.string.bluetoothtest_str_ct_2'))
    if ( btState === bluetoothManager.BluetoothState.STATE_ON ){
      if ( gattServerInstance ){
        message = resStr($r('app.string.bluetoothtest_str_ge_3'));
        promptAction.showToast({message: message})
        return message
      }
      LogUtil.info("CreateGattServer start -----------")
      gattServerInstance = BluetoothModelManager.createGattServer()
      message = resStr($r('app.string.bluetoothtest_str_cs_3'));
      LogUtil.info("CreateGattServer:succeeds!-----------")
      promptAction.showToast({message: message})
      return message
    } else if ( btState === bluetoothManager.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'))
    } else {
      message = resStr($r('app.string.bluetoothtest_str_sw'));
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testCreateGattServerManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testCreateGattServerManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testAddServiceManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_at'))
    let btStatus = bluetoothManager.getState()
    if ( btStatus === bluetoothManager.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
      return message
    }
    if ( !gattServerInstance ){
      message = "NO_GATT_Server_OBJECT";
      promptAction.showToast({message: message})
      return message
    }
    // build service
    // 调这个本地函数 buildGattService();
    let curServiceUUID: string = AppStorage.get("curServiceUUID") !
    let curCharacteristicUUID: string = AppStorage.get("curCharacteristicUUID") !
    let curDescriptorUUID: string = AppStorage.get("curDescriptorUUID") !
    let curDescriptorValue: string = AppStorage.get("curDescriptorValue") !
    let curCharacteristicValue: string = AppStorage.get("curCharacteristicValue") !
    let isPrimary: boolean = AppStorage.get("isPrimary") !
    let descriptors: Array<bluetoothManager.BLEDescriptor> = [];
    let descriptor: bluetoothManager.BLEDescriptor = {
      serviceUuid: curServiceUUID,
      characteristicUuid: curCharacteristicUUID,
      descriptorUuid: curDescriptorUUID,
      descriptorValue: string2ArrayBuffer(curDescriptorValue)
    };
    descriptors.push(descriptor);

    // build characteristics
    let characteristics: Array<bluetoothManager.BLECharacteristic> = [];
    let characteristic: bluetoothManager.BLECharacteristic = {
      serviceUuid: curServiceUUID,
      characteristicUuid: curCharacteristicUUID,
      characteristicValue: string2ArrayBuffer(curCharacteristicValue),
      descriptors: descriptors
    };
    characteristics.push(characteristic);
    let curGattService: bluetoothManager.GattService = {
      serviceUuid: curServiceUUID,
      isPrimary: isPrimary,
      characteristics: characteristics,
      includeServices: []
    }
    LogUtil.info("gattServerInstance.addService start -----------")
    let ret = gattServerInstance!.addService(curGattService)
    message = resStr($r('app.string.bluetoothtest_str_ra')) + ret;
    LogUtil.info("gattServerInstance.addService succeeds!-----------" + message)
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testAddServiceManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testAddServiceManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testRemoveServiceManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_rt_4'))
    let curServiceUUID: string = AppStorage.get("curServiceUUID") !
    let btState = bluetoothManager.getState()
    if ( btState == bluetoothManager.BluetoothState.STATE_ON ){
      if ( !gattServerInstance ){
        message = "NO_GATT_Server_OBJECT";
        promptAction.showToast({message: message})
        return message
      }
      message = resStr($r('app.string.bluetoothtest_str_rr'));
      gattServerInstance.removeService(curServiceUUID)
      message += 'true';
    } else if ( btState == bluetoothManager.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
    } else {
      message = resStr($r('app.string.bluetoothtest_str_spw'));
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testRemoveServiceManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testRemoveServiceManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

let curAdvData: bluetoothManager.AdvertiseData = {
  serviceUuids: [ "00003344-0000-1000-8000-00805f9b34fb", "00008899-0000-1000-8000-00805f9b34fb" ],
  manufactureData: [ {
    manufactureId: 7456,
    manufactureValue: string2ArrayBuffer("OHOS CHN")
  } ],
  serviceData: [ {
    serviceUuid: "000054F6-0000-1000-8000-00805f9b34fb",
    serviceValue: string2ArrayBuffer("OhosBle!")
  }, {
    serviceUuid: "00009999-0000-1000-8000-00805f9b34fb",
    serviceValue: string2ArrayBuffer("dudu_tiger")
  } ]
};

let curAdvResponse: bluetoothManager.AdvertiseData = {
  serviceUuids: [//"000054F6-0000-1000-8000-00805f9b34fb",
    "00009999-0000-1000-8000-00805f9b34fb" ],
  manufactureData: [ {
    manufactureId: 6547,
    manufactureValue: string2ArrayBuffer("OHOS CHN")
  } ],
  serviceData: [ {
    serviceUuid: "000054F6-0000-1000-8000-00805f9b34fb",
    serviceValue: string2ArrayBuffer("OhosBle!")
  }, {
    serviceUuid: "00007456-0000-1000-8000-00805f9b34fb",
    serviceValue: string2ArrayBuffer("OhosBle")
  } ]
};

export function testStartAdvertisingManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_13'))
    let btState = bluetoothManager.getState();
    if ( btState === bluetoothManager.BluetoothState.STATE_ON ){
      if ( !gattServerInstance ){
        message = "NO_GATT_Server_OBJECT";
        promptAction.showToast({message: message})
        return message
      }
      message = resStr($r('app.string.bluetoothtest_str_sbipi'));
      // 调三个本地函数
      // buildAdvSettings();
      let cbxAdvSettings: boolean = AppStorage.get("cbxAdvSettings") !
      let cbxInterval: boolean = AppStorage.get("cbxInterval") !
      let cbxTxPower: boolean = AppStorage.get("cbxTxPower") !
      let curAdvSettings: bluetooth.AdvertiseSetting = {
        interval: 800,//500ms
        txPower: - 7,
        connectable: true
      }
      if ( cbxAdvSettings ){
        if ( cbxInterval ){
          let advSettings_interval: string = AppStorage.get("advSettings_interval") !
          curAdvSettings.interval = parseInt(advSettings_interval)
        }
        if ( cbxTxPower ){
          let advSettings_txPower: string = AppStorage.get("advSettings_txPower") !
          curAdvSettings.txPower = parseInt(advSettings_txPower)
        }
        let advSettings_connectable: boolean = AppStorage.get("advSettings_connectable") !
        curAdvSettings.connectable = advSettings_connectable
      }

      //    buildAdvData();
      let advDataServiceUuids_serviceUuid: string = AppStorage.get("advDataServiceUuids_serviceUuid") !
      let advDataManufactureData_manufactureId: string = AppStorage.get("advDataManufactureData_manufactureId") !
      let advDataManufactureData_manufactureValue: string = AppStorage.get("advDataManufactureData_manufactureValue") !
      let advDataServiceData_serviceUuid: string = AppStorage.get("advDataServiceData_serviceUuid") !
      let advDataServiceData_serviceValue: string = AppStorage.get("advDataServiceData_serviceValue") !
      let advDataServiceUuidList: Array<string> = []
      let advDataManufactureDataList: Array<bluetoothManager.ManufactureData> = []
      let advDataServiceDataList: Array<bluetoothManager.ServiceData> = []
      let advDataManufactureDataInstance: bluetoothManager.ManufactureData = {
        manufactureId: parseInt(advDataManufactureData_manufactureId),
        manufactureValue: string2ArrayBuffer(advDataManufactureData_manufactureValue)
      }
      let advDataServiceDataInstance: bluetoothManager.ServiceData = {
        serviceUuid: advDataServiceData_serviceUuid,
        serviceValue: string2ArrayBuffer(advDataServiceData_serviceValue)
      }
      advDataServiceUuidList.push(advDataServiceUuids_serviceUuid)
      advDataManufactureDataList.push(advDataManufactureDataInstance)
      advDataServiceDataList.push(advDataServiceDataInstance)

      let curAdvData: bluetoothManager.AdvertiseData = {
        serviceUuids: advDataServiceUuidList,
        manufactureData: advDataManufactureDataList,
        serviceData: advDataServiceDataList
      }

      //    buildAdvResponse();
      let advResponseServiceUuids_serviceUuid: string = AppStorage.get("advResponseServiceUuids_serviceUuid") !
      let advResponseManufactureData_manufactureId: string = AppStorage.get("advResponseManufactureData_manufactureId") !
      let advResponseManufactureData_manufactureValue: string = AppStorage.get("advResponseManufactureData_manufactureValue") !
      let advResponseServiceData_serviceUuid: string = AppStorage.get("advResponseServiceData_serviceUuid") !
      let advResponseServiceData_serviceValue: string = AppStorage.get("advResponseServiceData_serviceValue") !
      let advResponseServiceUuidList: Array<string> = []
      let advResponseManufactureDataList: Array<bluetoothManager.ManufactureData> = []
      let advResponseServiceDataList: Array<bluetoothManager.ServiceData> = []
      let advResponseManufactureDataInstance: bluetoothManager.ManufactureData = {
        manufactureId: parseInt(advResponseManufactureData_manufactureId),
        manufactureValue: string2ArrayBuffer(advResponseManufactureData_manufactureValue)
      }
      let advResponseServiceDataInstance: bluetoothManager.ServiceData = {
        serviceUuid: advResponseServiceData_serviceUuid,
        serviceValue: string2ArrayBuffer(advResponseServiceData_serviceValue)
      }

      advResponseServiceUuidList.push(advResponseServiceUuids_serviceUuid)
      advResponseManufactureDataList.push(advResponseManufactureDataInstance)
      advResponseServiceDataList.push(advResponseServiceDataInstance)

      let curAdvResponse: bluetoothManager.AdvertiseData = {
        serviceUuids: advResponseServiceUuidList,
        manufactureData: advResponseManufactureDataList,
        serviceData: advResponseServiceDataList
      }

      let setting = curAdvSettings;
      let advData = curAdvData;
      let advResponse = curAdvResponse;
      let cbxAdvResponse: boolean = AppStorage.get("cbxAdvResponse") !
      message = resStr($r('app.string.bluetoothtest_str_sbipo'));

      if ( !cbxAdvResponse ){
        LogUtil.info("advData.serviceUuids:" + advData.serviceUuids + "\n")
        LogUtil.info("advData.manufactureData:" + advData.manufactureData + (typeof advData.manufactureData) + "\n")
        LogUtil.info("advData.serviceData:" + advData.serviceData + "\n")
        gattServerInstance!.startAdvertising(setting, advData)
        LogUtil.info("startAdvertising(setting,advData)-----------")
      } else {
        LogUtil.info("advResponse.serviceUuids:" + advResponse.serviceUuids + "\n")
        LogUtil.info("advResponse.manufactureData:" + advResponse.manufactureData + "\n")
        LogUtil.info("advResponse.serviceData:" + advResponse.serviceData + "\n")
        gattServerInstance!.startAdvertising(setting, advData, advResponse)
        LogUtil.info("startAdvertising(setting,advData,advResponse)-----------")
      }
      LogUtil.info("cbxAdvSettings:" + cbxAdvSettings + "\n")
      LogUtil.info("cbxAdvResponse:" + cbxAdvResponse + "\n")
      message = resStr($r('app.string.bluetoothtest_str_ss')) + cbxAdvSettings + "," + cbxAdvResponse;
    } else if ( btState === bluetoothManager.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
    } else {
      message = resStr($r('app.string.bluetoothtest_str_spw'));
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testStartAdvertisingManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testStartAdvertisingManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testStopAdvertisingManager(): String {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_16'))
    let btState = bluetoothManager.getState()
    if ( btState == bluetoothManager.BluetoothState.STATE_ON ){
      if ( !gattServerInstance ){
        message = "NO_GATT_Server_OBJECT";
        promptAction.showToast({message: message})
        return message
      }
      gattServerInstance!.stopAdvertising();
      message = resStr($r('app.string.bluetoothtest_str_ss_1'));
    } else if ( btState == bluetoothManager.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
    } else {
      message = resStr($r('app.string.bluetoothtest_str_spw'));
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testStopAdvertisingManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testStopAdvertisingManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testGattServerCloseManager(): String {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_gt_10'))
    let btState = bluetoothManager.getState()
    if ( btState == bluetoothManager.BluetoothState.STATE_ON ){
      if ( !gattServerInstance ){
        message = "NO_GATT_Server_OBJECT";
        promptAction.showToast({message: message})
        return message
      }
      gattServerInstance!.close();
      message = resStr($r('app.string.bluetoothtest_str_gcs'));
    } else if ( btState == bluetoothManager.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
    } else {
      message = resStr($r('app.string.bluetoothtest_str_spw'));
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testGattServerCloseManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGattServerCloseManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

//回调函数
let connectStateChange = false

export function testOnConnectStateChangeManager(): String {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot_6'))
    let btStatus = bluetoothManager.getState()
    if ( btStatus === bluetoothManager.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
      return message
    }
    if ( connectStateChange ){
      gattServerInstance!.off("connectStateChange", () => {
      })
      message = resStr($r('app.string.bluetoothtest_str_gbljztbh'))
      promptAction.showToast({message: message})
      return message
    } else {
      gattServerInstance!.on('connectStateChange', (data) => {
        let connectStateMsg = "deviceId:" + data.deviceId + ",connectState:" + data.state;
        AppStorage.setOrCreate("connect_StateMsg", data.state)
        promptAction.showToast({message: connectStateMsg})
      })
      message = resStr($r('app.string.bluetoothtest_str_dkljztbh'))
    }
    connectStateChange = !connectStateChange
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testOnConnectStateChangeManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnConnectStateChangeManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

//回调函数
let characteristicRead = false

export function testOnCharacteristicReadManager(): String {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot_5'))
    let btStatus = bluetoothManager.getState()
    if ( btStatus === bluetoothManager.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
      return message
    }
    if ( characteristicRead ){
      gattServerInstance!.off("characteristicRead", () => {
      })
      message = resStr($r('app.string.bluetoothtest_str_tzzdqgb'))
      promptAction.showToast({message: message})
      return message
    } else {
      gattServerInstance!.on('characteristicRead', (characteristicReadReq) => {

        let characteristicReadMsg = 'characteristicRead received!';
        // client address
        let clientDeviceId = characteristicReadReq.deviceId;
        // transId
        let transId = characteristicReadReq.transId;
        // the byte offset of the start position for reading characteristic value
        let offset = characteristicReadReq.offset;
        // service/characteristic Uuid
        let characteristicUuid = characteristicReadReq.characteristicUuid;
        let serviceUuid = characteristicReadReq.serviceUuid;

        characteristicReadMsg += 'characteristicRead input parameters: deviceId = ' + clientDeviceId + '\n';
        characteristicReadMsg += 'transId = ' + transId + '\n';
        characteristicReadMsg += "characteristicUuid = " + characteristicUuid + '\n';
        characteristicReadMsg += "serviceUuid = " + serviceUuid + '\n';
        AppStorage.setOrCreate("characteristicReadMsg", characteristicReadMsg)// build data for response
        let arrayBufferCCC = string2ArrayBuffer('DaYuBlue');

        let serverResponse: bluetoothManager.ServerResponse = {
          deviceId: clientDeviceId,
          transId: transId,
          status: 0,
          offset: offset,
          value: arrayBufferCCC
        }

        let ret = gattServerInstance!.sendResponse(serverResponse)
        message = resStr($r('app.string.bluetoothtest_str_cs_1')) + ret
        promptAction.showToast({message: message})
      })
      message = resStr($r('app.string.bluetoothtest_str_tzzdqdk'))
    }
    characteristicRead = !characteristicRead
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testOnCharacteristicReadManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnCharacteristicReadManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

//回调函数
let characteristicWrite = false

export function testOnCharacteristicWriteManager(): String {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot_4'))
    let btStatus = bluetoothManager.getState()
    if ( btStatus === bluetoothManager.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
      return message
    }
    if ( characteristicWrite ){
      gattServerInstance!.off("characteristicWrite", () => {
      });
      message = resStr($r('app.string.bluetoothtest_str_tzzxrgb'))
      promptAction.showToast({message: message})
      return message
    } else {
      gattServerInstance!.on('characteristicWrite', (characteristicWriteReq) => {
        let btOnCharacteristicWrite = 'on characteristicWrite'
        let characteristicWriteMsg = '';

        let arrayBufferCCC = string2ArrayBuffer("characteristicWriteForResponse");

        // client address
        let deviceId = characteristicWriteReq.deviceId;
        let transId = characteristicWriteReq.transId;
        let offset = characteristicWriteReq.offset;
        /** Whether this request should be pending for later operation */
        let isPrep = characteristicWriteReq.isPrep;
        /** Whether the remote client need a response */
        let needRsp = characteristicWriteReq.needRsp;
        /** Indicates the value to be written */
        let value = new Uint8Array(characteristicWriteReq.value);
        let characteristicUuid = characteristicWriteReq.characteristicUuid;
        let serviceUuid = characteristicWriteReq.serviceUuid;


        characteristicWriteMsg += "The characteristicWriteReq input parameters: deviceId =" + deviceId;
        characteristicWriteMsg += "transId =" + transId;
        characteristicWriteMsg += "offset =" + offset;
        characteristicWriteMsg += "isPrep =" + isPrep;
        characteristicWriteMsg += "needRsp =" + needRsp;
        characteristicWriteMsg += "value =" + JSON.stringify(value);
        characteristicWriteMsg += "characteristicUuid =" + characteristicUuid;
        characteristicWriteMsg += "serviceUuid =" + serviceUuid;
        AppStorage.setOrCreate("characteristicWriteMsg", characteristicWriteMsg)
        let serverResponse: bluetoothManager.ServerResponse = {
          deviceId: deviceId,
          transId: transId,
          status: 0,
          offset: offset,
          value: arrayBufferCCC
        };
        // sendResponse
        if ( needRsp ){
          let ret = gattServerInstance!.sendResponse(serverResponse);
          message = resStr($r('app.string.bluetoothtest_str_cs')) + ret;
          promptAction.showToast({message: message})
        }
        let notifyCharacteristic_notifyCharacteristic: bluetoothManager.NotifyCharacteristic = {
          serviceUuid: serviceUuid,
          characteristicUuid: characteristicUuid,
          characteristicValue: string2ArrayBuffer("Value4notifyCharacteristic"),
          confirm: true//notification:true; indication:false;
        }
        let notifyCharacteristic = notifyCharacteristic_notifyCharacteristic;
        gattServerInstance!.notifyCharacteristicChanged(deviceId, notifyCharacteristic)
        message = resStr($r('app.string.bluetoothtest_str_ns'));
      })
      message = resStr($r('app.string.bluetoothtest_str_tzzxrdk'));
    }
    characteristicWrite = !characteristicWrite
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testOnCharacteristicWriteManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnCharacteristicWriteManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

//回调函数
let descriptorRead = false

export function testOnDescriptorReadManager(): String {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot_9'))
    let btStatus = bluetoothManager.getState()
    if ( btStatus === bluetoothManager.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      return message;
    }
    if ( descriptorRead ){
      gattServerInstance!.off("descriptorRead", () => {
      });
      message = resStr($r('app.string.bluetoothtest_str_msfdqgb'));
      promptAction.showToast({message: message})
      return message
    } else {
      gattServerInstance!.on('descriptorRead', (descriptorReadReq) => {
        let btOnDescriptorRead = 'on:descriptorRead'
        let descriptorReadMsg = "descriptorReadIn!!";
        let deviceId = descriptorReadReq.deviceId;
        let transId = descriptorReadReq.transId;
        /** Indicates the byte offset of the start position for reading characteristic value */
        let offset = descriptorReadReq.offset;
        let descriptorUuid = descriptorReadReq.descriptorUuid;
        let characteristicUuid = descriptorReadReq.characteristicUuid;
        let serviceUuid = descriptorReadReq.serviceUuid;
        let arrayBufferDesc = string2ArrayBuffer("Response4descriptorRead");
        let serverResponse: bluetoothManager.ServerResponse = {
          deviceId: deviceId,
          transId: transId,
          status: 0,
          offset: offset,
          value: arrayBufferDesc
        }
        let ret = gattServerInstance!.sendResponse(serverResponse);
        message = resStr($r('app.string.bluetoothtest_str_ds_1')) + ret;
        descriptorReadMsg += message
        AppStorage.setOrCreate("descriptorReadMsg", descriptorReadMsg)
        promptAction.showToast({message: message})
      })
      message = resStr($r('app.string.bluetoothtest_str_msfdqdk'));
    }
    descriptorRead = !descriptorRead
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testOnDescriptorReadManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnDescriptorReadManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

//回调函数
let descriptorWrite = false

export function testOnDescriptorWriteManager(): String {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot_8'))
    let btStatus = bluetoothManager.getState()
    if ( btStatus === bluetoothManager.BluetoothState.STATE_OFF ){
      message = resStr($r('app.string.bluetoothtest_str_bine'));
      promptAction.showToast({message: message})
      return message
    }
    if ( descriptorWrite ){
      gattServerInstance!.off("descriptorWrite", () => {
      });
      message = resStr($r('app.string.bluetoothtest_str_msfxrgb'));
      promptAction.showToast({message: message})
      return message
    } else {
      gattServerInstance!.on('descriptorWrite', (descriptorWriteReq) => {
        let btOnDescriptorWrite = 'on:descriptorWrite'
        let descriptorWriteMsg = "descriptorWriteIn";
        descriptorWriteMsg += "The descriptorWriteReq client address: deviceId is " + descriptorWriteReq.deviceId;
        let deviceId = descriptorWriteReq.deviceId;
        let transId = descriptorWriteReq.transId;
        /** Indicates the byte offset of the start position for writing descriptor value */
        let offset = descriptorWriteReq.offset;
        /** Whether this request should be pending for later operation */
        let isPrep = descriptorWriteReq.isPrep;
        /** Whether the remote client need a response */
        let needRsp = descriptorWriteReq.needRsp;
        /** Indicates the value to be written */
        let value = new Uint8Array(descriptorWriteReq.value);
        let descriptorUuid = descriptorWriteReq.descriptorUuid;
        let characteristicUuid = descriptorWriteReq.characteristicUuid;
        let serviceUuid = descriptorWriteReq.serviceUuid;

        let arrayBufferDesc = new ArrayBuffer(8);
        let descValue = new Uint8Array(arrayBufferDesc);
        descValue[ 0 ] = value[ 0 ];
        let serverResponse: bluetoothManager.ServerResponse = {
          deviceId: deviceId,
          transId: transId,
          status: 0,
          offset: offset,
          value: arrayBufferDesc
        };

        if ( needRsp ){
          let ret = gattServerInstance!.sendResponse(serverResponse);
          message = resStr($r('app.string.bluetoothtest_str_ds')) + ret;
          promptAction.showToast({message: message})
        }
        descriptorWriteMsg += 'descriptorWrite Callback finished!';
      })
      message = resStr($r('app.string.bluetoothtest_str_msfxrdk'));
    }
    descriptorWrite = !descriptorWrite
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testOnDescriptorWriteManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnDescriptorWriteManager. message: ${ e.message }, code: ${ e.code }`
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