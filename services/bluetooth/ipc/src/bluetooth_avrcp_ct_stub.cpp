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

#include "bluetooth_avrcp_ct_stub.h"
#include "bluetooth_log.h"
#include "permission_manager.h"

#ifdef STUB_FUNC
#undef STUB_FUNC
#endif
#define STUB_FUNC(code, func, perm) std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::func), perm)

namespace OHOS {
namespace Bluetooth {
const uint32_t AVRCP_NOTIDICATION_EVENT_COUNT_MAX = 0XFF;
const uint32_t AVRCP_PLAYER_APP_SETTING_ATTRIBUTES_NUM_MAX = 0XFF;

// Note: Permissions need to be configured when the itf to be used. "nullptr" means no permission needed.
const std::map<uint32_t, BluetoothAvrcpCtStub::AvrcpCtStubFuncPerm> BluetoothAvrcpCtStub::memberFuncMap_ = {
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_REGISTER_OBSERVER),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::RegisterObserverInner), nullptr)},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_UNREGISTER_OBSERVER),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::UnregisterObserverInner), nullptr)},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_CONNECTED_DEVICES),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetConnectedDevicesInner), nullptr)},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_DEVICES_BY_STATES),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetDevicesByStatesInner), nullptr)},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_DEVICE_STATE),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetDeviceStateInner), nullptr)},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_CONNECT),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::ConnectInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_DISCONNECT),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::DisconnectInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_PRESS_BUTTON),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::PressButtonnner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_RELEASE_BUTTON),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::ReleaseButtonInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_UNIT_INFO),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetUnitInfoInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_SUB_UNIT_INFO),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetSubUnitInfoInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_SUPPORTED_COMPANIES),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetSupportedCompaniesInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_SUPPORTED_EVENTS),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetSupportedEventsInner), nullptr)},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_PLAYER_APP_SETTING_ATTRIBUTES),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetPlayerAppSettingAttributesInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_PLAYER_APP_SETTING_VALUES),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetPlayerAppSettingValuesInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_PLAYER_APP_SETTING_CURRENT_VALUE),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetPlayerAppSettingCurrentValueInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_SET_PLAYER_APP_SETTING_CURRENT_VALUE),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::SetPlayerAppSettingCurrentValueInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_PLAYER_APP_SETTING_ATTRIBUTE_TEXT),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetPlayerAppSettingAttributeTextInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_PLAYER_APP_SETTING_VALUES_TEXT),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetPlayerAppSettingValueTextInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_ELEMENT_ATTRIBUTES),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetElementAttributesInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_PLAYER_STATUS),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetPlayStatusInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_PLAY_ITEM),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::PlayItemInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_FOLDER_ITEMS),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetFolderItemsInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_TOTAL_NUMBER_OF_ITEMS),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetTotalNumberOfItemsInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_SET_ABSOLUTE_VOLUME),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::SetAbsoluteVolumeInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_ENABLE_NOTIFICATION),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::EnableNotificationInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_DISABLE_NOTIFICATION),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::DisableNotificationInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_GET_ITEM_ATTRIBUTES),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetItemAttributesInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_SET_BROWSERED_PLAYER),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::SetBrowsedPlayerInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
    {static_cast<uint32_t>(BluetoothAvrcpCtInterfaceCode::AVRCP_CT_MEDIA_PLAYER_LIST),
     std::make_pair(AvrcpCtStubFunc(&BluetoothAvrcpCtStub::GetMeidaPlayerListInner),
         CHECK_PERM(false, {}, MULTI_PERM(ACCESS_BLUETOOTH, MANAGE_BLUETOOTH)))},
};

BluetoothAvrcpCtStub::BluetoothAvrcpCtStub()
{}

BluetoothAvrcpCtStub::~BluetoothAvrcpCtStub()
{}

int BluetoothAvrcpCtStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    CHECK_PERMISSION_AND_EXECUTE_FUNC_RETURN(BluetoothAvrcpCtStub);
}

int32_t BluetoothAvrcpCtStub::RegisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    sptr<IRemoteObject>  tempObject = data.ReadRemoteObject();    
    sptr<IBluetoothAvrcpCtObserver> observer;
    observer = iface_cast<IBluetoothAvrcpCtObserver>(tempObject);
    RegisterObserver(observer);
    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::UnregisterObserverInner(MessageParcel &data, MessageParcel &reply)
{
    sptr<IRemoteObject>  tempObject = data.ReadRemoteObject();
    sptr<IBluetoothAvrcpCtObserver> observer;
    observer = iface_cast<IBluetoothAvrcpCtObserver>(tempObject);
    UnregisterObserver(observer);
    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetConnectedDevicesInner(MessageParcel &data, MessageParcel &reply)
{
    std::vector<RawAddress> rawAdds = GetConnectedDevices();
    reply.WriteInt32(rawAdds.size());
    for (auto rawAdd : rawAdds) {
        if (!reply.WriteString(rawAdd.GetAddress())) {
            HILOGE("GetConnectedDevicesInner: write WriteString failed");
            return ERR_INVALID_STATE;
        }
    }
    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetDevicesByStatesInner(MessageParcel &data, MessageParcel &reply)
{
    std::vector<int32_t> states = {};
    int32_t stateSize = data.ReadInt32();
    if (IsInvalidDeviceStatesSize(stateSize)) {
        HILOGE("BluetoothAvrcpCtStub::GetDevicesByStatesInner Invalid MessageParcel device states");
        return ERR_INVALID_VALUE;
    }
    for (int i = 0; i < stateSize; i++) {
        int32_t state = data.ReadInt32();
        states.push_back(state);
    }

    std::vector<RawAddress> rawAdds = GetDevicesByStates(states);
    reply.WriteInt32(rawAdds.size());

    for (auto rawAdd : rawAdds) {
        if (!reply.WriteString(rawAdd.GetAddress())) {
            HILOGE("GetConnectedDevicesInner: write WriteString failed");
            return ERR_INVALID_STATE;
        }
    }
    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetDeviceStateInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t state = GetDeviceState(RawAddress(data.ReadString()));
    reply.WriteInt32(state);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::ConnectInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = Connect(RawAddress(data.ReadString()));
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::DisconnectInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = Disconnect(RawAddress(data.ReadString()));
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::PressButtonnner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = PressButton(RawAddress(data.ReadString()), data.ReadInt32());
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::ReleaseButtonInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = ReleaseButton(RawAddress(data.ReadString()), data.ReadInt32());
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetUnitInfoInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = GetUnitInfo(RawAddress(data.ReadString()));
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetSubUnitInfoInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = GetSubUnitInfo(RawAddress(data.ReadString()));
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetSupportedCompaniesInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = GetSupportedCompanies(RawAddress(data.ReadString()));
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetSupportedEventsInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = GetSupportedEvents(RawAddress(data.ReadString()));
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetPlayerAppSettingAttributesInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = GetPlayerAppSettingAttributes(RawAddress(data.ReadString()));
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetPlayerAppSettingValuesInner(MessageParcel &data, MessageParcel &reply)
{
    int32_t result = GetPlayerAppSettingValues(RawAddress(data.ReadString()), data.ReadInt32());
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetPlayerAppSettingCurrentValueInner(MessageParcel &data, MessageParcel &reply)
{
    RawAddress raw = RawAddress(data.ReadString());
    std::vector<int32_t> attributes = {};
    int32_t attributesSize = data.ReadInt32();
    if (IsInvalidAttributesSize(attributesSize)) {
        HILOGE("BluetoothAvrcpCtStub::GetPlayerAppSettingCurrentValueInner Invalid MessageParcel attributes");
        return ERR_INVALID_VALUE;
    }
    for (int i = 0; i < attributesSize; i++) {
        int32_t attribute = data.ReadInt32();
        attributes.push_back(attribute);
    }
    int32_t result = GetPlayerAppSettingCurrentValue(raw, attributes);
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::SetPlayerAppSettingCurrentValueInner(MessageParcel &data, MessageParcel &reply)
{
    RawAddress raw = RawAddress(data.ReadString());
    std::vector<int32_t> attributes = {};
    int32_t attributesSize = data.ReadInt32();
    if (IsInvalidAttributesSize(attributesSize)) {
        HILOGE("BluetoothAvrcpCtStub::SetPlayerAppSettingCurrentValueInner Invalid MessageParcel attributes");
        return ERR_INVALID_VALUE;
    }
    for (int i = 0; i < attributesSize; i++) {
        int32_t attribute = data.ReadInt32();
        attributes.push_back(attribute);
    }

    std::vector<int32_t> values = {};
    int32_t valuesSize = data.ReadInt32();
    if (IsInvalidAttributesSize(valuesSize)) {
        HILOGE("BluetoothAvrcpCtStub::SetPlayerAppSettingCurrentValueInner Invalid MessageParcel values");
        return ERR_INVALID_VALUE;
    }
    for (int i = 0; i < valuesSize; i++) {
        int32_t value = data.ReadInt32();
        values.push_back(value);
    }

    int32_t result = SetPlayerAppSettingCurrentValue(raw, attributes, values);
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetPlayerAppSettingAttributeTextInner(MessageParcel &data, MessageParcel &reply)
{
    RawAddress raw = RawAddress(data.ReadString());
    std::vector<int32_t> attributes = {};
    int32_t attributesSize = data.ReadInt32();
    if (IsInvalidAttributesSize(attributesSize)) {
        HILOGE("BluetoothAvrcpCtStub::GetPlayerAppSettingAttributeTextInner Invalid MessageParcel attributes");
        return ERR_INVALID_VALUE;
    }
    for (int i = 0; i < attributesSize; i++) {
        int32_t attribute = data.ReadInt32();
        attributes.push_back(attribute);
    }
    int32_t result = GetPlayerAppSettingAttributeText(raw, attributes);
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetPlayerAppSettingValueTextInner(MessageParcel &data, MessageParcel &reply)
{
    RawAddress raw = RawAddress(data.ReadString());
    int32_t attributes = data.ReadInt32();
    std::vector<int32_t> values = {};
    int32_t valuesSize = data.ReadInt32();
    if (IsInvalidAttributesSize(valuesSize)) {
        HILOGE("BluetoothAvrcpCtStub::GetPlayerAppSettingValueTextInner Invalid MessageParcel values");
        return ERR_INVALID_VALUE;
    }
    for (int i = 0; i < valuesSize; i++) {
        int32_t value = data.ReadInt32();
        values.push_back(value);
    }

    int32_t result = GetPlayerAppSettingValueText(raw, attributes, values);
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetElementAttributesInner(MessageParcel &data, MessageParcel &reply)
{
    RawAddress raw = RawAddress(data.ReadString());
    std::vector<int32_t> attributes = {};
    int32_t attributesSize = data.ReadInt32();
    if (IsInvalidAttributesSize(attributesSize)) {
        HILOGE("BluetoothAvrcpCtStub::GetElementAttributesInner Invalid MessageParcel attributes");
        return ERR_INVALID_VALUE;
    }
    for (int i = 0; i < attributesSize; i++) {
        int32_t attribute = data.ReadInt32();
        attributes.push_back(attribute);
    }
    int32_t result = GetElementAttributes(raw, attributes);
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetPlayStatusInner(MessageParcel &data, MessageParcel &reply)
{
    RawAddress raw = RawAddress(data.ReadString());
    int32_t result = GetPlayStatus(raw);
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::PlayItemInner(MessageParcel &data, MessageParcel &reply)
{
    RawAddress raw = RawAddress(data.ReadString());
    int32_t result = PlayItem(raw, data.ReadInt32(), data.ReadInt64(), data.ReadInt32());
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetFolderItemsInner(MessageParcel &data, MessageParcel &reply)
{
    RawAddress raw = RawAddress(data.ReadString());
    int32_t startItem = data.ReadInt32();
    int32_t endItem = data.ReadInt32();
    std::vector<int32_t> attributes = {};
    int32_t attributesSize = data.ReadInt32();
    if (IsInvalidAttributesSize(attributesSize)) {
        HILOGE("BluetoothAvrcpCtStub::GetFolderItemsInner Invalid MessageParcel attributes");
        return ERR_INVALID_VALUE;
    }
    for (int i = 0; i < attributesSize; i++) {
        int32_t attribute = data.ReadInt32();
        attributes.push_back(attribute);
    }

    int32_t result = GetFolderItems(raw, startItem, endItem, attributes);
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetTotalNumberOfItemsInner(MessageParcel &data, MessageParcel &reply)
{
    RawAddress raw = RawAddress(data.ReadString());
    int32_t scope = data.ReadInt32();
    int32_t result = GetTotalNumberOfItems(raw, scope);
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::SetAbsoluteVolumeInner(MessageParcel &data, MessageParcel &reply)
{
    RawAddress raw = RawAddress(data.ReadString());
    int32_t volume = data.ReadInt32();
    int32_t result = SetAbsoluteVolume(raw, volume);
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::EnableNotificationInner(MessageParcel &data, MessageParcel &reply)
{
    RawAddress raw = RawAddress(data.ReadString());
    std::vector<int32_t> events = {};
    int32_t attributesSize = data.ReadInt32();
    if (static_cast<uint32_t>(attributesSize) > AVRCP_NOTIDICATION_EVENT_COUNT_MAX) {
        return ERR_INVALID_STATE;
    }
    for (int i = 0; i < attributesSize; i++) {
        int32_t attribute = data.ReadInt32();
        events.push_back(attribute);
    }
    int32_t interval = data.ReadInt32();
    int32_t result = EnableNotification(raw, events, interval);
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::DisableNotificationInner(MessageParcel &data, MessageParcel &reply)
{
    RawAddress raw = RawAddress(data.ReadString());
    std::vector<int32_t> events = {};
    int32_t attributesSize = data.ReadInt32();
    if (static_cast<uint32_t>(attributesSize) > AVRCP_NOTIDICATION_EVENT_COUNT_MAX) {
        return ERR_INVALID_STATE;
    }
    for (int i = 0; i < attributesSize; i++) {
        int32_t attribute = data.ReadInt32();
        events.push_back(attribute);
    }
    int32_t result = DisableNotification(raw, events);
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetItemAttributesInner(MessageParcel &data, MessageParcel &reply)
{
    RawAddress raw = RawAddress(data.ReadString());
    int64_t uid = data.ReadInt64();
    int32_t uidCounter = data.ReadInt32();
    std::vector<int32_t> events = {};
    int32_t attributesSize = data.ReadInt32();
    if (static_cast<uint32_t>(attributesSize) > AVRCP_PLAYER_APP_SETTING_ATTRIBUTES_NUM_MAX) {
        return ERR_INVALID_STATE;
    }
    for (int i = 0; i < attributesSize; i++) {
        int32_t attribute = data.ReadInt32();
        events.push_back(attribute);
    }
    int32_t result = GetItemAttributes(raw, uid, uidCounter, events);
    reply.WriteInt32(result);

    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::SetBrowsedPlayerInner(MessageParcel &data, MessageParcel &reply)
{
    RawAddress raw = RawAddress(data.ReadString());
    int32_t playerId = data.ReadInt32();
    int32_t result = SetBrowsedPlayer(raw, playerId);
    reply.WriteInt32(result);
    return NO_ERROR;
}

int32_t BluetoothAvrcpCtStub::GetMeidaPlayerListInner(MessageParcel &data, MessageParcel &reply)
{
    RawAddress raw = RawAddress(data.ReadString());
    int32_t startItem = data.ReadInt32();
    int32_t endItem = data.ReadInt32();

    int32_t result = GetMeidaPlayerList(raw, startItem, endItem);
    reply.WriteInt32(result);

    return NO_ERROR;
}

bool BluetoothAvrcpCtStub::IsInvalidAttributesSize(int32_t attributesSize)
{
    bool ret = false;
    const int32_t COUNT_OF_AVRC_PLAYER_ATTRIBUTE = 255;
    if (attributesSize > COUNT_OF_AVRC_PLAYER_ATTRIBUTE) {
        ret =  true;
    }
    return ret;
}

bool BluetoothAvrcpCtStub::IsInvalidDeviceStatesSize(int32_t statesSize)
{
    bool ret = false;
    const int32_t COUNT_OF_DEVICE_STATE = 4;
    if (statesSize > COUNT_OF_DEVICE_STATE) {
        ret =  true;
    }
    return ret;
}
}  // namespace Bluetooth
}  // namespace OHOS
