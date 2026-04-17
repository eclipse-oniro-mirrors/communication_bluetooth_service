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

import hilog from '@ohos.hilog';
/**
 * Gatt Page Of Bluetooth test
 */

import router from '@ohos.router';
import promptAction from '@ohos.promptAction';

const PAGE_URI_GET_GATT_CLIENT_MANAGER = 'pages/subManualApiTestManager/subGattTestManager/gattClientManager';
const PAGE_URI_GET_GATT_SERVER_MANAGER = 'pages/subManualApiTestManager/subGattTestManager/gattServerManager';

export function testGattClientManager(): string {
  try {
    router.pushUrl({ url : PAGE_URI_GET_GATT_CLIENT_MANAGER })
    let message = $r('app.string.bluetoothtest_str_tzcg')
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGattClientManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGattClientManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}

export function testGattServerManager(): string {
  try {
    router.pushUrl({ url : PAGE_URI_GET_GATT_SERVER_MANAGER })
    let message = $r('app.string.bluetoothtest_str_tzcg')
    promptAction.showToast({ message : message })
    return message
  } catch( e ) {
    hilog.info(0x0000, 'BtTest', '%{public}s', `Failed to testGattServerManager. message: ${ e.message }, code: ${ e.code }`)
    let message = `Failed to testGattServerManager. message: ${ e.message }, code: ${ e.code }`
    promptAction.showToast({ message : message })
    return message
  }
}


