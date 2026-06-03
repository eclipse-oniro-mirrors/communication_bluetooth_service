/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
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
#include "ipc_skeleton.h"
#include "bluetooth_errorcode.h"
#include "log.h"
#include "iservice_registry.h"
#include "system_ability_definition.h"
#include "bundle_mgr_proxy.h"

namespace OHOS {
namespace Bluetooth {

using namespace OHOS;
using namespace Security::AccessToken;

#ifdef PERMISSION_ALWAYS_GRANT
bool g_isAllGranted = true;
#else
bool g_isAllGranted = false;
#endif

bool PermissionManager::VerifyPermission(const std::string &permissionName)
{
    uint32_t tokenId = IPCSkeleton::GetCallingTokenID();
    return VerifyPermission(permissionName, tokenId);
}
bool PermissionManager::VerifyPermission(const std::string &permissionName, const uint32_t &tokenId)
{
    int result;
    ATokenTypeEnum tokenType = Security::AccessToken::AccessTokenKit::GetTokenTypeFlag(tokenId);
    if (tokenType == TOKEN_NATIVE || tokenType == TOKEN_SHELL) {
        result = Security::AccessToken::PermissionState::PERMISSION_GRANTED;
        return true;
    } else if (tokenType == TOKEN_HAP) {
        result = Security::AccessToken::AccessTokenKit::VerifyAccessToken(tokenId, permissionName);
        HILOGI("permission(%{public}s)  tokenId(%{public}d), result(%{public}d)",
            permissionName.c_str(), tokenId, result);
        HapTokenInfo hapTokenInfo;
        if (Security::AccessToken::AccessTokenKit::GetHapTokenInfo(tokenId, hapTokenInfo) == RET_SUCCESS) {
            if (IsSystemHap() && (hapTokenInfo.bundleName == "com.ohos.settings" ||
                hapTokenInfo.bundleName == "com.ohos.systemui")) {
                return true;
            }
        }
        bool ret = (result == Security::AccessToken::PermissionState::PERMISSION_GRANTED);
        return ret;
    } else {
        HILOGI("[PERMISSION] unknown token type(%{public}d)", tokenType);
        return false;
    }
}

bool PermissionManager::IsPermissionsGranted(const  std::set<std::string> &permissions)
{
    for (auto &it : permissions) {
        CHECK_AND_RETURN_LOG_RET(VerifyPermission(it), false,
            "[PERMISSION] check permission failed, permission(%{public}s)", it.c_str());
    }
    return true;
}

int32_t PermissionManager::VerifyMultiPermissions(const std::shared_ptr<PermissionItem> &item)
{
    if (g_isAllGranted) {
        return Bluetooth::BT_NO_ERROR;
    }
    if (item == nullptr) {
        return Bluetooth::BT_NO_ERROR;
    }

    int32_t apiVerIdx = API_VERSION_10;
    int32_t apiVersion = API_VERSION_10;
    if (IsHapCaller()) {
        apiVersion = GetApiVersion();
        apiVerIdx =
            (apiVersion >= API_VERSION_10 || apiVersion == API_VERSION_INVALID) ? API_VERSION_10 : API_VERSION_9;
    }
    std::set<std::string> perm = item->GetPermissions(apiVerIdx);
    CHECK_AND_RETURN_LOG_RET(!perm.empty(), Bluetooth::BT_ERR_PERMISSION_FAILED,
        "[PERMISSION] the caller's api version is unsupported, apiVersion(%{public}d)", apiVersion);
    if (item->SystemHapNeeded() && !IsSystemHap()) {
        HILOGE("[PERMISSION] system hap needed.");
        return Bluetooth::BT_ERR_SYSTEM_PERMISSION_FAILED;
    }
    CHECK_AND_RETURN_LOG_RET(IsPermissionsGranted(perm), Bluetooth::BT_ERR_PERMISSION_FAILED,
        "[PERMISSION] check permissions failed. apiVersion is %{public}d", apiVersion);
    return Bluetooth::BT_NO_ERROR;
}

int32_t PermissionManager::GetCallingUid()
{
    return IPCSkeleton::GetCallingUid();
}

bool PermissionManager::IsHapCaller(void)
{
    uint32_t tokenId = IPCSkeleton::GetCallingTokenID();
    return IsHapCaller(tokenId);
}

bool PermissionManager::IsHapCaller(uint32_t &tokenId)
{
    ATokenTypeEnum callingType = AccessTokenKit::GetTokenTypeFlag(tokenId);
    if (callingType == ATokenTypeEnum::TOKEN_HAP) {
        return true;
    }
    return false;
}

int32_t PermissionManager::GetApiVersion()
{
    uint32_t tokenId = IPCSkeleton::GetCallingTokenID();
    return GetApiVersion(tokenId);
}

int32_t PermissionManager::GetApiVersion(uint32_t tokenId)
{
    if (!IsHapCaller(tokenId)) {
        return API_VERSION_INVALID;
    }
    HapTokenInfo hapTokenInfo;
    if (AccessTokenKit::GetHapTokenInfo(tokenId, hapTokenInfo) != AccessTokenKitRet::RET_SUCCESS) {
        HILOGE("[PERMISSION] GetHapTokenInfo failed.");
        return API_VERSION_INVALID;
    }
    return hapTokenInfo.apiVersion;
}

bool PermissionManager::IsNativeCaller(void)
{
    uint32_t tokenId = IPCSkeleton::GetCallingTokenID();
    return IsNativeCaller(tokenId);
}
bool PermissionManager::IsNativeCaller(const uint32_t &tokenId)
{
    ATokenTypeEnum callingType = AccessTokenKit::GetTokenTypeFlag(tokenId);
    if (callingType == ATokenTypeEnum::TOKEN_NATIVE || callingType == ATokenTypeEnum::TOKEN_SHELL) {
        return true;
    }
    return false;
}
bool PermissionManager::IsSystemHap()
{
    uint64_t fullTokenId = IPCSkeleton::GetCallingFullTokenID();
    return IsSystemHap(fullTokenId);
}

bool PermissionManager::IsSystemHap(const uint64_t &fullTokenId)
{
    bool isSystemApp = Security::AccessToken::TokenIdKit::IsSystemAppByFullTokenID(fullTokenId);
    Security::AccessToken::ATokenTypeEnum callingType =
        Security::AccessToken::AccessTokenKit::GetTokenTypeFlag(static_cast<uint32_t>(fullTokenId));
    if (callingType == Security::AccessToken::ATokenTypeEnum::TOKEN_HAP && !isSystemApp) {
        return false;
    }
    return true;
}

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

std::shared_ptr<PermissionItem> PermissionManager::CreateItem(bool isSystemApi,
    const std::set<std::string> api9PermSet, const std::set<std::string> api10PermSet)
{
    return std::make_shared<PermissionItem>(isSystemApi, api9PermSet, api10PermSet);
}
} // namespace Bluetooth
} // namespace OHOS