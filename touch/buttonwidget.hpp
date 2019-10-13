/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki.toolkit.git
@file    /touch/buttonwidget.hpp
@author  Cale McCollough <https://cale-mccollough.github.io>
@license Copyright (C) 2014-9 Cale McCollough; all right reserved (R). 
This Source Code Form is subject to the terms of the Mozilla Public License, 
v. 2.0. If a copy of the MPL was not distributed with this file, You can 
obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once
#include <_config.h>
#if SEAM >= KABUKI_TOOLKIT_AV_1
#ifndef KABUKI_TOOLKIT_AV_DEVICEBUTTON
#define KABUKI_TOOLKIT_AV_DEVICEBUTTON

#include "button.hpp"

namespace _ {

/*
class LIB_MEMBER ButtonMacro;
class LIB_MEMBER MacroEvent;
class LIB_MEMBER Widget;
class LIB_MEMBER ControlsPage; */

/* A ButtonWidget is a ButtonMacro that has the ability to change the Widget
on a ControlSurfaces.
A Widget button is capable of changing the Widget on a ControlSurface and
triggering a MacroEvent. */
class LIB_MEMBER ButtonWidget : public ButtonMacro {
 public:
  /* Constructor. */
  ButtonWidget(const CHA* HmiComponent = const CHA* ::empty,
               const MacroEvent& initMacro = MacroEvent(),
               const Widget& initDevice = Widget::empty,
               const ControlsPage& initPage = ControlsPage::blank)
    : Button (label, macro),
    focus_device_ (initState->focus_template_),
    focus_page_ (initiState->focus_page_) {}

  /* Copy constructor. */
  ButtonWidget(const ButtonWidget& o);

  /* Destructor. */
  ~ButtonWidget();

  /* Function that is called when this Button gets pressed. */
  void Press(ButtonEvent event) {
    Button::Press (event);
  }

  /* Function that is called when this Button gets FPD pressed. */
  void Depress (ButtonEvent event) {}

  /* Widget that gets loaded when this macro gets triggered. */
  Widget* FocusDevice () { return focus_device_;  }

  /* Gets a pointer to page to focus on. */
  ControlsPage* FocusPage() { return focus_page_; }

  /* Sets this Widget's focus to device. */
  ISC SetDevice(Widget* d) {
    if (!T) return false;
    focus_template_ = T;
    return true;
  }

  /* Sets the focus_page_ to new_page. */
  BOL SetFocusPage(ControlsPage* new_page) {
    if (!new_page) return false;
    focus_page_ = new_page;
    return true;
  }

  /* Script2 operations. */
  const Op* Star (CHW index, Expr *expr) {
    static const Op this_member = {
        "ButtonDevice", MemberCount (0),
        "An button that brings up a Widget." };
    if (index == 0) return this_member;

    switch (index) {
    case '?':
      const Op m_63 = { "PageButton", MemberCount (0),
                       "Loads a page into the coresponding control layer." };
      if (!io) return m_63;
    }

    return nullptr;
  }

 private:
  Widget* focus_device_;      //< Widget to load for this macro.
  ControlsPage* focus_page_;  //< Last saved page to switch between macros.
};                            //< class ButtonWidget
}  // namespace _
#endif
#endif
