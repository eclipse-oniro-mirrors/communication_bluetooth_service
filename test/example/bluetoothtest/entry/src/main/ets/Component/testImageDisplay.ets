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
/**
 * Test-ImageDisplay of bluetooth test
 */
@Component
export struct TestImageDisplay {
  private testItem!: TestData

  build() {
    Stack({ alignContent : Alignment.BottomStart }) {
      Flex({ alignItems : ItemAlign.Start }) {
        Stack() {
          Image(this.testItem.image)
            .height($r('app.float.wh_value_80'))
            .width($r('app.float.wh_value_80'))
            .margin({ left : $r('app.float.wh_value_10') , bottom : $r('app.float.wh_value_10') })
        }
        .width($r('app.float.wh_value_20p'))

        Column() {
          Text(this.testItem.name)
            .fontSize($r('app.float.wh_value_26'))
            .fontWeight($r('app.float.wh_value_500'))
            .margin({ left : $r('app.float.wh_value_26') , top : $r('app.float.wh_value_10') })
            .width($r('app.float.wh_value_90p'))
          Text(this.testItem.detail)
            .fontSize($r('app.float.wh_value_17'))
            .fontWeight($r('app.float.wh_value_500'))
            .margin({ top : $r('app.float.wh_value_8') })
            .width($r('app.float.wh_value_80p'))
        }
        .width($r('app.float.wh_value_80p'))
        .padding({ left : $r('app.float.wh_value_20') })
      }
    }
    .backgroundColor($r('app.color.color_FFEDF2F5_bg'))
    .height($r('app.float.wh_value_160'))
  }
}