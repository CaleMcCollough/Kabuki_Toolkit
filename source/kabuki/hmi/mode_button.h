/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/mode_button.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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
 
#ifndef HEADER_FOR_KABUKI_HMI_MODEBUTTON
#define HEADER_FOR_KABUKI_HMI_MODEBUTTON

#include "button.h"

namespace kabuki { namespace hmi { namespace Buttons {

class KABUKI MacroButton;
class KABUKI MacroEvent;
class KABUKI Device;
class KABUKI ControlsPage;

/** A ModeButton is a MacroButton that has the ability to change the Device on a ControlSurfaces.
    A Device button is capable of changing the Device on a ControlSurface and triggering a MacroEvent. 
*/
class KABUKI ModeButton : public Button {
    public:

    /** Constructor. */
    ModeButton  (const char        * label = "", 
                 const MacroEvent  & macro = MacroEvent (),
                 const Device      & device = Device::Dummy,
                 const ControlsPage& page = ControlsPage::blank);
        
    /** Copy constructor. */
    ModeButton  (const ModeButton& other);
    
    /** Destructor. */
    ~ModeButton ();
    
    /** The function Called when this Button gets pressed. */
    void Press  (ButtonEvent buttonEvent);
    
    void Depress  (ButtonEvent buttonEvent);
    
    /** Function Called when this Button gets double pressed. */
    void Depress  (ButtonEvent buttonEvent);

    /** Device loaded when this macro gets triggered. */
    Device *focusDevice ();
    
    /** Gets a pointer to page to focus on. */
    ControlsPage *focusPage ();

    /** Sets this Device's focus to device. */
    int setDevice  (Device *device);
    
    /** Sets the focusPage to page. */
    int SetFocusPage  (ControlsPage *page);
    
    /** Prints this object to a terminal. */
    void Print ();

    private:

    Device      * device_; //< Device to load for this macro.
    ControlsPage* page_;   //< Last saved page for switching macros.
    
};      /** class ModeButton
}       /** namespace Buttons
}       /** namespace hmi
#endif  /** HEADER_FOR_KABUKI_HMI_MODEBUTTON
