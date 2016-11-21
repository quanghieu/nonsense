/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_NV_ChangeAuth // Command must be defined
#ifndef _NV_ChangeAuth_H
#define _NV_ChangeAuth_H

// Input structure definition
typedef struct {
    TPMI_RH_NV_INDEX    nvIndex;
    TPM2B_AUTH          newAuth;
} NV_ChangeAuth_In;

// Response code modifiers
#define    RC_NV_ChangeAuth_nvIndex     (TPM_RC_H + TPM_RC_1)
#define    RC_NV_ChangeAuth_newAuth     (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_NV_ChangeAuth(
    NV_ChangeAuth_In *in
);


#endif  // _NV_ChangeAuth_H
#endif  // TPM_CC_NV_ChangeAuth
