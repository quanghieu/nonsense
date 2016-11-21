/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_FlushContext // Command must be defined
#ifndef _FlushContext_H
#define _FlushContext_H

// Input structure definition
typedef struct {
    TPMI_DH_CONTEXT    flushHandle;
} FlushContext_In;

// Response code modifiers
#define    RC_FlushContext_flushHandle    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_FlushContext(
    FlushContext_In *in
);


#endif  // _FlushContext_H
#endif  // TPM_CC_FlushContext
