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

#include "performance_test.h"

#include <sys/time.h>

#include "test_common.h"

namespace Contacts {
namespace Test {
PerformanceTest::PerformanceTest()
{
}

PerformanceTest::~PerformanceTest()
{
}

long long PerformanceTest::GetCurrentTime()
{
    int subtle = 1000000;
    struct timeval time;
    gettimeofday(&time, nullptr);
    return (time.tv_sec * subtle + time.tv_usec);
}

int PerformanceTest::CalcTime(long long startTime, long long endTime)
{
    return (int)(endTime - startTime);
}

void PerformanceTest::DeleteContact()
{
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    OHOS::Uri uriRawContact(ContactsUri::RAW_CONTACT);
    predicates.NotEqualTo("id", "0");
    predicates.And();
    predicates.EqualTo("is_deleted", "0");
    contactsDataAbility.Delete(uriRawContact, predicates);
    predicates.Clear();
    int count = 0;
    int deleteCount = 9999;
    std::vector<std::string> columns;
    OHOS::Uri uriRawContactComplete(ContactsUri::DELETED_RAW_CONTACT);
    while (count < deleteCount) {
        int time = Time::SLEEP_TIME_MERGE_DELETE;
        std::chrono::milliseconds dura(time);
        std::this_thread::sleep_for(dura);
        std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet =
            contactsDataAbility.Query(uriRawContactComplete, columns, predicates);
        resultSet->GetRowCount(count);
        resultSet->Close();
    }
    int time = Time::SLEEP_TIME_MERGE_DELETE;
    std::chrono::milliseconds dura(time);
    std::this_thread::sleep_for(dura);
    predicates.Clear();
    predicates.NotEqualTo("id", "0");
    contactsDataAbility.Delete(uriRawContactComplete, predicates);
}

/*
 * @tc.number  raw_contact_insert_performance_test_900
 * @tc.name    raw_contact performance testing add 10000
 * @tc.desc    add 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, raw_contact_insert_performance_test_900, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- raw_contact_insert_performance_test_900 is starting! ---");
    OHOS::Uri uriRawContact(ContactsUri::RAW_CONTACT);
    std::vector<OHOS::NativeRdb::ValuesBucket> values;
    for (int i = 0; i < 10000; i++) {
        OHOS::NativeRdb::ValuesBucket rawContactValues;
        std::string name("xiaoyan");
        name.append(std::to_string(i + 1));
        rawContactValues.PutString("display_name", name);
        rawContactValues.PutString("company", "company");
        rawContactValues.PutString("position", "position");
        values.push_back(rawContactValues);
    }
    HILOG_INFO("raw_contact_insert_performance_test_900  start! ");
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    int batchInsertCode = contactsDataAbility.BatchInsert(uriRawContact, values);
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    HILOG_INFO("raw_contact_insert_performance_test_900 : time is %{public}d", elaps);
    ASSERT_LE(elaps, TIME_USEC_RAW_CONTACT_INSERT);
    HILOG_INFO("raw_contact_insert_performance_test_900  finish! ");
    HILOG_INFO("raw_contact_insert_performance_test_900 : batchInsertCode = %{public}d", batchInsertCode);
    EXPECT_EQ(batchInsertCode, 0);
}

/*
 * @tc.number  raw_contact_update_performance_test_1000
 * @tc.name    raw_contact performance testing update 10000
 * @tc.desc    update 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, raw_contact_update_performance_test_1000, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- raw_contact_update_performance_test_1000 is starting! ---");
    OHOS::Uri uriRawContact(ContactsUri::RAW_CONTACT);
    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutInt("favorite", 1);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    predicates.And();
    predicates.EqualTo("is_deleted", "0");
    HILOG_INFO("raw_contact_update_performance_test_1000  start! ");
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    int updateCode = contactsDataAbility.Update(uriRawContact, updateValues, predicates);
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    HILOG_INFO("raw_contact_update_performance_test_1000 : time is %{public}d", elaps);
    ASSERT_LE(elaps, TIME_USEC_RAW_CONTACT_UPDATE);
    EXPECT_EQ(updateCode, 0);
}

/*
 * @tc.number  raw_contact_query_performance_test_1100
 * @tc.name    raw_contact performance testing query 10000
 * @tc.desc    query 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, raw_contact_query_performance_test_1100, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- raw_contact_query_performance_test_1100 is starting! ---");
    OHOS::Uri uriRawContact(ContactsUri::RAW_CONTACT);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("display_name");
    columns.push_back("company");
    columns.push_back("position");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    predicates.And();
    predicates.EqualTo("is_deleted", "0");
    HILOG_INFO("raw_contact_query_performance_test_1100  start! ");
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet =
        contactsDataAbility.Query(uriRawContact, columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_GT(rowCount, 9999);
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    resultSet->Close();
    HILOG_INFO("raw_contact_query_performance_test_1100 : time is %{public}d", elaps);
    ASSERT_LE(elaps, TIME_USEC_RAW_CONTACT_QUERY);
}

/*
 * @tc.number  raw_contact_delete_performance_test_1200
 * @tc.name    raw_contact performance testing delete 10000
 * @tc.desc    delete 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, raw_contact_delete_performance_test_1200, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- raw_contact_delete_performance_test_1200 is starting! ---");
    OHOS::Uri uriRawContact(ContactsUri::RAW_CONTACT);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    predicates.And();
    predicates.EqualTo("is_deleted", "0");
    HILOG_INFO("raw_contact_delete_performance_test_1200  start! ");
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    int deleteCode = contactsDataAbility.Delete(uriRawContact, predicates);
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    HILOG_INFO("raw_contact_delete_performance_test_1200 : time is %{public}d", elaps);
    ASSERT_LE(elaps, TIME_USEC_RAW_CONTACT_DELETED);
    HILOG_INFO("raw_contact_delete_performance_test_1200 : deleteCode = %{public}d", deleteCode);
    EXPECT_EQ(deleteCode, 0);
}

/*
 * @tc.number  contact_data_insert_performance_test_1300
 * @tc.name    contact_data performance testing add 10000
 * @tc.desc    add 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, contact_data_insert_performance_test_1300, testing::ext::TestSize.Level1)
{
    OHOS::Uri uriRawContact(ContactsUri::RAW_CONTACT);
    OHOS::NativeRdb::ValuesBucket rawContactValues;
    std::string rawName("xiaoyan");
    rawContactValues.PutString("display_name", rawName);
    rawContactValues.PutString("company", "company");
    rawContactValues.PutString("position", "position");
    int rawContactId = contactsDataAbility.Insert(uriRawContact, rawContactValues);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.NotEqualTo("id", "0");
    predicates.And();
    predicates.EqualTo("is_deleted", "0");
    contactsDataAbility.Delete(uriRawContact, predicates);
    HILOG_INFO("--- contact_data_insert_performance_test_1300 is starting! ---");
    OHOS::Uri uriContactData(ContactsUri::CONTACT_DATA);
    std::vector<OHOS::NativeRdb::ValuesBucket> values;
    for (int i = 0; i < 10000; i++) {
        OHOS::NativeRdb::ValuesBucket contactDataValues;
        std::string name("xiaoyan");
        name.append(std::to_string(i + 1));
        contactDataValues.PutInt("raw_contact_id", rawContactId);
        contactDataValues.PutString("content_type", "name");
        contactDataValues.PutString("detail_info", name);
        values.push_back(contactDataValues);
    }
    HILOG_INFO("contact_data_insert_performance_test_1300  start! ");
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    int batchInsertCode = contactsDataAbility.BatchInsert(uriContactData, values);
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    HILOG_INFO("contact_data_insert_performance_test_1300 : time is %{public}d", elaps);
    ASSERT_LE(elaps, TIME_USEC_CONTACT_DATA_INSERT);
    EXPECT_EQ(batchInsertCode, 0);
}

/*
 * @tc.number  contact_data_update_performance_test_1400
 * @tc.name    contact_data performance testing update 10000
 * @tc.desc    update 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, contact_data_update_performance_test_1400, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- contact_data_update_performance_test_1400 is starting! ---");
    OHOS::Uri uriContactData(ContactsUri::CONTACT_DATA);
    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutString("syn_1", "test");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    int updateCode = contactsDataAbility.Update(uriContactData, updateValues, predicates);
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    HILOG_INFO("contact_data_update_performance_test_1400 : time is %{public}d", elaps);
    ASSERT_LE(elaps, TIME_USEC_CONTACT_DATA_UPDATE);
    EXPECT_EQ(updateCode, 0);
}

/*
 * @tc.number  contact_data_query_performance_test_1500
 * @tc.name    contact_data performance testing query 10000
 * @tc.desc    query 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, contact_data_query_performance_test_1500, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- contact_data_query_performance_test_1500 is starting! ---");
    OHOS::Uri uriContactData(ContactsUri::CONTACT_DATA);
    std::vector<std::string> columns;
    columns.push_back("raw_contact_id");
    columns.push_back("detail_info");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet =
        contactsDataAbility.Query(uriContactData, columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_GT(rowCount, 9999);
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    resultSet->Close();
    HILOG_INFO("contact_data_query_performance_test_1500 : time is %{public}d", elaps);
    ASSERT_LE(elaps, TIME_USEC_CONTACT_DATA_QUERY);
}

/*
 * @tc.number  contact_data_delete_performance_test_1600
 * @tc.name    contact_data performance testing delete 10000
 * @tc.desc    delete 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, contact_data_delete_performance_test_1600, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- contact_data_delete_performance_test_1600 is starting! ---");
    OHOS::Uri uriContactData(ContactsUri::CONTACT_DATA);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    int deleteCode = contactsDataAbility.Delete(uriContactData, predicates);
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    HILOG_INFO("contact_data_delete_performance_test_1600 : time is %{public}d", elaps);
    ASSERT_LE(elaps, TIME_USEC_CONTACT_DATA_DELETED);
    EXPECT_EQ(deleteCode, 0);
}

/*
 * @tc.number  calllog_insert_performance_test_100
 * @tc.name    calllog calllog performance testing add 10000
 * @tc.desc    add 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, calllog_insert_performance_test_100, testing::ext::TestSize.Level1)
{
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    OHOS::NativeRdb::DataAbilityPredicates predicatesOne;
    predicatesOne.GreaterThan("id", "0");
    HILOG_INFO("calllog_insert_performance_test_100 deleted  start! ");
    calllogAbility.Delete(uriCalllog, predicatesOne);
    HILOG_INFO("--- calllog_insert_performance_test_100 is starting! ---");
    std::vector<OHOS::NativeRdb::ValuesBucket> values;
    for (int i = 0; i < 10000; i++) {
        OHOS::NativeRdb::ValuesBucket calllogValues;
        calllogValues.PutString("phone_number", std::to_string(i + 1));
        values.push_back(calllogValues);
    }
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    int batchInsertCode = calllogAbility.BatchInsert(uriCalllog, values);
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    HILOG_INFO("calllog_insert_performance_test_100 : time is %{public}d", elaps);
    ASSERT_LE(elaps, TIME_USEC_CALL_LOG_INSERT);
    EXPECT_EQ(batchInsertCode, 0);
}

/*
 * @tc.number  calllog_update_performance_test_200
 * @tc.name    calllog calllog performance testing update 10000
 * @tc.desc    update 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, calllog_update_performance_test_200, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_update_performance_test_200 is starting! ---");
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutInt("answer_state", 1);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    int updateCode = calllogAbility.Update(uriCalllog, updateValues, predicates);
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    ASSERT_LE(elaps, TIME_USEC_CALL_LOG_UPDATE);
    HILOG_INFO("calllog_update_performance_test_200 : time is %{public}d", elaps);
    EXPECT_EQ(updateCode, 0);
}

/*
 * @tc.number  calllog_query_performance_test_300
 * @tc.name    calllog calllog performance testing query 10000
 * @tc.desc    query 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, calllog_query_performance_test_300, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_query_performance_test_300 is starting! ---");
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet =
        calllogAbility.Query(uriCalllog, columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_GT(rowCount, 9999);
    resultSet->Close();
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    HILOG_INFO("calllog_query_performance_test_300 : time is %{public}d", elaps);
    ASSERT_LE(elaps, TIME_USEC_CALL_LOG_QUERY);
}

/*
 * @tc.number  calllog_delete_performance_test_400
 * @tc.name    calllog calllog performance testing delete 10000
 * @tc.desc    delete 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, calllog_delete_performance_test_400, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_delete_performance_test_400 is starting! ---");
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    int deleteCode = calllogAbility.Delete(uriCalllog, predicates);
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    HILOG_INFO("calllog_delete_performance_test_400 : time is %{public}d", elaps);
    ASSERT_LE(elaps, TIME_USEC_CALL_LOG_DELETED);
    EXPECT_EQ(deleteCode, 0);
}

/*
 * @tc.number  voicemail_insert_performance_test_500
 * @tc.name    voicemail performance testing delete 10000
 * @tc.desc    delete 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, voicemail_insert_performance_test_500, testing::ext::TestSize.Level1)
{
    OHOS::Uri uriVoiceMail(VoicemailUri::VOICEMAIL);
    OHOS::NativeRdb::DataAbilityPredicates predicatesOne;
    predicatesOne.GreaterThan("id", "0");
    HILOG_INFO("voicemail_insert_performance_test_500 deleted  start! ");
    voicemailAbility.Delete(uriVoiceMail, predicatesOne);
    HILOG_INFO("--- voicemail_insert_performance_test_500 is starting! ---");
    std::vector<OHOS::NativeRdb::ValuesBucket> values;
    for (int i = 0; i < 10000; i++) {
        OHOS::NativeRdb::ValuesBucket voicemailValues;
        voicemailValues.PutString("phone_number", std::to_string(i + 1));
        values.push_back(voicemailValues);
    }
    HILOG_INFO("voicemail_insert_performance_test_500  start! ");
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    int batchInsertCode = voicemailAbility.BatchInsert(uriVoiceMail, values);
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    HILOG_INFO("voicemail_insert_performance_test_500 : time is %{public}d", elaps);
    ASSERT_LE(elaps, TIME_USEC_VOICEMAIL_INSERT);
    EXPECT_EQ(batchInsertCode, 0);
}

/*
 * @tc.number  voicemail_update_performance_test_600
 * @tc.name    voicemail performance testing update 10000
 * @tc.desc    update 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, voicemail_update_performance_test_600, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_update_performance_test_600 is starting! ---");
    OHOS::Uri uriVoiceMail(VoicemailUri::VOICEMAIL);
    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutString("origin_type", "origin");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    int updateCode = voicemailAbility.Update(uriVoiceMail, updateValues, predicates);
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    HILOG_INFO("voicemail_update_performance_test_600 : time is %{public}d", elaps);
    ASSERT_LE(elaps, TIME_USEC_VOICEMAIL_UPDATE);
    EXPECT_EQ(updateCode, 0);
}

/*
 * @tc.number  voicemail_query_performance_test_700
 * @tc.name    voicemail performance testing query 10000
 * @tc.desc    query 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, voicemail_query_performance_test_700, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_query_performance_test_700 is starting! ---");
    std::string tag("voicemail_query_performance_test_700");
    OHOS::Uri uriVoiceMail(VoicemailUri::VOICEMAIL);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    HILOG_INFO("voicemail_query_performance_test_700  start! ");
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet =
        voicemailAbility.Query(uriVoiceMail, columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_GT(rowCount, 9999);
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    resultSet->Close();
    HILOG_INFO("voicemail_query_performance_test_700 : time is %{public}d", elaps);
    ASSERT_LE(elaps, TIME_USEC_VOICEMAIL_QUERY);
}

/*
 * @tc.number  voicemail_delete_performance_test_800
 * @tc.name    voicemail performance testing delete 10000
 * @tc.desc    delete 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(PerformanceTest, voicemail_delete_performance_test_800, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_delete_performance_test_800 is starting! ---");
    OHOS::Uri uriVoiceMail(VoicemailUri::VOICEMAIL);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    HILOG_INFO("voicemail_delete_performance_test_800  start! ");
    long long startTime, endTime;
    int elaps;
    startTime = GetCurrentTime();
    int deleteCode = voicemailAbility.Delete(uriVoiceMail, predicates);
    endTime = GetCurrentTime();
    elaps = CalcTime(startTime, endTime);
    HILOG_INFO("voicemail_delete_performance_test_800 : time is %{public}d", elaps);
    ASSERT_LE(elaps, TIME_USEC_VOICEMAIL_DELETED);
    EXPECT_EQ(deleteCode, 0);
}

HWTEST_F(PerformanceTest, PerformanceTestDeleted, testing::ext::TestSize.Level1)
{
    DeleteContact();
}
} // namespace Test
} // namespace Contacts