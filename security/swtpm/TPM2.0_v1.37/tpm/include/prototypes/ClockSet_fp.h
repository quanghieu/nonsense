/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_ClockSet // Command must be defined
#ifndef _ClockSet_H
#define _ClockSet_H

// Input structure definition
typedef struct {
    TPMI_RH_PROVISION    auth;
    UINT64               newTime;
} ClockSet_In;

// Response code modifiers
#define    RC_ClockSet_auth       (TPM_RC_H + TPM_RC_1)
#define    RC_ClockSet_newTime    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_ClockSet(
    ClockSet_In *in
);


#endif  // _ClockSet_H
#endif  // TPM_CC_ClockSet
