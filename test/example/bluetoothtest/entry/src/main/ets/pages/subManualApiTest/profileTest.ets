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

import ConfigData from '../../Utils/ConfigData';
import { testItemFromRouterOrPlaceholder} from '../../Utils/common'
import { TestData} from '../../MainAbility/model/testData'
import { initA2dpSourceProfileApiData} from '../../MainAbility/model/testDataModels'
import { PageTitle} from '../../Component/pageTitle';
import { TestImageDisplay} from '../../Component/testImageDisplay';
import { ContentTable} from '../../Component/contentTable';
import btProfileModel from '../../MainAbility/model/btProfileModel';

/**
 * Profile Test Page Of Bluetooth test
 */

@Entry
@Component
struct ProfileTest {
  private testItem: TestData = testItemFromRouterOrPlaceholder()

  aboutToAppear(){
  // btProfileModel.initProfile()
  }

  build(){
    Column(){
      Stack({alignContent: Alignment.TopStart}){
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
    .accessibilityLevel('yes')
    .id('pages_subManualApiTest_profileTest__pt_0')
      }
      ContentTable({apiItems: initA2dpSourceProfileApiData()})
    }
    .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
    .height(ConfigData.WH_100_100)
    .alignItems(HorizontalAlign.Start)
  }
}