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


import ConfigData from '../../Utils/ConfigData';
import { PairDevices, testItemFromRouterOrPlaceholder} from '../../Utils/common'
import promptAction from '@ohos.promptAction';
import { PageTitle} from '../../component/pageTitle';
import { TestImageDisplay} from '../../component/testImageDisplay';
import { TestData} from '../../MainAbility/model/testData'
import bluetooth from '@ohos.bluetooth';
import { opp} from '@kit.ConnectivityKit';

/**
 * OppSend Scenario Test Page Of Bluetooth test
 */

@Entry
@Component
struct oppSendScenarioTest {
  @State changeIndex: number = - 1;
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @StorageLink("oppSendScenarioMessage") oppSendScenarioMessage: string = ""
  private oppProfile!: opp.OppServerProfile;
  private pairDevices: PairDevices[] = []
  @State serverMac: string = "00:00:00:00:00:00";
  private fileName: string = "test.jpg";
  private fileType: string = "jpg"

  aboutToAppear() {
    try {
      this.oppProfile = opp.createOppServerProfile();
      testLog("OppSend get opp profile success");
    } catch(error) {
      testLog("OppSend get opp profile fail, err: " + JSON.stringify(error));
    }
    this.getPairDevice();
  }

  getPairDevice() {
    let macs = bluetooth.getPairedDevices();
    LogUtil.info("OppSend getPairDevice macs: " + JSON.stringify(macs));
    for(let i = 0 ; i<macs.length ; i++) {
      let name = bluetooth.getRemoteDeviceName(macs[i]);
      LogUtil.info("OppSend getPairDevice name: " + JSON.stringify(name));
      this.pairDevices.push({
        id: macs[i],
        name: name
      });
    }
    LogUtil.info("OppSend getPairDevice pairDevices: " + JSON.stringify(this.pairDevices));
  }

  build() {
    Column() {
      Stack({alignContent: Alignment.TopStart}) {
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
        .accessibilityLevel('yes')
        .id('pages_subScenarioTest_oppSendScenarioTest__pt_0')
      }

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column() {
        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_ypdsblbdjxzsb'))
              .fontSize(ConfigData.VP_20)
              .fontWeight(FontWeight.Bold)
              .margin({top: ConfigData.VP_5})
            Scroll() {
              List({space: ConfigData.VP_5}) {
                ForEach(this.pairDevices, (item: PairDevices, index: number) => {
                  ListItem() {
                    Text(item.name)
                      .fontSize(ConfigData.VP_25)
                      .width(ConfigData.WH_80_100)
                      .margin({left: ConfigData.PCT_10})
                      .backgroundColor(this.serverMac === item.id ? $r("app.color.moon") :
                        $r("app.color.white"))
                      .border({
                        radius: 5
                      })
                  }
                  .id(index === 0 ? 'pages_subScenarioTest_oppSendScenarioTest__li_0' :
                    `pages_subScenarioTest_oppSendScenarioTest__li_${index}`)
                  .onClick(() => {
                    this.serverMac = item.id;
                  })
                })
              }
            }
            .accessibilityLevel('yes')
            .id('pages_subScenarioTest_oppSendScenarioTest__sc_1')
            .height(ConfigData.WH_80_100)
            .width(ConfigData.WH_80_100)
          }
          .height(ConfigData.WH_30_100)
          .width(ConfigData.WH_100_100)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTest_oppSendScenarioTest__sc_0')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));

        Column() {
          Text($r('app.string.bluetoothtest_text_yxzdsbdlymdz'))
            .fontSize(ConfigData.VP_18)
            .fontWeight(FontWeight.Bold)
            .margin({top: ConfigData.VP_3})
          Text(this.serverMac)
            .fontSize(ConfigData.VP_25)
            .width(ConfigData.PCT_70)
            .borderRadius(ConfigData.VP_5)
            .backgroundColor($r("app.color.moon"))
        }
          .height(ConfigData.PCT_15)

        Column() {
          Text($r('app.string.bluetoothtest_text_qsrxyfsdwjmcrtt'))
            .fontSize(ConfigData.VP_18)
            .fontWeight(FontWeight.Bold)
          Text($r('app.string.bluetoothtest_text_zwjbxzdblmlx'))
            .fontSize(ConfigData.VP_18)
            .fontWeight(FontWeight.Bold)
          TextInput({placeholder: $r('app.string.bluetoothtest_str_wjmc')})
            .accessibilityLevel('yes')
            .id('pages_subScenarioTest_oppSendScenarioTest__ti_0')
            .fontSize(ConfigData.VP_20)
            .onChange((str) => {
              if (str) {
                let strs = str.split(".");
                this.fileName = str;
                this.fileType = strs[1];
              }
              this.oppSendScenarioMessage =
                resStr($r('app.string.bluetoothtest_str_xyfsdwjwdbl')) + str + "\n"
            })
            .width(ConfigData.PCT_70)
            .borderRadius(ConfigData.HEIGHT_1_VP)
        }
          .height(ConfigData.PCT_20)

        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_lcrz'))
              .fontSize(ConfigData.VP_17)
              .margin({
                top: ConfigData.VP_8,
                bottom: ConfigData.VP_8,
                left: ConfigData.VP_20
              })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            List() {
              ListItem() {
                Text(this.oppSendScenarioMessage)
                  .fontSize(ConfigData.VP_18)
                  .margin({
                    top: ConfigData.VP_5,
                    left: ConfigData.VP_30,
                    right: ConfigData.VP_10
                  })
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
              .accessibilityLevel('yes')
              .id('pages_subScenarioTest_oppSendScenarioTest__li_1')
            }
            .height(ConfigData.WH_80_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.WH_30_100)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTest_oppSendScenarioTest__sc_2')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.On)
      }
      .height(ConfigData.PCT_66)
      .width(ConfigData.WH_100_100)
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));

      Column() {
        Flex({
          alignItems: ItemAlign.Center,
          justifyContent: FlexAlign.SpaceBetween
        }) {
          Column() {
            Button('oppSendFile')
              .accessibilityLevel('yes')
              .id('pages_subScenarioTest_oppSendScenarioTest__btn_0')
              .borderRadius(ConfigData.VP_8)
              .backgroundColor($r("app.color.blue"))
              .width(ConfigData.VP_200)
              .height(ConfigData.VP_60)
              .fontSize(ConfigData.VP_20)
              .margin({top: ConfigData.VP_15})
              .align(Alignment.Center)
              .alignSelf(ItemAlign.Center)
              .onClick(async(event: ClickEvent) => {
                try {
                  let fileHolders: Array<opp.FileHolder> = [];
                  this.oppProfile.sendFile(this.serverMac, fileHolders).then(() => {
                    let result: boolean = true;
                    if (result) {
                      this.oppSendScenarioMessage +=
                        resStr($r('app.string.bluetoothtest_str_ofswjcgr'))
                        + JSON.stringify(result)
                    } else {
                      this.oppSendScenarioMessage +=
                        resStr($r('app.string.bluetoothtest_str_ofswjsbr'))
                        + JSON.stringify(result)
                        + resStr($r('app.string.bluetoothtest_str_qhdwjmchwjljhzsbpdqk'))
                    }
                  })
              } catch(error) {
                  this.oppSendScenarioMessage += resStr($r('app.string.bluetoothtest_str_ofswjsbe'))
                    + JSON.stringify(error)
              }
                testLog(this.oppSendScenarioMessage);
                AppStorage.setOrCreate("oppSendScenarioMessage", this.oppSendScenarioMessage)
              })
          }
          .width(ConfigData.WH_100_100)
          .align(Alignment.Center)
          .alignItems(HorizontalAlign.Center)
          .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        }
        .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        .width(ConfigData.WH_100_100)
      }
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
      .width(ConfigData.WH_100_100)
      .height(ConfigData.WH_100_100);
    }
  }
}

export function testLog(logStr: string) {
  promptAction.showToast({message: logStr})
  LogUtil.info(logStr)
}