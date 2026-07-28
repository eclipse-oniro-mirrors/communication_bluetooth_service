     * @tc.name sk_getL2capPsm_x
     * @tc.desc Verify Bluetooth API sk getL2capPsm repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getL2capPsm_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = socket.getL2capPsm(0) as Object;
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
        value = socket.getL2capPsm(0) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETL2CAPPSM_C
     * @tc.name sk_getL2capPsm_c
     * @tc.desc Verify Bluetooth API sk getL2capPsm condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getL2capPsm_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = socket.getL2capPsm(0) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETL2CAPPSM_PERF
     * @tc.name sk_getL2capPsm_perf
     * @tc.desc Verify Bluetooth apitest case sk getL2capPsm perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getL2capPsm_perf', 0, () => {
      runPerf('sk_getL2capPsm_perf', 5000, () => {
        try {
          socket.getL2capPsm(0) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETL2CAPPSM_STRESS
     * @tc.name sk_getL2capPsm_stress
     * @tc.desc Verify Bluetooth apitest case sk getL2capPsm stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getL2capPsm_stress', 0, () => {
      runStress('sk_getL2capPsm_stress', 20, () => {
        try {
          socket.getL2capPsm(0) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('sk_getMaxReceiveDataSize', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXRECEIVEDATASIZE_N
     * @tc.name sk_getMaxReceiveDataSize_n
     * @tc.desc Verify Bluetooth API sk getMaxReceiveDataSize normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxReceiveDataSize_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = socket.getMaxReceiveDataSize(0) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXRECEIVEDATASIZE_E
     * @tc.name sk_getMaxReceiveDataSize_e
     * @tc.desc Verify Bluetooth API sk getMaxReceiveDataSize error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxReceiveDataSize_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        socket.getMaxReceiveDataSize(-1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXRECEIVEDATASIZE_B
     * @tc.name sk_getMaxReceiveDataSize_b
     * @tc.desc Verify Bluetooth API sk getMaxReceiveDataSize boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxReceiveDataSize_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        socket.getMaxReceiveDataSize(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXRECEIVEDATASIZE_R
     * @tc.name sk_getMaxReceiveDataSize_r
     * @tc.desc Verify Bluetooth API sk getMaxReceiveDataSize return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxReceiveDataSize_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = socket.getMaxReceiveDataSize(0) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXRECEIVEDATASIZE_X
     * @tc.name sk_getMaxReceiveDataSize_x
     * @tc.desc Verify Bluetooth API sk getMaxReceiveDataSize repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxReceiveDataSize_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = socket.getMaxReceiveDataSize(0) as Object;
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
        value = socket.getMaxReceiveDataSize(0) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXRECEIVEDATASIZE_C
     * @tc.name sk_getMaxReceiveDataSize_c
     * @tc.desc Verify Bluetooth API sk getMaxReceiveDataSize condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxReceiveDataSize_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = socket.getMaxReceiveDataSize(0) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXRECEIVEDATASIZE_PERF
     * @tc.name sk_getMaxReceiveDataSize_perf
     * @tc.desc Verify Bluetooth apitest case sk getMaxReceiveDataSize perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxReceiveDataSize_perf', 0, () => {
      runPerf('sk_getMaxReceiveDataSize_perf', 5000, () => {
        try {
          socket.getMaxReceiveDataSize(0) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXRECEIVEDATASIZE_STRESS
     * @tc.name sk_getMaxReceiveDataSize_stress
     * @tc.desc Verify Bluetooth apitest case sk getMaxReceiveDataSize stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxReceiveDataSize_stress', 0, () => {
      runStress('sk_getMaxReceiveDataSize_stress', 20, () => {
        try {
          socket.getMaxReceiveDataSize(0) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('sk_getMaxTransmitDataSize', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXTRANSMITDATASIZE_N
     * @tc.name sk_getMaxTransmitDataSize_n
     * @tc.desc Verify Bluetooth API sk getMaxTransmitDataSize normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxTransmitDataSize_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = socket.getMaxTransmitDataSize(0) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXTRANSMITDATASIZE_E
     * @tc.name sk_getMaxTransmitDataSize_e
     * @tc.desc Verify Bluetooth API sk getMaxTransmitDataSize error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxTransmitDataSize_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        socket.getMaxTransmitDataSize(-1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXTRANSMITDATASIZE_B
     * @tc.name sk_getMaxTransmitDataSize_b
     * @tc.desc Verify Bluetooth API sk getMaxTransmitDataSize boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxTransmitDataSize_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        socket.getMaxTransmitDataSize(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXTRANSMITDATASIZE_R
     * @tc.name sk_getMaxTransmitDataSize_r
     * @tc.desc Verify Bluetooth API sk getMaxTransmitDataSize return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxTransmitDataSize_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = socket.getMaxTransmitDataSize(0) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXTRANSMITDATASIZE_X
     * @tc.name sk_getMaxTransmitDataSize_x
     * @tc.desc Verify Bluetooth API sk getMaxTransmitDataSize repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxTransmitDataSize_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = socket.getMaxTransmitDataSize(0) as Object;
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
        value = socket.getMaxTransmitDataSize(0) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXTRANSMITDATASIZE_C
     * @tc.name sk_getMaxTransmitDataSize_c
     * @tc.desc Verify Bluetooth API sk getMaxTransmitDataSize condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxTransmitDataSize_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = socket.getMaxTransmitDataSize(0) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXTRANSMITDATASIZE_PERF
     * @tc.name sk_getMaxTransmitDataSize_perf
     * @tc.desc Verify Bluetooth apitest case sk getMaxTransmitDataSize perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxTransmitDataSize_perf', 0, () => {
      runPerf('sk_getMaxTransmitDataSize_perf', 5000, () => {
        try {
          socket.getMaxTransmitDataSize(0) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_GETMAXTRANSMITDATASIZE_STRESS
     * @tc.name sk_getMaxTransmitDataSize_stress
     * @tc.desc Verify Bluetooth apitest case sk getMaxTransmitDataSize stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_getMaxTransmitDataSize_stress', 0, () => {
      runStress('sk_getMaxTransmitDataSize_stress', 20, () => {
        try {
          socket.getMaxTransmitDataSize(0) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('sk_isConnected', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_ISCONNECTED_N
     * @tc.name sk_isConnected_n
     * @tc.desc Verify Bluetooth API sk isConnected normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_isConnected_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = socket.isConnected(0) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_ISCONNECTED_E
     * @tc.name sk_isConnected_e
     * @tc.desc Verify Bluetooth API sk isConnected error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_isConnected_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        socket.isConnected(-1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_ISCONNECTED_B
     * @tc.name sk_isConnected_b
     * @tc.desc Verify Bluetooth API sk isConnected boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_isConnected_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        socket.isConnected(0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_ISCONNECTED_R
     * @tc.name sk_isConnected_r
     * @tc.desc Verify Bluetooth API sk isConnected return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_isConnected_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = socket.isConnected(0) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_ISCONNECTED_X
     * @tc.name sk_isConnected_x
     * @tc.desc Verify Bluetooth API sk isConnected repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_isConnected_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = socket.isConnected(0) as Object;
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
        value = socket.isConnected(0) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_ISCONNECTED_C
     * @tc.name sk_isConnected_c
     * @tc.desc Verify Bluetooth API sk isConnected condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_isConnected_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = socket.isConnected(0) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_ISCONNECTED_PERF
     * @tc.name sk_isConnected_perf
     * @tc.desc Verify Bluetooth apitest case sk isConnected perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_isConnected_perf', 0, () => {
      runPerf('sk_isConnected_perf', 5000, () => {
        try {
          socket.isConnected(0) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_ISCONNECTED_STRESS
     * @tc.name sk_isConnected_stress
     * @tc.desc Verify Bluetooth apitest case sk isConnected stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_isConnected_stress', 0, () => {
      runStress('sk_isConnected_stress', 20, () => {
        try {
          socket.isConnected(0) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('sk_on_sppRead', () => {
    it('sk_on_sppRead_n', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        socket.on('sppRead', 0, () => {});
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
    it('sk_on_sppRead_e', 0, async (done: Function) => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        socket.on('sppRead', -1, () => {});
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
    it('sk_on_sppRead_b', 0, async (done: Function) => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        socket.on('sppRead', -1, () => {});
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
    it('sk_on_sppRead_r', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        socket.on('sppRead', 0, () => {});
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
    it('sk_on_sppRead_x', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        socket.on('sppRead', 0, () => {});
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
        socket.on('sppRead', 0, () => {});
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
    it('sk_on_sppRead_c', 0, async (done: Function) => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        socket.on('sppRead', 0, () => {});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_ON_SPPREAD_PERF
     * @tc.name sk_on_sppRead_perf
     * @tc.desc Verify Bluetooth apitest case sk on sppRead perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_on_sppRead_perf', 0, () => {
      runPerf('sk_on_sppRead_perf', 5000, () => {
        try {
          socket.on('sppRead', 0, () => {});
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_ON_SPPREAD_STRESS
     * @tc.name sk_on_sppRead_stress
     * @tc.desc Verify Bluetooth apitest case sk on sppRead stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_on_sppRead_stress', 0, () => {
      runStress('sk_on_sppRead_stress', 20, () => {
        try {
          socket.on('sppRead', 0, () => {});
        } catch (_e) {
        }
      });
    });
    });
    describe('sk_off_sppRead', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_OFF_SPPREAD_N
     * @tc.name sk_off_sppRead_n
     * @tc.desc Verify Bluetooth API sk off sppRead normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_off_sppRead_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        socket.off('sppRead', 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_OFF_SPPREAD_E
     * @tc.name sk_off_sppRead_e
     * @tc.desc Verify Bluetooth API sk off sppRead error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_off_sppRead_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        socket.off('sppRead', -1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_OFF_SPPREAD_B
     * @tc.name sk_off_sppRead_b
     * @tc.desc Verify Bluetooth API sk off sppRead boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_off_sppRead_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        socket.off('sppRead', -1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_OFF_SPPREAD_R
     * @tc.name sk_off_sppRead_r
     * @tc.desc Verify Bluetooth API sk off sppRead return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_off_sppRead_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        socket.off('sppRead', 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_OFF_SPPREAD_X
     * @tc.name sk_off_sppRead_x
     * @tc.desc Verify Bluetooth API sk off sppRead repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_off_sppRead_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        socket.off('sppRead', 0);
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
        socket.off('sppRead', 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_OFF_SPPREAD_C
     * @tc.name sk_off_sppRead_c
     * @tc.desc Verify Bluetooth API sk off sppRead condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_off_sppRead_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        socket.off('sppRead', 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_OFF_SPPREAD_PERF
     * @tc.name sk_off_sppRead_perf
     * @tc.desc Verify Bluetooth apitest case sk off sppRead perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_off_sppRead_perf', 0, () => {
      runPerf('sk_off_sppRead_perf', 5000, () => {
        try {
          socket.off('sppRead', 0);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_OFF_SPPREAD_STRESS
     * @tc.name sk_off_sppRead_stress
     * @tc.desc Verify Bluetooth apitest case sk off sppRead stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_off_sppRead_stress', 0, () => {
      runStress('sk_off_sppRead_stress', 20, () => {
        try {
          socket.off('sppRead', 0);
        } catch (_e) {
        }
      });
    });
    });
  });
}


