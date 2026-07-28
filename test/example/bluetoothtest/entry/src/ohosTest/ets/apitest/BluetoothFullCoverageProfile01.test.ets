      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.getAutoPlayDisabledDuration(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_GETAUTOPLAYDISABLEDDURATION_R
     * @tc.name prof_a2dp_getAutoPlayDisabledDuration_r
     * @tc.desc Verify Bluetooth API prof a2dp getAutoPlayDisabledDuration return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_getAutoPlayDisabledDuration_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getAutoPlayDisabledDuration(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_GETAUTOPLAYDISABLEDDURATION_X
     * @tc.name prof_a2dp_getAutoPlayDisabledDuration_x
     * @tc.desc Verify Bluetooth API prof a2dp getAutoPlayDisabledDuration repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_getAutoPlayDisabledDuration_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getAutoPlayDisabledDuration(BADDR) as Object;
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
        value = _p.getAutoPlayDisabledDuration(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_GETAUTOPLAYDISABLEDDURATION_C
     * @tc.name prof_a2dp_getAutoPlayDisabledDuration_c
     * @tc.desc Verify Bluetooth API prof a2dp getAutoPlayDisabledDuration condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_getAutoPlayDisabledDuration_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        value = _p.getAutoPlayDisabledDuration(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_GETAUTOPLAYDISABLEDDURATION_PERF
     * @tc.name prof_a2dp_getAutoPlayDisabledDuration_perf
     * @tc.desc Verify Bluetooth apitest case prof a2dp getAutoPlayDisabledDuration perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_getAutoPlayDisabledDuration_perf', 0, () => {
      runPerf('prof_a2dp_getAutoPlayDisabledDuration_perf', 5000, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.getAutoPlayDisabledDuration(BADDR) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_GETAUTOPLAYDISABLEDDURATION_STRESS
     * @tc.name prof_a2dp_getAutoPlayDisabledDuration_stress
     * @tc.desc Verify Bluetooth apitest case prof a2dp getAutoPlayDisabledDuration stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_getAutoPlayDisabledDuration_stress', 0, () => {
      runStress('prof_a2dp_getAutoPlayDisabledDuration_stress', 20, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.getAutoPlayDisabledDuration(BADDR) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_a2dp_enableAbsoluteVolume', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEABSOLUTEVOLUME_N
     * @tc.name prof_a2dp_enableAbsoluteVolume_n
     * @tc.desc Verify Bluetooth API prof a2dp enableAbsoluteVolume normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAbsoluteVolume_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.enableAbsoluteVolume(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEABSOLUTEVOLUME_E
     * @tc.name prof_a2dp_enableAbsoluteVolume_e
     * @tc.desc Verify Bluetooth API prof a2dp enableAbsoluteVolume error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAbsoluteVolume_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.enableAbsoluteVolume(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEABSOLUTEVOLUME_B
     * @tc.name prof_a2dp_enableAbsoluteVolume_b
     * @tc.desc Verify Bluetooth API prof a2dp enableAbsoluteVolume boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAbsoluteVolume_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.enableAbsoluteVolume(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEABSOLUTEVOLUME_R
     * @tc.name prof_a2dp_enableAbsoluteVolume_r
     * @tc.desc Verify Bluetooth API prof a2dp enableAbsoluteVolume return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAbsoluteVolume_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.enableAbsoluteVolume(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEABSOLUTEVOLUME_X
     * @tc.name prof_a2dp_enableAbsoluteVolume_x
     * @tc.desc Verify Bluetooth API prof a2dp enableAbsoluteVolume repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAbsoluteVolume_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.enableAbsoluteVolume(BADDR);
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
        _p.enableAbsoluteVolume(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEABSOLUTEVOLUME_C
     * @tc.name prof_a2dp_enableAbsoluteVolume_c
     * @tc.desc Verify Bluetooth API prof a2dp enableAbsoluteVolume condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAbsoluteVolume_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.enableAbsoluteVolume(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEABSOLUTEVOLUME_PERF
     * @tc.name prof_a2dp_enableAbsoluteVolume_perf
     * @tc.desc Verify Bluetooth apitest case prof a2dp enableAbsoluteVolume perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAbsoluteVolume_perf', 0, () => {
      runPerf('prof_a2dp_enableAbsoluteVolume_perf', 5000, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.enableAbsoluteVolume(BADDR);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEABSOLUTEVOLUME_STRESS
     * @tc.name prof_a2dp_enableAbsoluteVolume_stress
     * @tc.desc Verify Bluetooth apitest case prof a2dp enableAbsoluteVolume stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAbsoluteVolume_stress', 0, () => {
      runStress('prof_a2dp_enableAbsoluteVolume_stress', 20, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.enableAbsoluteVolume(BADDR);
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_a2dp_disableAbsoluteVolume', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEABSOLUTEVOLUME_N
     * @tc.name prof_a2dp_disableAbsoluteVolume_n
     * @tc.desc Verify Bluetooth API prof a2dp disableAbsoluteVolume normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAbsoluteVolume_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.disableAbsoluteVolume(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEABSOLUTEVOLUME_E
     * @tc.name prof_a2dp_disableAbsoluteVolume_e
     * @tc.desc Verify Bluetooth API prof a2dp disableAbsoluteVolume error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAbsoluteVolume_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.disableAbsoluteVolume(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEABSOLUTEVOLUME_B
     * @tc.name prof_a2dp_disableAbsoluteVolume_b
     * @tc.desc Verify Bluetooth API prof a2dp disableAbsoluteVolume boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAbsoluteVolume_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.disableAbsoluteVolume(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEABSOLUTEVOLUME_R
     * @tc.name prof_a2dp_disableAbsoluteVolume_r
     * @tc.desc Verify Bluetooth API prof a2dp disableAbsoluteVolume return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAbsoluteVolume_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.disableAbsoluteVolume(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEABSOLUTEVOLUME_X
     * @tc.name prof_a2dp_disableAbsoluteVolume_x
     * @tc.desc Verify Bluetooth API prof a2dp disableAbsoluteVolume repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAbsoluteVolume_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.disableAbsoluteVolume(BADDR);
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
        _p.disableAbsoluteVolume(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEABSOLUTEVOLUME_C
     * @tc.name prof_a2dp_disableAbsoluteVolume_c
     * @tc.desc Verify Bluetooth API prof a2dp disableAbsoluteVolume condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAbsoluteVolume_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.disableAbsoluteVolume(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEABSOLUTEVOLUME_PERF
     * @tc.name prof_a2dp_disableAbsoluteVolume_perf
     * @tc.desc Verify Bluetooth apitest case prof a2dp disableAbsoluteVolume perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAbsoluteVolume_perf', 0, () => {
      runPerf('prof_a2dp_disableAbsoluteVolume_perf', 5000, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.disableAbsoluteVolume(BADDR);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEABSOLUTEVOLUME_STRESS
     * @tc.name prof_a2dp_disableAbsoluteVolume_stress
     * @tc.desc Verify Bluetooth apitest case prof a2dp disableAbsoluteVolume stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAbsoluteVolume_stress', 0, () => {
      runStress('prof_a2dp_disableAbsoluteVolume_stress', 20, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.disableAbsoluteVolume(BADDR);
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_a2dp_enableAutoPlay', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEAUTOPLAY_N
     * @tc.name prof_a2dp_enableAutoPlay_n
     * @tc.desc Verify Bluetooth API prof a2dp enableAutoPlay normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAutoPlay_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.enableAutoPlay(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEAUTOPLAY_E
     * @tc.name prof_a2dp_enableAutoPlay_e
     * @tc.desc Verify Bluetooth API prof a2dp enableAutoPlay error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAutoPlay_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.enableAutoPlay(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEAUTOPLAY_B
     * @tc.name prof_a2dp_enableAutoPlay_b
     * @tc.desc Verify Bluetooth API prof a2dp enableAutoPlay boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAutoPlay_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.enableAutoPlay(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEAUTOPLAY_R
     * @tc.name prof_a2dp_enableAutoPlay_r
     * @tc.desc Verify Bluetooth API prof a2dp enableAutoPlay return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAutoPlay_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.enableAutoPlay(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEAUTOPLAY_X
     * @tc.name prof_a2dp_enableAutoPlay_x
     * @tc.desc Verify Bluetooth API prof a2dp enableAutoPlay repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAutoPlay_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.enableAutoPlay(BADDR);
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
        _p.enableAutoPlay(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEAUTOPLAY_C
     * @tc.name prof_a2dp_enableAutoPlay_c
     * @tc.desc Verify Bluetooth API prof a2dp enableAutoPlay condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAutoPlay_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.enableAutoPlay(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEAUTOPLAY_PERF
     * @tc.name prof_a2dp_enableAutoPlay_perf
     * @tc.desc Verify Bluetooth apitest case prof a2dp enableAutoPlay perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAutoPlay_perf', 0, () => {
      runPerf('prof_a2dp_enableAutoPlay_perf', 5000, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.enableAutoPlay(BADDR);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_ENABLEAUTOPLAY_STRESS
     * @tc.name prof_a2dp_enableAutoPlay_stress
     * @tc.desc Verify Bluetooth apitest case prof a2dp enableAutoPlay stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_enableAutoPlay_stress', 0, () => {
      runStress('prof_a2dp_enableAutoPlay_stress', 20, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.enableAutoPlay(BADDR);
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_a2dp_disableAutoPlay', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEAUTOPLAY_N
     * @tc.name prof_a2dp_disableAutoPlay_n
     * @tc.desc Verify Bluetooth API prof a2dp disableAutoPlay normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAutoPlay_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.disableAutoPlay(BADDR, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEAUTOPLAY_E
     * @tc.name prof_a2dp_disableAutoPlay_e
     * @tc.desc Verify Bluetooth API prof a2dp disableAutoPlay error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAutoPlay_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.disableAutoPlay(EMPTY_MAC, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEAUTOPLAY_B
     * @tc.name prof_a2dp_disableAutoPlay_b
     * @tc.desc Verify Bluetooth API prof a2dp disableAutoPlay boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAutoPlay_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.disableAutoPlay(EMPTY_MAC, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEAUTOPLAY_R
     * @tc.name prof_a2dp_disableAutoPlay_r
     * @tc.desc Verify Bluetooth API prof a2dp disableAutoPlay return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAutoPlay_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.disableAutoPlay(BADDR, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEAUTOPLAY_X
     * @tc.name prof_a2dp_disableAutoPlay_x
     * @tc.desc Verify Bluetooth API prof a2dp disableAutoPlay repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAutoPlay_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.disableAutoPlay(BADDR, 0);
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
        _p.disableAutoPlay(BADDR, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEAUTOPLAY_C
     * @tc.name prof_a2dp_disableAutoPlay_c
     * @tc.desc Verify Bluetooth API prof a2dp disableAutoPlay condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAutoPlay_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.disableAutoPlay(BADDR, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEAUTOPLAY_PERF
     * @tc.name prof_a2dp_disableAutoPlay_perf
     * @tc.desc Verify Bluetooth apitest case prof a2dp disableAutoPlay perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAutoPlay_perf', 0, () => {
      runPerf('prof_a2dp_disableAutoPlay_perf', 5000, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.disableAutoPlay(BADDR, 0);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_DISABLEAUTOPLAY_STRESS
     * @tc.name prof_a2dp_disableAutoPlay_stress
     * @tc.desc Verify Bluetooth apitest case prof a2dp disableAutoPlay stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_disableAutoPlay_stress', 0, () => {
      runStress('prof_a2dp_disableAutoPlay_stress', 20, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.disableAutoPlay(BADDR, 0);
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_a2dp_setCurrentCodecInfo', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_SETCURRENTCODECINFO_N
     * @tc.name prof_a2dp_setCurrentCodecInfo_n
     * @tc.desc Verify Bluetooth API prof a2dp setCurrentCodecInfo normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_setCurrentCodecInfo_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.setCurrentCodecInfo(BADDR, {codecType:0, codecBitsPerSample:0, codecChannelMode:0,
          codecSampleRate:0});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_SETCURRENTCODECINFO_E
     * @tc.name prof_a2dp_setCurrentCodecInfo_e
     * @tc.desc Verify Bluetooth API prof a2dp setCurrentCodecInfo error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_setCurrentCodecInfo_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.setCurrentCodecInfo(EMPTY_MAC, {codecType:0, codecBitsPerSample:0, codecChannelMode:0,
          codecSampleRate:0});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_SETCURRENTCODECINFO_B
     * @tc.name prof_a2dp_setCurrentCodecInfo_b
     * @tc.desc Verify Bluetooth API prof a2dp setCurrentCodecInfo boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_setCurrentCodecInfo_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.setCurrentCodecInfo(EMPTY_MAC, {codecType:0, codecBitsPerSample:0, codecChannelMode:0,
          codecSampleRate:0});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_SETCURRENTCODECINFO_R
     * @tc.name prof_a2dp_setCurrentCodecInfo_r
     * @tc.desc Verify Bluetooth API prof a2dp setCurrentCodecInfo return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_setCurrentCodecInfo_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.setCurrentCodecInfo(BADDR, {codecType:0, codecBitsPerSample:0, codecChannelMode:0,
          codecSampleRate:0});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_SETCURRENTCODECINFO_X
     * @tc.name prof_a2dp_setCurrentCodecInfo_x
     * @tc.desc Verify Bluetooth API prof a2dp setCurrentCodecInfo repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_setCurrentCodecInfo_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.setCurrentCodecInfo(BADDR, {codecType:0, codecBitsPerSample:0, codecChannelMode:0,
          codecSampleRate:0});
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
        _p.setCurrentCodecInfo(BADDR, {codecType:0, codecBitsPerSample:0, codecChannelMode:0,
          codecSampleRate:0});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_SETCURRENTCODECINFO_C
     * @tc.name prof_a2dp_setCurrentCodecInfo_c
     * @tc.desc Verify Bluetooth API prof a2dp setCurrentCodecInfo condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_setCurrentCodecInfo_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = a2dp.createA2dpSrcProfile();
        _p.setCurrentCodecInfo(BADDR, {codecType:0, codecBitsPerSample:0, codecChannelMode:0,
          codecSampleRate:0});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_SETCURRENTCODECINFO_PERF
     * @tc.name prof_a2dp_setCurrentCodecInfo_perf
     * @tc.desc Verify Bluetooth apitest case prof a2dp setCurrentCodecInfo perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_setCurrentCodecInfo_perf', 0, () => {
      runPerf('prof_a2dp_setCurrentCodecInfo_perf', 5000, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.setCurrentCodecInfo(BADDR, {codecType:0, codecBitsPerSample:0, codecChannelMode:0,
            codecSampleRate:0});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_A2DP_SETCURRENTCODECINFO_STRESS
     * @tc.name prof_a2dp_setCurrentCodecInfo_stress
     * @tc.desc Verify Bluetooth apitest case prof a2dp setCurrentCodecInfo stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_a2dp_setCurrentCodecInfo_stress', 0, () => {
      runStress('prof_a2dp_setCurrentCodecInfo_stress', 20, () => {
        try {
          const _p = a2dp.createA2dpSrcProfile();
          _p.setCurrentCodecInfo(BADDR, {codecType:0, codecBitsPerSample:0, codecChannelMode:0,
            codecSampleRate:0});
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_hfp_createHfpAgProfile', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CREATEHFPAGPROFILE_N
     * @tc.name prof_hfp_createHfpAgProfile_n
     * @tc.desc Verify Bluetooth API prof hfp createHfpAgProfile normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_createHfpAgProfile_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = hfp.createHfpAgProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CREATEHFPAGPROFILE_E
     * @tc.name prof_hfp_createHfpAgProfile_e
     * @tc.desc Verify Bluetooth API prof hfp createHfpAgProfile error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_createHfpAgProfile_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        hfp.createHfpAgProfile();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CREATEHFPAGPROFILE_B
     * @tc.name prof_hfp_createHfpAgProfile_b
     * @tc.desc Verify Bluetooth API prof hfp createHfpAgProfile boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_createHfpAgProfile_b', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        value = hfp.createHfpAgProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CREATEHFPAGPROFILE_R
     * @tc.name prof_hfp_createHfpAgProfile_r
     * @tc.desc Verify Bluetooth API prof hfp createHfpAgProfile return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_createHfpAgProfile_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = hfp.createHfpAgProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CREATEHFPAGPROFILE_X
     * @tc.name prof_hfp_createHfpAgProfile_x
     * @tc.desc Verify Bluetooth API prof hfp createHfpAgProfile repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_createHfpAgProfile_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = hfp.createHfpAgProfile() as Object;
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
        value = hfp.createHfpAgProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CREATEHFPAGPROFILE_C
     * @tc.name prof_hfp_createHfpAgProfile_c
     * @tc.desc Verify Bluetooth API prof hfp createHfpAgProfile condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_createHfpAgProfile_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = hfp.createHfpAgProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CREATEHFPAGPROFILE_PERF
     * @tc.name prof_hfp_createHfpAgProfile_perf
     * @tc.desc Verify Bluetooth apitest case prof hfp createHfpAgProfile perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_createHfpAgProfile_perf', 0, () => {
      runPerf('prof_hfp_createHfpAgProfile_perf', 5000, () => {
        try {
          hfp.createHfpAgProfile() as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CREATEHFPAGPROFILE_STRESS
     * @tc.name prof_hfp_createHfpAgProfile_stress
     * @tc.desc Verify Bluetooth apitest case prof hfp createHfpAgProfile stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_createHfpAgProfile_stress', 0, () => {
      runStress('prof_hfp_createHfpAgProfile_stress', 20, () => {
        try {
          hfp.createHfpAgProfile() as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_hfp_connect', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CONNECT_N
     * @tc.name prof_hfp_connect_n
     * @tc.desc Verify Bluetooth API prof hfp connect normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_connect_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hfp.createHfpAgProfile();
        _p.connect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CONNECT_E
     * @tc.name prof_hfp_connect_e
     * @tc.desc Verify Bluetooth API prof hfp connect error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_connect_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hfp.createHfpAgProfile();
        _p.connect(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CONNECT_B
     * @tc.name prof_hfp_connect_b
     * @tc.desc Verify Bluetooth API prof hfp connect boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_connect_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hfp.createHfpAgProfile();
        _p.connect(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CONNECT_R
     * @tc.name prof_hfp_connect_r
     * @tc.desc Verify Bluetooth API prof hfp connect return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_connect_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hfp.createHfpAgProfile();
        _p.connect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CONNECT_X
     * @tc.name prof_hfp_connect_x
     * @tc.desc Verify Bluetooth API prof hfp connect repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_connect_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hfp.createHfpAgProfile();
        _p.connect(BADDR);
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
        const _p = hfp.createHfpAgProfile();
        _p.connect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CONNECT_C
     * @tc.name prof_hfp_connect_c
     * @tc.desc Verify Bluetooth API prof hfp connect condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_connect_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hfp.createHfpAgProfile();
        _p.connect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CONNECT_PERF
     * @tc.name prof_hfp_connect_perf
     * @tc.desc Verify Bluetooth apitest case prof hfp connect perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_connect_perf', 0, () => {
      runPerf('prof_hfp_connect_perf', 5000, () => {
        try {
          const _p = hfp.createHfpAgProfile();
          _p.connect(BADDR);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_CONNECT_STRESS
     * @tc.name prof_hfp_connect_stress
     * @tc.desc Verify Bluetooth apitest case prof hfp connect stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_connect_stress', 0, () => {
      runStress('prof_hfp_connect_stress', 20, () => {
        try {
          const _p = hfp.createHfpAgProfile();
          _p.connect(BADDR);
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_hfp_disconnect', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_DISCONNECT_N
     * @tc.name prof_hfp_disconnect_n
     * @tc.desc Verify Bluetooth API prof hfp disconnect normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_disconnect_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hfp.createHfpAgProfile();
        _p.disconnect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_DISCONNECT_E
     * @tc.name prof_hfp_disconnect_e
     * @tc.desc Verify Bluetooth API prof hfp disconnect error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_disconnect_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hfp.createHfpAgProfile();
        _p.disconnect(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_DISCONNECT_B
     * @tc.name prof_hfp_disconnect_b
     * @tc.desc Verify Bluetooth API prof hfp disconnect boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_disconnect_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hfp.createHfpAgProfile();
        _p.disconnect(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_DISCONNECT_R
     * @tc.name prof_hfp_disconnect_r
     * @tc.desc Verify Bluetooth API prof hfp disconnect return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_disconnect_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hfp.createHfpAgProfile();
        _p.disconnect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_DISCONNECT_X
     * @tc.name prof_hfp_disconnect_x
     * @tc.desc Verify Bluetooth API prof hfp disconnect repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_disconnect_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hfp.createHfpAgProfile();
        _p.disconnect(BADDR);
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
        const _p = hfp.createHfpAgProfile();
        _p.disconnect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_DISCONNECT_C
     * @tc.name prof_hfp_disconnect_c
     * @tc.desc Verify Bluetooth API prof hfp disconnect condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_disconnect_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hfp.createHfpAgProfile();
        _p.disconnect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_DISCONNECT_PERF
     * @tc.name prof_hfp_disconnect_perf
     * @tc.desc Verify Bluetooth apitest case prof hfp disconnect perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_disconnect_perf', 0, () => {
      runPerf('prof_hfp_disconnect_perf', 5000, () => {
        try {
          const _p = hfp.createHfpAgProfile();
          _p.disconnect(BADDR);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HFP_DISCONNECT_STRESS
     * @tc.name prof_hfp_disconnect_stress
     * @tc.desc Verify Bluetooth apitest case prof hfp disconnect stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hfp_disconnect_stress', 0, () => {
      runStress('prof_hfp_disconnect_stress', 20, () => {
        try {
          const _p = hfp.createHfpAgProfile();
          _p.disconnect(BADDR);
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_hidhost_createHidHostProfile', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CREATEHIDHOSTPROFILE_N
     * @tc.name prof_hidhost_createHidHostProfile_n
     * @tc.desc Verify Bluetooth API prof hidhost createHidHostProfile normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_createHidHostProfile_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = hid.createHidHostProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CREATEHIDHOSTPROFILE_E
     * @tc.name prof_hidhost_createHidHostProfile_e
     * @tc.desc Verify Bluetooth API prof hidhost createHidHostProfile error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_createHidHostProfile_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        hid.createHidHostProfile();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CREATEHIDHOSTPROFILE_B
     * @tc.name prof_hidhost_createHidHostProfile_b
     * @tc.desc Verify Bluetooth API prof hidhost createHidHostProfile boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_createHidHostProfile_b', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        value = hid.createHidHostProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CREATEHIDHOSTPROFILE_R
     * @tc.name prof_hidhost_createHidHostProfile_r
     * @tc.desc Verify Bluetooth API prof hidhost createHidHostProfile return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_createHidHostProfile_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = hid.createHidHostProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CREATEHIDHOSTPROFILE_X
     * @tc.name prof_hidhost_createHidHostProfile_x
     * @tc.desc Verify Bluetooth API prof hidhost createHidHostProfile repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_createHidHostProfile_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = hid.createHidHostProfile() as Object;
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
        value = hid.createHidHostProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CREATEHIDHOSTPROFILE_C
     * @tc.name prof_hidhost_createHidHostProfile_c
     * @tc.desc Verify Bluetooth API prof hidhost createHidHostProfile condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_createHidHostProfile_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = hid.createHidHostProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CREATEHIDHOSTPROFILE_PERF
     * @tc.name prof_hidhost_createHidHostProfile_perf
     * @tc.desc Verify Bluetooth apitest case prof hidhost createHidHostProfile perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_createHidHostProfile_perf', 0, () => {
      runPerf('prof_hidhost_createHidHostProfile_perf', 5000, () => {
        try {
          hid.createHidHostProfile() as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CREATEHIDHOSTPROFILE_STRESS
     * @tc.name prof_hidhost_createHidHostProfile_stress
     * @tc.desc Verify Bluetooth apitest case prof hidhost createHidHostProfile stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_createHidHostProfile_stress', 0, () => {
      runStress('prof_hidhost_createHidHostProfile_stress', 20, () => {
        try {
          hid.createHidHostProfile() as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_hidhost_connect', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CONNECT_N
     * @tc.name prof_hidhost_connect_n
     * @tc.desc Verify Bluetooth API prof hidhost connect normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_connect_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidHostProfile();
        _p.connect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CONNECT_E
     * @tc.name prof_hidhost_connect_e
     * @tc.desc Verify Bluetooth API prof hidhost connect error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_connect_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidHostProfile();
        _p.connect(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CONNECT_B
     * @tc.name prof_hidhost_connect_b
     * @tc.desc Verify Bluetooth API prof hidhost connect boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_connect_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidHostProfile();
        _p.connect(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CONNECT_R
     * @tc.name prof_hidhost_connect_r
     * @tc.desc Verify Bluetooth API prof hidhost connect return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_connect_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidHostProfile();
        _p.connect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CONNECT_X
     * @tc.name prof_hidhost_connect_x
     * @tc.desc Verify Bluetooth API prof hidhost connect repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_connect_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidHostProfile();
        _p.connect(BADDR);
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
        const _p = hid.createHidHostProfile();
        _p.connect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CONNECT_C
     * @tc.name prof_hidhost_connect_c
     * @tc.desc Verify Bluetooth API prof hidhost connect condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_connect_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidHostProfile();
        _p.connect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CONNECT_PERF
     * @tc.name prof_hidhost_connect_perf
     * @tc.desc Verify Bluetooth apitest case prof hidhost connect perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_connect_perf', 0, () => {
      runPerf('prof_hidhost_connect_perf', 5000, () => {
        try {
          const _p = hid.createHidHostProfile();
          _p.connect(BADDR);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_CONNECT_STRESS
     * @tc.name prof_hidhost_connect_stress
     * @tc.desc Verify Bluetooth apitest case prof hidhost connect stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_connect_stress', 0, () => {
      runStress('prof_hidhost_connect_stress', 20, () => {
        try {
          const _p = hid.createHidHostProfile();
          _p.connect(BADDR);
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_hidhost_disconnect', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_DISCONNECT_N
     * @tc.name prof_hidhost_disconnect_n
     * @tc.desc Verify Bluetooth API prof hidhost disconnect normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_disconnect_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = hid.createHidHostProfile();
        _p.disconnect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_DISCONNECT_E
     * @tc.name prof_hidhost_disconnect_e
     * @tc.desc Verify Bluetooth API prof hidhost disconnect error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_disconnect_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = hid.createHidHostProfile();
        _p.disconnect(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_DISCONNECT_B
     * @tc.name prof_hidhost_disconnect_b
     * @tc.desc Verify Bluetooth API prof hidhost disconnect boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_disconnect_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = hid.createHidHostProfile();
        _p.disconnect(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_DISCONNECT_R
     * @tc.name prof_hidhost_disconnect_r
     * @tc.desc Verify Bluetooth API prof hidhost disconnect return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_disconnect_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = hid.createHidHostProfile();
        _p.disconnect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_DISCONNECT_X
     * @tc.name prof_hidhost_disconnect_x
     * @tc.desc Verify Bluetooth API prof hidhost disconnect repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_disconnect_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = hid.createHidHostProfile();
        _p.disconnect(BADDR);
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
        const _p = hid.createHidHostProfile();
        _p.disconnect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_DISCONNECT_C
     * @tc.name prof_hidhost_disconnect_c
     * @tc.desc Verify Bluetooth API prof hidhost disconnect condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_disconnect_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = hid.createHidHostProfile();
        _p.disconnect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_DISCONNECT_PERF
     * @tc.name prof_hidhost_disconnect_perf
     * @tc.desc Verify Bluetooth apitest case prof hidhost disconnect perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_disconnect_perf', 0, () => {
      runPerf('prof_hidhost_disconnect_perf', 5000, () => {
        try {
          const _p = hid.createHidHostProfile();
          _p.disconnect(BADDR);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_HIDHOST_DISCONNECT_STRESS
     * @tc.name prof_hidhost_disconnect_stress
     * @tc.desc Verify Bluetooth apitest case prof hidhost disconnect stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_hidhost_disconnect_stress', 0, () => {
      runStress('prof_hidhost_disconnect_stress', 20, () => {
        try {
          const _p = hid.createHidHostProfile();
          _p.disconnect(BADDR);
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_map_createMapMseProfile', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_CREATEMAPMSEPROFILE_N
     * @tc.name prof_map_createMapMseProfile_n
     * @tc.desc Verify Bluetooth API prof map createMapMseProfile normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_createMapMseProfile_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = map.createMapMseProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_CREATEMAPMSEPROFILE_E
     * @tc.name prof_map_createMapMseProfile_e
     * @tc.desc Verify Bluetooth API prof map createMapMseProfile error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_createMapMseProfile_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        map.createMapMseProfile();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_CREATEMAPMSEPROFILE_B
     * @tc.name prof_map_createMapMseProfile_b
     * @tc.desc Verify Bluetooth API prof map createMapMseProfile boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_createMapMseProfile_b', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        value = map.createMapMseProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_CREATEMAPMSEPROFILE_R
     * @tc.name prof_map_createMapMseProfile_r
     * @tc.desc Verify Bluetooth API prof map createMapMseProfile return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_createMapMseProfile_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = map.createMapMseProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_CREATEMAPMSEPROFILE_X
     * @tc.name prof_map_createMapMseProfile_x
     * @tc.desc Verify Bluetooth API prof map createMapMseProfile repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_createMapMseProfile_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = map.createMapMseProfile() as Object;
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
        value = map.createMapMseProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_CREATEMAPMSEPROFILE_C
     * @tc.name prof_map_createMapMseProfile_c
     * @tc.desc Verify Bluetooth API prof map createMapMseProfile condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_createMapMseProfile_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = map.createMapMseProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_CREATEMAPMSEPROFILE_PERF
     * @tc.name prof_map_createMapMseProfile_perf
     * @tc.desc Verify Bluetooth apitest case prof map createMapMseProfile perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_createMapMseProfile_perf', 0, () => {
      runPerf('prof_map_createMapMseProfile_perf', 5000, () => {
        try {
          map.createMapMseProfile() as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_CREATEMAPMSEPROFILE_STRESS
     * @tc.name prof_map_createMapMseProfile_stress
     * @tc.desc Verify Bluetooth apitest case prof map createMapMseProfile stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_createMapMseProfile_stress', 0, () => {
      runStress('prof_map_createMapMseProfile_stress', 20, () => {
        try {
          map.createMapMseProfile() as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_map_disconnect', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_DISCONNECT_N
     * @tc.name prof_map_disconnect_n
     * @tc.desc Verify Bluetooth API prof map disconnect normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_disconnect_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = map.createMapMseProfile();
        _p.disconnect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_DISCONNECT_E
     * @tc.name prof_map_disconnect_e
     * @tc.desc Verify Bluetooth API prof map disconnect error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_disconnect_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = map.createMapMseProfile();
        _p.disconnect(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_DISCONNECT_B
     * @tc.name prof_map_disconnect_b
     * @tc.desc Verify Bluetooth API prof map disconnect boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_disconnect_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = map.createMapMseProfile();
        _p.disconnect(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_DISCONNECT_R
     * @tc.name prof_map_disconnect_r
     * @tc.desc Verify Bluetooth API prof map disconnect return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_disconnect_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = map.createMapMseProfile();
        _p.disconnect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_DISCONNECT_X
     * @tc.name prof_map_disconnect_x
     * @tc.desc Verify Bluetooth API prof map disconnect repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_disconnect_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = map.createMapMseProfile();
        _p.disconnect(BADDR);
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
        const _p = map.createMapMseProfile();
        _p.disconnect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_DISCONNECT_C
     * @tc.name prof_map_disconnect_c
     * @tc.desc Verify Bluetooth API prof map disconnect condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_disconnect_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = map.createMapMseProfile();
        _p.disconnect(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_DISCONNECT_PERF
     * @tc.name prof_map_disconnect_perf
     * @tc.desc Verify Bluetooth apitest case prof map disconnect perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_disconnect_perf', 0, () => {
      runPerf('prof_map_disconnect_perf', 5000, () => {
        try {
          const _p = map.createMapMseProfile();
          _p.disconnect(BADDR);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_DISCONNECT_STRESS
     * @tc.name prof_map_disconnect_stress
     * @tc.desc Verify Bluetooth apitest case prof map disconnect stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_disconnect_stress', 0, () => {
      runStress('prof_map_disconnect_stress', 20, () => {
        try {
          const _p = map.createMapMseProfile();
          _p.disconnect(BADDR);
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_map_setMessageAccessAuthorization', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_SETMESSAGEACCESSAUTHORIZATION_N
     * @tc.name prof_map_setMessageAccessAuthorization_n
     * @tc.desc Verify Bluetooth API prof map setMessageAccessAuthorization normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_setMessageAccessAuthorization_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = map.createMapMseProfile();
        _p.setMessageAccessAuthorization(BADDR, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_SETMESSAGEACCESSAUTHORIZATION_E
     * @tc.name prof_map_setMessageAccessAuthorization_e
     * @tc.desc Verify Bluetooth API prof map setMessageAccessAuthorization error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_setMessageAccessAuthorization_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = map.createMapMseProfile();
        _p.setMessageAccessAuthorization(EMPTY_MAC, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_SETMESSAGEACCESSAUTHORIZATION_B
     * @tc.name prof_map_setMessageAccessAuthorization_b
     * @tc.desc Verify Bluetooth API prof map setMessageAccessAuthorization boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_setMessageAccessAuthorization_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = map.createMapMseProfile();
        _p.setMessageAccessAuthorization(EMPTY_MAC, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_SETMESSAGEACCESSAUTHORIZATION_R
     * @tc.name prof_map_setMessageAccessAuthorization_r
     * @tc.desc Verify Bluetooth API prof map setMessageAccessAuthorization return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_setMessageAccessAuthorization_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = map.createMapMseProfile();
        _p.setMessageAccessAuthorization(BADDR, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_SETMESSAGEACCESSAUTHORIZATION_X
     * @tc.name prof_map_setMessageAccessAuthorization_x
     * @tc.desc Verify Bluetooth API prof map setMessageAccessAuthorization repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_setMessageAccessAuthorization_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = map.createMapMseProfile();
        _p.setMessageAccessAuthorization(BADDR, 0);
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
        const _p = map.createMapMseProfile();
        _p.setMessageAccessAuthorization(BADDR, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_SETMESSAGEACCESSAUTHORIZATION_C
     * @tc.name prof_map_setMessageAccessAuthorization_c
     * @tc.desc Verify Bluetooth API prof map setMessageAccessAuthorization condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_setMessageAccessAuthorization_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = map.createMapMseProfile();
        _p.setMessageAccessAuthorization(BADDR, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_SETMESSAGEACCESSAUTHORIZATION_PERF
     * @tc.name prof_map_setMessageAccessAuthorization_perf
     * @tc.desc Verify Bluetooth apitest case prof map setMessageAccessAuthorization perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_setMessageAccessAuthorization_perf', 0, () => {
      runPerf('prof_map_setMessageAccessAuthorization_perf', 5000, () => {
        try {
          const _p = map.createMapMseProfile();
          _p.setMessageAccessAuthorization(BADDR, 0);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_SETMESSAGEACCESSAUTHORIZATION_STRESS
     * @tc.name prof_map_setMessageAccessAuthorization_stress
     * @tc.desc Verify Bluetooth apitest case prof map setMessageAccessAuthorization stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_setMessageAccessAuthorization_stress', 0, () => {
      runStress('prof_map_setMessageAccessAuthorization_stress', 20, () => {
        try {
          const _p = map.createMapMseProfile();
          _p.setMessageAccessAuthorization(BADDR, 0);
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_map_getMessageAccessAuthorization', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_GETMESSAGEACCESSAUTHORIZATION_N
     * @tc.name prof_map_getMessageAccessAuthorization_n
     * @tc.desc Verify Bluetooth API prof map getMessageAccessAuthorization normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_getMessageAccessAuthorization_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const _p = map.createMapMseProfile();
        value = _p.getMessageAccessAuthorization(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_GETMESSAGEACCESSAUTHORIZATION_E
     * @tc.name prof_map_getMessageAccessAuthorization_e
     * @tc.desc Verify Bluetooth API prof map getMessageAccessAuthorization error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_getMessageAccessAuthorization_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const _p = map.createMapMseProfile();
        _p.getMessageAccessAuthorization(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_GETMESSAGEACCESSAUTHORIZATION_B
     * @tc.name prof_map_getMessageAccessAuthorization_b
     * @tc.desc Verify Bluetooth API prof map getMessageAccessAuthorization boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_getMessageAccessAuthorization_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const _p = map.createMapMseProfile();
        _p.getMessageAccessAuthorization(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_GETMESSAGEACCESSAUTHORIZATION_R
     * @tc.name prof_map_getMessageAccessAuthorization_r
     * @tc.desc Verify Bluetooth API prof map getMessageAccessAuthorization return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_getMessageAccessAuthorization_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const _p = map.createMapMseProfile();
        value = _p.getMessageAccessAuthorization(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_GETMESSAGEACCESSAUTHORIZATION_X
     * @tc.name prof_map_getMessageAccessAuthorization_x
     * @tc.desc Verify Bluetooth API prof map getMessageAccessAuthorization repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_getMessageAccessAuthorization_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const _p = map.createMapMseProfile();
        value = _p.getMessageAccessAuthorization(BADDR) as Object;
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
        const _p = map.createMapMseProfile();
        value = _p.getMessageAccessAuthorization(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_GETMESSAGEACCESSAUTHORIZATION_C
     * @tc.name prof_map_getMessageAccessAuthorization_c
     * @tc.desc Verify Bluetooth API prof map getMessageAccessAuthorization condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_getMessageAccessAuthorization_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const _p = map.createMapMseProfile();
        value = _p.getMessageAccessAuthorization(BADDR) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_GETMESSAGEACCESSAUTHORIZATION_PERF
     * @tc.name prof_map_getMessageAccessAuthorization_perf
     * @tc.desc Verify Bluetooth apitest case prof map getMessageAccessAuthorization perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_getMessageAccessAuthorization_perf', 0, () => {
      runPerf('prof_map_getMessageAccessAuthorization_perf', 5000, () => {
        try {
          const _p = map.createMapMseProfile();
          _p.getMessageAccessAuthorization(BADDR) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_MAP_GETMESSAGEACCESSAUTHORIZATION_STRESS
     * @tc.name prof_map_getMessageAccessAuthorization_stress
     * @tc.desc Verify Bluetooth apitest case prof map getMessageAccessAuthorization stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_map_getMessageAccessAuthorization_stress', 0, () => {
      runStress('prof_map_getMessageAccessAuthorization_stress', 20, () => {
        try {
          const _p = map.createMapMseProfile();
          _p.getMessageAccessAuthorization(BADDR) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('prof_pan_createPanProfile', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_PAN_CREATEPANPROFILE_N
     * @tc.name prof_pan_createPanProfile_n
     * @tc.desc Verify Bluetooth API prof pan createPanProfile normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_pan_createPanProfile_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = pan.createPanProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_PAN_CREATEPANPROFILE_E
     * @tc.name prof_pan_createPanProfile_e
     * @tc.desc Verify Bluetooth API prof pan createPanProfile error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_pan_createPanProfile_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        pan.createPanProfile();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_PAN_CREATEPANPROFILE_B
     * @tc.name prof_pan_createPanProfile_b
     * @tc.desc Verify Bluetooth API prof pan createPanProfile boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_pan_createPanProfile_b', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        value = pan.createPanProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_PAN_CREATEPANPROFILE_R
     * @tc.name prof_pan_createPanProfile_r
     * @tc.desc Verify Bluetooth API prof pan createPanProfile return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_pan_createPanProfile_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = pan.createPanProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_PAN_CREATEPANPROFILE_X
     * @tc.name prof_pan_createPanProfile_x
     * @tc.desc Verify Bluetooth API prof pan createPanProfile repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_pan_createPanProfile_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = pan.createPanProfile() as Object;
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
        value = pan.createPanProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_PAN_CREATEPANPROFILE_C
     * @tc.name prof_pan_createPanProfile_c
     * @tc.desc Verify Bluetooth API prof pan createPanProfile condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_pan_createPanProfile_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = pan.createPanProfile() as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_PAN_CREATEPANPROFILE_PERF
     * @tc.name prof_pan_createPanProfile_perf
     * @tc.desc Verify Bluetooth apitest case prof pan createPanProfile perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_pan_createPanProfile_perf', 0, () => {
      runPerf('prof_pan_createPanProfile_perf', 5000, () => {
        try {
          pan.createPanProfile() as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PROF_PAN_CREATEPANPROFILE_STRESS
     * @tc.name prof_pan_createPanProfile_stress
     * @tc.desc Verify Bluetooth apitest case prof pan createPanProfile stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('prof_pan_createPanProfile_stress', 0, () => {
      runStress('prof_pan_createPanProfile_stress', 20, () => {
        try {
          pan.createPanProfile() as Object;
        } catch (_e) {
        }
      });
    });
    });
  });
}


