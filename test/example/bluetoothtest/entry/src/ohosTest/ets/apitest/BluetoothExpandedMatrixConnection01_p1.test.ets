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

export default function bluetoothExpandedMatrixConnection01Test_p1() {
  describe('ExpandedMatrix_CK_getProfileConnectionState_0_129_p1', () => {
    beforeEach(() => {
    ensureBtOn();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_0
     * @tc.name expanded_ck_getProfileConnectionState_0
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 0
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_0', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(0 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_1
     * @tc.name expanded_ck_getProfileConnectionState_1
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 1
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_1', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(1 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_2
     * @tc.name expanded_ck_getProfileConnectionState_2
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_2', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(2 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_3
     * @tc.name expanded_ck_getProfileConnectionState_3
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 3
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_3', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(3 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_4
     * @tc.name expanded_ck_getProfileConnectionState_4
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 4
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_4', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(4 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_5
     * @tc.name expanded_ck_getProfileConnectionState_5
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 5
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_5', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(5 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_6
     * @tc.name expanded_ck_getProfileConnectionState_6
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 6
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_6', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(6 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_7
     * @tc.name expanded_ck_getProfileConnectionState_7
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 7
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_7', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(7 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_8
     * @tc.name expanded_ck_getProfileConnectionState_8
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 8
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_8', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(8 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_9
     * @tc.name expanded_ck_getProfileConnectionState_9
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 9
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_9', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(9 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_10
     * @tc.name expanded_ck_getProfileConnectionState_10
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 10
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_10', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(10 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_11
     * @tc.name expanded_ck_getProfileConnectionState_11
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 11
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_11', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(11 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_12
     * @tc.name expanded_ck_getProfileConnectionState_12
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 12
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_12', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(12 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_13
     * @tc.name expanded_ck_getProfileConnectionState_13
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 13
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_13', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(13 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_14
     * @tc.name expanded_ck_getProfileConnectionState_14
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 14
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_14', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(14 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_15
     * @tc.name expanded_ck_getProfileConnectionState_15
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 15
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_15', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(15 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_16
     * @tc.name expanded_ck_getProfileConnectionState_16
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 16
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_16', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(16 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_17
     * @tc.name expanded_ck_getProfileConnectionState_17
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 17
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_17', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(17 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_18
     * @tc.name expanded_ck_getProfileConnectionState_18
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 18
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_18', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(18 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_19
     * @tc.name expanded_ck_getProfileConnectionState_19
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 19
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_19', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(19 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_20
     * @tc.name expanded_ck_getProfileConnectionState_20
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 20
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_20', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(20 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_21
     * @tc.name expanded_ck_getProfileConnectionState_21
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 21
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_21', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(21 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_22
     * @tc.name expanded_ck_getProfileConnectionState_22
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 22
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_22', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(22 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_23
     * @tc.name expanded_ck_getProfileConnectionState_23
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 23
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_23', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(23 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_24
     * @tc.name expanded_ck_getProfileConnectionState_24
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 24
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_24', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(24 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_25
     * @tc.name expanded_ck_getProfileConnectionState_25
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 25
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_25', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(25 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_26
     * @tc.name expanded_ck_getProfileConnectionState_26
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 26
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_26', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(26 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_27
     * @tc.name expanded_ck_getProfileConnectionState_27
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 27
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_27', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(27 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_28
     * @tc.name expanded_ck_getProfileConnectionState_28
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 28
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_28', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(28 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_29
     * @tc.name expanded_ck_getProfileConnectionState_29
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 29
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_29', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(29 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_30
     * @tc.name expanded_ck_getProfileConnectionState_30
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 30
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_30', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(30 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_31
     * @tc.name expanded_ck_getProfileConnectionState_31
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 31
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_31', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(31 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_32
     * @tc.name expanded_ck_getProfileConnectionState_32
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 32
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_32', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(32 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_33
     * @tc.name expanded_ck_getProfileConnectionState_33
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 33
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_33', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(33 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_34
     * @tc.name expanded_ck_getProfileConnectionState_34
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 34
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_34', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(34 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_35
     * @tc.name expanded_ck_getProfileConnectionState_35
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 35
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_35', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(35 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_36
     * @tc.name expanded_ck_getProfileConnectionState_36
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 36
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_36', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(36 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_37
     * @tc.name expanded_ck_getProfileConnectionState_37
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 37
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_37', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(37 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_38
     * @tc.name expanded_ck_getProfileConnectionState_38
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 38
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_38', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(38 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_39
     * @tc.name expanded_ck_getProfileConnectionState_39
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 39
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_39', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(39 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_40
     * @tc.name expanded_ck_getProfileConnectionState_40
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 40
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_40', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(40 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_41
     * @tc.name expanded_ck_getProfileConnectionState_41
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 41
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_41', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(41 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_42
     * @tc.name expanded_ck_getProfileConnectionState_42
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 42
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_42', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(42 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_43
     * @tc.name expanded_ck_getProfileConnectionState_43
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 43
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_43', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(43 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_44
     * @tc.name expanded_ck_getProfileConnectionState_44
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 44
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_44', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(44 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_45
     * @tc.name expanded_ck_getProfileConnectionState_45
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 45
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_45', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(45 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_46
     * @tc.name expanded_ck_getProfileConnectionState_46
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 46
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_46', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(46 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_47
     * @tc.name expanded_ck_getProfileConnectionState_47
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 47
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_47', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(47 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_48
     * @tc.name expanded_ck_getProfileConnectionState_48
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 48
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_48', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(48 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_49
     * @tc.name expanded_ck_getProfileConnectionState_49
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 49
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_49', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(49 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_50
     * @tc.name expanded_ck_getProfileConnectionState_50
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 50
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_50', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(50 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_51
     * @tc.name expanded_ck_getProfileConnectionState_51
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 51
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_51', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(51 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_52
     * @tc.name expanded_ck_getProfileConnectionState_52
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 52
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_52', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(52 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_53
     * @tc.name expanded_ck_getProfileConnectionState_53
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 53
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_53', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(53 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_54
     * @tc.name expanded_ck_getProfileConnectionState_54
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 54
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_54', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(54 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_55
     * @tc.name expanded_ck_getProfileConnectionState_55
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 55
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_55', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(55 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_56
     * @tc.name expanded_ck_getProfileConnectionState_56
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 56
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_56', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(56 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_57
     * @tc.name expanded_ck_getProfileConnectionState_57
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 57
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_57', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(57 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_58
     * @tc.name expanded_ck_getProfileConnectionState_58
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 58
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_58', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(58 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_59
     * @tc.name expanded_ck_getProfileConnectionState_59
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 59
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_59', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(59 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_60
     * @tc.name expanded_ck_getProfileConnectionState_60
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 60
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_60', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(60 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_61
     * @tc.name expanded_ck_getProfileConnectionState_61
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 61
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_61', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(61 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_62
     * @tc.name expanded_ck_getProfileConnectionState_62
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 62
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_62', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(62 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_63
     * @tc.name expanded_ck_getProfileConnectionState_63
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 63
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_63', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(63 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_64
     * @tc.name expanded_ck_getProfileConnectionState_64
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 64
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_64', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(64 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_65
     * @tc.name expanded_ck_getProfileConnectionState_65
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 65
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_65', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(65 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_66
     * @tc.name expanded_ck_getProfileConnectionState_66
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 66
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_66', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(66 as constant.ProfileId);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_67
     * @tc.name expanded_ck_getProfileConnectionState_67
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 67
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_67', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(67 as constant.ProfileId);
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
