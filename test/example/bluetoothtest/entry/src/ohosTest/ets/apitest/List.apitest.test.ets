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
 * 仅注册 apitest/ 下套件（不含 ../uitest）。全量验证时可让 TestAbility 指向本文件 default export。
 */
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
import bluetoothFullCoverageAccess01 from './BluetoothFullCoverageAccess01.test'
import bluetoothFullCoverageLegacy01 from './BluetoothFullCoverageLegacy01.test'
import bluetoothFullCoverageLegacy02 from './BluetoothFullCoverageLegacy02.test'
import bluetoothFullCoverageProfile01 from './BluetoothFullCoverageProfile01.test'
import bluetoothFullCoverageProfile02 from './BluetoothFullCoverageProfile02.test'
import bluetoothFullCoverageGatt01 from './BluetoothFullCoverageGatt01.test'
import bluetoothFullCoverageGatt02 from './BluetoothFullCoverageGatt02.test'
import bluetoothFullCoverageEnterprise01 from './BluetoothFullCoverageEnterprise01.test'
import bluetoothFullCoverageCombo01 from './BluetoothFullCoverageCombo01.test'
import bluetoothFullCoverageCombo02 from './BluetoothFullCoverageCombo02.test'

export function registerApitestSuite(): void {
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
  bluetoothFullCoverageAccess01()
  bluetoothFullCoverageLegacy01()
  bluetoothFullCoverageLegacy02()
  bluetoothFullCoverageProfile01()
  bluetoothFullCoverageProfile02()
  bluetoothFullCoverageGatt01()
  bluetoothFullCoverageGatt02()
  bluetoothFullCoverageEnterprise01()
  bluetoothFullCoverageCombo01()
  bluetoothFullCoverageCombo02()
}

export default function testsuiteApitestOnly(): void {
  registerApitestSuite()
}
