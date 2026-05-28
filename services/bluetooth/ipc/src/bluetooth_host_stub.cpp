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
#include "permission_manager.h"

#ifdef STUB_FUNC
#undef STUB_FUNC
#endif
#define STUB_FUNC(code, func, perm) BluetoothHostInterfaceCode::code, {&BluetoothHostStub::func, perm}

namespace OHOS {
namespace Bluetooth {
using namespace OHOS::bluetooth;
// Note: Permissions need to be configured when the itf to be used. "nullptr" means no permission needed.
const std::map<uint32_t, BluetoothHostStub::BluetoothHostStubFuncPerm> BluetoothHostStub::memberFuncMap_ = {
    {STUB_FUNC(BT_REGISTER_OBSERVER, RegisterObserverInner, nullptr)},
    {STUB_FUNC(BT_DEREGISTER_OBSERVER, DeregisterObserverInner, nullptr)},
    {STUB_FUNC(BT_ENABLE, EnableBtInner, CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_DISABLE, DisableBtInner, CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_GETPROFILE, GetProfileInner, nullptr)},
    {STUB_FUNC(BT_GET_BLE, GetBleRemoteInner, nullptr)},
    {STUB_FUNC(BT_FACTORY_RESET, BluetoothFactoryResetInner,
        CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_GETSTATE, GetBtStateInner, nullptr)},
    {STUB_FUNC(BT_GET_LOCAL_ADDRESS, GetLocalAddressInner,
        CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, GET_BLUETOOTH_LOCAL_MAC)))},
    {STUB_FUNC(BT_GENERATE_LOCAL_OOB_DATA, GenerateLocalOobDataInner, CHECK_PERM(true, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_DISABLE_BLE, DisableBleInner, CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_ENABLE_BLE, EnableBleInner, CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_GET_PROFILE_LIST, GetProfileListInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_GET_MAXNUM_CONNECTED_AUDIODEVICES, GetMaxNumConnectedAudioDevicesInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_GET_BT_STATE, GetBtConnectionStateInner, CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_GET_BT_PROFILE_CONNSTATE, GetBtProfileConnStateInner,
        CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_GET_LOCAL_DEVICE_CLASS, GetLocalDeviceClassInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_SET_LOCAL_DEVICE_CLASS, SetLocalDeviceClassInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_GET_LOCAL_NAME, GetLocalNameInner, CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_SET_LOCAL_NAME, SetLocalNameInner, CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_GET_BT_SCAN_MODE, GetBtScanModeInner, CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_SET_BT_SCAN_MODE, SetBtScanModeInner, CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_GET_BONDABLE_MODE, GetBondableModeInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_SET_BONDABLE_MODE, SetBondableModeInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_START_BT_DISCOVERY, StartBtDiscoveryInner, nullptr)},
    {STUB_FUNC(BT_CANCEL_BT_DISCOVERY, CancelBtDiscoveryInner,
        CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_IS_BT_DISCOVERING, IsBtDiscoveringInner, CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_GET_BT_DISCOVERY_END_MILLIS, GetBtDiscoveryEndMillisInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_GET_PAIRED_DEVICES, GetPairedDevicesInner, CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_REMOVE_PAIR, RemovePairInner, CHECK_PERM(true, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_REMOVE_ALL_PAIRS, RemoveAllPairsInner,
        CHECK_PERM(true, {DISCOVER_BLUETOOTH}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(CANCEL_PAIRING, CancelPairingInner, CHECK_PERM(true, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_REGISTER_REMOTE_DEVICE_OBSERVER, RegisterRemoteDeviceObserverInner, nullptr)},
    {STUB_FUNC(BT_DEREGISTER_REMOTE_DEVICE_OBSERVER, DeregisterRemoteDeviceObserverInner, nullptr)},
    {STUB_FUNC(BT_GET_BLE_MAX_ADVERTISING_DATALENGTH, GetBleMaxAdvertisingDataLengthInner, nullptr)},
    {STUB_FUNC(BT_GET_CONNECTED_BLE_DEVICES, GetConnectedBLEDevicesInner,
        CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(GET_DEVICE_TYPE, GetDeviceTypeInner, nullptr)},
    {STUB_FUNC(GET_PHONEBOOK_PERMISSION, GetPhonebookPermissionInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(SET_PHONEBOOK_PERMISSION, SetPhonebookPermissionInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(GET_MESSAGE_PERMISSION, GetMessagePermissionInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(SET_MESSAGE_PERMISSION, SetMessagePermissionInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(GET_POWER_MODE, GetPowerModeInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(GET_DEVICE_NAME, GetDeviceNameInner, CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(GET_DEVICE_ALIAS, GetDeviceAliasInner,
        CHECK_PERM(true, {USE_BLUETOOTH}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(SET_DEVICE_ALIAS, SetDeviceAliasInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(GET_DEVICE_BATTERY_INFO, GetRemoteDeviceBatteryInfoInner, CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(SET_DEVICE_BATTERY_INFO, SetRemoteDeviceBatteryInfoInner, CHECK_PERM(true, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(GET_PAIR_STATE, GetPairStateInner, CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(START_PAIR, StartPairInner, CHECK_PERM(false, {DISCOVER_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(START_CREDIBLE_PAIR, StartCrediblePairInner,
        CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(IS_BONDED_FROM_LOCAL, IsBondedFromLocalInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(IS_ACL_CONNECTED, IsAclConnectedInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(IS_ACL_ENCRYPTED, IsAclEncryptedInner, nullptr)},
    {STUB_FUNC(GET_DEVICE_CLASS, GetDeviceClassInner, nullptr)},
    {STUB_FUNC(SET_DEVICE_PIN, SetDevicePinInner, CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(SET_DEVICE_PAIRING_CONFIRMATION, SetDevicePairingConfirmationInner,
        CHECK_PERM(false, {MANAGE_BLUETOOTH}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(SET_DEVICE_PASSKEY, SetDevicePasskeyInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(PAIR_REQUEST_PEPLY, PairRequestReplyInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(READ_REMOTE_RSSI_VALUE, ReadRemoteRssiValueInner, nullptr)},
    {STUB_FUNC(GET_LOCAL_SUPPORTED_UUIDS, GetLocalSupportedUuidsInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(GET_LOCAL_PROFILE_UUIDS, GetLocalProfileUuidsInner, CHECK_PERM(true, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(GET_DEVICE_UUIDS, GetDeviceUuidsInner, CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_REGISTER_BLE_ADAPTER_OBSERVER, RegisterBleAdapterObserverInner, nullptr)},
    {STUB_FUNC(BT_DEREGISTER_BLE_ADAPTER_OBSERVER, DeregisterBleAdapterObserverInner, nullptr)},
    {STUB_FUNC(BT_REGISTER_BLE_PERIPHERAL_OBSERVER, RegisterBlePeripheralCallbackInner, nullptr)},
    {STUB_FUNC(BT_DEREGISTER_BLE_PERIPHERAL_OBSERVER, DeregisterBlePeripheralCallbackInner, nullptr)},
    {STUB_FUNC(BT_SET_FAST_SCAN, SetFastScanInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(GET_RANDOM_ADDRESS, GetRandomAddressInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(GET_REAL_ADDRESS, GetRealAddressInner,
        CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(SYNC_RANDOM_ADDRESS, SyncRandomAddressInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(CONNECT_ALLOWED_PROFILES, ConnectAllowedProfilesInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(DISCONNECT_ALLOWED_PROFILES, DisconnectAllowedProfilesInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(SET_CUSTOM_TYPE, SetDeviceCustomTypeInner, CHECK_PERM(true, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(SATELLITE_CONTROL, SatelliteControlInner,
        CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_REGISTER_RESOURCE_MANAGER_OBSERVER, RegisterBtResourceManagerObserverInner,
        CHECK_PERM(true, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_DEREGISTER_RESOURCE_MANAGER_OBSERVER, DeregisterBtResourceManagerObserverInner,
        CHECK_PERM(true, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(GET_VIRTUAL_AUTO_CONN_SWITCH, IsSupportVirtualAutoConnectInner,
        CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(SET_VIRTUAL_AUTO_CONN_TYPE, SetVirtualAutoConnectTypeInner,
        CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(SET_FAST_SCAN_LEVEL, SetFastScanLevelInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(UPDATE_VIRTUAL_DEVICE, UpdateVirtualDeviceInner,
        CHECK_PERM(true, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(CTRL_DEVICE_ACTION, ControlDeviceActionInner,
        CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(GET_CONNECTION_TIME, GetLastConnectionTimeInner, nullptr)},
    {STUB_FUNC(BT_UPDATE_CLOUD_DEVICE, UpdateCloudBluetoothDevInner,
        CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(GET_CLOUD_BOND_STATE, GetCloudBondStateInner,
        CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(BT_UPDATE_REFUSE_POLICY, UpdateRefusePolicyInner,
        CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(PROCESS_RANDOM_DEVICE_ID_COMMAND, ProcessRandomDeviceIdCommandInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(GET_DEVICE_TRANSPORT, GetDeviceTransportInner, nullptr)},
    {STUB_FUNC(BT_GET_CAR_KEY_DFX_DATA, GetCarKeyDfxDataInner, nullptr)},
    {STUB_FUNC(BT_SET_CAR_KEY_CARD_DATA, SetCarKeyCardDataInner, nullptr)},
    {STUB_FUNC(IS_PROFILE_EXIST, IsProfileExistInner, nullptr)},
    {STUB_FUNC(BT_NOTIFY_DIALOG_RESULT, NotifyDialogResultInner,
        CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(START_REMOTE_SDP_SEARCH, StartRemoteSdpSearchInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(GET_REMOTE_SERVICES, GetRemoteServicesInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(GET_VIRTUAL_ADDRESS_BY_HASH, GetVirtualAddressByHashInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(SET_CONNECTION_PRIORITY, SetConnectionPriorityInner,
        CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(IS_BAS_SUPPORTED, IsBasSupportedInner, nullptr)},
    {STUB_FUNC(BT_REGISTER_DEVICE_BATTERY_OBSERVER, RegisterDeviceBatteryObserverInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_DEREGISTER_DEVICE_BATTERY_OBSERVER, DeregisterDeviceBatteryObserverInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(BT_GET_BATTERY_LEVEL, GetBatteryLevelInner, CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(GET_CONNECTED_DEVICE_BATTERY_INFOS, GetConnectedDeviceBatteryInfosInner,
        CHECK_PERM(false, {}, {ACCESS_BLUETOOTH}))},
};

BluetoothHostStub::BluetoothHostStub(){};
BluetoothHostStub::~BluetoothHostStub(){};

int32_t BluetoothHostStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    CHECK_PERMISSION_AND_EXECUTE_FUNC_RETURN(BluetoothHostStub);
}

int32_t BluetoothHostStub::RegisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGI("BluetoothHostStub::RegisterObserverInner starts");
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothHostObserver> observer = OHOS::iface_cast<IBluetoothHostObserver>(remote);
    RegisterObserver(observer);
    return NO_ERROR;
}

int32_t BluetoothHostStub::DeregisterObserverInner(MessageParcel &data, MessageParcel &reply)
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
    bool isAsync = false;
    if (!data.ReadBool(isAsync)) {
        HILOGE("BluetoothHostStub::DisableBt isAsync failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    std::string callingName = data.ReadString();
    int32_t result = DisableBt(isAsync, callingName);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetProfileInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::GetBleRemoteInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::GetBtStateInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::GenerateLocalOobDataInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGE("API not supported.");
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
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
    bool noAutoConnect = false;
    if (!data.ReadBool(noAutoConnect)) {
        HILOGE("BluetoothHostStub::EnableBle noAutoConnect failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    bool isAsync = false;
    if (!data.ReadBool(isAsync)) {
        HILOGE("BluetoothHostStub::EnableBle isAsync failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    std::string callingName = data.ReadString();
    int32_t result = EnableBle(noAutoConnect, isAsync, callingName);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetProfileListInner(MessageParcel &data, MessageParcel &reply)
{
    std::vector<uint32_t> result = GetProfileList();
    bool ret = reply.WriteUInt32Vector(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetMaxNumConnectedAudioDevicesInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::GetLocalDeviceClassInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = GetLocalDeviceClass();
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::SetLocalDeviceClassInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::GetDeviceTypeInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::GetPhonebookPermissionInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::SetPhonebookPermissionInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::GetBondableModeInner(MessageParcel &data, MessageParcel &reply)
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
int32_t BluetoothHostStub::GetMessagePermissionInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::SetBondableModeInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::SetMessagePermissionInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::GetPowerModeInner(MessageParcel &data, MessageParcel &reply)
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
    bool alias = true;
    if (!data.ReadBool(alias)) {
        HILOGE("BluetoothHostStub::GetDeviceName alias failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    std::string name = "";
    int32_t result = GetDeviceName(transport, address, name, alias);
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

int32_t BluetoothHostStub::GetDeviceAliasInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::IsBtDiscoveringInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::SetDeviceAliasInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::SetRemoteDeviceBatteryInfoInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGE("API not supported.");
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetBtDiscoveryEndMillisInner(MessageParcel &data, MessageParcel &reply)
{
    int64_t result = GetBtDiscoveryEndMillis();
    bool ret = reply.WriteInt64(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetPairStateInner(MessageParcel &data, MessageParcel &reply)
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
    if (!reply.WriteInt32(result)) {
        HILOGE("BluetoothHostStub: reply writing result failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    if (result != BT_NO_ERROR) {
        return BT_ERR_IPC_TRANS_FAILED;
    }
    if (!reply.WriteInt32(pairDevice.size())) {
        HILOGE("BluetoothHostStub: reply writing size failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    } else {
        for (auto device : pairDevice) {
            reply.WriteParcelable(&device);
        }
    }
    return BT_NO_ERROR;
}

int32_t BluetoothHostStub::StartPairInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t transport;
    if (!data.ReadInt32(transport)) {
        HILOGE("BluetoothHostStub::StartPair transport failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    sptr<BluetoothRawAddress> device(data.ReadStrongParcelable<BluetoothRawAddress>());
    if (!device) {
        HILOGE("BluetoothHostStub::StartPair device failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    sptr<BluetoothOobData> oobData(data.ReadStrongParcelable<BluetoothOobData>());
    if (!oobData) {
        HILOGE("BluetoothHostStub::StartPair oobData failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    int32_t result = StartPair(transport, *device, *oobData);
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

int32_t BluetoothHostStub::CancelPairingInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::RemoveAllPairsInner(MessageParcel &data, MessageParcel &reply)
{
    bool result = RemoveAllPairs();
    bool ret = reply.WriteBool(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }

    return NO_ERROR;
}

int32_t BluetoothHostStub::IsBondedFromLocalInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::IsAclConnectedInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::RegisterRemoteDeviceObserverInner(MessageParcel &data, MessageParcel &reply)
{
    auto tempObject = data.ReadRemoteObject();
    sptr<IBluetoothRemoteDeviceObserver> observer;
    observer = iface_cast<IBluetoothRemoteDeviceObserver>(tempObject);
    RegisterRemoteDeviceObserver(observer);
    return NO_ERROR;
}

int32_t BluetoothHostStub::DeregisterRemoteDeviceObserverInner(MessageParcel &data, MessageParcel &reply)
{
    auto tempObject = data.ReadRemoteObject();
    sptr<IBluetoothRemoteDeviceObserver> observer;
    observer = iface_cast<IBluetoothRemoteDeviceObserver>(tempObject);
    DeregisterRemoteDeviceObserver(observer);
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetBleMaxAdvertisingDataLengthInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = GetBleMaxAdvertisingDataLength();
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetConnectedBLEDevicesInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    return NO_ERROR;
}

int32_t BluetoothHostStub::IsAclEncryptedInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::SetDevicePasskeyInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::PairRequestReplyInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::ReadRemoteRssiValueInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::GetLocalSupportedUuidsInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::GetDeviceUuidsInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::GetLocalProfileUuidsInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothHostStub::RegisterBleAdapterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    auto tempObject = data.ReadRemoteObject();
    sptr<IBluetoothHostObserver> observer;
    observer = iface_cast<IBluetoothHostObserver>(tempObject);
    RegisterBleAdapterObserver(observer);
    return NO_ERROR;
}

int32_t BluetoothHostStub::DeregisterBleAdapterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    auto tempObject = data.ReadRemoteObject();
    sptr<IBluetoothHostObserver> observer;
    observer = iface_cast<IBluetoothHostObserver>(tempObject);
    DeregisterBleAdapterObserver(observer);
    return NO_ERROR;
}

int32_t BluetoothHostStub::RegisterBlePeripheralCallbackInner(MessageParcel &data, MessageParcel &reply)
{
    auto tempObject = data.ReadRemoteObject();
    sptr<IBluetoothBlePeripheralObserver> observer;
    observer = iface_cast<IBluetoothBlePeripheralObserver>(tempObject);
    RegisterBlePeripheralCallback(observer);
    return NO_ERROR;
}

int32_t BluetoothHostStub::DeregisterBlePeripheralCallbackInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::GetRandomAddressInner(MessageParcel &data, MessageParcel &reply)
{
    std::string realAddress = data.ReadString();
    std::string randomAddress;
    int result = GetRandomAddress(realAddress, randomAddress);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return TRANSACTION_ERR;
    }
    if (result != NO_ERROR) {
        if (!reply.WriteString(randomAddress)) {
            HILOGE("BluetoothHostStub:GetRandomAddressInner write randomAddress error");
            return TRANSACTION_ERR;
        }
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetRealAddressInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    return NO_ERROR;
}

int32_t BluetoothHostStub::SyncRandomAddressInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothHostStub::StartCrediblePairInner(MessageParcel &data, MessageParcel &reply)
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

int32_t BluetoothHostStub::SatelliteControlInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothHostStub::ConnectAllowedProfilesInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    return NO_ERROR;
}

int32_t BluetoothHostStub::DisconnectAllowedProfilesInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    return NO_ERROR;
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

int32_t BluetoothHostStub::IsProfileExistInner(MessageParcel &data, MessageParcel &reply)
{
    std::string profileName;
    CHECK_AND_RETURN_LOG_RET(data.ReadString(profileName), BT_ERR_INTERNAL_ERROR, "Read profileName failed");
    bool isProfileExist = false;
    int res = IsProfileExist(profileName, isProfileExist);
    if (!reply.WriteInt32(res)) {
        HILOGE("reply writing failed.");
        return BT_ERR_INTERNAL_ERROR;
    }
    if (res != BT_NO_ERROR) {
        return res;
    }
    if (!reply.WriteBool(isProfileExist)) {
        HILOGE("reply writing failed.");
        return BT_ERR_INTERNAL_ERROR;
    }
    return BT_NO_ERROR;
}

int32_t BluetoothHostStub::GetLastConnectionTimeInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothHostStub::UpdateCloudBluetoothDevInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetCloudBondStateInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetDeviceTransportInner(MessageParcel &data, MessageParcel &reply)
{
    std::string address;
    if (!data.ReadString(address)) {
        HILOGE("BluetoothHostStub::GetDeviceTransport address failed");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    int32_t transport = bluetooth::TRANSPORT_UNKNOWN;
    int32_t result = GetDeviceTransport(address, transport);
    bool ret = reply.WriteInt32(result);
    if (!ret) {
        HILOGE("BluetoothHostStub: reply writing failed in: %{public}s.", __func__);
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHostStub::UpdateRefusePolicyInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHostStub::ProcessRandomDeviceIdCommandInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetCarKeyDfxDataInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHostStub::SetCarKeyCardDataInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHostStub::NotifyDialogResultInner(MessageParcel &data, MessageParcel &reply)
{
    return BT_ERR_API_NOT_SUPPORT;
}

int32_t BluetoothHostStub::StartRemoteSdpSearchInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetRemoteServicesInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetVirtualAddressByHashInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    return NO_ERROR;
}

int32_t BluetoothHostStub::SetConnectionPriorityInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    return NO_ERROR;
}

int32_t BluetoothHostStub::IsBasSupportedInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    return NO_ERROR;
}

int32_t BluetoothHostStub::RegisterDeviceBatteryObserverInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    return NO_ERROR;
}

int32_t BluetoothHostStub::DeregisterDeviceBatteryObserverInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetBatteryLevelInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    return NO_ERROR;
}

int32_t BluetoothHostStub::GetConnectedDeviceBatterInfosInner(MessageParcel &data, MessageParcel &reply)
{
    reply.WriteInt32(BT_ERR_API_NOT_SUPPORT);
    return NO_ERROR;
}
}  // namespace Bluetooth
}  // namespace OHOS
