/**
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

/**
 * 与 entry/src/main/resources/base/profile/main_pages.json 中 src 数组保持同步，
 * 供 Hypium 用例做「路由注册完整性」校验（不依赖主模块源码）。
 * 若新增页面，请同步更新 main_pages.json 与本常量。
 */
export const BLUETOOTHTEST_ENTRY_PAGE_COUNT: number = 90;

/** 主模块 entry 中注册的全部页面路径（不含前缀 /） */
export const BLUETOOTHTEST_ENTRY_PAGE_ROUTES: string[] = [
  'pages/homePage',
  'pages/subManualApiTest/BrInterfaceTest',
  'pages/subManualApiTest/bleTest',
  'pages/subManualApiTest/gattTest',
  'pages/subManualApiTest/sppTest',
  'pages/subManualApiTest/profileTest',
  'pages/subManualApiTestManager/brTestManager',
  'pages/subManualApiTestManager/bleTestManager',
  'pages/subManualApiTestManager/gattTestManager',
  'pages/subManualApiTestManager/sppTestManager',
  'pages/subManualApiTestManager/profileTestManager',
  'pages/subManualApiTest/subBrTest/deviceName',
  'pages/subManualApiTest/subBrTest/setScanMode',
  'pages/subManualApiTest/subBrTest/deviceFound',
  'pages/subManualApiTest/subBrTest/profileConnState',
  'pages/subManualApiTest/subBrTest/searchProfileConnStateById',
  'pages/subManualApiTest/subBrTest/pairedDevices',
  'pages/subManualApiTest/subGattTest/gattClient',
  'pages/subManualApiTest/subGattTest/gattServer',
  'pages/subManualApiTest/subSppTest/sppClientTest',
  'pages/subManualApiTest/subSppTest/sppServerTest',
  'pages/subManualApiTestManager/subGattTestManager/gattClientManager',
  'pages/subManualApiTestManager/subGattTestManager/gattServerManager',
  'pages/subManualApiTestManager/subSppTestManager/sppClientManager',
  'pages/subManualApiTestManager/subSppTestManager/sppServerManager',
  'pages/subAutoTest/brAutoTest',
  'pages/subAutoTest/bleAutoTest',
  'pages/subAutoTest/gattAutoTest',
  'pages/subAutoTest/sppAutoTest',
  'pages/subAutoTest/profileAutoTest',
  'pages/subAutoTest/subGattAutoTest/gattClientAutoTest',
  'pages/subAutoTest/subGattAutoTest/gattServerAutoTest',
  'pages/subAutoTest/subSppAutoTest/sppClientAutoTest',
  'pages/subAutoTest/subSppAutoTest/sppServerAutoTest',
  'pages/subAutoTest/subProfileAutoTest/a2dpSourceAutoTest',
  'pages/subAutoTest/subProfileAutoTest/handsFreeAudioGatewayAutoTest',
  'pages/subAutoTest/subProfileAutoTest/hitHostAutoTest',
  'pages/subAutoTest/subProfileAutoTest/panNetworkAutoTest',
  'pages/subAutoTestManager/brAutoTestManager',
  'pages/subAutoTestManager/bleAutoTestManager',
  'pages/subAutoTestManager/gattAutoTestManager',
  'pages/subAutoTestManager/sppAutoTestManager',
  'pages/subAutoTestManager/profileAutoTestManager',
  'pages/subAutoTestManager/subGattAutoTestManager/gattClientAutoTestManager',
  'pages/subAutoTestManager/subGattAutoTestManager/gattServerAutoTestManager',
  'pages/subAutoTestManager/subSppAutoTestManager/sppClientAutoTestManager',
  'pages/subAutoTestManager/subSppAutoTestManager/sppServerAutoTestManager',
  'pages/subAutoTestManager/subProfileAutoTestManager/a2dpSourceAutoTestManager',
  'pages/subAutoTestManager/subProfileAutoTestManager/handsFreeAudioGatewayAutoTestManager',
  'pages/subAutoTestManager/subProfileAutoTestManager/hitHostAutoTestManager',
  'pages/subAutoTestManager/subProfileAutoTestManager/panNetworkAutoTestManager',
  'pages/subScenarioTest/brFoundScenarioTest',
  'pages/subScenarioTest/bleFoundScenarioTest',
  'pages/subScenarioTest/gattClientScenarioTest',
  'pages/subScenarioTest/gattServerScenarioTest',
  'pages/subScenarioTest/sppClientScenarioTest',
  'pages/subScenarioTest/sppServerScenarioTest',
  'pages/subScenarioTest/oppSendScenarioTest',
  'pages/subScenarioTest/oppRecvScenarioTest',
  'pages/subScenarioTestManager/brFoundScenarioTestManager',
  'pages/subScenarioTestManager/bleFoundScenarioTestManager',
  'pages/subScenarioTestManager/gattClientScenarioTestManager',
  'pages/subScenarioTestManager/gattServerScenarioTestManager',
  'pages/subScenarioTestManager/sppClientScenarioTestManager',
  'pages/subScenarioTestManager/sppServerScenarioTestManager',
  'pages/subProfileTest/a2dpSourceProfileTest',
  'pages/subProfileTest/handsFreeAudioGatewayProfileTest',
  'pages/subProfileTest/hidHostProfileTest',
  'pages/subProfileTest/panNetworkProfileTest',
  'pages/subProfileTest/oppProfileTest',
  'pages/subProfileTestManager/a2dpSourceProfileTestManager',
  'pages/subProfileTestManager/handsFreeAudioGatewayProfileTestManager',
  'pages/subProfileTestManager/hidHostProfileTestManager',
  'pages/subProfileTestManager/panNetworkProfileTestManager',
  'pages/subBenchmarkTest/brBenchmarkTest',
  'pages/subBenchmarkTest/bleBenchmarkTest',
  'pages/subBenchmarkTest/gattClientVelocityBenchmarkTest',
  'pages/subBenchmarkTest/gattServerVelocityBenchmarkTest',
  'pages/subBenchmarkTest/sppClientVelocityBenchmarkTest',
  'pages/subBenchmarkTest/sppServerVelocityBenchmarkTest',
  'pages/subBenchmarkTest/sppClientBandWidthBenchmarkTest',
  'pages/subBenchmarkTest/sppServerBandWidthBenchmarkTest',
  'pages/subBenchmarkTestManager/brBenchmarkTestManager',
  'pages/subBenchmarkTestManager/bleBenchmarkTestManager',
  'pages/subBenchmarkTestManager/gattClientVelocityBenchmarkTestManager',
  'pages/subBenchmarkTestManager/gattServerVelocityBenchmarkTestManager',
  'pages/subBenchmarkTestManager/sppClientBandWidthBenchmarkTestManager',
  'pages/subBenchmarkTestManager/sppClientVelocityBenchmarkTestManager',
  'pages/subBenchmarkTestManager/sppServerBandWidthBenchmarkTestManager',
  'pages/subBenchmarkTestManager/sppServerVelocityBenchmarkTestManager'
];


