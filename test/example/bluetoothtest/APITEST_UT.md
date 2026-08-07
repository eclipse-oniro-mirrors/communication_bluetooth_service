# Bluetooth Apitest UT 使用说明与设计文档

> 适用路径：`test/example/bluetoothtest/`  
> 目标设备：OpenHarmony 真机（单板，蓝牙已开启，无对端）  
> 设计基线：**1A 实逻辑断言**（禁止 `assert(true)` / `expect(true).assertTrue()`）

---

## 1. 结论：全量 UT 能否一次跑通？

| 问题 | 答案 |
|------|------|
| 能否在一份 List 里一次跑完全部用例？ | **不建议。** `List.apitest.test.ets` 聚合约 **3500+** 个 `it`，真机一次执行易触发 `THREAD_BLOCK_6S` / `App died`。 |
| 全量是否“等价可过”？ | **可以（分段）。** coreA+coreB+profiles 近期实测合计 **2808** FullCoverage Pass（见 §6）；Mass **570** 已绿跑（`List.apitest.mass`）。 |
| 日常回归怎么跑？ | 用 `List.apitest.coreA` / `coreB` / `profiles` 三包即可覆盖 Access+Legacy、GATT+Conn+Socket、各 Profile+Enterprise+Partner；Mass-only 场景用 `List.apitest.mass`；**场景组合流**分段用 `List.apitest.scenario.core` / `scenario.profiles` / `scenario.legacy` / `scenario.extra`（聚合 `scenario.test` 勿一次上板；设计见 [`APITEST_SCENARIO.md`](./APITEST_SCENARIO.md)）。 |
| 单文件行数？ | **≤2000 行**（超限的 FullCoverage/Mass/ExpandedMatrix/jsapi `.d.ts` 已拆为 `*_pN` / `*.partN.d.ts`）。 |

**推荐执行策略：分段跑全量，不要一键全 List。**

---

## 2. 目录与角色

```
test/example/bluetoothtest/
├── APITEST_UT.md                          ← 本文（使用说明 + 设计）
├── entry/src/ohosTest/ets/
│   ├── TestAbility/TestAbility.ets        ← 入口：改 import 的 List 即可切换套件
│   ├── test/List.test.ets                 ← 工程默认入口（常转调 apitest）
│   ├── apitest/
│   │   ├── List.apitest.test.ets          ← 全量聚合（勿一次跑）
│   │   ├── List.apitest.coreA.test.ets    ← Access + Constant/Common + Legacy
│   │   ├── List.apitest.coreB.test.ets    ← GattClient/Server + Conn + Socket
│   │   ├── List.apitest.profiles.test.ets ← a2dp/hfp/hid/map/opp/pan/pbap/wear/enterprise/partner
│   │   ├── List.apitest.mass.test.ets     ← ConnBleSocketMass*_pN（~570）
│   │   ├── List.apitest.scenario.test.ets ← 场景聚合（239，勿一次上板）
│   │   ├── List.apitest.scenario.core.test.ets     ← baseline+Conn/Ble/Socket/Cross（~187）
│   │   ├── List.apitest.scenario.profiles.test.ets ← ProfileBase01（25）
│   │   ├── List.apitest.scenario.legacy.test.ets   ← Legacy01（15）
│   │   ├── List.apitest.scenario.extra.test.ets    ← Extra01（12）
│   │   ├── List.apitest.part1a.test.ets   ← Mass + ExpandedMatrix（体量大，单独段）
│   │   ├── ApitestRealAssert.ets          ← 1A 断言助手
│   │   ├── ApitestScenarioShared.ets      ← Scenario 流程断言 / 参数 fixture
│   │   ├── ApitestFullCoverageShared.ets  ← 公共常量 / runPerf / runStress
│   │   ├── BluetoothScenario*.test.ets    ← 场景组合用例（全模块）
│   │   └── BluetoothFullCoverage*_pN.test.ets  ← 单文件 ≤2000 行
│   └── jsapi/26/
│       ├── APITEST_COVERAGE.md            ← API 覆盖清单
│       └── BluetoothCoreApi.catalog.json
├── APITEST_SCENARIO.md                    ← Scenario 全量交叉设计稿（正/异常/真实等待/容量）
└── skills/ohtest/（仓库根）
    ├── bluetooth_full_coverage.py         ← FullCoverage 生成器
    ├── bluetooth_scenario_flow.py         ← Scenario baseline 生成器
    ├── bluetooth_scenario_modules.py      ← Scenario 全模块 L1–L4 生成器
    ├── gen_scenario_full_design.py        ← 从 catalog 生成 APITEST_SCENARIO.md 设计稿
    ├── gen_scenario_doc.py                ← 从已实现用例抽对照表
    └── bluetooth_core_catalog.py          ← API 目录源
```

---

## 3. UT 设计（1A）

### 3.1 环境假设

- **单板**：仅本机，无第二台对端设备。
- **蓝牙 ON**：每个 `describe` 的 `beforeEach` 调用 `ensureBtOn()`。
- **无对端**：需要远端 GATT/连接/配对的操作，预期业务失败码，用 `assertPeerExpect`，不得用 `assert(true)` 顶替。

### 3.2 断言三分法

| 助手 | 场景 | 规则 |
|------|------|------|
| `assertLocalOk(errCode)` | 纯本机可读/可写 API | 仅允许 `0` 或白名单本地码（如 `201`） |
| `assertPeerExpect(errCode)` | 依赖对端/连接态 | 允许 `0` 或明确的 peer 失败码（`2900001`/`2900099`/`401`/`801`/`12300001` 等） |
| `assertThrownErrIn(errCode, codes)` | 异常路径 / 非法入参 | `errCode` 必须落在给定集合内 |

实现见：`apitest/ApitestRealAssert.ets`。

### 3.3 标准六态（Body6）

每个 API 生成 **6** 个 `it`（`min-cases` 可调，默认每 API ≥6）：

| 后缀 | 含义 |
|------|------|
| `_r` | 正常入参（本机可达则 LocalOk，否则 PeerExpect） |
| `_e` | 异常入参 / 非法对象，`assertThrownErrIn` |
| `_perf` | 限时循环（默认 5s） |
| `_stress` | 限次循环（默认 100） |
| `_reliability` | 重复调用稳定性 |
| `_compatibility` | 兼容/边界组合 |

工厂类 API（如 `createGattClientDevice`）必须放在 **`try` 内**，避免空 MAC / 非法参数在 `try` 外抛出导致 **Error**（非 Fail）。

### 3.4 禁止项

- `expect(true).assertTrue()` / `assert(true)` 作为业务结论。
- `errCode >= 0` 式弱断言。
- `fcRunCombo` 等“空跑组合”冒充覆盖。
- 在 `try` 外调用会抛 `BusinessError` 的工厂/建连 API（会导致套件 Error）。

### 3.5 生成与覆盖

```bash
# 仓库根目录
python3 skills/ohtest/bluetooth_full_coverage.py --min-cases 1000
```

- 目录源：`skills/ohtest/bluetooth_core_catalog.py` → 写出 `jsapi/26/BluetoothCoreApi.catalog.json`
- 覆盖说明：`jsapi/26/APITEST_COVERAGE.md`
- 生成清单：`apitest/BluetoothFullCoverage.manifest.txt`

---

## 4. 如何跑 UT（操作步骤）

### 4.1 环境准备

```bash
# 设备连接（示例）
/tmp/hdcwrap/hdc list targets
# 常用：192.168.11.90:8710

export HDC=/tmp/hdcwrap/hdc
export TARGET=192.168.11.90:8710
```

确认设备蓝牙已打开（用例 `beforeEach` 也会尝试 `enableBluetooth`）。

### 4.2 切换测试套件

编辑 `entry/src/ohosTest/ets/TestAbility/TestAbility.ets`，只保留需要的 List import，例如：

```typescript
// 全量聚合（不建议一次跑）
import testsuite from '../apitest/List.apitest.test';

// FullCoverage 分段：
// import testsuite from '../apitest/List.apitest.coreA.test';
// import testsuite from '../apitest/List.apitest.coreB.test';
// import testsuite from '../apitest/List.apitest.profiles.test';

// Scenario 分段（当前默认 core）：
// import testsuite from '../apitest/List.apitest.scenario.core.test';
// import testsuite from '../apitest/List.apitest.scenario.profiles.test';
// import testsuite from '../apitest/List.apitest.scenario.legacy.test';
// import testsuite from '../apitest/List.apitest.scenario.extra.test';
```

### 4.3 编译 / 签名 / 一键上板（推荐）

在仓库根目录：

```bash
export HOS_CLT_PATH=/root/toolchains/command-line-tools
export OHOS_SDK_PATH=/root/toolchains/ohos-sdk-6.0-release
export NODE_HOME=$HOS_CLT_PATH/tool/node
export PATH=/tmp/hdcwrap:$NODE_HOME/bin:$PATH   # hdcwrap 固定 -t <设备>

# 改完 TestAbility 的 List 后：
python3 skills/ohhap/hapbuild.py build-test test/example/bluetoothtest
python3 skills/ohhap/hapbuild.py sign test/example/bluetoothtest

# 安装主包+测试包并测试（勿加 -s class，否则嵌套 describe 可能 Tests run: 0）
python3 skills/ohhdc/ohhdc.py deploy-test test/example/bluetoothtest --timeout 3600000
```

关注报告行：

```
Tests run: N, Failure: 0, Error: 0, Pass: N, Ignore: 0
```

### 4.4 分段全量清单（推荐顺序）

| 顺序 | List 文件 | 内容 | 预期规模（约） |
|------|-----------|------|----------------|
| 1 | `List.apitest.coreA.test.ets` | Access + Constant/枚举 + Legacy | ~1128 |
| 2 | `List.apitest.coreB.test.ets` | Gatt + Conn + Socket | ~960 |
| 3 | `List.apitest.profiles.test.ets` | Profile / Enterprise / Partner | ~720 |
| 4 | `List.apitest.mass.test.ets` | ConnBleSocketMass01～04 | ~570 |
| 5 | `List.apitest.part1a.test.ets`（如需要） | Mass + ExpandedMatrix | 视生成配置 |
| S1 | `List.apitest.scenario.core.test.ets` | Scenario baseline+Conn/Ble/Socket/Cross | ~187 |
| S2 | `List.apitest.scenario.profiles.test.ets` | Scenario ProfileBase | 25 |
| S3 | `List.apitest.scenario.legacy.test.ets` | Scenario Legacy | 15 |
| S4 | `List.apitest.scenario.extra.test.ets` | Scenario Extra | 12 |

每段独立编译安装跑完、确认 **Fail=0 Error=0** 后再切下一段。Scenario 设计明细见 [`APITEST_SCENARIO.md`](./APITEST_SCENARIO.md)。

---

## 5. 新增 / 修改用例时注意

1. **优先改生成器** `bluetooth_full_coverage.py`，再 `python3 ... --min-cases 1000` 重生，避免手改 `BluetoothFullCoverage*.test.ets` 被覆盖丢失。
2. 新错误码先加入 `ApitestRealAssert.ets` 白名单，再让用例引用。
3. Profile 工厂（`createA2dpSrcProfile` 等）一律放在 `try` 内；`801`（能力缺失）走 Peer/Thrown 路径。
4. 本地改完后至少跑对应分段 List，不要只靠编译通过。

---

## 6. 近期分段实测（参考）

设备：`192.168.11.90:8710`（API 26），单板 BT ON、无对端。

| 套件 | Pass | Fail | Error | 说明 |
|------|------|------|-------|------|
| profiles | 720 | 0 | 0 | a2dp/hfp/hid/map/opp/pan/pbap/wear/enterprise/partner |
| coreB | **960** | **0** | **0** | Gatt+Conn+Socket（含 Mass 并入的 OOB/Advertise/getConnectedBLEDevices 等，2026-07-28） |
| coreA | **1128** | **0** | **0** | Access + Constant/枚举穷举(304，含 `DialogType`) + Legacy（含 legacy adv，2026-07-28） |
| mass | **570** | **0** | **0** | ConnBleSocketMass01～04；`ConnBleSocketShared` 导出 `assertThrownErrIn`（2026-07-28） |
| scenario.core | **187** | **0** | **0** | baseline + Conn02 + BleAdvGatt + Socket + Cross + MultiModule |
| scenario.profiles | **25** | **0** | **0** | ProfileBase01（含 pan 801 容错） |
| scenario.legacy | **15** | **0** | **0** | `@ohos.bluetooth` / `bluetoothManager` |
| scenario.extra | **12** | **0** | **0** | wear / enterprise / partner / system stub / constant |

**分段合计（coreA+coreB+profiles）：2808 Pass / 0 Fail / 0 Error**（与 FullCoverage 生成总量一致）。  
Mass 单独段：**570 Pass / 0 Fail / 0 Error**。  
Scenario 分段合计：**239 Pass / 0 Fail / 0 Error**。  
说明：原 Mass-only API（如 `pairDeviceOutOfBand`、`startAdvertising`、`getConnectedBLEDevices` 等）已并入 FullCoverage；Mass 套件仍作高密度回归。`@system.bluetooth` 为 **FA-only**，Stage 用 `ble`/`BleScanner` 等价覆盖。

`List.apitest.test` 若再聚合 Mass/ExpandedMatrix，体量更大，仍须继续分段，勿一次测试。Scenario 聚合 `List.apitest.scenario.test` 同理勿一次上板。

> 数字随 `--min-cases` / catalog 变更会浮动；以当次设备报告为准。

---

## 7. 相关文档

- API 覆盖：[`entry/src/ohosTest/ets/jsapi/26/APITEST_COVERAGE.md`](entry/src/ohosTest/ets/jsapi/26/APITEST_COVERAGE.md)
- Scenario 组合设计：[`APITEST_SCENARIO.md`](./APITEST_SCENARIO.md)
- 断言实现：`entry/src/ohosTest/ets/apitest/ApitestRealAssert.ets`
- 生成器：`skills/ohtest/bluetooth_full_coverage.py`、`bluetooth_scenario_flow.py`、`bluetooth_scenario_modules.py`
