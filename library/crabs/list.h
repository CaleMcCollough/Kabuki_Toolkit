/** Kabuki Toolkit
    @version 0.x
    @file    ~/libraries/crabs/list.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#pragma once
#include <stdafx.h>

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

#ifndef HEADER_FOR_CRABS_LIST
#define HEADER_FOR_CRABS_LIST

#if MAJOR_SEAM == 1 && MINOR_SEAM == 3
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PUTCHAR(c) putchar(c);
#define PRINT_HEADING\
    std::cout << '\n';\
    for (int i = 80; i > 0; --i) std::cout << '-';
#define PRINT_TYPE(type, value)\
    Console<> ().Out () << TypeValue (type, value);
#else
#define PRINTF(x, ...)
#define PUTCHAR(c)
#define PRINT_HEADING
#define PRINT_TYPE(type, value)
#endif

#include "set.h"
#include "align.h"
#include "stack.h"

namespace _ {

/** An ASCII List header.
    Like all ASCII Object Types, the size may only be 16-bit, 32-bit, or 
    64-bit. The unsigned value must be twice the width of the signed value.

    @code
    +==========================+ -----------
    |_______ Buffer            |   ^     ^
    |_______ ...               |   |     |
    |_______ Data N            |  Data   |
    |_______ ...               |   |     |
    |_______ Data 0            |   v     |
    |==========================| -----   |
    |_______ count_max         |   ^     |
    |_______ ...               |   |     |
    |_______ Data Offset N     |   |     |
    |_______ ...               |   |    Size
    |        Data Offset 1     |   |     |
    |==========================| Header  |
    |_______ count_max         |   |     |
    |_______ ...               |   |     |
    |_______ Type byte N       |   |     |
    |_______ ...               |   |     |
    |        Type byte 1       |   |     |   ^ 0x(N+c)+sizeof(AsciiList<UI, SI>)
    |==========================|   |     |   | 
    | AsciiList<UI, SI> Struct |   v     v   ^
    +==========================+ ----------- ^ 0xN
    @endcode
*/
template<typename UI = uint32_t, typename SI = int16_t>
struct AsciiList {
    UI size;
    SI count_max,
       count;
};

/** Returns the minimum count to align the data struct to a 64-bit boundary. */
template<typename UI = uint32_t, typename SI = int16_t>
SI ListCountMaxMin () {
    return 8 / sizeof (SI);
}

/** Returns the min size of an ASCII List with the given count_max.
    The min size is defined as enough memory to store the given count_max with 
    the largest_expected_type.
*/
template<typename UI = uint32_t, typename SI = int16_t, 
         size_t largest_expected_type = sizeof (intptr_t)>
constexpr UI ListSizeMin (SI count_max) {
    return (UI)sizeof (AsciiList<UI, SI>) + 
           (UI)(count_max * (largest_expected_type + sizeof (UI) + 1));
    // << 2 to * 4.
}

/** Initializes a AsciiList from preallocated memory.
    count_max must be in multiples of 4. Given there is a fixed size, both the 
    count_max and size will be downsized to a multiple of 4 automatically. */
template<typename UI = uint32_t, typename SI = int16_t>
AsciiList<UI, SI>* ListInit (uintptr_t* buffer, UI size, SI count_max) 
{
    if (!buffer) // This may be nullptr if ListNew<UI,SI> (SI, UI) failed.
        return nullptr;
    PRINTF ("\n  Initializing List with size_bytes:%u and count_max:%i", 
            (uint)size, (int)count_max)
    SI count_max_min = ListCountMaxMin<UI, SI> ();
    if (count_max < count_max_min) {
        PRINTF ("\n count_max == 0 and is now %i", (int)count_max_min)
        count_max = count_max_min;
    } else {
        PRINTF ("\ncount_max was %i ", (int)count_max)
        count_max = AlignUp8<SI> (count_max);
        PRINTF ("\n and now is %i.", (int)count_max)
    }

    //count_max = AlignUp8<SI> (count_max);
    //PRINTF ("\n  Aligning up to count_max:%i", (int)count_max)

    AsciiList<UI, SI>* list = reinterpret_cast<AsciiList<UI, SI>*> (buffer);
    list->size = size;
    list->count      = 0;
    list->count_max  = count_max;
    return list;
}

/** Creates a list from dynamic memory. */
template<typename UI = uint32_t, typename SI = int16_t>
uintptr_t* ListNew (SI count_max, UI size) {
    count_max = AlignUp8<SI> (count_max);
    if (size < ListSizeMin<UI, SI> (count_max))
        return nullptr;
    uintptr_t* buffer = new uintptr_t[size >> kWordBitCount];

    AsciiList<UI, SI>* list = reinterpret_cast<AsciiList<UI, SI>*> (buffer);
    list->size = size;
    list->count = 0;
    list->count_max = count_max;
    return buffer;
}

/** Creates a list from dynamic memory. */
template<typename UI = uint32_t, typename SI = int16_t,
         size_t largest_expected_type = sizeof (intptr_t)>
inline uintptr_t* ListNew (SI count_max) {
    count_max = AlignUp8<SI> (count_max);
    UI size = ListSizeMin<UI, SI, largest_expected_type> (count_max);
    uintptr_t* buffer = new uintptr_t[size >> kWordBitCount];

    AsciiList<UI, SI>* list = reinterpret_cast<AsciiList<UI, SI>*> (buffer);
    list->size = size;
    list->count = 0;
    list->count_max = count_max;
    return buffer;
}

/** Returns the type bytes array. */
template<typename UI = uint32_t, typename SI = int16_t>
type_t* ListTypes (AsciiList<UI, SI>* list) {
    ASSERT (list)
    return reinterpret_cast<type_t*> (list) + sizeof (AsciiList<UI, SI>);
}

/** Gets a pointer to the begging of the data buffer. */
template<typename UI = uint32_t, typename SI = int16_t>
inline char* ListDataBegin (AsciiList<UI, SI>* list) {
    ASSERT (list)
    return reinterpret_cast<char*> (list) + list->count_max * sizeof (SI);
}

/** Gets the base element 0 of the list's offset array. */
template<typename UI = uint32_t, typename SI = int16_t>
inline UI* ListOffsets (AsciiList<UI, SI>* list) {
    char* base = reinterpret_cast<char*> (list);
    return reinterpret_cast<UI*> (base + list->count_max);
}

/** Returns the last byte in the data array. */
template<typename UI = uint32_t, typename SI = int16_t>
inline char* ListDataStop (AsciiList<UI, SI>* list, SI index) {
    ASSERT (list)
    SI count = list->count;
    if (count == 0)
        return nullptr;
    type_t* types = ListTypes<UI, SI> (list);
    type_t  type  = types[index];
    UI* offsets = ListOffsets<UI, SI> (list);
    UI offset = offsets[index];
    char* pointer = reinterpret_cast<char*> (list) + offset;
    return ObjectEnd<UI> (type, pointer);
}

/** Returns the last byte in the data array. */
template<typename UI = uint32_t, typename SI = int16_t>
inline char* ListDataStop (AsciiList<UI, SI>* list) {
    ASSERT (list);
    return ListDataStop<UI, SI> (list, list->count - 1);
}

/** Returns the last byte in the data array. */
template<typename UI = uint32_t, typename SI = int16_t>
inline char* ListDataEnd (AsciiList<UI, SI>* list) {
    ASSERT (list)
    return reinterpret_cast<char*> (list) + list->size - 1;
}

/** Returns the last byte in the data array. */
template<typename UI = uint32_t, typename SI = int16_t>
inline char* ListDataEnd (AsciiList<UI, SI>* list, SI index) {
    ASSERT (list)
    if (index < 0 || index >= index->count)
        return nullptr;
    return reinterpret_cast<char*> (list) + list->size - 1;
}

/** Returns a pointer to the begging of the data buffer. */
template<typename UI = uint32_t, typename SI = int16_t>
Socket ListDataVector (AsciiList<UI, SI>* list) {
    return Socket (ListDataBegin<UI, SI> (list), ListDataEnd<UI, SI> (list));
}

/** Insets the given type-value tuple. */
template<typename UI = uint32_t, typename SI = int16_t>
SI ListInsert (AsciiList<UI, SI>* list, type_t type, const void* value, SI index) {
    ASSERT (list)
    PRINTF ("\nInserting type:")
    PRINT_TYPE (type, value)
    PRINTF (" into index:%i", index)

    if (value == nullptr) {
        PRINTF (" 111111111111111111111111")
        return -1;
    }
    if (index < 0) {
        PRINTF (" 222222222222222222222222")
        return index;
    }
    SI count = list->count;
    if (count >= list->count_max || index > count || !TypeIsValid (type)) {
        PRINTF (" 333333333333333333333333333333");
        return -1;
    }
    PRINTF (" when count is %i", (int) count)

    type_t* types = ListTypes<UI, SI> (list);

    // 1. Check if the count is zero and do add at the begin.
    if (count == 0) {
        PRINTF ("\nInserting first element.")
        types[0] = type;
        Write (ListDataBegin<UI, SI> (list), ListDataEnd<UI, SI> (list), type, 
               value);
        list->count = 1;
        return 0;
    }

    types += index;

    // 2. Shift up the types.
    type_t* types_cursor = types + count - 1;
    while (types_cursor > types)
        *types_cursor-- = *types_cursor;

    // Add the type byte
    *types = type;

    // Shift up the offsets.
    UI* offsets_begin  = ListOffsets<UI, SI> (list),
      * offsets_cursor = offsets_begin + count;

    while (offsets_cursor > offsets_begin)
        *offsets_cursor-- = *offsets_cursor;

    char* aligned_begin  = AlignUpPointer8<> (ListDataStop<UI, SI> (list) + 1);

    if (!Write (aligned_begin, ListDataEnd<UI, SI> (list), type, value))
        return -1;
    list->count = ++count;
    return count;
}

/** Adds a type-value to the end of the list. */
template<typename UI = uint32_t, typename SI = int16_t>
inline SI ListPush (AsciiList<UI, SI>* list, type_t type, const void* value) {
    return ListInsert<UI, SI> (list, type, value, list->count);
}

/** Removes a type-value to the end of the list. */
template<typename UI = uint32_t, typename SI = int16_t>
inline SI ListPop (AsciiList<UI, SI>* list) {
    return ListRemove<UI, SI> (list, list->count - 1);
}

/** Returns the max count an array can handle. */
template<typename UI = uint32_t, typename SI = int16_t>
SI ListCountMax () {
    enum {
        kMaxIndexes = sizeof (SI) == 1 ? 120 :
                      sizeof (SI) == 2 ? 8 * 1024 : 
                      sizeof (SI) == 4 ? 512 * 1024 * 1024 : 0,
    };
    return kMaxIndexes;
}

/** Deletes the list contents without wiping the contents. */
template<typename UI = uint32_t, typename SI = int16_t>
void ListClear (AsciiList<UI, SI>* list) {
    ASSERT (list)
    list->count = 0;
}

/** Deletes the list contents by overwriting it with zeros. */
template<typename UI = uint32_t, typename SI = int16_t>
void ListWipe (AsciiList<UI, SI>* list) {
    ASSERT (list)
    list->count = 0;
    UI size = list->size - sizeof (AsciiList<UI, SI>);
    memset (reinterpret_cast<char*> (list) + sizeof (AsciiList<UI, SI>), 0, size);
}

/** Returns true if this expr contains only the given address.
    @warning This function assumes that the member you're checking for came 
             from Kabuki Toolkit. If it's you're own code calling this, you are
             required to ensure the value came from a ASCII List.
    @return  True if the data lies in the list's memory socket. */
template<typename UI = uint32_t, typename SI = int16_t>
bool ListContains (AsciiList<UI, SI>* list, void* address) {
    ASSERT (list)
    if (reinterpret_cast<char*> (address) < reinterpret_cast<char*> (list))
        return false;
    if (reinterpret_cast<char*> (address) > ListEndByte())
        return false;
    return true;
}

/** Removes the item at the given address from the list. */
template<typename UI = uint32_t, typename SI = int16_t>
SI ListRemove (AsciiList<UI, SI>* list, SI index) {
    SI count = list->count;
    ArrayRemove<UI, SI> (ListOffsets<UI, SI> (list), count, index);
    return ArrayRemove<type_t, SI> (ListTypes<UI, SI> (list), count, index);
}

/** Finds a tuple that contains the given pointer. */
template<typename UI = uint32_t, typename SI = int16_t>
SI ListFind (AsciiList<UI, SI>* list, void* adress) {
    ASSERT (list)
    UI* offsets = ListOffsets<UI, SI> (list),
      * offset_end = offsets +  list->count;
    while (offsets < offset_end) {
        char* begin = reinterpret_cast<char*> (list) + *offsets++,
            * end   = ListDataStop<UI, SI> (list, index);
        if (reinterpret_cast<char*> (address) >= begin &&
            reinterpret_cast<char*> (address) <= end)
            return true;
    }
    return -1;
}

/** Removes the item at the given address from the list. */
template<typename UI = uint32_t, typename SI = int16_t>
bool ListRemove (AsciiList<UI, SI>* list, void* adress) {
    return ListRemove<UI, SI> (list, ListFind (list, address));
}

/** Returns the value at the given index.
    @return Returns nil if the index is out of the count range. */
template<typename UI = uint32_t, typename SI = int16_t>
const void* ListValue (AsciiList<UI, SI>* list, SI index) {
    ASSERT (list)
    if (index < 0 || index >= list->count)
        return nullptr;
    return reinterpret_cast<const char*> (list) + 
           ListOffsets<UI, SI> (list)[index];
}

/** Prints the given AsciiList to the console. */
template<typename UI = uint32_t, typename SI = int16_t>
Printer& PrintList (Printer& printer, AsciiList<UI, SI>* list) {
    ASSERT (list)
    
    SI count = list->count;
    printer << "\n\nList: size_bytes:" << list->size << " count:" << count 
            << " count_max:" << list->count_max;
    for (SI index = 0; index < count; ++index) {
        printer << '\n' << index << ".) "
                << TypeValue (ListTypes<UI, SI> (list)[index],
                              ListValue<UI, SI> (list, index));
    }
    return printer;
}

/** ASCII List that uses dynamic memory. */
template<typename UI = uint32_t, typename SI = int16_t>
class List {
    public:

    /** Constructs a list with a given count_max with estimated size_bytes. */
    List (SI count_max = 0) {
        buffer_ = ListNew<UI, SI> (count_max);
    }

    /** Constructs a List with the given size_bytes and count_max. 
        size_bytes and count_max both get rounded down to a multiple of 64 
        before allocating the buffer. If the count_max is not enough for the 
        buffer then the size_bytes will be increased to the minimum size to
        make a valid ASCII List. */
    List (SI count_max, UI size) {
        buffer_ = ListNew<UI, SI> (count_max, size);
    }

    /** Deletes the dynamically allocated buffer. */
    ~List () {
        delete [] buffer_;
    }

    inline SI Push (type_t type, const void* value) {
        return ListPush<UI, SI> (This (), type, value);
    }

    /** Inserts the given type-value tuple in the list at the given index. */
    inline SI Insert (byte type, void* value, SI index) {
        return ListInsert<UI, SI> (This (), type, value, index);
    }

    /** Returns the maximum count of the give list in the current memory 
        profile. */
    inline SI CountMax () {
        return ListCountMax<UI, SI> ();
    }

    /** Clears the list without overwriting the contents. */
    void Clear (AsciiList<UI, SI>* list) {
        ListClear<UI, SI> (This ());
    }

    /** Deletes the list contents by overwriting it with zeros. */
    inline void Wipe () {
        ListWipe<UI, SI> (This ());
    }

    /** Returns true if this expr contains only the given address.
        @warning This function assumes that the member you're checking for came
                 from Kabuki Toolkit. If it's you're own code calling this, you 
                 are required to ensure the value came from a ASCII List.
        @return  True if the data lies in the list's memory socket. */
    inline bool Contains (void* value) {
        return ListContains<UI, SI> (This (), value);
    }

    /** Removes the item at the given address from the list. */
    inline bool Remove (void* adress) {
        return ListRemove<UI, SI> (This (), adress);
    }

    /** Removes the item at the given address from the list. */
    inline bool Remove (SI index) {
        return ListRemove<UI, SI> (This (), index);
    }

    /** Removes the last item from the list. */
    inline SI Pop () {
        return ListPop<UI, SI> (This ());
    }

    /** Prints the given AsciiList to the console. */
    inline Printer& Out (Printer& printer) {
        return PrintList<UI, SI> (printer, This ());
    }

    /** Returns the contiugous ASCII List buffer_. */
    inline AsciiList<UI, SI>* This () {
        return reinterpret_cast<AsciiList<UI, SI>*> (buffer_);
    }

    private:

    uintptr_t* buffer_; //< Dynamically allocated word-aligned buffer.
};

}       //< namespace _

/** Overloaded operator<< prints the list. */
template<typename UI = uint32_t, typename SI = int16_t>
inline _::Printer& operator<< (_::Printer& printer, _::List<UI, SI>& list) {
    return list.Out (printer);
}

/** Overloaded operator<< prints the list. */
template<typename UI = uint32_t, typename SI = int16_t>
inline _::Printer& operator<< (_::Printer& printer, _::AsciiList<UI, SI>* list) {
    return PrintList<UI, SI> (printer, list);
}

#undef PRINTF
#undef PUTCHAR
#undef PRINT_HEADING
#endif  //< HEADER_FOR_CRABS_LIST
#endif  //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 3
