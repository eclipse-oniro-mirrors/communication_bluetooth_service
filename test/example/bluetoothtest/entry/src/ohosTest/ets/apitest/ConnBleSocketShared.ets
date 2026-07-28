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
 * Shared stubs for generated ConnBleSocketMass*.test.ets suites.
 * Case bodies call APIs directly with LogUtil + real return/error assertions.
 */
import ble from '@ohos.bluetooth.ble';
import socket from '@ohos.bluetooth.socket';

export {
  assertErrIn,
  assertLocalOk,
  assertPeerExpect,
  assertThrownErrIn,
  BT_PEER_EXPECT,
  BT_DOC_ERR_CODES,
  ensureBtOn
} from './ApitestRealAssert';

const SPP_UUID = '00001101-0000-1000-8000-00805F9B34FB';

/** Sample device address used by mass connection/socket cases. */
export const BADDR = '00:11:22:33:44:55';
export const BAD_MAC = 'ZZ:ZZ:ZZ:ZZ:ZZ:ZZ';
export const EMPTY_MAC = '';

export function buf0(): ArrayBuffer {
  return new ArrayBuffer(0);
}

export function sppOpt(): socket.SppOptions {
  return {
    uuid: SPP_UUID,
    secure: false,
    type: socket.SppType.SPP_RFCOMM
  };
}

export function bleAd(): ble.AdvertiseData {
  return {
    serviceUuids: [SPP_UUID],
    manufactureData: [],
    serviceData: []
  };
}

export function bleAs(): ble.AdvertiseSetting {
  return {
    interval: 1600,
    txPower: -7,
    connectable: true
  };
}

export function bleAp(): ble.AdvertisingParams {
  return {
    advertisingSettings: bleAs(),
    advertisingData: bleAd()
  };
}

