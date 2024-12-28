/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "bluetooth_dialog.h"

#include <algorithm>
#include <string>

#include "bluetooth_ability_connection.h"
#include "bool_wrapper.h"
#include "double_wrapper.h"
#include "foundation/ability/ability_runtime/interfaces/inner_api/extension_manager/include/extension_manager_client.h"
#include "foundation/ability/ability_runtime/interfaces/kits/native/ability/ability_runtime/ability_connect_callback.h"
#include "int_wrapper.h"
#include "ipc_skeleton.h"
#include "log.h"
#include "raw_address.h"
#include "string_wrapper.h"
#include "want_params_wrapper.h"

#include "base/hiviewdfx/hitrace/interfaces/native/innerkits/include/hitrace_meter/hitrace_meter.h"

namespace OHOS {
namespace bluetooth {
namespace {
constexpr int32_t DEFAULT_VALUE = -1;
constexpr const char* CALLING_NAME = "com.ohos.settings";
constexpr const char* UI_TYPE_KEY = "ability.want.params.uiExtensionType";
constexpr const char* UI_TYPE_VAL = "sysdialog/common";
constexpr const char* PARAM_DEVICE_ID = "deviceid";
constexpr const char* PARAM_DEVICE_NAME = "device";

const std::map<DialogType, std::string> ABILITY_NAME_MAP = {
    { PBAP_AUTH_DIALOG, "BluetoothPbapPeSeDialog" },
    { MAP_AUTH_DIALOG, "BluetoothMapAuthDialog" },
    { AUTO_PLAY_AUTH_DIALOG, "BluetoothAutoPlayAuthDialog" },
};

std::queue<DialogInfo> g_dialogQueue;
std::mutex g_queueMutex;
} // namespace

bool BluetoothDialog::RequestAuthDialog(DialogInfo& dialog)
{
    HILOGI("device:  request dialog, type: %{public}d", dialog.type);
    std::lock_guard<std::mutex> lock(g_queueMutex);
    if (g_dialogQueue.empty()) {
        DisplayDialog(dialog);
    }
    g_dialogQueue.push(dialog);
    return true;
}

bool BluetoothDialog::DisplayDialog(DialogInfo& dialog)
{
    HILOGI("start display dialog, device:, type: %{public}d", dialog.type);
    auto it = ABILITY_NAME_MAP.find(dialog.type);
    if (it == ABILITY_NAME_MAP.end()) {
        HILOGE("dialog ability name not exist.");
        return false;
    }
    std::string abilityName = it->second;

    std::string connectStr = BuildStartCommand(dialog.address);
    if (!DialogConnectExtension(connectStr, CALLING_NAME, abilityName)) {
        HILOGE("failed to connect dialog.");
        return false;
    }

    HILOGI("success display dialog");
    if (dialog.timer != nullptr) {
        HILOGI("start timer");
        dialog.timer->Start(dialog.timeOutMs);
    } else {
        HILOGI("refuse play dialog not need timer");
    }
    return true;
}

bool BluetoothDialog::DismissCurAndShowNext()
{
    std::lock_guard<std::mutex> lock(g_queueMutex);
    if (g_dialogQueue.empty()) {
        return false;
    }

    g_dialogQueue.pop();

    if (g_dialogQueue.empty()) {
        HILOGI("g_dialogQueue is empty, not continue");
        return false;
    }

    return DisplayDialog(g_dialogQueue.front());
}

std::string BluetoothDialog::BuildStartCommand(const std::string& address)
{
    Json::Value root;
    Json::FastWriter writer;

    RawAddress rawAddr(address);
    std::string deviceName =
        "RemoteDeviceProperties::GetInstance()->GetDeviceName(rawAddr)";

    root[UI_TYPE_KEY] = UI_TYPE_VAL;
    root[PARAM_DEVICE_ID] = address;
    if (!deviceName.empty()) {
        root[PARAM_DEVICE_NAME] = deviceName;
    } else {
        root[PARAM_DEVICE_NAME] = address;
    }

    std::string cmdData = writer.write(root);
    return cmdData;
}

bool BluetoothDialog::DialogConnectExtensionAbility(
    const AAFwk::Want& want, const std::string commandStr, const std::string bundleName, const std::string abilityName)
{
    sptr<BluetoothAbilityConnection> connection_ = sptr<BluetoothAbilityConnection>(
        new (std::nothrow) BluetoothAbilityConnection(commandStr, bundleName, abilityName));
    if (connection_ == nullptr) {
        HILOGE("connection_ is nullptr.");
        return false;
    }
    HILOGI("calling pid is : %{public}d, calling uid is: %{public}d, fullTokenId:xxx",
        IPCSkeleton::GetCallingPid(), IPCSkeleton::GetCallingUid());
    std::string identity = IPCSkeleton::ResetCallingIdentity();
    auto ret = AAFwk::ExtensionManagerClient::GetInstance().ConnectServiceExtensionAbility(
        want, connection_, nullptr, DEFAULT_VALUE);
    HILOGI("ret is:%{public}d.", ret);
    IPCSkeleton::SetCallingIdentity(identity);
    if (ret != ERR_OK) {
        HILOGE("ret isn't ERR_OK");
        return false;
    }
    return true;
}

bool BluetoothDialog::DialogConnectExtension(
    const std::string commandStr, const std::string bundleName, const std::string abilityName)
{
    AAFwk::Want want;
    std::string sceneboardName = "com.ohos.systemui";
    std::string abilityNames = "com.ohos.systemui.dialog";
    want.SetElementName(sceneboardName, abilityNames);
    bool ret = DialogConnectExtensionAbility(want, commandStr, bundleName, abilityName);
    if (!ret) {
        HILOGE("ConnectExtensionAbility failed.");
        return false;
    }
    HILOGI("ConnectExtensionAbility succeeded.");
    return true;
}
} // namespace bluetooth
} // namespace OHOS
