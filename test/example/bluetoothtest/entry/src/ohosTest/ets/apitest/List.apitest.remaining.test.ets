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
 * 续跑套件：全量跑到 FullCoverage Legacy02 中途 App 卡死后使用。
 * 覆盖 Legacy02 及之后 Profile/Gatt/Enterprise/Connection/Socket。
 */
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
export default function testsuiteApitestRemaining(): void {
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
}
