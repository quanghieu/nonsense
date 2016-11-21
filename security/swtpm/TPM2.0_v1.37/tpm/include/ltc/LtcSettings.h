/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
//
// This header file contains some defines that are necessary to get LTC to compile
// correctly
//
#ifndef _LTC_SETTINGS_H_
#define _LTC_SETTINGS_H_

#if HASH_LIB == LTC || SYM_LIB == LTC || MATH_LIB == LTC

#ifdef TPM_ALG_AES
#  define LTC_RIJNDAEL
#endif
#ifdef TPM_ALG_TDES
#   define LTC_DES
#endif

#define _Bool   int

// LibTomCrypt types
typedef unsigned long long  ulong64;

/* default no functions m for LTC */
#define LTC_MUTEX_GLOBAL(x)
#define LTC_MUTEX_PROTO(x)
#define LTC_MUTEX_TYPE(x)
#define LTC_MUTEX_INIT(x)
#define LTC_MUTEX_LOCK(x)
#define LTC_MUTEX_UNLOCK(x)

#ifndef XMEM_NEQ
#define XMEM_NEQ
#endif

#define LTC_SHA512
#define LTC_SHA384
#define LTC_SHA256
#define LTC_SHA1

// Define these function calls as needed
#define CryptLibStartup()       LtcLibStartup()

_REDUCE_WARNING_LEVEL_(0)
#include "tomcrypt.h"
_NORMAL_WARNING_LEVEL_

#endif

#endif //
