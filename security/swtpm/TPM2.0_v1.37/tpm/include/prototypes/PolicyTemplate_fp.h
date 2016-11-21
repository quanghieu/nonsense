/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyTemplate // Command must be defined
#ifndef _PolicyTemplate_H
#define _PolicyTemplate_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY    policySession;
    TPM2B_DIGEST      templateHash;
} PolicyTemplate_In;

// Response code modifiers
#define    RC_PolicyTemplate_policySession    (TPM_RC_H + TPM_RC_1)
#define    RC_PolicyTemplate_templateHash     (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PolicyTemplate(
    PolicyTemplate_In *in
);


#endif  // _PolicyTemplate_H
#endif  // TPM_CC_PolicyTemplate
