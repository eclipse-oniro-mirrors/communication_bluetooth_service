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
import * as GattServerManagerInterface from '../../MainAbility/model/gattServerManagerInterface'
import { testSetBluetoothScanModeManager , testSetLocalNameManager } from '../../MainAbility/model/brManagerInterface'

/**
 *  Bluetooth GattServer Manager Velocity Benchmark Test Page Of Bluetooth test
 */

@Entry
@Component
struct GattServerVelocityBenchmarkTestManager {
  private testItem: TestData = (router.getParams() as myParams).testItem
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
      }

      Stack().height("1vp").backgroundColor("#000000");

      Column() {
        Scroll() {
          Column() {
            Text("性能测试:")
              .fontSize("17vp")
              .margin({ top : "10vp" , bottom : "10vp" , left : "20vp" })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            Text("Gatt的性能测试是测试Gatt两端收发notify信息变化的速率性能")
              .fontSize("17vp")
              .margin({ left : "15vp" })
              .backgroundColor($r("app.color.spring"))
            Row() {
              Text("gatt_Cycle_index:").fontSize(20).width(180).padding({ left : "15vp" })
              Column() {
                Select([
                  { value : "100" },
                  { value : "100" },
                  { value : "1000" }
                ])
                  .borderRadius(10)
                  .backgroundColor($r("app.color.white"))
                  .selected(0)
                  .value("100")
                  .font({ size : 17 })
                  .margin({ right : 120 })
                  .selectedOptionFont({ size : 17 })
                  .optionFont({ size : 15 })
                  .width($r('app.float.wh_value_50p'))
                  .onSelect((index: number) => {
                    console.log("Select:" + index)
                    AppStorage.setOrCreate('gatt_Cycle_index' , index);
                  })
              }
              .width($r('app.float.wh_value_90p'))
            }
            .padding(3)
            .justifyContent(FlexAlign.Start)
            .alignItems(VerticalAlign.Center)
          }
          .width($r('app.float.wh_value_100p'))
          .height(130)
        }
        .scrollBarWidth(10)
        .scrollBar(BarState.Auto)

        Stack().height("1vp").backgroundColor("#000000")

        Scroll() {
          Column() {
            Text("日志:")
              .fontSize("17vp")
              .margin({ top : "8vp" , bottom : "8vp" , left : "10vp" })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            List() {
              ListItem() {
                Text(this.gattServerBenchmarkTestMessage)
                  .fontSize("17vp")
                  .margin({ top : "5vp" , left : "10vp" , right : "10vp" })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
              }
            }
            .height($r('app.float.wh_value_90p'))
            .width($r('app.float.wh_value_90p'))
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width($r('app.float.wh_value_100p'))
          .height(330)
        }
        .scrollBarWidth(10)
        .scrollBar(BarState.On)
      }
      .width($r('app.float.wh_value_100p'))
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('速率性能测试').fontSize("24vp").fontColor(0xffffff).margin({ left : 5 , right : 5 })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.blue"))
          .width(100)
          .height(66)
          .margin({ top : "15vp" , left : "5vp" , right : "5vp" })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let gattServerBenchmarkTestMessage = ""
            //创建Gatt服务器
            gattServerBenchmarkTestMessage += "testCreateGattServer 执行:" + testFunc(GattServerManagerInterface.testCreateGattServerManager) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)
            //添加服务
            gattServerBenchmarkTestMessage += "testAddService 执行:" + testFunc(GattServerManagerInterface.testAddServiceManager) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)

            gattServerBenchmarkTestMessage += "testOnConnectStateChange 执行:" + testFunc(GattServerManagerInterface.testOnConnectStateChangeManager) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)

            gattServerBenchmarkTestMessage += "testStartAdvertising 执行:" + testFunc(GattServerManagerInterface.testStartAdvertisingManager) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(20)

            gattServerBenchmarkTestMessage += "testOnCharacteristicRead 执行:" + testFunc(GattServerManagerInterface.testOnCharacteristicReadManager) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)

            gattServerBenchmarkTestMessage += "testOnCharacteristicWrite 执行:" + testFunc(GattServerManagerInterface.testOnCharacteristicWriteManager) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('结束测试').fontSize("24vp").fontColor(0xffffff).margin({ left : 5 , right : 5 })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.blue"))
          .width(100)
          .height(66)
          .margin({ top : "15vp" , left : "5vp" , right : "5vp" })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let gattServerBenchmarkTestMessage = ""
            gattServerBenchmarkTestMessage += "testStopAdvertising 执行：" + testFunc(GattServerManagerInterface.testStopAdvertisingManager) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)

            gattServerBenchmarkTestMessage += "testRemoveService 执行：" + testFunc(GattServerManagerInterface.testRemoveServiceManager) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)

            gattServerBenchmarkTestMessage += "testGattServerClose 执行：" + testFunc(GattServerManagerInterface.testGattServerCloseManager) + "\n"
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
            await sleep(2)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Image($r('app.media.ic_public_delete_filled')).width(30).height(40).margin({ left : 5 })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.moon"))
          .width(50)
          .height(66)
          .margin({ top : "15vp" , right : "40vp" })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let gattServerBenchmarkTestMessage = ""
            AppStorage.setOrCreate("gattServerBenchmarkTestMessage" , gattServerBenchmarkTestMessage)
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

export function test(func: Function) {
  let start = new Date().getTime(); //起始时间
  func(); //执行待测函数
  let end = new Date().getTime(); //接受时间
  return (end - start) + "ms"; //返回函数执行需要时间
}

export function testFunc(func: Function) {
  let start = new Date().getTime()
  switch ( func ) {
    case testSetLocalNameManager: {
      func("DaYuBlue")
    }
      break;
    case testSetBluetoothScanModeManager: {
      func(4 , 0)
    }
      break;
    default: {
      func()
    }
      break;
  }

  let end = new Date().getTime()
  console.log("开始:" + Number(start) + "ms")
  console.log("结束:" + Number(end) + "ms")
  console.log("花费:" + Number(end - start) + "ms")
  let message = ""
  message += "花费:" + Number(end - start) + "ms" + "\n"
  message += "开始:" + Number(start) + "ms; " + "结束:" + Number(end) + "ms" + "\n"
  return message
}

async function sleep(time: number): Promise<void> {
  return new Promise<void>((resolve , reject) => {
    setTimeout(() => {
      resolve();
    } , time * 1000);
  });
}
