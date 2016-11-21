/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_VerifySignature // Command must be defined
#ifndef _VerifySignature_H
#define _VerifySignature_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT      keyHandle;
    TPM2B_DIGEST        digest;
    TPMT_SIGNATURE      signature;
} VerifySignature_In;

// Output structure definition
typedef struct {
    TPMT_TK_VERIFIED    validation;
} VerifySignature_Out;

// Response code modifiers
#define    RC_VerifySignature_keyHandle    (TPM_RC_H + TPM_RC_1)
#define    RC_VerifySignature_digest       (TPM_RC_P + TPM_RC_1)
#define    RC_VerifySignature_signature    (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_VerifySignature(
    VerifySignature_In *in,
    VerifySignature_Out *out
);


#endif  // _VerifySignature_H
#endif  // TPM_CC_VerifySignature
