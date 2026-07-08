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

import LogUtil from '../Utils/LogUtil';
import ConfigData from '../Utils/ConfigData';
import { TestScenario} from '../MainAbility/model/testData'

/**
 *  Scenario Test ContentTable Component Page Of Bluetooth test
 */

@Component
export struct ScenarioContentTable {
  @State scenarioItems: TestScenario[] = [];
  @Prop changeIndex: number;
  @State localName: string = 'DaYuBlue'

  @Builder IngredientItem(scenarioItem: TestScenario, index: number){
    Stack(){
      Flex(){
        Flex({direction: FlexDirection.Column, alignItems: ItemAlign.Start}){
          Row(){
            Text(scenarioItem.detail)
              .fontSize(ConfigData.VP_17)
              .margin({top: ConfigData.VP_3, bottom: ConfigData.VP_3, left: ConfigData.VP_10})
              .textAlign(TextAlign.Start)
              .height(ConfigData.VP_40)
              .width(ConfigData.VP_30)

            Text(scenarioItem.method)
              .fontSize(ConfigData.VP_17)
              .margin({top: ConfigData.VP_3, bottom: ConfigData.VP_3, left: ConfigData.VP_10})
              .textAlign(TextAlign.Start)
              .height(ConfigData.VP_40)
              .width(ConfigData.VP_240)
          }
          .height(ConfigData.VP_30)
          .width(ConfigData.PCT_70)
        }
        .height(ConfigData.VP_30)
        .layoutWeight(6)

        Flex({alignItems: ItemAlign.Center}){
          Text(this.changeIndex >= 0 ? this.getCurrentState(index) : scenarioItem.result)
            .margin({left: ConfigData.VP_10})
            .fontSize(ConfigData.VP_17)
            .flexGrow(1)
        }
        .layoutWeight(3)
        .padding({left: ConfigData.VP_10})
        .margin({left: ConfigData.VP_10})
      }
    }
    .height(ConfigData.VP_40)
    .borderRadius(ConfigData.VP_10)
    .margin({top: ConfigData.VP_3, bottom: ConfigData.VP_3})
    .backgroundColor($r("app.color.white"))
  }

  getCurrentState(index: number){
    if ( index <= this.changeIndex ) this.scenarioItems[ index ].result = 'pass';
    return this.scenarioItems[ index ].result
  }

  aboutToAppear(): void {
    LogUtil.info($r('app.string.bluetoothtest_str_szjgjwb'))
  }

  build(){
    Column(){
      Scroll(){
        Flex({
          direction: FlexDirection.Column,
          justifyContent: FlexAlign.SpaceBetween,
          alignItems: ItemAlign.Start
        }){
          ForEach(this.scenarioItems, (item: TestScenario, index: number) => {
            this.IngredientItem(item, index)
          })
        }
      }
      .scrollBarWidth(ConfigData.VP_20)
    }
    .height(ConfigData.WH_80_100)
    .padding({top: ConfigData.VP_5, right: ConfigData.VP_10, left: ConfigData.VP_10})
  }
}