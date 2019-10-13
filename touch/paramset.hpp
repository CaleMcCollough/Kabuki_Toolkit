/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki.toolkit.git
@file    /touch/paramset.hpp
@author  Cale McCollough <https://cale-mccollough.github.io>
@license Copyright (C) 2014-9 Cale McCollough; all right reserved (R). 
This Source Code Form is subject to the terms of the Mozilla Public License, 
v. 2.0. If a copy of the MPL was not distributed with this file, You can 
obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once
#include <_config.h>
#if SEAM >= KABUKI_TOOLKIT_AV_1
#ifndef KABUKI_TOOLKIT_AV_PARAMETERSET
#define KABUKI_TOOLKIT_AV_PARAMETERSET

#include "param.hpp"

namespace _ {

/* A set of Parameter<T> that can be addressed by index.
 */
template <typename T>
class LIB_MEMBER ParamSet {
 public:
  enum {
    kMinNumElements = 2,    //< The minimum number of columns.
    kMaxNumElements = 256,  //< The minimum number of columns.
  };

  /* Default constructor. */
  ParamSet(ISC newNumControls = kMinNumElements) {
    newNumControls = BoundRange (newNumControls, 0, ParamSet::kMaxNumElements);
    ParamCount() = newNumControls;
    controls = new Parameter<T>*[newNumControls];
    for (ISC i = 0; i < newNumControls; ++i) controls[i] = nullptr;
  }

  /* Constructor creates a row with pointers to the newControls. */
  ParamSet(const TArray<Parameter<T>*>& params) {
    ISC newNumControls = o.size ();

    if (newNumControls < kMinNumElements) {
      ISC i;
      for (i = 0; i < newNumControls; ++i) controls.Push (params[i]);
      for (; i < kMinNumElements; ++i) controls.Push (nullptr);
      return;
    }

    for (ISC i = 0; i < newNumControls; ++i) controls[i] = params[i];
  }

  /* Clones the other object. */
  ParamSet(const ParamSet& o) {
    for (ISC i = 0; i < o.ParamCount (); ++i)
      o.Push (c.controls[i]);
  }

  /* Destructor. */
  ~ParamSet () {}

  /* Adds a new control to the array. */
  void AddParam(Parameter<T>& c) {
    for (ISC i = 0; i < params.ParamCount (); ++i)
      newArray.Push (controls[i]);
  }

  /* Adds a new control to the array.
  @return Returns 0 upon success, and index if it was out of bounds. */
  ISC SetParam(ISC index, Parameter<T>& c) {
    if (index < 0) return -1;
    if (index > getCount ()) return 1;

    return 0;
  }

  /* Deletes the Parameter<T> at the given index. */
  ISC Remove(ISC index) {
    if (index < 0) return nullptr;
    if (index >= getCount ()) return nullptr;

    Parameter<T>* temp = controls[index];
    controls[index] = nullptr;
    return temp;
  }

  /* Gets the parameter at the given index.
  @return gets nullptr if the index if out of bounds. */
  Parameter<T>* Param(ISC index) {
    if (index < 0 || index >= getCount ()) return nullptr;

    return controls[index];
  }

  //< gets the number of parameters in the set.
  ISC ParamCount() const { return params_.Count (); }

  /* Prints this object to a Printer. */
  template<typename Printer>
  Printer& Print(Printer& o) const {
    o << "\nParamSet:" << "ControlCount: " << Count ()
      << LineStrand ('-')
      << Parameter<T>::MacroHeader << kLF
      << LineStrand ('-')
      << "ParamCount():", ParamCount());
    for (ISC i = 0; i < ParamCount(); ++i) {
      auto control = controls[i];
      if (!control) {
        o << "|  nil    |      |      |      |     |     |       |    "
             "|        |      |\n");
      }
      else {
        control->PrintRow (o);
        o << kLF;
      }
    }
    return o << LineStrand ('-');
  }

 private:
  TArray<Parameter<T>> params;  //< An array of Parameter<T> pointers.

  inline ISC BoundRange (ISC value, ISC min, ISC max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
  }
};


}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_PARAMETERSET
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1
