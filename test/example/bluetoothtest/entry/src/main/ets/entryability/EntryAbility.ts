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
import UIAbility from '@ohos.app.ability.UIAbility';
import window from '@ohos.window';
import type { Want} from '@kit.AbilityKit';
import { setSkipBluetoothPermissionForAutomatedTest} from '../bluetoothTestNav';
import { setUiAbilityContext} from '../Utils/uiContextStore';

const DEFAULT_PAGE = 'pages/homePage';
const TEST_PAGE_KEY = 'testTargetPage';

export default class EntryAbility extends UIAbility {
  private windowStageRef: window.WindowStage | null = null;
  private initialPage: string = DEFAULT_PAGE;

  private extractTestTargetPage(w: Want | undefined): string | undefined {
    if (!w?.parameters){
      return undefined;
    }
    const v = w.parameters[TEST_PAGE_KEY];
    return typeof v === 'string' && v.length > 0 ? v: undefined;
  }

  onCreate(want, launchParam){
    LogUtil.info('Ability onCreate');
    setUiAbilityContext(this.context);
    const p = this.extractTestTargetPage(want);
    const fromAutomatedTest = p !== undefined;
    setSkipBluetoothPermissionForAutomatedTest(fromAutomatedTest);
    if (fromAutomatedTest && p){
      this.initialPage = p;
    }
  }

  onNewWant(want, launchParam){
    const p = want?.parameters?.[TEST_PAGE_KEY];
    if (typeof p !== 'string' || p.length === 0 || !this.windowStageRef){
      return;
    }
    setSkipBluetoothPermissionForAutomatedTest(true);
    this.windowStageRef.loadContent(p, (err, data) => {
      if (err.code){
        LogUtil.error('loadContent onNewWant failed: ' + (JSON.stringify(err) ?? ''));
        return;
      }
      LogUtil.info('onNewWant loadContent ok: ' + (JSON.stringify(data) ?? ''));
    });
  }

  onDestroy(){
    LogUtil.info('Ability onDestroy');
  }

  onWindowStageCreate(windowStage: window.WindowStage){
    LogUtil.info('Ability onWindowStageCreate');
    this.windowStageRef = windowStage;
    const fromLaunch = this.extractTestTargetPage(this.launchWant);
    if (fromLaunch){
      setSkipBluetoothPermissionForAutomatedTest(true);
      this.initialPage = fromLaunch;
    }
    const page = this.initialPage;
    windowStage.loadContent(page, (err, data) => {
      if (err.code){
        LogUtil.error('Failed to load the content. Cause: ' + (JSON.stringify(err) ?? ''));
        return;
      }
      LogUtil.info('Succeeded in loading the content. Data: ' + (JSON.stringify(data) ?? ''))
    });
  }

  onWindowStageDestroy(){
    LogUtil.info('Ability onWindowStageDestroy');
    this.windowStageRef = null;
  }

  onForeground(){
    LogUtil.info('Ability onForeground');
  }

  onBackground(){
    LogUtil.info('Ability onBackground');
  }
}
