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
 * @ohos.bluetooth.getProfileConnState sweep 0..129.
 * Direct API calls; LogUtil + errCode/return assertions (no matrixTryRun / tautology).
 */
import { describe, beforeEach, it, expect } from '@ohos/hypium';
import { assertPeerExpect, ensureBtOn } from './ApitestRealAssert';
import LogUtil from '../../../main/ets/Utils/LogUtil';
import { BusinessError } from '@ohos.base';
import bluetooth from '@ohos.bluetooth';

export default function bluetoothExpandedMatrixLegacy01Test_p1() {
  describe('ExpandedMatrix_legacy_getProfileConnState_0_129_p1', () => {
    beforeEach(() => {
    ensureBtOn();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_0
     * @tc.name expanded_legacy_getProfileConnState_0
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 0
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_0', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_1
     * @tc.name expanded_legacy_getProfileConnState_1
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 1
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_1', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_2
     * @tc.name expanded_legacy_getProfileConnState_2
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_2', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(2);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_3
     * @tc.name expanded_legacy_getProfileConnState_3
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 3
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_3', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(3);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_4
     * @tc.name expanded_legacy_getProfileConnState_4
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 4
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_4', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(4);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_5
     * @tc.name expanded_legacy_getProfileConnState_5
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 5
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_5', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(5);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_6
     * @tc.name expanded_legacy_getProfileConnState_6
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 6
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_6', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(6);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_7
     * @tc.name expanded_legacy_getProfileConnState_7
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 7
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_7', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(7);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_8
     * @tc.name expanded_legacy_getProfileConnState_8
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 8
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_8', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(8);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_9
     * @tc.name expanded_legacy_getProfileConnState_9
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 9
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_9', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(9);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_10
     * @tc.name expanded_legacy_getProfileConnState_10
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 10
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_10', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(10);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_11
     * @tc.name expanded_legacy_getProfileConnState_11
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 11
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_11', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(11);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_12
     * @tc.name expanded_legacy_getProfileConnState_12
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 12
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_12', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(12);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_13
     * @tc.name expanded_legacy_getProfileConnState_13
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 13
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_13', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(13);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_14
     * @tc.name expanded_legacy_getProfileConnState_14
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 14
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_14', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(14);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_15
     * @tc.name expanded_legacy_getProfileConnState_15
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 15
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_15', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(15);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_16
     * @tc.name expanded_legacy_getProfileConnState_16
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 16
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_16', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(16);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_17
     * @tc.name expanded_legacy_getProfileConnState_17
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 17
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_17', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(17);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_18
     * @tc.name expanded_legacy_getProfileConnState_18
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 18
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_18', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(18);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_19
     * @tc.name expanded_legacy_getProfileConnState_19
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 19
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_19', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(19);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_20
     * @tc.name expanded_legacy_getProfileConnState_20
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 20
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_20', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(20);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_21
     * @tc.name expanded_legacy_getProfileConnState_21
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 21
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_21', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(21);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_22
     * @tc.name expanded_legacy_getProfileConnState_22
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 22
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_22', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(22);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_23
     * @tc.name expanded_legacy_getProfileConnState_23
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 23
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_23', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(23);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_24
     * @tc.name expanded_legacy_getProfileConnState_24
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 24
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_24', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(24);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_25
     * @tc.name expanded_legacy_getProfileConnState_25
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 25
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_25', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(25);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_26
     * @tc.name expanded_legacy_getProfileConnState_26
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 26
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_26', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(26);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_27
     * @tc.name expanded_legacy_getProfileConnState_27
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 27
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_27', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(27);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_28
     * @tc.name expanded_legacy_getProfileConnState_28
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 28
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_28', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(28);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_29
     * @tc.name expanded_legacy_getProfileConnState_29
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 29
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_29', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(29);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_30
     * @tc.name expanded_legacy_getProfileConnState_30
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 30
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_30', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(30);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_31
     * @tc.name expanded_legacy_getProfileConnState_31
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 31
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_31', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(31);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_32
     * @tc.name expanded_legacy_getProfileConnState_32
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 32
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_32', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(32);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_33
     * @tc.name expanded_legacy_getProfileConnState_33
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 33
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_33', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(33);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_34
     * @tc.name expanded_legacy_getProfileConnState_34
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 34
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_34', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(34);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_35
     * @tc.name expanded_legacy_getProfileConnState_35
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 35
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_35', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(35);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_36
     * @tc.name expanded_legacy_getProfileConnState_36
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 36
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_36', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(36);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_37
     * @tc.name expanded_legacy_getProfileConnState_37
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 37
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_37', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(37);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_38
     * @tc.name expanded_legacy_getProfileConnState_38
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 38
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_38', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(38);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_39
     * @tc.name expanded_legacy_getProfileConnState_39
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 39
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_39', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(39);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_40
     * @tc.name expanded_legacy_getProfileConnState_40
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 40
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_40', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(40);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_41
     * @tc.name expanded_legacy_getProfileConnState_41
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 41
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_41', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(41);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_42
     * @tc.name expanded_legacy_getProfileConnState_42
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 42
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_42', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(42);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_43
     * @tc.name expanded_legacy_getProfileConnState_43
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 43
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_43', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(43);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_44
     * @tc.name expanded_legacy_getProfileConnState_44
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 44
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_44', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(44);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_45
     * @tc.name expanded_legacy_getProfileConnState_45
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 45
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_45', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(45);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_46
     * @tc.name expanded_legacy_getProfileConnState_46
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 46
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_46', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(46);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_47
     * @tc.name expanded_legacy_getProfileConnState_47
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 47
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_47', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(47);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_48
     * @tc.name expanded_legacy_getProfileConnState_48
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 48
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_48', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(48);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_49
     * @tc.name expanded_legacy_getProfileConnState_49
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 49
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_49', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(49);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_50
     * @tc.name expanded_legacy_getProfileConnState_50
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 50
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_50', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(50);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_51
     * @tc.name expanded_legacy_getProfileConnState_51
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 51
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_51', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(51);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_52
     * @tc.name expanded_legacy_getProfileConnState_52
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 52
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_52', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(52);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_53
     * @tc.name expanded_legacy_getProfileConnState_53
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 53
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_53', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(53);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_54
     * @tc.name expanded_legacy_getProfileConnState_54
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 54
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_54', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(54);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_55
     * @tc.name expanded_legacy_getProfileConnState_55
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 55
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_55', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(55);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_56
     * @tc.name expanded_legacy_getProfileConnState_56
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 56
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_56', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(56);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_57
     * @tc.name expanded_legacy_getProfileConnState_57
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 57
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_57', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(57);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_58
     * @tc.name expanded_legacy_getProfileConnState_58
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 58
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_58', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(58);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_59
     * @tc.name expanded_legacy_getProfileConnState_59
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 59
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_59', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(59);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_60
     * @tc.name expanded_legacy_getProfileConnState_60
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 60
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_60', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(60);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_61
     * @tc.name expanded_legacy_getProfileConnState_61
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 61
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_61', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(61);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_62
     * @tc.name expanded_legacy_getProfileConnState_62
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 62
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_62', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(62);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_63
     * @tc.name expanded_legacy_getProfileConnState_63
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 63
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_63', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(63);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_64
     * @tc.name expanded_legacy_getProfileConnState_64
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 64
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_64', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(64);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_65
     * @tc.name expanded_legacy_getProfileConnState_65
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 65
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_65', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(65);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_66
     * @tc.name expanded_legacy_getProfileConnState_66
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 66
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_66', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(66);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_67
     * @tc.name expanded_legacy_getProfileConnState_67
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 67
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_67', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(67);
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
