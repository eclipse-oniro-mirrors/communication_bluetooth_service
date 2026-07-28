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
