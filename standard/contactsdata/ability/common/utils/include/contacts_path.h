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

#ifndef CONTACTSDATAABILITY_CONTACT_PATH_H
#define CONTACTSDATAABILITY_CONTACT_PATH_H

#include <string>

namespace OHOS {
namespace Contacts {
class ContactsPath {
public:
    ContactsPath();
    ~ContactsPath();
    static std::string RDB_PATH;
    static std::string RDB_BACKUP_PATH;
    static std::string DUMP_PATH;
};
} // namespace Contacts
} // namespace OHOS
#endif // CONTACTSDATAABILITY_CONTACT_PATH_H
