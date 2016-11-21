/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
// This file contains constant definition shared by CryptUtil and the parts
// of the Crypto Engine.
//

#ifndef _CRYPT_TEST_H
#define _CRYPT_TEST_H


// This is the definition of a bit array with one bit per algorithm
typedef BYTE    ALGORITHM_VECTOR[(ALG_LAST_VALUE + 7) / 8];

#ifdef  TEST_SELF_TEST
LIB_EXPORT    extern  ALGORITHM_VECTOR    LibToTest;
#endif

//** Self-test
// This structure is used to contain self-test tracking information for the crypto
// engine. Each of the major modules is given a 32-bit value in which it may
// maintain its own self test information. The convention for this state is that
// when all of the bits in this structure are 0, all functions need to be
// tested.
typedef struct
{
    UINT32      rng;
    UINT32      hash;
    UINT32      sym;
#ifdef TPM_ALG_RSA
    UINT32      rsa;
#endif
#ifdef  TPM_ALG_ECC
    UINT32      ecc;
#endif
} CRYPTO_SELF_TEST_STATE;

//*** g_cryptoSelfTestState
// This structure contains the crypto self-test state values.
extern CRYPTO_SELF_TEST_STATE   g_cryptoSelfTestState;

#endif // _CRYPT_TEST_H
