# 调试与日志定位

## 一、hilog 日志

- 本仓全层（server/ipc/service/stack/hardware）统一日志域 `LOG_DOMAIN 0xD000102`、默认 tag `"Bluetooth"`（`services/bluetooth/common/log.h`，stack 的 C 代码同源 include `log.h`）。
- 日志格式自带 `[文件(函数:行号)]` 前缀（`HILOGD/I/W/E/F` 宏），可直接按文件名/函数名过滤：

```bash
hilog | grep "Bluetooth"              # 按 tag 过滤本仓日志
hilog | grep "bluetooth_host_server"  # 按文件定位（server 层）
hilog | grep "hci.c"                  # 按文件定位（协议栈 HCI 层）
hilog | grep "EnableBt"               # 按函数名定位
```

- 地址在日志中已匿名化（`GET_ENCRYPT_ADDR` / `GET_ENCRYPT_GATT_ADDR` / `GET_ENCRYPT_AVRCP_ADDR`），对照设备时使用加密串而非明文。

## 二、HCI / btsnoop 日志

- btsnoop 由 `bt_config.xml` 的 `OutputSetting` 节控制，`AdapterManager::OutputSetting()` 在启动时下发给协议栈（`BTM_SetSnoopFilePath` / `BTM_EnableSnoopFileOutput`）：

| 配置项 | 默认值 | 说明 |
| --- | --- | --- |
| `BtsnoopOutput` | true | btsnoop 抓包开关 |
| `BtsnoopOutputPath` | /data/bluetooth/log/snoop.log | 抓包落盘路径 |
| `HciLogOutput` | false | HCI 内部日志输出（`BTM_EnableHciLogOutput`） |
| `Desensitization` | false | 输出脱敏 |
| `BtOutputMaxSize` | 0x64 | 输出文件大小上限 |

- 判读原则（第一判据）：
  - 有请求无响应 → 问题在响应方（接收端）；
  - 请求未发出 → 问题在发起方（发送端）；
  - 响应带异常码 → 按 HCI 异常码表直接定位拒因（`stack/src/hci/hci_error.h`、`hci_def_status_params_cmd.h`）；
  - 本机 hilog 仅作辅助验证，不作为定责依据。

## 三、系统状态 dump（hidumper）

- SA 1130 注册了 dump 能力（`server/src/bluetooth_host_dumper.cpp`）：

```bash
hidumper -s 1130 -a '-h'    # 显示支持的 dump 选项
hidumper -s 1130 -a '-br'   # 显示 BR/BLE 开关状态
```

- 适用：快速确认服务存活与开关状态；更细的状态需结合 hilog / btsnoop。

## 四、补充说明

- 本仓无 CLI 调试工具（bluetoothTool 在 bluetooth 仓）；应用侧验证用 `test/example/bluetoothtest`（见 `APITEST_UT.md`）。
- 启动失败排查：`bluetooth_service.cfg` 中的进程约束（uid `bluetooth`、CAP_NET_ADMIN、SELinux `u:r:bluetooth_service:s0`）与 `OnStart/Init/Publish failed` 日志。
- 单元测试 / fuzz 调试见 `test.md`。
