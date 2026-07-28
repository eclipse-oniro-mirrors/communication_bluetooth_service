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


