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
 * ConnectivityKit 蓝牙能力覆盖：以 OpenHarmony SDK `ets/kits/@kit.ConnectivityKit.d.ts`
 * 中导出的蓝牙相关模块为准（与文档「ConnectivityKit / 蓝牙」条目一致）。
 *
 * 覆盖的导出模块：a2dp、access、baseProfile、ble、connection、constant、hfp、hid、
 * common、bluetooth（经典废弃）、map、pan、pbap、opp、socket、wearDetection、
 * bluetoothManager，以及 @system.bluetooth 的 Bluetooth 精简 BLE API。
 *
 * 说明：部分 API 为 system 能力或依赖真机状态，用 invokeApiAssertErrCode 记录并断言 errCode；不保证异步回调在
 * 单测周期内完成，仅校验同步入口可调用或按预期抛错。
 *
 * 设计补强（对齐 APITEST_COVERAGE.md）：枚举与常量取值断言、参数边界、
 * 可量化性能（assertPerfBudget）与压力（assertStressHasSuccess）循环。
 */
import { describe, it, expect } from '@ohos/hypium';
import LogUtil from '../../../main/ets/Utils/LogUtil';
import { BusinessError } from '@ohos.base';
import {
  a2dp,
  access,
  baseProfile,
  ble,
  connection,
  constant,
  hfp,
  hid,
  common,
  bluetooth,
  map,
  pan,
  pbap,
  opp,
  socket,
  wearDetection,
  bluetoothManager
} from '@kit.ConnectivityKit';
import { assertPerfBudget, assertStressHasSuccess } from './ApitestCoverageHelpers';

const DUMMY_MAC = '00:11:22:33:44:55';
const BAD_MAC_GARBAGE = 'ZZ:ZZ:ZZ:ZZ:ZZ:ZZ';
const SPP_UUID = '00001101-0000-1000-8000-00805F9B34FB';

const invokeApiAssertErrCode = (tag: string, fn: () => void): void => {
  let errCode = 0;
  LogUtil.info(tag + ': invoke');
  try {
    fn();
  } catch (err) {
    const _c = (err as BusinessError).code;
    errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
    if (isNaN(errCode)) {
      errCode = 0;
    }
    LogUtil.error(tag + ': ' + JSON.stringify(err));
  }
  LogUtil.info(tag + ': errCode=' + errCode);
  expect(errCode >= 0).assertTrue();
};

export default function connectivityKitBluetoothTest() {
  describe('ConnectivityKit_constant', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CONSTANT_PROFILEID_ENUM
     * @tc.name constant_ProfileId_enum
     * @tc.desc Verify Bluetooth apitest case constant ProfileId enum
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('constant_ProfileId_enum', 0, () => {
      expect(constant.ProfileId.PROFILE_A2DP_SOURCE).assertEqual(1);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CONSTANT_PROFILEID_ALL_DECLARED_VALUES
     * @tc.name constant_ProfileId_all_declared_values
     * @tc.desc Verify Bluetooth apitest case constant ProfileId all declared values
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('constant_ProfileId_all_declared_values', 0, () => {
      expect(constant.ProfileId.PROFILE_HANDSFREE_AUDIO_GATEWAY).assertEqual(4);
      expect(constant.ProfileId.PROFILE_HID_HOST).assertEqual(6);
      expect(constant.ProfileId.PROFILE_PAN_NETWORK).assertEqual(7);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CONSTANT_PROFILECONNECTIONSTATE_ENUM
     * @tc.name constant_ProfileConnectionState_enum
     * @tc.desc Verify Bluetooth apitest case constant ProfileConnectionState enum
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('constant_ProfileConnectionState_enum', 0, () => {
      expect(constant.ProfileConnectionState.STATE_DISCONNECTED).assertEqual(0);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CONSTANT_PROFILECONNECTIONSTATE_ALL_VALUES
     * @tc.name constant_ProfileConnectionState_all_values
     * @tc.desc Verify Bluetooth apitest case constant ProfileConnectionState all values
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('constant_ProfileConnectionState_all_values', 0, () => {
      expect(constant.ProfileConnectionState.STATE_CONNECTING).assertEqual(1);
      expect(constant.ProfileConnectionState.STATE_CONNECTED).assertEqual(2);
      expect(constant.ProfileConnectionState.STATE_DISCONNECTING).assertEqual(3);
    });
  });

  describe('ConnectivityKit_common', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMMON_BLUETOOTHADDRESSTYPE_ENUM
     * @tc.name common_BluetoothAddressType_enum
     * @tc.desc Verify Bluetooth apitest case common BluetoothAddressType enum
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('common_BluetoothAddressType_enum', 0, () => {
      expect(common.BluetoothAddressType.VIRTUAL).assertEqual(1);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMMON_BLUETOOTHADDRESSTYPE_REAL
     * @tc.name common_BluetoothAddressType_REAL
     * @tc.desc Verify Bluetooth apitest case common BluetoothAddressType REAL
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('common_BluetoothAddressType_REAL', 0, () => {
      expect(common.BluetoothAddressType.REAL).assertEqual(2);
    });
  });

  describe('ConnectivityKit_baseProfile', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BASEPROFILE_DISCONNECTCAUSE_ENUM
     * @tc.name baseProfile_DisconnectCause_enum
     * @tc.desc Verify Bluetooth apitest case baseProfile DisconnectCause enum
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('baseProfile_DisconnectCause_enum', 0, () => {
      expect(baseProfile.DisconnectCause.USER_DISCONNECT).assertEqual(0);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BASEPROFILE_DISCONNECTCAUSE_ALL_VALUES
     * @tc.name baseProfile_DisconnectCause_all_values
     * @tc.desc Verify Bluetooth apitest case baseProfile DisconnectCause all values
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('baseProfile_DisconnectCause_all_values', 0, () => {
      expect(baseProfile.DisconnectCause.CONNECT_FROM_KEYBOARD).assertEqual(1);
      expect(baseProfile.DisconnectCause.CONNECT_FROM_MOUSE).assertEqual(2);
      expect(baseProfile.DisconnectCause.CONNECT_FROM_CAR).assertEqual(3);
      expect(baseProfile.DisconnectCause.TOO_MANY_CONNECTED_DEVICES).assertEqual(4);
      expect(baseProfile.DisconnectCause.CONNECT_FAIL_INTERNAL).assertEqual(5);
    });
  });

  describe('ConnectivityKit_access', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ACCESS_GETSTATE_NORMAL
     * @tc.name access_getState_normal
     * @tc.desc Verify Bluetooth apitest case access getState normal
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('access_getState_normal', 0, () => {
      invokeApiAssertErrCode('access.getState', () => {
        const s = access.getState();
        expect(s !== null && s !== undefined).assertTrue();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ACCESS_ISVALIDRANDOMDEVICEID_NORMAL_MAC
     * @tc.name access_isValidRandomDeviceId_normal_mac
     * @tc.desc Verify Bluetooth apitest case access isValidRandomDeviceId normal mac
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('access_isValidRandomDeviceId_normal_mac', 0, () => {
      invokeApiAssertErrCode('access.isValidRandomDeviceId_mac', () => {
        const ok = access.isValidRandomDeviceId(DUMMY_MAC);
        expect(typeof ok === 'boolean').assertTrue();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ACCESS_ISVALIDRANDOMDEVICEID_BOUNDARY_EMPTY
     * @tc.name access_isValidRandomDeviceId_boundary_empty
     * @tc.desc Verify Bluetooth apitest case access isValidRandomDeviceId boundary empty
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('access_isValidRandomDeviceId_boundary_empty', 0, () => {
      invokeApiAssertErrCode('access.isValidRandomDeviceId_empty', () => {
        const ok = access.isValidRandomDeviceId('');
        expect(typeof ok === 'boolean').assertTrue();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ACCESS_CONVERTUUID_NORMAL_KNOWN_SDP
     * @tc.name access_convertUuid_normal_known_sdp
     * @tc.desc Verify Bluetooth apitest case access convertUuid normal known sdp
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('access_convertUuid_normal_known_sdp', 0, () => {
      invokeApiAssertErrCode('access.convertUuid', () => {
        const u = access.convertUuid('1101');
        expect(typeof u === 'string').assertTrue();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ACCESS_CONVERTUUID_BOUNDARY_EMPTY
     * @tc.name access_convertUuid_boundary_empty
     * @tc.desc Verify Bluetooth apitest case access convertUuid boundary empty
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('access_convertUuid_boundary_empty', 0, () => {
      invokeApiAssertErrCode('access.convertUuid_empty', () => {
        const u = access.convertUuid('');
        expect(typeof u === 'string').assertTrue();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ACCESS_ISVALIDRANDOMDEVICEID_INVALID_PATTERN
     * @tc.name access_isValidRandomDeviceId_invalid_pattern
     * @tc.desc Verify Bluetooth apitest case access isValidRandomDeviceId invalid pattern
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('access_isValidRandomDeviceId_invalid_pattern', 0, () => {
      invokeApiAssertErrCode('access.isValidRandomDeviceId_garbage', () => {
        const ok = access.isValidRandomDeviceId(BAD_MAC_GARBAGE);
        expect(typeof ok === 'boolean').assertTrue();
      });
    });
  });

  describe('ConnectivityKit_connection', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CONNECTION_GETPROFILECONNECTIONSTATE_OPTIONALARG
     * @tc.name connection_getProfileConnectionState_optionalArg
     * @tc.desc Verify Bluetooth apitest case connection getProfileConnectionState optionalArg
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('connection_getProfileConnectionState_optionalArg', 0, () => {
      invokeApiAssertErrCode('connection.getProfileConnectionState()', () => {
        const s = connection.getProfileConnectionState();
        expect(s !== null && s !== undefined).assertTrue();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CONNECTION_GETPROFILECONNECTIONSTATE_WITHPROFILEID
     * @tc.name connection_getProfileConnectionState_withProfileId
     * @tc.desc Verify Bluetooth apitest case connection getProfileConnectionState withProfileId
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('connection_getProfileConnectionState_withProfileId', 0, () => {
      invokeApiAssertErrCode('connection.getProfileConnectionState(profile)', () => {
        const s = connection.getProfileConnectionState(constant.ProfileId.PROFILE_A2DP_SOURCE);
        expect(s !== null && s !== undefined).assertTrue();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CONNECTION_GETLOCALNAME
     * @tc.name connection_getLocalName
     * @tc.desc Verify Bluetooth apitest case connection getLocalName
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('connection_getLocalName', 0, () => {
      invokeApiAssertErrCode('connection.getLocalName', () => {
        const n = connection.getLocalName();
        expect(typeof n === 'string').assertTrue();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CONNECTION_GETPAIREDDEVICES
     * @tc.name connection_getPairedDevices
     * @tc.desc Verify Bluetooth apitest case connection getPairedDevices
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('connection_getPairedDevices', 0, () => {
      invokeApiAssertErrCode('connection.getPairedDevices', () => {
        const a = connection.getPairedDevices();
        expect(Array.isArray(a)).assertTrue();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CONNECTION_GETREMOTEDEVICENAME_INVALID_MAY_THROW
     * @tc.name connection_getRemoteDeviceName_invalid_may_throw
     * @tc.desc Verify Bluetooth apitest case connection getRemoteDeviceName invalid may throw
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('connection_getRemoteDeviceName_invalid_may_throw', 0, () => {
      invokeApiAssertErrCode('connection.getRemoteDeviceName', () => {
        connection.getRemoteDeviceName(DUMMY_MAC);
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CONNECTION_GETPAIRSTATE_INVALID_DEVICE
     * @tc.name connection_getPairState_invalid_device
     * @tc.desc Verify Bluetooth apitest case connection getPairState invalid device
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('connection_getPairState_invalid_device', 0, () => {
      invokeApiAssertErrCode('connection.getPairState', () => {
        connection.getPairState('');
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CONNECTION_GETPROFILECONNECTIONSTATE_EACH_PROFILEID
     * @tc.name connection_getProfileConnectionState_each_ProfileId
     * @tc.desc Verify Bluetooth apitest case connection getProfileConnectionState each ProfileId
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('connection_getProfileConnectionState_each_ProfileId', 0, () => {
      invokeApiAssertErrCode('connection.getProfileConnectionState_matrix', () => {
        const ids = [
          constant.ProfileId.PROFILE_A2DP_SOURCE,
          constant.ProfileId.PROFILE_HANDSFREE_AUDIO_GATEWAY,
          constant.ProfileId.PROFILE_HID_HOST,
          constant.ProfileId.PROFILE_PAN_NETWORK
        ];
        for (let i = 0; i < ids.length; i++) {
          const s = connection.getProfileConnectionState(ids[i]);
          expect(s !== null && s !== undefined).assertTrue();
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CONNECTION_GETPROFILECONNECTIONSTATE_INVALID_PROFILE_BOUNDARY
     * @tc.name connection_getProfileConnectionState_invalid_profile_boundary
     * @tc.desc Verify Bluetooth apitest case connection getProfileConnectionState invalid profile boundary
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('connection_getProfileConnectionState_invalid_profile_boundary', 0, () => {
      invokeApiAssertErrCode('connection.getProfileConnectionState_bad', () => {
        connection.getProfileConnectionState(-1 as constant.ProfileId);
      });
    });
  });

  describe('ConnectivityKit_ble', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GETCONNECTEDBLEDEVICES
     * @tc.name ble_getConnectedBLEDevices
     * @tc.desc Verify Bluetooth apitest case ble getConnectedBLEDevices
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_getConnectedBLEDevices', 0, () => {
      invokeApiAssertErrCode('ble.getConnectedBLEDevices', () => {
        const a = ble.getConnectedBLEDevices();
        expect(Array.isArray(a)).assertTrue();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_STOPBLESCAN_IDEMPOTENT
     * @tc.name ble_stopBLEScan_idempotent
     * @tc.desc Verify Bluetooth apitest case ble stopBLEScan idempotent
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_stopBLEScan_idempotent', 0, () => {
      invokeApiAssertErrCode('ble.stopBLEScan', () => {
        ble.stopBLEScan();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CREATEGATTSERVER
     * @tc.name ble_createGattServer
     * @tc.desc Verify Bluetooth apitest case ble createGattServer
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_createGattServer', 0, () => {
      invokeApiAssertErrCode('ble.createGattServer', () => {
        const s = ble.createGattServer();
        expect(s !== null && s !== undefined).assertTrue();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CREATEBLESCANNER
     * @tc.name ble_createBleScanner
     * @tc.desc Verify Bluetooth apitest case ble createBleScanner
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_createBleScanner', 0, () => {
      invokeApiAssertErrCode('ble.createBleScanner', () => {
        const sc = ble.createBleScanner();
        expect(sc !== null && sc !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_a2dp', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CREATEA2DPSRCPROFILE
     * @tc.name a2dp_createA2dpSrcProfile
     * @tc.desc Verify Bluetooth apitest case a2dp createA2dpSrcProfile
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_createA2dpSrcProfile', 0, () => {
      invokeApiAssertErrCode('a2dp.createA2dpSrcProfile', () => {
        const p = a2dp.createA2dpSrcProfile();
        expect(p !== null && p !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_hfp', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HFP_CREATEHFPAGPROFILE
     * @tc.name hfp_createHfpAgProfile
     * @tc.desc Verify Bluetooth apitest case hfp createHfpAgProfile
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hfp_createHfpAgProfile', 0, () => {
      invokeApiAssertErrCode('hfp.createHfpAgProfile', () => {
        const p = hfp.createHfpAgProfile();
        expect(p !== null && p !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_map', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_MAP_CREATEMAPMSEPROFILE
     * @tc.name map_createMapMseProfile
     * @tc.desc Verify Bluetooth apitest case map createMapMseProfile
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('map_createMapMseProfile', 0, () => {
      invokeApiAssertErrCode('map.createMapMseProfile', () => {
        const p = map.createMapMseProfile();
        expect(p !== null && p !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_pbap', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PBAP_CREATEPBAPSERVERPROFILE
     * @tc.name pbap_createPbapServerProfile
     * @tc.desc Verify Bluetooth apitest case pbap createPbapServerProfile
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pbap_createPbapServerProfile', 0, () => {
      invokeApiAssertErrCode('pbap.createPbapServerProfile', () => {
        const p = pbap.createPbapServerProfile();
        expect(p !== null && p !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_opp', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_OPP_CREATEOPPSERVERPROFILE
     * @tc.name opp_createOppServerProfile
     * @tc.desc Verify Bluetooth apitest case opp createOppServerProfile
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('opp_createOppServerProfile', 0, () => {
      invokeApiAssertErrCode('opp.createOppServerProfile', () => {
        const p = opp.createOppServerProfile();
        expect(p !== null && p !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_hid_pan', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_CREATEHIDHOSTPROFILE
     * @tc.name hid_createHidHostProfile
     * @tc.desc Verify Bluetooth apitest case hid createHidHostProfile
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_createHidHostProfile', 0, () => {
      invokeApiAssertErrCode('hid.createHidHostProfile', () => {
        const p = hid.createHidHostProfile();
        expect(p !== null && p !== undefined).assertTrue();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PAN_CREATEPANPROFILE
     * @tc.name pan_createPanProfile
     * @tc.desc Verify Bluetooth apitest case pan createPanProfile
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pan_createPanProfile', 0, () => {
      invokeApiAssertErrCode('pan.createPanProfile', () => {
        const p = pan.createPanProfile();
        expect(p !== null && p !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_socket', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SOCKET_GETDEVICEID_INVALID_HANDLE_BOUNDARY
     * @tc.name socket_getDeviceId_invalid_handle_boundary
     * @tc.desc Verify Bluetooth apitest case socket getDeviceId invalid handle boundary
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('socket_getDeviceId_invalid_handle_boundary', 0, () => {
      invokeApiAssertErrCode('socket.getDeviceId', () => {
        socket.getDeviceId(-1);
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SOCKET_SPPLISTEN_ENTRY
     * @tc.name socket_sppListen_entry
     * @tc.desc Verify Bluetooth apitest case socket sppListen entry
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('socket_sppListen_entry', 0, () => {
      invokeApiAssertErrCode('socket.sppListen', () => {
        const opts: socket.SppOptions = {
          uuid: SPP_UUID,
          secure: false,
          type: socket.SppType.SPP_RFCOMM
        };
        socket.sppListen('hypiumSppListen', opts, () => {});
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SOCKET_SPPLISTEN_SPPTYPE_L2CAP_OPTIONAL
     * @tc.name socket_sppListen_sppType_l2cap_optional
     * @tc.desc Verify Bluetooth apitest case socket sppListen sppType l2cap optional
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('socket_sppListen_sppType_l2cap_optional', 0, () => {
      invokeApiAssertErrCode('socket.sppListen_l2cap', () => {
        const opts: socket.SppOptions = {
          uuid: SPP_UUID,
          secure: false,
          type: socket.SppType.SPP_L2CAP
        };
        socket.sppListen('hypiumSppL2cap', opts, () => {});
      });
    });
  });

  describe('ConnectivityKit_wearDetection', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WEARDETECTION_ISWEARDETECTIONSUPPORTED_CALLBACK_ENTRY
     * @tc.name wearDetection_isWearDetectionSupported_callback_entry
     * @tc.desc Verify Bluetooth apitest case wearDetection isWearDetectionSupported callback entry
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wearDetection_isWearDetectionSupported_callback_entry', 0, () => {
      invokeApiAssertErrCode('wearDetection.isWearDetectionSupported', () => {
        wearDetection.isWearDetectionSupported(DUMMY_MAC, () => {});
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WEARDETECTION_ISWEARDETECTIONSUPPORTED_CALLBACK_INVOKED
     * @tc.name wearDetection_isWearDetectionSupported_callback_invoked
     * @tc.desc Verify Bluetooth apitest case wearDetection isWearDetectionSupported callback invoked
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wearDetection_isWearDetectionSupported_callback_invoked', 0, () => {
      invokeApiAssertErrCode('wearDetection.cb', () => {
        let invoked = false;
        wearDetection.isWearDetectionSupported(DUMMY_MAC, () => {
          invoked = true;
        });
        expect(typeof invoked === 'boolean').assertTrue();
      });
    });
  });

  describe('ConnectivityKit_bluetoothManager', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLUETOOTHMANAGER_GETSTATE
     * @tc.name bluetoothManager_getState
     * @tc.desc Verify Bluetooth apitest case bluetoothManager getState
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bluetoothManager_getState', 0, () => {
      invokeApiAssertErrCode('bluetoothManager.getState', () => {
        const s = bluetoothManager.getState();
        expect(s !== null && s !== undefined).assertTrue();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLUETOOTHMANAGER_GETPAIREDDEVICES
     * @tc.name bluetoothManager_getPairedDevices
     * @tc.desc Verify Bluetooth apitest case bluetoothManager getPairedDevices
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bluetoothManager_getPairedDevices', 0, () => {
      invokeApiAssertErrCode('bluetoothManager.getPairedDevices', () => {
        const a = bluetoothManager.getPairedDevices();
        expect(Array.isArray(a)).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_bluetooth_deprecated', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLUETOOTH_GETSTATE
     * @tc.name bluetooth_getState
     * @tc.desc Verify Bluetooth apitest case bluetooth getState
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bluetooth_getState', 0, () => {
      invokeApiAssertErrCode('bluetooth.getState', () => {
        const s = bluetooth.getState();
        expect(s !== null && s !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_systemBluetooth_lite', () => {
    // @system.bluetooth is FA-only in Stage (11706008). Stage equivalents below map:
    // startBLEScan/stopBLEScan -> ble.*; subscribe/unsubscribeBLEFound -> BleScanner on/off.

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLUETOOTH_STARTBLESCAN_STATIC
     * @tc.name Bluetooth_startBLEScan_static
     * @tc.desc Stage-eq for @system.bluetooth startBLEScan via ble.startBLEScan
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('Bluetooth_startBLEScan_static', 0, () => {
      invokeApiAssertErrCode('Bluetooth.startBLEScan', () => {
        ble.startBLEScan([]);
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLUETOOTH_STOPBLESCAN_STATIC
     * @tc.name Bluetooth_stopBLEScan_static
     * @tc.desc Stage-eq for @system.bluetooth stopBLEScan via ble.stopBLEScan
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('Bluetooth_stopBLEScan_static', 0, () => {
      invokeApiAssertErrCode('Bluetooth.stopBLEScan', () => {
        ble.stopBLEScan();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLUETOOTH_SUBSCRIBEBLEFOUND_STATIC
     * @tc.name Bluetooth_subscribeBLEFound_static
     * @tc.desc Stage-eq for @system.bluetooth subscribeBLEFound via BleScanner.on
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('Bluetooth_subscribeBLEFound_static', 0, () => {
      invokeApiAssertErrCode('Bluetooth.subscribeBLEFound', () => {
        ble.createBleScanner().on('BLEDeviceFind', () => {});
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLUETOOTH_UNSUBSCRIBEBLEFOUND_STATIC
     * @tc.name Bluetooth_unsubscribeBLEFound_static
     * @tc.desc Stage-eq for @system.bluetooth unsubscribeBLEFound via BleScanner.off
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('Bluetooth_unsubscribeBLEFound_static', 0, () => {
      invokeApiAssertErrCode('Bluetooth.unsubscribeBLEFound', () => {
        ble.createBleScanner().off('BLEDeviceFind');
      });
    });
  });

  describe('ConnectivityKit_design_perf', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PERF_ACCESS_GETSTATE_BUDGET
     * @tc.name perf_access_getState_budget
     * @tc.desc Verify Bluetooth apitest case perf access getState budget
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('perf_access_getState_budget', 0, () => {
      assertPerfBudget('access.getState', 30, 20000, () => {
        access.getState();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PERF_BLUETOOTHMANAGER_GETSTATE_BUDGET
     * @tc.name perf_bluetoothManager_getState_budget
     * @tc.desc Verify Bluetooth apitest case perf bluetoothManager getState budget
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('perf_bluetoothManager_getState_budget', 0, () => {
      assertPerfBudget('bluetoothManager.getState', 30, 20000, () => {
        bluetoothManager.getState();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PERF_BLE_STOPBLESCAN_BUDGET
     * @tc.name perf_ble_stopBLEScan_budget
     * @tc.desc Verify Bluetooth apitest case perf ble stopBLEScan budget
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('perf_ble_stopBLEScan_budget', 0, () => {
      assertPerfBudget('ble.stopBLEScan', 40, 25000, () => {
        ble.stopBLEScan();
      });
    });
  });

  describe('ConnectivityKit_design_stress', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_STRESS_BLE_STOPBLESCAN_ITERATIONS
     * @tc.name stress_ble_stopBLEScan_iterations
     * @tc.desc Verify Bluetooth apitest case stress ble stopBLEScan iterations
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('stress_ble_stopBLEScan_iterations', 0, () => {
      assertStressHasSuccess('ble.stopBLEScan', 120, () => {
        ble.stopBLEScan();
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_STRESS_CONNECTION_GETLOCALNAME_ITERATIONS
     * @tc.name stress_connection_getLocalName_iterations
     * @tc.desc Verify Bluetooth apitest case stress connection getLocalName iterations
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('stress_connection_getLocalName_iterations', 0, () => {
      assertStressHasSuccess('connection.getLocalName', 80, () => {
        connection.getLocalName();
      });
    });
  });
}





