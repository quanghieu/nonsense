/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyAuthorizeNV // Command must be defined
#ifndef _PolicyAuthorizeNV_H
#define _PolicyAuthorizeNV_H

// Input structure definition
typedef struct {
    TPMI_RH_NV_AUTH     authHandle;
    TPMI_RH_NV_INDEX    nvIndex;
    TPMI_SH_POLICY      policySession;
} PolicyAuthorizeNV_In;

// Response code modifiers
#define    RC_PolicyAuthorizeNV_authHandle       (TPM_RC_H + TPM_RC_1)
#define    RC_PolicyAuthorizeNV_nvIndex          (TPM_RC_H + TPM_RC_2)
#define    RC_PolicyAuthorizeNV_policySession    (TPM_RC_H + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_PolicyAuthorizeNV(
    PolicyAuthorizeNV_In *in
);


#endif  // _PolicyAuthorizeNV_H
#endif  // TPM_CC_PolicyAuthorizeNV
