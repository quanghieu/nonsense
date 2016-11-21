/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_HMAC // Command must be defined
#ifndef _HMAC_H
#define _HMAC_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT      handle;
    TPM2B_MAX_BUFFER    buffer;
    TPMI_ALG_HASH       hashAlg;
} HMAC_In;

// Output structure definition
typedef struct {
    TPM2B_DIGEST        outHMAC;
} HMAC_Out;

// Response code modifiers
#define    RC_HMAC_handle     (TPM_RC_H + TPM_RC_1)
#define    RC_HMAC_buffer     (TPM_RC_P + TPM_RC_1)
#define    RC_HMAC_hashAlg    (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_HMAC(
    HMAC_In *in,
    HMAC_Out *out
);


#endif  // _HMAC_H
#endif  // TPM_CC_HMAC
