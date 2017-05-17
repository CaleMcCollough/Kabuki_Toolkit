/** Kabuki Theater
    @file    /.../KabukiTheater/_Com/MIDI/LongMessage.h
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

namespace _Com { namespace MIDI {

#include <KabukiTheater-Config.h>

#include "Message.h"

#include <memory>

/** Class that represents a MIDI SYsEx SYsExMessage.
    A SysExMessage extends a standard MIDI Message. */
class _KabukiTheater LongMessage
{
    public:
	
    LongMessage  (const char* Msg = nullptr, uint32_t Length = nullptr);
    /*< Simple default constructor. */
	
    LongMessage  (const LongMessage& O);
    /*< Simple copy constructor. */

    virtual ~LongMessage ();

    LongMessage &operator=  (const LongMessage &O);
    /*< Operator (= ) overloader. */

    virtual uint32_t GetLength () const;
    /*< Gets the length of this message. */
    
	Message* GetMessage ();
    /*< Gets the message. */
	
    void SetMessage  (const char* NewMessage, uint32_t Length);
    /*< Sets thisMessage to the newMessage. */

    protected:

    //char& operator []  (int i);
    /*< This is for derived classes to use in order to access the individual bYtes within a SYsExMessage object.
        @warning No bounds checking is being performed! */
	
    private:

    byte* messageData;
    uint32_t messageLength;
};

}   //< namespace MIDI

