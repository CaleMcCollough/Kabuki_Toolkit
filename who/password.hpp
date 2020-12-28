/* Kabuki Toolkit @version 0.x
@link    https://github.com/KabukiStarship/KabukiToolkit.git
@file    /Who/password.hpp
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright (C) 2014-20 Cale McCollough; all right reserved (R). 
This Source Code Form is subject to the terms of the Mozilla Public License, 
v. 2.0. If a copy of the MPL was not distributed with this file, You can 
obtain one at https://mozilla.org/MPL/2.0/. */
#pragma once
#ifndef KABUKI_TOOLKIT_WHO_PASSWORD
#define KABUKI_TOOLKIT_WHO_PASSWORD
#include <_Config.h>
#if SEAM >= KABUKI_TOOLKIT_WHO_CORE
#include "Authenticator.hpp"
namespace _ {

/* Interface for a class that can validate a password. */
struct PasswordAuthenticator {
  /* Function validates the password for correctness. */
  virtual const TString<>& IsValid(const TString<>& password) = 0;
};

/* An account password Authenticator .
    @todo Add salt!
*/
class TPassword {
 public:
  enum {
    kValidation = 2,        //< Validation type.
    kDefaultMinLength = 0,  //< The minimum length of a password.
    kMaxLength = 255,       //< The maximum length of a password.
  };

  static const CHA kDefault[];  //< Default password.

  /* Attempts to create a password from the given password with the default
      format.
      If the password does not match the default format, the default password
      will be used.
  */
  TPassword(TAuthenticator* authenticator, const TString<>& password = kDefault)
    : auth_ (authenticator), type_ (kValidation) {
    SetKey (password);
  }

  /* Destructs the password. */
  ~TPassword() {}

  /* Gets the password key. */
  const TString<>& GetKey() { return key_; }

  /* Attempts to set the password and returns a non-zero error message upon
   * failure. */
  BOL SetKey(const TString<>& password) {
    if (password == nullptr) return false;

    if (auth_->PasswordInvalid (password)) {
      return false;
    }
    delete key_;
    key_ = StringClone (password);
    return true;
  }

  /* Gets true if the given password is valid. */
  static BOL IsValid(const TString<>& password) { 
    return true;
  }

  /* Returns true if this password equals the given one. */
  BOL Equals (const TString<>& AString) {
    return StringEquals (key_, key);
  }

  /* Returns true if this password equals the given one. */
  BOL Equals(const TPassword& p) {
    return StringEquals (key_, other.key_);
  }

  /* Prints this object to a expression. */
  template<typename Printer> Printer& Print (Printer& o) { 
    return o << key_;
  }

 private:
  TString<>* key_;        //< Unencrypted password.
  TAuthenticator* auth_;  //< Password authenticator.
  ISC type_;              //< The validation type.
};

}       // namespace _
#endif
#endif
