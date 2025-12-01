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

import { TestData , TestApi } from '../MainAbility/model/testData'
import { initBRApiData } from '../MainAbility/model/testDataModels'

/**
 * BleFilterTable Of Bluetooth test
 */

@Component
export struct BleFilterTable {
  private testItem!: TestData
  @State apiItems: TestApi[] = initBRApiData()
  @State changeIndex: number = - 1
  // input ble scan parameters:
  /*ScanFilter*/
  @State cbxBleScanFilter: boolean = false;
  @State txtScanFilter_deviceId: string = '6c:96:d7:3d:87:6f'; //6c:96:d7:3d:87:6f
  @State txtScanFilter_name: string = "dudu-tiger";
  @State txtScanFilter_serviceUuid: string = "0000180A-0000-1000-8000-00805f9b34fb";
  /*ScanOptions*/
  @State cbxBleScanOptions: boolean = false;
  @State txtScanOptions_interval: string = "0";

  getCurrentState(index: number) {
    return this.apiItems[ index ].result
  }

  aboutToAppear() {
    AppStorage.setOrCreate('txtScanFilterDeviceId' , this.txtScanFilter_deviceId);
    AppStorage.setOrCreate('txtScanFilterName' , this.txtScanFilter_name);
    AppStorage.setOrCreate('txtScanFilterServiceUuid' , this.txtScanFilter_serviceUuid);
    AppStorage.setOrCreate('txtScanOptions_interval' , this.txtScanOptions_interval);
    AppStorage.setOrCreate('txtScanOptionsDutyMode' , '0');
    AppStorage.setOrCreate('txtScanOptionsMatchMode' , '0');
    AppStorage.setOrCreate('cbxBleScanFilter' , this.cbxBleScanFilter);
    AppStorage.setOrCreate('cbxBleScanOptions' , this.cbxBleScanOptions);
  }

  build() {
    Scroll() {
      Column() {
        Row() {
          Text("BLE扫描过滤参数:")
            .fontSize("17vp")
            .fontColor("#4444444")
            .width($r('app.float.wh_value_80p'))
            .padding("5vp")
          Checkbox({ name : 'bleScanFilter' , group : 'checkboxGroup99' })
            .select(this.cbxBleScanFilter)
            .selectedColor(0x39a2db)
            .onChange((value) => {
              if ( value ) {
                this.cbxBleScanFilter = true;
              } else {
                this.cbxBleScanFilter = false;
              }
              AppStorage.setOrCreate('cbxBleScanFilter' , value);
            })
            .width(20)
        }
        .height(40)
        .width($r('app.float.wh_value_100p'))
        .borderWidth(1)
        .padding(5)
        .borderColor($r("app.color.lead"))
        .backgroundColor( $r( "app.color.moon" ) )

        Column() {
          Column() {
            Column() {
              Row() {
                Text("mac").fontSize("17vp").width(60);
                TextInput({ text : this.txtScanFilter_deviceId , placeholder : "input deviceId" })
                  .fontSize("15vp")
                  .onChange((strInput) => {
                    this.txtScanFilter_deviceId = strInput;
                    AppStorage.setOrCreate('txtScanFilterDeviceId' , strInput);
                  })
                  .width($r('app.float.wh_value_85p'))
                  .borderRadius(1)
              }
              .padding(5)
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor( $r( "app.color.spring" ) )

              Row() {
                Text("name").fontSize(17).width(60);
                TextInput({ text : this.txtScanFilter_name , placeholder : "input name" })
                  .fontSize("15vp")
                  .onChange((strInput) => {
                    this.txtScanFilter_name = strInput;
                    AppStorage.setOrCreate('txtScanFilterName' , strInput);
                  })
                  .width($r('app.float.wh_value_85p'))
                  .borderRadius(1)
              }
              .padding(5)
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor( $r( "app.color.spring" ) )

              Row() {
                Text("serviceUuid").fontSize(17).width(100);
                TextInput({ text : this.txtScanFilter_serviceUuid , placeholder : "input serviceUuid" })
                  .fontSize("15vp")
                  .onChange((strInput) => {
                    this.txtScanFilter_serviceUuid = strInput;
                    AppStorage.setOrCreate('txtScanFilterServiceUuid' , strInput);
                  })
                  .width("77%")
                  .borderRadius(1)
              }
              .padding(5)
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor( $r( "app.color.spring" ) )
            }
            .height(180)
            .width($r('app.float.wh_value_100p'))
            .padding(5)
          }
          .borderColor($r("app.color.lead"))
          .borderWidth(1)

          Column() {
            Row() {
              Text("BLE扫描配置:")
                .padding("5vp")
                .fontSize("17vp")
                .fontColor("#4444444")
                .width($r('app.float.wh_value_80p'))
              Checkbox({ name : 'bleScanOptions' , group : 'checkboxGroup95' })
                .select(this.cbxBleScanOptions)
                .selectedColor(0x39a2db)
                .onChange((value) => {
                  if ( value ) {
                    this.cbxBleScanOptions = true;
                    this.cbxBleScanFilter = true;
                    AppStorage.setOrCreate('cbxBleScanOptions' , this.cbxBleScanOptions);
                    AppStorage.setOrCreate('cbxBleScanFilter' , this.cbxBleScanFilter);
                  } else {
                    this.cbxBleScanOptions = false;
                    AppStorage.setOrCreate('cbxBleScanOptions' , this.cbxBleScanOptions);
                  }
                })
                .width(20)
                .margin({ left : "1vp" })
            }
            .height(40)
            .width($r('app.float.wh_value_100p'))
            .padding(5)
            .backgroundColor( $r( "app.color.moon" ) );

            Column() {
              Row() {
                Text("interval").fontSize(17).width(80)
                TextInput({ text : this.txtScanOptions_interval , placeholder : "input interval" })
                  .fontSize("17vp")
                  .onChange((strInput) => {
                    this.txtScanOptions_interval = strInput;
                    AppStorage.setOrCreate('txtScanOptionsInterval' , strInput);
                  })
                  .width($r('app.float.wh_value_80p'))
              }
              .padding(5)
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor( $r( "app.color.spring" ) )

              Row() {
                Text("dutyMode").fontSize(17).width(80)
                Column() {
                  Select([
                    { value : 'LOW_POWER' },
                    { value : 'BALANCED' },
                    { value : 'LOW_LATENCY' }
                  ])
                    .selected(2)
                    .value('LOW_POWER')
                    .font({ size : 17 })
                    .selectedOptionFont({ size : 17 })
                    .optionFont({ size : 15 })
                    .width($r('app.float.wh_value_80p'))
                    .onSelect((index: number) => {
                      console.log("Select:" + index)
                      AppStorage.setOrCreate('txtScanOptionsDutyMode' , index);
                    })
                }
                .width($r('app.float.wh_value_80p'))
              }
              .padding(5)
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor( $r( "app.color.spring" ) )

              Row() {
                Text("matchMode").fontSize(17).width(90)
                Column() {
                  Select([
                    { value : 'MATCH_MODE_AGGRESSIVE' },
                    { value : 'MATCH_MODE_STICKY' }
                  ])
                    .value('MATCH_MODE_AGGRESSIVE')
                    .font({ size : 17 })
                    .selectedOptionFont({ size : 17 })
                    .optionFont({ size : 15 })
                    .width($r('app.float.wh_value_75p'))
                    .onSelect((index: number) => {
                      console.log("Select:" + index)
                      AppStorage.setOrCreate('txtScanOptionsMatchMode' , index);
                    })
                }.width("78%")
              }
              .padding(5)
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor( $r( "app.color.spring" ) )
            }
            .width($r('app.float.wh_value_100p'))
            .padding(5)
          }
          .height(240)
          .borderWidth(1)
          .borderColor($r("app.color.lead"))
        }
      }
      .height($r('app.float.wh_value_100p'))
    }
    .scrollBarWidth(20)
    .padding({ bottom : 10 })
  }
}