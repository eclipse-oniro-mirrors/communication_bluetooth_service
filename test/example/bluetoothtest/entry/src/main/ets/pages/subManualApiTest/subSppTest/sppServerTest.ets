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
import LogUtil from '../../../Utils/LogUtil';
import ConfigData from '../../../Utils/ConfigData';
import BluetoothModel , { BluetoothDevice } from '../../../MainAbility/model/BluetoothModel';
import BluetoothDeviceController from '../../../MainAbility/controller/BluetoothDeviceController';
import { TestData } from '../../../MainAbility/model/testData'
import { PageTitle } from '../../../Component/pageTitle';
import { ContentTable } from '../../../Component/contentTable';
import { initSppServerApiData } from '../../../MainAbility/model/testDataModels'
import { TestImageDisplay } from '../../../Component/testImageDisplay';
import router from '@ohos.router';

interface PinRequiredData {
  deviceId: string;
  pinCode: string;
}

let MODEL = "bluetoothModel:";

/**
 * SPPServer Test Page Of Bluetooth test
 */

@Entry
@Component
struct SppServer {
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
        BluetoothModel.setDevicePairingConfirmation(this.deviceId , accept);
      }
    }) ,
    alignment : DialogAlignment.Bottom ,
    autoCancel : true
  });

  aboutToAppear() {
    this.btEnable = BluetoothModel.isStateOn();
    BluetoothModel.subscribePinRequired((data: PinRequiredData) => {
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

          Stack().height($r('app.float.wh_value_0_5')).backgroundColor($r('app.color.black'));

          Row() {
            Text("本机设备UUID：").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_85'))
            TextInput({ text : this.uuid , placeholder : "请输入uuid" })
              .fontSize($r('app.float.wh_value_17'))
              .onChange((str) => {
                this.uuid = str;
                AppStorage.setOrCreate('inputUuid' , str)
              })
              .width($r('app.float.wh_value_80p'))
              .borderRadius($r('app.float.wh_value_1'))
          }
          .backgroundColor($r("app.color.spring"))
          .padding($r('app.float.wh_value_5'))
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)

          Row() {
            Text("写入目标设备数据：").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_95'))
            TextInput({ text : this.writeServerData , placeholder : "请输入写入的数据" })
              .fontSize($r('app.float.wh_value_14'))
              .onChange((str) => {
                this.writeServerData = str;
                AppStorage.setOrCreate("writeServerData" , str);
              })
              .width($r('app.float.wh_value_80p'))
              .borderRadius($r('app.float.wh_value_1'))
          }
          .backgroundColor($r("app.color.moon"))
          .padding($r('app.float.wh_value_5'))
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)

          Row() {
            Text("读取的数据为：").fontSize($r('app.float.wh_value_17'))
            Text(this.dataReadMessage)
              .fontSize($r('app.float.wh_value_14'))
              .width($r('app.float.wh_value_80p'))
              .borderRadius($r('app.float.wh_value_1'))
          }
          .padding($r('app.float.wh_value_5'))
          .justifyContent(FlexAlign.Start)
          .alignItems(VerticalAlign.Center)

          Column() {
            Text("接口调用结果：" + this.message).fontSize($r('app.float.wh_value_17'))
              .padding($r('app.float.wh_value_10'))
              .textAlign(TextAlign.Start)
          }
          .backgroundColor($r("app.color.moon"))

          Column() {
            Scroll() {
              ContentTable({ apiItems : initSppServerApiData() }) //子层调用
            }
            .width(ConfigData.WH_100_100)
            .align(Alignment.TopStart)
            .layoutWeight($r('app.float.wh_value_1'))
            .margin({ top : $r('sys.float.ohos_id_card_margin_middle') })
            .height($r('app.float.wh_value_100p'))
          }
          .height($r('app.float.wh_value_85p'))
        }
      }
      .width($r('app.float.wh_value_100p'))
    }
    .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
    .height($r('app.float.wh_value_100p'))
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
    console.log('spp bluetooth read is: ' + data);
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
  private deviceName: string = 'defaultDeviceName';
  private pinCode: string = 'defaultPinCode';
  dialogController: CustomDialogController;
  action!: (accept: boolean) => void;

  aboutToAppear(): void {
    LogUtil.log(ConfigData.TAG + `bluetooth PairDialog aboutToAppear pinCode = ${ this.pinCode }`)
  }

  build() {
    Column() {
      Text($r('app.string.bluetooth_pairing_request'))
        .fontSize($r('app.float.font_22'))
        .height($r('app.float.wh_value_60'))
        .width(ConfigData.WH_100_100)
        .padding({
          left : $r('app.float.distance_24') ,
          top : $r('app.float.distance_14') ,
          bottom : $r('app.float.distance_14')
        })
      Column() {
        if ( this.pinCode ) {
          Text($r('app.string.bluetooth_pairing_intelligent_device_hit' , this.deviceName , this.deviceName))
            .fontSize($r('app.float.font_16'))
            .width(ConfigData.WH_100_100)
            .margin({
              top : $r('app.float.distance_14') ,
              bottom : $r('app.float.distance_14')
            })

          Text(`${ this.pinCode }`)
            .fontSize($r('app.float.pinCode_font_size'))
            .fontWeight(FontWeight.Bolder)
            .width(ConfigData.WH_100_100)
            .textAlign(TextAlign.Center)
            .margin({
              top : $r('app.float.item_common_horizontal_margin') ,
              bottom : $r('app.float.item_common_horizontal_margin')
            })

        } else {
          Text($r('app.string.bluetooth_pairing_media_device_hit'))
            .fontSize($r('app.float.font_16'))
            .width(ConfigData.WH_100_100)
            .margin({
              bottom : $r('app.float.switch_summary_margin')
            })

          Text(this.deviceName)
            .fontSize($r('app.float.font_16'))
            .width(ConfigData.WH_100_100)
            .fontWeight(FontWeight.Bold)
        }

        // button
        Flex({ justifyContent : FlexAlign.Center , alignItems : ItemAlign.Center }) {
          Button() {
            Text($r('app.string.cancel'))
              .fontSize($r('app.float.font_16'))
              .fontColor(Color.Blue)
          }
          .backgroundColor($r("app.color.white_bg_color"))
          .width($r("app.float.wh_value_160"))
          .height($r("app.float.wh_value_40"))
          .flexGrow($r('app.float.wh_value_1'))
          .onClick(() => {
            this.dialogController.close();
            this.action(false);
          })

          Divider()
            .height($r("app.float.wh_value_24"))
            .strokeWidth($r('app.float.wh_value_0_5'))
            .vertical(true)
            .color(($r("app.color.color_E3E3E3_grey")))

          Button() {
            Text($r('app.string.bluetooth_pair_button'))
              .fontSize($r('app.float.font_16'))
              .fontColor(Color.Blue)
          }
          .backgroundColor($r("app.color.white_bg_color"))
          .width($r("app.float.wh_value_160"))
          .height($r("app.float.wh_value_40"))
          .flexGrow($r('app.float.wh_value_1'))
          .onClick(() => {
            this.dialogController.close();
            this.action(true);
          })
        }
        .width(ConfigData.WH_100_100)
        .height($r('app.float.wh_value_60'));
      }
      .width(ConfigData.WH_100_100)
      .padding({
        left : $r('app.float.distance_24') ,
        right : $r('app.float.distance_24')
      })
    }
    .padding({
      left : $r('app.float.distance_24') ,
      right : $r('app.float.distance_24')
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

