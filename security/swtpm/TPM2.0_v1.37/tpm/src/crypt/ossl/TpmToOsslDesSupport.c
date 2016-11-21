/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
//
// The functions in this file are used for initialization of the interface to the
// OpenSSL library.

//** Defines and Includes

#include "Tpm.h"

#if SYM_LIB == OSSL && defined TPM_ALG_TDES

//**Functions
//*** TDES_set_encyrpt_key()
// This function makes creation of a TDES key look like the creation of a key for
// any of the other OpenSSL block ciphers. It will create three key schedules,
// one for each of the DES keys. If there are only two keys, then the third schedule
// is a copy of the first.
void
TDES_set_encrypt_key(
    const BYTE                  *key,
    UINT16                       keySizeInBits,
    tpmKeyScheduleTDES          *keySchedule
    )
{
    DES_set_key_unchecked((const_DES_cblock *)key, &keySchedule[0]);
    DES_set_key_unchecked((const_DES_cblock *)&key[8], &keySchedule[1]);
    // If is two-key, copy the schedule for K1 into K3, otherwise, compute the 
    // the schedule for K3
    if(keySizeInBits == 128)
        keySchedule[2] = keySchedule[0];
    else
        DES_set_key_unchecked((const_DES_cblock *)&key[16],
                              &keySchedule[2]);
}


//*** TDES_encyrpt()
// The TPM code uses one key schedule. For TDES, the schedule contains three
// schedules. OpenSSL wants the schedules referenced separately. This function
// does that.
void TDES_encrypt(
    const BYTE              *in, 
    BYTE                    *out,
    tpmKeyScheduleTDES      *ks
    )
{
    DES_ecb3_encrypt((const_DES_cblock *)in, (DES_cblock *)out,
                     &ks[0], &ks[1], &ks[2], 
                     DES_ENCRYPT);
}

//*** TDES_decrypt()
// As with TDES_encypt() this function bridges between the TPM single schedule
// model and the OpenSSL three schedule model.
void TDES_decrypt(
    const BYTE          *in,
    BYTE                *out,
    tpmKeyScheduleTDES   *ks
    )
{
    DES_ecb3_encrypt((const_DES_cblock *)in, (DES_cblock *)out,
                     &ks[0], &ks[1], &ks[2], 
                     DES_DECRYPT);
}

#endif // SYM_LIB == OSSL
