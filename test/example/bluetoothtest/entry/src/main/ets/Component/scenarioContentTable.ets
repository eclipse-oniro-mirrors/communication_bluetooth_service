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
              .fontSize('17vp')
              .margin({ top : '3vp' , bottom : '3vp' , left : '10vp' })
              .textAlign(TextAlign.Start)
              .height('40vp')
              .width('30vp')

            Text(scenarioItem.method)
              .fontSize('17vp')
              .margin({ top : '3vp' , bottom : '3vp' , left : '10vp' })
              .textAlign(TextAlign.Start)
              .height('40vp')
              .width('240vp')
          }
          .height('30vp')
          .width('70%')
        }
        .height('30vp')
        .layoutWeight(6)

        Flex({ alignItems : ItemAlign.Center }) {
          Text(this.changeIndex >= 0 ? this.getCurrentState(index) : scenarioItem.result)
            .margin({ left : '10vp' })
            .fontSize('17vp')
            .flexGrow(1)
        }
        .layoutWeight(3)
        .padding({ left : '10vp' })
        .margin({ left : '10vp' })
      }
    }
    .height('40vp')
    .borderRadius('10vp')
    .margin({ top : '3vp' , bottom : '3vp' })
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
      .scrollBarWidth('20vp')
    }
    .height('80%')
    .padding({ top : '5vp' , right : '10vp' , left : '10vp' })
  }
}