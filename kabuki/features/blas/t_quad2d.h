/* Kabuki Toolkit @version 0.x_
@link    https://github.com/kabuki-starship/kabuki_toolkit.git
@file    /kabuki/features/math/t_quad2d.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <cale@astartup.net>;
All right reserved (R). This Source Code Form is subject to the terms of the 
Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with 
this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_FEATURES_MATH_1
#ifndef KABUKI_FEATURES_MATH_QUAD2D_T
#define KABUKI_FEATURES_MATH_QUAD2D_T 1

#include "t_rect2d.h"

namespace _ {

/* A 2D quadralateral storted as Type types. */
template<typename T>
struct SDK TQuad2D {
  T x0,   //< The x0 coordinate.
    x1,     //< The x1 coordinate.
    x2,     //< The x2 coordinate.
    x3,     //< The x3 coordinate.
    y0,     //< The y0 coordinate.
    y1,     //< The y1 coordinate.
    y2,     //< The y2 coordinate.
    y3;     //< The y2 coordinate.

  /* Constructs an uninitialized quadralateral. */
  TQuad2D () :
    x0 (0),
    x1 (0),
    x2 (0),
    x3 (0),
    y0 (0),
    y1 (0),
    y2 (0),
    y3 (0)
  {
    // Nothing to do here! :-)
  }

  /* Default constructor initializes with given Point. */
  TQuad2D (T initX0, T initY0, T initX1, T initY1, T initX2, T initY2, T initX3,
    T initY3) :
    x1 (initX1),
    x2 (initX1),
    x3 (initX1),
    x4 (initX1),
    y1 (initY1),
    y2 (initY1),
    y3 (initY1),
    y4 (initY1) {
    /// Nothing to do here!
  }

  /* Sets the X and Y Values to the new values. */
  inline void Set (T newX0, T newY0, T newX1, T newY1, T newX2, T newY2, T newX3,
    T newY3) {
    x1 = newX1;
    x2 = newX2;
    x3 = newX3;
    x4 = newX4;
    y1 = newY1;
    y2 = newY2;
    y3 = newY3;
    y4 = newY4;
  }

  /* Sets the quad to a rectangle with the given coordinates. */
  inline void Set (T x, T y, T width, T height) {
    // 1<----4
    // |     ^
    // |     |
    // v     |
    // 2---->3

    x1 = x;
    y1 = y;
    x2 = x;
    y2 = y + height;
    x3 = x + width;
    y3 = y + height;
    x4 = x + width;
    y4 = y;
  }

  /* Sets the xn and yn values to the given rectangle. */
  inline void Set (const TRect2D<T>& r) {
    set (r.getX (), r.getY (), r.Width (), r.Height ());
  }

  /* Sets the X and Y values to the the given point and TArray. */
  inline void Set (const TPoint2D<T>& p, const Vect<T>& v) {
    return Set (p.x, p.y, v.x, v.y);
  }

  /* Sets the */
  inline void Set (const Quad2D<T>& q) {
    return Set (q.x, q.y, q.x, q.y);
  }

  /* Translates the given TArray to the point. */
  inline void Translate (T dx, T dy) {
    y1 += dy;
    x1 += dx;
    y2 += dy;
    x2 += dx;
    y3 += dy;
    x3 += dx;
    y4 += dy;
    x4 += dx;
  }

  /* Translates the given TArray to the point. */
  inline void Translate (const Vect<T>& v) {
    return Translate (v.x, v.y);
  }

  /* Compares this object to the other and returns true if the two are identical. */
  inline BOL Equals (const Quad2D<T>& p) const {
    if (x != q.x || y != q.y)
      return false;
    return true;
  }

  /* Compares this object to the other and returns a numberetical zero true value. */
  inline SI4 Compare (const Quad2D<T>& p) const {
    if (x >= q.x)
      if (y >= q.y)
        return 1;
      else if (y < q.y)
        return 4;
    if (x < q.x)
      if (y > q.y)
        return 2;
      else if (y < q.y)
        return 3;
    return 0;   //< There are equal.
  }

  /* Swaps this object's Point with a. */
  inline void Swap (Quad2D<T> p) {
    T tempX0 = q.x0,
      tempY0 = q.y0;

    q.x0 = x0;
    q.y0 = y0;

    x0 = tempX0;
    y0 = tempY0;

    T tempX0 = q.x0,
      tempY0 = q.y0;

    q.x0 = x0;
    q.y0 = y0;

    x0 = tempX0;
    y0 = tempY0;

    T tempX0 = q.x0,
      tempY0 = q.y0;

    q.x0 = x0;
    q.y0 = y0;

    x0 = tempX0;
    y0 = tempY0;

    T tempX0 = q.x0,
      tempY0 = q.y0;

    q.x0 = x0;
    q.y0 = y0;

    x0 = tempX0;
    y0 = tempY0;
  }

  /* Prints this object to the terminal. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    o << "\nQuad2D:";
  }
};

}       //< namespace _
#endif
#endif

