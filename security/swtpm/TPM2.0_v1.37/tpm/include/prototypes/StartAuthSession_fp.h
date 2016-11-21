/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_StartAuthSession // Command must be defined
#ifndef _StartAuthSession_H
#define _StartAuthSession_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT            tpmKey;
    TPMI_DH_ENTITY            bind;
    TPM2B_NONCE               nonceCaller;
    TPM2B_ENCRYPTED_SECRET    encryptedSalt;
    TPM_SE                    sessionType;
    TPMT_SYM_DEF              symmetric;
    TPMI_ALG_HASH             authHash;
} StartAuthSession_In;

// Output structure definition
typedef struct {
    TPMI_SH_AUTH_SESSION      sessionHandle;
    TPM2B_NONCE               nonceTPM;
} StartAuthSession_Out;

// Response code modifiers
#define    RC_StartAuthSession_tpmKey           (TPM_RC_H + TPM_RC_1)
#define    RC_StartAuthSession_bind             (TPM_RC_H + TPM_RC_2)
#define    RC_StartAuthSession_nonceCaller      (TPM_RC_P + TPM_RC_1)
#define    RC_StartAuthSession_encryptedSalt    (TPM_RC_P + TPM_RC_2)
#define    RC_StartAuthSession_sessionType      (TPM_RC_P + TPM_RC_3)
#define    RC_StartAuthSession_symmetric        (TPM_RC_P + TPM_RC_4)
#define    RC_StartAuthSession_authHash         (TPM_RC_P + TPM_RC_5)

// Function prototype
TPM_RC
TPM2_StartAuthSession(
    StartAuthSession_In *in,
    StartAuthSession_Out *out
);


#endif  // _StartAuthSession_H
#endif  // TPM_CC_StartAuthSession
