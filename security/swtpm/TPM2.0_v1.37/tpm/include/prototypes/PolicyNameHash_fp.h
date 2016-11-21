/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyNameHash // Command must be defined
#ifndef _PolicyNameHash_H
#define _PolicyNameHash_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY    policySession;
    TPM2B_DIGEST      nameHash;
} PolicyNameHash_In;

// Response code modifiers
#define    RC_PolicyNameHash_policySession    (TPM_RC_H + TPM_RC_1)
#define    RC_PolicyNameHash_nameHash         (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PolicyNameHash(
    PolicyNameHash_In *in
);


#endif  // _PolicyNameHash_H
#endif  // TPM_CC_PolicyNameHash
