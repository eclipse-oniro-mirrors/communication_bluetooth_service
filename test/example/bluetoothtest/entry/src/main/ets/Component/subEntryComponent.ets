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
    Navigator({ target : this.targetPage }) {
      Flex({ justifyContent : FlexAlign.SpaceBetween , alignItems : ItemAlign.Center }) {
        Row() {
          Text(this.title)
            .fontSize('30fp')
            .lineHeight('32vp')
            .fontColor($r('app.color.font_color_182431'))
            .fontWeight(FontWeight.Medium)
            .margin({ left : '8vp' })
            .textAlign(TextAlign.Start)
            .height('32vp');
        }

        Image(('app.media.ic_settings_arrow'))
          .width('12vp')
          .height('100vp')
          .margin({ right : '8vp' });
      }
      .height(ConfigData.WH_100_100)
      .width(ConfigData.WH_100_100)
      .borderRadius('12vp')
      .linearGradient(this.isTouched ? {
                                         angle : 90 ,
                                         direction : GradientDirection.Right ,
                                         colors : [ [ $r("app.color.DCEAF9"), 0.0 ], [ $r("app.color.FAFAFA"), 1.0 ] ]
                                       } : {
                                             angle : 90 ,
                                             direction : GradientDirection.Right ,
                                             colors : [ [ $r("sys.color.ohos_id_color_foreground_contrary"), 1 ], [ $r("sys.color.ohos_id_color_foreground_contrary"), 1 ] ]
                                           })
      .onTouch((event: TouchEvent) => {
        if ( event.type === TouchType.Down ) {
          this.isTouched = true;
        }
        if ( event.type === TouchType.Up ) {
          this.isTouched = false;
        }
      })
    }
    .params({ date : this.date , deviceId : this.deviceId })
    .padding('4vp')
    .height('100vp')
    .borderRadius('24vp')
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
    Navigator({ target : this.targetPage }) {
      Flex({ justifyContent : FlexAlign.SpaceBetween , alignItems : ItemAlign.Center }) {
        Row() {
          Text(this.title)
            .fontSize('16fp')
            .lineHeight('22vp')
            .fontWeight(FontWeight.Medium)
            .fontColor($r('app.color.font_color_182431'))
            .margin({ left : '8vp' })
            .textAlign(TextAlign.Start);
        }

        Row() {
          Text(this.endText)
            .fontSize('14fp')
            .lineHeight('19vp')
            .fontWeight(FontWeight.Regular)
            .fontColor($r('sys.color.ohos_id_color_text_secondary'))
            .margin({ right : '4vp' })
            .textAlign(TextAlign.End);
          Image(('app.media.ic_settings_arrow'))
            .width('12vp')
            .height('24vp')
            .margin({ right : '8vp' });
        }
      }
      .height(ConfigData.WH_100_100)
      .width(ConfigData.WH_100_100)
      .borderRadius('20vp')
      .linearGradient(this.isTouched ? {
                                         angle : 90 ,
                                         direction : GradientDirection.Right ,
                                         colors : [ [ $r("app.color.DCEAF9"), 0.0 ], [ $r("app.color.FAFAFA"), 1.0 ] ]
                                       } : {
                                             angle : 90 ,
                                             direction : GradientDirection.Right ,
                                             colors : [ [ $r("sys.color.ohos_id_color_foreground_contrary"), 1 ], [ $r("sys.color.ohos_id_color_foreground_contrary"), 1 ] ]
                                           })
      .onTouch((event: TouchEvent) => {
        if ( event.type === TouchType.Down ) {
          this.isTouched = true;
        }
        if ( event.type === TouchType.Up ) {
          this.isTouched = false;
        }
      });
    }
    .padding('4vp')
    .height('56vp')
    .borderRadius('24vp')
    .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"));
  }
}