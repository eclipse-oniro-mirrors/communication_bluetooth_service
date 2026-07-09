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
import ConfigData from '../../Utils/ConfigData';
import BaseModel from '../../Utils/BaseModel';
import btm from '@ohos.bluetoothManager';
import pan from '@ohos.bluetooth.pan';

/**
 * BT ProfileModel Test Page Of Bluetooth test
 */

export class BTMProfileModel extends BaseModel {
  private TAG = ConfigData.TAG + 'BTMProfileModel';
  private profiles: btm.A2dpSourceProfile | btm.HandsFreeAudioGatewayProfile | btm.HidHostProfile |
    btm.PanProfile | null [] = []
  public PROFILE_A2DP_SOURCE = 1;
  public PROFILE_HANDS_FREE_AUDIO_GATEWAY = 4;
  public PROFILE_HID_HOST = 6;
  public PROFILE_PAN_NETWORK = 7;
  public BLUETOOTH_STATE_OFF = 0;

  constructor() {
    super();
  }

  initProfile(): void {
    LogUtil.info('btm_test BTProfileModel start');

    this.profiles[this.PROFILE_A2DP_SOURCE] = btm.getProfileInstance(this.PROFILE_A2DP_SOURCE) as
      btm.A2dpSourceProfile;

    this.profiles[this.PROFILE_HANDS_FREE_AUDIO_GATEWAY] =
      btm.getProfileInstance(this.PROFILE_HANDS_FREE_AUDIO_GATEWAY) as
        btm.HandsFreeAudioGatewayProfile;

    this.profiles[this.PROFILE_HID_HOST] = btm.getProfileInstance(this.PROFILE_HID_HOST) as
      btm.HidHostProfile;

    this.profiles[this.PROFILE_PAN_NETWORK] = btm.getProfileInstance(this.PROFILE_PAN_NETWORK) as
      btm.PanProfile;

    LogUtil.info('bt_test BTProfileModel end');
  }

  hasProfile(type: number): boolean {
    if (!this.profiles[type]) {
      return false;
    } else {
      return true;
    }
  }

  getA2DPSourceProfile(): btm.A2dpSourceProfile | null {
    if (this.profiles[this.PROFILE_A2DP_SOURCE] != null) {
      return this.profiles[this.PROFILE_A2DP_SOURCE]
    } else {
      return null
    }

  }

  getHandsFreeAudioGatewayProfile(): btm.HandsFreeAudioGatewayProfile | null {
    if (this.profiles[this.PROFILE_HANDS_FREE_AUDIO_GATEWAY] != null) {
      return this.profiles[this.PROFILE_HANDS_FREE_AUDIO_GATEWAY];
    } else {
      return null
    }
  }

  getHidHostProfile(): btm.HidHostProfile | null {
    if (this.profiles[this.PROFILE_HID_HOST] != null) {
      return this.profiles[this.PROFILE_HID_HOST];
    } else {
      return null
    }
  }

  getPanNetworkProfile(): btm.PanProfile | null {
    if (this.profiles[this.PROFILE_PAN_NETWORK] != null) {
      return this.profiles[this.PROFILE_PAN_NETWORK];
    } else {
      return null
    }
  }

  getProfile(type: number): btm.A2dpSourceProfile | btm.HandsFreeAudioGatewayProfile |
    btm.HidHostProfile | btm.PanProfile | null {
    if (this.profiles[type] != null) {
      return this.profiles[type];
    } else {
      LogUtil.info("This.profiles[ type ] = null")
      return null
    }
  }

  getBluetoothState(): number {
    return btm.getState()
  }
}

let btmProfileModel = new BTMProfileModel();

export default btmProfileModel as BTMProfileModel;