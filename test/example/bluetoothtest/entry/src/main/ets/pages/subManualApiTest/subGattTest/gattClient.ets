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
import { initGattClientApiData } from '../../../MainAbility/model/testDataModels'
import { PageTitle } from '../../../Component/pageTitle';
import { ContentTable } from '../../../Component/contentTable';
import { TestData } from '../../../MainAbility/model/testData'

const G_MAC = '6c:96:d7:3d:87:6f'

/**
 * Gatt Client Test Page Of Bluetooth test
 */

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
        Text("外围设备MAC:").fontSize(16).width(70)
        TextInput({ text : this.peripheralDeviceId , placeholder : "input peripheral deviceId." })
          .fontSize("15vp")
          .onChange((strInput) => {
            this.peripheralDeviceId = strInput;
            //判断合法性
            if ( strInput.length >= 17 ) {
              AppStorage.setOrCreate('peripheralDeviceId' , strInput);
            }
          })
          .width($r('app.float.wh_value_80p'))
          .borderRadius(1)
      }
      .backgroundColor($r("app.color.moon"))
      .padding(5)
      .justifyContent(FlexAlign.Start)
      .alignItems(VerticalAlign.Center)

      Column() {
        Stack().height("0.25vp").backgroundColor("#000000");
        Column() {
          Row() {
            Text("服务UUID:").fontSize(15).width(60);
            TextInput({ text : this.serviceUUID , placeholder : "input Service UUID" })
              .fontSize("15vp")
              .onChange((strInput) => {
                this.serviceUUID = strInput;
                if ( strInput.length >= 36 ) {
                  AppStorage.setOrCreate('serviceUUID' , strInput);
                }
              })
              .width($r('app.float.wh_value_80p'))
              .borderRadius(1)
          }
          .padding(5)
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)
          .backgroundColor($r("app.color.spring"))

          Row() {
            Text("特征值UUID:").fontSize(15).width(60);
            TextInput({ text : this.characteristicUUID , placeholder : "input Characteristic UUID" })
              .fontSize("15vp")
              .onChange((strInput) => {
                this.characteristicUUID = strInput;
                if ( strInput.length >= 36 ) {
                  AppStorage.setOrCreate('characteristicUUID' , strInput);
                }
              })
              .width($r('app.float.wh_value_80p'))
              .borderRadius(1)
          }
          .backgroundColor($r("app.color.spring"))
          .padding(5)
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)

          Row() {
            Text("特征值Value:").fontSize(15).width(60);
            TextInput({ text : this.characteristicUUID , placeholder : "Characteristic Value" })
              .fontSize("15vp")
              .onChange((strInput) => {
                this.characteristicValue = strInput;
                if ( strInput.length >= 1 ) {
                  AppStorage.setOrCreate('characteristicValue' , strInput);
                }
              })
              .width($r('app.float.wh_value_80p'))
              .borderRadius(1)
          }
          .backgroundColor($r("app.color.spring"))
          .padding(5)
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)

          Row() {
            Text("描述符UUID:").fontSize(15).width(60);
            TextInput({ text : this.descriptorUUID , placeholder : "input descriptor UUID" })
              .fontSize("15vp")
              .onChange((strInput) => {
                this.descriptorUUID = strInput;
                if ( strInput.length >= 36 ) {
                  AppStorage.setOrCreate('descriptorUUID' , strInput);
                }
              })
              .width($r('app.float.wh_value_80p'))
              .borderRadius(1)
          }
          .backgroundColor($r("app.color.spring"))
          .padding(5)
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)

          Row() {
            Text("描述符Value:").fontSize(15).width(60);
            TextInput({ text : this.descriptorUUID , placeholder : "descriptor Value" })
              .fontSize("15vp")
              .onChange((strInput) => {
                this.descriptorValue = strInput;
                if ( strInput.length >= 1 ) {
                  AppStorage.setOrCreate('descriptorValue' , strInput);
                }
              })
              .width($r('app.float.wh_value_80p'))
              .borderRadius(1)
          }
          .backgroundColor($r("app.color.spring"))
          .padding(5)
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)
        }
        Stack().height("0.25vp").backgroundColor("#000000");
      }
    }
  }
}

@Entry
@Component
struct GattClient {
  @State message: string = 'GattClient'
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
  // @State  gattClientInstance:bluetooth.GattClientDevice = null;
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

      Stack().height("0.5vp").backgroundColor("#000000");
      Row() {
        Text("GattClientSetting")
          .fontSize("20vp")
          .height(60)
          .width($r('app.float.wh_value_80p'))
          .padding({ left : "15vp" })
        Image($r('app.media.Switch'))
          .height(50)
          .width(50)
          .padding({ top : 5 , bottom : 5 , left : 10 })
          .onClick(() => {
            this.showList = !this.showList;
          })
      }.width($r('app.float.wh_value_100p'))

      Column() {
        if ( this.showList ) {
          GattClientSetting()
        } else {
          Column() {
            Scroll() {
              Column() {
                Text("结果显示:")
                  .fontSize("17vp")
                  .margin({ top : "5vp" , left : "10vp" })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_100p'))
                Text("当前配置:" + this.message)
                  .fontSize("17vp")
                  .margin({ top : "5vp" , left : "15vp" })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
                Text("BLE连接状态:" + this.bleConnectionStateInfo)
                  .fontSize("17vp")
                  .margin({ top : "5vp" , left : "15vp" })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
                Text("BLE特征变化:" + this.bleCharacteristicChangeInfo)
                  .fontSize("17vp")
                  .margin({ top : "5vp" , left : "15vp" })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
                Text("deviceName:" + this.deviceName)
                  .fontSize("17vp")
                  .margin({ top : "5vp" , left : "15vp" })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
                Text("Rssi:" + this.Rssi)
                  .fontSize("17vp")
                  .margin({ top : "5vp" , left : "15vp" })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
                Text("getService:" + this.getServices)
                  .fontSize("17vp")
                  .margin({ top : "5vp" , left : "15vp" })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
                Text("readCharacteristic:" + this.read_CharacteristicValue)
                  .fontSize("17vp")
                  .margin({ top : "5vp" , left : "15vp" })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
                Text("readDescriptor:" + this.read_DescriptorValue)
                  .fontSize("17vp")
                  .margin({ top : "5vp" , left : "15vp" })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
              }
              .width($r('app.float.wh_value_100p'))
            }
            .scrollBarWidth(10)
            .scrollBar(BarState.Auto)
          }
          .height(110)
          .width($r('app.float.wh_value_95p'))
          .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))

          Stack().height("0.5vp").backgroundColor("#000000");
          Column() {
            Text("功能测试:")
              .margin({ top : "5vp" , left : "10vp" })
              .padding("5vp")
              .fontSize("18vp")
              .width($r('app.float.wh_value_100p'))
            Scroll() {
              ContentTable({ apiItems : initGattClientApiData() })
            }
            .width(ConfigData.WH_100_100)
            .align(Alignment.TopStart)
            .layoutWeight(1)
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

/**
 * string to ArrayBuffer
 * @param {*} str, the string object to
 * @return: ArrayBuffer
 */
function string2ArrayBuffer(str: string) {
  let array = new Uint8Array(str.length);
  for ( let i = 0 ; i < str.length ; i ++ ) {
    array[ i ] = str.charCodeAt(i);
  }
  return array.buffer
}
