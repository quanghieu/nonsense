/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_CertifyCreation // Command must be defined
#ifndef _CertifyCreation_H
#define _CertifyCreation_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT      signHandle;
    TPMI_DH_OBJECT      objectHandle;
    TPM2B_DATA          qualifyingData;
    TPM2B_DIGEST        creationHash;
    TPMT_SIG_SCHEME     inScheme;
    TPMT_TK_CREATION    creationTicket;
} CertifyCreation_In;

// Output structure definition
typedef struct {
    TPM2B_ATTEST        certifyInfo;
    TPMT_SIGNATURE      signature;
} CertifyCreation_Out;

// Response code modifiers
#define    RC_CertifyCreation_signHandle        (TPM_RC_H + TPM_RC_1)
#define    RC_CertifyCreation_objectHandle      (TPM_RC_H + TPM_RC_2)
#define    RC_CertifyCreation_qualifyingData    (TPM_RC_P + TPM_RC_1)
#define    RC_CertifyCreation_creationHash      (TPM_RC_P + TPM_RC_2)
#define    RC_CertifyCreation_inScheme          (TPM_RC_P + TPM_RC_3)
#define    RC_CertifyCreation_creationTicket    (TPM_RC_P + TPM_RC_4)

// Function prototype
TPM_RC
TPM2_CertifyCreation(
    CertifyCreation_In *in,
    CertifyCreation_Out *out
);


#endif  // _CertifyCreation_H
#endif  // TPM_CC_CertifyCreation
