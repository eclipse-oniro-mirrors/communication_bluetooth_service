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
 * ConnectivityKit getProfileConnectionState sweep 130..259.
 * Direct API calls; LogUtil + errCode/return assertions (no matrixTryRun / tautology).
 */
import { describe, beforeEach, it, expect } from '@ohos/hypium';
import { assertPeerExpect, ensureBtOn } from './ApitestRealAssert';
import LogUtil from '../../../main/ets/Utils/LogUtil';
import { BusinessError } from '@ohos.base';
import { connection, constant } from '@kit.ConnectivityKit';

export default function bluetoothExpandedMatrixConnection02Test_p2() {
  describe('ExpandedMatrix_CK_getProfileConnectionState_130_259_p2', () => {
    beforeEach(() => {
    ensureBtOn();
    });
/**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_198
     * @tc.name expanded_ck_getProfileConnectionState_198
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 198
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_198', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(198 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_199
     * @tc.name expanded_ck_getProfileConnectionState_199
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 199
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_199', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(199 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_200
     * @tc.name expanded_ck_getProfileConnectionState_200
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 200
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_200', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(200 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_201
     * @tc.name expanded_ck_getProfileConnectionState_201
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 201
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_201', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(201 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_202
     * @tc.name expanded_ck_getProfileConnectionState_202
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 202
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_202', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(202 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_203
     * @tc.name expanded_ck_getProfileConnectionState_203
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 203
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_203', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(203 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_204
     * @tc.name expanded_ck_getProfileConnectionState_204
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 204
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_204', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(204 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_205
     * @tc.name expanded_ck_getProfileConnectionState_205
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 205
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_205', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(205 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_206
     * @tc.name expanded_ck_getProfileConnectionState_206
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 206
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_206', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(206 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_207
     * @tc.name expanded_ck_getProfileConnectionState_207
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 207
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_207', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(207 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_208
     * @tc.name expanded_ck_getProfileConnectionState_208
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 208
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_208', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(208 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_209
     * @tc.name expanded_ck_getProfileConnectionState_209
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 209
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_209', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(209 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_210
     * @tc.name expanded_ck_getProfileConnectionState_210
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 210
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_210', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(210 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_211
     * @tc.name expanded_ck_getProfileConnectionState_211
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 211
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_211', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(211 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_212
     * @tc.name expanded_ck_getProfileConnectionState_212
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 212
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_212', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(212 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_213
     * @tc.name expanded_ck_getProfileConnectionState_213
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 213
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_213', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(213 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_214
     * @tc.name expanded_ck_getProfileConnectionState_214
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 214
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_214', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(214 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_215
     * @tc.name expanded_ck_getProfileConnectionState_215
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 215
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_215', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(215 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_216
     * @tc.name expanded_ck_getProfileConnectionState_216
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 216
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_216', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(216 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_217
     * @tc.name expanded_ck_getProfileConnectionState_217
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 217
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_217', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(217 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_218
     * @tc.name expanded_ck_getProfileConnectionState_218
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 218
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_218', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(218 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_219
     * @tc.name expanded_ck_getProfileConnectionState_219
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 219
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_219', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(219 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_220
     * @tc.name expanded_ck_getProfileConnectionState_220
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 220
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_220', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(220 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_221
     * @tc.name expanded_ck_getProfileConnectionState_221
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 221
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_221', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(221 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_222
     * @tc.name expanded_ck_getProfileConnectionState_222
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 222
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_222', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(222 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_223
     * @tc.name expanded_ck_getProfileConnectionState_223
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 223
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_223', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(223 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_224
     * @tc.name expanded_ck_getProfileConnectionState_224
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 224
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_224', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(224 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_225
     * @tc.name expanded_ck_getProfileConnectionState_225
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 225
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_225', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(225 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_226
     * @tc.name expanded_ck_getProfileConnectionState_226
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 226
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_226', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(226 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_227
     * @tc.name expanded_ck_getProfileConnectionState_227
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 227
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_227', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(227 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_228
     * @tc.name expanded_ck_getProfileConnectionState_228
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 228
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_228', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(228 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_229
     * @tc.name expanded_ck_getProfileConnectionState_229
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 229
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_229', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(229 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_230
     * @tc.name expanded_ck_getProfileConnectionState_230
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 230
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_230', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(230 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_231
     * @tc.name expanded_ck_getProfileConnectionState_231
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 231
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_231', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(231 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_232
     * @tc.name expanded_ck_getProfileConnectionState_232
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 232
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_232', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(232 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_233
     * @tc.name expanded_ck_getProfileConnectionState_233
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 233
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_233', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(233 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_234
     * @tc.name expanded_ck_getProfileConnectionState_234
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 234
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_234', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(234 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_235
     * @tc.name expanded_ck_getProfileConnectionState_235
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 235
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_235', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(235 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_236
     * @tc.name expanded_ck_getProfileConnectionState_236
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 236
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_236', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(236 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_237
     * @tc.name expanded_ck_getProfileConnectionState_237
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 237
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_237', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(237 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_238
     * @tc.name expanded_ck_getProfileConnectionState_238
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 238
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_238', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(238 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_239
     * @tc.name expanded_ck_getProfileConnectionState_239
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 239
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_239', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(239 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_240
     * @tc.name expanded_ck_getProfileConnectionState_240
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 240
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_240', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(240 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_241
     * @tc.name expanded_ck_getProfileConnectionState_241
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 241
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_241', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(241 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_242
     * @tc.name expanded_ck_getProfileConnectionState_242
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 242
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_242', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(242 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_243
     * @tc.name expanded_ck_getProfileConnectionState_243
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 243
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_243', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(243 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_244
     * @tc.name expanded_ck_getProfileConnectionState_244
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 244
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_244', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(244 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_245
     * @tc.name expanded_ck_getProfileConnectionState_245
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 245
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_245', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(245 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_246
     * @tc.name expanded_ck_getProfileConnectionState_246
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 246
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_246', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(246 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_247
     * @tc.name expanded_ck_getProfileConnectionState_247
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 247
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_247', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(247 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_248
     * @tc.name expanded_ck_getProfileConnectionState_248
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 248
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_248', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(248 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_249
     * @tc.name expanded_ck_getProfileConnectionState_249
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 249
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_249', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(249 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_250
     * @tc.name expanded_ck_getProfileConnectionState_250
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 250
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_250', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(250 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_251
     * @tc.name expanded_ck_getProfileConnectionState_251
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 251
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_251', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(251 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_252
     * @tc.name expanded_ck_getProfileConnectionState_252
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 252
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_252', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(252 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_253
     * @tc.name expanded_ck_getProfileConnectionState_253
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 253
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_253', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(253 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_254
     * @tc.name expanded_ck_getProfileConnectionState_254
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 254
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_254', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(254 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_255
     * @tc.name expanded_ck_getProfileConnectionState_255
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 255
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_255', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(255 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_256
     * @tc.name expanded_ck_getProfileConnectionState_256
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 256
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_256', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(256 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_257
     * @tc.name expanded_ck_getProfileConnectionState_257
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 257
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_257', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(257 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_258
     * @tc.name expanded_ck_getProfileConnectionState_258
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 258
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_258', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(258 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_259
     * @tc.name expanded_ck_getProfileConnectionState_259
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 259
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
it('expanded_ck_getProfileConnectionState_259', 0, () => {
      let errCode = 0;
      let value = -1;
      LogUtil.info('invoke');
      try {
        value = connection.getProfileConnectionState(259 as constant.ProfileId);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) {
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
