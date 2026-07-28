/**
 * Profile pack (1A): a2dp/hfp/hid/map/opp/pan/pbap/baseProfile + enterprise/partner.
 * wearDetection lives in Access01 (covered via List.apitest.coreA).
 */
import bluetoothFullCoverageProfile01 from './BluetoothFullCoverageProfile01.test'
import bluetoothFullCoverageProfile02 from './BluetoothFullCoverageProfile02.test'
import bluetoothFullCoverageEnterprise01 from './BluetoothFullCoverageEnterprise01.test'

export default function testsuiteApitestProfiles(): void {
  bluetoothFullCoverageProfile01()
  bluetoothFullCoverageProfile02()
  bluetoothFullCoverageEnterprise01()
}
