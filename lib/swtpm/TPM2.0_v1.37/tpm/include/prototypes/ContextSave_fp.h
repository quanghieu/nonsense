/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_ContextSave // Command must be defined
#ifndef _ContextSave_H
#define _ContextSave_H

// Input structure definition
typedef struct {
    TPMI_DH_CONTEXT    saveHandle;
} ContextSave_In;

// Output structure definition
typedef struct {
    TPMS_CONTEXT       context;
} ContextSave_Out;

// Response code modifiers
#define    RC_ContextSave_saveHandle    (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_ContextSave(
    ContextSave_In *in,
    ContextSave_Out *out
);


#endif  // _ContextSave_H
#endif  // TPM_CC_ContextSave
