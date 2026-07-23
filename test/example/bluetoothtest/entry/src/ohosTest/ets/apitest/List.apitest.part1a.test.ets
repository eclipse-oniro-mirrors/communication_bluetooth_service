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
import connBleSocketMass01 from './ConnBleSocketMass01.test'
import connBleSocketMass02 from './ConnBleSocketMass02.test'
import connBleSocketMass03 from './ConnBleSocketMass03.test'
import connBleSocketMass04 from './ConnBleSocketMass04.test'
import bluetoothExpandedMatrixConnection01 from './BluetoothExpandedMatrixConnection01.test'
import bluetoothExpandedMatrixConnection02 from './BluetoothExpandedMatrixConnection02.test'
import bluetoothExpandedMatrixLegacy01 from './BluetoothExpandedMatrixLegacy01.test'
import bluetoothExpandedMatrixLegacy02 from './BluetoothExpandedMatrixLegacy02.test'
import bluetoothExpandedMatrixRotate from './BluetoothExpandedMatrixRotate.test'

export default function testsuiteApitestPart1a(): void {
  abilityTest()
  pageRoutesTest()
  bluetoothApiCoverageTest()
  connectivityKitBluetoothTest()
  connBleSocketMass01()
  connBleSocketMass02()
  connBleSocketMass03()
  connBleSocketMass04()
  bluetoothExpandedMatrixConnection01()
  bluetoothExpandedMatrixConnection02()
  bluetoothExpandedMatrixLegacy01()
  bluetoothExpandedMatrixLegacy02()
  bluetoothExpandedMatrixRotate()
}
