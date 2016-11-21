/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_GetCapability // Command must be defined
#ifndef _GetCapability_H
#define _GetCapability_H

// Input structure definition
typedef struct {
    TPM_CAP                 capability;
    UINT32                  property;
    UINT32                  propertyCount;
} GetCapability_In;

// Output structure definition
typedef struct {
    TPMI_YES_NO             moreData;
    TPMS_CAPABILITY_DATA    capabilityData;
} GetCapability_Out;

// Response code modifiers
#define    RC_GetCapability_capability       (TPM_RC_P + TPM_RC_1)
#define    RC_GetCapability_property         (TPM_RC_P + TPM_RC_2)
#define    RC_GetCapability_propertyCount    (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_GetCapability(
    GetCapability_In *in,
    GetCapability_Out *out
);


#endif  // _GetCapability_H
#endif  // TPM_CC_GetCapability
