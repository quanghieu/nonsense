/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_MakeCredential // Command must be defined
#ifndef _MakeCredential_H
#define _MakeCredential_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT            handle;
    TPM2B_DIGEST              credential;
    TPM2B_NAME                objectName;
} MakeCredential_In;

// Output structure definition
typedef struct {
    TPM2B_ID_OBJECT           credentialBlob;
    TPM2B_ENCRYPTED_SECRET    secret;
} MakeCredential_Out;

// Response code modifiers
#define    RC_MakeCredential_handle        (TPM_RC_H + TPM_RC_1)
#define    RC_MakeCredential_credential    (TPM_RC_P + TPM_RC_1)
#define    RC_MakeCredential_objectName    (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_MakeCredential(
    MakeCredential_In *in,
    MakeCredential_Out *out
);


#endif  // _MakeCredential_H
#endif  // TPM_CC_MakeCredential
