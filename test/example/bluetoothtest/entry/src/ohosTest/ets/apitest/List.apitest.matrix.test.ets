/**
 * ExpandedMatrix + light apitest kits (no Mass; Mass has its own List).
 */
import abilityTest from './Ability.test'
import pageRoutesTest from './PageRoutes.test'
import bluetoothApiCoverageTest from './BluetoothApiCoverage.test'
import connectivityKitBluetoothTest from './ConnectivityKitBluetooth.test'
import bluetoothExpandedMatrixConnection01_p1 from './BluetoothExpandedMatrixConnection01_p1.test'
import bluetoothExpandedMatrixConnection01_p2 from './BluetoothExpandedMatrixConnection01_p2.test'
import bluetoothExpandedMatrixConnection02_p1 from './BluetoothExpandedMatrixConnection02_p1.test'
import bluetoothExpandedMatrixConnection02_p2 from './BluetoothExpandedMatrixConnection02_p2.test'
import bluetoothExpandedMatrixLegacy01_p1 from './BluetoothExpandedMatrixLegacy01_p1.test'
import bluetoothExpandedMatrixLegacy01_p2 from './BluetoothExpandedMatrixLegacy01_p2.test'
import bluetoothExpandedMatrixLegacy02_p1 from './BluetoothExpandedMatrixLegacy02_p1.test'
import bluetoothExpandedMatrixLegacy02_p2 from './BluetoothExpandedMatrixLegacy02_p2.test'
import bluetoothExpandedMatrixRotate from './BluetoothExpandedMatrixRotate.test'

export default function testsuiteApitestMatrix(): void {
  abilityTest()
  pageRoutesTest()
  bluetoothApiCoverageTest()
  connectivityKitBluetoothTest()
  bluetoothExpandedMatrixConnection01_p1()
  bluetoothExpandedMatrixConnection01_p2()
  bluetoothExpandedMatrixConnection02_p1()
  bluetoothExpandedMatrixConnection02_p2()
  bluetoothExpandedMatrixLegacy01_p1()
  bluetoothExpandedMatrixLegacy01_p2()
  bluetoothExpandedMatrixLegacy02_p1()
  bluetoothExpandedMatrixLegacy02_p2()
  bluetoothExpandedMatrixRotate()
}
