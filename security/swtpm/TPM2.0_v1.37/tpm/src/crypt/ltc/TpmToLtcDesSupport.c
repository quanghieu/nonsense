/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
//
// The functions in this file are used for initialization of the interface to the
// LibTomCrypt and MpaLib libraries. This is not used if only the LTC hash and
// symmetric functions are used.

//** Defines and Includes

#include "Tpm.h"

#if SYM_LIB == LTC && defined TPM_ALG_TDES

//** TDES_setup
// This function calls the LTC function to generate a TDES key schedule. If the
// key is one DES key (8 bytes), then it is replicated two more times to create a 
// 24-byte TDES key. If the key is two key (16 bytes), then the first DES key is
// replicated to the third key position.
void TDES_setup(
    const BYTE          *key, 
    UINT32               keyBits, 
    symmetric_key       *skey
    )
{
    BYTE                 k[24];
    BYTE                *kp;

    // If this is two-key, make it three key by replicating K1
    if(keyBits == 128)
    {
        memcpy(k, key, 16);
        memcpy(&k[16], key, 8);
        kp = k;
    }
    else
        kp = (BYTE *)key;

    des3_setup(kp, 24, 0, skey);
}

#endif
