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
          Text("目标设备MAC：").fontSize($r('app.float.wh_value_20'))
          TextInput({ text : this.deviceId , placeholder : "请输入mac地址" })
            .fontSize($r('app.float.wh_value_20'))
            .onChange((str) => {
              this.deviceId = str;
              AppStorage.setOrCreate('inputDeviceId' , str)
            })
            .width($r('app.float.wh_value_70p'))
        }
        .backgroundColor($r("app.color.moon"))
        .margin({ top : $r('app.float.wh_value_20') , left : $r('app.float.wh_value_20') , right : $r('app.float.wh_value_20') })
        .alignItems(VerticalAlign.Center)

        List() {
          ListItem() {
            TitleComponent({
              title : "A2DP_SOURCE" ,
              fontSize : $r('app.float.wh_value_15') ,
              bgColor : this.currentClick === 0 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
            });
          }
          .padding({ top : $r('app.float.wh_value_20') })
          .height($r('app.float.wh_value_80'))
          .borderRadius($r('app.float.wh_value_10'))
          .onClick(() => {
            this.currentClick = 0;
            this.profileId = 1;
            let ret = bt.getProfile(this.profileId).getDeviceState(this.deviceId);
            console.log('A2DP state is -' + ret);
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
            this.message = "当前A2DP的状态是：" + this.stateProfile;
          })

          ListItem() {
            TitleComponent({
              title : "HANDS_FREE_AUDIO_GATEWAY" ,
              fontSize : $r('app.float.wh_value_15') ,
              bgColor : this.currentClick === 1 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
            });
          }
          .padding({ top : $r('app.float.wh_value_20') })
          .height($r('app.float.wh_value_80'))
          .borderRadius($r('app.float.wh_value_10'))
          .onClick(() => {
            this.currentClick = 1;
            this.profileId = 4;
            let ret = bt.getProfile(this.profileId).getDeviceState(this.deviceId);
            console.log('HFP state is -' + ret);
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
            this.message = "当前HFP的状态是：" + this.stateProfile;
          })

          ListItem() {
            TitleComponent({
              title : "HID_HOST" ,
              fontSize : $r('app.float.wh_value_15') ,
              bgColor : this.currentClick === 2 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
            });
          }
          .padding({ top : $r('app.float.wh_value_20') })
          .height($r('app.float.wh_value_80'))
          .borderRadius($r('app.float.wh_value_10'))
          .onClick(() => {
            this.currentClick = 2;
            this.profileId = 6;
            let ret = bt.getProfile(this.profileId).getDeviceState(this.deviceId);
            console.log('HID state is -' + ret);
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
            this.message = "当前HID的状态是：" + this.stateProfile;
          })
        }
        .height($r('app.float.wh_value_300'))
        .margin($r('app.float.wh_value_20'));

        Text("接口调用结果：" + this.message)
          .textAlign(TextAlign.Start)
          .fontSize('17fp')
          .backgroundColor($r("app.color.white"))
      }.width($r('app.float.wh_value_100p'))
    }
    .height($r('app.float.wh_value_100p'))
    .alignItems(VerticalAlign.Top)
    .backgroundColor($r("app.color.lead"))
  }
}