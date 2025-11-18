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
import router from '@ohos.router';
import { PageTitle } from '../../Component/pageTitle';
import { TestImageDisplay } from '../../Component/testImageDisplay';
import { TestData } from '../../MainAbility/model/testData'
import bluetooth from '@ohos.bluetooth';
import promptAction from '@ohos.promptAction';
import { opp } from '@kit.ConnectivityKit'
import { fileIo as fs} from '@kit.CoreFileKit';

/**
 * OppRecv Scenario Test Page Of Bluetooth test
 */

@Entry
@Component
struct OppRecvScenarioTest {
  private testItem: TestData = (router.getParams() as myParams).testItem
  @State changeIndex: number = - 1;
  @StorageLink("oppRecvScenarioMessage") oppRecvScenarioMessage: string = ""
  private oppProfile!: opp.OppServerProfile
  @State showProgress: boolean = false;
  @State curProgress: number = 0;
  @State serverMac: string = "00:00:00:00:00:00";
  private deviceName: string = "";
  private fileName: string = "";
  private pairedDevices: PairDevices[] = [];
  private receiveDialogController: CustomDialogController = new CustomDialogController({
    builder : ReceiveCustomDialog({
      deviceName : this.deviceName ,
      fileName : this.fileName ,
      confirm : () => {
        try {
          let pathDir = "/test.jpg"; // Replace the example path with the actual one.
          let file = fs.openSync(pathDir, fs.OpenMode.CREATE | fs.OpenMode.READ_WRITE);
          this.oppProfile.setIncomingFileConfirmation(true, file.fd);
          this.receiveDialogController.close();
          this.onTransferStateChange();
          console.log("receive success");
        } catch( error ) {
          console.log("receive fail, err: " + JSON.stringify(error));
        }
      } ,
      cancel : () => {
        try {
          let pathDir = "/test.jpg"; // Replace the example path with the actual one.
          let file = fs.openSync(pathDir, fs.OpenMode.CREATE | fs.OpenMode.READ_WRITE);
          this.oppProfile.setIncomingFileConfirmation(false, file.fd);
          this.receiveDialogController.close();
          console.log("reject receive success");
        } catch( error ) {
          console.log("reject receive fail, err: " + JSON.stringify(error));
        }
      }
    })
  })

  aboutToAppear() {
    try {
      this.oppProfile = opp.createOppServerProfile();
      testLog("oppRecv get opp profile success");
    } catch( error ) {
      testLog("oppRecv get opp profile fail, err: " + JSON.stringify(error));
    }
    this.getPairDevice();
  }

  getPairDevice() {
    let macs = bluetooth.getPairedDevices();
    console.log("OppRecv getPairDevice macs: " + JSON.stringify(macs));
    for ( let i = 0 ; i < macs.length ; i ++ ) {
      let name = bluetooth.getRemoteDeviceName(macs[ i ]);
      console.log("OppRecv getPairDevice name: " + JSON.stringify(name));
      this.pairedDevices.push({ id : macs[ i ] , name : name });
    }
    console.log("OppRecv getPairDevice pairedDevices: " + JSON.stringify(this.pairedDevices));
  }

  onTransferStateChange() {
    try {
      this.oppProfile.on("transferStateChange" , (stateInformation) => {
        console.log("transferStateChange enter");
        this.showProgress = true;
        this.curProgress = (stateInformation.currentBytes * 100) / stateInformation.totalBytes;
        if ( this.curProgress === 100 ) {
          this.showProgress = false;
          this.oppRecvScenarioMessage = "接收文件成功，文件存放路径：" + JSON.stringify(stateInformation.filePath) + "\n"
          testLog(this.oppRecvScenarioMessage);
        }
      })
    } catch( error ) {
      console.log("on transferStateChange fail, err: " + JSON.stringify(error));
    }
  }

  build() {
    Column() {
      Stack({ alignContent : Alignment.TopStart }) {
        TestImageDisplay({ testItem : this.testItem })
        PageTitle({ testItem : this.testItem })
      }

      Stack().height("1vp").backgroundColor("#000000");
      Column() {
        Scroll() {
          Column() {
            Text("已配对设备列表：(点击选择设备)")
              .fontSize("20vp")
              .fontWeight(FontWeight.Bold)
              .margin({ top : "5vp" })
            Scroll() {
              List({ space : 5 }) {
                ForEach(this.pairedDevices , (item: PairDevices) => {
                  ListItem() {
                    Text(item.name)
                      .fontSize("25vp")
                      .width("80%")
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
            .height("70%")
            .width("80%")
          }
          .height("40%")
          .width("100%")

        }
        .scrollBarWidth(10)
        .scrollBar(BarState.On)

        Stack().height("1vp").backgroundColor("#000000");

        Column() {
          Text("已选择的设备的蓝牙mac地址：")
            .fontSize("18vp")
            .fontWeight(FontWeight.Bold)
            .margin({ top : "3vp" })
          Text(this.serverMac)
            .fontSize("25vp")
            .width("70%")
            .borderRadius(5)
            .backgroundColor($r("app.color.moon"))
        }.height("20%")

        Scroll() {
          Column() {
            Text("流程日志:")
              .fontSize("18vp")
              .margin({ top : "8vp" , bottom : "8vp" , left : "20vp" })
              .textAlign(TextAlign.Start)
              .width("100%")
            List() {
              ListItem() {
                Text(this.oppRecvScenarioMessage)
                  .fontSize("18vp")
                  .margin({ top : "5vp" , left : "30vp" , right : "10vp" })
                  .textAlign(TextAlign.Start)
                  .width("90%")
              }
            }
            .height("80%")
            .width("90%")
            .backgroundColor($r("sys.color.ohos_id_color_foreground_contrary"))
          }
          .width("100%")
          .height("30%")
        }
        .scrollBarWidth(10)
        .scrollBar(BarState.On)
      }
      .height("66%")
      .width("100%")
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

      Stack().height("1vp").backgroundColor("#000000");

      Column() {
        Flex({ alignItems : ItemAlign.Center , justifyContent : FlexAlign.SpaceBetween }) {
          Column() {
            Button('opp开启接收文件')
              .fontSize("20vp")
              .borderRadius(8)
              .backgroundColor($r("app.color.blue"))
              .width(200)
              .height(60)
              .margin({ top : "15vp" })
              .align(Alignment.Center)
              .alignSelf(ItemAlign.Center)
              .onClick(() => {
                try {
                  this.oppProfile.on("receiveIncomingFile" , ( information) => {
                    testLog("opp开启接收文件")
                    this.deviceName = information.remoteDeviceName;
                    this.fileName = information.filePath;
                    this.oppRecvScenarioMessage = "发送端为：" + this.deviceName + ";文件名为：" + this.fileName + "\n"
                    testLog(this.oppRecvScenarioMessage);
                    console.log(JSON.stringify(information));
                    this.receiveDialogController.open();
                  })
                } catch( error ) {
                  this.oppRecvScenarioMessage = "opp开启接收文件失败，err: " + JSON.stringify(error)
                  testLog(this.oppRecvScenarioMessage);
                }
              })
              .width('60%')
          }
          .width("100%")
          .align(Alignment.Center)
          .alignItems(HorizontalAlign.Center)
          .backgroundColor($r("sys.color.ohos_id_color_sub_background"))

          if ( this.showProgress ) {
            Row({ space : 5 }) {
              Progress({ value : this.curProgress , total : 100 , type : ProgressType.Linear })
                .style({ strokeWidth : 10 })

              Text(this.curProgress + "%")
                .fontSize(30)
            }
          }
        }
        .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
        .width("100%")
      }
      .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
      .width("100%")
      .height("100%");
    }
  }
}

@CustomDialog
struct ReceiveCustomDialog {
  controller: CustomDialogController
  deviceName: string = 'defaultDeviceName'
  fileName: string = 'defaultFileName'
  cancel!: () => void
  confirm!: () => void

  build() {
    Column() {
      Text("是否接收来自 " + this.deviceName + " 的文件").fontSize("20vp").margin({ top : 10 , bottom : 10 })
      Text(this.fileName).fontSize("18vp").margin({ bottom : 10 })
      Flex({ justifyContent : FlexAlign.SpaceAround }) {
        Button('拒接')
          .onClick(() => {
            this.cancel()
          })
        Button('接收')
          .onClick(() => {
            this.confirm()
          })
      }.margin({ bottom : 10 })
    }
  }
}

export function testLog(logStr: string) {
  promptAction.showToast({ message : logStr })
  console.log(logStr)
}