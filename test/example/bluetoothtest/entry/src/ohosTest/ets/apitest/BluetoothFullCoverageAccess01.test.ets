      assertPeerExpect(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        wearDetection.disableWearDetection(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_DISABLEWEARDETECTION_C
     * @tc.name wd_disableWearDetection_c
     * @tc.desc Verify Bluetooth API wd disableWearDetection condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_disableWearDetection_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        wearDetection.disableWearDetection(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_DISABLEWEARDETECTION_PERF
     * @tc.name wd_disableWearDetection_perf
     * @tc.desc Verify Bluetooth apitest case wd disableWearDetection perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_disableWearDetection_perf', 0, () => {
      runPerf('wd_disableWearDetection_perf', 5000, () => {
        try {
          wearDetection.disableWearDetection(BADDR);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_DISABLEWEARDETECTION_STRESS
     * @tc.name wd_disableWearDetection_stress
     * @tc.desc Verify Bluetooth apitest case wd disableWearDetection stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_disableWearDetection_stress', 0, () => {
      runStress('wd_disableWearDetection_stress', 20, () => {
        try {
          wearDetection.disableWearDetection(BADDR);
        } catch (_e) {
        }
      });
    });
    });
    describe('wd_isWearDetectionEnabled', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONENABLED_N
     * @tc.name wd_isWearDetectionEnabled_n
     * @tc.desc Verify Bluetooth API wd isWearDetectionEnabled normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionEnabled_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = wearDetection.isWearDetectionEnabled(BADDR) as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONENABLED_E
     * @tc.name wd_isWearDetectionEnabled_e
     * @tc.desc Verify Bluetooth API wd isWearDetectionEnabled error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionEnabled_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        wearDetection.isWearDetectionEnabled(EMPTY_MAC);
      } catch (err) {
        threw = true;
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: threw=' + threw + ' errCode=' + errCode);
      assertThrownErrIn(threw, errCode, BT_DOC_ERR_CODES);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONENABLED_B
     * @tc.name wd_isWearDetectionEnabled_b
     * @tc.desc Verify Bluetooth API wd isWearDetectionEnabled boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionEnabled_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        wearDetection.isWearDetectionEnabled(BAD_MAC);
      } catch (err) {
        threw = true;
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: threw=' + threw + ' errCode=' + errCode);
      assertThrownErrIn(threw, errCode, BT_DOC_ERR_CODES);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONENABLED_R
     * @tc.name wd_isWearDetectionEnabled_r
     * @tc.desc Verify Bluetooth API wd isWearDetectionEnabled return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionEnabled_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = wearDetection.isWearDetectionEnabled(BADDR) as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONENABLED_X
     * @tc.name wd_isWearDetectionEnabled_x
     * @tc.desc Verify Bluetooth API wd isWearDetectionEnabled repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionEnabled_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = wearDetection.isWearDetectionEnabled(BADDR) as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
      errCode = 0;
      value = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        value = wearDetection.isWearDetectionEnabled(BADDR) as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONENABLED_C
     * @tc.name wd_isWearDetectionEnabled_c
     * @tc.desc Verify Bluetooth API wd isWearDetectionEnabled condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionEnabled_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = wearDetection.isWearDetectionEnabled(BADDR) as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONENABLED_PERF
     * @tc.name wd_isWearDetectionEnabled_perf
     * @tc.desc Verify Bluetooth apitest case wd isWearDetectionEnabled perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionEnabled_perf', 0, () => {
      runPerf('wd_isWearDetectionEnabled_perf', 5000, () => {
        try {
          wearDetection.isWearDetectionEnabled(BADDR) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONENABLED_STRESS
     * @tc.name wd_isWearDetectionEnabled_stress
     * @tc.desc Verify Bluetooth apitest case wd isWearDetectionEnabled stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionEnabled_stress', 0, () => {
      runStress('wd_isWearDetectionEnabled_stress', 20, () => {
        try {
          wearDetection.isWearDetectionEnabled(BADDR) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('wd_isWearDetectionSupported', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONSUPPORTED_N
     * @tc.name wd_isWearDetectionSupported_n
     * @tc.desc Verify Bluetooth API wd isWearDetectionSupported normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionSupported_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = wearDetection.isWearDetectionSupported(BADDR) as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONSUPPORTED_E
     * @tc.name wd_isWearDetectionSupported_e
     * @tc.desc Verify Bluetooth API wd isWearDetectionSupported error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionSupported_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        wearDetection.isWearDetectionSupported(EMPTY_MAC);
      } catch (err) {
        threw = true;
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'e: threw=' + threw + ' errCode=' + errCode);
      assertThrownErrIn(threw, errCode, BT_DOC_ERR_CODES);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONSUPPORTED_B
     * @tc.name wd_isWearDetectionSupported_b
     * @tc.desc Verify Bluetooth API wd isWearDetectionSupported boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionSupported_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        wearDetection.isWearDetectionSupported(BAD_MAC);
      } catch (err) {
        threw = true;
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: threw=' + threw + ' errCode=' + errCode);
      assertThrownErrIn(threw, errCode, BT_DOC_ERR_CODES);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONSUPPORTED_R
     * @tc.name wd_isWearDetectionSupported_r
     * @tc.desc Verify Bluetooth API wd isWearDetectionSupported return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionSupported_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = wearDetection.isWearDetectionSupported(BADDR) as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONSUPPORTED_X
     * @tc.name wd_isWearDetectionSupported_x
     * @tc.desc Verify Bluetooth API wd isWearDetectionSupported repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionSupported_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = wearDetection.isWearDetectionSupported(BADDR) as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
      errCode = 0;
      value = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        value = wearDetection.isWearDetectionSupported(BADDR) as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONSUPPORTED_C
     * @tc.name wd_isWearDetectionSupported_c
     * @tc.desc Verify Bluetooth API wd isWearDetectionSupported condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionSupported_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = wearDetection.isWearDetectionSupported(BADDR) as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONSUPPORTED_PERF
     * @tc.name wd_isWearDetectionSupported_perf
     * @tc.desc Verify Bluetooth apitest case wd isWearDetectionSupported perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionSupported_perf', 0, () => {
      runPerf('wd_isWearDetectionSupported_perf', 5000, () => {
        try {
          wearDetection.isWearDetectionSupported(BADDR) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ISWEARDETECTIONSUPPORTED_STRESS
     * @tc.name wd_isWearDetectionSupported_stress
     * @tc.desc Verify Bluetooth apitest case wd isWearDetectionSupported stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_isWearDetectionSupported_stress', 0, () => {
      runStress('wd_isWearDetectionSupported_stress', 20, () => {
        try {
          wearDetection.isWearDetectionSupported(BADDR) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('sys_startBLEScan', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STARTBLESCAN_N
     * @tc.name sys_startBLEScan_n
     * @tc.desc Verify Bluetooth API sys startBLEScan normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_startBLEScan_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        ble.startBLEScan([]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STARTBLESCAN_E
     * @tc.name sys_startBLEScan_e
     * @tc.desc Verify Bluetooth API sys startBLEScan error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_startBLEScan_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        ble.startBLEScan([]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STARTBLESCAN_B
     * @tc.name sys_startBLEScan_b
     * @tc.desc Verify Bluetooth API sys startBLEScan boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_startBLEScan_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        ble.startBLEScan([]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STARTBLESCAN_R
     * @tc.name sys_startBLEScan_r
     * @tc.desc Verify Bluetooth API sys startBLEScan return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_startBLEScan_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        ble.startBLEScan([]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STARTBLESCAN_X
     * @tc.name sys_startBLEScan_x
     * @tc.desc Verify Bluetooth API sys startBLEScan repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_startBLEScan_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        ble.startBLEScan([]);
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
        ble.startBLEScan([]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STARTBLESCAN_C
     * @tc.name sys_startBLEScan_c
     * @tc.desc Verify Bluetooth API sys startBLEScan condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_startBLEScan_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        ble.startBLEScan([]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STARTBLESCAN_PERF
     * @tc.name sys_startBLEScan_perf
     * @tc.desc Verify Bluetooth apitest case sys startBLEScan perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_startBLEScan_perf', 0, () => {
      runPerf('sys_startBLEScan_perf', 5000, () => {
        try {
          ble.startBLEScan([]);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STARTBLESCAN_STRESS
     * @tc.name sys_startBLEScan_stress
     * @tc.desc Verify Bluetooth apitest case sys startBLEScan stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_startBLEScan_stress', 0, () => {
      runStress('sys_startBLEScan_stress', 20, () => {
        try {
          ble.startBLEScan([]);
        } catch (_e) {
        }
      });
    });
    });
    describe('sys_stopBLEScan', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STOPBLESCAN_N
     * @tc.name sys_stopBLEScan_n
     * @tc.desc Verify Bluetooth API sys stopBLEScan normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_stopBLEScan_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        ble.stopBLEScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STOPBLESCAN_E
     * @tc.name sys_stopBLEScan_e
     * @tc.desc Verify Bluetooth API sys stopBLEScan error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_stopBLEScan_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        ble.stopBLEScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STOPBLESCAN_B
     * @tc.name sys_stopBLEScan_b
     * @tc.desc Verify Bluetooth API sys stopBLEScan boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_stopBLEScan_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        ble.stopBLEScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STOPBLESCAN_R
     * @tc.name sys_stopBLEScan_r
     * @tc.desc Verify Bluetooth API sys stopBLEScan return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_stopBLEScan_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        ble.stopBLEScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STOPBLESCAN_X
     * @tc.name sys_stopBLEScan_x
     * @tc.desc Verify Bluetooth API sys stopBLEScan repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_stopBLEScan_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        ble.stopBLEScan();
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
        ble.stopBLEScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STOPBLESCAN_C
     * @tc.name sys_stopBLEScan_c
     * @tc.desc Verify Bluetooth API sys stopBLEScan condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_stopBLEScan_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        ble.stopBLEScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STOPBLESCAN_PERF
     * @tc.name sys_stopBLEScan_perf
     * @tc.desc Verify Bluetooth apitest case sys stopBLEScan perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_stopBLEScan_perf', 0, () => {
      runPerf('sys_stopBLEScan_perf', 5000, () => {
        try {
          ble.stopBLEScan();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_STOPBLESCAN_STRESS
     * @tc.name sys_stopBLEScan_stress
     * @tc.desc Verify Bluetooth apitest case sys stopBLEScan stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_stopBLEScan_stress', 0, () => {
      runStress('sys_stopBLEScan_stress', 20, () => {
        try {
          ble.stopBLEScan();
        } catch (_e) {
        }
      });
    });
    });
    describe('sys_subscribeBLEFound', () => {
    it('sys_subscribeBLEFound_n', 0, async (done: Function) => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        sc.on('BLEDeviceFind', () => {});
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
    it('sys_subscribeBLEFound_e', 0, async (done: Function) => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        sc.on('BLEDeviceFind', () => {});
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
    it('sys_subscribeBLEFound_b', 0, async (done: Function) => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        sc.on('BLEDeviceFind', () => {});
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
    it('sys_subscribeBLEFound_r', 0, async (done: Function) => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        sc.on('BLEDeviceFind', () => {});
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
    it('sys_subscribeBLEFound_x', 0, async (done: Function) => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        sc.on('BLEDeviceFind', () => {});
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
        sc.on('BLEDeviceFind', () => {});
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
    it('sys_subscribeBLEFound_c', 0, async (done: Function) => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        sc.on('BLEDeviceFind', () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_SUBSCRIBEBLEFOUND_PERF
     * @tc.name sys_subscribeBLEFound_perf
     * @tc.desc Verify Bluetooth apitest case sys subscribeBLEFound perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_subscribeBLEFound_perf', 0, () => {
      runPerf('sys_subscribeBLEFound_perf', 5000, () => {
        try {
          const sc = ble.createBleScanner();
          sc.on('BLEDeviceFind', () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_SUBSCRIBEBLEFOUND_STRESS
     * @tc.name sys_subscribeBLEFound_stress
     * @tc.desc Verify Bluetooth apitest case sys subscribeBLEFound stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_subscribeBLEFound_stress', 0, () => {
      runStress('sys_subscribeBLEFound_stress', 20, () => {
        try {
          const sc = ble.createBleScanner();
          sc.on('BLEDeviceFind', () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('sys_unsubscribeBLEFound', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_UNSUBSCRIBEBLEFOUND_N
     * @tc.name sys_unsubscribeBLEFound_n
     * @tc.desc Verify Bluetooth API sys unsubscribeBLEFound normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_unsubscribeBLEFound_n', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        sc.off('BLEDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_UNSUBSCRIBEBLEFOUND_E
     * @tc.name sys_unsubscribeBLEFound_e
     * @tc.desc Verify Bluetooth API sys unsubscribeBLEFound error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_unsubscribeBLEFound_e', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        sc.off('BLEDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_UNSUBSCRIBEBLEFOUND_B
     * @tc.name sys_unsubscribeBLEFound_b
     * @tc.desc Verify Bluetooth API sys unsubscribeBLEFound boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_unsubscribeBLEFound_b', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        sc.off('BLEDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_UNSUBSCRIBEBLEFOUND_R
     * @tc.name sys_unsubscribeBLEFound_r
     * @tc.desc Verify Bluetooth API sys unsubscribeBLEFound return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_unsubscribeBLEFound_r', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        sc.off('BLEDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_UNSUBSCRIBEBLEFOUND_X
     * @tc.name sys_unsubscribeBLEFound_x
     * @tc.desc Verify Bluetooth API sys unsubscribeBLEFound repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_unsubscribeBLEFound_x', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        sc.off('BLEDeviceFind');
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
        sc.off('BLEDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_UNSUBSCRIBEBLEFOUND_C
     * @tc.name sys_unsubscribeBLEFound_c
     * @tc.desc Verify Bluetooth API sys unsubscribeBLEFound condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_unsubscribeBLEFound_c', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        sc.off('BLEDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_UNSUBSCRIBEBLEFOUND_PERF
     * @tc.name sys_unsubscribeBLEFound_perf
     * @tc.desc Verify Bluetooth apitest case sys unsubscribeBLEFound perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_unsubscribeBLEFound_perf', 0, () => {
      runPerf('sys_unsubscribeBLEFound_perf', 5000, () => {
        try {
          const sc = ble.createBleScanner();
          sc.off('BLEDeviceFind');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SYS_UNSUBSCRIBEBLEFOUND_STRESS
     * @tc.name sys_unsubscribeBLEFound_stress
     * @tc.desc Verify Bluetooth apitest case sys unsubscribeBLEFound stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sys_unsubscribeBLEFound_stress', 0, () => {
      runStress('sys_unsubscribeBLEFound_stress', 20, () => {
        try {
          const sc = ble.createBleScanner();
          sc.off('BLEDeviceFind');
        } catch (_e) {
        }
      });
    });
    });
  });
}


