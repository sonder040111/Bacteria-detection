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

#ifndef URI_UTILS_H
#define URI_UTILS_H

#include "uri.h"

#include <map>
#include <string>
#include <vector>

namespace OHOS {
namespace Contacts {
class UriUtils {
public:
    UriUtils();
    ~UriUtils();
    std::vector<std::string> split(const std::string &str, const std::string &split);
    std::map<std::string, std::string> getQueryParameter(OHOS::Uri &uri);
    int UriParse(OHOS::Uri &uri, std::map<std::string, int> &keyMap);
};
} // namespace Contacts
} // namespace OHOS
#endif // URI_UTILS_H
