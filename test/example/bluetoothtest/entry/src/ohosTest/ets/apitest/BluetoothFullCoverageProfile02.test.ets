     * @tc.level Level 2
     */
    it('prof_opp_on_receiveIncomingFile_stress', 0, () => {
      runStress('prof_opp_on_receiveIncomingFile_stress', 20, () => {
        try {
          const _p = opp.createOppServerProfile();
          _p.on('receiveIncomingFile', () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_opp_off_receiveIncomingFile', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_OPP_OFF_RECEIVEINCOMINGFILE_N
     * @tc.name prof_opp_off_receiveIncomingFile_n
     * @tc.desc Verify Bluetooth API prof opp off receiveIncomingFile normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_opp_off_receiveIncomingFile_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = opp.createOppServerProfile();
        _p.off('receiveIncomingFile');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_OPP_OFF_RECEIVEINCOMINGFILE_E
     * @tc.name prof_opp_off_receiveIncomingFile_e
     * @tc.desc Verify Bluetooth API prof opp off receiveIncomingFile error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_opp_off_receiveIncomingFile_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = opp.createOppServerProfile();
        _p.off('receiveIncomingFile', () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_OPP_OFF_RECEIVEINCOMINGFILE_B
     * @tc.name prof_opp_off_receiveIncomingFile_b
     * @tc.desc Verify Bluetooth API prof opp off receiveIncomingFile boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_opp_off_receiveIncomingFile_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = opp.createOppServerProfile();
        _p.off('receiveIncomingFile', () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_OPP_OFF_RECEIVEINCOMINGFILE_R
     * @tc.name prof_opp_off_receiveIncomingFile_r
     * @tc.desc Verify Bluetooth API prof opp off receiveIncomingFile return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_opp_off_receiveIncomingFile_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = opp.createOppServerProfile();
        _p.off('receiveIncomingFile');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_OPP_OFF_RECEIVEINCOMINGFILE_X
     * @tc.name prof_opp_off_receiveIncomingFile_x
     * @tc.desc Verify Bluetooth API prof opp off receiveIncomingFile repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_opp_off_receiveIncomingFile_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = opp.createOppServerProfile();
        _p.off('receiveIncomingFile');
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
        const _p = opp.createOppServerProfile();
        _p.off('receiveIncomingFile');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_OPP_OFF_RECEIVEINCOMINGFILE_C
     * @tc.name prof_opp_off_receiveIncomingFile_c
     * @tc.desc Verify Bluetooth API prof opp off receiveIncomingFile condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_opp_off_receiveIncomingFile_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = opp.createOppServerProfile();
        _p.off('receiveIncomingFile');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_OPP_OFF_RECEIVEINCOMINGFILE_PERF
     * @tc.name prof_opp_off_receiveIncomingFile_perf
     * @tc.desc Verify Bluetooth apitest case prof opp off receiveIncomingFile perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_opp_off_receiveIncomingFile_perf', 0, () => {
      runPerf('prof_opp_off_receiveIncomingFile_perf', 5000, () => {
        try {
          const _p = opp.createOppServerProfile();
          _p.off('receiveIncomingFile');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_OPP_OFF_RECEIVEINCOMINGFILE_STRESS
     * @tc.name prof_opp_off_receiveIncomingFile_stress
     * @tc.desc Verify Bluetooth apitest case prof opp off receiveIncomingFile stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_opp_off_receiveIncomingFile_stress', 0, () => {
      runStress('prof_opp_off_receiveIncomingFile_stress', 20, () => {
        try {
          const _p = opp.createOppServerProfile();
          _p.off('receiveIncomingFile');
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_base_setConnectionStrategy', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_SETCONNECTIONSTRATEGY_N
     * @tc.name prof_base_setConnectionStrategy_n
     * @tc.desc Verify Bluetooth API prof base setConnectionStrategy normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_setConnectionStrategy_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.setConnectionStrategy(BADDR, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_SETCONNECTIONSTRATEGY_E
     * @tc.name prof_base_setConnectionStrategy_e
     * @tc.desc Verify Bluetooth API prof base setConnectionStrategy error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_setConnectionStrategy_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.setConnectionStrategy(EMPTY_MAC, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_SETCONNECTIONSTRATEGY_B
     * @tc.name prof_base_setConnectionStrategy_b
     * @tc.desc Verify Bluetooth API prof base setConnectionStrategy boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_setConnectionStrategy_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.setConnectionStrategy(EMPTY_MAC, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_SETCONNECTIONSTRATEGY_R
     * @tc.name prof_base_setConnectionStrategy_r
     * @tc.desc Verify Bluetooth API prof base setConnectionStrategy return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_setConnectionStrategy_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.setConnectionStrategy(BADDR, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_SETCONNECTIONSTRATEGY_X
     * @tc.name prof_base_setConnectionStrategy_x
     * @tc.desc Verify Bluetooth API prof base setConnectionStrategy repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_setConnectionStrategy_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.setConnectionStrategy(BADDR, 0);
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
        const _p = a2dp.createA2dpSrcProfile();
        _p.setConnectionStrategy(BADDR, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_SETCONNECTIONSTRATEGY_C
     * @tc.name prof_base_setConnectionStrategy_c
     * @tc.desc Verify Bluetooth API prof base setConnectionStrategy condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_setConnectionStrategy_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.setConnectionStrategy(BADDR, 1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_SETCONNECTIONSTRATEGY_PERF
     * @tc.name prof_base_setConnectionStrategy_perf
     * @tc.desc Verify Bluetooth apitest case prof base setConnectionStrategy perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_setConnectionStrategy_perf', 0, () => {
      runPerf('prof_base_setConnectionStrategy_perf', 5000, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.setConnectionStrategy(BADDR, 0);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_SETCONNECTIONSTRATEGY_STRESS
     * @tc.name prof_base_setConnectionStrategy_stress
     * @tc.desc Verify Bluetooth apitest case prof base setConnectionStrategy stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_setConnectionStrategy_stress', 0, () => {
      runStress('prof_base_setConnectionStrategy_stress', 20, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.setConnectionStrategy(BADDR, 0);
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_base_getConnectionStrategy', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTRATEGY_N
     * @tc.name prof_base_getConnectionStrategy_n
     * @tc.desc Verify Bluetooth API prof base getConnectionStrategy normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionStrategy_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getConnectionStrategy(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTRATEGY_E
     * @tc.name prof_base_getConnectionStrategy_e
     * @tc.desc Verify Bluetooth API prof base getConnectionStrategy error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionStrategy_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.getConnectionStrategy(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTRATEGY_B
     * @tc.name prof_base_getConnectionStrategy_b
     * @tc.desc Verify Bluetooth API prof base getConnectionStrategy boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionStrategy_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.getConnectionStrategy(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTRATEGY_R
     * @tc.name prof_base_getConnectionStrategy_r
     * @tc.desc Verify Bluetooth API prof base getConnectionStrategy return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionStrategy_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getConnectionStrategy(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTRATEGY_X
     * @tc.name prof_base_getConnectionStrategy_x
     * @tc.desc Verify Bluetooth API prof base getConnectionStrategy repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionStrategy_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getConnectionStrategy(BADDR) as Object;
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
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getConnectionStrategy(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTRATEGY_C
     * @tc.name prof_base_getConnectionStrategy_c
     * @tc.desc Verify Bluetooth API prof base getConnectionStrategy condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionStrategy_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getConnectionStrategy(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTRATEGY_PERF
     * @tc.name prof_base_getConnectionStrategy_perf
     * @tc.desc Verify Bluetooth apitest case prof base getConnectionStrategy perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionStrategy_perf', 0, () => {
      runPerf('prof_base_getConnectionStrategy_perf', 5000, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.getConnectionStrategy(BADDR) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTRATEGY_STRESS
     * @tc.name prof_base_getConnectionStrategy_stress
     * @tc.desc Verify Bluetooth apitest case prof base getConnectionStrategy stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionStrategy_stress', 0, () => {
      runStress('prof_base_getConnectionStrategy_stress', 20, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.getConnectionStrategy(BADDR) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_base_getConnectedDevices', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTEDDEVICES_N
     * @tc.name prof_base_getConnectedDevices_n
     * @tc.desc Verify Bluetooth API prof base getConnectedDevices normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectedDevices_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getConnectedDevices() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTEDDEVICES_E
     * @tc.name prof_base_getConnectedDevices_e
     * @tc.desc Verify Bluetooth API prof base getConnectedDevices error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectedDevices_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.getConnectedDevices();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTEDDEVICES_B
     * @tc.name prof_base_getConnectedDevices_b
     * @tc.desc Verify Bluetooth API prof base getConnectedDevices boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectedDevices_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.getConnectedDevices();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTEDDEVICES_R
     * @tc.name prof_base_getConnectedDevices_r
     * @tc.desc Verify Bluetooth API prof base getConnectedDevices return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectedDevices_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getConnectedDevices() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTEDDEVICES_X
     * @tc.name prof_base_getConnectedDevices_x
     * @tc.desc Verify Bluetooth API prof base getConnectedDevices repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectedDevices_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getConnectedDevices() as Object;
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
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getConnectedDevices() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTEDDEVICES_C
     * @tc.name prof_base_getConnectedDevices_c
     * @tc.desc Verify Bluetooth API prof base getConnectedDevices condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectedDevices_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getConnectedDevices() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTEDDEVICES_PERF
     * @tc.name prof_base_getConnectedDevices_perf
     * @tc.desc Verify Bluetooth apitest case prof base getConnectedDevices perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectedDevices_perf', 0, () => {
      runPerf('prof_base_getConnectedDevices_perf', 5000, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.getConnectedDevices() as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTEDDEVICES_STRESS
     * @tc.name prof_base_getConnectedDevices_stress
     * @tc.desc Verify Bluetooth apitest case prof base getConnectedDevices stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectedDevices_stress', 0, () => {
      runStress('prof_base_getConnectedDevices_stress', 20, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.getConnectedDevices() as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_base_getConnectionState', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTATE_N
     * @tc.name prof_base_getConnectionState_n
     * @tc.desc Verify Bluetooth API prof base getConnectionState normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionState_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getConnectionState(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTATE_E
     * @tc.name prof_base_getConnectionState_e
     * @tc.desc Verify Bluetooth API prof base getConnectionState error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionState_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.getConnectionState(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTATE_B
     * @tc.name prof_base_getConnectionState_b
     * @tc.desc Verify Bluetooth API prof base getConnectionState boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionState_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.getConnectionState(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTATE_R
     * @tc.name prof_base_getConnectionState_r
     * @tc.desc Verify Bluetooth API prof base getConnectionState return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionState_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getConnectionState(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTATE_X
     * @tc.name prof_base_getConnectionState_x
     * @tc.desc Verify Bluetooth API prof base getConnectionState repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionState_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getConnectionState(BADDR) as Object;
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
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getConnectionState(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTATE_C
     * @tc.name prof_base_getConnectionState_c
     * @tc.desc Verify Bluetooth API prof base getConnectionState condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionState_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getConnectionState(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTATE_PERF
     * @tc.name prof_base_getConnectionState_perf
     * @tc.desc Verify Bluetooth apitest case prof base getConnectionState perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionState_perf', 0, () => {
      runPerf('prof_base_getConnectionState_perf', 5000, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.getConnectionState(BADDR) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_GETCONNECTIONSTATE_STRESS
     * @tc.name prof_base_getConnectionState_stress
     * @tc.desc Verify Bluetooth apitest case prof base getConnectionState stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_getConnectionState_stress', 0, () => {
      runStress('prof_base_getConnectionState_stress', 20, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.getConnectionState(BADDR) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_base_on_connectionStateChange', () => {
    it('prof_base_on_connectionStateChange_n', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.on('connectionStateChange', () => {});
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
    it('prof_base_on_connectionStateChange_e', 0, async (done: Function) => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.on('connectionStateChange', null);
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
      waitCallback(done, 200);
    });
    it('prof_base_on_connectionStateChange_b', 0, async (done: Function) => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.on('connectionStateChange', null);
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
      waitCallback(done, 200);
    });
    it('prof_base_on_connectionStateChange_r', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.on('connectionStateChange', () => {});
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
    it('prof_base_on_connectionStateChange_x', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.on('connectionStateChange', () => {});
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
        const _p = a2dp.createA2dpSrcProfile();
        _p.on('connectionStateChange', () => {});
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
    it('prof_base_on_connectionStateChange_c', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.on('connectionStateChange', () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_ON_CONNECTIONSTATECHANGE_PERF
     * @tc.name prof_base_on_connectionStateChange_perf
     * @tc.desc Verify Bluetooth apitest case prof base on connectionStateChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_on_connectionStateChange_perf', 0, () => {
      runPerf('prof_base_on_connectionStateChange_perf', 5000, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.on('connectionStateChange', () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_ON_CONNECTIONSTATECHANGE_STRESS
     * @tc.name prof_base_on_connectionStateChange_stress
     * @tc.desc Verify Bluetooth apitest case prof base on connectionStateChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_on_connectionStateChange_stress', 0, () => {
      runStress('prof_base_on_connectionStateChange_stress', 20, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.on('connectionStateChange', () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_base_off_connectionStateChange', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_OFF_CONNECTIONSTATECHANGE_N
     * @tc.name prof_base_off_connectionStateChange_n
     * @tc.desc Verify Bluetooth API prof base off connectionStateChange normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_off_connectionStateChange_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.off('connectionStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_OFF_CONNECTIONSTATECHANGE_E
     * @tc.name prof_base_off_connectionStateChange_e
     * @tc.desc Verify Bluetooth API prof base off connectionStateChange error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_off_connectionStateChange_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.off('connectionStateChange', () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_OFF_CONNECTIONSTATECHANGE_B
     * @tc.name prof_base_off_connectionStateChange_b
     * @tc.desc Verify Bluetooth API prof base off connectionStateChange boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_off_connectionStateChange_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.off('connectionStateChange', () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_OFF_CONNECTIONSTATECHANGE_R
     * @tc.name prof_base_off_connectionStateChange_r
     * @tc.desc Verify Bluetooth API prof base off connectionStateChange return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_off_connectionStateChange_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.off('connectionStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_OFF_CONNECTIONSTATECHANGE_X
     * @tc.name prof_base_off_connectionStateChange_x
     * @tc.desc Verify Bluetooth API prof base off connectionStateChange repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_off_connectionStateChange_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.off('connectionStateChange');
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
        const _p = a2dp.createA2dpSrcProfile();
        _p.off('connectionStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_OFF_CONNECTIONSTATECHANGE_C
     * @tc.name prof_base_off_connectionStateChange_c
     * @tc.desc Verify Bluetooth API prof base off connectionStateChange condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_off_connectionStateChange_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.off('connectionStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_OFF_CONNECTIONSTATECHANGE_PERF
     * @tc.name prof_base_off_connectionStateChange_perf
     * @tc.desc Verify Bluetooth apitest case prof base off connectionStateChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_off_connectionStateChange_perf', 0, () => {
      runPerf('prof_base_off_connectionStateChange_perf', 5000, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.off('connectionStateChange');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_BASE_OFF_CONNECTIONSTATECHANGE_STRESS
     * @tc.name prof_base_off_connectionStateChange_stress
     * @tc.desc Verify Bluetooth apitest case prof base off connectionStateChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_base_off_connectionStateChange_stress', 0, () => {
      runStress('prof_base_off_connectionStateChange_stress', 20, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.off('connectionStateChange');
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_createHidDeviceProfile', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CREATEHIDDEVICEPROFILE_N
     * @tc.name hiddev_createHidDeviceProfile_n
     * @tc.desc Verify Bluetooth API hiddev createHidDeviceProfile normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_createHidDeviceProfile_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = hid.createHidDeviceProfile() as Object;
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertLocalOk(errCode);
      if (errCode === 0) {
        expect(value !== null && value !== undefined).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CREATEHIDDEVICEPROFILE_E
     * @tc.name hiddev_createHidDeviceProfile_e
     * @tc.desc Verify Bluetooth API hiddev createHidDeviceProfile error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_createHidDeviceProfile_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        hid.createHidDeviceProfile();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CREATEHIDDEVICEPROFILE_B
     * @tc.name hiddev_createHidDeviceProfile_b
     * @tc.desc Verify Bluetooth API hiddev createHidDeviceProfile boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_createHidDeviceProfile_b', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        value = hid.createHidDeviceProfile() as Object;
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertLocalOk(errCode);
      if (errCode === 0) {
        expect(value !== null && value !== undefined).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CREATEHIDDEVICEPROFILE_R
     * @tc.name hiddev_createHidDeviceProfile_r
     * @tc.desc Verify Bluetooth API hiddev createHidDeviceProfile return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_createHidDeviceProfile_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = hid.createHidDeviceProfile() as Object;
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertLocalOk(errCode);
      if (errCode === 0) {
        expect(value !== null && value !== undefined).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CREATEHIDDEVICEPROFILE_X
     * @tc.name hiddev_createHidDeviceProfile_x
     * @tc.desc Verify Bluetooth API hiddev createHidDeviceProfile repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_createHidDeviceProfile_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = hid.createHidDeviceProfile() as Object;
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertLocalOk(errCode);
      if (errCode === 0) {
        expect(value !== null && value !== undefined).assertTrue();
      }
      errCode = 0;
      value = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        value = hid.createHidDeviceProfile() as Object;
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertLocalOk(errCode);
      if (errCode === 0) {
        expect(value !== null && value !== undefined).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CREATEHIDDEVICEPROFILE_C
     * @tc.name hiddev_createHidDeviceProfile_c
     * @tc.desc Verify Bluetooth API hiddev createHidDeviceProfile condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_createHidDeviceProfile_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = hid.createHidDeviceProfile() as Object;
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertLocalOk(errCode);
      if (errCode === 0) {
        expect(value !== null && value !== undefined).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CREATEHIDDEVICEPROFILE_PERF
     * @tc.name hiddev_createHidDeviceProfile_perf
     * @tc.desc Verify Bluetooth apitest case hiddev createHidDeviceProfile perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_createHidDeviceProfile_perf', 0, () => {
      runPerf('hiddev_createHidDeviceProfile_perf', 5000, () => {
        try {
          hid.createHidDeviceProfile() as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CREATEHIDDEVICEPROFILE_STRESS
     * @tc.name hiddev_createHidDeviceProfile_stress
     * @tc.desc Verify Bluetooth apitest case hiddev createHidDeviceProfile stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_createHidDeviceProfile_stress', 0, () => {
      runStress('hiddev_createHidDeviceProfile_stress', 20, () => {
        try {
          hid.createHidDeviceProfile() as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_register', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REGISTER_N
     * @tc.name hiddev_register_n
     * @tc.desc Verify Bluetooth API hiddev register normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_register_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.registerHidDevice(hidSdpStub() as hid.HidDeviceSdp, hidQosStub() as hid.HidDeviceQos,
          hidQosStub() as hid.HidDeviceQos, () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REGISTER_E
     * @tc.name hiddev_register_e
     * @tc.desc Verify Bluetooth API hiddev register error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_register_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.registerHidDevice({} as hid.HidDeviceSdp, hidQosStub() as hid.HidDeviceQos,
          hidQosStub() as hid.HidDeviceQos, null);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REGISTER_B
     * @tc.name hiddev_register_b
     * @tc.desc Verify Bluetooth API hiddev register boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_register_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.registerHidDevice({} as hid.HidDeviceSdp, hidQosStub() as hid.HidDeviceQos,
          hidQosStub() as hid.HidDeviceQos, null);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REGISTER_R
     * @tc.name hiddev_register_r
     * @tc.desc Verify Bluetooth API hiddev register return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_register_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.registerHidDevice(hidSdpStub() as hid.HidDeviceSdp, hidQosStub() as hid.HidDeviceQos,
          hidQosStub() as hid.HidDeviceQos, () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REGISTER_X
     * @tc.name hiddev_register_x
     * @tc.desc Verify Bluetooth API hiddev register repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_register_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.registerHidDevice(hidSdpStub() as hid.HidDeviceSdp, hidQosStub() as hid.HidDeviceQos,
          hidQosStub() as hid.HidDeviceQos, () => {});
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
        _p.registerHidDevice(hidSdpStub() as hid.HidDeviceSdp, hidQosStub() as hid.HidDeviceQos,
          hidQosStub() as hid.HidDeviceQos, () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REGISTER_C
     * @tc.name hiddev_register_c
     * @tc.desc Verify Bluetooth API hiddev register condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_register_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.registerHidDevice(hidSdpStub() as hid.HidDeviceSdp, hidQosStub() as hid.HidDeviceQos,
          hidQosStub() as hid.HidDeviceQos, () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REGISTER_PERF
     * @tc.name hiddev_register_perf
     * @tc.desc Verify Bluetooth apitest case hiddev register perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_register_perf', 0, () => {
      runPerf('hiddev_register_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.registerHidDevice(hidSdpStub() as hid.HidDeviceSdp, hidQosStub() as hid.HidDeviceQos,
            hidQosStub() as hid.HidDeviceQos, () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REGISTER_STRESS
     * @tc.name hiddev_register_stress
     * @tc.desc Verify Bluetooth apitest case hiddev register stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_register_stress', 0, () => {
      runStress('hiddev_register_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.registerHidDevice(hidSdpStub() as hid.HidDeviceSdp, hidQosStub() as hid.HidDeviceQos,
            hidQosStub() as hid.HidDeviceQos, () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_unregister', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_UNREGISTER_N
     * @tc.name hiddev_unregister_n
     * @tc.desc Verify Bluetooth API hiddev unregister normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_unregister_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.unregisterHidDevice();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_UNREGISTER_E
     * @tc.name hiddev_unregister_e
     * @tc.desc Verify Bluetooth API hiddev unregister error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_unregister_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.unregisterHidDevice();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_UNREGISTER_B
     * @tc.name hiddev_unregister_b
     * @tc.desc Verify Bluetooth API hiddev unregister boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_unregister_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.unregisterHidDevice();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_UNREGISTER_R
     * @tc.name hiddev_unregister_r
     * @tc.desc Verify Bluetooth API hiddev unregister return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_unregister_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.unregisterHidDevice();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_UNREGISTER_X
     * @tc.name hiddev_unregister_x
     * @tc.desc Verify Bluetooth API hiddev unregister repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_unregister_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.unregisterHidDevice();
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
        _p.unregisterHidDevice();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_UNREGISTER_C
     * @tc.name hiddev_unregister_c
     * @tc.desc Verify Bluetooth API hiddev unregister condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_unregister_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.unregisterHidDevice();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_UNREGISTER_PERF
     * @tc.name hiddev_unregister_perf
     * @tc.desc Verify Bluetooth apitest case hiddev unregister perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_unregister_perf', 0, () => {
      runPerf('hiddev_unregister_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.unregisterHidDevice();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_UNREGISTER_STRESS
     * @tc.name hiddev_unregister_stress
     * @tc.desc Verify Bluetooth apitest case hiddev unregister stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_unregister_stress', 0, () => {
      runStress('hiddev_unregister_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.unregisterHidDevice();
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_connect', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CONNECT_N
     * @tc.name hiddev_connect_n
     * @tc.desc Verify Bluetooth API hiddev connect normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_connect_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.connect({address:BADDR, addressType:0});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CONNECT_E
     * @tc.name hiddev_connect_e
     * @tc.desc Verify Bluetooth API hiddev connect error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_connect_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.connect({address:EMPTY_MAC, addressType:0});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CONNECT_B
     * @tc.name hiddev_connect_b
     * @tc.desc Verify Bluetooth API hiddev connect boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_connect_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.connect({address:EMPTY_MAC, addressType:0});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CONNECT_R
     * @tc.name hiddev_connect_r
     * @tc.desc Verify Bluetooth API hiddev connect return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_connect_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.connect({address:BADDR, addressType:0});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CONNECT_X
     * @tc.name hiddev_connect_x
     * @tc.desc Verify Bluetooth API hiddev connect repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_connect_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.connect({address:BADDR, addressType:0});
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
        _p.connect({address:BADDR, addressType:0});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CONNECT_C
     * @tc.name hiddev_connect_c
     * @tc.desc Verify Bluetooth API hiddev connect condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_connect_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.connect({address:BADDR, addressType:0});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CONNECT_PERF
     * @tc.name hiddev_connect_perf
     * @tc.desc Verify Bluetooth apitest case hiddev connect perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_connect_perf', 0, () => {
      runPerf('hiddev_connect_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.connect({address:BADDR, addressType:0});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_CONNECT_STRESS
     * @tc.name hiddev_connect_stress
     * @tc.desc Verify Bluetooth apitest case hiddev connect stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_connect_stress', 0, () => {
      runStress('hiddev_connect_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.connect({address:BADDR, addressType:0});
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_disconnect', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_DISCONNECT_N
     * @tc.name hiddev_disconnect_n
     * @tc.desc Verify Bluetooth API hiddev disconnect normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_disconnect_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.disconnect();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_DISCONNECT_E
     * @tc.name hiddev_disconnect_e
     * @tc.desc Verify Bluetooth API hiddev disconnect error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_disconnect_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.disconnect();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_DISCONNECT_B
     * @tc.name hiddev_disconnect_b
     * @tc.desc Verify Bluetooth API hiddev disconnect boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_disconnect_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.disconnect();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_DISCONNECT_R
     * @tc.name hiddev_disconnect_r
     * @tc.desc Verify Bluetooth API hiddev disconnect return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_disconnect_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.disconnect();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_DISCONNECT_X
     * @tc.name hiddev_disconnect_x
     * @tc.desc Verify Bluetooth API hiddev disconnect repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_disconnect_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.disconnect();
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
        _p.disconnect();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_DISCONNECT_C
     * @tc.name hiddev_disconnect_c
     * @tc.desc Verify Bluetooth API hiddev disconnect condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_disconnect_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.disconnect();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_DISCONNECT_PERF
     * @tc.name hiddev_disconnect_perf
     * @tc.desc Verify Bluetooth apitest case hiddev disconnect perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_disconnect_perf', 0, () => {
      runPerf('hiddev_disconnect_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.disconnect();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_DISCONNECT_STRESS
     * @tc.name hiddev_disconnect_stress
     * @tc.desc Verify Bluetooth apitest case hiddev disconnect stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_disconnect_stress', 0, () => {
      runStress('hiddev_disconnect_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.disconnect();
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_sendReport', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_SENDREPORT_N
     * @tc.name hiddev_sendReport_n
     * @tc.desc Verify Bluetooth API hiddev sendReport normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_sendReport_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.sendReport(1, new Uint8Array([0]));
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_SENDREPORT_E
     * @tc.name hiddev_sendReport_e
     * @tc.desc Verify Bluetooth API hiddev sendReport error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_sendReport_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.sendReport(-1, new Uint8Array([0]));
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_SENDREPORT_B
     * @tc.name hiddev_sendReport_b
     * @tc.desc Verify Bluetooth API hiddev sendReport boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_sendReport_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.sendReport(-1, new Uint8Array([0]));
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_SENDREPORT_R
     * @tc.name hiddev_sendReport_r
     * @tc.desc Verify Bluetooth API hiddev sendReport return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_sendReport_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.sendReport(1, new Uint8Array([0]));
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_SENDREPORT_X
     * @tc.name hiddev_sendReport_x
     * @tc.desc Verify Bluetooth API hiddev sendReport repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_sendReport_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.sendReport(1, new Uint8Array([0]));
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
        _p.sendReport(1, new Uint8Array([0]));
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_SENDREPORT_C
     * @tc.name hiddev_sendReport_c
     * @tc.desc Verify Bluetooth API hiddev sendReport condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_sendReport_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.sendReport(1, new Uint8Array([0]));
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_SENDREPORT_PERF
     * @tc.name hiddev_sendReport_perf
     * @tc.desc Verify Bluetooth apitest case hiddev sendReport perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_sendReport_perf', 0, () => {
      runPerf('hiddev_sendReport_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.sendReport(1, new Uint8Array([0]));
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_SENDREPORT_STRESS
     * @tc.name hiddev_sendReport_stress
     * @tc.desc Verify Bluetooth apitest case hiddev sendReport stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_sendReport_stress', 0, () => {
      runStress('hiddev_sendReport_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.sendReport(1, new Uint8Array([0]));
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_replyReport', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPLYREPORT_N
     * @tc.name hiddev_replyReport_n
     * @tc.desc Verify Bluetooth API hiddev replyReport normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_replyReport_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.replyReport(0, 1, new Uint8Array([0]));
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPLYREPORT_E
     * @tc.name hiddev_replyReport_e
     * @tc.desc Verify Bluetooth API hiddev replyReport error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_replyReport_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.replyReport(-1, 1, new Uint8Array([0]));
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPLYREPORT_B
     * @tc.name hiddev_replyReport_b
     * @tc.desc Verify Bluetooth API hiddev replyReport boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_replyReport_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.replyReport(-1, 1, new Uint8Array([0]));
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPLYREPORT_R
     * @tc.name hiddev_replyReport_r
     * @tc.desc Verify Bluetooth API hiddev replyReport return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_replyReport_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.replyReport(0, 1, new Uint8Array([0]));
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPLYREPORT_X
     * @tc.name hiddev_replyReport_x
     * @tc.desc Verify Bluetooth API hiddev replyReport repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_replyReport_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.replyReport(0, 1, new Uint8Array([0]));
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
        _p.replyReport(0, 1, new Uint8Array([0]));
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPLYREPORT_C
     * @tc.name hiddev_replyReport_c
     * @tc.desc Verify Bluetooth API hiddev replyReport condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_replyReport_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.replyReport(0, 1, new Uint8Array([0]));
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPLYREPORT_PERF
     * @tc.name hiddev_replyReport_perf
     * @tc.desc Verify Bluetooth apitest case hiddev replyReport perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_replyReport_perf', 0, () => {
      runPerf('hiddev_replyReport_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.replyReport(0, 1, new Uint8Array([0]));
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPLYREPORT_STRESS
     * @tc.name hiddev_replyReport_stress
     * @tc.desc Verify Bluetooth apitest case hiddev replyReport stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_replyReport_stress', 0, () => {
      runStress('hiddev_replyReport_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.replyReport(0, 1, new Uint8Array([0]));
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_reportError', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPORTERROR_N
     * @tc.name hiddev_reportError_n
     * @tc.desc Verify Bluetooth API hiddev reportError normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_reportError_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.reportError(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPORTERROR_E
     * @tc.name hiddev_reportError_e
     * @tc.desc Verify Bluetooth API hiddev reportError error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_reportError_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.reportError(-1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPORTERROR_B
     * @tc.name hiddev_reportError_b
     * @tc.desc Verify Bluetooth API hiddev reportError boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_reportError_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.reportError(-1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPORTERROR_R
     * @tc.name hiddev_reportError_r
     * @tc.desc Verify Bluetooth API hiddev reportError return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_reportError_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.reportError(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPORTERROR_X
     * @tc.name hiddev_reportError_x
     * @tc.desc Verify Bluetooth API hiddev reportError repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_reportError_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.reportError(0);
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
        _p.reportError(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPORTERROR_C
     * @tc.name hiddev_reportError_c
     * @tc.desc Verify Bluetooth API hiddev reportError condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_reportError_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.reportError(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPORTERROR_PERF
     * @tc.name hiddev_reportError_perf
     * @tc.desc Verify Bluetooth apitest case hiddev reportError perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_reportError_perf', 0, () => {
      runPerf('hiddev_reportError_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.reportError(0);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_REPORTERROR_STRESS
     * @tc.name hiddev_reportError_stress
     * @tc.desc Verify Bluetooth apitest case hiddev reportError stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_reportError_stress', 0, () => {
      runStress('hiddev_reportError_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.reportError(0);
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_onGetReport', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONGETREPORT_N
     * @tc.name hiddev_onGetReport_n
     * @tc.desc Verify Bluetooth API hiddev onGetReport normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onGetReport_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onGetReport(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONGETREPORT_E
     * @tc.name hiddev_onGetReport_e
     * @tc.desc Verify Bluetooth API hiddev onGetReport error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onGetReport_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onGetReport(null);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONGETREPORT_B
     * @tc.name hiddev_onGetReport_b
     * @tc.desc Verify Bluetooth API hiddev onGetReport boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onGetReport_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onGetReport(null);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONGETREPORT_R
     * @tc.name hiddev_onGetReport_r
     * @tc.desc Verify Bluetooth API hiddev onGetReport return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onGetReport_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onGetReport(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONGETREPORT_X
     * @tc.name hiddev_onGetReport_x
     * @tc.desc Verify Bluetooth API hiddev onGetReport repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onGetReport_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onGetReport(() => {});
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
        _p.onGetReport(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONGETREPORT_C
     * @tc.name hiddev_onGetReport_c
     * @tc.desc Verify Bluetooth API hiddev onGetReport condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onGetReport_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onGetReport(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONGETREPORT_PERF
     * @tc.name hiddev_onGetReport_perf
     * @tc.desc Verify Bluetooth apitest case hiddev onGetReport perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onGetReport_perf', 0, () => {
      runPerf('hiddev_onGetReport_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.onGetReport(() => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONGETREPORT_STRESS
     * @tc.name hiddev_onGetReport_stress
     * @tc.desc Verify Bluetooth apitest case hiddev onGetReport stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onGetReport_stress', 0, () => {
      runStress('hiddev_onGetReport_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.onGetReport(() => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_offGetReport', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFGETREPORT_N
     * @tc.name hiddev_offGetReport_n
     * @tc.desc Verify Bluetooth API hiddev offGetReport normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offGetReport_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offGetReport();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFGETREPORT_E
     * @tc.name hiddev_offGetReport_e
     * @tc.desc Verify Bluetooth API hiddev offGetReport error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offGetReport_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offGetReport();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFGETREPORT_B
     * @tc.name hiddev_offGetReport_b
     * @tc.desc Verify Bluetooth API hiddev offGetReport boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offGetReport_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offGetReport();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFGETREPORT_R
     * @tc.name hiddev_offGetReport_r
     * @tc.desc Verify Bluetooth API hiddev offGetReport return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offGetReport_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offGetReport();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFGETREPORT_X
     * @tc.name hiddev_offGetReport_x
     * @tc.desc Verify Bluetooth API hiddev offGetReport repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offGetReport_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offGetReport();
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
        _p.offGetReport();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFGETREPORT_C
     * @tc.name hiddev_offGetReport_c
     * @tc.desc Verify Bluetooth API hiddev offGetReport condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offGetReport_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offGetReport();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFGETREPORT_PERF
     * @tc.name hiddev_offGetReport_perf
     * @tc.desc Verify Bluetooth apitest case hiddev offGetReport perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offGetReport_perf', 0, () => {
      runPerf('hiddev_offGetReport_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.offGetReport();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFGETREPORT_STRESS
     * @tc.name hiddev_offGetReport_stress
     * @tc.desc Verify Bluetooth apitest case hiddev offGetReport stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offGetReport_stress', 0, () => {
      runStress('hiddev_offGetReport_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.offGetReport();
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_onSetReport', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETREPORT_N
     * @tc.name hiddev_onSetReport_n
     * @tc.desc Verify Bluetooth API hiddev onSetReport normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetReport_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onSetReport(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETREPORT_E
     * @tc.name hiddev_onSetReport_e
     * @tc.desc Verify Bluetooth API hiddev onSetReport error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetReport_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onSetReport(null);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETREPORT_B
     * @tc.name hiddev_onSetReport_b
     * @tc.desc Verify Bluetooth API hiddev onSetReport boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetReport_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onSetReport(null);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETREPORT_R
     * @tc.name hiddev_onSetReport_r
     * @tc.desc Verify Bluetooth API hiddev onSetReport return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetReport_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onSetReport(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETREPORT_X
     * @tc.name hiddev_onSetReport_x
     * @tc.desc Verify Bluetooth API hiddev onSetReport repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetReport_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onSetReport(() => {});
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
        _p.onSetReport(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETREPORT_C
     * @tc.name hiddev_onSetReport_c
     * @tc.desc Verify Bluetooth API hiddev onSetReport condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetReport_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.onSetReport(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETREPORT_PERF
     * @tc.name hiddev_onSetReport_perf
     * @tc.desc Verify Bluetooth apitest case hiddev onSetReport perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetReport_perf', 0, () => {
      runPerf('hiddev_onSetReport_perf', 5000, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.onSetReport(() => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_ONSETREPORT_STRESS
     * @tc.name hiddev_onSetReport_stress
     * @tc.desc Verify Bluetooth apitest case hiddev onSetReport stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_onSetReport_stress', 0, () => {
      runStress('hiddev_onSetReport_stress', 20, () => {
        try {
          const _p = hid.createHidDeviceProfile();
          _p.onSetReport(() => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('hiddev_offSetReport', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETREPORT_N
     * @tc.name hiddev_offSetReport_n
     * @tc.desc Verify Bluetooth API hiddev offSetReport normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetReport_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offSetReport();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HIDDEV_OFFSETREPORT_E
     * @tc.name hiddev_offSetReport_e
     * @tc.desc Verify Bluetooth API hiddev offSetReport error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hiddev_offSetReport_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidDeviceProfile();
        _p.offSetReport();
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


