/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_Clear // Command must be defined
#ifndef _Clear_H
#define _Clear_H

// Input structure definition
typedef struct {
    TPMI_RH_CLEAR    authHandle;
} Clear_In;

// Response code modifiers
#define    RC_Clear_authHandle     (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_Clear(
    Clear_In *in
);


#endif  // _Clear_H
#endif  // TPM_CC_Clear
