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

import { myParams , PairDevices } from '../../Utils/common'
import promptAction from '@ohos.promptAction';
import { ContentTable } from '../../Component/contentTable';
import { initOppProfileApiData } from '../../MainAbility/model/testDataModels'
import router from '@ohos.router';
import btProfileModel from '../../MainAbility/model/btProfileModel';
import ConfigData from '../../Utils/ConfigData';
import LogUtil from '../../Utils/LogUtil';
import { PageTitle } from '../../Component/pageTitle';
import { TestImageDisplay } from '../../Component/testImageDisplay';
import bluetooth from '@ohos.bluetooth';
import { TestData } from '../../MainAbility/model/testData'
import bluetoothManager from '@ohos.bluetoothManager';

/**
 * oppProfile Test Page Of Bluetooth test
 */

@Entry
@Component
struct OppProfileTest {
  @State hasProfile: boolean = false;
  @StorageLink('textMessage') oppMessage: string = "";
  @State connectionStateInfo: string = "";
  @State serverMac: string = "00:00:00:00:00:00";
  @State title: Resource = $r('app.string.opp_profile_test_title');
  @State profileExist: string = ""
  @State profileNotExist: string = ""
  @State profileType: number = 0;
  @State bluetoothState: number = 0;
  @State btConnectionState: number = 0;
  @State currentClick: number = - 1;
  private pageType = 10;
  private deviceId = "8C:5A:F8:25:FB:E9";
  private testItem: TestData = (router.getParams() as myParams).testItem
  private pairDevices: PairDevices[] = [];

  aboutToAppear() {
    LogUtil.log(ConfigData.TAG + "xzyj: aboutToAppear a2dp: deviceId " + JSON.stringify(this.deviceId));
    AppStorage.setOrCreate("deviceId" , this.deviceId)
    btProfileModel.initProfile()
    this.getPairDevice();
  }

  getPairDevice() {
    let macs = bluetooth.getPairedDevices();
    console.log("OppSend getPairDevice macs: " + JSON.stringify(macs));
    for ( let i = 0 ; i < macs.length ; i ++ ) {
      let name = bluetooth.getRemoteDeviceName(macs[ i ]);
      console.log("OppSend getPairDevice name: " + JSON.stringify(name));
      this.pairDevices.push({ id : macs[ i ] , name : name } as PairDevices);
    }
    console.log("OppSend getPairDevice pairDevices: " + JSON.stringify(this.pairDevices));
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
          Column() {
            Scroll() {
              Column() {
                Text("已配对设备列表：(点击选择设备)")
                  .fontSize("20vp")
                  .fontWeight(FontWeight.Bold)
                  .margin({ top : "5vp" })
                Scroll() {
                  List({ space : 5 }) {
                    ForEach(this.pairDevices , (item: PairDevices) => {
                      ListItem() {
                        Text(item.name)
                          .fontSize("25vp")
                          .width($r('app.float.wh_value_80p'))
                          .margin({ left : '10%' })
                          .backgroundColor(this.serverMac === item.id ? $r("app.color.moon") : $r("app.color.white"))
                          .border({
                            radius : 5
                          })
                      }
                      .onClick(() => {
                        this.serverMac = item.id;
                      })
                    })
                  }
                }
                .height("50%")
                .width($r('app.float.wh_value_80p'))

                Text("已选择的设备的蓝牙mac地址：")
                  .fontSize("18vp")
                  .fontWeight(FontWeight.Bold)
                  .margin({ top : "3vp" })
                Text(this.serverMac)
                  .fontSize("25vp")
                  .width("70%")
                  .borderRadius(5)
                  .backgroundColor($r("app.color.moon"))
              }
              .height("15%")
              .width($r('app.float.wh_value_100p'))
            }
            .scrollBarWidth(10)
            .scrollBar(BarState.Auto)
          }

          Stack().height("1vp").backgroundColor("#000000");

          Scroll() {
            Column() {
              Text("接口调用结果： " + this.oppMessage)
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
            .height(80)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
            .width("88%")
          }
          .scrollBarWidth(10)
          .scrollBar(BarState.Auto)

          Text("请点击需要测试的方法")
            .margin({ top : "10vp" })
            .padding("5vp")
            .fontSize("17vp")
            .fontColor("#4444444");
          Column() {
            ContentTable({ apiItems : initOppProfileApiData() })
          }
          .height("66%")
        }
        .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        .width($r('app.float.wh_value_100p'))
        .height($r('app.float.wh_value_100p'))
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
    this.hasProfile = btProfileModel.hasProfile(btProfileModel.PROFILE_OPP);
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
    //   case 6:
    //     this.title = $r('app.string.hid_host_profile_test_title');
    //     this.profileExist = "HID HOST exist";
    //     this.profileNotExist = "HID HOST does not exist";
    //     this.profileType = btProfileModel.PROFILE_HID_HOST;
    //     break;
    //   case 7:
    //     this.title = $r('app.string.pan_network_profile_test_title');
    //     this.profileExist = "PAN_NETWORK exist";
    //     this.profileNotExist = "PAN_NETWORK does not exist";
    //     this.profileType = btProfileModel.PROFILE_PAN_NETWORK;
    //     break;
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
      (btProfileModel.getProfile(this.profileType) as bluetooth.A2dpSourceProfile | bluetooth.HandsFreeAudioGatewayProfile).on("connectionStateChange" , (date: bluetoothManager.StateChangeParam) => {
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