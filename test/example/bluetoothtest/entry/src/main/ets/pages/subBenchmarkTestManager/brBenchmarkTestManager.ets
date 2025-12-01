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
import router from '@ohos.router';
import { PageTitle } from '../../Component/pageTitle';
import { TestImageDisplay } from '../../Component/testImageDisplay';
import { TestData } from '../../MainAbility/model/testData'
import * as BrManagerInterface from '../../MainAbility/model/brManagerInterface'
import { testSetBluetoothScanModeManager , testSetLocalNameManager } from '../../MainAbility/model/brManagerInterface'
import BluetoothModelManager from '../../MainAbility/model/BluetoothModelManager'
import { BrManagerBenchmarkTestCase } from '../../MainAbility/model/benchmarkTestDataModelsManager'
import promptAction from '@ohos.promptAction';

/**
 * Classic Bluetooth Manager Benchmark Test Page Of Bluetooth test
 */

@Entry
@Component
struct BrBenchmarkTestManager {
  private testItem: TestData = (router.getParams() as myParams).testItem
  @State changeIndex: number = - 1
  @StorageLink("brBenchmarkTestMessage") brBenchmarkTestMessage: string = ""

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
            Text("性能测试:")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_10') , bottom : $r('app.float.wh_value_10') , left : $r('app.float.wh_value_20') })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            Text("Br的性能测试是测试在发现蓝牙时的时间性能和稳定性能")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ left : $r('app.float.wh_value_10') })
          }
          .width($r('app.float.wh_value_100p'))
          .height($r('app.float.wh_value_100'))
        }
        .scrollBarWidth(10)
        .scrollBar(BarState.Auto)

        Stack().height($r('app.float.wh_value_1')).backgroundColor("#000000")

        Scroll() {
          Column() {
            Text("日志:")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_8') , bottom : $r('app.float.wh_value_8') , left : $r('app.float.wh_value_10') })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            List() {
              ListItem() {
                Text(this.brBenchmarkTestMessage)
                  .fontSize($r('app.float.wh_value_17'))
                  .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_10') , right : $r('app.float.wh_value_10') })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
              }
            }
            .height($r('app.float.wh_value_90p'))
            .width($r('app.float.wh_value_90p'))
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width($r('app.float.wh_value_100p'))
          .height($r('app.float.wh_value_400'))
        }
        .scrollBarWidth(10)
        .scrollBar(BarState.On)
      }
      .width($r('app.float.wh_value_100p'))
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Text('性能测试准备').fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : 5 , right : 5 })
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.blue"))
          .width($r('app.float.wh_value_100'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , left : $r('app.float.wh_value_5') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            this.forLoop()
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('时间性能测试').fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : 5 , right : 5 })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.blue"))
          .width($r('app.float.wh_value_100'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , left : $r('app.float.wh_value_5') , right : $r('app.float.wh_value_5') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let brBenchmarkTestMessage = ""
            brBenchmarkTestMessage += "subscribeBluetoothDeviceFind:" + testFunc(subscribeBluetoothDeviceFind) + "\n"
            AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
            await sleep(1)
            brBenchmarkTestMessage += "startBluetoothDiscovery 执行：" + testFunc(BluetoothModelManager.startBluetoothDiscovery) + "\n"
            AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
            await sleep(10)
            brBenchmarkTestMessage += "stopBluetoothDiscovery 执行：" + testFunc(BrManagerInterface.testStopBluetoothDiscoveryManager) + "\n"
            AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
            await sleep(1)
            brBenchmarkTestMessage += "testOffStateChange:" + testFunc(BrManagerInterface.testOnStateChangeManager) + "\n"
            AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
            await sleep(1)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('稳定性能测试').fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : 5 , right : 5 })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.blue"))
          .width($r('app.float.wh_value_100'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , left : $r('app.float.wh_value_5') , right : $r('app.float.wh_value_5') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let brBenchmarkTestMessage = ""
            let i = 0
            for ( i ; i < 10 ; i ++ ) {
              let m = i + 1
              brBenchmarkTestMessage = "第" + (i + 1) + "次稳定性测试：" + "\n"
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(1)
              brBenchmarkTestMessage += "subscribeBluetoothDeviceFind:" + testFunc(subscribeBluetoothDeviceFind) + "\n"
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(1)
              brBenchmarkTestMessage += "startBluetoothDiscovery 执行：" + testFunc(BluetoothModelManager.startBluetoothDiscovery) + "\n"
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(10)
              brBenchmarkTestMessage += "stopBluetoothDiscovery 执行：" + testFunc(BrManagerInterface.testStopBluetoothDiscoveryManager) + "\n"
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(1)
              brBenchmarkTestMessage += "unsubscribeBluetoothDeviceFind 执行：" + testFunc(BluetoothModelManager.unsubscribeBluetoothDeviceFind) + "\n"
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(1)
              brBenchmarkTestMessage = ""
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(1)
            }
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Image($r('app.media.ic_public_delete_filled')).width($r('app.float.wh_value_30')).height($r('app.float.wh_value_40')).margin({ left : 5 })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.moon"))
          .width($r('app.float.wh_value_50'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , right : $r('app.float.wh_value_5') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let brBenchmarkTestMessage = ""
            AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
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

  async forLoop() {
    let brBenchmarkTestMessage = ""
    //拼装数组
    let brBenchmarkArray: Array<BTAutoArrayItem> = [
      { name : "testSetLocalNameManager" , para : [ "DaYuBlue" ] },
      { name : "testSetBluetoothScanModeManager" , para : [ 4, 0 ] }
    ]
    let interfaces = BrManagerBenchmarkTestCase.map(i => {
      let item = brBenchmarkArray.find(itm => itm.name === i.api.name);
      let para = item ? item.para : [];
      return { name : i.api.name , para } as BTAutoArrayItem;
    })
    for ( let m = 0 ; m < interfaces.length ; m ++ ) {
      brBenchmarkTestMessage += BrManagerBenchmarkTestCase[ m ].name + "," + testFunc(BrManagerBenchmarkTestCase[ m ].api) + "\n"
      AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
      await sleep(1)
    }
  }
}


export function test(func: Function) {
  let start = new Date().getTime(); //起始时间
  func(); //执行待测函数
  let end = new Date().getTime(); //接受时间
  return (end - start) + "ms"; //返回函数执行需要时间
}

export function testFunc(func: Function) {
  let start = new Date().getTime()
  switch ( func ) {
    case testSetLocalNameManager: {
      func("DaYuBlue")
    }
      break;
    case testSetBluetoothScanModeManager: {
      func(4 , 0)
    }
      break;
    default: {
      func()
    }
      break;
  }
  let end = new Date().getTime()
  console.log("开始:" + Number(start) + "ms")
  console.log("结束:" + Number(end) + "ms")

  console.log("花费:" + Number(end - start) + "ms")
  let message = ""
  message += "花费:" + Number(end - start) + "ms" + "\n"
  message += "开始:" + Number(start) + "ms; " + "结束:" + Number(end) + "ms" + "\n"
  return message
}


async function sleep(time: number): Promise<void> {
  return new Promise<void>((resolve , reject) => {
    setTimeout(() => {
      resolve();
    } , time * 1000);
  });
}

function subscribeBluetoothDeviceFind() {
  let start1 = new Date().getTime()
  let brRunMessage = "scan:"
  //  let brFoundDevices: BluetoothDevice[] = [];
  //  let message = "subscribeBluetoothDeviceFind test"
  console.log("subscribeBluetoothDeviceFind test")
  BluetoothModelManager.subscribeBluetoothDeviceFind((deviceInfo: Array<string>) => {
    let end1 = new Date().getTime()
    console.log('available bluetooth devices changed.');
    deviceInfo?.forEach(async(deviceItemInfo) => {
      //      let device = brFoundDevices.find((availableDevice) => {
      //        return availableDevice.deviceId === deviceId
      //      })
      console.log('available bluetooth find [' + deviceItemInfo + '] ')
      let message = ('available bluetooth find [' + deviceItemInfo + ']  ')
      promptAction.showToast({ message : message })
      brRunMessage += "\n" + "花费:" + Number(end1 - start1) + "ms;" + "Mac为：[" + deviceItemInfo + "]" + "\n"
      brRunMessage += "开始:" + Number(start1) + "ms; " + "结束:" + Number(end1) + "ms" + "\n"

      console.log("花费:" + Number(end1 - start1) + "ms;" + "Mac为：[" + deviceItemInfo + "]" + "\n")
      console.log("开始:" + Number(start1) + "ms; " + "结束:" + Number(end1) + "ms" + "\n")

      AppStorage.setOrCreate('brRunMessage' , brRunMessage);
      await sleep(1)

      AppStorage.setOrCreate("brBenchmarkTestMessage" , brRunMessage)
      await sleep(1)
      return brRunMessage
    })
  });
}
