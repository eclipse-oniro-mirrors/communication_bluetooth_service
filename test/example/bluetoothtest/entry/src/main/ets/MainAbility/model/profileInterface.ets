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
import { resStr} from '../../Utils/resString';


import router from '@ohos.router';
import promptAction from '@ohos.promptAction';

const PAGE_URI_GET_A2DP_SOURCE_PROFILE = 'pages/subProfileTest/a2dpSourceProfileTest';
const PAGE_URI_GET_HANDS_FREE_AUDIO_GATEWAY_PROFILE =
  'pages/subProfileTest/handsFreeAudioGatewayProfileTest';
const PAGE_URI_GET_HID_HOST_PROFILE = 'pages/subProfileTest/hidHostProfileTest';
const PAGE_URI_GET_Pan_NETWORK_PROFILE = 'pages/subProfileTest/panNetworkProfileTest';


export function testA2dpSourceProfile(): string {
  router.pushUrl({url: PAGE_URI_GET_A2DP_SOURCE_PROFILE})
  let message = resStr($r('app.string.bluetoothtest_str_tzcg'))
  promptAction.showToast({message: message})
  return message
}

export function testHandsFreeAudioGatewayProfile(): string {
  router.pushUrl({url: PAGE_URI_GET_HANDS_FREE_AUDIO_GATEWAY_PROFILE})
  let message = resStr($r('app.string.bluetoothtest_str_tzcg'))
  promptAction.showToast({message: message})
  return message
}

export function testHidHostProfile(): string {
  router.pushUrl({url: PAGE_URI_GET_HID_HOST_PROFILE})
  let message = resStr($r('app.string.bluetoothtest_str_tzcg'))
  promptAction.showToast({message: message})
  return message
}

export function testPanNetworkProfile(): string {
  router.pushUrl({url: PAGE_URI_GET_Pan_NETWORK_PROFILE})
  let message = resStr($r('app.string.bluetoothtest_str_tzcg'))
  promptAction.showToast({message: message})
  return message
}
