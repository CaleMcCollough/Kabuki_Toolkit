/* Kabuki Toolkit @version 0.x
@link    https://github.com/KabukiStarship/KabukiToolkit.git
@file    /Touch/mixer_channel.hpp
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright (C) 2014-20 Cale McCollough; all right reserved (R). 
This Source Code Form is subject to the terms of the Mozilla Public License, 
v. 2.0. If a copy of the MPL was not distributed with this file, You can 
obtain one at https://mozilla.org/MPL/2.0/. */
#pragma once
#ifndef KABUKI_TOOLKIT_TOUCH_MIXERCHANNEL
#define KABUKI_TOOLKIT_TOUCH_MIXERCHANNEL
#include <_Config.h>
#if SEAM >= KABUKI_TOOLKIT_TOUCH_CORE
namespace _ {

/* A mixer channel Operation. */
class LIB_MEMBER MixerChannel : public Op {
  NONCOPYABLE(MixerChannel)
 public:
  enum {
    DefaultNumAuxSends = 3,  //< Default number of aux sends.
  };

  /* Constructs a mixer channel with the given name. */
  MixerChannel(const CHA* initName = nullptr);

  /* Destructor. */
  ~MixerChannel();

  /* Gets th level of this channel. */
  ISC GetVolume();

  /* Gets th pan of of the channel. */
  ISC GetPan();

  /* Gets if this channel is muted. */
  ISC IsMuted();

  /* Gets if this channel is soloed. */
  ISC IsSoloed();

  /* Sets the channel level to the value. */
  void setVolume(ISC value);

  /* Sets the channel pan to the value. */
  void SetPan(ISC value);

  /* Either mutes or unmutes this channel. */
  void SetMute(BOL isMuted);

  /* Either solos or unsolos this channel. */
  void SetSolo(BOL isSoloed);

  /* Toggles the mute variable. */
  void ToggleMute();

  /* Toggles the solo variable. */
  void ToggleSolo();

  /* Gets thisAux level. */
  ISC GetAux(ISC thisAux);

  /* Sets thisAuxSend level. */
  void SetAux(ISC thisAuxSend, ISC level);

  /* Deletes the aux send channel at thisIndex.
  @warning This function does not trim the auxSend array size for speed
  considerations Inserts an aux send at thisIndex. */
  void DeleteAux(ISC thisIndex);

  /* Script Operations. */
  const Op Star(uint index, Expr expr);

 private:
  const CHA* label_text_;   //< Name of this channel.
  ISC vol_value_,           //< Volume value for this channel.
      pan_value_,           //< Pan value for this channel.
      mute_value_,          //< Mute value for this channel.
      solo_value_,          //< Solo value for this channel.
      num_aux_sends_;       //< Number of aux sends.
  ISC* aux_send_;           //< Array of aux send levels.

};  //< class MixerChannel
}  // namespace _
#endif
#endif
