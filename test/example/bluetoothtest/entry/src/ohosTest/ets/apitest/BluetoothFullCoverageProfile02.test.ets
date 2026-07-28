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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETREPORT_B
     * @tc.name hiddev_offSetReport_b
     * @tc.desc Verify Bluetooth API hiddev offSetReport boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetReport_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offSetReport();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETREPORT_R
     * @tc.name hiddev_offSetReport_r
     * @tc.desc Verify Bluetooth API hiddev offSetReport return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetReport_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offSetReport();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETREPORT_X
     * @tc.name hiddev_offSetReport_x
     * @tc.desc Verify Bluetooth API hiddev offSetReport repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetReport_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offSetReport();
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
        const _p = hid.createHidDeviceProfile();
        _p.offSetReport();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETREPORT_C
     * @tc.name hiddev_offSetReport_c
     * @tc.desc Verify Bluetooth API hiddev offSetReport condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetReport_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offSetReport();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETREPORT_PERF
     * @tc.name hiddev_offSetReport_perf
     * @tc.desc Verify Bluetooth apitest case hiddev offSetReport perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetReport_perf', 0, () => {
      runPerf('hiddev_offSetReport_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.offSetReport();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETREPORT_STRESS
     * @tc.name hiddev_offSetReport_stress
     * @tc.desc Verify Bluetooth apitest case hiddev offSetReport stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetReport_stress', 0, () => {
      runStress('hiddev_offSetReport_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.offSetReport();
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_onInterruptDataReceived', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONINTERRUPTDATARECEIVED_N
     * @tc.name hiddev_onInterruptDataReceived_n
     * @tc.desc Verify Bluetooth API hiddev onInterruptDataReceived normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onInterruptDataReceived_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onInterruptDataReceived(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONINTERRUPTDATARECEIVED_E
     * @tc.name hiddev_onInterruptDataReceived_e
     * @tc.desc Verify Bluetooth API hiddev onInterruptDataReceived error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onInterruptDataReceived_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onInterruptDataReceived(null);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONINTERRUPTDATARECEIVED_B
     * @tc.name hiddev_onInterruptDataReceived_b
     * @tc.desc Verify Bluetooth API hiddev onInterruptDataReceived boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onInterruptDataReceived_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onInterruptDataReceived(null);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONINTERRUPTDATARECEIVED_R
     * @tc.name hiddev_onInterruptDataReceived_r
     * @tc.desc Verify Bluetooth API hiddev onInterruptDataReceived return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onInterruptDataReceived_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onInterruptDataReceived(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONINTERRUPTDATARECEIVED_X
     * @tc.name hiddev_onInterruptDataReceived_x
     * @tc.desc Verify Bluetooth API hiddev onInterruptDataReceived repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onInterruptDataReceived_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onInterruptDataReceived(() => {});
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
        const _p = hid.createHidDeviceProfile();
        _p.onInterruptDataReceived(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONINTERRUPTDATARECEIVED_C
     * @tc.name hiddev_onInterruptDataReceived_c
     * @tc.desc Verify Bluetooth API hiddev onInterruptDataReceived condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onInterruptDataReceived_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onInterruptDataReceived(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONINTERRUPTDATARECEIVED_PERF
     * @tc.name hiddev_onInterruptDataReceived_perf
     * @tc.desc Verify Bluetooth apitest case hiddev onInterruptDataReceived perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onInterruptDataReceived_perf', 0, () => {
      runPerf('hiddev_onInterruptDataReceived_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.onInterruptDataReceived(() => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONINTERRUPTDATARECEIVED_STRESS
     * @tc.name hiddev_onInterruptDataReceived_stress
     * @tc.desc Verify Bluetooth apitest case hiddev onInterruptDataReceived stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onInterruptDataReceived_stress', 0, () => {
      runStress('hiddev_onInterruptDataReceived_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.onInterruptDataReceived(() => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_offInterruptDataReceived', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFINTERRUPTDATARECEIVED_N
     * @tc.name hiddev_offInterruptDataReceived_n
     * @tc.desc Verify Bluetooth API hiddev offInterruptDataReceived normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offInterruptDataReceived_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offInterruptDataReceived();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFINTERRUPTDATARECEIVED_E
     * @tc.name hiddev_offInterruptDataReceived_e
     * @tc.desc Verify Bluetooth API hiddev offInterruptDataReceived error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offInterruptDataReceived_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offInterruptDataReceived();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFINTERRUPTDATARECEIVED_B
     * @tc.name hiddev_offInterruptDataReceived_b
     * @tc.desc Verify Bluetooth API hiddev offInterruptDataReceived boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offInterruptDataReceived_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offInterruptDataReceived();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFINTERRUPTDATARECEIVED_R
     * @tc.name hiddev_offInterruptDataReceived_r
     * @tc.desc Verify Bluetooth API hiddev offInterruptDataReceived return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offInterruptDataReceived_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offInterruptDataReceived();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFINTERRUPTDATARECEIVED_X
     * @tc.name hiddev_offInterruptDataReceived_x
     * @tc.desc Verify Bluetooth API hiddev offInterruptDataReceived repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offInterruptDataReceived_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offInterruptDataReceived();
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
        const _p = hid.createHidDeviceProfile();
        _p.offInterruptDataReceived();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFINTERRUPTDATARECEIVED_C
     * @tc.name hiddev_offInterruptDataReceived_c
     * @tc.desc Verify Bluetooth API hiddev offInterruptDataReceived condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offInterruptDataReceived_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offInterruptDataReceived();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFINTERRUPTDATARECEIVED_PERF
     * @tc.name hiddev_offInterruptDataReceived_perf
     * @tc.desc Verify Bluetooth apitest case hiddev offInterruptDataReceived perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offInterruptDataReceived_perf', 0, () => {
      runPerf('hiddev_offInterruptDataReceived_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.offInterruptDataReceived();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFINTERRUPTDATARECEIVED_STRESS
     * @tc.name hiddev_offInterruptDataReceived_stress
     * @tc.desc Verify Bluetooth apitest case hiddev offInterruptDataReceived stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offInterruptDataReceived_stress', 0, () => {
      runStress('hiddev_offInterruptDataReceived_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.offInterruptDataReceived();
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_onSetProtocol', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETPROTOCOL_N
     * @tc.name hiddev_onSetProtocol_n
     * @tc.desc Verify Bluetooth API hiddev onSetProtocol normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetProtocol_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onSetProtocol(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETPROTOCOL_E
     * @tc.name hiddev_onSetProtocol_e
     * @tc.desc Verify Bluetooth API hiddev onSetProtocol error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetProtocol_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onSetProtocol(null);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETPROTOCOL_B
     * @tc.name hiddev_onSetProtocol_b
     * @tc.desc Verify Bluetooth API hiddev onSetProtocol boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetProtocol_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onSetProtocol(null);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETPROTOCOL_R
     * @tc.name hiddev_onSetProtocol_r
     * @tc.desc Verify Bluetooth API hiddev onSetProtocol return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetProtocol_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onSetProtocol(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETPROTOCOL_X
     * @tc.name hiddev_onSetProtocol_x
     * @tc.desc Verify Bluetooth API hiddev onSetProtocol repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetProtocol_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onSetProtocol(() => {});
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
        const _p = hid.createHidDeviceProfile();
        _p.onSetProtocol(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETPROTOCOL_C
     * @tc.name hiddev_onSetProtocol_c
     * @tc.desc Verify Bluetooth API hiddev onSetProtocol condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetProtocol_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onSetProtocol(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETPROTOCOL_PERF
     * @tc.name hiddev_onSetProtocol_perf
     * @tc.desc Verify Bluetooth apitest case hiddev onSetProtocol perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetProtocol_perf', 0, () => {
      runPerf('hiddev_onSetProtocol_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.onSetProtocol(() => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETPROTOCOL_STRESS
     * @tc.name hiddev_onSetProtocol_stress
     * @tc.desc Verify Bluetooth apitest case hiddev onSetProtocol stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetProtocol_stress', 0, () => {
      runStress('hiddev_onSetProtocol_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.onSetProtocol(() => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_offSetProtocol', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETPROTOCOL_N
     * @tc.name hiddev_offSetProtocol_n
     * @tc.desc Verify Bluetooth API hiddev offSetProtocol normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetProtocol_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offSetProtocol();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETPROTOCOL_E
     * @tc.name hiddev_offSetProtocol_e
     * @tc.desc Verify Bluetooth API hiddev offSetProtocol error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetProtocol_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offSetProtocol();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETPROTOCOL_B
     * @tc.name hiddev_offSetProtocol_b
     * @tc.desc Verify Bluetooth API hiddev offSetProtocol boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetProtocol_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offSetProtocol();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETPROTOCOL_R
     * @tc.name hiddev_offSetProtocol_r
     * @tc.desc Verify Bluetooth API hiddev offSetProtocol return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetProtocol_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offSetProtocol();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETPROTOCOL_X
     * @tc.name hiddev_offSetProtocol_x
     * @tc.desc Verify Bluetooth API hiddev offSetProtocol repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetProtocol_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offSetProtocol();
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
        const _p = hid.createHidDeviceProfile();
        _p.offSetProtocol();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETPROTOCOL_C
     * @tc.name hiddev_offSetProtocol_c
     * @tc.desc Verify Bluetooth API hiddev offSetProtocol condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetProtocol_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offSetProtocol();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETPROTOCOL_PERF
     * @tc.name hiddev_offSetProtocol_perf
     * @tc.desc Verify Bluetooth apitest case hiddev offSetProtocol perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetProtocol_perf', 0, () => {
      runPerf('hiddev_offSetProtocol_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.offSetProtocol();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETPROTOCOL_STRESS
     * @tc.name hiddev_offSetProtocol_stress
     * @tc.desc Verify Bluetooth apitest case hiddev offSetProtocol stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetProtocol_stress', 0, () => {
      runStress('hiddev_offSetProtocol_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.offSetProtocol();
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_onVirtualCableUnplug', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONVIRTUALCABLEUNPLUG_N
     * @tc.name hiddev_onVirtualCableUnplug_n
     * @tc.desc Verify Bluetooth API hiddev onVirtualCableUnplug normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onVirtualCableUnplug_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onVirtualCableUnplug(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONVIRTUALCABLEUNPLUG_E
     * @tc.name hiddev_onVirtualCableUnplug_e
     * @tc.desc Verify Bluetooth API hiddev onVirtualCableUnplug error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onVirtualCableUnplug_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onVirtualCableUnplug(null);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONVIRTUALCABLEUNPLUG_B
     * @tc.name hiddev_onVirtualCableUnplug_b
     * @tc.desc Verify Bluetooth API hiddev onVirtualCableUnplug boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onVirtualCableUnplug_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onVirtualCableUnplug(null);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONVIRTUALCABLEUNPLUG_R
     * @tc.name hiddev_onVirtualCableUnplug_r
     * @tc.desc Verify Bluetooth API hiddev onVirtualCableUnplug return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onVirtualCableUnplug_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onVirtualCableUnplug(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONVIRTUALCABLEUNPLUG_X
     * @tc.name hiddev_onVirtualCableUnplug_x
     * @tc.desc Verify Bluetooth API hiddev onVirtualCableUnplug repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onVirtualCableUnplug_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onVirtualCableUnplug(() => {});
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
        const _p = hid.createHidDeviceProfile();
        _p.onVirtualCableUnplug(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONVIRTUALCABLEUNPLUG_C
     * @tc.name hiddev_onVirtualCableUnplug_c
     * @tc.desc Verify Bluetooth API hiddev onVirtualCableUnplug condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onVirtualCableUnplug_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onVirtualCableUnplug(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONVIRTUALCABLEUNPLUG_PERF
     * @tc.name hiddev_onVirtualCableUnplug_perf
     * @tc.desc Verify Bluetooth apitest case hiddev onVirtualCableUnplug perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onVirtualCableUnplug_perf', 0, () => {
      runPerf('hiddev_onVirtualCableUnplug_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.onVirtualCableUnplug(() => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONVIRTUALCABLEUNPLUG_STRESS
     * @tc.name hiddev_onVirtualCableUnplug_stress
     * @tc.desc Verify Bluetooth apitest case hiddev onVirtualCableUnplug stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onVirtualCableUnplug_stress', 0, () => {
      runStress('hiddev_onVirtualCableUnplug_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.onVirtualCableUnplug(() => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_offVirtualCableUnplug', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFVIRTUALCABLEUNPLUG_N
     * @tc.name hiddev_offVirtualCableUnplug_n
     * @tc.desc Verify Bluetooth API hiddev offVirtualCableUnplug normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offVirtualCableUnplug_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offVirtualCableUnplug();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFVIRTUALCABLEUNPLUG_E
     * @tc.name hiddev_offVirtualCableUnplug_e
     * @tc.desc Verify Bluetooth API hiddev offVirtualCableUnplug error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offVirtualCableUnplug_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offVirtualCableUnplug();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFVIRTUALCABLEUNPLUG_B
     * @tc.name hiddev_offVirtualCableUnplug_b
     * @tc.desc Verify Bluetooth API hiddev offVirtualCableUnplug boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offVirtualCableUnplug_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offVirtualCableUnplug();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFVIRTUALCABLEUNPLUG_R
     * @tc.name hiddev_offVirtualCableUnplug_r
     * @tc.desc Verify Bluetooth API hiddev offVirtualCableUnplug return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offVirtualCableUnplug_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offVirtualCableUnplug();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFVIRTUALCABLEUNPLUG_X
     * @tc.name hiddev_offVirtualCableUnplug_x
     * @tc.desc Verify Bluetooth API hiddev offVirtualCableUnplug repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offVirtualCableUnplug_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offVirtualCableUnplug();
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
        const _p = hid.createHidDeviceProfile();
        _p.offVirtualCableUnplug();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFVIRTUALCABLEUNPLUG_C
     * @tc.name hiddev_offVirtualCableUnplug_c
     * @tc.desc Verify Bluetooth API hiddev offVirtualCableUnplug condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offVirtualCableUnplug_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offVirtualCableUnplug();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFVIRTUALCABLEUNPLUG_PERF
     * @tc.name hiddev_offVirtualCableUnplug_perf
     * @tc.desc Verify Bluetooth apitest case hiddev offVirtualCableUnplug perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offVirtualCableUnplug_perf', 0, () => {
      runPerf('hiddev_offVirtualCableUnplug_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.offVirtualCableUnplug();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFVIRTUALCABLEUNPLUG_STRESS
     * @tc.name hiddev_offVirtualCableUnplug_stress
     * @tc.desc Verify Bluetooth apitest case hiddev offVirtualCableUnplug stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offVirtualCableUnplug_stress', 0, () => {
      runStress('hiddev_offVirtualCableUnplug_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.offVirtualCableUnplug();
        } catch (_e) {
        }
      });
    });
    });
  });
}


