/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_LoadExternal // Command must be defined
#ifndef _LoadExternal_H
#define _LoadExternal_H

// Input structure definition
typedef struct {
    TPM2B_SENSITIVE       inPrivate;
    TPM2B_PUBLIC          inPublic;
    TPMI_RH_HIERARCHY     hierarchy;
} LoadExternal_In;

// Output structure definition
typedef struct {
    TPM_HANDLE            objectHandle;
    TPM2B_NAME            name;
} LoadExternal_Out;

// Response code modifiers
#define    RC_LoadExternal_inPrivate    (TPM_RC_P + TPM_RC_1)
#define    RC_LoadExternal_inPublic     (TPM_RC_P + TPM_RC_2)
#define    RC_LoadExternal_hierarchy    (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_LoadExternal(
    LoadExternal_In *in,
    LoadExternal_Out *out
);


#endif  // _LoadExternal_H
#endif  // TPM_CC_LoadExternal
