/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Includes

#define _CRT_RAND_S
#include <stdlib.h>
#include <memory.h>
#include "PlatformData.h"
#include "Platform_fp.h"


//** Local values
// This is the last 32-bits of hardware entropy produced. We have to check to
// see that two consecutive 32-bit values are not the same because
// (according to FIPS 140-2, annex C
//
// 1. If each call to a RNG produces blocks of n bits (where n > 15), the first
// n-bit block generated after power-up, initialization, or reset shall not be
// used, but shall be saved for comparison with the next n-bit block to be
// generated. Each subsequent generation of an n-bit block shall be compared with
// the previously generated block. The test shall fail if any two compared n-bit
// blocks are equal.
extern uint32_t        lastEntropy;

extern int             firstValue;

//** _plat__GetEntropy()
// This function is used to get available hardware entropy. In a hardware
// implementation of this function, there would be no call to the system
// to get entropy.
// If the caller does not ask for any entropy, then this is a startup indication
// and 'firstValue' should be reset.

// return type: int32_t
//  < 0        hardware failure of the entropy generator, this is sticky
// >= 0        the returned amount of entropy (bytes)
//
LIB_EXPORT int32_t
_plat__GetEntropy(
    unsigned char       *entropy,           // output buffer
    uint32_t             amount             // amount requested
    )
{
    uint32_t            rndNum;
    int                 OK = 1;

    if(amount == 0)
    {
        firstValue = 1;
        return 0;
    }

    // Only provide entropy 32 bits at a time to test the ability
    // of the caller to deal with partial results.
    rndNum = rand();
    if(firstValue)
        firstValue = 0;
    else
        OK = (rndNum != lastEntropy);
    if(OK)
    {
        lastEntropy = rndNum;
        if(amount > sizeof(rndNum))
            amount = sizeof(rndNum);
        memcpy(entropy, &rndNum, amount);
    }
    return (OK) ? (int32_t)amount : -1;
}