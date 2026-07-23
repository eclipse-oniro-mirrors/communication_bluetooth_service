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
 * apitest 分段2：Connection + Socket（单进程后半段）
 */
import bluetoothFullCoverageConnection01 from './BluetoothFullCoverageConnection01.test'
import bluetoothFullCoverageConnection02 from './BluetoothFullCoverageConnection02.test'
import bluetoothFullCoverageSocket01 from './BluetoothFullCoverageSocket01.test'

export default function testsuiteApitestPart2(): void {
  bluetoothFullCoverageConnection01()
  bluetoothFullCoverageConnection02()
  bluetoothFullCoverageSocket01()
}
