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
  @State message: string = "请点击选择SCAN MODE";
  private duration: string = '0';
  private scanMode: number = 0;
  @State currentClick: number = - 1;

  onPageShow(): void {
    this.currentClick = bluetooth.getBluetoothScanMode();
  }

  build() {
    Column() {
      PageTitle({ detail : '设置扫描类型' })
      Scroll() {
        Column() {
          List() {
            ListItem() {
              TitleComponent({
                title : "SCAN_MODE_SELECT" ,
                bgColor : $r('app.color.moon') ,
                fontSize : '17vp' ,
              });
            }
            .height('80vp')
            .borderRadius('10vp')

            ListItem() {
              TitleComponent({
                title : "SCAN_MODE_NONE" ,
                fontSize : '15vp' ,
                bgColor : this.currentClick === 0 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
              });
            }
            .padding({ top : '10vp' })
            .height('80vp')
            .borderRadius('10vp')
            .onClick(() => {
              this.currentClick = 0;
            })

            ListItem() {
              TitleComponent({
                title : "CONNECTABLE" ,
                fontSize : '15vp' ,
                bgColor : this.currentClick === 1 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
              });
            }
            .padding({ top : '10vp' })
            .height('80vp')
            .borderRadius('10vp')
            .onClick(() => {
              this.currentClick = 1;
            })

            ListItem() {
              TitleComponent({
                title : "GENERAL_DISCOVERABLE" ,
                fontSize : '15vp' ,
                bgColor : this.currentClick === 2 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
              });
            }
            .padding({ top : '10vp' })
            .height('80vp')
            .borderRadius('10vp')
            .onClick(() => {
              this.currentClick = 2;
            })

            ListItem() {
              TitleComponent({
                title : "LIMITED_DISCOVERABLE" ,
                fontSize : '15vp' ,
                bgColor : this.currentClick === 3 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
              });
            }
            .padding({ top : '10vp' })
            .height('80vp')
            .borderRadius('10vp')
            .onClick(() => {
              this.currentClick = 3;
            })

            ListItem() {
              TitleComponent({
                title : "CONNECTABLE_GENERAL_DISCOVERABLE" ,
                fontSize : '15vp' ,
                bgColor : this.currentClick === 4 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
              });
            }
            .padding({ top : '10vp' })
            .height('80vp')
            .borderRadius('10vp')
            .onClick(() => {
              this.currentClick = 4;
            })

            ListItem() {
              TitleComponent({
                title : "CONNECTABLE_LIMITED_DISCOVERABLE" ,
                fontSize : '15vp' ,
                bgColor : this.currentClick === 5 ? $r('app.color.font_color_007DFF') : $r('app.color.white_bg_color')
              });
            }
            .padding({ top : '10vp' })
            .height('80vp')
            .borderRadius('10vp')
            .onClick(() => {
              this.currentClick = 5;
            })
          }

          Blank()
        }
      }
      .height('50%')
      .scrollable(ScrollDirection.Vertical)
      .scrollBar(BarState.On)
      .scrollBarColor(Color.Gray)
      .scrollBarWidth('30vp')

      Row() {
        Text('duration')
          .textAlign(TextAlign.Center)
          .fontSize('20fp')
          .fontColor($r('app.color.font_color_182431'))
          .fontWeight(FontWeight.Medium)
        TextInput({ text : this.duration })
          .placeholderFont({
            size : '18fp' ,
            weight : FontWeight.Normal ,
            style : FontStyle.Normal
          })
          .type(InputType.Normal)
          .enterKeyType(EnterKeyType.Done)
          .caretColor(Color.Blue)
          .height('40vp')
          .borderRadius('0vp')
          .backgroundColor($r('app.color.color_00000000_transparent'))
          .onChange((value: string) => {
            console.log(ConfigData.TAG + 'device name changed to: ' + JSON.stringify(value));
            this.duration = value;
          })
          .onSubmit((enterKey) => {
            InputMethod.getInputMethodController().stopInput().then((ret) => {
              console.log(`${ ConfigData.TAG }, enterType: ${ enterKey }, stopInput: ${ ret }`);
            });
          })
      }
      .padding('15vp')
      .width('100%')

      Text(this.message)
        .textAlign(TextAlign.Center)
        .fontSize('17fp')
        .height('50vp')
        .width('100%')
        .fontWeight(FontWeight.Medium)
        .backgroundColor($r("app.color.moon"))

      Flex({ direction : FlexDirection.Row , justifyContent : FlexAlign.Center , alignItems : ItemAlign.Center }) {
        Row() {
          ButtonComponent({ text : $r("app.string.cancel") , isOnClick : () => {
            router.back();
          } })
        }
        .width('120vp')
        .margin({ right : '12vp' })

        Row() {
          ButtonComponent({ text : $r("app.string.confirm") , isOnClick : () => {
            if ( this.currentClick === - 1 ) {
              this.message = '请点击选择SCAN MODE';
              return;
            }
            this.message = '';
            let dur = parseInt(this.duration);
            let parseRet = isNaN(dur);
            let ret = BluetoothModel.setBluetoothScanMode(this.currentClick , !parseRet ? dur : 0);
            AppStorage.setOrCreate('setScanModeRet' , ret);
            router.back();
          } })
        }.width('120vp')

      }
    }
    .padding({ left : '24vp' , right : '24vp' })
    .width('100%')
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
        .fontSize('16fp')
        .fontWeight(FontWeight.Medium)
        .lineHeight('22vp')
        .fontColor($r('app.color.font_color_007DFF'))
        .textAlign(TextAlign.Center)
        .width('120vp')
        .height('40vp')
        .borderRadius('20vp')
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
