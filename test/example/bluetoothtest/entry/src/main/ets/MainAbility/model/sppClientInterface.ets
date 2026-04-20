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


import bluetooth from '@ohos.bluetooth';
import promptAction from '@ohos.promptAction';

let clientNumber = - 1

export function testSppClientConnectSafe(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_8'))
    LogUtil.info('testSppClientConnectSafe create! ');
    let uuid: string = AppStorage.get('inputUuid') !
    let deviceId: string = AppStorage.get('inputDeviceId') !
    LogUtil.info('bluetoothModel sppConnect: ' + deviceId);
    bluetooth.sppConnect(deviceId, {
      uuid: uuid,
      secure: true,
      type: 0
    }, (code, socketID) => {
      LogUtil.info("spp client code:" + JSON.stringify(code))
      if ( code.code != 0 ){
        message = resStr($r('app.string.bluetoothtest_str_khdljsb'))
        LogUtil.info('bluetoothModel sppConnect callback socketNumber(safe mode): ' + JSON.stringify(code));
        promptAction.showToast({message: message})
        return message;
      }
      acceptClientSocket(socketID);
      LogUtil.info('bluetoothModel sppConnect callback socketNumber(safe mode): ' + socketID);
      message = resStr($r('app.string.bluetoothtest_str_khdljcg'));
      AppStorage.setOrCreate("clientNumber", clientNumber)
      promptAction.showToast({message: message})
      return message
    })
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testSppClientConnectSafe. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSppClientConnectSafe. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSppClientConnectUnSafe(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_4'))
    LogUtil.info('testSppClientConnectUnSafe create! ')
    let uuid: string = AppStorage.get('inputUuid') !
    let deviceId: string = AppStorage.get('inputDeviceId') !
    LogUtil.info('bluetoothModel sppConnect: ' + deviceId);
    bluetooth.sppConnect(deviceId, {
      uuid: uuid,
      secure: false,
      type: 0
    }, (code, socketID) => {
      LogUtil.info("spp client unsafe code:" + JSON.stringify(code))
      if ( code.code != 0 ){
        message = resStr($r('app.string.bluetoothtest_str_khdljsb'))
        LogUtil.info('bluetoothModel sppConnect callback socketNumber(non-safe mode): ' + socketID);
        promptAction.showToast({message: message})
        return message;
      }
      acceptClientSocket(socketID);
      LogUtil.info('bluetoothModel sppConnect callback socketNumber(non-safe mode): ' + socketID);
      message = resStr($r('app.string.bluetoothtest_str_khdljcg'));
      promptAction.showToast({message: message})
      return message
    });
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testSppClientConnectUnSafe. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSppClientConnectUnSafe. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSppClientWrite(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_18'))
    LogUtil.info('bluetoothModel sppWrite clientNumber1: ' + JSON.stringify(clientNumber))
    let writeClientData: string = AppStorage.get("writeClientData") !
    let writeArrayBuffer: ArrayBuffer = strToArrayBuffer(writeClientData) as ArrayBuffer;
    const sppWriteSuccess = bluetooth.sppWrite(clientNumber, writeArrayBuffer);
    AppStorage.setOrCreate("clientNumber", clientNumber)
    if ( sppWriteSuccess ){
      LogUtil.info('sppWrite successfully');
      message = resStr($r('app.string.bluetoothtest_str_xrcg'));
    } else {
      LogUtil.info('sppWrite failed');
      message = resStr($r('app.string.bluetoothtest_str_xrsb'));
    }
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testSppClientWrite. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSppClientWrite. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSppClientReadOn(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_14'))
    bluetooth.on('sppRead', clientNumber, (dataBuffer: ArrayBuffer) => {
      const data = new Uint8Array(dataBuffer)
      LogUtil.info('spp bluetooth read is: ' + data);
      if ( data != null ){
        message = resStr($r('app.string.bluetoothtest_str_dqcg_2')) + JSON.stringify(data);
      } else {
        message = resStr($r('app.string.bluetoothtest_str_dqsb'));
      }
      AppStorage.setOrCreate("dataReadMessage", message)
      promptAction.showToast({message: message})
      return message
    });
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testSppClientReadOn. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSppClientReadOn. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSppClientReadOff(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_11'))
    bluetooth.off('sppRead', clientNumber, (dataBuffer: ArrayBuffer) => {
      const data = new Uint8Array(dataBuffer)
      LogUtil.info('spp bluetooth read is: ' + data);
      if ( data != null ){
        message = resStr($r('app.string.bluetoothtest_str_dqcg_2')) + JSON.stringify(data);
      } else {
        message = resStr($r('app.string.bluetoothtest_str_dqsb'));
      }
      AppStorage.setOrCreate("dataReadMessage", message)
      promptAction.showToast({message: message})
      return message
    });
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testSppClientReadOff. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSppClientReadOff. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSppClientClose(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_17'))
    sppClientClose()
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to testSppClientClose. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSppClientClose. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function acceptClientSocket(number: number){
  LogUtil.info('spp clientSocket socketID: ' + number)
  // 获取的clientNumber用作服务端后续读/写操作socket的id。
  clientNumber = number;
  AppStorage.setOrCreate("clientNumber", clientNumber)
}

export function sppClientClose(){
  try {
    LogUtil.info('bluetoothModel sppCloseClientSocket clientNumber: ' + JSON.stringify(clientNumber))
    bluetooth.sppCloseClientSocket(clientNumber)
    let message = resStr($r('app.string.bluetoothtest_str_gbcgc')) + clientNumber;
    promptAction.showToast({message: message})
    return message
  } catch( e ){
    LogUtil.info(`Failed to sppClientClose. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to sppClientClose. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function strToArrayBuffer(str: string){
  let array = new Uint8Array(str.length)
  for(let i = 0 ; i<str.length ; i++ ){
    array[ i ] = str.charCodeAt(i);
  }
  return array.buffer
}

export function dataRead(dataBuffer: ArrayBuffer){
  let dataReadMessage: string = ''
  const data = new Uint8Array(dataBuffer);
  LogUtil.info('spp bluetooth read is: ' + data);
  if ( data != null ){
    dataReadMessage = resStr($r('app.string.bluetoothtest_str_dqcg_2')) + JSON.stringify(data);
  } else {
    dataReadMessage = resStr($r('app.string.bluetoothtest_str_dqsb'));
  }
}