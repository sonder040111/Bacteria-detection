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

#include "voicemailability_test.h"

#include "data_ability_operation_builder.h"

namespace Contacts {
namespace Test {
VoicemailAbilityTest::VoicemailAbilityTest()
{
}

VoicemailAbilityTest::~VoicemailAbilityTest()
{
}

int64_t VoicemailAbilityTest::VoicemailStatusInsert(std::string phoneNumber, int status)
{
    OHOS::Uri uriVoicemail(VoicemailUri::VOICEMAIL);
    OHOS::NativeRdb::ValuesBucket voicemailValues;
    voicemailValues.PutString("phone_number", phoneNumber);
    voicemailValues.PutInt("voice_status", status);
    int64_t code = voicemailAbility.Insert(uriVoicemail, voicemailValues);
    return code;
}

int VoicemailAbilityTest::VoicemailUpdate(
    OHOS::NativeRdb::ValuesBucket updateValues, OHOS::NativeRdb::DataAbilityPredicates predicates)
{
    OHOS::Uri uriVoicemail(VoicemailUri::VOICEMAIL);
    int code = voicemailAbility.Update(uriVoicemail, updateValues, predicates);
    return code;
}

int VoicemailAbilityTest::VoicemailDelete(OHOS::NativeRdb::DataAbilityPredicates predicates)
{
    OHOS::Uri uriVoicemail(VoicemailUri::VOICEMAIL);
    int code = voicemailAbility.Delete(uriVoicemail, predicates);
    return code;
}

std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> VoicemailAbilityTest::VoicemailQuery(
    std::vector<std::string> columns, OHOS::NativeRdb::DataAbilityPredicates predicates)
{
    OHOS::Uri uriVoicemail(VoicemailUri::VOICEMAIL);
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet =
        voicemailAbility.Query(uriVoicemail, columns, predicates);
    return resultSet;
}

void VoicemailAbilityTest::CheckResultSetBatch(OHOS::NativeRdb::DataAbilityPredicates predicates,
    std::vector<std::string> &columns, OHOS::NativeRdb::ValuesBucket values, std::string testName)
{
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(values, resultSet, testName);
}

int VoicemailAbilityTest::VoicemailInsertValues(OHOS::NativeRdb::ValuesBucket &values)
{
    OHOS::Uri uriVoicemail(VoicemailUri::VOICEMAIL);
    int64_t code = voicemailAbility.Insert(uriVoicemail, values);
    int result = code;
    return result;
}

int64_t VoicemailAbilityTest::VoicemailInsertValue(std::string displayName, OHOS::NativeRdb::ValuesBucket &values)
{
    OHOS::Uri uriVoicemail(VoicemailUri::VOICEMAIL);
    values.PutString("display_name", displayName);
    int64_t code = voicemailAbility.Insert(uriVoicemail, values);
    return code;
}

OHOS::NativeRdb::ValuesBucket VoicemailAbilityTest::GetVoiceMailValues(
    int columnsStart, int columnsEnd, std::vector<std::string> &columns)
{
    std::string voiceMailTestStringValue = std::to_string(ContactsRand());
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    valuesBucket.PutString("phone_number", "15412121" + voiceMailTestStringValue);
    valuesBucket.PutString("quicksearch_key", "dfquicksearch_key" + voiceMailTestStringValue);
    valuesBucket.PutString("display_name", "dfName" + voiceMailTestStringValue);
    valuesBucket.PutString("voicemail_uri", "dfUri::voicemail_uri" + voiceMailTestStringValue);
    valuesBucket.PutInt("voicemail_type", ContactsRand());
    valuesBucket.PutInt("voice_file_size", ContactsRand());
    valuesBucket.PutInt("voice_duration", ContactsRand());
    valuesBucket.PutInt("voice_status", ContactsRand());
    valuesBucket.PutString("origin_type", "origin_type" + voiceMailTestStringValue);
    valuesBucket.PutInt("create_time", ContactsRand());
    std::vector<std::string> columnsTemp;
    GetVoiceMailValuesColumn(columnsTemp);
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

void VoicemailAbilityTest::GetVoiceMailValuesColumn(std::vector<std::string> &columns)
{
    columns.push_back("phone_number");
    columns.push_back("quicksearch_key");
    columns.push_back("display_name");
    columns.push_back("voicemail_uri");
    columns.push_back("voicemail_type");
    columns.push_back("voice_file_size");
    columns.push_back("voice_duration");
    columns.push_back("voice_status");
    columns.push_back("origin_type");
    columns.push_back("create_time");
}

void VoicemailAbilityTest::ClearVoicemail()
{
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    int deleteCode = VoicemailDelete(predicates);
    EXPECT_EQ(0, deleteCode);
}

/*
 * @tc.number  voicemail_Insert_test_100
 * @tc.name    Add a voice mailbox with status 1 and verify that the insertion was successful
 * @tc.desc    Added ability to voice mailbox status
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Insert_test_100, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Insert_test_100 is starting! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "166320");
    values.PutInt("voice_status", 1);
    int voicemailId = VoicemailInsertValues(values);
    HILOG_INFO("voicemail_Insert_test_100 : voicemailId = %{public}d", voicemailId);
    EXPECT_GT(voicemailId, 0);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(voicemailId));
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    columns.push_back("voice_status");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(values, result, "voicemail_Insert_test_100");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Insert_test_200
 * @tc.name    Add a single piece of voicemail data and verify whether the insertion is successful
 * @tc.desc    New capabilities for voicemail data
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Insert_test_200, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Insert_test_200 is staring! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "154236");
    int voicemailId = VoicemailInsertValues(values);
    HILOG_INFO("voicemail_Insert_test_200 : voicemailId = %{public}d", voicemailId);
    EXPECT_GT(voicemailId, 0);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(voicemailId));
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    columns.push_back("id");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(values, result, "voicemail_Insert_test_200");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Insert_test_300
 * @tc.name    Add a full field data to voicemail and verify that the insertion was successful
 * @tc.desc    Added ability to call record
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Insert_test_300, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----voicemail_Insert_test_300 is starting!-----");
    std::vector<std::string> columns;
    GetVoiceMailValuesColumn(columns);
    int columnsSize = 10;
    OHOS::NativeRdb::ValuesBucket valuesBucket = GetVoiceMailValues(0, columnsSize, columns);
    int rawId = VoicemailInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawId));
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = VoicemailQuery(columns, predicates);
    // resultSet count 1
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);

    CheckResultSet(valuesBucket, resultSet, "voicemail_Insert_test_300");
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(rawId));
    // test end delete data
    int deleteCode = VoicemailDelete(predicates);
    EXPECT_EQ(deleteCode, 0);
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Update_test_400
 * @tc.name    Delete the voicemail status and verify that the deletion was successful
 * @tc.desc    Voicemail status deletion capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Update_test_400, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Update_test_400 is starting! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "4396");
    values.PutInt("voice_status", 1);
    int voicemailId = VoicemailInsertValues(values);
    EXPECT_GT(voicemailId, 0);

    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutInt("voice_status", 0);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(voicemailId));
    int updateCode = VoicemailUpdate(updateValues, predicates);
    EXPECT_EQ(updateCode, 0);

    predicates.Clear();
    predicates.EqualTo("voice_status", "0");
    predicates.And();
    predicates.EqualTo("id", std::to_string(voicemailId));
    std::vector<std::string> columns;
    columns.push_back("voice_status");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(updateValues, result, "voicemail_Update_test_400");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Delete_test_500
 * @tc.name    Delete a single voice mailbox data and verify whether the deletion is successful
 * @tc.desc    Voicemail data deletion capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Delete_test_500, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Delete_test_500 is starting! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "154236");
    int voicemailId = VoicemailInsertValues(values);
    HILOG_INFO("voicemail_Delete_test_500 : voicemailId = %{public}d", voicemailId);
    EXPECT_GT(voicemailId, 0);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(voicemailId));
    int deleteCode = VoicemailDelete(predicates);
    HILOG_INFO("voicemail_Delete_test_500: deleteCode = %{public}d", deleteCode);
    EXPECT_EQ(deleteCode, 0);

    predicates.Clear();
    predicates.EqualTo("id", std::to_string(voicemailId));
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    columns.push_back("id");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    result->Close();
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Delete_test_600
 * @tc.name    Insert 3 pieces of voicemail data first, then delete 2 of them, and verify whether the deletion is
 * successful
 * @tc.desc    Voicemail data deletion capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Delete_test_600, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Delete_test_600 is staring! ---");
    OHOS::NativeRdb::ValuesBucket valuesOne;
    valuesOne.PutString("phone_number", "1000111");
    int voicemailIdOne = VoicemailInsertValues(valuesOne);
    HILOG_INFO("voicemail_Delete_test_600 : voicemailIdOne = %{public}d", voicemailIdOne);
    EXPECT_GT(voicemailIdOne, 0);

    OHOS::NativeRdb::ValuesBucket valuesTwo;
    valuesTwo.PutString("phone_number", "1000111");
    int voicemailIdTwo = VoicemailInsertValues(valuesTwo);
    HILOG_INFO("voicemail_Delete_test_600 : voicemailIdTwo = %{public}d", voicemailIdTwo);
    EXPECT_GT(voicemailIdTwo, 0);

    OHOS::NativeRdb::ValuesBucket valuesThree;
    valuesThree.PutString("phone_number", "1000111");
    int voicemailIdThree = VoicemailInsertValues(valuesThree);
    HILOG_INFO("voicemail_Delete_test_600 : voicemailIdThree = %{public}d", voicemailIdThree);
    EXPECT_GT(voicemailIdThree, 0);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(voicemailIdOne));
    predicates.Or();
    predicates.EqualTo("id", std::to_string(voicemailIdThree));
    int deleteCode = VoicemailDelete(predicates);
    HILOG_INFO("voicemail_Delete_test_600: deleteCode = %{public}d", deleteCode);
    EXPECT_EQ(deleteCode, 0);

    std::vector<std::string> columns;
    columns.push_back("phone_number");
    columns.push_back("id");
    valuesTwo.PutInt("id", voicemailIdTwo);
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(voicemailIdOne));
    predicates.Or();
    predicates.EqualTo("id", std::to_string(voicemailIdTwo));
    predicates.Or();
    predicates.EqualTo("id", std::to_string(voicemailIdThree));
    predicates.OrderByAsc("id");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(valuesTwo, result, "voicemail_Delete_test_600");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Delete_test_700
 * @tc.name    Delete all fields of single voice mailbox data, and verify whether the deletion is successful
 * @tc.desc    Call log deletion capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Delete_test_700, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----voicemail_Delete_test_700 is starting!-----");
    // insert
    std::vector<std::string> columns;
    GetVoiceMailValuesColumn(columns);
    int columnsSize = 10;
    OHOS::NativeRdb::ValuesBucket valuesBucket = GetVoiceMailValues(0, columnsSize, columns);
    int rawId = VoicemailInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawId));
    // test end delete data
    int deleteCode = VoicemailDelete(predicates);
    EXPECT_EQ(deleteCode, 0);

    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetDeleteQuery = VoicemailQuery(columns, predicates);
    // resultSet count 0
    int rowCount = 0;
    resultSetDeleteQuery->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    resultSetDeleteQuery->Close();
    HILOG_INFO("-----voicemail_Delete_test_700 is edn!-----");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Delete_test_800
 * @tc.name    Delete voice mailbox data in batch and verify whether the deletion is successful
 * @tc.desc    delete ability to call records in batches
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Delete_test_800, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Delete_test_800 is starting!---");
    OHOS::NativeRdb::ValuesBucket voicemailValues;
    voicemailValues.PutString("phone_number", "154236");
    int voicemailIdOne = VoicemailInsertValues(voicemailValues);
    EXPECT_GT(voicemailIdOne, 0);
    voicemailValues.PutString("phone_number", "154236");
    int voicemailIdTwo = VoicemailInsertValues(voicemailValues);
    EXPECT_GT(voicemailIdTwo, 0);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    std::shared_ptr<OHOS::NativeRdb::DataAbilityPredicates> executePredicates =
        std::make_shared<OHOS::NativeRdb::DataAbilityPredicates>(predicates);
    std::shared_ptr<Uri> uri = std::make_shared<Uri>(VoicemailUri::VOICEMAIL);
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
        voicemailAbility.ExecuteBatch(executeBatchOperations);
    EXPECT_EQ(0, dataAbilityResult[0]->GetCount());
    std::vector<std::string> columns;
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetInsertQuery = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    resultSetInsertQuery->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Update_test_900
 * @tc.name    Update the voice mailbox status and verify whether the update is successful (voice_status changes from 0
 * to 1)
 * @tc.desc    Voicemail status update capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Update_test_900, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Update_test_900 is starting! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "166323340");
    values.PutInt("voice_status", 0);
    int voicemailId = VoicemailInsertValues(values);
    HILOG_INFO("voicemail_Update_test_900 : voicemailId = %{public}d", voicemailId);
    EXPECT_GT(voicemailId, 0);

    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutInt("voice_status", 1);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(voicemailId));
    int updateCode = VoicemailUpdate(updateValues, predicates);
    EXPECT_EQ(updateCode, 0);

    updateValues.PutString("phone_number", "166323340");
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(voicemailId));
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    columns.push_back("voice_status");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(updateValues, result, "voicemail_Update_test_900");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Update_test_1000
 * @tc.name    FUpdate single voicemail data and verify whether the update is successful
 * @tc.desc    Voicemail data update capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Update_test_1000, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Update_test_1000 is staring! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "10086");
    int voicemailId = VoicemailInsertValues(values);
    HILOG_INFO("voicemail_Update_test_1000 : voicemailId = %{public}d", voicemailId);
    EXPECT_GT(voicemailId, 0);

    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutString("phone_number", std::string("18522547896"));
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(voicemailId));
    int updateCode = VoicemailUpdate(updateValues, predicates);
    HILOG_INFO("voicemail_Update_test_1000: updateCode = %{public}d", updateCode);
    EXPECT_EQ(updateCode, 0);

    predicates.Clear();
    predicates.EqualTo("id", std::to_string(voicemailId));
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(updateValues, result, "voicemail_Update_test_1000");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Update_test_1100
 * @tc.name    Insert 3 pieces of voicemail data, modify 2 of them, and verify whether the modification is successful
 * @tc.desc    Voicemail data update capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Update_test_1100, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Update_test_1100 is staring! ---");
    OHOS::NativeRdb::ValuesBucket valuesOne;
    valuesOne.PutString("phone_number", "177888855");
    int voicemailIdOne = VoicemailInsertValues(valuesOne);
    EXPECT_GT(voicemailIdOne, 0);

    OHOS::NativeRdb::ValuesBucket valuesTwo;
    valuesTwo.PutString("phone_number", "177888856");
    int voicemailIdTwo = VoicemailInsertValues(valuesTwo);
    EXPECT_GT(voicemailIdTwo, 0);

    OHOS::NativeRdb::ValuesBucket valuesThree;
    valuesThree.PutString("phone_number", "177888856");
    int voicemailIdThree = VoicemailInsertValues(valuesThree);
    EXPECT_GT(voicemailIdThree, 0);

    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutString("phone_number", std::string("20014455"));
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(voicemailIdOne));
    int updateCode = VoicemailUpdate(updateValues, predicates);
    EXPECT_EQ(updateCode, 0);

    OHOS::NativeRdb::ValuesBucket updateValuesTwo;
    updateValuesTwo.PutString("phone_number", std::string("1993025"));
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(voicemailIdTwo));
    updateCode = VoicemailUpdate(updateValuesTwo, predicates);
    EXPECT_EQ(updateCode, 0);

    std::vector<std::string> columns;
    columns.push_back("phone_number");
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(voicemailIdOne));
    predicates.Or();
    predicates.EqualTo("id", std::to_string(voicemailIdTwo));
    predicates.OrderByAsc("id");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(2, rowCount);
    std::vector<OHOS::NativeRdb::ValuesBucket> valuesVector;
    valuesVector.push_back(updateValues);
    valuesVector.push_back(updateValuesTwo);
    CheckResultSetList(valuesVector, result, "voicemail_Update_test_1100");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Update_test_1200
 * @tc.name    Update the full field data of voice mailbox and verify whether the update is successful
 * @tc.desc    Support all call log update capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Update_test_1200, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----voicemail_Update_test_1200 is starting!-----");
    std::vector<std::string> columns;
    GetVoiceMailValuesColumn(columns);
    int columnsSize = 10;
    OHOS::NativeRdb::ValuesBucket valuesBucket = GetVoiceMailValues(0, columnsSize, columns);
    int rawId = VoicemailInsertValues(valuesBucket);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawId));
    OHOS::NativeRdb::ValuesBucket upDateValuesBucket = GetVoiceMailValues(0, columnsSize, columns);
    int upDateCode = VoicemailUpdate(upDateValuesBucket, predicates);
    EXPECT_EQ(upDateCode, 0);

    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = VoicemailQuery(columns, predicates);
    // resultSet count 1
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);

    CheckResultSet(upDateValuesBucket, resultSet, "voicemail_Update_test_1200");
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(rawId));
    // test end delete data
    int deleteCode = VoicemailDelete(predicates);
    EXPECT_EQ(deleteCode, 0);
    HILOG_INFO("-----voicemail_Update_test_1200 is edn!-----");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Update_test_1300
 * @tc.name    Batch update voice mailbox data and verify whether the update is successful
 * @tc.desc    update ability to call records in batches
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Update_test_1300, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Update_test_1300 is starting!---");
    OHOS::NativeRdb::ValuesBucket voicemailValues;
    voicemailValues.PutString("phone_number", "154236");
    int voicemailIdOne = VoicemailInsertValues(voicemailValues);
    EXPECT_GT(voicemailIdOne, 0);
    voicemailValues.PutString("phone_number", "154236");
    int voicemailIdTwo = VoicemailInsertValues(voicemailValues);
    EXPECT_GT(voicemailIdTwo, 0);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(voicemailIdOne));
    predicates.Or();
    predicates.EqualTo("id", std::to_string(voicemailIdTwo));
    std::shared_ptr<OHOS::NativeRdb::ValuesBucket> values =
        std::make_shared<OHOS::NativeRdb::ValuesBucket>(voicemailValues);
    std::shared_ptr<OHOS::NativeRdb::DataAbilityPredicates> executePredicates =
        std::make_shared<OHOS::NativeRdb::DataAbilityPredicates>(predicates);
    std::shared_ptr<Uri> uri = std::make_shared<Uri>(VoicemailUri::VOICEMAIL);
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
        voicemailAbility.ExecuteBatch(executeBatchOperations);
    EXPECT_EQ(0, dataAbilityResult[0]->GetCount());
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetInsertQuery = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    resultSetInsertQuery->GetRowCount(rowCount);
    EXPECT_EQ(2, rowCount);
    std::vector<OHOS::NativeRdb::ValuesBucket> listValue;
    listValue.push_back(voicemailValues);
    listValue.push_back(voicemailValues);
    CheckResultSetList(listValue, resultSetInsertQuery, "voicemail_Update_test_1300");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Query_test_1400
 * @tc.name    Query the voice mailbox in status 1 and return the query results
 * @tc.desc    Voicemail status query capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Query_test_1400, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Query_test_1400 is starting! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "882203110");
    values.PutInt("voice_status", 1);
    int voicemailId = VoicemailInsertValues(values);
    HILOG_INFO("voicemail_Query_test_1400 : voicemailId = %{public}d", voicemailId);
    EXPECT_GT(voicemailId, 0);

    values.PutInt("id", voicemailId);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    columns.push_back("voice_status");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("voice_status", "1");
    predicates.And();
    predicates.EqualTo("id", std::to_string(voicemailId));
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(values, result, "voicemail_Query_test_1400");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Query_test_1500
 * @tc.name    Insert a single voice mailbox and query the inserted voice mailbox data according to the returned ID
 * @tc.desc    Voicemail data query capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Query_test_1500, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Query_test_1500 is staring! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "10001");
    int voicemailId = VoicemailInsertValues(values);
    HILOG_INFO("voicemail_Query_test_1500 : voicemailId = %{public}d", voicemailId);
    EXPECT_GT(voicemailId, 0);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(voicemailId));
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(values, result, "voicemail_Query_test_1500");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Query_test_1600
 * @tc.name    Insert multiple voice mailboxes, and query the inserted voice mailbox data according to the returned ID
 * @tc.desc    Voicemail data query capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Query_test_1600, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Query_test_1600 is staring! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "10001");
    int voicemailIdOne = VoicemailInsertValues(values);
    HILOG_INFO("voicemail_Query_test_1600 : voicemailIdOne = %{public}d", voicemailIdOne);
    EXPECT_GT(voicemailIdOne, 0);

    OHOS::NativeRdb::ValuesBucket valuesTwo;
    valuesTwo.PutString("phone_number", "891320");
    int voicemailIdTwo = VoicemailInsertValues(valuesTwo);
    HILOG_INFO("voicemail_Query_test_1600 : voicemailIdTwo = %{public}d", voicemailIdTwo);
    EXPECT_GT(voicemailIdTwo, 0);

    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(voicemailIdOne));
    predicates.Or();
    predicates.EqualTo("id", std::to_string(voicemailIdTwo));
    predicates.OrderByAsc("id");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(2, rowCount);
    std::vector<OHOS::NativeRdb::ValuesBucket> valuesVector;
    valuesVector.push_back(values);
    valuesVector.push_back(valuesTwo);
    CheckResultSetList(valuesVector, result, "voicemail_Query_test_1600");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Query_test_1700
 * @tc.name    Query all voice mail data and return query results
 * @tc.desc    Voicemail data query capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Query_test_1700, testing::ext::TestSize.Level1)
{
    // query database count  There are two test cases above ,the number is 4
    HILOG_INFO("--- voicemail_Query_test_1700 is staring! ---");
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    result->Close();
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_BatchInsert_test_1800
 * @tc.name    Add 3 voice mail messages with status  and verify whether the insertion is successful
 * @tc.desc    Added ability to batch voicemail status
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_BatchInsert_test_1800, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_BatchInsert_test_1800 is starting! ---");

    OHOS::Uri uriData(VoicemailUri::VOICEMAIL);
    OHOS::NativeRdb::ValuesBucket voicemailValuesOne;
    voicemailValuesOne.PutString("phone_number", std::string("1470023"));
    voicemailValuesOne.PutInt("voice_status", 1);
    OHOS::NativeRdb::ValuesBucket voicemailValuesTwo;
    voicemailValuesTwo.PutString("phone_number", std::string("1470056"));
    voicemailValuesTwo.PutInt("voice_status", 0);
    OHOS::NativeRdb::ValuesBucket voicemailValuesThree;
    voicemailValuesThree.PutString("phone_number", std::string("1470089"));
    voicemailValuesThree.PutInt("voice_status", 0);

    std::vector<OHOS::NativeRdb::ValuesBucket> listAddBluk;
    listAddBluk.push_back(voicemailValuesOne);
    listAddBluk.push_back(voicemailValuesTwo);
    listAddBluk.push_back(voicemailValuesThree);
    int batchInsertCode = voicemailAbility.BatchInsert(uriData, listAddBluk);
    EXPECT_EQ(batchInsertCode, 0);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("phone_number", "1470023");
    predicates.Or();
    predicates.EqualTo("phone_number", "1470089");
    predicates.OrderByAsc("id");
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    columns.push_back("voice_status");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(2, rowCount);
    std::vector<OHOS::NativeRdb::ValuesBucket> listValue;
    listValue.push_back(voicemailValuesOne);
    listValue.push_back(voicemailValuesThree);
    CheckResultSetList(listValue, result, "voicemail_BatchInsert_test_1800");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_BatchInsert_test_1900
 * @tc.name    Add 5 voice mail data and verify whether the insertion is successful
 * @tc.desc    New capabilities for voice mail data in batches
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_BatchInsert_test_1900, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_BatchInsert_test_1900 is staring! ---");
    OHOS::Uri uriData(VoicemailUri::VOICEMAIL);
    OHOS::NativeRdb::ValuesBucket voicemailValuesOne;
    voicemailValuesOne.PutString("phone_number", std::string("123478"));
    voicemailValuesOne.PutString("display_name", std::string("xiaoming"));
    OHOS::NativeRdb::ValuesBucket voicemailValuesTwo;
    voicemailValuesTwo.PutString("phone_number", std::string("10002"));
    voicemailValuesTwo.PutString("display_name", std::string("xiaozi"));
    OHOS::NativeRdb::ValuesBucket voicemailValuesThree;
    voicemailValuesThree.PutString("phone_number", std::string("10003"));
    voicemailValuesThree.PutString("display_name", std::string("xiaozhi"));
    OHOS::NativeRdb::ValuesBucket voicemailValuesFour;
    voicemailValuesFour.PutString("phone_number", std::string("10004"));
    voicemailValuesFour.PutString("display_name", std::string("xiaohei"));
    OHOS::NativeRdb::ValuesBucket voicemailValuesFive;
    voicemailValuesFive.PutString("phone_number", std::string("10005"));
    voicemailValuesFive.PutString("display_name", std::string("xiaohong"));
    std::vector<OHOS::NativeRdb::ValuesBucket> listAddBluk;
    listAddBluk.push_back(voicemailValuesOne);
    listAddBluk.push_back(voicemailValuesTwo);
    listAddBluk.push_back(voicemailValuesThree);
    listAddBluk.push_back(voicemailValuesFour);
    listAddBluk.push_back(voicemailValuesFive);
    int batchInsertCode = voicemailAbility.BatchInsert(uriData, listAddBluk);
    EXPECT_EQ(0, batchInsertCode);

    int size = listAddBluk.size();
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    columns.push_back("display_name");
    for (int i = 0; i < size; i++) {
        OHOS::NativeRdb::DataAbilityPredicates predicates;
        std::string name;
        OHOS::NativeRdb::ValueObject value;
        listAddBluk[i].GetObject("display_name", value);
        value.GetString(name);
        predicates.EqualTo("display_name", name);
        predicates.And();
        std::string phoneNumber;
        OHOS::NativeRdb::ValueObject phoneNumberValue;
        listAddBluk[i].GetObject("phone_number", phoneNumberValue);
        phoneNumberValue.GetString(phoneNumber);
        predicates.EqualTo("phone_number", phoneNumber);
        CheckResultSetBatch(predicates, columns, listAddBluk[i], "voicemail_BatchInsert_test_1900");
    }
    ClearVoicemail();
}

/*
 * @tc.number  abnormal_voicemail_Insert_test_2000
 * @tc.name    When voice mail status is added, a non-existent field is passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, abnormal_voicemail_Insert_test_2000, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_voicemail_Insert_test_2000 is starting! ---");

    OHOS::Uri uriVoicemail(VoicemailUri::VOICEMAIL);
    OHOS::NativeRdb::ValuesBucket voicemailValues;
    voicemailValues.PutString("phone_numbers", "89633");
    int64_t voicemailId = voicemailAbility.Insert(uriVoicemail, voicemailValues);
    HILOG_INFO("abnormal_voicemail_Insert_test_2000: voicemailId = %{public}lld", voicemailId);
    EXPECT_EQ(voicemailId, -1);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("phone_number", "89633");
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    result->Close();
    ClearVoicemail();
}

/*
 * @tc.number  abnormal_voicemail_Insert_test_2100
 * @tc.name    When adding voice mail data, a nonexistent table name is passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, abnormal_voicemail_Insert_test_2100, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_voicemail_Insert_test_2100 is starting! ---");

    OHOS::Uri errorUriVoicemails(VoicemailUri::ERROR_URI);
    OHOS::NativeRdb::ValuesBucket voicemailValues;
    voicemailValues.PutString("phone_number", "230000");
    int64_t voicemailId = voicemailAbility.Insert(errorUriVoicemails, voicemailValues);
    HILOG_INFO("abnormal_voicemail_Insert_test_2100: voicemailId = %{public}lld", voicemailId);
    EXPECT_EQ(voicemailId, -1);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("phone_number", "230000");
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    result->Close();
    ClearVoicemail();
}

/*
 * @tc.number  abnormal_voicemail_Update_test_2200
 * @tc.name    When deleting voicemail status, a non-existent field is passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, abnormal_voicemail_Update_test_2200, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_voicemail_Update_test_2200 is starting! ---");
    int64_t voicemailId = VoicemailStatusInsert("123456", 1);
    HILOG_INFO("abnormal_voicemail_Update_test_2200: voicemailId = %{public}lld", voicemailId);
    EXPECT_GT(voicemailId, 0);

    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutInt("voice_status", 0);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("ids", std::to_string(voicemailId));
    int updateCode = VoicemailUpdate(updateValues, predicates);
    HILOG_INFO("abnormal_voicemail_Update_test_2200 : updateCode = %{public}d", updateCode);
    EXPECT_EQ(updateCode, -1);

    predicates.Clear();
    predicates.EqualTo("phone_number", "123456");
    predicates.And();
    predicates.EqualTo("voice_status", "0");
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    columns.push_back("voice_status");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    result->Close();
    ClearVoicemail();
}

/*
 * @tc.number  abnormal_voicemail_Delete_test_2300
 * @tc.name    When deleting voicemail data, a non-existent field is passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, abnormal_voicemail_Delete_test_2300, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_voicemail_Delete_test_2300 is starting! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "10086123");
    int voicemailId = VoicemailInsertValues(values);
    HILOG_INFO("abnormal_voicemail_Delete_test_2300 : voicemailId = %{public}d", voicemailId);
    EXPECT_GT(voicemailId, 0);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("ids", std::to_string(voicemailId));
    int deleteCode = VoicemailDelete(predicates);
    HILOG_INFO("abnormal_voicemail_Delete_test_2300: deleteCode = %{public}d", deleteCode);
    EXPECT_EQ(deleteCode, -1);
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(voicemailId));
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(values, result, "abnormal_voicemail_Delete_test_2300");
    ClearVoicemail();
}

/*
 * @tc.number  abnormal_voicemail_Delete_test_2400
 * @tc.name    When deleting voicemail data, a nonexistent table name is passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, abnormal_voicemail_Delete_test_2400, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_voicemail_Delete_test_2400 is starting! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "166320");
    int voicemailId = VoicemailInsertValues(values);
    HILOG_INFO("abnormal_voicemail_Delete_test_2400 : voicemailId = %{public}d", voicemailId);
    EXPECT_GT(voicemailId, 0);

    OHOS::Uri uriVoicemails(VoicemailUri::ERROR_URI);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(voicemailId));
    int deleteCode = voicemailAbility.Delete(uriVoicemails, predicates);
    HILOG_INFO("abnormal_voicemail_Delete_test_2400: deleteCode = %{public}d", deleteCode);
    EXPECT_NE(deleteCode, 0);

    std::vector<std::string> columns;
    columns.push_back("phone_number");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(values, result, "abnormal_voicemail_Delete_test_2400");
    ClearVoicemail();
}

/*
 * @tc.number  abnormal_voicemail_Update_test_2500
 * @tc.name    When updating voicemail status, a non-existent field was passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, abnormal_voicemail_Update_test_2500, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_voicemail_Update_test_2500 is starting! ---");
    int64_t voicemailId = VoicemailStatusInsert("123456", 1);
    HILOG_INFO("abnormal_voicemail_Update_test_2500: voicemailId = %{public}lld", voicemailId);
    EXPECT_GT(voicemailId, 0);

    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutInt("voice_status", 0);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("ids", std::to_string(voicemailId));
    int updateCode = VoicemailUpdate(updateValues, predicates);
    HILOG_INFO("abnormal_voicemail_Update_test_2500 : updateCode = %{public}d", updateCode);
    EXPECT_EQ(updateCode, -1);

    predicates.Clear();
    predicates.EqualTo("phone_number", "123456");
    predicates.And();
    predicates.EqualTo("voice_status", "0");
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    columns.push_back("voice_status");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    result->Close();
    ClearVoicemail();
}

/*
 * @tc.number  abnormal_voicemail_Update_test_2600
 * @tc.name    When updating voicemail data, a non-existent field was passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, abnormal_voicemail_Update_test_2600, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_voicemail_Update_test_2600 is staring! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "11100886622");
    int voicemailId = VoicemailInsertValues(values);
    HILOG_INFO("abnormal_voicemail_Update_test_2600 : voicemailId = %{public}d", voicemailId);
    EXPECT_GT(voicemailId, 0);

    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutString("phone_number", std::string("18522547896"));
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("ids", std::to_string(voicemailId));
    int updateCode = VoicemailUpdate(updateValues, predicates);
    HILOG_INFO("abnormal_voicemail_Update_test_2600: updateCode = %{public}d", updateCode);
    EXPECT_EQ(updateCode, -1);
    ClearVoicemail();
}

/*
 * @tc.number  abnormal_voicemail_Update_test_2700
 * @tc.name    When updating voicemail data, a nonexistent table name is passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, abnormal_voicemail_Update_test_2700, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_voicemail_Update_test_2700 is staring! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "188520");
    int voicemailId = VoicemailInsertValues(values);
    HILOG_INFO("abnormal_voicemail_Update_test_2700 : voicemailId = %{public}d", voicemailId);
    EXPECT_GT(voicemailId, 0);

    OHOS::Uri uriVoicemails(VoicemailUri::ERROR_URI);
    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutString("phone_number", std::string("18522547896"));
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(voicemailId));
    int updateCode = voicemailAbility.Update(uriVoicemails, updateValues, predicates);
    HILOG_INFO("abnormal_voicemail_Update_test_2700: updateCode = %{public}d", updateCode);
    EXPECT_EQ(updateCode, -1);
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(values, result, "abnormal_voicemail_Update_test_2700");
    ClearVoicemail();
}

/*
 * @tc.number  abnormal_voicemail_Query_test_2800
 * @tc.name    When querying voicemail status, a non-existent field is passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, abnormal_voicemail_Query_test_2800, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_voicemail_Query_test_2800 is starting! ---");
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_numbers");
    columns.push_back("voice_status");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("voice_status", "1");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(-1, rowCount);
    resultSet->Close();
    ClearVoicemail();
}

/*
 * @tc.number  abnormal_voicemail_Query_test_2900
 * @tc.name    An incorrect field was passed in when querying voicemail data
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, abnormal_voicemail_Query_test_2900, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----abnormal_voicemail_Query_test_2900 is starting!-----");
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_numbers");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = VoicemailQuery(columns, predicates);
    int rowCount = -1;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(-1, rowCount);
    resultSet->Close();
    ClearVoicemail();
}

/*
 * @tc.number  abnormal_voicemail_Query_test_3000
 * @tc.name    When querying voicemail data, a non-existent field value is passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, abnormal_voicemail_Query_test_3000, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----abnormal_voicemail_Query_test_3000 is starting!-----");
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", "10000000");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    result->Close();
    EXPECT_EQ(0, rowCount);
    ClearVoicemail();
}

/*
 * @tc.number  abnormal_voicemail_Query_test_3100
 * @tc.name    When querying voicemail data, a nonexistent table name was passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, abnormal_voicemail_Query_test_3100, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----abnormal_voicemail_Query_test_3100 is starting!-----");
    OHOS::Uri errorUri(VoicemailUri::ERROR_URI);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet =
        voicemailAbility.Query(errorUri, columns, predicates);
    EXPECT_EQ(resultSet, nullptr);
    ClearVoicemail();
}

/*
 * @tc.number  abnormal_voicemail_BatchInsert_test_3200
 * @tc.name    When adding voice mail status in batch, an error field is passed in
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, abnormal_voicemail_BatchInsert_test_3200, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_voicemail_BatchInsert_test_3200 is starting! ---");
    OHOS::Uri uriData(VoicemailUri::VOICEMAIL);
    OHOS::NativeRdb::ValuesBucket voicemailValuesOne;
    voicemailValuesOne.PutString("phone_number", std::string("147002366666666"));
    voicemailValuesOne.PutInt("voice_status", 1);
    OHOS::NativeRdb::ValuesBucket voicemailValuesTwo;
    voicemailValuesTwo.PutString("phone_numbers", std::string("1470056"));
    voicemailValuesTwo.PutInt("voice_statuss", 0);
    OHOS::NativeRdb::ValuesBucket voicemailValuesThree;
    voicemailValuesThree.PutString("phone_number", std::string("1470089"));
    voicemailValuesThree.PutInt("voice_status", 0);

    std::vector<OHOS::NativeRdb::ValuesBucket> listAddBluk;
    listAddBluk.push_back(voicemailValuesOne);
    listAddBluk.push_back(voicemailValuesTwo);
    listAddBluk.push_back(voicemailValuesThree);
    int batchInsertCode = voicemailAbility.BatchInsert(uriData, listAddBluk);
    EXPECT_EQ(batchInsertCode, -1);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("phone_number", "147002366666666");
    predicates.Or();
    predicates.EqualTo("phone_numbers", "1470056");
    predicates.Or();
    predicates.EqualTo("phone_number", "1470089");
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    columns.push_back("phone_numbers");
    columns.push_back("voice_status");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(-1, rowCount);
    resultSet->Close();
    ClearVoicemail();
}

/*
 * @tc.number  abnormal_voicemail_BatchInsert_test_3300
 * @tc.name    When adding voice mail data in batch, several of them fail. Check the processing logic
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, abnormal_voicemail_BatchInsert_test_3300, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- abnormal_voicemail_BatchInsert_test_3300 is starting! ---");
    OHOS::Uri uriData(VoicemailUri::VOICEMAIL);
    OHOS::NativeRdb::ValuesBucket voicemailValues;
    std::vector<OHOS::NativeRdb::ValuesBucket> listAddBluk;
    int batchInserCode = 0;
    for (int i = 0; i < 10; i++) {
        listAddBluk.clear();
        for (int j = 10 * i + 1; j <= 10 * (i + 1); j++) {
            voicemailValues.Clear();
            voicemailValues.PutString("phone_number", std::to_string(j));
            std::string name("zhangsan");
            name.append(std::to_string(j));
            if (j == 14 || j == 27 || j == 57) {
                voicemailValues.PutString("display_names", name);
            } else {
                voicemailValues.PutString("display_name", name);
            }
            listAddBluk.push_back(voicemailValues);
        }
        batchInserCode = voicemailAbility.BatchInsert(uriData, listAddBluk);
        HILOG_INFO("abnormal_voicemail_BatchInsert_test_3300 : batchInserCode = %{public}d", batchInserCode);
        if (batchInserCode == 0) {
            HILOG_INFO("abnormal_voicemail_BatchInsert_test_3300 batch insert success!");
        } else {
            HILOG_INFO("abnormal_voicemail_BatchInsert_test_3300 batch insert fail!");
            EXPECT_EQ(batchInserCode, -1);
        }
    }
    // clear data
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    int deleteCode = VoicemailDelete(predicates);
    EXPECT_EQ(0, deleteCode);
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Insert_test_3400
 * @tc.name    Add a voice mailbox with status 0 and verify that the insertion was successful
 * @tc.desc    Added ability to voice mailbox status
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Insert_test_3400, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Insert_test_3400 is starting! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "166320");
    values.PutInt("voice_status", 0);
    int voicemailId = VoicemailInsertValues(values);
    EXPECT_GT(voicemailId, 0);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(voicemailId));
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    columns.push_back("voice_status");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(values, result, "voicemail_Insert_test_3400");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Update_test_3500
 * @tc.name    Update the voice mailbox status and verify whether the update is successful (voice_status changes from 1
 * to 0)
 * @tc.desc    Voicemail status update capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Update_test_3500, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Update_test_3500 is starting! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "166323340");
    values.PutInt("voice_status", 1);
    int voicemailId = VoicemailInsertValues(values);
    EXPECT_GT(voicemailId, 0);

    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutInt("voice_status", 0);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(voicemailId));
    int updateCode = VoicemailUpdate(updateValues, predicates);
    EXPECT_EQ(updateCode, 0);

    updateValues.PutString("phone_number", "166323340");
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(voicemailId));
    std::vector<std::string> columns;
    columns.push_back("phone_number");
    columns.push_back("voice_status");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(updateValues, result, "voicemail_Update_test_3500");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_Query_test_3600
 * @tc.name    Query the voice mailbox with status 0 and return the query results
 * @tc.desc    Voicemail status query capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_Query_test_3600, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_Query_test_3600 is starting! ---");
    OHOS::NativeRdb::ValuesBucket values;
    values.PutString("phone_number", "882203110");
    values.PutInt("voice_status", 0);
    int voicemailId = VoicemailInsertValues(values);
    EXPECT_GT(voicemailId, 0);

    values.PutInt("id", voicemailId);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    columns.push_back("voice_status");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("voice_status", "0");
    predicates.And();
    predicates.EqualTo("id", std::to_string(voicemailId));
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> result = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    result->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(values, result, "voicemail_Query_test_3600");
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_async_insert_test_3800
 * @tc.name    Add voicemail async
 * @tc.desc    Ability to join or add voicemail
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_async_insert_test_3700, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_async_insert_test_3700 is staring! ---");
    std::map<int, OHOS::NativeRdb::ValuesBucket> result;
    std::vector<VoicemailAsync *> voicemailAsyncVector;
    int threadNum = 6;
    for (int i = 0; i < threadNum; ++i) {
        OHOS::NativeRdb::ValuesBucket values;
        std::string name;
        name.append("asyncTest");
        name.append(std::to_string(i));
        values.PutString("display_name", name);
        VoicemailAsync *contactAsync = new VoicemailAsync(values, result);
        std::thread asyncThread(&VoicemailAsync::Insert, contactAsync);
        voicemailAsyncVector.push_back(contactAsync);
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
        std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = VoicemailQuery(columns, predicates);
        CheckResultSet(it->second, resultSet, "voicemail_async_insert_test_3700");
    }
    for (int i = 0; i < threadNum; ++i) {
        delete voicemailAsyncVector[i];
    }
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_async_update_test_3900
 * @tc.name    update voicemail async
 * @tc.desc    Ability to join or update voicemail
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_async_update_test_3800, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_async_update_test_3800 is staring! ---");
    OHOS::NativeRdb::ValuesBucket voicemailValues;
    std::vector<int64_t> voicemailId;
    voicemailId.push_back(VoicemailInsertValue("async_update1", voicemailValues));
    voicemailValues.Clear();
    voicemailId.push_back(VoicemailInsertValue("async_update2", voicemailValues));
    voicemailValues.Clear();
    voicemailId.push_back(VoicemailInsertValue("async_update3", voicemailValues));
    voicemailValues.Clear();
    voicemailId.push_back(VoicemailInsertValue("async_update4", voicemailValues));
    voicemailValues.Clear();
    voicemailId.push_back(VoicemailInsertValue("async_update5", voicemailValues));
    voicemailValues.Clear();
    voicemailId.push_back(VoicemailInsertValue("async_update6", voicemailValues));
    voicemailValues.Clear();
    std::vector<OHOS::NativeRdb::ValuesBucket> upDateValues;
    OHOS::NativeRdb::DataAbilityPredicates queryPredicates;
    std::vector<VoicemailAsync *> voicemailAsyncVector;
    int size = voicemailId.size();
    for (int i = 0; i < size; ++i) {
        OHOS::NativeRdb::ValuesBucket values;
        std::string name;
        name.append("asyncTest");
        values.PutString("display_name", name);
        upDateValues.push_back(values);
        int id = voicemailId[i];
        VoicemailAsync *contactAsync = new VoicemailAsync(values, id);
        voicemailAsyncVector.push_back(contactAsync);
        std::thread asyncThread(&VoicemailAsync::Update, contactAsync);
        asyncThread.detach();
        queryPredicates.EqualTo("id", std::to_string(voicemailId[i]));
        if (i < size - 1) {
            queryPredicates.Or();
        }
    }
    std::chrono::milliseconds dura(Time::ASYNC_SLEEP_TIME);
    std::this_thread::sleep_for(dura);
    std::vector<std::string> columns;
    columns.push_back("display_name");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = VoicemailQuery(columns, queryPredicates);
    CheckResultSetList(upDateValues, resultSet, "voicemail_async_update_test_3800");
    for (int i = 0; i < size; ++i) {
        delete voicemailAsyncVector[i];
    }
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_async_query_test_3900
 * @tc.name    query voicemail async
 * @tc.desc    Ability to join or query
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_async_query_test_3900, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_async_query_test_4000 is staring! ---");
    OHOS::NativeRdb::ValuesBucket voicemailValues;
    std::vector<int64_t> voicemailId;
    voicemailId.push_back(VoicemailInsertValue("async_query1", voicemailValues));
    voicemailValues.Clear();
    voicemailId.push_back(VoicemailInsertValue("async_query2", voicemailValues));
    voicemailValues.Clear();
    voicemailId.push_back(VoicemailInsertValue("async_query3", voicemailValues));
    voicemailValues.Clear();
    voicemailId.push_back(VoicemailInsertValue("async_query4", voicemailValues));
    voicemailValues.Clear();
    voicemailId.push_back(VoicemailInsertValue("async_query5", voicemailValues));
    voicemailValues.Clear();
    voicemailId.push_back(VoicemailInsertValue("async_query6", voicemailValues));
    voicemailValues.Clear();
    std::vector<VoicemailAsync *> voicemailAsyncVector;
    std::vector<std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet>> resultSetVector;
    int threadNum = 6;
    for (int i = 0; i < threadNum; ++i) {
        VoicemailAsync *contactAsync = new VoicemailAsync(resultSetVector, voicemailId);
        voicemailAsyncVector.push_back(contactAsync);
        std::thread asyncThread(&VoicemailAsync::Query, contactAsync);
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
        delete voicemailAsyncVector[i];
    }
    ClearVoicemail();
}

/*
 * @tc.number  voicemail_async_delete_test_4100
 * @tc.name    delete voicemail async
 * @tc.desc    Ability to join or delete voicemail
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(VoicemailAbilityTest, voicemail_async_delete_test_4000, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_async_delete_test_4000 is staring! ---");
    OHOS::NativeRdb::ValuesBucket voicemailValues;
    std::vector<int64_t> voicemailId;
    voicemailId.push_back(VoicemailInsertValue("async_update1", voicemailValues));
    voicemailValues.Clear();
    voicemailId.push_back(VoicemailInsertValue("async_update2", voicemailValues));
    voicemailValues.Clear();
    voicemailId.push_back(VoicemailInsertValue("async_update3", voicemailValues));
    voicemailValues.Clear();
    voicemailId.push_back(VoicemailInsertValue("async_update4", voicemailValues));
    voicemailValues.Clear();
    voicemailId.push_back(VoicemailInsertValue("async_update5", voicemailValues));
    voicemailValues.Clear();
    voicemailId.push_back(VoicemailInsertValue("async_update6", voicemailValues));
    voicemailValues.Clear();
    int size = voicemailId.size();
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.BeginWrap();
    std::vector<VoicemailAsync *> voicemailAsyncVector;
    for (int i = 0; i < size; ++i) {
        int id = voicemailId[i];
        VoicemailAsync *contactAsync = new VoicemailAsync(id);
        voicemailAsyncVector.push_back(contactAsync);
        std::thread asyncThread(&VoicemailAsync::Delete, contactAsync);
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
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = VoicemailQuery(columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    resultSet->Close();
    for (int i = 0; i < size; ++i) {
        delete voicemailAsyncVector[i];
    }
    ClearVoicemail();
}
} // namespace Test
} // namespace Contacts