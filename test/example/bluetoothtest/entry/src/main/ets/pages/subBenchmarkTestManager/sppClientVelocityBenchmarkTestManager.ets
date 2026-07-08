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
import { resStr} from '../../Utils/resString';


import ConfigData from '../../Utils/ConfigData';
import { PageTitle} from '../../component/pageTitle';
import { TestImageDisplay} from '../../component/testImageDisplay';
import { TestData} from '../../MainAbility/model/testData'
import * as SppClientManagerInterface from '../../MainAbility/model/sppClientManagerInterface'
import bluetoothManager from '@ohos.bluetoothManager';
import { testSetBluetoothScanModeManager, testSetLocalNameManager} from '../../MainAbility/model/brManagerInterface'
import LogUtil from '../../Utils/LogUtil';
import promptAction from '@ohos.promptAction';
import { testItemFromRouterOrPlaceholder} from '../../Utils/common'

/**
 *  Bluetooth SppClient Velocity Benchmark Manager Test Page Of Bluetooth test
 */

@Entry
@Component
struct SppClientVelocityBenchmarkTestManager {
  @State message: string = 'SppClientBenchmarkTestManager';
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @State changeIndex: number = - 1
  @StorageLink('dataClientReadMessage') dataClientReadMessage: string = "00";
  @StorageLink("sppClientVelocityBenchmarkTestMessage") sppClientVelocityBenchmarkTestMessage: string = ""
  @State writeClientData: string = '1';
  private clientNumber: number = - 1;
  private uuid: string = "00001101-0000-1000-8000-00805f9b34fb";
  @State deviceId: string = "08:FB:EA:1B:3C:63";//08:FB:EA:1B:3C:63

  aboutToAppear(){
    AppStorage.setOrCreate('clientV_Cycle_index', 0);
    AppStorage.setOrCreate('inputDeviceId', this.deviceId);
    AppStorage.setOrCreate('inputUuid', this.uuid);
    AppStorage.setOrCreate("writeClientData", this.writeClientData);
  }

  build(){
    Column(){
      Stack({alignContent: Alignment.TopStart}){
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
              .accessibilityLevel('yes')
              .id('pages_subBenchmarkTestManager_sppClientVelocityBenchmarkTestManager__pt_0')
      }

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));

      Column(){
        Scroll(){
          Column(){
            Text($r('app.string.bluetoothtest_text_xncs'))
              .fontSize(ConfigData.VP_17)
              .margin({top: ConfigData.VP_5, bottom: ConfigData.VP_5, left: ConfigData.VP_20})
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            Text($r('app.string.bluetoothtest_text_sdxncsscssldsfxxdslhdkxn'))
              .fontSize(ConfigData.VP_17)
              .margin({left: ConfigData.VP_15})
              .backgroundColor($r("app.color.spring"))

              Stack()
                .height(ConfigData.HEIGHT_1_VP)
                .backgroundColor($r('app.color.black'))

              Row(){
              Text($r('app.string.bluetoothtest_text_mbsbm'))
                .fontSize(ConfigData.VP_17)
                .width(ConfigData.VP_1300)
              TextInput({text: this.deviceId, placeholder: $r('app.string.bluetoothtest_str_qsrmdz')})
                .accessibilityLevel('yes')
                .id('pages_subBenchmarkTestManager_sppClientVelocityBenchmarkTestManager__ti_0')
                .fontSize(ConfigData.VP_17)
                .onChange((str) => {
                  this.deviceId = str;
                  AppStorage.setOrCreate('inputDeviceId', str)
                })
                .width(ConfigData.WH_80_100)
                .borderRadius(ConfigData.HEIGHT_1_VP)
                .backgroundColor($r("app.color.white"))
                .height(ConfigData.VP_30)
              }
            .padding(ConfigData.VP_5)
            .justifyContent(FlexAlign.Start)
            .alignItems(VerticalAlign.Center)

            Row(){
              Text($r('app.string.bluetoothtest_text_cci'))
                .fontSize(ConfigData.VP_17)
                .width(ConfigData.VP_180)
                .padding({left: ConfigData.VP_5})
              Column(){
                Select([
                  { value: "100"},
                  { value: "1000"},
                  { value: "10000"}
                ])
                  .borderRadius(ConfigData.VP_10)
                  .backgroundColor($r("app.color.white"))
                  .selected(0)
                  .value($r('app.string.bluetoothtest_str_xhcs'))
                  .font({size: ConfigData.VP_15})
                  .margin({right: ConfigData.VP_160})
                  .selectedOptionFont({size: 17})
                  .optionFont({size: ConfigData.VP_15})
                  .width(ConfigData.WH_90_100)
                  .height(ConfigData.VP_40)
                  .onSelect((index: number) => {
                    LogUtil.info("index Select:" + index)
                    AppStorage.setOrCreate('clientV_Cycle_index', index)
                  })
              }
              .width(ConfigData.WH_90_100)
            }
            .padding(ConfigData.VP_2)
            .justifyContent(FlexAlign.Start)
            .alignItems(VerticalAlign.Center)

            Row(){
              Text($r('app.string.bluetoothtest_text_xrmbsbsj'))
                .fontSize(ConfigData.VP_17)
                .width(ConfigData.VP_160)
                .padding({left: ConfigData.VP_5});
              TextInput({text: this.writeClientData, placeholder: $r('app.string.bluetoothtest_str_qsrxrdsj')})
                .accessibilityLevel('yes')
                .id('pages_subBenchmarkTestManager_sppClientVelocityBenchmarkTestManager__ti_1')
                .align(Alignment.Center)
                .fontSize(ConfigData.VP_15)
                .backgroundColor($r("app.color.white"))
                .onChange((str) => {
                  this.writeClientData = str;
                  AppStorage.setOrCreate("writeClientData", str);
                })
                .width(ConfigData.WH_50_100)
                .borderRadius(ConfigData.VP_10)
                .height(ConfigData.VP_32)
            }
            .padding(ConfigData.VP_2)
            .justifyContent(FlexAlign.Start)
            .alignItems(VerticalAlign.Center)

            Scroll(){
              Column(){
                Text($r('app.string.bluetoothtest_text_dqdsjw'))
                  .fontSize(ConfigData.VP_17)
                  .margin({left: ConfigData.VP_10})
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_100_100)
                Scroll(){
                  List(){
                    ListItem(){
                      Text(this.dataClientReadMessage)
                        .align(Alignment.TopStart)
                        .fontSize(ConfigData.VP_14)
                        .width(ConfigData.WH_90_100)
                        .height(ConfigData.VP_150)
                        .borderRadius(ConfigData.VP_10)
                        .backgroundColor($r("app.color.white"))
                    }
                  .accessibilityLevel('yes')
                  .id('pages_subBenchmarkTestManager_sppClientVelocityBenchmarkTestManager__li_0')
                  }
                  .height(ConfigData.PCT_95)
                  .width(ConfigData.WH_90_100)
                  .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
                }
                .accessibilityLevel('yes')
                .id('pages_subBenchmarkTestManager_sppClientVelocityBenchmarkTestManager__sc_2')
                .scrollBarWidth(ConfigData.VP_10)
                .scrollBar(BarState.On)
              }
              .width(ConfigData.WH_100_100)
              .height(ConfigData.VP_150)
            }
            .accessibilityLevel('yes')
            .id('pages_subBenchmarkTestManager_sppClientVelocityBenchmarkTestManager__sc_1')
            .scrollBarWidth(ConfigData.VP_10)
            .scrollBar(BarState.On)
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.VP_360)
          .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        }
        .accessibilityLevel('yes')
        .id('pages_subBenchmarkTestManager_sppClientVelocityBenchmarkTestManager__sc_0')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)

        Stack()
          .height(ConfigData.HEIGHT_1_VP)
          .backgroundColor($r('app.color.black'))

        Scroll(){
          Column(){
            Text($r('app.string.bluetoothtest_text_rz'))
              .fontSize(ConfigData.VP_17)
              .margin({top: ConfigData.VP_8, bottom: ConfigData.VP_8, left: ConfigData.VP_10})
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
              .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
            List(){
              ListItem(){
                Text(this.sppClientVelocityBenchmarkTestMessage)
                  .fontSize(ConfigData.VP_17)
                  .margin({top: ConfigData.VP_5, left: ConfigData.VP_10, right: ConfigData.VP_10})
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
            .accessibilityLevel('yes')
            .id('pages_subBenchmarkTestManager_sppClientVelocityBenchmarkTestManager__li_1')
            }
            .height(ConfigData.WH_90_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.VP_150)
        }
        .accessibilityLevel('yes')
        .id('pages_subBenchmarkTestManager_sppClientVelocityBenchmarkTestManager__sc_3')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.On)
      }
      .width(ConfigData.WH_100_100)
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Column(){
        Flex({alignItems: ItemAlign.Center, justifyContent: FlexAlign.SpaceBetween}){
          Button({type: ButtonType.Normal, stateEffect: true}){
            Row(){
              Text($r('app.string.bluetoothtest_text_slxncs'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({left: ConfigData.VP_5, right: ConfigData.VP_5})
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTestManager_sppClientVelocityBenchmarkTestManager__btn_0')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({top: ConfigData.VP_15, left: ConfigData.VP_5, right: ConfigData.VP_5})
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppClientVelocityBenchmarkTestMessage = ""
            let writeClientData = loopWriteDate()
            AppStorage.setOrCreate("writeClientData", writeClientData)
            await sleep(2)
            sppClientVelocityBenchmarkTestMessage += "sppClientConnectSafe" + testFunc(sppClientConnectSafe)
            AppStorage.setOrCreate("sppClientVelocityBenchmarkTestMessage", sppClientVelocityBenchmarkTestMessage)
            await sleep(5)

            sppClientVelocityBenchmarkTestMessage += "sppClientWrite" + testFunc(sppClientWrite)
            AppStorage.setOrCreate("sppClientVelocityBenchmarkTestMessage", sppClientVelocityBenchmarkTestMessage)
            await sleep(5)

          })

          Button({type: ButtonType.Normal, stateEffect: true}){
            Row(){
              Text($r('app.string.bluetoothtest_text_jscs'))
                .fontSize(ConfigData.VP_24)
                .fontColor(0xffffff)
                .margin({left: ConfigData.VP_5, right: ConfigData.VP_5})
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTestManager_sppClientVelocityBenchmarkTestManager__btn_1')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_80)
          .height(ConfigData.VP_66)
          .margin({top: ConfigData.VP_15, left: ConfigData.VP_5, right: ConfigData.VP_5})
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppClientVelocityBenchmarkTestMessage = ""

            sppClientVelocityBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_13')) + testFunc(SppClientManagerInterface.testSppClientReadOffManager) + "\n"
            AppStorage.setOrCreate("sppClientVelocityBenchmarkTestMessage", sppClientVelocityBenchmarkTestMessage)
            await sleep(1)

            sppClientVelocityBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_19')) + testFunc(SppClientManagerInterface.testSppClientCloseManager) + "\n"
            AppStorage.setOrCreate("sppClientVelocityBenchmarkTestMessage", sppClientVelocityBenchmarkTestMessage)
            await sleep(1)

          })


          Button({type: ButtonType.Normal, stateEffect: true}){
            Row(){
              Image($r('app.media.ic_public_delete_filled'))
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTestManager_sppClientVelocityBenchmarkTestManager__img_0').width(ConfigData.VP_30).height(ConfigData.VP_40).margin({left: ConfigData.VP_5})
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTestManager_sppClientVelocityBenchmarkTestManager__btn_2')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.moon"))
          .width(ConfigData.VP_50)
          .height(ConfigData.VP_66)
          .margin({top: ConfigData.VP_15, right: ConfigData.VP_40})
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let sppClientVelocityBenchmarkTestMessage = ""
            AppStorage.setOrCreate("sppClientVelocityBenchmarkTestMessage", sppClientVelocityBenchmarkTestMessage)
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

  dataRead(dataBuffer: ArrayBuffer){
    const data = new Uint8Array(dataBuffer);
    LogUtil.info('spp bluetoothManager read is: ' + data);
    if ( data != null ){
      this.dataClientReadMessage = resStr($r('app.string.bluetoothtest_str_dqcg_2')) + JSON.stringify(data);
    } else {
      this.dataClientReadMessage = resStr($r('app.string.bluetoothtest_str_dqsb'));
    }
  }

  sppClientClose(){
    LogUtil.info('sppCloseClientSocket clientNumber: ' + JSON.stringify(this.clientNumber))
    bluetoothManager.sppCloseClientSocket(this.clientNumber);
    this.message = resStr($r('app.string.bluetoothtest_str_gbcgc')) + this.clientNumber;
  }
}

export function testFunc(func: Function){
  let start = new Date().getTime()
  switch ( func ){
    case testSetLocalNameManager: {
      func("DaYuBlue")
    }
      break;
    case testSetBluetoothScanModeManager: {
      func(4, 0)
    }
      break;
    default: {
      func()
    }
      break;
  }

  let end = new Date().getTime()
  LogUtil.info(resStr($r('app.string.bluetoothtest_str_ks')) + Number(start) + "ms")
  LogUtil.info(resStr($r('app.string.bluetoothtest_str_js')) + Number(end) + "ms")
  LogUtil.info(resStr($r('app.string.bluetoothtest_str_hf')) + Number(end - start) + "ms")
  let message = ""
  message += resStr($r('app.string.bluetoothtest_str_hf')) + Number(end - start) + "ms" + "\n"
  message += resStr($r('app.string.bluetoothtest_str_ks')) + Number(start) + resStr($r('app.string.bluetoothtest_str_m_1')) + resStr($r('app.string.bluetoothtest_str_js')) + Number(end) + "ms" + "\n"
  return message
}

async function sleep(time: number): Promise<void> {
  return new Promise<void>((resolve, reject) => {
    setTimeout(() => {
      resolve()
    }, time * 1000);
  });
}

function choose(index: Number){
  let indexNumber: number = 0
  switch ( index ){
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

function sppClientConnectSafe(): string {
  let message = resStr($r('app.string.bluetoothtest_str_st_8'))
  LogUtil.info('testSppClientConnectSafe create! ');
  let uuid: string = AppStorage.get('inputUuid') !
  let deviceId: string = AppStorage.get('inputDeviceId') !
  LogUtil.info('bluetoothModelManager sppConnect inputDeviceId: ' + deviceId);
  bluetoothManager.sppConnect(deviceId, {
    uuid: uuid,
    secure: true,
    type: 0
  }, (code, socketID) => {
    LogUtil.info("spp client code:" + JSON.stringify(code))
    if ( code.code != 0 ){
      message = resStr($r('app.string.bluetoothtest_str_khdljsb'))
      LogUtil.info('bluetoothModelManager sppConnect callback socketNumber(safe mode): ' + JSON.stringify(code));
      promptAction.showToast({message: message})
      return message;
    }
    acceptClientSocket(socketID);
    LogUtil.info('bluetoothModelManager sppConnect callback socketNumber(safe mode): ' + socketID);
    message = resStr($r('app.string.bluetoothtest_str_khdljcg'));
    promptAction.showToast({message: message})
    return message
  })
  promptAction.showToast({message: message})
  return message
}

function acceptClientSocket(number: number){
  LogUtil.info('spp clientSocket socketID: ' + number);
  // 获取的clientNumber用作服务端后续读/写操作socket的id。
  clientNumber = number;
  AppStorage.setOrCreate("clientNumber", clientNumber)
}

function sppClientWrite(): string {
  let message = resStr($r('app.string.bluetoothtest_str_st_18'))
  let clientNumber: number = AppStorage.get("clientNumber") !
  LogUtil.info('bluetoothModelManager sppClientWrite sppWrite clientNumber: ' + JSON.stringify(clientNumber));
  let writeClientData: string = AppStorage.get("writeClientData") !
  let writeArrayBuffer: ArrayBuffer = strToArrayBuffer(writeClientData) as ArrayBuffer;
  let writeStartTime = new Date().getTime()
  bluetoothManager.sppWrite(clientNumber, writeArrayBuffer);
  LogUtil.info($r('app.string.bluetoothtest_str_d1css'));
  message = resStr($r('app.string.bluetoothtest_str_d1csxrcg'));
  AppStorage.setOrCreate("writeStartTime", writeStartTime)
  sppClientReadOn()
  promptAction.showToast({message: message})
  return message
}


function sppClientReadOn(): string {
  let message = resStr($r('app.string.bluetoothtest_str_st_14'))
  let clientReadMessage = ""
  let i = 1
  let index: number = AppStorage.get('clientV_Cycle_index') !;
  let indexNumber = choose(index)
  let cycleNumber = indexNumber + 1
  LogUtil.info(resStr($r('app.string.bluetoothtest_str_xhsw')) + indexNumber)
  LogUtil.info("sppClientReadOn")
  let clientNumber: number = AppStorage.get("clientNumber") !
  bluetoothManager.on('sppRead', clientNumber, (dataBuffer: ArrayBuffer) => {
    const data = new Uint8Array(dataBuffer);
    LogUtil.info('spp bluetoothManager read is: ' + data);
    if ( data != null ){
      //倒着显示
      clientReadMessage = resStr($r('app.string.bluetoothtest_str_d_1')) + i + resStr($r('app.string.bluetoothtest_str_cdqcg')) + "\n" + clientReadMessage
      LogUtil.info(resStr($r('app.string.bluetoothtest_str_d_1')) + i + resStr($r('app.string.bluetoothtest_str_cdqcg_1')) + JSON.stringify(data));
      AppStorage.setOrCreate("dataClientReadMessage", clientReadMessage)
      promptAction.showToast({message: clientReadMessage})
      //        sppClientWrite();
      LogUtil.info('bluetoothModelManager sppClientWrite sppWrite clientNumber: ' + JSON.stringify(clientNumber));
      i++
      if ( i < cycleNumber ){
        //每次循环 data加一位"1"
        //        let writeClientData = AppStorage.get("writeClientData") + "1"
        //        AppStorage.setOrCreate("writeClientData", writeClientData)
        //每次都循环"11.....11"
        let writeClientData: string = AppStorage.get("writeClientData") !
        let writeArrayBuffer: ArrayBuffer = strToArrayBuffer(writeClientData) as ArrayBuffer;
        LogUtil.info('sppServerWrite  writeClientData: ' + writeClientData);
        bluetoothManager.sppWrite(clientNumber, writeArrayBuffer);
        LogUtil.info(resStr($r('app.string.bluetoothtest_str_sd')) + i + resStr($r('app.string.bluetoothtest_str_cxrcg_1')));
        message = resStr($r('app.string.bluetoothtest_str_sd_2')) + i + resStr($r('app.string.bluetoothtest_str_cxrcg_2'));
        promptAction.showToast({message: message})
      } else {
        promptAction.showToast({message: resStr($r('app.string.bluetoothtest_str_xhcsdg'))})
        LogUtil.info($r('app.string.bluetoothtest_str_xhcsdg'))
        let readEndTime: number = new Date().getTime()
        let writeStartTime: number = AppStorage.get("writeStartTime") !
        let sppRunTime = ""
        sppRunTime += resStr($r('app.string.bluetoothtest_str_shf')) + Number(readEndTime - writeStartTime) + "ms;" + "\n"
        sppRunTime += resStr($r('app.string.bluetoothtest_str_ks')) + Number(writeStartTime) + "ms; " + resStr($r('app.string.bluetoothtest_str_js')) + Number(readEndTime) + "ms" + "\n"
        AppStorage.setOrCreate("sppClientVelocityBenchmarkTestMessage", sppRunTime)
        LogUtil.info(resStr($r('app.string.bluetoothtest_str_hf')) + Number(readEndTime - writeStartTime) + "ms;" + "\n")
        LogUtil.info(resStr($r('app.string.bluetoothtest_str_ks')) + Number(writeStartTime) + "ms; " + resStr($r('app.string.bluetoothtest_str_js')) + Number(readEndTime) + "ms" + "\n")
        sppClientReadOff()
      }
    } else {
      message = resStr($r('app.string.bluetoothtest_str_dqsb'));
    }
    //    AppStorage.setOrCreate("dataClientReadMessage",message)
    promptAction.showToast({message: message})
    return message
  });
  promptAction.showToast({message: message})
  return message
}


function sppClientReadOff(): string {
  let message = resStr($r('app.string.bluetoothtest_str_st_11'))
  let clientNumber: number = AppStorage.get("clientNumber") !
  LogUtil.info("sppClientReadOff");
  bluetoothManager.off('sppRead', clientNumber, (dataBuffer: ArrayBuffer) => {
    const data = new Uint8Array(dataBuffer);
    LogUtil.info('spp bluetoothManager read is: ' + data);
    if ( data != null ){
      message = resStr($r('app.string.bluetoothtest_str_dqcg_2')) + JSON.stringify(data);
    } else {
      message = resStr($r('app.string.bluetoothtest_str_dqsb'));
    }
    AppStorage.setOrCreate("dataClientReadMessage", message)
    promptAction.showToast({message: message})
    return message
  });
  promptAction.showToast({message: message})
  return message
}

export function strToArrayBuffer(str: string){
  let array = new Uint8Array(str.length);
  for(let i = 0 ; i<str.length ; i++ ){
    array[ i ] = str.charCodeAt(i);
  }
  return array.buffer
}

function loopWriteDate(){
  let a = "1";
  for(let i = 0 ;i<100 ; i++ ){
    a += "1";
  }
  return a
}
