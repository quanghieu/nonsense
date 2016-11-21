/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_SequenceComplete // Command must be defined
#ifndef _SequenceComplete_H
#define _SequenceComplete_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT        sequenceHandle;
    TPM2B_MAX_BUFFER      buffer;
    TPMI_RH_HIERARCHY     hierarchy;
} SequenceComplete_In;

// Output structure definition
typedef struct {
    TPM2B_DIGEST          result;
    TPMT_TK_HASHCHECK     validation;
} SequenceComplete_Out;

// Response code modifiers
#define    RC_SequenceComplete_sequenceHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_SequenceComplete_buffer             (TPM_RC_P + TPM_RC_1)
#define    RC_SequenceComplete_hierarchy          (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_SequenceComplete(
    SequenceComplete_In *in,
    SequenceComplete_Out *out
);


#endif  // _SequenceComplete_H
#endif  // TPM_CC_SequenceComplete
