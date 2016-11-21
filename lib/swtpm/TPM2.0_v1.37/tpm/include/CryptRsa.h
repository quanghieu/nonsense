/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#ifndef _CRYPT_RSA_H
#define _CRYPT_RSA_H

//*** RSA-related Structures and defines

// This structure is a succinct representation of the cryptographic components
// of an RSA key. It is used in testing
typedef struct
{
    UINT32        exponent;      // The public exponent pointer
    TPM2B        *publicKey;     // Pointer to the public modulus
    TPM2B        *privateKey;    // The private prime
} RSA_KEY;

// These values are used in the bigNum representation of various RSA values.

#define RSA_BITS            (MAX_RSA_KEY_BYTES * 8)
BN_TYPE(rsa, RSA_BITS);
#define BN_RSA(name)       BN_VAR(name, RSA_BITS)
#define BN_RSA_INITIALIZED(name, initializer)                      \
    BN_INITIALIZED(name, RSA_BITS, initializer)

#define BN_PRIME(name)     BN_VAR(name, (RSA_BITS / 2))
BN_TYPE(prime, (RSA_BITS / 2));
#define BN_PRIME_INITIALIZED(name, initializer)                    \
    BN_INITIALIZED(name, RSA_BITS / 2, initializer)

typedef struct privateExponent
{
#if CRT_FORMAT_RSA == NO
    bn_rsa_t            D;
#else
    bn_prime_t          Q;
    bn_prime_t          dP;
    bn_prime_t          dQ;
    bn_prime_t          qInv;
#endif // CRT_FORMAT_RSA
} privateExponent_t;

#endif      // _CRYPT_RSA_H
