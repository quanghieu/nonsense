/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyAuthValue // Command must be defined
#ifndef _PolicyAuthValue_H
#define _PolicyAuthValue_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY    policySession;
} PolicyAuthValue_In;

// Response code modifiers
#define    RC_PolicyAuthValue_policySession    (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PolicyAuthValue(
    PolicyAuthValue_In *in
);


#endif  // _PolicyAuthValue_H
#endif  // TPM_CC_PolicyAuthValue
