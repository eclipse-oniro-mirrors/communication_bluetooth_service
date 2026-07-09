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
import abilityTest from './Ability.test'
import pageRoutesTest from './PageRoutes.test'
import bluetoothApiCoverageTest from './BluetoothApiCoverage.test'
import connectivityKitBluetoothTest from './ConnectivityKitBluetooth.test'
import connBleSocketMass01 from './ConnBleSocketMass01.test'
import connBleSocketMass02 from './ConnBleSocketMass02.test'
import connBleSocketMass03 from './ConnBleSocketMass03.test'
import connBleSocketMass04 from './ConnBleSocketMass04.test'
import { registerApitestSuite } from './List.apitest.test'
import uitestStubSmoke from '../uitest/UitestStubSmoke.test'
import mainPageControlsUi01 from '../uitest/MainPageControlsUi01.test'
import mainPageControlsUi02 from '../uitest/MainPageControlsUi02.test'
import mainPageControlsUi03 from '../uitest/MainPageControlsUi03.test'

export default function testsuite() {
  registerApitestSuite()
  uitestStubSmoke()
  mainPageControlsUi01()
  mainPageControlsUi02()
  mainPageControlsUi03()
}



