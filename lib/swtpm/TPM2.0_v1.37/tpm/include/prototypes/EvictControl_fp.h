/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_EvictControl // Command must be defined
#ifndef _EvictControl_H
#define _EvictControl_H

// Input structure definition
typedef struct {
    TPMI_RH_PROVISION     auth;
    TPMI_DH_OBJECT        objectHandle;
    TPMI_DH_PERSISTENT    persistentHandle;
} EvictControl_In;

// Response code modifiers
#define    RC_EvictControl_auth                (TPM_RC_H + TPM_RC_1)
#define    RC_EvictControl_objectHandle        (TPM_RC_H + TPM_RC_2)
#define    RC_EvictControl_persistentHandle    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_EvictControl(
    EvictControl_In *in
);


#endif  // _EvictControl_H
#endif  // TPM_CC_EvictControl
