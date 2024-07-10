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

#include "calllogfuzzyquery_test.h"

#include "test_common.h"

namespace Contacts {
namespace Test {
CalllogFuzzyQueryTest::CalllogFuzzyQueryTest()
{
}

CalllogFuzzyQueryTest::~CalllogFuzzyQueryTest()
{
}

int CalllogFuzzyQueryTest::CalllogDelete(OHOS::NativeRdb::DataAbilityPredicates predicates)
{
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    int code = calllogAbility.Delete(uriCalllog, predicates);
    return code;
}

std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> CalllogFuzzyQueryTest::CalllogQuery(
    std::vector<std::string> columns, OHOS::NativeRdb::DataAbilityPredicates predicates)
{
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet =
        calllogAbility.Query(uriCalllog, columns, predicates);
    return resultSet;
}

int64_t CalllogFuzzyQueryTest::CalllogInsertValues(OHOS::NativeRdb::ValuesBucket &values)
{
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    int64_t code = calllogAbility.Insert(uriCalllog, values);
    return code;
}

void CalllogFuzzyQueryTest::ClearCallLog()
{
    // clear all callLog data
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    int deleteCode = CalllogDelete(predicates);
    EXPECT_EQ(deleteCode, 0);
}

/**
 * @tc.number  calllog_Query_test_100
 * @tc.name    Fuzzy query according to the beginning of mobile phone number
 * @tc.desc    Query call records of mobile phone numbers that start with 123
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogFuzzyQueryTest, calllog_Query_test_100, testing::ext::TestSize.Level1)
{
    HILOG_INFO("----calllog_Query_test_100 is starting!----");
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "123456789");
    int rawId = CalllogInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);

    valuesBucket.PutInt("id", rawId);
    OHOS::NativeRdb::ValuesBucket valuesBucketTwo;
    valuesBucketTwo.PutString("phone_number", "1842794123");
    rawId = CalllogInsertValues(valuesBucketTwo);
    valuesBucketTwo.PutInt("id", rawId);
    EXPECT_GT(rawId, 0);

    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.Like("phone_number", "123%");
    predicates.OrderByAsc("id");
    std::vector<OHOS::NativeRdb::ValuesBucket> valuesBucketVector;
    valuesBucketVector.push_back(valuesBucket);
    valuesBucketVector.push_back(valuesBucketTwo);
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    // resultSet count 1
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    int currentCount = 1;
    EXPECT_EQ(currentCount, rowCount);
    CheckResultSetList(valuesBucketVector, resultSet, "calllog_Query_test_100");
    resultSet->Close();
    ClearCallLog();
}

/**
 * @tc.number  calllog_Query_test_200
 * @tc.name    Fuzzy query according to the end of mobile phone number
 * @tc.desc    Query call records with mobile phone number ending in 0
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogFuzzyQueryTest, calllog_Query_test_200, testing::ext::TestSize.Level1)
{
    HILOG_INFO("----calllog_Query_test_200 is starting!----");
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "1854250750");
    int rawId = CalllogInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);
    valuesBucket.PutInt("id", rawId);
    OHOS::NativeRdb::ValuesBucket valuesBucketTwo;
    valuesBucketTwo.PutString("phone_number", "17542368015");
    rawId = CalllogInsertValues(valuesBucketTwo);
    valuesBucketTwo.PutInt("id", rawId);
    EXPECT_GT(rawId, 0);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.Like("phone_number", "%750");
    predicates.OrderByAsc("id");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    // resultSet count 1
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    int currentCount = 1;
    EXPECT_EQ(currentCount, rowCount);
    CheckResultSet(valuesBucket, resultSet, "calllog_Query_test_200");
    resultSet->Close();
    ClearCallLog();
}

/**
 * @tc.number  calllog_Query_test_300
 * @tc.name    Fuzzy query according to the middle digits of the mobile phone number
 * @tc.desc    Query the call history of the mobile phone number starting with 123 and ending with 8910
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogFuzzyQueryTest, calllog_Query_test_300, testing::ext::TestSize.Level1)
{
    HILOG_INFO("----calllog_Query_test_300 is starting!----");
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "12378418910");
    int rawId = CalllogInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);
    valuesBucket.PutInt("id", rawId);
    OHOS::NativeRdb::ValuesBucket valuesBucketTwo;
    valuesBucketTwo.PutString("phone_number", "12374589961");
    rawId = CalllogInsertValues(valuesBucketTwo);
    valuesBucketTwo.PutInt("id", rawId);
    EXPECT_GT(rawId, 0);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.Like("phone_number", "1238910516156465%");
    predicates.OrderByAsc("id");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    // resultSet count 0
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    resultSet->Close();
    ClearCallLog();
}

/**
 * @tc.number  calllog_Query_test_400
 * @tc.name    Fuzzy query according to the fields contained in the mobile phone number
 * @tc.desc    Query the call history of the mobile phone number starting with 123
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogFuzzyQueryTest, calllog_Query_test_400, testing::ext::TestSize.Level1)
{
    HILOG_INFO("----calllog_Query_test_400 is starting!----");
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "17778418910");
    int rawId = CalllogInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);
    valuesBucket.PutInt("id", rawId);
    OHOS::NativeRdb::ValuesBucket valuesBucketTwo;
    valuesBucketTwo.PutString("phone_number", "12378451245");
    rawId = CalllogInsertValues(valuesBucketTwo);
    valuesBucketTwo.PutInt("id", rawId);
    EXPECT_GT(rawId, 0);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.Like("phone_number", "%37845%");
    predicates.OrderByAsc("id");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    // resultSet count 0
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(valuesBucketTwo, resultSet, "calllog_Query_test_400");
    resultSet->Close();
    ClearCallLog();
}

/**
 * @tc.number  abnormal_calllog_Query_test_500
 * @tc.name    An error field is passed in during fuzzy query based on mobile phone number
 * @tc.desc    Query the call history of the mobile phone number starting with 123 and ending with 8910
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(CalllogFuzzyQueryTest, abnormal_calllog_Query_test_500, testing::ext::TestSize.Level1)
{
    HILOG_INFO("----abnormal_calllog_Query_test_500 is starting!----");
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "12378418910");
    int rawId = CalllogInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);

    valuesBucket.PutInt("id", rawId);
    OHOS::NativeRdb::ValuesBucket valuesBucketTwo;
    valuesBucketTwo.PutString("phone_number", "12374589961");
    rawId = CalllogInsertValues(valuesBucketTwo);
    valuesBucketTwo.PutInt("id", rawId);
    EXPECT_GT(rawId, 0);

    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.Like("phone_numbers", "1238910516156465%");
    predicates.OrderByAsc("id");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = CalllogQuery(columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(-1, rowCount);
    resultSet->Close();
    ClearCallLog();
}
} // namespace Test
} // namespace Contacts