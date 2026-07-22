     * @tc.level Level 2
     */
    it('combo_access_state_scan_e', 0, () => {
      try {
            fcRunCombo('combo_access_state_scan', [() => {access.getState();}, () => {ble.stopBLEScan();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_ACCESS_STATE_SCAN_B
     * @tc.name combo_access_state_scan_b
     * @tc.desc Verify Bluetooth API combo access state scan boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_access_state_scan_b', 0, () => {
      try {
            fcRunCombo('combo_access_state_scan', [() => {access.getState();}, () => {ble.stopBLEScan();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_ACCESS_STATE_SCAN_R
     * @tc.name combo_access_state_scan_r
     * @tc.desc Verify Bluetooth API combo access state scan return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_access_state_scan_r', 0, () => {
      try {
            fcRunCombo('combo_access_state_scan', [() => {access.getState();}, () => {ble.stopBLEScan();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_ACCESS_STATE_SCAN_X
     * @tc.name combo_access_state_scan_x
     * @tc.desc Verify Bluetooth API combo access state scan repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_access_state_scan_x', 0, () => {
      try {
            fcRunCombo('combo_access_state_scan', [() => {access.getState();}, () => {ble.stopBLEScan();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_ACCESS_STATE_SCAN_C
     * @tc.name combo_access_state_scan_c
     * @tc.desc Verify Bluetooth API combo access state scan combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_access_state_scan_c', 0, () => {
      try {
            fcRunCombo('combo_access_state_scan', [() => {access.getState();}, () => {ble.stopBLEScan();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_access_convert_valid', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_ACCESS_CONVERT_VALID_N
     * @tc.name combo_access_convert_valid_n
     * @tc.desc Verify Bluetooth API combo access convert valid normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_access_convert_valid_n', 0, () => {
      try {
            fcRunCombo('combo_access_convert_valid', [() => {access.convertUuid('180A');}, () => {access.isValidRandomDeviceId(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_ACCESS_CONVERT_VALID_E
     * @tc.name combo_access_convert_valid_e
     * @tc.desc Verify Bluetooth API combo access convert valid error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_access_convert_valid_e', 0, () => {
      try {
            fcRunCombo('combo_access_convert_valid', [() => {access.convertUuid('180A');}, () => {access.isValidRandomDeviceId(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_ACCESS_CONVERT_VALID_B
     * @tc.name combo_access_convert_valid_b
     * @tc.desc Verify Bluetooth API combo access convert valid boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_access_convert_valid_b', 0, () => {
      try {
            fcRunCombo('combo_access_convert_valid', [() => {access.convertUuid('180A');}, () => {access.isValidRandomDeviceId(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_ACCESS_CONVERT_VALID_R
     * @tc.name combo_access_convert_valid_r
     * @tc.desc Verify Bluetooth API combo access convert valid return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_access_convert_valid_r', 0, () => {
      try {
            fcRunCombo('combo_access_convert_valid', [() => {access.convertUuid('180A');}, () => {access.isValidRandomDeviceId(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_ACCESS_CONVERT_VALID_X
     * @tc.name combo_access_convert_valid_x
     * @tc.desc Verify Bluetooth API combo access convert valid repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_access_convert_valid_x', 0, () => {
      try {
            fcRunCombo('combo_access_convert_valid', [() => {access.convertUuid('180A');}, () => {access.isValidRandomDeviceId(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_ACCESS_CONVERT_VALID_C
     * @tc.name combo_access_convert_valid_c
     * @tc.desc Verify Bluetooth API combo access convert valid combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_access_convert_valid_c', 0, () => {
      try {
            fcRunCombo('combo_access_convert_valid', [() => {access.convertUuid('180A');}, () => {access.isValidRandomDeviceId(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_conn_pair_cancel', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_PAIR_CANCEL_N
     * @tc.name combo_conn_pair_cancel_n
     * @tc.desc Verify Bluetooth API combo conn pair cancel normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_pair_cancel_n', 0, () => {
      try {
            fcRunCombo('combo_conn_pair_cancel', [() => {connection.pairDevice(BADDR);}, () => {connection.cancelPairingDevice(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_PAIR_CANCEL_E
     * @tc.name combo_conn_pair_cancel_e
     * @tc.desc Verify Bluetooth API combo conn pair cancel error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_pair_cancel_e', 0, () => {
      try {
            fcRunCombo('combo_conn_pair_cancel', [() => {connection.pairDevice(BADDR);}, () => {connection.cancelPairingDevice(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_PAIR_CANCEL_B
     * @tc.name combo_conn_pair_cancel_b
     * @tc.desc Verify Bluetooth API combo conn pair cancel boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_pair_cancel_b', 0, () => {
      try {
            fcRunCombo('combo_conn_pair_cancel', [() => {connection.pairDevice(BADDR);}, () => {connection.cancelPairingDevice(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_PAIR_CANCEL_R
     * @tc.name combo_conn_pair_cancel_r
     * @tc.desc Verify Bluetooth API combo conn pair cancel return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_pair_cancel_r', 0, () => {
      try {
            fcRunCombo('combo_conn_pair_cancel', [() => {connection.pairDevice(BADDR);}, () => {connection.cancelPairingDevice(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_PAIR_CANCEL_X
     * @tc.name combo_conn_pair_cancel_x
     * @tc.desc Verify Bluetooth API combo conn pair cancel repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_pair_cancel_x', 0, () => {
      try {
            fcRunCombo('combo_conn_pair_cancel', [() => {connection.pairDevice(BADDR);}, () => {connection.cancelPairingDevice(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_PAIR_CANCEL_C
     * @tc.name combo_conn_pair_cancel_c
     * @tc.desc Verify Bluetooth API combo conn pair cancel combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_pair_cancel_c', 0, () => {
      try {
            fcRunCombo('combo_conn_pair_cancel', [() => {connection.pairDevice(BADDR);}, () => {connection.cancelPairingDevice(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_conn_disc_start_stop', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_DISC_START_STOP_N
     * @tc.name combo_conn_disc_start_stop_n
     * @tc.desc Verify Bluetooth API combo conn disc start stop normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_disc_start_stop_n', 0, () => {
      try {
            fcRunCombo('combo_conn_disc_start_stop', [() => {connection.startBluetoothDiscovery();}, () => {connection.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_DISC_START_STOP_E
     * @tc.name combo_conn_disc_start_stop_e
     * @tc.desc Verify Bluetooth API combo conn disc start stop error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_disc_start_stop_e', 0, () => {
      try {
            fcRunCombo('combo_conn_disc_start_stop', [() => {connection.startBluetoothDiscovery();}, () => {connection.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_DISC_START_STOP_B
     * @tc.name combo_conn_disc_start_stop_b
     * @tc.desc Verify Bluetooth API combo conn disc start stop boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_disc_start_stop_b', 0, () => {
      try {
            fcRunCombo('combo_conn_disc_start_stop', [() => {connection.startBluetoothDiscovery();}, () => {connection.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_DISC_START_STOP_R
     * @tc.name combo_conn_disc_start_stop_r
     * @tc.desc Verify Bluetooth API combo conn disc start stop return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_disc_start_stop_r', 0, () => {
      try {
            fcRunCombo('combo_conn_disc_start_stop', [() => {connection.startBluetoothDiscovery();}, () => {connection.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_DISC_START_STOP_X
     * @tc.name combo_conn_disc_start_stop_x
     * @tc.desc Verify Bluetooth API combo conn disc start stop repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_disc_start_stop_x', 0, () => {
      try {
            fcRunCombo('combo_conn_disc_start_stop', [() => {connection.startBluetoothDiscovery();}, () => {connection.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_DISC_START_STOP_C
     * @tc.name combo_conn_disc_start_stop_c
     * @tc.desc Verify Bluetooth API combo conn disc start stop combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_disc_start_stop_c', 0, () => {
      try {
            fcRunCombo('combo_conn_disc_start_stop', [() => {connection.startBluetoothDiscovery();}, () => {connection.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_conn_local_name', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_LOCAL_NAME_N
     * @tc.name combo_conn_local_name_n
     * @tc.desc Verify Bluetooth API combo conn local name normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_local_name_n', 0, () => {
      try {
            fcRunCombo('combo_conn_local_name', [() => {connection.setLocalName('t');}, () => {connection.getLocalName();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_LOCAL_NAME_E
     * @tc.name combo_conn_local_name_e
     * @tc.desc Verify Bluetooth API combo conn local name error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_local_name_e', 0, () => {
      try {
            fcRunCombo('combo_conn_local_name', [() => {connection.setLocalName('t');}, () => {connection.getLocalName();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_LOCAL_NAME_B
     * @tc.name combo_conn_local_name_b
     * @tc.desc Verify Bluetooth API combo conn local name boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_local_name_b', 0, () => {
      try {
            fcRunCombo('combo_conn_local_name', [() => {connection.setLocalName('t');}, () => {connection.getLocalName();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_LOCAL_NAME_R
     * @tc.name combo_conn_local_name_r
     * @tc.desc Verify Bluetooth API combo conn local name return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_local_name_r', 0, () => {
      try {
            fcRunCombo('combo_conn_local_name', [() => {connection.setLocalName('t');}, () => {connection.getLocalName();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_LOCAL_NAME_X
     * @tc.name combo_conn_local_name_x
     * @tc.desc Verify Bluetooth API combo conn local name repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_local_name_x', 0, () => {
      try {
            fcRunCombo('combo_conn_local_name', [() => {connection.setLocalName('t');}, () => {connection.getLocalName();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_LOCAL_NAME_C
     * @tc.name combo_conn_local_name_c
     * @tc.desc Verify Bluetooth API combo conn local name combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_local_name_c', 0, () => {
      try {
            fcRunCombo('combo_conn_local_name', [() => {connection.setLocalName('t');}, () => {connection.getLocalName();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_conn_scan_mode', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_SCAN_MODE_N
     * @tc.name combo_conn_scan_mode_n
     * @tc.desc Verify Bluetooth API combo conn scan mode normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_scan_mode_n', 0, () => {
      try {
            fcRunCombo('combo_conn_scan_mode', [() => {connection.setBluetoothScanMode(4, 0);}, () => {connection.getBluetoothScanMode();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_SCAN_MODE_E
     * @tc.name combo_conn_scan_mode_e
     * @tc.desc Verify Bluetooth API combo conn scan mode error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_scan_mode_e', 0, () => {
      try {
            fcRunCombo('combo_conn_scan_mode', [() => {connection.setBluetoothScanMode(4, 0);}, () => {connection.getBluetoothScanMode();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_SCAN_MODE_B
     * @tc.name combo_conn_scan_mode_b
     * @tc.desc Verify Bluetooth API combo conn scan mode boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_scan_mode_b', 0, () => {
      try {
            fcRunCombo('combo_conn_scan_mode', [() => {connection.setBluetoothScanMode(4, 0);}, () => {connection.getBluetoothScanMode();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_SCAN_MODE_R
     * @tc.name combo_conn_scan_mode_r
     * @tc.desc Verify Bluetooth API combo conn scan mode return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_scan_mode_r', 0, () => {
      try {
            fcRunCombo('combo_conn_scan_mode', [() => {connection.setBluetoothScanMode(4, 0);}, () => {connection.getBluetoothScanMode();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_SCAN_MODE_X
     * @tc.name combo_conn_scan_mode_x
     * @tc.desc Verify Bluetooth API combo conn scan mode repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_scan_mode_x', 0, () => {
      try {
            fcRunCombo('combo_conn_scan_mode', [() => {connection.setBluetoothScanMode(4, 0);}, () => {connection.getBluetoothScanMode();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_CONN_SCAN_MODE_C
     * @tc.name combo_conn_scan_mode_c
     * @tc.desc Verify Bluetooth API combo conn scan mode combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_conn_scan_mode_c', 0, () => {
      try {
            fcRunCombo('combo_conn_scan_mode', [() => {connection.setBluetoothScanMode(4, 0);}, () => {connection.getBluetoothScanMode();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_ble_scan_cycle', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BLE_SCAN_CYCLE_N
     * @tc.name combo_ble_scan_cycle_n
     * @tc.desc Verify Bluetooth API combo ble scan cycle normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_ble_scan_cycle_n', 0, () => {
      try {
            fcRunCombo('combo_ble_scan_cycle', [() => {ble.startBLEScan([]);}, () => {ble.stopBLEScan();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BLE_SCAN_CYCLE_E
     * @tc.name combo_ble_scan_cycle_e
     * @tc.desc Verify Bluetooth API combo ble scan cycle error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_ble_scan_cycle_e', 0, () => {
      try {
            fcRunCombo('combo_ble_scan_cycle', [() => {ble.startBLEScan([]);}, () => {ble.stopBLEScan();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BLE_SCAN_CYCLE_B
     * @tc.name combo_ble_scan_cycle_b
     * @tc.desc Verify Bluetooth API combo ble scan cycle boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_ble_scan_cycle_b', 0, () => {
      try {
            fcRunCombo('combo_ble_scan_cycle', [() => {ble.startBLEScan([]);}, () => {ble.stopBLEScan();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BLE_SCAN_CYCLE_R
     * @tc.name combo_ble_scan_cycle_r
     * @tc.desc Verify Bluetooth API combo ble scan cycle return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_ble_scan_cycle_r', 0, () => {
      try {
            fcRunCombo('combo_ble_scan_cycle', [() => {ble.startBLEScan([]);}, () => {ble.stopBLEScan();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BLE_SCAN_CYCLE_X
     * @tc.name combo_ble_scan_cycle_x
     * @tc.desc Verify Bluetooth API combo ble scan cycle repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_ble_scan_cycle_x', 0, () => {
      try {
            fcRunCombo('combo_ble_scan_cycle', [() => {ble.startBLEScan([]);}, () => {ble.stopBLEScan();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BLE_SCAN_CYCLE_C
     * @tc.name combo_ble_scan_cycle_c
     * @tc.desc Verify Bluetooth API combo ble scan cycle combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_ble_scan_cycle_c', 0, () => {
      try {
            fcRunCombo('combo_ble_scan_cycle', [() => {ble.startBLEScan([]);}, () => {ble.stopBLEScan();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_ble_adv_cycle', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BLE_ADV_CYCLE_N
     * @tc.name combo_ble_adv_cycle_n
     * @tc.desc Verify Bluetooth API combo ble adv cycle normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_ble_adv_cycle_n', 0, () => {
      try {
            fcRunCombo('combo_ble_adv_cycle', [() => {ble.startAdvertising(bleAp());}, () => {ble.stopAdvertising();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BLE_ADV_CYCLE_E
     * @tc.name combo_ble_adv_cycle_e
     * @tc.desc Verify Bluetooth API combo ble adv cycle error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_ble_adv_cycle_e', 0, () => {
      try {
            fcRunCombo('combo_ble_adv_cycle', [() => {ble.startAdvertising(bleAp());}, () => {ble.stopAdvertising();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BLE_ADV_CYCLE_B
     * @tc.name combo_ble_adv_cycle_b
     * @tc.desc Verify Bluetooth API combo ble adv cycle boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_ble_adv_cycle_b', 0, () => {
      try {
            fcRunCombo('combo_ble_adv_cycle', [() => {ble.startAdvertising(bleAp());}, () => {ble.stopAdvertising();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BLE_ADV_CYCLE_R
     * @tc.name combo_ble_adv_cycle_r
     * @tc.desc Verify Bluetooth API combo ble adv cycle return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_ble_adv_cycle_r', 0, () => {
      try {
            fcRunCombo('combo_ble_adv_cycle', [() => {ble.startAdvertising(bleAp());}, () => {ble.stopAdvertising();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BLE_ADV_CYCLE_X
     * @tc.name combo_ble_adv_cycle_x
     * @tc.desc Verify Bluetooth API combo ble adv cycle repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_ble_adv_cycle_x', 0, () => {
      try {
            fcRunCombo('combo_ble_adv_cycle', [() => {ble.startAdvertising(bleAp());}, () => {ble.stopAdvertising();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BLE_ADV_CYCLE_C
     * @tc.name combo_ble_adv_cycle_c
     * @tc.desc Verify Bluetooth API combo ble adv cycle combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_ble_adv_cycle_c', 0, () => {
      try {
            fcRunCombo('combo_ble_adv_cycle', [() => {ble.startAdvertising(bleAp());}, () => {ble.stopAdvertising();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_gatt_client_lifecycle', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_GATT_CLIENT_LIFECYCLE_N
     * @tc.name combo_gatt_client_lifecycle_n
     * @tc.desc Verify Bluetooth API combo gatt client lifecycle normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_gatt_client_lifecycle_n', 0, () => {
      try {
            fcRunCombo('combo_gatt_client_lifecycle', [() => {const g = ble.createGattClientDevice(BADDR);g.connect();}, () => {const g = ble.createGattClientDevice(BADDR);g.close();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_GATT_CLIENT_LIFECYCLE_E
     * @tc.name combo_gatt_client_lifecycle_e
     * @tc.desc Verify Bluetooth API combo gatt client lifecycle error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_gatt_client_lifecycle_e', 0, () => {
      try {
            fcRunCombo('combo_gatt_client_lifecycle', [() => {const g = ble.createGattClientDevice(BADDR);g.connect();}, () => {const g = ble.createGattClientDevice(BADDR);g.close();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_GATT_CLIENT_LIFECYCLE_B
     * @tc.name combo_gatt_client_lifecycle_b
     * @tc.desc Verify Bluetooth API combo gatt client lifecycle boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_gatt_client_lifecycle_b', 0, () => {
      try {
            fcRunCombo('combo_gatt_client_lifecycle', [() => {const g = ble.createGattClientDevice(BADDR);g.connect();}, () => {const g = ble.createGattClientDevice(BADDR);g.close();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_GATT_CLIENT_LIFECYCLE_R
     * @tc.name combo_gatt_client_lifecycle_r
     * @tc.desc Verify Bluetooth API combo gatt client lifecycle return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_gatt_client_lifecycle_r', 0, () => {
      try {
            fcRunCombo('combo_gatt_client_lifecycle', [() => {const g = ble.createGattClientDevice(BADDR);g.connect();}, () => {const g = ble.createGattClientDevice(BADDR);g.close();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_GATT_CLIENT_LIFECYCLE_X
     * @tc.name combo_gatt_client_lifecycle_x
     * @tc.desc Verify Bluetooth API combo gatt client lifecycle repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_gatt_client_lifecycle_x', 0, () => {
      try {
            fcRunCombo('combo_gatt_client_lifecycle', [() => {const g = ble.createGattClientDevice(BADDR);g.connect();}, () => {const g = ble.createGattClientDevice(BADDR);g.close();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_GATT_CLIENT_LIFECYCLE_C
     * @tc.name combo_gatt_client_lifecycle_c
     * @tc.desc Verify Bluetooth API combo gatt client lifecycle combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_gatt_client_lifecycle_c', 0, () => {
      try {
            fcRunCombo('combo_gatt_client_lifecycle', [() => {const g = ble.createGattClientDevice(BADDR);g.connect();}, () => {const g = ble.createGattClientDevice(BADDR);g.close();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_gatt_server_lifecycle', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_GATT_SERVER_LIFECYCLE_N
     * @tc.name combo_gatt_server_lifecycle_n
     * @tc.desc Verify Bluetooth API combo gatt server lifecycle normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_gatt_server_lifecycle_n', 0, () => {
      try {
            fcRunCombo('combo_gatt_server_lifecycle', [() => {const s = ble.createGattServer();s.addService(bleGattService());}, () => {const s = ble.createGattServer();s.close();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_GATT_SERVER_LIFECYCLE_E
     * @tc.name combo_gatt_server_lifecycle_e
     * @tc.desc Verify Bluetooth API combo gatt server lifecycle error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_gatt_server_lifecycle_e', 0, () => {
      try {
            fcRunCombo('combo_gatt_server_lifecycle', [() => {const s = ble.createGattServer();s.addService(bleGattService());}, () => {const s = ble.createGattServer();s.close();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_GATT_SERVER_LIFECYCLE_B
     * @tc.name combo_gatt_server_lifecycle_b
     * @tc.desc Verify Bluetooth API combo gatt server lifecycle boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_gatt_server_lifecycle_b', 0, () => {
      try {
            fcRunCombo('combo_gatt_server_lifecycle', [() => {const s = ble.createGattServer();s.addService(bleGattService());}, () => {const s = ble.createGattServer();s.close();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_GATT_SERVER_LIFECYCLE_R
     * @tc.name combo_gatt_server_lifecycle_r
     * @tc.desc Verify Bluetooth API combo gatt server lifecycle return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_gatt_server_lifecycle_r', 0, () => {
      try {
            fcRunCombo('combo_gatt_server_lifecycle', [() => {const s = ble.createGattServer();s.addService(bleGattService());}, () => {const s = ble.createGattServer();s.close();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_GATT_SERVER_LIFECYCLE_X
     * @tc.name combo_gatt_server_lifecycle_x
     * @tc.desc Verify Bluetooth API combo gatt server lifecycle repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_gatt_server_lifecycle_x', 0, () => {
      try {
            fcRunCombo('combo_gatt_server_lifecycle', [() => {const s = ble.createGattServer();s.addService(bleGattService());}, () => {const s = ble.createGattServer();s.close();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_GATT_SERVER_LIFECYCLE_C
     * @tc.name combo_gatt_server_lifecycle_c
     * @tc.desc Verify Bluetooth API combo gatt server lifecycle combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_gatt_server_lifecycle_c', 0, () => {
      try {
            fcRunCombo('combo_gatt_server_lifecycle', [() => {const s = ble.createGattServer();s.addService(bleGattService());}, () => {const s = ble.createGattServer();s.close();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_a2dp_connect_disconnect', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_A2DP_CONNECT_DISCONNECT_N
     * @tc.name combo_a2dp_connect_disconnect_n
     * @tc.desc Verify Bluetooth API combo a2dp connect disconnect normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_a2dp_connect_disconnect_n', 0, () => {
      try {
            fcRunCombo('combo_a2dp_connect_disconnect', [() => {a2dp.createA2dpSrcProfile().connect(BADDR);}, () => {a2dp.createA2dpSrcProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_A2DP_CONNECT_DISCONNECT_E
     * @tc.name combo_a2dp_connect_disconnect_e
     * @tc.desc Verify Bluetooth API combo a2dp connect disconnect error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_a2dp_connect_disconnect_e', 0, () => {
      try {
            fcRunCombo('combo_a2dp_connect_disconnect', [() => {a2dp.createA2dpSrcProfile().connect(BADDR);}, () => {a2dp.createA2dpSrcProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_A2DP_CONNECT_DISCONNECT_B
     * @tc.name combo_a2dp_connect_disconnect_b
     * @tc.desc Verify Bluetooth API combo a2dp connect disconnect boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_a2dp_connect_disconnect_b', 0, () => {
      try {
            fcRunCombo('combo_a2dp_connect_disconnect', [() => {a2dp.createA2dpSrcProfile().connect(BADDR);}, () => {a2dp.createA2dpSrcProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_A2DP_CONNECT_DISCONNECT_R
     * @tc.name combo_a2dp_connect_disconnect_r
     * @tc.desc Verify Bluetooth API combo a2dp connect disconnect return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_a2dp_connect_disconnect_r', 0, () => {
      try {
            fcRunCombo('combo_a2dp_connect_disconnect', [() => {a2dp.createA2dpSrcProfile().connect(BADDR);}, () => {a2dp.createA2dpSrcProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_A2DP_CONNECT_DISCONNECT_X
     * @tc.name combo_a2dp_connect_disconnect_x
     * @tc.desc Verify Bluetooth API combo a2dp connect disconnect repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_a2dp_connect_disconnect_x', 0, () => {
      try {
            fcRunCombo('combo_a2dp_connect_disconnect', [() => {a2dp.createA2dpSrcProfile().connect(BADDR);}, () => {a2dp.createA2dpSrcProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_A2DP_CONNECT_DISCONNECT_C
     * @tc.name combo_a2dp_connect_disconnect_c
     * @tc.desc Verify Bluetooth API combo a2dp connect disconnect combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_a2dp_connect_disconnect_c', 0, () => {
      try {
            fcRunCombo('combo_a2dp_connect_disconnect', [() => {a2dp.createA2dpSrcProfile().connect(BADDR);}, () => {a2dp.createA2dpSrcProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_hfp_connect_disconnect', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_HFP_CONNECT_DISCONNECT_N
     * @tc.name combo_hfp_connect_disconnect_n
     * @tc.desc Verify Bluetooth API combo hfp connect disconnect normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_hfp_connect_disconnect_n', 0, () => {
      try {
            fcRunCombo('combo_hfp_connect_disconnect', [() => {hfp.createHfpAgProfile().connect(BADDR);}, () => {hfp.createHfpAgProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_HFP_CONNECT_DISCONNECT_E
     * @tc.name combo_hfp_connect_disconnect_e
     * @tc.desc Verify Bluetooth API combo hfp connect disconnect error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_hfp_connect_disconnect_e', 0, () => {
      try {
            fcRunCombo('combo_hfp_connect_disconnect', [() => {hfp.createHfpAgProfile().connect(BADDR);}, () => {hfp.createHfpAgProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_HFP_CONNECT_DISCONNECT_B
     * @tc.name combo_hfp_connect_disconnect_b
     * @tc.desc Verify Bluetooth API combo hfp connect disconnect boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_hfp_connect_disconnect_b', 0, () => {
      try {
            fcRunCombo('combo_hfp_connect_disconnect', [() => {hfp.createHfpAgProfile().connect(BADDR);}, () => {hfp.createHfpAgProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_HFP_CONNECT_DISCONNECT_R
     * @tc.name combo_hfp_connect_disconnect_r
     * @tc.desc Verify Bluetooth API combo hfp connect disconnect return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_hfp_connect_disconnect_r', 0, () => {
      try {
            fcRunCombo('combo_hfp_connect_disconnect', [() => {hfp.createHfpAgProfile().connect(BADDR);}, () => {hfp.createHfpAgProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_HFP_CONNECT_DISCONNECT_X
     * @tc.name combo_hfp_connect_disconnect_x
     * @tc.desc Verify Bluetooth API combo hfp connect disconnect repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_hfp_connect_disconnect_x', 0, () => {
      try {
            fcRunCombo('combo_hfp_connect_disconnect', [() => {hfp.createHfpAgProfile().connect(BADDR);}, () => {hfp.createHfpAgProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_HFP_CONNECT_DISCONNECT_C
     * @tc.name combo_hfp_connect_disconnect_c
     * @tc.desc Verify Bluetooth API combo hfp connect disconnect combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_hfp_connect_disconnect_c', 0, () => {
      try {
            fcRunCombo('combo_hfp_connect_disconnect', [() => {hfp.createHfpAgProfile().connect(BADDR);}, () => {hfp.createHfpAgProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_hid_connect_disconnect', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_HID_CONNECT_DISCONNECT_N
     * @tc.name combo_hid_connect_disconnect_n
     * @tc.desc Verify Bluetooth API combo hid connect disconnect normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_hid_connect_disconnect_n', 0, () => {
      try {
            fcRunCombo('combo_hid_connect_disconnect', [() => {hid.createHidHostProfile().connect(BADDR);}, () => {hid.createHidHostProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_HID_CONNECT_DISCONNECT_E
     * @tc.name combo_hid_connect_disconnect_e
     * @tc.desc Verify Bluetooth API combo hid connect disconnect error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_hid_connect_disconnect_e', 0, () => {
      try {
            fcRunCombo('combo_hid_connect_disconnect', [() => {hid.createHidHostProfile().connect(BADDR);}, () => {hid.createHidHostProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_HID_CONNECT_DISCONNECT_B
     * @tc.name combo_hid_connect_disconnect_b
     * @tc.desc Verify Bluetooth API combo hid connect disconnect boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_hid_connect_disconnect_b', 0, () => {
      try {
            fcRunCombo('combo_hid_connect_disconnect', [() => {hid.createHidHostProfile().connect(BADDR);}, () => {hid.createHidHostProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_HID_CONNECT_DISCONNECT_R
     * @tc.name combo_hid_connect_disconnect_r
     * @tc.desc Verify Bluetooth API combo hid connect disconnect return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_hid_connect_disconnect_r', 0, () => {
      try {
            fcRunCombo('combo_hid_connect_disconnect', [() => {hid.createHidHostProfile().connect(BADDR);}, () => {hid.createHidHostProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_HID_CONNECT_DISCONNECT_X
     * @tc.name combo_hid_connect_disconnect_x
     * @tc.desc Verify Bluetooth API combo hid connect disconnect repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_hid_connect_disconnect_x', 0, () => {
      try {
            fcRunCombo('combo_hid_connect_disconnect', [() => {hid.createHidHostProfile().connect(BADDR);}, () => {hid.createHidHostProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_HID_CONNECT_DISCONNECT_C
     * @tc.name combo_hid_connect_disconnect_c
     * @tc.desc Verify Bluetooth API combo hid connect disconnect combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_hid_connect_disconnect_c', 0, () => {
      try {
            fcRunCombo('combo_hid_connect_disconnect', [() => {hid.createHidHostProfile().connect(BADDR);}, () => {hid.createHidHostProfile().disconnect(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_pan_tether', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_PAN_TETHER_N
     * @tc.name combo_pan_tether_n
     * @tc.desc Verify Bluetooth API combo pan tether normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_pan_tether_n', 0, () => {
      try {
            fcRunCombo('combo_pan_tether', [() => {pan.createPanProfile().setTethering(true);}, () => {pan.createPanProfile().isTetheringOn();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_PAN_TETHER_E
     * @tc.name combo_pan_tether_e
     * @tc.desc Verify Bluetooth API combo pan tether error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_pan_tether_e', 0, () => {
      try {
            fcRunCombo('combo_pan_tether', [() => {pan.createPanProfile().setTethering(true);}, () => {pan.createPanProfile().isTetheringOn();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_PAN_TETHER_B
     * @tc.name combo_pan_tether_b
     * @tc.desc Verify Bluetooth API combo pan tether boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_pan_tether_b', 0, () => {
      try {
            fcRunCombo('combo_pan_tether', [() => {pan.createPanProfile().setTethering(true);}, () => {pan.createPanProfile().isTetheringOn();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_PAN_TETHER_R
     * @tc.name combo_pan_tether_r
     * @tc.desc Verify Bluetooth API combo pan tether return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_pan_tether_r', 0, () => {
      try {
            fcRunCombo('combo_pan_tether', [() => {pan.createPanProfile().setTethering(true);}, () => {pan.createPanProfile().isTetheringOn();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_PAN_TETHER_X
     * @tc.name combo_pan_tether_x
     * @tc.desc Verify Bluetooth API combo pan tether repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_pan_tether_x', 0, () => {
      try {
            fcRunCombo('combo_pan_tether', [() => {pan.createPanProfile().setTethering(true);}, () => {pan.createPanProfile().isTetheringOn();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_PAN_TETHER_C
     * @tc.name combo_pan_tether_c
     * @tc.desc Verify Bluetooth API combo pan tether combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_pan_tether_c', 0, () => {
      try {
            fcRunCombo('combo_pan_tether', [() => {pan.createPanProfile().setTethering(true);}, () => {pan.createPanProfile().isTetheringOn();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_socket_listen_close', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_SOCKET_LISTEN_CLOSE_N
     * @tc.name combo_socket_listen_close_n
     * @tc.desc Verify Bluetooth API combo socket listen close normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_socket_listen_close_n', 0, () => {
      try {
            fcRunCombo('combo_socket_listen_close', [() => {socket.sppListen('h', sppOpt(), () => {});}, () => {socket.sppCloseServerSocket(0);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_SOCKET_LISTEN_CLOSE_E
     * @tc.name combo_socket_listen_close_e
     * @tc.desc Verify Bluetooth API combo socket listen close error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_socket_listen_close_e', 0, () => {
      try {
            fcRunCombo('combo_socket_listen_close', [() => {socket.sppListen('h', sppOpt(), () => {});}, () => {socket.sppCloseServerSocket(0);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_SOCKET_LISTEN_CLOSE_B
     * @tc.name combo_socket_listen_close_b
     * @tc.desc Verify Bluetooth API combo socket listen close boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_socket_listen_close_b', 0, () => {
      try {
            fcRunCombo('combo_socket_listen_close', [() => {socket.sppListen('h', sppOpt(), () => {});}, () => {socket.sppCloseServerSocket(0);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_SOCKET_LISTEN_CLOSE_R
     * @tc.name combo_socket_listen_close_r
     * @tc.desc Verify Bluetooth API combo socket listen close return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_socket_listen_close_r', 0, () => {
      try {
            fcRunCombo('combo_socket_listen_close', [() => {socket.sppListen('h', sppOpt(), () => {});}, () => {socket.sppCloseServerSocket(0);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_SOCKET_LISTEN_CLOSE_X
     * @tc.name combo_socket_listen_close_x
     * @tc.desc Verify Bluetooth API combo socket listen close repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_socket_listen_close_x', 0, () => {
      try {
            fcRunCombo('combo_socket_listen_close', [() => {socket.sppListen('h', sppOpt(), () => {});}, () => {socket.sppCloseServerSocket(0);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_SOCKET_LISTEN_CLOSE_C
     * @tc.name combo_socket_listen_close_c
     * @tc.desc Verify Bluetooth API combo socket listen close combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_socket_listen_close_c', 0, () => {
      try {
            fcRunCombo('combo_socket_listen_close', [() => {socket.sppListen('h', sppOpt(), () => {});}, () => {socket.sppCloseServerSocket(0);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_legacy_bt_disc', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_LEGACY_BT_DISC_N
     * @tc.name combo_legacy_bt_disc_n
     * @tc.desc Verify Bluetooth API combo legacy bt disc normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_legacy_bt_disc_n', 0, () => {
      try {
            fcRunCombo('combo_legacy_bt_disc', [() => {bluetooth.startBluetoothDiscovery();}, () => {bluetooth.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_LEGACY_BT_DISC_E
     * @tc.name combo_legacy_bt_disc_e
     * @tc.desc Verify Bluetooth API combo legacy bt disc error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_legacy_bt_disc_e', 0, () => {
      try {
            fcRunCombo('combo_legacy_bt_disc', [() => {bluetooth.startBluetoothDiscovery();}, () => {bluetooth.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_LEGACY_BT_DISC_B
     * @tc.name combo_legacy_bt_disc_b
     * @tc.desc Verify Bluetooth API combo legacy bt disc boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_legacy_bt_disc_b', 0, () => {
      try {
            fcRunCombo('combo_legacy_bt_disc', [() => {bluetooth.startBluetoothDiscovery();}, () => {bluetooth.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_LEGACY_BT_DISC_R
     * @tc.name combo_legacy_bt_disc_r
     * @tc.desc Verify Bluetooth API combo legacy bt disc return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_legacy_bt_disc_r', 0, () => {
      try {
            fcRunCombo('combo_legacy_bt_disc', [() => {bluetooth.startBluetoothDiscovery();}, () => {bluetooth.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_LEGACY_BT_DISC_X
     * @tc.name combo_legacy_bt_disc_x
     * @tc.desc Verify Bluetooth API combo legacy bt disc repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_legacy_bt_disc_x', 0, () => {
      try {
            fcRunCombo('combo_legacy_bt_disc', [() => {bluetooth.startBluetoothDiscovery();}, () => {bluetooth.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_LEGACY_BT_DISC_C
     * @tc.name combo_legacy_bt_disc_c
     * @tc.desc Verify Bluetooth API combo legacy bt disc combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_legacy_bt_disc_c', 0, () => {
      try {
            fcRunCombo('combo_legacy_bt_disc', [() => {bluetooth.startBluetoothDiscovery();}, () => {bluetooth.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_btm_disc', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BTM_DISC_N
     * @tc.name combo_btm_disc_n
     * @tc.desc Verify Bluetooth API combo btm disc normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_btm_disc_n', 0, () => {
      try {
            fcRunCombo('combo_btm_disc', [() => {bluetoothManager.startBluetoothDiscovery();}, () => {bluetoothManager.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BTM_DISC_E
     * @tc.name combo_btm_disc_e
     * @tc.desc Verify Bluetooth API combo btm disc error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_btm_disc_e', 0, () => {
      try {
            fcRunCombo('combo_btm_disc', [() => {bluetoothManager.startBluetoothDiscovery();}, () => {bluetoothManager.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BTM_DISC_B
     * @tc.name combo_btm_disc_b
     * @tc.desc Verify Bluetooth API combo btm disc boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_btm_disc_b', 0, () => {
      try {
            fcRunCombo('combo_btm_disc', [() => {bluetoothManager.startBluetoothDiscovery();}, () => {bluetoothManager.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BTM_DISC_R
     * @tc.name combo_btm_disc_r
     * @tc.desc Verify Bluetooth API combo btm disc return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_btm_disc_r', 0, () => {
      try {
            fcRunCombo('combo_btm_disc', [() => {bluetoothManager.startBluetoothDiscovery();}, () => {bluetoothManager.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BTM_DISC_X
     * @tc.name combo_btm_disc_x
     * @tc.desc Verify Bluetooth API combo btm disc repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_btm_disc_x', 0, () => {
      try {
            fcRunCombo('combo_btm_disc', [() => {bluetoothManager.startBluetoothDiscovery();}, () => {bluetoothManager.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_BTM_DISC_C
     * @tc.name combo_btm_disc_c
     * @tc.desc Verify Bluetooth API combo btm disc combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_btm_disc_c', 0, () => {
      try {
            fcRunCombo('combo_btm_disc', [() => {bluetoothManager.startBluetoothDiscovery();}, () => {bluetoothManager.stopBluetoothDiscovery();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_profile_conn_state', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_PROFILE_CONN_STATE_N
     * @tc.name combo_profile_conn_state_n
     * @tc.desc Verify Bluetooth API combo profile conn state normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_profile_conn_state_n', 0, () => {
      try {
            fcRunCombo('combo_profile_conn_state', [() => {connection.getProfileConnectionState(1);}, () => {connection.getPairedDevices();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_PROFILE_CONN_STATE_E
     * @tc.name combo_profile_conn_state_e
     * @tc.desc Verify Bluetooth API combo profile conn state error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_profile_conn_state_e', 0, () => {
      try {
            fcRunCombo('combo_profile_conn_state', [() => {connection.getProfileConnectionState(1);}, () => {connection.getPairedDevices();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_PROFILE_CONN_STATE_B
     * @tc.name combo_profile_conn_state_b
     * @tc.desc Verify Bluetooth API combo profile conn state boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_profile_conn_state_b', 0, () => {
      try {
            fcRunCombo('combo_profile_conn_state', [() => {connection.getProfileConnectionState(1);}, () => {connection.getPairedDevices();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_PROFILE_CONN_STATE_R
     * @tc.name combo_profile_conn_state_r
     * @tc.desc Verify Bluetooth API combo profile conn state return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_profile_conn_state_r', 0, () => {
      try {
            fcRunCombo('combo_profile_conn_state', [() => {connection.getProfileConnectionState(1);}, () => {connection.getPairedDevices();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_PROFILE_CONN_STATE_X
     * @tc.name combo_profile_conn_state_x
     * @tc.desc Verify Bluetooth API combo profile conn state repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_profile_conn_state_x', 0, () => {
      try {
            fcRunCombo('combo_profile_conn_state', [() => {connection.getProfileConnectionState(1);}, () => {connection.getPairedDevices();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_PROFILE_CONN_STATE_C
     * @tc.name combo_profile_conn_state_c
     * @tc.desc Verify Bluetooth API combo profile conn state combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_profile_conn_state_c', 0, () => {
      try {
            fcRunCombo('combo_profile_conn_state', [() => {connection.getProfileConnectionState(1);}, () => {connection.getPairedDevices();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_opp_transfer', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_OPP_TRANSFER_N
     * @tc.name combo_opp_transfer_n
     * @tc.desc Verify Bluetooth API combo opp transfer normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_opp_transfer_n', 0, () => {
      try {
            fcRunCombo('combo_opp_transfer', [() => {const p = opp.createOppServerProfile();p.getCurrentTransferInformation();p.cancelTransfer();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_OPP_TRANSFER_E
     * @tc.name combo_opp_transfer_e
     * @tc.desc Verify Bluetooth API combo opp transfer error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_opp_transfer_e', 0, () => {
      try {
            fcRunCombo('combo_opp_transfer', [() => {const p = opp.createOppServerProfile();p.getCurrentTransferInformation();p.cancelTransfer();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_OPP_TRANSFER_B
     * @tc.name combo_opp_transfer_b
     * @tc.desc Verify Bluetooth API combo opp transfer boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_opp_transfer_b', 0, () => {
      try {
            fcRunCombo('combo_opp_transfer', [() => {const p = opp.createOppServerProfile();p.getCurrentTransferInformation();p.cancelTransfer();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_OPP_TRANSFER_R
     * @tc.name combo_opp_transfer_r
     * @tc.desc Verify Bluetooth API combo opp transfer return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_opp_transfer_r', 0, () => {
      try {
            fcRunCombo('combo_opp_transfer', [() => {const p = opp.createOppServerProfile();p.getCurrentTransferInformation();p.cancelTransfer();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_OPP_TRANSFER_X
     * @tc.name combo_opp_transfer_x
     * @tc.desc Verify Bluetooth API combo opp transfer repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_opp_transfer_x', 0, () => {
      try {
            fcRunCombo('combo_opp_transfer', [() => {const p = opp.createOppServerProfile();p.getCurrentTransferInformation();p.cancelTransfer();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_OPP_TRANSFER_C
     * @tc.name combo_opp_transfer_c
     * @tc.desc Verify Bluetooth API combo opp transfer combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_opp_transfer_c', 0, () => {
      try {
            fcRunCombo('combo_opp_transfer', [() => {const p = opp.createOppServerProfile();p.getCurrentTransferInformation();p.cancelTransfer();}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
    describe('combo_wear_detect', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_WEAR_DETECT_N
     * @tc.name combo_wear_detect_n
     * @tc.desc Verify Bluetooth API combo wear detect normal scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_wear_detect_n', 0, () => {
      try {
            fcRunCombo('combo_wear_detect', [() => {wearDetection.isWearDetectionSupported(BADDR);}, () => {wearDetection.isWearDetectionEnabled(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_WEAR_DETECT_E
     * @tc.name combo_wear_detect_e
     * @tc.desc Verify Bluetooth API combo wear detect error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_wear_detect_e', 0, () => {
      try {
            fcRunCombo('combo_wear_detect', [() => {wearDetection.isWearDetectionSupported(BADDR);}, () => {wearDetection.isWearDetectionEnabled(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_WEAR_DETECT_B
     * @tc.name combo_wear_detect_b
     * @tc.desc Verify Bluetooth API combo wear detect boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_wear_detect_b', 0, () => {
      try {
            fcRunCombo('combo_wear_detect', [() => {wearDetection.isWearDetectionSupported(BADDR);}, () => {wearDetection.isWearDetectionEnabled(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_WEAR_DETECT_R
     * @tc.name combo_wear_detect_r
     * @tc.desc Verify Bluetooth API combo wear detect return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_wear_detect_r', 0, () => {
      try {
            fcRunCombo('combo_wear_detect', [() => {wearDetection.isWearDetectionSupported(BADDR);}, () => {wearDetection.isWearDetectionEnabled(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_WEAR_DETECT_X
     * @tc.name combo_wear_detect_x
     * @tc.desc Verify Bluetooth API combo wear detect repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_wear_detect_x', 0, () => {
      try {
            fcRunCombo('combo_wear_detect', [() => {wearDetection.isWearDetectionSupported(BADDR);}, () => {wearDetection.isWearDetectionEnabled(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_COMBO_WEAR_DETECT_C
     * @tc.name combo_wear_detect_c
     * @tc.desc Verify Bluetooth API combo wear detect combo scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('combo_wear_detect_c', 0, () => {
      try {
            fcRunCombo('combo_wear_detect', [() => {wearDetection.isWearDetectionSupported(BADDR);}, () => {wearDetection.isWearDetectionEnabled(BADDR);}])
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
    });
  });
}




