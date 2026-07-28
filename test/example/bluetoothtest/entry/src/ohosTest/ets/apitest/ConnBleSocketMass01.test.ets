     * @tc.name cn_25_stop_disc_x
     * @tc.desc Verify Bluetooth API cn 25 stop disc repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_25_stop_disc_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_x: invoke');
      try {
        connection.stopBluetoothDiscovery();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_26_is_discovering', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_26_IS_DISCOVERING_N
     * @tc.name cn_26_is_discovering_n
     * @tc.desc Verify Bluetooth API cn 26 is discovering normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_26_is_discovering_n', 0, () => {
      let errCode = 0;
      let value = false;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_n: invoke');
      try {
        value = connection.isBluetoothDiscovering();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_26_is_discovering_n: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(typeof value).assertEqual('boolean');
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_26_IS_DISCOVERING_E
     * @tc.name cn_26_is_discovering_e
     * @tc.desc Verify Bluetooth API cn 26 is discovering error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_26_is_discovering_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_e: invoke');
      try {
        connection.isBluetoothDiscovering();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_26_IS_DISCOVERING_B
     * @tc.name cn_26_is_discovering_b
     * @tc.desc Verify Bluetooth API cn 26 is discovering boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_26_is_discovering_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_b: invoke');
      try {
        connection.isBluetoothDiscovering();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_26_IS_DISCOVERING_E2
     * @tc.name cn_26_is_discovering_e2
     * @tc.desc Verify Bluetooth apitest case cn 26 is discovering e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_26_is_discovering_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_e2: invoke');
      try {
        connection.isBluetoothDiscovering();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_26_IS_DISCOVERING_R
     * @tc.name cn_26_is_discovering_r
     * @tc.desc Verify Bluetooth API cn 26 is discovering return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_26_is_discovering_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_r: invoke');
      try {
        connection.isBluetoothDiscovering();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_26_IS_DISCOVERING_X
     * @tc.name cn_26_is_discovering_x
     * @tc.desc Verify Bluetooth API cn 26 is discovering repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_26_is_discovering_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_x: invoke');
      try {
        connection.isBluetoothDiscovering();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_27_local_uuids_cb', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_27_LOCAL_UUIDS_CB_N
     * @tc.name cn_27_local_uuids_cb_n
     * @tc.desc Verify Bluetooth API cn 27 local uuids cb normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_27_local_uuids_cb_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_n: invoke');
      try {
        connection.getLocalProfileUuids(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_27_LOCAL_UUIDS_CB_E
     * @tc.name cn_27_local_uuids_cb_e
     * @tc.desc Verify Bluetooth API cn 27 local uuids cb error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_27_local_uuids_cb_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_e: invoke');
      try {
        connection.getLocalProfileUuids(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_27_LOCAL_UUIDS_CB_B
     * @tc.name cn_27_local_uuids_cb_b
     * @tc.desc Verify Bluetooth API cn 27 local uuids cb boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_27_local_uuids_cb_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_b: invoke');
      try {
        connection.getLocalProfileUuids(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_27_LOCAL_UUIDS_CB_E2
     * @tc.name cn_27_local_uuids_cb_e2
     * @tc.desc Verify Bluetooth apitest case cn 27 local uuids cb e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_27_local_uuids_cb_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_e2: invoke');
      try {
        connection.getLocalProfileUuids(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_27_LOCAL_UUIDS_CB_R
     * @tc.name cn_27_local_uuids_cb_r
     * @tc.desc Verify Bluetooth API cn 27 local uuids cb return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_27_local_uuids_cb_r', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_r: invoke');
      try {
        connection.getLocalProfileUuids(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_27_local_uuids_cb_r: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(Array.isArray(value)).assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_27_LOCAL_UUIDS_CB_X
     * @tc.name cn_27_local_uuids_cb_x
     * @tc.desc Verify Bluetooth API cn 27 local uuids cb repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_27_local_uuids_cb_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_x: invoke');
      try {
        connection.getLocalProfileUuids(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_28_local_uuids_p', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_28_LOCAL_UUIDS_P_N
     * @tc.name cn_28_local_uuids_p_n
     * @tc.desc Verify Bluetooth API cn 28 local uuids p normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_28_local_uuids_p_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_n: invoke');
      try {
        connection.getLocalProfileUuids();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_28_LOCAL_UUIDS_P_E
     * @tc.name cn_28_local_uuids_p_e
     * @tc.desc Verify Bluetooth API cn 28 local uuids p error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_28_local_uuids_p_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_e: invoke');
      try {
        connection.getLocalProfileUuids();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_28_LOCAL_UUIDS_P_B
     * @tc.name cn_28_local_uuids_p_b
     * @tc.desc Verify Bluetooth API cn 28 local uuids p boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_28_local_uuids_p_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_b: invoke');
      try {
        connection.getLocalProfileUuids();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_28_LOCAL_UUIDS_P_E2
     * @tc.name cn_28_local_uuids_p_e2
     * @tc.desc Verify Bluetooth apitest case cn 28 local uuids p e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_28_local_uuids_p_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_e2: invoke');
      try {
        connection.getLocalProfileUuids();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_28_LOCAL_UUIDS_P_R
     * @tc.name cn_28_local_uuids_p_r
     * @tc.desc Verify Bluetooth API cn 28 local uuids p return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_28_local_uuids_p_r', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_r: invoke');
      try {
        connection.getLocalProfileUuids();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_28_local_uuids_p_r: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(Array.isArray(value)).assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_28_LOCAL_UUIDS_P_X
     * @tc.name cn_28_local_uuids_p_x
     * @tc.desc Verify Bluetooth API cn 28 local uuids p repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_28_local_uuids_p_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_x: invoke');
      try {
        connection.getLocalProfileUuids();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_29_remote_uuids_cb', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_29_REMOTE_UUIDS_CB_N
     * @tc.name cn_29_remote_uuids_cb_n
     * @tc.desc Verify Bluetooth API cn 29 remote uuids cb normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_29_remote_uuids_cb_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_n: invoke');
      try {
        connection.getRemoteProfileUuids(BADDR, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_29_REMOTE_UUIDS_CB_E
     * @tc.name cn_29_remote_uuids_cb_e
     * @tc.desc Verify Bluetooth API cn 29 remote uuids cb error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_29_remote_uuids_cb_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_e: invoke expect error');
      try {
        connection.getRemoteProfileUuids('', () => {});
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_29_REMOTE_UUIDS_CB_B
     * @tc.name cn_29_remote_uuids_cb_b
     * @tc.desc Verify Bluetooth API cn 29 remote uuids cb boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_29_remote_uuids_cb_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_b: invoke');
      try {
        connection.getRemoteProfileUuids(BADDR, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_29_REMOTE_UUIDS_CB_E2
     * @tc.name cn_29_remote_uuids_cb_e2
     * @tc.desc Verify Bluetooth apitest case cn 29 remote uuids cb e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_29_remote_uuids_cb_e2', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_e2: invoke expect error');
      try {
        connection.getRemoteProfileUuids('', () => {});
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_e2: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_29_REMOTE_UUIDS_CB_R
     * @tc.name cn_29_remote_uuids_cb_r
     * @tc.desc Verify Bluetooth API cn 29 remote uuids cb return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_29_remote_uuids_cb_r', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_r: invoke');
      try {
        connection.getRemoteProfileUuids(BADDR, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_29_remote_uuids_cb_r: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(Array.isArray(value)).assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_29_REMOTE_UUIDS_CB_X
     * @tc.name cn_29_remote_uuids_cb_x
     * @tc.desc Verify Bluetooth API cn 29 remote uuids cb repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_29_remote_uuids_cb_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_x: invoke');
      try {
        connection.getRemoteProfileUuids(BADDR, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
  });
}



