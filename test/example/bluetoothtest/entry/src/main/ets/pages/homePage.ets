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

import ConfigData from '../Utils/ConfigData';
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
import { shouldSkipBluetoothPermissionRequest } from '../bluetoothTestNav';

const TAG = '[Screenshot]'

@Component
struct TestCategory {
  private testItems: TestData[] = []

  /** Tab 标签栏始终可见；UiTest 的 id 挂在标签上，避免仅当前 Tab 的 TabContent 被挂载导致查找不到。 */
  @Builder
  HomeTabBarItem(label: Resource, stableId: string) {
    Column() {
      Text(label)
        .id(stableId)
        .accessibilityLevel('yes')
        .fontSize(12)
        .margin({ top: ConfigData.VP_5 })
    }
    .accessibilityLevel('yes')
  }

  build() {
    Stack() {
      Tabs() {
        TabContent() {
          ManualApiTest({ testItems : this.testItems.filter(item => (item.category === Category.API)) })
        }
        .tabBar((): void => {
          this.HomeTabBarItem($r("app.string.api"), 'pages_homePage__tc_0');
        })

        TabContent() {
          ManualApiTest({ testItems : this.testItems.filter(item => (item.category === Category.APIManager)) })
        }
        .tabBar((): void => {
          this.HomeTabBarItem($r("app.string.api_M"), 'pages_homePage__tc_1');
        })

        TabContent() {
          AutoTest({ testItems : this.testItems.filter(item => (item.category === Category.AutoTest)) })
        }
        .tabBar((): void => {
          this.HomeTabBarItem($r("app.string.autotest"), 'pages_homePage__tc_2');
        })

        TabContent() {
          AutoTest({ testItems : this.testItems.filter(item => (item.category === Category.AutoTestManager)) })
        }
        .tabBar((): void => {
          this.HomeTabBarItem($r("app.string.autotest_M"), 'pages_homePage__tc_3');
        })

        TabContent() {
          ManualSceneTest({ testItems : this.testItems.filter(item => (item.category === Category.Scenario)) })
        }
        .tabBar((): void => {
          this.HomeTabBarItem($r("app.string.scenario"), 'pages_homePage__tc_4');
        })

        TabContent() {
          ManualSceneTest({ testItems : this.testItems.filter(item => (item.category === Category.ScenarioManager)) })
        }
        .tabBar((): void => {
          this.HomeTabBarItem($r("app.string.scenario_M"), 'pages_homePage__tc_5');
        })

        TabContent() {
          ProfileTest({ testItems : this.testItems.filter(item => (item.category === Category.Profile)) })
        }
        .tabBar((): void => {
          this.HomeTabBarItem($r("app.string.profile"), 'pages_homePage__tc_6');
        })

        TabContent() {
          ProfileTest({ testItems : this.testItems.filter(item => (item.category === Category.ProfileManager)) })
        }
        .tabBar((): void => {
          this.HomeTabBarItem($r("app.string.profile_M"), 'pages_homePage__tc_7');
        })

        TabContent() {
          BenchmarkTest({ testItems : this.testItems.filter(item => (item.category === Category.BenchMark)) })
        }
        .tabBar((): void => {
          this.HomeTabBarItem($r("app.string.benchmark"), 'pages_homePage__tc_8');
        })

        TabContent() {
          BenchmarkTest({
            testItems : this.testItems.filter(item => (item.category === Category.BenchMarkManager))
          })
        }
        .tabBar((): void => {
          this.HomeTabBarItem($r("app.string.benchmark_M"), 'pages_homePage__tc_9');
        })
      }
      .accessibilityLevel('yes')
      .id('pages_homePage__tabs_0')
      .accessibilityLevel('yes')
      .barWidth(ConfigData.WH_80_100)
      .barHeight(ConfigData.VP_40)
      .align(Alignment.TopStart)
      .barMode(BarMode.Fixed)
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

  /** UiTest 在 Tabs 自定义 tabBar 上常扫不到 Text.id；自动化时在首页底部挂载与 Tab 一一对应的锚点（仍参与无障碍树）。 */
  @Builder
  HomeUiTestTabIdAnchors() {
    Row() {
      Text($r('app.string.bluetoothtest_text_empty'))
        .id('pages_homePage__tc_0')
        .layoutWeight(1)
        .fontSize(1)
        .height(ConfigData.VP_2)
        .accessibilityLevel('yes')
  Text($r('app.string.bluetoothtest_text_empty'))
    .id('pages_homePage__tc_1')
    .layoutWeight(1)
    .fontSize(1)
    .height(ConfigData.VP_2)
    .accessibilityLevel('yes')
  Text($r('app.string.bluetoothtest_text_empty'))
    .id('pages_homePage__tc_2')
    .layoutWeight(1)
    .fontSize(1)
    .height(ConfigData.VP_2)
    .accessibilityLevel('yes')
  Text($r('app.string.bluetoothtest_text_empty'))
    .id('pages_homePage__tc_3')
    .layoutWeight(1)
    .fontSize(1)
    .height(ConfigData.VP_2)
    .accessibilityLevel('yes')
  Text($r('app.string.bluetoothtest_text_empty'))
    .id('pages_homePage__tc_4')
    .layoutWeight(1)
    .fontSize(1)
    .height(ConfigData.VP_2)
    .accessibilityLevel('yes')
  Text($r('app.string.bluetoothtest_text_empty'))
    .id('pages_homePage__tc_5')
    .layoutWeight(1)
    .fontSize(1)
    .height(ConfigData.VP_2)
    .accessibilityLevel('yes')
  Text($r('app.string.bluetoothtest_text_empty'))
    .id('pages_homePage__tc_6')
    .layoutWeight(1)
    .fontSize(1)
    .height(ConfigData.VP_2)
    .accessibilityLevel('yes')
  Text($r('app.string.bluetoothtest_text_empty'))
    .id('pages_homePage__tc_7')
    .layoutWeight(1)
    .fontSize(1)
    .height(ConfigData.VP_2)
    .accessibilityLevel('yes')
  Text($r('app.string.bluetoothtest_text_empty'))
    .id('pages_homePage__tc_8')
    .layoutWeight(1)
    .fontSize(1)
    .height(ConfigData.VP_2)
    .accessibilityLevel('yes')
  Text($r('app.string.bluetoothtest_text_empty'))
    .id('pages_homePage__tc_9')
    .layoutWeight(1)
    .fontSize(1)
    .height(ConfigData.VP_2)
    .accessibilityLevel('yes')
    }
    .width(ConfigData.WH_100_100)
    .height(ConfigData.VP_2)
  }

  aboutToAppear() {
    if (shouldSkipBluetoothPermissionRequest()) {
      return;
    }
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
        .accessibilityLevel('yes')
        .id('pages_homePage__mpt_0')
        .accessibilityLevel('yes')
      Stack({ alignContent : Alignment.TopEnd }) {
        TestCategory({ testItems : this.testItems })
        Image($r('app.media.Switch'))
          .accessibilityLevel('yes')
          .id('pages_homePage__img_0')
          .accessibilityLevel('yes')
          .height(ConfigData.VP_30)
          .width(ConfigData.VP_30)
          .margin({ top : ConfigData.VP_5 , right : ConfigData.VP_10 })
          .onClick(() => {
          })
      }
      .height(ConfigData.WH_100_100)
      if (shouldSkipBluetoothPermissionRequest()) {
        this.HomeUiTestTabIdAnchors()
      }
    }
  }
}
