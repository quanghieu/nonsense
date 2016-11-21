/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_EncryptDecrypt2 // Command must be defined
#ifndef _EncryptDecrypt2_H
#define _EncryptDecrypt2_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT        keyHandle;
    TPM2B_MAX_BUFFER      inData;
    TPMI_YES_NO           decrypt;
    TPMI_ALG_SYM_MODE     mode;
    TPM2B_IV              ivIn;
} EncryptDecrypt2_In;

// Output structure definition
typedef struct {
    TPM2B_MAX_BUFFER      outData;
    TPM2B_IV              ivOut;
} EncryptDecrypt2_Out;

// Response code modifiers
#define    RC_EncryptDecrypt2_keyHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_EncryptDecrypt2_inData        (TPM_RC_P + TPM_RC_1)
#define    RC_EncryptDecrypt2_decrypt       (TPM_RC_P + TPM_RC_2)
#define    RC_EncryptDecrypt2_mode          (TPM_RC_P + TPM_RC_3)
#define    RC_EncryptDecrypt2_ivIn          (TPM_RC_P + TPM_RC_4)

// Function prototype
TPM_RC
TPM2_EncryptDecrypt2(
    EncryptDecrypt2_In *in,
    EncryptDecrypt2_Out *out
);


#endif  // _EncryptDecrypt2_H
#endif  // TPM_CC_EncryptDecrypt2
