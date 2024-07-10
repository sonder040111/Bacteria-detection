# 联系人数据库子系统
# applications_contactsdata

-   [简介](#section1166054159366)
-   [架构图](#section1619419895966)
-   [目录](#section161941989596)
-   [相关仓](#section1371113476307)

## 简介<a name="section1166054159366"></a>

联系人数据库应用是每一个用户管理联系人数据库的不可缺少的一个管理信息应用，
它的内容对于用户管理者来说是至关重要的，所以联系人数据库应该能够为每一个  
用户的管理提供充足的信息和快捷查询手段，大大方便用户合理的管理联系人数据库信息。  

## 架构图<a name="section1619419895966"></a>

![](figures/Image_architecture.png)

## 目录<a name="section161941989596"></a>

```
/foundation/contactsdataability/
├── ability
│   ├── account                        #账户管理
│   ├── checkpermission                #权限检测
│   ├── common                         #公共方法:log,utils等
│   ├── datadisasterrecovery           #数据损坏恢复
│   ├── merge                          #联系人合并
│   └── sinicization                   #汉字转拼音相关
├── contacts                            #NAPI
├── dataBusiness
│   ├── calllog                        #通话记录
│   ├── contacts                       #联系人
│   ├── quicksearch                    #快速检索
│   └── voicemail                      #语音信箱
├── test                               #测试目录
├── BUILD.gn
└── ohos.build
```
## 使用说明

### 概述

提供联系人数据和通话记录以及语音信箱增、删、改、查、等接口。
接口参数主要有string类型的uri和对象类型的value、DataAbilityPredicates

使用功能接口前，需要导入相关模块。

```js
import featureAbility from '@ohos.ability.featureAbility';
import ohos_data_ability from '@ohos.data.dataability';
```
### 通话记录

#### DAHelper.insert(uri: string, value: ValuesBucket)

- 接口说明

  通话记录插入

- insert参数描述

  | 名称     | 读写属性 | 类型                   | 必填 | 描述                           |
  | -------- | -------- | ---------------------- | ---- | ------------------------------ |
  | uri      | 只读     | string                 | 是   | 具体操作的uri     |
  | value    | 只读     | ValuesBucket           | 是   | 数据库字段key-value对象 |

返回值为 通话记录id

更新示例
```js
import featureAbility from '@ohos.ability.featureAbility';
import ohos_data_ability from '@ohos.data.dataability';
var  calllogDAhelperUri= "dataability:///com.ohos.calllogability";
var calllogUri = "dataability:///com.ohos.calllogability/calls/calllog";
var value = {"phone_number" : "xxxxx","display_name" : "xxx"};
// 获取DAHelper
var DAHelper = featureAbility.acquireDataAbilityHelper(calllogDAhelperUri);
// 调用insert方法
DAHelper.insert(calllogUri, value).then((data) => {
	console.info("calllogId = " + data);
});
```
#### DAHelper.update(uri: string, value: ValuesBucket, condition: DataAbilityPredicates)

- 接口说明

  通话记录修改

- update参数描述

  | 名称         | 读写属性 | 类型                    | 必填 | 描述                           |
  | --------     | -------- | ---------------------- | ---- | ------------------------------ |
  | uri          | 只读     | string                 | 是   | 具体操作的uri          |
  | value        | 只读     | ValuesBucket           | 是   | 数据库字段key-value对象 |
  | condition    | 只读     | DataAbilityPredicates  | 是   | 更新条件            |

返回值为 成功为0否则为-1

修改示例
```js
import featureAbility from '@ohos.ability.featureAbility';
import ohos_data_ability from '@ohos.data.dataability';
var  calllogDAhelperUri= "dataability:///com.ohos.calllogability";
var calllogUri = "dataability:///com.ohos.calllogability/calls/calllog";
var value = {"phone_number" : "xxxxx","display_name" : "xxx"};
// 获取DAHelper
var DAHelper = featureAbility.acquireDataAbilityHelper(calllogDAhelperUri);
// 条件参数
var condition = new ohos_data_ability.DataAbilityPredicates();
condition.equalTo("id", "xxx");
// 调用update方法
DAHelper.update(calllogUri, value, condition).then((data) => {
	console.info("resultCode = " + data);
});
```

#### DAHelper.delete(uri: string, condition: DataAbilityPredicates)

- 接口说明

  通话记录删除

- delete参数描述

  | 名称         | 读写属性 | 类型                    | 必填 | 描述                           |
  | --------     | -------- | ---------------------- | ---- | ------------------------------ |
  | uri          | 只读     | string                 | 是   | 具体操作的uri          |
  | condition    | 只读     | DataAbilityPredicates  | 是   | 删除条件            |

返回值为 成功为0否则为-1

删除示例
```js
import featureAbility from '@ohos.ability.featureAbility';
import ohos_data_ability from '@ohos.data.dataability';
var  calllogDAhelperUri= "dataability:///com.ohos.calllogability";
var calllogUri = "dataability:///com.ohos.calllogability/calls/calllog";
// 获取DAHelper
var DAHelper = featureAbility.acquireDataAbilityHelper(calllogDAhelperUri);
// 条件参数
var condition = new ohos_data_ability.DataAbilityPredicates();
condition.equalTo("id", "xxx");
// 调用delete方法
DAHelper.delete(calllogUri, condition).then((data) => {
	console.info("deleteCode = " + data);
});
```

#### DAHelper.query(uri: string, resultColumns: array, condition: DataAbilityPredicates)

- 接口说明

  通话记录查询

- 查询参数描述

  | 名称             | 读写属性 | 类型                    | 必填  | 描述                           |
  | --------         | -------- | ---------------------- | ----  | ------------------------------ |
  | uri              | 只读     | string                 | 是    | 具体操作的uri        |
  | resultColumns    | 只读     | array                   | 是   | 需要查询的列字段名称   |
  | condition        | 只读     | DataAbilityPredicates  | 是    | 查询条件            |

返回值为 ResultSet 查询结果集

查询示例
```js
import featureAbility from '@ohos.ability.featureAbility';
import ohos_data_ability from '@ohos.data.dataability';
var  calllogDAhelperUri= "dataability:///com.ohos.calllogability";
var calllogUri = "dataability:///com.ohos.calllogability/calls/calllog";
// 获取DAHelper
var DAHelper = featureAbility.acquireDataAbilityHelper(calllogDAhelperUri);
// 条件参数
var condition = new ohos_data_ability.DataAbilityPredicates();
condition.equalTo("id", "xxx");
var resultColumns = [ "id", "display_name", "phone_number"];
// 调用query方法
DAHelper.query(calllogUri, resultColumns, condition).then((data) => {
    if (resultSet.goToFirstRow()) {
        do {
            var displayName = resultSet.getString(resultSet.getColumnIndex("display_name"));
            var id = resultSet.getInt(resultSet.getColumnIndex("id"));
            var phoneNumber = resultSet.getString(resultSet.getColumnIndex("phone_number"));
        } while (resultSet.goToNextRow());
    }
    resultSet.close();	
});
```
返回值为 ResultSet 查询结果集

### 语音信箱

#### DAHelper.insert(uri: string, value: ValuesBucket)

- 接口说明

  语音信箱插入

- insert参数描述

  | 名称     | 读写属性 | 类型                   | 必填 | 描述                           |
  | -------- | -------- | ---------------------- | ---- | ------------------------------ |
  | uri      | 只读     | string                 | 是   | 具体操作的uri     |
  | value    | 只读     | ValuesBucket           | 是   | 数据库字段key-value对象 |

返回值为 语音信箱表id

更新示例
```js
import featureAbility from '@ohos.ability.featureAbility';
import ohos_data_ability from '@ohos.data.dataability';
var  voicemailDAhelperUri= "dataability:///com.ohos.voicemailability";
var voicemaiUri = "dataability:///com.ohos.voicemailability/calls/voicemail";
var value = {"phone_number" : "xxxxx","display_name" : "xxx"};
// 获取DAHelper
var DAHelper = featureAbility.acquireDataAbilityHelper(voicemailDAhelperUri);
// 调用insert方法
DAHelper.insert(voicemaiUri, value).then((data) => {
	console.info("calllogId = " + data);
});
```
#### DAHelper.update(uri: string, value: ValuesBucket, condition:DataAbilityPredicates)

- 接口说明

  语音信箱修改

- update参数描述

  | 名称         | 读写属性 | 类型                    | 必填 | 描述                           |
  | --------     | -------- | ---------------------- | ---- | ------------------------------ |
  | uri          | 只读     | string                 | 是   | 具体操作的uri          |
  | value        | 只读     | ValuesBucket           | 是   | 数据库字段key-value对象 |
  | condition    | 只读     | DataAbilityPredicates  | 是   | 更新条件            |

返回值为 成功为0否则为-1

修改示例
```js
import featureAbility from '@ohos.ability.featureAbility';
import ohos_data_ability from '@ohos.data.dataability';
var  voicemailDAhelperUri= "dataability:///com.ohos.voicemailability";
var voicemaiUri = "dataability:///com.ohos.voicemailability/calls/voicemail";
var value = {"phone_number" : "xxxxx","display_name" : "xxx"};
// 获取DAHelper
var DAHelper = featureAbility.acquireDataAbilityHelper(voicemailDAhelperUri);
// 条件参数
var condition = new ohos_data_ability.DataAbilityPredicates();
condition.equalTo("id", "xxx");
// 调用update方法
DAHelper.update(voicemaiUri, value, condition).then((data) => {
	console.info("resultCode = " + data);
});
```

#### DAHelper.delete(uri: string, condition: DataAbilityPredicates)

- 接口说明

  语音信箱删除

- delete参数描述

  | 名称         | 读写属性 | 类型                    | 必填 | 描述                           |
  | --------     | -------- | ---------------------- | ---- | ------------------------------ |
  | uri          | 只读     | string                 | 是   | 具体操作的uri          |
  | condition    | 只读     | DataAbilityPredicates  | 是   | 删除条件            |

返回值为 成功为0否则为-1

删除示例
```js
import featureAbility from '@ohos.ability.featureAbility';
import ohos_data_ability from '@ohos.data.dataability';
var  voicemailDAhelperUri= "dataability:///com.ohos.voicemailability";
var voicemaiUri = "dataability:///com.ohos.voicemailability/calls/voicemail";
// 获取DAHelper
var DAHelper = featureAbility.acquireDataAbilityHelper(voicemailDAhelperUri);
// 条件参数
var condition = new ohos_data_ability.DataAbilityPredicates();
condition.equalTo("id", "xxx");
// 调用delete方法
DAHelper.delete(voicemaiUri, condition).then((data) => {
	console.info("deleteCode = " + data);
});
```

#### DAHelper.query(uri: string, resultColumns:array, condition: DataAbilityPredicates)

- 接口说明

  语音信箱查询

- 查询参数描述

  | 名称             | 读写属性 | 类型                    | 必填  | 描述                           |
  | --------         | -------- | ---------------------- | ----  | ------------------------------ |
  | uri              | 只读     | string                 | 是    | 具体操作的uri        |
  | resultColumns    | 只读     | array                   | 是   | 需要查询的列字段名称   |
  | condition        | 只读     | DataAbilityPredicates  | 是    | 查询条件            |

返回值为 ResultSet 查询结果集

查询示例
```js
import featureAbility from '@ohos.ability.featureAbility';
import ohos_data_ability from '@ohos.data.dataability';
var  voicemailDAhelperUri= "dataability:///com.ohos.voicemailability";
var voicemaiUri = "dataability:///com.ohos.voicemailability/calls/voicemail";
// 获取DAHelper
var DAHelper = featureAbility.acquireDataAbilityHelper(calllogDAhelperUri);
// 条件参数
var condition = new ohos_data_ability.DataAbilityPredicates();
condition.equalTo("id", "xxx");
var resultColumns = [ "id", "display_name", "phone_number"];
// 调用query方法
DAHelper.query(voicemaiUri, resultColumns, condition).then((data) => {
    if (resultSet.goToFirstRow()) {
        do {
            var displayName = resultSet.getString(resultSet.getColumnIndex("display_name"));
            var id = resultSet.getInt(resultSet.getColumnIndex("id"));
            var phoneNumber = resultSet.getString(resultSet.getColumnIndex("phone_number"));
        } while (resultSet.goToNextRow());
    }
    resultSet.close();	
});
```

### 联系人

#### DAHelper.insert(uri: string, value: ValuesBucket)

- 接口说明

  联系人插入

- insert参数描述

  | 名称     | 读写属性 | 类型                   | 必填 | 描述                           |
  | -------- | -------- | ---------------------- | ---- | ------------------------------ |
  | uri      | 只读     | string                 | 是   | 具体操作的uri     |
  | value    | 只读     | ValuesBucket           | 是   | 数据库字段key-value对象 |

返回值为 联系人数据表id

更新示例
```js
import featureAbility from '@ohos.ability.featureAbility';
import ohos_data_ability from '@ohos.data.dataability';
var  contactsUri= "dataability:///com.ohos.contactsdataability";
var rawContactUri  = "dataability:///com.ohos.contactsdataability/contacts/raw_contact";
var contactDataUri  = "dataability:///com.ohos.contactsdataability/contacts/contact_data";
var value = {"display_name" : "xxx"};
// 获取DAHelper
var DAHelper = featureAbility.acquireDataAbilityHelper(contactsUri);
// 调用insert方法
DAHelper.insert(rawContactUri, value).then((rawContactId) => {
	console.info("rawContactId = " + rawContactId);
    // 联系人详细信息插入
    var contactDataValues = {
        "raw_contact_id" : rawContactId,
        "content_type" : "name",
        "detail_info" : "xxxxxxx"
    };
   DAHelper.insert(contactDataUri, contactDataValues).then((contactDataId) => {
      console.info("rawContactId = " + contactDataId);
   };
});
```
#### DAHelper.update(uri: string, value: ValuesBucket, condition: DataAbilityPredicates)

- 接口说明

  联系人修改

- update参数描述

  | 名称         | 读写属性 | 类型                    | 必填 | 描述                           |
  | --------     | -------- | ---------------------- | ---- | ------------------------------ |
  | uri          | 只读     | string                 | 是   | 具体操作的uri          |
  | value        | 只读     | ValuesBucket           | 是   | 数据库字段key-value对象 |
  | condition    | 只读     | DataAbilityPredicates  | 是   | 更新条件            |

返回值为 成功为0否则为-1

修改示例
```js
import featureAbility from '@ohos.ability.featureAbility';
import ohos_data_ability from '@ohos.data.dataability';
var  contactsUri= "dataability:///com.ohos.contactsdataability";
var rawContactUri  = "dataability:///com.ohos.contactsdataability/contacts/raw_contact";
var contactDataUri  = "dataability:///com.ohos.contactsdataability/contacts/contact_data";
var value = {"display_name" : "xxx"};
// 获取DAHelper
var DAHelper = featureAbility.acquireDataAbilityHelper(contactsUri);
// 条件参数
var condition = new ohos_data_ability.DataAbilityPredicates();
condition.equalTo("id", "xxx");
// 调用update方法
DAHelper.update(rawContactUri, value, condition).then((data) => {
	console.info("resultCode = " + data);
});
```

#### DAHelper.delete(uri: string, condition: DataAbilityPredicates)

- 接口说明

  联系人删除

- delete参数描述

  | 名称         | 读写属性 | 类型                    | 必填 | 描述                           |
  | --------     | -------- | ---------------------- | ---- | ------------------------------ |
  | uri          | 只读     | string                 | 是   | 具体操作的uri          |
  | condition    | 只读     | DataAbilityPredicates  | 是   | 删除条件            |

返回值为 成功为0否则为-1

删除示例
```js
import featureAbility from '@ohos.ability.featureAbility';
import ohos_data_ability from '@ohos.data.dataability';
var  contactsUri= "dataability:///com.ohos.contactsdataability";
var rawContactUri  = "dataability:///com.ohos.contactsdataability/contacts/raw_contact";
var contactDataUri  = "dataability:///com.ohos.contactsdataability/contacts/contact_data";
// 获取DAHelper
var DAHelper = featureAbility.acquireDataAbilityHelper(contactsUri);
// 条件参数
var condition = new ohos_data_ability.DataAbilityPredicates();
condition.equalTo("id", "xxx");
// 调用delete方法
DAHelper.delete(contactDataUri, condition).then((data) => {
	console.info("deleteCode = " + data);
});
```
#### DAHelper.query(uri: string, resultColumns: array, condition: DataAbilityPredicates)

- 接口说明

  联系人查询

- 查询参数描述

  | 名称             | 读写属性 | 类型                    | 必填  | 描述                           |
  | --------         | -------- | ---------------------- | ----  | ------------------------------ |
  | uri              | 只读     | string                 | 是    | 具体操作的uri        |
  | resultColumns    | 只读     | array                   | 是   | 需要查询的列字段名称   |
  | condition        | 只读     | DataAbilityPredicates  | 是    | 查询条件            |

返回值为 ResultSet 查询结果集

查询示例
```js
import featureAbility from '@ohos.ability.featureAbility';
import ohos_data_ability from '@ohos.data.dataability';
var  contactsUri= "dataability:///com.ohos.contactsdataability";
var rawContactUri  = "dataability:///com.ohos.contactsdataability/contacts/raw_contact";
// 获取DAHelper
var DAHelper = featureAbility.acquireDataAbilityHelper(contactsUri);
// 条件参数
var condition = new ohos_data_ability.DataAbilityPredicates();
condition.equalTo("id", "xxx");
var resultColumns = [ "id", "display_name"];
// 调用query方法
DAHelper.query(rawContactUri, resultColumns, condition).then((data) => {
    if (resultSet.goToFirstRow()) {
        do {
            var displayName = resultSet.getString(resultSet.getColumnIndex("display_name"));
        } while (resultSet.goToNextRow());
    }
    resultSet.close();	
});
```
| 系统公共uri                                                                       |
| ---------------------------------------------------------                        | 
| dataability:///com.ohos.contactsdataability                                      | 
| dataability:///com.ohos.contactsdataability/contacts/contact                     | 
| dataability:///com.ohos.contactsdataability/contacts/raw_contact                 | 
| dataability:///com.ohos.contactsdataability/contacts/contact_data                |
| dataability:///com.ohos.contactsdataability/contacts/raw_contact/query_merge_list| 
| dataability:///com.ohos.contactsdataability/contacts/raw_contact/split_contact   |
| dataability:///com.ohos.contactsdataability/contacts/raw_contact/manual_merge    |
| dataability:///com.ohos.contactsdataability/contacts/raw_contact/auto_merge      |
| dataability:///com.ohos.contactsdataability/contacts/contact_type                |
| dataability:///com.ohos.contactsdataability/contacts/groups                      |
| dataability:///com.ohos.contactsdataability/contacts/photo_files                 |  
| dataability:///com.ohos.contactsdataability/contacts/contact_blocklist           | 
| dataability:///com.ohos.contactsdataability/contacts/deleted_raw_contact         | 
| dataability:///com.ohos.contactsdataability/contacts/search_contact              | 
| dataability:///com.ohos.contactsdataability/contacts/deleted_raw_contact         | 
| dataability:///com.ohos.contactsdataability/contacts/deleted_raw_contact_record  |
| dataability:///com.ohos.contactsdataability/contacts/backup                      |
| dataability:///com.ohos.contactsdataability/contacts/recover                     |
| dataability:///com.ohos.contactsdataability/profile/contact                      | 
| dataability:///com.ohos.contactsdataability/profile/raw_contact                  | 
| dataability:///com.ohos.contactsdataability/profile/contact_data                 |
| dataability:///com.ohos.contactsdataability/profile/raw_contact/query_merge_list | 
| dataability:///com.ohos.contactsdataability/profile/raw_contact/split_contact    |
| dataability:///com.ohos.contactsdataability/profile/raw_contact/manual_merge     |
| dataability:///com.ohos.contactsdataability/profile/raw_contact/auto_merge       |
| dataability:///com.ohos.contactsdataability/profile/contact_type                 |
| dataability:///com.ohos.contactsdataability/profile/groups                       |
| dataability:///com.ohos.contactsdataability/profile/photo_files                  |  
| dataability:///com.ohos.contactsdataability/profile/contact_blocklist            | 
| dataability:///com.ohos.contactsdataability/profile/deleted_raw_contact          | 
| dataability:///com.ohos.contactsdataability/profile/search_contact               | 
| dataability:///com.ohos.contactsdataability/profile/deleted_raw_contact          | 
| dataability:///com.ohos.contactsdataability/profile/deleted_raw_contact_record   |
| dataability:///com.ohos.contactsdataability/profile/backup                       |
| dataability:///com.ohos.contactsdataability/profile/recover                      |    
| dataability:///com.ohos.calllogability                                           | 
| dataability:///com.ohos.calllogability/calls/calllog                             |
| dataability:///com.ohos.voicemailability                                         | 
| dataability:///com.ohos.voicemailability/calls/voicemail                         | 
## 相关仓<a name="section1371113476307"></a>

系统应用

applications_contactsdata