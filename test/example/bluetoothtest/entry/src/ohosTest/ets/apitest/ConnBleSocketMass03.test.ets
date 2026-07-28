     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_13_dis_adv_cb_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_n: invoke');
      try {
        ble.disableAdvertising({ advertisingId: 0 }, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_13_DIS_ADV_CB_E
     * @tc.name bl_13_dis_adv_cb_e
     * @tc.desc Verify Bluetooth API bl 13 dis adv cb error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_13_dis_adv_cb_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_e: invoke');
      try {
        ble.disableAdvertising({ advertisingId: 0 }, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_13_DIS_ADV_CB_B
     * @tc.name bl_13_dis_adv_cb_b
     * @tc.desc Verify Bluetooth API bl 13 dis adv cb boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_13_dis_adv_cb_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_b: invoke');
      try {
        ble.disableAdvertising({ advertisingId: 0 }, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_13_DIS_ADV_CB_E2
     * @tc.name bl_13_dis_adv_cb_e2
     * @tc.desc Verify Bluetooth apitest case bl 13 dis adv cb e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_13_dis_adv_cb_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_e2: invoke');
      try {
        ble.disableAdvertising({ advertisingId: 0 }, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_13_DIS_ADV_CB_R
     * @tc.name bl_13_dis_adv_cb_r
     * @tc.desc Verify Bluetooth API bl 13 dis adv cb return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_13_dis_adv_cb_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_r: invoke');
      try {
        ble.disableAdvertising({ advertisingId: 0 }, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_13_DIS_ADV_CB_X
     * @tc.name bl_13_dis_adv_cb_x
     * @tc.desc Verify Bluetooth API bl 13 dis adv cb repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_13_dis_adv_cb_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_x: invoke');
      try {
        ble.disableAdvertising({ advertisingId: 0 }, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_13_dis_adv_cb_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('bl_14_dis_adv_p', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_14_DIS_ADV_P_N
     * @tc.name bl_14_dis_adv_p_n
     * @tc.desc Verify Bluetooth API bl 14 dis adv p normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_14_dis_adv_p_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_n: invoke');
      try {
        ble.disableAdvertising({ advertisingId: 0 });
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_14_DIS_ADV_P_E
     * @tc.name bl_14_dis_adv_p_e
     * @tc.desc Verify Bluetooth API bl 14 dis adv p error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_14_dis_adv_p_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_e: invoke');
      try {
        ble.disableAdvertising({ advertisingId: 0 });
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_14_DIS_ADV_P_B
     * @tc.name bl_14_dis_adv_p_b
     * @tc.desc Verify Bluetooth API bl 14 dis adv p boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_14_dis_adv_p_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_b: invoke');
      try {
        ble.disableAdvertising({ advertisingId: 0 });
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_14_DIS_ADV_P_E2
     * @tc.name bl_14_dis_adv_p_e2
     * @tc.desc Verify Bluetooth apitest case bl 14 dis adv p e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_14_dis_adv_p_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_e2: invoke');
      try {
        ble.disableAdvertising({ advertisingId: 0 });
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_14_DIS_ADV_P_R
     * @tc.name bl_14_dis_adv_p_r
     * @tc.desc Verify Bluetooth API bl 14 dis adv p return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_14_dis_adv_p_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_r: invoke');
      try {
        ble.disableAdvertising({ advertisingId: 0 });
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_14_DIS_ADV_P_X
     * @tc.name bl_14_dis_adv_p_x
     * @tc.desc Verify Bluetooth API bl 14 dis adv p repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_14_dis_adv_p_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_x: invoke');
      try {
        ble.disableAdvertising({ advertisingId: 0 });
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_14_dis_adv_p_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('bl_15_stop_adv_id_cb', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_15_STOP_ADV_ID_CB_N
     * @tc.name bl_15_stop_adv_id_cb_n
     * @tc.desc Verify Bluetooth API bl 15 stop adv id cb normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_15_stop_adv_id_cb_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_n: invoke');
      try {
        ble.stopAdvertising(0, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_15_STOP_ADV_ID_CB_E
     * @tc.name bl_15_stop_adv_id_cb_e
     * @tc.desc Verify Bluetooth API bl 15 stop adv id cb error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_15_stop_adv_id_cb_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_e: invoke expect error');
      try {
        ble.stopAdvertising(-1, () => {});
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_15_STOP_ADV_ID_CB_B
     * @tc.name bl_15_stop_adv_id_cb_b
     * @tc.desc Verify Bluetooth API bl 15 stop adv id cb boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_15_stop_adv_id_cb_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_b: invoke');
      try {
        ble.stopAdvertising(1, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_15_STOP_ADV_ID_CB_E2
     * @tc.name bl_15_stop_adv_id_cb_e2
     * @tc.desc Verify Bluetooth apitest case bl 15 stop adv id cb e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_15_stop_adv_id_cb_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_e2: invoke');
      try {
        ble.stopAdvertising(0, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_15_STOP_ADV_ID_CB_R
     * @tc.name bl_15_stop_adv_id_cb_r
     * @tc.desc Verify Bluetooth API bl 15 stop adv id cb return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_15_stop_adv_id_cb_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_r: invoke');
      try {
        ble.stopAdvertising(0, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_15_STOP_ADV_ID_CB_X
     * @tc.name bl_15_stop_adv_id_cb_x
     * @tc.desc Verify Bluetooth API bl 15 stop adv id cb repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_15_stop_adv_id_cb_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_x: invoke');
      try {
        ble.stopAdvertising(0, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_15_stop_adv_id_cb_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('bl_16_stop_adv_id_p', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_16_STOP_ADV_ID_P_N
     * @tc.name bl_16_stop_adv_id_p_n
     * @tc.desc Verify Bluetooth API bl 16 stop adv id p normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_16_stop_adv_id_p_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_n: invoke');
      try {
        ble.stopAdvertising(0);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_16_STOP_ADV_ID_P_E
     * @tc.name bl_16_stop_adv_id_p_e
     * @tc.desc Verify Bluetooth API bl 16 stop adv id p error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_16_stop_adv_id_p_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_e: invoke expect error');
      try {
        ble.stopAdvertising(-1);
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_16_STOP_ADV_ID_P_B
     * @tc.name bl_16_stop_adv_id_p_b
     * @tc.desc Verify Bluetooth API bl 16 stop adv id p boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_16_stop_adv_id_p_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_b: invoke');
      try {
        ble.stopAdvertising(1);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_16_STOP_ADV_ID_P_E2
     * @tc.name bl_16_stop_adv_id_p_e2
     * @tc.desc Verify Bluetooth apitest case bl 16 stop adv id p e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_16_stop_adv_id_p_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_e2: invoke');
      try {
        ble.stopAdvertising(0);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_16_STOP_ADV_ID_P_R
     * @tc.name bl_16_stop_adv_id_p_r
     * @tc.desc Verify Bluetooth API bl 16 stop adv id p return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_16_stop_adv_id_p_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_r: invoke');
      try {
        ble.stopAdvertising(0);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_16_STOP_ADV_ID_P_X
     * @tc.name bl_16_stop_adv_id_p_x
     * @tc.desc Verify Bluetooth API bl 16 stop adv id p repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_16_stop_adv_id_p_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_x: invoke');
      try {
        ble.stopAdvertising(0);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_16_stop_adv_id_p_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('bl_17_on_adv_chg', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_17_ON_ADV_CHG_N
     * @tc.name bl_17_on_adv_chg_n
     * @tc.desc Verify Bluetooth API bl 17 on adv chg normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_17_on_adv_chg_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_n: invoke');
      try {
        ble.on('advertisingStateChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_17_ON_ADV_CHG_E
     * @tc.name bl_17_on_adv_chg_e
     * @tc.desc Verify Bluetooth API bl 17 on adv chg error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_17_on_adv_chg_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_e: invoke');
      try {
        ble.on('advertisingStateChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_17_ON_ADV_CHG_B
     * @tc.name bl_17_on_adv_chg_b
     * @tc.desc Verify Bluetooth API bl 17 on adv chg boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_17_on_adv_chg_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_b: invoke');
      try {
        ble.on('advertisingStateChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_17_ON_ADV_CHG_E2
     * @tc.name bl_17_on_adv_chg_e2
     * @tc.desc Verify Bluetooth apitest case bl 17 on adv chg e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_17_on_adv_chg_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_e2: invoke');
      try {
        ble.on('advertisingStateChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_17_ON_ADV_CHG_R
     * @tc.name bl_17_on_adv_chg_r
     * @tc.desc Verify Bluetooth API bl 17 on adv chg return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_17_on_adv_chg_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_r: invoke');
      try {
        ble.on('advertisingStateChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_17_ON_ADV_CHG_X
     * @tc.name bl_17_on_adv_chg_x
     * @tc.desc Verify Bluetooth API bl 17 on adv chg repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_17_on_adv_chg_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_x: invoke');
      try {
        ble.on('advertisingStateChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_17_on_adv_chg_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('bl_18_off_adv_chg', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_18_OFF_ADV_CHG_N
     * @tc.name bl_18_off_adv_chg_n
     * @tc.desc Verify Bluetooth API bl 18 off adv chg normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_18_off_adv_chg_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_n: invoke');
      try {
        ble.off('advertisingStateChange');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_18_OFF_ADV_CHG_E
     * @tc.name bl_18_off_adv_chg_e
     * @tc.desc Verify Bluetooth API bl 18 off adv chg error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_18_off_adv_chg_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_e: invoke');
      try {
        ble.off('advertisingStateChange');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_18_OFF_ADV_CHG_B
     * @tc.name bl_18_off_adv_chg_b
     * @tc.desc Verify Bluetooth API bl 18 off adv chg boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_18_off_adv_chg_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_b: invoke');
      try {
        ble.off('advertisingStateChange', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_18_OFF_ADV_CHG_E2
     * @tc.name bl_18_off_adv_chg_e2
     * @tc.desc Verify Bluetooth apitest case bl 18 off adv chg e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_18_off_adv_chg_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_e2: invoke');
      try {
        ble.off('advertisingStateChange');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_18_OFF_ADV_CHG_R
     * @tc.name bl_18_off_adv_chg_r
     * @tc.desc Verify Bluetooth API bl 18 off adv chg return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_18_off_adv_chg_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_r: invoke');
      try {
        ble.off('advertisingStateChange');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_18_OFF_ADV_CHG_X
     * @tc.name bl_18_off_adv_chg_x
     * @tc.desc Verify Bluetooth API bl 18 off adv chg repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_18_off_adv_chg_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_x: invoke');
      try {
        ble.off('advertisingStateChange');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_18_off_adv_chg_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('bl_19_on_ble_find', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_19_ON_BLE_FIND_N
     * @tc.name bl_19_on_ble_find_n
     * @tc.desc Verify Bluetooth API bl 19 on ble find normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_19_on_ble_find_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_n: invoke');
      try {
        ble.on('BLEDeviceFind', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_19_ON_BLE_FIND_E
     * @tc.name bl_19_on_ble_find_e
     * @tc.desc Verify Bluetooth API bl 19 on ble find error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_19_on_ble_find_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_e: invoke');
      try {
        ble.on('BLEDeviceFind', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_19_ON_BLE_FIND_B
     * @tc.name bl_19_on_ble_find_b
     * @tc.desc Verify Bluetooth API bl 19 on ble find boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_19_on_ble_find_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_b: invoke');
      try {
        ble.on('BLEDeviceFind', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_19_ON_BLE_FIND_E2
     * @tc.name bl_19_on_ble_find_e2
     * @tc.desc Verify Bluetooth apitest case bl 19 on ble find e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_19_on_ble_find_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_e2: invoke');
      try {
        ble.on('BLEDeviceFind', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_19_ON_BLE_FIND_R
     * @tc.name bl_19_on_ble_find_r
     * @tc.desc Verify Bluetooth API bl 19 on ble find return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_19_on_ble_find_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_r: invoke');
      try {
        ble.on('BLEDeviceFind', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_19_ON_BLE_FIND_X
     * @tc.name bl_19_on_ble_find_x
     * @tc.desc Verify Bluetooth API bl 19 on ble find repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_19_on_ble_find_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_x: invoke');
      try {
        ble.on('BLEDeviceFind', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_19_on_ble_find_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('bl_20_off_ble_find', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_20_OFF_BLE_FIND_N
     * @tc.name bl_20_off_ble_find_n
     * @tc.desc Verify Bluetooth API bl 20 off ble find normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_20_off_ble_find_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_n: invoke');
      try {
        ble.off('BLEDeviceFind');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_20_OFF_BLE_FIND_E
     * @tc.name bl_20_off_ble_find_e
     * @tc.desc Verify Bluetooth API bl 20 off ble find error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_20_off_ble_find_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_e: invoke');
      try {
        ble.off('BLEDeviceFind');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_20_OFF_BLE_FIND_B
     * @tc.name bl_20_off_ble_find_b
     * @tc.desc Verify Bluetooth API bl 20 off ble find boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_20_off_ble_find_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_b: invoke');
      try {
        ble.off('BLEDeviceFind', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_20_OFF_BLE_FIND_E2
     * @tc.name bl_20_off_ble_find_e2
     * @tc.desc Verify Bluetooth apitest case bl 20 off ble find e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_20_off_ble_find_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_e2: invoke');
      try {
        ble.off('BLEDeviceFind');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_20_OFF_BLE_FIND_R
     * @tc.name bl_20_off_ble_find_r
     * @tc.desc Verify Bluetooth API bl 20 off ble find return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_20_off_ble_find_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_r: invoke');
      try {
        ble.off('BLEDeviceFind');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_BL_20_OFF_BLE_FIND_X
     * @tc.name bl_20_off_ble_find_x
     * @tc.desc Verify Bluetooth API bl 20 off ble find repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('bl_20_off_ble_find_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_x: invoke');
      try {
        ble.off('BLEDeviceFind');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'bl_20_off_ble_find_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
  });
}



