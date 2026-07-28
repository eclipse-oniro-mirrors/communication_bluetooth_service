/**
 * Mass-only pack: ConnBleSocketMass01～04 (BT ON, no peer).
 * Prefer after coreA/coreB; ~570 its.
 */
import connBleSocketMass01 from './ConnBleSocketMass01.test'
import connBleSocketMass02 from './ConnBleSocketMass02.test'
import connBleSocketMass03 from './ConnBleSocketMass03.test'
import connBleSocketMass04 from './ConnBleSocketMass04.test'

export default function testsuiteApitestMass(): void {
  connBleSocketMass01()
  connBleSocketMass02()
  connBleSocketMass03()
  connBleSocketMass04()
}
