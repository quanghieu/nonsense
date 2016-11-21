/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PCR_SetAuthValue // Command must be defined
#ifndef _PCR_SetAuthValue_H
#define _PCR_SetAuthValue_H

// Input structure definition
typedef struct {
    TPMI_DH_PCR     pcrHandle;
    TPM2B_DIGEST    auth;
} PCR_SetAuthValue_In;

// Response code modifiers
#define    RC_PCR_SetAuthValue_pcrHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_PCR_SetAuthValue_auth          (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PCR_SetAuthValue(
    PCR_SetAuthValue_In *in
);


#endif  // _PCR_SetAuthValue_H
#endif  // TPM_CC_PCR_SetAuthValue
