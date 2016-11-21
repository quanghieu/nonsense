/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_Duplicate // Command must be defined
#ifndef _Duplicate_H
#define _Duplicate_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT            objectHandle;
    TPMI_DH_OBJECT            newParentHandle;
    TPM2B_DATA                encryptionKeyIn;
    TPMT_SYM_DEF_OBJECT       symmetricAlg;
} Duplicate_In;

// Output structure definition
typedef struct {
    TPM2B_DATA                encryptionKeyOut;
    TPM2B_PRIVATE             duplicate;
    TPM2B_ENCRYPTED_SECRET    outSymSeed;
} Duplicate_Out;

// Response code modifiers
#define    RC_Duplicate_objectHandle       (TPM_RC_H + TPM_RC_1)
#define    RC_Duplicate_newParentHandle    (TPM_RC_H + TPM_RC_2)
#define    RC_Duplicate_encryptionKeyIn    (TPM_RC_P + TPM_RC_1)
#define    RC_Duplicate_symmetricAlg       (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_Duplicate(
    Duplicate_In *in,
    Duplicate_Out *out
);


#endif  // _Duplicate_H
#endif  // TPM_CC_Duplicate
