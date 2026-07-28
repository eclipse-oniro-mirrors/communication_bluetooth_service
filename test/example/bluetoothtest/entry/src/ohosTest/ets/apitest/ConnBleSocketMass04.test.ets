      hilog.info(0x0000, 'ConnBleSocket', 'sk_12_off_spp_read_x: invoke');
      try {
        socket.off('sppRead', 0);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'sk_12_off_spp_read_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'sk_12_off_spp_read_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('sk_13_write_async', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_13_WRITE_ASYNC_N
     * @tc.name sk_13_write_async_n
     * @tc.desc Verify Bluetooth API sk 13 write async normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_13_write_async_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'sk_13_write_async_n: invoke');
      try {
        socket.sppWriteAsync(0, buf0());
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'sk_13_write_async_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'sk_13_write_async_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_13_WRITE_ASYNC_E
     * @tc.name sk_13_write_async_e
     * @tc.desc Verify Bluetooth API sk 13 write async error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_13_write_async_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'sk_13_write_async_e: invoke expect error');
      try {
        socket.sppWriteAsync(-1, buf0());
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'sk_13_write_async_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'sk_13_write_async_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_13_WRITE_ASYNC_B
     * @tc.name sk_13_write_async_b
     * @tc.desc Verify Bluetooth API sk 13 write async boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_13_write_async_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'sk_13_write_async_b: invoke');
      try {
        socket.sppWriteAsync(0, buf0());
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'sk_13_write_async_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'sk_13_write_async_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_13_WRITE_ASYNC_E2
     * @tc.name sk_13_write_async_e2
     * @tc.desc Verify Bluetooth apitest case sk 13 write async e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_13_write_async_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'sk_13_write_async_e2: invoke');
      try {
        socket.sppWriteAsync(0, buf0());
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'sk_13_write_async_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'sk_13_write_async_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_13_WRITE_ASYNC_R
     * @tc.name sk_13_write_async_r
     * @tc.desc Verify Bluetooth API sk 13 write async return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_13_write_async_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'sk_13_write_async_r: invoke');
      try {
        socket.sppWriteAsync(0, buf0());
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'sk_13_write_async_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'sk_13_write_async_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_13_WRITE_ASYNC_X
     * @tc.name sk_13_write_async_x
     * @tc.desc Verify Bluetooth API sk 13 write async repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_13_write_async_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'sk_13_write_async_x: invoke');
      try {
        socket.sppWriteAsync(0, buf0());
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'sk_13_write_async_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'sk_13_write_async_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
    describe('sk_14_read_async', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_14_READ_ASYNC_N
     * @tc.name sk_14_read_async_n
     * @tc.desc Verify Bluetooth API sk 14 read async normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_14_read_async_n', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'sk_14_read_async_n: invoke');
      try {
        socket.sppReadAsync(0);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'sk_14_read_async_n: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'sk_14_read_async_n: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_14_READ_ASYNC_E
     * @tc.name sk_14_read_async_e
     * @tc.desc Verify Bluetooth API sk 14 read async error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_14_read_async_e', 0, () => {
      let threw = false;
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'sk_14_read_async_e: invoke expect error');
      try {
        socket.sppReadAsync(-1);
      } catch (err) {
        threw = true;
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'sk_14_read_async_e: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'sk_14_read_async_e: threw=' + threw + ' errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_14_READ_ASYNC_B
     * @tc.name sk_14_read_async_b
     * @tc.desc Verify Bluetooth API sk 14 read async boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_14_read_async_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'sk_14_read_async_b: invoke');
      try {
        socket.sppReadAsync(1);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'sk_14_read_async_b: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'sk_14_read_async_b: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_14_READ_ASYNC_E2
     * @tc.name sk_14_read_async_e2
     * @tc.desc Verify Bluetooth apitest case sk 14 read async e2
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_14_read_async_e2', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'sk_14_read_async_e2: invoke');
      try {
        socket.sppReadAsync(0);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'sk_14_read_async_e2: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'sk_14_read_async_e2: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_14_READ_ASYNC_R
     * @tc.name sk_14_read_async_r
     * @tc.desc Verify Bluetooth API sk 14 read async return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_14_read_async_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'sk_14_read_async_r: invoke');
      try {
        socket.sppReadAsync(0);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'sk_14_read_async_r: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'sk_14_read_async_r: errCode=' + errCode);
      assertPeerExpect(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_SK_14_READ_ASYNC_X
     * @tc.name sk_14_read_async_x
     * @tc.desc Verify Bluetooth API sk 14 read async repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('sk_14_read_async_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'ConnBleSocket', 'sk_14_read_async_x: invoke');
      try {
        socket.sppReadAsync(0);
      } catch (err) {
        const _c = (err as BusinessError).code;
        errCode = (_c === undefined || _c === null) ? 0 : Number(_c);
        if (isNaN(errCode)) { errCode = 0; }
        hilog.error(0x0000, 'ConnBleSocket', 'sk_14_read_async_x: ' + JSON.stringify(err));
      }
      hilog.info(0x0000, 'ConnBleSocket', 'sk_14_read_async_x: errCode=' + errCode);
      assertPeerExpect(errCode);
    });
    });
  });
}



