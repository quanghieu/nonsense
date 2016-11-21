/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PCR_Reset // Command must be defined
#ifndef _PCR_Reset_H
#define _PCR_Reset_H

// Input structure definition
typedef struct {
    TPMI_DH_PCR    pcrHandle;
} PCR_Reset_In;

// Response code modifiers
#define    RC_PCR_Reset_pcrHandle     (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PCR_Reset(
    PCR_Reset_In *in
);


#endif  // _PCR_Reset_H
#endif  // TPM_CC_PCR_Reset
