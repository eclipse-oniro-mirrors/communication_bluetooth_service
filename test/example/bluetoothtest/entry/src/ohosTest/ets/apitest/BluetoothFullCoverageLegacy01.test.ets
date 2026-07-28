    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_STOPBLUETOOTHDISCOVERY_R
     * @tc.name leg_bt_stopBluetoothDiscovery_r
     * @tc.desc Verify Bluetooth API leg bt stopBluetoothDiscovery return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_stopBluetoothDiscovery_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetooth.stopBluetoothDiscovery();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_STOPBLUETOOTHDISCOVERY_X
     * @tc.name leg_bt_stopBluetoothDiscovery_x
     * @tc.desc Verify Bluetooth API leg bt stopBluetoothDiscovery repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_stopBluetoothDiscovery_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetooth.stopBluetoothDiscovery();
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
        bluetooth.stopBluetoothDiscovery();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_STOPBLUETOOTHDISCOVERY_C
     * @tc.name leg_bt_stopBluetoothDiscovery_c
     * @tc.desc Verify Bluetooth API leg bt stopBluetoothDiscovery condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_stopBluetoothDiscovery_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.stopBluetoothDiscovery();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_STOPBLUETOOTHDISCOVERY_PERF
     * @tc.name leg_bt_stopBluetoothDiscovery_perf
     * @tc.desc Verify Bluetooth apitest case leg bt stopBluetoothDiscovery perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_stopBluetoothDiscovery_perf', 0, () => {
      runPerf('leg_bt_stopBluetoothDiscovery_perf', 5000, () => {
        try {
          bluetooth.stopBluetoothDiscovery();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_STOPBLUETOOTHDISCOVERY_STRESS
     * @tc.name leg_bt_stopBluetoothDiscovery_stress
     * @tc.desc Verify Bluetooth apitest case leg bt stopBluetoothDiscovery stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_stopBluetoothDiscovery_stress', 0, () => {
      runStress('leg_bt_stopBluetoothDiscovery_stress', 20, () => {
        try {
          bluetooth.stopBluetoothDiscovery();
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_bt_sppListen', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPLISTEN_N
     * @tc.name leg_bt_sppListen_n
     * @tc.desc Verify Bluetooth API leg bt sppListen normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppListen_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.sppListen('h', legacySppOpt(), () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPLISTEN_E
     * @tc.name leg_bt_sppListen_e
     * @tc.desc Verify Bluetooth API leg bt sppListen error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppListen_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetooth.sppListen('', legacySppOpt(), () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPLISTEN_B
     * @tc.name leg_bt_sppListen_b
     * @tc.desc Verify Bluetooth API leg bt sppListen boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppListen_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetooth.sppListen('h', legacySppOpt(), () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPLISTEN_R
     * @tc.name leg_bt_sppListen_r
     * @tc.desc Verify Bluetooth API leg bt sppListen return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppListen_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetooth.sppListen('h', legacySppOpt(), () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPLISTEN_X
     * @tc.name leg_bt_sppListen_x
     * @tc.desc Verify Bluetooth API leg bt sppListen repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppListen_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetooth.sppListen('h', legacySppOpt(), () => {});
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
        bluetooth.sppListen('h', legacySppOpt(), () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPLISTEN_C
     * @tc.name leg_bt_sppListen_c
     * @tc.desc Verify Bluetooth API leg bt sppListen condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppListen_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.sppListen('h', legacySppOpt(), () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPLISTEN_PERF
     * @tc.name leg_bt_sppListen_perf
     * @tc.desc Verify Bluetooth apitest case leg bt sppListen perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppListen_perf', 0, () => {
      runPerf('leg_bt_sppListen_perf', 5000, () => {
        try {
          bluetooth.sppListen('h', legacySppOpt(), () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPLISTEN_STRESS
     * @tc.name leg_bt_sppListen_stress
     * @tc.desc Verify Bluetooth apitest case leg bt sppListen stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppListen_stress', 0, () => {
      runStress('leg_bt_sppListen_stress', 20, () => {
        try {
          bluetooth.sppListen('h', legacySppOpt(), () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_bt_sppAccept', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPACCEPT_N
     * @tc.name leg_bt_sppAccept_n
     * @tc.desc Verify Bluetooth API leg bt sppAccept normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppAccept_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.sppAccept(0, () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPACCEPT_E
     * @tc.name leg_bt_sppAccept_e
     * @tc.desc Verify Bluetooth API leg bt sppAccept error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppAccept_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetooth.sppAccept(-1, () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPACCEPT_B
     * @tc.name leg_bt_sppAccept_b
     * @tc.desc Verify Bluetooth API leg bt sppAccept boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppAccept_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetooth.sppAccept(0, () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPACCEPT_R
     * @tc.name leg_bt_sppAccept_r
     * @tc.desc Verify Bluetooth API leg bt sppAccept return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppAccept_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetooth.sppAccept(0, () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPACCEPT_X
     * @tc.name leg_bt_sppAccept_x
     * @tc.desc Verify Bluetooth API leg bt sppAccept repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppAccept_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetooth.sppAccept(0, () => {});
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
        bluetooth.sppAccept(0, () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPACCEPT_C
     * @tc.name leg_bt_sppAccept_c
     * @tc.desc Verify Bluetooth API leg bt sppAccept condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppAccept_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.sppAccept(0, () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPACCEPT_PERF
     * @tc.name leg_bt_sppAccept_perf
     * @tc.desc Verify Bluetooth apitest case leg bt sppAccept perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppAccept_perf', 0, () => {
      runPerf('leg_bt_sppAccept_perf', 5000, () => {
        try {
          bluetooth.sppAccept(0, () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPACCEPT_STRESS
     * @tc.name leg_bt_sppAccept_stress
     * @tc.desc Verify Bluetooth apitest case leg bt sppAccept stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppAccept_stress', 0, () => {
      runStress('leg_bt_sppAccept_stress', 20, () => {
        try {
          bluetooth.sppAccept(0, () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_bt_sppConnect', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCONNECT_N
     * @tc.name leg_bt_sppConnect_n
     * @tc.desc Verify Bluetooth API leg bt sppConnect normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppConnect_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.sppConnect(BADDR, legacySppOpt(), () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCONNECT_E
     * @tc.name leg_bt_sppConnect_e
     * @tc.desc Verify Bluetooth API leg bt sppConnect error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppConnect_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetooth.sppConnect(EMPTY_MAC, legacySppOpt(), () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCONNECT_B
     * @tc.name leg_bt_sppConnect_b
     * @tc.desc Verify Bluetooth API leg bt sppConnect boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppConnect_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.sppConnect(BAD_MAC, legacySppOpt(), () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCONNECT_R
     * @tc.name leg_bt_sppConnect_r
     * @tc.desc Verify Bluetooth API leg bt sppConnect return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppConnect_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetooth.sppConnect(BADDR, legacySppOpt(), () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCONNECT_X
     * @tc.name leg_bt_sppConnect_x
     * @tc.desc Verify Bluetooth API leg bt sppConnect repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppConnect_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetooth.sppConnect(BADDR, legacySppOpt(), () => {});
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
        bluetooth.sppConnect(BADDR, legacySppOpt(), () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCONNECT_C
     * @tc.name leg_bt_sppConnect_c
     * @tc.desc Verify Bluetooth API leg bt sppConnect condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppConnect_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.sppConnect(BADDR, legacySppOpt(), () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCONNECT_PERF
     * @tc.name leg_bt_sppConnect_perf
     * @tc.desc Verify Bluetooth apitest case leg bt sppConnect perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppConnect_perf', 0, () => {
      runPerf('leg_bt_sppConnect_perf', 5000, () => {
        try {
          bluetooth.sppConnect(BADDR, legacySppOpt(), () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCONNECT_STRESS
     * @tc.name leg_bt_sppConnect_stress
     * @tc.desc Verify Bluetooth apitest case leg bt sppConnect stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppConnect_stress', 0, () => {
      runStress('leg_bt_sppConnect_stress', 20, () => {
        try {
          bluetooth.sppConnect(BADDR, legacySppOpt(), () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_bt_sppCloseServerSocket', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSESERVERSOCKET_N
     * @tc.name leg_bt_sppCloseServerSocket_n
     * @tc.desc Verify Bluetooth API leg bt sppCloseServerSocket normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseServerSocket_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.sppCloseServerSocket(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSESERVERSOCKET_E
     * @tc.name leg_bt_sppCloseServerSocket_e
     * @tc.desc Verify Bluetooth API leg bt sppCloseServerSocket error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseServerSocket_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetooth.sppCloseServerSocket(-1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSESERVERSOCKET_B
     * @tc.name leg_bt_sppCloseServerSocket_b
     * @tc.desc Verify Bluetooth API leg bt sppCloseServerSocket boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseServerSocket_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetooth.sppCloseServerSocket(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSESERVERSOCKET_R
     * @tc.name leg_bt_sppCloseServerSocket_r
     * @tc.desc Verify Bluetooth API leg bt sppCloseServerSocket return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseServerSocket_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetooth.sppCloseServerSocket(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSESERVERSOCKET_X
     * @tc.name leg_bt_sppCloseServerSocket_x
     * @tc.desc Verify Bluetooth API leg bt sppCloseServerSocket repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseServerSocket_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetooth.sppCloseServerSocket(0);
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
        bluetooth.sppCloseServerSocket(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSESERVERSOCKET_C
     * @tc.name leg_bt_sppCloseServerSocket_c
     * @tc.desc Verify Bluetooth API leg bt sppCloseServerSocket condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseServerSocket_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.sppCloseServerSocket(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSESERVERSOCKET_PERF
     * @tc.name leg_bt_sppCloseServerSocket_perf
     * @tc.desc Verify Bluetooth apitest case leg bt sppCloseServerSocket perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseServerSocket_perf', 0, () => {
      runPerf('leg_bt_sppCloseServerSocket_perf', 5000, () => {
        try {
          bluetooth.sppCloseServerSocket(0);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSESERVERSOCKET_STRESS
     * @tc.name leg_bt_sppCloseServerSocket_stress
     * @tc.desc Verify Bluetooth apitest case leg bt sppCloseServerSocket stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseServerSocket_stress', 0, () => {
      runStress('leg_bt_sppCloseServerSocket_stress', 20, () => {
        try {
          bluetooth.sppCloseServerSocket(0);
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_bt_sppCloseClientSocket', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSECLIENTSOCKET_N
     * @tc.name leg_bt_sppCloseClientSocket_n
     * @tc.desc Verify Bluetooth API leg bt sppCloseClientSocket normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseClientSocket_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.sppCloseClientSocket(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSECLIENTSOCKET_E
     * @tc.name leg_bt_sppCloseClientSocket_e
     * @tc.desc Verify Bluetooth API leg bt sppCloseClientSocket error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseClientSocket_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetooth.sppCloseClientSocket(-1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSECLIENTSOCKET_B
     * @tc.name leg_bt_sppCloseClientSocket_b
     * @tc.desc Verify Bluetooth API leg bt sppCloseClientSocket boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseClientSocket_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetooth.sppCloseClientSocket(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSECLIENTSOCKET_R
     * @tc.name leg_bt_sppCloseClientSocket_r
     * @tc.desc Verify Bluetooth API leg bt sppCloseClientSocket return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseClientSocket_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetooth.sppCloseClientSocket(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSECLIENTSOCKET_X
     * @tc.name leg_bt_sppCloseClientSocket_x
     * @tc.desc Verify Bluetooth API leg bt sppCloseClientSocket repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseClientSocket_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetooth.sppCloseClientSocket(0);
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
        bluetooth.sppCloseClientSocket(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSECLIENTSOCKET_C
     * @tc.name leg_bt_sppCloseClientSocket_c
     * @tc.desc Verify Bluetooth API leg bt sppCloseClientSocket condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseClientSocket_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.sppCloseClientSocket(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSECLIENTSOCKET_PERF
     * @tc.name leg_bt_sppCloseClientSocket_perf
     * @tc.desc Verify Bluetooth apitest case leg bt sppCloseClientSocket perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseClientSocket_perf', 0, () => {
      runPerf('leg_bt_sppCloseClientSocket_perf', 5000, () => {
        try {
          bluetooth.sppCloseClientSocket(0);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPCLOSECLIENTSOCKET_STRESS
     * @tc.name leg_bt_sppCloseClientSocket_stress
     * @tc.desc Verify Bluetooth apitest case leg bt sppCloseClientSocket stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppCloseClientSocket_stress', 0, () => {
      runStress('leg_bt_sppCloseClientSocket_stress', 20, () => {
        try {
          bluetooth.sppCloseClientSocket(0);
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_bt_sppWrite', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPWRITE_N
     * @tc.name leg_bt_sppWrite_n
     * @tc.desc Verify Bluetooth API leg bt sppWrite normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppWrite_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.sppWrite(0, buf0());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPWRITE_E
     * @tc.name leg_bt_sppWrite_e
     * @tc.desc Verify Bluetooth API leg bt sppWrite error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppWrite_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetooth.sppWrite(-1, buf0());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPWRITE_B
     * @tc.name leg_bt_sppWrite_b
     * @tc.desc Verify Bluetooth API leg bt sppWrite boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppWrite_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetooth.sppWrite(0, buf0());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPWRITE_R
     * @tc.name leg_bt_sppWrite_r
     * @tc.desc Verify Bluetooth API leg bt sppWrite return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppWrite_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetooth.sppWrite(0, buf0());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPWRITE_X
     * @tc.name leg_bt_sppWrite_x
     * @tc.desc Verify Bluetooth API leg bt sppWrite repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppWrite_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetooth.sppWrite(0, buf0());
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
        bluetooth.sppWrite(0, buf0());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPWRITE_C
     * @tc.name leg_bt_sppWrite_c
     * @tc.desc Verify Bluetooth API leg bt sppWrite condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppWrite_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.sppWrite(0, buf0());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPWRITE_PERF
     * @tc.name leg_bt_sppWrite_perf
     * @tc.desc Verify Bluetooth apitest case leg bt sppWrite perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppWrite_perf', 0, () => {
      runPerf('leg_bt_sppWrite_perf', 5000, () => {
        try {
          bluetooth.sppWrite(0, buf0());
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_SPPWRITE_STRESS
     * @tc.name leg_bt_sppWrite_stress
     * @tc.desc Verify Bluetooth apitest case leg bt sppWrite stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_sppWrite_stress', 0, () => {
      runStress('leg_bt_sppWrite_stress', 20, () => {
        try {
          bluetooth.sppWrite(0, buf0());
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_bt_createGattServer', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTSERVER_N
     * @tc.name leg_bt_createGattServer_n
     * @tc.desc Verify Bluetooth API leg bt createGattServer normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattServer_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.BLE.createGattServer();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTSERVER_E
     * @tc.name leg_bt_createGattServer_e
     * @tc.desc Verify Bluetooth API leg bt createGattServer error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattServer_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetooth.BLE.createGattServer();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTSERVER_B
     * @tc.name leg_bt_createGattServer_b
     * @tc.desc Verify Bluetooth API leg bt createGattServer boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattServer_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetooth.BLE.createGattServer();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTSERVER_R
     * @tc.name leg_bt_createGattServer_r
     * @tc.desc Verify Bluetooth API leg bt createGattServer return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattServer_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetooth.BLE.createGattServer();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTSERVER_X
     * @tc.name leg_bt_createGattServer_x
     * @tc.desc Verify Bluetooth API leg bt createGattServer repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattServer_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetooth.BLE.createGattServer();
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
        bluetooth.BLE.createGattServer();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTSERVER_C
     * @tc.name leg_bt_createGattServer_c
     * @tc.desc Verify Bluetooth API leg bt createGattServer condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattServer_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.BLE.createGattServer();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTSERVER_PERF
     * @tc.name leg_bt_createGattServer_perf
     * @tc.desc Verify Bluetooth apitest case leg bt createGattServer perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattServer_perf', 0, () => {
      runPerf('leg_bt_createGattServer_perf', 5000, () => {
        try {
          bluetooth.BLE.createGattServer();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTSERVER_STRESS
     * @tc.name leg_bt_createGattServer_stress
     * @tc.desc Verify Bluetooth apitest case leg bt createGattServer stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattServer_stress', 0, () => {
      runStress('leg_bt_createGattServer_stress', 20, () => {
        try {
          bluetooth.BLE.createGattServer();
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_bt_createGattClientDevice', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTCLIENTDEVICE_N
     * @tc.name leg_bt_createGattClientDevice_n
     * @tc.desc Verify Bluetooth API leg bt createGattClientDevice normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattClientDevice_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.BLE.createGattClientDevice(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTCLIENTDEVICE_E
     * @tc.name leg_bt_createGattClientDevice_e
     * @tc.desc Verify Bluetooth API leg bt createGattClientDevice error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattClientDevice_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetooth.BLE.createGattClientDevice(EMPTY_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTCLIENTDEVICE_B
     * @tc.name leg_bt_createGattClientDevice_b
     * @tc.desc Verify Bluetooth API leg bt createGattClientDevice boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattClientDevice_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.BLE.createGattClientDevice(BAD_MAC);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTCLIENTDEVICE_R
     * @tc.name leg_bt_createGattClientDevice_r
     * @tc.desc Verify Bluetooth API leg bt createGattClientDevice return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattClientDevice_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetooth.BLE.createGattClientDevice(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTCLIENTDEVICE_X
     * @tc.name leg_bt_createGattClientDevice_x
     * @tc.desc Verify Bluetooth API leg bt createGattClientDevice repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattClientDevice_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetooth.BLE.createGattClientDevice(BADDR);
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
        bluetooth.BLE.createGattClientDevice(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTCLIENTDEVICE_C
     * @tc.name leg_bt_createGattClientDevice_c
     * @tc.desc Verify Bluetooth API leg bt createGattClientDevice condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattClientDevice_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.BLE.createGattClientDevice(BADDR);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTCLIENTDEVICE_PERF
     * @tc.name leg_bt_createGattClientDevice_perf
     * @tc.desc Verify Bluetooth apitest case leg bt createGattClientDevice perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattClientDevice_perf', 0, () => {
      runPerf('leg_bt_createGattClientDevice_perf', 5000, () => {
        try {
          bluetooth.BLE.createGattClientDevice(BADDR);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_CREATEGATTCLIENTDEVICE_STRESS
     * @tc.name leg_bt_createGattClientDevice_stress
     * @tc.desc Verify Bluetooth apitest case leg bt createGattClientDevice stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_createGattClientDevice_stress', 0, () => {
      runStress('leg_bt_createGattClientDevice_stress', 20, () => {
        try {
          bluetooth.BLE.createGattClientDevice(BADDR);
        } catch (_e) {
        }
      });
    });
    });
    describe('leg_bt_startBLEScan', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_STARTBLESCAN_N
     * @tc.name leg_bt_startBLEScan_n
     * @tc.desc Verify Bluetooth API leg bt startBLEScan normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_startBLEScan_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.BLE.startBLEScan([]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_STARTBLESCAN_E
     * @tc.name leg_bt_startBLEScan_e
     * @tc.desc Verify Bluetooth API leg bt startBLEScan error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_startBLEScan_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        bluetooth.BLE.startBLEScan([]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_STARTBLESCAN_B
     * @tc.name leg_bt_startBLEScan_b
     * @tc.desc Verify Bluetooth API leg bt startBLEScan boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_startBLEScan_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      try {
        bluetooth.BLE.startBLEScan([]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_STARTBLESCAN_R
     * @tc.name leg_bt_startBLEScan_r
     * @tc.desc Verify Bluetooth API leg bt startBLEScan return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_startBLEScan_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        bluetooth.BLE.startBLEScan([]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_STARTBLESCAN_X
     * @tc.name leg_bt_startBLEScan_x
     * @tc.desc Verify Bluetooth API leg bt startBLEScan repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_startBLEScan_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        bluetooth.BLE.startBLEScan([]);
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
        bluetooth.BLE.startBLEScan([]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_STARTBLESCAN_C
     * @tc.name leg_bt_startBLEScan_c
     * @tc.desc Verify Bluetooth API leg bt startBLEScan condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_startBLEScan_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        bluetooth.BLE.startBLEScan([]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_STARTBLESCAN_PERF
     * @tc.name leg_bt_startBLEScan_perf
     * @tc.desc Verify Bluetooth apitest case leg bt startBLEScan perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_startBLEScan_perf', 0, () => {
      runPerf('leg_bt_startBLEScan_perf', 5000, () => {
        try {
          bluetooth.BLE.startBLEScan([]);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_LEG_BT_STARTBLESCAN_STRESS
     * @tc.name leg_bt_startBLEScan_stress
     * @tc.desc Verify Bluetooth apitest case leg bt startBLEScan stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('leg_bt_startBLEScan_stress', 0, () => {
      runStress('leg_bt_startBLEScan_stress', 20, () => {
        try {
          bluetooth.BLE.startBLEScan([]);
        } catch (_e) {
        }
      });
    });
    });
  });
}


