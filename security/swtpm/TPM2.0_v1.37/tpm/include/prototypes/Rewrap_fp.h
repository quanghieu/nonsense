/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_Rewrap // Command must be defined
#ifndef _Rewrap_H
#define _Rewrap_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT            oldParent;
    TPMI_DH_OBJECT            newParent;
    TPM2B_PRIVATE             inDuplicate;
    TPM2B_NAME                name;
    TPM2B_ENCRYPTED_SECRET    inSymSeed;
} Rewrap_In;

// Output structure definition
typedef struct {
    TPM2B_PRIVATE             outDuplicate;
    TPM2B_ENCRYPTED_SECRET    outSymSeed;
} Rewrap_Out;

// Response code modifiers
#define    RC_Rewrap_oldParent      (TPM_RC_H + TPM_RC_1)
#define    RC_Rewrap_newParent      (TPM_RC_H + TPM_RC_2)
#define    RC_Rewrap_inDuplicate    (TPM_RC_P + TPM_RC_1)
#define    RC_Rewrap_name           (TPM_RC_P + TPM_RC_2)
#define    RC_Rewrap_inSymSeed      (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_Rewrap(
    Rewrap_In *in,
    Rewrap_Out *out
);


#endif  // _Rewrap_H
#endif  // TPM_CC_Rewrap
