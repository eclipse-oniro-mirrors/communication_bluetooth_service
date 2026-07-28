/*
 * Copyright (C) 2023-2024 Huawei Device Co., Ltd.
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
 * @file
 * @kit ConnectivityKit
 */
import type { AsyncCallback, Callback} from './@ohos.base';
import type constant from './@ohos.bluetooth.constant';
import type common from './@ohos.bluetooth.common';
/**
 * Provides methods to operate or manage Bluetooth.
 *
 * @namespace ble
 * @syscap SystemCapability.Communication.Bluetooth.Core
 * @since 10
 */
/**
 * Provides methods to operate or manage Bluetooth.
 *
 * @namespace ble
 * @syscap SystemCapability.Communication.Bluetooth.Core
 * @atomicservice
 * @since 12
 */
/**
 * Provides methods to operate or manage Bluetooth.
 *
 * @namespace ble
 * @syscap SystemCapability.Communication.Bluetooth.Core
 * @crossplatform
 * @atomicservice
 * @since 13
 */
declare namespace ble {
    /**
     * Indicate the profile connection state.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Indicate the profile connection state.
     *
     * @typedef { constant.ProfileConnectionState} ProfileConnectionState
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Indicate the profile connection state.
     *
     * @typedef { constant.ProfileConnectionState} ProfileConnectionState
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    type ProfileConnectionState = constant.ProfileConnectionState;
    /**
     * Bluetooth device address.
     *
     * @typedef { common.BluetoothAddress} BluetoothAddress
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 23
     */
    type BluetoothAddress = common.BluetoothAddress;
    /**
     * create a Gatt server instance.
     *
     * @returns { GattServer} Returns a Gatt server instance {@code GattServer}.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * create a Gatt server instance.
     *
     * @returns { GattServer} Returns a Gatt server instance {@code GattServer}.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * create a Gatt server instance.
     *
     * @returns { GattServer} Returns a Gatt server instance {@code GattServer}.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    function createGattServer(): GattServer;
    /**
     * create a Gatt client device instance.
     *
     * @param { string} deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
     * @returns { GattClientDevice} Returns a Gatt client device instance {@code GattClientDevice}.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * create a Gatt client device instance.
     *
     * @param { string} deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
     * @returns { GattClientDevice} Returns a Gatt client device instance {@code GattClientDevice}.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * create a Gatt client device instance.
     *
     * @param { string} deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
     * @returns { GattClientDevice} Returns a Gatt client device instance {@code GattClientDevice}.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    function createGattClientDevice(deviceId: string): GattClientDevice;
    /**
     * Create a ble scanner instance. Each ble scanner instance can be independently started or stopped.
     *
     * @returns { BleScanner} Returns the promise object.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 15
     */
    function createBleScanner(): BleScanner;
    /**
     * Obtains the list of devices in the connected status.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @returns { Array<string>} Returns the list of device address.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Obtains the list of devices in the connected status.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @returns { Array<string>} Returns the list of device address.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    function getConnectedBLEDevices(): Array<string>;
    /**
     * Obtains the list of devices in the connected status.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { BleProfile} profile - The profile in the BLE protocol.
     *     It is used to obtain the connected devices corresponding to the profile.
     * @returns { Array<string>} Returns the list of device address.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 21
     */
    function getConnectedBLEDevices(profile: BleProfile): Array<string>;
    /**
     * Starts scanning for specified BLE devices with filters.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { Array<ScanFilter>} filters - Indicates the list of filters used to filter out specified devices.
     * If you do not want to use filter, set this parameter to {@code null}.
     * @param { ScanOptions} options - Indicates the parameters for scanning and if the user does not assign a value, the default value will be used.
     * {@link ScanOptions#interval} set to 0, {@link ScanOptions#dutyMode} set to {@link SCAN_MODE_LOW_POWER}
     * and {@link ScanOptions#matchMode} set to {@link MATCH_MODE_AGGRESSIVE}.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Starts scanning for specified BLE devices with filters.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { Array<ScanFilter>} filters - Indicates the list of filters used to filter out specified devices.
     * If you do not want to use filter, set this parameter to {@code null}.
     * @param { ScanOptions} options - Indicates the parameters for scanning and if the user does not assign a value, the default value will be used.
     * {@link ScanOptions#interval} set to 0, {@link ScanOptions#dutyMode} set to {@link SCAN_MODE_LOW_POWER}
     * and {@link ScanOptions#matchMode} set to {@link MATCH_MODE_AGGRESSIVE}.
     * and {@link ScanOptions#phyType} set to {@link PHY_LE_ALL_SUPPORTED}.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Starts scanning for specified BLE devices with filters.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { Array<ScanFilter>} filters - Indicates the list of filters used to filter out specified devices.
     * If you do not want to use filter, set this parameter to {@code null}.
     * @param { ScanOptions} options - Indicates the parameters for scanning and if the user does not assign a value, the default value will be used.
     * {@link ScanOptions#interval} set to 0, {@link ScanOptions#dutyMode} set to {@link SCAN_MODE_LOW_POWER}
     * and {@link ScanOptions#matchMode} set to {@link MATCH_MODE_AGGRESSIVE}.
     * and {@link ScanOptions#phyType} set to {@link PHY_LE_ALL_SUPPORTED}.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    function startBLEScan(filters: Array<ScanFilter>, options?: ScanOptions): void;
    /**
     * Stops BLE scanning.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Stops BLE scanning.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Stops BLE scanning.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    function stopBLEScan(): void;
    /**
     * Starts BLE advertising.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertiseSetting} setting - Indicates the settings for BLE advertising.
     * @param { AdvertiseData} advData - Indicates the advertising data.
     * @param { AdvertiseData} advResponse - Indicates the scan response associated with the advertising data.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Starts BLE advertising.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertiseSetting} setting - Indicates the settings for BLE advertising.
     * @param { AdvertiseData} advData - Indicates the advertising data.
     * @param { AdvertiseData} advResponse - Indicates the scan response associated with the advertising data.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Starts BLE advertising.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertiseSetting} setting - Indicates the settings for BLE advertising.
     * @param { AdvertiseData} advData - Indicates the advertising data.
     * @param { AdvertiseData} advResponse - Indicates the scan response associated with the advertising data.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    /**
     * Starts BLE advertising.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertiseSetting} setting - Indicates the settings for BLE advertising.
     * @param { AdvertiseData} advData - Indicates the advertising data.
     * @param { AdvertiseData} advResponse - Indicates the scan response associated with the advertising data.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900010 - The number of advertising resources reaches the upper limit.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @throws { BusinessError} 2902054 - The length of the advertising data exceeds the upper limit.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 20
     */
    /**
     * Starts BLE advertising.
     * - If only {@link AdvertiseData#includeDeviceName} is set to true,
     *   the local name will be carried in the broadcast packet.
     * - If only {@link AdvertiseData#advertiseName} is set,
     *   its value will be used as a custom name and carried in the broadcast packet.
     * - If {@link AdvertiseData#includeDeviceName} is set to true and {@link AdvertiseData#advertiseName} is specified,
     *   the {@link AdvertiseData#advertiseName} property will take effect.
     * - To set {@link AdvertiseData#advertiseName},
     *   ensure that ohos.permission.MANAGE_BLUETOOTH_ADVERTISER_NAME has been added.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH or (ohos.permission.ACCESS_BLUETOOTH and
     *     ohos.permission.MANAGE_BLUETOOTH_ADVERTISER_NAME)
     * @param { AdvertiseSetting} setting - Indicates the settings for BLE advertising.
     * @param { AdvertiseData} advData - Indicates the advertising data.
     * @param { AdvertiseData} advResponse - Indicates the scan response associated with the advertising data.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900010 - The number of advertising resources reaches the upper limit.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @throws { BusinessError} 2902054 - The length of the advertising data exceeds the upper limit.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 23
     */
    function startAdvertising(setting: AdvertiseSetting, advData: AdvertiseData, advResponse?: AdvertiseData): void;
    /**
     * Stops BLE advertising.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Stops BLE advertising.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Stops BLE advertising.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    function stopAdvertising(): void;
    /**
     * Starts BLE advertising.
     * The API returns a advertising ID. The ID can be used to temporarily enable or disable this advertising
     * using the API {@link enableAdvertising} or {@link disableAdvertising}.
     * To completely stop the advertising corresponding to the ID, invoke the API {@link stopAdvertising} with ID.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertisingParams} advertisingParams - Indicates the params for BLE advertising.
     * @param { AsyncCallback<number>} callback - the callback of advertise ID.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 11
     */
    /**
     * Starts BLE advertising.
     * The API returns a advertising ID. The ID can be used to temporarily enable or disable this advertising
     * using the API {@link enableAdvertising} or {@link disableAdvertising}.
     * To completely stop the advertising corresponding to the ID, invoke the API {@link stopAdvertising} with ID.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertisingParams} advertisingParams - Indicates the params for BLE advertising.
     * @param { AsyncCallback<number>} callback - the callback of advertise ID.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    /**
     * Starts BLE advertising.
     * The API returns a advertising ID. The ID can be used to temporarily enable or disable this advertising
     * using the API {@link enableAdvertising} or {@link disableAdvertising}.
     * To completely stop the advertising corresponding to the ID, invoke the API {@link stopAdvertising} with ID.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertisingParams} advertisingParams - Indicates the params for BLE advertising.
     * @param { AsyncCallback<number>} callback - the callback of advertise ID.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900010 - The number of advertising resources reaches the upper limit.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @throws { BusinessError} 2902054 - The length of the advertising data exceeds the upper limit.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 20
     */
    /**
     * Starts BLE advertising.
     * The API returns a advertising ID. The ID can be used to temporarily enable or disable this advertising
     * using the API {@link enableAdvertising} or {@link disableAdvertising}.
     * To completely stop the advertising corresponding to the ID, invoke the API {@link stopAdvertising} with ID.
     * - If only {@link AdvertiseData#includeDeviceName} is set to true,
     *   the local name will be carried in the broadcast packet.
     * - If only {@link AdvertiseData#advertiseName} is set,
     *   its value will be used as a custom name and carried in the broadcast packet.
     * - If {@link AdvertiseData#includeDeviceName} is set to true and {@link AdvertiseData#advertiseName} is specified,
     *   the {@link AdvertiseData#advertiseName} property will take effect.
     * - To set {@link AdvertiseData#advertiseName},
     *   ensure that ohos.permission.MANAGE_BLUETOOTH_ADVERTISER_NAME has been added.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH or (ohos.permission.ACCESS_BLUETOOTH and
     *     ohos.permission.MANAGE_BLUETOOTH_ADVERTISER_NAME)
     * @param { AdvertisingParams} advertisingParams - Indicates the params for BLE advertising.
     * @param { AsyncCallback<number>} callback - the callback of advertise ID.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900010 - The number of advertising resources reaches the upper limit.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @throws { BusinessError} 2902054 - The length of the advertising data exceeds the upper limit.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 23
     */
    function startAdvertising(advertisingParams: AdvertisingParams, callback: AsyncCallback<number>): void;
    /**
     * Starts BLE advertising.
     * The API returns a advertising ID. The ID can be used to temporarily enable or disable this advertising
     * using the API {@link enableAdvertising} or {@link disableAdvertising}.
     * To completely stop the advertising corresponding to the ID, invoke the API {@link stopAdvertising} with ID.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertisingParams} advertisingParams - Indicates the param for BLE advertising.
     * @returns { Promise<number>} Returns the promise object.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 11
     */
    /**
     * Starts BLE advertising.
     * The API returns a advertising ID. The ID can be used to temporarily enable or disable this advertising
     * using the API {@link enableAdvertising} or {@link disableAdvertising}.
     * To completely stop the advertising corresponding to the ID, invoke the API {@link stopAdvertising} with ID.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertisingParams} advertisingParams - Indicates the param for BLE advertising.
     * @returns { Promise<number>} Returns the promise object.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    /**
     * Starts BLE advertising.
     * The API returns a advertising ID. The ID can be used to temporarily enable or disable this advertising
     * using the API {@link enableAdvertising} or {@link disableAdvertising}.
     * To completely stop the advertising corresponding to the ID, invoke the API {@link stopAdvertising} with ID.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertisingParams} advertisingParams - Indicates the param for BLE advertising.
     * @returns { Promise<number>} Returns the promise object.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900010 - The number of advertising resources reaches the upper limit.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @throws { BusinessError} 2902054 - The length of the advertising data exceeds the upper limit.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 20
     */
    /**
     * Starts BLE advertising.
     * The API returns a advertising ID. The ID can be used to temporarily enable or disable this advertising
     * using the API {@link enableAdvertising} or {@link disableAdvertising}.
     * To completely stop the advertising corresponding to the ID, invoke the API {@link stopAdvertising} with ID.
     * - If only {@link AdvertiseData#includeDeviceName} is set to true,
     *   the local name will be carried in the broadcast packet.
     * - If only {@link AdvertiseData#advertiseName} is set,
     *   its value will be used as a custom name and carried in the broadcast packet.
     * - If {@link AdvertiseData#includeDeviceName} is set to true and {@link AdvertiseData#advertiseName} is specified,
     *   the {@link AdvertiseData#advertiseName} property will take effect.
     * - To set {@link AdvertiseData#advertiseName},
     *   ensure that ohos.permission.MANAGE_BLUETOOTH_ADVERTISER_NAME has been added.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH or (ohos.permission.ACCESS_BLUETOOTH and
     *     ohos.permission.MANAGE_BLUETOOTH_ADVERTISER_NAME)
     * @param { AdvertisingParams} advertisingParams - Indicates the param for BLE advertising.
     * @returns { Promise<number>} Returns the promise object.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900010 - The number of advertising resources reaches the upper limit.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @throws { BusinessError} 2902054 - The length of the advertising data exceeds the upper limit.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 23
     */
    function startAdvertising(advertisingParams: AdvertisingParams): Promise<number>;
    /**
     * Enable the advertising with a specific ID temporarily.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertisingEnableParams} advertisingEnableParams - Indicates the params for enable advertising.
     * @param { AsyncCallback<void>} callback - the callback result.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 11
     */
    /**
     * Enable the advertising with a specific ID temporarily.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertisingEnableParams} advertisingEnableParams - Indicates the params for enable advertising.
     * @param { AsyncCallback<void>} callback - the callback result.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @throws { BusinessError} 2902055 - Invalid advertising id.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 20
     */
    function enableAdvertising(advertisingEnableParams: AdvertisingEnableParams, callback: AsyncCallback<void>): void;
    /**
     * Enable the advertising with a specific ID temporarily.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertisingEnableParams} advertisingEnableParams - Indicates the params for enable advertising.
     * @returns { Promise<void>} Returns the promise object.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 11
     */
    /**
     * Enable the advertising with a specific ID temporarily.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertisingEnableParams} advertisingEnableParams - Indicates the params for enable advertising.
     * @returns { Promise<void>} Returns the promise object.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @throws { BusinessError} 2902055 - Invalid advertising id.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 20
     */
    function enableAdvertising(advertisingEnableParams: AdvertisingEnableParams): Promise<void>;
    /**
     * Disable the advertising with a specific ID temporarily.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertisingDisableParams} advertisingDisableParams - Indicates the params for disable advertising.
     * @param { AsyncCallback<void>} callback - the callback result.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 11
     */
    /**
     * Disable the advertising with a specific ID temporarily.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertisingDisableParams} advertisingDisableParams - Indicates the params for disable advertising.
     * @param { AsyncCallback<void>} callback - the callback result.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @throws { BusinessError} 2902055 - Invalid advertising id.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 20
     */
    function disableAdvertising(advertisingDisableParams: AdvertisingDisableParams, callback: AsyncCallback<void>): void;
    /**
     * Disable the advertising with a specific ID temporarily.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertisingDisableParams} advertisingDisableParams - Indicates the params for disable advertising.
     * @returns { Promise<void>} Returns the promise object.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 11
     */
    /**
     * Disable the advertising with a specific ID temporarily.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { AdvertisingDisableParams} advertisingDisableParams - Indicates the params for disable advertising.
     * @returns { Promise<void>} Returns the promise object.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @throws { BusinessError} 2902055 - Invalid advertising id.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 20
     */
    function disableAdvertising(advertisingDisableParams: AdvertisingDisableParams): Promise<void>;
    /**
     * Stops BLE advertising.
     * Completely stop the advertising corresponding to the ID.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { number} advertisingId - Indicates the ID for this BLE advertising.
     * @param { AsyncCallback<void>} callback - the callback result.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 11
     */
    /**
     * Stops BLE advertising.
     * Completely stop the advertising corresponding to the ID.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { number} advertisingId - Indicates the ID for this BLE advertising.
     * @param { AsyncCallback<void>} callback - the callback result.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    /**
     * Stops BLE advertising.
     * Completely stop the advertising corresponding to the ID.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { number} advertisingId - Indicates the ID for this BLE advertising.
     * @param { AsyncCallback<void>} callback - the callback result.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @throws { BusinessError} 2902055 - Invalid advertising id.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 20
     */
    function stopAdvertising(advertisingId: number, callback: AsyncCallback<void>): void;
    /**
     * Stops BLE advertising.
     * Completely stop the advertising corresponding to the ID.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { number} advertisingId - Indicates the ID for this BLE advertising.
     * @returns { Promise<void>} Returns the promise object.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 11
     */
    /**
     * Stops BLE advertising.
     * Completely stop the advertising corresponding to the ID.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { number} advertisingId - Indicates the ID for this BLE advertising.
     * @returns { Promise<void>} Returns the promise object.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    /**
     * Stops BLE advertising.
     * Completely stop the advertising corresponding to the ID.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { number} advertisingId - Indicates the ID for this BLE advertising.
     * @returns { Promise<void>} Returns the promise object.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @throws { BusinessError} 2902055 - Invalid advertising id.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 20
     */
    function stopAdvertising(advertisingId: number): Promise<void>;
    /**
     * Subscribing to advertising state change event.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'advertisingStateChange'} type - Type of the advertising state to listen for.
     * @param { Callback<AdvertisingStateChangeInfo>} callback - Callback used to listen for the advertising state.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 11
     */
    /**
     * Subscribing to advertising state change event.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'advertisingStateChange'} type - Type of the advertising state to listen for.
     * @param { Callback<AdvertisingStateChangeInfo>} callback - Callback used to listen for the advertising state.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    function on(type: 'advertisingStateChange', callback: Callback<AdvertisingStateChangeInfo>): void;
    /**
     * Unsubscribe from advertising state change event.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'advertisingStateChange'} type - Type of the advertising state to listen for.
     * @param { Callback<AdvertisingStateChangeInfo>} callback - Callback used to listen for the advertising state.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 11
     */
    /**
     * Unsubscribe from advertising state change event.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'advertisingStateChange'} type - Type of the advertising state to listen for.
     * @param { Callback<AdvertisingStateChangeInfo>} callback - Callback used to listen for the advertising state.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    function off(type: 'advertisingStateChange', callback?: Callback<AdvertisingStateChangeInfo>): void;
    /**
     * Subscribe BLE scan result.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'BLEDeviceFind'} type - Type of the scan result event to listen for.
     * @param { Callback<Array<ScanResult>>} callback - Callback used to listen for the scan result event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Subscribe BLE scan result.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'BLEDeviceFind'} type - Type of the scan result event to listen for.
     * @param { Callback<Array<ScanResult>>} callback - Callback used to listen for the scan result event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Subscribe BLE scan result.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'BLEDeviceFind'} type - Type of the scan result event to listen for.
     * @param { Callback<Array<ScanResult>>} callback - Callback used to listen for the scan result event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    function on(type: 'BLEDeviceFind', callback: Callback<Array<ScanResult>>): void;
    /**
     * Unsubscribe BLE scan result.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'BLEDeviceFind'} type - Type of the scan result event to listen for.
     * @param { Callback<Array<ScanResult>>} callback - Callback used to listen for the scan result event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Unsubscribe BLE scan result.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'BLEDeviceFind'} type - Type of the scan result event to listen for.
     * @param { Callback<Array<ScanResult>>} callback - Callback used to listen for the scan result event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Unsubscribe BLE scan result.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'BLEDeviceFind'} type - Type of the scan result event to listen for.
     * @param { Callback<Array<ScanResult>>} callback - Callback used to listen for the scan result event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    function off(type: 'BLEDeviceFind', callback?: Callback<Array<ScanResult>>): void;
    /**
     * Manages GATT server. Before calling an Gatt server method, you must use {@link createGattServer} to create an GattServer instance.
     *
     * @typedef GattServer
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Manages GATT server. Before calling an Gatt server method, you must use {@link createGattServer} to create an GattServer instance.
     *
     * @typedef GattServer
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Manages GATT server. Before calling an Gatt server method, you must use {@link createGattServer} to create an GattServer instance.
     *
     * @typedef GattServer
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface GattServer {
        /**
         * Adds a specified service to be hosted.
         * <p>The added service and its characteristics are provided by the local device.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { GattService} service - Indicates the service to add.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Adds a specified service to be hosted.
         * <p>The added service and its characteristics are provided by the local device.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { GattService} service - Indicates the service to add.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Adds a specified service to be hosted.
         * <p>The added service and its characteristics are provided by the local device.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { GattService} service - Indicates the service to add.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        addService(service: GattService): void;
        /**
         * Removes a specified service from the list of GATT services provided by this device.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} serviceUuid - Indicates the UUID of the service to remove.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Removes a specified service from the list of GATT services provided by this device.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} serviceUuid - Indicates the UUID of the service to remove.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Removes a specified service from the list of GATT services provided by this device.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} serviceUuid - Indicates the UUID of the service to remove.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        removeService(serviceUuid: string): void;
        /**
         * Obtain a specific GATT service by using a UUID.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} serviceUuid - Indicates the UUID of the service.
         * @returns { GattService} The GATT service has been obtained.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @throws { BusinessError} 2901008 - Gatt service is not found.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 22
         */
        getService(serviceUuid: string): GattService;
        /**
         * Obtain the list of GATT services registered by the application.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @returns { GattService[]} The list of GATT service has been obtained.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 22
         */
        getServices(): GattService[];
        /**
         * Closes this {@code GattServer} object and unregisters its callbacks.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Closes this {@code GattServer} object and unregisters its callbacks.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Closes this {@code GattServer} object and unregisters its callbacks.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        close(): void;
        /**
         * Sends a notification of a change in a specified local characteristic with a asynchronous callback.
         * <p>This method should be called for every BLE peripheral device that has requested notifications.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
         * @param { NotifyCharacteristic} notifyCharacteristic - Indicates the local characteristic that has changed.
         * @param { AsyncCallback<void>} callback - Callback used to return the result.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Sends a notification of a change in a specified local characteristic with a asynchronous callback.
         * <p>This method should be called for every BLE peripheral device that has requested notifications.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
         * @param { NotifyCharacteristic} notifyCharacteristic - Indicates the local characteristic that has changed.
         * @param { AsyncCallback<void>} callback - Callback used to return the result.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Sends a notification of a change in a specified local characteristic with a asynchronous callback.
         * <p>This method should be called for every BLE peripheral device that has requested notifications.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
         * @param { NotifyCharacteristic} notifyCharacteristic - Indicates the local characteristic that has changed.
         * @param { AsyncCallback<void>} callback - Callback used to return the result.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        notifyCharacteristicChanged(deviceId: string, notifyCharacteristic: NotifyCharacteristic, callback: AsyncCallback<void>): void;
        /**
         * Sends a notification of a change in a specified local characteristic with a asynchronous callback.
         * <p>This method should be called for every BLE peripheral device that has requested notifications.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
         * @param { NotifyCharacteristic} notifyCharacteristic - Indicates the local characteristic that has changed.
         * @returns { Promise<void>} Promise used to return the result.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Sends a notification of a change in a specified local characteristic with a asynchronous callback.
         * <p>This method should be called for every BLE peripheral device that has requested notifications.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
         * @param { NotifyCharacteristic} notifyCharacteristic - Indicates the local characteristic that has changed.
         * @returns { Promise<void>} Promise used to return the result.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Sends a notification of a change in a specified local characteristic with a asynchronous callback.
         * <p>This method should be called for every BLE peripheral device that has requested notifications.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
         * @param { NotifyCharacteristic} notifyCharacteristic - Indicates the local characteristic that has changed.
         * @returns { Promise<void>} Promise used to return the result.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        notifyCharacteristicChanged(deviceId: string, notifyCharacteristic: NotifyCharacteristic): Promise<void>;
        /**
         * Sends a response to a specified read or write request to a given BLE peripheral device.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { ServerResponse} serverResponse - Indicates the response parameters {@link ServerResponse}.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Sends a response to a specified read or write request to a given BLE peripheral device.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { ServerResponse} serverResponse - Indicates the response parameters {@link ServerResponse}.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Sends a response to a specified read or write request to a given BLE peripheral device.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { ServerResponse} serverResponse - Indicates the response parameters {@link ServerResponse}.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        sendResponse(serverResponse: ServerResponse): void;
        /**
         * Get the connection state of a specific device.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
         * @returns { ProfileConnectionState} Connection state.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 22
         */
        getConnectedState(deviceId: string): ProfileConnectionState;
        /**
         * Read the phy associated with the connection.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
         * @returns { Promise<PhyValue>} Promise used to return the phy value read.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @throws { BusinessError} 2901003 - The connection is not established.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @stagemodelonly
         * @since 23
         */
        readPhy(deviceId: string): Promise<PhyValue>;
        /**
         * Set the preferred phy associated with the connection.
         * Whether the phy value will be changed depends on the strategy of the Bluetooth chip.
         * A successful call to this interface does not guarantee that the chip's phy value has been successfully set.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
         * @param { PhyValue} phyValue - Indicates the phy to set.
         * @returns { Promise<void>} Promise used to return the result.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @throws { BusinessError} 2901003 - The connection is not established.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @stagemodelonly
         * @since 23
         */
        setPhy(deviceId: string, phyValue: PhyValue): Promise<void>;
        /**
         * Removes all services from the list of GATT services offered by this device.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 801 - Capability not supported.
         *     Failed to call the API because the short-range chip is not inserted on the 2in1 device.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @stagemodelonly
         * @atomicservice
         * @since 26.0.0
         */
        removeAllServices(): void;
        /**
         * Subscribe characteristic read event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'characteristicRead'} type - Type of the characteristic read event to listen for.
         * @param { Callback<CharacteristicReadRequest>} callback - Callback used to listen for the characteristic read event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Subscribe characteristic read event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'characteristicRead'} type - Type of the characteristic read event to listen for.
         * @param { Callback<CharacteristicReadRequest>} callback - Callback used to listen for the characteristic read event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Subscribe characteristic read event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'characteristicRead'} type - Type of the characteristic read event to listen for.
         * @param { Callback<CharacteristicReadRequest>} callback - Callback used to listen for the characteristic read event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        on(type: 'characteristicRead', callback: Callback<CharacteristicReadRequest>): void;
        /**
         * Unsubscribe characteristic read event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'characteristicRead'} type - Type of the characteristic read event to listen for.
         * @param { Callback<CharacteristicReadRequest>} callback - Callback used to listen for the characteristic read event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Unsubscribe characteristic read event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'characteristicRead'} type - Type of the characteristic read event to listen for.
         * @param { Callback<CharacteristicReadRequest>} callback - Callback used to listen for the characteristic read event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Unsubscribe characteristic read event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'characteristicRead'} type - Type of the characteristic read event to listen for.
         * @param { Callback<CharacteristicReadRequest>} callback - Callback used to listen for the characteristic read event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        off(type: 'characteristicRead', callback?: Callback<CharacteristicReadRequest>): void;
        /**
         * Subscribe characteristic write event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'characteristicWrite'} type - Type of the characteristic write event to listen for.
         * @param { Callback<CharacteristicWriteRequest>} callback - Callback used to listen for the characteristic write event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Subscribe characteristic write event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'characteristicWrite'} type - Type of the characteristic write event to listen for.
         * @param { Callback<CharacteristicWriteRequest>} callback - Callback used to listen for the characteristic write event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Subscribe characteristic write event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'characteristicWrite'} type - Type of the characteristic write event to listen for.
         * @param { Callback<CharacteristicWriteRequest>} callback - Callback used to listen for the characteristic write event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        on(type: 'characteristicWrite', callback: Callback<CharacteristicWriteRequest>): void;
        /**
         * Unsubscribe characteristic write event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'characteristicWrite'} type - Type of the characteristic write event to listen for.
         * @param { Callback<CharacteristicWriteRequest>} callback - Callback used to listen for the characteristic write event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Unsubscribe characteristic write event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'characteristicWrite'} type - Type of the characteristic write event to listen for.
         * @param { Callback<CharacteristicWriteRequest>} callback - Callback used to listen for the characteristic write event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Unsubscribe characteristic write event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'characteristicWrite'} type - Type of the characteristic write event to listen for.
         * @param { Callback<CharacteristicWriteRequest>} callback - Callback used to listen for the characteristic write event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        off(type: 'characteristicWrite', callback?: Callback<CharacteristicWriteRequest>): void;
        /**
         * Subscribe descriptor read event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'descriptorRead'} type - Type of the descriptor read event to listen for.
         * @param { Callback<DescriptorReadRequest>} callback - Callback used to listen for the descriptor read event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Subscribe descriptor read event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'descriptorRead'} type - Type of the descriptor read event to listen for.
         * @param { Callback<DescriptorReadRequest>} callback - Callback used to listen for the descriptor read event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Subscribe descriptor read event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'descriptorRead'} type - Type of the descriptor read event to listen for.
         * @param { Callback<DescriptorReadRequest>} callback - Callback used to listen for the descriptor read event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        on(type: 'descriptorRead', callback: Callback<DescriptorReadRequest>): void;
        /**
         * Unsubscribe descriptor read event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'descriptorRead'} type - Type of the descriptor read event to listen for.
         * @param { Callback<DescriptorReadRequest>} callback - Callback used to listen for the descriptor read event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Unsubscribe descriptor read event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'descriptorRead'} type - Type of the descriptor read event to listen for.
         * @param { Callback<DescriptorReadRequest>} callback - Callback used to listen for the descriptor read event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Unsubscribe descriptor read event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'descriptorRead'} type - Type of the descriptor read event to listen for.
         * @param { Callback<DescriptorReadRequest>} callback - Callback used to listen for the descriptor read event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        off(type: 'descriptorRead', callback?: Callback<DescriptorReadRequest>): void;
        /**
         * Subscribe descriptor write event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'descriptorWrite'} type - Type of the descriptor write event to listen for.
         * @param { Callback<DescriptorWriteRequest>} callback - Callback used to listen for the descriptor write event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Subscribe descriptor write event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'descriptorWrite'} type - Type of the descriptor write event to listen for.
         * @param { Callback<DescriptorWriteRequest>} callback - Callback used to listen for the descriptor write event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Subscribe descriptor write event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'descriptorWrite'} type - Type of the descriptor write event to listen for.
         * @param { Callback<DescriptorWriteRequest>} callback - Callback used to listen for the descriptor write event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        on(type: 'descriptorWrite', callback: Callback<DescriptorWriteRequest>): void;
        /**
         * Unsubscribe descriptor write event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'descriptorWrite'} type - Type of the descriptor write event to listen for.
         * @param { Callback<DescriptorWriteRequest>} callback - Callback used to listen for the descriptor write event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Unsubscribe descriptor write event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'descriptorWrite'} type - Type of the descriptor write event to listen for.
         * @param { Callback<DescriptorWriteRequest>} callback - Callback used to listen for the descriptor write event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Unsubscribe descriptor write event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'descriptorWrite'} type - Type of the descriptor write event to listen for.
         * @param { Callback<DescriptorWriteRequest>} callback - Callback used to listen for the descriptor write event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        off(type: 'descriptorWrite', callback?: Callback<DescriptorWriteRequest>): void;
        /**
         * Subscribe server connection state changed event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'connectionStateChange'} type - Type of the connection state changed event to listen for.
         * @param { Callback<BLEConnectionChangeState>} callback - Callback used to listen for the connection state changed event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Subscribe server connection state changed event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'connectionStateChange'} type - Type of the connection state changed event to listen for.
         * @param { Callback<BLEConnectionChangeState>} callback - Callback used to listen for the connection state changed event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Subscribe server connection state changed event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'connectionStateChange'} type - Type of the connection state changed event to listen for.
         * @param { Callback<BLEConnectionChangeState>} callback - Callback used to listen for the connection state changed event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        on(type: 'connectionStateChange', callback: Callback<BLEConnectionChangeState>): void;
        /**
         * Unsubscribe server connection state changed event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'connectionStateChange'} type - Type of the connection state changed event to listen for.
         * @param { Callback<BLEConnectionChangeState>} callback - Callback used to listen for the connection state changed event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Unsubscribe server connection state changed event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'connectionStateChange'} type - Type of the connection state changed event to listen for.
         * @param { Callback<BLEConnectionChangeState>} callback - Callback used to listen for the connection state changed event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Unsubscribe server connection state changed event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'connectionStateChange'} type - Type of the connection state changed event to listen for.
         * @param { Callback<BLEConnectionChangeState>} callback - Callback used to listen for the connection state changed event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        off(type: 'connectionStateChange', callback?: Callback<BLEConnectionChangeState>): void;
        /**
         * Subscribe mtu changed event.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'BLEMtuChange'} type - Type of the mtu changed event to listen for.
         * @param { Callback<number>} callback - Callback used to listen for the mtu changed event.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
