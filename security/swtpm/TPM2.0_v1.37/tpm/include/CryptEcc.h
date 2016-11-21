/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
// This file contains structure definitions used for ECC. The
// structures in this file are only used internally. The ECC-related structures
// that cross the TPM interface are defined in TpmTypes.h
//

#ifndef _CRYPT_ECC_H
#define _CRYPT_ECC_H

//*** ECC-related Structures

// This is used to define the macro that may or may not be in the data set for the
// curve (BnEccData.c). If there is a mismatch, the compiler will warn that there is
// to much/not enough initialization data in the curve. The macro is used because
// not all versions of the CryptEccData.c need the curve name.
#ifdef NAMED_CURVES
#define CURVE_NAME(a) , a
#define CURVE_NAME_DEF const char *name;
#else
#  define CURVE_NAME(a)
#  define CURVE_NAME_DEF
#endif

typedef struct ECC_CURVE
{
    const TPM_ECC_CURVE          curveId;
    const UINT16                 keySizeBits;
    const TPMT_KDF_SCHEME        kdf;
    const TPMT_ECC_SCHEME        sign;
    const ECC_CURVE_DATA        *curveData; // the address of the curve data
    CURVE_NAME_DEF
} ECC_CURVE;

extern const ECC_CURVE eccCurves[ECC_CURVE_COUNT];

#endif
