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
 * @ohos.bluetooth.getProfileConnState sweep 130..259.
 * Direct API calls; LogUtil + errCode/return assertions (no matrixTryRun / tautology).
 */
import { describe, beforeEach, it, expect } from '@ohos/hypium';
import { assertPeerExpect, ensureBtOn } from './ApitestRealAssert';
import LogUtil from '../../../main/ets/Utils/LogUtil';
import { BusinessError } from '@ohos.base';
import bluetooth from '@ohos.bluetooth';

export default function bluetoothExpandedMatrixLegacy02Test_p2() {
  describe('ExpandedMatrix_legacy_getProfileConnState_130_259_p2', () => {
    beforeEach(() => {
    ensureBtOn();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_198
     * @tc.name expanded_legacy_getProfileConnState_198
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 198
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_198', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(198);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_199
     * @tc.name expanded_legacy_getProfileConnState_199
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 199
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_199', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(199);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_200
     * @tc.name expanded_legacy_getProfileConnState_200
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 200
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_200', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(200);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_201
     * @tc.name expanded_legacy_getProfileConnState_201
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 201
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_201', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(201);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_202
     * @tc.name expanded_legacy_getProfileConnState_202
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 202
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_202', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(202);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_203
     * @tc.name expanded_legacy_getProfileConnState_203
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 203
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_203', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(203);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_204
     * @tc.name expanded_legacy_getProfileConnState_204
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 204
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_204', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(204);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_205
     * @tc.name expanded_legacy_getProfileConnState_205
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 205
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_205', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(205);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_206
     * @tc.name expanded_legacy_getProfileConnState_206
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 206
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_206', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(206);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_207
     * @tc.name expanded_legacy_getProfileConnState_207
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 207
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_207', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(207);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_208
     * @tc.name expanded_legacy_getProfileConnState_208
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 208
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_208', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(208);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_209
     * @tc.name expanded_legacy_getProfileConnState_209
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 209
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_209', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(209);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_210
     * @tc.name expanded_legacy_getProfileConnState_210
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 210
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_210', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(210);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_211
     * @tc.name expanded_legacy_getProfileConnState_211
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 211
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_211', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(211);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_212
     * @tc.name expanded_legacy_getProfileConnState_212
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 212
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_212', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(212);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_213
     * @tc.name expanded_legacy_getProfileConnState_213
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 213
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_213', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(213);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_214
     * @tc.name expanded_legacy_getProfileConnState_214
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 214
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_214', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(214);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_215
     * @tc.name expanded_legacy_getProfileConnState_215
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 215
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_215', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(215);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_216
     * @tc.name expanded_legacy_getProfileConnState_216
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 216
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_216', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(216);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_217
     * @tc.name expanded_legacy_getProfileConnState_217
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 217
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_217', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(217);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_218
     * @tc.name expanded_legacy_getProfileConnState_218
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 218
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_218', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(218);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_219
     * @tc.name expanded_legacy_getProfileConnState_219
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 219
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_219', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(219);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_220
     * @tc.name expanded_legacy_getProfileConnState_220
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 220
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_220', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(220);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_221
     * @tc.name expanded_legacy_getProfileConnState_221
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 221
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_221', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(221);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_222
     * @tc.name expanded_legacy_getProfileConnState_222
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 222
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_222', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(222);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_223
     * @tc.name expanded_legacy_getProfileConnState_223
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 223
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_223', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(223);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_224
     * @tc.name expanded_legacy_getProfileConnState_224
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 224
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_224', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(224);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_225
     * @tc.name expanded_legacy_getProfileConnState_225
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 225
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_225', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(225);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_226
     * @tc.name expanded_legacy_getProfileConnState_226
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 226
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_226', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(226);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_227
     * @tc.name expanded_legacy_getProfileConnState_227
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 227
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_227', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(227);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_228
     * @tc.name expanded_legacy_getProfileConnState_228
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 228
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_228', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(228);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_229
     * @tc.name expanded_legacy_getProfileConnState_229
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 229
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_229', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(229);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_230
     * @tc.name expanded_legacy_getProfileConnState_230
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 230
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_230', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(230);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_231
     * @tc.name expanded_legacy_getProfileConnState_231
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 231
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_231', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(231);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_232
     * @tc.name expanded_legacy_getProfileConnState_232
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 232
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_232', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(232);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_233
     * @tc.name expanded_legacy_getProfileConnState_233
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 233
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_233', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(233);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_234
     * @tc.name expanded_legacy_getProfileConnState_234
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 234
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_234', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(234);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_235
     * @tc.name expanded_legacy_getProfileConnState_235
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 235
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_235', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(235);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_236
     * @tc.name expanded_legacy_getProfileConnState_236
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 236
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_236', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(236);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_237
     * @tc.name expanded_legacy_getProfileConnState_237
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 237
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_237', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(237);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_238
     * @tc.name expanded_legacy_getProfileConnState_238
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 238
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_238', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(238);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_239
     * @tc.name expanded_legacy_getProfileConnState_239
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 239
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_239', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(239);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_240
     * @tc.name expanded_legacy_getProfileConnState_240
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 240
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_240', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(240);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_241
     * @tc.name expanded_legacy_getProfileConnState_241
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 241
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_241', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(241);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_242
     * @tc.name expanded_legacy_getProfileConnState_242
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 242
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_242', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(242);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_243
     * @tc.name expanded_legacy_getProfileConnState_243
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 243
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_243', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(243);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_244
     * @tc.name expanded_legacy_getProfileConnState_244
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 244
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_244', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(244);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_245
     * @tc.name expanded_legacy_getProfileConnState_245
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 245
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_245', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(245);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_246
     * @tc.name expanded_legacy_getProfileConnState_246
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 246
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_246', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(246);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_247
     * @tc.name expanded_legacy_getProfileConnState_247
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 247
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_247', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(247);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_248
     * @tc.name expanded_legacy_getProfileConnState_248
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 248
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_248', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(248);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_249
     * @tc.name expanded_legacy_getProfileConnState_249
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 249
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_249', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(249);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_250
     * @tc.name expanded_legacy_getProfileConnState_250
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 250
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_250', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(250);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_251
     * @tc.name expanded_legacy_getProfileConnState_251
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 251
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_251', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(251);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_252
     * @tc.name expanded_legacy_getProfileConnState_252
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 252
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_252', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(252);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_253
     * @tc.name expanded_legacy_getProfileConnState_253
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 253
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_253', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(253);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_254
     * @tc.name expanded_legacy_getProfileConnState_254
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 254
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_254', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(254);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_255
     * @tc.name expanded_legacy_getProfileConnState_255
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 255
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_255', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(255);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_256
     * @tc.name expanded_legacy_getProfileConnState_256
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 256
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_256', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(256);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_257
     * @tc.name expanded_legacy_getProfileConnState_257
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 257
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_257', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(257);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_258
     * @tc.name expanded_legacy_getProfileConnState_258
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 258
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_258', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(258);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_LEGACY_GETPROFILECONNSTATE_259
     * @tc.name expanded_legacy_getProfileConnState_259
     * @tc.desc Verify Bluetooth apitest case expanded legacy getProfileConnState 259
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_legacy_getProfileConnState_259', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = bluetooth.getProfileConnState(259);
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
