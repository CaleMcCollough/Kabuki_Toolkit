/* Kabuki Toolkit @version 0.x
@link    https://github.com/KabukiStarship/KabukiToolkit.git
@file    /Who/Account.hpp
@author  Cale McCollough <https://cookingwithcale.org>
@license Copyright (C) 2014-20 Cale McCollough; all right reserved (R). 
This Source Code Form is subject to the terms of the Mozilla Public License, 
v. 2.0. If a copy of the MPL was not distributed with this file, You can 
obtain one at https://mozilla.org/MPL/2.0/. */
#pragma once
#ifndef KABUKI_TOOLKIT_ID_ACCOUNT
#define KABUKI_TOOLKIT_ID_ACCOUNT
#include <_Config.h>
#if SEAM >= KABUKI_TOOLKIT_WHO_CORE
#include "Handle.hpp"
#include "Password.hpp"
#include "Roster.hpp"
#include "User.hpp"
namespace _ {
/* An account with a username and password.
*/
class TAccount {
 public:
  /* Constructs an Account with the given CHA. */
  TAccount(const TString<>& username) : name_ (username) {}

  /* Constructs an account from the given CHA and password. */
  TAccount(const TString<>& username, const TString<>& password) 
      : name_ (username), password_ (password) {
    // role = new Role ();
  }

  /* Returns true if this is a valid account. */
  BOL IsValid() {
    /*
    if (password == nullptr)
    {
        // The account does not require a password
        if (!name.IsValid ())
            return false;

        return true;
    }
    else // we have to test the password
    if (!name.IsValid () || !password.IsValid ())
        return false;
    */
    return true;
  }

  /* Gets a reference to the name.  */
  const TString<>& Name() { return name_.GetKey (); }

  /* Sets the name to the CHA. */
  BOL SetName(const TString<>& AString) { return name_.SetKey (AString); }

  /* Returns true if this account requires a password. */
  BOL RequiresPassword() { return password_.Equals (""); }

  /* Sets the password to the new CHA. */
  void SetPassword(const TString<>& AString) { password_.SetKey (AString); }

  /* Gets a reverence to the user list. */
  TRoster& Users() { return users_; }

  /* Allows another user to be attached to this account. */
  void AddUser(TUser& user) { users_.Add (user); }

  /* Prints this object to the log. */
  template<typename Printer>
  Printer Print(Printer& o) {
    o << "Account Name:\"" << name_.GetKey ()
      << "\" Password:\"" << password_.GetKey () << '\"';
    return out
  }

 private:
  THandle name_;        //< Account handle/name.
  TPassword password_;  //< Admin password for this account.
  TRoster users_;       //< Account Roster describes User Roles.
};

}       // namespace _
#endif
#endif
