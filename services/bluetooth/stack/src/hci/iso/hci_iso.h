/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
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

#ifndef HCI_ISO_H
#define HCI_ISO_H

#include <stdint.h>

#include "hci/acl/hci_acl.h"
#include "packet.h"

#ifdef __cplusplus
extern "C" {
#endif

void HciInitIso(void);
void HciCloseIso(void);

void HciOnIsoData(Packet *packet);

void HciIsoSetIsoDataPackets(uint16_t totalPackets);
uint16_t HciIsoGetAvailableIsoDataPackets(void);
void HciIsoRegisterHandle(uint16_t connectionHandle);
void HciIsoDeregisterHandle(uint16_t connectionHandle);
void HciIsoOnNumberOfCompletedPackets(uint8_t numberOfHandles, const HciNumberOfCompletedPackets *list);

#ifdef __cplusplus
}
#endif

#endif
