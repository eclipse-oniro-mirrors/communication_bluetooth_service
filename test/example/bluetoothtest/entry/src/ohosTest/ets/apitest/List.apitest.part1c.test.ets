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

/** apitest 分段1c：Gatt + Enterprise */
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
export default function testsuiteApitestPart1c(): void {
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
}
