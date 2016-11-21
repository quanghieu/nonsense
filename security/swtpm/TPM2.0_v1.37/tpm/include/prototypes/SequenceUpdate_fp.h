/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_SequenceUpdate // Command must be defined
#ifndef _SequenceUpdate_H
#define _SequenceUpdate_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT      sequenceHandle;
    TPM2B_MAX_BUFFER    buffer;
} SequenceUpdate_In;

// Response code modifiers
#define    RC_SequenceUpdate_sequenceHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_SequenceUpdate_buffer             (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_SequenceUpdate(
    SequenceUpdate_In *in
);


#endif  // _SequenceUpdate_H
#endif  // TPM_CC_SequenceUpdate
