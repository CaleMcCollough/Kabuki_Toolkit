/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/pch.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once

#define API

#define SEAM_LAYER 0
#define SEAM_MAJOR 0
#define SEAM_MINOR 0

#ifndef INCLUDED_SEAMS
#define INCLUDED_SEAMS

#define SEAM_0_0_00 1
#define SEAM_0_0_01 2
#define SEAM_0_0_02 3
#define SEAM_0_0_03 4
#define SEAM_0_0_04 5
#define SEAM_0_0_05 6
#define SEAM_0_0_06 7
#define SEAM_0_0_07 8

#if SEAM_LAYER == 0
#if SEAM_MAJOR == 0
#if SEAM_MINOR == 0
#define SEAM 1
#elif SEAM_MINOR == 1
#define SEAM 2
#elif SEAM_MINOR == 2
#define SEAM 3
#elif SEAM_MINOR == 3
#define SEAM 4
#elif SEAM_MINOR == 4
#define SEAM 5
#elif SEAM_MINOR == 5
#define SEAM 6
#elif SEAM_MINOR == 6
#define SEAM 7
#elif SEAM_MINOR == 7
#define SEAM 8
#else
#define SEAM 0
#endif
#endif
#if SEAM_MAJOR == 1
#if SEAM_MINOR == 0
#define SEAM_0_1_0 9
#define SEAM 9
#endif
#endif  //< #if SEAM_MAJOR == 0
#endif  //< #if SEAM_LAYER == 0

#define SEAM_N 9
typedef unsigned int uint;
#endif

#include <cstdarg>
#include <cstdint>