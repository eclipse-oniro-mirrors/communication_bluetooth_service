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
import { resStr } from '../../../Utils/resString';


import hilog from '@ohos.hilog';
import ConfigData from '../../../Utils/ConfigData';
import BluetoothModel from '../../../MainAbility/model/BluetoothModel';
import router from '@ohos.router'
import { TitleComponent } from '../../../Component/titleComponent';
import InputMethod from '@ohos.inputMethod';
import bluetooth from '@ohos.bluetooth';
import { PageTitle } from '../../../Component/pageTitle';

/**
 * setScanMode page of bluetooth test
 */

@Entry
@Component
struct etScanMode {
  @State message: Resource | string = $r('app.string.bluetoothtest_str_qdjxzsm');
  private duration: string = '0';
  private scanMode: number = 0;
  @State currentClick: number = - 1;

  onPageShow(): void {
    this.currentClick = bluetooth.getBluetoothScanMode();
  }

  build() {
    Column() {
      PageTitle({ detail : $r('app.string.bluetoothtest_str_szsmlx') })
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_setScanMode__pt_0')
      Scroll() {
        Column() {
          List() {
            ListItem() {
              TitleComponent({
                title : "SCAN_MODE_SELECT" ,
                bgColor : $r('app.color.moon') ,
                fontSize : ConfigData.VP_17 ,
              })
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_setScanMode__tt_0');
            }
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_setScanMode__li_0')
            .height(ConfigData.VP_80)
            .borderRadius(ConfigData.VP_10)

            ListItem() {
              TitleComponent({
                title : "SCAN_MODE_NONE" ,
                fontSize : ConfigData.VP_15 ,
                bgColor : this.currentClick === 0 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
              })
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_setScanMode__tt_1');
            }
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_setScanMode__li_1')
            .padding({ top : ConfigData.VP_10 })
            .height(ConfigData.VP_80)
            .borderRadius(ConfigData.VP_10)
            .onClick(() => {
              this.currentClick = 0;
            })

            ListItem() {
              TitleComponent({
                title : "CONNECTABLE" ,
                fontSize : ConfigData.VP_15 ,
                bgColor : this.currentClick === 1 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
              })
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_setScanMode__tt_2');
            }
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_setScanMode__li_2')
            .padding({ top : ConfigData.VP_10 })
            .height(ConfigData.VP_80)
            .borderRadius(ConfigData.VP_10)
            .onClick(() => {
              this.currentClick = 1;
            })

            ListItem() {
              TitleComponent({
                title : "GENERAL_DISCOVERABLE" ,
                fontSize : ConfigData.VP_15 ,
                bgColor : this.currentClick === 2 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
              })
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_setScanMode__tt_3');
            }
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_setScanMode__li_3')
            .padding({ top : ConfigData.VP_10 })
            .height(ConfigData.VP_80)
            .borderRadius(ConfigData.VP_10)
            .onClick(() => {
              this.currentClick = 2;
            })

            ListItem() {
              TitleComponent({
                title : "LIMITED_DISCOVERABLE" ,
                fontSize : ConfigData.VP_15 ,
                bgColor : this.currentClick === 3 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
              })
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_setScanMode__tt_4');
            }
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_setScanMode__li_4')
            .padding({ top : ConfigData.VP_10 })
            .height(ConfigData.VP_80)
            .borderRadius(ConfigData.VP_10)
            .onClick(() => {
              this.currentClick = 3;
            })

            ListItem() {
              TitleComponent({
                title : "CONNECTABLE_GENERAL_DISCOVERABLE" ,
                fontSize : ConfigData.VP_15 ,
                bgColor : this.currentClick === 4 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
              })
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_setScanMode__tt_5');
            }
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_setScanMode__li_5')
            .padding({ top : ConfigData.VP_10 })
            .height(ConfigData.VP_80)
            .borderRadius(ConfigData.VP_10)
            .onClick(() => {
              this.currentClick = 4;
            })

            ListItem() {
              TitleComponent({
                title : "CONNECTABLE_LIMITED_DISCOVERABLE" ,
                fontSize : ConfigData.VP_15 ,
                bgColor : this.currentClick === 5 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
              })
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_setScanMode__tt_6');
            }
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_setScanMode__li_6')
            .padding({ top : ConfigData.VP_10 })
            .height(ConfigData.VP_80)
            .borderRadius(ConfigData.VP_10)
            .onClick(() => {
              this.currentClick = 5;
            })
          }

          Blank()
        }
      }
      .accessibilityLevel('yes')
      .id('pages_subManualApiTest_subBrTest_setScanMode__sc_0')
      .height(ConfigData.WH_50_100)
      .scrollable(ScrollDirection.Vertical)
      .scrollBar(BarState.On)
      .scrollBarColor(Color.Gray)
      .scrollBarWidth(ConfigData.VP_30)

      Row() {
        Text($r('app.string.bluetoothtest_text_d'))
          .textAlign(TextAlign.Center)
          .fontSize(ConfigData.FP_20)
          .fontColor($r('app.color.font_color_182431'))
          .fontWeight(FontWeight.Medium)
        TextInput({ text : this.duration })
          .accessibilityLevel('yes')
          .id('pages_subManualApiTest_subBrTest_setScanMode__ti_0')
          .placeholderFont({
            size : ConfigData.FP_18 ,
            weight : FontWeight.Normal ,
            style : FontStyle.Normal
          })
          .type(InputType.Normal)
          .enterKeyType(EnterKeyType.Done)
          .caretColor(Color.Blue)
          .height(ConfigData.VP_40)
          .borderRadius(ConfigData.VP_0)
          .backgroundColor($r('app.color.color_00000000_transparent'))
          .onChange((value: string) => {
            hilog.info(0x0000, 'BtTest', '%{public}s', ConfigData.TAG + 'device name changed to: ' + JSON.stringify(value));
            this.duration = value;
          })
          .onSubmit((enterKey) => {
            InputMethod.getInputMethodController().stopInput().then((ret) => {
              hilog.info(0x0000, 'BtTest', '%{public}s', `${ ConfigData.TAG }, enterType: ${ enterKey }, stopInput: ${ ret }`);
            });
          })
      }
      .padding(ConfigData.VP_15)
      .width(ConfigData.WH_100_100)

      Text(this.message)
        .textAlign(TextAlign.Center)
        .fontSize(ConfigData.FP_17)
        .height(ConfigData.VP_50)
        .width(ConfigData.WH_100_100)
        .fontWeight(FontWeight.Medium)
        .backgroundColor($r("app.color.moon"))

      Flex({ direction : FlexDirection.Row , justifyContent : FlexAlign.Center , alignItems : ItemAlign.Center }) {
        Row() {
          ButtonComponent({ text : $r("app.string.cancel") , isOnClick : () => {
            router.back();
          } })
        .accessibilityLevel('yes')
        .id('pages_subManualApiTest_subBrTest_setScanMode__bcomp_0')
        }
        .width(ConfigData.VP_120)
        .margin({ right : ConfigData.VP_12 })

        Row() {
          ButtonComponent({ text : $r("app.string.confirm") , isOnClick : () => {
            if ( this.currentClick === - 1 ) {
              this.message = resStr($r('app.string.bluetoothtest_str_qdjxzsm'));
              return;
            }
            this.message = '';
            let dur = parseInt(this.duration);
            let parseRet = isNaN(dur);
            let ret = BluetoothModel.setBluetoothScanMode(this.currentClick , !parseRet ? dur : 0);
            AppStorage.setOrCreate('setScanModeRet' , ret);
            router.back();
          } })
    .accessibilityLevel('yes')
    .id('pages_subManualApiTest_subBrTest_setScanMode__bcomp_1')
        }
          .width(ConfigData.VP_120)

      }
    }
    .padding({ left : ConfigData.VP_24 , right : ConfigData.VP_24 })
    .width(ConfigData.WH_100_100)
    .height(ConfigData.WH_100_100)
    .backgroundColor($r("app.color.lead"))
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
    .align(Alignment.Center);
  }
}
