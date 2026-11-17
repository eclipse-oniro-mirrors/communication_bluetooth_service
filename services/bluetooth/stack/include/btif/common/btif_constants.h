/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#ifndef BTIF_COMMON_BTIF_CONSTANTS_H
#define BTIF_COMMON_BTIF_CONSTANTS_H

#include <cstddef>
#include <cstdint>

namespace bluetooth {
namespace btif {

inline constexpr uint32_t kInquiryLength = 0x08;
inline constexpr uint32_t kScanDutyDenominator = 2;
inline constexpr uint32_t kBleDiscoveryDedupMs = 1000;
inline constexpr uint32_t kBleScanStageTimeoutMs = 12000;
inline constexpr uint8_t kBrSecurityMode = 1;
inline constexpr uint8_t kBrIoCapability = 1;
inline constexpr uint8_t kBrAuthReq = 1;
inline constexpr char kBtsnoopFilePath[] = "/data/log/bt/btsnoop_hci.log";
inline constexpr uint32_t kBtsnoopMaxMb = 10;
inline constexpr uint16_t kSdpAttributeRangeEnd = 0xFFFF;
inline constexpr uint16_t kSdpRfcommProtocolUuid = 0x0003;
inline constexpr uint16_t kSdpL2capProtocolUuid = 0x0100;
inline constexpr int32_t kSdpUnknownChannel = -1;
inline constexpr uint32_t kSdpQueryTimeoutMs = 1500;
inline constexpr uint8_t kSdpFallbackUuidEnable = 1;
inline constexpr uint16_t kSdpFallbackUuid16 = 0x1002;
inline constexpr char kDefaultLocalName[] = "OpenHarmony 3.2";
inline constexpr char kDeviceStorePath[] = "/data/service/el1/public/bluetooth/btif_devices.bin";
inline constexpr char kDeviceStoreTmpPath[] = "/data/service/el1/public/bluetooth/btif_devices.tmp";
inline constexpr size_t kBondedDevicesMax = 16;
inline constexpr size_t kAdapterPropertyMax = 3;
inline constexpr size_t kDevicePropertyMax = 6;
inline constexpr size_t kBitsPerByte = 8;
inline constexpr uint8_t kByteMask = 0xFF;
inline constexpr size_t kUint32Size = 4;
inline constexpr int kNamePropertyCount = 2;
inline constexpr uint32_t kLeConnectStartDelayUs = 300000;
inline constexpr uint8_t kEirTypeShortenedLocalName = 0x08;
inline constexpr uint8_t kEirTypeCompleteLocalName = 0x09;

} // namespace btif
} // namespace bluetooth

#endif // BTIF_COMMON_BTIF_CONSTANTS_H
