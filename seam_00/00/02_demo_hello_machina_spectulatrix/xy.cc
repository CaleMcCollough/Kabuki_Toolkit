/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/seam_0_0/0_0_0_experiments/maze_agent.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#include "xy.h"

#include "../../kabuki/f2_pico/f2_pico.h"

#include <conio.h>
#include <windows.h>
#include <iostream>
using namespace std;

namespace _ {

const char* XYDirectionAcronyms() {
  // Direction acronyms aligned to a 4-bit boundary.
  static const char kDirectionAcronyms[] = {
      'R', 0,   0, 0,  //
      'U', 'R', 0, 0,  //
      'U', 0,   0, 0,  //
      'U', 'L', 0, 0,  //
      'L', 0,   0, 0,  //
      'L', 'L', 0, 0,  //
      'D', 0,   0, 0,  //
      'L', 'R', 0, 0,  //
      'C', 0,   0, 0,  //
  };
  return kDirectionAcronyms;
}

const char* XYDirectionString(int direction) {
  if (direction < 0 || direction >= kXYCenter) direction = kXYCenter;
  return &XYDirectionAcronyms()[direction << 2];  //< << 2 to * 4.
}

void XYDirectionHistoryPrint(int bits) {
  const char* dir_strings = XYDirectionAcronyms();
  for (int i = 0; i < 8; ++i) {
    if ((bits >> i) & 1) {
      Print(&XYDirectionAcronyms()[i]);
      if (i != 7) Print('\n', ' ');
    }
  }
}

}  // namespace _
