/* Kabuki Toolkit @version 0.x
@link    https://github.com/KabukiStarship/KT.git
@file    /_Seams/Database/00.Core.inl
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright 2019-20 (C) Kabuki Starship <kabukistarship.com>; all rights 
reserved (R). This Source Code Form is subject to the terms of the Mozilla 
Public License, v. 2.0. If a copy of the MPL was not distributed with this file,
You can obtain one at <https://mozilla.org/MPL/2.0/>. */
#pragma once
#include <_Config.h>
#if SEAM == KABUKI_TOOLKIT_HYPERTEXT_CORE
#include <Script2/_Debug.inl>
#else
#include <Script2/_Release.inl>
#endif
using namespace _;
namespace KT {
namespace Database {
inline const CHA* Core(CHA* seam_log, CHA* seam_end, const CHA* args) {
#if SEAM >= KABUKI_TOOLKIT_PRO_0
  A_TEST_BEGIN;

#endif
  return 0;
}
}  // namespace Database
}  // namespace KT
