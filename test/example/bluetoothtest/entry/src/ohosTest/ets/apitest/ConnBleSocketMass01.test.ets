        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_12_remote_name_2_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_12_remote_name_2_r: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(typeof value).assertEqual('string');
      expect(value.length >= 0).assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_12_REMOTE_NAME_2_X
     * @tc.name cn_12_remote_name_2_x
     * @tc.desc Verify Bluetooth API cn 12 remote name 2 repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_12_remote_name_2_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_12_remote_name_2_x: invoke');
      try {
        connection.getRemoteDeviceName(BADDR, true);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_12_remote_name_2_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_12_remote_name_2_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_13_remote_class', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_13_REMOTE_CLASS_N
     * @tc.name cn_13_remote_class_n
     * @tc.desc Verify Bluetooth API cn 13 remote class normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_13_remote_class_n', 0, () => {
      let errCode = 0;
      let value: Object | null = null;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_13_remote_class_n: invoke');
      try {
        value = connection.getRemoteDeviceClass(BADDR) as Object;
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_13_remote_class_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_13_remote_class_n: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || (value !== null && value !== undefined)).assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_13_REMOTE_CLASS_E
     * @tc.name cn_13_remote_class_e
     * @tc.desc Verify Bluetooth API cn 13 remote class error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_13_remote_class_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_13_remote_class_e: invoke expect error');
      try {
        connection.getRemoteDeviceClass('');
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_13_remote_class_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_13_remote_class_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_13_REMOTE_CLASS_B
     * @tc.name cn_13_remote_class_b
     * @tc.desc Verify Bluetooth API cn 13 remote class boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_13_remote_class_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_13_remote_class_b: invoke');
      try {
        connection.getRemoteDeviceClass(BADDR);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_13_remote_class_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_13_remote_class_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_13_REMOTE_CLASS_E2
     * @tc.name cn_13_remote_class_e2
     * @tc.desc Verify Bluetooth apitest case cn 13 remote class e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_13_remote_class_e2', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_13_remote_class_e2: invoke expect error');
      try {
        connection.getRemoteDeviceClass('');
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_13_remote_class_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_13_remote_class_e2: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_13_REMOTE_CLASS_R
     * @tc.name cn_13_remote_class_r
     * @tc.desc Verify Bluetooth API cn 13 remote class return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_13_remote_class_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_13_remote_class_r: invoke');
      try {
        connection.getRemoteDeviceClass(BADDR);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_13_remote_class_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_13_remote_class_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_13_REMOTE_CLASS_X
     * @tc.name cn_13_remote_class_x
     * @tc.desc Verify Bluetooth API cn 13 remote class repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_13_remote_class_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_13_remote_class_x: invoke');
      try {
        connection.getRemoteDeviceClass(BADDR);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_13_remote_class_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_13_remote_class_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_14_remote_transport', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_14_REMOTE_TRANSPORT_N
     * @tc.name cn_14_remote_transport_n
     * @tc.desc Verify Bluetooth API cn 14 remote transport normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_14_remote_transport_n', 0, () => {
      let errCode = 0;
      let value = -1;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_14_remote_transport_n: invoke');
      try {
        value = connection.getRemoteDeviceTransport(BADDR);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_14_remote_transport_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_14_remote_transport_n: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_14_REMOTE_TRANSPORT_E
     * @tc.name cn_14_remote_transport_e
     * @tc.desc Verify Bluetooth API cn 14 remote transport error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_14_remote_transport_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_14_remote_transport_e: invoke expect error');
      try {
        connection.getRemoteDeviceTransport('');
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_14_remote_transport_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_14_remote_transport_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_14_REMOTE_TRANSPORT_B
     * @tc.name cn_14_remote_transport_b
     * @tc.desc Verify Bluetooth API cn 14 remote transport boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_14_remote_transport_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_14_remote_transport_b: invoke');
      try {
        connection.getRemoteDeviceTransport(BADDR);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_14_remote_transport_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_14_remote_transport_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_14_REMOTE_TRANSPORT_E2
     * @tc.name cn_14_remote_transport_e2
     * @tc.desc Verify Bluetooth apitest case cn 14 remote transport e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_14_remote_transport_e2', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_14_remote_transport_e2: invoke expect error');
      try {
        connection.getRemoteDeviceTransport('');
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_14_remote_transport_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_14_remote_transport_e2: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_14_REMOTE_TRANSPORT_R
     * @tc.name cn_14_remote_transport_r
     * @tc.desc Verify Bluetooth API cn 14 remote transport return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_14_remote_transport_r', 0, () => {
      let errCode = 0;
      let value = -1;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_14_remote_transport_r: invoke');
      try {
        value = connection.getRemoteDeviceTransport(BADDR);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_14_remote_transport_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_14_remote_transport_r: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_14_REMOTE_TRANSPORT_X
     * @tc.name cn_14_remote_transport_x
     * @tc.desc Verify Bluetooth API cn 14 remote transport repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_14_remote_transport_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_14_remote_transport_x: invoke');
      try {
        connection.getRemoteDeviceTransport(BADDR);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_14_remote_transport_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_14_remote_transport_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_15_get_local_name', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_15_GET_LOCAL_NAME_N
     * @tc.name cn_15_get_local_name_n
     * @tc.desc Verify Bluetooth API cn 15 get local name normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_15_get_local_name_n', 0, () => {
      let errCode = 0;
      let value = '';
      hilog.info(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_n: invoke');
      try {
        value = connection.getLocalName();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_15_get_local_name_n: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(typeof value).assertEqual('string');
      expect(value.length >= 0).assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_15_GET_LOCAL_NAME_E
     * @tc.name cn_15_get_local_name_e
     * @tc.desc Verify Bluetooth API cn 15 get local name error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_15_get_local_name_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_e: invoke');
      try {
        connection.getLocalName();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_15_GET_LOCAL_NAME_B
     * @tc.name cn_15_get_local_name_b
     * @tc.desc Verify Bluetooth API cn 15 get local name boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_15_get_local_name_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_b: invoke');
      try {
        connection.getLocalName();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_15_GET_LOCAL_NAME_E2
     * @tc.name cn_15_get_local_name_e2
     * @tc.desc Verify Bluetooth apitest case cn 15 get local name e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_15_get_local_name_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_e2: invoke');
      try {
        connection.getLocalName();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_15_GET_LOCAL_NAME_R
     * @tc.name cn_15_get_local_name_r
     * @tc.desc Verify Bluetooth API cn 15 get local name return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_15_get_local_name_r', 0, () => {
      let errCode = 0;
      let value = '';
      hilog.info(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_r: invoke');
      try {
        value = connection.getLocalName();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_15_get_local_name_r: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(typeof value).assertEqual('string');
      expect(value.length >= 0).assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_15_GET_LOCAL_NAME_X
     * @tc.name cn_15_get_local_name_x
     * @tc.desc Verify Bluetooth API cn 15 get local name repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_15_get_local_name_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_x: invoke');
      try {
        connection.getLocalName();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_15_get_local_name_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_16_get_paired', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_16_GET_PAIRED_N
     * @tc.name cn_16_get_paired_n
     * @tc.desc Verify Bluetooth API cn 16 get paired normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_16_get_paired_n', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'ConnBleSocket', 'cn_16_get_paired_n: invoke');
      try {
        value = connection.getPairedDevices() as Array < Object > ;
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_16_get_paired_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_16_get_paired_n: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(Array.isArray(value)).assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_16_GET_PAIRED_E
     * @tc.name cn_16_get_paired_e
     * @tc.desc Verify Bluetooth API cn 16 get paired error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_16_get_paired_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_16_get_paired_e: invoke');
      try {
        connection.getPairedDevices();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_16_get_paired_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_16_get_paired_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_16_GET_PAIRED_B
     * @tc.name cn_16_get_paired_b
     * @tc.desc Verify Bluetooth API cn 16 get paired boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_16_get_paired_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_16_get_paired_b: invoke');
      try {
        connection.getPairedDevices();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_16_get_paired_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_16_get_paired_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_16_GET_PAIRED_E2
     * @tc.name cn_16_get_paired_e2
     * @tc.desc Verify Bluetooth apitest case cn 16 get paired e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_16_get_paired_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_16_get_paired_e2: invoke');
      try {
        connection.getPairedDevices();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_16_get_paired_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_16_get_paired_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_16_GET_PAIRED_R
     * @tc.name cn_16_get_paired_r
     * @tc.desc Verify Bluetooth API cn 16 get paired return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_16_get_paired_r', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'ConnBleSocket', 'cn_16_get_paired_r: invoke');
      try {
        value = connection.getPairedDevices() as Array < Object > ;
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_16_get_paired_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_16_get_paired_r: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(Array.isArray(value)).assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_16_GET_PAIRED_X
     * @tc.name cn_16_get_paired_x
     * @tc.desc Verify Bluetooth API cn 16 get paired repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_16_get_paired_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_16_get_paired_x: invoke');
      try {
        connection.getPairedDevices();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_16_get_paired_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_16_get_paired_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_17_get_pair_state', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_17_GET_PAIR_STATE_N
     * @tc.name cn_17_get_pair_state_n
     * @tc.desc Verify Bluetooth API cn 17 get pair state normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_17_get_pair_state_n', 0, () => {
      let errCode = 0;
      let value = -1;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_n: invoke');
      try {
        value = connection.getPairState(BADDR);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_17_get_pair_state_n: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_17_GET_PAIR_STATE_E
     * @tc.name cn_17_get_pair_state_e
     * @tc.desc Verify Bluetooth API cn 17 get pair state error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_17_get_pair_state_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_e: invoke expect error');
      try {
        connection.getPairState('');
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_17_GET_PAIR_STATE_B
     * @tc.name cn_17_get_pair_state_b
     * @tc.desc Verify Bluetooth API cn 17 get pair state boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_17_get_pair_state_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_b: invoke');
      try {
        connection.getPairState(BADDR);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_17_GET_PAIR_STATE_E2
     * @tc.name cn_17_get_pair_state_e2
     * @tc.desc Verify Bluetooth apitest case cn 17 get pair state e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_17_get_pair_state_e2', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_e2: invoke expect error');
      try {
        connection.getPairState('');
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_e2: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_17_GET_PAIR_STATE_R
     * @tc.name cn_17_get_pair_state_r
     * @tc.desc Verify Bluetooth API cn 17 get pair state return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_17_get_pair_state_r', 0, () => {
      let errCode = 0;
      let value = -1;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_r: invoke');
      try {
        value = connection.getPairState(BADDR);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_17_get_pair_state_r: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_17_GET_PAIR_STATE_X
     * @tc.name cn_17_get_pair_state_x
     * @tc.desc Verify Bluetooth API cn 17 get pair state repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_17_get_pair_state_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_x: invoke');
      try {
        connection.getPairState(BADDR);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_17_get_pair_state_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_18_pair_confirm', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_18_PAIR_CONFIRM_N
     * @tc.name cn_18_pair_confirm_n
     * @tc.desc Verify Bluetooth API cn 18 pair confirm normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_18_pair_confirm_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_n: invoke');
      try {
        connection.setDevicePairingConfirmation(BADDR, true);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_18_PAIR_CONFIRM_E
     * @tc.name cn_18_pair_confirm_e
     * @tc.desc Verify Bluetooth API cn 18 pair confirm error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_18_pair_confirm_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_e: invoke expect error');
      try {
        connection.setDevicePairingConfirmation('', false);
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_18_PAIR_CONFIRM_B
     * @tc.name cn_18_pair_confirm_b
     * @tc.desc Verify Bluetooth API cn 18 pair confirm boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_18_pair_confirm_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_b: invoke');
      try {
        connection.setDevicePairingConfirmation(BADDR, false);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_18_PAIR_CONFIRM_E2
     * @tc.name cn_18_pair_confirm_e2
     * @tc.desc Verify Bluetooth apitest case cn 18 pair confirm e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_18_pair_confirm_e2', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_e2: invoke expect error');
      try {
        connection.setDevicePairingConfirmation('', true);
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_e2: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_18_PAIR_CONFIRM_R
     * @tc.name cn_18_pair_confirm_r
     * @tc.desc Verify Bluetooth API cn 18 pair confirm return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_18_pair_confirm_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_r: invoke');
      try {
        connection.setDevicePairingConfirmation(BADDR, true);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_18_PAIR_CONFIRM_X
     * @tc.name cn_18_pair_confirm_x
     * @tc.desc Verify Bluetooth API cn 18 pair confirm repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_18_pair_confirm_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_x: invoke');
      try {
        connection.setDevicePairingConfirmation(BADDR, false);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_18_pair_confirm_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_19_set_pin_cb', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_19_SET_PIN_CB_N
     * @tc.name cn_19_set_pin_cb_n
     * @tc.desc Verify Bluetooth API cn 19 set pin cb normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_19_set_pin_cb_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_n: invoke');
      try {
        connection.setDevicePinCode(BADDR, '0000', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_19_SET_PIN_CB_E
     * @tc.name cn_19_set_pin_cb_e
     * @tc.desc Verify Bluetooth API cn 19 set pin cb error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_19_set_pin_cb_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_e: invoke expect error');
      try {
        connection.setDevicePinCode('', '0000', () => {});
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_19_SET_PIN_CB_B
     * @tc.name cn_19_set_pin_cb_b
     * @tc.desc Verify Bluetooth API cn 19 set pin cb boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_19_set_pin_cb_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_b: invoke');
      try {
        connection.setDevicePinCode(BADDR, '1234', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_19_SET_PIN_CB_E2
     * @tc.name cn_19_set_pin_cb_e2
     * @tc.desc Verify Bluetooth apitest case cn 19 set pin cb e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_19_set_pin_cb_e2', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_e2: invoke expect error');
      try {
        connection.setDevicePinCode('', '00', () => {});
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_e2: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_19_SET_PIN_CB_R
     * @tc.name cn_19_set_pin_cb_r
     * @tc.desc Verify Bluetooth API cn 19 set pin cb return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_19_set_pin_cb_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_r: invoke');
      try {
        connection.setDevicePinCode(BADDR, '0000', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_19_SET_PIN_CB_X
     * @tc.name cn_19_set_pin_cb_x
     * @tc.desc Verify Bluetooth API cn 19 set pin cb repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_19_set_pin_cb_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_x: invoke');
      try {
        connection.setDevicePinCode(BADDR, '0000', () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_19_set_pin_cb_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_20_set_pin_p', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_20_SET_PIN_P_N
     * @tc.name cn_20_set_pin_p_n
     * @tc.desc Verify Bluetooth API cn 20 set pin p normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_20_set_pin_p_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_n: invoke');
      try {
        connection.setDevicePinCode(BADDR, '0000');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_20_SET_PIN_P_E
     * @tc.name cn_20_set_pin_p_e
     * @tc.desc Verify Bluetooth API cn 20 set pin p error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_20_set_pin_p_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_e: invoke expect error');
      try {
        connection.setDevicePinCode('', '0000');
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_20_SET_PIN_P_B
     * @tc.name cn_20_set_pin_p_b
     * @tc.desc Verify Bluetooth API cn 20 set pin p boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_20_set_pin_p_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_b: invoke');
      try {
        connection.setDevicePinCode(BADDR, '1234');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_20_SET_PIN_P_E2
     * @tc.name cn_20_set_pin_p_e2
     * @tc.desc Verify Bluetooth apitest case cn 20 set pin p e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_20_set_pin_p_e2', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_e2: invoke expect error');
      try {
        connection.setDevicePinCode('', '00');
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_e2: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_20_SET_PIN_P_R
     * @tc.name cn_20_set_pin_p_r
     * @tc.desc Verify Bluetooth API cn 20 set pin p return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_20_set_pin_p_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_r: invoke');
      try {
        connection.setDevicePinCode(BADDR, '0000');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_20_SET_PIN_P_X
     * @tc.name cn_20_set_pin_p_x
     * @tc.desc Verify Bluetooth API cn 20 set pin p repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_20_set_pin_p_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_x: invoke');
      try {
        connection.setDevicePinCode(BADDR, '0000');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_20_set_pin_p_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_21_set_local_name', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_21_SET_LOCAL_NAME_N
     * @tc.name cn_21_set_local_name_n
     * @tc.desc Verify Bluetooth API cn 21 set local name normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_21_set_local_name_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_n: invoke');
      try {
        connection.setLocalName('hypium');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_21_SET_LOCAL_NAME_E
     * @tc.name cn_21_set_local_name_e
     * @tc.desc Verify Bluetooth API cn 21 set local name error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_21_set_local_name_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_e: invoke expect error');
      try {
        connection.setLocalName('');
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_21_SET_LOCAL_NAME_B
     * @tc.name cn_21_set_local_name_b
     * @tc.desc Verify Bluetooth API cn 21 set local name boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_21_set_local_name_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_b: invoke');
      try {
        connection.setLocalName('a');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_21_SET_LOCAL_NAME_E2
     * @tc.name cn_21_set_local_name_e2
     * @tc.desc Verify Bluetooth apitest case cn 21 set local name e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_21_set_local_name_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_e2: invoke');
      try {
        connection.setLocalName('x');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_21_SET_LOCAL_NAME_R
     * @tc.name cn_21_set_local_name_r
     * @tc.desc Verify Bluetooth API cn 21 set local name return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_21_set_local_name_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_r: invoke');
      try {
        connection.setLocalName('hypium');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_21_SET_LOCAL_NAME_X
     * @tc.name cn_21_set_local_name_x
     * @tc.desc Verify Bluetooth API cn 21 set local name repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_21_set_local_name_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_x: invoke');
      try {
        connection.setLocalName('hypium2');
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_21_set_local_name_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_22_set_scan_mode', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_22_SET_SCAN_MODE_N
     * @tc.name cn_22_set_scan_mode_n
     * @tc.desc Verify Bluetooth API cn 22 set scan mode normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_22_set_scan_mode_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_n: invoke');
      try {
        connection.setBluetoothScanMode(4, 0);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_22_SET_SCAN_MODE_E
     * @tc.name cn_22_set_scan_mode_e
     * @tc.desc Verify Bluetooth API cn 22 set scan mode error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_22_set_scan_mode_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_e: invoke expect error');
      try {
        connection.setBluetoothScanMode(0, -1);
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_22_SET_SCAN_MODE_B
     * @tc.name cn_22_set_scan_mode_b
     * @tc.desc Verify Bluetooth API cn 22 set scan mode boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_22_set_scan_mode_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_b: invoke');
      try {
        connection.setBluetoothScanMode(1, 0);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_22_SET_SCAN_MODE_E2
     * @tc.name cn_22_set_scan_mode_e2
     * @tc.desc Verify Bluetooth apitest case cn 22 set scan mode e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_22_set_scan_mode_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_e2: invoke');
      try {
        connection.setBluetoothScanMode(5, 999999);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_22_SET_SCAN_MODE_R
     * @tc.name cn_22_set_scan_mode_r
     * @tc.desc Verify Bluetooth API cn 22 set scan mode return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_22_set_scan_mode_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_r: invoke');
      try {
        connection.setBluetoothScanMode(4, 0);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_22_SET_SCAN_MODE_X
     * @tc.name cn_22_set_scan_mode_x
     * @tc.desc Verify Bluetooth API cn 22 set scan mode repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_22_set_scan_mode_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_x: invoke');
      try {
        connection.setBluetoothScanMode(4, 1);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_22_set_scan_mode_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_23_get_scan_mode', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_23_GET_SCAN_MODE_N
     * @tc.name cn_23_get_scan_mode_n
     * @tc.desc Verify Bluetooth API cn 23 get scan mode normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_23_get_scan_mode_n', 0, () => {
      let errCode = 0;
      let value = -1;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_n: invoke');
      try {
        value = connection.getBluetoothScanMode();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_23_get_scan_mode_n: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_23_GET_SCAN_MODE_E
     * @tc.name cn_23_get_scan_mode_e
     * @tc.desc Verify Bluetooth API cn 23 get scan mode error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_23_get_scan_mode_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_e: invoke');
      try {
        connection.getBluetoothScanMode();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_23_GET_SCAN_MODE_B
     * @tc.name cn_23_get_scan_mode_b
     * @tc.desc Verify Bluetooth API cn 23 get scan mode boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_23_get_scan_mode_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_b: invoke');
      try {
        connection.getBluetoothScanMode();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_23_GET_SCAN_MODE_E2
     * @tc.name cn_23_get_scan_mode_e2
     * @tc.desc Verify Bluetooth apitest case cn 23 get scan mode e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_23_get_scan_mode_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_e2: invoke');
      try {
        connection.getBluetoothScanMode();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_23_GET_SCAN_MODE_R
     * @tc.name cn_23_get_scan_mode_r
     * @tc.desc Verify Bluetooth API cn 23 get scan mode return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_23_get_scan_mode_r', 0, () => {
      let errCode = 0;
      let value = -1;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_r: invoke');
      try {
        value = connection.getBluetoothScanMode();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_23_get_scan_mode_r: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(errCode !== 0 || typeof value === 'number').assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_23_GET_SCAN_MODE_X
     * @tc.name cn_23_get_scan_mode_x
     * @tc.desc Verify Bluetooth API cn 23 get scan mode repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_23_get_scan_mode_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_x: invoke');
      try {
        connection.getBluetoothScanMode();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_23_get_scan_mode_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_24_start_disc', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_24_START_DISC_N
     * @tc.name cn_24_start_disc_n
     * @tc.desc Verify Bluetooth API cn 24 start disc normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_24_start_disc_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_24_start_disc_n: invoke');
      try {
        connection.startBluetoothDiscovery();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_24_start_disc_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_24_start_disc_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_24_START_DISC_E
     * @tc.name cn_24_start_disc_e
     * @tc.desc Verify Bluetooth API cn 24 start disc error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_24_start_disc_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_24_start_disc_e: invoke');
      try {
        connection.startBluetoothDiscovery();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_24_start_disc_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_24_start_disc_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_24_START_DISC_B
     * @tc.name cn_24_start_disc_b
     * @tc.desc Verify Bluetooth API cn 24 start disc boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_24_start_disc_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_24_start_disc_b: invoke');
      try {
        connection.startBluetoothDiscovery();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_24_start_disc_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_24_start_disc_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_24_START_DISC_E2
     * @tc.name cn_24_start_disc_e2
     * @tc.desc Verify Bluetooth apitest case cn 24 start disc e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_24_start_disc_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_24_start_disc_e2: invoke');
      try {
        connection.startBluetoothDiscovery();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_24_start_disc_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_24_start_disc_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_24_START_DISC_R
     * @tc.name cn_24_start_disc_r
     * @tc.desc Verify Bluetooth API cn 24 start disc return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_24_start_disc_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_24_start_disc_r: invoke');
      try {
        connection.startBluetoothDiscovery();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_24_start_disc_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_24_start_disc_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_24_START_DISC_X
     * @tc.name cn_24_start_disc_x
     * @tc.desc Verify Bluetooth API cn 24 start disc repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_24_start_disc_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_24_start_disc_x: invoke');
      try {
        connection.startBluetoothDiscovery();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_24_start_disc_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_24_start_disc_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_25_stop_disc', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_25_STOP_DISC_N
     * @tc.name cn_25_stop_disc_n
     * @tc.desc Verify Bluetooth API cn 25 stop disc normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_25_stop_disc_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_n: invoke');
      try {
        connection.stopBluetoothDiscovery();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_25_STOP_DISC_E
     * @tc.name cn_25_stop_disc_e
     * @tc.desc Verify Bluetooth API cn 25 stop disc error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_25_stop_disc_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_e: invoke');
      try {
        connection.stopBluetoothDiscovery();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_25_STOP_DISC_B
     * @tc.name cn_25_stop_disc_b
     * @tc.desc Verify Bluetooth API cn 25 stop disc boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_25_stop_disc_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_b: invoke');
      try {
        connection.stopBluetoothDiscovery();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_25_STOP_DISC_E2
     * @tc.name cn_25_stop_disc_e2
     * @tc.desc Verify Bluetooth apitest case cn 25 stop disc e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_25_stop_disc_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_e2: invoke');
      try {
        connection.stopBluetoothDiscovery();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_25_STOP_DISC_R
     * @tc.name cn_25_stop_disc_r
     * @tc.desc Verify Bluetooth API cn 25 stop disc return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_25_stop_disc_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_r: invoke');
      try {
        connection.stopBluetoothDiscovery();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_25_STOP_DISC_X
     * @tc.name cn_25_stop_disc_x
     * @tc.desc Verify Bluetooth API cn 25 stop disc repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_25_stop_disc_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_x: invoke');
      try {
        connection.stopBluetoothDiscovery();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_25_stop_disc_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_26_is_discovering', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_26_IS_DISCOVERING_N
     * @tc.name cn_26_is_discovering_n
     * @tc.desc Verify Bluetooth API cn 26 is discovering normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_26_is_discovering_n', 0, () => {
      let errCode = 0;
      let value = false;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_n: invoke');
      try {
        value = connection.isBluetoothDiscovering();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_26_is_discovering_n: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(typeof value).assertEqual('boolean');
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_26_IS_DISCOVERING_E
     * @tc.name cn_26_is_discovering_e
     * @tc.desc Verify Bluetooth API cn 26 is discovering error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_26_is_discovering_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_e: invoke');
      try {
        connection.isBluetoothDiscovering();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_26_IS_DISCOVERING_B
     * @tc.name cn_26_is_discovering_b
     * @tc.desc Verify Bluetooth API cn 26 is discovering boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_26_is_discovering_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_b: invoke');
      try {
        connection.isBluetoothDiscovering();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_26_IS_DISCOVERING_E2
     * @tc.name cn_26_is_discovering_e2
     * @tc.desc Verify Bluetooth apitest case cn 26 is discovering e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_26_is_discovering_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_e2: invoke');
      try {
        connection.isBluetoothDiscovering();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_26_IS_DISCOVERING_R
     * @tc.name cn_26_is_discovering_r
     * @tc.desc Verify Bluetooth API cn 26 is discovering return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_26_is_discovering_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_r: invoke');
      try {
        connection.isBluetoothDiscovering();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_26_IS_DISCOVERING_X
     * @tc.name cn_26_is_discovering_x
     * @tc.desc Verify Bluetooth API cn 26 is discovering repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_26_is_discovering_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_x: invoke');
      try {
        connection.isBluetoothDiscovering();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_26_is_discovering_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_27_local_uuids_cb', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_27_LOCAL_UUIDS_CB_N
     * @tc.name cn_27_local_uuids_cb_n
     * @tc.desc Verify Bluetooth API cn 27 local uuids cb normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_27_local_uuids_cb_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_n: invoke');
      try {
        connection.getLocalProfileUuids(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_27_LOCAL_UUIDS_CB_E
     * @tc.name cn_27_local_uuids_cb_e
     * @tc.desc Verify Bluetooth API cn 27 local uuids cb error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_27_local_uuids_cb_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_e: invoke');
      try {
        connection.getLocalProfileUuids(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_27_LOCAL_UUIDS_CB_B
     * @tc.name cn_27_local_uuids_cb_b
     * @tc.desc Verify Bluetooth API cn 27 local uuids cb boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_27_local_uuids_cb_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_b: invoke');
      try {
        connection.getLocalProfileUuids(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_27_LOCAL_UUIDS_CB_E2
     * @tc.name cn_27_local_uuids_cb_e2
     * @tc.desc Verify Bluetooth apitest case cn 27 local uuids cb e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_27_local_uuids_cb_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_e2: invoke');
      try {
        connection.getLocalProfileUuids(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_27_LOCAL_UUIDS_CB_R
     * @tc.name cn_27_local_uuids_cb_r
     * @tc.desc Verify Bluetooth API cn 27 local uuids cb return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_27_local_uuids_cb_r', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_r: invoke');
      try {
        connection.getLocalProfileUuids(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_27_local_uuids_cb_r: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(Array.isArray(value)).assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_27_LOCAL_UUIDS_CB_X
     * @tc.name cn_27_local_uuids_cb_x
     * @tc.desc Verify Bluetooth API cn 27 local uuids cb repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_27_local_uuids_cb_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_x: invoke');
      try {
        connection.getLocalProfileUuids(() => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_27_local_uuids_cb_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_28_local_uuids_p', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_28_LOCAL_UUIDS_P_N
     * @tc.name cn_28_local_uuids_p_n
     * @tc.desc Verify Bluetooth API cn 28 local uuids p normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_28_local_uuids_p_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_n: invoke');
      try {
        connection.getLocalProfileUuids();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_28_LOCAL_UUIDS_P_E
     * @tc.name cn_28_local_uuids_p_e
     * @tc.desc Verify Bluetooth API cn 28 local uuids p error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_28_local_uuids_p_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_e: invoke');
      try {
        connection.getLocalProfileUuids();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_e: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_28_LOCAL_UUIDS_P_B
     * @tc.name cn_28_local_uuids_p_b
     * @tc.desc Verify Bluetooth API cn 28 local uuids p boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_28_local_uuids_p_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_b: invoke');
      try {
        connection.getLocalProfileUuids();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_28_LOCAL_UUIDS_P_E2
     * @tc.name cn_28_local_uuids_p_e2
     * @tc.desc Verify Bluetooth apitest case cn 28 local uuids p e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_28_local_uuids_p_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_e2: invoke');
      try {
        connection.getLocalProfileUuids();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_28_LOCAL_UUIDS_P_R
     * @tc.name cn_28_local_uuids_p_r
     * @tc.desc Verify Bluetooth API cn 28 local uuids p return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_28_local_uuids_p_r', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_r: invoke');
      try {
        connection.getLocalProfileUuids();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_28_local_uuids_p_r: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(Array.isArray(value)).assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_28_LOCAL_UUIDS_P_X
     * @tc.name cn_28_local_uuids_p_x
     * @tc.desc Verify Bluetooth API cn 28 local uuids p repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_28_local_uuids_p_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_x: invoke');
      try {
        connection.getLocalProfileUuids();
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_28_local_uuids_p_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('cn_29_remote_uuids_cb', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_29_REMOTE_UUIDS_CB_N
     * @tc.name cn_29_remote_uuids_cb_n
     * @tc.desc Verify Bluetooth API cn 29 remote uuids cb normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_29_remote_uuids_cb_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_n: invoke');
      try {
        connection.getRemoteProfileUuids(BADDR, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_29_REMOTE_UUIDS_CB_E
     * @tc.name cn_29_remote_uuids_cb_e
     * @tc.desc Verify Bluetooth API cn 29 remote uuids cb error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_29_remote_uuids_cb_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_e: invoke expect error');
      try {
        connection.getRemoteProfileUuids('', () => {});
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_29_REMOTE_UUIDS_CB_B
     * @tc.name cn_29_remote_uuids_cb_b
     * @tc.desc Verify Bluetooth API cn 29 remote uuids cb boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_29_remote_uuids_cb_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_b: invoke');
      try {
        connection.getRemoteProfileUuids(BADDR, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_29_REMOTE_UUIDS_CB_E2
     * @tc.name cn_29_remote_uuids_cb_e2
     * @tc.desc Verify Bluetooth apitest case cn 29 remote uuids cb e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_29_remote_uuids_cb_e2', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_e2: invoke expect error');
      try {
        connection.getRemoteProfileUuids('', () => {});
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_e2: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_29_REMOTE_UUIDS_CB_R
     * @tc.name cn_29_remote_uuids_cb_r
     * @tc.desc Verify Bluetooth API cn 29 remote uuids cb return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_29_remote_uuids_cb_r', 0, () => {
      let errCode = 0;
      let value: Array < Object >  = [];
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_r: invoke');
      try {
        connection.getRemoteProfileUuids(BADDR, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 
        'cn_29_remote_uuids_cb_r: errCode=' + errCode + ' value=' + JSON.stringify(value));
      assertPeerExpect(errCode);
      expect(Array.isArray(value)).assertTrue();
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_CN_29_REMOTE_UUIDS_CB_X
     * @tc.name cn_29_remote_uuids_cb_x
     * @tc.desc Verify Bluetooth API cn 29 remote uuids cb repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('cn_29_remote_uuids_cb_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_x: invoke');
      try {
        connection.getRemoteProfileUuids(BADDR, () => {});
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'cn_29_remote_uuids_cb_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
  });
}



