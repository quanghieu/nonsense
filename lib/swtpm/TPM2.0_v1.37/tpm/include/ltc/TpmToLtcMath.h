/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
// This file contains the structure definitions used for linking from the TPM
// code to the MPA and LTC math libraries.

#ifndef _TPM_TO_LTC_MATH_H_
#define _TPM_TO_LTC_MATH_H_

#if MATH_LIB == LTC

_REDUCE_WARNING_LEVEL_(2)
#include "LtcSettings.h"
#include "mpalib.h"
#include "mpa.h"
#include "tomcrypt_mpa.h"
_NORMAL_WARNING_LEVEL_


#if RADIX_BITS != 32
#error "The mpa library used with LibTopCrypt only works for 32-bit words"
#endif

// These macros handle entering and leaving a scope
// from which an MPA or LibTopCrypt function may be called.
// Many of these functions require a scratch pool from which
// they will allocate scratch variables (rather than using their
// own stack).
extern mpa_scratch_mem external_mem_pool;

#define MPA_ENTER(vars, bits)                                       \
    mpa_word_t           POOL_ [                                    \
                         mpa_scratch_mem_size_in_U32(vars, bits)];  \
    mpa_scratch_mem      pool_save = external_mem_pool;             \
    mpa_scratch_mem      POOL = LtcPoolInit(POOL_, vars, bits)

#define MPA_LEAVE()     init_mpa_tomcrypt(pool_save)

typedef ECC_CURVE_DATA bnCurve_t;

typedef bnCurve_t  *bigCurve;

#define AccessCurveData(E)  (E)

// Include the support functions for the routines that are used by LTC thunk.
#include "TpmToLtcSupport_fp.h"

#define CURVE_INITIALIZED(name, initializer)                        \
    bnCurve_t      *name = (ECC_CURVE_DATA *)GetCurveData(initializer)

#define CURVE_FREE(E)

// This definition would change if there were something to report
#define MathLibSimulationEnd()

#endif // MATH_LIB == LTC


#endif
