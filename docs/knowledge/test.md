# 单元测试

## 一、测试域与注册位置

| 域 | 目录 | 说明 |
| --- | --- | --- |
| 单元测试 | `test/unittest/<模块>/` | 各功能域单测（a2dp / avrcp / ble / gatt / gatt_c / hfp / hid / host / map / opp / pan / pbap / spp / stack），目录内 `group("unittest")` 聚合 |
| 模块测试 | `test/moduletest/` | `btsvr_module_test`（server 层模块测试，`hosttest/host_srv_module_test.cpp`） |
| Fuzz | `test/fuzztest/host/` | `SetLocalNameFuzzTest`、`PairDeviceFuzzTest`、`SetBluetoothScanModeFuzzTest`、`CancelPairedDeviceFuzzTest` |
| 示例应用 | `test/example/` | `bluetoothtest`（API 自测应用，见 `APITEST_UT.md`）；`BluetoothSocketTest` 不参与 CI 编译 |

发布测试在 `bundle.json` 的 `component.build.test` 注册（spp/host/ble/hid/pan/gatt_c/stack 的 `:unittest`、`fuzztest/host:fuzztest`、`example/bluetoothtest:bluetoothtest`）。

## 二、新增用例模板

在对应模块目录 `BUILD.gn` 新增 `ohos_unittest` 目标，并注册到本目录 `group("unittest")`：

```gn
ohos_unittest("btfw_<模块>_unit_test") {
    module_out_path = "bluetooth_service/bluetooth_service"
    sources = [ "<模块>_test.cpp" ]
    configs = [ ":module_private_config" ]   # 测试日志宏 BTFW_LOG_TAG/BTFW_LOG_DOMAIN、mock/include
    external_deps = [
        "bluetooth:btframework",   # 经 bluetooth 仓框架接口访问服务能力
        "c_utils:utils",
        "hilog:libhilog",
        "ipc:ipc_core",
        "googletest:gmock_main",
        "googletest:gtest_main",
    ]
}

group("unittest") {
    testonly = true
    deps = [ ":btfw_<模块>_unit_test" ]
}
```

要点：

- 目标命名沿用 `btfw_<模块>_*`（参考既有：`btfw_spp_test`、`btfw_host_unit_test`、`btfw_stack_unit_test`、`btfw_gatt_client_c_unit_test`）；个别模块用 `ohos_moduletest`（如 ble 的 `btfw_ble_unit_test`）；
- 外层聚合常带 `if (is_phone_product)` 条件（非 phone 形态不编译）；
- fuzz 用例用 `ohos_fuzztest`，配置 `fuzz_config_file` 指向本目录，头文件引用 bluetooth 仓 `frameworks/inner/include`（示例：`test/fuzztest/host/setlocalname_fuzzer/BUILD.gn`）。

## 三、编译

从 OpenHarmony 源码根目录执行：

```bash
prebuilts/build-tools/linux-x86/bin/ninja -C out/rk3568 btfw_ble_unit_test
prebuilts/build-tools/linux-x86/bin/ninja -C out/rk3568 btsvr_module_test
```

产物输出到 `out/rk3568/tests/unittest/bluetooth_service/bluetooth_service/`（`module_output_path`）下，由单测框架调度执行。
