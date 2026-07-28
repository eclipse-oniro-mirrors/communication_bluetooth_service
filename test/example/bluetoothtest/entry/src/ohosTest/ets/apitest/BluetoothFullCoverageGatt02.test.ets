      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattServer.off('descriptorWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORWRITE_R
     * @tc.name gatts_off_descriptorWrite_r
     * @tc.desc Verify Bluetooth API gatts off descriptorWrite return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorWrite_r', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.off('descriptorWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORWRITE_X
     * @tc.name gatts_off_descriptorWrite_x
     * @tc.desc Verify Bluetooth API gatts off descriptorWrite repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorWrite_x', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.off('descriptorWrite');
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
        gattServer.off('descriptorWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORWRITE_C
     * @tc.name gatts_off_descriptorWrite_c
     * @tc.desc Verify Bluetooth API gatts off descriptorWrite condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorWrite_c', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.off('descriptorWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORWRITE_PERF
     * @tc.name gatts_off_descriptorWrite_perf
     * @tc.desc Verify Bluetooth apitest case gatts off descriptorWrite perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorWrite_perf', 0, () => {
      runPerf('gatts_off_descriptorWrite_perf', 5000, () => {
        try {
          const gattServer = ble.createGattServer();
          gattServer.off('descriptorWrite');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORWRITE_STRESS
     * @tc.name gatts_off_descriptorWrite_stress
     * @tc.desc Verify Bluetooth apitest case gatts off descriptorWrite stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorWrite_stress', 0, () => {
      runStress('gatts_off_descriptorWrite_stress', 20, () => {
        try {
          const gattServer = ble.createGattServer();
          gattServer.off('descriptorWrite');
        } catch (_e) {
        }
      });
    });
    });
    describe('gatts_on_connectionStateChange', () => {
    it('gatts_on_connectionStateChange_n', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattServer.on('connectionStateChange', () => {});
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
    it('gatts_on_connectionStateChange_e', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattServer.on('connectionStateChange', () => {});
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
    it('gatts_on_connectionStateChange_b', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattServer.on('connectionStateChange', () => {});
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
    it('gatts_on_connectionStateChange_r', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.on('connectionStateChange', () => {});
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
    it('gatts_on_connectionStateChange_x', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.on('connectionStateChange', () => {});
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
        gattServer.on('connectionStateChange', () => {});
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
    it('gatts_on_connectionStateChange_c', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.on('connectionStateChange', () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ON_CONNECTIONSTATECHANGE_PERF
     * @tc.name gatts_on_connectionStateChange_perf
     * @tc.desc Verify Bluetooth apitest case gatts on connectionStateChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_on_connectionStateChange_perf', 0, () => {
      runPerf('gatts_on_connectionStateChange_perf', 5000, () => {
        try {
          const gattServer = ble.createGattServer();
          gattServer.on('connectionStateChange', () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ON_CONNECTIONSTATECHANGE_STRESS
     * @tc.name gatts_on_connectionStateChange_stress
     * @tc.desc Verify Bluetooth apitest case gatts on connectionStateChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_on_connectionStateChange_stress', 0, () => {
      runStress('gatts_on_connectionStateChange_stress', 20, () => {
        try {
          const gattServer = ble.createGattServer();
          gattServer.on('connectionStateChange', () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('gatts_off_connectionStateChange', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CONNECTIONSTATECHANGE_N
     * @tc.name gatts_off_connectionStateChange_n
     * @tc.desc Verify Bluetooth API gatts off connectionStateChange normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_connectionStateChange_n', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattServer.off('connectionStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CONNECTIONSTATECHANGE_E
     * @tc.name gatts_off_connectionStateChange_e
     * @tc.desc Verify Bluetooth API gatts off connectionStateChange error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_connectionStateChange_e', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattServer.off('connectionStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CONNECTIONSTATECHANGE_B
     * @tc.name gatts_off_connectionStateChange_b
     * @tc.desc Verify Bluetooth API gatts off connectionStateChange boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_connectionStateChange_b', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattServer.off('connectionStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CONNECTIONSTATECHANGE_R
     * @tc.name gatts_off_connectionStateChange_r
     * @tc.desc Verify Bluetooth API gatts off connectionStateChange return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_connectionStateChange_r', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.off('connectionStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CONNECTIONSTATECHANGE_X
     * @tc.name gatts_off_connectionStateChange_x
     * @tc.desc Verify Bluetooth API gatts off connectionStateChange repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_connectionStateChange_x', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.off('connectionStateChange');
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
        gattServer.off('connectionStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CONNECTIONSTATECHANGE_C
     * @tc.name gatts_off_connectionStateChange_c
     * @tc.desc Verify Bluetooth API gatts off connectionStateChange condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_connectionStateChange_c', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.off('connectionStateChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CONNECTIONSTATECHANGE_PERF
     * @tc.name gatts_off_connectionStateChange_perf
     * @tc.desc Verify Bluetooth apitest case gatts off connectionStateChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_connectionStateChange_perf', 0, () => {
      runPerf('gatts_off_connectionStateChange_perf', 5000, () => {
        try {
          const gattServer = ble.createGattServer();
          gattServer.off('connectionStateChange');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CONNECTIONSTATECHANGE_STRESS
     * @tc.name gatts_off_connectionStateChange_stress
     * @tc.desc Verify Bluetooth apitest case gatts off connectionStateChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_connectionStateChange_stress', 0, () => {
      runStress('gatts_off_connectionStateChange_stress', 20, () => {
        try {
          const gattServer = ble.createGattServer();
          gattServer.off('connectionStateChange');
        } catch (_e) {
        }
      });
    });
    });
    describe('gatts_on_mtuChange', () => {
    it('gatts_on_mtuChange_n', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattServer.on('BLEMtuChange', () => {});
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
    it('gatts_on_mtuChange_e', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattServer.on('BLEMtuChange', () => {});
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
    it('gatts_on_mtuChange_b', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattServer.on('BLEMtuChange', () => {});
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
    it('gatts_on_mtuChange_r', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.on('BLEMtuChange', () => {});
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
    it('gatts_on_mtuChange_x', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.on('BLEMtuChange', () => {});
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
        gattServer.on('BLEMtuChange', () => {});
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
    it('gatts_on_mtuChange_c', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.on('BLEMtuChange', () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ON_MTUCHANGE_PERF
     * @tc.name gatts_on_mtuChange_perf
     * @tc.desc Verify Bluetooth apitest case gatts on mtuChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_on_mtuChange_perf', 0, () => {
      runPerf('gatts_on_mtuChange_perf', 5000, () => {
        try {
          const gattServer = ble.createGattServer();
          gattServer.on('BLEMtuChange', () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ON_MTUCHANGE_STRESS
     * @tc.name gatts_on_mtuChange_stress
     * @tc.desc Verify Bluetooth apitest case gatts on mtuChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_on_mtuChange_stress', 0, () => {
      runStress('gatts_on_mtuChange_stress', 20, () => {
        try {
          const gattServer = ble.createGattServer();
          gattServer.on('BLEMtuChange', () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('gatts_off_mtuChange', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_MTUCHANGE_N
     * @tc.name gatts_off_mtuChange_n
     * @tc.desc Verify Bluetooth API gatts off mtuChange normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_mtuChange_n', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattServer.off('BLEMtuChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_MTUCHANGE_E
     * @tc.name gatts_off_mtuChange_e
     * @tc.desc Verify Bluetooth API gatts off mtuChange error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_mtuChange_e', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattServer.off('BLEMtuChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_MTUCHANGE_B
     * @tc.name gatts_off_mtuChange_b
     * @tc.desc Verify Bluetooth API gatts off mtuChange boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_mtuChange_b', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattServer.off('BLEMtuChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_MTUCHANGE_R
     * @tc.name gatts_off_mtuChange_r
     * @tc.desc Verify Bluetooth API gatts off mtuChange return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_mtuChange_r', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.off('BLEMtuChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_MTUCHANGE_X
     * @tc.name gatts_off_mtuChange_x
     * @tc.desc Verify Bluetooth API gatts off mtuChange repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_mtuChange_x', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.off('BLEMtuChange');
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
        gattServer.off('BLEMtuChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_MTUCHANGE_C
     * @tc.name gatts_off_mtuChange_c
     * @tc.desc Verify Bluetooth API gatts off mtuChange condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_mtuChange_c', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.off('BLEMtuChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_MTUCHANGE_PERF
     * @tc.name gatts_off_mtuChange_perf
     * @tc.desc Verify Bluetooth apitest case gatts off mtuChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_mtuChange_perf', 0, () => {
      runPerf('gatts_off_mtuChange_perf', 5000, () => {
        try {
          const gattServer = ble.createGattServer();
          gattServer.off('BLEMtuChange');
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_MTUCHANGE_STRESS
     * @tc.name gatts_off_mtuChange_stress
     * @tc.desc Verify Bluetooth apitest case gatts off mtuChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_mtuChange_stress', 0, () => {
      runStress('gatts_off_mtuChange_stress', 20, () => {
        try {
          const gattServer = ble.createGattServer();
          gattServer.off('BLEMtuChange');
        } catch (_e) {
        }
      });
    });
    });
    describe('blescan_startScan', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_N
     * @tc.name blescan_startScan_n
     * @tc.desc Verify Bluetooth API blescan startScan normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_n', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        sc.startScan([], {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_E
     * @tc.name blescan_startScan_e
     * @tc.desc Verify Bluetooth API blescan startScan error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_e', 0, () => {
      const sc = ble.createBleScanner();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        sc.startScan([], {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_B
     * @tc.name blescan_startScan_b
     * @tc.desc Verify Bluetooth API blescan startScan boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_b', 0, () => {
      const sc = ble.createBleScanner();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        sc.startScan([], {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_R
     * @tc.name blescan_startScan_r
     * @tc.desc Verify Bluetooth API blescan startScan return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_r', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        sc.startScan([], {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_X
     * @tc.name blescan_startScan_x
     * @tc.desc Verify Bluetooth API blescan startScan repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_x', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        sc.startScan([], {});
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
        sc.startScan([], {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_C
     * @tc.name blescan_startScan_c
     * @tc.desc Verify Bluetooth API blescan startScan condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_c', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        sc.startScan([], {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_PERF
     * @tc.name blescan_startScan_perf
     * @tc.desc Verify Bluetooth apitest case blescan startScan perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_perf', 0, () => {
      runPerf('blescan_startScan_perf', 5000, () => {
        try {
          const sc = ble.createBleScanner();
          sc.startScan([], {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_STRESS
     * @tc.name blescan_startScan_stress
     * @tc.desc Verify Bluetooth apitest case blescan startScan stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_stress', 0, () => {
      runStress('blescan_startScan_stress', 20, () => {
        try {
          const sc = ble.createBleScanner();
          sc.startScan([], {});
        } catch (_e) {
        }
      });
    });
    });
    describe('blescan_stopScan', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_N
     * @tc.name blescan_stopScan_n
     * @tc.desc Verify Bluetooth API blescan stopScan normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_n', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        sc.stopScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_E
     * @tc.name blescan_stopScan_e
     * @tc.desc Verify Bluetooth API blescan stopScan error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_e', 0, () => {
      const sc = ble.createBleScanner();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        sc.stopScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_B
     * @tc.name blescan_stopScan_b
     * @tc.desc Verify Bluetooth API blescan stopScan boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_b', 0, () => {
      const sc = ble.createBleScanner();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        sc.stopScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_R
     * @tc.name blescan_stopScan_r
     * @tc.desc Verify Bluetooth API blescan stopScan return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_r', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        sc.stopScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_X
     * @tc.name blescan_stopScan_x
     * @tc.desc Verify Bluetooth API blescan stopScan repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_x', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        sc.stopScan();
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
        sc.stopScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_C
     * @tc.name blescan_stopScan_c
     * @tc.desc Verify Bluetooth API blescan stopScan condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_c', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        sc.stopScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_PERF
     * @tc.name blescan_stopScan_perf
     * @tc.desc Verify Bluetooth apitest case blescan stopScan perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_perf', 0, () => {
      runPerf('blescan_stopScan_perf', 5000, () => {
        try {
          const sc = ble.createBleScanner();
          sc.stopScan();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_STRESS
     * @tc.name blescan_stopScan_stress
     * @tc.desc Verify Bluetooth apitest case blescan stopScan stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_stress', 0, () => {
      runStress('blescan_stopScan_stress', 20, () => {
        try {
          const sc = ble.createBleScanner();
          sc.stopScan();
        } catch (_e) {
        }
      });
    });
    });
    describe('blescan_on_BLEDeviceFind', () => {
    it('blescan_on_BLEDeviceFind_n', 0, async (done: Function) => {
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
    it('blescan_on_BLEDeviceFind_e', 0, async (done: Function) => {
      const sc = ble.createBleScanner();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        sc.on('BLEDeviceFind', () => {});
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
    it('blescan_on_BLEDeviceFind_b', 0, async (done: Function) => {
      const sc = ble.createBleScanner();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        sc.on('BLEDeviceFind', () => {});
        gattServer.on('characteristicWrite', () => {});
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
    it('gatts_on_characteristicWrite_r', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.on('characteristicWrite', () => {});
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
    it('gatts_on_characteristicWrite_x', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.on('characteristicWrite', () => {});
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
        gattServer.on('characteristicWrite', () => {});
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
    it('gatts_on_characteristicWrite_c', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.on('characteristicWrite', () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ON_CHARACTERISTICWRITE_PERF
     * @tc.name gatts_on_characteristicWrite_perf
     * @tc.desc Verify Bluetooth apitest case gatts on characteristicWrite perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_on_characteristicWrite_perf', 0, () => {
      runPerf('gatts_on_characteristicWrite_perf', 5000, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ON_CHARACTERISTICWRITE_STRESS
     * @tc.name gatts_on_characteristicWrite_stress
     * @tc.desc Verify Bluetooth apitest case gatts on characteristicWrite stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_on_characteristicWrite_stress', 0, () => {
      runStress('gatts_on_characteristicWrite_stress', 20, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });
    });
    describe('gatts_off_characteristicWrite', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CHARACTERISTICWRITE_N
     * @tc.name gatts_off_characteristicWrite_n
     * @tc.desc Verify Bluetooth API gatts off characteristicWrite normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_characteristicWrite_n', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattServer.off('characteristicWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CHARACTERISTICWRITE_E
     * @tc.name gatts_off_characteristicWrite_e
     * @tc.desc Verify Bluetooth API gatts off characteristicWrite error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_characteristicWrite_e', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattServer.off('characteristicWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CHARACTERISTICWRITE_B
     * @tc.name gatts_off_characteristicWrite_b
     * @tc.desc Verify Bluetooth API gatts off characteristicWrite boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_characteristicWrite_b', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattServer.off('characteristicWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CHARACTERISTICWRITE_R
     * @tc.name gatts_off_characteristicWrite_r
     * @tc.desc Verify Bluetooth API gatts off characteristicWrite return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_characteristicWrite_r', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.off('characteristicWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CHARACTERISTICWRITE_X
     * @tc.name gatts_off_characteristicWrite_x
     * @tc.desc Verify Bluetooth API gatts off characteristicWrite repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_characteristicWrite_x', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.off('characteristicWrite');
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
        gattServer.off('characteristicWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CHARACTERISTICWRITE_C
     * @tc.name gatts_off_characteristicWrite_c
     * @tc.desc Verify Bluetooth API gatts off characteristicWrite condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_characteristicWrite_c', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.off('characteristicWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CHARACTERISTICWRITE_PERF
     * @tc.name gatts_off_characteristicWrite_perf
     * @tc.desc Verify Bluetooth apitest case gatts off characteristicWrite perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_characteristicWrite_perf', 0, () => {
      runPerf('gatts_off_characteristicWrite_perf', 5000, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_CHARACTERISTICWRITE_STRESS
     * @tc.name gatts_off_characteristicWrite_stress
     * @tc.desc Verify Bluetooth apitest case gatts off characteristicWrite stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_characteristicWrite_stress', 0, () => {
      runStress('gatts_off_characteristicWrite_stress', 20, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });
    });
    describe('gatts_on_descriptorRead', () => {
    it('gatts_on_descriptorRead_n', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattServer.on('descriptorRead', () => {});
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
    it('gatts_on_descriptorRead_e', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattServer.on('descriptorRead', () => {});
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
    it('gatts_on_descriptorRead_b', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattServer.on('descriptorRead', () => {});
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
    it('gatts_on_descriptorRead_r', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.on('descriptorRead', () => {});
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
    it('gatts_on_descriptorRead_x', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.on('descriptorRead', () => {});
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
        gattServer.on('descriptorRead', () => {});
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
    it('gatts_on_descriptorRead_c', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.on('descriptorRead', () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ON_DESCRIPTORREAD_PERF
     * @tc.name gatts_on_descriptorRead_perf
     * @tc.desc Verify Bluetooth apitest case gatts on descriptorRead perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_on_descriptorRead_perf', 0, () => {
      runPerf('gatts_on_descriptorRead_perf', 5000, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ON_DESCRIPTORREAD_STRESS
     * @tc.name gatts_on_descriptorRead_stress
     * @tc.desc Verify Bluetooth apitest case gatts on descriptorRead stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_on_descriptorRead_stress', 0, () => {
      runStress('gatts_on_descriptorRead_stress', 20, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });
    });
    describe('gatts_off_descriptorRead', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORREAD_N
     * @tc.name gatts_off_descriptorRead_n
     * @tc.desc Verify Bluetooth API gatts off descriptorRead normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorRead_n', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattServer.off('descriptorRead');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORREAD_E
     * @tc.name gatts_off_descriptorRead_e
     * @tc.desc Verify Bluetooth API gatts off descriptorRead error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorRead_e', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattServer.off('descriptorRead');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORREAD_B
     * @tc.name gatts_off_descriptorRead_b
     * @tc.desc Verify Bluetooth API gatts off descriptorRead boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorRead_b', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattServer.off('descriptorRead');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORREAD_R
     * @tc.name gatts_off_descriptorRead_r
     * @tc.desc Verify Bluetooth API gatts off descriptorRead return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorRead_r', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.off('descriptorRead');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORREAD_X
     * @tc.name gatts_off_descriptorRead_x
     * @tc.desc Verify Bluetooth API gatts off descriptorRead repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorRead_x', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.off('descriptorRead');
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
        gattServer.off('descriptorRead');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORREAD_C
     * @tc.name gatts_off_descriptorRead_c
     * @tc.desc Verify Bluetooth API gatts off descriptorRead condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorRead_c', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.off('descriptorRead');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORREAD_PERF
     * @tc.name gatts_off_descriptorRead_perf
     * @tc.desc Verify Bluetooth apitest case gatts off descriptorRead perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorRead_perf', 0, () => {
      runPerf('gatts_off_descriptorRead_perf', 5000, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORREAD_STRESS
     * @tc.name gatts_off_descriptorRead_stress
     * @tc.desc Verify Bluetooth apitest case gatts off descriptorRead stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorRead_stress', 0, () => {
      runStress('gatts_off_descriptorRead_stress', 20, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });
    });
    describe('gatts_on_descriptorWrite', () => {
    it('gatts_on_descriptorWrite_n', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattServer.on('descriptorWrite', () => {});
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
    it('gatts_on_descriptorWrite_e', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattServer.on('descriptorWrite', () => {});
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
    it('gatts_on_descriptorWrite_b', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattServer.on('descriptorWrite', () => {});
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
    it('gatts_on_descriptorWrite_r', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.on('descriptorWrite', () => {});
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
    it('gatts_on_descriptorWrite_x', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.on('descriptorWrite', () => {});
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
        gattServer.on('descriptorWrite', () => {});
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
    it('gatts_on_descriptorWrite_c', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.on('descriptorWrite', () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ON_DESCRIPTORWRITE_PERF
     * @tc.name gatts_on_descriptorWrite_perf
     * @tc.desc Verify Bluetooth apitest case gatts on descriptorWrite perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_on_descriptorWrite_perf', 0, () => {
      runPerf('gatts_on_descriptorWrite_perf', 5000, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ON_DESCRIPTORWRITE_STRESS
     * @tc.name gatts_on_descriptorWrite_stress
     * @tc.desc Verify Bluetooth apitest case gatts on descriptorWrite stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_on_descriptorWrite_stress', 0, () => {
      runStress('gatts_on_descriptorWrite_stress', 20, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });
    });
    describe('gatts_off_descriptorWrite', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORWRITE_N
     * @tc.name gatts_off_descriptorWrite_n
     * @tc.desc Verify Bluetooth API gatts off descriptorWrite normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorWrite_n', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattServer.off('descriptorWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORWRITE_E
     * @tc.name gatts_off_descriptorWrite_e
     * @tc.desc Verify Bluetooth API gatts off descriptorWrite error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorWrite_e', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattServer.off('descriptorWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORWRITE_B
     * @tc.name gatts_off_descriptorWrite_b
     * @tc.desc Verify Bluetooth API gatts off descriptorWrite boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorWrite_b', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattServer.off('descriptorWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORWRITE_R
     * @tc.name gatts_off_descriptorWrite_r
     * @tc.desc Verify Bluetooth API gatts off descriptorWrite return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorWrite_r', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.off('descriptorWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORWRITE_X
     * @tc.name gatts_off_descriptorWrite_x
     * @tc.desc Verify Bluetooth API gatts off descriptorWrite repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorWrite_x', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.off('descriptorWrite');
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
        gattServer.off('descriptorWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORWRITE_C
     * @tc.name gatts_off_descriptorWrite_c
     * @tc.desc Verify Bluetooth API gatts off descriptorWrite condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorWrite_c', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.off('descriptorWrite');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORWRITE_PERF
     * @tc.name gatts_off_descriptorWrite_perf
     * @tc.desc Verify Bluetooth apitest case gatts off descriptorWrite perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorWrite_perf', 0, () => {
      runPerf('gatts_off_descriptorWrite_perf', 5000, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_DESCRIPTORWRITE_STRESS
     * @tc.name gatts_off_descriptorWrite_stress
     * @tc.desc Verify Bluetooth apitest case gatts off descriptorWrite stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_descriptorWrite_stress', 0, () => {
      runStress('gatts_off_descriptorWrite_stress', 20, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });
    });
    describe('gatts_on_BLEMtuChange', () => {
    it('gatts_on_BLEMtuChange_n', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattServer.on('BLEMtuChange', () => {});
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
    it('gatts_on_BLEMtuChange_e', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattServer.on('BLEMtuChange', () => {});
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
    it('gatts_on_BLEMtuChange_b', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattServer.on('BLEMtuChange', () => {});
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
    it('gatts_on_BLEMtuChange_r', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.on('BLEMtuChange', () => {});
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
    it('gatts_on_BLEMtuChange_x', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.on('BLEMtuChange', () => {});
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
        gattServer.on('BLEMtuChange', () => {});
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
    it('gatts_on_BLEMtuChange_c', 0, async (done: Function) => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.on('BLEMtuChange', () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ON_BLEMTUCHANGE_PERF
     * @tc.name gatts_on_BLEMtuChange_perf
     * @tc.desc Verify Bluetooth apitest case gatts on BLEMtuChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_on_BLEMtuChange_perf', 0, () => {
      runPerf('gatts_on_BLEMtuChange_perf', 5000, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ON_BLEMTUCHANGE_STRESS
     * @tc.name gatts_on_BLEMtuChange_stress
     * @tc.desc Verify Bluetooth apitest case gatts on BLEMtuChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_on_BLEMtuChange_stress', 0, () => {
      runStress('gatts_on_BLEMtuChange_stress', 20, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });
    });
    describe('gatts_off_BLEMtuChange', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_BLEMTUCHANGE_N
     * @tc.name gatts_off_BLEMtuChange_n
     * @tc.desc Verify Bluetooth API gatts off BLEMtuChange normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_BLEMtuChange_n', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattServer.off('BLEMtuChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_BLEMTUCHANGE_E
     * @tc.name gatts_off_BLEMtuChange_e
     * @tc.desc Verify Bluetooth API gatts off BLEMtuChange error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_BLEMtuChange_e', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattServer.off('BLEMtuChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_BLEMTUCHANGE_B
     * @tc.name gatts_off_BLEMtuChange_b
     * @tc.desc Verify Bluetooth API gatts off BLEMtuChange boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_BLEMtuChange_b', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattServer.off('BLEMtuChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_BLEMTUCHANGE_R
     * @tc.name gatts_off_BLEMtuChange_r
     * @tc.desc Verify Bluetooth API gatts off BLEMtuChange return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_BLEMtuChange_r', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.off('BLEMtuChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_BLEMTUCHANGE_X
     * @tc.name gatts_off_BLEMtuChange_x
     * @tc.desc Verify Bluetooth API gatts off BLEMtuChange repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_BLEMtuChange_x', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.off('BLEMtuChange');
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
        gattServer.off('BLEMtuChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_BLEMTUCHANGE_C
     * @tc.name gatts_off_BLEMtuChange_c
     * @tc.desc Verify Bluetooth API gatts off BLEMtuChange condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_BLEMtuChange_c', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.off('BLEMtuChange');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_BLEMTUCHANGE_PERF
     * @tc.name gatts_off_BLEMtuChange_perf
     * @tc.desc Verify Bluetooth apitest case gatts off BLEMtuChange perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_BLEMtuChange_perf', 0, () => {
      runPerf('gatts_off_BLEMtuChange_perf', 5000, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFF_BLEMTUCHANGE_STRESS
     * @tc.name gatts_off_BLEMtuChange_stress
     * @tc.desc Verify Bluetooth apitest case gatts off BLEMtuChange stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_off_BLEMtuChange_stress', 0, () => {
      runStress('gatts_off_BLEMtuChange_stress', 20, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });
    });
    describe('gatts_onBlePhyUpdate', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ONBLEPHYUPDATE_N
     * @tc.name gatts_onBlePhyUpdate_n
     * @tc.desc Verify Bluetooth API gatts onBlePhyUpdate normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_onBlePhyUpdate_n', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattServer.onBlePhyUpdate(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ONBLEPHYUPDATE_E
     * @tc.name gatts_onBlePhyUpdate_e
     * @tc.desc Verify Bluetooth API gatts onBlePhyUpdate error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_onBlePhyUpdate_e', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattServer.onBlePhyUpdate(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ONBLEPHYUPDATE_B
     * @tc.name gatts_onBlePhyUpdate_b
     * @tc.desc Verify Bluetooth API gatts onBlePhyUpdate boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_onBlePhyUpdate_b', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattServer.onBlePhyUpdate(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ONBLEPHYUPDATE_R
     * @tc.name gatts_onBlePhyUpdate_r
     * @tc.desc Verify Bluetooth API gatts onBlePhyUpdate return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_onBlePhyUpdate_r', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.onBlePhyUpdate(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ONBLEPHYUPDATE_X
     * @tc.name gatts_onBlePhyUpdate_x
     * @tc.desc Verify Bluetooth API gatts onBlePhyUpdate repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_onBlePhyUpdate_x', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.onBlePhyUpdate(() => {});
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
        gattServer.onBlePhyUpdate(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ONBLEPHYUPDATE_C
     * @tc.name gatts_onBlePhyUpdate_c
     * @tc.desc Verify Bluetooth API gatts onBlePhyUpdate condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_onBlePhyUpdate_c', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.onBlePhyUpdate(() => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ONBLEPHYUPDATE_PERF
     * @tc.name gatts_onBlePhyUpdate_perf
     * @tc.desc Verify Bluetooth apitest case gatts onBlePhyUpdate perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_onBlePhyUpdate_perf', 0, () => {
      runPerf('gatts_onBlePhyUpdate_perf', 5000, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_ONBLEPHYUPDATE_STRESS
     * @tc.name gatts_onBlePhyUpdate_stress
     * @tc.desc Verify Bluetooth apitest case gatts onBlePhyUpdate stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_onBlePhyUpdate_stress', 0, () => {
      runStress('gatts_onBlePhyUpdate_stress', 20, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });
    });
    describe('gatts_offBlePhyUpdate', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFFBLEPHYUPDATE_N
     * @tc.name gatts_offBlePhyUpdate_n
     * @tc.desc Verify Bluetooth API gatts offBlePhyUpdate normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_offBlePhyUpdate_n', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattServer.offBlePhyUpdate();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFFBLEPHYUPDATE_E
     * @tc.name gatts_offBlePhyUpdate_e
     * @tc.desc Verify Bluetooth API gatts offBlePhyUpdate error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_offBlePhyUpdate_e', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattServer.offBlePhyUpdate();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFFBLEPHYUPDATE_B
     * @tc.name gatts_offBlePhyUpdate_b
     * @tc.desc Verify Bluetooth API gatts offBlePhyUpdate boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_offBlePhyUpdate_b', 0, () => {
      const gattServer = ble.createGattServer();
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattServer.offBlePhyUpdate();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFFBLEPHYUPDATE_R
     * @tc.name gatts_offBlePhyUpdate_r
     * @tc.desc Verify Bluetooth API gatts offBlePhyUpdate return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_offBlePhyUpdate_r', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattServer.offBlePhyUpdate();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFFBLEPHYUPDATE_X
     * @tc.name gatts_offBlePhyUpdate_x
     * @tc.desc Verify Bluetooth API gatts offBlePhyUpdate repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_offBlePhyUpdate_x', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattServer.offBlePhyUpdate();
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
        gattServer.offBlePhyUpdate();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFFBLEPHYUPDATE_C
     * @tc.name gatts_offBlePhyUpdate_c
     * @tc.desc Verify Bluetooth API gatts offBlePhyUpdate condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_offBlePhyUpdate_c', 0, () => {
      const gattServer = ble.createGattServer();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattServer.offBlePhyUpdate();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFFBLEPHYUPDATE_PERF
     * @tc.name gatts_offBlePhyUpdate_perf
     * @tc.desc Verify Bluetooth apitest case gatts offBlePhyUpdate perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_offBlePhyUpdate_perf', 0, () => {
      runPerf('gatts_offBlePhyUpdate_perf', 5000, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTS_OFFBLEPHYUPDATE_STRESS
     * @tc.name gatts_offBlePhyUpdate_stress
     * @tc.desc Verify Bluetooth apitest case gatts offBlePhyUpdate stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gatts_offBlePhyUpdate_stress', 0, () => {
      runStress('gatts_offBlePhyUpdate_stress', 20, () => {
        try {
          ble.createGattServer();
        } catch (_e) {
        }
      });
    });
    });
    describe('blescan_startScan', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_N
     * @tc.name blescan_startScan_n
     * @tc.desc Verify Bluetooth API blescan startScan normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_n', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        sc.startScan([], {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_E
     * @tc.name blescan_startScan_e
     * @tc.desc Verify Bluetooth API blescan startScan error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_e', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        sc.startScan([], {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_B
     * @tc.name blescan_startScan_b
     * @tc.desc Verify Bluetooth API blescan startScan boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_b', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        sc.startScan([], {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_R
     * @tc.name blescan_startScan_r
     * @tc.desc Verify Bluetooth API blescan startScan return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_r', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        sc.startScan([], {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_X
     * @tc.name blescan_startScan_x
     * @tc.desc Verify Bluetooth API blescan startScan repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_x', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        sc.startScan([], {});
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
        sc.startScan([], {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_C
     * @tc.name blescan_startScan_c
     * @tc.desc Verify Bluetooth API blescan startScan condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_c', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        sc.startScan([], {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_PERF
     * @tc.name blescan_startScan_perf
     * @tc.desc Verify Bluetooth apitest case blescan startScan perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_perf', 0, () => {
      runPerf('blescan_startScan_perf', 5000, () => {
        try {
          ble.createBleScanner();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STARTSCAN_STRESS
     * @tc.name blescan_startScan_stress
     * @tc.desc Verify Bluetooth apitest case blescan startScan stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_startScan_stress', 0, () => {
      runStress('blescan_startScan_stress', 20, () => {
        try {
          ble.createBleScanner();
        } catch (_e) {
        }
      });
    });
    });
    describe('blescan_stopScan', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_N
     * @tc.name blescan_stopScan_n
     * @tc.desc Verify Bluetooth API blescan stopScan normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_n', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        sc.stopScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_E
     * @tc.name blescan_stopScan_e
     * @tc.desc Verify Bluetooth API blescan stopScan error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_e', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      try {
        sc.stopScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_B
     * @tc.name blescan_stopScan_b
     * @tc.desc Verify Bluetooth API blescan stopScan boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_b', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        sc.stopScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_R
     * @tc.name blescan_stopScan_r
     * @tc.desc Verify Bluetooth API blescan stopScan return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_r', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        sc.stopScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_X
     * @tc.name blescan_stopScan_x
     * @tc.desc Verify Bluetooth API blescan stopScan repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_x', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        sc.stopScan();
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
        sc.stopScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_C
     * @tc.name blescan_stopScan_c
     * @tc.desc Verify Bluetooth API blescan stopScan condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_c', 0, () => {
      const sc = ble.createBleScanner();
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        sc.stopScan();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_PERF
     * @tc.name blescan_stopScan_perf
     * @tc.desc Verify Bluetooth apitest case blescan stopScan perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_perf', 0, () => {
      runPerf('blescan_stopScan_perf', 5000, () => {
        try {
          ble.createBleScanner();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_STOPSCAN_STRESS
     * @tc.name blescan_stopScan_stress
     * @tc.desc Verify Bluetooth apitest case blescan stopScan stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_stopScan_stress', 0, () => {
      runStress('blescan_stopScan_stress', 20, () => {
        try {
          ble.createBleScanner();
        } catch (_e) {
        }
      });
    });
    });
    describe('blescan_on_BLEDeviceFind', () => {
    it('blescan_on_BLEDeviceFind_n', 0, async (done: Function) => {
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
    it('blescan_on_BLEDeviceFind_e', 0, async (done: Function) => {
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
    it('blescan_on_BLEDeviceFind_b', 0, async (done: Function) => {
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
    it('blescan_on_BLEDeviceFind_r', 0, async (done: Function) => {
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
    it('blescan_on_BLEDeviceFind_x', 0, async (done: Function) => {
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
    it('blescan_on_BLEDeviceFind_c', 0, async (done: Function) => {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_ON_BLEDEVICEFIND_PERF
     * @tc.name blescan_on_BLEDeviceFind_perf
     * @tc.desc Verify Bluetooth apitest case blescan on BLEDeviceFind perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_on_BLEDeviceFind_perf', 0, () => {
      runPerf('blescan_on_BLEDeviceFind_perf', 5000, () => {
        try {
          ble.createBleScanner();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_ON_BLEDEVICEFIND_STRESS
     * @tc.name blescan_on_BLEDeviceFind_stress
     * @tc.desc Verify Bluetooth apitest case blescan on BLEDeviceFind stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_on_BLEDeviceFind_stress', 0, () => {
      runStress('blescan_on_BLEDeviceFind_stress', 20, () => {
        try {
          ble.createBleScanner();
        } catch (_e) {
        }
      });
    });
    });
    describe('blescan_off_BLEDeviceFind', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_OFF_BLEDEVICEFIND_N
     * @tc.name blescan_off_BLEDeviceFind_n
     * @tc.desc Verify Bluetooth API blescan off BLEDeviceFind normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_off_BLEDeviceFind_n', 0, () => {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_OFF_BLEDEVICEFIND_E
     * @tc.name blescan_off_BLEDeviceFind_e
     * @tc.desc Verify Bluetooth API blescan off BLEDeviceFind error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_off_BLEDeviceFind_e', 0, () => {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_OFF_BLEDEVICEFIND_B
     * @tc.name blescan_off_BLEDeviceFind_b
     * @tc.desc Verify Bluetooth API blescan off BLEDeviceFind boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_off_BLEDeviceFind_b', 0, () => {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_OFF_BLEDEVICEFIND_R
     * @tc.name blescan_off_BLEDeviceFind_r
     * @tc.desc Verify Bluetooth API blescan off BLEDeviceFind return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_off_BLEDeviceFind_r', 0, () => {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_OFF_BLEDEVICEFIND_X
     * @tc.name blescan_off_BLEDeviceFind_x
     * @tc.desc Verify Bluetooth API blescan off BLEDeviceFind repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_off_BLEDeviceFind_x', 0, () => {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_OFF_BLEDEVICEFIND_C
     * @tc.name blescan_off_BLEDeviceFind_c
     * @tc.desc Verify Bluetooth API blescan off BLEDeviceFind condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_off_BLEDeviceFind_c', 0, () => {
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_OFF_BLEDEVICEFIND_PERF
     * @tc.name blescan_off_BLEDeviceFind_perf
     * @tc.desc Verify Bluetooth apitest case blescan off BLEDeviceFind perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_off_BLEDeviceFind_perf', 0, () => {
      runPerf('blescan_off_BLEDeviceFind_perf', 5000, () => {
        try {
          ble.createBleScanner();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLESCAN_OFF_BLEDEVICEFIND_STRESS
     * @tc.name blescan_off_BLEDeviceFind_stress
     * @tc.desc Verify Bluetooth apitest case blescan off BLEDeviceFind stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('blescan_off_BLEDeviceFind_stress', 0, () => {
      runStress('blescan_off_BLEDeviceFind_stress', 20, () => {
        try {
          ble.createBleScanner();
        } catch (_e) {
        }
      });
    });
    });
  });
}


