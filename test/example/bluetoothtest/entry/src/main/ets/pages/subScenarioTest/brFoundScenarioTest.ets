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

import router from '@ohos.router';
import { PageTitle } from '../../Component/pageTitle';
import { TestImageDisplay } from '../../Component/testImageDisplay';
import { TestData } from '../../MainAbility/model/testData'
import { ScenarioContentTable } from '../../Component/scenarioContentTable'
import { BrFoundScenarioTestCase , initBRFoundScenarioData } from '../../MainAbility/model/scenarioTestDataModel'
import * as BrInterface from '../../MainAbility/model/brInterface'
import BluetoothModel from '../../MainAbility/model/BluetoothModel'
import promptAction from '@ohos.promptAction';
import { BTAutoArrayItem , myParams } from '../../Utils/common'

/**
 * Classic Bluetooth Found Scenario Test Page Of Bluetooth test
 */

@Entry
@Component
struct BrFoundScenarioTest {
  private testItem: TestData = (router.getParams() as myParams).testItem
  @State changeIndex: number = - 1
  @StorageLink("brFoundScenarioMessage") brFoundScenarioMessage: string = ""

  build() {
    Column() {
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
      }

      Stack().height($r('app.float.wh_value_1')).backgroundColor("#000000");
      Column() {
        Scroll() {
          Column() {
            Text("场景流程验证:")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_10') , bottom : $r('app.float.wh_value_10') , left : $r('app.float.wh_value_20') })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            ScenarioContentTable({
              scenarioItems : initBRFoundScenarioData() ,
              changeIndex : this.changeIndex
            })
          }
          .width($r('app.float.wh_value_100p'))
          .height($r('app.float.wh_value_280'))
        }
        .scrollBarWidth(10)
        .scrollBar(BarState.Auto)

        Stack().height($r('app.float.wh_value_1')).backgroundColor("#000000");

        Scroll() {
          Column() {
            Text("流程日志:")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_8') , bottom : $r('app.float.wh_value_8') , left : $r('app.float.wh_value_20') })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            List() {
              ListItem() {
                Text(this.brFoundScenarioMessage)
                  .fontSize($r('app.float.wh_value_17'))
                  .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_30') , right : $r('app.float.wh_value_10') })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
              }
            }
            .height($r('app.float.wh_value_80p'))
            .width($r('app.float.wh_value_90p'))
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width($r('app.float.wh_value_100p'))
          .height($r('app.float.wh_value_260'))
        }
        .scrollBarWidth(10)
        .scrollBar(BarState.On)
      }
      .height($r('app.float.wh_value_500'))
      .width($r('app.float.wh_value_100p'))
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))


      Stack().height($r('app.float.wh_value_1')).backgroundColor("#000000");
      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Text('场景测试开始').fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : 5 , right : 5 })
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.blue"))
          .width($r('app.float.wh_value_100'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , left : $r('app.float.wh_value_20') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            promptAction.showToast({ message : '开始Br蓝牙发现场景测试' })
            this.forLoop()
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('场景测试结束').fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : 5 , right : 5 })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.blue"))
          .width($r('app.float.wh_value_100'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , left : $r('app.float.wh_value_10') , right : $r('app.float.wh_value_10') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let brFoundScenarioMessage = ""
            // 停止蓝牙发现
            brFoundScenarioMessage += BrInterface.testStopBluetoothDiscovery() + "\n"
            console.log("testStopBluetoothDiscovery success")
            //            self.changeIndex = arr[8]
            await sleep(1)
            //关闭注册状态变化
            brFoundScenarioMessage += BrInterface.testOnStateChange() + "\n"
            console.log("testOffStateChange success")
            await sleep(1)
            //关蓝牙
            brFoundScenarioMessage += BrInterface.testDisableBluetooth() + "\n"
            console.log("testDisableBluetooth success")
            //            self.changeIndex = arr[9]
            await sleep(1)
            AppStorage.setOrCreate("brFoundScenarioMessage" , brFoundScenarioMessage)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('清空日志').fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : 5 , right : 5 })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.moon"))
          .width($r('app.float.wh_value_80'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , right : $r('app.float.wh_value_20') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let brFoundScenarioMessage = ""
            AppStorage.setOrCreate("brFoundScenarioMessage" , brFoundScenarioMessage)
          })
        }
        .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        .width($r('app.float.wh_value_100p'))
      }
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
      .width($r('app.float.wh_value_100p'))
      .height($r('app.float.wh_value_100p'));
    }
  }

  subscribeBluetoothDeviceFind(): string {
    let message = "subscribeBluetoothDeviceFind test"
    console.log("subscribeBluetoothDeviceFind")
    BluetoothModel.subscribeBluetoothDeviceFind((deviceIds: Array<string>) => {
      console.log('available bluetooth devices changed.');
      deviceIds?.forEach(deviceId => {
        // let device = brFoundDevices.find((availableDevice) => {
        //   return availableDevice.deviceId === deviceId
        // })
        console.log('available bluetooth find [' + deviceId + '] ')
        let message = ('available bluetooth find [' + deviceId + ']  ')
        promptAction.showToast({ message : message })
        AppStorage.setOrCreate('bluetoothAvailableDevices' , deviceIds);
      })
    });
    promptAction.showToast({ message : message })
    return message
  }

  //循环调用
  async forLoop() {
    let brFoundScenarioMessage = ""
    //拼装数组
    let brScenarioArray: Array<BTAutoArrayItem> = [
      { name : "testSetLocalName" , para : [ "DaYuBlue" ] },
      { name : "testSetBluetoothScanMode" , para : [ 4, 0 ] }
    ]
    let interfaces = BrFoundScenarioTestCase.map(i => {
      let item = brScenarioArray.find(itm => itm.name === i.api.name);
      let para = item ? item.para : [];
      return { name : i.api.name , para } as BTAutoArrayItem
    })
    for ( let m = 0 ; m < interfaces.length ; m ++ ) {
      // 检查 BrFoundScenarioTestCase 中对应的 api 是否是一个函数
      if ( typeof BrFoundScenarioTestCase[ m ].api === "function" ) {
        // 使用 TypeScript 的类型断言确保 para 是一个数组
        brFoundScenarioMessage += BrFoundScenarioTestCase[ m ].api(... interfaces[ m ].para) + "\n";
      } else {
        console.error(`在 BrAutoTestCase 中找不到方法: ${ BrFoundScenarioTestCase[ m ] }`);
      }
      this.changeIndex = m
      await sleep(1)
      AppStorage.setOrCreate("brFoundScenarioMessage" , brFoundScenarioMessage)
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