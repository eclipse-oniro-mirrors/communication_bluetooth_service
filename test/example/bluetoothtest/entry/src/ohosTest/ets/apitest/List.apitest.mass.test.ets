/**
 * Mass-only pack: ConnBleSocketMass01～04 (BT ON, no peer).
 * Prefer after coreA/coreB; ~570 its.
 */
import connBleSocketMass01_p1 from './ConnBleSocketMass01_p1.test'
import connBleSocketMass01_p2 from './ConnBleSocketMass01_p2.test'
import connBleSocketMass01_p3 from './ConnBleSocketMass01_p3.test'
import connBleSocketMass02_p1 from './ConnBleSocketMass02_p1.test'
import connBleSocketMass02_p2 from './ConnBleSocketMass02_p2.test'
import connBleSocketMass02_p3 from './ConnBleSocketMass02_p3.test'
import connBleSocketMass03_p1 from './ConnBleSocketMass03_p1.test'
import connBleSocketMass03_p2 from './ConnBleSocketMass03_p2.test'
import connBleSocketMass04_p1 from './ConnBleSocketMass04_p1.test'
import connBleSocketMass04_p2 from './ConnBleSocketMass04_p2.test'
export default function testsuiteApitestMass(): void {
  connBleSocketMass01_p1()
  connBleSocketMass01_p2()
  connBleSocketMass01_p3()
  connBleSocketMass02_p1()
  connBleSocketMass02_p2()
  connBleSocketMass02_p3()
  connBleSocketMass03_p1()
  connBleSocketMass03_p2()
  connBleSocketMass04_p1()
  connBleSocketMass04_p2()
}
