/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#ifndef     SELF_TEST_DATA
#error  "This file many only be included in AlgorithmTests.c"
#endif

#ifndef     _SYMMETRIC_TEST_H
#define     _SYMMETRIC_TEST_H

// This file contains the structures and data definitions for the symmetric tests.
// This file references the header file that contains the actual test vectors. This
// organization was chosen so that the program that is used to generate the test
// vector values does not have to also re-generate this data.

#include    "SymmetricTestData.h"
//*** Symmetric Test Structures


const SYMMETRIC_TEST_VECTOR   c_symTestValues[NUM_SYMS] = {
#undef  COMMA
#if AES_128
    {ALG_AES_VALUE, 128, key_AES128, 16, sizeof(dataIn_AES128), dataIn_AES128,
    {dataOut_AES128_CTR, dataOut_AES128_OFB, dataOut_AES128_CBC, 
     dataOut_AES128_CFB, dataOut_AES128_ECB}}
#   define COMMA ,
#endif
#if AES_192
    COMMA
    {ALG_AES_VALUE, 192, key_AES192, 16, sizeof(dataIn_AES192), dataIn_AES192,
    {dataOut_AES192_CTR, dataOut_AES192_OFB, dataOut_AES192_CBC, 
     dataOut_AES192_CFB, dataOut_AES192_ECB}}
#   undef   COMMA
#   define COMMA ,
#endif
#if AES_256
    COMMA
    {ALG_AES_VALUE, 256, key_AES256, 16, sizeof(dataIn_AES256), dataIn_AES256,
    {dataOut_AES256_CTR, dataOut_AES256_OFB, dataOut_AES256_CBC,
    dataOut_AES256_CFB, dataOut_AES256_ECB}}
#   undef  COMMA
#   define COMMA ,
#endif
#if SM4_128
    COMMA
    {ALG_SM4_VALUE, 128, key_SM4128, 16, sizeof(dataIn_SM4128), dataIn_SM4128,
    {dataOut_SM4128_CTR, dataOut_SM4128_OFB, dataOut_SM4128_CBC, 
     dataOut_SM4128_CFB, dataOut_AES128_ECB}}
#endif
};
#undef COMMA



#endif  // _SYMMETRIC_TEST_H
