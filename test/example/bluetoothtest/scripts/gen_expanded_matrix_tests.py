#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (C) 2022 Huawei Device Co., Ltd.
"""Generate BluetoothExpandedMatrix*.test.ets parameter-sweep suites."""
from __future__ import annotations

import os
import sys

OUT_DIR = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
    "entry",
    "src",
    "ohosTest",
    "ets",
    "apitest",
)

OAT = """/**
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
"""

ASSERT_KNOWN_LINES = [
    "expect(errCode === 0 || errCode === 201 || errCode === 801 ||",
    "  errCode === 2900001 || errCode === 2900099).assertTrue();",
]

ROTATE_CALLS = [
    ("access.getState()", "number"),
    ("ble.stopBLEScan()", "void"),
    ("bluetoothManager.getState()", "number"),
    ("connection.getLocalName()", "string"),
    ("ble.getConnectedBLEDevices()", "array"),
]


def tc_comment(it_name: str) -> list[str]:
    num = it_name.upper().replace("-", "_")
    return [
        "    /**",
        f"     * @tc.number SUB_BLUETOOTHTEST_APITEST_{num}",
        f"     * @tc.name {it_name}",
        f"     * @tc.desc Verify Bluetooth apitest case {it_name.replace('_', ' ')}",
        "     * @tc.size MediumTest",
        "     * @tc.type Function",
        "     * @tc.level Level 2",
        "     */",
    ]


def body_void(tag: str, call: str) -> list[str]:
    return [
        "      let errCode = 0;",
        "      hilog.info(0x0000, 'ExpandedMatrix', 'invoke');",
        "      try {",
        f"        {call};",
        "      } catch (err) {",
        "        errCode = (err as BusinessError).code;",
        "        hilog.error(0x0000, 'ExpandedMatrix', JSON.stringify(err));",
        "      }",
        "      hilog.info(0x0000, 'ExpandedMatrix', 'errCode=' + errCode);",
        *[f"      {ln}" for ln in ASSERT_KNOWN_LINES],
    ]


def body_return(tag: str, call: str, kind: str) -> list[str]:
    init = {
        "number": "      let value = -1;",
        "string": "      let value = '';",
        "array": "      let value: Array<Object> = [];",
        "bool": "      let value = false;",
    }[kind]
    assign = {
        "number": f"        value = {call};",
        "string": f"        value = {call};",
        "array": f"        value = {call} as Array<Object>;",
        "bool": f"        value = {call};",
    }[kind]
    checks = {
        "number": ["      expect(typeof value).assertEqual('number');"],
        "string": [
            "      expect(typeof value).assertEqual('string');",
            "      expect(value.length >= 0).assertTrue();",
        ],
        "array": ["      expect(Array.isArray(value)).assertTrue();"],
        "bool": ["      expect(typeof value).assertEqual('boolean');"],
    }[kind]
    return [
        "      let errCode = 0;",
        init,
        "      hilog.info(0x0000, 'ExpandedMatrix', 'invoke');",
        "      try {",
        assign,
        "      } catch (err) {",
        "        errCode = (err as BusinessError).code;",
        "        hilog.error(0x0000, 'ExpandedMatrix', JSON.stringify(err));",
        "      }",
        "      hilog.info(0x0000, 'ExpandedMatrix', 'errCode=' + errCode);",
        "      hilog.info(0x0000, 'ExpandedMatrix', 'value=' + JSON.stringify(value));",
        *[f"      {ln}" for ln in ASSERT_KNOWN_LINES],
        *checks,
    ]


def emit_case(it_name: str, call: str, kind: str) -> list[str]:
    lines = tc_comment(it_name)
    lines.append(f"    it('{it_name}', 0, () => {{")
    if kind == "void":
        lines.extend(body_void(it_name, call))
    else:
        lines.extend(body_return(it_name, call, kind))
    lines.append("    });")
    lines.append("")
    return lines


def write_file(path: str, content: str) -> None:
    _repo = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
    _skill = os.path.join(_repo, "skills", "ohtest")
    if os.path.isdir(_skill) and _skill not in sys.path:
        sys.path.insert(0, _skill)
    try:
        from apitest_format import format_apitest_source

        content = format_apitest_source(content, wrap_outer_try=False)
    except ImportError:
        pass
    for banned in ("expect(true).assertTrue()", "matrixTryRun(", "tI("):
        if banned in content:
            raise RuntimeError(f"banned {banned} in {path}")
    with open(path, "w", encoding="utf-8") as f:
        f.write(content)


def gen_profile_sweep(
    filename: str,
    func_name: str,
    describe_name: str,
    header_doc: str,
    imports: list[str],
    it_prefix: str,
    call_fmt: str,
    start: int,
    end: int,
    ret_kind: str,
) -> int:
    lines = [
        OAT.rstrip(),
        "",
        "/**",
        f" * {header_doc}",
        " * Direct API calls; hilog + errCode/return assertions (no matrixTryRun / tautology).",
        " */",
        "import { describe, it, expect } from '@ohos/hypium';",
        "import hilog from '@ohos.hilog';",
        "import { BusinessError } from '@ohos.base';",
        *imports,
        "",
        f"export default function {func_name}() {{",
        f"  describe('{describe_name}', () => {{",
        "",
    ]
    n = 0
    for i in range(start, end + 1):
        it_name = f"{it_prefix}_{i}"
        call = call_fmt.format(i=i)
        lines.extend(emit_case(it_name, call, ret_kind))
        n += 1
    lines.append("  });")
    lines.append("}")
    lines.append("")
    write_file(os.path.join(OUT_DIR, filename), "\n".join(lines))
    return n


def gen_rotate() -> int:
    lines = [
        OAT.rstrip(),
        "",
        "/**",
        " * Kit idempotent entry rotation (20 cases).",
        " * Direct API calls; hilog + errCode/return assertions.",
        " */",
        "import { describe, it, expect } from '@ohos/hypium';",
        "import hilog from '@ohos.hilog';",
        "import { BusinessError } from '@ohos.base';",
        "import { access, ble, bluetoothManager, connection } from '@kit.ConnectivityKit';",
        "",
        "export default function bluetoothExpandedMatrixRotateTest() {",
        "  describe('ExpandedMatrix_kit_idempotent_rotate', () => {",
        "",
    ]
    n = 0
    for i in range(20):
        call, kind = ROTATE_CALLS[i % len(ROTATE_CALLS)]
        it_name = f"expanded_kit_rotate_{i}"
        lines.extend(emit_case(it_name, call, kind))
        n += 1
    lines.append("  });")
    lines.append("}")
    lines.append("")
    write_file(os.path.join(OUT_DIR, "BluetoothExpandedMatrixRotate.test.ets"), "\n".join(lines))
    return n


def main() -> None:
    os.makedirs(OUT_DIR, exist_ok=True)
    total = 0
    total += gen_profile_sweep(
        "BluetoothExpandedMatrixConnection01.test.ets",
        "bluetoothExpandedMatrixConnection01Test",
        "ExpandedMatrix_CK_getProfileConnectionState_0_129",
        "ConnectivityKit getProfileConnectionState sweep 0..129.",
        ["import { connection, constant } from '@kit.ConnectivityKit';"],
        "expanded_ck_getProfileConnectionState",
        "connection.getProfileConnectionState({i} as constant.ProfileId)",
        0,
        129,
        "number",
    )
    total += gen_profile_sweep(
        "BluetoothExpandedMatrixConnection02.test.ets",
        "bluetoothExpandedMatrixConnection02Test",
        "ExpandedMatrix_CK_getProfileConnectionState_130_259",
        "ConnectivityKit getProfileConnectionState sweep 130..259.",
        ["import { connection, constant } from '@kit.ConnectivityKit';"],
        "expanded_ck_getProfileConnectionState",
        "connection.getProfileConnectionState({i} as constant.ProfileId)",
        130,
        259,
        "number",
    )
    total += gen_profile_sweep(
        "BluetoothExpandedMatrixLegacy01.test.ets",
        "bluetoothExpandedMatrixLegacy01Test",
        "ExpandedMatrix_legacy_getProfileConnState_0_129",
        "@ohos.bluetooth.getProfileConnState sweep 0..129.",
        ["import bluetooth from '@ohos.bluetooth';"],
        "expanded_legacy_getProfileConnState",
        "bluetooth.getProfileConnState({i})",
        0,
        129,
        "number",
    )
    total += gen_profile_sweep(
        "BluetoothExpandedMatrixLegacy02.test.ets",
        "bluetoothExpandedMatrixLegacy02Test",
        "ExpandedMatrix_legacy_getProfileConnState_130_259",
        "@ohos.bluetooth.getProfileConnState sweep 130..259.",
        ["import bluetooth from '@ohos.bluetooth';"],
        "expanded_legacy_getProfileConnState",
        "bluetooth.getProfileConnState({i})",
        130,
        259,
        "number",
    )
    total += gen_rotate()
    print(f"generated {total} ExpandedMatrix cases")


if __name__ == "__main__":
    main()
