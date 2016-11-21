/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyPCR // Command must be defined
#ifndef _PolicyPCR_H
#define _PolicyPCR_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY        policySession;
    TPM2B_DIGEST          pcrDigest;
    TPML_PCR_SELECTION    pcrs;
} PolicyPCR_In;

// Response code modifiers
#define    RC_PolicyPCR_policySession    (TPM_RC_H + TPM_RC_1)
#define    RC_PolicyPCR_pcrDigest        (TPM_RC_P + TPM_RC_1)
#define    RC_PolicyPCR_pcrs             (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_PolicyPCR(
    PolicyPCR_In *in
);


#endif  // _PolicyPCR_H
#endif  // TPM_CC_PolicyPCR
