/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_NV_UndefineSpaceSpecial // Command must be defined
#ifndef _NV_UndefineSpaceSpecial_H
#define _NV_UndefineSpaceSpecial_H

// Input structure definition
typedef struct {
    TPMI_RH_NV_INDEX    nvIndex;
    TPMI_RH_PLATFORM    platform;
} NV_UndefineSpaceSpecial_In;

// Response code modifiers
#define    RC_NV_UndefineSpaceSpecial_nvIndex      (TPM_RC_H + TPM_RC_1)
#define    RC_NV_UndefineSpaceSpecial_platform     (TPM_RC_H + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_NV_UndefineSpaceSpecial(
    NV_UndefineSpaceSpecial_In *in
);


#endif  // _NV_UndefineSpaceSpecial_H
#endif  // TPM_CC_NV_UndefineSpaceSpecial
