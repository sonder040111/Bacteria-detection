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

#ifndef CONTACTQUERY_TEST_H
#define CONTACTQUERY_TEST_H

#include "base_test.h"

namespace Contacts {
namespace Test {
class ContactQueryTest : public BaseTest {
public:
    ContactQueryTest();
    ~ContactQueryTest();
    int64_t RawContactInsert(std::string displayName, OHOS::NativeRdb::ValuesBucket &alues);
    int64_t ContactDataInsert(int64_t rawContactId, std::string contentType, std::string detailInfo,
        std::string position, OHOS::NativeRdb::ValuesBucket &contactDataValues);
    std::shared_ptr<OHOS::NativeRdb::AbsSharedResultSet> ContactQuery(const std::string &tableName,
        std::vector<std::string> columns, OHOS::NativeRdb::DataAbilityPredicates predicates);
    void ClearData();
};
} // namespace Test
} // namespace Contacts
#endif // CONTACTQUERY_TEST_H