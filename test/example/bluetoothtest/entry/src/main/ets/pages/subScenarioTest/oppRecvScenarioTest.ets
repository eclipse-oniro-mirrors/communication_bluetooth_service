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
import { PageTitle} from '../../component/pageTitle';
import { TestImageDisplay} from '../../component/testImageDisplay';
import { TestData} from '../../MainAbility/model/testData'
import bluetooth from '@ohos.bluetooth';
import promptAction from '@ohos.promptAction';
import { opp} from '@kit.ConnectivityKit'
import { fileIo as fs} from '@kit.CoreFileKit';

/**
 * OppRecv Scenario Test Page Of Bluetooth test
 */

@Entry
@Component
struct OppRecvScenarioTest {
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @State changeIndex: number = - 1;
  @StorageLink("oppRecvScenarioMessage") oppRecvScenarioMessage: string = ""
  private oppProfile!: opp.OppServerProfile
  @State showProgress: boolean = false;
  @State curProgress: number = 0;
  @State serverMac: string = "00:00:00:00:00:00";
  private deviceName: string = "";
  private fileName: string = "";
  private pairedDevices: PairDevices[] = [];
  private receiveDialogController: CustomDialogController = new CustomDialogController({
    builder: ReceiveCustomDialog({
      deviceName: this.deviceName,
      fileName: this.fileName,
      confirm: () => {
        try {
          let pathDir = "/test.jpg";// Replace the example path with the actual one.
          let file = fs.openSync(pathDir, fs.OpenMode.CREATE | fs.OpenMode.READ_WRITE);
          this.oppProfile.setIncomingFileConfirmation(true, file.fd);
          this.receiveDialogController.close();
          this.onTransferStateChange();
          LogUtil.info("receive success");
        } catch(error) {
          LogUtil.info("receive fail, err: " + JSON.stringify(error));
        }
      },
      cancel: () => {
        try {
          let pathDir = "/test.jpg";// Replace the example path with the actual one.
          let file = fs.openSync(pathDir, fs.OpenMode.CREATE | fs.OpenMode.READ_WRITE);
          this.oppProfile.setIncomingFileConfirmation(false, file.fd);
          this.receiveDialogController.close();
          LogUtil.info("reject receive success");
        } catch(error) {
          LogUtil.info("reject receive fail, err: " + JSON.stringify(error));
        }
      }
    })
  })

  aboutToAppear() {
    try {
      this.oppProfile = opp.createOppServerProfile();
      testLog("oppRecv get opp profile success");
    } catch(error) {
      testLog("oppRecv get opp profile fail, err: " + JSON.stringify(error));
    }
    this.getPairDevice();
  }

  getPairDevice() {
    let macs = bluetooth.getPairedDevices();
    LogUtil.info("OppRecv getPairDevice macs: " + JSON.stringify(macs));
    for(let i = 0 ; i<macs.length ; i++) {
      let name = bluetooth.getRemoteDeviceName(macs[i]);
      LogUtil.info("OppRecv getPairDevice name: " + JSON.stringify(name));
      this.pairedDevices.push({
        id: macs[i],
        name: name
      });
    }
    LogUtil.info("OppRecv getPairDevice pairedDevices: " + JSON.stringify(this.pairedDevices));
  }

  onTransferStateChange() {
    try {
      this.oppProfile.on("transferStateChange", (stateInformation) => {
        LogUtil.info("transferStateChange enter");
        this.showProgress = true;
        this.curProgress = (stateInformation.currentBytes * 100) / stateInformation.totalBytes;
        if (this.curProgress === 100) {
          this.showProgress = false;
          this.oppRecvScenarioMessage =
            resStr($r('app.string.bluetoothtest_str_jswjcgwjcflj')) +
              JSON.stringify(stateInformation.filePath) + "\n"
          testLog(this.oppRecvScenarioMessage);
        }
      })
    } catch(error) {
      LogUtil.info("on transferStateChange fail, err: " + JSON.stringify(error));
    }
  }

  build() {
    Column() {
      Stack({alignContent: Alignment.TopStart}) {
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
        .accessibilityLevel('yes')
        .id('pages_subScenarioTest_oppRecvScenarioTest__pt_0')
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
                ForEach(this.pairedDevices, (item: PairDevices, index: number) => {
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
                  .id(index === 0 ? 'pages_subScenarioTest_oppRecvScenarioTest__li_0' :
                    `pages_subScenarioTest_oppRecvScenarioTest__li_${index}`)
                  .onClick(() => {
                    this.serverMac = item.id;
                  })
                })
              }
            }
            .accessibilityLevel('yes')
            .id('pages_subScenarioTest_oppRecvScenarioTest__sc_1')
            .height(ConfigData.PCT_70)
            .width(ConfigData.WH_80_100)
          }
          .height(ConfigData.WH_40_100)
          .width(ConfigData.WH_100_100)

        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTest_oppRecvScenarioTest__sc_0')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.On)

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
          .height(ConfigData.PCT_20)

        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_lcrz'))
              .fontSize(ConfigData.VP_18)
              .margin({
                top: ConfigData.VP_8,
                bottom: ConfigData.VP_8,
                left: ConfigData.VP_20
              })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            List() {
              ListItem() {
                Text(this.oppRecvScenarioMessage)
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
            .id('pages_subScenarioTest_oppRecvScenarioTest__li_1')
            }
            .height(ConfigData.WH_80_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.WH_30_100)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTest_oppRecvScenarioTest__sc_2')
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
            Button($r('app.string.bluetoothtest_str_okqjswj'))
              .accessibilityLevel('yes')
              .id('pages_subScenarioTest_oppRecvScenarioTest__btn_0')
              .fontSize(ConfigData.VP_20)
              .borderRadius(ConfigData.VP_8)
              .backgroundColor($r("app.color.blue"))
              .width(ConfigData.VP_200)
              .height(ConfigData.VP_60)
              .margin({top: ConfigData.VP_15})
              .align(Alignment.Center)
              .alignSelf(ItemAlign.Center)
              .onClick(() => {
                try {
                  this.oppProfile.on("receiveIncomingFile", (information) => {
                    testLog(resStr($r('app.string.bluetoothtest_str_okqjswj')))
                    this.deviceName = information.remoteDeviceName;
                    this.fileName = information.filePath;
                    this.oppRecvScenarioMessage =
                      resStr($r('app.string.bluetoothtest_str_fsdw')) + this.deviceName +
                        resStr($r('app.string.bluetoothtest_str_wjmw')) + this.fileName + "\n"
                    testLog(this.oppRecvScenarioMessage);
                    LogUtil.info(JSON.stringify(information));
                    this.receiveDialogController.open();
                  })
              } catch(error) {
                  this.oppRecvScenarioMessage =
                    resStr($r('app.string.bluetoothtest_str_okqjswjsbe')) + JSON.stringify(error)
                  testLog(this.oppRecvScenarioMessage);
              }
              })
              .width(ConfigData.PCT_60)
          }
          .width(ConfigData.WH_100_100)
          .align(Alignment.Center)
          .alignItems(HorizontalAlign.Center)
          .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

          if (this.showProgress) {
            Row({space: ConfigData.VP_5}) {
              Progress({
                value: this.curProgress,
                total: 100,
                type: ProgressType.Linear
              })
                .style({strokeWidth: ConfigData.VP_10})

              Text(this.curProgress.toString() +
                getContext(this).resourceManager.getStringSync($r('app.string.percent_symbol')))
                .fontSize(ConfigData.VP_30)
            }
          }
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

@CustomDialog
struct ReceiveCustomDialog {
  controller: CustomDialogController
  deviceName: string = 'defaultDeviceName'
  fileName: string = 'defaultFileName'
  cancel!: () => void
  confirm!: () => void

  build() {
    Column() {
      Text(resStr($r('app.string.opp_recv_confirm_prefix')) + this.deviceName +
        resStr($r('app.string.opp_recv_confirm_suffix')))
        .fontSize(ConfigData.VP_20)
        .margin({
          top: ConfigData.VP_10,
          bottom: ConfigData.VP_10
        })
      Text(this.fileName)
    .fontSize(ConfigData.VP_18)
    .margin({bottom: ConfigData.VP_10})
      Flex({justifyContent: FlexAlign.SpaceAround}) {
        Button($r('app.string.bluetoothtest_str_jj'))
          .accessibilityLevel('yes')
          .id('pages_subScenarioTest_oppRecvScenarioTest__btn_1')
          .onClick(() => {
            this.cancel()
          })
        Button($r('app.string.bluetoothtest_str_js_1'))
          .accessibilityLevel('yes')
          .id('pages_subScenarioTest_oppRecvScenarioTest__btn_2')
          .onClick(() => {
            this.confirm()
          })
      }
        .margin({bottom: ConfigData.VP_10})
    }
  }
}

export function testLog(logStr: string) {
  promptAction.showToast({message: logStr})
  LogUtil.info(logStr)
}