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
import {
  A2dpSourceProfileAutoTestCase ,
  initA2dpSourceProfileAutoData
} from '../../../MainAbility/model/autoTestDataModels';
import btProfileModel from '../../../MainAbility/model/btProfileModel'

/**
 *  A2dpSourceAutoTest Page Of Bluetooth test
 */

@Entry
@Component
struct A2dpSourceAutoTest {
  private testItem: TestData = (router.getParams() as myParams).testItem
  @State changeIndex: number = - 1;
  @State profileType: number = 1;
  @State btConnectionState: number = 0;
  private deviceId = "8C:5A:F8:25:FB:E9";
  @StorageLink("a2dpSourceProfileAutoTestMessage") a2dpSourceProfileAutoTestMessage: string = ""

  aboutToAppear() {
    AppStorage.setOrCreate("deviceId" , this.deviceId)
    AppStorage.setOrCreate("profileType" , this.profileType)
    AppStorage.setOrCreate("btConnectionState" , this.btConnectionState)
    btProfileModel.initProfile()
  }

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
            Text("API验证:")
              .fontSize('17vp')
              .margin({ top : '10vp' , bottom : '10vp' , left : '15vp' })
              .textAlign(TextAlign.Start)
              .width('100%')
            AutoContentTable({
              autoItems : initA2dpSourceProfileAutoData() ,
              changeIndex : this.changeIndex
            })
          }
          .width('100%')
          .height('260vp')
        }
        .scrollBarWidth('10vp')
        .scrollBar(BarState.Auto)

        Stack().height('1vp').backgroundColor($r('app.color.black'));

        Scroll() {
          Column() {
            Text("Debug日志:")
              .fontSize('17vp')
              .margin({ top : '8vp' , bottom : '8vp' , left : '15vp' })
              .textAlign(TextAlign.Start)
              .width('100%')
            List() {
              ListItem() {
                Text("log:" + "\n" + this.a2dpSourceProfileAutoTestMessage)
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
          .height('280vp')
        }
        .scrollBarWidth('10vp')
        .scrollBar(BarState.Auto)
      }
      .height('500vp')
      .width('100%')
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Stack().height('1vp').backgroundColor($r('app.color.black'));
      Column() {
        Button({ type : ButtonType.Normal , stateEffect : true }) {
          Row() {
            Text('自动测试').fontSize('24vp').fontColor(0xffffff).margin({ left : '5vp' , right : '5vp' })
          }.alignItems(VerticalAlign.Center)
        }
        .borderRadius('8vp')
        .backgroundColor($r("app.color.blue"))
        .width('180vp')
        .height('50vp')
        .margin({ top : '15vp' })
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
    let a2dpSourceProfileAutoTestMessage = ""
    //拼装数组
    let a2dpAutoArray: Array<BTAutoArrayItem> = [
      { name : "testSetLocalName" , para : [ "DaYuBlue" ] },
      { name : "testSetBluetoothScanMode" , para : [ 4, 0 ] }
    ]
    let interfaces = A2dpSourceProfileAutoTestCase.map(i => {
      let item = a2dpAutoArray.find(itm => itm.name === i.api.name);
      let para = item ? item.para : []; // 确保 para 总是一个数组
      return { name : i.api.name , para } as BTAutoArrayItem;
    });
    for ( let m = 0 ; m < interfaces.length ; m ++ ) {
      // 检查 A2dpSourceProfileAutoTestCase 中对应的 api 是否是一个函数
      if ( typeof A2dpSourceProfileAutoTestCase[ m ].api === "function" ) {
        // 使用 TypeScript 的类型断言确保 para 是一个数组
        a2dpSourceProfileAutoTestMessage += A2dpSourceProfileAutoTestCase[ m ].api(... interfaces[ m ].para) + "\n";
      } else {
        console.error(`在 BrAutoTestCase 中找不到方法: ${ A2dpSourceProfileAutoTestCase[ m ] }`);
      }
      this.changeIndex = m
      await sleep(1)
      AppStorage.setOrCreate("a2dpSourceProfileAutoTestMessage" , a2dpSourceProfileAutoTestMessage)
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