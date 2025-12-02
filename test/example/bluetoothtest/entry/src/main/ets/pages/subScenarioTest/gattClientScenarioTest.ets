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

import { myParams } from '../../Utils/common'
import router from '@ohos.router';
import { PageTitle } from '../../Component/pageTitle';
import { TestImageDisplay } from '../../Component/testImageDisplay';
import { TestData } from '../../MainAbility/model/testData'
import { ScenarioContentTable } from '../../Component/scenarioContentTable'
import { initGattClientScenarioData } from '../../MainAbility/model/scenarioTestDataModel'
import * as GattClientInterface from '../../MainAbility/model/gattClientInterface'
import * as BrInterface from '../../MainAbility/model/brInterface'

/**
 * GattClient Scenario Test Page Of Bluetooth test
 */

@Entry
@Component
struct GattClientScenarioTest {
  private testItem: TestData = (router.getParams() as myParams).testItem
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
      }

      Stack().height($r('app.float.wh_value_1')).backgroundColor($r('app.color.black'));
      Column() {
        Scroll() {
          Column() {
            Text("场景流程验证:")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_10') , bottom : $r('app.float.wh_value_10') , left : $r('app.float.wh_value_20') })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            ScenarioContentTable({
              scenarioItems : initGattClientScenarioData() ,
              changeIndex : this.changeIndex
            })
          }
          .width($r('app.float.wh_value_100p'))
          .height($r('app.float.wh_value_280'))
        }
        .scrollBarWidth($r('app.float.wh_value_10'))
        .scrollBar(BarState.Auto)

        Stack().height($r('app.float.wh_value_1')).backgroundColor($r('app.color.black'));

        Scroll() {
          Column() {
            Text("流程日志:")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_8') , bottom : $r('app.float.wh_value_8') , left : $r('app.float.wh_value_20') })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            List() {
              ListItem() {
                Text(this.gattClientScenarioMessage)
                  .fontSize($r('app.float.wh_value_17'))
                  .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_30') , right : $r('app.float.wh_value_10') })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
              }
            }
            .height($r('app.float.wh_value_80p'))
            .width($r('app.float.wh_value_90p'))
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width($r('app.float.wh_value_100p'))
          .height($r('app.float.wh_value_260'))
        }
        .scrollBarWidth($r('app.float.wh_value_10'))
        .scrollBar(BarState.On)
      }
      .height($r('app.float.wh_value_500'))
      .width($r('app.float.wh_value_100p'))
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Stack().height($r('app.float.wh_value_1')).backgroundColor($r('app.color.black'));
      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Text('场景测试开始').fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : $r('app.float.wh_value_5') , right : $r('app.float.wh_value_5') })
          }
          .borderRadius($r('app.float.wh_value_8'))
          .backgroundColor($r("app.color.blue"))
          .width($r('app.float.wh_value_100'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , left : $r('app.float.wh_value_20') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let self = this
            const arr = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 ]
            //            AlertDialog.show({ message: '开始GattClient场景测试' })
            let gattClientScenarioMessage = ""

            // 开启注册状态变化
            gattClientScenarioMessage += BrInterface.testOnStateChange() + "\n"
            console.log("testOnStateChange success")
            self.changeIndex = arr[ 0 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 开蓝牙
            gattClientScenarioMessage += BrInterface.testEnableBluetooth() + "\n";
            console.log("testEnableBluetooth success")
            self.changeIndex = arr[ 1 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(2)

            // 获取状态
            gattClientScenarioMessage += BrInterface.testGetState() + "\n";
            console.log("testGetState success")
            self.changeIndex = arr[ 2 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 创建Gatt客户端
            gattClientScenarioMessage += GattClientInterface.testCreateGattClient() + "\n"
            console.log("testCreateGattClient success")
            self.changeIndex = arr[ 3 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(2)

            //开启Ble连接状态变化
            gattClientScenarioMessage += GattClientInterface.testOnBLEConnectionStateChange() + "\n"
            console.log("testOnBLEConnectionStateChange success")
            self.changeIndex = arr[ 4 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 连接
            gattClientScenarioMessage += GattClientInterface.testConnect() + "\n"
            console.log("testConnect success")
            self.changeIndex = arr[ 5 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(5)

            // 设置通知特性更改
            gattClientScenarioMessage += GattClientInterface.testSetNotifyCharacteristicChanged() + "\n"
            console.log("testSetNotifyCharacteristicChanged success")
            self.changeIndex = arr[ 6 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 开启BLE特征值变化
            gattClientScenarioMessage += GattClientInterface.testOnBLECharacteristicChange() + "\n"
            console.log("testOnBLECharacteristicChange success")
            self.changeIndex = arr[ 7 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 获取服务callback
            gattClientScenarioMessage += GattClientInterface.testGetServicesCallback() + "\n"
            console.log("testGetServicesCallback success")
            self.changeIndex = arr[ 8 ]
            gattClientScenarioMessage += AppStorage.get("getServices") + "\n"
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 读取特征值callback
            gattClientScenarioMessage += GattClientInterface.testReadCharacteristicValueCallback() + "\n"
            console.log("testReadCharacteristicValueCallback success")
            self.changeIndex = arr[ 9 ]
            gattClientScenarioMessage += AppStorage.get("read_CharacteristicValue") + "\n"
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 读取描述符值callback
            gattClientScenarioMessage += GattClientInterface.testReadDescriptorValueCallback() + "\n"
            console.log("testReadDescriptorValueCallback success")
            self.changeIndex = arr[ 10 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 获取设备名称callback
            gattClientScenarioMessage += GattClientInterface.testGetDeviceNameCallback() + "\n"
            console.log("testGetDeviceNameCallback success")
            self.changeIndex = arr[ 11 ]
            gattClientScenarioMessage += AppStorage.get("deviceName") + "\n"
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 获取Rssi值callback
            gattClientScenarioMessage += GattClientInterface.testGetRssiValueCallback() + "\n"
            console.log("testGetRssiValueCallback success")
            self.changeIndex = arr[ 12 ]
            gattClientScenarioMessage += AppStorage.get("Rssi") + "\n"
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('场景测试结束').fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : $r('app.float.wh_value_5') , right : $r('app.float.wh_value_5') })
            }.alignItems(VerticalAlign.Top)
          }
          .borderRadius($r('app.float.wh_value_8'))
          .backgroundColor($r("app.color.blue"))
          .width($r('app.float.wh_value_100'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , left : $r('app.float.wh_value_10') , right : $r('app.float.wh_value_10') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let self = this
            const arr = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 ]
            let gattClientScenarioMessage = ""
            // 关闭BLE特征值变化
            gattClientScenarioMessage += GattClientInterface.testOnBLECharacteristicChange() + "\n"
            console.log("testOffBLECharacteristicChange success")
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 取消连接
            gattClientScenarioMessage += GattClientInterface.testDisconnect() + "\n"
            console.log("testDisconnect success")
            self.changeIndex = arr[ 13 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            //关闭Ble连接状态变化
            gattClientScenarioMessage += GattClientInterface.testOnBLEConnectionStateChange() + "\n"
            console.log("testOffBLEConnectionStateChange success")
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            //关闭客户端
            gattClientScenarioMessage += GattClientInterface.testGattClientClose() + "\n"
            console.log("testGattClientClose success")
            self.changeIndex = arr[ 14 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            // 关闭注册状态变化
            gattClientScenarioMessage += BrInterface.testOnStateChange() + "\n"
            console.log("testOffStateChange success")
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

            //关蓝牙
            gattClientScenarioMessage += BrInterface.testDisableBluetooth() + "\n"
            console.log("testDisableBluetooth success")
            self.changeIndex = arr[ 15 ]
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
            await sleep(1)

          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('清空日志').fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : $r('app.float.wh_value_5') , right : $r('app.float.wh_value_5') })
            }.alignItems(VerticalAlign.Top)
          }
          .borderRadius($r('app.float.wh_value_8'))
          .backgroundColor($r("app.color.moon"))
          .width($r('app.float.wh_value_80'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , right : $r('app.float.wh_value_20') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let gattClientScenarioMessage = ""
            AppStorage.setOrCreate("gattClientScenarioMessage" , gattClientScenarioMessage)
          })
        }
      }
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
      .width($r('app.float.wh_value_100p'))
      .height($r('app.float.wh_value_100p'));
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