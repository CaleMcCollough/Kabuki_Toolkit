/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki_toolkit.git
@file    /kabuki/features/math/t_bezier2d.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <cale@astartup.net>;
All right reserved (R). This Source Code Form is subject to the terms of the 
Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with 
this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_FEATURES_MATH_1
#ifndef KABUKI_FEATURES_MATH_BEZIER2D_T
#define KABUKI_FEATURES_MATH_BEZIER2D_T 1

#include "t_point2d.h"

namespace _ {

/* Class that represents a n order Bezier. */
template<typename T>
class Bezier2D {
public:

  enum {
    kCountMin = 3     //< Min number of points reqruied for a bezier curve.
  };

  /* Constructor.
      @contract The length of XPoints and YPoints must equal NumPoints or curve will not be created correctly. */
  Bezier2D (T* initXPoints, T* initYPoints, SI4 initNumPoints) :
    x_points ((FP4*)theXPoints),
    y_points ((FP4*)theYPoints),
    count_ (theNumPoints) {
    DASSERT (theXPoints != nullptr);
    DASSERT (theYPoints != nullptr);
    DASSERT (theNumPoints >= 0);
  }

  /* Constructor.
  @contract The length of XPoints and YPoints must equal NumPoints. */
  Bezier2D (const T* initXPoints, const T* initYPoints, SI4 initNumPoints)
    : x_points ((FP4*)theXPoints),
    y_points ((FP4*)theYPoints),
    count_ (theNumPoints) {
    DASSERT (theXPoints != nullptr);
    DASSERT (theYPoints != nullptr);
    DASSERT (theNumPoints >= 0);
  }

  /* Constructs a quadratic bezier curve with the given points. */
  Bezier2D (T x0, T y0, T x1, T y1, T x2, T y2, T x3, T y3) :
    x_points (new FP4[3]),
    y_points (new FP4[3]),
    count_ (3) {
    count_ = 4;

    x_points[0] = x0;
    y_points[0] = y0;
    x_points[1] = x1;
    y_points[1] = y1;
    x_points[2] = x2;
    y_points[2] = y2;
    x_points[3] = x3;
    y_points[3] = y3;
  }

  /* Copy constructor initializes curve with the given points. */
  Bezier2D (const Bezier2D& o) {
    SI4 newNumPoints = o.count_;
    count_ = newNumPoints;
    FP4* newXPoints = new FP4[newNumPoints],
      *newYPoints = new FP4[newNumPoints];
    x_points = newXPoints;
    y_points = newYPoints;

    for (SI4 i = 0; i < newNumPoints; ++i) {
      newXPoints[i] = o.x_points[i];
      newYPoints[i] = o.y_points[i];
    }
  }

  /* Destructs dynamic memory if used. */
  ~Bezier2D () {
    delete x_points;
    delete y_points;
  }

  /* Retuns a pointer to the x points. */
  T* GetXPoints () { return x_points; }

    /* Retuns a pointer to the y points. */
  T* GetYPoints () { return y_points; }

  /* Gets the number of points. */
  SI4 Count () {
    return count_;
  }

  /* Gets a Point<T> along the path of the of curve at time t. */
  TPoint2D<T> GetPoint (T p) {
    FP4 x = 0,
      y = 0,
      factn = factoral (count_);

    SI4 n = count_;

    for (SI4 i = 0; i <= n; i++) {
      // Calculate binomial coefficient.
      FP4 b = factn / (factoral (i) * factoral (n - i));

      // Calculate powers
      FP4 k = power (1 - p, n - i) * power (p, i);

      // Add weighted points to totals
      x += b * k * x_points[i];
      y += b * k * y_points[i];
    }

    return TPoint3D<FP4> (x, y);
  }

  /* Prints this object to the terminal. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    o << "\nBezier2D:";
    for (SI4 i = 0; i < count_; ++i) {
      o << "\n" << i << ":{x:" << x_points[i] << ",y:"
        << y_points[i] << '}';
    }
    return o;
  }

private:

  T* x_points,   //< Array of X points.
   * y_points;   //< Array of Y points.
  SI4 count_;    //< Number of points in the polygon.
};

}       //< namespace _
#endif
#endif
