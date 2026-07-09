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
import LogUtil from '../../../Utils/LogUtil';
import { resStr} from '../../../Utils/resString';


import { BTAutoArrayItem, testItemFromRouterOrPlaceholder} from '../../../Utils/common'
import ConfigData from '../../../Utils/ConfigData';
import { TestData} from '../../../MainAbility/model/testData'
import { PageTitle} from '../../../component/pageTitle';
import { TestImageDisplay} from '../../../component/testImageDisplay';
import { AutoContentTable} from '../../../component/autoContentTable';
import {
  HitHostProfileManagerAutoTestCase,
  initHitHostProfileManagerAutoData
} from '../../../MainAbility/model/autoTestDataModelsManager';
import btmProfileModel from '../../../MainAbility/model/btmProfileModel'

/**
 *  HitHostAutoTest Manager Page Of Bluetooth test
 */

@Entry
@Component
struct HitHostAutoTestManager {
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @State changeIndex: number = - 1;
  @State profileType: number = 6;
  @State btConnectionState: number = 0;
  private deviceId = "8C:5A:F8:25:FB:E9";
  @StorageLink("hidHostProfileAutoTestMessage") hidHostProfileAutoTestMessage: string = ""

  aboutToAppear() {
    AppStorage.setOrCreate("deviceId", this.deviceId)
    this.profileType = btmProfileModel.PROFILE_A2DP_SOURCE;
    AppStorage.setOrCreate("profileType", this.profileType)
    AppStorage.setOrCreate("btConnectionState", this.btConnectionState)
    btmProfileModel.initProfile()
  }

  build() {
    Column() {
      Stack({alignContent: Alignment.TopStart}) {
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
        .accessibilityLevel('yes')
        .id('pages_subAutoTestManager_subProfileAutoTestManager_hitHostAutoTestManager__pt_0')
      }

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column() {
        Scroll() {
          Column() {
            Text($r('app.string.autotest_api_verify_label'))
              .fontSize(ConfigData.VP_17)
              .margin({
                top: ConfigData.VP_10,
                bottom: ConfigData.VP_10,
                left: ConfigData.VP_15
              })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            AutoContentTable({
              autoItems: initHitHostProfileManagerAutoData(),
              changeIndex: this.changeIndex
            })

          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_260_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subAutoTestManager_subProfileAutoTestManager_hitHostAutoTestManager__sc_0')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)

        Stack()
          .height(ConfigData.HEIGHT_1_VP)
          .backgroundColor($r('app.color.black'));

        Scroll() {
          Column() {
            Text($r('app.string.autotest_debug_log_label'))
              .fontSize(ConfigData.VP_17)
              .margin({
                top: ConfigData.VP_8,
                bottom: ConfigData.VP_8,
                left: ConfigData.VP_15
              })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            List() {
              ListItem() {
                Text(resStr($r('app.string.autotest_log_prefix')) + "\n" +
                  this.hidHostProfileAutoTestMessage)
                  .fontSize(ConfigData.VP_17)
                  .margin({
                    top: ConfigData.VP_5,
                    left: ConfigData.VP_30,
                    right: ConfigData.VP_10
                  })
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
            .accessibilityLevel('yes')
            .id('pages_subAutoTestManager_subProfileAutoTestManager_hitHostAutoTestManager__li_0')
            }
            .height(ConfigData.WH_80_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_280_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subAutoTestManager_subProfileAutoTestManager_hitHostAutoTestManager__sc_1')
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
        Button({
          type: ButtonType.Normal,
          stateEffect: true
        }) {
          Row() {
            Text($r('app.string.autotest'))
              .fontSize(ConfigData.VP_24)
              .fontColor(0xffffff)
              .margin({
                left: ConfigData.VP_5,
                right: ConfigData.VP_5
              })
          }
            .alignItems(VerticalAlign.Center)
        }
        .accessibilityLevel('yes')
        .id('pages_subAutoTestManager_subProfileAutoTestManager_hitHostAutoTestManager__btn_0')
        .borderRadius(ConfigData.VP_8)
        .backgroundColor($r("app.color.blue"))
        .width(ConfigData.VP_180)
        .height(ConfigData.VP_50)
        .margin({top: ConfigData.VP_15})
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
    let hidHostProfileAutoTestMessage = ""

    //拼装数组
    let hitHostAutoArray: Array<BTAutoArrayItem> = [
      {
        name: "testSetLocalNameManager",
        para: ["DaYuBlue"]
      },
      {
        name: "testSetBluetoothScanModeManager",
        para: [4, 0]
      }
    ]
    let interfaces = HitHostProfileManagerAutoTestCase.map(i => {
      let item = hitHostAutoArray.find(itm => itm.name === i.api.name);
      let para = item ? item.para: [];// 确保 para 总是一个数组
      return { name: i.api.name, para} as BTAutoArrayItem;
    });
    for(let m = 0 ; m<interfaces.length ; m++) {
      // 检查 HitHostProfileManagerAutoTestCase 中对应的 api 是否是一个函数
      if (typeof HitHostProfileManagerAutoTestCase[m].api === "function") {
        // 使用 TypeScript 的类型断言确保 para 是一个数组
        hidHostProfileAutoTestMessage += HitHostProfileManagerAutoTestCase[m].api(... interfaces[
          m].para)
          + "\n";
      } else {
        LogUtil.error(`在 BrAutoTestCase 中找不到方法: ${HitHostProfileManagerAutoTestCase[m] }`);
      }
      this.changeIndex = m
      await sleep(1)
      AppStorage.setOrCreate("hidHostProfileAutoTestMessage", hidHostProfileAutoTestMessage)
    }
  }
}

async function sleep(time: number): Promise<void> {
  return new Promise<void>((resolve, reject) => {
    setTimeout(() => {
      resolve();
    }, time * 1000);
  });
}