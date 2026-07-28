      let value = false;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = partnerAgent.isDeviceBound(partnerDeviceAddr());
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
        expect(typeof value).assertEqual('boolean');
      }
      errCode = 0;
      value = false;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        value = partnerAgent.isDeviceBound(partnerDeviceAddr());
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
        expect(typeof value).assertEqual('boolean');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICEBOUND_C
     * @tc.name pa_isDeviceBound_c
     * @tc.desc Verify Bluetooth API pa isDeviceBound condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceBound_c', 0, () => {
      let errCode = 0;
      let value = false;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = partnerAgent.isDeviceBound(partnerDeviceAddr());
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
        expect(typeof value).assertEqual('boolean');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICEBOUND_PERF
     * @tc.name pa_isDeviceBound_perf
     * @tc.desc Verify Bluetooth apitest case pa isDeviceBound perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceBound_perf', 0, () => {
      runPerf('pa_isDeviceBound_perf', 5000, () => {
        try {
          partnerAgent.isDeviceBound(partnerDeviceAddr());
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICEBOUND_STRESS
     * @tc.name pa_isDeviceBound_stress
     * @tc.desc Verify Bluetooth apitest case pa isDeviceBound stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceBound_stress', 0, () => {
      runStress('pa_isDeviceBound_stress', 20, () => {
        try {
          partnerAgent.isDeviceBound(partnerDeviceAddr());
        } catch (_e) {
        }
      });
    });
    });
    describe('pa_getBoundDevices', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_GETBOUNDDEVICES_N
     * @tc.name pa_getBoundDevices_n
     * @tc.desc Verify Bluetooth API pa getBoundDevices normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_getBoundDevices_n', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = partnerAgent.getBoundDevices() as Array < Object > ;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_GETBOUNDDEVICES_E
     * @tc.name pa_getBoundDevices_e
     * @tc.desc Verify Bluetooth API pa getBoundDevices error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_getBoundDevices_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        partnerAgent.getBoundDevices();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_GETBOUNDDEVICES_B
     * @tc.name pa_getBoundDevices_b
     * @tc.desc Verify Bluetooth API pa getBoundDevices boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_getBoundDevices_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        partnerAgent.getBoundDevices();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_GETBOUNDDEVICES_R
     * @tc.name pa_getBoundDevices_r
     * @tc.desc Verify Bluetooth API pa getBoundDevices return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_getBoundDevices_r', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = partnerAgent.getBoundDevices() as Array < Object > ;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_GETBOUNDDEVICES_X
     * @tc.name pa_getBoundDevices_x
     * @tc.desc Verify Bluetooth API pa getBoundDevices repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_getBoundDevices_x', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = partnerAgent.getBoundDevices() as Array < Object > ;
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
        value = partnerAgent.getBoundDevices() as Array < Object > ;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_GETBOUNDDEVICES_C
     * @tc.name pa_getBoundDevices_c
     * @tc.desc Verify Bluetooth API pa getBoundDevices condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_getBoundDevices_c', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = partnerAgent.getBoundDevices() as Array < Object > ;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_GETBOUNDDEVICES_PERF
     * @tc.name pa_getBoundDevices_perf
     * @tc.desc Verify Bluetooth apitest case pa getBoundDevices perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_getBoundDevices_perf', 0, () => {
      runPerf('pa_getBoundDevices_perf', 5000, () => {
        try {
          partnerAgent.getBoundDevices() as Array < Object > ;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_GETBOUNDDEVICES_STRESS
     * @tc.name pa_getBoundDevices_stress
     * @tc.desc Verify Bluetooth apitest case pa getBoundDevices stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_getBoundDevices_stress', 0, () => {
      runStress('pa_getBoundDevices_stress', 20, () => {
        try {
          partnerAgent.getBoundDevices() as Array < Object > ;
        } catch (_e) {
        }
      });
    });
    });
    describe('pa_enableDeviceControl', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ENABLEDEVICECONTROL_N
     * @tc.name pa_enableDeviceControl_n
     * @tc.desc Verify Bluetooth API pa enableDeviceControl normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_enableDeviceControl_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = partnerAgent.enableDeviceControl(partnerDeviceAddr()) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ENABLEDEVICECONTROL_E
     * @tc.name pa_enableDeviceControl_e
     * @tc.desc Verify Bluetooth API pa enableDeviceControl error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_enableDeviceControl_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        partnerAgent.enableDeviceControl({bluetoothAddress:{address:EMPTY_MAC, addressType:0}});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ENABLEDEVICECONTROL_B
     * @tc.name pa_enableDeviceControl_b
     * @tc.desc Verify Bluetooth API pa enableDeviceControl boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_enableDeviceControl_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        partnerAgent.enableDeviceControl({bluetoothAddress:{address:EMPTY_MAC, addressType:0}});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ENABLEDEVICECONTROL_R
     * @tc.name pa_enableDeviceControl_r
     * @tc.desc Verify Bluetooth API pa enableDeviceControl return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_enableDeviceControl_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = partnerAgent.enableDeviceControl(partnerDeviceAddr()) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ENABLEDEVICECONTROL_X
     * @tc.name pa_enableDeviceControl_x
     * @tc.desc Verify Bluetooth API pa enableDeviceControl repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_enableDeviceControl_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = partnerAgent.enableDeviceControl(partnerDeviceAddr()) as Object;
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
        value = partnerAgent.enableDeviceControl(partnerDeviceAddr()) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ENABLEDEVICECONTROL_C
     * @tc.name pa_enableDeviceControl_c
     * @tc.desc Verify Bluetooth API pa enableDeviceControl condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_enableDeviceControl_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = partnerAgent.enableDeviceControl(partnerDeviceAddr()) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ENABLEDEVICECONTROL_PERF
     * @tc.name pa_enableDeviceControl_perf
     * @tc.desc Verify Bluetooth apitest case pa enableDeviceControl perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_enableDeviceControl_perf', 0, () => {
      runPerf('pa_enableDeviceControl_perf', 5000, () => {
        try {
          partnerAgent.enableDeviceControl(partnerDeviceAddr()) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ENABLEDEVICECONTROL_STRESS
     * @tc.name pa_enableDeviceControl_stress
     * @tc.desc Verify Bluetooth apitest case pa enableDeviceControl stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_enableDeviceControl_stress', 0, () => {
      runStress('pa_enableDeviceControl_stress', 20, () => {
        try {
          partnerAgent.enableDeviceControl(partnerDeviceAddr()) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('pa_disableDeviceControl', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DISABLEDEVICECONTROL_N
     * @tc.name pa_disableDeviceControl_n
     * @tc.desc Verify Bluetooth API pa disableDeviceControl normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_disableDeviceControl_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = partnerAgent.disableDeviceControl(partnerDeviceAddr()) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DISABLEDEVICECONTROL_E
     * @tc.name pa_disableDeviceControl_e
     * @tc.desc Verify Bluetooth API pa disableDeviceControl error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_disableDeviceControl_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        partnerAgent.disableDeviceControl({bluetoothAddress:{address:EMPTY_MAC, addressType:0}});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DISABLEDEVICECONTROL_B
     * @tc.name pa_disableDeviceControl_b
     * @tc.desc Verify Bluetooth API pa disableDeviceControl boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_disableDeviceControl_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        partnerAgent.disableDeviceControl({bluetoothAddress:{address:EMPTY_MAC, addressType:0}});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DISABLEDEVICECONTROL_R
     * @tc.name pa_disableDeviceControl_r
     * @tc.desc Verify Bluetooth API pa disableDeviceControl return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_disableDeviceControl_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = partnerAgent.disableDeviceControl(partnerDeviceAddr()) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DISABLEDEVICECONTROL_X
     * @tc.name pa_disableDeviceControl_x
     * @tc.desc Verify Bluetooth API pa disableDeviceControl repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_disableDeviceControl_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = partnerAgent.disableDeviceControl(partnerDeviceAddr()) as Object;
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
        value = partnerAgent.disableDeviceControl(partnerDeviceAddr()) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DISABLEDEVICECONTROL_C
     * @tc.name pa_disableDeviceControl_c
     * @tc.desc Verify Bluetooth API pa disableDeviceControl condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_disableDeviceControl_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = partnerAgent.disableDeviceControl(partnerDeviceAddr()) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DISABLEDEVICECONTROL_PERF
     * @tc.name pa_disableDeviceControl_perf
     * @tc.desc Verify Bluetooth apitest case pa disableDeviceControl perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_disableDeviceControl_perf', 0, () => {
      runPerf('pa_disableDeviceControl_perf', 5000, () => {
        try {
          partnerAgent.disableDeviceControl(partnerDeviceAddr()) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DISABLEDEVICECONTROL_STRESS
     * @tc.name pa_disableDeviceControl_stress
     * @tc.desc Verify Bluetooth apitest case pa disableDeviceControl stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_disableDeviceControl_stress', 0, () => {
      runStress('pa_disableDeviceControl_stress', 20, () => {
        try {
          partnerAgent.disableDeviceControl(partnerDeviceAddr()) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('pa_isDeviceControlEnabled', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICECONTROLENABLED_N
     * @tc.name pa_isDeviceControlEnabled_n
     * @tc.desc Verify Bluetooth API pa isDeviceControlEnabled normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceControlEnabled_n', 0, () => {
      let errCode = 0;
      let value = false;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = partnerAgent.isDeviceControlEnabled(partnerDeviceAddr());
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
        expect(typeof value).assertEqual('boolean');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICECONTROLENABLED_E
     * @tc.name pa_isDeviceControlEnabled_e
     * @tc.desc Verify Bluetooth API pa isDeviceControlEnabled error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceControlEnabled_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        partnerAgent.isDeviceControlEnabled({bluetoothAddress:{address:EMPTY_MAC, addressType:0}});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICECONTROLENABLED_B
     * @tc.name pa_isDeviceControlEnabled_b
     * @tc.desc Verify Bluetooth API pa isDeviceControlEnabled boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceControlEnabled_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        partnerAgent.isDeviceControlEnabled({bluetoothAddress:{address:EMPTY_MAC, addressType:0}});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICECONTROLENABLED_R
     * @tc.name pa_isDeviceControlEnabled_r
     * @tc.desc Verify Bluetooth API pa isDeviceControlEnabled return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceControlEnabled_r', 0, () => {
      let errCode = 0;
      let value = false;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = partnerAgent.isDeviceControlEnabled(partnerDeviceAddr());
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
        expect(typeof value).assertEqual('boolean');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICECONTROLENABLED_X
     * @tc.name pa_isDeviceControlEnabled_x
     * @tc.desc Verify Bluetooth API pa isDeviceControlEnabled repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceControlEnabled_x', 0, () => {
      let errCode = 0;
      let value = false;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = partnerAgent.isDeviceControlEnabled(partnerDeviceAddr());
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
        expect(typeof value).assertEqual('boolean');
      }
      errCode = 0;
      value = false;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      try {
        value = partnerAgent.isDeviceControlEnabled(partnerDeviceAddr());
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
        expect(typeof value).assertEqual('boolean');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICECONTROLENABLED_C
     * @tc.name pa_isDeviceControlEnabled_c
     * @tc.desc Verify Bluetooth API pa isDeviceControlEnabled condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceControlEnabled_c', 0, () => {
      let errCode = 0;
      let value = false;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = partnerAgent.isDeviceControlEnabled(partnerDeviceAddr());
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
        expect(typeof value).assertEqual('boolean');
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICECONTROLENABLED_PERF
     * @tc.name pa_isDeviceControlEnabled_perf
     * @tc.desc Verify Bluetooth apitest case pa isDeviceControlEnabled perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceControlEnabled_perf', 0, () => {
      runPerf('pa_isDeviceControlEnabled_perf', 5000, () => {
        try {
          partnerAgent.isDeviceControlEnabled(partnerDeviceAddr());
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICECONTROLENABLED_STRESS
     * @tc.name pa_isDeviceControlEnabled_stress
     * @tc.desc Verify Bluetooth apitest case pa isDeviceControlEnabled stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceControlEnabled_stress', 0, () => {
      runStress('pa_isDeviceControlEnabled_stress', 20, () => {
        try {
          partnerAgent.isDeviceControlEnabled(partnerDeviceAddr());
        } catch (_e) {
        }
      });
    });
    });
  });
}


