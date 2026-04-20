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
import { PageTitle} from '../../Component/pageTitle';
import { TestImageDisplay} from '../../Component/testImageDisplay';
import { TestData} from '../../MainAbility/model/testData'
import * as GattClientManagerInterface from '../../MainAbility/model/gattClientManagerInterface'
import { testOnBLECharacteristicChangeManager} from '../../MainAbility/model/gattClientManagerInterface'
import { testSetBluetoothScanModeManager, testSetLocalNameManager} from '../../MainAbility/model/brManagerInterface'
import bluetoothManager from '@ohos.bluetoothManager';
import { BusinessError} from '@ohos.base';
import bluetooth from '@ohos.bluetooth';
import promptAction from '@ohos.promptAction';

/**
 *  Bluetooth GattClient Velocity Benchmark Test Page Of Bluetooth test
 */

@Entry
@Component
struct GattClientVelocityBenchmarkTest {
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @State changeIndex: number = - 1
  @State gatt_Cycle_index: number = 10
  @StorageLink("gattClientBenchmarkTestMessage") gattClientBenchmarkTestMessage: string = ""
  private peripheralDeviceId = "08:FB:EA:1B:3C:63"
  @State serviceUUID: string = "00001801-0000-1000-8000-00805f9b34fb";
  @State characteristicUUID: string = "00002b29-0000-1000-8000-00805f9b34fb";
  @State characteristicValue: string = "CccValue";
  @State descriptorUUID: string = "00002902-0000-1000-8000-00805f9b34fb";
  @State descriptorValue: string = "DesValue";

  aboutToAppear(){
    AppStorage.setOrCreate("peripheralDeviceId", this.peripheralDeviceId);
    AppStorage.setOrCreate("serviceUUID", this.serviceUUID);
    AppStorage.setOrCreate("characteristicUUID", this.characteristicUUID);
    AppStorage.setOrCreate("characteristicValue", this.characteristicValue);
    AppStorage.setOrCreate("descriptorUUID", this.descriptorUUID);
    AppStorage.setOrCreate("descriptorValue", this.descriptorValue);
  }

  build(){
    Column(){
      Stack({alignContent: Alignment.TopStart}){
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
              .accessibilityLevel('yes')
              .id('pages_subBenchmarkTestManager_gattClientVelocityBenchmarkTestManager__pt_0')
      }

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));

      Column(){
        Scroll(){
          Column(){
            Text($r('app.string.bluetoothtest_text_xncs'))
              .fontSize(ConfigData.VP_17)
              .margin({top: ConfigData.VP_10, bottom: ConfigData.VP_10, left: ConfigData.VP_20})
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            Text($r('app.string.bluetoothtest_text_gdxncsscsgldsfnxxbhdslxn'))
              .fontSize(ConfigData.VP_17)
              .margin({left: ConfigData.VP_15})
              .backgroundColor($r("app.color.spring"))
            Row(){
              Text($r('app.string.bluetoothtest_text_gci'))
                .fontSize(ConfigData.VP_20)
                .width(ConfigData.VP_180)
                .padding({left: ConfigData.VP_15})
              Column(){
                Select([
                  { value: "100"},
                  { value: "1000"},
                  { value: "10000"}
                ])
                  .borderRadius(ConfigData.VP_10)
                  .backgroundColor($r("app.color.white"))
                  .selected(0)
                  .value("100")
                  .font({size: 17})
                  .margin({right: ConfigData.VP_80})
                  .selectedOptionFont({size: 17})
                  .optionFont({size: ConfigData.VP_15})
                  .width(ConfigData.WH_50_100)
                  .onSelect((index: number) => {
                    LogUtil.info("Select:" + index)
                    AppStorage.setOrCreate("gatt_Cycle_index", index)
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
        .id('pages_subBenchmarkTestManager_gattClientVelocityBenchmarkTestManager__sc_0')
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
            List(){
              ListItem(){
                Text(this.gattClientBenchmarkTestMessage)
                  .fontSize(ConfigData.VP_17)
                  .margin({top: ConfigData.VP_5, left: ConfigData.VP_10, right: ConfigData.VP_10})
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
            .accessibilityLevel('yes')
            .id('pages_subBenchmarkTestManager_gattClientVelocityBenchmarkTestManager__li_0')
            }
            .height(ConfigData.WH_90_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.VP_330)
        }
        .accessibilityLevel('yes')
        .id('pages_subBenchmarkTestManager_gattClientVelocityBenchmarkTestManager__sc_1')
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
          .id('pages_subBenchmarkTestManager_gattClientVelocityBenchmarkTestManager__btn_0')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({top: ConfigData.VP_15, left: ConfigData.VP_5, right: ConfigData.VP_5})
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let gattClientBenchmarkTestMessage = ""
            //创建Gatt客户端
            gattClientBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_11')) + testFunc(GattClientManagerInterface.testCreateGattClientManager) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage", gattClientBenchmarkTestMessage)
            await sleep(5)

            //BLE连接状态
            gattClientBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_2')) + testFunc(GattClientManagerInterface.testOnBLEConnectionStateChangeManager) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage", gattClientBenchmarkTestMessage)
            await sleep(2)

            //连接
            gattClientBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_24')) + testFunc(GattClientManagerInterface.testConnectManager) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage", gattClientBenchmarkTestMessage)
            await sleep(5)

            //设置通知特性更改
            gattClientBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_1')) + testFunc(GattClientManagerInterface.testSetNotifyCharacteristicChangedManager) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage", gattClientBenchmarkTestMessage)
            await sleep(3)

            //BLE特征值变化
            gattClientBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_3')) + testFunc(testOnBLECharacteristicChangeManager) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage", gattClientBenchmarkTestMessage)
            await sleep(2)

            //获取server
            gattClientBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_9')) + testFunc(GattClientManagerInterface.testGetServicesCallbackManager) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage", gattClientBenchmarkTestMessage)
            await sleep(2)

            //获取特征值
            gattClientBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx')) + testFunc(GattClientManagerInterface.testReadCharacteristicValueCallbackManager) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage", gattClientBenchmarkTestMessage)
            await sleep(2)

            //写入特征值
            gattClientBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_4')) + testFunc(writeCharacteristicValue) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage", gattClientBenchmarkTestMessage)
            await sleep(2)
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
          .id('pages_subBenchmarkTestManager_gattClientVelocityBenchmarkTestManager__btn_1')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.blue"))
          .width(ConfigData.VP_100)
          .height(ConfigData.VP_66)
          .margin({top: ConfigData.VP_15, left: ConfigData.VP_5, right: ConfigData.VP_5})
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let gattClientBenchmarkTestMessage = ""

            gattClientBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_23')) + testFunc(GattClientManagerInterface.testDisconnectManager) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage", gattClientBenchmarkTestMessage)
            await sleep(2)

            gattClientBenchmarkTestMessage += resStr($r('app.string.bluetoothtest_str_tzx_16')) + testFunc(GattClientManagerInterface.testGattClientCloseManager) + "\n"
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage", gattClientBenchmarkTestMessage)
            await sleep(2)
          })

          Button({type: ButtonType.Normal, stateEffect: true}){
            Row(){
              Image($r("app.media.ic_public_delete_filled"))
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTestManager_gattClientVelocityBenchmarkTestManager__img_0').width(ConfigData.VP_30).height(ConfigData.VP_40).margin({left: ConfigData.VP_5})
            }
              .alignItems(VerticalAlign.Center)
          }
          .accessibilityLevel('yes')
          .id('pages_subBenchmarkTestManager_gattClientVelocityBenchmarkTestManager__btn_2')
          .borderRadius(ConfigData.VP_8)
          .backgroundColor($r("app.color.moon"))
          .width(ConfigData.VP_50)
          .height(ConfigData.VP_66)
          .margin({top: ConfigData.VP_15, right: ConfigData.VP_40})
          .align(Alignment.Start)
          .onClick(async(event: ClickEvent) => {
            let gattClientBenchmarkTestMessage = ""
            AppStorage.setOrCreate("gattClientBenchmarkTestMessage", gattClientBenchmarkTestMessage)
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

export function test(func: Function){
  let start = new Date().getTime();//起始时间
  func();//执行待测函数
  let end = new Date().getTime();//接受时间
  return (end - start) + "ms";//返回函数执行需要时间
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

let i = 1
let gattClientInstance: bluetooth.GattClientDevice

function writeCharacteristicValue(): string {
  let serviceUUID: string = AppStorage.get('serviceUUID') !;
  let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
  let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
  let descriptorValue: string = AppStorage.get('descriptorValue') !;
  let message = resStr($r('app.string.bluetoothtest_str_wt'))
  let btState = bluetoothManager.getState();
  if ( btState === bluetoothManager.BluetoothState.STATE_OFF ){
    message = resStr($r('app.string.bluetoothtest_str_bine'));
    promptAction.showToast({message: message})
    return message;
  }
  if ( !gattClientInstance ){
    message = "NO_GATT_CLIENT_OBJECT";
    promptAction.showToast({message: message})
    return message;
  }
  if ( i < 101 ){
    let descriptors: Array<bluetooth.BLEDescriptor> = [];
    let bufferCCC = new ArrayBuffer(8);
    let cccV = new Uint8Array(bufferCCC);
    cccV[ 0 ] = 66;
    let descriptor: bluetooth.BLEDescriptor = {
      serviceUuid: serviceUUID,
      characteristicUuid: characteristicUUID,
      descriptorUuid: descriptorUUID,
      descriptorValue: string2ArrayBuffer(descriptorValue)
    }
    descriptors.push(descriptor);
    let bleCharacteristicDataIn: bluetooth.BLECharacteristic = {
      serviceUuid: serviceUUID,
      characteristicUuid: characteristicUUID,
      characteristicValue: bufferCCC,
      descriptors: descriptors
    };
    if ( gattClientInstance.writeCharacteristicValue(bleCharacteristicDataIn) ){
      message = resStr($r('app.string.bluetoothtest_str_ws'));
      readCharacteristicValueCallback()
    } else {
      message = resStr($r('app.string.bluetoothtest_str_wf'));
    }
  }
  promptAction.showToast({message: message})
  return message
}

function readCharacteristicValueCallback(): string {
  let serviceUUID: string = AppStorage.get('serviceUUID') !;
  let characteristicUUID: string = AppStorage.get('characteristicUUID') !;
  let descriptorUUID: string = AppStorage.get('descriptorUUID') !;
  let descriptorValue: string = AppStorage.get('descriptorValue') !;
  let message = resStr($r('app.string.bluetoothtest_str_rt'))
  let btState = bluetoothManager.getState();
  if ( btState === bluetoothManager.BluetoothState.STATE_OFF ){
    message = resStr($r('app.string.bluetoothtest_str_bine'));
    promptAction.showToast({message: message})
  }
  if ( !gattClientInstance ){
    message = "NO_GATT_CLIENT_OBJECT";
    promptAction.showToast({message: message})
  }
  let descriptors: Array<bluetooth.BLEDescriptor> = [];
  let bufferCCC = new ArrayBuffer(8);
  let cccV = new Uint8Array(bufferCCC);
  cccV[ 0 ] = 66;
  let descriptor: bluetooth.BLEDescriptor = {
    serviceUuid: serviceUUID,
    characteristicUuid: characteristicUUID,
    descriptorUuid: descriptorUUID,
    descriptorValue: string2ArrayBuffer(descriptorValue)
  }
  descriptors.push(descriptor);
  let bleCharacteristicDataIn: bluetooth.BLECharacteristic = {
    serviceUuid: serviceUUID,
    characteristicUuid: characteristicUUID,
    characteristicValue: bufferCCC,
    descriptors: descriptors
  };
  gattClientInstance.readCharacteristicValue(bleCharacteristicDataIn, (err: BusinessError, bleCharacteristicDataOut: bluetoothManager.BLECharacteristic) => {
    if ( err.code != 0 ){
      message = resStr($r('app.string.bluetoothtest_str_rec')) + err.code + resStr($r('app.string.bluetoothtest_str_i')) + serviceUUID;
      LogUtil.info(message)
      promptAction.showToast({message: message})
    }
    message = resStr($r('app.string.bluetoothtest_str_rc'))
    message += resStr($r('app.string.bluetoothtest_str_bcu')) + bleCharacteristicDataOut.characteristicUuid + "\n";
    let value = new Uint8Array(bleCharacteristicDataOut.characteristicValue);
    for(let i = 0 ; i<bleCharacteristicDataOut.characteristicValue.byteLength ; i++ ){
      message += resStr($r('app.string.bluetoothtest_str_bcv')) + value[ i ];
    }
    AppStorage.setOrCreate("read_CharacteristicValue", message)
    promptAction.showToast({message: message})
    i++
    writeCharacteristicValue()
  });
  message = resStr($r('app.string.bluetoothtest_str_rcis'));
  promptAction.showToast({message: message})
  return message
}

function string2ArrayBuffer(str: string){
  let array = new Uint8Array(str.length);
  for(let i = 0 ; i<str.length ; i++ ){
    array[ i ] = str.charCodeAt(i);
  }
  return array.buffer
}
