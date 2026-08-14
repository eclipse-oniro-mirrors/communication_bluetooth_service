/*
 * Copyright (C) 2022-2023 Huawei Device Co., Ltd.
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
/**
 * Provides methods to operate or manage Bluetooth.
 *
 * @namespace bluetoothManager
 * @syscap SystemCapability.Communication.Bluetooth.Core
 * @since 9
 * @deprecated since 10
 */
declare namespace bluetoothManager {
    /**
     * Obtains the Bluetooth status of a device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @returns { BluetoothState} Returns the Bluetooth status, which can be {@link BluetoothState#STATE_OFF},
     * {@link BluetoothState#STATE_TURNING_ON}, {@link BluetoothState#STATE_ON}, {@link BluetoothState#STATE_TURNING_OFF},
     * {@link BluetoothState#STATE_BLE_TURNING_ON}, {@link BluetoothState#STATE_BLE_ON},
     * or {@link BluetoothState#STATE_BLE_TURNING_OFF}.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.access/access#getState
     */
    /**
     * Obtains the Bluetooth status of a device.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @returns { BluetoothState} Returns the Bluetooth status, which can be {@link BluetoothState#STATE_OFF},
     * {@link BluetoothState#STATE_TURNING_ON}, {@link BluetoothState#STATE_ON}, {@link BluetoothState#STATE_TURNING_OFF},
     * {@link BluetoothState#STATE_BLE_TURNING_ON}, {@link BluetoothState#STATE_BLE_ON},
     * or {@link BluetoothState#STATE_BLE_TURNING_OFF}.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.access/access#getState
     */
    function getState(): BluetoothState;
    /**
     * Get the local device connection state to any profile of any remote device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @returns { ProfileConnectionState} One of {@link ProfileConnectionState#STATE_DISCONNECTED},
     * {@link ProfileConnectionState#STATE_CONNECTING}, {@link ProfileConnectionState#STATE_CONNECTED},
     * {@link ProfileConnectionState#STATE_DISCONNECTING}.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#getProfileConnectionState
     */
    /**
     * Get the local device connection state to any profile of any remote device.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @returns { ProfileConnectionState} One of {@link ProfileConnectionState#STATE_DISCONNECTED},
     * {@link ProfileConnectionState#STATE_CONNECTING}, {@link ProfileConnectionState#STATE_CONNECTED},
     * {@link ProfileConnectionState#STATE_DISCONNECTING}.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#getProfileConnectionState
     */
    function getBtConnectionState(): ProfileConnectionState;
    /**
     * Starts pairing with a remote Bluetooth device.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @param { string} deviceId - The address of the remote device to pair.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#pairDevice
     */
    /**
     * Starts pairing with a remote Bluetooth device.
     * The permission required by this interface is changed from DISCOVER_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { string} deviceId - The address of the remote device to pair.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#pairDevice
     */
    function pairDevice(deviceId: string): void;
    /**
     * Remove a paired remote device.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @param { string} deviceId - The address of the remote device to be removed.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 202 - Non-system applications are not allowed to use system APIs.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#cancelPairedDevice
     */
    /**
     * Remove a paired remote device.
     * The permission required by this interface is changed from DISCOVER_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { string} deviceId - The address of the remote device to be removed.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 202 - Non-system applications are not allowed to use system APIs.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#cancelPairedDevice
     */
    function cancelPairedDevice(deviceId: string): void;
    /**
     * Obtains the name of a peer Bluetooth device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { string} deviceId - The address of the remote device.
     * @returns { string} Returns the device name in character string format.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#getRemoteDeviceName
     */
    /**
     * Obtains the name of a peer Bluetooth device.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { string} deviceId - The address of the remote device.
     * @returns { string} Returns the device name in character string format.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#getRemoteDeviceName
     */
    function getRemoteDeviceName(deviceId: string): string;
    /**
     * Obtains the class of a peer Bluetooth device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { string} deviceId - The address of the remote device.
     * @returns { DeviceClass} The class of the remote device, {@link DeviceClass}.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#getRemoteDeviceClass
     */
    /**
     * Obtains the class of a peer Bluetooth device.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { string} deviceId - The address of the remote device.
     * @returns { DeviceClass} The class of the remote device, {@link DeviceClass}.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#getRemoteDeviceClass
     */
    function getRemoteDeviceClass(deviceId: string): DeviceClass;
    /**
     * Enables Bluetooth on a device.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.access/access#enableBluetooth
     */
    /**
     * Enables Bluetooth on a device.
     * The permission required by this interface is changed from DISCOVER_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.access/access#enableBluetooth
     */
    function enableBluetooth(): void;
    /**
     * Disables Bluetooth on a device.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.access/access#disableBluetooth
     */
    /**
     * Disables Bluetooth on a device.
     * The permission required by this interface is changed from DISCOVER_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.access/access#disableBluetooth
     */
    function disableBluetooth(): void;
    /**
     * Obtains the Bluetooth local name of a device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @returns { string} Returns the name the device.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#getLocalName
     */
    /**
     * Obtains the Bluetooth local name of a device.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @returns { string} Returns the name the device.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#getLocalName
     */
    function getLocalName(): string;
    /**
     * Obtains the list of Bluetooth devices that have been paired with the current device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @returns { Array<string>} Returns a list of paired Bluetooth devices's address.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#getPairedDevices
     */
    /**
     * Obtains the list of Bluetooth devices that have been paired with the current device.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @returns { Array<string>} Returns a list of paired Bluetooth devices's address.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#getPairedDevices
     */
    function getPairedDevices(): Array<string>;
    /**
     * Obtains the connection state of profile.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { ProfileId} profileId - The profile id.
     * @returns { ProfileConnectionState} Returns the connection state.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900004 - Profile not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#getProfileConnectionState
     */
    /**
     * Obtains the connection state of profile.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { ProfileId} profileId - The profile id.
     * @returns { ProfileConnectionState} Returns the connection state.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900004 - Profile not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#getProfileConnectionState
     */
    function getProfileConnectionState(profileId: ProfileId): ProfileConnectionState;
    /**
     * Sets the confirmation of pairing with a certain device.
     *
     * @permission ohos.permission.MANAGE_BLUETOOTH
     * @param { string} device - The address of the remote device.
     * @param { boolean} accept - Indicates whether to accept the pairing request, {@code true} indicates accept or {@code false} otherwise.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#setDevicePairingConfirmation
     */
    /**
     * Sets the confirmation of pairing with a certain device.
     * The permission required by this interface is changed from MANAGE_BLUETOOTH to ACCESS_BLUETOOTH and MANAGE_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
     * @param { string} device - The address of the remote device.
     * @param { boolean} accept - Indicates whether to accept the pairing request, {@code true} indicates accept or {@code false} otherwise.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#setDevicePairingConfirmation
     */
    function setDevicePairingConfirmation(device: string, accept: boolean): void;
    /**
     * Sets the Bluetooth friendly name of a device.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @param { string} name - Indicates a valid Bluetooth name.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#setLocalName
     */
    /**
     * Sets the Bluetooth friendly name of a device.
     * The permission required by this interface is changed from DISCOVER_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { string} name - Indicates a valid Bluetooth name.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#setLocalName
     */
    function setLocalName(name: string): void;
    /**
     * Sets the Bluetooth scan mode for a device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { ScanMode} mode - Indicates the Bluetooth scan mode to set, {@link ScanMode}.
     * @param { number} duration - Indicates the duration in seconds, in which the host is discoverable.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#setBluetoothScanMode
     */
    /**
     * Sets the Bluetooth scan mode for a device.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { ScanMode} mode - Indicates the Bluetooth scan mode to set, {@link ScanMode}.
     * @param { number} duration - Indicates the duration in seconds, in which the host is discoverable.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#setBluetoothScanMode
     */
    function setBluetoothScanMode(mode: ScanMode, duration: number): void;
    /**
     * Obtains the Bluetooth scanning mode of a device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @returns { ScanMode} Returns the Bluetooth scanning mode, {@link ScanMode}.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#getBluetoothScanMode
     */
    /**
     * Obtains the Bluetooth scanning mode of a device.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @returns { ScanMode} Returns the Bluetooth scanning mode, {@link ScanMode}.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#getBluetoothScanMode
     */
    function getBluetoothScanMode(): ScanMode;
    /**
     * Starts scanning Bluetooth devices.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH and ohos.permission.LOCATION
     *     and ohos.permission.APPROXIMATELY_LOCATION
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#startBluetoothDiscovery
     */
    /**
     * Starts scanning Bluetooth devices.
     * The permission required by this interface is changed from DISCOVER_BLUETOOTH and LOCATION and APPROXIMATELY_LOCATION to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#startBluetoothDiscovery
     */
    function startBluetoothDiscovery(): void;
    /**
     * Stops Bluetooth device scanning.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#stopBluetoothDiscovery
     */
    /**
     * Stops Bluetooth device scanning.
     * The permission required by this interface is changed from DISCOVER_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection#stopBluetoothDiscovery
     */
    function stopBluetoothDiscovery(): void;
    /**
     * Subscribe the event reported when a remote Bluetooth device is discovered.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { 'bluetoothDeviceFind'} type - Type of the discovering event to listen for.
     * @param { Callback<Array<string>>} callback - Callback used to listen for the discovering event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.on#event:bluetoothDeviceFind
     */
    /**
     * Subscribe the event reported when a remote Bluetooth device is discovered.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'bluetoothDeviceFind'} type - Type of the discovering event to listen for.
     * @param { Callback<Array<string>>} callback - Callback used to listen for the discovering event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.on#event:bluetoothDeviceFind
     */
    function on(type: 'bluetoothDeviceFind', callback: Callback<Array<string>>): void;
    /**
     * Unsubscribe the event reported when a remote Bluetooth device is discovered.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { 'bluetoothDeviceFind'} type - Type of the discovering event to listen for.
     * @param { Callback<Array<string>>} callback - Callback used to listen for the discovering event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.off#event:bluetoothDeviceFind
     */
    /**
     * Unsubscribe the event reported when a remote Bluetooth device is discovered.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'bluetoothDeviceFind'} type - Type of the discovering event to listen for.
     * @param { Callback<Array<string>>} callback - Callback used to listen for the discovering event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.off#event:bluetoothDeviceFind
     */
    function off(type: 'bluetoothDeviceFind', callback?: Callback<Array<string>>): void;
    /**
     * Subscribe the event reported when a remote Bluetooth device is bonded.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { 'bondStateChange'} type - Type of the bond state event to listen for.
     * @param { Callback<BondStateParam>} callback - Callback used to listen for the bond state event, {@link BondStateParam}.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.on#event:bondStateChange
     */
    /**
     * Subscribe the event reported when a remote Bluetooth device is bonded.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'bondStateChange'} type - Type of the bond state event to listen for.
     * @param { Callback<BondStateParam>} callback - Callback used to listen for the bond state event, {@link BondStateParam}.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.on#event:bondStateChange
     */
    function on(type: 'bondStateChange', callback: Callback<BondStateParam>): void;
    /**
     * Unsubscribe the event reported when a remote Bluetooth device is bonded.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { 'bondStateChange'} type - Type of the bond state event to listen for.
     * @param { Callback<BondStateParam>} callback - Callback used to listen for the bond state event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.off#event:bondStateChange
     */
    /**
     * Unsubscribe the event reported when a remote Bluetooth device is bonded.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'bondStateChange'} type - Type of the bond state event to listen for.
     * @param { Callback<BondStateParam>} callback - Callback used to listen for the bond state event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.off#event:bondStateChange
     */
    function off(type: 'bondStateChange', callback?: Callback<BondStateParam>): void;
    /**
     * Subscribe the event of a pairing request from a remote Bluetooth device.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @param { 'pinRequired'} type - Type of the pairing request event to listen for.
     * @param { Callback<PinRequiredParam>} callback - Callback used to listen for the pairing request event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.on#event:pinRequired
     */
    /**
     * Subscribe the event of a pairing request from a remote Bluetooth device.
     * The permission required by this interface is changed from DISCOVER_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'pinRequired'} type - Type of the pairing request event to listen for.
     * @param { Callback<PinRequiredParam>} callback - Callback used to listen for the pairing request event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.on#event:pinRequired
     */
    function on(type: 'pinRequired', callback: Callback<PinRequiredParam>): void;
    /**
     * Unsubscribe the event of a pairing request from a remote Bluetooth device.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @param { 'pinRequired'} type - Type of the pairing request event to listen for.
     * @param { Callback<PinRequiredParam>} callback - Callback used to listen for the pairing request event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.off#event:pinRequired
     */
    /**
     * Unsubscribe the event of a pairing request from a remote Bluetooth device.
     * The permission required by this interface is changed from DISCOVER_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'pinRequired'} type - Type of the pairing request event to listen for.
     * @param { Callback<PinRequiredParam>} callback - Callback used to listen for the pairing request event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.off#event:pinRequired
     */
    function off(type: 'pinRequired', callback?: Callback<PinRequiredParam>): void;
    /**
     * Subscribe the event reported when the Bluetooth state changes.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { 'stateChange'} type - Type of the Bluetooth state changes event to listen for.
     * @param { Callback<BluetoothState>} callback - Callback used to listen for the Bluetooth state event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.access/access.on#event:stateChange
     */
    /**
     * Subscribe the event reported when the Bluetooth state changes.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'stateChange'} type - Type of the Bluetooth state changes event to listen for.
     * @param { Callback<BluetoothState>} callback - Callback used to listen for the Bluetooth state event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.access/access.on#event:stateChange
     */
    function on(type: 'stateChange', callback: Callback<BluetoothState>): void;
    /**
     * Unsubscribe the event reported when the Bluetooth state changes.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { 'stateChange'} type - Type of the Bluetooth state changes event to listen for.
     * @param { Callback<BluetoothState>} callback - Callback used to listen for the Bluetooth state event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.access/access.off#event:stateChange
     */
    /**
     * Unsubscribe the event reported when the Bluetooth state changes.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { 'stateChange'} type - Type of the Bluetooth state changes event to listen for.
     * @param { Callback<BluetoothState>} callback - Callback used to listen for the Bluetooth state event.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.access/access.off#event:stateChange
     */
    function off(type: 'stateChange', callback?: Callback<BluetoothState>): void;
    /**
     * Creates a Bluetooth server listening socket.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { string} name - Indicates the service name.
     * @param { SppOption} option - Indicates the listen parameters {@link SppOption}.
     * @param { AsyncCallback<number>} callback - Callback used to return a server socket ID.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900004 - Profile not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.socket/socket#sppListen
     */
    /**
     * Creates a Bluetooth server listening socket.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { string} name - Indicates the service name.
     * @param { SppOption} option - Indicates the listen parameters {@link SppOption}.
     * @param { AsyncCallback<number>} callback - Callback used to return a server socket ID.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900004 - Profile not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.socket/socket#sppListen
     */
    function sppListen(name: string, option: SppOption, callback: AsyncCallback<number>): void;
    /**
     * Waits for a remote device to connect.
     *
     * @param { number} serverSocket - Indicates the server socket ID, returned by {@link sppListen}.
     * @param { AsyncCallback<number>} callback - Callback used to return a client socket ID.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900004 - Profile not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.socket/socket#sppAccept
     */
    function sppAccept(serverSocket: number, callback: AsyncCallback<number>): void;
    /**
     * Connects to a remote device over the socket.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { string} device - The address of the remote device to connect.
     * @param { SppOption} option - Indicates the connect parameters {@link SppOption}.
     * @param { AsyncCallback<number>} callback - Callback used to return a client socket ID.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900004 - Profile not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.socket/socket#sppConnect
     */
    /**
     * Connects to a remote device over the socket.
     * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH
     * @param { string} device - The address of the remote device to connect.
     * @param { SppOption} option - Indicates the connect parameters {@link SppOption}.
     * @param { AsyncCallback<number>} callback - Callback used to return a client socket ID.
     * @throws { BusinessError} 201 - Permission denied.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900003 - Bluetooth disabled.
     * @throws { BusinessError} 2900004 - Profile not supported.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     * @deprecated since 10
     * @useinstead ohos.bluetooth.socket/socket#sppConnect
     */
    function sppConnect(device: string, option: SppOption, callback: AsyncCallback<number>): void;
    /**
     * Disables an spp server socket and releases related resources.
     *
     * @param { number} socket - Indicates the server socket ID, returned by {@link sppListen}.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.socket/socket#sppCloseServerSocket
     */
    function sppCloseServerSocket(socket: number): void;
    /**
     * Disables an spp client socket and releases related resources.
     *
     * @param { number} socket - Indicates the client socket ID, returned by {@link sppAccept} or {@link sppConnect}.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2900001 - Service stopped.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.socket/socket#sppCloseClientSocket
     */
    function sppCloseClientSocket(socket: number): void;
    /**
     * Write data through the socket.
     *
     * @param { number} clientSocket - Indicates the client socket ID, returned by {@link sppAccept} or {@link sppConnect}.
     * @param { ArrayBuffer} data - Indicates the data to write.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2901054 - IO error.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.socket/socket#sppWrite
     */
    function sppWrite(clientSocket: number, data: ArrayBuffer): void;
    /**
     * Subscribe the event reported when data is read from the socket.
     *
     * @param { 'sppRead'} type - Type of the spp read event to listen for.
     * @param { number} clientSocket - Client socket ID, which is obtained by sppAccept or sppConnect.
     * @param { Callback<ArrayBuffer>} callback - Callback used to listen for the spp read event.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @throws { BusinessError} 2901054 - IO error.
     * @throws { BusinessError} 2900099 - Operation failed.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.socket/socket.on#event:sppRead
     */
    function on(type: 'sppRead', clientSocket: number, callback: Callback<ArrayBuffer>): void;
    /**
     * Unsubscribe the event reported when data is read from the socket.
     *
     * @param { 'sppRead'} type - Type of the spp read event to listen for.
     * @param { number} clientSocket - Client socket ID, which is obtained by sppAccept or sppConnect.
     * @param { Callback<ArrayBuffer>} callback - Callback used to listen for the spp read event.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.socket/socket.off#event:sppRead
     */
    function off(type: 'sppRead', clientSocket: number, callback?: Callback<ArrayBuffer>): void;
    /**
     * Obtains the instance of profile.
     *
     * @param { ProfileId} profileId - The profile id..
     * @returns { A2dpSourceProfile | HandsFreeAudioGatewayProfile | HidHostProfile | PanProfile} Returns the instance of profile.
     * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError} 801 - Capability not supported.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     */
    function getProfileInstance(profileId: ProfileId): A2dpSourceProfile | HandsFreeAudioGatewayProfile | HidHostProfile | PanProfile;
    /**
     * Base interface of profile.
     *
     * @typedef BaseProfile
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.baseProfile/baseProfile.BaseProfile
     */
    interface BaseProfile {
        /**
         * Obtains the connected devices list of profile.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @returns { Array<string>} Returns the address of connected devices list.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.baseProfile/baseProfile#getConnectedDevices
         */
        /**
         * Obtains the connected devices list of profile.
         * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @returns { Array<string>} Returns the address of connected devices list.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         * @deprecated since 10
         * @useinstead ohos.bluetooth.baseProfile/baseProfile#getConnectedDevices
         */
        getConnectionDevices(): Array<string>;
        /**
         * Obtains the profile state of device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { string} device - The address of bluetooth device.
         * @returns { ProfileConnectionState} Returns {@link ProfileConnectionState} of device.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.baseProfile/baseProfile#getConnectionState
         */
        /**
         * Obtains the profile state of device.
         * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} device - The address of bluetooth device.
         * @returns { ProfileConnectionState} Returns {@link ProfileConnectionState} of device.
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
         * @deprecated since 10
         * @useinstead ohos.bluetooth.baseProfile/baseProfile#getConnectionState
         */
        getDeviceState(device: string): ProfileConnectionState;
    }
    /**
     * Manager a2dp source profile.
     *
     * @typedef A2dpSourceProfile
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.a2dp/a2dp.A2dpSourceProfile
     */
    interface A2dpSourceProfile extends BaseProfile {
        /**
         * Connect to device with a2dp.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH
         * @param { string} device - The address of the remote device to connect.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.a2dp/a2dp.A2dpSourceProfile#connect
         */
        /**
         * Connect to device with a2dp.
         * The permission required by this interface is changed from DISCOVER_BLUETOOTH to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} device - The address of the remote device to connect.
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
         * @deprecated since 10
         * @useinstead ohos.bluetooth.a2dp/a2dp.A2dpSourceProfile#connect
         */
        connect(device: string): void;
        /**
         * Disconnect to device with a2dp.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH
         * @param { string} device - The address of the remote device to disconnect.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.a2dp/a2dp.A2dpSourceProfile#disconnect
         */
        /**
         * Disconnect to device with a2dp.
         * The permission required by this interface is changed from DISCOVER_BLUETOOTH to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} device - The address of the remote device to disconnect.
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
         * @deprecated since 10
         * @useinstead ohos.bluetooth.a2dp/a2dp.A2dpSourceProfile#disconnect
         */
        disconnect(device: string): void;
        /**
         * Subscribe the event reported when the profile connection state changes .
         *
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.a2dp/a2dp.A2dpSourceProfile.on#event:connectionStateChange
         */
        /**
         * Subscribe the event reported when the profile connection state changes.
         * The permission required by this interface is changed to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         * @deprecated since 10
         * @useinstead ohos.bluetooth.a2dp/a2dp.A2dpSourceProfile.on#event:connectionStateChange
         */
        on(type: 'connectionStateChange', callback: Callback<StateChangeParam>): void;
        /**
         * Unsubscribe the event reported when the profile connection state changes .
         *
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.a2dp/a2dp.A2dpSourceProfile.off#event:connectionStateChange
         */
        /**
         * Unsubscribe the event reported when the profile connection state changes.
         * The permission required by this interface is changed to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         * @deprecated since 10
         * @useinstead ohos.bluetooth.a2dp/a2dp.A2dpSourceProfile.off#event:connectionStateChange
         */
        off(type: 'connectionStateChange', callback?: Callback<StateChangeParam>): void;
        /**
         * Obtains the playing state of device.
         *
         * @param { string} device - The address of the remote device.
         * @returns { PlayingState} Returns {@link PlayingState} of the remote device.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.a2dp/a2dp.A2dpSourceProfile#getPlayingState
         */
        /**
         * Obtains the playing state of device.
         * The permission required by this interface is changed to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} device - The address of the remote device.
         * @returns { PlayingState} Returns {@link PlayingState} of the remote device.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         * @deprecated since 10
         * @useinstead ohos.bluetooth.a2dp/a2dp.A2dpSourceProfile#getPlayingState
         */
        getPlayingState(device: string): PlayingState;
    }
    /**
     * Manager handsfree AG profile.
     *
     * @typedef HandsFreeAudioGatewayProfile
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.hfp/hfp.HandsFreeAudioGatewayProfile
     */
    interface HandsFreeAudioGatewayProfile extends BaseProfile {
        /**
         * Connect to device with hfp.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH
         * @param { string} device - The address of the remote device to connect.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hfp/hfp.HandsFreeAudioGatewayProfile#connect
         */
        /**
         * Connect to device with hfp.
         * The permission required by this interface is changed from DISCOVER_BLUETOOTH to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} device - The address of the remote device to connect.
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
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hfp/hfp.HandsFreeAudioGatewayProfile#connect
         */
        connect(device: string): void;
        /**
         * Disconnect to device with hfp.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH
         * @param { string} device - The address of the remote device to disconnect.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hfp/hfp.HandsFreeAudioGatewayProfile#disconnect
         */
        /**
         * Disconnect to device with hfp.
         * The permission required by this interface is changed from DISCOVER_BLUETOOTH to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} device - The address of the remote device to disconnect.
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
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hfp/hfp.HandsFreeAudioGatewayProfile#disconnect
         */
        disconnect(device: string): void;
        /**
         * Subscribe the event reported when the profile connection state changes .
         *
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hfp/hfp.HandsFreeAudioGatewayProfile.on#event:connectionStateChange
         */
        /**
         * Subscribe the event reported when the profile connection state changes.
         * The permission required by this interface is changed to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hfp/hfp.HandsFreeAudioGatewayProfile.on#event:connectionStateChange
         */
        on(type: 'connectionStateChange', callback: Callback<StateChangeParam>): void;
        /**
         * Unsubscribe the event reported when the profile connection state changes .
         *
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hfp/hfp.HandsFreeAudioGatewayProfile.off#event:connectionStateChange
         */
        /**
         * Unsubscribe the event reported when the profile connection state changes.
         * The permission required by this interface is changed to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hfp/hfp.HandsFreeAudioGatewayProfile.off#event:connectionStateChange
         */
        off(type: 'connectionStateChange', callback?: Callback<StateChangeParam>): void;
    }
    /**
     * Manager hid host profile.
     *
     * @typedef HidHostProfile
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.hid/hid.HidHostProfile
     */
    interface HidHostProfile extends BaseProfile {
        /**
         * Connect to device with hid host.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH
         * @param { string} device - The address of the remote device to connect.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 202 - Non-system applications are not allowed to use system APIs.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi Hide this for inner system use.
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hid/hid.HidHostProfile#connect
         */
        /**
         * Connect to device with hid host.
         * The permission required by this interface is changed from DISCOVER_BLUETOOTH to ACCESS_BLUETOOTH and MANAGE_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
         * @param { string} device - The address of the remote device to connect.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 202 - Non-system applications are not allowed to use system APIs.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi Hide this for inner system use.
         * @since 10
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hid/hid.HidHostProfile#connect
         */
        connect(device: string): void;
        /**
         * Disconnect to device with hid host.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH
         * @param { string} device - The address of the remote device to disconnect.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 202 - Non-system applications are not allowed to use system APIs.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi Hide this for inner system use.
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hid/hid.HidHostProfile#disconnect
         */
        /**
         * Disconnect to device with hid host.
         * The permission required by this interface is changed from DISCOVER_BLUETOOTH to ACCESS_BLUETOOTH and MANAGE_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
         * @param { string} device - The address of the remote device to disconnect.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 202 - Non-system applications are not allowed to use system APIs.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi Hide this for inner system use.
         * @since 10
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hid/hid.HidHostProfile#disconnect
         */
        disconnect(device: string): void;
        /**
         * Subscribe the event reported when the profile connection state changes .
         *
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hid/hid.HidHostProfile.on#event:connectionStateChange
         */
        /**
         * Subscribe the event reported when the profile connection state changes.
         * The permission required by this interface is changed to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hid/hid.HidHostProfile.on#event:connectionStateChange
         */
        on(type: 'connectionStateChange', callback: Callback<StateChangeParam>): void;
        /**
         * Unsubscribe the event reported when the profile connection state changes.
         *
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for.
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hid/hid.HidHostProfile.off#event:connectionStateChange
         */
        /**
         * Unsubscribe the event reported when the profile connection state changes.
         * The permission required by this interface is changed to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for.
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         * @deprecated since 10
         * @useinstead ohos.bluetooth.hid/hid.HidHostProfile.off#event:connectionStateChange
         */
        off(type: 'connectionStateChange', callback?: Callback<StateChangeParam>): void;
    }
    /**
     * Manager pan profile.
     *
     * @typedef PanProfile
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.pan/pan.PanProfile
     */
    interface PanProfile extends BaseProfile {
        /**
         * Disconnect to device with pan.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { string} device - The address of the remote device to disconnect.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 202 - Non-system applications are not allowed to use system APIs.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi Hide this for inner system use.
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.pan/pan.PanProfile#disconnect
         */
        /**
         * Disconnect to device with pan.
         * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { string} device - The address of the remote device to disconnect.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 202 - Non-system applications are not allowed to use system APIs.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi Hide this for inner system use.
         * @since 10
         * @deprecated since 10
         * @useinstead ohos.bluetooth.pan/pan.PanProfile#disconnect
         */
        disconnect(device: string): void;
        /**
         * Subscribe the event reported when the profile connection state changes .
         *
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.pan/pan.PanProfile.on#event:connectionStateChange
         */
        /**
         * Subscribe the event reported when the profile connection state changes.
         * The permission required by this interface is changed to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         * @deprecated since 10
         * @useinstead ohos.bluetooth.pan/pan.PanProfile.on#event:connectionStateChange
         */
        on(type: 'connectionStateChange', callback: Callback<StateChangeParam>): void;
        /**
         * Unsubscribe the event reported when the profile connection state changes.
         *
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for.
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.pan/pan.PanProfile.off#event:connectionStateChange
         */
        /**
         * Unsubscribe the event reported when the profile connection state changes.
         * The permission required by this interface is changed to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for.
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         * @deprecated since 10
         * @useinstead ohos.bluetooth.pan/pan.PanProfile.off#event:connectionStateChange
         */
        off(type: 'connectionStateChange', callback?: Callback<StateChangeParam>): void;
        /**
         * Enable bluetooth tethering.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH
         * @param { boolean} enable - Specifies whether to enable tethering. The value {@code true} indicates
         * that tethering is enabled, and the value {@code false} indicates that tethering is disabled.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 202 - Non-system applications are not allowed to use system APIs.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi Hide this for inner system use.
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.pan/pan.PanProfile#setTethering
         */
        /**
         * Enable bluetooth tethering.
         * The permission required by this interface is changed from DISCOVER_BLUETOOTH to ACCESS_BLUETOOTH and MANAGE_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
         * @param { boolean} enable - Specifies whether to enable tethering. The value {@code true} indicates
         * that tethering is enabled, and the value {@code false} indicates that tethering is disabled.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 202 - Non-system applications are not allowed to use system APIs.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900004 - Profile not supported.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi Hide this for inner system use.
         * @since 10
         * @deprecated since 10
         * @useinstead ohos.bluetooth.pan/pan.PanProfile#setTethering
         */
        setTethering(enable: boolean): void;
        /**
         * Obtains the tethering enable or disable.
         *
         * @returns { boolean} Returns the value {@code true} is tethering is on, returns {@code false} otherwise.
         * @throws { BusinessError} 202 - Non-system applications are not allowed to use system APIs.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi Hide this for inner system use.
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.pan/pan.PanProfile#isTetheringOn
         */
        /**
         * Obtains the tethering enable or disable.
         * The permission required by this interface is changed to ACCESS_BLUETOOTH.
         *
         * @permission ohos.permission.ACCESS_BLUETOOTH
         * @returns { boolean} Returns the value {@code true} is tethering is on, returns {@code false} otherwise.
         * @throws { BusinessError} 202 - Non-system applications are not allowed to use system APIs.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi Hide this for inner system use.
         * @since 10
         * @deprecated since 10
         * @useinstead ohos.bluetooth.pan/pan.PanProfile#isTetheringOn
         */
        isTetheringOn(): boolean;
    }
    /**
     * Provides methods to operate or manage Bluetooth.
     *
     * @namespace BLE
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.ble/ble
     */
    namespace BLE {
        /**
         * create a JavaScript Gatt server instance.
         *
         * @returns { GattServer} Returns a JavaScript Gatt server instance {@code GattServer}.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble#createGattServer
         */
        function createGattServer(): GattServer;
        /**
         * create a JavaScript Gatt client device instance.
         *
         * @param { string} deviceId - The address of the remote device.
         * @returns { GattClientDevice} Returns a JavaScript Gatt client device instance {@code GattClientDevice}.
         * @throws { BusinessError} 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
         * <br>2. Incorrect parameter types. 3. Parameter verification failed.
         * @throws { BusinessError} 801 - Capability not supported.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble#createGattClientDevice
         */
        function createGattClientDevice(deviceId: string): GattClientDevice;
        /**
         * Obtains the list of devices in the connected status.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @returns { Array<string>} Returns the list of device address.
         * @throws { BusinessError} 201 - Permission denied.
         * @throws { BusinessError} 801 - Capability not supported.
         * @throws { BusinessError} 2900001 - Service stopped.
         * @throws { BusinessError} 2900003 - Bluetooth disabled.
         * @throws { BusinessError} 2900099 - Operation failed.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble#getConnectedBLEDevices
         */
        /**
         * Obtains the list of devices in the connected status.
         * The permission required by this interface is changed from USE_BLUETOOTH to ACCESS_BLUETOOTH.
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
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble#getConnectedBLEDevices
         */
        function getConnectedBLEDevices(): Array<string>;
        /**
         * Starts scanning for specified BLE devices with filters.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH and ohos.permission.LOCATION
         *     and ohos.permission.APPROXIMATELY_LOCATION
         * @param { Array<ScanFilter>} filters - Indicates the list of filters used to filter out specified devices.
         * If you do not want to use filter, set this parameter to {@code null}.
         * @param { ScanOptions} options - Indicates the parameters for scanning and if the user does not assign a value, the default value will be used.
         * {@link ScanOptions#interval} set to 0, {@link ScanOptions#dutyMode} set to {@link SCAN_MODE_LOW_POWER}
         * and {@link ScanOptions#matchMode} set to {@link MATCH_MODE_AGGRESSIVE}.
         * @throws { BusinessError} 201 - Permission denied.
