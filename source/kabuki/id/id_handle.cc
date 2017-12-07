/** kabuki::pro
    @file    ~/source/kabuki/id/handle.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#include <stdafx.h>
#include "handle.h"

using namespace _;
using namespace std;

namespace kabuki { namespace id {

const char Handle::kDefault[] = "user";

Handle::Handle (Validator* validator, const char* key, int min_length,
                int max_length) :
                validator_ (validator),
                type_      (kValidation) {
    if (min_length >= max_length) {
        int temp = min_length;
        min_length = max_length;
        max_length = temp;
    }
    key = key;
    if (min_length < kDefaultMinLength) min_length = kDefaultMinLength;
    else if (min_length > kDefaultMaxLength) min_length = kDefaultMinLength;
    key_ = StringClone (key);
}

const char* Handle::GetKey () { return key_; }

bool Handle::SetKey (const char* key) {
    size_t length = StringLength (key);
    if (length < kDefaultMinLength || length > kDefaultMaxLength)
        return false;

    if (!IsValid (key))
        return false;
    delete key_;
    key_ = StringClone (key);
    return true;
}

bool Handle::IsValid (const char* key) {
    size_t length = StringLength (key);
    if (length < kDefaultMinLength || length > kDefaultMaxLength)
        return false;
    return true;
}

bool Handle::Equals (const Handle& h) {
    return StringEquals (key_, h.key_);
}

bool Handle::Equals (const char* handle) {
    return StringEquals (key_, handle);
}

void Handle::Print () {
    cout << "\n| Handle: " << key_;
}

}       //< id
}       //< kabuki
