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
/* Continuation part 3 of @ohos.bluetoothManager.d.ts (reference only). */
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.DescriptorWriteRequest#value
         */
        value: ArrayBuffer;
        /**
         * The UUID of a DescriptorWriteRequest instance
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.DescriptorWriteRequest#descriptorUuid
         */
        descriptorUuid: string;
        /**
         * The UUID of the characteristic to which the descriptor belongs
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.DescriptorWriteRequest#characteristicUuid
         */
        characteristicUuid: string;
        /**
         * The UUID of the service to which the descriptor belongs
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.DescriptorWriteRequest#serviceUuid
         */
        serviceUuid: string;
    }
    /**
     * Describes the parameters of a response send by the server to a specified read or write request.
     *
     * @typedef ServerResponse
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.ble/ble.ServerResponse
     */
    interface ServerResponse {
        /**
         * Indicates the address of the client to which to send the response
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ServerResponse#deviceId
         */
        deviceId: string;
        /**
         * The Id of the write request
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ServerResponse#transId
         */
        transId: number;
        /**
         * Indicates the status of the read or write request, set this parameter to '0' in normal cases
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ServerResponse#status
         */
        status: number;
        /**
         * Indicates the byte offset of the start position for reading or writing operation
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ServerResponse#offset
         */
        offset: number;
        /**
         * Indicates the value to be sent
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ServerResponse#value
         */
        value: ArrayBuffer;
    }
    /**
     * Describes the Gatt profile connection state.
     *
     * @typedef BLEConnectChangedState
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.ble/ble.BLEConnectionChangeState
     */
    interface BLEConnectChangedState {
        /**
         * Indicates the peer device address
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.BLEConnectionChangeState#deviceId
         */
        deviceId: string;
        /**
         * Connection state of the Gatt profile
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.BLEConnectionChangeState#state
         */
        state: ProfileConnectionState;
    }
    /**
     * Describes the contents of the scan results.
     *
     * @typedef ScanResult
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.ble/ble.ScanResult
     */
    interface ScanResult {
        /**
         * Address of the scanned device
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanResult#deviceId
         */
        deviceId: string;
        /**
         * RSSI of the remote device
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanResult#rssi
         */
        rssi: number;
        /**
         * The raw data of broadcast packet
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanResult#data
         */
        data: ArrayBuffer;
    }
    /**
     * Describes the settings for BLE advertising.
     *
     * @typedef AdvertiseSetting
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.ble/ble.AdvertiseSetting
     */
    interface AdvertiseSetting {
        /**
         * Minimum slot value for the advertising interval, which is {@code 32} (20 ms)
         * Maximum slot value for the advertising interval, which is {@code 16777215} (10485.759375s)
         * Default slot value for the advertising interval, which is {@code 1600} (1s)
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.AdvertiseSetting#interval
         */
        interval?: number;
        /**
         * Minimum transmission power level for advertising, which is {@code -127}
         * Maximum transmission power level for advertising, which is {@code 1}
         * Default transmission power level for advertising, which is {@code -7}
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.AdvertiseSetting#txPower
         */
        txPower?: number;
        /**
         * Indicates whether the BLE is connectable, default is {@code true}
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.AdvertiseSetting#connectable
         */
        connectable?: boolean;
    }
    /**
     * Describes the advertising data.
     *
     * @typedef AdvertiseData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.ble/ble.AdvertiseData
     */
    interface AdvertiseData {
        /**
         * The specified service UUID list to this advertisement
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.AdvertiseData#serviceUuids
         */
        serviceUuids: Array<string>;
        /**
         * The specified manufacturer data list to this advertisement
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.AdvertiseData#manufactureData
         */
        manufactureData: Array<ManufactureData>;
        /**
         * The specified service data list to this advertisement
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.AdvertiseData#serviceData
         */
        serviceData: Array<ServiceData>;
    }
    /**
     * Describes the manufacturer data.
     *
     * @typedef ManufactureData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.ble/ble.ManufactureData
     */
    interface ManufactureData {
        /**
         * Indicates the manufacturer ID assigned by Bluetooth SIG
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ManufactureData#manufactureId
         */
        manufactureId: number;
        /**
         * Indicates the manufacturer data to add
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ManufactureData#manufactureValue
         */
        manufactureValue: ArrayBuffer;
    }
    /**
     * Describes the service data.
     *
     * @typedef ServiceData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.ble/ble.ServiceData
     */
    interface ServiceData {
        /**
         * Indicates the UUID of the service data to add
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ServiceData#serviceUuid
         */
        serviceUuid: string;
        /**
         * Indicates the service data to add
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ServiceData#serviceValue
         */
        serviceValue: ArrayBuffer;
    }
    /**
     * Describes the criteria for filtering scanning results can be set.
     *
     * @typedef ScanFilter
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.ble/ble.ScanFilter
     */
    interface ScanFilter {
        /**
         * The address of a BLE peripheral device
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanFilter#deviceId
         */
        deviceId?: string;
        /**
         * The name of a BLE peripheral device
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanFilter#name
         */
        name?: string;
        /**
         * The service UUID of a BLE peripheral device
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanFilter#serviceUuid
         */
        serviceUuid?: string;
        /**
         * Service UUID mask.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanFilter#serviceUuidMask
         */
        serviceUuidMask?: string;
        /**
         * Service solicitation UUID.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanFilter#serviceSolicitationUuid
         */
        serviceSolicitationUuid?: string;
        /**
         * Service solicitation UUID mask.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanFilter#serviceSolicitationUuidMask
         */
        serviceSolicitationUuidMask?: string;
        /**
         * Service data.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanFilter#serviceData
         */
        serviceData?: ArrayBuffer;
        /**
         * Service data mask.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanFilter#serviceDataMask
         */
        serviceDataMask?: ArrayBuffer;
        /**
         * Manufacture id.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanFilter#manufactureId
         */
        manufactureId?: number;
        /**
         * Manufacture data.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanFilter#manufactureData
         */
        manufactureData?: ArrayBuffer;
        /**
         * Manufacture data mask.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanFilter#manufactureDataMask
         */
        manufactureDataMask?: ArrayBuffer;
    }
    /**
     * Describes the parameters for scan.
     *
     * @typedef ScanOptions
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.ble/ble.ScanOptions
     */
    interface ScanOptions {
        /**
         * Time of delay for reporting the scan result
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanOptions#interval
         */
        interval?: number;
        /**
         * Bluetooth LE scan mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanOptions#dutyMode
         */
        dutyMode?: ScanDuty;
        /**
         * Match mode for Bluetooth LE scan filters hardware match
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanOptions#matchMode
         */
        matchMode?: MatchMode;
    }
    /**
     * Describes the spp parameters.
     *
     * @typedef SppOption
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.socket/socket.SppOptions
     */
    interface SppOption {
        /**
         * Indicates the UUID in the SDP record.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.socket/socket.SppOptions#uuid
         */
        uuid: string;
        /**
         * Indicates secure channel or not
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.socket/socket.SppOptions#secure
         */
        secure: boolean;
        /**
         * Spp link type {@link SppType}
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.socket/socket.SppOptions#type
         */
        type: SppType;
    }
    /**
     * Describes the bond key param.
     *
     * @typedef PinRequiredParam
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.PinRequiredParam
     */
    interface PinRequiredParam {
        /**
         * ID of the device to pair.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.PinRequiredParam#deviceId
         */
        deviceId: string;
        /**
         * Key for the device pairing.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.PinRequiredParam#pinCode
         */
        pinCode: string;
    }
    /**
     * Describes the class of a bluetooth device.
     *
     * @typedef DeviceClass
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.DeviceClass
     */
    interface DeviceClass {
        /**
         * Major classes of Bluetooth devices.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.DeviceClass#majorClass
         */
        majorClass: MajorClass;
        /**
         * Major and minor classes of Bluetooth devices.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.DeviceClass#majorMinorClass
         */
        majorMinorClass: MajorMinorClass;
        /**
         * Class of the device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.DeviceClass#classOfDevice
         */
        classOfDevice: number;
    }
    /**
     * Describes the class of a bluetooth device.
     *
     * @typedef BondStateParam
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.BondStateParam
     */
    interface BondStateParam {
        /**
         * Address of a Bluetooth device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.BondStateParam#deviceId
         */
        deviceId: string;
        /**
         * Profile connection state of the device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.BondStateParam#state
         */
        state: BondState;
    }
    /**
     * Profile state change parameters.
     *
     * @typedef StateChangeParam
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.baseProfile/baseProfile.StateChangeParam
     */
    interface StateChangeParam {
        /**
         * The address of device
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.baseProfile/baseProfile.StateChangeParam#deviceId
         */
        deviceId: string;
        /**
         * Profile state value
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.baseProfile/baseProfile.StateChangeParam#state
         */
        state: ProfileConnectionState;
    }
    /**
     * The enum of scan duty.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.ble/ble.ScanDuty
     */
    enum ScanDuty {
        /**
         * low power mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanDuty#SCAN_MODE_LOW_POWER
         */
        SCAN_MODE_LOW_POWER = 0,
        /**
         * balanced power mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanDuty#SCAN_MODE_BALANCED
         */
        SCAN_MODE_BALANCED = 1,
        /**
         * Scan using highest duty cycle
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.ScanDuty#SCAN_MODE_LOW_LATENCY
         */
        SCAN_MODE_LOW_LATENCY = 2
    }
    /**
     * The enum of BLE match mode.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.ble/ble.MatchMode
     */
    enum MatchMode {
        /**
         * aggressive mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.MatchMode#MATCH_MODE_AGGRESSIVE
         */
        MATCH_MODE_AGGRESSIVE = 1,
        /**
         * sticky mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.ble/ble.MatchMode#MATCH_MODE_STICKY
         */
        MATCH_MODE_STICKY = 2
    }
    /**
     * The enum of profile connection state.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.constant/constant.ProfileConnectionState
     */
    enum ProfileConnectionState {
        /**
         * the current profile is disconnected
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.ProfileConnectionState#STATE_DISCONNECTED
         */
        STATE_DISCONNECTED = 0,
        /**
         * the current profile is being connected
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.ProfileConnectionState#STATE_CONNECTING
         */
        STATE_CONNECTING = 1,
        /**
         * the current profile is connected
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.ProfileConnectionState#STATE_CONNECTED
         */
        STATE_CONNECTED = 2,
        /**
         * the current profile is being disconnected
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.ProfileConnectionState#STATE_DISCONNECTING
         */
        STATE_DISCONNECTING = 3
    }
    /**
     * The enum of bluetooth state.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.access/access.BluetoothState
     */
    enum BluetoothState {
        /**
         * Indicates the local Bluetooth is off
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.access/access.BluetoothState#STATE_OFF
         */
        STATE_OFF = 0,
        /**
         * Indicates the local Bluetooth is turning on
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.access/access.BluetoothState#STATE_TURNING_ON
         */
        STATE_TURNING_ON = 1,
        /**
         * Indicates the local Bluetooth is on, and ready for use
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.access/access.BluetoothState#STATE_ON
         */
        STATE_ON = 2,
        /**
         * Indicates the local Bluetooth is turning off
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.access/access.BluetoothState#STATE_TURNING_OFF
         */
        STATE_TURNING_OFF = 3,
        /**
         * Indicates the local Bluetooth is turning LE mode on
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.access/access.BluetoothState#STATE_BLE_TURNING_ON
         */
        STATE_BLE_TURNING_ON = 4,
        /**
         * Indicates the local Bluetooth is in LE only mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.access/access.BluetoothState#STATE_BLE_ON
         */
        STATE_BLE_ON = 5,
        /**
         * Indicates the local Bluetooth is turning off LE only mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.access/access.BluetoothState#STATE_BLE_TURNING_OFF
         */
        STATE_BLE_TURNING_OFF = 6
    }
    /**
     * The enum of SPP type.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.socket/socket.SppType
     */
    enum SppType {
        /**
         * RFCOMM
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.socket/socket.SppType#SPP_RFCOMM
         */
        SPP_RFCOMM
    }
    /**
     * The enum of BR scan mode.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.ScanMode
     */
    enum ScanMode {
        /**
         * Indicates the scan mode is none
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.ScanMode#SCAN_MODE_NONE
         */
        SCAN_MODE_NONE = 0,
        /**
         * Indicates the scan mode is connectable
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.ScanMode#SCAN_MODE_CONNECTABLE
         */
        SCAN_MODE_CONNECTABLE = 1,
        /**
         * Indicates the scan mode is general discoverable
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.ScanMode#SCAN_MODE_GENERAL_DISCOVERABLE
         */
        SCAN_MODE_GENERAL_DISCOVERABLE = 2,
        /**
         * Indicates the scan mode is limited discoverable
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.ScanMode#SCAN_MODE_LIMITED_DISCOVERABLE
         */
        SCAN_MODE_LIMITED_DISCOVERABLE = 3,
        /**
         * Indicates the scan mode is connectable and general discoverable
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.ScanMode#SCAN_MODE_CONNECTABLE_GENERAL_DISCOVERABLE
         */
        SCAN_MODE_CONNECTABLE_GENERAL_DISCOVERABLE = 4,
        /**
         * Indicates the scan mode is connectable and limited discoverable
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.ScanMode#SCAN_MODE_CONNECTABLE_LIMITED_DISCOVERABLE
         */
        SCAN_MODE_CONNECTABLE_LIMITED_DISCOVERABLE = 5
    }
    /**
     * The enum of bond state.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.connection/connection.BondState
     */
    enum BondState {
        /**
         * Indicate the bond state is invalid
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.BondState#BOND_STATE_INVALID
         */
        BOND_STATE_INVALID = 0,
        /**
         * Indicate the bond state is bonding
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.BondState#BOND_STATE_BONDING
         */
        BOND_STATE_BONDING = 1,
        /**
         * Indicate the bond state is bonded
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.connection/connection.BondState#BOND_STATE_BONDED
         */
        BOND_STATE_BONDED = 2
    }
    /**
     * The enum of major class of a bluetooth device.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.constant/constant.MajorClass
     */
    enum MajorClass {
        /**
         * Miscellaneous device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorClass#MAJOR_MISC
         */
        MAJOR_MISC = 0x0000,
        /**
         * Computer.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorClass#MAJOR_COMPUTER
         */
        MAJOR_COMPUTER = 0x0100,
        /**
         * Mobile phone.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorClass#MAJOR_PHONE
         */
        MAJOR_PHONE = 0x0200,
        /**
         * Network device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorClass#MAJOR_NETWORKING
         */
        MAJOR_NETWORKING = 0x0300,
        /**
         * Audio or video device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorClass#MAJOR_AUDIO_VIDEO
         */
        MAJOR_AUDIO_VIDEO = 0x0400,
        /**
         * Peripheral device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorClass#MAJOR_PERIPHERAL
         */
        MAJOR_PERIPHERAL = 0x0500,
        /**
         * Imaging device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorClass#MAJOR_IMAGING
         */
        MAJOR_IMAGING = 0x0600,
        /**
         * Wearable device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorClass#MAJOR_WEARABLE
         */
        MAJOR_WEARABLE = 0x0700,
        /**
         * Toy.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorClass#MAJOR_TOY
         */
        MAJOR_TOY = 0x0800,
        /**
         * Health device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorClass#MAJOR_HEALTH
         */
        MAJOR_HEALTH = 0x0900,
        /**
         * Unclassified device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorClass#MAJOR_UNCATEGORIZED
         */
        MAJOR_UNCATEGORIZED = 0x1F00
    }
    /**
     * The enum of major minor class of a bluetooth device.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass
     */
    enum MajorMinorClass {
        /**
         * The Minor Device Class field
         * Computer Major Class
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#COMPUTER_UNCATEGORIZED
         */
        COMPUTER_UNCATEGORIZED = 0x0100,
        /**
         * Desktop computer.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#COMPUTER_DESKTOP
         */
        COMPUTER_DESKTOP = 0x0104,
        /**
         * Server.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#COMPUTER_SERVER
         */
        COMPUTER_SERVER = 0x0108,
        /**
         * Laptop.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#COMPUTER_LAPTOP
         */
        COMPUTER_LAPTOP = 0x010C,
        /**
         * Hand-held computer.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#COMPUTER_HANDHELD_PC_PDA
         */
        COMPUTER_HANDHELD_PC_PDA = 0x0110,
        /**
         * Palmtop computer.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#COMPUTER_PALM_SIZE_PC_PDA
         */
        COMPUTER_PALM_SIZE_PC_PDA = 0x0114,
        /**
         * Wearable computer.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#COMPUTER_WEARABLE
         */
        COMPUTER_WEARABLE = 0x0118,
        /**
         * Tablet.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#COMPUTER_TABLET
         */
        COMPUTER_TABLET = 0x011C,
        /**
         * Phone Major Class
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PHONE_UNCATEGORIZED
         */
        PHONE_UNCATEGORIZED = 0x0200,
        /**
         * Portable phone.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PHONE_CELLULAR
         */
        PHONE_CELLULAR = 0x0204,
        /**
         * Cordless phone.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PHONE_CORDLESS
         */
        PHONE_CORDLESS = 0x0208,
        /**
         * Smartphone.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PHONE_SMART
         */
        PHONE_SMART = 0x020C,
        /**
         * Modem or gateway phone.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PHONE_MODEM_OR_GATEWAY
         */
        PHONE_MODEM_OR_GATEWAY = 0x0210,
        /**
         * ISDN phone.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PHONE_ISDN
         */
        PHONE_ISDN = 0x0214,
        /**
         * LAN/Network Access Point Major Class
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#NETWORK_FULLY_AVAILABLE
         */
        NETWORK_FULLY_AVAILABLE = 0x0300,
        /**
         * Device used on network 1 to 17.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#NETWORK_1_TO_17_UTILIZED
         */
        NETWORK_1_TO_17_UTILIZED = 0x0320,
        /**
         * Device used on network 17 to 33.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#NETWORK_17_TO_33_UTILIZED
         */
        NETWORK_17_TO_33_UTILIZED = 0x0340,
        /**
         * Device used on network 33 to 50.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#NETWORK_33_TO_50_UTILIZED
         */
        NETWORK_33_TO_50_UTILIZED = 0x0360,
        /**
         * Device used on network 60 to 67.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#NETWORK_60_TO_67_UTILIZED
         */
        NETWORK_60_TO_67_UTILIZED = 0x0380,
        /**
         * Device used on network 67 to 83.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#NETWORK_67_TO_83_UTILIZED
         */
        NETWORK_67_TO_83_UTILIZED = 0x03A0,
        /**
         * Device used on network 83 to 99.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#NETWORK_83_TO_99_UTILIZED
         */
        NETWORK_83_TO_99_UTILIZED = 0x03C0,
        /**
         * Device without network service.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#NETWORK_NO_SERVICE
         */
        NETWORK_NO_SERVICE = 0x03E0,
        /**
         * Unclassified audio or video device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_UNCATEGORIZED
         */
        AUDIO_VIDEO_UNCATEGORIZED = 0x0400,
        /**
         * Wearable audio or video headset.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_WEARABLE_HEADSET
         */
        AUDIO_VIDEO_WEARABLE_HEADSET = 0x0404,
        /**
         * Hands-free audio or video device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_HANDSFREE
         */
        AUDIO_VIDEO_HANDSFREE = 0x0408,
        /**
         * Audio or video microphone.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_MICROPHONE
         */
        AUDIO_VIDEO_MICROPHONE = 0x0410,
        /**
         * Audio or video loudspeaker.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_LOUDSPEAKER
         */
        AUDIO_VIDEO_LOUDSPEAKER = 0x0414,
        /**
         * Audio or video headphones.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_HEADPHONES
         */
        AUDIO_VIDEO_HEADPHONES = 0x0418,
        /**
         * Portable audio or video device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_PORTABLE_AUDIO
         */
        AUDIO_VIDEO_PORTABLE_AUDIO = 0x041C,
        /**
         * In-vehicle audio or video device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_CAR_AUDIO
         */
        AUDIO_VIDEO_CAR_AUDIO = 0x0420,
        /**
         * Audio or video STB device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_SET_TOP_BOX
         */
        AUDIO_VIDEO_SET_TOP_BOX = 0x0424,
        /**
         * High-fidelity speaker device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_HIFI_AUDIO
         */
        AUDIO_VIDEO_HIFI_AUDIO = 0x0428,
        /**
         * Video cassette recording (VCR) device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_VCR
         */
        AUDIO_VIDEO_VCR = 0x042C,
        /**
         * Camera.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_VIDEO_CAMERA
         */
        AUDIO_VIDEO_VIDEO_CAMERA = 0x0430,
        /**
         * Camcorder.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_CAMCORDER
         */
        AUDIO_VIDEO_CAMCORDER = 0x0434,
        /**
         * Audio or video monitor.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_VIDEO_MONITOR
         */
        AUDIO_VIDEO_VIDEO_MONITOR = 0x0438,
        /**
         * Video display or loudspeaker.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_VIDEO_DISPLAY_AND_LOUDSPEAKER
         */
        AUDIO_VIDEO_VIDEO_DISPLAY_AND_LOUDSPEAKER = 0x043C,
        /**
         * Video conferencing device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_VIDEO_CONFERENCING
         */
        AUDIO_VIDEO_VIDEO_CONFERENCING = 0x0440,
        /**
         * Audio or video gaming toy.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#AUDIO_VIDEO_VIDEO_GAMING_TOY
         */
        AUDIO_VIDEO_VIDEO_GAMING_TOY = 0x0448,
        /**
         * Peripheral Major Class
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PERIPHERAL_NON_KEYBOARD_NON_POINTING
         */
        PERIPHERAL_NON_KEYBOARD_NON_POINTING = 0x0500,
        /**
         * Keyboard device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PERIPHERAL_KEYBOARD
         */
        PERIPHERAL_KEYBOARD = 0x0540,
        /**
         * Pointing peripheral device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PERIPHERAL_POINTING_DEVICE
         */
        PERIPHERAL_POINTING_DEVICE = 0x0580,
        /**
         * Keyboard pointing device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PERIPHERAL_KEYBOARD_POINTING
         */
        PERIPHERAL_KEYBOARD_POINTING = 0x05C0,
        /**
         * Unclassified peripheral device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PERIPHERAL_UNCATEGORIZED
         */
        PERIPHERAL_UNCATEGORIZED = 0x0500,
        /**
         * Peripheral joystick.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PERIPHERAL_JOYSTICK
         */
        PERIPHERAL_JOYSTICK = 0x0504,
        /**
         * Peripheral game pad.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PERIPHERAL_GAMEPAD
         */
        PERIPHERAL_GAMEPAD = 0x0508,
        /**
         * Peripheral remote control device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PERIPHERAL_REMOTE_CONTROL
         */
        PERIPHERAL_REMOTE_CONTROL = 0x05C0,
        /**
         * Peripheral sensing device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PERIPHERAL_SENSING_DEVICE
         */
        PERIPHERAL_SENSING_DEVICE = 0x0510,
        /**
         * Peripheral digitizer tablet.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PERIPHERAL_DIGITIZER_TABLET
         */
        PERIPHERAL_DIGITIZER_TABLET = 0x0514,
        /**
         * Peripheral card reader.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PERIPHERAL_CARD_READER
         */
        PERIPHERAL_CARD_READER = 0x0518,
        /**
         * Peripheral digital pen.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PERIPHERAL_DIGITAL_PEN
         */
        PERIPHERAL_DIGITAL_PEN = 0x051C,
        /**
         * Peripheral RFID scanner.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PERIPHERAL_SCANNER_RFID
         */
        PERIPHERAL_SCANNER_RFID = 0x0520,
        /**
         * Gesture input device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#PERIPHERAL_GESTURAL_INPUT
         */
        PERIPHERAL_GESTURAL_INPUT = 0x0522,
        /**
         * Imaging Major Class
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#IMAGING_UNCATEGORIZED
         */
        IMAGING_UNCATEGORIZED = 0x0600,
        /**
         * Imaging display device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#IMAGING_DISPLAY
         */
        IMAGING_DISPLAY = 0x0610,
        /**
         * Imaging camera device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#IMAGING_CAMERA
         */
        IMAGING_CAMERA = 0x0620,
        /**
         * Imaging scanner.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#IMAGING_SCANNER
         */
        IMAGING_SCANNER = 0x0640,
        /**
         * Imaging printer.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#IMAGING_PRINTER
         */
        IMAGING_PRINTER = 0x0680,
        /**
         * Wearable Major Class
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#WEARABLE_UNCATEGORIZED
         */
        WEARABLE_UNCATEGORIZED = 0x0700,
        /**
         * Smart watch.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#WEARABLE_WRIST_WATCH
         */
        WEARABLE_WRIST_WATCH = 0x0704,
        /**
         * Wearable pager.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#WEARABLE_PAGER
         */
        WEARABLE_PAGER = 0x0708,
        /**
         * Smart jacket.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#WEARABLE_JACKET
         */
        WEARABLE_JACKET = 0x070C,
        /**
         * Wearable helmet.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#WEARABLE_HELMET
         */
        WEARABLE_HELMET = 0x0710,
        /**
         * Wearable glasses.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#WEARABLE_GLASSES
         */
        WEARABLE_GLASSES = 0x0714,
        /**
         * Minor Device Class field - Toy Major Class
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#TOY_UNCATEGORIZED
         */
        TOY_UNCATEGORIZED = 0x0800,
        /**
         * Toy robot.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#TOY_ROBOT
         */
        TOY_ROBOT = 0x0804,
        /**
         * Toy vehicle.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#TOY_VEHICLE
         */
        TOY_VEHICLE = 0x0808,
        /**
         * Humanoid toy doll.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#TOY_DOLL_ACTION_FIGURE
         */
        TOY_DOLL_ACTION_FIGURE = 0x080C,
        /**
         * Toy controller.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#TOY_CONTROLLER
         */
        TOY_CONTROLLER = 0x0810,
        /**
         * Toy gaming device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#TOY_GAME
         */
        TOY_GAME = 0x0814,
        /**
         * Minor Device Class field - Health
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_UNCATEGORIZED
         */
        HEALTH_UNCATEGORIZED = 0x0900,
        /**
         * Blood pressure device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_BLOOD_PRESSURE
         */
        HEALTH_BLOOD_PRESSURE = 0x0904,
        /**
         * Thermometer.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_THERMOMETER
         */
        HEALTH_THERMOMETER = 0x0908,
        /**
         * Body scale.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_WEIGHING
         */
        HEALTH_WEIGHING = 0x090C,
        /**
         * Blood glucose monitor.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_GLUCOSE
         */
        HEALTH_GLUCOSE = 0x0910,
        /**
         * Pulse oximeter.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_PULSE_OXIMETER
         */
        HEALTH_PULSE_OXIMETER = 0x0914,
        /**
         * Heart rate monitor.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_PULSE_RATE
         */
        HEALTH_PULSE_RATE = 0x0918,
        /**
         * Health data display.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_DATA_DISPLAY
         */
        HEALTH_DATA_DISPLAY = 0x091C,
        /**
         * Step counter.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_STEP_COUNTER
         */
        HEALTH_STEP_COUNTER = 0x0920,
        /**
         * Body composition analyzer.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_BODY_COMPOSITION_ANALYZER
         */
        HEALTH_BODY_COMPOSITION_ANALYZER = 0x0924,
        /**
         * Hygrometer.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_PEAK_FLOW_MONITOR
         */
        HEALTH_PEAK_FLOW_MONITOR = 0x0928,
        /**
         * Medication monitor.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_MEDICATION_MONITOR
         */
        HEALTH_MEDICATION_MONITOR = 0x092C,
        /**
         * Prosthetic knee.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_KNEE_PROSTHESIS
         */
        HEALTH_KNEE_PROSTHESIS = 0x0930,
        /**
         * Prosthetic ankle.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_ANKLE_PROSTHESIS
         */
        HEALTH_ANKLE_PROSTHESIS = 0x0934,
        /**
         * Generic health management device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_GENERIC_HEALTH_MANAGER
         */
        HEALTH_GENERIC_HEALTH_MANAGER = 0x0938,
        /**
         * Personal mobility device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.MajorMinorClass#HEALTH_PERSONAL_MOBILITY_DEVICE
         */
        HEALTH_PERSONAL_MOBILITY_DEVICE = 0x093C
    }
    /**
     * The enum of a2dp playing state.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.a2dp/a2dp.PlayingState
     */
    enum PlayingState {
        /**
         * Not playing.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.a2dp/a2dp.PlayingState#STATE_NOT_PLAYING
         */
        STATE_NOT_PLAYING,
        /**
         * Playing.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.a2dp/a2dp.PlayingState#STATE_PLAYING
         */
        STATE_PLAYING
    }
    /**
     * The enum of profile id.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 9
     * @deprecated since 10
     * @useinstead ohos.bluetooth.constant/constant.ProfileId
     */
    enum ProfileId {
        /**
         * A2DP profile.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.ProfileId#PROFILE_A2DP_SOURCE
         */
        PROFILE_A2DP_SOURCE = 1,
        /**
         * HFP profile.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.ProfileId#PROFILE_HANDSFREE_AUDIO_GATEWAY
         */
        PROFILE_HANDS_FREE_AUDIO_GATEWAY = 4,
        /**
         * Human Interface Device (HID) profile.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.ProfileId#PROFILE_HID_HOST
         */
        PROFILE_HID_HOST = 6,
        /**
         * PAN profile.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 9
         * @deprecated since 10
         * @useinstead ohos.bluetooth.constant/constant.ProfileId#PROFILE_PAN_NETWORK
         */
        PROFILE_PAN_NETWORK = 7
    }
}
export default bluetoothManager;
