/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
// This file contains the structure definitions used for ECC in the LibTopCrypt
// version of the code. These definitions would change, based on the library.
// The ECC-related structures that cross the TPM interface are defined
// in TpmTypes.h
//

#ifndef _TPM_TO_OSSL_MATH_H
#define _TPM_TO_OSSL_MATH_H

#if MATH_LIB == OSSL

#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/bn.h>

// Make sure that the library is using the correct size for a crypt word
#if    defined THIRTY_TWO_BIT && (RADIX_BITS != 32)  \
    || defined SIXTY_FOUR_BIT && (RADIX_BITS != 64)
#  error "Ossl library is using different radix"
#endif

// Allocate a local BIGNUM value. For the allocation, a bigNum structure is created
// as is a local BIGNUM. The bigNum is initialized and then the BIGNUM is
// set to reference the local value.
#define BIG_VAR(name, bits)                                         \
    BN_VAR(name##Bn, (bits));                                       \
    BIGNUM          _##name;                                        \
    BIGNUM          *name = BigInitialized(&_##name,                \
                                BnInit(name##Bn,                    \
                                BYTES_TO_CRYPT_WORDS(sizeof(_##name##Bn.d))))

// Allocate a BIGNUM and initialize with the values in a bigNum initializer
#define BIG_INITIALIZED(name, initializer)                      \
    BIGNUM           _##name;                                   \
    BIGNUM          *name = BigInitialized(&_##name, initializer)


typedef struct
{
    const ECC_CURVE_DATA    *C;     // the TPM curve values
    EC_GROUP                *G;     // group parameters
    BN_CTX                  *CTX;   // the context for the math (this might not be
                                    // the context in which the curve was created>;
} OSSL_CURVE_DATA;

typedef OSSL_CURVE_DATA      *bigCurve;

#define AccessCurveData(E)  ((E)->C)

#define CURVE_INITIALIZED(E, initializer)                        \
    OSSL_CURVE_DATA     _##name;                                 \
    bigCurve            E =  BnCurveInitialize(&_##name, initializer)

#include "TpmToOsslSupport_fp.h"

#define CURVE_FREE(E)                   \
    if(E != NULL)                       \
    {                                   \
        if(E->G != NULL)                \
            EC_GROUP_free(E->G);        \
        OsslContextLeave(E->CTX);       \
    }

#define OSSL_ENTER()     BN_CTX      *CTX = OsslContextEnter()

#define OSSL_LEAVE()     OsslContextLeave(CTX)

// This definition would change if there were something to report
#define MathLibSimulationEnd()

#endif // MATH_LIB == OSSL

#endif
