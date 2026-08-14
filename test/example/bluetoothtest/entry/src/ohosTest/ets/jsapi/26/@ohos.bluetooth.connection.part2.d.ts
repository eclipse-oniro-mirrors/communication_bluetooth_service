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
/* Continuation part 2 of @ohos.bluetooth.connection.d.ts (reference only). */
    }
    /**
     * The enum of BR scan mode.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * The enum of BR scan mode.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    enum ScanMode {
        /**
         * Indicates the scan mode is none
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the scan mode is none
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 13
         */
        SCAN_MODE_NONE = 0,
        /**
         * Indicates the scan mode is connectable
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the scan mode is connectable
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 13
         */
        SCAN_MODE_CONNECTABLE = 1,
        /**
         * Indicates the scan mode is general discoverable
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        SCAN_MODE_GENERAL_DISCOVERABLE = 2,
        /**
         * Indicates the scan mode is limited discoverable
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        SCAN_MODE_LIMITED_DISCOVERABLE = 3,
        /**
         * Indicates the scan mode is connectable and general discoverable
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicates the scan mode is connectable and general discoverable
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 13
         */
        SCAN_MODE_CONNECTABLE_GENERAL_DISCOVERABLE = 4,
        /**
         * Indicates the scan mode is connectable and limited discoverable
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        SCAN_MODE_CONNECTABLE_LIMITED_DISCOVERABLE = 5
    }
    /**
     * The enum of bond state.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * The enum of bond state.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * The enum of bond state.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    enum BondState {
        /**
         * Indicate the bond state is invalid
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicate the bond state is invalid
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicate the bond state is invalid
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        BOND_STATE_INVALID = 0,
        /**
         * Indicate the bond state is bonding
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicate the bond state is bonding
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicate the bond state is bonding
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        BOND_STATE_BONDING = 1,
        /**
         * Indicate the bond state is bonded
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 10
         */
        /**
         * Indicate the bond state is bonded
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @atomicservice
         * @since 12
         */
        /**
         * Indicate the bond state is bonded
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @atomicservice
         * @since 13
         */
        BOND_STATE_BONDED = 2
    }
    /**
     * Enum for the type of pairing to a remote device
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 10
     */
    enum PinType {
        /**
         * The user needs to enter the pin code displayed on the peer device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 10
         */
        PIN_TYPE_ENTER_PIN_CODE = 0,
        /**
         * The user needs to enter the passkey displayed on the peer device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 10
         */
        PIN_TYPE_ENTER_PASSKEY = 1,
        /**
         * The user needs to confirm the passkey displayed on the local device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 10
         */
        PIN_TYPE_CONFIRM_PASSKEY = 2,
        /**
         * The user needs to accept or deny the pairing request.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 10
         */
        PIN_TYPE_NO_PASSKEY_CONSENT = 3,
        /**
         * The user needs to enter the passkey displayed on the local device on the peer device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 10
         */
        PIN_TYPE_NOTIFY_PASSKEY = 4,
        /**
         * The user needs to enter the pin code displayed on the peer device, used for bluetooth 2.0.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 10
         */
        PIN_TYPE_DISPLAY_PIN_CODE = 5,
        /**
         * The user needs to accept or deny the OOB pairing request.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 10
         */
        PIN_TYPE_OOB_CONSENT = 6,
        /**
         * The user needs to enter the 16-digit pin code displayed on the peer device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 10
         */
        PIN_TYPE_PIN_16_DIGITS = 7
    }
    /**
     * Describes the contents of the discovery results
     *
     * @typedef DiscoveryResult
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 12
     */
    /**
     * Describes the contents of the discovery results
     *
     * @typedef DiscoveryResult
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 18
     */
    interface DiscoveryResult {
        /**
         * Identify of the discovery device
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 12
         */
        /**
         * Identify of the discovery device
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 18
         */
        deviceId: string;
        /**
         * RSSI of the remote device
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 12
         */
        /**
         * RSSI of the remote device
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 18
         */
        rssi: number;
        /**
         * The local name of the device
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 12
         */
        /**
         * The local name of the device
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 18
         */
        deviceName: string;
        /**
         * The class of the device
         *
         * @type { DeviceClass}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 12
         */
        /**
         * The class of the device
         *
         * @type { DeviceClass}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 18
         */
        deviceClass: DeviceClass;
    }
    /**
     * Describes the contents of the battery information.
     *
     * @typedef BatteryInfo
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    interface BatteryInfo {
        /**
         * Identify of the discovery device.
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 12
         */
        deviceId: string;
        /**
         * Electricity value of the general device. {@code -1} means no power information.
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        batteryLevel: number;
        /**
         * Electricity value of the left ear. {@code -1} means no power information.
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        leftEarBatteryLevel: number;
        /**
         * The charge state of the left ear.
         *
         * @type { DeviceChargeState}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        leftEarChargeState: DeviceChargeState;
        /**
         * Electricity value of the right ear. {@code -1} means no power information.
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        rightEarBatteryLevel: number;
        /**
         * The charge state of the right ear.
         *
         * @type { DeviceChargeState}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        rightEarChargeState: DeviceChargeState;
        /**
         * Electricity value of the box. {@code -1} means no power information.
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        boxBatteryLevel: number;
        /**
         * The charge state of the box.
         *
         * @type { DeviceChargeState}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        boxChargeState: DeviceChargeState;
    }
    /**
     * Enum for the charge state.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    enum DeviceChargeState {
        /**
         * Not support super charge, and not charged.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        DEVICE_NORMAL_CHARGE_NOT_CHARGED = 0,
        /**
         * Not support super charge, and in charging.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        DEVICE_NORMAL_CHARGE_IN_CHARGING = 1,
        /**
         * Support super charge, and not charged.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        DEVICE_SUPER_CHARGE_NOT_CHARGED = 2,
        /**
         * Support super charge, and in charging.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        DEVICE_SUPER_CHARGE_IN_CHARGING = 3
    }
    /**
     * Enum for the custom type of remote device.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 12
     */
    enum DeviceType {
        /**
         * Default type, the type is consistent with COD.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 12
         */
        DEVICE_TYPE_DEFAULT = 0,
        /**
         * Car bluetooth.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 12
         */
        DEVICE_TYPE_CAR = 1,
        /**
         * Headset bluetooth.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 12
         */
        DEVICE_TYPE_HEADSET = 2,
        /**
         * Hearing Aid.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 12
         */
        DEVICE_TYPE_HEARING = 3,
        /**
         * Glasses device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 12
         */
        DEVICE_TYPE_GLASSES = 4,
        /**
         * Watch device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 12
         */
        DEVICE_TYPE_WATCH = 5,
        /**
         * Speaker device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 12
         */
        DEVICE_TYPE_SPEAKER = 6,
        /**
         * Others bluetooth.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 12
         */
        DEVICE_TYPE_OTHERS = 7
    }
    /**
     * Enum for cause of unbond.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    /**
     * Enum for cause of unbond.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    enum UnbondCause {
        /**
         * User proactively removed device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        /**
         * User proactively removed device.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @crossplatform
         * @since 13
         */
        USER_REMOVED = 0,
        /**
         * Remote device shut down.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        REMOTE_DEVICE_DOWN = 1,
        /**
         * Wrong PIN code.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        AUTH_FAILURE = 2,
        /**
         * Remote device rejected.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        AUTH_REJECTED = 3,
        /**
         * Internal error.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @since 12
         */
        INTERNAL_ERROR = 4
    }
    /**
     * Describes information about controlling the Bluetooth peripheral.
     *
     * @typedef ControlDeviceActionParams
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    interface ControlDeviceActionParams {
        /**
         * Indicates the address of the peripheral.
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        deviceId: string;
        /**
         * Indicates the control type.
         *
         * @type { ControlType}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        type: ControlType;
        /**
         * Indicates the control value.
         *
         * @type { ControlTypeValue}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        typeValue: ControlTypeValue;
        /**
         * Indicates the control object.
         *
         * @type { ControlObject}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        controlObject: ControlObject;
    }
    /**
     * Describes the control type.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    enum ControlType {
        /**
         * Indicates the control command of play.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        PLAY = 0,
        /**
         * Indicates the control command of vibration.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        VIBRATE = 1,
        /**
         * Indicates the control command of flash.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        FLASH = 2,
        /**
         * Indicates the control command of lock.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        LOCK = 3,
        /**
         * Indicates the control command of erase.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        ERASE = 4
    }
    /**
     * Describes the control type value.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    enum ControlTypeValue {
        /**
         * Indicates the action of disable.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        DISABLE = 0,
        /**
         * Indicates the action of enable.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        ENABLE = 1,
        /**
         * Indicates the action of query.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        QUERY = 2
    }
    /**
     * Describes the control object.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    enum ControlObject {
        /**
         * Control object of left ear.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        LEFT_EAR = 0,
        /**
         * Control object of right ear.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        RIGHT_EAR = 1,
        /**
         * Control object of left and right ear.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        LEFT_RIGHT_EAR = 2
    }
    /**
     * Describes the cloud pair device.
     *
     * @typedef TrustedPairedDevices
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    interface TrustedPairedDevices {
        /**
         * The list of cloud pair devices.
         *
         * @type { Array<TrustedPairedDevice>}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        trustedPairedDevices: Array<TrustedPairedDevice>;
    }
    /**
     * Describes device of cloud pair.
     *
     * @typedef TrustedPairedDevice
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    interface TrustedPairedDevice {
        /**
         * Indicates the device identify.
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        sn: string;
        /**
         * Indicates the device type of the peripheral.
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        deviceType: string;
        /**
         * Indicates the modelId of the peripheral.
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        modelId: string;
        /**
         * Indicates the manufactory of the peripheral.
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        manufactory: string;
        /**
         * Indicates the productId of the peripheral.
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        productId: string;
        /**
         * Indicates the HiLink version of the peripheral.
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        hiLinkVersion: string;
        /**
         * Indicates the macAddress of the peripheral.
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        macAddress: string;
        /**
         * Indicates the service type of the peripheral.
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        serviceType: string;
        /**
         * Indicates the service id of the peripheral.
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        serviceId: string;
        /**
         * The local name of the device
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        deviceName: string;
        /**
         * Indicates the uuid of the peripheral.
         *
         * @type { string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        uuids: string;
        /**
         * Indicates the bluetoothClass of the peripheral.
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        bluetoothClass: number;
        /**
         * Indicates the token of the peripheral.
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        token: ArrayBuffer;
        /**
         * Indicates the deviceNameTime of the peripheral.
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        deviceNameTime: number;
        /**
         * Indicates the securityAdvInfo of the peripheral.
         *
         * @type { ArrayBuffer}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        secureAdvertisingInfo: ArrayBuffer;
        /**
         * Indicates the pairState of the peripheral.
         *
         * @type { number}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @since 15
         */
        pairState: number;
    }
    /**
     * Out Of Band data used in Bluetooth device pairing.
     *
     * @typedef OobData
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @stagemodelonly
     * @since 23
     */
    interface OobData {
        /**
         * The address of remote Bluetooth device.
         *
         * @type { BluetoothAddress}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @stagemodelonly
         * @since 23
         */
        deviceId: BluetoothAddress;
        /**
         * Confirmation data in OOB pairing, with a size of 16 octets.
         *
         * @type { Uint8Array}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @stagemodelonly
         * @since 23
         */
        confirmationHash: Uint8Array;
        /**
         * Randomizer data in OOB pairing, with a size of 16 octets.
         *
         * @type { ?Uint8Array}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @stagemodelonly
         * @since 23
         */
        randomizerHash?: Uint8Array;
        /**
         * The name of the remote Bluetooth device.
         *
         * @type { ?string}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @stagemodelonly
         * @since 23
         */
        deviceName?: string;
        /**
         * The role of the remote Bluetooth device.
         *
         * @type { ?DeviceRole}
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @stagemodelonly
         * @since 23
         */
        deviceRole?: DeviceRole;
    }
    /**
     * Enum for the role of device.
     *
     * @enum { number}
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @stagemodelonly
     * @since 23
     */
    enum DeviceRole {
        /**
         * Only peripheral supported.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @stagemodelonly
         * @since 23
         */
        DEVICE_ROLE_PERIPHERAL_ONLY = 0,
        /**
         * Only central supported.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @stagemodelonly
         * @since 23
         */
        DEVICE_ROLE_CENTRAL_ONLY = 1,
        /**
         * Central & peripheral supported, peripheral preferred.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @stagemodelonly
         * @since 23
         */
        DEVICE_ROLE_BOTH_PREFER_PERIPHERAL = 2,
        /**
         * Central & peripheral supported, central preferred.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @stagemodelonly
         * @since 23
         */
        DEVICE_ROLE_BOTH_PREFER_CENTRAL = 3
    }
    /**
     * Enum for the action of car key.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @stagemodelonly
     * @since 26.0.0
     */
    enum CarKeyActionType {
        /**
         * Add the data of car key.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @stagemodelonly
         * @since 26.0.0
         */
        CAR_KEY_ACTION_ADD = 0,
        /**
         * Delete the data of car key.
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @systemapi
         * @stagemodelonly
         * @since 26.0.0
         */
        CAR_KEY_ACTION_DELETE = 1
    }
    /**
     * Enum for the hash algorithm type.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @stagemodelonly
     * @since 24
     */
    enum HashAlgorithmType {
        /**
         * SHA256 hash algorithm
         *
         * @syscap SystemCapability.Communication.Bluetooth.Core
         * @stagemodelonly
         * @since 24
         */
        HASH_ALGORITHM_SHA256 = 0
    }
}
export default connection;
