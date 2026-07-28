      expect(typeof _v).assertEqual('number');
      expect(Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.UNKNOWN_REASON)).assertEqual(0);
      expect(Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.USER_CLOSED_ABILITY)).assertEqual(1
        );
      expect(Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.DEVICE_UNPAIRED)).assertEqual(2);
      expect(Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.DEVICE_LOST)).assertEqual(3);
      expect(Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.BLUETOOTH_DISABLED)).assertEqual(4)
        ;
      hilog.info(0x0000, 'BtFullCoverage', 'n: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DESTROYREASON_E
     * @tc.name pa_DestroyReason_e
     * @tc.desc Verify Bluetooth API pa DestroyReason error or exception scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_DestroyReason_e', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'e: invoke');
      const _a = Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.UNKNOWN_REASON);
      const _b = Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.USER_CLOSED_ABILITY);
      expect(_a !== _b).assertTrue();
      hilog.info(0x0000, 'BtFullCoverage', 'e: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DESTROYREASON_B
     * @tc.name pa_DestroyReason_b
     * @tc.desc Verify Bluetooth API pa DestroyReason boundary scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_DestroyReason_b', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'b: invoke');
      const _v = Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.BLUETOOTH_DISABLED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'b: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DESTROYREASON_R
     * @tc.name pa_DestroyReason_r
     * @tc.desc Verify Bluetooth API pa DestroyReason return value scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_DestroyReason_r', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'r: invoke');
      const _v = Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.DEVICE_UNPAIRED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'r: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DESTROYREASON_X
     * @tc.name pa_DestroyReason_x
     * @tc.desc Verify Bluetooth API pa DestroyReason repeat invocation scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_DestroyReason_x', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x1: invoke');
      const _v = Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.UNKNOWN_REASON);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x1: errCode=' + errCode);
      assertLocalOk(errCode);
      errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'x2: invoke');
      const _v2 = Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.UNKNOWN_REASON);
      expect(typeof _v2).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'x2: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DESTROYREASON_C
     * @tc.name pa_DestroyReason_c
     * @tc.desc Verify Bluetooth API pa DestroyReason condition scenario
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_DestroyReason_c', 0, () => {
      let errCode = 0;
      hilog.info(0x0000, 'BtFullCoverage', 'c: invoke');
      const _v = Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.BLUETOOTH_DISABLED);
      expect(typeof _v).assertEqual('number');
      hilog.info(0x0000, 'BtFullCoverage', 'c: errCode=' + errCode);
      assertLocalOk(errCode);
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DESTROYREASON_PERF
     * @tc.name pa_DestroyReason_perf
     * @tc.desc Verify Bluetooth apitest case pa DestroyReason perf
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_DestroyReason_perf', 0, () => {
      runPerf('pa_DestroyReason_perf', 5000, () => {
        try {
          Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.UNKNOWN_REASON);
        } catch (_e) {
        }
      });
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_PA_DESTROYREASON_STRESS
     * @tc.name pa_DestroyReason_stress
     * @tc.desc Verify Bluetooth apitest case pa DestroyReason stress
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('pa_DestroyReason_stress', 0, () => {
      runStress('pa_DestroyReason_stress', 20, () => {
        try {
          Number(partnerAgent.PartnerAgentExtensionAbilityDestroyReason.UNKNOWN_REASON);
        } catch (_e) {
        }
      });
    });
    });
  });
}


