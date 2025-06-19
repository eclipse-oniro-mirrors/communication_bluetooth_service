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

#include "dialog_switch.h"

#include <algorithm>
#include <string>

#include "bluetooth_dialog.h"
#include "bool_wrapper.h"
#include "double_wrapper.h"
#include "foundation/ability/ability_base/interfaces/inner_api/base/include/bool_wrapper.h"
#include "foundation/ability/ability_runtime/interfaces/inner_api/extension_manager/include/extension_manager_client.h"
#include "foundation/ability/ability_runtime/interfaces/kits/native/ability/ability_runtime/ability_connect_callback.h"
#include "int_wrapper.h"
#include "ipc_skeleton.h"
#include "json/json.h"
#include "json/writer.h"
#include "log.h"
#include "nlohmann/json.hpp"
#include "permission_manager.h"
#include "raw_address.h"
#include "refbase.h"
#include "string_wrapper.h"
#include "want_params_wrapper.h"

namespace OHOS {
namespace bluetooth {

bool DialogSwitch::RequestBluetoothSwitchDialog(DialogSwitchType type)
{
    const std::string abilityName = "bluetooth";
    const std::string bundleName = "com.ohos.settings";
    std::string thirdlyBundleName = PermissionManager::GetCallingName();
    std::string connectStr = DialogSwitch::BuildStartCommand(type, thirdlyBundleName);
    HILOGI("The bundlename is %{public}s.", thirdlyBundleName.c_str());
    if (!BluetoothDialog::DialogConnectExtension(connectStr, bundleName, abilityName)) {
        HILOGE("Failed to build switch dialog.");
        return false;
    }
    return true;
}

std::string DialogSwitch::BuildStartCommand(DialogSwitchType type, std::string thirdlyBundleName)
{
    std::string types;
    if (type == ENABLE_BLUETOOTH) {
        types = "enable";
    } else {
        types = "disable";
    }
    nlohmann::json root;
    std::string uiType = "sysDialog/common";
    root["ability.want.params.uiExtensionType"] = uiType;
    root["bundleName"] = thirdlyBundleName;
    root["type"] = types;
    std::string cmdData = root.dump();
    HILOGI("cmdData is: %{public}s.", cmdData.c_str());
    return cmdData;
}
} // namespace bluetooth
} // namespace OHOS
