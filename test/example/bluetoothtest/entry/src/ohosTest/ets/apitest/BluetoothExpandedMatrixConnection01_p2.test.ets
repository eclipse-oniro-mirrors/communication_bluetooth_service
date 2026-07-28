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
 * ConnectivityKit getProfileConnectionState sweep 0..129.
 * Direct API calls; LogUtil + errCode/return assertions (no matrixTryRun / tautology).
 */
import { describe, beforeEach, it, expect } from '@ohos/hypium';
import { assertPeerExpect, ensureBtOn } from './ApitestRealAssert';
import LogUtil from '../../../main/ets/Utils/LogUtil';
import { BusinessError } from '@ohos.base';
import { connection, constant } from '@kit.ConnectivityKit';

export default function bluetoothExpandedMatrixConnection01Test_p2() {
  describe('ExpandedMatrix_CK_getProfileConnectionState_0_129_p2', () => {
    beforeEach(() => {
    ensureBtOn();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_68
     * @tc.name expanded_ck_getProfileConnectionState_68
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 68
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_68', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(68 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_69
     * @tc.name expanded_ck_getProfileConnectionState_69
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 69
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_69', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(69 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_70
     * @tc.name expanded_ck_getProfileConnectionState_70
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 70
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_70', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(70 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_71
     * @tc.name expanded_ck_getProfileConnectionState_71
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 71
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_71', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(71 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_72
     * @tc.name expanded_ck_getProfileConnectionState_72
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 72
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_72', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(72 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_73
     * @tc.name expanded_ck_getProfileConnectionState_73
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 73
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_73', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(73 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_74
     * @tc.name expanded_ck_getProfileConnectionState_74
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 74
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_74', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(74 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_75
     * @tc.name expanded_ck_getProfileConnectionState_75
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 75
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_75', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(75 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_76
     * @tc.name expanded_ck_getProfileConnectionState_76
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 76
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_76', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(76 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_77
     * @tc.name expanded_ck_getProfileConnectionState_77
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 77
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_77', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(77 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_78
     * @tc.name expanded_ck_getProfileConnectionState_78
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 78
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_78', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(78 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_79
     * @tc.name expanded_ck_getProfileConnectionState_79
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 79
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_79', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(79 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_80
     * @tc.name expanded_ck_getProfileConnectionState_80
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 80
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_80', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(80 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_81
     * @tc.name expanded_ck_getProfileConnectionState_81
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 81
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_81', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(81 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_82
     * @tc.name expanded_ck_getProfileConnectionState_82
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 82
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_82', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(82 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_83
     * @tc.name expanded_ck_getProfileConnectionState_83
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 83
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_83', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(83 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_84
     * @tc.name expanded_ck_getProfileConnectionState_84
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 84
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_84', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(84 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_85
     * @tc.name expanded_ck_getProfileConnectionState_85
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 85
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_85', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(85 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_86
     * @tc.name expanded_ck_getProfileConnectionState_86
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 86
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_86', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(86 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_87
     * @tc.name expanded_ck_getProfileConnectionState_87
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 87
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_87', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(87 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_88
     * @tc.name expanded_ck_getProfileConnectionState_88
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 88
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_88', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(88 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_89
     * @tc.name expanded_ck_getProfileConnectionState_89
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 89
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_89', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(89 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_90
     * @tc.name expanded_ck_getProfileConnectionState_90
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 90
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_90', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(90 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_91
     * @tc.name expanded_ck_getProfileConnectionState_91
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 91
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_91', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(91 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_92
     * @tc.name expanded_ck_getProfileConnectionState_92
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 92
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_92', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(92 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_93
     * @tc.name expanded_ck_getProfileConnectionState_93
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 93
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_93', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(93 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_94
     * @tc.name expanded_ck_getProfileConnectionState_94
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 94
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_94', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(94 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_95
     * @tc.name expanded_ck_getProfileConnectionState_95
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 95
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_95', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(95 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_96
     * @tc.name expanded_ck_getProfileConnectionState_96
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 96
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_96', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(96 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_97
     * @tc.name expanded_ck_getProfileConnectionState_97
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 97
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_97', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(97 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_98
     * @tc.name expanded_ck_getProfileConnectionState_98
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 98
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_98', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(98 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_99
     * @tc.name expanded_ck_getProfileConnectionState_99
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 99
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_99', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(99 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_100
     * @tc.name expanded_ck_getProfileConnectionState_100
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 100
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_100', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(100 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_101
     * @tc.name expanded_ck_getProfileConnectionState_101
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 101
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_101', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(101 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_102
     * @tc.name expanded_ck_getProfileConnectionState_102
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 102
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_102', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(102 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_103
     * @tc.name expanded_ck_getProfileConnectionState_103
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 103
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_103', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(103 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_104
     * @tc.name expanded_ck_getProfileConnectionState_104
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 104
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_104', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(104 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_105
     * @tc.name expanded_ck_getProfileConnectionState_105
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 105
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_105', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(105 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_106
     * @tc.name expanded_ck_getProfileConnectionState_106
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 106
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_106', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(106 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_107
     * @tc.name expanded_ck_getProfileConnectionState_107
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 107
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_107', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(107 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_108
     * @tc.name expanded_ck_getProfileConnectionState_108
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 108
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_108', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(108 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_109
     * @tc.name expanded_ck_getProfileConnectionState_109
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 109
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_109', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(109 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_110
     * @tc.name expanded_ck_getProfileConnectionState_110
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 110
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_110', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(110 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_111
     * @tc.name expanded_ck_getProfileConnectionState_111
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 111
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_111', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(111 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_112
     * @tc.name expanded_ck_getProfileConnectionState_112
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 112
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_112', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(112 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_113
     * @tc.name expanded_ck_getProfileConnectionState_113
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 113
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_113', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(113 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_114
     * @tc.name expanded_ck_getProfileConnectionState_114
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 114
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_114', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(114 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_115
     * @tc.name expanded_ck_getProfileConnectionState_115
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 115
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_115', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(115 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_116
     * @tc.name expanded_ck_getProfileConnectionState_116
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 116
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_116', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(116 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_117
     * @tc.name expanded_ck_getProfileConnectionState_117
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 117
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_117', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(117 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_118
     * @tc.name expanded_ck_getProfileConnectionState_118
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 118
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_118', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(118 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_119
     * @tc.name expanded_ck_getProfileConnectionState_119
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 119
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_119', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(119 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_120
     * @tc.name expanded_ck_getProfileConnectionState_120
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 120
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_120', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(120 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_121
     * @tc.name expanded_ck_getProfileConnectionState_121
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 121
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_121', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(121 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_122
     * @tc.name expanded_ck_getProfileConnectionState_122
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 122
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_122', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(122 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_123
     * @tc.name expanded_ck_getProfileConnectionState_123
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 123
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_123', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(123 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_124
     * @tc.name expanded_ck_getProfileConnectionState_124
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 124
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_124', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(124 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_125
     * @tc.name expanded_ck_getProfileConnectionState_125
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 125
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_125', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(125 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_126
     * @tc.name expanded_ck_getProfileConnectionState_126
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 126
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_126', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(126 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_127
     * @tc.name expanded_ck_getProfileConnectionState_127
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 127
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_127', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(127 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_128
     * @tc.name expanded_ck_getProfileConnectionState_128
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 128
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_128', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(128 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_129
     * @tc.name expanded_ck_getProfileConnectionState_129
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 129
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_129', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(129 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
          errCode = 0;
        }
        LogUtil.error(JSON.stringify(err));
      }
      LogUtil.info('errCode=' + errCode);
      LogUtil.info('value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });
  });
}
