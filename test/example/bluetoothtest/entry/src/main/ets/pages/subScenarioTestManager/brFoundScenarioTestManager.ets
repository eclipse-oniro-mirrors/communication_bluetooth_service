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

import { BTAutoArrayItem , myParams } from '../../Utils/common'
import promptAction from '@ohos.promptAction';
import router from '@ohos.router';
import { PageTitle } from '../../Component/pageTitle';
import { TestImageDisplay } from '../../Component/testImageDisplay';
import { TestData } from '../../MainAbility/model/testData'
import { ScenarioContentTable } from '../../Component/scenarioContentTable'
import {
  BrFoundManagerScenarioTestCase ,
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
  private testItem: TestData = (router.getParams() as myParams).testItem
  @State changeIndex: number = - 1
  @StorageLink("brFoundScenarioMessage") brFoundScenarioMessage: string = ""

  build() {
    Column() {
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
      }

      Stack().height('1vp').backgroundColor($r('app.color.black'));
      Column() {
        Scroll() {
          Column() {
            Text("场景流程验证:")
              .fontSize('17vp')
              .margin({ top : '10vp' , bottom : '10vp' , left : '20vp' })
              .textAlign(TextAlign.Start)
              .width('100%')
            ScenarioContentTable({
              scenarioItems : initBRFoundManagerScenarioData() ,
              changeIndex : this.changeIndex
            })
          }
          .width('100%')
          .height('280vp')
        }
        .scrollBarWidth('10vp')
        .scrollBar(BarState.Auto)

        Stack().height('1vp').backgroundColor($r('app.color.black'));

        Scroll() {
          Column() {
            Text("流程日志:")
              .fontSize('17vp')
              .margin({ top : '8vp' , bottom : '8vp' , left : '20vp' })
              .textAlign(TextAlign.Start)
              .width('100%')
            List() {
              ListItem() {
                Text(this.brFoundScenarioMessage)
                  .fontSize('17vp')
                  .margin({ top : '5vp' , left : '30vp' , right : '10vp' })
                  .textAlign(TextAlign.Start)
                  .width('90%')
              }
            }
            .height('80%')
            .width('90%')
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width('100%')
          .height('260vp')
        }
        .scrollBarWidth('10vp')
        .scrollBar(BarState.On)
      }
      .height('500vp')
      .width('100%')
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))


      Stack().height('1vp').backgroundColor($r('app.color.black'));
      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Text('场景测试开始').fontSize('24vp').fontColor(0xffffff).margin({ left : '5vp' , right : '5vp' })
          }
          .borderRadius('8vp')
          .backgroundColor($r("app.color.blue"))
          .width('100vp')
          .height('66vp')
          .margin({ top : '15vp' , left : '20vp' })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            promptAction.showToast({ message : '开始Br蓝牙发现场景测试' })
            this.forLoop()
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('场景测试结束').fontSize('24vp').fontColor(0xffffff).margin({ left : '5vp' , right : '5vp' })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius('8vp')
          .backgroundColor($r("app.color.blue"))
          .width('100vp')
          .height('66vp')
          .margin({ top : '15vp' , left : '10vp' , right : '10vp' })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let self = this
            //            const arr = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
            let brFoundScenarioMessage = ""
            // 停止蓝牙发现
            brFoundScenarioMessage += BrManagerInterface.testStopBluetoothDiscoveryManager() + "\n"
            console.log("testStopBluetoothDiscovery success")
            //            self.changeIndex = arr[8]
            await sleep(1)
            //关闭注册状态变化
            brFoundScenarioMessage += BrManagerInterface.testOnStateChangeManager() + "\n"
            console.log("testOffStateChange success")
            await sleep(1)
            //关蓝牙
            brFoundScenarioMessage += BrManagerInterface.testDisableBluetoothManager() + "\n"
            console.log("testDisableBluetooth success")
            //            self.changeIndex = arr[9]
            await sleep(1)
            AppStorage.setOrCreate("brFoundScenarioMessage" , brFoundScenarioMessage)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('清空日志').fontSize('24vp').fontColor(0xffffff).margin({ left : '5vp' , right : '5vp' })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius('8vp')
          .backgroundColor($r("app.color.moon"))
          .width('80vp')
          .height('66vp')
          .margin({ top : '15vp' , right : '20vp' })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let brFoundScenarioMessage = ""
            AppStorage.setOrCreate("brFoundScenarioMessage" , brFoundScenarioMessage)
          })
        }
        .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        .width('100%')
      }
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
      .width('100%')
      .height('100%');
    }
  }

  subscribeBluetoothDeviceFind(): string {
    let message = "subscribeBluetoothDeviceFind test"
    console.log("subscribeBluetoothDeviceFind")
    BluetoothModelManager.subscribeBluetoothDeviceFind((deviceIds: Array<string>) => {
      console.log('available bluetooth devices changed.');
      deviceIds?.forEach(async(deviceId) => {
        // let device = brFoundDevices.find((availableDevice) => {
        //   return availableDevice.deviceId === deviceId
        //  })
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
      { name : "testSetLocalNameManager" , para : [ "DaYuBlue" ] },
      { name : "testSetBluetoothScanModeManager" , para : [ 4, 0 ] }
    ]
    let interfaces = BrFoundManagerScenarioTestCase.map(i => {
      let item = brScenarioArray.find(itm => itm.name === i.api.name);
      let para = item ? item.para : [];
      return { name : i.api.name , para } as BTAutoArrayItem;
    })

    for ( let m = 0 ; m < interfaces.length ; m ++ ) {
      // 检查 BrFoundManagerScenarioTestCase 中对应的 api 是否是一个函数
      if ( typeof BrFoundManagerScenarioTestCase[ m ].api === "function" ) {
        // 使用 TypeScript 的类型断言确保 para 是一个数组
        brFoundScenarioMessage += BrFoundManagerScenarioTestCase[ m ].api(... interfaces[ m ].para) + "\n";
      } else {
        console.error(`在 BrAutoTestCase 中找不到方法: ${ BrFoundManagerScenarioTestCase[ m ] }`);
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