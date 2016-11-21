/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_RSA_Decrypt // Command must be defined
#ifndef _RSA_Decrypt_H
#define _RSA_Decrypt_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT          keyHandle;
    TPM2B_PUBLIC_KEY_RSA    cipherText;
    TPMT_RSA_DECRYPT        inScheme;
    TPM2B_DATA              label;
} RSA_Decrypt_In;

// Output structure definition
typedef struct {
    TPM2B_PUBLIC_KEY_RSA    message;
} RSA_Decrypt_Out;

// Response code modifiers
#define    RC_RSA_Decrypt_keyHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_RSA_Decrypt_cipherText    (TPM_RC_P + TPM_RC_1)
#define    RC_RSA_Decrypt_inScheme      (TPM_RC_P + TPM_RC_2)
#define    RC_RSA_Decrypt_label         (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_RSA_Decrypt(
    RSA_Decrypt_In *in,
    RSA_Decrypt_Out *out
);


#endif  // _RSA_Decrypt_H
#endif  // TPM_CC_RSA_Decrypt
