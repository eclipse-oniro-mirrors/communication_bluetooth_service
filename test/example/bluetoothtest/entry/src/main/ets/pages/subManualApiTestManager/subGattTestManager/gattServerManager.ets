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
import { TestImageDisplay } from '../../../Component/testImageDisplay';
import router from '@ohos.router';
import ConfigData from '../../../Utils/ConfigData';
import bluetoothManager from '@ohos.bluetoothManager';
import { ContentTable } from '../../../Component/contentTable';
import { PageTitle } from '../../../Component/pageTitle';
import { initGattServerManagerApiData } from '../../../MainAbility/model/testDataModelsManager'
import { TestData } from '../../../MainAbility/model/testData'
import promptAction from '@ohos.promptAction';
import bluetooth from '@ohos.bluetooth'
/**
 * Gatt Server Manager Test Page Of Bluetooth test
 */

@Component
struct GattServerSetting {
  // GATT service
  private curServiceUUID: string = '00001877-0000-1000-8000-00805F9B34FB';
  private curCharacteristicUUID: string = '00002BE0-0000-1000-8000-00805F9B34FB';
  private curCharacteristicValue: string = 'DaYuBlue'; //OHOS:BLE NANJING CHINA
  private curDescriptorUUID: string = '00002902-0000-1000-8000-00805F9B34FB';
  private curDescriptorValue: string = 'Descriptor';

  // Advertising input parameters:
  /*settings*/
  private cbxAdvSettings: boolean = true;
  private cbxAdvResponse: boolean = false;
  private cbxInterval: boolean = false;
  private advSettings_interval: string = "1600"; //default: 1s

  private cbxTxPower: boolean = false;
  private advSettings_txPower: string = "-7"; //default: -7 dbm, range:-127~1

  private cbxConnectable: boolean = true;
  private advSettings_connectable: boolean = true; //default: true
  // @State selectAdvSettings_connectableIndex:number = 0;

  /* Adv Data*/
  private advDataServiceUuids_serviceUuid: string = "000054F6-0000-1000-8000-00805f9b34fb";
  private advDataManufactureData_manufactureId: string = "7456";
  private advDataManufactureData_manufactureValue: string = "CHN";
  private advDataServiceData_serviceUuid: string = "000054F6-0000-1000-8000-00805f9b34fb";
  private advDataServiceData_serviceValue: string = "AdvValue";

  /*Adv Response*/
  private advResponseServiceUuids_serviceUuid: string = "000054F6-0000-1000-8000-00805f9b34fb";
  private advResponseManufactureData_manufactureId: string = "6547";
  private advResponseManufactureData_manufactureValue: string = "CHN";
  private advResponseServiceData_serviceUuid: string = "000054F6-0000-1000-8000-00805f9b34fb";
  private advResponseServiceData_serviceValue: string = "AdvValue";

  build() {
    Column() {
      Column() {
        Text("Gatt服务配置:")
          .margin({ top : $r('app.float.wh_value_1') })
          .padding($r('app.float.wh_value_1'))
          .fontSize($r('app.float.wh_value_17'))
          .fontColor("#4444444")
          .height($r('app.float.wh_value_30'))
          .width($r('app.float.wh_value_94p'))
          .textAlign(TextAlign.Start)
        Scroll() {
          Column() {
            Row() {
              Text("服务UUID:").fontSize($r('app.float.wh_value_15')).width($r('app.float.wh_value_70')).padding({ left : 10 });
              TextInput({ text : this.curServiceUUID , placeholder : "input service UUID" })
                .fontSize($r('app.float.wh_value_17'))
                .onChange((strInput) => {
                  this.curServiceUUID = strInput;
                  AppStorage.setOrCreate('curServiceUUID' , strInput);
                })
                .width($r('app.float.wh_value_80p'))
                .borderRadius(1)
            }
            .height($r('app.float.wh_value_60'))
            .width($r('app.float.wh_value_100p'))
            .padding($r('app.float.wh_value_5'))
            .backgroundColor($r("app.color.moon"));

            Column() {
              Row() {
                Text("CCC UUID:").fontSize($r('app.float.wh_value_15')).width($r('app.float.wh_value_70')).padding({ left : 10 });
                TextInput({ text : this.curCharacteristicUUID , placeholder : "input characteristic UUID" })
                  .fontSize($r('app.float.wh_value_17'))
                  .onChange((strInput) => {
                    this.curCharacteristicUUID = strInput;
                    AppStorage.setOrCreate('curCharacteristicUUID' , strInput);
                  })
                  .width($r('app.float.wh_value_80p'))
                  .borderRadius(1)
              }
              .width($r('app.float.wh_value_100p'))
              .padding($r('app.float.wh_value_5'))
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)

              Row() {
                Text("CCC Value:").fontSize($r('app.float.wh_value_15')).width($r('app.float.wh_value_70')).padding({ left : 10 });
                TextInput({ text : this.curCharacteristicValue , placeholder : "input characteristic Value" })
                  .fontSize($r('app.float.wh_value_17'))
                  .onChange((strInput) => {
                    this.curCharacteristicValue = strInput;
                    AppStorage.setOrCreate('curCharacteristicValue' , strInput);
                  })
                  .width($r('app.float.wh_value_80p'))
                  .borderRadius(1)
              }
              .width($r('app.float.wh_value_100p'))
              .padding($r('app.float.wh_value_5'))
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
            }
            .height($r('app.float.wh_value_100'))
            .width($r('app.float.wh_value_100p'))
            .padding($r('app.float.wh_value_5'))
            .backgroundColor($r("app.color.spring"));

            Flex({ direction : FlexDirection.Column }) {
              Row() {
                Text("Descriptor UUID:").fontSize($r('app.float.wh_value_15')).width($r('app.float.wh_value_70'));
                TextInput({ text : this.curDescriptorUUID , placeholder : "input descriptor UUID" })
                  .fontSize($r('app.float.wh_value_17'))
                  .onChange((strInput) => {
                    this.curDescriptorUUID = strInput;
                    AppStorage.setOrCreate('curDescriptorUUID' , strInput);
                  })
                  .width($r('app.float.wh_value_80p'))
                  .borderRadius(1)
              }
              .width($r('app.float.wh_value_100p'))
              .padding($r('app.float.wh_value_5'))
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)

              Row() {
                Text("Descriptor Value:").fontSize($r('app.float.wh_value_15')).width($r('app.float.wh_value_70'));
                TextInput({ text : this.curDescriptorValue , placeholder : "input descriptor Value" })
                  .fontSize($r('app.float.wh_value_17'))
                  .onChange((strInput) => {
                    this.curDescriptorValue = strInput;
                    AppStorage.setOrCreate('curDescriptorValue' , strInput);
                  })
                  .width($r('app.float.wh_value_80p'))
                  .borderRadius(1)
              }
              .width($r('app.float.wh_value_100p'))
              .padding($r('app.float.wh_value_5'))
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
            }
            .height($r('app.float.wh_value_100'))
            .width($r('app.float.wh_value_100p'))
            .backgroundColor($r("app.color.moon"));
          }
        }
        .height($r('app.float.wh_value_22p'))
      }
      .backgroundColor($r("app.color.lead"))

      Column() {
        Row() {
          Text("广播配置:")
            .margin({ top : $r('app.float.wh_value_1') })
            .padding($r('app.float.wh_value_5'))
            .fontSize($r('app.float.wh_value_17'))
            .fontColor("#4444444")
            .height($r('app.float.wh_value_30'))
            .width($r('app.float.wh_value_70p'))
            .textAlign(TextAlign.Start)
          Checkbox({ name : 'AdvSettings' , group : 'checkboxGroup1' })
            .select(this.cbxAdvSettings)
            .selectedColor(0x39a2db)
            .onChange((value: boolean) => {
              if ( value ) {
                this.cbxAdvSettings = true;
              } else {
                this.cbxAdvSettings = false;
              }
              AppStorage.setOrCreate('cbxAdvSettings' , value);
              promptAction.showToast({ message : String(value) })
            })
            .width($r('app.float.wh_value_30'))
            .margin({ left : $r('app.float.wh_value_1') })
          Text("设置")
            .fontSize($r('app.float.wh_value_14'))
            .fontColor("#4444444")
        }
        .height($r('app.float.wh_value_40'))

        Scroll() {
          Column() {
            Column() {
              Row() {
                Checkbox({ name : 'interval' , group : 'checkboxGroup1' })
                  .select(this.cbxInterval)
                  .selectedColor(0x39a2db)
                  .width($r('app.float.wh_value_30'))
                  .onChange((value) => {
                    if ( value ) {
                      this.cbxInterval = true;
                    } else {
                      this.cbxInterval = false;
                    }
                    AppStorage.setOrCreate('cbxInterval' , value);
                  })
                Text("interval").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_70'));
                TextInput({ text : this.advSettings_interval , placeholder : "32~1600" })
                  .fontSize($r('app.float.wh_value_18'))
                  .onChange((strInput) => {
                    this.advSettings_interval = strInput;
                    AppStorage.setOrCreate('advSettings_interval' , strInput);
                  })
                  .width($r('app.float.wh_value_60p'))
                  .borderRadius(1)
              }
              .height($r('app.float.wh_value_40'))
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)

              Row() {
                Checkbox({ name : 'txPower' , group : 'checkboxGroup' })
                  .select(this.cbxTxPower)
                  .selectedColor(0x39a2db)
                  .width($r('app.float.wh_value_30'))
                  .onChange((value) => {
                    if ( value ) {
                      this.cbxTxPower = true;
                    } else {
                      this.cbxTxPower = false;
                    }
                    AppStorage.setOrCreate('cbxTxPower' , value);
                  })
                Text("txPower").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_70'))
                TextInput({ text : this.advSettings_txPower.toString() , placeholder : "input txPower" })
                  .fontSize($r('app.float.wh_value_18'))
                  .onChange((strInput) => {
                    this.advSettings_txPower = strInput;
                    AppStorage.setOrCreate('advSettings_txPower' , strInput);
                  })
                  .width($r('app.float.wh_value_60p'))
                  .borderRadius(1)
              }
              .height($r('app.float.wh_value_40'))
              .padding({ top : 5 })
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)

              Row() {
                Checkbox({ name : 'connectable' , group : 'checkboxGroup' })
                  .select(this.cbxConnectable)
                  .selectedColor(0x39a2db)
                  .onChange((value) => {
                    if ( value ) {
                      this.cbxConnectable = true;
                    } else {
                      this.cbxConnectable = false;
                    }
                    this.advSettings_connectable = this.cbxConnectable;
                    AppStorage.setOrCreate('advSettings_connectable' , value);
                  })
                  .width($r('app.float.wh_value_30'))
                  .margin({ left : $r('app.float.wh_value_1') })
                Text("connectable").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_80p'))
              }
              .height($r('app.float.wh_value_40'))
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
            }
            .height($r('app.float.wh_value_120'))
            .width($r('app.float.wh_value_100p'))
            .padding($r('app.float.wh_value_5'))
            .backgroundColor(0xAFEEEE);

            Column() {
              Row() {
                Text("服务Uuid").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_60')).padding({ left : 10 })
                TextInput({ text : this.advDataServiceUuids_serviceUuid , placeholder : "input serviceUuid" })
                  .fontSize($r('app.float.wh_value_17'))
                  .onChange((strInput) => {
                    this.advDataServiceUuids_serviceUuid = strInput;
                    AppStorage.setOrCreate('advDataServiceUuids_serviceUuid' , strInput);
                  })
                  .width($r('app.float.wh_value_80p'))
              }
              .width($r('app.float.wh_value_100p'))

              Column() {
                Text("manufactureData").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_100p'))
                Row() {
                  Text("Id").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_30'))
                  TextInput({
                    text : this.advDataManufactureData_manufactureId ,
                    placeholder : "input manufactureId(less than 8 digi)"
                  })
                    .fontSize($r('app.float.wh_value_17'))
                    .onChange((strInput) => {
                      this.advDataManufactureData_manufactureId = strInput;
                      AppStorage.setOrCreate('advDataManufactureData_manufactureId' , strInput)
                    })
                    .width($r('app.float.wh_value_100'))
                  Text("Value").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_70')).padding({ left : 10 })
                  TextInput({
                    text : this.advDataManufactureData_manufactureValue ,
                    placeholder : "input manufactureValue(less than 8 char)"
                  })
                    .fontSize($r('app.float.wh_value_17'))
                    .onChange((strInput) => {
                      this.advDataManufactureData_manufactureValue = strInput;
                      AppStorage.setOrCreate('advDataManufactureData_manufactureValue' , strInput)
                    })
                    .width($r('app.float.wh_value_100'))
                }
                .width($r('app.float.wh_value_90p'))
              }
              .borderWidth(1)
              .borderColor($r("app.color.lead"))

              Column() {
                Text("serviceData").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_100p'))
                Column() {
                  Row() {
                    Text("serviceUuid").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_70'))
                    TextInput({ text : '' + this.advDataServiceData_serviceUuid , placeholder : "input serviceUuid" })
                      .fontSize($r('app.float.wh_value_17'))
                      .onChange((strInput) => {
                        this.advDataServiceData_serviceUuid = strInput;
                        AppStorage.setOrCreate('advDataServiceData_serviceUuid' , strInput)
                      }).width($r('app.float.wh_value_80p'))
                  }

                  Row() {
                    Text("serviceValue").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_70'))
                    TextInput({
                      text : this.advDataServiceData_serviceValue ,
                      placeholder : "input manufactureValue(less than 8 char)"
                    })
                      .fontSize($r('app.float.wh_value_17'))
                      .onChange((strInput) => {
                        this.advDataServiceData_serviceValue = strInput;
                        AppStorage.setOrCreate('advDataServiceData_serviceValue' , strInput)
                      })
                      .width($r('app.float.wh_value_80p'))
                  }
                  .padding({ top : 10 })
                }
              }
              .borderWidth(1)
              .borderColor($r("app.color.lead"))
            }
            .width($r('app.float.wh_value_100p'))
            .backgroundColor($r("app.color.moon"));

            Row() {
              Checkbox({ name : 'AdvResponse' , group : 'checkboxGroup1' })
                .select(this.cbxAdvResponse)
                .selectedColor(0x39a2db)
                .onChange((value) => {
                  if ( value ) {
                    this.cbxAdvResponse = true;
                  } else {
                    this.cbxAdvResponse = false;
                  }
                  AppStorage.setOrCreate('cbxAdvResponse' , value)
                  console.log('cbxAdvResponse' , value)
                  promptAction.showToast({ message : value.toString() })
                })
                .width($r('app.float.wh_value_30'))
                .margin({ left : $r('app.float.wh_value_1') })
              Text("Response:")
                .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_5') })
                .fontSize($r('app.float.wh_value_17'))
                .fontColor("#4444444");
            }
            .height($r('app.float.wh_value_40'))
            .width($r('app.float.wh_value_100p'))
            .padding($r('app.float.wh_value_5'));
            Column() {
              Row() {
                Text("serviceUuid").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_70')) //.width($r('app.float.wh_value_40'))
                TextInput({ text : this.advResponseServiceUuids_serviceUuid , placeholder : "input serviceUuid" })
                  .fontSize($r('app.float.wh_value_17'))
                  .onChange((strInput) => {
                    this.advResponseServiceUuids_serviceUuid = strInput;
                    AppStorage.setOrCreate('advResponseServiceUuids_serviceUuid' , strInput)
                  })
                  .width($r('app.float.wh_value_80p'))
              }

              Column() {
                Text("manufactureData").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_100p'))
                  .backgroundColor($r("app.color.lead"))
                Column() {
                  Row() {
                    Text("Id").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_50'))
                    TextInput({ text : this.advResponseManufactureData_manufactureId ,
                      placeholder : "input manufactureId(less than 8 char)" })
                      .fontSize($r('app.float.wh_value_17'))
                      .onChange((strInput) => {
                        this.advResponseManufactureData_manufactureId = strInput;
                        AppStorage.setOrCreate('advResponseManufactureData_manufactureId' , strInput)
                      })
                      .width($r('app.float.wh_value_80p'))
                  }

                  Row() {
                    Text("Value").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_50'))
                    TextInput({ text : this.advResponseManufactureData_manufactureValue ,
                      placeholder : "input manufactureValue(less than 8 char)" })
                      .fontSize($r('app.float.wh_value_17'))
                      .onChange((strInput) => {
                        this.advResponseManufactureData_manufactureValue = strInput;
                        AppStorage.setOrCreate('advResponseManufactureData_manufactureValue' , strInput)
                      })
                      .width($r('app.float.wh_value_80p'))
                  }
                  .padding({ top : 5 })
                }
              }
              .borderWidth(1)
              .borderColor($r("app.color.lead"))

              Column() {
                Text("serviceData").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_100p'))
                  .backgroundColor($r("app.color.lead"))
                Column() {
                  Row() {
                    Text("serviceUuid").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_70'))
                    TextInput({
                      text : '' + this.advResponseServiceData_serviceUuid ,
                      placeholder : "input serviceUuid"
                    })
                      .fontSize($r('app.float.wh_value_17'))
                      .onChange((strInput) => {
                        this.advResponseServiceData_serviceUuid = strInput;
                        AppStorage.setOrCreate('advResponseServiceData_serviceUuid' , strInput)
                      })
                      .width($r('app.float.wh_value_80p'))
                  }

                  Row() {
                    Text("serviceValue").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_70'))
                    TextInput({
                      text : this.advResponseServiceData_serviceValue ,
                      placeholder : "input manufactureValue(less than 8 char)"
                    })
                      .fontSize($r('app.float.wh_value_17'))
                      .onChange((strInput) => {
                        this.advResponseServiceData_serviceValue = strInput;
                        AppStorage.setOrCreate('advResponseServiceData_serviceValue' , strInput)
                      })
                      .width($r('app.float.wh_value_80p'))
                  }
                  .padding({ top : 5 })
                }
              }
              .borderWidth(1)
              .borderColor($r("app.color.lead"))
            }
            .height($r('app.float.wh_value_85p'))
            .width($r('app.float.wh_value_100p'))
            .padding($r('app.float.wh_value_5'))
            .backgroundColor($r("app.color.moon"));
          }
        }
        .height($r('app.float.wh_value_50p'))
      }
      .backgroundColor($r("app.color.lead"))
    }
  }
}

@Entry
@Component
struct GattServerManager {
  @State message: string = 'GattServerManager'
  @State currentClick: number = - 1;
  @State showList: boolean = false;
  @StorageLink("connect_StateMsg") connectStateMsg: string = "initial";
  @StorageLink("characteristicReadMsg") characteristicReadMsg: string = "initial";
  @StorageLink("characteristicWriteMsg") characteristicWriteMsg: string = "initial";
  @StorageLink("descriptorReadMsg") descriptorReadMsg: string = "initial";
  @StorageLink("descriptorWriteMsg") descriptorWriteMsg: string = "initial";
  private testItem: TestData = (router.getParams() as myParams).testItem;

  //----------all @state -> private-------------------

  // GATT service
  private curServiceUUID: string = '00001877-0000-1000-8000-00805F9B34FB';
  private curServiceIsPrimary: boolean = true;
  private curCharacteristicUUID: string = '00002BE0-0000-1000-8000-00805F9B34FB';
  private curCharacteristicValue: string = 'DaYuBlue '; //OHOS:BLE NANJING CHINA
  private curDescriptorUUID: string = '00002902-0000-1000-8000-00805F9B34FB';
  private curDescriptorValue: string = 'Descriptor';
  private curGattService: bluetoothManager.GattService = {
    serviceUuid : '' ,
    isPrimary : true ,
    characteristics : [] ,
    includeServices : []
  }

  // Notify Characteristic
  private notifyCharacteristic_serviceUuid: string = '00001810-0000-1000-8000-00805F9B34FB';
  private notifyCharacteristic_characteristicUuid: string = '00001821-0000-1000-8000-00805F9B34FB';
  private notifyCharacteristic_characteristicValue: string = 'test';
  private notifyCharacteristic_confirm: boolean = false;
  private notifyCharacteristic_notifyCharacteristic: bluetoothManager.NotifyCharacteristic = {
    serviceUuid : '00001810-0000-1000-8000-00805F9B34FB' ,
    characteristicUuid : '00001821-0000-1000-8000-00805F9B34FB' ,
    characteristicValue : string2ArrayBuffer("999") ,
    confirm : false
  }

  // ServerResponse
  private serverResponse_deviceId: string = '08:FB:EA:1B:3C:63'; //'88:36:CF:09:C1:90'
  private serverResponse_transId: string = '1';
  private serverResponse_status: string = '0';
  private serverResponse_offset: string = '0';
  private serverResponse_value: string = 'respond value';
  private serverResponse: bluetoothManager.ServerResponse = {
    deviceId : '08:FB:EA:1B:3C:63' , //'88:36:CF:09:C1:90'
    transId : 1 ,
    status : 0 ,
    offset : 0 ,
    value : string2ArrayBuffer("respond value")
  };
  // Advertising input parameters:
  /*settings*/
  private cbxAdvSettings: boolean = true;
  private cbxAdvResponse: boolean = false;
  private cbxInterval: boolean = false;
  private advSettings_interval: string = "1600"; //default: 1s

  private cbxTxPower: boolean = false;
  private advSettings_txPower: string = "-7"; //default: -7 dbm, range:-127~1

  private cbxConnectable: boolean = true;
  private advSettings_connectable: boolean = true; //default: true
  // @State selectAdvSettings_connectableIndex:number = 0;

  /* Adv Data*/
  private advDataServiceUuids_serviceUuid: string = "000054F6-0000-1000-8000-00805f9b34fb";
  private advDataManufactureData_manufactureId: string = "7456";
  private advDataManufactureData_manufactureValue: string = "CHN";
  private advDataServiceData_serviceUuid: string = "000054F6-0000-1000-8000-00805f9b34fb";
  private advDataServiceData_serviceValue: string = "AdvValue";

  /*Adv Response*/
  private advResponseServiceUuids_serviceUuid: string = "000054F6-0000-1000-8000-00805f9b34fb";
  private advResponseManufactureData_manufactureId: string = "6547";
  private advResponseManufactureData_manufactureValue: string = "CHN";
  private advResponseServiceData_serviceUuid: string = "000054F6-0000-1000-8000-00805f9b34fb";
  private advResponseServiceData_serviceValue: string = "AdvValue";
  private curAdvSettings: bluetoothManager.AdvertiseSetting = {
    interval : 800 , //500ms
    txPower : - 7 ,
    connectable : true
  };
  private curAdvData: bluetoothManager.AdvertiseData = {
    serviceUuids : [ "00003344-0000-1000-8000-00805f9b34fb", "00008899-0000-1000-8000-00805f9b34fb" ] ,
    manufactureData : [ {
      manufactureId : 7456 ,
      manufactureValue : string2ArrayBuffer("OHOS CHN")
    } ] ,
    serviceData : [ {
      serviceUuid : "000054F6-0000-1000-8000-00805f9b34fb" ,
      serviceValue : string2ArrayBuffer("OhosBle!")
    }, {
      serviceUuid : "00009999-0000-1000-8000-00805f9b34fb" ,
      serviceValue : string2ArrayBuffer("dudu_tiger")
    } ]
  };
  private curAdvResponse: bluetoothManager.AdvertiseData = {
    serviceUuids : [ //"000054F6-0000-1000-8000-00805f9b34fb",
      "00009999-0000-1000-8000-00805f9b34fb" ] ,
    manufactureData : [ {
      manufactureId : 6547 ,
      manufactureValue : string2ArrayBuffer("OHOS CHN")
    } ] ,
    serviceData : [ {
      serviceUuid : "000054F6-0000-1000-8000-00805f9b34fb" ,
      serviceValue : string2ArrayBuffer("OhosBle!")
    }, {
      serviceUuid : "00007456-0000-1000-8000-00805f9b34fb" ,
      serviceValue : string2ArrayBuffer("OhosBle")
    } ]
  };

  //------------@State->private------------------------------

  aboutToAppear() {
    AppStorage.setOrCreate('curServiceUUID' , this.curServiceUUID);
    AppStorage.setOrCreate('curCharacteristicUUID' , this.curCharacteristicUUID);
    AppStorage.setOrCreate('curDescriptorUUID' , this.curDescriptorUUID);
    AppStorage.setOrCreate('curDescriptorValue' , this.curDescriptorValue);
    AppStorage.setOrCreate('curCharacteristicValue' , this.curCharacteristicValue);
    AppStorage.setOrCreate('isPrimary' , this.curServiceIsPrimary);
    AppStorage.setOrCreate('advSettings_interval' , this.advSettings_interval);
    AppStorage.setOrCreate('advSettings_txPower' , this.advSettings_txPower);
    AppStorage.setOrCreate('advSettings_connectable' , this.advSettings_connectable);
    AppStorage.setOrCreate('advDataServiceUuids_serviceUuid' , this.advDataServiceUuids_serviceUuid);
    AppStorage.setOrCreate('advDataManufactureData_manufactureId' , this.advDataManufactureData_manufactureId);
    AppStorage.setOrCreate('advDataManufactureData_manufactureValue' , this.advDataManufactureData_manufactureValue);
    AppStorage.setOrCreate('advDataServiceData_serviceUuid' , this.advDataServiceData_serviceUuid);
    AppStorage.setOrCreate('advDataServiceData_serviceValue' , this.advDataServiceData_serviceValue);
    AppStorage.setOrCreate('advResponseServiceUuids_serviceUuid' , this.advResponseServiceUuids_serviceUuid);
    AppStorage.setOrCreate('advResponseManufactureData_manufactureId' , this.advResponseManufactureData_manufactureId);
    AppStorage.setOrCreate('advResponseManufactureData_manufactureValue' , this.advResponseManufactureData_manufactureValue);
    AppStorage.setOrCreate('advResponseServiceData_serviceUuid' , this.advResponseServiceData_serviceUuid);
    AppStorage.setOrCreate('advResponseServiceData_serviceValue' , this.advResponseServiceData_serviceValue);
    AppStorage.setOrCreate('cbxAdvSettings' , this.cbxAdvSettings);
    AppStorage.setOrCreate('cbxInterval' , this.cbxInterval);
    AppStorage.setOrCreate('cbxTxPower' , this.cbxTxPower);
    AppStorage.setOrCreate('cbxAdvResponse' , this.cbxAdvResponse);
  }

  build() {
    Column() {
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
      }

      Stack().height($r('app.float.wh_value_0_5')).backgroundColor("#000000");
      Row() {
        Text("GattServerSetting")
          .fontSize($r('app.float.wh_value_20'))
          .height($r('app.float.wh_value_60'))
          .width($r('app.float.wh_value_80p'))
          .padding({ left : 10 })
        Image($r('app.media.Switch'))
          .height($r('app.float.wh_value_50'))
          .width($r('app.float.wh_value_50'))
          .padding({ top : 10 , bottom : 10 , left : 10 })
          .onClick(() => {
            this.showList = !this.showList;
          })
      }
      .width($r('app.float.wh_value_100p'))

      Column() {
        if ( this.showList ) {
          GattServerSetting()
        } else {
          Column() {
            Column() {
              Text("结果显示:")
                .fontSize($r('app.float.wh_value_17'))
                .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_10') })
                .textAlign(TextAlign.Start)
                .width($r('app.float.wh_value_100p'))
              Text("Result:" + this.message)
                .fontSize($r('app.float.wh_value_17'))
                .margin({ top : $r('app.float.wh_value_1') , left : $r('app.float.wh_value_10') })
                .textAlign(TextAlign.Start)
                .width($r('app.float.wh_value_90p'))
              Text("ConnectionStateChange:" + this.connectStateMsg)
                .fontSize($r('app.float.wh_value_17'))
                .margin({ top : $r('app.float.wh_value_1') , left : $r('app.float.wh_value_10') })
                .textAlign(TextAlign.Start)
                .width($r('app.float.wh_value_90p'))
              Text("characteristicRead:" + this.characteristicReadMsg)
                .fontSize($r('app.float.wh_value_17'))
                .margin({ top : $r('app.float.wh_value_1') , left : $r('app.float.wh_value_10') })
                .textAlign(TextAlign.Start)
                .width($r('app.float.wh_value_90p'))
              Text("characteristicWrite:" + this.characteristicWriteMsg)
                .fontSize($r('app.float.wh_value_17'))
                .margin({ top : $r('app.float.wh_value_1') , left : $r('app.float.wh_value_10') })
                .textAlign(TextAlign.Start)
                .width($r('app.float.wh_value_90p'))
              Text("descriptorRead:" + this.descriptorReadMsg)
                .fontSize($r('app.float.wh_value_17'))
                .margin({ top : $r('app.float.wh_value_1') , left : $r('app.float.wh_value_10') })
                .textAlign(TextAlign.Start)
                .width($r('app.float.wh_value_90p'))
              Text("descriptorWrite:" + this.descriptorWriteMsg)
                .fontSize($r('app.float.wh_value_17'))
                .margin({ top : $r('app.float.wh_value_1') , left : $r('app.float.wh_value_10') })
                .textAlign(TextAlign.Start)
                .width($r('app.float.wh_value_90p'))
            }
            .width($r('app.float.wh_value_95p'))
            .height($r('app.float.wh_value_160'))
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))

            Stack().height($r('app.float.wh_value_0_25')).backgroundColor("#000000");
            Column() {
              Text("功能测试:")
                .margin({ top : $r('app.float.wh_value_5') , left : $r('app.float.wh_value_10') })
                .padding($r('app.float.wh_value_5'))
                .fontSize($r('app.float.wh_value_18'))
                .width($r('app.float.wh_value_100p'))
              Scroll() {
                ContentTable({ apiItems : initGattServerManagerApiData() }) //子层调用
              }
              .width(ConfigData.WH_100_100)
              .align(Alignment.TopStart)
              .layoutWeight(1)
              .margin({ top : $r('sys.float.ohos_id_card_margin_middle') })
            }
            .height($r('app.float.wh_value_75p'))
          }
        }
      }
      .visibility(Visibility.Visible);
    }
    .width($r('app.float.wh_value_100p'))
    .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
  }

  private buildAdvSettings() {
    if ( this.cbxAdvSettings ) {
      if ( this.cbxInterval ) {
        this.curAdvSettings.interval = parseInt(this.advSettings_interval);
      }
      if ( this.cbxTxPower ) {
        this.curAdvSettings.txPower = parseInt(this.advSettings_txPower);
      }
      if ( this.cbxConnectable ) {
        this.curAdvSettings.connectable = this.advSettings_connectable;
      }
    }
  }

  private buildAdvData() {
    let serviceUuidInstance = this.advDataServiceUuids_serviceUuid;
    this.curAdvData.serviceUuids.length = 0;
    this.curAdvData.serviceUuids.push(serviceUuidInstance);

    let manufactureIdTmp = parseInt(this.advDataManufactureData_manufactureId);
    let manufactureValueTmp = this.advDataManufactureData_manufactureValue;
    let manufactureDataInstance: bluetoothManager.ManufactureData = {
      manufactureId : manufactureIdTmp ,
      manufactureValue : string2ArrayBuffer(manufactureValueTmp)
    };
    this.curAdvData.manufactureData.length = 0;
    this.curAdvData.manufactureData.push(manufactureDataInstance);

    let serviceUuid4Service = this.advDataServiceData_serviceUuid;
    let serviceValue4Service = this.advDataServiceData_serviceValue;
    let serviceDataInstance: bluetoothManager.ServiceData = {
      serviceUuid : serviceUuid4Service ,
      serviceValue : string2ArrayBuffer(serviceValue4Service)
    };
    this.curAdvData.serviceData.length = 0;
    this.curAdvData.serviceData.push(serviceDataInstance);
  }

  private buildAdvResponse() {
    if ( !this.cbxAdvResponse ) {
      return;
    }
    let serviceUuidInstance = this.advResponseServiceUuids_serviceUuid;
    this.curAdvResponse.serviceUuids.length = 0;
    this.curAdvResponse.serviceUuids.push(serviceUuidInstance);

    let manufactureIdTmp = parseInt(this.advResponseManufactureData_manufactureId);
    let manufactureValueTmp = this.advResponseManufactureData_manufactureValue;
    let manufactureDataInstance: bluetoothManager.ManufactureData = {
      manufactureId : manufactureIdTmp ,
      manufactureValue : string2ArrayBuffer(manufactureValueTmp)
    };
    this.curAdvResponse.manufactureData.length = 0;
    this.curAdvResponse.manufactureData.push(manufactureDataInstance);

    let serviceUuid4Service = this.advResponseServiceData_serviceUuid;
    let serviceValue4Service = this.advResponseServiceData_serviceValue;
    let serviceDataInstance: bluetoothManager.ServiceData = {
      serviceUuid : serviceUuid4Service ,
      serviceValue : string2ArrayBuffer(serviceValue4Service)
    };

    this.curAdvResponse.serviceData.length = 0;
    this.curAdvResponse.serviceData.push(serviceDataInstance);
  }

  private buildGattService() {
    // build descriptors
    let descriptors: Array<bluetooth.BLEDescriptor> = [];
    let descriptor: bluetooth.BLEDescriptor = {
      serviceUuid : this.curServiceUUID ,
      characteristicUuid : this.curCharacteristicUUID ,
      descriptorUuid : this.curDescriptorUUID ,
      descriptorValue : string2ArrayBuffer(this.curDescriptorValue)
    };
    descriptors.push(descriptor);

    // build characteristics
    let characteristics: Array<bluetooth.BLECharacteristic> = [];
    let cccValue = this.curCharacteristicValue;
    let characteristic: bluetooth.BLECharacteristic = {
      serviceUuid : this.curServiceUUID ,
      characteristicUuid : this.curCharacteristicUUID ,
      characteristicValue : string2ArrayBuffer(cccValue) ,
      descriptors : descriptors
    };

    characteristics.push(characteristic);
    this.curGattService.serviceUuid = this.curServiceUUID;
    this.curGattService.characteristics = characteristics;
    this.curGattService.isPrimary = this.curServiceIsPrimary;
    this.curGattService.includeServices = [];
  }

  private buildNotifyCharacteristic() {
    this.notifyCharacteristic_notifyCharacteristic = {
      serviceUuid : this.notifyCharacteristic_serviceUuid ,
      characteristicUuid : this.notifyCharacteristic_characteristicUuid ,
      characteristicValue : string2ArrayBuffer(this.notifyCharacteristic_characteristicValue) ,
      confirm : this.notifyCharacteristic_confirm
    }
  }

  private buildServerResponse() {
    this.serverResponse = {
      deviceId : this.serverResponse_deviceId ,
      transId : parseInt(this.serverResponse_transId) ,
      status : parseInt(this.serverResponse_status) ,
      offset : parseInt(this.serverResponse_offset) ,
      value : string2ArrayBuffer(this.serverResponse_value)
    }
  }
}

/**
 * string to ArrayBuffer
 * @param {*} str: the string to covert
 * @return: ArrayBuffer
 */
function string2ArrayBuffer(str: string) {
  let array = new Uint8Array(str.length);
  for ( let i = 0 ; i < str.length ; i ++ ) {
    array[ i ] = str.charCodeAt(i);
  }
  return array.buffer
}
