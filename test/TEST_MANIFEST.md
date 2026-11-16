# bluetooth_service 测试清单

bluetooth_service 仅负责：

```text
services/bluetooth/stack        -> btstack
services/bluetooth/stack/vendor -> bt_vendor_service
```

`bundle.json` 注册 stack、vendor 和 btif 三部分测试入口。框架层与服务层测试属于 `bluetooth` 仓。

## Stack 测试

| 测试组 | GN 目标 | 用例数 | 源文件 | 输出目录 |
| --- | --- | ---: | --- | --- |
| stack | `//foundation/communication/bluetooth_service/test/unittest/stack:btfw_stack_unit_test` | 344 | `stack_att_52_test.cpp`, `stack_gap_le_5_1_event_parse_test.cpp`, `stack_gap_le_5_1_interact_test.cpp`, `stack_gap_le_5_1_test.cpp`, `stack_gap_le_5_2_test.cpp`, `stack_gap_le_5_3_test.cpp`, `stack_gap_le_5_4_test.cpp`, `stack_gap_le_6_0_interact_test.cpp`, `stack_gap_le_6_0_interact_cs_test.cpp`, `stack_gap_le_6_0_test.cpp`, `stack_gap_le_6_0_cs_test.cpp`, `stack_gap_le_interact_test.cpp`, `stack_gap_le_test.cpp`, `stack_iso_data_plane_test.cpp`, `stack_smp_tool_test.cpp` | `tests/unittest/bluetooth_service/bluetooth_service` |

说明：

```text
包含解析/状态测试和双机交互测试。
交互用例需要真实控制器，部分用例会停止 bluetooth_service
并设置 SELinux permissive，只能在专用设备上运行。
```

## Vendor 测试

| GN 目标 | 用例数 | 源文件 | 输出目录 |
| --- | ---: | --- | --- |
| `//foundation/communication/bluetooth_service/test/unittest/vendor:btfw_vendor_unit_test` | 5 | `vendor_interface_test.cpp` | `tests/unittest/bluetooth_service/bluetooth_service` |

覆盖内容：

```text
vendor 命令映射
wallet 调用方识别
真实地址 / MMI 白名单默认值
包名、Ability 和 Action 名称
未知命令类型
```

## Btif 测试

| 测试组 | GN 目标 | 用例数 | 源文件 | 输出目录 |
| --- | --- | ---: | --- | --- |
| btif | `//foundation/communication/bluetooth_service/test/unittest/btif:btif_unit_test` | 47 | `core_ble_scan_policy_test.cpp`, `btif_task_scheduler_test.cpp`, `core_callback_registry_test.cpp`, `core_device_store_test.cpp`, `core_discovery_state_machine_test.cpp`, `core_pairing_manager_test.cpp`, `core_sdp_discovery_test.cpp`, `core_util_test.cpp`, `hci_tx_gate_test.cpp` | `tests/unittest/bluetooth_service/bluetooth_service` |
| btif 生命周期 | `//foundation/communication/bluetooth_service/test/unittest/btif:btif_task_scheduler_lifecycle_test` | 3 | `btif_task_scheduler_lifecycle_test.cpp` | 同上 |

## Bundle 入口

```json
"test": [
  "//foundation/communication/bluetooth_service/test:stack_unittest",
  "//foundation/communication/bluetooth_service/test:vendor_unittest",
  "//foundation/communication/bluetooth_service/test:btif_unittest"
]
```

## 构建命令

```bash
./build.sh -p wukong100 --ccache \
  --build-target foundation/communication/bluetooth_service/test:stack_unittest \
  --build-target foundation/communication/bluetooth_service/test:vendor_unittest \
  --build-target foundation/communication/bluetooth_service/test:btif_unittest
```
