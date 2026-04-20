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

/**
 * 默认 UiTest 套件：拉起同 bundle 的 UiTestStubAbility，验证 Driver 可见 stub 控件。
 * 跨模块枚举 entry 主界面控件在部分 ROM 上不可用，见 entry/src/ohosTest/ets/uitest/DEPLOY.txt。
 */
import { describe, beforeAll, it} from '@ohos/hypium';
import { Driver, ON} from '@ohos.UiTest';
import { assertStubTitleVisible, MAIN_BUNDLE, startUiTestStubAbility} from './MainPageControlUiShared';

export default function uitestStubSmoke(){
  describe('UitestStubSmoke', () => {
    beforeAll(async () => {
      await startUiTestStubAbility();
    });

    it('stubTitleById', 0, async () => {
      await assertStubTitleVisible();
    });

    it('stubTitleByText', 0, async () => {
      const driver = Driver.create();
      await driver.waitForIdle(200, 8000);
      await driver.assertComponentExist(ON.inWindow(MAIN_BUNDLE).text('uitest stub'));
    });
  });
}
