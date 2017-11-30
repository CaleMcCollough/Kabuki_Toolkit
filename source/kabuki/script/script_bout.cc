/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_bout.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
    @desc    Slot implementation split into three files, slot.h, 
             monoid_rx.h, and monoid_tx.h because of the large read/write
             functions.
*/

#include <stdafx.h>
#include "bout.h"
#include "ascii.h"
#include "types.h"
#include "slot.h"
#include "args.h"
#include "address.h"
#include "text.h"

namespace _ {

#if SCRIPT_DEBUG
const char* BoutErrorString (Bout::Error error) {
    static const char* strings[] = {
        "Buffer overflow",  //< 0
        "Locked Error"      //< 1
        "Room Error"        //< 2
    };
    // Compiler says this will always be false but I thought enum was signed int type?
    //if ((error < 0) || (error > Bout::RoomError))
    //    return strings[Bout::RoomError];
    if (error > Bout::RoomError)
        return strings[Bout::RoomError];
    return strings[error];
}

const char* BoutStateString (Bout::State state) {
    static const char* strings[] = {
        "WritingState",
        "LockedState"
    };
    if (state >= Bout::LockedState)
        return strings[Bout::LockedState];
    return strings[state];
}
#endif  //< SCRIPT_DEBUG

byte* BoutBuffer (Bout* bout) {
    if (bout == nullptr)
        return nullptr;
    byte* ptr = reinterpret_cast<byte*> (bout);
    return ptr + sizeof (Bout);
}

Bout* BoutInit (uintptr_t* buffer, uint_t size) {
    if (size < kMinSlotSize)
        return nullptr;
    if (buffer == nullptr)
        return nullptr;

    Bout* bout  = reinterpret_cast<Bout*> (buffer);
    bout->size  = size - sizeof (Bin);
    bout->start = 0;
    bout->stop  = 0;
    bout->read  = 0;

#if SCRIPT_DEBUG
    MemoryClear (BoutBuffer (bout), size);
   // BoutPrint (bout);
#endif
    return bout;
}

uint_t BoutSpace (Bout* bout) {
    if (bout == nullptr) return ~0;
    byte* txb_ptr = reinterpret_cast<byte*>(bout);
    return SlotSpace (txb_ptr + bout->start, txb_ptr + bout->stop,
                      bout->size);
}

uint_t BoutBufferLength (Bout* bout) {
    if (bout == nullptr) return ~0;
    byte* base = BoutBuffer (bout);
    return SlotLength (base + bout->start, base + bout->stop, bout->size);
}

byte* BoutEndAddress (Bout* bout) {
    return reinterpret_cast<byte*>(bout) + (4 * sizeof (uint_t)) + bout->size;
}

int BoutStreamByte (Bout* bout) {

    byte* begin = BoutBuffer (bout),
        *end = begin + bout->size;
    byte* open = (byte*)begin + bout->read,
        *start = begin + bout->start,
        *cursor = start;

    int length = start < open?open - start + 1:
        (end - start) + (open - begin) + 2;

    if (length < 1) {
        BoutResult (bout, Bout::BufferOverflowError, Params<1, STR> (), 2,
                    start);
        return -1;
    }
    //byte b = *cursor;
    bout->stop = (++cursor > end)?static_cast<uint_t> (Diff (begin, end)):
        static_cast<uint_t> (Diff (begin, cursor));
    return 0;
}

#if SCRIPT_DEBUG
void BoutPrint (Bout* bout) {
    PrintLine ('_');
    if (bout == nullptr) {
        printf ("\n| Bout: NIL");
        PrintLine ('_');
        return;
    }
    uint_t size = bout->size;
    printf ("\n| Bout 0x%p: size: %u, start: %u, stop: %u, read: %u", bout,
            size, bout->start, bout->stop, bout->read);
    PrintMemory (BoutBuffer (bout), size + 64); // @todo remove the + 64.);
}
#endif  //< SCRIPT_DEBUG

const Operation* BoutWrite (Bout* bout, const uint_t* params, void** args) {
    
#if SCRIPT_DEBUG
    std::cout << "\n|\n|Writing ";
    ParamsPrint (params); 
    std::cout << " to B-Output:";
    printf ("%p", bout);
    BoutPrint (bout);
#endif  //< SCRIPT_DEBUG
    if (bout == nullptr)
        return BoutResult (bout, Bout::RoomError);
    if (params == nullptr)
        return BoutResult (bout, Bout::RoomError);
    if (args == nullptr)
        return BoutResult (bout, Bout::RoomError);

    // Temp variables packed into groups of 8 bytes for memory alignment.
    byte //type,
        ui1;
#if USING_2_BYTE_TYPES
    uint16_t ui2;
#endif
#if USING_4_BYTE_TYPES
    uint32_t ui4;
#endif
#if USING_8_BYTE_TYPES
    uint64_t ui8;
#endif

    uint_t num_params = params[0];
    if (num_params == 0) return 0;          //< Nothing to do.

    uint_t   type,
             size = bout->size,             //< Size of the buffer.
             space,                         //< Space in the buffer.
             index,                         //< Index in the params.
             arg_index = 0,                 //< Index in the args.
             length,                        //< Length of a type to write.
             obj_size_width;                //< Width of the array size.
    hash16_t hash = kLargest16BitPrime;     //< Reset hash to largest 16-bit prime.
    const uint_t* param = params;           //< Pointer to the current param.
    // Convert the socket offsets to pointers.
    byte* begin = BoutBuffer (bout),            //< Beginning of the buffer.
        * end   = begin + size,             //< End of the buffer.
        * start = begin + bout->start,      //< Start of the data.
        * stop  = begin + bout->stop;       //< Stop of the data.
    const byte* ui1_ptr;                    //< Pointer to a 1-byte type.
#if USING_2_BYTE_TYPES || USING_VARINT2
    const uint16_t* ui2_ptr;                //< Pointer to a 2-byte type.
#endif
#if USING_4_BYTE_TYPES || USING_VARINT4
    const uint32_t* ui4_ptr;                //< Pointer to a 4-byte type.
#endif
#if USING_8_BYTE_TYPES || USING_VARINT8
    const uint64_t* ui8_ptr;                //< Pointer to a 8-byte type.
#endif

    space = SlotSpace (start, stop, size);

    // Check if the buffer has enough room.
    if (space == 0)
        return BoutResult (bout, Bout::BufferOverflowError);
    --space;
    length = params[0];   //< Load the max char length.
    ++param;

    // Write data.
    for (index = 1; index <= num_params; ++index) {
        type = params[index];
#if SCRIPT_DEBUG
        printf ("\n| param:%2i TType:%s start:%u, stop:%u space:%u value:", 
                arg_index + 1, TypeString (type), Diff (begin, start),
                Diff (begin, stop), space);
#endif
        switch (type) {
            case NIL:
                break;

            case ADR: //< _W_r_i_t_e__A_d_d_r_e_s_s__S_t_r_i_n_g________________
            case STR: //< _W_r_i_t_e__U_T_F_-_8__S_t_r_i_n_g____________________
                if (space == 0)
                    return BoutResult (bout, Bout::BufferOverflowError, params,
                                       index, start);
                if (type != ADR) {
                    // We might not need to write anything if it's an ADR with null string.
                    length = params[++index]; //< Load the max char length.
                    ++num_params;
                } else {
                    length = kMaxAddresLength;
                }
                // Load the source data pointer and increment args.fs
                ui1_ptr = reinterpret_cast<const byte*> (args[arg_index]);
#if SCRIPT_DEBUG
                printf ("\"%s\"", ui1_ptr);
#endif  //< SCRIPT_DEBUG

                // We know we will always have at least one null-term char.
                ui1 = *ui1_ptr;
                while (ui1 != 0) {
                    if (space-- == 0)
                        return BoutResult (bout, Bout::BufferOverflowError,
                                           params, index, start);
                    hash = Hash16 (ui1, hash);

                    *stop = ui1;        // Write byte
                    if (++stop >= end) stop -= size;
                    ++ui1_ptr;
                    ui1 = *ui1_ptr;     // Read byte.
                }
                if (type != ADR) {  //< 1 is faster to compare than 2
                                    // More likely to have ADR than STR
                    *stop = 0;      // Write null-term char.
                    if (++stop >= end) stop -= size;
                    break;
                }

                break;
            case ST2: //< _W_r_i_t_e__U_T_F_-_1_6__S_t_r_i_n_g__________________
#if USING_UTF16
#else
                return BoutResult (bout, Bout::RoomError, params, index);
#endif  //< USING_UTF-16
            case ST4: //< _W_r_i_t_e__U_T_F_-_3_2__S_t_r_i_n_g__________________
#if USING_UTF32
#else
                return BoutResult (bout, Bout::RoomError, params, index);
#endif  //< USING_UTF-32
            case SI1: //< _W_r_i_t_e__8_-_b_i_t__T_y_p_e_s______________________
            case UI1:
            case BOL:
#if USING_1_BYTE_TYPES
                // Check if the buffer has enough room.
                if (space-- == 0)
                    return BoutResult (bout, Bout::BufferOverflowError, params,
                                       index, start);

                // Load pointer and read data to write.
                ui1_ptr = reinterpret_cast<const byte*> (args[arg_index]);
                ui1 = *ui1_ptr;

                // Write data.
                *stop = ui1;
                hash = Hash16 (ui1, hash);
                if (++stop >= end) stop -= size;
                break;
#else
                goto InvalidType;
#endif
            case SI2: //< _W_r_i_t_e__1_6_-_b_i_t__T_y_p_e_s____________________
            case UI2:
            case HLF:
#if USING_2_BYTE_TYPES
                // Align the buffer to a word boundary and check if the 
                // buffer has enough room.
                if (space < sizeof (uint16_t))
                    return BoutResult (bout, Bout::BufferOverflowError,
                                       params, index, start);
                space -= sizeof (uint16_t);

                // Load pointer and value to write.
                ui2_ptr = reinterpret_cast<uint16_t*> (args[arg_index]);
                ui2 = *ui2_ptr;

                // Write data.

                // Byte 1
                ui1 = (byte)ui2;
                *stop = ui1;
                if (++stop >= end) stop -= size;
                hash = Hash16 (ui1, hash);

                // Byte 2
                ui1 = (byte)(ui2 >> 8);
                *stop = ui1;
                if (++stop >= end) stop -= size;
                hash = Hash16 (ui1, hash);
                break;
#else
                goto InvalidType;
#endif

            case SI4: //< _W_r_i_t_e__3_2_-_b_i_t__T_y_p_e_s____________________
            case UI4:
            case FLT:
            case TMS:
#if USING_4_BYTE_TYPES
                // Align the buffer to a word boundary and check if the buffer
                // has enough room.

                if (space < sizeof (uint32_t))
                    return BoutResult (bout, Bout::BufferOverflowError, params,
                                       index, start);
                space -= sizeof (uint64_t);

                // Load pointer and value to write.
                ui4_ptr = reinterpret_cast<uint32_t*> (args[arg_index]);
                ui4 = *ui4_ptr;


                // Byte 1
                ui1 = (byte)ui4;
                *stop = ui1;
                hash = Hash16 (ui1, hash);
                if (++stop >= end) stop -= size;

                // Byte 2
                ui1 = (byte)(ui4 >> 8);
                *stop = ui1;
                hash = Hash16 (ui1, hash);
                if (++stop >= end) stop -= size;

                // Byte 3
                ui1 = (byte)(ui4 >> 16);
                *stop = ui1;
                hash = Hash16 (ui1, hash);
                if (++stop >= end) stop -= size;

                // Byte 4
                ui4 = (byte)(ui4 >> 24);
                *stop = ui1;
                hash = Hash16 (ui1, hash);
                if (++stop >= end) stop -= size;
                break;
#else
                goto InvalidType;
#endif

            case TMU: //< _W_r_i_t_e__6_4_-_b_i_t__T_y_p_e_s____________________
            case SI8:
            case UI8:
            case DBL:
#if USING_8_BYTE_TYPES
                // Align the buffer to a word boundary and check if the buffer
                // has enough room.
                if (space < sizeof (uint64_t))
                    return BoutResult (bout, Bout::BufferOverflowError, params,
                                       index, start);
                space -= sizeof (uint64_t);

                // Load pointer and value to write.
                ui8_ptr = reinterpret_cast<uint64_t*> (args[arg_index]);
                ui8 = *ui8_ptr;

                // Write data.

                // Byte 1
                ui1 = (byte)ui8;
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 2
                ui1 = (byte)(ui8 >> 8);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 3
                ui1 = (byte)(ui8 >> 16);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 4
                ui8 = (byte)(ui8 >> 24);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 5
                ui1 = (byte)(ui8 >> 32);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 6
                ui1 = (byte)(ui8 >> 40);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 7
                ui1 = (byte)(ui8 >> 48);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 8
                ui8 = (byte)(ui8 >> 56);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;
                break;
#else
                goto InvalidType;
#endif

            case SV2: //< _W_r_i_t_e__2_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_____

#if USING_VARINT2
                      // Load number to write and increment args.
                ui2_ptr = reinterpret_cast<const uint16_t*> (args[arg_index]);
                ui2 = *ui2_ptr;
                // We are using the same code to print both signed and unsigned 
                // varints. In order to convert from a negative 2's complement 
                // signed integer to a transmittable format, we need to invert 
                // the bits and add 1. Then we just shift the bits left one and 
                // put the sign bit in the LSB.
                ui2 = PackSignedVarint<uint16_t> (ui2);
                goto WriteVarint2;
            case UV2: //< _W_r_i_t_e__2_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_
                      // Load next pointer value to write.
                ui2_ptr = reinterpret_cast<const uint16_t*> (args[arg_index]);
                if (ui2_ptr == nullptr)
                    return BoutResult (bout, Bout::RoomError, params, index,
                                       start);
                ui2 = *ui2_ptr;

                WriteVarint2:
                {
                    // Byte 1
                    if (space-- == 0)
                        return BoutResult (bout, Bout::BufferOverflowError,
                                           params, index, start);
                    ui1 = ui2 & 0x7f;
                    ui2 = ui2 >> 7;
                    if (ui2 == 0) {
                        ui1 |= 0x80;
                        *stop = ui1;
                        if (++stop >= end) stop -= size;
                        hash = Hash16 (ui1, hash);
                        break;
                    }
                    *stop = ui1;
                    if (++stop >= end) stop -= size;
                    hash = Hash16 (ui1, hash);

                    // Byte 2
                    if (--space == 0)
                        return BoutResult (bout, Bout::BufferOverflowError,
                                           params, index, start);
                    ui1 = ui2 & 0x7f;
                    ui2 = ui2 >> 7;
                    if (ui2 == 0) {
                        ui1 |= 0x80;
                        *stop = ui1;
                        if (++stop >= end) stop -= size;
                        hash = Hash16 (ui1, hash);
                        break;
                    }
                    *stop = ui1;
                    if (++stop >= end) stop -= size;
                    hash = Hash16 (ui1, hash);

                    // Byte 3
                    if (--space == 0)
                        return BoutResult (bout, Bout::BufferOverflowError,
                                           params, index, start);
                    ui1 = ui2 & 0x7f;
                    ui1 |= 0x80;
                    *stop = ui1;
                    if (++stop >= end) stop -= size;
                    hash = Hash16 (ui1, hash);
                }
                break;
#else
            case UV2:
                goto InvalidType;
#endif
            case SV4: //< _W_r_i_t_e__4_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_____

#if USING_VARINT4
                      // Load number to write and increment args.
                ui4_ptr = reinterpret_cast<const uint32_t*> (args[arg_index]);
                ui4 = *ui4_ptr;
                ui4 = PackSignedVarint<uint32_t> (ui4);
                goto WriteVarint4;
            case UV4: //< _W_r_i_t_e__4_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_
                      // Load the 4-byte type to write to the buffer.
                ui4_ptr = reinterpret_cast<const uint32_t*> (args[arg_index]);
                ui4 = *ui4_ptr;
                WriteVarint4: { //< Optimized manual do while loop.
                    ui2 = 5;
                    if (space == 0) //< @todo Benchmark to space--
                        return BoutResult (bout, Bout::BufferOverflowError,
                                           params, index, start);
                    --space;    //< @todo Benchmark to space--
                    ui1 = ui4 & 0x7f;
                    ui4 = ui4 >> 7;
                    if (ui4 == 0) {
                        ui1 |= 0x80;
                        *stop = ui1;
                        if (++stop >= end) stop -= size;
                        hash = Hash16 (ui1, hash);
                        break;
                    }
                    *stop = ui1;
                    if (++stop >= end) stop -= size;
                    hash = Hash16 (ui1, hash);
                    // This wont happen I don't think.
                    //if (--ui2 == 0)
                    //    return BoutResult (VarintOverflowError, params, index,
                    //                       start);

                    goto WriteVarint4;
                }
                break;
#else
            case UV4:
                goto InvalidType;
#endif
            case SV8: //< _W_r_i_t_e__8_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_____
#if USING_VARINT8
                      // Load number to write and increment args.
                ui8_ptr = reinterpret_cast<const uint64_t*> (args[arg_index]);
                ui8 = *ui8_ptr;
                ui8 = PackSignedVarint<uint64_t> (ui8);
                goto WriteVarint8;
            case UV8: //< _W_r_i_t_e__8_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_
                      // Load the 4-byte type to write to the buffer.
                ui8_ptr = reinterpret_cast<const uint64_t*> (args[arg_index]);
                ui8 = *ui8_ptr;
                WriteVarint8:           //< Optimized manual do while loop.
                {
                    ui2 = 8;            //< The max number of varint bytes - 1.
                    if (space <= 9)     //< @todo Benchmark to space--
                        return BoutResult (bout, Bout::BufferOverflowError,
                                           params, index,
                                           start);
                    --space;            //< @todo Benchmark to space--
                    if (--ui2 == 0) {   //< It's the last byte not term bit.
                        ui1 = ui8 & 0xFF;
                        *stop = ui1;
                        if (++stop >= end) stop -= size;
                        hash = Hash16 (ui1, hash);
                        break;
                    }
                    ui1 = ui8 & 0x7f;   //< Take 7 bits at a time.
                    ui8 = ui8 >> 7;     //< 
                    if (ui8 == 0) {
                        ui1 |= 0x80;
                        *stop = ui1;
                        if (++stop >= end) stop -= size;
                        hash = Hash16 (ui1, hash);
                        break;
                    }
                    *stop = ui1;
                    if (++stop >= end) stop -= size;
                    hash = Hash16 (ui1, hash);

                    goto WriteVarint8;
                }
                break;
#else
            case UV8:
                goto InvalidType;
#endif
            case BSC: //< _W_r_i_t_e__E_s_c_a_p_e_S_e_q_u_e_n_c_e_______________
                      // Load pointer to data to write and get size.
                ui1_ptr = reinterpret_cast<const byte*> (args[arg_index]);
                if (ui1_ptr == nullptr)
                    return BoutResult (bout, Bout::RoomError, params, index,
                                       start);
                length = *param++;
                Bout nested_bout;
                nested_bout.size = bout->size;
                nested_bout.start = bout->start;
                nested_bout.stop = Diff (begin, stop);
                nested_bout.read = bout->read;
                BoutWrite (&nested_bout, &params[index], &args[arg_index]);
                start = begin + nested_bout.start;
                break;
            default: {
                obj_size_width = type >> 5;
                if ((type >> 5) && type > OBJ)
                    return BoutResult (bout, Bout::RoomError, params, index);
                if ((type >> 7) && ((type & 0x1f) >= OBJ)) {
                    // Cannot have multi-dimensional arrays of objects!
                    type &= 0x1f;
                    InvalidType: {
                        return BoutResult (bout, Bout::RoomError, params, index,
                                           start);
                    }
                }
                type = type & 0x1f;   //< Mask off lower 5 bits.
                switch (obj_size_width) {
                    case 0: {
                        ui1_ptr = reinterpret_cast<const byte*> 
                                  (args[arg_index]);
                        if (ui1_ptr == nullptr)
                            return BoutResult (bout, Bout::RoomError, params,
                                               index, start);
                        ui1 = *ui1_ptr;
                        length = static_cast<uint_t>(ui1);
                    }
#if USING_2_BYTE_TYPES
                    case 1:
                    {
                        ui2_ptr = reinterpret_cast<const uint16_t*>
                            (args[arg_index]);
                        if (ui2_ptr == nullptr)
                            return BoutResult (bout, Bout::RoomError, params,
                                               index, start);
                        ui2 = *ui2_ptr;
                        length = static_cast<uint_t>(ui2);
                        ui1_ptr = reinterpret_cast<const byte*> (ui2_ptr);
                    }
#endif  //< USING_2_BYTE_TYPES
#if USING_4_BYTE_TYPES
                    case 2:
                    {
                        ui4_ptr = reinterpret_cast<const uint32_t*>
                            (args[arg_index]);
                        if (ui4_ptr == nullptr)
                            return BoutResult (bout, Bout::RoomError, params,
                                               index, start);
                        ui4 = *ui4_ptr;
                        length = static_cast<uint_t>(ui4);
                        ui1_ptr = reinterpret_cast<const byte*> (ui4_ptr);
                    }
#endif  //< USING_4_BYTE_TYPES
#if USING_8_BYTE_TYPES
                    case 3:
                    {
                        ui8_ptr = reinterpret_cast<const uint64_t*>
                            (args[arg_index]);
                        if (ui8_ptr == nullptr)
                            return BoutResult (bout, Bout::RoomError, params,
                                               index, start);
                        ui8 = *ui8_ptr;
                        length = static_cast<uint_t>(ui8);
                        ui1_ptr = reinterpret_cast<const byte*> (ui8_ptr);
                    }
#endif  //< USING_8_BYTE_TYPES
                    default:
                    {  // This wont happen due to the & 0x3 bit mask
                       // but it stops the compiler from barking.
                        return BoutResult (bout, Bout::RoomError, params,
                                           index, start);
                    }

                }
                if (space < length)
                    return BoutResult (bout, Bout::BufferOverflowError,
                                       params, index, start);
                if (length == 0)
                    break;          //< Not sure if this is an error.
                if (start + length >= end) {
                    for (; size - length > 0; --length) {
                        ui1 = *(ui1_ptr++);
                        hash = Hash16 (ui1, hash);
                        *stop = ui1;
                        ++stop;
                    }
                    stop = begin - 1;
                    for (; length > 0; --length) {
                        ui1 = *(ui1_ptr++);
                        hash = Hash16 (ui1, hash);
                        *stop = ui1;
                        ++stop;
                    }
                    break;
                }
                for (; length > 0; --length) {
                    ui1 = *(ui1_ptr++);
                    hash = Hash16 (ui1, hash);
                    *stop = ui1;
                    ++stop;
                }
                break;
            }
        }
        ++arg_index;
    }
    if (space < 3)
        return BoutResult (bout, Bout::BufferOverflowError, params, index,
                           start);
    //space -= 2;   //< We don't need to save this variable.
    *stop = (byte)hash;
    if (++stop >= end) stop -= size;
    *stop = (byte)(hash >> 8);
    if (++stop >= end) stop -= size;
    bout->stop = Diff (begin, stop);
#if SCRIPT_DEBUG
    printf ("\n| Done writing to B-Output with the hash 0x%x.", hash);
#endif  //< SCRIPT_DEBUG
    return 0;
}

void BoutRingBell (Bout* bout, const char* address) {
    if (bout == nullptr) {
        return;
    }
    if (address == nullptr) {
        address = "";
    }
#if SCRIPT_DEBUG
    std::cout << "\n|\n| Ringing BEL to address:" << address;
#endif  //< SCRIPT_DEBUG

    // Temp variables packed into groups of 8 bytes for memory alignment.
    byte c;

    uint_t size = bout->size,               //< Size of the buffer.
        space;                              //< Space in the buffer.
    // Convert the Slot offsets to pointers.
    byte* begin = BoutBuffer (bout),        //< Beginning of the buffer.
        * end   = begin + size,             //< End of the buffer.
        * start = begin + bout->start,      //< Start of the data.
        * stop  = begin + bout->stop;       //< Stop of the data.
    space = SlotSpace (start, stop, size);
    if (space == 0) {
#if SCRIPT_DEBUG
        std::cout << "\n| Buffer overflow!";
#endif  //< SCRIPT_DEBUG
        return;
    }
    *stop = BEL;
    if (++stop >= end) stop -= size;

    c = *address;
    while (c) {
        if (space == 0) {
#if SCRIPT_DEBUG
            std::cout << "\n| Buffer overflow!";
#endif  //< SCRIPT_DEBUG
            return;
        }
        *stop = c;
        if (++stop >= end) stop -= size;
        ++address;
        c = *address;
    }
    bout->stop = Diff (begin, stop);
}

void BoutAckBack (Bout* bout, const char* address) {
    if (bout == nullptr) {
        return;
    }
    if (address == nullptr) {
        address = "";
    }
#if SCRIPT_DEBUG
    std::cout << "\n|\n| Ringing BEL to address:" << address;
#endif  //< SCRIPT_DEBUG

    // Temp variables packed into groups of 8 bytes for memory alignment.
    byte c;

    uint_t size = bout->size,               //< Size of the buffer.
        space;                              //< Space in the buffer.
    // Convert the Slot offsets to pointers.
    byte* begin = BoutBuffer (bout),        //< Beginning of the buffer.
        * end   = begin + size,             //< End of the buffer.
        * start = begin + bout->start,      //< Start of the data.
        * stop  = begin + bout->stop;       //< Stop of the data.
    space = SlotSpace (start, stop, size);
    if (space == 0) {
#if SCRIPT_DEBUG
        std::cout << "\n| Buffer overflow!";
#endif  //< SCRIPT_DEBUG
        return;
    }
    *stop = ACK;
    if (++stop >= end) stop -= size;

    c = *address;
    while (c) {
        if (space == 0) {
#if SCRIPT_DEBUG
            std::cout << "\n| Buffer overflow!";
#endif  //< SCRIPT_DEBUG
            return;
        }
        *stop = c;
        if (++stop >= end) stop -= size;
        ++address;
        c = *address;
    }
    bout->stop = Diff (begin, stop);
}

const Operation* BoutRead (Bout* bout, const uint_t* params, void** args) {
    return BinRead (reinterpret_cast<Bin*> (bout), params, args);
} 

const Operation* BoutConnect (Bout* bout, const char* address) {
    void* args[2];
    return BoutWrite (bout, Params <2, ADR, ADR> (),
                      Args (args, address, Address<_::BEL> ()));
}

}       //< namespace _

