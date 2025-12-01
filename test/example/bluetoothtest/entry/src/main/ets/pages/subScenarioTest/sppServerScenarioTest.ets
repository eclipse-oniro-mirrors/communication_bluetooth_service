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
import { initSppServerScenarioData } from '../../MainAbility/model/scenarioTestDataModel'
import * as SPPServerInterface from '../../MainAbility/model/sppServerInterface'
import * as BrInterface from '../../MainAbility/model/brInterface'

/**
 * SppServer Scenario Test Page Of Bluetooth test
 */

@Entry
@Component
struct SppServerScenarioTest {
  private testItem: TestData = (router.getParams() as myParams).testItem
  @State changeIndex: number = - 1;
  @State writeServerData: string = '11111';
  private uuid: string = "00001101-0000-1000-8000-00805f9b34fb";
  @StorageLink("sppServerScenarioMessage") sppServerScenarioMessage: string = ""
  @StorageLink('dataReadMessage') dataReadMessage: string = "";

  aboutToAppear() {
    AppStorage.setOrCreate('txtScanFilterDeviceId' , "08:FB:EA:1B:3C:63");
    AppStorage.setOrCreate('txtScanFilterName' , "dudu-tiger");
    AppStorage.setOrCreate('txtScanFilterServiceUuid' , "0000180A-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('txtScanOptions_interval' , "0");
    AppStorage.setOrCreate('txtScanOptionsDutyMode' , '0');
    AppStorage.setOrCreate('txtScanOptionsMatchMode' , '0');
    AppStorage.setOrCreate('cbxBleScanFilter' , true);
    AppStorage.setOrCreate('cbxBleScanOptions' , true);
    AppStorage.setOrCreate('inputUuid' , this.uuid);
    AppStorage.setOrCreate('writeServerData' , this.writeServerData);
  }

  build() {
    Column() {
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
      }

      Stack().height("1vp").backgroundColor("#000000");
      Column() {
        Scroll() {
          Column() {
            Text("场景流程验证:")
              .fontSize("17vp")
              .margin({ top : "10vp" , bottom : "10vp" , left : "20vp" })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            ScenarioContentTable({
              scenarioItems : initSppServerScenarioData() ,
              changeIndex : this.changeIndex
            })
          }
          .width($r('app.float.wh_value_100p'))
          .height(280)
        }
        .scrollBarWidth(10)
        .scrollBar(BarState.Auto)

        Stack().height("1vp").backgroundColor("#000000");

        Scroll() {
          Column() {
            Text("流程日志:")
              .fontSize("17vp")
              .margin({ top : "8vp" , bottom : "8vp" , left : "20vp" })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            List() {
              ListItem() {
                Text(this.sppServerScenarioMessage)
                  .fontSize("17vp")
                  .margin({ top : "5vp" , left : "30vp" , right : "10vp" })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
              }
            }
            .height($r('app.float.wh_value_80p'))
            .width($r('app.float.wh_value_90p'))
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width($r('app.float.wh_value_100p'))
          .height(260)
        }
        .scrollBarWidth(10)
        .scrollBar(BarState.On)
      }
      .height(500)
      .width($r('app.float.wh_value_100p'))
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Stack().height("1vp").backgroundColor("#000000");
      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Text('场景测试开始').fontSize("24vp").fontColor(0xffffff).margin({ left : 5 , right : 5 })
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.blue"))
          .width(100)
          .height(66)
          .margin({ top : "15vp" , left : "20vp" })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppServerScenarioMessage = ""
            let self = this
            const arr = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 ]
            // 注册状态变化
            sppServerScenarioMessage += BrInterface.testOnStateChange() + "\n"
            console.log("testOnStateChange success")
            self.changeIndex = arr[ 0 ]
            await sleep(1)

            // 开蓝牙
            sppServerScenarioMessage += BrInterface.testEnableBluetooth() + "\n";
            console.log("testEnableBluetooth success")
            self.changeIndex = arr[ 1 ]
            AppStorage.setOrCreate("sppServerScenarioMessage" , sppServerScenarioMessage)
            await sleep(2)

            // 获取状态
            sppServerScenarioMessage += BrInterface.testGetState() + "\n";
            console.log("testGetState success")
            self.changeIndex = arr[ 2 ]
            await sleep(1)

            // 设置扫描模式
            sppServerScenarioMessage += BrInterface.testSetBluetoothScanMode(4 , 0) + "\n"
            console.log("testSetBluetoothScanMode success")
            self.changeIndex = arr[ 3 ]
            await sleep(1)

            // 获取扫描模式
            sppServerScenarioMessage += BrInterface.testGetBluetoothScanMode() + "\n"
            console.log("testGetBluetoothScanMode success")
            self.changeIndex = arr[ 4 ]
            await sleep(1)

            // 开启注册pin码变化
            sppServerScenarioMessage += BrInterface.testOnPinRequired() + "\n"
            console.log("testOnPinRequired success")
            self.changeIndex = arr[ 5 ]
            await sleep(1)

            // Spp服务器创建
            sppServerScenarioMessage += SPPServerInterface.testSppServerCreateSafe() + "\n"
            console.log("testAddService success")
            self.changeIndex = arr[ 6 ]
            AppStorage.setOrCreate("sppServerScenarioMessage" , sppServerScenarioMessage)
            await sleep(5)

            //获取当前连接状态
            sppServerScenarioMessage += BrInterface.testGetBtConnectionState() + "\n"
            console.log("testGetBtConnectionState success")
            self.changeIndex = arr[ 7 ]
            AppStorage.setOrCreate("sppServerScenarioMessage" , sppServerScenarioMessage)
            await sleep(1)

            // Spp服务器接受
            sppServerScenarioMessage += SPPServerInterface.testSppServerAccept() + "\n"
            console.log("testSppServerAccept success")
            self.changeIndex = arr[ 8 ]
            AppStorage.setOrCreate("sppServerScenarioMessage" , sppServerScenarioMessage)
            await sleep(5)

            // Spp服务器读取打开
            sppServerScenarioMessage += SPPServerInterface.testSppServerReadOn() + "\n"
            console.log("testOnCharacteristicRead success")
            self.changeIndex = arr[ 9 ]
            await sleep(3)
            sppServerScenarioMessage += AppStorage.get("dataReadMessage") + "\n"
            AppStorage.setOrCreate("sppServerScenarioMessage" , sppServerScenarioMessage)

            // Spp服务器写入
            sppServerScenarioMessage += SPPServerInterface.testSppServerWrite() + "\n"
            console.log("testOnCharacteristicWrite success")
            self.changeIndex = arr[ 10 ]
            await sleep(2)
            AppStorage.setOrCreate("sppServerScenarioMessage" , sppServerScenarioMessage)

          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('场景测试结束').fontSize("24vp").fontColor(0xffffff).margin({ left : 5 , right : 5 })
            }.alignItems(VerticalAlign.Top)
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.blue"))
          .width(100)
          .height(66)
          .margin({ top : "15vp" , left : "10vp" , right : "10vp" })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppServerScenarioMessage = ""
            let self = this
            const arr = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 ]
            // Spp服务器读取关闭
            sppServerScenarioMessage += SPPServerInterface.testSppServerReadOff() + "\n"
            console.log("testOnCharacteristicRead success")
            self.changeIndex = arr[ 11 ]
            await sleep(1)
            // Spp服务器关闭
            sppServerScenarioMessage += SPPServerInterface.testSppCloseServerSocket() + "\n"
            console.log("testSppCloseServerSocket success")
            self.changeIndex = arr[ 12 ]
            await sleep(1)
            // Spp客户端关闭
            sppServerScenarioMessage += SPPServerInterface.testSppCloseClientSocket() + "\n"
            console.log("testSppCloseServerSocket success")
            self.changeIndex = arr[ 13 ]
            await sleep(1)
            // 关闭注册pin码变化
            sppServerScenarioMessage += BrInterface.testOnPinRequired() + "\n"
            console.log("testOffPinRequired success")
            await sleep(1)
            // 关闭注册状态变化
            sppServerScenarioMessage += BrInterface.testOnStateChange() + "\n"
            console.log("testOffStateChange success")
            await sleep(1)
            // 关蓝牙
            sppServerScenarioMessage += BrInterface.testDisableBluetooth() + "\n"
            console.log("testDisableBluetooth success")
            self.changeIndex = arr[ 14 ]
            AppStorage.setOrCreate("sppServerScenarioMessage" , sppServerScenarioMessage)
            await sleep(1)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('清空日志').fontSize("24vp").fontColor(0xffffff).margin({ left : 5 , right : 5 })
            }.alignItems(VerticalAlign.Top)
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.moon"))
          .width(80)
          .height(66)
          .margin({ top : "15vp" , right : "20vp" })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppServerScenarioMessage = ""
            AppStorage.setOrCreate("sppServerScenarioMessage" , sppServerScenarioMessage)
          })
        }
        .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        .width($r('app.float.wh_value_100p'))
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

export function strToArrayBuffer(str: string) {
  let array = new Uint8Array(str.length);
  for ( let i = 0 ; i < str.length ; i ++ ) {
    array[ i ] = str.charCodeAt(i);
  }
  return array.buffer
}