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
            .fontSize($r('app.float.wh_value_17'))
            .fontColor($r('app.color.color_4444444_grey'))
            .width($r('app.float.wh_value_80p'))
            .padding($r('app.float.wh_value_5'))
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
            .width($r('app.float.wh_value_20'))
        }
        .height($r('app.float.wh_value_40'))
        .width($r('app.float.wh_value_100p'))
        .borderWidth($r('app.float.wh_value_1'))
        .padding($r('app.float.wh_value_5'))
        .borderColor($r("app.color.lead"))
        .backgroundColor( $r( "app.color.moon" ) )

        Column() {
          Column() {
            Column() {
              Row() {
                Text("mac").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_60'));
                TextInput({ text : this.txtScanFilter_deviceId , placeholder : "input deviceId" })
                  .fontSize($r('app.float.wh_value_15'))
                  .onChange((strInput) => {
                    this.txtScanFilter_deviceId = strInput;
                    AppStorage.setOrCreate('txtScanFilterDeviceId' , strInput);
                  })
                  .width($r('app.float.wh_value_85p'))
                  .borderRadius($r('app.float.wh_value_1'))
              }
              .padding($r('app.float.wh_value_5'))
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor( $r( "app.color.spring" ) )

              Row() {
                Text("name").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_60'));
                TextInput({ text : this.txtScanFilter_name , placeholder : "input name" })
                  .fontSize($r('app.float.wh_value_15'))
                  .onChange((strInput) => {
                    this.txtScanFilter_name = strInput;
                    AppStorage.setOrCreate('txtScanFilterName' , strInput);
                  })
                  .width($r('app.float.wh_value_85p'))
                  .borderRadius($r('app.float.wh_value_1'))
              }
              .padding($r('app.float.wh_value_5'))
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor( $r( "app.color.spring" ) )

              Row() {
                Text("serviceUuid").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_100'));
                TextInput({ text : this.txtScanFilter_serviceUuid , placeholder : "input serviceUuid" })
                  .fontSize($r('app.float.wh_value_15'))
                  .onChange((strInput) => {
                    this.txtScanFilter_serviceUuid = strInput;
                    AppStorage.setOrCreate('txtScanFilterServiceUuid' , strInput);
                  })
                  .width($r('app.float.wh_value_77p'))
                  .borderRadius($r('app.float.wh_value_1'))
              }
              .padding($r('app.float.wh_value_5'))
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor( $r( "app.color.spring" ) )
            }
            .height($r('app.float.wh_value_180'))
            .width($r('app.float.wh_value_100p'))
            .padding($r('app.float.wh_value_5'))
          }
          .borderColor($r("app.color.lead"))
          .borderWidth($r('app.float.wh_value_1'))

          Column() {
            Row() {
              Text("BLE扫描配置:")
                .padding($r('app.float.wh_value_5'))
                .fontSize($r('app.float.wh_value_17'))
                .fontColor($r('app.color.color_4444444_grey'))
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
                .width($r('app.float.wh_value_20'))
                .margin({ left : $r('app.float.wh_value_1') })
            }
            .height($r('app.float.wh_value_40'))
            .width($r('app.float.wh_value_100p'))
            .padding($r('app.float.wh_value_5'))
            .backgroundColor( $r( "app.color.moon" ) );

            Column() {
              Row() {
                Text("interval").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_80'))
                TextInput({ text : this.txtScanOptions_interval , placeholder : "input interval" })
                  .fontSize($r('app.float.wh_value_17'))
                  .onChange((strInput) => {
                    this.txtScanOptions_interval = strInput;
                    AppStorage.setOrCreate('txtScanOptionsInterval' , strInput);
                  })
                  .width($r('app.float.wh_value_80p'))
              }
              .padding($r('app.float.wh_value_5'))
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor( $r( "app.color.spring" ) )

              Row() {
                Text("dutyMode").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_80'))
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
                    .optionFont({ size : $r('app.float.wh_value_15') })
                    .width($r('app.float.wh_value_80p'))
                    .onSelect((index: number) => {
                      console.log("Select:" + index)
                      AppStorage.setOrCreate('txtScanOptionsDutyMode' , index);
                    })
                }
                .width($r('app.float.wh_value_80p'))
              }
              .padding($r('app.float.wh_value_5'))
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor( $r( "app.color.spring" ) )

              Row() {
                Text("matchMode").fontSize($r('app.float.wh_value_17')).width($r('app.float.wh_value_90'))
                Column() {
                  Select([
                    { value : 'MATCH_MODE_AGGRESSIVE' },
                    { value : 'MATCH_MODE_STICKY' }
                  ])
                    .value('MATCH_MODE_AGGRESSIVE')
                    .font({ size : 17 })
                    .selectedOptionFont({ size : 17 })
                    .optionFont({ size : $r('app.float.wh_value_15') })
                    .width($r('app.float.wh_value_75p'))
                    .onSelect((index: number) => {
                      console.log("Select:" + index)
                      AppStorage.setOrCreate('txtScanOptionsMatchMode' , index);
                    })
                }.width($r('app.float.wh_value_78p'))
              }
              .padding($r('app.float.wh_value_5'))
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor( $r( "app.color.spring" ) )
            }
            .width($r('app.float.wh_value_100p'))
            .padding($r('app.float.wh_value_5'))
          }
          .height($r('app.float.wh_value_240'))
          .borderWidth($r('app.float.wh_value_1'))
          .borderColor($r("app.color.lead"))
        }
      }
      .height($r('app.float.wh_value_100p'))
    }
    .scrollBarWidth($r('app.float.wh_value_20'))
    .padding({ bottom : $r('app.float.wh_value_10') })
  }
}