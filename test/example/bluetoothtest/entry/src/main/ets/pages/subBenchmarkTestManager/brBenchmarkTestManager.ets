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
import { BTAutoArrayItem, testItemFromRouterOrPlaceholder } from '../../Utils/common'
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
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @State changeIndex: number = - 1
  @StorageLink("brBenchmarkTestMessage") brBenchmarkTestMessage: string = ""

  build() {
    Column() {
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
              .accessibilityLevel('yes')
              .id('pages_subBenchmarkTestManager_brBenchmarkTestManager__pt_0')
      }

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));

      Column() {
        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_xncs'))
              .fontSize(ConfigData.VP_17)
              .margin({ top : ConfigData.VP_10 , bottom : ConfigData.VP_10 , left : ConfigData.VP_20 })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            Text($r('app.string.bluetoothtest_text_bdxncsscszfxlysdsjxnhwdxn'))
              .fontSize(ConfigData.VP_17)
              .margin({ left : ConfigData.VP_10 })
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.VP_100)
        }
        .accessibilityLevel('yes')
        .id('pages_subBenchmarkTestManager_brBenchmarkTestManager__sc_0')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)

        Stack()
          .height(ConfigData.HEIGHT_1_VP)
          .backgroundColor($r('app.color.black'))

        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_rz'))
              .fontSize(ConfigData.VP_17)
              .margin({ top : ConfigData.VP_8 , bottom : ConfigData.VP_8 , left : ConfigData.VP_10 })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            List() {
              ListItem() {
                Text(this.brBenchmarkTestMessage)
                  .fontSize(ConfigData.VP_17)
                  .margin({ top : ConfigData.VP_5 , left : ConfigData.VP_10 , right : ConfigData.VP_10 })
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
            .accessibilityLevel('yes')
            .id('pages_subBenchmarkTestManager_brBenchmarkTestManager__li_0')
            }
            .height(ConfigData.WH_90_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.VP_400)
        }
        .accessibilityLevel('yes')
        .id('pages_subBenchmarkTestManager_brBenchmarkTestManager__sc_1')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.On)
      }
      .width(ConfigData.WH_100_100)
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Text($r('app.string.bluetoothtest_text_xncszb'))
              .fontSize(ConfigData.VP_24)
              .fontColor(0xffffff)
              .margin({ left : ConfigData.VP_5 , right : ConfigData.VP_5 })
          }
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTestManager_brBenchmarkTestManager__btn_0')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , left : ConfigData.VP_5 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            this.forLoop()
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text($r('app.string.bluetoothtest_text_sjxncs'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({ left : ConfigData.VP_5 , right : ConfigData.VP_5 })
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTestManager_brBenchmarkTestManager__btn_1')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , left : ConfigData.VP_5 , right : ConfigData.VP_5 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let brBenchmarkTestMessage = ""
            brBenchmarkTestMessage += $r('app.string.bluetoothtest_str_s') + testFunc(subscribeBluetoothDeviceFind) + "\n"
            AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
            await sleep(1)
            brBenchmarkTestMessage += $r('app.string.bluetoothtest_str_szx') + testFunc(BluetoothModelManager.startBluetoothDiscovery) + "\n"
            AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
            await sleep(10)
            brBenchmarkTestMessage += $r('app.string.bluetoothtest_str_szx_1') + testFunc(BrManagerInterface.testStopBluetoothDiscoveryManager) + "\n"
            AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
            await sleep(1)
            brBenchmarkTestMessage += $r('app.string.bluetoothtest_str_t') + testFunc(BrManagerInterface.testOnStateChangeManager) + "\n"
            AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
            await sleep(1)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text($r('app.string.bluetoothtest_text_wdxncs'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({ left : ConfigData.VP_5 , right : ConfigData.VP_5 })
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTestManager_brBenchmarkTestManager__btn_2')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , left : ConfigData.VP_5 , right : ConfigData.VP_5 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let brBenchmarkTestMessage = ""
            let i = 0
            for ( i ; i < 10 ; i ++ ) {
              let m = i + 1
              brBenchmarkTestMessage = $r('app.string.bluetoothtest_str_d_1') + (i + 1) + $r('app.string.bluetoothtest_str_cwdxcs') + "\n"
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(1)
              brBenchmarkTestMessage += $r('app.string.bluetoothtest_str_s') + testFunc(subscribeBluetoothDeviceFind) + "\n"
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(1)
              brBenchmarkTestMessage += $r('app.string.bluetoothtest_str_szx') + testFunc(BluetoothModelManager.startBluetoothDiscovery) + "\n"
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(10)
              brBenchmarkTestMessage += $r('app.string.bluetoothtest_str_szx_1') + testFunc(BrManagerInterface.testStopBluetoothDiscoveryManager) + "\n"
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(1)
              brBenchmarkTestMessage += $r('app.string.bluetoothtest_str_uzx') + testFunc(BluetoothModelManager.unsubscribeBluetoothDeviceFind) + "\n"
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(1)
              brBenchmarkTestMessage = ""
              AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
              await sleep(1)
            }
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Image($r('app.media.ic_public_delete_filled'))
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTestManager_brBenchmarkTestManager__img_0').width(ConfigData.VP_30).height(ConfigData.VP_40).margin({ left : ConfigData.VP_5 })
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTestManager_brBenchmarkTestManager__btn_3')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.moon"))
          .width(ConfigData.VP_50)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , right : ConfigData.VP_5 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let brBenchmarkTestMessage = ""
            AppStorage.setOrCreate("brBenchmarkTestMessage" , brBenchmarkTestMessage)
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
  hilog.info(0x0000, 'BtTest', '%{public}s', $r('app.string.bluetoothtest_str_ks') + Number(start) + "ms")
  hilog.info(0x0000, 'BtTest', '%{public}s', $r('app.string.bluetoothtest_str_js') + Number(end) + "ms")

  hilog.info(0x0000, 'BtTest', '%{public}s', $r('app.string.bluetoothtest_str_hf') + Number(end - start) + "ms")
  let message = ""
  message += $r('app.string.bluetoothtest_str_hf') + Number(end - start) + "ms" + "\n"
  message += $r('app.string.bluetoothtest_str_ks') + Number(start) + $r('app.string.bluetoothtest_str_m_1') + $r('app.string.bluetoothtest_str_js') + Number(end) + "ms" + "\n"
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
  let brRunMessage = $r('app.string.bluetoothtest_str_s_3')
  //  let brFoundDevices: BluetoothDevice[] = [];
  //  let message = "subscribeBluetoothDeviceFind test"
  hilog.info(0x0000, 'BtTest', '%{public}s', "subscribeBluetoothDeviceFind test")
  BluetoothModelManager.subscribeBluetoothDeviceFind((deviceInfo: Array<string>) => {
    let end1 = new Date().getTime()
    hilog.info(0x0000, 'BtTest', '%{public}s', 'available bluetooth devices changed.');
    deviceInfo?.forEach(async(deviceItemInfo) => {
      //      let device = brFoundDevices.find((availableDevice) => {
      //        return availableDevice.deviceId === deviceId
      //      })
      hilog.info(0x0000, 'BtTest', '%{public}s', 'available bluetooth find [' + deviceItemInfo + '] ')
      let message = ($r('app.string.bluetoothtest_str_abf') + deviceItemInfo + $r('app.string.bluetoothtest_str_str'))
      promptAction.showToast({ message : message })
      brRunMessage += "\n" + $r('app.string.bluetoothtest_str_hf') + Number(end1 - start1) + $r('app.string.bluetoothtest_str_m_2') + $r('app.string.bluetoothtest_str_mw') + deviceItemInfo + $r('app.string.bluetoothtest_str_str_2') + "\n"
      brRunMessage += $r('app.string.bluetoothtest_str_ks') + Number(start1) + $r('app.string.bluetoothtest_str_m_1') + $r('app.string.bluetoothtest_str_js') + Number(end1) + "ms" + "\n"

      hilog.info(0x0000, 'BtTest', '%{public}s', $r('app.string.bluetoothtest_str_hf') + Number(end1 - start1) + "ms;" + $r('app.string.bluetoothtest_str_mw') + deviceItemInfo + "]" + "\n")
      hilog.info(0x0000, 'BtTest', '%{public}s', $r('app.string.bluetoothtest_str_ks') + Number(start1) + "ms; " + $r('app.string.bluetoothtest_str_js') + Number(end1) + "ms" + "\n")

      AppStorage.setOrCreate('brRunMessage' , brRunMessage);
      await sleep(1)

      AppStorage.setOrCreate("brBenchmarkTestMessage" , brRunMessage)
      await sleep(1)
      return brRunMessage
    })
  });
}
