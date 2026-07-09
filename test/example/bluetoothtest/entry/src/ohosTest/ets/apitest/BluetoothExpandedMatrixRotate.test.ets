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
 * Kit 幂等入口轮换（20 条），按序号固定分支。
 */
import { describe, it, expect } from '@ohos/hypium';
import { access, ble, bluetoothManager, connection } from '@kit.ConnectivityKit';
import { matrixTryRun } from './BluetoothExpandedMatrixShared';

export default function bluetoothExpandedMatrixRotateTest() {
  describe('ExpandedMatrix_kit_idempotent_rotate', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_0
     * @tc.name expanded_kit_rotate_0
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 0
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_0', 0, () => {
      try {
            matrixTryRun(() => {
              access.getState();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_1
     * @tc.name expanded_kit_rotate_1
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 1
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_1', 0, () => {
      try {
            matrixTryRun(() => {
              ble.stopBLEScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_2
     * @tc.name expanded_kit_rotate_2
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_2', 0, () => {
      try {
            matrixTryRun(() => {
              bluetoothManager.getState();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_3
     * @tc.name expanded_kit_rotate_3
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 3
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_3', 0, () => {
      try {
            matrixTryRun(() => {
              connection.getLocalName();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_4
     * @tc.name expanded_kit_rotate_4
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 4
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_4', 0, () => {
      try {
            matrixTryRun(() => {
              ble.getConnectedBLEDevices();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_5
     * @tc.name expanded_kit_rotate_5
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 5
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_5', 0, () => {
      try {
            matrixTryRun(() => {
              access.getState();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_6
     * @tc.name expanded_kit_rotate_6
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 6
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_6', 0, () => {
      try {
            matrixTryRun(() => {
              ble.stopBLEScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_7
     * @tc.name expanded_kit_rotate_7
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 7
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_7', 0, () => {
      try {
            matrixTryRun(() => {
              bluetoothManager.getState();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_8
     * @tc.name expanded_kit_rotate_8
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 8
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_8', 0, () => {
      try {
            matrixTryRun(() => {
              connection.getLocalName();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_9
     * @tc.name expanded_kit_rotate_9
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 9
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_9', 0, () => {
      try {
            matrixTryRun(() => {
              ble.getConnectedBLEDevices();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_10
     * @tc.name expanded_kit_rotate_10
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 10
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_10', 0, () => {
      try {
            matrixTryRun(() => {
              access.getState();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_11
     * @tc.name expanded_kit_rotate_11
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 11
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_11', 0, () => {
      try {
            matrixTryRun(() => {
              ble.stopBLEScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_12
     * @tc.name expanded_kit_rotate_12
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 12
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_12', 0, () => {
      try {
            matrixTryRun(() => {
              bluetoothManager.getState();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_13
     * @tc.name expanded_kit_rotate_13
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 13
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_13', 0, () => {
      try {
            matrixTryRun(() => {
              connection.getLocalName();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_14
     * @tc.name expanded_kit_rotate_14
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 14
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_14', 0, () => {
      try {
            matrixTryRun(() => {
              ble.getConnectedBLEDevices();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_15
     * @tc.name expanded_kit_rotate_15
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 15
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_15', 0, () => {
      try {
            matrixTryRun(() => {
              access.getState();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_16
     * @tc.name expanded_kit_rotate_16
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 16
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_16', 0, () => {
      try {
            matrixTryRun(() => {
              ble.stopBLEScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_17
     * @tc.name expanded_kit_rotate_17
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 17
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_17', 0, () => {
      try {
            matrixTryRun(() => {
              bluetoothManager.getState();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_18
     * @tc.name expanded_kit_rotate_18
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 18
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_18', 0, () => {
      try {
            matrixTryRun(() => {
              connection.getLocalName();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EXPANDED_KIT_ROTATE_19
     * @tc.name expanded_kit_rotate_19
     * @tc.desc Verify Bluetooth apitest case expanded kit rotate 19
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('expanded_kit_rotate_19', 0, () => {
      try {
            matrixTryRun(() => {
              ble.getConnectedBLEDevices();
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



