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


import btmProfileModel from './btmProfileModel'
import promptAction from '@ohos.promptAction';
import btm from '@ohos.bluetoothManager'

/**
 * Profile A2DP Manager Test Page Of Bluetooth test
 */

export function testProfileConnectManager() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_pt_4'))
    let textMessage = ""
    let btConnectionState = AppStorage.get("btConnectionState") !
    let deviceId: string = AppStorage.get("deviceId") !
    let profileType: number = AppStorage.get("profileType") !
    if (btConnectionState == 1 || btConnectionState == 2) {
      promptAction.showToast({message: btConnectionState == 1 ?
        $r('app.string.bluetoothtest_str_gsbzzljz') : $r('app.string.bluetoothtest_str_gsbylj')});
      return message;
    }
    (btmProfileModel.getProfile(profileType) as btm.A2dpSourceProfile |
      btm.HandsFreeAudioGatewayProfile | btm.HidHostProfile).connect(deviceId);
    btConnectionState = 1;
    textMessage = resStr($r('app.string.bluetoothtest_str_cr'))
    message = resStr($r('app.string.bluetoothtest_str_cr'));
    AppStorage.setOrCreate("textMessage", textMessage)
    promptAction.showToast({message: message});
    return message
  } catch(e) {
    LogUtil.info(`Failed to testProfileConnectManager. message: ${e.message }, code: ${e.code }`)
    let message = `Failed to testProfileConnectManager. message: ${e.message }, code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testProfileDisConnectManager() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_pt_3'))
    let textMessage = ""
    let btConnectionState = AppStorage.get("btConnectionState") !
    let deviceId: string = AppStorage.get("deviceId") !
    let profileType: number = AppStorage.get("profileType") !
    if (btConnectionState == 0 || btConnectionState == 3) {
      promptAction.showToast({message: btConnectionState == 0 ?
        $r('app.string.bluetoothtest_str_gsbydk') : $r('app.string.bluetoothtest_str_gsbzzdklj')})
      return message;
    }
    btmProfileModel.getProfile(profileType)!.disconnect(deviceId);
    btConnectionState = 3;
    textMessage = resStr($r('app.string.bluetoothtest_str_dr'));
    AppStorage.setOrCreate("textMessage", textMessage)
    message = resStr($r('app.string.bluetoothtest_str_dr'));
    promptAction.showToast({message: message});
    return message
  } catch(e) {
    LogUtil.info(`Failed to testProfileDisConnectManager. message: ${e.message },
      code: ${e.code }`)
    let message = `Failed to testProfileDisConnectManager. message: ${e.message },
      code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testProfileGetPlayingStateManager() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_pt_1'))
    let textMessage = ""
    let deviceId: string = AppStorage.get("deviceId") !
    let profileType: string = AppStorage.get("profileType") !
    promptAction.showToast({message: profileType})
    LogUtil.info("profileType:" + profileType)
    let profile: btm.A2dpSourceProfile
    profile = btmProfileModel.getProfile(parseInt(profileType)) as btm.A2dpSourceProfile
    let ret = - 1
    if (profile) {
      ret = profile.getPlayingState(deviceId)
    }
    textMessage = resStr($r('app.string.bluetoothtest_str_gr_1')) + ret;
    message = resStr($r('app.string.bluetoothtest_str_gr_1')) + ret;
    AppStorage.setOrCreate("textMessage", textMessage)
    promptAction.showToast({message: message});
    return message
  } catch(e) {
    LogUtil.info(`Failed to testProfileGetPlayingStateManager. message: ${e.message },
      code: ${e.code }`)
    let message = `Failed to testProfileGetPlayingStateManager. message: ${e.message },
      code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testProfileGetConnectionDevicesManager() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_pt'))
    let textMessage = ""
    let profileType: number = AppStorage.get("profileType") !
    let msgArray: Array<string> = btmProfileModel.getProfile(profileType)!.getConnectionDevices()
    textMessage = resStr($r('app.string.bluetoothtest_str_gr')) + JSON.stringify(msgArray);
    message = resStr($r('app.string.bluetoothtest_str_gr')) + JSON.stringify(msgArray);
    AppStorage.setOrCreate("textMessage", textMessage)
    promptAction.showToast({message: message});
    return message
  } catch(e) {
    LogUtil.info(`Failed to testProfileGetConnectionDevicesManager. message: ${e.message },
      code: ${e.code }`)
    let message = `Failed to testProfileGetConnectionDevicesManager. message: ${e.message },
      code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}

export function testProfileGetDeviceStateManager() {
  try {
    let message = resStr($r('app.string.bluetoothtest_str_pt_2'))
    let textMessage = ""
    let deviceId: string = AppStorage.get("deviceId") !
    let profileType: number = AppStorage.get("profileType") !
    btmProfileModel.getProfile(profileType)!.getDeviceState(deviceId)
    textMessage = resStr($r('app.string.bluetoothtest_str_gr_2'));
    message = resStr($r('app.string.bluetoothtest_str_gr_2'));
    AppStorage.setOrCreate("textMessage", textMessage)
    promptAction.showToast({message: message});
    return message
  } catch(e) {
    LogUtil.info(`Failed to testProfileGetDeviceStateManager. message: ${e.message },
      code: ${e.code }`)
    let message = `Failed to testProfileGetDeviceStateManager. message: ${e.message },
      code: ${e.code }`
    promptAction.showToast({message: message})
    return message
  }
}
