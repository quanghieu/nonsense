/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_FirmwareRead // Command must be defined
#ifndef _FirmwareRead_H
#define _FirmwareRead_H

// Input structure definition
typedef struct {
    UINT32              sequenceNumber;
} FirmwareRead_In;

// Output structure definition
typedef struct {
    TPM2B_MAX_BUFFER    fuData;
} FirmwareRead_Out;

// Response code modifiers
#define    RC_FirmwareRead_sequenceNumber    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_FirmwareRead(
    FirmwareRead_In *in,
    FirmwareRead_Out *out
);


#endif  // _FirmwareRead_H
#endif  // TPM_CC_FirmwareRead
