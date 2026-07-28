      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(baseProfile.DisconnectCause.USER_DISCONNECT);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BP_DISCONNECTCAUSE_C
     * @tc.name bp_DisconnectCause_c
     * @tc.desc Verify Bluetooth API bp DisconnectCause condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bp_DisconnectCause_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(baseProfile.DisconnectCause.CONNECT_FAIL_INTERNAL);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BP_DISCONNECTCAUSE_PERF
     * @tc.name bp_DisconnectCause_perf
     * @tc.desc Verify Bluetooth apitest case bp DisconnectCause perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bp_DisconnectCause_perf', 0, () => {
      runPerf('bp_DisconnectCause_perf', 5000, () => {
        try {
          Number(baseProfile.DisconnectCause.USER_DISCONNECT);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BP_DISCONNECTCAUSE_STRESS
     * @tc.name bp_DisconnectCause_stress
     * @tc.desc Verify Bluetooth apitest case bp DisconnectCause stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bp_DisconnectCause_stress', 0, () => {
      runStress('bp_DisconnectCause_stress', 20, () => {
        try {
          Number(baseProfile.DisconnectCause.USER_DISCONNECT);
        } catch (_e) {
        }
      });
    });
    });
    describe('socket_SppType', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SOCKET_SPPTYPE_N
     * @tc.name socket_SppType_n
     * @tc.desc Verify Bluetooth API socket SppType normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('socket_SppType_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(socket.SppType.SPP_RFCOMM);
      expect(typeof _v).assertEqual('number');
      expect(Number(socket.SppType.SPP_RFCOMM)).assertEqual(0);
      expect(Number(socket.SppType.SPP_L2CAP)).assertEqual(1);
      expect(Number(socket.SppType.SPP_L2CAP_BLE)).assertEqual(2);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SOCKET_SPPTYPE_E
     * @tc.name socket_SppType_e
     * @tc.desc Verify Bluetooth API socket SppType error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('socket_SppType_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(socket.SppType.SPP_RFCOMM);
      const _b = Number(socket.SppType.SPP_L2CAP);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SOCKET_SPPTYPE_B
     * @tc.name socket_SppType_b
     * @tc.desc Verify Bluetooth API socket SppType boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('socket_SppType_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(socket.SppType.SPP_L2CAP_BLE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SOCKET_SPPTYPE_R
     * @tc.name socket_SppType_r
     * @tc.desc Verify Bluetooth API socket SppType return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('socket_SppType_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(socket.SppType.SPP_L2CAP);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SOCKET_SPPTYPE_X
     * @tc.name socket_SppType_x
     * @tc.desc Verify Bluetooth API socket SppType repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('socket_SppType_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(socket.SppType.SPP_RFCOMM);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(socket.SppType.SPP_RFCOMM);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SOCKET_SPPTYPE_C
     * @tc.name socket_SppType_c
     * @tc.desc Verify Bluetooth API socket SppType condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('socket_SppType_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(socket.SppType.SPP_L2CAP_BLE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SOCKET_SPPTYPE_PERF
     * @tc.name socket_SppType_perf
     * @tc.desc Verify Bluetooth apitest case socket SppType perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('socket_SppType_perf', 0, () => {
      runPerf('socket_SppType_perf', 5000, () => {
        try {
          Number(socket.SppType.SPP_RFCOMM);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SOCKET_SPPTYPE_STRESS
     * @tc.name socket_SppType_stress
     * @tc.desc Verify Bluetooth apitest case socket SppType stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('socket_SppType_stress', 0, () => {
      runStress('socket_SppType_stress', 20, () => {
        try {
          Number(socket.SppType.SPP_RFCOMM);
        } catch (_e) {
        }
      });
    });
    });
    describe('ble_GattWriteType', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTWRITETYPE_N
     * @tc.name ble_GattWriteType_n
     * @tc.desc Verify Bluetooth API ble GattWriteType normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattWriteType_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(ble.GattWriteType.WRITE);
      expect(typeof _v).assertEqual('number');
      expect(Number(ble.GattWriteType.WRITE)).assertEqual(1);
      expect(Number(ble.GattWriteType.WRITE_NO_RESPONSE)).assertEqual(2);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTWRITETYPE_E
     * @tc.name ble_GattWriteType_e
     * @tc.desc Verify Bluetooth API ble GattWriteType error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattWriteType_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(ble.GattWriteType.WRITE);
      const _b = Number(ble.GattWriteType.WRITE_NO_RESPONSE);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTWRITETYPE_B
     * @tc.name ble_GattWriteType_b
     * @tc.desc Verify Bluetooth API ble GattWriteType boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattWriteType_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(ble.GattWriteType.WRITE_NO_RESPONSE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTWRITETYPE_R
     * @tc.name ble_GattWriteType_r
     * @tc.desc Verify Bluetooth API ble GattWriteType return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattWriteType_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(ble.GattWriteType.WRITE_NO_RESPONSE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTWRITETYPE_X
     * @tc.name ble_GattWriteType_x
     * @tc.desc Verify Bluetooth API ble GattWriteType repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattWriteType_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(ble.GattWriteType.WRITE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(ble.GattWriteType.WRITE);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTWRITETYPE_C
     * @tc.name ble_GattWriteType_c
     * @tc.desc Verify Bluetooth API ble GattWriteType condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattWriteType_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(ble.GattWriteType.WRITE_NO_RESPONSE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTWRITETYPE_PERF
     * @tc.name ble_GattWriteType_perf
     * @tc.desc Verify Bluetooth apitest case ble GattWriteType perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattWriteType_perf', 0, () => {
      runPerf('ble_GattWriteType_perf', 5000, () => {
        try {
          Number(ble.GattWriteType.WRITE);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTWRITETYPE_STRESS
     * @tc.name ble_GattWriteType_stress
     * @tc.desc Verify Bluetooth apitest case ble GattWriteType stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattWriteType_stress', 0, () => {
      runStress('ble_GattWriteType_stress', 20, () => {
        try {
          Number(ble.GattWriteType.WRITE);
        } catch (_e) {
        }
      });
    });
    });
    describe('ble_ScanDuty', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANDUTY_N
     * @tc.name ble_ScanDuty_n
     * @tc.desc Verify Bluetooth API ble ScanDuty normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanDuty_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(ble.ScanDuty.SCAN_MODE_LOW_POWER);
      expect(typeof _v).assertEqual('number');
      expect(Number(ble.ScanDuty.SCAN_MODE_LOW_POWER)).assertEqual(0);
      expect(Number(ble.ScanDuty.SCAN_MODE_BALANCED)).assertEqual(1);
      expect(Number(ble.ScanDuty.SCAN_MODE_LOW_LATENCY)).assertEqual(2);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANDUTY_E
     * @tc.name ble_ScanDuty_e
     * @tc.desc Verify Bluetooth API ble ScanDuty error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanDuty_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(ble.ScanDuty.SCAN_MODE_LOW_POWER);
      const _b = Number(ble.ScanDuty.SCAN_MODE_BALANCED);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANDUTY_B
     * @tc.name ble_ScanDuty_b
     * @tc.desc Verify Bluetooth API ble ScanDuty boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanDuty_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(ble.ScanDuty.SCAN_MODE_LOW_LATENCY);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANDUTY_R
     * @tc.name ble_ScanDuty_r
     * @tc.desc Verify Bluetooth API ble ScanDuty return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanDuty_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(ble.ScanDuty.SCAN_MODE_BALANCED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANDUTY_X
     * @tc.name ble_ScanDuty_x
     * @tc.desc Verify Bluetooth API ble ScanDuty repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanDuty_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(ble.ScanDuty.SCAN_MODE_LOW_POWER);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(ble.ScanDuty.SCAN_MODE_LOW_POWER);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANDUTY_C
     * @tc.name ble_ScanDuty_c
     * @tc.desc Verify Bluetooth API ble ScanDuty condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanDuty_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(ble.ScanDuty.SCAN_MODE_LOW_LATENCY);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANDUTY_PERF
     * @tc.name ble_ScanDuty_perf
     * @tc.desc Verify Bluetooth apitest case ble ScanDuty perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanDuty_perf', 0, () => {
      runPerf('ble_ScanDuty_perf', 5000, () => {
        try {
          Number(ble.ScanDuty.SCAN_MODE_LOW_POWER);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANDUTY_STRESS
     * @tc.name ble_ScanDuty_stress
     * @tc.desc Verify Bluetooth apitest case ble ScanDuty stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanDuty_stress', 0, () => {
      runStress('ble_ScanDuty_stress', 20, () => {
        try {
          Number(ble.ScanDuty.SCAN_MODE_LOW_POWER);
        } catch (_e) {
        }
      });
    });
    });
    describe('ble_MatchMode', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_MATCHMODE_N
     * @tc.name ble_MatchMode_n
     * @tc.desc Verify Bluetooth API ble MatchMode normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_MatchMode_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(ble.MatchMode.MATCH_MODE_AGGRESSIVE);
      expect(typeof _v).assertEqual('number');
      expect(Number(ble.MatchMode.MATCH_MODE_AGGRESSIVE)).assertEqual(1);
      expect(Number(ble.MatchMode.MATCH_MODE_STICKY)).assertEqual(2);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_MATCHMODE_E
     * @tc.name ble_MatchMode_e
     * @tc.desc Verify Bluetooth API ble MatchMode error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_MatchMode_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(ble.MatchMode.MATCH_MODE_AGGRESSIVE);
      const _b = Number(ble.MatchMode.MATCH_MODE_STICKY);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_MATCHMODE_B
     * @tc.name ble_MatchMode_b
     * @tc.desc Verify Bluetooth API ble MatchMode boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_MatchMode_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(ble.MatchMode.MATCH_MODE_STICKY);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_MATCHMODE_R
     * @tc.name ble_MatchMode_r
     * @tc.desc Verify Bluetooth API ble MatchMode return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_MatchMode_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(ble.MatchMode.MATCH_MODE_STICKY);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_MATCHMODE_X
     * @tc.name ble_MatchMode_x
     * @tc.desc Verify Bluetooth API ble MatchMode repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_MatchMode_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(ble.MatchMode.MATCH_MODE_AGGRESSIVE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(ble.MatchMode.MATCH_MODE_AGGRESSIVE);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_MATCHMODE_C
     * @tc.name ble_MatchMode_c
     * @tc.desc Verify Bluetooth API ble MatchMode condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_MatchMode_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(ble.MatchMode.MATCH_MODE_STICKY);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_MATCHMODE_PERF
     * @tc.name ble_MatchMode_perf
     * @tc.desc Verify Bluetooth apitest case ble MatchMode perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_MatchMode_perf', 0, () => {
      runPerf('ble_MatchMode_perf', 5000, () => {
        try {
          Number(ble.MatchMode.MATCH_MODE_AGGRESSIVE);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_MATCHMODE_STRESS
     * @tc.name ble_MatchMode_stress
     * @tc.desc Verify Bluetooth apitest case ble MatchMode stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_MatchMode_stress', 0, () => {
      runStress('ble_MatchMode_stress', 20, () => {
        try {
          Number(ble.MatchMode.MATCH_MODE_AGGRESSIVE);
        } catch (_e) {
        }
      });
    });
    });
    describe('ble_AdvertisingState', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_ADVERTISINGSTATE_N
     * @tc.name ble_AdvertisingState_n
     * @tc.desc Verify Bluetooth API ble AdvertisingState normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_AdvertisingState_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(ble.AdvertisingState.STARTED);
      expect(typeof _v).assertEqual('number');
      expect(Number(ble.AdvertisingState.STARTED)).assertEqual(1);
      expect(Number(ble.AdvertisingState.ENABLED)).assertEqual(2);
      expect(Number(ble.AdvertisingState.DISABLED)).assertEqual(3);
      expect(Number(ble.AdvertisingState.STOPPED)).assertEqual(4);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_ADVERTISINGSTATE_E
     * @tc.name ble_AdvertisingState_e
     * @tc.desc Verify Bluetooth API ble AdvertisingState error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_AdvertisingState_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(ble.AdvertisingState.STARTED);
      const _b = Number(ble.AdvertisingState.ENABLED);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_ADVERTISINGSTATE_B
     * @tc.name ble_AdvertisingState_b
     * @tc.desc Verify Bluetooth API ble AdvertisingState boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_AdvertisingState_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(ble.AdvertisingState.STOPPED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_ADVERTISINGSTATE_R
     * @tc.name ble_AdvertisingState_r
     * @tc.desc Verify Bluetooth API ble AdvertisingState return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_AdvertisingState_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(ble.AdvertisingState.DISABLED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_ADVERTISINGSTATE_X
     * @tc.name ble_AdvertisingState_x
     * @tc.desc Verify Bluetooth API ble AdvertisingState repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_AdvertisingState_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(ble.AdvertisingState.STARTED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(ble.AdvertisingState.STARTED);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_ADVERTISINGSTATE_C
     * @tc.name ble_AdvertisingState_c
     * @tc.desc Verify Bluetooth API ble AdvertisingState condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_AdvertisingState_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(ble.AdvertisingState.STOPPED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_ADVERTISINGSTATE_PERF
     * @tc.name ble_AdvertisingState_perf
     * @tc.desc Verify Bluetooth apitest case ble AdvertisingState perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_AdvertisingState_perf', 0, () => {
      runPerf('ble_AdvertisingState_perf', 5000, () => {
        try {
          Number(ble.AdvertisingState.STARTED);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_ADVERTISINGSTATE_STRESS
     * @tc.name ble_AdvertisingState_stress
     * @tc.desc Verify Bluetooth apitest case ble AdvertisingState stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_AdvertisingState_stress', 0, () => {
      runStress('ble_AdvertisingState_stress', 20, () => {
        try {
          Number(ble.AdvertisingState.STARTED);
        } catch (_e) {
        }
      });
    });
    });
    describe('ble_PhyType', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_PHYTYPE_N
     * @tc.name ble_PhyType_n
     * @tc.desc Verify Bluetooth API ble PhyType normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_PhyType_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(ble.PhyType.PHY_LE_1M);
      expect(typeof _v).assertEqual('number');
      expect(Number(ble.PhyType.PHY_LE_1M)).assertEqual(1);
      expect(Number(ble.PhyType.PHY_LE_ALL_SUPPORTED)).assertEqual(255);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_PHYTYPE_E
     * @tc.name ble_PhyType_e
     * @tc.desc Verify Bluetooth API ble PhyType error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_PhyType_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(ble.PhyType.PHY_LE_1M);
      const _b = Number(ble.PhyType.PHY_LE_ALL_SUPPORTED);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_PHYTYPE_B
     * @tc.name ble_PhyType_b
     * @tc.desc Verify Bluetooth API ble PhyType boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_PhyType_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(ble.PhyType.PHY_LE_ALL_SUPPORTED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_PHYTYPE_R
     * @tc.name ble_PhyType_r
     * @tc.desc Verify Bluetooth API ble PhyType return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_PhyType_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(ble.PhyType.PHY_LE_ALL_SUPPORTED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_PHYTYPE_X
     * @tc.name ble_PhyType_x
     * @tc.desc Verify Bluetooth API ble PhyType repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_PhyType_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(ble.PhyType.PHY_LE_1M);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(ble.PhyType.PHY_LE_1M);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_PHYTYPE_C
     * @tc.name ble_PhyType_c
     * @tc.desc Verify Bluetooth API ble PhyType condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_PhyType_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(ble.PhyType.PHY_LE_ALL_SUPPORTED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_PHYTYPE_PERF
     * @tc.name ble_PhyType_perf
     * @tc.desc Verify Bluetooth apitest case ble PhyType perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_PhyType_perf', 0, () => {
      runPerf('ble_PhyType_perf', 5000, () => {
        try {
          Number(ble.PhyType.PHY_LE_1M);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_PHYTYPE_STRESS
     * @tc.name ble_PhyType_stress
     * @tc.desc Verify Bluetooth apitest case ble PhyType stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_PhyType_stress', 0, () => {
      runStress('ble_PhyType_stress', 20, () => {
        try {
          Number(ble.PhyType.PHY_LE_1M);
        } catch (_e) {
        }
      });
    });
    });
    describe('ble_ScanReportMode', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTMODE_N
     * @tc.name ble_ScanReportMode_n
     * @tc.desc Verify Bluetooth API ble ScanReportMode normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportMode_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(ble.ScanReportMode.NORMAL);
      expect(typeof _v).assertEqual('number');
      expect(Number(ble.ScanReportMode.NORMAL)).assertEqual(1);
      expect(Number(ble.ScanReportMode.BATCH)).assertEqual(2);
      expect(Number(ble.ScanReportMode.FENCE_SENSITIVITY_LOW)).assertEqual(10);
      expect(Number(ble.ScanReportMode.FENCE_SENSITIVITY_HIGH)).assertEqual(11);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTMODE_E
     * @tc.name ble_ScanReportMode_e
     * @tc.desc Verify Bluetooth API ble ScanReportMode error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportMode_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(ble.ScanReportMode.NORMAL);
      const _b = Number(ble.ScanReportMode.BATCH);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTMODE_B
     * @tc.name ble_ScanReportMode_b
     * @tc.desc Verify Bluetooth API ble ScanReportMode boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportMode_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(ble.ScanReportMode.FENCE_SENSITIVITY_HIGH);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTMODE_R
     * @tc.name ble_ScanReportMode_r
     * @tc.desc Verify Bluetooth API ble ScanReportMode return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportMode_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(ble.ScanReportMode.FENCE_SENSITIVITY_LOW);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTMODE_X
     * @tc.name ble_ScanReportMode_x
     * @tc.desc Verify Bluetooth API ble ScanReportMode repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportMode_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(ble.ScanReportMode.NORMAL);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(ble.ScanReportMode.NORMAL);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTMODE_C
     * @tc.name ble_ScanReportMode_c
     * @tc.desc Verify Bluetooth API ble ScanReportMode condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportMode_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(ble.ScanReportMode.FENCE_SENSITIVITY_HIGH);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTMODE_PERF
     * @tc.name ble_ScanReportMode_perf
     * @tc.desc Verify Bluetooth apitest case ble ScanReportMode perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportMode_perf', 0, () => {
      runPerf('ble_ScanReportMode_perf', 5000, () => {
        try {
          Number(ble.ScanReportMode.NORMAL);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTMODE_STRESS
     * @tc.name ble_ScanReportMode_stress
     * @tc.desc Verify Bluetooth apitest case ble ScanReportMode stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportMode_stress', 0, () => {
      runStress('ble_ScanReportMode_stress', 20, () => {
        try {
          Number(ble.ScanReportMode.NORMAL);
        } catch (_e) {
        }
      });
    });
    });
    describe('ble_ScanReportType', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTTYPE_N
     * @tc.name ble_ScanReportType_n
     * @tc.desc Verify Bluetooth API ble ScanReportType normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportType_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(ble.ScanReportType.ON_FOUND);
      expect(typeof _v).assertEqual('number');
      expect(Number(ble.ScanReportType.ON_FOUND)).assertEqual(1);
      expect(Number(ble.ScanReportType.ON_LOST)).assertEqual(2);
      expect(Number(ble.ScanReportType.ON_BATCH)).assertEqual(3);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTTYPE_E
     * @tc.name ble_ScanReportType_e
     * @tc.desc Verify Bluetooth API ble ScanReportType error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportType_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(ble.ScanReportType.ON_FOUND);
      const _b = Number(ble.ScanReportType.ON_LOST);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTTYPE_B
     * @tc.name ble_ScanReportType_b
     * @tc.desc Verify Bluetooth API ble ScanReportType boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportType_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(ble.ScanReportType.ON_BATCH);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTTYPE_R
     * @tc.name ble_ScanReportType_r
     * @tc.desc Verify Bluetooth API ble ScanReportType return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportType_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(ble.ScanReportType.ON_LOST);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTTYPE_X
     * @tc.name ble_ScanReportType_x
     * @tc.desc Verify Bluetooth API ble ScanReportType repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportType_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(ble.ScanReportType.ON_FOUND);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(ble.ScanReportType.ON_FOUND);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTTYPE_C
     * @tc.name ble_ScanReportType_c
     * @tc.desc Verify Bluetooth API ble ScanReportType condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportType_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(ble.ScanReportType.ON_BATCH);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTTYPE_PERF
     * @tc.name ble_ScanReportType_perf
     * @tc.desc Verify Bluetooth apitest case ble ScanReportType perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportType_perf', 0, () => {
      runPerf('ble_ScanReportType_perf', 5000, () => {
        try {
          Number(ble.ScanReportType.ON_FOUND);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_SCANREPORTTYPE_STRESS
     * @tc.name ble_ScanReportType_stress
     * @tc.desc Verify Bluetooth apitest case ble ScanReportType stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ScanReportType_stress', 0, () => {
      runStress('ble_ScanReportType_stress', 20, () => {
        try {
          Number(ble.ScanReportType.ON_FOUND);
        } catch (_e) {
        }
      });
    });
    });
    describe('ble_BleProfile', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPROFILE_N
     * @tc.name ble_BleProfile_n
     * @tc.desc Verify Bluetooth API ble BleProfile normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BleProfile_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(ble.BleProfile.GATT);
      expect(typeof _v).assertEqual('number');
      expect(Number(ble.BleProfile.GATT)).assertEqual(1);
      expect(Number(ble.BleProfile.GATT_CLIENT)).assertEqual(2);
      expect(Number(ble.BleProfile.GATT_SERVER)).assertEqual(3);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPROFILE_E
     * @tc.name ble_BleProfile_e
     * @tc.desc Verify Bluetooth API ble BleProfile error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BleProfile_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(ble.BleProfile.GATT);
      const _b = Number(ble.BleProfile.GATT_CLIENT);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPROFILE_B
     * @tc.name ble_BleProfile_b
     * @tc.desc Verify Bluetooth API ble BleProfile boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BleProfile_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(ble.BleProfile.GATT_SERVER);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPROFILE_R
     * @tc.name ble_BleProfile_r
     * @tc.desc Verify Bluetooth API ble BleProfile return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BleProfile_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(ble.BleProfile.GATT_CLIENT);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPROFILE_X
     * @tc.name ble_BleProfile_x
     * @tc.desc Verify Bluetooth API ble BleProfile repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BleProfile_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(ble.BleProfile.GATT);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(ble.BleProfile.GATT);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPROFILE_C
     * @tc.name ble_BleProfile_c
     * @tc.desc Verify Bluetooth API ble BleProfile condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BleProfile_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(ble.BleProfile.GATT_SERVER);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPROFILE_PERF
     * @tc.name ble_BleProfile_perf
     * @tc.desc Verify Bluetooth apitest case ble BleProfile perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BleProfile_perf', 0, () => {
      runPerf('ble_BleProfile_perf', 5000, () => {
        try {
          Number(ble.BleProfile.GATT);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPROFILE_STRESS
     * @tc.name ble_BleProfile_stress
     * @tc.desc Verify Bluetooth apitest case ble BleProfile stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BleProfile_stress', 0, () => {
      runStress('ble_BleProfile_stress', 20, () => {
        try {
          Number(ble.BleProfile.GATT);
        } catch (_e) {
        }
      });
    });
    });
    describe('ble_ConnectionParam', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CONNECTIONPARAM_N
     * @tc.name ble_ConnectionParam_n
     * @tc.desc Verify Bluetooth API ble ConnectionParam normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ConnectionParam_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(ble.ConnectionParam.LOW_POWER);
      expect(typeof _v).assertEqual('number');
      expect(Number(ble.ConnectionParam.LOW_POWER)).assertEqual(1);
      expect(Number(ble.ConnectionParam.BALANCED)).assertEqual(2);
      expect(Number(ble.ConnectionParam.HIGH)).assertEqual(3);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CONNECTIONPARAM_E
     * @tc.name ble_ConnectionParam_e
     * @tc.desc Verify Bluetooth API ble ConnectionParam error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ConnectionParam_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(ble.ConnectionParam.LOW_POWER);
      const _b = Number(ble.ConnectionParam.BALANCED);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CONNECTIONPARAM_B
     * @tc.name ble_ConnectionParam_b
     * @tc.desc Verify Bluetooth API ble ConnectionParam boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ConnectionParam_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(ble.ConnectionParam.HIGH);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CONNECTIONPARAM_R
     * @tc.name ble_ConnectionParam_r
     * @tc.desc Verify Bluetooth API ble ConnectionParam return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ConnectionParam_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(ble.ConnectionParam.BALANCED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CONNECTIONPARAM_X
     * @tc.name ble_ConnectionParam_x
     * @tc.desc Verify Bluetooth API ble ConnectionParam repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ConnectionParam_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(ble.ConnectionParam.LOW_POWER);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(ble.ConnectionParam.LOW_POWER);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CONNECTIONPARAM_C
     * @tc.name ble_ConnectionParam_c
     * @tc.desc Verify Bluetooth API ble ConnectionParam condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ConnectionParam_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(ble.ConnectionParam.HIGH);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CONNECTIONPARAM_PERF
     * @tc.name ble_ConnectionParam_perf
     * @tc.desc Verify Bluetooth apitest case ble ConnectionParam perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ConnectionParam_perf', 0, () => {
      runPerf('ble_ConnectionParam_perf', 5000, () => {
        try {
          Number(ble.ConnectionParam.LOW_POWER);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CONNECTIONPARAM_STRESS
     * @tc.name ble_ConnectionParam_stress
     * @tc.desc Verify Bluetooth apitest case ble ConnectionParam stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_ConnectionParam_stress', 0, () => {
      runStress('ble_ConnectionParam_stress', 20, () => {
        try {
          Number(ble.ConnectionParam.LOW_POWER);
        } catch (_e) {
        }
      });
    });
    });
    describe('ble_GattDisconnectReason', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTDISCONNECTREASON_N
     * @tc.name ble_GattDisconnectReason_n
     * @tc.desc Verify Bluetooth API ble GattDisconnectReason normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattDisconnectReason_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(ble.GattDisconnectReason.CONN_TIMEOUT);
      expect(typeof _v).assertEqual('number');
      expect(Number(ble.GattDisconnectReason.CONN_TIMEOUT)).assertEqual(1);
      expect(Number(ble.GattDisconnectReason.CONN_TERMINATE_PEER_USER)).assertEqual(2);
      expect(Number(ble.GattDisconnectReason.CONN_TERMINATE_LOCAL_HOST)).assertEqual(3);
      expect(Number(ble.GattDisconnectReason.CONN_UNKNOWN)).assertEqual(4);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTDISCONNECTREASON_E
     * @tc.name ble_GattDisconnectReason_e
     * @tc.desc Verify Bluetooth API ble GattDisconnectReason error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattDisconnectReason_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(ble.GattDisconnectReason.CONN_TIMEOUT);
      const _b = Number(ble.GattDisconnectReason.CONN_TERMINATE_PEER_USER);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTDISCONNECTREASON_B
     * @tc.name ble_GattDisconnectReason_b
     * @tc.desc Verify Bluetooth API ble GattDisconnectReason boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattDisconnectReason_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(ble.GattDisconnectReason.CONN_UNKNOWN);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTDISCONNECTREASON_R
     * @tc.name ble_GattDisconnectReason_r
     * @tc.desc Verify Bluetooth API ble GattDisconnectReason return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattDisconnectReason_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(ble.GattDisconnectReason.CONN_TERMINATE_LOCAL_HOST);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTDISCONNECTREASON_X
     * @tc.name ble_GattDisconnectReason_x
     * @tc.desc Verify Bluetooth API ble GattDisconnectReason repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattDisconnectReason_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(ble.GattDisconnectReason.CONN_TIMEOUT);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(ble.GattDisconnectReason.CONN_TIMEOUT);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTDISCONNECTREASON_C
     * @tc.name ble_GattDisconnectReason_c
     * @tc.desc Verify Bluetooth API ble GattDisconnectReason condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattDisconnectReason_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(ble.GattDisconnectReason.CONN_UNKNOWN);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTDISCONNECTREASON_PERF
     * @tc.name ble_GattDisconnectReason_perf
     * @tc.desc Verify Bluetooth apitest case ble GattDisconnectReason perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattDisconnectReason_perf', 0, () => {
      runPerf('ble_GattDisconnectReason_perf', 5000, () => {
        try {
          Number(ble.GattDisconnectReason.CONN_TIMEOUT);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_GATTDISCONNECTREASON_STRESS
     * @tc.name ble_GattDisconnectReason_stress
     * @tc.desc Verify Bluetooth apitest case ble GattDisconnectReason stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_GattDisconnectReason_stress', 0, () => {
      runStress('ble_GattDisconnectReason_stress', 20, () => {
        try {
          Number(ble.GattDisconnectReason.CONN_TIMEOUT);
        } catch (_e) {
        }
      });
    });
    });
    describe('ble_BlePhy', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPHY_N
     * @tc.name ble_BlePhy_n
     * @tc.desc Verify Bluetooth API ble BlePhy normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BlePhy_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(ble.BlePhy.BLE_PHY_1M);
      expect(typeof _v).assertEqual('number');
      expect(Number(ble.BlePhy.BLE_PHY_1M)).assertEqual(1);
      expect(Number(ble.BlePhy.BLE_PHY_2M)).assertEqual(2);
      expect(Number(ble.BlePhy.BLE_PHY_CODED)).assertEqual(3);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPHY_E
     * @tc.name ble_BlePhy_e
     * @tc.desc Verify Bluetooth API ble BlePhy error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BlePhy_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(ble.BlePhy.BLE_PHY_1M);
      const _b = Number(ble.BlePhy.BLE_PHY_2M);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPHY_B
     * @tc.name ble_BlePhy_b
     * @tc.desc Verify Bluetooth API ble BlePhy boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BlePhy_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(ble.BlePhy.BLE_PHY_CODED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPHY_R
     * @tc.name ble_BlePhy_r
     * @tc.desc Verify Bluetooth API ble BlePhy return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BlePhy_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(ble.BlePhy.BLE_PHY_2M);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPHY_X
     * @tc.name ble_BlePhy_x
     * @tc.desc Verify Bluetooth API ble BlePhy repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BlePhy_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(ble.BlePhy.BLE_PHY_1M);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(ble.BlePhy.BLE_PHY_1M);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPHY_C
     * @tc.name ble_BlePhy_c
     * @tc.desc Verify Bluetooth API ble BlePhy condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BlePhy_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(ble.BlePhy.BLE_PHY_CODED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPHY_PERF
     * @tc.name ble_BlePhy_perf
     * @tc.desc Verify Bluetooth apitest case ble BlePhy perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BlePhy_perf', 0, () => {
      runPerf('ble_BlePhy_perf', 5000, () => {
        try {
          Number(ble.BlePhy.BLE_PHY_1M);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_BLEPHY_STRESS
     * @tc.name ble_BlePhy_stress
     * @tc.desc Verify Bluetooth apitest case ble BlePhy stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_BlePhy_stress', 0, () => {
      runStress('ble_BlePhy_stress', 20, () => {
        try {
          Number(ble.BlePhy.BLE_PHY_1M);
        } catch (_e) {
        }
      });
    });
    });
    describe('ble_CodedPhyMode', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CODEDPHYMODE_N
     * @tc.name ble_CodedPhyMode_n
     * @tc.desc Verify Bluetooth API ble CodedPhyMode normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_CodedPhyMode_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(ble.CodedPhyMode.BLE_PHY_CODED_S2);
      expect(typeof _v).assertEqual('number');
      expect(Number(ble.CodedPhyMode.BLE_PHY_CODED_S2)).assertEqual(1);
      expect(Number(ble.CodedPhyMode.BLE_PHY_CODED_S8)).assertEqual(2);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CODEDPHYMODE_E
     * @tc.name ble_CodedPhyMode_e
     * @tc.desc Verify Bluetooth API ble CodedPhyMode error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_CodedPhyMode_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(ble.CodedPhyMode.BLE_PHY_CODED_S2);
      const _b = Number(ble.CodedPhyMode.BLE_PHY_CODED_S8);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CODEDPHYMODE_B
     * @tc.name ble_CodedPhyMode_b
     * @tc.desc Verify Bluetooth API ble CodedPhyMode boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_CodedPhyMode_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(ble.CodedPhyMode.BLE_PHY_CODED_S8);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CODEDPHYMODE_R
     * @tc.name ble_CodedPhyMode_r
     * @tc.desc Verify Bluetooth API ble CodedPhyMode return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_CodedPhyMode_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(ble.CodedPhyMode.BLE_PHY_CODED_S8);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CODEDPHYMODE_X
     * @tc.name ble_CodedPhyMode_x
     * @tc.desc Verify Bluetooth API ble CodedPhyMode repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_CodedPhyMode_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(ble.CodedPhyMode.BLE_PHY_CODED_S2);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(ble.CodedPhyMode.BLE_PHY_CODED_S2);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CODEDPHYMODE_C
     * @tc.name ble_CodedPhyMode_c
     * @tc.desc Verify Bluetooth API ble CodedPhyMode condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_CodedPhyMode_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(ble.CodedPhyMode.BLE_PHY_CODED_S8);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CODEDPHYMODE_PERF
     * @tc.name ble_CodedPhyMode_perf
     * @tc.desc Verify Bluetooth apitest case ble CodedPhyMode perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_CodedPhyMode_perf', 0, () => {
      runPerf('ble_CodedPhyMode_perf', 5000, () => {
        try {
          Number(ble.CodedPhyMode.BLE_PHY_CODED_S2);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BLE_CODEDPHYMODE_STRESS
     * @tc.name ble_CodedPhyMode_stress
     * @tc.desc Verify Bluetooth apitest case ble CodedPhyMode stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ble_CodedPhyMode_stress', 0, () => {
      runStress('ble_CodedPhyMode_stress', 20, () => {
        try {
          Number(ble.CodedPhyMode.BLE_PHY_CODED_S2);
        } catch (_e) {
        }
      });
    });
    });
    describe('a2dp_CodecType', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECTYPE_N
     * @tc.name a2dp_CodecType_n
     * @tc.desc Verify Bluetooth API a2dp CodecType normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecType_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(a2dp.CodecType.CODEC_TYPE_INVALID);
      expect(typeof _v).assertEqual('number');
      expect(Number(a2dp.CodecType.CODEC_TYPE_INVALID)).assertEqual(-1);
      expect(Number(a2dp.CodecType.CODEC_TYPE_SBC)).assertEqual(0);
      expect(Number(a2dp.CodecType.CODEC_TYPE_AAC)).assertEqual(1);
      expect(Number(a2dp.CodecType.CODEC_TYPE_L2HC)).assertEqual(2);
      expect(Number(a2dp.CodecType.CODEC_TYPE_L2HCST)).assertEqual(3);
      expect(Number(a2dp.CodecType.CODEC_TYPE_LDAC)).assertEqual(4);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECTYPE_E
     * @tc.name a2dp_CodecType_e
     * @tc.desc Verify Bluetooth API a2dp CodecType error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecType_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(a2dp.CodecType.CODEC_TYPE_INVALID);
      const _b = Number(a2dp.CodecType.CODEC_TYPE_SBC);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECTYPE_B
     * @tc.name a2dp_CodecType_b
     * @tc.desc Verify Bluetooth API a2dp CodecType boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecType_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(a2dp.CodecType.CODEC_TYPE_LDAC);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECTYPE_R
     * @tc.name a2dp_CodecType_r
     * @tc.desc Verify Bluetooth API a2dp CodecType return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecType_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(a2dp.CodecType.CODEC_TYPE_L2HC);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECTYPE_X
     * @tc.name a2dp_CodecType_x
     * @tc.desc Verify Bluetooth API a2dp CodecType repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecType_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(a2dp.CodecType.CODEC_TYPE_INVALID);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(a2dp.CodecType.CODEC_TYPE_INVALID);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECTYPE_C
     * @tc.name a2dp_CodecType_c
     * @tc.desc Verify Bluetooth API a2dp CodecType condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecType_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(a2dp.CodecType.CODEC_TYPE_LDAC);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECTYPE_PERF
     * @tc.name a2dp_CodecType_perf
     * @tc.desc Verify Bluetooth apitest case a2dp CodecType perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecType_perf', 0, () => {
      runPerf('a2dp_CodecType_perf', 5000, () => {
        try {
          Number(a2dp.CodecType.CODEC_TYPE_INVALID);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECTYPE_STRESS
     * @tc.name a2dp_CodecType_stress
     * @tc.desc Verify Bluetooth apitest case a2dp CodecType stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecType_stress', 0, () => {
      runStress('a2dp_CodecType_stress', 20, () => {
        try {
          Number(a2dp.CodecType.CODEC_TYPE_INVALID);
        } catch (_e) {
        }
      });
    });
    });
    describe('a2dp_CodecChannelMode', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECCHANNELMODE_N
     * @tc.name a2dp_CodecChannelMode_n
     * @tc.desc Verify Bluetooth API a2dp CodecChannelMode normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecChannelMode_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(a2dp.CodecChannelMode.CODEC_CHANNEL_MODE_NONE);
      expect(typeof _v).assertEqual('number');
      expect(Number(a2dp.CodecChannelMode.CODEC_CHANNEL_MODE_NONE)).assertEqual(0);
      expect(Number(a2dp.CodecChannelMode.CODEC_CHANNEL_MODE_MONO)).assertEqual(1);
      expect(Number(a2dp.CodecChannelMode.CODEC_CHANNEL_MODE_STEREO)).assertEqual(2);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECCHANNELMODE_E
     * @tc.name a2dp_CodecChannelMode_e
     * @tc.desc Verify Bluetooth API a2dp CodecChannelMode error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecChannelMode_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(a2dp.CodecChannelMode.CODEC_CHANNEL_MODE_NONE);
      const _b = Number(a2dp.CodecChannelMode.CODEC_CHANNEL_MODE_MONO);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECCHANNELMODE_B
     * @tc.name a2dp_CodecChannelMode_b
     * @tc.desc Verify Bluetooth API a2dp CodecChannelMode boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecChannelMode_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(a2dp.CodecChannelMode.CODEC_CHANNEL_MODE_STEREO);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECCHANNELMODE_R
     * @tc.name a2dp_CodecChannelMode_r
     * @tc.desc Verify Bluetooth API a2dp CodecChannelMode return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecChannelMode_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(a2dp.CodecChannelMode.CODEC_CHANNEL_MODE_MONO);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECCHANNELMODE_X
     * @tc.name a2dp_CodecChannelMode_x
     * @tc.desc Verify Bluetooth API a2dp CodecChannelMode repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecChannelMode_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(a2dp.CodecChannelMode.CODEC_CHANNEL_MODE_NONE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(a2dp.CodecChannelMode.CODEC_CHANNEL_MODE_NONE);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECCHANNELMODE_C
     * @tc.name a2dp_CodecChannelMode_c
     * @tc.desc Verify Bluetooth API a2dp CodecChannelMode condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecChannelMode_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(a2dp.CodecChannelMode.CODEC_CHANNEL_MODE_STEREO);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECCHANNELMODE_PERF
     * @tc.name a2dp_CodecChannelMode_perf
     * @tc.desc Verify Bluetooth apitest case a2dp CodecChannelMode perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecChannelMode_perf', 0, () => {
      runPerf('a2dp_CodecChannelMode_perf', 5000, () => {
        try {
          Number(a2dp.CodecChannelMode.CODEC_CHANNEL_MODE_NONE);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECCHANNELMODE_STRESS
     * @tc.name a2dp_CodecChannelMode_stress
     * @tc.desc Verify Bluetooth apitest case a2dp CodecChannelMode stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecChannelMode_stress', 0, () => {
      runStress('a2dp_CodecChannelMode_stress', 20, () => {
        try {
          Number(a2dp.CodecChannelMode.CODEC_CHANNEL_MODE_NONE);
        } catch (_e) {
        }
      });
    });
    });
    describe('a2dp_CodecBitsPerSample', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITSPERSAMPLE_N
     * @tc.name a2dp_CodecBitsPerSample_n
     * @tc.desc Verify Bluetooth API a2dp CodecBitsPerSample normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitsPerSample_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(a2dp.CodecBitsPerSample.CODEC_BITS_PER_SAMPLE_NONE);
      expect(typeof _v).assertEqual('number');
      expect(Number(a2dp.CodecBitsPerSample.CODEC_BITS_PER_SAMPLE_NONE)).assertEqual(0);
      expect(Number(a2dp.CodecBitsPerSample.CODEC_BITS_PER_SAMPLE_16)).assertEqual(1);
      expect(Number(a2dp.CodecBitsPerSample.CODEC_BITS_PER_SAMPLE_24)).assertEqual(2);
      expect(Number(a2dp.CodecBitsPerSample.CODEC_BITS_PER_SAMPLE_32)).assertEqual(3);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITSPERSAMPLE_E
     * @tc.name a2dp_CodecBitsPerSample_e
     * @tc.desc Verify Bluetooth API a2dp CodecBitsPerSample error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitsPerSample_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(a2dp.CodecBitsPerSample.CODEC_BITS_PER_SAMPLE_NONE);
      const _b = Number(a2dp.CodecBitsPerSample.CODEC_BITS_PER_SAMPLE_16);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITSPERSAMPLE_B
     * @tc.name a2dp_CodecBitsPerSample_b
     * @tc.desc Verify Bluetooth API a2dp CodecBitsPerSample boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitsPerSample_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(a2dp.CodecBitsPerSample.CODEC_BITS_PER_SAMPLE_32);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITSPERSAMPLE_R
     * @tc.name a2dp_CodecBitsPerSample_r
     * @tc.desc Verify Bluetooth API a2dp CodecBitsPerSample return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitsPerSample_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(a2dp.CodecBitsPerSample.CODEC_BITS_PER_SAMPLE_24);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITSPERSAMPLE_X
     * @tc.name a2dp_CodecBitsPerSample_x
     * @tc.desc Verify Bluetooth API a2dp CodecBitsPerSample repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitsPerSample_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(a2dp.CodecBitsPerSample.CODEC_BITS_PER_SAMPLE_NONE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(a2dp.CodecBitsPerSample.CODEC_BITS_PER_SAMPLE_NONE);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITSPERSAMPLE_C
     * @tc.name a2dp_CodecBitsPerSample_c
     * @tc.desc Verify Bluetooth API a2dp CodecBitsPerSample condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitsPerSample_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(a2dp.CodecBitsPerSample.CODEC_BITS_PER_SAMPLE_32);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITSPERSAMPLE_PERF
     * @tc.name a2dp_CodecBitsPerSample_perf
     * @tc.desc Verify Bluetooth apitest case a2dp CodecBitsPerSample perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitsPerSample_perf', 0, () => {
      runPerf('a2dp_CodecBitsPerSample_perf', 5000, () => {
        try {
          Number(a2dp.CodecBitsPerSample.CODEC_BITS_PER_SAMPLE_NONE);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITSPERSAMPLE_STRESS
     * @tc.name a2dp_CodecBitsPerSample_stress
     * @tc.desc Verify Bluetooth apitest case a2dp CodecBitsPerSample stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitsPerSample_stress', 0, () => {
      runStress('a2dp_CodecBitsPerSample_stress', 20, () => {
        try {
          Number(a2dp.CodecBitsPerSample.CODEC_BITS_PER_SAMPLE_NONE);
        } catch (_e) {
        }
      });
    });
    });
    describe('a2dp_CodecSampleRate', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECSAMPLERATE_N
     * @tc.name a2dp_CodecSampleRate_n
     * @tc.desc Verify Bluetooth API a2dp CodecSampleRate normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecSampleRate_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_NONE);
      expect(typeof _v).assertEqual('number');
      expect(Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_NONE)).assertEqual(0);
      expect(Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_44100)).assertEqual(1);
      expect(Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_48000)).assertEqual(2);
      expect(Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_88200)).assertEqual(3);
      expect(Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_96000)).assertEqual(4);
      expect(Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_176400)).assertEqual(5);
      expect(Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_192000)).assertEqual(6);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECSAMPLERATE_E
     * @tc.name a2dp_CodecSampleRate_e
     * @tc.desc Verify Bluetooth API a2dp CodecSampleRate error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecSampleRate_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_NONE);
      const _b = Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_44100);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECSAMPLERATE_B
     * @tc.name a2dp_CodecSampleRate_b
     * @tc.desc Verify Bluetooth API a2dp CodecSampleRate boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecSampleRate_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_192000);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECSAMPLERATE_R
     * @tc.name a2dp_CodecSampleRate_r
     * @tc.desc Verify Bluetooth API a2dp CodecSampleRate return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecSampleRate_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_88200);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECSAMPLERATE_X
     * @tc.name a2dp_CodecSampleRate_x
     * @tc.desc Verify Bluetooth API a2dp CodecSampleRate repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecSampleRate_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_NONE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_NONE);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECSAMPLERATE_C
     * @tc.name a2dp_CodecSampleRate_c
     * @tc.desc Verify Bluetooth API a2dp CodecSampleRate condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecSampleRate_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_192000);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECSAMPLERATE_PERF
     * @tc.name a2dp_CodecSampleRate_perf
     * @tc.desc Verify Bluetooth apitest case a2dp CodecSampleRate perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecSampleRate_perf', 0, () => {
      runPerf('a2dp_CodecSampleRate_perf', 5000, () => {
        try {
          Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_NONE);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECSAMPLERATE_STRESS
     * @tc.name a2dp_CodecSampleRate_stress
     * @tc.desc Verify Bluetooth apitest case a2dp CodecSampleRate stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecSampleRate_stress', 0, () => {
      runStress('a2dp_CodecSampleRate_stress', 20, () => {
        try {
          Number(a2dp.CodecSampleRate.CODEC_SAMPLE_RATE_NONE);
        } catch (_e) {
        }
      });
    });
    });
    describe('a2dp_CodecBitRate', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITRATE_N
     * @tc.name a2dp_CodecBitRate_n
     * @tc.desc Verify Bluetooth API a2dp CodecBitRate normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitRate_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(a2dp.CodecBitRate.CODEC_BIT_RATE_96000);
      expect(typeof _v).assertEqual('number');
      expect(Number(a2dp.CodecBitRate.CODEC_BIT_RATE_96000)).assertEqual(0);
      expect(Number(a2dp.CodecBitRate.CODEC_BIT_RATE_128000)).assertEqual(1);
      expect(Number(a2dp.CodecBitRate.CODEC_BIT_RATE_192000)).assertEqual(2);
      expect(Number(a2dp.CodecBitRate.CODEC_BIT_RATE_256000)).assertEqual(3);
      expect(Number(a2dp.CodecBitRate.CODEC_BIT_RATE_320000)).assertEqual(4);
      expect(Number(a2dp.CodecBitRate.CODEC_BIT_RATE_480000)).assertEqual(5);
      expect(Number(a2dp.CodecBitRate.CODEC_BIT_RATE_640000)).assertEqual(6);
      expect(Number(a2dp.CodecBitRate.CODEC_BIT_RATE_960000)).assertEqual(7);
      expect(Number(a2dp.CodecBitRate.CODEC_BIT_RATE_ABR)).assertEqual(8);
      expect(Number(a2dp.CodecBitRate.CODEC_BIT_RATE_1500000)).assertEqual(9);
      expect(Number(a2dp.CodecBitRate.CODEC_BIT_RATE_2300000)).assertEqual(10);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITRATE_E
     * @tc.name a2dp_CodecBitRate_e
     * @tc.desc Verify Bluetooth API a2dp CodecBitRate error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitRate_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(a2dp.CodecBitRate.CODEC_BIT_RATE_96000);
      const _b = Number(a2dp.CodecBitRate.CODEC_BIT_RATE_128000);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITRATE_B
     * @tc.name a2dp_CodecBitRate_b
     * @tc.desc Verify Bluetooth API a2dp CodecBitRate boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitRate_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(a2dp.CodecBitRate.CODEC_BIT_RATE_2300000);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITRATE_R
     * @tc.name a2dp_CodecBitRate_r
     * @tc.desc Verify Bluetooth API a2dp CodecBitRate return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitRate_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(a2dp.CodecBitRate.CODEC_BIT_RATE_480000);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITRATE_X
     * @tc.name a2dp_CodecBitRate_x
     * @tc.desc Verify Bluetooth API a2dp CodecBitRate repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitRate_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(a2dp.CodecBitRate.CODEC_BIT_RATE_96000);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(a2dp.CodecBitRate.CODEC_BIT_RATE_96000);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITRATE_C
     * @tc.name a2dp_CodecBitRate_c
     * @tc.desc Verify Bluetooth API a2dp CodecBitRate condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitRate_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(a2dp.CodecBitRate.CODEC_BIT_RATE_2300000);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITRATE_PERF
     * @tc.name a2dp_CodecBitRate_perf
     * @tc.desc Verify Bluetooth apitest case a2dp CodecBitRate perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitRate_perf', 0, () => {
      runPerf('a2dp_CodecBitRate_perf', 5000, () => {
        try {
          Number(a2dp.CodecBitRate.CODEC_BIT_RATE_96000);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECBITRATE_STRESS
     * @tc.name a2dp_CodecBitRate_stress
     * @tc.desc Verify Bluetooth apitest case a2dp CodecBitRate stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecBitRate_stress', 0, () => {
      runStress('a2dp_CodecBitRate_stress', 20, () => {
        try {
          Number(a2dp.CodecBitRate.CODEC_BIT_RATE_96000);
        } catch (_e) {
        }
      });
    });
    });
    describe('a2dp_CodecFrameLength', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECFRAMELENGTH_N
     * @tc.name a2dp_CodecFrameLength_n
     * @tc.desc Verify Bluetooth API a2dp CodecFrameLength normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecFrameLength_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(a2dp.CodecFrameLength.CODEC_FRAME_LENGTH_5MS);
      expect(typeof _v).assertEqual('number');
      expect(Number(a2dp.CodecFrameLength.CODEC_FRAME_LENGTH_5MS)).assertEqual(0);
      expect(Number(a2dp.CodecFrameLength.CODEC_FRAME_LENGTH_10MS)).assertEqual(1);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECFRAMELENGTH_E
     * @tc.name a2dp_CodecFrameLength_e
     * @tc.desc Verify Bluetooth API a2dp CodecFrameLength error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecFrameLength_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(a2dp.CodecFrameLength.CODEC_FRAME_LENGTH_5MS);
      const _b = Number(a2dp.CodecFrameLength.CODEC_FRAME_LENGTH_10MS);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECFRAMELENGTH_B
     * @tc.name a2dp_CodecFrameLength_b
     * @tc.desc Verify Bluetooth API a2dp CodecFrameLength boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecFrameLength_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(a2dp.CodecFrameLength.CODEC_FRAME_LENGTH_10MS);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECFRAMELENGTH_R
     * @tc.name a2dp_CodecFrameLength_r
     * @tc.desc Verify Bluetooth API a2dp CodecFrameLength return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecFrameLength_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(a2dp.CodecFrameLength.CODEC_FRAME_LENGTH_10MS);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECFRAMELENGTH_X
     * @tc.name a2dp_CodecFrameLength_x
     * @tc.desc Verify Bluetooth API a2dp CodecFrameLength repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecFrameLength_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(a2dp.CodecFrameLength.CODEC_FRAME_LENGTH_5MS);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(a2dp.CodecFrameLength.CODEC_FRAME_LENGTH_5MS);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECFRAMELENGTH_C
     * @tc.name a2dp_CodecFrameLength_c
     * @tc.desc Verify Bluetooth API a2dp CodecFrameLength condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecFrameLength_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(a2dp.CodecFrameLength.CODEC_FRAME_LENGTH_10MS);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECFRAMELENGTH_PERF
     * @tc.name a2dp_CodecFrameLength_perf
     * @tc.desc Verify Bluetooth apitest case a2dp CodecFrameLength perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecFrameLength_perf', 0, () => {
      runPerf('a2dp_CodecFrameLength_perf', 5000, () => {
        try {
          Number(a2dp.CodecFrameLength.CODEC_FRAME_LENGTH_5MS);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_A2DP_CODECFRAMELENGTH_STRESS
     * @tc.name a2dp_CodecFrameLength_stress
     * @tc.desc Verify Bluetooth apitest case a2dp CodecFrameLength stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('a2dp_CodecFrameLength_stress', 0, () => {
      runStress('a2dp_CodecFrameLength_stress', 20, () => {
        try {
          Number(a2dp.CodecFrameLength.CODEC_FRAME_LENGTH_5MS);
        } catch (_e) {
        }
      });
    });
    });
    describe('hid_Subclass', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SUBCLASS_N
     * @tc.name hid_Subclass_n
     * @tc.desc Verify Bluetooth API hid Subclass normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_Subclass_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(hid.Subclass.SUBCLASS_UNCATEGORIZED);
      expect(typeof _v).assertEqual('number');
      expect(Number(hid.Subclass.SUBCLASS_UNCATEGORIZED)).assertEqual(0);
      expect(Number(hid.Subclass.SUBCLASS_JOYSTICK)).assertEqual(1);
      expect(Number(hid.Subclass.SUBCLASS_GAMEPAD)).assertEqual(2);
      expect(Number(hid.Subclass.SUBCLASS_REMOTE_CONTROL)).assertEqual(3);
      expect(Number(hid.Subclass.SUBCLASS_SENSING_DEVICE)).assertEqual(4);
      expect(Number(hid.Subclass.SUBCLASS_DIGITIZER_TABLET)).assertEqual(5);
      expect(Number(hid.Subclass.SUBCLASS_CARD_READER)).assertEqual(6);
      expect(Number(hid.Subclass.SUBCLASS_KEYBOARD)).assertEqual(64);
      expect(Number(hid.Subclass.SUBCLASS_MOUSE)).assertEqual(128);
      expect(Number(hid.Subclass.SUBCLASS_COMBO)).assertEqual(192);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SUBCLASS_E
     * @tc.name hid_Subclass_e
     * @tc.desc Verify Bluetooth API hid Subclass error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_Subclass_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(hid.Subclass.SUBCLASS_UNCATEGORIZED);
      const _b = Number(hid.Subclass.SUBCLASS_JOYSTICK);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SUBCLASS_B
     * @tc.name hid_Subclass_b
     * @tc.desc Verify Bluetooth API hid Subclass boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_Subclass_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(hid.Subclass.SUBCLASS_COMBO);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SUBCLASS_R
     * @tc.name hid_Subclass_r
     * @tc.desc Verify Bluetooth API hid Subclass return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_Subclass_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(hid.Subclass.SUBCLASS_DIGITIZER_TABLET);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SUBCLASS_X
     * @tc.name hid_Subclass_x
     * @tc.desc Verify Bluetooth API hid Subclass repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_Subclass_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(hid.Subclass.SUBCLASS_UNCATEGORIZED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(hid.Subclass.SUBCLASS_UNCATEGORIZED);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SUBCLASS_C
     * @tc.name hid_Subclass_c
     * @tc.desc Verify Bluetooth API hid Subclass condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_Subclass_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(hid.Subclass.SUBCLASS_COMBO);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SUBCLASS_PERF
     * @tc.name hid_Subclass_perf
     * @tc.desc Verify Bluetooth apitest case hid Subclass perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_Subclass_perf', 0, () => {
      runPerf('hid_Subclass_perf', 5000, () => {
        try {
          Number(hid.Subclass.SUBCLASS_UNCATEGORIZED);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SUBCLASS_STRESS
     * @tc.name hid_Subclass_stress
     * @tc.desc Verify Bluetooth apitest case hid Subclass stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_Subclass_stress', 0, () => {
      runStress('hid_Subclass_stress', 20, () => {
        try {
          Number(hid.Subclass.SUBCLASS_UNCATEGORIZED);
        } catch (_e) {
        }
      });
    });
    });
    describe('hid_ReportType', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_REPORTTYPE_N
     * @tc.name hid_ReportType_n
     * @tc.desc Verify Bluetooth API hid ReportType normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ReportType_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(hid.ReportType.REPORT_TYPE_INPUT);
      expect(typeof _v).assertEqual('number');
      expect(Number(hid.ReportType.REPORT_TYPE_INPUT)).assertEqual(1);
      expect(Number(hid.ReportType.REPORT_TYPE_OUTPUT)).assertEqual(2);
      expect(Number(hid.ReportType.REPORT_TYPE_FEATURE)).assertEqual(3);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_REPORTTYPE_E
     * @tc.name hid_ReportType_e
     * @tc.desc Verify Bluetooth API hid ReportType error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ReportType_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(hid.ReportType.REPORT_TYPE_INPUT);
      const _b = Number(hid.ReportType.REPORT_TYPE_OUTPUT);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_REPORTTYPE_B
     * @tc.name hid_ReportType_b
     * @tc.desc Verify Bluetooth API hid ReportType boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ReportType_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(hid.ReportType.REPORT_TYPE_FEATURE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_REPORTTYPE_R
     * @tc.name hid_ReportType_r
     * @tc.desc Verify Bluetooth API hid ReportType return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ReportType_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(hid.ReportType.REPORT_TYPE_OUTPUT);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_REPORTTYPE_X
     * @tc.name hid_ReportType_x
     * @tc.desc Verify Bluetooth API hid ReportType repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ReportType_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(hid.ReportType.REPORT_TYPE_INPUT);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(hid.ReportType.REPORT_TYPE_INPUT);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_REPORTTYPE_C
     * @tc.name hid_ReportType_c
     * @tc.desc Verify Bluetooth API hid ReportType condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ReportType_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(hid.ReportType.REPORT_TYPE_FEATURE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_REPORTTYPE_PERF
     * @tc.name hid_ReportType_perf
     * @tc.desc Verify Bluetooth apitest case hid ReportType perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ReportType_perf', 0, () => {
      runPerf('hid_ReportType_perf', 5000, () => {
        try {
          Number(hid.ReportType.REPORT_TYPE_INPUT);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_REPORTTYPE_STRESS
     * @tc.name hid_ReportType_stress
     * @tc.desc Verify Bluetooth apitest case hid ReportType stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ReportType_stress', 0, () => {
      runStress('hid_ReportType_stress', 20, () => {
        try {
          Number(hid.ReportType.REPORT_TYPE_INPUT);
        } catch (_e) {
        }
      });
    });
    });
    describe('hid_ServiceType', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SERVICETYPE_N
     * @tc.name hid_ServiceType_n
     * @tc.desc Verify Bluetooth API hid ServiceType normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ServiceType_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(hid.ServiceType.SERVICE_NO_TRAFFIC);
      expect(typeof _v).assertEqual('number');
      expect(Number(hid.ServiceType.SERVICE_NO_TRAFFIC)).assertEqual(0);
      expect(Number(hid.ServiceType.SERVICE_BEST_EFFORT)).assertEqual(1);
      expect(Number(hid.ServiceType.SERVICE_GUARANTEED)).assertEqual(2);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SERVICETYPE_E
     * @tc.name hid_ServiceType_e
     * @tc.desc Verify Bluetooth API hid ServiceType error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ServiceType_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(hid.ServiceType.SERVICE_NO_TRAFFIC);
      const _b = Number(hid.ServiceType.SERVICE_BEST_EFFORT);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SERVICETYPE_B
     * @tc.name hid_ServiceType_b
     * @tc.desc Verify Bluetooth API hid ServiceType boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ServiceType_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(hid.ServiceType.SERVICE_GUARANTEED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SERVICETYPE_R
     * @tc.name hid_ServiceType_r
     * @tc.desc Verify Bluetooth API hid ServiceType return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ServiceType_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(hid.ServiceType.SERVICE_BEST_EFFORT);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SERVICETYPE_X
     * @tc.name hid_ServiceType_x
     * @tc.desc Verify Bluetooth API hid ServiceType repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ServiceType_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(hid.ServiceType.SERVICE_NO_TRAFFIC);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(hid.ServiceType.SERVICE_NO_TRAFFIC);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SERVICETYPE_C
     * @tc.name hid_ServiceType_c
     * @tc.desc Verify Bluetooth API hid ServiceType condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ServiceType_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(hid.ServiceType.SERVICE_GUARANTEED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SERVICETYPE_PERF
     * @tc.name hid_ServiceType_perf
     * @tc.desc Verify Bluetooth apitest case hid ServiceType perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ServiceType_perf', 0, () => {
      runPerf('hid_ServiceType_perf', 5000, () => {
        try {
          Number(hid.ServiceType.SERVICE_NO_TRAFFIC);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_SERVICETYPE_STRESS
     * @tc.name hid_ServiceType_stress
     * @tc.desc Verify Bluetooth apitest case hid ServiceType stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ServiceType_stress', 0, () => {
      runStress('hid_ServiceType_stress', 20, () => {
        try {
          Number(hid.ServiceType.SERVICE_NO_TRAFFIC);
        } catch (_e) {
        }
      });
    });
    });
    describe('hid_ErrorReason', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_ERRORREASON_N
     * @tc.name hid_ErrorReason_n
     * @tc.desc Verify Bluetooth API hid ErrorReason normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ErrorReason_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(hid.ErrorReason.RSP_SUCCESS);
      expect(typeof _v).assertEqual('number');
      expect(Number(hid.ErrorReason.RSP_SUCCESS)).assertEqual(0);
      expect(Number(hid.ErrorReason.RSP_NOT_READY)).assertEqual(1);
      expect(Number(hid.ErrorReason.RSP_INVALID_REPORT_ID)).assertEqual(2);
      expect(Number(hid.ErrorReason.RSP_UNSUPPORTED_REQ)).assertEqual(3);
      expect(Number(hid.ErrorReason.RSP_INVALID_PARAM)).assertEqual(4);
      expect(Number(hid.ErrorReason.RSP_UNKNOWN)).assertEqual(14);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_ERRORREASON_E
     * @tc.name hid_ErrorReason_e
     * @tc.desc Verify Bluetooth API hid ErrorReason error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ErrorReason_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(hid.ErrorReason.RSP_SUCCESS);
      const _b = Number(hid.ErrorReason.RSP_NOT_READY);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_ERRORREASON_B
     * @tc.name hid_ErrorReason_b
     * @tc.desc Verify Bluetooth API hid ErrorReason boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ErrorReason_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(hid.ErrorReason.RSP_UNKNOWN);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_ERRORREASON_R
     * @tc.name hid_ErrorReason_r
     * @tc.desc Verify Bluetooth API hid ErrorReason return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ErrorReason_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(hid.ErrorReason.RSP_UNSUPPORTED_REQ);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_ERRORREASON_X
     * @tc.name hid_ErrorReason_x
     * @tc.desc Verify Bluetooth API hid ErrorReason repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ErrorReason_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(hid.ErrorReason.RSP_SUCCESS);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(hid.ErrorReason.RSP_SUCCESS);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_ERRORREASON_C
     * @tc.name hid_ErrorReason_c
     * @tc.desc Verify Bluetooth API hid ErrorReason condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ErrorReason_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(hid.ErrorReason.RSP_UNKNOWN);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_ERRORREASON_PERF
     * @tc.name hid_ErrorReason_perf
     * @tc.desc Verify Bluetooth apitest case hid ErrorReason perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ErrorReason_perf', 0, () => {
      runPerf('hid_ErrorReason_perf', 5000, () => {
        try {
          Number(hid.ErrorReason.RSP_SUCCESS);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_ERRORREASON_STRESS
     * @tc.name hid_ErrorReason_stress
     * @tc.desc Verify Bluetooth apitest case hid ErrorReason stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ErrorReason_stress', 0, () => {
      runStress('hid_ErrorReason_stress', 20, () => {
        try {
          Number(hid.ErrorReason.RSP_SUCCESS);
        } catch (_e) {
        }
      });
    });
    });
    describe('hid_ProtocolType', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_PROTOCOLTYPE_N
     * @tc.name hid_ProtocolType_n
     * @tc.desc Verify Bluetooth API hid ProtocolType normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ProtocolType_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(hid.ProtocolType.PROTOCOL_BOOT_MODE);
      expect(typeof _v).assertEqual('number');
      expect(Number(hid.ProtocolType.PROTOCOL_BOOT_MODE)).assertEqual(0);
      expect(Number(hid.ProtocolType.PROTOCOL_REPORT_MODE)).assertEqual(1);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_PROTOCOLTYPE_E
     * @tc.name hid_ProtocolType_e
     * @tc.desc Verify Bluetooth API hid ProtocolType error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ProtocolType_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(hid.ProtocolType.PROTOCOL_BOOT_MODE);
      const _b = Number(hid.ProtocolType.PROTOCOL_REPORT_MODE);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_PROTOCOLTYPE_B
     * @tc.name hid_ProtocolType_b
     * @tc.desc Verify Bluetooth API hid ProtocolType boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ProtocolType_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(hid.ProtocolType.PROTOCOL_REPORT_MODE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_PROTOCOLTYPE_R
     * @tc.name hid_ProtocolType_r
     * @tc.desc Verify Bluetooth API hid ProtocolType return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ProtocolType_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(hid.ProtocolType.PROTOCOL_REPORT_MODE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_PROTOCOLTYPE_X
     * @tc.name hid_ProtocolType_x
     * @tc.desc Verify Bluetooth API hid ProtocolType repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ProtocolType_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(hid.ProtocolType.PROTOCOL_BOOT_MODE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(hid.ProtocolType.PROTOCOL_BOOT_MODE);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_PROTOCOLTYPE_C
     * @tc.name hid_ProtocolType_c
     * @tc.desc Verify Bluetooth API hid ProtocolType condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ProtocolType_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(hid.ProtocolType.PROTOCOL_REPORT_MODE);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_PROTOCOLTYPE_PERF
     * @tc.name hid_ProtocolType_perf
     * @tc.desc Verify Bluetooth apitest case hid ProtocolType perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ProtocolType_perf', 0, () => {
      runPerf('hid_ProtocolType_perf', 5000, () => {
        try {
          Number(hid.ProtocolType.PROTOCOL_BOOT_MODE);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HID_PROTOCOLTYPE_STRESS
     * @tc.name hid_ProtocolType_stress
     * @tc.desc Verify Bluetooth apitest case hid ProtocolType stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('hid_ProtocolType_stress', 0, () => {
      runStress('hid_ProtocolType_stress', 20, () => {
        try {
          Number(hid.ProtocolType.PROTOCOL_BOOT_MODE);
        } catch (_e) {
        }
      });
    });
    });
    describe('ent_Protocol', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_PROTOCOL_N
     * @tc.name ent_Protocol_n
     * @tc.desc Verify Bluetooth API ent Protocol normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_Protocol_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(enterpriseBt.Protocol.GATT);
      expect(typeof _v).assertEqual('number');
      expect(Number(enterpriseBt.Protocol.GATT)).assertEqual(0);
      expect(Number(enterpriseBt.Protocol.SPP)).assertEqual(1);
      expect(Number(enterpriseBt.Protocol.OPP)).assertEqual(2);
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_PROTOCOL_E
     * @tc.name ent_Protocol_e
     * @tc.desc Verify Bluetooth API ent Protocol error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_Protocol_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(enterpriseBt.Protocol.GATT);
      const _b = Number(enterpriseBt.Protocol.SPP);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_PROTOCOL_B
     * @tc.name ent_Protocol_b
     * @tc.desc Verify Bluetooth API ent Protocol boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_Protocol_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(enterpriseBt.Protocol.OPP);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_PROTOCOL_R
     * @tc.name ent_Protocol_r
     * @tc.desc Verify Bluetooth API ent Protocol return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_Protocol_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(enterpriseBt.Protocol.SPP);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_PROTOCOL_X
     * @tc.name ent_Protocol_x
     * @tc.desc Verify Bluetooth API ent Protocol repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_Protocol_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(enterpriseBt.Protocol.GATT);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(enterpriseBt.Protocol.GATT);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_PROTOCOL_C
     * @tc.name ent_Protocol_c
     * @tc.desc Verify Bluetooth API ent Protocol condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_Protocol_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(enterpriseBt.Protocol.OPP);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_PROTOCOL_PERF
     * @tc.name ent_Protocol_perf
     * @tc.desc Verify Bluetooth apitest case ent Protocol perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_Protocol_perf', 0, () => {
      runPerf('ent_Protocol_perf', 5000, () => {
        try {
          Number(enterpriseBt.Protocol.GATT);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_PROTOCOL_STRESS
     * @tc.name ent_Protocol_stress
     * @tc.desc Verify Bluetooth apitest case ent Protocol stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_Protocol_stress', 0, () => {
      runStress('ent_Protocol_stress', 20, () => {
        try {
          Number(enterpriseBt.Protocol.GATT);
        } catch (_e) {
        }
      });
    });
    });
    describe('pa_DestroyReason', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DESTROYREASON_N
     * @tc.name pa_DestroyReason_n
     * @tc.desc Verify Bluetooth API pa DestroyReason normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_DestroyReason_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      const _v = Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.UNKNOWN_REASON);
      expect(typeof _v).assertEqual('number');
      expect(Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.UNKNOWN_REASON)).assertEqual(0);
      expect(Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.USER_CLOSED_ABILITY)).assertEqual(1
        );
      expect(Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.DEVICE_UNPAIRED)).assertEqual(2);
      expect(Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.DEVICE_LOST)).assertEqual(3);
      expect(Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.BLUETOOTH_DISABLED)).assertEqual(4)
        ;
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DESTROYREASON_E
     * @tc.name pa_DestroyReason_e
     * @tc.desc Verify Bluetooth API pa DestroyReason error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_DestroyReason_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.UNKNOWN_REASON);
      const _b = Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.USER_CLOSED_ABILITY);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DESTROYREASON_B
     * @tc.name pa_DestroyReason_b
     * @tc.desc Verify Bluetooth API pa DestroyReason boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_DestroyReason_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.BLUETOOTH_DISABLED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DESTROYREASON_R
     * @tc.name pa_DestroyReason_r
     * @tc.desc Verify Bluetooth API pa DestroyReason return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_DestroyReason_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.DEVICE_UNPAIRED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DESTROYREASON_X
     * @tc.name pa_DestroyReason_x
     * @tc.desc Verify Bluetooth API pa DestroyReason repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_DestroyReason_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.UNKNOWN_REASON);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.UNKNOWN_REASON);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DESTROYREASON_C
     * @tc.name pa_DestroyReason_c
     * @tc.desc Verify Bluetooth API pa DestroyReason condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_DestroyReason_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.BLUETOOTH_DISABLED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DESTROYREASON_PERF
     * @tc.name pa_DestroyReason_perf
     * @tc.desc Verify Bluetooth apitest case pa DestroyReason perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_DestroyReason_perf', 0, () => {
      runPerf('pa_DestroyReason_perf', 5000, () => {
        try {
          Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.UNKNOWN_REASON);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DESTROYREASON_STRESS
     * @tc.name pa_DestroyReason_stress
     * @tc.desc Verify Bluetooth apitest case pa DestroyReason stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_DestroyReason_stress', 0, () => {
      runStress('pa_DestroyReason_stress', 20, () => {
        try {
          Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.UNKNOWN_REASON);
        } catch (_e) {
        }
      });
    });
    });
  });
}


