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
import screenshot from '@ohos.screenshot'
import image from '@ohos.multimedia.image'

import MediaUtils from '../MainAbility/model/MediaUtils'
import Logger from '../MainAbility/model/Logger'

const TAG = '[Screenshot]'

@Component
struct TestCategory {
  private testItems: TestData[]

  build() {
    Stack() {
      Tabs() {
        TabContent() {
          ManualApiTest( { testItems : this.testItems.filter( item => ( item.category === Category.API ) ) } )
        }.tabBar( $r( "app.string.api" ) )

        TabContent() {
          ManualApiTest( { testItems : this.testItems.filter( item => ( item.category === Category.APIManager ) ) } )
        }.tabBar( $r( "app.string.api_M" ) )

        TabContent() {
          AutoTest( { testItems : this.testItems.filter( item => ( item.category === Category.AutoTest ) ) } )
        }.tabBar( $r( "app.string.autotest" ) )

        TabContent() {
          AutoTest( { testItems : this.testItems.filter( item => ( item.category === Category.AutoTestManager ) ) } )
        }.tabBar( $r( "app.string.autotest_M" ) )

        TabContent() {
          ManualSceneTest( { testItems : this.testItems.filter( item => ( item.category === Category.Scenario ) ) } )
        }.tabBar( $r( "app.string.scenario" ) )

        TabContent() {
          ManualSceneTest( { testItems : this.testItems.filter( item => ( item.category === Category.ScenarioManager ) ) } )
        }.tabBar( $r( "app.string.scenario_M" ) )

        TabContent() {
          ProfileTest( { testItems : this.testItems.filter( item => ( item.category === Category.Profile ) ) } )
        }.tabBar( $r( "app.string.profile" ) )

        TabContent() {
          ProfileTest( { testItems : this.testItems.filter( item => ( item.category === Category.ProfileManager ) ) } )
        }.tabBar( $r( "app.string.profile_M" ) )

        TabContent() {
          BenchmarkTest( { testItems : this.testItems.filter( item => ( item.category === Category.BenchMark ) ) } )
        }.tabBar( $r( "app.string.benchmark" ) )

        TabContent() {
          BenchmarkTest( {
            testItems : this.testItems.filter( item => ( item.category === Category.BenchMarkManager ) )
          } )
        }.tabBar( $r( "app.string.benchmark_M" ) )
      }
      .barWidth( "80%" )
      .barHeight( 60 )
      .barMode( BarMode.Scrollable )
      .backgroundColor( $r( "app.color.white" ) )
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

  build() {
    Column() {
      MainPageTitle()
      Stack( { alignContent : Alignment.TopEnd } ) {
        TestCategory( { testItems : this.testItems } )
        Image( $r( 'app.media.Switch' ) )
          .height( 30 )
          .width( 30 )
          .margin( { top : 15 , right : 10 } )
          .onClick( () => {
          } )
      }
      .height( '100%' )
    }
  }
}
