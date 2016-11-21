/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_ECDH_ZGen // Command must be defined
#ifndef _ECDH_ZGen_H
#define _ECDH_ZGen_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT     keyHandle;
    TPM2B_ECC_POINT    inPoint;
} ECDH_ZGen_In;

// Output structure definition
typedef struct {
    TPM2B_ECC_POINT    outPoint;
} ECDH_ZGen_Out;

// Response code modifiers
#define    RC_ECDH_ZGen_keyHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_ECDH_ZGen_inPoint       (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_ECDH_ZGen(
    ECDH_ZGen_In *in,
    ECDH_ZGen_Out *out
);


#endif  // _ECDH_ZGen_H
#endif  // TPM_CC_ECDH_ZGen
