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
 * Test-ListItem of bluetooth test
 */
@Component
export struct TestListItem {
  private testItem!: TestData


  aboutToAppear() {
    console.info("TestListItem testItem:" + JSON.stringify(this.testItem))
  }

  build() {
    Navigator({ target : this.testItem.url ? this.testItem.url : "" }) {
      Flex({ justifyContent : FlexAlign.Start , alignItems : ItemAlign.Center }) {
        Row() {
          Image(this.testItem.image)
            .objectFit(ImageFit.Contain)
            .autoResize(false)
            .height($r('app.float.wh_value_60'))
            .width($r('app.float.wh_value_60'))
        }
        .backgroundColor($r("app.color.white"))
        .width($r('app.float.wh_value_30p'))

        Text(this.testItem.name)
          .fontSize($r('app.float.wh_value_20'))
          .flexGrow($r('app.float.wh_value_1'))
          .margin({ left : $r('app.float.wh_value_12') })
          .width($r('app.float.wh_value_45p'))
        Text(this.testItem.detail)
          .fontSize($r('app.float.wh_value_17'))
          .margin({ left : $r('app.float.wh_value_10') , right : $r('app.float.wh_value_5') })
          .width($r('app.float.wh_value_45p'))
      }
      .height($r('app.float.wh_value_95'))
    }
    .params({ testItem : this.testItem })
    .margin({ right : $r('app.float.wh_value_10') , left : $r('app.float.wh_value_10') , top : $r('app.float.wh_value_10') })
    .padding({ left : $r('app.float.wh_value_15') , right : $r('app.float.wh_value_15') })
    .borderRadius($r('app.float.wh_value_10'))
    .backgroundColor($r("app.color.white"))
  }
}
