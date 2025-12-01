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

import { myParams } from '../../Utils/common'
import { initHitHostProfileApiData } from '../../MainAbility/model/testDataModels'
import { ContentTable } from '../../Component/contentTable';
import router from '@ohos.router';
import btProfileModel from '../../MainAbility/model/btProfileModel';
import ConfigData from '../../Utils/ConfigData';
import LogUtil from '../../Utils/LogUtil';
import { PageTitle } from '../../Component/pageTitle';
import { TestImageDisplay } from '../../Component/testImageDisplay';
import { TestData } from '../../MainAbility/model/testData'
import bluetooth from '@ohos.bluetooth';
import promptAction from '@ohos.promptAction';

/**
 * hidHostProfile Test Page Of Bluetooth test
 */

@Entry
@Component
struct HidHostProfileTest {
  @State hasProfile: boolean = false;
  @StorageLink('textMessage') hidHostMessage: string = "";
  @State connectionStateInfo: string = "";
  @State title: Resource = $r('app.string.hid_host_profile_test_title');
  @State profileExist: string = ""
  @State profileNotExist: string = ""
  @State profileType: number = 0;
  @State bluetoothState: number = 0;
  @State btConnectionState: number = 0;
  @State currentClick: number = - 1;
  private pageType = 6;
  private deviceId = "8C:5A:F8:25:FB:E9";
  private testItem: TestData = (router.getParams() as myParams).testItem

  aboutToAppear() {
    LogUtil.log(ConfigData.TAG + "xzyj: aboutToAppear a2dp: deviceId " + JSON.stringify(this.deviceId));
    AppStorage.setOrCreate("deviceId" , this.deviceId)
    btProfileModel.initProfile()
  }

  build() {
    Row() {
      Column() {
        Stack({ alignContent : Alignment.TopStart }) {
          TestImageDisplay({ testItem : this.testItem })
          PageTitle({ testItem : this.testItem })
        }

        Stack().height("0.5vp").backgroundColor("#000000");
        Text(this.hasProfile ? this.profileExist : this.profileNotExist)
          .margin({ top : "10vp" })
          .fontSize("20vp")
          .fontColor(this.hasProfile ? "#66ccff" : "#ff0000")
          .border({ width : 0.5 })
          .padding(5)
          .fontStyle(FontStyle.Italic);
        Column() {
          Row() {
            Text("现在正在连接的设备为：")
              .fontSize("17").width(110).margin("5vp")
            TextInput({ text : JSON.stringify(this.deviceId) , placeholder : "请输入需要连接的deviceId" })
              .fontSize("17vp")
              .onChange((str) => {
                this.deviceId = str;
                AppStorage.setOrCreate("deviceId" , str)
              })
              .width("70%")
              .borderRadius(1)
          }
          .backgroundColor($r("app.color.moon"))
          .margin({ top : "20vp" , bottom : "20vp" , left : "20vp" , right : "20vp" })
          .alignItems(VerticalAlign.Center)

          Column() {
            Text("接口调用结果： " + this.hidHostMessage)
              .fontSize("17vp")
              .textAlign(TextAlign.Start)
              .margin({ top : "5vp" , left : "10vp" })
              .width($r('app.float.wh_value_100p'))
            Text("connectionStateChange： " + this.connectionStateInfo)
              .fontSize("17vp")
              .margin({ top : "10vp" , left : "10vp" })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_90p'))
          }
          .height(90)
          .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          .width("88%")

          Text("请点击需要测试的方法")
            .margin({ top : "10vp" })
            .padding("5vp")
            .fontSize("17vp")
            .fontColor("#4444444");
          Column() {
            ContentTable({ apiItems : initHitHostProfileApiData() })
          }
          .height("66%")
        }
        .visibility(this.hasProfile ? Visibility.Visible : Visibility.Hidden);
      }
      .width($r('app.float.wh_value_100p'))
    }
    .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
    .height($r('app.float.wh_value_100p'))
    .alignItems(VerticalAlign.Top)
  }

  onPageShow() {
    this.bluetoothState = btProfileModel.getBluetoothState();
    if ( this.bluetoothState == btProfileModel.BLUETOOTH_STATE_OFF ) {
      this.profileNotExist = "蓝牙未打开，请先开启蓝牙后再使用！"
      return;
    }
    this.hasProfile = btProfileModel.hasProfile(btProfileModel.PROFILE_HID_HOST);
    console.log("bt_test pageType: " + this.pageType);
    switch ( this.pageType ) {
    //      case 0:
    //        this.title = $r('app.string.a2dp_sink_profile_test_title');
    //        this.profileExist = "A2DP_SINK exist";
    //        this.profileNotExist = "A2DP_SINK does not exist";
    //        this.profileType = btProfileModel.PROFILE_A2DP_SINK;
    //        break;
      case 1:
        this.title = $r('app.string.a2dp_sour_profile_test_title');
        this.profileExist = "A2DP_SOURCE exist";
        this.profileNotExist = "A2DP_SOURCE does not exist";
        this.profileType = btProfileModel.PROFILE_A2DP_SOURCE;
        break;
    //      case 2:
    //        this.title = $r('app.string.avrcp_ct_profile_test_title');
    //        this.profileExist = "AVRCP_CT exist";
    //        this.profileNotExist = "AVRCP_CT does not exist";
    //        this.profileType = btProfileModel.PROFILE_AVRCP_CT;
    //        break;
    //      case 3:
    //        this.title = $r('app.string.avrcp_tg_profile_test_title');
    //        this.profileExist = "AVRCP_TG exist";
    //        this.profileNotExist = "AVRCP_TG does not exist";
    //        this.profileType = btProfileModel.PROFILE_AVRCP_TG;
    //        break;
      case 4:
        this.title = $r('app.string.hands_free_audio_gateway_profile_test_title');
        this.profileExist = "HANDS_FREE_AUDIO_GATEWAY exist";
        this.profileNotExist = "HANDS_FREE_AUDIO_GATEWAY does not exist";
        this.profileType = btProfileModel.PROFILE_HANDS_FREE_AUDIO_GATEWAY;
        break;
    //      case 5:
    //        this.title = $r('app.string.hands_free_unit_profile_test_title');
    //        this.profileExist = "HANDS_FREE_UNIT exist";
    //        this.profileNotExist = "HANDS_FREE_UNIT does not exist";
    //        this.profileType = btProfileModel.PROFILE_HANDS_FREE_UNIT;
    //        break;
      case 6:
        this.title = $r('app.string.hid_host_profile_test_title');
        this.profileExist = "HID HOST exist";
        this.profileNotExist = "HID HOST does not exist";
        this.profileType = btProfileModel.PROFILE_HID_HOST;
        break;
      case 7:
        this.title = $r('app.string.pan_network_profile_test_title');
        this.profileExist = "PAN_NETWORK exist";
        this.profileNotExist = "PAN_NETWORK does not exist";
        this.profileType = btProfileModel.PROFILE_PAN_NETWORK;
        break;
    //      case 8:
    //        this.title = $r('app.string.p_bap_clinet_profile_test_title');
    //        this.profileExist = "PBAP_CLIENT exist";
    //        this.profileNotExist = "PBAP_CLIENT does not exist";
    //        this.profileType = btProfileModel.PROFILE_PBAP_CLIENT;
    //        break;
    //      case 9:
    //        this.title = $r('app.string.p_bap_server_profile_test_title');
    //        this.profileExist = "PBAP_SERVER exist";
    //        this.profileNotExist = "PBAP_SERVER does not exist";
    //        this.profileType = btProfileModel.PROFILE_PBAP_SERVER;
    //        break;

      case 10:
        this.title = $r('app.string.opp_profile_test_title');
        this.profileExist = "Opp exist";
        this.profileNotExist = "Opp does not exist";
        this.profileType = btProfileModel.PROFILE_OPP;
        break;
      default:
        router.back();
    }
    AppStorage.setOrCreate("profileType" , this.profileType)
    promptAction.showToast({ message : this.profileType.toString() })
    this.hasProfile = btProfileModel.hasProfile(this.profileType);
    if ( this.hasProfile ) {
      console.log("bt_test registered connectionStateChange type: " + this.profileType);
      (btProfileModel.getProfile(this.profileType) as bluetooth.A2dpSourceProfile | bluetooth.HandsFreeAudioGatewayProfile).on("connectionStateChange" , (date: bluetooth.StateChangeParam) => {
        this.connectionStateInfo = JSON.stringify(date);
        this.btConnectionState = date.state;
        AppStorage.setOrCreate("btConnectionState" , this.btConnectionState)
      });
    }
  }

  onPageHide() {
    if ( this.hasProfile ) {
      (btProfileModel.getProfile(this.profileType) as bluetooth.A2dpSourceProfile | bluetooth.HandsFreeAudioGatewayProfile).off("connectionStateChange");
    }
    console.log("bt_test Ability end");
  }
}