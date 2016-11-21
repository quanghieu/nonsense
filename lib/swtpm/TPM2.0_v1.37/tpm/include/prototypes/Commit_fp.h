/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_Commit // Command must be defined
#ifndef _Commit_H
#define _Commit_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT          signHandle;
    TPM2B_ECC_POINT         P1;
    TPM2B_SENSITIVE_DATA    s2;
    TPM2B_ECC_PARAMETER     y2;
} Commit_In;

// Output structure definition
typedef struct {
    TPM2B_ECC_POINT         K;
    TPM2B_ECC_POINT         L;
    TPM2B_ECC_POINT         E;
    UINT16                  counter;
} Commit_Out;

// Response code modifiers
#define    RC_Commit_signHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_Commit_P1             (TPM_RC_P + TPM_RC_1)
#define    RC_Commit_s2             (TPM_RC_P + TPM_RC_2)
#define    RC_Commit_y2             (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_Commit(
    Commit_In *in,
    Commit_Out *out
);


#endif  // _Commit_H
#endif  // TPM_CC_Commit
