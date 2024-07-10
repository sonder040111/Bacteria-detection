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
import LogUtil from '../../../../../../../../common/utils/src/main/ets/default/baseUtil/LogUtil';
import Bundle from '@ohos.bundle';
import ResMgr from '@ohos.resourceManager';
import appManager from '@ohos.application.appManager'
import ConfigData from '../../../../../../../../common/utils/src/main/ets/default/baseUtil/ConfigData';
import {LogAll} from '../../../../../../../../common/utils/src/main/ets/default/baseUtil/LogDecorator';

const INDEX = 0;
const IS_INCLUDE_ABILITY_INFO = 0;
const URI_PAGE = 'pages/applicationInfo';
let icon_arrow = $r('app.media.ic_settings_arrow');
let icon_default = $r('app.media.icon_default');

const MODULE_TAG = ConfigData.TAG + 'application-> ';

/**
 * app management service class
 */
@LogAll
export class AppManagementModel extends BaseModel {
  private mBundleInfoList: any[]= [];
 /**
  * get application data
  */
  setAppManagementListener() {
    LogUtil.info('settings AppManagementModel getIconItem ResMgr:' + JSON.stringify(ResMgr));
    Bundle.getAllBundleInfo(IS_INCLUDE_ABILITY_INFO)
      .then((data) => {
      LogUtil.info('settings AppManagementModel setAppManagementListener getBundleInfos() start ');
      LogUtil.info('settings AppManagementModel data.length: ' + data.length + ' data: ' + JSON.stringify(data));
      this.getResourceItem(INDEX, data.length, data);
    });
    LogUtil.info('settings appManagement init AppManagementModel setAppManagementListener end');
  }

  /**
   * get resource information according to resource id
   *
   * @param index - array position
   * @param count - array length
   * @param data - data
   */
  getResourceItem(index, count, data) {
    LogUtil.info('settings AppManagementModel getIconItem start data.length' + data.length);
    let imageValue = '';
    let label = '';
    let that = this;
    LogUtil.info('settings AppManagementModel data[index].name :' + data[index].name);
    try {
      ResMgr.getResourceManager(data[index].name, (error, item) => {
        LogUtil.info('settings AppManagementModel getResourceManager item' + JSON.stringify(item) + "|error:" + error);
        let appInfo = data[index].appInfo;
        LogUtil.info('settings AppManagementModel getResourceManager appInfo.labelId:' + JSON.stringify(appInfo));
        LogUtil.info('settings AppManagementModel labelId:' + appInfo.labelId + "|iconId:" + appInfo.iconId);
        if (appInfo.labelId > 0) {
          item.getString(appInfo.labelId, (error, value) => {
            LogUtil.info('settings AppManagementModel getResourceManager  value.length:' + value.length);
            if (value != null) {
              LogUtil.info('settings AppManagementModel getResourceManager getString() value:' + value);
              label = value;
              LogUtil.info('settings AppManagementModel getResourceManager getString() label:' + label);
            } else {
              LogUtil.info('settings AppManagementModel getResourceManager getString() error:' + error);
            }
          });
        } else {
          LogUtil.info('settings AppManagementModel getResourceManager getString() label:' + appInfo.label);
          label = appInfo.label;
        }
        LogUtil.info('settings AppManagementModel getResourceManager getString() finish label:' + label);
        if (appInfo.iconId > 0) {
          item.getMediaBase64(appInfo.iconId, (error, value) => {
            LogUtil.info('settings AppManagementModel getMediaBase64 error:' + error);
            if (error != null) {
              LogUtil.info('settings AppManagementModel getMediaBase64 error!=null:' + error);
              this.mBundleInfoList.push({
                settingIcon: icon_default,
                settingTitle: label,
                settingValue: '',
                settingArrow: icon_arrow,
                settingSummary: data[index].versionName,
                settingBundleName: data[index].name,
                settingIconId: appInfo.iconId,
                settingUri: URI_PAGE
              });
              if (count - 1 > index) {
                LogUtil.info('settings AppManagementModel getMediaBase64() id=0:' + index + ' | count:' + count);
                index = index + 1;
                that.getResourceItem(index, count, data);
              } else {
                LogUtil.info('settings AppManagementModel getMediaBase64() id=0:' + index + ' | count:' + count);
                LogUtil.info('settings AppManagementModel mBundleInfoList[i]: ' + JSON.stringify(this.mBundleInfoList));
                AppStorage.SetOrCreate('appManagementList', this.mBundleInfoList);
              }
            }
            LogUtil.info('settings AppManagementModel getResourceManager getMediaBase64() :' + value.length);
            if (value.length > 0) {
              imageValue = value;
              LogUtil.info('settings AppManagementModel getResourceManager getMediaBase64() imageValue:' + imageValue);
            }
            LogUtil.info('settings AppManagementModel getResourceManager getMediaBase64() end');
            this.mBundleInfoList.push({
              settingIcon: imageValue,
              settingTitle: label,
              settingValue: '',
              settingArrow: icon_arrow,
              settingSummary: data[index].versionName,
              settingBundleName: data[index].name,
              settingIconId: appInfo.iconId,
              settingUri: URI_PAGE
            });
            if (count - 1 > index) {
              LogUtil.info('settings AppManagementModel getMediaBase64() if index:' + index + ' | count:' + count);
              index = index + 1;
              that.getResourceItem(index, count, data);
            } else {
              LogUtil.info('settings AppManagementModel getMediaBase64() else index:' + index + ' | count:' + count);
              LogUtil.info('settings AppManagementModel mBundleInfoList[i]: ' + JSON.stringify(this.mBundleInfoList));
              AppStorage.SetOrCreate('appManagementList', this.mBundleInfoList);
            }
          });
        } else {
          if (count - 1 > index) {
            LogUtil.info('settings AppManagementModel getMediaBase64() id=0:' + index + ' | count:' + count);
            index = index + 1;
            that.getResourceItem(index, count, data);
          } else {
            LogUtil.info('settings AppManagementModel getMediaBase64() id=0:' + index + ' | count:' + count);
            LogUtil.info('settings AppManagementModel mBundleInfoList[i]: ' + JSON.stringify(this.mBundleInfoList));
            AppStorage.SetOrCreate('appManagementList', this.mBundleInfoList);
          }
        }
      });
    } catch (error) {
      LogUtil.info('settings AppManagementModel catch error:' + error);
    }
    LogUtil.info('settings appManagement AppManagementModel getIconItem end');
  }

  /**
   * Clear up application data by bundle name
   * @param bundleName bundle name
   */
  clearUpApplicationData(bundleName: string, callback) {
    appManager.clearUpApplicationData(bundleName, callback);
  }

  /**
   * Clears cache data of a specified application.
   * @param bundleName bundle name
   */
  cleanBundleCacheFiles(bundleName: string, callback) {
    Bundle.cleanBundleCacheFiles(bundleName, callback);
  }

  /**
   * Uninstall an application.
   * @param bundleName bundle name
   */
   async uninstall(bundleName: string, callback) {
    LogUtil.info(ConfigData.TAG + "start uninstall in model");
    const bundlerInstaller = await Bundle.getBundleInstaller();
    LogUtil.info(ConfigData.TAG + "get bundlerInstaller : " + typeof bundlerInstaller);
    bundlerInstaller.uninstall(bundleName, {
        param: { userId: 0, installFlag: 0, isKeepData: false }
      },
      (result) => { callback(result); });
    LogUtil.info(ConfigData.TAG + "end uninstall in model");
  }

  getBundleInfo(bundleName, callback) {
    LogUtil.info(MODULE_TAG + 'start get bundle info');
    Bundle.getBundleInfo(bundleName, Bundle.BundleFlag.GET_APPLICATION_INFO_WITH_PERMISSION, callback);
    LogUtil.info(MODULE_TAG + 'end get bundle info');
  }

  /**
   * Kill processes by bundle name
   * @param bundleName bundle name
   */
  killProcessesByBundleName(bundleName: string, callback) {
    appManager.killProcessesByBundleName(bundleName, callback);
  }

}

let appManagementModel = new AppManagementModel();

export default appManagementModel as AppManagementModel
;