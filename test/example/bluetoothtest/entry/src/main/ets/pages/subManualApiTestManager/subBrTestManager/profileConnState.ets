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
import { PageTitle } from '../../../Component/pageTitle';
import { TitleComponent } from '../../../Component/titleComponent';

/**
 * about phone
 */
@Entry
@Component
struct profileConnState {
  @State message: string = "请点击选择PROFILE TYPE";
  @State currentClick: number = - 1;
  @State profileId: number = - 1;
  @State profileType: string = '';

  build() {
    Column() {
      PageTitle({ detail : '选择PROFILE ID' })
      Column() {
        List() {
          ListItem() {
            TitleComponent({
              title : "SELECT_PROFILE_ID" ,
              bgColor : $r("app.color.moon") ,
              fontSize : $r('app.float.wh_value_17') ,
            })
          }
          .height($r('app.float.wh_value_80'))
          .borderRadius($r('app.float.wh_value_10'))

          ListItem() {
            TitleComponent({
              title : "A2DP_SOURCE" ,
              fontSize : $r('app.float.wh_value_15') ,
              bgColor : this.currentClick === 0 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
            });
          }
          .padding({ top : $r('app.float.wh_value_10') })
          .height($r('app.float.wh_value_80'))
          .borderRadius($r('app.float.wh_value_10'))
          .onClick(() => {
            this.currentClick = 0;
            this.profileId = 1;
          })

          ListItem() {
            TitleComponent({
              title : "HANDS_FREE_AUDIO_GATEWAY" ,
              fontSize : $r('app.float.wh_value_15') ,
              bgColor : this.currentClick === 1 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
            })
          }
          .padding({ top : $r('app.float.wh_value_20') })
          .height($r('app.float.wh_value_80'))
          .borderRadius($r('app.float.wh_value_10'))
          .onClick(() => {
            this.currentClick = 1;
            this.profileId = 4;
          })

          ListItem() {
            TitleComponent({
              title : "HID_HOST" ,
              fontSize : $r('app.float.wh_value_15') ,
              bgColor : this.currentClick === 2 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
            });
          }
          .padding({ top : $r('app.float.wh_value_20') })
          .height($r('app.float.wh_value_80'))
          .borderRadius($r('app.float.wh_value_10'))
          .onClick(() => {
            this.currentClick = 2;
            this.profileId = 6;
          })
        }
        .height($r('app.float.wh_value_400'))
      }

      Text(this.message)
        .textAlign(TextAlign.Center)
        .fontSize('17fp')
        .height($r('app.float.wh_value_60'))
        .width($r('app.float.wh_value_100p'))
        .backgroundColor($r("app.color.moon"))

      Row() {
        Column() {
          Text("取消")
            .textAlign(TextAlign.Center)
            .fontSize('17fp')
            .height($r('app.float.wh_value_60'))
            .width($r('app.float.wh_value_100'))
            .borderRadius($r('app.float.wh_value_10'))
            .borderColor($r("app.color.moon"))
            .backgroundColor($r("app.color.white"))
        }
        .padding({ right : $r('app.float.wh_value_10') })

        Column() {
          Text("确认")
            .textAlign(TextAlign.Center)
            .fontSize('17fp')
            .height($r('app.float.wh_value_60'))
            .width($r('app.float.wh_value_100'))
            .borderRadius($r('app.float.wh_value_10'))
            .borderColor($r("app.color.moon"))
            .backgroundColor($r("app.color.white"))
        }
        .padding({ left : $r('app.float.wh_value_50') })
      }
      .width($r('app.float.wh_value_80p'))
    }
    .padding({ left : $r('app.float.distance_24') , right : $r('app.float.distance_24') })
    .width($r('app.float.wh_value_100p'))
    .height(ConfigData.WH_100_100)
    .backgroundColor($r("app.color.lead"))
  }
}


@Component
struct ButtonComponent {
  @State isTouched: boolean = false;
  private text;
  private isOnClick = () => {};

  build() {
    Column() {
      Text(this.text)
        .fontSize($r('app.float.font_16'))
        .fontWeight(FontWeight.Medium)
        .lineHeight($r('app.float.wh_value_22'))
        .fontColor($r('app.color.font_color_007DFF'))
        .textAlign(TextAlign.Center)
        .width($r('app.float.wh_value_100'))
        .height($r("app.float.wh_value_40"))
        .borderRadius($r('app.float.radius_20'))
        .backgroundColor(!this.isTouched ? $r("app.color.color_button_grey") : $r("sys.color.ohos_id_color_foreground_contrary"))
        .onTouch((event: TouchEvent) => {
          if ( event.type === TouchType.Down ) {
            this.isTouched = true;
          }
          if ( event.type === TouchType.Up ) {
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
