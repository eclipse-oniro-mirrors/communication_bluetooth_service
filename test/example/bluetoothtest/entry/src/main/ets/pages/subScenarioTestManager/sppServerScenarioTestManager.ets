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

import LogUtil from '../../Utils/LogUtil';
import ConfigData from '../../Utils/ConfigData';
import { testItemFromRouterOrPlaceholder} from '../../Utils/common'
import { PageTitle} from '../../Component/pageTitle';
import { TestImageDisplay} from '../../Component/testImageDisplay';
import { TestData} from '../../MainAbility/model/testData'
import { ScenarioContentTable} from '../../Component/scenarioContentTable'
import { initSppServerManagerScenarioData} from '../../MainAbility/model/scenarioTestDataModelManager'
import * as SPPServerManagerInterface from '../../MainAbility/model/sppServerManagerInterface'
import * as BrManagerInterface from '../../MainAbility/model/brManagerInterface'

/**
 * SppServer Scenario Manager Test Page Of Bluetooth test
 */

@Entry
@Component
struct SppServerScenarioTestManager {
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @State changeIndex: number = - 1;
  @State writeServerData: string = '11111';
  private uuid: string = "00001101-0000-1000-8000-00805f9b34fb";
  @StorageLink("sppServerScenarioMessage") sppServerScenarioMessage: string = ""
  @StorageLink('dataReadMessage') dataReadMessage: string = "";

  aboutToAppear(){

    AppStorage.setOrCreate('txtScanFilterDeviceId', "08:FB:EA:1B:3C:63");
    AppStorage.setOrCreate('txtScanFilterName', "dudu-tiger");
    AppStorage.setOrCreate('txtScanFilterServiceUuid', "0000180A-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('txtScanOptions_interval', "0");
    AppStorage.setOrCreate('txtScanOptionsDutyMode', '0');
    AppStorage.setOrCreate('txtScanOptionsMatchMode', '0');
    AppStorage.setOrCreate('cbxBleScanFilter', true);
    AppStorage.setOrCreate('cbxBleScanOptions', true);

    AppStorage.setOrCreate('inputUuid', this.uuid);
    AppStorage.setOrCreate('writeServerData', this.writeServerData);
  }

  build(){
    Column(){
      Stack({alignContent: Alignment.TopStart}){
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
              .accessibilityLevel('yes')
              .id('pages_subScenarioTestManager_sppServerScenarioTestManager__pt_0')
      }

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column(){
        Scroll(){
          Column(){
            Text($r('app.string.bluetoothtest_text_cjlcyz'))
              .fontSize(ConfigData.VP_17)
              .margin({top: ConfigData.VP_10, bottom: ConfigData.VP_10, left: ConfigData.VP_20})
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            ScenarioContentTable({
              scenarioItems: initSppServerManagerScenarioData(),
              changeIndex: this.changeIndex
            })
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_280_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTestManager_sppServerScenarioTestManager__sc_0')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)

        Stack()
          .height(ConfigData.HEIGHT_1_VP)
          .backgroundColor($r('app.color.black'));

        Scroll(){
          Column(){
            Text($r('app.string.bluetoothtest_text_lcrz'))
              .fontSize(ConfigData.VP_17)
              .margin({top: ConfigData.VP_8, bottom: ConfigData.VP_8, left: ConfigData.VP_20})
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            List(){
              ListItem(){
                Text(this.sppServerScenarioMessage)
                  .fontSize(ConfigData.VP_17)
                  .margin({top: ConfigData.VP_5, left: ConfigData.VP_30, right: ConfigData.VP_10})
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
            .accessibilityLevel('yes')
            .id('pages_subScenarioTestManager_sppServerScenarioTestManager__li_0')
            }
            .height(ConfigData.WH_80_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_260_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTestManager_sppServerScenarioTestManager__sc_1')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.On)
      }
      .height(ConfigData.HEIGHT_500_VP)
      .width(ConfigData.WH_100_100)
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column(){
        Flex({alignItems: ItemAlign.Center, justifyContent: FlexAlign.SpaceBetween}){
          Button({type: ButtonType.Normal, stateEffect: true}){
            Text($r('app.string.bluetoothtest_text_cjcsks'))
              .fontSize(ConfigData.VP_24)
              .fontColor(0xffffff)
              .margin({left: ConfigData.VP_5, right: ConfigData.VP_5})
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTestManager_sppServerScenarioTestManager__btn_0')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({top: ConfigData.VP_15, left: ConfigData.VP_20})
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppServerScenarioMessage = ""
            let self = this
            const arr = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 ]
            // 注册状态变化
            sppServerScenarioMessage += BrManagerInterface.testOnStateChangeManager() + "\n"
            LogUtil.info("testOnStateChange success")
            self.changeIndex = arr[ 0 ]
            await sleep(1)

            // 开蓝牙
            sppServerScenarioMessage += BrManagerInterface.testEnableBluetoothManager() + "\n";
            LogUtil.info("testEnableBluetooth success")
            self.changeIndex = arr[ 1 ]
            AppStorage.setOrCreate("sppServerScenarioMessage", sppServerScenarioMessage)
            await sleep(2)

            // 获取状态
            sppServerScenarioMessage += BrManagerInterface.testGetStateManager() + "\n";
            LogUtil.info("testGetState success")
            self.changeIndex = arr[ 2 ]
            await sleep(1)

            // 设置扫描模式
            sppServerScenarioMessage += BrManagerInterface.testSetBluetoothScanModeManager(4, 0) + "\n"
            LogUtil.info("testSetBluetoothScanMode success")
            self.changeIndex = arr[ 3 ]
            await sleep(1)

            // 获取扫描模式
            sppServerScenarioMessage += BrManagerInterface.testGetBluetoothScanModeManager() + "\n"
            LogUtil.info("testGetBluetoothScanMode success")
            self.changeIndex = arr[ 4 ]
            await sleep(1)

            // 开启注册pin码变化
            sppServerScenarioMessage += BrManagerInterface.testOnPinRequiredManager() + "\n"
            LogUtil.info("testOnPinRequired success")
            self.changeIndex = arr[ 5 ]
            await sleep(1)

            // Spp服务器创建
            sppServerScenarioMessage += SPPServerManagerInterface.testSppServerCreateSafeManager() + "\n"
            LogUtil.info("testAddService success")
            self.changeIndex = arr[ 6 ]
            AppStorage.setOrCreate("sppServerScenarioMessage", sppServerScenarioMessage)
            await sleep(5)

            //获取当前连接状态
            sppServerScenarioMessage += BrManagerInterface.testGetBtConnectionStateManager() + "\n"
            LogUtil.info("testGetBtConnectionState success")
            self.changeIndex = arr[ 7 ]
            AppStorage.setOrCreate("sppServerScenarioMessage", sppServerScenarioMessage)
            await sleep(1)

            // Spp服务器接受
            sppServerScenarioMessage += SPPServerManagerInterface.testSppServerAcceptManager() + "\n"
            LogUtil.info("testSppServerAccept success")
            self.changeIndex = arr[ 8 ]
            AppStorage.setOrCreate("sppServerScenarioMessage", sppServerScenarioMessage)
            await sleep(5)

            // Spp服务器读取打开
            sppServerScenarioMessage += SPPServerManagerInterface.testSppServerReadOnManager() + "\n"
            LogUtil.info("testOnCharacteristicRead success")
            self.changeIndex = arr[ 9 ]
            await sleep(3)
            sppServerScenarioMessage += AppStorage.get("dataReadMessage") + "\n"
            AppStorage.setOrCreate("sppServerScenarioMessage", sppServerScenarioMessage)

            // Spp服务器写入
            sppServerScenarioMessage += SPPServerManagerInterface.testSppServerWriteManager() + "\n"
            LogUtil.info("testOnCharacteristicWrite success")
            self.changeIndex = arr[ 10 ]
            await sleep(2)
            AppStorage.setOrCreate("sppServerScenarioMessage", sppServerScenarioMessage)

          })

          Button({type: ButtonType.Normal, stateEffect: true}){
            Row(){
              Text($r('app.string.bluetoothtest_text_cjcsjs'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({left: ConfigData.VP_5, right: ConfigData.VP_5})
            }
              .alignItems(VerticalAlign.Top)
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTestManager_sppServerScenarioTestManager__btn_1')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({top: ConfigData.VP_15, left: ConfigData.VP_10, right: ConfigData.VP_10})
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppServerScenarioMessage = ""
            let self = this
            const arr = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 ]
            // Spp服务器读取关闭
            sppServerScenarioMessage += SPPServerManagerInterface.testSppServerReadOffManager() + "\n"
            LogUtil.info("testOnCharacteristicRead success")
            self.changeIndex = arr[ 11 ]
            await sleep(1)
            // Spp服务器关闭
            sppServerScenarioMessage += SPPServerManagerInterface.testSppCloseServerSocketManager() + "\n"
            LogUtil.info("testSppCloseServerSocket success")
            self.changeIndex = arr[ 12 ]
            await sleep(1)
            // Spp客户端关闭
            sppServerScenarioMessage += SPPServerManagerInterface.testSppCloseClientSocketManager() + "\n"
            LogUtil.info("testSppCloseServerSocket success")
            self.changeIndex = arr[ 13 ]
            await sleep(1)
            // 关闭注册pin码变化
            sppServerScenarioMessage += BrManagerInterface.testOnPinRequiredManager() + "\n"
            LogUtil.info("testOffPinRequired success")
            await sleep(1)
            // 关闭注册状态变化
            sppServerScenarioMessage += BrManagerInterface.testOnStateChangeManager() + "\n"
            LogUtil.info("testOffStateChange success")
            await sleep(1)
            // 关蓝牙
            sppServerScenarioMessage += BrManagerInterface.testDisableBluetoothManager() + "\n"
            LogUtil.info("testDisableBluetooth success")
            self.changeIndex = arr[ 14 ]
            AppStorage.setOrCreate("sppServerScenarioMessage", sppServerScenarioMessage)
            await sleep(1)
          })

          Button({type: ButtonType.Normal, stateEffect: true}){
            Row(){
              Text($r('app.string.bluetoothtest_text_qkrz'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({left: ConfigData.VP_5, right: ConfigData.VP_5})
            }
              .alignItems(VerticalAlign.Top)
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTestManager_sppServerScenarioTestManager__btn_2')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.moon"))
          .width(ConfigData.VP_80)
          .height(ConfigData.VP_66)
          .margin({top: ConfigData.VP_15, right: ConfigData.VP_20})
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppServerScenarioMessage = ""
            AppStorage.setOrCreate("sppServerScenarioMessage", sppServerScenarioMessage)
          })
        }
        .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        .width(ConfigData.WH_100_100)
      }
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
      .width(ConfigData.WH_100_100)
      .height(ConfigData.WH_100_100)
    }
  }
}

async function sleep(time: number): Promise<void> {
  return new Promise<void>((resolve, reject) => {
    setTimeout(() => {
      resolve();
    }, time * 1000);
  });
}

export function strToArrayBuffer(str: string){
  let array = new Uint8Array(str.length);
  for(let i = 0 ; i<str.length ; i++ ){
    array[ i ] = str.charCodeAt(i);
  }
  return array.buffer
}