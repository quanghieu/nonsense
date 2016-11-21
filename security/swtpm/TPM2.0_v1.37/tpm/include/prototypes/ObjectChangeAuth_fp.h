/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_ObjectChangeAuth // Command must be defined
#ifndef _ObjectChangeAuth_H
#define _ObjectChangeAuth_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT    objectHandle;
    TPMI_DH_OBJECT    parentHandle;
    TPM2B_AUTH        newAuth;
} ObjectChangeAuth_In;

// Output structure definition
typedef struct {
    TPM2B_PRIVATE     outPrivate;
} ObjectChangeAuth_Out;

// Response code modifiers
#define    RC_ObjectChangeAuth_objectHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_ObjectChangeAuth_parentHandle     (TPM_RC_H + TPM_RC_2)
#define    RC_ObjectChangeAuth_newAuth          (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_ObjectChangeAuth(
    ObjectChangeAuth_In *in,
    ObjectChangeAuth_Out *out
);


#endif  // _ObjectChangeAuth_H
#endif  // TPM_CC_ObjectChangeAuth
