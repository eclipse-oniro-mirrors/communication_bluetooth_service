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
import router from '@ohos.router'
import { TitleComponent} from '../../../component/titleComponent';
import { PageTitle} from '../../../component/pageTitle';
/**
 * about phone
 */
@Entry
@Component
struct pairedDevices {
  @State message: string = "";
  private duration: string = '0';
  private scanMode: number = 0;
  @StorageLink('pairedDevices') pairedDevices: string[] = [];

  build() {
    Column() {
      GridContainer({
        columns: 12,
        sizeType: SizeType.Auto,
        gutter: vp2px(1) === 2 ? ConfigData.VP_12: ConfigData.VP_0,
        margin: vp2px(1) === 2 ? ConfigData.VP_24: ConfigData.VP_0
      }) {
        Row({}) {
          Column() {
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.WH_100_100)
          .useSizeType({
            xs: {
              span: 0,
              offset: 0
            },
            sm: {
              span: 0,
              offset: 0
            },
            md: {
              span: 0,
              offset: 0
            },
            lg: {
              span: 2,
              offset: 0
            }
          });

          Column() {
            PageTitle({detail: $r('app.string.bluetoothtest_str_ypdsb')})


            Divider()
              .padding({
                left: $r('sys.float.ohos_id_card_margin_start'),
                right: $r('sys.float.ohos_id_card_margin_end')
              })

            Column() {
              List() {
                ForEach(this.pairedDevices, (item: string, index: number) => {
                  ListItem() {
                    TitleComponent({title: item});
                  }
                    .padding({
                      top: ConfigData.VP_2,
                      bottom: ConfigData.VP_2
                    })
                }, item => JSON.stringify(item));
              }

              Blank()
            }

            Flex({
              direction: FlexDirection.Row,
              justifyContent: FlexAlign.Center,
              alignItems: ItemAlign.Center
            }) {
              Row() {
                ButtonComponent({text: $r("app.string.cancel"), isOnClick: () => {
                  router.back();
                }})
              }
              .width(ConfigData.VP_120)
              .margin({right: ConfigData.VP_12})

              Row() {
                ButtonComponent({text: $r("app.string.confirm"), isOnClick: () => {
                  router.back();
                }})
              }
                .width(ConfigData.VP_120)
            }
          }
          .padding({
            left: ConfigData.VP_24,
            right: ConfigData.VP_24
          })
          .width(ConfigData.WH_100_100)
          .height(ConfigData.WH_100_100)
          .useSizeType({
            xs: {
              span: 12,
              offset: 0
            },
            sm: {
              span: 12,
              offset: 0
            },
            md: {
              span: 12,
              offset: 0
            },
            lg: {
              span: 8,
              offset: 2
            }
          });
        }
        .width(ConfigData.WH_100_100)
        .height(ConfigData.WH_100_100);
      }
      .width(ConfigData.WH_100_100)
      .height(ConfigData.WH_100_100);
    }
    .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
    .width(ConfigData.WH_100_100)
    .height(ConfigData.WH_100_100);
  }
}


@Component
struct ButtonComponent {
  @State isTouched: boolean = false;
  private text;
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
        .width(ConfigData.VP_120)
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
    .align(Alignment.Center);
  }
}
