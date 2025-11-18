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

/**
 * BT ProfileModel Test Page Of Bluetooth test
 */

import ConfigData from '../../Utils/ConfigData';
import BaseModel from '../../Utils/BaseModel';
import bt from '@ohos.bluetooth';
import hid from '@ohos.bluetooth.hid'
import pan from '@ohos.bluetooth.pan'
import { opp } from '@kit.ConnectivityKit'


export class BTProfileModel extends BaseModel {
  private TAG = ConfigData.TAG + 'BTProfileModel';
  private profiles: bt.A2dpSourceProfile | bt.HandsFreeAudioGatewayProfile | hid.HidHostProfile |
    pan.PanProfile | opp.OppServerProfile | null [] = []
  public PROFILE_A2DP_SOURCE = 1;
  public PROFILE_HANDS_FREE_AUDIO_GATEWAY = 4;
  public PROFILE_HID_HOST = 6;
  public PROFILE_PAN_NETWORK = 7;
  public PROFILE_OPP = 10;
  public BLUETOOTH_STATE_OFF = 0;

  constructor() {
    super();
  }

  initProfile() {
    console.log('bt_test BTProfileModel start');

    this.profiles[ this.PROFILE_A2DP_SOURCE ] = bt.getProfile(this.PROFILE_A2DP_SOURCE) as bt.A2dpSourceProfile;

    this.profiles[ this.PROFILE_HANDS_FREE_AUDIO_GATEWAY ] = bt.getProfile(this.PROFILE_HANDS_FREE_AUDIO_GATEWAY) as bt.HandsFreeAudioGatewayProfile;

    this.profiles[ this.PROFILE_HID_HOST ] = hid.createHidHostProfile() as hid.HidHostProfile;

    this.profiles[ this.PROFILE_PAN_NETWORK ] = pan.createPanProfile() as pan.PanProfile;

    this.profiles[ this.PROFILE_OPP ] = opp.createOppServerProfile();

    console.log("this.profiles[ this.PROFILE_A2DP_SOURCE ]" + JSON.stringify(bt.getProfile(this.PROFILE_A2DP_SOURCE)))
    console.log("this.profiles[ this.PROFILE_HANDS_FREE_AUDIO_GATEWAY ]" + bt.getProfile(this.PROFILE_HANDS_FREE_AUDIO_GATEWAY))
    console.log("this.profiles[ this.PROFILE_HID_HOST ]" + this.profiles[ this.PROFILE_HID_HOST ])
    console.log("this.profiles[ this.PROFILE_PAN_NETWORK ]" + this.profiles[ this.PROFILE_PAN_NETWORK ])
    console.log("this.profiles[ this.PROFILE_OPP ]" + bt.getProfile(this.PROFILE_OPP))
    console.log('bt_test BTProfileModel end');
  }

  hasProfile(type: number): boolean {
    console.log("hasProfile1:" + JSON.stringify(this.profiles[ type ]))
    if ( !this.profiles[ type ] ) {
      return false;
    }
    return true;
  }

  getA2DPSourceProfile(): bt.A2dpSourceProfile | null {
    if ( this.profiles[ this.PROFILE_A2DP_SOURCE ] != null ) {
      return this.profiles[ this.PROFILE_A2DP_SOURCE ] as bt.A2dpSourceProfile;
    }
    return null;
  }

  getHandsFreeAudioGatewayProfile(): bt.HandsFreeAudioGatewayProfile | null {
    if ( this.profiles[ this.PROFILE_HANDS_FREE_AUDIO_GATEWAY ] != null ) {
      return this.profiles[ this.PROFILE_HANDS_FREE_AUDIO_GATEWAY ] as bt.HandsFreeAudioGatewayProfile;
    }
    return null;
  }

  getHidHostProfile(): hid.HidHostProfile | null {
    if ( this.profiles[ this.PROFILE_HID_HOST ] != null ) {
      return this.profiles[ this.PROFILE_HID_HOST ] as hid.HidHostProfile;
    }
    return null;
  }

  getPanNetworkProfile(): pan.PanProfile | null {
    if ( this.profiles[ this.PROFILE_PAN_NETWORK ] != null ) {
      return this.profiles[ this.PROFILE_PAN_NETWORK ] as pan.PanProfile;
    }
    return null;
  }

  getOppProfile(): opp.OppServerProfile | null {
    if ( this.profiles[ this.PROFILE_OPP ] != null ) {
      return this.profiles[ this.PROFILE_OPP ] as opp.OppServerProfile;
    }
    return null;
  }

  getProfile(type: number): bt.A2dpSourceProfile | bt.HandsFreeAudioGatewayProfile | hid.HidHostProfile |
    pan.PanProfile | opp.OppServerProfile | null {
    if ( this.profiles[ type ] != null ) {
      return this.profiles[ type ];
    }
    return null;
  }

  getBluetoothState() {
    return bt.getState();
  }
}

let btProfileModel = new BTProfileModel();

export default btProfileModel as BTProfileModel;