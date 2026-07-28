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

/** apitest 分段1a：Ability ~ ExpandedMatrix（单进程防 THREAD_BLOCK） */
import abilityTest from './Ability.test'
import pageRoutesTest from './PageRoutes.test'
import bluetoothApiCoverageTest from './BluetoothApiCoverage.test'
import connectivityKitBluetoothTest from './ConnectivityKitBluetooth.test'
import connBleSocketMass01_p1 from './ConnBleSocketMass01_p1.test'
import connBleSocketMass01_p2 from './ConnBleSocketMass01_p2.test'
import connBleSocketMass01_p3 from './ConnBleSocketMass01_p3.test'
import connBleSocketMass02_p1 from './ConnBleSocketMass02_p1.test'
import connBleSocketMass02_p2 from './ConnBleSocketMass02_p2.test'
import connBleSocketMass02_p3 from './ConnBleSocketMass02_p3.test'
import connBleSocketMass03_p1 from './ConnBleSocketMass03_p1.test'
import connBleSocketMass03_p2 from './ConnBleSocketMass03_p2.test'
import connBleSocketMass04_p1 from './ConnBleSocketMass04_p1.test'
import connBleSocketMass04_p2 from './ConnBleSocketMass04_p2.test'
import bluetoothExpandedMatrixConnection01_p1 from './BluetoothExpandedMatrixConnection01_p1.test'
import bluetoothExpandedMatrixConnection01_p2 from './BluetoothExpandedMatrixConnection01_p2.test'
import bluetoothExpandedMatrixConnection02_p1 from './BluetoothExpandedMatrixConnection02_p1.test'
import bluetoothExpandedMatrixConnection02_p2 from './BluetoothExpandedMatrixConnection02_p2.test'
import bluetoothExpandedMatrixLegacy01_p1 from './BluetoothExpandedMatrixLegacy01_p1.test'
import bluetoothExpandedMatrixLegacy01_p2 from './BluetoothExpandedMatrixLegacy01_p2.test'
import bluetoothExpandedMatrixLegacy02_p1 from './BluetoothExpandedMatrixLegacy02_p1.test'
import bluetoothExpandedMatrixLegacy02_p2 from './BluetoothExpandedMatrixLegacy02_p2.test'
import bluetoothExpandedMatrixRotate from './BluetoothExpandedMatrixRotate.test'

export default function testsuiteApitestPart1a(): void {
  abilityTest()
  pageRoutesTest()
  bluetoothApiCoverageTest()
  connectivityKitBluetoothTest()
  connBleSocketMass01_p1()
  connBleSocketMass01_p2()
  connBleSocketMass01_p3()
  connBleSocketMass02_p1()
  connBleSocketMass02_p2()
  connBleSocketMass02_p3()
  connBleSocketMass03_p1()
  connBleSocketMass03_p2()
  connBleSocketMass04_p1()
  connBleSocketMass04_p2()
  bluetoothExpandedMatrixConnection01_p1()
  bluetoothExpandedMatrixConnection01_p2()
  bluetoothExpandedMatrixConnection02_p1()
  bluetoothExpandedMatrixConnection02_p2()
  bluetoothExpandedMatrixLegacy01_p1()
  bluetoothExpandedMatrixLegacy01_p2()
  bluetoothExpandedMatrixLegacy02_p1()
  bluetoothExpandedMatrixLegacy02_p2()
  bluetoothExpandedMatrixRotate()
}
