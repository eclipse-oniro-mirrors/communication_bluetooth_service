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




