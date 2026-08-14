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
 *
 * 注意：真机单进程跑满 ~2927 例易触发 THREAD_BLOCK_6S 被杀。
 * 建议按 List.apitest.part1a/1b/1c/part2.test 分段执行（合计 1187+918+474+348=2927）。
 */
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
import bluetoothFullCoverageAccess01_p1 from './BluetoothFullCoverageAccess01_p1.test'
import bluetoothFullCoverageAccess01_p2 from './BluetoothFullCoverageAccess01_p2.test'
import bluetoothFullCoverageAccess01_p3 from './BluetoothFullCoverageAccess01_p3.test'
import bluetoothFullCoverageLegacy01_p1 from './BluetoothFullCoverageLegacy01_p1.test'
import bluetoothFullCoverageLegacy01_p2 from './BluetoothFullCoverageLegacy01_p2.test'
import bluetoothFullCoverageLegacy01_p3 from './BluetoothFullCoverageLegacy01_p3.test'
import bluetoothFullCoverageLegacy02_p1 from './BluetoothFullCoverageLegacy02_p1.test'
import bluetoothFullCoverageLegacy02_p2 from './BluetoothFullCoverageLegacy02_p2.test'
import bluetoothFullCoverageLegacy02_p3 from './BluetoothFullCoverageLegacy02_p3.test'
import bluetoothFullCoverageLegacy02_p4 from './BluetoothFullCoverageLegacy02_p4.test'
import bluetoothFullCoverageLegacy02_p5 from './BluetoothFullCoverageLegacy02_p5.test'
import bluetoothFullCoverageLegacy02_p6 from './BluetoothFullCoverageLegacy02_p6.test'
import bluetoothFullCoverageLegacy02_p7 from './BluetoothFullCoverageLegacy02_p7.test'
import bluetoothFullCoverageProfile01_p1 from './BluetoothFullCoverageProfile01_p1.test'
import bluetoothFullCoverageProfile01_p2 from './BluetoothFullCoverageProfile01_p2.test'
import bluetoothFullCoverageProfile01_p3 from './BluetoothFullCoverageProfile01_p3.test'
import bluetoothFullCoverageProfile01_p4 from './BluetoothFullCoverageProfile01_p4.test'
import bluetoothFullCoverageProfile02_p1 from './BluetoothFullCoverageProfile02_p1.test'
import bluetoothFullCoverageProfile02_p2 from './BluetoothFullCoverageProfile02_p2.test'
import bluetoothFullCoverageProfile02_p3 from './BluetoothFullCoverageProfile02_p3.test'
import bluetoothFullCoverageProfile02_p4 from './BluetoothFullCoverageProfile02_p4.test'
import bluetoothFullCoverageProfile02_p5 from './BluetoothFullCoverageProfile02_p5.test'
import bluetoothFullCoverageProfile02_p6 from './BluetoothFullCoverageProfile02_p6.test'
import bluetoothFullCoverageGatt01_p1 from './BluetoothFullCoverageGatt01_p1.test'
import bluetoothFullCoverageGatt01_p2 from './BluetoothFullCoverageGatt01_p2.test'
import bluetoothFullCoverageGatt02_p1 from './BluetoothFullCoverageGatt02_p1.test'
import bluetoothFullCoverageGatt02_p2 from './BluetoothFullCoverageGatt02_p2.test'
import bluetoothFullCoverageGatt02_p3 from './BluetoothFullCoverageGatt02_p3.test'
import bluetoothFullCoverageGatt02_p4 from './BluetoothFullCoverageGatt02_p4.test'
import bluetoothFullCoverageGatt02_p5 from './BluetoothFullCoverageGatt02_p5.test'
import bluetoothFullCoverageGatt02_p6 from './BluetoothFullCoverageGatt02_p6.test'
import bluetoothFullCoverageEnterprise01_p1 from './BluetoothFullCoverageEnterprise01_p1.test'
import bluetoothFullCoverageEnterprise01_p2 from './BluetoothFullCoverageEnterprise01_p2.test'
import bluetoothFullCoverageEnterprise01_p3 from './BluetoothFullCoverageEnterprise01_p3.test'
import bluetoothFullCoverageConnection01_p1 from './BluetoothFullCoverageConnection01_p1.test'
import bluetoothFullCoverageConnection01_p2 from './BluetoothFullCoverageConnection01_p2.test'
import bluetoothFullCoverageConnection01_p3 from './BluetoothFullCoverageConnection01_p3.test'
import bluetoothFullCoverageConnection02_p1 from './BluetoothFullCoverageConnection02_p1.test'
import bluetoothFullCoverageConnection02_p2 from './BluetoothFullCoverageConnection02_p2.test'
import bluetoothFullCoverageConnection02_p3 from './BluetoothFullCoverageConnection02_p3.test'
import bluetoothFullCoverageSocket01_p1 from './BluetoothFullCoverageSocket01_p1.test'
import bluetoothFullCoverageSocket01_p2 from './BluetoothFullCoverageSocket01_p2.test'
import bluetoothFullCoverageConstant01_p1 from './BluetoothFullCoverageConstant01_p1.test'
import bluetoothFullCoverageConstant01_p2 from './BluetoothFullCoverageConstant01_p2.test'
import bluetoothFullCoverageConstant01_p3 from './BluetoothFullCoverageConstant01_p3.test'
import bluetoothFullCoverageConstant01_p4 from './BluetoothFullCoverageConstant01_p4.test'
export function registerApitestSuite(): void {
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
  bluetoothFullCoverageAccess01_p1()
  bluetoothFullCoverageAccess01_p2()
  bluetoothFullCoverageAccess01_p3()
  bluetoothFullCoverageLegacy01_p1()
  bluetoothFullCoverageLegacy01_p2()
  bluetoothFullCoverageLegacy01_p3()
  bluetoothFullCoverageLegacy02_p1()
  bluetoothFullCoverageLegacy02_p2()
  bluetoothFullCoverageLegacy02_p3()
  bluetoothFullCoverageLegacy02_p4()
  bluetoothFullCoverageLegacy02_p5()
  bluetoothFullCoverageLegacy02_p6()
  bluetoothFullCoverageLegacy02_p7()
  bluetoothFullCoverageProfile01_p1()
  bluetoothFullCoverageProfile01_p2()
  bluetoothFullCoverageProfile01_p3()
  bluetoothFullCoverageProfile01_p4()
  bluetoothFullCoverageProfile02_p1()
  bluetoothFullCoverageProfile02_p2()
  bluetoothFullCoverageProfile02_p3()
  bluetoothFullCoverageProfile02_p4()
  bluetoothFullCoverageProfile02_p5()
  bluetoothFullCoverageProfile02_p6()
  bluetoothFullCoverageGatt01_p1()
  bluetoothFullCoverageGatt01_p2()
  bluetoothFullCoverageGatt02_p1()
  bluetoothFullCoverageGatt02_p2()
  bluetoothFullCoverageGatt02_p3()
  bluetoothFullCoverageGatt02_p4()
  bluetoothFullCoverageGatt02_p5()
  bluetoothFullCoverageGatt02_p6()
  bluetoothFullCoverageEnterprise01_p1()
  bluetoothFullCoverageEnterprise01_p2()
  bluetoothFullCoverageEnterprise01_p3()
  bluetoothFullCoverageConnection01_p1()
  bluetoothFullCoverageConnection01_p2()
  bluetoothFullCoverageConnection01_p3()
  bluetoothFullCoverageConnection02_p1()
  bluetoothFullCoverageConnection02_p2()
  bluetoothFullCoverageConnection02_p3()
  bluetoothFullCoverageSocket01_p1()
  bluetoothFullCoverageSocket01_p2()
  bluetoothFullCoverageConstant01_p1()
  bluetoothFullCoverageConstant01_p2()
  bluetoothFullCoverageConstant01_p3()
  bluetoothFullCoverageConstant01_p4()
}

export default function testsuiteApitestOnly(): void {
  registerApitestSuite()
}




