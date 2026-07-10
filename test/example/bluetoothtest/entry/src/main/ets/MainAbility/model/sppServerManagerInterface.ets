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

let serverNumber: number = - 1;

export function testSppServerCreateSafeManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_10'))
    LogUtil.info('spp server socketId(safe mode) create! ');
    let uuid: string = AppStorage.get('inputUuid') !
    if (uuid == '') {
      message = resStr($r('app.string.bluetoothtest_str_uwk'));
      promptAction.showToast({message: message})
      return message
    }
    // 创建蓝牙服务器侦听套接字, 返回服务端ID
    bluetoothManager.sppListen('BluetoothDevice', {
      uuid: uuid,
      secure: true,
      type: 0
    }, (code, serverSocketID) => {
      LogUtil.info('bluetooth error code: ' + typeof code + " ; " + JSON.stringify(code));
      if (!code || code.code == 0) {
        LogUtil.info('spp server: serverSocketID(safe mode) - ' + serverSocketID);
        serverNumber = serverSocketID;
        message = resStr($r('app.string.bluetoothtest_str_cjscg'));
        promptAction.showToast({message: message})
      }
    });
    promptAction.showToast({message: message})
    return message
  } catch(e) {
    LogUtil.info(`Failed to testSppServerCreateSafeManager. message: ${e.message },
      code: ${e.code }`)
    let message = `Failed to testSppServerCreateSafeManager. message: ${e.message },
      code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}


export function testSppServerCreateUnSafeManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_7'))
    let uuid: string = AppStorage.get('inputUuid') !
    LogUtil.info('spp server socketId(non-safe mode) create! ')
    if (uuid == '') {
      message = resStr($r('app.string.bluetoothtest_str_uwk'));
      promptAction.showToast({message: message})
      return message
    }
    // 创建蓝牙服务器侦听套接字, 返回服务端ID
    bluetoothManager.sppListen('BluetoothDevice', {
      uuid: uuid,
      secure: false,
      type: 0
    }, (code, serverSocketID) => {
      LogUtil.info('bluetooth error code: ' + typeof code + " ; " + JSON.stringify(code));
      if (!code || code.code == 0) {
        LogUtil.info('spp server: serverSocketID(non-safe mode) ' + serverSocketID);
        serverNumber = serverSocketID;
        message = resStr($r('app.string.bluetoothtest_str_cjscg'));
        promptAction.showToast({message: message})
      }
    });
    promptAction.showToast({message: message})
    return message
  } catch(e) {
    LogUtil.info(`Failed to testSppServerCreateUnSafeManager. message: ${e.message },
      code: ${e.code }`)
    let message = `Failed to testSppServerCreateUnSafeManager. message: ${e.message },
      code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

let clientNumber: number = - 1;

export function testSppServerAcceptManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_tt'))
    let serverNumber: number = AppStorage.get("serverNumber") !
    LogUtil.info('testSppServerAccept test')
    LogUtil.info('testSppServerAccept serverNumber ' + serverNumber)
    bluetoothManager.sppAccept(serverNumber, (code, clientSocketID) => {
      LogUtil.info('bluetooth error code: ' + typeof code + " ; " + JSON.stringify(code));
      if (!code || code.code == 0) {
        LogUtil.info('spp server: clientSocketID - ' + clientSocketID);
        LogUtil.info('test serverNumber ' + serverNumber);
        clientNumber = clientSocketID;
        //      AppStorage.setOrCreate("clientNumber",clientNumber)
        message = resStr($r('app.string.bluetoothtest_str_jskhdcg'));
        bluetoothManager.on('sppRead', clientNumber,
          (dataBuffer: ArrayBuffer) => {
            const data = new Uint8Array(dataBuffer);
            LogUtil.info('spp bluetoothManager read is: ' + data);
            if (data != null) {
              message = resStr($r('app.string.bluetoothtest_str_dqcg_2')) + JSON.stringify(data);
            } else {
              message = resStr($r('app.string.bluetoothtest_str_dqsb'));
            }
            AppStorage.setOrCreate("dataReadMessage", message)
          })
      }
      promptAction.showToast({message: message})
      return message
    })
    promptAction.showToast({message: message})
    return message
  } catch(e) {
    LogUtil.info(`Failed to testSppServerAcceptManager. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to testSppServerAcceptManager. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSppServerWriteManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_20'))
    LogUtil.info(MODEL + 'sppWrite clientNumber: ' + JSON.stringify(clientNumber))
    let writeServerData: string = AppStorage.get("writeServerData") !
    let writeArrayBuffer: ArrayBuffer = strToArrayBuffer(writeServerData) as ArrayBuffer;
    bluetoothManager.sppWrite(clientNumber, writeArrayBuffer);
    LogUtil.info('sppWrite successfully');
    message = resStr($r('app.string.bluetoothtest_str_xrcg'));
    promptAction.showToast({message: message})
    return message
  } catch(e) {
    LogUtil.info(`Failed to testSppServerWriteManager. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to testSppServerWriteManager. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}


export function testSppServerReadOnManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_15'))
    bluetoothManager.on('sppRead', clientNumber,
      (dataBuffer: ArrayBuffer) => {
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
      }
    );
    promptAction.showToast({message: message})
    return message
  } catch(e) {
    LogUtil.info(`Failed to testSppServerReadOnManager. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to testSppServerReadOnManager. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSppServerReadOffManager(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_12'))
    bluetoothManager.off('sppRead', clientNumber,
      (dataBuffer: ArrayBuffer) => {
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
      }
    );
    promptAction.showToast({message: message})
    return message
  } catch(e) {
    LogUtil.info(`Failed to testSppServerReadOffManager. message: ${e.message },
      code: ${e.code }`)
    let message = `Failed to testSppServerReadOffManager. message: ${e.message },
      code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

//关闭服务端serverSocket
let MODEL = "bluetoothModel:"

export function testSppCloseServerSocketManager(): String {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_19'))
    LogUtil.info(MODEL + 'sppCloseServerSocket serverNumber: ' + serverNumber)
    bluetoothManager.sppCloseServerSocket(serverNumber)
    message = resStr($r('app.string.bluetoothtest_str_gbfwd'));
    promptAction.showToast({message: message})
    return message
  } catch(e) {
    LogUtil.info(`Failed to testSppCloseServerSocketManager. message: ${e.message },
      code: ${e.code }`)
    let message = `Failed to testSppCloseServerSocketManager. message: ${e.message },
      code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

//关闭服务端accept回调的clientSocket
export function testSppCloseClientSocketManager(): String {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_9'))
    LogUtil.info(MODEL + 'sppCloseClientSocket clientNumber: ' + clientNumber)
    bluetoothManager.sppCloseClientSocket(clientNumber)
    message = resStr($r('app.string.bluetoothtest_str_gbkhd'));
    promptAction.showToast({message: message})
    return message
  } catch(e) {
    LogUtil.info(`Failed to testSppCloseClientSocketManager. message: ${e.message },
      code: ${e.code }`)
    let message = `Failed to testSppCloseClientSocketManager. message: ${e.message },
      code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

function strToArrayBuffer(str: string) {
  let array = new Uint8Array(str.length)
  for(let i = 0 ; i<str.length ; i++) {
    array[i] = str.charCodeAt(i);
  }
  return array.buffer
}