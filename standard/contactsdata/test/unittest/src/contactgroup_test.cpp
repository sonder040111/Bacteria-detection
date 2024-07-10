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

#include "contactgroup_test.h"

#include "test_common.h"

namespace Contacts {
namespace Test {
ContactGroupTest::ContactGroupTest()
{
}

ContactGroupTest::~ContactGroupTest()
{
}

void ContactGroupTest::ClearData()
{
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    std::string groups = "groups";
    int deleteCode = ContactDelete(groups, predicates);
    EXPECT_EQ(deleteCode, 0);
}

int64_t ContactGroupTest::GroupsInsert(std::string groupName, OHOS::NativeRdb::ValuesBucket &groupValues)
{
    OHOS::Uri uriGroups(ContactsUri::GROUPS);
    groupValues.PutString("group_name", groupName);
    int64_t code = contactsDataAbility.Insert(uriGroups, groupValues);
    return code;
}

int ContactGroupTest::ContactUpdate(const std::string &tableName, OHOS::NativeRdb::ValuesBucket updateValues,
    OHOS::NativeRdb::DataAbilityPredicates predicates)
{
    int code = 0;
    if (tableName == ContactTabName::RAW_CONTACT) {
        OHOS::Uri uriRawContact(ContactsUri::RAW_CONTACT);
        code = contactsDataAbility.Update(uriRawContact, updateValues, predicates);
    } else if (tableName == ContactTabName::CONTACT_DATA) {
        OHOS::Uri uriContactData(ContactsUri::CONTACT_DATA);
        code = contactsDataAbility.Update(uriContactData, updateValues, predicates);
    } else if (tableName == ContactTabName::GROUPS) {
        OHOS::Uri uriGroups(ContactsUri::GROUPS);
        code = contactsDataAbility.Update(uriGroups, updateValues, predicates);
    } else if (tableName == ContactTabName::CONTACT_BLOCKLIST) {
        OHOS::Uri uriBlocklist(ContactsUri::BLOCKLIST);
        code = contactsDataAbility.Update(uriBlocklist, updateValues, predicates);
    } else {
        HILOG_ERROR("ContactsDataAbility ====>no match uri action");
    }
    return code;
}

int ContactGroupTest::ContactDelete(const std::string &tableName, OHOS::NativeRdb::DataAbilityPredicates predicates)
{
    int code = 0;
    if (tableName == ContactTabName::RAW_CONTACT) {
        OHOS::Uri uriRawContact(ContactsUri::RAW_CONTACT);
        code = contactsDataAbility.Delete(uriRawContact, predicates);
    } else if (tableName == ContactTabName::CONTACT_DATA) {
        OHOS::Uri uriContactData(ContactsUri::CONTACT_DATA);
        code = contactsDataAbility.Delete(uriContactData, predicates);
    } else if (tableName == ContactTabName::GROUPS) {
        OHOS::Uri uriGroups(ContactsUri::GROUPS);
        code = contactsDataAbility.Delete(uriGroups, predicates);
    } else if (tableName == ContactTabName::CONTACT_BLOCKLIST) {
        OHOS::Uri uriBlocklist(ContactsUri::BLOCKLIST);
        code = contactsDataAbility.Delete(uriBlocklist, predicates);
    } else if (tableName == ContactTabName::CONTACT) {
        OHOS::Uri uriContact(ContactsUri::CONTACT);
        code = contactsDataAbility.Delete(uriContact, predicates);
    } else {
        HILOG_ERROR("ContactsDataAbility ====>no match uri action");
    }
    return code;
}

std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> ContactGroupTest::ContactQuery(
    const std::string &tableName, std::vector<std::string> columns, OHOS::NativeRdb::DataAbilityPredicates predicates)
{
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet;
    if (tableName == ContactTabName::RAW_CONTACT) {
        OHOS::Uri uriRawContact(ContactsUri::RAW_CONTACT);
        resultSet = contactsDataAbility.Query(uriRawContact, columns, predicates);
    } else if (tableName == ContactTabName::CONTACT_DATA) {
        OHOS::Uri uriContactData(ContactsUri::CONTACT_DATA);
        resultSet = contactsDataAbility.Query(uriContactData, columns, predicates);
    } else if (tableName == ContactTabName::CONTACT) {
        OHOS::Uri uriContact(ContactsUri::CONTACT);
        resultSet = contactsDataAbility.Query(uriContact, columns, predicates);
    } else if (tableName == ContactTabName::GROUPS) {
        OHOS::Uri uriGroups(ContactsUri::GROUPS);
        resultSet = contactsDataAbility.Query(uriGroups, columns, predicates);
    } else if (tableName == ContactTabName::CONTACT_BLOCKLIST) {
        OHOS::Uri uriBlocklist(ContactsUri::BLOCKLIST);
        resultSet = contactsDataAbility.Query(uriBlocklist, columns, predicates);
    } else if (tableName == ContactTabName::DELETED_RAW_CONTACT) {
        OHOS::Uri uriDeletedRawContact(ContactsUri::DELETED_RAW_CONTACT);
        resultSet = contactsDataAbility.Query(uriDeletedRawContact, columns, predicates);
    } else if (tableName == ContactTabName::SEARCH_CONTACT) {
        OHOS::Uri uriSearchContact(ContactsUri::SEARCH);
        resultSet = contactsDataAbility.Query(uriSearchContact, columns, predicates);
    } else {
        HILOG_ERROR("ContactsDataAbility ====>no match uri action");
    }
    return resultSet;
}

void ContactGroupTest::QueryAndExpectResult(const std::string &tableName,
    OHOS::NativeRdb::DataAbilityPredicates predicates, OHOS::NativeRdb::ValuesBucket &values, std::string testName)
{
    std::vector<std::string> columns;
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = ContactQuery(tableName, columns, predicates);
    CheckResultSet(values, resultSet, testName);
}

void ContactGroupTest::GetAllGroupsColumns(std::vector<std::string> &columnInt, std::vector<std::string> &columnStr)
{
    columnInt.push_back("account_id");
    columnInt.push_back("ringtone_modify_time");
    columnInt.push_back("lastest_modify_time");

    columnStr.push_back("group_name");
    columnStr.push_back("group_notes");
    columnStr.push_back("group_ringtone");
}

void ContactGroupTest::MergeColumns(
    std::vector<std::string> &columns, std::vector<std::string> &columnsInt, std::vector<std::string> &columnsStr)
{
    int columnsIntSize = columnsInt.size();
    for (int i = 0; i < columnsIntSize; i++) {
        columns.push_back(columnsInt[i]);
    }
    int columnsStrSize = columnsStr.size();
    for (int i = 0; i < columnsStrSize; i++) {
        columns.push_back(columnsStr[i]);
    }
}

/**
 * @brief get ValuesBucket
 * @params columnsStart column start index
 * @params columnsEnd column end index
 * @return ValuesBucket
 */
OHOS::NativeRdb::ValuesBucket ContactGroupTest::GetAllColumnsValues(
    std::vector<std::string> &columnsInt, std::vector<std::string> &columnsStr)
{
    OHOS::NativeRdb::ValuesBucket valuesBucket;
    int randomInt = 0;
    int columnsIntSize = columnsInt.size();
    for (int i = 0; i < columnsIntSize; i++) {
        randomInt = ContactsRand();
        HILOG_INFO("rand=%{public}d", randomInt);
        valuesBucket.PutInt(columnsInt[i], randomInt);
    }
    std::string randomStr = "";
    int columnsStringSize = columnsStr.size();
    for (int i = 0; i < columnsStringSize; i++) {
        randomStr = columnsStr[i] + std::to_string(ContactsRand());
        valuesBucket.PutString(columnsStr[i], randomStr);
    }

    return valuesBucket;
}

int64_t ContactGroupTest::GroupsInsertValues(OHOS::NativeRdb::ValuesBucket &values)
{
    OHOS::Uri uriGroups(ContactsUri::GROUPS);
    int64_t code = contactsDataAbility.Insert(uriGroups, values);
    return code;
}

/*
 * @tc.number  groups_Insert_test_100
 * @tc.name    Add a single contact group
 * @tc.desc    Add capabilities for contact groups
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(ContactGroupTest, groups_Insert_test_100, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- groups_Insert_test_100 is staring! ---");
    OHOS::NativeRdb::ValuesBucket valuesGroup;
    int64_t groupId = GroupsInsert("personnel", valuesGroup);
    HILOG_INFO("groups_Insert_test_100: groupId = %{public}lld", groupId);
    EXPECT_GT(groupId, 0);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(groupId));
    std::string groups = ContactTabName::GROUPS;
    QueryAndExpectResult(groups, predicates, valuesGroup, "groups_Insert_test_100");
    ClearData();
}

/*
 * @tc.number  groups_Insert_test_200
 * @tc.name    Adding multiple contact groups
 * @tc.desc    Add capabilities for contact groups
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(ContactGroupTest, groups_Insert_test_200, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- groups_Insert_test_200 is staring! ---");
    OHOS::NativeRdb::ValuesBucket valuesGroup;
    int64_t groupId = GroupsInsert("Test a set", valuesGroup);
    HILOG_INFO("groups_Insert_test_200: groupId = %{public}lld", groupId);
    EXPECT_GT(groupId, 0);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(groupId));
    std::string groups = ContactTabName::GROUPS;
    QueryAndExpectResult(groups, predicates, valuesGroup, "groups_Insert_test_200");

    valuesGroup.Clear();
    groupId = GroupsInsert("Test group two", valuesGroup);
    HILOG_INFO("groups_Insert_test_200: groupId = %{public}lld", groupId);
    EXPECT_GT(groupId, 0);
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(groupId));
    QueryAndExpectResult(groups, predicates, valuesGroup, "groups_Insert_test_200");

    valuesGroup.Clear();
    groupId = GroupsInsert("Test three groups", valuesGroup);
    HILOG_INFO("groups_Insert_test_200: groupId = %{public}lld", groupId);
    EXPECT_GT(groupId, 0);
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(groupId));
    QueryAndExpectResult(groups, predicates, valuesGroup, "groups_Insert_test_200");
    ClearData();
}

/*
 * @tc.number  groups_Insert_test_300
 * @tc.name    Add a full field data to the groups table and verify whether the insertion is successful
 * @tc.desc    Added ability to groups
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(ContactGroupTest, groups_Insert_test_300, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----groups_Insert_test_300 is starting!-----");
    std::vector<std::string> columnsInt;
    std::vector<std::string> columnsStr;
    std::vector<std::string> columns;
    std::string groups = ContactTabName::GROUPS;
    GetAllGroupsColumns(columnsInt, columnsStr);
    OHOS::NativeRdb::ValuesBucket valuesBucket = GetAllColumnsValues(columnsInt, columnsStr);
    int rawId = GroupsInsertValues(valuesBucket);
    EXPECT_GT(rawId, 0);

    MergeColumns(columns, columnsInt, columnsStr);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(rawId));
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = ContactQuery(groups, columns, predicates);
    // resultSet count 1
    int rowCount = -1;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(valuesBucket, resultSet, "groups_Insert_test_300");
    HILOG_INFO("-----groups_Insert_test_300 is end!-----");
    ClearData();
}

/*
 * @tc.number  groups_Delete_test_400
 * @tc.name    Single contact group deletion
 * @tc.desc    Delete group ability based on group ID
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(ContactGroupTest, groups_Delete_test_400, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- groups_Delete_test_400 is starting! ---");
    OHOS::NativeRdb::ValuesBucket valuesGroup;
    int64_t groupId = GroupsInsert("Personnel Group", valuesGroup);
    EXPECT_GT(groupId, 0);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(groupId));
    std::string groups = ContactTabName::GROUPS;
    int deleteCode = ContactDelete(groups, predicates);
    EXPECT_EQ(deleteCode, 0);
    std::vector<std::string> columns;
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = ContactQuery(groups, columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    ClearData();
}

/*
 * @tc.number  groups_Delete_test_500
 * @tc.name    Multiple contact groups deleted
 * @tc.desc    Delete group ability based on group ID
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(ContactGroupTest, groups_Delete_test_500, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- groups_Delete_test_500 is starting! ---");
    OHOS::NativeRdb::ValuesBucket valuesGroup;
    int64_t groupIdOne = GroupsInsert("Personnel Group", valuesGroup);
    EXPECT_GT(groupIdOne, 0);

    OHOS::NativeRdb::ValuesBucket valuesGroupTwo;
    int64_t groupIdTwo = GroupsInsert("Sell one", valuesGroupTwo);
    EXPECT_GT(groupIdTwo, 0);

    OHOS::NativeRdb::ValuesBucket valuesGroupThree;
    int64_t groupIdThree = GroupsInsert("Develop a group", valuesGroupThree);
    EXPECT_GT(groupIdThree, 0);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(groupIdOne));
    predicates.Or();
    predicates.EqualTo("id", std::to_string(groupIdTwo));
    predicates.Or();
    predicates.EqualTo("id", std::to_string(groupIdThree));
    std::string groups = ContactTabName::GROUPS;
    int deleteCode = ContactDelete(groups, predicates);
    HILOG_INFO("groups_Delete_test_500: deleteCode = %{public}d", deleteCode);
    EXPECT_EQ(deleteCode, 0);
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(groupIdOne));
    predicates.Or();
    predicates.EqualTo("id", std::to_string(groupIdTwo));
    predicates.Or();
    predicates.EqualTo("id", std::to_string(groupIdThree));
    std::vector<std::string> columns;
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = ContactQuery(groups, columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    ClearData();
}

/*
 * @tc.number  groups_Delete_test_600
 * @tc.name    Delete a full field data in the groups table and verify whether the deletion is successful
 * @tc.desc    Groups deletion capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(ContactGroupTest, groups_Delete_test_600, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----groups_Delete_test_600 is starting!-----");
    // insert
    std::vector<std::string> columns;
    std::vector<std::string> columnsInt;
    std::vector<std::string> columnsStr;
    GetAllGroupsColumns(columnsInt, columnsStr);
    OHOS::NativeRdb::ValuesBucket valuesBucket = GetAllColumnsValues(columnsInt, columnsStr);
    int groupId = GroupsInsertValues(valuesBucket);
    EXPECT_GT(groupId, 0);

    // test end delete data
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(groupId));
    EXPECT_GT(groupId, 0);
    std::string groups = ContactTabName::GROUPS;
    int deleteCode = ContactDelete(groups, predicates);
    EXPECT_EQ(deleteCode, 0);

    MergeColumns(columns, columnsInt, columnsStr);
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetDeleteQuery =
        ContactQuery(groups, columns, predicates);
    int rowCount = -1;
    resultSetDeleteQuery->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    ClearData();
}

/*
 * @tc.number  groups_Query_test_700
 * @tc.name    Query all contact groups
 * @tc.desc    Query all group capabilities
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(ContactGroupTest, groups_Query_test_700, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- groups_Query_test_700 is starting! ---");
    OHOS::NativeRdb::ValuesBucket valuesGroupTwo;
    GroupsInsert("Sell one1", valuesGroupTwo);
    valuesGroupTwo.Clear();
    GroupsInsert("Sell one2", valuesGroupTwo);
    valuesGroupTwo.Clear();
    GroupsInsert("Sell one3", valuesGroupTwo);
    valuesGroupTwo.Clear();
    GroupsInsert("Sell one4", valuesGroupTwo);
    valuesGroupTwo.Clear();
    GroupsInsert("Sell one5", valuesGroupTwo);
    valuesGroupTwo.Clear();
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("group_name");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("id", "0");
    std::string groups = ContactTabName::GROUPS;
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = ContactQuery(groups, columns, predicates);
    int rowCount = 0;
    int currentCount = 5;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(currentCount, rowCount);
    ClearData();
}

/*
 * @tc.number  groups_Update_test_800
 * @tc.name    Single contact group update
 * @tc.desc    Contacts group update capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(ContactGroupTest, groups_Update_test_800, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- groups_Update_test_800 is staring! ---");
    OHOS::NativeRdb::ValuesBucket valuesGroup;
    int64_t groupId = GroupsInsert("physical education", valuesGroup);
    EXPECT_GT(groupId, 0);

    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutString("group_name", std::string("Language"));
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(groupId));
    std::string groups = ContactTabName::GROUPS;
    int updateCode = ContactUpdate(groups, updateValues, predicates);
    EXPECT_EQ(updateCode, 0);
    QueryAndExpectResult(groups, predicates, updateValues, "groups_Update_test_800");
    ClearData();
}

/*
 * @tc.number  groups_Update_test_900
 * @tc.name    Multiple contact group updates
 * @tc.desc    Contacts group update capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(ContactGroupTest, groups_Update_test_900, testing::ext::TestSize.Level1)
{
    HILOG_INFO("--- groups_Update_test_900 is staring! ---");
    OHOS::NativeRdb::ValuesBucket valuesGroupOne;
    int64_t groupIdOne = GroupsInsert("Develop a group", valuesGroupOne);
    EXPECT_GT(groupIdOne, 0);

    OHOS::NativeRdb::ValuesBucket valuesGroupTwo;
    int64_t groupIdTwo = GroupsInsert("Development Group 2", valuesGroupTwo);
    EXPECT_GT(groupIdTwo, 0);

    OHOS::NativeRdb::ValuesBucket valuesGroupThree;
    int64_t groupIdThree = GroupsInsert("Development three groups", valuesGroupThree);
    EXPECT_GT(groupIdThree, 0);

    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutString("group_name", std::string("Develop a group"));
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(groupIdOne));
    std::string groups = ContactTabName::GROUPS;
    int updateCode = ContactUpdate(groups, updateValues, predicates);
    EXPECT_EQ(updateCode, 0);
    QueryAndExpectResult(groups, predicates, updateValues, "groups_Update_test_900");

    updateValues.Clear();
    updateValues.PutString("group_name", std::string("Development Second Group"));
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(groupIdTwo));
    updateCode = ContactUpdate(groups, updateValues, predicates);
    EXPECT_EQ(updateCode, 0);
    QueryAndExpectResult(groups, predicates, updateValues, "groups_Update_test_900");

    updateValues.Clear();
    updateValues.PutString("group_name", std::string("Develop three groups"));
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(groupIdThree));
    updateCode = ContactUpdate(groups, updateValues, predicates);
    EXPECT_EQ(updateCode, 0);
    QueryAndExpectResult(groups, predicates, updateValues, "groups_Update_test_900");
    ClearData();
}

/*
 * @tc.number  groups_Update_test_1000
 * @tc.name    Update a full field data of the groups table and verify whether the update is successful
 * @tc.desc    Support all groups update capability
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(ContactGroupTest, groups_Update_test_1000, testing::ext::TestSize.Level1)
{
    HILOG_INFO("-----groups_Update_test_1000 is starting!-----");
    std::vector<std::string> columns;
    std::vector<std::string> columnsInt;
    std::vector<std::string> columnsStr;
    std::string groups = ContactTabName::GROUPS;
    GetAllGroupsColumns(columnsInt, columnsStr);
    OHOS::NativeRdb::ValuesBucket valuesBucket = GetAllColumnsValues(columnsInt, columnsStr);
    int groupId = GroupsInsertValues(valuesBucket);
    EXPECT_GT(groupId, 0);
    MergeColumns(columns, columnsInt, columnsStr);
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(groupId));
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetOne = ContactQuery(groups, columns, predicates);
    CheckResultSet(valuesBucket, resultSetOne, "groups_Update_test_1000");

    OHOS::NativeRdb::ValuesBucket upDateValuesBucket = GetAllColumnsValues(columnsInt, columnsStr);
    int upDateCode = ContactUpdate(groups, upDateValuesBucket, predicates);
    EXPECT_EQ(upDateCode, 0);
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = ContactQuery(groups, columns, predicates);
    // resultSet count 1
    int rowCount = -1;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);
    CheckResultSet(upDateValuesBucket, resultSet, "groups_Update_test_1000");
    ClearData();
}

/*
 * @tc.number  abnormal_groups_Insert_test_1100
 * @tc.name    An incorrect field or table name was passed in when inserting a group
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(ContactGroupTest, abnormal_groups_Insert_test_1100, testing::ext::TestSize.Level1)
{
    OHOS::Uri uriGroups(ContactsUri::GROUPS);
    OHOS::Uri errorUri(ContactsUri::GROUPS_ERROR);
    OHOS::NativeRdb::ValuesBucket groupValues;
    groupValues.PutString("group_names", "Board of Directors");
    int64_t groupId = contactsDataAbility.Insert(uriGroups, groupValues);
    EXPECT_EQ(groupId, -1);

    groupValues.Clear();
    groupValues.PutString("group_name", "Board of Directors");
    groupId = contactsDataAbility.Insert(errorUri, groupValues);
    EXPECT_EQ(groupId, -1);
    std::vector<std::string> columns;
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("group_name", "Board of Directors");
    std::string groups = ContactTabName::GROUPS;
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = ContactQuery(groups, columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(0, rowCount);
    ClearData();
}

/*
 * @tc.number  abnormal_groups_Delete_test_1200
 * @tc.name    When deleting a group, pass in non-existent fields, field values, and table names
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(ContactGroupTest, abnormal_groups_Delete_test_1200, testing::ext::TestSize.Level1)
{
    OHOS::Uri errorUri(ContactsUri::GROUPS_ERROR);
    OHOS::NativeRdb::ValuesBucket values;
    int64_t groupId = GroupsInsert("Board of Directors 2", values);
    HILOG_INFO("abnormal_groups_Delete_test_1200: groupId = %{public}lld", groupId);
    EXPECT_GT(groupId, 0);

    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("ids", std::to_string(groupId));
    std::string groups = ContactTabName::GROUPS;
    int deleteCode = ContactDelete(groups, predicates);
    HILOG_INFO("abnormal_groups_Delete_test_1200: deleteCode = %{public}d", deleteCode);
    EXPECT_EQ(deleteCode, -1);
    std::vector<std::string> columns;
    predicates.Clear();
    predicates.EqualTo("id", std::to_string(groupId));
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSet = ContactQuery(groups, columns, predicates);
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    EXPECT_EQ(1, rowCount);

    predicates.Clear();
    predicates.EqualTo("id", std::to_string(groupId));
    deleteCode = contactsDataAbility.Delete(errorUri, predicates);
    HILOG_INFO("abnormal_groups_Delete_test_1200: deleteCode = %{public}d", deleteCode);
    EXPECT_EQ(deleteCode, -1);

    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetTwo = ContactQuery(groups, columns, predicates);
    int rowCountTwo = 0;
    resultSetTwo->GetRowCount(rowCountTwo);
    EXPECT_EQ(1, rowCountTwo);

    predicates.Clear();
    predicates.EqualTo("id", "10000000");
    deleteCode = ContactDelete(groups, predicates);
    EXPECT_EQ(deleteCode, 0);
    ClearData();
}

/*
 * @tc.number  abnormal_groups_Query_test_1300
 * @tc.name    When querying a group, pass in non-existent fields, field values, and table names
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(ContactGroupTest, abnormal_groups_Query_test_1300, testing::ext::TestSize.Level1)
{
    std::string tag("abnormal_groups_Query_test_1300");
    OHOS::Uri errorUri(ContactsUri::GROUPS_ERROR);
    std::string groups = ContactTabName::GROUPS;
    std::vector<std::string> columns;
    columns.push_back("id");
    columns.push_back("group_name");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.GreaterThan("ids", "0");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetOne = ContactQuery(groups, columns, predicates);
    int rowCountOne = 0;
    resultSetOne->GetRowCount(rowCountOne);
    EXPECT_EQ(-1, rowCountOne);

    predicates.Clear();
    predicates.EqualTo("id", "10000000");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetTwo = ContactQuery(groups, columns, predicates);
    int rowCountTwo = 0;
    resultSetTwo->GetRowCount(rowCountTwo);
    EXPECT_EQ(0, rowCountTwo);

    predicates.Clear();
    predicates.GreaterThan("id", "0");
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> resultSetThree =
        contactsDataAbility.Query(errorUri, columns, predicates);
    EXPECT_EQ(resultSetThree, nullptr);
    ClearData();
}

/*
 * @tc.number  abnormal_groups_Update_test_1400
 * @tc.name    When modifying a group, pass in non-existent fields, field values, and table names
 * @tc.desc    Exception use case
 * @tc.level   Level1
 * @tc.size    MediumTest
 * @tc.type    Function
 */
HWTEST_F(ContactGroupTest, abnormal_groups_Update_test_1400, testing::ext::TestSize.Level1)
{
    OHOS::Uri errorUri(ContactsUri::GROUPS_ERROR);
    OHOS::NativeRdb::ValuesBucket valuesInsert;
    int64_t groupId = GroupsInsert("Board of Directors", valuesInsert);
    HILOG_INFO("abnormal_groups_Update_test_1400: groupId = %{public}lld", groupId);
    EXPECT_GT(groupId, 0);

    OHOS::NativeRdb::ValuesBucket updateValues;
    updateValues.PutString("group_names", "Develop a group");
    OHOS::NativeRdb::DataAbilityPredicates predicates;
    predicates.EqualTo("id", std::to_string(groupId));
    std::string groups = ContactTabName::GROUPS;
    int updateCode = ContactUpdate(groups, updateValues, predicates);
    HILOG_INFO("abnormal_groups_Update_test_1400: updateCode = %{public}d", updateCode);
    EXPECT_EQ(updateCode, -1);
    QueryAndExpectResult(groups, predicates, valuesInsert, "abnormal_groups_Update_test_1400");

    updateValues.Clear();
    updateValues.PutString("group_name", "Develop a group");
    updateCode = contactsDataAbility.Update(errorUri, updateValues, predicates);
    HILOG_INFO("abnormal_groups_Update_test_1400: updateCode = %{public}d", updateCode);
    EXPECT_EQ(updateCode, -1);
    QueryAndExpectResult(groups, predicates, valuesInsert, "abnormal_groups_Update_test_1400");
    predicates.Clear();

    predicates.EqualTo("id", "10000000");
    updateCode = ContactUpdate(groups, updateValues, predicates);
    EXPECT_EQ(updateCode, 0);
    ClearData();
}
} // namespace Test
} // namespace Contacts