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
import { resStr } from '../../Utils/resString';


import hilog from '@ohos.hilog';
import ConfigData from '../../Utils/ConfigData';
import { testItemFromRouterOrPlaceholder } from '../../Utils/common'
import { PageTitle } from '../../Component/pageTitle';
import { TestImageDisplay } from '../../Component/testImageDisplay';
import { TestData } from '../../MainAbility/model/testData'
import { ScenarioContentTable } from '../../Component/scenarioContentTable'
import { initSppClientScenarioData } from '../../MainAbility/model/scenarioTestDataModel'
import * as BrInterface from '../../MainAbility/model/brInterface'
import * as SPPClientInterface from '../../MainAbility/model/sppClientInterface'
import bluetooth from '@ohos.bluetooth';
import promptAction from '@ohos.promptAction';

/**
 * SppClient Scenario Test Page Of Bluetooth test
 */

@Entry
@Component
struct SppClientScenarioTest {
  @State changeIndex: number = - 1;
  @State writeClientData: string = '11';
  private uuid: string = "00001101-0000-1000-8000-00805f9b34fb";
  @State deviceId: string = "08:FB:EA:1B:3C:63"; //08:FB:EA:1B:3C:63
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @StorageLink("sppClientScenarioMessage") sppClientScenarioMessage: string = ""
  @StorageLink('dataReadMessage') dataReadMessage: string = "";

  aboutToAppear() {

    AppStorage.setOrCreate('txtScanFilterDeviceId' , "08:FB:EA:1B:3C:63");
    AppStorage.setOrCreate('txtScanFilterName' , "dudu-tiger");
    AppStorage.setOrCreate('txtScanFilterServiceUuid' , "0000180A-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('txtScanOptions_interval' , "0");
    AppStorage.setOrCreate('txtScanOptionsDutyMode' , '0');
    AppStorage.setOrCreate('txtScanOptionsMatchMode' , '0');
    AppStorage.setOrCreate('cbxBleScanFilter' , true);
    AppStorage.setOrCreate('cbxBleScanOptions' , true);

    AppStorage.setOrCreate('inputDeviceId' , this.deviceId);
    AppStorage.setOrCreate('inputUuid' , this.uuid);
    AppStorage.setOrCreate('writeClientData' , this.writeClientData);
  }

  build() {
    Column() {
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
              .accessibilityLevel('yes')
              .id('pages_subScenarioTest_sppClientScenarioTest__pt_0')
      }

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column() {
        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_cjlcyz'))
              .fontSize(ConfigData.VP_17)
              .margin({ top : ConfigData.VP_10 , bottom : ConfigData.VP_10 , left : ConfigData.VP_20 })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            ScenarioContentTable({
              scenarioItems : initSppClientScenarioData() ,
              changeIndex : this.changeIndex
            })
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_280_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTest_sppClientScenarioTest__sc_0')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)

        Stack()
          .height(ConfigData.HEIGHT_1_VP)
          .backgroundColor($r('app.color.black'));

        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_lcrz'))
              .fontSize(ConfigData.VP_17)
              .margin({ top : ConfigData.VP_8 , bottom : ConfigData.VP_8 , left : ConfigData.VP_20 })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            List() {
              ListItem() {
                Text(this.sppClientScenarioMessage)
                  .fontSize(ConfigData.VP_17)
                  .margin({ top : ConfigData.VP_5 , left : ConfigData.VP_30 , right : ConfigData.VP_10 })
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
            .accessibilityLevel('yes')
            .id('pages_subScenarioTest_sppClientScenarioTest__li_0')
            }
            .height(ConfigData.WH_80_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_260_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTest_sppClientScenarioTest__sc_1')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.On)
      }
      .height(ConfigData.HEIGHT_500_VP)
      .width(ConfigData.WH_100_100)
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))


      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Text($r('app.string.bluetoothtest_text_cjcsks'))
              .fontSize(ConfigData.VP_24)
              .fontColor(0xffffff)
              .margin({ left : ConfigData.VP_5 , right : ConfigData.VP_5 })
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTest_sppClientScenarioTest__btn_0')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , left : ConfigData.VP_20 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let self = this
            const arr = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 ]
            let sppClientScenarioMessage = ""
            // 注册状态变化
            sppClientScenarioMessage += BrInterface.testOnStateChange() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testOnStateChange success")
            self.changeIndex = arr[ 0 ]
            await sleep(1)

            // 开蓝牙
            sppClientScenarioMessage += BrInterface.testEnableBluetooth() + "\n";
            hilog.info(0x0000, 'BtTest', '%{public}s', "testEnableBluetooth success")
            self.changeIndex = arr[ 1 ]
            await sleep(2)
            AppStorage.setOrCreate("sppClientScenarioMessage" , sppClientScenarioMessage)

            // 获取状态
            sppClientScenarioMessage += BrInterface.testGetState() + "\n";
            hilog.info(0x0000, 'BtTest', '%{public}s', "testGetState success")
            self.changeIndex = arr[ 2 ]
            await sleep(1)

            // 设置扫描模式
            sppClientScenarioMessage += BrInterface.testSetBluetoothScanMode(4 , 0) + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testSetBluetoothScanMode success")
            self.changeIndex = arr[ 3 ]
            await sleep(1)

            // 获取扫描模式
            sppClientScenarioMessage += BrInterface.testGetBluetoothScanMode() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testGetBluetoothScanMode success")
            self.changeIndex = arr[ 4 ]
            await sleep(1)

            // Spp客户端连接
            sppClientScenarioMessage += SPPClientInterface.testSppClientConnectSafe() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testSppClientConnectSafe success")
            self.changeIndex = arr[ 5 ]
            AppStorage.setOrCreate("sppClientScenarioMessage" , sppClientScenarioMessage)
            await sleep(10)

            //获取当前连接状态
            sppClientScenarioMessage += BrInterface.testGetBtConnectionState() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testGetBtConnectionState success")
            self.changeIndex = arr[ 6 ]
            AppStorage.setOrCreate("sppClientScenarioMessage" , sppClientScenarioMessage)
            await sleep(2)

            //spp客户端读取打开
            sppClientScenarioMessage += SPPClientInterface.testSppClientReadOn() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testSppClientReadOn success")
            self.changeIndex = arr[ 7 ]
            sppClientScenarioMessage += AppStorage.get("dataReadMessage") + "\n"
            AppStorage.setOrCreate("sppClientScenarioMessage" , sppClientScenarioMessage)
            await sleep(5)

            // SPP客户端写入
            sppClientScenarioMessage += SPPClientInterface.testSppClientWrite() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testSppClientWrite success")
            self.changeIndex = arr[ 8 ]
            AppStorage.setOrCreate("sppClientScenarioMessage" , sppClientScenarioMessage)
            await sleep(2)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text($r('app.string.bluetoothtest_text_cjcsjs'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({ left : ConfigData.VP_5 , right : ConfigData.VP_5 })
            }
              .alignItems(VerticalAlign.Top)
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTest_sppClientScenarioTest__btn_1')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , left : ConfigData.VP_10 , right : ConfigData.VP_10 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let self = this
            const arr = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 ]
            let sppClientScenarioMessage = ""
            // spp客户端读取关闭
            sppClientScenarioMessage += SPPClientInterface.testSppClientReadOff() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testSppClientReadOff success")
            self.changeIndex = arr[ 9 ]
            await sleep(1)

            // spp客户端关闭
            sppClientScenarioMessage += SPPClientInterface.testSppClientClose() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testSppClientClose success")
            self.changeIndex = arr[ 10 ]
            await sleep(1)

            //关闭注册状态变化
            sppClientScenarioMessage += BrInterface.testOnStateChange() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testOffStateChange success")
            await sleep(1)

            //关蓝牙
            sppClientScenarioMessage += BrInterface.testDisableBluetooth() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testDisableBluetooth success")
            self.changeIndex = arr[ 11 ]
            await sleep(1)
            AppStorage.setOrCreate("sppClientScenarioMessage" , sppClientScenarioMessage)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text($r('app.string.bluetoothtest_text_qkrz'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({ left : ConfigData.VP_5 , right : ConfigData.VP_5 })
            }
              .alignItems(VerticalAlign.Top)
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTest_sppClientScenarioTest__btn_2')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.moon"))
          .width(ConfigData.VP_80)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , right : ConfigData.VP_20 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppClientScenarioMessage = ""
            AppStorage.setOrCreate("sppClientScenarioMessage" , sppClientScenarioMessage)
          })
        }
        .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        .width(ConfigData.WH_100_100)
      }
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
      .width(ConfigData.WH_100_100)
      .height(ConfigData.WH_100_100);
    }
  }

  dataRead(dataBuffer: ArrayBuffer) {
    const data = new Uint8Array(dataBuffer);
    hilog.info(0x0000, 'BtTest', '%{public}s', 'spp bluetooth read is: ' + data);
    if ( data != null ) {
      this.dataReadMessage = resStr($r('app.string.bluetoothtest_str_dqcg_2')) + JSON.stringify(data);
    } else {
      this.dataReadMessage = resStr($r('app.string.bluetoothtest_str_dqsb'));
    }
  }
}

async function sleep(time: number): Promise<void> {
  return new Promise<void>((resolve , reject) => {
    setTimeout(() => {
      resolve();
    } , time * 1000);
  });
}

let clientNumber = - 1

export function acceptClientSocket(number: number) {
  hilog.info(0x0000, 'BtTest', '%{public}s', 'spp clientSocket socketID: ' + number);
  // 获取的clientNumber用作服务端后续读/写操作socket的id。
  clientNumber = number;
}

export function sppClientClose() {
  hilog.info(0x0000, 'BtTest', '%{public}s', "bluetoothModel:" + 'sppCloseClientSocket clientNumber: ' + JSON.stringify(clientNumber))
  bluetooth.sppCloseClientSocket(clientNumber);
  let message = resStr($r('app.string.bluetoothtest_str_gbcgc')) + clientNumber;
  promptAction.showToast({ message : message })
  return message
}

export function strToArrayBuffer(str: string) {
  let array = new Uint8Array(str.length);
  for ( let i = 0 ; i < str.length ; i ++ ) {
    array[ i ] = str.charCodeAt(i);
  }
  return array.buffer
}