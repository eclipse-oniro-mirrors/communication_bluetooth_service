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

import { myParams , PairDevices } from '../../Utils/common'
import promptAction from '@ohos.promptAction';
import router from '@ohos.router';
import { PageTitle } from '../../Component/pageTitle';
import { TestImageDisplay } from '../../Component/testImageDisplay';
import { TestData } from '../../MainAbility/model/testData'
import bluetooth from '@ohos.bluetooth';
import { opp } from '@kit.ConnectivityKit';

/**
 * OppSend Scenario Test Page Of Bluetooth test
 */

@Entry
@Component
struct oppSendScenarioTest {
  @State changeIndex: number = - 1;
  private testItem: TestData = (router.getParams() as myParams).testItem
  @StorageLink("oppSendScenarioMessage") oppSendScenarioMessage: string = ""
  private oppProfile!: opp.OppServerProfile;
  private pairDevices: PairDevices[] = []
  @State serverMac: string = "00:00:00:00:00:00";
  private fileName: string = "test.jpg";
  private fileType: string = "jpg"

  aboutToAppear() {
    try {
      this.oppProfile = opp.createOppServerProfile();
      testLog("OppSend get opp profile success");
    } catch( error ) {
      testLog("OppSend get opp profile fail, err: " + JSON.stringify(error));
    }
    this.getPairDevice();
  }

  getPairDevice() {
    let macs = bluetooth.getPairedDevices();
    console.log("OppSend getPairDevice macs: " + JSON.stringify(macs));
    for ( let i = 0 ; i < macs.length ; i ++ ) {
      let name = bluetooth.getRemoteDeviceName(macs[ i ]);
      console.log("OppSend getPairDevice name: " + JSON.stringify(name));
      this.pairDevices.push({ id : macs[ i ] , name : name });
    }
    console.log("OppSend getPairDevice pairDevices: " + JSON.stringify(this.pairDevices));
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
            Text("已配对设备列表：(点击选择设备)")
              .fontSize('20vp')
              .fontWeight(FontWeight.Bold)
              .margin({ top : '5vp' })
            Scroll() {
              List({ space : '5vp' }) {
                ForEach(this.pairDevices , (item: PairDevices) => {
                  ListItem() {
                    Text(item.name)
                      .fontSize('25vp')
                      .width('80%')
                      .margin({ left : '10%' })
                      .backgroundColor(this.serverMac === item.id ? $r("app.color.moon") : $r("app.color.white"))
                      .border({
                        radius : 5
                      })
                  }
                  .onClick(() => {
                    this.serverMac = item.id;
                  })
                })
              }
            }
            .height('80%')
            .width('80%')
          }
          .height('30%')
          .width('100%')
        }
        .scrollBarWidth('10vp')
        .scrollBar(BarState.Auto)

        Stack().height('1vp').backgroundColor($r('app.color.black'));

        Column() {
          Text("已选择的设备的蓝牙mac地址：")
            .fontSize('18vp')
            .fontWeight(FontWeight.Bold)
            .margin({ top : '3vp' })
          Text(this.serverMac)
            .fontSize('25vp')
            .width('70%')
            .borderRadius('5vp')
            .backgroundColor($r("app.color.moon"))
        }.height('15%')

        Column() {
          Text("请输入需要发送的文件名称，如：test.txt").fontSize('18vp').fontWeight(FontWeight.Bold)
          Text("注：文件必须在/data/bluetooth/log/目录下").fontSize('18vp').fontWeight(FontWeight.Bold)
          TextInput({ placeholder : "文件名称" })
            .fontSize('20vp')
            .onChange((str) => {
              if ( str ) {
                let strs = str.split(".");
                this.fileName = str;
                this.fileType = strs[ 1 ];
              }
              this.oppSendScenarioMessage = "需要发送的文件为：/data/bluetooth/log/" + str + "\n"
            }).width('70%').borderRadius('1vp')
        }.height('20%')

        Scroll() {
          Column() {
            Text("流程日志:")
              .fontSize('17vp')
              .margin({ top : '8vp' , bottom : '8vp' , left : '20vp' })
              .textAlign(TextAlign.Start)
              .width('100%')
            List() {
              ListItem() {
                Text(this.oppSendScenarioMessage)
                  .fontSize('18vp')
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
          .height('30%')
        }
        .scrollBarWidth('10vp')
        .scrollBar(BarState.On)
      }
      .height('66%')
      .width('100%')
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Stack().height('1vp').backgroundColor($r('app.color.black'));

      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Column() {
            Button('oppSendFile')
              .borderRadius('8vp')
              .backgroundColor($r("app.color.blue"))
              .width('200vp')
              .height('60vp')
              .fontSize('20vp')
              .margin({ top : '15vp' })
              .align(Alignment.Center)
              .alignSelf(ItemAlign.Center)
              .onClick(async(event: ClickEvent) => {
                try {
                  let fileHolders : Array<opp.FileHolder> = [];
                  this.oppProfile.sendFile(this.serverMac , fileHolders).then(() => {
                    let result: boolean = true;
                    if ( result ) {
                      this.oppSendScenarioMessage += "opp发送文件成功！ result: " + JSON.stringify(result)
                    } else {
                      this.oppSendScenarioMessage += "opp发送文件失败， result: " + JSON.stringify(result) +
                        "; 请核对文件名称或文件路径或者设备配对情况"
                    }
                  })
                } catch( error ) {
                  this.oppSendScenarioMessage += "opp发送文件失败，err :" + JSON.stringify(error)
                }
                testLog(this.oppSendScenarioMessage);
                AppStorage.setOrCreate("oppSendScenarioMessage" , this.oppSendScenarioMessage)
              })
          }
          .width('100%')
          .align(Alignment.Center)
          .alignItems(HorizontalAlign.Center)
          .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        }
        .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        .width('100%')
      }
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
      .width('100%')
      .height('100%');
    }
  }
}

export function testLog(logStr: string) {
  promptAction.showToast({ message : logStr })
  console.log(logStr)
}