     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_CONTROLDEVICEACTION_STRESS
     * @tc.name cn_controlDeviceAction_stress
     * @tc.desc Verify Bluetooth apitest case cn controlDeviceAction stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_controlDeviceAction_stress', 0, () => {
      runStress('cn_controlDeviceAction_stress', 20, () => {
        try {
          connection.controlDeviceAction(ctrlDeviceActionParams(BADDR));
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_updateCloudBluetoothDevice', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_UPDATECLOUDBLUETOOTHDEVICE_N
     * @tc.name cn_updateCloudBluetoothDevice_n
     * @tc.desc Verify Bluetooth API cn updateCloudBluetoothDevice normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_updateCloudBluetoothDevice_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        connection.updateCloudBluetoothDevice(trustedPairedDevices());
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_UPDATECLOUDBLUETOOTHDEVICE_E
     * @tc.name cn_updateCloudBluetoothDevice_e
     * @tc.desc Verify Bluetooth API cn updateCloudBluetoothDevice error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_updateCloudBluetoothDevice_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        connection.updateCloudBluetoothDevice(trustedPairedDevices());
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_UPDATECLOUDBLUETOOTHDEVICE_B
     * @tc.name cn_updateCloudBluetoothDevice_b
     * @tc.desc Verify Bluetooth API cn updateCloudBluetoothDevice boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_updateCloudBluetoothDevice_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        connection.updateCloudBluetoothDevice(trustedPairedDevices());
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_UPDATECLOUDBLUETOOTHDEVICE_R
     * @tc.name cn_updateCloudBluetoothDevice_r
     * @tc.desc Verify Bluetooth API cn updateCloudBluetoothDevice return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_updateCloudBluetoothDevice_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        connection.updateCloudBluetoothDevice(trustedPairedDevices());
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_UPDATECLOUDBLUETOOTHDEVICE_X
     * @tc.name cn_updateCloudBluetoothDevice_x
     * @tc.desc Verify Bluetooth API cn updateCloudBluetoothDevice repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_updateCloudBluetoothDevice_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        connection.updateCloudBluetoothDevice(trustedPairedDevices());
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertPeerExpect(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        connection.updateCloudBluetoothDevice(trustedPairedDevices());
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_UPDATECLOUDBLUETOOTHDEVICE_C
     * @tc.name cn_updateCloudBluetoothDevice_c
     * @tc.desc Verify Bluetooth API cn updateCloudBluetoothDevice condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_updateCloudBluetoothDevice_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        connection.updateCloudBluetoothDevice(trustedPairedDevices());
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_UPDATECLOUDBLUETOOTHDEVICE_PERF
     * @tc.name cn_updateCloudBluetoothDevice_perf
     * @tc.desc Verify Bluetooth apitest case cn updateCloudBluetoothDevice perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_updateCloudBluetoothDevice_perf', 0, () => {
      runPerf('cn_updateCloudBluetoothDevice_perf', 5000, () => {
        try {
          connection.updateCloudBluetoothDevice(trustedPairedDevices());
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_UPDATECLOUDBLUETOOTHDEVICE_STRESS
     * @tc.name cn_updateCloudBluetoothDevice_stress
     * @tc.desc Verify Bluetooth apitest case cn updateCloudBluetoothDevice stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_updateCloudBluetoothDevice_stress', 0, () => {
      runStress('cn_updateCloudBluetoothDevice_stress', 20, () => {
        try {
          connection.updateCloudBluetoothDevice(trustedPairedDevices());
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_generateLocalOobData', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GENERATELOCALOOBDATA_N
     * @tc.name cn_generateLocalOobData_n
     * @tc.desc Verify Bluetooth API cn generateLocalOobData normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_generateLocalOobData_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = connection.generateLocalOobData(connection.BluetoothTransport.TRANSPORT_BR_EDR) as Object;
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(value !== null && value !== undefined).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GENERATELOCALOOBDATA_E
     * @tc.name cn_generateLocalOobData_e
     * @tc.desc Verify Bluetooth API cn generateLocalOobData error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_generateLocalOobData_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        connection.generateLocalOobData(connection.BluetoothTransport.TRANSPORT_LE);
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GENERATELOCALOOBDATA_B
     * @tc.name cn_generateLocalOobData_b
     * @tc.desc Verify Bluetooth API cn generateLocalOobData boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_generateLocalOobData_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        connection.generateLocalOobData(connection.BluetoothTransport.TRANSPORT_BR_EDR);
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GENERATELOCALOOBDATA_R
     * @tc.name cn_generateLocalOobData_r
     * @tc.desc Verify Bluetooth API cn generateLocalOobData return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_generateLocalOobData_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = connection.generateLocalOobData(connection.BluetoothTransport.TRANSPORT_BR_EDR) as Object;
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(value !== null && value !== undefined).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GENERATELOCALOOBDATA_X
     * @tc.name cn_generateLocalOobData_x
     * @tc.desc Verify Bluetooth API cn generateLocalOobData repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_generateLocalOobData_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = connection.generateLocalOobData(connection.BluetoothTransport.TRANSPORT_BR_EDR) as Object;
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(value !== null && value !== undefined).assertTrue();
      }
      errCode = 0;
      value = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        value = connection.generateLocalOobData(connection.BluetoothTransport.TRANSPORT_BR_EDR) as Object;
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(value !== null && value !== undefined).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GENERATELOCALOOBDATA_C
     * @tc.name cn_generateLocalOobData_c
     * @tc.desc Verify Bluetooth API cn generateLocalOobData condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_generateLocalOobData_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = connection.generateLocalOobData(connection.BluetoothTransport.TRANSPORT_LE) as Object;
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(value !== null && value !== undefined).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GENERATELOCALOOBDATA_PERF
     * @tc.name cn_generateLocalOobData_perf
     * @tc.desc Verify Bluetooth apitest case cn generateLocalOobData perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_generateLocalOobData_perf', 0, () => {
      runPerf('cn_generateLocalOobData_perf', 5000, () => {
        try {
          connection.generateLocalOobData(connection.BluetoothTransport.TRANSPORT_BR_EDR) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GENERATELOCALOOBDATA_STRESS
     * @tc.name cn_generateLocalOobData_stress
     * @tc.desc Verify Bluetooth apitest case cn generateLocalOobData stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_generateLocalOobData_stress', 0, () => {
      runStress('cn_generateLocalOobData_stress', 20, () => {
        try {
          connection.generateLocalOobData(connection.BluetoothTransport.TRANSPORT_BR_EDR) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_getVirtualAddressByHash', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETVIRTUALADDRESSBYHASH_N
     * @tc.name cn_getVirtualAddressByHash_n
     * @tc.desc Verify Bluetooth API cn getVirtualAddressByHash normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getVirtualAddressByHash_n', 0, () => {
      let errCode = 0;
      let value = '';
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = connection.getVirtualAddressByHash(connection.HashAlgorithmType.HASH_ALGORITHM_SHA256, 'aa');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(typeof value).assertEqual('string');
        expect(value.length >= 0).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETVIRTUALADDRESSBYHASH_E
     * @tc.name cn_getVirtualAddressByHash_e
     * @tc.desc Verify Bluetooth API cn getVirtualAddressByHash error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getVirtualAddressByHash_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        connection.getVirtualAddressByHash(connection.HashAlgorithmType.HASH_ALGORITHM_SHA256, '');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETVIRTUALADDRESSBYHASH_B
     * @tc.name cn_getVirtualAddressByHash_b
     * @tc.desc Verify Bluetooth API cn getVirtualAddressByHash boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getVirtualAddressByHash_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        connection.getVirtualAddressByHash(connection.HashAlgorithmType.HASH_ALGORITHM_SHA256, 'ff');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETVIRTUALADDRESSBYHASH_R
     * @tc.name cn_getVirtualAddressByHash_r
     * @tc.desc Verify Bluetooth API cn getVirtualAddressByHash return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getVirtualAddressByHash_r', 0, () => {
      let errCode = 0;
      let value = '';
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = connection.getVirtualAddressByHash(connection.HashAlgorithmType.HASH_ALGORITHM_SHA256, 'aa');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(typeof value).assertEqual('string');
        expect(value.length >= 0).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETVIRTUALADDRESSBYHASH_X
     * @tc.name cn_getVirtualAddressByHash_x
     * @tc.desc Verify Bluetooth API cn getVirtualAddressByHash repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getVirtualAddressByHash_x', 0, () => {
      let errCode = 0;
      let value = '';
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = connection.getVirtualAddressByHash(connection.HashAlgorithmType.HASH_ALGORITHM_SHA256, 'aa');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(typeof value).assertEqual('string');
        expect(value.length >= 0).assertTrue();
      }
      errCode = 0;
      value = '';
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        value = connection.getVirtualAddressByHash(connection.HashAlgorithmType.HASH_ALGORITHM_SHA256, 'aa');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(typeof value).assertEqual('string');
        expect(value.length >= 0).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETVIRTUALADDRESSBYHASH_C
     * @tc.name cn_getVirtualAddressByHash_c
     * @tc.desc Verify Bluetooth API cn getVirtualAddressByHash condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getVirtualAddressByHash_c', 0, () => {
      let errCode = 0;
      let value = '';
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = connection.getVirtualAddressByHash(connection.HashAlgorithmType.HASH_ALGORITHM_SHA256, '11');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(typeof value).assertEqual('string');
        expect(value.length >= 0).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETVIRTUALADDRESSBYHASH_PERF
     * @tc.name cn_getVirtualAddressByHash_perf
     * @tc.desc Verify Bluetooth apitest case cn getVirtualAddressByHash perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getVirtualAddressByHash_perf', 0, () => {
      runPerf('cn_getVirtualAddressByHash_perf', 5000, () => {
        try {
          connection.getVirtualAddressByHash(connection.HashAlgorithmType.HASH_ALGORITHM_SHA256, 'aa');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETVIRTUALADDRESSBYHASH_STRESS
     * @tc.name cn_getVirtualAddressByHash_stress
     * @tc.desc Verify Bluetooth apitest case cn getVirtualAddressByHash stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getVirtualAddressByHash_stress', 0, () => {
      runStress('cn_getVirtualAddressByHash_stress', 20, () => {
        try {
          connection.getVirtualAddressByHash(connection.HashAlgorithmType.HASH_ALGORITHM_SHA256, 'aa');
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_on_bluetoothDeviceFind', () => {
    it('cn_on_bluetoothDeviceFind_n', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        connection.on('bluetoothDeviceFind', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_bluetoothDeviceFind_e', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        connection.on('bluetoothDeviceFind', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_bluetoothDeviceFind_b', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        connection.on('bluetoothDeviceFind', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_bluetoothDeviceFind_r', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        connection.on('bluetoothDeviceFind', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_bluetoothDeviceFind_x', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        connection.on('bluetoothDeviceFind', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertPeerExpect(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        connection.on('bluetoothDeviceFind', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_bluetoothDeviceFind_c', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        connection.on('bluetoothDeviceFind', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ON_BLUETOOTHDEVICEFIND_PERF
     * @tc.name cn_on_bluetoothDeviceFind_perf
     * @tc.desc Verify Bluetooth apitest case cn on bluetoothDeviceFind perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_on_bluetoothDeviceFind_perf', 0, () => {
      runPerf('cn_on_bluetoothDeviceFind_perf', 5000, () => {
        try {
          connection.on('bluetoothDeviceFind', () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ON_BLUETOOTHDEVICEFIND_STRESS
     * @tc.name cn_on_bluetoothDeviceFind_stress
     * @tc.desc Verify Bluetooth apitest case cn on bluetoothDeviceFind stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_on_bluetoothDeviceFind_stress', 0, () => {
      runStress('cn_on_bluetoothDeviceFind_stress', 20, () => {
        try {
          connection.on('bluetoothDeviceFind', () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_off_bluetoothDeviceFind', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BLUETOOTHDEVICEFIND_N
     * @tc.name cn_off_bluetoothDeviceFind_n
     * @tc.desc Verify Bluetooth API cn off bluetoothDeviceFind normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bluetoothDeviceFind_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        connection.off('bluetoothDeviceFind');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BLUETOOTHDEVICEFIND_E
     * @tc.name cn_off_bluetoothDeviceFind_e
     * @tc.desc Verify Bluetooth API cn off bluetoothDeviceFind error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bluetoothDeviceFind_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        connection.off('bluetoothDeviceFind');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BLUETOOTHDEVICEFIND_B
     * @tc.name cn_off_bluetoothDeviceFind_b
     * @tc.desc Verify Bluetooth API cn off bluetoothDeviceFind boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bluetoothDeviceFind_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        connection.off('bluetoothDeviceFind');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BLUETOOTHDEVICEFIND_R
     * @tc.name cn_off_bluetoothDeviceFind_r
     * @tc.desc Verify Bluetooth API cn off bluetoothDeviceFind return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bluetoothDeviceFind_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        connection.off('bluetoothDeviceFind');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BLUETOOTHDEVICEFIND_X
     * @tc.name cn_off_bluetoothDeviceFind_x
     * @tc.desc Verify Bluetooth API cn off bluetoothDeviceFind repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bluetoothDeviceFind_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        connection.off('bluetoothDeviceFind');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertPeerExpect(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        connection.off('bluetoothDeviceFind');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BLUETOOTHDEVICEFIND_C
     * @tc.name cn_off_bluetoothDeviceFind_c
     * @tc.desc Verify Bluetooth API cn off bluetoothDeviceFind condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bluetoothDeviceFind_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        connection.off('bluetoothDeviceFind');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BLUETOOTHDEVICEFIND_PERF
     * @tc.name cn_off_bluetoothDeviceFind_perf
     * @tc.desc Verify Bluetooth apitest case cn off bluetoothDeviceFind perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bluetoothDeviceFind_perf', 0, () => {
      runPerf('cn_off_bluetoothDeviceFind_perf', 5000, () => {
        try {
          connection.off('bluetoothDeviceFind');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BLUETOOTHDEVICEFIND_STRESS
     * @tc.name cn_off_bluetoothDeviceFind_stress
     * @tc.desc Verify Bluetooth apitest case cn off bluetoothDeviceFind stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bluetoothDeviceFind_stress', 0, () => {
      runStress('cn_off_bluetoothDeviceFind_stress', 20, () => {
        try {
          connection.off('bluetoothDeviceFind');
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_on_bondStateChange', () => {
    it('cn_on_bondStateChange_n', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        connection.on('bondStateChange', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_bondStateChange_e', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        connection.on('bondStateChange', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_bondStateChange_b', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        connection.on('bondStateChange', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_bondStateChange_r', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        connection.on('bondStateChange', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_bondStateChange_x', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        connection.on('bondStateChange', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertPeerExpect(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        connection.on('bondStateChange', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_bondStateChange_c', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        connection.on('bondStateChange', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ON_BONDSTATECHANGE_PERF
     * @tc.name cn_on_bondStateChange_perf
     * @tc.desc Verify Bluetooth apitest case cn on bondStateChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_on_bondStateChange_perf', 0, () => {
      runPerf('cn_on_bondStateChange_perf', 5000, () => {
        try {
          connection.on('bondStateChange', () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ON_BONDSTATECHANGE_STRESS
     * @tc.name cn_on_bondStateChange_stress
     * @tc.desc Verify Bluetooth apitest case cn on bondStateChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_on_bondStateChange_stress', 0, () => {
      runStress('cn_on_bondStateChange_stress', 20, () => {
        try {
          connection.on('bondStateChange', () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_off_bondStateChange', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BONDSTATECHANGE_N
     * @tc.name cn_off_bondStateChange_n
     * @tc.desc Verify Bluetooth API cn off bondStateChange normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bondStateChange_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        connection.off('bondStateChange');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BONDSTATECHANGE_E
     * @tc.name cn_off_bondStateChange_e
     * @tc.desc Verify Bluetooth API cn off bondStateChange error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bondStateChange_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        connection.off('bondStateChange');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BONDSTATECHANGE_B
     * @tc.name cn_off_bondStateChange_b
     * @tc.desc Verify Bluetooth API cn off bondStateChange boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bondStateChange_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        connection.off('bondStateChange');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BONDSTATECHANGE_R
     * @tc.name cn_off_bondStateChange_r
     * @tc.desc Verify Bluetooth API cn off bondStateChange return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bondStateChange_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        connection.off('bondStateChange');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BONDSTATECHANGE_X
     * @tc.name cn_off_bondStateChange_x
     * @tc.desc Verify Bluetooth API cn off bondStateChange repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bondStateChange_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        connection.off('bondStateChange');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertPeerExpect(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        connection.off('bondStateChange');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BONDSTATECHANGE_C
     * @tc.name cn_off_bondStateChange_c
     * @tc.desc Verify Bluetooth API cn off bondStateChange condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bondStateChange_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        connection.off('bondStateChange');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BONDSTATECHANGE_PERF
     * @tc.name cn_off_bondStateChange_perf
     * @tc.desc Verify Bluetooth apitest case cn off bondStateChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bondStateChange_perf', 0, () => {
      runPerf('cn_off_bondStateChange_perf', 5000, () => {
        try {
          connection.off('bondStateChange');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BONDSTATECHANGE_STRESS
     * @tc.name cn_off_bondStateChange_stress
     * @tc.desc Verify Bluetooth apitest case cn off bondStateChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_bondStateChange_stress', 0, () => {
      runStress('cn_off_bondStateChange_stress', 20, () => {
        try {
          connection.off('bondStateChange');
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_on_pinRequired', () => {
    it('cn_on_pinRequired_n', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        connection.on('pinRequired', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_pinRequired_e', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        connection.on('pinRequired', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_pinRequired_b', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        connection.on('pinRequired', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_pinRequired_r', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        connection.on('pinRequired', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_pinRequired_x', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        connection.on('pinRequired', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertPeerExpect(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        connection.on('pinRequired', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_pinRequired_c', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        connection.on('pinRequired', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ON_PINREQUIRED_PERF
     * @tc.name cn_on_pinRequired_perf
     * @tc.desc Verify Bluetooth apitest case cn on pinRequired perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_on_pinRequired_perf', 0, () => {
      runPerf('cn_on_pinRequired_perf', 5000, () => {
        try {
          connection.on('pinRequired', () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ON_PINREQUIRED_STRESS
     * @tc.name cn_on_pinRequired_stress
     * @tc.desc Verify Bluetooth apitest case cn on pinRequired stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_on_pinRequired_stress', 0, () => {
      runStress('cn_on_pinRequired_stress', 20, () => {
        try {
          connection.on('pinRequired', () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_off_pinRequired', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_PINREQUIRED_N
     * @tc.name cn_off_pinRequired_n
     * @tc.desc Verify Bluetooth API cn off pinRequired normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_pinRequired_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        connection.off('pinRequired');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_PINREQUIRED_E
     * @tc.name cn_off_pinRequired_e
     * @tc.desc Verify Bluetooth API cn off pinRequired error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_pinRequired_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        connection.off('pinRequired');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_PINREQUIRED_B
     * @tc.name cn_off_pinRequired_b
     * @tc.desc Verify Bluetooth API cn off pinRequired boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_pinRequired_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        connection.off('pinRequired');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_PINREQUIRED_R
     * @tc.name cn_off_pinRequired_r
     * @tc.desc Verify Bluetooth API cn off pinRequired return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_pinRequired_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        connection.off('pinRequired');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_PINREQUIRED_X
     * @tc.name cn_off_pinRequired_x
     * @tc.desc Verify Bluetooth API cn off pinRequired repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_pinRequired_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        connection.off('pinRequired');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertPeerExpect(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        connection.off('pinRequired');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_PINREQUIRED_C
     * @tc.name cn_off_pinRequired_c
     * @tc.desc Verify Bluetooth API cn off pinRequired condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_pinRequired_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        connection.off('pinRequired');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_PINREQUIRED_PERF
     * @tc.name cn_off_pinRequired_perf
     * @tc.desc Verify Bluetooth apitest case cn off pinRequired perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_pinRequired_perf', 0, () => {
      runPerf('cn_off_pinRequired_perf', 5000, () => {
        try {
          connection.off('pinRequired');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_PINREQUIRED_STRESS
     * @tc.name cn_off_pinRequired_stress
     * @tc.desc Verify Bluetooth apitest case cn off pinRequired stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_pinRequired_stress', 0, () => {
      runStress('cn_off_pinRequired_stress', 20, () => {
        try {
          connection.off('pinRequired');
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_onScanModeChange', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ONSCANMODECHANGE_N
     * @tc.name cn_onScanModeChange_n
     * @tc.desc Verify Bluetooth API cn onScanModeChange normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_onScanModeChange_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        connection.onScanModeChange(() => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ONSCANMODECHANGE_E
     * @tc.name cn_onScanModeChange_e
     * @tc.desc Verify Bluetooth API cn onScanModeChange error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_onScanModeChange_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        connection.onScanModeChange(() => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ONSCANMODECHANGE_B
     * @tc.name cn_onScanModeChange_b
     * @tc.desc Verify Bluetooth API cn onScanModeChange boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_onScanModeChange_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        connection.onScanModeChange(() => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ONSCANMODECHANGE_R
     * @tc.name cn_onScanModeChange_r
     * @tc.desc Verify Bluetooth API cn onScanModeChange return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_onScanModeChange_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        connection.onScanModeChange(() => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ONSCANMODECHANGE_X
     * @tc.name cn_onScanModeChange_x
     * @tc.desc Verify Bluetooth API cn onScanModeChange repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_onScanModeChange_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        connection.onScanModeChange(() => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertPeerExpect(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        connection.onScanModeChange(() => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ONSCANMODECHANGE_C
     * @tc.name cn_onScanModeChange_c
     * @tc.desc Verify Bluetooth API cn onScanModeChange condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_onScanModeChange_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        connection.onScanModeChange(() => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ONSCANMODECHANGE_PERF
     * @tc.name cn_onScanModeChange_perf
     * @tc.desc Verify Bluetooth apitest case cn onScanModeChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_onScanModeChange_perf', 0, () => {
      runPerf('cn_onScanModeChange_perf', 5000, () => {
        try {
          connection.onScanModeChange(() => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ONSCANMODECHANGE_STRESS
     * @tc.name cn_onScanModeChange_stress
     * @tc.desc Verify Bluetooth apitest case cn onScanModeChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_onScanModeChange_stress', 0, () => {
      runStress('cn_onScanModeChange_stress', 20, () => {
        try {
          connection.onScanModeChange(() => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_offScanModeChange', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFFSCANMODECHANGE_N
     * @tc.name cn_offScanModeChange_n
     * @tc.desc Verify Bluetooth API cn offScanModeChange normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_offScanModeChange_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        connection.offScanModeChange();
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFFSCANMODECHANGE_E
     * @tc.name cn_offScanModeChange_e
     * @tc.desc Verify Bluetooth API cn offScanModeChange error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_offScanModeChange_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        connection.offScanModeChange();
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFFSCANMODECHANGE_B
     * @tc.name cn_offScanModeChange_b
     * @tc.desc Verify Bluetooth API cn offScanModeChange boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_offScanModeChange_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        connection.offScanModeChange();
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFFSCANMODECHANGE_R
     * @tc.name cn_offScanModeChange_r
     * @tc.desc Verify Bluetooth API cn offScanModeChange return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_offScanModeChange_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        connection.offScanModeChange();
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFFSCANMODECHANGE_X
     * @tc.name cn_offScanModeChange_x
     * @tc.desc Verify Bluetooth API cn offScanModeChange repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_offScanModeChange_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        connection.offScanModeChange();
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertPeerExpect(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        connection.offScanModeChange();
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFFSCANMODECHANGE_C
     * @tc.name cn_offScanModeChange_c
     * @tc.desc Verify Bluetooth API cn offScanModeChange condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_offScanModeChange_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        connection.offScanModeChange();
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFFSCANMODECHANGE_PERF
     * @tc.name cn_offScanModeChange_perf
     * @tc.desc Verify Bluetooth apitest case cn offScanModeChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_offScanModeChange_perf', 0, () => {
      runPerf('cn_offScanModeChange_perf', 5000, () => {
        try {
          connection.offScanModeChange();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFFSCANMODECHANGE_STRESS
     * @tc.name cn_offScanModeChange_stress
     * @tc.desc Verify Bluetooth apitest case cn offScanModeChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_offScanModeChange_stress', 0, () => {
      runStress('cn_offScanModeChange_stress', 20, () => {
        try {
          connection.offScanModeChange();
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_on_batteryChange', () => {
    it('cn_on_batteryChange_n', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        connection.on('batteryChange', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_batteryChange_e', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        connection.on('batteryChange', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_batteryChange_b', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        connection.on('batteryChange', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_batteryChange_r', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        connection.on('batteryChange', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_batteryChange_x', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        connection.on('batteryChange', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertPeerExpect(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        connection.on('batteryChange', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_batteryChange_c', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        connection.on('batteryChange', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ON_BATTERYCHANGE_PERF
     * @tc.name cn_on_batteryChange_perf
     * @tc.desc Verify Bluetooth apitest case cn on batteryChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_on_batteryChange_perf', 0, () => {
      runPerf('cn_on_batteryChange_perf', 5000, () => {
        try {
          connection.on('batteryChange', () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ON_BATTERYCHANGE_STRESS
     * @tc.name cn_on_batteryChange_stress
     * @tc.desc Verify Bluetooth apitest case cn on batteryChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_on_batteryChange_stress', 0, () => {
      runStress('cn_on_batteryChange_stress', 20, () => {
        try {
          connection.on('batteryChange', () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_off_batteryChange', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BATTERYCHANGE_N
     * @tc.name cn_off_batteryChange_n
     * @tc.desc Verify Bluetooth API cn off batteryChange normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_batteryChange_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        connection.off('batteryChange');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BATTERYCHANGE_E
     * @tc.name cn_off_batteryChange_e
     * @tc.desc Verify Bluetooth API cn off batteryChange error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_batteryChange_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        connection.off('batteryChange');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BATTERYCHANGE_B
     * @tc.name cn_off_batteryChange_b
     * @tc.desc Verify Bluetooth API cn off batteryChange boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_batteryChange_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        connection.off('batteryChange');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BATTERYCHANGE_R
     * @tc.name cn_off_batteryChange_r
     * @tc.desc Verify Bluetooth API cn off batteryChange return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_batteryChange_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        connection.off('batteryChange');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BATTERYCHANGE_X
     * @tc.name cn_off_batteryChange_x
     * @tc.desc Verify Bluetooth API cn off batteryChange repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_batteryChange_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        connection.off('batteryChange');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertPeerExpect(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        connection.off('batteryChange');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BATTERYCHANGE_C
     * @tc.name cn_off_batteryChange_c
     * @tc.desc Verify Bluetooth API cn off batteryChange condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_batteryChange_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        connection.off('batteryChange');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BATTERYCHANGE_PERF
     * @tc.name cn_off_batteryChange_perf
     * @tc.desc Verify Bluetooth apitest case cn off batteryChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_batteryChange_perf', 0, () => {
      runPerf('cn_off_batteryChange_perf', 5000, () => {
        try {
          connection.off('batteryChange');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_BATTERYCHANGE_STRESS
     * @tc.name cn_off_batteryChange_stress
     * @tc.desc Verify Bluetooth apitest case cn off batteryChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_batteryChange_stress', 0, () => {
      runStress('cn_off_batteryChange_stress', 20, () => {
        try {
          connection.off('batteryChange');
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_on_discoveryResult', () => {
    it('cn_on_discoveryResult_n', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        connection.on('discoveryResult', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_discoveryResult_e', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        connection.on('discoveryResult', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_discoveryResult_b', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        connection.on('discoveryResult', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_discoveryResult_r', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        connection.on('discoveryResult', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_discoveryResult_x', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        connection.on('discoveryResult', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertPeerExpect(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        connection.on('discoveryResult', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });
    it('cn_on_discoveryResult_c', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        connection.on('discoveryResult', () => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
      waitCallback(done, 200);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ON_DISCOVERYRESULT_PERF
     * @tc.name cn_on_discoveryResult_perf
     * @tc.desc Verify Bluetooth apitest case cn on discoveryResult perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_on_discoveryResult_perf', 0, () => {
      runPerf('cn_on_discoveryResult_perf', 5000, () => {
        try {
          connection.on('discoveryResult', () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_ON_DISCOVERYRESULT_STRESS
     * @tc.name cn_on_discoveryResult_stress
     * @tc.desc Verify Bluetooth apitest case cn on discoveryResult stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_on_discoveryResult_stress', 0, () => {
      runStress('cn_on_discoveryResult_stress', 20, () => {
        try {
          connection.on('discoveryResult', () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_off_discoveryResult', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_DISCOVERYRESULT_N
     * @tc.name cn_off_discoveryResult_n
     * @tc.desc Verify Bluetooth API cn off discoveryResult normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_discoveryResult_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        connection.off('discoveryResult');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_DISCOVERYRESULT_E
     * @tc.name cn_off_discoveryResult_e
     * @tc.desc Verify Bluetooth API cn off discoveryResult error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_discoveryResult_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        connection.off('discoveryResult');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_DISCOVERYRESULT_B
     * @tc.name cn_off_discoveryResult_b
     * @tc.desc Verify Bluetooth API cn off discoveryResult boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_discoveryResult_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        connection.off('discoveryResult');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_DISCOVERYRESULT_R
     * @tc.name cn_off_discoveryResult_r
     * @tc.desc Verify Bluetooth API cn off discoveryResult return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_discoveryResult_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        connection.off('discoveryResult');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_DISCOVERYRESULT_X
     * @tc.name cn_off_discoveryResult_x
     * @tc.desc Verify Bluetooth API cn off discoveryResult repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_discoveryResult_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        connection.off('discoveryResult');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertPeerExpect(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        connection.off('discoveryResult');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_DISCOVERYRESULT_C
     * @tc.name cn_off_discoveryResult_c
     * @tc.desc Verify Bluetooth API cn off discoveryResult condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_discoveryResult_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        connection.off('discoveryResult');
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_DISCOVERYRESULT_PERF
     * @tc.name cn_off_discoveryResult_perf
     * @tc.desc Verify Bluetooth apitest case cn off discoveryResult perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_discoveryResult_perf', 0, () => {
      runPerf('cn_off_discoveryResult_perf', 5000, () => {
        try {
          connection.off('discoveryResult');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_OFF_DISCOVERYRESULT_STRESS
     * @tc.name cn_off_discoveryResult_stress
     * @tc.desc Verify Bluetooth apitest case cn off discoveryResult stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_off_discoveryResult_stress', 0, () => {
      runStress('cn_off_discoveryResult_stress', 20, () => {
        try {
          connection.off('discoveryResult');
        } catch (_e) {
        }
      });
    });
    });
  });
}


