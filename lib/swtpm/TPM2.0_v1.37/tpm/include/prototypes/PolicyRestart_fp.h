/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyRestart // Command must be defined
#ifndef _PolicyRestart_H
#define _PolicyRestart_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY    sessionHandle;
} PolicyRestart_In;

// Response code modifiers
#define    RC_PolicyRestart_sessionHandle    (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PolicyRestart(
    PolicyRestart_In *in
);


#endif  // _PolicyRestart_H
#endif  // TPM_CC_PolicyRestart
