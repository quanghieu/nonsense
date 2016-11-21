/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyCommandCode // Command must be defined
#ifndef _PolicyCommandCode_H
#define _PolicyCommandCode_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY    policySession;
    TPM_CC            code;
} PolicyCommandCode_In;

// Response code modifiers
#define    RC_PolicyCommandCode_policySession    (TPM_RC_H + TPM_RC_1)
#define    RC_PolicyCommandCode_code             (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PolicyCommandCode(
    PolicyCommandCode_In *in
);


#endif  // _PolicyCommandCode_H
#endif  // TPM_CC_PolicyCommandCode
