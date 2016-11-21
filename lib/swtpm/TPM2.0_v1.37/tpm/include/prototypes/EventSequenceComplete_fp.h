/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_EventSequenceComplete // Command must be defined
#ifndef _EventSequenceComplete_H
#define _EventSequenceComplete_H

// Input structure definition
typedef struct {
    TPMI_DH_PCR           pcrHandle;
    TPMI_DH_OBJECT        sequenceHandle;
    TPM2B_MAX_BUFFER      buffer;
} EventSequenceComplete_In;

// Output structure definition
typedef struct {
    TPML_DIGEST_VALUES    results;
} EventSequenceComplete_Out;

// Response code modifiers
#define    RC_EventSequenceComplete_pcrHandle          (TPM_RC_H + TPM_RC_1)
#define    RC_EventSequenceComplete_sequenceHandle     (TPM_RC_H + TPM_RC_2)
#define    RC_EventSequenceComplete_buffer             (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_EventSequenceComplete(
    EventSequenceComplete_In *in,
    EventSequenceComplete_Out *out
);


#endif  // _EventSequenceComplete_H
#endif  // TPM_CC_EventSequenceComplete
