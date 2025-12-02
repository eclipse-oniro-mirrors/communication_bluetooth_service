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
import { initSppServerManagerScenarioData } from '../../MainAbility/model/scenarioTestDataModelManager'
import * as SPPServerManagerInterface from '../../MainAbility/model/sppServerManagerInterface'
import * as BrManagerInterface from '../../MainAbility/model/brManagerInterface'

/**
 * SppServer Scenario Manager Test Page Of Bluetooth test
 */

@Entry
@Component
struct SppServerScenarioTestManager {
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
              scenarioItems : initSppServerManagerScenarioData() ,
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
                Text(this.sppServerScenarioMessage)
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
            let sppServerScenarioMessage = ""
            let self = this
            const arr = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 ]
            // 注册状态变化
            sppServerScenarioMessage += BrManagerInterface.testOnStateChangeManager() + "\n"
            console.log("testOnStateChange success")
            self.changeIndex = arr[ 0 ]
            await sleep(1)

            // 开蓝牙
            sppServerScenarioMessage += BrManagerInterface.testEnableBluetoothManager() + "\n";
            console.log("testEnableBluetooth success")
            self.changeIndex = arr[ 1 ]
            AppStorage.setOrCreate("sppServerScenarioMessage" , sppServerScenarioMessage)
            await sleep(2)

            // 获取状态
            sppServerScenarioMessage += BrManagerInterface.testGetStateManager() + "\n";
            console.log("testGetState success")
            self.changeIndex = arr[ 2 ]
            await sleep(1)

            // 设置扫描模式
            sppServerScenarioMessage += BrManagerInterface.testSetBluetoothScanModeManager(4 , 0) + "\n"
            console.log("testSetBluetoothScanMode success")
            self.changeIndex = arr[ 3 ]
            await sleep(1)

            // 获取扫描模式
            sppServerScenarioMessage += BrManagerInterface.testGetBluetoothScanModeManager() + "\n"
            console.log("testGetBluetoothScanMode success")
            self.changeIndex = arr[ 4 ]
            await sleep(1)

            // 开启注册pin码变化
            sppServerScenarioMessage += BrManagerInterface.testOnPinRequiredManager() + "\n"
            console.log("testOnPinRequired success")
            self.changeIndex = arr[ 5 ]
            await sleep(1)

            // Spp服务器创建
            sppServerScenarioMessage += SPPServerManagerInterface.testSppServerCreateSafeManager() + "\n"
            console.log("testAddService success")
            self.changeIndex = arr[ 6 ]
            AppStorage.setOrCreate("sppServerScenarioMessage" , sppServerScenarioMessage)
            await sleep(5)

            //获取当前连接状态
            sppServerScenarioMessage += BrManagerInterface.testGetBtConnectionStateManager() + "\n"
            console.log("testGetBtConnectionState success")
            self.changeIndex = arr[ 7 ]
            AppStorage.setOrCreate("sppServerScenarioMessage" , sppServerScenarioMessage)
            await sleep(1)

            // Spp服务器接受
            sppServerScenarioMessage += SPPServerManagerInterface.testSppServerAcceptManager() + "\n"
            console.log("testSppServerAccept success")
            self.changeIndex = arr[ 8 ]
            AppStorage.setOrCreate("sppServerScenarioMessage" , sppServerScenarioMessage)
            await sleep(5)

            // Spp服务器读取打开
            sppServerScenarioMessage += SPPServerManagerInterface.testSppServerReadOnManager() + "\n"
            console.log("testOnCharacteristicRead success")
            self.changeIndex = arr[ 9 ]
            await sleep(3)
            sppServerScenarioMessage += AppStorage.get("dataReadMessage") + "\n"
            AppStorage.setOrCreate("sppServerScenarioMessage" , sppServerScenarioMessage)

            // Spp服务器写入
            sppServerScenarioMessage += SPPServerManagerInterface.testSppServerWriteManager() + "\n"
            console.log("testOnCharacteristicWrite success")
            self.changeIndex = arr[ 10 ]
            await sleep(2)
            AppStorage.setOrCreate("sppServerScenarioMessage" , sppServerScenarioMessage)

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
            let sppServerScenarioMessage = ""
            let self = this
            const arr = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 ]
            // Spp服务器读取关闭
            sppServerScenarioMessage += SPPServerManagerInterface.testSppServerReadOffManager() + "\n"
            console.log("testOnCharacteristicRead success")
            self.changeIndex = arr[ 11 ]
            await sleep(1)
            // Spp服务器关闭
            sppServerScenarioMessage += SPPServerManagerInterface.testSppCloseServerSocketManager() + "\n"
            console.log("testSppCloseServerSocket success")
            self.changeIndex = arr[ 12 ]
            await sleep(1)
            // Spp客户端关闭
            sppServerScenarioMessage += SPPServerManagerInterface.testSppCloseClientSocketManager() + "\n"
            console.log("testSppCloseServerSocket success")
            self.changeIndex = arr[ 13 ]
            await sleep(1)
            // 关闭注册pin码变化
            sppServerScenarioMessage += BrManagerInterface.testOnPinRequiredManager() + "\n"
            console.log("testOffPinRequired success")
            await sleep(1)
            // 关闭注册状态变化
            sppServerScenarioMessage += BrManagerInterface.testOnStateChangeManager() + "\n"
            console.log("testOffStateChange success")
            await sleep(1)
            // 关蓝牙
            sppServerScenarioMessage += BrManagerInterface.testDisableBluetoothManager() + "\n"
            console.log("testDisableBluetooth success")
            self.changeIndex = arr[ 14 ]
            AppStorage.setOrCreate("sppServerScenarioMessage" , sppServerScenarioMessage)
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