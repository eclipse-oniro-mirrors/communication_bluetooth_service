/**
 * Core pack part B: GATT + Connection + Socket (BT ON, no peer).
 */
import bluetoothFullCoverageGatt01 from './BluetoothFullCoverageGatt01.test'
import bluetoothFullCoverageGatt02 from './BluetoothFullCoverageGatt02.test'
import bluetoothFullCoverageConnection01 from './BluetoothFullCoverageConnection01.test'
import bluetoothFullCoverageConnection02 from './BluetoothFullCoverageConnection02.test'
import bluetoothFullCoverageSocket01 from './BluetoothFullCoverageSocket01.test'

export default function testsuiteApitestCoreB(): void {
  bluetoothFullCoverageGatt01()
  bluetoothFullCoverageGatt02()
  bluetoothFullCoverageConnection01()
  bluetoothFullCoverageConnection02()
  bluetoothFullCoverageSocket01()
}
