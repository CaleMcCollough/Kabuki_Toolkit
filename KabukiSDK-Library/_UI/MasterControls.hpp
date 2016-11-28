/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_UI/MacroButton.hpp
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

#include <KabukiSDK-Config.hpp>
#include "AVControl.hpp"

namespace _Dev { namespace Controls {

/** Used for Quad ISymmetric control over a Device. */
class _KabukiSDK MasterControls
{
    public:
    
    MasterControls ();
    /*< Constructs a blank set of master controls. */
    
    void Print (I2P::Terminal& Slot);
    /*< Prints this object to a terminal. */
    
    private:

    AVControl* a,
        *b,
        *c,
        *d;
};
}   //< namespace Controls
}   //< namespace _Dev

