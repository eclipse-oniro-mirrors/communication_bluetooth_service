/**
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (C) 2022 Huawei Device Co., Ltd.
"""Thin wrapper — delegates to skills/ohtest/full_coverage.py."""

from __future__ import annotations

import os
import sys

_REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
_SKILL_DIR = os.path.join(_REPO_ROOT, "skills", "ohtest")
if _SKILL_DIR not in sys.path:
    sys.path.insert(0, _SKILL_DIR)

from full_coverage import main  # noqa: E402

if __name__ == "__main__":
    # Default project = bluetoothtest (parent of scripts/)
    _project = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
    if len(sys.argv) == 1:
        sys.argv.extend(["generate", "--project", _project])
    elif sys.argv[1] not in ("generate", "count", "help"):
        sys.argv = ["full_coverage", "generate", "--project", _project] + sys.argv[1:]
    raise SystemExit(main())
