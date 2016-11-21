/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_ActivateCredential // Command must be defined
#ifndef _ActivateCredential_H
#define _ActivateCredential_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT            activateHandle;
    TPMI_DH_OBJECT            keyHandle;
    TPM2B_ID_OBJECT           credentialBlob;
    TPM2B_ENCRYPTED_SECRET    secret;
} ActivateCredential_In;

// Output structure definition
typedef struct {
    TPM2B_DIGEST              certInfo;
} ActivateCredential_Out;

// Response code modifiers
#define    RC_ActivateCredential_activateHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_ActivateCredential_keyHandle          (TPM_RC_H + TPM_RC_2)
#define    RC_ActivateCredential_credentialBlob     (TPM_RC_P + TPM_RC_1)
#define    RC_ActivateCredential_secret             (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_ActivateCredential(
    ActivateCredential_In *in,
    ActivateCredential_Out *out
);


#endif  // _ActivateCredential_H
#endif  // TPM_CC_ActivateCredential
