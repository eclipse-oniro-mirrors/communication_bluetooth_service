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

#include "bluetooth_host_stub.h"
#include "bluetooth_log.h"
#include "bluetooth_errorcode.h"
#include "ipc_types.h"
#include "raw_address.h"
#include "string_ex.h"
#include "permission_utils.h"

namespace OHOS {
namespace Bluetooth {
const std::map<uint32_t, std::function<ErrCode(BluetoothHostStub *, MessageParcel &, MessageParcel &)>>
    BluetoothHostStub::memberFuncMap_ = {
        {BluetoothHostInterfaceCode::BT_REGISTER_OBSERVER,
            std::bind(&BluetoothHostStub::RegisterObserverInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_DEREGISTER_OBSERVER,
            std::bind(&BluetoothHostStub::DeregisterObserverInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_ENABLE,
            std::bind(&BluetoothHostStub::EnableBtInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_DISABLE,
            std::bind(&BluetoothHostStub::DisableBtInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_GETPROFILE,
            std::bind(&BluetoothHostStub::GetProfileInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_GET_BLE,
            std::bind(&BluetoothHostStub::GetBleRemoteInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_FACTORY_RESET,
            std::bind(&BluetoothHostStub::BluetoothFactoryResetInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_GETSTATE,
            std::bind(&BluetoothHostStub::GetBtStateInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_GET_LOCAL_ADDRESS,
            std::bind(&BluetoothHostStub::GetLocalAddressInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_DISABLE_BLE,
            std::bind(&BluetoothHostStub::DisableBleInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_ENABLE_BLE,
            std::bind(&BluetoothHostStub::EnableBleInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_GET_PROFILE_LIST,
            std::bind(&BluetoothHostStub::GetProfileListInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_GET_MAXNUM_CONNECTED_AUDIODEVICES,
            std::bind(&BluetoothHostStub::GetMaxNumConnectedAudioDevicesInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_GET_BT_STATE,
            std::bind(&BluetoothHostStub::GetBtConnectionStateInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_GET_BT_PROFILE_CONNSTATE,
            std::bind(&BluetoothHostStub::GetBtProfileConnStateInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_GET_LOCAL_DEVICE_CLASS,
            std::bind(&BluetoothHostStub::GetLocalDeviceClassInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_SET_LOCAL_DEVICE_CLASS,
            std::bind(&BluetoothHostStub::SetLocalDeviceClassInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_GET_LOCAL_NAME,
            std::bind(&BluetoothHostStub::GetLocalNameInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_SET_LOCAL_NAME,
            std::bind(&BluetoothHostStub::SetLocalNameInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_GET_BT_SCAN_MODE,
            std::bind(&BluetoothHostStub::GetBtScanModeInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_SET_BT_SCAN_MODE,
            std::bind(&BluetoothHostStub::SetBtScanModeInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_GET_BONDABLE_MODE,
            std::bind(&BluetoothHostStub::GetBondableModeInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_SET_BONDABLE_MODE,
            std::bind(&BluetoothHostStub::SetBondableModeInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_START_BT_DISCOVERY,
            std::bind(&BluetoothHostStub::StartBtDiscoveryInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_CANCEL_BT_DISCOVERY,
            std::bind(&BluetoothHostStub::CancelBtDiscoveryInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_IS_BT_DISCOVERING,
            std::bind(&BluetoothHostStub::IsBtDiscoveringInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_GET_BT_DISCOVERY_END_MILLIS,
            std::bind(&BluetoothHostStub::GetBtDiscoveryEndMillisInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_GET_PAIRED_DEVICES,
            std::bind(&BluetoothHostStub::GetPairedDevicesInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_REMOVE_PAIR,
            std::bind(&BluetoothHostStub::RemovePairInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_REMOVE_ALL_PAIRS,
            std::bind(&BluetoothHostStub::RemoveAllPairsInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_REGISTER_REMOTE_DEVICE_OBSERVER,
            std::bind(&BluetoothHostStub::RegisterRemoteDeviceObserverInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_DEREGISTER_REMOTE_DEVICE_OBSERVER,
            std::bind(&BluetoothHostStub::DeregisterRemoteDeviceObserverInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_GET_BLE_MAX_ADVERTISING_DATALENGTH,
            std::bind(&BluetoothHostStub::GetBleMaxAdvertisingDataLengthInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::GET_DEVICE_TYPE,
            std::bind(&BluetoothHostStub::GetDeviceTypeInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::GET_PHONEBOOK_PERMISSION,
            std::bind(&BluetoothHostStub::GetPhonebookPermissionInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::SET_PHONEBOOK_PERMISSION,
            nullptr},
        {BluetoothHostInterfaceCode::GET_MESSAGE_PERMISSION,
            std::bind(&BluetoothHostStub::GetMessagePermissionInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::SET_MESSAGE_PERMISSION,
            nullptr},
        {BluetoothHostInterfaceCode::GET_POWER_MODE,
            std::bind(&BluetoothHostStub::GetPowerModeInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::GET_DEVICE_NAME,
            std::bind(&BluetoothHostStub::GetDeviceNameInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::GET_DEVICE_ALIAS,
            std::bind(&BluetoothHostStub::GetDeviceAliasInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::SET_DEVICE_ALIAS,
            std::bind(&BluetoothHostStub::SetDeviceAliasInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::GET_DEVICE_BATTERY_INFO,
            std::bind(&BluetoothHostStub::GetRemoteDeviceBatteryInfoInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::GET_PAIR_STATE,
            std::bind(&BluetoothHostStub::GetPairStateInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::START_PAIR,
            std::bind(&BluetoothHostStub::StartPairInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::CANCEL_PAIRING,
            std::bind(&BluetoothHostStub::CancelPairingInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::IS_BONDED_FROM_LOCAL,
            std::bind(&BluetoothHostStub::IsBondedFromLocalInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::IS_ACL_CONNECTED,
            std::bind(&BluetoothHostStub::IsAclConnectedInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::IS_ACL_ENCRYPTED,
            std::bind(&BluetoothHostStub::IsAclEncryptedInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::GET_DEVICE_CLASS,
            std::bind(&BluetoothHostStub::GetDeviceClassInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::SET_DEVICE_PIN,
            std::bind(&BluetoothHostStub::SetDevicePinInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::SET_DEVICE_PAIRING_CONFIRMATION,
            std::bind(&BluetoothHostStub::SetDevicePairingConfirmationInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::SET_DEVICE_PASSKEY,
            std::bind(&BluetoothHostStub::SetDevicePasskeyInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::PAIR_REQUEST_PEPLY,
            std::bind(&BluetoothHostStub::PairRequestReplyInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::READ_REMOTE_RSSI_VALUE,
            std::bind(&BluetoothHostStub::ReadRemoteRssiValueInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::GET_LOCAL_SUPPORTED_UUIDS,
            std::bind(&BluetoothHostStub::GetLocalSupportedUuidsInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::GET_DEVICE_UUIDS,
            std::bind(&BluetoothHostStub::GetDeviceUuidsInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::GET_LOCAL_PROFILE_UUIDS,
            std::bind(&BluetoothHostStub::GetLocalProfileUuidsInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_REGISTER_BLE_ADAPTER_OBSERVER,
            std::bind(&BluetoothHostStub::RegisterBleAdapterObserverInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_DEREGISTER_BLE_ADAPTER_OBSERVER,
            std::bind(&BluetoothHostStub::DeregisterBleAdapterObserverInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_REGISTER_BLE_PERIPHERAL_OBSERVER,
            std::bind(&BluetoothHostStub::RegisterBlePeripheralCallbackInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_DEREGISTER_BLE_PERIPHERAL_OBSERVER,
            std::bind(&BluetoothHostStub::DeregisterBlePeripheralCallbackInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_SET_FAST_SCAN,
            std::bind(&BluetoothHostStub::SetFastScanInner, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::GET_RANDOM_ADDRESS, std::bind(&BluetoothHostStub::GetRandomAddressInner,
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::SYNC_RANDOM_ADDRESS, std::bind(&BluetoothHostStub::SyncRandomAddressInner,
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::START_CREDIBLE_PAIR,
            std::bind(&BluetoothHostStub::StartCrediblePairInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::CONNECT_ALLOWED_PROFILES,
            std::bind(&BluetoothHostStub::ConnectAllowedProfilesInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::DISCONNECT_ALLOWED_PROFILES,
            std::bind(&BluetoothHostStub::DisconnectAllowedProfilesInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::SET_CUSTOM_TYPE,
            std::bind(&BluetoothHostStub::SetDeviceCustomTypeInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::SATELLITE_CONTROL,
            std::bind(&BluetoothHostStub::SatelliteControlInner, std::placeholders::_1, std::placeholders::_2,
                std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_REGISTER_RESOURCE_MANAGER_OBSERVER,
            std::bind(&BluetoothHostStub::RegisterBtResourceManagerObserverInner,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_DEREGISTER_RESOURCE_MANAGER_OBSERVER,
            std::bind(&BluetoothHostStub::DeregisterBtResourceManagerObserverInner,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::UPDATE_VIRTUAL_DEVICE,
            std::bind(&BluetoothHostStub::UpdateVirtualDeviceInner,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::GET_VIRTUAL_AUTO_CONN_SWITCH,
            std::bind(&BluetoothHostStub::IsSupportVirtualAutoConnectInner,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::SET_VIRTUAL_AUTO_CONN_TYPE,
            std::bind(&BluetoothHostStub::SetVirtualAutoConnectTypeInner,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::SET_FAST_SCAN_LEVEL,
            std::bind(&BluetoothHostStub::SetFastScanLevelInner,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::CTRL_DEVICE_ACTION,
            std::bind(&BluetoothHostStub::ControlDeviceActionInner,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::GET_CONNECTION_TIME,
            std::bind(&BluetoothHostStub::GetLastConnectionTimeInner,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::BT_UPDATE_CLOUD_DEVICE,
            std::bind(&BluetoothHostStub::UpdateCloudBluetoothDevInner,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {BluetoothHostInterfaceCode::GET_CLOUD_BOND_STATE,
            std::bind(&BluetoothHostStub::GetCloudBondStateInner,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
};

BluetoothHostStub::BluetoothHostStub(){};
BluetoothHostStub::~BluetoothHostStub()
{}
int32_t BluetoothHostStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    HILOGI("BluetoothHostStub::OnRemoteRequest, cmd = %{public}u, flags= %{public}d", code, option.GetFlags());
    if (BluetoothHostStub::GetDescriptor() != data.ReadInterfaceToken()) {
        HILOGE("BluetoothHostStub::OnRemoteRequest, local descriptor is not equal to remote");
        return ERR_INVALID_STATE;
    }
    auto itFunc = memberFuncMap_.find(code);
    if (itFunc != memberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            return memberFunc(this, data, reply);
        }
    }
    HILOGW("BluetoothHostStub::OnRemoteRequest, default case, need check.");
    return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
}

ErrCode BluetoothHostStub::RegisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("BluetoothHostStub::RegisterObserverInner starts");
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothHostObserver> observer = OHOS::iface_cast<IBluetoothHostObserver>(remote);
    RegisterObserver(observer);
    return NO_ERROR;
}

ErrCode BluetoothHostStub::DeregisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("BluetoothHostStub::DeregisterObserverInner starts");
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothHostObserver> observer = OHOS::iface_cast<IBluetoothHostObserver>(remote);
    DeregisterObserver(observer);
    return NO_ERROR;
}

int32_t BluetoothHostStub::EnableBtInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = EnableBt();
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::DisableBtInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = DisableBt();
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetProfileInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("BluetoothHostStub::GetProfileInner starts");
    std::string name = data.ReadString();
    sptr<IRemoteObject> result = GetProfile(name);
    bool ret = reply.WriteRemoteObject(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetBleRemoteInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("BluetoothHostStub::GetBleRemoteInner starts");
    std::string name = data.ReadString();
    sptr<IRemoteObject> result = GetBleRemote(name);
    bool ret = reply.WriteRemoteObject(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::BluetoothFactoryResetInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = BluetoothFactoryReset();
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("WriteInt32 failed");
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetBtStateInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("BluetoothHostStub::GetBtStateInner starts");
    int32_t state = 0;
    int32_t result = GetBtState(state);
    (void)reply.WriteInt32(result);
    (void)reply.WriteInt32(state);
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetLocalAddressInner(MessageParcel &data, MessageParcel &reply)
{
    std::string addr = "00:00:00:00:00:00";
    int32_t result = GetLocalAddress(addr);

    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(result), BT_ERR_IPC_TRANS_FAILED, "writing res failed");
    CHECK_AND_RETURN_LOG_RET((result == BT_NO_ERROR), NO_ERROR, "get local addr failed");
    CHECK_AND_RETURN_LOG_RET(reply.WriteString(addr), BT_ERR_IPC_TRANS_FAILED, "writing addr failed");
    return NO_ERROR;
}

int32_t BluetoothHostStub::DisableBleInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = DisableBle();
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::EnableBleInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = EnableBle();
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetProfileListInner(MessageParcel &data, MessageParcel &reply)
{
    std::vector<uint32_t> result = GetProfileList();
    bool ret = reply.WriteUInt32Vector(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetMaxNumConnectedAudioDevicesInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = GetMaxNumConnectedAudioDevices();
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetBtConnectionStateInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("BluetoothHostStub::GetBtConnectionStateInner starts");
    int32_t state = 0;
    int32_t result = GetBtConnectionState(state);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    ret = reply.WriteInt32(state);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetBtProfileConnStateInner(MessageParcel &data, MessageParcel &reply)
{
    uint32_t profileId;
    data.ReadUint32(profileId);
    int32_t state = 0;
    int32_t result = GetBtProfileConnState(profileId, state);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    ret = reply.WriteInt32(state);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetLocalDeviceClassInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = GetLocalDeviceClass();
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::SetLocalDeviceClassInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t deviceClass;
    data.ReadInt32(deviceClass);
    bool result = SetLocalDeviceClass(deviceClass);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetLocalNameInner(MessageParcel &data, MessageParcel &reply)
{
    std::string name = "";
    int32_t result = GetLocalName(name);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    ret = reply.WriteString(name);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::SetLocalNameInner(MessageParcel &data, MessageParcel &reply)
{
    std::string name;
    data.ReadString(name);
    int32_t result = SetLocalName(name);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }

    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetDeviceTypeInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t transport;
    if (!data.ReadInt32(transport)) {
        HILOGE("BluetoothHostStub::GetDeviceType transport failed");
        return TRANSACTION_ERR;
    }
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::GetDeviceType address failed");
        return TRANSACTION_ERR;
    }
    int result = GetDeviceType(transport, address);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetBtScanModeInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t scanMode = 0;
    int32_t result = GetBtScanMode(scanMode);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    ret = reply.WriteInt32(scanMode);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetPhonebookPermissionInner(MessageParcel &data, MessageParcel &reply)
{
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::GetPhonebookPermission address failed");
        return TRANSACTION_ERR;
    }
    int result = GetPhonebookPermission(address);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::SetBtScanModeInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t mode;
    data.ReadInt32(mode);
    int32_t duration;
    data.ReadInt32(duration);
    int32_t result = SetBtScanMode(mode, duration);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::SetPhonebookPermissionInner(MessageParcel &data, MessageParcel &reply)
{
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::SetPhonebookPermission address failed");
        return TRANSACTION_ERR;
    }
    int32_t permission;
    if (!data.ReadInt32(permission)) {
        HILOGE("BluetoothHostStub::SetPhonebookPermission permission failed");
        return TRANSACTION_ERR;
    }
    bool result = SetPhonebookPermission(address, permission);
    bool ret = reply.WriteBool(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetBondableModeInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t transport;
    data.ReadInt32(transport);
    int32_t result = GetBondableMode(transport);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}
ErrCode BluetoothHostStub::GetMessagePermissionInner(MessageParcel &data, MessageParcel &reply)
{
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::GetMessagePermission address failed");
        return TRANSACTION_ERR;
    }
    int result = GetMessagePermission(address);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::SetBondableModeInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t transport;
    data.ReadInt32(transport);
    int32_t mode;
    data.ReadInt32(mode);

    bool result = SetBondableMode(transport, mode);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::SetMessagePermissionInner(MessageParcel &data, MessageParcel &reply)
{
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::SetMessagePermission address failed");
        return TRANSACTION_ERR;
    }
    int32_t permission;
    if (!data.ReadInt32(permission)) {
        HILOGE("BluetoothHostStub::SetMessagePermission permission failed");
        return TRANSACTION_ERR;
    }
    bool result = SetMessagePermission(address, permission);
    bool ret = reply.WriteBool(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetPowerModeInner(MessageParcel &data, MessageParcel &reply)
{
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::GetPowerMode address failed");
        return TRANSACTION_ERR;
    }
    int result = GetPowerMode(address);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::StartBtDiscoveryInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = StartBtDiscovery();
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::CancelBtDiscoveryInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = CancelBtDiscovery();
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetDeviceNameInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t transport;
    if (!data.ReadInt32(transport)) {
        HILOGE("BluetoothHostStub::GetDeviceName transport failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::GetDeviceName address failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    std::string name = "";
    int32_t result = GetDeviceName(transport, address, name);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    ret = reply.WriteString(name);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetDeviceAliasInner(MessageParcel &data, MessageParcel &reply)
{
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::GetDeviceAlias address failed");
        return TRANSACTION_ERR;
    }
    std::string result = GetDeviceAlias(address);
    bool ret = reply.WriteString(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::IsBtDiscoveringInner(MessageParcel &data, MessageParcel &reply)
{
    bool isDiscovering = false;
    int32_t transport;
    data.ReadInt32(transport);
    int32_t result = IsBtDiscovering(isDiscovering, transport);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    ret = reply.WriteBool(isDiscovering);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply isDiscovering writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return BT_NO_ERROR;
}

ErrCode BluetoothHostStub::SetDeviceAliasInner(MessageParcel &data, MessageParcel &reply)
{
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::SetDeviceAlias address failed");
        return TRANSACTION_ERR;
    }
    std::string aliasName;
    if (!data.ReadString(aliasName)) {
        HILOGE("BluetoothHostStub::SetDeviceAlias aliasName failed");
        return TRANSACTION_ERR;
    }
    int32_t res = SetDeviceAlias(address, aliasName);
    bool ret = reply.WriteInt32(res);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetRemoteDeviceBatteryInfoInner(MessageParcel &data, MessageParcel &reply)
{
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::GetRemoteDeviceBattery address failed");
        return TRANSACTION_ERR;
    }
    BluetoothBatteryInfo info;
    int ret = GetRemoteDeviceBatteryInfo(address, info);
    CHECK_AND_RETURN_LOG_RET(reply.WriteInt32(ret), BT_ERR_INTERNAL_ERROR, "write ret failed");
    CHECK_AND_RETURN_LOG_RET(reply.WriteParcelable(&info), BT_ERR_INTERNAL_ERROR,
        "write battery failed");
    return BT_NO_ERROR;
}

ErrCode BluetoothHostStub::GetBtDiscoveryEndMillisInner(MessageParcel &data, MessageParcel &reply)
{
    int64_t result = GetBtDiscoveryEndMillis();
    bool ret = reply.WriteInt64(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetPairStateInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t transport;
    if (!data.ReadInt32(transport)) {
        HILOGE("BluetoothHostStub::GetPairState transport failed");
        return TRANSACTION_ERR;
    }
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::GetPairState address failed");
        return TRANSACTION_ERR;
    }
    int32_t pairState;
    int result = GetPairState(transport, address, pairState);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    ret = reply.WriteInt32(pairState);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return BT_NO_ERROR;
}

int32_t BluetoothHostStub::GetPairedDevicesInner(MessageParcel &data, MessageParcel &reply)
{
    std::vector<BluetoothRawAddress> pairDevice;
    int32_t result = GetPairedDevices(pairDevice);
    bool ret = true;
    if (!reply.WriteInt32(pairDevice.size())) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    } else {
        for (auto device : pairDevice) {
            reply.WriteParcelable(&device);
        }
    }
    ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::StartPairInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t transport;
    if (!data.ReadInt32(transport)) {
        HILOGE("BluetoothHostStub::StartPair transport failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::StartPair address failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    int32_t result = StartPair(transport, address);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::RemovePairInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t transport = data.ReadInt32();
    sptr<BluetoothRawAddress> device = data.ReadStrongParcelable<BluetoothRawAddress>();
    int32_t result = RemovePair(transport, device);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::CancelPairingInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t transport;
    if (!data.ReadInt32(transport)) {
        HILOGE("BluetoothHostStub::CancelPairing transport failed");
        return TRANSACTION_ERR;
    }
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::CancelPairing address failed");
        return TRANSACTION_ERR;
    }
    bool result = CancelPairing(transport, address);
    bool ret = reply.WriteBool(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::RemoveAllPairsInner(MessageParcel &data, MessageParcel &reply)
{
    bool result = RemoveAllPairs();
    bool ret = reply.WriteBool(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }

    return NO_ERROR;
}

ErrCode BluetoothHostStub::IsBondedFromLocalInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t transport;
    if (!data.ReadInt32(transport)) {
        HILOGE("BluetoothHostStub::IsBondedFromLocal transport failed");
        return TRANSACTION_ERR;
    }
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::IsBondedFromLocal address failed");
        return TRANSACTION_ERR;
    }
    bool result = IsBondedFromLocal(transport, address);
    bool ret = reply.WriteBool(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::SetDevicePinInner(MessageParcel &data, MessageParcel &reply)
{
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::SetDevicePin address failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    std::string pin;
    if (!data.ReadString(pin)) {
        HILOGE("BluetoothHostStub::SetDevicePin pin failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    int32_t result = SetDevicePin(address, pin);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::IsAclConnectedInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t transport;
    if (!data.ReadInt32(transport)) {
        HILOGE("BluetoothHostStub::IsAclConnected transport failed");
        return TRANSACTION_ERR;
    }
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::IsAclConnected address failed");
        return TRANSACTION_ERR;
    }
    bool result = IsAclConnected(transport, address);
    bool ret = reply.WriteBool(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::RegisterRemoteDeviceObserverInner(MessageParcel &data, MessageParcel &reply)
{
    auto tempObject = data.ReadRemoteObject();
    sptr<IBluetoothRemoteDeviceObserver> observer;
    observer = iface_cast<IBluetoothRemoteDeviceObserver>(tempObject);
    RegisterRemoteDeviceObserver(observer);
    return NO_ERROR;
}

ErrCode BluetoothHostStub::DeregisterRemoteDeviceObserverInner(MessageParcel &data, MessageParcel &reply)
{
    auto tempObject = data.ReadRemoteObject();
    sptr<IBluetoothRemoteDeviceObserver> observer;
    observer = iface_cast<IBluetoothRemoteDeviceObserver>(tempObject);
    DeregisterRemoteDeviceObserver(observer);
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetBleMaxAdvertisingDataLengthInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = GetBleMaxAdvertisingDataLength();
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::IsAclEncryptedInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t transport;
    if (!data.ReadInt32(transport)) {
        HILOGE("BluetoothHostStub::IsAclEncrypted transport failed");
        return TRANSACTION_ERR;
    }
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::IsAclEncrypted address failed");
        return TRANSACTION_ERR;
    }
    bool result = IsAclEncrypted(transport, address);
    bool ret = reply.WriteBool(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetDeviceClassInner(MessageParcel &data, MessageParcel &reply)
{
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::GetDeviceClass address failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    int32_t cod = 0;
    int result = GetDeviceClass(address, cod);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    ret = reply.WriteInt32(cod);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::SetDevicePairingConfirmationInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t transport;
    if (!data.ReadInt32(transport)) {
        HILOGE("BluetoothHostStub::SetDevicePairingConfirmation transport failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::SetDevicePairingConfirmation address failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    bool accept = false;
    if (!data.ReadBool(accept)) {
        HILOGE("BluetoothHostStub::SetDevicePairingConfirmation accept failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    int32_t result = SetDevicePairingConfirmation(transport, address, accept);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::SetDevicePasskeyInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t transport;
    if (!data.ReadInt32(transport)) {
        HILOGE("BluetoothHostStub::SetDevicePasskey transport failed");
        return TRANSACTION_ERR;
    }
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::SetDevicePasskey address failed");
        return TRANSACTION_ERR;
    }
    int32_t passkey;
    if (!data.ReadInt32(passkey)) {
        HILOGE("BluetoothHostStub::SetDevicePasskey passkey failed");
        return TRANSACTION_ERR;
    }
    bool accept = false;
    if (!data.ReadBool(accept)) {
        HILOGE("BluetoothHostStub::SetDevicePasskey accept failed");
        return TRANSACTION_ERR;
    }
    bool result = SetDevicePasskey(transport, address, passkey, accept);
    bool ret = reply.WriteBool(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::PairRequestReplyInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t transport;
    if (!data.ReadInt32(transport)) {
        HILOGE("BluetoothHostStub::PairRequestReply transport failed");
        return TRANSACTION_ERR;
    }
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::PairRequestReply address failed");
        return TRANSACTION_ERR;
    }
    bool accept = false;
    if (!data.ReadBool(accept)) {
        HILOGE("BluetoothHostStub::PairRequestReply accept failed");
        return TRANSACTION_ERR;
    }
    bool result = PairRequestReply(transport, address, accept);
    bool ret = reply.WriteBool(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::ReadRemoteRssiValueInner(MessageParcel &data, MessageParcel &reply)
{
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::ReadRemoteRssiValue address failed");
        return TRANSACTION_ERR;
    }
    bool result = ReadRemoteRssiValue(address);
    bool ret = reply.WriteBool(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetLocalSupportedUuidsInner(MessageParcel &data, MessageParcel &reply)
{
    std::vector<std::string> uuids;
    GetLocalSupportedUuids(uuids);
    int32_t size = uuids.size();
    bool ret = reply.WriteInt32(size);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    for (auto uuid : uuids) {
        if (!reply.WriteString(uuid)) {
            HILOGE("BluetoothHostStub: GetLocalSupportedUuidsInner write uuid error");
            return TRANSACTION_ERR;
        }
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetDeviceUuidsInner(MessageParcel &data, MessageParcel &reply)
{
    std::string address;
    std::vector<std::string> uuids;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostProxy::GetDeviceUuids Read address error");
        return TRANSACTION_ERR;
    }
    int res = GetDeviceUuids(address, uuids);
    int32_t size = uuids.size();
    bool ret = reply.WriteInt32(size);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    for (auto uuid : uuids) {
        if (!reply.WriteString(uuid)) {
            HILOGE("BluetoothHostStub: GetDeviceUuidsInner write uuid error");
            return TRANSACTION_ERR;
        }
    }
    if (!reply.WriteInt32(res)) {
        HILOGE("BluetoothHostStub: GetDeviceUuidsInner write result error");
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetLocalProfileUuidsInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothHostStub::RegisterBleAdapterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    auto tempObject = data.ReadRemoteObject();
    sptr<IBluetoothHostObserver> observer;
    observer = iface_cast<IBluetoothHostObserver>(tempObject);
    RegisterBleAdapterObserver(observer);
    return NO_ERROR;
}

ErrCode BluetoothHostStub::DeregisterBleAdapterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    auto tempObject = data.ReadRemoteObject();
    sptr<IBluetoothHostObserver> observer;
    observer = iface_cast<IBluetoothHostObserver>(tempObject);
    DeregisterBleAdapterObserver(observer);
    return NO_ERROR;
}

ErrCode BluetoothHostStub::RegisterBlePeripheralCallbackInner(MessageParcel &data, MessageParcel &reply)
{
    auto tempObject = data.ReadRemoteObject();
    sptr<IBluetoothBlePeripheralObserver> observer;
    observer = iface_cast<IBluetoothBlePeripheralObserver>(tempObject);
    RegisterBlePeripheralCallback(observer);
    return NO_ERROR;
}

ErrCode BluetoothHostStub::DeregisterBlePeripheralCallbackInner(MessageParcel &data, MessageParcel &reply)
{
    auto tempObject = data.ReadRemoteObject();
    sptr<IBluetoothBlePeripheralObserver> observer;
    observer = iface_cast<IBluetoothBlePeripheralObserver>(tempObject);
    DeregisterBlePeripheralCallback(observer);
    return NO_ERROR;
}

int32_t BluetoothHostStub::SetFastScanInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothHostStub::GetRandomAddressInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothHostStub::SyncRandomAddressInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

ErrCode BluetoothHostStub::StartCrediblePairInner(MessageParcel &data, MessageParcel &reply)
{
    std::string address = data.ReadString();
    int32_t transport = data.ReadInt32();
    int result = StartCrediblePair(transport, address);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
    }
    return NO_ERROR;
}

ErrCode BluetoothHostStub::SatelliteControlInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothHostStub::ConnectAllowedProfilesInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHostStub::DisconnectAllowedProfilesInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHostStub::SetDeviceCustomTypeInner(MessageParcel &data, MessageParcel &reply)
{
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::SetDeviceCustomType address failed");
        return TRANSACTION_ERR;
    }
    int32_t customType;
    if (!data.ReadInt32(customType)) {
        HILOGE("BluetoothHostStub::SetDeviceCustomType customType failed");
        return TRANSACTION_ERR;
    }
    bool ret = reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHostStub::RegisterBtResourceManagerObserverInner(MessageParcel &data, MessageParcel &reply)
{
    auto tempObject = data.ReadRemoteObject();
    sptr<IBluetoothResourceManagerObserver> observer;
    observer = iface_cast<IBluetoothResourceManagerObserver>(tempObject);
    CHECK_AND_RETURN_LOG_RET(observer != nullptr, ERR_INVALID_VALUE, "observer is nullptr");
    RegisterBtResourceManagerObserver(observer);
    return NO_ERROR;
}

int32_t BluetoothHostStub::DeregisterBtResourceManagerObserverInner(MessageParcel &data, MessageParcel &reply)
{
    auto tempObject = data.ReadRemoteObject();
    sptr<IBluetoothResourceManagerObserver> observer;
    observer = iface_cast<IBluetoothResourceManagerObserver>(tempObject);
    CHECK_AND_RETURN_LOG_RET(observer != nullptr, ERR_INVALID_VALUE, "observer is nullptr");
    DeregisterBtResourceManagerObserver(observer);
    return NO_ERROR;
}

int32_t BluetoothHostStub::UpdateVirtualDeviceInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHostStub::IsSupportVirtualAutoConnectInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHostStub::SetVirtualAutoConnectTypeInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHostStub::SetFastScanLevelInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHostStub::EnableBluetoothToRestrictModeInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHostStub::ControlDeviceActionInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHostStub::GetLastConnectionTimeInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHostStub::UpdateCloudBluetoothDevInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHostStub::GetCloudBondStateInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}
}  // namespace Bluetooth
}  // namespace OHOS
