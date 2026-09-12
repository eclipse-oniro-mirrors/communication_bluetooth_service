# 蓝牙服务组件指引

## 项目定位

本仓库对应 OpenHarmony `foundation/communication/bluetooth_service`，实现系统蓝牙服务（SA 1130）、核心业务与协议栈。应用侧接口（NAPI/ETS/C API）与 IPC proxy 端不在此仓，位于配对的 `bluetooth` 仓。

- `sa_profile/`：SA 1130 注册（`1130.json`，进程 `bluetooth_service`，加载 `libbluetooth_server.z.so`）。
- `services/bluetooth/server/`：服务端入口与 IPC stub 端（`bluetooth_server`，libbluetooth_server.so）。
- `services/bluetooth/service/`：核心业务（`btservice`）——ble/classic/gatt/gavdp/obex/sock/transport/dialog/permission 及各 profile（a2dp/avrcp/hfp/hid/pan）；`btsbc` 为 A2DP SBC 编解码库。
- `services/bluetooth/ipc/`：IPC stub 与回调 proxy（`btipc_service`，静态库，链入 bluetooth_server）。
- `services/bluetooth/stack/`：协议栈（`btstack`）——btm/gap/l2cap/att/smp/sdp/rfcomm/avdtp/avctp/hci/iso。
- `services/bluetooth/hardware/`：HDI 适配（`bluetooth_hdi_adapter`，经 HDI 接口访问芯片）。
- `services/bluetooth/external/`：外部依赖桩（`btdummy`，telephony/media/map/vcard 等 stub 实现；被 `btstack`/`btservice`/`btsbc` 依赖，一般不直接改动）。
- `services/bluetooth/etc/init/`：服务启动与配置文件（`bluetooth_service.cfg`、`bt_config.xml`、`bt_device_info.xml` 等）。
- `test/`：单元测试（`unittest/`）、模块测试（`moduletest/`）、fuzz 目标（`fuzztest/`）和示例应用（`example/`）。

### 本仓与 bluetooth 仓的分工

本仓只实现服务侧（IPC stub 端）；应用接口（NAPI/ETS/C API）与 IPC proxy 在 `bluetooth` 仓（`foundation/communication/bluetooth`），经 `external_deps` 的 `bluetooth:btframework`、`bluetooth:btcommon` 提供。改动 IPC 接口时，本仓 stub 端须与 `bluetooth` 仓 proxy 端同步。

判断改哪个目录，看**改动落在哪一层**（应用接口 → bluetooth 仓；服务端 stub/业务/协议栈/HDI → 本仓），不要自行假设。

### 按任务类型定位代码

| 任务类型 | 修改位置 |
| --- | --- |
| 修改 Host 开关/设备管理 | `services/bluetooth/server/src/bluetooth_host_server.cpp` + `service/src/common/adapter_manager.*`、`service/src/classic/`、`service/src/ble/` |
| 修改 IPC stub/权限校验 | `services/bluetooth/ipc/src/bluetooth_*_stub.cpp`（`memberFuncMap_` 注册接口与权限项） |
| 新增/修改 Profile 业务 | `services/bluetooth/service/src/<profile>/`，并在 `service/BUILD.gn` 注册源文件 |
| 修改协议栈 | `services/bluetooth/stack/src/<协议>/`（btm/gap/l2cap/att/smp/sdp/rfcomm/avdtp/avctp/hci/iso） |
| 修改 HDI/芯片适配 | `services/bluetooth/hardware/`（`bluetooth_hdi*.cpp`、`bt_vendor_lib.h`） |
| 修改 Profile 开关/特性宏 | `bluetooth.gni`（declare_args）与各模块 `BUILD.gn`（如 server 端 `BLUETOOTH_*_FEATURE` defines） |
| 修改 SA 注册/启动配置 | `sa_profile/1130.json`、`services/bluetooth/etc/init/` |
| 新增单元测试 | `test/unittest/<模块>/`，并注册到该目录 `BUILD.gn` 的 `group("unittest")` |

### 嵌套指引

本仓库无目录级嵌套 AGENTS.md；所有任务级指引通过本文件与 `docs/knowledge/` 场景文档提供。

## 构建和验证

构建命令从 OpenHarmony 源码根目录执行，不在本子目录执行。

单模块编译：

```bash
# 注意：-T 传的是 gn target 名（如 btservice）或完整 label（路径:target），
# 组件名（bluetooth_service）不是 ninja target，会报 unknown target！
hb build -T btservice -T bluetooth_server
hb build -T foundation/communication/bluetooth_service/services/bluetooth/service:btservice
```

优先使用独立编译命令：

```bash
hb build bluetooth_service -i
```

若有整仓代码环境，可执行下述命令：

```bash
./build.sh --product-name rk3568 --build-target input --ccache
prebuilts/build-tools/linux-x86/bin/ninja -C out/rk3568 bluetooth_service
```

### 关键 gn target

| target | 位置 | 产物/说明 |
| --- | --- | --- |
| `bluetooth_server` | `services/bluetooth/server` | libbluetooth_server.so：服务端入口（SA 1130 加载） |
| `btservice` | `services/bluetooth/service` | libbtservice.so：核心服务（ble/classic/gatt/gavdp/obex 及各 profile） |
| `btsbc` | `services/bluetooth/service` | A2DP SBC 编解码动态库 |
| `btipc_service` | `services/bluetooth/ipc` | IPC stub + 回调 proxy（静态库，链入 bluetooth_server） |
| `btstack` | `services/bluetooth/stack` | libbtstack.so：协议栈 |
| `bluetooth_hdi_adapter` | `services/bluetooth/hardware` | HDI 适配层 |
| `btdummy` | `services/bluetooth/external` | 外部依赖桩库（telephony/media 等 stub 实现） |
| `communication_bluetooth_service_sa_profile` | `sa_profile` | SA 1130 profile（1130.json） |

### 完成标准

任务被认为完成，当且仅当：

1. **代码改动已提交** - 按"提交规范"提交；
2. **构建通过** - 执行对应构建命令（`hb build bluetooth_service -i` 或对应 gn target）；
3. **相关测试通过** - 对应单元测试通过（如有）；
4. **IPC 接口改动已核对** - stub 与 bluetooth 仓 proxy 的接口码、parcel 读写一致，错误码透传正常；
5. **真机链路提供证据** - 涉及连接/传输/音频/配对等真实设备链路的改动，附验证证据（日志或截图）。

### 如果无法运行验证

明确说明无法运行的原因，列出推荐的验证步骤供人工执行，显式标记需要人工验证的部分。

### 完成报告格式

报告应包含：改动摘要（文件列表、改动点）、验证结果（构建/测试输出）、风险评估（IPC 兼容性、行为变化）、未完成事项。

## 知识索引

稳定背景知识放在 `docs/knowledge/`。改动前按场景先读：

### 场景与路径路由

| 场景 | 先读文档 |
| --- | --- |
| 服务/协议栈架构与关键流程（状态机/观察者/SA 生命周期/AdapterManager） | `docs/knowledge/architecture.md` |
| 日志定位、HCI/btsnoop 日志（hidumper） | `docs/knowledge/debug.md` |
| IPC/接口改动（接口码/parcel/memberFuncMap_） | `docs/knowledge/architecture.md`（机制 1/4）+ bluetooth 仓 `frameworks/inner/ipc/interface/`（i_bluetooth_*.h） |
| 单元测试编写与运行 | `docs/knowledge/test.md` |
| 历史问题案例（缺陷模式与自查规则，改动前对照） | `docs/knowledge/key_history_issue/` |

### 开始编辑前

在修改代码前，按以下顺序确认：

1. 对照"按任务类型定位代码"确定任务类别；
2. 读取"场景与路径路由"对应的知识文档；
3. 核对"项目约束"，确认不违反红线；
4. 声明："修改 X，已阅读 Y 文档，遵循 Z 约束"。

## 问题分析与案例沉淀

每次完成问题分析（检视意见核查、缺陷定位、故障排查等）后，当问题被接纳上库时，执行以下收尾流程：

1. **总结 issue 案例**：按 `docs/knowledge/key_history_issue/` 已有案例的模板总结本次问题——教训式文件名 + frontmatter（KP 编号顺延）+ 陷阱模式 + 高风险代码区域 + 检查规则 + 正/反模式；
2. **征询固化**：向开发者确认是否需要将案例固化上库；
3. **确认后提交**：仅在开发者确认后按"提交规范"提交，未确认前不提交。

案例要求：落到"文件/类/函数"级；检查规则可执行（grep/步骤）；同类教训合并进已有案例，不重复建卡。

## 项目约束

### 架构与层次

- 不要违反代码的层次结构（server/ipc → service → stack → hardware）。不要在 server 层夹带业务逻辑，不要在 stack/hardware 反向依赖上层。
- 硬件访问统一走 HDI（`hardware/`），不要在 service/stack 绕过 HDI 直连驱动。

### 编码约定

- HILOG 日志不能明文打印蓝牙地址：使用 `GET_ENCRYPT_ADDR` / `GET_ENCRYPT_GATT_ADDR` / `GET_ENCRYPT_AVRCP_ADDR` 等宏（`service/src/util/log_util.h`、`server/include/bluetooth_utils_server.h`）。
- 错误码统一使用 `bluetooth_errorcode.h` 的 `BtErrCode`，不要硬编码数字；错误码需经 IPC reply 透传到应用层。
- 新增/修改 IPC 接口：同步更新 stub 的 `memberFuncMap_`（`ipc/src/bluetooth_*_stub.cpp`）与权限项（`service/src/permission/`）。

### IPC 接口约束（与 bluetooth 仓 proxy 配套）

**Do not（禁止）：**

- 修改 IPC 接口码、parcel 序列化顺序（须与 bluetooth 仓 proxy 端严格一致）；
- 在 IPC 边界丢弃错误码语义。

**Ask before（修改前必须确认）：**

- 新增 IPC 接口或修改接口码：确认与 bluetooth 仓 proxy 的跨版本兼容性。

### 安全与权限边界

**Do not（禁止）：**

- 绕过已有权限检查（PermissionItem / PermissionManager）；
- 在日志中输出敏感信息（地址、设备标识等，统一走匿名化宏）；
- 执行影响真机状态的破坏性命令（清空配对数据、固件/参数刷写、强制复位芯片等）。

**Ask before（修改前必须确认）：**

- 修改 `services/bluetooth/service/src/permission/` 的权限项配置；
- 涉及多用户/账户隔离的改动。

### 提交规范

使用 `git commit -s`，并新增 `Co-Authored-By: Agent` 信息。Signed-off-by 必须使用系统自带的（git config 的用户信息）。

```
# git commit 信息模板

fix(bluetooth_service): 修复 xxx 问题

xxx

Co-Authored-By: Agent
Change-Id: I3eea26405ea4c8551ea8621aeb8e8e09672c8b15
Signed-off-by: liuqian <liuxi64@h-partners.com>
```
