/**
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
import Log from '../../../../../../../../common/utils/src/main/ets/default/baseUtil/LogDecorator';
import PageSearchDataProvider from '../../../../../../../../common/search/src/main/ets/default/provider/PageSearchDataProvider';

const SEARCH_DATA = {
  data: []
};

/**
 * Search data provider for wifi
 */
export class WifiProvider extends PageSearchDataProvider {

  /**
   * Get page search data
   */
  @Log
  getPageSearchData(): any {
    return SEARCH_DATA;
  }

}

let wifiProvider = new WifiProvider();
export default wifiProvider as WifiProvider
;