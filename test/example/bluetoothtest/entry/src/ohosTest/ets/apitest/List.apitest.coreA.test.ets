/**
 * Core pack part A: Access + Legacy + constant/common (BT ON, no peer).
 */
import bluetoothFullCoverageAccess01 from './BluetoothFullCoverageAccess01.test'
import bluetoothFullCoverageConstant01 from './BluetoothFullCoverageConstant01.test'
import bluetoothFullCoverageLegacy01 from './BluetoothFullCoverageLegacy01.test'
import bluetoothFullCoverageLegacy02 from './BluetoothFullCoverageLegacy02.test'

export default function testsuiteApitestCoreA(): void {
  bluetoothFullCoverageAccess01()
  bluetoothFullCoverageConstant01()
  bluetoothFullCoverageLegacy01()
  bluetoothFullCoverageLegacy02()
}
