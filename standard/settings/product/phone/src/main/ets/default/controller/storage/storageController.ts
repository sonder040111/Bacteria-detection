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
import StorageModel from '../../model/StorageImpl/StorageModel'
import {LogAll} from '../../../../../../../../common/utils/src/main/ets/default/baseUtil/LogDecorator';
import LogUtil from '../../../../../../../../common/utils/src/main/ets/default/baseUtil/LogUtil';
import ConfigData from '../../../../../../../../common/utils/src/main/ets/default/baseUtil/ConfigData';
import BaseSettingsController from '../../../../../../../../common/component/src/main/ets/default/controller/BaseSettingsController';
import ISettingsController from '../../../../../../../../common/component/src/main/ets/default/controller/ISettingsController'

enum Space {
  /**
  * Used space
  */
  USED_SPACE = '0',
  /**
  * Remaining space
  */
  REMAINING_SPACE = '1'
}

@LogAll
export default class storageController  extends BaseSettingsController {
  private storageList: any[] = [];
  private totalSpace: any = '';
  private freeBytes: any = '';
  private usedSpace: any = '';
  private proportion: number = 0;
  private usedSpaceList:any = [];

  initData(): ISettingsController{
    super.initData();
    this.getTotalSpace();
    return this;
  }

  /**
   * get TotalSpace
   */
  getTotalSpace() {
    StorageModel.getStorageDataDir((err, data: string) => {

      LogUtil.info(ConfigData.TAG + 'getStorageDataDir err: ' + JSON.stringify(err));
      LogUtil.info(ConfigData.TAG + 'getStorageDataDir data: ' + JSON.stringify(data));
      if (data && data.length > 0) {
        StorageModel.getTotalSpace(data, (err, data) => {
          LogUtil.info(ConfigData.TAG + 'getTotalSpace err: ' + JSON.stringify(err));
          LogUtil.info(ConfigData.TAG + 'getTotalSpace data: ' + JSON.stringify(data));
          if (data && data >= 0) {
            LogUtil.info(ConfigData.TAG + 'getTotalSpace success');
            this.totalSpace = data;
            LogUtil.info(ConfigData.TAG + 'getTotalSpace this.totalSpace*********: ' + JSON.stringify(this.totalSpace));
            this.getFreeBytes();
          }
        });
      }
    });
  }

  /**
   * get RemainingSpace
   */
  getFreeBytes() {
    StorageModel.getStorageDataDir((err, data) => {
      LogUtil.info(ConfigData.TAG + 'getStorageDataDir err: ' + JSON.stringify(err));
      LogUtil.info(ConfigData.TAG + 'getStorageDataDir data: ' + JSON.stringify(data));
      if (data && data.length > 0) {
        StorageModel.getFreeBytes(data, (err, data) => {
          LogUtil.info(ConfigData.TAG + 'getFreeBytes err: ' + JSON.stringify(err));
          LogUtil.info(ConfigData.TAG + 'getFreeBytes data: ' + JSON.stringify(data));
          if (data && data >= 0) {
            LogUtil.info(ConfigData.TAG + 'getFreeBytes success');
            this.freeBytes = data;
            // get UsedSpace
            this.getUsedSpace();
            // get space proportion
            this.getSpaceProportion();
            // format data
            this.totalSpace = this.formatData(this.totalSpace)
            this.freeBytes = this.formatData(this.freeBytes)
            this.usedSpace = this.formatData(this.usedSpace)
            // set value for storage List
            this.storageList = [];
            this.storageList = this.getStorageList();
          }
        });
      }
    });
  }

  /**
   * get UsedSpace
   */
  getUsedSpace(){
    this.usedSpace = this.totalSpace - this.freeBytes;
  }

  /**
   * get space proportion
   */
  getSpaceProportion() {
    this.proportion = new Number((this.usedSpace / this.totalSpace * 100).toFixed(0)).valueOf();
    this.usedSpaceList.push(this.proportion);
  }

  /**
   * Get storage List
   */
  getStorageList() {
    let storageList =  StorageModel.getStorageItemList();
    for (let key in storageList) {
      LogUtil.info(ConfigData.TAG + 'Storage getStorageList key:' + key);
      switch (key) {
        case Space.USED_SPACE:
          storageList[key].settingIcon = $r('sys.color.ohos_id_color_palette11');
          storageList[key].settingTitle = $r('app.string.usedSpace');
          storageList[key].settingValue = this.usedSpace;
          break;
        case Space.REMAINING_SPACE:
          storageList[key].settingIcon = $r('app.color.color_666666_grey');
          storageList[key].settingTitle = $r('app.string.remainingSpace');
          storageList[key].settingValue = this.freeBytes;
          break;
      }
    }
    LogUtil.info(ConfigData.TAG + 'Storage getStorageList storageTitleValue out:' + JSON.stringify(storageList));
    return storageList;
  }

  /**
   * format data
   */
  formatData(val) {
    let result = '';
    if (val < 1024) {
      result = `${new Number(val).valueOf()} B`;
    } else if (val < 1024 * 1024) {
      result = `${(new Number(val / 1024).valueOf()).toFixed(2)} KB`;
    } else if (val < 1024 * 1024 * 1024) {
      result = `${(new Number((val / (1024 * 1024))).valueOf()).toFixed(2)} MB`;
    } else if (val < 1024 * 1024 * 1024 * 1024) {
      result = `${(new Number((val / (1024 * 1024 * 1024))).valueOf()).toFixed(2)} GB`;
    }
    return result;
  }
}