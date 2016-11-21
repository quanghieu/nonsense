/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
//
// This header file is used to "splice" the TPM to the LTC symmetric cipher code.

#ifndef _TPM_TO_LTC_SYM_H_
#define _TPM_TO_LTC_SYM_H_

#if SYM_LIB == LTC

// Avoid pulling in the MPA math if not doing asymmetric with LTC
#if MATH_LIB != LTC
#  define LTC_NO_ASYMMETRIC
#endif

#include "LtcSettings.h"

//***************************************************************
//******** Linking to the TomCrypt AES code *********************
//***************************************************************

#ifdef TPM_ALG_SM4
#error "SM4 is not available"
#endif

#ifdef  TPM_ALG_CAMELLIA
#error "Camellia is not available"
#endif

// Define the order of parameters to the functions that do block encryption and
// decryption.
typedef void(*TpmCryptSetSymKeyCall_t)(
    const void      *in,
    void            *out,
    void            *keySchedule
    );

// Macro to put the parameters in the order required by the library
#define SWIZZLE(keySchedule, in, out)                                   \
    (const void *)(in), (void *)(out), (void *)(keySchedule)

// Macros to set up the encryption/decryption key schedules
//
// AES:
# define TpmCryptSetEncryptKeyAES(key, keySizeInBits, schedule)            \
    aes_setup((key), BITS_TO_BYTES(keySizeInBits), 0, (symmetric_key *)(schedule))
# define TpmCryptSetDecryptKeyAES(key, keySizeInBits, schedule)            \
    aes_setup((key), BITS_TO_BYTES(keySizeInBits), 0, (symmetric_key *)(schedule))

// TDES:
# define TpmCryptSetEncryptKeyTDES(key, keySizeInBits, schedule)            \
    TDES_setup((key), (keySizeInBits), (symmetric_key *)(schedule))
# define TpmCryptSetDecryptKeyTDES(key, keySizeInBits, schedule)            \
    TDES_setup((key), (keySizeInBits), (symmetric_key *)(schedule))


// Macros to alias encrypt and decrypt function calls to library-specific values
// sparingly. These should be used sparingly. Currently, they are only used by
// CryptRand.c in the AES version of the DRBG.
#define TpmCryptEncryptAES      aes_ecb_encrypt
#define TpmCryptDecryptAES      aes_ecb_decrypt
#define tpmKeyScheduleAES       struct rijndael_key
//
#define TpmCryptEncryptTDES     des3_ecb_encrypt
#define TpmCryptDecryptTDES     des3_ecb_decrypt
#define tpmKeyScheduleTDES      struct des3_key

typedef union tpmCryptKeySchedule_t tpmCryptKeySchedule_t;

#include "TpmToLtcDesSupport_fp.h"

// This is used to trigger printing of simulation statistics

#define SymLibSimulationEnd()

#endif // SIM_LIB == LTC

#endif // _TPM_TO_LTC_SYM_H_
