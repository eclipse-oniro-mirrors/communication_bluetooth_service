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

import LogUtil from '../../../Utils/LogUtil';
import ConfigData from '../../../Utils/ConfigData';
import BluetoothModel from '../../../MainAbility/model/BluetoothModel';
import router from '@ohos.router';
import InputMethod from '@ohos.inputMethod';
import { PageTitle} from '../../../component/pageTitle';

/**
 * deviceName page of bluetooth test
 */

@Entry
@Component
struct DeviceNameCard {
  private deviceName: string = BluetoothModel.getLocalName();
  private deviceInfo: string = ''

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
            PageTitle({detail: $r('app.string.bluetoothtest_str_sbmc')})
              .accessibilityLevel('yes')
              .id('pages_subManualApiTest_subBrTest_deviceName__pt_0')

            TextInput({text: this.deviceName})
              .accessibilityLevel('yes')
              .id('pages_subManualApiTest_subBrTest_deviceName__ti_0')
              .placeholderFont({
                size: ConfigData.FP_18,
                weight: FontWeight.Normal,
                style: FontStyle.Normal
              })
              .type(InputType.Normal)
              .enterKeyType(EnterKeyType.Done)
              .caretColor(Color.Blue)
              .padding({
                left: $r('sys.float.ohos_id_card_margin_start'),
                right: $r('sys.float.ohos_id_card_margin_end')
              })
              .height(ConfigData.VP_40)
              .borderRadius(ConfigData.VP_0)
              .backgroundColor($r('app.color.color_00000000_transparent'))
              .onChange((value: string) => {
                LogUtil.info(ConfigData.TAG + 'device name changed to: ' + JSON.stringify(value));
                this.deviceName = value;
              })
              .onSubmit((enterKey) => {
                InputMethod.getInputMethodController().stopInput().then((ret) => {
                  LogUtil.info(`${ConfigData.TAG }, enterType: ${enterKey },
                    stopInput: ${ret }`);
              });
              })
            Divider()
              .padding({
                left: $r('sys.float.ohos_id_card_margin_start'),
                right: $r('sys.float.ohos_id_card_margin_end')
              })

            Column() {
              Blank()
            }
              .height(ConfigData.WH_25_100)

            Flex({
              direction: FlexDirection.Row,
              justifyContent: FlexAlign.Center,
              alignItems: ItemAlign.Center
            }) {
              Row() {
                ButtonComponent({text: $r("app.string.cancel"), isOnClick: () => {
                  router.back();
                }})
              .accessibilityLevel('yes')
              .id('pages_subManualApiTest_subBrTest_deviceName__bcomp_0')
              }
              .width(ConfigData.VP_120)
              .margin({right: ConfigData.VP_12})

              Row() {
                ButtonComponent({text: $r("app.string.confirm"), isOnClick: () => {
                  let ret = BluetoothModel.setLocalName(this.deviceName);
                  AppStorage.setOrCreate('setLocalNameRet', ret);
                  router.back();
                }})
          .accessibilityLevel('yes')
          .id('pages_subManualApiTest_subBrTest_deviceName__bcomp_1')
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

          Column() {
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.WH_100_100)
          .useSizeType({
            xs: {
              span: 0,
              offset: 12
            },
            sm: {
              span: 0,
              offset: 12
            },
            md: {
              span: 0,
              offset: 12
            },
            lg: {
              span: 2,
              offset: 10
            }
          })
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
  private text: string | Resource = 'defaultText';
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
