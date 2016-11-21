/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_HashSequenceStart // Command must be defined
#ifndef _HashSequenceStart_H
#define _HashSequenceStart_H

// Input structure definition
typedef struct {
    TPM2B_AUTH        auth;
    TPMI_ALG_HASH     hashAlg;
} HashSequenceStart_In;

// Output structure definition
typedef struct {
    TPMI_DH_OBJECT    sequenceHandle;
} HashSequenceStart_Out;

// Response code modifiers
#define    RC_HashSequenceStart_auth       (TPM_RC_P + TPM_RC_1)
#define    RC_HashSequenceStart_hashAlg    (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_HashSequenceStart(
    HashSequenceStart_In *in,
    HashSequenceStart_Out *out
);


#endif  // _HashSequenceStart_H
#endif  // TPM_CC_HashSequenceStart
