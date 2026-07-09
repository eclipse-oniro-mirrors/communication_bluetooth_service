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
import promptAction from '@ohos.promptAction';

let clientNumber = - 1

export function testSppClientConnectSafeManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_8'))
    LogUtil.info('testSppClientConnectSafe create! ');
    let uuid: string = AppStorage.get('inputUuid') !
    let deviceId: string = AppStorage.get('inputDeviceId') !
    LogUtil.info('bluetoothManager sppConnect: ' + deviceId);
    bluetoothManager.sppConnect(deviceId, {
      uuid: uuid,
      secure: true,
      type: 0
    }, (code, socketID) => {
      LogUtil.info("spp client code:" + JSON.stringify(code))
      if (code.code != 0) {
        message = resStr($r('app.string.bluetoothtest_str_khdljsb'))
        LogUtil.info('bluetoothManager sppConnect callback socketNumber(safe mode): ' +
          JSON.stringify(code));
        promptAction.showToast({message: message})
        return message;
      }
      acceptClientSocketManager(socketID);
      LogUtil.info('bluetoothManager sppConnect callback socketNumber(safe mode): ' + socketID);
      message = resStr($r('app.string.bluetoothtest_str_khdljcg'));
      AppStorage.setOrCreate("clientNumber", clientNumber)
      promptAction.showToast({message: message})
      return message
    })
    promptAction.showToast({message: message})
    return message
  } catch(e) {
    LogUtil.info(`Failed to testSppClientConnectSafeManager. message: ${e.message },
      code: ${e.code }`)
    let message = `Failed to testSppClientConnectSafeManager. message: ${e.message },
      code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSppClientConnectUnSafeManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_4'))
    LogUtil.info('testSppClientConnectUnSafe create! ')
    let uuid: string = AppStorage.get('inputUuid') !
    let deviceId: string = AppStorage.get('inputDeviceId') !
    LogUtil.info('bluetoothManager sppConnect: ' + deviceId);
    bluetoothManager.sppConnect(deviceId, {
      uuid: uuid,
      secure: false,
      type: 0
    }, (code, socketID) => {
      if (code) {
        message = resStr($r('app.string.bluetoothtest_str_khdljsb'));
        LogUtil.info('bluetoothManager sppConnect callback socketNumber(non-safe mode): ' +
          socketID);
        promptAction.showToast({message: message})
        return message;
      }
      acceptClientSocketManager(socketID);
      LogUtil.info('bluetoothManager sppConnect callback socketNumber(non-safe mode): ' + socketID);
      message = resStr($r('app.string.bluetoothtest_str_khdljcg'));
      promptAction.showToast({message: message})
      return message
    });
    promptAction.showToast({message: message})
    return message
  } catch(e) {
    LogUtil.info(`Failed to testSppClientConnectUnSafeManager. message: ${e.message },
      code: ${e.code }`)
    let message = `Failed to testSppClientConnectUnSafeManager. message: ${e.message },
      code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSppClientWriteManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_18'))
    LogUtil.info('bluetoothManager sppWrite clientNumber1: ' + JSON.stringify(clientNumber))
    let writeClientData: string = AppStorage.get("writeClientData") !
    let writeArrayBuffer: ArrayBuffer = strToArrayBuffer(writeClientData) as ArrayBuffer;
    bluetoothManager.sppWrite(clientNumber, writeArrayBuffer);
    AppStorage.setOrCreate("clientNumber", clientNumber)
    LogUtil.info('sppWrite successfully');
    message = resStr($r('app.string.bluetoothtest_str_xrcg'));
    promptAction.showToast({message: message})
    return message
  } catch(e) {
    LogUtil.info(`Failed to testSppClientWriteManager. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to testSppClientWriteManager. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSppClientReadOnManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_14'))
    bluetoothManager.on('sppRead', clientNumber, (dataBuffer: ArrayBuffer) => {
      const data = new Uint8Array(dataBuffer)
      LogUtil.info('spp bluetoothManager read is: ' + data);
      if (data != null) {
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
  } catch(e) {
    LogUtil.info(`Failed to testSppClientReadOnManager. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to testSppClientReadOnManager. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSppClientReadOffManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_11'))
    bluetoothManager.off('sppRead', clientNumber, (dataBuffer: ArrayBuffer) => {
      const data = new Uint8Array(dataBuffer)
      LogUtil.info('spp bluetoothManager read is: ' + data);
      if (data != null) {
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
  } catch(e) {
    LogUtil.info(`Failed to testSppClientReadOffManager. message: ${e.message },
      code: ${e.code }`)
    let message = `Failed to testSppClientReadOffManager. message: ${e.message },
      code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSppClientCloseManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_17'))
    sppClientCloseManager()
    promptAction.showToast({message: message})
    return message
  } catch(e) {
    LogUtil.info(`Failed to testSppClientCloseManager. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to testSppClientCloseManager. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function acceptClientSocketManager(number: number) {
  LogUtil.info('spp clientSocket socketID: ' + number)
  // 获取的clientNumber用作服务端后续读/写操作socket的id。
  clientNumber = number;
  AppStorage.setOrCreate("clientNumber", clientNumber)
}

export function sppClientCloseManager() {
  try {
    LogUtil.info('bluetoothModel sppCloseClientSocket clientNumber: ' +
      JSON.stringify(clientNumber))
    bluetoothManager.sppCloseClientSocket(clientNumber)
    let message = resStr($r('app.string.bluetoothtest_str_gbcgc')) + clientNumber;
    promptAction.showToast({message: message})
    return message
  } catch(e) {
    LogUtil.info(`Failed to sppClientCloseManager. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to sppClientCloseManager. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function strToArrayBuffer(str: string) {
  let array = new Uint8Array(str.length)
  for(let i = 0 ; i<str.length ; i++) {
    array[i] = str.charCodeAt(i);
  }
  return array.buffer
}

export function dataRead(dataBuffer: ArrayBuffer) {
  let dataReadMessage = ''
  const data = new Uint8Array(dataBuffer);
  LogUtil.info('spp bluetoothManager read is: ' + data);
  if (data != null) {
    dataReadMessage = resStr($r('app.string.bluetoothtest_str_dqcg_2')) + JSON.stringify(data);
  } else {
    dataReadMessage = resStr($r('app.string.bluetoothtest_str_dqsb'));
  }
}