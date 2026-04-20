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
import LogUtil from '../../../main/ets/Utils/LogUtil';
import UIAbility from '@ohos.app.ability.UIAbility';
import { AbilityConstant, Want} from '@kit.AbilityKit';
import AbilityDelegatorRegistry from '@ohos.app.ability.abilityDelegatorRegistry';
import { Hypium} from '@ohos/hypium';
/** 默认：API + UiTest（List.test）。仅跑 apitest 全量时将 import 改为 ../apitest/List.apitest.test */
import testsuite from '../apitest/List.test';
import window from '@ohos.window';

export default class TestAbility extends UIAbility {
    onCreate(want: Want, launchParam: AbilityConstant.LaunchParam): void {
        LogUtil.info('TestAbility onCreate');
        LogUtil.info('want param:' + JSON.stringify(want) ?? '');
        LogUtil.info('launchParam:'+ JSON.stringify(launchParam) ?? '');
        const abilityDelegator = AbilityDelegatorRegistry.getAbilityDelegator();
        const abilityDelegatorArguments = AbilityDelegatorRegistry.getArguments();
        LogUtil.info('start run testcase!!!');
        Hypium.hypiumTest(abilityDelegator, abilityDelegatorArguments, testsuite)
    }

    onDestroy(){
        LogUtil.info('TestAbility onDestroy');
    }

    onWindowStageCreate(windowStage: window.WindowStage){
        LogUtil.info('TestAbility onWindowStageCreate');
        windowStage.loadContent('TestAbility/pages/index', (err, data) => {
            if (err.code){
                LogUtil.error('Failed to load the content. Cause: ' + (JSON.stringify(err) ?? ''));
                return;
            }
            LogUtil.info('Succeeded in loading the content. Data: ' + (JSON.stringify(data) ?? ''))
    });
    }

    onWindowStageDestroy(){
        LogUtil.info('TestAbility onWindowStageDestroy');
    }

    onForeground(){
        LogUtil.info('TestAbility onForeground');
    }

    onBackground(){
        LogUtil.info('TestAbility onBackground');
    }
}