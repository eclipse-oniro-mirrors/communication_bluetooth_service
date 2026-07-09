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
import router from '@ohos.router'
/**
 * Test-GridItem of bluetooth test
 */
@Component
export struct TestGridItem {
  private testItem: TestData

  build() {
    Column() {
      Row() {
        Image(this.testItem.image)
          .objectFit(ImageFit.Contain)
          .height(ConfigData.VP_152)
          .width(ConfigData.WH_100_100)
      }
        .backgroundColor($r('app.color.color_FFF1F3F5_bg'))

      Flex({
        justifyContent: FlexAlign.Start,
        alignItems: ItemAlign.Center
      }) {
        Text(this.testItem.name)
          .fontSize(ConfigData.VP_14)
          .flexGrow(ConfigData.LAYOUT_WEIGHT_1)
          .padding({left: ConfigData.VP_8})
        Text(this.testItem.detail)
          .fontSize(ConfigData.VP_14)
          .margin({right: ConfigData.VP_6})
      }
      .height(ConfigData.VP_32)
      .width(ConfigData.WH_100_100)
      .backgroundColor($r('app.color.color_FFE5E5E5_bg'))
    }
    .height(ConfigData.VP_184)
    .width(ConfigData.WH_100_100)
    .onClick(() => {
      router.pushUrl({
        url: 'MainAbility/pagesTestDetail',
        params: { testId: this.testItem}
      })
    })
  }
}