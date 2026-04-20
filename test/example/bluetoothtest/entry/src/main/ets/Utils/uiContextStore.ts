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

import common from '@ohos.app.ability.common';

let uiAbilityContextRef: common.UIAbilityContext | undefined = undefined;

/** 由 EntryAbility.onCreate 调用；与 resString.ets 解耦，避免 TS 入口导入 .ets。 */
export function setUiAbilityContext(c: common.UIAbilityContext): void {
  uiAbilityContextRef = c;
}

export function getUiAbilityContext(): common.UIAbilityContext | undefined {
  return uiAbilityContextRef;
}
