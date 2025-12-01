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

import { BTAutoArrayItem , myParams } from '../../../Utils/common'
import ConfigData from '../../../Utils/ConfigData';
import router from '@ohos.router';
import { TestData } from '../../../MainAbility/model/testData'
import { PageTitle } from '../../../Component/pageTitle';
import { TestImageDisplay } from '../../../Component/testImageDisplay';
import { AutoContentTable } from '../../../Component/autoContentTable';
import { GattClientAutoTestCase , initGattClientAutoData } from '../../../MainAbility/model/autoTestDataModels';

/**
 *  GattClientAutoTest Page Of Bluetooth test
 */
@Entry
@Component
struct GattClientAutoTest {
  @State changeIndex: number = - 1
  private testItem: TestData = (router.getParams() as myParams).testItem
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
      }

      Stack().height($r('app.float.wh_value_1')).backgroundColor("#000000");
      Column() {
        Scroll() {
          Column() {
            Text("API验证:")
              .fontSize("17vp")
              .margin({ top : "10vp" , bottom : "10vp" , left : "15vp" })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            AutoContentTable({
              autoItems : initGattClientAutoData() ,
              changeIndex : this.changeIndex
            })
          }
          .width($r('app.float.wh_value_100p'))
          .height($r('app.float.wh_value_260'))
        }
        .scrollBarWidth(10)
        .scrollBar(BarState.Auto)

        Stack().height($r('app.float.wh_value_1')).backgroundColor("#000000");

        Scroll() {
          Column() {
            Text("Debug日志:")
              .fontSize("17vp")
              .margin({ top : "8vp" , bottom : "8vp" , left : "15vp" })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            List() {
              ListItem() {
                Text("log:" + "\n" + this.gattClientAutoTestMessage)
                  .fontSize("17vp")
                  .margin({ top : "5vp" , left : "30vp" , right : "10vp" })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
              }
            }
            .height($r('app.float.wh_value_80p'))
            .width($r('app.float.wh_value_90p'))
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width($r('app.float.wh_value_100p'))
          .height($r('app.float.wh_value_280'))
        }
        .scrollBarWidth(10)
        .scrollBar(BarState.Auto)
      }
      .height($r('app.float.wh_value_500'))
      .width($r('app.float.wh_value_100p'))
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))


      Stack().height($r('app.float.wh_value_1')).backgroundColor("#000000");
      Column() {
        Button({ type : ButtonType.Normal , stateEffect : true }) {
          Row() {
            Text('自动测试').fontSize("24vp").fontColor(0xffffff).margin({ left : 5 , right : 5 })
          }.alignItems(VerticalAlign.Center)
        }
        .borderRadius(8)
        .backgroundColor($r("app.color.blue"))
        .width($r('app.float.wh_value_180'))
        .height($r('app.float.wh_value_50'))
        .margin({ top : "15vp" })
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
      { name : "testSetLocalName" , para : [ "DaYuBlue" ] },
      { name : "testSetBluetoothScanMode" , para : [ 4, 0 ] }
    ]
    let interfaces = GattClientAutoTestCase.map(i => {
      let item = gattClientAutoArray.find(itm => itm.name === i.api.name);
      let para = item ? item.para : []; // 确保 para 总是一个数组
      return { name : i.api.name , para } as BTAutoArrayItem;
    });
    for ( let m = 0 ; m < interfaces.length ; m ++ ) {
      // 检查 GattClientAutoTestCase 中对应的 api 是否是一个函数
      if ( typeof GattClientAutoTestCase[ m ].api === "function" ) {
        // 使用 TypeScript 的类型断言确保 para 是一个数组
        gattClientAutoTestMessage += GattClientAutoTestCase[ m ].api(... interfaces[ m ].para) + "\n";
      } else {
        console.error(`在 BrAutoTestCase 中找不到方法: ${ GattClientAutoTestCase[ m ] }`);
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