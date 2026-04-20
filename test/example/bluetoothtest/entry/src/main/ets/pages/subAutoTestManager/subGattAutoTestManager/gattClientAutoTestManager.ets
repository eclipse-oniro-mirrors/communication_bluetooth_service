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
import { resStr } from '../../../Utils/resString';


import { BTAutoArrayItem, testItemFromRouterOrPlaceholder } from '../../../Utils/common'
import ConfigData from '../../../Utils/ConfigData';
import { TestData } from '../../../MainAbility/model/testData'
import { PageTitle } from '../../../Component/pageTitle';
import { TestImageDisplay } from '../../../Component/testImageDisplay';
import { AutoContentTable } from '../../../Component/autoContentTable';
import {
  GattClientManagerAutoTestCase ,
  initGattClientManagerAutoData
} from '../../../MainAbility/model/autoTestDataModelsManager';

/**
 *  GattClientAutoTest Manager Page Of Bluetooth test
 */

@Entry
@Component
struct GattClientAutoTestManager {
  @State changeIndex: number = - 1
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @StorageLink("gattClientAutoTestMessage") gattClientAutoTestMessage: string = ""

  aboutToAppear() {
    AppStorage.setOrCreate('peripheralDeviceId' , '6c:96:d7:3d:87:6f');
    AppStorage.setOrCreate('serviceUUID' , '00001801-0000-1000-8000-00805f9b34fb');
    AppStorage.setOrCreate('characteristicUUID' , '00002b29-0000-1000-8000-00805f9b34fb');
    AppStorage.setOrCreate('characteristicValue' , '00002b29-0000-1000-8000-00805f9b34fb');
    AppStorage.setOrCreate('descriptorUUID' , '00002902-0000-1000-8000-00805f9b34fb');
    AppStorage.setOrCreate('descriptorValue' , '00002902-0000-1000-8000-00805f9b34fb');
  }

  build() {
    Column() {
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
              .accessibilityLevel('yes')
              .id('pages_subAutoTestManager_subGattAutoTestManager_gattClientAutoTestManager__pt_0')
      }

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column() {
        Scroll() {
          Column() {
            Text($r('app.string.autotest_api_verify_label'))
              .fontSize(ConfigData.VP_17)
              .margin({ top : ConfigData.VP_10 , bottom : ConfigData.VP_10 , left : ConfigData.VP_15 })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            AutoContentTable({
              autoItems : initGattClientManagerAutoData() ,
              changeIndex : this.changeIndex
            })
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_260_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subAutoTestManager_subGattAutoTestManager_gattClientAutoTestManager__sc_0')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)

        Stack()
          .height(ConfigData.HEIGHT_1_VP)
          .backgroundColor($r('app.color.black'));

        Scroll() {
          Column() {
            Text($r('app.string.autotest_debug_log_label'))
              .fontSize(ConfigData.VP_17)
              .margin({ top : ConfigData.VP_8 , bottom : ConfigData.VP_8 , left : ConfigData.VP_15 })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            List() {
              ListItem() {
                Text(resStr($r('app.string.autotest_log_prefix')) + "\n" + this.gattClientAutoTestMessage)
                  .fontSize(ConfigData.VP_17)
                  .margin({ top : ConfigData.VP_5 , left : ConfigData.VP_30 , right : ConfigData.VP_10 })
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
            .accessibilityLevel('yes')
            .id('pages_subAutoTestManager_subGattAutoTestManager_gattClientAutoTestManager__li_0')
            }
            .height(ConfigData.WH_80_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_280_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subAutoTestManager_subGattAutoTestManager_gattClientAutoTestManager__sc_1')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)
      }
      .height(ConfigData.HEIGHT_500_VP)
      .width(ConfigData.WH_100_100)
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column() {
        Button({ type : ButtonType.Normal , stateEffect : true }) {
          Row() {
            Text($r('app.string.autotest'))
              .fontSize(ConfigData.VP_24)
              .fontColor(0xffffff)
              .margin({ left : ConfigData.VP_5 , right : ConfigData.VP_5 })
          }
            .alignItems(VerticalAlign.Center)
        }
        .accessibilityLevel('yes')
        .id('pages_subAutoTestManager_subGattAutoTestManager_gattClientAutoTestManager__btn_0')
        .borderRadius(ConfigData.VP_8)
        .backgroundColor($r("app.color.blue"))
        .width(ConfigData.VP_180)
        .height(ConfigData.VP_50)
        .margin({ top : ConfigData.VP_15 })
        .onClick(async(event: ClickEvent) => {
          this.forLoop()
        })
      }
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
      .width(ConfigData.WH_100_100)
      .height(ConfigData.WH_100_100);
    }
  }

  //循环调用
  async forLoop() {
    let gattClientAutoTestMessage = ""
    //拼装数组
    let gattClientAutoArray: Array<BTAutoArrayItem> = [
      { name : "testSetLocalNameManager" , para : [ "DaYuBlue" ] },
      { name : "testSetBluetoothScanModeManager" , para : [ 4, 0 ] }
    ]
    let interfaces = GattClientManagerAutoTestCase.map(i => {
      let item = gattClientAutoArray.find(itm => itm.name === i.api.name);
      let para = item ? item.para : []; // 确保 para 总是一个数组
      return { name : i.api.name , para } as BTAutoArrayItem;
    });
    for ( let m = 0 ; m < interfaces.length ; m ++ ) {
      // 检查 GattClientManagerAutoTestCase 中对应的 api 是否是一个函数
      if ( typeof GattClientManagerAutoTestCase[ m ].api === "function" ) {
        // 使用 TypeScript 的类型断言确保 para 是一个数组
        gattClientAutoTestMessage += GattClientManagerAutoTestCase[ m ].api(... interfaces[ m ].para) + "\n";
      } else {
        console.error(`在 BrAutoTestCase 中找不到方法: ${ GattClientManagerAutoTestCase[ m ] }`);
      }
      this.changeIndex = m
      await sleep(1)
      AppStorage.setOrCreate("gattClientAutoTestMessage" , gattClientAutoTestMessage)
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