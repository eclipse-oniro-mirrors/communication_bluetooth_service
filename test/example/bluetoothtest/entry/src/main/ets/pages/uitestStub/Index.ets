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

/** 与 UitestStubSmoke.test 中 text('uitest stub') 一致；勿改为 $r，否则 zh_CN 等语言下文案不同会导致用例失败。 */
const UITEST_STUB_TITLE: string = 'uitest stub';

@Entry
@Component
struct Index {
  build(){
    Column(){
      Text(UITEST_STUB_TITLE)
        .id('uitest_stub_title')
        .accessibilityLevel('yes')
        .fontSize(16)
    }
    .width(ConfigData.WH_100_100)
    .height(ConfigData.WH_100_100)
  }
}
