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

#include "contacts_data_ability.h"

#include <mutex>

#include "common.h"
#include "contacts_columns.h"
#include "contacts_common_event.h"
#include "data_ability_predicates.h"
#include "database_disaster_recovery.h"
#include "file_utils.h"
#include "hilog_wrapper.h"
#include "profile_database.h"
#include "rdb_predicates.h"
#include "sql_analyzer.h"
#include "uri_utils.h"

namespace OHOS {
namespace AppExecFwk {
REGISTER_AA(ContactsDataAbility);
namespace {
std::mutex g_mutex;
}
std::shared_ptr<Contacts::ContactsDataBase> ContactsDataAbility::contactDataBase_ = nullptr;
std::shared_ptr<Contacts::ProfileDatabase> ContactsDataAbility::profileDataBase_ = nullptr;
std::map<std::string, int> ContactsDataAbility::uriValueMap_ = {
    {"/com.ohos.contactsdataability/contacts/contact", Contacts::CONTACTS_CONTACT},
    {"/com.ohos.contactsdataability/contacts/deleted_raw_contact", Contacts::CONTACTS_DELETE},
    {"/com.ohos.contactsdataability/contacts/deleted_raw_contact_record", Contacts::CONTACTS_DELETE_RECORD},
    {"/com.ohos.contactsdataability/contacts/raw_contact", Contacts::CONTACTS_RAW_CONTACT},
    {"/com.ohos.contactsdataability/contacts/account", Contacts::ACCOUNT},
    {"/com.ohos.contactsdataability/contacts/raw_contact/query_merge_list", Contacts::QUERY_MERGE_LIST},
    {"/com.ohos.contactsdataability/contacts/raw_contact/split_contact", Contacts::SPLIT_CONTACT},
    {"/com.ohos.contactsdataability/contacts/raw_contact/manual_merge", Contacts::MANUAL_MERGE},
    {"/com.ohos.contactsdataability/contacts/raw_contact/auto_merge", Contacts::AUTO_MERGE},
    {"/com.ohos.contactsdataability/contacts/contact_data", Contacts::CONTACTS_CONTACT_DATA},
    {"/com.ohos.contactsdataability/contacts/contact_type", Contacts::CONTACT_TYPE},
    {"/com.ohos.contactsdataability/contacts/groups", Contacts::CONTACTS_GROUPS},
    {"/com.ohos.contactsdataability/contacts/contact_blocklist", Contacts::CONTACTS_BLOCKLIST},
    {"/com.ohos.contactsdataability/contacts/photo_files", Contacts::CONTACTS_PHOTO_FILES},
    {"/com.ohos.contactsdataability/contacts/search_contact", Contacts::CONTACTS_SEARCH_CONTACT},
    {"/com.ohos.contactsdataability/contacts/backup", Contacts::CONTACT_BACKUP},
    {"/com.ohos.contactsdataability/profile/backup", Contacts::PROFILE_BACKUP},
    {"/com.ohos.contactsdataability/contacts/recover", Contacts::CONTACT_RECOVER},
    {"/com.ohos.contactsdataability/profile/recover", Contacts::PROFILE_RECOVER},
    {"/com.ohos.contactsdataability/profile/contact", Contacts::PROFILE_CONTACT},
    {"/com.ohos.contactsdataability/profile/raw_contact", Contacts::PROFILE_RAW_CONTACT},
    {"/com.ohos.contactsdataability/profile/contact_data", Contacts::PROFILE_CONTACT_DATA},
    {"/com.ohos.contactsdataability/profile/groups", Contacts::PROFILE_GROUPS},
    {"/com.ohos.contactsdataability/profile/contact_blocklist", Contacts::PROFILE_BLOCKLIST},
    {"/com.ohos.contactsdataability/profile/photo_files", Contacts::PROFILE_PHOTO_FILES},
    {"/com.ohos.contactsdataability/profile/search_contact", Contacts::PROFILE_SEARCH_CONTACT},
    {"/com.ohos.contactsdataability/profile/deleted_raw_contact_record", Contacts::PROFILE_DELETE_RECORD},
    {"/com.ohos.contactsdataability/profile/deleted_raw_contact", Contacts::PROFILE_DELETE},
    {"/com.ohos.contactsdataability/profile/contact_type", Contacts::PROFILE_TYPE}
};

ContactsDataAbility::ContactsDataAbility(void)
{
}

ContactsDataAbility::~ContactsDataAbility()
{
}

void ContactsDataAbility::OnStart(const Want &want)
{
    Ability::OnStart(want);
    std::string basePath = GetDatabaseDir();
    Contacts::ContactsPath::RDB_PATH = basePath + "/";
    Contacts::ContactsPath::RDB_BACKUP_PATH = basePath + "/backup/";
    Contacts::ContactsPath::DUMP_PATH = GetFilesDir() + "/";
}

void ContactsDataAbility::Dump(const std::string &extra)
{
    Contacts::FileUtils fileUtils;
    std::string dirStr = Contacts::ContactsPath::DUMP_PATH;
    fileUtils.WriteStringToFileAppend(dirStr, extra);
}

/**
 * @brief ContactsDataAbility BeginTransaction emptiness problems
 *
 * @param code the return number of BeginTransaction
 * @param mutex transmission parameter : lock
 *
 * @return BeginTransaction emptiness true or false
 */
bool ContactsDataAbility::IsBeginTransactionOK(int code, std::mutex &mutex)
{
    mutex.try_lock();
    if (code != 0) {
        HILOG_ERROR("IsBeginTransactionOK fail");
        mutex.unlock();
        return false;
    }
    return true;
}

/**
 * @brief ContactsDataAbility Commit emptiness problems
 *
 * @param code the return number of Commit
 * @param mutex transmission parameter : lock
 *
 * @return Commit emptiness true or false
 */
bool ContactsDataAbility::IsCommitOK(int code, std::mutex &mutex)
{
    mutex.try_lock();
    if (code != 0) {
        HILOG_ERROR("IsCommitOK fail");
        mutex.unlock();
        return false;
    }
    return true;
}

/**
 * @brief ContactsDataAbility insert database
 *
 * @param uri Determine the data table name based on the URI
 * @param value Insert the data value of the database
 *
 * @return Insert database results code
 */
int ContactsDataAbility::Insert(const Uri &uri, const NativeRdb::ValuesBucket &value)
{
    HILOG_INFO("Insert start");
    Contacts::SqlAnalyzer sqlAnalyzer;
    bool isOk = sqlAnalyzer.CheckValuesBucket(value);
    if (!isOk) {
        HILOG_ERROR("ContactsDataAbility CheckValuesBucket is error");
        return Contacts::RDB_EXECUTE_FAIL;
    }
    g_mutex.lock();
    contactDataBase_ = Contacts::ContactsDataBase::GetInstance();
    profileDataBase_ = Contacts::ProfileDatabase::GetInstance();
    int resultId = Contacts::RDB_EXECUTE_FAIL;
    OHOS::Uri uriTemp = uri;
    int code = UriParseAndSwitch(uriTemp);
    int ret = contactDataBase_->BeginTransaction();
    if (!IsBeginTransactionOK(ret, g_mutex)) {
        g_mutex.unlock();
        return Contacts::RDB_EXECUTE_FAIL;
    }
    resultId = InsertExecute(code, value);
    HILOG_INFO("Insert id = %{public}d", resultId);
    if (resultId == Contacts::OPERATION_ERROR) {
        contactDataBase_->RollBack();
        g_mutex.unlock();
        return Contacts::OPERATION_ERROR;
    }
    ret = contactDataBase_->Commit();
    if (!IsCommitOK(ret, g_mutex)) {
        contactDataBase_->RollBack();
        g_mutex.unlock();
        return Contacts::RDB_EXECUTE_FAIL;
    }
    g_mutex.unlock();
    DataBaseNotifyChange(Contacts::CONTACT_INSERT, uri);
    HILOG_INFO("Insert end");
    return resultId;
}

int ContactsDataAbility::InsertExecute(int &code, const NativeRdb::ValuesBucket &value)
{
    int rowId = Contacts::RDB_EXECUTE_FAIL;
    switch (code) {
        case Contacts::CONTACTS_RAW_CONTACT:
        case Contacts::PROFILE_RAW_CONTACT:
            // contact Basic Information
            rowId = contactDataBase_->InsertRawContact(Contacts::ContactTableName::RAW_CONTACT, value);
            break;
        case Contacts::CONTACTS_CONTACT_DATA:
        case Contacts::PROFILE_CONTACT_DATA:
            // contact Information add
            rowId = contactDataBase_->InsertContactData(Contacts::ContactTableName::CONTACT_DATA, value);
            break;
        case Contacts::CONTACTS_GROUPS:
        case Contacts::PROFILE_GROUPS:
            // insert group
            rowId = contactDataBase_->InsertGroup(Contacts::ContactTableName::GROUPS, value);
            break;
        case Contacts::CONTACTS_BLOCKLIST:
        case Contacts::PROFILE_BLOCKLIST:
            // add blocklist
            rowId = contactDataBase_->InsertBlockList(Contacts::ContactTableName::CONTACT_BLOCKLIST, value);
            break;
        default:
            rowId = Contacts::OPERATION_ERROR;
            HILOG_INFO("ContactsDataAbility ====>no match uri action");
            break;
    }
    return rowId;
}

/**
 * @brief ContactsDataAbility BatchInsert database
 *
 * @param uri Determine the data table name based on the URI
 * @param value Insert the data values of the database
 *
 * @return BatchInsert database results code
 */
int ContactsDataAbility::BatchInsert(const Uri &uri, const std::vector<NativeRdb::ValuesBucket> &values)
{
    unsigned int size = values.size();
    if (size <= 0) {
        return Contacts::RDB_EXECUTE_FAIL;
    }
    g_mutex.lock();
    contactDataBase_ = Contacts::ContactsDataBase::GetInstance();
    profileDataBase_ = Contacts::ProfileDatabase::GetInstance();
    OHOS::Uri uriTemp = uri;
    int code = UriParseAndSwitch(uriTemp);
    int ret = contactDataBase_->BeginTransaction();
    if (!IsBeginTransactionOK(ret, g_mutex)) {
        g_mutex.unlock();
        return Contacts::RDB_EXECUTE_FAIL;
    }
    int count = 0;
    for (unsigned int i = 0; i < size; i++) {
        ++count;
        OHOS::NativeRdb::ValuesBucket rawContactValues = values[i];
        int rowRet = InsertExecute(code, rawContactValues);
        if (rowRet == Contacts::OPERATION_ERROR) {
            contactDataBase_->RollBack();
            g_mutex.unlock();
            return rowRet;
        }
        if (count % Contacts::BATCH_INSERT_COUNT == 0) {
            int markRet = contactDataBase_->Commit();
            int beginRet = contactDataBase_->BeginTransaction();
            if (!IsCommitOK(markRet, g_mutex) || !IsBeginTransactionOK(beginRet, g_mutex)) {
                contactDataBase_->RollBack();
                g_mutex.unlock();
                return Contacts::RDB_EXECUTE_FAIL;
            }
        }
    }
    int markRet = contactDataBase_->Commit();
    if (!IsCommitOK(markRet, g_mutex)) {
        g_mutex.unlock();
        return Contacts::RDB_EXECUTE_FAIL;
    }
    DataBaseNotifyChange(Contacts::CONTACT_INSERT, uri);
    g_mutex.unlock();
    return Contacts::OPERATION_OK;
}

/**
 * @brief ContactsDataAbility Update database
 *
 * @param uri Determine the data table name based on the URI
 * @param predicates Update the data value of the condition
 *
 * @return Update database results code
 */
int ContactsDataAbility::Update(
    const Uri &uri, const NativeRdb::ValuesBucket &value, const NativeRdb::DataAbilityPredicates &predicates)
{
    Contacts::SqlAnalyzer sqlAnalyzer;
    bool isOk = sqlAnalyzer.CheckValuesBucket(value);
    if (!isOk) {
        HILOG_ERROR("ContactsDataAbility CheckValuesBucket is error");
        return Contacts::RDB_EXECUTE_FAIL;
    }
    g_mutex.lock();
    contactDataBase_ = Contacts::ContactsDataBase::GetInstance();
    profileDataBase_ = Contacts::ProfileDatabase::GetInstance();
    int retCode = Contacts::RDB_EXECUTE_FAIL;
    OHOS::Uri uriTemp = uri;
    int code = UriParseAndSwitch(uriTemp);
    OHOS::NativeRdb::DataAbilityPredicates dataAbilityPredicates = predicates;
    UpdateExecute(retCode, code, value, dataAbilityPredicates);
    g_mutex.unlock();
    DataBaseNotifyChange(Contacts::CONTACT_UPDATE, uri);
    return retCode;
}

void ContactsDataAbility::UpdateExecute(int &retCode, int code, const NativeRdb::ValuesBucket &value,
    OHOS::NativeRdb::DataAbilityPredicates &dataAbilityPredicates)
{
    Contacts::PredicatesConvert predicatesConvert;
    OHOS::NativeRdb::RdbPredicates rdbPredicates("");
    switch (code) {
        case Contacts::CONTACTS_CONTACT:
        case Contacts::PROFILE_CONTACT:
            break;
        case Contacts::CONTACTS_RAW_CONTACT:
        case Contacts::PROFILE_RAW_CONTACT:
            rdbPredicates =
                predicatesConvert.ConvertPredicates(Contacts::ContactTableName::RAW_CONTACT, dataAbilityPredicates);
            retCode = contactDataBase_->UpdateRawContact(value, rdbPredicates);
            break;
        case Contacts::CONTACTS_CONTACT_DATA:
        case Contacts::PROFILE_CONTACT_DATA:
            rdbPredicates =
                predicatesConvert.ConvertPredicates(Contacts::ContactTableName::CONTACT_DATA, dataAbilityPredicates);
            retCode = contactDataBase_->UpdateContactData(value, rdbPredicates);
            break;
        case Contacts::CONTACTS_GROUPS:
        case Contacts::PROFILE_GROUPS:
            rdbPredicates =
                predicatesConvert.ConvertPredicates(Contacts::ContactTableName::GROUPS, dataAbilityPredicates);
            retCode = contactDataBase_->UpdateGroup(value, rdbPredicates);
            break;
        case Contacts::CONTACTS_BLOCKLIST:
        case Contacts::PROFILE_BLOCKLIST:
            rdbPredicates = predicatesConvert.ConvertPredicates(
                Contacts::ContactTableName::CONTACT_BLOCKLIST, dataAbilityPredicates);
            retCode = contactDataBase_->UpdateBlockList(value, rdbPredicates);
            break;
        default:
            SwitchUpdate(retCode, code, value, dataAbilityPredicates);
            break;
    }
}

void ContactsDataAbility::SwitchUpdate(int &retCode, int &code, const NativeRdb::ValuesBucket &value,
    OHOS::NativeRdb::DataAbilityPredicates &dataAbilityPredicates)
{
    Contacts::PredicatesConvert predicatesConvert;
    OHOS::NativeRdb::RdbPredicates rdbPredicates("");
    switch (code) {
        case Contacts::SPLIT_CONTACT:
            retCode = contactDataBase_->Split(dataAbilityPredicates);
            break;
        case Contacts::MANUAL_MERGE:
            retCode = contactDataBase_->ReContactMerge(dataAbilityPredicates);
            break;
        case Contacts::AUTO_MERGE:
            retCode = contactDataBase_->ContactMerge();
            break;
        case Contacts::CONTACT_BACKUP:
        case Contacts::PROFILE_BACKUP:
            retCode = BackUp();
            break;
        case Contacts::CONTACT_RECOVER:
        case Contacts::PROFILE_RECOVER:
            retCode = Recover(code);
            break;
        default:
            retCode = Contacts::RDB_EXECUTE_FAIL;
            HILOG_ERROR("ContactsDataAbility ====>no match uri action");
            break;
    }
}

/**
 * @brief ContactsDataAbility Delete database
 *
 * @param uri Determine the data table name based on the URI
 * @param predicates Delete the data values of the condition
 *
 * @return Delete database results code
 */
int ContactsDataAbility::Delete(const Uri &uri, const NativeRdb::DataAbilityPredicates &predicates)
{
    HILOG_INFO("ContactsDataAbility ====>Delete");
    g_mutex.lock();
    contactDataBase_ = Contacts::ContactsDataBase::GetInstance();
    profileDataBase_ = Contacts::ProfileDatabase::GetInstance();
    int retCode = Contacts::RDB_EXECUTE_FAIL;
    OHOS::Uri uriTemp = uri;
    int code = UriParseAndSwitch(uriTemp);
    OHOS::NativeRdb::DataAbilityPredicates dataAbilityPredicates = predicates;
    DeleteExecute(retCode, code, dataAbilityPredicates);
    g_mutex.unlock();
    DataBaseNotifyChange(Contacts::CONTACT_DELETE, uri);
    return retCode;
}

void ContactsDataAbility::DeleteExecute(
    int &retCode, int code, OHOS::NativeRdb::DataAbilityPredicates &dataAbilityPredicates)
{
    Contacts::PredicatesConvert predicatesConvert;
    OHOS::NativeRdb::RdbPredicates rdbPredicates("");
    switch (code) {
        case Contacts::CONTACTS_CONTACT:
        case Contacts::PROFILE_CONTACT:
            rdbPredicates =
                predicatesConvert.ConvertPredicates(Contacts::ContactTableName::CONTACT, dataAbilityPredicates);
            retCode = contactDataBase_->DeleteContact(rdbPredicates);
            break;
        case Contacts::CONTACTS_RAW_CONTACT:
        case Contacts::PROFILE_RAW_CONTACT:
            rdbPredicates =
                predicatesConvert.ConvertPredicates(Contacts::ContactTableName::RAW_CONTACT, dataAbilityPredicates);
            retCode = contactDataBase_->DeleteRawContact(rdbPredicates);
            break;
        case Contacts::CONTACTS_CONTACT_DATA:
        case Contacts::PROFILE_CONTACT_DATA:
            rdbPredicates =
                predicatesConvert.ConvertPredicates(Contacts::ContactTableName::CONTACT_DATA, dataAbilityPredicates);
            retCode = contactDataBase_->DeleteContactData(rdbPredicates);
            break;
        case Contacts::CONTACTS_GROUPS:
        case Contacts::PROFILE_GROUPS:
            rdbPredicates =
                predicatesConvert.ConvertPredicates(Contacts::ContactTableName::GROUPS, dataAbilityPredicates);
            retCode = contactDataBase_->DeleteGroup(rdbPredicates);
            break;
        case Contacts::CONTACTS_BLOCKLIST:
        case Contacts::PROFILE_BLOCKLIST:
            rdbPredicates = predicatesConvert.ConvertPredicates(
                Contacts::ContactTableName::CONTACT_BLOCKLIST, dataAbilityPredicates);
            retCode = contactDataBase_->DeleteBlockList(rdbPredicates);
            break;
        case Contacts::CONTACTS_DELETE:
        case Contacts::PROFILE_DELETE:
            rdbPredicates =
                predicatesConvert.ConvertPredicates(Contacts::ViewName::VIEW_DELETED, dataAbilityPredicates);
            retCode = contactDataBase_->CompletelyDelete(rdbPredicates);
            break;
        case Contacts::CONTACTS_DELETE_RECORD:
        case Contacts::PROFILE_DELETE_RECORD:
            rdbPredicates = predicatesConvert.ConvertPredicates(
                Contacts::ContactTableName::DELETE_RAW_CONTACT, dataAbilityPredicates);
            retCode = contactDataBase_->DeleteRecord(rdbPredicates);
            break;
        default:
            retCode = Contacts::OPERATION_ERROR;
            HILOG_ERROR("ContactsDataAbility ====>no match uri action");
            break;
    }
}

/**
 * @brief ContactsDataAbility query database
 *
 * @param uri Determine the data table name based on the URI
 * @param columns Conditions for query operation
 * @param predicates Query the data values of the condition
 *
 * @return Query database results
 */
std::shared_ptr<NativeRdb::AbsSharedResultSet> ContactsDataAbility::Query(
    const Uri &uri, const std::vector<std::string> &columns, const NativeRdb::DataAbilityPredicates &predicates)
{
    HILOG_INFO("ContactsDataAbility ====>Query start");
    g_mutex.lock();
    contactDataBase_ = Contacts::ContactsDataBase::GetInstance();
    profileDataBase_ = Contacts::ProfileDatabase::GetInstance();
    std::shared_ptr<NativeRdb::AbsSharedResultSet> result;
    OHOS::Uri uriTemp = uri;
    int parseCode = UriParseAndSwitch(uriTemp);
    std::vector<std::string> columnsTemp = columns;
    OHOS::NativeRdb::DataAbilityPredicates dataAbilityPredicates = predicates;
    QueryExecute(result, dataAbilityPredicates, columnsTemp, parseCode);
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> sharedPtrResult = std::move(result);
    g_mutex.unlock();
    HILOG_INFO("ContactsDataAbility ====>Query end");
    return sharedPtrResult;
}

void ContactsDataAbility::QueryExecute(std::shared_ptr<NativeRdb::AbsSharedResultSet> &result,
    OHOS::NativeRdb::DataAbilityPredicates &dataAbilityPredicates, std::vector<std::string> &columnsTemp,
    int &parseCode)
{
    Contacts::PredicatesConvert predicatesConvert;
    OHOS::NativeRdb::RdbPredicates rdbPredicates("");
    switch (parseCode) {
        case Contacts::CONTACTS_CONTACT:
        case Contacts::PROFILE_CONTACT:
            rdbPredicates =
                predicatesConvert.ConvertPredicates(Contacts::ViewName::VIEW_CONTACT, dataAbilityPredicates);
            result = contactDataBase_->Query(rdbPredicates, columnsTemp);
            break;
        case Contacts::CONTACTS_RAW_CONTACT:
        case Contacts::PROFILE_RAW_CONTACT:
            rdbPredicates =
                predicatesConvert.ConvertPredicates(Contacts::ViewName::VIEW_RAW_CONTACT, dataAbilityPredicates);
            result = contactDataBase_->Query(rdbPredicates, columnsTemp);
            break;
        case Contacts::CONTACTS_CONTACT_DATA:
        case Contacts::PROFILE_CONTACT_DATA:
            rdbPredicates =
                predicatesConvert.ConvertPredicates(Contacts::ViewName::VIEW_CONTACT_DATA, dataAbilityPredicates);
            result = contactDataBase_->Query(rdbPredicates, columnsTemp);
            break;
        case Contacts::CONTACTS_GROUPS:
        case Contacts::PROFILE_GROUPS:
            rdbPredicates = predicatesConvert.ConvertPredicates(Contacts::ViewName::VIEW_GROUPS, dataAbilityPredicates);
            result = contactDataBase_->Query(rdbPredicates, columnsTemp);
            break;
        default:
            QueryExecuteSwitchSplit(result, dataAbilityPredicates, columnsTemp, parseCode);
            break;
    }
}

void ContactsDataAbility::QueryExecuteSwitchSplit(std::shared_ptr<NativeRdb::AbsSharedResultSet> &result,
    OHOS::NativeRdb::DataAbilityPredicates &dataAbilityPredicates, std::vector<std::string> &columnsTemp,
    int &parseCode)
{
    Contacts::PredicatesConvert predicatesConvert;
    OHOS::NativeRdb::RdbPredicates rdbPredicates("");
    switch (parseCode) {
        case Contacts::CONTACTS_BLOCKLIST:
        case Contacts::PROFILE_BLOCKLIST:
            rdbPredicates = predicatesConvert.ConvertPredicates(
                Contacts::ContactTableName::CONTACT_BLOCKLIST, dataAbilityPredicates);
            result = contactDataBase_->Query(rdbPredicates, columnsTemp);
            break;
        case Contacts::CONTACTS_DELETE:
        case Contacts::PROFILE_DELETE:
            rdbPredicates =
                predicatesConvert.ConvertPredicates(Contacts::ViewName::VIEW_DELETED, dataAbilityPredicates);
            result = contactDataBase_->Query(rdbPredicates, columnsTemp);
            break;
        case Contacts::CONTACTS_SEARCH_CONTACT:
        case Contacts::PROFILE_SEARCH_CONTACT:
            rdbPredicates =
                predicatesConvert.ConvertPredicates(Contacts::ViewName::SEARCH_CONTACT_VIEW, dataAbilityPredicates);
            result = contactDataBase_->Query(rdbPredicates, columnsTemp);
            break;
        case Contacts::QUERY_MERGE_LIST:
            result = contactDataBase_->SelectCandidate();
            break;
        case Contacts::CONTACT_TYPE:
        case Contacts::PROFILE_TYPE:
            rdbPredicates =
                predicatesConvert.ConvertPredicates(Contacts::ContactTableName::CONTACT_TYPE, dataAbilityPredicates);
            result = contactDataBase_->Query(rdbPredicates, columnsTemp);
            break;
        case Contacts::ACCOUNT:
            rdbPredicates =
                predicatesConvert.ConvertPredicates(Contacts::ContactTableName::ACCOUNT, dataAbilityPredicates);
            result = contactDataBase_->Query(rdbPredicates, columnsTemp);
            break;
        default:
            HILOG_ERROR("ContactsDataAbility ====>no match uri action");
            break;
    }
}

int ContactsDataAbility::UriParseAndSwitch(Uri &uri)
{
    Contacts::UriUtils uriUtils;
    int parseCode = uriUtils.UriParse(uri, uriValueMap_);
    if (parseCode != Contacts::OPERATION_ERROR) {
        SwitchProfile(uri);
    }
    return parseCode;
}

void ContactsDataAbility::SwitchProfile(Uri &uri)
{
    std::vector<std::string> pathVector;
    uri.GetPathSegments(pathVector);
    if (pathVector.size() > 1 && pathVector[1].find("profile") == std::string::npos) {
        contactDataBase_ = Contacts::ContactsDataBase::GetInstance();
        contactDataBase_->store_ = contactDataBase_->contactStore_;
    } else {
        profileDataBase_ = Contacts::ProfileDatabase::GetInstance();
        contactDataBase_->store_ = profileDataBase_->store_;
    }
}

int ContactsDataAbility::BackUp()
{
    std::shared_ptr<OHOS::Contacts::DataBaseDisasterRecovery> instance =
        OHOS::Contacts::DataBaseDisasterRecovery::GetInstance();
    int retCode = instance->SQLliteCheckDb();
    if (retCode == 0) {
        retCode = instance->BackDatabase();
    }
    return retCode;
}

int ContactsDataAbility::Recover(int &code)
{
    std::string name = Contacts::PROFILE_DATABASE_NAME;
    if (code == Contacts::CONTACT_RECOVER) {
        name = Contacts::CONTACT_DATABASE_NAME;
    }
    std::shared_ptr<OHOS::Contacts::DataBaseDisasterRecovery> instance =
        OHOS::Contacts::DataBaseDisasterRecovery::GetInstance();
    int retCode = instance->RecoveryDatabase(name);
    contactDataBase_ = Contacts::ContactsDataBase::GetInstance();
    profileDataBase_ = Contacts::ProfileDatabase::GetInstance();
    return retCode;
}

void ContactsDataAbility::DataBaseNotifyChange(int code, Uri uri)
{
    Contacts::ContactsCommonEvent::SendContactChange(code);
}
} // namespace AppExecFwk
} // namespace OHOS