/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyOR // Command must be defined
#ifndef _PolicyOR_H
#define _PolicyOR_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY    policySession;
    TPML_DIGEST       pHashList;
} PolicyOR_In;

// Response code modifiers
#define    RC_PolicyOR_policySession    (TPM_RC_H + TPM_RC_1)
#define    RC_PolicyOR_pHashList        (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PolicyOR(
    PolicyOR_In *in
);


#endif  // _PolicyOR_H
#endif  // TPM_CC_PolicyOR
