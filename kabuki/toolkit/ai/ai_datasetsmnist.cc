/* Kabuki Toolkit @version 0.x
@file    /kabuki_toolkit/ai/aiml_mnist_importer.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>

#if SEAM >= KABUKI_TOOLKIT_AI_1
#include "c_datasetsmnist.h"

#include <fstream>
#include <iostream>
using namespace std;

namespace _ {

void DataSetImportMnist() {
  // ifstream is used for reading files
  // We'll read from a file called Sample.dat
  ifstream file("train-labels.idx1-ubyte");

  if (!file) {
    cout << "\nCould open file \"nmist.ts\" for reading!";
    exit(1);
  }

  MnistImages labels;

  while (file) {
    file >> labels.magic_number;
    cout << "\nMagic Number:" << labels.magic_number;

    file >> labels.item_count;
    cout << "\nItem Count:" << labels.item_count;

    file >> labels.row_count;
    cout << "\nWidth:" << labels.row_count;

    file >> labels.column_count;
    cout << " Height:" << labels.column_count;

    size_t n = labels.item_count * labels.row_count * labels.column_count;
    UI1* pixels = new UI1[n];
    for (; n > 0; --n) file >> pixels[n];
  }
}

}  // namespace _
#endif
