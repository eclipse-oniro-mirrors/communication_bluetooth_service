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
 * Shared helpers for main-module page control UiTest (Hypium).
 * EntryAbility loads route from want.parameters.testTargetPage (see entry EntryAbility.ts).
 */
import abilityDelegatorRegistry from '@ohos.app.ability.abilityDelegatorRegistry';
import { Want} from '@kit.AbilityKit';
import { Component, Driver, ON, On} from '@ohos.UiTest';

export const MAIN_BUNDLE: string = 'com.example.myapplicationbt';
/** 与 entry/module.json5 中 module.name 一致；多模块工程下 startAbility 需指定，否则无法正确拉起含页面路由的 entry UIAbility */
export const ENTRY_MODULE: string = 'entry';
export const ENTRY_ABILITY: string = 'EntryAbility';
/** stub 能力在 entry 主模块内（pages/uitestStub），与 Hypium 同包便于 Driver 枚举窗口 */
export const UITEST_STUB_ABILITY: string = 'UiTestStubAbility';

function delayMs(ms: number): Promise<void> {
  return new Promise<void>((resolve: (v?: void) => void) => {
    setTimeout((): void => {
      resolve();
    }, ms);
  });
}

async function tryWaitForControl(driver: Driver, on: On, timeoutMs: number): Promise<boolean> {
  try {
    const c: Component = await driver.waitForComponent(on, timeoutMs);
    return c !== undefined;
  } catch (_e){
    return false;
  }
}

/** 在窗口内可滚动容器（Scroll / List）上 scrollSearch，用于控件未进入可视区时的探测 */
async function tryScrollSearchInScrollables(driver: Driver, controlId: string): Promise<boolean> {
  const targetOn = ON.id(controlId);
  const kinds: string[] = ['Scroll', 'List'];
  for(let k = 0; k<kinds.length; k++){
    try {
      const containerOn = ON.inWindow(MAIN_BUNDLE).type(kinds[k]);
      const containers: Array<Component> = await driver.findComponents(containerOn);
      for(let i = 0; i<containers.length; i++){
        try {
          const found: Component = await containers[i].scrollSearch(targetOn);
          if (found !== undefined){
            return true;
          }
        } catch (_e){
        }
      }
    } catch (_e){
    }
  }
  return false;
}

export async function startMainPage(route: string): Promise<void> {
  const delegator = abilityDelegatorRegistry.getAbilityDelegator();
  /** 与 hdc shell 一致；部分机型上 delegator.startAbility 传入的 parameters 未进到 EntryAbility */
  const cmd =
    `aa start -b ${MAIN_BUNDLE} -a ${ENTRY_ABILITY} -m ${ENTRY_MODULE} --ps testTargetPage ${route}`;
  const shellRet = await delegator.executeShellCommand(cmd, 60);
  if (shellRet.exitCode !== 0){
    const want: Want = {
      bundleName: MAIN_BUNDLE,
      moduleName: ENTRY_MODULE,
      abilityName: ENTRY_ABILITY,
      parameters: {
        testTargetPage: route
      }
    };
    await delegator.startAbility(want);
  }
  const driver = Driver.create();
  for(let i = 0; i<40; i++){
    await delayMs(300);
    try {
      const win = await driver.findWindow({bundleName: MAIN_BUNDLE});
      if (win){
        await win.focus();
        await delayMs(600);
        break;
      }
    } catch (_e){
    }
  }
  await delayMs(400);
}

/** 拉起 entry 内的 UiTestStubAbility（pages/uitestStub/Index），供 UiTest 校验同 bundle 内窗口可见性 */
export async function startUiTestStubAbility(): Promise<void> {
  const delegator = abilityDelegatorRegistry.getAbilityDelegator();
  const cmd =
    `aa start -b ${MAIN_BUNDLE} -a ${UITEST_STUB_ABILITY} -m ${ENTRY_MODULE}`;
  const shellRet = await delegator.executeShellCommand(cmd, 60);
  if (shellRet.exitCode !== 0){
    const want: Want = {
      bundleName: MAIN_BUNDLE,
      moduleName: ENTRY_MODULE,
      abilityName: UITEST_STUB_ABILITY
    };
    await delegator.startAbility(want);
  }
  const driver = Driver.create();
  for(let i = 0; i<40; i++){
    await delayMs(300);
    try {
      const win = await driver.findWindow({bundleName: MAIN_BUNDLE});
      if (win){
        await win.focus();
        await delayMs(600);
        break;
      }
    } catch (_e){
    }
  }
  await delayMs(400);
}

export async function assertStubTitleVisible(): Promise<void> {
  const driver = Driver.create();
  try {
    const win = await driver.findWindow({bundleName: MAIN_BUNDLE});
    if (win){
      await win.focus();
      await delayMs(200);
    }
  } catch (_e){
  }
  const on = ON.inWindow(MAIN_BUNDLE).id('uitest_stub_title');
  await driver.waitForIdle(200, 8000);
  await driver.assertComponentExist(on);
}

export async function assertControlVisible(controlId: string): Promise<void> {
  const driver = Driver.create();
  try {
    const win = await driver.findWindow({bundleName: MAIN_BUNDLE});
    if (win){
      await win.focus();
      await delayMs(300);
    }
  } catch (_e){
  }
  const onScoped = ON.inWindow(MAIN_BUNDLE).id(controlId);
  const onPlain = ON.id(controlId);
  await driver.waitForIdle(200, 15000);
  if (await tryWaitForControl(driver, onScoped, 10000)){
    return;
  }
  if (await tryWaitForControl(driver, onPlain, 6000)){
    return;
  }
  const mayNeedScroll: boolean =
    controlId.indexOf('__sc_') >= 0 || controlId.indexOf('__li_') >= 0;
  if (mayNeedScroll){
    if (await tryScrollSearchInScrollables(driver, controlId)){
      return;
    }
    await driver.waitForIdle(200, 4000);
    if (await tryWaitForControl(driver, onScoped, 6000)){
      return;
    }
  }
  await delayMs(400);
  if (await tryWaitForControl(driver, onScoped, 5000)){
    return;
  }
  await driver.assertComponentExist(onScoped);
}
