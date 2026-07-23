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

/** apitest 分段1b：Access + Legacy + Profile */
import bluetoothFullCoverageAccess01 from './BluetoothFullCoverageAccess01.test'
import bluetoothFullCoverageLegacy01 from './BluetoothFullCoverageLegacy01.test'
import bluetoothFullCoverageLegacy02 from './BluetoothFullCoverageLegacy02.test'
import bluetoothFullCoverageProfile01 from './BluetoothFullCoverageProfile01.test'
import bluetoothFullCoverageProfile02 from './BluetoothFullCoverageProfile02.test'

export default function testsuiteApitestPart1b(): void {
  bluetoothFullCoverageAccess01()
  bluetoothFullCoverageLegacy01()
  bluetoothFullCoverageLegacy02()
  bluetoothFullCoverageProfile01()
  bluetoothFullCoverageProfile02()
}
