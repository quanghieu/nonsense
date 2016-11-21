/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
//
// This header file is used to select the library code that gets included in the
// TPM built

#ifndef _LIB_SUPPORT_H_
#define _LIB_SUPPORT_H_

// OSSL has a full suite but yields an executable that is much larger than it needs
// to be.
#define     OSSL        1
// LTC has symmetric support, RSA support, and inadequate ECC support
#define     LTC         2
// MSBN only provides math support so should not be used as the hash or symmetric
// library
#define     MSBN        3
// SYMCRYPT only provides symmetric cryptography so would need to be combined with
// another library that has math support
#define     SYMCRYPT    4

//*********************
#if RADIX_BITS == 32
#   define RADIX_BYTES 4
#elif RADIX_BITS == 64
#   define RADIX_BYTES 8
#else
#error  "RADIX_BITS must either be 32 or 64."
#endif

// Include the options for hashing
// If all the optional headers were always part of the distribution then it would
// not be necessary to do the conditional testing before the include. )-;
#if HASH_LIB == OSSL
#  include "ossl/TpmToOsslHash.h"
#elif HASH_LIB == LTC
#  include "ltc/TpmToLtcHash.h"
#elif HASH_LIB == SYMCRYPT
#include "symcrypt/TpmToSymcryptHash.h"
#else
#  error "No hash library selected"
#endif


// Set the linkage for the selected symmetric library
#if SYM_LIB == OSSL
#  include "ossl/TpmToOsslSym.h"
#elif SYM_LIB == LTC
#  include "ltc/TpmToLtcSym.h"
#elif SYM_LIB == SYMCRYPT
#include "symcrypt/TpmToSymcryptSym.h"
#else
#  error "No symmetric library selected"
#endif

#undef MIN
#undef MIN


// Select a big number Library.
// This uses a define rather than an include so that the header will not be included
// until the required values have been defined.
#if MATH_LIB == OSSL
#  define MATHLIB_H  "ossl/TpmToOsslMath.h"
#elif MATH_LIB == LTC
#  define MATHLIB_H  "ltc/TpmToLtcMath.h"
#elif MATH_LIB == MSBN
#define MATHLIB_H  "msbn/TpmToMsBnMath.h"
#else
#  error "No math library selected"
#endif

#endif // _LIB_SUPPORT_H_
