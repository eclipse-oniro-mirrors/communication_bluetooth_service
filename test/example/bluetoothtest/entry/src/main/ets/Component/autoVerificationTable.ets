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

import { TestData , TestApi } from '../MainAbility/model/testData'
import promptAction from '@ohos.promptAction'

/**
 *  AutoVerificationTable Component Of Bluetooth test
 */

@Component
export struct AutoVerificationTable {
  @State apiItems: TestApi[] = [];
  @State changeIndex: number = - 1
  @State localName: string = 'DaYuBlue'
  @State currentIndex: number = 1
  @StorageLink('pairedMac') pairedMac: string = "";

  @Builder IngredientItem(apiItem: TestApi , index: number) {
    Stack() {
      Flex() {
        Flex({ alignItems : ItemAlign.Center }) {
          Image($r('app.media.bluetooth'))
            .objectFit(ImageFit.Contain)
            .autoResize(false)
            .height($r('app.float.wh_value_30'))
            .width($r('app.float.wh_value_30'))
        }
        .layoutWeight($r('app.float.wh_value_1'))
        .padding({ left : $r('app.float.wh_value_10') })

        Flex({ direction : FlexDirection.Column , alignItems : ItemAlign.Start }) {
          Text(apiItem.name)
            .fontSize($r('app.float.wh_value_17_4'))
            .fontWeight(FontWeight.Bold)
            .layoutWeight($r('app.float.wh_value_1'))
            .padding({ left : $r('app.float.wh_value_10') , top : $r('app.float.wh_value_15') })
          Text(apiItem.detail)
            .fontSize($r('app.float.wh_value_17_4'))
            .fontWeight(FontWeight.Bold)
            .layoutWeight($r('app.float.wh_value_1'))
            .padding({ left : $r('app.float.wh_value_10') , top : $r('app.float.wh_value_5') })
        }
        .layoutWeight($r('app.float.wh_value_6'))

        Flex({ alignItems : ItemAlign.Center }) {
          Text(this.changeIndex >= 0 ? this.getCurrentState(index) : apiItem.result)
            .fontSize($r('app.float.wh_value_17_4'))
            .flexGrow($r('app.float.wh_value_1'))
        }
        .layoutWeight($r('app.float.wh_value_3'))
        .padding({ left : $r('app.float.wh_value_10') })
      }
    }
    .height($r('app.float.wh_value_80'))
    .borderRadius($r('app.float.wh_value_10'))
    .margin({ top : $r('app.float.wh_value_5') , bottom : $r('app.float.wh_value_5') })
    .backgroundColor($r("app.color.white"))
    .onClick(() => {
      switch ( apiItem.method ) {
        default: {
          let message = apiItem.api();
          apiItem.result = message;
          this.currentIndex += 1;
          this.changeIndex = this.currentIndex;
          promptAction.showToast({ message : message });
        }
          break;
      }
    })
  }

  getCurrentState(index: number) {
    return this.apiItems[ index ].result
  }

  build() {
    Column() {
      Scroll() {
        Flex({
          direction : FlexDirection.Column ,
          justifyContent : FlexAlign.SpaceBetween ,
          alignItems : ItemAlign.Start
        }) {
          ForEach(this.apiItems , (item: TestApi , index: number) => {
            this.IngredientItem(item , index)
          })
        }
      }
      .scrollBarWidth($r('app.float.wh_value_20'))
    }
    .height($r('app.float.wh_value_88p'))
    .padding({ top : $r('app.float.wh_value_5') , right : $r('app.float.wh_value_10') , left : $r('app.float.wh_value_10') })
  }
}