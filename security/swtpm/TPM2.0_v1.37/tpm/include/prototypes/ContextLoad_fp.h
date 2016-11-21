/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_ContextLoad // Command must be defined
#ifndef _ContextLoad_H
#define _ContextLoad_H

// Input structure definition
typedef struct {
    TPMS_CONTEXT       context;
} ContextLoad_In;

// Output structure definition
typedef struct {
    TPMI_DH_CONTEXT    loadedHandle;
} ContextLoad_Out;

// Response code modifiers
#define    RC_ContextLoad_context    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_ContextLoad(
    ContextLoad_In *in,
    ContextLoad_Out *out
);


#endif  // _ContextLoad_H
#endif  // TPM_CC_ContextLoad
