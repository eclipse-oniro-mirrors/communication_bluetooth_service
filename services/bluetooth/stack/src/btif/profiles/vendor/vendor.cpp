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

#ifndef COMMUNICATION_L2
#define COMMUNICATION_L2
#endif

#include "btif/profiles/vendor/vendor.h"

#include <string>
#include <utility>
#include <vector>

#include "btif/profiles/profile_interfaces.h"

namespace {

bt_status_t BtifVendorInit(BthwifCallbacks *callbacks)
{
    (void)callbacks;
    return BT_STATUS_SUCCESS;
}

int BtifVendorHwDynamicConfig()
{
    return 0;
}

bool BtifVendorNeedSyncPairedInfo(std::vector<std::string> &pairedInfo)
{
    (void)pairedInfo;
    return false;
}

static int BtifVendorReadRssi(STACK::RawAddress *bdAddr)
{
    (void)bdAddr;
    return {};
}

static bt_status_t BtifVendorFastConnect(int clientIf, const STACK::RawAddress *bdAddr, bool isDirect, int transport)
{
    (void)clientIf;
    (void)bdAddr;
    (void)isDirect;
    (void)transport;
    return {};
}

static int BtifVendorHfGetRemoteFeatures(STACK::RawAddress *bdAddr)
{
    (void)bdAddr;
    return {};
}

static int BtifVendorEnableRadio()
{
    return {};
}

static int BtifVendorDisableRadio()
{
    return {};
}

static const void *BtifVendorGetFmInterface()
{
    return {};
}

static int BtifVendorGetLdacQuality()
{
    return {};
}

static bool BtifVendorGetHighQuality(STACK::RawAddress *bdAddr)
{
    (void)bdAddr;
    return {};
}

static bool BtifVendorSetHighQuality(STACK::RawAddress *bdAddr, bool enable)
{
    (void)bdAddr;
    (void)enable;
    return {};
}

static bool BtifVendorGetInteropDataMatchAddr(const STACK::RawAddress *bdAddr, std::string feature)
{
    (void)bdAddr;
    (void)feature;
    return {};
}

static bool BtifVendorSetInteropDataMatchAddr(const STACK::RawAddress *bdAddr, std::string feature, bool enable)
{
    (void)bdAddr;
    (void)feature;
    (void)enable;
    return {};
}

static bool BtifVendorCheckRemoteDeviceBonded(const STACK::RawAddress &remoteAddr)
{
    (void)remoteAddr;
    return {};
}

static void BtifVendorSetInNaviMode(bool isNavi)
{
    (void)isNavi;
}

static void BtifVendorSetHiDeviceScanFlag(bool flag)
{
    (void)flag;
}

static void BtifVendorSetBurstParam(int duration, int maxExtAdvEvents, int burstWindow, int burstInterval, int advId)
{
    (void)duration;
    (void)maxExtAdvEvents;
    (void)burstWindow;
    (void)burstInterval;
    (void)advId;
}

static void BtifVendorSendLowLatencyStatus(bool isEnabled)
{
    (void)isEnabled;
}

static void BtifVendorHwBtifA2dpQueueRemove(STACK::RawAddress &bdAddr)
{
    (void)bdAddr;
}

static void BtifVendorHwBtifHfpQueueRemove(STACK::RawAddress &bdAddr)
{
    (void)bdAddr;
}

static int BtifVendorDisconnectRcuInStr(const STACK::RawAddress &bdAddr)
{
    (void)bdAddr;
    return {};
}

static void BtifVendorHwBtsndHcicDisconnect(const STACK::RawAddress &bdAddr)
{
    (void)bdAddr;
}

static bool BtifVendorHwGetDeviceInfo(const STACK::RawAddress &bdAddr, bt_property_t &prop)
{
    (void)bdAddr;
    (void)prop;
    return {};
}

static void BtifVendorHwSendSceneType(int type)
{
    (void)type;
}

static int BtifVendorHwGetCurrentSceneType()
{
    return {};
}

static STACK::RawAddress BtifVendorHwGetMasterTwsAddress(const STACK::RawAddress &addr)
{
    (void)addr;
    return {};
}

static bool BtifVendorGetTwsMonoMode()
{
    return {};
}

static void BtifVendorSetHdapActive(const STACK::RawAddress &bdAddr)
{
    (void)bdAddr;
}

static void BtifVendorSendDeviceSelection(const STACK::RawAddress &bdAddr, int useA2dp, int useHfp, int userSelection)
{
    (void)bdAddr;
    (void)useA2dp;
    (void)useHfp;
    (void)userSelection;
}

static STACK::RawAddress BtifVendorHwGetIdentityAddress(const STACK::RawAddress &addr)
{
    (void)addr;
    return {};
}

static bool BtifVendorIsIncomingConnection(const STACK::RawAddress &bdAddr)
{
    (void)bdAddr;
    return {};
}

static void BtifVendorLeConnectFastest(const STACK::RawAddress &addr)
{
    (void)addr;
}

static void BtifVendorSetCocPreferredPhy(const STACK::RawAddress &addr, int txPhy, int rxPhy, int options)
{
    (void)addr;
    (void)txPhy;
    (void)rxPhy;
    (void)options;
}

static void BtifVendorUpdateCocConnectionParams(const STACK::RawAddress &bdAddr, const int *params, int len)
{
    (void)bdAddr;
    (void)params;
    (void)len;
}

static bool BtifVendorGetSensorhubCap()
{
    return {};
}

static bool BtifVendorChangeAdvReportPath(bool isWake)
{
    (void)isWake;
    return {};
}

static uint8_t BtifVendorGetSensorhubAdvHandle()
{
    return {};
}

static void BtifVendorResetSensorhub(bool enableSensorhub)
{
    (void)enableSensorhub;
}

static void BtifVendorSendParamsToSensorhub(const char *params, char type, int length)
{
    (void)params;
    (void)type;
    (void)length;
}

static void BtifVendorDeleteSensorhubFilter(char filterID)
{
    (void)filterID;
}

static bool BtifVendorHwSetAdvKey(
    const STACK::RawAddress &addr, const char &irk, const char &hbk, const uint8_t version, const char &feature)
{
    (void)addr;
    (void)irk;
    (void)hbk;
    (void)version;
    (void)feature;
    return {};
}

static bool BtifVendorHwRemoveAdvKey(const STACK::RawAddress &addr)
{
    (void)addr;
    return {};
}

static void BtifVendorHwGetAdvIrk(const STACK::RawAddress &addr, std::vector<uint8_t> &irk)
{
    (void)addr;
    (void)irk;
}

static void BtifVendorHwSetHDCallState(bool isEnable)
{
    (void)isEnable;
}

static void BtifVendorSendHiechoData(uint8_t type, uint8_t *source, uint16_t len, uint8_t *args)
{
    (void)type;
    (void)source;
    (void)len;
    (void)args;
}

static bool BtifVendorIsDeviceSupportEcho(const STACK::RawAddress &bdAddr)
{
    (void)bdAddr;
    return {};
}

static void BtifVendorHwSetConnReasonFlag(
    const STACK::RawAddress &addr, int connType, const uint8_t *connReasonData, int connReasonDataLen)
{
    (void)addr;
    (void)connType;
    (void)connReasonData;
    (void)connReasonDataLen;
}

static bool BtifVendorHwBtifIsHdapSupported()
{
    return {};
}

static void BtifVendorRegisterConnection(const STACK::RawAddress &addr, const ::bluetooth::Uuid &uuid)
{
    (void)addr;
    (void)uuid;
}

static void BtifVendorUnRegisterConnection(const STACK::RawAddress &addr, const ::bluetooth::Uuid &uuid)
{
    (void)addr;
    (void)uuid;
}

static void BtifVendorHwRecordRmtDeviceSpecificCap(const STACK::RawAddress &addr, int capID)
{
    (void)addr;
    (void)capID;
}

static void BtifVendorHwClearRmtDeviceSpecificCap(const STACK::RawAddress &addr, int capID)
{
    (void)addr;
    (void)capID;
}

static void BtifVendorHwSetLowLatencySwitchStatusByAddr(const STACK::RawAddress &addr, bool isOpen)
{
    (void)addr;
    (void)isOpen;
}

static void BtifVendorHwClearLowLatencySwitchStatus(const STACK::RawAddress &addr)
{
    (void)addr;
}

static void BtifVendorSetBtPower(int power)
{
    (void)power;
}

static void BtifVendorHwSetShareAudioStatus(
    bool enable, const STACK::RawAddress &shareAddr, const STACK::RawAddress &activeAddr)
{
    (void)enable;
    (void)shareAddr;
    (void)activeAddr;
}

static bool BtifVendorHwBtifIsA2dpShareSupported(const STACK::RawAddress &activeAddress)
{
    (void)activeAddress;
    return {};
}

static void BtifVendorSetBtFreezeApp(int uid, bool isFreeze)
{
    (void)uid;
    (void)isFreeze;
}

static void BtifVendorClearBtFreezeApp(int uid)
{
    (void)uid;
}

static bool BtifVendorHwBtifIsA2dpShortTimeShareSupported(const STACK::RawAddress &activeAddress)
{
    (void)activeAddress;
    return {};
}

static bool BtifVendorHwBtifIsShareAudioTwsDdmSupport(const STACK::RawAddress &shareAddr)
{
    (void)shareAddr;
    return {};
}

static void BtifVendorHwBtifTwsSetShareAudioState(int state)
{
    (void)state;
}

static void BtifVendorHwBtifBtsnoopCapture(bool captureFlag)
{
    (void)captureFlag;
}

static bool BtifVendorCheckInterOpMatchModelId(const std::string &feature, const std::string &modelId)
{
    (void)feature;
    (void)modelId;
    return {};
}

static void BtifVendorHwUpdateBtAudioLatency(const STACK::RawAddress &peer_address, int latency)
{
    (void)peer_address;
    (void)latency;
}

static int BtifVendorResumeBackGroundConnectionsInStr()
{
    return {};
}

static void BtifVendorSetScanReportChannelSensorHub(uint8_t filterIdx, uint8_t isToAp)
{
    (void)filterIdx;
    (void)isToAp;
}

static bool BtifVendorIsBondingOrSdp()
{
    return {};
}

static void BtifVendorHwSatelliteModeSwitch(int handoverDirection)
{
    (void)handoverDirection;
}

static void BtifVendorHwBtifHfpScoOccupiedClean(STACK::RawAddress &addr)
{
    (void)addr;
}

static bool BtifVendorIsHwDualConnDevice(const STACK::RawAddress &addr)
{
    (void)addr;
    return {};
}

static int BtifVendorA2dpOffloadSetSinkStreamStart(const STACK::RawAddress &addr)
{
    (void)addr;
    return {};
}

static int BtifVendorA2dpOffloadSetSinkStreamStop(const STACK::RawAddress &addr)
{
    (void)addr;
    return {};
}

static int BtifVendorA2dpOffloadGetSinkStreamEncodingPath(
    const STACK::RawAddress &addr, const std::vector<HwBtA2dpSteamSessionInfo> &sessionsInfo, bool needStart)
{
    (void)addr;
    (void)sessionsInfo;
    (void)needStart;
    return {};
}

static uint32_t BtifVendorA2dpOffloadGetLatency()
{
    return {};
}

static bool BtifVendorA2dpOffloadIsRunning(const STACK::RawAddress &addr)
{
    (void)addr;
    return {};
}

static void BtifVendorHwSetAdvIndReportFlag(int filterIndex, bool advIndReport)
{
    (void)filterIndex;
    (void)advIndReport;
}

static void BtifVendorHwDeletAdvIndReportFlag(int filterIndex)
{
    (void)filterIndex;
}

static void BtifVendorNotifyAudioRenderState(bool isRenderActive)
{
    (void)isRenderActive;
}

static void BtifVendorHwSetSpatialAudioModeEnabled(const bool enabled)
{
    (void)enabled;
}

static void BtifVendorHwSetSpatialAudioHeadTrackingEnabled(const bool enabled)
{
    (void)enabled;
}

static void BtifVendorHwSetCollAudioEnableState(bool state)
{
    (void)state;
}

static bool BtifVendorInteropMatch(const uint16_t feature, const STACK::RawAddress &bdAddr)
{
    (void)feature;
    (void)bdAddr;
    return {};
}

static void BtifVendorChangeCodec(int32_t appType, int32_t sampleRate, uint8_t fastFlag)
{
    (void)appType;
    (void)sampleRate;
    (void)fastFlag;
}

static void BtifVendorHwSetActiveMode(const STACK::RawAddress &addr)
{
    (void)addr;
}

static bool BtifVendorGetA2dpOffloadCodecConfig(const STACK::RawAddress &addr, A2dpOffloadConfigCallback &config)
{
    (void)addr;
    (void)config;
    return {};
}

static void BtifVendorHdapStartSession(uint8_t sampleRate, uint8_t sampleLen, uint8_t channel)
{
    (void)sampleRate;
    (void)sampleLen;
    (void)channel;
}

static void BtifVendorHwSetAudioSourceType(const uint8_t mode)
{
    (void)mode;
}

static void BtifVendorHwSetAdaptiveSwitchStatus(const bool state)
{
    (void)state;
}

static void BtifVendorHwConnAttrSet(uint8_t transport, const STACK::RawAddress &addr, const HwConnAttr *attr)
{
    (void)transport;
    (void)addr;
    (void)attr;
}

static void BtifVendorHwVendorSpecificCommand(SpecificCmdToHisi *cmd)
{
    (void)cmd;
}

static void BtifVendorHwEnableHisiPcm(bool state)
{
    (void)state;
}

static void BtifVendorHwSetLocalVoiceCombineFeatureState(const int featureState)
{
    (void)featureState;
}

static int BtifVendorHwGetLocalVoiceCombineFeatureState()
{
    return {};
}

static bool BtifVendorHwGetRemoteVoiceCombineAbility(const STACK::RawAddress &addr)
{
    (void)addr;
    return {};
}

static void BtifVendorHwSetLocalVoiceCombineNnState(const int state)
{
    (void)state;
}

static void BtifVendorHwSetLeConnectionScanToFast(bool enable)
{
    (void)enable;
}

static void BtifVendorHwSetVoiceCombineScenarioEnabled(const STACK::RawAddress &addr, const bool isEnabled)
{
    (void)addr;
    (void)isEnabled;
}

static bool BtifVendorHwIsSupportEchoDevice(const STACK::RawAddress &addr)
{
    (void)addr;
    return {};
}

static uint8_t BtifVendorHwGetLocalDeviceEchoType()
{
    return {};
}

static void BtifVendorHwSetMacId(int state)
{
    (void)state;
}

static int BtifVendorGetCloudPushPowerLevel()
{
    return {};
}

const bthwif_interface_t kVendorInterface = {
    sizeof(bthwif_interface_t),
    BtifVendorInit,
    BtifVendorReadRssi,
    BtifVendorFastConnect,
    BtifVendorHfGetRemoteFeatures,
    BtifVendorEnableRadio,
    BtifVendorDisableRadio,
    BtifVendorGetFmInterface,
    BtifVendorHwDynamicConfig,
    BtifVendorGetLdacQuality,
    BtifVendorGetHighQuality,
    BtifVendorSetHighQuality,
    BtifVendorGetInteropDataMatchAddr,
    BtifVendorSetInteropDataMatchAddr,
    BtifVendorCheckRemoteDeviceBonded,
    BtifVendorSetInNaviMode,
    BtifVendorSetHiDeviceScanFlag,
    BtifVendorSetBurstParam,
    BtifVendorSendLowLatencyStatus,
    BtifVendorHwBtifA2dpQueueRemove,
    BtifVendorHwBtifHfpQueueRemove,
    BtifVendorDisconnectRcuInStr,
    BtifVendorHwBtsndHcicDisconnect,
    BtifVendorHwGetDeviceInfo,
    BtifVendorHwSendSceneType,
    BtifVendorHwGetCurrentSceneType,
    BtifVendorHwGetMasterTwsAddress,
    BtifVendorGetTwsMonoMode,
    BtifVendorSetHdapActive,
    BtifVendorSendDeviceSelection,
    BtifVendorHwGetIdentityAddress,
    BtifVendorIsIncomingConnection,
    BtifVendorLeConnectFastest,
    BtifVendorSetCocPreferredPhy,
    BtifVendorUpdateCocConnectionParams,
    BtifVendorGetSensorhubCap,
    BtifVendorChangeAdvReportPath,
    BtifVendorGetSensorhubAdvHandle,
    BtifVendorResetSensorhub,
    BtifVendorSendParamsToSensorhub,
    BtifVendorDeleteSensorhubFilter,
    BtifVendorHwSetAdvKey,
    BtifVendorHwRemoveAdvKey,
    BtifVendorHwGetAdvIrk,
    BtifVendorHwSetHDCallState,
    BtifVendorSendHiechoData,
    BtifVendorIsDeviceSupportEcho,
    BtifVendorHwSetConnReasonFlag,
    BtifVendorHwBtifIsHdapSupported,
    BtifVendorRegisterConnection,
    BtifVendorUnRegisterConnection,
    BtifVendorHwRecordRmtDeviceSpecificCap,
    BtifVendorHwClearRmtDeviceSpecificCap,
    BtifVendorHwSetLowLatencySwitchStatusByAddr,
    BtifVendorHwClearLowLatencySwitchStatus,
    BtifVendorSetBtPower,
    BtifVendorHwSetShareAudioStatus,
    BtifVendorHwBtifIsA2dpShareSupported,
    BtifVendorSetBtFreezeApp,
    BtifVendorClearBtFreezeApp,
    BtifVendorHwBtifIsA2dpShortTimeShareSupported,
    BtifVendorHwBtifIsShareAudioTwsDdmSupport,
    BtifVendorHwBtifTwsSetShareAudioState,
    BtifVendorHwBtifBtsnoopCapture,
    BtifVendorCheckInterOpMatchModelId,
    BtifVendorHwUpdateBtAudioLatency,
    BtifVendorResumeBackGroundConnectionsInStr,
    BtifVendorSetScanReportChannelSensorHub,
    BtifVendorIsBondingOrSdp,
    BtifVendorHwSatelliteModeSwitch,
    BtifVendorHwBtifHfpScoOccupiedClean,
    BtifVendorIsHwDualConnDevice,
    BtifVendorNeedSyncPairedInfo,
    BtifVendorA2dpOffloadSetSinkStreamStart,
    BtifVendorA2dpOffloadSetSinkStreamStop,
    BtifVendorA2dpOffloadGetSinkStreamEncodingPath,
    BtifVendorA2dpOffloadGetLatency,
    BtifVendorA2dpOffloadIsRunning,
    BtifVendorHwSetAdvIndReportFlag,
    BtifVendorHwDeletAdvIndReportFlag,
    BtifVendorNotifyAudioRenderState,
    BtifVendorHwSetSpatialAudioModeEnabled,
    BtifVendorHwSetSpatialAudioHeadTrackingEnabled,
    BtifVendorHwSetCollAudioEnableState,
    BtifVendorInteropMatch,
    BtifVendorChangeCodec,
    BtifVendorHwSetActiveMode,
    BtifVendorGetA2dpOffloadCodecConfig,
    BtifVendorHdapStartSession,
    BtifVendorHwSetAudioSourceType,
    BtifVendorHwSetAdaptiveSwitchStatus,
    BtifVendorHwConnAttrSet,
    BtifVendorHwVendorSpecificCommand,
    BtifVendorHwEnableHisiPcm,
    BtifVendorHwSetLocalVoiceCombineFeatureState,
    BtifVendorHwGetLocalVoiceCombineFeatureState,
    BtifVendorHwGetRemoteVoiceCombineAbility,
    BtifVendorHwSetLocalVoiceCombineNnState,
    BtifVendorHwSetLeConnectionScanToFast,
    BtifVendorHwSetVoiceCombineScenarioEnabled,
    BtifVendorHwIsSupportEchoDevice,
    BtifVendorHwGetLocalDeviceEchoType,
    BtifVendorHwSetMacId,
    BtifVendorGetCloudPushPowerLevel,
    {nullptr},
};

}  // namespace

const void *ProfileInterfaces::GetVendor()
{
    return &kVendorInterface;
}
