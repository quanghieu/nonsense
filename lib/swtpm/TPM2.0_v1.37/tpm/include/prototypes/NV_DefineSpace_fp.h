/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_NV_DefineSpace // Command must be defined
#ifndef _NV_DefineSpace_H
#define _NV_DefineSpace_H

// Input structure definition
typedef struct {
    TPMI_RH_PROVISION    authHandle;
    TPM2B_AUTH           auth;
    TPM2B_NV_PUBLIC      publicInfo;
} NV_DefineSpace_In;

// Response code modifiers
#define    RC_NV_DefineSpace_authHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_NV_DefineSpace_auth           (TPM_RC_P + TPM_RC_1)
#define    RC_NV_DefineSpace_publicInfo     (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_NV_DefineSpace(
    NV_DefineSpace_In *in
);


#endif  // _NV_DefineSpace_H
#endif  // TPM_CC_NV_DefineSpace
