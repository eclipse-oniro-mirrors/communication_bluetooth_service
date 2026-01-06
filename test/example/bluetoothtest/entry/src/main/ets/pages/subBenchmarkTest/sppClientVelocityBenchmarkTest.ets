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
import * as SppClientInterface from '../../MainAbility/model/sppClientInterface'
import bluetooth from '@ohos.bluetooth';
import { testSetBluetoothScanMode , testSetLocalName } from '../../MainAbility/model/brInterface'
import promptAction from '@ohos.promptAction';
import LogUtil from '../../Utils/LogUtil'

/**
 *  Bluetooth SppClient Velocity Benchmark Test Page Of Bluetooth test
 */

@Entry
@Component
struct SppClientVelocityBenchmarkTest {
  @State message: string = 'SppClientBenchmarkTest';
  private testItem: TestData = (router.getParams() as myParams).testItem
  @State changeIndex: number = - 1
  @StorageLink('dataClientReadMessage') dataClientReadMessage: string = "00";
  @StorageLink("sppClientVelocityBenchmarkTestMessage") sppClientVelocityBenchmarkTestMessage: string = ""
  @State writeClientData: string = '1';
  private clientNumber: number = - 1;
  private uuid: string = "00001101-0000-1000-8000-00805f9b34fb";
  @State deviceId: string = "08:FB:EA:1B:3C:63"; //08:FB:EA:1B:3C:63

  aboutToAppear() {

    AppStorage.setOrCreate('clientV_Cycle_index' , 0);
    AppStorage.setOrCreate('inputDeviceId' , this.deviceId);
    AppStorage.setOrCreate('inputUuid' , this.uuid);
    AppStorage.setOrCreate("writeClientData" , this.writeClientData);
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
              Text("目标设备MAC：").fontSize('17vp').width('1300vp')
              TextInput({ text : this.deviceId , placeholder : "请输入mac地址" })
                .fontSize('17vp')
                .onChange((str) => {
                  this.deviceId = str;
                  AppStorage.setOrCreate('inputDeviceId' , str)
                })
                .width('70%')
                .borderRadius('1vp')
                .backgroundColor($r("app.color.white"))
                .height('35vp')
            }
            .padding('5vp')
            .width('96%')
            .justifyContent(FlexAlign.Start)
            .alignItems(VerticalAlign.Center)

            Row() {
              Text("clientV_Cycle_index:").fontSize('17vp').width('200vp')
              Column() {
                Select([
                  { value : "100" },
                  { value : "1000" },
                  { value : "10000" }
                ])
                  .borderRadius('10vp')
                  .backgroundColor($r("app.color.white"))
                  .selected(0)
                  .value("循环次数")
                  .font({ size : '15vp' })
                  .margin({ right : '160vp' })
                  .selectedOptionFont({ size : 17 })
                  .optionFont({ size : '15vp' })
                  .width('80%')
                  .height('40vp')
                  .onSelect((index: number) => {
                    console.log("index Select:" + index)
                    AppStorage.setOrCreate('clientV_Cycle_index' , index);
                  })
              }
              .width('70%')
            }
            .padding('2vp')
            .width('96%')
            .justifyContent(FlexAlign.Start)
            .alignItems(VerticalAlign.Center)

            Row() {
              Text("写入目标设备数据：").fontSize('17vp').width('170vp')
              TextInput({ text : this.writeClientData , placeholder : "请输入写入的数据" })
                .align(Alignment.Center)
                .fontSize('15vp')
                .backgroundColor($r("app.color.white"))
                .onChange((str) => {
                  this.writeClientData = str;
                  AppStorage.setOrCreate("writeClientData" , str);
                })
                .width('60%')
                .borderRadius('10vp')
                .height('32vp')
            }
            .padding('2vp')
            .width('96%')
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
                      Text(this.dataClientReadMessage)
                        .align(Alignment.TopStart)
                        .fontSize('14vp')
                        .width('90%')
                        .height('150vp')
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
              .height('150vp')
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
              .margin({ top : '8vp' , bottom : '8vp' , left : '10vp' })
              .textAlign(TextAlign.Start)
              .width('100%')
              .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
            List() {
              ListItem() {
                Text(this.sppClientVelocityBenchmarkTestMessage)
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
              Text('速率性能测试').fontSize('24vp').fontColor(0xffffff).margin({ left : '5vp' , right : '5vp' })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius('8vp')
          .backgroundColor($r("app.color.blue"))
          .width('100vp')
          .height('66vp')
          .margin({ top : '15vp' , left : '5vp' , right : '5vp' })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppClientVelocityBenchmarkTestMessage = ""
            let writeClientData = loopWriteDate()
            AppStorage.setOrCreate("writeClientData" , writeClientData)
            await sleep(2)
            sppClientVelocityBenchmarkTestMessage += "sppClientConnectSafe" + testFunc(sppClientConnectSafe)
            AppStorage.setOrCreate("sppClientVelocityBenchmarkTestMessage" , sppClientVelocityBenchmarkTestMessage)
            await sleep(10)

            sppClientVelocityBenchmarkTestMessage += "sppClientWrite" + testFunc(sppClientWrite)
            AppStorage.setOrCreate("sppClientVelocityBenchmarkTestMessage" , sppClientVelocityBenchmarkTestMessage)
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
            let sppClientVelocityBenchmarkTestMessage = ""

            sppClientVelocityBenchmarkTestMessage += "testSppClientReadOff 执行：" + testFunc(SppClientInterface.testSppClientReadOff) + "\n"
            AppStorage.setOrCreate("sppClientVelocityBenchmarkTestMessage" , sppClientVelocityBenchmarkTestMessage)
            await sleep(1)

            sppClientVelocityBenchmarkTestMessage += "testSppClientClose 执行：" + testFunc(SppClientInterface.testSppClientClose) + "\n"
            AppStorage.setOrCreate("sppClientVelocityBenchmarkTestMessage" , sppClientVelocityBenchmarkTestMessage)
            await sleep(1)

          })

          Button({ type : ButtonType.Normal , stateEffect : true }) {
            Row() {
              Image($r('app.media.ic_public_delete_filled')).width('30vp').height('40vp').margin({ left : '5vp' })
              //              Text('清空').fontSize('24vp').fontColor(0xffffff).margin({ left: '5vp', right: '10vp' })
            }.alignItems(VerticalAlign.Center)
          }
          .borderRadius('8vp')
          .backgroundColor($r("app.color.moon"))
          .width('50vp')
          .height('66vp')
          .margin({ top : '15vp' , right : '40vp' })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppClientVelocityBenchmarkTestMessage = ""
            AppStorage.setOrCreate("sppClientVelocityBenchmarkTestMessage" , sppClientVelocityBenchmarkTestMessage)
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
      this.dataClientReadMessage = "读取成功" + JSON.stringify(data);
    } else {
      this.dataClientReadMessage = "读取失败";
    }
  }

  sppClientClose() {
    LogUtil.info('sppCloseClientSocket clientNumber: ' + JSON.stringify(this.clientNumber))
    bluetooth.sppCloseClientSocket(this.clientNumber);
    this.message = '关闭成功 clientNumber' + this.clientNumber;
  }
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
  //  console.log("运行了" + JSON.stringify(func))
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


function choose(index: number) {
  let indexNumber: number = 0
  switch ( index ) {
    case 0: {
      indexNumber = 100;
    }
      break;
    case 1: {
      indexNumber = 1000;
    }
      break;
    case 2: {
      indexNumber = 10000;
    }
      break;
    default: {
      indexNumber = 100;
    }
      break;
  }
  return indexNumber
}

let clientNumber = - 1

function sppClientConnectSafe(): void {
  let message = "sppClientConnectSafe test"
  console.log('testSppClientConnectSafe create! ');
  let uuid: string = AppStorage.get('inputUuid') !
  let deviceId: string = AppStorage.get('inputDeviceId') !
  console.log('bluetoothModel sppConnect inputDeviceId: ' + deviceId);
  bluetooth.sppConnect(deviceId , {
    uuid : uuid ,
    secure : true ,
    type : 0
  } , (code , socketID) => {
    console.log("spp client code:" + JSON.stringify(code))
    if ( !code || code.code == 0 ) {
      acceptClientSocket(socketID);
      console.log('bluetoothModel sppConnect success socketNumber(safe mode): ' + socketID);
      message = "客户端连接成功!";
      promptAction.showToast({ message : message })
    } else {
      message = "客户端连接失败!";
      console.log('bluetoothModel sppConnect fail code: ' + JSON.stringify(code));
      promptAction.showToast({ message : message })
    }
  })
  return
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
  console.log('bluetoothModel sppClientWrite sppWrite clientNumber: ' + JSON.stringify(clientNumber));
  let writeClientData: string = AppStorage.get("writeClientData") !
  let writeArrayBuffer: ArrayBuffer = strToArrayBuffer(writeClientData) as ArrayBuffer;
  let writeStartTime: number = new Date().getTime()
  const sppWriteSuccess = bluetooth.sppWrite(clientNumber , writeArrayBuffer);
  if ( sppWriteSuccess ) {
    console.log('第1次sppClientWrite successfully');
    message = "第1次sppClientWrite写入成功 ";
    AppStorage.setOrCreate("writeStartTime" , writeStartTime)
    sppClientReadOn()
  } else {
    console.log('第1次sppClientWrite failed');
    message = "第1次sppClientWrite写入失败";
  }
  promptAction.showToast({ message : message })
  return message
}


function sppClientReadOn(): string {
  let message = "sppClientReadOn test"
  let clientReadMessage = ""
  let i = 1
  let index: number = AppStorage.get('clientV_Cycle_index') !;
  let indexNumber = choose(index)
  let cycleNumber = indexNumber + 1
  console.log("循环数为:" + indexNumber)
  console.log("sppClientReadOn");
  let clientNumber: number = AppStorage.get("clientNumber") !
  bluetooth.on('sppRead' , clientNumber , (dataBuffer: ArrayBuffer) => {
    const data = new Uint8Array(dataBuffer);
    console.log('spp bluetooth read is: ' + data);
    if ( data != null ) {
      //倒着显示
      clientReadMessage = "第" + i + "次,读取成功！" + "\n" + clientReadMessage
      console.log("第" + i + "次,读取成功：" + JSON.stringify(data));
      AppStorage.setOrCreate("dataClientReadMessage" , clientReadMessage)
      promptAction.showToast({ message : clientReadMessage })
      //        sppClientWrite();
      console.log('bluetoothModel sppClientWrite sppWrite clientNumber: ' + JSON.stringify(clientNumber));
      i ++
      if ( i < cycleNumber ) {
        //每次循环 data加一位"1"
        //        let writeClientData = AppStorage.get("writeClientData") + "1"
        //        AppStorage.setOrCreate("writeClientData", writeClientData)
        //每次都循环"11.....11"
        let writeClientData: string = AppStorage.get("writeClientData") !
        let writeArrayBuffer: ArrayBuffer = strToArrayBuffer(writeClientData) as ArrayBuffer;
        console.log('sppServerWrite  writeClientData: ' + writeClientData);
        const sppWriteSuccess = bluetooth.sppWrite(clientNumber , writeArrayBuffer);
        if ( sppWriteSuccess ) {
          console.log("sppClientWrite第" + i + "次,写入成功");
          message = "sppClient第" + i + "次写入成功 ";
        } else {
          console.log("sppClientWrite第" + i + "次,写入失败");
          message = "sppClient第" + i + "次写入失败";
          return message
        }
        promptAction.showToast({ message : message })
      } else {
        promptAction.showToast({ message : "循环次数到咯" })
        console.log("循环次数到咯")
        let readEndTime = new Date().getTime()
        let writeStartTime: number = AppStorage.get("writeStartTime") !
        let sppRunTime = ""
        sppRunTime += "sppRunTime,花费:" + Number(readEndTime - writeStartTime) + "ms;" + "\n"
        sppRunTime += "开始:" + Number(writeStartTime) + "ms; " + "结束:" + Number(readEndTime) + "ms" + "\n"
        AppStorage.setOrCreate("sppClientVelocityBenchmarkTestMessage" , sppRunTime)
        console.log("花费:" + Number(readEndTime - writeStartTime) + "ms;" + "\n")
        console.log("开始:" + Number(writeStartTime) + "ms; " + "结束:" + Number(readEndTime) + "ms" + "\n")
        sppClientReadOff()
      }
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
  bluetooth.off('sppRead' , clientNumber , (dataBuffer: ArrayBuffer) => {
    const data = new Uint8Array(dataBuffer);
    console.log('spp bluetooth read is: ' + data);
    if ( data != null ) {
      message = "读取成功" + JSON.stringify(data);
    } else {
      message = "读取失败";
    }
    AppStorage.setOrCreate("dataClientReadMessage" , message)
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

