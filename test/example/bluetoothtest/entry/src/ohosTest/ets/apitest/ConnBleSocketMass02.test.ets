      hilog.info(0x0000, 'ConnBleSocket', 'cn_42_update_cloud_p_e2: invoke');
      try {
        const c4:connection.TrustedPairedDevices={trustedPairedDevices:[]};
        connection.updateCloudBluetoothDevice(c4);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_42_update_cloud_p_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_42_update_cloud_p_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_42_UPDATE_CLOUD_P_R
     * @tc.name cn_42_update_cloud_p_r
     * @tc.desc Verify Bluetooth API cn 42 update cloud p return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_42_update_cloud_p_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_42_update_cloud_p_r: invoke');
      try {
        const c5:connection.TrustedPairedDevices={trustedPairedDevices:[]};
        connection.updateCloudBluetoothDevice(c5);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_42_update_cloud_p_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_42_update_cloud_p_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_42_UPDATE_CLOUD_P_X
     * @tc.name cn_42_update_cloud_p_x
     * @tc.desc Verify Bluetooth API cn 42 update cloud p repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_42_update_cloud_p_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_42_update_cloud_p_x: invoke');
      try {
        const c6:connection.TrustedPairedDevices={trustedPairedDevices:[]};
        connection.updateCloudBluetoothDevice(c6);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_42_update_cloud_p_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_42_update_cloud_p_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_43_gen_oob_p', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_43_GEN_OOB_P_N
     * @tc.name cn_43_gen_oob_p_n
     * @tc.desc Verify Bluetooth API cn 43 gen oob p normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_43_gen_oob_p_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_n: invoke');
      try {
        connection.generateLocalOobData(connection.BluetoothTransport.TRANSPORT_BR_EDR);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_43_GEN_OOB_P_E
     * @tc.name cn_43_gen_oob_p_e
     * @tc.desc Verify Bluetooth API cn 43 gen oob p error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_43_gen_oob_p_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_e: invoke');
      try {
        connection.generateLocalOobData(connection.BluetoothTransport.TRANSPORT_LE);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_43_GEN_OOB_P_B
     * @tc.name cn_43_gen_oob_p_b
     * @tc.desc Verify Bluetooth API cn 43 gen oob p boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_43_gen_oob_p_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_b: invoke');
      try {
        connection.generateLocalOobData(connection.BluetoothTransport.TRANSPORT_BR_EDR);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_43_GEN_OOB_P_E2
     * @tc.name cn_43_gen_oob_p_e2
     * @tc.desc Verify Bluetooth apitest case cn 43 gen oob p e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_43_gen_oob_p_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_e2: invoke');
      try {
        connection.generateLocalOobData(connection.BluetoothTransport.TRANSPORT_BR_EDR);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_43_GEN_OOB_P_R
     * @tc.name cn_43_gen_oob_p_r
     * @tc.desc Verify Bluetooth API cn 43 gen oob p return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_43_gen_oob_p_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_r: invoke');
      try {
        connection.generateLocalOobData(connection.BluetoothTransport.TRANSPORT_BR_EDR);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_43_GEN_OOB_P_X
     * @tc.name cn_43_gen_oob_p_x
     * @tc.desc Verify Bluetooth API cn 43 gen oob p repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_43_gen_oob_p_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_x: invoke');
      try {
        connection.generateLocalOobData(connection.BluetoothTransport.TRANSPORT_LE);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_43_gen_oob_p_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_44_virt_hash', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_44_VIRT_HASH_N
     * @tc.name cn_44_virt_hash_n
     * @tc.desc Verify Bluetooth API cn 44 virt hash normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_44_virt_hash_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_n: invoke');
      try {
        value = connection.getVirtualAddressByHash(connection.HashAlgorithmType.HASH_ALGORITHM_SHA256,
          '00') as Object;
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_44_virt_hash_n: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || (value !== null && value !== undefined)).assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_44_VIRT_HASH_E
     * @tc.name cn_44_virt_hash_e
     * @tc.desc Verify Bluetooth API cn 44 virt hash error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_44_virt_hash_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_e: invoke expect error');
      try {
        connection.getVirtualAddressByHash(connection.HashAlgorithmType.HASH_ALGORITHM_SHA256, '');
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_44_VIRT_HASH_B
     * @tc.name cn_44_virt_hash_b
     * @tc.desc Verify Bluetooth API cn 44 virt hash boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_44_virt_hash_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_b: invoke');
      try {
        connection.getVirtualAddressByHash(connection.HashAlgorithmType.HASH_ALGORITHM_SHA256, 'ff');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_44_VIRT_HASH_E2
     * @tc.name cn_44_virt_hash_e2
     * @tc.desc Verify Bluetooth apitest case cn 44 virt hash e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_44_virt_hash_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_e2: invoke');
      try {
        connection.getVirtualAddressByHash(connection.HashAlgorithmType.HASH_ALGORITHM_SHA256, 'aa');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_44_VIRT_HASH_R
     * @tc.name cn_44_virt_hash_r
     * @tc.desc Verify Bluetooth API cn 44 virt hash return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_44_virt_hash_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_r: invoke');
      try {
        connection.getVirtualAddressByHash(connection.HashAlgorithmType.HASH_ALGORITHM_SHA256, '00');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_44_VIRT_HASH_X
     * @tc.name cn_44_virt_hash_x
     * @tc.desc Verify Bluetooth API cn 44 virt hash repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_44_virt_hash_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_x: invoke');
      try {
        connection.getVirtualAddressByHash(connection.HashAlgorithmType.HASH_ALGORITHM_SHA256, '11');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_44_virt_hash_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_45_car_key_set', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_45_CAR_KEY_SET_N
     * @tc.name cn_45_car_key_set_n
     * @tc.desc Verify Bluetooth API cn 45 car key set normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_45_car_key_set_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_n: invoke');
      try {
        connection.setCarKeyDfxData(BADDR, 0);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_45_CAR_KEY_SET_E
     * @tc.name cn_45_car_key_set_e
     * @tc.desc Verify Bluetooth API cn 45 car key set error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_45_car_key_set_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_e: invoke expect error');
      try {
        connection.setCarKeyDfxData('', 0);
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_45_CAR_KEY_SET_B
     * @tc.name cn_45_car_key_set_b
     * @tc.desc Verify Bluetooth API cn 45 car key set boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_45_car_key_set_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_b: invoke');
      try {
        connection.setCarKeyDfxData(BADDR, 1);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_45_CAR_KEY_SET_E2
     * @tc.name cn_45_car_key_set_e2
     * @tc.desc Verify Bluetooth apitest case cn 45 car key set e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_45_car_key_set_e2', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_e2: invoke expect error');
      try {
        connection.setCarKeyDfxData('', 0);
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_e2: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_45_CAR_KEY_SET_R
     * @tc.name cn_45_car_key_set_r
     * @tc.desc Verify Bluetooth API cn 45 car key set return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_45_car_key_set_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_r: invoke');
      try {
        connection.setCarKeyDfxData(BADDR, 0);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_45_CAR_KEY_SET_X
     * @tc.name cn_45_car_key_set_x
     * @tc.desc Verify Bluetooth API cn 45 car key set repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_45_car_key_set_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_x: invoke');
      try {
        connection.setCarKeyDfxData(BADDR, 1);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_45_car_key_set_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_46_car_key_get', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_46_CAR_KEY_GET_N
     * @tc.name cn_46_car_key_get_n
     * @tc.desc Verify Bluetooth API cn 46 car key get normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_46_car_key_get_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_n: invoke');
      try {
        value = connection.getCarKeyDfxData() as Object;
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_46_car_key_get_n: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || (value !== null && value !== undefined)).assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_46_CAR_KEY_GET_E
     * @tc.name cn_46_car_key_get_e
     * @tc.desc Verify Bluetooth API cn 46 car key get error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_46_car_key_get_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_e: invoke');
      try {
        connection.getCarKeyDfxData();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_46_CAR_KEY_GET_B
     * @tc.name cn_46_car_key_get_b
     * @tc.desc Verify Bluetooth API cn 46 car key get boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_46_car_key_get_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_b: invoke');
      try {
        connection.getCarKeyDfxData();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_46_CAR_KEY_GET_E2
     * @tc.name cn_46_car_key_get_e2
     * @tc.desc Verify Bluetooth apitest case cn 46 car key get e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_46_car_key_get_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_e2: invoke');
      try {
        connection.getCarKeyDfxData();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_46_CAR_KEY_GET_R
     * @tc.name cn_46_car_key_get_r
     * @tc.desc Verify Bluetooth API cn 46 car key get return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_46_car_key_get_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_r: invoke');
      try {
        connection.getCarKeyDfxData();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_46_CAR_KEY_GET_X
     * @tc.name cn_46_car_key_get_x
     * @tc.desc Verify Bluetooth API cn 46 car key get repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_46_car_key_get_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_x: invoke');
      try {
        connection.getCarKeyDfxData();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_46_car_key_get_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_47_on_dev_find', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_47_ON_DEV_FIND_N
     * @tc.name cn_47_on_dev_find_n
     * @tc.desc Verify Bluetooth API cn 47 on dev find normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_47_on_dev_find_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_n: invoke');
      try {
        connection.on('bluetoothDeviceFind', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_47_ON_DEV_FIND_E
     * @tc.name cn_47_on_dev_find_e
     * @tc.desc Verify Bluetooth API cn 47 on dev find error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_47_on_dev_find_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_e: invoke');
      try {
        connection.on('bluetoothDeviceFind', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_47_ON_DEV_FIND_B
     * @tc.name cn_47_on_dev_find_b
     * @tc.desc Verify Bluetooth API cn 47 on dev find boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_47_on_dev_find_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_b: invoke');
      try {
        connection.on('bluetoothDeviceFind', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_47_ON_DEV_FIND_E2
     * @tc.name cn_47_on_dev_find_e2
     * @tc.desc Verify Bluetooth apitest case cn 47 on dev find e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_47_on_dev_find_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_e2: invoke');
      try {
        connection.on('bluetoothDeviceFind', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_47_ON_DEV_FIND_R
     * @tc.name cn_47_on_dev_find_r
     * @tc.desc Verify Bluetooth API cn 47 on dev find return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_47_on_dev_find_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_r: invoke');
      try {
        connection.on('bluetoothDeviceFind', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_47_ON_DEV_FIND_X
     * @tc.name cn_47_on_dev_find_x
     * @tc.desc Verify Bluetooth API cn 47 on dev find repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_47_on_dev_find_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_x: invoke');
      try {
        connection.on('bluetoothDeviceFind', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_47_on_dev_find_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_48_off_dev_find', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_48_OFF_DEV_FIND_N
     * @tc.name cn_48_off_dev_find_n
     * @tc.desc Verify Bluetooth API cn 48 off dev find normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_48_off_dev_find_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_n: invoke');
      try {
        connection.off('bluetoothDeviceFind');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_48_OFF_DEV_FIND_E
     * @tc.name cn_48_off_dev_find_e
     * @tc.desc Verify Bluetooth API cn 48 off dev find error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_48_off_dev_find_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_e: invoke');
      try {
        connection.off('bluetoothDeviceFind');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_48_OFF_DEV_FIND_B
     * @tc.name cn_48_off_dev_find_b
     * @tc.desc Verify Bluetooth API cn 48 off dev find boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_48_off_dev_find_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_b: invoke');
      try {
        connection.off('bluetoothDeviceFind', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_48_OFF_DEV_FIND_E2
     * @tc.name cn_48_off_dev_find_e2
     * @tc.desc Verify Bluetooth apitest case cn 48 off dev find e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_48_off_dev_find_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_e2: invoke');
      try {
        connection.off('bluetoothDeviceFind');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_48_OFF_DEV_FIND_R
     * @tc.name cn_48_off_dev_find_r
     * @tc.desc Verify Bluetooth API cn 48 off dev find return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_48_off_dev_find_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_r: invoke');
      try {
        connection.off('bluetoothDeviceFind');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_48_OFF_DEV_FIND_X
     * @tc.name cn_48_off_dev_find_x
     * @tc.desc Verify Bluetooth API cn 48 off dev find repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_48_off_dev_find_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_x: invoke');
      try {
        connection.off('bluetoothDeviceFind');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_48_off_dev_find_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_49_on_disc_res', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_49_ON_DISC_RES_N
     * @tc.name cn_49_on_disc_res_n
     * @tc.desc Verify Bluetooth API cn 49 on disc res normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_49_on_disc_res_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_n: invoke');
      try {
        connection.on('discoveryResult', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_49_ON_DISC_RES_E
     * @tc.name cn_49_on_disc_res_e
     * @tc.desc Verify Bluetooth API cn 49 on disc res error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_49_on_disc_res_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_e: invoke');
      try {
        connection.on('discoveryResult', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_49_ON_DISC_RES_B
     * @tc.name cn_49_on_disc_res_b
     * @tc.desc Verify Bluetooth API cn 49 on disc res boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_49_on_disc_res_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_b: invoke');
      try {
        connection.on('discoveryResult', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_49_ON_DISC_RES_E2
     * @tc.name cn_49_on_disc_res_e2
     * @tc.desc Verify Bluetooth apitest case cn 49 on disc res e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_49_on_disc_res_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_e2: invoke');
      try {
        connection.on('discoveryResult', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_49_ON_DISC_RES_R
     * @tc.name cn_49_on_disc_res_r
     * @tc.desc Verify Bluetooth API cn 49 on disc res return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_49_on_disc_res_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_r: invoke');
      try {
        connection.on('discoveryResult', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_49_ON_DISC_RES_X
     * @tc.name cn_49_on_disc_res_x
     * @tc.desc Verify Bluetooth API cn 49 on disc res repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_49_on_disc_res_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_x: invoke');
      try {
        connection.on('discoveryResult', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_49_on_disc_res_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_50_off_disc_res', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_50_OFF_DISC_RES_N
     * @tc.name cn_50_off_disc_res_n
     * @tc.desc Verify Bluetooth API cn 50 off disc res normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_50_off_disc_res_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_n: invoke');
      try {
        connection.off('discoveryResult');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_50_OFF_DISC_RES_E
     * @tc.name cn_50_off_disc_res_e
     * @tc.desc Verify Bluetooth API cn 50 off disc res error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_50_off_disc_res_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_e: invoke');
      try {
        connection.off('discoveryResult');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_50_OFF_DISC_RES_B
     * @tc.name cn_50_off_disc_res_b
     * @tc.desc Verify Bluetooth API cn 50 off disc res boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_50_off_disc_res_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_b: invoke');
      try {
        connection.off('discoveryResult', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_50_OFF_DISC_RES_E2
     * @tc.name cn_50_off_disc_res_e2
     * @tc.desc Verify Bluetooth apitest case cn 50 off disc res e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_50_off_disc_res_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_e2: invoke');
      try {
        connection.off('discoveryResult');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_50_OFF_DISC_RES_R
     * @tc.name cn_50_off_disc_res_r
     * @tc.desc Verify Bluetooth API cn 50 off disc res return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_50_off_disc_res_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_r: invoke');
      try {
        connection.off('discoveryResult');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_50_OFF_DISC_RES_X
     * @tc.name cn_50_off_disc_res_x
     * @tc.desc Verify Bluetooth API cn 50 off disc res repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_50_off_disc_res_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_x: invoke');
      try {
        connection.off('discoveryResult');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_50_off_disc_res_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_51_on_bond', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_51_ON_BOND_N
     * @tc.name cn_51_on_bond_n
     * @tc.desc Verify Bluetooth API cn 51 on bond normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_51_on_bond_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_51_on_bond_n: invoke');
      try {
        connection.on('bondStateChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_51_on_bond_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_51_on_bond_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_51_ON_BOND_E
     * @tc.name cn_51_on_bond_e
     * @tc.desc Verify Bluetooth API cn 51 on bond error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_51_on_bond_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_51_on_bond_e: invoke');
      try {
        connection.on('bondStateChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_51_on_bond_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_51_on_bond_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_51_ON_BOND_B
     * @tc.name cn_51_on_bond_b
     * @tc.desc Verify Bluetooth API cn 51 on bond boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_51_on_bond_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_51_on_bond_b: invoke');
      try {
        connection.on('bondStateChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_51_on_bond_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_51_on_bond_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_51_ON_BOND_E2
     * @tc.name cn_51_on_bond_e2
     * @tc.desc Verify Bluetooth apitest case cn 51 on bond e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_51_on_bond_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_51_on_bond_e2: invoke');
      try {
        connection.on('bondStateChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_51_on_bond_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_51_on_bond_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_51_ON_BOND_R
     * @tc.name cn_51_on_bond_r
     * @tc.desc Verify Bluetooth API cn 51 on bond return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_51_on_bond_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_51_on_bond_r: invoke');
      try {
        connection.on('bondStateChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_51_on_bond_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_51_on_bond_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_51_ON_BOND_X
     * @tc.name cn_51_on_bond_x
     * @tc.desc Verify Bluetooth API cn 51 on bond repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_51_on_bond_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_51_on_bond_x: invoke');
      try {
        connection.on('bondStateChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_51_on_bond_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_51_on_bond_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_52_off_bond', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_52_OFF_BOND_N
     * @tc.name cn_52_off_bond_n
     * @tc.desc Verify Bluetooth API cn 52 off bond normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_52_off_bond_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_52_off_bond_n: invoke');
      try {
        connection.off('bondStateChange');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_52_off_bond_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_52_off_bond_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_52_OFF_BOND_E
     * @tc.name cn_52_off_bond_e
     * @tc.desc Verify Bluetooth API cn 52 off bond error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_52_off_bond_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_52_off_bond_e: invoke');
      try {
        connection.off('bondStateChange');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_52_off_bond_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_52_off_bond_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_52_OFF_BOND_B
     * @tc.name cn_52_off_bond_b
     * @tc.desc Verify Bluetooth API cn 52 off bond boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_52_off_bond_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_52_off_bond_b: invoke');
      try {
        connection.off('bondStateChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_52_off_bond_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_52_off_bond_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_52_OFF_BOND_E2
     * @tc.name cn_52_off_bond_e2
     * @tc.desc Verify Bluetooth apitest case cn 52 off bond e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_52_off_bond_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_52_off_bond_e2: invoke');
      try {
        connection.off('bondStateChange');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_52_off_bond_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_52_off_bond_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_52_OFF_BOND_R
     * @tc.name cn_52_off_bond_r
     * @tc.desc Verify Bluetooth API cn 52 off bond return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_52_off_bond_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_52_off_bond_r: invoke');
      try {
        connection.off('bondStateChange');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_52_off_bond_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_52_off_bond_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_52_OFF_BOND_X
     * @tc.name cn_52_off_bond_x
     * @tc.desc Verify Bluetooth API cn 52 off bond repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_52_off_bond_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_52_off_bond_x: invoke');
      try {
        connection.off('bondStateChange');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_52_off_bond_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_52_off_bond_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_53_on_pin', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_53_ON_PIN_N
     * @tc.name cn_53_on_pin_n
     * @tc.desc Verify Bluetooth API cn 53 on pin normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_53_on_pin_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_53_on_pin_n: invoke');
      try {
        connection.on('pinRequired', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_53_on_pin_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_53_on_pin_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_53_ON_PIN_E
     * @tc.name cn_53_on_pin_e
     * @tc.desc Verify Bluetooth API cn 53 on pin error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_53_on_pin_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_53_on_pin_e: invoke');
      try {
        connection.on('pinRequired', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_53_on_pin_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_53_on_pin_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_53_ON_PIN_B
     * @tc.name cn_53_on_pin_b
     * @tc.desc Verify Bluetooth API cn 53 on pin boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_53_on_pin_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_53_on_pin_b: invoke');
      try {
        connection.on('pinRequired', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_53_on_pin_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_53_on_pin_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_53_ON_PIN_E2
     * @tc.name cn_53_on_pin_e2
     * @tc.desc Verify Bluetooth apitest case cn 53 on pin e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_53_on_pin_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_53_on_pin_e2: invoke');
      try {
        connection.on('pinRequired', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_53_on_pin_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_53_on_pin_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_53_ON_PIN_R
     * @tc.name cn_53_on_pin_r
     * @tc.desc Verify Bluetooth API cn 53 on pin return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_53_on_pin_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_53_on_pin_r: invoke');
      try {
        connection.on('pinRequired', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_53_on_pin_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_53_on_pin_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_53_ON_PIN_X
     * @tc.name cn_53_on_pin_x
     * @tc.desc Verify Bluetooth API cn 53 on pin repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_53_on_pin_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_53_on_pin_x: invoke');
      try {
        connection.on('pinRequired', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_53_on_pin_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_53_on_pin_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_54_off_pin', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_54_OFF_PIN_N
     * @tc.name cn_54_off_pin_n
     * @tc.desc Verify Bluetooth API cn 54 off pin normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_54_off_pin_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_54_off_pin_n: invoke');
      try {
        connection.off('pinRequired');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_54_off_pin_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_54_off_pin_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_54_OFF_PIN_E
     * @tc.name cn_54_off_pin_e
     * @tc.desc Verify Bluetooth API cn 54 off pin error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_54_off_pin_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_54_off_pin_e: invoke');
      try {
        connection.off('pinRequired');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_54_off_pin_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_54_off_pin_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_54_OFF_PIN_B
     * @tc.name cn_54_off_pin_b
     * @tc.desc Verify Bluetooth API cn 54 off pin boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_54_off_pin_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_54_off_pin_b: invoke');
      try {
        connection.off('pinRequired', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_54_off_pin_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_54_off_pin_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_54_OFF_PIN_E2
     * @tc.name cn_54_off_pin_e2
     * @tc.desc Verify Bluetooth apitest case cn 54 off pin e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_54_off_pin_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_54_off_pin_e2: invoke');
      try {
        connection.off('pinRequired');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_54_off_pin_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_54_off_pin_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_54_OFF_PIN_R
     * @tc.name cn_54_off_pin_r
     * @tc.desc Verify Bluetooth API cn 54 off pin return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_54_off_pin_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_54_off_pin_r: invoke');
      try {
        connection.off('pinRequired');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_54_off_pin_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_54_off_pin_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_54_OFF_PIN_X
     * @tc.name cn_54_off_pin_x
     * @tc.desc Verify Bluetooth API cn 54 off pin repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_54_off_pin_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_54_off_pin_x: invoke');
      try {
        connection.off('pinRequired');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_54_off_pin_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_54_off_pin_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_55_on_batt', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_55_ON_BATT_N
     * @tc.name cn_55_on_batt_n
     * @tc.desc Verify Bluetooth API cn 55 on batt normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_55_on_batt_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_55_on_batt_n: invoke');
      try {
        connection.on('batteryChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_55_on_batt_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_55_on_batt_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_55_ON_BATT_E
     * @tc.name cn_55_on_batt_e
     * @tc.desc Verify Bluetooth API cn 55 on batt error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_55_on_batt_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_55_on_batt_e: invoke');
      try {
        connection.on('batteryChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_55_on_batt_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_55_on_batt_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_55_ON_BATT_B
     * @tc.name cn_55_on_batt_b
     * @tc.desc Verify Bluetooth API cn 55 on batt boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_55_on_batt_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_55_on_batt_b: invoke');
      try {
        connection.on('batteryChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_55_on_batt_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_55_on_batt_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_55_ON_BATT_E2
     * @tc.name cn_55_on_batt_e2
     * @tc.desc Verify Bluetooth apitest case cn 55 on batt e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_55_on_batt_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_55_on_batt_e2: invoke');
      try {
        connection.on('batteryChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_55_on_batt_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_55_on_batt_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_55_ON_BATT_R
     * @tc.name cn_55_on_batt_r
     * @tc.desc Verify Bluetooth API cn 55 on batt return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_55_on_batt_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_55_on_batt_r: invoke');
      try {
        connection.on('batteryChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_55_on_batt_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_55_on_batt_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_55_ON_BATT_X
     * @tc.name cn_55_on_batt_x
     * @tc.desc Verify Bluetooth API cn 55 on batt repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_55_on_batt_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_55_on_batt_x: invoke');
      try {
        connection.on('batteryChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_55_on_batt_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_55_on_batt_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_56_off_batt', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_56_OFF_BATT_N
     * @tc.name cn_56_off_batt_n
     * @tc.desc Verify Bluetooth API cn 56 off batt normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_56_off_batt_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_56_off_batt_n: invoke');
      try {
        connection.off('batteryChange');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_56_off_batt_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_56_off_batt_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_56_OFF_BATT_E
     * @tc.name cn_56_off_batt_e
     * @tc.desc Verify Bluetooth API cn 56 off batt error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_56_off_batt_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_56_off_batt_e: invoke');
      try {
        connection.off('batteryChange');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_56_off_batt_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_56_off_batt_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_56_OFF_BATT_B
     * @tc.name cn_56_off_batt_b
     * @tc.desc Verify Bluetooth API cn 56 off batt boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_56_off_batt_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_56_off_batt_b: invoke');
      try {
        connection.off('batteryChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_56_off_batt_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_56_off_batt_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_56_OFF_BATT_E2
     * @tc.name cn_56_off_batt_e2
     * @tc.desc Verify Bluetooth apitest case cn 56 off batt e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_56_off_batt_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_56_off_batt_e2: invoke');
      try {
        connection.off('batteryChange');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_56_off_batt_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_56_off_batt_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_56_OFF_BATT_R
     * @tc.name cn_56_off_batt_r
     * @tc.desc Verify Bluetooth API cn 56 off batt return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_56_off_batt_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_56_off_batt_r: invoke');
      try {
        connection.off('batteryChange');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_56_off_batt_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_56_off_batt_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_56_OFF_BATT_X
     * @tc.name cn_56_off_batt_x
     * @tc.desc Verify Bluetooth API cn 56 off batt repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_56_off_batt_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_56_off_batt_x: invoke');
      try {
        connection.off('batteryChange');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_56_off_batt_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_56_off_batt_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_57_on_scan_mode', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_57_ON_SCAN_MODE_N
     * @tc.name cn_57_on_scan_mode_n
     * @tc.desc Verify Bluetooth API cn 57 on scan mode normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_57_on_scan_mode_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_n: invoke');
      try {
        connection.onScanModeChange(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_57_ON_SCAN_MODE_E
     * @tc.name cn_57_on_scan_mode_e
     * @tc.desc Verify Bluetooth API cn 57 on scan mode error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_57_on_scan_mode_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_e: invoke');
      try {
        connection.onScanModeChange(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_57_ON_SCAN_MODE_B
     * @tc.name cn_57_on_scan_mode_b
     * @tc.desc Verify Bluetooth API cn 57 on scan mode boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_57_on_scan_mode_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_b: invoke');
      try {
        connection.onScanModeChange(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_57_ON_SCAN_MODE_E2
     * @tc.name cn_57_on_scan_mode_e2
     * @tc.desc Verify Bluetooth apitest case cn 57 on scan mode e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_57_on_scan_mode_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_e2: invoke');
      try {
        connection.onScanModeChange(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_57_ON_SCAN_MODE_R
     * @tc.name cn_57_on_scan_mode_r
     * @tc.desc Verify Bluetooth API cn 57 on scan mode return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_57_on_scan_mode_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_r: invoke');
      try {
        connection.onScanModeChange(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_57_ON_SCAN_MODE_X
     * @tc.name cn_57_on_scan_mode_x
     * @tc.desc Verify Bluetooth API cn 57 on scan mode repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_57_on_scan_mode_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_x: invoke');
      try {
        connection.onScanModeChange(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_57_on_scan_mode_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_58_off_scan_mode', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_58_OFF_SCAN_MODE_N
     * @tc.name cn_58_off_scan_mode_n
     * @tc.desc Verify Bluetooth API cn 58 off scan mode normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_58_off_scan_mode_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_n: invoke');
      try {
        connection.offScanModeChange();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_58_OFF_SCAN_MODE_E
     * @tc.name cn_58_off_scan_mode_e
     * @tc.desc Verify Bluetooth API cn 58 off scan mode error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_58_off_scan_mode_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_e: invoke');
      try {
        connection.offScanModeChange();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_58_OFF_SCAN_MODE_B
     * @tc.name cn_58_off_scan_mode_b
     * @tc.desc Verify Bluetooth API cn 58 off scan mode boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_58_off_scan_mode_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_b: invoke');
      try {
        connection.offScanModeChange(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_58_OFF_SCAN_MODE_E2
     * @tc.name cn_58_off_scan_mode_e2
     * @tc.desc Verify Bluetooth apitest case cn 58 off scan mode e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_58_off_scan_mode_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_e2: invoke');
      try {
        connection.offScanModeChange();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_58_OFF_SCAN_MODE_R
     * @tc.name cn_58_off_scan_mode_r
     * @tc.desc Verify Bluetooth API cn 58 off scan mode return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_58_off_scan_mode_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_r: invoke');
      try {
        connection.offScanModeChange();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_58_OFF_SCAN_MODE_X
     * @tc.name cn_58_off_scan_mode_x
     * @tc.desc Verify Bluetooth API cn 58 off scan mode repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_58_off_scan_mode_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_x: invoke');
      try {
        connection.offScanModeChange();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_58_off_scan_mode_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
  });
}



