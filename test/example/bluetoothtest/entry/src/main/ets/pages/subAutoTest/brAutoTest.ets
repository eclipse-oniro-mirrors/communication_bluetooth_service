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

import ConfigData from '../../Utils/ConfigData';
import router from '@ohos.router';
import { TestData } from '../../MainAbility/model/testData'
import { PageTitle } from '../../Component/pageTitle';
import { TestImageDisplay } from '../../Component/testImageDisplay';
import { BrAutoTestCase , initBrAutoData } from '../../MainAbility/model/autoTestDataModels'
import { AutoContentTable } from '../../Component/autoContentTable'
import { BTAutoArrayItem , myParams } from '../../Utils/common'

/**
 *  BrAutoTest Page Of Bluetooth test
 */

@Entry
@Component
struct BrAutoTest {
  private testItem: TestData = (router.getParams() as myParams).testItem
  @State changeIndex: number = - 1
  @StorageLink("brAutoTestMessage") brAutoTestMessage: string = ""

  build() {
    Column() {
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
      }

      Stack().height($r('app.float.wh_value_1')).backgroundColor($r('app.color.black'));
      Column() {
        Scroll() {
          Column() {
            Text("API验证:")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_10') , bottom : $r('app.float.wh_value_10') , left : $r('app.float.wh_value_15') })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            AutoContentTable({
              autoItems : initBrAutoData() ,
              changeIndex : this.changeIndex
            })
          }
          .width($r('app.float.wh_value_100p'))
          .height($r('app.float.wh_value_280'))
        }
        .scrollBarWidth($r('app.float.wh_value_10'))
        .scrollBar(BarState.Auto)

        Stack().height($r('app.float.wh_value_1')).backgroundColor($r('app.color.black'));

        Scroll() {
          Column() {
            Text("Debug日志:")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_8') , bottom : $r('app.float.wh_value_8') , left : $r('app.float.wh_value_15') })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            List() {
              ListItem() {
                Text("log:" + "\n" + this.brAutoTestMessage)
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
        .scrollBarWidth($r('app.float.wh_value_10'))
        .scrollBar(BarState.On)
      }
      .height($r('app.float.wh_value_500'))
      .width($r('app.float.wh_value_100p'))
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))


      Stack().height($r('app.float.wh_value_1')).backgroundColor($r('app.color.black'));
      Column() {
        Button({ type : ButtonType.Normal , stateEffect : true }) {
          Row() {
            Text('自动测试').fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : $r('app.float.wh_value_5') , right : $r('app.float.wh_value_5') })
          }.alignItems(VerticalAlign.Center)
        }
        .borderRadius($r('app.float.wh_value_8'))
        .backgroundColor($r("app.color.blue"))
        .width($r('app.float.wh_value_180'))
        .height($r('app.float.wh_value_50'))
        .margin({ top : $r('app.float.wh_value_15') })
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
    let brAutoTestMessage = ""
    //拼装数组
    let brAutoArray: Array<BTAutoArrayItem> = [
      { name : "testSetLocalName" , para : [ "DaYuBlue" ] },
      { name : "testSetBluetoothScanMode" , para : [ 4, 0 ] }
    ]
    let interfaces = BrAutoTestCase.map(i => {
      let item = brAutoArray.find(itm => itm.name === i.api.name);
      let para = item ? item.para : []; // 确保 para 总是一个数组
      return { name : i.api.name , para } as BTAutoArrayItem;
    });

    for ( let m = 0 ; m < interfaces.length ; m ++ ) {
      // 检查 BrAutoTestCase 中对应的 api 是否是一个函数
      if ( typeof BrAutoTestCase[ m ].api === "function" ) {
        // 使用 TypeScript 的类型断言确保 para 是一个数组
        brAutoTestMessage += BrAutoTestCase[ m ].api(... interfaces[ m ].para) + "\n";
      } else {
        console.error(`API method not found for: ${ BrAutoTestCase[ m ] }`);
      }
      this.changeIndex = m;
      await sleep(1);
      AppStorage.setOrCreate("brAutoTestMessage" , brAutoTestMessage);
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