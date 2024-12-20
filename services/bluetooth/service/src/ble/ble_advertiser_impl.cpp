/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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

#include "ble_advertiser_impl.h"

#include <algorithm>

#include "ble_adapter.h"
#include "ble_defs.h"
#include "ble_feature.h"
#include "ble_properties.h"
#include "ble_utils.h"
#include "securec.h"

namespace OHOS {
namespace bluetooth {
struct BleAdvertiserImpl::impl {
    std::map<uint8_t, BleAdvertiserImplWrapData> advHandleSettingDatas_ {};
    std::recursive_mutex mutex_ {};
    /// For RPA 15 minitues change bluetooth address
    std::mutex rpamutex_ {};
    std::condition_variable cvfull_ {};
    std::vector<uint8_t> advCreateHandles_ {};
    bool operationLast_ = false;
    /// Gap callback pointer
    /// Advertising parameters
    BleAdvParams advParams_ {};
    std::queue<BleAdvertiserImplOp> advHandleQue_ {};
};

BleAdvertiserImpl::BleAdvertiserImpl(
    IBleAdvertiserCallback &callback, IAdapterBle &bleAdapter, utility::Dispatcher &dispatch)
    : callback_(&callback),
      bleAdapter_(&bleAdapter),
      dispatcher_(&dispatch),
      pimpl(std::make_unique<BleAdvertiserImpl::impl>())
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    pimpl->advParams_.advMinInterval = BLE_ADV_MIN_INTERVAL;
    pimpl->advParams_.advMaxInterval = BLE_ADV_MAX_INTERVAL;
    pimpl->advParams_.advType = ADV_TYPE_IND;
    pimpl->advParams_.channelMap = ADV_CHNL_ALL;
    pimpl->advParams_.advFilterPolicy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;
    pimpl->advParams_.peerAddrType = BLE_ADDR_TYPE_PUBLIC;
}

BleAdvertiserImpl::~BleAdvertiserImpl()
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    for (auto iter = pimpl->advHandleSettingDatas_.begin(); iter != pimpl->advHandleSettingDatas_.end(); iter++) {
        if (iter->second.timer_ != nullptr) {
            iter->second.timer_->Stop();
            iter->second.timer_ = nullptr;
        }
    }
    pimpl->advHandleSettingDatas_.clear();
    while (!pimpl->advHandleQue_.empty()) {
        pimpl->advHandleQue_.pop();
    }
}

void BleAdvertiserImpl::AdvSetParamResult(uint8_t status, void *context)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *bleAdvertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((bleAdvertiser != nullptr) && (bleAdvertiser->dispatcher_ != nullptr)) {
        bleAdvertiser->dispatcher_->PostTask(std::bind(
            &BleAdvertiserImpl::HandleGapEvent, bleAdvertiser, BLE_GAP_ADV_PARAM_SET_COMPLETE_EVT, status, 0));
    }
}

void BleAdvertiserImpl::AdvReadTxPower(uint8_t status, int8_t txPower, void *context)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *bleAdvertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((bleAdvertiser != nullptr) && (bleAdvertiser->dispatcher_ != nullptr)) {
        bleAdvertiser->dispatcher_->PostTask(std::bind(
            &BleAdvertiserImpl::HandleGapEvent, bleAdvertiser, BLE_GAP_ADV_READ_TXPOWER_EVT, status, txPower));
    }
}

void BleAdvertiserImpl::AdvSetDataResult(uint8_t status, void *context)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *bleAdvertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((bleAdvertiser != nullptr) && (bleAdvertiser->dispatcher_ != nullptr)) {
        bleAdvertiser->dispatcher_->PostTask(
            std::bind(&BleAdvertiserImpl::HandleGapEvent, bleAdvertiser, BLE_GAP_ADV_DATA_SET_COMPLETE_EVT, status, 0));
    }
}

void BleAdvertiserImpl::AdvSetScanRspDataResult(uint8_t status, void *context)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *bleAdvertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((bleAdvertiser != nullptr) && (bleAdvertiser->dispatcher_ != nullptr)) {
        bleAdvertiser->dispatcher_->PostTask(std::bind(
            &BleAdvertiserImpl::HandleGapEvent, bleAdvertiser, BLE_GAP_ADV_SCAN_RSP_DATA_SET_COMPLETE_EVT, status, 0));
    }
}

void BleAdvertiserImpl::AdvSetEnableResult(uint8_t status, void *context)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *bleAdvertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((bleAdvertiser != nullptr) && (bleAdvertiser->dispatcher_ != nullptr)) {
        bleAdvertiser->dispatcher_->PostTask(
            std::bind(&BleAdvertiserImpl::AdvSetEnableResultTask, bleAdvertiser, status));
        (static_cast<BleAdapter *>(bleAdvertiser->bleAdapter_))->NotifyAllWaitContinue();
    }
}

void BleAdvertiserImpl::AdvSetEnableResultTask(uint8_t status)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    uint8_t tempAdvStatus = ADV_STATUS_DEFAULT;
    std::lock_guard<std::recursive_mutex> lk(pimpl->mutex_);
    if (pimpl->advHandleQue_.empty()) {
        LOG_DEBUG("[BleAdvertiserImpl] %{public}s,error", __func__);
        return;
    }
    auto iter = pimpl->advHandleSettingDatas_.find(pimpl->advHandleQue_.front().advHandle);
    if (iter != pimpl->advHandleSettingDatas_.end()) {
        tempAdvStatus = pimpl->advHandleQue_.front().advOpStatus;
    } else {
        LOG_DEBUG("[BleAdvertiserImpl] %{public}s,error", __func__);
        pimpl->advHandleQue_.pop();
        return;
    }
    switch (tempAdvStatus) {
        case ADV_STATUS_STARTING:
            HandleGapEvent(BLE_GAP_ADV_START_COMPLETE_EVT, status, 0);
            break;
        case ADV_STATUS_STOPING:
            HandleGapEvent(BLE_GAP_ADV_STOP_COMPLETE_EVT, status, 0);
            break;
        default:
            pimpl->advHandleQue_.pop();
            break;
    }
}

int BleAdvertiserImpl::GetAdvertisingStatus() const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    std::lock_guard<std::recursive_mutex> lk(pimpl->mutex_);
    for (auto iter = pimpl->advHandleSettingDatas_.begin(); iter != pimpl->advHandleSettingDatas_.end(); iter++) {
        if (iter->second.advStatus_ == ADVERTISE_FAILED_ALREADY_STARTED) {
            return ADVERTISE_FAILED_ALREADY_STARTED;
        }
    }
    return ADVERTISE_NOT_STARTED;
}

void BleAdvertiserImpl::ReStartLegacyAdvertising() const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    int status = AdapterManager::GetInstance()->GetState(BTTransport::ADAPTER_BLE);
    if (status != BTStateID::STATE_TURN_ON) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Bluetooth adapter is invalid.", __func__);
        return;
    }

    int advHandle = BLE_LEGACY_ADVERTISING_HANDLE;
    auto iter = pimpl->advHandleSettingDatas_.find(advHandle);
    if (iter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, advHandle);
        return;
    }

    if (iter->second.advStatus_ != ADVERTISE_FAILED_ALREADY_STARTED) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Advertising has not started.", __func__);
        return;
    }

    iter->second.advStatus_ = ADVERTISE_FAILED_ALREADY_STARTED;

    /// Start adv
    int ret = SetAdvEnableToGap(true);
    if (ret != BT_SUCCESS) {
        iter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Stop advertising failed! handle = %u.", __func__, iter->first);
    } else {
        pimpl->advHandleQue_.push(BleAdvertiserImplOp(advHandle, ADV_STATUS_STARTING));
    }
}

int BleAdvertiserImpl::CheckAdvertiserPara(const BleAdvertiserSettingsImpl &settings,
    const BleAdvertiserDataImpl &advData, const BleAdvertiserDataImpl &scanResponse) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    int status = AdapterManager::GetInstance()->GetState(BTTransport::ADAPTER_BLE);
    if (status != BTStateID::STATE_TURN_ON) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Bluetooth adapter is invalid.", __func__);
        return BT_BAD_STATUS;
    }

    bool isLegacyMode = settings.IsLegacyMode();
    if (isLegacyMode) {
        if (advData.GetPayload().length() > BLE_LEGACY_ADV_DATA_LEN_MAX) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Legacy advertising data too big.", __func__);
            return BT_BAD_PARAM;
        }
        if (scanResponse.GetPayload().length() > BLE_LEGACY_ADV_DATA_LEN_MAX) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Legacy scan response data too big.", __func__);
            return BT_BAD_PARAM;
        }
    } else {
        bool isCodedPhySupported = BleFeature::GetInstance().IsLeCodedPhySupported();
        bool is2MPhySupported = BleFeature::GetInstance().IsLe2MPhySupported();
        int primaryPhy = settings.GetPrimaryPhy();
        int secondaryPhy = settings.GetSecondaryPhy();

        if ((!isCodedPhySupported) && (primaryPhy == PHY_LE_CODED)) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Unsupported primary coded PHY selected.", __func__);
            return BT_BAD_PARAM;
        }

        if ((!is2MPhySupported) && (secondaryPhy == PHY_LE_CODED)) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Unsupported primary 2M PHY selected.", __func__);
            return BT_BAD_PARAM;
        }

        size_t maxData = GetMaxAdvertisingDataLength(settings);
        if (advData.GetPayload().length() > maxData) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Advertising data too big.", __func__);
            return BT_BAD_PARAM;
        }

        if (scanResponse.GetPayload().length() > maxData) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Scan response data too big.", __func__);
            return BT_BAD_PARAM;
        }
    }
    return BT_SUCCESS;
}

void BleAdvertiserImpl::StartAdvertising(const BleAdvertiserSettingsImpl &settings,
    const BleAdvertiserDataImpl &advData, const BleAdvertiserDataImpl &scanResponse, uint8_t advHandle)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    std::lock_guard<std::recursive_mutex> lk(pimpl->mutex_);
    int advStatus = ADVERTISE_NOT_STARTED;
    auto iter = pimpl->advHandleSettingDatas_.find(advHandle);
    if (iter != pimpl->advHandleSettingDatas_.end()) {
        advStatus = iter->second.advStatus_;
    }

    if (BleFeature::GetInstance().IsLeExtendedAdvertisingSupported()) {
        if (advStatus == ADVERTISE_FAILED_ALREADY_STARTED) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Extend Advertising has started already.", __func__);
            callback_->OnStartResultEvent(ADVERTISE_FAILED_ALREADY_STARTED, advHandle);
            return;
        }
    } else {
        if (advStatus == ADVERTISE_FAILED_ALREADY_STARTED) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Advertising has started already.", __func__);
            callback_->OnStartResultEvent(ADVERTISE_FAILED_ALREADY_STARTED, advHandle);
            return;
        }
    }

    int ret = CheckAdvertiserPara(settings, advData, scanResponse);
    if (ret != BT_SUCCESS) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Check adv parameter failed!.", __func__);
        callback_->OnStartResultEvent(
            ret == BT_BAD_PARAM ? ADVERTISE_FAILED_DATA_TOO_LARGE : ADVERTISE_FAILED_INTERNAL_ERROR,
            advHandle,
            BLE_ADV_START_FAILED_OP_CODE);
        return;
    }

    advStatus = ADVERTISE_FAILED_ALREADY_STARTED;
    if (iter != pimpl->advHandleSettingDatas_.end()) {
        if (iter->second.timer_ != nullptr) {
            iter->second.timer_->Stop();
            iter->second.timer_ = nullptr;
        }
        pimpl->advHandleSettingDatas_.erase(iter);
    }
    pimpl->advHandleSettingDatas_.insert(
        std::make_pair(advHandle, BleAdvertiserImplWrapData(settings, advData, scanResponse, advStatus)));
    StartLegacyAdvOrExAdv(advHandle);
}

void BleAdvertiserImpl::StartLegacyAdvOrExAdv(uint8_t advHandle)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());
    if (BleFeature::GetInstance().IsLeExtendedAdvertisingSupported()) {
        ExtentAdvertising(advHandle);
    } else {
        LegacyAdvertising(advHandle);
    }
}

void BleAdvertiserImpl::LegacyAdvertising(uint8_t advHandle)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    auto iter = pimpl->advHandleSettingDatas_.find(advHandle);
    int ret = RegisterCallbackToGap();
    if (ret != BT_SUCCESS) {
        iter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Set ble roles failed!.", __func__);
        callback_->OnStartResultEvent(ADVERTISE_FAILED_INTERNAL_ERROR, advHandle);
        return;
    }

    ret = SetAdvParamToGap(iter->second.settings_);
    if (ret != BT_SUCCESS) {
        iter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Set adv parameter to gap failed!.", __func__);
        callback_->OnStartResultEvent(ADVERTISE_FAILED_INTERNAL_ERROR, advHandle);
    } else {
        pimpl->advHandleQue_.push(BleAdvertiserImplOp(advHandle, ADV_STATUS_SET_PARA));
    }
}

void BleAdvertiserImpl::ExtentAdvertising(uint8_t advHandle)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    auto iter = pimpl->advHandleSettingDatas_.find(advHandle);
    int ret = RegisterExAdvCallbackToGap();
    if (ret != BT_SUCCESS) {
        iter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        LOG_ERROR("Register ex adv gap callback failed!");
        callback_->OnStartResultEvent(ADVERTISE_FAILED_INTERNAL_ERROR, advHandle, BLE_ADV_START_FAILED_OP_CODE);
        RemoveAdvHandle(advHandle);
        return;
    }

    pimpl->advHandleQue_.push(BleAdvertiserImplOp(advHandle, ADV_STATUS_SET_PARA));
    ret = SetExAdvParamToGap(advHandle, iter->second.settings_);
    if (ret != BT_SUCCESS) {
        iter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        LOG_ERROR("Set adv parameter to gap failed!");
        callback_->OnStartResultEvent(ADVERTISE_FAILED_INTERNAL_ERROR, advHandle, BLE_ADV_START_FAILED_OP_CODE);
        RemoveAdvHandle(advHandle);
    }
}

void BleAdvertiserImpl::StopAdvertising(uint8_t advHandle) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    std::lock_guard<std::recursive_mutex> lk(pimpl->mutex_);
    auto iter = pimpl->advHandleSettingDatas_.find(advHandle);
    if (iter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invlalid handle! %u.", __func__, advHandle);
        return;
    }

    if (iter->second.advStatus_ == ADVERTISE_NOT_STARTED) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:StopAdvertising failed! %u.", __func__, advHandle);
        return;
    }

    int ret;

    iter->second.stopAllAdvType_ = STOP_ADV_TYPE_SINGLE;
    if (iter->second.timer_ != nullptr) {
        iter->second.timer_->Stop();
        iter->second.timer_ = nullptr;
    }
    if (BleFeature::GetInstance().IsLeExtendedAdvertisingSupported()) {
        ret = SetExAdvEnableToGap(advHandle, false);
    } else {
        ret = SetAdvEnableToGap(false);
    }
    if (ret != BT_SUCCESS) {
        iter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Stop advertising failed!.", __func__);
    } else {
        iter->second.advStatus_ = ADVERTISE_NOT_STARTED;
        pimpl->advHandleQue_.push(BleAdvertiserImplOp(advHandle, ADV_STATUS_STOPING));
        LOG_DEBUG("[BleAdvertiserImpl] %{public}s:Stop advertising success!.", __func__);
    }
}

void BleAdvertiserImpl::StartOrStopAllAdvertising(const STOP_ALL_ADV_TYPE &stopAllAdvType, bool isStartAdv) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());
    std::lock_guard<std::recursive_mutex> lk(pimpl->mutex_);
    if (isStartAdv) {
        StartAllAdvertising(stopAllAdvType, isStartAdv);
    } else {
        StopAllAdvertising(stopAllAdvType, isStartAdv);
    }
}

void BleAdvertiserImpl::StartAllAdvertising(const STOP_ALL_ADV_TYPE &stopAllAdvType, bool isStartAdv) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());
    if (BleFeature::GetInstance().IsLeExtendedAdvertisingSupported()) {
        int ret = SetExAdvBatchEnableToGap(isStartAdv);
        if (ret != BT_SUCCESS) {
            UpdateAllAdvertisingStatus(ADVERTISE_FAILED_INTERNAL_ERROR);
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Start extend advertising failed!.", __func__);
        } else {
            UpdateAllAdvertisingStatus(ADVERTISE_FAILED_ALREADY_STARTED);
            auto iter = pimpl->advHandleSettingDatas_.begin();
            pimpl->advHandleQue_.push(BleAdvertiserImplOp(iter->first,
                isStartAdv?ADV_STATUS_STARTING:ADV_STATUS_STOPING));
            for (; iter != pimpl->advHandleSettingDatas_.end(); ++iter) {
                iter->second.stopAllAdvType_ = stopAllAdvType;
            }
            LOG_DEBUG("[BleAdvertiserImpl] %{public}s:Start extend advertising success!.", __func__);
        }
    } else {
        auto iter = pimpl->advHandleSettingDatas_.begin();
        if (iter == pimpl->advHandleSettingDatas_.end()) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Start legacy advertising failed!.", __func__);
            return;
        }

        int ret = SetAdvEnableToGap(isStartAdv);
        if (ret != BT_SUCCESS) {
            iter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:start advertising failed! handle = %u.", __func__, iter->first);
        } else {
            iter->second.stopAllAdvType_ = stopAllAdvType;
            iter->second.advStatus_ = ADVERTISE_FAILED_ALREADY_STARTED;
            pimpl->advHandleQue_.push(BleAdvertiserImplOp(iter->first,
                isStartAdv?ADV_STATUS_STARTING:ADV_STATUS_STOPING));
            LOG_DEBUG("[BleAdvertiserImpl] %{public}s:start advertising success!.", __func__);
        }
    }
}

void BleAdvertiserImpl::StopAllAdvertising(const STOP_ALL_ADV_TYPE &stopAllAdvType, bool isStartAdv) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());
    if (BleFeature::GetInstance().IsLeExtendedAdvertisingSupported()) {
        int ret = SetExAdvBatchEnableToGap(isStartAdv);
        if (ret != BT_SUCCESS) {
            UpdateAllAdvertisingStatus(ADVERTISE_FAILED_INTERNAL_ERROR);
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Stop extend advertising failed!.", __func__);
        } else {
            if (stopAllAdvType != STOP_ADV_TYPE_RESOLVING_LIST) {
                UpdateAllAdvertisingStatus(ADVERTISE_NOT_STARTED);
            }
            auto iter = pimpl->advHandleSettingDatas_.begin();
            pimpl->advHandleQue_.push(BleAdvertiserImplOp(iter->first,
                isStartAdv?ADV_STATUS_STARTING:ADV_STATUS_STOPING));
            for (; iter != pimpl->advHandleSettingDatas_.end(); ++iter) {
                iter->second.stopAllAdvType_ = stopAllAdvType;
            }
            LOG_DEBUG("[BleAdvertiserImpl] %{public}s:Stop extend advertising success!.", __func__);
        }
    } else {
        auto iter = pimpl->advHandleSettingDatas_.begin();
        if (iter == pimpl->advHandleSettingDatas_.end()) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Stop legacy advertising failed!.", __func__);
            return;
        }

        int ret = SetAdvEnableToGap(isStartAdv);
        if (ret != BT_SUCCESS) {
            iter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Stop advertising failed! handle = %u.", __func__, iter->first);
        } else {
            iter->second.stopAllAdvType_ = stopAllAdvType;
            pimpl->advHandleQue_.push(BleAdvertiserImplOp(iter->first,
                isStartAdv?ADV_STATUS_STARTING:ADV_STATUS_STOPING));
            if (stopAllAdvType != STOP_ADV_TYPE_RESOLVING_LIST) {
                iter->second.advStatus_ = ADVERTISE_NOT_STARTED;
            }
            LOG_DEBUG("[BleAdvertiserImpl] %{public}s:Stop advertising success!.", __func__);
        }
    }
}

void BleAdvertiserImpl::UpdateAllAdvertisingStatus(const ADVERTISE_STATUS &advStatus) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());
    auto iter = pimpl->advHandleSettingDatas_.begin();
    for (; iter != pimpl->advHandleSettingDatas_.end(); ++iter) {
        iter->second.advStatus_ = advStatus;
    }
}

void BleAdvertiserImpl::Close(uint8_t advHandle) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    auto iter = pimpl->advHandleSettingDatas_.find(advHandle);
    if (iter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invlalid handle! %u.", __func__, advHandle);
        return;
    }

    StopAdvertising(advHandle);
    iter->second.advStatus_ = ADVERTISE_NOT_STARTED;
}

int BleAdvertiserImpl::RegisterCallbackToGap()
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);
    GapAdvCallback advCallbacks_ {};
    advCallbacks_.advReadTxPower = &BleAdvertiserImpl::AdvReadTxPower;
    advCallbacks_.advSetDataResult = &BleAdvertiserImpl::AdvSetDataResult;
    advCallbacks_.advSetEnableResult = &BleAdvertiserImpl::AdvSetEnableResult;
    advCallbacks_.advSetParamResult = &BleAdvertiserImpl::AdvSetParamResult;
    advCallbacks_.advSetScanRspDataResult = &BleAdvertiserImpl::AdvSetScanRspDataResult;

    return GAPIF_RegisterAdvCallback(&advCallbacks_, this);
}

void BleAdvertiserImpl::ExAdvSetRandAddrResult(uint8_t status, void *context)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *bleAdvertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((bleAdvertiser != nullptr) && (bleAdvertiser->dispatcher_ != nullptr)) {
        bleAdvertiser->dispatcher_->PostTask(std::bind(&BleAdvertiserImpl::HandleGapExAdvEvent,
            bleAdvertiser,
            BLE_GAP_EX_ADV_SET_RAND_ADDR_RESULT_EVT,
            status,
            0,
            0));
    }
}

void BleAdvertiserImpl::ExAdvSetParamResult(uint8_t status, uint8_t selectTxPower, void *context)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *bleAdvertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((bleAdvertiser != nullptr) && (bleAdvertiser->dispatcher_ != nullptr)) {
        bleAdvertiser->dispatcher_->PostTask(std::bind(&BleAdvertiserImpl::HandleGapExAdvEvent,
            bleAdvertiser,
            BLE_GAP_EX_ADV_PARAM_SET_COMPLETE_EVT,
            status,
            selectTxPower,
            0));
    }
}

void BleAdvertiserImpl::ExAdvSetDataResult(uint8_t status, void *context)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *bleAdvertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((bleAdvertiser != nullptr) && (bleAdvertiser->dispatcher_ != nullptr)) {
        bleAdvertiser->dispatcher_->PostTask(std::bind(&BleAdvertiserImpl::HandleGapExAdvEvent,
            bleAdvertiser,
            BLE_GAP_EX_ADV_DATA_SET_COMPLETE_EVT,
            status,
            0,
            0));
    }
}

void BleAdvertiserImpl::ExAdvSetScanRspDataResult(uint8_t status, void *context)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *bleAdvertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((bleAdvertiser != nullptr) && (bleAdvertiser->dispatcher_ != nullptr)) {
        bleAdvertiser->dispatcher_->PostTask(std::bind(&BleAdvertiserImpl::HandleGapExAdvEvent,
            bleAdvertiser,
            BLE_GAP_EX_ADV_SCAN_RSP_DATA_SET_COMPLETE_EVT,
            status,
            0,
            0));
    }
}

void BleAdvertiserImpl::ExAdvSetEnableResult(uint8_t status, void *context)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *bleAdvertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((bleAdvertiser != nullptr) && (bleAdvertiser->dispatcher_ != nullptr)) {
        bleAdvertiser->dispatcher_->PostTask(
            std::bind(&BleAdvertiserImpl::ExAdvSetEnableResultTask, bleAdvertiser, status));
        (static_cast<BleAdapter *>(bleAdvertiser->bleAdapter_))->NotifyAllWaitContinue();
    }
}

void BleAdvertiserImpl::ExAdvSetEnableResultTask(uint8_t status)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    uint8_t tempAdvStatus = ADV_STATUS_DEFAULT;
    std::lock_guard<std::recursive_mutex> lk(pimpl->mutex_);
    if (pimpl->advHandleQue_.empty()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s,error", __func__);
        return;
    }
    auto iter = pimpl->advHandleSettingDatas_.find(pimpl->advHandleQue_.front().advHandle);
    if (iter != pimpl->advHandleSettingDatas_.end()) {
        tempAdvStatus = pimpl->advHandleQue_.front().advOpStatus;
    } else {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s,error", __func__);
        pimpl->advHandleQue_.pop();
        return;
    }
    if (tempAdvStatus == ADV_STATUS_STOPING) {
        switch (iter->second.stopAllAdvType_) {
            case STOP_ADV_TYPE_ALL:
                HandleGapExAdvEvent(BLE_GAP_EX_ALL_ADV_STOP_COMPLETE_EVT, status);
                break;
            case STOP_ADV_TYPE_SINGLE:
                HandleGapExAdvEvent(BLE_GAP_EX_ADV_STOP_COMPLETE_EVT, status);
                break;
            case STOP_ADV_TYPE_RESOLVING_LIST:
                HandleGapExAdvEvent(BLE_GAP_EX_RESOLVING_LIST_ADV_STOP_COMPLETE_EVT, status);
                break;
            default:
                pimpl->advHandleQue_.pop();
                break;
        }
    } else if (tempAdvStatus == ADV_STATUS_STARTING) {
        switch (iter->second.stopAllAdvType_) {
            case STOP_ADV_TYPE_ALL:
            case STOP_ADV_TYPE_SINGLE:
                HandleGapExAdvEvent(BLE_GAP_EX_ADV_START_COMPLETE_EVT, status);
                break;
            case STOP_ADV_TYPE_RESOLVING_LIST:
                HandleGapExAdvEvent(BLE_GAP_EX_RESOLVING_LIST_ADV_START_COMPLETE_EVT, status);
                break;
            default:
                pimpl->advHandleQue_.pop();
                break;
        }
    }
}

void BleAdvertiserImpl::ExAdvRemoveHandleResult(uint8_t status, void *context)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *bleAdvertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((bleAdvertiser != nullptr) && (bleAdvertiser->dispatcher_ != nullptr)) {
        bleAdvertiser->dispatcher_->PostTask(std::bind(&BleAdvertiserImpl::HandleGapExAdvEvent,
            bleAdvertiser,
            BLE_GAP_EX_ADV_REMOVE_HANDLE_RESULT_EVT,
            status,
            0,
            0));
        (static_cast<BleAdapter *>(bleAdvertiser->bleAdapter_))->NotifyAllWaitContinue();
    }
}

void BleAdvertiserImpl::ExAdvClearHandleResult(uint8_t status, void *context)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *bleAdvertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((bleAdvertiser != nullptr) && (bleAdvertiser->dispatcher_ != nullptr)) {
        bleAdvertiser->dispatcher_->PostTask(std::bind(&BleAdvertiserImpl::HandleGapExAdvEvent,
            bleAdvertiser,
            BLE_GAP_EX_ADV_CLEAR_HANDLE_RESULT_EVT,
            status,
            0,
            0));
        (static_cast<BleAdapter *>(bleAdvertiser->bleAdapter_))->NotifyAllWaitContinue();
    }
}

void BleAdvertiserImpl::ExAdvScanRequestReceived(uint8_t advHandle, const BtAddr *scannerAddr, void *context)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *bleAdvertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((bleAdvertiser != nullptr) && (bleAdvertiser->dispatcher_ != nullptr)) {
        bleAdvertiser->dispatcher_->PostTask(std::bind(
            &BleAdvertiserImpl::HandleGapExAdvEvent, bleAdvertiser, BLE_GAP_EX_ADV_SCAN_REQUEST_RECEIVED_EVT, 0, 0, 0));
    }
}

void BleAdvertiserImpl::ExAdvTerminatedAdvSet(
    uint8_t status, uint8_t advHandle, uint16_t connectionHandle, uint8_t completedNumber, void *context)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *bleAdvertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((bleAdvertiser != nullptr) && (bleAdvertiser->dispatcher_ != nullptr) && (status == BT_SUCCESS)) {
        bleAdvertiser->dispatcher_->PostTask(std::bind(&BleAdvertiserImpl::HandleGapExAdvEvent,
            bleAdvertiser,
            BLE_GAP_EX_ADC_TERMINATED_ADV_SET_EVT,
            status,
            0,
            advHandle));
    }
}

int BleAdvertiserImpl::RegisterExAdvCallbackToGap()
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);
    exAdvCallback_.exAdvClearHandleResult = &BleAdvertiserImpl::ExAdvClearHandleResult;
    exAdvCallback_.exAdvRemoveHandleResult = &BleAdvertiserImpl::ExAdvRemoveHandleResult;
    exAdvCallback_.exAdvScanRequestReceived = &BleAdvertiserImpl::ExAdvScanRequestReceived;
    exAdvCallback_.exAdvSetDataResult = &BleAdvertiserImpl::ExAdvSetDataResult;
    exAdvCallback_.exAdvSetEnableResult = &BleAdvertiserImpl::ExAdvSetEnableResult;
    exAdvCallback_.exAdvSetParamResult = &BleAdvertiserImpl::ExAdvSetParamResult;
    exAdvCallback_.exAdvSetRandAddrResult = &BleAdvertiserImpl::ExAdvSetRandAddrResult;
    exAdvCallback_.exAdvSetScanRspDataResult = &BleAdvertiserImpl::ExAdvSetScanRspDataResult;
    exAdvCallback_.exAdvTerminatedAdvSet = &BleAdvertiserImpl::ExAdvTerminatedAdvSet;
    return GAPIF_RegisterExAdvCallback(&exAdvCallback_, this);
}

int BleAdvertiserImpl::DeregisterCallbackToGap()
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    int ret;
    if (BleFeature::GetInstance().IsLeExtendedAdvertisingSupported()) {
        ret = GAPIF_DeregisterExAdvCallback();
        exAdvCallback_ = {};
    } else {
        ret = GAPIF_DeregisterAdvCallback();
    }
    return ret;
}

void BleAdvertiserImpl::SetAdvParam(const BleAdvertiserSettingsImpl &settings) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    SetMaxInterval(settings.GetInterval());
    SetMinInterval(settings.GetInterval());
    SetFilter(false, false);
    SetChannelMap(ADV_CHNL_ALL);
}

int BleAdvertiserImpl::SetAdvParamToGap(const BleAdvertiserSettingsImpl &settings) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    SetAdvParam(settings);
    GapLeAdvParam para;
    BtAddr addr;
    (void)memset_s(&addr, sizeof(addr), 0x00, sizeof(addr));
    para.advIntervalMin = pimpl->advParams_.advMinInterval;
    para.advIntervalMax = pimpl->advParams_.advMaxInterval;
    para.advFilterPolicy = pimpl->advParams_.advFilterPolicy;
    para.advChannelMap = pimpl->advParams_.channelMap;
    para.peerAddr = &addr;

    uint8_t advType = GAP_ADV_TYPE_CONN_UNDIR;
    if (!settings.IsConnectable()) {
        advType = GAP_ADV_TYPE_SCAN_UNDIR;
    }
    return GAPIF_LeAdvSetParam(advType, para);
}

int BleAdvertiserImpl::SetExAdvParamToGap(uint8_t advHandle, const BleAdvertiserSettingsImpl &settings) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    SetAdvParam(settings);
    GapLeExAdvParam para;
    BtAddr addr;
    (void)memset_s(&addr, sizeof(addr), 0x00, sizeof(addr));
    para.advIntervalMin = pimpl->advParams_.advMinInterval;
    para.advIntervalMax = pimpl->advParams_.advMaxInterval;
    para.advFilterPolicy = pimpl->advParams_.advFilterPolicy;
    para.advChannelMap = pimpl->advParams_.channelMap;
    para.peerAddr = &addr;
    para.advSid = 0x01;
    para.primaryAdvPhy = settings.GetPrimaryPhy();
    para.secondaryAdvPhy = settings.GetSecondaryPhy();
    para.scanRequestNotifyEnable = 0x01;
    para.secondaryAdvMaxSkip = 0x00;

    uint8_t advType = 0;
    if (settings.IsLegacyMode()) {
        if (settings.IsConnectable()) {
            advType |= BLE_LEGACY_ADV_IND_WITH_EX_ADV;
        } else {
            advType |= BLE_LEGACY_ADV_NONCONN_IND_WITH_EX_ADV;
        }
    } else {
        if (settings.IsConnectable()) {
            advType |= GAP_ADVERTISING_PROPERTY_CONNECTABLE | GAP_ADVERTISING_PROPERTY_INCLUDE_TXPOWER;
        } else {
            advType |= GAP_ADVERTISING_PROPERTY_SCANABLE | GAP_ADVERTISING_PROPERTY_INCLUDE_TXPOWER;
        }
    }
    return GAPIF_LeExAdvSetParam(advHandle, advType, settings.GetTxPower(), para);
}

int BleAdvertiserImpl::CheckAdvertiserLen(uint8_t payload, uint8_t advType)
{
    switch (advType) {
        case BLE_AD_TYPE_FLAG: {  /// Data Type: 0x01
            if (payload > BLE_LEGACY_ADV_DATA_LEN_MAX) {
                return BT_NOT_SUPPORT;
            } else {
                return BT_SUCCESS;
            }
            break;
        }
        default:
            break;
    }
    return BT_OPERATION_FAILED;
}

int BleAdvertiserImpl::CheckAdvertiserFlag(const std::string &payload) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    size_t sizeConsumed = 0;
    bool finished = false;
    size_t totalLen = payload.size();
    auto iter = payload.begin();

    while ((!finished) && (iter != payload.end())) {
        size_t length = *iter;
        sizeConsumed += 1 + length;
        if (sizeConsumed > totalLen) {
            break;
        }
        iter++;

        if (length != 0) {
            uint8_t advType = *iter;
            iter++;
            length--;

            int ret = CheckAdvertiserLen(*iter, advType);
            if (ret != BT_OPERATION_FAILED) {
                return ret;
            }

            iter += length;
        }
        if (sizeConsumed >= totalLen) {
            finished = true;
        }
    }
    return BT_SUCCESS;
}

int BleAdvertiserImpl::SetAdvDataToGap(
    const BleAdvertiserDataImpl &advData, const BleAdvertiserSettingsImpl &settings, int8_t txPowerLevel) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    BleAdvertiserDataImpl data = static_cast<BleAdvertiserDataImpl>(advData);

    int ret = CheckAdvertiserFlag(data.GetPayload());
    if (ret != BT_SUCCESS) {
        return ret;
    }

    // bluetooth localname
    std::string name = BleProperties::GetInstance().GetLocalName();
    size_t maxDataLen = GetMaxAdvertisingDataLength(settings);
    if ((!data.GetPayload().empty()) && (!name.empty()) &&
        (data.GetPayload().size() + name.size() + BLE_ADV_DATA_FIELD_TYPE_AND_LEN <= maxDataLen)) {
        data.SetDeviceName(name);
    }
    // adv txpower
    if ((!data.GetPayload().empty()) && (data.GetPayload().size() + BLE_ADV_DATA_BYTE_FIELD_LEN <= maxDataLen)) {
        data.SetTxPowerLevel(txPowerLevel);
    }

    if (!data.GetPayload().empty()) {
        std::vector<uint8_t> legacyDatas;
        for (size_t i = 0; i < data.GetPayload().size(); i++) {
            legacyDatas.push_back(data.GetPayload()[i]);
        }
        LOG_INFO("[BleAdvertiserImpl] %{public}s: Advertising Data Data=%{public}s %{public}zu",
            __func__,
            BleUtils::ConvertIntToHexString(legacyDatas).c_str(),
            data.GetPayload().size());
    }
    return GAPIF_LeAdvSetData(data.GetPayload().size(), (uint8_t *)data.GetPayload().c_str());
}

int BleAdvertiserImpl::ExAdvDataFragment(const BleAdvertiserDataImpl &data) const
{
    pimpl->operationLast_ = false;
    uint8_t advStartHandle = pimpl->advHandleQue_.front().advHandle;
    size_t payloadLen = data.GetPayload().size();
    std::string payload = data.GetPayload();
    uint8_t maxlen = BLE_EX_ADV_PAYLOAD_DATA_LEN;
    uint8_t fragment = GAP_CONTROLLER_SHOULD_NOT_FRAGMENT;
    int ret = BT_SUCCESS;
    if ((payloadLen / maxlen == 0) || ((payloadLen / maxlen == 1) && (payloadLen % maxlen == 0))) {
        uint8_t operation = GAP_ADVERTISING_DATA_OPERATION_COMPLETE;
        pimpl->operationLast_ = true;
        ret = GAPIF_LeExAdvSetData(advStartHandle, operation, fragment, payloadLen,
            reinterpret_cast<uint8_t *>(const_cast<char *>(payload.c_str())));
    } else if (((payloadLen / maxlen == 1) && (payloadLen % maxlen > 0)) ||
               ((payloadLen / maxlen == BLE_DIV_RESULT_TWO) && (payloadLen % maxlen == 0))) {
        std::string advData = payload.substr(0, maxlen);
        uint8_t operation = GAP_ADVERTISING_DATA_OPERATION_FIRST;
        ret = GAPIF_LeExAdvSetData(advStartHandle, operation, fragment, maxlen,
            reinterpret_cast<uint8_t *>(const_cast<char *>(advData.c_str())));
        pimpl->operationLast_ = true;
        operation = GAP_ADVERTISING_DATA_OPERATION_LAST;
        uint8_t length = payloadLen - maxlen;
        advData = payload.substr(maxlen, payloadLen - maxlen);
        ret &= GAPIF_LeExAdvSetData(advStartHandle, operation, fragment, length,
            reinterpret_cast<uint8_t *>(const_cast<char *>(advData.c_str())));
    } else if (((payloadLen / maxlen == BLE_DIV_RESULT_TWO) && (payloadLen % maxlen > 0)) ||
               payloadLen / maxlen > BLE_DIV_RESULT_TWO) {
        uint8_t operation = GAP_ADVERTISING_DATA_OPERATION_FIRST;
        std::string advData = payload.substr(0, maxlen);
        ret = GAPIF_LeExAdvSetData(advStartHandle, operation, fragment, maxlen,
            reinterpret_cast<uint8_t *>(const_cast<char *>(advData.c_str())));
        operation = GAP_ADVERTISING_DATA_OPERATION_INTERMEDIATE;
        for (size_t i = 0; i < (payloadLen / maxlen - 1); i++) {
            if ((i == (payloadLen / maxlen - 1) - 1) && (payloadLen - maxlen * (payloadLen / maxlen) == 0)) {
                operation = GAP_ADVERTISING_DATA_OPERATION_LAST;
            }
            uint8_t length = maxlen * (i + 1);
            advData = payload.substr(maxlen * (i + 1), maxlen);
            ret &= GAPIF_LeExAdvSetData(advStartHandle, operation, fragment, length,
                reinterpret_cast<uint8_t *>(const_cast<char *>(advData.c_str())));
        }
        pimpl->operationLast_ = true;
        if (payloadLen - maxlen * (payloadLen / maxlen) > 0) {
            ret &= GAPIF_LeExAdvSetData(advStartHandle, GAP_ADVERTISING_DATA_OPERATION_LAST, fragment,
                payloadLen - maxlen * (payloadLen / maxlen),
                (uint8_t *)payload.substr(maxlen * (payloadLen / maxlen), payloadLen - maxlen * (payloadLen / maxlen))
                    .c_str());
        }
    }
    return ret;
}

int BleAdvertiserImpl::ExResDataFragment(const BleAdvertiserDataImpl &data) const
{
    pimpl->operationLast_ = false;
    uint8_t advStartHandle = pimpl->advHandleQue_.front().advHandle;
    size_t payloadLen = data.GetPayload().size();
    std::string payload = data.GetPayload();
    uint8_t fragmentPreference = GAP_CONTROLLER_SHOULD_NOT_FRAGMENT;
    int ret = BT_SUCCESS;
    int maxlen = BLE_EX_ADV_PAYLOAD_DATA_LEN;
    if ((payloadLen / maxlen == 0) || ((payloadLen / maxlen == 1) && (payloadLen % maxlen == 0))) {
        uint8_t operation = GAP_ADVERTISING_DATA_OPERATION_COMPLETE;
        pimpl->operationLast_ = true;
        ret = GAPIF_LeExAdvSetScanRspData(advStartHandle, operation, fragmentPreference, payloadLen,
            reinterpret_cast<uint8_t *>(const_cast<char *>(payload.c_str())));
    } else if (((payloadLen / maxlen == 1) && (payloadLen % maxlen > 0)) ||
               ((payloadLen / maxlen == BLE_DIV_RESULT_TWO) && (payloadLen % maxlen == 0))) {
        uint8_t operation = GAP_ADVERTISING_DATA_OPERATION_FIRST;
        ret = GAPIF_LeExAdvSetScanRspData(advStartHandle, operation, fragmentPreference, maxlen,
            reinterpret_cast<uint8_t *>(const_cast<char *>(payload.substr(maxlen).c_str())));
        pimpl->operationLast_ = true;
        operation = GAP_ADVERTISING_DATA_OPERATION_LAST;
        ret &= GAPIF_LeExAdvSetScanRspData(advStartHandle, operation, fragmentPreference, payloadLen - maxlen,
            reinterpret_cast<uint8_t *>(const_cast<char *>(payload.substr(maxlen, payloadLen - maxlen).c_str())));
    } else if (payloadLen / maxlen > 1) {
        uint8_t operation = GAP_ADVERTISING_DATA_OPERATION_FIRST;
        ret = GAPIF_LeExAdvSetScanRspData(advStartHandle, operation, fragmentPreference, maxlen,
            reinterpret_cast<uint8_t *>(const_cast<char *>(payload.substr(maxlen).c_str())));
        operation = static_cast<uint8_t>(GAP_ADVERTISING_DATA_OPERATION_INTERMEDIATE);
        for (size_t i = 0; i < (payloadLen / maxlen - 1); i++) {
            ret &= GAPIF_LeExAdvSetScanRspData(advStartHandle, operation, fragmentPreference, maxlen * (i + 1),
                reinterpret_cast<uint8_t *>(const_cast<char *>(payload.substr(maxlen * (i + 1), maxlen).c_str())));
        }
        pimpl->operationLast_ = true;
        operation = GAP_ADVERTISING_DATA_OPERATION_LAST;
        ret &= GAPIF_LeExAdvSetScanRspData(advStartHandle, operation, fragmentPreference,
            data.GetPayload().size() - maxlen *(payloadLen / maxlen),
            reinterpret_cast<uint8_t *>(const_cast<char *>(payload.substr(maxlen * (payloadLen / maxlen),
                payloadLen - maxlen * (payloadLen / maxlen)).c_str())));
    }
    if (ret == BT_SUCCESS) {
        pimpl->advHandleQue_.push(BleAdvertiserImplOp(advStartHandle, ADV_STATUS_SET_SCAN_DATA));
    }
    return ret;
}

int BleAdvertiserImpl::SetExAdvDataToGap(
    const BleAdvertiserDataImpl &advData, const BleAdvertiserSettingsImpl &settings, int8_t txPowerLevel) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    BleAdvertiserDataImpl data = static_cast<BleAdvertiserDataImpl>(advData);
    // bluetooth localname
    std::string name = BleProperties::GetInstance().GetLocalName();
    size_t maxData = GetMaxAdvertisingDataLength(settings);
    if ((!data.GetPayload().empty()) && (!name.empty()) && (data.GetPayload().size() + name.size() <= maxData)) {
        data.SetDeviceName(name);
    }
    // adv txpower
    if ((!data.GetPayload().empty()) && (data.GetPayload().size() + BLE_ADV_DATA_BYTE_FIELD_LEN <= maxData)) {
        data.SetTxPowerLevel(txPowerLevel);
    }

    if (!data.GetPayload().empty()) {
        std::vector<uint8_t> exAdvDatas;
        for (size_t i = 0; i < data.GetPayload().size(); i++) {
            exAdvDatas.push_back(data.GetPayload()[i]);
        }
        LOG_INFO("[BleAdvertiserImpl] %{public}s: Advertising Data Data=%{public}s %{public}zu",
            __func__,
            BleUtils::ConvertIntToHexString(exAdvDatas).c_str(),
            data.GetPayload().size());
    }
    // fragment data
    return ExAdvDataFragment(data);
}

int BleAdvertiserImpl::SetAdvScanRspDataToGap(
    const BleAdvertiserDataImpl &scanResponse, const BleAdvertiserSettingsImpl &settings, int8_t txPowerLevel) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    BleAdvertiserDataImpl legacyScandata = static_cast<BleAdvertiserDataImpl>(scanResponse);
    // bluetooth localname
    std::string name = BleProperties::GetInstance().GetLocalName();
    size_t maxDataLen = GetMaxAdvertisingDataLength(settings);
    if ((!name.empty()) &&
        (legacyScandata.GetPayload().size() + name.size() + BLE_ADV_DATA_FIELD_TYPE_AND_LEN <= maxDataLen)) {
        legacyScandata.SetDeviceName(name);
    }

    if (!legacyScandata.GetPayload().empty()) {
        std::vector<uint8_t> legacyDatas;
        for (size_t i = 0; i < legacyScandata.GetPayload().size(); i++) {
            legacyDatas.push_back(legacyScandata.GetPayload()[i]);
        }
        LOG_INFO("[BleAdvertiserImpl] %{public}s: Advertising Data Data=%{public}s %{public}zu",
            __func__,
            BleUtils::ConvertIntToHexString(legacyDatas).c_str(),
            legacyScandata.GetPayload().size());
    }
    return GAPIF_LeAdvSetScanRspData(
        legacyScandata.GetPayload().size(), (uint8_t *)legacyScandata.GetPayload().c_str());
}

int BleAdvertiserImpl::SetExAdvScanRspDataToGap(
    const BleAdvertiserDataImpl &scanResponse, const BleAdvertiserSettingsImpl &settings, int8_t txPowerLevel) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());
    if (pimpl->advHandleQue_.empty()) {
        return BT_BAD_PARAM;
    }
    uint8_t advStartHandle = pimpl->advHandleQue_.front().advHandle;
    BleAdvertiserDataImpl exAdvScandata = static_cast<BleAdvertiserDataImpl>(scanResponse);
    auto iter = pimpl->advHandleSettingDatas_.find(advStartHandle);
    if (iter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, advStartHandle);
        return BT_BAD_PARAM;
    }
    // bluetooth localname
    std::string name = BleProperties::GetInstance().GetLocalName();
    size_t maxData = GetMaxAdvertisingDataLength(settings);
    if ((!iter->second.settings_.IsConnectable()) && (!name.empty()) &&
        (exAdvScandata.GetPayload().size() + name.size() + BLE_ADV_DATA_FIELD_TYPE_AND_LEN <= maxData)) {
        exAdvScandata.SetDeviceName(name);
    }

    if (!exAdvScandata.GetPayload().empty()) {
        std::vector<uint8_t> exAdvDatas;
        for (size_t i = 0; i < exAdvScandata.GetPayload().size(); i++) {
            exAdvDatas.push_back(exAdvScandata.GetPayload()[i]);
        }
        LOG_INFO("[BleAdvertiserImpl] %{public}s: Advertising Data Data=%{public}s %{public}zu",
            __func__,
            BleUtils::ConvertIntToHexString(exAdvDatas).c_str(),
            exAdvScandata.GetPayload().size());
    }
    // fragment data
    return ExResDataFragment(exAdvScandata);
}

int BleAdvertiserImpl::SetAdvEnableToGap(bool isEnable) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    return GAPIF_LeAdvSetEnable(isEnable);
}

int BleAdvertiserImpl::SetExAdvEnableToGap(uint8_t advHandle, bool isEnable) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    GapExAdvSet exAdvSet;
    exAdvSet.advHandle = advHandle;
    exAdvSet.duration = 0;
    exAdvSet.maxExAdvEvt = 0;
    return GAPIF_LeExAdvSetEnable(isEnable, 0x01, &exAdvSet);
}

int BleAdvertiserImpl::SetExAdvBatchEnableToGap(bool isEnable) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());
    std::vector<GapExAdvSet> advSets;
    auto iter = pimpl->advHandleSettingDatas_.begin();
    for (; iter != pimpl->advHandleSettingDatas_.end(); iter++) {
        if (iter->second.timer_ != nullptr) {
            iter->second.timer_->Stop();
        }

        GapExAdvSet exAdvSets;
        exAdvSets.advHandle = iter->first;
        exAdvSets.duration = 0;
        exAdvSets.maxExAdvEvt = 0;
        advSets.push_back(exAdvSets);
    }
    return GAPIF_LeExAdvSetEnable(isEnable, advSets.size(), &advSets[0]);
}

void BleAdvertiserImpl::SetMinInterval(uint16_t mininterval) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    pimpl->advParams_.advMinInterval = mininterval;
}

void BleAdvertiserImpl::SetMaxInterval(uint16_t maxinterval) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    pimpl->advParams_.advMaxInterval = maxinterval;
}

void BleAdvertiserImpl::SetFilter(bool scanReqWhitelistOnly, bool connectWhitelistOnly) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    if ((!scanReqWhitelistOnly) && (!connectWhitelistOnly)) {
        pimpl->advParams_.advFilterPolicy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;
        return;
    }
    if ((scanReqWhitelistOnly) && (!connectWhitelistOnly)) {
        pimpl->advParams_.advFilterPolicy = ADV_FILTER_ALLOW_SCAN_WLST_CON_ANY;
        return;
    }
    if ((!scanReqWhitelistOnly) && (connectWhitelistOnly)) {
        pimpl->advParams_.advFilterPolicy = ADV_FILTER_ALLOW_SCAN_ANY_CON_WLST;
        return;
    }
    if ((scanReqWhitelistOnly) && (connectWhitelistOnly)) {
        pimpl->advParams_.advFilterPolicy = ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST;
        return;
    }
}

void BleAdvertiserImpl::SetChannelMap(const BLE_ADV_CHANNEL &channelMap) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    pimpl->advParams_.channelMap = channelMap;
}

uint8_t BleAdvertiserImpl::CreateAdvertiserSetHandle() const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    uint8_t handle = BLE_INVALID_ADVERTISING_HANDLE;
    if (!BleFeature::GetInstance().IsLeExtendedAdvertisingSupported()) {
        handle = BLE_LEGACY_ADVERTISING_HANDLE;
        std::vector<uint8_t>::iterator ret =
            std::find(pimpl->advCreateHandles_.begin(), pimpl->advCreateHandles_.end(), handle);
        if (ret == pimpl->advCreateHandles_.end()) {
            pimpl->advCreateHandles_.push_back(handle);
        }
        return handle;
    }

    uint8_t newHandle = BLE_LEGACY_ADVERTISING_HANDLE;
    uint8_t end = BleFeature::GetInstance().GetBleExAdvGetMaxHandleNum();
    for (; newHandle < end; ++newHandle) {
        std::vector<uint8_t>::iterator ret =
            std::find(pimpl->advCreateHandles_.begin(), pimpl->advCreateHandles_.end(), newHandle);
        if (ret == pimpl->advCreateHandles_.end()) {
            pimpl->advCreateHandles_.push_back(newHandle);
            LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}d", __func__, newHandle);
            return newHandle;
        }
    }
    return handle;
}

void BleAdvertiserImpl::RemoveAdvHandle(uint8_t handle) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    if (!pimpl->advCreateHandles_.empty()) {
        pimpl->advCreateHandles_.erase(remove(pimpl->advCreateHandles_.begin(), pimpl->advCreateHandles_.end(), handle),
            pimpl->advCreateHandles_.end());
    }

    auto iter = pimpl->advHandleSettingDatas_.begin();
    while (iter != pimpl->advHandleSettingDatas_.end()) {
        if (iter->first == handle) {
            if (iter->second.timer_ != nullptr) {
                iter->second.timer_->Stop();
                iter->second.timer_ = nullptr;
            }
            pimpl->advHandleSettingDatas_.erase(iter++);
        } else {
            ++iter;
        }
    }
}

void BleAdvertiserImpl::RemoveAllAdvHandle(int status) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());
    pimpl->advCreateHandles_.clear();
    for (auto iter = pimpl->advHandleSettingDatas_.begin(); iter != pimpl->advHandleSettingDatas_.end(); iter++) {
        if (iter->second.timer_ != nullptr) {
            iter->second.timer_->Stop();
            iter->second.timer_ = nullptr;
        }
    }
    pimpl->advHandleSettingDatas_.clear();
}

void BleAdvertiserImpl::AddCharacteristicValue(uint8_t adtype, const std::string &data) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());
    if (pimpl->advHandleQue_.empty()) {
        return;
    }
    uint8_t advStartHandle = pimpl->advHandleQue_.front().advHandle;
    auto iter = pimpl->advHandleSettingDatas_.find(advStartHandle);
    if (iter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, advStartHandle);
        return;
    }
    iter->second.advData_.AddCharacteristicValue(adtype, data);
}

void BleAdvertiserImpl::GapAdvParamSetCompleteEvt(int status) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());
    if (pimpl->advHandleQue_.empty()) {
        return;
    }
    uint8_t advStartHandle = pimpl->advHandleQue_.front().advHandle;
    pimpl->advHandleQue_.pop();
    auto paraIter = pimpl->advHandleSettingDatas_.find(advStartHandle);
    if (paraIter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, advStartHandle);
        return;
    }

    if (status != BT_SUCCESS) {
        paraIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        callback_->OnStartResultEvent(ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle);
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Set adv param failed! %{public}d.", __func__, status);
        return;
    }
    int ret = GAPIF_LeAdvReadTxPower();
    if (ret != BT_SUCCESS) {
        paraIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Read adv tx power from gap failed! %{public}d.", __func__, ret);
        callback_->OnStartResultEvent(ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle);
    } else {
        pimpl->advHandleQue_.push(BleAdvertiserImplOp(advStartHandle, ADV_STATUS_READ_TX_POWER));
    }
}

void BleAdvertiserImpl::GapAdvReadTxPowerEvt(int status, int8_t txPower) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    if (pimpl->advHandleQue_.empty()) {
        return;
    }
    uint8_t advStartHandle = pimpl->advHandleQue_.front().advHandle;
    pimpl->advHandleQue_.pop();
    auto txPowerIter = pimpl->advHandleSettingDatas_.find(advStartHandle);
    if (txPowerIter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, advStartHandle);
        return;
    }

    if (status != BT_SUCCESS) {
        txPowerIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        callback_->OnStartResultEvent(ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle);
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Read tx power failed! %{public}d.", __func__, status);
        return;
    }
    int ret = SetAdvDataToGap(txPowerIter->second.advData_, txPowerIter->second.settings_, txPower);
    if (ret != BT_SUCCESS) {
        txPowerIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Set adv data to gap failed! %{public}d.", __func__, ret);
        callback_->OnStartResultEvent(ADVERTISE_FAILED_DATA_TOO_LARGE, advStartHandle);
    } else {
        pimpl->advHandleQue_.push(BleAdvertiserImplOp(advStartHandle, ADV_STATUS_SET_ADV_DATA));
    }
}

void BleAdvertiserImpl::GapAdvDataSetCompleteEvt(int status, int8_t txPower) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    if (pimpl->advHandleQue_.empty()) {
        return;
    }
    uint8_t advStartHandle = pimpl->advHandleQue_.front().advHandle;
    pimpl->advHandleQue_.pop();
    auto dataIter = pimpl->advHandleSettingDatas_.find(advStartHandle);
    if (dataIter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, advStartHandle);
        return;
    }

    if (status != BT_SUCCESS) {
        dataIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        callback_->OnStartResultEvent(ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle);
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Set adv data failed! %{public}d.", __func__, status);
        return;
    }
    int ret = SetAdvScanRspDataToGap(dataIter->second.rspData_, dataIter->second.settings_, txPower);
    if (ret != BT_SUCCESS) {
        dataIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Set adv response data to gap failed! %{public}d.", __func__, ret);
        callback_->OnStartResultEvent(ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle);
    } else {
        pimpl->advHandleQue_.push(BleAdvertiserImplOp(advStartHandle, ADV_STATUS_SET_SCAN_DATA));
    }
}

void BleAdvertiserImpl::GapAdvScanRspDataSetCompleteEvt(int status) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    if (pimpl->advHandleQue_.empty()) {
        return;
    }
    uint8_t advStartHandle = pimpl->advHandleQue_.front().advHandle;
    pimpl->advHandleQue_.pop();
    auto scanResIter = pimpl->advHandleSettingDatas_.find(pimpl->advHandleQue_.front().advHandle);
    if (scanResIter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, advStartHandle);
        return;
    }

    if (status != BT_SUCCESS) {
        scanResIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        callback_->OnStartResultEvent(ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle);
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Set scan response data failed! %{public}d.", __func__, status);
        return;
    }
    int ret = SetAdvEnableToGap(true);
    if (ret != BT_SUCCESS) {
        scanResIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Start advertising failed! %{public}d.", __func__, ret);
        callback_->OnStartResultEvent(ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle);
    } else {
        pimpl->advHandleQue_.push(BleAdvertiserImplOp(advStartHandle, ADV_STATUS_STARTING));
    }
}

void BleAdvertiserImpl::GapAdvStartCompleteEvt(int status)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    if (pimpl->advHandleQue_.empty()) {
        return;
    }
    uint8_t advStartHandle = pimpl->advHandleQue_.front().advHandle;
    auto startIter = pimpl->advHandleSettingDatas_.find(pimpl->advHandleQue_.front().advHandle);
    pimpl->advHandleQue_.pop();
    if (startIter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, advStartHandle);
        return;
    }

    if (startIter->second.stopAllAdvType_ == STOP_ADV_TYPE_RESOLVING_LIST) {
        if (status != BT_SUCCESS) {
            startIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Start advertising failed! %{public}d.", __func__, status);
        } else {
            if (startIter->second.timer_ != nullptr) {
                int inerval = BLE_CHANGE_RPA_ADDRESS_INTERVAL;
                startIter->second.timer_->Start(inerval, true);
            }
        }
        return;
    }

    if (status != BT_SUCCESS) {
        startIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Start advertising failed! %{public}d.", __func__, status);
        callback_->OnStartResultEvent(ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle);
        return;
    }
    callback_->OnStartResultEvent(status, advStartHandle);
    if (BleConfig::GetInstance().GetBleAddrType() == BLE_ADDR_TYPE_RPA) {
        if (startIter->second.timer_ == nullptr) {
            startIter->second.timer_ =
                std::make_unique<utility::Timer>(std::bind(&TimerCallback, this, advStartHandle));
            int inerval = BLE_CHANGE_RPA_ADDRESS_INTERVAL;
            startIter->second.timer_->Start(inerval, true);
        }
    }
    (static_cast<BleAdapter *>(bleAdapter_))->OnStartAdvertisingEvt();
}

void BleAdvertiserImpl::GapAdvStopCompleteEvt(int status) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    if (pimpl->advHandleQue_.empty()) {
        return;
    }
    uint8_t advStopHandle = pimpl->advHandleQue_.front().advHandle;
    pimpl->advHandleQue_.pop();
    auto stopIter = pimpl->advHandleSettingDatas_.find(advStopHandle);
    if (stopIter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, advStopHandle);
        return;
    }

    switch (stopIter->second.stopAllAdvType_) {
        case STOP_ADV_TYPE_SINGLE:
            break;
        case STOP_ADV_TYPE_ALL:
            callback_->OnAutoStopAdvEvent(advStopHandle);
            break;
        case STOP_ADV_TYPE_RESOLVING_LIST:
            if (status != BT_SUCCESS) {
                stopIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
                if (stopIter->second.timer_ != nullptr) {
                    int interval = BLE_CHANGE_RPA_ADDRESS_INTERVAL;
                    stopIter->second.timer_->Start(interval, true);
                }
                LOG_ERROR("[BleAdvertiserImpl] %{public}s:Stop advertising failed! %{public}d.", __func__, status);
            }
            return;
        default:
            break;
    }

    if (status != BT_SUCCESS) {
        stopIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Stop advertising failed! %{public}d.", __func__, status);
        return;
    }
    RemoveAdvHandle(advStopHandle);
    (static_cast<BleAdapter *>(bleAdapter_))->OnStopAdvertisingEvt();
}

void BleAdvertiserImpl::HandleGapEvent(const BLE_GAP_CB_EVENT &event, int status, int8_t txPower)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s:[event no: %{public}d]", __func__, static_cast<int>(event));
    std::lock_guard<std::recursive_mutex> lk(pimpl->mutex_);
    switch (event) {
        case BLE_GAP_ADV_PARAM_SET_COMPLETE_EVT:
            GapAdvParamSetCompleteEvt(status);
            break;
        case BLE_GAP_ADV_READ_TXPOWER_EVT:
            GapAdvReadTxPowerEvt(status, txPower);
            break;
        case BLE_GAP_ADV_DATA_SET_COMPLETE_EVT:
            GapAdvDataSetCompleteEvt(status, txPower);
            break;
        case BLE_GAP_ADV_SCAN_RSP_DATA_SET_COMPLETE_EVT:
            GapAdvScanRspDataSetCompleteEvt(status);
            break;
        case BLE_GAP_ADV_START_COMPLETE_EVT:
            GapAdvStartCompleteEvt(status);
            break;
        case BLE_GAP_ADV_STOP_COMPLETE_EVT:
            GapAdvStopCompleteEvt(status);
            break;
        default:
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Invalid event! %{public}d.", __func__, event);
            break;
    }
}

void BleAdvertiserImpl::GapExAdvSetRandAddrResultEvt(int status) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    if (pimpl->advHandleQue_.empty()) {
        return;
    }
    uint8_t advStartHandle = pimpl->advHandleQue_.front().advHandle;
    pimpl->advHandleQue_.pop();
    pimpl->cvfull_.notify_all();
    auto iter = pimpl->advHandleSettingDatas_.find(advStartHandle);
    if (iter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, advStartHandle);
        return;
    }

    if (status != BT_SUCCESS) {
        LOG_ERROR("Set rand addr failed! %{public}d", status);
        callback_->OnStartResultEvent(
            ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle, BLE_ADV_START_FAILED_OP_CODE);
        iter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        RemoveAdvHandle(advStartHandle);
        return;
    }
    int ret = SetExAdvEnableToGap(advStartHandle, true);
    if (ret != BT_SUCCESS) {
        LOG_ERROR("Start ex advertising failed!");
        iter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        callback_->OnStartResultEvent(
            ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle, BLE_ADV_START_FAILED_OP_CODE);
        RemoveAdvHandle(advStartHandle);
    } else {
        pimpl->advHandleQue_.push(BleAdvertiserImplOp(advStartHandle, ADV_STATUS_STARTING));
    }
}

void BleAdvertiserImpl::GapExAdvParamSetCompleteEvt(int status, int8_t txPower) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    if (pimpl->advHandleQue_.empty()) {
        return;
    }
    uint8_t advStartHandle = pimpl->advHandleQue_.front().advHandle;
    auto iter = pimpl->advHandleSettingDatas_.find(advStartHandle);
    if (iter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, advStartHandle);
        return;
    }

    if (status != BT_SUCCESS) {
        LOG_ERROR("Set ex adv parameter failed! %{public}d", status);
        iter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        callback_->OnStartResultEvent(
            ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle, BLE_ADV_START_FAILED_OP_CODE);
        RemoveAdvHandle(advStartHandle);
        return;
    }
    if ((iter->second.settings_.IsConnectable()) || (iter->second.settings_.IsLegacyMode())) {
        int ret = SetExAdvDataToGap(iter->second.advData_, iter->second.settings_, txPower);
        pimpl->advHandleQue_.pop();
        if (ret != BT_SUCCESS) {
            LOG_ERROR("Set ex adv data to gap failed!");
            iter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
            callback_->OnStartResultEvent(
                ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle, BLE_ADV_START_FAILED_OP_CODE);
            RemoveAdvHandle(advStartHandle);
            return;
        }
        pimpl->advHandleQue_.push(BleAdvertiserImplOp(advStartHandle, ADV_STATUS_SET_ADV_DATA));
    } else {
        int ret = SetExAdvScanRspDataToGap(iter->second.rspData_, iter->second.settings_, txPower);
        pimpl->advHandleQue_.pop();
        if (ret != BT_SUCCESS) {
            LOG_ERROR("Set ex adv response data to gap failed!");
            iter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
            callback_->OnStartResultEvent(
                ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle, BLE_ADV_START_FAILED_OP_CODE);
            RemoveAdvHandle(advStartHandle);
            return;
        }
    }
}

void BleAdvertiserImpl::GapExAdvDataSetCompleteEvt(int status, int8_t txPower)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    uint8_t advStartHandle = pimpl->advHandleQue_.front().advHandle;
    auto exAdvDataIter = pimpl->advHandleSettingDatas_.find(advStartHandle);
    if (exAdvDataIter == pimpl->advHandleSettingDatas_.end()) {
        pimpl->advHandleQue_.pop();
        return;
    }

    if (status != BT_SUCCESS) {
        LOG_ERROR("Set ex adv data failed! %{public}d", status);
        callback_->OnStartResultEvent(
            ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle, BLE_ADV_START_FAILED_OP_CODE);
        exAdvDataIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        RemoveAdvHandle(advStartHandle);
        pimpl->advHandleQue_.pop();
        return;
    }

    if (exAdvDataIter->second.settings_.IsLegacyMode()) {
        int ret = SetExAdvScanRspDataToGap(exAdvDataIter->second.rspData_, exAdvDataIter->second.settings_, txPower);
        pimpl->advHandleQue_.pop();
        if (ret != BT_SUCCESS) {
            LOG_ERROR("Set ex adv response data to gap failed!");
            exAdvDataIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
            callback_->OnStartResultEvent(
                ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle, BLE_ADV_START_FAILED_OP_CODE);
            RemoveAdvHandle(advStartHandle);
        }
    } else {
        /// Generate rpa address
        if ((BleConfig::GetInstance().GetBleAddrType() == BLE_ADDR_TYPE_RPA) && (pimpl->operationLast_)) {
            std::unique_lock<std::mutex> exAdvDataLock(pimpl->rpamutex_);
            int ret = GAPIF_LeGenResPriAddr(&BleAdvertiserImpl::GenResPriAddrResult, this);
            if (ret != BT_SUCCESS) {
                LOG_ERROR("[BleAdvertiserImpl] %{public}s:GAP_LeGenResPriAddrAsync failed!", __func__);
            }
        } else if (pimpl->operationLast_) {
            pimpl->advHandleQue_.pop();
            int ret = SetExAdvEnableToGap(advStartHandle, true);
            if (ret != BT_SUCCESS) {
                LOG_ERROR("Start ex advertising failed!");
                exAdvDataIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
                callback_->OnStartResultEvent(
                    ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle, BLE_ADV_START_FAILED_OP_CODE);
                RemoveAdvHandle(advStartHandle);
            } else {
                pimpl->advHandleQue_.push(BleAdvertiserImplOp(advStartHandle, ADV_STATUS_STARTING));
            }
        }
    }
}

void BleAdvertiserImpl::GapExAdvScanRspDataSetCompleteEvt(int status)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    if (pimpl->advHandleQue_.empty()) {
        return;
    }
    uint8_t advStartHandle = pimpl->advHandleQue_.front().advHandle;
    auto exAdvScanDataIter = pimpl->advHandleSettingDatas_.find(advStartHandle);
    if (exAdvScanDataIter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, advStartHandle);
        pimpl->advHandleQue_.pop();
        return;
    }

    if (status != BT_SUCCESS) {
        LOG_ERROR("Set ex scan response data failed! %{public}d", status);
        callback_->OnStartResultEvent(
            ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle, BLE_ADV_START_FAILED_OP_CODE);
        exAdvScanDataIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        RemoveAdvHandle(advStartHandle);
        pimpl->advHandleQue_.pop();
        return;
    }
    if ((BleConfig::GetInstance().GetBleAddrType() == BLE_ADDR_TYPE_RPA) && (pimpl->operationLast_)) {
        /// Generate rpa address
        std::unique_lock<std::mutex> exAdvScanResLock(pimpl->rpamutex_);
        int ret = GAPIF_LeGenResPriAddr(&BleAdvertiserImpl::GenResPriAddrResult, this);
        if (ret != BT_SUCCESS) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:GAP_LeGenResPriAddrAsync failed!", __func__);
        }
    } else if (pimpl->operationLast_) {
        pimpl->advHandleQue_.pop();
        int ret = SetExAdvEnableToGap(advStartHandle, true);
        if (ret != BT_SUCCESS) {
            LOG_ERROR("Start ex advertising failed!");
            exAdvScanDataIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
            callback_->OnStartResultEvent(
                ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle, BLE_ADV_START_FAILED_OP_CODE);
            RemoveAdvHandle(advStartHandle);
        } else {
            pimpl->advHandleQue_.push(BleAdvertiserImplOp(advStartHandle, ADV_STATUS_STARTING));
        }
    }
}

void BleAdvertiserImpl::GapExAdvStartCompleteEvt(int status)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    if (pimpl->advHandleQue_.empty()) {
        return;
    }
    uint8_t advStartHandle = pimpl->advHandleQue_.front().advHandle;
    pimpl->advHandleQue_.pop();
    auto exAdvStartIter = pimpl->advHandleSettingDatas_.find(advStartHandle);
    if (exAdvStartIter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, advStartHandle);
        return;
    }

    if (status != BT_SUCCESS) {
        LOG_ERROR("Start ex advertising failed! %{public}d", status);
        callback_->OnStartResultEvent(
            ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle, BLE_ADV_START_FAILED_OP_CODE);
        exAdvStartIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        RemoveAdvHandle(advStartHandle);
        return;
    }

    callback_->OnStartResultEvent(status, advStartHandle);
    if (BleConfig::GetInstance().GetBleAddrType() == BLE_ADDR_TYPE_RPA) {
        if (exAdvStartIter->second.timer_ == nullptr) {
            exAdvStartIter->second.timer_ =
                std::make_unique<utility::Timer>(std::bind(&TimerCallbackEx, this, advStartHandle));
            int inerval = BLE_CHANGE_RPA_ADDRESS_INTERVAL;
            exAdvStartIter->second.timer_->Start(inerval, true);
        }
    }

    if (bleAdapter_ != nullptr) {
        (static_cast<BleAdapter *>(bleAdapter_))->OnStartAdvertisingEvt();
    }
}

void BleAdvertiserImpl::GAPExAdvClearHandle()
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    if (exAdvCallback_.exAdvClearHandleResult == nullptr) {
        RegisterExAdvCallbackToGap();
    }

    int ret = GAPIF_LeExAdvClearHandle();
    if (ret != BT_SUCCESS) {
        LOG_ERROR("GAPIF_LeExAdvClearHandle failed! %{public}d", ret);
    }
}

void BleAdvertiserImpl::GapExAdvStopAllCompleteEvt(int status) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());
    pimpl->advHandleQue_.pop();
    if (status != BT_SUCCESS) {
        for (auto iter = pimpl->advHandleSettingDatas_.begin(); iter != pimpl->advHandleSettingDatas_.end(); iter++) {
            iter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
            LOG_ERROR("Stop ex advertising failed! %{public}d", status);
        }
        return;
    }
    RemoveAllAdvHandle(status);
}

void BleAdvertiserImpl::GapExAdvResolvingListStartCompleteEvt(int status) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());
    pimpl->advHandleQue_.pop();
    if (status != BT_SUCCESS) {
        UpdateAllAdvertisingStatus(ADVERTISE_FAILED_INTERNAL_ERROR);
        LOG_ERROR("Stop or start resolving list ex advertising failed! %{public}d", status);
        return;
    }

    for (auto iter = pimpl->advHandleSettingDatas_.begin(); iter != pimpl->advHandleSettingDatas_.end(); ++iter) {
        if (iter->second.timer_ != nullptr) {
            int interval = BLE_CHANGE_RPA_ADDRESS_INTERVAL;
            iter->second.timer_->Start(interval, true);
        }
    }
}

void BleAdvertiserImpl::GapExAdvResolvingListStopCompleteEvt(int status) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());
    pimpl->advHandleQue_.pop();
    if (status != BT_SUCCESS) {
        UpdateAllAdvertisingStatus(ADVERTISE_FAILED_INTERNAL_ERROR);
        LOG_ERROR("Stop or start resolving list ex advertising failed! %{public}d", status);
        for (auto iter = pimpl->advHandleSettingDatas_.begin(); iter != pimpl->advHandleSettingDatas_.end(); ++iter) {
            if (iter->second.timer_ != nullptr) {
                int interval = BLE_CHANGE_RPA_ADDRESS_INTERVAL;
                iter->second.timer_->Start(interval, true);
            }
        }
    }
}

void BleAdvertiserImpl::GapExAdvStopCompleteEvt(int status) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());
    if (pimpl->advHandleQue_.empty()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s advHandleQue_ empty .", __func__);
        return;
    }
    uint8_t tempHandle = pimpl->advHandleQue_.front().advHandle;
    pimpl->advHandleQue_.pop();
    auto exAdvStopIter = pimpl->advHandleSettingDatas_.find(tempHandle);
    if (exAdvStopIter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, tempHandle);
        return;
    }

    if (status != BT_SUCCESS) {
        exAdvStopIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Stop ex advertising failed! %{public}d", __func__, status);
        return;
    }
    RemoveAdvHandle(tempHandle);
}

void BleAdvertiserImpl::GapExAdvRemoveHandleResultEvt(int status) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());
    if (pimpl->advHandleQue_.empty()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s advHandleQue_ empty .", __func__);
        return;
    }
    uint8_t tempHandle = pimpl->advHandleQue_.front().advHandle;
    pimpl->advHandleQue_.pop();
    auto exAdvRemoveIter = pimpl->advHandleSettingDatas_.find(tempHandle);
    if (exAdvRemoveIter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, tempHandle);
        return;
    }

    if (status != BT_SUCCESS) {
        exAdvRemoveIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Remove handle failed! %{public}d", __func__, status);
        return;
    }
    RemoveAdvHandle(tempHandle);
}

void BleAdvertiserImpl::GapExAdvTerminatedAdvSetEvt(int status, uint8_t handle) const
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s,%{public}zu", __func__, pimpl->advHandleQue_.size());

    auto exAdvTermIter = pimpl->advHandleSettingDatas_.find(handle);
    if (exAdvTermIter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:invalid handle! %u.", __func__, handle);
        return;
    }

    if (exAdvTermIter->second.advStatus_ == ADVERTISE_NOT_STARTED) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:user stop adv handle! %u.", __func__, handle);
        return;
    }
    if (exAdvTermIter->second.timer_ != nullptr) {
        exAdvTermIter->second.timer_->Stop();
        int inerval = BLE_CHANGE_RPA_ADDRESS_INTERVAL;
        exAdvTermIter->second.timer_->Start(inerval, true);
    }
    exAdvTermIter->second.advStatus_ = ADVERTISE_FAILED_ALREADY_STARTED;

    /// Start adv
    int ret = SetExAdvEnableToGap(handle, true);
    if (ret != BT_SUCCESS) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:Start advertising failed!.", __func__);
        exAdvTermIter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
        RemoveAdvHandle(handle);
    } else {
        pimpl->advHandleQue_.push(BleAdvertiserImplOp(handle, ADV_STATUS_STARTING));
    }
}

void BleAdvertiserImpl::HandleGapExAdvEvent(const BLE_GAP_CB_EVENT &event, int status, int8_t txPower, uint8_t handle)
{
    HILOGI("event no: %{public}d.", static_cast<int>(event));
    std::lock_guard<std::recursive_mutex> lk(pimpl->mutex_);
    switch (event) {
        case BLE_GAP_EX_ADV_SET_RAND_ADDR_RESULT_EVT:
            GapExAdvSetRandAddrResultEvt(status);
            break;
        case BLE_GAP_EX_ADV_PARAM_SET_COMPLETE_EVT:
            GapExAdvParamSetCompleteEvt(status, txPower);
            break;
        case BLE_GAP_EX_ADV_DATA_SET_COMPLETE_EVT:
            GapExAdvDataSetCompleteEvt(status, txPower);
            break;
        case BLE_GAP_EX_ADV_SCAN_RSP_DATA_SET_COMPLETE_EVT:
            GapExAdvScanRspDataSetCompleteEvt(status);
            break;
        case BLE_GAP_EX_ADV_START_COMPLETE_EVT:
            GapExAdvStartCompleteEvt(status);
            break;
        case BLE_GAP_EX_RESOLVING_LIST_ADV_START_COMPLETE_EVT:
            GapExAdvResolvingListStartCompleteEvt(status);
            break;
        case BLE_GAP_EX_ADV_STOP_COMPLETE_EVT:
            GapExAdvStopCompleteEvt(status);
            break;
        case BLE_GAP_EX_ALL_ADV_STOP_COMPLETE_EVT:
            GapExAdvStopAllCompleteEvt(status);
            break;
        case BLE_GAP_EX_RESOLVING_LIST_ADV_STOP_COMPLETE_EVT:
            GapExAdvResolvingListStopCompleteEvt(status);
            break;
        case BLE_GAP_EX_ADV_REMOVE_HANDLE_RESULT_EVT:
            GapExAdvRemoveHandleResultEvt(status);
            break;
        case BLE_GAP_EX_ADV_CLEAR_HANDLE_RESULT_EVT:
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:clrear event! %{public}d.", __func__, event);
            break;
        case BLE_GAP_EX_ADV_SCAN_REQUEST_RECEIVED_EVT:
            LOG_ERROR(
                "[BleAdvertiserImpl] %{public}s:scan request! %{public}d status %{public}d.", __func__, event, status);
            break;
        case BLE_GAP_EX_ADC_TERMINATED_ADV_SET_EVT:
            GapExAdvTerminatedAdvSetEvt(status, handle);
            break;
        default:
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:Invalid event! %{public}d.", __func__, event);
            break;
    }
}

void BleAdvertiserImpl::TimerCallback(void *context, uint8_t advHandle)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *advertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((advertiser != nullptr) && (advertiser->dispatcher_ != nullptr)) {
        /// Stop adv
        auto iter = advertiser->pimpl->advHandleSettingDatas_.find(advHandle);
        if (iter == advertiser->pimpl->advHandleSettingDatas_.end()) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:AdvHandleSettingDatas is empty!", __func__);
            return;
        }
        advertiser->dispatcher_->PostTask(std::bind(&BleAdvertiserImpl::SetAdvEnableToGap, advertiser, false));
        advertiser->pimpl->advHandleQue_.push(BleAdvertiserImplOp(advHandle, ADV_STATUS_STOPING));

        std::unique_lock<std::mutex> legacyLock(advertiser->pimpl->rpamutex_);
        int ret = GAPIF_LeGenResPriAddr(&BleAdvertiserImpl::GenResPriAddrResult, advertiser);
        if (ret != BT_SUCCESS) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:GAP_LeGenResPriAddrAsync failed!", __func__);
        }
        if (advertiser->pimpl->cvfull_.wait_for(legacyLock, std::chrono::seconds(BLE_THREAD_WAIT_TIMEOUT)) ==
            std::cv_status::timeout) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:GAP_LeGenResPriAddrAsync timeout!", __func__);
        }
        /// Start adv
        advertiser->dispatcher_->PostTask(std::bind(&BleAdvertiserImpl::SetAdvEnableToGap, advertiser, true));
        advertiser->pimpl->advHandleQue_.push(BleAdvertiserImplOp(advHandle, ADV_STATUS_STARTING));
    }
}

void BleAdvertiserImpl::TimerCallbackEx(void *context, uint8_t advHandle)
{
    LOG_DEBUG("[BleAdvertiserImpl] %{public}s", __func__);

    auto *advertiser = static_cast<BleAdvertiserImpl *>(context);
    if ((advertiser != nullptr) && (advertiser->dispatcher_ != nullptr)) {
        /// Stop adv
        auto iter = advertiser->pimpl->advHandleSettingDatas_.find(advHandle);
        if (iter == advertiser->pimpl->advHandleSettingDatas_.end()) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:AdvHandleSettingDatas is empty!", __func__);
            return;
        }
        advertiser->dispatcher_->PostTask(
            std::bind(&BleAdvertiserImpl::SetExAdvEnableToGap, advertiser, advHandle, false));
        advertiser->pimpl->advHandleQue_.push(BleAdvertiserImplOp(advHandle, ADV_STATUS_STOPING));

        std::unique_lock<std::mutex> exAdvLock(advertiser->pimpl->rpamutex_);
        int ret = GAPIF_LeGenResPriAddr(&BleAdvertiserImpl::GenResPriAddrResult, advertiser);
        if (ret != BT_SUCCESS) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:GAP_LeGenResPriAddrAsync failed!", __func__);
        }
        if (advertiser->pimpl->cvfull_.wait_for(exAdvLock, std::chrono::seconds(BLE_THREAD_WAIT_TIMEOUT)) ==
            std::cv_status::timeout) {
            LOG_ERROR("[BleAdvertiserImpl] %{public}s:GAP_LeGenResPriAddrAsync timeout!", __func__);
        }
        /// Start adv in GenResPriAddrResult->GenResPriAddrResultTask
    }
}

void BleAdvertiserImpl::GenResPriAddrResultTask(uint8_t result, BtAddr btAddr) const
{
    HILOGI("ResPriAddr = %{public}s", GetEncryptAddr(RawAddress::ConvertToString(btAddr.addr).GetAddress()).c_str());

    if (pimpl->advHandleQue_.empty()) {
        return;
    }
    uint8_t advStartHandle = pimpl->advHandleQue_.front().advHandle;
    auto iter = pimpl->advHandleSettingDatas_.find(advStartHandle);
    if (iter == pimpl->advHandleSettingDatas_.end()) {
        LOG_ERROR("[BleAdvertiserImpl] %{public}s:AdvHandleSettingDatas is empty!", __func__);
    } else {
        if (BleFeature::GetInstance().IsLeExtendedAdvertisingSupported()) {
            pimpl->advHandleQue_.pop();
            int ret = GAPIF_LeExAdvSetRandAddr(advStartHandle, &btAddr.addr[0]);
            if (ret != BT_SUCCESS) {
                LOG_ERROR("Set ex adv rand addr gap failed!");
                iter->second.advStatus_ = ADVERTISE_FAILED_INTERNAL_ERROR;
                callback_->OnStartResultEvent(
                    ADVERTISE_FAILED_INTERNAL_ERROR, advStartHandle, BLE_ADV_START_FAILED_OP_CODE);
                RemoveAdvHandle(advStartHandle);
            } else {
                pimpl->advHandleQue_.push(BleAdvertiserImplOp(advStartHandle, ADV_STATUS_SET_RAN_ADDR));
            }
        } else {
            int ret = BTM_SetLeRandomAddress(&btAddr);
            if (ret != BT_SUCCESS) {
                LOG_ERROR("[BleAdapter] %{public}s:GenResPriAddrResult failed!", __func__);
            }
            pimpl->cvfull_.notify_all();
        }
    }
}

void BleAdvertiserImpl::GenResPriAddrResult(uint8_t result, const uint8_t addr[BT_ADDRESS_SIZE], void *context)
{
    HILOGI("ResPriAddr = %{public}s", GetEncryptAddr(RawAddress::ConvertToString(addr).GetAddress()).c_str());

    auto *bleAdvertiser = static_cast<BleAdvertiserImpl *>(context);

    BtAddr btAddr;
    (void)memcpy_s(&btAddr.addr, BT_ADDRESS_SIZE, addr, BT_ADDRESS_SIZE);
    btAddr.type = BLE_ADDR_TYPE_RANDOM;

    if ((bleAdvertiser != nullptr) && (bleAdvertiser->dispatcher_ != nullptr)) {
        bleAdvertiser->dispatcher_->PostTask(
            std::bind(&BleAdvertiserImpl::GenResPriAddrResultTask, bleAdvertiser, result, btAddr));
    }
}

int BleAdvertiserImpl::GetMaxAdvertisingDataLength(const BleAdvertiserSettingsImpl &settings)
{
    if (settings.IsLegacyMode()) {
        return BLE_LEGACY_ADV_DATA_LEN_MAX;
    } else {
        return BleFeature::GetInstance().GetBleMaximumAdvertisingDataLength();
    }
}
}  // namespace bluetooth
}  // namespace OHOS
