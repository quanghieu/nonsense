/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_HMAC_Start // Command must be defined
#ifndef _HMAC_Start_H
#define _HMAC_Start_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT    handle;
    TPM2B_AUTH        auth;
    TPMI_ALG_HASH     hashAlg;
} HMAC_Start_In;

// Output structure definition
typedef struct {
    TPMI_DH_OBJECT    sequenceHandle;
} HMAC_Start_Out;

// Response code modifiers
#define    RC_HMAC_Start_handle     (TPM_RC_H + TPM_RC_1)
#define    RC_HMAC_Start_auth       (TPM_RC_P + TPM_RC_1)
#define    RC_HMAC_Start_hashAlg    (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_HMAC_Start(
    HMAC_Start_In *in,
    HMAC_Start_Out *out
);


#endif  // _HMAC_Start_H
#endif  // TPM_CC_HMAC_Start
