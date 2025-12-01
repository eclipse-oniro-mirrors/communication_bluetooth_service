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
import * as GattClientInterface from '../../MainAbility/model/gattClientInterface'
import { testOnBLECharacteristicChange } from '../../MainAbility/model/gattClientInterface'
import { testSetBluetoothScanMode , testSetLocalName } from '../../MainAbility/model/brInterface'
import bluetooth from '@ohos.bluetooth';
import { BusinessError } from '@ohos.base';
import promptAction from '@ohos.promptAction';

let gattClientInstance: bluetooth.GattClientDevice

/**
 *  Bluetooth GattClient Velocity Benchmark Test Page Of Bluetooth test
 */

@Entry
@Component
struct GattClientVelocityBenchmarkTest {
  private testItem: TestData = (router.getParams() as myParams).testItem
  @State changeIndex: number = - 1
  @State gatt_Cycle_index: number = 10
  @StorageLink("gattClientBenchmarkTestMessage") gattClientBenchmarkTestMessage: string = ""
  private peripheralDeviceId = "08:FB:EA:1B:3C:63"
  @State serviceUUID: string = "00001801-0000-1000-8000-00805f9b34fb";
  @State characteristicUUID: string = "00002b29-0000-1000-8000-00805f9b34fb";
  @State characteristicValue: string = "CccValue";
  @State descriptorUUID: string = "00002902-0000-1000-8000-00805f9b34fb";
  @State descriptorValue: string = "DesValue";

  aboutToAppear() {
    AppStorage.setOrCreate("peripheralDeviceId" , this.peripheralDeviceId);
    AppStorage.setOrCreate("serviceUUID" , this.serviceUUID);
    AppStorage.setOrCreate("characteristicUUID" , this.characteristicUUID);
    AppStorage.setOrCreate("characteristicValue" , this.characteristicValue);
    AppStorage.setOrCreate("descriptorUUID" , this.descriptorUUID);
    AppStorage.setOrCreate("descriptorValue" , this.descriptorValue);
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
            Text("性能测试:")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_10') , bottom : $r('app.float.wh_value_10') , left : $r('app.float.wh_value_20') })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            Text("Gatt的性能测试是测试Gatt两端收发notify信息变化的速率性能")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ left : $r('app.float.wh_value_15') })
              .backgroundColor($r("app.color.spring"))
            Row() {
              Text("gatt_Cycle_index:").fontSize($r('app.float.wh_value_20')).width($r('app.float.wh_value_180')).padding({ left : $r('app.float.wh_value_15') })
              Column() {
                Select([
                  { value : "100" },
                  { value : "1000" },
                  { value : "10000" }
                ])
                  .borderRadius(10)
                  .backgroundColor($r("app.color.white"))
                  .selected(0)
                  .value("100")
                  .font({ size : 17 })
                  .margin({ right : 80 })
                  .selectedOptionFont({ size : 17 })
                  .optionFont({ size : 15 })
                  .width($r('app.float.wh_value_50p'))
                  .onSelect((index: number) => {
                    console.log("Select:" + index)
                    AppStorage.setOrCreate("gatt_Cycle_index" , index);
                  })
              }
              .width($r('app.float.wh_value_90p'))
            }
            .padding($r('app.float.wh_value_3'))
            .justifyContent(FlexAlign.Start)
            .alignItems(VerticalAlign.Center)
          }
          .width($r('app.float.wh_value_100p'))
          .height($r('app.float.wh_value_130'))
        }
        .scrollBarWidth(10)
        .scrollBar(BarState.Auto)

        Stack().height($r('app.float.wh_value_1')).backgroundColor("#000000")

        Scroll() {
          Column() {
            Text("日志:")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_8') , bottom : $r('app.float.wh_value_8') , left : $r('app.float.wh_value_10') })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            List() {
              ListItem() {
                Text(this.gattClientBenchmarkTestMessage)
                  .fontSize($r('app.float.wh_value_17'))
                  .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_10') , right : $r('app.float.wh_value_10') })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_90p'))
              }
            }
            .height($r('app.float.wh_value_90p'))
            .width($r('app.float.wh_value_90p'))
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width($r('app.float.wh_value_100p'))
          .height($r('app.float.wh_value_330'))
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
              Text("速率性能测试").fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : 5 , right : 5 })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.blue"))
          .width($r('app.float.wh_value_100'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , left : $r('app.float.wh_value_5') , right : $r('app.float.wh_value_5') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let gattClientBenchmarkTestMessage = ""
            //创建Gatt客户端
            gattClientBenchmarkTestMessage += "testCreateGattClient 执行：" + testFunc(GattClientInterface.testCreateGattClient) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage" , gattClientBenchmarkTestMessage)
            await sleep(5)

            //BLE连接状态
            gattClientBenchmarkTestMessage += "testOnBLEConnectionStateChange 执行：" + testFunc(GattClientInterface.testOnBLEConnectionStateChange) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage" , gattClientBenchmarkTestMessage)
            await sleep(2)

            //连接
            gattClientBenchmarkTestMessage += "testConnect 执行：" + testFunc(GattClientInterface.testConnect) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage" , gattClientBenchmarkTestMessage)
            await sleep(5)

            //设置通知特性更改
            gattClientBenchmarkTestMessage += "testSetNotifyCharacteristicChanged 执行：" + testFunc(GattClientInterface.testSetNotifyCharacteristicChanged) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage" , gattClientBenchmarkTestMessage)
            await sleep(3)

            //BLE特征值变化
            gattClientBenchmarkTestMessage += "testOnBLECharacteristicChange 执行：" + testFunc(testOnBLECharacteristicChange) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage" , gattClientBenchmarkTestMessage)
            await sleep(2)

            //获取server
            gattClientBenchmarkTestMessage += "testGetServicesCallback 执行：" + testFunc(GattClientInterface.testGetServicesCallback) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage" , gattClientBenchmarkTestMessage)
            await sleep(2)

            //获取特征值
            gattClientBenchmarkTestMessage += "testReadCharacteristicValueCallback 执行：" + testFunc(GattClientInterface.testReadCharacteristicValueCallback) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage" , gattClientBenchmarkTestMessage)
            await sleep(2)

            //写入特征值
            gattClientBenchmarkTestMessage += "testWriteCharacteristicValue 执行：" + testFunc(writeCharacteristicValue) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage" , gattClientBenchmarkTestMessage)
            await sleep(2)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text("结束测试").fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : 5 , right : 5 })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.blue"))
          .width($r('app.float.wh_value_100'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , left : $r('app.float.wh_value_5') , right : $r('app.float.wh_value_5') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let gattClientBenchmarkTestMessage = ""

            gattClientBenchmarkTestMessage += "testDisconnect 执行：" + testFunc(GattClientInterface.testDisconnect) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage" , gattClientBenchmarkTestMessage)
            await sleep(2)

            gattClientBenchmarkTestMessage += "testGattClientClose 执行：" + testFunc(GattClientInterface.testGattClientClose) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage" , gattClientBenchmarkTestMessage)
            await sleep(2)
          })


          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Image($r("app.media.ic_public_delete_filled")).width($r('app.float.wh_value_30')).height($r('app.float.wh_value_40')).margin({ left : 5 })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius(8)
          .backgroundColor($r("app.color.moon"))
          .width($r('app.float.wh_value_50'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , right : $r('app.float.wh_value_40') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let gattClientBenchmarkTestMessage = ""
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage" , gattClientBenchmarkTestMessage)
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

let i = 1

function writeCharacteristicValue(): string {
  let serviceUUID: string = AppStorage.get('serviceUUID') !;
  let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
  let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
  let descriptorValue: string = AppStorage.get('descriptorValue') !;
  let message = "writeCharacteristicValue test"
  let btState = bluetooth.getState();
  if ( btState === bluetooth.BluetoothState.STATE_OFF ) {
    message = "BT is not enabled!";
    promptAction.showToast({ message : message })
    return message;
  }
  if ( !gattClientInstance ) {
    message = "NO_GATT_CLIENT_OBJECT";
    promptAction.showToast({ message : message })
    return message;
  }
  if ( i < 101 ) {
    let descriptors: Array<bluetooth.BLEDescriptor> = [];
    let bufferCCC = new ArrayBuffer(8);
    let cccV = new Uint8Array(bufferCCC);
    cccV[ 0 ] = 66;
    let descriptor: bluetooth.BLEDescriptor = {
      serviceUuid : serviceUUID ,
      characteristicUuid : characteristicUUID ,
      descriptorUuid : descriptorUUID ,
      descriptorValue : string2ArrayBuffer(descriptorValue)
    }
    descriptors.push(descriptor);
    let bleCharacteristicDataIn: bluetooth.BLECharacteristic = {
      serviceUuid : serviceUUID ,
      characteristicUuid : characteristicUUID ,
      characteristicValue : bufferCCC ,
      descriptors : descriptors
    };
    if ( gattClientInstance.writeCharacteristicValue(bleCharacteristicDataIn) ) {
      message = "writeCharacteristicValue succeeds.";
      readCharacteristicValueCallback()
    } else {
      message = "writeCharacteristicValue failed!";
    }
  }
  promptAction.showToast({ message : message })
  return message
}

function readCharacteristicValueCallback(): string {
  let serviceUUID: string = AppStorage.get('serviceUUID') !;
  let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
  let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
  let descriptorValue: string = AppStorage.get('descriptorValue') !;
  let message = "readCharacteristicValueCallback test"
  let btState = bluetooth.getState();
  if ( btState === bluetooth.BluetoothState.STATE_OFF ) {
    message = "BT is not enabled!";
    promptAction.showToast({ message : message })
    return message;
  }
  if ( !gattClientInstance ) {
    message = "NO_GATT_CLIENT_OBJECT";
    promptAction.showToast({ message : message })
    return message;
  }
  let descriptors: Array<bluetooth.BLEDescriptor> = [];
  let bufferCCC = new ArrayBuffer(8);
  let cccV = new Uint8Array(bufferCCC);
  cccV[ 0 ] = 66;
  let descriptor: bluetooth.BLEDescriptor = {
    serviceUuid : serviceUUID ,
    characteristicUuid : characteristicUUID ,
    descriptorUuid : descriptorUUID ,
    descriptorValue : string2ArrayBuffer(descriptorValue)
  }
  descriptors.push(descriptor);
  let bleCharacteristicDataIn: bluetooth.BLECharacteristic = {
    serviceUuid : serviceUUID ,
    characteristicUuid : characteristicUUID ,
    characteristicValue : bufferCCC ,
    descriptors : descriptors
  };
  gattClientInstance.readCharacteristicValue(bleCharacteristicDataIn , (err: BusinessError , bleCharacteristicDataOut: bluetooth.BLECharacteristic) => {
    if ( err.code != 0 ) {
      message = "readCharacteristicValue error code:" + err.code + ",id:" + serviceUUID;
      console.log(message)
      promptAction.showToast({ message : message })
    }
    message = "readCharacteristicValue callback:";
    message += 'bluetooth characteristic uuid:' + bleCharacteristicDataOut.characteristicUuid + "\n";
    let value = new Uint8Array(bleCharacteristicDataOut.characteristicValue);
    for ( let i = 0 ; i < bleCharacteristicDataOut.characteristicValue.byteLength ; i ++ ) {
      message += 'bluetooth characteristic value: ' + value[ i ];
    }
    AppStorage.setOrCreate("read_CharacteristicValue" , message)
    promptAction.showToast({ message : message })
    i ++
    writeCharacteristicValue()
  });
  message = 'readCharacteristicValue(callback):invoke succeeds!';
  promptAction.showToast({ message : message })
  return message
}

function string2ArrayBuffer(str: string) {
  let array = new Uint8Array(str.length);
  for ( let i = 0 ; i < str.length ; i ++ ) {
    array[ i ] = str.charCodeAt(i);
  }
  return array.buffer
}
