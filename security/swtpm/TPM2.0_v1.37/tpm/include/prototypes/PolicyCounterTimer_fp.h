/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyCounterTimer // Command must be defined
#ifndef _PolicyCounterTimer_H
#define _PolicyCounterTimer_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY    policySession;
    TPM2B_OPERAND     operandB;
    UINT16            offset;
    TPM_EO            operation;
} PolicyCounterTimer_In;

// Response code modifiers
#define    RC_PolicyCounterTimer_policySession    (TPM_RC_H + TPM_RC_1)
#define    RC_PolicyCounterTimer_operandB         (TPM_RC_P + TPM_RC_1)
#define    RC_PolicyCounterTimer_offset           (TPM_RC_P + TPM_RC_2)
#define    RC_PolicyCounterTimer_operation        (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_PolicyCounterTimer(
    PolicyCounterTimer_In *in
);


#endif  // _PolicyCounterTimer_H
#endif  // TPM_CC_PolicyCounterTimer
