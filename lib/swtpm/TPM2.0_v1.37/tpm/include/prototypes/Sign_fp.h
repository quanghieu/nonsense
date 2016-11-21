/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_Sign // Command must be defined
#ifndef _Sign_H
#define _Sign_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT       keyHandle;
    TPM2B_DIGEST         digest;
    TPMT_SIG_SCHEME      inScheme;
    TPMT_TK_HASHCHECK    validation;
} Sign_In;

// Output structure definition
typedef struct {
    TPMT_SIGNATURE       signature;
} Sign_Out;

// Response code modifiers
#define    RC_Sign_keyHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_Sign_digest        (TPM_RC_P + TPM_RC_1)
#define    RC_Sign_inScheme      (TPM_RC_P + TPM_RC_2)
#define    RC_Sign_validation    (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_Sign(
    Sign_In *in,
    Sign_Out *out
);


#endif  // _Sign_H
#endif  // TPM_CC_Sign
