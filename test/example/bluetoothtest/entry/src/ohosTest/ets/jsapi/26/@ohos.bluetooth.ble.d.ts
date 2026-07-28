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
         */
        serviceUuids: Array<string>;
        /**
         * The specified manufacturer data list to this advertisement
         *
         * @type { Array<ManufactureData>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The specified manufacturer data list to this advertisement
         *
         * @type { Array<ManufactureData>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The specified manufacturer data list to this advertisement
         *
         * @type { Array<ManufactureData>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        manufactureData: Array<ManufactureData>;
        /**
         * The specified service data list to this advertisement
         *
         * @type { Array<ServiceData>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The specified service data list to this advertisement
         *
         * @type { Array<ServiceData>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The specified service data list to this advertisement
         *
         * @type { Array<ServiceData>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceData: Array<ServiceData>;
        /**
         * Indicates whether the device name will be included in the advertisement packet.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates whether the device name will be included in the advertisement packet.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates whether the device name will be included in the advertisement packet.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        includeDeviceName?: boolean;
        /**
         * Indicates whether the tx power will be included in the advertisement packet.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 18
         */
        includeTxPower?: boolean;
        /**
         * Indicates the local name data type in the advertisement packet. If both the property and
         * {@link AdvertiseData#includeDeviceName} property are used together,
         * the {@link AdvertiseData#advertiseName} property will ultimately take effect.
         *
         * @permission ohos.permission.MANAGE_BLUETOOTH_ADVERTISER_NAME
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 23
         */
        advertiseName?: string;
    }
    /**
     * Describes the advertising parameters.
     *
     * @typedef AdvertisingParams
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 11
     */
    /**
     * Describes the advertising parameters.
     *
     * @typedef AdvertisingParams
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    interface AdvertisingParams {
        /**
         * Indicates the advertising settings.
         *
         * @type { AdvertiseSetting}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 11
         */
        /**
         * Indicates the advertising settings.
         *
         * @type { AdvertiseSetting}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 13
         */
        advertisingSettings: AdvertiseSetting;
        /**
         * Indicates the advertising data.
         *
         * @type { AdvertiseData}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 11
         */
        /**
         * Indicates the advertising data.
         *
         * @type { AdvertiseData}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 13
         */
        advertisingData: AdvertiseData;
        /**
         * Indicates the advertising response.
         *
         * @type { ?AdvertiseData}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 11
         */
        /**
         * Indicates the advertising response.
         *
         * @type { ?AdvertiseData}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 13
         */
        advertisingResponse?: AdvertiseData;
        /**
         * Indicates the duration for advertising continuously.
         * The duration, in 10ms unit. Valid range is from 1 (10ms) to 65535 (655,350 ms).
         * If this parameter is not specified or is set to 0, advertisement is continuously sent.
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 11
         */
        duration?: number;
    }
    /**
     * Parameter for dynamically enable advertising.
     *
     * @typedef AdvertisingEnableParams
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 11
     */
    interface AdvertisingEnableParams {
        /**
         * Indicates the ID of current advertising.
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 11
         */
        advertisingId: number;
        /**
         * Indicates the duration for advertising continuously.
         * The duration, in 10ms unit. Valid range is from 1 (10ms) to 65535 (655,350 ms).
         * If this parameter is not specified or is set to 0, advertise is continuously sent.
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 11
         */
        duration?: number;
    }
    /**
     * Parameter for dynamically disable advertising.
     *
     * @typedef AdvertisingDisableParams
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 11
     */
    interface AdvertisingDisableParams {
        /**
         * Indicates the ID of current advertising.
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 11
         */
        advertisingId: number;
    }
    /**
     * Advertising state change information.
     *
     * @typedef AdvertisingStateChangeInfo
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 11
     */
    /**
     * Advertising state change information.
     *
     * @typedef AdvertisingStateChangeInfo
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    interface AdvertisingStateChangeInfo {
        /**
         * Indicates the ID of current advertising.
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 11
         */
        /**
         * Indicates the ID of current advertising.
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 13
         */
        advertisingId: number;
        /**
         * Indicates the advertising state.
         *
         * @type { AdvertisingState}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 11
         */
        /**
         * Indicates the advertising state.
         *
         * @type { AdvertisingState}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 13
         */
        state: AdvertisingState;
    }
    /**
     * Describes the manufacturer data.
     *
     * @typedef ManufactureData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the manufacturer data.
     *
     * @typedef ManufactureData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the manufacturer data.
     *
     * @typedef ManufactureData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface ManufactureData {
        /**
         * Indicates the manufacturer ID assigned by Bluetooth SIG
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the manufacturer ID assigned by Bluetooth SIG
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates the manufacturer ID assigned by Bluetooth SIG
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        manufactureId: number;
        /**
         * Indicates the manufacturer data to add
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the manufacturer data to add
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates the manufacturer data to add
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        manufactureValue: ArrayBuffer;
    }
    /**
     * Describes the service data.
     *
     * @typedef ServiceData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the service data.
     *
     * @typedef ServiceData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the service data.
     *
     * @typedef ServiceData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface ServiceData {
        /**
         * Indicates the UUID of the service data to add
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the UUID of the service data to add
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates the UUID of the service data to add
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceUuid: string;
        /**
         * Indicates the service data to add
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the service data to add
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicates the service data to add
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceValue: ArrayBuffer;
    }
    /**
     * Describes the criteria for filtering scanning results can be set.
     *
     * @typedef ScanFilter
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the criteria for filtering scanning results can be set.
     *
     * @typedef ScanFilter
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the criteria for filtering scanning results can be set.
     *
     * @typedef ScanFilter
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface ScanFilter {
        /**
         * The address of a BLE peripheral device
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The address of a BLE peripheral device
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The address of a BLE peripheral device
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        deviceId?: string;
        /**
         * The address object of a BLE peripheral device, including the address type.
         *
         * @type { ?BluetoothAddress}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 23
         */
        address?: BluetoothAddress;
        /**
         * Identity Resolving Key of BLE peripheral device.
         * {@link ScanFilter#irk} needs to be used with {@link ScanFilter#address}.
         *
         * @type { ?Uint8Array}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 23
         */
        irk?: Uint8Array;
        /**
         * The name of a BLE peripheral device
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The name of a BLE peripheral device
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The name of a BLE peripheral device
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        name?: string;
        /**
         * The service UUID of a BLE peripheral device
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * The service UUID of a BLE peripheral device
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * The service UUID of a BLE peripheral device
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceUuid?: string;
        /**
         * Service UUID mask.
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Service UUID mask.
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Service UUID mask.
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceUuidMask?: string;
        /**
         * Service solicitation UUID.
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Service solicitation UUID.
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Service solicitation UUID.
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceSolicitationUuid?: string;
        /**
         * Service solicitation UUID mask.
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Service solicitation UUID mask.
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Service solicitation UUID mask.
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceSolicitationUuidMask?: string;
        /**
         * Service data.
         *
         * @type { ?ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Service data.
         *
         * @type { ?ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Service data.
         *
         * @type { ?ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceData?: ArrayBuffer;
        /**
         * Service data mask.
         *
         * @type { ?ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Service data mask.
         *
         * @type { ?ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Service data mask.
         *
         * @type { ?ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        serviceDataMask?: ArrayBuffer;
        /**
         * Manufacture id.
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Manufacture id.
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Manufacture id.
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        manufactureId?: number;
        /**
         * Manufacture data.
         *
         * @type { ?ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Manufacture data.
         *
         * @type { ?ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Manufacture data.
         *
         * @type { ?ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        manufactureData?: ArrayBuffer;
        /**
         * Manufacture data mask.
         *
         * @type { ?ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Manufacture data mask.
         *
         * @type { ?ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Manufacture data mask.
         *
         * @type { ?ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        manufactureDataMask?: ArrayBuffer;
        /**
         * RSSI threshold for filtering advertising that pass through.
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 23
         */
        rssiThreshold?: number;
    }
    /**
     * Describes the parameters for scan.
     *
     * @typedef ScanOptions
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the parameters for scan.
     *
     * @typedef ScanOptions
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the parameters for scan.
     *
     * @typedef ScanOptions
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface ScanOptions {
        /**
         * Time of delay for reporting the scan result
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Time of delay for reporting the scan result
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Time of delay for reporting the scan result
         *
         * @type { ?number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        interval?: number;
        /**
         * Bluetooth LE scan mode
         *
         * @type { ?ScanDuty}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Bluetooth LE scan mode
         *
         * @type { ?ScanDuty}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Bluetooth LE scan mode
         *
         * @type { ?ScanDuty}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        dutyMode?: ScanDuty;
        /**
         * Match mode for Bluetooth LE scan filters hardware match
         *
         * @type { ?MatchMode}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Match mode for Bluetooth LE scan filters hardware match
         *
         * @type { ?MatchMode}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        matchMode?: MatchMode;
        /**
         * Physical Layer used during scan.
         *
         * @type { ?PhyType}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Physical Layer used during scan.
         *
         * @type { ?PhyType}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        phyType?: PhyType;
        /**
         * Report mode used during scan.
         *
         * @type { ?ScanReportMode}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 15
         */
        reportMode?: ScanReportMode;
    }
    /**
     * Describes the properties of a gatt characteristic.
     *
     * @typedef GattProperties
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Describes the properties of a gatt characteristic.
     *
     * @typedef GattProperties
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Describes the properties of a gatt characteristic.
     *
     * @typedef GattProperties
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    interface GattProperties {
        /**
         * Support write property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Support write property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Support write property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        write?: boolean;
        /**
         * Support write no response property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Support write no response property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Support write no response property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        writeNoResponse?: boolean;
        /**
         * Support read property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Support read property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Support read property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        read?: boolean;
        /**
         * Support notify property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Support notify property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Support notify property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        notify?: boolean;
        /**
         * Support indicate property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Support indicate property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        indicate?: boolean;
        /**
         * Support broadcast property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 20
         */
        broadcast?: boolean;
        /**
         * Support authenticated signed write property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 20
         */
        authenticatedSignedWrite?: boolean;
        /**
         * Support extended properties property of the characteristic.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 20
         */
        extendedProperties?: boolean;
    }
    /**
     * The enum of gatt characteristic write type
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * The enum of gatt characteristic write type
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * The enum of gatt characteristic write type
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    enum GattWriteType {
        /**
         * Write characteristic with response.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Write characteristic with response.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Write characteristic with response.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        WRITE = 1,
        /**
         * Write characteristic without response.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Write characteristic without response.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Write characteristic without response.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        WRITE_NO_RESPONSE = 2
    }
    /**
     * The enum of scan duty.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * The enum of scan duty.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * The enum of scan duty.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    enum ScanDuty {
        /**
         * low power mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * low power mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * low power mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        SCAN_MODE_LOW_POWER = 0,
        /**
         * balanced power mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * balanced power mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * balanced power mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        SCAN_MODE_BALANCED = 1,
        /**
         * Scan using highest duty cycle
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Scan using highest duty cycle
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Scan using highest duty cycle
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        SCAN_MODE_LOW_LATENCY = 2
    }
    /**
     * The enum of BLE match mode.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * The enum of BLE match mode.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    enum MatchMode {
        /**
         * aggressive mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * aggressive mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        MATCH_MODE_AGGRESSIVE = 1,
        /**
         * sticky mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * sticky mode
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        MATCH_MODE_STICKY = 2
    }
    /**
     * The enum of BLE advertising state.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 11
     */
    /**
     * The enum of BLE advertising state.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    enum AdvertisingState {
        /**
         * advertising started.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 11
         */
        /**
         * advertising started.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 13
         */
        STARTED = 1,
        /**
         * advertising temporarily enabled.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 11
         */
        ENABLED = 2,
        /**
         * advertising temporarily disabled.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 11
         */
        DISABLED = 3,
        /**
         * advertising stopped.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 11
         */
        /**
         * advertising stopped.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 13
         */
        STOPPED = 4
    }
    /**
     * Phy type used during scan.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Phy type used during scan.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    enum PhyType {
        /**
         * Use 1M phy for scanning.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Use 1M phy for scanning.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        PHY_LE_1M = 1,
        /**
         * Use all supported Phys for scanning.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Use all supported Phys for scanning.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        PHY_LE_ALL_SUPPORTED = 255
    }
    /**
     * Report mode used during scan.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 15
     */
    enum ScanReportMode {
        /**
         * In normal mode, the advertisement packet is reported immediately after being scanned.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 15
         */
        NORMAL = 1,
        /**
         * Enables the batch mode in which advertisement packets are sent after the interval specified by {@link
         * ScanOptions#interval}.
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 19
         */
        BATCH = 2,
        /**
         * In low sensitivity fence mode, the advertisement packets are reported only when they are received for
         * the first time and lost for the last time. The reception sensitivity is low.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 18
         */
        FENCE_SENSITIVITY_LOW = 10,
        /**
         * In high sensitivity fence mode, the advertisement packets are reported only when they are received for
         * the first time and lost for the last time. The reception sensitivity is high.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 18
         */
        FENCE_SENSITIVITY_HIGH = 11
    }
    /**
     * Scan report type used during scan.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 15
     */
    enum ScanReportType {
        /**
         * The found of advertisement packet.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 15
         */
        ON_FOUND = 1,
        /**
         * The lost of advertisement packet.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 15
         */
        ON_LOST = 2,
        /**
         * The type of advertisement packet reported in batch mode.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 19
         */
        ON_BATCH = 3
    }
    /**
     * The Profile of the BLE protocol.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 21
     */
    enum BleProfile {
        /**
         * Indicates the profile type of the gatt, including gatt client and gatt server.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 21
         */
        GATT = 1,
        /**
         * Indicates the profile type of the gatt client.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 21
         */
        GATT_CLIENT = 2,
        /**
         * Indicates the profile type of the gatt server.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 21
         */
        GATT_SERVER = 3
    }
    /**
     * GATT connection parameters.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 22
     */
    enum ConnectionParam {
        /**
         * low power mode.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 22
         */
        LOW_POWER = 1,
        /**
         * balanced power mode.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 22
         */
        BALANCED = 2,
        /**
         * Use the highest connection parameters.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 22
         */
        HIGH = 3
    }
    /**
     * The enum of gatt disconnection reasons.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 20
     */
    enum GattDisconnectReason {
        /**
         * Disconnection due to timeout.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 20
         */
        CONN_TIMEOUT = 1,
        /**
         * The connection is disconnected due to the peer.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 20
         */
        CONN_TERMINATE_PEER_USER = 2,
        /**
         * The connection is disconnected due to the local host.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 20
         */
        CONN_TERMINATE_LOCAL_HOST = 3,
        /**
         * Disconnection due to unknown reason.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 20
         */
        CONN_UNKNOWN = 4
    }
    /**
     * Phy type for advertising or connection.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @stagemodelonly
     * @since 23
     */
    enum BlePhy {
        /**
         * Use 1M phy for advertising or connection.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @stagemodelonly
         * @since 23
         */
        BLE_PHY_1M = 1,
        /**
         * Use 2M phy for advertising or connection.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @stagemodelonly
         * @since 23
         */
        BLE_PHY_2M = 2,
        /**
         * Use coded phy for advertising or connection.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @stagemodelonly
         * @since 23
         */
        BLE_PHY_CODED = 3
    }
    /**
     * Coded phy mode for advertising or connection.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @stagemodelonly
     * @since 23
     */
    enum CodedPhyMode {
        /**
         * Use coded S2 phy for advertising or connection.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @stagemodelonly
         * @since 23
         */
        BLE_PHY_CODED_S2 = 1,
        /**
         * Use coded S8 phy for advertising or connection.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @stagemodelonly
         * @since 23
         */
        BLE_PHY_CODED_S8 = 2
    }
    /**
     * Describes the permission of a att attribute item.
     *
     * @typedef GattPermissions
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 20
     */
    interface GattPermissions {
        /**
         * The attribute field has the read permission.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 20
         */
        read?: boolean;
        /**
         * The attribute field has the encrypted read permission.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 20
         */
        readEncrypted?: boolean;
        /**
         * The attribute field has the read permission for encryption authentication.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 20
         */
        readEncryptedMitm?: boolean;
        /**
         * The attribute field has the write permission.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 20
         */
        write?: boolean;
        /**
         * The attribute field has the encrypted write permission.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 20
         */
        writeEncrypted?: boolean;
        /**
         * The attribute field has the write permission for encryption authentication.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 20
         */
        writeEncryptedMitm?: boolean;
        /**
         * The attribute field has the signed write permission.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 20
         */
        writeSigned?: boolean;
        /**
         * The attribute field has the write permission for signature authentication.
         *
         * @type { ?boolean}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 20
         */
        writeSignedMitm?: boolean;
    }
    /**
     * Describe the context of GATT responses.
     *
     * @typedef GattRspContext
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 23
     */
    interface GattRspContext {
        /**
         * Timestamp of when Bluetooth received the response command.
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 23
         */
        timestamp: number;
    }
    /**
     * Describes the parameters of the Ble phy.
     *
     * @typedef PhyValue
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @stagemodelonly
     * @since 23
     */
    interface PhyValue {
        /**
         * Transmitter phy.
         *
         * @type { BlePhy}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @stagemodelonly
         * @since 23
         */
        txPhy: BlePhy;
        /**
         * Receiver phy.
         *
         * @type { BlePhy}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @stagemodelonly
         * @since 23
         */
        rxPhy: BlePhy;
        /**
         * Preferred coded phy mode.
         *
         * @type { ?CodedPhyMode}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @stagemodelonly
         * @since 23
         */
        phyMode?: CodedPhyMode;
    }
}
export default ble;
