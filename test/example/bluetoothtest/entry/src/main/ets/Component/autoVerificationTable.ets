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
            .height('30vp')
            .width('30vp')
        }
        .layoutWeight(1)
        .padding({ left : '10vp' })

        Flex({ direction : FlexDirection.Column , alignItems : ItemAlign.Start }) {
          Text(apiItem.name)
            .fontSize('17.4vp')
            .fontWeight(FontWeight.Bold)
            .layoutWeight(1)
            .padding({ left : '10vp' , top : '15vp' })
          Text(apiItem.detail)
            .fontSize('17.4vp')
            .fontWeight(FontWeight.Bold)
            .layoutWeight(1)
            .padding({ left : '10vp' , top : '5vp' })
        }
        .layoutWeight(6)

        Flex({ alignItems : ItemAlign.Center }) {
          Text(this.changeIndex >= 0 ? this.getCurrentState(index) : apiItem.result)
            .fontSize('17.4vp')
            .flexGrow(1)
        }
        .layoutWeight(3)
        .padding({ left : '10vp' })
      }
    }
    .height('80vp')
    .borderRadius('10vp')
    .margin({ top : '5vp' , bottom : '5vp' })
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
      .scrollBarWidth('20vp')
    }
    .height('88%')
    .padding({ top : '5vp' , right : '10vp' , left : '10vp' })
  }
}