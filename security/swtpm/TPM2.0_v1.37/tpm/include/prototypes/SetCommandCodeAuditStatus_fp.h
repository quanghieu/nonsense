/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_SetCommandCodeAuditStatus // Command must be defined
#ifndef _SetCommandCodeAuditStatus_H
#define _SetCommandCodeAuditStatus_H

// Input structure definition
typedef struct {
    TPMI_RH_PROVISION    auth;
    TPMI_ALG_HASH        auditAlg;
    TPML_CC              setList;
    TPML_CC              clearList;
} SetCommandCodeAuditStatus_In;

// Response code modifiers
#define    RC_SetCommandCodeAuditStatus_auth         (TPM_RC_H + TPM_RC_1)
#define    RC_SetCommandCodeAuditStatus_auditAlg     (TPM_RC_P + TPM_RC_1)
#define    RC_SetCommandCodeAuditStatus_setList      (TPM_RC_P + TPM_RC_2)
#define    RC_SetCommandCodeAuditStatus_clearList    (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_SetCommandCodeAuditStatus(
    SetCommandCodeAuditStatus_In *in
);


#endif  // _SetCommandCodeAuditStatus_H
#endif  // TPM_CC_SetCommandCodeAuditStatus
