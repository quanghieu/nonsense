/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
//
// The functions in this file are used for initialization of the interface to the
// OpenSSL library.

//** Defines and Includes

#include "Tpm.h"

#if MATH_LIB == OSSL

// Used to pass the pointers to the correct sub-keys
typedef const BYTE *desKeyPointers[3];

//*** SupportLibInit()
// This does any initialization required by the support library.
LIB_EXPORT int
SupportLibInit(
    void
    )
{
#ifdef LIBRARY_COMPATIBILITY_CHECK
    MathLibraryCompatibilityCheck();
#endif
    return TRUE;
}

//*** OsslContextEnter()
// This function is used to initialize an OpenSSL context at the start of a function
// that will call to an OpenSSL math function.
BN_CTX *
OsslContextEnter(
    void
    )
{
    BN_CTX              *context = BN_CTX_new();
    if(context == NULL)
        FAIL(FATAL_ERROR_ALLOCATION);
    BN_CTX_start(context);
    return context;
}

//*** OsslContextLeave()
// This is the companion function to OsslContextEnter().
void
OsslContextLeave(
    BN_CTX          *context
    )
{
    if(context != NULL)
    {
        BN_CTX_end(context);
        BN_CTX_free(context);
    }
}
#endif // MATH_LIB == OSSL

