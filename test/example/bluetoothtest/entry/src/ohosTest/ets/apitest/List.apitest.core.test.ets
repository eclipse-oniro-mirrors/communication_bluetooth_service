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
 * Core real-coverage pack (1A+2B): access / connection / ble / socket / legacy.
 * Prefer running as segmented aa test to avoid THREAD_BLOCK.
 */
import bluetoothFullCoverageAccess01 from './BluetoothFullCoverageAccess01.test'
import bluetoothFullCoverageLegacy01 from './BluetoothFullCoverageLegacy01.test'
import bluetoothFullCoverageLegacy02 from './BluetoothFullCoverageLegacy02.test'
import bluetoothFullCoverageGatt01 from './BluetoothFullCoverageGatt01.test'
import bluetoothFullCoverageGatt02 from './BluetoothFullCoverageGatt02.test'
import bluetoothFullCoverageConnection01 from './BluetoothFullCoverageConnection01.test'
import bluetoothFullCoverageConnection02 from './BluetoothFullCoverageConnection02.test'
import bluetoothFullCoverageSocket01 from './BluetoothFullCoverageSocket01.test'
import connBleSocketMass01 from './ConnBleSocketMass01.test'
import connBleSocketMass02 from './ConnBleSocketMass02.test'
import connBleSocketMass03 from './ConnBleSocketMass03.test'
import connBleSocketMass04 from './ConnBleSocketMass04.test'

export default function testsuiteApitestCore(): void {
  bluetoothFullCoverageAccess01()
  bluetoothFullCoverageLegacy01()
  bluetoothFullCoverageLegacy02()
  bluetoothFullCoverageGatt01()
  bluetoothFullCoverageGatt02()
  bluetoothFullCoverageConnection01()
  bluetoothFullCoverageConnection02()
  bluetoothFullCoverageSocket01()
  connBleSocketMass01()
  connBleSocketMass02()
  connBleSocketMass03()
  connBleSocketMass04()
}
