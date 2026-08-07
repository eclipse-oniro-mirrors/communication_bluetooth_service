/**
 * Scenario core: baseline Pair/Triple/Quad/Scan/Name/Multi + Conn02 + BleAdvGatt + Socket + Cross.
 * Segmented green-run (do not merge with full List.apitest.test).
 */
import bluetoothScenarioPair01 from './BluetoothScenarioPair01.test'
import bluetoothScenarioTriple01 from './BluetoothScenarioTriple01.test'
import bluetoothScenarioQuad01 from './BluetoothScenarioQuad01.test'
import bluetoothScenarioScanParams01 from './BluetoothScenarioScanParams01.test'
import bluetoothScenarioNameParams01 from './BluetoothScenarioNameParams01.test'
import bluetoothScenarioMultiInstance01 from './BluetoothScenarioMultiInstance01.test'
import bluetoothScenarioConn02 from './BluetoothScenarioConn02.test'
import bluetoothScenarioBleAdvGatt02 from './BluetoothScenarioBleAdvGatt02.test'
import bluetoothScenarioSocket01 from './BluetoothScenarioSocket01.test'
import bluetoothScenarioCross01 from './BluetoothScenarioCross01.test'
import bluetoothScenarioMultiModule01 from './BluetoothScenarioMultiModule01.test'
export default function testsuiteApitestScenarioCore(): void {
  bluetoothScenarioPair01()
  bluetoothScenarioTriple01()
  bluetoothScenarioQuad01()
  bluetoothScenarioScanParams01()
  bluetoothScenarioNameParams01()
  bluetoothScenarioMultiInstance01()
  bluetoothScenarioConn02()
  bluetoothScenarioBleAdvGatt02()
  bluetoothScenarioSocket01()
  bluetoothScenarioCross01()
  bluetoothScenarioMultiModule01()
}
