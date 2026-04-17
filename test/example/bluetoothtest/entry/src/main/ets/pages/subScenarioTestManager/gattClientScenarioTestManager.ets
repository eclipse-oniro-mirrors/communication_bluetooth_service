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

import hilog from '@ohos.hilog';
import ConfigData from '../../Utils/ConfigData';
import { testItemFromRouterOrPlaceholder } from '../../Utils/common'
import { PageTitle } from '../../Component/pageTitle';
import { TestImageDisplay } from '../../Component/testImageDisplay';
import { TestData } from '../../MainAbility/model/testData'
import { ScenarioContentTable } from '../../Component/scenarioContentTable'
import { initGattClientManagerScenarioData } from '../../MainAbility/model/scenarioTestDataModelManager'
import * as GattClientManagerInterface from '../../MainAbility/model/gattClientManagerInterface'
import * as BrManagerInterface from '../../MainAbility/model/brManagerInterface'

/**
 * GattClient Scenario Manager Test Page Of Bluetooth test
 */

@Entry
@Component
struct GattClientScenarioTestManager {
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @State changeIndex: number = - 1
  @StorageLink("gattClientScenarioMessage") gattClientScenarioMessage: string = ""
  @StorageLink("getServices") getServices: string = "";
  @StorageLink("deviceName") deviceName: string = "";
  @StorageLink("Rssi") Rssi: string = "";
  @StorageLink("read_CharacteristicValue") read_CharacteristicValue: string = "";
  private peripheralDeviceId = '08:FB:EA:1B:3C:63'
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
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
              .accessibilityLevel('yes')
              .id('pages_subScenarioTestManager_gattClientScenarioTestManager__pt_0')
      }

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column() {
        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_cjlcyz'))
              .fontSize(ConfigData.VP_17)
              .margin({ top : ConfigData.VP_10 , bottom : ConfigData.VP_10 , left : ConfigData.VP_20 })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            ScenarioContentTable({
              scenarioItems : initGattClientManagerScenarioData() ,
              changeIndex : this.changeIndex
            })
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_280_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTestManager_gattClientScenarioTestManager__sc_0')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)

        Stack()
          .height(ConfigData.HEIGHT_1_VP)
          .backgroundColor($r('app.color.black'));

        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_lcrz'))
              .fontSize(ConfigData.VP_17)
              .margin({ top : ConfigData.VP_8 , bottom : ConfigData.VP_8 , left : ConfigData.VP_20 })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            List() {
              ListItem() {
                Text(this.gattClientScenarioMessage)
                  .fontSize(ConfigData.VP_17)
                  .margin({ top : ConfigData.VP_5 , left : ConfigData.VP_30 , right : ConfigData.VP_10 })
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
            .accessibilityLevel('yes')
            .id('pages_subScenarioTestManager_gattClientScenarioTestManager__li_0')
            }
            .height(ConfigData.WH_80_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_260_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTestManager_gattClientScenarioTestManager__sc_1')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.On)
      }
      .height(ConfigData.HEIGHT_500_VP)
      .width(ConfigData.WH_100_100)
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))


      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Text($r('app.string.bluetoothtest_text_cjcsks'))
              .fontSize(ConfigData.VP_24)
              .fontColor(0xffffff)
              .margin({ left : ConfigData.VP_5 , right : ConfigData.VP_5 })
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTestManager_gattClientScenarioTestManager__btn_0')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , left : ConfigData.VP_20 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let self = this
            const arr = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 ]
            let gattClientScenarioMessage = ""

            // 开启注册状态变化
            gattClientScenarioMessage += BrManagerInterface.testOnStateChangeManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testOnStateChange success")
            self.changeIndex = arr[ 0 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 开蓝牙
            gattClientScenarioMessage += BrManagerInterface.testEnableBluetoothManager() + "\n";
            hilog.info(0x0000, 'BtTest', '%{public}s', "testEnableBluetooth success")
            self.changeIndex = arr[ 1 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(2)

            // 获取状态
            gattClientScenarioMessage += BrManagerInterface.testGetStateManager() + "\n";
            hilog.info(0x0000, 'BtTest', '%{public}s', "testGetState success")
            self.changeIndex = arr[ 2 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 创建Gatt客户端
            gattClientScenarioMessage += GattClientManagerInterface.testCreateGattClientManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testCreateGattClient success")
            self.changeIndex = arr[ 3 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(2)

            //开启Ble连接状态变化
            gattClientScenarioMessage += GattClientManagerInterface.testOnBLEConnectionStateChangeManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testOnBLEConnectionStateChange success")
            self.changeIndex = arr[ 4 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 连接
            gattClientScenarioMessage += GattClientManagerInterface.testConnectManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testConnect success")
            self.changeIndex = arr[ 5 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(5)

            // 设置通知特性更改
            gattClientScenarioMessage += GattClientManagerInterface.testSetNotifyCharacteristicChangedManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testSetNotifyCharacteristicChanged success")
            self.changeIndex = arr[ 6 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 开启BLE特征值变化
            gattClientScenarioMessage += GattClientManagerInterface.testOnBLECharacteristicChangeManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testOnBLECharacteristicChange success")
            self.changeIndex = arr[ 7 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 获取服务callback
            gattClientScenarioMessage += GattClientManagerInterface.testGetServicesCallbackManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testGetServicesCallback success")
            self.changeIndex = arr[ 8 ]
            gattClientScenarioMessage += AppStorage.get("getServices") + "\n"
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 读取特征值callback
            gattClientScenarioMessage += GattClientManagerInterface.testReadCharacteristicValueCallbackManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testReadCharacteristicValueCallback success")
            self.changeIndex = arr[ 9 ]
            gattClientScenarioMessage += AppStorage.get("read_CharacteristicValue") + "\n"
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 读取描述符值callback
            gattClientScenarioMessage += GattClientManagerInterface.testReadDescriptorValueCallbackManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testReadDescriptorValueCallback success")
            self.changeIndex = arr[ 10 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 获取设备名称callback
            gattClientScenarioMessage += GattClientManagerInterface.testGetDeviceNameCallbackManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testGetDeviceNameCallback success")
            self.changeIndex = arr[ 11 ]
            gattClientScenarioMessage += AppStorage.get("deviceName") + "\n"
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 获取Rssi值callback
            gattClientScenarioMessage += GattClientManagerInterface.testGetRssiValueCallbackManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testGetRssiValueCallback success")
            self.changeIndex = arr[ 12 ]
            gattClientScenarioMessage += AppStorage.get("Rssi") + "\n"
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text($r('app.string.bluetoothtest_text_cjcsjs'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({ left : ConfigData.VP_5 , right : ConfigData.VP_5 })
            }
              .alignItems(VerticalAlign.Top)
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTestManager_gattClientScenarioTestManager__btn_1')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , left : ConfigData.VP_10 , right : ConfigData.VP_10 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let self = this
            const arr = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 ]
            let gattClientScenarioMessage = ""
            // 关闭BLE特征值变化
            gattClientScenarioMessage += GattClientManagerInterface.testOnBLECharacteristicChangeManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testOffBLECharacteristicChange success")
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 取消连接
            gattClientScenarioMessage += GattClientManagerInterface.testDisconnectManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testDisconnect success")
            self.changeIndex = arr[ 13 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            //关闭Ble连接状态变化
            gattClientScenarioMessage += GattClientManagerInterface.testOnBLEConnectionStateChangeManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testOffBLEConnectionStateChange success")
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            //关闭客户端
            gattClientScenarioMessage += GattClientManagerInterface.testGattClientCloseManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testGattClientClose success")
            self.changeIndex = arr[ 14 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 关闭注册状态变化
            gattClientScenarioMessage += BrManagerInterface.testOnStateChangeManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testOffStateChange success")
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            //关蓝牙
            gattClientScenarioMessage += BrManagerInterface.testDisableBluetoothManager() + "\n"
            hilog.info(0x0000, 'BtTest', '%{public}s', "testDisableBluetooth success")
            self.changeIndex = arr[ 15 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text($r('app.string.bluetoothtest_text_qkrz'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({ left : ConfigData.VP_5 , right : ConfigData.VP_5 })
            }
              .alignItems(VerticalAlign.Top)
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTestManager_gattClientScenarioTestManager__btn_2')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.moon"))
          .width(ConfigData.VP_80)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , right : ConfigData.VP_20 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let gattClientScenarioMessage = ""
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
          })
        }
      }
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
      .width(ConfigData.WH_100_100)
      .height(ConfigData.WH_100_100);
    }
  }
}

async function sleep(time: number): Promise<void> {
  return new Promise<void>((resolve , reject) => {
    setTimeout(() => {
      resolve();
    } , time * 1000);
  });
}

export function string2ArrayBuffer(str: string) {
  let array = new Uint8Array(str.length);
  for ( let i = 0 ; i < str.length ; i ++ ) {
    array[ i ] = str.charCodeAt(i);
  }
  return array.buffer
}