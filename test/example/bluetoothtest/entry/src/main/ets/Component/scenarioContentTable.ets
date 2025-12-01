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

import { TestScenario } from '../MainAbility/model/testData'

/**
 *  Scenario Test ContentTable Component Page Of Bluetooth test
 */

@Component
export struct ScenarioContentTable {
  @State scenarioItems: TestScenario[] = [];
  @Prop changeIndex: number;
  @State localName: string = 'DaYuBlue'

  @Builder IngredientItem(scenarioItem: TestScenario , index: number) {
    Stack() {
      Flex() {
        Flex({ direction : FlexDirection.Column , alignItems : ItemAlign.Start }) {
          Row() {
            Text(scenarioItem.detail)
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_3') , bottom : $r('app.float.wh_value_3') , left : $r('app.float.wh_value_10') })
              .textAlign(TextAlign.Start)
              .height($r('app.float.wh_value_40'))
              .width($r('app.float.wh_value_30'))

            Text(scenarioItem.method)
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_3') , bottom : $r('app.float.wh_value_3') , left : $r('app.float.wh_value_10') })
              .textAlign(TextAlign.Start)
              .height($r('app.float.wh_value_40'))
              .width($r('app.float.wh_value_240'))
          }
          .height($r('app.float.wh_value_30'))
          .width($r('app.float.wh_value_70p'))
        }
        .height($r('app.float.wh_value_30'))
        .layoutWeight(6)

        Flex({ alignItems : ItemAlign.Center }) {
          Text(this.changeIndex >= 0 ? this.getCurrentState(index) : scenarioItem.result)
            .margin({ left : $r('app.float.wh_value_10') })
            .fontSize($r('app.float.wh_value_17'))
            .flexGrow(1)
        }
        .layoutWeight(3)
        .padding({ left : 10 })
        .margin({ left : 10 })
      }
    }
    .height($r('app.float.wh_value_40'))
    .borderRadius(10)
    .margin({ top : 3 , bottom : 3 })
    .backgroundColor($r("app.color.white"))
  }

  getCurrentState(index: number) {
    if ( index <= this.changeIndex ) this.scenarioItems[ index ].result = 'pass';
    return this.scenarioItems[ index ].result
  }

  aboutToAppear(): void {
    console.log("scenario组件构建完毕")
  }

  build() {
    Column() {
      Scroll() {
        Flex({
          direction : FlexDirection.Column ,
          justifyContent : FlexAlign.SpaceBetween ,
          alignItems : ItemAlign.Start
        }) {
          ForEach(this.scenarioItems , (item: TestScenario , index: number) => {
            this.IngredientItem(item , index)
          })
        }
      }
      .scrollBarWidth(20)
    }
    .height($r('app.float.wh_value_80p'))
    .padding({ top : 5 , right : 10 , left : 10 })
  }
}