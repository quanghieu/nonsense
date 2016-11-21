/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
// This file contains conversion functions. The conversions are to-from 
// octets to/from integer values.
//
// These functions are all in-lined. The purpose in making them in-line rather than
// macros is to make it easier to insure proper data conversions.
//
//** Includes

#include "Tpm.h"

//** Functions

//*** UINT32_TO_BYTE_ARRAY()
// Stores a 32-bit aligned value to a, possibly, unaligned location
#ifndef INLINE_FUNCTIONS
void
UINT32_TO_BYTE_ARRAY(
    UINT32           in,
    BYTE            *out
    )
{
#if BIG_ENDIAN
    MemorySet(&b->buffer[b->size], 0, newSize - b->size);
    b->size = newSize;
}
#endif // INLINE_FUNCTIONS



//*** TestBit()
// This function is used to check the setting of a bit in an array of bits.
// return type: BOOL
//  TRUE    bit is set
//  FALSE   bit is not set
LIB_EXPORT BOOL
TestBit(
    unsigned int     bitNum,        // IN: number of the bit in 'bArray'
    BYTE            *bArray,        // IN: array containing the bits
    unsigned int     bytesInArray   // IN: size in bytes of 'bArray'
    )
{
    pAssert(bytesInArray > (bitNum >> 3));
    return((bArray[bitNum >> 3] & (1 << (bitNum & 7))) != 0);
}


//%#define TEST_BIT(bit, vector)   TestBit((bit), (BYTE *)&(vector), sizeof(vector))

//*** SetBit()
// This function will set the indicated bit in 'bArray'.
LIB_EXPORT void
SetBit(
    unsigned int     bitNum,        // IN: number of the bit in 'bArray'
    BYTE            *bArray,        // IN: array containing the bits
    unsigned int     bytesInArray   // IN: size in bytes of 'bArray'
    )
{
    pAssert(bytesInArray > (bitNum >> 3));
    bArray[bitNum >> 3] |= (1 << (bitNum & 7));
}

//%#define SET_BIT(bit, vector)    SetBit((bit), (BYTE *)&(vector), sizeof(vector))

//*** ClearBit()
// This function will clear the indicated bit in 'bArray'.
LIB_EXPORT void
ClearBit(
    unsigned int     bitNum,        // IN: number of the bit in 'bArray'.
    BYTE            *bArray,        // IN: array containing the bits
    unsigned int     bytesInArray   // IN: size in bytes of 'bArray'
    )
{
    pAssert(bytesInArray > (bitNum >> 3));
    bArray[bitNum >> 3] &= ~(1 << (bitNum & 7));
}

//%#define CLEAR_BIT(bit, vector) ClearBit((bit), (BYTE *)&(vector), sizeof(vector))

