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


import { TestData } from '../MainAbility/model/testData'
import { TestListItem } from './testListItems';
import ConfigData from '../utils/ConfigData';
/**
 * Test-List of bluetooth test
 */
@Component
export struct TestList {
  private testItems!: TestData[]

  aboutToAppear() {
    console.info("TestList testItems:" + JSON.stringify(this.testItems))
  }

  build() {
    Column() {
      Scroll() {
        List() {
          ForEach(this.testItems , (item: TestData) => {
            ListItem() {
              TestListItem({
                testItem : item
              })
            }
          } , (item: TestData) => item.id)
        }
        .height(ConfigData.WH_100_100)
        .backgroundColor($r("app.color.lead"))
      }.scrollBarWidth($r('app.float.wh_value_20'))
    }
  }
}