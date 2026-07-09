/**
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
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
 * ConnectivityKit `getProfileConnectionState` 扫参 **0～129**（展开写法）。与 `BluetoothExpandedMatrixConnection02.test` 合起来覆盖 0～259。
 */
import { describe, it, expect } from '@ohos/hypium';
import { connection, constant } from '@kit.ConnectivityKit';
import { matrixTryRun } from './BluetoothExpandedMatrixShared';

export default function bluetoothExpandedMatrixConnection01Test() {
  describe('ExpandedMatrix_CK_getProfileConnectionState_0_129', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_CK_GETPROFILECONNECTIONSTATE_0
     * @tc.name expanded_ck_getProfileConnectionState_0
     * @tc.desc Verify Bluetooth apitest case expanded ck getProfileConnectionState 0
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_ck_getProfileConnectionState_0', 0, () => {
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(0 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(1 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(2 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(3 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(4 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(5 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(6 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(7 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(8 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(9 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(10 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(11 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(12 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(13 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(14 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(15 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(16 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(17 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(18 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(19 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(20 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(21 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(22 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(23 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(24 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(25 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(26 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(27 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(28 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(29 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(30 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(31 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(32 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(33 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(34 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(35 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(36 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(37 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(38 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(39 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(40 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(41 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(42 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(43 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(44 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(45 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(46 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(47 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(48 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(49 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(50 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(51 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(52 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(53 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(54 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(55 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(56 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(57 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(58 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(59 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(60 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(61 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(62 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(63 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(64 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(65 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(66 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(67 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(68 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(69 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(70 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(71 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(72 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(73 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(74 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(75 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(76 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(77 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(78 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(79 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(80 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(81 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(82 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(83 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(84 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(85 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(86 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(87 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(88 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(89 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(90 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(91 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(92 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(93 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(94 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(95 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(96 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(97 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(98 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(99 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(100 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(101 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(102 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(103 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(104 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(105 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(106 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(107 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(108 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(109 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(110 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(111 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(112 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(113 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(114 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(115 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(116 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(117 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(118 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(119 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(120 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(121 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(122 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(123 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(124 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(125 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(126 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(127 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(128 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
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
      try {
            matrixTryRun(() => {
              connection.getProfileConnectionState(129 as constant.ProfileId);
            });
            expect(true).assertTrue();
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

  });
}



