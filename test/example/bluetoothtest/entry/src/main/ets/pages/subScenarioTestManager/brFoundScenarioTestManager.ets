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
import { BTAutoArrayItem, testItemFromRouterOrPlaceholder} from '../../Utils/common'
import promptAction from '@ohos.promptAction';
import { PageTitle} from '../../component/pageTitle';
import { TestImageDisplay} from '../../component/testImageDisplay';
import { TestData} from '../../MainAbility/model/testData'
import { ScenarioContentTable} from '../../component/scenarioContentTable'
import {
  BrFoundManagerScenarioTestCase,
  initBRFoundManagerScenarioData
} from '../../MainAbility/model/scenarioTestDataModelManager'
import * as BrManagerInterface from '../../MainAbility/model/brManagerInterface'
import BluetoothModelManager from '../../MainAbility/model/BluetoothModelManager'

/**
 * Classic Bluetooth Found Scenario Manager Test Page Of Bluetooth test
 */

@Entry
@Component
struct BrFoundScenarioTestManager {
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @State changeIndex: number = - 1
  @StorageLink("brFoundScenarioMessage") brFoundScenarioMessage: string = ""

  build() {
    Column() {
      Stack({alignContent: Alignment.TopStart}) {
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
        .accessibilityLevel('yes')
        .id('pages_subScenarioTestManager_brFoundScenarioTestManager__pt_0')
      }

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column() {
        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_cjlcyz'))
              .fontSize(ConfigData.VP_17)
              .margin({
                top: ConfigData.VP_10,
                bottom: ConfigData.VP_10,
                left: ConfigData.VP_20
              })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            ScenarioContentTable({
              scenarioItems: initBRFoundManagerScenarioData(),
              changeIndex: this.changeIndex
            })
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_280_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTestManager_brFoundScenarioTestManager__sc_0')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)

        Stack()
          .height(ConfigData.HEIGHT_1_VP)
          .backgroundColor($r('app.color.black'));

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
                Text(this.brFoundScenarioMessage)
                  .fontSize(ConfigData.VP_17)
                  .margin({
                    top: ConfigData.VP_5,
                    left: ConfigData.VP_30,
                    right: ConfigData.VP_10
                  })
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
            .accessibilityLevel('yes')
            .id('pages_subScenarioTestManager_brFoundScenarioTestManager__li_0')
            }
            .height(ConfigData.WH_80_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_260_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTestManager_brFoundScenarioTestManager__sc_1')
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
        Flex({
          alignItems: ItemAlign.Center,
          justifyContent: FlexAlign.SpaceBetween
        }) {
          Button({
            type: ButtonType.Normal,
            stateEffect: true
          }) {
            Text($r('app.string.bluetoothtest_text_cjcsks'))
              .fontSize(ConfigData.VP_24)
              .fontColor(0xffffff)
              .margin({
                left: ConfigData.VP_5,
                right: ConfigData.VP_5
              })
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTestManager_brFoundScenarioTestManager__btn_0')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({
            top: ConfigData.VP_15,
            left: ConfigData.VP_20
          })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            try {
            promptAction.showToast({message:
              resStr($r('app.string.bluetoothtest_str_ksblyfxcjcs'))})
            this.forLoop()
          
            } catch (e) {
              LogUtil.info(`async onClick failed: ${e.message}, code: ${e.code}`);
            }})

          Button({
            type: ButtonType.Normal,
            stateEffect: true
          }) {
            Row() {
              Text($r('app.string.bluetoothtest_text_cjcsjs'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({
                  left: ConfigData.VP_5,
                  right: ConfigData.VP_5
                })
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTestManager_brFoundScenarioTestManager__btn_1')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({
            top: ConfigData.VP_15,
            left: ConfigData.VP_10,
            right: ConfigData.VP_10
          })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            try {
            let self = this
            //            const arr = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
            let brFoundScenarioMessage = ""
            // 停止蓝牙发现
            brFoundScenarioMessage += BrManagerInterface.testStopBluetoothDiscoveryManager() + "\n"
            LogUtil.info("testStopBluetoothDiscovery success")
            //            self.changeIndex = arr[8]
            await sleep(1)
            //关闭注册状态变化
            brFoundScenarioMessage += BrManagerInterface.testOnStateChangeManager() + "\n"
            LogUtil.info("testOffStateChange success")
            await sleep(1)
            //关蓝牙
            brFoundScenarioMessage += BrManagerInterface.testDisableBluetoothManager() + "\n"
            LogUtil.info("testDisableBluetooth success")
            //            self.changeIndex = arr[9]
            await sleep(1)
            AppStorage.setOrCreate("brFoundScenarioMessage", brFoundScenarioMessage)
          
            } catch (e) {
              LogUtil.info(`async onClick failed: ${e.message}, code: ${e.code}`);
            }})

          Button({
            type: ButtonType.Normal,
            stateEffect: true
          }) {
            Row() {
              Text($r('app.string.bluetoothtest_text_qkrz'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({
                  left: ConfigData.VP_5,
                  right: ConfigData.VP_5
                })
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTestManager_brFoundScenarioTestManager__btn_2')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.moon"))
          .width(ConfigData.VP_80)
          .height(ConfigData.VP_66)
          .margin({
            top: ConfigData.VP_15,
            right: ConfigData.VP_20
          })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            try {
            let brFoundScenarioMessage = ""
            AppStorage.setOrCreate("brFoundScenarioMessage", brFoundScenarioMessage)
          
            } catch (e) {
              LogUtil.info(`async onClick failed: ${e.message}, code: ${e.code}`);
            }})
        }
        .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        .width(ConfigData.WH_100_100)
      }
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
      .width(ConfigData.WH_100_100)
      .height(ConfigData.WH_100_100)
    }
  }

  subscribeBluetoothDeviceFind(): string {
    let message = resStr($r('app.string.bluetoothtest_str_st_1'))
    LogUtil.info("subscribeBluetoothDeviceFind")
    BluetoothModelManager.subscribeBluetoothDeviceFind((deviceIds: Array<string>) => {
      LogUtil.info('available bluetooth devices changed.')
      deviceIds?.forEach(async(deviceId) => {
        // let device = brFoundDevices.find((availableDevice) => {
        //   return availableDevice.deviceId === deviceId
        //  })
        LogUtil.info('available bluetooth find [' + deviceId + '] ')
        let message = (resStr($r('app.string.bluetoothtest_str_abf')) + deviceId +
          resStr($r('app.string.bluetoothtest_str_str')))
        promptAction.showToast({message: message})
        AppStorage.setOrCreate('bluetoothAvailableDevices', deviceIds)
      })
    });
    promptAction.showToast({message: message})
    return message
  }

  //循环调用
  async forLoop() {
    try {
    let brFoundScenarioMessage = ""
    //拼装数组
    let brScenarioArray: Array<BTAutoArrayItem> = [
      {
        name: "testSetLocalNameManager",
        para: ["DaYuBlue"]
      },
      {
        name: "testSetBluetoothScanModeManager",
        para: [4, 0]
      }
    ]
    let interfaces = BrFoundManagerScenarioTestCase.map(i => {
      let item = brScenarioArray.find(itm => itm.name === i.api.name);
      let para = item ? item.para: [];
      return { name: i.api.name, para} as BTAutoArrayItem;
    })

    for(let m = 0 ; m<interfaces.length ; m++) {
      // 检查 BrFoundManagerScenarioTestCase 中对应的 api 是否是一个函数
      if (typeof BrFoundManagerScenarioTestCase[m].api === "function") {
        // 使用 TypeScript 的类型断言确保 para 是一个数组
        brFoundScenarioMessage += BrFoundManagerScenarioTestCase[m].api(... interfaces[m].para)
          + "\n";
      } else {
        LogUtil.error(`在 BrAutoTestCase 中找不到方法: ${BrFoundManagerScenarioTestCase[m] }`);
      }
      this.changeIndex = m
      await sleep(1)
      AppStorage.setOrCreate("brFoundScenarioMessage", brFoundScenarioMessage)
    }
  
    } catch (e) {
      LogUtil.info(`forLoop failed: ${e.message}, code: ${e.code}`);
    }}
}

async function sleep(time: number): Promise<void> {
  return new Promise<void>((resolve, reject) => {
    setTimeout(() => {
      resolve();
    }, time * 1000);
  });
}