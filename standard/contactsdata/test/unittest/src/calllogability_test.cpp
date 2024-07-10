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

#include "calllogability_test.h"

#include "data_ability_operation_builder.h"

namespace Contacts {
namespace Test {
CalllogAbilityTest::CalllogAbilityTest()
{
}

CalllogAbilityTest::~CalllogAbilityTest()
{
}

int64_t CalllogAbilityTest::CalllogInsert(std::string phoneNumber)
{
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    OHOS::NativeRdb::ValuesBucket calllogValues;
    calllogValues.PutString("phone_number", phoneNumber);
    int64_t code = calllogAbility.Insert(uriCalllog, calllogValues);
    calllogValues.Clear();
    return code;
}

int CalllogAbilityTest::CalllogUpdate(
    OHOS::NativeRdb::ValuesBucket updateValues, OHOS::NativeRdb::DataAbilityPredicates predicates)
{
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    int code = calllogAbility.Update(uriCalllog, updateValues, predicates);
    return code;
}

int CalllogAbilityTest::CalllogDelete(OHOS::NativeRdb::DataAbilityPredicates predicates)
{
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    int code = calllogAbility.Delete(uriCalllog, predicates);
    return code;
}

std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> CalllogAbilityTest::CalllogQuery(
    std::vector<std::string> columns, OHOS::NativeRdb::DataAbilityPredicates predicates)
{
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet =
        calllogAbility.Query(uriCalllog, columns, predicates);
    return resultSet;
}

int64_t CalllogAbilityTest::CalllogInsertValues(OHOS::NativeRdb::ValuesBucket &values)
{
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    int64_t code = calllogAbility.Insert(uriCalllog, values);
    return code;
}

int64_t CalllogAbilityTest::CalllogInsertValue(std::string displayName, OHOS::NativeRdb::ValuesBucket &values)
{
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    values.PutString("display_name", displayName);
    int64_t code = calllogAbility.Insert(uriCalllog, values);
    return code;
}

void CalllogAbilityTest::GetAllValuesColumn(std::vector<std::string> &column)
{
    column.push_back("phone_number");
    column.push_back("display_name");
    column.push_back("call_direction");
    column.push_back("voicemail_uri");
    column.push_back("sim_type");
    column.push_back("is_hd");
    column.push_back("is_read");
    column.push_back("ring_duration");
    column.push_back("talk_duration");
    column.push_back("format_number");
    column.push_back("quicksearch_key");
    column.push_back("number_type");
    column.push_back("number_type_name");
    column.push_back("begin_time");
    column.push_back("end_time");
    column.push_back("answer_state");
    column.push_back("create_time");
    column.push_back("number_location");
    column.push_back("photo_id");
    column.push_back("photo_uri");
    column.push_back("country_iso_code");
    column.push_back("extra1");
    column.push_back("extra2");
    column.push_back("extra3");
    column.push_back("extra4");
    column.push_back("extra5");
    column.push_back("extra6");
}

/**
 * @brief get ValuesBucket
 * @params columnsStart column start index
 * @params columnsEnd column end index
 * @return ValuesBucket
 */
OHOS::NativeRdb::ValuesBucket CalllogAbilityTest::GetCallLogValues(
    int columnsStart, int columnsEnd, std::vector<std::string> &columns)
{
    std::string callLogTestStringValue = std::to_string(ContactsRand());
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "154121" + callLogTestStringValue);
    valuesBucket.PutString("display_name", "name");
    valuesBucket.PutInt("call_direction", ContactsRand());
    valuesBucket.PutString("voicemail_uri", "uri::voicemail_uri" + callLogTestStringValue);
    valuesBucket.PutInt("sim_type", ContactsRand());
    valuesBucket.PutInt("is_hd", ContactsRand());
    valuesBucket.PutInt("is_read", ContactsRand());
    valuesBucket.PutInt("ring_duration", ContactsRand());
    valuesBucket.PutInt("talk_duration", ContactsRand());
    valuesBucket.PutString("format_number", "154 121" + callLogTestStringValue);
    valuesBucket.PutString("quicksearch_key", "1");
    valuesBucket.PutInt("number_type", ContactsRand());
    valuesBucket.PutString("number_type_name", "numberType" + callLogTestStringValue);
    valuesBucket.PutInt("begin_time", ContactsRand());
    valuesBucket.PutInt("end_time", ContactsRand());
    valuesBucket.PutInt("answer_state", ContactsRand());
    valuesBucket.PutInt("create_time", ContactsRand());
    valuesBucket.PutString("number_location", "location" + callLogTestStringValue);
    valuesBucket.PutInt("photo_id", ContactsRand());
    valuesBucket.PutString("photo_uri", "uri::photo_uri" + callLogTestStringValue);
    valuesBucket.PutInt("country_iso_code", ContactsRand());
    valuesBucket.PutString("extra1", "extra1" + callLogTestStringValue);
    valuesBucket.PutString("extra2", "extra2" + callLogTestStringValue);
    valuesBucket.PutString("extra3", "extra3" + callLogTestStringValue);
    valuesBucket.PutString("extra4", "extra4" + callLogTestStringValue);
    valuesBucket.PutString("extra5", "extra5" + callLogTestStringValue);
    valuesBucket.PutString("extra6", "extra6" + callLogTestStringValue);
    std::vector<std::string> columnsTemp;
    GetAllValuesColumn(columnsTemp);
    int allSize = columnsTemp.size();
    for (int i = 0; i < allSize; i++) {
        if (i >= columnsStart && i < columnsEnd) {
            columns.push_back(columnsTemp[i]);
        } else {
            valuesBucket.Delete(columnsTemp[i]);
        }
    }
    return valuesBucket;
}

void CalllogAbilityTest::ClearCallLog()
{
    // clear all callLog data
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    int deleteCode = CalllogDelete(predicates);
    EXPECT_EQ(deleteCode, 0);
}

/*
 * @tc.number  calllog_Insert_test_100
 * @tc.name    Add a single contact data and verify whether the insertion is successful
 * @tc.desc    Added ability to call record
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_Insert_test_100, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_Insert_test_100 is starting! ---");
    std::vector<std::string> columns;
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "1234567890");
    columns.push_back("phone_number");
    int rawId = CalllogInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);

    // query insert data
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawId));
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetDeleteQuery = CalllogQuery(columns, predicates);
    // resultSet count 1
    int rowCount = 0;
    resultSetDeleteQuery->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(valuesBucket, resultSetDeleteQuery, "calllog_Insert_test_100");
    ClearCallLog();
}

/*
 * @tc.number  calllog_Insert_test_200
 * @tc.name    Add full field data to the callog table and verify that the insertion was successful
 * @tc.desc    Added ability to call record
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_Insert_test_200, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----calllog_Insert_test_200 is starting!-----");
    std::vector<std::string> columns;
    GetAllValuesColumn(columns);
    int columnsSize = 27;
    OHOS::NativeRdb::ValuesBucket valuesBucket = GetCallLogValues(0, columnsSize, columns);
    int rawId = CalllogInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawId));
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    // resultSet count 1
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(valuesBucket, resultSet, "calllog_Insert_test_200");
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(rawId));
    // test end delete data
    int deleteCode = CalllogDelete(predicates);
    EXPECT_EQ(deleteCode, 0);
    ClearCallLog();
}

/*
 * @tc.number  calllog_Update_test_300
 * @tc.name    Update single contact data and verify whether the update is successful
 * @tc.desc    Support single call record update capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_Update_test_300, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_Update_test_300 is starting! ---");
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "123456789055");
    int rawId = CalllogInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);

    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutString("phone_number", "0987654321");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawId));
    int updateCode = CalllogUpdate(updateValues, predicates);
    EXPECT_EQ(0, updateCode);

    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetDeleteQuery = CalllogQuery(columns, predicates);
    // resultSet count 1
    int rowCount = 0;
    resultSetDeleteQuery->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(updateValues, resultSetDeleteQuery, "calllog_Update_test_300");
    ClearCallLog();
}

/*
 * @tc.number  calllog_Update_test_400
 * @tc.name    Update the data of all fields of callog table and verify whether the update is successful
 * @tc.desc    Support all call log update capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_Update_test_400, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----calllog_Update_test_400 is starting!-----");
    std::vector<std::string> columns;
    GetAllValuesColumn(columns);
    int columnsSize = 27;
    OHOS::NativeRdb::ValuesBucket valuesBucket = GetCallLogValues(0, columnsSize, columns);
    int rawId = CalllogInsertValues(valuesBucket);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawId));
    OHOS::NativeRdb::ValuesBucket upDateValuesBucket = GetCallLogValues(0, columnsSize, columns);
    int upDateCode = CalllogUpdate(upDateValuesBucket, predicates);
    EXPECT_EQ(upDateCode, 0);

    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);

    CheckResultSet(upDateValuesBucket, resultSet, "calllog_Update_test_400");
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(rawId));
    // test end delete data
    int deleteCode = CalllogDelete(predicates);
    EXPECT_EQ(deleteCode, 0);
    ClearCallLog();
}

/*
 * @tc.number  calllog_Update_test_500
 * @tc.name    Update all call records and verify that the update is successful
 * @tc.desc    Support all call log update capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_Update_test_500, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_Update_test_500 is starting! ---");
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "1234567890");
    valuesBucket.PutString("display_name", "testName");
    CalllogInsertValues(valuesBucket);
    CalllogInsertValues(valuesBucket);
    int rawCount = 2;
    std::vector<std::string> columns;
    columns.push_back("ring_duration");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetQuery = CalllogQuery(columns, predicates);
    int rowCurrentCount = 0;
    resultSetQuery->GetRowCount(rowCurrentCount);
    resultSetQuery->Close();
    EXPECT_EQ(rowCurrentCount, rawCount);

    // update database current data
    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutInt("ring_duration", 500);
    predicates.GreaterThan("id", "0");
    int updateCode = CalllogUpdate(updateValues, predicates);
    EXPECT_EQ(updateCode, 0);
    // check update data or dataBase
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetUpDateQuery = CalllogQuery(columns, predicates);
    int rowUpDateCount = 0;
    resultSetUpDateQuery->GetRowCount(rowUpDateCount);
    EXPECT_EQ(rowCurrentCount, rowUpDateCount);
    std::vector<OHOS::NativeRdb::ValuesBucket> upValues;
    upValues.push_back(updateValues);
    upValues.push_back(updateValues);
    CheckResultSetList(upValues, resultSetUpDateQuery, "calllog_Update_test_500");
    ClearCallLog();
}

/*
 * @tc.number  calllog_Query_test_600
 * @tc.name    Add call record data, query the data according to the returned ID and return the query result
 * @tc.desc    Support single call record query capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_Query_test_600, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_Query_test_600 is starting! ---");
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    columns.push_back("display_name");
    columns.push_back("id");
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "1234567890");
    valuesBucket.PutString("display_name", "testName");
    int rawId = CalllogInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);
    // query insert data
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawId));
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetDeleteQuery = CalllogQuery(columns, predicates);
    // resultSet count 1
    int rowCount = 0;
    resultSetDeleteQuery->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    valuesBucket.PutInt("id", rawId);
    CheckResultSet(valuesBucket, resultSetDeleteQuery, "calllog_Query_test_600");
    ClearCallLog();
}

/*
 * @tc.number  calllog_Query_test_800
 * @tc.name    Query the call record according to the combination conditions and return the query results
 * @tc.desc    Support combination condition query call record ability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_Query_test_800, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----calllog_Query_test_800 is starting!-----");
    int ringDuration = 9121215;
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "1234567890");
    valuesBucket.PutString("display_name", "testName");
    valuesBucket.PutInt("ring_duration", ringDuration);
    int rawId = CalllogInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);

    std::vector<std::string> columns;
    columns.push_back("phone_number");
    columns.push_back("display_name");
    columns.push_back("ring_duration");
    columns.push_back("id");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawId));
    predicates.And();
    predicates.EqualTo("ring_duration", std::to_string(ringDuration));
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    // resultSet count 1
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    // add id check
    valuesBucket.PutInt("id", rawId);
    CheckResultSet(valuesBucket, resultSet, "calllog_Query_test_800");
    ClearCallLog();
}

/*
 * @tc.number  calllog_Query_test_900
 * @tc.name    Query all call records and return query results
 * @tc.desc    Query all call records
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_Query_test_900, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----calllog_Query_test_900 is starting!-----");
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "12345678901254");
    valuesBucket.PutString("display_name", "testName");
    CalllogInsertValues(valuesBucket);
    CalllogInsertValues(valuesBucket);
    CalllogInsertValues(valuesBucket);
    CalllogInsertValues(valuesBucket);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    int current = 4;
    // resultSet count 4
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(current, rowCount);
    resultSet->Close();
    ClearCallLog();
}

/*
 * @tc.number  calllog_Delete_test_1000
 * @tc.name    Delete a single call record and verify whether the deletion is successful
 * @tc.desc    Call log deletion capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_Delete_test_1000, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_Delete_test_1000 is starting! ---");
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "1234567890");
    std::vector<std::string> columns;
    int rawId = CalllogInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawId));
    int deleteCode = CalllogDelete(predicates);
    EXPECT_EQ(deleteCode, 0);
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    // resultSet count
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    resultSet->Close();
    ClearCallLog();
}

/*
 * @tc.number  calllog_Delete_test_1100
 * @tc.name    Delete a single call record data and verify whether the deletion is successful
 * @tc.desc    Call log deletion capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_Delete_test_1100, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_Delete_test_1100 is starting! ---");
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "1234567890");
    int rawId = CalllogInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);

    OHOS::NativeRdb::ValuesBucket valuesBucketTwo;
    valuesBucketTwo.PutString("phone_number", "18552504057");
    int rawIdTwo = CalllogInsertValues(valuesBucketTwo);
    EXPECT_GT(rawIdTwo, 0);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawId));
    predicates.Or();
    predicates.EqualTo("id", std::to_string(rawIdTwo));
    int deleteCode = CalllogDelete(predicates);
    EXPECT_EQ(deleteCode, 0);

    std::vector<std::string> columns;
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    // resultSet count
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    resultSet->Close();
    ClearCallLog();
}

/*
 * @tc.number  calllog_Delete_test_1200
 * @tc.name    Delete the data of all fields in the callog table and verify whether the deletion is successful
 * @tc.desc    Call log deletion capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_Delete_test_1200, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----calllog_Delete_test_1200 is starting!-----");
    // insert
    std::vector<std::string> columns;
    GetAllValuesColumn(columns);
    int columnsSize = 27;
    OHOS::NativeRdb::ValuesBucket valuesBucket = GetCallLogValues(0, columnsSize, columns);
    int rawId = CalllogInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawId));
    int deleteCode = CalllogDelete(predicates);
    EXPECT_EQ(deleteCode, 0);

    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetDeleteQuery = CalllogQuery(columns, predicates);
    int rowCount = 0;
    resultSetDeleteQuery->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    resultSetDeleteQuery->Close();
    ClearCallLog();
}

/*
 * @tc.number  calllog_BatchInsert_test_1300
 * @tc.name    Batch add call record data and verify whether the insertion is successful
 * @tc.desc    Add ability to call records in batches
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_BatchInsert_test_1300, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_BatchInsert_test_1300 is starting!---");
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "test_1000_1234567890");
    valuesBucket.PutInt("ring_duration", 1000);
    OHOS::NativeRdb::ValuesBucket valuesBucketTwo;
    valuesBucketTwo.PutString("phone_number", "test_1000_0987654321");
    valuesBucketTwo.PutInt("ring_duration", 1200);
    OHOS::NativeRdb::ValuesBucket valuesBucketThree;
    valuesBucketThree.PutString("phone_number", "test_1000_1111122222");
    valuesBucketThree.PutInt("ring_duration", 1500);
    std::vector<OHOS::NativeRdb::ValuesBucket> listAddValuesBucket;
    listAddValuesBucket.push_back(valuesBucket);
    listAddValuesBucket.push_back(valuesBucketTwo);
    listAddValuesBucket.push_back(valuesBucketThree);
    OHOS::Uri batchInsertUri(CallLogUri::CALL_LOG);

    int batchInsertCode = calllogAbility.BatchInsert(batchInsertUri, listAddValuesBucket);
    EXPECT_EQ(batchInsertCode, 0);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    columns.push_back("ring_duration");
    predicates.EqualTo("phone_number", "test_1000_1234567890");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    predicates.Clear();
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);

    CheckResultSet(valuesBucket, resultSet, "calllog_BatchInsert_test_1300");
    predicates.Clear();
    predicates.EqualTo("phone_number", "test_1000_0987654321");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetTwo = CalllogQuery(columns, predicates);
    int rowCountTwo = 0;
    resultSetTwo->GetRowCount(rowCountTwo);
    EXPECT_EQ(1, rowCountTwo);

    CheckResultSet(valuesBucketTwo, resultSetTwo, "calllog_BatchInsert_test_1300");
    predicates.Clear();
    predicates.EqualTo("phone_number", "test_1000_1111122222");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetThree = CalllogQuery(columns, predicates);
    int rowCountThree = 0;
    resultSetThree->GetRowCount(rowCountThree);
    EXPECT_EQ(1, rowCountThree);
    CheckResultSet(valuesBucketThree, resultSetThree, "calllog_BatchInsert_test_1300");
    ClearCallLog();
}

/*
 * @tc.number  calllog_Delete_test_1400
 * @tc.name    Delete call record data in batch and verify whether the deletion is successful
 * @tc.desc    delete ability to call records in batches
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_Delete_test_1400, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_Delete_test_1400 is starting!---");
    OHOS::NativeRdb::ValuesBucket valuesBucketOne;
    valuesBucketOne.PutString("phone_number", "999999999Bratch");
    valuesBucketOne.PutInt("ring_duration", 998);
    int rawIdOne = CalllogInsertValues(valuesBucketOne);
    EXPECT_GT(rawIdOne, 0);
    int rawIdTwo = CalllogInsertValues(valuesBucketOne);
    EXPECT_GT(rawIdTwo, 0);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    std::shared_ptr<OHOS::NativeRdb::DataAbilityPredicates> executePredicates =
        std::make_shared<OHOS::NativeRdb::DataAbilityPredicates>(predicates);
    std::shared_ptr<Uri> uri = std::make_shared<Uri>(CallLogUri::CALL_LOG);
    std::shared_ptr<OHOS::AppExecFwk::DataAbilityOperation> operation =
        OHOS::AppExecFwk::DataAbilityOperation::NewDeleteBuilder(uri)
            ->WithPredicatesBackReference(0, 0)
            ->WithPredicates(executePredicates)
            ->WithInterruptionAllowed(true)
            ->Build();
    std::vector<std::shared_ptr<OHOS::AppExecFwk::DataAbilityOperation>> executeBatchOperations;
    executeBatchOperations.push_back(operation);
    InitAbility();
    std::vector<std::shared_ptr<OHOS::AppExecFwk::DataAbilityResult>> dataAbilityResult =
        calllogAbility.ExecuteBatch(executeBatchOperations);
    EXPECT_EQ(0, dataAbilityResult[0]->GetCount());
    std::vector<std::string> columns;
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetInsertQuery = CalllogQuery(columns, predicates);
    int rowCount = 0;
    resultSetInsertQuery->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    ClearCallLog();
}

/*
 * @tc.number  calllog_Update_test_1500
 * @tc.name    Batch update call record data and verify whether the update is successful
 * @tc.desc    update ability to call records in batches
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_Update_test_1500, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_Update_test_1500 is starting!---");
    OHOS::NativeRdb::ValuesBucket valuesBucketOne;
    valuesBucketOne.PutString("phone_number", "999999999UpdateBratch");
    valuesBucketOne.PutInt("ring_duration", 998);
    int rawIdOne = CalllogInsertValues(valuesBucketOne);
    EXPECT_GT(rawIdOne, 0);
    int rawIdTwo = CalllogInsertValues(valuesBucketOne);
    EXPECT_GT(rawIdTwo, 0);
    OHOS::NativeRdb::ValuesBucket calllogValues;
    calllogValues.PutString("phone_number", "12345");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawIdOne));
    predicates.Or();
    predicates.EqualTo("id", std::to_string(rawIdTwo));
    std::shared_ptr<OHOS::NativeRdb::ValuesBucket> values =
        std::make_shared<OHOS::NativeRdb::ValuesBucket>(calllogValues);
    std::shared_ptr<OHOS::NativeRdb::DataAbilityPredicates> executePredicates =
        std::make_shared<OHOS::NativeRdb::DataAbilityPredicates>(predicates);
    std::shared_ptr<Uri> uri = std::make_shared<Uri>(CallLogUri::CALL_LOG);
    std::shared_ptr<OHOS::AppExecFwk::DataAbilityOperation> operation =
        OHOS::AppExecFwk::DataAbilityOperation::NewUpdateBuilder(uri)
            ->WithValuesBucket(values)
            ->WithPredicatesBackReference(0, 0)
            ->WithPredicates(executePredicates)
            ->WithInterruptionAllowed(true)
            ->Build();
    std::vector<std::shared_ptr<OHOS::AppExecFwk::DataAbilityOperation>> executeBatchOperations;
    executeBatchOperations.push_back(operation);
    InitAbility();
    std::vector<std::shared_ptr<OHOS::AppExecFwk::DataAbilityResult>> dataAbilityResult =
        calllogAbility.ExecuteBatch(executeBatchOperations);
    EXPECT_EQ(0, dataAbilityResult[0]->GetCount());
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetInsertQuery = CalllogQuery(columns, predicates);
    int rowCount = 0;
    resultSetInsertQuery->GetRowCount(rowCount);
    EXPECT_EQ(2, rowCount);
    std::vector<OHOS::NativeRdb::ValuesBucket> listValue;
    listValue.push_back(calllogValues);
    listValue.push_back(calllogValues);
    CheckResultSetList(listValue, resultSetInsertQuery, "calllog_Update_test_1500");
    ClearCallLog();
}

/*
 * @tc.number  abnormal_calllog_Insert_test_1600
 * @tc.name    When adding individual contact data, a non-existent field is passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, abnormal_calllog_Insert_test_1600, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----abnormal_calllog_Insert_test_1600 is starting!-----");

    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    OHOS::Uri errorUri(CallLogUri::ERROR_URI);
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    OHOS::NativeRdb::ValuesBucket calllogValues;
    calllogValues.PutString("phone_numbers", "998877");
    int calllogId = calllogAbility.Insert(uriCalllog, calllogValues);
    EXPECT_EQ(calllogId, -1);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("phone_numbers", "998877");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(-1, rowCount);

    resultSet->Close();
    calllogValues.Clear();
    calllogValues.PutString("phone_number", "998877");
    calllogId = calllogAbility.Insert(errorUri, calllogValues);
    EXPECT_EQ(calllogId, -1);

    calllogValues.Clear();
    predicates.EqualTo("phone_number", "998877");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetUriError = CalllogQuery(columns, predicates);
    int rowCountUriError = 0;
    resultSetUriError->GetRowCount(rowCountUriError);
    EXPECT_EQ(-1, rowCountUriError);
    resultSetUriError->Close();
    ClearCallLog();
}

/*
 * @tc.number  abnormal_calllog_Update_test_1700
 * @tc.name    Update a single call record and pass in non-existent fields
 * @tc.desc    Updated ability to call record
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, abnormal_calllog_Update_test_1700, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_calllog_Update_test_1700 is starting! ---");
    std::vector<std::string> columns;
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "1234567890");
    valuesBucket.PutString("display_name", "abnormal_sigle");
    columns.push_back("phone_number");
    columns.push_back("display_name");
    int rawId = CalllogInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);

    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutString("phone_number_sha", "0987654321");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawId));
    int updateCode = CalllogUpdate(updateValues, predicates);
    EXPECT_EQ(-1, updateCode);

    // query insert data
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetInsertQuery = CalllogQuery(columns, predicates);
    // resultSet count 1
    int rowCount = 0;
    resultSetInsertQuery->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(valuesBucket, resultSetInsertQuery, "abnormal_calllog_Update_test_1700");
    ClearCallLog();
}

/*
 * @tc.number  abnormal_calllog_Update_test_1800
 * @tc.name    Update all call records and pass in a non-existent field
 * @tc.desc    Updated ability to call record
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, abnormal_calllog_Update_test_1800, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_calllog_Update_test_1800 is starting! ---");
    std::vector<std::string> columns;
    OHOS::NativeRdb::ValuesBucket valuesBucketOne;
    valuesBucketOne.PutString("phone_number", "1234567890");
    valuesBucketOne.PutInt("ring_duration", 998);
    int rawIdOne = CalllogInsertValues(valuesBucketOne);
    EXPECT_GT(rawIdOne, 0);

    OHOS::NativeRdb::ValuesBucket valuesBucketTwo;
    valuesBucketTwo.PutString("phone_number", "638292093737");
    valuesBucketTwo.PutInt("ring_duration", 999);
    int rawIdTwo = CalllogInsertValues(valuesBucketTwo);
    EXPECT_GT(rawIdTwo, 0);

    columns.push_back("phone_number");
    columns.push_back("ring_duration");

    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutString("phone_number", "0987654321");
    updateValues.PutInt("ring_duration", 888);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    predicates.And();
    predicates.EqualTo("ring_duration_shs", "999");
    int updateCode = CalllogUpdate(updateValues, predicates);
    EXPECT_EQ(-1, updateCode);

    // query insert data
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(rawIdOne));
    predicates.Or();
    predicates.EqualTo("id", std::to_string(rawIdTwo));
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetInsertQuery = CalllogQuery(columns, predicates);

    // resultSet count 2
    int rowCount = 0;
    resultSetInsertQuery->GetRowCount(rowCount);
    EXPECT_EQ(2, rowCount);
    std::vector<OHOS::NativeRdb::ValuesBucket> listValue;
    listValue.push_back(valuesBucketOne);
    listValue.push_back(valuesBucketTwo);
    CheckResultSetList(listValue, resultSetInsertQuery, "abnormal_calllog_Update_test_1800");
    ClearCallLog();
}

/*
 * @tc.number  abnormal_calllog_Query_test_1900
 * @tc.name    When querying the call record, a non-existent data is passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, abnormal_calllog_Query_test_1900, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----abnormal_calllog_Query_test_1900 is starting!-----");
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", "100000000");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    // resultSet count
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    resultSet->Close();
    ClearCallLog();
}

/*
 * @tc.number  abnormal_calllog_Query_test_2000
 * @tc.name    An incorrect table name was passed in when querying the call record
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, abnormal_calllog_Query_test_2000, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----abnormal_calllog_Query_test_2000 is starting!-----");
    OHOS::Uri uriCalllogs(CallLogUri::ERROR_URI);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet =
        calllogAbility.Query(uriCalllogs, columns, predicates);
    EXPECT_EQ(resultSet, nullptr);
    ClearCallLog();
}

/*
 * @tc.number  abnormal_calllog_Query_test_2100
 * @tc.name    When querying the call record according to the combination conditions,
 *             the wrong field name and nonexistent data are passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, abnormal_calllog_Query_test_2100, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----abnormal_calllog_Query_test_2100 is starting!-----");

    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("ids", "0");
    predicates.And();
    predicates.LessThan("id", "20");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet =
        calllogAbility.Query(uriCalllog, columns, predicates);
    // resultSet count
    int rowCount = -1;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(-1, rowCount);
    resultSet->Close();
    ClearCallLog();
}

/*
 * @tc.number  abnormal_calllog_Query_test_2200
 * @tc.name    An error field is passed in when querying all call records
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, abnormal_calllog_Query_test_2200, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----abnormal_calllog_Query_test_2200 is starting!-----");
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");

    int deleteCode = calllogAbility.Delete(uriCalllog, predicates);
    EXPECT_EQ(deleteCode, 0);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_numbers");
    predicates.Clear();
    predicates.GreaterThan("id", "0");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    EXPECT_NE(resultSet, nullptr);
    ClearCallLog();
}

/*
 * @tc.number  abnormal_calllog_Delete_test_2300
 * @tc.name    An error field is passed in when deleting a single call record
 * @tc.desc    Added ability to call record
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, abnormal_calllog_Delete_test_2300, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----abnormal_calllog_Delete_test_2300 is starting!-----");
    std::vector<std::string> columns;
    GetAllValuesColumn(columns);
    int columnsSize = 27;
    OHOS::NativeRdb::ValuesBucket valuesBucket = GetCallLogValues(0, columnsSize, columns);
    int rawId = CalllogInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawId));
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(valuesBucket, resultSet, "abnormal_calllog_Delete_test_2300");
    predicates.Clear();
    predicates.EqualTo("ids", std::to_string(rawId));
    // test end delete data
    int deleteCode = CalllogDelete(predicates);
    EXPECT_EQ(deleteCode, -1);

    predicates.Clear();
    predicates.EqualTo("id", std::to_string(rawId));
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetDelete = CalllogQuery(columns, predicates);
    // resultSet count 1
    int rowCountDelete = 0;
    resultSetDelete->GetRowCount(rowCountDelete);
    EXPECT_EQ(1, rowCountDelete);
    CheckResultSet(valuesBucket, resultSetDelete, "abnormal_calllog_Delete_test_2300");
    ClearCallLog();
}

/*
 * @tc.number  abnormal_calllog_BatchInsert_test_2400
 * @tc.name    Batch call records are added and several of them fail. See the processing logic
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, abnormal_calllog_BatchInsert_test_2400, testing::ext::TestSize.Level1)
{
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    int deleteCode = CalllogDelete(predicates);
    EXPECT_EQ(deleteCode, 0);
    HILOG_INFO("--- abnormal_calllog_BatchInsert_test_2400 is starting! ---");

    OHOS::Uri uriData(CallLogUri::CALL_LOG);
    OHOS::NativeRdb::ValuesBucket calllogValues;
    std::vector<OHOS::NativeRdb::ValuesBucket> listAddBluk;
    int batchInserCode = 0;
    for (int i = 0; i < 10; i++) {
        listAddBluk.clear();
        for (int j = 10 * i + 1; j <= 10 * (i + 1); j++) {
            calllogValues.Clear();
            if (j == 14 || j == 27 || j == 57) {
                calllogValues.PutString("phone_numbers", std::to_string(j));
            } else {
                calllogValues.PutString("phone_number", std::to_string(j));
            }
            listAddBluk.push_back(calllogValues);
        }
        batchInserCode = calllogAbility.BatchInsert(uriData, listAddBluk);
        HILOG_INFO("abnormal_calllog_BatchInsert_test_2400 : batchInserCode = %{public}d", batchInserCode);
        if (batchInserCode == 0) {
            HILOG_INFO("abnormal_calllog_BatchInsert_test_2400 batch insert success!");
            EXPECT_EQ(batchInserCode, 0);
        } else {
            HILOG_INFO("abnormal_calllog_BatchInsert_test_2400 batch insert fail!");
            EXPECT_EQ(batchInserCode, -1);
        }
    }
    ClearCallLog();
}

/*
 * @tc.number  abnormal_calllog_BatchInsert_test_2500
 * @tc.name    Add 1000 pieces of data in batch to make article 500 fail. Check the subsequent processing logic
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, abnormal_calllog_BatchInsert_test_2500, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_calllog_BatchInsert_test_2500 is starting! ---");

    OHOS::Uri uriData(CallLogUri::CALL_LOG);
    OHOS::NativeRdb::ValuesBucket calllogValues;
    std::vector<OHOS::NativeRdb::ValuesBucket> listAddBluk;
    int batchInserCode = 0;
    for (int i = 0; i < 1000; i++) {
        calllogValues.Clear();
        if (i == 500) {
            calllogValues.PutString("phone_numbers", std::to_string(i + 1));
        } else {
            calllogValues.PutString("phone_number", std::to_string(i + 1));
        }
        listAddBluk.push_back(calllogValues);
    }
    batchInserCode = calllogAbility.BatchInsert(uriData, listAddBluk);
    if (batchInserCode == 0) {
        HILOG_INFO("abnormal_calllog_BatchInsert_test_2500 batch insert success!");
        EXPECT_EQ(batchInserCode, 0);
    } else {
        HILOG_INFO("abnormal_calllog_BatchInsert_test_2500 batch insert fail!");
        EXPECT_EQ(batchInserCode, -1);
    }
    ClearCallLog();
}

/*
 * @tc.number  abnormal_calllog_delete_test_2600
 * @tc.name    Delete call record data in batch, and several of them fail. Check the processing logic
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, abnormal_calllog_delete_test_2600, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_calllog_delete_test_2600 is starting! ---");

    OHOS::Uri errorUriCalllogs(CallLogUri::ERROR_URI);
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "1234567890");
    int calllogIdOne = CalllogInsertValues(valuesBucket);
    EXPECT_GT(calllogIdOne, 0);
    OHOS::NativeRdb::ValuesBucket valuesBucketTwo;
    valuesBucketTwo.PutString("phone_number", "1234567890");
    int calllogIdTwo = CalllogInsertValues(valuesBucketTwo);
    EXPECT_GT(calllogIdTwo, 0);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("ids", std::to_string(calllogIdOne));
    int deleteCode = CalllogDelete(predicates);
    EXPECT_EQ(deleteCode, -1);
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetOne = CalllogQuery(columns, predicates);
    int rowCountOne = 0;
    resultSetOne->GetRowCount(rowCountOne);
    EXPECT_EQ(-1, rowCountOne);

    CheckResultSet(valuesBucket, resultSetOne, "abnormal_calllog_delete_test_2600");
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(calllogIdTwo));
    deleteCode = calllogAbility.Delete(errorUriCalllogs, predicates);
    EXPECT_EQ(deleteCode, -1);
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetTwo = CalllogQuery(columns, predicates);
    int rowCountTwo = 0;
    resultSetTwo->GetRowCount(rowCountTwo);
    EXPECT_EQ(1, rowCountTwo);
    resultSetTwo->Close();
    ClearCallLog();
}

/*
 * @tc.number  abnormal_calllog_Update_test_2700
 * @tc.name    When batch call record is modified, the wrong field or table name is passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, abnormal_calllog_Update_test_2700, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_calllog_Update_test_2700 is starting! ---");

    OHOS::Uri errorUri(CallLogUri::ERROR_URI);
    int64_t calllogIdOne = CalllogInsert("1234567890");
    EXPECT_GT(calllogIdOne, 0);
    int64_t calllogIdTwo = CalllogInsert("0987654321");
    EXPECT_GT(calllogIdTwo, 0);

    OHOS::NativeRdb::ValuesBucket updateCalllogValues;
    updateCalllogValues.PutString("phone_number", "998877");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("ids", std::to_string(calllogIdOne));
    int updateCode = CalllogUpdate(updateCalllogValues, predicates);
    EXPECT_EQ(updateCode, -1);
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    predicates.Clear();
    predicates.EqualTo("phone_number", "998877");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetOne = CalllogQuery(columns, predicates);
    int rowCountOne = 0;
    resultSetOne->GetRowCount(rowCountOne);
    EXPECT_EQ(0, rowCountOne);

    resultSetOne->Close();
    updateCalllogValues.Clear();
    updateCalllogValues.PutString("phone_number", "987654");
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(calllogIdTwo));
    updateCode = calllogAbility.Update(errorUri, updateCalllogValues, predicates);
    EXPECT_EQ(updateCode, -1);

    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetTwo = CalllogQuery(columns, predicates);
    int rowCountTwo = 0;
    resultSetTwo->GetRowCount(rowCountOne);
    EXPECT_EQ(0, rowCountTwo);
    OHOS::NativeRdb::ValuesBucket oldValue;
    oldValue.PutString("phone_number", "0987654321");
    CheckResultSet(oldValue, resultSetTwo, "abnormal_calllog_Update_test_2700");
    ClearCallLog();
}

/*
 * @tc.number  calllog_async_insert_test_2800
 * @tc.name    Add callLog async
 * @tc.desc    Ability to join or add calllog
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_async_insert_test_2800, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_async_insert_test_2800 is staring! ---");
    std::map<int, OHOS::NativeRdb::ValuesBucket> result;
    std::vector<CallLogAsync *> callLogAsyncVector;
    int threadNum = 6;
    for (int i = 0; i < threadNum; ++i) {
        OHOS::NativeRdb::ValuesBucket values;
        std::string name;
        name.append("asyncTest");
        name.append(std::to_string(i));
        values.PutString("display_name", name);
        CallLogAsync *contactAsync = new CallLogAsync(values, result);
        std::thread asyncThread(&CallLogAsync::Insert, contactAsync);
        callLogAsyncVector.push_back(contactAsync);
        asyncThread.detach();
    }
    std::chrono::milliseconds dura(Time::ASYNC_SLEEP_TIME);
    std::this_thread::sleep_for(dura);
    std::vector<std::string> columns;
    columns.push_back("display_name");
    std::map<int, OHOS::NativeRdb::ValuesBucket>::iterator it;
    for (it = result.begin(); it != result.end(); it++) {
        OHOS::NativeRdb::DataAbilityPredicates predicates;
        predicates.EqualTo("id", std::to_string(it->first));
        std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
        CheckResultSet(it->second, resultSet, "calllog_async_insert_test_2800");
    }
    for (int i = 0; i < threadNum; ++i) {
        delete callLogAsyncVector[i];
    }
    ClearCallLog();
}

/*
 * @tc.number  calllog_async_update_test_2900
 * @tc.name    update calllog async
 * @tc.desc    Ability to join or update calllog
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_async_update_test_2900, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_async_update_test_2900 is staring! ---");
    OHOS::NativeRdb::ValuesBucket callValues;
    std::vector<int64_t> callLogId;
    callLogId.push_back(CalllogInsertValue("async_update1", callValues));
    callValues.Clear();
    callLogId.push_back(CalllogInsertValue("async_update2", callValues));
    callValues.Clear();
    callLogId.push_back(CalllogInsertValue("async_update3", callValues));
    callValues.Clear();
    callLogId.push_back(CalllogInsertValue("async_update4", callValues));
    callValues.Clear();
    callLogId.push_back(CalllogInsertValue("async_update5", callValues));
    callValues.Clear();
    callLogId.push_back(CalllogInsertValue("async_update6", callValues));
    callValues.Clear();
    std::vector<OHOS::NativeRdb::ValuesBucket> upDateValues;
    OHOS::NativeRdb::DataAbilityPredicates queryPredicates;
    std::vector<CallLogAsync *> callLogAsyncVector;
    int size = callLogId.size();
    for (int i = 0; i < size; ++i) {
        OHOS::NativeRdb::ValuesBucket values;
        std::string name;
        name.append("asyncTest");
        values.PutString("display_name", name);
        upDateValues.push_back(values);
        int id = callLogId[i];
        CallLogAsync *contactAsync = new CallLogAsync(values, id);
        callLogAsyncVector.push_back(contactAsync);
        std::thread asyncThread(&CallLogAsync::Update, contactAsync);
        asyncThread.detach();
        queryPredicates.EqualTo("id", std::to_string(callLogId[i]));
        if (i < size - 1) {
            queryPredicates.Or();
        }
    }
    std::chrono::milliseconds dura(Time::ASYNC_SLEEP_TIME);
    std::this_thread::sleep_for(dura);
    std::vector<std::string> columns;
    columns.push_back("display_name");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, queryPredicates);
    CheckResultSetList(upDateValues, resultSet, "calllog_async_update_test_2900");
    for (int i = 0; i < size; ++i) {
        delete callLogAsyncVector[i];
    }
    ClearCallLog();
}

/*
 * @tc.number  calllog_async_query_test_3000
 * @tc.name    query calllog async
 * @tc.desc    Ability to join or query
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_async_query_test_3000, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_async_query_test_3000 is staring! ---");
    OHOS::NativeRdb::ValuesBucket callLogValues;
    std::vector<int64_t> callLogId;
    callLogId.push_back(CalllogInsertValue("async_query1", callLogValues));
    callLogValues.Clear();
    callLogId.push_back(CalllogInsertValue("async_query2", callLogValues));
    callLogValues.Clear();
    callLogId.push_back(CalllogInsertValue("async_query3", callLogValues));
    callLogValues.Clear();
    callLogId.push_back(CalllogInsertValue("async_query4", callLogValues));
    callLogValues.Clear();
    callLogId.push_back(CalllogInsertValue("async_query5", callLogValues));
    callLogValues.Clear();
    callLogId.push_back(CalllogInsertValue("async_query6", callLogValues));
    callLogValues.Clear();
    std::vector<CallLogAsync *> callLogAsyncVector;
    std::vector<std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet>> resultSetVector;
    int threadNum = 6;
    for (int i = 0; i < threadNum; ++i) {
        CallLogAsync *contactAsync = new CallLogAsync(resultSetVector, callLogId);
        callLogAsyncVector.push_back(contactAsync);
        std::thread asyncThread(&CallLogAsync::Query, contactAsync);
        asyncThread.detach();
    }
    int queryCount = 6;
    int size = resultSetVector.size();
    for (int i = 0; i < size; ++i) {
        int rowCount = 0;
        resultSetVector[i]->GetRowCount(rowCount);
        EXPECT_EQ(queryCount, rowCount);
        resultSetVector[i]->Close();
    }
    for (int i = 0; i < size; ++i) {
        delete callLogAsyncVector[i];
    }
    ClearCallLog();
}

/*
 * @tc.number  contact_async_update_test_7300
 * @tc.name    delete calllog async
 * @tc.desc    Ability to join or delete calllog
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogAbilityTest, calllog_async_delete_test_3100, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_async_delete_test_3100 is staring! ---");
    OHOS::NativeRdb::ValuesBucket callLogValues;
    std::vector<int64_t> callLogId;
    callLogId.push_back(CalllogInsertValue("async_update1", callLogValues));
    callLogValues.Clear();
    callLogId.push_back(CalllogInsertValue("async_update2", callLogValues));
    callLogValues.Clear();
    callLogId.push_back(CalllogInsertValue("async_update3", callLogValues));
    callLogValues.Clear();
    callLogId.push_back(CalllogInsertValue("async_update4", callLogValues));
    callLogValues.Clear();
    callLogId.push_back(CalllogInsertValue("async_update5", callLogValues));
    callLogValues.Clear();
    callLogId.push_back(CalllogInsertValue("async_update6", callLogValues));
    callLogValues.Clear();
    int size = callLogId.size();
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.BeginWrap();
    std::vector<CallLogAsync *> callLogAsyncVector;
    for (int i = 0; i < size; ++i) {
        int id = callLogId[i];
        CallLogAsync *contactAsync = new CallLogAsync(id);
        callLogAsyncVector.push_back(contactAsync);
        std::thread asyncThread(&CallLogAsync::Delete, contactAsync);
        asyncThread.detach();
        predicates.EqualTo("id", std::to_string(id));
        if (i < size - 1) {
            predicates.Or();
        }
    }
    predicates.EndWrap();
    std::chrono::milliseconds dura(Time::ASYNC_SLEEP_TIME);
    std::this_thread::sleep_for(dura);
    std::vector<std::string> columns;
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    resultSet->Close();
    for (int i = 0; i < size; ++i) {
        delete callLogAsyncVector[i];
    }
    ClearCallLog();
}
} // namespace Test
} // namespace Contacts
