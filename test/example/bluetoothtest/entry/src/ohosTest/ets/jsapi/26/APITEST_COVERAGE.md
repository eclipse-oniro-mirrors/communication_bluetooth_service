# `jsapi/26` ↔ `apitest` 覆盖对照

## 完全覆盖判定标准（本表采用）

若记为 **「是」**，须同时满足对 **该 `.d.ts` 中每个对外接口**：

1. **参数**：每个参数均覆盖正常值、错误值、边界值；类型分支齐全；若为枚举则穷举取值及**合理业务组合**（含多参数时的组合，而非仅单参枚举全排列若组合空间无穷则采用等价类划分并说明）。
2. **返回值 / 回调**：对同步返回值、异步结果与错误分支有明确断言；错误码/状态与文档一致。
3. **流程**：正常主路径、可预期异常路径均有用例；多接口调用序列的组合覆盖到主要业务场景。
4. **性能**：每个接口有**可量化指标**的简单性能用例（如耗时阈值、重复调用统计），并在 CI/本地可复现。
5. **压力**：每个接口有简单压力用例（如并发/连续调用次数、资源不泄漏断言）。

当前 `apitest` 以 **入口冒烟**、`tryInvoke` 吞异常、**ConnBleSocket Mass** 批量调用为主，**不满足**上述「完全覆盖」定义；表中 **「覆盖维度说明」** 仅描述**相对该标准**已做到的大致范围（缩写：**参**=参数，**返**=返回值/回调，**组**=多接口/多参数组合，**正**=正常路径，**异**=异常路径，**性**=性能，**压**=压力）。

---

说明（前三列）：

- **第二列**：对应 `entry/src/ohosTest/ets/apitest/` 下的 Hypium 源文件。
- **第三列**：`it(` 条数；`ConnectivityKitBluetooth.test.ets` 按各 `describe` 块拆分（当前整文件 **48** 条 `it`）；`BluetoothApiCoverage.test.ets` 按所调模块拆分（当前整文件 **24** 条）；`ConnBleSocketMass01`～`Mass04` 四文件合计 **570**（180+174+126+90），与 `connection` / `ble` / `socket` / `constant` / `common` 五类声明交叉对应时记为 **「ConnectivityKit 中该模块条数 + 570」**。与 Mass 叠加的第三列为手工加总，以 `grep it(` 为准。
- **无对应** 填 **—**，用例数 **0**。

## 对照表

| `jsapi/26` 文件 | 对应 `apitest` 文件 | 用例数（`it`） | 是否完全覆盖 | 覆盖维度说明（相对上述严格标准） |
|-----------------|---------------------|----------------|--------------|-----------------------------------|
| `2in1.json` | — | 0 | 否 | 无 apitest；**参/返/正/异/组/性/压**均未实施 |
| `default.json` | — | 0 | 否 | 同上 |
| `tablet.json` | — | 0 | 否 | 同上 |
| `tv.json` | — | 0 | 否 | 同上 |
| `wearable.json` | — | 0 | 否 | 同上 |
| `permissions.d.ts` | —（权限常量，无单独 Hypium） | 0 | 否 | 无运行时接口级 Hypium；权限字符串/能力未做用例级校验 |
| `@ohos.FusionConnectivity.partnerAgent.d.ts` | `BluetoothFullCoverageEnterprise01.test.ets` | 48 | 否 | **正/异/返**：`isPartnerAgentSupported`/`bindDevice`/`unbindDevice` 等 8 API × 6 场景（n/e/b/r/x/c）；**参**：`PartnerDeviceAddress`/`DeviceCapability`/`BusinessCapability` stub；**组**：无跨模块专项；**性/压**：无 |
| `@ohos.backgroundTaskManager.d.ts` | — | 0 | 否 | 同上 |
| `@ohos.batteryStatistics.d.ts` | — | 0 | 否 | 同上 |
| `@ohos.bluetooth.a2dp.d.ts` | `ConnectivityKitBluetooth.test.ets`；`BluetoothFullCoverageProfile01.test.ets` | 127 | 否 | **正/异/返/组**：FullCoverage 对 A2dpSrcProfile 全方法 6 场景（含 `setCurrentCodecInfo`/`disableAutoPlay` 参数组合）；CK 入口 1 条；**性/压**：无 |
| `@ohos.bluetooth.access.d.ts` | `ConnectivityKitBluetooth.test.ets`；`BluetoothFullCoverageAccess01.test.ets`；`BluetoothFullCoverageCombo01.test.ets`～`Combo02.test.ets` | 163 | 否 | **正/异/返/组**：FullCoverage 对 access 全 API 6 场景 + Combo 跨模块序列；CK 7 条；**性**：`design_perf`；相对 `.d.ts` 全 API：**未**达严格完全覆盖 |
| `@ohos.bluetooth.baseProfile.d.ts` | `ConnectivityKitBluetooth.test.ets`；`BluetoothFullCoverageProfile01.test.ets` | 26 | 否 | **正/异/返/组**：FullCoverage 对 baseProfile 方法（`setConnectionStrategy`/`getConnectedDevices` 等）6 场景；CK 枚举 2 条 |
| `@ohos.bluetooth.ble.d.ts` | `ConnectivityKitBluetooth.test.ets`；`ConnBleSocketMass01.test.ets`～`ConnBleSocketMass04.test.ets`；`BluetoothFullCoverageGatt01.test.ets`～`Gatt02.test.ets`；`BluetoothFullCoverageCombo01.test.ets`～`Combo02.test.ets` | 858 | 否 | **正/异/返/组**：FullCoverage GattClient/GattServer/BleScanner 全 API 6 场景 + Combo 扫描/广播/GATT 生命周期；Mass + CK；**性/压**：`design_perf`/`design_stress` |
| `@ohos.bluetooth.common.d.ts` | `ConnectivityKitBluetooth.test.ets`；`ConnBleSocketMass01.test.ets`～`ConnBleSocketMass04.test.ets` | 572 | 否 | **正**：`BluetoothAddressType` 含 VIRTUAL/REAL + Mass；**性/压**：无专项 |
| `@ohos.bluetooth.connection.d.ts` | `ConnectivityKitBluetooth.test.ets`；`ConnBleSocketMass01.test.ets`～`ConnBleSocketMass04.test.ets`；`BluetoothFullCoverageCombo01.test.ets`～`Combo02.test.ets` | 639 | 否 | **正/异/组**：FullCoverage Combo 含配对/发现/Profile 状态矩阵（50×6）；Mass + CK |
| `@ohos.bluetooth.constant.d.ts` | `ConnectivityKitBluetooth.test.ets`；`ConnBleSocketMass01.test.ets`～`ConnBleSocketMass04.test.ets` | 574 | 否 | **正**：`ProfileId` / `ProfileConnectionState` 枚举补全 + Mass；**性/压**：无专项 |
| `@ohos.bluetooth.d.ts` | `BluetoothApiCoverage.test.ets`；`ConnectivityKitBluetooth.test.ets`；`BluetoothFullCoverageLegacy01.test.ets`～`Legacy02.test.ets` | 389 | 否 | **正/异/返/组**：FullCoverage 对 legacy `@ohos.bluetooth` 全 API（含 `BLE.*` 命名空间）6 场景；CK + ApiCoverage 17 条 |
| `@ohos.bluetooth.hfp.d.ts` | `ConnectivityKitBluetooth.test.ets`；`BluetoothFullCoverageProfile01.test.ets` | 13 | 否 | **正/异/返/组**：FullCoverage HfpAgProfile connect/disconnect 6 场景；CK 1 条 |
| `@ohos.bluetooth.hid.d.ts` | `BluetoothApiCoverage.test.ets`；`ConnectivityKitBluetooth.test.ets`；`BluetoothFullCoverageProfile01.test.ets`～`Profile02.test.ets` | 98 | 否 | **正/异/返/组**：FullCoverage HidHost + HidDevice 全 API 6 场景；CK/ApiCoverage 2 条 |
| `@ohos.bluetooth.map.d.ts` | `ConnectivityKitBluetooth.test.ets`；`BluetoothFullCoverageProfile01.test.ets` | 19 | 否 | **正/异/返/组**：FullCoverage MapMseProfile 6 场景；CK 1 条 |
| `@ohos.bluetooth.opp.d.ts` | `BluetoothApiCoverage.test.ets`；`ConnectivityKitBluetooth.test.ets`；`BluetoothFullCoverageProfile02.test.ets` | 50 | 否 | **正/异/返/组**：FullCoverage OppServerProfile 全方法 + 事件 6 场景；CK/ApiCoverage 2 条 |
| `@ohos.bluetooth.pan.d.ts` | `BluetoothApiCoverage.test.ets`；`ConnectivityKitBluetooth.test.ets`；`BluetoothFullCoverageProfile01.test.ets` | 20 | 否 | **正/异/返/组**：FullCoverage PanProfile 6 场景；CK/ApiCoverage 2 条 |
| `@ohos.bluetooth.pbap.d.ts` | `ConnectivityKitBluetooth.test.ets`；`BluetoothFullCoverageProfile02.test.ets` | 31 | 否 | **正/异/返/组**：FullCoverage PbapServerProfile 6 场景；CK 1 条 |
| `@ohos.bluetooth.socket.d.ts` | `ConnectivityKitBluetooth.test.ets`；`ConnBleSocketMass01.test.ets`～`ConnBleSocketMass04.test.ets`；`BluetoothFullCoverageCombo01.test.ets` | 579 | 否 | **正/组**：FullCoverage Combo 含 sppListen/sppClose 序列；Mass + CK |
| `@ohos.bluetooth.wearDetection.d.ts` | `ConnectivityKitBluetooth.test.ets`；`BluetoothFullCoverageAccess01.test.ets`；`BluetoothFullCoverageCombo02.test.ets` | 14 | 否 | **正/异/返/组**：FullCoverage 4 API × 6 场景 + Combo 穿戴检测序列；CK 2 条 |
| `@ohos.bluetoothManager.d.ts` | `BluetoothApiCoverage.test.ets`；`ConnectivityKitBluetooth.test.ets`；`BluetoothFullCoverageLegacy02.test.ets` | 201 | 否 | **正/异/返/组**：FullCoverage legacy bluetoothManager 全 API 6 场景；CK/ApiCoverage 9 条 |
| `@ohos.commonEvent.d.ts` | — | 0 | 否 | 无 apitest；**参/返/正/异/组/性/压**均未实施 |
| `@ohos.commonEventManager.d.ts` | — | 0 | 否 | 同上 |
| `@ohos.distributedDeviceManager.d.ts` | — | 0 | 否 | 同上 |
| `@ohos.distributedHardware.deviceManager.d.ts` | — | 0 | 否 | 同上 |
| `@ohos.distributedsched.abilityConnectionManager.d.ts` | — | 0 | 否 | 同上 |
| `@ohos.distributedsched.proxyChannelManager.d.ts` | — | 0 | 否 | 同上 |
| `@ohos.enterprise.bluetoothManager.d.ts` | `BluetoothFullCoverageEnterprise01.test.ets` | 84 | 否 | **正/异/返**：14 企业管控 API × 6 场景；Admin 对象 stub；**组/性/压**：无 |
| `@ohos.enterprise.deviceSettings.d.ts` | — | 0 | 否 | 无 apitest |
| `@ohos.enterprise.restrictions.d.ts` | — | 0 | 否 | 无 apitest |
| `@ohos.geoLocationManager.d.ts` | — | 0 | 否 | 无 apitest |
| `@ohos.hidebug.d.ts` | — | 0 | 否 | 无 apitest |
| `@ohos.multimedia.audio.d.ts` | — | 0 | 否 | 无 apitest |
| `@ohos.multimedia.avsession.d.ts` | — | 0 | 否 | 无 apitest |
| `@ohos.multimodalAwareness.spatialAwareness.d.ts` | — | 0 | 否 | 无 apitest |
| `@ohos.multimodalInput.keyCode.d.ts` | — | 0 | 否 | 无 apitest |
| `@ohos.net.connection.d.ts` | — | 0 | 否 | 无 apitest |
| `@ohos.net.sharing.d.ts` | — | 0 | 否 | 无 apitest |
| `@ohos.notificationExtensionSubscription.d.ts` | — | 0 | 否 | 无 apitest |
| `@ohos.resourceschedule.backgroundTaskManager.d.ts` | — | 0 | 否 | 无 apitest |
| `@ohos.resourceschedule.workScheduler.d.ts` | — | 0 | 否 | 无 apitest |
| `@ohos.settings.d.ts` | — | 0 | 否 | 无 apitest |
| `@ohos.telephony.call.d.ts` | — | 0 | 否 | 无 apitest |
| `@system.bluetooth.d.ts` | `ConnectivityKitBluetooth.test.ets`；`BluetoothFullCoverageAccess01.test.ets` | 26 | 否 | **正/异/返/组**：FullCoverage 4 API × 6 场景（`StartBLEScanOptions`/`StopBLEScanOptions`/`SubscribeBLEFoundOptions` stub）；CK 2 条 |

---

## 测试设计补强记录（相对 APITEST_COVERAGE.md 文首标准）

以下在 **`apitest`** 中落地，用于向「参数 / 返回值 / 组合 / 正常 / 异常 / 性能 / 压力」靠拢；**仍不等于**声明文件级「完全覆盖」。

| 说明 | 内容 |
|------|------|
| 辅助模块 | `apitest/ApitestCoverageHelpers.ets`：`assertPerfBudget`（总耗时 ≤ 预算 ms）、`assertStressHasSuccess`（迭代内至少一次成功调用） |
| `ConnectivityKitBluetooth.test.ets` | `constant`：`ProfileId` / `ProfileConnectionState` 枚举取值补全；`common`：`BluetoothAddressType.REAL`；`baseProfile`：`DisconnectCause` 全量取值断言；`access`：`convertUuid('')`、非法 MAC 形态；`connection`：按 `ProfileId` 矩阵调用 `getProfileConnectionState`、非法 profile 边界；`socket`：补充 `SppType.SPP_L2CAP`；`wearDetection` / `@system.bluetooth`：回调标记用例；**`ConnectivityKit_design_perf`** / **`ConnectivityKit_design_stress`**：性能与压力循环 |
| `BluetoothApiCoverage.test.ets` | `@ohos.bluetooth`：`getProfileConnState` 对 HID/PAN profile、非法 profile；**`BluetoothApiCoverage_design_perf`** / **`BluetoothApiCoverage_design_stress`**：`getState` / `getPairedDevices` 性能预算，`getBtConnectionState` / `BLE.stopBLEScan` 压力迭代 |
| 扩展矩阵（多文件） | **540** 条 `it`：`BluetoothExpandedMatrixConnection01/02.test`（各 130 条 CK `getProfileConnectionState`）+ `BluetoothExpandedMatrixLegacy01/02.test`（各 130 条 `@ohos.bluetooth.getProfileConnState`）+ `BluetoothExpandedMatrixRotate.test`（20 条）+ `BluetoothExpandedMatrixShared.ets`；单文件少于 **1500** 行；由 `registerApitestSuite` 注册 |
| **FullCoverage 全量补强（2026-07）** | **1716** 条 `it`：`BluetoothFullCoverageAccess01`（150）+ `Legacy01`（90）+ `Legacy02`（282）+ `Profile01`（126）+ `Profile02`（234）+ `Gatt01`（78）+ `Gatt02`（204）+ `Enterprise01`（132）+ `Combo01`（210）+ `Combo02`（210）；辅助 `ApitestFullCoverageShared.ets`；生成脚本 `scripts/gen_bluetooth_full_coverage_tests.py`；每 API **6 场景**：`n` 正常 / `e` 异常 / `b` 边界 / `r` 返回值 / `x` 重复 / `c` 组合；`ohosTest` 编译 **BUILD SUCCESSFUL** |
| 未改 | `ConnBleSocketMass01`～`Mass04` 仍为生成/大体量套件；若需同等级「设计标准」需单独评审生成脚本或拆分模块 |

---

## 按测试类型矩阵（独立表格）

与上文「对照表」**并列**，不追加列。第一列为 `jsapi/26` 下 **API 声明文件名**；其余列为测试类型。取值：**无**（无对应用例或该维度未体现）、**部分**（有非系统化验证）、**是**（满足文档开头「完全覆盖」中对该维度的要求；当前仓库一般为空）。

| API 文件名 | 参数覆盖 | 返回值 / 回调覆盖 | 调用组合覆盖 | 正常流程覆盖 | 异常流程覆盖 | 性能覆盖 | 压力覆盖 |
|------------|----------|-------------------|--------------|--------------|--------------|----------|----------|
| `2in1.json` | 不适用 | 不适用 | 不适用 | 不适用 | 不适用 | 不适用 | 不适用 |
| `default.json` | 不适用 | 不适用 | 不适用 | 不适用 | 不适用 | 不适用 | 不适用 |
| `tablet.json` | 不适用 | 不适用 | 不适用 | 不适用 | 不适用 | 不适用 | 不适用 |
| `tv.json` | 不适用 | 不适用 | 不适用 | 不适用 | 不适用 | 不适用 | 不适用 |
| `wearable.json` | 不适用 | 不适用 | 不适用 | 不适用 | 不适用 | 不适用 | 不适用 |
| `permissions.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.FusionConnectivity.partnerAgent.d.ts` | 部分 | 部分 | 无 | 部分 | 部分 | 无 | 无 |
| `@ohos.backgroundTaskManager.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.batteryStatistics.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.bluetooth.a2dp.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 无 | 无 |
| `@ohos.bluetooth.access.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 部分 | 无 |
| `@ohos.bluetooth.baseProfile.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 无 | 无 |
| `@ohos.bluetooth.ble.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 部分 | 部分 |
| `@ohos.bluetooth.common.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 无 | 无 |
| `@ohos.bluetooth.connection.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 无 | 部分 |
| `@ohos.bluetooth.constant.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 无 | 无 |
| `@ohos.bluetooth.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 部分 | 部分 |
| `@ohos.bluetooth.hfp.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 无 | 无 |
| `@ohos.bluetooth.hid.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 无 | 无 |
| `@ohos.bluetooth.map.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 无 | 无 |
| `@ohos.bluetooth.opp.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 无 | 无 |
| `@ohos.bluetooth.pan.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 无 | 无 |
| `@ohos.bluetooth.pbap.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 无 | 无 |
| `@ohos.bluetooth.socket.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 无 | 无 |
| `@ohos.bluetooth.wearDetection.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 无 | 无 |
| `@ohos.bluetoothManager.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 部分 | 无 |
| `@ohos.commonEvent.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.commonEventManager.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.distributedDeviceManager.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.distributedHardware.deviceManager.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.distributedsched.abilityConnectionManager.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.distributedsched.proxyChannelManager.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.enterprise.bluetoothManager.d.ts` | 部分 | 部分 | 无 | 部分 | 部分 | 无 | 无 |
| `@ohos.enterprise.deviceSettings.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.enterprise.restrictions.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.geoLocationManager.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.hidebug.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.multimedia.audio.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.multimedia.avsession.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.multimodalAwareness.spatialAwareness.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.multimodalInput.keyCode.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.net.connection.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.net.sharing.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.notificationExtensionSubscription.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.resourceschedule.backgroundTaskManager.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.resourceschedule.workScheduler.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.settings.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@ohos.telephony.call.d.ts` | 无 | 无 | 无 | 无 | 无 | 无 | 无 |
| `@system.bluetooth.d.ts` | 部分 | 部分 | 部分 | 部分 | 部分 | 无 | 无 |

## 补充

1. **`ConnBleSocketMass01`～`Mass04` 的 570** 会同时计入 **connection / ble / socket / constant / common** 五行；按「声明模块归属」统计，**不是**全仓库 `it` 去重后的条数。全量 `apitest` 设备报告含 **`BluetoothFullCoverage*.test.ets` 1716 条** 后显著增加（与 Mass/ExpandedMatrix 叠加计数方式相同）。
2. **`ConnBleSocketShared.ets`** 为辅助模块，无 `it`，未列入第二列。
3. **`apitest` 相对路径**：`../../apitest/`（相对本文件所在 `jsapi/26`）。
4. 若后续某模块达到「完全覆盖」标准，将 **是否完全覆盖** 改为 **是**，并在 **覆盖维度说明** 中给出可追溯用例文件与指标说明（性能阈值、压测次数等）。
5. **「按测试类型矩阵」** 与第一张对照表独立；产品形态 JSON 无 JS API 语义，测试类型列填 **不适用**。矩阵中 **是** 表示该维度已达文首严格定义，当前多为 **无** / **部分**。
