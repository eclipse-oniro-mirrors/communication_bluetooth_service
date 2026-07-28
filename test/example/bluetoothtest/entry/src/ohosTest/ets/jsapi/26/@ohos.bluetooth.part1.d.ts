/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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
 * @namespace bluetooth
 * @syscap SystemCapability.Communication.Bluetooth.Core
 * @since 7
 * @deprecated since 9
 * @useinstead ohos.bluetoothManager
 */
declare namespace bluetooth {
    /**
     * Obtains the Bluetooth status of a device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @returns { BluetoothState} Returns the Bluetooth status, which can be {@link BluetoothState#STATE_OFF},
     * {@link BluetoothState#STATE_TURNING_ON}, {@link BluetoothState#STATE_ON}, {@link BluetoothState#STATE_TURNING_OFF},
     * {@link BluetoothState#STATE_BLE_TURNING_ON}, {@link BluetoothState#STATE_BLE_ON},
     * or {@link BluetoothState#STATE_BLE_TURNING_OFF}.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.getState
     */
    function getState(): BluetoothState;
    /**
     * Get the local device connection state to any profile of any remote device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @returns { ProfileConnectionState} One of {@link ProfileConnectionState#STATE_DISCONNECTED},
     * {@link ProfileConnectionState#STATE_CONNECTING}, {@link ProfileConnectionState#STATE_CONNECTED},
     * {@link ProfileConnectionState#STATE_DISCONNECTING}.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.getBtConnectionState
     */
    function getBtConnectionState(): ProfileConnectionState;
    /**
     * Starts pairing with a remote Bluetooth device.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @param { string} deviceId - The address of the remote device to pair.
     * @returns { boolean} Returns {@code true} if the pairing process is started; returns {@code false} otherwise.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.pairDevice
     */
    function pairDevice(deviceId: string): boolean;
    /**
     * Remove a paired remote device.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @param { string} deviceId - The address of the remote device to be removed.
     * @returns { boolean} Returns {@code true} if the cancel process is started; returns {@code false} otherwise.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi Hide this for inner system use
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.cancelPairedDevice
     */
    function cancelPairedDevice(deviceId: string): boolean;
    /**
     * Obtains the name of a peer Bluetooth device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { string} deviceId - The address of the remote device.
     * @returns { string} Returns the device name in character string format.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.getRemoteDeviceName
     */
    function getRemoteDeviceName(deviceId: string): string;
    /**
     * Obtains the class of a peer Bluetooth device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { string} deviceId - The address of the remote device.
     * @returns { DeviceClass} The class of the remote device, {@link DeviceClass}.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.getRemoteDeviceClass
     */
    function getRemoteDeviceClass(deviceId: string): DeviceClass;
    /**
     * Enables Bluetooth on a device.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @returns { boolean} Returns {@code true} if Bluetooth is being enabled; returns {@code false} if an error occurs.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.enableBluetooth
     */
    function enableBluetooth(): boolean;
    /**
     * Disables Bluetooth on a device.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @returns { boolean} Returns {@code true} if Bluetooth is being disabled; returns {@code false} if an error occurs.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.disableBluetooth
     */
    function disableBluetooth(): boolean;
    /**
     * Obtains the Bluetooth local name of a device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @returns { string} Returns the name the device.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.getLocalName
     */
    function getLocalName(): string;
    /**
     * Obtains the list of Bluetooth devices that have been paired with the current device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @returns { Array<string>} Returns a list of paired Bluetooth devices's address.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.getPairedDevices
     */
    function getPairedDevices(): Array<string>;
    /**
     * Obtains the connection state of profile.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { ProfileId} profileId - The profile id.
     * @returns { ProfileConnectionState} Returns the connection state.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.getProfileConnectionState
     */
    function getProfileConnState(profileId: ProfileId): ProfileConnectionState;
    /**
     * Sets the confirmation of pairing with a certain device.
     *
     * @permission ohos.permission.MANAGE_BLUETOOTH
     * @param { string} device - The address of the remote device.
     * @param { boolean} accept - Indicates whether to accept the pairing request, {@code true} indicates accept or {@code false} otherwise.
     * @returns { boolean} Returns {@code true} if the pairing confirmation is set; returns {@code false} otherwise.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.setDevicePairingConfirmation
     */
    function setDevicePairingConfirmation(device: string, accept: boolean): boolean;
    /**
     * Sets the Bluetooth friendly name of a device.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @param { string} name - Indicates a valid Bluetooth name.
     * @returns { boolean} Returns {@code true} if the Bluetooth name is set successfully; returns {@code false} otherwise.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.setLocalName
     */
    function setLocalName(name: string): boolean;
    /**
     * Sets the Bluetooth scan mode for a device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { ScanMode} mode - Indicates the Bluetooth scan mode to set, {@link ScanMode}.
     * @param { number} duration - Indicates the duration in seconds, in which the host is discoverable.
     * @returns { boolean} Returns {@code true} if the Bluetooth scan mode is set; returns {@code false} otherwise.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.setBluetoothScanMode
     */
    function setBluetoothScanMode(mode: ScanMode, duration: number): boolean;
    /**
     * Obtains the Bluetooth scanning mode of a device.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @returns { ScanMode} Returns the Bluetooth scanning mode, {@link ScanMode}.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.getBluetoothScanMode
     */
    function getBluetoothScanMode(): ScanMode;
    /**
     * Starts scanning Bluetooth devices.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH and ohos.permission.LOCATION
     * @returns { boolean} Returns {@code true} if the scan is started successfully; returns {@code false} otherwise.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.startBluetoothDiscovery
     */
    function startBluetoothDiscovery(): boolean;
    /**
     * Stops Bluetooth device scanning.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @returns { boolean} Returns {@code true} if scanning is stopped successfully; returns {@code false} otherwise.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.stopBluetoothDiscovery
     */
    function stopBluetoothDiscovery(): boolean;
    /**
     * Subscribe the event reported when a remote Bluetooth device is discovered.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { 'bluetoothDeviceFind'} type - Type of the discovering event to listen for.
     * @param { Callback<Array<string>>} callback - Callback used to listen for the discovering event.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.on#event:bluetoothDeviceFind
     */
    function on(type: 'bluetoothDeviceFind', callback: Callback<Array<string>>): void;
    /**
     * Unsubscribe the event reported when a remote Bluetooth device is discovered.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { 'bluetoothDeviceFind'} type - Type of the discovering event to listen for.
     * @param { Callback<Array<string>>} callback - Callback used to listen for the discovering event.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.off#event:bluetoothDeviceFind
     */
    function off(type: 'bluetoothDeviceFind', callback?: Callback<Array<string>>): void;
    /**
     * Subscribe the event reported when a remote Bluetooth device is bonded.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { 'bondStateChange'} type - Type of the bond state event to listen for.
     * @param { Callback<BondStateParam>} callback - Callback used to listen for the bond state event, {@link BondStateParam}.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.on#event:bondStateChange
     */
    function on(type: 'bondStateChange', callback: Callback<BondStateParam>): void;
    /**
     * Unsubscribe the event reported when a remote Bluetooth device is bonded.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { 'bondStateChange'} type - Type of the bond state event to listen for.
     * @param { Callback<BondStateParam>} callback - Callback used to listen for the bond state event.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.off#event:bondStateChange
     */
    function off(type: 'bondStateChange', callback?: Callback<BondStateParam>): void;
    /**
     * Subscribe the event of a pairing request from a remote Bluetooth device.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @param { 'pinRequired'} type - Type of the pairing request event to listen for.
     * @param { Callback<PinRequiredParam>} callback - Callback used to listen for the pairing request event.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.on#event:pinRequired
     */
    function on(type: 'pinRequired', callback: Callback<PinRequiredParam>): void;
    /**
     * Unsubscribe the event of a pairing request from a remote Bluetooth device.
     *
     * @permission ohos.permission.DISCOVER_BLUETOOTH
     * @param { 'pinRequired'} type - Type of the pairing request event to listen for.
     * @param { Callback<PinRequiredParam>} callback - Callback used to listen for the pairing request event.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.off#event:pinRequired
     */
    function off(type: 'pinRequired', callback?: Callback<PinRequiredParam>): void;
    /**
     * Subscribe the event reported when the Bluetooth state changes.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { 'stateChange'} type - Type of the Bluetooth state changes event to listen for.
     * @param { Callback<BluetoothState>} callback - Callback used to listen for the Bluetooth state event.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.on#event:stateChange
     */
    function on(type: 'stateChange', callback: Callback<BluetoothState>): void;
    /**
     * Unsubscribe the event reported when the Bluetooth state changes.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { 'stateChange'} type - Type of the Bluetooth state changes event to listen for.
     * @param { Callback<BluetoothState>} callback - Callback used to listen for the Bluetooth state event.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.off#event:stateChange
     */
    function off(type: 'stateChange', callback?: Callback<BluetoothState>): void;
    /**
     * Creates a Bluetooth server listening socket.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { string} name - Indicates the service name.
     * @param { SppOption} option - Indicates the listen parameters {@link SppOption}.
     * @param { AsyncCallback<number>} callback - Callback used to return a server socket ID.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.sppListen
     */
    function sppListen(name: string, option: SppOption, callback: AsyncCallback<number>): void;
    /**
     * Waits for a remote device to connect.
     *
     * @param { number} serverSocket - Indicates the server socket ID, returned by {@link sppListen}.
     * @param { AsyncCallback<number>} callback - Callback used to return a client socket ID.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.sppAccept
     */
    function sppAccept(serverSocket: number, callback: AsyncCallback<number>): void;
    /**
     * Connects to a remote device over the socket.
     *
     * @permission ohos.permission.USE_BLUETOOTH
     * @param { string} device - The address of the remote device to connect.
     * @param { SppOption} option - Indicates the connect parameters {@link SppOption}.
     * @param { AsyncCallback<number>} callback - Callback used to return a client socket ID.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.sppConnect
     */
    function sppConnect(device: string, option: SppOption, callback: AsyncCallback<number>): void;
    /**
     * Disables an spp server socket and releases related resources.
     *
     * @param { number} socket - Indicates the server socket ID, returned by {@link sppListen}.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.sppCloseServerSocket
     */
    function sppCloseServerSocket(socket: number): void;
    /**
     * Disables an spp client socket and releases related resources.
     *
     * @param { number} socket - Indicates the client socket ID, returned by {@link sppAccept} or {@link sppConnect}.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.sppCloseClientSocket
     */
    function sppCloseClientSocket(socket: number): void;
    /**
     * Write data through the socket.
     *
     * @param { number} clientSocket - Indicates the client socket ID, returned by {@link sppAccept} or {@link sppConnect}.
     * @param { ArrayBuffer} data - Indicates the data to write.
     * @returns { boolean} Returns {@code true} if the data is write successfully; returns {@code false} otherwise.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.sppWrite
     */
    function sppWrite(clientSocket: number, data: ArrayBuffer): boolean;
    /**
     * Subscribe the event reported when data is read from the socket.
     *
     * @param { 'sppRead'} type - Type of the spp read event to listen for.
     * @param { number} clientSocket - Client socket ID, which is obtained by sppAccept or sppConnect.
     * @param { Callback<ArrayBuffer>} callback - Callback used to listen for the spp read event.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.on#event:sppRead
     */
    function on(type: 'sppRead', clientSocket: number, callback: Callback<ArrayBuffer>): void;
    /**
     * Unsubscribe the event reported when data is read from the socket.
     *
     * @param { 'sppRead'} type - Type of the spp read event to listen for.
     * @param { number} clientSocket - Client socket ID, which is obtained by sppAccept or sppConnect.
     * @param { Callback<ArrayBuffer>} callback - Callback used to listen for the spp read event.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.off#event:sppRead
     */
    function off(type: 'sppRead', clientSocket: number, callback?: Callback<ArrayBuffer>): void;
    /**
     * Obtains the instance of profile.
     *
     * @param { ProfileId} profileId - The profile id..
     * @returns { A2dpSourceProfile | HandsFreeAudioGatewayProfile} Returns instance of profile.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.getProfileInstance
     */
    function getProfile(profileId: ProfileId): A2dpSourceProfile | HandsFreeAudioGatewayProfile;
    /**
     * Base interface of profile.
     *
     * @typedef BaseProfile
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.BaseProfile
     */
    interface BaseProfile {
        /**
         * Obtains the connected devices list of profile.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @returns { Array<string>} Returns the address of connected devices list.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 8
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BaseProfile#getConnectionDevices
         */
        getConnectionDevices(): Array<string>;
        /**
         * Obtains the profile state of device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { string} device - The address of bluetooth device.
         * @returns { ProfileConnectionState} Returns {@link ProfileConnectionState} of device.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 8
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BaseProfile#getDeviceState
         */
        getDeviceState(device: string): ProfileConnectionState;
    }
    /**
     * Manager a2dp source profile.
     *
     * @typedef A2dpSourceProfile
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.A2dpSourceProfile
     */
    interface A2dpSourceProfile extends BaseProfile {
        /**
         * Connect to device with a2dp.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH
         * @param { string} device - The address of the remote device to connect.
         * @returns { boolean} Returns {@code true} if the connect is in process; returns {@code false} otherwise.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 8
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.A2dpSourceProfile#connect
         */
        connect(device: string): boolean;
        /**
         * Disconnect to device with a2dp.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH
         * @param { string} device - The address of the remote device to disconnect.
         * @returns { boolean} Returns {@code true} if the disconnect is in process; returns {@code false} otherwise.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 8
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.A2dpSourceProfile#disconnect
         */
        disconnect(device: string): boolean;
        /**
         * Subscribe the event reported when the profile connection state changes .
         *
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 8
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.A2dpSourceProfile.on#event:connectionStateChange
         */
        on(type: 'connectionStateChange', callback: Callback<StateChangeParam>): void;
        /**
         * Unsubscribe the event reported when the profile connection state changes .
         *
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 8
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.A2dpSourceProfile.off#event:connectionStateChange
         */
        off(type: 'connectionStateChange', callback?: Callback<StateChangeParam>): void;
        /**
         * Obtains the playing state of device.
         *
         * @param { string} device - The address of the remote device.
         * @returns { PlayingState} Returns {@link PlayingState} of the remote device.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 8
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.A2dpSourceProfile#getPlayingState
         */
        getPlayingState(device: string): PlayingState;
    }
    /**
     * Manager handsfree AG profile.
     *
     * @typedef HandsFreeAudioGatewayProfile
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.HandsFreeAudioGatewayProfile
     */
    interface HandsFreeAudioGatewayProfile extends BaseProfile {
        /**
         * Connect to device with hfp.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH
         * @param { string} device - The address of the remote device to connect.
         * @returns { boolean} Returns {@code true} if the connect is in process; returns {@code false} otherwise.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 8
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.HandsFreeAudioGatewayProfile#connect
         */
        connect(device: string): boolean;
        /**
         * Disconnect to device with hfp.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH
         * @param { string} device - The address of the remote device to disconnect.
         * @returns { boolean} Returns {@code true} if the disconnect is in process; returns {@code false} otherwise.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 8
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.HandsFreeAudioGatewayProfile#disconnect
         */
        disconnect(device: string): boolean;
        /**
         * Subscribe the event reported when the profile connection state changes .
         *
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 8
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.HandsFreeAudioGatewayProfile.on#event:connectionStateChange
         */
        on(type: 'connectionStateChange', callback: Callback<StateChangeParam>): void;
        /**
         * Unsubscribe the event reported when the profile connection state changes .
         *
         * @param { 'connectionStateChange'} type - Type of the profile connection state changes event to listen for .
         * @param { Callback<StateChangeParam>} callback - Callback used to listen for event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 8
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.HandsFreeAudioGatewayProfile.off#event:connectionStateChange
         */
        off(type: 'connectionStateChange', callback?: Callback<StateChangeParam>): void;
    }
    /**
     * Provides methods to operate or manage Bluetooth.
     *
     * @namespace BLE
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.BLE
     */
    namespace BLE {
        /**
         * create a JavaScript Gatt server instance.
         *
         * @returns { GattServer} Returns a JavaScript Gatt server instance {@code GattServer}.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLE.createGattServer
         */
        function createGattServer(): GattServer;
        /**
         * create a JavaScript Gatt client device instance.
         *
         * @param { string} deviceId - The address of the remote device.
         * @returns { GattClientDevice} Returns a JavaScript Gatt client device instance {@code GattClientDevice}.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLE.createGattClientDevice
         */
        function createGattClientDevice(deviceId: string): GattClientDevice;
        /**
         * Obtains the list of devices in the connected status.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @returns { Array<string>} Returns the list of device address.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLE.getConnectedBLEDevices
         */
        function getConnectedBLEDevices(): Array<string>;
        /**
         * Starts scanning for specified BLE devices with filters.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH and ohos.permission.LOCATION
         * @param { Array<ScanFilter>} filters - Indicates the list of filters used to filter out specified devices.
         * If you do not want to use filter, set this parameter to {@code null}.
         * @param { ScanOptions} options - Indicates the parameters for scanning and if the user does not assign a value, the default value will be used.
         * {@link ScanOptions#interval} set to 0, {@link ScanOptions#dutyMode} set to {@link SCAN_MODE_LOW_POWER}
         * and {@link ScanOptions#matchMode} set to {@link MATCH_MODE_AGGRESSIVE}.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLE.startBLEScan
         */
        function startBLEScan(filters: Array<ScanFilter>, options?: ScanOptions): void;
        /**
         * Stops BLE scanning.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLE.stopBLEScan
         */
        function stopBLEScan(): void;
        /**
         * Subscribe BLE scan result.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'BLEDeviceFind'} type - Type of the scan result event to listen for.
         * @param { Callback<Array<ScanResult>>} callback - Callback used to listen for the scan result event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLE.on#event:BLEDeviceFind
         */
        function on(type: 'BLEDeviceFind', callback: Callback<Array<ScanResult>>): void;
        /**
         * Unsubscribe BLE scan result.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'BLEDeviceFind'} type - Type of the scan result event to listen for.
         * @param { Callback<Array<ScanResult>>} callback - Callback used to listen for the scan result event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLE.off#event:BLEDeviceFind
         */
        function off(type: 'BLEDeviceFind', callback?: Callback<Array<ScanResult>>): void;
    }
    /**
     * Manages GATT server. Before calling an Gatt server method, you must use {@link createGattServer} to create an GattServer instance.
     *
     * @typedef GattServer
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer
     */
    interface GattServer {
        /**
         * Starts BLE advertising.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH
         * @param { AdvertiseSetting} setting - Indicates the settings for BLE advertising.
         * If you need to use the default value, set this parameter to {@code null}.
         * @param { AdvertiseData} advData - Indicates the advertising data.
         * @param { AdvertiseData} advResponse - Indicates the scan response associated with the advertising data.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer#startAdvertising
         */
        startAdvertising(setting: AdvertiseSetting, advData: AdvertiseData, advResponse?: AdvertiseData): void;
        /**
         * Stops BLE advertising.
         *
         * @permission ohos.permission.DISCOVER_BLUETOOTH
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer#stopAdvertising
         */
        stopAdvertising(): void;
        /**
         * Adds a specified service to be hosted.
         * <p>The added service and its characteristics are provided by the local device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { GattService} service - Indicates the service to add.
         * @returns { boolean} Returns {@code true} if the service is added; returns {@code false} otherwise.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer#addService
         */
        addService(service: GattService): boolean;
        /**
         * Removes a specified service from the list of GATT services provided by this device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { string} serviceUuid - Indicates the UUID of the service to remove.
         * @returns { boolean} Returns {@code true} if the service is removed; returns {@code false} otherwise.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer#removeService
         */
        removeService(serviceUuid: string): boolean;
        /**
         * Closes this {@code GattServer} object and unregisters its callbacks.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer#close
         */
        close(): void;
        /**
         * Sends a notification of a change in a specified local characteristic.
         * <p>This method should be called for every BLE peripheral device that has requested notifications.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { string} deviceId - Indicates the address of the BLE peripheral device to receive the notification.
         * @param { NotifyCharacteristic} notifyCharacteristic - Indicates the local characteristic that has changed.
         * @returns { boolean} Returns {@code true} if the notification is sent successfully; returns {@code false} otherwise.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer#notifyCharacteristicChanged
         */
        notifyCharacteristicChanged(deviceId: string, notifyCharacteristic: NotifyCharacteristic): boolean;
        /**
         * Sends a response to a specified read or write request to a given BLE peripheral device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { ServerResponse} serverResponse - Indicates the response parameters {@link ServerResponse}.
         * @returns { boolean} Returns {@code true} if the response is sent successfully; returns {@code false} otherwise.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer#sendResponse
         */
        sendResponse(serverResponse: ServerResponse): boolean;
        /**
         * Subscribe characteristic read event.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'characteristicRead'} type - Type of the characteristic read event to listen for.
         * @param { Callback<CharacteristicReadReq>} callback - Callback used to listen for the characteristic read event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer.on#event:characteristicRead
         */
        on(type: 'characteristicRead', callback: Callback<CharacteristicReadReq>): void;
        /**
         * Unsubscribe characteristic read event.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'characteristicRead'} type - Type of the characteristic read event to listen for.
         * @param { Callback<CharacteristicReadReq>} callback - Callback used to listen for the characteristic read event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer.off#event:characteristicRead
         */
        off(type: 'characteristicRead', callback?: Callback<CharacteristicReadReq>): void;
        /**
         * Subscribe characteristic write event.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'characteristicWrite'} type - Type of the characteristic write event to listen for.
         * @param { Callback<CharacteristicWriteReq>} callback - Callback used to listen for the characteristic write event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer.on#event:characteristicWrite
         */
        on(type: 'characteristicWrite', callback: Callback<CharacteristicWriteReq>): void;
        /**
         * Unsubscribe characteristic write event.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'characteristicWrite'} type - Type of the characteristic write event to listen for.
         * @param { Callback<CharacteristicWriteReq>} callback - Callback used to listen for the characteristic write event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer.off#event:characteristicWrite
         */
        off(type: 'characteristicWrite', callback?: Callback<CharacteristicWriteReq>): void;
        /**
         * Subscribe descriptor read event.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'descriptorRead'} type - Type of the descriptor read event to listen for.
         * @param { Callback<DescriptorReadReq>} callback - Callback used to listen for the descriptor read event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer.on#event:descriptorRead
         */
        on(type: 'descriptorRead', callback: Callback<DescriptorReadReq>): void;
        /**
         * Unsubscribe descriptor read event.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'descriptorRead'} type - Type of the descriptor read event to listen for.
         * @param { Callback<DescriptorReadReq>} callback - Callback used to listen for the descriptor read event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer.off#event:descriptorRead
         */
        off(type: 'descriptorRead', callback?: Callback<DescriptorReadReq>): void;
        /**
         * Subscribe descriptor write event.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'descriptorWrite'} type - Type of the descriptor write event to listen for.
         * @param { Callback<DescriptorWriteReq>} callback - Callback used to listen for the descriptor write event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer.on#event:descriptorWrite
         */
        on(type: 'descriptorWrite', callback: Callback<DescriptorWriteReq>): void;
        /**
         * Unsubscribe descriptor write event.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'descriptorWrite'} type - Type of the descriptor write event to listen for.
         * @param { Callback<DescriptorWriteReq>} callback - Callback used to listen for the descriptor write event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer.off#event:descriptorWrite
         */
        off(type: 'descriptorWrite', callback?: Callback<DescriptorWriteReq>): void;
        /**
         * Subscribe server connection state changed event.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'connectStateChange'} type - Type of the connection state changed event to listen for.
         * @param { Callback<BLEConnectChangedState>} callback - Callback used to listen for the connection state changed event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer.on#event:connectStateChange
         */
        on(type: 'connectStateChange', callback: Callback<BLEConnectChangedState>): void;
        /**
         * Unsubscribe server connection state changed event.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'connectStateChange'} type - Type of the connection state changed event to listen for.
         * @param { Callback<BLEConnectChangedState>} callback - Callback used to listen for the connection state changed event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattServer.off#event:connectStateChange
         */
        off(type: 'connectStateChange', callback?: Callback<BLEConnectChangedState>): void;
    }
    /**
     * Manages GATT client. Before calling an Gatt client method, you must use {@link createGattClientDevice} to create an GattClientDevice instance.
     *
     * @typedef GattClientDevice
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice
     */
    interface GattClientDevice {
        /**
         * Connects to a BLE peripheral device.
         * <p>The 'BLEConnectionStateChange' event is subscribed to return the connection state.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @returns { boolean} Returns {@code true} if the connection process starts; returns {@code false} otherwise.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#connect
         */
        connect(): boolean;
        /**
         * Disconnects from or stops an ongoing connection to a BLE peripheral device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @returns { boolean} Returns {@code true} if the disconnection process starts; returns {@code false} otherwise.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#disconnect
         */
        disconnect(): boolean;
        /**
         * Disables a BLE peripheral device.
         * <p> This method unregisters the device and clears the registered callbacks and handles.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @returns { boolean} Returns {@code true} if the the device is disabled; returns {@code false} otherwise.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#close
         */
        close(): boolean;
        /**
         * Obtains the name of BLE peripheral device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { AsyncCallback<string>} callback - Callback used to obtain the device name.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#getDeviceName
         */
        getDeviceName(callback: AsyncCallback<string>): void;
        /**
         * Obtains the name of BLE peripheral device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @returns { Promise<string>} Returns a string representation of the name if obtained;
         * returns {@code null} if the name fails to be obtained or the name does not exist.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#getDeviceName
         */
        getDeviceName(): Promise<string>;
        /**
         * Starts discovering services.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { AsyncCallback<Array<GattService>>} callback - Callback used to catch the services.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#getServices
         */
        getServices(callback: AsyncCallback<Array<GattService>>): void;
        /**
         * Starts discovering services.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @returns { Promise<Array<GattService>>} Returns the list of services {@link GattService} of the BLE peripheral device.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#getServices
         */
        getServices(): Promise<Array<GattService>>;
        /**
         * Reads the characteristic of a BLE peripheral device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { BLECharacteristic} characteristic - Indicates the characteristic to read.
         * @param { AsyncCallback<BLECharacteristic>} callback - Callback invoked to return the characteristic value read.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#readCharacteristicValue
         */
        readCharacteristicValue(characteristic: BLECharacteristic, callback: AsyncCallback<BLECharacteristic>): void;
        /**
         * Reads the characteristic of a BLE peripheral device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { BLECharacteristic} characteristic - Indicates the characteristic to read.
         * @returns { Promise<BLECharacteristic>} - Promise used to return the characteristic value read.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#readCharacteristicValue
         */
        readCharacteristicValue(characteristic: BLECharacteristic): Promise<BLECharacteristic>;
        /**
         * Reads the descriptor of a BLE peripheral device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { BLEDescriptor} descriptor - Indicates the descriptor to read.
         * @param { AsyncCallback<BLEDescriptor>} callback - Callback invoked to return the descriptor read.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#readDescriptorValue
         */
        readDescriptorValue(descriptor: BLEDescriptor, callback: AsyncCallback<BLEDescriptor>): void;
        /**
         * Reads the descriptor of a BLE peripheral device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { BLEDescriptor} descriptor - Indicates the descriptor to read.
         * @returns { Promise<BLEDescriptor>} - Promise used to return the descriptor read.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#readDescriptorValue
         */
        readDescriptorValue(descriptor: BLEDescriptor): Promise<BLEDescriptor>;
        /**
         * Writes the characteristic of a BLE peripheral device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { BLECharacteristic} characteristic - Indicates the characteristic to write.
         * @returns { boolean} Returns {@code true} if the characteristic is written successfully; returns {@code false} otherwise.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#writeCharacteristicValue
         */
        writeCharacteristicValue(characteristic: BLECharacteristic): boolean;
        /**
         * Writes the descriptor of a BLE peripheral device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { BLEDescriptor} descriptor - Indicates the descriptor to write.
         * @returns { boolean} Returns {@code true} if the descriptor is written successfully; returns {@code false} otherwise.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#writeDescriptorValue
         */
        writeDescriptorValue(descriptor: BLEDescriptor): boolean;
        /**
         * Get the RSSI value of this BLE peripheral device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { AsyncCallback<number>} callback - Callback invoked to return the RSSI, in dBm.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#getRssiValue
         */
        getRssiValue(callback: AsyncCallback<number>): void;
        /**
         * Get the RSSI value of this BLE peripheral device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @returns { Promise<number>} Returns the RSSI value.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#getRssiValue
         */
        getRssiValue(): Promise<number>;
        /**
         * Set the mtu size of a BLE peripheral device.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { number} mtu - The maximum transmission unit.
         * @returns { boolean} Returns {@code true} if the set mtu is successfully; returns {@code false} otherwise.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#setBLEMtuSize
         */
        setBLEMtuSize(mtu: number): boolean;
        /**
         * Enables or disables notification of a characteristic when value changed.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { BLECharacteristic} characteristic - BLE characteristic to listen for.
         * @param { boolean} enable - Specifies whether to enable notification of the characteristic. The value {@code true} indicates
         * that notification is enabled, and the value {@code false} indicates that notification is disabled.
         * @returns { boolean} Returns {@code true} if notification of the characteristic is enabled or disabled;
         * returns {@code false} otherwise.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice#setNotifyCharacteristicChanged
         */
        setNotifyCharacteristicChanged(characteristic: BLECharacteristic, enable: boolean): boolean;
        /**
         * Subscribe characteristic value changed event.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'BLECharacteristicChange'} type - Type of the characteristic value changed event to listen for.
         * @param { Callback<BLECharacteristic>} callback - Callback used to listen for the characteristic value changed event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice.on#event:BLECharacteristicChange
         */
        on(type: 'BLECharacteristicChange', callback: Callback<BLECharacteristic>): void;
        /**
         * Unsubscribe characteristic value changed event.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'BLECharacteristicChange'} type - Type of the characteristic value changed event to listen for.
         * @param { Callback<BLECharacteristic>} callback - Callback used to listen for the characteristic value changed event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice.off#event:BLECharacteristicChange
         */
        off(type: 'BLECharacteristicChange', callback?: Callback<BLECharacteristic>): void;
        /**
         * Subscribe client connection state changed event.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'BLEConnectionStateChange'} type - Type of the connection state changed event to listen for.
         * @param { Callback<BLEConnectChangedState>} callback - Callback used to listen for the connection state changed event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice.on#event:BLEConnectionStateChange
         */
        on(type: 'BLEConnectionStateChange', callback: Callback<BLEConnectChangedState>): void;
        /**
         * Unsubscribe client connection state changed event.
         *
         * @permission ohos.permission.USE_BLUETOOTH
         * @param { 'BLEConnectionStateChange'} type - Type of the connection state changed event to listen for.
         * @param { Callback<BLEConnectChangedState>} callback - Callback used to listen for the connection state changed event.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattClientDevice.off#event:BLEConnectionStateChange
         */
        off(type: 'BLEConnectionStateChange', callback?: Callback<BLEConnectChangedState>): void;
    }
    /**
     * Describes the Gatt service.
     *
     * @typedef GattService
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.GattService
     */
    interface GattService {
        /**
         * The UUID of a GattService instance
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattService.serviceUuid
         */
        serviceUuid: string;
        /**
         * Indicates whether the GattService instance is primary or secondary.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattService.isPrimary
         */
        isPrimary: boolean;
        /**
         * The {@link BLECharacteristic} list belongs to this GattService instance
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattService.characteristics
         */
        characteristics: Array<BLECharacteristic>;
        /**
         * The list of GATT services contained in the service
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.GattService.includeServices
         */
        includeServices?: Array<GattService>;
    }
    /**
     * Describes the Gatt characteristic.
     *
     * @typedef BLECharacteristic
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.BLECharacteristic
     */
    interface BLECharacteristic {
        /**
         * The UUID of the {@link GattService} instance to which the characteristic belongs
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLECharacteristic.serviceUuid
         */
        serviceUuid: string;
        /**
         * The UUID of a BLECharacteristic instance
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLECharacteristic.characteristicUuid
         */
        characteristicUuid: string;
        /**
         * The value of a BLECharacteristic instance
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLECharacteristic.characteristicValue
         */
        characteristicValue: ArrayBuffer;
        /**
         * The list of {@link BLEDescriptor} contained in the characteristic
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLECharacteristic.descriptors
         */
        descriptors: Array<BLEDescriptor>;
    }
    /**
     * Describes the Gatt descriptor.
     *
     * @typedef BLEDescriptor
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.BLEDescriptor
     */
    interface BLEDescriptor {
        /**
         * The UUID of the {@link GattService} instance to which the descriptor belongs
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLEDescriptor.serviceUuid
         */
        serviceUuid: string;
        /**
         * The UUID of the {@link BLECharacteristic} instance to which the descriptor belongs
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLEDescriptor.characteristicUuid
         */
        characteristicUuid: string;
        /**
         * The UUID of the BLEDescriptor instance
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLEDescriptor.descriptorUuid
         */
        descriptorUuid: string;
        /**
         * The value of the BLEDescriptor instance
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLEDescriptor.descriptorValue
         */
        descriptorValue: ArrayBuffer;
    }
    /**
     * Describes the value of the indication or notification sent by the Gatt server.
     *
     * @typedef NotifyCharacteristic
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.NotifyCharacteristic
     */
    interface NotifyCharacteristic {
        /**
         * The UUID of the {@link GattService} instance to which the characteristic belongs
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.NotifyCharacteristic.serviceUuid
         */
        serviceUuid: string;
        /**
         * The UUID of a NotifyCharacteristic instance
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.NotifyCharacteristic.characteristicUuid
         */
        characteristicUuid: string;
        /**
         * The value of a NotifyCharacteristic instance
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.NotifyCharacteristic.characteristicValue
         */
        characteristicValue: ArrayBuffer;
        /**
         * Specifies whether to request confirmation from the BLE peripheral device (indication) or
         * send a notification. Value {@code true} indicates the former and {@code false} indicates the latter.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.NotifyCharacteristic.confirm
         */
        confirm: boolean;
    }
    /**
     * Describes the parameters of the Gatt client's characteristic read request.
     *
     * @typedef CharacteristicReadReq
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.CharacteristicReadRequest
     */
    interface CharacteristicReadReq {
        /**
         * Indicates the address of the client that initiates the read request
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.CharacteristicReadRequest.deviceId
         */
        deviceId: string;
        /**
         * The Id of the read request
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.CharacteristicReadRequest.transId
         */
        transId: number;
        /**
         * Indicates the byte offset of the start position for reading characteristic value
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.CharacteristicReadRequest.offset
         */
        offset: number;
        /**
         * The UUID of a CharacteristicReadReq instance
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.CharacteristicReadRequest.characteristicUuid
         */
        characteristicUuid: string;
        /**
         * The UUID of the service to which the characteristic belongs
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.CharacteristicReadRequest.serviceUuid
         */
        serviceUuid: string;
    }
    /**
     * Describes the parameters of the of the Gatt client's characteristic write request.
     *
     * @typedef CharacteristicWriteReq
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.CharacteristicWriteRequest
     */
    interface CharacteristicWriteReq {
        /**
         * Indicates the address of the client that initiates the write request
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.CharacteristicWriteRequest.deviceId
         */
        deviceId: string;
        /**
         * The Id of the write request
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.CharacteristicWriteRequest.transId
         */
        transId: number;
        /**
         * Indicates the byte offset of the start position for writing characteristic value
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.CharacteristicWriteRequest.offset
         */
        offset: number;
        /**
         * Whether this request should be pending for later operation
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.CharacteristicWriteRequest.isPrep
         */
        isPrep: boolean;
        /**
         * Whether the remote client need a response
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.CharacteristicWriteRequest.needRsp
         */
        needRsp: boolean;
        /**
         * Indicates the value to be written
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.CharacteristicWriteRequest.value
         */
        value: ArrayBuffer;
        /**
         * The UUID of a CharacteristicWriteReq instance
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.CharacteristicWriteRequest.characteristicUuid
         */
        characteristicUuid: string;
        /**
         * The UUID of the service to which the characteristic belongs
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.CharacteristicWriteRequest.serviceUuid
         */
        serviceUuid: string;
    }
    /**
     * Describes the parameters of the Gatt client's descriptor read request.
     *
     * @typedef DescriptorReadReq
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorReadRequest
     */
    interface DescriptorReadReq {
        /**
         * Indicates the address of the client that initiates the read request
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorReadRequest.deviceId
         */
        deviceId: string;
        /**
         * The Id of the read request
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorReadRequest.transId
         */
        transId: number;
        /**
         * Indicates the byte offset of the start position for reading characteristic value
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorReadRequest.offset
         */
        offset: number;
        /**
         * The UUID of a DescriptorReadReq instance
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorReadRequest.descriptorUuid
         */
        descriptorUuid: string;
        /**
         * The UUID of the characteristic to which the descriptor belongs
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorReadRequest.characteristicUuid
         */
        characteristicUuid: string;
        /**
         * The UUID of the service to which the descriptor belongs
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorReadRequest.serviceUuid
         */
        serviceUuid: string;
    }
    /**
     * Describes the parameters of the Gatt client's characteristic write request.
     *
     * @typedef DescriptorWriteReq
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorWriteRequest
     */
    interface DescriptorWriteReq {
        /**
         * Indicates the address of the client that initiates the write request
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorWriteRequest.deviceId
         */
        deviceId: string;
        /**
         * The Id of the write request
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorWriteRequest.transId
         */
        transId: number;
        /**
         * Indicates the byte offset of the start position for writing characteristic value
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorWriteRequest.offset
         */
        offset: number;
        /**
         * Whether this request should be pending for later operation
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorWriteRequest.isPrep
         */
        isPrep: boolean;
        /**
         * Whether the remote client need a response
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorWriteRequest.needRsp
         */
        needRsp: boolean;
        /**
         * Indicates the value to be written
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorWriteRequest.value
         */
        value: ArrayBuffer;
        /**
         * The UUID of a DescriptorWriteReq instance
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorWriteRequest.descriptorUuid
         */
        descriptorUuid: string;
        /**
         * The UUID of the characteristic to which the descriptor belongs
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorWriteRequest.characteristicUuid
         */
        characteristicUuid: string;
        /**
         * The UUID of the service to which the descriptor belongs
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.DescriptorWriteRequest.serviceUuid
         */
        serviceUuid: string;
    }
    /**
     * Describes the parameters of a response send by the server to a specified read or write request.
     *
     * @typedef ServerResponse
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.ServerResponse
     */
    interface ServerResponse {
        /**
         * Indicates the address of the client to which to send the response
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.ServerResponse.deviceId
         */
        deviceId: string;
        /**
         * The Id of the write request
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.ServerResponse.transId
         */
        transId: number;
        /**
         * Indicates the status of the read or write request, set this parameter to '0' in normal cases
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.ServerResponse.status
         */
        status: number;
        /**
         * Indicates the byte offset of the start position for reading or writing operation
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.ServerResponse.offset
         */
        offset: number;
        /**
         * Indicates the value to be sent
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.ServerResponse.value
         */
        value: ArrayBuffer;
    }
    /**
     * Describes the Gatt profile connection state.
     *
     * @typedef BLEConnectChangedState
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.BLEConnectChangedState
     */
    interface BLEConnectChangedState {
        /**
         * Indicates the peer device address
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLEConnectChangedState.deviceId
         */
        deviceId: string;
        /**
         * Connection state of the Gatt profile
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.BLEConnectChangedState.state
         */
        state: ProfileConnectionState;
    }
    /**
     * Describes the contents of the scan results.
     *
     * @typedef ScanResult
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.ScanResult
     */
    interface ScanResult {
        /**
         * Address of the scanned device
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.ScanResult.deviceId
         */
        deviceId: string;
        /**
         * RSSI of the remote device
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.ScanResult.rssi
         */
        rssi: number;
        /**
         * The raw data of broadcast packet
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.ScanResult.data
         */
        data: ArrayBuffer;
    }
    /**
     * Describes the settings for BLE advertising.
     *
     * @typedef AdvertiseSetting
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.AdvertiseSetting
     */
    interface AdvertiseSetting {
        /**
         * Minimum slot value for the advertising interval, which is {@code 32} (20 ms)
         * Maximum slot value for the advertising interval, which is {@code 16777215} (10485.759375s)
         * Default slot value for the advertising interval, which is {@code 1600} (1s)
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.AdvertiseSetting.interval
         */
        interval?: number;
        /**
         * Minimum transmission power level for advertising, which is {@code -127}
         * Maximum transmission power level for advertising, which is {@code 1}
         * Default transmission power level for advertising, which is {@code -7}
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.AdvertiseSetting.txPower
         */
        txPower?: number;
        /**
         * Indicates whether the BLE is connectable, default is {@code true}
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.AdvertiseSetting.connectable
         */
        connectable?: boolean;
    }
    /**
     * Describes the advertising data.
     *
     * @typedef AdvertiseData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.AdvertiseData
     */
    interface AdvertiseData {
        /**
         * The specified service UUID list to this advertisement
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.AdvertiseData.serviceUuids
         */
        serviceUuids: Array<string>;
        /**
         * The specified manufacturer data list to this advertisement
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.AdvertiseData.manufactureData
         */
        manufactureData: Array<ManufactureData>;
        /**
         * The specified service data list to this advertisement
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.AdvertiseData.serviceData
         */
        serviceData: Array<ServiceData>;
    }
    /**
     * Describes the manufacturer data.
     *
     * @typedef ManufactureData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.ManufactureData
     */
    interface ManufactureData {
        /**
         * Indicates the manufacturer ID assigned by Bluetooth SIG
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.ManufactureData.manufactureId
         */
        manufactureId: number;
        /**
         * Indicates the manufacturer data to add
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.ManufactureData.manufactureValue
         */
        manufactureValue: ArrayBuffer;
    }
    /**
     * Describes the service data.
     *
     * @typedef ServiceData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.bluetoothManager/bluetoothManager.ServiceData
     */
    interface ServiceData {
        /**
         * Indicates the UUID of the service data to add
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
         * @deprecated since 9
         * @useinstead ohos.bluetoothManager/bluetoothManager.ServiceData.serviceUuid
         */
        serviceUuid: string;
        /**
         * Indicates the service data to add
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 7
