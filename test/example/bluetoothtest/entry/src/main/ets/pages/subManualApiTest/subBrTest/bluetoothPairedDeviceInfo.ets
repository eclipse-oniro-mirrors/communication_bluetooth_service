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
import HeadTableComponent from '../../../Component/headerComponent';
import router from '@ohos.router';
import LogUtil from '../../../Utils/LogUtil';
import { BluetoothDevice } from '../../../MainAbility/model/BluetoothModel';
import BluetoothDeviceController from '../../../MainAbility/controller/BluetoothDeviceController'

let param;

/**
 * BluetoothPairedDeviceInfo Page of Bluetooth test
 */

@Entry
@Component
struct BluetoothPairedDeviceInfo {
  @State bluetoothDevice: BluetoothDevice = undefined;
  private controller: BluetoothDeviceController = new BluetoothDeviceController()
  @State isTouched: boolean = false;

  aboutToAppear() {
    param = router.getParams();
    this.bluetoothDevice = JSON.parse(param.bluetoothDevice);
    LogUtil.log(ConfigData.TAG + "aboutToAppear bluetoothDevice: " + JSON.stringify(this.bluetoothDevice));
  }

  build() {
    Column() {
      GridContainer({
        columns : $r('app.float.wh_value_12') ,
        sizeType : SizeType.Auto ,
        gutter : vp2px(1) === 2 ? $r('app.float.wh_value_12') : $r('app.float.wh_value_0') ,
        margin : vp2px(1) === 2 ? $r('app.float.wh_value_24') : $r('app.float.wh_value_0')
      }) {
        Row({}) {
          Column() {
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.WH_100_100)
          .useSizeType({
            xs : { span : 0 , offset : 0 } , sm : { span : 0 , offset : 0 } ,
            md : { span : 0 , offset : 0 } , lg : { span : $r('app.float.wh_value_2') , offset : 0 }
          });
          Column() {
            HeadTableComponent({ headName : this.bluetoothDevice.deviceName , isActive : true });
            Blank()
            Row() {
              Button() {
                Text($r("app.string.cancelPairing"))
                  .fontSize($r('app.float.application_button_subtitle_size'))
                  .fontColor(Color.Blue)
              }
              .backgroundColor($r('app.color.color_E3E3E3_grey'))
              .width($r("app.float.paired_device_button_width"))
              .height($r("app.float.wh_value_40"))
              .align(Alignment.Center)
              .onTouch((event: TouchEvent) => {
                if ( event.type === TouchType.Down ) {
                  this.isTouched = true;
                }
                if ( event.type === TouchType.Up ) {
                  this.isTouched = false;
                }
              })
              .onClick(() => {
                LogUtil.log(ConfigData.TAG + "unpair onClick");
                if ( this.controller.unpair(this.bluetoothDevice.deviceId) ) {
                  router.back();
                }
              })
            }.margin({ bottom : $r('app.float.distance_24') });
          }
          .padding({ left : $r('app.float.distance_24') , right : $r('app.float.distance_24') })
          .height(ConfigData.WH_100_100)
          .width(ConfigData.WH_100_100)
          .useSizeType({
            xs : { span : $r('app.float.wh_value_12') , offset : 0 } , sm : { span : $r('app.float.wh_value_12') , offset : 0 } ,
            md : { span : $r('app.float.wh_value_12') , offset : 0 } , lg : { span : $r('app.float.wh_value_8') , offset : $r('app.float.wh_value_2') }
          });

          Column() {
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.WH_100_100)
          .useSizeType({
            xs : { span : 0 , offset : $r('app.float.wh_value_12') } , sm : { span : 0 , offset : $r('app.float.wh_value_12') } ,
            md : { span : 0 , offset : $r('app.float.wh_value_12') } , lg : { span : $r('app.float.wh_value_2') , offset : $r('app.float.wh_value_10') }
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




