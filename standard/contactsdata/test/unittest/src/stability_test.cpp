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

#include "stability_test.h"

#include <sys/time.h>

#include "test_common.h"

namespace Contacts {
namespace Test {
StabilityTest::StabilityTest()
{
}

StabilityTest::~StabilityTest()
{
}

void StabilityTest::DeleteContact()
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
 * @tc.name    raw_contact stability testing add 10000
 * @tc.desc    add 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, raw_contact_insert_performance_test_900, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- raw_contact_insert_performance_test_900 is starting! ---");
    OHOS::Uri uriRawContact(ContactsUri::RAW_CONTACT);
    std::vector<OHOS::NativeRdb::ValuesBucket> values;
    for (int i = 0; i < 10000; i++) {
        OHOS::NativeRdb::ValuesBucket rawContactValues;
        std::string name("小严");
        name.append(std::to_string(i + 1));
        rawContactValues.PutString("display_name", name);
        rawContactValues.PutString("company", "company");
        rawContactValues.PutString("position", "position");
        values.push_back(rawContactValues);
    }
    int batchInsertCode = contactsDataAbility.BatchInsert(uriRawContact, values);
    EXPECT_EQ(batchInsertCode, 0);
}

/*
 * @tc.number  raw_contact_update_performance_test_1000
 * @tc.name    raw_contact stability testing update 10000
 * @tc.desc    update 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, raw_contact_update_performance_test_1000, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- raw_contact_update_performance_test_1000 is starting! ---");
    OHOS::Uri uriRawContact(ContactsUri::RAW_CONTACT);
    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutInt("favorite", 1);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    predicates.And();
    predicates.EqualTo("is_deleted", "0");
    int updateCode = contactsDataAbility.Update(uriRawContact, updateValues, predicates);
    EXPECT_EQ(updateCode, 0);
}

/*
 * @tc.number  raw_contact_query_performance_test_1100
 * @tc.name    raw_contact stability testing query 10000
 * @tc.desc    query 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, raw_contact_query_performance_test_1100, testing::ext::TestSize.Level1)
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
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet =
        contactsDataAbility.Query(uriRawContact, columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_GT(rowCount, 9999);
    resultSet->Close();
}

/*
 * @tc.number  raw_contact_delete_performance_test_1200
 * @tc.name    raw_contact stability testing delete 10000
 * @tc.desc    delete 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, raw_contact_delete_performance_test_1200, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- raw_contact_delete_performance_test_1200 is starting! ---");
    OHOS::Uri uriRawContact(ContactsUri::RAW_CONTACT);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    predicates.And();
    predicates.EqualTo("is_deleted", "0");
    int deleteCode = contactsDataAbility.Delete(uriRawContact, predicates);
    HILOG_INFO("raw_contact_delete_performance_test_1200 : deleteCode = %{public}d", deleteCode);
    EXPECT_EQ(deleteCode, 0);
}

/*
 * @tc.number  contact_data_insert_performance_test_1300
 * @tc.name    contact_data stability testing add 10000
 * @tc.desc    add 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, contact_data_insert_performance_test_1300, testing::ext::TestSize.Level1)
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
    int batchInsertCode = contactsDataAbility.BatchInsert(uriContactData, values);
    EXPECT_EQ(batchInsertCode, 0);
}

/*
 * @tc.number  contact_data_update_performance_test_1400
 * @tc.name    contact_data stability testing update 10000
 * @tc.desc    update 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, contact_data_update_performance_test_1400, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- contact_data_update_performance_test_1400 is starting! ---");
    OHOS::Uri uriContactData(ContactsUri::CONTACT_DATA);
    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutString("syn_1", "test");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    int updateCode = contactsDataAbility.Update(uriContactData, updateValues, predicates);
    EXPECT_EQ(updateCode, 0);
}

/*
 * @tc.number  contact_data_query_performance_test_1500
 * @tc.name    contact_data stability testing query 10000
 * @tc.desc    query 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, contact_data_query_performance_test_1500, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- contact_data_query_performance_test_1500 is starting! ---");
    OHOS::Uri uriContactData(ContactsUri::CONTACT_DATA);
    std::vector<std::string> columns;
    columns.push_back("raw_contact_id");
    columns.push_back("detail_info");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet =
        contactsDataAbility.Query(uriContactData, columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_GT(rowCount, 9999);
    resultSet->Close();
}

/*
 * @tc.number  contact_data_delete_performance_test_1600
 * @tc.name    contact_data stability testing delete 10000
 * @tc.desc    delete 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, contact_data_delete_performance_test_1600, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- contact_data_delete_performance_test_1600 is starting! ---");
    OHOS::Uri uriContactData(ContactsUri::CONTACT_DATA);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    int deleteCode = contactsDataAbility.Delete(uriContactData, predicates);
    EXPECT_EQ(deleteCode, 0);
}

/*
 * @tc.number  calllog_insert_performance_test_100
 * @tc.name    calllog calllog stability testing add 10000
 * @tc.desc    add 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, calllog_insert_performance_test_100, testing::ext::TestSize.Level1)
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
    int batchInsertCode = calllogAbility.BatchInsert(uriCalllog, values);
    EXPECT_EQ(batchInsertCode, 0);
}

/*
 * @tc.number  calllog_update_performance_test_200
 * @tc.name    calllog calllog stability testing update 10000
 * @tc.desc    update 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, calllog_update_performance_test_200, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_update_performance_test_200 is starting! ---");
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutInt("answer_state", 1);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    int updateCode = calllogAbility.Update(uriCalllog, updateValues, predicates);
    HILOG_INFO("calllog_update_performance_test_200 : updateCode = %{public}d", updateCode);
    EXPECT_EQ(updateCode, 0);
}

/*
 * @tc.number  calllog_query_performance_test_300
 * @tc.name    calllog calllog stability testing query 10000
 * @tc.desc    query 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, calllog_query_performance_test_300, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_query_performance_test_300 is starting! ---");
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet =
        calllogAbility.Query(uriCalllog, columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_GT(rowCount, 9999);
    resultSet->Close();
}

/*
 * @tc.number  calllog_delete_performance_test_400
 * @tc.name    calllog calllog stability testing delete 10000
 * @tc.desc    delete 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, calllog_delete_performance_test_400, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- calllog_delete_performance_test_400 is starting! ---");
    OHOS::Uri uriCalllog(CallLogUri::CALL_LOG);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    int deleteCode = calllogAbility.Delete(uriCalllog, predicates);
    EXPECT_EQ(deleteCode, 0);
}

/*
 * @tc.number  voicemail_insert_performance_test_500
 * @tc.name    voicemail stability testing delete 10000
 * @tc.desc    delete 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, voicemail_insert_performance_test_500, testing::ext::TestSize.Level1)
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
    int batchInsertCode = voicemailAbility.BatchInsert(uriVoiceMail, values);
    EXPECT_EQ(batchInsertCode, 0);
}

/*
 * @tc.number  voicemail_update_performance_test_600
 * @tc.name    voicemail stability testing update 10000
 * @tc.desc    update 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, voicemail_update_performance_test_600, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_update_performance_test_600 is starting! ---");
    OHOS::Uri uriVoiceMail(VoicemailUri::VOICEMAIL);
    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutString("origin_type", "origin");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    int updateCode = voicemailAbility.Update(uriVoiceMail, updateValues, predicates);
    EXPECT_EQ(updateCode, 0);
}

/*
 * @tc.number  voicemail_query_performance_test_700
 * @tc.name    voicemail stability testing query 10000
 * @tc.desc    query 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, voicemail_query_performance_test_700, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_query_performance_test_700 is starting! ---");
    OHOS::Uri uriVoiceMail(VoicemailUri::VOICEMAIL);
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("phone_number");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet =
        voicemailAbility.Query(uriVoiceMail, columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_GT(rowCount, 9999);
    resultSet->Close();
}

/*
 * @tc.number  voicemail_delete_performance_test_800
 * @tc.name    voicemail stability testing delete 10000
 * @tc.desc    delete 10000
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(StabilityTest, voicemail_delete_performance_test_800, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- voicemail_delete_performance_test_800 is starting! ---");
    OHOS::Uri uriVoiceMail(VoicemailUri::VOICEMAIL);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    int deleteCode = voicemailAbility.Delete(uriVoiceMail, predicates);
    EXPECT_EQ(deleteCode, 0);
}

HWTEST_F(StabilityTest, PerformanceTestDeleted, testing::ext::TestSize.Level1)
{
    DeleteContact();
}
} // namespace Test
} // namespace Contacts