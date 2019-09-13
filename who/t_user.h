/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki.toolkit.git
@file    /who/t_user.h
@author  Cale McCollough <<https://calemccollough.github.io>>
@license Copyright (C) 2014-9 Cale McCollough <<calemccollough.github.io>>;
All right reserved (R). This Source Code Form is subject to the terms of the 
Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with 
this file, You can obtain one at <https://mozilla.org/MPL/2.0/>. */

#pragma once
#include <pch.h>
#if SEAM >= kabuki.toolkit_WHO_1
#ifndef kabuki.toolkit_WHO_USER
#define kabuki.toolkit_WHO_USER
#include "t_authenticator.h"
#include "t_handle.h"
#include "t_password.h"

namespace _ {

/* A user account on a computer.
A user does not necessarily have an account. Some accounts are and any user
can use. i.e. the guest account. */
class TUser {
 public:
  enum {
    kDefaultMinDislpayNameLength = 2,  //< Default max display name length.
    kMaxStatusLength = 63,             //< Default max display name length.
    kDefaultValue = 0,                 //< Default abstract user value.
  };
  static const FP8 kDefaultBalance;  //< Default account balance.

  /* Creates a user with the given handle, password, and status. */
  TUser(TAuthenticator* authenticator, UID uid = 0,
       const TStrand<>& handle = THandle::kDefault,
       const TStrand<>& password = TPassword::kDefault,
       FP8 balance = kDefaultBalance, int64_t v-alue = kDefaultValue)
    : handle_ (authenticator, handle),
    status_ (StrandClone ("")),
    password_ (authenticator, password),
    auth_ (authenticator),
    uid_ (uid),
    session_ (0),
    public_key_ (0),
    balance_ (balance),
    value_ (value),
    slot_ (nullptr) {}

  /* Constructor deep copies the other object. */
  TUser(const TUser& other)
    : handle_ (other.handle_),
    status_ (other.status_),
    password_ (other.password_),
    auth_ (other.auth_),
    session_ (other.session_),
    uid_ (other.uid_),
    public_key_ (other.public_key_),
    response_ (other.response_),
    balance_ (other.balance_),
    value_ (other.value_),
    slot_ (other.slot_) {}

  TUser& operator=(const TUser& other) {
    handle_ = other.handle_;
    status_ = other.status_;
    password_ = other.password_;
    auth_ = other.auth_;
    session_ = other.session_;
    uid_ = other.uid_;
    public_key_ = other.public_key_;
    response_ = other.response_;
    balance_ = other.balance_;
    value_ = other.value_;
    slot_ = other.slot_;
    return *this;
  }

  /* Virtual destructor. */
  virtual ~TUser() {}

  /* Gets the handle's key. */
  const TStrand<>& GetStatus() { return status_; }

  /* Gets the handle's key. */
  virtual const TStrand<>& SetStatus(const TStrand<>& name) {
    if (name == nullptr) {
      return "name can't be nil.";
    }
    status_ = StrandClone (name);
    return nullptr;
  }

  /* Gets a reference to the handle_. */
  THandle& GetHandle() { return handle_; }

  /* Gets a reference to the handle_.GetKey (). */
  const TStrand<>& GetHandleKey() { return handle_.GetKey (); }

  /* Gets a reference to the password. */
  TPassword& GetPassword() { return password_; }

  /* Gets the user's uid. */
  UID GetUid() { return uid_; }

  /* Gets the session uid. */
  int32_t GetSession() { return session_; }

  /* Sets the session uid. */
  virtual const TStrand<>& SetSession(int32_t session) {
    if (session == 0) return "Invalid session key";
    session_ = session;
    return nullptr;
  }

  /* Gets the session uid. */
  UID GetSessionKey();

  /* Gets the abstract response code. */
  UID GetResponse();

  /* Sets the abstract response code. */
  virtual const TStrand<>& SetResponse(UID response) {
    response_ = response;
    return nullptr;
  }

  /* Gets the abstract balance. */
  FP8 GetBalance() { return balance_; }

  /* Sets the abstract balance. */
  virtual const TStrand<>& SetBalance(FP8 balance) {
    balance_ = balance;
    return nullptr;
  }

  /* Attempts to buy the given points.
      @returns false if the balance_ is too low. */
  BOL BuyValue(int64_t num_coins, FP8 point_cost) {
    // Right now we're not checking how much money the player has.
    FP8 cost = point_cost * (FP8)num_coins;

    if (cost > balance_) {
      cout << "\n| Can't buy coins because your ass is broke.";
      return false;
    }
    value_ += num_coins;
    balance_ -= cost;
    return true;
  }

  /* Increase the balance_ by the given amount. */
  BOL AddBalance(FP8 amount) {
    FP8 balance = balance_;
    balance_ = balance + amount;
    return balance;
  }

  /* Gets the abstract value. */
  int64_t GetValue() { return value_; }

  /* Increase the balance_ by the given amount. */
  int64_t AddValue(int64_t amount) {
    int64_t value = value_;
    value_ = value + amount;
    return value;
  }

  /* Sets the abstract value. */
  virtual const TStrand<>& SetValue(int64_t value) {
    value_ = value;
    return nullptr;
  }

  /* Returns true if this user is the same as the given one.  */
  BOL Equals(TUser* user) {
    if (user == nullptr) {
      return false;
    }
    if (!handle_.Equals (handle_.GetKey ())) {
      return false;
    }
    return password_.Equals (user->password_);
  }

  /* Gets the Script Slot to send messages to this User. */
  Expr* GetSlot() { return slot_; }

#if SCRIPT_USING_MIRROR
  /* Gets the Script Slot to send messages to this User. */
  Mirror* GetMirror();
#endif  //< SCRIPT_USING_MIRROR

  /* Returns true if this session and cypher match the same as the given
      one.  */
  BOL IsAuthentic(int32_t session, UID public_key) {
    if (session_ != session) {
      return false;
    }
    if (public_key_ != public_key) {
      return false;
    }
    return true;
  }

  /* Prints this object to a expression. */
  template<typename Printer>
    Printer& Print (Printer& o) {
    return o << "\n| User: @" << handle_.GetKey () << " password:\""
             << password_.GetKey () << "\" balance:" << balance_
             << " value:" << value_;
  }

  /* @todo Convert to Operation.
  const TStrand<>& Sudo(const TStrand<>& text, const TStrand<>& strand_end) {
    enum {
      kMessageLength = 141,
    };
    const TStrand<>& token;
    FP8 balance;
    int64_t value;
    CH1 input[kMessageLength];

    text = TextSkipSpaces(text, strand_end);
    if (!text) {
      return nullptr;
    }
    if (TokenCompare(text, strand_end, "AddBalance")) {
      token = TextRead(text, strand_end, balance);
      if (!token) {
        return nullptr;
      }
      balance_ += balance;
    } else if (TokenCompare(text, strand_end, "AddValue")) {
      token = TextRead(text, strand_end, value);
      if (!token) {
        return nullptr;
      }
      value_ += value;
    } else if (TokenCompare(text, strand_end, "SetStatus")) {
      token = TextRead(text, strand_end, input, input + kMessageLength);
      if (!token) {
        return nullptr;
      }
      SetStatus(input);
    } else if (TokenCompare(text, strand_end, "SetHandle")) {
      token = TextRead(text, strand_end, input, input + kMessageLength);
      if (!token) {
        return nullptr;
      }
      if (!auth_->HandleIsValid(input)) {
        return "\n| Error: Password in invalid format!";
      }
      handle_.SetKey(input);
    } else if (TokenCompare(text, strand_end, "SetPassword")) {
      token = TextRead(text, strand_end, input, input + 141);
      if (!token) {
        return nullptr;
      }
      if (auth_->PasswordInvalid(input)) {
        return "\n| Error: Password in invalid format!";
      }
      password_.SetKey(input);
    } else if (TokenCompare(text, strand_end, "print")) {
      cout << Print();
      return text;
    }

    token = TextRead(text, strand_end, input, input + kMessageLength);
    if (!token) {
      return nullptr;
    }
    return token;
  }*/

  /* Abstract Script Operation(s).
  @param index The index of the expression.
  @param expr  The Expr to read and write from.
  @return      Returns null upon success, a Set header upon query, and an
  error_t ticket upon Read-Write failure. */
  virtual const Op* Star(uint index, Expr* expr) {
    static const Op kThis{ "User", OperationCount (0), OperationFirst ('A'),
                         "A user account.", 0 };
    switch (index) {
    case '?':
      return ExpressionOperand (expr, &kThis);
    }
    return nullptr;
  }

 private:
  THandle handle_;        //< User's handle (i.e. key).
  TStrand<>& status_;     //< User's status
  TPassword password_;     //< User's password.
  TAuthenticator* auth_;  //< Handle and Password authenticator.
  int32_t session_;       //< Session index.
  UID uid_,             //< Index of user in the UserList.
      public_key_,        //< Fake session encryption key.
      response_;          //< Gets user abstract response code.
  FP8 balance_;           //< Abstract user account balance or money.
  int64_t value_;         //< Abstract account value, points, or coins.
  Expr* slot_;            //< Portal to User's machine.

};
}       // namespace _
#endif  //< kabuki.toolkit_WHO_USER
#endif  //< #if SEAM >= kabuki.toolkit_WHO_1
