     * @tc.name gattc_writeCharacteristicValueWithContext_stress
     * @tc.desc Verify Bluetooth apitest case gattc writeCharacteristicValueWithContext stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_writeCharacteristicValueWithContext_stress', 0, () => {
      runStress('gattc_writeCharacteristicValueWithContext_stress', 20, () => {
        try {
          const gattClient = ble.createGattClientDevice(BADDR);
          gattClient.writeCharacteristicValueWithContext(bleCharRef(), ble.GattWriteType.WRITE);
        } catch (_e) {
        }
      });
    });
    });
    describe('gattc_writeDescriptorValue', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_WRITEDESCRIPTORVALUE_N
     * @tc.name gattc_writeDescriptorValue_n
     * @tc.desc Verify Bluetooth API gattc writeDescriptorValue normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_writeDescriptorValue_n', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattClient.writeDescriptorValue(bleDescRef());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_WRITEDESCRIPTORVALUE_E
     * @tc.name gattc_writeDescriptorValue_e
     * @tc.desc Verify Bluetooth API gattc writeDescriptorValue error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_writeDescriptorValue_e', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattClient.writeDescriptorValue(bleDescRef());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_WRITEDESCRIPTORVALUE_B
     * @tc.name gattc_writeDescriptorValue_b
     * @tc.desc Verify Bluetooth API gattc writeDescriptorValue boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_writeDescriptorValue_b', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattClient.writeDescriptorValue(bleDescRef());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_WRITEDESCRIPTORVALUE_R
     * @tc.name gattc_writeDescriptorValue_r
     * @tc.desc Verify Bluetooth API gattc writeDescriptorValue return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_writeDescriptorValue_r', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattClient.writeDescriptorValue(bleDescRef());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_WRITEDESCRIPTORVALUE_X
     * @tc.name gattc_writeDescriptorValue_x
     * @tc.desc Verify Bluetooth API gattc writeDescriptorValue repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_writeDescriptorValue_x', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattClient.writeDescriptorValue(bleDescRef());
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
        gattClient.writeDescriptorValue(bleDescRef());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_WRITEDESCRIPTORVALUE_C
     * @tc.name gattc_writeDescriptorValue_c
     * @tc.desc Verify Bluetooth API gattc writeDescriptorValue condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_writeDescriptorValue_c', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattClient.writeDescriptorValue(bleDescRef());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_WRITEDESCRIPTORVALUE_PERF
     * @tc.name gattc_writeDescriptorValue_perf
     * @tc.desc Verify Bluetooth apitest case gattc writeDescriptorValue perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_writeDescriptorValue_perf', 0, () => {
      runPerf('gattc_writeDescriptorValue_perf', 5000, () => {
        try {
          const gattClient = ble.createGattClientDevice(BADDR);
          gattClient.writeDescriptorValue(bleDescRef());
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_WRITEDESCRIPTORVALUE_STRESS
     * @tc.name gattc_writeDescriptorValue_stress
     * @tc.desc Verify Bluetooth apitest case gattc writeDescriptorValue stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_writeDescriptorValue_stress', 0, () => {
      runStress('gattc_writeDescriptorValue_stress', 20, () => {
        try {
          const gattClient = ble.createGattClientDevice(BADDR);
          gattClient.writeDescriptorValue(bleDescRef());
        } catch (_e) {
        }
      });
    });
    });
    describe('gattc_getRssiValue', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_GETRSSIVALUE_N
     * @tc.name gattc_getRssiValue_n
     * @tc.desc Verify Bluetooth API gattc getRssiValue normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_getRssiValue_n', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattClient.getRssiValue();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_GETRSSIVALUE_E
     * @tc.name gattc_getRssiValue_e
     * @tc.desc Verify Bluetooth API gattc getRssiValue error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_getRssiValue_e', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattClient.getRssiValue();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_GETRSSIVALUE_B
     * @tc.name gattc_getRssiValue_b
     * @tc.desc Verify Bluetooth API gattc getRssiValue boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_getRssiValue_b', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattClient.getRssiValue();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_GETRSSIVALUE_R
     * @tc.name gattc_getRssiValue_r
     * @tc.desc Verify Bluetooth API gattc getRssiValue return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_getRssiValue_r', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattClient.getRssiValue();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_GETRSSIVALUE_X
     * @tc.name gattc_getRssiValue_x
     * @tc.desc Verify Bluetooth API gattc getRssiValue repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_getRssiValue_x', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattClient.getRssiValue();
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
        gattClient.getRssiValue();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_GETRSSIVALUE_C
     * @tc.name gattc_getRssiValue_c
     * @tc.desc Verify Bluetooth API gattc getRssiValue condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_getRssiValue_c', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattClient.getRssiValue();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_GETRSSIVALUE_PERF
     * @tc.name gattc_getRssiValue_perf
     * @tc.desc Verify Bluetooth apitest case gattc getRssiValue perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_getRssiValue_perf', 0, () => {
      runPerf('gattc_getRssiValue_perf', 5000, () => {
        try {
          const gattClient = ble.createGattClientDevice(BADDR);
          gattClient.getRssiValue();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_GETRSSIVALUE_STRESS
     * @tc.name gattc_getRssiValue_stress
     * @tc.desc Verify Bluetooth apitest case gattc getRssiValue stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_getRssiValue_stress', 0, () => {
      runStress('gattc_getRssiValue_stress', 20, () => {
        try {
          const gattClient = ble.createGattClientDevice(BADDR);
          gattClient.getRssiValue();
        } catch (_e) {
        }
      });
    });
    });
    describe('gattc_setBLEMtuSize', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETBLEMTUSIZE_N
     * @tc.name gattc_setBLEMtuSize_n
     * @tc.desc Verify Bluetooth API gattc setBLEMtuSize normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setBLEMtuSize_n', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattClient.setBLEMtuSize(512);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETBLEMTUSIZE_E
     * @tc.name gattc_setBLEMtuSize_e
     * @tc.desc Verify Bluetooth API gattc setBLEMtuSize error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setBLEMtuSize_e', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattClient.setBLEMtuSize(512);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETBLEMTUSIZE_B
     * @tc.name gattc_setBLEMtuSize_b
     * @tc.desc Verify Bluetooth API gattc setBLEMtuSize boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setBLEMtuSize_b', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattClient.setBLEMtuSize(512);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETBLEMTUSIZE_R
     * @tc.name gattc_setBLEMtuSize_r
     * @tc.desc Verify Bluetooth API gattc setBLEMtuSize return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setBLEMtuSize_r', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattClient.setBLEMtuSize(512);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETBLEMTUSIZE_X
     * @tc.name gattc_setBLEMtuSize_x
     * @tc.desc Verify Bluetooth API gattc setBLEMtuSize repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setBLEMtuSize_x', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattClient.setBLEMtuSize(512);
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
        gattClient.setBLEMtuSize(512);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETBLEMTUSIZE_C
     * @tc.name gattc_setBLEMtuSize_c
     * @tc.desc Verify Bluetooth API gattc setBLEMtuSize condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setBLEMtuSize_c', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattClient.setBLEMtuSize(512);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETBLEMTUSIZE_PERF
     * @tc.name gattc_setBLEMtuSize_perf
     * @tc.desc Verify Bluetooth apitest case gattc setBLEMtuSize perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setBLEMtuSize_perf', 0, () => {
      runPerf('gattc_setBLEMtuSize_perf', 5000, () => {
        try {
          const gattClient = ble.createGattClientDevice(BADDR);
          gattClient.setBLEMtuSize(512);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETBLEMTUSIZE_STRESS
     * @tc.name gattc_setBLEMtuSize_stress
     * @tc.desc Verify Bluetooth apitest case gattc setBLEMtuSize stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setBLEMtuSize_stress', 0, () => {
      runStress('gattc_setBLEMtuSize_stress', 20, () => {
        try {
          const gattClient = ble.createGattClientDevice(BADDR);
          gattClient.setBLEMtuSize(512);
        } catch (_e) {
        }
      });
    });
    });
    describe('gattc_setCharacteristicChangeNotification', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGENOTIFICATION_N
     * @tc.name gattc_setCharacteristicChangeNotification_n
     * @tc.desc Verify Bluetooth API gattc setCharacteristicChangeNotification normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeNotification_n', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattClient.setCharacteristicChangeNotification(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGENOTIFICATION_E
     * @tc.name gattc_setCharacteristicChangeNotification_e
     * @tc.desc Verify Bluetooth API gattc setCharacteristicChangeNotification error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeNotification_e', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattClient.setCharacteristicChangeNotification(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGENOTIFICATION_B
     * @tc.name gattc_setCharacteristicChangeNotification_b
     * @tc.desc Verify Bluetooth API gattc setCharacteristicChangeNotification boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeNotification_b', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattClient.setCharacteristicChangeNotification(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGENOTIFICATION_R
     * @tc.name gattc_setCharacteristicChangeNotification_r
     * @tc.desc Verify Bluetooth API gattc setCharacteristicChangeNotification return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeNotification_r', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattClient.setCharacteristicChangeNotification(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGENOTIFICATION_X
     * @tc.name gattc_setCharacteristicChangeNotification_x
     * @tc.desc Verify Bluetooth API gattc setCharacteristicChangeNotification repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeNotification_x', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattClient.setCharacteristicChangeNotification(bleCharRef(), true);
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
        gattClient.setCharacteristicChangeNotification(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGENOTIFICATION_C
     * @tc.name gattc_setCharacteristicChangeNotification_c
     * @tc.desc Verify Bluetooth API gattc setCharacteristicChangeNotification condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeNotification_c', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattClient.setCharacteristicChangeNotification(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGENOTIFICATION_PERF
     * @tc.name gattc_setCharacteristicChangeNotification_perf
     * @tc.desc Verify Bluetooth apitest case gattc setCharacteristicChangeNotification perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeNotification_perf', 0, () => {
      runPerf('gattc_setCharacteristicChangeNotification_perf', 5000, () => {
        try {
          const gattClient = ble.createGattClientDevice(BADDR);
          gattClient.setCharacteristicChangeNotification(bleCharRef(), true);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGENOTIFICATION_STRESS
     * @tc.name gattc_setCharacteristicChangeNotification_stress
     * @tc.desc Verify Bluetooth apitest case gattc setCharacteristicChangeNotification stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeNotification_stress', 0, () => {
      runStress('gattc_setCharacteristicChangeNotification_stress', 20, () => {
        try {
          const gattClient = ble.createGattClientDevice(BADDR);
          gattClient.setCharacteristicChangeNotification(bleCharRef(), true);
        } catch (_e) {
        }
      });
    });
    });
    describe('gattc_setCharacteristicChangeIndication', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGEINDICATION_N
     * @tc.name gattc_setCharacteristicChangeIndication_n
     * @tc.desc Verify Bluetooth API gattc setCharacteristicChangeIndication normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeIndication_n', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        gattClient.setCharacteristicChangeIndication(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGEINDICATION_E
     * @tc.name gattc_setCharacteristicChangeIndication_e
     * @tc.desc Verify Bluetooth API gattc setCharacteristicChangeIndication error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeIndication_e', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        gattClient.setCharacteristicChangeIndication(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGEINDICATION_B
     * @tc.name gattc_setCharacteristicChangeIndication_b
     * @tc.desc Verify Bluetooth API gattc setCharacteristicChangeIndication boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeIndication_b', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        gattClient.setCharacteristicChangeIndication(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGEINDICATION_R
     * @tc.name gattc_setCharacteristicChangeIndication_r
     * @tc.desc Verify Bluetooth API gattc setCharacteristicChangeIndication return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeIndication_r', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        gattClient.setCharacteristicChangeIndication(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGEINDICATION_X
     * @tc.name gattc_setCharacteristicChangeIndication_x
     * @tc.desc Verify Bluetooth API gattc setCharacteristicChangeIndication repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeIndication_x', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        gattClient.setCharacteristicChangeIndication(bleCharRef(), true);
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
        gattClient.setCharacteristicChangeIndication(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGEINDICATION_C
     * @tc.name gattc_setCharacteristicChangeIndication_c
     * @tc.desc Verify Bluetooth API gattc setCharacteristicChangeIndication condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeIndication_c', 0, () => {
      const gattClient = ble.createGattClientDevice(BADDR);
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        gattClient.setCharacteristicChangeIndication(bleCharRef(), true);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGEINDICATION_PERF
     * @tc.name gattc_setCharacteristicChangeIndication_perf
     * @tc.desc Verify Bluetooth apitest case gattc setCharacteristicChangeIndication perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeIndication_perf', 0, () => {
      runPerf('gattc_setCharacteristicChangeIndication_perf', 5000, () => {
        try {
          const gattClient = ble.createGattClientDevice(BADDR);
          gattClient.setCharacteristicChangeIndication(bleCharRef(), true);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_GATTC_SETCHARACTERISTICCHANGEINDICATION_STRESS
     * @tc.name gattc_setCharacteristicChangeIndication_stress
     * @tc.desc Verify Bluetooth apitest case gattc setCharacteristicChangeIndication stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('gattc_setCharacteristicChangeIndication_stress', 0, () => {
      runStress('gattc_setCharacteristicChangeIndication_stress', 20, () => {
        try {
          const gattClient = ble.createGattClientDevice(BADDR);
          gattClient.setCharacteristicChangeIndication(bleCharRef(), true);
        } catch (_e) {
        }
      });
    });
    });
  });
}


