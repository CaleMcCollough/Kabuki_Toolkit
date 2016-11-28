/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Library/KabukiSDK-Impl/_Text/Grammer.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include <_Id/Grammer.hpp>

namespace _Id {

Grammer::Grammer ()
{
    numIllegalCharRanges = 0;
    numMandatoryCharRanges = 0;

    illegalChars = nullptr;
    mandatoryChars = nullptr;
}

Grammer::Grammer (const Grammer& initState)
{
    numIllegalCharRanges = initState.numIllegalCharRanges;
    numMandatoryCharRanges = initState.numMandatoryCharRanges;

/*
    int i;
    if (numIllegalCharRanges)
    {
        illegalChars = new char[numIllegalCharRanges][2];
        for (i = 0; i < numIllegalCharRanges; ++i)
        {
            illegalChars[i][1] = initState.illegalChars[i][1];
            illegalChars[i][2] = initState.illegalChars[i][2];
        }
    }
    else
        illegalChars = nullptr;

    if (numMandatoryCharRanges)
    {
        illegaChars = new char[numIllegalCharRanges][2];
        for (i = 0; i < numIllegalCharRanges; ++i)
        {
            illegalChars[i][1] = initState.illegalChars[i][1];
            illegalChars[i][2] = initState.illegalChars[i][2];
        }
    }
    else
        mandatoryChars = nullptr;
 */
}

bool Grammer::isValid (const char* aString)
{
    /*
       int stringLength = strlen (thisstring);

       if (stringLength  < minLength || stringLength > maxLength)
           return false;

       int i, j; // Looping variables
       for (i = 0; i < ; ++i)
       {
           for (j = 0; j < numIllegalCharRanges; ++j)
               if (thisstring[i])
       }
    */
    return true;
}

int Grammer::getMinLength () { return minLength; }

bool Grammer::getMinLength (int value)
{
    if (value < 0 || value >= maxLength) return false;
    minLength = value;
    return true;
}

int Grammer::getMaxLength () { return maxLength; }

bool Grammer::getMinLength (int value)
{
    if (value < minLength) return false;
    maxLength = value;
    return true;
}

bool Grammer::isIllegal (char aChar)
{
    return true;
}

bool Grammer::makeIllegal (char thisChar, string descstring)
{
    //if (!isprint (thisChar))
     //   return false;
    return true;
}

bool Grammer::makeIllegal (int startIndex, int stopIndex, string descstring)
{
    return true;
}

bool Grammer::makeMandatory (int startIndex, int stopIndex, int numInstances, string descstring)
{
    return true;
}

const char* Grammer::getState (byte Value)
{
    return 0;
}

const char* Grammer::sub (I2P::Terminal& slot, int index, int Enq)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("Grammer", "_Id"): InvalidIndex ();
}

}   //< namespace _Id
