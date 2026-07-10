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

/**
 * Sub-Page Entry Component of bluetooth test
 */
@Component
export struct SubEntryComponent {
  private targetPage: string = 'defaultPage'
  private title: string | Resource = 'defaultTitle';
  @State isTouched: boolean = false;
  private date: string = 'defaultDate';
  private deviceId: string = 'defaultDeviceId';

  build() {
    Navigator({target: this.targetPage}) {
      Flex({
        justifyContent: FlexAlign.SpaceBetween,
        alignItems: ItemAlign.Center
      }) {
        Row() {
          Text(this.title)
            .fontSize(ConfigData.FP_30)
            .lineHeight(ConfigData.VP_32)
            .fontColor($r('app.color.font_color_182431'))
            .fontWeight(FontWeight.Medium)
            .margin({left: ConfigData.VP_8})
            .textAlign(TextAlign.Start)
            .height(ConfigData.VP_32);
        }

        Image(('app.media.ic_settings_arrow'))
          .width(ConfigData.VP_12)
          .height(ConfigData.VP_100)
          .margin({right: ConfigData.VP_8});
      }
      .height(ConfigData.WH_100_100)
      .width(ConfigData.WH_100_100)
      .borderRadius(ConfigData.VP_12)
      .linearGradient(this.isTouched ? {
        angle: ConfigData.GRADIENT_ANGLE_90,
        direction: GradientDirection.Right,
        colors: [
          [$r("app.color.DCEAF9"), 0.0],
          [$r("app.color.FAFAFA"), 1.0]
        ]
      } : {
        angle: ConfigData.GRADIENT_ANGLE_90,
        direction: GradientDirection.Right,
        colors: [
          [$r("sys.color.ohos_id_color_foreground_contrary"), 1],
          [$r("sys.color.ohos_id_color_foreground_contrary"), 1]
        ]
      })
      .onTouch((event: TouchEvent) => {
        if (event.type === TouchType.Down) {
          this.isTouched = true;
        }
        if (event.type === TouchType.Up) {
          this.isTouched = false;
        }
      })
    }
    .params({
      date: this.date,
      deviceId: this.deviceId
    })
    .padding(ConfigData.VP_4)
    .height(ConfigData.VP_100)
    .borderRadius(ConfigData.VP_24)
    .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"));
  }
}

/**
 * Sub-Page Entry Component with EndText
 */
@Component
export struct SubEntryComponentWithEndText {
  @State isTouched: boolean = false;
  @Prop endText: string;
  private targetPage: string = 'defaultPage'
  private title: string | Resource = 'defaultTitle'

  build() {
    Navigator({target: this.targetPage}) {
      Flex({
        justifyContent: FlexAlign.SpaceBetween,
        alignItems: ItemAlign.Center
      }) {
        Row() {
          Text(this.title)
            .fontSize(ConfigData.FP_16)
            .lineHeight('22vp')
            .fontWeight(FontWeight.Medium)
            .fontColor($r('app.color.font_color_182431'))
            .margin({left: ConfigData.VP_8})
            .textAlign(TextAlign.Start);
        }

        Row() {
          Text(this.endText)
            .fontSize(ConfigData.FP_14)
            .lineHeight('19vp')
            .fontWeight(FontWeight.Regular)
            .fontColor($r('sys.color.ohos_id_color_text_secondary'))
            .margin({right: ConfigData.VP_4})
            .textAlign(TextAlign.End);
          Image(('app.media.ic_settings_arrow'))
            .width(ConfigData.VP_12)
            .height(ConfigData.VP_24)
            .margin({right: ConfigData.VP_8});
        }
      }
      .height(ConfigData.WH_100_100)
      .width(ConfigData.WH_100_100)
      .borderRadius(ConfigData.VP_20)
      .linearGradient(this.isTouched ? {
        angle: ConfigData.GRADIENT_ANGLE_90,
        direction: GradientDirection.Right,
        colors: [
          [$r("app.color.DCEAF9"), 0.0],
          [$r("app.color.FAFAFA"), 1.0]
        ]
      } : {
        angle: ConfigData.GRADIENT_ANGLE_90,
        direction: GradientDirection.Right,
        colors: [
          [$r("sys.color.ohos_id_color_foreground_contrary"), 1],
          [$r("sys.color.ohos_id_color_foreground_contrary"), 1]
        ]
      })
      .onTouch((event: TouchEvent) => {
        if (event.type === TouchType.Down) {
          this.isTouched = true;
        }
        if (event.type === TouchType.Up) {
          this.isTouched = false;
        }
      });
    }
    .padding(ConfigData.VP_4)
    .height(ConfigData.VP_56)
    .borderRadius(ConfigData.VP_24)
    .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"));
  }
}