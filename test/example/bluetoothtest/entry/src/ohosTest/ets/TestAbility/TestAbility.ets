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
import access from '@ohos.bluetooth.access';
/** 默认全量 List.apitest.test（勿一次上板）；分段请改 scenario.core / profiles / legacy / extra */
import testsuite from '../apitest/List.apitest.scenario.core.test';
import { scDismissBtEnableDialog } from '../apitest/ApitestScenarioShared';
import window from '@ohos.window';

/** Give enableBluetooth time to settle without busy-spin (avoids THREAD_BLOCK_6S). */
const BT_PREPARE_MS: number = 2500;

export default class TestAbility extends UIAbility {
    onCreate(want: Want, launchParam: AbilityConstant.LaunchParam): void {
        LogUtil.info('TestAbility onCreate');
        LogUtil.info('want param:' + JSON.stringify(want) ?? '');
        LogUtil.info('launchParam:'+ JSON.stringify(launchParam) ?? '');
        const abilityDelegator = AbilityDelegatorRegistry.getAbilityDelegator();
        const abilityDelegatorArguments = AbilityDelegatorRegistry.getArguments();
        let needEnable = true;
        try {
            needEnable = access.getState() !== access.BluetoothState.STATE_ON;
        } catch (e0) {
            LogUtil.error(JSON.stringify(e0));
        }
        if (needEnable) {
            try {
                access.enableBluetooth();
                LogUtil.info('TestAbility pre-enableBluetooth ok');
            } catch (e) {
                LogUtil.error('TestAbility pre-enableBluetooth ' + JSON.stringify(e));
            }
        } else {
            LogUtil.info('TestAbility skip enableBluetooth (already STATE_ON)');
        }
        LogUtil.info('start run testcase after ' + BT_PREPARE_MS + 'ms');
        setTimeout((): void => {
            scDismissBtEnableDialog(2000).then((): void => {
                try {
                    const st = access.getState();
                    LogUtil.info('TestAbility pre-state=' + st);
                } catch (e2) {
                    LogUtil.error(JSON.stringify(e2));
                }
                Hypium.hypiumTest(abilityDelegator, abilityDelegatorArguments, testsuite);
            }).catch((): void => {
                Hypium.hypiumTest(abilityDelegator, abilityDelegatorArguments, testsuite);
            });
        }, BT_PREPARE_MS);
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