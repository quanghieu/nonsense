/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_HierarchyControl // Command must be defined
#ifndef _HierarchyControl_H
#define _HierarchyControl_H

// Input structure definition
typedef struct {
    TPMI_RH_HIERARCHY    authHandle;
    TPMI_RH_ENABLES      enable;
    TPMI_YES_NO          state;
} HierarchyControl_In;

// Response code modifiers
#define    RC_HierarchyControl_authHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_HierarchyControl_enable         (TPM_RC_P + TPM_RC_1)
#define    RC_HierarchyControl_state          (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_HierarchyControl(
    HierarchyControl_In *in
);


#endif  // _HierarchyControl_H
#endif  // TPM_CC_HierarchyControl
