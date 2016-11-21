/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyGetDigest // Command must be defined
#ifndef _PolicyGetDigest_H
#define _PolicyGetDigest_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY    policySession;
} PolicyGetDigest_In;

// Output structure definition
typedef struct {
    TPM2B_DIGEST      policyDigest;
} PolicyGetDigest_Out;

// Response code modifiers
#define    RC_PolicyGetDigest_policySession    (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PolicyGetDigest(
    PolicyGetDigest_In *in,
    PolicyGetDigest_Out *out
);


#endif  // _PolicyGetDigest_H
#endif  // TPM_CC_PolicyGetDigest
