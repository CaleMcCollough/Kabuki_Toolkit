/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Id/Person.hpp
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
#include <KabukiSDK-Config.hpp>

namespace _Id {

class _KabukiSDK UID_i
{
    virtual int GetUID () = 0;
    //< Gets the Unique identifier number.

    virtual const char* SetUID  (int Value) = 0;
    //< Sets the Unique identifier number.

    const char* GetName () = 0;
    //< Gets the ID name.

    const char* SetName  (const char* Name) = 0;
    //< Sets the ID name.
    
    void Print (I2P::Terminal& Slot);
    /*< Prints this object to a terminal. */
};
}   //< namespace _Id

