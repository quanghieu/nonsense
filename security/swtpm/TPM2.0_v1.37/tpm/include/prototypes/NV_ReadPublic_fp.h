/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_NV_ReadPublic // Command must be defined
#ifndef _NV_ReadPublic_H
#define _NV_ReadPublic_H

// Input structure definition
typedef struct {
    TPMI_RH_NV_INDEX    nvIndex;
} NV_ReadPublic_In;

// Output structure definition
typedef struct {
    TPM2B_NV_PUBLIC     nvPublic;
    TPM2B_NAME          nvName;
} NV_ReadPublic_Out;

// Response code modifiers
#define    RC_NV_ReadPublic_nvIndex    (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_NV_ReadPublic(
    NV_ReadPublic_In *in,
    NV_ReadPublic_Out *out
);


#endif  // _NV_ReadPublic_H
#endif  // TPM_CC_NV_ReadPublic
