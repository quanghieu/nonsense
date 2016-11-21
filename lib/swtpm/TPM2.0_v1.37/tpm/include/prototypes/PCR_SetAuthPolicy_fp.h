/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PCR_SetAuthPolicy // Command must be defined
#ifndef _PCR_SetAuthPolicy_H
#define _PCR_SetAuthPolicy_H

// Input structure definition
typedef struct {
    TPMI_RH_PLATFORM    authHandle;
    TPM2B_DIGEST        authPolicy;
    TPMI_ALG_HASH       hashAlg;
    TPMI_DH_PCR         pcrNum;
} PCR_SetAuthPolicy_In;

// Response code modifiers
#define    RC_PCR_SetAuthPolicy_authHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_PCR_SetAuthPolicy_authPolicy     (TPM_RC_P + TPM_RC_1)
#define    RC_PCR_SetAuthPolicy_hashAlg        (TPM_RC_P + TPM_RC_2)
#define    RC_PCR_SetAuthPolicy_pcrNum         (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_PCR_SetAuthPolicy(
    PCR_SetAuthPolicy_In *in
);


#endif  // _PCR_SetAuthPolicy_H
#endif  // TPM_CC_PCR_SetAuthPolicy
