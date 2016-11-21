/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyAuthorize // Command must be defined
#ifndef _PolicyAuthorize_H
#define _PolicyAuthorize_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY      policySession;
    TPM2B_DIGEST        approvedPolicy;
    TPM2B_NONCE         policyRef;
    TPM2B_NAME          keySign;
    TPMT_TK_VERIFIED    checkTicket;
} PolicyAuthorize_In;

// Response code modifiers
#define    RC_PolicyAuthorize_policySession     (TPM_RC_H + TPM_RC_1)
#define    RC_PolicyAuthorize_approvedPolicy    (TPM_RC_P + TPM_RC_1)
#define    RC_PolicyAuthorize_policyRef         (TPM_RC_P + TPM_RC_2)
#define    RC_PolicyAuthorize_keySign           (TPM_RC_P + TPM_RC_3)
#define    RC_PolicyAuthorize_checkTicket       (TPM_RC_P + TPM_RC_4)

// Function prototype
TPM_RC
TPM2_PolicyAuthorize(
    PolicyAuthorize_In *in
);


#endif  // _PolicyAuthorize_H
#endif  // TPM_CC_PolicyAuthorize
