/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include "bt_vendor.h"

namespace OHOS {
namespace bluetooth {

/*
 * Default vendor data for the open bluetooth service.
 *
 * bluetooth/services/bluetooth/vendor_interface loads this library with
 * dlopen("libbt_vendor_service.z.so") and resolves the extern "C" symbols
 * below. A product vendor library can replace this module without changing
 * the framework.
 */
constexpr const char *SETTINGS_BUNDLE_NAME = "com.ohos.settings";
constexpr const char *AUDIO_MANAGER_BUNDLE_NAME = "com.ohos.audiomanager";
constexpr const char *PC_FILES_BUNDLE_NAME = "com.ohos.filemanager";
constexpr const char *PHONE_FILES_BUNDLE_NAME = "com.ohos.filemanager";
constexpr const char *PHOTO_BUNDLE_NAME = "com.ohos.photos";
constexpr const char *PHOTO_ABILITY_NAME = "MainAbility";
constexpr const char *VOICE_RECOGNITION_BUNDLE_NAME = "com.ohos.voiceassistant";
constexpr const char *WALLET_BUNDLE_NAME = "com.ohos.wallet";
constexpr const char *WALLET_SERVICE_NAME = "com.ohos.wallet.service";
constexpr const char *WALLET_BLE_DUMP_ACTION = "com.ohos.wallet.action.BLE_DUMP";

constexpr const char *VENDOR_BATTERY_CMD = "+VENDORBATTERY";
constexpr const char *UPDATE_VENDOR_BATTERY_CMD = "+UPDATEVENDORBATTERY";
constexpr const char *CLOSE_VENDOR_BATTERY_CMD = "+CLOSEVENDORBATTERY";
constexpr const char *VENDOR_BATTERY_QUERY_CMD = "+VENDORBATTERY=?";
constexpr const char *VENDOR_BATTERY_OK_CMD = "+VENDORBATTERY=OK";
constexpr const char *X_VENDOR_SF_CMD = "+XVENDORSF";
constexpr const char *XS_VENDOR_F_CMD = "+XSVENDORF";
constexpr const char *VENDOR_CMD = "VENDOR";

const std::vector<std::string> FAST_ADV_WHITELIST = {};
const std::unordered_set<std::string> FAST_SCAN_WHITELIST = {};
const std::map<std::string, std::string> REAL_ADDRESS_WHITELIST = {};
const std::map<std::string, std::string> MMI_NOT_DISCOVERY_BLE_WHITELIST = {};

extern "C" {
std::vector<std::string> OH_BtGetFastAdvWhitelist()
{
    return FAST_ADV_WHITELIST;
}

std::unordered_set<std::string> OH_BtGetFastScanWhitelist()
{
    return FAST_SCAN_WHITELIST;
}

bool OH_BtIsWalletService(const std::string &callingName)
{
    return callingName == WALLET_BUNDLE_NAME || callingName == WALLET_SERVICE_NAME;
}

std::string OH_BtGetRealAddressWhitelistAppId(const std::string &bundleName)
{
    auto it = REAL_ADDRESS_WHITELIST.find(bundleName);
    return it == REAL_ADDRESS_WHITELIST.end() ? "" : it->second;
}

bool OH_BtIsInRealAddressWhitelist(const std::string &bundleName)
{
    return REAL_ADDRESS_WHITELIST.find(bundleName) != REAL_ADDRESS_WHITELIST.end();
}

std::string OH_BtGetMmiNotDiscoveryBleWhitelistAppId(const std::string &bundleName)
{
    auto it = MMI_NOT_DISCOVERY_BLE_WHITELIST.find(bundleName);
    return it == MMI_NOT_DISCOVERY_BLE_WHITELIST.end() ? "" : it->second;
}

bool OH_BtIsInMmiNotDiscoveryBleWhitelist(const std::string &bundleName)
{
    return MMI_NOT_DISCOVERY_BLE_WHITELIST.find(bundleName) != MMI_NOT_DISCOVERY_BLE_WHITELIST.end();
}

std::string OH_BtGetVendorCommandType(VendorCommandType type)
{
    switch (type) {
        case VendorCommandType::VENDOR_BATTERY:
            return VENDOR_BATTERY_CMD;
        case VendorCommandType::UPDATE_VENDOR_BATTERY:
            return UPDATE_VENDOR_BATTERY_CMD;
        case VendorCommandType::CLOSE_VENDOR_BATTERY:
            return CLOSE_VENDOR_BATTERY_CMD;
        case VendorCommandType::VENDOR_BATTERY_QUERY:
            return VENDOR_BATTERY_QUERY_CMD;
        case VendorCommandType::VENDOR_BATTERY_OK:
            return VENDOR_BATTERY_OK_CMD;
        case VendorCommandType::X_VENDOR_SF:
            return X_VENDOR_SF_CMD;
        case VendorCommandType::XS_VENDOR_F:
            return XS_VENDOR_F_CMD;
        case VendorCommandType::VENDOR:
            return VENDOR_CMD;
        default:
            return "";
    }
}

std::string OH_BtGetAudioManagerPkgName()
{
    return AUDIO_MANAGER_BUNDLE_NAME;
}

std::string OH_BtGetSettingsCallingName()
{
    return SETTINGS_BUNDLE_NAME;
}

std::string OH_BtGetPcFilesBundleName()
{
    return PC_FILES_BUNDLE_NAME;
}

std::string OH_BtGetPhoneFilesBundleName()
{
    return PHONE_FILES_BUNDLE_NAME;
}

std::string OH_BtGetPhotoBundleName()
{
    return PHOTO_BUNDLE_NAME;
}

std::string OH_BtGetPhotoAbilityName()
{
    return PHOTO_ABILITY_NAME;
}

std::string OH_BtGetVoiceRecognitionBundleName()
{
    return VOICE_RECOGNITION_BUNDLE_NAME;
}

std::string OH_BtGetWalletServiceName()
{
    return WALLET_SERVICE_NAME;
}

std::string OH_BtGetWalletPackageName()
{
    return WALLET_BUNDLE_NAME;
}

std::string OH_BtGetWalletBleDumpAction()
{
    return WALLET_BLE_DUMP_ACTION;
}

std::map<std::string, std::string> OH_BtGetRealAddressWhiteMap()
{
    return REAL_ADDRESS_WHITELIST;
}

std::map<std::string, std::string> OH_BtGetMmiNotDiscoveryBleWhiteMap()
{
    return MMI_NOT_DISCOVERY_BLE_WHITELIST;
}

} // extern "C"

} // namespace bluetooth
} // namespace OHOS
