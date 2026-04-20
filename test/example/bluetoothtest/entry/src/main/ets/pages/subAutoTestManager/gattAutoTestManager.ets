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

import { testItemFromRouterOrPlaceholder} from '../../Utils/common'
import ConfigData from '../../Utils/ConfigData';
import { TestData} from '../../MainAbility/model/testData'
import { PageTitle} from '../../Component/pageTitle';
import { TestImageDisplay} from '../../Component/testImageDisplay';
import { initGattManagerAutoTestData} from '../../MainAbility/model/testDataModelsManager'
import { TestListItem} from '../../Component/testListItems';

/**
 *  GattAutoTest Manager Page Of Bluetooth test
 */

@Entry
@Component
struct GattAutoTestManager {
  private gattAutoTestItems: TestData[] = initGattManagerAutoTestData();
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @StorageLink("autoTestMessage") autoTestMessage: string = ""

  build(){
    Column(){
      Stack({alignContent: Alignment.TopStart}){
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
      .accessibilityLevel('yes')
      .id('pages_subAutoTestManager_gattAutoTestManager__pt_0')
      }

      List(){
        ForEach(this.gattAutoTestItems, (item: TestData, index: number) => {
          ListItem(){
            TestListItem({
              testItem: item
            })
          }
      .id(index === 0 ? 'pages_subAutoTestManager_gattAutoTestManager__li_0' : `pages_subAutoTestManager_gattAutoTestManager__li_${index}`)
        }, (item: TestData) => item.id)
      }
      .height(ConfigData.PCT_93)
      .backgroundColor($r("app.color.lead"))
    }
    .backgroundColor($r("sys.color.ohos_id_color_sub_background"))
    .width(ConfigData.WH_100_100)
    .height(ConfigData.WH_100_100);
  }
}

