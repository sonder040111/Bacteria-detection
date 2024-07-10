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
import BaseModel from '../../../../../../../../common/utils/src/main/ets/default/model/BaseModel';
import Updater from '@ohos.update';
import LogUtil from '../../../../../../../../common/utils/src/main/ets/default/baseUtil/LogUtil';
import ConfigData from '../../../../../../../../common/utils/src/main/ets/default/baseUtil/ConfigData';
import {LogAll} from '../../../../../../../../common/utils/src/main/ets/default/baseUtil/LogDecorator';

/**
 * Restore factorySettings model
 */
@LogAll
export class RestoreFactorySettingsModel extends BaseModel {


  /**
   * Reboot and clean userData
   */
  rebootAndCleanUserData(callback?: (value) => void) {
    Updater.getUpdater('/data/updater/updater.zip', 'OTA').rebootAndCleanUserData().then((value) => {
      LogUtil.log(ConfigData.TAG + "rebootAndCleanUserData value:" + value)
    })
  }
}

let restoreFactorySettingsModel = new RestoreFactorySettingsModel();

export default restoreFactorySettingsModel as RestoreFactorySettingsModel
;