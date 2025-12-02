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
import * as SppClientManagerInterface from '../../MainAbility/model/sppClientManagerInterface'
import bluetoothManager from '@ohos.bluetoothManager';
import { testSetBluetoothScanModeManager , testSetLocalNameManager } from '../../MainAbility/model/brManagerInterface'
import bluetooth from '@ohos.bluetooth';
import LogUtil from '../../Utils/LogUtil';
import promptAction from '@ohos.promptAction';

/**
 *  Bluetooth SppClient BandWidth Benchmark Manager Test Page Of Bluetooth test
 */

@Entry
@Component
struct SppClientBandWidthBenchmarkTestManager {
  @State message: string = 'SppClientBenchmarkTestManager';
  private testItem: TestData = (router.getParams() as myParams).testItem
  @State changeIndex: number = - 1
  @StorageLink('dataClientReadMessageBW') dataClientReadMessageBW: string = "00";
  @StorageLink("sppClientBandWidthBenchmarkTestMessage") sppClientBandWidthBenchmarkTestMessage: string = ""
  @State writeClientDataBW: string = '1';
  private clientNumber: number = - 1;
  private uuid: string = "00001101-0000-1000-8000-00805f9b34fb";
  @State deviceId: string = "08:FB:EA:1B:3C:63"; //08:FB:EA:1B:3C:63

  aboutToAppear() {

    AppStorage.setOrCreate('inputDeviceId' , this.deviceId);
    AppStorage.setOrCreate('inputUuid' , this.uuid);
    AppStorage.setOrCreate("writeClientDataBW" , this.writeClientDataBW);
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
            Text("性能测试:")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_5') , bottom : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_20') })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
            Text("Spp的性能测试是测试Spp两端收发信息的速率和带宽性能")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ left : $r('app.float.wh_value_15') })
              .backgroundColor($r("app.color.spring"))

            Stack().height($r('app.float.wh_value_1')).backgroundColor($r('app.color.black'))

            Row() {
              Text("目标设备MAC：").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_130'))
              TextInput({ text : this.deviceId , placeholder : "请输入mac地址" })
                .fontSize($r('app.float.wh_value_17'))
                .onChange((str) => {
                  this.deviceId = str;
                  AppStorage.setOrCreate('inputDeviceId' , str)
                })
                .width($r('app.float.wh_value_80p'))
                .borderRadius($r('app.float.wh_value_1'))
                .backgroundColor($r("app.color.white"))
                .height($r('app.float.wh_value_30'))
            }
            .padding($r('app.float.wh_value_5'))
            .justifyContent(FlexAlign.Start)
            .alignItems(VerticalAlign.Center)

            Row() {
              Text("写入目标设备数据：").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_160')).padding({ left : $r('app.float.wh_value_5') });
              TextInput({ text : this.writeClientDataBW , placeholder : "请输入写入的数据" })
                .align(Alignment.Center)
                .fontSize($r('app.float.wh_value_15'))
                .backgroundColor($r("app.color.white"))
                .onChange((str) => {
                  this.writeClientDataBW = str;
                  AppStorage.setOrCreate("writeClientDataBW" , str);
                })
                .width($r('app.float.wh_value_50p'))
                .borderRadius($r('app.float.wh_value_10'))
                .height($r('app.float.wh_value_32'))
            }
            .padding($r('app.float.wh_value_2'))
            .justifyContent(FlexAlign.Start)
            .alignItems(VerticalAlign.Center)

            Scroll() {
              Column() {
                Text("读取的数据为：")
                  .fontSize($r('app.float.wh_value_17'))
                  .margin({ left : $r('app.float.wh_value_10') })
                  .textAlign(TextAlign.Start)
                  .width($r('app.float.wh_value_100p'))
                Scroll() {
                  List() {
                    ListItem() {
                      Text(this.dataClientReadMessageBW)
                        .align(Alignment.TopStart)
                        .fontSize($r('app.float.wh_value_14'))
                        .width($r('app.float.wh_value_90p'))
                        .height($r('app.float.wh_value_150'))
                        .borderRadius($r('app.float.wh_value_10'))
                        .backgroundColor($r("app.color.white"))
                    }
                  }
                  .height($r('app.float.wh_value_95p'))
                  .width($r('app.float.wh_value_90p'))
                  .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
                }
                .scrollBarWidth($r('app.float.wh_value_10'))
                .scrollBar(BarState.On)
              }
              .width($r('app.float.wh_value_100p'))
              .height($r('app.float.wh_value_150'))
            }
            .scrollBarWidth($r('app.float.wh_value_10'))
            .scrollBar(BarState.On)
          }
          .width($r('app.float.wh_value_100p'))
          .height($r('app.float.wh_value_360'))
          .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        }
        .scrollBarWidth($r('app.float.wh_value_10'))
        .scrollBar(BarState.Auto)

        Stack().height($r('app.float.wh_value_1')).backgroundColor($r('app.color.black'))

        Scroll() {
          Column() {
            Text("日志:")
              .fontSize($r('app.float.wh_value_17'))
              .margin({ top : $r('app.float.wh_value_8') , bottom : $r('app.float.wh_value_8') , left : $r('app.float.wh_value_10') })
              .textAlign(TextAlign.Start)
              .width($r('app.float.wh_value_100p'))
              .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
            List() {
              ListItem() {
                Text(this.sppClientBandWidthBenchmarkTestMessage)
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
          .height($r('app.float.wh_value_150'))
        }
        .scrollBarWidth($r('app.float.wh_value_10'))
        .scrollBar(BarState.On)
      }
      .width($r('app.float.wh_value_100p'))
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('带宽性能测试').fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : $r('app.float.wh_value_5') , right : $r('app.float.wh_value_5') })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius($r('app.float.wh_value_8'))
          .backgroundColor($r("app.color.blue"))
          .width($r('app.float.wh_value_100'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , left : $r('app.float.wh_value_5') , right : $r('app.float.wh_value_5') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppClientBandWidthBenchmarkTestMessage = ""
            let writeClientDataBW = loopWriteDate()
            AppStorage.setOrCreate("writeClientDataBW" , writeClientDataBW)
            await sleep(2)
            sppClientBandWidthBenchmarkTestMessage += "sppClientConnectSafe" + testFunc(sppClientConnectSafe)
            AppStorage.setOrCreate("sppClientBandWidthBenchmarkTestMessage" , sppClientBandWidthBenchmarkTestMessage)
            await sleep(5)

            sppClientBandWidthBenchmarkTestMessage += "sppClientWrite" + testFunc(sppClientWrite)
            AppStorage.setOrCreate("sppClientBandWidthBenchmarkTestMessage" , sppClientBandWidthBenchmarkTestMessage)
            await sleep(5)

          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('结束测试').fontSize($r('app.float.wh_value_24')).fontColor(0xffffff).margin({ left : $r('app.float.wh_value_5') , right : $r('app.float.wh_value_5') })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius($r('app.float.wh_value_8'))
          .backgroundColor($r("app.color.blue"))
          .width($r('app.float.wh_value_80'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , left : $r('app.float.wh_value_5') , right : $r('app.float.wh_value_5') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppClientBandWidthBenchmarkTestMessage = ""

            sppClientBandWidthBenchmarkTestMessage += "testSppClientReadOff 执行：" + testFunc(SppClientManagerInterface.testSppClientReadOffManager) + "\n"
            AppStorage.setOrCreate("sppClientBandWidthBenchmarkTestMessage" , sppClientBandWidthBenchmarkTestMessage)
            await sleep(1)

            sppClientBandWidthBenchmarkTestMessage += "testSppClientClose 执行：" + testFunc(SppClientManagerInterface.testSppClientCloseManager) + "\n"
            AppStorage.setOrCreate("sppClientBandWidthBenchmarkTestMessage" , sppClientBandWidthBenchmarkTestMessage)
            await sleep(1)

          })


          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Image($r('app.media.ic_public_delete_filled')).width($r('app.float.wh_value_30')).height($r('app.float.wh_value_40')).margin({ left : $r('app.float.wh_value_5') })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius($r('app.float.wh_value_8'))
          .backgroundColor($r("app.color.moon"))
          .width($r('app.float.wh_value_50'))
          .height($r('app.float.wh_value_66'))
          .margin({ top : $r('app.float.wh_value_15') , right : $r('app.float.wh_value_40') })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppClientBandWidthBenchmarkTestMessage = ""
            AppStorage.setOrCreate("sppClientBandWidthBenchmarkTestMessage" , sppClientBandWidthBenchmarkTestMessage)
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
      this.dataClientReadMessageBW = "读取成功" + JSON.stringify(data);
    } else {
      this.dataClientReadMessageBW = "读取失败";
    }
  }

  sppClientClose() {
    LogUtil.info('sppCloseClientSocket clientNumber: ' + JSON.stringify(this.clientNumber))
    bluetoothManager.sppCloseClientSocket(this.clientNumber);
    this.message = '关闭成功 clientNumber' + this.clientNumber;
  }
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

let clientNumber = - 1

function sppClientConnectSafe(): string {
  let message = "sppClientConnectSafe test"
  console.log('testSppClientConnectSafe create! ');
  let uuid: string = AppStorage.get('inputUuid') !
  let deviceId: string = AppStorage.get('inputDeviceId') !
  console.log('bluetoothModelManager sppConnect inputDeviceId: ' + deviceId);
  bluetoothManager.sppConnect(deviceId , {
    uuid : uuid ,
    secure : true ,
    type : 0
  } , (code , socketID) => {
    console.log("spp client code:" + JSON.stringify(code))
    if ( code.code != 0  ) {
      message = "客户端连接失败!";
      console.log('bluetoothModelManager sppConnect callback socketNumber(safe mode): ' + JSON.stringify(code));
      promptAction.showToast({ message : message })
      return message;
    }
    acceptClientSocket(socketID);
    console.log('bluetoothModelManager sppConnect callback socketNumber(safe mode): ' + socketID);
    message = "客户端连接成功!";
    promptAction.showToast({ message : message })
    return message
  })
  promptAction.showToast({ message : message })
  return message
}

function acceptClientSocket(number: number) {
  console.log('spp clientSocket socketID: ' + number);
  // 获取的clientNumber用作服务端后续读/写操作socket的id。
  clientNumber = number;
  AppStorage.setOrCreate("clientNumber" , clientNumber)
}

function sppClientWrite(): string {
  let message = "sppClientWrite test"
  let clientNumber: number = AppStorage.get("clientNumber") !
  console.log('bluetoothModelManager sppClientWrite sppWrite clientNumber: ' + JSON.stringify(clientNumber));
  let writeClientDataBW: string = AppStorage.get("writeClientDataBW") !
  let writeArrayBuffer: ArrayBuffer = strToArrayBuffer(writeClientDataBW) as ArrayBuffer;
  let writeStartTimeBW = new Date().getTime()
  const sppWriteSuccess = bluetooth.sppWrite(clientNumber , writeArrayBuffer);
  if ( sppWriteSuccess ) {
    console.log('sppClientWrite successfully');
    message = "sppClientWrite写入成功 ";
    AppStorage.setOrCreate("writeStartTimeBW" , writeStartTimeBW)
    sppClientReadOn()
  } else {
    console.log('sppClientWrite failed');
    message = "sppClientWrite写入失败";
  }
  promptAction.showToast({ message : message })
  return message
}


function sppClientReadOn(): string {
  let message = "sppClientReadOn test"
  let clientReadMessageBW = ""
  console.log("sppClientReadOn");
  let clientNumber: number = AppStorage.get("clientNumber") !
  bluetoothManager.on('sppRead' , clientNumber , (dataBuffer: ArrayBuffer) => {
    const data = new Uint8Array(dataBuffer);
    console.log('spp bluetoothManager read is: ' + data);
    if ( data != null ) {
      clientReadMessageBW += "读取成功！" + "\n"
      console.log("读取成功：" + JSON.stringify(data));
      AppStorage.setOrCreate("dataClientReadMessageBW" , clientReadMessageBW)
      promptAction.showToast({ message : clientReadMessageBW })

      let readEndTimeBW: number = new Date().getTime()
      let writeStartTimeBW: number = AppStorage.get("writeStartTimeBW") !
      let sppRunTime = ""
      sppRunTime += "sppRunTime,花费:" + (readEndTimeBW - writeStartTimeBW) + "ms;" + "\n"
      sppRunTime += "开始:" + (writeStartTimeBW) + "ms; " + "结束:" + (readEndTimeBW) + "ms" + "\n"
      AppStorage.setOrCreate("sppClientBandWidthBenchmarkTestMessage" , sppRunTime)
      console.log("花费:" + (readEndTimeBW - writeStartTimeBW) + "ms;" + "\n")
      console.log("开始:" + (writeStartTimeBW) + "ms; " + "结束:" + (readEndTimeBW) + "ms" + "\n")
      sppClientReadOff()
    } else {
      message = "读取失败";
    }
    promptAction.showToast({ message : message })
    return message
  });
  promptAction.showToast({ message : message })
  return message
}


function sppClientReadOff(): string {
  let message = "sppClientReadOff test"
  let clientNumber: number = AppStorage.get("clientNumber") !
  console.log("sppClientReadOff");
  bluetoothManager.off('sppRead' , clientNumber , (dataBuffer: ArrayBuffer) => {
    const data = new Uint8Array(dataBuffer);
    console.log('spp bluetoothManager read is: ' + data);
    if ( data != null ) {
      message = "读取成功" + JSON.stringify(data);
    } else {
      message = "读取失败";
    }
    AppStorage.setOrCreate("dataClientReadMessageBW" , message)
    promptAction.showToast({ message : message })
    return message
  });
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

function loopWriteDate() {
  let a = "1";
  for ( let i = 0 ;i < 100 ; i ++ ) {
    a += "1";
  }
  return a
}

