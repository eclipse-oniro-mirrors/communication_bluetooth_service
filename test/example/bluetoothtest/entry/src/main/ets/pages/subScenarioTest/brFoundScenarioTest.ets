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
import LogUtil from '../../Utils/LogUtil';
import { resStr} from '../../Utils/resString';


import ConfigData from '../../Utils/ConfigData';
import { PageTitle} from '../../component/pageTitle';
import { TestImageDisplay} from '../../component/testImageDisplay';
import { TestData} from '../../MainAbility/model/testData'
import { ScenarioContentTable} from '../../component/scenarioContentTable'
import { BrFoundScenarioTestCase, initBRFoundScenarioData} from '../../MainAbility/model/scenarioTestDataModel'
import * as BrInterface from '../../MainAbility/model/brInterface'
import BluetoothModel from '../../MainAbility/model/BluetoothModel'
import promptAction from '@ohos.promptAction';
import { BTAutoArrayItem, testItemFromRouterOrPlaceholder} from '../../Utils/common'

/**
 * Classic Bluetooth Found Scenario Test Page Of Bluetooth test
 */

@Entry
@Component
struct BrFoundScenarioTest {
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @State changeIndex: number = - 1
  @StorageLink("brFoundScenarioMessage") brFoundScenarioMessage: string = ""

  build(){
    Column(){
      Stack({alignContent: Alignment.TopStart}){
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
              .accessibilityLevel('yes')
              .id('pages_subScenarioTest_brFoundScenarioTest__pt_0')
      }

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column(){
        Scroll(){
          Column(){
            Text($r('app.string.bluetoothtest_text_cjlcyz'))
              .fontSize(ConfigData.VP_17)
              .margin({top: ConfigData.VP_10, bottom: ConfigData.VP_10, left: ConfigData.VP_20})
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            ScenarioContentTable({
              scenarioItems: initBRFoundScenarioData(),
              changeIndex: this.changeIndex
            })
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_280_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTest_brFoundScenarioTest__sc_0')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)

        Stack()
          .height(ConfigData.HEIGHT_1_VP)
          .backgroundColor($r('app.color.black'));

        Scroll(){
          Column(){
            Text($r('app.string.bluetoothtest_text_lcrz'))
              .fontSize(ConfigData.VP_17)
              .margin({top: ConfigData.VP_8, bottom: ConfigData.VP_8, left: ConfigData.VP_20})
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            List(){
              ListItem(){
                Text(this.brFoundScenarioMessage)
                  .fontSize(ConfigData.VP_17)
                  .margin({top: ConfigData.VP_5, left: ConfigData.VP_30, right: ConfigData.VP_10})
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
            .accessibilityLevel('yes')
            .id('pages_subScenarioTest_brFoundScenarioTest__li_0')
            }
            .height(ConfigData.WH_80_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_260_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTest_brFoundScenarioTest__sc_1')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.On)
      }
      .height(ConfigData.HEIGHT_500_VP)
      .width(ConfigData.WH_100_100)
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))


      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column(){
        Flex({alignItems: ItemAlign.Center, justifyContent: FlexAlign.SpaceBetween}){
          Button({type: ButtonType.Normal, stateEffect: true}){
            Text($r('app.string.bluetoothtest_text_cjcsks'))
              .fontSize(ConfigData.VP_24)
              .fontColor(0xffffff)
              .margin({left: ConfigData.VP_5, right: ConfigData.VP_5})
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTest_brFoundScenarioTest__btn_0')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({top: ConfigData.VP_15, left: ConfigData.VP_20})
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            promptAction.showToast({message: resStr($r('app.string.bluetoothtest_str_ksblyfxcjcs'))})
            this.forLoop()
          })

          Button({type: ButtonType.Normal, stateEffect: true}){
            Row(){
              Text($r('app.string.bluetoothtest_text_cjcsjs'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({left: ConfigData.VP_5, right: ConfigData.VP_5})
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTest_brFoundScenarioTest__btn_1')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({top: ConfigData.VP_15, left: ConfigData.VP_10, right: ConfigData.VP_10})
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let brFoundScenarioMessage = ""
            // 停止蓝牙发现
            brFoundScenarioMessage += BrInterface.testStopBluetoothDiscovery() + "\n"
            LogUtil.info("testStopBluetoothDiscovery success")
            //            self.changeIndex = arr[8]
            await sleep(1)
            //关闭注册状态变化
            brFoundScenarioMessage += BrInterface.testOnStateChange() + "\n"
            LogUtil.info("testOffStateChange success")
            await sleep(1)
            //关蓝牙
            brFoundScenarioMessage += BrInterface.testDisableBluetooth() + "\n"
            LogUtil.info("testDisableBluetooth success")
            //            self.changeIndex = arr[9]
            await sleep(1)
            AppStorage.setOrCreate("brFoundScenarioMessage", brFoundScenarioMessage)
          })

          Button({type: ButtonType.Normal, stateEffect: true}){
            Row(){
              Text($r('app.string.bluetoothtest_text_qkrz'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({left: ConfigData.VP_5, right: ConfigData.VP_5})
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTest_brFoundScenarioTest__btn_2')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.moon"))
          .width(ConfigData.VP_80)
          .height(ConfigData.VP_66)
          .margin({top: ConfigData.VP_15, right: ConfigData.VP_20})
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let brFoundScenarioMessage = ""
            AppStorage.setOrCreate("brFoundScenarioMessage", brFoundScenarioMessage)
          })
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
    BluetoothModel.subscribeBluetoothDeviceFind((deviceIds: Array<string>) => {
      LogUtil.info('available bluetooth devices changed.')
      deviceIds?.forEach(deviceId => {
        // let device = brFoundDevices.find((availableDevice) => {
        //   return availableDevice.deviceId === deviceId
        // })
        LogUtil.info('available bluetooth find [' + deviceId + '] ')
        let message = (resStr($r('app.string.bluetoothtest_str_abf')) + deviceId + resStr($r('app.string.bluetoothtest_str_str')))
        promptAction.showToast({message: message})
        AppStorage.setOrCreate('bluetoothAvailableDevices', deviceIds)
      })
    });
    promptAction.showToast({message: message})
    return message
  }

  //循环调用
  async forLoop(){
    let brFoundScenarioMessage = ""
    //拼装数组
    let brScenarioArray: Array<BTAutoArrayItem> = [
      { name: "testSetLocalName", para: [ "DaYuBlue" ]},
      { name: "testSetBluetoothScanMode", para: [ 4, 0 ]}
    ]
    let interfaces = BrFoundScenarioTestCase.map(i => {
      let item = brScenarioArray.find(itm => itm.name === i.api.name);
      let para = item ? item.para: [];
      return { name: i.api.name, para} as BTAutoArrayItem
    })
    for(let m = 0 ; m<interfaces.length ; m++ ){
      // 检查 BrFoundScenarioTestCase 中对应的 api 是否是一个函数
      if ( typeof BrFoundScenarioTestCase[ m ].api === "function" ){
        // 使用 TypeScript 的类型断言确保 para 是一个数组
        brFoundScenarioMessage += BrFoundScenarioTestCase[ m ].api(... interfaces[ m ].para) + "\n";
      } else {
        hilog.error(0x0000, 'BtTest', '%{public}s', `在 BrAutoTestCase 中找不到方法: ${ BrFoundScenarioTestCase[ m ] }`);
      }
      this.changeIndex = m
      await sleep(1)
      AppStorage.setOrCreate("brFoundScenarioMessage", brFoundScenarioMessage)
    }
  }
}

async function sleep(time: number): Promise<void> {
  return new Promise<void>((resolve, reject) => {
    setTimeout(() => {
      resolve();
    }, time * 1000);
  });
}