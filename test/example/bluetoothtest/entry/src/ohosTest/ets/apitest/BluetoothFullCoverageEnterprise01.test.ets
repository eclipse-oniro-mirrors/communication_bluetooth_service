        if (isNaN(errCode)) {
          errCode = 2900099;
        }
        hilog.error(0x0000, 'BtFullCoverage', 'c: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_ADDDISALLOWEDBLUETOOTHDEVICES_PERF
     * @tc.name ent_addDisallowedBluetoothDevices_perf
     * @tc.desc Verify Bluetooth apitest case ent addDisallowedBluetoothDevices perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_addDisallowedBluetoothDevices_perf', 0, () => {
      runPerf('ent_addDisallowedBluetoothDevices_perf', 5000, () => {
        try {
          enterpriseBt.addDisallowedBluetoothDevices({bundleName:'com.example.test',
            abilityName:'EntryAbility'}, [BADDR]);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_ADDDISALLOWEDBLUETOOTHDEVICES_STRESS
     * @tc.name ent_addDisallowedBluetoothDevices_stress
     * @tc.desc Verify Bluetooth apitest case ent addDisallowedBluetoothDevices stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_addDisallowedBluetoothDevices_stress', 0, () => {
      runStress('ent_addDisallowedBluetoothDevices_stress', 20, () => {
        try {
          enterpriseBt.addDisallowedBluetoothDevices({bundleName:'com.example.test',
            abilityName:'EntryAbility'}, [BADDR]);
        } catch (_e) {
        }
      });
    });
    });
    describe('ent_removeDisallowedBluetoothDevices', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHDEVICES_N
     * @tc.name ent_removeDisallowedBluetoothDevices_n
     * @tc.desc Verify Bluetooth API ent removeDisallowedBluetoothDevices normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothDevices_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        enterpriseBt.removeDisallowedBluetoothDevices({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, [BADDR]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHDEVICES_E
     * @tc.name ent_removeDisallowedBluetoothDevices_e
     * @tc.desc Verify Bluetooth API ent removeDisallowedBluetoothDevices error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothDevices_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        enterpriseBt.removeDisallowedBluetoothDevices({bundleName:'', abilityName:''}, [BADDR]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHDEVICES_B
     * @tc.name ent_removeDisallowedBluetoothDevices_b
     * @tc.desc Verify Bluetooth API ent removeDisallowedBluetoothDevices boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothDevices_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        enterpriseBt.removeDisallowedBluetoothDevices({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, [EMPTY_MAC]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHDEVICES_R
     * @tc.name ent_removeDisallowedBluetoothDevices_r
     * @tc.desc Verify Bluetooth API ent removeDisallowedBluetoothDevices return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothDevices_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        enterpriseBt.removeDisallowedBluetoothDevices({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, [BADDR]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHDEVICES_X
     * @tc.name ent_removeDisallowedBluetoothDevices_x
     * @tc.desc Verify Bluetooth API ent removeDisallowedBluetoothDevices repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothDevices_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        enterpriseBt.removeDisallowedBluetoothDevices({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, [BADDR]);
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
        enterpriseBt.removeDisallowedBluetoothDevices({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, [BADDR]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHDEVICES_C
     * @tc.name ent_removeDisallowedBluetoothDevices_c
     * @tc.desc Verify Bluetooth API ent removeDisallowedBluetoothDevices condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothDevices_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        enterpriseBt.removeDisallowedBluetoothDevices({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, [BADDR]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHDEVICES_PERF
     * @tc.name ent_removeDisallowedBluetoothDevices_perf
     * @tc.desc Verify Bluetooth apitest case ent removeDisallowedBluetoothDevices perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothDevices_perf', 0, () => {
      runPerf('ent_removeDisallowedBluetoothDevices_perf', 5000, () => {
        try {
          enterpriseBt.removeDisallowedBluetoothDevices({bundleName:'com.example.test',
            abilityName:'EntryAbility'}, [BADDR]);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHDEVICES_STRESS
     * @tc.name ent_removeDisallowedBluetoothDevices_stress
     * @tc.desc Verify Bluetooth apitest case ent removeDisallowedBluetoothDevices stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothDevices_stress', 0, () => {
      runStress('ent_removeDisallowedBluetoothDevices_stress', 20, () => {
        try {
          enterpriseBt.removeDisallowedBluetoothDevices({bundleName:'com.example.test',
            abilityName:'EntryAbility'}, [BADDR]);
        } catch (_e) {
        }
      });
    });
    });
    describe('ent_getDisallowedBluetoothDevices', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHDEVICES_N
     * @tc.name ent_getDisallowedBluetoothDevices_n
     * @tc.desc Verify Bluetooth API ent getDisallowedBluetoothDevices normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothDevices_n', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = enterpriseBt.getDisallowedBluetoothDevices({bundleName:'com.example.test',
          abilityName:'EntryAbility'}) as Array < Object > ;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHDEVICES_E
     * @tc.name ent_getDisallowedBluetoothDevices_e
     * @tc.desc Verify Bluetooth API ent getDisallowedBluetoothDevices error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothDevices_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        enterpriseBt.getDisallowedBluetoothDevices({bundleName:'', abilityName:''});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHDEVICES_B
     * @tc.name ent_getDisallowedBluetoothDevices_b
     * @tc.desc Verify Bluetooth API ent getDisallowedBluetoothDevices boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothDevices_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        enterpriseBt.getDisallowedBluetoothDevices({bundleName:'', abilityName:''});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHDEVICES_R
     * @tc.name ent_getDisallowedBluetoothDevices_r
     * @tc.desc Verify Bluetooth API ent getDisallowedBluetoothDevices return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothDevices_r', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = enterpriseBt.getDisallowedBluetoothDevices({bundleName:'com.example.test',
          abilityName:'EntryAbility'}) as Array < Object > ;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHDEVICES_X
     * @tc.name ent_getDisallowedBluetoothDevices_x
     * @tc.desc Verify Bluetooth API ent getDisallowedBluetoothDevices repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothDevices_x', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = enterpriseBt.getDisallowedBluetoothDevices({bundleName:'com.example.test',
          abilityName:'EntryAbility'}) as Array < Object > ;
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
        value = enterpriseBt.getDisallowedBluetoothDevices({bundleName:'com.example.test',
          abilityName:'EntryAbility'}) as Array < Object > ;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHDEVICES_C
     * @tc.name ent_getDisallowedBluetoothDevices_c
     * @tc.desc Verify Bluetooth API ent getDisallowedBluetoothDevices condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothDevices_c', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = enterpriseBt.getDisallowedBluetoothDevices({bundleName:'com.example.test',
          abilityName:'EntryAbility'}) as Array < Object > ;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHDEVICES_PERF
     * @tc.name ent_getDisallowedBluetoothDevices_perf
     * @tc.desc Verify Bluetooth apitest case ent getDisallowedBluetoothDevices perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothDevices_perf', 0, () => {
      runPerf('ent_getDisallowedBluetoothDevices_perf', 5000, () => {
        try {
          enterpriseBt.getDisallowedBluetoothDevices({bundleName:'com.example.test',
            abilityName:'EntryAbility'}) as Array < Object > ;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHDEVICES_STRESS
     * @tc.name ent_getDisallowedBluetoothDevices_stress
     * @tc.desc Verify Bluetooth apitest case ent getDisallowedBluetoothDevices stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothDevices_stress', 0, () => {
      runStress('ent_getDisallowedBluetoothDevices_stress', 20, () => {
        try {
          enterpriseBt.getDisallowedBluetoothDevices({bundleName:'com.example.test',
            abilityName:'EntryAbility'}) as Array < Object > ;
        } catch (_e) {
        }
      });
    });
    });
    describe('ent_addDisallowedBluetoothProtocols', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_ADDDISALLOWEDBLUETOOTHPROTOCOLS_N
     * @tc.name ent_addDisallowedBluetoothProtocols_n
     * @tc.desc Verify Bluetooth API ent addDisallowedBluetoothProtocols normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_addDisallowedBluetoothProtocols_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        enterpriseBt.addDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, 0, [0]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_ADDDISALLOWEDBLUETOOTHPROTOCOLS_E
     * @tc.name ent_addDisallowedBluetoothProtocols_e
     * @tc.desc Verify Bluetooth API ent addDisallowedBluetoothProtocols error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_addDisallowedBluetoothProtocols_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        enterpriseBt.addDisallowedBluetoothProtocols({bundleName:'', abilityName:''}, 0, [0]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_ADDDISALLOWEDBLUETOOTHPROTOCOLS_B
     * @tc.name ent_addDisallowedBluetoothProtocols_b
     * @tc.desc Verify Bluetooth API ent addDisallowedBluetoothProtocols boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_addDisallowedBluetoothProtocols_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        enterpriseBt.addDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, -1, [0]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_ADDDISALLOWEDBLUETOOTHPROTOCOLS_R
     * @tc.name ent_addDisallowedBluetoothProtocols_r
     * @tc.desc Verify Bluetooth API ent addDisallowedBluetoothProtocols return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_addDisallowedBluetoothProtocols_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        enterpriseBt.addDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, 0, [0]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_ADDDISALLOWEDBLUETOOTHPROTOCOLS_X
     * @tc.name ent_addDisallowedBluetoothProtocols_x
     * @tc.desc Verify Bluetooth API ent addDisallowedBluetoothProtocols repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_addDisallowedBluetoothProtocols_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        enterpriseBt.addDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, 0, [0]);
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
        enterpriseBt.addDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, 0, [0]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_ADDDISALLOWEDBLUETOOTHPROTOCOLS_C
     * @tc.name ent_addDisallowedBluetoothProtocols_c
     * @tc.desc Verify Bluetooth API ent addDisallowedBluetoothProtocols condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_addDisallowedBluetoothProtocols_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        enterpriseBt.addDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, 0, [0]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_ADDDISALLOWEDBLUETOOTHPROTOCOLS_PERF
     * @tc.name ent_addDisallowedBluetoothProtocols_perf
     * @tc.desc Verify Bluetooth apitest case ent addDisallowedBluetoothProtocols perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_addDisallowedBluetoothProtocols_perf', 0, () => {
      runPerf('ent_addDisallowedBluetoothProtocols_perf', 5000, () => {
        try {
          enterpriseBt.addDisallowedBluetoothProtocols({bundleName:'com.example.test',
            abilityName:'EntryAbility'}, 0, [0]);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_ADDDISALLOWEDBLUETOOTHPROTOCOLS_STRESS
     * @tc.name ent_addDisallowedBluetoothProtocols_stress
     * @tc.desc Verify Bluetooth apitest case ent addDisallowedBluetoothProtocols stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_addDisallowedBluetoothProtocols_stress', 0, () => {
      runStress('ent_addDisallowedBluetoothProtocols_stress', 20, () => {
        try {
          enterpriseBt.addDisallowedBluetoothProtocols({bundleName:'com.example.test',
            abilityName:'EntryAbility'}, 0, [0]);
        } catch (_e) {
        }
      });
    });
    });
    describe('ent_removeDisallowedBluetoothProtocols', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHPROTOCOLS_N
     * @tc.name ent_removeDisallowedBluetoothProtocols_n
     * @tc.desc Verify Bluetooth API ent removeDisallowedBluetoothProtocols normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothProtocols_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        enterpriseBt.removeDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, 0, [0]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHPROTOCOLS_E
     * @tc.name ent_removeDisallowedBluetoothProtocols_e
     * @tc.desc Verify Bluetooth API ent removeDisallowedBluetoothProtocols error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothProtocols_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        enterpriseBt.removeDisallowedBluetoothProtocols({bundleName:'', abilityName:''}, 0, [0]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHPROTOCOLS_B
     * @tc.name ent_removeDisallowedBluetoothProtocols_b
     * @tc.desc Verify Bluetooth API ent removeDisallowedBluetoothProtocols boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothProtocols_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        enterpriseBt.removeDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, -1, [0]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHPROTOCOLS_R
     * @tc.name ent_removeDisallowedBluetoothProtocols_r
     * @tc.desc Verify Bluetooth API ent removeDisallowedBluetoothProtocols return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothProtocols_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        enterpriseBt.removeDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, 0, [0]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHPROTOCOLS_X
     * @tc.name ent_removeDisallowedBluetoothProtocols_x
     * @tc.desc Verify Bluetooth API ent removeDisallowedBluetoothProtocols repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothProtocols_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        enterpriseBt.removeDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, 0, [0]);
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
        enterpriseBt.removeDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, 0, [0]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHPROTOCOLS_C
     * @tc.name ent_removeDisallowedBluetoothProtocols_c
     * @tc.desc Verify Bluetooth API ent removeDisallowedBluetoothProtocols condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothProtocols_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        enterpriseBt.removeDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, 0, [0]);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHPROTOCOLS_PERF
     * @tc.name ent_removeDisallowedBluetoothProtocols_perf
     * @tc.desc Verify Bluetooth apitest case ent removeDisallowedBluetoothProtocols perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothProtocols_perf', 0, () => {
      runPerf('ent_removeDisallowedBluetoothProtocols_perf', 5000, () => {
        try {
          enterpriseBt.removeDisallowedBluetoothProtocols({bundleName:'com.example.test',
            abilityName:'EntryAbility'}, 0, [0]);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_REMOVEDISALLOWEDBLUETOOTHPROTOCOLS_STRESS
     * @tc.name ent_removeDisallowedBluetoothProtocols_stress
     * @tc.desc Verify Bluetooth apitest case ent removeDisallowedBluetoothProtocols stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_removeDisallowedBluetoothProtocols_stress', 0, () => {
      runStress('ent_removeDisallowedBluetoothProtocols_stress', 20, () => {
        try {
          enterpriseBt.removeDisallowedBluetoothProtocols({bundleName:'com.example.test',
            abilityName:'EntryAbility'}, 0, [0]);
        } catch (_e) {
        }
      });
    });
    });
    describe('ent_getDisallowedBluetoothProtocols', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHPROTOCOLS_N
     * @tc.name ent_getDisallowedBluetoothProtocols_n
     * @tc.desc Verify Bluetooth API ent getDisallowedBluetoothProtocols normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothProtocols_n', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = enterpriseBt.getDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, 0) as Array < Object > ;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHPROTOCOLS_E
     * @tc.name ent_getDisallowedBluetoothProtocols_e
     * @tc.desc Verify Bluetooth API ent getDisallowedBluetoothProtocols error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothProtocols_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        enterpriseBt.getDisallowedBluetoothProtocols({bundleName:'', abilityName:''}, 0);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHPROTOCOLS_B
     * @tc.name ent_getDisallowedBluetoothProtocols_b
     * @tc.desc Verify Bluetooth API ent getDisallowedBluetoothProtocols boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothProtocols_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        enterpriseBt.getDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, -1);
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHPROTOCOLS_R
     * @tc.name ent_getDisallowedBluetoothProtocols_r
     * @tc.desc Verify Bluetooth API ent getDisallowedBluetoothProtocols return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothProtocols_r', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = enterpriseBt.getDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, 0) as Array < Object > ;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHPROTOCOLS_X
     * @tc.name ent_getDisallowedBluetoothProtocols_x
     * @tc.desc Verify Bluetooth API ent getDisallowedBluetoothProtocols repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothProtocols_x', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = enterpriseBt.getDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, 0) as Array < Object > ;
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
        value = enterpriseBt.getDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, 0) as Array < Object > ;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHPROTOCOLS_C
     * @tc.name ent_getDisallowedBluetoothProtocols_c
     * @tc.desc Verify Bluetooth API ent getDisallowedBluetoothProtocols condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothProtocols_c', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = enterpriseBt.getDisallowedBluetoothProtocols({bundleName:'com.example.test',
          abilityName:'EntryAbility'}, 0) as Array < Object > ;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHPROTOCOLS_PERF
     * @tc.name ent_getDisallowedBluetoothProtocols_perf
     * @tc.desc Verify Bluetooth apitest case ent getDisallowedBluetoothProtocols perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothProtocols_perf', 0, () => {
      runPerf('ent_getDisallowedBluetoothProtocols_perf', 5000, () => {
        try {
          enterpriseBt.getDisallowedBluetoothProtocols({bundleName:'com.example.test',
            abilityName:'EntryAbility'}, 0) as Array < Object > ;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ENT_GETDISALLOWEDBLUETOOTHPROTOCOLS_STRESS
     * @tc.name ent_getDisallowedBluetoothProtocols_stress
     * @tc.desc Verify Bluetooth apitest case ent getDisallowedBluetoothProtocols stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('ent_getDisallowedBluetoothProtocols_stress', 0, () => {
      runStress('ent_getDisallowedBluetoothProtocols_stress', 20, () => {
        try {
          enterpriseBt.getDisallowedBluetoothProtocols({bundleName:'com.example.test',
            abilityName:'EntryAbility'}, 0) as Array < Object > ;
        } catch (_e) {
        }
      });
    });
    });
    describe('pa_isPartnerAgentSupported', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISPARTNERAGENTSUPPORTED_N
     * @tc.name pa_isPartnerAgentSupported_n
     * @tc.desc Verify Bluetooth API pa isPartnerAgentSupported normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isPartnerAgentSupported_n', 0, () => {
      let errCode = 0;
      let value = false;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = partnerAgent.isPartnerAgentSupported();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISPARTNERAGENTSUPPORTED_E
     * @tc.name pa_isPartnerAgentSupported_e
     * @tc.desc Verify Bluetooth API pa isPartnerAgentSupported error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isPartnerAgentSupported_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        partnerAgent.isPartnerAgentSupported();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISPARTNERAGENTSUPPORTED_B
     * @tc.name pa_isPartnerAgentSupported_b
     * @tc.desc Verify Bluetooth API pa isPartnerAgentSupported boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isPartnerAgentSupported_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        partnerAgent.isPartnerAgentSupported();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISPARTNERAGENTSUPPORTED_R
     * @tc.name pa_isPartnerAgentSupported_r
     * @tc.desc Verify Bluetooth API pa isPartnerAgentSupported return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isPartnerAgentSupported_r', 0, () => {
      let errCode = 0;
      let value = false;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = partnerAgent.isPartnerAgentSupported();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISPARTNERAGENTSUPPORTED_X
     * @tc.name pa_isPartnerAgentSupported_x
     * @tc.desc Verify Bluetooth API pa isPartnerAgentSupported repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isPartnerAgentSupported_x', 0, () => {
      let errCode = 0;
      let value = false;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = partnerAgent.isPartnerAgentSupported();
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
        value = partnerAgent.isPartnerAgentSupported();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISPARTNERAGENTSUPPORTED_C
     * @tc.name pa_isPartnerAgentSupported_c
     * @tc.desc Verify Bluetooth API pa isPartnerAgentSupported condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isPartnerAgentSupported_c', 0, () => {
      let errCode = 0;
      let value = false;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = partnerAgent.isPartnerAgentSupported();
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISPARTNERAGENTSUPPORTED_PERF
     * @tc.name pa_isPartnerAgentSupported_perf
     * @tc.desc Verify Bluetooth apitest case pa isPartnerAgentSupported perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isPartnerAgentSupported_perf', 0, () => {
      runPerf('pa_isPartnerAgentSupported_perf', 5000, () => {
        try {
          partnerAgent.isPartnerAgentSupported();
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISPARTNERAGENTSUPPORTED_STRESS
     * @tc.name pa_isPartnerAgentSupported_stress
     * @tc.desc Verify Bluetooth apitest case pa isPartnerAgentSupported stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isPartnerAgentSupported_stress', 0, () => {
      runStress('pa_isPartnerAgentSupported_stress', 20, () => {
        try {
          partnerAgent.isPartnerAgentSupported();
        } catch (_e) {
        }
      });
    });
    });
    describe('pa_bindDevice', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_BINDDEVICE_N
     * @tc.name pa_bindDevice_n
     * @tc.desc Verify Bluetooth API pa bindDevice normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_bindDevice_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = partnerAgent.bindDevice(partnerDeviceAddr(), partnerDeviceCap(), partnerBusinessCap(),
          'EntryAbility') as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_BINDDEVICE_E
     * @tc.name pa_bindDevice_e
     * @tc.desc Verify Bluetooth API pa bindDevice error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_bindDevice_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        partnerAgent.bindDevice({bluetoothAddress:{address:EMPTY_MAC, addressType:0}}, partnerDeviceCap(),
          partnerBusinessCap(), 'EntryAbility');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_BINDDEVICE_B
     * @tc.name pa_bindDevice_b
     * @tc.desc Verify Bluetooth API pa bindDevice boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_bindDevice_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        partnerAgent.bindDevice(partnerDeviceAddr(), partnerDeviceCap(), partnerBusinessCap(), '');
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_BINDDEVICE_R
     * @tc.name pa_bindDevice_r
     * @tc.desc Verify Bluetooth API pa bindDevice return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_bindDevice_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = partnerAgent.bindDevice(partnerDeviceAddr(), partnerDeviceCap(), partnerBusinessCap(),
          'EntryAbility') as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_BINDDEVICE_X
     * @tc.name pa_bindDevice_x
     * @tc.desc Verify Bluetooth API pa bindDevice repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_bindDevice_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = partnerAgent.bindDevice(partnerDeviceAddr(), partnerDeviceCap(), partnerBusinessCap(),
          'EntryAbility') as Object;
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
        value = partnerAgent.bindDevice(partnerDeviceAddr(), partnerDeviceCap(), partnerBusinessCap(),
          'EntryAbility') as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_BINDDEVICE_C
     * @tc.name pa_bindDevice_c
     * @tc.desc Verify Bluetooth API pa bindDevice condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_bindDevice_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = partnerAgent.bindDevice(partnerDeviceAddr(), partnerDeviceCap(), partnerBusinessCap(),
          'EntryAbility') as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_BINDDEVICE_PERF
     * @tc.name pa_bindDevice_perf
     * @tc.desc Verify Bluetooth apitest case pa bindDevice perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_bindDevice_perf', 0, () => {
      runPerf('pa_bindDevice_perf', 5000, () => {
        try {
          partnerAgent.bindDevice(partnerDeviceAddr(), partnerDeviceCap(), partnerBusinessCap(),
            'EntryAbility') as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_BINDDEVICE_STRESS
     * @tc.name pa_bindDevice_stress
     * @tc.desc Verify Bluetooth apitest case pa bindDevice stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_bindDevice_stress', 0, () => {
      runStress('pa_bindDevice_stress', 20, () => {
        try {
          partnerAgent.bindDevice(partnerDeviceAddr(), partnerDeviceCap(), partnerBusinessCap(),
            'EntryAbility') as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('pa_unbindDevice', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_UNBINDDEVICE_N
     * @tc.name pa_unbindDevice_n
     * @tc.desc Verify Bluetooth API pa unbindDevice normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_unbindDevice_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = partnerAgent.unbindDevice(partnerDeviceAddr()) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_UNBINDDEVICE_E
     * @tc.name pa_unbindDevice_e
     * @tc.desc Verify Bluetooth API pa unbindDevice error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_unbindDevice_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        partnerAgent.unbindDevice({bluetoothAddress:{address:EMPTY_MAC, addressType:0}});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_UNBINDDEVICE_B
     * @tc.name pa_unbindDevice_b
     * @tc.desc Verify Bluetooth API pa unbindDevice boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_unbindDevice_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        partnerAgent.unbindDevice({bluetoothAddress:{address:EMPTY_MAC, addressType:0}});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_UNBINDDEVICE_R
     * @tc.name pa_unbindDevice_r
     * @tc.desc Verify Bluetooth API pa unbindDevice return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_unbindDevice_r', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = partnerAgent.unbindDevice(partnerDeviceAddr()) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_UNBINDDEVICE_X
     * @tc.name pa_unbindDevice_x
     * @tc.desc Verify Bluetooth API pa unbindDevice repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_unbindDevice_x', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      try {
        value = partnerAgent.unbindDevice(partnerDeviceAddr()) as Object;
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
        value = partnerAgent.unbindDevice(partnerDeviceAddr()) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_UNBINDDEVICE_C
     * @tc.name pa_unbindDevice_c
     * @tc.desc Verify Bluetooth API pa unbindDevice condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_unbindDevice_c', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      try {
        value = partnerAgent.unbindDevice(partnerDeviceAddr()) as Object;
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_UNBINDDEVICE_PERF
     * @tc.name pa_unbindDevice_perf
     * @tc.desc Verify Bluetooth apitest case pa unbindDevice perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_unbindDevice_perf', 0, () => {
      runPerf('pa_unbindDevice_perf', 5000, () => {
        try {
          partnerAgent.unbindDevice(partnerDeviceAddr()) as Object;
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_UNBINDDEVICE_STRESS
     * @tc.name pa_unbindDevice_stress
     * @tc.desc Verify Bluetooth apitest case pa unbindDevice stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_unbindDevice_stress', 0, () => {
      runStress('pa_unbindDevice_stress', 20, () => {
        try {
          partnerAgent.unbindDevice(partnerDeviceAddr()) as Object;
        } catch (_e) {
        }
      });
    });
    });
    describe('pa_isDeviceBound', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICEBOUND_N
     * @tc.name pa_isDeviceBound_n
     * @tc.desc Verify Bluetooth API pa isDeviceBound normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceBound_n', 0, () => {
      let errCode = 0;
      let value = false;
      hilog.info(0x0000, 'BtFullCoverage', 'n: invoke');
      try {
        value = partnerAgent.isDeviceBound(partnerDeviceAddr());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICEBOUND_E
     * @tc.name pa_isDeviceBound_e
     * @tc.desc Verify Bluetooth API pa isDeviceBound error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceBound_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke expect error');
      try {
        partnerAgent.isDeviceBound({bluetoothAddress:{address:EMPTY_MAC, addressType:0}});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICEBOUND_B
     * @tc.name pa_isDeviceBound_b
     * @tc.desc Verify Bluetooth API pa isDeviceBound boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceBound_b', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke expect error');
      try {
        partnerAgent.isDeviceBound({bluetoothAddress:{address:EMPTY_MAC, addressType:0}});
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICEBOUND_R
     * @tc.name pa_isDeviceBound_r
     * @tc.desc Verify Bluetooth API pa isDeviceBound return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceBound_r', 0, () => {
      let errCode = 0;
      let value = false;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      try {
        value = partnerAgent.isDeviceBound(partnerDeviceAddr());
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
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_ISDEVICEBOUND_X
     * @tc.name pa_isDeviceBound_x
     * @tc.desc Verify Bluetooth API pa isDeviceBound repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_isDeviceBound_x', 0, () => {
      let errCode = 0;
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


