/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_ECDH_KeyGen // Command must be defined
#ifndef _ECDH_KeyGen_H
#define _ECDH_KeyGen_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT     keyHandle;
} ECDH_KeyGen_In;

// Output structure definition
typedef struct {
    TPM2B_ECC_POINT    zPoint;
    TPM2B_ECC_POINT    pubPoint;
} ECDH_KeyGen_Out;

// Response code modifiers
#define    RC_ECDH_KeyGen_keyHandle    (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_ECDH_KeyGen(
    ECDH_KeyGen_In *in,
    ECDH_KeyGen_Out *out
);


#endif  // _ECDH_KeyGen_H
#endif  // TPM_CC_ECDH_KeyGen
