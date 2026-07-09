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
import { PageTitle} from '../../component/pageTitle';
import { TestImageDisplay} from '../../component/testImageDisplay';
import { TestData} from '../../MainAbility/model/testData'
import { ScenarioContentTable} from '../../component/scenarioContentTable'
import { initGattServerScenarioData} from '../../MainAbility/model/scenarioTestDataModel'
import * as GattServerInterface from '../../MainAbility/model/gattServerInterface'
import * as BrInterface from '../../MainAbility/model/brInterface'

/**
 * GattServer Scenario Test Page Of Bluetooth test
 */

@Entry
@Component
struct GattServerScenarioTest {
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @State changeIndex: number = - 1
  @StorageLink("gattServerScenarioMessage") gattServerScenarioMessage: string = ""
  @StorageLink("characteristicReadMsg") characteristicReadMsg: string =
    "00002BE0-0000-1000-8000-00805F9B34FB";
  @StorageLink("characteristicWriteMsg") characteristicWriteMsg: string = "initial";
  @StorageLink("descriptorReadMsg") descriptorReadMsg: string =
    "00002902-0000-1000-8000-00805F9B34FB";
  @StorageLink("descriptorWriteMsg") descriptorWriteMsg: string = "initial";

  aboutToAppear() {
    AppStorage.setOrCreate('txtScanFilterDeviceId', "08:FB:EA:1B:3C:63");
    AppStorage.setOrCreate('txtScanFilterName', "dudu-tiger");
    AppStorage.setOrCreate('txtScanFilterServiceUuid', "0000180A-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('txtScanOptions_interval', "0");
    AppStorage.setOrCreate('txtScanOptionsDutyMode', '0');
    AppStorage.setOrCreate('txtScanOptionsMatchMode', '0');
    AppStorage.setOrCreate('cbxBleScanFilter', true);
    AppStorage.setOrCreate('cbxBleScanOptions', true);

    AppStorage.setOrCreate('curServiceUUID', '00001877-0000-1000-8000-00805F9B34FB');
    AppStorage.setOrCreate('curCharacteristicUUID', '00002BE0-0000-1000-8000-00805F9B34FB');
    AppStorage.setOrCreate('curDescriptorUUID', '00002902-0000-1000-8000-00805F9B34FB');
    AppStorage.setOrCreate('curDescriptorValue', 'Descriptor');
    AppStorage.setOrCreate('curCharacteristicValue', 'DaYuBlue2');
    AppStorage.setOrCreate('isPrimary', true);
    AppStorage.setOrCreate('advSettings_interval', "1600");
    AppStorage.setOrCreate('advSettings_txPower', "-7");
    AppStorage.setOrCreate('advSettings_connectable', true);
    AppStorage.setOrCreate('advDataServiceUuids_serviceUuid',
      "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advDataManufactureData_manufactureId', "7456");
    AppStorage.setOrCreate('advDataManufactureData_manufactureValue', "CHN");
    AppStorage.setOrCreate('advDataServiceData_serviceUuid',
      "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advDataServiceData_serviceValue', "AdvValue");
    AppStorage.setOrCreate('advResponseServiceUuids_serviceUuid',
      "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advResponseManufactureData_manufactureId', "6547");
    AppStorage.setOrCreate('advResponseManufactureData_manufactureValue', "CHN");
    AppStorage.setOrCreate('advResponseServiceData_serviceUuid',
      "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advResponseServiceData_serviceValue', "AdvValue");
    AppStorage.setOrCreate('cbxAdvSettings', true);
    AppStorage.setOrCreate('cbxInterval', true);
    AppStorage.setOrCreate('cbxTxPower', true);
    AppStorage.setOrCreate('cbxAdvResponse', true);
  }

  build() {
    Column() {
      Stack({alignContent: Alignment.TopStart}) {
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
        .accessibilityLevel('yes')
        .id('pages_subScenarioTest_gattServerScenarioTest__pt_0')
      }

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column() {
        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_cjlcyz'))
              .fontSize(ConfigData.VP_17)
              .margin({
                top: ConfigData.VP_10,
                bottom: ConfigData.VP_10,
                left: ConfigData.VP_20
              })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            ScenarioContentTable({
              scenarioItems: initGattServerScenarioData(),
              changeIndex: this.changeIndex
            })
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_280_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTest_gattServerScenarioTest__sc_0')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)

        Stack()
          .height(ConfigData.HEIGHT_1_VP)
          .backgroundColor($r('app.color.black'));

        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_lcrz'))
              .fontSize(ConfigData.VP_17)
              .margin({
                top: ConfigData.VP_8,
                bottom: ConfigData.VP_8,
                left: ConfigData.VP_20
              })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            List() {
              ListItem() {
                Text(this.gattServerScenarioMessage)
                  .fontSize(ConfigData.VP_17)
                  .margin({
                    top: ConfigData.VP_5,
                    left: ConfigData.VP_30,
                    right: ConfigData.VP_10
                  })
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
            .accessibilityLevel('yes')
            .id('pages_subScenarioTest_gattServerScenarioTest__li_0')
            }
            .height(ConfigData.WH_80_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_260_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subScenarioTest_gattServerScenarioTest__sc_1')
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
        Flex({
          alignItems: ItemAlign.Center,
          justifyContent: FlexAlign.SpaceBetween
        }) {
          Button({
            type: ButtonType.Normal,
            stateEffect: true
          }) {
            Row() {
              Text($r('app.string.bluetoothtest_text_cjcsks'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({
                  left: ConfigData.VP_5,
                  right: ConfigData.VP_5
                })
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTest_gattServerScenarioTest__btn_0')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({
            top: ConfigData.VP_15,
            left: ConfigData.VP_10,
            right: ConfigData.VP_10
          })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            try {
            let self = this
            const arr = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14]

            let gattServerScenarioMessage = ""
            // 注册状态变化
            gattServerScenarioMessage += BrInterface.testOnStateChange() + "\n"
            LogUtil.info("testOnStateChange success")
            self.changeIndex = arr[0]
            await sleep(1)

            // 开蓝牙
            gattServerScenarioMessage += BrInterface.testEnableBluetooth() + "\n";
            LogUtil.info("testEnableBluetooth success")
            self.changeIndex = arr[1]
            AppStorage.setOrCreate("gattServerScenarioMessage", gattServerScenarioMessage)
            await sleep(2)

            // 获取状态
            gattServerScenarioMessage += BrInterface.testGetState() + "\n";
            LogUtil.info("testGetState success")
            self.changeIndex = arr[2]
            await sleep(1)

            //创建Gatt服务器
            gattServerScenarioMessage += GattServerInterface.testCreateGattServer() + "\n"
            LogUtil.info("testCreateGattServer success")
            self.changeIndex = arr[3]
            await sleep(1)

            // 添加服务
            gattServerScenarioMessage += GattServerInterface.testAddService() + "\n"
            LogUtil.info("testAddService success")
            self.changeIndex = arr[4]

            // 开启连接状态变化
            gattServerScenarioMessage += GattServerInterface.testOnConnectStateChange() + "\n"
            LogUtil.info("testOnConnectStateChange success")
            self.changeIndex = arr[5]
            await sleep(1)

            // 开始广播
            gattServerScenarioMessage += GattServerInterface.testStartAdvertising() + "\n"
            LogUtil.info("testStartAdvertising success")
            self.changeIndex = arr[6]
            AppStorage.setOrCreate("gattServerScenarioMessage", gattServerScenarioMessage)
            await sleep(10)

            // 读取特征值
            gattServerScenarioMessage += GattServerInterface.testOnCharacteristicRead() + "\n"
            LogUtil.info("testOnCharacteristicRead success")
            self.changeIndex = arr[7]
            gattServerScenarioMessage += AppStorage.get("characteristicReadMsg") + "\n"
            AppStorage.setOrCreate("gattServerScenarioMessage", gattServerScenarioMessage)
            await sleep(3)

            // 写入特征值
            gattServerScenarioMessage += GattServerInterface.testOnCharacteristicWrite() + "\n"
            LogUtil.info("testOnCharacteristicWrite success")
            self.changeIndex = arr[8]
            gattServerScenarioMessage += AppStorage.get("characteristicWriteMsg") + "\n"
            AppStorage.setOrCreate("gattServerScenarioMessage", gattServerScenarioMessage)
            await sleep(3)

            // 读取描述符值
            gattServerScenarioMessage += GattServerInterface.testOnDescriptorRead() + "\n"
            LogUtil.info("testOnDescriptorRead success")
            self.changeIndex = arr[9]
            gattServerScenarioMessage += AppStorage.get("descriptorReadMsg") + "\n"
            AppStorage.setOrCreate("gattServerScenarioMessage", gattServerScenarioMessage)
            await sleep(3)

            // 写入描述符值
            gattServerScenarioMessage += GattServerInterface.testOnDescriptorWrite() + "\n"
            LogUtil.info("testOnDescriptorWrite success")
            self.changeIndex = arr[10]
            gattServerScenarioMessage += AppStorage.get("descriptorWriteMsg") + "\n"
            AppStorage.setOrCreate("gattServerScenarioMessage", gattServerScenarioMessage)
            await sleep(3)
          
            } catch (e) {
              LogUtil.info(`async onClick failed: ${e.message}, code: ${e.code}`);
            }})

          Button({
            type: ButtonType.Normal,
            stateEffect: true
          }) {
            Row() {
              Text($r('app.string.bluetoothtest_text_cjcsjs'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({
                  left: ConfigData.VP_5,
                  right: ConfigData.VP_5
                })
            }
              .alignItems(VerticalAlign.Top)
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTest_gattServerScenarioTest__btn_1')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({
            top: ConfigData.VP_15,
            left: ConfigData.VP_10,
            right: ConfigData.VP_10
          })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            try {
            let self = this
            const arr = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14]
            let gattServerScenarioMessage = ""
            // 停止广播
            gattServerScenarioMessage += GattServerInterface.testStopAdvertising() + "\n"
            LogUtil.info("testStopAdvertising success")
            self.changeIndex = arr[11]
            await sleep(1)
            // 关闭连接状态变化
            gattServerScenarioMessage += GattServerInterface.testOnConnectStateChange() + "\n"
            LogUtil.info("testOffConnectStateChange success")
            await sleep(1)
            // 删除服务
            gattServerScenarioMessage += GattServerInterface.testRemoveService() + "\n"
            LogUtil.info("testRemoveService success")
            self.changeIndex = arr[12]
            await sleep(1)
            // 关闭服务端
            gattServerScenarioMessage += GattServerInterface.testGattServerClose() + "\n"
            LogUtil.info("testGattServerClose success")
            self.changeIndex = arr[13]
            await sleep(1)
            // 关闭注册状态变化
            gattServerScenarioMessage += BrInterface.testOnStateChange() + "\n"
            LogUtil.info("testOffStateChange success")
            await sleep(1)
            // 关蓝牙
            gattServerScenarioMessage += BrInterface.testDisableBluetooth() + "\n"
            LogUtil.info("testDisableBluetooth success")
            self.changeIndex = arr[14]
            await sleep(1)
            AppStorage.setOrCreate("gattServerScenarioMessage", gattServerScenarioMessage)
          
            } catch (e) {
              LogUtil.info(`async onClick failed: ${e.message}, code: ${e.code}`);
            }})

          Button({
            type: ButtonType.Normal,
            stateEffect: true
          }) {
            Row() {
              Text($r('app.string.bluetoothtest_text_qkrz'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({
                  left: ConfigData.VP_5,
                  right: ConfigData.VP_5
                })
            }
              .alignItems(VerticalAlign.Top)
          }
          .accessibilityLevel('yes')
          .id('pages_subScenarioTest_gattServerScenarioTest__btn_2')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.moon"))
          .width(ConfigData.VP_80)
          .height(ConfigData.VP_66)
          .margin({
            top: ConfigData.VP_15,
            right: ConfigData.VP_20
          })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            try {
            let gattServerScenarioMessage = ""
            AppStorage.setOrCreate("gattServerScenarioMessage", gattServerScenarioMessage)
          
            } catch (e) {
              LogUtil.info(`async onClick failed: ${e.message}, code: ${e.code}`);
            }})
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

export function string2ArrayBuffer(str: string) {
  let array = new Uint8Array(str.length);
  for(let i = 0 ; i<str.length ; i++) {
    array[i] = str.charCodeAt(i);
  }
  return array.buffer
}