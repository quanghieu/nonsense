/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyLocality // Command must be defined
#ifndef _PolicyLocality_H
#define _PolicyLocality_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY    policySession;
    TPMA_LOCALITY     locality;
} PolicyLocality_In;

// Response code modifiers
#define    RC_PolicyLocality_policySession    (TPM_RC_H + TPM_RC_1)
#define    RC_PolicyLocality_locality         (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PolicyLocality(
    PolicyLocality_In *in
);


#endif  // _PolicyLocality_H
#endif  // TPM_CC_PolicyLocality
