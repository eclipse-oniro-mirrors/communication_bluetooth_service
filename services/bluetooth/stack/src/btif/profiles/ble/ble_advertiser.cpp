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

#include "btif/profiles/ble/ble.h"

namespace {

class BtifBleAdvertiserPlaceholder : public BleAdvertiserInterface {
public:
    void RegisterAdvertiser(IdStatusCallback) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void GetOwnAddress(uint8_t advertiserId, GetAddressCallback cb) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void SetParameters(uint8_t advertiserId, AdvertiseParameters params, ParametersCallback cb) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void SetData(int advertiserId, bool setScanRsp, std::vector<uint8_t> data, StatusCallback cb) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

void Enable(uint8_t advertiserId, bool enable, StatusCallback cb, uint16_t duration, uint8_t maxExtAdvEvents,
        StatusCallback timeoutCb) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void Unregister(uint8_t advertiserId) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

void StartAdvertising(uint8_t advertiserId, StatusCallback cb, AdvertiseParameters params,
        std::vector<uint8_t> advertise_data, std::vector<uint8_t> scan_response_data, int timeoutS,
        StatusCallback timeoutCb) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

void StartAdvertisingSet(uint8_t clientId, int regId, IdTxPowerStatusCallback registerCb, AdvertiseParameters params,
        std::vector<uint8_t> advertise_data, std::vector<uint8_t> scan_response_data,
        PeriodicAdvertisingParameters periodic_params, std::vector<uint8_t> periodic_data, uint16_t duration,
        uint8_t maxExtAdvEvents, IdStatusCallback timeoutCb) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void SetPeriodicAdvertisingParameters(int advertiserId, PeriodicAdvertisingParameters parameters,
        StatusCallback cb) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void SetPeriodicAdvertisingData(int advertiserId, std::vector<uint8_t> data, StatusCallback cb) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void SetPeriodicAdvertisingEnable(int advertiserId, bool enable, bool includeAdi, StatusCallback cb) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }

    void RegisterCallbacks(AdvertisingCallbacks* callbacks) override
    {
        // Stub: the stack API for this profile is not wired up yet.
    }
};

} // namespace

BleAdvertiserInterface *BtifGetBleAdvertiserInstance()
{
    static BtifBleAdvertiserPlaceholder instance;
    return &instance;
}
