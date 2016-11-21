/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PCR_Allocate // Command must be defined
#ifndef _PCR_Allocate_H
#define _PCR_Allocate_H

// Input structure definition
typedef struct {
    TPMI_RH_PLATFORM      authHandle;
    TPML_PCR_SELECTION    pcrAllocation;
} PCR_Allocate_In;

// Output structure definition
typedef struct {
    TPMI_YES_NO           allocationSuccess;
    UINT32                maxPCR;
    UINT32                sizeNeeded;
    UINT32                sizeAvailable;
} PCR_Allocate_Out;

// Response code modifiers
#define    RC_PCR_Allocate_authHandle       (TPM_RC_H + TPM_RC_1)
#define    RC_PCR_Allocate_pcrAllocation    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PCR_Allocate(
    PCR_Allocate_In *in,
    PCR_Allocate_Out *out
);


#endif  // _PCR_Allocate_H
#endif  // TPM_CC_PCR_Allocate
