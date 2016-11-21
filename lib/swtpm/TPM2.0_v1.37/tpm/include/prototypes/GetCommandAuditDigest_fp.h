/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_GetCommandAuditDigest // Command must be defined
#ifndef _GetCommandAuditDigest_H
#define _GetCommandAuditDigest_H

// Input structure definition
typedef struct {
    TPMI_RH_ENDORSEMENT    privacyHandle;
    TPMI_DH_OBJECT         signHandle;
    TPM2B_DATA             qualifyingData;
    TPMT_SIG_SCHEME        inScheme;
} GetCommandAuditDigest_In;

// Output structure definition
typedef struct {
    TPM2B_ATTEST           auditInfo;
    TPMT_SIGNATURE         signature;
} GetCommandAuditDigest_Out;

// Response code modifiers
#define    RC_GetCommandAuditDigest_privacyHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_GetCommandAuditDigest_signHandle        (TPM_RC_H + TPM_RC_2)
#define    RC_GetCommandAuditDigest_qualifyingData    (TPM_RC_P + TPM_RC_1)
#define    RC_GetCommandAuditDigest_inScheme          (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_GetCommandAuditDigest(
    GetCommandAuditDigest_In *in,
    GetCommandAuditDigest_Out *out
);


#endif  // _GetCommandAuditDigest_H
#endif  // TPM_CC_GetCommandAuditDigest
