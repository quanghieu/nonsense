/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_Startup // Command must be defined
#ifndef _Startup_H
#define _Startup_H

// Input structure definition
typedef struct {
    TPM_SU    startupType;
} Startup_In;

// Response code modifiers
#define    RC_Startup_startupType    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_Startup(
    Startup_In *in
);


#endif  // _Startup_H
#endif  // TPM_CC_Startup
