/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_NV_Certify // Command must be defined
#ifndef _NV_Certify_H
#define _NV_Certify_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT      signHandle;
    TPMI_RH_NV_AUTH     authHandle;
    TPMI_RH_NV_INDEX    nvIndex;
    TPM2B_DATA          qualifyingData;
    TPMT_SIG_SCHEME     inScheme;
    UINT16              size;
    UINT16              offset;
} NV_Certify_In;

// Output structure definition
typedef struct {
    TPM2B_ATTEST        certifyInfo;
    TPMT_SIGNATURE      signature;
} NV_Certify_Out;

// Response code modifiers
#define    RC_NV_Certify_signHandle        (TPM_RC_H + TPM_RC_1)
#define    RC_NV_Certify_authHandle        (TPM_RC_H + TPM_RC_2)
#define    RC_NV_Certify_nvIndex           (TPM_RC_H + TPM_RC_3)
#define    RC_NV_Certify_qualifyingData    (TPM_RC_P + TPM_RC_1)
#define    RC_NV_Certify_inScheme          (TPM_RC_P + TPM_RC_2)
#define    RC_NV_Certify_size              (TPM_RC_P + TPM_RC_3)
#define    RC_NV_Certify_offset            (TPM_RC_P + TPM_RC_4)

// Function prototype
TPM_RC
TPM2_NV_Certify(
    NV_Certify_In *in,
    NV_Certify_Out *out
);


#endif  // _NV_Certify_H
#endif  // TPM_CC_NV_Certify
