/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
//
// This header file is used to 'splice' the OpenSSL library into the TPM code.
//
// The support required of a library are a hash module, a block cipher module and
// portions of a big number library.

#ifndef _TPM_TO_OSSL_SYM_H_
#define _TPM_TO_OSSL_SYM_H_

#if SYM_LIB == OSSL

#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/bn.h>
#include <openssl/ossl_typ.h>

//***************************************************************
//******** Linking to the OpenSSL AES code *********************
//***************************************************************

#ifdef TPM_ALG_SM4
#error "SM4 is not available"
#endif

#ifdef  TPM_ALG_CAMELLIA
#error "Camellia is not available"
#endif

// Define the order of parameters to the library functions that do block encryption
// and decryption.
typedef void(*TpmCryptSetSymKeyCall_t)(
    const BYTE  *in,
    BYTE        *out,
    void *keySchedule
    );

// The Crypt functions that call the block encryption function use the parameters 
// in the order:
//  1) keySchedule
//  2) in buffer
//  3) out buffer
// Since open SSL uses the order in encryptoCall_t above, need to swizzle the
// values to the order required by the library.
#define SWIZZLE(keySchedule, in, out)                                   \
    (const BYTE *)(in), (BYTE *)(out), (void *)(keySchedule)

// Macros to set up the encryption/decryption key schedules
//
// AES:
#define TpmCryptSetEncryptKeyAES(key, keySizeInBits, schedule)            \
    AES_set_encrypt_key((key), (keySizeInBits), (tpmKeyScheduleAES *)(schedule))
#define TpmCryptSetDecryptKeyAES(key, keySizeInBits, schedule)            \
    AES_set_decrypt_key((key), (keySizeInBits), (tpmKeyScheduleAES *)(schedule))

// TDES:
#define TpmCryptSetEncryptKeyTDES(key, keySizeInBits, schedule)            \
    TDES_set_encrypt_key((key), (keySizeInBits), (tpmKeyScheduleTDES *)(schedule))
#define TpmCryptSetDecryptKeyTDES(key, keySizeInBits, schedule)            \
    TDES_set_encrypt_key((key), (keySizeInBits), (tpmKeyScheduleTDES *)(schedule))

// Macros to alias encryption calls to specific algorithms. This should be used
// sparingly. Currently, only used by CryptRand.c
// 
// When using these calls, to call the AES block encryption code, the caller 
// should use:
//      TpmCryptEncryptAES(SWIZZLE(keySchedule, in, out));
#define TpmCryptEncryptAES          AES_encrypt
#define TpmCryptDecryptAES          AES_decrypt
#define tpmKeyScheduleAES           AES_KEY


#define TpmCryptEncryptTDES         TDES_encrypt
#define TpmCryptDecryptTDES         TDES_decrypt 
#define tpmKeyScheduleTDES          DES_key_schedule

typedef union tpmCryptKeySchedule_t tpmCryptKeySchedule_t;

#ifdef TPM_ALG_TDES
#include "TpmToOsslDesSupport_fp.h"
#endif

// This definition would change if there were something to report
#define SymLibSimulationEnd()

#endif // SYM_LIB == OSSL

#endif // _TPM_TO_OSSL_SYM_H_
