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
import { PairDevices, testItemFromRouterOrPlaceholder } from '../../Utils/common'
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
import { shouldSkipBluetoothPermissionRequest } from '../../bluetoothTestNav';

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
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  private pairDevices: PairDevices[] = [];

  aboutToAppear() {
    LogUtil.log(ConfigData.TAG + "xzyj: aboutToAppear a2dp: deviceId " + JSON.stringify(this.deviceId));
    AppStorage.setOrCreate("deviceId" , this.deviceId)
    btProfileModel.initProfile()
    this.getPairDevice();
  }

  getPairDevice() {
    let macs = bluetooth.getPairedDevices();
    hilog.info(0x0000, 'BtTest', '%{public}s', "OppSend getPairDevice macs: " + JSON.stringify(macs));
    for ( let i = 0 ; i < macs.length ; i ++ ) {
      let name = bluetooth.getRemoteDeviceName(macs[ i ]);
      hilog.info(0x0000, 'BtTest', '%{public}s', "OppSend getPairDevice name: " + JSON.stringify(name));
      this.pairDevices.push({ id : macs[ i ] , name : name } as PairDevices);
    }
    hilog.info(0x0000, 'BtTest', '%{public}s', "OppSend getPairDevice pairDevices: " + JSON.stringify(this.pairDevices));
  }

  build() {
    Row() {
      Column() {
        Stack({ alignContent : Alignment.TopStart }) {
          TestImageDisplay({ testItem : this.testItem })
          PageTitle({ testItem : this.testItem })
          .accessibilityLevel('yes')
          .id('pages_subProfileTest_oppProfileTest__pt_0')
        }

        Stack()
          .height(ConfigData.VP_0_5)
          .backgroundColor($r('app.color.black'));
        Text(this.hasProfile ? this.profileExist : this.profileNotExist)
          .margin({ top : ConfigData.VP_10 })
          .fontSize(ConfigData.VP_20)
          .fontColor(this.hasProfile ? $r('app.color.color_66CCFF_blue') : $r('app.color.color_FF0000_red'))
          .border({ width : 0.5 })
          .padding(ConfigData.VP_5)
          .fontStyle(FontStyle.Italic);
        Column() {
          Column() {
            Scroll() {
              Column() {
                Text($r('app.string.bluetoothtest_text_ypdsblbdjxzsb'))
                  .fontSize(ConfigData.VP_20)
                  .fontWeight(FontWeight.Bold)
                  .margin({ top : ConfigData.VP_5 })
                Scroll() {
                  List({ space : ConfigData.VP_5 }) {
                    ForEach(
                      this.pairDevices.length > 0 ? this.pairDevices :
                        [{ id: '__uitest_placeholder__', name: '—' } as PairDevices],
                      (item: PairDevices, index: number) => {
                        ListItem() {
                          Text(item.name)
                            .fontSize(ConfigData.VP_25)
                            .width(ConfigData.WH_80_100)
                            .margin({ left : ConfigData.PCT_10 })
                            .backgroundColor(this.serverMac === item.id ? $r("app.color.moon") : $r("app.color.white"))
                            .border({
                              radius : 5
                            })
                        }
                        .id(index === 0 ? 'pages_subProfileTest_oppProfileTest__li_0' :
                          `pages_subProfileTest_oppProfileTest__li_${index}`)
                        .accessibilityLevel('yes')
                        .onClick(() => {
                          if (item.id !== '__uitest_placeholder__') {
                            this.serverMac = item.id;
                          }
                        })
                      },
                      (item: PairDevices) => item.id
                    )
                  }
                }
                .accessibilityLevel('yes')
                .id('pages_subProfileTest_oppProfileTest__sc_1')
                .height(ConfigData.WH_50_100)
                .width(ConfigData.WH_80_100)

                Text($r('app.string.bluetoothtest_text_yxzdsbdlymdz'))
                  .fontSize(ConfigData.VP_18)
                  .fontWeight(FontWeight.Bold)
                  .margin({ top : ConfigData.VP_3 })
                Text(this.serverMac)
                  .fontSize(ConfigData.VP_25)
                  .width(ConfigData.PCT_70)
                  .borderRadius(ConfigData.VP_5)
                  .backgroundColor($r("app.color.moon"))
              }
              .height(ConfigData.PCT_15)
              .width(ConfigData.WH_100_100)
            }
            .accessibilityLevel('yes')
            .id('pages_subProfileTest_oppProfileTest__sc_0')
            .scrollBarWidth(ConfigData.VP_10)
            .scrollBar(BarState.Auto)
          }

          Stack()
            .height(ConfigData.HEIGHT_1_VP)
            .backgroundColor($r('app.color.black'));

          Scroll() {
            Column() {
              Text($r('app.string.label_api_result_space') + this.oppMessage)
                .fontSize(ConfigData.VP_17)
                .textAlign(TextAlign.Start)
                .margin({ top : ConfigData.VP_5 , left : ConfigData.VP_10 })
                .width(ConfigData.WH_100_100)
              Text($r('app.string.label_connection_state_change') + this.connectionStateInfo)
                .fontSize(ConfigData.VP_17)
                .margin({ top : ConfigData.VP_10 , left : ConfigData.VP_10 })
                .textAlign(TextAlign.Start)
                .width(ConfigData.WH_90_100)
            }
            .height(ConfigData.VP_80)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
            .width(ConfigData.PCT_88)
          }
          .accessibilityLevel('yes')
          .id('pages_subProfileTest_oppProfileTest__sc_2')
          .scrollBarWidth(ConfigData.VP_10)
          .scrollBar(BarState.Auto)

          Text($r('app.string.bluetoothtest_text_qdjxycsdff'))
            .margin({ top : ConfigData.VP_10 })
            .padding(ConfigData.VP_5)
            .fontSize(ConfigData.VP_17)
            .fontColor($r('app.color.color_4444444_grey'));
          Column() {
            ContentTable({ apiItems : initOppProfileApiData() })
          }
          .height(ConfigData.PCT_66)
        }
        .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        .width(ConfigData.WH_100_100)
        .height(ConfigData.WH_100_100)
        .visibility(shouldSkipBluetoothPermissionRequest() || this.hasProfile ? Visibility.Visible : Visibility.Hidden);
      }
      .width(ConfigData.WH_100_100)
    }
    .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
    .height(ConfigData.WH_100_100)
    .alignItems(VerticalAlign.Top)
  }

  onPageShow() {
    this.bluetoothState = btProfileModel.getBluetoothState();
    if ( this.bluetoothState == btProfileModel.BLUETOOTH_STATE_OFF ) {
      this.profileNotExist = $r('app.string.bluetoothtest_str_lywdkqxkqlyhzsy')
      return;
    }
    this.hasProfile = btProfileModel.hasProfile(btProfileModel.PROFILE_OPP);
    hilog.info(0x0000, 'BtTest', '%{public}s', "bt_test pageType: " + this.pageType);
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
      hilog.info(0x0000, 'BtTest', '%{public}s', "bt_test registered connectionStateChange type: " + this.profileType);
      if (this.profileType === btProfileModel.PROFILE_A2DP_SOURCE ||
        this.profileType === btProfileModel.PROFILE_HANDS_FREE_AUDIO_GATEWAY) {
        (btProfileModel.getProfile(this.profileType) as bluetooth.A2dpSourceProfile | bluetooth.HandsFreeAudioGatewayProfile).on("connectionStateChange" , (date: bluetoothManager.StateChangeParam) => {
          this.connectionStateInfo = JSON.stringify(date);
          this.btConnectionState = date.state;
          AppStorage.setOrCreate("btConnectionState" , this.btConnectionState)
        });
      }
    }
  }

  onPageHide() {
    if ( this.hasProfile ) {
      if (this.profileType === btProfileModel.PROFILE_A2DP_SOURCE ||
        this.profileType === btProfileModel.PROFILE_HANDS_FREE_AUDIO_GATEWAY) {
        (btProfileModel.getProfile(this.profileType) as bluetooth.A2dpSourceProfile | bluetooth.HandsFreeAudioGatewayProfile).off("connectionStateChange");
      }
    }
    hilog.info(0x0000, 'BtTest', '%{public}s', "bt_test Ability end");
  }
}