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
import bluetoothManager from '@ohos.bluetoothManager';
import btProfileModel from './btProfileModel'
import promptAction from '@ohos.promptAction';
import { fileIo as fs} from '@kit.CoreFileKit';
import { opp} from '@kit.ConnectivityKit';
import { BusinessError } from '@ohos.base';


/**
 * Profile A2DP Test Page Of Bluetooth test
 */

export function testProfileConnect() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_pt_4'))
    let textMessage = ""
    let btConnectionState: number = AppStorage.get("btConnectionState") !
    let deviceId: string = AppStorage.get("deviceId") !
    let profileType: number = AppStorage.get("profileType") !
    if (btConnectionState == 1 || btConnectionState == 2) {
      promptAction.showToast({message: btConnectionState == 1 ?
        $r('app.string.bluetoothtest_str_gsbzzljz') : $r('app.string.bluetoothtest_str_gsbylj')});
      return message;
    }
    let ret: boolean = (btProfileModel.getProfile(profileType) as bluetooth.A2dpSourceProfile |
      bluetooth.HandsFreeAudioGatewayProfile).connect(deviceId);
    if (ret) {
      btConnectionState = 1;
    }
    textMessage = resStr($r('app.string.bluetoothtest_str_cr')) + ret;
    message = resStr($r('app.string.bluetoothtest_str_cr')) + ret;
    AppStorage.SetOrCreate("textMessage", textMessage)
    promptAction.showToast({message: message});
    return message
  } catch(e) {
    LogUtil.info(`Failed to testProfileConnect. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to testProfileConnect. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testProfileDisConnect() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_pt_3'))
    let textMessage = ""
    let btConnectionState: number = AppStorage.get("btConnectionState") !
    let deviceId: string = AppStorage.get("deviceId") !
    let profileType: number = AppStorage.get("profileType") !
    if (btConnectionState == 0 || btConnectionState == 3) {
      promptAction.showToast({message: btConnectionState == 0 ?
        $r('app.string.bluetoothtest_str_gsbydk') : $r('app.string.bluetoothtest_str_gsbzzdklj')})
      return message;
    }
    let ret: boolean = (btProfileModel.getProfile(profileType) as bluetooth.A2dpSourceProfile |
      bluetooth.HandsFreeAudioGatewayProfile).disconnect(deviceId);
    if (ret) {
      btConnectionState = 3;
    }
    textMessage = resStr($r('app.string.bluetoothtest_str_dr')) + ret;
    AppStorage.SetOrCreate("textMessage", textMessage)
    message = resStr($r('app.string.bluetoothtest_str_dr')) + ret;
    promptAction.showToast({message: message});
    return message
  } catch(e) {
    LogUtil.info(`Failed to testProfileDisConnect. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to testProfileDisConnect. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testProfileGetPlayingState() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_pt_1'))
    let textMessage = ""
    let deviceId: string = AppStorage.get("deviceId") !
    let profileType: string = AppStorage.get("profileType") !
    promptAction.showToast({message: profileType.toString()})
    LogUtil.info("profileType:" + profileType)
    let profile: bluetooth.A2dpSourceProfile
    profile = btProfileModel.getProfile(parseInt(profileType)) as bluetooth.A2dpSourceProfile
    let ret = - 1
    if (profile) {
      ret = profile.getPlayingState(deviceId)
    }
    textMessage = resStr($r('app.string.bluetoothtest_str_gr_1')) + ret;
    message = resStr($r('app.string.bluetoothtest_str_gr_1')) + ret;
    AppStorage.SetOrCreate("textMessage", textMessage)
    promptAction.showToast({message: message});
    return message
  } catch(e) {
    LogUtil.info(`Failed to testProfileGetPlayingState. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to testProfileGetPlayingState. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testProfileGetConnectionDevices() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_pt'))
    let textMessage = ""
    let profileType: number = AppStorage.get("profileType") !
    let ret: Array<string> =(btProfileModel.getProfile(profileType) as
      bluetooth.BaseProfile).getConnectionDevices()
    textMessage = resStr($r('app.string.bluetoothtest_str_gr')) + JSON.stringify(ret);
    message = resStr($r('app.string.bluetoothtest_str_gr')) + JSON.stringify(ret);
    AppStorage.SetOrCreate("textMessage", textMessage)
    promptAction.showToast({message: message});
    return message
  } catch(e) {
    LogUtil.info(`Failed to testProfileGetConnectionDevices. message: ${e.message },
      code: ${e.code }`)
    let message = `Failed to testProfileGetConnectionDevices. message: ${e.message },
      code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testProfileGetDeviceState() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_pt_2'))
    let textMessage = ""
    let deviceId: string = AppStorage.get("deviceId") !
    let profileType: number = AppStorage.get("profileType") !
    let ret: number = (btProfileModel.getProfile(profileType) as
      bluetooth.BaseProfile).getDeviceState(deviceId)
    textMessage = resStr($r('app.string.bluetoothtest_str_gr_2')) + ret;
    message = resStr($r('app.string.bluetoothtest_str_gr_2')) + ret;
    AppStorage.SetOrCreate("textMessage", textMessage)
    promptAction.showToast({message: message});
    return message
  } catch(e) {
    LogUtil.info(`Failed to testProfileGetDeviceState. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to testProfileGetDeviceState. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

let transferStateChange = false

export function testOnTransferStateChange() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot_3'))
    let textMessage = ""
    let profileType: number = AppStorage.get("profileType") !
    if (transferStateChange) {
      (btProfileModel.getProfile(profileType) as opp.OppServerProfile).off("transferStateChange",
        () => {
        promptAction.showToast({message: resStr($r('app.string.bluetoothtest_str_otjtygb'))})
      })
      message = resStr($r('app.string.bluetoothtest_str_gbcsztgbsj'))
    } else {
      (btProfileModel.getProfile(profileType) as opp.OppServerProfile).on("transferStateChange",
        (stateInformation) => {
        LogUtil.info("transferStateChange enter")
        textMessage = resStr($r('app.string.bluetoothtest_str_ts'));
        message = resStr($r('app.string.bluetoothtest_str_ts'));
        AppStorage.SetOrCreate("textMessage", textMessage)
        let curProgress = (stateInformation.currentBytes * 100) / stateInformation.totalBytes;
        if (curProgress === 100) {
          LogUtil.info(resStr($r('app.string.bluetoothtest_str_jswjcgwjcflj')) +
            JSON.stringify(stateInformation.filePath));
          promptAction.showToast({message: resStr($r('app.string.bluetoothtest_str_jswjcgwjcflj')) +
            JSON.stringify(stateInformation.filePath)})
        }
        promptAction.showToast({message: message});
      })
    }
    transferStateChange = !transferStateChange
    return message
  } catch(e) {
    LogUtil.info(`Failed to testOnTransferStateChange. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to testOnTransferStateChange. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

let receiveIncomingFile = false

export function testOnReceiveIncomingFile() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ot_2'))
    let textMessage = ""
    let profileType: number = AppStorage.get("profileType") !
    if (receiveIncomingFile) {
      (btProfileModel.getProfile(profileType) as opp.OppServerProfile).off("receiveIncomingFile",
        () => {
        promptAction.showToast({message: resStr($r('app.string.bluetoothtest_str_orjtygb'))})
      })
      message = resStr($r('app.string.bluetoothtest_str_gbjswjztgbsj'))
    } else {
      (btProfileModel.getProfile(profileType) as opp.OppServerProfile).on("receiveIncomingFile",
        (information) => {
        LogUtil.info("receiveIncomingFile enter")
        let deviceName: string = information.remoteDeviceName;
        let fileName: string = information.filePath;
        textMessage =
          resStr($r('app.string.bluetoothtest_str_r')) + deviceName +
            resStr($r('app.string.bluetoothtest_str_f')) + fileName;
        message =
          resStr($r('app.string.bluetoothtest_str_r')) + deviceName +
            resStr($r('app.string.bluetoothtest_str_f')) + fileName;
        AppStorage.SetOrCreate("textMessage", textMessage)
        promptAction.showToast({message: message});
      })
    }

    receiveIncomingFile = !receiveIncomingFile
    return message
  }
  catch(e) {
    LogUtil.info(`Failed to testOnReceiveIncomingFile. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to testOnReceiveIncomingFile. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSendFile(serverMac: string, fileName: string, fileType: string) {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_22'))
    let textMessage = ""
    let profileType: number = AppStorage.get("profileType") !
    let fileHolders: Array<opp.FileHolder> = [];
    (btProfileModel.getProfile(profileType) as opp.OppServerProfile).sendFile(
      serverMac, fileHolders).then(() => {
      LogUtil.info($r('app.string.bluetoothtest_str_ofswjcgr'));
      textMessage = resStr($r('app.string.bluetoothtest_str_tr'));
      AppStorage.SetOrCreate("textMessage", textMessage)
      message = resStr($r('app.string.bluetoothtest_str_ofswjcgr'))
      promptAction.showToast({message: message})
    }).catch((e: BusinessError) => {
      LogUtil.info(`Failed to testSendFile. message: ${e.message}, code: ${e.code}`)
      let errMessage = `Failed to testSendFile. message: ${e.message}, code: ${e.code}`
      promptAction.showToast({message: errMessage})
    })

    return message
  } catch(e) {
    LogUtil.info(`Failed to testSendFile. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to testSendFile. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testSetIncomingFileConfirmation() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_st_2'))
    let textMessage = ""
    let profileType: number = AppStorage.get("profileType") !
    let pathDir = "/test.jpg";
    let file = fs.openSync(pathDir, fs.OpenMode.CREATE | fs.OpenMode.READ_WRITE);
    (btProfileModel.getProfile(profileType) as
      opp.OppServerProfile).setIncomingFileConfirmation(true, file.fd).
      then(() => {
        let result = true;
        LogUtil.info("result: " + result);
        textMessage = resStr($r('app.string.bluetoothtest_str_r_2')) + result;
        AppStorage.SetOrCreate("textMessage", textMessage)
        message = resStr($r('app.string.bluetoothtest_str_r_2')) + result
        promptAction.showToast({message: message})
      }).catch((e: BusinessError) => {
        LogUtil.info(`Failed to testSetIncomingFileConfirmation. message: ${e.message}, code: ${e.code}`)
        let errMessage = `Failed to testSetIncomingFileConfirmation. message: ${e.message}, code: ${e.code}`
        promptAction.showToast({message: errMessage})
      })

    return message
  } catch(e) {
    LogUtil.info(`Failed to testSetIncomingFileConfirmation. message: ${e.message },
      code: ${e.code }`)
    let message = `Failed to testSetIncomingFileConfirmation. message: ${e.message },
      code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testGetCurrentTransferInformation() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_gt'))
    let textMessage = ""
    let profileType: number = AppStorage.get("profileType")!
    ;(btProfileModel.getProfile(profileType) as opp.OppServerProfile).
      getCurrentTransferInformation().then((value: opp.OppTransferInformation) => {
        let result = true;
        LogUtil.info(resStr($r('app.string.bluetoothtest_str_dqxx')) + JSON.stringify(result));
        textMessage = resStr($r('app.string.bluetoothtest_str_dqxx')) + JSON.stringify(result);
        AppStorage.SetOrCreate("textMessage", textMessage)
        message = resStr($r('app.string.bluetoothtest_str_dqxx')) + JSON.stringify(result)
        promptAction.showToast({message: message})
      }).catch((e: BusinessError) => {
        LogUtil.info(`Failed to testGetCurrentTransferInformation. message: ${e.message}, code: ${e.code}`)
        let errMessage = `Failed to testGetCurrentTransferInformation. message: ${e.message}, code: ${e.code}`
        promptAction.showToast({message: errMessage})
      })

    return message
  } catch(e) {
    LogUtil.info(`Failed to testGetCurrentTransferInformation. message: ${e.message },
      code: ${e.code }`)
    let message = `Failed to testGetCurrentTransferInformation. message: ${e.message },
      code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testCancelTransfer() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_ct_3'))
    let textMessage = ""
    let profileType: number = AppStorage.get("profileType")!
    ;(btProfileModel.getProfile(profileType) as opp.OppServerProfile).cancelTransfer().then(() => {
      let result = true;
      LogUtil.info(resStr($r('app.string.bluetoothtest_str_qxcs')) + JSON.stringify(result));
      textMessage = resStr($r('app.string.bluetoothtest_str_qxcs')) + JSON.stringify(result);
      AppStorage.SetOrCreate("textMessage", textMessage)
      message = resStr($r('app.string.bluetoothtest_str_qxcs')) + JSON.stringify(result)
      promptAction.showToast({message: message})
    }).catch((e: BusinessError) => {
      LogUtil.info(`Failed to testCancelTransfer. message: ${e.message}, code: ${e.code}`)
      let errMessage = `Failed to testCancelTransfer. message: ${e.message}, code: ${e.code}`
      promptAction.showToast({message: errMessage})
    })

    return message
  } catch(e) {
    LogUtil.info(`Failed to testCancelTransfer. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to testCancelTransfer. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}