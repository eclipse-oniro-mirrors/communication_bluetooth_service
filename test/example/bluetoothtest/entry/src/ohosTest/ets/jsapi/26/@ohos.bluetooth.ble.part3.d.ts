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
/* Continuation part 3 of @ohos.bluetooth.ble.d.ts (reference only). */
     * Describes the Gatt service.
     *
     * @typedef GattService
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface GattService {
        /**
         * The UUID of a GattService instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of a GattService instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of a GattService instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceUuid: string;
        /**
         * Indicates whether the GattService instance is primary or secondary.
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates whether the GattService instance is primary or secondary.
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates whether the GattService instance is primary or secondary.
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        isPrimary: boolean;
        /**
         * The {@link BLECharacteristic} list belongs to this GattService instance
         *
         * @type { Array<BLECharacteristic>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The {@link BLECharacteristic} list belongs to this GattService instance
         *
         * @type { Array<BLECharacteristic>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The {@link BLECharacteristic} list belongs to this GattService instance
         *
         * @type { Array<BLECharacteristic>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        characteristics: Array<BLECharacteristic>;
        /**
         * The list of GATT services contained in the service
         *
         * @type { ?Array<GattService>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The list of GATT services contained in the service
         *
         * @type { ?Array<GattService>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        includeServices?: Array<GattService>;
    }
    /**
     * Describes the Gatt characteristic.
     *
     * @typedef BLECharacteristic
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the Gatt characteristic.
     *
     * @typedef BLECharacteristic
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the Gatt characteristic.
     *
     * @typedef BLECharacteristic
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface BLECharacteristic {
        /**
         * The UUID of the {@link GattService} instance to which the characteristic belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of the {@link GattService} instance to which the characteristic belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of the {@link GattService} instance to which the characteristic belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceUuid: string;
        /**
         * The UUID of a BLECharacteristic instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of a BLECharacteristic instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of a BLECharacteristic instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        characteristicUuid: string;
        /**
         * The value of a BLECharacteristic instance
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The value of a BLECharacteristic instance
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The value of a BLECharacteristic instance
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        characteristicValue: ArrayBuffer;
        /**
         * The list of {@link BLEDescriptor} contained in the characteristic
         *
         * @type { Array<BLEDescriptor>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The list of {@link BLEDescriptor} contained in the characteristic
         *
         * @type { Array<BLEDescriptor>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The list of {@link BLEDescriptor} contained in the characteristic
         *
         * @type { Array<BLEDescriptor>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        descriptors: Array<BLEDescriptor>;
        /**
         * The properties of a BLECharacteristic instance
         *
         * @type { ?GattProperties}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The properties of a BLECharacteristic instance
         *
         * @type { ?GattProperties}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The properties of a BLECharacteristic instance
         *
         * @type { ?GattProperties}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        properties?: GattProperties;
        /**
         * The characteristic value handle of a BLECharacteristic instance
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 18
         */
        characteristicValueHandle?: number;
        /**
         * The permissions of a BLECharacteristic instance. The default value is Readable and Writable.
         *
         * @type { ?GattPermissions}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 20
         */
        permissions?: GattPermissions;
    }
    /**
     * Describes the Gatt descriptor.
     *
     * @typedef BLEDescriptor
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the Gatt descriptor.
     *
     * @typedef BLEDescriptor
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the Gatt descriptor.
     *
     * @typedef BLEDescriptor
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface BLEDescriptor {
        /**
         * The UUID of the {@link GattService} instance to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of the {@link GattService} instance to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of the {@link GattService} instance to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceUuid: string;
        /**
         * The UUID of the {@link BLECharacteristic} instance to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of the {@link BLECharacteristic} instance to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of the {@link BLECharacteristic} instance to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        characteristicUuid: string;
        /**
         * The UUID of the BLEDescriptor instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of the BLEDescriptor instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of the BLEDescriptor instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        descriptorUuid: string;
        /**
         * The value of the BLEDescriptor instance
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The value of the BLEDescriptor instance
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The value of the BLEDescriptor instance
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        descriptorValue: ArrayBuffer;
        /**
         * The descriptor handle of the BLEDescriptor instance
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 18
         */
        descriptorHandle?: number;
        /**
         * The permissions of a BLEDescriptor instance. The default value is Readable and Writable.
         *
         * @type { ?GattPermissions}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 20
         */
        permissions?: GattPermissions;
    }
    /**
     * Describes the value of the indication or notification sent by the Gatt server.
     *
     * @typedef NotifyCharacteristic
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the value of the indication or notification sent by the Gatt server.
     *
     * @typedef NotifyCharacteristic
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the value of the indication or notification sent by the Gatt server.
     *
     * @typedef NotifyCharacteristic
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface NotifyCharacteristic {
        /**
         * The UUID of the {@link GattService} instance to which the characteristic belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of the {@link GattService} instance to which the characteristic belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of the {@link GattService} instance to which the characteristic belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceUuid: string;
        /**
         * The UUID of a NotifyCharacteristic instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of a NotifyCharacteristic instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of a NotifyCharacteristic instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        characteristicUuid: string;
        /**
         * The value of a NotifyCharacteristic instance
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The value of a NotifyCharacteristic instance
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The value of a NotifyCharacteristic instance
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        characteristicValue: ArrayBuffer;
        /**
         * Specifies whether to request confirmation from the BLE peripheral device (indication) or
         * send a notification. Value {@code true} indicates the former and {@code false} indicates the latter.
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Specifies whether to request confirmation from the BLE peripheral device (indication) or
         * send a notification. Value {@code true} indicates the former and {@code false} indicates the latter.
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        confirm: boolean;
    }
    /**
     * Describes the parameters of the Gatt client's characteristic read request.
     *
     * @typedef CharacteristicReadRequest
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the parameters of the Gatt client's characteristic read request.
     *
     * @typedef CharacteristicReadRequest
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the parameters of the Gatt client's characteristic read request.
     *
     * @typedef CharacteristicReadRequest
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface CharacteristicReadRequest {
        /**
         * Indicates the address of the client that initiates the read request
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the address of the client that initiates the read request
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates the address of the client that initiates the read request
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        deviceId: string;
        /**
         * The Id of the read request
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The Id of the read request
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The Id of the read request
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        transId: number;
        /**
         * Indicates the byte offset of the start position for reading characteristic value
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the byte offset of the start position for reading characteristic value
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        offset: number;
        /**
         * The UUID of a CharacteristicReadRequest instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of a CharacteristicReadRequest instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of a CharacteristicReadRequest instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        characteristicUuid: string;
        /**
         * The UUID of the service to which the characteristic belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of the service to which the characteristic belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of the service to which the characteristic belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceUuid: string;
    }
    /**
     * Describes the parameters of the of the Gatt client's characteristic write request.
     *
     * @typedef CharacteristicWriteRequest
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the parameters of the of the Gatt client's characteristic write request.
     *
     * @typedef CharacteristicWriteRequest
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the parameters of the of the Gatt client's characteristic write request.
     *
     * @typedef CharacteristicWriteRequest
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface CharacteristicWriteRequest {
        /**
         * Indicates the address of the client that initiates the write request
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the address of the client that initiates the write request
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates the address of the client that initiates the write request
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        deviceId: string;
        /**
         * The Id of the write request
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The Id of the write request
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The Id of the write request
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        transId: number;
        /**
         * Indicates the byte offset of the start position for writing characteristic value
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the byte offset of the start position for writing characteristic value
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        offset: number;
        /**
         * Whether this request should be pending for later operation
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Whether this request should be pending for later operation
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        isPrepared: boolean;
        /**
         * Whether the remote client need a response
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Whether the remote client need a response
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        needRsp: boolean;
        /**
         * Indicates the value to be written
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the value to be written
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates the value to be written
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        value: ArrayBuffer;
        /**
         * The UUID of a CharacteristicWriteRequest instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of a CharacteristicWriteRequest instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of a CharacteristicWriteRequest instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        characteristicUuid: string;
        /**
         * The UUID of the service to which the characteristic belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of the service to which the characteristic belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of the service to which the characteristic belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceUuid: string;
    }
    /**
     * Describes the parameters of the Gatt client's descriptor read request.
     *
     * @typedef DescriptorReadRequest
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the parameters of the Gatt client's descriptor read request.
     *
     * @typedef DescriptorReadRequest
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the parameters of the Gatt client's descriptor read request.
     *
     * @typedef DescriptorReadRequest
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface DescriptorReadRequest {
        /**
         * Indicates the address of the client that initiates the read request
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the address of the client that initiates the read request
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates the address of the client that initiates the read request
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        deviceId: string;
        /**
         * The Id of the read request
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The Id of the read request
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The Id of the read request
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        transId: number;
        /**
         * Indicates the byte offset of the start position for reading characteristic value
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the byte offset of the start position for reading characteristic value
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        offset: number;
        /**
         * The UUID of a DescriptorReadRequest instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of a DescriptorReadRequest instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of a DescriptorReadRequest instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        descriptorUuid: string;
        /**
         * The UUID of the characteristic to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of the characteristic to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of the characteristic to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        characteristicUuid: string;
        /**
         * The UUID of the service to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of the service to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of the service to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceUuid: string;
    }
    /**
     * Describes the parameters of the Gatt client's characteristic write request.
     *
     * @typedef DescriptorWriteRequest
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the parameters of the Gatt client's characteristic write request.
     *
     * @typedef DescriptorWriteRequest
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the parameters of the Gatt client's characteristic write request.
     *
     * @typedef DescriptorWriteRequest
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface DescriptorWriteRequest {
        /**
         * Indicates the address of the client that initiates the write request
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the address of the client that initiates the write request
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates the address of the client that initiates the write request
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        deviceId: string;
        /**
         * The Id of the write request
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The Id of the write request
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The Id of the write request
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        transId: number;
        /**
         * Indicates the byte offset of the start position for writing characteristic value
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the byte offset of the start position for writing characteristic value
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        offset: number;
        /**
         * Whether this request should be pending for later operation
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Whether this request should be pending for later operation
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        isPrepared: boolean;
        /**
         * Whether the remote client need a response
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Whether the remote client need a response
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        needRsp: boolean;
        /**
         * Indicates the value to be written
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the value to be written
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates the value to be written
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        value: ArrayBuffer;
        /**
         * The UUID of a DescriptorWriteRequest instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of a DescriptorWriteRequest instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of a DescriptorWriteRequest instance
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        descriptorUuid: string;
        /**
         * The UUID of the characteristic to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of the characteristic to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of the characteristic to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        characteristicUuid: string;
        /**
         * The UUID of the service to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The UUID of the service to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The UUID of the service to which the descriptor belongs
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceUuid: string;
    }
    /**
     * Describes the parameters of a response send by the server to a specified read or write request.
     *
     * @typedef ServerResponse
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the parameters of a response send by the server to a specified read or write request.
     *
     * @typedef ServerResponse
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the parameters of a response send by the server to a specified read or write request.
     *
     * @typedef ServerResponse
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface ServerResponse {
        /**
         * Indicates the address of the client to which to send the response
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the address of the client to which to send the response
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates the address of the client to which to send the response
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        deviceId: string;
        /**
         * The Id of the write request
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The Id of the write request
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The Id of the write request
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        transId: number;
        /**
         * Indicates the status of the read or write request, set this parameter to '0' in normal cases
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the status of the read or write request, set this parameter to '0' in normal cases
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates the status of the read or write request, set this parameter to '0' in normal cases
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        status: number;
        /**
         * Indicates the byte offset of the start position for reading or writing operation
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the byte offset of the start position for reading or writing operation
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        offset: number;
        /**
         * Indicates the value to be sent
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the value to be sent
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates the value to be sent
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        value: ArrayBuffer;
    }
    /**
     * Describes the Gatt profile connection state.
     *
     * @typedef BLEConnectionChangeState
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the Gatt profile connection state.
     *
     * @typedef BLEConnectionChangeState
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the Gatt profile connection state.
     *
     * @typedef BLEConnectionChangeState
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface BLEConnectionChangeState {
        /**
         * Indicates the peer device address
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the peer device address
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates the peer device address
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        deviceId: string;
        /**
         * Connection state of the Gatt profile
         *
         * @type { ProfileConnectionState}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Connection state of the Gatt profile
         *
         * @type { ProfileConnectionState}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Connection state of the Gatt profile
         *
         * @type { ProfileConnectionState}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        state: ProfileConnectionState;
        /**
         * Reason of the disconnection of the gatt connection.
         *
         * @type { ?GattDisconnectReason}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 20
         */
        reason?: GattDisconnectReason;
    }
    /**
     * Describes the contents of the scan results.
     *
     * @typedef ScanResult
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the contents of the scan results.
     *
     * @typedef ScanResult
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the contents of the scan results.
     *
     * @typedef ScanResult
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface ScanResult {
        /**
         * Address of the scanned device
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Address of the scanned device
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Address of the scanned device
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        deviceId: string;
        /**
         * The address object of a BLE peripheral device, including the address type.
         *
         * @type { ?BluetoothAddress}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 23
         */
        address?: BluetoothAddress;
        /**
         * RSSI of the remote device
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * RSSI of the remote device
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * RSSI of the remote device
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        rssi: number;
        /**
         * The raw data of broadcast packet
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The raw data of broadcast packet
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The raw data of broadcast packet
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        data: ArrayBuffer;
        /**
         * The local name of the BLE device
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The local name of the BLE device
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The local name of the BLE device
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        deviceName: string;
        /**
         * Connectable of the remote device
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Connectable of the remote device
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Connectable of the remote device
         *
         * @type { boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        connectable: boolean;
        /**
         * This field is used to identify the discovery mode and supported capabilities of the peer device.
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 22
         */
        advertiseFlags?: number;
        /**
         * Map of manufacturer data.
         *
         * @type { ?Map<number, Uint8Array>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 22
         */
        manufacturerDataMap?: Map<number, Uint8Array>;
        /**
         * Map of service data.
         *
         * @type { ?Map<string, Uint8Array>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 22
         */
        serviceDataMap?: Map<string, Uint8Array>;
        /**
         * The list of service uuid.
         *
         * @type { ?string[]}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 22
         */
        serviceUuids?: string[];
        /**
         * The tx power level of the packet in dBm.
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 22
         */
        txPowerLevel?: number;
        /**
         * Map of advertising data fields.
         *
         * @type { ?Map<number, Uint8Array>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 22
         */
        advertisingDataMap?: Map<number, Uint8Array>;
    }
    /**
     * Describes the contents of the scan report.
     *
     * @typedef ScanReport
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 15
     */
    interface ScanReport {
        /**
         * The type of scan report
         *
         * @type { ScanReportType}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 15
         */
        reportType: ScanReportType;
        /**
         * Describes the contents of the scan results.
         *
         * @type { Array<ScanResult>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 15
         */
        scanResult: Array<ScanResult>;
    }
    /**
     * Describes the settings for BLE advertising.
     *
     * @typedef AdvertiseSetting
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the settings for BLE advertising.
     *
     * @typedef AdvertiseSetting
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the settings for BLE advertising.
     *
     * @typedef AdvertiseSetting
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface AdvertiseSetting {
        /**
         * Minimum slot value for the advertising interval, which is {@code 32} (20 ms)
         * Maximum slot value for the advertising interval, which is {@code 16777215} (10485.759375s)
         * Default slot value for the advertising interval, which is {@code 1600} (1s)
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Minimum slot value for the advertising interval, which is {@code 32} (20 ms)
         * Maximum slot value for the advertising interval, which is {@code 16777215} (10485.759375s)
         * Default slot value for the advertising interval, which is {@code 1600} (1s)
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        interval?: number;
        /**
         * Minimum transmission power level for advertising, which is {@code -127}
         * Maximum transmission power level for advertising, which is {@code 1}
         * Default transmission power level for advertising, which is {@code -7}
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Minimum transmission power level for advertising, which is {@code -127}
         * Maximum transmission power level for advertising, which is {@code 1}
         * Default transmission power level for advertising, which is {@code -7}
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        txPower?: number;
        /**
         * Indicates whether the BLE is connectable, default is {@code true}
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates whether the BLE is connectable, default is {@code true}
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates whether the BLE is connectable, default is {@code true}
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        connectable?: boolean;
    }
    /**
     * Describes the advertising data.
     *
     * @typedef AdvertiseData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the advertising data.
     *
     * @typedef AdvertiseData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the advertising data.
     *
     * @typedef AdvertiseData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface AdvertiseData {
        /**
         * The specified service UUID list to this advertisement
         *
         * @type { Array<string>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The specified service UUID list to this advertisement
         *
         * @type { Array<string>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The specified service UUID list to this advertisement
         *
         * @type { Array<string>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
