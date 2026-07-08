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
import { resStr} from '../../../Utils/resString';


import { testItemFromRouterOrPlaceholder} from '../../../Utils/common'
import { TestImageDisplay} from '../../../component/testImageDisplay';
import ConfigData from '../../../Utils/ConfigData';
import { initGattClientManagerApiData} from '../../../MainAbility/model/testDataModelsManager'
import { PageTitle} from '../../../component/pageTitle';
import { ContentTable} from '../../../component/contentTable';
import { TestData} from '../../../MainAbility/model/testData'

const G_MAC = '6c:96:d7:3d:87:6f'

@Component
struct GattClientSetting {
  private peripheralDeviceId = G_MAC;// '08:FB:EA:1B:3C:63';DaYu200//
  @State isOnBLECharacteristicChangeClick: boolean = false;
  @State bleCharacteristicChangeInfo: string = '';
  @State serviceUUID: string = '00001801-0000-1000-8000-00805f9b34fb';
  @State characteristicUUID: string = '00002b29-0000-1000-8000-00805f9b34fb';
  @State characteristicValue: string = 'CccValue';
  @State descriptorUUID: string = '00002902-0000-1000-8000-00805f9b34fb';
  @State descriptorValue: string = 'DesValue';

  aboutToAppear(){
    AppStorage.setOrCreate('peripheralDeviceId', this.peripheralDeviceId);
    AppStorage.setOrCreate('serviceUUID', this.serviceUUID);
    AppStorage.setOrCreate('characteristicUUID', this.characteristicUUID);
    AppStorage.setOrCreate('characteristicValue', this.characteristicValue);
    AppStorage.setOrCreate('descriptorUUID', this.descriptorUUID);
    AppStorage.setOrCreate('descriptorValue', this.descriptorValue);

  }

  build(){
    Column(){
      Row(){
        Text($r('app.string.bluetoothtest_text_wwsbm'))
          .fontSize(ConfigData.VP_16)
          .width(ConfigData.VP_70)
        TextInput({text: this.peripheralDeviceId, placeholder: "input peripheral deviceId."})
          .accessibilityLevel('yes')
          .id('pages_subManualApiTestManager_subGattTestManager_gattClientManager__ti_0')
          .fontSize(ConfigData.VP_15)
          .onChange((strInput) => {
            this.peripheralDeviceId = strInput;
            //判断合法性
            if ( strInput.length >= 17 ){
              AppStorage.setOrCreate('peripheralDeviceId', strInput);
            }
          })
          .width(ConfigData.WH_80_100)
          .borderRadius(ConfigData.HEIGHT_1_VP)
      }
      .backgroundColor($r("app.color.moon"))
      .padding(ConfigData.VP_5)
      .justifyContent(FlexAlign.Start)
      .alignItems(VerticalAlign.Center)

      Column(){
        Stack()
          .height(ConfigData.VP_0_25)
          .backgroundColor($r('app.color.black'));
        Column(){
          Row(){
            Text($r('app.string.bluetoothtest_text_fwu_1'))
              .fontSize(ConfigData.VP_15)
              .width(ConfigData.VP_60);
            TextInput({text: this.serviceUUID, placeholder: "input Service UUID"})
              .accessibilityLevel('yes')
              .id('pages_subManualApiTestManager_subGattTestManager_gattClientManager__ti_1')
              .fontSize(ConfigData.VP_15)
              .onChange((strInput) => {
                this.serviceUUID = strInput;
                if ( strInput.length >= 36 ){
                  AppStorage.setOrCreate('serviceUUID', strInput);
                }
              })
              .width(ConfigData.WH_80_100)
              .borderRadius(ConfigData.HEIGHT_1_VP)
          }
          .padding(ConfigData.VP_5)
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)
          .backgroundColor($r("app.color.spring"))

          Row(){
            Text($r('app.string.bluetoothtest_text_tzzu'))
              .fontSize(ConfigData.VP_15)
              .width(ConfigData.VP_60);
            TextInput({text: this.characteristicUUID, placeholder: "input Characteristic UUID"})
              .accessibilityLevel('yes')
              .id('pages_subManualApiTestManager_subGattTestManager_gattClientManager__ti_2')
              .fontSize(ConfigData.VP_15)
              .onChange((strInput) => {
                this.characteristicUUID = strInput;
                if ( strInput.length >= 36 ){
                  AppStorage.setOrCreate('characteristicUUID', strInput);
                }
              })
              .width(ConfigData.WH_80_100)
              .borderRadius(ConfigData.HEIGHT_1_VP)
          }
          .backgroundColor($r("app.color.spring"))
          .padding(ConfigData.VP_5)
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)

          Row(){
            Text($r('app.string.bluetoothtest_text_tzzv'))
              .fontSize(ConfigData.VP_15)
              .width(ConfigData.VP_60);
            TextInput({text: this.characteristicUUID, placeholder: "Characteristic Value"})
              .accessibilityLevel('yes')
              .id('pages_subManualApiTestManager_subGattTestManager_gattClientManager__ti_3')
              .fontSize(ConfigData.VP_15)
              .onChange((strInput) => {
                this.characteristicValue = strInput;
                if ( strInput.length >= 1 ){
                  AppStorage.setOrCreate('characteristicValue', strInput);
                }
              })
              .width(ConfigData.WH_80_100)
              .borderRadius(ConfigData.HEIGHT_1_VP)
          }
          .backgroundColor($r("app.color.spring"))
          .padding(ConfigData.VP_5)
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)

          Row(){
            Text($r('app.string.bluetoothtest_text_msfu'))
              .fontSize(ConfigData.VP_15)
              .width(ConfigData.VP_60);
            TextInput({text: this.descriptorUUID, placeholder: "input descriptor UUID"})
              .accessibilityLevel('yes')
              .id('pages_subManualApiTestManager_subGattTestManager_gattClientManager__ti_4')
              .fontSize(ConfigData.VP_15)
              .onChange((strInput) => {
                this.descriptorUUID = strInput;
                if ( strInput.length >= 36 ){
                  AppStorage.setOrCreate('descriptorUUID', strInput);
                }
              })
              .width(ConfigData.WH_80_100)
              .borderRadius(ConfigData.HEIGHT_1_VP)
          }
          .backgroundColor($r("app.color.spring"))
          .padding(ConfigData.VP_5)
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)

          Row(){
            Text($r('app.string.bluetoothtest_text_msfv'))
              .fontSize(ConfigData.VP_15)
              .width(ConfigData.VP_60);
            TextInput({text: this.descriptorUUID, placeholder: "descriptor Value"})
              .accessibilityLevel('yes')
              .id('pages_subManualApiTestManager_subGattTestManager_gattClientManager__ti_5')
              .fontSize(ConfigData.VP_15)
              .onChange((strInput) => {
                this.descriptorValue = strInput;
                if ( strInput.length >= 1 ){
                  AppStorage.setOrCreate('descriptorValue', strInput);
                }
              })
              .width(ConfigData.WH_80_100)
              .borderRadius(ConfigData.HEIGHT_1_VP)
          }
          .backgroundColor($r("app.color.spring"))
          .padding(ConfigData.VP_5)
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)
        }

        Stack()
          .height(ConfigData.VP_0_25)
          .backgroundColor($r('app.color.black'));
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
  private peripheralDeviceId = G_MAC;// '6c:96:d7:3d:87:6f';PC//
  private testItem: TestData = testItemFromRouterOrPlaceholder();

  aboutToAppear(){
    AppStorage.setOrCreate('peripheralDeviceId', this.peripheralDeviceId);
    AppStorage.setOrCreate('serviceUUID', this.serviceUUID);
    AppStorage.setOrCreate('characteristicUUID', this.characteristicUUID);
    AppStorage.setOrCreate('characteristicValue', this.characteristicValue);
    AppStorage.setOrCreate('descriptorUUID', this.descriptorUUID);
    AppStorage.setOrCreate('descriptorValue', this.descriptorValue);
  }

  build(){
    Column(){
      Stack({alignContent: Alignment.TopStart}){
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
          .accessibilityLevel('yes')
          .id('pages_subManualApiTestManager_subGattTestManager_gattClientManager__pt_0')
      }

      Stack()
        .height(ConfigData.VP_0_5)
        .backgroundColor($r('app.color.black'));
      Row(){
        Text($r('app.string.bluetoothtest_text_g'))
          .fontSize(ConfigData.VP_20)
          .height(ConfigData.VP_60)
          .width(ConfigData.WH_80_100)
          .padding({left: ConfigData.VP_15})
        Image($r('app.media.Switch'))
          .accessibilityLevel('yes')
          .id('pages_subManualApiTestManager_subGattTestManager_gattClientManager__img_0')
          .height(ConfigData.VP_50)
          .width(ConfigData.VP_50)
          .padding({top: ConfigData.VP_10, bottom: ConfigData.VP_10, left: ConfigData.VP_10})
          .onClick(() => {
            this.showList = !this.showList;
          })
      }
      .width(ConfigData.WH_100_100)

      Column(){
        if ( this.showList ){
          GattClientSetting()
        } else {
          Column(){
            Scroll(){
              Column(){
                Text($r('app.string.bluetoothtest_text_jgxs'))
                  .fontSize(ConfigData.VP_17)
                  .margin({top: ConfigData.VP_5, left: ConfigData.VP_10})
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_100_100)
                Text(resStr($r('app.string.label_gatt_current_config')) + this.message)
                  .fontSize(ConfigData.VP_17)
                  .margin({top: ConfigData.VP_5, left: ConfigData.VP_15})
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
                Text(resStr($r('app.string.label_ble_conn_state')) + this.bleConnectionStateInfo)
                  .fontSize(ConfigData.VP_17)
                  .margin({top: ConfigData.VP_5, left: ConfigData.VP_15})
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
                Text(resStr($r('app.string.label_ble_char_change')) + this.bleCharacteristicChangeInfo)
                  .fontSize(ConfigData.VP_17)
                  .margin({top: ConfigData.VP_5, left: ConfigData.VP_15})
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
                Text(resStr($r('app.string.label_device_name_colon')) + this.deviceName)
                  .fontSize(ConfigData.VP_17)
                  .margin({top: ConfigData.VP_5, left: ConfigData.VP_15})
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
                Text(resStr($r('app.string.label_rssi_colon')) + this.Rssi)
                  .fontSize(ConfigData.VP_17)
                  .margin({top: ConfigData.VP_5, left: ConfigData.VP_15})
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
                Text(resStr($r('app.string.label_get_service_colon')) + this.getServices)
                  .fontSize(ConfigData.VP_17)
                  .margin({top: ConfigData.VP_5, left: ConfigData.VP_15})
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
                Text(resStr($r('app.string.label_read_char_colon')) + this.read_CharacteristicValue)
                  .fontSize(ConfigData.VP_17)
                  .margin({top: ConfigData.VP_5, left: ConfigData.VP_15})
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
                Text(resStr($r('app.string.label_read_desc_colon')) + this.read_DescriptorValue)
                  .fontSize(ConfigData.VP_17)
                  .margin({top: ConfigData.VP_5, left: ConfigData.VP_15})
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
              .width(ConfigData.WH_100_100)
            }
            .accessibilityLevel('yes')
            .id('pages_subManualApiTestManager_subGattTestManager_gattClientManager__sc_0')
            .scrollBarWidth(ConfigData.VP_10)
            .scrollBar(BarState.Auto)
          }
          .height(ConfigData.VP_110)
          .width(ConfigData.PCT_95)
          .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))

          Stack()
            .height(ConfigData.VP_0_5)
            .backgroundColor($r('app.color.black'));
          Column(){
            Text($r('app.string.bluetoothtest_text_gncs'))
              .margin({top: ConfigData.VP_5, left: ConfigData.VP_10})
              .padding(ConfigData.VP_5)
              .fontSize(ConfigData.VP_18)
              .width(ConfigData.WH_100_100)
            Scroll(){
              ContentTable({apiItems: initGattClientManagerApiData()})
            }
            .accessibilityLevel('yes')
            .id('pages_subManualApiTestManager_subGattTestManager_gattClientManager__sc_1')
            .width(ConfigData.WH_100_100)
            .align(Alignment.TopStart)
            .layoutWeight(1)
            .margin({top: $r('sys.float.ohos_id_card_margin_middle')})
            .height(ConfigData.WH_100_100)
          }
          .height(ConfigData.WH_80_100)
        }
      }
      .visibility(Visibility.Visible);
    }
    .backgroundColor($r("app.color.lead"))
    .height(ConfigData.WH_100_100)
  }
}