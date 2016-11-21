/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_ChangeEPS // Command must be defined
#ifndef _ChangeEPS_H
#define _ChangeEPS_H

// Input structure definition
typedef struct {
    TPMI_RH_PLATFORM    authHandle;
} ChangeEPS_In;

// Response code modifiers
#define    RC_ChangeEPS_authHandle     (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_ChangeEPS(
    ChangeEPS_In *in
);


#endif  // _ChangeEPS_H
#endif  // TPM_CC_ChangeEPS
