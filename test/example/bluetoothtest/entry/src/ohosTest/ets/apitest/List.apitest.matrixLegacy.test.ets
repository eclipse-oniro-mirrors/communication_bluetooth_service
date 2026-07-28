/**
 * ExpandedMatrix Legacy + Rotate + light kits.
 */
import abilityTest from './Ability.test'
import pageRoutesTest from './PageRoutes.test'
import bluetoothApiCoverageTest from './BluetoothApiCoverage.test'
import connectivityKitBluetoothTest from './ConnectivityKitBluetooth.test'
import bluetoothExpandedMatrixLegacy01_p1 from './BluetoothExpandedMatrixLegacy01_p1.test'
import bluetoothExpandedMatrixLegacy01_p2 from './BluetoothExpandedMatrixLegacy01_p2.test'
import bluetoothExpandedMatrixLegacy02_p1 from './BluetoothExpandedMatrixLegacy02_p1.test'
import bluetoothExpandedMatrixLegacy02_p2 from './BluetoothExpandedMatrixLegacy02_p2.test'
import bluetoothExpandedMatrixRotate from './BluetoothExpandedMatrixRotate.test'

export default function testsuiteApitestMatrixLegacy(): void {
  abilityTest()
  pageRoutesTest()
  bluetoothApiCoverageTest()
  connectivityKitBluetoothTest()
  bluetoothExpandedMatrixLegacy01_p1()
  bluetoothExpandedMatrixLegacy01_p2()
  bluetoothExpandedMatrixLegacy02_p1()
  bluetoothExpandedMatrixLegacy02_p2()
  bluetoothExpandedMatrixRotate()
}
