/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_NV_Increment // Command must be defined
#ifndef _NV_Increment_H
#define _NV_Increment_H

// Input structure definition
typedef struct {
    TPMI_RH_NV_AUTH     authHandle;
    TPMI_RH_NV_INDEX    nvIndex;
} NV_Increment_In;

// Response code modifiers
#define    RC_NV_Increment_authHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_NV_Increment_nvIndex        (TPM_RC_H + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_NV_Increment(
    NV_Increment_In *in
);


#endif  // _NV_Increment_H
#endif  // TPM_CC_NV_Increment
