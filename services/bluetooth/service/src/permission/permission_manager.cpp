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

#include "permission_manager.h"

#include <cstdint>
#include <string>

#include "access_token.h"
#include "accesstoken_kit.h"
#include "bundlemgr/bundle_mgr_proxy.h"
#include "common/log.h"
#include "hap_token_info.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "native_token_info.h"
#include "system_ability_definition.h"
#include "tokenid_kit.h"

#include "hitrace_meter.h"

namespace OHOS {
namespace bluetooth {

std::string PermissionManager::GetCallingName()
{
    uint32_t tokenId = IPCSkeleton::GetCallingTokenID();
    return GetCallingName(tokenId);
}

std::string PermissionManager::GetCallingName(const uint32_t& tokenId)
{
    Security::AccessToken::ATokenTypeEnum callingType =
        Security::AccessToken::AccessTokenKit::GetTokenTypeFlag(tokenId);
    switch (callingType) {
        case Security::AccessToken::ATokenTypeEnum::TOKEN_HAP: {
            Security::AccessToken::HapTokenInfo hapTokenInfo;
            if (Security::AccessToken::AccessTokenKit::GetHapTokenInfo(tokenId, hapTokenInfo) ==
                Security::AccessToken::AccessTokenKitRet::RET_SUCCESS) {
                return hapTokenInfo.bundleName;
            }
            HILOGE("permission callingtype(%{public}d), getHapTokenInfoFail", callingType);
            return "";
        }
        case OHOS::Security::AccessToken::ATokenTypeEnum::TOKEN_SHELL:
        case OHOS::Security::AccessToken::ATokenTypeEnum::TOKEN_NATIVE: {
            Security::AccessToken::NativeTokenInfo naitiveTokenInfo;
            if (Security::AccessToken::AccessTokenKit::GetNativeTokenInfo(tokenId, naitiveTokenInfo) ==
                Security::AccessToken::AccessTokenKitRet::RET_SUCCESS) {
                return naitiveTokenInfo.processName;
            }
            HILOGE("permission callingtype(%{public}d), getNativeTokenInfoFail", callingType);
            return "";
        }
        default:
            HILOGE("permission callingtype(%{public}d) is invalid.", callingType);
            return "";
    }
}

bool PermissionManager::IsSystemHap()
{
    uint64_t fullTokenId = IPCSkeleton::GetCallingFullTokenID();
    return IsSystemHap(fullTokenId);
}

bool PermissionManager::IsSystemHap(const uint64_t& fullTokenId)
{
    bool isSystemApp = Security::AccessToken::TokenIdKit::IsSystemAppByFullTokenID(fullTokenId);
    Security::AccessToken::ATokenTypeEnum callingType =
        Security::AccessToken::AccessTokenKit::GetTokenTypeFlag(static_cast<uint32_t>(fullTokenId));
    if (callingType == Security::AccessToken::ATokenTypeEnum::TOKEN_HAP && !isSystemApp) {
        return false;
    }
    return true;
}
} // namespace bluetooth
} // namespace OHOS
