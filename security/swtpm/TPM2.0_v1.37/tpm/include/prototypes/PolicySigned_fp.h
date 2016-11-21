/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicySigned // Command must be defined
#ifndef _PolicySigned_H
#define _PolicySigned_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT    authObject;
    TPMI_SH_POLICY    policySession;
    TPM2B_NONCE       nonceTPM;
    TPM2B_DIGEST      cpHashA;
    TPM2B_NONCE       policyRef;
    INT32             expiration;
    TPMT_SIGNATURE    auth;
} PolicySigned_In;

// Output structure definition
typedef struct {
    TPM2B_TIMEOUT     timeout;
    TPMT_TK_AUTH      policyTicket;
} PolicySigned_Out;

// Response code modifiers
#define    RC_PolicySigned_authObject       (TPM_RC_H + TPM_RC_1)
#define    RC_PolicySigned_policySession    (TPM_RC_H + TPM_RC_2)
#define    RC_PolicySigned_nonceTPM         (TPM_RC_P + TPM_RC_1)
#define    RC_PolicySigned_cpHashA          (TPM_RC_P + TPM_RC_2)
#define    RC_PolicySigned_policyRef        (TPM_RC_P + TPM_RC_3)
#define    RC_PolicySigned_expiration       (TPM_RC_P + TPM_RC_4)
#define    RC_PolicySigned_auth             (TPM_RC_P + TPM_RC_5)

// Function prototype
TPM_RC
TPM2_PolicySigned(
    PolicySigned_In *in,
    PolicySigned_Out *out
);


#endif  // _PolicySigned_H
#endif  // TPM_CC_PolicySigned
