# Bluetooth Scenario 场景用例设计说明（已落地实现）

> 本文档描述 **已生成并上板验证** 的 `BluetoothScenario*` Hypium 场景套件，与旧稿 `APITEST_SCENARIO.md`（组合空间设计稿 v3，约 8.7 万条规划）区分开。  
> 统计口径：`entry/src/ohosTest/ets/apitest/BluetoothScenario*.test.ets` 中的 `it()`。

## 1. 总量结论

| 范围 | 套件数 | case 数 | 入口 List |
|------|--------|---------|----------|
| **全量场景** | 14 | **246** | `List.apitest.scenario.test.ets` |
| **其中 core（当前默认上板）** | 11 | **194** | `List.apitest.scenario.core.test.ets` |
| profiles 分段 | 1 | 25 | `List.apitest.scenario.profiles.test.ets` |
| legacy 分段 | 1 | 15 | `List.apitest.scenario.legacy.test.ets` |
| extra 分段 | 1 | 12 | `List.apitest.scenario.extra.test.ets` |

**上板结论（API 23 设备，授 BT/Location 权限后）**：`scenario.core` **194 / Pass 194 / Fail 0**（`-s timeout 45000`）。

### 分套件计数

| 分段 | 套件文件 | case |
|------|----------|------|
| core | `BluetoothScenarioPair01` | 32 |
| core | `BluetoothScenarioTriple01` | 20 |
| core | `BluetoothScenarioQuad01` | 17 |
| core | `BluetoothScenarioScanParams01` | 35 |
| core | `BluetoothScenarioNameParams01` | 21 |
| core | `BluetoothScenarioMultiInstance01` | 13 |
| core | `BluetoothScenarioConn02` | 15 |
| core | `BluetoothScenarioBleAdvGatt02` | 17 |
| core | `BluetoothScenarioSocket01` | 11 |
| core | `BluetoothScenarioCross01` | 8 |
| core | `BluetoothScenarioMultiModule01` | 5 |
| profiles | `BluetoothScenarioProfileBase01` | 25 |
| legacy | `BluetoothScenarioLegacy01` | 15 |
| extra | `BluetoothScenarioExtra01` | 12 |
| **合计** | | **246** |

## 2. 是否都有明确流程与测试目标？

**是。** 落地规则如下（生成器强制）：

1. **每条 case 都有 `@tc.name` + `@tc.desc`**：desc 写清 N-way 步骤与期望结果（例如 `enable → setLocalName → getLocalName == want`）。
2. **流程多步、可观测**：不是单点空调；典型形态为  
   `前置(enable/disable) → 动作 API → 观测(get*/回调/扫描命中) → 清理(stop/off/restore)`。
3. **禁止恒真断言**：用 `assertLocalOk` / `scAssertStateOn|Off` / `scAssertNameRoundTrip` / `scAssertAmbientFindHit` 等结果断言；扫描类必须有等待窗口。
4. **前后置恢复**：各套件 `beforeEach` / `afterEach` 调用 `ensureBtOn`，并 `scSafeStop*`（扫描/发现/广播），保证 1A 单板可连续跑。
5. **设计维度 1A**：单板、无对端真机依赖；peer 类 API（假 MAC）走 `assertPeerExpect` 白名单。

因此 246 条均可对照「目标 → 步骤 → 断言」阅读；下文章节 6 是完整清单。

## 3. 设计原则（1A 场景）

| 原则 | 说明 |
|------|------|
| 单板可跑 | 不依赖第二台设备配对成功；连接类断言 DISCONNECTED / 文档错误码 |
| 组合分层 | 2-way（Pair）→ 3-way（Triple）→ 4-way（Quad）→ 参数矩阵 → 跨模块 |
| 正反向 | BT ON 可做 / BT OFF 不可做（改名、扫描、广播）成对覆盖 |
| 次序敏感 | `off→on` 与 `on→off` 分 case |
| 真实射频 | 环境发现：经典 `startBluetoothDiscovery` + BLE `startBLEScan`，断言至少一侧命中 |
| 状态机宽松 | 接受 `TURNING_ON/OFF`、`BLE_TURNING_*`；关蓝牙后本地名允许空串 |
| 能力差异 | 无车钥匙等特性：`false` / 空串 / `2900100` 可接受 |

共享实现：`ApitestScenarioShared.ets`、`ApitestRealAssert.ets`。

## 4. 生成与入口

| 产物 | 生成脚本 |
|------|----------|
| Pair / Triple / Quad / Scan / Name / MultiInstance | `skills/ohtest/bluetooth_scenario_flow.py` |
| Conn02 / BleAdvGatt / Socket / Profile / Legacy / Extra / Cross / MultiModule | `skills/ohtest/bluetooth_scenario_modules.py` |

```bash
python3 skills/ohtest/bluetooth_scenario_flow.py
python3 skills/ohtest/bluetooth_scenario_modules.py
```

`TestAbility.ets` 默认指向 `List.apitest.scenario.core.test`（194）。全量 246 改为：

```ts
import testsuite from '../apitest/List.apitest.scenario.test';
```

## 5. 典型流程模板

### 5.1 2-way 正向（BT ON）

```
enableBluetooth
→ scAssertStateOn (ON|TURNING_ON)
→ 目标 API（合法参数）
→ 结果断言（err∈白名单 / 返回值 / 扫描命中）
→ stop / restoreLocalName
```

### 5.2 2-way 反向（BT OFF）

```
disableBluetooth
→ scAssertStateOff (OFF|TURNING_OFF)
→ 目标 API
→ scAssertLocalFail / scAssertNameCannotSet / scAssertBleCannotScan
→ enable 恢复
```

### 5.3 环境扫描（可收数）

```
经典：stop 旧 inquiry → on(bluetoothDeviceFind) → startBluetoothDiscovery → 等 ~10s
BLE：on(BLEDeviceFind) → startBLEScan(null, LOW_LATENCY+AGGRESSIVE) → 等 ~8s
→ scAssertAmbientFindHit(bleHits, classicIds)  // 至少一侧非空
```

### 5.4 跨模块

```
enable → 模块A API → 模块B API → 清理
例：sppListen → startAdvertising → stopAdvertising
```

## 6. 全量 case 清单（246）

### Pair01（`BluetoothScenarioPair01.test.ets`，32）

**套件目标**：2-way 配对流程：enable/disable × 本地查询/扫描/改名

| # | case 名 | 测试目标 / 流程摘要 |
|---|---------|-------------------|
| 1 | `pair_enable_getState` | 2-way: enableBluetooth then getState == STATE_ON |
| 2 | `pair_enable_getLocalName` | 2-way: enable → setLocalName → getLocalName == want |
| 3 | `pair_enable_getPairedDevices` | 2-way: enableBluetooth then getPairedDevices |
| 4 | `pair_enable_getProfileConnectionState` | 2-way: enableBluetooth then getProfileConnectionState == DISCONNECTED |
| 5 | `pair_enable_getBluetoothScanMode` | 2-way: enableBluetooth then getBluetoothScanMode in ScanMode range |
| 6 | `pair_enable_stateChange_on` | 2-way: enableBluetooth then on(stateChange) |
| 7 | `pair_enable_startStopDiscovery` | 2-way: enableBluetooth then start/stopBluetoothDiscovery + discovering true/false |
| 8 | `pair_enable_startStopBleScan` | 2-way: enable → startBLEScan listen hit → stop |
| 9 | `pair_enable_createGattServer` | 2-way: enableBluetooth then createGattServer+close |
| 10 | `pair_enable_getRemoteDeviceName_fake` | 2-way: enableBluetooth then getRemoteDeviceName fake mac (peer path) |
| 11 | `pair_enable_getConnectedBLEDevices` | 2-way: enableBluetooth then getConnectedBLEDevices |
| 12 | `pair_enable_getState_async_strict` | 2-way async: enableBluetooth then wait and assert valid state |
| 13 | `pair_enable_profileConn_a2dp` | 2-way: enable then getProfileConnectionState(a2dp) == DISCONNECTED |
| 14 | `pair_enable_profileConn_hfp` | 2-way: enable then getProfileConnectionState(hfp) == DISCONNECTED |
| 15 | `pair_enable_profileConn_hid` | 2-way: enable then getProfileConnectionState(hid) == DISCONNECTED |
| 16 | `pair_enable_profileConn_pan` | 2-way: enable then getProfileConnectionState(pan) == DISCONNECTED |
| 17 | `pair_setLocalName_getLocalName` | 2-way: setLocalName then getLocalName round-trip |
| 18 | `pair_setGetScanMode_none` | 2-way: setBluetoothScanMode(none) then getBluetoothScanMode == set |
| 19 | `pair_setGetScanMode_connectable` | 2-way: setBluetoothScanMode(connectable) then getBluetoothScanMode == set |
| 20 | `pair_setGetScanMode_general` | 2-way: setBluetoothScanMode(general) then getBluetoothScanMode == set |
| 21 | `pair_setGetScanMode_conn_general` | 2-way: setBluetoothScanMode(conn_general) then getBluetoothScanMode == set |
| 22 | `pair_disable_getState` | 2-way: disableBluetooth then getState |
| 23 | `pair_disable_getLocalName` | 2-way: setLocalName while ON → disable → getLocalName empty or want |
| 24 | `pair_disable_getPairedDevices` | 2-way: disableBluetooth then getPairedDevices |
| 25 | `pair_disable_startDiscovery` | 2-way: disableBluetooth then startBluetoothDiscovery expect fail/local |
| 26 | `pair_disable_startBleScan` | 2-way: disableBluetooth then startBLEScan cannot scan (fail or miss) |
| 27 | `pair_disable_createGattServer` | 2-way: disableBluetooth then createGattServer |
| 28 | `pair_name_can_set` | 2-way: BT ON setLocalName succeeds and getLocalName matches |
| 29 | `pair_name_cannot_set_bt_off` | 2-way: BT OFF setLocalName must fail or leave name unchanged |
| 30 | `pair_order_off_then_on` | 2-way order: disable then enable → ON\|TURNING_ON |
| 31 | `pair_order_on_then_off` | 2-way order: enable then disable → OFF\|TURNING_OFF |
| 32 | `pair_enable_getState_x3` | 2-way repeated: enable+getState x3 |

### Triple01（`BluetoothScenarioTriple01.test.ets`，20）

**套件目标**：3-way 组合：电源 × 扫描/改名/profile

| # | case 名 | 测试目标 / 流程摘要 |
|---|---------|-------------------|
| 1 | `triple_enable_getState_getLocalName` | 3-way: enable -> getState -> set/getLocalName == want |
| 2 | `triple_enable_getState_getPairedDevices` | 3-way: enable -> getState -> getPairedDevices |
| 3 | `triple_enable_getState_getProfileConn` | 3-way: enable -> getState -> getProfileConnectionState |
| 4 | `triple_enable_onState_getState` | 3-way: on(stateChange) -> enable -> getState |
| 5 | `triple_enable_setName_getName` | 3-way: enable -> setLocalName -> getLocalName |
| 6 | `triple_enable_setScan_getScan` | 3-way: enable -> setBluetoothScanMode -> getBluetoothScanMode |
| 7 | `triple_enable_discovery_stop` | 3-way: enable -> startBluetoothDiscovery -> stopBluetoothDiscovery |
| 8 | `triple_enable_bleScan_stop` | 3-way: enable -> startBLEScan listen hit -> stop |
| 9 | `triple_enable_gatt_close` | 3-way: enable -> createGattServer -> close |
| 10 | `triple_enable_paired_remoteName` | 3-way: enable -> getPairedDevices -> getRemoteDeviceName first/fake |
| 11 | `triple_enable_getState_createBleScanner` | 3-way: enable -> getState -> createBleScanner |
| 12 | `triple_disable_enable_getState` | 3-way: disable -> enable -> getState ON\|TURNING_ON |
| 13 | `triple_enable_disable_getState` | 3-way: enable -> disable -> getState OFF\|TURNING_OFF |
| 14 | `triple_disable_getState_enable` | 3-way: disable -> getState -> enable restore |
| 15 | `triple_setName_getName_getPaired` | 3-way: setLocalName -> getLocalName -> getPairedDevices |
| 16 | `triple_enable_onFind_startDiscovery` | 3-way: enable -> on(bluetoothDeviceFind) -> startDiscovery |
| 17 | `triple_enable_state_profile_a2dp` | 3-way: enable -> getState -> profileConn a2dp |
| 18 | `triple_enable_state_profile_hfp` | 3-way: enable -> getState -> profileConn hfp |
| 19 | `triple_enable_state_profile_hid` | 3-way: enable -> getState -> profileConn hid |
| 20 | `triple_enable_bleScan_getState_x2` | 3-way repeated: enable -> bleScan -> getState x2 |

### Quad01（`BluetoothScenarioQuad01.test.ets`，17）

**套件目标**：4-way 组合：电源循环 × 本地能力

| # | case 名 | 测试目标 / 流程摘要 |
|---|---------|-------------------|
| 1 | `quad_enable_on_getState_getLocalName` | 4-way: on(stateChange) -> enable -> getState -> set/getLocalName |
| 2 | `quad_enable_state_name_paired` | 4-way: enable -> getState -> set/getLocalName -> getPairedDevices |
| 3 | `quad_enable_state_profile_scanMode` | 4-way: enable -> getState -> getProfileConnectionState -> getBluetoothScanMode |
| 4 | `quad_enable_setName_getName_getPaired_getState` | 4-way: enable -> setLocalName -> getLocalName -> getPairedDevices -> getState |
| 5 | `quad_enable_discovery_find_stop_state` | 4-way: enable -> on(find) -> startDiscovery -> stop -> getState |
| 6 | `quad_enable_scanOpt_start_stop_state` | 4-way: enable -> configure ScanOptions -> startBLEScan -> stop -> getState |
| 7 | `quad_enable_gatt_scanner_state_name` | 4-way: enable -> createGattServer -> createBleScanner -> getState -> getLocalName |
| 8 | `quad_off_on_off_on` | 4-way power: disable -> enable -> disable -> enable |
| 9 | `quad_on_off_on_getName` | 4-way power: enable -> disable -> enable -> getLocalName |
| 10 | `quad_enable_setScan_getScan_paired_state` | 4-way: enable -> setScanMode -> getScanMode -> getPairedDevices -> getState |
| 11 | `quad_enable_filter_scan_stop_paired` | 4-way: enable -> filter+options scan -> stop -> getPairedDevices |
| 12 | `quad_chain_local_queries` | 4-way local query chain after enable |
| 13 | `quad_enable_state_profile_a2dp_name_paired` | 4-way: enable -> state -> profile a2dp -> name -> paired |
| 14 | `quad_enable_state_profile_hfp_name_paired` | 4-way: enable -> state -> profile hfp -> name -> paired |
| 15 | `quad_enable_state_profile_hid_name_paired` | 4-way: enable -> state -> profile hid -> name -> paired |
| 16 | `quad_enable_state_profile_pan_name_paired` | 4-way: enable -> state -> profile pan -> name -> paired |
| 17 | `quad_power_cycle_x2` | 4-way repeated power cycle x2 then verify ON\|TURNING_ON |

### ScanParams01（`BluetoothScenarioScanParams01.test.ets`，35）

**套件目标**：扫描参数矩阵：duty/filter/空滤波/关蓝牙不可扫

| # | case 名 | 测试目标 / 流程摘要 |
|---|---------|-------------------|
| 1 | `scan_can_receive_empty` | enable → advertise → BleScanner empty filter → listen hit |
| 2 | `scan_cannot_receive_miss_uuid` | enable → dead UUID filter → listen miss (cannot match) |
| 3 | `scan_param_duty_low_power` | enable + dutyMode=low_power → listen BLEDeviceFind hit |
| 4 | `scan_param_duty_balanced` | enable + dutyMode=balanced → listen BLEDeviceFind hit |
| 5 | `scan_param_duty_low_latency` | enable + dutyMode=low_latency → listen BLEDeviceFind hit |
| 6 | `scan_param_interval_0` | enable + ScanOptions.interval=0 + startBLEScan |
| 7 | `scan_param_interval_100` | enable + ScanOptions.interval=100 + startBLEScan |
| 8 | `scan_param_interval_1000` | enable + ScanOptions.interval=1000 + startBLEScan |
| 9 | `scan_param_interval_neg1` | enable + ScanOptions.interval=neg1 + startBLEScan |
| 10 | `scan_param_interval_maxish` | enable + ScanOptions.interval=maxish + startBLEScan |
| 11 | `scan_param_match_aggressive` | enable + matchMode=aggressive + start/stopBLEScan |
| 12 | `scan_param_match_sticky` | enable + matchMode=sticky + start/stopBLEScan |
| 13 | `scan_param_phy_1m` | enable + phyType=1m + startBLEScan |
| 14 | `scan_param_phy_all` | enable + phyType=all + startBLEScan |
| 15 | `scan_param_report_normal` | enable + reportMode=normal + startBLEScan |
| 16 | `scan_param_report_batch` | enable + reportMode=batch + startBLEScan |
| 17 | `scan_param_filter_empty` | enable + ScanFilter=empty + startBLEScan |
| 18 | `scan_param_filter_name_short` | enable + ScanFilter=name_short + startBLEScan |
| 19 | `scan_param_filter_name_long` | enable + ScanFilter=name_long + startBLEScan |
| 20 | `scan_param_filter_uuid` | enable + ScanFilter=uuid + startBLEScan |
| 21 | `scan_param_filter_mfg0` | enable + ScanFilter=mfg0 + startBLEScan |
| 22 | `scan_param_filter_mfg_apple` | enable + ScanFilter=mfg_apple + startBLEScan |
| 23 | `scan_param_filter_combo` | enable + ScanFilter=combo + startBLEScan |
| 24 | `scan_param_filter_bad_uuid` | enable + ScanFilter=bad_uuid + startBLEScan |
| 25 | `scan_combo3_duty_lp_match_agg` | 3-param combo: duty=lp match=agg + filter name |
| 26 | `scan_combo3_duty_lp_match_stk` | 3-param combo: duty=lp match=stk + filter name |
| 27 | `scan_combo3_duty_bal_match_agg` | 3-param combo: duty=bal match=agg + filter name |
| 28 | `scan_combo3_duty_bal_match_stk` | 3-param combo: duty=bal match=stk + filter name |
| 29 | `scan_combo3_duty_ll_match_agg` | 3-param combo: duty=ll match=agg + filter name |
| 30 | `scan_combo3_duty_ll_match_stk` | 3-param combo: duty=ll match=stk + filter name |
| 31 | `scan_combo4_opts_lp` | 4-param ScanOptions combo duty=lp + uuid filter + paired |
| 32 | `scan_combo4_opts_bal` | 4-param ScanOptions combo duty=bal + uuid filter + paired |
| 33 | `scan_combo4_opts_ll` | 4-param ScanOptions combo duty=ll + uuid filter + paired |
| 34 | `scan_after_disable` | disable → startBLEScan → fail or listen miss (cannot scan) |
| 35 | `scan_start_twice` | enable + startBLEScan twice (multi start) then stop |

### NameParams01（`BluetoothScenarioNameParams01.test.ets`，21）

**套件目标**：本地名参数：边界串、重复设置、关蓝牙不可设

| # | case 名 | 测试目标 / 流程摘要 |
|---|---------|-------------------|
| 1 | `name_param_len1` | enable + setLocalName(len1) + getLocalName round-trip |
| 2 | `name_param_ascii` | enable + setLocalName(ascii) + getLocalName round-trip |
| 3 | `name_param_zh` | enable + setLocalName(zh) + getLocalName round-trip |
| 4 | `name_param_alnum` | enable + setLocalName(alnum) + getLocalName round-trip |
| 5 | `name_param_len31` | enable + setLocalName(len31) + getLocalName round-trip |
| 6 | `name_param_len32` | enable + setLocalName(len32) + getLocalName round-trip |
| 7 | `name_param_len64` | enable + setLocalName(len64) + getLocalName round-trip |
| 8 | `name_param_len248` | enable + setLocalName(len248) + getLocalName round-trip |
| 9 | `name_param_len249` | enable + setLocalName(len249) + getLocalName round-trip |
| 10 | `name_param_space` | enable + setLocalName(space) + getLocalName round-trip |
| 11 | `name_param_spaces` | enable + setLocalName(spaces) + getLocalName round-trip |
| 12 | `name_param_special` | enable + setLocalName(special) + getLocalName round-trip |
| 13 | `name_param_empty` | enable + setLocalName(empty) + getLocalName round-trip |
| 14 | `name_param_dash` | enable + setLocalName(dash) + getLocalName round-trip |
| 15 | `name_triple_short_paired` | 3-way: setLocalName(short) -> getLocalName -> getPairedDevices |
| 16 | `name_triple_mid_paired` | 3-way: setLocalName(mid) -> getLocalName -> getPairedDevices |
| 17 | `name_triple_zh2_paired` | 3-way: setLocalName(zh2) -> getLocalName -> getPairedDevices |
| 18 | `name_quad_set_get_scan_state` | 4-way: setLocalName -> getLocalName -> getBluetoothScanMode -> getState |
| 19 | `name_set_after_disable` | disable then setLocalName must fail or leave name unchanged |
| 20 | `name_set_twice` | setLocalName A then B then verify getLocalName |
| 21 | `name_fixtures_loop` | enable then walk scNameFixtures set/get (multi param) |

### MultiInstance01（`BluetoothScenarioMultiInstance01.test.ets`，13）

**套件目标**：多实例：重复 enable/扫描/GattClient

| # | case 名 | 测试目标 / 流程摘要 |
|---|---------|-------------------|
| 1 | `multi_gattServer_x2` | create 2 GattServers then close all |
| 2 | `multi_gattServer_x3` | create 3 GattServers then close all |
| 3 | `multi_gattServer_x5` | create 5 GattServers then close all |
| 4 | `multi_bleScanner_x2` | create 2 BleScanners |
| 5 | `multi_bleScanner_x3` | create 3 BleScanners |
| 6 | `multi_bleScanner_x5` | create 5 BleScanners |
| 7 | `multi_discovery_start_twice` | startBluetoothDiscovery twice then stop |
| 8 | `multi_discovery_listeners_x3` | register 3 bluetoothDeviceFind listeners + discovery |
| 9 | `multi_stateChange_listeners_x3` | register 3 stateChange listeners around enable |
| 10 | `multi_gatt_and_scanner_mix` | create 2 GattServers + 2 BleScanners together |
| 11 | `multi_scan_then_discovery` | BLE scan + classic discovery concurrently then stop both |
| 12 | `multi_profile_query_all` | query all ProfileId connection states after enable |
| 13 | `multi_create_close_gatt_loop` | create/close GattServer in loop x5 |

### Conn02（`BluetoothScenarioConn02.test.ets`，15）

**套件目标**：connection 模块 L1–L2 本地 API 场景

| # | case 名 | 测试目标 / 流程摘要 |
|---|---------|-------------------|
| 1 | `conn2_enable_isBluetoothDiscovering` | L1: enable → connection.isBluetoothDiscovering |
| 2 | `conn2_enable_getLocalProfileUuids` | L1: enable → connection.getLocalProfileUuids |
| 3 | `conn2_enable_generateLocalOob_br` | L1: enable → connection.generateLocalOob_br |
| 4 | `conn2_enable_generateLocalOob_le` | L1: enable → connection.generateLocalOob_le |
| 5 | `conn2_enable_on_bondStateChange` | L1: enable → connection.on_bondStateChange |
| 6 | `conn2_enable_on_pinRequired` | L1: enable → connection.on_pinRequired |
| 7 | `conn2_enable_on_discoveryResult` | L1: enable → connection.on_discoveryResult |
| 8 | `conn2_enable_on_batteryChange` | L1: enable → connection.on_batteryChange |
| 9 | `conn2_enable_onScanModeChange` | L1: enable → connection.onScanModeChange |
| 10 | `conn2_enable_getVirtualAddressByHash` | L1: enable → connection.getVirtualAddressByHash |
| 11 | `conn2_enable_getCarKeyDfxData` | L1: enable → connection.getCarKeyDfxData |
| 12 | `conn2_triple_name_scan_discovery` | L2: enable → setLocalName → setScanMode → start/stopDiscovery |
| 13 | `conn2_quad_discovery_find_paired_mode` | L2: enable → on(find) → startDiscovery → getPairedDevices → getScanMode → stop/off |
| 14 | `conn2_disable_then_discovery` | L1: disable → startBluetoothDiscovery (local err path) |
| 15 | `conn2_enable_setRemoteDeviceName_peer` | L1: enable → setRemoteDeviceName fake (peer) |

### BleAdvGatt02（`BluetoothScenarioBleAdvGatt02.test.ets`，17）

**套件目标**：BLE 广播/扫描/Gatt + 环境设备发现

| # | case 名 | 测试目标 / 流程摘要 |
|---|---------|-------------------|
| 1 | `ble2_adv_param_default` | L1/L2: startAdvertising(default) → listen BLEDeviceFind hit → stop |
| 2 | `ble2_adv_param_fast` | L1/L2: startAdvertising(fast) → listen BLEDeviceFind hit → stop |
| 3 | `ble2_adv_param_slow` | L1/L2: startAdvertising(slow) → listen BLEDeviceFind hit → stop |
| 4 | `ble2_adv_param_mid` | L1/L2: startAdvertising(mid) → listen BLEDeviceFind hit → stop |
| 5 | `ble2_adv_params_object` | L1: startAdvertising(AdvertisingParams) → listen scan hit → stop |
| 6 | `ble2_scan_can_receive` | L2: enable → advertise → BleScanner listen hit (can scan) |
| 7 | `ble2_scan_cannot_bt_off` | L2: disable → BleScanner.startScan → fail or listen miss (cannot scan) |
| 8 | `ble2_scan_cannot_miss_filter` | L2: enable → filter dead UUID → listen miss (cannot match) |
| 9 | `ble2_gatt_add_remove_service` | L2: enable → createGattServer → addService → removeService → close |
| 10 | `ble2_gatt_on_characteristicWrite` | L2: enable → createGattServer → on(characteristicWrite) → off → close |
| 11 | `ble2_scanner_start_stop` | L2: BleScanner startScan API (801=unsupported on some images) |
| 12 | `ble2_scanner_duty_lp` | L2: BleScanner duty=lp (801=unsupported ok) |
| 13 | `ble2_scanner_duty_bal` | L2: BleScanner duty=bal (801=unsupported ok) |
| 14 | `ble2_scanner_duty_ll` | L2: BleScanner duty=ll (801=unsupported ok) |
| 15 | `ble2_createGattClient_peer` | L1: enable → createGattClientDevice(fake) peer path |
| 16 | `ble2_quad_adv_scan_state` | L2: startAdvertising → startBLEScan → listen hit → STATE_ON |
| 17 | `ble2_disable_startAdv` | L1: disable → startAdvertising must fail (cannot advertise) |

### Socket01（`BluetoothScenarioSocket01.test.ets`，11）

**套件目标**：SPP listen 参数与关蓝牙路径

| # | case 名 | 测试目标 / 流程摘要 |
|---|---------|-------------------|
| 1 | `sock_enable_sppListen_insecure` | L1: enable → socket.sppListen(insecure) |
| 2 | `sock_enable_sppListen_secure` | L1: enable → socket.sppListen(secure) |
| 3 | `sock_sppListen_uuid_variants` | L2: enable → sppListen with sppOpt / custom name |
| 4 | `sock_enable_getDeviceId` | L1: enable → socket.getDeviceId (no live socket → peer/local) |
| 5 | `sock_enable_getL2capPsm` | L1: enable → socket.getL2capPsm (no live socket → peer/local) |
| 6 | `sock_enable_getMaxReceiveDataSize` | L1: enable → socket.getMaxReceiveDataSize (no live socket → peer/local) |
| 7 | `sock_enable_getMaxTransmitDataSize` | L1: enable → socket.getMaxTransmitDataSize (no live socket → peer/local) |
| 8 | `sock_enable_isConnected` | L1: enable → socket.isConnected (no live socket → peer/local) |
| 9 | `sock_on_off_sppRead` | L2: enable → on(sppRead) → off(sppRead) |
| 10 | `sock_triple_listen_on_off` | L2: enable → sppListen → on(sppRead) → off |
| 11 | `sock_disable_sppListen` | L1: disable → sppListen |

### Cross01（`BluetoothScenarioCross01.test.ets`，8）

**套件目标**：跨模块串联：socket×ble、profile×conn、name×scan

| # | case 名 | 测试目标 / 流程摘要 |
|---|---------|-------------------|
| 1 | `cross_discovery_and_bleScan` | L3: enable → startDiscovery → startBLEScan → stop both → getPairedDevices |
| 2 | `cross_gatt_a2dp_profileConn` | L3: enable → createGattServer → createA2dpSrcProfile → getProfileConnectionState → close |
| 3 | `cross_sppListen_and_advertising` | L3: enable → sppListen → startAdvertising → stopAdvertising |
| 4 | `cross_a2dp_base_and_conn_profile` | L3: enable → a2dp.on(connectionStateChange) → getProfileConnectionState → off |
| 5 | `cross_name_scan_ble_adv` | L3: setLocalName → setScanMode → startAdvertising → listen scan hit → restore |
| 6 | `cross_disable_modules_then_enable` | L3: disable → ble scan cannot receive → conn/socket local path → enable |
| 7 | `cross_scanner_and_classic_discovery` | L3: enable → startBLEScan → startDiscovery → stop both |
| 8 | `cross_quad_local_query_mix` | L3: enable → getState → set/getLocalName → getProfileConnectionState → getConnectedBLEDevices |

### MultiModule01（`BluetoothScenarioMultiModule01.test.ets`，5）

**套件目标**：多模块压力：重复 listen/scan/gatt/adv

| # | case 名 | 测试目标 / 流程摘要 |
|---|---------|-------------------|
| 1 | `multi_a2dp_profile_x2` | L4: create 2 A2dpSrcProfile instances |
| 2 | `multi_sppListen_x3` | L4: sppListen three server names |
| 3 | `multi_bleScanner_startScan_x2` | L4: two BleScanners startScan then stop (801=unsupported ok) |
| 4 | `multi_legacy_and_modern_gatt` | L4: bluetooth.BLE.createGattServer + ble.createGattServer together |
| 5 | `multi_adv_and_scan_repeat` | L4: startBLEScan listen hit → stop (×2 rounds) |

### ProfileBase01（`BluetoothScenarioProfileBase01.test.ets`，25）

**套件目标**：A2DP/HFP/HID/PAN/OPP/MAP/PBAP baseProfile 场景

| # | case 名 | 测试目标 / 流程摘要 |
|---|---------|-------------------|
| 1 | `prof_enable_create_a2dp` | L1: enable → create profile a2dp |
| 2 | `prof_enable_create_hfp` | L1: enable → create profile hfp |
| 3 | `prof_enable_create_hid_host` | L1: enable → create profile hid_host |
| 4 | `prof_enable_create_hid_dev` | L1: enable → create profile hid_dev |
| 5 | `prof_enable_create_map` | L1: enable → create profile map |
| 6 | `prof_enable_create_opp` | L1: enable → create profile opp |
| 7 | `prof_enable_create_pan` | L1: enable → create profile pan |
| 8 | `prof_enable_create_pbap` | L1: enable → create profile pbap |
| 9 | `prof_base_on_connectionStateChange` | L2: enable → a2dp.create → on/off connectionStateChange (baseProfile) |
| 10 | `prof_a2dp_getPlayingState` | L1: enable → a2dp.getPlayingState(fake) peer/local |
| 11 | `prof_a2dp_isAbsoluteVolumeSupported` | L1: enable → a2dp.isAbsoluteVolumeSupported |
| 12 | `prof_a2dp_isAbsoluteVolumeEnabled` | L1: enable → a2dp.isAbsoluteVolumeEnabled |
| 13 | `prof_a2dp_getCurrentCodecInfo` | L1: enable → a2dp.getCurrentCodecInfo |
| 14 | `prof_a2dp_getCurrentFullCodecInfo` | L1: enable → a2dp.getCurrentFullCodecInfo |
| 15 | `prof_a2dp_getAutoPlayDisabledDuration` | L1: enable → a2dp.getAutoPlayDisabledDuration |
| 16 | `prof_pan_isTetheringOn` | L1: enable → pan.isTetheringOn |
| 17 | `prof_pan_setTethering_false` | L1: enable → pan.setTethering(false) |
| 18 | `prof_map_getMessageAccessAuthorization` | L1: enable → map.getMessageAccessAuthorization peer |
| 19 | `prof_pbap_getShareType` | L1: enable → pbap.getShareType peer |
| 20 | `prof_opp_on_transferStateChange` | L2: enable → opp.on/off transferStateChange |
| 21 | `prof_quad_a2dp_base_profileConn` | L2: enable → createA2dp → on(state) → getProfileConnectionState(A2DP) → off |
| 22 | `prof_read_ConnectionStrategy_enum` | L1: enable → read baseProfile.ConnectionStrategy enums |
| 23 | `prof_hfp_connect_peer` | L1: enable → hfp_connect peer |
| 24 | `prof_hid_connect_peer` | L1: enable → hid_connect peer |
| 25 | `prof_a2dp_disconnect_peer` | L1: enable → a2dp_disconnect peer |

### Legacy01（`BluetoothScenarioLegacy01.test.ets`，15）

**套件目标**：legacy bluetooth / bluetoothManager 场景

| # | case 名 | 测试目标 / 流程摘要 |
|---|---------|-------------------|
| 1 | `leg_bt_enable_getState` | L1: enable → bluetooth.getState |
| 2 | `leg_bt_enable_getLocalName` | L1: enable → bluetooth.getLocalName |
| 3 | `leg_bt_enable_getPairedDevices` | L1: enable → bluetooth.getPairedDevices |
| 4 | `leg_bt_enable_startBLEScan` | L1: enable → bluetooth.startBLEScan |
| 5 | `leg_bt_enable_createGattServer` | L1: enable → bluetooth.createGattServer |
| 6 | `leg_bt_enable_sppListen` | L1: enable → bluetooth.sppListen |
| 7 | `leg_bm_enable_getState` | L1: enable → bluetoothManager.getState |
| 8 | `leg_bm_enable_getLocalName` | L1: enable → bluetoothManager.getLocalName |
| 9 | `leg_bm_enable_getPairedDevices` | L1: enable → bluetoothManager.getPairedDevices |
| 10 | `leg_bm_enable_startBLEScan` | L1: enable → bluetoothManager.startBLEScan |
| 11 | `leg_bm_enable_createGattServer` | L1: enable → bluetoothManager.createGattServer |
| 12 | `leg_bm_enable_sppListen` | L1: enable → bluetoothManager.sppListen |
| 13 | `leg_bt_enableBluetooth_getState` | L1: bluetooth.enableBluetooth → getState == STATE_ON |
| 14 | `leg_bm_getBtConnectionState` | L1: enable → bluetoothManager.getBtConnectionState |
| 15 | `leg_quad_bt_scan_gatt` | L2: enable → bluetooth.BLE.startBLEScan → createGattServer → stop → close |

### Extra01（`BluetoothScenarioExtra01.test.ets`，12）

**套件目标**：enterprise / wear / partner / system.bluetooth

| # | case 名 | 测试目标 / 流程摘要 |
|---|---------|-------------------|
| 1 | `extra_wear_isWearDetectionSupported` | L1: enable → wearDetection.isWearDetectionSupported |
| 2 | `extra_wear_isWearDetectionEnabled` | L1: enable → wearDetection.isWearDetectionEnabled |
| 3 | `extra_wear_enableWearDetection` | L1: enable → wearDetection.enableWearDetection |
| 4 | `extra_wear_disableWearDetection` | L1: enable → wearDetection.disableWearDetection |
| 5 | `extra_partner_isSupported` | L1: enable → partnerAgent.isPartnerAgentSupported |
| 6 | `extra_partner_getBoundDevices` | L1: enable → partnerAgent.getBoundDevices |
| 7 | `extra_ent_isBluetoothDisabled` | L1: enable → enterprise.isBluetoothDisabled |
| 8 | `extra_ent_getBluetoothInfo` | L1: enable → enterprise.getBluetoothInfo |
| 9 | `extra_ent_getAllowedBluetoothDevices` | L1: enable → enterprise.getAllowedBluetoothDevices |
| 10 | `extra_sys_stub_options_and_ble_scan` | L1: enable → construct @system.bluetooth option stubs + ble.startBLEScan substitute |
| 11 | `extra_constant_ProfileId_in_flow` | L1: enable → use constant.ProfileId with getProfileConnectionState |
| 12 | `extra_common_BluetoothAddressType` | L1: enable → read common.BluetoothAddressType |

## 7. 上板与权限

```bash
# 编译签名（compatibleSdk 23）
python3 skills/ohhap/hapbuild.py build-test test/example/bluetoothtest
python3 skills/ohhap/hapbuild.py sign test/example/bluetoothtest

hdc install -r …/entry-default-signed.hap
hdc install -r …/entry-ohosTest-signed.hap

TID=$(hdc shell atm dump -t -b com.example.myapplicationbt | grep -oE '[0-9]{5,}' | head -1)
hdc shell atm perm -g -i $TID -p ohos.permission.ACCESS_BLUETOOTH
hdc shell atm perm -g -i $TID -p ohos.permission.LOCATION
hdc shell atm perm -g -i $TID -p ohos.permission.APPROXIMATELY_LOCATION

hdc shell "aa test -b com.example.myapplicationbt -m entry_test \
  -s unittest OpenHarmonyTestRunner -s timeout 45000"
```

## 8. 与设计稿 v3 的关系

| 文档 | 性质 | 规模 |
|------|------|------|
| [`APITEST_SCENARIO.md`](./APITEST_SCENARIO.md) | 组合空间规划（有序两两/三三理论全集） | ~87890（规划） |
| **本文** | **已实现、可上板** 的分层场景抽样 + 参数矩阵 | **246** |

落地策略：先用 Pair/Triple/Quad + 参数矩阵 + 跨模块代表路径覆盖主风险；全量笛卡尔积仍以 jsonl 设计稿为准，按需继续生成。

## 9. 维护注意

- 改断言/流程优先改 Shared 与生成器，再 regen，避免手改 `BluetoothScenario*.test.ets` 被覆盖。
- 新增 case 必须带 `@tc.desc` 多步说明，并落到对应 List。
- 广告资源耗尽（`2900010`）时 `stopAdvertising` 可能 `2902055`：start 失败则不再强断言 stop 成功。
