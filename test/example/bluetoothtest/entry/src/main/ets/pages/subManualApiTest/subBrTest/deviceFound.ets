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
import { resStr } from '../../../Utils/resString';


import ConfigData from '../../../Utils/ConfigData';
import LogUtil from '../../../Utils/LogUtil';
import BluetoothDeviceController from '../../../MainAbility/controller/BluetoothDeviceController';
import { BluetoothDevice , DeviceState , DeviceType } from '../../../MainAbility/model/BluetoothModel'
import router from '@ohos.router';
import ImageAnimatorComponent from '../../../Component/imageAnimatorComponent';
import { PageTitle } from '../../../Component/pageTitle';
import bluetooth from '@ohos.bluetooth';
import promptAction from '@ohos.promptAction';

interface BluetoothDeviceParam {
  bluetoothDevice: string;
}

/**
 * deviceFound page of bluetooth Test
 */

const PAIRED_ITEM_NUMBER = 3;
const PAGE_URI_BLUETOOTH_PAIRED_DEVICE_INFO = 'pages/subManualApiTest/subBrTest/bluetoothPairedDeviceInfo';

@Entry
@Component
struct DeviceFound {
  private PAGE_TAG = ConfigData.TAG + 'Bluetooth page '
  private deviceController: BluetoothDeviceController = new BluetoothDeviceController();
  @State message: string = 'BrTest';
  @State bluetoothSwitch: boolean = false;
  @State scanSwitch: boolean = false;
  @StorageLink('bluetoothIsOn') isOn: boolean = false;
  @StorageLink('bluetoothToggleEnabled') bluetoothToggleEnabled: boolean = true;
  @StorageLink('bluetoothLocalName') localName: string = '';

  aboutToAppear(): void {
    LogUtil.log(this.PAGE_TAG + 'aboutToAppear in : isOn = ' + this.isOn)
    this.deviceController
      .initData()
      .subscribe();
    LogUtil.log(this.PAGE_TAG + 'aboutToAppear out : isOn = ' + this.isOn)
  }

  onPageShow(): void {
    LogUtil.log(this.PAGE_TAG + 'onPageShow in : localName = ' + this.localName)
    this.deviceController.getLocalName();
    AppStorage.setOrCreate('btDeviceController' , this.deviceController);
    LogUtil.log(this.PAGE_TAG + 'onPageShow out : localName = ' + this.localName)
  }

  build() {
    Column() {
      GridContainer({
        columns : 12 ,
        sizeType : SizeType.Auto ,
        gutter : vp2px(1) === 2 ? ConfigData.VP_12 : ConfigData.VP_0 ,
        margin : vp2px(1) === 2 ? ConfigData.VP_24 : ConfigData.VP_0
      }) {
        Row({}) {
          Column() {
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.WH_100_100)
          .useSizeType({
            xs : { span : 0 , offset : 0 } , sm : { span : 0 , offset : 0 } ,
            md : { span : 0 , offset : 0 } , lg : { span : 2 , offset : 0 }
          });
          Column({ space : ConfigData.VP_12 }) {
            PageTitle({ detail : $r('app.string.bluetoothtest_str_smlysb') })
              .accessibilityLevel('yes')
              .id('pages_subManualApiTest_subBrTest_deviceFound__pt_0')
            Scroll() {
              Column() {
                if ( this.isOn ) {
                  PairedDeviceComponent({
                    controller : this.deviceController
                  })

                  AvailableDeviceComponent({
                    controller : this.deviceController
                  })

                }
              }
              .width(ConfigData.WH_100_100)
              .margin({ bottom : $r('sys.float.ohos_id_default_padding_bottom_fixed') })
            }
            .accessibilityLevel('yes')
            .id('pages_subManualApiTest_subBrTest_deviceFound__sc_0')
            .width(ConfigData.WH_100_100)
            .align(Alignment.TopStart)
            .layoutWeight(1)
            .margin({ top : $r('sys.float.ohos_id_card_margin_middle') })
          }
          .padding({
            left : ConfigData.VP_24 ,
            right : ConfigData.VP_24 ,
            bottom : ConfigData.VP_32
          })
          .width(ConfigData.WH_100_100)
          .height(ConfigData.WH_100_100)
          .useSizeType({
            xs : { span : 12 , offset : 0 } , sm : { span : 12 , offset : 0 } ,
            md : { span : 12 , offset : 0 } , lg : { span : 8 , offset : 2 }
          });

          Column() {
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.WH_100_100)
          .useSizeType({
            xs : { span : 0 , offset : 12 } , sm : { span : 0 , offset : 12 } ,
            md : { span : 0 , offset : 12 } , lg : { span : 2 , offset : 10 }
          })
        }
        .width(ConfigData.WH_100_100)
        .height(ConfigData.WH_100_100);
      }
      .width(ConfigData.WH_100_100)
      .height(ConfigData.WH_100_100);
    }
    .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
    .width(ConfigData.WH_100_100)
    .height(ConfigData.WH_100_100);
  }
}

/**
 * Paired device component
 */

@Component
export struct PairedDeviceComponent {
  private TAG_PAGE = ConfigData.TAG + 'PairedDeviceComponent ';
  controller!: BluetoothDeviceController;
  @StorageLink('bluetoothPairedDevices') pairedDevices: BluetoothDevice[] = [];
  @State isTouched: boolean = false;
  @State hide: boolean = true;
  @State currentPairedDevice: string = "";

  aboutToAppear(): void {
    if ( this.controller ) {
      // bind component and initialize
      this.controller.bindComponent(this)
        .bindProperties([ "pairedDevices" ])
        .initData();
    }
  }

  build() {
    Column() {
      if ( this.pairedDevices && this.pairedDevices.length > 0 ) {
        // paired devices title
        Row() {
          Text($r('app.string.bluetooth_paired_devices'))
            .fontSize(ConfigData.FP_14)
            .fontColor($r('app.color.font_color_182431'))
        }
        .width(ConfigData.WH_100_100)
        .padding({
          left : ConfigData.VP_24 ,
          top : ConfigData.VP_19_5 ,
          bottom : ConfigData.VP_9_5
        })

        List() {
          // paired devices list
          ForEach(this.pairedDevices , (item: BluetoothDevice , index: number) => {
            ListItem() {
              Row() {
                PairedItem({
                  name : item.deviceName ,
                  type : item.deviceType ,
                  state : item.connectionState.toString() ,
                  mac : item.deviceId
                })
              }
              .width(ConfigData.WH_100_100)
              .borderRadius(ConfigData.VP_24)
              .padding(ConfigData.VP_4)
              .backgroundColor($r("app.color.white_bg_color"))
              .onClick(() => {
                this.itemClicked(item);
              })

            }
        .id(index === 0 ? 'pages_subManualApiTest_subBrTest_deviceFound__li_0' : `pages_subManualApiTest_subBrTest_deviceFound__li_${index}`)
          } , (item: BluetoothDevice) => JSON.stringify(item));
        }
        .height(ConfigData.VP_200)
        .divider({
          strokeWidth : ConfigData.HEIGHT_1_VP ,
          color : $r('app.color.color_E3E3E3_grey') ,
          startMargin : ConfigData.VP_52 ,
          endMargin : ConfigData.VP_12
        })
        .backgroundColor($r("app.color.white_bg_color"))
        .borderRadius(ConfigData.VP_24)

      }
    }
  }

  /**
   * Whether headphone connected.
   * @param item device
   * @return headphone connected or not
   */
  private isHeadPhoneConnected(item: BluetoothDevice): boolean {
    return item.deviceType === DeviceType.HEADPHONE && item.connectionState === DeviceState.STATE_CONNECTED
  }

  /**
   * Goto paired device Info
   * @param item device
   */

  private gotoPairedDeviceInfo(item: BluetoothDevice) {
    if ( item.connectionState != DeviceState.STATE_CONNECTING
    && item.connectionState != DeviceState.STATE_DISCONNECTING ) {
      LogUtil.info(this.TAG_PAGE + 'item right icon on click : ' + JSON.stringify(item));
      let param: BluetoothDeviceParam = {
        bluetoothDevice : JSON.stringify(item)
      }
      router.pushUrl({
        url : PAGE_URI_BLUETOOTH_PAIRED_DEVICE_INFO ,
        params : param
      });
    }
  }

  /**
   * Item clicked
   * @param item device
   */
  private itemClicked(item: BluetoothDevice) {
    switch ( item.connectionState ) {
      case DeviceState.STATE_CONNECTED:
        this.showDisconnectDialog(item.deviceName , () => {
          this.controller.disconnect(item.deviceId)
        });
        break;
      case DeviceState.STATE_DISCONNECTED:
        if ( !this.controller.connect(item.deviceId) ) {
          this.showConnectFailedDialog();
        }
        break;
    }
  }

  /**
   * Connect Failed Dialog
   */
  private showConnectFailedDialog() {
    showDialog(
      $r("app.string.bluetooth_connect_failed") ,
      $r("app.string.bluetooth_connect_failed_msg") ,
      $r("app.string.bluetooth_know_button")
    );
  }

  /**
   * Disconnect Dialog
   */
  showDisconnectDialog(deviceName: string , callback: () => void) {
    AlertDialog.show({
      title : $r('app.string.bluetooth_disconnect') ,
      message : $r("app.string.bluetooth_disconnect_device" , deviceName) ,
      primaryButton : {
        value : $r('app.string.cancel') ,
        action : () => {
          LogUtil.info(ConfigData.TAG + 'Closed callbacks');
        }
      } ,
      secondaryButton : {
        value : $r('app.string.confirm') ,
        action : () => {
          LogUtil.info(ConfigData.TAG + `AlertDialog success:`);
          callback();
        }
      } ,
      alignment : DialogAlignment.Bottom
    })
  }
}

/**
 * Discovering animator component
 */
@Component
struct DiscoveringAnimatorComponent {
  build() {
    Column() {
      Row() {
        ImageAnimatorComponent({
          imageWidth : 40 ,
          imageHeight : 40 })
      }
    }
  }
}

/**
 * AvailableItem
 */
@Component
export struct AvailableItem {
  private mac: string = 'defaultMac'
  private type: string = 'defaultType'
  private name: string = 'defaultName'
  private state: string = 'defaultState'

  build() {
    Row() {
      Row() {
        Image($r('app.media.bleSmall'))
          .accessibilityLevel('yes')
          .id('pages_subManualApiTest_subBrTest_deviceFound__img_0')
          .objectFit(ImageFit.Contain)
          .autoResize(false)
          .height(ConfigData.VP_30)
          .width(ConfigData.VP_30)
      }
      .width(ConfigData.PCT_10)
      .padding({ left : ConfigData.VP_10 })

      Column() {
        Text(this.name)
          .fontSize(ConfigData.VP_17_4)
          .fontWeight(FontWeight.Bold)
          .width(ConfigData.WH_90_100)
          .height(ConfigData.VP_40)
          .padding({ left : ConfigData.VP_10 , top : ConfigData.VP_15 })
        Text(this.mac)
          .fontSize(ConfigData.VP_17_4)
          .fontWeight(FontWeight.Bold)
          .width(ConfigData.WH_90_100)
          .height(ConfigData.VP_30)
          .padding({ left : ConfigData.VP_10 , top : ConfigData.VP_5 })
      }
      .height(ConfigData.VP_80)
      .width(ConfigData.WH_80_100)

      Row() {
        Text(this.state)
          .fontSize(ConfigData.VP_17_4)
          .flexGrow(1)
      }
      .width(ConfigData.PCT_10)
      .padding({ left : ConfigData.VP_10 })
    }
  }
}

/**
 * AvailableItem
 */
@Component
export struct PairedItem {
  private mac: string = 'defaultMac'
  private type: string = 'defaultType'
  private name: string = 'defaultName'
  private state: string = 'defaultState'

  build() {
    Row() {
      Row() {
        Image($r('app.media.bleSmall'))
          .accessibilityLevel('yes')
          .id('pages_subManualApiTest_subBrTest_deviceFound__img_1')
          .objectFit(ImageFit.Contain)
          .autoResize(false)
          .height(ConfigData.VP_30)
          .width(ConfigData.VP_30)
      }
      .width(ConfigData.PCT_10)
      .padding({ left : ConfigData.VP_10 })

      Column() {
        Text(this.name)
          .fontSize(ConfigData.VP_17_4)
          .fontWeight(FontWeight.Bold)
          .width(ConfigData.WH_90_100)
          .height(ConfigData.VP_40)
          .padding({ left : ConfigData.VP_10 , top : ConfigData.VP_15 })
        Text(this.mac)
          .fontSize(ConfigData.VP_17_4)
          .fontWeight(FontWeight.Bold)
          .width(ConfigData.WH_90_100)
          .height(ConfigData.VP_30)
          .padding({ left : ConfigData.VP_10 , top : ConfigData.VP_5 })
      }
      .height(ConfigData.VP_80)
      .width(ConfigData.PCT_70)

      Row() {
        Text(this.state)
          .fontSize(ConfigData.VP_17_4)
          .flexGrow(1)
      }
      .width(ConfigData.PCT_10)
      .padding({ left : ConfigData.VP_10 })

      Row() {
        Text($r('app.string.cancel'))
          .fontSize(ConfigData.VP_17_4)
          .flexGrow(1)
      }
      .width(ConfigData.PCT_10)
      .padding({ left : ConfigData.VP_10 })
      .onClick(() => {
        let ret = bluetooth.cancelPairedDevice(this.mac)
        if ( ret ) {
          promptAction.showToast({ message : resStr($r('app.string.bluetoothtest_str_qxpdcg')) })
        } else {
          promptAction.showToast({ message : resStr($r('app.string.bluetoothtest_str_qxpdsb')) })
        }
      })
    }
  }
}

/**
 * Available device component
 */
@Component
export struct AvailableDeviceComponent {
  private TAG_PAGE = ConfigData.TAG + 'AvailableDeviceComponent ';
  @State isDeviceDiscovering: boolean = false;
  @StorageLink('bluetoothAvailableDevices') availableDevices: BluetoothDevice[] = [];
  @State pairPinCode: string = '';
  private pairingDevice!: BluetoothDevice
  @State pairDeviceInfo: Resource | string = $r('app.string.bluetoothtest_str_wpd');
  controller!: BluetoothDeviceController;
  pairDialog: CustomDialogController = new CustomDialogController({
    builder : PairDialog({
      deviceName : this.pairingDevice.deviceName ,
      pinCode : this.pairPinCode ,
      action : (accept: boolean) => {
        this.confirmPairing(accept);
      }
    }) ,
    alignment : DialogAlignment.Bottom ,
    autoCancel : true
  });

  aboutToAppear(): void {
    if ( this.controller ) {

      // bind component and initialize
      this.controller.bindComponent(this)
        .bindProperties([ "isDeviceDiscovering", "availableDevices", "pairPinCode" ])
        .initData();

      this.controller.startBluetoothDiscovery();
    }
  }

  build() {
    Column() {
      Row() {
        // available devices title
        Text($r('app.string.bluetooth_available_devices'))
          .fontSize(ConfigData.FP_14)
          .fontColor($r('app.color.font_color_182431'))
          .width(ConfigData.WH_100_100)
          .padding({
            left : ConfigData.VP_24 ,
            top : ConfigData.VP_19_5 ,
            bottom : ConfigData.VP_9_5
          })
        Blank()

        // bluetooth discovering
        if ( this.isDeviceDiscovering ) {
          DiscoveringAnimatorComponent()
        }
      }
      .width(ConfigData.WH_100_100)

      if ( this.availableDevices && this.availableDevices.length >= 1 ) {
        Scroll() {
          List() {
            // paired devices list
            ForEach(this.availableDevices , (item: BluetoothDevice, index: number) => {
              ListItem() {
                Row() {
                  AvailableItem({
                    name : item.deviceName ,
                    type : item.deviceType ,
                    state : item.connectionState.toString() ,
                    mac : item.deviceId
                  })
                }
                .width(ConfigData.WH_100_100)
                .borderRadius(ConfigData.VP_24)
                .padding(ConfigData.VP_4)
                .backgroundColor($r("app.color.white_bg_color"))
                .onClick(() => {
                  LogUtil.info(this.TAG_PAGE + 'item on click : ' + JSON.stringify(item));
                  AppStorage.setOrCreate('pairedMac' , item.deviceId);
                  //                AppStorage.setOrCreate('pairedName', item.deviceName);
                  this.pairDevice(item)
                })
              }
          .accessibilityLevel('yes')
          .id('pages_subManualApiTest_subBrTest_deviceFound__li_1')
            } , (item: BluetoothDevice) => JSON.stringify(item));
          }
          .backgroundColor($r("app.color.white_bg_color"))
          .borderRadius(ConfigData.VP_24)
          .height(ConfigData.WH_80_100)
          .divider({
            strokeWidth : ConfigData.HEIGHT_1_VP ,
            color : $r('app.color.color_E3E3E3_grey') ,
            startMargin : ConfigData.VP_52 ,
            endMargin : ConfigData.VP_12
          })
        }
        .accessibilityLevel('yes')
        .id('pages_subManualApiTest_subBrTest_deviceFound__sc_1')
        .scrollBar(BarState.Auto)
        .scrollBarWidth(ConfigData.VP_20)
      } else {
        Row() {
          // Scanning...
          Text($r('app.string.scanning'))
            .fontSize(ConfigData.FP_20)
            .textCase(TextCase.UpperCase);
        }
      }
    }
  }

  /**
   * Pair device
   * @param device
   */
  pairDevice(device: BluetoothDevice) {
    this.controller.pair(device.deviceId , (pinCode: string) => {
      LogUtil.info(this.TAG_PAGE + 'pairDevice success callback : pinCode = ' + pinCode);
      // show pair pin dialog
      this.pairPinCode = pinCode;
      this.pairingDevice = device;
      AppStorage.setOrCreate('pairedDeviceInfo' , device);
      AppStorage.setOrCreate('pairedDeviceId' , device.deviceId);
      this.pairDialog.open();
      this.pairDeviceInfo = 'pairDeviceID: ' + device.deviceId + ' true';
    } , () => {
      this.pairDeviceInfo = 'pairDeviceID: ' + device.deviceId + ' false';
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
        .fontSize(ConfigData.FP_22)
        .height(ConfigData.VP_60)
        .width(ConfigData.WH_100_100)
        .padding({
          left : ConfigData.VP_24 ,
          top : ConfigData.VP_14 ,
          bottom : ConfigData.VP_14
        })
      Column() {
        if ( this.pinCode ) {
          Text($r('app.string.bluetooth_pairing_intelligent_device_hit' , this.deviceName , this.deviceName))
            .fontSize(ConfigData.FP_16)
            .width(ConfigData.WH_100_100)
            .margin({
              top : ConfigData.VP_14 ,
              bottom : ConfigData.VP_14
            })

          Text(`${ this.pinCode }`)
            .fontSize(ConfigData.FP_32)
            .fontWeight(FontWeight.Bolder)
            .width(ConfigData.WH_100_100)
            .textAlign(TextAlign.Center)
            .margin({
              top : ConfigData.VP_24 ,
              bottom : ConfigData.VP_24
            })

        } else {
          Text($r('app.string.bluetooth_pairing_media_device_hit'))
            .fontSize(ConfigData.FP_16)
            .width(ConfigData.WH_100_100)
            .margin({
              bottom : ConfigData.VP_2
            })

          Text(this.deviceName)
            .fontSize(ConfigData.FP_16)
            .width(ConfigData.WH_100_100)
            .fontWeight(FontWeight.Bold)
        }

        // button
        Flex({ justifyContent : FlexAlign.Center , alignItems : ItemAlign.Center }) {

          Button() {
            Text($r('app.string.cancel'))
              .fontSize(ConfigData.FP_16)
              .fontColor(Color.Blue)
          }
          .accessibilityLevel('yes')
          .id('pages_subManualApiTest_subBrTest_deviceFound__btn_0')
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

          Button() {
            Text($r('app.string.bluetooth_pair_button'))
              .fontSize(ConfigData.FP_16)
              .fontColor(Color.Blue)
          }
          .accessibilityLevel('yes')
          .id('pages_subManualApiTest_subBrTest_deviceFound__btn_1')
          .backgroundColor($r("app.color.white_bg_color"))
          .width(ConfigData.VP_160)
          .height(ConfigData.VP_40)
          .flexGrow(1)
          .onClick(() => {
            this.dialogController.close();
            this.action(true);
          })
        }
        .width(ConfigData.WH_100_100)
        .height(ConfigData.VP_60);
      }
      .width(ConfigData.WH_100_100)
      .padding({
        left : ConfigData.VP_24 ,
        right : ConfigData.VP_24
      })
    }
    .padding({
      left : ConfigData.VP_24 ,
      right : ConfigData.VP_24
    })
    .width(ConfigData.WH_100_100);
  }
}

/**
 * Get device icon resource
 * @param type
 * @return device icon path
 */
function getDeviceIconPath(deviceType: string): string {
  let path: string = "/res/image/ic_bluetooth_device.svg";
  switch ( deviceType ) {
    case DeviceType.HEADPHONE:
      path = "/res/image/ic_device_earphone_hero.svg";
      break;

    case DeviceType.PHONE:
      path = "/res/image/ic_public_devices_phone.svg";
      break;

    case DeviceType.WATCH:
      path = "/res/image/ic_device_watch.svg";
      break;

    case DeviceType.COMPUTER:
      path = "/res/image/ic_device_matebook.svg";
      break;
  }
  return path
}

/**
 * Get connection state text
 * @param connectionState
 */
function getConnectionStateText(device: BluetoothDevice): Resource {
  let stateText: Resource = $r('app.string.bluetooth_state_unknown');
  switch ( device.connectionState ) {
    case DeviceState.STATE_DISCONNECTED:
      stateText = $r('app.string.bluetooth_state_disconnected');
      break;

    case DeviceState.STATE_CONNECTING:
      stateText = $r('app.string.bluetooth_state_connecting');
      break;

    case DeviceState.STATE_CONNECTED:
      if ( device.deviceType === DeviceType.HEADPHONE ) {
        stateText = $r('app.string.bluetooth_state_connected');
      } else {
        stateText = $r('app.string.bluetooth_state_unknown');
      }
      break;

    case DeviceState.STATE_DISCONNECTING:
      stateText = $r('app.string.bluetooth_state_disconnecting');
      break;

    case DeviceState.STATE_PAIRING:
      stateText = $r('app.string.bluetooth_state_pairing');
      break;

  }
  return stateText;
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