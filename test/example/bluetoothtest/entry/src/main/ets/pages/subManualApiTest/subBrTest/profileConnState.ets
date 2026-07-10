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

import ConfigData from '../../../Utils/ConfigData';
import { PageTitle} from '../../../component/pageTitle';
import { TitleComponent} from '../../../component/titleComponent';

/**
 * profileConnState page of bluetooth test
 */

@Entry
@Component
struct ProfileConnState {
  @State message: Resource | string = $r('app.string.bluetoothtest_str_qdjxzpt');
  @State currentClick: number = - 1;
  @State profileId: number = - 1;
  @State profileType: string = '';

  build() {
    Column() {
      PageTitle({detail: $r('app.string.bluetoothtest_str_xzpi')})
          .accessibilityLevel('yes')
          .id('pages_subManualApiTest_subBrTest_profileConnState__pt_0')
      Column() {
        List() {
          ListItem() {
            TitleComponent({
              title: "SELECT_PROFILE_ID",
              bgColor: $r("app.color.moon"),
              fontSize: ConfigData.VP_17,
            })
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_profileConnState__tt_0')
          }
          .accessibilityLevel('yes')
          .id('pages_subManualApiTest_subBrTest_profileConnState__li_0')
          .height(ConfigData.VP_80)
          .borderRadius(ConfigData.VP_10)

          ListItem() {
            TitleComponent({
              title: "A2DP_SOURCE",
              fontSize: ConfigData.VP_15,
              bgColor: this.currentClick === 0 ? $r('app.color.font_color_007DFF') :
                $r('app.color.white_bg_color')
            })
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_profileConnState__tt_1');
          }
          .accessibilityLevel('yes')
          .id('pages_subManualApiTest_subBrTest_profileConnState__li_1')
          .padding({top: ConfigData.VP_10})
          .height(ConfigData.VP_80)
          .borderRadius(ConfigData.VP_10)
          .onClick(() => {
            this.currentClick = 0;
            this.profileId = 1;
          })

          ListItem() {
            TitleComponent({
              title: "HANDS_FREE_AUDIO_GATEWAY",
              fontSize: ConfigData.VP_15,
              bgColor: this.currentClick === 1 ? $r('app.color.font_color_007DFF') :
                $r('app.color.white_bg_color')
            })
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_profileConnState__tt_2')
          }
          .accessibilityLevel('yes')
          .id('pages_subManualApiTest_subBrTest_profileConnState__li_2')
          .padding({top: ConfigData.VP_20})
          .height(ConfigData.VP_80)
          .borderRadius(ConfigData.VP_10)
          .onClick(() => {
            this.currentClick = 1;
            this.profileId = 4;
          })

          ListItem() {
            TitleComponent({
              title: "HID_HOST",
              fontSize: ConfigData.VP_15,
              bgColor: this.currentClick === 2 ? $r('app.color.font_color_007DFF') :
                $r('app.color.white_bg_color')
            })
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_profileConnState__tt_3');
          }
          .accessibilityLevel('yes')
          .id('pages_subManualApiTest_subBrTest_profileConnState__li_3')
          .padding({top: ConfigData.VP_20})
          .height(ConfigData.VP_80)
          .borderRadius(ConfigData.VP_10)
          .onClick(() => {
            this.currentClick = 2;
            this.profileId = 6;
          })
        }
        .height(ConfigData.VP_400)
      }

      Text(this.message)
        .textAlign(TextAlign.Center)
        .fontSize(ConfigData.FP_17)
        .height(ConfigData.VP_60)
        .width(ConfigData.WH_100_100)
        .backgroundColor($r("app.color.moon"))


      Row() {
        Column() {
          Text($r('app.string.cancel'))
            .textAlign(TextAlign.Center)
            .fontSize(ConfigData.FP_17)
            .height(ConfigData.VP_60)
            .width(ConfigData.VP_100)
            .borderRadius(ConfigData.VP_10)
            .borderColor($r("app.color.moon"))
            .backgroundColor($r("app.color.white"))
        }
        .padding({right: ConfigData.VP_10})

        Column() {
          Text($r('app.string.confirm'))
            .textAlign(TextAlign.Center)
            .fontSize(ConfigData.FP_17)
            .height(ConfigData.VP_60)
            .width(ConfigData.VP_100)
            .borderRadius(ConfigData.VP_10)
            .borderColor($r("app.color.moon"))
            .backgroundColor($r("app.color.white"))
        }
        .padding({left: ConfigData.VP_50})

      }
      .width(ConfigData.WH_80_100)
    }
    .padding({
      left: ConfigData.VP_24,
      right: ConfigData.VP_24
    })
    .width(ConfigData.WH_100_100)
    .height(ConfigData.WH_100_100)
    .backgroundColor($r("app.color.lead"))
  }
}

@Component
struct ButtonComponent {
  @State isTouched: boolean = false;
  private text: string = 'defaultText';
  private isOnClick = () => {
  };

  build() {
    Column() {
      Text(this.text)
        .fontSize(ConfigData.FP_16)
        .fontWeight(FontWeight.Medium)
        .lineHeight('22vp')
        .fontColor($r('app.color.font_color_007DFF'))
        .textAlign(TextAlign.Center)
        .width(ConfigData.VP_100)
        .height(ConfigData.VP_40)
        .borderRadius(ConfigData.VP_20)
        .backgroundColor(!this.isTouched ? $r("app.color.color_button_grey") :
          $r("sys.color.ohos_id_color_foreground_contrary"))
        .onTouch((event: TouchEvent) => {
          if (event.type === TouchType.Down) {
            this.isTouched = true;
          }
          if (event.type === TouchType.Up) {
            this.isTouched = false;
          }
        })
        .onClick(() => {
          this.isOnClick();
        })
    }
    .width(ConfigData.WH_100_100)
  }
}
