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
import hilog from '@ohos.hilog';
import { testItemFromRouterOrPlaceholder} from '../../Utils/common'
import { PageTitle} from '../../component/pageTitle';
import { TestImageDisplay} from '../../component/testImageDisplay';
import { ContentTable} from '../../component/contentTable';
import BluetoothModel, { BluetoothDevice} from '../../MainAbility/model/BluetoothModel';
import BluetoothDeviceController from '../../MainAbility/controller/BluetoothDeviceController';
import ConfigData from '../../Utils/ConfigData';
import LogUtil from '../../Utils/LogUtil';
import { initBRApiData} from '../../MainAbility/model/testDataModels'
import { TestData} from '../../MainAbility/model/testData'

/**
 * Br Test Page Of Bluetooth test
 */

const TAG = "[Bluetooth]"

@Entry
@Component
struct BrTest {
  private TAG_PAGE = ConfigData.TAG + 'Paired_Device ';
  private testItem: TestData = testItemFromRouterOrPlaceholder();
  @State btEnable: boolean = false;
  @StorageLink('setLocalNameRet') setLocalNameRet: boolean = false;
  @StorageLink('setScanModeRet') setScanModeRet: boolean = false;
  @State deviceId: string = '';
  @State currentClick: number = - 1;
  @StorageLink('pairedDeviceInfo') pairingDevice: BluetoothDevice = new BluetoothDevice;
  @StorageLink('pairedDeviceId') pairedDeviceId: string = '';
  @StorageLink('btDeviceController') controller: BluetoothDeviceController = new BluetoothDeviceController;
  @StorageLink('pairedMac') pairedMac: string = "6c:96:d7:3d:87:6f";
  @State pairPinCode: string = '';
  @State btOnState: string = 'on:stateChange';
  @State btPinRequired: string = 'on:pinRequired';
  @State btBondStateChange: string = 'on:bondStateChange';
  @State isStateChangeClick: boolean = false;
  @State isPinRequiredClick: boolean = false;
  @State isBondStateChangeClick: boolean = false;
  pairDialog: CustomDialogController = new CustomDialogController({
    builder: PairDialog({
      deviceName: this.pairingDevice.deviceName,
      pinCode: this.pairPinCode,
      action: (accept: boolean) => {
        LogUtil.info("PairDialog start -------")
        this.confirmPairing(accept)
        LogUtil.info("PairDialog end -------")
      }
    }),
    alignment: DialogAlignment.Bottom,
    autoCancel: true
  })

  aboutToAppear(){
    hilog.info(0x0000, 'BtTest', '%{public}s %{public}s', TAG, "this.testItem:" + JSON.stringify(this.testItem))
    this.btEnable = BluetoothModel.isStateOn();
    this.controller.subscribePinRequired((deviceId, pinCode) => {
      this.pairPinCode = pinCode
      this.pairingDevice.deviceId = deviceId;
      this.pairingDevice.deviceName = deviceId;
      this.pairDialog.open();
    })
    AppStorage.setOrCreate('bluetoothIsOn', this.btEnable);
    AppStorage.setOrCreate('pairedMac', this.pairedMac);
  }

  init(){
    LogUtil.info("init-----------")
  }

  build(){
    Column(){
      Stack({alignContent: Alignment.TopStart}){
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_BrInterfaceTest__pt_0')
      }

      Stack()
        .height(ConfigData.VP_0_5)
        .backgroundColor($r('app.color.black'))
      Column(){
        Row(){
          Text($r('app.string.bluetoothtest_text_mbsbm'))
            .fontSize(ConfigData.VP_18)
            .height(ConfigData.VP_40)
            .width(ConfigData.WH_30_100)
            .align(Alignment.Start)
          TextInput({text: this.pairedMac, placeholder: $r('app.string.bluetoothtest_str_qsrjjljdsbm')})
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_BrInterfaceTest__ti_0')
            .height(ConfigData.VP_40)
            .borderStyle(BorderStyle.Dashed)
            .backgroundColor($r("app.color.white"))
            .onChange((str) => {
              this.deviceId = str;
            })
            .width(ConfigData.PCT_60)
            .padding({top: ConfigData.VP_5, left: ConfigData.VP_5})
        }
      }
      .align(Alignment.TopStart)
      ContentTable({apiItems: initBRApiData()})
    }
    .alignItems(HorizontalAlign.Center)
    .backgroundColor($r("app.color.lead"))
    .height(ConfigData.WH_100_100)
  }

  /**
   * Pair device
   * @param device
   */
  pairDevice(deviceId: string){
    this.controller.pair(deviceId, (pinCode: string) => {
      LogUtil.info(this.TAG_PAGE + 'pairDevice success callback : pinCode = ' + pinCode);
      // show pair pin dialog
      this.pairPinCode = pinCode
      this.pairingDevice.deviceId = deviceId;
      this.pairingDevice.deviceName = deviceId;
      this.pairDialog.open();
    }, () => {
      LogUtil.info(this.TAG_PAGE + 'pairDevice error callback');
      this.showPairFailedDialog()
    })
  }

  /**
   * Confirm pairing
   */
  confirmPairing(accept: boolean){
    LogUtil.info("confirmPairing start -----------")
    LogUtil.info(this.TAG_PAGE + 'confirmPairing : pairingDevice + ' + JSON.stringify(this.pairingDevice))
    this.controller.confirmPairing(this.pairingDevice.deviceId, accept);
    LogUtil.info("confirmPairing end -----------")
  }

  /**
   * Show pair failed dialog
   */
  showPairFailedDialog(){
    this.showPairingFailedDialog()
  }

  /**
   * Show connect Failed Dialog
   */
  private showConnectFailedDialog(){
    showDialog(
      $r("app.string.bluetooth_connect_failed"),
      $r("app.string.bluetooth_connect_failed_msg"),
      $r("app.string.bluetooth_know_button"))
  }

  /**
   * Show pairing failed title Dialog
   */
  private showPairingFailedDialog(){
    showDialog(
      $r("app.string.pairing_failed_title"),
      $r("app.string.pairing_failed_message"),
      $r("app.string.bluetooth_know_button")
    );
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

  build(){
    Column(){
      Text($r('app.string.bluetooth_pairing_request'))
        .fontSize(ConfigData.FP_22)
        .height(ConfigData.VP_60)
        .width(ConfigData.WH_100_100)
        .padding({
          left: ConfigData.VP_24,
          top: ConfigData.VP_14,
          bottom: ConfigData.VP_14
        })
      Column(){
        if ( this.pinCode ){
          Text($r('app.string.bluetooth_pairing_intelligent_device_hit', this.deviceName, this.deviceName))
            .fontSize(ConfigData.FP_16)
            .width(ConfigData.WH_100_100)
            .margin({
              top: ConfigData.VP_14,
              bottom: ConfigData.VP_14
            })

          Text(`${ this.pinCode }`)
            .fontSize(ConfigData.FP_32)
            .fontWeight(FontWeight.Bolder)
            .width(ConfigData.WH_100_100)
            .textAlign(TextAlign.Center)
            .margin({
              top: ConfigData.VP_24,
              bottom: ConfigData.VP_24
            })

        } else {
          Text($r('app.string.bluetooth_pairing_media_device_hit'))
            .fontSize(ConfigData.FP_16)
            .width(ConfigData.WH_100_100)
            .margin({
              bottom: ConfigData.VP_2
            })

          Text(this.deviceName)
            .fontSize(ConfigData.FP_16)
            .width(ConfigData.WH_100_100)
            .fontWeight(FontWeight.Bold)
        }

        // button
        Flex({justifyContent: FlexAlign.Center, alignItems: ItemAlign.Center}){

          Button(){
            Text($r('app.string.cancel'))
              .fontSize(ConfigData.FP_16)
              .fontColor(Color.Blue)
          }
          .accessibilityLevel('yes')
          .id('pages_subManualApiTest_BrInterfaceTest__btn_0')
          .backgroundColor($r("app.color.white_bg_color"))
          .width(ConfigData.VP_160)
          .height(ConfigData.VP_40)
          .flexGrow(1)
          .onClick(() => {
            this.dialogController.close();
            this.action(false);
          })

          Divider()
          .height(ConfigData.VP_24)
          .strokeWidth(ConfigData.VP_0_5)
          .vertical(true)
          .color(($r("app.color.color_E3E3E3_grey")))

          Button(){
            Text($r('app.string.bluetooth_pair_button'))
              .fontSize(ConfigData.FP_16)
              .fontColor(Color.Blue)
          }
          .accessibilityLevel('yes')
          .id('pages_subManualApiTest_BrInterfaceTest__btn_1')
          .backgroundColor($r("app.color.white_bg_color"))
          .width(ConfigData.VP_160)
          .height(ConfigData.VP_40)
          .flexGrow(1)
          .onClick(() => {
            this.dialogController.close();
            LogUtil.info("onClick start --------------")
            this.action(true)
            LogUtil.info("onClick end --------------")
          })
        }
        .width(ConfigData.WH_100_100)
        .height(ConfigData.VP_60)
      }
      .width(ConfigData.WH_100_100)
      .padding({
        left: ConfigData.VP_24,
        right: ConfigData.VP_24
      })
    }
    .padding({
      left: ConfigData.VP_24,
      right: ConfigData.VP_24
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
function showDialog(dialogTitle: string | Resource, dialogMessage: string | Resource, buttonValue: string | Resource){
  AlertDialog.show({
    title: dialogTitle,
    message: dialogMessage,
    confirm: {
      value: buttonValue,
      action: () => {
        LogUtil.info('Button-clicking callback')
      }
    },
    cancel: () => {
      LogUtil.info('Closed callbacks')
    },
    alignment: DialogAlignment.Bottom
  })
}
