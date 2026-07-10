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

import LogUtil from '../Utils/LogUtil';
import ConfigData from '../Utils/ConfigData';
import screenshot from '@ohos.screenshot'
import image from '@ohos.multimedia.image'
import MediaUtils from '../MainAbility/model/MediaUtils'
import Logger from '../MainAbility/model/Logger'

const TAG = '[Screenshot]'

/**
 * Main-Page Title Component of bluetooth test
 */
@Component
export struct MainPageTitle {
  @State colOpacity: number = 1
  @State count: number = 0
  @State myborderWidth: number = 0
  @State myborderRadius: number = 0
  @State offsetX: number = 0
  @State offsetY: number = 0
  @State myscale: number = 1
  @State angle: number = 0
  getScreen = (isFullScreen: boolean) => {
    let screenshotOptions: screenshot.ScreenshotOptions = {
      screenRect: {
        left: 0,
        top: 0,
        width: ConfigData.SCREENSHOT_SIZE,
        height: ConfigData.SCREENSHOT_SIZE
      },
      imageSize: {
        width: ConfigData.SCREENSHOT_SIZE,
        height: ConfigData.SCREENSHOT_SIZE
      },
      rotation: 0,
      displayId: 0
    }
    if (isFullScreen) {
      screenshotOptions = {
        rotation: 0
      }
    }
    try {
      screenshot.save(screenshotOptions, (err, data: image.PixelMap) => {
        if (err) {
          Logger.info(TAG, `Failed to save the screenshot. Error:${JSON.stringify(err) }`)
        }
        Logger.info(TAG, 'save callback')
        MediaUtils.savePicture(data, getContext(this))
      })
    } catch(err) {
      Logger.error(`save failed, code is ${err.code }, message is ${err.message }`)
    }
  }

  @Builder
  CustomBtn(text: Resource, isFullScreen: boolean) {
    Button() {
      Image($r('app.media.screenShot'))
        .height(ConfigData.VP_50)
        .width(ConfigData.VP_50)
    }
    .key(isFullScreen + 'FullScreenShot')
    .type(ButtonType.Normal)
    .backgroundColor($r("app.color.blue"))
    .height(ConfigData.WH_100_100)
    .width(ConfigData.WH_100_100)
    .size({
      width: ConfigData.SCREENSHOT_BTN_SIZE,
      height: ConfigData.SCREENSHOT_BTN_SIZE
    })
    .margin({
      left: ConfigData.VP_6,
      right: ConfigData.VP_6,
      top: ConfigData.VP_3
    })
    .onClick(() => {
      LogUtil.info('[Screenshot] onClick, screenshot')
      this.getScreen(isFullScreen)
    })
  }

  build() {
    Flex({alignItems: ItemAlign.Start}) {
      Text($r('app.string.bluetoothtest_text_lytszs'))
        .fontSize(ConfigData.VP_26)
        .margin({left: ConfigData.VP_15})
        .padding({top: ConfigData.VP_28})

      Scroll() {
        Column() {
          this.CustomBtn($r('app.string.btn_fullscreen'), true)
        }
        .width(ConfigData.WH_100_100)
        .height(ConfigData.WH_100_100)
        .borderWidth(this.myborderWidth)
        .borderRadius(this.myborderRadius)
        .borderColor(Color.Green)
        .gesture(
          GestureGroup(GestureMode.Parallel,
            TapGesture({
              count: 2,
              fingers: 1
            })
              .onAction(() => {
                this.myborderWidth += 1
        }),
            LongPressGesture({
              fingers: 1,
              repeat: true,
              duration: ConfigData.LONG_PRESS_DURATION_MS
            })
              .onAction((event: GestureEvent) => {
                this.colOpacity -= 0.1
                if (event.repeat) {
                  this.count++
                }
              })
              .onActionEnd(() => {
                this.colOpacity = 1;
                this.count = 0;
        }),
            PanGesture({
              fingers: 1,
              direction: PanDirection.All,
              distance: ConfigData.PAN_GESTURE_DISTANCE
            })
              .onActionStart(() => {
                LogUtil.info('Pan start')
              })
              .onActionUpdate((event: GestureEvent) => {
                this.offsetY = event.offsetY;
                this.offsetX = event.offsetX;
              })
              .onActionEnd(() => {
                LogUtil.info('Pan end')
        }),
            PinchGesture({
              fingers: 2,
              distance: ConfigData.PINCH_GESTURE_DISTANCE
            })
              .onActionStart(() => {
                LogUtil.info('PinchGesture start')
              })
              .onActionUpdate((event: GestureEvent) => {
                this.myscale = event.scale
                this.myborderRadius = ConfigData.BORDER_RADIUS_10
              })
              .onActionEnd(() => {
                LogUtil.info('PinchGesture end')
                this.getScreen(true)
        }),
            RotationGesture({
              fingers: 2,
              angle: ConfigData.ROTATION_GESTURE_ANGLE
            })
              .onActionStart(() => {
                LogUtil.info('RotationGesture start')
              })
              .onActionUpdate((event: GestureEvent) => {
                this.angle = event.angle
              })
              .onActionEnd(() => {
                LogUtil.info('RotationGesture end')
              })
          )
            .onCancel(() => {
              LogUtil.info('Parallel gesture canceled')
            })
        )
      }
      .layoutWeight(ConfigData.LAYOUT_WEIGHT_1)
    }
    .height(ConfigData.VP_80)
    .backgroundColor($r("app.color.blue"))
    .padding({left: ConfigData.VP_20})
  }
}