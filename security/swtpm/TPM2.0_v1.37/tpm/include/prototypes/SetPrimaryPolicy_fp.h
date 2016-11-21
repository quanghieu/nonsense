/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_SetPrimaryPolicy // Command must be defined
#ifndef _SetPrimaryPolicy_H
#define _SetPrimaryPolicy_H

// Input structure definition
typedef struct {
    TPMI_RH_HIERARCHY_AUTH    authHandle;
    TPM2B_DIGEST              authPolicy;
    TPMI_ALG_HASH             hashAlg;
} SetPrimaryPolicy_In;

// Response code modifiers
#define    RC_SetPrimaryPolicy_authHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_SetPrimaryPolicy_authPolicy     (TPM_RC_P + TPM_RC_1)
#define    RC_SetPrimaryPolicy_hashAlg        (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_SetPrimaryPolicy(
    SetPrimaryPolicy_In *in
);


#endif  // _SetPrimaryPolicy_H
#endif  // TPM_CC_SetPrimaryPolicy
