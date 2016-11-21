/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_Load // Command must be defined
#ifndef _Load_H
#define _Load_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT    parentHandle;
    TPM2B_PRIVATE     inPrivate;
    TPM2B_PUBLIC      inPublic;
} Load_In;

// Output structure definition
typedef struct {
    TPM_HANDLE        objectHandle;
    TPM2B_NAME        name;
} Load_Out;

// Response code modifiers
#define    RC_Load_parentHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_Load_inPrivate        (TPM_RC_P + TPM_RC_1)
#define    RC_Load_inPublic         (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_Load(
    Load_In *in,
    Load_Out *out
);


#endif  // _Load_H
#endif  // TPM_CC_Load
