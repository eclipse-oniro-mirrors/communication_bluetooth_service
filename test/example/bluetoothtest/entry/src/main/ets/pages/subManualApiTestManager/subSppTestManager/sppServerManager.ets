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

import { myParams } from '../../../Utils/common'
import { TestData } from '../../../MainAbility/model/testData'
import LogUtil from '../../../Utils/LogUtil';
import ConfigData from '../../../Utils/ConfigData';
import BluetoothModelManager from '../../../MainAbility/model/BluetoothModelManager';
import { BluetoothDevice } from '../../../MainAbility/model/BluetoothModel'
import BluetoothDeviceController from '../../../MainAbility/controller/BluetoothDeviceController';
import { PageTitle } from '../../../Component/pageTitle';
import { ContentTable } from '../../../Component/contentTable';
import { initSppServerManagerApiData } from '../../../MainAbility/model/testDataModelsManager'
import { TestImageDisplay } from '../../../Component/testImageDisplay';
import router from '@ohos.router';

interface PinRequiredData {
  deviceId: string;
  pinCode: string;
}

/**
 * SPPServer Manager Test Page Of Bluetooth test
 */

@Entry
@Component
struct SppServerManager {
  @State message: string = "";
  @State deviceId: string = AppStorage.get('inputDeviceId') !;
  @StorageLink('dataReadMessage') dataReadMessage: string = "";
  @State writeServerData: string = '1';
  private uuid: string = "00001101-0000-1000-8000-00805f9b34fb";
  private pinCode: string = '';
  @State pairPinCode: string = '';
  private TAG_PAGE = ConfigData.TAG + 'Paired_Device ';
  @State btEnable: boolean = false;
  @State currentClick: number = - 1;
  @StorageLink('pairedDeviceInfo') pairingDevice: BluetoothDevice = new BluetoothDevice;
  @StorageLink('btDeviceController') controller: BluetoothDeviceController = new BluetoothDeviceController;
  private testItem: TestData = (router.getParams() as myParams).testItem;
  pairDialog: CustomDialogController = new CustomDialogController({
    builder : PairDialog({
      deviceName : this.pairingDevice.deviceName ,
      pinCode : this.pairPinCode ,
      action : (accept: boolean) => {
        BluetoothModelManager.setDevicePairingConfirmation(this.deviceId , accept);
      }
    }) ,
    alignment : DialogAlignment.Bottom ,
    autoCancel : true
  });

  aboutToAppear() {
    this.btEnable = BluetoothModelManager.isStateOn();
    BluetoothModelManager.subscribePinRequired((data: PinRequiredData) => {
      this.pairPinCode = data.pinCode
      this.pairingDevice.deviceId = data.deviceId;
      this.pairingDevice.deviceName = data.deviceId;
      this.pairDialog.open();
    })
    AppStorage.setOrCreate('bluetoothIsOn' , this.btEnable);
    AppStorage.setOrCreate('inputUuid' , this.uuid);
    AppStorage.setOrCreate("writeServerData" , this.writeServerData);
  }

  build() {
    Row() {
      Column() {
        Column() {
          Stack({ alignContent : Alignment.TopStart }) {
            TestImageDisplay({ testItem : this.testItem })
            PageTitle({ testItem : this.testItem })
          }

          Stack().height('0.5vp').backgroundColor($r('app.color.black'));

          Row() {
            Text("本机设备UUID：").fontSize('17vp').width('85vp')
            TextInput({ text : this.uuid , placeholder : "请输入uuid" })
              .fontSize('17vp')
              .onChange((str) => {
                this.uuid = str;
                AppStorage.setOrCreate('inputUuid' , str)
              })
              .width('80%')
              .borderRadius('1vp')
          }
          .backgroundColor($r("app.color.spring"))
          .padding('5vp')
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)

          Row() {
            Text("写入目标设备数据：").fontSize('17vp').width('95vp')
            TextInput({ text : this.writeServerData , placeholder : "请输入写入的数据" })
              .fontSize('14vp')
              .onChange((str) => {
                this.writeServerData = str;
                AppStorage.setOrCreate("writeServerData" , str);
              })
              .width('80%')
              .borderRadius('1vp')
          }
          .backgroundColor($r("app.color.moon"))
          .padding('5vp')
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)

          Row() {
            Text("读取的数据为：").fontSize('17vp')
            Text(this.dataReadMessage)
              .fontSize('14vp')
              .width('80%')
              .borderRadius('1vp')
          }
          .padding('5vp')
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)

          Column() {
            Text("接口调用结果：" + this.message).fontSize('17vp')
              .padding('10vp')
              .textAlign(TextAlign.Start)
          }
          .backgroundColor($r("app.color.moon"))

          Column() {
            Scroll() {
              ContentTable({ apiItems : initSppServerManagerApiData() }) //子层调用
            }
            .width(ConfigData.WH_100_100)
            .align(Alignment.TopStart)
            .layoutWeight(1)
            .margin({ top : $r('sys.float.ohos_id_card_margin_middle') })
            .height('100%')
          }
          .height('85%')
        }
      }
      .width('100%')
    }
    .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
    .height('100%')
    .alignItems(VerticalAlign.Top)
  }

  /**
   * 字符串转arrayBuffer
   * @param {*} str 需要转换的字符串
   * @returns 返回 arrayBuffer
   */
  strToArrayBuffer(str: string) {
    let array = new Uint8Array(str.length);
    for ( let i = 0 ; i < str.length ; i ++ ) {
      array[ i ] = str.charCodeAt(i);
    }
    return array.buffer
  }

  /**
   * Pair device
   * @param device
   */
  pairDevice(deviceId: string) {
    this.controller.pair(this.deviceId , (pinCode: string) => {
      LogUtil.info(this.TAG_PAGE + 'pairDevice success callback : pinCode = ' + pinCode);
      // show pair pin dialog
      this.pairPinCode = pinCode
      this.pairingDevice.deviceId = this.deviceId;
      this.pairingDevice.deviceName = this.deviceId;
      this.pairDialog.open();
    } , () => {
      LogUtil.info(this.TAG_PAGE + 'pairDevice error callback');
      this.showPairFailedDialog()
    })
  }

  /**
   * Confirm pairing
   */
  confirmPairing(accept: boolean) {
    LogUtil.info(this.TAG_PAGE + 'confirmPairing : pairingDevice + ' + JSON.stringify(this.pairingDevice));
    this.controller.confirmPairing(this.pairingDevice.deviceId , accept);
  }

  /**
   * Show pair failed dialog
   */
  showPairFailedDialog() {
    this.showPairingFailedDialog()
  }

  /**
   * Show connect Failed Dialog
   */
  private showConnectFailedDialog() {
    showDialog(
      $r("app.string.bluetooth_connect_failed") ,
      $r("app.string.bluetooth_connect_failed_msg") ,
      $r("app.string.bluetooth_know_button")
    );
  }

  /**
   * Show pairing failed title Dialog
   */
  private showPairingFailedDialog() {
    showDialog(
      $r("app.string.pairing_failed_title") ,
      $r("app.string.pairing_failed_message") ,
      $r("app.string.bluetooth_know_button")
    );
  }

  dataRead(dataBuffer: ArrayBuffer) {
    const data = new Uint8Array(dataBuffer);
    console.log('spp bluetoothManager read is: ' + data);
    if ( data != null ) {
      this.dataReadMessage = "读取成功" + JSON.stringify(data);
    } else {
      this.dataReadMessage = "读取失败";
    }
  }
}

/**
 * Pair dialog
 */
@CustomDialog
struct PairDialog {
  private deviceName: string = 'defaultPinCode';
  private pinCode: string = 'defaultPinCode';
  dialogController: CustomDialogController;
  action!: (accept: boolean) => void;

  aboutToAppear(): void {
    LogUtil.log(ConfigData.TAG + `bluetoothManager PairDialog aboutToAppear pinCode = ${ this.pinCode }`)
  }

  build() {
    Column() {
      Text($r('app.string.bluetooth_pairing_request'))
        .fontSize('22fp')
        .height('60vp')
        .width(ConfigData.WH_100_100)
        .padding({
          left : '24vp' ,
          top : '14vp' ,
          bottom : '14vp'
        })
      Column() {
        if ( this.pinCode ) {
          Text($r('app.string.bluetooth_pairing_intelligent_device_hit' , this.deviceName , this.deviceName))
            .fontSize('16fp')
            .width(ConfigData.WH_100_100)
            .margin({
              top : '14vp' ,
              bottom : '14vp'
            })

          Text(`${ this.pinCode }`)
            .fontSize('32fp')
            .fontWeight(FontWeight.Bolder)
            .width(ConfigData.WH_100_100)
            .textAlign(TextAlign.Center)
            .margin({
              top : '24vp' ,
              bottom : '24vp'
            })
        } else {
          Text($r('app.string.bluetooth_pairing_media_device_hit'))
            .fontSize('16fp')
            .width(ConfigData.WH_100_100)
            .margin({
              bottom : '2vp'
            })

          Text(this.deviceName)
            .fontSize('16fp')
            .width(ConfigData.WH_100_100)
            .fontWeight(FontWeight.Bold)
        }

        // button
        Flex({ justifyContent : FlexAlign.Center , alignItems : ItemAlign.Center }) {
          Button() {
            Text($r('app.string.cancel'))
              .fontSize('16fp')
              .fontColor(Color.Blue)
          }
          .backgroundColor($r("app.color.white_bg_color"))
          .width('160vp')
          .height('40vp')
          .flexGrow(1)
          .onClick(() => {
            this.dialogController.close();
            this.action(false);
          })

          Divider()
            .height('24vp')
            .strokeWidth('0.5vp')
            .vertical(true)
            .color(($r("app.color.color_E3E3E3_grey")))

          Button() {
            Text($r('app.string.bluetooth_pair_button'))
              .fontSize('16fp')
              .fontColor(Color.Blue)
          }
          .backgroundColor($r("app.color.white_bg_color"))
          .width('160vp')
          .height('40vp')
          .flexGrow(1)
          .onClick(() => {
            this.dialogController.close();
            this.action(true);
          })
        }
        .width(ConfigData.WH_100_100)
        .height('60vp');
      }
      .width(ConfigData.WH_100_100)
      .padding({
        left : '24vp' ,
        right : '24vp'
      })
    }
    .padding({
      left : '24vp' ,
      right : '24vp'
    })
    .width(ConfigData.WH_100_100);
  }
}
/**
 * Pair mode prompt
 * @param dialogTitle Dialog title
 * @param dialogMessage Dialog message
 * @param buttonValue Dialog buttonValue
 */
function showDialog(dialogTitle: string | Resource , dialogMessage: string | Resource , buttonValue: string | Resource) {
  AlertDialog.show({
    title : dialogTitle ,
    message : dialogMessage ,
    confirm : {
      value : buttonValue ,
      action : () => {
        LogUtil.info('Button-clicking callback')
      }
    } ,
    cancel : () => {
      LogUtil.info('Closed callbacks')
    } ,
    alignment : DialogAlignment.Bottom
  })
}

