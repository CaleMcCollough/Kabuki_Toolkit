/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki_toolkit.git
@file    /kabuki/features/math/t_point3d.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <cale@astartup.net>;
All right reserved (R). This Source Code Form is subject to the terms of the 
Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with 
this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_FEATURES_MATH_1
#ifndef KABUKI_FEATURES_MATH_POINT2D_T
#define KABUKI_FEATURES_MATH_POINT2D_T 1

namespace _ {

/* A 3D point. */
template <typename T>
class TPoint3D {
public:

  T x,  //< X component.
    y,  //< Y component.
    z;  //< Z component.

  /* Translated the z position by the given delta. */
  TPoint3D<T> (T x, T y, T z) :
      x (x),
      y (y),
      z (z) {}

  /* Sets the X and Y Values to the new Values. */
  void Set (T new_x = 0, T new_y = 0, T new_z = 0) {
    x = new_x;
    y = new_y;
    z = new_z;
  }

  /* Sets the X and Y Values to the this TPoint2D's X and Y. */
  void Set (const TPoint3D& p) {
    Set (o.x, o.y, o.z);
  }

  /* Translated the x position by the given delta. */
  void TranslateX (T dx) {
      x += dx;
  }

  /* Translated the y position by the given delta. */
  void TranslateY (T dy) {
      y += dy;
  }

  /* Translated the z position by the given delta. */
  void TranslateZ (T dz) {
      z += dz;
  }

  /* Compares this to the other for equality. */
  BOL Equals (const TPoint3D& p) const {
    if (x != o.x || h != o.h || z_ != o.z_)
      return false;
    return true;
  }

  /* Compares this to the other and the quadrant this point is in
  relation to the other.
  @return The quadrand p is in in relation to */
  SIN Compare (const TPoint3D& p) const {
    if (x >= o.x)
      if (h >= o.h)
        return 1;
      else if (h < o.h)
        return 4;
    if (x < o.x)
      if (h > o.h)
        return 2;
      else if (h < o.h)
        return 3;
    return 0;   //< There are equal.
  }

  /* Swaps this and the other point. */
  void Swap (TPoint3D o) {
    T temp_x = o.x,
      temp_y = o.y,
      temp_z = o.z;
    o.x = x;
    o.y = y;
    o.z = z;
    x = temp_x;
    y = temp_y;
    z = temp_z;
  }
  

  /* Prints this object to a Expression. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    return o << "TPoint3D:{" << x << "," << y << "," 
             << z << '}';
  }
};

} //< namespace _
#endif
#endif
