/**
 * Scenario aggregate (all modules). Prefer segmented: scenario.core / profiles / legacy / extra.
 * Do not run with full List.apitest.test in one shot.
 */
import testsuiteApitestScenarioCore from './List.apitest.scenario.core.test'
import testsuiteApitestScenarioProfiles from './List.apitest.scenario.profiles.test'
import testsuiteApitestScenarioLegacy from './List.apitest.scenario.legacy.test'
import testsuiteApitestScenarioExtra from './List.apitest.scenario.extra.test'
export default function testsuiteApitestScenario(): void {
  testsuiteApitestScenarioCore()
  testsuiteApitestScenarioProfiles()
  testsuiteApitestScenarioLegacy()
  testsuiteApitestScenarioExtra()
}
