/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_EncryptDecrypt // Command must be defined
#ifndef _EncryptDecrypt_H
#define _EncryptDecrypt_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT        keyHandle;
    TPMI_YES_NO           decrypt;
    TPMI_ALG_SYM_MODE     mode;
    TPM2B_IV              ivIn;
    TPM2B_MAX_BUFFER      inData;
} EncryptDecrypt_In;

// Output structure definition
typedef struct {
    TPM2B_MAX_BUFFER      outData;
    TPM2B_IV              ivOut;
} EncryptDecrypt_Out;

// Response code modifiers
#define    RC_EncryptDecrypt_keyHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_EncryptDecrypt_decrypt       (TPM_RC_P + TPM_RC_1)
#define    RC_EncryptDecrypt_mode          (TPM_RC_P + TPM_RC_2)
#define    RC_EncryptDecrypt_ivIn          (TPM_RC_P + TPM_RC_3)
#define    RC_EncryptDecrypt_inData        (TPM_RC_P + TPM_RC_4)

// Function prototype
TPM_RC
TPM2_EncryptDecrypt(
    EncryptDecrypt_In *in,
    EncryptDecrypt_Out *out
);


#endif  // _EncryptDecrypt_H
#endif  // TPM_CC_EncryptDecrypt
