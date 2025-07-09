/*
 * Copyright (C) 2024 Huawei Device Co., Ltd. 2023. All rightsreserved
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

#include "bluetooth_ability_connection.h"

#include <algorithm>
#include <string>

#include "hitrace_meter.h"
#include "log.h"

constexpr int32_t PARAM_NUM = 3;

namespace OHOS {
namespace bluetooth {
void ::OHOS::bluetooth::BluetoothAbilityConnection::OnAbilityConnectDone(
    const AppExecFwk::ElementName& element, const sptr<IRemoteObject>& remoteObject, int32_t resultCode)
{
    HILOGI("on BT_TRACE_TAG ability connected");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    data.WriteInt32(PARAM_NUM);
    data.WriteString16(u"bundleName");
    data.WriteString16(Str8ToStr16(bundleName_));
    data.WriteString16(u"abilityName");
    data.WriteString16(Str8ToStr16(abilityName_));
    data.WriteString16(u"parameters");
    data.WriteString16(Str8ToStr16(commandStr_));

    if (!data.WriteParcelable(&element)) {
        HILOGE("Connect done element error.");
        return;
    }

    if (!data.WriteRemoteObject(remoteObject)) {
        HILOGE("Connect done remote object error.");
        return;
    }

    if (!data.WriteInt32(resultCode)) {
        HILOGE("Connect done result code error.");
        return;
    }

    int32_t errCode =
        remoteObject->SendRequest(AAFwk::IAbilityConnection::ON_ABILITY_CONNECT_DONE, data, reply, option);
    HILOGI("AbilityConnectionWrapperProxy::OnConnectConnectDone result %{public}d", errCode);
}

void ::OHOS::bluetooth::BluetoothAbilityConnection::OnAbilityDisconnectDone(
    const AppExecFwk::ElementName& element, int32_t resultCode)
{
    HILOGI("on ability disconnected");
}

} // namespace bluetooth
} // namespace OHOS