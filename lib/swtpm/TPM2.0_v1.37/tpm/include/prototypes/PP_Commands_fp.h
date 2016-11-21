/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PP_Commands // Command must be defined
#ifndef _PP_Commands_H
#define _PP_Commands_H

// Input structure definition
typedef struct {
    TPMI_RH_PLATFORM    auth;
    TPML_CC             setList;
    TPML_CC             clearList;
} PP_Commands_In;

// Response code modifiers
#define    RC_PP_Commands_auth         (TPM_RC_H + TPM_RC_1)
#define    RC_PP_Commands_setList      (TPM_RC_P + TPM_RC_1)
#define    RC_PP_Commands_clearList    (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_PP_Commands(
    PP_Commands_In *in
);


#endif  // _PP_Commands_H
#endif  // TPM_CC_PP_Commands
