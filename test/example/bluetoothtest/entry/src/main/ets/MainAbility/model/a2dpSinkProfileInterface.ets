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

import bluetooth from '@ohos.bluetooth';
import bluetoothManager from '@ohos.bluetoothManager';
import btProfileModel from './btProfileModel'
import promptAction from '@ohos.promptAction';
import { fileIo as fs} from '@kit.CoreFileKit';
import { opp } from '@kit.ConnectivityKit';


/**
 * Profile A2DP Test Page Of Bluetooth test
 */

export function testProfileConnect() {
  try {
    let message = "ProfileConnect test"
    let textMessage = ""
    let btConnectionState: number = AppStorage.get("btConnectionState") !
    let deviceId: string = AppStorage.get("deviceId") !
    let profileType: number = AppStorage.get("profileType") !
    if ( btConnectionState == 1 || btConnectionState == 2 ) {
      promptAction.showToast({ message : btConnectionState == 1 ? "该设备正在连接中" : "该设备已连接" });
      return message;
    }
    let ret: boolean = (btProfileModel.getProfile(profileType) as bluetooth.A2dpSourceProfile | bluetooth.HandsFreeAudioGatewayProfile).connect(deviceId);
    if ( ret ) {
      btConnectionState = 1;
    }
    textMessage = "connect ret：" + ret;
    message = "connect ret：" + ret;
    AppStorage.SetOrCreate("textMessage" , textMessage)
    promptAction.showToast({ message : message });
    return message
  } catch( e ) {
    console.log(`Failed to testProfileConnect. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testProfileConnect. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testProfileDisConnect() {
  try {
    let message = "profileDisConnect test"
    let textMessage = ""
    let btConnectionState: number = AppStorage.get("btConnectionState") !
    let deviceId: string = AppStorage.get("deviceId") !
    let profileType: number = AppStorage.get("profileType") !
    if ( btConnectionState == 0 || btConnectionState == 3 ) {
      promptAction.showToast({ message : btConnectionState == 0 ? "该设备已断开" : "该设备正在断开连接" });
      return message;
    }
    let ret: boolean = (btProfileModel.getProfile(profileType) as bluetooth.A2dpSourceProfile | bluetooth.HandsFreeAudioGatewayProfile).disconnect(deviceId);
    if ( ret ) {
      btConnectionState = 3;
    }
    textMessage = "disconnect ret：" + ret;
    AppStorage.SetOrCreate("textMessage" , textMessage)
    message = "disconnect ret：" + ret;
    promptAction.showToast({ message : message });
    return message
  } catch( e ) {
    console.log(`Failed to testProfileDisConnect. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testProfileDisConnect. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testProfileGetPlayingState() {
  try {
    let message = "profileGetPlayingState test"
    let textMessage = ""
    let deviceId: string = AppStorage.get("deviceId") !
    let profileType: string = AppStorage.get("profileType") !
    promptAction.showToast({ message : profileType.toString() })
    console.log("profileType:" + profileType)
    let profile: bluetooth.A2dpSourceProfile
    profile = btProfileModel.getProfile(parseInt(profileType)) as bluetooth.A2dpSourceProfile
    let ret = - 1
    if ( profile ) {
      ret = profile.getPlayingState(deviceId);
    }
    textMessage = "getPlayingState ret: " + ret;
    message = "getPlayingState ret: " + ret;
    AppStorage.SetOrCreate("textMessage" , textMessage)
    promptAction.showToast({ message : message });
    return message
  } catch( e ) {
    console.log(`Failed to testProfileGetPlayingState. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testProfileGetPlayingState. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testProfileGetConnectionDevices() {
  try {
    let message = "profileGetConnectionDevices test"
    let textMessage = ""
    let profileType: number = AppStorage.get("profileType") !
    let ret: Array<string> =(btProfileModel.getProfile(profileType) as bluetooth.BaseProfile).getConnectionDevices();
    textMessage = "getConnectionDevices ret: " + JSON.stringify(ret);
    message = "getConnectionDevices ret: " + JSON.stringify(ret);
    AppStorage.SetOrCreate("textMessage" , textMessage)
    promptAction.showToast({ message : message });
    return message
  } catch( e ) {
    console.log(`Failed to testProfileGetConnectionDevices. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testProfileGetConnectionDevices. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testProfileGetDeviceState() {
  try {
    let message = "ProfileGetDeviceState test"
    let textMessage = ""
    let deviceId: string = AppStorage.get("deviceId") !
    let profileType: number = AppStorage.get("profileType") !
    let ret: number = (btProfileModel.getProfile(profileType) as bluetooth.BaseProfile).getDeviceState(deviceId);
    textMessage = "getDeviceState ret: " + ret;
    message = "getDeviceState ret: " + ret;
    AppStorage.SetOrCreate("textMessage" , textMessage)
    promptAction.showToast({ message : message });
    return message
  } catch( e ) {
    console.log(`Failed to testProfileGetDeviceState. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testProfileGetDeviceState. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

let transferStateChange = false

export function testOnTransferStateChange() {
  try {
    let message = "OnTransferStateChange test"
    let textMessage = ""
    let profileType: number = AppStorage.get("profileType") !
    if ( transferStateChange ) {
      (btProfileModel.getProfile(profileType) as opp.OppServerProfile).off("transferStateChange" , () => {
        promptAction.showToast({ message : "on.transferStateChange监听已关闭" })
      })
      message = ("关闭传输状态改变事件")
    } else {
      (btProfileModel.getProfile(profileType) as opp.OppServerProfile).on("transferStateChange" , (stateInformation) => {
        console.log("transferStateChange enter");
        textMessage = "transferStateChange success";
        message = "transferStateChange success";
        AppStorage.SetOrCreate("textMessage" , textMessage)
        let curProgress = (stateInformation.currentBytes * 100) / stateInformation.totalBytes;
        if ( curProgress === 100 ) {
          console.log("接收文件成功，文件存放路径：" + JSON.stringify(stateInformation.filePath));
          promptAction.showToast({ message : "接收文件成功，文件存放路径：" + JSON.stringify(stateInformation.filePath) })
        }
        promptAction.showToast({ message : message });
      })
    }
    transferStateChange = !transferStateChange
    return message
  } catch( e ) {
    console.log(`Failed to testOnTransferStateChange. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnTransferStateChange. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

let receiveIncomingFile = false

export function testOnReceiveIncomingFile() {
  try {
    let message = "OnReceiveIncomingFile test"
    let textMessage = ""
    let profileType: number = AppStorage.get("profileType") !
    if ( receiveIncomingFile ) {
      (btProfileModel.getProfile(profileType) as opp.OppServerProfile).off("receiveIncomingFile" , () => {
        promptAction.showToast({ message : "on.receiveIncomingFile监听已关闭" })
      })
      message = ("关闭接受文件状态改变事件")
    } else {
      (btProfileModel.getProfile(profileType) as opp.OppServerProfile).on("receiveIncomingFile" , (information) => {
        console.log("receiveIncomingFile enter");
        let deviceName: string = information.remoteDeviceName;
        let fileName: string = information.filePath;
        textMessage = "remoteDeviceName: " + deviceName + " filePath: " + fileName;
        message = "remoteDeviceName: " + deviceName + " filePath: " + fileName;
        AppStorage.SetOrCreate("textMessage" , textMessage)
        promptAction.showToast({ message : message });
      })
    }

    receiveIncomingFile = !receiveIncomingFile
    return message
  }
  catch( e ) {
    console.log(`Failed to testOnReceiveIncomingFile. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testOnReceiveIncomingFile. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testSendFile(serverMac: string , fileName: string , fileType: string) {
  try {
    let message = "sendFile test"
    let textMessage = ""
    let profileType: number | undefined = AppStorage.get("profileType")
    let fileHolders : Array<opp.FileHolder> = [];
    (btProfileModel.getProfile(profileType) as opp.OppServerProfile).sendFile(
      serverMac , fileHolders).then(() => {
      console.log("opp发送文件成功！ result: ");
      textMessage = "testSendFile ret: ";
      AppStorage.SetOrCreate("textMessage" , textMessage)
      message = "opp发送文件成功！ result: "
      promptAction.showToast({ message : message })
    })

    return message
  } catch( e ) {
    console.log(`Failed to testSendFile. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSendFile. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testSetIncomingFileConfirmation() {
  try {
    let message = "setIncomingFileConfirmation test"
    let textMessage = ""
    let profileType: number | undefined = AppStorage.get("profileType")
    let pathDir = "/test.jpg";
    let file = fs.openSync(pathDir, fs.OpenMode.CREATE | fs.OpenMode.READ_WRITE);
    (btProfileModel.getProfile(profileType) as opp.OppServerProfile).setIncomingFileConfirmation(true, file.fd).
      then(() => {
        let result = true;
        console.log("result: " + result);
        textMessage = "result: " + result;
        AppStorage.SetOrCreate("textMessage" , textMessage)
        message = "result: " + result
        promptAction.showToast({ message : message })
      })

    return message
  } catch( e ) {
    console.log(`Failed to testSetIncomingFileConfirmation. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testSetIncomingFileConfirmation. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGetCurrentTransferInformation() {
  try {
    let message = "getCurrentTransferInformation test"
    let textMessage = ""
    let profileType: number | undefined = AppStorage.get("profileType");
    (btProfileModel.getProfile(profileType) as opp.OppServerProfile).
      getCurrentTransferInformation().then((value: opp.OppTransferInformation) => {
        let result = true;
        console.log("当前信息: " + JSON.stringify(result));
        textMessage = "当前信息: " + JSON.stringify(result);
        AppStorage.SetOrCreate("textMessage" , textMessage)
        message = "当前信息: " + JSON.stringify(result)
        promptAction.showToast({ message : message })
      })

    return message
  } catch( e ) {
    console.log(`Failed to testGetCurrentTransferInformation. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGetCurrentTransferInformation. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testCancelTransfer() {
  try {
    let message = "cancelTransfer test"
    let textMessage = ""
    let profileType: number | undefined = AppStorage.get("profileType");
    (btProfileModel.getProfile(profileType) as opp.OppServerProfile).cancelTransfer().then(() => {
      let result = true;
      console.log("取消传输: " + JSON.stringify(result));
      textMessage = "取消传输: " + JSON.stringify(result);
      AppStorage.SetOrCreate("textMessage" , textMessage)
      message = "取消传输: " + JSON.stringify(result)
      promptAction.showToast({ message : message })
    })

    return message
  } catch( e ) {
    console.log(`Failed to testCancelTransfer. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testCancelTransfer. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}