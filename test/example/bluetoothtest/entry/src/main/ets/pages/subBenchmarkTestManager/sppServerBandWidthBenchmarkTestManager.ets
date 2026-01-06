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
import * as SppServerManagerInterface from '../../MainAbility/model/sppServerManagerInterface'
import bluetoothManager from '@ohos.bluetoothManager';
import { testSetBluetoothScanModeManager , testSetLocalNameManager } from '../../MainAbility/model/brManagerInterface'
import promptAction from '@ohos.promptAction';

/**
 *  Bluetooth SppServer BandWidth Benchmark Test Page Of Bluetooth test
 */

@Entry
@Component
struct SppServerBandWidthBenchmarkTestManager {
  private testItem: TestData = (router.getParams() as myParams).testItem
  @State changeIndex: number = - 1
  @StorageLink("sppServerBandWidthBenchmarkTestMessage") sppServerBandWidthBenchmarkTestMessage: string = ""
  private uuid: string = "00001101-0000-1000-8000-00805f9b34fb";
  @StorageLink('dataServerReadMessageBW') dataServerReadMessageBW: string = "00";
  @State writeServerDataBW: string = '1';

  aboutToAppear() {
    AppStorage.setOrCreate('inputUuid' , this.uuid);
    AppStorage.setOrCreate("writeServerDataBW" , this.writeServerDataBW);
  }

  build() {
    Column() {
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
      }

      Stack().height('1vp').backgroundColor($r('app.color.black'));

      Column() {
        Scroll() {
          Column() {
            Text("性能测试:")
              .fontSize('17vp')
              .margin({ top : '5vp' , bottom : '5vp' , left : '20vp' })
              .textAlign(TextAlign.Start)
              .width('100%')
            Text("Spp的性能测试是测试Spp两端收发信息的速率和带宽性能")
              .fontSize('17vp')
              .margin({ left : '15vp' })
              .backgroundColor($r("app.color.spring"))
            Stack().height('1vp').backgroundColor($r('app.color.black'))

            Row() {
              Text("写入目标设备数据：").fontSize('17vp').width('160vp')
              TextInput({ text : this.writeServerDataBW , placeholder : "请输入写入的数据" })
                .fontSize('14vp')
                .backgroundColor($r("app.color.white"))
                .onChange((str) => {
                  this.writeServerDataBW = str;
                  AppStorage.setOrCreate("writeServerDataBW" , str);
                })
                .width('50%')
                .borderRadius('10vp')
                .height('32vp')

            }
            .padding('2vp')
            .justifyContent(FlexAlign.Start)
            .alignItems(VerticalAlign.Center)

            Scroll() {
              Column() {
                Text("读取的数据为：")
                  .fontSize('17vp')
                  .margin({ left : '10vp' })
                  .textAlign(TextAlign.Start)
                  .width('100%')
                Scroll() {
                  List() {
                    ListItem() {
                      Text(this.dataServerReadMessageBW)
                        .align(Alignment.TopStart)
                        .fontSize('14vp')
                        .width('90%')
                        .height('180vp')
                        .borderRadius('10vp')
                        .backgroundColor($r("app.color.white"))
                    }
                  }
                  .height('95%')
                  .width('90%')
                  .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
                }
                .scrollBarWidth('10vp')
                .scrollBar(BarState.On)
              }
              .width('100%')
              .height('180vp')
            }
            .scrollBarWidth('10vp')
            .scrollBar(BarState.On)
          }
          .width('100%')
          .height('360vp')
          .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        }
        .scrollBarWidth('10vp')
        .scrollBar(BarState.Auto)

        Stack().height('1vp').backgroundColor($r('app.color.black'))

        Scroll() {
          Column() {
            Text("日志:")
              .fontSize('17vp')
              .margin({ top : '5vp' , bottom : '5vp' , left : '10vp' })
              .textAlign(TextAlign.Start)
              .width('100%')
              .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
            List() {
              ListItem() {
                Text(this.sppServerBandWidthBenchmarkTestMessage)
                  .fontSize('17vp')
                  .margin({ top : '5vp' , left : '10vp' , right : '10vp' })
                  .textAlign(TextAlign.Start)
                  .width('90%')
              }
            }
            .height('90%')
            .width('90%')
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width('100%')
          .height('150vp')
        }
        .scrollBarWidth('10vp')
        .scrollBar(BarState.On)
      }
      .width('100%')
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('带宽性能测试').fontSize('24vp').fontColor(0xffffff).margin({ left : '5vp' , right : '5vp' })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius('8vp')
          .backgroundColor($r("app.color.blue"))
          .width('100vp')
          .height('66vp')
          .margin({ top : '15vp' , left : '5vp' , right : '5vp' })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppServerBandWidthBenchmarkTestMessage = ""

            let writeServerDataBW: string = loopWriteDate()
            AppStorage.setOrCreate("writeServerDataBW" , writeServerDataBW)
            await sleep(2)

            sppServerBandWidthBenchmarkTestMessage += "sppServerCreateSafe" + testFunc(sppServerCreateSafe)
            AppStorage.setOrCreate("sppServerBandWidthBenchmarkTestMessage" , sppServerBandWidthBenchmarkTestMessage)
            await sleep(8)

            sppServerBandWidthBenchmarkTestMessage += "sppServerAccept" + testFunc(sppServerAccept)
            AppStorage.setOrCreate("sppServerBandWidthBenchmarkTestMessage" , sppServerBandWidthBenchmarkTestMessage)
            await sleep(5)

          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Text('结束测试').fontSize('24vp').fontColor(0xffffff).margin({ left : '5vp' , right : '5vp' })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius('8vp')
          .backgroundColor($r("app.color.blue"))
          .width('80vp')
          .height('66vp')
          .margin({ top : '15vp' , left : '5vp' , right : '5vp' })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppServerBandWidthBenchmarkTestMessage = ""

            sppServerBandWidthBenchmarkTestMessage += "testSppServerReadOff 执行：" + testFunc(SppServerManagerInterface.testSppServerReadOffManager) + "\n"
            AppStorage.setOrCreate("sppServerBandWidthBenchmarkTestMessage" , sppServerBandWidthBenchmarkTestMessage)
            await sleep(1)

            sppServerBandWidthBenchmarkTestMessage += "testSppCloseServerSocket 执行：" + testFunc(SppServerManagerInterface.testSppCloseServerSocketManager) + "\n"
            AppStorage.setOrCreate("sppServerBandWidthBenchmarkTestMessage" , sppServerBandWidthBenchmarkTestMessage)
            await sleep(1)

            sppServerBandWidthBenchmarkTestMessage += "sppCloseClientSocket 执行：" + testFunc(SppServerManagerInterface.testSppCloseClientSocketManager) + "\n"
            AppStorage.setOrCreate("sppServerBandWidthBenchmarkTestMessage" , sppServerBandWidthBenchmarkTestMessage)
            await sleep(1)
          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Image($r('app.media.ic_public_delete_filled')).width('30vp').height('40vp').margin({ left : '5vp' })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius('8vp')
          .backgroundColor($r("app.color.moon"))
          .width('50vp')
          .height('66vp')
          .margin({ top : '15vp' , right : '40vp' })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppServerBandWidthBenchmarkTestMessage = ""
            AppStorage.setOrCreate("sppServerBandWidthBenchmarkTestMessage" , sppServerBandWidthBenchmarkTestMessage)
          })
        }
        .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        .width('100%')
      }
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
      .width('100%')
      .height('100%');
    }
  }

  dataRead(dataBuffer: ArrayBuffer) {
    const data = new Uint8Array(dataBuffer);
    console.log('spp bluetooth read is: ' + data);
    if ( data != null ) {
      this.dataServerReadMessageBW = "读取成功" + JSON.stringify(data);
    } else {
      this.dataServerReadMessageBW = "读取失败";
    }
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

let serverNumber: number = - 1;
let clientNumber: number = - 1;

function sppServerCreateSafe(): string {
  let message = "sppServerCreateSafe test"
  console.log('spp server socketId(safe mode) create! ');
  let uuid: string = AppStorage.get('inputUuid') !
  if ( uuid == '' ) {
    message = 'uuid为空';
    promptAction.showToast({ message : message })
    return message
  }
  // 创建蓝牙服务器侦听套接字, 返回服务端ID
  bluetoothManager.sppListen('BluetoothDevice' , {
    uuid : uuid ,
    secure : true ,
    type : 0
  } , (code , serverSocketID) => {
    console.log('bluetooth error code: ' + JSON.stringify(code));
    if ( !code || code.code == 0 ) {
      console.log('spp server: serverSocketID(safe mode) - ' + serverSocketID);
      serverNumber = serverSocketID;
      message = "创建server成功";
      promptAction.showToast({ message : message })
      AppStorage.setOrCreate("serverNumber" , serverNumber)
      console.log('testSppServerAccept test')
      console.log('testSppServerAccept serverNumber ' + serverNumber)
    }
  });
  promptAction.showToast({ message : message })
  return message
}


function sppServerAccept(): string {
  let message = "testSppServerAccept test"
  let serverReadMessageBW = ""
  //  serverNumber = AppStorage.get("serverNumber")
  console.log('testSppServerAccept test')
  console.log('testSppServerAccept serverNumber ' + serverNumber)

  bluetoothManager.sppAccept(serverNumber , (code , clientSocketID) => {
    if ( !code || code.code == 0 ) {
      console.log('spp server: clientSocketID - ' + clientSocketID);
      console.log('test serverNumber ' + serverNumber);
      clientNumber = clientSocketID;
      AppStorage.setOrCreate("clientNumber" , clientNumber)
      message = "接受客户端成功"
      promptAction.showToast({ message : message })
      bluetoothManager.on('sppRead' , clientNumber , (dataBuffer: ArrayBuffer) => {
        const data = new Uint8Array(dataBuffer);
        console.log('spp bluetooth read is: ' + data);
        if ( data != null ) {
          serverReadMessageBW += "读取成功！" + "\n"
          console.log("读取成功：" + JSON.stringify(data));
          AppStorage.setOrCreate("dataServerReadMessageBW" , serverReadMessageBW)
          promptAction.showToast({ message : serverReadMessageBW })
          //          sppServerWrite();
          console.log('sppServerWrite sppWrite clientNumber: ' + clientNumber);
          //            let writeServerDataBW = AppStorage.get("writeServerDataBW") 值为1
          let writeServerDataBW: string = AppStorage.get("writeServerDataBW") !
          let writeArrayBuffer: ArrayBuffer = strToArrayBuffer(writeServerDataBW) as ArrayBuffer;
          console.log('sppServerWrite  writeServerDataBW: ' + writeServerDataBW);
          bluetoothManager.sppWrite(clientNumber , writeArrayBuffer);
          console.log("写入成功");
          message = "写入成功 ";
          promptAction.showToast({ message : message })
        } else {
          message = "读取失败";
        }
        promptAction.showToast({ message : message })
      })
      promptAction.showToast({ message : message })
    } else {
      message = "接受客户端失败";
      console.log("code.code:" + JSON.stringify(code))
      promptAction.showToast({ message : message })
    }
  })
  promptAction.showToast({ message : message })
  return message
}

function sppServerWrite(): string {
  let message = "sppServerWrite test"
  let clientNumber: number = AppStorage.get("clientNumber") !
  console.log('sppServerWrite sppWrite clientNumber: ' + clientNumber);
  let writeServerDataBW: string = AppStorage.get("writeServerDataBW") !
  let writeArrayBuffer: ArrayBuffer = strToArrayBuffer(writeServerDataBW) as ArrayBuffer;
  bluetoothManager.sppWrite(clientNumber , writeArrayBuffer);
  console.log('sppWrite successfully');
  message = "写入成功 ";
  promptAction.showToast({ message : message })
  return message
}

function sppServerReadOff(): string {
  let message = "sppServerReadOff test"
  console.log('sppServerReadOff');
  let clientNumber: number = AppStorage.get("clientNumber") !
  bluetoothManager.off('sppRead' , clientNumber ,
    (dataBuffer: ArrayBuffer) => {
      const data = new Uint8Array(dataBuffer);
      console.log('spp bluetooth read is: ' + data);
      if ( data != null ) {
        message = "读取成功" + JSON.stringify(data);
      } else {
        message = "读取失败";
      }
      AppStorage.setOrCreate("dataServerReadMessageBW" , message)
      promptAction.showToast({ message : message })
      return message
    }
  );
  promptAction.showToast({ message : message })
  return message
}

function strToArrayBuffer(str: string) {
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
