/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
//
// The functions in this file are used for initialization of the interface to the
// LibTomCrypt and MpsLib libraries. This is not used if only the LTC hash and
// symmetric functions are used.

//** Defines and Includes

#include "Tpm.h"

#if MATH_LIB == LTC

// This state is used because there is no way to pass the random number state
// to LibTopCrypt. I do not think that this is currently an issue because...
// Heck, just put in an assert and see what happens.
static void             *s_randState;

//*** LtcRand()
// This is a stub function that is called from the LibTomCrypt or libmpa code
// to get a random number. In turn, this will call the random RandGenerate
// function that was passed in LibraryInit(). This function will pass the pointer
// to the current rand state along with the random byte request.
uint32_t     LtcRand(
    void            *buf,
    size_t           blen
    )
{
    pAssert(1);
    DRBG_Generate(s_randState, buf, (uint16_t)blen);
    return 0;
}

//*** SupportLibInit()
// This does any initialization required by the support library.
LIB_EXPORT int
SupportLibInit(
    void
    )
{
    mpa_set_random_generator(LtcRand);
    s_randState = NULL;
    external_mem_pool = NULL;
    return 1;
}

//*** LtcPoolInit()
// Function to initialize a pool. ****
LIB_EXPORT mpa_scratch_mem
LtcPoolInit(
    mpa_word_t      *poolAddress,
    int              vars,
    int              bits
    )
{
    mpa_scratch_mem     pool = (mpa_scratch_mem)poolAddress;
    mpa_init_scratch_mem(pool, vars, bits);
    init_mpa_tomcrypt(pool);
    return pool;
}

#endif // MATH_LIB == LTC
