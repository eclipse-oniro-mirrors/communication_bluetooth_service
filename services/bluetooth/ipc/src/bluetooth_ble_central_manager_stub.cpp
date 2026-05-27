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

#include <map>
#include <vector>

#include "bluetooth_ble_central_manager_stub.h"
#include "bluetooth_log.h"
#include "ipc_types.h"
#include "parcel_bt_uuid.h"
#include "bluetooth_bt_uuid.h"
#include "bluetooth_errorcode.h"
#include "permission_manager.h"

#ifdef STUB_FUNC
#undef STUB_FUNC
#endif
#define STUB_FUNC(code, func, perm) \
BluetoothBleCentralManagerInterfaceCode::code, {&BluetoothBleCentralManagerStub::func, perm} \

namespace OHOS {
namespace Bluetooth {
using namespace OHOS::bluetooth;
const int32_t BLE_CENTRAL_MANAGER_STUB_READ_DATA_SIZE_MAX_LEN = 0x100;
// Note: Permissions need to be configured when the itf to be used. "nullptr" means no permission needed.
const BluetoothBleCentralManagerStub::BleCentralManagerStubFuncMap BluetoothBleCentralManagerStub::memberFuncMap_ = {
    {STUB_FUNC(BLE_REGISTER_BLE_CENTRAL_MANAGER_CALLBACK, RegisterBleCentralManagerCallbackInner, nullptr)},
    {STUB_FUNC(BLE_DE_REGISTER_BLE_CENTRAL_MANAGER_CALLBACK, DeregisterBleCentralManagerCallbackInner, nullptr)},
    {STUB_FUNC(BLE_START_SCAN, StartScanInner, nullptr)},
    {STUB_FUNC(BLE_REMOVE_SCAN_FILTER, RemoveScanFilterInner,
        CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BLE_STOP_SCAN, StopScanInner, CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BLE_FREEZE_BY_RSS, FreezeByRssInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BLE_RESET_ALL_PROXY, ResetAllProxyInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BLE_SET_LPDEVICE_ADV_PARAM, SetLpDeviceAdvParamInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BLE_SET_SCAN_REPORT_CHANNEL_TO_LPDEVICE, SetScanReportChannelToLpDeviceInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BLE_ENABLE_SYNC_DATA_TO_LPDEVICE, EnableSyncDataToLpDeviceInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BLE_DISABLE_SYNC_DATA_TO_LPDEVICE, DisableSyncDataToLpDeviceInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BLE_SEND_PARAMS_TO_LPDEVICE, SendParamsToLpDeviceInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BLE_IS_LPDEVICE_AVAILABLE, IsLpDeviceAvailableInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BLE_SET_LPDEVICE_PARAM, SetLpDeviceParamInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BLE_REMOVE_LPDEVICE_PARAM, RemoveLpDeviceParamInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BLE_CHANGE_SCAN_PARAM, ChangeScanParamsInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BLE_IS_VALID_SCANNERID, IsValidScannerIdInner, nullptr)},
    {STUB_FUNC(BLE_FLUSH_BATCH_SCAN_RESULTS, FlushBatchScanResultsInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
};

BluetoothBleCentralManagerStub::BluetoothBleCentralManagerStub()
{}

BluetoothBleCentralManagerStub::~BluetoothBleCentralManagerStub()
{}

int BluetoothBleCentralManagerStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    CHECK_PERMISSION_AND_EXECUTE_FUNC_RETURN(BluetoothBleCentralManagerStub);
}

int32_t BluetoothBleCentralManagerStub::RegisterBleCentralManagerCallbackInner(
    MessageParcel &data, MessageParcel &reply)
{
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothBleCentralManagerCallback> callBack =
        OHOS::iface_cast<IBluetoothBleCentralManagerCallback>(remote);
    bool enableRandomAddrMode = data.ReadBool();
    int32_t scannerId = 0;
    RegisterBleCentralManagerCallback(scannerId, enableRandomAddrMode, callBack);
    if (!reply.WriteInt32(scannerId)) {
        HILOGE("reply writing failed");
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::DeregisterBleCentralManagerCallbackInner(
    MessageParcel &data, MessageParcel &reply)
{
    int32_t scannerId = data.ReadInt32();
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothBleCentralManagerCallback> callBack =
        OHOS::iface_cast<IBluetoothBleCentralManagerCallback>(remote);
    DeregisterBleCentralManagerCallback(scannerId, callBack);
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::StartScanInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t scannerId = data.ReadInt32();
    std::shared_ptr<BluetoothBleScanSettings> settings(data.ReadParcelable<BluetoothBleScanSettings>());
    if (settings == nullptr) {
        HILOGW("[StartScanInner] fail: read settings failed");
        return TRANSACTION_ERR;
    }

    std::vector<BluetoothBleScanFilter> filters {};
    int32_t itemsSize = 0;
    if (!data.ReadInt32(itemsSize) || itemsSize > BLE_CENTRAL_MANAGER_STUB_READ_DATA_SIZE_MAX_LEN) {
        HILOGE("read Parcelable size failed.");
        return ERR_INVALID_VALUE;
    }
    for (int i = 0; i < itemsSize; i++) {
        std::shared_ptr<BluetoothBleScanFilter> res(data.ReadParcelable<BluetoothBleScanFilter>());
        if (res == nullptr) {
            HILOGE("null pointer");
            return ERR_INVALID_VALUE;
        }
        BluetoothBleScanFilter item = *(res);
        filters.push_back(item);
    }

    bool isNewApi = data.ReadBool();
    int ret = StartScan(scannerId, *settings, filters, isNewApi);
    if (!reply.WriteInt32(ret)) {
        HILOGE("reply writing failed");
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::StopScanInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t scannerId = data.ReadInt32();
    int ret = StopScan(scannerId);
    if (!reply.WriteInt32(ret)) {
        HILOGE("reply writing failed");
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::RemoveScanFilterInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t scannerId = data.ReadInt32();

    RemoveScanFilter(scannerId);
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::FreezeByRssInner(MessageParcel &data, MessageParcel &reply)
{
    std::vector<int> pidVec {};
    CHECK_AND_RETURN_LOG_RET(data.ReadInt32Vector(&pidVec), ERR_INVALID_VALUE, "ipc failed");
    bool isProxy = data.ReadBool();
    uint8_t freezeType = data.ReadUint8();

    std::set<int> pidSet(pidVec.begin(), pidVec.end());
    bool ret = FreezeByRss(pidSet, isProxy, freezeType);
    if (!reply.WriteBool(ret)) {
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::ResetAllProxyInner(MessageParcel &data, MessageParcel &reply)
{
    bool ret = ResetAllProxy();
    if (!reply.WriteBool(ret)) {
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::SetLpDeviceAdvParamInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::SetScanReportChannelToLpDeviceInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::EnableSyncDataToLpDeviceInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::DisableSyncDataToLpDeviceInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::SendParamsToLpDeviceInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::IsLpDeviceAvailableInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::SetLpDeviceParamInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::RemoveLpDeviceParamInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::ChangeScanParamsInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t scannerId = data.ReadInt32();
    std::shared_ptr<BluetoothBleScanSettings> settings(data.ReadParcelable<BluetoothBleScanSettings>());
    CHECK_AND_RETURN_LOG_RET(settings != nullptr, ERR_INVALID_VALUE, "settings nullptr");

    std::vector<BluetoothBleScanFilter> filters{};
    int32_t filterSize = 0;
    if (!data.ReadInt32(filterSize) || filterSize > BLE_CENTRAL_MANAGER_STUB_READ_DATA_SIZE_MAX_LEN) {
        HILOGE("read Parcelable size failed.");
        return ERR_INVALID_VALUE;
    }

    for (int32_t i = 0; i < filterSize; i++) {
        std::shared_ptr<BluetoothBleScanFilter> filter(data.ReadParcelable<BluetoothBleScanFilter>());
        if (filter == nullptr) {
            HILOGE("filter nullptr");
            return ERR_INVALID_VALUE;
        }
        BluetoothBleScanFilter item = *filter;
        filters.push_back(item);
    }
    uint32_t filterAction = 0;
    if (!data.ReadUint32(filterAction)) {
        HILOGE("read Parcelable filterAction failed.");
        return ERR_INVALID_VALUE;
    }
    int32_t ret = ChangeScanParams(scannerId, *settings, filters, filterAction);
    if (!reply.WriteInt32(ret)) {
        HILOGE("reply writing failed.");
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::IsValidScannerIdInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t scannerId = data.ReadInt32();
    bool isValid = true;
    IsValidScannerId(scannerId, isValid);
    if (!reply.WriteBool(isValid)) {
        HILOGE("reply writing failed");
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothBleCentralManagerStub::FlushBatchScanResultsInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("Not Support");
    if (!reply.WriteInt32(BT_ERR_API_NOT_SUPPORT)) {
        HILOGE("BluetoothBleCentralManagerStub: WriteFrameInner reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}
}  // namespace Bluetooth
}  // namespace OHOS
