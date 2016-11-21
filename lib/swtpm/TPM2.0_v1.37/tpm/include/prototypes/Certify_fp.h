/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_Certify // Command must be defined
#ifndef _Certify_H
#define _Certify_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT      objectHandle;
    TPMI_DH_OBJECT      signHandle;
    TPM2B_DATA          qualifyingData;
    TPMT_SIG_SCHEME     inScheme;
} Certify_In;

// Output structure definition
typedef struct {
    TPM2B_ATTEST        certifyInfo;
    TPMT_SIGNATURE      signature;
} Certify_Out;

// Response code modifiers
#define    RC_Certify_objectHandle      (TPM_RC_H + TPM_RC_1)
#define    RC_Certify_signHandle        (TPM_RC_H + TPM_RC_2)
#define    RC_Certify_qualifyingData    (TPM_RC_P + TPM_RC_1)
#define    RC_Certify_inScheme          (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_Certify(
    Certify_In *in,
    Certify_Out *out
);


#endif  // _Certify_H
#endif  // TPM_CC_Certify
