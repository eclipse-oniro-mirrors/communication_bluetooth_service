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
import promptAction from '@ohos.promptAction';
import bluetooth from '@ohos.bluetooth';
import router from '@ohos.router';
import { PageTitle } from '../../Component/pageTitle';
import { TestImageDisplay } from '../../Component/testImageDisplay';
import { TestData } from '../../MainAbility/model/testData'
import { ScenarioContentTable } from '../../Component/scenarioContentTable'
import { initSppClientManagerScenarioData } from '../../MainAbility/model/scenarioTestDataModelManager'
import * as BrManagerInterface from '../../MainAbility/model/brManagerInterface'
import * as SPPClientManagerInterface from '../../MainAbility/model/sppClientManagerInterface'

/**
 * SppClient Scenario Manager Test Page Of Bluetooth test
 */

@Entry
@Component
struct SppClientScenarioTestManager {
  @State changeIndex: number = - 1;
  @State writeClientData: string = '11';
  private uuid: string = "00001101-0000-1000-8000-00805f9b34fb";
  @State deviceId: string = "08:FB:EA:1B:3C:63"; //08:FB:EA:1B:3C:63
  private testItem: TestData = (router.getParams() as myParams).testItem
  @StorageLink("sppClientScenarioMessage") sppClientScenarioMessage: string = ""
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

    AppStorage.setOrCreate('inputDeviceId' , this.deviceId);
    AppStorage.setOrCreate('inputUuid' , this.uuid);
    AppStorage.setOrCreate('writeClientData' , this.writeClientData);
  }

  build() {
    Column() {
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
      }

      Stack().height($r('app.float.wh_value_1')).backgroundColor("#000000");
      Column() {
        Scroll() {
          Column() {
            Text("场景流程验证:")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_10') , bottom : $r('app.float.wh_value_10') , left : $r('app.float.wh_value_20') })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            ScenarioContentTable({
              scenarioItems : initSppClientManagerScenarioData() ,
              changeIndex : this.changeIndex
            })
          }
          .width($r('app.float.wh_value_100p'))
          .height($r('app.float.wh_value_280'))
        }
        .scrollBarWidth(10)
        .scrollBar(BarState.Auto)

        Stack().height($r('app.float.wh_value_1')).backgroundColor("#000000");

        Scroll() {
          Column() {
            Text("流程日志:")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_8') , bottom : $r('app.float.wh_value_8') , left : $r('app.float.wh_value_20') })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            List() {
              ListItem() {
                Text(this.sppClientScenarioMessage)
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
        .scrollBarWidth(10)
        .scrollBar(BarState.On)
      }
      .height($r('app.float.wh_value_500'))
      .width($r('app.float.wh_value_100p'))
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))


      Stack().height($r('app.float.wh_value_1')).backgroundColor("#000000");
      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Text('场景测试开始').fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : 5 , right : 5 })
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.blue"))
          .width($r('app.float.wh_value_100'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , left : $r('app.float.wh_value_20') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {

            let self = this
            const arr = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 ]
            let sppClientScenarioMessage = ""
            // 注册状态变化
            sppClientScenarioMessage += BrManagerInterface.testOnStateChangeManager() + "\n"
            console.log("testOnStateChange success")
            self.changeIndex = arr[ 0 ]
            await sleep(1)

            // 开蓝牙
            sppClientScenarioMessage += BrManagerInterface.testEnableBluetoothManager() + "\n";
            console.log("testEnableBluetooth success")
            self.changeIndex = arr[ 1 ]
            await sleep(2)
            AppStorage.setOrCreate("sppClientScenarioMessage" , sppClientScenarioMessage)

            // 获取状态
            sppClientScenarioMessage += BrManagerInterface.testGetStateManager() + "\n";
            console.log("testGetState success")
            self.changeIndex = arr[ 2 ]
            await sleep(1)

            // 设置扫描模式
            sppClientScenarioMessage += BrManagerInterface.testSetBluetoothScanModeManager(4 , 0) + "\n"
            console.log("testSetBluetoothScanMode success")
            self.changeIndex = arr[ 3 ]
            await sleep(1)

            // 获取扫描模式
            sppClientScenarioMessage += BrManagerInterface.testGetBluetoothScanModeManager() + "\n"
            console.log("testGetBluetoothScanMode success")
            self.changeIndex = arr[ 4 ]
            await sleep(1)

            // Spp客户端连接
            sppClientScenarioMessage += SPPClientManagerInterface.testSppClientConnectSafeManager() + "\n"
            console.log("testSppClientConnectSafe success")
            self.changeIndex = arr[ 5 ]
            AppStorage.setOrCreate("sppClientScenarioMessage" , sppClientScenarioMessage)
            await sleep(10)

            //获取当前连接状态
            sppClientScenarioMessage += BrManagerInterface.testGetBtConnectionStateManager() + "\n"
            console.log("testGetBtConnectionState success")
            self.changeIndex = arr[ 6 ]
            AppStorage.setOrCreate("sppClientScenarioMessage" , sppClientScenarioMessage)
            await sleep(2)

            //spp客户端读取打开
            sppClientScenarioMessage += SPPClientManagerInterface.testSppClientReadOnManager() + "\n"
            console.log("testSppClientReadOn success")
            self.changeIndex = arr[ 7 ]
            sppClientScenarioMessage += AppStorage.get("dataReadMessage") + "\n"
            AppStorage.setOrCreate("sppClientScenarioMessage" , sppClientScenarioMessage)
            await sleep(5)

            // SPP客户端写入
            sppClientScenarioMessage += SPPClientManagerInterface.testSppClientWriteManager() + "\n"
            console.log("testSppClientWrite success")
            self.changeIndex = arr[ 8 ]
            AppStorage.setOrCreate("sppClientScenarioMessage" , sppClientScenarioMessage)
            await sleep(2)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('场景测试结束').fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : 5 , right : 5 })
            }.alignItems(VerticalAlign.Top)
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.blue"))
          .width($r('app.float.wh_value_100'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , left : $r('app.float.wh_value_10') , right : $r('app.float.wh_value_10') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let self = this
            const arr = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 ]
            let sppClientScenarioMessage = ""
            // spp客户端读取关闭
            sppClientScenarioMessage += SPPClientManagerInterface.testSppClientReadOffManager() + "\n"
            console.log("testSppClientReadOff success")
            self.changeIndex = arr[ 9 ]
            await sleep(1)

            // spp客户端关闭
            sppClientScenarioMessage += SPPClientManagerInterface.testSppClientCloseManager() + "\n"
            console.log("testSppClientClose success")
            self.changeIndex = arr[ 10 ]
            await sleep(1)

            //关闭注册状态变化
            sppClientScenarioMessage += BrManagerInterface.testOnStateChangeManager() + "\n"
            console.log("testOffStateChange success")
            await sleep(1)

            //关蓝牙
            sppClientScenarioMessage += BrManagerInterface.testDisableBluetoothManager() + "\n"
            console.log("testDisableBluetooth success")
            self.changeIndex = arr[ 11 ]
            await sleep(1)
            AppStorage.setOrCreate("sppClientScenarioMessage" , sppClientScenarioMessage)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('清空日志').fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : 5 , right : 5 })
            }.alignItems(VerticalAlign.Top)
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.moon"))
          .width($r('app.float.wh_value_80'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , right : $r('app.float.wh_value_20') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppClientScenarioMessage = ""
            AppStorage.setOrCreate("sppClientScenarioMessage" , sppClientScenarioMessage)
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

  dataRead(dataBuffer: ArrayBuffer) {
    const data = new Uint8Array(dataBuffer);
    console.log('spp bluetooth read is: ' + data);
    if ( data != null ) {
      this.dataReadMessage = "读取成功" + JSON.stringify(data);
    } else {
      this.dataReadMessage = "读取失败";
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

let clientNumber = - 1

export function acceptClientSocket(number: number) {
  console.log('spp clientSocket socketID: ' + number);
  // 获取的clientNumber用作服务端后续读/写操作socket的id。
  clientNumber = number;
}

export function sppClientClose() {
  console.log("bluetoothModel:" + 'sppCloseClientSocket clientNumber: ' + JSON.stringify(clientNumber))
  bluetooth.sppCloseClientSocket(clientNumber);
  let message = '关闭成功 clientNumber' + clientNumber;
  promptAction.showToast({ message : message })
  return message
}

export function strToArrayBuffer(str: string) {
  let array = new Uint8Array(str.length);
  for ( let i = 0 ; i < str.length ; i ++ ) {
    array[ i ] = str.charCodeAt(i);
  }
  return array.buffer
}