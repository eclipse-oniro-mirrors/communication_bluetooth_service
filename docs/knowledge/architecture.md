# 蓝牙服务仓架构：关键机制与流程枢纽

> 本文只记录跨模块的机制性知识（设计模式与流程入口），不重复目录分层与模块位置（见 AGENTS.md）。

## 一、关键设计机制

1. **单例 + 代理访问**：应用经 bluetooth 仓 proxy 跨进程访问本仓 `BluetoothHostServer`（`SystemAbility` 派生，`MakeAndRegisterAbility` 注册 SA 1130，`GetInstance()` 全局单例）；server 深入同进程的 service 层单例（`IAdapterManager::GetInstance()`、`ProfileServiceManager::GetInstance()`）完成业务，无二次 IPC。
2. **SA 生命周期**：`bluetooth_service` 进程由 init 在 `post-fs-data` 阶段拉起（`services/bluetooth/etc/init/bluetooth_service.cfg`），SA 1130 `run-on-create` 注册（`sa_profile/1130.json`）；`OnStart()` → `Init()`（`Publish` 注册 SA）→ `STATE_RUNNING`，`OnStop()` → `pimpl->Clear()`。
3. **状态机体系**：通用基类 `utility::StateMachine`（`service/src/util/state_machine.h`，嵌套 `State` + 消息驱动）；系统/适配器级有 `SysStateMachine`（系统启停/复位）、`AdapterStateMachine`（开关）、`PowerStateMachine`，各 profile 自带状态机（如 `avrcp_ct_state_machine.h`）。
4. **观察者 + 远端分发**：server 端用 `RemoteObserverList` 向多个应用分发事件，注册时记录 caller 的 token/pid（`observersToken_`/`observersPid_`，`IPCSkeleton::GetCallingTokenID`）；service 端用 `base_observer_list.h` 维护本地观察者。
5. **Profile 管理**：`ProfileServiceManager`（`IProfileManager` 单例）统一管理 profile 装载/卸载与生命周期；profile 开关由 `bt_config.xml` 的 `ClassicAdapter`/`BleAdapter` 节配置项（如 `A2dpSrcService`、`HfpAgService`）控制（`AdapterConfig`/`profile_info.cpp`），profile 元信息见 `profile_info.h`、`profile_list.h`。
6. **配置体系**：`AdapterConfig`（bt_config.xml）、`AdapterDeviceConfig`（bt_device_config.xml）、`AdapterDeviceInfo`（bt_device_info.xml）、`ProfileConfig`（bt_profile_config.xml）、`ClassicConfig`/`BleConfig`；均为 `GetInstance()->GetValue(section, property, value)` 单例读取，运行路径缺失时从 base 路径回拷模板（`Reload()`）。
7. **定时器与事件分发**：`TimerManager`（`service/src/util/timer.h` + `internal/timer_linux.cpp`，单例，epoll 实现）；`utility::Dispatcher`（`util/dispatcher.h`）承载 service 层异步事件。
8. **协议栈与 HDI 接口**：service 经 `stack/include/`（`btm.h`、`l2cap_if.h`、`rfcomm.h`、`sdp.h`、`att.h`、`gap_le_if*.h`、`avdtp.h`、`avctp.h` 等）调用 libbtstack；HCI 通道经 `hardware/`（`bluetooth_hdi_adapter` → HDI proxy `libbluetooth_hci_proxy_1.0`，vendor 扩展见 `bt_vendor_lib.h`）。

## 二、核心流程入口（速查）

| 流程 | 入口 | 关键枢纽 |
| --- | --- | --- |
| 蓝牙开关 | `BluetoothHostServer::EnableBt` / `DisableBt` | `IAdapterManager::Enable/Disable(BTTransport)` → `SysStateMachine` → `AdapterStateMachine` → `ClassicAdapter`/`BleAdapter` |
| 设备发现 | `BluetoothHostServer::StartBtDiscovery` / `CancelBtDiscovery` | `ClassicAdapter`（`classic/classic_adapter.*`） |
| 配对 | `StartPair` / `SetDevicePairingConfirmation` / `PairRequestReply` | `ClassicAdapter` / `BleAdapter` + `AdapterDeviceConfig` |
| BLE 扫描 | `bluetooth_ble_central_manager_server` → `ble/ble_central_manager_impl` | `BleAdapter`、`stack/include/gap_le_if*.h` |
| GATT | `bluetooth_gatt_client_server` / `bluetooth_gatt_server_server` → `gatt/` | `GattClientProfile` / `GattServerProfile`、`gatt_connection_manager`、`gatt_cache` |
| Socket 传输 | `bluetooth_socket_server` → `sock/socket_service` | `socket_gap_*` / `socket_sdp_*`、`transport/`（RFCOMM/L2CAP） |
| 事件订阅分发 | `RegisterObserver` / `RegisterRemoteDeviceObserver` | `RemoteObserverList` + caller token/pid 记录 |
| HFP AG 与电话系统 | `hfp_ag_system_interface`（订阅电话 SA） | `SystemAbilityStatusChange`（电话 SA 起停联动） |
| HCI 日志开关 | `AdapterManager::OutputSetting()` | `BTM_SetSnoopFilePath` / `BTM_EnableSnoopFileOutput`（btsnoop） |

## 三、新增模块/文档时的基线

- 新增 profile 走读文档建议结构：目录职责 → 关键类 → 状态机/数据流 → 相邻层交互点 → 错误码与日志关键字。
- 新增机制说明必须落到"类/文件"级别，避免只写概念。
