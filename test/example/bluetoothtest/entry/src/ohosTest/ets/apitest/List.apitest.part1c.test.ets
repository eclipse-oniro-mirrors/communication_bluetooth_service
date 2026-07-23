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
import bluetoothFullCoverageGatt01 from './BluetoothFullCoverageGatt01.test'
import bluetoothFullCoverageGatt02 from './BluetoothFullCoverageGatt02.test'
import bluetoothFullCoverageEnterprise01 from './BluetoothFullCoverageEnterprise01.test'

export default function testsuiteApitestPart1c(): void {
  bluetoothFullCoverageGatt01()
  bluetoothFullCoverageGatt02()
  bluetoothFullCoverageEnterprise01()
}
