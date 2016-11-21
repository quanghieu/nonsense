/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_Hash // Command must be defined
#ifndef _Hash_H
#define _Hash_H

// Input structure definition
typedef struct {
    TPM2B_MAX_BUFFER      data;
    TPMI_ALG_HASH         hashAlg;
    TPMI_RH_HIERARCHY     hierarchy;
} Hash_In;

// Output structure definition
typedef struct {
    TPM2B_DIGEST          outHash;
    TPMT_TK_HASHCHECK     validation;
} Hash_Out;

// Response code modifiers
#define    RC_Hash_data         (TPM_RC_P + TPM_RC_1)
#define    RC_Hash_hashAlg      (TPM_RC_P + TPM_RC_2)
#define    RC_Hash_hierarchy    (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_Hash(
    Hash_In *in,
    Hash_Out *out
);


#endif  // _Hash_H
#endif  // TPM_CC_Hash
