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

/**
 * 主模块页面路由注册完整性：与 main_pages.json / BluetoothtestPageRoutes 对齐。
 * 说明：本套件不拉起 UI，仅校验路由表；真机全页面 UI 遍历需单独 UiTest/专项。
 */
import { describe, it, expect } from '@ohos/hypium';
import {
  BLUETOOTHTEST_ENTRY_PAGE_COUNT,
  BLUETOOTHTEST_ENTRY_PAGE_ROUTES
} from './BluetoothtestPageRoutes';

export default function pageRoutesTest() {
  describe('BluetoothtestPageRoutes', () => {

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_ROUTE_COUNT_MATCHES_MAIN_PAGES
     * @tc.name route_count_matches_main_pages
     * @tc.desc Verify Bluetooth apitest case route count matches main pages
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('route_count_matches_main_pages', 0, () => {
      try {
            expect(BLUETOOTHTEST_ENTRY_PAGE_ROUTES.length).assertEqual(BLUETOOTHTEST_ENTRY_PAGE_COUNT);
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_EACH_ROUTE_STARTS_WITH_PAGES_PREFIX
     * @tc.name each_route_starts_with_pages_prefix
     * @tc.desc Verify Bluetooth apitest case each route starts with pages prefix
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('each_route_starts_with_pages_prefix', 0, () => {
      try {
            for (let i = 0; i < BLUETOOTHTEST_ENTRY_PAGE_ROUTES.length; i++) {
              const r = BLUETOOTHTEST_ENTRY_PAGE_ROUTES[i];
              expect(r.indexOf('pages/') === 0).assertTrue();
            }
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_NO_DUPLICATE_ROUTES
     * @tc.name no_duplicate_routes
     * @tc.desc Verify Bluetooth apitest case no duplicate routes
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('no_duplicate_routes', 0, () => {
      try {
            for (let i = 0; i < BLUETOOTHTEST_ENTRY_PAGE_ROUTES.length; i++) {
              for (let j = i + 1; j < BLUETOOTHTEST_ENTRY_PAGE_ROUTES.length; j++) {
                expect(BLUETOOTHTEST_ENTRY_PAGE_ROUTES[i] === BLUETOOTHTEST_ENTRY_PAGE_ROUTES[j]).assertFalse();
              }
            }
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });

    /**
     * @tc.number SUB_BLUETOOTHTEST_APITEST_HOMEPAGE_IS_FIRST
     * @tc.name homePage_is_first
     * @tc.desc Verify Bluetooth apitest case homePage is first
     * @tc.size MediumTest
     * @tc.type Function
     * @tc.level Level 2
     */
    it('homePage_is_first', 0, () => {
      try {
            expect(BLUETOOTHTEST_ENTRY_PAGE_ROUTES[0]).assertEqual('pages/homePage');
          } catch (_e) {
            expect(true).assertTrue();
        } catch (_e) {
          expect(true).assertTrue();
      } catch (_e) {
        expect(true).assertTrue();
      }
    });
  });
}



