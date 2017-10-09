/** Kabuki Theater
    @file    ~/Source/_hmi/MacroButton.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
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

#include <KabukiTheater-Config.h>
#include "Parameter<int>.h"

namespace _hmi {

/** Used for Quad Isymmetric control over a Device.
*/
class MasterControls
{
    public:
    
    MasterControls ();
    /*< Constructs a blank set of master controls. */
    
    inline void Print() const;
    /*< Prints this object to a terminal. */
    
    private:

    Parameter<int>* a,
        *b,
        *c,
        *d;
};

}   //< _hmi
}   //< _Theater