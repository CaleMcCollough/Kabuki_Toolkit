/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_/IEvent.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#pragma once

#include <FreeI2P.hpp>

namespace _ {

class IEvent
/*< Interface with an event. */
{
    public:

    virtual timestamp_t getEventTime () = 0;
    /*< Gets the timestamp of when the event occurred. */
    
    virtual void Trigger (I2P::IObject* source) = 0;
    /*< Triggers the event. */

    virtual void print (I2P::Terminal& slot) = 0;
    /*< Prints this object to the terminal. */
};

}   //< namespace _
