/** Kabuki Tek
    @file    /.../Source/KabukiTek-Impl/Mixer/MixerPage.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough](calemccollough.github.io)

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

#include <tek/Mixer/MixerPage.h>

namespace tek { namespace Mixer {

#include "Mixer.h"
#include "MixerPage.h"

MixerPage::MixerPage  (Mixer *theMixer, int initStartIndex):
    mixer  (theMixer)
{
    if  (initStartIndex < 0)  // We currently don't have a way to check the upper bounds of the intiStartIndex.
        initStartIndex = 0;
    else
        startingIndex = initStartIndex;


    channel = new MixerPage *[8];  // Init that shit yo!
    for  (int i=0; i < 8; ++i)
        channel[i] = 0;
}

MixerPage::~MixerPage ()
{
    for  (int i=0; i < 8; ++i)
        delete channel[i];
    delete channel;
    delete mixer;
}

int MixerPage::startIndex ()
{
    return startingIndex;
}

void MixerPage::setStartIndex  (int newIndex)
{
    if  (newIndex < 0)
        return;
    if  (mixer != 0)
        if  (newIndex > mixer->numChannels ())
            return;

    startingIndex = newIndex; //< The channel index starts at 1.
}

byte MixerPage::getState ()
{
    return 0;
}

const char* MixerPage::setState (byte Value)
{
    return 0;
}

const char* MixerPage::Op (byte index, Uniprinter* io)
{
    switch (Index)
    {
        case '?': if (!io) return memberDevice<"MixerPage", 0, "tek::Mixer">();
        break;
    }
    
    return invalidIndex;
}

}   //< Mixer
}   //< tek