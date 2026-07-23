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
import bluetoothFullCoverageLegacy02 from './BluetoothFullCoverageLegacy02.test'
import bluetoothFullCoverageProfile01 from './BluetoothFullCoverageProfile01.test'
import bluetoothFullCoverageProfile02 from './BluetoothFullCoverageProfile02.test'
import bluetoothFullCoverageGatt01 from './BluetoothFullCoverageGatt01.test'
import bluetoothFullCoverageGatt02 from './BluetoothFullCoverageGatt02.test'
import bluetoothFullCoverageEnterprise01 from './BluetoothFullCoverageEnterprise01.test'
import bluetoothFullCoverageConnection01 from './BluetoothFullCoverageConnection01.test'
import bluetoothFullCoverageConnection02 from './BluetoothFullCoverageConnection02.test'
import bluetoothFullCoverageSocket01 from './BluetoothFullCoverageSocket01.test'

export default function testsuiteApitestRemaining(): void {
  bluetoothFullCoverageLegacy02()
  bluetoothFullCoverageProfile01()
  bluetoothFullCoverageProfile02()
  bluetoothFullCoverageGatt01()
  bluetoothFullCoverageGatt02()
  bluetoothFullCoverageEnterprise01()
  bluetoothFullCoverageConnection01()
  bluetoothFullCoverageConnection02()
  bluetoothFullCoverageSocket01()
}
