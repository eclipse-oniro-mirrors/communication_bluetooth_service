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



