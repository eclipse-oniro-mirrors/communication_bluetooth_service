        hilog.error(0x0000, 'BtFullCoverage', 'x1: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertPeerExpect(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        bluetoothManager.BLE.stopBLEScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STOPBLESCAN_C
     * @tc.name leg_btm_stopBLEScan_c
     * @tc.desc Verify Bluetooth API leg btm stopBLEScan condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_stopBLEScan_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.BLE.stopBLEScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STOPBLESCAN_PERF
     * @tc.name leg_btm_stopBLEScan_perf
     * @tc.desc Verify Bluetooth apitest case leg btm stopBLEScan perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_stopBLEScan_perf', 0, () => {
      runPerf('leg_btm_stopBLEScan_perf', 5000, () => {
        try {
          bluetoothManager.BLE.stopBLEScan();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STOPBLESCAN_STRESS
     * @tc.name leg_btm_stopBLEScan_stress
     * @tc.desc Verify Bluetooth apitest case leg btm stopBLEScan stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_stopBLEScan_stress', 0, () => {
      runStress('leg_btm_stopBLEScan_stress', 20, () => {
        try {
          bluetoothManager.BLE.stopBLEScan();
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_on_bondStateChange', () => {
    it('leg_btm_on_bondStateChange_n', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.on('bondStateChange', () => {});
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
    it('leg_btm_on_bondStateChange_e', 0, async (done: Function) => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetoothManager.on('bondStateChange', () => {});
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
    it('leg_btm_on_bondStateChange_b', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetoothManager.on('bondStateChange', () => {});
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
    it('leg_btm_on_bondStateChange_r', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetoothManager.on('bondStateChange', () => {});
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
    it('leg_btm_on_bondStateChange_x', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetoothManager.on('bondStateChange', () => {});
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
        bluetoothManager.on('bondStateChange', () => {});
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
    it('leg_btm_on_bondStateChange_c', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.on('bondStateChange', () => {});
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

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_ON_BONDSTATECHANGE_PERF
     * @tc.name leg_btm_on_bondStateChange_perf
     * @tc.desc Verify Bluetooth apitest case leg btm on bondStateChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_on_bondStateChange_perf', 0, () => {
      runPerf('leg_btm_on_bondStateChange_perf', 5000, () => {
        try {
          bluetoothManager.on('bondStateChange', () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_ON_BONDSTATECHANGE_STRESS
     * @tc.name leg_btm_on_bondStateChange_stress
     * @tc.desc Verify Bluetooth apitest case leg btm on bondStateChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_on_bondStateChange_stress', 0, () => {
      runStress('leg_btm_on_bondStateChange_stress', 20, () => {
        try {
          bluetoothManager.on('bondStateChange', () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_off_bondStateChange', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BONDSTATECHANGE_N
     * @tc.name leg_btm_off_bondStateChange_n
     * @tc.desc Verify Bluetooth API leg btm off bondStateChange normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bondStateChange_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.off('bondStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BONDSTATECHANGE_E
     * @tc.name leg_btm_off_bondStateChange_e
     * @tc.desc Verify Bluetooth API leg btm off bondStateChange error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bondStateChange_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetoothManager.off('bondStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BONDSTATECHANGE_B
     * @tc.name leg_btm_off_bondStateChange_b
     * @tc.desc Verify Bluetooth API leg btm off bondStateChange boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bondStateChange_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetoothManager.off('bondStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BONDSTATECHANGE_R
     * @tc.name leg_btm_off_bondStateChange_r
     * @tc.desc Verify Bluetooth API leg btm off bondStateChange return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bondStateChange_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetoothManager.off('bondStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BONDSTATECHANGE_X
     * @tc.name leg_btm_off_bondStateChange_x
     * @tc.desc Verify Bluetooth API leg btm off bondStateChange repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bondStateChange_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetoothManager.off('bondStateChange');
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
        bluetoothManager.off('bondStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BONDSTATECHANGE_C
     * @tc.name leg_btm_off_bondStateChange_c
     * @tc.desc Verify Bluetooth API leg btm off bondStateChange condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bondStateChange_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.off('bondStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BONDSTATECHANGE_PERF
     * @tc.name leg_btm_off_bondStateChange_perf
     * @tc.desc Verify Bluetooth apitest case leg btm off bondStateChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bondStateChange_perf', 0, () => {
      runPerf('leg_btm_off_bondStateChange_perf', 5000, () => {
        try {
          bluetoothManager.off('bondStateChange');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BONDSTATECHANGE_STRESS
     * @tc.name leg_btm_off_bondStateChange_stress
     * @tc.desc Verify Bluetooth apitest case leg btm off bondStateChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bondStateChange_stress', 0, () => {
      runStress('leg_btm_off_bondStateChange_stress', 20, () => {
        try {
          bluetoothManager.off('bondStateChange');
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_on_bluetoothDeviceFind', () => {
    it('leg_btm_on_bluetoothDeviceFind_n', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.on('bluetoothDeviceFind', () => {});
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
    it('leg_btm_on_bluetoothDeviceFind_e', 0, async (done: Function) => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetoothManager.on('bluetoothDeviceFind', () => {});
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
    it('leg_btm_on_bluetoothDeviceFind_b', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetoothManager.on('bluetoothDeviceFind', () => {});
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
    it('leg_btm_on_bluetoothDeviceFind_r', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetoothManager.on('bluetoothDeviceFind', () => {});
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
    it('leg_btm_on_bluetoothDeviceFind_x', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetoothManager.on('bluetoothDeviceFind', () => {});
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
        bluetoothManager.on('bluetoothDeviceFind', () => {});
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
    it('leg_btm_on_bluetoothDeviceFind_c', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.on('bluetoothDeviceFind', () => {});
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

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_ON_BLUETOOTHDEVICEFIND_PERF
     * @tc.name leg_btm_on_bluetoothDeviceFind_perf
     * @tc.desc Verify Bluetooth apitest case leg btm on bluetoothDeviceFind perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_on_bluetoothDeviceFind_perf', 0, () => {
      runPerf('leg_btm_on_bluetoothDeviceFind_perf', 5000, () => {
        try {
          bluetoothManager.on('bluetoothDeviceFind', () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_ON_BLUETOOTHDEVICEFIND_STRESS
     * @tc.name leg_btm_on_bluetoothDeviceFind_stress
     * @tc.desc Verify Bluetooth apitest case leg btm on bluetoothDeviceFind stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_on_bluetoothDeviceFind_stress', 0, () => {
      runStress('leg_btm_on_bluetoothDeviceFind_stress', 20, () => {
        try {
          bluetoothManager.on('bluetoothDeviceFind', () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_off_bluetoothDeviceFind', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLUETOOTHDEVICEFIND_N
     * @tc.name leg_btm_off_bluetoothDeviceFind_n
     * @tc.desc Verify Bluetooth API leg btm off bluetoothDeviceFind normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bluetoothDeviceFind_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.off('bluetoothDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLUETOOTHDEVICEFIND_E
     * @tc.name leg_btm_off_bluetoothDeviceFind_e
     * @tc.desc Verify Bluetooth API leg btm off bluetoothDeviceFind error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bluetoothDeviceFind_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetoothManager.off('bluetoothDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLUETOOTHDEVICEFIND_B
     * @tc.name leg_btm_off_bluetoothDeviceFind_b
     * @tc.desc Verify Bluetooth API leg btm off bluetoothDeviceFind boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bluetoothDeviceFind_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetoothManager.off('bluetoothDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLUETOOTHDEVICEFIND_R
     * @tc.name leg_btm_off_bluetoothDeviceFind_r
     * @tc.desc Verify Bluetooth API leg btm off bluetoothDeviceFind return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bluetoothDeviceFind_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetoothManager.off('bluetoothDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLUETOOTHDEVICEFIND_X
     * @tc.name leg_btm_off_bluetoothDeviceFind_x
     * @tc.desc Verify Bluetooth API leg btm off bluetoothDeviceFind repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bluetoothDeviceFind_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetoothManager.off('bluetoothDeviceFind');
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
        bluetoothManager.off('bluetoothDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLUETOOTHDEVICEFIND_C
     * @tc.name leg_btm_off_bluetoothDeviceFind_c
     * @tc.desc Verify Bluetooth API leg btm off bluetoothDeviceFind condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bluetoothDeviceFind_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.off('bluetoothDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLUETOOTHDEVICEFIND_PERF
     * @tc.name leg_btm_off_bluetoothDeviceFind_perf
     * @tc.desc Verify Bluetooth apitest case leg btm off bluetoothDeviceFind perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bluetoothDeviceFind_perf', 0, () => {
      runPerf('leg_btm_off_bluetoothDeviceFind_perf', 5000, () => {
        try {
          bluetoothManager.off('bluetoothDeviceFind');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLUETOOTHDEVICEFIND_STRESS
     * @tc.name leg_btm_off_bluetoothDeviceFind_stress
     * @tc.desc Verify Bluetooth apitest case leg btm off bluetoothDeviceFind stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_bluetoothDeviceFind_stress', 0, () => {
      runStress('leg_btm_off_bluetoothDeviceFind_stress', 20, () => {
        try {
          bluetoothManager.off('bluetoothDeviceFind');
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_on_BLEDeviceFind', () => {
    it('leg_btm_on_BLEDeviceFind_n', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.BLE.on('BLEDeviceFind', () => {});
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
    it('leg_btm_on_BLEDeviceFind_e', 0, async (done: Function) => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetoothManager.BLE.on('BLEDeviceFind', () => {});
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
    it('leg_btm_on_BLEDeviceFind_b', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetoothManager.BLE.on('BLEDeviceFind', () => {});
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
    it('leg_btm_on_BLEDeviceFind_r', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetoothManager.BLE.on('BLEDeviceFind', () => {});
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
    it('leg_btm_on_BLEDeviceFind_x', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetoothManager.BLE.on('BLEDeviceFind', () => {});
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
        bluetoothManager.BLE.on('BLEDeviceFind', () => {});
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
    it('leg_btm_on_BLEDeviceFind_c', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.BLE.on('BLEDeviceFind', () => {});
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

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_ON_BLEDEVICEFIND_PERF
     * @tc.name leg_btm_on_BLEDeviceFind_perf
     * @tc.desc Verify Bluetooth apitest case leg btm on BLEDeviceFind perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_on_BLEDeviceFind_perf', 0, () => {
      runPerf('leg_btm_on_BLEDeviceFind_perf', 5000, () => {
        try {
          bluetoothManager.BLE.on('BLEDeviceFind', () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_ON_BLEDEVICEFIND_STRESS
     * @tc.name leg_btm_on_BLEDeviceFind_stress
     * @tc.desc Verify Bluetooth apitest case leg btm on BLEDeviceFind stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_on_BLEDeviceFind_stress', 0, () => {
      runStress('leg_btm_on_BLEDeviceFind_stress', 20, () => {
        try {
          bluetoothManager.BLE.on('BLEDeviceFind', () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_off_BLEDeviceFind', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLEDEVICEFIND_N
     * @tc.name leg_btm_off_BLEDeviceFind_n
     * @tc.desc Verify Bluetooth API leg btm off BLEDeviceFind normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_BLEDeviceFind_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.BLE.off('BLEDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLEDEVICEFIND_E
     * @tc.name leg_btm_off_BLEDeviceFind_e
     * @tc.desc Verify Bluetooth API leg btm off BLEDeviceFind error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_BLEDeviceFind_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetoothManager.BLE.off('BLEDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLEDEVICEFIND_B
     * @tc.name leg_btm_off_BLEDeviceFind_b
     * @tc.desc Verify Bluetooth API leg btm off BLEDeviceFind boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_BLEDeviceFind_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetoothManager.BLE.off('BLEDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLEDEVICEFIND_R
     * @tc.name leg_btm_off_BLEDeviceFind_r
     * @tc.desc Verify Bluetooth API leg btm off BLEDeviceFind return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_BLEDeviceFind_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetoothManager.BLE.off('BLEDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLEDEVICEFIND_X
     * @tc.name leg_btm_off_BLEDeviceFind_x
     * @tc.desc Verify Bluetooth API leg btm off BLEDeviceFind repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_BLEDeviceFind_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetoothManager.BLE.off('BLEDeviceFind');
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
        bluetoothManager.BLE.off('BLEDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLEDEVICEFIND_C
     * @tc.name leg_btm_off_BLEDeviceFind_c
     * @tc.desc Verify Bluetooth API leg btm off BLEDeviceFind condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_BLEDeviceFind_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.BLE.off('BLEDeviceFind');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLEDEVICEFIND_PERF
     * @tc.name leg_btm_off_BLEDeviceFind_perf
     * @tc.desc Verify Bluetooth apitest case leg btm off BLEDeviceFind perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_BLEDeviceFind_perf', 0, () => {
      runPerf('leg_btm_off_BLEDeviceFind_perf', 5000, () => {
        try {
          bluetoothManager.BLE.off('BLEDeviceFind');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_BLEDEVICEFIND_STRESS
     * @tc.name leg_btm_off_BLEDeviceFind_stress
     * @tc.desc Verify Bluetooth apitest case leg btm off BLEDeviceFind stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_BLEDeviceFind_stress', 0, () => {
      runStress('leg_btm_off_BLEDeviceFind_stress', 20, () => {
        try {
          bluetoothManager.BLE.off('BLEDeviceFind');
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_on_pinRequired', () => {
    it('leg_btm_on_pinRequired_n', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.on('pinRequired', () => {});
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
    it('leg_btm_on_pinRequired_e', 0, async (done: Function) => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetoothManager.on('pinRequired', () => {});
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
    it('leg_btm_on_pinRequired_b', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetoothManager.on('pinRequired', () => {});
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
    it('leg_btm_on_pinRequired_r', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetoothManager.on('pinRequired', () => {});
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
    it('leg_btm_on_pinRequired_x', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetoothManager.on('pinRequired', () => {});
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
        bluetoothManager.on('pinRequired', () => {});
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
    it('leg_btm_on_pinRequired_c', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.on('pinRequired', () => {});
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

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_ON_PINREQUIRED_PERF
     * @tc.name leg_btm_on_pinRequired_perf
     * @tc.desc Verify Bluetooth apitest case leg btm on pinRequired perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_on_pinRequired_perf', 0, () => {
      runPerf('leg_btm_on_pinRequired_perf', 5000, () => {
        try {
          bluetoothManager.on('pinRequired', () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_ON_PINREQUIRED_STRESS
     * @tc.name leg_btm_on_pinRequired_stress
     * @tc.desc Verify Bluetooth apitest case leg btm on pinRequired stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_on_pinRequired_stress', 0, () => {
      runStress('leg_btm_on_pinRequired_stress', 20, () => {
        try {
          bluetoothManager.on('pinRequired', () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_off_pinRequired', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_PINREQUIRED_N
     * @tc.name leg_btm_off_pinRequired_n
     * @tc.desc Verify Bluetooth API leg btm off pinRequired normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_pinRequired_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.off('pinRequired');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_PINREQUIRED_E
     * @tc.name leg_btm_off_pinRequired_e
     * @tc.desc Verify Bluetooth API leg btm off pinRequired error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_pinRequired_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetoothManager.off('pinRequired');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_PINREQUIRED_B
     * @tc.name leg_btm_off_pinRequired_b
     * @tc.desc Verify Bluetooth API leg btm off pinRequired boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_pinRequired_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetoothManager.off('pinRequired');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_PINREQUIRED_R
     * @tc.name leg_btm_off_pinRequired_r
     * @tc.desc Verify Bluetooth API leg btm off pinRequired return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_pinRequired_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetoothManager.off('pinRequired');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_PINREQUIRED_X
     * @tc.name leg_btm_off_pinRequired_x
     * @tc.desc Verify Bluetooth API leg btm off pinRequired repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_pinRequired_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetoothManager.off('pinRequired');
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
        bluetoothManager.off('pinRequired');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_PINREQUIRED_C
     * @tc.name leg_btm_off_pinRequired_c
     * @tc.desc Verify Bluetooth API leg btm off pinRequired condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_pinRequired_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.off('pinRequired');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_PINREQUIRED_PERF
     * @tc.name leg_btm_off_pinRequired_perf
     * @tc.desc Verify Bluetooth apitest case leg btm off pinRequired perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_pinRequired_perf', 0, () => {
      runPerf('leg_btm_off_pinRequired_perf', 5000, () => {
        try {
          bluetoothManager.off('pinRequired');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_PINREQUIRED_STRESS
     * @tc.name leg_btm_off_pinRequired_stress
     * @tc.desc Verify Bluetooth apitest case leg btm off pinRequired stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_pinRequired_stress', 0, () => {
      runStress('leg_btm_off_pinRequired_stress', 20, () => {
        try {
          bluetoothManager.off('pinRequired');
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_on_connectStateChange', () => {
    it('leg_btm_on_connectStateChange_n', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.BLE.createGattServer();
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
    it('leg_btm_on_connectStateChange_e', 0, async (done: Function) => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetoothManager.BLE.createGattServer();
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
    it('leg_btm_on_connectStateChange_b', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetoothManager.BLE.createGattServer();
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
    it('leg_btm_on_connectStateChange_r', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetoothManager.BLE.createGattServer();
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
    it('leg_btm_on_connectStateChange_x', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetoothManager.BLE.createGattServer();
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
        bluetoothManager.BLE.createGattServer();
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
    it('leg_btm_on_connectStateChange_c', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.BLE.createGattServer();
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

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_ON_CONNECTSTATECHANGE_PERF
     * @tc.name leg_btm_on_connectStateChange_perf
     * @tc.desc Verify Bluetooth apitest case leg btm on connectStateChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_on_connectStateChange_perf', 0, () => {
      runPerf('leg_btm_on_connectStateChange_perf', 5000, () => {
        try {
          bluetoothManager.BLE.createGattServer();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_ON_CONNECTSTATECHANGE_STRESS
     * @tc.name leg_btm_on_connectStateChange_stress
     * @tc.desc Verify Bluetooth apitest case leg btm on connectStateChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_on_connectStateChange_stress', 0, () => {
      runStress('leg_btm_on_connectStateChange_stress', 20, () => {
        try {
          bluetoothManager.BLE.createGattServer();
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_off_connectStateChange', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_CONNECTSTATECHANGE_N
     * @tc.name leg_btm_off_connectStateChange_n
     * @tc.desc Verify Bluetooth API leg btm off connectStateChange normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_connectStateChange_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.BLE.createGattServer();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_CONNECTSTATECHANGE_E
     * @tc.name leg_btm_off_connectStateChange_e
     * @tc.desc Verify Bluetooth API leg btm off connectStateChange error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_connectStateChange_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetoothManager.BLE.createGattServer();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_CONNECTSTATECHANGE_B
     * @tc.name leg_btm_off_connectStateChange_b
     * @tc.desc Verify Bluetooth API leg btm off connectStateChange boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_connectStateChange_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetoothManager.BLE.createGattServer();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_CONNECTSTATECHANGE_R
     * @tc.name leg_btm_off_connectStateChange_r
     * @tc.desc Verify Bluetooth API leg btm off connectStateChange return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_connectStateChange_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetoothManager.BLE.createGattServer();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_CONNECTSTATECHANGE_X
     * @tc.name leg_btm_off_connectStateChange_x
     * @tc.desc Verify Bluetooth API leg btm off connectStateChange repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_connectStateChange_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetoothManager.BLE.createGattServer();
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
        bluetoothManager.BLE.createGattServer();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_CONNECTSTATECHANGE_C
     * @tc.name leg_btm_off_connectStateChange_c
     * @tc.desc Verify Bluetooth API leg btm off connectStateChange condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_connectStateChange_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.BLE.createGattServer();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_CONNECTSTATECHANGE_PERF
     * @tc.name leg_btm_off_connectStateChange_perf
     * @tc.desc Verify Bluetooth apitest case leg btm off connectStateChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_connectStateChange_perf', 0, () => {
      runPerf('leg_btm_off_connectStateChange_perf', 5000, () => {
        try {
          bluetoothManager.BLE.createGattServer();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_CONNECTSTATECHANGE_STRESS
     * @tc.name leg_btm_off_connectStateChange_stress
     * @tc.desc Verify Bluetooth apitest case leg btm off connectStateChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_connectStateChange_stress', 0, () => {
      runStress('leg_btm_off_connectStateChange_stress', 20, () => {
        try {
          bluetoothManager.BLE.createGattServer();
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_on_sppRead', () => {
    it('leg_btm_on_sppRead_n', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.on('sppRead', 0, () => {});
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
    it('leg_btm_on_sppRead_e', 0, async (done: Function) => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetoothManager.on('sppRead', -1, () => {});
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
    it('leg_btm_on_sppRead_b', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetoothManager.on('sppRead', 0, () => {});
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
    it('leg_btm_on_sppRead_r', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetoothManager.on('sppRead', 0, () => {});
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
    it('leg_btm_on_sppRead_x', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetoothManager.on('sppRead', 0, () => {});
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
        bluetoothManager.on('sppRead', 0, () => {});
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
    it('leg_btm_on_sppRead_c', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.on('sppRead', 0, () => {});
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

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_ON_SPPREAD_PERF
     * @tc.name leg_btm_on_sppRead_perf
     * @tc.desc Verify Bluetooth apitest case leg btm on sppRead perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_on_sppRead_perf', 0, () => {
      runPerf('leg_btm_on_sppRead_perf', 5000, () => {
        try {
          bluetoothManager.on('sppRead', 0, () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_ON_SPPREAD_STRESS
     * @tc.name leg_btm_on_sppRead_stress
     * @tc.desc Verify Bluetooth apitest case leg btm on sppRead stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_on_sppRead_stress', 0, () => {
      runStress('leg_btm_on_sppRead_stress', 20, () => {
        try {
          bluetoothManager.on('sppRead', 0, () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_off_sppRead', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_SPPREAD_N
     * @tc.name leg_btm_off_sppRead_n
     * @tc.desc Verify Bluetooth API leg btm off sppRead normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_sppRead_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.off('sppRead', 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_SPPREAD_E
     * @tc.name leg_btm_off_sppRead_e
     * @tc.desc Verify Bluetooth API leg btm off sppRead error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_sppRead_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetoothManager.off('sppRead', -1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_SPPREAD_B
     * @tc.name leg_btm_off_sppRead_b
     * @tc.desc Verify Bluetooth API leg btm off sppRead boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_sppRead_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetoothManager.off('sppRead', 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_SPPREAD_R
     * @tc.name leg_btm_off_sppRead_r
     * @tc.desc Verify Bluetooth API leg btm off sppRead return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_sppRead_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetoothManager.off('sppRead', 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_SPPREAD_X
     * @tc.name leg_btm_off_sppRead_x
     * @tc.desc Verify Bluetooth API leg btm off sppRead repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_sppRead_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetoothManager.off('sppRead', 0);
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
        bluetoothManager.off('sppRead', 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_SPPREAD_C
     * @tc.name leg_btm_off_sppRead_c
     * @tc.desc Verify Bluetooth API leg btm off sppRead condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_sppRead_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.off('sppRead', 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_SPPREAD_PERF
     * @tc.name leg_btm_off_sppRead_perf
     * @tc.desc Verify Bluetooth apitest case leg btm off sppRead perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_sppRead_perf', 0, () => {
      runPerf('leg_btm_off_sppRead_perf', 5000, () => {
        try {
          bluetoothManager.off('sppRead', 0);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_OFF_SPPREAD_STRESS
     * @tc.name leg_btm_off_sppRead_stress
     * @tc.desc Verify Bluetooth apitest case leg btm off sppRead stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_off_sppRead_stress', 0, () => {
      runStress('leg_btm_off_sppRead_stress', 20, () => {
        try {
          bluetoothManager.off('sppRead', 0);
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_getProfileInstance', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETPROFILEINSTANCE_N
     * @tc.name leg_btm_getProfileInstance_n
     * @tc.desc Verify Bluetooth API leg btm getProfileInstance normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getProfileInstance_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.getProfileInstance(1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETPROFILEINSTANCE_E
     * @tc.name leg_btm_getProfileInstance_e
     * @tc.desc Verify Bluetooth API leg btm getProfileInstance error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getProfileInstance_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetoothManager.getProfileInstance(-1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETPROFILEINSTANCE_B
     * @tc.name leg_btm_getProfileInstance_b
     * @tc.desc Verify Bluetooth API leg btm getProfileInstance boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getProfileInstance_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetoothManager.getProfileInstance(1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETPROFILEINSTANCE_R
     * @tc.name leg_btm_getProfileInstance_r
     * @tc.desc Verify Bluetooth API leg btm getProfileInstance return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getProfileInstance_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetoothManager.getProfileInstance(1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETPROFILEINSTANCE_X
     * @tc.name leg_btm_getProfileInstance_x
     * @tc.desc Verify Bluetooth API leg btm getProfileInstance repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getProfileInstance_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetoothManager.getProfileInstance(1);
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
        bluetoothManager.getProfileInstance(1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETPROFILEINSTANCE_C
     * @tc.name leg_btm_getProfileInstance_c
     * @tc.desc Verify Bluetooth API leg btm getProfileInstance condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getProfileInstance_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetoothManager.getProfileInstance(1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETPROFILEINSTANCE_PERF
     * @tc.name leg_btm_getProfileInstance_perf
     * @tc.desc Verify Bluetooth apitest case leg btm getProfileInstance perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getProfileInstance_perf', 0, () => {
      runPerf('leg_btm_getProfileInstance_perf', 5000, () => {
        try {
          bluetoothManager.getProfileInstance(1);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETPROFILEINSTANCE_STRESS
     * @tc.name leg_btm_getProfileInstance_stress
     * @tc.desc Verify Bluetooth apitest case leg btm getProfileInstance stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getProfileInstance_stress', 0, () => {
      runStress('leg_btm_getProfileInstance_stress', 20, () => {
        try {
          bluetoothManager.getProfileInstance(1);
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_getConnectedBLEDevices', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTEDBLEDEVICES_N
     * @tc.name leg_btm_getConnectedBLEDevices_n
     * @tc.desc Verify Bluetooth API leg btm getConnectedBLEDevices normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectedBLEDevices_n', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = bluetoothManager.BLE.getConnectedBLEDevices() as Array < Object > ;
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
        expect(Array.isArray(value)).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTEDBLEDEVICES_E
     * @tc.name leg_btm_getConnectedBLEDevices_e
     * @tc.desc Verify Bluetooth API leg btm getConnectedBLEDevices error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectedBLEDevices_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        bluetoothManager.BLE.getConnectedBLEDevices();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTEDBLEDEVICES_B
     * @tc.name leg_btm_getConnectedBLEDevices_b
     * @tc.desc Verify Bluetooth API leg btm getConnectedBLEDevices boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectedBLEDevices_b', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        value = bluetoothManager.BLE.getConnectedBLEDevices() as Array < Object > ;
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
        expect(Array.isArray(value)).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTEDBLEDEVICES_R
     * @tc.name leg_btm_getConnectedBLEDevices_r
     * @tc.desc Verify Bluetooth API leg btm getConnectedBLEDevices return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectedBLEDevices_r', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = bluetoothManager.BLE.getConnectedBLEDevices() as Array < Object > ;
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
        expect(Array.isArray(value)).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTEDBLEDEVICES_X
     * @tc.name leg_btm_getConnectedBLEDevices_x
     * @tc.desc Verify Bluetooth API leg btm getConnectedBLEDevices repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectedBLEDevices_x', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = bluetoothManager.BLE.getConnectedBLEDevices() as Array < Object > ;
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
        expect(Array.isArray(value)).assertTrue();
      }
      errCode = 0;
      value = [];
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        value = bluetoothManager.BLE.getConnectedBLEDevices() as Array < Object > ;
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
        expect(Array.isArray(value)).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTEDBLEDEVICES_C
     * @tc.name leg_btm_getConnectedBLEDevices_c
     * @tc.desc Verify Bluetooth API leg btm getConnectedBLEDevices condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectedBLEDevices_c', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = bluetoothManager.BLE.getConnectedBLEDevices() as Array < Object > ;
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
        expect(Array.isArray(value)).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTEDBLEDEVICES_PERF
     * @tc.name leg_btm_getConnectedBLEDevices_perf
     * @tc.desc Verify Bluetooth apitest case leg btm getConnectedBLEDevices perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectedBLEDevices_perf', 0, () => {
      runPerf('leg_btm_getConnectedBLEDevices_perf', 5000, () => {
        try {
          bluetoothManager.BLE.getConnectedBLEDevices() as Array < Object > ;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTEDBLEDEVICES_STRESS
     * @tc.name leg_btm_getConnectedBLEDevices_stress
     * @tc.desc Verify Bluetooth apitest case leg btm getConnectedBLEDevices stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectedBLEDevices_stress', 0, () => {
      runStress('leg_btm_getConnectedBLEDevices_stress', 20, () => {
        try {
          bluetoothManager.BLE.getConnectedBLEDevices() as Array < Object > ;
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_startAdvertising', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STARTADVERTISING_N
     * @tc.name leg_btm_startAdvertising_n
     * @tc.desc Verify Bluetooth API leg btm startAdvertising normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_startAdvertising_n', 0, () => {
      const gattServer = bluetoothManager.BLE.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattServer.startAdvertising(bleAs(), bleAd());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STARTADVERTISING_E
     * @tc.name leg_btm_startAdvertising_e
     * @tc.desc Verify Bluetooth API leg btm startAdvertising error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_startAdvertising_e', 0, () => {
      const gattServer = bluetoothManager.BLE.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        gattServer.startAdvertising(bleAs(), bleAd());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STARTADVERTISING_B
     * @tc.name leg_btm_startAdvertising_b
     * @tc.desc Verify Bluetooth API leg btm startAdvertising boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_startAdvertising_b', 0, () => {
      const gattServer = bluetoothManager.BLE.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        gattServer.startAdvertising(bleAs(), bleAd());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STARTADVERTISING_R
     * @tc.name leg_btm_startAdvertising_r
     * @tc.desc Verify Bluetooth API leg btm startAdvertising return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_startAdvertising_r', 0, () => {
      const gattServer = bluetoothManager.BLE.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.startAdvertising(bleAs(), bleAd());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STARTADVERTISING_X
     * @tc.name leg_btm_startAdvertising_x
     * @tc.desc Verify Bluetooth API leg btm startAdvertising repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_startAdvertising_x', 0, () => {
      const gattServer = bluetoothManager.BLE.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.startAdvertising(bleAs(), bleAd());
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
        gattServer.startAdvertising(bleAs(), bleAd());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STARTADVERTISING_C
     * @tc.name leg_btm_startAdvertising_c
     * @tc.desc Verify Bluetooth API leg btm startAdvertising condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_startAdvertising_c', 0, () => {
      const gattServer = bluetoothManager.BLE.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.startAdvertising(bleAs(), bleAd());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STARTADVERTISING_PERF
     * @tc.name leg_btm_startAdvertising_perf
     * @tc.desc Verify Bluetooth apitest case leg btm startAdvertising perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_startAdvertising_perf', 0, () => {
      runPerf('leg_btm_startAdvertising_perf', 5000, () => {
        try {
          const gattServer = bluetoothManager.BLE.createGattServer();
          gattServer.startAdvertising(bleAs(), bleAd());
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STARTADVERTISING_STRESS
     * @tc.name leg_btm_startAdvertising_stress
     * @tc.desc Verify Bluetooth apitest case leg btm startAdvertising stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_startAdvertising_stress', 0, () => {
      runStress('leg_btm_startAdvertising_stress', 20, () => {
        try {
          const gattServer = bluetoothManager.BLE.createGattServer();
          gattServer.startAdvertising(bleAs(), bleAd());
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_stopAdvertising', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STOPADVERTISING_N
     * @tc.name leg_btm_stopAdvertising_n
     * @tc.desc Verify Bluetooth API leg btm stopAdvertising normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_stopAdvertising_n', 0, () => {
      const gattServer = bluetoothManager.BLE.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattServer.stopAdvertising();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STOPADVERTISING_E
     * @tc.name leg_btm_stopAdvertising_e
     * @tc.desc Verify Bluetooth API leg btm stopAdvertising error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_stopAdvertising_e', 0, () => {
      const gattServer = bluetoothManager.BLE.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        gattServer.stopAdvertising();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STOPADVERTISING_B
     * @tc.name leg_btm_stopAdvertising_b
     * @tc.desc Verify Bluetooth API leg btm stopAdvertising boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_stopAdvertising_b', 0, () => {
      const gattServer = bluetoothManager.BLE.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        gattServer.stopAdvertising();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STOPADVERTISING_R
     * @tc.name leg_btm_stopAdvertising_r
     * @tc.desc Verify Bluetooth API leg btm stopAdvertising return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_stopAdvertising_r', 0, () => {
      const gattServer = bluetoothManager.BLE.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.stopAdvertising();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STOPADVERTISING_X
     * @tc.name leg_btm_stopAdvertising_x
     * @tc.desc Verify Bluetooth API leg btm stopAdvertising repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_stopAdvertising_x', 0, () => {
      const gattServer = bluetoothManager.BLE.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.stopAdvertising();
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
        gattServer.stopAdvertising();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STOPADVERTISING_C
     * @tc.name leg_btm_stopAdvertising_c
     * @tc.desc Verify Bluetooth API leg btm stopAdvertising condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_stopAdvertising_c', 0, () => {
      const gattServer = bluetoothManager.BLE.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.stopAdvertising();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STOPADVERTISING_PERF
     * @tc.name leg_btm_stopAdvertising_perf
     * @tc.desc Verify Bluetooth apitest case leg btm stopAdvertising perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_stopAdvertising_perf', 0, () => {
      runPerf('leg_btm_stopAdvertising_perf', 5000, () => {
        try {
          const gattServer = bluetoothManager.BLE.createGattServer();
          gattServer.stopAdvertising();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_STOPADVERTISING_STRESS
     * @tc.name leg_btm_stopAdvertising_stress
     * @tc.desc Verify Bluetooth apitest case leg btm stopAdvertising stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_stopAdvertising_stress', 0, () => {
      runStress('leg_btm_stopAdvertising_stress', 20, () => {
        try {
          const gattServer = bluetoothManager.BLE.createGattServer();
          gattServer.stopAdvertising();
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_getConnectionDevices', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTIONDEVICES_N
     * @tc.name leg_btm_getConnectionDevices_n
     * @tc.desc Verify Bluetooth API leg btm getConnectionDevices normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectionDevices_n', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const profile = bluetoothManager.getProfileInstance(1);
        value = profile.getConnectionDevices() as Array < Object > ;
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(Array.isArray(value)).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTIONDEVICES_E
     * @tc.name leg_btm_getConnectionDevices_e
     * @tc.desc Verify Bluetooth API leg btm getConnectionDevices error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectionDevices_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const profile = bluetoothManager.getProfileInstance(-1);
        profile.getConnectionDevices();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTIONDEVICES_B
     * @tc.name leg_btm_getConnectionDevices_b
     * @tc.desc Verify Bluetooth API leg btm getConnectionDevices boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectionDevices_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const profile = bluetoothManager.getProfileInstance(-1);
        profile.getConnectionDevices();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTIONDEVICES_R
     * @tc.name leg_btm_getConnectionDevices_r
     * @tc.desc Verify Bluetooth API leg btm getConnectionDevices return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectionDevices_r', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const profile = bluetoothManager.getProfileInstance(1);
        value = profile.getConnectionDevices() as Array < Object > ;
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(Array.isArray(value)).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTIONDEVICES_X
     * @tc.name leg_btm_getConnectionDevices_x
     * @tc.desc Verify Bluetooth API leg btm getConnectionDevices repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectionDevices_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const profile = bluetoothManager.getProfileInstance(1);
        profile.getConnectionDevices();
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
        const profile = bluetoothManager.getProfileInstance(1);
        profile.getConnectionDevices();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTIONDEVICES_C
     * @tc.name leg_btm_getConnectionDevices_c
     * @tc.desc Verify Bluetooth API leg btm getConnectionDevices condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectionDevices_c', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const profile = bluetoothManager.getProfileInstance(1);
        value = profile.getConnectionDevices() as Array < Object > ;
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(Array.isArray(value)).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTIONDEVICES_PERF
     * @tc.name leg_btm_getConnectionDevices_perf
     * @tc.desc Verify Bluetooth apitest case leg btm getConnectionDevices perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectionDevices_perf', 0, () => {
      runPerf('leg_btm_getConnectionDevices_perf', 5000, () => {
        try {
          bluetoothManager.getProfileInstance(1);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETCONNECTIONDEVICES_STRESS
     * @tc.name leg_btm_getConnectionDevices_stress
     * @tc.desc Verify Bluetooth apitest case leg btm getConnectionDevices stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getConnectionDevices_stress', 0, () => {
      runStress('leg_btm_getConnectionDevices_stress', 20, () => {
        try {
          bluetoothManager.getProfileInstance(1);
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_getDeviceState', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETDEVICESTATE_N
     * @tc.name leg_btm_getDeviceState_n
     * @tc.desc Verify Bluetooth API leg btm getDeviceState normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getDeviceState_n', 0, () => {
      let errCode = 0;
      let value = -1;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const profile = bluetoothManager.getProfileInstance(1);
        value = profile.getDeviceState(BADDR);
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(typeof value).assertEqual('number');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETDEVICESTATE_E
     * @tc.name leg_btm_getDeviceState_e
     * @tc.desc Verify Bluetooth API leg btm getDeviceState error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getDeviceState_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const profile = bluetoothManager.getProfileInstance(1);
        profile.getDeviceState(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETDEVICESTATE_B
     * @tc.name leg_btm_getDeviceState_b
     * @tc.desc Verify Bluetooth API leg btm getDeviceState boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getDeviceState_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const profile = bluetoothManager.getProfileInstance(1);
        profile.getDeviceState(BAD_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETDEVICESTATE_R
     * @tc.name leg_btm_getDeviceState_r
     * @tc.desc Verify Bluetooth API leg btm getDeviceState return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getDeviceState_r', 0, () => {
      let errCode = 0;
      let value = -1;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const profile = bluetoothManager.getProfileInstance(1);
        value = profile.getDeviceState(BADDR);
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(typeof value).assertEqual('number');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETDEVICESTATE_X
     * @tc.name leg_btm_getDeviceState_x
     * @tc.desc Verify Bluetooth API leg btm getDeviceState repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getDeviceState_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const profile = bluetoothManager.getProfileInstance(1);
        profile.getDeviceState(BADDR);
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
        const profile = bluetoothManager.getProfileInstance(1);
        profile.getDeviceState(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETDEVICESTATE_C
     * @tc.name leg_btm_getDeviceState_c
     * @tc.desc Verify Bluetooth API leg btm getDeviceState condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getDeviceState_c', 0, () => {
      let errCode = 0;
      let value = -1;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const profile = bluetoothManager.getProfileInstance(1);
        value = profile.getDeviceState(BADDR);
      } catch (err) {
        errCode = Number((err as BusinessError).code);
        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
      if (errCode === 0) {
        expect(typeof value).assertEqual('number');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETDEVICESTATE_PERF
     * @tc.name leg_btm_getDeviceState_perf
     * @tc.desc Verify Bluetooth apitest case leg btm getDeviceState perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getDeviceState_perf', 0, () => {
      runPerf('leg_btm_getDeviceState_perf', 5000, () => {
        try {
          bluetoothManager.getProfileInstance(1);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_GETDEVICESTATE_STRESS
     * @tc.name leg_btm_getDeviceState_stress
     * @tc.desc Verify Bluetooth apitest case leg btm getDeviceState stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_getDeviceState_stress', 0, () => {
      runStress('leg_btm_getDeviceState_stress', 20, () => {
        try {
          bluetoothManager.getProfileInstance(1);
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_btm_setNotifyCharacteristicChanged', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_SETNOTIFYCHARACTERISTICCHANGED_N
     * @tc.name leg_btm_setNotifyCharacteristicChanged_n
     * @tc.desc Verify Bluetooth API leg btm setNotifyCharacteristicChanged normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_setNotifyCharacteristicChanged_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        const gattClient = bluetoothManager.BLE.createGattClientDevice(BADDR);
        gattClient.setNotifyCharacteristicChanged(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_SETNOTIFYCHARACTERISTICCHANGED_E
     * @tc.name leg_btm_setNotifyCharacteristicChanged_e
     * @tc.desc Verify Bluetooth API leg btm setNotifyCharacteristicChanged error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_setNotifyCharacteristicChanged_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        const gattClient = bluetoothManager.BLE.createGattClientDevice(BADDR);
        gattClient.setNotifyCharacteristicChanged(bleCharRef(), false);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_SETNOTIFYCHARACTERISTICCHANGED_B
     * @tc.name leg_btm_setNotifyCharacteristicChanged_b
     * @tc.desc Verify Bluetooth API leg btm setNotifyCharacteristicChanged boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_setNotifyCharacteristicChanged_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        const gattClient = bluetoothManager.BLE.createGattClientDevice(BADDR);
        gattClient.setNotifyCharacteristicChanged(bleCharRef(), false);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_SETNOTIFYCHARACTERISTICCHANGED_R
     * @tc.name leg_btm_setNotifyCharacteristicChanged_r
     * @tc.desc Verify Bluetooth API leg btm setNotifyCharacteristicChanged return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_setNotifyCharacteristicChanged_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        const gattClient = bluetoothManager.BLE.createGattClientDevice(BADDR);
        gattClient.setNotifyCharacteristicChanged(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_SETNOTIFYCHARACTERISTICCHANGED_X
     * @tc.name leg_btm_setNotifyCharacteristicChanged_x
     * @tc.desc Verify Bluetooth API leg btm setNotifyCharacteristicChanged repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_setNotifyCharacteristicChanged_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        const gattClient = bluetoothManager.BLE.createGattClientDevice(BADDR);
        gattClient.setNotifyCharacteristicChanged(bleCharRef(), true);
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
        const gattClient = bluetoothManager.BLE.createGattClientDevice(BADDR);
        gattClient.setNotifyCharacteristicChanged(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_SETNOTIFYCHARACTERISTICCHANGED_C
     * @tc.name leg_btm_setNotifyCharacteristicChanged_c
     * @tc.desc Verify Bluetooth API leg btm setNotifyCharacteristicChanged condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_setNotifyCharacteristicChanged_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        const gattClient = bluetoothManager.BLE.createGattClientDevice(BADDR);
        gattClient.setNotifyCharacteristicChanged(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_SETNOTIFYCHARACTERISTICCHANGED_PERF
     * @tc.name leg_btm_setNotifyCharacteristicChanged_perf
     * @tc.desc Verify Bluetooth apitest case leg btm setNotifyCharacteristicChanged perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_setNotifyCharacteristicChanged_perf', 0, () => {
      runPerf('leg_btm_setNotifyCharacteristicChanged_perf', 5000, () => {
        try {
          const gattClient = bluetoothManager.BLE.createGattClientDevice(BADDR);
          gattClient.setNotifyCharacteristicChanged(bleCharRef(), true);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BTM_SETNOTIFYCHARACTERISTICCHANGED_STRESS
     * @tc.name leg_btm_setNotifyCharacteristicChanged_stress
     * @tc.desc Verify Bluetooth apitest case leg btm setNotifyCharacteristicChanged stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_btm_setNotifyCharacteristicChanged_stress', 0, () => {
      runStress('leg_btm_setNotifyCharacteristicChanged_stress', 20, () => {
        try {
          const gattClient = bluetoothManager.BLE.createGattClientDevice(BADDR);
          gattClient.setNotifyCharacteristicChanged(bleCharRef(), true);
        } catch (_e) {
        }
      });
    });
    });
  });
}


