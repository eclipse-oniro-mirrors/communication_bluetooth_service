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
import { resStr} from '../Utils/resString';


import ConfigData from '../Utils/ConfigData';
import { CaseCategory, TestApi, TestData} from '../MainAbility/model/testData'
import promptAction from '@ohos.promptAction';
import { ProfileList, ScanMode} from '../Utils/common'

/**
 *  ContentTable Component Page Of Bluetooth test
 */

@CustomDialog
export struct GetProfileConnSateDialog {
  @StorageLink('pairedMac') pairedMac: string = "";
  private hitText: Resource | string = 'defaultHitText'
  private getProfileList: ProfileList[] = [
    { value: 'A2DP_SOURCE'},
    { value: 'HANDS_FREE_AUDIO_GATEWAY'},
    { value: 'HID_HOST'}
  ]
  @State currentClick: number = 1;
  @Link textValue: string
  @Link inputValue: string
  @Link apiItem: TestApi
  @Link currentIndex: number
  @Link changeIndex: number
  controller: CustomDialogController
  cancel!: () => void
  confirm!: (value: string) => void

  build(){
    Column(){
      Text(this.hitText)
        .fontSize(ConfigData.VP_20)
        .margin({top: ConfigData.VP_10, bottom: ConfigData.VP_10})
      Select(this.getProfileList)
        .selected(this.currentClick)
        .value(this.getProfileList[ this.currentClick ].value)
        .font({size: ConfigData.VP_20})
        .selectedOptionFont({size: ConfigData.VP_20})
        .optionFont({size: 17})
        .height(ConfigData.VP_80)
        .width(ConfigData.WH_90_100)
        .onSelect((index: number) => {
          LogUtil.info("Select:" + index)
          this.currentClick = index
        })

      Flex({justifyContent: FlexAlign.SpaceAround}){
        Button($r('app.string.bluetoothtest_str_qx'))
          .onClick(() => {
            this.controller.close()
            this.cancel()
          })
          .backgroundColor(0xffffff).fontColor(Color.Black)
        Button($r('app.string.bluetoothtest_str_qr'))
          .onClick(() => {
            this.inputValue = this.currentClick.toString()
            this.changeIndex = this.currentIndex
            //TODO: 设备调试时候需要打开
            if ( true ){
              let message = ""
              message = this.apiItem.api!(this.currentClick, this.pairedMac)
              this.apiItem.result = this.getProfileList[ this.currentClick ].value + resStr($r('app.string.bluetoothtest_str_zt')) + message
              LogUtil.info("this.apiItem.result:" + this.apiItem.result)
            } else {
              let message = ""
              this.apiItem.result = this.getProfileList[ this.currentClick ].value
              LogUtil.info("this.apiItem.result:" + this.apiItem.result)
            }
            this.controller.close()
            this.confirm(this.inputValue)
          })
          .backgroundColor(0xffffff).fontColor(Color.Red)
      }
      .margin({bottom: ConfigData.VP_10})
    }
  }
}

@CustomDialog
export struct SetScanModeDialog {
  private hitText: Resource | string = 'defaultHitText'
  private scanModeList: ProfileList[] = [
    { value: 'SCAN_MODE_NONE'},
    { value: 'CONNECTABLE'},
    { value: 'GENERAL_DISCOVERABLE'},
    { value: 'LIMITED_DISCOVERABLE'},
    { value: 'CONNECTABLE_GENERAL_DISCOVERABLE'},
    { value: 'CONNECTABLE_LIMITED_DISCOVERABLE'}
  ]
  @State currentClick: number = 4;
  @Link textValue: string
  @Link inputValue: string
  @Link apiItem: TestApi
  @Link currentIndex: number
  @Link changeIndex: number
  @State duration: number = 0
  controller: CustomDialogController
  cancel!: () => void
  confirm!: (value: string) => void

  build(){
    Column(){
      Text(this.hitText)
        .fontSize(ConfigData.VP_20)
        .margin({top: ConfigData.VP_10, bottom: ConfigData.VP_10})
      Select(this.scanModeList)
        .selected(this.currentClick)
        .value(this.scanModeList[ this.currentClick ].value)
        .font({size: ConfigData.VP_12})
        .selectedOptionFont({size: ConfigData.VP_12})
        .optionFont({size: ConfigData.VP_12})
        .height(ConfigData.VP_60)
        .width(ConfigData.WH_90_100)
        .onSelect((index: number) => {
          LogUtil.info("Select:" + index)
          this.currentClick = index
        })
      TextInput({
        placeholder: $r('app.string.bluetoothtest_str_qsrycsj'),
        text: this.duration.toString(),
      }).type(InputType.Number)
        .height(ConfigData.VP_60).width(ConfigData.WH_90_100)
        .onChange((value: string) => {
          if ( value == "" ){
            this.duration = 0
          } else {
            this.duration = parseInt(value)
          }
        })
      Flex({justifyContent: FlexAlign.SpaceAround}){
        Button($r('app.string.bluetoothtest_str_qx'))
          .onClick(() => {
            this.controller.close()
            this.cancel()
          })
          .backgroundColor(0xffffff).fontColor(Color.Black)
        Button($r('app.string.bluetoothtest_str_qr'))
          .onClick(() => {
            this.inputValue = this.currentClick.toString()
            this.changeIndex = this.currentIndex
            //TODO: 设备调试时候需要打开
            if ( true ){
              let message: string = this.apiItem.api!(this.currentClick, this.duration)
              LogUtil.info(resStr($r('app.string.bluetoothtest_str_lx')) +typeof message)
              // LogUtil.info("类型:" + this.scanModeList[ message.mode ].value)
              LogUtil.info(resStr($r('app.string.bluetoothtest_str_yc')) + message)
              // this.apiItem.result = "类型:" + this.scanModeList[ message.mode ].value + ";延迟:" + message.duration.toString()
              this.apiItem.result = resStr($r('app.string.bluetoothtest_str_lxhyc')) + message
              LogUtil.info("this.apiItem.result:" + this.apiItem.result)
            } else {
              this.apiItem.result = this.scanModeList[ this.currentClick ].value
              LogUtil.info("this.apiItem.result:" + this.apiItem.result)
            }
            this.controller.close()
            this.confirm(this.inputValue)
          })
          .backgroundColor(0xffffff)
          .fontColor(Color.Red)
      }
      .margin({bottom: ConfigData.VP_10})
    }
  }
}

@CustomDialog
export struct InputDialog {
  private hitText: Resource | string = 'defaultHitText'
  @Link textValue: string
  @Link inputValue: string
  @Link apiItem: TestApi
  @Link currentIndex: number
  @Link changeIndex: number
  controller: CustomDialogController
  cancel!: () => void
  confirm!: (value: string) => void

  build(){
    Column(){
      Text(this.hitText)
        .fontSize(ConfigData.VP_20)
        .margin({top: ConfigData.VP_10, bottom: ConfigData.VP_10})
      TextInput({placeholder: '', text: this.textValue})
        .height(ConfigData.VP_60)
        .width(ConfigData.WH_90_100)
        .onChange((value: string) => {
          this.textValue = value
        })

      Flex({justifyContent: FlexAlign.SpaceAround}){
        Button($r('app.string.bluetoothtest_str_qx'))
          .onClick(() => {
            this.controller.close()
            this.cancel()
          })
          .backgroundColor(0xffffff).fontColor(Color.Black)
        Button($r('app.string.bluetoothtest_str_qr'))
          .onClick(() => {
            this.inputValue = this.textValue
            this.changeIndex = this.currentIndex
            //TODO: 设备调试时候需要打开
            let message = ""
            if ( true ){
              message = this.apiItem.api!(this.textValue)
              LogUtil.info("message:" + message)
              this.apiItem.result = message
            } else {
              this.apiItem.result = this.textValue
            }
            this.controller.close()
            this.confirm(this.inputValue)
          })
          .backgroundColor(0xffffff)
          .fontColor(Color.Red)
      }
      .margin({bottom: ConfigData.VP_10})
    }
  }
}

@CustomDialog
export struct SetConnectionStrategyDialog {
  private hitText: Resource | string = 'defaultHitText'
  private connectionStrategyList: ProfileList[] = [
    { value: 'CONNECTION_STRATEGY_UNSUPPORTED'},
    { value: 'CONNECTION_STRATEGY_ALLOWED'},
    { value: 'CONNECTION_STRATEGY_FORBIDDEN'},
  ]
  @State currentClick: number = 1;
  @Link textValue: string
  @Link inputValue: string
  @Link apiItem: TestApi
  @Link currentIndex: number
  @Link changeIndex: number
  controller: CustomDialogController
  cancel!: () => void
  confirm!: (value: string) => void

  build(){
    Column(){
      Text(this.hitText)
        .fontSize(ConfigData.VP_20)
        .margin({top: ConfigData.VP_10, bottom: ConfigData.VP_10})
      Select(this.connectionStrategyList)
        .selected(this.currentClick)
        .value(this.connectionStrategyList[ this.currentClick ].value)
        .font({size: ConfigData.VP_18})
        .selectedOptionFont({size: ConfigData.VP_18})
        .optionFont({size: 17})
        .height(ConfigData.VP_80)
        .width(ConfigData.WH_90_100)
        .onSelect((index: number) => {
          LogUtil.info("Select:" + index)
          this.currentClick = index
        })

      Flex({justifyContent: FlexAlign.SpaceAround}){
        Button($r('app.string.bluetoothtest_str_qx'))
          .onClick(() => {
            this.controller.close()
            this.cancel()
          })
          .backgroundColor(0xffffff).fontColor(Color.Black)
        Button($r('app.string.bluetoothtest_str_qr'))
          .onClick(() => {
            this.inputValue = this.currentClick.toString()
            this.changeIndex = this.currentIndex
            //TODO: 设备调试时候需要打开
            if ( true ){
              let message = ""
              message = this.apiItem.api!(this.currentClick)
              this.apiItem.result = this.connectionStrategyList[ this.currentClick ].value
              LogUtil.info("this.apiItem.result:" + this.apiItem.result)
            } else {
              let message = ""
              this.apiItem.result = this.connectionStrategyList[ this.currentClick ].value
              LogUtil.info("this.apiItem.result:" + this.apiItem.result)
            }
            this.controller.close()
            this.confirm(this.inputValue)
          })
          .backgroundColor(0xffffff).fontColor(Color.Red)
      }
      .margin({bottom: ConfigData.VP_10})
    }
  }
}

/**
 * ContentTable
 */
@Component
export struct ContentTable {
  @State currentApiItem: TestApi = new TestApi('', '', '', (): void => {}, '', CaseCategory.BR)
  @State apiItems: TestApi[] = [];
  @State changeIndex: number = - 1
  @State localName: string = 'DaYuBlue1'
  @State currentIndex: number = 1
  @StorageLink('pairedMac') pairedMac: string = "";
  getProfileConnSateDialog: CustomDialogController = new CustomDialogController({
    builder: GetProfileConnSateDialog({
      hitText: $r('app.string.bluetoothtest_str_szpzlx'),
      cancel: this.onCancel,
      confirm: this.onAccept,
      textValue: $localName,
      inputValue: $localName,
      currentIndex: $currentIndex,
      apiItem: $currentApiItem,
      changeIndex: $changeIndex,
    }),
    cancel: this.onCancel,
    autoCancel: true
  })
  setScanModelDialog: CustomDialogController = new CustomDialogController({
    builder: SetScanModeDialog({
      hitText: $r('app.string.bluetoothtest_str_szsmms'),
      cancel: this.onCancel,
      confirm: this.onAccept,
      textValue: $localName,
      inputValue: $localName,
      currentIndex: $currentIndex,
      apiItem: $currentApiItem,
      changeIndex: $changeIndex,
    }),
    cancel: this.onCancel,
    autoCancel: true
  })
  inputDialog: CustomDialogController = new CustomDialogController({
    builder: InputDialog({
      hitText: $r('app.string.bluetoothtest_str_qszbdmc'),
      cancel: this.onCancel,
      confirm: this.onAccept,
      textValue: $localName,
      inputValue: $localName,
      currentIndex: $currentIndex,
      apiItem: $currentApiItem,
      changeIndex: $changeIndex,
    }),
    cancel: this.onCancel,
    autoCancel: true
  })
  setConnectionStrategyDialog: CustomDialogController = new CustomDialogController({
    builder: SetConnectionStrategyDialog({
      hitText: $r('app.string.bluetoothtest_str_szljcl'),
      cancel: this.onCancel,
      confirm: this.onAccept,
      textValue: $localName,
      inputValue: $localName,
      currentIndex: $currentIndex,
      apiItem: $currentApiItem,
      changeIndex: $changeIndex,
    }),
    cancel: this.onCancel,
    autoCancel: true
  })

  onCancel(){
    LogUtil.info($r('app.string.bluetoothtest_str_qx'))
  }

  onAccept(text: string){
    LogUtil.info('input text:' + String(text))
  }

  @Builder IngredientItem(apiItem: TestApi, index: number){
    Stack(){
      Flex(){
        Flex({alignItems: ItemAlign.Center}){
          Image($r('app.media.bleSmall'))
            .objectFit(ImageFit.Contain)
            .autoResize(false)
            .height(ConfigData.VP_50)
            .width(ConfigData.VP_40)
            .padding({top: ConfigData.VP_15})
        }
        .layoutWeight(1)
        .padding({left: ConfigData.VP_10})

        Flex({direction: FlexDirection.Column, alignItems: ItemAlign.Start}){
          Text(apiItem.name)
            .fontSize(ConfigData.VP_17_4)
            .fontWeight(FontWeight.Bold)
            .layoutWeight(1)
            .padding({left: ConfigData.VP_10, top: ConfigData.VP_10})
          Text(apiItem.method)
            .fontSize(ConfigData.VP_17_4)
            .fontWeight(FontWeight.Bold)
            .layoutWeight(1)
            .padding({left: ConfigData.VP_10, top: ConfigData.VP_5})
        }
        .layoutWeight(6)
        .width(ConfigData.WH_80_100)

        Flex({alignItems: ItemAlign.Center}){
          Text(this.changeIndex >= 0 ? this.getCurrentState(index) : apiItem.result)
            .fontSize(ConfigData.VP_17_4)
            .flexGrow(1)
            .padding({top: ConfigData.VP_15})
        }
        .layoutWeight(3)
        .padding({left: ConfigData.VP_25})
      }
    }
    .height(ConfigData.VP_80)
    .borderRadius(ConfigData.VP_10)
    .margin({top: ConfigData.VP_5, bottom: ConfigData.VP_5})
    .backgroundColor($r("app.color.white"))
    .onClick(() => {
      switch ( apiItem.method ){
        case "setLocalName": {
          this.currentApiItem = apiItem;
          this.currentIndex += 1;
          this.inputDialog.open();
        }
          break;
        case "setLocalNameManager": {
          this.currentApiItem = apiItem;
          this.currentIndex += 1;
          this.inputDialog.open();
        }
          break;
        case "setBluetoothScanMode": {
          this.currentApiItem = apiItem;
          this.currentIndex += 1;
          this.setScanModelDialog.open();
        }
          break;
        case "setBluetoothScanModeManager": {
          this.currentApiItem = apiItem;
          this.currentIndex += 1;
          this.setScanModelDialog.open();
        }
          break;
        case "pairDevice": {
          let message: string = apiItem.api?.(this.pairedMac) || $r('app.string.bluetoothtest_str_mrxx');
          apiItem.result = message;
          this.currentIndex += 1;
          this.changeIndex = this.currentIndex;
          promptAction.showToast({message: message});
        }
          break;
        case "pairDeviceManager": {
          let message: string = apiItem.api?.(this.pairedMac) || $r('app.string.bluetoothtest_str_mrxx');
          apiItem.result = message;
          this.currentIndex += 1;
          this.changeIndex = this.currentIndex;
          promptAction.showToast({message: message});
        }
          break;
        case "unpairDevice": {
          let message: string = apiItem.api?.(this.pairedMac) || $r('app.string.bluetoothtest_str_mrxx');
          apiItem.result = message;
          this.currentIndex += 1;
          this.changeIndex = this.currentIndex;
          promptAction.showToast({message: message});
        }
          break;
        case "unpairDeviceManager": {
          let message: string = apiItem.api?.(this.pairedMac) || $r('app.string.bluetoothtest_str_mrxx');
          apiItem.result = message;
          this.currentIndex += 1;
          this.changeIndex = this.currentIndex;
          promptAction.showToast({message: message});
        }
          break;
        case "getDeviceName": {
          let message: string = apiItem.api?.(this.pairedMac) || $r('app.string.bluetoothtest_str_mrxx');
          apiItem.result = message;
          this.currentIndex += 1;
          this.changeIndex = this.currentIndex;
          promptAction.showToast({message: message});
        }
          break;
        case "getDeviceNameManager": {
          let message: string = apiItem.api?.(this.pairedMac) || $r('app.string.bluetoothtest_str_mrxx');
          apiItem.result = message;
          this.currentIndex += 1;
          this.changeIndex = this.currentIndex;
          promptAction.showToast({message: message});
        }
          break;
        case "getDeviceType": {
          let message: string = apiItem.api?.(this.pairedMac) || $r('app.string.bluetoothtest_str_mrxx');
          apiItem.result = message;
          this.currentIndex += 1;
          this.changeIndex = this.currentIndex;
          promptAction.showToast({message: message});
        }
          break;
        case "getDeviceTypeManager": {
          let message: string = apiItem.api?.(this.pairedMac) || $r('app.string.bluetoothtest_str_mrxx');
          apiItem.result = message;
          this.currentIndex += 1;
          this.changeIndex = this.currentIndex;
          promptAction.showToast({message: message});
        }
          break;
        case "getProfileConnState": {
          this.currentApiItem = apiItem;
          this.currentIndex += 1;
          this.getProfileConnSateDialog.open();
        }
          break;
        case "getProfileConnStateManager": {
          this.currentApiItem = apiItem;
          this.currentIndex += 1;
          this.getProfileConnSateDialog.open();
        }
          break;
        case "getDeviceState": {
          this.currentApiItem = apiItem;
          this.currentIndex += 1;
          this.getProfileConnSateDialog.open();
        }
          break;
        case "getDeviceStateManager": {
          this.currentApiItem = apiItem;
          this.currentIndex += 1;
          this.getProfileConnSateDialog.open();
        }
          break;
        case "setConnectionStrategy": {
          this.currentApiItem = apiItem;
          this.currentIndex += 1;
          this.setConnectionStrategyDialog.open();
        }
          break;
        default: {
          let message: string = apiItem.api();
          apiItem.result = message;
          this.currentIndex += 1;
          this.changeIndex = this.currentIndex;
          promptAction.showToast({message: message});
        }
          break;
      }
    })
  }

  getCurrentState(index: number){
    return this.apiItems[ index ].result
  }

  build(){
    Column(){
      Scroll(){
        Flex({
          direction: FlexDirection.Column,
          justifyContent: FlexAlign.SpaceBetween,
          alignItems: ItemAlign.Start
        }){
          ForEach(this.apiItems, (item: TestApi, index: number) => {
            this.IngredientItem(item, index)
          })
        }
      }
      .scrollBarWidth(ConfigData.VP_20)
    }
    .height(ConfigData.PCT_66)
    .padding({top: ConfigData.VP_5, right: ConfigData.VP_10, left: ConfigData.VP_10})
  }
}