/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyPassword // Command must be defined
#ifndef _PolicyPassword_H
#define _PolicyPassword_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY    policySession;
} PolicyPassword_In;

// Response code modifiers
#define    RC_PolicyPassword_policySession    (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PolicyPassword(
    PolicyPassword_In *in
);


#endif  // _PolicyPassword_H
#endif  // TPM_CC_PolicyPassword
