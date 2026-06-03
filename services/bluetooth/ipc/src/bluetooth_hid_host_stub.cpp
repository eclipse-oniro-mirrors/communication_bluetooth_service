/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
#include "bluetooth_hid_host_stub.h"

#include "bluetooth_errorcode.h"
#include "bluetooth_log.h"
#include "permission_manager.h"

#ifdef STUB_FUNC
#undef STUB_FUNC
#endif
#define STUB_FUNC(code, func, perm) \
static_cast<uint32_t>(BluetoothHidHostInterfaceCode::code), {&BluetoothHidHostStub::func, perm} \

namespace OHOS {
namespace Bluetooth {
using namespace OHOS::bluetooth;
const uint32_t HID_DEVICE_BY_STATES_NUM_MAX = 0XFF;
// Note: Permissions need to be configured when the itf to be used. "nullptr" means no permission needed.
const std::map<uint32_t, BluetoothHidHostStub::HidHostStubFuncPerm> BluetoothHidHostStub::memberFuncMap_ = {
    {STUB_FUNC(COMMAND_CONNECT, ConnectInner,
    CHECK_PERM(true, {DISCOVER_BLUETOOTH}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(COMMAND_DISCONNECT, DisconnectInner,
    CHECK_PERM(true, {DISCOVER_BLUETOOTH}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(COMMAND_GET_DEVICE_STATE, GetDeviceStateInner,
    CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_GET_DEVICES_BY_STATES, GetDevicesByStatesInner,
    CHECK_PERM(false, {USE_BLUETOOTH}, {ACCESS_BLUETOOTH}))},
    {STUB_FUNC(COMMAND_REGISTER_OBSERVER, RegisterObserverInner, nullptr)},
    {STUB_FUNC(COMMAND_DEREGISTER_OBSERVER, DeregisterObserverInner, nullptr)},
    {STUB_FUNC(COMMAND_VCUN_PLUG, HidHostVCUnplugInner,
    CHECK_PERM(false, {DISCOVER_BLUETOOTH}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(COMMAND_SEND_DATA, HidHostSendDataInner,
    CHECK_PERM(false, {DISCOVER_BLUETOOTH}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(COMMAND_SET_REPORT, HidHostSetReportInner,
    CHECK_PERM(false, {DISCOVER_BLUETOOTH}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(COMMAND_GET_REPORT, HidHostGetReportInner,
    CHECK_PERM(false, {DISCOVER_BLUETOOTH}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(COMMAND_SET_CONNECT_STRATEGY, HidHostSetConnectStrategyInner,
    CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {STUB_FUNC(COMMAND_GET_CONNECT_STRATEGY, HidHostGetConnectStrategyInner,
    CHECK_PERM(true, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
};

BluetoothHidHostStub::BluetoothHidHostStub()
{}

BluetoothHidHostStub::~BluetoothHidHostStub()
{}

int BluetoothHidHostStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    CHECK_PERMISSION_AND_EXECUTE_FUNC_RETURN(BluetoothHidHostStub);
}

int32_t BluetoothHidHostStub::ConnectInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return BT_ERR_IPC_TRANS_FAILED;
    }
    HILOGD("BluetoothHidHostStub::ConnectInner");
    int32_t errCode = Connect(*device);
    // write error code
    if (!reply.WriteInt32(errCode)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHidHostStub::DisconnectInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return BT_ERR_IPC_TRANS_FAILED;
    }
    HILOGD("BluetoothHidHostStub::DisconnectInner");
    int32_t errCode = Disconnect(*device);
    // write error code
    if (!reply.WriteInt32(errCode)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHidHostStub::GetDeviceStateInner(MessageParcel &data, MessageParcel &reply)
{
    std::shared_ptr<BluetoothRawAddress> device(data.ReadParcelable<BluetoothRawAddress>());
    if (!device) {
        return BT_ERR_IPC_TRANS_FAILED;
    }
    HILOGD("BluetoothHidHostStub::GetDeviceStateInner");
    int32_t state;
    int32_t errCode = GetDeviceState(*device, state);
    // write error code
    if (!reply.WriteInt32(errCode)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    if (errCode != NO_ERROR) {
        HILOGE("internal error.");
        return BT_ERR_INTERNAL_ERROR;
    }
    // write state
    if (!reply.WriteInt32(state)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    return NO_ERROR;
}

int32_t BluetoothHidHostStub::GetDevicesByStatesInner(MessageParcel &data, MessageParcel &reply)
{
    std::vector<int32_t> states = {};
    int32_t stateSize = data.ReadInt32();
    if (static_cast<uint32_t>(stateSize) > HID_DEVICE_BY_STATES_NUM_MAX) {
        return ERR_INVALID_STATE;
    }
    HILOGD("BluetoothHidHostStub::GetDevicesByStatesInner");
    for (int i = 0; i < stateSize; i++) {
        int32_t state = data.ReadInt32();
        states.push_back(state);
    }
    std::vector<BluetoothRawAddress> rawAdds;
    int32_t errCode = GetDevicesByStates(states, rawAdds);
    // write error code
    if (!reply.WriteInt32(errCode)) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    if (errCode != NO_ERROR) {
        HILOGE("internal error.");
        return BT_ERR_INTERNAL_ERROR;
    }
    // write size
    if (!reply.WriteInt32(rawAdds.size())) {
        HILOGE("reply write failed.");
        return BT_ERR_IPC_TRANS_FAILED;
    }
    // write devices
    for (auto rawAdd : rawAdds) {
        if (!reply.WriteParcelable(&rawAdd)) {
            return BT_ERR_IPC_TRANS_FAILED;
        }
    }
    return NO_ERROR;
}

ErrCode BluetoothHidHostStub::RegisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGD("BluetoothHidHostStub::RegisterObserverInner");
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothHidHostObserver> observer = OHOS::iface_cast<IBluetoothHidHostObserver>(remote);
    RegisterObserver(observer);
    return NO_ERROR;
}

ErrCode BluetoothHidHostStub::DeregisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGD("BluetoothHidHostStub::DeregisterObserverInner");
    sptr<IRemoteObject> remote = data.ReadRemoteObject();
    const sptr<IBluetoothHidHostObserver> observer = OHOS::iface_cast<IBluetoothHidHostObserver>(remote);
    DeregisterObserver(observer);
    return NO_ERROR;
}

ErrCode BluetoothHidHostStub::HidHostVCUnplugInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGD("BluetoothHidHostStub::HidHostVCUnplugInner");
    std::string device = data.ReadString();
    uint8_t id = data.ReadUint8();
    uint16_t size = data.ReadUint16();
    uint8_t type = data.ReadUint8();
    int result;
    ErrCode ec = HidHostVCUnplug(device, id, size, type, result);
    if (SUCCEEDED(ec)) {
        reply.WriteInt32(result);
    }
    return NO_ERROR;
}

ErrCode BluetoothHidHostStub::HidHostSendDataInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGD("BluetoothHidHostStub::HidHostSendDataInner");
    std::string device = data.ReadString();
    uint8_t id = data.ReadUint8();
    uint16_t size = data.ReadUint16();
    uint8_t type = data.ReadUint8();
    int result;
    ErrCode ec = HidHostSendData(device, id, size, type, result);
    if (SUCCEEDED(ec)) {
        reply.WriteInt32(result);
    }
    return NO_ERROR;
}

ErrCode BluetoothHidHostStub::HidHostSetReportInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGD("BluetoothHidHostStub::HidHostSetReportInner");
    std::string device = data.ReadString();
    uint8_t type = data.ReadUint8();
    std::string report = data.ReadString();
    int result;
    ErrCode ec = HidHostSetReport(device, type, report, result);
    if (SUCCEEDED(ec)) {
        reply.WriteInt32(result);
    }
    return NO_ERROR;
}

ErrCode BluetoothHidHostStub::HidHostGetReportInner(MessageParcel &data, MessageParcel &reply)
{
    HILOGD("BluetoothHidHostStub::HidHostGetReportInner");
    std::string device = data.ReadString();
    uint8_t id = data.ReadUint8();
    uint16_t size = data.ReadUint16();
    uint8_t type = data.ReadUint8();
    int result;
    ErrCode ec = HidHostGetReport(device, id, size, type, result);
    if (SUCCEEDED(ec)) {
        reply.WriteInt32(result);
    }
    return NO_ERROR;
}

ErrCode BluetoothHidHostStub::HidHostSetConnectStrategyInner(MessageParcel &data, MessageParcel &reply)
{
    std::string addr = data.ReadString();
    int strategy = data.ReadInt32();

    int result = SetConnectStrategy(RawAddress(addr), strategy);
    if (!reply.WriteInt32(result)) {
        HILOGE("BluetoothHidHostStub: reply writing failed in: %{public}s.", __func__);
        return ERR_INVALID_VALUE;
    }
    return NO_ERROR;
}

ErrCode BluetoothHidHostStub::HidHostGetConnectStrategyInner(MessageParcel &data, MessageParcel &reply)
{
    return NO_ERROR;
}
} // Bluetooth
} // OHOS
