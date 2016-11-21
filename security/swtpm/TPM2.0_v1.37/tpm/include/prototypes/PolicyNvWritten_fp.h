/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyNvWritten // Command must be defined
#ifndef _PolicyNvWritten_H
#define _PolicyNvWritten_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY    policySession;
    TPMI_YES_NO       writtenSet;
} PolicyNvWritten_In;

// Response code modifiers
#define    RC_PolicyNvWritten_policySession    (TPM_RC_H + TPM_RC_1)
#define    RC_PolicyNvWritten_writtenSet       (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PolicyNvWritten(
    PolicyNvWritten_In *in
);


#endif  // _PolicyNvWritten_H
#endif  // TPM_CC_PolicyNvWritten
