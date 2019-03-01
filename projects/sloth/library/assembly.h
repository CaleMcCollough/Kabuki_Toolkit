/* Sloth @version 0.x
@link    https://github.com/kabuki-starship/sloth.git
@file    ~/kabuki/kt/assembly.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <cale@astartup.net>;
All right reserved (R). This Source Code Form is subject to the terms of the 
Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with 
this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

// Choose BARE_METAL, MBED, MBED_OS, ARDUINO, MINGW32, WINDOWS, ANDROID,
// LINUX, OSX, or IOS
#define CRABS_PLATFORM MBED

// Choose X86, X64, ARM8, ARM16, ARM32, or ARM64
#define CRABS_ACHITECTURE X64

#define WORD_SIZE 64

#define MEMORY_ALIGNMENT_REQUIRED NO

#define BIG_ENDIAN IS_NOT_STUPID

/* @brief The Memory Profiles 1-4 are used to signal that the system is
    memory constrained.
    For constrained memory devices, use CRABS_MEMORY_PROFILE 1. For most uses
   use CRABS_MEMORY_PROFILE 2. For applications that require 64-bit memory
   addresses, use CRABS_MEMORY_PROFILE 3.

    @code
    | RUL | sizeof (uint_t) | #bits Unit |
    |:---:|:---------------:|:----------:|
    |  1  |      1 byte     |     5      |
    |  2  |      2 byte     |    10      |
    |  4  |      4 byte     |    16      |
    @endcode
*/
#define CRABS_MEMORY_PROFILE 2

// Max errors before locking up.
#define SCRIPT2_MAX_ERRORS 8

// Max number of parameters.
#define SCRIPT2_MAX_PARAMS 26

// The max CH1 length.
#define CRABS_TEXT_SIZE_DEFAULT 2 * 1024

// Max number of Slots.
#define MAX_NUM_SLOTS 256

// Number of seconds before a timeout over a generic communication link.
#define COM_TIMEOUT_TICKS 100

// Index width in bytes: 8, 16, or 32.
#define CRABS_CHAR_WIDTH 8

// Max number of walls in this Room
#define SCRIPT2_MAX_WALLS 1024

// The size of the default Text<> () buffer in bytes.
#define SCRIPT2_TEXT_LENGTH 2048

// Default init.
#define CRABS_STRAND_INIT_BUFFER_SIZE 256

// String that gets printed at the beginning of each new line.
#define NEW_LINE_HEADER "\n"

// Max Script Op name length in chars.
#define SCRIPT2_OP_MAX_NAME_LENGTH 64

// Max Script Op description length in chars.
#define SCRIPT2_OP_MAX_DECRABSION_LENGTH 1024

// Max Script address length.
#define SCRIPT2_MAX_ADDRESS_LENGTH 26

#define CRABS_MAX_KEY_LENGTH 64

/* Native string width: 8 for UTF-8, 16 for UTF-16, and 32 for UTF-32.
    This macro dictates what Unicode format strings will be unpacked to when
    unpacked from a BIn. */
#define CRABS_NATIVE_STRING_WIDTH 8

// Size or init size of the Rx/Keyboard buffer.
#define SCRIPT2_BUFFER_SIZE_RX 256

// Size or init size of the Tx/Console buffer.
#define SCRIPT2_BUFFER_SIZE_TX 1024

// Sets any of the following macros to YES or NO.

#define USING_DYNAMIC_MEMORY YES
#define USING_CRABS_1_BYTE_TYPES YES
#define USING_CRABS_2_BYTE_TYPES YES
#define USING_CRABS_4_BYTE_TYPES YES
#define USING_CRABS_8_BYTE_TYPES YES
#define USING_CRABS_OBJ YES
#define USING_CRABS_ARRAY NO
#define USING_CRABS_BOOK NO
#define USING_CRABS_LIST NO
#define USING_CRABS_DIC NO
#define CRABS_TEXT YES
#define USING_CRABS_EXCEPTIONS YES
