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

import LogUtil from '../Utils/LogUtil';
import ConfigData from '../Utils/ConfigData';
import { TestData, TestApi} from '../MainAbility/model/testData'
import { initBRApiData} from '../MainAbility/model/testDataModels'

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
  @State txtScanFilter_deviceId: string = '6c:96:d7:3d:87:6f';//6c:96:d7:3d:87:6f
  @State txtScanFilter_name: string = "dudu-tiger";
  @State txtScanFilter_serviceUuid: string = "0000180A-0000-1000-8000-00805f9b34fb";
  /*ScanOptions*/
  @State cbxBleScanOptions: boolean = false;
  @State txtScanOptions_interval: string = "0";

  getCurrentState(index: number) {
    return this.apiItems[index].result
  }

  aboutToAppear() {
    AppStorage.setOrCreate('txtScanFilterDeviceId', this.txtScanFilter_deviceId);
    AppStorage.setOrCreate('txtScanFilterName', this.txtScanFilter_name);
    AppStorage.setOrCreate('txtScanFilterServiceUuid', this.txtScanFilter_serviceUuid);
    AppStorage.setOrCreate('txtScanOptions_interval', this.txtScanOptions_interval);
    AppStorage.setOrCreate('txtScanOptionsDutyMode', '0');
    AppStorage.setOrCreate('txtScanOptionsMatchMode', '0');
    AppStorage.setOrCreate('cbxBleScanFilter', this.cbxBleScanFilter);
    AppStorage.setOrCreate('cbxBleScanOptions', this.cbxBleScanOptions);
  }

  build() {
    Scroll() {
      Column() {
        Row() {
          Text($r('app.string.bluetoothtest_text_bsmglcs'))
            .fontSize(ConfigData.VP_17)
            .fontColor($r('app.color.color_4444444_grey'))
            .width(ConfigData.WH_80_100)
            .padding(ConfigData.VP_5)
          Checkbox({
            name: 'bleScanFilter',
            group: 'checkboxGroup99'
          })
            .select(this.cbxBleScanFilter)
            .selectedColor(0x39a2db)
            .onChange((value) => {
              if (value) {
                this.cbxBleScanFilter = true;
              } else {
                this.cbxBleScanFilter = false;
              }
              AppStorage.setOrCreate('cbxBleScanFilter', value);
            })
            .width(ConfigData.VP_20)
        }
        .height(ConfigData.VP_40)
        .width(ConfigData.WH_100_100)
        .borderWidth(ConfigData.HEIGHT_1_VP)
        .padding(ConfigData.VP_5)
        .borderColor($r("app.color.lead"))
        .backgroundColor($r("app.color.moon"))

        Column() {
          Column() {
            Column() {
              Row() {
                Text($r('app.string.bluetoothtest_text_m'))
                  .fontSize(ConfigData.VP_17)
                  .width(ConfigData.VP_60);
                TextInput({
                  text: this.txtScanFilter_deviceId,
                  placeholder: "input deviceId"
                })
                  .fontSize(ConfigData.VP_15)
                  .onChange((strInput) => {
                    this.txtScanFilter_deviceId = strInput;
                    AppStorage.setOrCreate('txtScanFilterDeviceId', strInput);
                  })
                  .width(ConfigData.PCT_85)
                  .borderRadius(ConfigData.HEIGHT_1_VP)
              }
              .padding(ConfigData.VP_5)
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor($r("app.color.spring"))

              Row() {
                Text($r('app.string.bluetoothtest_text_n'))
                  .fontSize(ConfigData.VP_17)
                  .width(ConfigData.VP_60);
                TextInput({
                  text: this.txtScanFilter_name,
                  placeholder: "input name"
                })
                  .fontSize(ConfigData.VP_15)
                  .onChange((strInput) => {
                    this.txtScanFilter_name = strInput;
                    AppStorage.setOrCreate('txtScanFilterName', strInput);
                  })
                  .width(ConfigData.PCT_85)
                  .borderRadius(ConfigData.HEIGHT_1_VP)
              }
              .padding(ConfigData.VP_5)
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor($r("app.color.spring"))

              Row() {
                Text($r('app.string.bluetoothtest_text_s_1'))
                  .fontSize(ConfigData.VP_17)
                  .width(ConfigData.VP_100);
                TextInput({
                  text: this.txtScanFilter_serviceUuid,
                  placeholder: "input serviceUuid"
                })
                  .fontSize(ConfigData.VP_15)
                  .onChange((strInput) => {
                    this.txtScanFilter_serviceUuid = strInput;
                    AppStorage.setOrCreate('txtScanFilterServiceUuid', strInput);
                  })
                  .width(ConfigData.PCT_77)
                  .borderRadius(ConfigData.HEIGHT_1_VP)
              }
              .padding(ConfigData.VP_5)
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor($r("app.color.spring"))
            }
            .height(ConfigData.VP_180)
            .width(ConfigData.WH_100_100)
            .padding(ConfigData.VP_5)
          }
          .borderColor($r("app.color.lead"))
          .borderWidth(ConfigData.HEIGHT_1_VP)

          Column() {
            Row() {
              Text($r('app.string.bluetoothtest_text_bsmpz'))
                .padding(ConfigData.VP_5)
                .fontSize(ConfigData.VP_17)
                .fontColor($r('app.color.color_4444444_grey'))
                .width(ConfigData.WH_80_100)
              Checkbox({
                name: 'bleScanOptions',
                group: 'checkboxGroup95'
              })
                .select(this.cbxBleScanOptions)
                .selectedColor(0x39a2db)
                .onChange((value) => {
                  if (value) {
                    this.cbxBleScanOptions = true;
                    this.cbxBleScanFilter = true;
                    AppStorage.setOrCreate('cbxBleScanOptions', this.cbxBleScanOptions);
                    AppStorage.setOrCreate('cbxBleScanFilter', this.cbxBleScanFilter);
                  } else {
                    this.cbxBleScanOptions = false;
                    AppStorage.setOrCreate('cbxBleScanOptions', this.cbxBleScanOptions);
                  }
                })
                .width(ConfigData.VP_20)
                .margin({left: ConfigData.HEIGHT_1_VP})
            }
            .height(ConfigData.VP_40)
            .width(ConfigData.WH_100_100)
            .padding(ConfigData.VP_5)
            .backgroundColor($r("app.color.moon"));

            Column() {
              Row() {
                Text($r('app.string.bluetoothtest_text_i_1'))
                  .fontSize(ConfigData.VP_17)
                  .width(ConfigData.VP_80)
                TextInput({
                  text: this.txtScanOptions_interval,
                  placeholder: "input interval"
                })
                  .fontSize(ConfigData.VP_17)
                  .onChange((strInput) => {
                    this.txtScanOptions_interval = strInput;
                    AppStorage.setOrCreate('txtScanOptionsInterval', strInput);
                  })
                  .width(ConfigData.WH_80_100)
              }
              .padding(ConfigData.VP_5)
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor($r("app.color.spring"))

              Row() {
                Text($r('app.string.bluetoothtest_text_d_1'))
                  .fontSize(ConfigData.VP_17)
                  .width(ConfigData.VP_80)
                Column() {
                  Select([
                    { value: 'LOW_POWER'},
                    { value: 'BALANCED'},
                    { value: 'LOW_LATENCY'}
                  ])
                    .selected(2)
                    .value('LOW_POWER')
                    .font({size: ConfigData.font_17})
                    .selectedOptionFont({size: ConfigData.font_17})
                    .optionFont({size: ConfigData.VP_15})
                    .width(ConfigData.WH_80_100)
                    .onSelect((index: number) => {
                      LogUtil.info("Select:" + index)
                      AppStorage.setOrCreate('txtScanOptionsDutyMode', index)
                    })
                }
                .width(ConfigData.WH_80_100)
              }
              .padding(ConfigData.VP_5)
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor($r("app.color.spring"))

              Row() {
                Text($r('app.string.bluetoothtest_text_m_1'))
                  .fontSize(ConfigData.VP_17)
                  .width(ConfigData.VP_90)
                Column() {
                  Select([
                    { value: 'MATCH_MODE_AGGRESSIVE'},
                    { value: 'MATCH_MODE_STICKY'}
                  ])
                    .value('MATCH_MODE_AGGRESSIVE')
                    .font({size: ConfigData.font_17})
                    .selectedOptionFont({size: ConfigData.font_17})
                    .optionFont({size: ConfigData.VP_15})
                    .width(ConfigData.PCT_75)
                    .onSelect((index: number) => {
                      LogUtil.info("Select:" + index)
                      AppStorage.setOrCreate('txtScanOptionsMatchMode', index)
                    })
                }
                  .width(ConfigData.PCT_78)
              }
              .padding(ConfigData.VP_5)
              .justifyContent(FlexAlign.Start)
              .alignItems(VerticalAlign.Center)
              .backgroundColor($r("app.color.spring"))
            }
            .width(ConfigData.WH_100_100)
            .padding(ConfigData.VP_5)
          }
          .height(ConfigData.VP_240)
          .borderWidth(ConfigData.HEIGHT_1_VP)
          .borderColor($r("app.color.lead"))
        }
      }
      .height(ConfigData.WH_100_100)
    }
    .scrollBarWidth(ConfigData.VP_20)
    .padding({bottom: ConfigData.VP_10})
  }
}