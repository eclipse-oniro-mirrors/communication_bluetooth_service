/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#ifndef HCI_DEF_CMD_COMMON_H
#define HCI_DEF_CMD_COMMON_H

#include "hci_def_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAKE_OPCODE(ocf, ogf) ((ocf) | (ogf) << 10)
#define GET_OGF(x) (((x) >> 10) & 0x3f)
#define GET_OCF(x) ((x) & 0x3ff)

#ifdef __cplusplus
}
#endif

#endif