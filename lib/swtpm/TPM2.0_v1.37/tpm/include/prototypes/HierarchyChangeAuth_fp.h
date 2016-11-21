/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_HierarchyChangeAuth // Command must be defined
#ifndef _HierarchyChangeAuth_H
#define _HierarchyChangeAuth_H

// Input structure definition
typedef struct {
    TPMI_RH_HIERARCHY_AUTH    authHandle;
    TPM2B_AUTH                newAuth;
} HierarchyChangeAuth_In;

// Response code modifiers
#define    RC_HierarchyChangeAuth_authHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_HierarchyChangeAuth_newAuth        (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_HierarchyChangeAuth(
    HierarchyChangeAuth_In *in
);


#endif  // _HierarchyChangeAuth_H
#endif  // TPM_CC_HierarchyChangeAuth
