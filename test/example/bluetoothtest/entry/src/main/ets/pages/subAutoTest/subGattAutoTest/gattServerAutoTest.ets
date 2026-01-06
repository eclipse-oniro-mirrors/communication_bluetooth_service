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

import { BTAutoArrayItem , myParams } from '../../../Utils/common'
import ConfigData from '../../../Utils/ConfigData';
import router from '@ohos.router';
import { TestData } from '../../../MainAbility/model/testData'
import { PageTitle } from '../../../Component/pageTitle';
import { TestImageDisplay } from '../../../Component/testImageDisplay';
import { AutoContentTable } from '../../../Component/autoContentTable';
import { GattServerAutoTestCase , initGattServerAutoData } from '../../../MainAbility/model/autoTestDataModels';

/**
 *  GattServerAutoTest Page Of Bluetooth test
 */

@Entry
@Component
struct GattServerAutoTest {
  @State changeIndex: number = - 1
  private testItem: TestData = (router.getParams() as myParams).testItem
  @StorageLink("gattServerAutoTestMessage") gattServerAutoTestMessage: string = ""

  aboutToAppear() {
    AppStorage.setOrCreate('txtScanFilterDeviceId' , "6c:96:d7:3d:87:6f");
    AppStorage.setOrCreate('txtScanFilterName' , "dudu-tiger");
    AppStorage.setOrCreate('txtScanFilterServiceUuid' , "0000180A-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('txtScanOptions_interval' , "0");
    AppStorage.setOrCreate('txtScanOptionsDutyMode' , '0');
    AppStorage.setOrCreate('txtScanOptionsMatchMode' , '0');
    AppStorage.setOrCreate('cbxBleScanFilter' , true);
    AppStorage.setOrCreate('cbxBleScanOptions' , true);

    AppStorage.setOrCreate('curServiceUUID' , '00001877-0000-1000-8000-00805F9B34FB');
    AppStorage.setOrCreate('curCharacteristicUUID' , '00002BE0-0000-1000-8000-00805F9B34FB');
    AppStorage.setOrCreate('curDescriptorUUID' , '00002902-0000-1000-8000-00805F9B34FB');
    AppStorage.setOrCreate('curDescriptorValue' , 'Descriptor');
    AppStorage.setOrCreate('curCharacteristicValue' , 'DaYuBlue');
    AppStorage.setOrCreate('isPrimary' , true);
    AppStorage.setOrCreate('advSettings_interval' , "1600");
    AppStorage.setOrCreate('advSettings_txPower' , "-7");
    AppStorage.setOrCreate('advSettings_connectable' , true);
    AppStorage.setOrCreate('advDataServiceUuids_serviceUuid' , "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advDataManufactureData_manufactureId' , "7456");
    AppStorage.setOrCreate('advDataManufactureData_manufactureValue' , "CHN");
    AppStorage.setOrCreate('advDataServiceData_serviceUuid' , "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advDataServiceData_serviceValue' , "AdvValue");
    AppStorage.setOrCreate('advResponseServiceUuids_serviceUuid' , "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advResponseManufactureData_manufactureId' , "6547");
    AppStorage.setOrCreate('advResponseManufactureData_manufactureValue' , "CHN");
    AppStorage.setOrCreate('advResponseServiceData_serviceUuid' , "000054F6-0000-1000-8000-00805f9b34fb");
    AppStorage.setOrCreate('advResponseServiceData_serviceValue' , "AdvValue");
    AppStorage.setOrCreate('cbxAdvSettings' , true);
    AppStorage.setOrCreate('cbxInterval' , true);
    AppStorage.setOrCreate('cbxTxPower' , true);
    AppStorage.setOrCreate('cbxAdvResponse' , true);
  }

  build() {
    Column() {
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
      }

      Stack().height('1vp').backgroundColor($r('app.color.black'));
      Column() {
        Scroll() {
          Column() {
            Text("API验证:")
              .fontSize('17vp')
              .margin({ top : '10vp' , bottom : '10vp' , left : '15vp' })
              .textAlign(TextAlign.Start)
              .width('100%')
            AutoContentTable({
              autoItems : initGattServerAutoData() ,
              changeIndex : this.changeIndex
            })
          }
          .width('100%')
          .height('280vp')
        }
        .scrollBarWidth('10vp')
        .scrollBar(BarState.Auto)

        Stack().height('1vp').backgroundColor($r('app.color.black'));

        Scroll() {
          Column() {
            Text("Debug日志:")
              .fontSize('17vp')
              .margin({ top : '8vp' , bottom : '8vp' , left : '15vp' })
              .textAlign(TextAlign.Start)
              .width('100%')
            List() {
              ListItem() {
                Text("log:" + "\n" + this.gattServerAutoTestMessage)
                  .fontSize('17vp')
                  .margin({ top : '5vp' , left : '30vp' , right : '10vp' })
                  .textAlign(TextAlign.Start)
                  .width('90%')
              }
            }
            .height('80%')
            .width('90%')
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width('100%')
          .height('260vp')
        }
        .scrollBarWidth('10vp')
        .scrollBar(BarState.Auto)
      }
      .height('500vp')
      .width('100%')
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Stack().height('1vp').backgroundColor($r('app.color.black'));
      Column() {
        Button({ type : ButtonType.Normal , stateEffect : true }) {
          Row() {
            Text('自动测试').fontSize('24vp').fontColor(0xffffff).margin({ left : '5vp' , right : '5vp' })
          }.alignItems(VerticalAlign.Center)
        }
        .borderRadius('8vp')
        .backgroundColor($r("app.color.blue"))
        .width('180vp')
        .height('50vp')
        .margin({ top : '15vp' })
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
      { name : "testSetLocalName" , para : [ "DaYuBlue" ] },
      { name : "testSetBluetoothScanMode" , para : [ 4, 0 ] }
    ]
    let interfaces = GattServerAutoTestCase.map(i => {
      let item = gattServerAutoArray.find(itm => itm.name === i.api.name);
      let para = item ? item.para : []; // 确保 para 总是一个数组
      return { name : i.api.name , para } as BTAutoArrayItem;
    });

    for ( let m = 0 ; m < interfaces.length ; m ++ ) {
      // 检查 GattServerAutoTestCase 中对应的 api 是否是一个函数
      if ( typeof GattServerAutoTestCase[ m ].api === "function" ) {
        // 使用 TypeScript 的类型断言确保 para 是一个数组
        gattServerAutoTestMessage += GattServerAutoTestCase[ m ].api(... interfaces[ m ].para) + "\n";
      } else {
        console.error(`在 BrAutoTestCase 中找不到方法: ${ GattServerAutoTestCase[ m ] }`);
      }
      this.changeIndex = m
      await sleep(1)
      AppStorage.setOrCreate("gattServerAutoTestMessage" , gattServerAutoTestMessage)
    }
  }
}

async function sleep(time: number): Promise<void> {
  return new Promise<void>((resolve , reject) => {
    setTimeout(() => {
      resolve();
    } , time * 1000);
  });
}