/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyNV // Command must be defined
#ifndef _PolicyNV_H
#define _PolicyNV_H

// Input structure definition
typedef struct {
    TPMI_RH_NV_AUTH     authHandle;
    TPMI_RH_NV_INDEX    nvIndex;
    TPMI_SH_POLICY      policySession;
    TPM2B_OPERAND       operandB;
    UINT16              offset;
    TPM_EO              operation;
} PolicyNV_In;

// Response code modifiers
#define    RC_PolicyNV_authHandle       (TPM_RC_H + TPM_RC_1)
#define    RC_PolicyNV_nvIndex          (TPM_RC_H + TPM_RC_2)
#define    RC_PolicyNV_policySession    (TPM_RC_H + TPM_RC_3)
#define    RC_PolicyNV_operandB         (TPM_RC_P + TPM_RC_1)
#define    RC_PolicyNV_offset           (TPM_RC_P + TPM_RC_2)
#define    RC_PolicyNV_operation        (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_PolicyNV(
    PolicyNV_In *in
);


#endif  // _PolicyNV_H
#endif  // TPM_CC_PolicyNV
