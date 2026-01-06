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
import * as BrInterface from '../../MainAbility/model/brInterface'
import { testSetBluetoothScanMode , testSetLocalName } from '../../MainAbility/model/brInterface'
import BluetoothModel from '../../MainAbility/model/BluetoothModel'
import { BrBenchmarkTestCase } from '../../MainAbility/model/benchmarkTestDataModels'

/**
 * Classic Bluetooth Benchmark Test Page Of Bluetooth test
 */

@Entry
@Component
struct BrBenchmarkTest {
  private testItem: TestData = (router.getParams() as myParams).testItem
  @State changeIndex: number = - 1
  @StorageLink("brBenchmarkTestMessage") brBenchmarkTestMessage: string = ""

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
            Text("性能测试:")
              .fontSize('17vp')
              .margin({ top : '10vp' , bottom : '10vp' , left : '20vp' })
              .textAlign(TextAlign.Start)
              .width('100%')
            Text("Br的性能测试是测试在发现蓝牙时的时间性能和稳定性能")
              .fontSize('17vp')
              .margin({ left : '10vp' })
          }
          .width('100%')
          .height('100vp')
        }
        .scrollBarWidth('10vp')
        .scrollBar(BarState.Auto)

        Stack().height('1vp').backgroundColor($r('app.color.black'))

        Scroll() {
          Column() {
            Text("日志:")
              .fontSize('17vp')
              .margin({ top : '8vp' , bottom : '8vp' , left : '10vp' })
              .textAlign(TextAlign.Start)
              .width('100%')
            List() {
              ListItem() {
                Text(this.brBenchmarkTestMessage)
                  .fontSize('17vp')
                  .margin({ top : '5vp' , left : '10vp' , right : '10vp' })
                  .textAlign(TextAlign.Start)
                  .width('90%')
              }
            }
            .height('90%')
            .width('90%')
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width('100%')
          .height('400vp')
        }
        .scrollBarWidth('10vp')
        .scrollBar(BarState.On)
      }
      .width('100%')
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Text('性能测试准备').fontSize('24vp').fontColor(0xffffff).margin({ left : '5vp' , right : '5vp' })
          }
          .borderRadius('8vp')
          .backgroundColor($r("app.color.blue"))
          .width('100vp')
          .height('66vp')
          .margin({ top : '15vp' , left : '5vp' })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            this.forLoop()
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('时间性能测试').fontSize('24vp').fontColor(0xffffff).margin({ left : '5vp' , right : '5vp' })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius('8vp')
          .backgroundColor($r("app.color.blue"))
          .width('100vp')
          .height('66vp')
          .margin({ top : '15vp' , left : '5vp' , right : '5vp' })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let brBenchmarkTestMessage = ""
            brBenchmarkTestMessage += "subscribeBluetoothDeviceFind:" + testFunc(subscribeBluetoothDeviceFind) + "\n"
            AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
            await sleep(1)
            brBenchmarkTestMessage += "startBluetoothDiscovery 执行：" + testFunc(BluetoothModel.startBluetoothDiscovery) + "\n"
            AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
            await sleep(10)
            brBenchmarkTestMessage += "stopBluetoothDiscovery 执行：" + testFunc(BrInterface.testStopBluetoothDiscovery) + "\n"
            AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
            await sleep(1)
            brBenchmarkTestMessage += "testOffStateChange:" + testFunc(BrInterface.testOnStateChange) + "\n"
            AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
            await sleep(1)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('稳定性能测试').fontSize('24vp').fontColor(0xffffff).margin({ left : '5vp' , right : '5vp' })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius('8vp')
          .backgroundColor($r("app.color.blue"))
          .width('100vp')
          .height('66vp')
          .margin({ top : '15vp' , left : '5vp' , right : '5vp' })
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
              brBenchmarkTestMessage += "startBluetoothDiscovery 执行：" + testFunc(BluetoothModel.startBluetoothDiscovery) + "\n"
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(10)
              brBenchmarkTestMessage += "stopBluetoothDiscovery 执行：" + testFunc(BrInterface.testStopBluetoothDiscovery) + "\n"
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(1)
              brBenchmarkTestMessage += "unsubscribeBluetoothDeviceFind 执行：" + testFunc(BluetoothModel.unsubscribeBluetoothDeviceFind) + "\n"
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(1)
              brBenchmarkTestMessage = ""
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(1)
            }
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Image($r('app.media.ic_public_delete_filled')).width('30vp').height('40vp').margin({ left : '5vp' })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius('8vp')
          .backgroundColor($r("app.color.moon"))
          .width('50vp')
          .height('66vp')
          .margin({ top : '15vp' , right : '5vp' })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let brBenchmarkTestMessage = ""
            AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
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

  async forLoop() {
    let brBenchmarkTestMessage = ""
    //拼装数组
    let brBenchmarkArray: Array<BTAutoArrayItem> = [
      { name : "testSetLocalName" , para : [ "DaYuBlue" ] },
      { name : "testSetBluetoothScanMode" , para : [ 4, 0 ] }
    ]
    let interfaces = BrBenchmarkTestCase.map(i => {
      let item = brBenchmarkArray.find(itm => itm.name === i.api.name);
      let para = item ? item.para : [];
      return { name : i.api.name , para } as BTAutoArrayItem;
    })

    for ( let m = 0 ; m < interfaces.length ; m ++ ) {
      brBenchmarkTestMessage += BrBenchmarkTestCase[ m ].name + "," + testFunc(BrBenchmarkTestCase[ m ].api) + "\n"
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
    case testSetLocalName: {
      func("DaYuBlue")
    }
      break;
    case testSetBluetoothScanMode: {
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
  console.log("subscribeBluetoothDeviceFind test")
  BluetoothModel.subscribeBluetoothDeviceFind((deviceInfo: Array<string>) => {
    let end1 = new Date().getTime()
    console.log('available bluetooth devices changed.');
    deviceInfo?.forEach(async(deviceItemInfo) => {
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
