/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>

#include "bt_vendor.h"

using namespace testing::ext;
using namespace OHOS::bluetooth;

class VendorInterfaceTest : public testing::Test {};

HWTEST_F(VendorInterfaceTest, VendorInterface_CommandMapping_001, TestSize.Level1)
{
    EXPECT_EQ(OH_BtGetVendorCommandType(VendorCommandType::VENDOR_BATTERY), "+VENDORBATTERY");
    EXPECT_EQ(OH_BtGetVendorCommandType(VendorCommandType::UPDATE_VENDOR_BATTERY), "+UPDATEVENDORBATTERY");
    EXPECT_EQ(OH_BtGetVendorCommandType(VendorCommandType::CLOSE_VENDOR_BATTERY), "+CLOSEVENDORBATTERY");
    EXPECT_EQ(OH_BtGetVendorCommandType(VendorCommandType::VENDOR_BATTERY_QUERY), "+VENDORBATTERY=?");
    EXPECT_EQ(OH_BtGetVendorCommandType(VendorCommandType::VENDOR_BATTERY_OK), "+VENDORBATTERY=OK");
    EXPECT_EQ(OH_BtGetVendorCommandType(VendorCommandType::X_VENDOR_SF), "+XVENDORSF");
    EXPECT_EQ(OH_BtGetVendorCommandType(VendorCommandType::XS_VENDOR_F), "+XSVENDORF");
    EXPECT_EQ(OH_BtGetVendorCommandType(VendorCommandType::VENDOR), "VENDOR");
}

HWTEST_F(VendorInterfaceTest, VendorInterface_UnknownCommand_002, TestSize.Level1)
{
    EXPECT_EQ(OH_BtGetVendorCommandType(static_cast<VendorCommandType>(0xFF)), "");
}

HWTEST_F(VendorInterfaceTest, VendorInterface_WalletCaller_003, TestSize.Level1)
{
    EXPECT_TRUE(OH_BtIsWalletService("com.ohos.wallet"));
    EXPECT_TRUE(OH_BtIsWalletService("com.ohos.wallet.service"));
    EXPECT_FALSE(OH_BtIsWalletService("com.ohos.settings"));
    EXPECT_FALSE(OH_BtIsWalletService(""));
}

HWTEST_F(VendorInterfaceTest, VendorInterface_WhitelistDefaults_004, TestSize.Level1)
{
    EXPECT_TRUE(OH_BtGetFastAdvWhitelist().empty());
    EXPECT_TRUE(OH_BtGetFastScanWhitelist().empty());
    EXPECT_TRUE(OH_BtGetRealAddressWhiteMap().empty());
    EXPECT_TRUE(OH_BtGetMmiNotDiscoveryBleWhiteMap().empty());
    EXPECT_FALSE(OH_BtIsInRealAddressWhitelist("com.ohos.settings"));
    EXPECT_EQ(OH_BtGetRealAddressWhitelistAppId("com.ohos.settings"), "");
    EXPECT_FALSE(OH_BtIsInMmiNotDiscoveryBleWhitelist("com.ohos.settings"));
    EXPECT_EQ(OH_BtGetMmiNotDiscoveryBleWhitelistAppId("com.ohos.settings"), "");
}

HWTEST_F(VendorInterfaceTest, VendorInterface_PackageNames_005, TestSize.Level1)
{
    EXPECT_EQ(OH_BtGetSettingsCallingName(), "com.ohos.settings");
    EXPECT_EQ(OH_BtGetAudioManagerPkgName(), "com.ohos.audiomanager");
    EXPECT_EQ(OH_BtGetPcFilesBundleName(), "com.ohos.filemanager");
    EXPECT_EQ(OH_BtGetPhoneFilesBundleName(), "com.ohos.filemanager");
    EXPECT_EQ(OH_BtGetPhotoBundleName(), "com.ohos.photos");
    EXPECT_EQ(OH_BtGetPhotoAbilityName(), "MainAbility");
    EXPECT_EQ(OH_BtGetVoiceRecognitionBundleName(), "com.ohos.voiceassistant");
    EXPECT_EQ(OH_BtGetWalletServiceName(), "com.ohos.wallet.service");
    EXPECT_EQ(OH_BtGetWalletPackageName(), "com.ohos.wallet");
    EXPECT_EQ(OH_BtGetWalletBleDumpAction(), "com.ohos.wallet.action.BLE_DUMP");
}
