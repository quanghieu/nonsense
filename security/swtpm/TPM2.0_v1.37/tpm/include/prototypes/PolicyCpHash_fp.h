/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyCpHash // Command must be defined
#ifndef _PolicyCpHash_H
#define _PolicyCpHash_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY    policySession;
    TPM2B_DIGEST      cpHashA;
} PolicyCpHash_In;

// Response code modifiers
#define    RC_PolicyCpHash_policySession    (TPM_RC_H + TPM_RC_1)
#define    RC_PolicyCpHash_cpHashA          (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PolicyCpHash(
    PolicyCpHash_In *in
);


#endif  // _PolicyCpHash_H
#endif  // TPM_CC_PolicyCpHash
