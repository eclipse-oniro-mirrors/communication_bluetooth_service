# Bluetooth Scenario 全量交叉用例设计（设计稿 v3 · 两两/次序/多路）

> **已落地实现说明**：当前仓库可上板的场景套件共 **246** 条（core **194**），详见 [`APITEST_SCENARIO_CASES.md`](./APITEST_SCENARIO_CASES.md)。下文为组合空间设计稿 v3，不是已实现清单。  
> **机读清单**：`apitest_scenario_design/*.jsonl` 体积过大（单文件可超 10MiB），**不入库**；需要时本地用 `skills/ohtest/gen_scenario_full_design.py` 重新生成。


> **纠偏**：v2 的 ~1167 条只是「每接口单独串 enable」级别，**远不够**两两交叉。  
> 本地可执行接口 **263**（catalog 本地合计 291，含 enum/interface；全量 catalog **388**）。  
> 仅 **模块内有序两两** 已是 **7430**；**跨模块有序两两** **61476**；全局有序两两 **68906**。

## 0. 组合空间（先算清楚再设计）

| 空间 | 公式 | 数量 |
|------|------|------|
| catalog 总 API | — | **388** |
| 本地 API（含类型） | needsPeer=false | **291** |
| 对端 API | needsPeer=true | **97** |
| 本地可执行（function/method/onOff） | 去重后按模块 | **263** |
| 全局无序两两 C(N,2) | N(N-1)/2 | **34453** |
| 全局有序两两（调用次序不同） | N(N-1) | **68906** |
| 模块内有序两两 | Σ nᵢ(nᵢ-1) | **7430** |
| 跨模块有序两两 | Σᵢ<ⱼ 2·nᵢ·nⱼ | **61476** |

结论：**只做两两且区分次序，下限就是 ~6.9 万**；再乘异常/真实等待会更大。本设计 **不再压缩成一千级**，而是按层生成完整清单（jsonl），MD 负责原则与统计。

## 1. 设计承诺（v3）

1. **模块内全量有序两两**：对每个模块本地可执行 API，生成全部 `A→B`（A≠B），每条再带 `A-BTOFF`；若 A/B 属于扫描/连接类，再加 `R-WAIT`。
2. **跨模块全量有序两两**：不同模块 API 的全部 `A→B` 与 `B→A`（完整笛卡尔 × 次序）。
3. **跨模块异常**：对 hub 子集做 `A-BTOFF`（全量 cross×异常会使体量再翻倍，清单里单独一层；实现期可用同一生成器打开 `FULL_CROSS_ABN=1` 扩到全量）。
4. **三三组合**：模块内 hub 全排列 + `getState×hubᵢ×hubⱼ` 跨模块（含次序对调）。
5. **单接口真实场景**：扫描等到收数、GattClient 连接+getServices、容量步进 20……保留。
6. **调用次序**：`A→B` 与 `B→A` 视为不同用例（已计入有序两两）。
7. **清单落盘**：完整 ID 在 `apitest_scenario_design/*.jsonl`，禁止只在脑里「抽几条代表」。

## 2. 本设计稿用例总量

| 层 | 内容 | 条数 |
|----|------|------|
| S1 solo | 每 API 正常/异常/(真实) | **779** |
| P2 within N | 模块内有序两两正常 | **7430** |
| P2 within A | 模块内两两 BT_OFF | **7430** |
| P2 within R | 模块内两两含等待 | **1306** |
| P2 cross N | 跨模块有序两两（含次序对调） | **61476** |
| P2 cross A-hub | 跨模块 hub BT_OFF | **4798** |
| P3 triples | 三三（模块内 hub + 跨模块） | **4660** |
| CAP | 容量步进 | **11** |
| **合计（v3 设计）** | | **87890** |

完整机器清单：
- [`apitest_scenario_design/within_pairs.jsonl`](./apitest_scenario_design/within_pairs.jsonl)
- [`apitest_scenario_design/cross_pairs.jsonl`](./apitest_scenario_design/cross_pairs.jsonl)
- [`apitest_scenario_design/triples.jsonl`](./apitest_scenario_design/triples.jsonl)
- [`apitest_scenario_design/solo.jsonl`](./apitest_scenario_design/solo.jsonl)
- [`apitest_scenario_design/SUMMARY.json`](./apitest_scenario_design/SUMMARY.json)

## 3. 每模块本地可执行数与模块内两两

| 模块 | 本地可执行 n | 有序两两 n(n-1) | ×(N+A) 约 2n(n-1) |
|------|-------------|-----------------|-------------------|
| `legacy.bluetoothManager` | 48 | 2256 | 4512 |
| `legacy.bluetooth` | 45 | 1980 | 3960 |
| `ble` | 39 | 1482 | 2964 |
| `connection` | 27 | 702 | 1404 |
| `access` | 16 | 240 | 480 |
| `hid` | 16 | 240 | 480 |
| `enterprise` | 14 | 182 | 364 |
| `a2dp` | 12 | 132 | 264 |
| `socket` | 8 | 56 | 112 |
| `partnerAgent` | 8 | 56 | 112 |
| `opp` | 7 | 42 | 84 |
| `pbap` | 5 | 20 | 40 |
| `system.bluetooth` | 4 | 12 | 24 |
| `wearDetection` | 4 | 12 | 24 |
| `map` | 3 | 6 | 12 |
| `pan` | 3 | 6 | 12 |
| `baseProfile` | 3 | 6 | 12 |
| `hfp` | 1 | 0 | 0 |

## 4. 用例形态（每条两两长什么样）

### 4.1 正常有序两两 `SC-P2-{modA}.{apiA}__{modB}.{apiB}-N`

```
access.enableBluetooth → access.getState
→ modA.apiA（合法入参；若为 start* 则先 on 再调用）
→ modB.apiB
→ 若涉及扫描/发现/广播/连接：waitUntil(观测, T)
→ 对称 stop/close/off / 恢复本地名
```

- **断言**：本地步 `assertLocalOk`；peer 步 `assertPeerExpect`。
- **禁止**：只调 apiA 立刻结束；扫描类无等待窗口。

### 4.2 次序对调

`apiA→apiB` 与 `apiB→apiA` 是两条 ID（跨模块已显式生成两向；模块内由笛卡尔积覆盖）。

### 4.3 异常 BT_OFF

`disable → apiA → apiB → enable`，检查错误码白名单与不崩溃。

### 4.4 真实等待 / 连接（R-WAIT）

当两两中任一侧属于 `connect`, `createGattClientDevice`, `enableAdvertising`, `sppListen`, `startAdvertising`, `startBLEScan`, `startBluetoothDiscovery`, `startScan`, `subscribeBLEFound` 时，必须挂监听并 `waitUntil`。

### 4.5 GattClient / 容量（保留专项）

- createGattClient：扫描选址 → connect → wait CONNECTED → getServices → disconnect。
- 容量：步长 20 递增 create 直到失败，记 max，释放，复验。

## 5. 三三 / 多场景（P3）

已生成 **4660** 条：
- 模块内：hub 集合上全部有序三三（互异）。
- 跨模块：`access.getState → hubᵢ → hubⱼ` 与次序对调。

后续可再开 P4：四元组（discovery∥bleScan∥sppListen∥adv）全排列 —— 生成器预留 `build_quads()`。

## 6. 与「291 两两」口径对齐

- 你提到的 **291**：catalog `needsPeer=false` 全量（含 enum/interface）= **291**。
- 真正能进调用链两两的是可执行 **263**。
- 若对 291 做无序两两 C(291,2)=**42195**；有序=**84390** —— 类型条目不参与调用两两，改为「必须在某条 P2/P3 链中被引用」的 REF 约束。

## 7. 实现分期（按清单消费，不许再缩成千级）

| 阶段 | 消费清单 | 验收 |
|------|----------|------|
| P0 | SUMMARY 数字评审 | 合计与 jsonl 行数一致 |
| P1 | within N（7430）分段生成 ets | 模块内两两可跑 |
| P2 | cross N（61476）按模块对拆 List | 跨模块两两可跑 |
| P3 | within A + R + triples + CAP | 异常/真实/容量 |
| P4 | FULL_CROSS_ABN 全量异常两两（可选翻倍） | 关蓝牙交叉 |

分段 List 建议按「模块对」拆（如 `scenario.p2.ble_connection`），避免一次 THREAD_BLOCK。

## 8. 维护

```bash
python3 skills/ohtest/gen_scenario_full_design.py
```

旧 v2「每接口一条」设计作废；以 v3 合计与 jsonl 为准。

**v3 设计合计：87890 条（完整 ID 见 apitest_scenario_design/）。**
