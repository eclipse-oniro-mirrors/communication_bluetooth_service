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
import bluetooth from '@ohos.bluetooth';
import promptAction from '@ohos.promptAction';
import { BusinessError } from '@ohos.base';

let serverNumber: number = - 1;

export function testSppServerCreateSafe(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_10'))
    hilog.info(0x0000, 'BtTest', '%{public}s', 'spp server socketId(safe mode) create! ');
    let uuid: string = AppStorage.get('inputUuid') !
    if ( uuid == '' ) {
      message = resStr($r('app.string.bluetoothtest_str_uwk'));
      promptAction.showToast({ message : message })
      return message
    }
    // 创建蓝牙服务器侦听套接字, 返回服务端ID
    bluetooth.sppListen('BluetoothDevice' , {
      uuid : uuid ,
      secure : true ,
      type : 0
    } , (code: BusinessError , serverSocketID) => {
      hilog.info(0x0000, 'BtTest', '%{public}s', 'bluetooth error code: ' + typeof code + " ; " + JSON.stringify(code));
      if ( !code || code.code == 0 ) {
        hilog.info(0x0000, 'BtTest', '%{public}s', 'spp server: serverSocketID(safe mode) - ' + serverSocketID);
        serverNumber = serverSocketID;
        message = resStr($r('app.string.bluetoothtest_str_cjscg'));
        AppStorage.setOrCreate("serverNumber" , serverNumber)
        hilog.info(0x0000, 'BtTest', '%{public}s', 'spp server: serverNumber： ' + serverNumber);
        promptAction.showToast({ message : message })
      }
    });
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testSppServerCreateSafe. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSppServerCreateSafe. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testSppServerCreateUnSafe(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_7'))
    let uuid: string = AppStorage.get('inputUuid') !
    hilog.info(0x0000, 'BtTest', '%{public}s', 'spp server socketId(non-safe mode) create! ');
    if ( uuid == '' ) {
      message = resStr($r('app.string.bluetoothtest_str_uwk'));
      promptAction.showToast({ message : message })
    }
    // 创建蓝牙服务器侦听套接字, 返回服务端ID
    bluetooth.sppListen('BluetoothDevice' , {
      uuid : uuid ,
      secure : false ,
      type : 0
    } , (code , serverSocketID) => {
      hilog.info(0x0000, 'BtTest', '%{public}s', 'bluetooth error code: ' + typeof code + " ; " + JSON.stringify(code));
      if ( !code || code.code == 0 ) {
        hilog.info(0x0000, 'BtTest', '%{public}s', 'spp server: serverSocketID(non-safe mode) ' + serverSocketID);
        serverNumber = serverSocketID;
        message = resStr($r('app.string.bluetoothtest_str_cjscg'));
        promptAction.showToast({ message : message })
      }
    });
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testSppServerCreateUnSafe. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSppServerCreateUnSafe. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

let clientNumber: number = - 1;

export function testSppServerAccept(): string {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_tt'))
    let serverNumber: number = AppStorage.get("serverNumber") !
    hilog.info(0x0000, 'BtTest', '%{public}s', 'testSppServerAccept test')
    hilog.info(0x0000, 'BtTest', '%{public}s', 'testSppServerAccept serverNumber ' + serverNumber)
    bluetooth.sppAccept(serverNumber , (code , clientSocketID) => {
      if ( !code || code.code == 0 ) {
        hilog.info(0x0000, 'BtTest', '%{public}s', 'spp server: clientSocketID - ' + clientSocketID);
        hilog.info(0x0000, 'BtTest', '%{public}s', 'test serverNumber ' + serverNumber);
        clientNumber = clientSocketID;
        //      AppStorage.setOrCreate("clientNumber",clientNumber)
        message = resStr($r('app.string.bluetoothtest_str_jskhdcg'));
        bluetooth.on('sppRead' , clientNumber ,
          (dataBuffer: ArrayBuffer) => {
            const data = new Uint8Array(dataBuffer);
            hilog.info(0x0000, 'BtTest', '%{public}s', 'spp bluetooth read is: ' + data);
            if ( data != null ) {
              message = resStr($r('app.string.bluetoothtest_str_dqcg_2')) + JSON.stringify(data);
            } else {
              message = resStr($r('app.string.bluetoothtest_str_dqsb'));
            }
            AppStorage.setOrCreate("dataReadMessage" , message)
          })
      }
      promptAction.showToast({ message : message })
      return message
    })
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testSppServerAccept. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSppServerAccept. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testSppServerWrite(): String {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_20'))
    hilog.info(0x0000, 'BtTest', '%{public}s', MODEL + 'sppWrite clientNumber: ' + JSON.stringify(clientNumber));
    let writeServerData: string = AppStorage.get("writeServerData") !
    let writeArrayBuffer: ArrayBuffer = strToArrayBuffer(writeServerData) as ArrayBuffer;
    const sppWriteSuccess = bluetooth.sppWrite(clientNumber , writeArrayBuffer);
    if ( sppWriteSuccess ) {
      hilog.info(0x0000, 'BtTest', '%{public}s', 'sppWrite successfully');
      message = resStr($r('app.string.bluetoothtest_str_xrcg'));
    } else {
      hilog.info(0x0000, 'BtTest', '%{public}s', 'sppWrite failed');
      message = resStr($r('app.string.bluetoothtest_str_xrsb'));
    }
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testSppServerWrite. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSppServerWrite. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}


export function testSppServerReadOn(): String {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_15'))
    bluetooth.on('sppRead' , clientNumber ,
      (dataBuffer: ArrayBuffer) => {
        const data = new Uint8Array(dataBuffer);
        hilog.info(0x0000, 'BtTest', '%{public}s', 'spp bluetooth read is: ' + data);
        if ( data != null ) {
          message = resStr($r('app.string.bluetoothtest_str_dqcg_2')) + JSON.stringify(data);
        } else {
          message = resStr($r('app.string.bluetoothtest_str_dqsb'));
        }
        AppStorage.setOrCreate("dataReadMessage" , message)
        promptAction.showToast({ message : message })
        return message
      }
    );
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testSppServerReadOn. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSppServerReadOn. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testSppServerReadOff(): String {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_12'))
    bluetooth.off('sppRead' , clientNumber ,
      (dataBuffer: ArrayBuffer) => {
        const data = new Uint8Array(dataBuffer);
        hilog.info(0x0000, 'BtTest', '%{public}s', 'spp bluetooth read is: ' + data);
        if ( data != null ) {
          message = resStr($r('app.string.bluetoothtest_str_dqcg_2')) + JSON.stringify(data);
        } else {
          message = resStr($r('app.string.bluetoothtest_str_dqsb'));
        }
        AppStorage.setOrCreate("dataReadMessage" , message)
        promptAction.showToast({ message : message })
        return message
      }
    );
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testSppServerReadOff. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSppServerReadOff. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

//关闭服务端serverSocket
let MODEL = "bluetoothModel:"

export function testSppCloseServerSocket(): String {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_19'))
    hilog.info(0x0000, 'BtTest', '%{public}s', MODEL + 'sppCloseServerSocket serverNumber: ' + serverNumber)
    bluetooth.sppCloseServerSocket(serverNumber);
    message = resStr($r('app.string.bluetoothtest_str_gbfwd'));
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testSppCloseServerSocket. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSppCloseServerSocket. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

//关闭服务端accept回调的clientSocket
export function testSppCloseClientSocket(): String {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_9'))
    hilog.info(0x0000, 'BtTest', '%{public}s', MODEL + 'sppCloseClientSocket clientNumber: ' + clientNumber)
    bluetooth.sppCloseClientSocket(clientNumber);
    message = resStr($r('app.string.bluetoothtest_str_gbkhd'));
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testSppCloseClientSocket. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSppCloseClientSocket. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function strToArrayBuffer(str: string) {
  let array = new Uint8Array(str.length);
  for ( let i = 0 ; i < str.length ; i ++ ) {
    array[ i ] = str.charCodeAt(i);
  }
  return array.buffer
}