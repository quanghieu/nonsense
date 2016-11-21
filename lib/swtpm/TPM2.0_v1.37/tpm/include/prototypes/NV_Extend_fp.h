/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_NV_Extend // Command must be defined
#ifndef _NV_Extend_H
#define _NV_Extend_H

// Input structure definition
typedef struct {
    TPMI_RH_NV_AUTH        authHandle;
    TPMI_RH_NV_INDEX       nvIndex;
    TPM2B_MAX_NV_BUFFER    data;
} NV_Extend_In;

// Response code modifiers
#define    RC_NV_Extend_authHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_NV_Extend_nvIndex        (TPM_RC_H + TPM_RC_2)
#define    RC_NV_Extend_data           (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_NV_Extend(
    NV_Extend_In *in
);


#endif  // _NV_Extend_H
#endif  // TPM_CC_NV_Extend
