/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PCR_Extend // Command must be defined
#ifndef _PCR_Extend_H
#define _PCR_Extend_H

// Input structure definition
typedef struct {
    TPMI_DH_PCR           pcrHandle;
    TPML_DIGEST_VALUES    digests;
} PCR_Extend_In;

// Response code modifiers
#define    RC_PCR_Extend_pcrHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_PCR_Extend_digests       (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PCR_Extend(
    PCR_Extend_In *in
);


#endif  // _PCR_Extend_H
#endif  // TPM_CC_PCR_Extend
