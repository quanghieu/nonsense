/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_Import // Command must be defined
#ifndef _Import_H
#define _Import_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT            parentHandle;
    TPM2B_DATA                encryptionKey;
    TPM2B_PUBLIC              objectPublic;
    TPM2B_PRIVATE             duplicate;
    TPM2B_ENCRYPTED_SECRET    inSymSeed;
    TPMT_SYM_DEF_OBJECT       symmetricAlg;
} Import_In;

// Output structure definition
typedef struct {
    TPM2B_PRIVATE             outPrivate;
} Import_Out;

// Response code modifiers
#define    RC_Import_parentHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_Import_encryptionKey    (TPM_RC_P + TPM_RC_1)
#define    RC_Import_objectPublic     (TPM_RC_P + TPM_RC_2)
#define    RC_Import_duplicate        (TPM_RC_P + TPM_RC_3)
#define    RC_Import_inSymSeed        (TPM_RC_P + TPM_RC_4)
#define    RC_Import_symmetricAlg     (TPM_RC_P + TPM_RC_5)

// Function prototype
TPM_RC
TPM2_Import(
    Import_In *in,
    Import_Out *out
);


#endif  // _Import_H
#endif  // TPM_CC_Import
