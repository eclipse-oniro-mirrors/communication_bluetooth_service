#!/usr/bin/env python3
# Copyright (c) 2026 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import argparse
import json
import re
from pathlib import Path


def fail(message):
    print("FAIL: " + message)
    raise SystemExit(1)


def load_bundle(path):
    with path.open(encoding="utf-8") as stream:
        data = json.load(stream)
    tests = data.get("component", {}).get("build", {}).get("test")
    if not isinstance(tests, list) or not all(isinstance(item, str) for item in tests):
        fail("component.build.test must be a string array")
    return tests


def check_duplicates(targets):
    seen = set()
    for target in targets:
        if target in seen:
            fail("duplicate test target: " + target)
        seen.add(target)


def check_targets_exist(root, targets):
    allowed = {
        "//foundation/communication/bluetooth_service/test:stack_unittest",
        "//foundation/communication/bluetooth_service/test:vendor_unittest",
        "//foundation/communication/bluetooth_service/test:btif_unittest",
    }
    test_build = (root / "test" / "BUILD.gn").read_text(encoding="utf-8")
    for target in targets:
        if target not in allowed:
            fail("out-of-scope test target: " + target)
        label = target.split(":")[-1] if ":" in target else target
        if not re.search(rf'\b{re.escape(label)}\b', test_build):
            fail("target not declared in test/BUILD.gn: " + target)


def check_manifest(root, targets):
    manifest = (root / "test" / "TEST_MANIFEST.md").read_text(encoding="utf-8")
    for target in targets:
        if target not in manifest:
            fail("target missing from test/TEST_MANIFEST.md: " + target)


def collect_test_names(directory, names):
    pattern = re.compile(r"HWTEST(?:_F|_P)?\s*\(\s*(\w+)\s*,\s*(\w+)")
    for source in directory.rglob("*.cpp"):
        for match in pattern.finditer(source.read_text(encoding="utf-8", errors="ignore")):
            suite, case = match.group(1), match.group(2)
            key = suite + "." + case
            if key in names:
                fail("duplicate HWTEST case: " + key)
            names[key] = source


def check_test_cases(root):
    names = {}
    in_scope = [
        root / "test" / "unittest" / "stack",
        root / "test" / "unittest" / "btif",
        root / "test" / "unittest" / "vendor",
    ]
    for directory in in_scope:
        collect_test_names(directory, names)
    return len(names)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--bundle", required=True)
    parser.add_argument("--root", required=True)
    args = parser.parse_args()

    root = Path(args.root)
    targets = load_bundle(Path(args.bundle))
    check_duplicates(targets)
    check_targets_exist(root, targets)
    check_manifest(root, targets)
    count = check_test_cases(root)
    print("OK: bundle test targets:", len(targets))
    print("OK: unique HWTEST cases:", count)


if __name__ == "__main__":
    main()
