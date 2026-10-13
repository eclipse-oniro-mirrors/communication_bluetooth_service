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

#ifndef STACK_GAP_LE_6_0_TEST_H
#define STACK_GAP_LE_6_0_TEST_H

#include <cstddef>
#include <cstdint>
#include <gtest/gtest.h>

#include "hci/hci_def_le_evt.h"
#include "hci/hci_def_le_evt_6_0.h"
#include "hci/hci_evt_callbacks.h"
#include "iso_le_if.h"

// Capture structs shared by stack_gap_le_6_0_test.cpp and
// stack_gap_le_6_0_cs_test.cpp (the CS cases were split out to keep each source
// file within the size limit, but they still read the ISOAL parse captures).
namespace OHOS {
namespace Bluetooth {

struct RemoteFeaturesCapture {
    bool received = false;
    HciLeReadAllRemoteFeaturesCompleteEventParam param = {};

    void Reset()
    {
        *this = RemoteFeaturesCapture{};
    }
};

struct CisEstablishedV2ParseCapture {
    bool received = false;
    HciLeCisEstablishedV2EventParam param = {};

    void Reset()
    {
        *this = CisEstablishedV2ParseCapture{};
    }
};

struct BigInfoParseCapture {
    bool received = false;
    HciLeBigInfoAdvertisingReportEventParam param = {};

    void Reset()
    {
        *this = BigInfoParseCapture{};
    }
};

// C++17 inline variables: one object across all TUs that include this header.
inline RemoteFeaturesCapture g_capture;
inline CisEstablishedV2ParseCapture g_v2ParseCapture;
inline BigInfoParseCapture g_bigInfoParseCapture;

// Shared HCI event callback table and the helper that resets it.
inline HciEventCallbacks g_parseCallbacks;

void InitParseCallbacks(void);

void InjectWire(const uint8_t *wire, size_t size);

// Light fixture for BT 6.0 single-board tests that drive the HCI event parser
// directly without booting the full GAP/BTM stack.
class StackGapLe60Test : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp() override;
    void TearDown() override;
};

// Heavy fixture for BT 6.0 single-board tests that need the BTM/GAP stack.
class StackGapLe60BtmTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp(void) override;
    void TearDown(void) override;
};

// ---------------------------------------------------------------------------
// ISO/CIG/BIG test helpers moved here so that stack_gap_le_6_0_cs_test.cpp
// can reuse them while each .cpp stays below the build system's size limit.
// ---------------------------------------------------------------------------

struct IsoCisCtx {
    uint32_t establishedCount = 0;
    uint8_t estStatus = 0xFF;
    uint16_t estHandle = 0xFFFF;
    bool hasV2Ext = false;
    IsoLeCisEstablishedV2Ext v2Ext = {};
    uint32_t disconnectedCount = 0;
    uint16_t discHandle = 0xFFFF;
    uint8_t discReason = 0xFF;
};

void OnCisEstablishedIso(uint8_t status, const IsoLeCisEstablishedInfo *info, void *context);

inline void OnCisDisconnectedIso(uint8_t status, uint16_t cisHandle, uint8_t reason, void *context)
{
    auto *ctx = static_cast<IsoCisCtx *>(context);
    ctx->disconnectedCount++;
    ctx->estStatus = status;
    ctx->discHandle = cisHandle;
    ctx->discReason = reason;
}

class CigCallbackGuard {
public:
    int registered = BT_SUCCESS;

    explicit CigCallbackGuard(void *context)
    {
        cb.cisEstablished = OnCisEstablishedIso;
        cb.cisDisconnected = OnCisDisconnectedIso;
        registered = ISOIF_LeRegisterCigCallback(&cb, context);
    }
    ~CigCallbackGuard()
    {
        (void)ISOIF_LeDeregisterCigCallback();
    }
    CigCallbackGuard(const CigCallbackGuard &) = delete;
    CigCallbackGuard &operator=(const CigCallbackGuard &) = delete;

private:
    IsoLeCigCallback cb = {};
};

struct IsoBigInfoCtx {
    uint32_t count = 0;
    uint16_t syncHandle = 0xFFFF;
    uint8_t numBis = 0xFF;
    uint8_t framing = 0xFF;
};

inline void OnBigInfoReportIso(const IsoLeBigInfoReportInfo *info, void *context)
{
    auto *ctx = static_cast<IsoBigInfoCtx *>(context);
    ctx->count++;
    if (info != nullptr) {
        ctx->syncHandle = info->syncHandle;
        ctx->numBis = info->numBis;
        ctx->framing = info->framing;
    }
}

class BigCallbackGuard {
public:
    int registered = BT_SUCCESS;

    explicit BigCallbackGuard(void *context)
    {
        cb.bigInfoReport = OnBigInfoReportIso;
        registered = ISOIF_LeRegisterBigCallback(&cb, context);
    }
    ~BigCallbackGuard()
    {
        (void)ISOIF_LeDeregisterBigCallback();
    }
    BigCallbackGuard(const BigCallbackGuard &) = delete;
    BigCallbackGuard &operator=(const BigCallbackGuard &) = delete;

private:
    IsoLeBigCallback cb = {};
};
}  // namespace Bluetooth
}  // namespace OHOS

#endif // STACK_GAP_LE_6_0_TEST_H
