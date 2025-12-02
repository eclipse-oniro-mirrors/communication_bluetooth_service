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

import { myParams } from '../../../Utils/common'
import { TestImageDisplay } from '../../../Component/testImageDisplay';
import router from '@ohos.router';
import ConfigData from '../../../Utils/ConfigData';
import { initGattClientManagerApiData } from '../../../MainAbility/model/testDataModelsManager'
import { PageTitle } from '../../../Component/pageTitle';
import { ContentTable } from '../../../Component/contentTable';
import { TestData } from '../../../MainAbility/model/testData'

const G_MAC = '6c:96:d7:3d:87:6f'

@Component
struct GattClientSetting {
  private peripheralDeviceId = G_MAC; // '08:FB:EA:1B:3C:63';DaYu200//
  @State isOnBLECharacteristicChangeClick: boolean = false;
  @State bleCharacteristicChangeInfo: string = '';
  @State serviceUUID: string = '00001801-0000-1000-8000-00805f9b34fb';
  @State characteristicUUID: string = '00002b29-0000-1000-8000-00805f9b34fb';
  @State characteristicValue: string = 'CccValue';
  @State descriptorUUID: string = '00002902-0000-1000-8000-00805f9b34fb';
  @State descriptorValue: string = 'DesValue';

  aboutToAppear() {
    AppStorage.setOrCreate('peripheralDeviceId' , this.peripheralDeviceId);
    AppStorage.setOrCreate('serviceUUID' , this.serviceUUID);
    AppStorage.setOrCreate('characteristicUUID' , this.characteristicUUID);
    AppStorage.setOrCreate('characteristicValue' , this.characteristicValue);
    AppStorage.setOrCreate('descriptorUUID' , this.descriptorUUID);
    AppStorage.setOrCreate('descriptorValue' , this.descriptorValue);

  }

  build() {
    Column() {
      Row() {
        Text("外围设备MAC:").fontSize($r('app.float.wh_value_16')).width($r('app.float.wh_value_70'))
        TextInput({ text : this.peripheralDeviceId , placeholder : "input peripheral deviceId." })
          .fontSize($r('app.float.wh_value_15'))
          .onChange((strInput) => {
            this.peripheralDeviceId = strInput;
            //判断合法性
            if ( strInput.length >= 17 ) {
              AppStorage.setOrCreate('peripheralDeviceId' , strInput);
            }
          })
          .width($r('app.float.wh_value_80p'))
          .borderRadius($r('app.float.wh_value_1'))
      }
      .backgroundColor($r("app.color.moon"))
      .padding($r('app.float.wh_value_5'))
      .justifyContent(FlexAlign.Start)
      .alignItems(VerticalAlign.Center)

      Column() {
        Stack().height($r('app.float.wh_value_0_25')).backgroundColor($r('app.color.black'));
        Column() {
          Row() {
            Text("服务UUID:").fontSize($r('app.float.wh_value_15')).width($r('app.float.wh_value_60'));
            TextInput({ text : this.serviceUUID , placeholder : "input Service UUID" })
              .fontSize($r('app.float.wh_value_15'))
              .onChange((strInput) => {
                this.serviceUUID = strInput;
                if ( strInput.length >= 36 ) {
                  AppStorage.setOrCreate('serviceUUID' , strInput);
                }
              })
              .width($r('app.float.wh_value_80p'))
              .borderRadius($r('app.float.wh_value_1'))
          }
          .padding($r('app.float.wh_value_5'))
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)
          .backgroundColor($r("app.color.spring"))

          Row() {
            Text("特征值UUID:").fontSize($r('app.float.wh_value_15')).width($r('app.float.wh_value_60'));
            TextInput({ text : this.characteristicUUID , placeholder : "input Characteristic UUID" })
              .fontSize($r('app.float.wh_value_15'))
              .onChange((strInput) => {
                this.characteristicUUID = strInput;
                if ( strInput.length >= 36 ) {
                  AppStorage.setOrCreate('characteristicUUID' , strInput);
                }
              })
              .width($r('app.float.wh_value_80p'))
              .borderRadius($r('app.float.wh_value_1'))
          }
          .backgroundColor($r("app.color.spring"))
          .padding($r('app.float.wh_value_5'))
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)

          Row() {
            Text("特征值Value:").fontSize($r('app.float.wh_value_15')).width($r('app.float.wh_value_60'));
            TextInput({ text : this.characteristicUUID , placeholder : "Characteristic Value" })
              .fontSize($r('app.float.wh_value_15'))
              .onChange((strInput) => {
                this.characteristicValue = strInput;
                if ( strInput.length >= 1 ) {
                  AppStorage.setOrCreate('characteristicValue' , strInput);
                }
              })
              .width($r('app.float.wh_value_80p'))
              .borderRadius($r('app.float.wh_value_1'))
          }
          .backgroundColor($r("app.color.spring"))
          .padding($r('app.float.wh_value_5'))
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)

          Row() {
            Text("描述符UUID:").fontSize($r('app.float.wh_value_15')).width($r('app.float.wh_value_60'));
            TextInput({ text : this.descriptorUUID , placeholder : "input descriptor UUID" })
              .fontSize($r('app.float.wh_value_15'))
              .onChange((strInput) => {
                this.descriptorUUID = strInput;
                if ( strInput.length >= 36 ) {
                  AppStorage.setOrCreate('descriptorUUID' , strInput);
                }
              })
              .width($r('app.float.wh_value_80p'))
              .borderRadius($r('app.float.wh_value_1'))
          }
          .backgroundColor($r("app.color.spring"))
          .padding($r('app.float.wh_value_5'))
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)

          Row() {
            Text("描述符Value:").fontSize($r('app.float.wh_value_15')).width($r('app.float.wh_value_60'));
            TextInput({ text : this.descriptorUUID , placeholder : "descriptor Value" })
              .fontSize($r('app.float.wh_value_15'))
              .onChange((strInput) => {
                this.descriptorValue = strInput;
                if ( strInput.length >= 1 ) {
                  AppStorage.setOrCreate('descriptorValue' , strInput);
                }
              })
              .width($r('app.float.wh_value_80p'))
              .borderRadius($r('app.float.wh_value_1'))
          }
          .backgroundColor($r("app.color.spring"))
          .padding($r('app.float.wh_value_5'))
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)
        }

        Stack().height($r('app.float.wh_value_0_25')).backgroundColor($r('app.color.black'));
      }
    }
  }
}
/**
 * Gatt Client Manager Test Page Of Bluetooth test
 */
@Entry
@Component
struct GattClientManager {
  @State message: string = 'GattClientManager'
  @State currentClick: number = - 1;
  @State showList: boolean = false;
  @State btOnBLEConnectionStateChange: string = 'on:BLEConnectionStateChange';
  @State isOnBLEConnectionStateChangeClick: boolean = false;
  @StorageLink('bleConnectionStateInfo') bleConnectionStateInfo: string = 'Disconnected';
  @StorageLink('bleCharacteristicChangeInfo') bleCharacteristicChangeInfo: string = 'bleCharacteristicChangeInfo';
  @State bleConnectionState: number = 0;
  @State btOnBLECharacteristicChange: string = 'on:BLECharacteristicChange';
  @State isOnBLECharacteristicChangeClick: boolean = false;
  @State serviceUUID: string = '00000923-0000-1000-8000-00805f9b34fb';
  @State characteristicUUID: string = '00002001-0000-1000-8000-00805f9b34fb';
  @State characteristicValue: string = 'CccValue';
  @State descriptorUUID: string = '00002902-0000-1000-8000-00805f9b34fb';
  @State descriptorValue: string = 'DesValue';
  @StorageLink("getServices") getServices: string = "";
  @StorageLink("deviceName") deviceName: string = "";
  @StorageLink("Rssi") Rssi: string = "";
  @StorageLink("read_CharacteristicValue") read_CharacteristicValue: string = "";
  @StorageLink("read_DescriptorValue") read_DescriptorValue: string = "";
  @State mtuSize: number = 128;

  //------------@State->private------------------------------
  private peripheralDeviceId = G_MAC; // '6c:96:d7:3d:87:6f';PC//
  private testItem: TestData = (router.getParams() as myParams).testItem;

  aboutToAppear() {
    AppStorage.setOrCreate('peripheralDeviceId' , this.peripheralDeviceId);
    AppStorage.setOrCreate('serviceUUID' , this.serviceUUID);
    AppStorage.setOrCreate('characteristicUUID' , this.characteristicUUID);
    AppStorage.setOrCreate('characteristicValue' , this.characteristicValue);
    AppStorage.setOrCreate('descriptorUUID' , this.descriptorUUID);
    AppStorage.setOrCreate('descriptorValue' , this.descriptorValue);
  }

  build() {
    Column() {
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
      }

      Stack().height($r('app.float.wh_value_0_5')).backgroundColor($r('app.color.black'));
      Row() {
        Text("GattClientSetting")
          .fontSize($r('app.float.wh_value_20'))
          .height($r('app.float.wh_value_60'))
          .width($r('app.float.wh_value_80p'))
          .padding({ left : $r('app.float.wh_value_15') })
        Image($r('app.media.Switch'))
          .height($r('app.float.wh_value_50'))
          .width($r('app.float.wh_value_50'))
          .padding({ top : $r('app.float.wh_value_10') , bottom : $r('app.float.wh_value_10') , left : $r('app.float.wh_value_10') })
          .onClick(() => {
            this.showList = !this.showList;
          })
      }
      .width($r('app.float.wh_value_100p'))

      Column() {
        if ( this.showList ) {
          GattClientSetting()
        } else {
          Column() {
            Scroll() {
              Column() {
                Text("结果显示:")
                  .fontSize($r('app.float.wh_value_17'))
                  .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_10') })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_100p'))
                Text("当前配置:" + this.message)
                  .fontSize($r('app.float.wh_value_17'))
                  .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_15') })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
                Text("BLE连接状态:" + this.bleConnectionStateInfo)
                  .fontSize($r('app.float.wh_value_17'))
                  .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_15') })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
                Text("BLE特征变化:" + this.bleCharacteristicChangeInfo)
                  .fontSize($r('app.float.wh_value_17'))
                  .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_15') })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
                Text("deviceName:" + this.deviceName)
                  .fontSize($r('app.float.wh_value_17'))
                  .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_15') })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
                Text("Rssi:" + this.Rssi)
                  .fontSize($r('app.float.wh_value_17'))
                  .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_15') })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
                Text("getService:" + this.getServices)
                  .fontSize($r('app.float.wh_value_17'))
                  .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_15') })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
                Text("readCharacteristic:" + this.read_CharacteristicValue)
                  .fontSize($r('app.float.wh_value_17'))
                  .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_15') })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
                Text("readDescriptor:" + this.read_DescriptorValue)
                  .fontSize($r('app.float.wh_value_17'))
                  .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_15') })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
              }
              .width($r('app.float.wh_value_100p'))
            }
            .scrollBarWidth($r('app.float.wh_value_10'))
            .scrollBar(BarState.Auto)
          }
          .height($r('app.float.wh_value_110'))
          .width($r('app.float.wh_value_95p'))
          .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))

          Stack().height($r('app.float.wh_value_0_5')).backgroundColor($r('app.color.black'));
          Column() {
            Text("功能测试:")
              .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_10') })
              .padding($r('app.float.wh_value_5'))
              .fontSize($r('app.float.wh_value_18'))
              .width($r('app.float.wh_value_100p'))
            Scroll() {
              ContentTable({ apiItems : initGattClientManagerApiData() })
            }
            .width(ConfigData.WH_100_100)
            .align(Alignment.TopStart)
            .layoutWeight($r('app.float.wh_value_1'))
            .margin({ top : $r('sys.float.ohos_id_card_margin_middle') })
            .height($r('app.float.wh_value_100p'))
          }
          .height($r('app.float.wh_value_80p'))
        }
      }
      .visibility(Visibility.Visible);
    }
    .backgroundColor($r("app.color.lead"))
    .height($r('app.float.wh_value_100p'))
  }
}