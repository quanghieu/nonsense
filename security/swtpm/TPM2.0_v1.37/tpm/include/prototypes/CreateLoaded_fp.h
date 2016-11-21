/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_CreateLoaded // Command must be defined
#ifndef _CreateLoaded_H
#define _CreateLoaded_H

// Input structure definition
typedef struct {
    TPMI_DH_PARENT            parentHandle;
    TPM2B_SENSITIVE_CREATE    inSensitive;
    TPM2B_TEMPLATE            inPublic;
} CreateLoaded_In;

// Output structure definition
typedef struct {
    TPM_HANDLE                objectHandle;
    TPM2B_PRIVATE             outPrivate;
    TPM2B_PUBLIC              outPublic;
    TPM2B_NAME                name;
} CreateLoaded_Out;

// Response code modifiers
#define    RC_CreateLoaded_parentHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_CreateLoaded_inSensitive      (TPM_RC_P + TPM_RC_1)
#define    RC_CreateLoaded_inPublic         (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_CreateLoaded(
    CreateLoaded_In *in,
    CreateLoaded_Out *out
);


#endif  // _CreateLoaded_H
#endif  // TPM_CC_CreateLoaded
