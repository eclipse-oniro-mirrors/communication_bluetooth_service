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
import { resStr} from '../../Utils/resString';


import ConfigData from '../../Utils/ConfigData';
import { testItemFromRouterOrPlaceholder} from '../../Utils/common'
import { PageTitle} from '../../component/pageTitle';
import { TestImageDisplay} from '../../component/testImageDisplay';
import { TestData} from '../../MainAbility/model/testData'
import * as SppServerInterface from '../../MainAbility/model/sppServerInterface'
import bluetooth from '@ohos.bluetooth';
import { testSetBluetoothScanMode, testSetLocalName} from '../../MainAbility/model/brInterface'
import promptAction from '@ohos.promptAction';

/**
 *  Bluetooth SppServer Velocity Benchmark Test Page Of Bluetooth test
 */

@Entry
@Component
struct SppServerVelocityBenchmarkTest {
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @State changeIndex: number = - 1
  @StorageLink("sppServerVelocityBenchmarkTestMessage") sppServerVelocityBenchmarkTestMessage:
    string = ""
  private uuid: string = "00001101-0000-1000-8000-00805f9b34fb";
  @StorageLink('dataServerReadMessage') dataServerReadMessage: string = "00";
  @State writeServerData: string = '1';

  aboutToAppear() {
    AppStorage.setOrCreate('serverV_Cycle_index', '1');
    AppStorage.setOrCreate('inputUuid', this.uuid);
    AppStorage.setOrCreate("writeServerData", this.writeServerData);
  }

  build() {
    Column() {
      Stack({alignContent: Alignment.TopStart}) {
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
        .accessibilityLevel('yes')
        .id('pages_subBenchmarkTest_sppServerVelocityBenchmarkTest__pt_0')
      }

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));

      Column() {
        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_xncs'))
              .fontSize(ConfigData.VP_17)
              .margin({
                top: ConfigData.VP_5,
                bottom: ConfigData.VP_5,
                left: ConfigData.VP_20
              })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            Text($r('app.string.bluetoothtest_text_sdxncsscssldsfxxdslhdkxn'))
              .fontSize(ConfigData.VP_17)
              .margin({left: ConfigData.VP_15})
              .backgroundColor($r("app.color.spring"))
              Stack()
                .height(ConfigData.HEIGHT_1_VP)
                .backgroundColor($r('app.color.black'))

              Row() {
              Text($r('app.string.bluetoothtest_text_sci'))
                .fontSize(ConfigData.VP_17)
                .width(ConfigData.VP_200)
              Column() {
                Select([
                  { value: "100"},
                  { value: "1000"},
                  { value: "10000"}
                ])
                  .borderRadius(ConfigData.VP_10)
                  .backgroundColor($r("app.color.white"))
                  .selected(0)
                  .value($r('app.string.bluetoothtest_str_xhcs'))
                  .font({size: ConfigData.font_17})
                  .margin({right: ConfigData.VP_160})
                  .selectedOptionFont({size: ConfigData.font_17})
                  .optionFont({size: ConfigData.VP_15})
                  .height(ConfigData.VP_36)
                  .width(ConfigData.WH_80_100)
                  .onSelect((index: number) => {
                    LogUtil.info("index Select:" + index)
                    AppStorage.setOrCreate('serverV_Cycle_index', index)
                  })
              }
              .width(ConfigData.WH_80_100)
              }
            .padding(ConfigData.VP_3)
            .width(ConfigData.PCT_96)
            .justifyContent(FlexAlign.Start)
            .alignItems(VerticalAlign.Center)

            Row() {
              Text($r('app.string.bluetoothtest_text_xrmbsbsj'))
                .fontSize(ConfigData.VP_17)
                .width(ConfigData.VP_160)
              TextInput({text: this.writeServerData,
                placeholder: $r('app.string.bluetoothtest_str_qsrxrdsj')})
                .accessibilityLevel('yes')
                .id('pages_subBenchmarkTest_sppServerVelocityBenchmarkTest__ti_0')
                .fontSize(ConfigData.VP_14)
                .backgroundColor($r("app.color.white"))
                .onChange((str) => {
                  this.writeServerData = str;
                  AppStorage.setOrCreate("writeServerData", str);
                })
                .width(ConfigData.WH_50_100)
                .borderRadius(ConfigData.VP_10)
                .height(ConfigData.VP_32)
            }
            .padding(ConfigData.VP_2)
            .width(ConfigData.PCT_96)
            .justifyContent(FlexAlign.Start)
            .alignItems(VerticalAlign.Center)

            Scroll() {
              Column() {
                Text($r('app.string.bluetoothtest_text_dqdsjw'))
                  .fontSize(ConfigData.VP_17)
                  .margin({left: ConfigData.VP_10})
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_100_100)
                Scroll() {
                  List() {
                    ListItem() {
                      Text(this.dataServerReadMessage)
                        .align(Alignment.TopStart)
                        .fontSize(ConfigData.VP_14)
                        .width(ConfigData.WH_90_100)
                        .height(ConfigData.VP_180)
                        .borderRadius(ConfigData.VP_10)
                        .backgroundColor($r("app.color.white"))
                    }
                  .accessibilityLevel('yes')
                  .id('pages_subBenchmarkTest_sppServerVelocityBenchmarkTest__li_0')
                  }
                  .height(ConfigData.PCT_95)
                  .width(ConfigData.WH_90_100)
                  .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
                }
                .accessibilityLevel('yes')
                .id('pages_subBenchmarkTest_sppServerVelocityBenchmarkTest__sc_2')
                .scrollBarWidth(ConfigData.VP_10)
                .scrollBar(BarState.On)
              }
              .width(ConfigData.WH_100_100)
              .height(ConfigData.VP_180)
            }
            .accessibilityLevel('yes')
            .id('pages_subBenchmarkTest_sppServerVelocityBenchmarkTest__sc_1')
            .scrollBarWidth(ConfigData.VP_10)
            .scrollBar(BarState.On)
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.VP_360)
          .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        }
        .accessibilityLevel('yes')
        .id('pages_subBenchmarkTest_sppServerVelocityBenchmarkTest__sc_0')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)

        Stack()
          .height(ConfigData.HEIGHT_1_VP)
          .backgroundColor($r('app.color.black'))

        Scroll() {
          Column() {
            Text($r('app.string.bluetoothtest_text_rz'))
              .fontSize(ConfigData.VP_17)
              .margin({
                top: ConfigData.VP_5,
                bottom: ConfigData.VP_5,
                left: ConfigData.VP_10
              })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
              .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
            List() {
              ListItem() {
                Text(this.sppServerVelocityBenchmarkTestMessage)
                  .fontSize(ConfigData.VP_17)
                  .margin({
                    top: ConfigData.VP_5,
                    left: ConfigData.VP_10,
                    right: ConfigData.VP_10
                  })
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
            .accessibilityLevel('yes')
            .id('pages_subBenchmarkTest_sppServerVelocityBenchmarkTest__li_1')
            }
            .height(ConfigData.WH_90_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.VP_150)
        }
        .accessibilityLevel('yes')
        .id('pages_subBenchmarkTest_sppServerVelocityBenchmarkTest__sc_3')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.On)
      }
      .width(ConfigData.WH_100_100)
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

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
              Text($r('app.string.bluetoothtest_text_slxncs'))
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
          .id('pages_subBenchmarkTest_sppServerVelocityBenchmarkTest__btn_0')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({
            top: ConfigData.VP_15,
            left: ConfigData.VP_5,
            right: ConfigData.VP_5
          })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            try {
            let sppServerVelocityBenchmarkTestMessage = ""

            let writeServerData = loopWriteDate()
            AppStorage.setOrCreate("writeServerData", writeServerData)
            await sleep(2)

            sppServerVelocityBenchmarkTestMessage += "sppServerCreateSafe"
              + testFunc(sppServerCreateSafe)
            AppStorage.setOrCreate("sppServerVelocityBenchmarkTestMessage",
              sppServerVelocityBenchmarkTestMessage)
            await sleep(8)

            sppServerVelocityBenchmarkTestMessage += "sppServerAccept" + testFunc(sppServerAccept)
            AppStorage.setOrCreate("sppServerVelocityBenchmarkTestMessage",
              sppServerVelocityBenchmarkTestMessage)
            await sleep(5)

          
            } catch (e) {
              LogUtil.info(`async onClick failed: ${e.message}, code: ${e.code}`);
            }})

          Button({
            type: ButtonType.Normal,
            stateEffect: true
          }) {
            Row() {
              Text($r('app.string.bluetoothtest_text_jscs'))
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
          .id('pages_subBenchmarkTest_sppServerVelocityBenchmarkTest__btn_1')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_80)
          .height(ConfigData.VP_66)
          .margin({
            top: ConfigData.VP_15,
            left: ConfigData.VP_5,
            right: ConfigData.VP_5
          })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            try {
            let sppServerVelocityBenchmarkTestMessage = ""

            sppServerVelocityBenchmarkTestMessage +=
              resStr($r('app.string.bluetoothtest_str_tzx_14'))
              + testFunc(SppServerInterface.testSppServerReadOff)
              + "\n"
            AppStorage.setOrCreate("sppServerVelocityBenchmarkTestMessage",
              sppServerVelocityBenchmarkTestMessage)
            await sleep(1)

            sppServerVelocityBenchmarkTestMessage +=
              resStr($r('app.string.bluetoothtest_str_tzx_8'))
              + testFunc(SppServerInterface.testSppCloseServerSocket)
              + "\n"
            AppStorage.setOrCreate("sppServerVelocityBenchmarkTestMessage",
              sppServerVelocityBenchmarkTestMessage)
            await sleep(1)

            sppServerVelocityBenchmarkTestMessage +=
              resStr($r('app.string.bluetoothtest_str_szx_2'))
              + testFunc(SppServerInterface.testSppCloseClientSocket)
              + "\n"
            AppStorage.setOrCreate("sppServerVelocityBenchmarkTestMessage",
              sppServerVelocityBenchmarkTestMessage)
            await sleep(1)
          
            } catch (e) {
              LogUtil.info(`async onClick failed: ${e.message}, code: ${e.code}`);
            }})

          Button({
            type: ButtonType.Normal,
            stateEffect: true
          }) {
            Row() {
              Image($r('app.media.ic_public_delete_filled'))
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTest_sppServerVelocityBenchmarkTest__img_0')
          .width(ConfigData.VP_30)
          .height(ConfigData.VP_40)
          .margin({left: ConfigData.VP_5})
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTest_sppServerVelocityBenchmarkTest__btn_2')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.moon"))
          .width(ConfigData.VP_50)
          .height(ConfigData.VP_66)
          .margin({
            top: ConfigData.VP_15,
            right: ConfigData.VP_40
          })
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            try {
            let sppServerVelocityBenchmarkTestMessage = ""
            AppStorage.setOrCreate("sppServerVelocityBenchmarkTestMessage",
              sppServerVelocityBenchmarkTestMessage)
          
            } catch (e) {
              LogUtil.info(`async onClick failed: ${e.message}, code: ${e.code}`);
            }})
        }
        .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        .width(ConfigData.WH_100_100)
      }
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
      .width(ConfigData.WH_100_100)
      .height(ConfigData.WH_100_100);
    }
  }

  dataRead(dataBuffer: ArrayBuffer) {
    const data = new Uint8Array(dataBuffer);
    LogUtil.info('spp bluetooth read is: ' + data);
    if (data != null) {
      this.dataServerReadMessage =
        resStr($r('app.string.bluetoothtest_str_dqcg_2')) + JSON.stringify(data);
    } else {
      this.dataServerReadMessage = resStr($r('app.string.bluetoothtest_str_dqsb'));
    }
  }
}


export function testFunc(func: Function) {
  let start = new Date().getTime()
  switch (func) {
    case testSetLocalName: {
      func("DaYuBlue")
    }
      break;
    case testSetBluetoothScanMode: {
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
  message += resStr($r('app.string.bluetoothtest_str_ks'))
    + Number(start)
    + resStr($r('app.string.bluetoothtest_str_m_1'))
    + resStr($r('app.string.bluetoothtest_str_js'))
    + Number(end)
    + "ms"
    + "\n"
  return message
}

async function sleep(time: number): Promise<void> {
  return new Promise<void>((resolve, reject) => {
    setTimeout(() => {
      resolve()
    }, time * 1000);
  });
}

function choose(index: number) {
  let indexNumber: number = 0
  switch (index) {
    case 0: {
      indexNumber = 100;
    }
      break;
    case 1: {
      indexNumber = 1000
    }
      break;
    case 2: {
      indexNumber = 10000
    }
      break;
    default: {
      indexNumber = 100
    }
      break;
  }
  return indexNumber
}


let serverNumber: number = - 1;
let clientNumber: number = - 1;

function sppServerCreateSafe(): string {
  let message = resStr($r('app.string.bluetoothtest_str_st_10'))
  LogUtil.info('spp server socketId(safe mode) create! ');
  let uuid: string = AppStorage.get('inputUuid') !
  if (uuid == '') {
    message = resStr($r('app.string.bluetoothtest_str_uwk'));
    promptAction.showToast({message: message})
  }
  // 创建蓝牙服务器侦听套接字, 返回服务端ID
  bluetooth.sppListen('BluetoothDevice', {
    uuid: uuid,
    secure: true,
    type: 0
  }, (code,
    serverSocketID) => {
    LogUtil.info('bluetooth error code: ' + typeof code + " ; " + JSON.stringify(code));
    if (!code || code.code == 0) {
      LogUtil.info('spp server: serverSocketID(safe mode) - ' + serverSocketID);
      serverNumber = serverSocketID;
      message = resStr($r('app.string.bluetoothtest_str_cjscg'));
      promptAction.showToast({message: message})
      LogUtil.info('testSppServerAccept test')
      LogUtil.info('testSppServerAccept serverNumber ' + serverNumber)
    }
  })
  promptAction.showToast({message: message})
  return message
}


function sppServerAccept(): string {
  let message = resStr($r('app.string.bluetoothtest_str_tt'))
  let serverReadMessage = ""
  LogUtil.info('testSppServerAccept test')
  LogUtil.info('testSppServerAccept serverNumber ' + serverNumber)
  let i = 1
  let index: number = AppStorage.get('serverV_Cycle_index') !;
  let indexNumber = choose(index)
  let cycleNumber = indexNumber + 1
  LogUtil.info(resStr($r('app.string.bluetoothtest_str_xhsw')) + indexNumber)

  bluetooth.sppAccept(serverNumber, (code, clientSocketID) => {
    if (!code || code.code == 0) {
      LogUtil.info('spp server: clientSocketID - ' + clientSocketID)
      LogUtil.info('test serverNumber ' + serverNumber);
      clientNumber = clientSocketID;
      AppStorage.setOrCreate("clientNumber", clientNumber)
      message = resStr($r('app.string.bluetoothtest_str_jskhdcg'))
      promptAction.showToast({message: message})
      bluetooth.on('sppRead', clientNumber, (dataBuffer: ArrayBuffer) => {
        const data = new Uint8Array(dataBuffer);
        LogUtil.info('spp bluetooth read is: ' + data);
        if (data != null) {
          if (i < cycleNumber) {
            serverReadMessage =
              resStr($r('app.string.bluetoothtest_str_d_1')) + i +
                resStr($r('app.string.bluetoothtest_str_cdqcg')) + "\n" + serverReadMessage
            LogUtil.info(resStr($r('app.string.bluetoothtest_str_d_1')) + i +
              resStr($r('app.string.bluetoothtest_str_cdqcg_1')) + JSON.stringify(data));
            AppStorage.setOrCreate("dataServerReadMessage", serverReadMessage)
            promptAction.showToast({message: serverReadMessage})
            //          sppServerWrite();
            LogUtil.info('sppServerWrite sppWrite clientNumber: ' + clientNumber);
            //            let writeServerData = AppStorage.get("writeServerData") 值为1
            let writeServerData: string = AppStorage.get("writeServerData") !
            let writeArrayBuffer: ArrayBuffer = strToArrayBuffer(writeServerData) as ArrayBuffer;
            LogUtil.info('sppServerWrite  writeServerData: ' + writeServerData);
            const sppWriteSuccess = bluetooth.sppWrite(clientNumber, writeArrayBuffer);
            if (sppWriteSuccess) {
              LogUtil.info(resStr($r('app.string.bluetoothtest_str_sd_1')) + i +
                resStr($r('app.string.bluetoothtest_str_cxrcg_1')));
              message =
                resStr($r('app.string.bluetoothtest_str_sd_3')) + i +
                  resStr($r('app.string.bluetoothtest_str_cxrcg'));
              i++;
            //每次循环 data加一位"1"
            //              writeServerData = writeServerData + "1"
            //              AppStorage.setOrCreate("writeServerData", writeServerData)
            } else {
              LogUtil.info(resStr($r('app.string.bluetoothtest_str_sd_1')) + i +
                resStr($r('app.string.bluetoothtest_str_cxrsb')));
              message =
                resStr($r('app.string.bluetoothtest_str_sd_3')) + i +
                  resStr($r('app.string.bluetoothtest_str_cxrsb'));
            }
            promptAction.showToast({message: message})
          } else {
            promptAction.showToast({message: resStr($r('app.string.bluetoothtest_str_xhcsdg'))})
            sppServerReadOff()
          }
        } else {
          message = resStr($r('app.string.bluetoothtest_str_dqsb'));
        }
        promptAction.showToast({message: message})
      })
      promptAction.showToast({message: message})
    } else {
      message = resStr($r('app.string.bluetoothtest_str_jskhdsb'));
      LogUtil.info("code.code:" + JSON.stringify(code))
      promptAction.showToast({message: message})
    }
  })
  promptAction.showToast({message: message})
  return message
}


function sppServerWrite(): string {
  let message = resStr($r('app.string.bluetoothtest_str_st_20'))
  let clientNumber: number = AppStorage.get("clientNumber") !
  LogUtil.info('sppServerWrite sppWrite clientNumber: ' + clientNumber)
  let writeServerData: string = AppStorage.get("writeServerData") !
  let writeArrayBuffer: ArrayBuffer = strToArrayBuffer(writeServerData) as ArrayBuffer;
  const sppWriteSuccess = bluetooth.sppWrite(clientNumber, writeArrayBuffer);
  if (sppWriteSuccess) {
    LogUtil.info('sppWrite successfully');
    message = resStr($r('app.string.bluetoothtest_str_xrcg'));
  } else {
    LogUtil.info('sppWrite failed');
    message = resStr($r('app.string.bluetoothtest_str_xrsb'));
  }
  promptAction.showToast({message: message})
  return message
}

function sppServerReadOff(): string {
  let message = resStr($r('app.string.bluetoothtest_str_st_12'))
  LogUtil.info('sppServerReadOff');
  let clientNumber: number = AppStorage.get("clientNumber") !
  bluetooth.off('sppRead', clientNumber,
    (dataBuffer: ArrayBuffer) => {
      const data = new Uint8Array(dataBuffer);
      LogUtil.info('spp bluetooth read is: ' + data);
      if (data != null) {
        message = resStr($r('app.string.bluetoothtest_str_dqcg_2')) + JSON.stringify(data);
      } else {
        message = resStr($r('app.string.bluetoothtest_str_dqsb'));
      }
      AppStorage.setOrCreate("dataServerReadMessage", message)
      promptAction.showToast({message: message})
      return message
    }
  );
  promptAction.showToast({message: message})
  return message
}

function strToArrayBuffer(str: string) {
  let array = new Uint8Array(str.length);
  for(let i = 0 ; i<str.length ; i++) {
    array[i] = str.charCodeAt(i);
  }
  return array.buffer
}

function loopWriteDate() {
  let a = "1";
  for(let i = 0 ;i<100 ; i++) {
    a += "1";
  }
  return a
}
