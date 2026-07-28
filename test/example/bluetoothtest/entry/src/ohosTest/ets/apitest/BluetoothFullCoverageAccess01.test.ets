          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_DELETEPERSISTENTDEVICEID_E
     * @tc.name ac_deletePersistentDeviceId_e
     * @tc.desc Verify Bluetooth API ac deletePersistentDeviceId error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_deletePersistentDeviceId_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        access.deletePersistentDeviceId(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_DELETEPERSISTENTDEVICEID_B
     * @tc.name ac_deletePersistentDeviceId_b
     * @tc.desc Verify Bluetooth API ac deletePersistentDeviceId boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_deletePersistentDeviceId_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        access.deletePersistentDeviceId(BAD_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_DELETEPERSISTENTDEVICEID_R
     * @tc.name ac_deletePersistentDeviceId_r
     * @tc.desc Verify Bluetooth API ac deletePersistentDeviceId return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_deletePersistentDeviceId_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        access.deletePersistentDeviceId(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_DELETEPERSISTENTDEVICEID_X
     * @tc.name ac_deletePersistentDeviceId_x
     * @tc.desc Verify Bluetooth API ac deletePersistentDeviceId repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_deletePersistentDeviceId_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        access.deletePersistentDeviceId(BADDR);
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
        access.deletePersistentDeviceId(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_DELETEPERSISTENTDEVICEID_C
     * @tc.name ac_deletePersistentDeviceId_c
     * @tc.desc Verify Bluetooth API ac deletePersistentDeviceId condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_deletePersistentDeviceId_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        access.deletePersistentDeviceId(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_DELETEPERSISTENTDEVICEID_PERF
     * @tc.name ac_deletePersistentDeviceId_perf
     * @tc.desc Verify Bluetooth apitest case ac deletePersistentDeviceId perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_deletePersistentDeviceId_perf', 0, () => {
      runPerf('ac_deletePersistentDeviceId_perf', 5000, () => {
        try {
          access.deletePersistentDeviceId(BADDR);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_DELETEPERSISTENTDEVICEID_STRESS
     * @tc.name ac_deletePersistentDeviceId_stress
     * @tc.desc Verify Bluetooth apitest case ac deletePersistentDeviceId stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_deletePersistentDeviceId_stress', 0, () => {
      runStress('ac_deletePersistentDeviceId_stress', 20, () => {
        try {
          access.deletePersistentDeviceId(BADDR);
        } catch (_e) {
        }
      });
    });
    });
    describe('ac_on_stateChange', () => {
    it('ac_on_stateChange_n', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        access.on('stateChange', () => {});
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
    it('ac_on_stateChange_e', 0, async (done: Function) => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        access.on('stateChange', null);
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
    it('ac_on_stateChange_b', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        access.on('stateChange', () => {});
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
    it('ac_on_stateChange_r', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        access.on('stateChange', () => {});
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
    it('ac_on_stateChange_x', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        access.on('stateChange', () => {});
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
        access.on('stateChange', () => {});
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
    it('ac_on_stateChange_c', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        access.on('stateChange', (_state) => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_ON_STATECHANGE_PERF
     * @tc.name ac_on_stateChange_perf
     * @tc.desc Verify Bluetooth apitest case ac on stateChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_on_stateChange_perf', 0, () => {
      runPerf('ac_on_stateChange_perf', 5000, () => {
        try {
          access.on('stateChange', () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_ON_STATECHANGE_STRESS
     * @tc.name ac_on_stateChange_stress
     * @tc.desc Verify Bluetooth apitest case ac on stateChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_on_stateChange_stress', 0, () => {
      runStress('ac_on_stateChange_stress', 20, () => {
        try {
          access.on('stateChange', () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('ac_off_stateChange', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_OFF_STATECHANGE_N
     * @tc.name ac_off_stateChange_n
     * @tc.desc Verify Bluetooth API ac off stateChange normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_off_stateChange_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        access.off('stateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_OFF_STATECHANGE_E
     * @tc.name ac_off_stateChange_e
     * @tc.desc Verify Bluetooth API ac off stateChange error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_off_stateChange_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        access.off('stateChange', () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_OFF_STATECHANGE_B
     * @tc.name ac_off_stateChange_b
     * @tc.desc Verify Bluetooth API ac off stateChange boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_off_stateChange_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        access.off('stateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_OFF_STATECHANGE_R
     * @tc.name ac_off_stateChange_r
     * @tc.desc Verify Bluetooth API ac off stateChange return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_off_stateChange_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        access.off('stateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_OFF_STATECHANGE_X
     * @tc.name ac_off_stateChange_x
     * @tc.desc Verify Bluetooth API ac off stateChange repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_off_stateChange_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        access.off('stateChange');
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
        access.off('stateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_OFF_STATECHANGE_C
     * @tc.name ac_off_stateChange_c
     * @tc.desc Verify Bluetooth API ac off stateChange condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_off_stateChange_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        access.off('stateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_OFF_STATECHANGE_PERF
     * @tc.name ac_off_stateChange_perf
     * @tc.desc Verify Bluetooth apitest case ac off stateChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_off_stateChange_perf', 0, () => {
      runPerf('ac_off_stateChange_perf', 5000, () => {
        try {
          access.off('stateChange');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_OFF_STATECHANGE_STRESS
     * @tc.name ac_off_stateChange_stress
     * @tc.desc Verify Bluetooth apitest case ac off stateChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_off_stateChange_stress', 0, () => {
      runStress('ac_off_stateChange_stress', 20, () => {
        try {
          access.off('stateChange');
        } catch (_e) {
        }
      });
    });
    });
    describe('ac_factoryReset_cb', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_CB_N
     * @tc.name ac_factoryReset_cb_n
     * @tc.desc Verify Bluetooth API ac factoryReset cb normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_cb_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        access.factoryReset(() => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_CB_E
     * @tc.name ac_factoryReset_cb_e
     * @tc.desc Verify Bluetooth API ac factoryReset cb error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_cb_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        access.factoryReset(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_CB_B
     * @tc.name ac_factoryReset_cb_b
     * @tc.desc Verify Bluetooth API ac factoryReset cb boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_cb_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        access.factoryReset(() => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_CB_R
     * @tc.name ac_factoryReset_cb_r
     * @tc.desc Verify Bluetooth API ac factoryReset cb return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_cb_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        access.factoryReset(() => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_CB_X
     * @tc.name ac_factoryReset_cb_x
     * @tc.desc Verify Bluetooth API ac factoryReset cb repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_cb_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        access.factoryReset(() => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        access.factoryReset(() => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_CB_C
     * @tc.name ac_factoryReset_cb_c
     * @tc.desc Verify Bluetooth API ac factoryReset cb condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_cb_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        access.factoryReset(() => {});
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_CB_PERF
     * @tc.name ac_factoryReset_cb_perf
     * @tc.desc Verify Bluetooth apitest case ac factoryReset cb perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_cb_perf', 0, () => {
      runPerf('ac_factoryReset_cb_perf', 5000, () => {
        try {
          access.factoryReset(() => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_CB_STRESS
     * @tc.name ac_factoryReset_cb_stress
     * @tc.desc Verify Bluetooth apitest case ac factoryReset cb stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_cb_stress', 0, () => {
      runStress('ac_factoryReset_cb_stress', 20, () => {
        try {
          access.factoryReset(() => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('ac_factoryReset_p', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_P_N
     * @tc.name ac_factoryReset_p_n
     * @tc.desc Verify Bluetooth API ac factoryReset p normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_p_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        access.factoryReset();
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_P_E
     * @tc.name ac_factoryReset_p_e
     * @tc.desc Verify Bluetooth API ac factoryReset p error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_p_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        access.factoryReset();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_P_B
     * @tc.name ac_factoryReset_p_b
     * @tc.desc Verify Bluetooth API ac factoryReset p boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_p_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        access.factoryReset();
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_P_R
     * @tc.name ac_factoryReset_p_r
     * @tc.desc Verify Bluetooth API ac factoryReset p return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_p_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        access.factoryReset();
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_P_X
     * @tc.name ac_factoryReset_p_x
     * @tc.desc Verify Bluetooth API ac factoryReset p repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_p_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        access.factoryReset();
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        access.factoryReset();
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'x2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_P_C
     * @tc.name ac_factoryReset_p_c
     * @tc.desc Verify Bluetooth API ac factoryReset p condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_p_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        access.factoryReset();
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_P_PERF
     * @tc.name ac_factoryReset_p_perf
     * @tc.desc Verify Bluetooth apitest case ac factoryReset p perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_p_perf', 0, () => {
      runPerf('ac_factoryReset_p_perf', 5000, () => {
        try {
          access.factoryReset();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_FACTORYRESET_P_STRESS
     * @tc.name ac_factoryReset_p_stress
     * @tc.desc Verify Bluetooth apitest case ac factoryReset p stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_factoryReset_p_stress', 0, () => {
      runStress('ac_factoryReset_p_stress', 20, () => {
        try {
          access.factoryReset();
        } catch (_e) {
        }
      });
    });
    });
    describe('ac_getLocalAddress', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_GETLOCALADDRESS_N
     * @tc.name ac_getLocalAddress_n
     * @tc.desc Verify Bluetooth API ac getLocalAddress normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_getLocalAddress_n', 0, () => {
      let errCode = 0;
      let value = '';
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = access.getLocalAddress();
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
        expect(typeof value).assertEqual('string');
        expect(value.length >= 0).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_GETLOCALADDRESS_E
     * @tc.name ac_getLocalAddress_e
     * @tc.desc Verify Bluetooth API ac getLocalAddress error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_getLocalAddress_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        access.getLocalAddress();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_GETLOCALADDRESS_B
     * @tc.name ac_getLocalAddress_b
     * @tc.desc Verify Bluetooth API ac getLocalAddress boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_getLocalAddress_b', 0, () => {
      let errCode = 0;
      let value = '';
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        value = access.getLocalAddress();
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
        expect(typeof value).assertEqual('string');
        expect(value.length >= 0).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_GETLOCALADDRESS_R
     * @tc.name ac_getLocalAddress_r
     * @tc.desc Verify Bluetooth API ac getLocalAddress return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_getLocalAddress_r', 0, () => {
      let errCode = 0;
      let value = '';
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = access.getLocalAddress();
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
        expect(typeof value).assertEqual('string');
        expect(value.length >= 0).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_GETLOCALADDRESS_X
     * @tc.name ac_getLocalAddress_x
     * @tc.desc Verify Bluetooth API ac getLocalAddress repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_getLocalAddress_x', 0, () => {
      let errCode = 0;
      let value = '';
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = access.getLocalAddress();
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
        expect(typeof value).assertEqual('string');
        expect(value.length >= 0).assertTrue();
      }
      errCode = 0;
      value = '';
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        value = access.getLocalAddress();
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
        expect(typeof value).assertEqual('string');
        expect(value.length >= 0).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_GETLOCALADDRESS_C
     * @tc.name ac_getLocalAddress_c
     * @tc.desc Verify Bluetooth API ac getLocalAddress condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_getLocalAddress_c', 0, () => {
      let errCode = 0;
      let value = '';
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = access.getLocalAddress();
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
        expect(typeof value).assertEqual('string');
        expect(value.length >= 0).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_GETLOCALADDRESS_PERF
     * @tc.name ac_getLocalAddress_perf
     * @tc.desc Verify Bluetooth apitest case ac getLocalAddress perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_getLocalAddress_perf', 0, () => {
      runPerf('ac_getLocalAddress_perf', 5000, () => {
        try {
          access.getLocalAddress();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_GETLOCALADDRESS_STRESS
     * @tc.name ac_getLocalAddress_stress
     * @tc.desc Verify Bluetooth apitest case ac getLocalAddress stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_getLocalAddress_stress', 0, () => {
      runStress('ac_getLocalAddress_stress', 20, () => {
        try {
          access.getLocalAddress();
        } catch (_e) {
        }
      });
    });
    });
    describe('ac_restrictBluetooth', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_RESTRICTBLUETOOTH_N
     * @tc.name ac_restrictBluetooth_n
     * @tc.desc Verify Bluetooth API ac restrictBluetooth normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_restrictBluetooth_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = access.restrictBluetooth() as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_RESTRICTBLUETOOTH_E
     * @tc.name ac_restrictBluetooth_e
     * @tc.desc Verify Bluetooth API ac restrictBluetooth error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_restrictBluetooth_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        access.restrictBluetooth();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_RESTRICTBLUETOOTH_B
     * @tc.name ac_restrictBluetooth_b
     * @tc.desc Verify Bluetooth API ac restrictBluetooth boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_restrictBluetooth_b', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        value = access.restrictBluetooth() as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_RESTRICTBLUETOOTH_R
     * @tc.name ac_restrictBluetooth_r
     * @tc.desc Verify Bluetooth API ac restrictBluetooth return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_restrictBluetooth_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = access.restrictBluetooth() as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_RESTRICTBLUETOOTH_X
     * @tc.name ac_restrictBluetooth_x
     * @tc.desc Verify Bluetooth API ac restrictBluetooth repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_restrictBluetooth_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = access.restrictBluetooth() as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
      errCode = 0;
      value = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        value = access.restrictBluetooth() as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_RESTRICTBLUETOOTH_C
     * @tc.name ac_restrictBluetooth_c
     * @tc.desc Verify Bluetooth API ac restrictBluetooth condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_restrictBluetooth_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = access.restrictBluetooth() as Object;
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
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_RESTRICTBLUETOOTH_PERF
     * @tc.name ac_restrictBluetooth_perf
     * @tc.desc Verify Bluetooth apitest case ac restrictBluetooth perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_restrictBluetooth_perf', 0, () => {
      runPerf('ac_restrictBluetooth_perf', 5000, () => {
        try {
          access.restrictBluetooth() as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_RESTRICTBLUETOOTH_STRESS
     * @tc.name ac_restrictBluetooth_stress
     * @tc.desc Verify Bluetooth apitest case ac restrictBluetooth stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_restrictBluetooth_stress', 0, () => {
      runStress('ac_restrictBluetooth_stress', 20, () => {
        try {
          access.restrictBluetooth() as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('ac_notifyDialogResult', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_NOTIFYDIALOGRESULT_N
     * @tc.name ac_notifyDialogResult_n
     * @tc.desc Verify Bluetooth API ac notifyDialogResult normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_notifyDialogResult_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = access.notifyDialogResult({ dialogType: 0, dialogResult: true }) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_NOTIFYDIALOGRESULT_E
     * @tc.name ac_notifyDialogResult_e
     * @tc.desc Verify Bluetooth API ac notifyDialogResult error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_notifyDialogResult_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        access.notifyDialogResult({ dialogType: -1, dialogResult: false });
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_NOTIFYDIALOGRESULT_B
     * @tc.name ac_notifyDialogResult_b
     * @tc.desc Verify Bluetooth API ac notifyDialogResult boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_notifyDialogResult_b', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        value = access.notifyDialogResult({ dialogType: 0, dialogResult: false }) as Object;
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(value !== null).assertTrue();
        expect(typeof (value as ESObject).then).assertEqual('function');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_NOTIFYDIALOGRESULT_R
     * @tc.name ac_notifyDialogResult_r
     * @tc.desc Verify Bluetooth API ac notifyDialogResult return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_notifyDialogResult_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = access.notifyDialogResult({ dialogType: 0, dialogResult: true }) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_NOTIFYDIALOGRESULT_X
     * @tc.name ac_notifyDialogResult_x
     * @tc.desc Verify Bluetooth API ac notifyDialogResult repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_notifyDialogResult_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = access.notifyDialogResult({ dialogType: 0, dialogResult: true }) as Object;
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
        value = access.notifyDialogResult({ dialogType: 0, dialogResult: true }) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_NOTIFYDIALOGRESULT_C
     * @tc.name ac_notifyDialogResult_c
     * @tc.desc Verify Bluetooth API ac notifyDialogResult condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_notifyDialogResult_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = access.notifyDialogResult({ dialogType: 0, dialogResult: false }) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_NOTIFYDIALOGRESULT_PERF
     * @tc.name ac_notifyDialogResult_perf
     * @tc.desc Verify Bluetooth apitest case ac notifyDialogResult perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_notifyDialogResult_perf', 0, () => {
      runPerf('ac_notifyDialogResult_perf', 5000, () => {
        try {
          access.notifyDialogResult({ dialogType: 0, dialogResult: true }) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_AC_NOTIFYDIALOGRESULT_STRESS
     * @tc.name ac_notifyDialogResult_stress
     * @tc.desc Verify Bluetooth apitest case ac notifyDialogResult stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ac_notifyDialogResult_stress', 0, () => {
      runStress('ac_notifyDialogResult_stress', 20, () => {
        try {
          access.notifyDialogResult({ dialogType: 0, dialogResult: true }) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('wd_enableWearDetection', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ENABLEWEARDETECTION_N
     * @tc.name wd_enableWearDetection_n
     * @tc.desc Verify Bluetooth API wd enableWearDetection normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_enableWearDetection_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        wearDetection.enableWearDetection(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ENABLEWEARDETECTION_E
     * @tc.name wd_enableWearDetection_e
     * @tc.desc Verify Bluetooth API wd enableWearDetection error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_enableWearDetection_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        wearDetection.enableWearDetection(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ENABLEWEARDETECTION_B
     * @tc.name wd_enableWearDetection_b
     * @tc.desc Verify Bluetooth API wd enableWearDetection boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_enableWearDetection_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        wearDetection.enableWearDetection(BAD_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ENABLEWEARDETECTION_R
     * @tc.name wd_enableWearDetection_r
     * @tc.desc Verify Bluetooth API wd enableWearDetection return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_enableWearDetection_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        wearDetection.enableWearDetection(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ENABLEWEARDETECTION_X
     * @tc.name wd_enableWearDetection_x
     * @tc.desc Verify Bluetooth API wd enableWearDetection repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_enableWearDetection_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        wearDetection.enableWearDetection(BADDR);
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
        wearDetection.enableWearDetection(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ENABLEWEARDETECTION_C
     * @tc.name wd_enableWearDetection_c
     * @tc.desc Verify Bluetooth API wd enableWearDetection condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_enableWearDetection_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        wearDetection.enableWearDetection(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ENABLEWEARDETECTION_PERF
     * @tc.name wd_enableWearDetection_perf
     * @tc.desc Verify Bluetooth apitest case wd enableWearDetection perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_enableWearDetection_perf', 0, () => {
      runPerf('wd_enableWearDetection_perf', 5000, () => {
        try {
          wearDetection.enableWearDetection(BADDR);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_ENABLEWEARDETECTION_STRESS
     * @tc.name wd_enableWearDetection_stress
     * @tc.desc Verify Bluetooth apitest case wd enableWearDetection stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_enableWearDetection_stress', 0, () => {
      runStress('wd_enableWearDetection_stress', 20, () => {
        try {
          wearDetection.enableWearDetection(BADDR);
        } catch (_e) {
        }
      });
    });
    });
    describe('wd_disableWearDetection', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_DISABLEWEARDETECTION_N
     * @tc.name wd_disableWearDetection_n
     * @tc.desc Verify Bluetooth API wd disableWearDetection normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_disableWearDetection_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        wearDetection.disableWearDetection(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_DISABLEWEARDETECTION_E
     * @tc.name wd_disableWearDetection_e
     * @tc.desc Verify Bluetooth API wd disableWearDetection error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_disableWearDetection_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        wearDetection.disableWearDetection(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_DISABLEWEARDETECTION_B
     * @tc.name wd_disableWearDetection_b
     * @tc.desc Verify Bluetooth API wd disableWearDetection boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_disableWearDetection_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        wearDetection.disableWearDetection(BAD_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_DISABLEWEARDETECTION_R
     * @tc.name wd_disableWearDetection_r
     * @tc.desc Verify Bluetooth API wd disableWearDetection return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_disableWearDetection_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        wearDetection.disableWearDetection(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_WD_DISABLEWEARDETECTION_X
     * @tc.name wd_disableWearDetection_x
     * @tc.desc Verify Bluetooth API wd disableWearDetection repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('wd_disableWearDetection_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        wearDetection.disableWearDetection(BADDR);
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


