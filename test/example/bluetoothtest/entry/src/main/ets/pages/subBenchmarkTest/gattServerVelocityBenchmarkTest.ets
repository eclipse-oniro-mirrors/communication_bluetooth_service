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
import { resStr } from '../../Utils/resString';


import hilog from '@ohos.hilog';
import ConfigData from '../../Utils/ConfigData';
import { testItemFromRouterOrPlaceholder } from '../../Utils/common'
import { PageTitle } from '../../Component/pageTitle';
import { TestImageDisplay } from '../../Component/testImageDisplay';
import { TestData } from '../../MainAbility/model/testData'
import * as GattServerInterface from '../../MainAbility/model/gattServerInterface'
import { testSetBluetoothScanMode , testSetLocalName } from '../../MainAbility/model/brInterface'

/**
 *  Bluetooth GattServer Velocity  Benchmark Test Page Of Bluetooth test
 */

@Entry
@Component
struct GattServerVelocityBenchmarkTest {
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @State changeIndex: number = - 1
  @State gatt_Cycle_index: number = 10
  @StorageLink("gattServerBenchmarkTestMessage") gattServerBenchmarkTestMessage: string = ""

  aboutToAppear() {
    AppStorage.setOrCreate('txtScanFilterDeviceId' , "08:FB:EA:1B:3C:63");
    AppStorage.setOrCreate('txtScanFilterName' , "dudu-tiger");
    AppStorage.setOrCreate('txtScanFilterServiceUuid' , "0000180A-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('txtScanOptions_interval' , "0");
    AppStorage.setOrCreate('txtScanOptionsDutyMode' , '0');
    AppStorage.setOrCreate('txtScanOptionsMatchMode' , '0');
    AppStorage.setOrCreate('cbxBleScanFilter' , false);
    AppStorage.setOrCreate('cbxBleScanOptions' , false);
    AppStorage.setOrCreate('curServiceUUID' , '00001877-0000-1000-8000-00805F9B34FB');
    AppStorage.setOrCreate('curCharacteristicUUID' , '00002BE0-0000-1000-8000-00805F9B34FB');
    AppStorage.setOrCreate('curDescriptorUUID' , '00002902-0000-1000-8000-00805F9B34FB');
    AppStorage.setOrCreate('curDescriptorValue' , 'Descriptor');
    AppStorage.setOrCreate('curCharacteristicValue' , 'DaYuBlue2');
    AppStorage.setOrCreate('isPrimary' , true);
    AppStorage.setOrCreate('advSettings_interval' , "1600");
    AppStorage.setOrCreate('advSettings_txPower' , "-7");
    AppStorage.setOrCreate('advSettings_connectable' , true);
    AppStorage.setOrCreate('advDataServiceUuids_serviceUuid' , "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advDataManufactureData_manufactureId' , "7456");
    AppStorage.setOrCreate('advDataManufactureData_manufactureValue' , "CHN");
    AppStorage.setOrCreate('advDataServiceData_serviceUuid' , "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advDataServiceData_serviceValue' , "AdvValue");
    AppStorage.setOrCreate('advResponseServiceUuids_serviceUuid' , "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advResponseManufactureData_manufactureId' , "6547");
    AppStorage.setOrCreate('advResponseManufactureData_manufactureValue' , "CHN");
    AppStorage.setOrCreate('advResponseServiceData_serviceUuid' , "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advResponseServiceData_serviceValue' , "AdvValue");
    AppStorage.setOrCreate('cbxAdvSettings' , true);
    AppStorage.setOrCreate('cbxInterval' , true);
    AppStorage.setOrCreate('cbxTxPower' , true);
    AppStorage.setOrCreate('cbxAdvResponse' , true);
  }

  build() {
    Column() {
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
              .accessibilityLevel('yes')
              .id('pages_subBenchmarkTest_gattServerVelocityBenchmarkTest__pt_0')
      }

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));

      Column() {
        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_xncs'))
              .fontSize(ConfigData.VP_17)
              .margin({ top : ConfigData.VP_10 , bottom : ConfigData.VP_10 , left : ConfigData.VP_20 })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            Text($r('app.string.bluetoothtest_text_gdxncsscsgldsfnxxbhdslxn'))
              .fontSize(ConfigData.VP_17)
              .margin({ left : ConfigData.VP_15 })
              .backgroundColor($r("app.color.spring"))
            Row() {
              Text($r('app.string.bluetoothtest_text_gci'))
                .fontSize(ConfigData.VP_20)
                .width(ConfigData.VP_180)
                .padding({ left : ConfigData.VP_15 })
              Column() {
                Select([
                  { value : "100" },
                  { value : "100" },
                  { value : "1000" }
                ])
                  .borderRadius(ConfigData.VP_10)
                  .backgroundColor($r("app.color.white"))
                  .selected(0)
                  .value("100")
                  .font({ size : 17 })
                  .margin({ right : ConfigData.VP_120 })
                  .selectedOptionFont({ size : 17 })
                  .optionFont({ size : ConfigData.VP_15 })
                  .width(ConfigData.WH_50_100)
                  .onSelect((index: number) => {
                    hilog.info(0x0000, 'BtTest', '%{public}s', "Select:" + index)
                    AppStorage.setOrCreate('gatt_Cycle_index' , index);
                  })
              }
              .width(ConfigData.WH_90_100)
            }
            .padding(ConfigData.VP_3)
            .justifyContent(FlexAlign.Start)
            .alignItems(VerticalAlign.Center)
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.VP_1300)
        }
        .accessibilityLevel('yes')
        .id('pages_subBenchmarkTest_gattServerVelocityBenchmarkTest__sc_0')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)

        Stack()
          .height(ConfigData.HEIGHT_1_VP)
          .backgroundColor($r('app.color.black'))

        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_rz'))
              .fontSize(ConfigData.VP_17)
              .margin({ top : ConfigData.VP_8 , bottom : ConfigData.VP_8 , left : ConfigData.VP_10 })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            List() {
              ListItem() {
                Text(this.gattServerBenchmarkTestMessage)
                  .fontSize(ConfigData.VP_17)
                  .margin({ top : ConfigData.VP_5 , left : ConfigData.VP_10 , right : ConfigData.VP_10 })
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
            .accessibilityLevel('yes')
            .id('pages_subBenchmarkTest_gattServerVelocityBenchmarkTest__li_0')
            }
            .height(ConfigData.WH_90_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.VP_330)
        }
        .accessibilityLevel('yes')
        .id('pages_subBenchmarkTest_gattServerVelocityBenchmarkTest__sc_1')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.On)
      }
      .width(ConfigData.WH_100_100)
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text($r('app.string.bluetoothtest_text_slxncs'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({ left : ConfigData.VP_5 , right : ConfigData.VP_5 })
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTest_gattServerVelocityBenchmarkTest__btn_0')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , left : ConfigData.VP_5 , right : ConfigData.VP_5 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let gattServerBenchmarkTestMessage = ""
            //创建Gatt服务器
            gattServerBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_12')) + testFunc(GattServerInterface.testCreateGattServer) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)
            //添加服务
            gattServerBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_22')) + testFunc(GattServerInterface.testAddService) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)

            gattServerBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_7')) + testFunc(GattServerInterface.testOnConnectStateChange) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)

            gattServerBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_15')) + testFunc(GattServerInterface.testStartAdvertising) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(20)

            gattServerBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_6')) + testFunc(GattServerInterface.testOnCharacteristicRead) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)

            gattServerBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_5')) + testFunc(GattServerInterface.testOnCharacteristicWrite) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text($r('app.string.bluetoothtest_text_jscs'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({ left : ConfigData.VP_5 , right : ConfigData.VP_5 })
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTest_gattServerVelocityBenchmarkTest__btn_1')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , left : ConfigData.VP_5 , right : ConfigData.VP_5 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let gattServerBenchmarkTestMessage = ""
            gattServerBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_18')) + testFunc(GattServerInterface.testStopAdvertising) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)

            gattServerBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_20')) + testFunc(GattServerInterface.testRemoveService) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)

            gattServerBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_17')) + testFunc(GattServerInterface.testGattServerClose) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)
          })


          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Image($r('app.media.ic_public_delete_filled'))
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTest_gattServerVelocityBenchmarkTest__img_0').width(ConfigData.VP_30).height(ConfigData.VP_40).margin({ left : ConfigData.VP_5 })
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTest_gattServerVelocityBenchmarkTest__btn_2')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.moon"))
          .width(ConfigData.VP_50)
          .height(ConfigData.VP_66)
          .margin({ top : ConfigData.VP_15 , right : ConfigData.VP_40 })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let gattServerBenchmarkTestMessage = ""
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
          })
        }
        .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        .width(ConfigData.WH_100_100)
      }
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
      .width(ConfigData.WH_100_100)
      .height(ConfigData.WH_100_100);
    }
  }
}


export function test(func: Function) {
  let start = new Date().getTime(); //起始时间
  func(); //执行待测函数
  let end = new Date().getTime(); //接受时间
  return (end - start) + "ms"; //返回函数执行需要时间
}

export function testFunc(func: Function) {
  let start = new Date().getTime()
  switch ( func ) {
    case testSetLocalName: {
      func("DaYuBlue")
    }
      break;
    case testSetBluetoothScanMode: {
      func(4 , 0)
    }
      break;
    default: {
      func()
    }
      break;
  }

  let end = new Date().getTime()
  hilog.info(0x0000, 'BtTest', '%{public}s', resStr($r('app.string.bluetoothtest_str_ks')) + Number(start) + "ms")
  hilog.info(0x0000, 'BtTest', '%{public}s', resStr($r('app.string.bluetoothtest_str_js')) + Number(end) + "ms")
  hilog.info(0x0000, 'BtTest', '%{public}s', resStr($r('app.string.bluetoothtest_str_hf')) + Number(end - start) + "ms")
  let message = ""
  message += resStr($r('app.string.bluetoothtest_str_hf')) + Number(end - start) + "ms" + "\n"
  message += resStr($r('app.string.bluetoothtest_str_ks')) + Number(start) + resStr($r('app.string.bluetoothtest_str_m_1')) + resStr($r('app.string.bluetoothtest_str_js')) + Number(end) + "ms" + "\n"
  return message
}

async function sleep(time: number): Promise<void> {
  return new Promise<void>((resolve , reject) => {
    setTimeout(() => {
      resolve();
    } , time * 1000);
  });
}
