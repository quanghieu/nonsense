/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_GetTime // Command must be defined
#ifndef _GetTime_H
#define _GetTime_H

// Input structure definition
typedef struct {
    TPMI_RH_ENDORSEMENT    privacyAdminHandle;
    TPMI_DH_OBJECT         signHandle;
    TPM2B_DATA             qualifyingData;
    TPMT_SIG_SCHEME        inScheme;
} GetTime_In;

// Output structure definition
typedef struct {
    TPM2B_ATTEST           timeInfo;
    TPMT_SIGNATURE         signature;
} GetTime_Out;

// Response code modifiers
#define    RC_GetTime_privacyAdminHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_GetTime_signHandle             (TPM_RC_H + TPM_RC_2)
#define    RC_GetTime_qualifyingData         (TPM_RC_P + TPM_RC_1)
#define    RC_GetTime_inScheme               (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_GetTime(
    GetTime_In *in,
    GetTime_Out *out
);


#endif  // _GetTime_H
#endif  // TPM_CC_GetTime
