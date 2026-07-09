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
import LogUtil from '../../../Utils/LogUtil';
import { resStr} from '../../../Utils/resString';


import { BTAutoArrayItem, testItemFromRouterOrPlaceholder} from '../../../Utils/common'
import ConfigData from '../../../Utils/ConfigData';
import { TestData} from '../../../MainAbility/model/testData'
import { PageTitle} from '../../../component/pageTitle';
import { TestImageDisplay} from '../../../component/testImageDisplay';
import { AutoContentTable} from '../../../component/autoContentTable';
import {
  GattServerManagerAutoTestCase,
  initGattServerManagerAutoData
} from '../../../MainAbility/model/autoTestDataModelsManager';

/**
 *  GattServerAutoTest Manager Page Of Bluetooth test
 */

@Entry
@Component
struct GattServerAutoTestManager {
  @State changeIndex: number = - 1
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @StorageLink("gattServerAutoTestMessage") gattServerAutoTestMessage: string = ""

  aboutToAppear() {
    AppStorage.setOrCreate('txtScanFilterDeviceId', "6c:96:d7:3d:87:6f");
    AppStorage.setOrCreate('txtScanFilterName', "dudu-tiger");
    AppStorage.setOrCreate('txtScanFilterServiceUuid', "0000180A-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('txtScanOptions_interval', "0");
    AppStorage.setOrCreate('txtScanOptionsDutyMode', '0');
    AppStorage.setOrCreate('txtScanOptionsMatchMode', '0');
    AppStorage.setOrCreate('cbxBleScanFilter', true);
    AppStorage.setOrCreate('cbxBleScanOptions', true);

    AppStorage.setOrCreate('curServiceUUID', '00001877-0000-1000-8000-00805F9B34FB');
    AppStorage.setOrCreate('curCharacteristicUUID', '00002BE0-0000-1000-8000-00805F9B34FB');
    AppStorage.setOrCreate('curDescriptorUUID', '00002902-0000-1000-8000-00805F9B34FB');
    AppStorage.setOrCreate('curDescriptorValue', 'Descriptor');
    AppStorage.setOrCreate('curCharacteristicValue', 'DaYuBlue');
    AppStorage.setOrCreate('isPrimary', true);
    AppStorage.setOrCreate('advSettings_interval', "1600");
    AppStorage.setOrCreate('advSettings_txPower', "-7");
    AppStorage.setOrCreate('advSettings_connectable', true);
    AppStorage.setOrCreate('advDataServiceUuids_serviceUuid',
      "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advDataManufactureData_manufactureId', "7456");
    AppStorage.setOrCreate('advDataManufactureData_manufactureValue', "CHN");
    AppStorage.setOrCreate('advDataServiceData_serviceUuid',
      "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advDataServiceData_serviceValue', "AdvValue");
    AppStorage.setOrCreate('advResponseServiceUuids_serviceUuid',
      "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advResponseManufactureData_manufactureId', "6547");
    AppStorage.setOrCreate('advResponseManufactureData_manufactureValue', "CHN");
    AppStorage.setOrCreate('advResponseServiceData_serviceUuid',
      "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advResponseServiceData_serviceValue', "AdvValue");
    AppStorage.setOrCreate('cbxAdvSettings', true);
    AppStorage.setOrCreate('cbxInterval', true);
    AppStorage.setOrCreate('cbxTxPower', true);
    AppStorage.setOrCreate('cbxAdvResponse', true);
  }

  build() {
    Column() {
      Stack({alignContent: Alignment.TopStart}) {
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
        .accessibilityLevel('yes')
        .id('pages_subAutoTestManager_subGattAutoTestManager_gattServerAutoTestManager__pt_0')
      }

      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column() {
        Scroll() {
          Column() {
            Text($r('app.string.autotest_api_verify_label'))
              .fontSize(ConfigData.VP_17)
              .margin({
                top: ConfigData.VP_10,
                bottom: ConfigData.VP_10,
                left: ConfigData.VP_15
              })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            AutoContentTable({
              autoItems: initGattServerManagerAutoData(),
              changeIndex: this.changeIndex
            })
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_280_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subAutoTestManager_subGattAutoTestManager_gattServerAutoTestManager__sc_0')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)

        Stack()
          .height(ConfigData.HEIGHT_1_VP)
          .backgroundColor($r('app.color.black'));

        Scroll() {
          Column() {
            Text($r('app.string.autotest_debug_log_label'))
              .fontSize(ConfigData.VP_17)
              .margin({
                top: ConfigData.VP_8,
                bottom: ConfigData.VP_8,
                left: ConfigData.VP_15
              })
              .textAlign(TextAlign.Start)
              .width(ConfigData.WH_100_100)
            List() {
              ListItem() {
                Text(resStr($r('app.string.autotest_log_prefix')) + "\n" +
                  this.gattServerAutoTestMessage)
                  .fontSize(ConfigData.VP_17)
                  .margin({
                    top: ConfigData.VP_5,
                    left: ConfigData.VP_30,
                    right: ConfigData.VP_10
                  })
                  .textAlign(TextAlign.Start)
                  .width(ConfigData.WH_90_100)
              }
            .accessibilityLevel('yes')
            .id('pages_subAutoTestManager_subGattAutoTestManager_gattServerAutoTestManager__li_0')
            }
            .height(ConfigData.WH_80_100)
            .width(ConfigData.WH_90_100)
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width(ConfigData.WH_100_100)
          .height(ConfigData.HEIGHT_260_VP)
        }
        .accessibilityLevel('yes')
        .id('pages_subAutoTestManager_subGattAutoTestManager_gattServerAutoTestManager__sc_1')
        .scrollBarWidth(ConfigData.VP_10)
        .scrollBar(BarState.Auto)
      }
      .height(ConfigData.HEIGHT_500_VP)
      .width(ConfigData.WH_100_100)
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))


      Stack()
        .height(ConfigData.HEIGHT_1_VP)
        .backgroundColor($r('app.color.black'));
      Column() {
        Button({
          type: ButtonType.Normal,
          stateEffect: true
        }) {
          Row() {
            Text($r('app.string.autotest'))
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
        .id('pages_subAutoTestManager_subGattAutoTestManager_gattServerAutoTestManager__btn_0')
        .borderRadius(ConfigData.VP_8)
        .backgroundColor($r("app.color.blue"))
        .width(ConfigData.VP_180)
        .height(ConfigData.VP_50)
        .margin({top: ConfigData.VP_15})
        .onClick(async(event: ClickEvent) => {
          this.forLoop()
        })
      }
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
      .width(ConfigData.WH_100_100)
      .height(ConfigData.WH_100_100);
    }
  }

  //循环调用
  async forLoop() {
    let gattServerAutoTestMessage = ""
    //拼装数组
    let gattServerAutoArray: Array<BTAutoArrayItem> = [
      {
        name: "testSetLocalNameManager",
        para: ["DaYuBlue"]
      },
      {
        name: "testSetBluetoothScanModeManager",
        para: [4, 0]
      }
    ]
    let interfaces = GattServerManagerAutoTestCase.map(i => {
      let item = gattServerAutoArray.find(itm => itm.name === i.api.name);
      let para = item ? item.para: [];// 确保 para 总是一个数组
      return { name: i.api.name, para} as BTAutoArrayItem;
    });
    for(let m = 0 ; m<interfaces.length ; m++) {
      // 检查 GattServerManagerAutoTestCase 中对应的 api 是否是一个函数
      if (typeof GattServerManagerAutoTestCase[m].api === "function") {
        // 使用 TypeScript 的类型断言确保 para 是一个数组
        gattServerAutoTestMessage += GattServerManagerAutoTestCase[m].api(... interfaces[m
          ].para)
          + "\n";
      } else {
        LogUtil.error(`在 BrAutoTestCase 中找不到方法: ${GattServerManagerAutoTestCase[m] }`);
      }
      this.changeIndex = m
      await sleep(1)
      AppStorage.setOrCreate("gattServerAutoTestMessage", gattServerAutoTestMessage)
    }
  }
}

async function sleep(time: number): Promise<void> {
  return new Promise<void>((resolve, reject) => {
    setTimeout(() => {
      resolve();
    }, time * 1000);
  });
}