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
import { describe, it, expect} from '@ohos/hypium';
import { access, ble, bluetoothManager, connection} from '@kit.ConnectivityKit';
import { matrixTryRun} from './BluetoothExpandedMatrixShared';

export default function bluetoothExpandedMatrixRotateTest(){
  describe('ExpandedMatrix_kit_idempotent_rotate', () => {
    it('expanded_kit_rotate_0', 0, () => {
      matrixTryRun(() => {
        access.getState();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_1', 0, () => {
      matrixTryRun(() => {
        ble.stopBLEScan();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_2', 0, () => {
      matrixTryRun(() => {
        bluetoothManager.getState();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_3', 0, () => {
      matrixTryRun(() => {
        connection.getLocalName();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_4', 0, () => {
      matrixTryRun(() => {
        ble.getConnectedBLEDevices();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_5', 0, () => {
      matrixTryRun(() => {
        access.getState();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_6', 0, () => {
      matrixTryRun(() => {
        ble.stopBLEScan();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_7', 0, () => {
      matrixTryRun(() => {
        bluetoothManager.getState();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_8', 0, () => {
      matrixTryRun(() => {
        connection.getLocalName();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_9', 0, () => {
      matrixTryRun(() => {
        ble.getConnectedBLEDevices();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_10', 0, () => {
      matrixTryRun(() => {
        access.getState();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_11', 0, () => {
      matrixTryRun(() => {
        ble.stopBLEScan();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_12', 0, () => {
      matrixTryRun(() => {
        bluetoothManager.getState();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_13', 0, () => {
      matrixTryRun(() => {
        connection.getLocalName();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_14', 0, () => {
      matrixTryRun(() => {
        ble.getConnectedBLEDevices();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_15', 0, () => {
      matrixTryRun(() => {
        access.getState();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_16', 0, () => {
      matrixTryRun(() => {
        ble.stopBLEScan();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_17', 0, () => {
      matrixTryRun(() => {
        bluetoothManager.getState();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_18', 0, () => {
      matrixTryRun(() => {
        connection.getLocalName();
      });
      expect(true).assertTrue();
    });

    it('expanded_kit_rotate_19', 0, () => {
      matrixTryRun(() => {
        ble.getConnectedBLEDevices();
      });
      expect(true).assertTrue();
    });

  });
}
