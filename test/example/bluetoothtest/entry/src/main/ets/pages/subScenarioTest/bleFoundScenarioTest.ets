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
import ConfigData from '../../Utils/ConfigData';
import { PageTitle } from '../../Component/pageTitle';
import { TestImageDisplay } from '../../Component/testImageDisplay';
import { TestData } from '../../MainAbility/model/testData'
import { ScenarioContentTable } from '../../Component/scenarioContentTable'
import { BleFoundScenarioTestCase , initBleFoundScenarioData } from '../../MainAbility/model/scenarioTestDataModel'
import * as BLEInterface from '../../MainAbility/model/bleInterface'
import * as BrInterface from '../../MainAbility/model/brInterface'
import { BTAutoArrayItem, testItemFromRouterOrPlaceholder } from '../../Utils/common'

/**
 * BLE found Scenario Test Page Of Bluetooth test
 */

@Entry
@Component
struct BleFoundScenarioTest {
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @State changeIndex: number = - 1
  @StorageLink("bleFoundScenarioMessage") bleFoundScenarioMessage: string = ""

  aboutToAppear() {
    AppStorage.setOrCreate('txtScanFilterDeviceId' , "08:FB:EA:1B:3C:63"); // 6c:96:d7:3d:87:6f
    AppStorage.setOrCreate('txtScanFilterName' , "dudu-tiger");
    AppStorage.setOrCreate('txtScanFilterServiceUuid' , "0000180A-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('txtScanOptions_interval' , "0");
    AppStorage.setOrCreate('txtScanOptionsDutyMode' , '0');
    AppStorage.setOrCreate('txtScanOptionsMatchMode' , '0');
    AppStorage.setOrCreate('cbxBleScanFilter' , true);
    AppStorage.setOrCreate('cbxBleScanOptions' , true);
  }

  build() {
    Column() {
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
              .accessibilityLevel('yes')
              .id('pages_subScenarioTest_bleFoundScenarioTest__pt_0')
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
              scenarioItems : initBleFoundScenarioData() ,
              changeIndex : this.changeIndex
            })
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_280_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTest_bleFoundScenarioTest__sc_0')
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
                Text(this.bleFoundScenarioMessage)
                  .fontSize(ConfigData.VP_17)
                  .margin({ top : ConfigData.VP_5 , left : ConfigData.VP_30 , right : ConfigData.VP_10 })
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
            .accessibilityLevel('yes')
            .id('pages_subScenarioTest_bleFoundScenarioTest__li_0')
            }
            .height(ConfigData.WH_80_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_260_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTest_bleFoundScenarioTest__sc_1')
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
          .id('pages_subScenarioTest_bleFoundScenarioTest__btn_0')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , left : ConfigData.VP_20 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            this.forLoop()
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
          .id('pages_subScenarioTest_bleFoundScenarioTest__btn_1')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , left : ConfigData.VP_10 , right : ConfigData.VP_10 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let bleFoundScenarioMessage = ""
            // 停止BLE扫描
            bleFoundScenarioMessage += BLEInterface.testStopBLEScan() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testStopBLEScan success")
            //            self.changeIndex = arr[6]
            await sleep(1)
            // 关闭注册BLE查找状态变化
            bleFoundScenarioMessage += BLEInterface.testOnBLEDeviceFind() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testOffBLEDeviceFind success")
            await sleep(1)
            //关闭注册状态变化
            bleFoundScenarioMessage += BrInterface.testOnStateChange() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testOffStateChange success")
            await sleep(1)
            //关蓝牙
            bleFoundScenarioMessage += BrInterface.testDisableBluetooth() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testDisableBluetooth success")
            //            self.changeIndex = arr[7]
            await sleep(1)
            AppStorage.setOrCreate("bleFoundScenarioMessage" , bleFoundScenarioMessage)
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
          .id('pages_subScenarioTest_bleFoundScenarioTest__btn_2')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.moon"))
          .width(ConfigData.VP_80)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , right : ConfigData.VP_20 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let bleFoundScenarioMessage = ""
            AppStorage.setOrCreate("bleFoundScenarioMessage" , bleFoundScenarioMessage)
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

  //循环调用
  async forLoop() {
    let bleFoundScenarioMessage = ""
    //拼装数组
    let bleScenarioArray: Array<BTAutoArrayItem> = [
      { name : "testSetLocalName" , para : [ "DaYuBlue" ] },
      { name : "testSetBluetoothScanMode" , para : [ 4, 0 ] }
    ]
    let interfaces = BleFoundScenarioTestCase.map(i => {
      let item = bleScenarioArray.find(itm => itm.name === i.api.name);
      let para = item ? item.para : []; // 确保 para 总是一个数组
      return { name : i.api.name , para } as BTAutoArrayItem;
    });

    for ( let m = 0 ; m < interfaces.length ; m ++ ) {
      // 检查 BleFoundScenarioTestCase 中对应的 api 是否是一个函数
      if ( typeof BleFoundScenarioTestCase[ m ].api === "function" ) {
        // 使用 TypeScript 的类型断言确保 para 是一个数组
        bleFoundScenarioMessage += BleFoundScenarioTestCase[ m ].api(... interfaces[ m ].para) + "\n";
      } else {
        console.error(`在 BrAutoTestCase 中找不到方法: ${ BleFoundScenarioTestCase[ m ] }`);
      }
      this.changeIndex = m
      await sleep(1)
      AppStorage.setOrCreate("bleFoundScenarioMessage" , bleFoundScenarioMessage)
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