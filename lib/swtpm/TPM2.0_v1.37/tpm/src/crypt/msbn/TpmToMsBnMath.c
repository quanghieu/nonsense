/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
//
// This file contains the math functions that are not implemented in the BnMath
// library (yet). These math functions will call the MsBn library functions

#include "Tpm.h"
#if MATH_LIB == MSBN

_REDUCE_WARNING_LEVEL_(2)
#if defined(_MSC_VER)
#include <windows.h>
#endif
_NORMAL_WARNING_LEVEL_

#include "TpmToMsBnMath_fp.h"

//** Functions

//*** BnModMult()
// Does multiply and divide returning the remainder of the divide.
LIB_EXPORT BOOL
BnModMult(
    bigNum              result,
    bigConst            op1,
    bigConst            op2,
    bigConst            modulus
    )
{
    BN_CTX_START(LARGEST_NUMBER * 4);
    BN_VAR(temp, LARGEST_NUMBER_BITS * 2);
    BOOL                 OK;
    // mpa_mul does not allocate from the pool if the result is not the same as
    // op1 or op2. since this is assured by the stack allocation of 'temp', the
    // pool pointer can be NULL
    pAssert(BnGetAllocated(result) >= BnGetSize(modulus));
    multiply(BnGetArray(op1), BnGetSize(op1),
             BnGetArray(op2), BnGetSize(op2),
             BnGetArray(temp), P_BN_CTX);
    BnSetTop(temp, BnGetSize(op1) + BnGetSize(op2));
    OK = divide(BnGetArray(temp), BnGetSize(temp),
                BnGetArray(modulus), BnGetSize(modulus),
                reciprocal_1_NULL,
                NULL, BnGetArray(result), P_BN_CTX);
    if(OK)
        BnSetTop(result, BnGetSize(modulus));

    BN_CTX_END();
    return OK;
}

//*** BnMult()
// Multiplies two numbers
LIB_EXPORT BOOL
BnMult(
    bigNum           result,
    bigConst         multiplicand,
    bigConst         multiplier
    )
{
    BN_CTX_START(LARGEST_NUMBER * 4);
    // Make sure that the mpa_mul function does not allocate anything
    // from the POOL by eliminating the reason for doing it.
    BN_VAR(tempResult, LARGEST_NUMBER_BITS * 2);
    BOOL             OK;

    if(result != multiplicand && result != multiplier)
        tempResult = result;
    pAssert(BnGetAllocated(result)
            >= BnGetSize(multiplicand) + BnGetSize(multiplier));
    OK = multiply(BnGetArray(multiplicand), BnGetSize(multiplicand),
                  BnGetArray(multiplier), BnGetSize(multiplier),
                  BnGetArray(tempResult), P_BN_CTX);
    if(OK)
    {
        BnSetTop(tempResult, BnGetSize(multiplicand) + BnGetSize(multiplier));
        BnCopy(result, tempResult);
    }
    BN_CTX_END();
    return OK;
}

//*** BnDiv()
// This function divides two BIGNUM values.
LIB_EXPORT BOOL
BnDiv(
    bigNum               quotient,
    bigNum               remainder,
    bigConst             dividend,
    bigConst             divisor
    )
{
    BN_CTX_START(LARGEST_NUMBER * 8);
    BN_VAR(tRem, LARGEST_NUMBER_BITS);
    BN_VAR(tQuo, LARGEST_NUMBER_BITS);
    bigNum                   rem;
    bigNum                   quo;
    BOOL                     OK;
//
    if(BnGetSize(dividend) < BnGetSize(divisor))
    {
        if(quotient)
            BnSetWord(quotient, 0);
        if(remainder)
            BnCopy(remainder, dividend);
        OK = TRUE;
    }
    else
    {
    // Use the temp remainder if no remainder was specified or if the remainder
    // is the same as either the dividend or divisor
        if(remainder == NULL || remainder == dividend || remainder == divisor)
            rem = tRem;
        else
            rem = remainder;
        // Same check or quotient
        if(quotient == NULL || quotient == dividend || quotient == divisor)
            quo = tQuo;
        else
            quo = quotient;
        pAssert(!BnEqualZero(divisor));
        OK = divide(BnGetArray(dividend), BnGetSize(dividend),
                    BnGetArray(divisor), BnGetSize(divisor),
                    reciprocal_1_NULL,
                    BnGetArray(quo),
                    BnGetArray(rem),
                    P_BN_CTX);
        if(OK)
        {
            BnSetTop(rem, BnGetSize(divisor));
            BnSetTop(quo, BnGetSize(dividend));
            BnCopy(remainder, rem);
            BnCopy(quotient, quo);
        }
    }
    BN_CTX_END();
    return OK;
}

#ifdef TPM_ALG_RSA
//*** BnGcd()
// Get the greatest common divisor of two numbers
LIB_EXPORT BOOL
BnGcd(
    bigNum      gcd,            // OUT: the common divisor
    bigConst    number1,        // IN:
    bigConst    number2         // IN:
    )
{
    BN_CTX_START(LARGEST_NUMBER * 8);
    BN_VAR(temp, LARGEST_NUMBER_BITS);

    BOOL            OK;
    digit_t         newSize;
//
#ifdef mp_gcd
#undef mp_gcd
#endif
    OK = mp_gcd(BnGetArray(number1), BnGetSize(number1),
                BnGetArray(number2), BnGetSize(number2),
                BnGetArray(temp), &newSize, P_BN_CTX);
    if(OK)
    {
        BnSetTop(temp, newSize);
        BnCopy(gcd, temp);
    }
    BN_CTX_END();
    return OK;
}

//***BnModExp()
// Do modular exponentiation using BIGNUM values. The conversion from a bignum_t
// to a BIGNUM is trivial as they are based on the same structure
LIB_EXPORT BOOL
BnModExp(
    bigNum               result,         // OUT: the result
    bigConst             number,         // IN: number to exponentiate
    bigConst             exponent,       // IN:
    bigConst             modulus         // IN:
    )
{
    BN_CTX_START(LARGEST_NUMBER * 100);
    mp_modulus_t         mod;
    BOOL                 OK;

    pAssert(BnGetAllocated(result) >= BnGetSize(modulus));

    //  Convert to the modulus format and then do exponentiation
    OK = create_modulus(BnGetArray(modulus), BnGetSize(modulus), FROM_RIGHT,
                        &mod, P_BN_CTX);
    OK = OK && to_modular(BnGetArray(number), BnGetSize(number), BnGetArray(result),
                          &mod, P_BN_CTX);
    OK = OK && mod_exp(BnGetArray(result),
                       BnGetArray(exponent), BnGetSize(exponent),
                       BnGetArray(result), &mod, P_BN_CTX);
    OK = OK && from_modular(BnGetArray(result), BnGetArray(result), &mod,
                            P_BN_CTX);
    if(!OK)
        FAIL(FATAL_ERROR_INTERNAL);
    BnSetTop(result, BnGetSize(modulus));
    BN_CTX_END();
    return OK;
}

//*** BnModInverse()
// Modular multiplicative inverse
LIB_EXPORT BOOL
BnModInverse(
    bigNum               result,
    bigConst             number,
    bigConst             modulus
    )
{
    BN_CTX_START(LARGEST_NUMBER * 20);
    BN_VAR(temp, LARGEST_NUMBER_BITS);
    BOOL            OK;

    pAssert(BnGetAllocated(result) >= BnGetSize(modulus));

    // mp_invert assumes that all the values are the same size. Make sure that
    // 'number' is at least as big as modulus.
    if(BnGetAllocated(number) < BnGetSize(modulus))
        BnCopy(temp, number);
    else
        temp = (bigNum)number;

    // Compute the multiplicative inverse result = 1/number mod modulus

    OK = mp_invert(BnGetArray(temp), BnGetArray(modulus), BnGetSize(modulus),
                   BnGetArray(result), __FUNCTION__, NULL, P_BN_CTX);
    if(OK)
        BnSetTop(result, BnGetSize(modulus));
    BN_CTX_END();
    return OK;
}
#endif // TPM_ALG_RSA

#if defined TPM_ALG_ECC
//*** BnPointToMsBnPoint()
// Convert a point in TPM BigNum format to a point in MsBn format. If 'P' is NULL,
// or there is an error, then the function returns a NULL pointer
// return type: msBnPoint
//  NULL        error or P is NULL
//  bnP         conversion OK
msBnPoint
BnPointToMsBnPoint(
    msBnPoint            bnP,       // OUT: address to receive the converted point
    pointConst           P,         // IN: the point to convert
    bigCurve             E,         // IN: curve structure
    pMsbnCtx             pBigCtx    // IN: the working context for memory allocation
    )
{
    if(P != NULL)
    {
        digit_t            *to = (digit_t *)bnP;
        BOOL                OK;
        OK = to_modular(BnGetArray(P->x), BnGetSize(P->x), to,
                        E->F->fdesc->modulo, P_BN_CTX);
        to += E->F->fdesc->elng;
        OK = OK && to_modular(BnGetArray(P->y), BnGetSize(P->y), to,
                              E->F->fdesc->modulo, P_BN_CTX);
        return OK ? bnP : NULL;
    }
    return NULL;
}

//*** MsBnPointToBnPoint()
// Convert a point from MsBn format to TPM BigNum format
static BOOL
MsBnPointToBnPoint(
    bigPoint             P,
    msBnPoint            bnP,
    bigCurve             E,
    pMsbnCtx             pBigCtx
    )
{
    BOOL                OK = TRUE;
    uint32_t            eLen = E->F->fdesc->elng;
    digit_t             *from = (digit_t *)bnP;
//
    // Make sure that the sizes of the x and y values are large enough to hold
    // the values
    pAssert((P != NULL) && (bnP != NULL)
            && BnGetAllocated(P->x) >= (crypt_uword_t)eLen
            && BnGetAllocated(P->y) >= (crypt_uword_t)eLen);
    if(ecaffine_is_infinite((digit_t *)bnP, E->F, P_BN_CTX))
    {
        BnSetWord(P->z, 0);
        OK = FALSE;
    }
    // Convert x
    OK = OK && from_modular(from, BnGetArray(P->x), E->F->fdesc->modulo, P_BN_CTX);
    from += eLen;
    // Convert y
    OK = OK && from_modular(from, BnGetArray(P->y), E->F->fdesc->modulo, P_BN_CTX);
    if(OK)
    {
        BnSetTop(P->x, eLen);
        BnSetTop(P->y, eLen);
        BnSetWord(P->z, 1);
    }
    return OK;
}

//*** BnEccModMult()
// This function does a point multiply of the form R = [d]S
// return type: BOOL
//  FALSE       failure in operation; treat as result being point at infinity
LIB_EXPORT BOOL
BnEccModMult(
    bigPoint             R,         // OUT: computed point
    pointConst           S,         // IN: point to multiply by 'd'
    bigConst             d,         // IN: scalar for [d]S
    bigCurve             E
    )
{
    return BnEccModMult2(R, S, d, NULL, NULL, E);
}

//*** BnEccModMult2()
// This function does a point multiply of the form R = [d]S + [u]Q
// return type: BOOL
//  FALSE       failure in operation; treat as result being point at infinity
LIB_EXPORT BOOL
BnEccModMult2(
    bigPoint             R,         // OUT: computed point
    pointConst           S,         // IN: first point (optional)
    bigConst             d,         // IN: scalar for [d]S or [d]G
    pointConst           Q,         // IN: second point
    bigConst             u,         // IN: second scalar
    bigCurve             E          // IN: curve
    )
{
    BN_CTX_START(MAX_ECC_PARAMETER_BYTES * 200);
    DWORDREG                 count = 0;
    multi_exponent_t         multi[2];
    const ecurve_t          *F = E ? E->F : NULL;
    msBnPoint_t              bnSd;
    msBnPoint_t              bnQd;
    msBnPoint_t              bnRd;
    msBnPoint                bnS = BnPointToMsBnPoint(&bnSd, S, E, pBigCtx);
    msBnPoint                bnQ = BnPointToMsBnPoint(&bnQd, Q, E, pBigCtx);
    msBnPoint                bnR = &bnRd;
    BOOL                     OK;
//
    //Some invariant initialization
    pAssert(F != NULL);
    BnSetWord(R->z, 1);
    multi[0].offset_bits = 0;
    multi[1].offset_bits = 0;
    multi[0].lng_bits = d != NULL ? BnGetSize(d) * RADIX_BITS : 0;
    multi[1].lng_bits = u != NULL ? BnGetSize(u) * RADIX_BITS : 0;
    multi[0].lng_pexp_alloc = d != NULL ? BnGetSize(d) : 0;
    multi[1].lng_pexp_alloc = u != NULL ? BnGetSize(u) : 0;

    // If dIn is provided, do [d]G where G is the generator point of the curve
    if(d != NULL)
    {
        multi[0].pexponent = BnGetArray(d);
        if(S == NULL)
            multi[0].pbase = F->generator;
        else
            multi[0].pbase = (digit_t *)bnS;
        count = 1;
    }
    // If we have a second pair, add them to the list
    if(Q != NULL && u != NULL)
    {
        multi[count].pbase = (digit_t *)bnQ;
        multi[count].pexponent = BnGetArray(u);
        count = count + 1;
    }
    OK = ecaffine_exp_multi_via_projective(multi, count, (digit_t *)bnR, F,
                                           P_BN_CTX);

    OK = OK && MsBnPointToBnPoint(R, bnR, E, pBigCtx);

    BN_CTX_END();
    return OK;
}

//** BnEccAdd()
// This function does addition of two points.
// return type: BOOL
//  FALSE       failure in operation; treat as result being point at infinity
LIB_EXPORT BOOL
BnEccAdd(
    bigPoint             R,         // OUT: computed point
    pointConst           S,         // IN: point to multiply by 'd'
    pointConst           Q,         // IN: second point
    bigCurve             E          // IN: curve
    )
{
    BN_CTX_START(MAX_ECC_PARAMETER_BYTES * 20);
    msBnPoint_t          bnRd;
    msBnPoint_t          bnSd;
    msBnPoint_t          bnQd;
    msBnPoint            bnR = &bnRd;
    msBnPoint            bnS = BnPointToMsBnPoint(&bnSd, S, E, pBigCtx);
    msBnPoint            bnQ = BnPointToMsBnPoint(&bnQd, Q, E, pBigCtx);
    crypt_uword_t        scratch[2 * BYTES_TO_CRYPT_WORDS(MAX_ECC_PARAMETER_BYTES)];
    BOOL                 OK;

    OK = ecaffine_addition((digit_t *)bnS, (digit_t *)bnQ, (digit_t *)bnR,
                           1, E->F, scratch, P_BN_CTX);

    OK = OK && MsBnPointToBnPoint(R, bnR, E, pBigCtx);

    BN_CTX_END();
    return OK;
}

//*** BnCurveInit()
// This function will initialize a the MsBn curve structure.
bigCurve
BnCurveInit(
    TPM_ECC_CURVE            curveID,       // IN: the ID for the curve
    crypt_uword_t           *buffer,        // OUT: the buffer to receive the
                                            //      bigCurve structure and the MsBn
                                            //      curve structure.
    crypt_uword_t            wordCount      // IN:  number of crypt words in buffer
    )
{
    const ECC_CURVE         *T = CryptEccGetParametersByCurveId(curveID);
    const ECC_CURVE_DATA    *C = T ? T->curveData : NULL;
    bigCurve                 E;
    ecurve_t                *F;
    BOOL                     OK = TRUE;
    yoyo_allocator           alloc;
    TPM2B_ECC_PARAMETER      prime, order, a, b, x, y;
    NUMBYTES                   sizeOfOrder;
    msbnCtx_t                bnCtx = {0};
    pMsbnCtx                 pBigCtx = &bnCtx;  // Need this for macros to work
//
    pAssert(T != NULL);
    pAssert(wordCount > (SIZE_IN_CRYPT_WORDS(*E) + SIZE_IN_CRYPT_WORDS(*F)));
    // The curve structure is there first thing in the provided buffer
    E = (bigCurve)buffer;

    buffer = &buffer[SIZE_IN_CRYPT_WORDS(*E)];
    wordCount -= SIZE_IN_CRYPT_WORDS(*E);
    // Next comest the ecurve_t structure
    F = (ecurve_t *)buffer;
    buffer = &buffer[SIZE_IN_CRYPT_WORDS(*F)];
    wordCount -= SIZE_IN_CRYPT_WORDS(*F);
    // Can initialize the bigCurve structure
    E->T = T;   // pointer to the TPM values (curveId, number of bits, etc.)
    E->C = C;   // pointer to the bn curve data (values in bignum format)
    E->F = F;   // pointer to the msbn curve data (the MsBn-defined structure)

    sizeOfOrder = (NUMBYTES)BITS_TO_BYTES(BnMsb(CurveGetOrder(C)));
    // Convert everything to a format that ecc_intialize_prime_curve will understand
    BnTo2B(C->prime, &prime.b, 0);
    BnTo2B(C->order, &order.b, sizeOfOrder);
    BnTo2B(C->a, &a.b, sizeOfOrder);
    BnTo2B(C->b, &b.b, sizeOfOrder);
    BnTo2B(C->base.x, &x.b, sizeOfOrder);
    BnTo2B(C->base.y, &y.b, sizeOfOrder);

    // The bigctx_t used in ecc_initialize_prime_curve() will be using the local
    // yoyo_allocator structure. Everything else is null or zero
    bnCtx.pAllocCtx = &alloc;
    // Start the allocator
    yoyo_allocator_initialize(&alloc, buffer, RADIX_BYTES * wordCount);
    // Tacking for high water
    NEW_METRIC(T->name);

    OK = ecc_initialize_prime_curve(BnMsb(C->prime),
                                    prime.t.buffer, prime.t.size,
                                    a.t.buffer, a.t.size,
                                    b.t.buffer, b.t.size,
                                    order.t.buffer, order.t.size,
                                    x.t.buffer, x.t.size,
                                    y.t.buffer, y.t.size,
                                    BIG_ENDIAN, 
                                    F, P_BN_CTX);
    OK = OK && divide_precondition_1(F->gorder, F->lnggorder, &F->grecip,
                                     P_BN_CTX);
    if(!OK)
        FAIL(FATAL_ERROR_INTERNAL);
    // when looking at the allocation high water numbers, remember that they need
    // to have the proper overhead added.
    SET_HIGH_WATER(T->name, alloc.max_cnt_allocated);
    return E;
}

#endif // TPM_ALG_ECC

#endif // MATH_LIB == LTC