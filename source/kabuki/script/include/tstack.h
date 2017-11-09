/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/tstack.h
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
*/

#ifndef SCRIPT_TSTACK_H
#define SCRIPT_TSTACK_H

#include "module_config.h"

namespace _ {

/** Gets the max length of an stack of size T.
    The biggest thing you can fit in a buffer in Script is 1/2 max size of a uint_t
    divided by the size of the element in bytes. */
template<typename T, typename I = int_t>
inline uint_t MaxStackLength () {
    return (((uint_t)1) << (sizeof (uint_t) * 8 - 1)) / sizeof (T);
}

/** A stack of 8, 16, 32, or 64 bit plain-old-data (POD) types.

    Stack Memory Layout

    @code
        |================|
        |  Packed Stack  |  <-- Only if header_size = 0
        |================|
        | 64-bit Aligned |
        |     Buffer     |
        |================|
        | Stack Elements |
     ^  |================|
     |  |  Stack struct  |
    0xN |================|
    @endcode
*/
template<typename I = int_t>
struct TStack {
    I size_bytes,   //< Total size of the Stack in 64-bit aligned bytes.
      header_size,  //< Total size of the Dimensions Header in bytes.
      height,       //< Total height of the Stack in elements.
      count;        //< Count of the elements on the stack.
};

/** Gets the max number of elements in an stack with the specific index 
    width. */
template<typename T, typename I = int_t>
inline I StackMaxElements () {
    return (~(I)0) / sizeof (T);
}

/** Initializes an stack of n elements of the given type.
    @param buffer An stack of bytes large enough to fit the stack.
*/
template<typename T, typename I = int_t>
inline TStack<I>* StackInit (T* buffer, I size) {
    TStack<I>* stack = reinterpret_cast<TStack<I>*> (buffer);
    stack->size_bytes = size;
    stack->count = 0;
    stack->height = size;
    return stack;
}
template<typename T, typename I = int_t>
inline TStack<I>* StackInit (uintptr_t* buffer, I size) {
    return StackInit<T, I> (reinterpret_cast<T*> (buffer), size);
}

/** Inserts the item into the stack at the given index.
    @param a    			 The stack.
    @param item  The item to insert. 
    @param index The index to insert at.
    @return Returns -1 if a is null and -2 if the stack is full. */
template<typename T, typename I = int_t>
inline T StackInsert (TStack<I>* stack, T item, T index) {
    if (stack == nullptr)
        return -1;
    I size = stack->height,
        cout = stack->count;
    if (count >= size)
        return -2;
    T* items = StackBase<T, I> (stack);
    if (count == 0) {
        *items = item;
        stack->count = 1;
        return 0;
    }
    if (count == 1) {
        stack->cout = 1;
        if (index == 1) {
            items[1] = item;
            return 1;
        }
        else {
            items[1] = items[0];
            items[0] = item;
            return 1;
        }
    }
    if (index == count) {
        items[count] = item;
        stack->count = count + 1;
        return count;
    }
    // Common Case:
    // First move stack up one starting at index.
    T* insert_point = items + index,
     * end          = items + count - 1;
    T value;
    while (insert_point != end) {
        value = *end;
        *(end + 1) = value;
        --end;
    }
    stack->count = count + 1;
    return count;
}

template<typename T, typename I = int_t>
inline T* StackBase (TStack<I>* stack) {
    if (stack == nullptr)
        return nullptr;
    byte* address = reinterpret_cast<byte*> (stack) + sizeof (TStack<I>);
    return reinterpret_cast<T*> (address);
}

/** Removes the given index from the stack.
    @param  a     The stack.
    @param  index The index the item to remove.
    @return Returns true if the index is out of bounds. */
template<typename T, typename I = int_t>
inline bool StackRemove (TStack<I>* stack, T index) {
    if (stack == nullptr)
        return !((T)0);
    T count = stack->count,
      value;
    if (count == 0) // Nothing to remove!
        return false;
    int (index >= count)
        return false;
    T* items = StackBase<T, I> (stack);
    if (count == index - 1) {
        stack->count = count - 1;
        value = items[count - 1];
        return true;
    }
    // Move all of the elements after the index down one.
    T* insert_point = StackBase<T, I> (stack) + index,
     * end          = StackBase<T, I> (stack) + count - 1;
    while (insert_point != end) {
        value = *end;
        *(end - 1) = value;
        --end;
    }
    stack->count = count - 1;

    return true;
}

/** Adds the given item to the end of the stack.
    @param  a    The stack.
    @param  item The item to push onto the stack.
    @return Returns the index of the newly stacked item. */
template<typename T, typename I = int_t>
inline I StackPush (TStack<I>* stack, T item) {
    if (stack == nullptr)
        return -1;
    I size = stack->height,
      count = stack->count;
    if (count >= size)
        return -2;
    T* items = StackBase<T, I> (stack);
    items[count] = item;
    stack->count = count + 1;
    return count;
}

/** Pops the top item off of the stack.
    @note We do not delete the item at the 
    @param  a The stack.
    @return Returns the item popped off the stack. */
template<typename T, typename I = int_t>
inline T StackPop (TStack<I>* stack) {
    if (stack == nullptr)
        return !((T)0);
    I count = stack->count;
    if (count == 0) // Nothing to remove!
        return 0;
    T* stack = StackBase<T, I> (stack);
    stack->count = count - 1;
    T item = stack[count - 1];
    return item;
}

/** Gets the element at the given index.
    @param  stack    The stack.
    @param  index The index of the element to get.
    @return Returns -1 if a is null and -2 if the index is out of bounds. */
template<typename T, typename I = int_t>
inline T StackGet (TStack<I>* stack, T index) {
    if (stack == nullptr)
        return 0;
    if (index >= stack->count)
        return 0;
    return StackBase<T, I> (stack) + index;
}

/** Returns true if the given stack contains the given address. */
template<typename T, typename I = int_t>
inline bool StackContains (TStack<I>* stack, void* address) {
    if (stack == nullptr)
        return false;
    byte* ptr = reinterpret_cast<byte*> (stack),
        * adr = reinterpret_cast<byte*> (address);
    if (adr < ptr)
        return false;
    if (adr >= ptr + stack->size_bytes)
        return false;
    return true;
}

}       //< namespace _
#endif  //< SCRIPT_TSTACK_H
