/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_GetSessionAuditDigest // Command must be defined
#ifndef _GetSessionAuditDigest_H
#define _GetSessionAuditDigest_H

// Input structure definition
typedef struct {
    TPMI_RH_ENDORSEMENT    privacyAdminHandle;
    TPMI_DH_OBJECT         signHandle;
    TPMI_SH_HMAC           sessionHandle;
    TPM2B_DATA             qualifyingData;
    TPMT_SIG_SCHEME        inScheme;
} GetSessionAuditDigest_In;

// Output structure definition
typedef struct {
    TPM2B_ATTEST           auditInfo;
    TPMT_SIGNATURE         signature;
} GetSessionAuditDigest_Out;

// Response code modifiers
#define    RC_GetSessionAuditDigest_privacyAdminHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_GetSessionAuditDigest_signHandle             (TPM_RC_H + TPM_RC_2)
#define    RC_GetSessionAuditDigest_sessionHandle          (TPM_RC_H + TPM_RC_3)
#define    RC_GetSessionAuditDigest_qualifyingData         (TPM_RC_P + TPM_RC_1)
#define    RC_GetSessionAuditDigest_inScheme               (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_GetSessionAuditDigest(
    GetSessionAuditDigest_In *in,
    GetSessionAuditDigest_Out *out
);


#endif  // _GetSessionAuditDigest_H
#endif  // TPM_CC_GetSessionAuditDigest
