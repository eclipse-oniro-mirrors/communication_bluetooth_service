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

#ifndef PERMISSION_MANAGER_H
#define PERMISSION_MANAGER_H
#include <map>
#include <string>
#include "accesstoken_kit.h"
#include "permission_item.h"
#include "tokenid_kit.h"
#ifdef CHECK_PERM
#undef CHECK_PERM
#endif
#define CHECK_PERM(isSystemApi, api9Perm, api10Perm) PermissionManager::CreateItem(isSystemApi, api9Perm, api10Perm)

#ifdef MULTI_PERM
#undef MULTI_PERM
#endif
#define MULTI_PERM(...) {__VA_ARGS__}

#define CHECK_PERMISSION_AND_EXECUTE_FUNC_RETURN(STUB_CLASS)                        \
do {                                                                                \
    HILOGI("cmd(%{public}u), flags(%{public}d)", code, option.GetFlags());          \
    if (STUB_CLASS::GetDescriptor() != data.ReadInterfaceToken()) {                 \
        HILOGE("interface token check failed.");                                    \
        return ERR_INVALID_STATE;                                                   \
    }                                                                               \
    auto itFunc = memberFuncMap_.find(code);                                        \
    if (itFunc == memberFuncMap_.end()) {                                           \
        HILOGE("code(%{public}d) is not exist.", code);                             \
        return IPCObjectStub::OnRemoteRequest(code, data, reply, option);           \
    }                                                                               \
    auto memberFunc = itFunc->second.first;                                         \
    if (memberFunc == nullptr) {                                                    \
        HILOGE("memberFunc is nullptr. code(%{public}d)", code);                    \
        return IPCObjectStub::OnRemoteRequest(code, data, reply, option);           \
    }                                                                               \
    int errCode = PermissionManager::VerifyMultiPermissions(itFunc->second.second); \
    if (errCode != NO_ERROR) {                                                      \
        HILOGE("[PERMISSION] failed. code(%{public}d)", code);                      \
        reply.WriteInt32(errCode);                                                  \
        return NO_ERROR;                                                            \
    }                                                                               \
    return memberFunc(this, data, reply);                                           \
} while (0)                                                                         \

namespace OHOS {
namespace Bluetooth {

const std::string GET_BLUETOOTH_LOCAL_MAC =
    "ohos.permission.GET_BLUETOOTH_LOCAL_MAC";
const std::string ACCESS_BLUETOOTH = "ohos.permission.ACCESS_BLUETOOTH";
const std::string USE_BLUETOOTH = "ohos.permission.USE_BLUETOOTH";
const std::string DISCOVER_BLUETOOTH = "ohos.permission.DISCOVER_BLUETOOTH";
const std::string MANAGE_BLUETOOTH = "ohos.permission.MANAGE_BLUETOOTH";

const std::string APPROXIMATELY_LOCATION = "ohos.permission.APPROXIMATELY_LOCATION";
const std::string LOCATION = "ohos.permission.LOCATION";

class PermissionManager {
public:
    static bool VerifyPermission(const std::string &permissionName);
    static bool VerifyPermission(
        const std::string &permissionName, const uint32_t &tokenId);
    static int32_t VerifyMultiPermissions(const std::shared_ptr<PermissionItem> &item);
    static int32_t GetApiVersion();
    static int32_t GetApiVersion(uint32_t tokenId);
    static int32_t GetCallingUid();
    static std::string GetCallingName();
    static std::string GetCallingName(const uint32_t& tokenId);
    static bool IsHapCaller(void);
    static bool IsHapCaller(uint32_t &tokenId);
    static bool IsNativeCaller(void);
    static bool IsNativeCaller(const uint32_t &tokenId);
    static bool IsSystemHap();
    static bool IsSystemHap(const uint64_t &fullTokenId);
    static std::shared_ptr<PermissionItem> CreateItem(
        bool isSystemApi, const std::set<std::string> api9PermSet, const std::set<std::string> api10PermSet);
private:
    static bool IsPermissionsGranted(const  std::set<std::string> &permissions);
};

} // namespace Bluetooth
} // namespace OHOS
#endif