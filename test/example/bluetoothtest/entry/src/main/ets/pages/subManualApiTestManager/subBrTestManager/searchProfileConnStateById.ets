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
import hilog from '@ohos.hilog';
import ConfigData from '../../../Utils/ConfigData';
import { TitleComponent } from '../../../Component/titleComponent';
import bt from '@ohos.bluetooth';

/**
 * about phone
 */
@Entry
@Component
struct profileConnState2 {
  @State message: string = "";
  @State currentClick: number = - 1;
  @State profileId: number = - 1;
  private stateProfile: string = ""
  @State deviceId: string = AppStorage.get('inputDeviceId');

  build() {
    Row() {
      Column() {
        Row() {
          Text($r('app.string.bluetoothtest_text_mbsbm'))
            .fontSize(ConfigData.VP_20)
          TextInput({ text : this.deviceId , placeholder : $r('app.string.bluetoothtest_str_qsrmdz') })
            .fontSize(ConfigData.VP_20)
            .onChange((str) => {
              this.deviceId = str;
              AppStorage.setOrCreate('inputDeviceId' , str)
            })
            .width(ConfigData.PCT_70)
        }
        .backgroundColor($r("app.color.moon"))
        .margin({ top : ConfigData.VP_20 , left : ConfigData.VP_20 , right : ConfigData.VP_20 })
        .alignItems(VerticalAlign.Center)

        List() {
          ListItem() {
            TitleComponent({
              title : "A2DP_SOURCE" ,
              fontSize : ConfigData.VP_15 ,
              bgColor : this.currentClick === 0 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
            });
          }
          .padding({ top : ConfigData.VP_20 })
          .height(ConfigData.VP_80)
          .borderRadius(ConfigData.VP_10)
          .onClick(() => {
            this.currentClick = 0;
            this.profileId = 1;
            let ret = bt.getProfile(this.profileId).getDeviceState(this.deviceId);
            hilog.info(0x0000, 'BtTest', '%{public}s', 'A2DP state is -' + ret);
            switch ( ret ) {
              case 0:
                this.stateProfile = 'STATE_DISCONNECTED';
                break;
              case 1:
                this.stateProfile = 'STATE_CONNECTING';
                break;
              case 2:
                this.stateProfile = 'STATE_CONNECTED';
                break;
              case 3:
                this.stateProfile = 'STATE_DISCONNECTING';
                break;
              case - 1:
                this.stateProfile = 'FAIL';
                break;
            }
            this.message = $r('app.string.bluetoothtest_str_dqadzts') + this.stateProfile;
          })

          ListItem() {
            TitleComponent({
              title : "HANDS_FREE_AUDIO_GATEWAY" ,
              fontSize : ConfigData.VP_15 ,
              bgColor : this.currentClick === 1 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
            });
          }
          .padding({ top : ConfigData.VP_20 })
          .height(ConfigData.VP_80)
          .borderRadius(ConfigData.VP_10)
          .onClick(() => {
            this.currentClick = 1;
            this.profileId = 4;
            let ret = bt.getProfile(this.profileId).getDeviceState(this.deviceId);
            hilog.info(0x0000, 'BtTest', '%{public}s', 'HFP state is -' + ret);
            switch ( ret ) {
              case 0:
                this.stateProfile = 'STATE_DISCONNECTED';
                break;
              case 1:
                this.stateProfile = 'STATE_CONNECTING';
                break;
              case 2:
                this.stateProfile = 'STATE_CONNECTED';
                break;
              case 3:
                this.stateProfile = 'STATE_DISCONNECTING';
                break;
              case - 1:
                this.stateProfile = 'FAIL';
                break;
            }
            this.message = $r('app.string.bluetoothtest_str_dqhdzts') + this.stateProfile;
          })

          ListItem() {
            TitleComponent({
              title : "HID_HOST" ,
              fontSize : ConfigData.VP_15 ,
              bgColor : this.currentClick === 2 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
            });
          }
          .padding({ top : ConfigData.VP_20 })
          .height(ConfigData.VP_80)
          .borderRadius(ConfigData.VP_10)
          .onClick(() => {
            this.currentClick = 2;
            this.profileId = 6;
            let ret = bt.getProfile(this.profileId).getDeviceState(this.deviceId);
            hilog.info(0x0000, 'BtTest', '%{public}s', 'HID state is -' + ret);
            switch ( ret ) {
              case 0:
                this.stateProfile = 'STATE_DISCONNECTED';
                break;
              case 1:
                this.stateProfile = 'STATE_CONNECTING';
                break;
              case 2:
                this.stateProfile = 'STATE_CONNECTED';
                break;
              case 3:
                this.stateProfile = 'STATE_DISCONNECTING';
                break;
              case - 1:
                this.stateProfile = 'FAIL';
                break;
            }
            this.message = $r('app.string.bluetoothtest_str_dqhdzts_1') + this.stateProfile;
          })
        }
        .height(ConfigData.VP_300)
        .margin(ConfigData.VP_20);

        Text($r('app.string.label_api_result_tight') + this.message)
          .textAlign(TextAlign.Start)
          .fontSize(ConfigData.FP_17)
          .backgroundColor($r("app.color.white"))
      }
        .width(ConfigData.WH_100_100)
    }
    .height(ConfigData.WH_100_100)
    .alignItems(VerticalAlign.Top)
    .backgroundColor($r("app.color.lead"))
  }
}