/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyDuplicationSelect // Command must be defined
#ifndef _PolicyDuplicationSelect_H
#define _PolicyDuplicationSelect_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY    policySession;
    TPM2B_NAME        objectName;
    TPM2B_NAME        newParentName;
    TPMI_YES_NO       includeObject;
} PolicyDuplicationSelect_In;

// Response code modifiers
#define    RC_PolicyDuplicationSelect_policySession    (TPM_RC_H + TPM_RC_1)
#define    RC_PolicyDuplicationSelect_objectName       (TPM_RC_P + TPM_RC_1)
#define    RC_PolicyDuplicationSelect_newParentName    (TPM_RC_P + TPM_RC_2)
#define    RC_PolicyDuplicationSelect_includeObject    (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_PolicyDuplicationSelect(
    PolicyDuplicationSelect_In *in
);


#endif  // _PolicyDuplicationSelect_H
#endif  // TPM_CC_PolicyDuplicationSelect
