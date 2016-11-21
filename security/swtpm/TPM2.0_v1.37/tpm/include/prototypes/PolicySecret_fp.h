/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicySecret // Command must be defined
#ifndef _PolicySecret_H
#define _PolicySecret_H

// Input structure definition
typedef struct {
    TPMI_DH_ENTITY    authHandle;
    TPMI_SH_POLICY    policySession;
    TPM2B_NONCE       nonceTPM;
    TPM2B_DIGEST      cpHashA;
    TPM2B_NONCE       policyRef;
    INT32             expiration;
} PolicySecret_In;

// Output structure definition
typedef struct {
    TPM2B_TIMEOUT     timeout;
    TPMT_TK_AUTH      policyTicket;
} PolicySecret_Out;

// Response code modifiers
#define    RC_PolicySecret_authHandle       (TPM_RC_H + TPM_RC_1)
#define    RC_PolicySecret_policySession    (TPM_RC_H + TPM_RC_2)
#define    RC_PolicySecret_nonceTPM         (TPM_RC_P + TPM_RC_1)
#define    RC_PolicySecret_cpHashA          (TPM_RC_P + TPM_RC_2)
#define    RC_PolicySecret_policyRef        (TPM_RC_P + TPM_RC_3)
#define    RC_PolicySecret_expiration       (TPM_RC_P + TPM_RC_4)

// Function prototype
TPM_RC
TPM2_PolicySecret(
    PolicySecret_In *in,
    PolicySecret_Out *out
);


#endif  // _PolicySecret_H
#endif  // TPM_CC_PolicySecret
