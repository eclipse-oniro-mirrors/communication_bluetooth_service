      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        connection.getRemoteDeviceType(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICETYPE_B
     * @tc.name cn_getRemoteDeviceType_b
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceType boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceType_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        connection.getRemoteDeviceType(BAD_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICETYPE_R
     * @tc.name cn_getRemoteDeviceType_r
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceType return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceType_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = connection.getRemoteDeviceType(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICETYPE_X
     * @tc.name cn_getRemoteDeviceType_x
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceType repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceType_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = connection.getRemoteDeviceType(BADDR) as Object;
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
        value = connection.getRemoteDeviceType(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICETYPE_C
     * @tc.name cn_getRemoteDeviceType_c
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceType condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceType_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = connection.getRemoteDeviceType(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICETYPE_PERF
     * @tc.name cn_getRemoteDeviceType_perf
     * @tc.desc Verify Bluetooth apitest case cn getRemoteDeviceType perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceType_perf', 0, () => {
      runPerf('cn_getRemoteDeviceType_perf', 5000, () => {
        try {
          connection.getRemoteDeviceType(BADDR) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICETYPE_STRESS
     * @tc.name cn_getRemoteDeviceType_stress
     * @tc.desc Verify Bluetooth apitest case cn getRemoteDeviceType stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceType_stress', 0, () => {
      runStress('cn_getRemoteDeviceType_stress', 20, () => {
        try {
          connection.getRemoteDeviceType(BADDR) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_setRemoteDeviceType', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_SETREMOTEDEVICETYPE_N
     * @tc.name cn_setRemoteDeviceType_n
     * @tc.desc Verify Bluetooth API cn setRemoteDeviceType normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_setRemoteDeviceType_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        connection.setRemoteDeviceType(BADDR, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_SETREMOTEDEVICETYPE_E
     * @tc.name cn_setRemoteDeviceType_e
     * @tc.desc Verify Bluetooth API cn setRemoteDeviceType error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_setRemoteDeviceType_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        connection.setRemoteDeviceType(EMPTY_MAC, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_SETREMOTEDEVICETYPE_B
     * @tc.name cn_setRemoteDeviceType_b
     * @tc.desc Verify Bluetooth API cn setRemoteDeviceType boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_setRemoteDeviceType_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        connection.setRemoteDeviceType(BAD_MAC, -1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_SETREMOTEDEVICETYPE_R
     * @tc.name cn_setRemoteDeviceType_r
     * @tc.desc Verify Bluetooth API cn setRemoteDeviceType return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_setRemoteDeviceType_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        connection.setRemoteDeviceType(BADDR, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_SETREMOTEDEVICETYPE_X
     * @tc.name cn_setRemoteDeviceType_x
     * @tc.desc Verify Bluetooth API cn setRemoteDeviceType repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_setRemoteDeviceType_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        connection.setRemoteDeviceType(BADDR, 0);
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
        connection.setRemoteDeviceType(BADDR, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_SETREMOTEDEVICETYPE_C
     * @tc.name cn_setRemoteDeviceType_c
     * @tc.desc Verify Bluetooth API cn setRemoteDeviceType condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_setRemoteDeviceType_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        connection.setRemoteDeviceType(BADDR, 1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_SETREMOTEDEVICETYPE_PERF
     * @tc.name cn_setRemoteDeviceType_perf
     * @tc.desc Verify Bluetooth apitest case cn setRemoteDeviceType perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_setRemoteDeviceType_perf', 0, () => {
      runPerf('cn_setRemoteDeviceType_perf', 5000, () => {
        try {
          connection.setRemoteDeviceType(BADDR, 0);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_SETREMOTEDEVICETYPE_STRESS
     * @tc.name cn_setRemoteDeviceType_stress
     * @tc.desc Verify Bluetooth apitest case cn setRemoteDeviceType stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_setRemoteDeviceType_stress', 0, () => {
      runStress('cn_setRemoteDeviceType_stress', 20, () => {
        try {
          connection.setRemoteDeviceType(BADDR, 0);
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_getRemoteDeviceTransport', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICETRANSPORT_N
     * @tc.name cn_getRemoteDeviceTransport_n
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceTransport normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceTransport_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = connection.getRemoteDeviceTransport(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICETRANSPORT_E
     * @tc.name cn_getRemoteDeviceTransport_e
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceTransport error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceTransport_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        connection.getRemoteDeviceTransport(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICETRANSPORT_B
     * @tc.name cn_getRemoteDeviceTransport_b
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceTransport boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceTransport_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        connection.getRemoteDeviceTransport(BAD_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICETRANSPORT_R
     * @tc.name cn_getRemoteDeviceTransport_r
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceTransport return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceTransport_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = connection.getRemoteDeviceTransport(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICETRANSPORT_X
     * @tc.name cn_getRemoteDeviceTransport_x
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceTransport repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceTransport_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = connection.getRemoteDeviceTransport(BADDR) as Object;
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
        value = connection.getRemoteDeviceTransport(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICETRANSPORT_C
     * @tc.name cn_getRemoteDeviceTransport_c
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceTransport condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceTransport_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = connection.getRemoteDeviceTransport(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICETRANSPORT_PERF
     * @tc.name cn_getRemoteDeviceTransport_perf
     * @tc.desc Verify Bluetooth apitest case cn getRemoteDeviceTransport perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceTransport_perf', 0, () => {
      runPerf('cn_getRemoteDeviceTransport_perf', 5000, () => {
        try {
          connection.getRemoteDeviceTransport(BADDR) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICETRANSPORT_STRESS
     * @tc.name cn_getRemoteDeviceTransport_stress
     * @tc.desc Verify Bluetooth apitest case cn getRemoteDeviceTransport stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceTransport_stress', 0, () => {
      runStress('cn_getRemoteDeviceTransport_stress', 20, () => {
        try {
          connection.getRemoteDeviceTransport(BADDR) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_getRemoteDeviceBatteryInfo', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICEBATTERYINFO_N
     * @tc.name cn_getRemoteDeviceBatteryInfo_n
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceBatteryInfo normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceBatteryInfo_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = connection.getRemoteDeviceBatteryInfo(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICEBATTERYINFO_E
     * @tc.name cn_getRemoteDeviceBatteryInfo_e
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceBatteryInfo error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceBatteryInfo_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        connection.getRemoteDeviceBatteryInfo(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICEBATTERYINFO_B
     * @tc.name cn_getRemoteDeviceBatteryInfo_b
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceBatteryInfo boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceBatteryInfo_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        connection.getRemoteDeviceBatteryInfo(BAD_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICEBATTERYINFO_R
     * @tc.name cn_getRemoteDeviceBatteryInfo_r
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceBatteryInfo return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceBatteryInfo_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = connection.getRemoteDeviceBatteryInfo(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICEBATTERYINFO_X
     * @tc.name cn_getRemoteDeviceBatteryInfo_x
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceBatteryInfo repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceBatteryInfo_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = connection.getRemoteDeviceBatteryInfo(BADDR) as Object;
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
        value = connection.getRemoteDeviceBatteryInfo(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICEBATTERYINFO_C
     * @tc.name cn_getRemoteDeviceBatteryInfo_c
     * @tc.desc Verify Bluetooth API cn getRemoteDeviceBatteryInfo condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceBatteryInfo_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = connection.getRemoteDeviceBatteryInfo(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICEBATTERYINFO_PERF
     * @tc.name cn_getRemoteDeviceBatteryInfo_perf
     * @tc.desc Verify Bluetooth apitest case cn getRemoteDeviceBatteryInfo perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceBatteryInfo_perf', 0, () => {
      runPerf('cn_getRemoteDeviceBatteryInfo_perf', 5000, () => {
        try {
          connection.getRemoteDeviceBatteryInfo(BADDR) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEDEVICEBATTERYINFO_STRESS
     * @tc.name cn_getRemoteDeviceBatteryInfo_stress
     * @tc.desc Verify Bluetooth apitest case cn getRemoteDeviceBatteryInfo stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteDeviceBatteryInfo_stress', 0, () => {
      runStress('cn_getRemoteDeviceBatteryInfo_stress', 20, () => {
        try {
          connection.getRemoteDeviceBatteryInfo(BADDR) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_getRemoteProfileUuids', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEPROFILEUUIDS_N
     * @tc.name cn_getRemoteProfileUuids_n
     * @tc.desc Verify Bluetooth API cn getRemoteProfileUuids normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteProfileUuids_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = connection.getRemoteProfileUuids(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEPROFILEUUIDS_E
     * @tc.name cn_getRemoteProfileUuids_e
     * @tc.desc Verify Bluetooth API cn getRemoteProfileUuids error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteProfileUuids_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        connection.getRemoteProfileUuids(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEPROFILEUUIDS_B
     * @tc.name cn_getRemoteProfileUuids_b
     * @tc.desc Verify Bluetooth API cn getRemoteProfileUuids boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteProfileUuids_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        connection.getRemoteProfileUuids(BAD_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEPROFILEUUIDS_R
     * @tc.name cn_getRemoteProfileUuids_r
     * @tc.desc Verify Bluetooth API cn getRemoteProfileUuids return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteProfileUuids_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = connection.getRemoteProfileUuids(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEPROFILEUUIDS_X
     * @tc.name cn_getRemoteProfileUuids_x
     * @tc.desc Verify Bluetooth API cn getRemoteProfileUuids repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteProfileUuids_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = connection.getRemoteProfileUuids(BADDR) as Object;
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
        value = connection.getRemoteProfileUuids(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEPROFILEUUIDS_C
     * @tc.name cn_getRemoteProfileUuids_c
     * @tc.desc Verify Bluetooth API cn getRemoteProfileUuids condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteProfileUuids_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = connection.getRemoteProfileUuids(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEPROFILEUUIDS_PERF
     * @tc.name cn_getRemoteProfileUuids_perf
     * @tc.desc Verify Bluetooth apitest case cn getRemoteProfileUuids perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteProfileUuids_perf', 0, () => {
      runPerf('cn_getRemoteProfileUuids_perf', 5000, () => {
        try {
          connection.getRemoteProfileUuids(BADDR) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETREMOTEPROFILEUUIDS_STRESS
     * @tc.name cn_getRemoteProfileUuids_stress
     * @tc.desc Verify Bluetooth apitest case cn getRemoteProfileUuids stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getRemoteProfileUuids_stress', 0, () => {
      runStress('cn_getRemoteProfileUuids_stress', 20, () => {
        try {
          connection.getRemoteProfileUuids(BADDR) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('cn_getLocalProfileUuids', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETLOCALPROFILEUUIDS_N
     * @tc.name cn_getLocalProfileUuids_n
     * @tc.desc Verify Bluetooth API cn getLocalProfileUuids normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getLocalProfileUuids_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = connection.getLocalProfileUuids() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETLOCALPROFILEUUIDS_E
     * @tc.name cn_getLocalProfileUuids_e
     * @tc.desc Verify Bluetooth API cn getLocalProfileUuids error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getLocalProfileUuids_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        connection.getLocalProfileUuids();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETLOCALPROFILEUUIDS_B
     * @tc.name cn_getLocalProfileUuids_b
     * @tc.desc Verify Bluetooth API cn getLocalProfileUuids boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getLocalProfileUuids_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        connection.getLocalProfileUuids();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETLOCALPROFILEUUIDS_R
     * @tc.name cn_getLocalProfileUuids_r
     * @tc.desc Verify Bluetooth API cn getLocalProfileUuids return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getLocalProfileUuids_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = connection.getLocalProfileUuids() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETLOCALPROFILEUUIDS_X
     * @tc.name cn_getLocalProfileUuids_x
     * @tc.desc Verify Bluetooth API cn getLocalProfileUuids repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getLocalProfileUuids_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = connection.getLocalProfileUuids() as Object;
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
        value = connection.getLocalProfileUuids() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETLOCALPROFILEUUIDS_C
     * @tc.name cn_getLocalProfileUuids_c
     * @tc.desc Verify Bluetooth API cn getLocalProfileUuids condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getLocalProfileUuids_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = connection.getLocalProfileUuids() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETLOCALPROFILEUUIDS_PERF
     * @tc.name cn_getLocalProfileUuids_perf
     * @tc.desc Verify Bluetooth apitest case cn getLocalProfileUuids perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getLocalProfileUuids_perf', 0, () => {
      runPerf('cn_getLocalProfileUuids_perf', 5000, () => {
        try {
          connection.getLocalProfileUuids() as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_GETLOCALPROFILEUUIDS_STRESS
     * @tc.name cn_getLocalProfileUuids_stress
     * @tc.desc Verify Bluetooth apitest case cn getLocalProfileUuids stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_getLocalProfileUuids_stress', 0, () => {
      runStress('cn_getLocalProfileUuids_stress', 20, () => {
        try {
          connection.getLocalProfileUuids() as Object;
        } catch (_e) {
        }
      });
    });
    });
  });
}


