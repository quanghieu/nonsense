/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_NV_GlobalWriteLock // Command must be defined
#ifndef _NV_GlobalWriteLock_H
#define _NV_GlobalWriteLock_H

// Input structure definition
typedef struct {
    TPMI_RH_PROVISION    authHandle;
} NV_GlobalWriteLock_In;

// Response code modifiers
#define    RC_NV_GlobalWriteLock_authHandle     (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_NV_GlobalWriteLock(
    NV_GlobalWriteLock_In *in
);


#endif  // _NV_GlobalWriteLock_H
#endif  // TPM_CC_NV_GlobalWriteLock
