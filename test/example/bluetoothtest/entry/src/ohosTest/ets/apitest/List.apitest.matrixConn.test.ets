/**
 * ExpandedMatrix Connection only.
 */
import bluetoothExpandedMatrixConnection01_p1 from './BluetoothExpandedMatrixConnection01_p1.test'
import bluetoothExpandedMatrixConnection01_p2 from './BluetoothExpandedMatrixConnection01_p2.test'
import bluetoothExpandedMatrixConnection02_p1 from './BluetoothExpandedMatrixConnection02_p1.test'
import bluetoothExpandedMatrixConnection02_p2 from './BluetoothExpandedMatrixConnection02_p2.test'

export default function testsuiteApitestMatrixConn(): void {
  bluetoothExpandedMatrixConnection01_p1()
  bluetoothExpandedMatrixConnection01_p2()
  bluetoothExpandedMatrixConnection02_p1()
  bluetoothExpandedMatrixConnection02_p2()
}
