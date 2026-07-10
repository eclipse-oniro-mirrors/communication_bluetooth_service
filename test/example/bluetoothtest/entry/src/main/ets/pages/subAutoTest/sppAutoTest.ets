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
import { PageTitle} from '../../component/pageTitle';
import { TestImageDisplay} from '../../component/testImageDisplay';
import { initSppAutoTestData} from '../../MainAbility/model/testDataModels'
import { TestListItem} from '../../component/testListItems';

/**
 *  SppAutoTest Page Of Bluetooth test
 */

@Entry
@Component
struct SppAutoTest {
  private sppAutoTestItems: TestData[] = initSppAutoTestData();
  private testItem: TestData = testItemFromRouterOrPlaceholder()
  @StorageLink("autoTestMessage") autoTestMessage: string = ""

  build() {
    Column() {
      Stack({alignContent: Alignment.TopStart}) {
        TestImageDisplay({testItem: this.testItem})
        PageTitle({testItem: this.testItem})
      .accessibilityLevel('yes')
      .id('pages_subAutoTest_sppAutoTest__pt_0')
      }

      List() {
        ForEach(this.sppAutoTestItems, (item: TestData, index: number) => {
          ListItem() {
            TestListItem({
              testItem: item
            })
          }
      .id(index === 0 ? 'pages_subAutoTest_sppAutoTest__li_0' :
        `pages_subAutoTest_sppAutoTest__li_${index}`)
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