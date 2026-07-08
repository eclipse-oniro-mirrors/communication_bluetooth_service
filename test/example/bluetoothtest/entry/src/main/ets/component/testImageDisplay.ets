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

import ConfigData from '../Utils/ConfigData';
import { TestData} from '../MainAbility/model/testData'
/**
 * Test-ImageDisplay of bluetooth test
 */
@Component
export struct TestImageDisplay {
  private testItem!: TestData

  build(){
    Stack({alignContent: Alignment.BottomStart}){
      Flex({alignItems: ItemAlign.Start}){
        Stack(){
          Image(this.testItem.image)
            .height(ConfigData.VP_80)
            .width(ConfigData.VP_80)
            .margin({left: ConfigData.VP_10, bottom: ConfigData.VP_10})
        }
        .width(ConfigData.PCT_20)

        Column(){
          Text(this.testItem.name)
            .fontSize(ConfigData.VP_26)
            .fontWeight(500)
            .margin({left: ConfigData.VP_26, top: ConfigData.VP_10})
            .width(ConfigData.WH_90_100)
          Text(this.testItem.detail)
            .fontSize(ConfigData.VP_17)
            .fontWeight(500)
            .margin({top: ConfigData.VP_8})
            .width(ConfigData.WH_80_100)
        }
        .width(ConfigData.WH_80_100)
        .padding({left: ConfigData.VP_20})
      }
    }
    .backgroundColor($r('app.color.color_FFEDF2F5_bg'))
    .height(ConfigData.VP_160)
  }
}