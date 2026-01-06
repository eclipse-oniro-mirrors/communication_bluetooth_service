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

import { BTAutoArrayItem , myParams , ScanResult } from '../../Utils/common'
import router from '@ohos.router';
import { PageTitle } from '../../Component/pageTitle';
import { TestImageDisplay } from '../../Component/testImageDisplay';
import { TestData } from '../../MainAbility/model/testData'
import * as BleInterface from '../../MainAbility/model/bleInterface'
import { testStopBLEScan } from '../../MainAbility/model/bleInterface'
import * as BrInterface from '../../MainAbility/model/brInterface'
import { testSetBluetoothScanMode , testSetLocalName } from '../../MainAbility/model/brInterface'
import BluetoothModel from '../../MainAbility/model/BluetoothModel'
import { BleBenchmarkTestCase } from '../../MainAbility/model/benchmarkTestDataModels'
import bluetooth from '@ohos.bluetooth';
import promptAction from '@ohos.promptAction';

/**
 * BLE Benchmark Test Page Of Bluetooth test
 */

@Entry
@Component
struct BleBenchmarkTest {
  private testItem: TestData = (router.getParams() as myParams).testItem
  @State changeIndex: number = - 1
  @StorageLink("bleBenchmarkTestMessage") bleBenchmarkTestMessage: string = ""

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
            Text("Ble的性能测试是测试在发现BLE蓝牙时的时间性能和稳定性能")
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
                Text(this.bleBenchmarkTestMessage)
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
            let bleBenchmarkTestMessage = ""
            bleBenchmarkTestMessage += "subscribeBLEDeviceFind:" + testFunc(subscribeBLEDeviceFind) + "\n"
            AppStorage.setOrCreate("bleBenchmarkTestMessage" , bleBenchmarkTestMessage)
            await sleep(1)
            bleBenchmarkTestMessage += "startBleScan" + testFunc(BluetoothModel.startBLEScan) + "\n"
            AppStorage.setOrCreate("bleBenchmarkTestMessage" , bleBenchmarkTestMessage)
            await sleep(10)
            bleBenchmarkTestMessage += "testStopBLEScan 执行：" + testFunc(BleInterface.testStopBLEScan) + "\n"
            AppStorage.setOrCreate("bleBenchmarkTestMessage" , bleBenchmarkTestMessage)
            await sleep(1)
            bleBenchmarkTestMessage += "testOffStateChange:" + testFunc(BrInterface.testOnStateChange) + "\n"
            AppStorage.setOrCreate("bleBenchmarkTestMessage" , bleBenchmarkTestMessage)
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
            let bleBenchmarkTestMessage = ""
            let i = 0
            for ( i ; i < 10 ; i ++ ) {
              bleBenchmarkTestMessage = "第" + (i + 1) + "次稳定性测试：" + "\n"
              AppStorage.setOrCreate("bleBenchmarkTestMessage" , bleBenchmarkTestMessage)
              await sleep(1)

              bleBenchmarkTestMessage += "subscribeBLEDeviceFind:" + testFunc(subscribeBLEDeviceFind) + "\n"
              AppStorage.setOrCreate("bleBenchmarkTestMessage" , bleBenchmarkTestMessage)
              await sleep(1)

              bleBenchmarkTestMessage += "startBleScan" + testFunc(BluetoothModel.startBLEScan) + "\n"
              AppStorage.setOrCreate("bleBenchmarkTestMessage" , bleBenchmarkTestMessage)
              await sleep(10)

              bleBenchmarkTestMessage += "testStopBLEScan 执行：" + testFunc(testStopBLEScan) + "\n"
              AppStorage.setOrCreate("bleBenchmarkTestMessage" , bleBenchmarkTestMessage)
              await sleep(1)

              bleBenchmarkTestMessage += "unsubscribeBLEDeviceFind 执行：" + testFunc(BluetoothModel.unsubscribeBLEDeviceFind) + "\n"
              AppStorage.setOrCreate("bleBenchmarkTestMessage" , bleBenchmarkTestMessage)
              await sleep(1)

              bleBenchmarkTestMessage = ""
              AppStorage.setOrCreate("bleBenchmarkTestMessage" , bleBenchmarkTestMessage)
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
            let bleBenchmarkTestMessage = ""
            AppStorage.setOrCreate("bleBenchmarkTestMessage" , bleBenchmarkTestMessage)
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
    let bleBenchmarkTestMessage = ""
    //拼装数组
    let bleBenchmarkArray: Array<BTAutoArrayItem> = [
      { name : "testSetLocalName" , para : [ "DaYuBlue" ] },
      { name : "testSetBluetoothScanMode" , para : [ 4, 0 ] }
    ]
    let interfaces = BleBenchmarkTestCase.map(i => {
      let item = bleBenchmarkArray.find(itm => itm.name === i.api.name);
      let para = item ? item.para : [];
      return { name : i.api.name , para } as BTAutoArrayItem;
    })

    for ( let m = 0 ; m < interfaces.length ; m ++ ) {
      bleBenchmarkTestMessage += BleBenchmarkTestCase[ m ].name + "," + testFunc(BleBenchmarkTestCase[ m ].api) + "\n"
      AppStorage.setOrCreate("bleBenchmarkTestMessage" , bleBenchmarkTestMessage)
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
    case BluetoothModel.startBLEScan: {
      func(null)
    }
      break;
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

function subscribeBLEDeviceFind() {
  let start2 = new Date().getTime()
  let bleRunMessage = "scan:"
  let message = "subscribeBLEDeviceFind test"
  console.log("subscribeBLEDeviceFind test")
  BluetoothModel.subscribeBLEDeviceFind((deviceIds: Array<bluetooth.ScanResult>) => {
    let end2 = new Date().getTime()
    console.log('available ble devices changed.');
    deviceIds?.forEach(async(deviceId) => {
      let data = deviceId.data;
      let obj2: ScanResult = {}
      Object.keys(deviceId).forEach((key) => {
        obj2.deviceId = deviceId.deviceId;
        obj2.rssi = deviceId.rssi
      }); //从deviceId变量解构出data属性，并将剩余的属性收集到obj2对象中
      let btRssi = 10**(((- Number(JSON.stringify(obj2.rssi))) - 59.0) / (10.0 * 2.0))
      console.log("btRssi:" , btRssi)
      console.log("DeviceFound Start222 ------------------------")
      console.log('available ble find [' + JSON.stringify(obj2) + '] ')
      let message = ('available ble find [' + JSON.stringify(obj2) + ']  ')
      promptAction.showToast({ message : message })
      bleRunMessage += "\n" + "花费:" + Number(end2 - start2) + "ms;" + "Mac为：[" + JSON.stringify(obj2) + ",{dis：" + btRssi.toFixed(2) + "}.]" + "\n"
      bleRunMessage += "开始:" + Number(start2) + "ms; " + "结束:" + Number(end2) + "ms" + "\n"
      console.log("花费:" + Number(end2 - start2) + "ms;" + "Mac为：[" + JSON.stringify(obj2) + "]" + "\n")
      console.log("开始:" + Number(start2) + "ms; " + "结束:" + Number(end2) + "ms" + "\n")
      await sleep(1)
      AppStorage.setOrCreate('bleRunMessage' , bleRunMessage);
      await sleep(1)
      AppStorage.setOrCreate("bleBenchmarkTestMessage" , bleRunMessage)
      await sleep(1)
      promptAction.showToast({ message : bleRunMessage })
      return bleRunMessage
    })
  });
}