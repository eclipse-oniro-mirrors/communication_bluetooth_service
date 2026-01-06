/**
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import { Category , TestData } from '../MainAbility/model/testData'
import { initTestData } from '../MainAbility/model/testDataModels'
import { MainPageTitle } from '../Component/mainPageTitle';
import { ManualApiTest } from '../Component/manualApiTestComponent';
import { ManualSceneTest } from '../Component/manualSceneTest';
import { AutoTest } from '../Component/autoTest';
import { ProfileTest } from '../Component/profileTest';
import { BenchmarkTest } from '../Component/benchmarkTest';
import abilityAccessCtrl from '@ohos.abilityAccessCtrl';
import common from '@ohos.app.ability.common';
import { Permissions } from '@ohos.abilityAccessCtrl';
import dlog from '../MainAbility/model/dlog';

const TAG = '[Screenshot]'

@Component
struct TestCategory {
  private testItems: TestData[] = []

  build() {
    Stack() {
      Tabs() {
        TabContent() {
          ManualApiTest({ testItems : this.testItems.filter(item => (item.category === Category.API)) })
        }.tabBar($r("app.string.api")).margin({ top : '5vp' })

        TabContent() {
          ManualApiTest({ testItems : this.testItems.filter(item => (item.category === Category.APIManager)) })
        }.tabBar($r("app.string.api_M")).margin({ top : '5vp' })

        TabContent() {
          AutoTest({ testItems : this.testItems.filter(item => (item.category === Category.AutoTest)) })
        }.tabBar($r("app.string.autotest")).margin({ top : '5vp' })

        TabContent() {
          AutoTest({ testItems : this.testItems.filter(item => (item.category === Category.AutoTestManager)) })
        }.tabBar($r("app.string.autotest_M")).margin({ top : '5vp' })

        TabContent() {
          ManualSceneTest({ testItems : this.testItems.filter(item => (item.category === Category.Scenario)) })
        }.tabBar($r("app.string.scenario")).margin({ top : '5vp' })

        TabContent() {
          ManualSceneTest({ testItems : this.testItems.filter(item => (item.category === Category.ScenarioManager)) })
        }.tabBar($r("app.string.scenario_M")).margin({ top : '5vp' })

        TabContent() {
          ProfileTest({ testItems : this.testItems.filter(item => (item.category === Category.Profile)) })
        }.tabBar($r("app.string.profile")).margin({ top : '5vp' })

        TabContent() {
          ProfileTest({ testItems : this.testItems.filter(item => (item.category === Category.ProfileManager)) })
        }.tabBar($r("app.string.profile_M")).margin({ top : '5vp' })

        TabContent() {
          BenchmarkTest({ testItems : this.testItems.filter(item => (item.category === Category.BenchMark)) })
        }.tabBar($r("app.string.benchmark")).margin({ top : '5vp' })

        TabContent() {
          BenchmarkTest({
            testItems : this.testItems.filter(item => (item.category === Category.BenchMarkManager))
          })
        }.tabBar($r("app.string.benchmark_M"))
      }
      .barWidth('80%')
      .barHeight('40vp')
      .align(Alignment.TopStart)
      .barMode(BarMode.Scrollable)
      .backgroundColor($r("app.color.white"))
    }
  }
}

/**
 * Home Page Of Bluetooth test
 */
@Entry
@Component
struct HomePage {
  private testItems: TestData[] = initTestData()
  private permissions: Array<Permissions> = [
    "ohos.permission.LOCATION",
    "ohos.permission.ACCESS_BLUETOOTH",
    "ohos.permission.APPROXIMATELY_LOCATION",
    "ohos.permission.CAPTURE_SCREEN",
    "ohos.permission.WRITE_MEDIA",
    "ohos.permission.READ_MEDIA"
  ]

  init() {
    this.testItems = initTestData();
  }

  aboutToAppear() {
    let abilityMgr = abilityAccessCtrl.createAtManager();
    let context: Context = getContext(this) as common.UIAbilityContext;
    abilityMgr.requestPermissionsFromUser(context , this.permissions , (error , result) => {
        dlog.info("bluetooth requestPermissionsFromUser, err: %{public}s ; result: %{public}s",
          JSON.stringify(error), JSON.stringify(result));
    })
  }

  build() {
    Column() {
      MainPageTitle()
      Stack({ alignContent : Alignment.TopEnd }) {
        TestCategory({ testItems : this.testItems })
        Image($r('app.media.Switch'))
          .height('30vp')
          .width('30vp')
          .margin({ top : '5vp' , right : '10vp' })
          .onClick(() => {
          })
      }
      .height('100%')
    }
  }
}
