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

namespace OHOS {
namespace Bluetooth {
const int32_t BLE_CENTRAL_MANAGER_STUB_READ_DATA_SIZE_MAX_LEN = 0x100;
const std::map<uint32_t, std::function<ErrCode(BluetoothBleCentralManagerStub *, MessageParcel &, MessageParcel &)>>
    BluetoothBleCentralManagerStub::interfaces_ = {
        {BluetoothBleCentralManagerInterfaceCode::BLE_REGISTER_BLE_CENTRAL_MANAGER_CALLBACK,
            std::bind(&BluetoothBleCentralManagerStub::RegisterBleCentralManagerCallbackInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {BluetoothBleCentralManagerInterfaceCode::BLE_DE_REGISTER_BLE_CENTRAL_MANAGER_CALLBACK,
            std::bind(&BluetoothBleCentralManagerStub::DeregisterBleCentralManagerCallbackInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {BluetoothBleCentralManagerInterfaceCode::BLE_START_SCAN,
            std::bind(&BluetoothBleCentralManagerStub::StartScanInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothBleCentralManagerInterfaceCode::BLE_REMOVE_SCAN_FILTER,
            std::bind(&BluetoothBleCentralManagerStub::RemoveScanFilterInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {BluetoothBleCentralManagerInterfaceCode::BLE_STOP_SCAN,
            std::bind(&BluetoothBleCentralManagerStub::StopScanInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothBleCentralManagerInterfaceCode::BLE_FREEZE_BY_RSS,
            std::bind(&BluetoothBleCentralManagerStub::FreezeByRssInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothBleCentralManagerInterfaceCode::BLE_RESET_ALL_PROXY,
            std::bind(&BluetoothBleCentralManagerStub::ResetAllProxyInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothBleCentralManagerInterfaceCode::BLE_SET_LPDEVICE_ADV_PARAM,
            std::bind(&BluetoothBleCentralManagerStub::SetLpDeviceAdvParamInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
        {BluetoothBleCentralManagerInterfaceCode::BLE_SET_SCAN_REPORT_CHANNEL_TO_LPDEVICE,
            std::bind(&BluetoothBleCentralManagerStub::SetScanReportChannelToLpDeviceInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
        {BluetoothBleCentralManagerInterfaceCode::BLE_ENABLE_SYNC_DATA_TO_LPDEVICE,
            std::bind(&BluetoothBleCentralManagerStub::EnableSyncDataToLpDeviceInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
        {BluetoothBleCentralManagerInterfaceCode::BLE_DISABLE_SYNC_DATA_TO_LPDEVICE,
            std::bind(&BluetoothBleCentralManagerStub::DisableSyncDataToLpDeviceInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
        {BluetoothBleCentralManagerInterfaceCode::BLE_SEND_PARAMS_TO_LPDEVICE,
            std::bind(&BluetoothBleCentralManagerStub::SendParamsToLpDeviceInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
        {BluetoothBleCentralManagerInterfaceCode::BLE_IS_LPDEVICE_AVAILABLE,
            std::bind(&BluetoothBleCentralManagerStub::IsLpDeviceAvailableInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
        {BluetoothBleCentralManagerInterfaceCode::BLE_SET_LPDEVICE_PARAM,
            std::bind(&BluetoothBleCentralManagerStub::SetLpDeviceParamInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
        {BluetoothBleCentralManagerInterfaceCode::BLE_REMOVE_LPDEVICE_PARAM,
            std::bind(&BluetoothBleCentralManagerStub::RemoveLpDeviceParamInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
        {BluetoothBleCentralManagerInterfaceCode::BLE_CHANGE_SCAN_PARAM,
            std::bind(&BluetoothBleCentralManagerStub::ChangeScanParamsInner, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3)},
};

BluetoothBleCentralManagerStub::BluetoothBleCentralManagerStub()
{}

BluetoothBleCentralManagerStub::~BluetoothBleCentralManagerStub()
{}

int BluetoothBleCentralManagerStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    if (BluetoothBleCentralManagerStub::GetDescriptor() != data.ReadInterfaceToken()) {
        HILOGW("[OnRemoteRequest] fail: invalid interface token!");
        return OBJECT_NULL;
    }

    auto it = interfaces_.find(code);
    if (it == interfaces_.end()) {
        HILOGW("[OnRemoteRequest] fail: unknown code!");
        return IRemoteStub<IBluetoothBleCentralManager>::OnRemoteRequest(code, data, reply, option);
    }

    auto fun = it->second;
    if (fun == nullptr) {
        HILOGW("[OnRemoteRequest] fail: not find function!");
        return IRemoteStub<IBluetoothBleCentralManager>::OnRemoteRequest(code, data, reply, option);
    }

    ErrCode result = fun(this, data, reply);
    if (SUCCEEDED(result)) {
        return NO_ERROR;
    }

    HILOGW("[OnRemoteRequest] fail: Failed to call interface %{public}u, err:%{public}d", code, result);
    return result;
}

ErrCode BluetoothBleCentralManagerStub::RegisterBleCentralManagerCallbackInner(
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

ErrCode BluetoothBleCentralManagerStub::DeregisterBleCentralManagerCallbackInner(
    MessageParcel &data, MessageParcel &reply)
{
    int32_t scannerId = data.ReadInt32();
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothBleCentralManagerCallback> callBack =
        OHOS::iface_cast<IBluetoothBleCentralManagerCallback>(remote);
    DeregisterBleCentralManagerCallback(scannerId, callBack);
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::StartScanInner(MessageParcel &data, MessageParcel &reply)
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

    int ret = StartScan(scannerId, *settings, filters);
    if (!reply.WriteInt32(ret)) {
        HILOGE("reply writing failed");
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::StopScanInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t scannerId = data.ReadInt32();
    int ret = StopScan(scannerId);
    if (!reply.WriteInt32(ret)) {
        HILOGE("reply writing failed");
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::RemoveScanFilterInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t scannerId = data.ReadInt32();

    RemoveScanFilter(scannerId);
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::FreezeByRssInner(MessageParcel &data, MessageParcel &reply)
{
    std::vector<int> pidVec {};
    CHECK_AND_RETURN_LOG_RET(data.ReadInt32Vector(&pidVec), ERR_INVALID_VALUE, "ipc failed");
    bool isProxy = data.ReadBool();

    std::set<int> pidSet(pidVec.begin(), pidVec.end());
    bool ret = FreezeByRss(pidSet, isProxy);
    if (!reply.WriteBool(ret)) {
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::ResetAllProxyInner(MessageParcel &data, MessageParcel &reply)
{
    bool ret = ResetAllProxy();
    if (!reply.WriteBool(ret)) {
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::SetLpDeviceAdvParamInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::SetScanReportChannelToLpDeviceInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::EnableSyncDataToLpDeviceInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::DisableSyncDataToLpDeviceInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::SendParamsToLpDeviceInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::IsLpDeviceAvailableInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::SetLpDeviceParamInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::RemoveLpDeviceParamInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothBleCentralManagerStub::ChangeScanParamsInner(MessageParcel &data, MessageParcel &reply)
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
}  // namespace Bluetooth
}  // namespace OHOS
