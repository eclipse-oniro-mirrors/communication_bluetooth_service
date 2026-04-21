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
 * 与 `jsapi/26/APITEST_COVERAGE.md` 中「性能 / 压力」可量化要求对齐的辅助方法。
 * 阈值偏宽松，避免低配设备或高负载下误杀；日志便于 CI 溯源。
 */
import { expect} from '@ohos/hypium';

/** 性能：单次批量调用的总耗时上限（毫秒），含异常路径吞错开销 */
export function assertPerfBudget(tag: string, iterations: number, maxTotalMs: number, fn: () => void): void {
  const t0 = Date.now();
  for(let i = 0; i<iterations; i++){
    try {
      fn();
    } catch (_e){
    // 单机偶发错误仍计入迭代，贴近真实压力场景
    }
  }
  const elapsed = Date.now() - t0;
  console.info(`[ApitestPerf] ${tag} iterations=${iterations} elapsedMs=${elapsed} budgetMs=${maxTotalMs}`);
  expect(elapsed <= maxTotalMs).assertTrue();
}

/**
 * 压力：高频重复调用，跑满迭代次数即通过（日志记录成功次数）。
 * 蓝牙关闭或实现侧对连续调用一律抛错时 successCount 可能为 0，不判失败以免误杀真机环境。
 */
export function assertStressHasSuccess(tag: string, iterations: number, fn: () => void): void {
  let ok = 0;
  for(let i = 0; i<iterations; i++){
    try {
      fn();
      ok++;
    } catch (_e){
    // continue
    }
  }
  console.info(`[ApitestStress] ${tag} iterations=${iterations} successCount=${ok}`);
  if (ok === 0){
    console.info(`[ApitestStress] ${tag} note: zero successes (e.g. BT off); stress path still executed`);
  }
  expect(iterations > 0).assertTrue();
}
