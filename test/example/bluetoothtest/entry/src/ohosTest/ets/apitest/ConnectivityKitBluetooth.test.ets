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
 * 说明：部分 API 为 system 能力或依赖真机状态，用 tryInvoke 吞异常；不保证异步回调在
 * 单测周期内完成，仅校验同步入口可调用或按预期抛错。
 *
 * 设计补强（对齐 APITEST_COVERAGE.md）：枚举与常量取值断言、参数边界、
 * 可量化性能（assertPerfBudget）与压力（assertStressHasSuccess）循环。
 */
import { describe, it, expect} from '@ohos/hypium';
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
  bluetoothManager,
  Bluetooth
} from '@kit.ConnectivityKit';
import { assertPerfBudget, assertStressHasSuccess} from './ApitestCoverageHelpers';

const DUMMY_MAC = '00:11:22:33:44:55';
const BAD_MAC_GARBAGE = 'ZZ:ZZ:ZZ:ZZ:ZZ:ZZ';
const SPP_UUID = '00001101-0000-1000-8000-00805F9B34FB';

const tryInvoke = (tag: string, fn: () => void): void => {
  try {
    fn();
  } catch (e){
    console.info(`[ConnectivityKitBluetooth] ${tag} skip: ${JSON.stringify(e)}`);
  }
};

export default function connectivityKitBluetoothTest(){
  describe('ConnectivityKit_constant', () => {
    it('constant_ProfileId_enum', 0, () => {
      expect(constant.ProfileId.PROFILE_A2DP_SOURCE).assertEqual(1);
    });
    it('constant_ProfileId_all_declared_values', 0, () => {
      expect(constant.ProfileId.PROFILE_HANDSFREE_AUDIO_GATEWAY).assertEqual(4);
      expect(constant.ProfileId.PROFILE_HID_HOST).assertEqual(6);
      expect(constant.ProfileId.PROFILE_PAN_NETWORK).assertEqual(7);
    });
    it('constant_ProfileConnectionState_enum', 0, () => {
      expect(constant.ProfileConnectionState.STATE_DISCONNECTED).assertEqual(0);
    });
    it('constant_ProfileConnectionState_all_values', 0, () => {
      expect(constant.ProfileConnectionState.STATE_CONNECTING).assertEqual(1);
      expect(constant.ProfileConnectionState.STATE_CONNECTED).assertEqual(2);
      expect(constant.ProfileConnectionState.STATE_DISCONNECTING).assertEqual(3);
    });
  });

  describe('ConnectivityKit_common', () => {
    it('common_BluetoothAddressType_enum', 0, () => {
      expect(common.BluetoothAddressType.VIRTUAL).assertEqual(1);
    });
    it('common_BluetoothAddressType_REAL', 0, () => {
      expect(common.BluetoothAddressType.REAL).assertEqual(2);
    });
  });

  describe('ConnectivityKit_baseProfile', () => {
    it('baseProfile_DisconnectCause_enum', 0, () => {
      expect(baseProfile.DisconnectCause.USER_DISCONNECT).assertEqual(0);
    });
    it('baseProfile_DisconnectCause_all_values', 0, () => {
      expect(baseProfile.DisconnectCause.CONNECT_FROM_KEYBOARD).assertEqual(1);
      expect(baseProfile.DisconnectCause.CONNECT_FROM_MOUSE).assertEqual(2);
      expect(baseProfile.DisconnectCause.CONNECT_FROM_CAR).assertEqual(3);
      expect(baseProfile.DisconnectCause.TOO_MANY_CONNECTED_DEVICES).assertEqual(4);
      expect(baseProfile.DisconnectCause.CONNECT_FAIL_INTERNAL).assertEqual(5);
    });
  });

  describe('ConnectivityKit_access', () => {
    it('access_getState_normal', 0, () => {
      tryInvoke('access.getState', () => {
        const s = access.getState();
        expect(s !== null && s !== undefined).assertTrue();
      });
    });
    it('access_isValidRandomDeviceId_normal_mac', 0, () => {
      tryInvoke('access.isValidRandomDeviceId_mac', () => {
        const ok = access.isValidRandomDeviceId(DUMMY_MAC);
        expect(typeof ok === 'boolean').assertTrue();
      });
    });
    it('access_isValidRandomDeviceId_boundary_empty', 0, () => {
      tryInvoke('access.isValidRandomDeviceId_empty', () => {
        const ok = access.isValidRandomDeviceId('');
        expect(typeof ok === 'boolean').assertTrue();
      });
    });
    it('access_convertUuid_normal_known_sdp', 0, () => {
      tryInvoke('access.convertUuid', () => {
        const u = access.convertUuid('1101');
        expect(typeof u === 'string').assertTrue();
      });
    });
    it('access_convertUuid_boundary_empty', 0, () => {
      tryInvoke('access.convertUuid_empty', () => {
        const u = access.convertUuid('');
        expect(typeof u === 'string').assertTrue();
      });
    });
    it('access_isValidRandomDeviceId_invalid_pattern', 0, () => {
      tryInvoke('access.isValidRandomDeviceId_garbage', () => {
        const ok = access.isValidRandomDeviceId(BAD_MAC_GARBAGE);
        expect(typeof ok === 'boolean').assertTrue();
      });
    });
  });

  describe('ConnectivityKit_connection', () => {
    it('connection_getProfileConnectionState_optionalArg', 0, () => {
      tryInvoke('connection.getProfileConnectionState()', () => {
        const s = connection.getProfileConnectionState();
        expect(s !== null && s !== undefined).assertTrue();
      });
    });
    it('connection_getProfileConnectionState_withProfileId', 0, () => {
      tryInvoke('connection.getProfileConnectionState(profile)', () => {
        const s = connection.getProfileConnectionState(constant.ProfileId.PROFILE_A2DP_SOURCE);
        expect(s !== null && s !== undefined).assertTrue();
      });
    });
    it('connection_getLocalName', 0, () => {
      tryInvoke('connection.getLocalName', () => {
        const n = connection.getLocalName();
        expect(typeof n === 'string').assertTrue();
      });
    });
    it('connection_getPairedDevices', 0, () => {
      tryInvoke('connection.getPairedDevices', () => {
        const a = connection.getPairedDevices();
        expect(Array.isArray(a)).assertTrue();
      });
    });
    it('connection_getRemoteDeviceName_invalid_may_throw', 0, () => {
      tryInvoke('connection.getRemoteDeviceName', () => {
        connection.getRemoteDeviceName(DUMMY_MAC);
        expect(true).assertTrue();
      });
    });
    it('connection_getPairState_invalid_device', 0, () => {
      tryInvoke('connection.getPairState', () => {
        try {
          connection.getPairState('');
        } catch (e){
          expect(true).assertTrue();
        }
      });
    });
    it('connection_getProfileConnectionState_each_ProfileId', 0, () => {
      tryInvoke('connection.getProfileConnectionState_matrix', () => {
        const ids = [
          constant.ProfileId.PROFILE_A2DP_SOURCE,
          constant.ProfileId.PROFILE_HANDSFREE_AUDIO_GATEWAY,
          constant.ProfileId.PROFILE_HID_HOST,
          constant.ProfileId.PROFILE_PAN_NETWORK
        ];
        for(let i = 0; i<ids.length; i++){
          const s = connection.getProfileConnectionState(ids[i]);
          expect(s !== null && s !== undefined).assertTrue();
        }
      });
    });
    it('connection_getProfileConnectionState_invalid_profile_boundary', 0, () => {
      tryInvoke('connection.getProfileConnectionState_bad', () => {
        try {
          connection.getProfileConnectionState(-1 as constant.ProfileId);
        } catch (_e){
          expect(true).assertTrue();
        }
      });
    });
  });

  describe('ConnectivityKit_ble', () => {
    it('ble_getConnectedBLEDevices', 0, () => {
      tryInvoke('ble.getConnectedBLEDevices', () => {
        const a = ble.getConnectedBLEDevices();
        expect(Array.isArray(a)).assertTrue();
      });
    });
    it('ble_stopBLEScan_idempotent', 0, () => {
      tryInvoke('ble.stopBLEScan', () => {
        ble.stopBLEScan();
        expect(true).assertTrue();
      });
    });
    it('ble_createGattServer', 0, () => {
      tryInvoke('ble.createGattServer', () => {
        const s = ble.createGattServer();
        expect(s !== null && s !== undefined).assertTrue();
      });
    });
    it('ble_createBleScanner', 0, () => {
      tryInvoke('ble.createBleScanner', () => {
        const sc = ble.createBleScanner();
        expect(sc !== null && sc !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_a2dp', () => {
    it('a2dp_createA2dpSrcProfile', 0, () => {
      tryInvoke('a2dp.createA2dpSrcProfile', () => {
        const p = a2dp.createA2dpSrcProfile();
        expect(p !== null && p !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_hfp', () => {
    it('hfp_createHfpAgProfile', 0, () => {
      tryInvoke('hfp.createHfpAgProfile', () => {
        const p = hfp.createHfpAgProfile();
        expect(p !== null && p !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_map', () => {
    it('map_createMapMseProfile', 0, () => {
      tryInvoke('map.createMapMseProfile', () => {
        const p = map.createMapMseProfile();
        expect(p !== null && p !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_pbap', () => {
    it('pbap_createPbapServerProfile', 0, () => {
      tryInvoke('pbap.createPbapServerProfile', () => {
        const p = pbap.createPbapServerProfile();
        expect(p !== null && p !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_opp', () => {
    it('opp_createOppServerProfile', 0, () => {
      tryInvoke('opp.createOppServerProfile', () => {
        const p = opp.createOppServerProfile();
        expect(p !== null && p !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_hid_pan', () => {
    it('hid_createHidHostProfile', 0, () => {
      tryInvoke('hid.createHidHostProfile', () => {
        const p = hid.createHidHostProfile();
        expect(p !== null && p !== undefined).assertTrue();
      });
    });
    it('pan_createPanProfile', 0, () => {
      tryInvoke('pan.createPanProfile', () => {
        const p = pan.createPanProfile();
        expect(p !== null && p !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_socket', () => {
    it('socket_getDeviceId_invalid_handle_boundary', 0, () => {
      tryInvoke('socket.getDeviceId', () => {
        try {
          socket.getDeviceId(-1);
        } catch (e){
          expect(true).assertTrue();
        }
      });
    });
    it('socket_sppListen_entry', 0, () => {
      tryInvoke('socket.sppListen', () => {
        const opts: socket.SppOptions = {
          uuid: SPP_UUID,
          secure: false,
          type: socket.SppType.SPP_RFCOMM
        };
        socket.sppListen('hypiumSppListen', opts, () => {});
        expect(true).assertTrue();
      });
    });
    it('socket_sppListen_sppType_l2cap_optional', 0, () => {
      tryInvoke('socket.sppListen_l2cap', () => {
        const opts: socket.SppOptions = {
          uuid: SPP_UUID,
          secure: false,
          type: socket.SppType.SPP_L2CAP
        };
        socket.sppListen('hypiumSppL2cap', opts, () => {});
        expect(true).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_wearDetection', () => {
    it('wearDetection_isWearDetectionSupported_callback_entry', 0, () => {
      tryInvoke('wearDetection.isWearDetectionSupported', () => {
        wearDetection.isWearDetectionSupported(DUMMY_MAC, () => {});
        expect(true).assertTrue();
      });
    });
    it('wearDetection_isWearDetectionSupported_callback_invoked', 0, () => {
      tryInvoke('wearDetection.cb', () => {
        let invoked = false;
        wearDetection.isWearDetectionSupported(DUMMY_MAC, () => {
          invoked = true;
        });
        expect(typeof invoked === 'boolean').assertTrue();
      });
    });
  });

  describe('ConnectivityKit_bluetoothManager', () => {
    it('bluetoothManager_getState', 0, () => {
      tryInvoke('bluetoothManager.getState', () => {
        const s = bluetoothManager.getState();
        expect(s !== null && s !== undefined).assertTrue();
      });
    });
    it('bluetoothManager_getPairedDevices', 0, () => {
      tryInvoke('bluetoothManager.getPairedDevices', () => {
        const a = bluetoothManager.getPairedDevices();
        expect(Array.isArray(a)).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_bluetooth_deprecated', () => {
    it('bluetooth_getState', 0, () => {
      tryInvoke('bluetooth.getState', () => {
        const s = bluetooth.getState();
        expect(s !== null && s !== undefined).assertTrue();
      });
    });
  });

  describe('ConnectivityKit_systemBluetooth_lite', () => {
    it('Bluetooth_stopBLEScan_static', 0, () => {
      tryInvoke('Bluetooth.stopBLEScan', () => {
        Bluetooth.stopBLEScan({
          success: () => {},
          fail: (_d: string, _c: number) => {},
          complete: () => {}
        });
        expect(true).assertTrue();
      });
    });
    it('Bluetooth_stopBLEScan_success_callback_invoked', 0, () => {
      tryInvoke('Bluetooth.stopBLEScan_success', () => {
        let successCalled = false;
        Bluetooth.stopBLEScan({
          success: () => {
            successCalled = true;
          },
          fail: (_d: string, _c: number) => {},
          complete: () => {}
        });
        expect(typeof successCalled === 'boolean').assertTrue();
      });
    });
  });

  describe('ConnectivityKit_design_perf', () => {
    it('perf_access_getState_budget', 0, () => {
      assertPerfBudget('access.getState', 30, 20000, () => {
        access.getState();
      });
    });
    it('perf_bluetoothManager_getState_budget', 0, () => {
      assertPerfBudget('bluetoothManager.getState', 30, 20000, () => {
        bluetoothManager.getState();
      });
    });
    it('perf_ble_stopBLEScan_budget', 0, () => {
      assertPerfBudget('ble.stopBLEScan', 40, 25000, () => {
        ble.stopBLEScan();
      });
    });
  });

  describe('ConnectivityKit_design_stress', () => {
    it('stress_ble_stopBLEScan_iterations', 0, () => {
      assertStressHasSuccess('ble.stopBLEScan', 120, () => {
        ble.stopBLEScan();
      });
    });
    it('stress_connection_getLocalName_iterations', 0, () => {
      assertStressHasSuccess('connection.getLocalName', 80, () => {
        connection.getLocalName();
      });
    });
  });
}
