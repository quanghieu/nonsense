/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_NV_Read // Command must be defined
#ifndef _NV_Read_H
#define _NV_Read_H

// Input structure definition
typedef struct {
    TPMI_RH_NV_AUTH        authHandle;
    TPMI_RH_NV_INDEX       nvIndex;
    UINT16                 size;
    UINT16                 offset;
} NV_Read_In;

// Output structure definition
typedef struct {
    TPM2B_MAX_NV_BUFFER    data;
} NV_Read_Out;

// Response code modifiers
#define    RC_NV_Read_authHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_NV_Read_nvIndex        (TPM_RC_H + TPM_RC_2)
#define    RC_NV_Read_size           (TPM_RC_P + TPM_RC_1)
#define    RC_NV_Read_offset         (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_NV_Read(
    NV_Read_In *in,
    NV_Read_Out *out
);


#endif  // _NV_Read_H
#endif  // TPM_CC_NV_Read
