/*
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

#include "base_test.h"

#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "contacts_path.h"
#include "test_common.h"

namespace Contacts {
namespace Test {
BaseTest::BaseTest()
{
    OHOS::Contacts::ContactsPath::RDB_PATH = DataPath::RDB_PATH;
    OHOS::Contacts::ContactsPath::RDB_BACKUP_PATH = DataPath::RDB_BACKUP_PATH;
    OHOS::Contacts::ContactsPath::DUMP_PATH = DataPath::DUMP_PATH;
}

BaseTest::~BaseTest()
{
}

int BaseTest::ContactsRand()
{
    int fd = 0;
    int randNum = 0;
    int result;
    fd = open("/dev/urandom", O_RDONLY);
    read(fd, &result, sizeof(randNum));
    close(fd);
    return result;
}

void BaseTest::InitAbility()
{
    std::shared_ptr<OHOS::AppExecFwk::AbilityInfo> abilityInfo = std::make_shared<OHOS::AppExecFwk::AbilityInfo>();
    abilityInfo->name = "AbilityClassName";
    abilityInfo->type = OHOS::AppExecFwk::AbilityType::DATA;
    abilityInfo->isNativeAbility = true;
    std::shared_ptr<OHOS::AppExecFwk::OHOSApplication> application;
    std::shared_ptr<OHOS::AppExecFwk::AbilityHandler> handler;
    calllogAbility.Init(abilityInfo, application, handler, nullptr);
    voicemailAbility.Init(abilityInfo, application, handler, nullptr);
    contactsDataAbility.Init(abilityInfo, application, handler, nullptr);
}

/**
 * @brief check values and resultSet value
 *
 * @param rawContactValues of data source
 * @param resultSet of database
 * @param test name
 */
void BaseTest::CheckResultSet(OHOS::NativeRdb::ValuesBucket &values,
    const std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> &resultSet, std::string testName)
{
    std::vector<std::string> columnNames;
    resultSet->GetAllColumnNames(columnNames);
    int resultSetNum = resultSet->GoToFirstRow();
    while (resultSetNum == OHOS::NativeRdb::E_OK) {
        int size = columnNames.size();
        for (int i = 0; i < size; i++) {
            CheckData(values, resultSet, columnNames[i], testName);
        }
        break;
    }
    resultSet->Close();
}

void BaseTest::CheckData(OHOS::NativeRdb::ValuesBucket &values,
    const std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> &resultSet, std::string &columnName,
    std::string testName)
{
    std::string typeValue = columnName;
    int columnIndex = 0;
    resultSet->GetColumnIndex(typeValue, columnIndex);
    OHOS::NativeRdb::ColumnType columnType;
    resultSet->GetColumnType(columnIndex, columnType);
    // Compare values and resultSet column value  equality
    if (columnType == OHOS::NativeRdb::ColumnType::TYPE_INTEGER) {
        int resultSetIntValue = 0;
        resultSet->GetInt(columnIndex, resultSetIntValue);
        if (values.HasColumn(typeValue)) {
            int valuesIntValue = 0;
            OHOS::NativeRdb::ValueObject valuesObject;
            values.GetObject(typeValue, valuesObject);
            valuesObject.GetInt(valuesIntValue);
            std::string tempName = testName;
            tempName.append("CheckResultSet columnName : %{public}s insertValue = %{public}d  ");
            tempName.append("databaseValue = %{public}d");
            HILOG_INFO(tempName.c_str(), typeValue.c_str(), valuesIntValue, resultSetIntValue);
            EXPECT_EQ(resultSetIntValue, valuesIntValue);
        }
    } else if (columnType == OHOS::NativeRdb::ColumnType::TYPE_STRING) {
        std::string resultSetStringValue;
        std::string valuesStringValue;
        resultSet->GetString(columnIndex, resultSetStringValue);
        if (values.HasColumn(typeValue)) {
            OHOS::NativeRdb::ValueObject valuesObject;
            values.GetObject(typeValue, valuesObject);
            valuesObject.GetString(valuesStringValue);
            std::string tempName = testName;
            tempName.append("CheckResultSet columnName : %{public}s insertValue = %{public}s  ");
            tempName.append("databaseValue = %{public}s");
            HILOG_INFO(tempName.c_str(), typeValue.c_str(), valuesStringValue.c_str(), resultSetStringValue.c_str());
            EXPECT_EQ(resultSetStringValue, valuesStringValue);
        }
    }
}

/**
 * @brief check values and resultSet value
 *
 * @param values of data source
 * @param resultSet of database
 */
void BaseTest::CheckResultSetList(std::vector<OHOS::NativeRdb::ValuesBucket> &valuesVector,
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> &resultSet, std::string testName)
{
    std::vector<std::string> columnNames;
    resultSet->GetAllColumnNames(columnNames);
    int resultSetNum = resultSet->GoToFirstRow();
    int index = 0;
    while (resultSetNum == OHOS::NativeRdb::E_OK) {
        OHOS::NativeRdb::ValuesBucket values = valuesVector[index];
        int size = columnNames.size();
        for (int i = 0; i < size; i++) {
            CheckData(valuesVector[index], resultSet, columnNames[i], testName);
        }
        resultSetNum = resultSet->GoToNextRow();
        index++;
    }
    resultSet->Close();
}
} // namespace Test
} // namespace Contacts