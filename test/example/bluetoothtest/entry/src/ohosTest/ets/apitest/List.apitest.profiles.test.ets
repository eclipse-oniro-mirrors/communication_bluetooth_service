/**
 * Profile pack (1A): a2dp/hfp/hid/map/opp/pan/pbap/baseProfile + enterprise/partner.
 * wearDetection lives in Access01 (covered via List.apitest.coreA).
 */
import bluetoothFullCoverageProfile01_p1 from './BluetoothFullCoverageProfile01_p1.test'
import bluetoothFullCoverageProfile01_p2 from './BluetoothFullCoverageProfile01_p2.test'
import bluetoothFullCoverageProfile01_p3 from './BluetoothFullCoverageProfile01_p3.test'
import bluetoothFullCoverageProfile01_p4 from './BluetoothFullCoverageProfile01_p4.test'
import bluetoothFullCoverageProfile02_p1 from './BluetoothFullCoverageProfile02_p1.test'
import bluetoothFullCoverageProfile02_p2 from './BluetoothFullCoverageProfile02_p2.test'
import bluetoothFullCoverageProfile02_p3 from './BluetoothFullCoverageProfile02_p3.test'
import bluetoothFullCoverageProfile02_p4 from './BluetoothFullCoverageProfile02_p4.test'
import bluetoothFullCoverageProfile02_p5 from './BluetoothFullCoverageProfile02_p5.test'
import bluetoothFullCoverageProfile02_p6 from './BluetoothFullCoverageProfile02_p6.test'
import bluetoothFullCoverageEnterprise01_p1 from './BluetoothFullCoverageEnterprise01_p1.test'
import bluetoothFullCoverageEnterprise01_p2 from './BluetoothFullCoverageEnterprise01_p2.test'
import bluetoothFullCoverageEnterprise01_p3 from './BluetoothFullCoverageEnterprise01_p3.test'
export default function testsuiteApitestProfiles(): void {
  bluetoothFullCoverageProfile01_p1()
  bluetoothFullCoverageProfile01_p2()
  bluetoothFullCoverageProfile01_p3()
  bluetoothFullCoverageProfile01_p4()
  bluetoothFullCoverageProfile02_p1()
  bluetoothFullCoverageProfile02_p2()
  bluetoothFullCoverageProfile02_p3()
  bluetoothFullCoverageProfile02_p4()
  bluetoothFullCoverageProfile02_p5()
  bluetoothFullCoverageProfile02_p6()
  bluetoothFullCoverageEnterprise01_p1()
  bluetoothFullCoverageEnterprise01_p2()
  bluetoothFullCoverageEnterprise01_p3()
}
